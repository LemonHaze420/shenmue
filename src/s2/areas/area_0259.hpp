#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0259_AREA{SHEN_NAME(0259)};
inline constexpr shen_name AREA_0259_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0259_L012{SHEN_NAME(l012)};
inline constexpr shen_name AREA_0259_L023{SHEN_NAME(l023)};
inline constexpr shen_name AREA_0259_L204{SHEN_NAME(l204)};
inline constexpr shen_name AREA_0259_A438{SHEN_NAME(a438)};
inline constexpr shen_name AREA_0259_SFB_{SHEN_NAME(SFB_)};
inline constexpr shen_name AREA_0259_SF3_{SHEN_NAME(SF3_)};
inline constexpr shen_name AREA_0259_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0259_AW5_{SHEN_NAME(AW5_)};
inline constexpr shen_name AREA_0259_CHARACTERS[]{AREA_0259_SFB_, AREA_0259_SF3_, AREA_0259_CHN_, AREA_0259_AW5_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0259_MOTION[] = "0259A.MOT", AREA_0259_CONVERSATION[] = "0259";
inline constexpr const char *AREA_0259_DEBUG_NAMES[]{"0259A000.c\n", "0259A001.c\n", "0259A002.c\n", "0259A020.c\n"};

extern const ScriptAreaDefinition area0259ScriptDefinition;

} // namespace shenmue::s2::areas
