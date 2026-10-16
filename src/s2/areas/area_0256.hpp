#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0256_AREA{SHEN_NAME(0256)};
inline constexpr shen_name AREA_0256_A428{SHEN_NAME(a428)};
inline constexpr shen_name AREA_0256_A534{SHEN_NAME(a534)};
inline constexpr shen_name AREA_0256_AW3_{SHEN_NAME(AW3_)};
inline constexpr shen_name AREA_0256_SFA_{SHEN_NAME(SFA_)};
inline constexpr shen_name AREA_0256_RE1_{SHEN_NAME(RE1_)};
inline constexpr shen_name AREA_0256_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0256_CHARACTERS[]{AREA_0256_AW3_, AREA_0256_SFA_, AREA_0256_RE1_, AREA_0256_CHN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0256_MOTION[] = "0256A.MOT", AREA_0256_CONVERSATION[] = "0256";
inline constexpr const char *AREA_0256_DEBUG_NAMES[]{"0256A000.c\n", "0256A020.c\n"};

extern const ScriptAreaDefinition area0256ScriptDefinition;

} // namespace shenmue::s2::areas
