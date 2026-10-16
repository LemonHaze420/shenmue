#include "s2/areas/area_dart.hpp"
#include "s2/character.hpp"
#include "s2/model_component.hpp"

namespace shenmue::s2::areas {
shen_name dartsTemporaryCharacter{};


HLib::Task *QueueDartsGameArrow(shen_name character, shen_name arrow) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DartsGameArrowCallback, 4, 0x0b, sizeof(DartsArrowWork), DART_TASK);
    DartsArrowWork *const work = static_cast<DartsArrowWork *>(HLib::GetTaskParameter(task));
    if (!work)
        return nullptr;
    work->character = character;
    work->arrow = arrow;
    dartsTemporaryCharacter = {};
    return task;
}



void DartsGameArrowCallback(void *taskData) {
    DartsArrowWork *const work = static_cast<DartsArrowWork *>(taskData);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->character.name == dartsTemporaryCharacter.name) {
        const shen_name arrow = work->arrow;
        HLib::DestroyCurrentTask();
        ResetCharaFixo(FindCharacterByScenarioName(arrow));
        return;
    }
    if (!DoesScenarioCharacterExist(work->character) || !DoesScenarioCharacterExist(work->arrow)) {
        const shen_name arrow = work->arrow;
        EV_DisableCharacterDisplay(arrow);
        EV_ToggleCharacterFlag(arrow);
        HLib::DestroyCurrentTask();
        ResetCharaFixo(FindCharacterByScenarioName(arrow));
        return;
    }
    EV_EnableCharacterDisplay(work->arrow);
    EV_EnsureCharacterDraw(work->arrow);
    Character *const target = FindCharacterByScenarioName(work->arrow);
    Character *const source = FindCharacterByScenarioName(work->character);
    if (work->character.name == DART_JON.name) {
        SetFixoDataForMotionWork(target, source, 21, DART_JON_ARROW_OFFSET, DART_ARROW_ROTATION);
    } else if (work->character.name == DART_FCJ.name || work->character.name == DART_HLN.name) {
        SetFixoDataForMotionWork(target, source, 14, DART_FCJ_HLN_ARROW_OFFSET, DART_ARROW_ROTATION);
    } else {
        SetFixoDataForMotionWork(target, source, 14, DART_DEFAULT_ARROW_OFFSET, DART_ARROW_ROTATION);
    }
}
} // namespace shenmue::s2::areas
