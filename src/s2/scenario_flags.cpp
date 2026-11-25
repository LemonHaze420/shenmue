#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/scenario_flags_dc.hpp"

#include "s2/calendar_dc.hpp"
#include "s2/flag_data.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/story_state.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/flag_context_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "platform/dc/sdk.hpp"
#endif
#else
#include "s2/scenario_flags.hpp"
#include "s2/scenario_internal.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

using namespace event_runtime_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool GetInternalFlag(std::int32_t flagIndex);
#if SHENMUE_S2_GCC295
std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
#else
std::int64_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
#endif
std::int64_t ResetScenarioPersistentFlags();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace event_runtime_detail {

bool ReadBit(const std::uint8_t *bytes, std::size_t byteCount, std::int32_t index) {
    return index < static_cast<std::int64_t>(byteCount * 8) &&
        (bytes[static_cast<std::int64_t>(index) >> 3] & static_cast<std::uint8_t>(1U << (index & 7))) != 0;
}

void WriteBit(std::uint8_t *bytes, std::size_t byteCount, std::int32_t index, bool value) {
    if (index >= static_cast<std::int64_t>(byteCount * 8))
        return;
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (index & 7));
    if (value)
        bytes[static_cast<std::int64_t>(index) >> 3] |= mask;
    else
        bytes[static_cast<std::int64_t>(index) >> 3] &= static_cast<std::uint8_t>(~mask);
}


void NotifyScenarioMemoFlag(std::int32_t flag) {
    scenarioFlagNotificationJingle = nullptr;
    if (!NotifyMemoFlag(flag, true))
        return;
    switch (flag) {
    case 9:
    case 88:
    case 89:
    case 106:
    case 147:
    case 304:
    case 426:
    case 455:
    case 467:
    case 513:
    case 536:
    case 552:
    case 686:
    case 1121:
    case 1127:
    case 1204:
    case 1587:
    case 1652:
    case 2100:
        return;
    default:
        scenarioFlagNotificationPending = 1;
        scenarioFlagNotificationFrames = 0;
        return;
    }
}
} // namespace event_runtime_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)



