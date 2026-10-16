#include "s2/areas/area_pigeons.hpp"
#include "s2/area_event.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/sound_program.hpp"

#include <cmath>

namespace shenmue::s2 {

std::array<AreaPigeon, 10> areaPigeons1{};
std::array<AreaPigeon, 10> areaPigeons2{};
std::array<AreaPigeon, 10> areaPigeons3{};
std::int32_t areaPigeonsActive{};
Vector3 areaPigeonPlayerPosition{};
Vector3i areaPigeonPlayerRotation{};

void QueueAreaPigeons(const Vector3 *origin, float radius, AreaPigeon *pigeons, shen_name firstCharacter) {
    Vector3 position;
    position.y = origin->y;
    const float center = radius * 0.5f;
    Vector3i rotation{.x = 0, .y = 0, .z = 0};
    for (std::int32_t index = 0; index < 10; ++index) {
        position.x = (GetRandomNumberInRange(radius) + origin->x) - center;
        position.z = (GetRandomNumberInRange(radius) + origin->z) - center;
        rotation.y = shenGetRandomInt32(65535);
        EV_AT_SetPos(firstCharacter, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(firstCharacter, EventTransformFlags::AllAxes, rotation, nullptr);
        EnableScenarioCharacterDisplay(firstCharacter);
        EnsureScenarioCharacterDraw(firstCharacter);
        AreaPigeon &pigeon = pigeons[index];
        pigeon.name = firstCharacter;
        pigeon.origin = *origin;
        pigeon.position = position;
        pigeon.roamRadius = radius;
        pigeon.rotation = rotation;
        pigeon.distanceToPlayer = 100.0f;
        pigeon.distanceToOrigin = 0.0f;
        pigeon.state = PigeonState::Wander;
        pigeon.randomWaitFrames = shenGetRandomInt32(1000);
        pigeon.bobPhase = 0;
        pigeon.birdCount = 10;
        AssignImageModuleParameters(ctrlGetCharaByName(firstCharacter), 1, -1, 3.0f, 1);
        firstCharacter.name += 0x01000000U;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaPigeonsCallback, 3, 0x0b, sizeof(AreaPigeon *), PIGEON_TASK);
    AreaPigeon **const parameter = static_cast<AreaPigeon **>(HLib::GetTaskParameter(task));
    areaPigeonsActive = 1;
    *parameter = pigeons;
}

void AreaPigeonsCallback(void *taskData) {
    if (EV_GetShenmueFlag(0x115)) {
        DestroyAreaPigeons(HLib::CurrentTask);
        return;
    }
    AreaPigeon *const pigeons = *static_cast<AreaPigeon **>(taskData);
    const std::int32_t count = pigeons->birdCount;
    EV_GetCharaPosition(PIGEON_PLAYER, CHARACTER_TRANSLATION_ROOT, &areaPigeonPlayerPosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(PIGEON_PLAYER, &areaPigeonPlayerRotation, EventTransformFlags::Local);
    if (g_currentMap.name == PIGEON_ABERDEEN_AREA.name) {
        const bool inside = (areaPigeonPlayerPosition.x > 218.0f && areaPigeonPlayerPosition.z > 420.0f && areaPigeonPlayerPosition.x < 270.0f &&
                                areaPigeonPlayerPosition.z < 450.0f) ||
            (areaPigeonPlayerPosition.x > 225.0f && areaPigeonPlayerPosition.z > 390.0f && areaPigeonPlayerPosition.x < 240.0f &&
                areaPigeonPlayerPosition.z < 440.0f);
        if (!inside) {
            if (areaPigeonsActive) {
                for (std::int32_t index = 0; index < count; ++index) {
                    DisableScenarioCharacterDisplay(pigeons[index].name);
                    ToggleScenarioCharacterFlag(pigeons[index].name);
                }
                areaPigeonsActive = 0;
            }
            return;
        }
        if (!areaPigeonsActive) {
            for (std::int32_t index = 0; index < count; ++index) {
                EnableScenarioCharacterDisplay(pigeons[index].name);
                EnsureScenarioCharacterDraw(pigeons[index].name);
            }
            areaPigeonsActive = 1;
        }
    }
    for (std::int32_t index = 0; index < count; ++index) {
        AreaPigeon &pigeon = pigeons[index];
        pigeon.distanceToPlayer = GetPlanarDistance(&pigeon.position, &areaPigeonPlayerPosition);
        pigeon.distanceToOrigin = GetPlanarDistance(&pigeon.position, &pigeon.origin);
        AREA_PIGEON_STATE_CALLBACKS[static_cast<std::int32_t>(pigeon.state)](&pigeon);
        EV_AT_SetPos(pigeon.name, EventTransformFlags::AllAxes, pigeon.position);
        EV_AT_SetAngY(pigeon.name, EventTransformFlags::AllAxes, pigeon.rotation, nullptr);
    }
}

void DestroyAreaPigeons(HLib::Task *task) {
    AreaPigeon **const pigeons = static_cast<AreaPigeon **>(HLib::GetTaskParameter(task));
    for (std::int32_t count = (*pigeons)->birdCount; count; --count) {
        SetCharaKill((*pigeons)->name, 1);
        ++*pigeons;
    }
    HLib::DestroyTask(task);
}

void PigeonCooldown(AreaPigeon *pigeon) {
    if (pigeon->distanceToPlayer >= 2.5f) {
        if (--pigeon->randomWaitFrames > 0)
            return;
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 3.0f, 1);
        pigeon->state = PigeonState::Wander;
    } else {
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 3.0f, 1);
        pigeon->state = PigeonState::Approach;
    }
    pigeon->randomWaitFrames =
        g_stabilizeFps ? shenGetRandomInt32(static_cast<std::int32_t>((1.0f / ((GetFrameTime() * 30000.0f) / 1000.0f)) * 1000.0f)) : shenGetRandomInt32(1000);
}

void PigeonWander(AreaPigeon *pigeon) {
    EvaluatePigeonRoam(pigeon);
    float stepX = 0.01f;
    if (g_stabilizeFps)
        stepX = GetFrameTime() * 0.29999998f;
    pigeon->position.x -= sin_fixed16(static_cast<std::int16_t>(pigeon->rotation.y)) * stepX;
    float stepZ = 0.01f;
    if (g_stabilizeFps)
        stepZ = GetFrameTime() * 0.29999998f;
    pigeon->position.z -= cos_fixed16(static_cast<std::uint16_t>(pigeon->rotation.y)) * stepZ;
    if (pigeon->distanceToOrigin > pigeon->roamRadius) {
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 0.5f, 0);
        Execute3DSoundCommand(839, pigeon->position, 4.0f, 40.0f, pigeon->distanceToOrigin);
        pigeon->state = PigeonState::Cooldown;
        pigeon->bobPhase = 0;
    } else if (pigeon->distanceToPlayer < 2.5f) {
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 3.0f, 1);
        pigeon->state = PigeonState::Approach;
    } else if (--pigeon->randomWaitFrames < 10) {
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 3.0f, 2);
        pigeon->state = PigeonState::HoverBob;
        pigeon->randomWaitFrames = 30;
    }
}

