#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0062_AREA{SHEN_NAME(0062)};
inline constexpr shen_name AREA_0062_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0062_A014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0062_CHARACTERS[]{AREA_0062_CLJ_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0062_MOTION[] = "0062A.MOT", AREA_0062_CONVERSATION[] = "0062";
inline constexpr const char *AREA_0062_DEBUG_NAMES[]{"0062A000.c\n"};

extern const ScriptAreaDefinition area0062ScriptDefinition;

} // namespace shenmue::s2::areas
