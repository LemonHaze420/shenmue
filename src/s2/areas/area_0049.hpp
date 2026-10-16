#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0049_AREA{SHEN_NAME(0049)};
inline constexpr shen_name AREA_0049_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0049_A092{SHEN_NAME(a092)};
inline constexpr shen_name AREA_0049_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0049_CHARACTERS[]{AREA_0049_SYE_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0049_MOTION[] = "0049A.MOT", AREA_0049_CONVERSATION[] = "0049";

extern const ScriptAreaDefinition area0049ScriptDefinition;

} // namespace shenmue::s2::areas
