#include "s2/motion_work.hpp"

#include "s2/camera.hpp"
#include "s2/fight.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/hand.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"

#include <cmath>
#include <cstddef>
#include <cstring>
#include <span>

namespace shenmue::s2 {

namespace {

constexpr float motionFrameSeconds = 0.033333335f;

float RigAngleFloat(float radians) {
    return (radians * 57.29578f) * 182.04443f;
}

void ApplyRotationChannelsZYX(MotionRigChannel *const (&channels)[3]) {
    Matrix4x4::RotateAroundZ_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*channels[0]))));
    Matrix4x4::RotateAroundY_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*channels[1]))));
    Matrix4x4::RotateAroundX_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*channels[2]))));
}

// The callbacks' "copy channels or slerp toward the channel pose" body.
void EvaluateRotationParameters(MotionRotationParameters &parameters, std::uint32_t poseMode, float blend) {
    if (std::fabs(blend - 1.0f) <= 0.00000011920929f) {
        if (poseMode == motionPoseFromChannels)
            ApplyRotationChannelsZYX(parameters.channels);
        else if (poseMode == motionPoseFromAxisAngle)
            AxisAngleToBasisSlerp(*parameters.basis);
    } else {
        cbEvaluateTargetRotOverride(&parameters);
    }
}

void CaptureBasisTarget(MotionRotationBasis &basis, const Matrix4x4 &transform) {
    basis.targetAxis = {transform.rows[0].x, transform.rows[1].x, transform.rows[2].x};
    basis.targetTangent = {transform.rows[0].y, transform.rows[1].y, transform.rows[2].y};
}

// Slerps from the current blend source toward the captured target by blend * duration.
void SlerpToCapturedTarget(MotionRotationBasis &basis, float blend) {
    const float saved = g_motionCurrentAnimTime;
    g_motionCurrentAnimTime = g_motionMaximumAnimTime * blend;
    AxisAngleToBasisSlerp(basis);
    g_motionCurrentAnimTime = saved;
}

// Runs a target callback in world space: the callback sees the solver's output after
// the current frame, then the result returns through the inverted frame.
void InvokeTargetCallback(MotionTargetParameters &target, MotionWork &work, Vector3 *output, std::uint32_t controller) {
    Matrix4x4_Push();
    Matrix4x4::TransformPoint(*output, *output);
    target.callback(controller, output, &work, target.user);
    InvertModelWorkTransform();
    Matrix4x4::TransformPoint(*output, *output);
    Matrix4x4::Pop(1);
}

float BlendForController(const MotionWork &work, std::uint32_t controller) {
    float blend = controller <= 4 ? work.runtime.layerBlendAlpha[controller] : work.runtime.layerBlendAlpha[0];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    return blend;
}

void SampleBlendedTarget(MotionTargetParameters &target, float blend, Vector3 &sampled) {
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const float value = SampleMotionRigChannelAdvance(*target.channels[axis]);
        sampled[axis] = value;
        (*target.output)[axis] = target.pva[axis].w * (1.0f - blend) + value * blend;
    }
}

} // namespace

MotionRigClock *g_motionRegionClocks[5]{};
std::uint8_t g_motionLayerEvaluated[8]{};
std::int32_t g_ikEnabled{};

// @verified - p
void cbEvaluateTargetRotOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionRotationBasis &basis = *rotation.basis;
    float blend = *rotation.blendAlpha;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    Matrix4x4_PushActiveTransform();
    ApplyRotationChannelsZYX(rotation.channels);
    Matrix4x4 transform;
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    CaptureBasisTarget(basis, transform);
    SlerpToCapturedTarget(basis, blend);
}


// @verified - p
void cbEvaluateTwoJointRotationOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionRotationBasis &basis = *rotation.basis;
    float blend = *rotation.blendAlpha;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundZ_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*rotation.channels[0]))));
    Matrix4x4::RotateAroundY_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*rotation.channels[1]))));
    Matrix4x4 transform;
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    CaptureBasisTarget(basis, transform);
    SlerpToCapturedTarget(basis, blend);
}

// @verified - p
void cbEvaluateXRotationOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionRotationBasis &basis = *rotation.basis;
    float blend = *rotation.blendAlpha;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundX_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*rotation.channels[0]))));
    Matrix4x4 transform;
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    CaptureBasisTarget(basis, transform);
    SlerpToCapturedTarget(basis, blend);
}

// cbEvaluateZYRotationOverride reads the wrist pair at channels one and two.
// @verified - p
void cbEvaluateZYRotationOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionRotationBasis &basis = *rotation.basis;
    float blend = *rotation.blendAlpha;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundZ_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*rotation.channels[1]))));
    Matrix4x4::RotateAroundY_Wrapper(FloatToShort(RigAngleFloat(SampleMotionRigChannelAdvance(*rotation.channels[2]))));
    Matrix4x4 transform;
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    CaptureBasisTarget(basis, transform);
    SlerpToCapturedTarget(basis, blend);
}

// @verified - p
void ApplyRotY_ThenCopyOrRotZYX(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    float blend = *rotation.blendAlpha;
    const std::uint32_t poseMode = g_ikEnabled ? g_activeMotionWork->rig.lowerBody.poseMode : g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    if (g_ikEnabled && g_activeMotionWork->runtime.pelvisYawCurrent != 0)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_activeMotionWork->runtime.pelvisYawHalf));
    EvaluateRotationParameters(rotation, poseMode, blend);
}

// @verified - p
void ApplyChannelRotZYX_WithIKXBias(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionWork *const work = g_activeMotionWork;
    float blend = *rotation.blendAlpha;
    const std::uint32_t poseMode = g_ikEnabled ? work->rig.chest.poseMode : g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    EvaluateRotationParameters(rotation, poseMode, blend);
    if (g_ikEnabled && work->runtime.pelvisYawCurrent != 0)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(work->runtime.pelvisYawCurrent - work->runtime.pelvisYawHalf));
}

// @verified - p
void cbEvaluateChestOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionWork *const work = g_activeMotionWork;
    float blend = *rotation.blendAlpha;
    const std::uint32_t poseMode = g_ikEnabled ? work->rig.chest.poseMode : g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    const float chestBlend = work->runtime.chestBlendScale;
    if (g_ikEnabled && chestBlend > 0.0f) {
        Matrix4x4 root, pelvis;
        Matrix4x4::GetTransformMatrix(&root);
        Matrix4x4::Pop(1);
        Matrix4x4::GetTransformMatrix(&pelvis);
        const Vector3 rootAxis{root.rows[1].x, root.rows[1].y, root.rows[1].z};
        const Vector3 pelvisAxis{pelvis.rows[1].x, pelvis.rows[1].y, pelvis.rows[1].z};
        const float crossLength =
            std::sqrt(((pelvisAxis.x * rootAxis.z - rootAxis.x * pelvisAxis.z) * (pelvisAxis.x * rootAxis.z - rootAxis.x * pelvisAxis.z) +
                          (pelvisAxis.z * rootAxis.y - pelvisAxis.y * rootAxis.z) * (pelvisAxis.z * rootAxis.y - pelvisAxis.y * rootAxis.z)) +
                (rootAxis.x * pelvisAxis.y - pelvisAxis.x * rootAxis.y) * (rootAxis.x * pelvisAxis.y - pelvisAxis.x * rootAxis.y));
        const float dot = (rootAxis.y * pelvisAxis.y + rootAxis.x * pelvisAxis.x) + rootAxis.z * pelvisAxis.z;
        Vector3 axis{pelvisAxis.z * rootAxis.y - pelvisAxis.y * rootAxis.z, pelvisAxis.x * rootAxis.z - rootAxis.x * pelvisAxis.z,
            rootAxis.x * pelvisAxis.y - pelvisAxis.x * rootAxis.y};
        std::uint16_t angle = 0;
        if (Vector3::CalcMagnitude(axis) >= 0.00000011920929f) {
            angle = FloatToShort(static_cast<float>(FixedAtan2(crossLength, dot)) * chestBlend);
            if (angle != 0) {
                Matrix4x4::SetActiveTransform(&root);
                InvertModelWorkTransform();
                Matrix4x4::Transform(axis, axis);
            }
        }
        Matrix4x4_Push(&root);
        if (angle != 0)
            Matrix4x4_ApplyAngleRotationToVector(axis, angle);
    }
    EvaluateRotationParameters(rotation, poseMode, blend);
}

// @verified - p
void ApplyChannelRotZYX_WithIKBlend(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    MotionWork *const work = g_activeMotionWork;
    float blend = *rotation.blendAlpha;
    const std::uint32_t poseMode = g_ikEnabled ? work->rig.head.poseMode : g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    const float lookBlend = work->runtime.lookAtBlendTime;
    if (g_ikEnabled && lookBlend > 0.0f)
        Matrix4x4_PushActiveTransform();
    EvaluateRotationParameters(rotation, poseMode, blend);
    if (!(g_ikEnabled && lookBlend > 0.0f))
        return;
    Matrix4x4 transform;
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    MotionRotationBasis basis;
    basis.sourceAxis = {transform.rows[0].x, transform.rows[1].x, transform.rows[2].x};
    basis.sourceTangent = {transform.rows[0].y, transform.rows[1].y, transform.rows[2].y};
    if (work->rig.useOverride == 0) {
        basis.targetAxis = {0.0f, 0.0f, 1.0f};
        basis.targetTangent = {0.0f, -1.0f, 0.0f};
    } else {
        basis.targetAxis = {0.0f, 0.70710677f, 0.70710677f};
        basis.targetTangent = {0.0f, -0.70710677f, 0.70710677f};
    }
    SlerpToCapturedTarget(basis, lookBlend);
}

// @verified - p
void ApplyConstraint(MotionWork &work, Vector3 *position, std::uint32_t controller) {
    MotionRuntimeState &state = work.runtime;
    Vector3 source{};
    const float step = state.constraintStepLimit[controller];
    const std::uint32_t shift = 3U * controller;
    const std::uint32_t mode = (state.constraintStateBits >> shift) & 6U;
    Matrix4x4_Push();
    InvertModelWorkTransform();
    Matrix4x4::TransformPoint(state.constraintBlendSource[controller], source);
    if (mode == 2U) {
        Vector3 bone, delta;
        float distance = 0.0f;
        bool limited = false;
        if (state.seqFlags & MSF_LimitConstraintStep) {
            GetMotionControllerTargetPosition(&work, controller, bone);
            Matrix4x4::TransformPoint(bone, bone);
            delta = {source.x - bone.x, source.y - bone.y, source.z - bone.z};
            distance = Vector3::CalcMagnitude(delta);
            limited = step <= distance;
        }
        if (limited) {
            const float fraction = step / distance;
            position->x = shenGetScaledDeltaTickTime(fraction) * delta.x + bone.x;
            position->y = shenGetScaledDeltaTickTime(fraction) * delta.y + bone.y;
            position->z = shenGetScaledDeltaTickTime(fraction) * delta.z + bone.z;
        } else {
            *position = source;
        }
        state.constraintBlendTotalFrames[controller] = 0;
    } else if (mode == 4U) {
        if (!(state.seqFlags & MSF_LimitConstraintStep)) {
            state.constraintStateBits &= ~(6U << shift);
            state.constraintBlendTotalFrames[controller] = 0;
        } else {
            std::uint16_t total = state.constraintBlendTotalFrames[controller];
            if (total == 0) {
                Vector3 bone;
                GetMotionControllerTargetPosition(&work, controller, bone);
                state.constraintBlendSource[controller] = bone;
                Matrix4x4::TransformPoint(state.constraintBlendSource[controller], source);
                Matrix4x4::TransformPoint(bone, bone);
                const Vector3 delta{position->x - bone.x, position->y - bone.y, position->z - bone.z};
                total = static_cast<std::uint16_t>(static_cast<std::uint16_t>(static_cast<std::int64_t>(Vector3::CalcMagnitude(delta) / step)) + 1U);
                state.constraintBlendTotalFrames[controller] = total;
                state.constraintBlendRemainingFrames[controller] = total;
            }
            const std::uint16_t remaining = static_cast<std::uint16_t>(state.constraintBlendRemainingFrames[controller] - 1U);
            state.constraintBlendRemainingFrames[controller] = remaining;
            if (remaining == 0) {
                state.constraintStateBits &= ~(6U << shift);
                state.constraintBlendTotalFrames[controller] = 0;
            } else {
                const float amount = shenGetScaledDeltaTickTime(static_cast<float>(remaining) / static_cast<float>(total));
                position->x = (1.0f - amount) * position->x + amount * source.x;
                position->y = (1.0f - amount) * position->y + amount * source.y;
                position->z = (1.0f - amount) * position->z + amount * source.z;
            }
        }
    }
    Matrix4x4::Pop(1);
}

