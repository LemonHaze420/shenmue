#include "s2/areas/area_dart.hpp"
#include "s2/character.hpp"
#include "s2/motion_work.hpp"
#include "s2/save.hpp"
#include <cstring>

namespace shenmue::s2::areas {

void DartsInitializeScoreBanks(DartsEventWork *work) {
    std::memset(&work->bestScore, 0, 0x6c);
    work->alternateBestScore = work->entry->savedScores[0];
    work->alternatePreviousScore = work->entry->savedScores[1];
    if (!work->standardBoard) {
        work->fieldE8 = static_cast<std::int32_t>(GetSavedMinigameScore(static_cast<std::uint32_t>(SavedScoreType::Darts_BestScore1)));
        work->fieldEc = static_cast<std::int32_t>(GetSavedMinigameScore(static_cast<std::uint32_t>(SavedScoreType::Darts_BestScore2)));
    } else {
        const std::int32_t best = static_cast<std::int32_t>(GetSavedMinigameScore(static_cast<std::uint32_t>(SavedScoreType::Darts_BestScore4)));
        work->fieldE8 = best;
        work->bestScore = best;
        const std::int32_t previous = static_cast<std::int32_t>(GetSavedMinigameScore(static_cast<std::uint32_t>(SavedScoreType::Darts_BestScore3)));
        work->previousScore = previous;
        work->fieldEc = previous;
    }
    work->scoreUiFlags = 0;
    work->field0e = 15;
}


void DartsPlaceCameraDummies(DartsEventWork *work) {
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    const Vector3 position0{0, 0, 4.199999809265137f};
    const Vector3i rotation0{};
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(DART_DUMMY0)), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_DUMMY0, board, &position0, &rotation0, 0);
    if (work->entry->opponentMode == 2) {
        std::size_t index = 0;
        while (index < 5 && DART_OPPONENT_WORDS[index][0] != work->entry->enemy->name.name)
            ++index;
        const shen_name opponentBoard = work->entry->placements[work->entry->placementIndex].character;
        const Vector3 position1{0, 0, 4};
        const Vector3i rotation1{};
        SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(DART_DUMMY1)), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(DART_DUMMY1, opponentBoard, &position1, &rotation1, 0);
        Vector3 position;
        EV_GetCharaPosition(DART_DUMMY1, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.y += std::bit_cast<float>(DART_OPPONENT_WORDS[index][1]);
        EV_AT_SetPos(DART_DUMMY1, EventTransformFlags::AllAxes, position);
    }
}
} // namespace shenmue::s2::areas
