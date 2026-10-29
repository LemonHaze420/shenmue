#pragma once

#include "scn3/native/data.hpp"

#include "common/math.hpp"

#include <type_traits>

namespace shenmue::scenario {

template <class T> struct ScenarioNativeShape {
    using ElementType = T;
    static constexpr uint32_t count = 1;
};

template <class T, size_t N> struct ScenarioNativeShape<T[N]> {
    using ElementType = typename ScenarioNativeShape<T>::ElementType;
    static constexpr uint32_t count = static_cast<uint32_t>(N) * ScenarioNativeShape<T>::count;
};

template <class T> struct ScenarioNativeShape<Vector3T<T>> {
    static_assert(std::is_standard_layout_v<Vector3T<T>> && sizeof(Vector3T<T>) == sizeof(T) * 3);
    static_assert(offsetof(Vector3T<T>, x) == 0 && offsetof(Vector3T<T>, y) == sizeof(T) && offsetof(Vector3T<T>, z) == sizeof(T) * 2);
    using ElementType = T;
    static constexpr uint32_t count = 3;
};

template <class T> consteval ScenarioDataType scenarioNativeDataType() {
    if constexpr (std::is_enum_v<T>)
        return scenarioNativeDataType<std::underlying_type_t<T>>();
    else if constexpr (std::is_same_v<T, float>)
        return ScenarioDataType::F32;
    else if constexpr (std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int8_t>)
        return ScenarioDataType::I8;
    else if constexpr (std::is_same_v<T, std::uint16_t> || std::is_same_v<T, std::int16_t>)
        return ScenarioDataType::I16;
    else if constexpr (std::is_same_v<T, std::uint32_t>)
        return ScenarioDataType::U32;
    else if constexpr (std::is_same_v<T, std::int32_t>)
        return ScenarioDataType::I32;
    else
        static_assert(!std::is_same_v<T, T>, "Unsupported scenario field type");
}

template <class T> consteval ScenarioDataField scenarioNativeField(const char *name, uint32_t offset) {
    using Shape = ScenarioNativeShape<T>;
    return ScenarioDataField{.name = name, .offset = offset, .elementCount = Shape::count, .type = scenarioNativeDataType<typename Shape::ElementType>()};
}

template <class T> consteval ScenarioDataField scenarioNativeBytes(const char *name, uint32_t offset) {
    static_assert(std::is_same_v<std::remove_all_extents_t<T>, char> || std::is_same_v<std::remove_all_extents_t<T>, std::uint8_t>);
    return ScenarioDataField{.name = name, .offset = offset, .elementCount = sizeof(T), .type = ScenarioDataType::I8};
}

template <class T> consteval ScenarioDataField scenarioNativeOffset(const char *name, uint32_t offset, const char *pointee) {
    static_assert(std::is_integral_v<T> && sizeof(T) == sizeof(uint32_t));
    return ScenarioDataField{.name = name, .offset = offset, .elementCount = 1, .type = ScenarioDataType::ScriptOffset32, .pointeeType = pointee};
}

consteval bool validScenarioSchemas(std::span<const ScenarioDataSchema> schemas) {
    for (const ScenarioDataSchema &schema : schemas) {
        uint32_t covered = 0;
        for (const ScenarioDataField &field : schema.fields) {
            const uint32_t width = field.type == ScenarioDataType::U8 || field.type == ScenarioDataType::I8 ? 1U :
                field.type == ScenarioDataType::U16 || field.type == ScenarioDataType::I16                  ? 2U :
                                                                                                              4U;
            if (covered > schema.size || field.offset != covered || field.elementCount > (schema.size - covered) / width)
                return false;
            covered += field.elementCount * width;
        }
        if (covered != schema.size)
            return false;
    }
    return true;
}

} // namespace shenmue::scenario
