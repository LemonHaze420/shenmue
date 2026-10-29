#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/parser.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

void SourceParser::parseLoweringBlock() {
    PendingSourceFunction pending{};
    const std::string name = expectIdentifier();
    if (!matchIdentifier("at"))
        fail("Assembly function requires at(offset)");
    expectPunct("(");
    pending.function.start = expectNumber();
    expectPunct(")");
    if (!symbolOffsets_.emplace(name, pending.function.start).second)
        fail("Duplicate assembly symbol: " + name);
    pending.function.baseOffset = 0;
    if (matchIdentifier("reason")) {
        expectPunct("(");
        pending.function.reason = parseDiscoveryReasonName(expectIdentifier());
        expectPunct(")");
    }
    if (matchIdentifier("base")) {
        expectPunct("(");
        pending.function.baseOffset = expectNumber();
        expectPunct(")");
    }
    expectPunct("{");
    while (!matchPunct("}")) {
        std::optional<std::string> label;
        if (current_.kind == SourceTokenKind::Identifier) {
            label = expectIdentifier();
            expectPunct(":");
        }
        PendingSourceInsn instruction = parseInstruction();
        if (label.has_value() && !symbolOffsets_.emplace(*label, instruction.insn.offset).second)
            fail("Duplicate assembly symbol: " + *label);
        pending.instructions.push_back(std::move(instruction));
    }
    pendingFunctions_.push_back(std::move(pending));
}

PendingSourceInsn SourceParser::parseInstruction() {
    expectPunct("@");
    PendingSourceInsn pending{};
    pending.insn.offset = expectNumber();
    const std::string op = expectIdentifier();
    const std::optional<uint8_t> opcode = opcodeForMnemonic(op);
    if (opcode && *opcode < kAluClass) {
        pending.insn.cls = *opcode < kAddressClass ? InstrClass::Ctrl : InstrClass::Addr;
        parseOptionalImmediate(pending);
        pending.insn.opcode = static_cast<uint8_t>(*opcode | pending.insn.immWidth << 4);
    } else if (opcode || op.rfind("alu.op", 0) == 0) {
        pending.insn.cls = InstrClass::Alu;
        pending.insn.immWidth = 0;
        pending.insn.opcode = opcode ? *opcode : static_cast<uint8_t>(kAluClass | (std::stoul(op.substr(6), nullptr, 16) & 0x3Fu));
    } else if (op.rfind("unknown.op", 0) == 0) {
        pending.insn.cls = InstrClass::Unknown;
        pending.insn.opcode = static_cast<uint8_t>(std::stoul(op.substr(10), nullptr, 16) & 0xFFu);
        pending.insn.immWidth = 0;
        pending.insn.originalBytes = {pending.insn.opcode};
    } else {
        fail("Unknown instruction op: " + op);
    }
    expectPunct(";");
    if (pending.insn.cls != InstrClass::Unknown) {
        const uint32_t size = 1 + immWidthBytes(pending.insn.immWidth);
        pending.insn.originalBytes.assign(size, 0);
        if (!pending.operandSymbol.has_value()) {
            pending.insn.originalBytes = encodeLowLevelInsn(pending.insn);
        }
    }
    return pending;
}

void SourceParser::parseOptionalImmediate(PendingSourceInsn &pending) {
    if (current_.kind != SourceTokenKind::Identifier || current_.text.rfind("imm", 0) != 0) {
        pending.insn.immWidth = 0;
        pending.insn.imm = std::nullopt;
        return;
    }
    const std::string immName = expectIdentifier();
    pending.insn.immWidth = sourceImmWidthFromName(immName);
    expectPunct("(");
    if (current_.kind == SourceTokenKind::Identifier) {
        pending.operandSymbol = expectIdentifier();
        pending.insn.imm = 0;
    } else {
        pending.insn.imm = expectNumber();
    }
    expectPunct(")");
}

