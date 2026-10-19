#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {




using EventQteAuthoredCallback = void (*)(void *resultParameters, std::uint32_t delayFrames);

struct EventQteData {
    std::uint16_t requiredQteStatus;
    std::uint16_t retryReferenceFrames;
    std::uint16_t maxTime;
    std::int16_t callbackTimeA;
    std::int16_t callbackTimeB;
    std::int16_t callbackTimeC;
    std::uint32_t authoredWord;
    EventQteAuthoredCallback beginSuccess;
    EventQteAuthoredCallback beginFailure;
    EventQteAuthoredCallback resultSuccess;
    EventQteAuthoredCallback resultFailure;
};

static_assert(sizeof(EventQteData) == 0x30);
static_assert(offsetof(EventQteData, beginSuccess) == 0x10);

enum class EventQtePhase : std::uint8_t {
    Hidden,
    Waiting,
    Prompt,
    Correct,
    Wrong,
    TimedOut,
};

struct EventQtePresentationState {
    std::uint16_t requiredStatus{};
    std::uint16_t currentStatus{0xffff};
    std::uint16_t maximumFrames{};
    std::uint16_t displayMode{};
    float elapsedFrames{};
    EventQtePhase phase{EventQtePhase::Hidden};
    bool active{};
};

using EventQtePresentationSink = void (*)(const EventQtePresentationState &state) noexcept;

void SetEventQtePresentationSink(EventQtePresentationSink sink) noexcept;
const EventQtePresentationState &GetEventQtePresentationState() noexcept;
void EVT_SetQteButtonState(std::uint16_t mask) noexcept;
bool IsEventQteInputActive() noexcept;
void EVT_SetQteFailureCount(std::uint32_t failures) noexcept;
extern std::int16_t g_eventQteRetryFrames;
void EVT_SetQteRetryFrames(std::uint16_t frames) noexcept;
std::uint32_t EVT_GetQteFailureCount() noexcept;
std::int32_t EV_GetCurrentQteStatus() noexcept;
struct EventQtePromptTaskData {
    std::int32_t autoCleanupTask;
    std::uint16_t requiredStatus;
    std::int16_t maximumFrames;
    std::int32_t reserved08;
    std::int32_t displayState;
    float delayFrames;
};
static_assert(sizeof(EventQtePromptTaskData) == 0x14);
static_assert(offsetof(EventQtePromptTaskData, delayFrames) == 0x10);
void EVT_AT_Qte_Callback(void *taskData);
void EVT_AT_QTE(std::uint16_t requiredQteStatus, std::int16_t maxTime, std::int32_t displayState, std::int32_t delayFrames);
std::uint32_t Queue_EVT_AT_ResultQTE(std::int32_t *result, void *resultParameters, const EventQteData *qteData);

struct EventQteResultTaskData {
    std::int32_t *result;
    void *resultParameters;
    const EventQteData *qteData;
};

static_assert(sizeof(EventQteResultTaskData) == 0x18);

} // namespace shenmue::s2
