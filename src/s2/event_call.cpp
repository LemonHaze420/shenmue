#include "s2/event_call.hpp"
#include "s2/game_runtime.hpp"
#include "s2/pawn_shop.hpp"
#include <bit>
namespace shenmue::s2 {
// @verified - p
std::uint64_t EV_GetPawnSetItem(std::int32_t set, std::uint32_t ordinal) {
    return GetPawnSetItem(set, ordinal);
}


void EventCallFunction_Callback(void *data) {
    auto *work = static_cast<EventCallFunctionWork *>(data);
    if (work->delay <= 0.0f) {
        const std::int32_t previous = work->remainingCalls;
        work->remainingCalls = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(previous) - 1U);
        if (previous <= 0) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        } else
            work->callback(work->parameters);
    } else
        work->delay -= HLib::GetScaledDeltaTick();
}

EventCallFunctionWork *QueueEventRepeatedFunction(HLib::TaskCallback callback, void *parameters, std::int32_t delayFrames, std::int32_t iterations) {
    auto *task = HLib::EnqueueTaskWithParameter(EventCallFunction_Callback, 6, 0x0b, sizeof(EventCallFunctionWork), SHEN_NAME(EVNT));
    RegisterEventTask(task);
    auto *work = static_cast<EventCallFunctionWork *>(HLib::GetTaskParameter(task));
    work->callback = callback;
    work->parameters = parameters;
    work->remainingCalls = iterations;
    work->delay = static_cast<float>(delayFrames);
    return work;
}

EventCallFunctionWork *QueueEventCallFunction(HLib::TaskCallback callback, void *parameters, std::int32_t delayFrames) {
    return QueueEventRepeatedFunction(callback, parameters, delayFrames, 1);
}
void EventTickFunction_Callback(void *data) {
    EventTickFunctionWork *work = static_cast<EventTickFunctionWork *>(data);
    if (!(work->delay > 0.0f)) {
        const std::int32_t ticks = work->currentTicks;
        if (ticks >= work->maxTime) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        } else {
            work->callback(work->paramSize != 0 ? work + 1 : nullptr, ticks);
            work->currentTicks = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(work->currentTicks) + 1U);
        }
    } else
        work->delay -= HLib::GetScaledDeltaTick();
}
}
