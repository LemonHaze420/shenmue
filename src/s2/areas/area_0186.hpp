#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0186_AREA{SHEN_NAME(0186)};
inline constexpr shen_name AREA_0186_FIRST_AUTHOR{0x41363831U}, AREA_0186_SECOND_AUTHOR{0x42363831U};
inline constexpr shen_name AREA_0186_A126{SHEN_NAME(a126)};
inline constexpr shen_name AREA_0186_AB00{SHEN_NAME(AB00)};
inline constexpr shen_name AREA_0186_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0186_A142{SHEN_NAME(a142)};
inline constexpr shen_name AREA_0186_A506{SHEN_NAME(a506)};
inline constexpr shen_name AREA_0186_A504{SHEN_NAME(a504)};
inline constexpr shen_name AREA_0186_A648{SHEN_NAME(a648)};
inline constexpr shen_name AREA_0186_CHARACTERS[]{AREA_0186_WON_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0186_MOTION[] = "0186A.MOT", AREA_0186_CONVERSATION[] = "0186";

extern const ScriptAreaDefinition area0186ScriptDefinition;

} // namespace shenmue::s2::areas
