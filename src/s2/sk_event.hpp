#pragma once
#include "s2/hlib.hpp"
#include "s2/step_event.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name SK_EVENT_TASK{SHEN_NAME(EVNT)};
struct SkChoiceWaitWork {
    HLib::Task *self;
    HLib::Task *parent;
    std::int16_t choices[4];
    std::uint16_t timeout;
    std::uint16_t reserved1a;
    std::int32_t defaultChoice;
    std::uint16_t phase;
    std::byte reserved22[6];
};
struct SkEventMessageWork {
    const char *message;
    std::int32_t frames;
    std::uint32_t reserved0c;
};
static_assert(sizeof(SkChoiceWaitWork) == 0x28);
static_assert(offsetof(SkChoiceWaitWork, choices) == 0x10);
static_assert(offsetof(SkChoiceWaitWork, timeout) == 0x18);
static_assert(offsetof(SkChoiceWaitWork, defaultChoice) == 0x1c);
static_assert(offsetof(SkChoiceWaitWork, phase) == 0x20);
static_assert(sizeof(SkEventMessageWork) == 0x10);
void QueueSkAuthorStart(HLib::Task *parent, std::int32_t command, HLib::TaskCallback callback, void *callbackData, std::int32_t delayFrames);
void QueueSkChoiceWait(HLib::Task *parent, std::uintptr_t unusedArgument2, std::int16_t secondChoice, std::int16_t thirdChoice, std::int16_t fourthChoice,
    std::int32_t defaultChoice, std::int32_t unusedArgument7, std::int32_t delayFrames);
void QueueSkQteWait(std::int32_t *resultBlock, std::uint32_t button, std::int32_t referenceFrames, std::int32_t baseMaximumFrames, StepQteCallback success,
    StepQteCallback failure, std::int32_t delayFrames);
void QueueSkMoneyIconWait(HLib::Task *parent, std::int16_t maximum, std::int16_t step);
void QueueSkNpcWait(HLib::Task *parent);
void SkChoiceWaitCallback(void *);
void SkEventMessageBeginCallback(void *);
void SkEventMessageEndCallback(void *);
} // namespace shenmue::s2
