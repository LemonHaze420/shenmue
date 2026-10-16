#include "s2/event_qte.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


void PublishEventQteState() {
    if (eventQtePresentationSink)
        eventQtePresentationSink(eventQtePresentation);
}

void SetShowQteState(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::int32_t displayState) {
    std::uint32_t buttonType{};
    switch (displayState) {
    case 0:
        break;
    case 1:
    case 2:
        buttonType = 1;
        break;
    case 3:
        buttonType = 2;
        break;
    case 4:
        qteBattleUtil(BattleQteMode::SetWait, 10, 6, maximumFrames);
        qteBattleUtil(BattleQteMode::ResetStates, requiredStatus);
        qteBattleUtil(BattleQteMode::Start);
        return;
    default:
        return;
    }
    SetShowQteFlagForButtonType(requiredStatus, maximumFrames, buttonType);
    eventQtePresentation.requiredStatus = requiredStatus;
    eventQtePresentation.currentStatus = 0xffff;
    eventQtePresentation.maximumFrames = static_cast<std::uint16_t>(maximumFrames);
    eventQtePresentation.displayMode = static_cast<std::uint16_t>(buttonType);
    eventQtePresentation.elapsedFrames = 0.0f;
    eventQtePresentation.phase = EventQtePhase::Prompt;
    eventQtePresentation.active = true;
    PublishEventQteState();
}
} // namespace event_runtime_detail


void EVT_AT_Qte_Callback(void *taskData) {
    auto *const prompt = static_cast<EventQtePromptTaskData *>(taskData);
    if (!prompt) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
        return;
    }
    if (prompt->delayFrames > 0.0f) {
        prompt->delayFrames -= HLib::GetScaledDeltaTick();
        return;
    }

    SetShowQteState(prompt->requiredStatus, prompt->maximumFrames, prompt->displayState);
    if (prompt->autoCleanupTask)
        UnregisterEventTask(HLib::GetCurrentTask());
    if (prompt->autoCleanupTask)
        HLib::DestroyCurrentTask();
}
namespace event_runtime_detail {

void QueueEventQtePrompt(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::int32_t displayState, std::int32_t delayFrames) {
    if (delayFrames == 0) {
        SetShowQteState(requiredStatus, maximumFrames, displayState);
        return;
    }
    eventQtePresentation.requiredStatus = requiredStatus;
    eventQtePresentation.currentStatus = 0xffff;
    eventQtePresentation.maximumFrames = static_cast<std::uint16_t>(maximumFrames);
    eventQtePresentation.displayMode = displayState == 0 ? 0 : (displayState < 3 ? 1 : 2);
    eventQtePresentation.elapsedFrames = 0.0f;
    eventQtePresentation.phase = EventQtePhase::Waiting;
    eventQtePresentation.active = false;
    PublishEventQteState();

    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Qte_Callback, 6, 0x0b, sizeof(EventQtePromptTaskData), eventTaskToken);
    auto *const prompt = static_cast<EventQtePromptTaskData *>(HLib::GetTaskParameter(task));
    if (!prompt) {
        if (task)
            HLib::DestroyTask(task);
        return;
    }
    std::memset(prompt, 0, sizeof(*prompt));
    prompt->autoCleanupTask = 1;
    prompt->requiredStatus = requiredStatus;
    prompt->maximumFrames = maximumFrames;
    prompt->displayState = displayState;
    prompt->delayFrames = static_cast<float>(delayFrames);
    RegisterEventTask(task);
}

void EventQteResultCallback(void *taskData) {
    auto *const completion = static_cast<EventQteResultTaskData *>(taskData);
    if (!completion || !completion->qteData) {
        HLib::DestroyCurrentTask();
        return;
    }

    const bool succeeded = (EV_GetCurrentQteStatus() & 0x0fff) == completion->qteData->requiredQteStatus;
    if (completion->result)
        *completion->result = succeeded ? 1 : 0;
    EventQteAuthoredCallback const callback = succeeded ? completion->qteData->resultSuccess : completion->qteData->resultFailure;
    if (callback)
        callback(completion->resultParameters, 0);
    HLib::DestroyCurrentTask();
}
} // namespace event_runtime_detail
void SetEventQtePresentationSink(EventQtePresentationSink sink) noexcept {
    eventQtePresentationSink = sink;
    PublishEventQteState();
}

