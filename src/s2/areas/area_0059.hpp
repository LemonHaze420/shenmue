#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0059_AREA{SHEN_NAME(0059)}, AREA_0059_AB00{SHEN_NAME(AB00)}, AREA_0059_WON{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0059_A021{SHEN_NAME(a021) | 0x100U};
inline constexpr shen_name AREA_0059_A126{SHEN_NAME(a126)};
inline constexpr shen_name AREA_0059_A005{SHEN_NAME(a005) | 0x20100U};
inline constexpr shen_name AREA_0059_A020{SHEN_NAME(a020) | 0x4000100U};
inline constexpr shen_name AREA_0059_A103{SHEN_NAME(a103) | 0x20000U};
inline constexpr shen_name AREA_0059_A022{SHEN_NAME(a022) | 0x100U};
inline constexpr shen_name AREA_0059_CHARACTERS[]{
    SHEN_NAME(WON_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0059ScriptDefinition;
} // namespace shenmue::s2::areas
