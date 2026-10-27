#include "s2/debug.hpp"
#include "s2/event_movement.hpp"
#include "common/math.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include <bit>
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
std::array<std::int32_t, EVENT_MOVEMENT_SLOT_COUNT> eventMovementStates{};
std::array<HLib::Task *, EVENT_MOVEMENT_SLOT_COUNT> eventMovementTasks{};
// @verified - p
void ResetEventMovementTasks() {
    eventMovementStates.fill(0);
    eventMovementTasks.fill(nullptr);
}
// @verified - p
std::uint32_t QueueEventMovement(
    HLib::Task *parent, shen_name character, const EventMovementWaypoint *waypoints, const EventMovementMotions *motions, float speed, std::uint32_t flags) {
    std::uint32_t slot = 0;
    while (slot < EVENT_MOVEMENT_SLOT_COUNT && eventMovementStates[slot] != 0)
        ++slot;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventMovementCallback, 6, 0x0b, 0x38, EVENT_MOVEMENT_TASK);
    EventMovementWork *const work = static_cast<EventMovementWork *>(HLib::GetTaskParameter(task));
    std::uint32_t previousFlags;
    std::memcpy(&previousFlags, &work->flags, sizeof(previousFlags));
    if ((previousFlags & 0x20000U) == 0)
        HLib::SetupTimedTask(task, 5);
    work->speed = speed;
    work->parent = parent;
    work->character = character;
    eventMovementStates[slot] = 1;
    eventMovementTasks[slot] = task;
    work->flags = flags;
    work->waypoints = waypoints;
    work->motions = motions;
    work->index = -1;
    work->active = 0;
    work->slot = slot;
    evntInit(work);
    if (work->parent)
        HLib::SetupTimedTask(work->parent, 0);
    return work->slot;
}
// @verified - p
void evntInit(EventMovementWork *work) {
    float frame = 0.0f;
    std::uint32_t commandFlags = 0;
    if ((work->flags & 0x20000U) == 0) {
        std::uint32_t animation;
        if ((work->flags & 0x80000000U) == 0) {
            animation = work->motions ? work->motions->forwardMotion : 0;
            if (animation == 0)
                animation = 0xf005;
        } else {
            animation = work->motions ? work->motions->backwardMotion : 0;
            if (animation == 0)
                animation = 0xf006;
        }
        if ((work->flags & 0x80000U) != 0)
            commandFlags = 0x40000000U;
        else if (animation == GetMotionAnimationId(scenFindMotionwork(work->character), MotionRegion::LowerBody)) {
            frame = GetMotionFrame(scenFindMotionwork(work->character), MotionRegion::LowerBody);
            commandFlags = 0x40000000U;
        }
        EVT_AT_SetMot(work->character, animation, frame, work->speed, commandFlags, 0);
    }
    if ((work->flags & 0x40000U) != 0) {
        Vector3 position{};
        Vector3i rotation{};
        EV_GetCharaPosition(work->character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        rotation.y = static_cast<std::int16_t>(atan2_fixed16(-(work->waypoints->position.x - position.x), -(work->waypoints->position.z - position.z)));
        EV_AT_SetAngY(work->character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
    }
}
// @verified - p
void StartEventMovementSegment(EventMovementWork *work) {
    const EventMovementWaypoint *const waypoint = &work->waypoints[work->index];
    const std::int32_t copyType = (work->flags & 0x4000000U) ? 1 : static_cast<std::int32_t>((work->flags >> 24) & 2U);
    std::int16_t operation;
    if (work->flags & 0x400000U)
        operation = 5;
    else if (work->flags & 0x200000U)
        operation = 6;
    else if (work->flags & 0x100000U)
        operation = 7;
    else
        operation = 0;
    const std::uint16_t yaw = static_cast<std::uint16_t>(static_cast<std::int32_t>(waypoint->yawDegrees * 8192.0f / 45.0f));
    CopyMotionTransition(scenFindMotionwork(work->character), &waypoint->position, yaw, operation, copyType);
    CopyMotionTransition(scenFindMotionwork(work->character), nullptr, 0, (work->flags & 0x80000000U) ? 8 : 14, 0);
    if (work->flags & 0x40000000U)
        CopyMotionTransition(scenFindMotionwork(work->character), nullptr, 0, 11, 0);
    SetMotionSpeed(scenFindMotionwork(work->character), work->speed, MotionRegion::LowerBody);
}
// @verified - p
void ConfigureEventMovementMotions(EventMovementWork *work) {
    if (!work->motions)
        return;
    const std::uint16_t defaultMotion = static_cast<std::uint16_t>(work->motions->defaultMotion);
    if (defaultMotion)
        SetMotionTimeRelated(scenFindMotionwork(work->character), 1, 0, defaultMotion);
    if (work->motions->forwardMotion)
        SetMotionTimeRelated(scenFindMotionwork(work->character), 1, 1, static_cast<std::uint16_t>(work->motions->forwardMotion));
    if (work->motions->backwardMotion)
        SetMotionTimeRelated(scenFindMotionwork(work->character), 1, 4, static_cast<std::uint16_t>(work->motions->backwardMotion));
    if (work->motions->stopMotion)
        SetMotionTimeRelated(scenFindMotionwork(work->character), 1, 5, static_cast<std::uint16_t>(work->motions->stopMotion));
}
// @verified - p
void EventMovementCallback(void *taskData) {
    EventMovementWork *const work = static_cast<EventMovementWork *>(taskData);
    if (work->active == 0) {
        ++work->index;
        work->active = 1;
        StartEventMovementSegment(work);
        ConfigureEventMovementMotions(work);
    } else if (work->active == 1 && mtwkSetExtraFlag(scenFindMotionwork(work->character), 58, 1) == 0) {
        if (work->waypoints[work->index + 1].position.y <= 0.0f) {
            if (work->parent)
                HLib::ResolveDeferredCallback(work->parent);
            SetMotionTimeRelated(scenFindMotionwork(work->character), 0, 0, 0);
            eventMovementStates[work->slot] = 0;
            HLib::DestroyCurrentTask();
        } else
            work->active = 0;
    }
}
// @verified - p
void SetEventMovementSegment(std::int32_t slot, std::int32_t index) {
    if (eventMovementStates[slot] == 0)
        return;
    EventMovementWork *const work = static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[slot]));
    work->index = index;
    StartEventMovementSegment(work);
    ConfigureEventMovementMotions(work);
    work->active = 1;
}
// @verified - p
void CancelEventMovement(std::int32_t slot) {
    if (eventMovementStates[slot] == 0)
        return;
    EventMovementWork *const work = static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[slot]));
    CopyMotionTransition(scenFindMotionwork(work->character), nullptr, 0, 1, 0);
    SetMotionTimeRelated(scenFindMotionwork(work->character), 0, 0, 0);
    if (work->parent)
        HLib::ResolveDeferredCallback(work->parent);
    HLib::Task *const task = eventMovementTasks[slot];
    eventMovementStates[slot] = 0;
    HLib::DestroyTaskInternal(task);
}
namespace {
// @verified - p
void AdmitEventReadCharacter(shen_name character) {
    EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterDrawControl(character, 1, 1);
    AT_SetLocalBaseChara(character, EVENT_MOVEMENT_INVALID);
    figpSetFlagsBlendForBlur(character, 2, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
    mtwkSetExtraFlag(scenFindMotionwork(character), 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(character), 3, 2);
    mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    SetMotionFrameTime(scenFindMotionwork(character), 7, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 66, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 69, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(character), 71, 2, MotionRegion::LowerBody);
    SetPluralControlFor(FindCharacterByScenarioName(character), 1);
}
}
// @verified - p
EventNpcReadWork *QueueEventNpcRead() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventNpcReadCallback, 4, 0x0b, 0x188, EVENT_MOVEMENT_TASK);
    EventNpcReadWork *const work = static_cast<EventNpcReadWork *>(HLib::GetTaskParameter(task));
    work->busy = 0;
    work->loadingCharacter = EVENT_MOVEMENT_INVALID;
    for (std::size_t index = 0; index < 32; ++index) {
        work->characters[index] = EVENT_MOVEMENT_INVALID;
        work->states[index] = 0;
        work->requests[index] = 0;
    }
    return work;
}
// @verified - p
void RequestEventNpcCharacters(EventNpcReadWork *work, const shen_name *characters) {
    work->busy = 1;
    for (std::size_t index = 0; index < 32; ++index)
        if ((work->states[index] == 3 && work->requests[index] != 3) || work->requests[index] == 1)
            work->requests[index] = 2;
    std::size_t cursor = 0;
    for (const shen_name *character = characters; character->name != EVENT_MOVEMENT_INVALID.name; ++character) {
        std::size_t index = 0;
        for (; index < 32; ++index) {
            if (work->characters[index].name == character->name) {
                work->requests[index] = 1;
                break;
            }
            if (work->characters[cursor].name != EVENT_MOVEMENT_INVALID.name && ++cursor >= 32) {
                cursor = 0;
                EV_debug_log("common\\et_readnpc.c", 279, "-------- Error : RTC_ID Over --------\n");
            }
        }
        if (index == 32) {
            work->characters[cursor] = *character;
            work->requests[cursor] = 1;
        }
    }
}
// @verified - p
void EventNpcReadCallback(void *taskData) {
    EventNpcReadWork *const work = static_cast<EventNpcReadWork *>(taskData);
    if (work->busy != 0 && work->loadingCharacter.name != EVENT_MOVEMENT_INVALID.name) {
        const std::int32_t result = static_cast<std::int32_t>(EV_npcStreamOrLoadHuman(work->loadingCharacter));
        if (result == 1) {
            AdmitEventReadCharacter(work->loadingCharacter);
            work->loadingCharacter = EVENT_MOVEMENT_INVALID;
        } else {
            if (result < 0)
                EV_debug_log("common\\et_readnpc.c", 150, "-------- Error %d : %c%c%c%c --------\n", result, work->loadingCharacter.name & 255U,
                    (work->loadingCharacter.name >> 8) & 255U, (work->loadingCharacter.name >> 16) & 255U, (work->loadingCharacter.name >> 24) & 255U);
            return;
        }
    }
    if (work->busy != 0)
        work->busy = 0;
    for (std::size_t index = 0; index < 32; ++index) {
        const shen_name character = work->characters[index];
        if (character.name == EVENT_MOVEMENT_INVALID.name)
            work->states[index] = 0;
        else if (work->requests[index] == 1) {
            if (work->states[index] != 0) {
                if (static_cast<std::uint32_t>(work->states[index] - 1) <= 1U)
                    work->states[index] = 3;
            } else if (work->busy == 0) {
                EV_CheckWarmStartNpcRoute(character);
                work->busy = 1;
                work->loadingCharacter = character;
                work->states[index] = 1;
            } else
                continue;
        } else if (work->requests[index] == 2) {
            if (work->states[index] != 0)
                work->states[index] = 2;
            else
                work->characters[index] = EVENT_MOVEMENT_INVALID;
        } else if (work->requests[index] == 3) {
            if (work->states[index] != 0) {
                EV_npcFree(character);
                if (IsNpcRouteLoadedOrVisible(character))
                    EV_npcLockUnlock(character);
            }
            work->characters[index] = EVENT_MOVEMENT_INVALID;
            work->states[index] = 0;
        } else if (work->states[index] == 1)
            work->states[index] = 3;
        work->requests[index] = 0;
    }
}
// @verified - p
void UpdateEventChase(EventChaseWork *work) {
    const std::int32_t previousTicks = work->ticks;
    work->ticks = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(previousTicks) + 1U);
    if (previousTicks >= work->delay) {
        const float positionGain = std::fabs(
            sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(work->ticks) * work->positionGainFrequency * 8192.0f / 45.0f))) *
            work->positionGainAmplitude);
        const float positionDeadzone = std::fabs(sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(
                                                     static_cast<float>(work->ticks) * work->positionDeadzoneFrequency * 8192.0f / 45.0f))) *
            work->positionDeadzoneAmplitude);
        Vector3 position{};
        EV_GetCharaPosition(work->target, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const Vector3 desired = work->positions[work->readIndex];
        const float positionDeltas[3]{desired.x - position.x, desired.y - position.y, desired.z - position.z};
        const float gains[3]{work->positionGain.x, work->positionGain.y, work->positionGain.z};
        const float deadzones[3]{work->positionDeadzone.x, work->positionDeadzone.y, work->positionDeadzone.z};
        float *const components[3]{&position.x, &position.y, &position.z};
        for (std::size_t index = 0; index < 3; ++index) {
            float correction = std::fabs(positionDeltas[index]) - (positionDeadzone + deadzones[index]);
            if (correction > 0.0f) {
                if (positionDeltas[index] < 0.0f)
                    correction = -correction;
                *components[index] += (positionGain + gains[index]) * correction;
            }
        }
        const float rotationGain = std::fabs(
            sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(work->ticks) * work->rotationGainFrequency * 8192.0f / 45.0f))) *
            work->rotationGainAmplitude);
        const float rotationDeadzone = std::fabs(sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(
                                                     static_cast<float>(work->ticks) * work->rotationDeadzoneFrequency * 8192.0f / 45.0f))) *
            work->rotationDeadzoneAmplitude);
        Vector3i rotation{};
        GetScenarioCharacterEulerAngles(work->target, &rotation, EventTransformFlags::World);
        const Vector3i targetRotation = work->rotations[work->readIndex];
        const std::int32_t targets[3]{targetRotation.x, targetRotation.y, targetRotation.z};
        const float rotationGains[3]{work->rotationGain.x, work->rotationGain.y, work->rotationGain.z};
        const float rotationDeadzones[3]{work->rotationDeadzone.x, work->rotationDeadzone.y, work->rotationDeadzone.z};
        std::int32_t *const angles[3]{&rotation.x, &rotation.y, &rotation.z};
        for (std::size_t index = 0; index < 3; ++index) {
            float delta =
                static_cast<float>(std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(targets[index]) - static_cast<std::uint32_t>(*angles[index]))) /
                182.04445f;
            float magnitude = std::fabs(delta);
            if (magnitude > 180.0f) {
                delta = -delta;
                magnitude = 360.0f - magnitude;
            }
            float correction = magnitude - (rotationDeadzone + rotationDeadzones[index]);
            if (correction > 0.0f) {
                if (delta < 0.0f)
                    correction = -correction;
                *angles[index] = static_cast<std::int32_t>(
                    ((rotationGain + rotationGains[index]) * correction + static_cast<float>(*angles[index]) / 182.04445f) * 8192.0f / 45.0f);
            }
        }
        EV_AT_SetPos(work->target, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(work->target, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
        work->readIndex = static_cast<std::int16_t>((work->readIndex + 1) % 32);
    }
    work->writeIndex = static_cast<std::int16_t>((work->writeIndex + 1) % 32);
    EV_GetCharaPosition(work->source, CHARACTER_TRANSLATION_ROOT, &work->positions[work->writeIndex], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->source, &work->rotations[work->writeIndex], EventTransformFlags::World);
}
} // namespace shenmue::s2
