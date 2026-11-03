#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_street_debug.h"

#define SFBR_SD5 SHEN_NAME4('S', 'D', '5', '_')
#define SFBR_SFX SHEN_NAME4('S', 'F', 'X', '_')
#define SFBR_MKZI SHEN_NAME4('M', 'K', 'Z', 'I')
#define SFBR_BOOKIE_CAMERA SHEN_NAME4('S', 'F', '5', '_')
#define SFBR_OMAKE_SCENE SHEN_NAME4('O', 'M', 'A', 'K')

void SfbrMain(StreetFightStartParameters *start);
void QueueNormal(StreetFightStartParameters *start);
void SetupNormalActors(StreetFightEventWork *work);
StreetFightEventWork *InitializeNormal(HLibTask *task);
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
void BattleStartCallback(void *data);
void BattleWaitCallback(void *data);
void BattleWaitFadeCallback(void *data);
void BattleEndCallback(void *data);
int VictoryConversation(void *data);
int DefeatConversation(void *data);
int ExpiredConversation(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/street_fight_event.hpp"
namespace shenmue::s2 {
inline constexpr shen_name SFBR_SD5{SHEN_NAME(SD5_)}, SFBR_SFX{SHEN_NAME(SFX_)}, SFBR_MKZI{SHEN_NAME(MKZI)};
inline constexpr shen_name SFBR_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name SFBR_CHARACTERS[]{STREET_FIGHT_RYO, SFBR_SD5, SFBR_SFX, STREET_FIGHT_INVALID};
inline constexpr shen_name SFBR_DEBUG_CHARACTERS[]{STREET_FIGHT_RYO, STREET_FIGHT_BOOKIE, STREET_FIGHT_INVALID};
inline constexpr std::int32_t SFBR_BETS[16]{10, 20, 30, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
inline constexpr std::uint8_t SFBR_PROGRESS_WEIGHTS[12]{0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 0};
inline constexpr Vector3 SFBR_BASE_POSITION{.x = 266.2200012207031f, .y = 97.3499984741211f, .z = 526.7999877929688f};
inline constexpr Vector3 SFBR_DEBUG_STAGE_POSITION{.x = 266.2200012207031f, .y = 96.8499984741211f, .z = 526.7999877929688f};
inline constexpr Vector3 SFBR_ARRIVAL_POSITION{.x = -9.199999809265137f, .y = -0.5f, .z = -10.399999618530273f};
inline constexpr Vector3 SFBR_BOOKIE_POSITION{.x = -1.8639999628067017f, .y = 0, .z = -1.7999999523162842f};
inline constexpr Vector3 SFBR_EXIT_POSITION{.x = 260, .y = 96.8499984741211f, .z = 526.7999877929688f};
inline constexpr Vector3i SFBR_EXIT_ROTATION{.x = 0, .y = 49152, .z = 0};
inline constexpr Vector3 SFBR_HIDDEN_MARKER_POSITION{.x = 0, .y = 10000, .z = 0};
inline constexpr Vector3 SFBR_CONVERSATION_FIGHTER_POSITION{.x = -1.2879999876022339f, .y = 0, .z = -2.200000047683716f};
inline constexpr shen_name SFBR_OMAK{SHEN_NAME(OMAK)};
extern const ScriptAreaDefinition areaSfbrScriptDefinition;
} // namespace shenmue::s2
#endif
