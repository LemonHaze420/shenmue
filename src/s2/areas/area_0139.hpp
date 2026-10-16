#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0139_AREA{SHEN_NAME(0139)};
inline constexpr shen_name AREA_0139_SYN_{SHEN_NAME(SYN_)};
inline constexpr shen_name AREA_0139_A450{SHEN_NAME(a450)};
inline constexpr shen_name AREA_0139_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0139_AR03{SHEN_NAME(AR03)};
inline constexpr shen_name AREA_0139_CHARACTERS[]{AREA_0139_SYN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0139_MOTION[] = "0139A.MOT", AREA_0139_CONVERSATION[] = "0139";

extern const ScriptAreaDefinition area0139ScriptDefinition;

} // namespace shenmue::s2::areas
