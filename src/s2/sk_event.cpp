#include "s2/sk_event.hpp"
#include "s2/author.hpp"
#include "s2/event_runtime.hpp"
#include "s2/ui.hpp"
#include "s2/debug.hpp"
#include "s2/load_title.hpp"
#include "s2/npc_package.hpp"
namespace shenmue::s2 {
// @verified - p
void QueueSkAuthorStart(HLib::Task *parent, std::int32_t command, HLib::TaskCallback callback, void *callbackData, std::int32_t delayFrames) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), SK_EVENT_TASK);
    StepAuthorStartWork *const work = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->parent = parent;
    work->command = command;
    work->callback = callback;
    work->callbackData = callbackData;
    if (delayFrames <= 0) {
        work->phase = 1;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(command));
        if (work->callback)
            work->callback(work->callbackData);
    } else {
        work->phase = 0;
        HLib::SetupTimedTask(task, delayFrames);
    }
    HLib::SetupTimedTask(work->parent, 0);
}
// @verified - p
void QueueSkChoiceWait(HLib::Task *parent, std::uintptr_t, std::int16_t secondChoice, std::int16_t thirdChoice, std::int16_t, std::int32_t defaultChoice,
    std::int32_t, std::int32_t delayFrames) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkChoiceWaitCallback, 6, 0x0b, sizeof(SkChoiceWaitWork), SK_EVENT_TASK);
    SkChoiceWaitWork *const work = static_cast<SkChoiceWaitWork *>(HLib::GetTaskParameter(task));
    EV_SetAuthorActiveTo(0);
    work->phase = 0;
    work->choices[0] = -1;
    work->defaultChoice = defaultChoice;
    work->self = task;
    work->parent = parent;
    work->timeout = 0x7fff;
    work->reserved1a = 0;
    work->choices[3] = -1;
    work->choices[1] = secondChoice;
    work->choices[2] = thirdChoice;
    if (delayFrames > 0)
        HLib::SetupTimedTask(task, delayFrames);
    HLib::SetupTimedTask(work->parent, 0);
}
// @verified - p
void SkChoiceWaitCallback(void *parameter) {
    SkChoiceWaitWork *const work = static_cast<SkChoiceWaitWork *>(parameter);
    if (work->phase) {
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            HLib::ResolveDeferredCallback(work->parent);
            HLib::DestroyTaskInternal(work->self);
        }
    } else {
        const std::int16_t defaultChoice = work->defaultChoice == -1 ? -1 : static_cast<std::int16_t>(work->defaultChoice);
        const std::uint16_t mode = work->defaultChoice == -1 ? 0 : 2;
        SetupControlSpeakAndUI(work->choices, work->timeout, defaultChoice, mode);
        ++work->phase;
    }
}
// @verified - p
void QueueSkQteWait(std::int32_t *resultBlock, std::uint32_t button, std::int32_t referenceFrames, std::int32_t baseMaximumFrames, StepQteCallback success,
    StepQteCallback failure, std::int32_t delayFrames) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_SK_WaitQTE_Callback, 6, 0x0b, sizeof(StepWaitQteWork), SK_EVENT_TASK);
    StepWaitQteWork *const work = static_cast<StepWaitQteWork *>(HLib::GetTaskParameter(task));
    EV_SetAuthorActiveTo(0);
    resultBlock[0] = 0;
    resultBlock[3] = 0;
    work->success = success;
    work->failure = failure;
    work->task = task;
    work->button = button;
    work->referenceFrames = referenceFrames;
    work->maximumFrames = baseMaximumFrames + 4 * scenarioQteRetryCount;
    work->elapsed = resultBlock;
    work->status = reinterpret_cast<std::uint16_t *>(resultBlock + 2);
    work->succeeded = resultBlock + 3;
    work->phase = 0;
    if (delayFrames > 4) {
        delayFrames -= 4;
        work->referenceFrames += 4;
    }
    if (work->maximumFrames > work->referenceFrames - 4)
        work->maximumFrames = work->referenceFrames - 4;
    work->parent = HLib::GetCurrentTask();
    HLib::SetupTimedTask(work->parent, 0);
    if (delayFrames > 0)
        HLib::SetupTimedTask(task, delayFrames);
}
// @verified - p
void SkEventMessageBeginCallback(void *parameter) {
    SkEventMessageWork *const work = static_cast<SkEventMessageWork *>(parameter);
    (void)dbgPrintEventMessage(work->message);
    HLib::SetCurrentTaskTo(SkEventMessageEndCallback);
}
// @verified - p
void SkEventMessageEndCallback(void *parameter) {
    SkEventMessageWork *const work = static_cast<SkEventMessageWork *>(parameter);
    if (work->frames-- == 1) {
        ctrlStopSubtitles();
        (void)HLib::DestroyCurrentTask();
    }
}
namespace {
struct SkMoneyIconWork {
    HLib::Task *self;
    HLib::Task *parent;
    std::int32_t maximum;
    std::int16_t step;
    std::uint16_t phase;
};
static_assert(sizeof(SkMoneyIconWork) == 0x18);
void SkMoneyIcon_Callback(void *data) {
    auto *work = static_cast<SkMoneyIconWork *>(data);
    if (work->phase) {
        if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage)) {
            HLib::ResolveDeferredCallback(work->parent);
            HLib::DestroyTaskInternal(work->self);
        }
    } else {
        SetMoneyRelatedUIElement(static_cast<std::int16_t>(work->maximum), 32767, work->step);
        ++work->phase;
    }
}
}
void QueueSkMoneyIconWait(HLib::Task *parent, std::int16_t maximum, std::int16_t step) {
    auto *task = HLib::EnqueueTaskWithParameter(SkMoneyIcon_Callback, 6, 0x0b, sizeof(SkMoneyIconWork), SK_EVENT_TASK);
    auto *work = static_cast<SkMoneyIconWork *>(HLib::GetTaskParameter(task));
    *work = {task, parent, maximum, step, 0};
    HLib::SetupTimedTask(parent, 0);
}
namespace {
struct SkNpcWaitWork {
    HLib::Task *self;
    HLib::Task *parent;
    std::uint64_t reserved10;
};
static_assert(sizeof(SkNpcWaitWork) == 0x18);
void SkNpcWait_Callback(void *data) {
    auto *work = static_cast<SkNpcWaitWork *>(data);
    if (IsLoadingDrawActive())
        DrawLoadingModes();
    EV_debug_log("common\\et_skevent.c", 347, ".");
    if (!NpcWait()) {
        EV_debug_log("common\\et_skevent.c", 349, "\n");
        HLib::ResolveDeferredCallback(work->parent);
        HLib::DestroyTaskInternal(work->self);
    }
}
}
void QueueSkNpcWait(HLib::Task *parent) {
    auto *task = HLib::EnqueueTaskWithParameter(SkNpcWait_Callback, 6, 0x0b, sizeof(SkNpcWaitWork), SK_EVENT_TASK);
    auto *work = static_cast<SkNpcWaitWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->parent = parent;
    EV_debug_log("common\\et_skevent.c", 334, "WaitNpc");
    HLib::SetupTimedTask(parent, 0);
}
} // namespace shenmue::s2
