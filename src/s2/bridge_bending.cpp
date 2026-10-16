#include "s2/bridge_bending.hpp"
#include "s2/math.hpp"
#include <cmath>

namespace shenmue::s2 {
// @verified - p
void ResetBridgeSections(BridgeBendingWork *work) {
    Vector3 position{};
    Vector3i rotation{};
    work->phase = 0;
    EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
    for (std::int32_t index = 0; index < work->pairCount; ++index) {
        position.x = work->definition->sectionWidth;
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index]), &position, &rotation,
            EventCharacterPartTransformMode::Absolute);
        position.x = -work->definition->sectionWidth;
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index + work->pairCount]), &position,
            &rotation, EventCharacterPartTransformMode::Absolute);
    }
}

// @verified - p
void BendHasiBridge(BridgeBendingWork *work) {
    Vector3 position{};
    float height = 0.0f;
    std::uint16_t accumulatedAngle = 0;
    for (std::int32_t index = 0; index < work->pairCount; ++index) {
        Vector3i rotation{};
        rotation.z = static_cast<std::int32_t>(static_cast<float>(work->amplitude / (work->pairCount - index)) * work->scale);
        rotation.z = static_cast<std::int32_t>((static_cast<float>(rotation.z) * (sin_fixed16(static_cast<std::int16_t>(work->phase)) + 1.0f)) * 0.5f);
        position.x = work->definition->sectionWidth;
        position.y = sin_fixed16(static_cast<std::int16_t>(rotation.z)) * (position.x * 0.5f);
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index]), &position, &rotation,
            EventCharacterPartTransformMode::Absolute);
        const std::uint16_t angle = static_cast<std::uint16_t>(accumulatedAngle + static_cast<std::uint16_t>(rotation.z));
        float segmentHeight;
        if (index == work->pairCount - 1)
            segmentHeight = sin_fixed16(static_cast<std::int16_t>(angle)) * (work->definition->sectionWidth * 0.5f);
        else {
            segmentHeight = sin_fixed16(static_cast<std::int16_t>(angle)) * work->definition->sectionWidth;
            accumulatedAngle = angle;
        }
        rotation.z = -rotation.z;
        position.x *= -1.0f;
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index + work->pairCount]), &position,
            &rotation, EventCharacterPartTransformMode::Absolute);
        height = segmentHeight + height;
    }
    position.x = work->position.x;
    position.y = work->position.y - height;
    position.z = work->position.z;
    EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}

// @verified - p
void BendQdh1Bridge(BridgeBendingWork *work) {
    Vector3 position{};
    float height = 0.0f;
    std::uint16_t accumulatedAngle = 0;
    for (std::int32_t index = 0; index < work->pairCount; ++index) {
        Vector3i rotation{};
        rotation.z = static_cast<std::int32_t>(static_cast<float>(work->amplitude / (work->pairCount - index)) * work->scale);
        rotation.z *= static_cast<std::int32_t>((sin_fixed16(static_cast<std::int16_t>(work->phase)) + 1.0f) * 0.5f);
        position.x = work->definition->sectionWidth;
        position.y = sin_fixed16(static_cast<std::int16_t>(rotation.z)) * (position.x * 0.5f);
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index]), &position, &rotation,
            EventCharacterPartTransformMode::Absolute);
        const std::uint16_t angle = static_cast<std::uint16_t>(accumulatedAngle + static_cast<std::uint16_t>(rotation.z));
        float segmentHeight;
        if (index == work->pairCount - 1) {
            const double lastHeight =
                static_cast<double>(work->definition->sectionWidth) * 0.5 * static_cast<double>(sin_fixed16(static_cast<std::int16_t>(angle)));
            segmentHeight = static_cast<float>(static_cast<std::int32_t>(lastHeight));
        } else {
            segmentHeight = sin_fixed16(static_cast<std::int16_t>(angle)) * work->definition->sectionWidth;
            accumulatedAngle = angle;
        }
        rotation.z = -rotation.z;
        position.x *= -1.0f;
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index + work->pairCount]), &position,
            &rotation, EventCharacterPartTransformMode::Absolute);
        height = segmentHeight + height;
    }
    position.x = work->position.x;
    position.y = work->position.y - height;
    position.z = work->position.z;
    EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}

// @verified - p
void BendQdh2Bridge(BridgeBendingWork *work) {
    Vector3 position{};
    float height = 0.0f;
    std::uint16_t accumulatedAngle = 0;
    for (std::int32_t index = 0; index < work->pairCount; ++index) {
        Vector3i rotation{};
        rotation.z = static_cast<std::int32_t>((static_cast<float>(work->amplitude) / static_cast<float>(work->pairCount - index)) * work->scale);
        rotation.z *= static_cast<std::int32_t>((sin_fixed16(static_cast<std::int16_t>(work->phase)) + 1.0f) * 0.5f);
        position.x = work->definition->sectionWidth;
        position.y = sin_fixed16(static_cast<std::int16_t>(rotation.z)) * (position.x * 0.5f);
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index]), &position, &rotation,
            EventCharacterPartTransformMode::Absolute);
        const std::uint16_t angle = static_cast<std::uint16_t>(accumulatedAngle + static_cast<std::uint16_t>(rotation.z));
        float segmentHeight;
        if (index == work->pairCount - 1)
            segmentHeight = sin_fixed16(static_cast<std::int16_t>(angle)) * (work->definition->sectionWidth * 0.5f);
        else {
            segmentHeight = sin_fixed16(static_cast<std::int16_t>(angle)) * work->definition->sectionWidth;
            accumulatedAngle = angle;
        }
        rotation.z = -rotation.z;
        position.x *= -1.0f;
        EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index + work->pairCount]), &position,
            &rotation, EventCharacterPartTransformMode::Absolute);
        height = segmentHeight + height;
    }
    position.x = work->position.x;
    position.y = work->position.y - height;
    position.z = work->position.z;
    EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}

// @verified - p
void UpdateBridgeAttachedCharacter(BridgeBendingWork *work) {
    if (!work->attachedCharacter.name)
        return;
    Vector3 position;
    EV_GetCharaPosition(work->definition->character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    float height = position.y;
    EV_GetCharaPosition(work->attachedCharacter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    float distance = std::fabs(GetPlanarDistance(&position, &work->position));
    if (work->range * 0.5f <= distance) {
        SetBridgeAttachedCharacterHeight(work, work->position.y);
    } else {
        std::int32_t index = 0;
        while (distance > work->definition->sectionWidth) {
            Vector3i rotation;
            EV_scenApplyPosOrRotToPartOf(work->definition->character, static_cast<std::uint16_t>(work->definition->parts[index]), &position, &rotation,
                EventCharacterPartTransformMode::Get);
            ++index;
            height -= position.y;
            distance -= work->definition->sectionWidth;
        }
        EV_GetCharaPosition(work->attachedCharacter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.y = height + work->attachedHeight;
        EV_AT_SetPos(work->attachedCharacter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    }
}

// @verified - p
void SetBridgeAttachedCharacterHeight(BridgeBendingWork *work, float height) {
    Vector3 position;
    EV_GetCharaPosition(work->attachedCharacter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.y = height + work->attachedHeight;
    EV_AT_SetPos(work->attachedCharacter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}
}
