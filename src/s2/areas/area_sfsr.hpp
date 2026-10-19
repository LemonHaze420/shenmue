#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/event_characters.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFSR_BASE{SHEN_NAME(BASE)}, SFSR_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFSR_DEBUG_PACKAGE{SHEN_NAME(QLBT)}, SFSR_RESOURCE_PACKAGE{SHEN_NAME(Q200)};
inline constexpr shen_name SFSR_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFSR_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFSR_STAGE_ORIGIN{230.89999389648438f, 129.1999969482422f, 93.80000305175781f};
inline constexpr Vector3 SFSR_DEBUG_OPPONENT_ORIGIN{230.89999389648438f, 128.6999969482422f, 93.80000305175781f};
inline constexpr Vector3 SFSR_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFSR_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -.5f, -5.5f};
inline constexpr Vector3 SFSR_DEBUG_EXIT_POSITION{244.8000030517578f, 139.60000610351562f, 82.5999984741211f};
inline constexpr Vector3i SFSR_DEBUG_EXIT_ROTATION{0, 182, 0};
inline constexpr Vector3 SFSR_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFSR_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr std::int8_t SFSR_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr shen_name SFSR_SF3{SHEN_NAME(SF3_)}, SFSR_SFB{SHEN_NAME(SFB_)}, SFSR_AW5{SHEN_NAME(AW5_)}, SFSR_CHN{SHEN_NAME(CHN_)};
inline constexpr shen_name SFSR_SHADER_CHARACTER{SHEN_NAME(a438)}, SFSR_LIGHT_LEFT{SHEN_NAME(LGT1)}, SFSR_LIGHT_CENTRE{SHEN_NAME(LGT2)};
inline constexpr shen_name SFSR_LIGHT_RIGHT{SHEN_NAME(LGT3)}, SFSR_SPOTLIGHT_TASK{SHEN_NAME(spot)};
inline constexpr shen_name SFSR_AUTH_INTRO{SHEN_NAME(AU_0)}, SFSR_AUTH_DECLINE{SHEN_NAME(AU_1)}, SFSR_AUTH_ACCEPT{SHEN_NAME(AU_2)};
inline constexpr shen_name SFSR_AUTH_RESULT{SHEN_NAME(AU_3)}, SFSR_RETRY_AREA{SHEN_NAME(OMAK)}, SFSR_RETURN_AREA{SHEN_NAME(QR00)};
inline constexpr shen_name SFSR_RETRY_TITLE{SHEN_NAME(RTR2)};
inline constexpr shen_name SFSR_CHARACTERS[]{STREET_FIGHT_RYO, SFSR_SF3, SFSR_SFB, SFSR_CHN, SFSR_AW5, STREET_FIGHT_INVALID};
inline constexpr shen_name SFSR_AUTHOR_CHARACTERS[]{SFSR_SFB, SFSR_SF3, SFSR_CHN, SFSR_AW5, STREET_FIGHT_INVALID};
inline constexpr AuthoredPluralControlEntry SFSR_PLURAL_INTRO[]{{.frame = 10, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 10, .character = SHEN_NAME(SF3_), .control = 1}, {.frame = 10, .character = SHEN_NAME(AW5_), .control = 1},
    {.frame = 10, .character = SHEN_NAME(SFB_), .control = 1}, {.frame = 351, .character = SHEN_NAME(SF3_), .control = 0},
    {.frame = 446, .character = SHEN_NAME(SF3_), .control = 1}, {.frame = 811, .character = SHEN_NAME(RYO_), .control = 0},
    {.frame = 1036, .character = SHEN_NAME(RYO_), .control = 1}, {.frame = 1476, .character = SHEN_NAME(SF3_), .control = 0},
    {.frame = 1541, .character = SHEN_NAME(AW5_), .control = 0}, {.frame = 1651, .character = SHEN_NAME(AW5_), .control = 1},
    {.frame = 2351, .character = SHEN_NAME(SF3_), .control = 1}, {.frame = 2671, .character = SHEN_NAME(RYO_), .control = 0},
    {.frame = 2801, .character = SHEN_NAME(RYO_), .control = 1}, {.frame = 3431, .character = SHEN_NAME(RYO_), .control = 0},
    {.frame = 3706, .character = SHEN_NAME(RYO_), .control = 1}, {.frame = 3860, .character = SHEN_NAME(SF3_), .control = 0},
    {.frame = 4350, .character = SHEN_NAME(SF3_), .control = 1}, {.frame = -1, .character = shen_name{}, .control = 0}};
