#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0076_AREA{SHEN_NAME(0076)}, AREA_0076_AR03{SHEN_NAME(AR03)};
inline constexpr shen_name AREA_0076_PIN{SHEN_NAME(PIN_) | 2U}, AREA_0076_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0076_AUTHOR{909586480U}, AREA_0076_BEER_CHARACTER{925970785U};
inline constexpr shen_name AREA_0076_JOP{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0076_GOI{SHEN_NAME(GOI_)};
inline constexpr shen_name AREA_0076_HTK{SHEN_NAME(HTK_)};
inline constexpr shen_name AREA_0076_AA39{SHEN_NAME(aa39) | 0x40000U};
inline constexpr shen_name AREA_0076_A016{SHEN_NAME(a016) | 0x1000100U};
inline constexpr shen_name AREA_0076_AA44_FIRST{SHEN_NAME(aa44) | 0x2000000U};
inline constexpr shen_name AREA_0076_AA44_SECOND{SHEN_NAME(aa44) | 0x3000000U};
inline constexpr shen_name AREA_0076_A060{SHEN_NAME(a060) | 0x8000000U};
inline constexpr shen_name AREA_0076_CHARACTERS[]{
    SHEN_NAME(JOP_),
    SHEN_NAME(GOI_),
    SHEN_NAME(HTK_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0076ScriptDefinition;
} // namespace shenmue::s2::areas
