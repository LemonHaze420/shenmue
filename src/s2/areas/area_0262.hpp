#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0262_AREA{SHEN_NAME(0262)};
inline constexpr shen_name AREA_0262_A276{SHEN_NAME(a276)};
inline constexpr shen_name AREA_0262_AA80{SHEN_NAME(aa80)};
inline constexpr shen_name AREA_0262_AA77{SHEN_NAME(aa77)};
inline constexpr shen_name AREA_0262_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0262_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0262_262A{SHEN_NAME(262A)};
inline constexpr shen_name AREA_0262_262B{SHEN_NAME(262B)};
inline constexpr shen_name AREA_0262_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0262_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0262_WON_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0262_MOTION[] = "0262A.MOT", AREA_0262_CONVERSATION[] = "0262";
inline constexpr shen_name AREA_0262_AUTHORS[]{AREA_0262_262A, AREA_0262_262B};
extern const ScriptAreaDefinition area0262ScriptDefinition;

} // namespace shenmue::s2::areas
