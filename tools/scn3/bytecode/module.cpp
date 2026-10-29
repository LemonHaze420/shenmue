#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"

namespace shenmue::scenario {

bool rangeContains(uint32_t start, uint32_t end, uint32_t value) {
    return value >= start && value < end;
}

bool rangeInList(const std::vector<std::pair<uint32_t, uint32_t>> &ranges, uint32_t start, uint32_t end) {
    for (const auto &[rangeStart, rangeEnd] : ranges) {
        if (start >= rangeStart && end <= rangeEnd) {
            return true;
        }
    }
    return false;
}

bool isTailMergedStubCandidateRange(const Program &program, uint32_t start, uint32_t end) {
    if (start >= end || end > program.codeEnd) {
        return false;
    }
    try {
        const Instruction insn = decodeInstruction(program.blob, start, 0, end);
        const Control control = controlOperation(insn.opcode);
        return start + insn.size == end && insn.cls == InstrClass::Ctrl &&
            (control == Control::Return || control == Control::LocalJump || control == Control::Jump || control == Control::BranchFalse);
    } catch (const std::exception &) {
        return false;
    }
}

uint32_t parsedDataEnd(const Program &program) {
    return std::min<uint32_t>(program.parsedHeader.dataEndOffset, static_cast<uint32_t>(program.blob.size()));
}

std::vector<uint8_t> bytesForRange(const std::vector<uint8_t> &blob, uint32_t start, uint32_t end) {
    if (start > end || end > blob.size()) {
        throw std::runtime_error("Invalid byte range " + hexUnsigned(start) + ".." + hexUnsigned(end));
    }
    return std::vector<uint8_t>(blob.begin() + start, blob.begin() + end);
}

LowLevelInsn makeLowLevelInsn(const Program &program, const Instruction &insn) {
    LowLevelInsn out{};
    out.offset = insn.offset;
    out.opcode = insn.opcode;
    out.cls = insn.cls;
    out.imm = insn.imm;
    out.immWidth = insn.immWidth;
    if (insn.hasRelativeTarget) {
        out.relativeTarget = insn.relativeTarget;
    }
    if (insn.hasLocalTarget) {
        out.localTarget = insn.localTarget;
    }
    out.originalBytes = bytesForRange(program.blob, insn.offset, insn.offset + insn.size);
    return out;
}

void appendLayoutRange(Scn3Module &module, uint32_t start, uint32_t end, RangeKind kind, std::optional<uint32_t> functionStart) {
    if (start >= end) {
        return;
    }
    if (!module.layoutRanges.empty()) {
        ByteRange &last = module.layoutRanges.back();
        if (last.end == start && last.kind == kind && last.functionStart == functionStart) {
            last.end = end;
        } else {
            module.layoutRanges.push_back(ByteRange{start, end, kind, functionStart});
        }
    } else {
        module.layoutRanges.push_back(ByteRange{start, end, kind, functionStart});
    }
    if (kind != RangeKind::DecodedInstruction && kind != RangeKind::Header) {
        if (!module.rawRanges.empty()) {
            ByteRange &last = module.rawRanges.back();
            if (last.end == start && last.kind == kind && last.functionStart == functionStart) {
                last.end = end;
            } else {
                module.rawRanges.push_back(ByteRange{start, end, kind, functionStart});
            }
        } else {
            module.rawRanges.push_back(ByteRange{start, end, kind, functionStart});
        }
    }
    for (uint32_t off = start; off < end && off < module.byteOwners.size(); ++off) {
        module.byteOwners[off] = kind;
    }
}

Scn3Module buildScn3Module(const Program &program) {
    Scn3Module module{};
    module.rawHeader = program.header;
    module.parsedHeader = program.parsedHeader;
    module.originalBlob = program.blob;
    module.byteOwners.assign(program.blob.size(), RangeKind::Unowned);

    appendLayoutRange(module, 0, std::min<uint32_t>(sizeof(ScenarioHeader), static_cast<uint32_t>(program.blob.size())), RangeKind::Header);

    for (const auto &[_, function] : program.functions) {
        LowLevelFunction lowFunction{};
        lowFunction.start = function.start;
        lowFunction.baseOffset = function.baseOffset;
        lowFunction.reason = function.discoveryReason;
        for (const Instruction &insn : function.instructions) {
            lowFunction.instructions.push_back(makeLowLevelInsn(program, insn));
            appendLayoutRange(module, insn.offset, insn.offset + insn.size, RangeKind::DecodedInstruction, function.start);
        }
        module.functions.push_back(std::move(lowFunction));
    }

    for (const auto &[start, end] : program.uncoveredCodeRanges) {
        RangeKind kind = RangeKind::RawCodeGap;
        if (rangeInList(program.discardedPackedPaddingRanges, start, end)) {
            kind = RangeKind::PackedPadding;
        } else if (isTailMergedStubCandidateRange(program, start, end)) {
            kind = RangeKind::TailMergedStubCandidate;
        }
        appendLayoutRange(module, start, end, kind);
    }

    const uint32_t dataEnd = parsedDataEnd(program);
    appendLayoutRange(module, program.codeEnd, dataEnd, RangeKind::DataSegment);
    appendLayoutRange(module, dataEnd, static_cast<uint32_t>(program.blob.size()), RangeKind::TrailingDataOrUnknown);

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

    return module;
}

std::map<RangeKind, size_t> countBytesByRangeKind(const Scn3Module &module) {
    std::map<RangeKind, size_t> counts;
    counts[RangeKind::Header] = 0;
    counts[RangeKind::DecodedInstruction] = 0;
    counts[RangeKind::RawCodeGap] = 0;
    counts[RangeKind::TailMergedStubCandidate] = 0;
    counts[RangeKind::PackedPadding] = 0;
    counts[RangeKind::DataSegment] = 0;
    counts[RangeKind::TrailingDataOrUnknown] = 0;
    counts[RangeKind::Unowned] = 0;
    for (RangeKind kind : module.byteOwners) {
        ++counts[kind];
    }
    return counts;
}

void appendImmediateBytes(std::vector<uint8_t> &out, uint32_t value, uint8_t immWidth) {
    switch (immWidth) {
    case 0:
        return;
    case 1:
        out.push_back(static_cast<uint8_t>(value & 0xFFu));
        return;
    case 2:
        out.push_back(static_cast<uint8_t>(value & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
        return;
    case 3:
        out.push_back(static_cast<uint8_t>(value & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 8) & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 16) & 0xFFu));
        out.push_back(static_cast<uint8_t>((value >> 24) & 0xFFu));
        return;
    default:
        throw std::runtime_error("Invalid immediate width while emitting");
    }
}

std::vector<uint8_t> encodeLowLevelInsn(const LowLevelInsn &insn) {
    if (insn.cls == InstrClass::Unknown) {
        return insn.originalBytes;
    }
    std::vector<uint8_t> out;
    out.push_back(insn.opcode);
    if (insn.cls == InstrClass::Alu) {
        return out;
    }
    if (insn.cls != InstrClass::Alu && insn.immWidth != 0) {
        if (!insn.imm.has_value()) {
            throw std::runtime_error("Known instruction missing immediate at " + hexUnsigned(insn.offset));
        }
        appendImmediateBytes(out, *insn.imm, insn.immWidth);
    }
    if (out.size() != insn.originalBytes.size()) {
        throw std::runtime_error("Encoded instruction size mismatch at " + hexUnsigned(insn.offset));
    }
    return out;
}

std::string instructionSummary(const LowLevelInsn &insn) {
    std::ostringstream oss;
    oss << "offset=" << hexUnsigned(insn.offset) << " opcode=" << hexUnsigned(insn.opcode, 2) << " class=";
    switch (insn.cls) {
    case InstrClass::Ctrl:
        oss << "ctrl";
        break;
    case InstrClass::Addr:
        oss << "addr";
        break;
    case InstrClass::Alu:
        oss << "alu";
        break;
    case InstrClass::Unknown:
        oss << "unknown";
        break;
    }
    if (insn.imm.has_value()) {
        oss << " imm=" << hexUnsigned(*insn.imm);
    }
    if (insn.relativeTarget.has_value()) {
        oss << " relativeTarget=" << hexUnsigned(*insn.relativeTarget);
    }
    if (insn.localTarget.has_value()) {
        oss << " localTarget=" << hexUnsigned(*insn.localTarget);
    }
    return oss.str();
}

const LowLevelInsn *findInstructionOwningOffset(const Scn3Module &module, uint32_t offset) {
    for (const LowLevelFunction &function : module.functions) {
        for (const LowLevelInsn &insn : function.instructions) {
            if (rangeContains(insn.offset, insn.offset + static_cast<uint32_t>(insn.originalBytes.size()), offset)) {
                return &insn;
            }
        }
    }
    return nullptr;
}

std::string ownerNameAt(const Scn3Module &module, uint32_t offset) {
    if (offset >= module.byteOwners.size()) {
        return "out_of_range";
    }
    return rangeKindName(module.byteOwners[offset]);
}

void applyPatchImm(Scn3Module &module, uint32_t offset, uint32_t value) {
    for (LowLevelFunction &function : module.functions) {
        for (LowLevelInsn &insn : function.instructions) {
            if (insn.offset != offset) {
                continue;
            }
            if (insn.immWidth == 0 || !insn.imm.has_value()) {
                throw std::runtime_error("--patch-imm target has no immediate at " + hexUnsigned(offset));
            }
            if ((insn.immWidth == 1 && value > 0xFFu) || (insn.immWidth == 2 && value > 0xFFFFu)) {
                throw std::runtime_error("--patch-imm value does not fit original immediate width at " + hexUnsigned(offset));
            }
            insn.imm = value;
            return;
        }
    }
    throw std::runtime_error("--patch-imm offset is not a decoded instruction: " + hexUnsigned(offset));
}

std::vector<uint8_t> emitScn3Module(const Scn3Module &module) {
    if (std::count(module.byteOwners.begin(), module.byteOwners.end(), RangeKind::Unowned) != 0) {
        throw std::runtime_error("Cannot emit module with unowned bytes");
    }
    std::vector<uint8_t> out(module.originalBlob.size(), 0);
    std::vector<uint8_t> occupied(out.size(), 0);
    const auto write = [&](uint32_t offset, uint8_t value) {
        if (offset >= out.size())
            throw std::runtime_error("Module write extends beyond output");
        if (occupied[offset] && out[offset] != value)
            throw std::runtime_error("Conflicting physical writes at " + hexUnsigned(offset));
        out[offset] = value;
        occupied[offset] = 1;
    };

    const uint32_t headerEnd = std::min<uint32_t>(sizeof(ScenarioHeader), static_cast<uint32_t>(module.originalBlob.size()));
    std::array<uint8_t, sizeof(ScenarioHeader)> headerBytes{};
    std::memcpy(headerBytes.data(), &module.rawHeader, sizeof(module.rawHeader));
    for (uint32_t i = 0; i < headerEnd; ++i)
        write(i, headerBytes[i]);

    for (const ByteRange &range : module.rawRanges) {
        if (range.end > module.originalBlob.size()) {
            throw std::runtime_error("Raw range extends beyond original blob");
        }
        for (uint32_t i = range.start; i < range.end; ++i)
            write(i, module.originalBlob[i]);
    }

    for (const LowLevelFunction &function : module.functions) {
        for (const LowLevelInsn &insn : function.instructions) {
            const std::vector<uint8_t> encoded = encodeLowLevelInsn(insn);
            if (insn.offset + encoded.size() > out.size()) {
                throw std::runtime_error("Encoded instruction extends beyond output at " + hexUnsigned(insn.offset));
            }
            for (uint32_t i = 0; i < encoded.size(); ++i)
                write(insn.offset + i, encoded[i]);
        }
    }

    return out;
}

void compareEmittedBytes(const Scn3Module &module, const std::vector<uint8_t> &emitted) {
    if (emitted.size() != module.originalBlob.size()) {
        throw std::runtime_error(
            "Roundtrip size mismatch original=" + std::to_string(module.originalBlob.size()) + " emitted=" + std::to_string(emitted.size()));
    }
    for (uint32_t off = 0; off < module.originalBlob.size(); ++off) {
        if (module.originalBlob[off] == emitted[off]) {
            continue;
        }
        std::ostringstream oss;
        oss << "Roundtrip mismatch at " << hexUnsigned(off) << " original=" << hexUnsigned(module.originalBlob[off], 2)
            << " emitted=" << hexUnsigned(emitted[off], 2) << " owner=" << ownerNameAt(module, off);
        if (const LowLevelInsn *insn = findInstructionOwningOffset(module, off)) {
            oss << " instruction={" << instructionSummary(*insn) << "}";
        }
        throw std::runtime_error(oss.str());
    }
}

size_t unownedByteCount(const Scn3Module &module) {
    return static_cast<size_t>(std::count(module.byteOwners.begin(), module.byteOwners.end(), RangeKind::Unowned));
}

uint32_t moduleDataEnd(const Scn3Module &module) {
    return std::min<uint32_t>(module.parsedHeader.dataEndOffset, static_cast<uint32_t>(module.originalBlob.size()));
}

} // namespace shenmue::scenario
