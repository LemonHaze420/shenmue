#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0252_AREA{SHEN_NAME(0252)};
inline constexpr shen_name AREA_0252_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0252_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0252_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0252_A270{SHEN_NAME(a270)};
inline constexpr shen_name AREA_0252_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0252_A300{SHEN_NAME(a300)};
inline constexpr shen_name AREA_0252_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0252_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0252_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0252_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0252_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0252_CHO_{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0252_CHARACTERS[]{AREA_0252_CLJ_, AREA_0252_WON_, AREA_0252_JOP_, AREA_AUTHOR_REN, AREA_0252_CHO_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0252_MOTION[] = "0252A.MOT", AREA_0252_CONVERSATION[] = "0252";

extern const ScriptAreaDefinition area0252ScriptDefinition;

} // namespace shenmue::s2::areas
