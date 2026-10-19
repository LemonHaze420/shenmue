#include "s1/motion_internal.hpp"

namespace shenmue::s1 {
void MOTN_ReadVfRootPosition(MotionInformationWork &motion, float frame, Vector3 &outPosition);

bool motCalcDeltaXZFromAnimTime(MotionInformationWork &motion, float frame, Vector3 &outPosition);

float MOTN_CalculateRootMotionSpeedZ(std::uint16_t animationId, std::uint16_t basePoseTemplateIndex, bool *outUsedRootChannel);

void motmApplyRootMot(const Character *character, const Vector3 &localPoint, Vector3 &worldPoint);


std::int32_t MOTN_GetFrameCountFromAnimId(std::uint16_t animationId) {
    for (const std::uint16_t basePose : kBasePoseAnimationIds) {
        if (animationId == basePose)
            return 20;
    }
    const MotnAnimationHeader *const animation = MOTN_GetDataForAnimId(animationId);
    return animation ? animation->attribute & 0x7fffU : 0;
}

std::int32_t motnGetRootMotionEndMarker(std::uint16_t animationId) {
    for (const std::uint16_t basePose : kBasePoseAnimationIds) {
        if (animationId == basePose)
            return 0;
    }
    return MotionHasRootEndMarker(animationId) ? 1 : 0;
}

bool MOTN_ReadRootPositionAtFrame(std::uint16_t animationId, std::uint32_t basePoseTemplateIndex, float frame, Vector3 &outPosition, bool includeY) {
    outPosition = {};
    if (!std::isfinite(frame))
        return false;
    const MotnAnimationHeader *const animation = MOTN_GetDataForAnimId(animationId);
    if (!animation)
        return false;
    float scale = 1.0f;
    if (basePoseTemplateIndex <= 20U) {
        const MotionRigType *const target = MOTN_GetRigType(static_cast<std::uint16_t>(basePoseTemplateIndex));
        const MotionRigType *const source = MOTN_GetRigType(animation->sourceRig & 0x7fffU);
        if (!target || !source)
            return false;
        scale = target->nodes[0].position.y / source->nodes[0].position.y;
    }
    const auto *const bytes = reinterpret_cast<const std::uint8_t *>(animation);
    if (static_cast<std::int16_t>(animation->sourceRig) < 0) {
        const std::int32_t count = animation->attribute;
        if (count == 0)
            return false;
        const std::int32_t index = frame < 0.0f || frame >= static_cast<float>(count) ? count - 1 : static_cast<std::int32_t>(frame);
        const auto *const record = bytes + 0x0e + index * static_cast<std::int16_t>(animation->block2Offset);
        std::int16_t value;
        std::memcpy(&value, record, sizeof(value));
        outPosition.x = F16ToF32(value) * scale;
        if (includeY) {
            std::memcpy(&value, record + 2, sizeof(value));
            outPosition.y = F16ToF32(value) * scale;
        }
        std::memcpy(&value, record + 4, sizeof(value));
        outPosition.z = F16ToF32(value) * scale;
        return true;
    }
    const std::int32_t count = animation->attribute & 0x7fffU;
    if (count == 0 || (*reinterpret_cast<const std::uint16_t *>(animation + 1) & 0x1c0U) != 0x1c0U)
        return false;
    if (frame < 0.0f || frame > static_cast<float>(count - 1))
        frame = static_cast<float>(count - 1);
    const float time = static_cast<float>(static_cast<double>(frame) * 0.03333333333333333);
    MotnParseCursor cursor{};
    if (animation->attribute & 0x8000U)
        cursor.block2Words = reinterpret_cast<const std::uint16_t *>(bytes + animation->block2Offset);
    else
        cursor.block2Bytes = bytes + animation->block2Offset;
    if (count > 255)
        cursor.block3Words = reinterpret_cast<const std::uint16_t *>(bytes + animation->block3Offset);
    else
        cursor.block3Bytes = bytes + animation->block3Offset;
    cursor.block4 = bytes + animation->block4Offset;
    cursor.block5 = reinterpret_cast<const std::uint16_t *>(bytes + animation->block5Offset);
    MotnKeyframeChannel channel;
    ReadKeyframeData(cursor, channel);
    outPosition.x = MOTN_CalculateKeyframeForTime(channel, time, count) * scale;
    ReadKeyframeData(cursor, channel);
    if (includeY)
        outPosition.y = MOTN_CalculateKeyframeForTime(channel, time, count) * scale;
    ReadKeyframeData(cursor, channel);
    outPosition.z = MOTN_CalculateKeyframeForTime(channel, time, count) * scale;
    return true;
}

void MOTN_ReadVfRootPosition(MotionInformationWork &motion, float frame, Vector3 &outPosition) {
    const bool pending = motion.pendingAnimId != 0;
    const std::uint16_t animationId = pending ? motion.pendingAnimId : motion.currentAnimId;
    const bool mirrored = pending ? (((motion.motionFlags >> 7U) & 1U) != static_cast<std::uint32_t>(shenLookupAnimationIndex(animationId, 19U) != nullptr)) :
                                    ((motion.motionFlags >> 17U) & 1U) != 0;
    g_motnHasYValue = 1;
    static_cast<void>(MOTN_ReadRootPositionAtFrame(animationId, static_cast<std::uint32_t>(motion.basePoseTemplateIndex), frame, outPosition));
    if (mirrored)
        outPosition.x = -outPosition.x;
}

bool motCalcDeltaXZFromAnimTime(MotionInformationWork &motion, float frame, Vector3 &outPosition) {
    outPosition = {};
    const bool pending = motion.pendingAnimId != 0;
    const std::uint16_t animationId = pending ? motion.pendingAnimId : motion.currentAnimId;
    if (!animationId || !std::isfinite(frame) || frame < 0.0f)
        return false;

    Vector3 previous{};
    bool mirrored = false;
    if (pending) {
        if (frame != 0.0f)
            motion.forcedFrameTime = frame;
        std::uint16_t unused{};
        mirrored = ((motion.motionFlags >> 7U) & 1U) != static_cast<std::uint32_t>(FindAnimationExtraWord(animationId, 19U, unused));
    } else {
        mirrored = ((motion.motionFlags >> 17U) & 1U) != 0;
        motion.frameTimer = frame;
        if (motion.frameTimerRef >= 0.0f &&
            !MOTN_ReadRootPositionAtFrame(animationId, static_cast<std::uint32_t>(motion.basePoseTemplateIndex), motion.frameTimerRef, previous)) {
            return false;
        }
    }

    if (!MOTN_ReadRootPositionAtFrame(animationId, static_cast<std::uint32_t>(motion.basePoseTemplateIndex), frame, outPosition)) {
        outPosition = {};
        return false;
    }
    if (mirrored) {
        previous.x = -previous.x;
        outPosition.x = -outPosition.x;
    }
    outPosition.x -= previous.x;
    outPosition.z -= previous.z;
    return true;
}

float MOTN_CalculateRootMotionSpeedZ(std::uint16_t animationId, std::uint16_t basePoseTemplateIndex, bool *outUsedRootChannel) {
    constexpr float kDefaultFrameSpeed = 0.041666668f;
    if (outUsedRootChannel)
        *outUsedRootChannel = false;
    const std::int32_t duration = MOTN_GetFrameCountFromAnimId(animationId) - motnGetRootMotionEndMarker(animationId);
    if (duration < 1)
        return kDefaultFrameSpeed;

    Vector3 endPosition{};
    if (!MOTN_ReadRootPositionAtFrame(animationId, basePoseTemplateIndex, static_cast<float>(duration), endPosition)) {
        return kDefaultFrameSpeed;
    }
    if (outUsedRootChannel)
        *outUsedRootChannel = true;
    if (!std::isfinite(endPosition.z) || std::fabs(endPosition.z) < 0x1p-23f) {
        return kDefaultFrameSpeed;
    }
    return -(endPosition.z / static_cast<float>(duration));
}

void motmApplyRootMot(const Character *character, const Vector3 &localPoint, Vector3 &worldPoint) {
    Vector3 root{};
    std::int32_t yaw = 0;
    if (character) {
        root = GetPositionForEntity(const_cast<CharacterNode &>(character->entity.node));
        yaw = character->entity.node.rotation.y;
        if (const MotionManagerWork *const manager = MOTM_GetWork(const_cast<Character *>(character))) {
            const auto *const rows = static_cast<const MotionNodeRuntime *>(manager->motion.previousNodes);
            for (const MotionNodeRuntime *row = rows; row && row->nodeId != 0xff; ++row) {
                if (row->nodeId == 4 && row->transform) {
                    root = {
                        row->transform->rows[3].x,
                        row->transform->rows[3].y,
                        row->transform->rows[3].z,
                    };
                    break;
                }
            }
        }
    }

    const auto fixedYaw = static_cast<std::int16_t>(yaw);
    const float sine = sin_fixed16(fixedYaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - fixedYaw));
    worldPoint = {
        root.x + localPoint.x * cosine + localPoint.z * sine,
        root.y + localPoint.y,
        root.z - localPoint.x * sine + localPoint.z * cosine,
    };
}

}
