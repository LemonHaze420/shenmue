#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0265_AREA{SHEN_NAME(0265)};
inline constexpr shen_name AREA_0265_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0265_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0265_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0265_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0265_CHN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0265_MOTION[] = "0265A.MOT", AREA_0265_CONVERSATION[] = "0265";

extern const ScriptAreaDefinition area0265ScriptDefinition;

} // namespace shenmue::s2::areas
