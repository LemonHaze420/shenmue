#include "s1/motion_internal.hpp"

namespace shenmue::s1::motion_detail {
const std::byte *MOTN_RawRecordAt(const MotnAnimationHeader *header, std::int32_t frame);

void MOTN_ResolveRawFrame(const MotnAnimationHeader *header, std::int32_t frame, const std::byte *&current, const std::byte *&next);

void MOTN_ResolveRawWords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame, std::size_t slot);

void MOTN_ResolveRawRecords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame);

void MOTN_ResolveRawReferenceRecords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame);

void MOTN_ResolveRawPreviousWords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame);

void ResolvePointersForMotion(const MotionInformationWork &motion, MotnRawFrameContext &context);

void MOTN_ResolveRawSecondaryPointers(const MotionInformationWork &motion, MotnRawFrameContext &context);

void MOTN_ReadRawRootPosition(const MotionInformationWork &motion, MotnRawFrameContext &context);

bool MOTN_TestRawRootVisibility(const MotionInformationWork &motion, MotnRawFrameContext &context, std::int8_t stage);

std::int32_t MirrorRawAxis(std::int32_t value, std::uint8_t kind, std::size_t axis);

void MOTN_ApplyTranslationRotation(const MotionInformationWork &motion, const MotnRawFrameContext &context, const MotionNodeRuntime &node);

void MOTI_RotateX();

void MOTN_ApplyRawHeadLook();

std::int32_t RawSingleChannel(const std::int16_t *current, const std::int16_t *next, std::int32_t index, float t);

std::int32_t RawChannel(const std::int16_t *current, const std::int16_t *next, std::int32_t index, float t);

bool IsRawSingleChannelNode(std::uint8_t nodeId);

bool IsRawBindEulerNode(std::uint8_t nodeId);

void SampleRawEuler(const MotionNodeRuntime &row, const MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next,
    std::int32_t channel, std::uint32_t mirror, std::int32_t (&euler)[3]);

void MOTN_EvaluateRawRow(MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next);

void MOTN_EvaluateRawBlendRow(
    MotionNodeRuntime &source, MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next);

bool MOTN_EvaluateRawPose(MotionInformationWork &motion, MotionNodeRuntime &root, std::int8_t stage);

void ApplyBoneTransformWithLerp(
    const MotionInformationWork &motion, const MotionNodeRuntime &blend, MotionNodeRuntime &active, const MotnRawFrameContext &context);

bool MOTN_EvaluateRawBlendPose(MotionInformationWork &motion, MotionNodeRuntime &blend, MotionNodeRuntime &active, std::int8_t stage);

void MOTN_EvaluateRawSecondaryRow(MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next);

// ResolvingMOTNPtrs / MOTN_ResolveRawRecords / MOTN_ResolveRawReferenceRecords / MOTN_ResolveRawPreviousWords: fixed-record frame
// pointers. Records start 0x0e bytes into the motion; each carries three half-float root words followed by the euler words. The frame
// count is the unmasked header word.
const std::byte *MOTN_RawRecordAt(const MotnAnimationHeader *header, std::int32_t frame) {
    return reinterpret_cast<const std::byte *>(header) + 0x0e + static_cast<std::int64_t>(static_cast<std::int16_t>(header->block2Offset) * frame);
}

void MOTN_ResolveRawFrame(const MotnAnimationHeader *header, std::int32_t frame, const std::byte *&current, const std::byte *&next) {
    const std::int32_t count = header->attribute;
    if (frame >= count)
        frame = count - 1;
    current = MOTN_RawRecordAt(header, frame);
    next = MOTN_RawRecordAt(header, count - 1 <= frame ? frame : frame + 1);
}

// ResolvingMOTNPtrs.
void MOTN_ResolveRawWords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame, std::size_t slot) {
    const MotnAnimationHeader *const header = MOTN_GetDataForAnimId(animationId);
    if (!header) {
        context.currentWords[slot] = nullptr;
        context.nextWords[slot] = nullptr;
        return;
    }
    const std::byte *current = nullptr;
    const std::byte *next = nullptr;
    MOTN_ResolveRawFrame(header, frame, current, next);
    context.currentWords[slot] = reinterpret_cast<const std::int16_t *>(current + 6);
    context.nextWords[slot] = reinterpret_cast<const std::int16_t *>(next + 6);
}

// MOTN_ResolveRawRecords.
void MOTN_ResolveRawRecords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame) {
    const MotnAnimationHeader *const header = MOTN_GetDataForAnimId(animationId);
    if (!header) {
        context.currentRecord = nullptr;
        context.nextRecord = nullptr;
        return;
    }
    const std::byte *current = nullptr;
    const std::byte *next = nullptr;
    MOTN_ResolveRawFrame(header, frame, current, next);
    context.currentRecord = reinterpret_cast<const std::int16_t *>(current);
    context.nextRecord = reinterpret_cast<const std::int16_t *>(next);
}

// MOTN_ResolveRawReferenceRecords.
void MOTN_ResolveRawReferenceRecords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame) {
    const MotnAnimationHeader *const header = MOTN_GetDataForAnimId(animationId);
    if (!header) {
        context.referenceRecord = nullptr;
        context.referenceNextRecord = nullptr;
        return;
    }
    const std::byte *current = nullptr;
    const std::byte *next = nullptr;
    MOTN_ResolveRawFrame(header, frame, current, next);
    context.referenceRecord = reinterpret_cast<const std::int16_t *>(current);
    context.referenceNextRecord = reinterpret_cast<const std::int16_t *>(next);
}

