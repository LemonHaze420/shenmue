#include "scn3/sh4/emitter.hpp"
#include "scn3/sh4/native.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/common/util.hpp"

namespace shenmue::scenario {

using namespace sh4;

namespace {

constexpr uint8_t kWidthBytes[] = {0, 1, 2, 4};

std::vector<VmInsn> decodeVm(const std::vector<uint8_t> &vm, uint32_t start, uint32_t end) {
    std::vector<VmInsn> out;
    for (uint32_t off = start; off < end;) {
        const uint8_t opcode = vm[off];

        if (end - off < 4 && std::all_of(vm.begin() + off, vm.begin() + end, [](uint8_t value) { return value == 0xFF; }))
            break;
        if ((opcode & 0xC0U) == kReservedClass)
            throw std::runtime_error("Reserved VM opcode " + hexUnsigned(opcode, 2) + " at " + hexUnsigned(off));
        const uint8_t width = (opcode & 0xC0U) == kAluClass ? 0 : kWidthBytes[immediateWidth(opcode)];
        if (off + 1 + width > end)
            throw std::runtime_error("VM instruction runs past the code segment at " + hexUnsigned(off));
        int32_t operand = 0;
        if (width == 1)
            operand = static_cast<int8_t>(vm[off + 1]);
        else if (width == 2)
            operand = static_cast<int16_t>(readU16(vm, off + 1));
        else if (width == 4)
            operand = static_cast<int32_t>(readU32(vm, off + 1));
        VmInsn insn{.offset = off, .opcode = opcode, .operand = operand, .size = static_cast<uint8_t>(1 + width)};
        if ((opcode & 0xC0U) == 0) {
            const Control control = controlOperation(opcode);
            if (control == Control::Jump || control == Control::BranchFalse)
                insn.target = off + insn.size + static_cast<uint32_t>(operand);
            else if (opcode != 0 && (control == Control::LocalCall || control == Control::ScnfOffset))
                insn.target = static_cast<uint32_t>(operand);
        } else if ((opcode & 0xC0U) == kAddressClass && addressOperation(opcode) == Address::Relative3) {
            insn.target = static_cast<uint32_t>(operand);
        }
        out.push_back(insn);
        off += insn.size;
    }
    return out;
}

std::vector<std::pair<uint32_t, uint32_t>> relocations(const std::vector<uint8_t> &vm, uint32_t offset) {
    std::vector<std::pair<uint32_t, uint32_t>> out;
    if (offset == 0)
        return out;
    if (offset < sizeof(ScenarioHeader) || offset > vm.size())
        throw std::runtime_error("Invalid relocation table offset");
    for (; offset <= vm.size() && vm.size() - offset >= 8; offset += 8) {
        const uint32_t kind = readU32(vm, offset), site = readU32(vm, offset + 4);
        if (!kind && !site)
            break;
        if (kind != kCodeRelocation && kind != kDataRelocation)
            throw std::runtime_error("Unknown relocation kind " + hexUnsigned(kind));
        out.emplace_back(kind, site);
    }
    return out;
}

void put32(std::vector<uint8_t> &out, uint32_t offset, uint32_t value) {
    for (int shift = 0; shift < 32; shift += 8)
        out[offset + shift / 8] = static_cast<uint8_t>(value >> shift);
}

} // namespace

bool isNativeScenario(const std::vector<uint8_t> &blob) {
    return blob.size() >= sizeof(ScenarioHeader) && readU32(blob, 0) == SHEN_NAME(SCN3) && (readU32(blob, 8) & SCF_NativeCodeMask) != 0;
}

std::vector<uint8_t> translateScenarioToSh4(const std::vector<uint8_t> &vm) {
    const ScenarioHeader header = parseHeader(vm);
    if (isNativeScenario(vm))
        throw std::runtime_error("The module is already native SH-4 code");
    const uint32_t codeStart = static_cast<uint32_t>(header.entrypoint), dataStart = static_cast<uint32_t>(header.dataSegmentOffset);
    const uint32_t dataEnd = static_cast<uint32_t>(header.dataEndOffset);
    if (static_cast<uint32_t>(header.token) != SHEN_NAME(SCN3) || codeStart < sizeof(ScenarioHeader) || codeStart >= dataStart || dataStart > dataEnd ||
        dataEnd > vm.size() || static_cast<uint32_t>(header.functionOffset) < codeStart || static_cast<uint32_t>(header.functionOffset) >= dataStart)
        throw std::runtime_error("Invalid VM module offsets");
    const std::vector<VmInsn> code = decodeVm(vm, codeStart, dataStart);
    const auto relocs = relocations(vm, static_cast<uint32_t>(header.relocationTableOffset));

    std::set<uint32_t> targets, starts{codeStart, static_cast<uint32_t>(header.functionOffset)};
    std::vector<uint32_t> entries;
    const auto addEntry = [&](uint32_t target) {
        if (std::find(entries.begin(), entries.end(), target) == entries.end())
            entries.push_back(target);
    };
    for (const VmInsn &insn : code) {
        const Control control = controlOperation(insn.opcode);
        if ((insn.opcode & 0xC0U) == 0 && (control == Control::Jump || control == Control::BranchFalse))
            targets.insert(insn.target);
        if (insn.opcode == encode(Control::LocalCall, 3) || (insn.opcode == encode(Address::Relative3, 3) && insn.target < dataStart)) {
            starts.insert(insn.target);
            addEntry(insn.target);
        }
    }
    for (const auto &[kind, site] : relocs) {
        if (site < dataStart || site > dataEnd || dataEnd - site < 4)
            throw std::runtime_error("Relocation outside the data segment at " + hexUnsigned(site));
        if (kind == kCodeRelocation)
            starts.insert(readU32(vm, site));
    }
    for (size_t i = 1; i < code.size(); ++i)
        if ((code[i].opcode & kReservedClass) == 0 && controlOperation(code[i].opcode) == Control::AdjustStack &&
            (code[i - 1].opcode == encode(Control::Return) || controlOperation(code[i - 1].opcode) == Control::Jump) && (code[i - 1].opcode & 0xC0U) == 0 &&
            !targets.count(code[i].offset))
            starts.insert(code[i].offset);

    struct Function {
        uint32_t vmStart;
        std::vector<Item> items;
        uint32_t native = 0;
    };
    std::vector<Function> functions;
    std::unique_ptr<FunctionTranslator> translator;
    for (const VmInsn &insn : code) {
        if (starts.count(insn.offset)) {
            if (translator)
                functions.back().items = std::move(translator->items);
            if ((insn.opcode & 0xCFU) != 0x03)
                throw std::runtime_error("VM function at " + hexUnsigned(insn.offset) + " does not open a frame");
            translator = std::make_unique<FunctionTranslator>(-insn.operand, dataStart);
            translator->prologue();
            functions.push_back(Function{.vmStart = insn.offset});
            continue;
        }
        if (!translator)
            throw std::runtime_error("VM code before the first function at " + hexUnsigned(insn.offset));
        translator->items.push_back(
            Item{.kind = Item::Kind::Boundary, .value = insn.offset, .depth = static_cast<uint8_t>(translator->cache.size()), .opcode = insn.opcode});
        translator->translate(insn);
    }
    if (translator)
        functions.back().items = std::move(translator->items);

    std::map<uint32_t, uint32_t> native;
    uint32_t pc = kCodeOffset;
    for (Function &function : functions) {
        function.native = pc;
        native[function.vmStart] = pc;
        pc += functionSize(function.items, pc);
    }
    const AddressOf nativeFunction = [&](uint32_t vmOffset) {
        const auto found = native.find(vmOffset);
        if (found == native.end())
            throw std::runtime_error("No native function for VM offset " + hexUnsigned(vmOffset));
        return found->second;
    };

    const uint32_t nativeDataStart = pc;
    const uint32_t nativeDataEnd = nativeDataStart + (dataEnd - dataStart) + 4 * static_cast<uint32_t>(entries.size());
    std::vector<uint8_t> out(nativeDataEnd, 0);
    put32(out, 0x00, SHEN_NAME(SCN3));
    put32(out, 0x04, nativeDataEnd);
    put32(out, 0x08, kNativeFlags);
    put32(out, 0x0C, nativeFunction(static_cast<uint32_t>(header.functionOffset)));
    put32(out, 0x10, nativeDataStart);
    put32(out, 0x14, nativeDataEnd);
    put32(out, 0x18, static_cast<uint32_t>(header.workspaceSize));
    put32(out, 0x20, kStubOffset);
    const std::vector<uint8_t> stub = stubBytes();
    std::copy(stub.begin(), stub.end(), out.begin() + kStubOffset);
    for (const Function &function : functions) {
        const FunctionLayout layout = layoutFunction(function.items, function.native, nativeFunction);
        std::copy(layout.bytes.begin(), layout.bytes.end(), out.begin() + function.native);
    }
    std::copy(vm.begin() + dataStart, vm.begin() + dataEnd, out.begin() + nativeDataStart);
    for (const auto &[kind, site] : relocs) {
        if (site < dataStart || site + 4 > dataEnd)
            throw std::runtime_error("Relocation outside the data segment at " + hexUnsigned(site));
        const uint32_t value = readU32(vm, site);
        put32(out, nativeDataStart + (site - dataStart), kind == kCodeRelocation ? nativeFunction(value) : value + nativeDataStart - dataStart);
    }
    for (size_t i = 0; i < entries.size(); ++i)
        put32(out, nativeDataStart + (dataEnd - dataStart) + 4 * static_cast<uint32_t>(i), nativeFunction(entries[i]));
    return out;
}

} // namespace shenmue::scenario
