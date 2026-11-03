#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_street_debug.h"
#include "s2/areas/area_loop_dc.h"

#define SFKM_OD2 SHEN_NAME4('O', 'D', '2', '_')
#define SFKM_AW1 SHEN_NAME4('A', 'W', '1', '_')
#define SFKM_CHN SHEN_NAME4('C', 'H', 'N', '_')
#define SFKM_PACKAGE SHEN_NAME4('Q', 'S', 'F', 'C')
#define SFKM_BOOKIE_CAMERA SHEN_NAME4('d', 'u', 'm', 'A')
#define SFKM_FIGHTER_CAMERA SHEN_NAME4('d', 'u', 'm', 'Q')
#define SFKM_OMAKE_SCENE SHEN_NAME4('O', 'M', 'A', 'K')
#define SFKM_QRON_SCENE SHEN_NAME4('Q', 'R', '0', '0')
#define SFKM_INTRO_HAND_POSE 41
#define SFKM_VICTORY_HAND_POSE 12
#define SFKM_DEFEAT_HAND_POSE 40

void SfkmMain(StreetFightStartParameters *start);
void QueueNormal(StreetFightStartParameters *start);
void PlaceNormal(StreetFightEventWork *work);
void NormalDebugCallback(void *data);
void CleanupNormalCallback(void *data);
void MainCallback(void *data);
void RetryBeginCallback(void *data);
void RetryChoiceCallback(void *data);
void RetryResetCallback(void *data);
void RetryCancelCallback(void *data);
int FirstIntroConversation(void *data);
int RepeatIntroConversation(void *data);
int AcceptConversation(void *data);
int DeclineConversation(void *data);
int InsufficientConversation(void *data);
void BattleEntryCallback(void *data);
void BattlePreEventCallback(void *data);
void BattlePreInitCallback(void *data);
void BattleStartCallback(void *data);
short SelectFirstBattleVoice(void *data);
short SelectSecondBattleVoice(void *data);
void BattleWaitCallback(void *data);
void BattleEndCallback(void *data);
int VictoryConversation(void *data);
int PostVictoryConversation(void *data);
int DefeatConversation(void *data);
int TimeoutConversation(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFKM_BASE{SHEN_NAME(BASE)}, SFKM_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFKM_DEBUG_PACKAGE{SHEN_NAME(QSFC)};
inline constexpr shen_name SFKM_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFKM_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFKM_STAGE_ORIGIN{268.20001220703125f, 95.33000183105469f, 42.06999969482422f};
inline constexpr Vector3 SFKM_DEBUG_OPPONENT_ORIGIN{268.20001220703125f, 94.83000183105469f, 42.06999969482422f};
inline constexpr Vector3 SFKM_DEBUG_PLAYER_POSITION{-9.199999809265137f, -0.5f, -10.399999618530273f};
inline constexpr Vector3 SFKM_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -0.5f, -5.5f};
inline constexpr Vector3 SFKM_DEBUG_EXIT_POSITION{264.80999755859375f, 94.83000183105469f, 48.9900016784668f};
inline constexpr Vector3i SFKM_DEBUG_EXIT_ROTATION{0, 63575, 0};
inline constexpr std::int8_t SFKM_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr Vector3 SFKM_DEBUG_CONVERSATION_PLAYER_POSITION{0, -0.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFKM_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr Vector3 SFKM_FIGHTER_INTRO_POSITION{-1.2879999876022339f, 0.0f, -2.200000047683716f};
inline constexpr Vector3 SFKM_FIGHTER_GREETING_POSITION{-0.2879999876022339f, 0.0f, -0.20000000298023224f};
inline constexpr Vector3 SFKM_PLAYER_REPEAT_POSITION{-3.2899999618530273f, -0.5f, -5.46999979019165f};
inline constexpr Vector3 SFKM_FIGHTER_ACCEPT_POSITION{-0.2879999876022339f, 0.0f, -1.2000000476837158f};
inline constexpr Vector3 SFKM_FIGHTER_WIN_POSITION{-0.2879999876022339f, 0.0f, -1.7000000476837158f};
inline constexpr Vector3 SFKM_BATTLE_BOOKIE_POSITION{-3.299999952316284f, -0.5f, -5.5f};
inline constexpr shen_name SFKM_OD2{SHEN_NAME(OD2_)}, SFKM_AW1{SHEN_NAME(AW1_)}, SFKM_CHN{SHEN_NAME(CHN_)};
inline constexpr shen_name SFKM_BOOKIE_VOICE{SHEN_NAME(dumA)}, SFKM_FIGHTER_VOICE{SHEN_NAME(dumQ)};
inline constexpr shen_name SFKM_CHARACTERS[]{STREET_FIGHT_RYO, SFKM_OD2, SFKM_AW1, SFKM_CHN, STREET_FIGHT_INVALID};
inline constexpr std::int32_t SFKM_ENTRY_FEES[]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
inline constexpr std::uint16_t SFKM_FIRST_BATTLE_VOICES[]{165, 166, 167, 178}, SFKM_SECOND_BATTLE_VOICES[]{169, 170};
inline std::int32_t SFKM_GREETING_HAND[19][3]{
    {1999, 1456, 60440},
    {1638, 65354, 64810},
    {546, 61988, 4093},
    {0, 64811, 60257},
    {0, 64081, 59891},
    {0, 726, 64625},
    {0, 0, 64628},
    {0, 0, 61348},
    {0, 0, 59347},
    {0, 65172, 63717},
    {0, 0, 65354},
    {0, 729, 62439},
    {0, 65355, 59165},
    {0, 64810, 64991},
    {0, 0, 0},
    {0, 909, 58617},
    {0, 1456, 60618},
    {0, 63354, 182},
    {64080, 182, 0},
};
inline std::int32_t SFKM_VICTORY_HAND[19][3]{
    {1999, 65354, 62444},
    {1638, 180, 63900},
    {546, 64719, 3365},
    {0, 64810, 60984},
    {0, 64628, 61893},
    {0, 0, 64261},
    {0, 0, 64628},
    {0, 0, 60074},
    {0, 0, 59710},
    {0, 65172, 63353},
    {0, 0, 65354},
    {0, 729, 61165},
    {0, 65173, 58073},
    {0, 65172, 64444},
    {0, 0, 0},
    {0, 547, 57161},
    {0, 1639, 61165},
    {0, 64810, 64080},
    {64080, 182, 0},
};
inline std::int32_t SFKM_DEFEAT_HAND[19][3]{
    {1999, 65354, 59167},
    {1638, 543, 64810},
    {546, 63262, 4821},
    {0, 64810, 56252},
    {0, 64081, 58800},
    {0, 727, 363},
    {0, 0, 64628},
    {0, 0, 57709},
    {0, 0, 57344},
    {0, 65172, 65354},
    {0, 0, 65354},
    {0, 729, 60074},
    {0, 65355, 57526},
    {0, 64809, 182},
    {0, 0, 0},
    {0, 910, 55705},
    {0, 1456, 59164},
    {0, 63354, 1092},
    {64080, 182, 0},
};
inline constexpr shen_name SFKM_RETRY_AREA{SHEN_NAME(OMAK)}, SFKM_RETURN_AREA{SHEN_NAME(QR00)};
extern const ScriptAreaDefinition areaSfkmScriptDefinition;
} // namespace shenmue::s2
#endif
