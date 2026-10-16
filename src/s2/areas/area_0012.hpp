#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0012_AREA{SHEN_NAME(0012)};
inline constexpr shen_name AREA_0012_L082{SHEN_NAME(l082)};
inline constexpr shen_name AREA_0012_SAM_{SHEN_NAME(SAM_)};
inline constexpr shen_name AREA_0012_LLY_{SHEN_NAME(LLY_)};
inline constexpr shen_name AREA_0012_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0012_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0012_A014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0012_A100{SHEN_NAME(a100)};
inline constexpr shen_name AREA_0012_A101{SHEN_NAME(a101)};
inline constexpr shen_name AREA_0012_CHARACTERS[]{AREA_0012_WON_, AREA_0012_CLJ_, AREA_0012_SAM_, AREA_0012_LLY_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0012_MOTION[] = "0012A.MOT", AREA_0012_CONVERSATION[] = "0012";
inline constexpr const char *AREA_0012_DEBUG_NAMES[]{"0012A010.c\n", "0012A012.c\n", "0012A013.c\n", "0012A014.c\n", "0012A022.c\n", "0012A032.c\n"};

inline constexpr char AREA_0012_MEDIA_ZERO[] = "0011E000", AREA_0012_MEDIA_OTHER[] = "0012E011";
extern const ScriptAreaDefinition area0012ScriptDefinition;

} // namespace shenmue::s2::areas
