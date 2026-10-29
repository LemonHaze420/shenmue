#pragma once

#include "scn3/frontend/ast.hpp"

namespace shenmue::scenario {

struct SourceSlotRef {
    uint32_t baseOffset = 0;
    uint32_t byteOffset = 0;
    bool aggregateField = false;
};
uint8_t sourceImmWidthFromName(const std::string &name);
std::optional<uint8_t> sourceImmediateWidthCallName(const std::string &name);
bool sourceExprStructurallyEquals(const SourceExpr &lhs, const SourceExpr &rhs);
void validateScenarioCSource(const std::string &text);

SourceUnit parseScenarioC(const std::string &text);
void writeScenarioCHeader(const std::string &path);

void setScenarioCIncludeDirectory(const std::filesystem::path &directory);
void addScenarioCIncludeDirectory(const std::filesystem::path &directory);
std::optional<std::filesystem::path> findScenarioCHeader(const std::string &name, const std::filesystem::path &directory = {});
SourceUnit parseScn3SourceUnit(const std::string &path);
std::optional<SourceExpr> parseExpressionText(const std::string &text);
std::optional<SourceStmt> parseStatementText(const std::string &text);
Scn3Module compileScn3Source(const std::string &path);
std::optional<uint32_t> vectorComponentOffsetFromSuffix(const std::string &suffix);
std::optional<SourceSlotRef> localSlotRefFromName(const std::string &name);
std::optional<SourceSlotRef> taskSlotRefFromName(const std::string &name);
std::optional<uint32_t> argSlotOffsetFromName(const std::string &name);
bool sourceIdentifierWantsU16(const std::string &name);
uint8_t sourceIdentifierAccessWidth(const std::string &name);
std::string stripSourceWidthSuffix(std::string name);
SourceUnit parseScenarioSourceText(const std::string &text);
Scn3Module assembleScenarioText(const std::string &path);
std::optional<ScenarioDataType> scenarioDataTypeByName(std::string_view name);
std::optional<std::uint32_t> sourceDataConstantValue(const SourceExpr &expression, bool cSemantics = false);
bool sourceConstantIsUnsigned(const SourceExpr &expression);
std::optional<SourceDataAddress> resolveSourceDataAddress(const SourceUnit &unit, const SourceExpr &expression, bool typeOnly = false);
void applySourceDataRelocations(SourceUnit &unit);

} // namespace shenmue::scenario
