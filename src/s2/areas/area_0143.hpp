#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0143_AREA{SHEN_NAME(0143)};
inline constexpr shen_name AREA_0143_WON{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0143_CHARACTERS[]{AREA_0143_WON, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0143_MOTION[] = "0143A.MOT";
inline constexpr char AREA_0143_CONVERSATION[] = "0143";
extern const ScriptAreaDefinition area0143ScriptDefinition;
} // namespace shenmue::s2::areas
