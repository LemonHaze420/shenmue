#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0063_AREA{SHEN_NAME(0063)}, AREA_0063_PROP{SHEN_NAME(a067)};
inline constexpr shen_name AREA_0063_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0063_MOTION[] = "0063A.MOT", AREA_0063_CONVERSATION[] = "0063";
extern const ScriptAreaDefinition area0063ScriptDefinition;

} // namespace shenmue::s2::areas
