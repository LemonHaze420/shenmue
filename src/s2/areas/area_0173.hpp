#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0173_AREA{SHEN_NAME(0173)};
inline constexpr shen_name AREA_0173_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0173_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0173_SYP_{SHEN_NAME(SYP_)};
inline constexpr shen_name AREA_0173_CHARACTERS[]{AREA_0173_SYP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0173_MOTION[] = "0173A.MOT", AREA_0173_CONVERSATION[] = "0173";

extern const ScriptAreaDefinition area0173ScriptDefinition;

} // namespace shenmue::s2::areas