Scn3Module SourceParser::buildModule() {
    if ((static_cast<uint32_t>(header_.flags) & SCF_NativeCodeMask) != 0)
        fail("VM assembly cannot emit native SH-4 SCN3");
    if (!sawHeader_) {
        fail("Missing scn3_header");
    }
    if (!dataBlock_.has_value()) {
        fail("Missing data_segment block");
    }
    if (!trailingBlock_.has_value()) {
        fail("Missing trailing_data_or_unknown block");
    }
    SourceUnit data = buildSourceUnit();
    dataBlock_->bytes = std::move(data.dataSegment);
    trailingBlock_->bytes = std::move(data.trailingData);
    rawBlocks_ = std::move(data.rawBlocks);

    Scn3Module module{};
    const uint32_t dataStart = header_.dataSegmentOffset;
    const uint32_t dataEnd = dataStart + static_cast<uint32_t>(dataBlock_->bytes.size());
    const uint32_t trailingEnd = dataEnd + static_cast<uint32_t>(trailingBlock_->bytes.size());
    header_.wholeSize = trailingEnd;
    header_.dataEndOffset = dataEnd;
    header_.relocationTableOffset = dataEnd;
    if (startupFunctionSymbol_.has_value()) {
        const std::map<std::string, std::uint32_t>::const_iterator entry = symbolOffsets_.find(*startupFunctionSymbol_);
        if (entry == symbolOffsets_.end())
            fail("Unknown assembly entry function: " + *startupFunctionSymbol_);
        header_.functionOffset = entry->second;
    }
    module.rawHeader = header_;
    module.parsedHeader = parseHeaderSemantics(header_);
    module.originalBlob.assign(trailingEnd, 0);
    module.byteOwners.assign(trailingEnd, RangeKind::Unowned);

    appendLayoutRange(module, 0, std::min<uint32_t>(sizeof(ScenarioHeader), trailingEnd), RangeKind::Header);

    for (PendingSourceFunction &pending : pendingFunctions_) {
        LowLevelFunction function = pending.function;
        for (PendingSourceInsn &pendingInsn : pending.instructions) {
            if (pendingInsn.operandSymbol.has_value()) {
                const std::string &symbol = *pendingInsn.operandSymbol;
                const Control control = controlOperation(pendingInsn.insn.opcode);
                const bool branch = pendingInsn.insn.cls == InstrClass::Ctrl && (control == Control::Jump || control == Control::BranchFalse);
                std::uint32_t immediate = 0;
                const bool commandOperand = pendingInsn.insn.cls == InstrClass::Ctrl && nativeCall(control);
                if (commandOperand) {
                    const abi::FamilySpec *family = nullptr;
                    for (const abi::FamilySpec &candidate : abi::kFamilies) {
                        if (candidate.opcodeFamily == static_cast<std::uint8_t>(control)) {
                            family = &candidate;
                            break;
                        }
                    }
                    const std::optional<std::uint32_t> command = family ? lookupAbiBytecodeIndexByName(family->family, symbol) : std::nullopt;
                    if (!command.has_value())
                        fail("Unknown assembly command: " + symbol);
                    immediate = *command;
                } else {
                    const std::map<std::string, std::uint32_t>::const_iterator target = symbolOffsets_.find(symbol);
                    if (target == symbolOffsets_.end())
                        fail("Unknown assembly symbol: " + symbol);
                    if (branch) {
                        const std::int64_t displacement =
                            static_cast<std::int64_t>(target->second) - (pendingInsn.insn.offset + 1U + immWidthBytes(pendingInsn.insn.immWidth));
                        const std::uint8_t width = pendingInsn.insn.immWidth;
                        if ((width == 1 && (displacement < -128 || displacement > 127)) || (width == 2 && (displacement < -32768 || displacement > 32767)) ||
                            displacement < std::numeric_limits<std::int32_t>::min() || displacement > std::numeric_limits<std::int32_t>::max())
                            fail("Assembly branch does not fit its immediate width: " + symbol);
                        immediate = static_cast<std::uint32_t>(displacement);
                    } else {
                        immediate = target->second - function.baseOffset;
                    }
                }
                if (!branch && ((pendingInsn.insn.immWidth == 1 && immediate > 0xFFU) || (pendingInsn.insn.immWidth == 2 && immediate > 0xFFFFU)))
                    fail("Assembly operand does not fit its immediate width: " + symbol);
                pendingInsn.insn.imm = immediate;
                pendingInsn.insn.originalBytes = encodeLowLevelInsn(pendingInsn.insn);
            }
            function.instructions.push_back(pendingInsn.insn);
            const uint32_t end = pendingInsn.insn.offset + static_cast<uint32_t>(pendingInsn.insn.originalBytes.size());
            appendLayoutRange(module, pendingInsn.insn.offset, end, RangeKind::DecodedInstruction, function.start);
        }
        module.functions.push_back(std::move(function));
    }

    auto writeRawBlock = [&](const SourceRawBlock &block, uint32_t startOverride, RangeKind kind) {
        const uint32_t start = startOverride;
        const uint32_t end = start + static_cast<uint32_t>(block.bytes.size());
        if (end > module.originalBlob.size()) {
            throw std::runtime_error("Source raw/data block extends past rebuilt file");
        }
        std::copy(block.bytes.begin(), block.bytes.end(), module.originalBlob.begin() + start);
        appendLayoutRange(module, start, end, kind);
    };

    for (const SourceRawBlock &block : rawBlocks_) {
        writeRawBlock(block, block.declaredStart, block.kind);
    }
    writeRawBlock(*dataBlock_, dataStart, RangeKind::DataSegment);
    writeRawBlock(*trailingBlock_, dataEnd, RangeKind::TrailingDataOrUnknown);

    std::sort(module.functions.begin(), module.functions.end(), [](const LowLevelFunction &lhs, const LowLevelFunction &rhs) { return lhs.start < rhs.start; });
    std::sort(module.layoutRanges.begin(), module.layoutRanges.end(), [](const ByteRange &lhs, const ByteRange &rhs) {
        if (lhs.start != rhs.start)
            return lhs.start < rhs.start;
        return lhs.end < rhs.end;
    });
    std::sort(module.rawRanges.begin(), module.rawRanges.end(), [](const ByteRange &lhs, const ByteRange &rhs) {
        if (lhs.start != rhs.start)
            return lhs.start < rhs.start;
        return lhs.end < rhs.end;
    });
    if (unownedByteCount(module) != 0) {
        throw std::runtime_error("Compiled source left " + std::to_string(unownedByteCount(module)) + " unowned bytes");
    }
    return module;
}

} // namespace shenmue::scenario
