#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0248_AREA{SHEN_NAME(0248)};
inline constexpr shen_name AREA_0248_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0248_A834{SHEN_NAME(a834)};
inline constexpr shen_name AREA_0248_A397{SHEN_NAME(a397)};
inline constexpr shen_name AREA_0248_A398{SHEN_NAME(a398)};
inline constexpr shen_name AREA_0248_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0248_YUA_{SHEN_NAME(YUA_)};
inline constexpr shen_name AREA_0248_SGT_{SHEN_NAME(SGT_)};
inline constexpr shen_name AREA_0248_MRC_{SHEN_NAME(MRC_)};
inline constexpr shen_name AREA_0248_SCX_{SHEN_NAME(SCX_)};
inline constexpr shen_name AREA_0248_CHARACTERS[]{
    AREA_AUTHOR_REN, AREA_0248_TGY_, AREA_0248_YUA_, AREA_0248_SGT_, AREA_0248_MRC_, AREA_0248_SCX_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0248_MOTION[] = "0248A.MOT", AREA_0248_CONVERSATION[] = "0248";

extern const ScriptAreaDefinition area0248ScriptDefinition;

} // namespace shenmue::s2::areas
