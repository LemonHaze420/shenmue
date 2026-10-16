#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0058_AREA{SHEN_NAME(0058)};
inline constexpr shen_name AREA_0058_A093{SHEN_NAME(a093)};
inline constexpr shen_name AREA_0058_A291{SHEN_NAME(a291)};
inline constexpr shen_name AREA_0058_AA68{SHEN_NAME(aa68)};
inline constexpr shen_name AREA_0058_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0058_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0058_KT8_{SHEN_NAME(KT8_)};
inline constexpr shen_name AREA_0058_KTA_{SHEN_NAME(KTA_)};
inline constexpr shen_name AREA_0058_COH_{SHEN_NAME(COH_)};
inline constexpr shen_name AREA_0058_KU9_{SHEN_NAME(KU9_)};
inline constexpr shen_name AREA_0058_CHARACTERS[]{
    AREA_0058_WON_, AREA_AUTHOR_REN, AREA_0058_YUA_, AREA_0058_KT8_, AREA_0058_KTA_, AREA_0058_COH_, AREA_0058_KU9_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0058_MOTION[] = "0058A.MOT", AREA_0058_CONVERSATION[] = "0058";
inline constexpr const char *AREA_0058_DEBUG_NAMES[]{"0058A000.c\n", "0058A040.c\n", "0058A050.c\n"};

inline constexpr char AREA_0058_AUDIO[] = "0058A000";
extern const ScriptAreaDefinition area0058ScriptDefinition;

} // namespace shenmue::s2::areas
