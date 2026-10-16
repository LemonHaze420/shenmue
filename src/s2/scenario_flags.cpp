#include "s2/scenario_flags.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

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

std::int32_t GetScenarioFlagNotificationShown() {
    return scenarioFlagNotificationShown;
}

bool GetInternalFlag(std::int32_t flagIndex) {
    return flagIndex < 4096 &&
        (reinterpret_cast<const std::uint8_t *>(&scenarioPersistentFlags)[0x0c + (static_cast<std::int64_t>(flagIndex) >> 3)] &
            static_cast<std::uint8_t>(1U << (flagIndex & 7))) != 0;
}

std::int64_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag) {
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

std::int64_t SetScenarioByteStat(std::int32_t index, std::int32_t value) {
    if (index >= 64)
        return 0;
    std::uint8_t *const bank = reinterpret_cast<std::uint8_t *>(&scenarioPersistentFlags) + offsetof(ScenarioPersistentFlagBanks, stats);
    bank[index] = static_cast<std::uint8_t>(value < 0 ? 0 : value > 255 ? 255 : value);
    return index;
}

// @verified - p
std::int64_t ResetScenarioPersistentFlags() {
    storyFlags.fill(0);
    internalFlags.fill(0);
    stats.fill(0);
    return 64;
}

// @verified - p
std::byte *GetScenarioPersistentScratch() {
    return scenarioPersistentScratch.data();
}

// @verified - p
std::uint8_t *GetScenarioFlagBanks(std::uint8_t **banks) {
    banks[0] = storyFlags.data();
    banks[1] = internalFlags.data();
    banks[2] = stats.data();
    return banks[2];
}

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

} // namespace shenmue::s2
