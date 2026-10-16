#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0030_AREA{SHEN_NAME(0030)};
inline constexpr shen_name AREA_0030_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0030_A128{SHEN_NAME(a128)};
inline constexpr shen_name AREA_0030_RNT_{SHEN_NAME(RNT_)};
inline constexpr shen_name AREA_0030_IKO_{SHEN_NAME(IKO_)};
inline constexpr shen_name AREA_0030_III_{SHEN_NAME(III_)};
inline constexpr shen_name AREA_0030_RIK_{SHEN_NAME(RIK_)};
inline constexpr shen_name AREA_0030_KHO_{SHEN_NAME(KHO_)};
inline constexpr shen_name AREA_0030_VUN_{SHEN_NAME(VUN_)};
inline constexpr shen_name AREA_0030_KEK_{SHEN_NAME(KEK_)};
inline constexpr shen_name AREA_0030_CHARACTERS[]{
    AREA_0030_RNT_, AREA_0030_IKO_, AREA_0030_III_, AREA_0030_RIK_, AREA_0030_KHO_, AREA_0030_VUN_, AREA_0030_KEK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0030_MOTION[] = "0030A.MOT", AREA_0030_CONVERSATION[] = "0030";
inline constexpr const char *AREA_0030_DEBUG_NAMES[]{"0030A000.c.c\n", "0030A020.c.c\n"};

extern const ScriptAreaDefinition area0030ScriptDefinition;

} // namespace shenmue::s2::areas
