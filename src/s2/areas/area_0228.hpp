#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0228_AREA{SHEN_NAME(0228)};
inline constexpr shen_name AREA_0228_A480{SHEN_NAME(a480)};
inline constexpr shen_name AREA_0228_LCN_{SHEN_NAME(LCN_)};
inline constexpr shen_name AREA_0228_CHARACTERS[]{AREA_0228_LCN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0228_MOTION[] = "0228A.MOT", AREA_0228_CONVERSATION[] = "0228";
inline constexpr const char *AREA_0228_DEBUG_NAMES[]{"0228A000.c\n"};

extern const ScriptAreaDefinition area0228ScriptDefinition;

} // namespace shenmue::s2::areas
