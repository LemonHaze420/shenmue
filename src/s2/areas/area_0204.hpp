#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0204_AREA{SHEN_NAME(0204)};
inline constexpr shen_name AREA_0204_A358{SHEN_NAME(a358)};
inline constexpr shen_name AREA_0204_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0204_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0204_CHARACTERS[]{AREA_0204_CLJ_, AREA_AUTHOR_REN, AREA_0204_JOP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0204_MOTION[] = "0204A.MOT", AREA_0204_CONVERSATION[] = "0204";
inline constexpr const char *AREA_0204_DEBUG_NAMES[]{"0204A000.c\n", "0204A001.c\n", "0204A010.c\n", "0204A011.c\n", "0204A012.c\n", "0204A013.c\n",
    "0204A014.c\n", "0204A003.c\n", "0204A004.c\n", "0204A015.c\n", "0204A016.c\n"};

inline constexpr char AREA_0204_AUDIO[] = "0204A000";
extern const ScriptAreaDefinition area0204ScriptDefinition;

} // namespace shenmue::s2::areas
