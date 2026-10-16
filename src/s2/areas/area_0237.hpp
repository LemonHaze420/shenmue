#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0237_AREA{SHEN_NAME(0237)};
inline constexpr shen_name AREA_0237_CHO_{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0237_A336{SHEN_NAME(a336)};
inline constexpr shen_name AREA_0237_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0237_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0237_CHO_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0237_MOTION[] = "0237A.MOT", AREA_0237_CONVERSATION[] = "0237";

extern const ScriptAreaDefinition area0237ScriptDefinition;

} // namespace shenmue::s2::areas