// EvaluateHeadLookAt: the look-at constraint, limited by the projection cone and the
// per-frame fixed-angle step.
// @verified - p
void EvaluateHeadLookAt(MotionWork &work, Vector3 *target, std::uint32_t controller) {
    MotionRuntimeState &state = work.runtime;
    Vector3 local{};
    Matrix4x4 current;
    Matrix4x4::GetTransformMatrix(&current);
    const std::uint32_t shift = 3U * controller;
    const float ramp = state.pelvisYawRamp;
    const std::uint16_t limit = static_cast<std::uint16_t>(FloatToShort(state.constraintStepLimit[controller] * 10430.378f));
    std::uint16_t step = static_cast<std::uint16_t>(FloatToShort(state.constraintStepRate[controller] * 10430.378f));
    const std::uint16_t mode = static_cast<std::uint16_t>((state.constraintStateBits >> shift) & 6U);
    Matrix4x4_Push();
    InvertModelWorkTransform();
    Matrix4x4::TransformPoint(state.constraintBlendSource[controller], local);
    const Vector3 look = work.rig.head.lookTarget;
    if (state.motnFlags & 0x40U) {
        state.extraMotionMask = 0;
        if (local.x > 0.0f) {
            if (local.x * state.projectionCotangentNear > local.y) {
                state.lookAtYawStep = static_cast<std::int16_t>(FixedAtan2(-local.x, local.y));
                state.lookAtYawStep = static_cast<std::int16_t>(state.lookAtYawStep + AtanToFixedAngle(state.projectionCotangentNear));
                state.extraMotionMask |= 2U;
                local.y = local.x * state.projectionCotangentNear;
            } else if (local.x * state.extraMotionPosition.x > local.y) {
                state.extraMotionMask = 1;
            }
        } else {
            if (-(local.x * state.projectionCotangentNear) > local.y) {
                state.lookAtYawStep = static_cast<std::int16_t>(FixedAtan2(-local.x, local.y));
                state.lookAtYawStep = static_cast<std::int16_t>(state.lookAtYawStep - AtanToFixedAngle(state.projectionCotangentNear));
                state.extraMotionMask |= 8U;
                local.y = -(local.x * state.projectionCotangentNear);
            } else if (-(local.x * state.extraMotionPosition.x) > local.y) {
                state.extraMotionMask = 4;
            }
        }
        const float planar = std::sqrt(local.y * local.y + local.x * local.x);
        if (local.z > 0.0f) {
            const float cotangent = state.projectionCotangentMiddle;
            if (cotangent * local.z > planar) {
                if (cotangent > 0.0f)
                    local.z = planar / cotangent;
                state.extraMotionMask |= 0x20U;
            } else if (local.z * state.extraMotionPosition.y > planar) {
                state.extraMotionMask |= 0x10U;
            }
        } else {
            const float cotangent = state.projectionCotangentFar;
            if (cotangent * local.z > planar) {
                if (0.0f > cotangent)
                    local.z = planar / cotangent;
                state.extraMotionMask |= 0x80U;
            } else if (local.z * state.extraMotionPosition.z > planar) {
                state.extraMotionMask |= 0x40U;
            }
        }
        if (0.2f > planar) {
            if (planar > 0.00000011920929f) {
                const float scale = 0.2f / planar;
                local.x *= scale;
                local.y *= scale;
            } else {
                local.x = 0.0f;
                local.y = 0.2f;
            }
            state.extraMotionMask |= 0x200U;
        } else {
            state.extraMotionMask &= 0xfcffU;
        }
    }
    if (state.seqFlags & MSF_LimitConstraintStep) {
        step = static_cast<std::uint16_t>(step + limit / 5U);
        if (step > limit)
            step = limit;
        if (mode == 2U) {
            const float targetZ = target->z;
            const float targetPlanar = std::sqrt(target->x * target->x + target->y * target->y);
            Vector3 from = look, to = local;
            if (state.motnFlags & 0x40000U) {
                from.z = 0.0f;
                to.z = 0.0f;
            }
            const float fromLength = Vector3::CalcMagnitude(from);
            const float toLength = Vector3::CalcMagnitude(to);
            const float dot = (from.y * to.y + from.x * to.x) + from.z * to.z;
            const Vector3 axis{to.z * from.y - to.y * from.z, to.x * from.z - from.x * to.z, from.x * to.y - to.x * from.y};
            const float crossLength = Vector3::CalcMagnitude(axis);
            const std::uint16_t angle = crossLength >= 0.00000011920929f ? FixedAtan2(crossLength / (toLength * fromLength), dot / (toLength * fromLength)) : 0;
            if (angle >= step) {
                Matrix4x4_PushActiveTransform();
                Matrix4x4_ApplyAngleRotationToVector(axis, FloatToShort(shenGetScaledDeltaTickTime(static_cast<float>(step))));
                Matrix4x4::Transform(look, *target);
                Matrix4x4::Pop(1);
                if (state.motnFlags & 0x40000U)
                    target->z = std::sqrt(target->x * target->x + target->y * target->y) * (targetZ / targetPlanar);
            } else {
                if (0.00000011920929f > ramp)
                    step = 0;
                target->x = local.x;
                target->y = local.y;
                if (state.motnFlags & 0x40000U)
                    target->z = (std::sqrt(local.y * local.y + local.x * local.x) / targetPlanar) * target->z;
                else
                    target->z = local.z;
            }
        } else if (mode == 4U) {
            const float fromLength = Vector3::CalcMagnitude(look);
            const float toLength = Vector3::CalcMagnitude(*target);
            const float dot = (look.y * target->y + look.x * target->x) + look.z * target->z;
            const Vector3 axis{target->z * look.y - target->y * look.z, target->x * look.z - look.x * target->z, look.x * target->y - target->x * look.y};
            const float crossLength = Vector3::CalcMagnitude(axis);
            const std::uint16_t angle = crossLength >= 0.00000011920929f ? FixedAtan2(crossLength / (toLength * fromLength), dot / (toLength * fromLength)) : 0;
            if (angle >= step) {
                Matrix4x4_PushActiveTransform();
                Matrix4x4_ApplyAngleRotationToVector(axis, FloatToShort(shenGetScaledDeltaTickTime(static_cast<float>(step))));
                Matrix4x4::Transform(look, *target);
                Matrix4x4::Pop(1);
            } else {
                if (0.00000011920929f > ramp)
                    step = 0;
                state.constraintStateBits &= ~(6U << shift);
            }
        }
        state.constraintStepRate[controller] = static_cast<float>(step) / 10430.378f;
    } else {
        state.constraintStepRate[controller] = 0.0f;
        if (mode == 2U) {
            target->x = local.x;
            target->y = local.y;
            target->z = local.z;
        } else if (mode == 4U) {
            state.constraintStateBits &= ~(6U << shift);
        }
    }
    Matrix4x4::Pop(1);
}

// @verified - p
void cbEvaluateTargetPosOverride(void *parameters) {
    MotionTargetParameters &target = *static_cast<MotionTargetParameters *>(parameters);
    MotionWork &work = *g_activeMotionWork;
    Vector3 *const output = target.output;
    const Vector4 *const pva = target.pva;
    float blend = work.runtime.layerBlendAlpha[0];
    const std::uint32_t controller = *target.controller;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    const float rootX = SampleMotionRigChannelAdvance(*work.targets[0].channels[0]);
    const float rootZ = SampleMotionRigChannelAdvance(*work.targets[0].channels[2]);
    float blendedX = rootX, blendedZ = rootZ;
    if (work.runtime.motnFlags & 2U) {
        blendedX = (1.0f - blend) * work.rig.lowerBody.pelvisPva[0].w + rootX * blend;
        blendedZ = (1.0f - blend) * work.rig.lowerBody.pelvisPva[2].w + rootZ * blend;
    }
    const float root[3]{rootX, 0.0f, rootZ};
    const float blended[3]{blendedX, 0.0f, blendedZ};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const float value = SampleMotionRigChannelAdvance(*target.channels[axis]);
        const float rest = pva[axis].w * (1.0f - blend);
        (*output)[axis] = (((value - root[axis]) * blend) + rest) + blended[axis];
    }
    if (!g_ikEnabled)
        return;
    if ((work.runtime.constraintStateBits >> (3U * controller)) & 7U)
        ApplyConstraint(work, output, controller);
    if (target.callback)
        InvokeTargetCallback(target, work, output, controller);
}

// Motionwork::PelvisOverride.
// @verified - p
void Motionwork_PelvisOverride(void *parameters) {
    MotionTargetParameters &target = *static_cast<MotionTargetParameters *>(parameters);
    MotionWork &work = *g_activeMotionWork;
    Vector3 *const output = target.output;
    const Vector4 *const pva = target.pva;
    const std::uint32_t controller = *target.controller;
    float blend = work.runtime.layerBlendAlpha[0];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    if (work.runtime.motnFlags & 2U) {
        for (std::size_t axis = 0; axis < 3; ++axis) {
            const float value = SampleMotionRigChannelAdvance(*target.channels[axis]);
            (*output)[axis] = pva[axis].w * (1.0f - blend) + value * blend;
        }
    } else {
        output->x = SampleMotionRigChannelAdvance(*target.channels[0]);
        output->y = (1.0f - blend) * pva[1].w + SampleMotionRigChannelAdvance(*target.channels[1]) * blend;
        output->z = SampleMotionRigChannelAdvance(*target.channels[2]);
    }
    if (!g_ikEnabled)
        return;
    if ((work.runtime.constraintStateBits >> (3U * controller)) & 7U)
        ApplyConstraint(work, output, controller);
    if (target.callback)
        InvokeTargetCallback(target, work, output, controller);
}

// @verified - p
void cbEvaluateLookAtOverride(void *parameters) {
    MotionTargetParameters &target = *static_cast<MotionTargetParameters *>(parameters);
    MotionWork &work = *g_activeMotionWork;
    const std::uint32_t controller = *target.controller;
    const float blend = BlendForController(work, controller);
    Vector3 sampled;
    SampleBlendedTarget(target, blend, sampled);
    if (!g_ikEnabled)
        return;
    if ((work.runtime.constraintStateBits >> (3U * controller)) & 7U)
        EvaluateHeadLookAt(work, target.output, controller);
    if (target.callback)
        InvokeTargetCallback(target, work, target.output, controller);
}

// @verified - p
void Arm_TargetEffectorOverride(void *parameters) {
    MotionTargetParameters &target = *static_cast<MotionTargetParameters *>(parameters);
    MotionWork &work = *g_activeMotionWork;
    MotionRuntimeState &state = work.runtime;
    const std::uint32_t controller = *target.controller;
    const float blend = BlendForController(work, controller);
    Vector3 world;
    SampleBlendedTarget(target, blend, world);
    if (!g_ikEnabled)
        return;
    if ((state.constraintStateBits >> (3U * controller)) & 7U) {
        if (state.seqFlags & MSF_LookAt) {
            if (controller == 3U) {
                if (state.locomotionMotionSet & 0xa0U) {
                    Matrix4x4::TransformPoint(*target.output, world);
                    if (state.locomotionMotionSet & 0x80U) {
                        Vector3 &smoothed = state.constraintBlendSource[3];
                        const float z = (world.z + smoothed.z) * 0.5f;
                        smoothed.x = (world.x + smoothed.x) * 0.5f;
                        const float y = world.y + smoothed.y;
                        smoothed.z = z;
                        smoothed.y = y * 0.5f;
                    }
                }
            } else if (controller == 4U && (state.locomotionMotionSet & 0x140U)) {
                Matrix4x4::TransformPoint(*target.output, world);
                if (state.locomotionMotionSet & 0x100U) {
                    Vector3 &smoothed = state.constraintBlendSource[4];
                    const float z = (world.z + smoothed.z) * 0.5f;
                    smoothed.x = (world.x + smoothed.x) * 0.5f;
                    const float y = world.y + smoothed.y;
                    smoothed.z = z;
                    smoothed.y = y * 0.5f;
                }
            }
        }
        ApplyConstraint(work, target.output, controller);
    }
    if (target.callback)
        InvokeTargetCallback(target, work, target.output, controller);
}

// Motionwork::ChestIKEffectorOverride.
// @verified - p
void Motionwork_ChestIKEffectorOverride(void *parameters) {
    MotionTargetParameters &target = *static_cast<MotionTargetParameters *>(parameters);
    MotionWork &work = *g_activeMotionWork;
    MotionRuntimeState &state = work.runtime;
    Vector3 *const output = target.output;
    const std::uint32_t controller = *target.controller;
    const float blend = BlendForController(work, controller);
    Vector3 rest, sampled;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const float value = SampleMotionRigChannelAdvance(*target.channels[axis]);
        const float restValue = target.pva[axis].w;
        rest[axis] = restValue;
        sampled[axis] = value;
        (*output)[axis] = restValue * (1.0f - blend) + value * blend;
    }
    if (!g_ikEnabled)
        return;
    if (((state.constraintStateBits >> (3U * controller)) & 7U) || !(state.seqFlags & MSF_LookAt)) {
        if ((state.constraintStateBits >> (3U * controller)) & 7U)
            ApplyConstraint(work, output, controller);
        if (target.callback)
            InvokeTargetCallback(target, work, output, controller);
        return;
    }
    Matrix4x4::Transform(*output, sampled);
    const float length = Vector3::CalcMagnitude(sampled);
    rest = {output->x >= 0.0f ? 0.0f : output->x, length, 0.0f};
    Matrix4x4::Transform(rest, rest);
    float scale = state.extraBlendScale;
    bool advance = false;
    if (!(state.locomotionMotionSet & 8U) || state.transitionState == 3U) {
        if (0.30000001f > state.extraBlendTime)
            scale = ((scale * 0.69999999f) * state.extraBlendTime) / 0.30000001f + scale * 0.30000001f;
    } else {
        Vector3 unused;
        Matrix4x4::TransformPoint(*output, unused);
        if (state.extraMotionMask & 0x2aU) {
            if (state.extraBlendTime > 0.69999999f)
                scale = ((1.0f - state.extraBlendTime) * (scale * 0.69999999f)) / 0.30000001f + scale * 0.30000001f;
            state.extraBlendTime = shenGetScaledDeltaTickTime(scale) + state.extraBlendTime;
            advance = true;
        } else if (state.extraMotionMask & 0x3fU) {
            advance = true;
        } else if (0.30000001f > state.extraBlendTime) {
            scale = ((scale * 0.69999999f) * state.extraBlendTime) / 0.30000001f + scale * 0.30000001f;
        }
    }
    if (!advance)
        state.extraBlendTime = state.extraBlendTime - shenGetScaledDeltaTickTime(scale);
    if (state.extraBlendTime > 1.0f)
        state.extraBlendTime = 1.0f;
    else if (0.0f > state.extraBlendTime)
        state.extraBlendTime = 0.0f;
    const float time = state.extraBlendTime;
    sampled.x = (1.0f - time) * sampled.x + time * rest.x;
    sampled.z = (1.0f - time) * sampled.z + time * rest.z;
    sampled.y = (1.0f - time) * sampled.y + time * rest.y;
    Matrix4x4_Push();
    InvertModelWorkTransform();
    Matrix4x4::Transform(sampled, *output);
    Matrix4x4::Pop(1);
    if (target.callback)
        InvokeTargetCallback(target, work, output, controller);
}

namespace {

std::int32_t MotionFrameCount(std::uint16_t motion) {
    if (!motion)
        return 0;
    const std::span<const std::byte> data = FindLoadedMotionData(motion);
    if (data.empty())
        return 0;
    std::uint16_t frames;
    std::memcpy(&frames, data.data() + 4, sizeof(frames));
    return frames;
}

const std::uint16_t *MotionHeader(std::uint16_t motion) {
    const std::span<const std::byte> data = FindLoadedMotionData(motion);
    return data.empty() ? nullptr : reinterpret_cast<const std::uint16_t *>(data.data());
}

void BindCallback(MotionCallback &callback, void (*evaluate)(void *), void *parameters) {
    callback.evaluate = evaluate;
    callback.parameters = parameters;
}

MotionTimePair *RegionAuthoredTime(MotionWork *work, std::uint32_t region) {
    switch (region) {
    case 0:
        return &work->rig.lowerBody.clock.authored;
    case 1:
        return &work->rig.chest.clock.authored;
    case 2:
        return &work->rig.head.clock.authored;
    case 3:
        return &work->rig.rightArm.clock.authored;
    case 4:
        return &work->rig.leftArm.clock.authored;
    default:
        return nullptr;
    }
}

void TranslateMatrix(Matrix4x4 &matrix, float x, float y, float z) {
    matrix.rows[3].x = x + matrix.rows[3].x;
    matrix.rows[3].y = y + matrix.rows[3].y;
    matrix.rows[3].z = z + matrix.rows[3].z;
}

Vector3 &TargetOutput(MotionRigTarget &target) {
    return *reinterpret_cast<Vector3 *>(target.axes[2].dimensions);
}

void TranslateTarget(MotionRigTarget &target, float x, float y, float z) {
    Vector3 &output = TargetOutput(target);
    output.x = x + output.x;
    output.y = y + output.y;
    output.z = z + output.z;
}

} // namespace

