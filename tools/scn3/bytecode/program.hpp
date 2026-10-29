#pragma once

#include "scn3/common/std.hpp"
#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/value.hpp"

#include "s1/task/scen.hpp"

namespace shenmue::scenario {

using shenmue::s1::ScenarioHeader;

enum ScenarioFlag : uint32_t {
    SCF_Unknown0100 = 0x00000100U,
    SCF_NativeCodeMask = 0x00ff0000U,
};

enum class InstrClass {
    Ctrl,
    Addr,
    Alu,
    Unknown,
};

enum class AddrKind {
    Idx8,
    Ld16,
    Ld32,
    NewPointer,
    RelativeOffset,
    Literal,
};

struct Instruction {
    uint32_t offset = 0;
    uint8_t opcode = 0;
    uint8_t immWidth = 0;
    uint8_t size = 1;
    InstrClass cls = InstrClass::Unknown;
    std::optional<uint32_t> imm;
    AddrBase addrBase = AddrBase::CurrBase;
    AddrKind addrKind = AddrKind::Idx8;
    bool hasRelativeTarget = false;
    int32_t relativeImmediate = 0;
    uint32_t relativeTarget = 0;
    bool hasLocalTarget = false;
    uint32_t localTarget = 0;
};

enum class DiscoveryReason {
    FunctionPoolStart,
    StartupEntry,
    LocalCall,
    TaskCreated,
    PackedNext,
    DecodeUncovered,
};

struct PendingDecode {
    uint32_t baseOffset = 0;
    DiscoveryReason reason = DiscoveryReason::FunctionPoolStart;
};

struct FunctionInfo {
    uint32_t start = 0;
    uint32_t baseOffset = 0;
    DiscoveryReason discoveryReason = DiscoveryReason::FunctionPoolStart;
    std::vector<Instruction> instructions;
    std::set<uint32_t> localTargets;
    std::set<uint32_t> labelTargets;
    std::set<uint32_t> physicalOnlyInstructionOffsets;
};

struct ParsedHeader {
    uint32_t token = 0;
    uint32_t wholeSize = 0;
    uint32_t flags = 0;
    uint32_t startupFunctionOffset = 0;
    uint32_t dataSegmentOffset = 0;
    uint32_t dataEndOffset = 0;
    uint32_t workspaceSize = 0;
    uint32_t field1C = 0;
    uint32_t functionPoolStart = 0;
    uint32_t relocationTableOffset = 0;
    uint32_t field28 = 0;
    uint32_t field2C = 0;
};

struct Program {
    std::vector<uint8_t> blob;
    ScenarioHeader header;
    ParsedHeader parsedHeader;
    uint32_t codeStart = 0;
    uint32_t codeEnd = 0;
    std::map<uint32_t, FunctionInfo> functions;
    std::set<uint32_t> scriptCreatedTargets;
    std::map<uint32_t, std::vector<VmValue>> callArguments;
    std::vector<std::pair<uint32_t, uint8_t>> reservedOpcodeHits;
    std::vector<std::pair<uint32_t, uint32_t>> uncoveredCodeRanges;
    std::vector<std::pair<uint32_t, uint32_t>> discardedPackedPaddingRanges;
};

enum class RangeKind {
    Header,
    DecodedInstruction,
    RawCodeGap,
    TailMergedStubCandidate,
    PackedPadding,
    DataSegment,
    TrailingDataOrUnknown,
    Unowned,
};

struct ByteRange {
    uint32_t start = 0;
    uint32_t end = 0;
    RangeKind kind = RangeKind::Unowned;
    std::optional<uint32_t> functionStart;
};

struct LowLevelInsn {
    uint32_t offset = 0;
    uint8_t opcode = 0;
    InstrClass cls = InstrClass::Unknown;
    std::optional<uint32_t> imm;
    uint8_t immWidth = 0;
    std::optional<uint32_t> relativeTarget;
    std::optional<uint32_t> localTarget;
    std::vector<uint8_t> originalBytes;
};

struct LowLevelFunction {
    uint32_t start = 0;
    uint32_t baseOffset = 0;
    DiscoveryReason reason = DiscoveryReason::FunctionPoolStart;
    std::vector<LowLevelInsn> instructions;
};

struct Scn3Module {
    ScenarioHeader rawHeader;
    ParsedHeader parsedHeader;
    std::vector<LowLevelFunction> functions;
    std::vector<ByteRange> rawRanges;
    std::vector<ByteRange> layoutRanges;
    std::vector<RangeKind> byteOwners;
    std::vector<uint8_t> originalBlob;
};

} // namespace shenmue::scenario
