#include "s2/areas/area_dart.hpp"
#include "s2/darts.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"

namespace shenmue::s2::areas {

void DartsUpdateMachineScore(shen_name board, std::int32_t score, std::int32_t row) {
    Vector3 position{0.02239999920129776f, DART_GAME_LARGE_SCORE_Y[row], -0.03999999910593033f};
    const Vector3 scale{0.5f, 0.5f, 0.5f};
    const std::uint32_t firstDigit = static_cast<std::uint32_t>(score / 10000);
    std::int32_t remaining = score % 10000;
    SetPosRotForGame(board, 256, &DART_LARGE_DIGIT_STEP, 9999, 0);
    UpdateGameNodeFlags(board, firstDigit, &position, 1, nullptr);
    position = {DART_SMALL_DIGIT_STEP.x * 4 + 0.14740000665187836f, DART_GAME_SMALL_SCORE_Y[row], -0.03999999910593033f};
    scenGetCharaGameNode(board, 336, &position, 1, &scale);
    position = {0.14740000665187836f, DART_GAME_SMALL_SCORE_Y[row], -0.03999999910593033f};
    SetPosRotForGame(board, 256, &DART_SMALL_DIGIT_STEP, 9, 0);
    for (std::int32_t digit = 0; digit < 4; ++digit) {
        UpdateGameNodeFlags(board, static_cast<std::uint32_t>(remaining % 10), &position, 1, &scale);
        remaining /= 10;
        position.x += DART_SMALL_DIGIT_STEP.x;
        position.y += DART_SMALL_DIGIT_STEP.y;
        position.z += DART_SMALL_DIGIT_STEP.z;
    }
}


void DartsUpdateCompactMachineScore(shen_name board, std::uint32_t score, std::int32_t row) {
    const Vector3 step{DART_COMPACT_DIGIT_STEP[row], 0, 0};
    SetPosRotForGame(board, 256, &step, 9999, 0);
    UpdateGameNodeFlags(board, score, &DART_COMPACT_SCORE_POSITION[row], 1, &DART_COMPACT_SCORE_SCALE[row]);
}


void DartsDisplayCallback(void *taskData) {
    DartsEventWork *const work = static_cast<DartsChildWork *>(taskData)->root;
    const std::uint8_t flags = work->field0e;
    if (!flags)
        return;
    scenCopyCharaToGame(DartsSelectBoard(work, {}));
    if (DartsSelectBoard(work, {}).name == DART_COMPACT.name) {
        if (flags & 2)
            DartsUpdateCompactMachineScore(DartsSelectBoard(work, {}), work->bestScore < 50 ? 50 : work->bestScore, 0);
        if (flags & 4)
            DartsUpdateCompactMachineScore(DartsSelectBoard(work, {}), work->previousScore, 1);
        if (flags & 1)
            DartsUpdateCompactMachineScore(DartsSelectBoard(work, {}), work->opponentScore.accumulated, 3);
    } else {
        if (flags & 2)
            DartsUpdateMachineScore(DartsSelectBoard(work, {}), work->bestScore < 777777 ? 777777 : work->bestScore, 0);
        if (flags & 4)
            DartsUpdateMachineScore(DartsSelectBoard(work, {}), work->previousScore, 1);
        if (flags & 8) {
            const std::int32_t score = work->scoreTicker;
            std::int32_t remainder = score % 10000;
            if (work->field0e & 0x40)
                remainder = static_cast<std::int32_t>(shenGetRandomInt32(10000));
            DartsUpdateMachineScore(DartsSelectBoard(work, {}), 10000 * (score / 10000) + remainder, 2);
        }
        if (flags & 1) {
            const std::int32_t score = work->opponentScore.accumulated;
            std::int32_t remainder = score % 10000;
            if (work->field0e & 0x20)
                remainder = static_cast<std::int32_t>(shenGetRandomInt32(10000));
            DartsUpdateMachineScore(DartsSelectBoard(work, {}), 10000 * (score / 10000) + remainder, 3);
        }
        if (flags & 0x10)
            DartsUpdateMachineScore(DartsSelectBoard(work, {}), work->opponentScore.score, 4);
    }
}


void DartsIncrementFramesCallback(void *taskData) {
    ++static_cast<DartsChildWork *>(taskData)->root->frames;
}
} // namespace shenmue::s2::areas
