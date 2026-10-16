#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0140_AREA{SHEN_NAME(0140)};
inline constexpr shen_name AREA_0140_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0140_A010{SHEN_NAME(a010)};
inline constexpr shen_name AREA_0140_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0140_A546{SHEN_NAME(a546)};
inline constexpr shen_name AREA_0140_A548{SHEN_NAME(a548)};
inline constexpr shen_name AREA_0140_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0140_SHK_{SHEN_NAME(SHK_)};
inline constexpr shen_name AREA_0140_CHARACTERS[]{AREA_0140_JOP_, AREA_0140_SHK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0140_MOTION[] = "0140A.MOT", AREA_0140_CONVERSATION[] = "0140";

extern const ScriptAreaDefinition area0140ScriptDefinition;

} // namespace shenmue::s2::areas
