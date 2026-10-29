#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario {

struct SourceDataDecl {
    uint32_t start = 0;
    uint32_t end = 0;
    std::string type;
    std::string name;
};
std::string sourceDataName(uint32_t offset);
std::string sourceStringName(uint32_t offset);
std::string sourcePadName(uint32_t offset);
bool moduleDataContains(const Scn3Module &module, uint32_t offset);
std::set<uint32_t> collectSourceDataReferenceOffsets(const Scn3Module &module);
bool isPrintableCStringByte(uint8_t byte);
size_t printableCStringLength(const std::vector<uint8_t> &blob, uint32_t start, uint32_t end);
std::string sourceEscapeString(const std::vector<uint8_t> &blob, uint32_t start, size_t length);
std::vector<SourceDataDecl> planSourceDataDecls(
    const std::vector<uint8_t> &blob, uint32_t start, uint32_t end, const std::set<uint32_t> &forcedOffsets, std::span<const ScenarioDataView> typedViews = {});
std::map<uint32_t, std::string> sourceDataSymbolMap(const std::vector<SourceDataDecl> &decls);
void writeBytesArray(std::ostream &out, const std::vector<uint8_t> &blob, uint32_t start, uint32_t end, const std::string &indent);
void renderSourceDataDecls(std::ostream &out, const std::vector<uint8_t> &blob, const std::vector<SourceDataDecl> &decls, const std::string &indent);
void renderSourceRawRange(std::ostream &out, const Scn3Module &module, const ByteRange &range);
std::vector<ScenarioDataView> discoverScenarioDataViews(const Program &program, const Scn3Module &module, std::set<std::uint32_t> &references);
void renderScenarioDataRecord(std::ostream &out, const std::vector<std::uint8_t> &blob, std::uint32_t offset, const ScenarioDataSchema &schema,
    const std::map<std::uint32_t, std::string> &symbols, const std::string &indent);

} // namespace shenmue::scenario
