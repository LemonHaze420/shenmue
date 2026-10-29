#include "scn3/backend/compiler.hpp"
#include "scn3/backend/object.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"

namespace shenmue::scenario {
namespace {
struct SourceDataLayout {
    uint32_t codeEnd = 0;
    uint32_t dataStart = 0;
    std::optional<size_t> finalPackedPaddingIndex;
    std::vector<uint32_t> rawStarts;
};
} // namespace

std::vector<uint8_t> compileSourceFunctionBytes(const SourceUnit &unit, const std::string &functionName) {
    const std::map<std::string, SourceFunctionAst>::const_iterator it = unit.functions.find(functionName);
    if (it == unit.functions.end()) {
        throw std::runtime_error("No function named " + functionName + " in source");
    }
    return compileExplicitFunction(unit, it->second);
}

static const SourceRawBlock *rawBlockContainingOffset(const SourceUnit &unit, uint32_t offset) {
    for (const SourceRawBlock &block : unit.rawBlocks) {
        const uint32_t end = block.declaredStart + static_cast<uint32_t>(block.bytes.size());
        if (offset >= block.declaredStart && offset < end) {
            return &block;
        }
    }
    return nullptr;
}

static LowLevelInsn makeLowLevelInsnFromBlob(const std::vector<uint8_t> &blob, const Instruction &insn) {
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
    out.originalBytes = bytesForRange(blob, insn.offset, insn.offset + insn.size);
    return out;
}

static DiscoveryReason sourceFunctionDiscoveryReason(const SourceUnit &unit, uint32_t offset) {
    if (offset == unit.parsedHeader.functionPoolStart) {
        return DiscoveryReason::FunctionPoolStart;
    }
    if (offset == unit.parsedHeader.startupFunctionOffset) {
        return DiscoveryReason::StartupEntry;
    }
    return DiscoveryReason::LocalCall;
}

static std::vector<std::string> sourceFunctionNamesInOriginalOrder(const SourceUnit &unit) {
    std::vector<std::string> names;
    names.reserve(unit.functions.size());
    std::set<std::string> seen;
    if (!unit.functionOrder.empty()) {
        for (const std::string &name : unit.functionOrder) {
            if (unit.functions.count(name) != 0 && seen.insert(name).second) {
                names.push_back(name);
            }
        }
    }
    for (const auto &[name, function] : unit.functions) {
        (void)function;
        if (seen.insert(name).second) {
            names.push_back(name);
        }
    }
    if (!unit.functionOrder.empty()) {
        return names;
    }
    std::sort(names.begin(), names.end(), [&](const std::string &lhs, const std::string &rhs) {
        const SourceFunctionAst &lhsFunction = unit.functions.at(lhs);
        const SourceFunctionAst &rhsFunction = unit.functions.at(rhs);
        if (lhsFunction.offset != rhsFunction.offset)
            return lhsFunction.offset < rhsFunction.offset;
        return lhs < rhs;
    });
    return names;
}

static SourceUnit sourceUnitWithSourceOrderFunctionOffsets(const SourceUnit &original, const std::map<std::string, std::vector<uint8_t>> &functionBytes) {
    SourceUnit placed = original;
    uint32_t current = placed.parsedHeader.functionPoolStart;
    for (const std::string &name : sourceFunctionNamesInOriginalOrder(placed)) {
        SourceFunctionAst &function = placed.functions.at(name);
        if (function.declaredOffset.has_value()) {
            function.offset = *function.declaredOffset;
        } else {
            function.offset = current;
        }
        const std::map<std::string, std::vector<uint8_t>>::const_iterator bytesIt = functionBytes.find(name);
        current = function.offset + (bytesIt != functionBytes.end() ? static_cast<uint32_t>(bytesIt->second.size()) : 1u);
    }
    return placed;
}

static uint32_t sourceOriginalDataStart(const SourceUnit &unit) {
    return unit.dataSegmentDeclaredStart != 0 ? unit.dataSegmentDeclaredStart : unit.parsedHeader.dataSegmentOffset;
}

static uint32_t sourceOriginalTrailingStart(const SourceUnit &unit) {
    if (unit.trailingDataDeclaredStart != 0) {
        return unit.trailingDataDeclaredStart;
    }
    return sourceOriginalDataStart(unit) + static_cast<uint32_t>(unit.dataSegment.size());
}

static uint32_t alignSourceDataStart(uint32_t value) {
    return (value + 3u) & ~3u;
}

static SourceDataLayout computeSourceDataLayout(const SourceUnit &unit, const std::map<std::string, std::vector<uint8_t>> &functionBytes) {
    SourceDataLayout layout{};
    layout.codeEnd = unit.parsedHeader.functionPoolStart;
    layout.rawStarts.reserve(unit.rawBlocks.size());
    for (const SourceRawBlock &block : unit.rawBlocks) {
        layout.rawStarts.push_back(block.declaredStart);
    }

    for (const auto &[name, function] : unit.functions) {
        const std::map<std::string, std::vector<uint8_t>>::const_iterator bytesIt = functionBytes.find(name);
        if (bytesIt == functionBytes.end()) {
            throw std::runtime_error("Missing compiled function bytes for " + name);
        }
        layout.codeEnd = std::max<uint32_t>(layout.codeEnd, function.offset + static_cast<uint32_t>(bytesIt->second.size()));
    }

    const uint32_t originalDataStart = sourceOriginalDataStart(unit);
    std::vector<size_t> rawOrder(unit.rawBlocks.size());
    for (size_t i = 0; i < rawOrder.size(); ++i) {
        rawOrder[i] = i;
    }
    std::sort(rawOrder.begin(), rawOrder.end(), [&](size_t lhs, size_t rhs) { return unit.rawBlocks[lhs].declaredStart < unit.rawBlocks[rhs].declaredStart; });
    for (size_t rawIndex : rawOrder) {
        const SourceRawBlock &block = unit.rawBlocks[rawIndex];
        if (block.declaredStart >= originalDataStart) {
            continue;
        }
        if (block.kind == RangeKind::PackedPadding && block.declaredEnd == originalDataStart) {
            layout.finalPackedPaddingIndex = rawIndex;
            continue;
        }
        layout.codeEnd = std::max<uint32_t>(layout.codeEnd, block.declaredStart + static_cast<uint32_t>(block.bytes.size()));
    }
    if (layout.finalPackedPaddingIndex.has_value() && layout.codeEnd == originalDataStart) {
        layout.codeEnd = unit.rawBlocks[*layout.finalPackedPaddingIndex].declaredStart;
    }
    layout.dataStart = alignSourceDataStart(layout.codeEnd);
    if (layout.finalPackedPaddingIndex.has_value()) {
        layout.rawStarts[*layout.finalPackedPaddingIndex] = layout.codeEnd;
    }
    return layout;
}

static uint32_t rebaseSourceDataOffset(
    const SourceUnit &original, const std::vector<uint32_t> &finalRawStarts, uint32_t finalDataStart, uint32_t finalTrailingStart, uint32_t originalOffset) {
    for (size_t i = 0; i < original.rawBlocks.size(); ++i) {
        const SourceRawBlock &block = original.rawBlocks[i];
        const uint32_t size = static_cast<uint32_t>(block.bytes.size());
        if (originalOffset >= block.declaredStart && originalOffset < block.declaredStart + size) {
            return finalRawStarts[i] + (originalOffset - block.declaredStart);
        }
    }

    const uint32_t dataStart = sourceOriginalDataStart(original);
    const uint32_t dataSize = static_cast<uint32_t>(original.dataSegment.size());
    if (originalOffset >= dataStart && originalOffset < dataStart + dataSize) {
        return finalDataStart + (originalOffset - dataStart);
    }

    const uint32_t trailingStart = sourceOriginalTrailingStart(original);
    const uint32_t trailingSize = static_cast<uint32_t>(original.trailingData.size());
    if (originalOffset >= trailingStart && originalOffset < trailingStart + trailingSize) {
        return finalTrailingStart + (originalOffset - trailingStart);
    }

    return originalOffset;
}

static SourceUnit sourceUnitWithFinalDataLayout(const SourceUnit &original, const std::map<std::string, std::vector<uint8_t>> &functionBytes) {
    const SourceDataLayout layout = computeSourceDataLayout(original, functionBytes);
    SourceUnit relocated = original;
    for (size_t i = 0; i < original.rawBlocks.size(); ++i) {
        relocated.rawBlocks[i].declaredStart = layout.rawStarts[i];
        relocated.rawBlocks[i].declaredEnd = layout.rawStarts[i] + static_cast<uint32_t>(relocated.rawBlocks[i].bytes.size());
    }
    const uint32_t paddingSize = layout.dataStart - layout.codeEnd;
    if (layout.finalPackedPaddingIndex.has_value()) {
        SourceRawBlock &padding = relocated.rawBlocks[*layout.finalPackedPaddingIndex];
        padding.bytes.assign(paddingSize, 0xFFu);
        padding.declaredStart = layout.codeEnd;
        padding.declaredEnd = layout.dataStart;
    } else if (paddingSize != 0) {
        SourceRawBlock padding{};
        padding.kind = RangeKind::PackedPadding;
        padding.declaredStart = layout.codeEnd;
        padding.declaredEnd = layout.dataStart;
        padding.bytes.assign(paddingSize, 0xFFu);
        relocated.rawBlocks.push_back(std::move(padding));
    }

    const uint32_t finalDataStart = layout.dataStart;
    const uint32_t finalDataEnd = finalDataStart + static_cast<uint32_t>(relocated.dataSegment.size());
    const uint32_t finalTrailingEnd = finalDataEnd + static_cast<uint32_t>(relocated.trailingData.size());

    relocated.dataSymbols.clear();
    for (const auto &[name, offset] : original.dataSymbols) {
        relocated.dataSymbols[name] = rebaseSourceDataOffset(original, layout.rawStarts, finalDataStart, finalDataEnd, offset);
    }

    relocated.cstrOffsetsByText.clear();
    for (const auto &[text, offsets] : original.cstrOffsetsByText) {
        std::vector<uint32_t> &rebased = relocated.cstrOffsetsByText[text];
        rebased.reserve(offsets.size());
        for (uint32_t offset : offsets) {
            rebased.push_back(rebaseSourceDataOffset(original, layout.rawStarts, finalDataStart, finalDataEnd, offset));
        }
    }

    relocated.header = original.header;
    relocated.header.entrypoint = original.parsedHeader.functionPoolStart;
    if (relocated.startupFunctionSymbol.has_value()) {
        const std::map<std::string, SourceFunctionAst>::const_iterator it = relocated.functions.find(*relocated.startupFunctionSymbol);
        if (it == relocated.functions.end()) {
            throw std::runtime_error("startupFunctionOffset references unknown function: " + *relocated.startupFunctionSymbol);
        }
        relocated.header.functionOffset = it->second.offset;
    }
    relocated.header.dataSegmentOffset = finalDataStart;
    relocated.header.dataEndOffset = finalDataEnd;
    relocated.header.relocationTableOffset = finalDataEnd;
    relocated.header.wholeSize = finalTrailingEnd;
    relocated.parsedHeader = parseHeaderSemantics(relocated.header);
    applySourceDataRelocations(relocated);
    if (relocated.ordinaryC) {
        std::map<uint32_t, uint32_t> patches;
        for (const SourceDataRelocation &relocation : relocated.dataRelocations) {
            if (relocation.blockKind != RangeKind::DataSegment)
                throw std::runtime_error("C pointer initializer must belong to an initialized object");
            const std::optional<SourceDataAddress> target = resolveSourceDataAddress(relocated, relocation.target);
            if (!target)
                throw std::runtime_error("C pointer initializer has no defined target");
            const uint32_t kind = target->offset < finalDataStart ? 0x2402U : 0x1402U;
            if (!patches.emplace(finalDataStart + relocation.byteOffset, kind).second)
                throw std::runtime_error("Duplicate C pointer initializer relocation");
        }
        relocated.trailingData.clear();
        for (const std::pair<const uint32_t, uint32_t> &patch : patches) {
            for (uint32_t value : {patch.second, patch.first})
                for (uint32_t byte = 0; byte < 4; ++byte)
                    relocated.trailingData.push_back(static_cast<uint8_t>(value >> (byte * 8U)));
        }
        relocated.trailingData.resize(relocated.trailingData.size() + 8U, 0);
        relocated.header.wholeSize = finalDataEnd + static_cast<uint32_t>(relocated.trailingData.size());
        relocated.parsedHeader = parseHeaderSemantics(relocated.header);
    }
    return relocated;
}

Scn3Module buildScn3ModuleFromSourceUnit(const SourceUnit &sourceUnit) {
    if ((static_cast<std::uint32_t>(sourceUnit.header.flags) & SCF_NativeCodeMask) != 0)
        throw std::runtime_error("--compile-c emits VM bytecode; the reviewed source header must select bytecode, not native SH-4.");
    if (sourceUnit.header.token == 0) {
        throw std::runtime_error("Source is missing a valid scn3_header");
    }
    if (sourceUnit.functions.empty()) {
        throw std::runtime_error("Source contains no C-like script functions to compile");
    }

    SourceUnit unit = sourceUnit;
    std::map<std::string, VmFunctionObject> objects;
    std::map<std::string, std::vector<uint8_t>> functionBytes;
    for (const std::string &name : sourceFunctionNamesInOriginalOrder(unit)) {
        VmFunctionObject object = lowerVmFunction(unit, unit.functions.at(name));
        functionBytes.emplace(name, object.bytes);
        objects.emplace(name, std::move(object));
    }
    unit = sourceUnitWithSourceOrderFunctionOffsets(unit, functionBytes);
    unit = sourceUnitWithFinalDataLayout(unit, functionBytes);
    std::set<std::string> duplicateLabels;
    std::map<std::string, uint32_t> labels;
    for (const std::pair<const std::string, VmFunctionObject> &object : objects)
        for (const std::pair<const std::string, uint32_t> &label : object.second.labels)
            if (!labels.emplace(label.first, unit.functions.at(object.first).offset + label.second).second)
                duplicateLabels.insert(label.first);
    for (const std::pair<const std::string, uint32_t> &label : labels)
        if (!duplicateLabels.count(label.first))
            unit.codeLabels[label.first] = label.second;
    for (const std::string &name : sourceFunctionNamesInOriginalOrder(unit)) {
        const size_t measuredSize = functionBytes.at(name).size();
        functionBytes.at(name) = linkVmFunction(unit, unit.functions.at(name), std::move(objects.at(name)));
        if (functionBytes.at(name).size() != measuredSize)
            throw std::runtime_error("Relocation changed the encoded function size: " + name);
    }

    validateCStackDepth(unit, functionBytes);

    ScenarioHeader header = unit.header;
    const uint32_t dataStart = unit.parsedHeader.dataSegmentOffset;
    const uint32_t dataEnd = dataStart + static_cast<uint32_t>(unit.dataSegment.size());
    const uint32_t trailingEnd = dataEnd + static_cast<uint32_t>(unit.trailingData.size());
    header.wholeSize = trailingEnd;
    header.dataSegmentOffset = dataStart;
    header.dataEndOffset = dataEnd;
    header.relocationTableOffset = dataEnd;

    Scn3Module module{};
    module.rawHeader = header;
    module.parsedHeader = parseHeaderSemantics(header);
    module.originalBlob.assign(trailingEnd, 0);
    module.byteOwners.assign(trailingEnd, RangeKind::Unowned);
    std::vector<uint8_t> emittedCode(trailingEnd, 0);

    appendLayoutRange(module, 0, std::min<uint32_t>(sizeof(ScenarioHeader), trailingEnd), RangeKind::Header);

    for (const std::string &name : sourceFunctionNamesInOriginalOrder(unit)) {
        const SourceFunctionAst &function = unit.functions.at(name);
        const std::vector<uint8_t> &bytes = functionBytes.at(name);
        const uint32_t start = function.offset;
        const uint32_t end = start + static_cast<uint32_t>(bytes.size());
        if (start < sizeof(ScenarioHeader) || end > dataStart || end > module.originalBlob.size()) {
            throw std::runtime_error("Compiled function " + name + " does not fit the declared code segment");
        }
        for (uint32_t i = 0; i < bytes.size(); ++i) {
            if (emittedCode[start + i] && module.originalBlob[start + i] != bytes[i])
                throw std::runtime_error("Conflicting function views at " + hexUnsigned(start + i));
            emittedCode[start + i] = 1;
        }
        std::copy(bytes.begin(), bytes.end(), module.originalBlob.begin() + start);

        LowLevelFunction lowFunction{};
        lowFunction.start = start;
        lowFunction.baseOffset = function.explicitBase.value_or(0);
        lowFunction.reason = sourceFunctionDiscoveryReason(unit, start);

        uint32_t off = start;
        while (off < end) {
            if (const SourceRawBlock *raw = rawBlockContainingOffset(unit, off)) {
                const uint32_t rawEnd = std::min<uint32_t>(raw->declaredStart + static_cast<uint32_t>(raw->bytes.size()), end);
                off = rawEnd;
                continue;
            }
            const Instruction insn = decodeInstruction(module.originalBlob, off, lowFunction.baseOffset, dataStart);
            const uint32_t insnEnd = off + insn.size;
            if (insnEnd > end) {
                throw std::runtime_error("Compiled instruction for " + name + " extends past the function body at " + hexUnsigned(off));
            }
            lowFunction.instructions.push_back(makeLowLevelInsnFromBlob(module.originalBlob, insn));
            appendLayoutRange(module, off, insnEnd, RangeKind::DecodedInstruction, start);
            off = insnEnd;
        }

        module.functions.push_back(std::move(lowFunction));
    }

    auto writeRawBlock = [&](const SourceRawBlock &block, uint32_t startOverride, RangeKind kind) {
        const uint32_t start = startOverride;
        const uint32_t end = start + static_cast<uint32_t>(block.bytes.size());
        if (end > module.originalBlob.size()) {
            throw std::runtime_error("Source raw/data block extends past rebuilt file");
        }
        for (uint32_t i = 0; i < block.bytes.size(); ++i) {
            if ((emittedCode[start + i] || module.byteOwners[start + i] != RangeKind::Unowned) && module.originalBlob[start + i] != block.bytes[i])
                throw std::runtime_error("Conflicting code/data layout at " + hexUnsigned(start + i));
        }
        std::copy(block.bytes.begin(), block.bytes.end(), module.originalBlob.begin() + start);
        appendLayoutRange(module, start, end, kind);
    };

    for (const SourceRawBlock &block : unit.rawBlocks) {
        writeRawBlock(block, block.declaredStart, block.kind);
    }
    SourceRawBlock dataBlock{};
    dataBlock.bytes = unit.dataSegment;
    writeRawBlock(dataBlock, dataStart, RangeKind::DataSegment);
    SourceRawBlock trailingBlock{};
    trailingBlock.bytes = unit.trailingData;
    writeRawBlock(trailingBlock, dataEnd, RangeKind::TrailingDataOrUnknown);

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
        uint32_t firstStart = 0;
        uint32_t firstEnd = 0;
        for (uint32_t off = 0; off < module.byteOwners.size(); ++off) {
            if (module.byteOwners[off] != RangeKind::Unowned) {
                continue;
            }
            firstStart = off;
            firstEnd = off + 1;
            while (firstEnd < module.byteOwners.size() && module.byteOwners[firstEnd] == RangeKind::Unowned) {
                ++firstEnd;
            }
            break;
        }
        throw std::runtime_error("Compiled source left " + std::to_string(unownedByteCount(module)) + " unowned bytes; first gap " + hexUnsigned(firstStart) +
            ".." + hexUnsigned(firstEnd));
    }
    return module;
}

} // namespace shenmue::scenario