void PigeonApproach(AreaPigeon *pigeon) {
    const std::int16_t difference =
        static_cast<std::int16_t>(InverseFixedAtan2(pigeon->position.x - areaPigeonPlayerPosition.x, pigeon->position.z - areaPigeonPlayerPosition.z) -
            static_cast<std::int16_t>(pigeon->rotation.y));
    if (difference > 3640)
        pigeon->rotation.y += 1820;
    else if (difference < -3640)
        pigeon->rotation.y -= 1820;
    float stepX = 0.1f;
    if (g_stabilizeFps)
        stepX = GetFrameTime() * 3.0f;
    pigeon->position.x -= sin_fixed16(static_cast<std::int16_t>(pigeon->rotation.y)) * stepX;
    float stepZ = 0.1f;
    if (g_stabilizeFps)
        stepZ = GetFrameTime() * 3.0f;
    pigeon->position.z -= cos_fixed16(static_cast<std::uint16_t>(pigeon->rotation.y)) * stepZ;
    if (pigeon->distanceToPlayer > 3.0f) {
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 3.0f, 1);
        pigeon->state = PigeonState::Wander;
    } else {
        float context = 1.5f;
        if (pigeon->distanceToPlayer < 1.5f || (context = pigeon->distanceToOrigin, context > pigeon->roamRadius)) {
            AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 0.5f, 0);
            Execute3DSoundCommand(839, pigeon->position, 4.0f, 40.0f, context);
            pigeon->state = PigeonState::Cooldown;
            pigeon->bobPhase = 0;
        }
    }
}