// @verified - p
void mtwkCopyRigFlags(
    MotionWork *work, MotionRigClock **lowerBody, MotionRigClock **chest, MotionRigClock **head, MotionRigClock **rightArm, MotionRigClock **leftArm) {
    if (lowerBody)
        *lowerBody = &work->rig.lowerBody.clock;
    if (chest)
        *chest = &work->rig.chest.clock;
    if (head)
        *head = &work->rig.head.clock;
    if (rightArm)
        *rightArm = &work->rig.rightArm.clock;
    if (leftArm)
        *leftArm = &work->rig.leftArm.clock;
}

// Clamps the region's published clock to the requested split/current/end and mirrors the
// split and end into the rig's authored pair; returns the clamped current time.
// @verified - p
float TimePair_ClampAndPropagate(MotionWork *work, float split, float ticks, float maximum, MotionRegion region) {
    MotionTimePair &pair = g_motionRegionClocks[region]->current;
    if (maximum >= 0.0f && pair.maxTime > maximum)
        pair.maxTime = maximum;
    const float limit = pair.maxTime;
    if (split >= 0.0f && limit > split)
        pair.splitTimeFrames = split;
    else if (split >= limit)
        pair.splitTimeFrames = limit;
    if (ticks >= 0.0f)
        pair.ticksTime = ticks;
    if (pair.ticksTime >= limit)
        pair.ticksTime = limit;
    else if (pair.splitTimeFrames >= pair.ticksTime)
        pair.ticksTime = pair.splitTimeFrames;
    if (MotionTimePair *const authored = RegionAuthoredTime(work, region)) {
        if (split >= 0.0f)
            authored->splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            authored->ticksTime = g_motionRegionClocks[region]->current.maxTime;
    }
    return pair.ticksTime;
}

// @verified - p
void AssignStaticDataForBodyRegion(MotionWork *work, float split, float ticks, float maximum, MotionRegion region) {
    MotionTimePair &pair = g_motionRegionClocks[region]->current;
    if (split > 0.0f)
        pair.splitTimeFrames = 0.0f;
    if (ticks > 0.0f)
        pair.ticksTime = 0.0f;
    if (maximum > 0.0f) {
        pair.maxTime = static_cast<float>(MotionFrameCount(work->runtime.currentMotionId[region]) - 1);
        if (work->runtime.regionFlags[region] & MRF_ExcludeEndFrame)
            pair.maxTime = pair.maxTime - 0.001f;
    }
    if (MotionTimePair *const authored = RegionAuthoredTime(work, region)) {
        if (split >= 0.0f)
            authored->splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            authored->ticksTime = g_motionRegionClocks[region]->current.maxTime;
    }
}

// Returns 0 within range, 1 clamped to the end, 2 reset to the split, 3 wrapped across.
// @verified - p
std::uint32_t TimestepUpdate(MotionRigClock *clock, std::uint32_t allowOverflow, std::uint32_t allowReset) {
    MotionTimePair &pair = clock->current;
    const float time = shenGetScaledDeltaTickTime(clock->playbackSpeed) + pair.ticksTime;
    const float maximum = pair.maxTime;
    if (!(time > maximum)) {
        pair.ticksTime = time;
        return 0;
    }
    if (allowOverflow) {
        pair.ticksTime = time;
        const float overflow = pair.ticksTime - ceil_float(maximum);
        pair.ticksTime = (0.0f > overflow ? 0.0f : overflow) + pair.splitTimeFrames;
        return 3;
    }
    if (allowReset) {
        pair.ticksTime = pair.splitTimeFrames;
        return 2;
    }
    pair.ticksTime = maximum;
    return 1;
}

// Advances a tracked clock by one frame; 4 marks the forced cycle end.
// @verified - p
std::uint32_t AnimClock_ForceJump(MotionRigClock *clock) {
    const float time = shenGetScaledDeltaTickTime(1.0f) + clock->current.ticksTime;
    const bool running = time < clock->current.maxTime;
    clock->current.ticksTime = time;
    return running ? 0U : 4U;
}

// Samples a motion's root translation channels at a frame (negative selects the last frame),
// scaled from the authored rig to the requested one.
// @verified - p
bool SampleNativeMotionRoot(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 &output) {
    const std::uint16_t *const header = MotionHeader(animation);
    if (!header) {
        output = {};
        return false;
    }
    const std::uint16_t sourceRig = header[1] & 0xffU;
    float scale = 1.0f;
    if (boneId >= 0) {
        const std::uint16_t targetRig = static_cast<std::uint16_t>(boneId >= 17 ? 8 : boneId);
        if (targetRig != sourceRig)
            scale = g_motionRigDimensions[targetRig][0] / g_motionRigDimensions[sourceRig][0];
    }
    MotionRigAxis channels[3]{};
    for (MotionRigAxis &channel : channels)
        UpdateAnimRate(channel.channel, scale, 0.0f);
    const std::uint16_t flags = header[1];
    if (flags & motionWideKeyCounts)
        g_motionWideKeyCounts = header + 4;
    else
        g_motionKeyCounts = reinterpret_cast<const std::uint8_t *>(header + 4);
    g_motionInitFlags = motionInitStream;
    g_motionKeyData = reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(header) + static_cast<std::int16_t>(header[3]));
    g_motionAnimFlags = flags;
    g_motionPreviousAnimTime = 0.0f;
    g_motionMaximumAnimTime = static_cast<float>(header[2] - 1) * motionFrameSeconds;
    for (MotionRigAxis &channel : channels)
        ReadMotionKeyBlock(channel.channel);
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionCurrentAnimTime = 0.0f > frame ? g_motionMaximumAnimTime : frame * motionFrameSeconds;
    float sampled[3];
    for (std::size_t axis = 0; axis < 3; ++axis)
        sampled[axis] = SampleMotionRigChannelAdvance(channels[axis].channel);
    output = {sampled[0], sampled[1], sampled[2]};
    return true;
}

// @verified - p
void Rig_EvaluateControllers(MotionWork *work, const MotionStreamContext *context, float sourceTime, float time, std::uint32_t flags, MotionRegion region) {
    IKRig &rig = work->rig;
    g_motionUseOverride = static_cast<std::int32_t>(rig.useOverride);
    switch (region) {
    case LowerBody:
        if (rig.useOverride) {
            Matrix4x4_Push();
            InvertModelWorkTransform();
            Matrix4x4::MultiplyWithPrePassMatrix(&rig.lowerBody.rootPrepassTransform);
            Matrix4x4::MultiplyWithPrePassMatrix(&rig.lowerBody.pelvisTransform);
            g_motionOverrideRootTransform = workTransformMatrix;
            Matrix4x4::Pop(1);
        }
        ReadMotionBase(rig, rig.lowerBody, context, sourceTime, time, flags);
        break;
    case Chest:
        ReadMotionChest(rig, rig.chest, context, time, flags);
        break;
    case Head:
        ReadMotionHead(rig, rig.head, context, time, flags);
        break;
    case RightArm:
        ReadMotionArm(rig, rig.rightArm, context, time, flags);
        break;
    case LeftArm:
        ReadMotionArm(rig, rig.leftArm, context, time, flags | motionInitLeftArm);
        break;
    default:
        break;
    }
    g_motionOverrideBlendTime = -1.0f;
}

// Reads a region's motion for the tracked (root mode 2) graph and, when the motion carries
// graph frames, installs the override callbacks that blend from the captured PVA.
// @verified - p
bool Rig_SetupMotionGraph(IKRig &rig, const MotionStreamContext *context, float time, std::uint32_t flags, std::uint32_t region,
    MotionTargetParameters *targets, MotionRotationParameters *rotations) {
    std::uint32_t graph = (flags >> 8) & 0xffU;
    if (targets && rotations) {
        if (graph)
            flags = (flags & 0xffff00ffU) | motionInitCallbacks;
    } else {
        graph = 0;
    }
    flags |= motionInitChannels;
    MotionBaseRig &base = rig.lowerBody;
    switch (region) {
    case 0:
        ReadMotionBase(rig, base, context, -1.0f, time, flags);
        if (!graph)
            break;
        {
            const float rootX = base.pelvisPva[0].w;
            const float rootZ = base.pelvisPva[2].w;
            base.leftFoot.pva[0].w = base.leftFoot.pva[0].w - rootX;
            base.rightFoot.pva[0].w = base.rightFoot.pva[0].w - rootX;
            base.leftFoot.pva[2].w = base.leftFoot.pva[2].w - rootZ;
            base.rightFoot.pva[2].w = base.rightFoot.pva[2].w - rootZ;
            if (rig.useOverride) {
                base.overridePelvisTarget->pva[0].w = base.overridePelvisTarget->pva[0].w - rootX;
                base.overridePelvisTarget->pva[2].w = base.overridePelvisTarget->pva[2].w - base.pelvisPva[2].w;
            }
        }
        BindCallback(base.pelvisPositionCallback, Motionwork_PelvisOverride, &targets[0]);
        BindCallback(base.pelvisRotationCallback, ApplyRotY_ThenCopyOrRotZYX, &rotations[0]);
        if (!rig.useOverride) {
            BindCallback(base.hips.callback, cbEvaluateTargetRotOverride, &rotations[1]);
        } else {
            BindCallback(base.overrideHips->callback, cbEvaluateTargetRotOverride, &rotations[1]);
            BindCallback(base.overridePelvisTarget->callback, cbEvaluateTargetPosOverride, &targets[1]);
            BindCallback(base.leftHipNode->callback, cbEvaluateTargetRotOverride, &rotations[2]);
        }
        BindCallback(base.leftThigh.callback, cbEvaluateTargetRotOverride, &rotations[3]);
        BindCallback(base.leftFoot.callback, cbEvaluateTargetPosOverride, &targets[2]);
        BindCallback(base.leftAnkleCallback, cbEvaluateTargetRotOverride, &rotations[4]);
        if (rig.useOverride)
            BindCallback(base.rightHipNode->callback, cbEvaluateTargetRotOverride, &rotations[5]);
        BindCallback(base.rightThigh.callback, cbEvaluateTargetRotOverride, &rotations[6]);
        BindCallback(base.rightFoot.callback, cbEvaluateTargetPosOverride, &targets[3]);
        BindCallback(base.rightAnkleCallback, cbEvaluateTargetRotOverride, &rotations[7]);
        if (rig.useOverride) {
            BindCallback(base.rightToe1Node->callback, cbEvaluateTwoJointRotationOverride, &rotations[8]);
            BindCallback(base.rightToe2Node->callback, cbEvaluateTwoJointRotationOverride, &rotations[9]);
            BindCallback(base.rightToe3Node->callback, cbEvaluateTwoJointRotationOverride, &rotations[10]);
        }
        break;
    case 1:
        ReadMotionChest(rig, rig.chest, context, time, flags);
        if (!graph)
            break;
        BindCallback(rig.chest.rotation.callback, cbEvaluateChestOverride, &rotations[11]);
        if (!rig.useOverride) {
            BindCallback(rig.chest.target.callback, Motionwork_ChestIKEffectorOverride, &targets[4]);
            BindCallback(rig.chest.upperRotation.callback, ApplyChannelRotZYX_WithIKXBias, &rotations[12]);
        } else {
            rig.chest.target.pva[0].w = rig.chest.target.pva[0].w - base.pelvisPva[0].w;
            rig.chest.target.pva[2].w = rig.chest.target.pva[2].w - base.pelvisPva[2].w;
            BindCallback(rig.chest.target.callback, cbEvaluateTargetPosOverride, &targets[4]);
        }
        break;
    case 2:
        ReadMotionHead(rig, rig.head, context, time, flags);
        if (!graph)
            break;
        BindCallback(rig.head.baseRotation.callback, cbEvaluateTargetRotOverride, &rotations[13]);
        if (rig.useOverride) {
            rig.head.aimTargetNode->pva[0].w = rig.head.aimTargetNode->pva[0].w - base.pelvisPva[0].w;
            rig.head.aimTargetNode->pva[2].w = rig.head.aimTargetNode->pva[2].w - base.pelvisPva[2].w;
            BindCallback(rig.head.aimSourceNode->callback, cbEvaluateTargetRotOverride, &rotations[14]);
            BindCallback(rig.head.aimTargetNode->callback, cbEvaluateTargetPosOverride, &targets[5]);
        }
        BindCallback(rig.head.midRotation.callback, ApplyChannelRotZYX_WithIKBlend, &rotations[15]);
        BindCallback(rig.head.target.callback, cbEvaluateLookAtOverride, &targets[6]);
        if (rig.useOverride) {
            BindCallback(rig.head.nodeA->callback, cbEvaluateTargetRotOverride, &rotations[16]);
            BindCallback(rig.head.nodeB->callback, cbEvaluateTargetRotOverride, &rotations[17]);
            BindCallback(rig.head.nodeC->callback, cbEvaluateTargetRotOverride, &rotations[18]);
        }
        break;
    case 3:
        ReadMotionArm(rig, rig.rightArm, context, time, flags);
        if (!graph)
            break;
        BindCallback(rig.rightArm.shoulder.callback, cbEvaluateTargetRotOverride, &rotations[19]);
        BindCallback(rig.rightArm.elbow.callback, cbEvaluateTargetRotOverride, &rotations[20]);
        if (!rig.useOverride) {
            BindCallback(rig.rightArm.target.callback, Arm_TargetEffectorOverride, &targets[7]);
            BindCallback(rig.rightArm.forearmCallback, cbEvaluateXRotationOverride, &rotations[21]);
            BindCallback(rig.rightArm.wristCallback, cbEvaluateZYRotationOverride, &rotations[22]);
        } else {
            rig.rightArm.target.pva[0].w = rig.rightArm.target.pva[0].w - base.pelvisPva[0].w;
            rig.rightArm.target.pva[2].w = rig.rightArm.target.pva[2].w - base.pelvisPva[2].w;
            BindCallback(rig.rightArm.target.callback, cbEvaluateTargetPosOverride, &targets[7]);
            BindCallback(rig.rightArm.wristCallback, cbEvaluateTargetRotOverride, &rotations[22]);
        }
        break;
    case 4:
        ReadMotionArm(rig, rig.leftArm, context, time, flags | motionInitLeftArm);
        if (!graph)
            break;
        BindCallback(rig.leftArm.shoulder.callback, cbEvaluateTargetRotOverride, &rotations[23]);
        BindCallback(rig.leftArm.elbow.callback, cbEvaluateTargetRotOverride, &rotations[24]);
        if (!rig.useOverride) {
            BindCallback(rig.leftArm.target.callback, Arm_TargetEffectorOverride, &targets[8]);
            BindCallback(rig.leftArm.forearmCallback, cbEvaluateXRotationOverride, &rotations[25]);
            BindCallback(rig.leftArm.wristCallback, cbEvaluateZYRotationOverride, &rotations[26]);
        } else {
            rig.leftArm.target.pva[0].w = rig.leftArm.target.pva[0].w - base.pelvisPva[0].w;
            rig.leftArm.target.pva[2].w = rig.leftArm.target.pva[2].w - base.pelvisPva[2].w;
            // The override rig's left target reuses the right arm's parameter record.
            BindCallback(rig.leftArm.target.callback, cbEvaluateTargetPosOverride, &targets[7]);
            BindCallback(rig.leftArm.wristCallback, cbEvaluateTargetRotOverride, &rotations[26]);
        }
        break;
    default:
        break;
    }
    return true;
}

