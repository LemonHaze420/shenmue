#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0013_AREA{SHEN_NAME(0013)}, AREA_0013_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0013_WON{SHEN_NAME(WON_)}, AREA_0013_CLJ{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0013_A014{SHEN_NAME(a014)}, AREA_0013_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0013_CHARACTERS[]{AREA_0013_WON, AREA_0013_CLJ, AREA_AUTHOR_INVALID};
extern const ScriptAreaDefinition area0013ScriptDefinition;
} // namespace shenmue::s2::areas
