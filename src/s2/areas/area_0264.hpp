#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0264_AREA{SHEN_NAME(0264)};
inline constexpr shen_name AREA_0264_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0264_A520{SHEN_NAME(a520)};
inline constexpr shen_name AREA_0264_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0264_SIK_{SHEN_NAME(SIK_)};
inline constexpr shen_name AREA_0264_SZY_{SHEN_NAME(SZY_)};
inline constexpr shen_name AREA_0264_TKY_{SHEN_NAME(TKY_)};
inline constexpr shen_name AREA_0264_0308{SHEN_NAME(0308)};
inline constexpr shen_name AREA_0264_CHARACTERS[]{AREA_0264_CHN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0264_MOTION[] = "0264A.MOT", AREA_0264_CONVERSATION[] = "0264";

extern const ScriptAreaDefinition area0264ScriptDefinition;

} // namespace shenmue::s2::areas
