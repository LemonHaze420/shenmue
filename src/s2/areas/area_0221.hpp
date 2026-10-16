#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0221_AREA{SHEN_NAME(0221)};
inline constexpr shen_name AREA_0221_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0221_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0221_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0221_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0221_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0221_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0221_CHARACTERS[]{AREA_0221_WON_, AREA_AUTHOR_REN, AREA_0221_CLJ_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0221_MOTION[] = "0221A.MOT", AREA_0221_CONVERSATION[] = "0221";
inline constexpr const char *AREA_0221_DEBUG_NAMES[]{"0221A000.c\n", "0221A010.c\n"};

inline constexpr shen_name AREA_0221_REN{SHEN_NAME(REN_)}, AREA_0221_QR00{SHEN_NAME(QR00)};
extern const ScriptAreaDefinition area0221ScriptDefinition;

} // namespace shenmue::s2::areas
