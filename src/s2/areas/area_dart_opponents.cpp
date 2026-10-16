#include "s2/areas/area_dart.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/motion_work.hpp"

namespace shenmue::s2::areas {
// A failed five-name search reads the adjacent unnamed row.
std::int32_t DartsOpponentAttachBone(shen_name opponent, std::int32_t difficulty) {
    if (difficulty >= 2 && opponent.name == DART_JON.name)
        return 14;
    std::size_t index = 0;
    while (index < 5 && DART_OPPONENT_WORDS[index][0] != opponent.name)
        ++index;
    return static_cast<std::int32_t>(DART_OPPONENT_WORDS[index][2]);
}

// The fifth motion argument carries raw DWORD flags, not a float value.
std::uint32_t DartsBeginOpponentThrow(shen_name opponent, std::int32_t difficulty) {
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(opponent)), 13, 0);
    std::size_t index = 0;
    while (index < 5 && DART_OPPONENT_WORDS[index][0] != opponent.name)
        ++index;
    const std::uint32_t duration = DART_OPPONENT_WORDS[index][5];
    const std::uint32_t flags = difficulty >= 2 && opponent.name == DART_JON.name ? 0x20000000U : 0;
    EVT_AT_SetMot(opponent, static_cast<std::uint16_t>(DART_OPPONENT_WORDS[index][3]), -1.0f, 1.0f, flags, 0);
    return duration;
}

// A nonzero actor other than RYO never selects the secondary board.
shen_name DartsSelectBoard(DartsEventWork *work, shen_name actor) {
    const EventDartsPlacement &placement = work->entry->placements[work->entry->placementIndex];
    if (work->entry->opponentMode == 2) {
        if ((actor.name == DART_PLAYER.name || (!actor.name && !work->enemyTurn)) && placement.parent.name)
            return placement.parent;
    }
    return placement.character;
}
} // namespace shenmue::s2::areas
