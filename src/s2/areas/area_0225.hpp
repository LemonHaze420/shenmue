#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0225_AREA{SHEN_NAME(0225)};
inline constexpr shen_name AREA_0225_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0225_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0225_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0225_A300{SHEN_NAME(a300)};
inline constexpr shen_name AREA_0225_A224{SHEN_NAME(a224)};
inline constexpr shen_name AREA_0225_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0225_A223{SHEN_NAME(a223)};
inline constexpr shen_name AREA_0225_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0225_MOTION[] = "0225A.MOT", AREA_0225_CONVERSATION[] = "0225";
inline constexpr const char *AREA_0225_DEBUG_NAMES[]{"0225A000.c\n", "0225A020.c\n"};

inline constexpr char AREA_0225_AUDIO[] = "0225A000";
extern const ScriptAreaDefinition area0225ScriptDefinition;

} // namespace shenmue::s2::areas
