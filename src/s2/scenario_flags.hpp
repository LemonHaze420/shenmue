#pragma once
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/scenario_flags_dc.hpp"
#else
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {

struct ScenarioPersistentFlagBanks {
    std::int32_t initialized;
    std::array<std::uint8_t, 8> story;
    std::array<std::uint8_t, 512> internal;
    std::array<std::uint8_t, 64> stats;
    std::array<std::byte, 128> scratch;
};
static_assert(offsetof(ScenarioPersistentFlagBanks, story) == 0x04);
static_assert(offsetof(ScenarioPersistentFlagBanks, internal) == 0x0c);
static_assert(offsetof(ScenarioPersistentFlagBanks, stats) == 0x20c);
static_assert(offsetof(ScenarioPersistentFlagBanks, scratch) == 0x24c);
static_assert(sizeof(ScenarioPersistentFlagBanks) == 0x2cc);
// The main icon ticks SCEN's memo-flag notification; the HUD hint reads whether its jingle has played.
void TickScenarioFlagNotification();
std::int32_t GetScenarioFlagNotificationShown();

enum class PlayerFlagReadMode : std::int32_t {
    Internal = 0x0b,
    Story = 0x0d,
    Stat = 0x0f,
};

enum class PlayerFlagWriteMode : std::int32_t {
    Internal = 0x0c,
    Story = 0x0e,
    Stat = 0x10,
};

bool GetInternalFlag(std::int32_t flagIndex);
#if SHENMUE_S2_GCC295
std::int32_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
#else
std::int64_t EV_GetPlayerFlag(PlayerFlagReadMode mode, std::int16_t flag);
#endif
#if SHENMUE_S2_GCC295
void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int32_t flag, std::int32_t value);
#else
void EV_SetPlayerFlag(PlayerFlagWriteMode mode, std::int16_t flag, std::int32_t value);
#endif
std::uint8_t ClearOrSetShenmueFlags(std::int32_t flag, std::int32_t enabled);
std::int64_t SetScenarioByteStat(std::int32_t index, std::int32_t value);
std::int64_t ResetScenarioPersistentFlags();
std::byte *GetScenarioPersistentScratch();
std::uint8_t *GetScenarioFlagBanks(std::uint8_t **banks);

} // namespace shenmue::s2

#endif
