#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0216_AREA{SHEN_NAME(0216)};
inline constexpr shen_name AREA_0216_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0216_L101{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0216_A329{SHEN_NAME(a329)};
inline constexpr shen_name AREA_0216_A530{SHEN_NAME(a530)};
inline constexpr shen_name AREA_0216_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0216_MOTION[] = "0216A.MOT", AREA_0216_CONVERSATION[] = "0216";
inline constexpr const char *AREA_0216_DEBUG_NAMES[]{"0216A000.c.c\n", "0216A010.c.c\n"};

extern const ScriptAreaDefinition area0216ScriptDefinition;

} // namespace shenmue::s2::areas
