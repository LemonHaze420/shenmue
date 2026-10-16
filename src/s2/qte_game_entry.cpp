#include "s2/qte_game_entry.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/save.hpp"

namespace shenmue::s2 {

// @verified - p
void InitializeQteTitleDisplay(QteGameEntryParameter *parameter) {
    EV_EnsureCharacterDraw(QTE_GAME_TITLE);
    for (std::uint32_t index = 0; index < 6; ++index) {
        const shen_name actor = QTE_TITLE_ACTORS[index];
        AT_SetLocalBaseChara(actor, QTE_GAME_TITLE);
        EV_AT_SetPos(actor, EventTransformFlags::AllAxes, QTE_TITLE_ACTOR_POSITIONS[index]);
        EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, QTE_TITLE_ACTOR_ROTATIONS[index / 3], nullptr);
        if (index >= 3)
            EV_DisableCharacterDisplay(actor);
    }
    parameter->titleScore0 = static_cast<std::uint32_t>(GetSavedMinigameScore(5));
    if (!parameter->titleScore0)
        parameter->titleScore0 = 30000;
    parameter->titleScore1 = static_cast<std::uint32_t>(GetSavedMinigameScore(6));
    parameter->titleScore2 = static_cast<std::uint32_t>(GetSavedMinigameScore(7));
    scenCopyCharaToGame(QTE_GAME_TITLE);
    SetPosRotForGame(QTE_GAME_TITLE, 256, &QTE_TITLE_DIGIT_STEP, 99999999, 0);
    UpdateGameNodeFlags(QTE_GAME_TITLE, parameter->titleScore2, &QTE_TITLE_SCORE_POSITIONS[0], 1.0f, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, parameter->titleScore0, &QTE_TITLE_SCORE_POSITIONS[1], 1.0f, nullptr);
    UpdateGameNodeFlags(QTE_GAME_TITLE, parameter->titleScore1, &QTE_TITLE_SCORE_POSITIONS[2], 1.0f, nullptr);
}

// @verified - p
void InitializeQteMachineDisplay(QteGameEntryParameter *parameter) {
    EV_EnsureCharacterDraw(QTE_GAME_MACHINE);
    for (std::uint32_t index = 0; index < 16; ++index) {
        Vector3 position = QTE_MACHINE_POSITIONS[index];
        Vector3 scale{QTE_MACHINE_SCALES[index], QTE_MACHINE_SCALES[index], 1.0f};
        EV_scenApplyPosOrRotToPartOf(QTE_GAME_MACHINE, QTE_MACHINE_PARTS[index], &position, nullptr, EventCharacterPartTransformMode::Absolute);
        EV_scenApplyPosOrRotToPartOf(QTE_GAME_MACHINE, QTE_MACHINE_PARTS[index], &scale, nullptr, EventCharacterPartTransformMode::Set);
    }
    parameter->state = 0;
    parameter->exciteBest = static_cast<std::uint32_t>(GetSavedMinigameScore(0));
    if (!parameter->exciteBest)
        parameter->exciteBest = 10000;
    parameter->exciteContinued = static_cast<std::uint32_t>(GetSavedMinigameScore(1));
}


// @verified - p
void EVT_QteGameEnter_Callback(void *) {
}

// @verified - p
void EVT_QteGameEnter() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_QteGameEnter_Callback, 5, 0x0b, sizeof(QteGameEntryParameter), QTE_GAME_ENTRY_TASK);
    QteGameEntryParameter *const parameter = static_cast<QteGameEntryParameter *>(HLib::GetTaskParameter(task));
    parameter->self = task;
    InitializeQteTitleDisplay(parameter);
    InitializeQteMachineDisplay(parameter);
}

} // namespace shenmue::s2