// @verified - p
void Rig_RunOverrideBlendPass(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    for (std::uint32_t region = 0; region < 5; ++region) {
        if ((state.regionFlags[region] & MRF_Transition) && (state.currentRootMode & 0xfdU)) {
            state.overrideBlendTime = -1.0f;
            return;
        }
    }
    for (std::uint32_t region = 0; region < 5; ++region) {
        if (state.regionFlags[region] & MRF_Independent)
            continue;
        const std::uint16_t *const header = MotionHeader(state.currentMotionId[region]);
        const std::int32_t sourceRig = static_cast<std::int8_t>(header[1]);
        const float scale = mtwkGetScaleFactorFor(work->rig.boneId, static_cast<std::uint32_t>(sourceRig), region < 4 ? region : 3U);
        g_motionInitFlags = (static_cast<std::uint8_t>(state.regionFlags[region]) >> 5) & motionInitMirror;
        const std::uint8_t rig = static_cast<std::uint8_t>(sourceRig);
        std::int32_t evaluated = 0;
        switch (region) {
        case 0:
            evaluated = EvaluateBase(work->rig.lowerBody, rig, scale, state.overrideBlendTime);
            break;
        case 1:
            evaluated = EvaluateChestBlend(work->rig.chest, rig, scale, state.overrideBlendTime);
            break;
        case 2:
            evaluated = EvaluateHead(work->rig.head, rig, scale, state.overrideBlendTime);
            break;
        case 3:
            evaluated = EvaluateArm(work->rig.rightArm, rig, scale, state.overrideBlendTime);
            break;
        default:
            evaluated = EvaluateArm(work->rig.leftArm, rig, scale, state.overrideBlendTime);
            break;
        }
        if (static_cast<std::uint8_t>(evaluated))
            Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
    }
    TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
    state.overrideBlendTime = -1.0f;
}

// @verified - p
void ClearOverrideCallbacksForBodyRegion(MotionWork *work, MotionRegion region) {
    IKRig &rig = work->rig;
    MotionBaseRig &base = rig.lowerBody;
    switch (region) {
    case 0:
        base.pelvisPositionCallback = {};
        base.pelvisRotationCallback = {};
        base.hips.callback = {};
        base.leftThigh.callback = {};
        base.leftFoot.callback = {};
        base.leftAnkleCallback = {};
        base.rightThigh.callback = {};
        base.rightFoot.callback = {};
        base.rightAnkleCallback = {};
        if (rig.useOverride) {
            base.overrideHips->callback = {};
            base.overridePelvisTarget->callback = {};
            base.leftHipNode->callback = {};
            base.rightHipNode->callback = {};
            base.rightToe1Node->callback = {};
            base.rightToe2Node->callback = {};
            base.rightToe3Node->callback = {};
        }
        break;
    case 1:
        rig.chest.rotation.callback = {};
        rig.chest.target.callback = {};
        rig.chest.upperRotation.callback = {};
        break;
    case 2:
        rig.head.baseRotation.callback = {};
        rig.head.midRotation.callback = {};
        rig.head.target.callback = {};
        if (rig.useOverride) {
            rig.head.aimSourceNode->callback = {};
            rig.head.aimTargetNode->callback = {};
            rig.head.nodeA->callback = {};
            rig.head.nodeB->callback = {};
            rig.head.nodeC->callback = {};
        }
        break;
    case 3:
    case 4: {
        MotionArmRig &arm = region == 3 ? rig.rightArm : rig.leftArm;
        arm.shoulder.callback = {};
        arm.elbow.callback = {};
        arm.target.callback = {};
        arm.forearmCallback = {};
        arm.wristCallback = {};
        break;
    }
    default:
        break;
    }
}

// @verified - p
void InstallActiveOverrideCallbacks(MotionWork *work) {
    IKRig &rig = work->rig;
    MotionTargetParameters *const targets = work->targets;
    if (targets[0].callback)
        BindCallback(rig.lowerBody.pelvisPositionCallback, Motionwork_PelvisOverride, &targets[0]);
    if (targets[6].callback)
        BindCallback(rig.head.target.callback, cbEvaluateLookAtOverride, &targets[6]);
    if (targets[2].callback)
        BindCallback(rig.lowerBody.leftFoot.callback, cbEvaluateTargetPosOverride, &targets[2]);
    if (targets[3].callback)
        BindCallback(rig.lowerBody.rightFoot.callback, cbEvaluateTargetPosOverride, &targets[3]);
    if (rig.useOverride) {
        if (targets[4].callback)
            BindCallback(rig.chest.target.callback, cbEvaluateTargetPosOverride, &targets[4]);
        if (targets[1].callback)
            BindCallback(rig.lowerBody.overridePelvisTarget->callback, cbEvaluateTargetPosOverride, &targets[1]);
        if (targets[5].callback)
            BindCallback(rig.head.aimTargetNode->callback, cbEvaluateTargetPosOverride, &targets[5]);
        if (targets[7].callback)
            BindCallback(rig.rightArm.target.callback, cbEvaluateTargetPosOverride, &targets[7]);
        if (targets[8].callback)
            BindCallback(rig.leftArm.target.callback, cbEvaluateTargetPosOverride, &targets[8]);
    } else {
        if (targets[4].callback)
            BindCallback(rig.chest.target.callback, Motionwork_ChestIKEffectorOverride, &targets[4]);
        if (targets[7].callback)
            BindCallback(rig.rightArm.target.callback, Arm_TargetEffectorOverride, &targets[7]);
        if (targets[8].callback)
            BindCallback(rig.leftArm.target.callback, Arm_TargetEffectorOverride, &targets[8]);
    }
}

// @verified - p
void InstallConstraintTargetCallback(MotionWork *work, std::uint32_t controller) {
    IKRig &rig = work->rig;
    MotionTargetParameters *const targets = work->targets;
    switch (controller) {
    case 0:
        BindCallback(rig.lowerBody.pelvisPositionCallback, Motionwork_PelvisOverride, &targets[0]);
        break;
    case 1:
        BindCallback(rig.chest.target.callback, rig.useOverride ? cbEvaluateTargetPosOverride : Motionwork_ChestIKEffectorOverride, &targets[4]);
        break;
    case 2:
        BindCallback(rig.head.target.callback, cbEvaluateLookAtOverride, &targets[6]);
        break;
    case 3:
        BindCallback(rig.rightArm.target.callback, rig.useOverride ? cbEvaluateTargetPosOverride : Arm_TargetEffectorOverride, &targets[7]);
        break;
    case 4:
        BindCallback(rig.leftArm.target.callback, rig.useOverride ? cbEvaluateTargetPosOverride : Arm_TargetEffectorOverride, &targets[8]);
        break;
    case 5:
        BindCallback(rig.lowerBody.leftFoot.callback, cbEvaluateTargetPosOverride, &targets[2]);
        break;
    case 6:
        BindCallback(rig.lowerBody.rightFoot.callback, cbEvaluateTargetPosOverride, &targets[3]);
        break;
    default:
        if (!rig.useOverride)
            break;
        if (controller == 7)
            BindCallback(rig.lowerBody.overridePelvisTarget->callback, cbEvaluateTargetPosOverride, &targets[1]);
        else if (controller == 8)
            BindCallback(rig.head.aimTargetNode->callback, cbEvaluateTargetPosOverride, &targets[5]);
        break;
    }
}

// @verified - p
void ClearConstraintTargetCallbackIfInactive(MotionWork *work, std::uint32_t controller) {
    IKRig &rig = work->rig;
    const MotionRuntimeState &state = work->runtime;
    const auto inactive = [&](std::uint32_t region) { return !(state.regionFlags[region] & MRF_Transition) || state.currentRootMode != 2; };
    switch (controller) {
    case 0:
        if (inactive(0))
            rig.lowerBody.pelvisPositionCallback = {};
        break;
    case 1:
        if (inactive(1))
            rig.chest.target.callback = {};
        break;
    case 2:
        if (inactive(2))
            rig.head.target.callback = {};
        break;
    case 3:
        if (inactive(3))
            rig.rightArm.target.callback = {};
        break;
    case 4:
        if (inactive(4))
            rig.leftArm.target.callback = {};
        break;
    case 5:
        if (inactive(0))
            rig.lowerBody.leftFoot.callback = {};
        break;
    case 6:
        if (inactive(0))
            rig.lowerBody.rightFoot.callback = {};
        break;
    default:
        if (!rig.useOverride)
            break;
        if (controller == 7) {
            if (inactive(0))
                rig.lowerBody.overridePelvisTarget->callback = {};
        } else if (controller == 8 && inactive(2)) {
            rig.head.aimTargetNode->callback = {};
        }
        break;
    }
}



// @verified - p
void UpdatePelvisYaw(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    const std::int16_t clamp = state.pelvisYawClamp;
    std::int16_t step = static_cast<std::int16_t>(state.pelvisYawStep);
    const std::int16_t delta = static_cast<std::int16_t>(state.pelvisYawTarget - state.pelvisYawCurrent);
    const std::int16_t distance = static_cast<std::int16_t>(delta < 0 ? -delta : delta);
    const std::int16_t threshold = static_cast<std::int16_t>(TruncateToInt32(static_cast<float>(clamp) * 0.30000001f));
    if (distance < threshold)
        step = static_cast<std::int16_t>(TruncateToInt32((static_cast<float>(delta) / static_cast<float>(threshold)) * static_cast<float>(step)));
    else if (delta < 0)
        step = static_cast<std::int16_t>(-step);
    if (static_cast<std::int32_t>(distance) < (step < 0 ? -static_cast<std::int32_t>(step) : step))
        step = delta;
    const std::int32_t magnitude = step < 0 ? -static_cast<std::int32_t>(step) : step;
    if (0.00000011920929f > static_cast<float>(magnitude)) {
        state.pelvisYawRamp = 0.0f;
    } else {
        const float ramp = shenGetScaledDeltaTickTime(0.2f) + state.pelvisYawRamp;
        state.pelvisYawRamp = ramp;
        if (ramp > 1.0f)
            state.pelvisYawRamp = 1.0f;
    }
    const std::int32_t advance = TruncateToInt32(shenGetScaledDeltaTickTime(static_cast<float>(step) * state.pelvisYawRamp));
    state.pelvisYawVelocity = step;
    state.pelvisYawCurrent = static_cast<std::int16_t>(state.pelvisYawCurrent + advance);
    if (state.pelvisYawCurrent > clamp)
        state.pelvisYawCurrent = clamp;
    else if (static_cast<std::int32_t>(state.pelvisYawCurrent) < -static_cast<std::int32_t>(clamp))
        state.pelvisYawCurrent = static_cast<std::int16_t>(-clamp);
    const std::int16_t half = static_cast<std::int16_t>(state.pelvisYawCurrent / 2);
    state.pelvisYawHalf = half;
    if (half > 3640)
        state.pelvisYawHalf = 3640;
    else if (half < -3640)
        state.pelvisYawHalf = -3640;
}

