#pragma once

#include "s2/hlib.hpp"
#include "s2/fight_stage.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/battle_qte_ui.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_FBQT{SHEN_NAME(FBQT)};

enum class BattleQteMode : std::int32_t {
    Load = 0,
    ResetStates = 1,
    Queue = 2,
    Start = 3,
    GetResult = 4,
    RetireAndFree = 5,
    GetCompletedCount = 6,
    GetCommandCount = 7,
    SetWait = 8,
    SetFirstCommands = 9,
    SetLastCommands = 10,
    Retire = 11,
    FreeSprites = 12,
};

struct BattleQteIcon {
    std::int32_t kind, x, y;
    float scale;
};
struct BattleQteCommand {
    std::int32_t trigger;
    std::uint16_t initialWait, commandWait, resultWait, reserved;
    std::array<std::int32_t, 8> commands;
};
inline constexpr BattleQteCommand battleQteDefaultCommand{2, 30, 6, 30, 0, {0x40, 4, 0x10, 0x800, 0, 0, 0, 0}};
inline constexpr std::array<BattleQteIcon, 5> battleQteDefaultIcons{{
    {4, 206, 234, 2.0f},
    {2, 373, 232, 1.0f},
    {3, 428, 177, 1.0f},
    {0, 428, 287, 1.0f},
    {1, 483, 232, 1.0f},
}};
inline constexpr std::array<std::uint16_t, 5> battleQteIconMasks{0xf000, 4, 2, 0x400, 0x200};
inline constexpr std::array<std::uint16_t, 12> battleQteCommandMasks{
    0x1000,
    0x9000,
    0x8000,
    0xa000,
    0x2000,
    0x6000,
    0x4000,
    0x5000,
    0x400,
    0x200,
    4,
    2,
};
inline constexpr std::array<std::uint32_t, 4> battleQteDirectionMasks{0x1000, 0x2000, 0x4000, 0x8000};


struct BattleQteTaskData {
    std::array<BattleQteIcon, 5> icons;
    std::uint16_t shownMask, shownFrames;
    std::array<std::uint16_t, 5> iconFrames;
    std::uint16_t directionFlashFrames, age, directionFrames, departureFrames;
    std::uint16_t reserved66;
    std::uint32_t resultIcon, mode;
    std::uint8_t reserved70, active, inputCommand, shownCommand;
    std::uint16_t requiredMask, initialWait, commandWait, resultWait, remaining, retireWait;
    std::uint32_t reserved80;
    std::array<std::uint16_t, 8> commandAges;
};
static_assert(sizeof(BattleQteTaskData) == 0x94);
static_assert(offsetof(BattleQteTaskData, shownMask) == 0x50);
static_assert(offsetof(BattleQteTaskData, iconFrames) == 0x54);
static_assert(offsetof(BattleQteTaskData, mode) == 0x6c);
static_assert(offsetof(BattleQteTaskData, active) == 0x71);
static_assert(offsetof(BattleQteTaskData, commandAges) == 0x84);

HLib::Task *Queue_vfq_BattleQteMain();
void vfq_BattleQteMain_Callback(void *data);
std::int64_t qteBattleUtil(BattleQteMode mode, std::int32_t a2 = 0, std::int32_t a3 = 0, std::int32_t a4 = 0, std::int32_t a5 = 0);
bool vfqIsBattleQteActive() noexcept;
void ResetBattleQteTaskPointer() noexcept;
void fbqtToggleMode();
BattleQteTaskData *fbqtDisableMode();
std::uint8_t fbqtResetStates(std::uint8_t mode) noexcept;
std::uint16_t vfqConvertCommandMask(std::int32_t command) noexcept;
void vfqResetBattleQte(BattleQteTaskData &data) noexcept;
std::uint16_t vfqPollBattleQteInput(BattleQteTaskData &data);
void vfqSetBattleQteIcons(BattleQteTaskData &data, std::uint8_t mode, std::uint16_t mask, std::uint16_t frames) noexcept;
void EnableQteUi(BattleQteTaskData &data);

} // namespace shenmue::s2
