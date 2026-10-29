#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace shenmue::scenario {

enum class ScenarioDataType : std::uint8_t {
    U8,
    U16,
    U32,
    I8,
    I16,
    I32,
    F32,
    ScriptOffset32,
};

struct ScenarioDataField {
    const char *name;
    std::uint32_t offset;
    std::uint32_t elementCount;
    ScenarioDataType type;
    const char *pointeeType = nullptr;
};

struct ScenarioDataSchema {
    const char *name;
    std::uint32_t size;
    std::span<const ScenarioDataField> fields;
    std::uint32_t alignment = 1;
};

struct ScenarioDataView {
    std::uint32_t start;
    std::uint32_t end;
    const ScenarioDataSchema *schema;
};

std::span<const ScenarioDataSchema> scenarioDataSchemas();
const ScenarioDataSchema *findScenarioDataSchema(std::string_view name);

struct ScenarioScriptField {
    std::string name;
    std::uint32_t offset;
    std::uint32_t elementCount;
    ScenarioDataType type;
    std::string pointeeType;
};
void registerScenarioScriptSchema(const std::string &name, std::uint32_t size, std::uint32_t alignment, const std::vector<ScenarioScriptField> &fields);
std::uint32_t scenarioDataTypeSize(ScenarioDataType type);
std::optional<std::uint32_t> scenarioDataConstantValue(std::string_view name);
const char *scenarioPointeeCType(const char *pointee);

} // namespace shenmue::scenario
