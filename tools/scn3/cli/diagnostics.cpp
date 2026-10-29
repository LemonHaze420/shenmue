#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/cli/diagnostics.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/native/abi.hpp"
#include "scn3/native/lookup.hpp"

namespace shenmue::scenario {

size_t countLocalCallInstructions(const Program &program) {
    size_t count = 0;
    for (const auto &[_, function] : program.functions) {
        for (const Instruction &insn : function.instructions) {
            if (insn.cls == InstrClass::Ctrl && controlOperation(insn.opcode) == Control::LocalCall && insn.hasLocalTarget) {
                ++count;
            }
        }
    }
    return count;
}

size_t countCallsPublicIndex(const Program &program, abi::Family family, uint16_t publicIndex) {
    size_t count = 0;
    for (const auto &[_, function] : program.functions) {
        for (const Instruction &insn : function.instructions) {
            const Control control = controlOperation(insn.opcode);
            if (insn.cls == InstrClass::Ctrl && (control == Control::ScnfOffset || nativeCall(control)) &&
                abiCallIsPublicIndex(callFamilyName(insn), family, publicIndex)) {
                ++count;
            }
        }
    }
    return count;
}

size_t countRenderedFunctions(const Program &program) {
    const std::set<uint32_t> referencedStarts = collectReferencedFunctionStarts(program);
    size_t count = 0;
    for (const auto &[_, function] : program.functions) {
        if (shouldRenderFunction(function, referencedStarts)) {
            ++count;
        }
    }
    return count;
}

std::map<DiscoveryReason, size_t> countFunctionsByDiscoveryReason(const Program &program) {
    std::map<DiscoveryReason, size_t> counts;
    counts[DiscoveryReason::FunctionPoolStart] = 0;
    counts[DiscoveryReason::StartupEntry] = 0;
    counts[DiscoveryReason::LocalCall] = 0;
    counts[DiscoveryReason::TaskCreated] = 0;
    counts[DiscoveryReason::PackedNext] = 0;
    counts[DiscoveryReason::DecodeUncovered] = 0;
    for (const auto &[_, function] : program.functions) {
        ++counts[function.discoveryReason];
    }
    return counts;
}

size_t uncoveredByteCount(const Program &program) {
    size_t count = 0;
    for (const auto &[start, end] : program.uncoveredCodeRanges) {
        count += end - start;
    }
    return count;
}

size_t packedPaddingByteCount(const Program &program) {
    size_t count = 0;
    for (const auto &[start, end] : program.discardedPackedPaddingRanges) {
        count += end - start;
    }
    return count;
}

size_t countUnreferencedPackedNextFunctions(const Program &program) {
    const std::set<uint32_t> referencedStarts = collectReferencedFunctionStarts(program);
    size_t count = 0;
    for (const auto &[_, function] : program.functions) {
        if (function.discoveryReason == DiscoveryReason::PackedNext && referencedStarts.count(function.start) == 0) {
            ++count;
        }
    }
    return count;
}

void renderAudit(const Program &program) {
    const Scn3Module module = buildScn3Module(program);
    const std::map<RangeKind, size_t> bytesByKind = countBytesByRangeKind(module);
    std::cout << "codeStart=" << hexUnsigned(program.codeStart) << "\n";
    std::cout << "codeEnd=" << hexUnsigned(program.codeEnd) << "\n";
    std::cout << "functionPoolStart=" << hexUnsigned(program.parsedHeader.functionPoolStart) << "\n";
    std::cout << "startupFunctionOffset=" << hexUnsigned(program.parsedHeader.startupFunctionOffset) << "\n";
    std::cout << "dataEnd=" << hexUnsigned(parsedDataEnd(program)) << "\n";
    std::cout << "runtime_roots=" << hexUnsigned(program.parsedHeader.startupFunctionOffset) << "\n";
    std::cout << "decoded_functions=" << program.functions.size() << "\n";
    std::cout << "functions_discovered=" << program.functions.size() << "\n";
    std::cout << "rendered_functions=" << countRenderedFunctions(program) << "\n";
    for (const auto &[reason, count] : countFunctionsByDiscoveryReason(program)) {
        std::cout << "discovery_reason_" << discoveryReasonName(reason) << "=" << count << "\n";
    }
    std::cout << "unreferenced_packed_next_functions=" << countUnreferencedPackedNextFunctions(program) << "\n";
    std::cout << "local_calls_discovered=" << countLocalCallInstructions(program) << "\n";
    std::cout << "task_created_functions=" << program.scriptCreatedTargets.size() << "\n";
    std::cout << "Script_CreateScenarioTask_calls=" << countCallsPublicIndex(program, abi::Family::Shenmue, 2) << "\n";
    std::cout << "Script_WaitFrames_calls=" << countCallsPublicIndex(program, abi::Family::SCNF, 0) << "\n";
    std::cout << "reserved_c0_opcodes=" << program.reservedOpcodeHits.size() << "\n";
    for (const auto &[offset, opcode] : program.reservedOpcodeHits) {
        std::cout << "reserved_c0_opcode offset=" << hexUnsigned(offset) << " opcode=" << hexUnsigned(opcode, 2) << "\n";
    }
    std::cout << "discarded_packed_padding_ranges=" << program.discardedPackedPaddingRanges.size() << "\n";
    std::cout << "discarded_packed_padding_bytes=" << packedPaddingByteCount(program) << "\n";
    for (const auto &[start, end] : program.discardedPackedPaddingRanges) {
        std::cout << "discarded_packed_padding_range start=" << hexUnsigned(start) << " end=" << hexUnsigned(end) << " size=" << (end - start) << "\n";
    }
    std::cout << "raw_code_gap_bytes=" << bytesByKind.at(RangeKind::RawCodeGap) << "\n";
    std::cout << "tail_merged_stub_candidate_bytes=" << bytesByKind.at(RangeKind::TailMergedStubCandidate) << "\n";
    std::cout << "packed_padding_bytes=" << bytesByKind.at(RangeKind::PackedPadding) << "\n";
    std::cout << "data_segment_bytes=" << bytesByKind.at(RangeKind::DataSegment) << "\n";
    std::cout << "trailing_data_or_unknown_bytes=" << bytesByKind.at(RangeKind::TrailingDataOrUnknown) << "\n";
    std::cout << "unowned_bytes=" << bytesByKind.at(RangeKind::Unowned) << "\n";
    std::cout << "uncovered_ranges=" << program.uncoveredCodeRanges.size() << "\n";
    std::cout << "uncovered_bytes=" << uncoveredByteCount(program) << "\n";
    for (const auto &[start, end] : program.uncoveredCodeRanges) {
        std::cout << "uncovered_range start=" << hexUnsigned(start) << " end=" << hexUnsigned(end) << " size=" << (end - start) << "\n";
    }
}

int stackDeltaWords(const Instruction &insn) {
    if (insn.cls == InstrClass::Addr) {
        return 1;
    }
    if (insn.cls == InstrClass::Alu) {
        const Alu sub = aluOperation(insn.opcode);
        switch (sub) {
        case Alu::PopEax:
            return -1;
        case Alu::PushEax:
            return 1;
        case Alu::Load8:
        case Alu::Load16:
        case Alu::Load32:
        case Alu::Not:
        case Alu::IntToFloat:
        case Alu::FloatToInt:
            return 0;
        case Alu::Store8:
        case Alu::Store16:
        case Alu::Store32:
        case Alu::Equal:
        case Alu::NotEqual:
        case Alu::Less:
        case Alu::LessEqual:
        case Alu::Greater:
        case Alu::GreaterEqual:
        case Alu::And:
        case Alu::Or:
        case Alu::Xor:
        case Alu::Add:
        case Alu::Subtract:
        case Alu::Multiply:
        case Alu::Divide:
        case Alu::Modulo:
        case Alu::ShiftLeft:
        case Alu::ShiftRight:
        case Alu::FloatLess:
        case Alu::FloatLessEqual:
        case Alu::FloatGreater:
        case Alu::FloatGreaterEqual:
        case Alu::FloatAdd:
        case Alu::FloatSubtract:
        case Alu::FloatMultiply:
        case Alu::FloatDivide:
            return -1;
        default:
            return 0;
        }
    }
    return 0;
}

std::string irOpName(const Instruction &insn) {
    if (insn.cls == InstrClass::Addr) {
        return insn.addrKind == AddrKind::Literal ? "PushConst" : "PushExpr";
    }
    if (insn.cls == InstrClass::Alu) {
        const Alu sub = aluOperation(insn.opcode);
        if (sub == Alu::PopEax)
            return "PopEax";
        if (sub == Alu::PushEax)
            return "PushEax";
        if (sub >= Alu::Load8 && sub <= Alu::Load32)
            return "Load" + std::to_string(1u << (static_cast<uint32_t>(sub) - static_cast<uint32_t>(Alu::Load8)));
        if (sub >= Alu::Store8 && sub <= Alu::Store32)
            return "Store" + std::to_string(1u << (static_cast<uint32_t>(sub) - static_cast<uint32_t>(Alu::Store8)));
        if (comparisonOperatorForAlu(sub).has_value())
            return "CompareOp";
        if (sub == Alu::Not || sub == Alu::IntToFloat || sub == Alu::FloatToInt)
            return "UnaryOp";
        return "BinaryOp";
    }
    if (insn.cls == InstrClass::Ctrl) {
        switch (controlOperation(insn.opcode)) {
        case Control::Return:
            return "ReturnFromLocalCall";
        case Control::LocalCall:
            return "LocalCall";
        case Control::IndirectCall:
            return "CallIndirectLocal";
        case Control::AdjustStack:
            return "AdjustStack";
        case Control::LocalJump:
            return "JumpLocal";
        case Control::Jump:
            return "JumpRelative";
        case Control::BranchFalse:
            return "BranchIfFalse";
        case Control::LatchOperand:
            return "LatchOperand";
        case Control::ScnfOffset:
            return "CallSCNFByOffset";
        case Control::CallLegacy:
            return "CallLegacyFunc";
        case Control::CallGp:
            return "CallGeneralPurposeFunc";
        case Control::CallShenmue:
            return "CallShenmueScriptFunc";
        case Control::CallScnf:
            return "CallSCNF";
        case Control::CallDswk:
            return "CallDSWKFunc";
        default:
            return "UnknownRaw";
        }
    }
    return "UnknownRaw";
}

void renderDumpIr(const Program &program, uint32_t start) {
    const std::map<uint32_t, FunctionInfo>::const_iterator it = program.functions.find(start);
    if (it == program.functions.end()) {
        throw std::runtime_error("No decoded function at " + hexUnsigned(start));
    }
    const FunctionInfo &function = it->second;
    const std::vector<VmCallSite> calls = analyzeVmCalls(program, function);
    std::cout << "function " << formatScriptFunctionName(program, function.start) << " start=" << hexUnsigned(function.start) << "\n";
    for (const Instruction &insn : function.instructions) {
        std::cout << hexUnsigned(insn.offset) << " " << irOpName(insn) << " opcode=" << hexUnsigned(insn.opcode, 2)
                  << " size=" << static_cast<unsigned>(insn.size) << " stack_delta=" << stackDeltaWords(insn);
        if (insn.imm.has_value()) {
            std::cout << " imm=" << hexUnsigned(*insn.imm);
        }
        const Control control = controlOperation(insn.opcode);
        if (insn.cls == InstrClass::Ctrl && control == Control::BranchFalse && insn.hasRelativeTarget) {
            std::cout << " target=" << hexUnsigned(insn.relativeTarget) << " branch=EAX_ZERO";
        } else if (insn.cls == InstrClass::Ctrl && (control == Control::LocalCall || control == Control::LocalJump) && insn.hasLocalTarget) {
            std::cout << " target=" << hexUnsigned(insn.localTarget);
        } else if (insn.cls == InstrClass::Ctrl && (control == Control::ScnfOffset || nativeCall(control))) {
            std::cout << " function=" << callFamilyName(insn);
            if (control == Control::CallScnf) {
                for (const VmCallSite &call : calls)
                    if (call.offset == insn.offset)
                        std::cout << " argument_words=" << call.arguments.size();
            }
        } else if (insn.cls == InstrClass::Alu && comparisonOperatorForAlu(aluOperation(insn.opcode)).has_value()) {
            const bool isFloat = aluOperation(insn.opcode) >= Alu::FloatLess;
            std::cout << " compare=" << *comparisonOperatorForAlu(aluOperation(insn.opcode)) << " type=" << (isFloat ? "float" : "int")
                      << " true_value=0xFFFFFFFF false_value=0";
        } else if (isReservedOpcode(insn.opcode)) {
            std::cout << " reserved_c0=true";
        }
        std::cout << "\n";
    }
}

void renderDumpLayout(const Program &program) {
    const Scn3Module module = buildScn3Module(program);
    const std::map<RangeKind, size_t> bytesByKind = countBytesByRangeKind(module);
    std::cout << "header token=" << tokenString(module.parsedHeader.token) << " wholeSize=" << hexUnsigned(module.parsedHeader.wholeSize)
              << " codeStart=" << hexUnsigned(program.codeStart) << " codeEnd=" << hexUnsigned(program.codeEnd)
              << " startupFunctionOffset=" << hexUnsigned(module.parsedHeader.startupFunctionOffset) << " dataEnd=" << hexUnsigned(parsedDataEnd(program))
              << "\n";
    for (const auto &[kind, count] : bytesByKind) {
        std::cout << "bytes." << rangeKindName(kind) << "=" << count << "\n";
    }
    std::cout << "functions=" << module.functions.size() << "\n";
    for (const LowLevelFunction &function : module.functions) {
        uint32_t end = function.start;
        if (!function.instructions.empty()) {
            const LowLevelInsn &last = function.instructions.back();
            end = last.offset + static_cast<uint32_t>(last.originalBytes.size());
        }
        std::cout << "function start=" << hexUnsigned(function.start) << " end=" << hexUnsigned(end) << " reason=" << discoveryReasonName(function.reason)
                  << " instructions=" << function.instructions.size() << "\n";
    }
    std::cout << "raw_ranges=" << module.rawRanges.size() << "\n";
    for (const ByteRange &range : module.rawRanges) {
        std::cout << "range start=" << hexUnsigned(range.start) << " end=" << hexUnsigned(range.end) << " kind=" << rangeKindName(range.kind)
                  << " size=" << (range.end - range.start) << "\n";
    }
}

void runRoundtripCheck(const Program &program) {
    Scn3Module module = buildScn3Module(program);
    const std::vector<uint8_t> emitted = emitScn3Module(module);
    compareEmittedBytes(module, emitted);
    std::cout << "roundtrip_check=pass\n";
    std::cout << "file_bytes=" << emitted.size() << "\n";
    std::cout << "unowned_bytes=" << unownedByteCount(module) << "\n";
    std::cout << "raw_code_gap_bytes=" << countBytesByRangeKind(module).at(RangeKind::RawCodeGap) << "\n";
    std::cout << "tail_merged_stub_candidate_bytes=" << countBytesByRangeKind(module).at(RangeKind::TailMergedStubCandidate) << "\n";
    std::cout << "packed_padding_bytes=" << countBytesByRangeKind(module).at(RangeKind::PackedPadding) << "\n";
    std::cout << "reserved_c0_opcodes=" << program.reservedOpcodeHits.size() << "\n";
}

void printHeader(const ScenarioHeader &header) {
    const ParsedHeader parsed = parseHeaderSemantics(header);
    std::cout << "Header.token = " << tokenString(parsed.token) << "\n";
    std::cout << "Header.wholeSize = " << parsed.wholeSize << "\n";
    std::cout << "Header.flags = " << hexUnsigned(parsed.flags) << " (" << scenarioFlagsText(parsed.flags) << ")\n";
    std::cout << "Header.startupFunctionOffset = " << hexUnsigned(parsed.startupFunctionOffset) << "\n";
    std::cout << "Header.dataSegmentOffset = " << hexUnsigned(parsed.dataSegmentOffset) << "\n";
    std::cout << "Header.dataEndOffset = " << hexUnsigned(parsed.dataEndOffset) << "\n";
    std::cout << "Header.workspaceSize = " << hexUnsigned(parsed.workspaceSize) << "\n";
    std::cout << "Header.field_1C = " << hexUnsigned(parsed.field1C) << "\n";
    std::cout << "Header.functionPoolStart = " << hexUnsigned(parsed.functionPoolStart) << "\n";
    std::cout << "Header.relocationTableOffset = " << hexUnsigned(parsed.relocationTableOffset) << "\n";
    std::cout << "Header.field_28 = " << hexUnsigned(parsed.field28) << "\n";
    std::cout << "Header.field_2C = " << hexUnsigned(parsed.field2C) << "\n";
}

std::string bytesToHexLine(const std::vector<uint8_t> &bytes) {
    std::ostringstream oss;
    for (size_t i = 0; i < bytes.size(); ++i) {
        if (i != 0) {
            oss << ' ';
        }
        oss << hexUnsigned(bytes[i], 2);
    }
    return oss.str();
}

void dumpSourceDataSymbol(const SourceUnit &unit, const std::string &symbol) {
    const std::map<std::string, uint32_t>::const_iterator it = unit.dataSymbols.find(symbol);
    if (it == unit.dataSymbols.end()) {
        throw std::runtime_error("No data symbol named " + symbol + " in source");
    }
    if (it->second < unit.parsedHeader.dataSegmentOffset) {
        throw std::runtime_error("Data symbol is before data segment: " + symbol);
    }
    const uint32_t rel = it->second - unit.parsedHeader.dataSegmentOffset;
    if (rel >= unit.dataSegment.size()) {
        throw std::runtime_error("Data symbol is outside rebuilt data segment: " + symbol);
    }
    uint32_t end = rel;
    while (end < unit.dataSegment.size() && unit.dataSegment[end] != 0) {
        ++end;
    }
    if (end < unit.dataSegment.size()) {
        ++end;
    } else {
        end = std::min<uint32_t>(rel + 16u, static_cast<uint32_t>(unit.dataSegment.size()));
    }
    const std::vector<uint8_t> bytes(unit.dataSegment.begin() + rel, unit.dataSegment.begin() + end);
    std::cout << "symbol=" << symbol << "\n";
    std::cout << "offset=" << hexUnsigned(it->second) << "\n";
    std::cout << "size=" << bytes.size() << "\n";
    std::cout << "bytes=" << bytesToHexLine(bytes) << "\n";
}

} // namespace shenmue::scenario