void TickScenarioFlagNotification() {
    bool suppressed = IsItemSelectable() || g_currentMap.name == SHEN_NAME(ARK0) || GetCurrentAuthorId().name == SHEN_NAME(ARK0);
    for (const shen_name name : scenarioEventObjectMapNames)
        if (name.name == SHEN_NAME(ARK0))
            suppressed = true;
    if (suppressed) {
        scenarioFlagNotificationPending = 0;
        scenarioFlagNotificationShown = 0;
    }
    if (plymIsSearchBlocked() || (activeScenarioEventRuntime->flags & 0x800U) || cvsGet() || !scenarioFlagNotificationPending ||
        ++scenarioFlagNotificationFrames <= 3)
        return;
    scenarioFlagNotificationPending = 0;
    scenarioFlagNotificationShown = 1;
    scenarioFlagNotificationJingle = shenPlayAudioWithFlags("JINGURU03", 2);
    RequestHelpIconItemPrompt();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t GetScenarioFlagNotificationShown() {
    return scenarioFlagNotificationShown;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetShenmueFlag(std::int32_t flag);
#endif

std::int32_t stepEvaluateFlagStepRange(std::int32_t flag, std::uint32_t mode);

std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);

ScenarioPersistentFlagBanks g_scenarioPersistentFlags;

extern std::byte *g_flagDescriptorData;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool GetInternalFlag(std::int32_t flag);
bool GetStoryFlag(std::int32_t flag);
std::uint8_t GetScenarioByteStat(std::int32_t index);
const std::int32_t *FindFlagDefinition(std::int32_t flag);
static std::int32_t stepFindSleepSchedule(std::int32_t flag);
static void stepSelectSleepSchedule(const std::int32_t *flags);
static void stepSetDefaultStats(const std::int32_t *arguments);
static std::int32_t stepRandomizeStats(const std::int32_t *arguments);
static std::int32_t stepExecuteExtendedCommand(std::uint32_t operation, const std::int32_t *arguments);
static const std::int32_t *stepGetFlagCommands(std::int32_t flag, std::uint32_t mode);
std::uint32_t stepUpdateFlagUtil(std::int32_t flag, std::int32_t mode);
#else
namespace event_runtime_detail {

bool ReadBit(const std::uint8_t *bytes, std::size_t byteCount, std::int32_t index);
void WriteBit(std::uint8_t *bytes, std::size_t byteCount, std::int32_t index, bool value);
void NotifyScenarioMemoFlag(std::int32_t flag);

} // namespace event_runtime_detail

void TickScenarioFlagNotification();
std::int32_t GetScenarioFlagNotificationShown();
bool GetInternalFlag(std::int32_t flagIndex);
#if SHENMUE_S2_GCC295
std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
#else
std::int64_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
#endif
std::uint8_t ClearOrSetShenmueFlags(std::int32_t flag, std::int32_t enabled);
std::int64_t SetScenarioByteStat(std::int32_t index, std::int32_t value);
std::int64_t ResetScenarioPersistentFlags();
std::byte *GetScenarioPersistentScratch();
std::uint8_t *GetScenarioFlagBanks(std::uint8_t **banks);
void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int16_t flag, std::int32_t value);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t stepUpdateFlagUtil(std::int32_t flag, std::int32_t mode) {
    std::uint32_t result = 0;
    std::int32_t running = 1;
    const std::int32_t *command = stepGetFlagCommands(flag, static_cast<std::uint8_t>(mode));
    if (!command)
        return 0;
    do {
        std::uint32_t operand = static_cast<std::uint32_t>(*command++);
        const std::uint32_t op = operand >> 24;
        operand &= 0xffffffU;
        switch (op) {
        case StepFlagOp_End:
            running = 0;
        default:
            break;
        case StepFlagOp_SetStep:
            SetSTEP(operand);
            result |= 1U;
            break;
        case StepFlagOp_RaiseStep:
            if (GetCurrentStep() < operand) {
                SetSTEP(operand);
                result |= 1U;
            }
            break;
        case StepFlagOp_SetInternalFlag:
            EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, static_cast<std::int16_t>(operand), 1);
            break;
        case StepFlagOp_ClearInternalFlag:
            EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, static_cast<std::int16_t>(operand), 0);
            break;
        case StepFlagOp_ClearStat:
            EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, static_cast<std::int16_t>(operand), 0);
            break;
        case StepFlagOp_AddStat: {
            const std::uint32_t stat = operand >> 16;
            std::int32_t value = EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(stat)) + static_cast<std::int16_t>(operand);
            if (value > 255)
                value = 255;
            EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, static_cast<std::int16_t>(stat), value);
            break;
        }
        case StepFlagOp_SubtractStat: {
            const std::uint32_t stat = operand >> 16;
            std::int32_t value = EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(stat)) - static_cast<std::int16_t>(operand);
            if (value < 0)
                value = 0;
            EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, static_cast<std::int16_t>(stat), value);
            break;
        }
        case StepFlagOp_ApplyInvertedFlagSteps:
            stepUpdateFlagUtil(operand, GetShenmueFlag(operand) ^ 1);
            break;
        case StepFlagOp_Extended:
            command += stepExecuteExtendedCommand(operand, command);
            break;
        }
    } while (running);
    return result;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const std::int32_t *FindFlagDefinition(std::int32_t flag) {
    if (!g_flagDescriptorData)
        return 0;
    if (static_cast<std::uint32_t>(flag) >= reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData)->flagCount || flag < 0)
        return 0;
    return reinterpret_cast<const std::int32_t *>(g_flagDescriptorData) +
        (reinterpret_cast<const std::int32_t *>(g_flagDescriptorData) + reinterpret_cast<const std::int32_t *>(g_flagDescriptorData)[1])[flag];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static const std::int32_t *stepGetFlagCommands(std::int32_t flag, std::uint32_t mode) {
    const std::int32_t *const definition = FindFlagDefinition(flag);
    if (definition && mode < reinterpret_cast<const FlagDefinition *>(definition)->stepModeCount) {
        return definition + *(mode + (definition + definition[0]));
    }
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t stepExecuteExtendedCommand(std::uint32_t operation, const std::int32_t *arguments) {
    switch (static_cast<StepFlagExtendedOp>(operation)) {
    case StepFlagExtendedOp_Skip3:
        return 3;
    case StepFlagExtendedOp_SetDefaultStats:
        stepSetDefaultStats(arguments);
        return 0;
    case StepFlagExtendedOp_SelectSleepSchedule:
        stepSelectSleepSchedule(arguments);
        return 0;
    case StepFlagExtendedOp_RandomizeStats:
        return stepRandomizeStats(arguments);
    default:
        return 0;
    }
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void stepSelectSleepSchedule(const std::int32_t *flags) {
    const std::int8_t lateSleepWindow = static_cast<std::int8_t>(GetShenmueFlag(528));
    GetCurrentStep();
    if (g_actNumber == 1 && !GetShenmueFlag(179)) {
        stepEvaluateFlagStepRange(179, StepRangeMode_ApplyAndSetStep);
    } else if (g_actNumber == 2 && !GetShenmueFlag(90)) {
        stepEvaluateFlagStepRange(90, StepRangeMode_ApplyAndSetStep);
    } else if (g_actNumber == 3 && !GetShenmueFlag(91)) {
        stepEvaluateFlagStepRange(91, StepRangeMode_ApplyAndSetStep);
    } else if (g_actNumber == 4 && !GetShenmueFlag(110)) {
        stepEvaluateFlagStepRange(110, StepRangeMode_ApplyAndSetStep);
    }
    std::int32_t index;
    std::int8_t argument = 0;
    do {
        if (!flags[argument]) {
            if (static_cast<std::int32_t>(GetCurrentStep()) >= 700000) {
                EV_plymSetSceneContext(30000, 30030, static_cast<std::int16_t>(g_actNumber), sleepDefaultArea, 50);
                return;
            }
            shenPlayerData.sleepScheduleFlag = -1;
            index = 0;
            break;
        }
        index = stepFindSleepSchedule(flags[argument]);
        argument = static_cast<std::int8_t>(static_cast<std::uint8_t>(argument) + 1U);
    } while (index < 0);
    const SleepScheduleEntry &entry = sleepScheduleEntries[index];
    DateTime target;
    BuildTargetDateFromFrameClock(target);
    if (lateSleepWindow && (entry.kind & 0xff00U) == 0x100U)
        EV_plymSetSceneContext(1620, 1680, static_cast<std::int16_t>(g_actNumber), entry.area, entry.entryId);
    else
        EV_plymSetSceneContext(entry.holdWindowStartMinute, entry.holdWindowEndMinute, static_cast<std::int16_t>(g_actNumber), entry.area, entry.entryId);
    if (target.hour * 60 + target.minute < entry.holdWindowEndMinute || EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 1010)) {
        EV_setHoldStatesFlags(2);
        EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, 1010, 0);
    }
    shenPlayerData.sleepScheduleFlag = sleepScheduleEntries[index].flag;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t stepFindSleepSchedule(std::int32_t flag) {
    for (std::int32_t index = 0; sleepScheduleEntries[index].area; ++index) {
        if (sleepScheduleEntries[index].flag == flag)
            return GetShenmueFlag(sleepScheduleEntries[index].flag, 0) ? index : -1;
    }
    return -1;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void stepSetDefaultStats(const std::int32_t *) {
    for (std::int32_t stat = 16; stat < 32; ++stat)
        EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, static_cast<std::int16_t>(stat), 85);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t stepRandomizeStats(const std::int32_t *arguments) {
    std::int32_t index = 1;
    while (arguments[index]) {
        const std::int16_t stat = static_cast<std::int16_t>(arguments[index]);
        const float random = dc::sdk::RandomFloat();
        const std::int32_t value = static_cast<std::int32_t>(static_cast<float>(arguments[0]) * random);
        EV_SetPlayerFlag(PlayerFlagWriteMode_Stat, stat, value);
        ++index;
    }
    return index + 1;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool GetInternalFlag(std::int32_t flag) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (flag <= 4095) {
        const std::uint8_t *data = reinterpret_cast<const std::uint8_t *>(&g_scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, internal);
        data += flag >> 3;
        if (*data & scenarioFlagBitMasks[flag & 7])
            return true;
        return false;
    }
    return false;
#else
    const std::uint8_t *const data = reinterpret_cast<const std::uint8_t *>(&g_scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, internal);
    return flag < 4096 && (data[flag >> 3] & static_cast<std::uint8_t>(1U << (flag & 7))) != 0;
#endif
}

#else

bool GetInternalFlag(std::int32_t flagIndex) {
    return flagIndex < 4096 &&
        (reinterpret_cast<const std::uint8_t *>(&scenarioPersistentFlags)[0x0c + (static_cast<std::int64_t>(flagIndex) >> 3)] &
            static_cast<std::uint8_t>(1U << (flagIndex & 7))) != 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

#if SHENMUE_S2_GCC295
std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag) {
#else
std::int64_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag) {
#endif
    switch (mode) {
    case PlayerFlagReadMode::Internal:
        return GetInternalFlag(flag);
    case PlayerFlagReadMode::Story:
        return ReadBit(
            reinterpret_cast<const std::uint8_t *>(&scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, story), storyFlags.size(), flag);
    case PlayerFlagReadMode::Stat:
        return flag < 64 ? reinterpret_cast<const std::uint8_t *>(&scenarioPersistentFlags)[0x20c + static_cast<std::int64_t>(flag)] : 0;
    }
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint8_t ClearOrSetShenmueFlags(std::int32_t flag, std::int32_t enabled) {
    if (flag >= 4096)
        return 0;
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (flag & 7));
    std::uint8_t &byte = reinterpret_cast<std::uint8_t *>(&scenarioPersistentFlags)[0x0c + (static_cast<std::int64_t>(flag) >> 3)];
    if (enabled) {
        NotifyScenarioMemoFlag(flag);
        byte |= mask;
        return mask;
    }
    const std::uint8_t cleared = static_cast<std::uint8_t>(~mask);
    byte &= cleared;
    return cleared;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t SetScenarioByteStat(std::int32_t index, std::int32_t value) {
    if (index >= 64)
        return 0;
    std::uint8_t *const bank = reinterpret_cast<std::uint8_t *>(&scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, stats);
    bank[index] = static_cast<std::uint8_t>(value < 0 ? 0 : value > 255 ? 255 : value);
    return index;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool GetStoryFlag(std::int32_t flag) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (flag <= 63) {
        const std::uint8_t *data = reinterpret_cast<const std::uint8_t *>(&g_scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, story);
        data += flag >> 3;
        if (*data & scenarioFlagBitMasks[flag & 7])
            return true;
        return false;
    }
    return false;
#else
    const std::uint8_t *const data = reinterpret_cast<const std::uint8_t *>(&g_scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, story);
    return flag < 64 && (data[flag >> 3] & static_cast<std::uint8_t>(1U << (flag & 7))) != 0;
#endif
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t GetScenarioByteStat(std::int32_t index) {
    if (index < 64)
        return g_scenarioPersistentFlags.stats[index];
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetScenarioPersistentFlags() {
#else
std::int64_t ResetScenarioPersistentFlags() {
#endif
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_scenarioPersistentFlags.story)); ++index)
        g_scenarioPersistentFlags.story[index] = 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_scenarioPersistentFlags.internal)); ++index)
        g_scenarioPersistentFlags.internal[index] = 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_scenarioPersistentFlags.stats)); ++index)
        g_scenarioPersistentFlags.stats[index] = 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return 0;
#endif
}

#else

// @verified - p
std::int64_t ResetScenarioPersistentFlags() {
    storyFlags.fill(0);
    internalFlags.fill(0);
    stats.fill(0);
    return 64;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::byte *GetScenarioPersistentScratch() {
    return scenarioPersistentScratch.data();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint8_t *GetScenarioFlagBanks(std::uint8_t **banks) {
    banks[0] = storyFlags.data();
    banks[1] = internalFlags.data();
    banks[2] = stats.data();
    return banks[2];
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int16_t flag, std::int32_t value) {
    ScenarioExpressionOperand operand{.mode = 0, .index = flag};
    switch (mode) {
    case PlayerFlagWriteMode::Internal:
        operand.mode = 2;
        break;
    case PlayerFlagWriteMode::Story:
        operand.mode = 3;
        break;
    case PlayerFlagWriteMode::Stat:
        operand.mode = 4;
        break;
    default:
        return;
    }
    AssignScenarioExpressionOperand(operand, value);
}

#endif

} // namespace s2
} // namespace shenmue
