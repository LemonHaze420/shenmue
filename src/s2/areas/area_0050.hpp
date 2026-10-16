#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0050_AREA{SHEN_NAME(0050)}, AREA_0050_TKR{SHEN_NAME(TKR_)};
inline constexpr shen_name AREA_0050_AB10{SHEN_NAME(ab10)}, AREA_0050_AB11{SHEN_NAME(ab11)};
inline constexpr shen_name AREA_0050_LIGHT{SHEN_NAME(0000)}, AREA_0050_LIGHT_SPECIAL{SHEN_NAME(l441)};
inline constexpr shen_name AREA_0050_WAVE{SHEN_NAME(wave)};
inline constexpr shen_name AREA_0050_CHARACTERS[]{AREA_0050_TKR, AREA_AUTHOR_INVALID};
inline constexpr const char *AREA_0050_LABELS[]{"0050A000.c", "0050A001.c", "0050A020.c", "0050A021.c", "0050A022.c", "0050A030.c", "0050A040.c", "0050A041.c",
    "0050A042.c", "0050A043.c", "0050A044.c", "0050A035.c", "0050A045.c"};
extern const ScriptAreaDefinition area0050ScriptDefinition;
} // namespace shenmue::s2::areas