const EventQtePresentationState &GetEventQtePresentationState() noexcept {
    return eventQtePresentation;
}

void EVT_SetQteButtonState(std::uint16_t mask) noexcept {
    SetQteButtonState(mask);
}

bool IsEventQteInputActive() noexcept {
    return GetIsQuickIconShowing();
}

void EVT_SetQteFailureCount(std::uint32_t failures) noexcept {
    eventQteFailureCount = failures;
}

void EVT_SetQteRetryFrames(std::uint16_t frames) noexcept {
    g_eventQteRetryFrames = std::bit_cast<std::int16_t>(frames);
}

std::uint32_t EVT_GetQteFailureCount() noexcept {
    return eventQteFailureCount;
}

std::int32_t EV_GetCurrentQteStatus() noexcept {
    const std::uint16_t status = static_cast<std::uint16_t>(GetCurrentQteStatus());
    eventQtePresentation.currentStatus = status;
    if (status != 0xffffU) {
        eventQtePresentation.active = false;
        if (status == 0xa000U)
            eventQtePresentation.phase = EventQtePhase::TimedOut;
        else if ((status & 0x2000U) != 0)
            eventQtePresentation.phase = EventQtePhase::Correct;
        else
            eventQtePresentation.phase = EventQtePhase::Wrong;
    }
    PublishEventQteState();
    eventQtePresentation.currentStatus = 0xffffU;
    return status;
}

void EVT_AT_QTE(std::uint16_t requiredQteStatus, std::int16_t maxTime, std::int32_t displayState, std::int32_t delayFrames) {
    ReInitElement(UIElement::GameQte);
    std::uint16_t adjustedTime = std::bit_cast<std::uint16_t>(maxTime);
    if (g_eventQteRetryFrames > 0) {
        const std::int32_t retryDelta = (static_cast<std::int32_t>(g_eventQteRetryFrames) - maxTime) / 2;
        const std::int16_t failures = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(eventQteFailureCount));
        adjustedTime += static_cast<std::uint16_t>(failures * retryDelta);
    }
    QueueEventQtePrompt(requiredQteStatus, std::bit_cast<std::int16_t>(adjustedTime), displayState, delayFrames);
}

std::uint32_t Queue_EVT_AT_ResultQTE(std::int32_t *result, void *resultParameters, const EventQteData *qteData) {
    if (result)
        *result = -1;
    if (qteData->beginSuccess) {
        qteData->beginSuccess(resultParameters, static_cast<std::uint32_t>(static_cast<std::int32_t>(qteData->callbackTimeA)));
    }
    if (qteData->beginFailure) {
        qteData->beginFailure(resultParameters,
            static_cast<std::uint32_t>(static_cast<std::int32_t>(qteData->callbackTimeA) + static_cast<std::int32_t>(qteData->callbackTimeB) +
                static_cast<std::int32_t>(qteData->callbackTimeC)));
    }

    g_eventQteRetryFrames = std::bit_cast<std::int16_t>(qteData->retryReferenceFrames);
    EVT_AT_QTE(qteData->requiredQteStatus, static_cast<std::int16_t>(qteData->maxTime), 3,
        static_cast<std::int32_t>(qteData->callbackTimeA) + static_cast<std::int32_t>(qteData->callbackTimeB));

    const std::int32_t completionFrames =
        std::max(static_cast<std::int32_t>(qteData->callbackTimeC), static_cast<std::int32_t>(qteData->retryReferenceFrames)) +
        static_cast<std::int32_t>(qteData->callbackTimeA) + static_cast<std::int32_t>(qteData->callbackTimeB) + 5;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventQteResultCallback, 4, 0x0b, sizeof(EventQteResultTaskData), eventTaskToken);
    auto *const completion = static_cast<EventQteResultTaskData *>(HLib::GetTaskParameter(task));
    if (completion) {
        completion->result = result;
        completion->resultParameters = resultParameters;
        completion->qteData = qteData;
        HLib::SetupTimedTask(task, completionFrames);
    } else if (task) {
        HLib::DestroyTask(task);
    }
    return static_cast<std::uint32_t>(completionFrames);
}

} // namespace shenmue::s2
