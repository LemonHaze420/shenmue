#pragma once

#include "scn3/bytecode/isa.hpp"
#include "scn3/bytecode/program.hpp"

namespace shenmue::scenario {

uint32_t moduleDataEnd(const Scn3Module &module);
uint32_t immWidthBytes(uint8_t immWidth);
bool rangeContains(uint32_t start, uint32_t end, uint32_t value);
bool rangeInList(const std::vector<std::pair<uint32_t, uint32_t>> &ranges, uint32_t start, uint32_t end);
bool isTailMergedStubCandidateRange(const Program &program, uint32_t start, uint32_t end);
uint32_t parsedDataEnd(const Program &program);
std::vector<uint8_t> bytesForRange(const std::vector<uint8_t> &blob, uint32_t start, uint32_t end);
LowLevelInsn makeLowLevelInsn(const Program &program, const Instruction &insn);
void appendLayoutRange(Scn3Module &module, uint32_t start, uint32_t end, RangeKind kind, std::optional<uint32_t> functionStart = std::nullopt);
Scn3Module buildScn3Module(const Program &program);
std::map<RangeKind, size_t> countBytesByRangeKind(const Scn3Module &module);
void appendImmediateBytes(std::vector<uint8_t> &out, uint32_t value, uint8_t immWidth);
std::vector<uint8_t> encodeLowLevelInsn(const LowLevelInsn &insn);
std::string instructionSummary(const LowLevelInsn &insn);
std::string ownerNameAt(const Scn3Module &module, uint32_t offset);
void applyPatchImm(Scn3Module &module, uint32_t offset, uint32_t value);
std::vector<uint8_t> emitScn3Module(const Scn3Module &module);
void compareEmittedBytes(const Scn3Module &module, const std::vector<uint8_t> &emitted);
size_t unownedByteCount(const Scn3Module &module);
ScenarioHeader parseHeader(const std::vector<uint8_t> &blob);
ParsedHeader parseHeaderSemantics(const ScenarioHeader &header);
std::string scenarioFlagsText(uint32_t flags);
std::string tokenString(uint32_t token);
std::string formatScriptFunctionName(const Program &program, uint32_t offset);
Instruction decodeInstruction(const std::vector<uint8_t> &blob, uint32_t off, uint32_t functionBase, uint32_t codeEnd);
void writeScenarioHeader(std::ostream &out, const Scn3Module &module);

const LowLevelInsn *findInstructionOwningOffset(const Scn3Module &module, uint32_t offset);

bool isReservedOpcode(uint8_t opcode);
std::string discoveryReasonName(DiscoveryReason reason);
std::string rangeKindName(RangeKind kind);
int discoveryReasonPriority(DiscoveryReason reason);
std::optional<std::string> comparisonOperatorForAlu(Alu operation);
std::string comparisonRawText(Alu sub);

} // namespace shenmue::scenario
