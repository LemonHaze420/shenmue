#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/hlib.hpp"
#include <array>
#include <cstddef>
namespace shenmue::s2 {
inline constexpr shen_name EVENT_MOVEMENT_TASK{SHEN_NAME(EVNT)}, EVENT_MOVEMENT_INVALID{0xffffffffU};
inline constexpr std::size_t EVENT_MOVEMENT_SLOT_COUNT = 20;
struct EventMovementWaypoint {
    Vector3 position;
    float yawDegrees;
};
struct EventMovementMotions {
    std::uint32_t defaultMotion, forwardMotion, reserved08, reserved0c, backwardMotion, stopMotion;
};
struct EventMovementWork {
    HLib::Task *parent;
    shen_name character;
    std::uint32_t reserved0c;
    const EventMovementWaypoint *waypoints;
    const EventMovementMotions *motions;
    float speed;
    std::int32_t index;
    std::int32_t active;
    std::uint32_t slot;
    std::uint32_t flags;
};
struct EventNpcReadWork {
    shen_name characters[32];
    std::int32_t states[32];
    std::int32_t requests[32];
    std::int32_t busy;
    shen_name loadingCharacter;
};
struct EventChaseWork {
    shen_name source, target;
    std::int32_t delay;
    Vector3 positionGain, rotationGain;
    float positionGainAmplitude, rotationGainAmplitude, positionGainFrequency, rotationGainFrequency;
    Vector3 positionDeadzone, rotationDeadzone;
    float positionDeadzoneAmplitude, rotationDeadzoneAmplitude, positionDeadzoneFrequency, rotationDeadzoneFrequency;
    std::int32_t ticks;
    std::int16_t readIndex, writeIndex;
    Vector3 positions[32];
    Vector3i rotations[32];
};
static_assert(sizeof(EventMovementWaypoint) == 16);
static_assert(sizeof(EventMovementMotions) == 24);
static_assert(sizeof(EventMovementWork) == 0x38);
static_assert(offsetof(EventMovementWork, flags) == 0x30);
static_assert(sizeof(EventNpcReadWork) == 0x188);
static_assert(offsetof(EventNpcReadWork, states) == 0x80);
static_assert(offsetof(EventNpcReadWork, requests) == 0x100);
static_assert(offsetof(EventNpcReadWork, busy) == 0x180);
static_assert(sizeof(EventChaseWork) == 0x364);
static_assert(offsetof(EventChaseWork, positions) == 0x64);
static_assert(offsetof(EventChaseWork, rotations) == 0x1e4);
extern std::array<std::int32_t, EVENT_MOVEMENT_SLOT_COUNT> eventMovementStates;
extern std::array<HLib::Task *, EVENT_MOVEMENT_SLOT_COUNT> eventMovementTasks;
void ResetEventMovementTasks();
std::uint32_t QueueEventMovement(
    HLib::Task *parent, shen_name character, const EventMovementWaypoint *waypoints, const EventMovementMotions *motions, float speed, std::uint32_t flags);
void EventMovementCallback(void *);
void evntInit(EventMovementWork *);
void StartEventMovementSegment(EventMovementWork *);
void ConfigureEventMovementMotions(EventMovementWork *);
void SetEventMovementSegment(std::int32_t slot, std::int32_t index);
void CancelEventMovement(std::int32_t slot);
EventNpcReadWork *QueueEventNpcRead();
void RequestEventNpcCharacters(EventNpcReadWork *, const shen_name *characters);
void EventNpcReadCallback(void *);
void UpdateEventChase(EventChaseWork *);
} // namespace shenmue::s2
