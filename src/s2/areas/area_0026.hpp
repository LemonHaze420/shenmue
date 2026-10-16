#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
struct E0026LoopParameter {
    AreaAuthorTaskParameter *parent;
    std::int16_t currentFrame;
    std::int16_t smokeProfile;
    std::uint32_t smoke;
    std::uint32_t leaf;
    std::uint32_t secondaryLeaf;
    std::uint32_t reserved18;
    std::uint32_t reserved1c;
};
static_assert(sizeof(E0026LoopParameter) == 0x20);
static_assert(offsetof(E0026LoopParameter, smoke) == 0x0c);
static_assert(offsetof(E0026LoopParameter, leaf) == 0x10);
extern const ScriptAreaDefinition area0026ScriptDefinition;
} // namespace shenmue::s2::areas
