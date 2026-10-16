#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0235_AREA{SHEN_NAME(0235)};
inline constexpr shen_name AREA_0235_L101{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0235_A371{SHEN_NAME(a371)};
inline constexpr shen_name AREA_0235_A368{SHEN_NAME(a368)};
inline constexpr shen_name AREA_0235_A367{SHEN_NAME(a367)};
inline constexpr shen_name AREA_0235_A369{SHEN_NAME(a369)};
inline constexpr shen_name AREA_0235_A370{SHEN_NAME(a370)};
inline constexpr shen_name AREA_0235_A516{SHEN_NAME(a516)};
inline constexpr shen_name AREA_0235_A512{SHEN_NAME(a512)};
inline constexpr shen_name AREA_0235_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0235_OHS_{SHEN_NAME(OHS_)};
inline constexpr shen_name AREA_0235_JIA_{SHEN_NAME(JIA_)};
inline constexpr shen_name AREA_0235_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0235_YUA_, AREA_0235_OHS_, AREA_0235_JIA_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0235_MOTION[] = "0235A.MOT", AREA_0235_CONVERSATION[] = "0235";
inline constexpr const char *AREA_0235_DEBUG_NAMES[]{"0235A010.c.c\n", "0235A011.c.c\n", "0235A012.c.c\n"};

extern const ScriptAreaDefinition area0235ScriptDefinition;

} // namespace shenmue::s2::areas
