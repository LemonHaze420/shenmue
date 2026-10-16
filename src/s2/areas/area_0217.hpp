#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0217_AREA{SHEN_NAME(0217)};
inline constexpr shen_name AREA_0217_A514{SHEN_NAME(a514)};
inline constexpr shen_name AREA_0217_A516{SHEN_NAME(a516)};
inline constexpr shen_name AREA_0217_A512{SHEN_NAME(a512)};
inline constexpr shen_name AREA_0217_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0217_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0217_CHARACTERS[]{AREA_0217_TGY_, AREA_0217_YUA_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0217_MOTION[] = "0217A.MOT", AREA_0217_CONVERSATION[] = "0217";
inline constexpr const char *AREA_0217_DEBUG_NAMES[]{"0217A000.c\n", "0217A020.c\n", "0217A030.c\n"};

extern const ScriptAreaDefinition area0217ScriptDefinition;

} // namespace shenmue::s2::areas
