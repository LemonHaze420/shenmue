#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0077_AREA{SHEN_NAME(0077)};
inline constexpr shen_name AREA_0077_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0077_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0077_A456{SHEN_NAME(a456)};
inline constexpr shen_name AREA_0077_CHARACTERS[]{AREA_0077_JOP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0077_MOTION[] = "0077A.MOT", AREA_0077_CONVERSATION[] = "0077";
inline constexpr const char *AREA_0077_DEBUG_NAMES[]{"0077A000.c\n", "0077B000.c\n"};

inline constexpr shen_name AREA_0077_WN00{SHEN_NAME(WN00)}, AREA_0077_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0077_AUTHORS[]{SHEN_NAME(077A), SHEN_NAME(077B)};
extern const ScriptAreaDefinition area0077ScriptDefinition;

} // namespace shenmue::s2::areas
