#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/decompiler/data.hpp"
#include "scn3/decompiler/export.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/data.hpp"
#include "scn3/native/lookup.hpp"
namespace shenmue::scenario {

static std::string assemblyMnemonic(const LowLevelInsn &instruction) {
    if (const Operation *operation = operationOf(instruction.opcode); operation && operation->mnemonic)
        return operation->mnemonic;
    if (instruction.cls == InstrClass::Alu)
        return "alu.op" + hexName(static_cast<uint32_t>(aluOperation(instruction.opcode)), 2);
    return "unknown.op" + hexName(instruction.opcode, 2);
}

static std::string assemblyOperand(const LowLevelInsn &instruction, const LowLevelFunction &function, const std::set<std::uint32_t> &labels,
    const std::map<std::uint32_t, std::string> &dataSymbols, const std::string &profile) {
    if (instruction.relativeTarget.has_value() && labels.count(*instruction.relativeTarget))
        return "L" + hexName(*instruction.relativeTarget, 6);
    if (instruction.localTarget.has_value() && labels.count(*instruction.localTarget))
        return "L" + hexName(*instruction.localTarget, 6);
    const std::uint32_t value = instruction.imm.value_or(0);
    const Control control = controlOperation(instruction.opcode);
    const Address address = addressOperation(instruction.opcode);
    if ((instruction.cls == InstrClass::Addr && address >= Address::Relative1 && address <= Address::Relative3) ||
        (instruction.cls == InstrClass::Ctrl && control == Control::ScnfOffset)) {
        const std::map<std::uint32_t, std::string>::const_iterator symbol = dataSymbols.find(function.baseOffset + value);
        if (symbol != dataSymbols.end())
            return symbol->second;
    }
    if (instruction.cls == InstrClass::Ctrl && nativeCall(control) && profile == "s1-pc") {
        if (value <= std::numeric_limits<std::uint16_t>::max()) {
            const abi::CommandSpec *command = abi::findCommandByDispatch(static_cast<std::uint8_t>(control), static_cast<std::uint16_t>(value), 0);
            if (command && command->handlerName)
                return lookupAbiName(command->family, command->publicIndex, "command");
        }
    }
    return hexUnsigned(value);
}

void exportScenarioAssembly(const Program &program, const std::string &path, const std::string &profile) {
    const Scn3Module module = buildScn3Module(program);
    const std::uint32_t dataStart = module.parsedHeader.dataSegmentOffset;
    const std::uint32_t dataEnd = moduleDataEnd(module);
    const std::uint32_t fileEnd = static_cast<std::uint32_t>(module.originalBlob.size());
    std::set<std::uint32_t> references = collectSourceDataReferenceOffsets(module);
    const std::vector<ScenarioDataView> typedViews =
        profile == "s1-pc" ? discoverScenarioDataViews(program, module, references) : std::vector<ScenarioDataView>{};
    const std::vector<SourceDataDecl> dataDeclarations = planSourceDataDecls(module.originalBlob, dataStart, dataEnd, references, typedViews);
    const std::vector<SourceDataDecl> trailingDeclarations = planSourceDataDecls(module.originalBlob, dataEnd, fileEnd, {});
    const std::map<std::uint32_t, std::string> dataSymbols = sourceDataSymbolMap(dataDeclarations);
    std::set<std::uint32_t> labels;
    for (const LowLevelFunction &function : module.functions) {
        for (const LowLevelInsn &instruction : function.instructions)
            labels.insert(instruction.offset);
    }

    std::ofstream out(path);
    if (!out)
        throw std::runtime_error("Failed to open assembly output: " + path);
    out << "scn3_assembly 1;\n\n";
    writeScenarioHeader(out, module);
    for (const LowLevelFunction &function : module.functions) {
        out << "\nlowering " << scriptFunctionName(function.start) << " at(" << hexUnsigned(function.start) << ")";
        out << " reason(" << discoveryReasonName(function.reason) << ") base(" << hexUnsigned(function.baseOffset) << ") {\n";
        for (const LowLevelInsn &instruction : function.instructions) {
            out << "L" << hexName(instruction.offset, 6) << ":\n";
            out << "  @ " << hexUnsigned(instruction.offset, 6) << " " << assemblyMnemonic(instruction);
            if ((instruction.cls == InstrClass::Ctrl || instruction.cls == InstrClass::Addr) && instruction.immWidth)
                out << " imm" << immWidthBytes(instruction.immWidth) * 8 << "(" << assemblyOperand(instruction, function, labels, dataSymbols, profile) << ")";
            out << ";\n";
        }
        out << "}\n";
    }
    for (const ByteRange &range : module.rawRanges) {
        if (range.kind != RangeKind::DataSegment && range.kind != RangeKind::TrailingDataOrUnknown && range.kind != RangeKind::Header)
            renderSourceRawRange(out, module, range);
    }
    out << "\ndata_segment " << hexUnsigned(dataStart, 6) << " " << hexUnsigned(dataEnd, 6) << " {\n";
    renderSourceDataDecls(out, module.originalBlob, dataDeclarations, "  ");
    out << "}\n\nmodule_data " << hexUnsigned(dataEnd, 6) << " " << hexUnsigned(fileEnd, 6) << " {\n";
    renderSourceDataDecls(out, module.originalBlob, trailingDeclarations, "  ");
    out << "}\n";
    if (!out)
        throw std::runtime_error("Failed to write assembly output: " + path);
}

} // namespace shenmue::scenario