// MOTN_ResolveRawPreviousWords: the previous motion's euler words, without clamping the frame.
void MOTN_ResolveRawPreviousWords(MotnRawFrameContext &context, std::uint16_t animationId, std::int32_t frame) {
    const MotnAnimationHeader *const header = MOTN_GetDataForAnimId(animationId);
    context.previousWords = header ? reinterpret_cast<const std::int16_t *>(MOTN_RawRecordAt(header, frame) + 6) : nullptr;
}

// ResolvePointersForMotion.
void ResolvePointersForMotion(const MotionInformationWork &motion, MotnRawFrameContext &context) {
    context.fraction = motion.frameTimer - static_cast<float>(static_cast<std::int32_t>(motion.frameTimer));
    context.referenceFraction = motion.frameTimerRef - static_cast<float>(static_cast<std::int32_t>(motion.frameTimerRef));
    context.secondaryNodeId = motion.secondaryNodeId ? static_cast<std::uint8_t>(motion.secondaryNodeId) : 0xff;
    context.flags = 0;
    MOTN_ResolveRawWords(context, motion.currentAnimId, static_cast<std::int32_t>(motion.frameTimer), 0);
    MOTN_ResolveRawWords(context, motion.secondaryAnimId, static_cast<std::int32_t>(motion.secondaryFrameTimer), 1);
    MOTN_ResolveRawRecords(context, motion.currentAnimId, static_cast<std::int32_t>(motion.frameTimer));
    MOTN_ResolveRawReferenceRecords(context, motion.currentAnimId, static_cast<std::int32_t>(motion.frameTimerRef));
    MOTN_ResolveRawPreviousWords(context, motion.requestedAnimId, static_cast<std::int32_t>(motion.previousFrameTimer));
    context.entityYaw = motion.entityEuler.y;
}

// MOTN_ResolveRawSecondaryPointers: the secondary motion's records, referenced to its first frame.
void MOTN_ResolveRawSecondaryPointers(const MotionInformationWork &motion, MotnRawFrameContext &context) {
    const float frame = motion.secondaryFrameTimer;
    context.referenceFraction = 0.0f;
    context.flags = 0;
    context.fraction = frame - static_cast<float>(static_cast<std::int32_t>(frame));
    MOTN_ResolveRawWords(context, motion.secondaryAnimId, static_cast<std::int32_t>(motion.secondaryFrameTimer), 0);
    MOTN_ResolveRawWords(context, motion.secondaryAnimId, static_cast<std::int32_t>(motion.secondaryFrameTimer), 1);
    MOTN_ResolveRawRecords(context, motion.secondaryAnimId, static_cast<std::int32_t>(motion.secondaryFrameTimer));
    MOTN_ResolveRawReferenceRecords(context, motion.secondaryAnimId, 0);
    MOTN_ResolveRawPreviousWords(context, motion.secondaryAnimId, 0);
    context.entityYaw = 0;
}

// Motion_ConvertF16toF32 (MOTN_ReadRawRootPosition): the root half-floats at the frame, less the reference frame's X/Z.
void MOTN_ReadRawRootPosition(const MotionInformationWork &motion, MotnRawFrameContext &context) {
    const std::int16_t *const current = context.currentRecord;
    const std::int16_t *const next = context.nextRecord;
    const float t = context.fraction;
    const float x = (F16ToF32(next[0]) - F16ToF32(current[0])) * t + F16ToF32(current[0]);
    const float y = (F16ToF32(next[1]) - F16ToF32(current[1])) * t + F16ToF32(current[1]);
    const float z = (F16ToF32(next[2]) - F16ToF32(current[2])) * t + F16ToF32(current[2]);
    float referenceX = 0.0f;
    float referenceZ = 0.0f;
    if (motion.frameTimerRef >= 0.0f) {
        const std::int16_t *const reference = context.referenceRecord;
        const std::int16_t *const referenceNext = context.referenceNextRecord;
        referenceX = (F16ToF32(referenceNext[0]) - F16ToF32(reference[0])) * context.referenceFraction + F16ToF32(reference[0]);
        referenceZ = (F16ToF32(referenceNext[2]) - F16ToF32(reference[2])) * context.referenceFraction + F16ToF32(reference[2]);
    }
    const float offsetX = x - referenceX;
    context.rootPosition = {offsetX, y, z - referenceZ};
    if (g_motnMirror)
        context.rootPosition.x = -offsetX;
    if ((motion.motionFlags & 8U) != 0) {
        context.rootPosition.x = 0.0f;
        context.rootPosition.z = 0.0f;
    }
}

