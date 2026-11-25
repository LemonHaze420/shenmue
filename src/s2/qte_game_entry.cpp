#include "s2/events/event_api.h"
#include "qte_game_entry.hpp"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/save.hpp"
namespace shenmue::s2 {
#endif

// @verified - p
void EVT_QteGameEnter(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    HLibTask *task;
    QteGameEntryParameter *parameter;
    task = FTBL.HLib_EnqueueTaskWithParameter(EVT_QteGameEnter_Callback, 5, 0x0b, sizeof(QteGameEntryParameter), QTE_GAME_ENTRY_TASK);
    parameter = FTBL.HLib_GetTaskParameter(task);
#else
    HLib::Task *const task = FTBL.HLib_EnqueueTaskWithParameter(EVT_QteGameEnter_Callback, 5, 0x0b, sizeof(QteGameEntryParameter), QTE_GAME_ENTRY_TASK);
    QteGameEntryParameter *const parameter = static_cast<QteGameEntryParameter *>(FTBL.HLib_GetTaskParameter(task));
#endif
    parameter->self = task;
    InitializeQteTitleDisplay(parameter);
    InitializeQteMachineDisplay(parameter);
}



// @verified - p
void InitializeQteTitleDisplay(QteGameEntryParameter *parameter) {
#if defined(SHENMUE_EVENT_OVERLAY)
#include "qte_title_data.inl"
    int index;
#endif
    FTBL.EV_EnsureCharacterDraw(QTE_GAME_TITLE);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (index = 0; QTE_TITLE_ACTORS[index]; ++index) {
#else
    for (std::uint32_t index = 0; index < 6; ++index) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.AT_SetLocalBaseChara(QTE_TITLE_ACTORS[index], QTE_GAME_TITLE);
        FTBL.EV_AT_SetPos(QTE_TITLE_ACTORS[index], 0x38000000, &QTE_TITLE_ACTOR_POSITIONS[index]);
        FTBL.EV_AT_SetAngY_0(QTE_TITLE_ACTORS[index], 0x38000000, &QTE_TITLE_ACTOR_ROTATIONS[index / 3], 0);
        if (index >= 3)
            FTBL.EV_DisableCharacterDisplay(QTE_TITLE_ACTORS[index]);
#else
        const shen_name actor = QTE_TITLE_ACTORS[index];
        AT_SetLocalBaseChara(actor, QTE_GAME_TITLE);
        EV_AT_SetPos(actor, EventTransformFlags::AllAxes, QTE_TITLE_ACTOR_POSITIONS[index]);
        EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, QTE_TITLE_ACTOR_ROTATIONS[index / 3], nullptr);
        if (index >= 3)
            EV_DisableCharacterDisplay(actor);
#endif
    }
    parameter->titleScore0 = (unsigned int)FTBL.GetSavedMinigameScore(5);
    if (!parameter->titleScore0)
        parameter->titleScore0 = 30000;
    parameter->titleScore1 = (unsigned int)FTBL.GetSavedMinigameScore(6);
    parameter->titleScore2 = (unsigned int)FTBL.GetSavedMinigameScore(7);
    FTBL.scenCopyCharaToGame(QTE_GAME_TITLE);
    FTBL.SetPosRotForGame(QTE_GAME_TITLE, 256, &QTE_TITLE_DIGIT_STEP, 99999999, 0);
    FTBL.UpdateGameNodeFlags(QTE_GAME_TITLE, parameter->titleScore2, &QTE_TITLE_SCORE_POSITIONS[0], 1.0f, 0);
    FTBL.UpdateGameNodeFlags(QTE_GAME_TITLE, parameter->titleScore0, &QTE_TITLE_SCORE_POSITIONS[1], 1.0f, 0);
    FTBL.UpdateGameNodeFlags(QTE_GAME_TITLE, parameter->titleScore1, &QTE_TITLE_SCORE_POSITIONS[2], 1.0f, 0);
}

// @verified - p
void InitializeQteMachineDisplay(QteGameEntryParameter *parameter) {
#if defined(SHENMUE_EVENT_OVERLAY)
#include "qte_machine_data.inl"
    Vector3 scale;
    short index;
#endif
    FTBL.EV_EnsureCharacterDraw(QTE_GAME_MACHINE);
#if defined(SHENMUE_EVENT_OVERLAY)
    for (index = 0; QTE_MACHINE_PARTS[index]; ++index) {
        scale.x = scale.y = QTE_MACHINE_SCALES[index];
        scale.z = 1.0f;
        FTBL.scenApplyPosOrRotToChildId(QTE_GAME_MACHINE, QTE_MACHINE_PARTS[index], &QTE_MACHINE_POSITIONS[index], 0, 0);
#else
    for (std::uint32_t index = 0; index < 16; ++index) {
        Vector3 position = QTE_MACHINE_POSITIONS[index];
        Vector3 scale{QTE_MACHINE_SCALES[index], QTE_MACHINE_SCALES[index], 1.0f};
        EV_scenApplyPosOrRotToPartOf(QTE_GAME_MACHINE, QTE_MACHINE_PARTS[index], &position, nullptr, EventCharacterPartTransformMode::Absolute);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.scenApplyPosOrRotToChildId(QTE_GAME_MACHINE, QTE_MACHINE_PARTS[index], &scale, 0, 3);
#else
        EV_scenApplyPosOrRotToPartOf(QTE_GAME_MACHINE, QTE_MACHINE_PARTS[index], &scale, nullptr, EventCharacterPartTransformMode::Set);
#endif
    }
    parameter->state = 0;
    parameter->exciteBest = (unsigned int)FTBL.GetSavedMinigameScore(0);
    if (!parameter->exciteBest)
        parameter->exciteBest = 10000;
    parameter->exciteContinued = (unsigned int)FTBL.GetSavedMinigameScore(1);
}

// @verified - p
void EVT_QteGameEnter_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCurrentControlInput(7);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace shenmue::s2
#endif
