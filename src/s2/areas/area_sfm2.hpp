#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_street_debug.h"
#include "s2/areas/area_loop_dc.h"

#define SFM2_SHS SHEN_NAME4('S', 'H', 'S', '_')
#define SFM2_KZN SHEN_NAME4('K', 'Z', 'N', '_')
#define SFM2_SSY SHEN_NAME4('S', 'S', 'Y', '_')
#define SFM2_SF2 SHEN_NAME4('S', 'F', '2', '_')
#define SFM2_CHN SHEN_NAME4('C', 'H', 'N', '_')
#define SFM2_PACKAGE SHEN_NAME4('Q', 'S', 'F', 'A')
#define SFM2_BOOKIE_CAMERA SHEN_NAME4('d', 'u', 'm', 'A')
#define SFM2_KZN_CAMERA SHEN_NAME4('d', 'u', 'm', 'J')
#define SFM2_SSY_CAMERA SHEN_NAME4('d', 'u', 'm', 'K')
#define SFM2_SF2_CAMERA SHEN_NAME4('d', 'u', 'm', 'L')
#define SFM2_OMAKE_SCENE SHEN_NAME4('O', 'M', 'A', 'K')
#define SFM2_QRON_SCENE SHEN_NAME4('Q', 'R', '0', '0')
#define SFM2_OPEN_HAND_POSE 11
#define SFM2_CHEST_HAND_POSE 18
#define SFM2_GREETING_HAND_POSE 41

void Sfm2Main(StreetFightStartParameters *start);
void QueueNormal(StreetFightStartParameters *start);
void LoadNormalResources(StreetFightEventWork *work);
void PlaceNormal(StreetFightEventWork *work);
StreetFightEventWork *InitializeNormal(HLibTask *task);
void NormalDebugCallback(void *data);
void CleanupNormalCallback(void *data);
void SetFighterIndex(StreetFightEventWork *work, int rank);
void MainCallback(void *data);
void RetryBeginCallback(void *data);
void RetryChoiceCallback(void *data);
void RetryResetCallback(void *data);
void RetryCancelCallback(void *data);
int FirstIntroConversation(void *data);
int RepeatIntroConversation(void *data);
int AcceptConversation(void *data);
int NextAcceptConversation(void *data);
int StopConversation(void *data);
int FirstBattleConversation(void *data);
int NextBattleConversation(void *data);
int DeclineConversation(void *data);
int InsufficientConversation(void *data);
void BattleEntryCallback(void *data);
void BattlePreEventCallback(void *data);
void BattlePreInitCallback(void *data);
void BattleStartCallback(void *data);
void BattleReadyGoCallback(void *data);
void BattleReleaseCallback(void *data);
void BattleWaitCallback(void *data);
void BattleWaitFadeCallback(void *data);
void BattleEndCallback(void *data);
int VictoryConversation(void *data);
int VictoryAppearanceConversation(void *data);
int DefeatConversation(void *data);
short SelectKznBattleVoice(void *data);
short SelectSsyBattleVoice(void *data);
short SelectSf2BattleVoice(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFM2_BASE{SHEN_NAME(BASE)}, SFM2_FUY{SHEN_NAME(FUY_)};
inline constexpr shen_name SFM2_DEBUG_PACKAGE{SHEN_NAME(QSFA)}, SFM2_RESOURCE_PACKAGE{SHEN_NAME(Q200)};
inline constexpr shen_name SFM2_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, SFM2_FUY, STREET_FIGHT_INVALID};
inline constexpr Vector3 SFM2_STAGE_ORIGIN{174.39999389648438f, 100.5f, 233.57240295410156f};
inline constexpr Vector3 SFM2_DEBUG_OPPONENT_ORIGIN{174.39999389648438f, 100.0f, 233.57240295410156f};
inline constexpr Vector3 SFM2_DEBUG_PLAYER_POSITION{-9.199999809265137f, -.5f, -10.399999618530273f};
inline constexpr Vector3 SFM2_DEBUG_BATTLE_BOOKIE_POSITION{-3.3f, -.5f, -5.5f};
inline constexpr Vector3 SFM2_DEBUG_EXIT_POSITION{176.8000030517578f, 100.0f, 228.3000030517578f};
inline constexpr Vector3i SFM2_DEBUG_EXIT_ROTATION{0, 30947, 0};
inline constexpr Vector3 SFM2_DEBUG_CONVERSATION_PLAYER_POSITION{0, -.05000000074505806f, -1.2999999523162842f};
inline constexpr Vector3 SFM2_DEBUG_HIDDEN_FIGHTER_POSITION{0, -10000, -1.2999999523162842f};
inline constexpr std::int8_t SFM2_DEBUG_WIN_INCREMENTS[]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8};
inline constexpr shen_name SFM2_SHS{SHEN_NAME(SHS_)}, SFM2_KZN{SHEN_NAME(KZN_)}, SFM2_SSY{SHEN_NAME(SSY_)}, SFM2_SF2{SHEN_NAME(SF2_)},
    SFM2_CHN{SHEN_NAME(CHN_)};
inline constexpr shen_name SFM2_RETRY_AREA{SHEN_NAME(OMAK)}, SFM2_RETURN_AREA{SHEN_NAME(QR00)};
inline constexpr shen_name SFM2_CHARACTERS[]{STREET_FIGHT_RYO, SFM2_SHS, SFM2_KZN, SFM2_SSY, SFM2_SF2, SFM2_CHN, STREET_FIGHT_INVALID};
inline constexpr std::int32_t SFM2_ENTRY_FEES[]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
inline constexpr std::uint16_t SFM2_KZN_BATTLE_VOICES[]{105, 106}, SFM2_SSY_BATTLE_VOICES[]{107, 108}, SFM2_SF2_BATTLE_VOICES[]{214, 215};
inline constexpr shen_name SFM2_BOOKIE_VOICE{SHEN_NAME(dumA)}, SFM2_KZN_VOICE{SHEN_NAME(dumJ)}, SFM2_SSY_VOICE{SHEN_NAME(dumK)},
    SFM2_SF2_VOICE{SHEN_NAME(dumL)};
inline constexpr Vector3 SFM2_BATTLE_BOOKIE_POSITION{-3.299999952316284f, -0.5f, -5.5f};
inline constexpr Vector3 SFM2_PAIR_FIGHTER_POSITION{-0.5f, 0.0f, 1.0f};
inline constexpr Vector3 SFM2_TRIO_FIGHTER_POSITION{0.0f, 0.0f, 1.100000023841858f};
inline constexpr Vector3 SFM2_REPEAT_FIGHTER_POSITION{-0.5f, 0.0f, -1.0f};
inline constexpr Vector3 SFM2_VICTORY_FIGHTER_POSITION{-1.2879999876022339f, 0.0f, -2.200000047683716f};
inline constexpr Vector3 SFM2_VICTORY_SPECTATOR_POSITION{4.099999904632568f, -0.5f, 1.399999976158142f};
inline constexpr Vector3 SFM2_VICTORY_APPEARANCE_POSITION{182.52000427246094f, 100.0f, 230.57000732421875f};
inline constexpr shen_name SFM2_NORMAL_PACKAGE{SHEN_NAME(QSFA)};
extern const ScriptAreaDefinition areaSfm2ScriptDefinition;
} // namespace shenmue::s2
#endif