// @verified - p
std::int32_t UpdateMotionOverrideCallbacks(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    IKRig &rig = work->rig;
    const std::uint32_t controllers = rig.useOverride ? 9U : 7U;
    for (std::uint32_t controller = 0; controller < controllers; ++controller) {
        Vector3 *const target = state.activeConstraintTarget[controller];
        if (!target)
            continue;
        Matrix4x4::TransformPoint(*target, *target);
        state.latchedConstraintTarget[controller] = target;
        state.activeConstraintTarget[controller] = nullptr;
    }
    for (std::uint32_t controller = 0, shift = 0; controller < controllers; ++controller, shift += 3) {
        if (state.latchedConstraintTarget[controller]) {
            state.constraintStateBits = (3U << shift) | (state.constraintStateBits & ~(7U << shift));
            InstallConstraintTargetCallback(work, controller);
            continue;
        }
        const std::uint32_t bits = state.constraintStateBits;
        if (!((bits >> shift) & 7U))
            continue;
        if ((bits >> shift) & 6U) {
            state.constraintStateBits = (4U << shift) | (bits & ~(6U << shift));
            InstallConstraintTargetCallback(work, controller);
        } else {
            state.constraintStateBits = bits & ~(7U << shift);
            ClearConstraintTargetCallbackIfInactive(work, controller);
        }
    }
    if (!rig.useOverride) {
        if (state.pelvisYawCurrent || state.pelvisYawHalf || state.pelvisYawTarget) {
            BindCallback(rig.lowerBody.pelvisRotationCallback, ApplyRotY_ThenCopyOrRotZYX, &work->rotations[0]);
            BindCallback(rig.chest.upperRotation.callback, ApplyChannelRotZYX_WithIKXBias, &work->rotations[12]);
            UpdatePelvisYaw(work);
        } else {
            if (!(state.regionFlags[0] & MRF_Transition) || state.currentRootMode != 2)
                rig.lowerBody.pelvisRotationCallback.evaluate = nullptr;
            if (!(state.regionFlags[1] & MRF_Transition) || state.currentRootMode != 2)
                rig.chest.upperRotation.callback.evaluate = nullptr;
            state.lookAtYaw = 0;
        }
    }
    if (state.seqFlags & MSF_LookAt) {
        BindCallback(rig.head.target.callback, cbEvaluateLookAtOverride, &work->targets[6]);
        if (!rig.useOverride)
            BindCallback(rig.chest.target.callback, Motionwork_ChestIKEffectorOverride, &work->targets[4]);
        BindCallback(rig.head.midRotation.callback, ApplyChannelRotZYX_WithIKBlend, &work->rotations[15]);
        const float blend = shenGetScaledDeltaTickTime(0.1f) + state.lookAtBlendTime;
        state.lookAtBlendTime = blend;
        if (blend > 1.0f)
            state.lookAtBlendTime = 1.0f;
    } else {
        if (!(state.regionFlags[1] & MRF_Transition) || state.currentRootMode != 2)
            rig.chest.target.callback = {};
        const float blend = state.lookAtBlendTime - shenGetScaledDeltaTickTime(0.1f);
        state.lookAtBlendTime = blend;
        if (0.0f > blend)
            state.lookAtBlendTime = 0.0f;
        if ((!(state.regionFlags[2] & MRF_Transition) || state.currentRootMode != 2) && 0.00000011920929f > state.lookAtBlendTime)
            rig.head.midRotation.callback.evaluate = nullptr;
    }
    if ((state.seqFlags & MSF_TargetFacing) && (state.regionFlags[1] & (MRF_Independent | MRF_BlendToLowerBody)) == 0x100U) {
        const float rate = state.chestRotationOverrideBlendTime;
        BindCallback(rig.chest.rotation.callback, cbEvaluateChestOverride, &work->rotations[11]);
        const float blend = shenGetScaledDeltaTickTime(rate) + state.chestBlendScale;
        state.chestBlendScale = blend;
        if (blend > 1.0f)
            state.chestBlendScale = 1.0f;
    } else {
        const float blend = state.chestBlendScale - shenGetScaledDeltaTickTime(state.chestRotationOverrideBlendTime);
        state.chestBlendScale = blend;
        if (0.0f > blend)
            state.chestBlendScale = 0.0f;
        if ((!(state.regionFlags[1] & MRF_Transition) || state.currentRootMode != 2) && 0.00000011920929f > state.chestBlendScale)
            rig.chest.rotation.callback.evaluate = nullptr;
    }
    InstallActiveOverrideCallbacks(work);
    return 1;
}


// @verified - p
std::uint32_t HasLeadFootCommand(std::uint16_t motion) {
    return FindLoadedMotionSecondaryEntry(motion, 19).empty() ? 0U : 0x20U;
}

// @verified - p
void mtwkApplyRootPos(MotionWork *work) {
    const MotionRuntimeState &state = work->runtime;
    Matrix4x4::preTranslate(state.rootPosition.x, state.rootPosition.y, state.rootPosition.z);
    if (state.entityRotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(state.entityRotation.z));
    if (state.entityRotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.entityRotation.y));
    if (state.entityRotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(state.entityRotation.x));
}

// Prepares each region's request: retired or missing motions fall back to F001/F085,
// dependent regions follow the base region, and the requested root mode sets the blend
// length and, for modes 4/5, pre-translates the root by the blend's travel.
// @verified - p
void applyRootTranslation(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    g_ikEnabled = 0;
    state.pendingFootstepCue = 0;
    state.rootDelta = {};
    state.previousRootMode = state.currentRootMode;
    if (state.requestedRootMode == 2 && !IsFightActive() && g_mtwkCurrentIsRyo == 0)
        state.requestedRootMode = 1;
    if (state.motnFlags & 0x20000U) {
        Vector4 &root = work->rig.lowerBody.rootPrepassTransform.rows[3];
        root.x = state.rootPosition.x;
        root.y = state.rootPosition.y;
        root.z = state.rootPosition.z;
    }
    for (std::uint32_t region = 0; region < 5; ++region) {
        state.regionFlags[region] &= ~(MRF_Unknown01 | MRF_Unknown02);
        g_motionLayerEvaluated[region] = 0;
        if (state.queuedMotionId[region]) {
            const std::uint16_t *const header = MotionHeader(state.queuedMotionId[region]);
            if (header && header[1] == 0xffffU)
                state.queuedMotionId[region] = 0;
        }
        const std::uint16_t current = state.currentMotionId[region];
        if (!current)
            state.regionFlags[region] |= MRF_Reinitialize;
        bool retiredBase = false;
        if ((region == 0 || (state.regionFlags[region] & MRF_Independent)) && IsRetiredMotionId(current)) {
            retiredBase = region == 0;
            state.regionFlags[region] |= MRF_Reinitialize;
            if (!state.queuedMotionId[region])
                state.queuedMotionId[region] = 0xf001U;
        }
        if (region != 0) {
            const std::uint16_t flags = state.regionFlags[region];
            if (flags & 0x100U) {
                if (((flags & 0x200U) || (flags & 0xc00U) == 0x400U) && !state.queuedMotionId[region]) {
                    if (!retiredBase) {
                        state.queuedMotionId[region] = state.currentMotionId[0];
                        std::uint16_t leadFoot = state.regionFlags[0] & MRF_Mirrored;
                        if (HasLeadFootCommand(state.currentMotionId[0]))
                            leadFoot ^= 0x40U;
                        if (leadFoot)
                            state.regionFlags[region] |= MRF_LeadFoot;
                        std::uint16_t updated = state.regionFlags[region];
                        if (updated & 0x400U) {
                            const std::uint16_t step = state.defaultRegionStepFrames[region];
                            state.regionStepFrames[region] = step ? step : 8U;
                            updated |= 0x800U;
                        } else {
                            state.regionStepFrames[region] = 0;
                            updated = static_cast<std::uint16_t>((updated & 0xfe7fU) | 0x80U);
                        }
                        state.regionFlags[region] = updated;
                        MotionTimePair &base = g_motionRegionClocks[0]->current;
                        state.layerLoopStartTime[region] = base.splitTimeFrames;
                        float start = base.ticksTime;
                        state.pendingLayerStartFrame[region] = start;
                        if (state.regionStepFrames[region]) {
                            start = static_cast<float>(state.regionStepFrames[region]) * shenGetScaledDeltaTickTime(g_motionRegionClocks[0]->playbackSpeed) +
                                state.pendingLayerStartFrame[region];
                            state.pendingLayerStartFrame[region] = start;
                        }
                        const float maximum = g_motionRegionClocks[0]->current.maxTime;
                        state.layerEndFrame[region] = maximum;
                        if (start > maximum) {
                            if (state.regionFlags[region] & MRF_Loop) {
                                float wrapped = state.pendingLayerStartFrame[region];
                                if (wrapped > maximum) {
                                    do
                                        wrapped = wrapped - maximum;
                                    while (wrapped > maximum);
                                    state.pendingLayerStartFrame[region] = wrapped;
                                }
                            } else {
                                state.pendingLayerStartFrame[region] = maximum;
                            }
                        }
                        SetMotionSpeed(work, g_motionRegionClocks[0]->playbackSpeed, static_cast<MotionRegion>(region));
                    } else {
                        state.regionFlags[region] = flags & 0xfeffU;
                    }
                }
            } else {
                state.regionFlags[region] = flags & 0xf9ffU;
            }
            const std::uint16_t dependent = state.regionFlags[region];
            if ((dependent & 0x900U) != 0x100U) {
                if ((!state.queuedMotionId[region] || (dependent & 0xa00U)) && state.queuedMotionId[0]) {
                    state.queuedMotionId[region] = state.queuedMotionId[0];
                    state.layerLoopStartTime[region] = state.layerLoopStartTime[0];
                    state.pendingLayerStartFrame[region] = state.pendingLayerStartFrame[0];
                    state.layerEndFrame[region] = state.layerEndFrame[0];
                    std::uint16_t copied = (state.regionFlags[0] & MRF_LeadFoot) ? static_cast<std::uint16_t>(state.regionFlags[region] | MRF_LeadFoot) :
                                                                                   static_cast<std::uint16_t>(state.regionFlags[region] & ~MRF_LeadFoot);
                    state.regionFlags[region] = copied;
                    if (state.regionFlags[0] & MRF_Reinitialize) {
                        copied |= 0x80U;
                        state.regionFlags[region] = copied;
                    } else {
                        state.defaultRegionStepFrames[region] = state.defaultRegionStepFrames[0];
                    }
                    state.regionFlags[region] = static_cast<std::uint16_t>((copied & 0xf5ffU) | 0x200U);
                }
                g_motionRegionClocks[region]->playbackSpeed = g_motionRegionClocks[0]->playbackSpeed;
            }
            if (!state.queuedMotionId[region])
                continue;
        } else if (!current) {
            if (!state.queuedMotionId[region])
                state.queuedMotionId[region] = 0xf085U;
        } else if (!state.queuedMotionId[region]) {
            continue;
        }
        std::int32_t rootMode;
        if ((state.regionFlags[region] & (MRF_Independent | MRF_BlendToLowerBody)) == 0x900U) {
            rootMode = 1;
        } else {
            rootMode = state.requestedRootMode;
            state.regionFlags[region] &= ~MRF_BlendToLowerBody;
        }
        if (rootMode == 0 || (state.regionFlags[region] & MRF_Reinitialize)) {
            state.regionStepFrames[region] = 0;
        } else if (rootMode == 4) {
            const float rate = shenGetScaledDeltaTickTime(g_motionRegionClocks[region]->playbackSpeed) >= 0.00000011920929f ?
                shenGetScaledDeltaTickTime(g_motionRegionClocks[region]->playbackSpeed) :
                1.0f;
            const MotionTimePair &pair = g_motionRegionClocks[region]->current;
            state.regionStepFrames[region] = static_cast<std::uint16_t>(static_cast<std::int64_t>(ceil_float((pair.maxTime - pair.ticksTime) / rate)));
        } else {
            const std::uint16_t step = state.defaultRegionStepFrames[region];
            state.regionStepFrames[region] = step ? step : 8U;
        }
        state.currentRootMode = state.requestedRootMode;
        if (region != 0)
            continue;
        if (rootMode == 4) {
            Vector3 travel;
            if (!(state.regionFlags[region] & MRF_Transition) || state.currentRootMode == 2) {
                SampleNativeMotionRoot(state.currentMotionId[region], work->rig.boneId, g_motionRegionClocks[region]->current.maxTime, travel);
            } else {
                travel.x = work->rig.lowerBody.pelvisPosition[0].channel.next.value;
                travel.z = work->rig.lowerBody.pelvisPosition[2].channel.next.value;
            }
            const Vector4 &pelvis = work->rig.lowerBody.pelvisTransform.rows[3];
            const float x = travel.x + pelvis.x;
            const float z = travel.z + pelvis.z;
            if (state.motnFlags & 1U)
                Matrix4x4::preTranslate(x, 0.0f, z);
        } else if (rootMode == 5) {
            const float delta = shenGetScaledDeltaTickTime(g_motionRegionClocks[region]->playbackSpeed);
            const MotionTimePair &pair = g_motionRegionClocks[region]->current;
            const float advanced = static_cast<float>(state.regionStepFrames[region]) * delta + pair.ticksTime;
            const float time = pair.maxTime < advanced ? pair.maxTime : advanced;
            float x, z;
            if (!(state.regionFlags[region] & MRF_Transition) || state.currentRootMode == 2) {
                Vector3 travel;
                SampleNativeMotionRoot(state.currentMotionId[region], work->rig.boneId, time, travel);
                x = travel.x;
                z = travel.z;
            } else if (!(time > pair.maxTime)) {
                g_motionCurrentAnimTime = time * motionFrameSeconds;
                x = SampleMotionRigChannelAdvance(work->rig.lowerBody.pelvisPosition[0].channel);
                z = SampleMotionRigChannelAdvance(work->rig.lowerBody.pelvisPosition[2].channel);
            } else {
                x = work->rig.lowerBody.pelvisPosition[0].channel.next.value;
                z = work->rig.lowerBody.pelvisPosition[2].channel.next.value;
            }
            const Vector4 &pelvis = work->rig.lowerBody.pelvisTransform.rows[3];
            const float translatedX = x + pelvis.x;
            const float translatedZ = z + pelvis.z;
            if (state.motnFlags & 1U)
                Matrix4x4::preTranslate(translatedX, 0.0f, translatedZ);
        }
    }
}