void PigeonHoverBob(AreaPigeon *pigeon) {
    if (pigeon->roamRadius - 1.0f <= pigeon->distanceToOrigin) {
        EvaluatePigeonRoam(pigeon);
    } else {
        const std::int16_t difference =
            static_cast<std::int16_t>(InverseFixedAtan2(pigeon->position.x - areaPigeonPlayerPosition.x, pigeon->position.z - areaPigeonPlayerPosition.z) -
                static_cast<std::int16_t>(pigeon->rotation.y));
        if (difference > 3640)
            pigeon->rotation.y += 1820;
        else if (difference < -3640)
            pigeon->rotation.y -= 1820;
    }
    float phaseStep = 512.0f;
    if (g_stabilizeFps)
        phaseStep = GetFrameTime() * 15360.0f;
    pigeon->bobPhase = static_cast<std::int32_t>(static_cast<float>(pigeon->bobPhase) + phaseStep);
    if (pigeon->bobPhase > 32768) {
        AssignImageModuleParameters(ctrlGetCharaByName(pigeon->name), 1, -1, 3.0f, 1);
        pigeon->state = PigeonState::Wander;
        pigeon->bobPhase = 0;
    }
    float stepX = 0.2f;
    if (g_stabilizeFps)
        stepX = GetFrameTime() * 6.0f;
    pigeon->position.x -= sin_fixed16(static_cast<std::int16_t>(pigeon->rotation.y)) * stepX;
    float stepZ = 0.2f;
    if (g_stabilizeFps)
        stepZ = GetFrameTime() * 6.0f;
    pigeon->position.z -= cos_fixed16(static_cast<std::uint16_t>(pigeon->rotation.y)) * stepZ;
    pigeon->position.y = sin_fixed16(static_cast<std::int16_t>(pigeon->bobPhase)) * 1.5f + pigeon->origin.y;
}

std::uint16_t EvaluatePigeonRoam(AreaPigeon *pigeon) {
    if (pigeon->distanceToOrigin > pigeon->roamRadius + 0.5f) {
        const std::uint16_t yaw = static_cast<std::uint16_t>(InverseFixedAtan2(pigeon->origin.x - pigeon->position.x, pigeon->origin.z - pigeon->position.z));
        pigeon->rotation.y = yaw;
        return yaw;
    }
    if (pigeon->distanceToOrigin > pigeon->roamRadius)
        return ComputePigeonRotation(pigeon, &pigeon->origin, &pigeon->position, 10.0f);
    const float angle = std::fabs(pigeon->distanceToOrigin) >= 0.00000011920929f ? pigeon->distanceToOrigin / (pigeon->roamRadius + pigeon->roamRadius) : 0.0f;
    return ComputePigeonRotation(pigeon, &pigeon->origin, &pigeon->position, angle);
}

std::int16_t ComputePigeonRotation(AreaPigeon *pigeon, const Vector3 *origin, const Vector3 *position, float degrees) {
    const std::int16_t difference =
        static_cast<std::int16_t>(InverseFixedAtan2(origin->x - position->x, origin->z - position->z) - static_cast<std::int16_t>(pigeon->rotation.y));
    if (difference > 3640) {
        const std::int32_t step = static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f);
        pigeon->rotation.y += static_cast<std::uint16_t>(step);
        return static_cast<std::int16_t>(step);
    }
    if (difference < -3640) {
        const std::int32_t step = static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f);
        pigeon->rotation.y -= static_cast<std::uint16_t>(step);
        return static_cast<std::int16_t>(step);
    }
    return difference;
}

} // namespace shenmue::s2
