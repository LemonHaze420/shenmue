#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0080_AREA{SHEN_NAME(0080)};
inline constexpr shen_name AREA_0080_AA48{SHEN_NAME(aa48)};
inline constexpr shen_name AREA_0080_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0080_AKY0{SHEN_NAME(AKY0)};
inline constexpr shen_name AREA_0080_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0080_MOTION[] = "0080A.MOT", AREA_0080_CONVERSATION[] = "0080";

extern const ScriptAreaDefinition area0080ScriptDefinition;

} // namespace shenmue::s2::areas
