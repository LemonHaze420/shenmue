#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
std::uint64_t EV_GetPawnSetItem(std::int32_t set, std::uint32_t ordinal);
struct EventCallFunctionWork {
    HLib::TaskCallback callback;
    void *parameters;
    std::int32_t remainingCalls;
    float reserved14[2];
    float delay;
};
static_assert(sizeof(EventCallFunctionWork) == 0x20);
static_assert(offsetof(EventCallFunctionWork, remainingCalls) == 0x10);
static_assert(offsetof(EventCallFunctionWork, delay) == 0x1c);
void EventCallFunction_Callback(void *data);
EventCallFunctionWork *QueueEventCallFunction(HLib::TaskCallback callback, void *parameters, std::int32_t delayFrames);
EventCallFunctionWork *QueueEventRepeatedFunction(HLib::TaskCallback callback, void *parameters, std::int32_t delayFrames, std::int32_t iterations);
using EventTickFunction = void (*)(void *, std::int32_t);
struct EventTickFunctionWork {
    EventTickFunction callback;
    std::uint32_t reserved08[2];
    std::int32_t maxTime;
    std::int32_t currentTicks;
    std::int32_t paramSize;
    float delay;
};
static_assert(sizeof(EventTickFunctionWork) == 0x20);
static_assert(offsetof(EventTickFunctionWork, currentTicks) == 0x14);
static_assert(offsetof(EventTickFunctionWork, delay) == 0x1c);
void EventTickFunction_Callback(void *data);
}
