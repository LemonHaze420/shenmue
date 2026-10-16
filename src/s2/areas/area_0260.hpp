#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0260_AREA{SHEN_NAME(0260)}, AREA_0260_LIGHT{SHEN_NAME(0000)}, AREA_0260_LIGHT_SPECIAL{SHEN_NAME(l901)};
inline constexpr shen_name AREA_0260_A800{SHEN_NAME(a800)}, AREA_0260_A798{SHEN_NAME(a798)}, AREA_0260_JOP{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0260_A516{SHEN_NAME(a516)}, AREA_0260_A512{SHEN_NAME(a512)}, AREA_0260_A734{SHEN_NAME(a734)};
inline constexpr shen_name AREA_0260_SF4{SHEN_NAME(SF4_)};
inline constexpr shen_name AREA_0260_CHARACTERS_0[]{
    SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(KF3_), SHEN_NAME(KR1_), SHEN_NAME(KR2_), SHEN_NAME(SF4_), SHEN_NAME(SF9_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0260_CHARACTERS_1[]{
    SHEN_NAME(JOP_), SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(KR2_), SHEN_NAME(SF4_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0260_CHARACTERS_2[]{
    SHEN_NAME(KR2_), SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(BF6_), SHEN_NAME(SF4_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0260_CHARACTERS_3[]{
    SHEN_NAME(JOP_), SHEN_NAME(KF1_), SHEN_NAME(KF2_), SHEN_NAME(KR2_), SHEN_NAME(SF4_), SHEN_NAME(BF6_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0260_CHARACTERS_4[]{SHEN_NAME(JOP_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0260_CHARACTERS_5[]{SHEN_NAME(SF4_), shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0260_CHARACTERS_6[]{SHEN_NAME(SF4_), shen_name{0xffffffffU}};
inline constexpr const shen_name *AREA_0260_CHARACTER_TABLES[]{AREA_0260_CHARACTERS_0, AREA_0260_CHARACTERS_1, AREA_0260_CHARACTERS_2, AREA_0260_CHARACTERS_3,
    AREA_0260_CHARACTERS_4, AREA_0260_CHARACTERS_5, AREA_0260_CHARACTERS_6};
inline constexpr std::int32_t AREA_0260_DIGIT_PARTS[]{
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 162, 163, 174, 185, 196, 207, 231, 233, 234, -1};
inline constexpr std::int32_t AREA_0260_PANEL_PARTS[]{152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 163, 174, 185, 196, 207, 249, 250, 251, -1};
inline constexpr std::int32_t AREA_0260_TENS_PARTS[]{152, 163, 174, 185, 196};
inline constexpr const char *AREA_0260_LABELS[]{"0260A000.c", "0260A010.c", "0260A070.c", "0260A080.c", "0260A090.c", "0260A030.c", "0260A050.c"};
struct E0260PanelWork {
    std::int32_t fromFloor, toFloor, currentFloor, tens, delay;
};
static_assert(sizeof(E0260PanelWork) == 20);
extern const ScriptAreaDefinition area0260ScriptDefinition;
} // namespace shenmue::s2::areas