inline constexpr AuthoredPluralControlEntry SFSR_PLURAL_RESULT[]{{.frame = 1, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 1, .character = SHEN_NAME(AW5_), .control = 1}, {.frame = 1, .character = SHEN_NAME(SFB_), .control = 1},
    {.frame = 1, .character = SHEN_NAME(SF3_), .control = 0}, {.frame = 63, .character = SHEN_NAME(SF3_), .control = 1},
    {.frame = 208, .character = SHEN_NAME(RYO_), .control = 0}, {.frame = 326, .character = SHEN_NAME(RYO_), .control = 1},
    {.frame = 796, .character = SHEN_NAME(AW5_), .control = 0}, {.frame = 856, .character = SHEN_NAME(AW5_), .control = 1},
    {.frame = 1496, .character = SHEN_NAME(SF3_), .control = 0}, {.frame = 1549, .character = SHEN_NAME(SF3_), .control = 1},
    {.frame = 1549, .character = SHEN_NAME(RYO_), .control = 0}, {.frame = 1633, .character = SHEN_NAME(SF3_), .control = 0},
    {.frame = 1701, .character = SHEN_NAME(RYO_), .control = 1}, {.frame = 1701, .character = SHEN_NAME(SF3_), .control = 1},
    {.frame = 1989, .character = SHEN_NAME(AW5_), .control = 0}, {.frame = 2029, .character = SHEN_NAME(AW5_), .control = 1},
    {.frame = 2122, .character = SHEN_NAME(SF3_), .control = 0}, {.frame = 2329, .character = SHEN_NAME(SF3_), .control = 1},
    {.frame = -1, .character = shen_name{}, .control = 0}};
inline constexpr std::uint8_t SFSR_FIGHT_PARAMETERS[][12]{{112, 64, 64, 64, 143, 32, 0, 48, 111, 0, 0, 0}, {96, 96, 96, 96, 136, 48, 0, 42, 116, 0, 0, 0},
    {90, 104, 104, 104, 160, 64, 0, 37, 143, 0, 0, 0}, {74, 128, 128, 128, 208, 80, 0, 37, 160, 0, 0, 0}, {58, 136, 136, 136, 216, 96, 0, 37, 186, 0, 0, 0},
    {42, 144, 144, 144, 223, 112, 0, 37, 186, 0, 0, 0}};
inline constexpr Vector3 SFSR_CAMERA_TARGET{230.89999389648438f, 129.89999389648438f, 92.70000457763672f};
inline constexpr Vector3 SFSR_CAMERA_POSITION{232.6999969482422f, 130.09999084472656f, 90.10000610351562f};
inline constexpr const char *SFSR_ROUND_MESSAGES[]{"\xa1\xd6\xa3\xb1\xa3\xf3\xa3\xf4\x20\xa3\xcd\xa3\xc1\xa3\xd4\xa3\xc3\xa3\xc8\xa1\xd7",
    "\xa1\xd6\xa3\xb2\xa3\xee\xa3\xe4\x20\xa3\xcd\xa3\xc1\xa3\xd4\xa3\xc3\xa3\xc8\xa1\xd7",
    "\xa1\xd6\xa3\xc6\xa3\xc9\xa3\xce\xa3\xc1\xa3\xcc\x20\xa3\xcd\xa3\xc1\xa3\xd4\xa3\xc3\xa3\xc8\xa1\xd7"};
inline constexpr Vector4 SFSR_LEFT_LIGHT_COLOUR{1.0f, 1.0f, 0.9300000071525574f, 0.6399999856948853f};
inline constexpr Vector4 SFSR_RIGHT_LIGHT_COLOUR{1.0f, 0.9800000190734863f, 0.800000011920929f, 0.4000000059604645f};
inline constexpr Vector3 SFSR_SPOTLIGHT_ORIGIN{230.89999389648438f, 130.1999969482422f, 93.80000305175781f};
inline constexpr Vector3 SFSR_WIN_EXIT_POSITION{241.9199981689453f, 128.8000030517578f, 91.7300033569336f};
inline constexpr Vector3i SFSR_WIN_EXIT_ROTATION{0, 8192, 0};
inline constexpr Vector3 SFSR_LOSS_EXIT_POSITION{240.60000610351562f, 128.8000030517578f, 93.5f};
inline constexpr Vector3i SFSR_LOSS_EXIT_ROTATION{0, 16384, 0};
extern const ScriptAreaDefinition areaSfsrScriptDefinition;
} // namespace shenmue::s2
