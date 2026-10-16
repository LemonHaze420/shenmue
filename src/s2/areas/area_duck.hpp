#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2 {
struct EventReadCharactersWork;
}
namespace shenmue::s2::areas {
enum class DuckState : std::uint16_t {
    Init,
    IntroAuth,
    EntryDialogue,
    PayEntry,
    BettingIntro,
    InsufficientFunds,
    BetMenu,
    WagerInput,
    SelectDuck,
    PaddockView,
    Decline,
    PostRaceHold,
    Countdown,
    Results,
    Running,
    Cleanup
};
struct DuckEventWork {
    std::uint64_t reserved0;
    EventReadCharactersWork *npcLoad;
    DuckState state;
    std::uint16_t phase;
    std::int32_t wager, selection, reserved1c;
};
static_assert(sizeof(DuckEventWork) == 0x20 && offsetof(DuckEventWork, state) == 0x10);

std::int64_t DuckRaceRunBettingWinSequence(const DuckEventWork *work);
std::int64_t DuckRaceRunBettingLossSequence();
std::int64_t DuckRaceRunSpecialWinSequence();
std::int64_t DuckRaceRunSpecialLossSequence();
void AreaDUCKInit();
void AreaDUCKMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaDUCKScriptDefinition;
} // namespace shenmue::s2::areas