// au_re_Vector3_TransformLocalToWorld_1 (MOTN_TestRawRootVisibility): the view-cone test of the placed root; returns true when an
// off-screen root aborts the evaluation (stage >= 2).
bool MOTN_TestRawRootVisibility(const MotionInformationWork &motion, MotnRawFrameContext &context, std::int8_t stage) {
    const Vector3 root{motion.rootPreTranslate.x + context.rootPosition.x, motion.rootPreTranslate.y + context.rootPosition.y,
        motion.rootPreTranslate.z + context.rootPosition.z};
    Vector3 view{};
    Matrix4x4::TransformPoint(root, view);
    bool visible = false;
    const float depth = view.z * -1.0f + g_motnClipAngle;
    if (depth >= 0.0f) {
        const Vector2 &scale = prevScreenSpaceToWorldScale;
        const float halfWidth = depth * scale.x + g_motnClipAngle;
        if (view.x <= halfWidth && -halfWidth < view.x) {
            const float halfHeight = depth * scale.y + g_motnClipAngle;
            if (view.y <= halfHeight)
                visible = -halfHeight < view.y;
        }
    }
    context.visible = visible ? 1 : 0;
    if (visible) {
        context.flags &= ~2U;
        return false;
    }
    context.flags |= 2U;
    return stage >= 2;
}

std::int32_t MirrorRawAxis(std::int32_t value, std::uint8_t kind, std::size_t axis) {
    return static_cast<std::int32_t>(
        static_cast<float>(value) * VectorAxis(kBoneDirections[kind * 2U], axis) - VectorAxis(kBoneDirections[kind * 2U + 1U], axis) * 65536.0f);
}

// MOTN_ApplyTranslationRotation: places the fixed-record root at the entity position and yaw, rotated by its frame euler.
void MOTN_ApplyTranslationRotation(const MotionInformationWork &motion, const MotnRawFrameContext &context, const MotionNodeRuntime &node) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(motion.rootPreTranslate.x, motion.rootPreTranslate.y, motion.rootPreTranslate.z);
    const auto yaw = static_cast<std::uint16_t>(motion.entityEuler.y);
    const std::uint32_t folded = yaw <= 0x8000U ? yaw : 0x10000U - yaw;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded));
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), cosine);
    Matrix4x4::preTranslate(context.rootPosition.x, context.rootPosition.y, context.rootPosition.z);
    const std::int16_t *const euler = context.currentWords[0];
    if (g_motnMirror) {
        Matrix4x4::SetViewMatrixFromEuler(
            MirrorRawAxis(euler[1], node.nodeKind, 1), MirrorRawAxis(euler[0], node.nodeKind, 0), MirrorRawAxis(euler[2], node.nodeKind, 2));
        return;
    }
    Matrix4x4::SetViewMatrixFromEuler(euler[1], euler[0], euler[2]);
}

// MOTI_RotateX: the upper-body half turn of the fixed-record chest.
void MOTI_RotateX() {
    const std::uint32_t flags = g_pMotion->rootAngleFlags;
    if ((flags & 0x80U) == 0 && (flags & 0x20000U) != 0 && g_pMotion->currentTurnAngle)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-(g_pMotion->currentTurnAngle >> 1)));
}

