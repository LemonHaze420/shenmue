#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/decompiler/discovery.hpp"

namespace shenmue::scenario {

Program buildProgram(const std::vector<uint8_t> &blob, const DecodeOptions &options) {
    Program program{};
    program.blob = blob;
    program.header = parseHeader(blob);
    if ((static_cast<std::uint32_t>(program.header.flags) & SCF_NativeCodeMask) != 0)
        throw std::runtime_error("Native SH-4 SCN3 is not VM bytecode");
    program.parsedHeader = parseHeaderSemantics(program.header);
    program.codeStart = options.startOverride.value_or(program.parsedHeader.functionPoolStart);
    program.codeEnd = program.parsedHeader.dataSegmentOffset;
    if (program.codeStart >= program.codeEnd) {
        throw std::runtime_error("Invalid code range");
    }

    std::map<uint32_t, PendingDecode> pending{{program.codeStart, PendingDecode{0, DiscoveryReason::FunctionPoolStart}}};
    if (!options.startOverride.has_value() && program.parsedHeader.startupFunctionOffset >= program.codeStart &&
        program.parsedHeader.startupFunctionOffset < program.codeEnd) {
        pending[program.parsedHeader.startupFunctionOffset] = PendingDecode{0, DiscoveryReason::StartupEntry};
    }
    std::set<uint32_t> visited;
    std::vector<bool> covered(program.codeEnd, false);

    const auto queueDecode = [](std::map<uint32_t, PendingDecode> &worklist, uint32_t start, uint32_t baseOffset, DiscoveryReason reason) {
        const std::map<uint32_t, PendingDecode>::iterator it = worklist.find(start);
        if (it == worklist.end() || discoveryReasonPriority(reason) > discoveryReasonPriority(it->second.reason)) {
            worklist[start] = PendingDecode{baseOffset, reason};
        }
    };

    const auto decodeOne = [&](uint32_t start, uint32_t baseOffset, DiscoveryReason reason, std::map<uint32_t, PendingDecode> *worklist) {
        if (visited.count(start) != 0) {
            const std::map<uint32_t, FunctionInfo>::iterator existing = program.functions.find(start);
            if (existing != program.functions.end() && discoveryReasonPriority(reason) > discoveryReasonPriority(existing->second.discoveryReason)) {
                existing->second.discoveryReason = reason;
            }
            return;
        }
        visited.insert(start);
        if (start < program.codeStart || start >= program.codeEnd) {
            return;
        }

        FunctionInfo function{};
        function.start = start;
        function.baseOffset = baseOffset;
        function.discoveryReason = reason;
        std::map<uint32_t, Instruction> decoded;
        std::set<uint32_t> blockWorklist{start};
        std::vector<std::pair<uint32_t, uint32_t>> stagedCoverage;
        std::vector<std::pair<uint32_t, uint8_t>> stagedReservedHits;
        bool hasNonPaddingInstruction = false;

        while (!blockWorklist.empty()) {
            const uint32_t off = *blockWorklist.begin();
            blockWorklist.erase(blockWorklist.begin());
            if (off < program.codeStart || off >= program.codeEnd || decoded.count(off) != 0) {
                continue;
            }

            Instruction insn = decodeInstruction(program.blob, off, baseOffset, program.codeEnd);
            decoded.emplace(off, insn);
            if (isReservedOpcode(insn.opcode)) {
                stagedReservedHits.emplace_back(insn.offset, insn.opcode);
            }
            stagedCoverage.emplace_back(off, off + insn.size);
            if (insn.cls != InstrClass::Unknown) {
                hasNonPaddingInstruction = true;
            }

            const uint32_t nextOff = off + insn.size;
            if (insn.cls == InstrClass::Ctrl) {
                switch (controlOperation(insn.opcode)) {
                case Control::Return:
                    break;
                case Control::LocalCall:
                    if (insn.hasLocalTarget && insn.localTarget >= program.codeStart && insn.localTarget < program.codeEnd) {
                        function.localTargets.insert(insn.localTarget);
                        if (worklist != nullptr) {
                            queueDecode(*worklist, insn.localTarget, baseOffset, DiscoveryReason::LocalCall);
                        }
                    }
                    if (nextOff < program.codeEnd)
                        blockWorklist.insert(nextOff);
                    break;
                case Control::LocalJump:
                    if (insn.hasLocalTarget && insn.localTarget >= program.codeStart && insn.localTarget < program.codeEnd) {
                        function.labelTargets.insert(insn.localTarget);
                        blockWorklist.insert(insn.localTarget);
                    }
                    break;
                case Control::Jump:
                    if (insn.hasRelativeTarget && insn.relativeTarget >= program.codeStart && insn.relativeTarget < program.codeEnd) {
                        function.labelTargets.insert(insn.relativeTarget);
                        blockWorklist.insert(insn.relativeTarget);
                    }
                    break;
                case Control::BranchFalse:
                    if (insn.hasRelativeTarget && insn.relativeTarget >= program.codeStart && insn.relativeTarget < program.codeEnd) {
                        function.labelTargets.insert(insn.relativeTarget);
                        blockWorklist.insert(insn.relativeTarget);
                    }
                    if (nextOff < program.codeEnd)
                        blockWorklist.insert(nextOff);
                    break;
                case Control::ScnfOffset:
                    break;
                default:
                    if (nextOff < program.codeEnd)
                        blockWorklist.insert(nextOff);
                    break;
                }
            } else if (insn.cls != InstrClass::Unknown || isReservedOpcode(insn.opcode)) {
                if (nextOff < program.codeEnd)
                    blockWorklist.insert(nextOff);
            }
        }

        for (const auto &[_, insn] : decoded) {
            function.instructions.push_back(insn);
        }
        if (reason == DiscoveryReason::PackedNext && !hasNonPaddingInstruction) {
            if (!function.instructions.empty()) {
                uint32_t paddingStart = function.instructions.front().offset;
                uint32_t paddingEnd = paddingStart;
                for (const Instruction &insn : function.instructions) {
                    paddingStart = std::min(paddingStart, insn.offset);
                    paddingEnd = std::max<uint32_t>(paddingEnd, insn.offset + insn.size);
                }
                program.discardedPackedPaddingRanges.emplace_back(paddingStart, paddingEnd);
            }
            visited.erase(start);
            return;
        }
        for (const auto &[coverageStart, coverageEnd] : stagedCoverage) {
            for (uint32_t i = coverageStart; i < coverageEnd && i < covered.size(); ++i) {
                covered[i] = true;
            }
        }
        program.reservedOpcodeHits.insert(program.reservedOpcodeHits.end(), stagedReservedHits.begin(), stagedReservedHits.end());
        uint32_t functionEnd = start;
        for (const Instruction &insn : function.instructions) {
            functionEnd = std::max<uint32_t>(functionEnd, insn.offset + insn.size);
        }
        if (worklist != nullptr && functionEnd > start && functionEnd < program.codeEnd && visited.count(functionEnd) == 0 &&
            worklist->count(functionEnd) == 0) {
            queueDecode(*worklist, functionEnd, 0, DiscoveryReason::PackedNext);
        }
        program.functions.emplace(start, std::move(function));
    };

    const auto drainPending = [&]() {
        bool decodedAny = false;
        while (!pending.empty()) {
            const std::map<uint32_t, PendingDecode>::iterator it = pending.begin();
            const uint32_t start = it->first;
            const PendingDecode pendingDecode = it->second;
            pending.erase(it);
            const size_t before = program.functions.size();
            decodeOne(start, pendingDecode.baseOffset, pendingDecode.reason, &pending);
            decodedAny = decodedAny || program.functions.size() != before;
        }
        return decodedAny;
    };

    bool changed = true;
    while (changed) {
        changed = false;
        changed = drainPending() || changed;

        const VmProgramAnalysis analysis = analyzeVmProgram(program);
        if (analysis.arguments != program.callArguments) {
            program.callArguments = analysis.arguments;
            changed = true;
        }
        for (uint32_t target : analysis.callbackTargets) {
            if (program.scriptCreatedTargets.insert(target).second)
                changed = true;
            const std::map<uint32_t, FunctionInfo>::iterator existing = program.functions.find(target);
            if (existing != program.functions.end()) {
                if (discoveryReasonPriority(DiscoveryReason::TaskCreated) > discoveryReasonPriority(existing->second.discoveryReason))
                    existing->second.discoveryReason = DiscoveryReason::TaskCreated;
            } else
                queueDecode(pending, target, 0, DiscoveryReason::TaskCreated);
        }
    }

    if (options.decodeUncovered) {
        for (uint32_t off = program.codeStart; off < program.codeEnd; ++off) {
            if (covered[off] || program.functions.count(off) != 0) {
                continue;
            }
            decodeOne(off, 0, DiscoveryReason::DecodeUncovered, nullptr);
        }
    }

    const std::vector<bool> reachabilityCovered = covered;
    std::vector<bool> physicalCovered(program.codeEnd, false);
    for (std::map<uint32_t, FunctionInfo>::iterator it = program.functions.begin(); it != program.functions.end(); ++it) {
        FunctionInfo &function = it->second;
        const uint32_t physicalEnd = std::next(it) != program.functions.end() ? std::next(it)->first : program.codeEnd;
        std::vector<Instruction> physicalInstructions;
        std::set<uint32_t> physicalLabelTargets = function.labelTargets;
        std::set<uint32_t> physicalLocalTargets = function.localTargets;
        std::set<uint32_t> reachableInstructionOffsets;
        std::set<uint32_t> physicalInstructionOffsets;
        bool previousInstructionStopsFallthrough = false;
        for (uint32_t off = function.start; off < physicalEnd;) {
            Instruction insn;
            try {
                insn = decodeInstruction(program.blob, off, function.baseOffset, physicalEnd);
            } catch (const std::runtime_error &) {
                break;
            }
            if (insn.cls == InstrClass::Unknown) {
                break;
            }
            const bool wasReachabilityCovered = insn.offset < reachabilityCovered.size() && reachabilityCovered[insn.offset];
            physicalInstructionOffsets.insert(insn.offset);
            if (wasReachabilityCovered) {
                reachableInstructionOffsets.insert(insn.offset);
            }
            if (previousInstructionStopsFallthrough && !wasReachabilityCovered) {
                physicalLabelTargets.insert(insn.offset);
            }
            physicalInstructions.push_back(insn);
            for (uint32_t byte = insn.offset; byte < insn.offset + insn.size && byte < physicalCovered.size(); ++byte) {
                physicalCovered[byte] = true;
            }
            const uint32_t nextOff = insn.offset + insn.size;
            previousInstructionStopsFallthrough = false;
            if (insn.cls == InstrClass::Ctrl) {
                const Control control = controlOperation(insn.opcode);
                if (control == Control::LocalCall && insn.hasLocalTarget && insn.localTarget >= program.codeStart && insn.localTarget < program.codeEnd) {
                    physicalLocalTargets.insert(insn.localTarget);
                } else if (control == Control::LocalJump && insn.hasLocalTarget && insn.localTarget >= function.start && insn.localTarget < physicalEnd) {
                    physicalLabelTargets.insert(insn.localTarget);
                } else if ((control == Control::Jump || control == Control::BranchFalse) && insn.hasRelativeTarget && insn.relativeTarget >= function.start &&
                    insn.relativeTarget < physicalEnd) {
                    physicalLabelTargets.insert(insn.relativeTarget);
                }
                previousInstructionStopsFallthrough =
                    control == Control::Return || control == Control::LocalJump || control == Control::Jump || control == Control::ScnfOffset;
            }
            off = nextOff;
        }
        std::set<uint32_t> physicalOnlyInstructionOffsets;
        std::set_difference(physicalInstructionOffsets.begin(), physicalInstructionOffsets.end(), reachableInstructionOffsets.begin(),
            reachableInstructionOffsets.end(), std::inserter(physicalOnlyInstructionOffsets, physicalOnlyInstructionOffsets.end()));
        function.instructions = std::move(physicalInstructions);
        function.labelTargets = std::move(physicalLabelTargets);
        function.physicalOnlyInstructionOffsets = std::move(physicalOnlyInstructionOffsets);
        function.localTargets = std::move(physicalLocalTargets);
    }
    covered = std::move(physicalCovered);
    program.uncoveredCodeRanges.clear();
    for (uint32_t off = program.codeStart; off < program.codeEnd;) {
        if (covered[off]) {
            ++off;
            continue;
        }
        const uint32_t start = off;
        while (off < program.codeEnd && !covered[off]) {
            ++off;
        }
        program.uncoveredCodeRanges.emplace_back(start, off);
    }

    return program;
}

} // namespace shenmue::scenario
