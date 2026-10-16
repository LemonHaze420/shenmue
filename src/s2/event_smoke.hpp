#pragma once
#include "common/types.hpp"
#include <cstddef>
namespace shenmue::s2 {
inline constexpr shen_name EVENT_SMOKE_TASK{SHEN_NAME(EVNT)};
struct EventSmokeWork {
    shen_name character;
    std::uint32_t bone;
    std::uint32_t effect;
    std::int32_t startDelay, remaining, finishDelay, profile, state;
    float birthFraction, birthStep;
};
static_assert(sizeof(EventSmokeWork) == 0x28);
static_assert(offsetof(EventSmokeWork, startDelay) == 0x0c);
static_assert(offsetof(EventSmokeWork, remaining) == 0x10);
static_assert(offsetof(EventSmokeWork, finishDelay) == 0x14);
static_assert(offsetof(EventSmokeWork, profile) == 0x18);
static_assert(offsetof(EventSmokeWork, state) == 0x1c);
static_assert(offsetof(EventSmokeWork, birthFraction) == 0x20);
void CreateEventSmokeAtCharacter(EventSmokeWork *work);
void EventSmokeCallback(void *work);
void InitializeWarehouseFallingLeaves(std::uint32_t *effect);
} // namespace shenmue::s2
