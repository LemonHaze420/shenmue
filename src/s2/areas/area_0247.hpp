#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0247_AREA{SHEN_NAME(0247)};
inline constexpr shen_name AREA_0247_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0247_L201{SHEN_NAME(l201)};
inline constexpr shen_name AREA_0247_A321{SHEN_NAME(a321)};
inline constexpr shen_name AREA_0247_A454{SHEN_NAME(a454)};
inline constexpr shen_name AREA_0247_A806{SHEN_NAME(a806)};
inline constexpr shen_name AREA_0247_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0247_MOTION[] = "0247A.MOT", AREA_0247_CONVERSATION[] = "0247";
inline constexpr const char *AREA_0247_DEBUG_NAMES[]{"0247A000.c\n", "0247A010.c\n"};

extern const ScriptAreaDefinition area0247ScriptDefinition;

} // namespace shenmue::s2::areas
