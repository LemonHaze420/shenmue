#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0057_AREA{SHEN_NAME(0057)};
inline constexpr shen_name AREA_0057_A095{SHEN_NAME(a095)};
inline constexpr shen_name AREA_0057_AB20{SHEN_NAME(ab20)};
inline constexpr shen_name AREA_0057_AA48{SHEN_NAME(aa48)};
inline constexpr shen_name AREA_0057_A316{SHEN_NAME(a316)};
inline constexpr shen_name AREA_0057_A118{SHEN_NAME(a118)};
inline constexpr shen_name AREA_0057_A358{SHEN_NAME(a358)};
inline constexpr shen_name AREA_0057_A028{SHEN_NAME(a028)};
inline constexpr shen_name AREA_0057_A357{SHEN_NAME(a357)};
inline constexpr shen_name AREA_0057_L103{SHEN_NAME(l103)};
inline constexpr shen_name AREA_0057_L046{SHEN_NAME(l046)};
inline constexpr shen_name AREA_0057_SAM_{SHEN_NAME(SAM_)};
inline constexpr shen_name AREA_0057_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0057_LLY_{SHEN_NAME(LLY_)};
inline constexpr shen_name AREA_0057_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0057_SAM_, AREA_0057_CLJ_, AREA_0057_LLY_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0057_MOTION[] = "0057A.MOT", AREA_0057_CONVERSATION[] = "0057";
inline constexpr char AREA_0057_AUDIO[] = "0057A000";
inline constexpr const char *AREA_0057_DEBUG_NAMES[]{"0057A010.c\n", "0057A040.c\n", "0057A030.c\n"};
extern const ScriptAreaDefinition area0057ScriptDefinition;

} // namespace shenmue::s2::areas
