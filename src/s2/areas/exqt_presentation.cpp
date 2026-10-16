#include "s2/areas/area_exqt.hpp"
#include "s2/areas/area_dgct.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/qte_game_entry.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr Vector3 digitStep{-.015f, 0, 0}, digitScale{1.414f, 1.414f, 1.414f}, bonusScale{.75f, .75f, .75f};
void DrawNumber(std::int32_t score, const Vector3 &position, std::int32_t maximum = 99999999) {
    SetPosRotForGame(EXQT_MACHINE, 256, &digitStep, maximum, 0);
    UpdateGameNodeFlags(EXQT_MACHINE, score, &position, 1, &digitScale);
}
} // namespace
void InitializeExciteMachineParts() {
    for (std::uint16_t i = 0; i < 13; ++i) {
        Vector3 position = QTE_MACHINE_POSITIONS[i];
        Vector3 scale{QTE_MACHINE_SCALES[i], QTE_MACHINE_SCALES[i], 1};
        EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, QTE_MACHINE_PARTS[i], &position, nullptr, EventCharacterPartTransformMode::Absolute);
        EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, QTE_MACHINE_PARTS[i], &scale, nullptr, EventCharacterPartTransformMode::Set);
    }
}
void SetExciteMachinePart(std::uint16_t part, std::int32_t enabled) {
    Vector3 position;
    EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, part, &position, nullptr, EventCharacterPartTransformMode::Get);
    position.z = enabled == 1 ? .015f : 0;
    EV_scenApplyPosOrRotToPartOf(EXQT_MACHINE, part, &position, nullptr, EventCharacterPartTransformMode::Absolute);
}
void HideExciteMachineParts() {
    for (std::uint16_t i = 162; i < 177; ++i)
        SetExciteMachinePart(i, 0);
    for (std::uint16_t i = 182; i < 188; ++i)
        SetExciteMachinePart(i, 0);
}
void ResetExciteMachineDisplay() {
    HideExciteMachineParts();
    for (std::uint16_t i = 162; i < 166; ++i)
        SetExciteMachinePart(i, 1);
}
void UpdateExciteLives(const ExciteState &state) {
    SetExciteMachinePart(175, 0);
    SetExciteMachinePart(174, 0);
    SetExciteMachinePart(167, 0);
    if (state.lives == 3)
        SetExciteMachinePart(175, 1);
    if (state.lives >= 2)
        SetExciteMachinePart(174, 1);
    if (state.lives)
        SetExciteMachinePart(167, 1);
}
void ExcitePartCallback(void *data) {
    const ExcitePartWork &work = *static_cast<ExcitePartWork *>(data);
    SetExciteMachinePart(static_cast<std::uint16_t>(work.part), work.enabled);
    HLib::DestroyCurrentTask();
}
void QueueExcitePart(std::uint32_t part, std::int32_t enabled, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ExcitePartCallback, 6, 11, sizeof(ExcitePartWork), EXQT_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    ExcitePartWork &work = *static_cast<ExcitePartWork *>(HLib::GetTaskParameter(task));
    work.part = part;
    work.enabled = enabled;
}
void QueueExciteStageDisplay(const ExciteState &state) {
    const std::uint32_t part = state.patternStage == 1 ? 172 : state.patternStage == 2 ? 173 : state.patternStage == 9 ? 182 : 171;
    for (std::int32_t i = 0; i < 8; ++i)
        QueueExcitePart(part, (i & 1) == 0, 14 * i);
}
void QueueExciteResultDisplay(const ExciteState &state) {
    const std::uint32_t part = state.game.score < state.game.best ? 169 : 170;
    for (std::int32_t i = 0; i < 7; ++i) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(ExcitePartCallback, 6, 11, sizeof(ExcitePartWork), EXQT_TASK);
        if (i)
            HLib::SetupTimedTask(task, 14 * i);
        ExcitePartWork &work = *static_cast<ExcitePartWork *>(HLib::GetTaskParameter(task));
        work.part = part;
        // The three off callbacks deliberately retain the allocator's second DWORD.
        if (!(i & 1))
            work.enabled = 1;
    }
}
void ExciteScoreCallback(void *data) {
    ExciteScoreWork &work = *static_cast<ExciteScoreWork *>(data);
    const std::int32_t ticks = work.ticks--;
    if (ticks <= 0)
        HLib::DestroyCurrentTask();
    else
        DrawNumber(work.score, work.position);
}
void QueueExciteScoreFlash(const ExciteState &state, bool initial) {
    const bool best = initial || state.game.score >= state.game.best;
    const Vector3 position = best ? Vector3{.055f, 1.58f, .015f} : Vector3{-.032000002f, 1.528f, .015f};
    for (std::int32_t i = 0; i < 4; ++i) {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(ExciteScoreCallback, 6, 11, sizeof(ExciteScoreWork), EXQT_TASK);
        if (i || !initial)
            HLib::SetupTimedTask(task, i ? 28 * i : 1);
        ExciteScoreWork &work = *static_cast<ExciteScoreWork *>(HLib::GetTaskParameter(task));
        work.ticks = 14;
        work.position = position;
        work.score = best ? state.game.best : state.game.score;
    }
}
void DrawExciteScore(const ExciteState &state, std::uint8_t selector) {
    std::int32_t score = 0;
    Vector3 position;
    switch (selector) {
    case 0:
        score = state.game.score >= state.game.best ? state.game.score : state.game.best;
        position = {.055f, 1.58f, .015f};
        break;
    case 1:
        score = state.game.score;
        position = {-.032000002f, 1.528f, .015f};
        break;
    case 2:
        score = state.game.continued;
        position = {.13f, 1.528f, .015f};
        break;
    default:
        break;
    }
    DrawNumber(score, position);
}
void DrawExciteWindow(const ExciteState &state) {
    std::uint16_t part = 177;
    switch (state.patternStage) {
    case 2:
    case 5:
    case 8:
        part = 180;
        break;
    case 9:
        part = 179;
        break;
    case 10:
    case 11:
        part = 178;
        break;
    default:
        break;
    }
    const ExciteInput &input = state.controls.excite.input;
    Vector3 position{-.13f, 1.38f, .015f};
    if (input.window) {
        std::int16_t count = static_cast<std::int16_t>(30 * (input.window - input.elapsed) / input.window);
        while (count > 0) {
            if (count > 30)
                break;
            scenGetCharaGameNode(EXQT_MACHINE, part, &position, 1, nullptr);
            --count;
            position.x += .0070000002f;
        }
    }
}
void DrawExciteBonus(const ExciteState &state) {
    const ExciteInput &input = state.controls.excite.input;
    const float divisor = input.window ? static_cast<float>(static_cast<std::int32_t>(input.window)) : 1.0f;
    const float x = (((static_cast<std::int32_t>(input.window - input.elapsed) * 30.0f) / divisor) * .0070000002f + .02f) - .13f;
    Vector3 position{x, 1.38f, .015f};
    if (static_cast<std::int32_t>(state.game.timingBonus))
        scenGetCharaGameNode(EXQT_MACHINE, 181, &position, 1, &bonusScale);
    position.x += state.game.timingBonus == 100 ? .055f : state.game.timingBonus < 10 ? .025f : .039999999f;
    DrawNumber(static_cast<std::int32_t>(state.game.timingBonus), position, 100);
}
void UpdateExciteDisplay(const ExciteState &state) {
    scenCopyCharaToGame(EXQT_MACHINE);
    switch (state.game.state) {
    case 6:
        QueueExciteScoreFlash(state, true);
        break;
    case 8:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        break;
    case 9:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        if (state.game.waitTarget != 60 || state.game.timingBonus == 0) {
            DrawExciteWindow(state);
            DrawExciteBonus(state);
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        DrawExciteWindow(state);
        break;
    case 16:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        break;
    case 23:
        QueueExciteScoreFlash(state, false);
        break;
    default:
        break;
    }
}
void DrawGameCenterWindow(const ExciteState &state) {
    std::uint16_t part = 177;
    switch (state.patternStage) {
    case 1:
    case 4:
    case 7:
        part = 180;
        break;
    case 2:
    case 8:
        part = 179;
        break;
    case 5:
    case 9:
        part = 178;
        break;
    default:
        break;
    }
    const ExciteInput &input = state.controls.arcade.input;
    std::int16_t count = static_cast<std::int16_t>(30 * (input.window - input.elapsed) / input.window);
    Vector3 position{-.13f, 1.38f, .015f};
    while (count > 0) {
        if (count > 30)
            break;
        scenGetCharaGameNode(EXQT_MACHINE, part, &position, 1, nullptr);
        --count;
        position.x += .0070000002f;
    }
}
void DrawGameCenterBonus(const ExciteState &state) {
    const ExciteInput &input = state.controls.arcade.input;
    const float x =
        (((static_cast<std::int32_t>(input.window - input.elapsed) * 30.0f) / static_cast<std::int32_t>(input.window)) * .0070000002f + .02f) - .13f;
    Vector3 position{x, 1.38f, .015f};
    if (static_cast<std::int32_t>(state.game.timingBonus))
        scenGetCharaGameNode(EXQT_MACHINE, 181, &position, 1, &bonusScale);
    position.x += state.game.timingBonus == 100 ? .055f : state.game.timingBonus < 10 ? .025f : .039999999f;
    DrawNumber(static_cast<std::int32_t>(state.game.timingBonus), position, 100);
}
void UpdateGameCenterDisplay(const ExciteState &state) {
    scenCopyCharaToGame(EXQT_MACHINE);
    switch (state.game.state) {
    case 6:
        QueueExciteScoreFlash(state, true);
        break;
    case 8:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        if (state.game.waitTarget != 60 || state.game.timingBonus == 0) {
            DrawGameCenterWindow(state);
            DrawGameCenterBonus(state);
        }
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        DrawGameCenterWindow(state);
        break;
    case 15:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        break;
    case 21:
        QueueExciteScoreFlash(state, false);
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
