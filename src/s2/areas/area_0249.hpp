#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0249_AREA{SHEN_NAME(0249)}, AREA_0249_TENT{SHEN_NAME(TENT)};
inline constexpr shen_name AREA_0249_A297{SHEN_NAME(a297) | 0x100U}, AREA_0249_A298{SHEN_NAME(a298) | 0x100U};
inline constexpr shen_name AREA_0249_A205{SHEN_NAME(a205) | 0x30000U}, AREA_0249_A352{SHEN_NAME(a352)};
inline constexpr shen_name AREA_0249_A401{SHEN_NAME(a401)}, AREA_0249_SYE{SHEN_NAME(SYE_)}, AREA_0249_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0249_DYNAMICS_CHARACTER{825242721U};
inline constexpr shen_name AREA_0249_CHARACTERS[]{
    SHEN_NAME(REN_),
    SHEN_NAME(CHR_),
    SHEN_NAME(TGY_),
    SHEN_NAME(THG_),
    SHEN_NAME(SYP_),
    SHEN_NAME(CHO_),
    SHEN_NAME(YOH_),
    SHEN_NAME(RKN_),
    SHEN_NAME(III_),
    SHEN_NAME(SEE_),
    SHEN_NAME(SYE_),
    shen_name{0xffffffffU},
};
inline constexpr std::int32_t AREA_0249_PARTS[]{175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157, 166, 154, 173, 168, 153, 165, 160, 176, 171, 164,
    159, 161, 156, 192, 193, 180, 181, 182, 189, 190, 191, 186, 187, 188, 183, 184, 185, 177, 178, 179};
extern const ScriptAreaDefinition area0249ScriptDefinition;
} // namespace shenmue::s2::areas
