#pragma once

#include "scn3/frontend/ast.hpp"
#include "scn3/native/abi.hpp"

namespace shenmue::scenario {

std::string baseText(AddrBase base);
std::string addrKindText(AddrKind kind);
std::string rawAddrLine(const Instruction &insn);
std::string callFamilyName(const Instruction &insn);
std::string instructionBytes(const Program &program, const Instruction &insn);
std::string disassemblyText(const Program &program, const Instruction &insn);
bool shouldRenderFunction(const FunctionInfo &function, const std::set<uint32_t> &referencedStarts);
std::set<uint32_t> collectReferencedFunctionStarts(const Program &program);
void renderDisassembly(const Program &program, bool showBytes);
size_t countLocalCallInstructions(const Program &program);
size_t countCallsPublicIndex(const Program &program, abi::Family family, uint16_t publicIndex);
size_t countRenderedFunctions(const Program &program);
std::map<DiscoveryReason, size_t> countFunctionsByDiscoveryReason(const Program &program);
size_t uncoveredByteCount(const Program &program);
size_t packedPaddingByteCount(const Program &program);
size_t countUnreferencedPackedNextFunctions(const Program &program);
void renderAudit(const Program &program);
int stackDeltaWords(const Instruction &insn);
std::string irOpName(const Instruction &insn);
void renderDumpIr(const Program &program, uint32_t start);
void renderDumpLayout(const Program &program);
void runRoundtripCheck(const Program &program);
void printHeader(const ScenarioHeader &header);
std::string bytesToHexLine(const std::vector<uint8_t> &bytes);
void dumpSourceDataSymbol(const SourceUnit &unit, const std::string &symbol);

} // namespace shenmue::scenario