// Promotes each queued motion: resolves its stream, retires and registers the region's prop
// tracks, chooses the transition length and start frame, then reads the motion into the rig.
// @verified - p
std::int32_t MOTN_ProcessLayers(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    std::int32_t processed = 0;
    for (std::uint32_t region = 0; region < 5; ++region) {
        if (!state.queuedMotionId[region]) {
            state.regionFlags[region] &= ~(MRF_LeadFoot | MRF_Reinitialize | MRF_FollowLowerBody | MRF_ReturnToLowerBody);
            continue;
        }
        processed = 1;
        bool wasTracked = false;
        std::int32_t rootMode = 0;
        const std::uint16_t *header = MotionHeader(state.queuedMotionId[region]);
        if (!header) {
            state.queuedMotionId[region] = work->rig.useOverride ? 0xf0ebU : 0xf001U;
            header = MotionHeader(state.queuedMotionId[region]);
        }
        if (work->rig.useOverride != static_cast<std::uint32_t>(static_cast<std::uint8_t>(header[1]) == 16U)) {
            state.queuedMotionId[region] = work->rig.useOverride ? 0xf0ebU : 0xf001U;
            header = MotionHeader(state.queuedMotionId[region]);
        }
        state.previousRegionTrack[region] = state.regionTrack[region];
        RetireMotionPropTrack(state.regionTrack[region], region);
        state.regionPreviousTicksTime[region] =
            (state.regionFlags[region] & MRF_Transition) ? state.layerSampleFrame[region] : g_motionRegionClocks[region]->current.ticksTime;
        void **const armTrack = region == 3 ? &state.rightArmTrack : region == 4 ? &state.leftArmTrack : nullptr;
        MotionStreamContext context;
        UpdatePointersForCurrentFrame(&context, header, armTrack, &state.regionTrack[region]);
        RegisterMotionPropTrack(state.regionTrack[region], region);
        std::uint32_t flags = motionInitStream | motionInitChannels;
        const std::uint16_t leadFoot = FindLoadedMotionSecondaryEntry(state.queuedMotionId[region], 19).empty() ? 0U : 0x20U;
        std::uint16_t regionFlags = state.regionFlags[region];
        if ((regionFlags & MRF_LeadFoot) == leadFoot) {
            regionFlags &= ~MRF_Mirrored;
        } else {
            flags = motionInitStream | motionInitMirror | motionInitChannels;
            regionFlags |= MRF_Mirrored;
        }
        state.regionFlags[region] = regionFlags;
        state.layerSampleFrame[region] = -1.0f;
        const std::uint16_t step = state.regionStepFrames[region];
        if (step) {
            flags |= ((static_cast<std::uint32_t>(step) << 8) + 0x100U) & motionInitGraphFrames;
            state.blendTotalFrames[region] = step & 0xffU;
        } else {
            state.blendTotalFrames[region] = 0;
        }
        state.blendRemainingFrames[region] = state.blendTotalFrames[region];
        if (flags & motionInitGraphFrames) {
            const std::uint16_t previous = state.regionFlags[region];
            state.regionFlags[region] = previous | 0x10U;
            wasTracked = (previous >> 4) & 1U;
        } else {
            state.regionFlags[region] &= ~MRF_Transition;
        }
        float loopStart = -1.0f;
        float localTime = 0.0f;
        if (!(state.regionFlags[region] & MRF_Reinitialize)) {
            rootMode = (state.regionFlags[region] & MRF_BlendToLowerBody) ? 1 : state.currentRootMode;
            MotionRigClock *const clock = g_motionRegionClocks[region];
            float stepFrames = static_cast<float>(step);
            if (rootMode == 5) {
                if (shenGetScaledDeltaTickTime(clock->playbackSpeed) * stepFrames + clock->current.ticksTime > clock->current.maxTime) {
                    const float rate =
                        0.00000011920929f > shenGetScaledDeltaTickTime(clock->playbackSpeed) ? 1.0f : shenGetScaledDeltaTickTime(clock->playbackSpeed);
                    stepFrames = stepFrames - ceil_float((clock->current.maxTime - clock->current.ticksTime) / rate);
                } else {
                    stepFrames = 0.0f;
                }
            }
            const float pending = state.pendingLayerStartFrame[region];
            const float lastFrame = static_cast<float>(static_cast<std::int16_t>(context.frameCount) - 1);
            if (rootMode != 3 && rootMode != 5) {
                if (pending >= 0.0f) {
                    state.layerSampleFrame[region] = pending;
                    localTime = lastFrame < pending ? lastFrame : pending;
                }
                state.pendingLayerStartFrame[region] = -1.0f;
            } else {
                float sample;
                if (0.0f > pending) {
                    sample = shenGetScaledDeltaTickTime(clock->playbackSpeed) * stepFrames;
                } else {
                    if (pending > lastFrame)
                        state.pendingLayerStartFrame[region] = pending - lastFrame;
                    if (!(state.motnFlags & 0x4000U))
                        loopStart = state.pendingLayerStartFrame[region];
                    sample = shenGetScaledDeltaTickTime(clock->playbackSpeed) * stepFrames + state.pendingLayerStartFrame[region];
                }
                localTime = sample;
                state.layerSampleFrame[region] = sample;
                if (!(sample > lastFrame)) {
                    state.pendingLayerStartFrame[region] = -1.0f;
                } else if (FindLoadedMotionSecondaryEntry(state.queuedMotionId[region], 24).empty()) {
                    localTime = lastFrame;
                    state.pendingLayerStartFrame[region] = -1.0f;
                } else if (region == 0) {
                    float travelX = 0.0f, travelZ = 0.0f;
                    Vector3 sampled;
                    const double limit = static_cast<double>(lastFrame) - 0.0001;
                    while (static_cast<double>(state.layerSampleFrame[0]) > limit) {
                        state.layerSampleFrame[0] = state.layerSampleFrame[0] - lastFrame;
                        SampleNativeMotionRoot(state.queuedMotionId[0], work->rig.boneId, lastFrame, sampled);
                        travelX = travelX + sampled.x;
                        travelZ = travelZ + sampled.z;
                        if (loopStart >= 0.0f) {
                            SampleNativeMotionRoot(state.queuedMotionId[0], work->rig.boneId, loopStart, sampled);
                            travelX = travelX - sampled.x;
                            travelZ = travelZ - sampled.z;
                        }
                        SampleNativeMotionRoot(state.queuedMotionId[0], work->rig.boneId, 0.0f, sampled);
                        loopStart = -1.0f;
                        travelX = travelX - sampled.x;
                        travelZ = travelZ - sampled.z;
                    }
                    localTime = state.layerSampleFrame[0];
                    SampleNativeMotionRoot(state.queuedMotionId[0], work->rig.boneId, localTime, sampled);
                    travelX = travelX + sampled.x;
                    travelZ = travelZ + sampled.z;
                    if (state.motnFlags & 1U)
                        Matrix4x4::preTranslate(travelX, 0.0f, travelZ);
                    state.pendingLayerStartFrame[region] = -1.0f;
                } else {
                    float wrapped = state.layerSampleFrame[region];
                    if (wrapped > lastFrame) {
                        loopStart = -1.0f;
                        do
                            wrapped = wrapped - lastFrame;
                        while (wrapped > lastFrame);
                        state.layerSampleFrame[region] = wrapped;
                    }
                    localTime = state.layerSampleFrame[region];
                    state.pendingLayerStartFrame[region] = -1.0f;
                }
            }
        }
        if (region == 0) {
            if (state.regionFlags[0] & MRF_Transition) {
                float timer, limit;
                if (wasTracked) {
                    timer = state.transitionTimer;
                    limit = state.transitionTimerLimit;
                } else {
                    timer = g_motionRegionClocks[0]->current.ticksTime;
                    limit = static_cast<float>(MotionFrameCount(state.currentMotionId[0]) - 1);
                }
                const float reset = 0.0f > loopStart ? 0.0f : loopStart;
                state.baseTimeScale = g_motionRegionClocks[0]->playbackSpeed;
                if (rootMode == 3) {
                    state.transitionTimer = reset;
                    state.transitionTimerReset = reset;
                    state.transitionTimerLimit =
                        static_cast<float>(state.blendTotalFrames[0]) * shenGetScaledDeltaTickTime(g_motionRegionClocks[0]->playbackSpeed) + reset;
                    state.motnFlags |= 0x4000U;
                } else if (rootMode == 4) {
                    state.transitionTimer = timer;
                    state.transitionTimerReset = timer;
                    state.transitionTimerLimit = limit;
                } else if (rootMode == 5) {
                    state.transitionTimerReset = reset;
                    state.transitionTimer = timer;
                    state.transitionTimerLimit = limit;
                }
            } else if (rootMode == 3) {
                state.motnFlags |= 0x4000U;
            }
            if (0.0f > state.layerSampleFrame[0]) {
                if (state.motnFlags & 0x2000U)
                    flags |= motionInitTranslation;
            } else if (!(state.motnFlags & 0x4000U)) {
                flags |= motionInitTranslation;
            }
        }
        if (state.overrideBlendTime > 0.0f && (region == 0 || (state.regionFlags[region] & (MRF_Independent | MRF_FollowLowerBody)) == 0x200U))
            SetOverrideBlendTime(state.overrideBlendTime);
        if (rootMode == 2) {
            Rig_SetupMotionGraph(work->rig, &context, localTime, flags, region, work->targets, work->rotations);
        } else {
            if (region == 0 && (state.motnFlags & 0x8000U)) {
                flags |= motionInitSplitWindow;
                if (state.motnFlags & motionInitBaking)
                    flags |= motionInitSwapLeadFoot;
            }
            Rig_EvaluateControllers(work, &context, loopStart, localTime, flags, static_cast<MotionRegion>(region));
        }
        state.previousMotionId[region] = state.currentMotionId[region];
        state.currentMotionId[region] = state.queuedMotionId[region];
        state.queuedMotionId[region] = 0;
        if (region == 0) {
            Vector4 &pelvis = work->rig.lowerBody.pelvisTransform.rows[3];
            pelvis.x = 0.0f;
            pelvis.y = 0.0f;
            pelvis.z = 0.0f;
            if (state.regionFlags[0] & MRF_Transition) {
                if (state.currentRootMode == 2) {
                    if (0.0f > state.layerSampleFrame[0]) {
                        if (state.motnFlags & 0x2000U)
                            TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
                    } else if (!(state.motnFlags & 0x4000U)) {
                        TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
                    }
                }
            } else {
                const float start = state.layerSampleFrame[0];
                if (0.0f > start) {
                    if (state.motnFlags & 0x2000U)
                        TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
                } else if (!(state.motnFlags & 0x4000U)) {
                    TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
                    Vector3 end, begin;
                    SampleNativeMotionRoot(state.currentMotionId[0], work->rig.boneId, start, end);
                    float scale = 1.0f;
                    if (shenGetScaledDeltaTickTime(g_motionRegionClocks[0]->playbackSpeed) > start) {
                        SampleNativeMotionRoot(state.currentMotionId[0], work->rig.boneId, 0.0f, begin);
                        if (!(0.00000011920929f > start))
                            scale = shenGetScaledDeltaTickTime(g_motionRegionClocks[0]->playbackSpeed) / start;
                    } else {
                        SampleNativeMotionRoot(
                            state.currentMotionId[0], work->rig.boneId, start - shenGetScaledDeltaTickTime(g_motionRegionClocks[0]->playbackSpeed), begin);
                    }
                    end.x = end.x - begin.x;
                    end.z = end.z - begin.z;
                    pelvis.x = end.x * scale + pelvis.x;
                    pelvis.z = end.z * scale + pelvis.z;
                }
            }
            state.motnFlags &= ~0x2000U;
            if (state.gaitSplitFraction >= 0.0f)
                SetGaitSplitFraction(state.gaitSplitFraction);
        } else if ((state.regionFlags[region] & (MRF_Independent | MRF_FollowLowerBody)) != 0x200U) {
            state.regionFlags[region] |= MRF_Independent;
        }
        state.regionFlags[region] |= MRF_Unknown01;
        if (state.currentRootMode == 2 || !(state.regionFlags[region] & MRF_Transition))
            g_motionLayerEvaluated[region] = 1;
        state.regionFlags[region] &= ~(MRF_LeadFoot | MRF_Reinitialize | MRF_FollowLowerBody | MRF_ReturnToLowerBody);
    }
    if (processed)
        state.overrideBlendTime = -1.0f;
    return processed;
}

// Steps each region's clock: tracked transitions run a fixed frame, the base region's
// wrap or reset re-reads the motion and carries the root travel, dependent regions follow
// the base clock, and transition blends count down.
// @verified - p
void mtwkAdvanceAnimFrameAndLayers(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    if (state.overrideBlendTime > 0.0f)
        Rig_RunOverrideBlendPass(work);
    std::uint32_t result = 0;
    if (!g_motionLayerEvaluated[0]) {
        const std::uint16_t flags = state.regionFlags[0];
        if ((flags & 0x10U) && (state.currentRootMode & 0xfdU)) {
            result = AnimClock_ForceJump(g_motionRegionClocks[0]);
        } else if ((flags & 0x10U) && state.currentRootMode == 0) {
            result = 4;
            g_motionRegionClocks[0]->current.ticksTime = g_motionRegionClocks[0]->current.maxTime;
        } else {
            std::uint32_t allowReset = flags & 8U;
            std::uint32_t allowOverflow = flags & 4U;
            if (state.motnFlags & 0x200U)
                allowReset = allowOverflow = 0;
            result = TimestepUpdate(g_motionRegionClocks[0], allowOverflow, allowReset);
        }
        if (result) {
            const std::uint16_t updated = state.regionFlags[0];
            if (((updated & 0x10U) && (state.currentRootMode & 0xfdU)) || ((updated & 0x10U) && state.currentRootMode == 2)) {
                state.regionFlags[0] = updated & 0xffefU;
                state.transitionTimer = -1.0f;
            }
        }
    }
    bool reread = false;
    Matrix4x4 channels;
    Vector4 &pelvis = work->rig.lowerBody.pelvisTransform.rows[3];
    if (result == 4) {
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.maxTime, motionInterpolationHermiteAuto);
        const float endX = channels.rows[3].x + pelvis.x;
        const float endZ = channels.rows[3].z + pelvis.z;
        g_motionRegionClocks[0]->current.ticksTime = 0.0f;
        Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, MotionRegion::LowerBody);
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, motionInterpolationHermiteAuto);
        state.regionFlags[0] |= MRF_Unknown01;
        pelvis.x = -(channels.rows[3].x - endX);
        pelvis.z = -(channels.rows[3].z - endZ);
        reread = true;
    } else if (result == 3) {
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, ceil_float(g_motionRegionClocks[0]->current.maxTime), motionInterpolationHermiteAuto);
        const float endX = channels.rows[3].x + pelvis.x;
        const float endZ = channels.rows[3].z + pelvis.z;
        Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, MotionRegion::LowerBody);
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, motionInterpolationHermiteAuto);
        pelvis.x = -(channels.rows[3].x - endX);
        pelvis.z = -(channels.rows[3].z - endZ);
        reread = true;
    } else if (result == 2) {
        Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, MotionRegion::LowerBody);
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, motionInterpolationHermiteAuto);
        pelvis.x = -channels.rows[3].x;
        pelvis.z = -channels.rows[3].z;
        reread = true;
    }
    if (state.blendRemainingFrames[0] && state.currentRootMode == 2) {
        const std::uint16_t remaining = static_cast<std::uint16_t>(state.blendRemainingFrames[0] - 1U);
        state.blendRemainingFrames[0] = remaining;
        state.layerBlendAlpha[0] = 1.0f - static_cast<float>(remaining) / static_cast<float>(state.blendTotalFrames[0]);
        if (!remaining) {
            ClearOverrideCallbacksForBodyRegion(work, MotionRegion::LowerBody);
            state.regionFlags[0] &= ~MRF_Transition;
            state.blendTotalFrames[0] = 0;
        }
    } else {
        state.layerBlendAlpha[0] = 1.0f;
        if (state.previousRootMode == 2 && state.currentRootMode != 2)
            ClearOverrideCallbacksForBodyRegion(work, MotionRegion::LowerBody);
    }
    for (std::uint32_t region = 1; region < 5; ++region) {
        std::uint16_t &flags = state.regionFlags[region];
        MotionRigClock *const clock = g_motionRegionClocks[region];
        if ((flags & 0x100U) && !g_motionLayerEvaluated[region]) {
            std::uint32_t step = 0;
            bool advanced = true;
            if ((flags & 0x10U) && (state.currentRootMode & 0xfdU)) {
                const float time = shenGetScaledDeltaTickTime(1.0f) + clock->current.ticksTime;
                const bool running = time < clock->current.maxTime;
                clock->current.ticksTime = time;
                if (running)
                    advanced = false;
                else
                    step = 4;
            } else if ((flags & 0x10U) && state.currentRootMode == 0) {
                step = 4;
                clock->current.ticksTime = clock->current.maxTime;
            } else {
                std::uint32_t allowReset = flags & 8U;
                std::uint32_t allowOverflow = flags & 4U;
                if (state.motnFlags & 0x200U)
                    allowReset = allowOverflow = 0;
                step = TimestepUpdate(clock, allowOverflow, allowReset);
            }
            if (advanced) {
                bool evaluate = step > 1;
                if (step && (flags & 0x10U)) {
                    if (state.currentRootMode & 0xfdU) {
                        flags &= 0xffefU;
                        if (step == 4) {
                            flags |= 1U;
                            if (flags & 0x800U)
                                flags &= 0xf6ffU;
                            evaluate = true;
                        }
                    } else if (state.currentRootMode == 0) {
                        flags = static_cast<std::uint16_t>((flags & 0xffeeU) | 1U);
                        if (flags & 0x800U)
                            flags &= 0xf6ffU;
                    }
                }
                if (evaluate) {
                    clock->current.ticksTime = 0.0f;
                    Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
                }
            }
        } else {
            clock->current.ticksTime = g_motionRegionClocks[0]->current.ticksTime;
            if (reread) {
                Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
                if (state.regionFlags[0] & MRF_Unknown01)
                    flags |= 1U;
            }
            if (!(state.regionFlags[0] & MRF_Transition))
                flags &= 0xffefU;
        }
        if (state.blendRemainingFrames[region] && state.currentRootMode == 2) {
            const std::uint16_t remaining = static_cast<std::uint16_t>(state.blendRemainingFrames[region] - 1U);
            state.blendRemainingFrames[region] = remaining;
            state.layerBlendAlpha[region] = 1.0f - static_cast<float>(remaining) / static_cast<float>(state.blendTotalFrames[region]);
            if (!state.blendRemainingFrames[region]) {
                ClearOverrideCallbacksForBodyRegion(work, static_cast<MotionRegion>(region));
                state.blendTotalFrames[region] = 0;
                flags &= 0xffefU;
            }
            if (flags & 0x100U)
                state.layerBlendAlpha[region] = 1.0f;
        } else {
            state.layerBlendAlpha[region] = 1.0f;
            if (state.previousRootMode == 2 && state.currentRootMode != 2)
                ClearOverrideCallbacksForBodyRegion(work, static_cast<MotionRegion>(region));
        }
    }
    if (state.transitionTimer >= 0.0f) {
        const float timer = shenGetScaledDeltaTickTime(state.baseTimeScale) + state.transitionTimer;
        state.transitionTimer = timer;
        if (timer > state.transitionTimerLimit) {
            state.transitionTimer = state.transitionTimerReset;
            state.previousMotionId[0] = state.currentMotionId[0];
        }
    }
}



