#include "scn3/common/util.hpp"
#include "scn3/frontend/c_frontend.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario::c_frontend {

std::string cTypeName(std::string name) {
    for (const std::string &qualifier : {std::string("struct ")}) {
        size_t at;
        while ((at = name.find(qualifier)) != std::string::npos)
            name.erase(at, qualifier.size());
    }
    if (name.find('*') == std::string::npos) {
        while (name.starts_with("const ") || name.starts_with("volatile "))
            name.erase(0, name.find(' ') + 1);
    }
    if (name == "bool")
        return "_Bool";
    if (name == "int" || name == "long")
        return "int32_t";
    if (name == "unsigned int" || name == "unsigned long" || name == "shen_name" || name == "scn3_word")
        return "uint32_t";
    if (name == "signed char" || name == "char")
        return "int8_t";
    if (name == "unsigned char")
        return "uint8_t";
    if (name == "short")
        return "int16_t";
    if (name == "unsigned short")
        return "uint16_t";
    return name;
}

ObjectType objectType(const CType &type) {
    ObjectType result{.element = type.name};
    const CType *element = &type;
    if (type.kind == CType::Kind::Array && !type.inner.empty()) {
        result.array = true;
        result.count = type.count;
        element = &type.inner.front();
        result.element = element->name;
    }
    result.pointer = element->kind == CType::Kind::Pointer || element->kind == CType::Kind::FunctionPointer;
    result.functionPointer = element->kind == CType::Kind::FunctionPointer;
    if (result.pointer) {
        result.size = 4;
        result.alignment = 4;
    } else if (element->kind == CType::Kind::Array) {
        const ObjectType row = objectType(*element);
        result.size = row.size * row.count;
        result.alignment = row.alignment;
    } else {
        const ScenarioDataSchema *schema = findScenarioDataSchema(result.element);
        const std::optional<ScenarioDataType> dataType = scenarioDataTypeByName(result.element);
        result.size = schema ? schema->size : dataType ? scenarioDataTypeSize(*dataType) : 0;
        result.alignment = schema ? schema->alignment : result.size;
    }
    if (!result.size || !result.count || result.count > 0x7fffffffU / result.size)
        throw std::runtime_error("C object type has no VM layout: " + type.name);
    return result;
}

} // namespace shenmue::scenario::c_frontend
