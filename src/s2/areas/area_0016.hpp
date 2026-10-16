#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0016_AREA{SHEN_NAME(0016)};
inline constexpr shen_name AREA_0016_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0016_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0016_A718{SHEN_NAME(a718)};
inline constexpr shen_name AREA_0016_A298{SHEN_NAME(a298)};
inline constexpr shen_name AREA_0016_A714{SHEN_NAME(a714)};
inline constexpr shen_name AREA_0016_CHARACTERS[]{AREA_0016_JOP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0016_MOTION[] = "0016A.MOT", AREA_0016_CONVERSATION[] = "0016";
inline constexpr const char *AREA_0016_DEBUG_NAMES[]{
    "0016A000.c\n", "0016A001.c\n", "0016A002.c\n", "0016A010.c\n", "0016A011.c\n", "0016A012.c\n", "0016A030.c\n"};

extern const ScriptAreaDefinition area0016ScriptDefinition;

} // namespace shenmue::s2::areas