// MOTN_ApplyRawHeadLook: the fixed-record head look. Turns the evaluated head matrix toward the look target, ramping
// yaw and pitch through the same head-angle state PerformMatrixTransformationAndAnimationUpdate drives.
void MOTN_ApplyRawHeadLook() {
    MotionInformationWork &motion = *g_pMotion;
    motion.rootAngleFlags &= ~0x1cU;
    const std::uint32_t initialFlags = motion.rootAngleFlags;
    Vector3 target = motion.rootOffsetPosition;
    const Vector3 *lookTarget = nullptr;
    std::memcpy(&lookTarget, motion.reserved368, sizeof(lookTarget));
    if ((initialFlags & 0x40U) != 0 || !lookTarget)
        motion.rootAngleFlags = initialFlags & ~3U;
    else
        target = *lookTarget;
    const Matrix4x4 &skin = g_skinningTransformMatrix;
    const Vector3 localTarget = LocalDirectionTo(skin, target);
    const Vector3 localRoot = LocalDirectionTo(skin, motion.rootOffsetPosition);
    const Matrix4x4 head = workTransformMatrix;
    const std::uint32_t command = motion.headLookAngleCommand;
    const bool extended = (command & 0x8000U) != 0 && (command & 0x7fffU) >= 0x65U;
    std::int16_t yaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-localTarget.z, -localTarget.y));
    if (extended) {
        if ((motion.motionFlags & 0x400000U) != 0)
            yaw = static_cast<std::int16_t>(yaw + motion.currentTurnAngle - motion.targetTurnAngle);
        motion.headLookTimer = static_cast<std::int16_t>(2 * motion.currentTurnAngle);
    } else if (std::fabs(static_cast<float>(yaw)) > 28672.0f) {
        motion.rootAngleFlags |= 0x10U;
    }
    const float horizontal = std::sqrt(localTarget.z * localTarget.z + localTarget.y * localTarget.y);
    std::int16_t pitch = static_cast<std::int16_t>(-static_cast<std::int16_t>(Shenmue_Atan2Fixed(localTarget.x, horizontal)));
    if ((motion.motionFlags & 0x4000U) != 0)
        MOTN_ClampAnimationAngles(motion, yaw, pitch);
    if (command == 0x8065U) {
        motion.rootAngleFlags |= 0x2002U;
    } else if (extended) {
        auto *const blend = reinterpret_cast<std::int16_t *>(&motion.lookAtBlendTimer);
        std::int16_t yawOut = 0;
        motnUpdate(motion, {&blend[1], &motion.lookAtState, &blend[0], &motion.headLookPreviousTimer, &motion.headLookAngleBase}, yaw, yawOut, 1);
        auto *const hold = reinterpret_cast<std::int16_t *>(&motion.lookAtHoldTimer);
        std::int16_t pitchOut = 0;
        motnUpdate(motion, {&hold[1], reinterpret_cast<std::int16_t *>(&motion.lookAtFlags), &hold[0], &motion.lookAtModeBackup, &motion.lookAtMode}, pitch,
            pitchOut, 2);
        yaw = yawOut;
        pitch = pitchOut;
    } else {
        const auto step = static_cast<std::int32_t>(command);
        const std::int32_t pitchDifference = static_cast<std::int32_t>(pitch) - motion.lookAtMode;
        if (static_cast<float>(step >> 1) < std::fabs(static_cast<float>(pitchDifference)))
            pitch = static_cast<std::int16_t>(pitchDifference >= 0 ? (step >> 1) + motion.lookAtMode : motion.lookAtMode - (step >> 1));
        else
            motion.rootAngleFlags |= 2U;
        const std::int32_t yawDifference = static_cast<std::int32_t>(yaw) - motion.headLookAngleBase;
        if (static_cast<float>(step) < std::fabs(static_cast<float>(yawDifference))) {
            yaw = static_cast<std::int16_t>(yawDifference >= 0 ? step + motion.headLookAngleBase : motion.headLookAngleBase - step);
            motion.headLookTimer = 0;
        } else {
            motion.rootAngleFlags |= 1U;
            motion.headLookAngleBase = yaw;
            motion.headLookTimer = 0;
        }
    }
    const auto rootYaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-localRoot.z, -localRoot.y));
    motion.headLookAngleBase = yaw;
    motion.lookAtMode = pitch;
    if ((motion.motionFlags & 0x4000U) != 0) {
        workTransformMatrix = skin;
        const auto yawTurn = static_cast<std::uint16_t>(static_cast<std::uint16_t>(yaw) + 0x8000U);
        const std::uint32_t yawMagnitude = yawTurn <= 0x8000U ? yawTurn : static_cast<std::uint16_t>(-yawTurn);
        const float yawCosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - yawMagnitude));
        Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(yawTurn)), yawCosine);
        const auto pitchTurn = static_cast<std::uint16_t>(pitch);
        const std::uint32_t pitchMagnitude = pitchTurn <= 0x8000U ? pitchTurn : static_cast<std::uint16_t>(-pitchTurn);
        const float pitchCosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - pitchMagnitude));
        Matrix4x4::RotateAroundZ(sin_fixed16(pitch), pitchCosine);
        Matrix4x4::preTranslate(0.0f, 0.5f, 0.0f);
        const Vector3 aim{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = head;
        Matrix4x4::RotateZY(WorkLocalPoint(aim));
        if (!extended && (motion.motionFlags & 0x400000U) != 0)
            motion.currentTurnAngle = static_cast<std::int16_t>((static_cast<std::int32_t>(motion.headLookAngleBase) - rootYaw) >> 1);
    }
    if ((motion.rootAngleFlags & 0x2043U) == 0x2043U) {
        motion.motionFlags &= ~0xc00000U;
        motion.headLookAngleCommand = 2048U;
        motion.rootAngleFlags &= ~0x220040U;
    }
}

std::int32_t RawSingleChannel(const std::int16_t *current, const std::int16_t *next, std::int32_t index, float t) {
    const std::int32_t start = current[index];
    return start + static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(next[index]) - start) * t));
}

std::int32_t RawChannel(const std::int16_t *current, const std::int16_t *next, std::int32_t index, float t) {
    const auto delta = static_cast<std::int16_t>(static_cast<std::uint16_t>(next[index]) - static_cast<std::uint16_t>(current[index]));
    return static_cast<std::int32_t>(static_cast<float>(delta) * t + static_cast<float>(current[index]));
}

bool IsRawSingleChannelNode(std::uint8_t nodeId) {
    return nodeId <= 41U && ((0x20102010400ULL >> nodeId) & 1U) != 0;
}

bool IsRawBindEulerNode(std::uint8_t nodeId) {
    return nodeId <= 37U && ((0x2020400000ULL >> nodeId) & 1U) != 0;
}

// Samples a row's frame euler (the three channel words, or the single Z word) and applies the mirror remap.
void SampleRawEuler(const MotionNodeRuntime &row, const MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next,
    std::int32_t channel, std::uint32_t mirror, std::int32_t (&euler)[3]) {
    const float t = context.fraction;
    if (IsRawSingleChannelNode(row.nodeId)) {
        euler[0] = 0;
        euler[1] = 0;
        euler[2] = RawSingleChannel(current, next, channel, t);
    } else {
        if (row.nodeId == 36)
            g_motnRawIKFlag = 1;
        euler[2] = RawChannel(current, next, channel + 2, t);
        euler[1] = RawChannel(current, next, channel + 1, t);
        euler[0] = RawChannel(current, next, channel, t);
    }
    if (!mirror || row.nodeKind == 4)
        return;
    if (row.nodeId == 4) {
        euler[0] = static_cast<std::int32_t>(static_cast<float>(euler[0]) * -1.0f - 32768.0f);
        euler[1] = static_cast<std::int32_t>(static_cast<float>(euler[1]) * -1.0f);
        euler[2] = static_cast<std::int32_t>(static_cast<float>(euler[2]));
        return;
    }
    euler[0] = MirrorRawAxis(euler[0], row.nodeKind, 0);
    euler[1] = MirrorRawAxis(euler[1], row.nodeKind, 1);
    euler[2] = MirrorRawAxis(euler[2], row.nodeKind, 2);
}

