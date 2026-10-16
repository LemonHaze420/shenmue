#include "s2/event_gameplay_queued.hpp"
#include "s2/character.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/ui.hpp"

#include <array>
#include <bit>

namespace shenmue::s2 {

// @verified - p
void EVT_AT_Cling(void *taskData) {
    EventCling *const parameter = static_cast<EventCling *>(taskData);
    if (parameter->delay > 0.0f) {
        parameter->delay -= HLib::GetScaledDeltaTick();
        return;
    }
    if (!parameter->mode) {
        Character *const target = FindCharacterByScenarioName(parameter->target);
        Character *const character = FindCharacterByScenarioName(parameter->character);
        const std::array<std::int32_t, 3> rotation{parameter->rotation.x, parameter->rotation.y, parameter->rotation.z};
        SetFixoDataForMotionWork(target, character, parameter->bone, parameter->position, rotation);
    } else if (parameter->mode == 1) {
        ResetCharaFixo(FindCharacterByScenarioName(parameter->target));
    }
    if (parameter->cleanup) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
void EVT_AT_StartMoneyCount_Callback(void *taskData) {
    EventMoneyCount *const parameter = static_cast<EventMoneyCount *>(taskData);
    if (parameter->delay > 0.0f) {
        parameter->delay -= HLib::GetScaledDeltaTick();
        return;
    }
    AddThisToCashOrSegaCoins(std::bit_cast<std::int32_t>(parameter->cash), ECashOrCoins::Cash2);
    if (parameter->cleanup) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
std::int64_t PATI_StartMoneyCount(std::uint32_t cash, std::int32_t delay) {
    if (!delay)
        return AddThisToCashOrSegaCoins(std::bit_cast<std::int32_t>(cash), ECashOrCoins::Cash2);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), GAMEPLAY_QUEUED_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const parameter = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    parameter->cleanup = 1;
    parameter->cash = cash;
    parameter->delay = static_cast<float>(delay);
    return reinterpret_cast<std::intptr_t>(parameter);
}

// @verified - p
void EVT_AT_SetFaceLooks_Callback(void *taskData) {
    EventFaceLooks &parameter = *static_cast<EventFaceLooks *>(taskData);
    if (parameter.delay > 0.0f) {
        parameter.delay -= HLib::GetScaledDeltaTick();
        return;
    }
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(parameter.character), static_cast<std::int32_t>(parameter.expression),
        static_cast<float>(parameter.duration), parameter.intensity);
    if (parameter.cleanup) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void EVT_AT_SetFaceLooks(shen_name character, std::uint32_t expression, std::int32_t duration, float intensity, std::int32_t delay) {
    if (delay == 0) {
        SetHighModelAndFaceFlagsForChara(
            FindCharacterByScenarioName(character), static_cast<std::int32_t>(expression), static_cast<float>(duration), intensity);
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetFaceLooks_Callback, 6, 0x0b, sizeof(EventFaceLooks), GAMEPLAY_QUEUED_TASK);
    RegisterEventTask(task);
    EventFaceLooks *const parameter = static_cast<EventFaceLooks *>(HLib::GetTaskParameter(task));
    parameter->cleanup = 1;
    parameter->character = character;
    parameter->expression = expression;
    parameter->duration = duration;
    parameter->intensity = intensity;
    parameter->delay = static_cast<float>(delay);
}

} // namespace shenmue::s2
