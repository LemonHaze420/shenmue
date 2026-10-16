#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0066_AREA{SHEN_NAME(0066)};
inline constexpr shen_name AREA_0066_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0066_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0066_A890{SHEN_NAME(a890)};
inline constexpr shen_name AREA_0066_A540{SHEN_NAME(a540)};
inline constexpr shen_name AREA_0066_A044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_0066_A045{SHEN_NAME(a045)};
inline constexpr shen_name AREA_0066_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0066_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0066_WTA0{SHEN_NAME(WTA0)};
inline constexpr shen_name AREA_0066_0001{SHEN_NAME(0001)};
inline constexpr shen_name AREA_0066_WT00{SHEN_NAME(WT00)};
inline constexpr shen_name AREA_0066_CHARACTERS[]{AREA_0066_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0066_MOTION[] = "0066A.MOT", AREA_0066_CONVERSATION[] = "0066";

extern const ScriptAreaDefinition area0066ScriptDefinition;

} // namespace shenmue::s2::areas