// EvaluateBoneHierarchyAndTransformNode (MOTN_EvaluateRawRow): places one fixed-record row from its frame euler words.
void MOTN_EvaluateRawRow(MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next) {
    MotionInformationWork &motion = *g_pMotion;
    std::uint32_t mirror = g_motnMirror;
    const std::int16_t secondaryNodeId = motion.secondaryNodeId;
    if (static_cast<std::int8_t>(row.nodeId) == secondaryNodeId && secondaryNodeId) {
        auto *const source = static_cast<MotionNodeRuntime *>(motion.sourceNodes);
        const float amount = source->interpolationAmount;
        if (0x1p-23f > amount && !MOTN_GetAnimationFlag(motion.secondaryAnimId)) {
            MOTN_ApplyBoneHierarchyTransform(row, row);
            return;
        }
        if (amount != 0.0f) {
            MOTN_EvaluateRawBlendRow(*source, row, context, current, next);
            return;
        }
        current = context.currentWords[1];
        next = context.nextWords[1];
        mirror = (motion.motionFlags >> 20U) & 1U;
        g_motnMirror = mirror;
    }
    row.effectorPosition.x = row.bindPosition->x;
    row.effectorPosition.y = row.bindPosition->y;
    row.effectorPosition.z = row.bindPosition->z;
    row.bindEuler = *row.bindEulerSource;
    if ((row.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4_Push();
    Matrix4x4::preTranslate(row.bindPosition->x, row.bindPosition->y, row.bindPosition->z);
    if (row.flags) {
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        if (g_motnEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
    }
    const auto channel = static_cast<std::int8_t>(mirror ? row.bindPositionPacked[0] : row.reserved07);
    if (!channel) {
        if (IsRawBindEulerNode(row.nodeId)) {
            Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
        } else if (row.nodeId == 20) {
            Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
            if (!g_motnRawIKFlag)
                MOTI_RotateX();
        }
    } else {
        std::int32_t euler[3]{};
        SampleRawEuler(row, context, current, next, channel, mirror, euler);
        row.bindEuler = {static_cast<std::int16_t>(euler[0]), static_cast<std::int16_t>(euler[1]), static_cast<std::int16_t>(euler[2])};
        if (row.nodeId == 4 && !g_motnRawIKFlag) {
            g_skinningTransformMatrix = workTransformMatrix;
            Matrix4x4_Push();
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-motion.currentTurnAngle));
            Matrix4x4::SetViewMatrixFromEuler(euler[1], euler[0], euler[2]);
            Matrix4x4::preTranslate(0.5f, 0.0f, 0.0f);
            motion.rootOffsetPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4::Pop(1);
            if (motion.blendScratch[3] == 0.0f) {
                Matrix4x4::SetViewMatrixFromEuler(euler[1], euler[0], euler[2]);
            } else {
                Matrix4x4_Push();
                workTransformMatrix = transformMatrix_prePass;
                Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
                g_motnRawLookEnd = workTransformMatrix;
                workTransformMatrix = transformMatrix_prePass;
                Matrix4x4::SetViewMatrixFromEuler(euler[1], euler[0], euler[2]);
                g_motnBlendEnd = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::Multiply(
                    Matrix4x4_Lerp(g_motnRawLookBlend, motion.blendScratch[3], g_motnBlendEnd, g_motnRawLookEnd) ? &g_motnRawLookBlend : &g_motnRawLookEnd);
            }
            MOTN_ApplyRawHeadLook();
        } else {
            Matrix4x4::SetViewMatrixFromEuler(euler[1], euler[0], euler[2]);
            if (row.nodeId == 1 && !g_motnRawIKFlag)
                MOTN_RotateX();
        }
    }
    *MotionRowMatrix(row) = workTransformMatrix;
    row.bindPositionPacked[1] |= 2U;
    if (row.nodeId == 2 && context.visible == 1 && (context.flags & 2U) != 0) {
        if ((row.bindPositionPacked[1] & 1U) != 0)
            Matrix4x4::Pop(1);
        return;
    }
    for (std::uint8_t child = 0; child < row.childCount; ++child)
        MOTN_EvaluateRawRow(*row.children[child], context, current, next);
    if ((row.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
    g_motnMirror = (g_pMotion->motionFlags >> 17U) & 1U;
}

// EvaluateNodeEulerAndBlendPose (MOTN_EvaluateRawBlendRow): the fixed-record row blended from the source row's euler.
void MOTN_EvaluateRawBlendRow(
    MotionNodeRuntime &source, MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next) {
    MotionInformationWork &motion = *g_pMotion;
    MotionNodeRuntime *blendSource = &source;
    std::uint32_t mirror = g_motnMirror;
    const std::int16_t secondaryNodeId = motion.secondaryNodeId;
    if (static_cast<std::int8_t>(source.nodeId) == secondaryNodeId && secondaryNodeId) {
        blendSource = static_cast<MotionNodeRuntime *>(motion.sourceNodes);
        const float amount = blendSource->interpolationAmount;
        if (0x1p-23f > amount) {
            if (!MOTN_GetAnimationFlag(motion.secondaryAnimId)) {
                MOTN_ApplyBoneHierarchyTransform(row, *blendSource);
                return;
            }
            MOTN_EvaluateRawRow(*blendSource, context, current, next);
            return;
        }
        mirror = (motion.motionFlags >> 17U) & 1U;
        g_motnMirror = mirror;
    }
    const float t = blendSource->interpolationAmount;
    row.effectorPosition.x = row.bindPosition->x;
    row.effectorPosition.y = row.bindPosition->y;
    row.effectorPosition.z = row.bindPosition->z;
    row.bindEuler = *row.bindEulerSource;
    if ((row.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4_Push();
    Matrix4x4::preTranslate(row.bindPosition->x, row.bindPosition->y, row.bindPosition->z);
    if (g_motnRawParentUnrotated) {
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
    }
    const auto channel = static_cast<std::int8_t>(mirror ? row.bindPositionPacked[0] : row.reserved07);
    if (!channel) {
        if (IsRawBindEulerNode(row.nodeId)) {
            Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
        } else if (row.nodeId == 20) {
            Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
            if (!g_motnRawIKFlag)
                MOTI_RotateX();
        }
    } else {
        std::int32_t euler[3]{};
        SampleRawEuler(row, context, current, next, channel, mirror, euler);
        row.bindEuler = {static_cast<std::int16_t>(euler[0]), static_cast<std::int16_t>(euler[1]), static_cast<std::int16_t>(euler[2])};
        Matrix4x4_Push();
        const std::uint32_t unrotated = g_motnRawParentUnrotated;
        workTransformMatrix = transformMatrix_prePass;
        if (unrotated && g_motnEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnEntityEuler->y, g_motnEntityEuler->x, g_motnEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(row.bindEuler.y, row.bindEuler.x, row.bindEuler.z);
        g_motnBlendEnd = workTransformMatrix;
        workTransformMatrix = transformMatrix_prePass;
        if (unrotated && g_motnNextEntityEuler)
            Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
        Matrix4x4::SetViewMatrixFromEuler(blendSource->bindEuler.y, blendSource->bindEuler.x, blendSource->bindEuler.z);
        g_motnBlendStart = workTransformMatrix;
        Matrix4x4::Pop(1);
        Matrix4x4 blended{};
        if (row.nodeId != 4) {
            Matrix4x4::Multiply(Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
            if (row.nodeId == 1 && !g_motnRawIKFlag)
                MOTN_RotateX();
        } else {
            g_skinningTransformMatrix = workTransformMatrix;
            if (!g_motnRawIKFlag) {
                Matrix4x4_Push();
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-motion.currentTurnAngle));
                Matrix4x4::Multiply(Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
                Matrix4x4::preTranslate(0.5f, 0.0f, 0.0f);
                motion.rootOffsetPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
                Matrix4x4::Pop(1);
            }
            if (motion.blendScratch[3] == 0.0f) {
                Matrix4x4::Multiply(Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
            } else {
                if (Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd))
                    g_motnBlendEnd = blended;
                Matrix4x4_Push();
                workTransformMatrix = transformMatrix_prePass;
                Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
                g_motnBlendStart = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::Multiply(Matrix4x4_Lerp(blended, motion.blendScratch[3], g_motnBlendEnd, g_motnBlendStart) ? &blended : &g_motnBlendStart);
            }
            if (!g_motnRawIKFlag)
                MOTN_ApplyRawHeadLook();
        }
    }
    *MotionRowMatrix(row) = workTransformMatrix;
    row.bindPositionPacked[1] |= 2U;
    if (row.nodeId == 2 && context.visible == 1 && (context.flags & 2U) != 0) {
        if ((row.bindPositionPacked[1] & 1U) != 0)
            Matrix4x4::Pop(1);
        return;
    }
    g_motnRawParentUnrotated = row.flags != 0 ? 1U : 0U;
    for (std::uint8_t child = 0; child < row.childCount; ++child)
        MOTN_EvaluateRawBlendRow(*blendSource->children[child], *row.children[child], context, current, next);
    if ((row.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
}

// ApplyMotionTransformAndDispatchChildren (MOTN_EvaluateRawPose): the unblended fixed-record pose.
bool MOTN_EvaluateRawPose(MotionInformationWork &motion, MotionNodeRuntime &root, std::int8_t stage) {
    g_motnRawIKFlag = 0;
    g_motnMirror = (g_pMotion->motionFlags >> 17U) & 1U;
    MotnRawFrameContext context{};
    ResolvePointersForMotion(motion, context);
    MOTN_ReadRawRootPosition(motion, context);
    const float scale = root.timeScale;
    context.rootPosition = {context.rootPosition.x * scale, context.rootPosition.y * scale, context.rootPosition.z * scale};
    root.effectorPosition.x = context.rootPosition.x;
    root.effectorPosition.y = context.rootPosition.y;
    root.effectorPosition.z = context.rootPosition.z;
    std::memcpy(&root.bindEuler, context.currentWords[0], sizeof(root.bindEuler));
    if (MOTN_TestRawRootVisibility(motion, context, stage))
        return context.visible != 0;
    Matrix4x4_Push();
    MOTN_ApplyTranslationRotation(motion, context, root);
    root.effectorPosition.x = workTransformMatrix.rows[3].x;
    root.effectorPosition.y = workTransformMatrix.rows[3].y;
    root.effectorPosition.z = workTransformMatrix.rows[3].z;
    *MotionRowMatrix(root) = workTransformMatrix;
    root.bindPositionPacked[1] |= 2U;
    for (std::uint8_t child = stage == 1 && (context.flags & 2U) != 0 ? 1 : 0; child < root.childCount; ++child)
        MOTN_EvaluateRawRow(*root.children[child], context, context.currentWords[0], context.nextWords[0]);
    Matrix4x4::Pop(1);
    return context.visible != 0;
}

// ApplyBoneTransformWithLerp: the blended fixed-record root.
void ApplyBoneTransformWithLerp(
    const MotionInformationWork &motion, const MotionNodeRuntime &blend, MotionNodeRuntime &active, const MotnRawFrameContext &context) {
    const float t = blend.interpolationAmount;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(motion.rootPreTranslate.x, motion.rootPreTranslate.y, motion.rootPreTranslate.z);
    const Vector3i *const entityEuler = g_motnEntityEuler;
    if (entityEuler)
        Matrix4x4::SetViewMatrixFromEuler(entityEuler->y, entityEuler->x, entityEuler->z);
    Matrix4x4::preTranslate(context.rootPosition.x, context.rootPosition.y, context.rootPosition.z);
    active.effectorPosition.x = workTransformMatrix.rows[3].x;
    active.effectorPosition.y = workTransformMatrix.rows[3].y;
    active.effectorPosition.z = workTransformMatrix.rows[3].z;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(active.effectorPosition.x, (1.0f - t) * blend.effectorPosition.y + t * active.effectorPosition.y, active.effectorPosition.z);
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    if (g_motnNextEntityEuler)
        Matrix4x4::SetViewMatrixFromEuler(g_motnNextEntityEuler->y, g_motnNextEntityEuler->x, g_motnNextEntityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(blend.bindEuler.y, blend.bindEuler.x, blend.bindEuler.z);
    g_motnBlendStart = workTransformMatrix;
    workTransformMatrix = transformMatrix_prePass;
    if (entityEuler)
        Matrix4x4::SetViewMatrixFromEuler(entityEuler->y, entityEuler->x, entityEuler->z);
    Matrix4x4::SetViewMatrixFromEuler(active.bindEuler.y, active.bindEuler.x, active.bindEuler.z);
    g_motnBlendEnd = workTransformMatrix;
    Matrix4x4::Pop(1);
    Matrix4x4 blended{};
    Matrix4x4::Multiply(Matrix4x4_Lerp(blended, t, g_motnBlendStart, g_motnBlendEnd) ? &blended : &g_motnBlendEnd);
    *MotionRowMatrix(active) = workTransformMatrix;
}

// EvaluateAndTransformBone (MOTN_EvaluateRawBlendPose): the fixed-record pose blended from the previous rows.
bool MOTN_EvaluateRawBlendPose(MotionInformationWork &motion, MotionNodeRuntime &blend, MotionNodeRuntime &active, std::int8_t stage) {
    g_motnRawParentUnrotated = 0;
    g_motnRawIKFlag = 0;
    g_motnMirror = (g_pMotion->motionFlags >> 17U) & 1U;
    const std::uint32_t mirror = g_motnMirror;
    MotnRawFrameContext context{};
    ResolvePointersForMotion(motion, context);
    MOTN_ReadRawRootPosition(motion, context);
    const float scale = active.timeScale;
    context.rootPosition = {context.rootPosition.x * scale, context.rootPosition.y * scale, context.rootPosition.z * scale};
    active.effectorPosition.x = context.rootPosition.x;
    active.effectorPosition.y = context.rootPosition.y;
    active.effectorPosition.z = context.rootPosition.z;
    const std::int16_t *const euler = context.currentWords[0];
    if (mirror) {
        active.bindEuler = {static_cast<std::int16_t>(MirrorRawAxis(euler[0], active.nodeKind, 0)),
            static_cast<std::int16_t>(MirrorRawAxis(euler[1], active.nodeKind, 1)), static_cast<std::int16_t>(MirrorRawAxis(euler[2], active.nodeKind, 2))};
    } else {
        active.bindEuler = {euler[0], euler[1], euler[2]};
    }
    if (MOTN_TestRawRootVisibility(motion, context, stage))
        return context.visible != 0;
    Matrix4x4_Push();
    ApplyBoneTransformWithLerp(motion, blend, active, context);
    active.bindPositionPacked[1] |= 2U;
    for (std::uint8_t child = stage == 1 && (context.flags & 2U) != 0 ? 1 : 0; child < blend.childCount; ++child)
        MOTN_EvaluateRawBlendRow(*blend.children[child], *active.children[child], context, context.currentWords[0], context.nextWords[0]);
    Matrix4x4::Pop(1);
    return context.visible != 0;
}

// EvaluateMotionFrameAndDispatch (MOTN_EvaluateRawMotion): the fixed-record evaluator mainMotionUpdate runs instead of the mode switch.
bool MOTN_EvaluateRawMotion(MotionInformationWork &motion) {
    bool visible = false;
    auto *const blendRows = static_cast<MotionNodeRuntime *>(motion.blendTargetNodes);
    if ((motion.motionFlags & 0x10000U) == 0 || blendRows->nodeId == 0xff) {
        motion.motionFlags &= ~0x10000U;
        for (MotionNodeRuntime *row = blendRows; row && row->nodeId != 0xff; ++row)
            row->interpolationAmount = 0.0f;
        g_motnEntityEuler = &motion.entityEuler;
        visible = MOTN_EvaluateRawPose(motion, *static_cast<MotionNodeRuntime *>(motion.activeNodes), static_cast<std::int8_t>(motion.poseEvaluationStage));
        motion.interpolationMode = 0;
    } else {
        if (motion.blendFramesRemaining)
            --motion.blendFramesRemaining;
        const float ramp = ComputeLinearRamp01(motion.blendFramesTotal - motion.blendFramesRemaining, motion.blendFramesTotal);
        for (MotionNodeRuntime *row = blendRows; row && row->nodeId != 0xff; ++row)
            row->interpolationAmount = ramp;
        if (!motion.blendFramesRemaining)
            motion.motionFlags &= ~0x10000U;
        g_motnNextEntityEuler = &motion.nextEntityEuler;
        g_motnEntityEuler = &motion.entityEuler;
        visible = MOTN_EvaluateRawBlendPose(motion, *static_cast<MotionNodeRuntime *>(motion.blendTargetNodes),
            *static_cast<MotionNodeRuntime *>(motion.activeNodes), static_cast<std::int8_t>(motion.poseEvaluationStage));
        motion.interpolationMode = 1;
    }
    if (MaintainFrameRate(motion.frameTimer, motion.frameTimerRef, motion.residualFrameTime, motion.previousAnimId == 0xffffU,
            (motion.motionFlags & 0x20U) != 0, false, motion.frameStart, motion.frameEnd, MOTN_GetSmoothedFrameDelta(motion.playbackRate)))
        motion.motionFlags |= 2U;
    return visible;
}

// MOTN_EvaluateRawSecondaryRow.
void MOTN_EvaluateRawSecondaryRow(MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next) {
    if (row.nodeId == g_pMotion->secondaryNodeId) {
        g_motnSecondaryAnchor = workTransformMatrix;
        g_pMotion->sourceNodes = &row;
    }
    row.effectorPosition.x = row.bindPosition->x;
    row.effectorPosition.y = row.bindPosition->y;
    row.effectorPosition.z = row.bindPosition->z;
    row.bindEuler = *row.bindEulerSource;
    if ((row.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4_Push();
    Matrix4x4::preTranslate(row.bindPosition->x, row.bindPosition->y, row.bindPosition->z);
    if (row.flags) {
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
    }
    const std::uint32_t mirror = g_motnMirror;
    const auto channel = static_cast<std::int8_t>(mirror ? row.bindPositionPacked[0] : row.reserved07);
    if (channel) {
        std::int32_t euler[3]{};
        const float t = context.fraction;
        if (IsRawSingleChannelNode(row.nodeId)) {
            euler[2] = RawSingleChannel(current, next, channel, t);
        } else {
            euler[2] = RawChannel(current, next, channel + 2, t);
            euler[1] = RawChannel(current, next, channel + 1, t);
            euler[0] = RawChannel(current, next, channel, t);
        }
        if (mirror && row.nodeKind != 4) {
            if (row.nodeId == 4) {
                euler[0] = static_cast<std::int32_t>(static_cast<float>(euler[0]) * -1.0f - 32768.0f);
                euler[1] = static_cast<std::int32_t>(static_cast<float>(euler[1]) * -1.0f);
            } else {
                euler[0] = MirrorRawAxis(euler[0], row.nodeKind, 0);
                euler[1] = MirrorRawAxis(euler[1], row.nodeKind, 1);
                euler[2] = MirrorRawAxis(euler[2], row.nodeKind, 2);
            }
        }
        row.bindEuler = {static_cast<std::int16_t>(euler[0]), static_cast<std::int16_t>(euler[1]), static_cast<std::int16_t>(euler[2])};
        Matrix4x4::SetViewMatrixFromEuler(euler[1], euler[0], euler[2]);
    } else if (row.nodeId <= 37U && ((0x2020500000ULL >> row.nodeId) & 1U) != 0) {
        Matrix4x4::SetViewMatrixFromEuler(row.bindEulerSource->y, row.bindEulerSource->x, row.bindEulerSource->z);
    }
    *MotionRowMatrix(row) = workTransformMatrix;
    row.bindPositionPacked[1] |= 2U;
    for (std::uint8_t child = 0; child < row.childCount; ++child)
        MOTN_EvaluateRawSecondaryRow(*row.children[child], context, current, next);
    if ((row.bindPositionPacked[1] & 1U) != 0)
        Matrix4x4::Pop(1);
}

// MOTN_UpdateRawSecondaryPose: the fixed-record secondary overlay pose.
void MOTN_UpdateRawSecondaryPose(MotionInformationWork &motion, MotionNodeRuntime &rows) {
    g_motnMirror = (g_pMotion->motionFlags >> 20U) & 1U;
    MotnRawFrameContext context{};
    MOTN_ResolveRawSecondaryPointers(motion, context);
    MOTN_ReadRawRootPosition(motion, context);
    rows.effectorPosition.x = context.rootPosition.x;
    rows.effectorPosition.z = context.rootPosition.z;
    rows.effectorPosition.y = context.rootPosition.y;
    std::memcpy(&rows.bindEuler, context.currentWords[0], sizeof(rows.bindEuler));
    MOTN_ApplyTranslationRotation(motion, context, rows);
    *MotionRowMatrix(rows) = workTransformMatrix;
    rows.bindPositionPacked[1] |= 2U;
    for (std::uint8_t child = 0; child < rows.childCount; ++child)
        MOTN_EvaluateRawSecondaryRow(*rows.children[child], context, context.currentWords[0], context.nextWords[0]);
}
}
