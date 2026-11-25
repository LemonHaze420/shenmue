#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/legacy_types.hpp"
#include "common/types.hpp"
#define SHENMUE_PLAYER_ARRAY(type, name, count) type name[count]
#define SHENMUE_PLAYER_NOEXCEPT

namespace shenmue {
namespace s2 {

struct NativePlayerStats {
    std::uint32_t savedFlags;
    std::uint32_t statFlags[4];
    std::uint32_t retained;
    std::int32_t cash;
    std::int32_t segaCoins;
    std::uint16_t alternateHealth, defaultHealth, reactionCameraScale, assistScale;
    std::uint32_t gameConfigurationFlags;
    std::uint16_t contextActionMode;
    std::uint8_t optionA, optionB, actionState;
};
typedef char NativePlayerStatsSizeCheck[sizeof(NativePlayerStats) == 0x34 ? 1 : -1];
typedef char PlayerCashOffsetCheck[offsetof(NativePlayerStats, cash) == 0x18 ? 1 : -1];
typedef char PlayerConfigurationOffsetCheck[offsetof(NativePlayerStats, gameConfigurationFlags) == 0x28 ? 1 : -1];
typedef char PlayerActionStateOffsetCheck[offsetof(NativePlayerStats, actionState) == 0x30 ? 1 : -1];
struct ArcadeStatistics {
    std::uint32_t highScore;
    std::uint32_t games;
    std::uint32_t time;
};
typedef char ArcadeStatisticsSizeCheck[sizeof(ArcadeStatistics) == 12 ? 1 : -1];
extern NativePlayerStats g_nativePlayerStats;
bool vfCheckSkillFlag(shen_name flag);
std::int32_t GetPlayerStats(std::int32_t index);
void SetPlayerStat(std::int32_t index, std::uint32_t value);
extern std::int32_t g_transferredYen;
extern std::uint8_t g_fightActive, g_fightUiActive;
extern SHENMUE_PLAYER_ARRAY(std::uint32_t, g_minigameScores, 26);
std::uint32_t GetSavedMinigameScore(std::uint32_t index) SHENMUE_PLAYER_NOEXCEPT;
void SetSavedMinigameScore(std::uint32_t index, std::uint32_t score) SHENMUE_PLAYER_NOEXCEPT;
extern SHENMUE_PLAYER_ARRAY(std::uint8_t, g_playerRetainedResetWords, 16);
extern SHENMUE_PLAYER_ARRAY(std::byte, g_playerRetainedResetStorage, 1024);
extern std::uint16_t g_playerResetCounter;
extern ArcadeStatistics g_arcadeStatistics[4];
extern std::int16_t g_playerSaveSelections[3];
extern std::uint16_t g_playerSelectionCounter;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern std::uint8_t g_scenarioTalkOccurrences[512];
void RecordScenarioTalkGroup(std::int32_t index);
void RecordRecentScenarioTalk(std::int32_t index);
void RecordScenarioTalkOccurrence(std::uint16_t index);
#endif
extern std::uint32_t g_savedFrameCounter[2];
extern std::uint8_t g_retainedPlayerFlags[2];
extern std::uint32_t g_qronSceneChange[7];
extern std::uint8_t g_vfMotnNibbles[16];
extern float g_gameProgress;
extern std::uint32_t g_currentGameFlags, g_completedGameCount;
extern std::uint32_t g_mgOtoshidamaFlag;
void SetMGOTOSIDAMA(std::int32_t bit, std::int32_t enabled);
std::uint32_t GetMGOTOSIDAMA();
void ResetNativePlayerStats();
void ResetPlayerSaveSelections();
void ResetRetainedBattleState();
void ResetSavedFrameCounter();
void ResetRetainedPlayerFlags();
void ResetQronSceneChange();
void vfMotnNibbleSet(std::uint8_t code, std::int32_t value);
void SetGameCompletionState(float progress, std::uint32_t flags, std::uint32_t completed);

struct CombatStatistics {
    SHENMUE_PLAYER_ARRAY(std::byte, retainedPrefix, 0x168);
    std::int16_t sentinel;
    SHENMUE_PLAYER_ARRAY(std::byte, retainedCounters, 0x262);
    std::int32_t stepElapsedSeconds;
    SHENMUE_PLAYER_ARRAY(std::byte, retainedTail, 0xb06);
    std::uint16_t memoOpenCount;
    SHENMUE_PLAYER_ARRAY(std::byte, retainedTailEnd, 0x128);
};
typedef char CombatStatisticsSizeCheck[sizeof(CombatStatistics) == 0x1000 ? 1 : -1];
typedef char CombatStatisticsSentinelOffsetCheck[offsetof(CombatStatistics, sentinel) == 0x168 ? 1 : -1];
typedef char CombatStatisticsMemoOpenCountOffsetCheck[offsetof(CombatStatistics, memoOpenCount) == 0xed6 ? 1 : -1];
typedef char CombatStatisticsStepElapsedOffsetCheck[offsetof(CombatStatistics, stepElapsedSeconds) == 0x3cc ? 1 : -1];
extern CombatStatistics g_combatStatistics;
void InitializeCombatStatistics(CombatStatistics *statistics);
extern std::uint32_t g_stepElapsedFrames;
extern std::uint32_t g_stepDebugFrames;
extern std::int32_t g_stepElapsedSeconds;
extern std::uint32_t g_stepRangeElapsedFrames;
extern std::int32_t g_previousStoryStep, g_stepRangeStart, g_stepRangeEnd;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetStepCounterSlot(std::int32_t step);
#else
std::int32_t GetStepCounterSlot();
#endif
void ResetStepTiming(std::int32_t step);

} // namespace s2
} // namespace shenmue

#undef SHENMUE_PLAYER_ARRAY
#undef SHENMUE_PLAYER_NOEXCEPT

#endif
