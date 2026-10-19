#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0250_AREA{SHEN_NAME(0250)}, AREA_0250_NEXT{SHEN_NAME(0252)}, AREA_0250_LIGHT{SHEN_NAME(l002)};
inline constexpr shen_name AREA_0250_AB07{SHEN_NAME(ab07)}, AREA_0250_A297{SHEN_NAME(a297)}, AREA_0250_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0250_PROPS[]{SHEN_NAME(ab33), SHEN_NAME(ab34), SHEN_NAME(ab35), SHEN_NAME(ab36)};
inline constexpr shen_name AREA_0250_HIDDEN_PROPS[]{SHEN_NAME(F002), SHEN_NAME(R101), SHEN_NAME(R102), SHEN_NAME(R103), SHEN_NAME(R104), SHEN_NAME(R105),
    SHEN_NAME(R143), SHEN_NAME(R144), SHEN_NAME(R145), SHEN_NAME(R208), SHEN_NAME(R209)};
inline constexpr shen_name AREA_0250_CHARACTERS[]{SHEN_NAME(SYP_), SHEN_NAME(WON_), SHEN_NAME(SDJ_), shen_name{0xffffffffU}};
inline constexpr std::uint16_t AREA_0250_PALETTE[]{785, 785, 1041, 1297, 1554, 1810, 2610, 2866, 2882, 3152, 3440, 3696, 3712, 4000, 4035, 4054};
extern const ScriptAreaDefinition area0250ScriptDefinition;
} // namespace shenmue::s2::areas
