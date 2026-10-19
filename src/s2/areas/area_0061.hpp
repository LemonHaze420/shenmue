#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0061_AREA{SHEN_NAME(0061)}, AREA_0061_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0061_SPRING{SHEN_NAME(fntn)};
inline constexpr shen_name AREA_0061_A014{SHEN_NAME(a014)}, AREA_0061_A127{SHEN_NAME(a127) | 0x10200U};
inline constexpr shen_name AREA_0061_CHARACTERS[]{SHEN_NAME(LLY_), SHEN_NAME(CLJ_), SHEN_NAME(SAM_), SHEN_NAME(REN_), AREA_AUTHOR_INVALID};
extern const ScriptAreaDefinition area0061ScriptDefinition;
void E0061SpringCallback(void *parameter);
} // namespace shenmue::s2::areas
