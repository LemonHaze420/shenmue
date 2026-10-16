#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name TASK_SLT1{SHEN_NAME(SLT1)};
inline constexpr shen_name CHAR_SLOT_EYE{SHEN_NAME(SEYE)};
struct Slt1EnqueueParams {
    shen_name machine;
    std::int32_t entry;
};
struct Slt1Work {
    HLib::Task *task;
    HLib::Task *score;
    shen_name machine, dealer, prize;
    std::int32_t entry, denomination, availableCoins, credits, coins, insertCoins, wager, previousWager, payout;
    std::int32_t angles[3], speeds[3];
    std::int32_t handActive, forceJackpot, celebration;
    std::uint8_t statusTimer;
    std::int8_t handDelay, state, authorState, prizeState, drumState;
    std::int8_t reelTimers[3], stops[3];
    std::uint8_t stoppedReels, bounce[3], winningLines, selection, slideLeft, slideRight;
};
struct Slt1ScoreWork {
    HLib::Task *task;
    HLib::Task *root;
    std::int32_t credits;
    std::byte reserved14[4];
};
static_assert(sizeof(Slt1EnqueueParams) == 8 && offsetof(Slt1EnqueueParams, entry) == 4);
static_assert(sizeof(Slt1Work) == 0x78 && offsetof(Slt1Work, machine) == 0x10 && offsetof(Slt1Work, denomination) == 0x20);
static_assert(offsetof(Slt1Work, angles) == 0x40 && offsetof(Slt1Work, speeds) == 0x4c && offsetof(Slt1Work, handActive) == 0x58);
static_assert(offsetof(Slt1Work, state) == 0x66 && offsetof(Slt1Work, reelTimers) == 0x6a && offsetof(Slt1Work, stops) == 0x6d);
static_assert(offsetof(Slt1Work, stoppedReels) == 0x70 && offsetof(Slt1Work, selection) == 0x75 && offsetof(Slt1Work, slideRight) == 0x77);
static_assert(sizeof(Slt1ScoreWork) == 0x18 && offsetof(Slt1ScoreWork, credits) == 0x10);
void QueueSlt1(std::uintptr_t argument);
extern const ScriptAreaDefinition slt1ScriptDefinition;
} // namespace shenmue::s2