// @verified - p
void mtwkApplySequenceCommands(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    float holdTime = 0.0f;
    for (std::uint32_t region = 0; region < 5; ++region) {
        std::uint16_t &flags = state.regionFlags[region];
        if ((flags & 1U) && !((flags & 0x10U) && state.currentRootMode != 2)) {
            const std::span<const std::byte> loop = FindLoadedMotionSecondaryEntry(state.currentMotionId[region], 1);
            std::uint16_t endless = 0;
            if (loop.size() >= 4)
                std::memcpy(&endless, loop.data() + 2, sizeof(endless));
            if (!loop.empty() && endless == 0xffffU)
                flags |= 4U;
            else
                flags = (flags & 0x1000U) ? static_cast<std::uint16_t>(flags | 4U) : static_cast<std::uint16_t>(flags & 0xfffbU);
            if (FindLoadedMotionSecondaryEntry(state.currentMotionId[region], 24).empty()) {
                flags &= 0xfff7U;
            } else {
                flags |= 8U;
                MotionRigClock *clocks[5];
                mtwkCopyRigFlags(work, &clocks[0], &clocks[1], &clocks[2], &clocks[3], &clocks[4]);
                holdTime = clocks[region]->current.maxTime;
                TimePair_ClampAndPropagate(work, -1.0f, -1.0f, holdTime - 0.001f, static_cast<MotionRegion>(region));
            }
        }
        if ((flags & 0x10U) && state.currentRootMode != 2)
            continue;
        const float loopStart = state.layerLoopStartTime[region];
        if (!(loopStart >= 0.0f || state.pendingLayerStartFrame[region] >= 0.0f || state.layerSampleFrame[region] >= 0.0f ||
                state.layerEndFrame[region] >= 0.0f))
            continue;
        const float previousTicks = g_motionRegionClocks[region]->current.ticksTime;
        if (state.layerSampleFrame[region] >= 0.0f)
            state.pendingLayerStartFrame[region] = state.layerSampleFrame[region];
        AssignStaticDataForBodyRegion(work, loopStart, state.pendingLayerStartFrame[region], state.layerEndFrame[region], static_cast<MotionRegion>(region));
        const float current = TimePair_ClampAndPropagate(
            work, state.layerLoopStartTime[region], state.pendingLayerStartFrame[region], state.layerEndFrame[region], static_cast<MotionRegion>(region));
        if (previousTicks > current && !(flags & 1U)) {
            Rig_EvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
            TimePair_ClampAndPropagate(work, -1.0f, current, -1.0f, static_cast<MotionRegion>(region));
        }
        if (region == 0) {
            if (state.pendingLayerStartFrame[0] > 0.0f) {
                g_motionLayerEvaluated[0] = 1;
                Vector4 &pelvis = work->rig.lowerBody.pelvisTransform.rows[3];
                if (state.motnFlags & 0x4000U) {
                    pelvis.x = 0.0f;
                    pelvis.y = 0.0f;
                    pelvis.z = 0.0f;
                } else if (state.regionFlags[0] & MRF_Unknown01) {
                    const float x = pelvis.x;
                    const float z = pelvis.z;
                    Matrix4x4 channels;
                    BuildChannelTransformAtTime(
                        work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, motionInterpolationHermiteAuto);
                    const float translatedX = x + channels.rows[3].x;
                    const float translatedZ = z + channels.rows[3].z;
                    TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
                    pelvis.x = translatedX + pelvis.x;
                    pelvis.z = translatedZ + pelvis.z;
                } else {
                    const float alpha = state.layerBlendAlpha[0];
                    const float earlier = current - shenGetScaledDeltaTickTime(work->rig.lowerBody.clock.playbackSpeed);
                    TimePair_ClampAndPropagate(work, -1.0f, earlier > 0.0f ? earlier : 0.0f, -1.0f, MotionRegion::LowerBody);
                    if (state.regionFlags[0] & MRF_Transition)
                        state.layerBlendAlpha[0] = 1.0f - static_cast<float>(state.blendRemainingFrames[0] + 1) / static_cast<float>(state.blendTotalFrames[0]);
                    TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
                    TimePair_ClampAndPropagate(work, -1.0f, current, -1.0f, MotionRegion::LowerBody);
                    state.layerBlendAlpha[0] = alpha;
                }
            }
            for (std::uint32_t dependent = 1; dependent < 5; ++dependent)
                if (!(state.regionFlags[dependent] & MRF_Independent))
                    g_motionRegionClocks[dependent]->current.ticksTime = current;
        }
        state.layerLoopStartTime[region] = -1.0f;
        state.pendingLayerStartFrame[region] = -1.0f;
        state.layerEndFrame[region] = -1.0f;
        state.layerSampleFrame[region] = -1.0f;
    }
    state.motnFlags &= ~0x4000U;
    if (state.motnFlags & 0x200U) {
        Vector4 &pelvis = work->rig.lowerBody.pelvisTransform.rows[3];
        pelvis.x = 0.0f;
        pelvis.y = 0.0f;
        pelvis.z = 0.0f;
    }
    if (state.regionFlags[0] & MRF_Unknown01)
        state.activeRootYawOffset = state.requestedRootYawOffset;
    if (state.activeRootYawOffset && !(state.regionFlags[0] & MRF_Transition)) {
        const MotionTimePair &pair = g_motionRegionClocks[0]->current;
        const float elapsed = pair.ticksTime - pair.splitTimeFrames;
        const float span = (pair.maxTime - pair.splitTimeFrames) * 0.5f;
        const std::int16_t yaw = static_cast<std::int16_t>(state.activeRootYawOffset);
        std::int32_t angle;
        if (span > elapsed)
            angle = TruncateToInt32((static_cast<float>(static_cast<std::int16_t>(yaw / 2)) * elapsed) / span);
        else
            angle = static_cast<std::int16_t>(TruncateToInt32(static_cast<float>(yaw) - (((span + span) - elapsed) * static_cast<float>(yaw)) / (span + span)));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angle));
    }
}

// Translates every solved world matrix and target output by the root correction.
// @verified - p
void motnAdvanceInterpolated(MotionWork *work, float x, float y, float z) {
    IKRig &rig = work->rig;
    MotionBaseRig &base = rig.lowerBody;
    TranslateMatrix(base.pelvisWorld, x, y, z);
    TranslateMatrix(base.hipsWorld, x, y, z);
    TranslateMatrix(base.leftThighWorld, x, y, z);
    TranslateMatrix(base.leftShinWorld, x, y, z);
    TranslateMatrix(base.leftAnkleWorld, x, y, z);
    TranslateMatrix(base.rightThighWorld, x, y, z);
    TranslateMatrix(base.rightShinWorld, x, y, z);
    TranslateMatrix(base.rightAnkleWorld, x, y, z);
    TranslateMatrix(rig.chest.upperChestWorld, x, y, z);
    TranslateMatrix(rig.head.neckWorld, x, y, z);
    TranslateMatrix(rig.head.headWorld, x, y, z);
    for (MotionArmRig *arm : {&rig.rightArm, &rig.leftArm}) {
        TranslateMatrix(arm->shoulderWorld, x, y, z);
        TranslateMatrix(arm->upperArmWorld, x, y, z);
        TranslateMatrix(arm->forearmWorld, x, y, z);
        TranslateMatrix(arm->handWorld, x, y, z);
    }
    if (rig.useOverride) {
        TranslateMatrix(*base.leftHipWorld, x, y, z);
        TranslateMatrix(*base.rightHipWorld, x, y, z);
        TranslateMatrix(*base.rightToe1World, x, y, z);
        TranslateMatrix(*base.rightToe2World, x, y, z);
        TranslateMatrix(*base.rightToe3World, x, y, z);
        TranslateMatrix(*rig.head.aimSourceWorld, x, y, z);
        TranslateMatrix(*rig.head.nodeAWorld, x, y, z);
        TranslateMatrix(*rig.head.nodeBWorld, x, y, z);
        TranslateMatrix(*rig.head.nodeCWorld, x, y, z);
    } else {
        TranslateMatrix(rig.chest.chestWorld, x, y, z);
        TranslateMatrix(rig.rightArm.wristWorld, x, y, z);
        TranslateMatrix(rig.leftArm.wristWorld, x, y, z);
        TranslateMatrix(base.leftFootWorld, x, y, z);
        TranslateMatrix(base.rightFootWorld, x, y, z);
    }
}

// @verified - p
void motnCopyInterpolated(MotionWork *work, float x, float y, float z) {
    IKRig &rig = work->rig;
    TranslateTarget(rig.chest.target, x, y, z);
    TranslateTarget(rig.head.target, x, y, z);
    TranslateTarget(rig.rightArm.target, x, y, z);
    TranslateTarget(rig.leftArm.target, x, y, z);
    TranslateTarget(rig.lowerBody.leftFoot, x, y, z);
    TranslateTarget(rig.lowerBody.rightFoot, x, y, z);
    if (rig.useOverride) {
        TranslateTarget(*rig.lowerBody.overridePelvisTarget, x, y, z);
        TranslateTarget(*rig.head.aimTargetNode, x, y, z);
    }
}

// The solved pelvis yaw: the facing of its -Z axis, or of its Y axis when the pelvis
// pitches more than 0.6 out of the ground plane.
// @verified - p
std::uint32_t mtwkApplyTransforms(const Matrix4x4 *pelvis) {
    if (!pelvis)
        return 0;
    Matrix4x4 basis = *pelvis;
    basis.rows[3].x = 0.0f;
    basis.rows[3].y = 0.0f;
    basis.rows[3].z = 0.0f;
    Matrix4x4 scratch = basis;
    Vector3 axis{0.0f, 0.0f, -1.0f};
    Vector3 facing;
    Matrix4x4_Push(&scratch);
    Matrix4x4::TransformPoint(axis, facing);
    Matrix4x4::GetTransformMatrix(&scratch);
    Matrix4x4::Pop(1);
    if (std::fabs(facing.y) > 0.6f) {
        axis = {0.0f, 0.0f > facing.y ? 1.0f : -1.0f, 0.0f};
        Matrix4x4_Push(&scratch);
        Matrix4x4::TransformPoint(axis, facing);
        Matrix4x4::GetTransformMatrix(&scratch);
        Matrix4x4::Pop(1);
    }
    return FixedAtan2(-facing.x, -facing.z);
}

// The F005-relative footstep cue for walk-family motions: opcode 4 is looked up in F005 at
// the frames scaled by the two motions' lengths, and the remap flag selects the stair cues.
// @verified - p
void resolveAnimId(MotionWork *work, std::uint16_t motion, std::uint16_t remap, float begin, float end) {
    const std::int32_t walkFrames = MotionFrameCount(0xf005U);
    const std::int32_t frames = MotionFrameCount(motion);
    const float ratio = static_cast<float>(walkFrames - 1) / static_cast<float>(frames - 1);
    const std::span<const std::byte> command = FindLoadedMotionTimedCommand(0xf005U, 4, ratio * begin, ratio * end, 1);
    MotionRuntimeState &state = work->runtime;
    if (command.empty()) {
        state.pendingFootstepCue = 0;
        return;
    }
    state.materialFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(command[3])));
    std::memcpy(&state.pendingFootstepCue, command.data() + 4, sizeof(state.pendingFootstepCue));
    if (!remap)
        return;
    if (state.pendingFootstepCue == 0x800001U)
        state.pendingFootstepCue = 0x800004U;
    else if (state.pendingFootstepCue == 0x800000U)
        state.pendingFootstepCue = 0x800003U;
}

// @verified - p
void mtwkUpdateBoneForTime(MotionWork *work, std::uint16_t motion, float begin, float end) {
    const std::span<const std::byte> command = FindLoadedMotionTimedCommand(motion, 4, begin, end, 1);
    MotionRuntimeState &state = work->runtime;
    if (command.empty()) {
        state.pendingFootstepCue = 0;
        return;
    }
    state.materialFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(command[3])));
    std::memcpy(&state.pendingFootstepCue, command.data() + 4, sizeof(state.pendingFootstepCue));
}


