#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0034_AREA{SHEN_NAME(0034)};
inline constexpr shen_name AREA_0034_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0034_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0034_A044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_0034_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0034_A050{SHEN_NAME(a050)};
inline constexpr shen_name AREA_0034_A051{SHEN_NAME(a051)};
inline constexpr shen_name AREA_0034_A052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_0034_A053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_0034_AB27{SHEN_NAME(ab27)};
inline constexpr shen_name AREA_0034_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0034_WTA0{SHEN_NAME(WTA0)};
inline constexpr shen_name AREA_0034_WBBK{SHEN_NAME(WBBK)};
inline constexpr shen_name AREA_0034_TELS{SHEN_NAME(TELS)};
inline constexpr shen_name AREA_0034_CHARACTERS[]{AREA_0034_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0034_MOTION[] = "0034A.MOT", AREA_0034_CONVERSATION[] = "0034";

extern const ScriptAreaDefinition area0034ScriptDefinition;

} // namespace shenmue::s2::areas
