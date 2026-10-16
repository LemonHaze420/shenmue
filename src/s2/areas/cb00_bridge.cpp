#include "s2/areas/area_cb00.hpp"
#include "s2/event_runtime.hpp"
#include "s2/math.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
HLib::Task *cb00BridgeTask;
HLib::Task *cb00BridgeRequestTask;
void Cb00ResetBridge(Cb00BridgeWork *work) {
    const Cb00BridgeProfile &profile = *work->profile;
    work->angle = 0;
    EV_AT_SetPos(profile.character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
    Vector3 position{};
    Vector3i rotation{};
    for (std::int32_t index = 0; index < work->pairs; ++index) {
        position.x = profile.span;
        EV_scenApplyPosOrRotToPartOf(
            profile.character, static_cast<std::uint16_t>(profile.parts[index]), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        position.x = -profile.span;
        EV_scenApplyPosOrRotToPartOf(
            profile.character, static_cast<std::uint16_t>(profile.parts[index + work->pairs]), &position, &rotation, EventCharacterPartTransformMode::Absolute);
    }
}
void Cb00UpdateBridge(Cb00BridgeWork *work) {
    const Cb00BridgeProfile &profile = *work->profile;
    Vector3 position{};
    float height = 0;
    std::int32_t accumulatedAngle = 0;
    for (std::int32_t index = 0; index < work->pairs; ++index) {
        Vector3i rotation{};
        rotation.z = static_cast<std::int32_t>(static_cast<float>(work->maximumAngle / (work->pairs - index)) * work->gain);
        rotation.z = static_cast<std::int32_t>((static_cast<float>(rotation.z) * (sin_fixed16(static_cast<std::int16_t>(work->angle)) + 1)) * 0.5f);
        position.x = profile.span;
        position.y = sin_fixed16(static_cast<std::int16_t>(rotation.z)) * (position.x * 0.5f);
        EV_scenApplyPosOrRotToPartOf(
            profile.character, static_cast<std::uint16_t>(profile.parts[index]), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        const float sine = sin_fixed16(static_cast<std::int16_t>(accumulatedAngle + rotation.z));
        const float rise = index == work->pairs - 1 ? sine * (profile.span * 0.5f) : sine * profile.span;
        if (index != work->pairs - 1)
            accumulatedAngle += rotation.z;
        rotation.z = -rotation.z;
        position.x = -position.x;
        EV_scenApplyPosOrRotToPartOf(
            profile.character, static_cast<std::uint16_t>(profile.parts[index + work->pairs]), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        height = rise + height;
    }
    position = {work->position.x, work->position.y - height, work->position.z};
    EV_AT_SetPos(profile.character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}
void Cb00PlaceBridgeCharacter(Cb00BridgeWork *work, float height) {
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(work->character, 0, &position, EventTransformFlags::World);
    position.y = height;
    EV_AT_SetPos(work->profile->base, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    GetScenarioCharacterEulerAngles(work->character, &rotation, EventTransformFlags::World);
    EV_AT_SetAngY(work->profile->base, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_AT_SetPos(work->character, EventTransformFlags::AllAxes, {});
    EV_AT_SetAngY(work->character, EventTransformFlags::AllAxes, {});
}
void Cb00UpdateBridgeCharacter(Cb00BridgeWork *work) {
    if (!work->character.name)
        return;
    Vector3 position;
    EV_GetCharaPosition(work->profile->character, 0xffffffffU, &position, EventTransformFlags::Local);
    float height = position.y;
    EV_GetCharaPosition(work->character, 0xffffffffU, &position, EventTransformFlags::World);
    float distance = std::fabs(GetPlanarDistance(&position, &work->position));
    if (!(work->length * 0.5f > distance))
        return;
    std::int32_t index = 0;
    while (distance > work->profile->span) {
        Vector3i rotation;
        EV_scenApplyPosOrRotToPartOf(
            work->profile->character, static_cast<std::uint16_t>(work->profile->parts[index++]), &position, &rotation, EventCharacterPartTransformMode::Get);
        height -= position.y;
        distance -= work->profile->span;
    }
    Cb00PlaceBridgeCharacter(work, height);
}
void Cb00BridgeCallback(void *data) {
    Cb00BridgeWork *const work = static_cast<Cb00BridgeWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->angleStep)
        work->angle += static_cast<std::int32_t>(static_cast<float>(work->angleStep) * work->speed);
    Cb00UpdateBridge(work);
    Cb00UpdateBridgeCharacter(work);
}
void Cb00BridgeCharacterCallback(void *data) {
    Cb00BridgeCharacterWork *const request = static_cast<Cb00BridgeCharacterWork *>(data);
    if (!request) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (cb00BridgeTask) {
        Cb00BridgeWork *const work = static_cast<Cb00BridgeWork *>(HLib::GetTaskParameter(cb00BridgeTask));
        if (work) {
            if (work->character.name)
                AT_SetLocalBaseChara(work->character, shen_name{0xffffffffU});
            work->character = request->character;
            if (work->character.name) {
                Vector3 position;
                Vector3i rotation;
                EV_GetCharaPosition(work->character, 0xffffffffU, &position, EventTransformFlags::World);
                GetScenarioCharacterEulerAngles(work->character, &rotation, EventTransformFlags::World);
                EV_AT_SetPos(work->profile->base, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
                EV_AT_SetAngY(work->profile->base, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
                AT_SetLocalBaseChara(work->character, work->profile->base);
            }
        }
    }
    if (request->cleanup)
        HLib::DestroyCurrentTask();
}
void Cb00BridgeEnqueue(std::int32_t delay) {
    if (cb00BridgeTask)
        return;
    cb00BridgeTask = HLib::EnqueueTaskWithParameter(Cb00BridgeCallback, 6, 11, sizeof(Cb00BridgeWork), CB00_TASK);
    if (!cb00BridgeTask)
        return;
    Cb00BridgeWork *const work = static_cast<Cb00BridgeWork *>(HLib::GetTaskParameter(cb00BridgeTask));
    if (!work) {
        HLib::DestroyTaskInternal(cb00BridgeTask);
        cb00BridgeTask = nullptr;
        return;
    }
    *work = {};
    work->profile = &CB00_BRIDGE_PROFILE;
    work->angleStep = 1820;
    work->maximumAngle = 910;
    work->gain = 0.5f;
    work->speed = 1;
    work->pairs = CB00_BRIDGE_PROFILE.partCount / 2;
    work->length = static_cast<float>(CB00_BRIDGE_PROFILE.partCount + 1) * CB00_BRIDGE_PROFILE.span;
    EV_GetCharaPosition(CB00_BRIDGE_PROFILE.character, 0xffffffffU, &work->position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(CB00_BRIDGE_PROFILE.character, &work->rotation, EventTransformFlags::World);
    Cb00BridgeCharacterWork request{.character = shen_name{static_cast<std::uint32_t>(CB00_BRIDGE_PROFILE.attachedCharacter)}, .cleanup = 0};
    Cb00BridgeCharacterCallback(&request);
    Cb00ResetBridge(work);
    if (delay)
        HLib::SetupTimedTask(cb00BridgeTask, delay);
}
void Cb00BridgeEndCallback(void *data) {
    const std::int32_t *const cleanup = static_cast<std::int32_t *>(data);
    if (!cleanup) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (cb00BridgeTask) {
        Cb00BridgeWork *const work = static_cast<Cb00BridgeWork *>(HLib::GetTaskParameter(cb00BridgeTask));
        if (work) {
            Cb00ResetBridge(work);
            EV_AT_SetPos(work->profile->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
            EV_AT_SetAngY(work->profile->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->rotation);
            if (work->character.name)
                AT_SetLocalBaseChara(work->character, shen_name{0xffffffffU});
            HLib::DestroyTaskInternal(cb00BridgeTask);
            cb00BridgeTask = nullptr;
        }
    }
    if (*cleanup)
        HLib::DestroyCurrentTask();
}
void Cb00BridgeRequestCallback(void *data) {
    const Cb00BridgeRequestWork *const request = static_cast<Cb00BridgeRequestWork *>(data);
    std::byte *const target = static_cast<std::byte *>(HLib::GetTaskParameter(cb00BridgeRequestTask));
    std::memcpy(target + 4, &request->flags, 4);
    if (request->flags & 0x08000000U)
        std::memcpy(target + 0x18, &request->maximumAngle, 4);
    if (request->flags & 0x10000000U)
        std::memcpy(target + 0x24, &request->gain, 4);
    if (request->flags & 0x20000000U)
        std::memcpy(target + 0x48, &request->gainBits, 8);
    HLib::DestroyCurrentTask();
}
void QueueCb00BridgeRequest(std::uint32_t flags, float gain, std::int32_t delay) {
    if (!cb00BridgeRequestTask)
        return;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00BridgeRequestCallback, 5, 11, sizeof(Cb00BridgeRequestWork), CB00_TASK);
    HLib::SetupTimedTask(task, delay);
    Cb00BridgeRequestWork *const work = static_cast<Cb00BridgeRequestWork *>(HLib::GetTaskParameter(task));
    work->flags = flags;
    work->maximumAngle = 0;
    work->gain = gain;
    work->gainBits = 0;
}
} // namespace shenmue::s2
