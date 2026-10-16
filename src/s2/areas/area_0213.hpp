#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0213_AREA{SHEN_NAME(0213)};
inline constexpr shen_name AREA_0213_A330{SHEN_NAME(a330)};
inline constexpr shen_name AREA_0213_SOY_{SHEN_NAME(SOY_)};
inline constexpr shen_name AREA_0213_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_0213_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0213_SOY_, AREA_0213_CHR_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0213_MOTION[] = "0213A.MOT", AREA_0213_CONVERSATION[] = "0213";
inline constexpr const char *AREA_0213_DEBUG_NAMES[]{"0213A000.c\n", "0213A010.c\n"};

extern const ScriptAreaDefinition area0213ScriptDefinition;

} // namespace shenmue::s2::areas