// @verified - p
void mtwkSelectRandomFootstepCue(MotionWork *work, std::uint16_t motion, float begin, float end) {
    const std::span<const std::byte> command = FindLoadedMotionTimedCommand(motion, 49, begin, end, 1);
    MotionRuntimeState &state = work->runtime;
    if (command.empty()) {
        state.pendingFootstepCue = 0;
        return;
    }
    state.materialFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(command[3])));
    const std::int32_t index = TruncateToInt32(IncrementSharedPrngFloat() * 3.0f);
    std::memcpy(&state.pendingFootstepCue, command.data() + 4 + 4 * static_cast<std::ptrdiff_t>(index), sizeof(state.pendingFootstepCue));
}


// @verified - p
void UpdateMotionCollisionCluster(MotionWork *work) {
    IKRig &rig = work->rig;
    const std::uint8_t count = rig.useOverride ? 9U : 7U;
    Vector3 points[9]{};
    points[0] = {rig.lowerBody.pelvisWorld.rows[3].x, rig.lowerBody.pelvisWorld.rows[3].y, rig.lowerBody.pelvisWorld.rows[3].z};
    points[1] = TargetOutput(rig.chest.target);
    GetMotionBonePosition(work, MB_Head, &points[2]);
    points[3] = TargetOutput(rig.rightArm.target);
    points[4] = TargetOutput(rig.leftArm.target);
    points[5] = TargetOutput(rig.lowerBody.leftFoot);
    points[6] = TargetOutput(rig.lowerBody.rightFoot);
    if (rig.useOverride) {
        points[7] = TargetOutput(*rig.lowerBody.overridePelvisTarget);
        points[8] = TargetOutput(*rig.head.aimTargetNode);
    }
    float sumX = 0.0f, sumZ = 0.0f;
    for (std::uint8_t index = 0; index < count; ++index) {
        sumX = sumX + points[index].x;
        sumZ = sumZ + points[index].z;
    }
    const float centerX = sumX / static_cast<float>(count);
    const float centerZ = sumZ / static_cast<float>(count);
    float spread = 0.0f;
    for (std::uint8_t index = 0; index < count; ++index) {
        const float distance = (points[index].z - centerZ) * (points[index].z - centerZ) + (points[index].x - centerX) * (points[index].x - centerX);
        spread = distance > spread ? distance : spread;
    }
    work->runtime.clusterRadius = std::sqrt(spread);
    work->runtime.clusterCenter = {centerX, 0.0f, centerZ};
}

// Publishes this frame's root: pelvis yaw and position, footstep cues across the base
// clock's interval, environment flags, and the cleared one-frame requests.
// @verified - p
std::int32_t mtwkApplyInterpolatedTransforms(MotionWork *work) {
    MotionRuntimeState &state = work->runtime;
    const std::uint16_t baseFlags = state.regionFlags[0];
    if (baseFlags & 1U) {
        const float previous = g_motionRegionClocks[0]->current.ticksTime - shenGetScaledDeltaTickTime(g_motionRegionClocks[0]->playbackSpeed);
        state.layerPreviousTicks[0] = previous;
        if (0.0f > previous)
            state.layerPreviousTicks[0] = 0.0f;
    }
    float begin = state.layerPreviousTicks[0];
    for (std::uint32_t region = 0; region < 5; ++region) {
        if (!(state.regionFlags[region] & MRF_Transition) || !(state.currentRootMode & 0xfdU)) {
            MotionRigClock *const clock = g_motionRegionClocks[region];
            if (shenGetScaledDeltaTickTime(clock->playbackSpeed) + clock->current.ticksTime > clock->current.maxTime)
                state.regionFlags[region] |= MRF_Unknown02;
            state.regionFlags[region] &= ~MRF_Unknown1000;
        }
        state.layerPreviousTicks[region] = g_motionRegionClocks[region]->current.ticksTime;
    }
    if (state.activeRootYawOffset && (state.regionFlags[0] & MRF_Unknown02) && (state.regionFlags[0] & (MRF_Loop | MRF_ExcludeEndFrame)))
        state.activeRootYawOffset = 0;
    state.entityRotation.y = static_cast<std::int32_t>(mtwkApplyTransforms(&work->rig.lowerBody.pelvisWorld));
    state.previousRootPosition.x = state.rootPosition.x;
    state.previousRootPosition.z = state.rootPosition.z;
    if ((state.motnFlags & 9U) == 1U) {
        state.rootPosition.x = work->rig.lowerBody.pelvisWorld.rows[3].x;
        state.rootPosition.z = work->rig.lowerBody.pelvisWorld.rows[3].z;
    }
    work->rig.lowerBody.rootPrepassTransform.rows[3].x = state.rootPosition.x;
    work->rig.lowerBody.rootPrepassTransform.rows[3].z = state.rootPosition.z;
    const std::uint16_t tracked = state.regionFlags[0] & MRF_Transition;
    if (g_usingEnv && state.enableEnvironmentMotion && !(tracked && state.currentRootMode == 1)) {
        float end;
        std::uint16_t motion;
        if (!tracked || state.currentRootMode == 2) {
            end = g_motionRegionClocks[0]->current.ticksTime;
            motion = state.currentMotionId[0];
        } else {
            begin = state.transitionTimer - shenGetScaledDeltaTickTime(state.baseTimeScale);
            end = state.transitionTimer;
            motion = state.currentRootMode == 3 ? state.currentMotionId[0] : state.previousMotionId[0];
        }
        std::uint16_t remap = 2;
        if (motion == 0xf006U || (motion > 0xf034U && motion <= 0xf03aU))
            remap = 1;
        else if (motion > 0xf03aU && motion <= 0xf03dU)
            remap = 0;
        if (remap < 2) {
            if (begin > end) {
                resolveAnimId(work, motion, remap, begin, static_cast<float>(MotionFrameCount(motion)) - 1.0f);
                if (!state.pendingFootstepCue)
                    resolveAnimId(work, motion, remap, 0.0f, end);
            } else {
                resolveAnimId(work, motion, remap, begin, end);
            }
        } else if (begin > end) {
            mtwkUpdateBoneForTime(work, motion, begin, g_motionRegionClocks[0]->current.maxTime);
            if (!state.pendingFootstepCue) {
                mtwkSelectRandomFootstepCue(work, motion, begin, g_motionRegionClocks[0]->current.maxTime);
                if (!state.pendingFootstepCue) {
                    mtwkUpdateBoneForTime(work, motion, g_motionRegionClocks[0]->current.splitTimeFrames, end);
                    if (!state.pendingFootstepCue)
                        mtwkSelectRandomFootstepCue(work, motion, g_motionRegionClocks[0]->current.splitTimeFrames, end);
                }
            }
        } else {
            mtwkUpdateBoneForTime(work, motion, begin, end);
            if (!state.pendingFootstepCue)
                mtwkSelectRandomFootstepCue(work, motion, begin, end);
        }
    }
    if (state.regionFlags[0] & MRF_Unknown01)
        state.environmentFlags &= 0x0fU;
    if (state.environmentFlags & 0x0fU)
        state.environmentFlags = static_cast<std::uint8_t>(state.environmentFlags << 4);
    state.environmentFlags &= 0xf0U;
    state.channelFlags = 9;
    for (Vector3 *&target : state.latchedConstraintTarget)
        target = nullptr;
    if (!(state.regionFlags[0] & MRF_Transition))
        state.requestedRootYawOffset = 0;
    state.motnFlags &= ~0x20000U;
    state.gaitSplitFraction = -1.0f;
    UpdateMotionCollisionCluster(work);
    return 1;
}

// The rebase frame for an IK hand target: the right forearm basis re-aimed along the target
// segment, expressed relative to the solved forearm and anchored at the forearm position.
// @verified - p
Matrix4x4 *mtwkBuildArmRebaseTransform(Matrix4x4 *destination, MotionWork *work) {
    Vector3 anchor;
    GetMotionBonePosition(work, MB_RightForearm, &anchor);
    *destination = work->rig.rightArm.forearmWorld;
    const Vector3 *const segment = work->runtime.ikTargetPosition;
    Vector3 axis{segment[1].x - segment[0].x, segment[1].y - segment[0].y, segment[1].z - segment[0].z};
    const float length = Vector3::CalcMagnitude(axis);
    Matrix4x4 &matrix = *destination;
    const float x = axis.x / length, y = axis.y / length, z = axis.z / length;
    const float row2x = matrix.rows[2].x, row2y = matrix.rows[2].y;
    matrix.rows[0].x = x;
    matrix.rows[0].y = y;
    matrix.rows[0].z = z;
    axis = {row2y * z - y * matrix.rows[2].z, x * matrix.rows[2].z - row2x * z, row2x * y - row2y * x};
    matrix.rows[1].x = axis.x;
    matrix.rows[1].z = axis.z;
    matrix.rows[1].y = axis.y;
    float magnitude = Vector3::CalcMagnitude(axis);
    if (magnitude > 0.00000011920929f) {
        matrix.rows[1].x = matrix.rows[1].x / magnitude;
        matrix.rows[1].y = matrix.rows[1].y / magnitude;
        matrix.rows[1].z = matrix.rows[1].z / magnitude;
    }
    axis = {matrix.rows[0].y * matrix.rows[1].z - matrix.rows[0].z * matrix.rows[1].y,
        matrix.rows[0].z * matrix.rows[1].x - matrix.rows[0].x * matrix.rows[1].z, matrix.rows[0].x * matrix.rows[1].y - matrix.rows[0].y * matrix.rows[1].x};
    matrix.rows[2].x = axis.x;
    matrix.rows[2].z = axis.z;
    matrix.rows[2].y = axis.y;
    magnitude = Vector3::CalcMagnitude(axis);
    if (magnitude > 0.00000011920929f) {
        matrix.rows[2].x = matrix.rows[2].x / magnitude;
        matrix.rows[2].y = matrix.rows[2].y / magnitude;
        matrix.rows[2].z = matrix.rows[2].z / magnitude;
    }
    Matrix4x4 inverse;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&work->rig.rightArm.forearmWorld);
    InvertModelWorkTransform();
    Matrix4x4::GetTransformMatrix(&inverse);
    Matrix4x4::SetActiveTransform(destination);
    Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
    Matrix4x4::GetTransformMatrix(destination);
    matrix.rows[3].x = anchor.x;
    matrix.rows[3].z = anchor.z;
    matrix.rows[3].y = anchor.y;
    Matrix4x4::Pop(1);
    return destination;
}

// The MTWK frame: root and clock preparation, layer promotion and stepping, sequence
// commands and override callbacks, the IK solve under the root frame, then the root
// correction, the interpolated publication and any requested rebase.
// @verified - p
std::int32_t mtwkEvaluateMotionFrame(MotionWork *work) {
    if (!work)
        return 0;
    MotionRuntimeState &state = work->runtime;
    Matrix4x4_PushActiveTransform();
    mtwkApplyRootPos(work);
    mtwkCopyRigFlags(work, &g_motionRegionClocks[0], &g_motionRegionClocks[1], &g_motionRegionClocks[2], &g_motionRegionClocks[3], &g_motionRegionClocks[4]);
    applyRootTranslation(work);
    MOTN_ProcessLayers(work);
    mtwkAdvanceAnimFrameAndLayers(work);
    mtwkApplySequenceCommands(work);
    UpdateMotionOverrideCallbacks(work);
    AssignMotionCallbacks(work);
    const std::uint32_t interpolationMode = ((state.seqFlags >> motionSequenceInterpolationShift) & motionInterpolationLinear) | motionInterpolationHermite;
    if ((state.motnFlags & 9U) != 1U) {
        TranslatePelvisFromChannels(work->rig.lowerBody, motionInterpolationHermite);
        if (state.motnFlags & 8U) {
            Matrix4x4 channels;
            BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.ticksTime, motionInterpolationHermiteAuto);
            work->rig.lowerBody.pelvisTransform.rows[3].y = -channels.rows[3].y;
        } else {
            work->rig.lowerBody.pelvisTransform.rows[3].y = 0.0f;
        }
    }
    Vector3 bonePosition;
    if ((state.channelFlags & 0x1fU) < 9U && !(state.motnFlags & 8U))
        GetMotionControllerTargetPosition(work, state.channelFlags & 0x1fU, bonePosition);
    g_ikEnabled = 1;
    IK_BuildAndSolve(work->rig, interpolationMode);
    g_ikEnabled = 0;
    if ((state.channelFlags & 0x1fU) < 9U && !(state.motnFlags & 8U)) {
        Vector3 solved;
        GetMotionControllerTargetPosition(work, state.channelFlags & 0x1fU, solved);
        bonePosition = {bonePosition.x - solved.x, bonePosition.y - solved.y, bonePosition.z - solved.z};
        Matrix4x4_Push();
        InvertModelWorkTransform();
        Matrix4x4::Transform(bonePosition, bonePosition);
        const std::uint8_t channels = state.channelFlags;
        if (channels & 0x80U)
            bonePosition.y = 0.0f;
        if (channels & 0x40U)
            bonePosition.x = 0.0f;
        if (channels & 0x20U)
            bonePosition.z = 0.0f;
        Matrix4x4::Pop(1);
        Matrix4x4::Transform(bonePosition, bonePosition);
        if (!(state.motnFlags & 1U)) {
            bonePosition.x = 0.0f;
            bonePosition.z = 0.0f;
        }
        motnAdvanceInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        motnCopyInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        work->rig.lowerBody.pelvisTransform.rows[3].y = bonePosition.y;
    }
    mtwkApplyInterpolatedTransforms(work);
    Matrix4x4::Pop(1);
    Matrix4x4 rebase;
    if (state.ikTargetPosition) {
        GetMotionBonePosition(work, MB_RightForearm, &bonePosition);
        Matrix4x4 arm;
        rebase = *mtwkBuildArmRebaseTransform(&arm, work);
        state.transformStart = &rebase;
    }
    if (state.transformStart)
        RebaseMotionSolvedMatrices(work);
    if (state.ikTargetPosition) {
        const Vector3 &target = state.ikTargetPosition[0];
        motnAdvanceInterpolated(work, target.x - bonePosition.x, target.y - bonePosition.y, target.z - bonePosition.z);
        state.ikTargetPosition = nullptr;
        state.transformStart = nullptr;
    }
    if (state.seqFlags & MSF_SearchHand)
        return 1;
    return camCtrlIsInsideDepthScaledRect(state.rootPosition, state.depthScale) ? 1 : 0;
}

} // namespace shenmue::s2
