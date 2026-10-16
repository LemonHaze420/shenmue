#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0032_AREA{SHEN_NAME(0032)};
inline constexpr shen_name AREA_0032_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0032_A043{SHEN_NAME(a043)};
inline constexpr shen_name AREA_0032_CHARACTERS[]{AREA_0032_SYE_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0032_MOTION[] = "0032A.MOT", AREA_0032_CONVERSATION[] = "0032";
inline constexpr Vector3 AREA_0032_SMOKE_POSITIONS[]{
    {341.24029541015625f, 128.0225067138672f, 275.2820129394531f},
    {330.6080017089844f, 128.00750732421875f, 275.2528991699219f},
};
inline constexpr const char *AREA_0032_DEBUG_NAMES[]{"0032A000.c\n", "0032A020.c\n", "0032A030.c\n"};

extern const ScriptAreaDefinition area0032ScriptDefinition;

} // namespace shenmue::s2::areas
