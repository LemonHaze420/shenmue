#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0007_AREA{SHEN_NAME(0007)}, AREA_0007_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0007_LIGHT{SHEN_NAME(0000)}, AREA_0007_RCKO{SHEN_NAME(RCKO)};
inline constexpr shen_name AREA_0007_AA39{SHEN_NAME(aa39) | 0x40000U};
inline constexpr shen_name AREA_0007_AUTHORS[]{SHEN_NAME(007A), SHEN_NAME(007B)};
inline constexpr shen_name AREA_0007_JOP{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0007_SHI{SHEN_NAME(SHI_)};
inline constexpr shen_name AREA_0007_UNH{SHEN_NAME(UNH_)};
inline constexpr shen_name AREA_0007_GOI{SHEN_NAME(GOI_)};
inline constexpr shen_name AREA_0007_HTK{SHEN_NAME(HTK_)};
inline constexpr std::uint16_t AREA_0007_DUCK_GUTS_FLAG = 0x8b4U;
inline constexpr shen_name AREA_0007_CHARACTERS[]{
    SHEN_NAME(JOP_),
    SHEN_NAME(SHI_),
    SHEN_NAME(UNH_),
    SHEN_NAME(GOI_),
    SHEN_NAME(HTK_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0007ScriptDefinition;
} // namespace shenmue::s2::areas
