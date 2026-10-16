#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0215_AREA{SHEN_NAME(0215)};
inline constexpr shen_name AREA_0215_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0215_A067{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0215_A413{SHEN_NAME(a413)};
inline constexpr shen_name AREA_0215_A530{SHEN_NAME(a530)};
inline constexpr shen_name AREA_0215_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0215_MOTION[] = "0215A.MOT", AREA_0215_CONVERSATION[] = "0215";
inline constexpr const char *AREA_0215_DEBUG_NAMES[]{"0215A000.c\n"};

extern const ScriptAreaDefinition area0215ScriptDefinition;

} // namespace shenmue::s2::areas
