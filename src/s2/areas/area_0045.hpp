#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0045_AREA{SHEN_NAME(0045)};
inline constexpr shen_name AREA_0045_A219{SHEN_NAME(a219)};
inline constexpr shen_name AREA_0045_A244{SHEN_NAME(a244)};
inline constexpr shen_name AREA_0045_A245{SHEN_NAME(a245)};
inline constexpr shen_name AREA_0045_SAY_{SHEN_NAME(SAY_)};
inline constexpr shen_name AREA_0045_III_{SHEN_NAME(III_)};
inline constexpr shen_name AREA_0045_SEE_{SHEN_NAME(SEE_)};
inline constexpr shen_name AREA_0045_SJN_{SHEN_NAME(SJN_)};
inline constexpr shen_name AREA_0045_GNB_{SHEN_NAME(GNB_)};
inline constexpr shen_name AREA_0045_MYU_{SHEN_NAME(MYU_)};
inline constexpr shen_name AREA_0045_CHARACTERS[]{
    AREA_0045_SAY_, AREA_0045_III_, AREA_0045_SEE_, AREA_0045_SJN_, AREA_0045_GNB_, AREA_0045_MYU_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0045_MOTION[] = "0045A.MOT", AREA_0045_CONVERSATION[] = "0045";
inline constexpr const char *AREA_0045_DEBUG_NAMES[]{"0045A000.c\n", "0045A020.c\n", "0045A030.c\n"};
extern const ScriptAreaDefinition area0045ScriptDefinition;

} // namespace shenmue::s2::areas
