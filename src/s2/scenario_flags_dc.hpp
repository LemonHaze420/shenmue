#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include <stddef.h>

namespace shenmue {
namespace s2 {

struct ScenarioPersistentFlagBanks {
    std::uint8_t story[8];
    std::uint8_t internal[512];
    std::uint8_t stats[64];
    std::byte scratch[128];
};
typedef char ScenarioFlagLayoutCheck0[(offsetof(ScenarioPersistentFlagBanks, story) == 0x00) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck1[(offsetof(ScenarioPersistentFlagBanks, internal) == 0x08) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck2[(offsetof(ScenarioPersistentFlagBanks, stats) == 0x208) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck3[(offsetof(ScenarioPersistentFlagBanks, scratch) == 0x248) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck4[(sizeof(ScenarioPersistentFlagBanks) == 0x2c8) ? 1 : -1];
typedef std::int32_t PlayerFlagReadMode;
enum { PlayerFlagReadMode_Internal = 0x0b, PlayerFlagReadMode_Story = 0x0d, PlayerFlagReadMode_Stat = 0x0f };
typedef std::int32_t PlayerFlagWriteMode;
enum { PlayerFlagWriteMode_Internal = 0x0c, PlayerFlagWriteMode_Story = 0x0e, PlayerFlagWriteMode_Stat = 0x10 };
typedef std::uint8_t ShenFlagOp;
enum {
    ShenFlagOp_End = 0,
    ShenFlagOp_StepAtLeastToggleA = 1,
    ShenFlagOp_StepAtLeastToggleB = 2,
    ShenFlagOp_RequireInternalFlagSet = 3,
    ShenFlagOp_RequireInternalFlagClear = 4,
    ShenFlagOp_CommitGroup = 5,
    ShenFlagOp_ResetGroup = 6,
    ShenFlagOp_ToggleMode = 7,
    ShenFlagOp_RequireFlag = 8,
    ShenFlagOp_Unused09 = 9,
    ShenFlagOp_RequireTimeInRange = 0xc,
    ShenFlagOp_RequireItem = 0xe,
    ShenFlagOp_RequireStatAtLeast = 0x10,
    ShenFlagOp_RequireStatBelow = 0x11
};
extern ScenarioPersistentFlagBanks g_scenarioPersistentFlags;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern const std::int8_t scenarioFlagBitMasks[8];
#endif
struct ScenarioValueOperand {
    std::uint32_t mode;
    std::int32_t index;
};
typedef char ScenarioFlagLayoutCheck5[(sizeof(ScenarioValueOperand) == 8) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck6[(offsetof(ScenarioValueOperand, index) == 4) ? 1 : -1];
struct ScenarioGameplayFlagState {
    std::uint32_t words[10];
    std::int32_t initialized, day, step;
};
typedef char ScenarioFlagLayoutCheck7[(sizeof(ScenarioGameplayFlagState) == 0x34) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck8[(offsetof(ScenarioGameplayFlagState, initialized) == 0x28) ? 1 : -1];
static const std::int32_t scenarioEventValueMaximums[8] = {85, 31, 4, 1, 3, 1, 15, 1};
static const std::uint32_t scenarioEventValueShifts[8] = {0, 8, 14, 18, 20, 23, 25, 30};
static const std::uint32_t scenarioEventValueMasks[8] = {0xff, 0x3f, 0xf, 3, 7, 3, 0x1f, 3};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::uint32_t g_scenarioGameplayFlags[10];
#else
extern ScenarioGameplayFlagState g_scenarioGameplayFlags;
#endif
extern void *g_scenarioFlagNotificationJingle;
extern std::int32_t g_scenarioFlagNotificationPending, g_scenarioFlagNotificationFrames;
extern std::int32_t g_scenarioFlagNotificationShown;
std::int32_t GetScenarioFlagNotificationShown();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::int32_t g_scenarioTransientState[5];
void SetScenarioTransientState(std::int32_t index, std::int32_t value);
std::int32_t GetScenarioTransientState(std::int32_t index);
#endif

void TickScenarioFlagNotification();
void NotifyScenarioMemoFlag(std::int32_t flag);
void ClearOrSetShenmueFlags(std::int32_t flag, std::int32_t enabled);
void SetStoryFlag(std::int32_t flag, std::int32_t enabled);
void SetScenarioByteStat(std::int32_t index, std::int32_t value);
void AssignScenarioValue(const ScenarioValueOperand &operand, std::int32_t value);
std::int32_t ResolveScenarioValue(const ScenarioValueOperand &operand);
std::int32_t ResolveScntGameDataOperand(std::int32_t index);
void AssignScntGameDataOperand(std::int32_t index, std::int32_t value);
void UpdateGameplayFlags();
bool GetInternalFlag(std::int32_t flag);
bool GetStoryFlag(std::int32_t flag);
std::uint8_t GetScenarioByteStat(std::int32_t index);
std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int32_t flag, std::int32_t value);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetScenarioPersistentFlags();
#else
std::int64_t ResetScenarioPersistentFlags();
#endif
const std::int32_t *FindFlagDefinition(std::int32_t flag);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetShenmueFlag(std::int32_t flag, std::int32_t = 0);
#else
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif
std::int32_t EV_GetShenmueFlag(std::int32_t flag);
#if SHENMUE_IS_DC && SHENMUE_IS_S2

struct FlagDefinition {
    std::int32_t stepModeTable;
    std::uint16_t stepModeCount, retained06;
};
typedef char ScenarioFlagLayoutCheck9[(sizeof(FlagDefinition) == 8) ? 1 : -1];

typedef std::uint8_t StepFlagOp;
enum {
    StepFlagOp_End = 0,
    StepFlagOp_SetInternalFlag = 2,
    StepFlagOp_ClearInternalFlag = 3,
    StepFlagOp_SetStep = 4,
    StepFlagOp_RaiseStep = 5,
    StepFlagOp_ApplyInvertedFlagSteps = 6,
    StepFlagOp_Extended = 7,
    StepFlagOp_ClearStat = 9,
    StepFlagOp_AddStat = 10,
    StepFlagOp_SubtractStat = 11
};
typedef std::uint32_t StepFlagExtendedOp;
enum {
    StepFlagExtendedOp_Skip3 = 3,
    StepFlagExtendedOp_SetDefaultStats = 7,
    StepFlagExtendedOp_SelectSleepSchedule = 8,
    StepFlagExtendedOp_RandomizeStats = 9
};
typedef std::uint8_t StepRangeOp;
enum {
    StepRangeOp_End = 0,
    StepRangeOp_MinimumStep = 1,
    StepRangeOp_MaximumStep = 2,
    StepRangeOp_SetInternalFlag = 3,
    StepRangeOp_ClearInternalFlag = 4,
    StepRangeOp_EndGroup = 5,
    StepRangeOp_EndGroupB = 6,
    StepRangeOp_ResetRange = 7,
    StepRangeOp_Include = 8,
    StepRangeOp_EndGroupC = 9,
    StepRangeOp_RaiseStat = 16,
    StepRangeOp_LowerStat = 17
};
enum StepRangeMode { StepRangeMode_Minimum = 0, StepRangeMode_Maximum = 1, StepRangeMode_ApplyFlags = 2, StepRangeMode_ApplyAndSetStep = 3 };

struct SleepScheduleEntry {
    std::int16_t flag, holdWindowStartMinute, holdWindowEndMinute;
    std::uint16_t kind;
    std::int16_t entryId, retained0a;
    std::uint32_t area;
};
typedef char ScenarioFlagLayoutCheck10[(sizeof(SleepScheduleEntry) == 0x10) ? 1 : -1];
typedef char ScenarioFlagLayoutCheck11[(offsetof(SleepScheduleEntry, area) == 0xc) ? 1 : -1];
static const std::uint32_t sleepDefaultArea = SHEN_NAME4('K', 'R', 'H', '1');
static const SleepScheduleEntry sleepScheduleEntries[24] = {
    {16, 5940, 1379, 0x500, 40, 0, SHEN_NAME4('W', 'S', '0', '0')},
    {891, 5940, 1379, 0x501, 41, 0, SHEN_NAME4('W', 'T', 'A', '0')},
    {203, 5940, 1379, 0x502, 40, 0, SHEN_NAME4('W', 'T', 'A', '0')},
    {52, 5940, 1260, 0x603, 80, 0, SHEN_NAME4('W', 'K', '0', '0')},
    {111, 5940, 1379, 0x504, 40, 0, SHEN_NAME4('W', 'N', '0', '0')},
    {205, 5940, 1170, 0x605, 0, 0, SHEN_NAME4('T', 'O', 'F', 'B')},
    {1125, 5940, 1679, 0x605, 40, 0, SHEN_NAME4('A', 'R', '0', '3')},
    {204, 5940, 1379, 0x606, 95, 0, SHEN_NAME4('A', 'B', '0', '0')},
    {125, 5940, 1379, 0x607, 0, 0, SHEN_NAME4('A', 'K', '0', '0')},
    {202, 5940, 1379, 0x509, 40, 0, SHEN_NAME4('W', 'B', '0', '0')},
    {455, 5940, 1499, 0x50a, 77, 0, SHEN_NAME4('Y', 'M', 'Q', 'B')},
    {133, 5940, 1379, 0x50b, 0, 0, SHEN_NAME4('S', 'Y', 'W', '0')},
    {771, 5940, 1559, 0x50c, 40, 0, SHEN_NAME4('Q', 'R', '0', '0')},
    {772, 5940, 1559, 0x50d, 41, 0, SHEN_NAME4('Q', 'R', '0', '0')},
    {736, 5940, 780, 0x50e, 40, 0, SHEN_NAME4('Q', 'U', 'G', '0')},
    {241, 5940, 705, 0x50f, 0, 0, SHEN_NAME4('0', '5', '2', '3')},
    {235, 5940, 990, 0x510, 0, 0, SHEN_NAME4('K', 'W', 'Q', 'A')},
    {501, 1320, 1380, 0x101, 86, 0, SHEN_NAME4('A', 'R', '0', '1')},
    {502, 1320, 1380, 0x102, 79, 0, SHEN_NAME4('W', 'S', 'Y', '0')},
    {503, 1320, 1380, 0x103, 80, 0, SHEN_NAME4('W', 'T', 'A', '0')},
    {504, 1320, 1380, 0x104, 95, 0, SHEN_NAME4('A', 'B', '0', '0')},
    {505, 1320, 1380, 0x105, 80, 0, SHEN_NAME4('Q', 'R', 'R', '0')},
    {670, 29940, 29940, 0x000, 1, 0, SHEN_NAME4('0', '0', '0', '0')},
    {},
};

std::int32_t stepEvaluateFlagStepRange(std::int32_t flag, std::uint32_t mode);

std::uint32_t stepUpdateFlagUtil(std::int32_t flag, std::int32_t mode);
#endif

} // namespace s2
} // namespace shenmue

#endif
