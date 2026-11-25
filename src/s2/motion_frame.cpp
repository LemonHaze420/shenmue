#include "shenmue.h"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#include "s2/motion_work_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/motion_yaw_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/fight.hpp"
#include "s2/math.hpp"
#include <cstring>
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/event_vector_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "common/native_sqrt.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

// @verified - d
void mtwkApplyRootPos(MotionWork *work) {
    Matrix4x4::preTranslate(work->cachedRootPosition.x, work->cachedRootPosition.y, work->cachedRootPosition.z);
    if (work->entityRotationZ)
        Matrix4x4::RotateAroundZ_Wrapper(work->entityRotationZ);
    if (work->entityYaw)
        Matrix4x4::RotateAroundY_Wrapper(work->entityYaw);
    if (work->entityRotationX)
        Matrix4x4::RotateAroundX_Wrapper(work->entityRotationX);
}

namespace {

void TranslateMatrix(Matrix4x4 &matrix, float x, float y, float z) {
    matrix.rows[3].x = x + matrix.rows[3].x;
    matrix.rows[3].y = y + matrix.rows[3].y;
    matrix.rows[3].z = z + matrix.rows[3].z;
}

void TranslateTarget(Vector3 *output, float x, float y, float z) {
    output->x = x + output->x;
    output->y = y + output->y;
    output->z = z + output->z;
}

} // namespace

// @verified - d
void mtwkAdvanceInterpolated(MotionWork *work, float x, float y, float z) {
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.hipsWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftThighWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftShinWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftAnkleWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightThighWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightShinWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightAnkleWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.upperChestWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.neckWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.headWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.shoulderWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.upperArmWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.forearmWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.handWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.shoulderWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.upperArmWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.forearmWorld, x, y, z);
    TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.handWorld, x, y, z);
    if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
        TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.chestWorld, x, y, z);
        TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.wristWorld, x, y, z);
        TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.wristWorld, x, y, z);
        TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftFootWorld, x, y, z);
        TranslateMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightFootWorld, x, y, z);
    } else {
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftHipWorld, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightHipWorld, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe1World, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe2World, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe3World, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimSourceWorld, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeAWorld, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeBWorld, x, y, z);
        TranslateMatrix(*reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeCWorld, x, y, z);
    }
}

// @verified - d
void mtwkCopyInterpolated(MotionWork *work, float x, float y, float z) {
    IKRig &rig = reinterpret_cast<MotionWorkBindings *>(work)->rig;
    TranslateTarget(reinterpret_cast<Vector3 *>(rig.chest.target.axes[2].dimensions), x, y, z);
    TranslateTarget(reinterpret_cast<Vector3 *>(rig.head.target.axes[2].dimensions), x, y, z);
    TranslateTarget(reinterpret_cast<Vector3 *>(rig.rightArm.target.axes[2].dimensions), x, y, z);
    TranslateTarget(reinterpret_cast<Vector3 *>(rig.leftArm.target.axes[2].dimensions), x, y, z);
    TranslateTarget(reinterpret_cast<Vector3 *>(rig.lowerBody.leftFoot.axes[2].dimensions), x, y, z);
    TranslateTarget(reinterpret_cast<Vector3 *>(rig.lowerBody.rightFoot.axes[2].dimensions), x, y, z);
    if (rig.useOverride) {
        TranslateTarget(reinterpret_cast<Vector3 *>(rig.lowerBody.overridePelvisTarget->axes[2].dimensions), x, y, z);
        TranslateTarget(reinterpret_cast<Vector3 *>(rig.head.aimTargetNode->axes[2].dimensions), x, y, z);
    }
}

// @verified - d
void mtwkUpdateCollisionCluster(MotionWork *work) {
    Vector3 points[9];
    float spread = 0.0f;
    Vector3 center = {0.0f, 0.0f, 0.0f};
    std::uint8_t count;
    std::uint8_t index;
    if (reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride)
        count = 9;
    else
        count = 7;
    points[0] = *reinterpret_cast<const Vector3 *>(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisWorld.rows[3]);
    points[1] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.target.axes[2].dimensions);
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(5), &points[2]);
    points[3] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.target.axes[2].dimensions);
    points[4] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.target.axes[2].dimensions);
    points[5] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftFoot.axes[2].dimensions);
    points[6] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightFoot.axes[2].dimensions);
    if (reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
        points[7] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.overridePelvisTarget->axes[2].dimensions);
        points[8] = *reinterpret_cast<const Vector3 *>(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimTargetNode->axes[2].dimensions);
    }
    for (index = 0; index < count; ++index) {
        center.x = center.x + points[index].x;
        center.z = center.z + points[index].z;
    }
    center.x = center.x / static_cast<float>(count);
    center.z = center.z / static_cast<float>(count);
    for (index = 0; index < count; ++index) {
        float distance = points[index].x - center.x;
        const float z = points[index].z - center.z;
        distance = distance * distance + z * z;
        if (distance > spread)
            spread = distance;
    }
    work->clusterRadius = shenmue::NativeSqrt(spread);
    work->clusterCenter = center;
}

Matrix4x4 mtwkBuildArmRebaseTransform(MotionWork *work) {
    Vector3 axis;
    Vector3 direction;
    Vector3 anchor;
    Matrix4x4 inverse;
    Matrix4x4 matrix;
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(12), &anchor);
    dc::sdk::njSetMatrix(&matrix, mtwkGetBoneTransform(work, 12));
    const Vector3 *segment = work->ikTargetPosition;
    axis.x = segment[1].x - segment[0].x;
    axis.y = segment[1].y - segment[0].y;
    axis.z = segment[1].z - segment[0].z;
    const float length = Vector3::CalcMagnitude(axis);
    direction.x = axis.x / length;
    direction.y = axis.y / length;
    direction.z = axis.z / length;
    matrix.rows[0].x = direction.x;
    matrix.rows[0].y = direction.y;
    matrix.rows[0].z = direction.z;
    matrix.rows[1].x = matrix.rows[2].y * matrix.rows[0].z - matrix.rows[2].z * matrix.rows[0].y;
    matrix.rows[1].y = matrix.rows[2].z * matrix.rows[0].x - matrix.rows[2].x * matrix.rows[0].z;
    matrix.rows[1].z = matrix.rows[2].x * matrix.rows[0].y - matrix.rows[2].y * matrix.rows[0].x;
    axis.x = matrix.rows[1].x;
    axis.y = matrix.rows[1].y;
    axis.z = matrix.rows[1].z;
    float magnitude = Vector3::CalcMagnitude(axis);
    if (magnitude > 0.00000011920929f) {
        matrix.rows[1].x = matrix.rows[1].x / magnitude;
        matrix.rows[1].y = matrix.rows[1].y / magnitude;
        matrix.rows[1].z = matrix.rows[1].z / magnitude;
    }
    matrix.rows[2].x = matrix.rows[0].y * matrix.rows[1].z - matrix.rows[0].z * matrix.rows[1].y;
    matrix.rows[2].y = matrix.rows[0].z * matrix.rows[1].x - matrix.rows[0].x * matrix.rows[1].z;
    matrix.rows[2].z = matrix.rows[0].x * matrix.rows[1].y - matrix.rows[0].y * matrix.rows[1].x;
    axis.x = matrix.rows[2].x;
    axis.y = matrix.rows[2].y;
    axis.z = matrix.rows[2].z;
    magnitude = Vector3::CalcMagnitude(axis);
    if (magnitude > 0.00000011920929f) {
        matrix.rows[2].x = matrix.rows[2].x / magnitude;
        matrix.rows[2].y = matrix.rows[2].y / magnitude;
        matrix.rows[2].z = matrix.rows[2].z / magnitude;
    }
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(mtwkGetBoneTransform(work, 12));
#if SHENMUE_S2_GCC295
    matrix_stack::InvertMatrix(0);
#else
    matrix_stack::InvertMatrix(nullptr);
#endif
    matrix_stack::GetMatrix(&inverse);
    matrix_stack::LoadMatrix(&matrix);
    Matrix4x4::MultiplyWithPrePassMatrix(&inverse);
    matrix_stack::GetMatrix(&matrix);
    matrix.rows[3].x = anchor.x;
    matrix.rows[3].y = anchor.y;
    matrix.rows[3].z = anchor.z;
    PopCurrentMatrix(1);
    return matrix;
}

namespace {
inline IKRig &FrameRig(MotionWork *work) {
    return reinterpret_cast<MotionWorkBindings *>(work)->rig;
}
inline const IKRig &FrameRig(const MotionWork *work) {
    return reinterpret_cast<const MotionWorkBindings *>(work)->rig;
}
inline MotionRotationParameters *FrameRotations(MotionWork *work) {
    return reinterpret_cast<MotionWorkBindings *>(work)->rotations;
}
inline std::int32_t FrameAbs(std::int32_t value) {
    return value < 0 ? -value : value;
}

enum MotionRegionFlag {
    MRF_Unknown01 = 0x1U,
    MRF_Unknown02 = 0x2U,
    MRF_Loop = 0x4U,
    MRF_ExcludeEndFrame = 0x8U,
    MRF_Transition = 0x10U,
    MRF_LeadFoot = 0x20U,
    MRF_Mirrored = 0x40U,
    MRF_Reinitialize = 0x80U,
    MRF_Independent = 0x100U,
    MRF_FollowLowerBody = 0x200U,
    MRF_ReturnToLowerBody = 0x400U,
    MRF_BlendToLowerBody = 0x800U,
    MRF_Unknown1000 = 0x1000U,
};

static const float motionFrameSeconds = 0.033333335f;

} // namespace

MotionRigClock *g_motionRegionClocks[5] = {};
std::uint8_t g_motionLayerEvaluated[8] = {};
std::int32_t g_ikEnabled = 0;

void cbEvaluateTargetRotOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    float blend = *rotation.blendAlpha;
    MotionRotationBasis &basis = *rotation.basis;
    Matrix4x4 transform;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[0]) * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[1]) * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[2]) * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    basis.targetAxis.x = transform.rows[0].x;
    basis.targetAxis.y = transform.rows[1].x;
    basis.targetAxis.z = transform.rows[2].x;
    basis.targetTangent.x = transform.rows[0].y;
    basis.targetTangent.y = transform.rows[1].y;
    basis.targetTangent.z = transform.rows[2].y;
    const float saved = g_motionCurrentAnimTime;
    g_motionCurrentAnimTime = g_motionMaximumAnimTime * blend;
    AxisAngleToBasisSlerp(basis);
    g_motionCurrentAnimTime = saved;
}

void cbEvaluateTwoJointRotationOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    float blend = *rotation.blendAlpha;
    MotionRotationBasis &basis = *rotation.basis;
    Matrix4x4 transform;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[0]) * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[1]) * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    basis.targetAxis.x = transform.rows[0].x;
    basis.targetAxis.y = transform.rows[1].x;
    basis.targetAxis.z = transform.rows[2].x;
    basis.targetTangent.x = transform.rows[0].y;
    basis.targetTangent.y = transform.rows[1].y;
    basis.targetTangent.z = transform.rows[2].y;
    const float saved = g_motionCurrentAnimTime;
    g_motionCurrentAnimTime = g_motionMaximumAnimTime * blend;
    AxisAngleToBasisSlerp(basis);
    g_motionCurrentAnimTime = saved;
}

void cbEvaluateXRotationOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    float blend = *rotation.blendAlpha;
    MotionRotationBasis &basis = *rotation.basis;
    Matrix4x4 transform;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[0]) * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    basis.targetAxis.x = transform.rows[0].x;
    basis.targetAxis.y = transform.rows[1].x;
    basis.targetAxis.z = transform.rows[2].x;
    basis.targetTangent.x = transform.rows[0].y;
    basis.targetTangent.y = transform.rows[1].y;
    basis.targetTangent.z = transform.rows[2].y;
    const float saved = g_motionCurrentAnimTime;
    g_motionCurrentAnimTime = g_motionMaximumAnimTime * blend;
    AxisAngleToBasisSlerp(basis);
    g_motionCurrentAnimTime = saved;
}

void cbEvaluateZYRotationOverride(void *parameters) {
    MotionRotationParameters &rotation = *static_cast<MotionRotationParameters *>(parameters);
    float blend = *rotation.blendAlpha;
    MotionRotationBasis &basis = *rotation.basis;
    Matrix4x4 transform;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[1]) * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation.channels[2]) * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    basis.targetAxis.x = transform.rows[0].x;
    basis.targetAxis.y = transform.rows[1].x;
    basis.targetAxis.z = transform.rows[2].x;
    basis.targetTangent.x = transform.rows[0].y;
    basis.targetTangent.y = transform.rows[1].y;
    basis.targetTangent.z = transform.rows[2].y;
    const float saved = g_motionCurrentAnimTime;
    g_motionCurrentAnimTime = g_motionMaximumAnimTime * blend;
    AxisAngleToBasisSlerp(basis);
    g_motionCurrentAnimTime = saved;
}

void ApplyRotY_ThenCopyOrRotZYX(void *parameters) {
    MotionRotationParameters *rotation = static_cast<MotionRotationParameters *>(parameters);
    MotionWork *work = g_activeMotionWork;
    float blend = *rotation->blendAlpha;
    MotionRotationBasis *basis = rotation->basis;
    std::uint32_t poseMode;
    if (g_ikEnabled)
        poseMode = FrameRig(work).lowerBody.poseMode;
    else
        poseMode = g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    if (g_ikEnabled && work->pelvisYawCurrent != 0)
        Matrix4x4::RotateAroundY_Wrapper(work->pelvisYawHalf);
    if (std::fabs(blend - 1.0f) > 0.00000011920929f) {
        cbEvaluateTargetRotOverride(rotation);
    } else {
        switch (poseMode) {
        case MP_FromChannels:
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[0]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[1]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[2]) * 57.29578f * 182.04443f));
            break;
        case MP_FromAxisAngle:
            AxisAngleToBasisSlerp(*basis);
            break;
        }
    }
}

void ApplyChannelRotZYX_WithIKXBias(void *parameters) {
    MotionRotationParameters *rotation = static_cast<MotionRotationParameters *>(parameters);
    MotionWork *work = g_activeMotionWork;
    float blend = *rotation->blendAlpha;
    MotionRotationBasis *basis = rotation->basis;
    std::uint32_t poseMode;
    if (g_ikEnabled)
        poseMode = FrameRig(work).chest.poseMode;
    else
        poseMode = g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    if (std::fabs(blend - 1.0f) > 0.00000011920929f) {
        cbEvaluateTargetRotOverride(rotation);
    } else {
        switch (poseMode) {
        case MP_FromChannels:
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[0]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[1]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[2]) * 57.29578f * 182.04443f));
            break;
        case MP_FromAxisAngle:
            AxisAngleToBasisSlerp(*basis);
            break;
        }
    }
    if (g_ikEnabled && work->pelvisYawCurrent != 0)
        Matrix4x4::RotateAroundX_Wrapper(work->pelvisYawCurrent - work->pelvisYawHalf);
}

void cbEvaluateChestOverride(void *parameters) {
    MotionRotationParameters *rotation = static_cast<MotionRotationParameters *>(parameters);
    MotionWork *work = g_activeMotionWork;
    float blend = *rotation->blendAlpha;
    MotionRotationBasis *basis = rotation->basis;
    std::uint32_t poseMode;
    if (g_ikEnabled)
        poseMode = FrameRig(work).chest.poseMode;
    else
        poseMode = g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    const float chestBlend = work->chestBlendScale;
    if (g_ikEnabled && chestBlend > 0.0f) {
        Matrix4x4 root;
        Matrix4x4 pelvis;
        Vector3 rootAxis;
        Vector3 pelvisAxis;
        Vector3 axis;
        matrix_stack::GetMatrix(&root);
        PopCurrentMatrix(1);
        matrix_stack::GetMatrix(&pelvis);
        rootAxis.x = root.rows[1].x;
        rootAxis.y = root.rows[1].y;
        rootAxis.z = root.rows[1].z;
        pelvisAxis.x = pelvis.rows[1].x;
        pelvisAxis.y = pelvis.rows[1].y;
        pelvisAxis.z = pelvis.rows[1].z;
        const float crossLength = am2Vector3CrossLength(&rootAxis, &pelvisAxis);
        const float dot = am2Vector3Dot(&rootAxis, &pelvisAxis);
        am2Vector3Cross(&rootAxis, &pelvisAxis, &axis);
        std::int32_t angle;
        if (0.00000011920929f > Vector3::CalcMagnitude(axis))
            angle = 0;
        else
            angle = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(crossLength, dot))) * chestBlend);
        if (angle) {
            matrix_stack::LoadMatrix(&root);
            matrix_stack::InvertMatrix(0);
            Matrix4x4::Transform(axis, axis);
        }
        matrix_stack::Matrix4x4_Push(&root);
        if (angle)
            Matrix4x4_ApplyAngleRotationToVector(axis, angle);
    }
    if (std::fabs(blend - 1.0f) > 0.00000011920929f) {
        cbEvaluateTargetRotOverride(rotation);
    } else {
        switch (poseMode) {
        case MP_FromChannels:
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[0]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[1]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[2]) * 57.29578f * 182.04443f));
            break;
        case MP_FromAxisAngle:
            AxisAngleToBasisSlerp(*basis);
            break;
        }
    }
}

void ApplyChannelRotZYX_WithIKBlend(void *parameters) {
    MotionRotationParameters *rotation = static_cast<MotionRotationParameters *>(parameters);
    MotionWork *work = g_activeMotionWork;
    float blend = *rotation->blendAlpha;
    MotionRotationBasis *rotationBasis = rotation->basis;
    std::uint32_t poseMode;
    if (g_ikEnabled)
        poseMode = FrameRig(work).head.poseMode;
    else
        poseMode = g_motionPoseMode;
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    const float lookBlend = work->lookAtBlendTime;
    if (g_ikEnabled && lookBlend > 0.0f)
        matrix_stack::PushUnitMatrix();
    if (std::fabs(blend - 1.0f) > 0.00000011920929f) {
        cbEvaluateTargetRotOverride(rotation);
    } else {
        switch (poseMode) {
        case MP_FromChannels:
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[0]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[1]) * 57.29578f * 182.04443f));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(SampleMotionRigChannelAdvance(*rotation->channels[2]) * 57.29578f * 182.04443f));
            break;
        case MP_FromAxisAngle:
            AxisAngleToBasisSlerp(*rotationBasis);
            break;
        }
    }
    if (g_ikEnabled && lookBlend > 0.0f) {
        Matrix4x4 transform;
        MotionRotationBasis basis;
        matrix_stack::GetMatrix(&transform);
        PopCurrentMatrix(1);
        basis.sourceAxis.x = transform.rows[0].x;
        basis.sourceAxis.y = transform.rows[1].x;
        basis.sourceAxis.z = transform.rows[2].x;
        basis.sourceTangent.x = transform.rows[0].y;
        basis.sourceTangent.y = transform.rows[1].y;
        basis.sourceTangent.z = transform.rows[2].y;
        if (FrameRig(work).useOverride == 0) {
            basis.targetAxis.x = 0.0f;
            basis.targetAxis.y = 0.0f;
            basis.targetAxis.z = 1.0f;
            basis.targetTangent.x = 0.0f;
            basis.targetTangent.y = -1.0f;
            basis.targetTangent.z = 0.0f;
        } else {
            basis.targetAxis.x = 0.0f;
            basis.targetAxis.y = 0.70710677f;
            basis.targetAxis.z = 0.70710677f;
            basis.targetTangent.x = 0.0f;
            basis.targetTangent.y = -0.70710677f;
            basis.targetTangent.z = 0.70710677f;
        }
        const float saved = g_motionCurrentAnimTime;
        g_motionCurrentAnimTime = g_motionMaximumAnimTime * lookBlend;
        AxisAngleToBasisSlerp(basis);
        g_motionCurrentAnimTime = saved;
    }
}

void ApplyConstraint(MotionWork &work, Vector3 *position, std::uint32_t controller) {
    Vector3 bone;
    Vector3 delta;
    Vector3 source = {};
    Vector3 *blendSource = &work.constraintBlendSource[controller];
    float step = work.constraintStepLimit[controller];
    std::uint16_t mode = 6U & (work.constraintStateBits >> (3U * controller));
    matrix_stack::Matrix4x4_Push();
    matrix_stack::InvertMatrix(0);
    Matrix4x4::TransformPoint(*blendSource, source);
    if (mode == 2U) {
        if (work.sequenceFlags & MSF_LimitConstraintStep) {
            mtwkGetControllerTargetPosition(&work, controller, bone);
            Matrix4x4::TransformPoint(bone, bone);
            delta.x = source.x - bone.x;
            delta.y = source.y - bone.y;
            delta.z = source.z - bone.z;
            float distance = Vector3::CalcMagnitude(delta);
            if (step > distance) {
                position->x = source.x;
                position->y = source.y;
                position->z = source.z;
            } else {
                float fraction = step / distance;
                position->x = bone.x + fraction * delta.x;
                position->y = bone.y + fraction * delta.y;
                position->z = bone.z + fraction * delta.z;
            }
        } else {
            position->x = source.x;
            position->y = source.y;
            position->z = source.z;
        }
        work.constraintBlendTotalFrames[controller] = 0;
    } else if (mode == 4U) {
        if (work.sequenceFlags & MSF_LimitConstraintStep) {
            if (work.constraintBlendTotalFrames[controller] == 0) {
                mtwkGetControllerTargetPosition(&work, controller, bone);
                blendSource->x = bone.x;
                blendSource->y = bone.y;
                blendSource->z = bone.z;
                Matrix4x4::TransformPoint(*blendSource, source);
                Matrix4x4::TransformPoint(bone, bone);
                delta.x = position->x - bone.x;
                delta.y = position->y - bone.y;
                delta.z = position->z - bone.z;
                work.constraintBlendRemainingFrames[controller] = work.constraintBlendTotalFrames[controller] =
                    static_cast<std::uint32_t>(Vector3::CalcMagnitude(delta) / step) + 1U;
            }
            if (--work.constraintBlendRemainingFrames[controller] == 0) {
                work.constraintStateBits &= ~(6U << (3U * controller));
                work.constraintBlendTotalFrames[controller] = 0;
            } else {
                float amount =
                    static_cast<float>(work.constraintBlendRemainingFrames[controller]) / static_cast<float>(work.constraintBlendTotalFrames[controller]);
                position->x = (1.0f - amount) * position->x + amount * source.x;
                position->y = (1.0f - amount) * position->y + amount * source.y;
                position->z = (1.0f - amount) * position->z + amount * source.z;
            }
        } else {
            work.constraintStateBits &= ~(6U << (3U * controller));
            work.constraintBlendTotalFrames[controller] = 0;
        }
    }
    PopCurrentMatrix(1);
}

static std::uint32_t ComputeSignedVectorAngle(const Vector3 *first, const Vector3 *second, Vector3 *axis);
static std::uint32_t ComputeSignedVectorAngleVariant(const Vector3 *first, const Vector3 *second, Vector3 *axis);

void EvaluateHeadLookAt(MotionWork &work, Vector3 *target, std::uint32_t controller) {
    Vector3 look;
    Vector3 local = {0.0f, 0.0f, 0.0f};
    Matrix4x4 current;
    Vector3 axis;
    matrix_stack::GetMatrix(&current);
    std::uint16_t mode;
    std::uint16_t limit;
    std::uint16_t step;
    Vector3 *source = &work.constraintBlendSource[controller];
    limit = static_cast<std::uint16_t>(work.constraintStepLimit[controller] * 10430.378f);
    step = static_cast<std::uint16_t>(work.constraintStepRate[controller] * 10430.378f);
    mode = (work.constraintStateBits >> (controller * 3)) & 6U;
    const float ramp = work.pelvisYawRamp;
    matrix_stack::Matrix4x4_Push();
    matrix_stack::InvertMatrix(0);
    Matrix4x4::TransformPoint(*source, local);
    look.x = FrameRig(&work).head.lookTarget.x;
    look.y = FrameRig(&work).head.lookTarget.y;
    look.z = FrameRig(&work).head.lookTarget.z;
    if (work.motionFlags & 0x40U) {
        work.extraMotionMask = 0;
        if (local.x > 0.0f) {
            if (local.y < work.retainedCollisionValues[0] * local.x) {
                work.lookAtYawStep = dc::sdk::ArcTan2Fixed(-local.x, local.y);
                work.lookAtYawStep += dc::sdk::ArcTanFixed(work.retainedCollisionValues[0]);
                local.y = work.retainedCollisionValues[0] * local.x;
                work.extraMotionMask |= 2U;
            } else if (local.y < work.extraMotionPosition.x * local.x) {
                work.extraMotionMask |= 1U;
            }
        } else {
            if (local.y < -(work.retainedCollisionValues[0] * local.x)) {
                work.lookAtYawStep = dc::sdk::ArcTan2Fixed(-local.x, local.y);
                work.lookAtYawStep -= dc::sdk::ArcTanFixed(work.retainedCollisionValues[0]);
                local.y = -work.retainedCollisionValues[0] * local.x;
                work.extraMotionMask |= 8U;
            } else if (local.y < -(work.extraMotionPosition.x * local.x)) {
                work.extraMotionMask |= 4U;
            }
        }
        float planar;
        planar = shenmue::NativeSqrt(local.x * local.x + local.y * local.y);
        if (local.z > 0.0f) {
            if (local.z * work.retainedCollisionValues[1] > planar) {
                if (work.retainedCollisionValues[1] > 0.0f)
                    local.z = planar / work.retainedCollisionValues[1];
                work.extraMotionMask |= 0x20U;
            } else if (work.extraMotionPosition.y * local.z > planar) {
                work.extraMotionMask |= 0x10U;
            }
        } else {
            if (work.retainedCollisionValues[2] * local.z > planar) {
                if (0.0f > work.retainedCollisionValues[2])
                    local.z = planar / work.retainedCollisionValues[2];
                work.extraMotionMask |= 0x80U;
            } else if (work.extraMotionPosition.z * local.z > planar) {
                work.extraMotionMask |= 0x40U;
            }
        }
        if (0.2f > planar) {
            if (planar > 0.00000011920929f) {
                Vector3 *scaled = &local;
                const float scale = 0.2f / planar;
                scaled->x *= scale;
                scaled->y *= scale;
            } else {
                local.x = 0.0f;
                local.y = 0.2f;
            }
            work.extraMotionMask |= 0x200U;
        } else {
            work.extraMotionMask &= 0xfcffU;
        }
    }
    if (work.sequenceFlags & MSF_LimitConstraintStep) {
        std::uint16_t angle;
        step = limit / 5U + step;
        if (step > limit)
            step = limit;
        if (mode == 2U) {
            const float targetZ = target->z;
            float targetPlanar;
            targetPlanar = shenmue::NativeSqrt(target->x * target->x + target->y * target->y);
            if (work.motionFlags & 0x40000U)
                angle = ComputeSignedVectorAngleVariant(&look, &local, &axis);
            else
                angle = ComputeSignedVectorAngle(&look, &local, &axis);
            if (angle < step) {
                if (0.00000011920929f > ramp)
                    step = 0;
                target->x = local.x;
                target->y = local.y;
                if (work.motionFlags & 0x40000U)
                    target->z = shenmue::NativeSqrt(local.x * local.x + local.y * local.y) / targetPlanar * target->z;
                else
                    target->z = local.z;
            } else {
                matrix_stack::PushUnitMatrix();
                Matrix4x4_ApplyAngleRotationToVector(axis, step);
                Matrix4x4::Transform(look, *target);
                PopCurrentMatrix(1);
                if (work.motionFlags & 0x40000U)
                    target->z = targetZ / targetPlanar * shenmue::NativeSqrt(target->x * target->x + target->y * target->y);
            }
        } else if (mode == 4U) {
            angle = ComputeSignedVectorAngle(&look, target, &axis);
            if (angle < step) {
                if (0.00000011920929f > ramp)
                    step = 0;
                work.constraintStateBits &= ~(6U << (controller * 3));
            } else {
                matrix_stack::PushUnitMatrix();
                Matrix4x4_ApplyAngleRotationToVector(axis, step);
                Matrix4x4::Transform(look, *target);
                PopCurrentMatrix(1);
            }
        }
        work.constraintStepRate[controller] = static_cast<std::int32_t>(step) / 10430.378f;
    } else {
        work.constraintStepRate[controller] = 0.0f;
        if (mode == 2U) {
            target->x = local.x;
            target->y = local.y;
            target->z = local.z;
        } else if (mode == 4U) {
            work.constraintStateBits &= ~(6U << (controller * 3));
        }
    }
    PopCurrentMatrix(1);
}

void cbEvaluateTargetPosOverride(void *parameters) {
    MotionTargetParameters *target = static_cast<MotionTargetParameters *>(parameters);
    Vector3 *output = target->output;
    Vector4 *pva = target->pva;
    MotionWork *work = g_activeMotionWork;
    std::int32_t controller = *target->controller;
    float blend = work->layerBlendAlpha[0];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    float root[3], blended[3], difference[3], rest[3];
    MotionRigChannel **rootChannels = reinterpret_cast<MotionWorkBindings *>(work)->targets[0].channels;
    std::int32_t axis = 0;
    root[axis] = blended[axis] = SampleMotionRigChannelAdvance(*rootChannels[axis]);
    root[1] = blended[1] = 0.0f;
    root[2] = blended[2] = SampleMotionRigChannelAdvance(*rootChannels[2]);
    if (work->motionFlags & 2U) {
        rest[0] = FrameRig(work).lowerBody.pelvisPva[0].w;
        blended[0] = (1.0f - blend) * rest[0] + root[0] * blend;
        rest[2] = FrameRig(work).lowerBody.pelvisPva[2].w;
        blended[2] = (1.0f - blend) * rest[2] + root[2] * blend;
    }
    for (axis = 0; axis < 3; ++axis) {
        difference[axis] = SampleMotionRigChannelAdvance(*reinterpret_cast<MotionRigChannel **>(target)[axis + 1]) - root[axis];
        rest[axis] = pva[axis].w;
        output->data()[axis] = (1.0f - blend) * rest[axis] + blend * difference[axis] + blended[axis];
    }
    if (!g_ikEnabled)
        return;
    if ((work->constraintStateBits >> (controller * 3)) & 7U)
        ApplyConstraint(*work, output, controller);
    if (target->callback) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::TransformPoint(*output, *output);
        target->callback(controller, output, work, target->user);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(*output, *output);
        PopCurrentMatrix(1);
    }
}

void Motionwork_PelvisOverride(void *parameters) {
    MotionTargetParameters *target = static_cast<MotionTargetParameters *>(parameters);
    Vector3 *output = target->output;
    Vector4 *pva = target->pva;
    MotionWork *work = g_activeMotionWork;
    std::int32_t controller = *target->controller;
    float sampled[3];
    float rest[3];
    float blend = work->layerBlendAlpha[0];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    if (work->motionFlags & 2U) {
        for (std::int32_t axis = 0; axis < 3; ++axis) {
            sampled[axis] = SampleMotionRigChannelAdvance(*reinterpret_cast<MotionRigChannel **>(target)[axis + 1]);
            rest[axis] = pva[axis].w;
            output->data()[axis] = rest[axis] * (1.0f - blend) + sampled[axis] * blend;
        }
    } else {
        output->x = SampleMotionRigChannelAdvance(*target->channels[0]);
        sampled[1] = SampleMotionRigChannelAdvance(*target->channels[1]);
        rest[1] = pva[1].w;
        output->y = rest[1] * (1.0f - blend) + sampled[1] * blend;
        output->z = SampleMotionRigChannelAdvance(*target->channels[2]);
    }
    if (!g_ikEnabled)
        return;
    if ((work->constraintStateBits >> (controller * 3)) & 7U)
        ApplyConstraint(*work, output, controller);
    if (target->callback) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::TransformPoint(*output, *output);
        target->callback(controller, output, work, target->user);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(*output, *output);
        PopCurrentMatrix(1);
    }
}

void cbEvaluateLookAtOverride(void *parameters) {
    MotionTargetParameters *target = static_cast<MotionTargetParameters *>(parameters);
    Vector3 *output = target->output;
    Vector4 *pva = target->pva;
    MotionWork *work = g_activeMotionWork;
    std::int32_t controller = *target->controller;
    float sampled[3];
    float rest[3];
    float blend;
    if (controller > 4)
        blend = work->layerBlendAlpha[0];
    else
        blend = work->layerBlendAlpha[controller];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        sampled[axis] = SampleMotionRigChannelAdvance(*reinterpret_cast<MotionRigChannel **>(target)[axis + 1]);
        rest[axis] = pva[axis].w;
        output->data()[axis] = rest[axis] * (1.0f - blend) + sampled[axis] * blend;
    }
    if (!g_ikEnabled)
        return;
    if ((work->constraintStateBits >> (controller * 3)) & 7U)
        EvaluateHeadLookAt(*work, output, controller);
    if (target->callback) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::TransformPoint(*output, *output);
        target->callback(controller, output, work, target->user);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(*output, *output);
        PopCurrentMatrix(1);
    }
}

void Arm_TargetEffectorOverride(void *parameters) {
    MotionTargetParameters *target = static_cast<MotionTargetParameters *>(parameters);
    Vector3 *output = target->output;
    Vector4 *pva = target->pva;
    MotionWork *work = g_activeMotionWork;
    std::int32_t controller = *target->controller;
    float sampled[3];
    float rest[3];
    Vector3 world;
    float blend;
    if (controller > 4)
        blend = work->layerBlendAlpha[0];
    else
        blend = work->layerBlendAlpha[controller];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        sampled[axis] = SampleMotionRigChannelAdvance(*reinterpret_cast<MotionRigChannel **>(target)[axis + 1]);
        rest[axis] = pva[axis].w;
        output->data()[axis] = rest[axis] * (1.0f - blend) + sampled[axis] * blend;
    }
    if (!g_ikEnabled)
        return;
    if ((work->constraintStateBits >> (controller * 3)) & 7U) {
        if (work->sequenceFlags & MSF_LookAt) {
            if (controller == 3 && (*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0xa0U)) {
                Matrix4x4::TransformPoint(*output, world);
                if (work->locomotionMotionSet & 0x80U) {
                    work->constraintBlendSource[controller].x = (world.x + work->constraintBlendSource[controller].x) * 0.5f;
                    work->constraintBlendSource[controller].y = (world.y + work->constraintBlendSource[controller].y) * 0.5f;
                    work->constraintBlendSource[controller].z = (world.z + work->constraintBlendSource[controller].z) * 0.5f;
                }
            }
            if (controller == 4 && (*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0x140U)) {
                Matrix4x4::TransformPoint(*output, world);
                if (work->locomotionMotionSet & 0x100U) {
                    work->constraintBlendSource[controller].x = (world.x + work->constraintBlendSource[controller].x) * 0.5f;
                    work->constraintBlendSource[controller].y = (world.y + work->constraintBlendSource[controller].y) * 0.5f;
                    work->constraintBlendSource[controller].z = (world.z + work->constraintBlendSource[controller].z) * 0.5f;
                }
            }
        }
        ApplyConstraint(*work, output, controller);
    }
    if (target->callback) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::TransformPoint(*output, *output);
        target->callback(controller, output, work, target->user);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(*output, *output);
        PopCurrentMatrix(1);
    }
}

void Motionwork_ChestIKEffectorOverride(void *parameters) {
    MotionTargetParameters *target = static_cast<MotionTargetParameters *>(parameters);
    Vector3 *output = target->output;
    Vector4 *pva = target->pva;
    MotionWork *work = g_activeMotionWork;
    std::int32_t controller = *target->controller;
    float sampled[3];
    float rest[3];
    Vector3 anchor;
    Vector3 effector;
    Vector3 local;
    float blend;
    if (controller > 4)
        blend = work->layerBlendAlpha[0];
    else
        blend = work->layerBlendAlpha[controller];
    if (g_motionInitFlags & motionInitBaking)
        blend = 1.0f;
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        sampled[axis] = SampleMotionRigChannelAdvance(*reinterpret_cast<MotionRigChannel **>(target)[axis + 1]);
        rest[axis] = pva[axis].w;
        output->data()[axis] = rest[axis] * (1.0f - blend) + sampled[axis] * blend;
    }
    if (!g_ikEnabled)
        return;
    if ((work->constraintStateBits >> (controller * 3)) & 7U) {
        ApplyConstraint(*work, output, controller);
    } else if (work->sequenceFlags & MSF_LookAt) {
        Matrix4x4::Transform(*output, effector);
        const float length = Vector3::CalcMagnitude(effector);
        if (0.0f > output->x) {
            anchor.x = output->x;
            anchor.y = length;
            anchor.z = 0.0f;
        } else {
            anchor.x = 0.0f;
            anchor.y = length;
            anchor.z = 0.0f;
        }
        Matrix4x4::Transform(anchor, anchor);
        float scale = work->extraBlendScale;
        if ((work->locomotionMotionSet & 8U) && work->transitionState != 3U) {
            Matrix4x4::TransformPoint(*output, local);
            if (*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0x2a0000U) {
                if (work->extraBlendTime > 0.69999999f)
                    scale = scale * 0.30000001f + scale * 0.69999999f * (1.0f - work->extraBlendTime) / 0.30000001f;
                work->extraBlendTime = work->extraBlendTime + scale;
            } else if (!(*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0x3f0000U)) {
                if (0.30000001f > work->extraBlendTime)
                    scale = scale * 0.69999999f * work->extraBlendTime / 0.30000001f + scale * 0.30000001f;
                work->extraBlendTime = work->extraBlendTime - scale;
            }
        } else {
            if (0.30000001f > work->extraBlendTime)
                scale = scale * 0.69999999f * work->extraBlendTime / 0.30000001f + scale * 0.30000001f;
            work->extraBlendTime = work->extraBlendTime - scale;
        }
        if (work->extraBlendTime > 1.0f)
            work->extraBlendTime = 1.0f;
        else if (0.0f > work->extraBlendTime)
            work->extraBlendTime = 0.0f;
        const float time = work->extraBlendTime;
        effector.x = time * anchor.x + (1.0f - time) * effector.x;
        effector.y = time * anchor.y + (1.0f - time) * effector.y;
        effector.z = time * anchor.z + (1.0f - time) * effector.z;
        matrix_stack::Matrix4x4_Push();
        matrix_stack::InvertMatrix(0);
        Matrix4x4::Transform(effector, *output);
        PopCurrentMatrix(1);
    }
    if (target->callback) {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::TransformPoint(*output, *output);
        target->callback(controller, output, work, target->user);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::TransformPoint(*output, *output);
        PopCurrentMatrix(1);
    }
}

void mtwkCopyRigFlags(
    MotionWork *work, MotionRigClock **lowerBody, MotionRigClock **chest, MotionRigClock **head, MotionRigClock **rightArm, MotionRigClock **leftArm) {
    if (lowerBody)
        *lowerBody = &FrameRig(work).lowerBody.clock;
    if (chest)
        *chest = &FrameRig(work).chest.clock;
    if (head)
        *head = &FrameRig(work).head.clock;
    if (rightArm)
        *rightArm = &FrameRig(work).rightArm.clock;
    if (leftArm)
        *leftArm = &FrameRig(work).leftArm.clock;
}

float ClampMotionTimePair(MotionTimePair *pair, float split, float ticks, float maximum);

float mtwkTimePairClampAndPropagate(MotionWork *work, float split, float ticks, float maximum, MotionRegion region) {
    const float current = ClampMotionTimePair(&g_motionRegionClocks[region]->current, split, ticks, maximum);
    switch (region) {
    case 0:
        if (split >= 0.0f)
            FrameRig(work).lowerBody.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).lowerBody.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 1:
        if (split >= 0.0f)
            FrameRig(work).chest.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).chest.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 2:
        if (split >= 0.0f)
            FrameRig(work).head.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).head.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 3:
        if (split >= 0.0f)
            FrameRig(work).rightArm.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).rightArm.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 4:
        if (split >= 0.0f)
            FrameRig(work).leftArm.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).leftArm.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    }
    return current;
}

void mtwkAssignStaticDataForBodyRegion(MotionWork *work, float split, float ticks, float maximum, MotionRegion region) {
    if (split > 0.0f)
        g_motionRegionClocks[region]->current.splitTimeFrames = 0.0f;
    if (ticks > 0.0f)
        g_motionRegionClocks[region]->current.ticksTime = 0.0f;
    if (maximum > 0.0f) {
        g_motionRegionClocks[region]->current.maxTime = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(work->currentMotionId[region]) - 1));
        if (work->regionFlags[region] & MRF_ExcludeEndFrame)
            g_motionRegionClocks[region]->current.maxTime -= 0.001f;
    }
    switch (region) {
    case 0:
        if (split >= 0.0f)
            FrameRig(work).lowerBody.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).lowerBody.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 1:
        if (split >= 0.0f)
            FrameRig(work).chest.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).chest.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 2:
        if (split >= 0.0f)
            FrameRig(work).head.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).head.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 3:
        if (split >= 0.0f)
            FrameRig(work).rightArm.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).rightArm.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    case 4:
        if (split >= 0.0f)
            FrameRig(work).leftArm.clock.authored.splitTimeFrames = g_motionRegionClocks[region]->current.splitTimeFrames;
        if (maximum >= 0.0f)
            FrameRig(work).leftArm.clock.authored.ticksTime = g_motionRegionClocks[region]->current.maxTime;
        break;
    }
}

std::uint32_t TimestepUpdate(MotionRigClock *clock, std::uint32_t allowOverflow, std::uint32_t allowReset) {
    if (clock->current.ticksTime + clock->playbackSpeed > clock->current.maxTime) {
        if (allowOverflow) {
            clock->current.ticksTime += clock->playbackSpeed;
            clock->current.ticksTime = clock->current.splitTimeFrames +
                (0.0f > clock->current.ticksTime - ::ceilf(clock->current.maxTime) ? 0.0f : clock->current.ticksTime - ::ceilf(clock->current.maxTime));
            return 3;
        }
        if (allowReset) {
            clock->current.ticksTime = clock->current.splitTimeFrames;
            return 2;
        }
        clock->current.ticksTime = clock->current.maxTime;
        return 1;
    }
    clock->current.ticksTime += clock->playbackSpeed;
    return 0;
}

std::uint32_t AnimClock_ForceJump(MotionRigClock *clock) {
    clock->current.ticksTime += 1.0f;
    if (!(clock->current.ticksTime < clock->current.maxTime))
        return 4;
    return 0;
}

bool Rig_SetupMotionGraph(IKRig &rig, const MotionStreamContext *context, float time, std::uint32_t flags, std::uint32_t region,
    MotionTargetParameters *targets, MotionRotationParameters *rotations) {
    std::uint32_t graph = (static_cast<std::int32_t>(flags) >> 8) & 0xff;
    if (!targets || !rotations)
        graph = 0;
    if (graph)
        flags = (flags & 0xffff00ffU) | motionInitCallbacks;
    flags |= motionInitChannels;
    switch (region) {
    case 0:
        ReadMotionBase(rig, rig.lowerBody, context, -1.0f, time, flags);
        if (!graph)
            break;
        rig.lowerBody.leftFoot.pva[0].w -= rig.lowerBody.pelvisPva[0].w;
        rig.lowerBody.leftFoot.pva[2].w -= rig.lowerBody.pelvisPva[2].w;
        rig.lowerBody.rightFoot.pva[0].w -= rig.lowerBody.pelvisPva[0].w;
        rig.lowerBody.rightFoot.pva[2].w -= rig.lowerBody.pelvisPva[2].w;
        if (rig.useOverride) {
            rig.lowerBody.overridePelvisTarget->pva[0].w -= rig.lowerBody.pelvisPva[0].w;
            rig.lowerBody.overridePelvisTarget->pva[2].w -= rig.lowerBody.pelvisPva[2].w;
        }
        rig.lowerBody.pelvisPositionCallback.evaluate = Motionwork_PelvisOverride;
        rig.lowerBody.pelvisPositionCallback.parameters = &targets[0];
        rig.lowerBody.pelvisRotationCallback.evaluate = ApplyRotY_ThenCopyOrRotZYX;
        rig.lowerBody.pelvisRotationCallback.parameters = &rotations[0];
        if (!rig.useOverride) {
            rig.lowerBody.hips.callback.evaluate = cbEvaluateTargetRotOverride;
            rig.lowerBody.hips.callback.parameters = &rotations[1];
        } else {
            rig.lowerBody.overrideHips->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.lowerBody.overrideHips->callback.parameters = &rotations[1];
            rig.lowerBody.overridePelvisTarget->callback.evaluate = cbEvaluateTargetPosOverride;
            rig.lowerBody.overridePelvisTarget->callback.parameters = &targets[1];
            rig.lowerBody.leftHipNode->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.lowerBody.leftHipNode->callback.parameters = &rotations[2];
        }
        rig.lowerBody.leftThigh.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.lowerBody.leftThigh.callback.parameters = &rotations[3];
        rig.lowerBody.leftFoot.callback.evaluate = cbEvaluateTargetPosOverride;
        rig.lowerBody.leftFoot.callback.parameters = &targets[2];
        rig.lowerBody.leftAnkleCallback.evaluate = cbEvaluateTargetRotOverride;
        rig.lowerBody.leftAnkleCallback.parameters = &rotations[4];
        if (rig.useOverride) {
            rig.lowerBody.rightHipNode->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.lowerBody.rightHipNode->callback.parameters = &rotations[5];
        }
        rig.lowerBody.rightThigh.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.lowerBody.rightThigh.callback.parameters = &rotations[6];
        rig.lowerBody.rightFoot.callback.evaluate = cbEvaluateTargetPosOverride;
        rig.lowerBody.rightFoot.callback.parameters = &targets[3];
        rig.lowerBody.rightAnkleCallback.evaluate = cbEvaluateTargetRotOverride;
        rig.lowerBody.rightAnkleCallback.parameters = &rotations[7];
        if (rig.useOverride) {
            rig.lowerBody.rightToe1Node->callback.evaluate = cbEvaluateTwoJointRotationOverride;
            rig.lowerBody.rightToe1Node->callback.parameters = &rotations[8];
            rig.lowerBody.rightToe2Node->callback.evaluate = cbEvaluateTwoJointRotationOverride;
            rig.lowerBody.rightToe2Node->callback.parameters = &rotations[9];
            rig.lowerBody.rightToe3Node->callback.evaluate = cbEvaluateTwoJointRotationOverride;
            rig.lowerBody.rightToe3Node->callback.parameters = &rotations[10];
        }
        break;
    case 1:
        ReadMotionChest(rig, rig.chest, context, time, flags);
        if (!graph)
            break;
        rig.chest.rotation.callback.evaluate = cbEvaluateChestOverride;
        rig.chest.rotation.callback.parameters = &rotations[11];
        if (!rig.useOverride) {
            rig.chest.target.callback.evaluate = Motionwork_ChestIKEffectorOverride;
            rig.chest.target.callback.parameters = &targets[4];
            rig.chest.upperRotation.callback.evaluate = ApplyChannelRotZYX_WithIKXBias;
            rig.chest.upperRotation.callback.parameters = &rotations[12];
        } else {
            rig.chest.target.pva[0].w = rig.chest.target.pva[0].w - rig.lowerBody.pelvisPva[0].w;
            rig.chest.target.pva[2].w = rig.chest.target.pva[2].w - rig.lowerBody.pelvisPva[2].w;
            rig.chest.target.callback.evaluate = cbEvaluateTargetPosOverride;
            rig.chest.target.callback.parameters = &targets[4];
        }
        break;
    case 2:
        ReadMotionHead(rig, rig.head, context, time, flags);
        if (!graph)
            break;
        rig.head.baseRotation.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.head.baseRotation.callback.parameters = &rotations[13];
        if (rig.useOverride) {
            rig.head.aimTargetNode->pva[0].w = rig.head.aimTargetNode->pva[0].w - rig.lowerBody.pelvisPva[0].w;
            rig.head.aimTargetNode->pva[2].w = rig.head.aimTargetNode->pva[2].w - rig.lowerBody.pelvisPva[2].w;
            rig.head.aimSourceNode->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.head.aimSourceNode->callback.parameters = &rotations[14];
            rig.head.aimTargetNode->callback.evaluate = cbEvaluateTargetPosOverride;
            rig.head.aimTargetNode->callback.parameters = &targets[5];
        }
        rig.head.midRotation.callback.evaluate = ApplyChannelRotZYX_WithIKBlend;
        rig.head.midRotation.callback.parameters = &rotations[15];
        rig.head.target.callback.evaluate = cbEvaluateLookAtOverride;
        rig.head.target.callback.parameters = &targets[6];
        if (rig.useOverride) {
            rig.head.nodeA->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.head.nodeA->callback.parameters = &rotations[16];
            rig.head.nodeB->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.head.nodeB->callback.parameters = &rotations[17];
            rig.head.nodeC->callback.evaluate = cbEvaluateTargetRotOverride;
            rig.head.nodeC->callback.parameters = &rotations[18];
        }
        break;
    case 3:
        ReadMotionArm(rig, rig.rightArm, context, time, flags);
        if (!graph)
            break;
        rig.rightArm.shoulder.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.rightArm.shoulder.callback.parameters = &rotations[19];
        rig.rightArm.elbow.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.rightArm.elbow.callback.parameters = &rotations[20];
        if (!rig.useOverride) {
            rig.rightArm.target.callback.evaluate = Arm_TargetEffectorOverride;
            rig.rightArm.target.callback.parameters = &targets[7];
            rig.rightArm.forearmCallback.evaluate = cbEvaluateXRotationOverride;
            rig.rightArm.forearmCallback.parameters = &rotations[21];
            rig.rightArm.wristCallback.evaluate = cbEvaluateZYRotationOverride;
            rig.rightArm.wristCallback.parameters = &rotations[22];
        } else {
            rig.rightArm.target.pva[0].w = rig.rightArm.target.pva[0].w - rig.lowerBody.pelvisPva[0].w;
            rig.rightArm.target.pva[2].w = rig.rightArm.target.pva[2].w - rig.lowerBody.pelvisPva[2].w;
            rig.rightArm.target.callback.evaluate = cbEvaluateTargetPosOverride;
            rig.rightArm.target.callback.parameters = &targets[7];
            rig.rightArm.wristCallback.evaluate = cbEvaluateTargetRotOverride;
            rig.rightArm.wristCallback.parameters = &rotations[22];
        }
        break;
    case 4:
        ReadMotionArm(rig, rig.leftArm, context, time, flags | motionInitLeftArm);
        if (!graph)
            break;
        rig.leftArm.shoulder.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.leftArm.shoulder.callback.parameters = &rotations[23];
        rig.leftArm.elbow.callback.evaluate = cbEvaluateTargetRotOverride;
        rig.leftArm.elbow.callback.parameters = &rotations[24];
        if (!rig.useOverride) {
            rig.leftArm.target.callback.evaluate = Arm_TargetEffectorOverride;
            rig.leftArm.target.callback.parameters = &targets[8];
            rig.leftArm.forearmCallback.evaluate = cbEvaluateXRotationOverride;
            rig.leftArm.forearmCallback.parameters = &rotations[25];
            rig.leftArm.wristCallback.evaluate = cbEvaluateZYRotationOverride;
            rig.leftArm.wristCallback.parameters = &rotations[26];
        } else {
            rig.leftArm.target.pva[0].w = rig.leftArm.target.pva[0].w - rig.lowerBody.pelvisPva[0].w;
            rig.leftArm.target.pva[2].w = rig.leftArm.target.pva[2].w - rig.lowerBody.pelvisPva[2].w;
            // The override rig's left target reuses the right arm's parameter record.
            rig.leftArm.target.callback.evaluate = cbEvaluateTargetPosOverride;
            rig.leftArm.target.callback.parameters = &targets[7];
            rig.leftArm.wristCallback.evaluate = cbEvaluateTargetRotOverride;
            rig.leftArm.wristCallback.parameters = &rotations[26];
        }
        break;
    default:
        break;
    }
    return true;
}

// @verified - d
static std::uint32_t ComputeSignedVectorAngle(const Vector3 *first, const Vector3 *second, Vector3 *axis) {
    const float firstLength = Vector3::CalcMagnitude(*first);
    const float secondLength = Vector3::CalcMagnitude(*second);
    const float dot = am2Vector3Dot(first, second);
    am2Vector3Cross(first, second, axis);
    const float crossLength = Vector3::CalcMagnitude(*axis);
    if (0.00000011920929f > crossLength)
        return 0;
    return dc::sdk::ArcTan2Fixed(crossLength / (firstLength * secondLength), dot / (firstLength * secondLength));
}

// @verified - d
static std::uint32_t ComputeSignedVectorAngleVariant(const Vector3 *first, const Vector3 *second, Vector3 *axis) {
    Vector3 from = *first;
    Vector3 to = *second;
    from.z = to.z = 0.0f;
    const float fromLength = Vector3::CalcMagnitude(from);
    const float toLength = Vector3::CalcMagnitude(to);
    const float dot = am2Vector3Dot(&from, &to);
    am2Vector3Cross(&from, &to, axis);
    const float crossLength = Vector3::CalcMagnitude(*axis);
    if (0.00000011920929f > crossLength)
        return 0;
    return dc::sdk::ArcTan2Fixed(crossLength / (fromLength * toLength), dot / (fromLength * toLength));
}

void mtwkRigRunOverrideBlendPass(MotionWork *work) {
    std::int32_t region;
    for (region = 0; region < 5; ++region) {
        if ((work->regionFlags[region] & MRF_Transition) && work->currentRootMode != 0 && work->currentRootMode != 2) {
            work->overrideBlendTime = -1.0f;
            return;
        }
    }
    for (region = 0; region < 5; ++region) {
        if (work->regionFlags[region] & MRF_Independent)
            continue;
        const std::int32_t sourceRig = static_cast<std::int8_t>(MOTN_GetDataForAnimId(work->currentMotionId[region])->flags);
        const float scale = mtwkGetScaleFactorFor(FrameRig(work).boneId, sourceRig, region > 3 ? 3 : region);
        if (work->regionFlags[region] & MRF_Mirrored)
            g_motionInitFlags = motionInitMirror;
        else
            g_motionInitFlags = 0;
        char evaluated = 0;
        switch (region) {
        case 0:
            evaluated = EvaluateBase(FrameRig(work).lowerBody, sourceRig, scale, work->overrideBlendTime);
            break;
        case 1:
            evaluated = EvaluateChestBlend(FrameRig(work).chest, sourceRig, scale, work->overrideBlendTime);
            break;
        case 2:
            evaluated = EvaluateHead(FrameRig(work).head, sourceRig, scale, work->overrideBlendTime);
            break;
        case 3:
            evaluated = EvaluateArm(FrameRig(work).rightArm, sourceRig, scale, work->overrideBlendTime);
            break;
        case 4:
            evaluated = EvaluateArm(FrameRig(work).leftArm, sourceRig, scale, work->overrideBlendTime);
            break;
        }
        if (evaluated)
            mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
    }
    TranslatePelvisFromChannels(FrameRig(work).lowerBody, MI_Hermite);
    work->overrideBlendTime = -1.0f;
}

bool mtwkClearOverrideCallbacksForBodyRegion(MotionWork *work, MotionRegion region) {
    IKRig &rig = FrameRig(work);
    switch (region) {
    case 0:
        rig.lowerBody.pelvisPositionCallback.evaluate = 0;
        rig.lowerBody.pelvisPositionCallback.parameters = 0;
        rig.lowerBody.pelvisRotationCallback.evaluate = 0;
        rig.lowerBody.pelvisRotationCallback.parameters = 0;
        rig.lowerBody.hips.callback.evaluate = 0;
        rig.lowerBody.hips.callback.parameters = 0;
        rig.lowerBody.leftThigh.callback.evaluate = 0;
        rig.lowerBody.leftThigh.callback.parameters = 0;
        rig.lowerBody.leftFoot.callback.evaluate = 0;
        rig.lowerBody.leftFoot.callback.parameters = 0;
        rig.lowerBody.leftAnkleCallback.evaluate = 0;
        rig.lowerBody.leftAnkleCallback.parameters = 0;
        rig.lowerBody.rightThigh.callback.evaluate = 0;
        rig.lowerBody.rightThigh.callback.parameters = 0;
        rig.lowerBody.rightFoot.callback.evaluate = 0;
        rig.lowerBody.rightFoot.callback.parameters = 0;
        rig.lowerBody.rightAnkleCallback.evaluate = 0;
        rig.lowerBody.rightAnkleCallback.parameters = 0;
        if (rig.useOverride) {
            rig.lowerBody.overrideHips->callback.evaluate = 0;
            rig.lowerBody.overrideHips->callback.parameters = 0;
            rig.lowerBody.overridePelvisTarget->callback.evaluate = 0;
            rig.lowerBody.overridePelvisTarget->callback.parameters = 0;
            rig.lowerBody.leftHipNode->callback.evaluate = 0;
            rig.lowerBody.leftHipNode->callback.parameters = 0;
            rig.lowerBody.rightHipNode->callback.evaluate = 0;
            rig.lowerBody.rightHipNode->callback.parameters = 0;
            rig.lowerBody.rightToe1Node->callback.evaluate = 0;
            rig.lowerBody.rightToe1Node->callback.parameters = 0;
            rig.lowerBody.rightToe2Node->callback.evaluate = 0;
            rig.lowerBody.rightToe2Node->callback.parameters = 0;
            rig.lowerBody.rightToe3Node->callback.evaluate = 0;
            rig.lowerBody.rightToe3Node->callback.parameters = 0;
        }
        break;
    case 1:
        rig.chest.rotation.callback.evaluate = 0;
        rig.chest.rotation.callback.parameters = 0;
        rig.chest.target.callback.evaluate = 0;
        rig.chest.target.callback.parameters = 0;
        rig.chest.upperRotation.callback.evaluate = 0;
        rig.chest.upperRotation.callback.parameters = 0;
        break;
    case 2:
        rig.head.baseRotation.callback.evaluate = 0;
        rig.head.baseRotation.callback.parameters = 0;
        rig.head.midRotation.callback.evaluate = 0;
        rig.head.midRotation.callback.parameters = 0;
        rig.head.target.callback.evaluate = 0;
        rig.head.target.callback.parameters = 0;
        if (rig.useOverride) {
            rig.head.aimSourceNode->callback.evaluate = 0;
            rig.head.aimSourceNode->callback.parameters = 0;
            rig.head.aimTargetNode->callback.evaluate = 0;
            rig.head.aimTargetNode->callback.parameters = 0;
            rig.head.nodeA->callback.evaluate = 0;
            rig.head.nodeA->callback.parameters = 0;
            rig.head.nodeB->callback.evaluate = 0;
            rig.head.nodeB->callback.parameters = 0;
            rig.head.nodeC->callback.evaluate = 0;
            rig.head.nodeC->callback.parameters = 0;
        }
        break;
    case 3:
        rig.rightArm.shoulder.callback.evaluate = 0;
        rig.rightArm.shoulder.callback.parameters = 0;
        rig.rightArm.elbow.callback.evaluate = 0;
        rig.rightArm.elbow.callback.parameters = 0;
        rig.rightArm.target.callback.evaluate = 0;
        rig.rightArm.target.callback.parameters = 0;
        rig.rightArm.forearmCallback.evaluate = 0;
        rig.rightArm.forearmCallback.parameters = 0;
        rig.rightArm.wristCallback.evaluate = 0;
        rig.rightArm.wristCallback.parameters = 0;
        break;
    case 4:
        rig.leftArm.shoulder.callback.evaluate = 0;
        rig.leftArm.shoulder.callback.parameters = 0;
        rig.leftArm.elbow.callback.evaluate = 0;
        rig.leftArm.elbow.callback.parameters = 0;
        rig.leftArm.target.callback.evaluate = 0;
        rig.leftArm.target.callback.parameters = 0;
        rig.leftArm.forearmCallback.evaluate = 0;
        rig.leftArm.forearmCallback.parameters = 0;
        rig.leftArm.wristCallback.evaluate = 0;
        rig.leftArm.wristCallback.parameters = 0;
        break;
    }
    return true;
}

#define SHENMUE_BIND_FRAME_CALLBACK(callback, function, data)                                                                                                  \
    do {                                                                                                                                                       \
        (callback).evaluate = function;                                                                                                                        \
        (callback).parameters = data;                                                                                                                          \
    } while (0)

void mtwkInstallActiveOverrideCallbacks(MotionWork *work) {
    MotionWorkBindings *const bindings = reinterpret_cast<MotionWorkBindings *>(work);
    if (bindings->targets[0].callback)
        SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.lowerBody.pelvisPositionCallback, Motionwork_PelvisOverride, &bindings->targets[0]);
    if (bindings->targets[6].callback)
        SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.head.target.callback, cbEvaluateLookAtOverride, &bindings->targets[6]);
    if (bindings->targets[2].callback)
        SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.lowerBody.leftFoot.callback, cbEvaluateTargetPosOverride, &bindings->targets[2]);
    if (bindings->targets[3].callback)
        SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.lowerBody.rightFoot.callback, cbEvaluateTargetPosOverride, &bindings->targets[3]);
    if (!bindings->rig.useOverride) {
        if (bindings->targets[4].callback)
            SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.chest.target.callback, Motionwork_ChestIKEffectorOverride, &bindings->targets[4]);
        if (bindings->targets[7].callback)
            SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.rightArm.target.callback, Arm_TargetEffectorOverride, &bindings->targets[7]);
        if (bindings->targets[8].callback)
            SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.leftArm.target.callback, Arm_TargetEffectorOverride, &bindings->targets[8]);
    } else {
        if (bindings->targets[4].callback)
            SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.chest.target.callback, cbEvaluateTargetPosOverride, &bindings->targets[4]);
        if (bindings->targets[1].callback) {
            bindings->rig.lowerBody.overridePelvisTarget->callback.evaluate = cbEvaluateTargetPosOverride;
            bindings->rig.lowerBody.overridePelvisTarget->callback.parameters = &bindings->targets[1];
        }
        if (bindings->targets[5].callback) {
            bindings->rig.head.aimTargetNode->callback.evaluate = cbEvaluateTargetPosOverride;
            bindings->rig.head.aimTargetNode->callback.parameters = &bindings->targets[5];
        }
        if (bindings->targets[7].callback)
            SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.rightArm.target.callback, cbEvaluateTargetPosOverride, &bindings->targets[7]);
        if (bindings->targets[8].callback)
            SHENMUE_BIND_FRAME_CALLBACK(bindings->rig.leftArm.target.callback, cbEvaluateTargetPosOverride, &bindings->targets[8]);
    }
}

#undef SHENMUE_BIND_FRAME_CALLBACK

void mtwkInstallConstraintTargetCallback(MotionWork *work, std::uint32_t controller) {
    switch (controller) {
    case 0:
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisPositionCallback.evaluate = Motionwork_PelvisOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisPositionCallback.parameters =
            &reinterpret_cast<MotionWorkBindings *>(work)->targets[0];
        break;
    case 1:
        if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride)
            reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.target.callback.evaluate = Motionwork_ChestIKEffectorOverride;
        else
            reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.target.callback.evaluate = cbEvaluateTargetPosOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.target.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->targets[4];
        break;
    case 2:
        reinterpret_cast<MotionWorkBindings *>(work)->rig.head.target.callback.evaluate = cbEvaluateLookAtOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.head.target.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->targets[6];
        break;
    case 3:
        if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride)
            reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.target.callback.evaluate = Arm_TargetEffectorOverride;
        else
            reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.target.callback.evaluate = cbEvaluateTargetPosOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.target.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->targets[7];
        break;
    case 4:
        if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride)
            reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.target.callback.evaluate = Arm_TargetEffectorOverride;
        else
            reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.target.callback.evaluate = cbEvaluateTargetPosOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.target.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->targets[8];
        break;
    case 5:
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftFoot.callback.evaluate = cbEvaluateTargetPosOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftFoot.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->targets[2];
        break;
    case 6:
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightFoot.callback.evaluate = cbEvaluateTargetPosOverride;
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightFoot.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->targets[3];
        break;
    default:
        if (reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
            switch (controller) {
            case 7:
                reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.overridePelvisTarget->callback.evaluate = cbEvaluateTargetPosOverride;
                reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.overridePelvisTarget->callback.parameters =
                    &reinterpret_cast<MotionWorkBindings *>(work)->targets[1];
                break;
            case 8:
                reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimTargetNode->callback.evaluate = cbEvaluateTargetPosOverride;
                reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimTargetNode->callback.parameters =
                    &reinterpret_cast<MotionWorkBindings *>(work)->targets[5];
                break;
            }
        }
        break;
    }
}

void mtwkClearConstraintTargetCallbackIfInactive(MotionWork *work, std::uint32_t controller) {
    switch (controller) {
    case 0:
        if (!(work->regionFlags[0] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).lowerBody.pelvisPositionCallback.evaluate = 0;
            FrameRig(work).lowerBody.pelvisPositionCallback.parameters = 0;
        }
        break;
    case 1:
        if (!(work->regionFlags[1] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).chest.target.callback.evaluate = 0;
            FrameRig(work).chest.target.callback.parameters = 0;
        }
        break;
    case 2:
        if (!(work->regionFlags[2] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).head.target.callback.evaluate = 0;
            FrameRig(work).head.target.callback.parameters = 0;
        }
        break;
    case 3:
        if (!(work->regionFlags[3] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).rightArm.target.callback.evaluate = 0;
            FrameRig(work).rightArm.target.callback.parameters = 0;
        }
        break;
    case 4:
        if (!(work->regionFlags[4] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).leftArm.target.callback.evaluate = 0;
            FrameRig(work).leftArm.target.callback.parameters = 0;
        }
        break;
    case 5:
        if (!(work->regionFlags[0] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).lowerBody.leftFoot.callback.evaluate = 0;
            FrameRig(work).lowerBody.leftFoot.callback.parameters = 0;
        }
        break;
    case 6:
        if (!(work->regionFlags[0] & MRF_Transition) || work->currentRootMode != 2) {
            FrameRig(work).lowerBody.rightFoot.callback.evaluate = 0;
            FrameRig(work).lowerBody.rightFoot.callback.parameters = 0;
        }
        break;
    default:
        if (!FrameRig(work).useOverride)
            break;
        switch (controller) {
        case 7:
            if (!(work->regionFlags[0] & MRF_Transition) || work->currentRootMode != 2) {
                FrameRig(work).lowerBody.overridePelvisTarget->callback.evaluate = 0;
                FrameRig(work).lowerBody.overridePelvisTarget->callback.parameters = 0;
            }
            break;
        case 8:
            if (!(work->regionFlags[2] & MRF_Transition) || work->currentRootMode != 2) {
                FrameRig(work).head.aimTargetNode->callback.evaluate = 0;
                FrameRig(work).head.aimTargetNode->callback.parameters = 0;
            }
            break;
        }
        break;
    }
}

// @verified - d
void mtwkUpdatePelvisYaw(MotionWork *work) {
    MotionWork &state = *work;
    std::int16_t step = static_cast<std::int16_t>(state.pelvisYawStep);
    const std::int16_t clamp = state.pelvisYawClamp;
    const std::int16_t targetYaw = state.pelvisYawTarget;
    const std::int16_t threshold = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(clamp) * 0.30000001f));
    const std::int16_t delta = static_cast<std::int16_t>(targetYaw - state.pelvisYawCurrent);
    const std::int16_t distance = static_cast<std::int16_t>(static_cast<std::int32_t>(std::fabs(static_cast<float>(delta))));
    if (distance < threshold)
        step = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(step) * (static_cast<float>(delta) / static_cast<float>(threshold))));
    else
        step = delta < 0 ? static_cast<std::int16_t>(-step) : step;
    if (static_cast<float>(distance) < std::fabs(static_cast<float>(step)))
        step = delta;
    const float magnitude = std::fabs(static_cast<float>(step));
    if (0.00000011920929f > static_cast<float>(magnitude)) {
        state.pelvisYawRamp = 0.0f;
    } else {
        const float ramp = state.pelvisYawRamp + 0.2f;
        state.pelvisYawRamp = ramp;
        if (ramp > 1.0f)
            state.pelvisYawRamp = 1.0f;
    }
    std::int16_t &current = state.pelvisYawCurrent;
    current = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(step) * state.pelvisYawRamp)) + current;
    state.pelvisYawVelocity = step;
    if (state.pelvisYawCurrent > clamp)
        state.pelvisYawCurrent = clamp;
    else if (static_cast<std::int32_t>(state.pelvisYawCurrent) < -static_cast<std::int32_t>(clamp))
        state.pelvisYawCurrent = static_cast<std::int16_t>(-clamp);
    state.pelvisYawHalf = static_cast<std::int16_t>(state.pelvisYawCurrent / 2);
    if (state.pelvisYawHalf > 3640)
        state.pelvisYawHalf = 3640;
    else if (state.pelvisYawHalf < -3640)
        state.pelvisYawHalf = -3640;
}

std::int32_t mtwkUpdateOverrideCallbacks(MotionWork *work) {
    std::int32_t controllers = 7, controller;
    if (FrameRig(work).useOverride)
        controllers = 9;
    for (controller = 0; controller < controllers; ++controller) {
        Vector3 *const target = const_cast<Vector3 *>(work->activeConstraintTarget[controller]);
        if (target) {
            Matrix4x4::TransformPoint(*target, *target);
            work->latchedConstraintTarget[controller] = work->activeConstraintTarget[controller];
            work->activeConstraintTarget[controller] = 0;
        }
    }
    for (controller = 0; controller < controllers; ++controller) {
        if (work->latchedConstraintTarget[controller]) {
            work->constraintStateBits &= ~(7U << controller * 3);
            work->constraintStateBits |= 1U << controller * 3;
            work->constraintStateBits |= 2U << controller * 3;
            mtwkInstallConstraintTargetCallback(work, controller);
        } else if ((work->constraintStateBits >> controller * 3) & 7U) {
            if ((work->constraintStateBits >> controller * 3) & 6U) {
                work->constraintStateBits &= ~(6U << controller * 3);
                work->constraintStateBits |= 4U << controller * 3;
                mtwkInstallConstraintTargetCallback(work, controller);
            } else {
                work->constraintStateBits &= ~(7U << controller * 3);
                mtwkClearConstraintTargetCallbackIfInactive(work, controller);
            }
        }
    }
    if (!FrameRig(work).useOverride) {
        if (work->pelvisYawCurrent == 0 && work->pelvisYawHalf == 0 && work->pelvisYawTarget == 0) {
            if (!(work->regionFlags[0] & MRF_Transition) || work->currentRootMode != 2)
                FrameRig(work).lowerBody.pelvisRotationCallback.evaluate = 0;
            if (!(work->regionFlags[1] & MRF_Transition) || work->currentRootMode != 2)
                FrameRig(work).chest.upperRotation.callback.evaluate = 0;
            work->lookAtYaw = 0;
        } else {
            FrameRig(work).lowerBody.pelvisRotationCallback.evaluate = ApplyRotY_ThenCopyOrRotZYX;
            FrameRig(work).lowerBody.pelvisRotationCallback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->rotations[0];
            FrameRig(work).chest.upperRotation.callback.evaluate = ApplyChannelRotZYX_WithIKXBias;
            FrameRig(work).chest.upperRotation.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->rotations[12];
            mtwkUpdatePelvisYaw(work);
        }
    }
    if (work->sequenceFlags & MSF_LookAt) {
        mtwkInstallConstraintTargetCallback(work, 2);
        if (!FrameRig(work).useOverride)
            mtwkInstallConstraintTargetCallback(work, 1);
        FrameRig(work).head.midRotation.callback.evaluate = ApplyChannelRotZYX_WithIKBlend;
        FrameRig(work).head.midRotation.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->rotations[15];
        const float blend = work->lookAtBlendTime + 0.1f;
        work->lookAtBlendTime = blend;
        if (blend > 1.0f)
            work->lookAtBlendTime = 1.0f;
    } else {
        mtwkClearConstraintTargetCallbackIfInactive(work, 1);
        const float blend = work->lookAtBlendTime - 0.1f;
        work->lookAtBlendTime = blend;
        if (0.0f > blend)
            work->lookAtBlendTime = 0.0f;
        if ((!(work->regionFlags[2] & MRF_Transition) || work->currentRootMode != 2) && 0.00000011920929f > work->lookAtBlendTime)
            FrameRig(work).head.midRotation.callback.evaluate = 0;
    }
    if ((work->sequenceFlags & MSF_TargetFacing) &&
        (*reinterpret_cast<const std::uint32_t *>(work->regionFlags) & ((MRF_Independent | MRF_BlendToLowerBody) << 16)) == MRF_Independent << 16) {
        FrameRig(work).chest.rotation.callback.evaluate = cbEvaluateChestOverride;
        FrameRig(work).chest.rotation.callback.parameters = &reinterpret_cast<MotionWorkBindings *>(work)->rotations[11];
        const float blend = work->chestBlendScale + work->chestRotationOverrideBlendTime;
        work->chestBlendScale = blend;
        if (blend > 1.0f)
            work->chestBlendScale = 1.0f;
    } else {
        const float blend = work->chestBlendScale - work->chestRotationOverrideBlendTime;
        work->chestBlendScale = blend;
        if (0.0f > blend)
            work->chestBlendScale = 0.0f;
        if ((!(work->regionFlags[1] & MRF_Transition) || work->currentRootMode != 2) && 0.00000011920929f > work->chestBlendScale)
            FrameRig(work).chest.rotation.callback.evaluate = 0;
    }
    mtwkInstallActiveOverrideCallbacks(work);
    return 1;
}

std::int32_t MOTN_IsEndlessMotion(std::uint16_t animation);

void mtwkApplyRootTranslation(MotionWork *work) {
    std::int32_t region, retired, mode;
    Vector3 travel;
    g_ikEnabled = 0;
    work->pendingFootstepCue = 0;
    work->accumulatedTranslation.x = 0.0f;
    work->accumulatedTranslation.y = 0.0f;
    work->accumulatedTranslation.z = 0.0f;
    work->previousRootMode = work->currentRootMode;
    if (work->requestedRootMode == 2 && !IsFightActive() && !g_mtwkCurrentIsRyo)
        work->requestedRootMode = 1;
    if (work->motionFlags & 0x20000U) {
        Vector4 *root = &FrameRig(work).lowerBody.rootPrepassTransform.rows[3];
        root->x = work->cachedRootPosition.x;
        root->y = work->cachedRootPosition.y;
        root->z = work->cachedRootPosition.z;
    }
    for (region = 0; region < 5; ++region) {
        work->regionFlags[region] &= ~MRF_Unknown01;
        work->regionFlags[region] &= ~MRF_Unknown02;
        g_motionLayerEvaluated[region] = 0;
        if (work->queuedMotionId[region] && MOTN_IsEndlessMotion(work->queuedMotionId[region]))
            work->queuedMotionId[region] = 0;
        if (!work->currentMotionId[region])
            work->regionFlags[region] |= MRF_Reinitialize;
        retired = 0;
        if (region == 0 || (work->regionFlags[region] & MRF_Independent)) {
            if (!IsMotionAnimationValid(work->currentMotionId[region])) {
                if (region == 0)
                    retired = 1;
                work->regionFlags[region] |= MRF_Reinitialize;
                if (!work->queuedMotionId[region])
                    work->queuedMotionId[region] = 0xf001U;
            }
        }
        if (region == 0) {
            if (!work->currentMotionId[region] && !work->queuedMotionId[region])
                work->queuedMotionId[region] = 0xf085U;
        } else {
            if (work->regionFlags[region] & MRF_Independent) {
                if ((work->regionFlags[region] & MRF_FollowLowerBody) ||
                    ((work->regionFlags[region] & MRF_ReturnToLowerBody) && !(work->regionFlags[region] & MRF_BlendToLowerBody))) {
                    if (!work->queuedMotionId[region] && !retired) {
                        work->queuedMotionId[region] = work->currentMotionId[0];
                        if (mtwkSetRegionFlag(work, 0x46, 1, static_cast<MotionRegion>(0)))
                            work->regionFlags[region] |= MRF_LeadFoot;
                        if (work->regionFlags[region] & MRF_ReturnToLowerBody) {
                            if (!work->defaultRegionStepFrames[region])
                                work->regionStepFrames[region] = 8;
                            else
                                work->regionStepFrames[region] = work->defaultRegionStepFrames[region];
                            work->regionFlags[region] |= MRF_BlendToLowerBody;
                        } else {
                            work->regionStepFrames[region] = 0;
                            work->regionFlags[region] = (work->regionFlags[region] & 0xfeffU) | MRF_Reinitialize;
                        }
                        work->layerLoopStartTime[region] = g_motionRegionClocks[0]->current.splitTimeFrames;
                        work->pendingLayerStartFrame[region] = g_motionRegionClocks[0]->current.ticksTime;
                        if (work->regionStepFrames[region])
                            work->pendingLayerStartFrame[region] = static_cast<float>(work->regionStepFrames[region]) * g_motionRegionClocks[0]->playbackSpeed +
                                work->pendingLayerStartFrame[region];
                        work->layerEndFrame[region] = g_motionRegionClocks[0]->current.maxTime;
                        if (work->pendingLayerStartFrame[region] > work->layerEndFrame[region]) {
                            if (work->regionFlags[region] & MRF_Loop) {
                                while (work->pendingLayerStartFrame[region] > work->layerEndFrame[region])
                                    work->pendingLayerStartFrame[region] = work->pendingLayerStartFrame[region] - work->layerEndFrame[region];
                            } else {
                                work->pendingLayerStartFrame[region] = work->layerEndFrame[region];
                            }
                        }
                        mtwkSetSpeed(work, g_motionRegionClocks[0]->playbackSpeed, static_cast<MotionRegion>(region));
                    } else if (!work->queuedMotionId[region] && retired) {
                        work->regionFlags[region] &= 0xfeffU;
                    }
                }
            } else {
                work->regionFlags[region] &= 0xfdffU;
                work->regionFlags[region] &= 0xfbffU;
            }
            if (!(work->regionFlags[region] & MRF_Independent) || (work->regionFlags[region] & MRF_BlendToLowerBody)) {
                if (!work->queuedMotionId[region] || (work->regionFlags[region] & MRF_BlendToLowerBody) || (work->regionFlags[region] & MRF_FollowLowerBody)) {
                    if (work->queuedMotionId[0]) {
                        work->queuedMotionId[region] = work->queuedMotionId[0];
                        work->layerLoopStartTime[region] = work->layerLoopStartTime[0];
                        work->pendingLayerStartFrame[region] = work->pendingLayerStartFrame[0];
                        work->layerEndFrame[region] = work->layerEndFrame[0];
                        if (work->regionFlags[0] & MRF_LeadFoot)
                            work->regionFlags[region] |= MRF_LeadFoot;
                        else
                            work->regionFlags[region] &= 0xffdfU;
                        if (work->regionFlags[0] & MRF_Reinitialize)
                            work->regionFlags[region] |= MRF_Reinitialize;
                        else
                            work->defaultRegionStepFrames[region] = work->defaultRegionStepFrames[0];
                        work->regionFlags[region] |= MRF_FollowLowerBody;
                        work->regionFlags[region] &= 0xf7ffU;
                    }
                }
                g_motionRegionClocks[region]->playbackSpeed = g_motionRegionClocks[0]->playbackSpeed;
            }
        }
        if (!work->queuedMotionId[region])
            continue;
        if ((work->regionFlags[region] & MRF_BlendToLowerBody) && (work->regionFlags[region] & MRF_Independent)) {
            mode = 1;
        } else {
            mode = work->requestedRootMode;
            work->regionFlags[region] &= 0xf7ffU;
        }
        if (mode == 0 || (work->regionFlags[region] & MRF_Reinitialize)) {
            work->regionStepFrames[region] = 0;
        } else if (mode == 4) {
            MotionRigClock *clock = g_motionRegionClocks[region];
            float frames = clock->current.maxTime - clock->current.ticksTime;
            if (clock->playbackSpeed >= 0.00000011920929f)
                frames = frames / clock->playbackSpeed;
            work->regionStepFrames[region] = static_cast<std::int32_t>(::ceilf(frames));
        } else {
            if (!work->defaultRegionStepFrames[region])
                work->regionStepFrames[region] = 8;
            else
                work->regionStepFrames[region] = work->defaultRegionStepFrames[region];
        }
        work->currentRootMode = work->requestedRootMode;
        if (region == 0) {
            if (mode == 4) {
                const float time = g_motionRegionClocks[region]->current.maxTime;
                if (!(work->regionFlags[region] & MRF_Transition) || work->currentRootMode == 2) {
                    SampleNativeMotionRoot(work->currentMotionId[region], FrameRig(work).boneId, time, &travel);
                } else {
                    travel.x = FrameRig(work).lowerBody.pelvisPosition[0].channel.next.value;
                    travel.z = FrameRig(work).lowerBody.pelvisPosition[2].channel.next.value;
                }
                travel.x = travel.x + FrameRig(work).lowerBody.pelvisTransform.rows[3].x;
                travel.z = travel.z + FrameRig(work).lowerBody.pelvisTransform.rows[3].z;
                if (work->motionFlags & 1U)
                    Matrix4x4::preTranslate(travel.x, 0.0f, travel.z);
            } else if (mode == 5) {
                float time = static_cast<float>(work->regionStepFrames[region]) * g_motionRegionClocks[region]->playbackSpeed +
                    g_motionRegionClocks[region]->current.ticksTime;
                if (time > g_motionRegionClocks[region]->current.maxTime)
                    time = g_motionRegionClocks[region]->current.maxTime;
                if (!(work->regionFlags[region] & MRF_Transition) || work->currentRootMode == 2) {
                    SampleNativeMotionRoot(work->currentMotionId[region], FrameRig(work).boneId, time, &travel);
                } else if (time > g_motionRegionClocks[region]->current.maxTime) {
                    travel.x = FrameRig(work).lowerBody.pelvisPosition[0].channel.next.value;
                    travel.z = FrameRig(work).lowerBody.pelvisPosition[2].channel.next.value;
                } else {
                    g_motionCurrentAnimTime = motionFrameSeconds * time;
                    travel.x = SampleMotionRigChannelAdvance(FrameRig(work).lowerBody.pelvisPosition[0].channel);
                    travel.z = SampleMotionRigChannelAdvance(FrameRig(work).lowerBody.pelvisPosition[2].channel);
                }
                travel.x = travel.x + FrameRig(work).lowerBody.pelvisTransform.rows[3].x;
                travel.z = travel.z + FrameRig(work).lowerBody.pelvisTransform.rows[3].z;
                if (work->motionFlags & 1U)
                    Matrix4x4::preTranslate(travel.x, 0.0f, travel.z);
            }
        }
    }
}

bool MOTN_HasHoldCommand(std::uint16_t animation);

// @verified - d
void mtwkProcessLayers(MotionWork *work) {
    std::uint32_t flags;
    std::int32_t rootMode, wasTracked, processed;
    processed = 0;
    for (std::int32_t region = 0; region < 5; ++region) {
        if (work->queuedMotionId[region]) {
            processed = 1;
            wasTracked = 0;
            rootMode = 0;
            const std::uint16_t *header = reinterpret_cast<const std::uint16_t *>(MOTN_GetDataForAnimId(work->queuedMotionId[region]));
            if (!header) {
                if (work->useMatrixOverrides == 0)
                    work->queuedMotionId[region] = 0xf001U;
                else
                    work->queuedMotionId[region] = 0xf0ebU;
                header = reinterpret_cast<const std::uint16_t *>(MOTN_GetDataForAnimId(work->queuedMotionId[region]));
            }
            if (work->useMatrixOverrides != static_cast<std::uint32_t>(static_cast<char>(header[1]) == 16)) {
                if (work->useMatrixOverrides == 0)
                    work->queuedMotionId[region] = 0xf001U;
                else
                    work->queuedMotionId[region] = 0xf0ebU;
                header = reinterpret_cast<const std::uint16_t *>(MOTN_GetDataForAnimId(work->queuedMotionId[region]));
            }
            work->previousRegionTrack[region] = work->regionTrack[region];
            if (work->regionFlags[region] & MRF_Transition)
                work->previousLayerSampleFrame[region] = work->layerSampleFrame[region];
            else
                work->previousLayerSampleFrame[region] = g_motionRegionClocks[region]->current.ticksTime;
            MotionStreamContext context;
            if (region == 3)
                UpdatePointersForCurrentFrame(&context, header, reinterpret_cast<void **>(const_cast<std::uint16_t **>(&work->armHandTracks[0])),
                    reinterpret_cast<void **>(const_cast<std::uint16_t **>(&work->regionTrack[3])));
            else if (region == 4)
                UpdatePointersForCurrentFrame(&context, header, reinterpret_cast<void **>(const_cast<std::uint16_t **>(&work->armHandTracks[1])),
                    reinterpret_cast<void **>(const_cast<std::uint16_t **>(&work->regionTrack[4])));
            else
                UpdatePointersForCurrentFrame(&context, header, 0, reinterpret_cast<void **>(const_cast<std::uint16_t **>(&work->regionTrack[region])));
            flags = motionInitStream | motionInitChannels;
            if ((work->regionFlags[region] & MRF_LeadFoot) != MOTN_GetLeadFootFlag(work->queuedMotionId[region])) {
                flags = motionInitStream | motionInitMirror | motionInitChannels;
                work->regionFlags[region] |= MRF_Mirrored;
            } else {
                work->regionFlags[region] &= ~MRF_Mirrored;
            }
            work->layerSampleFrame[region] = -1.0f;
            if (work->regionStepFrames[region]) {
                flags |= ((work->regionStepFrames[region] + 1) & 0xff) << 8;
                work->blendTotalFrames[region] = work->regionStepFrames[region] & 0xff;
            } else {
                work->blendTotalFrames[region] = 0;
            }
            work->blendRemainingFrames[region] = work->blendTotalFrames[region];
            if (flags & motionInitGraphFrames) {
                if (work->regionFlags[region] & MRF_Transition)
                    wasTracked = 1;
                work->regionFlags[region] |= MRF_Transition;
            } else {
                work->regionFlags[region] &= ~MRF_Transition;
            }
            float loopStart = -1.0f;
            float localTime = 0.0f;
            if (!(work->regionFlags[region] & MRF_Reinitialize)) {
                if (work->regionFlags[region] & MRF_BlendToLowerBody)
                    rootMode = 1;
                else
                    rootMode = work->currentRootMode;
                float stepFrames = static_cast<float>(work->regionStepFrames[region]);
                if (rootMode == 5) {
                    MotionRigClock *const clock = g_motionRegionClocks[region];
                    if (clock->playbackSpeed * stepFrames + clock->current.ticksTime > clock->current.maxTime) {
                        stepFrames = stepFrames -
                            ::ceilf(
                                (clock->current.maxTime - clock->current.ticksTime) / (0.00000011920929f > clock->playbackSpeed ? 1.0f : clock->playbackSpeed));
                    } else {
                        stepFrames = 0.0f;
                    }
                }
                const float lastFrame = static_cast<float>(static_cast<std::int16_t>(context.frameCount) - 1);
                if (rootMode == 3 || rootMode == 5) {
                    if (0.0f > work->pendingLayerStartFrame[region]) {
                        work->layerSampleFrame[region] = g_motionRegionClocks[region]->playbackSpeed * stepFrames;
                    } else {
                        if (work->pendingLayerStartFrame[region] > lastFrame)
                            work->pendingLayerStartFrame[region] = work->pendingLayerStartFrame[region] - lastFrame;
                        if (!(work->motionFlags & 0x4000U))
                            loopStart = work->pendingLayerStartFrame[region];
                        work->layerSampleFrame[region] = g_motionRegionClocks[region]->playbackSpeed * stepFrames + work->pendingLayerStartFrame[region];
                    }
                    if (work->layerSampleFrame[region] > lastFrame) {
                        if (MOTN_HasHoldCommand(work->queuedMotionId[region])) {
                            Vector3 travel = {0.0f, 0.0f, 0.0f};
                            Vector3 sampled;
                            if (region == 0) {
                                while (work->layerSampleFrame[region] > lastFrame - 0.0001f) {
                                    work->layerSampleFrame[region] = work->layerSampleFrame[region] - lastFrame;
                                    SampleNativeMotionRoot(work->queuedMotionId[region], work->rigBoneId, lastFrame, &sampled);
                                    travel.x = travel.x + sampled.x;
                                    travel.z = travel.z + sampled.z;
                                    if (!(0.0f > loopStart)) {
                                        SampleNativeMotionRoot(work->queuedMotionId[region], work->rigBoneId, loopStart, &sampled);
                                        travel.x = travel.x - sampled.x;
                                        travel.z = travel.z - sampled.z;
                                    }
                                    SampleNativeMotionRoot(work->queuedMotionId[region], work->rigBoneId, 0.0f, &sampled);
                                    travel.x = travel.x - sampled.x;
                                    travel.z = travel.z - sampled.z;
                                    loopStart = -1.0f;
                                }
                                localTime = work->layerSampleFrame[region];
                                SampleNativeMotionRoot(work->queuedMotionId[region], work->rigBoneId, localTime, &sampled);
                                travel.x = travel.x + sampled.x;
                                travel.z = travel.z + sampled.z;
                                if (work->motionFlags & 1U)
                                    Matrix4x4::preTranslate(travel.x, travel.y, travel.z);
                            } else {
                                while (work->layerSampleFrame[region] > lastFrame) {
                                    work->layerSampleFrame[region] = work->layerSampleFrame[region] - lastFrame;
                                    loopStart = -1.0f;
                                }
                                localTime = work->layerSampleFrame[region];
                            }
                        } else {
                            localTime = lastFrame;
                        }
                    } else {
                        localTime = work->layerSampleFrame[region];
                    }
                } else if (!(0.0f > work->pendingLayerStartFrame[region])) {
                    if (work->pendingLayerStartFrame[region] > lastFrame)
                        localTime = lastFrame;
                    else
                        localTime = work->pendingLayerStartFrame[region];
                    work->layerSampleFrame[region] = work->pendingLayerStartFrame[region];
                }
                work->pendingLayerStartFrame[region] = -1.0f;
            }
            if (region == 0) {
                if (work->regionFlags[region] & MRF_Transition) {
                    float timer, limit;
                    if (!wasTracked) {
                        timer = g_motionRegionClocks[region]->current.ticksTime;
                        limit = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(work->currentMotionId[region]) - 1));
                    } else {
                        timer = work->transitionTimer;
                        limit = work->transitionTimerLimit;
                    }
                    float reset = 0.0f > loopStart ? 0.0f : loopStart;
                    work->baseTimeScale = g_motionRegionClocks[region]->playbackSpeed;
                    switch (rootMode) {
                    case 3:
                        work->transitionTimerReset = work->transitionTimer = reset;
                        work->transitionTimerLimit = g_motionRegionClocks[region]->playbackSpeed * static_cast<float>(work->blendTotalFrames[region]) + reset;
                        break;
                    case 4:
                        work->transitionTimerReset = work->transitionTimer = timer;
                        work->transitionTimerLimit = limit;
                        break;
                    case 5:
                        work->transitionTimerReset = reset;
                        work->transitionTimer = timer;
                        work->transitionTimerLimit = limit;
                        break;
                    }
                }
                if (rootMode == 3)
                    work->motionFlags |= 0x4000U;
                if (0.0f > work->layerSampleFrame[region]) {
                    if (work->motionFlags & 0x2000U)
                        flags |= motionInitTranslation;
                } else if (!(work->motionFlags & 0x4000U)) {
                    flags |= motionInitTranslation;
                }
            }
            if (work->overrideBlendTime > 0.0f)
                SetOverrideBlendTime(work->overrideBlendTime);
            if (rootMode == 2) {
                Rig_SetupMotionGraph(reinterpret_cast<MotionWorkBindings *>(work)->rig, &context, localTime, flags, region,
                    reinterpret_cast<MotionWorkBindings *>(work)->targets, reinterpret_cast<MotionWorkBindings *>(work)->rotations);
            } else {
                if (region == 0 && (work->motionFlags & 0x8000U)) {
                    flags |= motionInitSplitWindow;
                    if (work->motionFlags & motionInitBaking)
                        flags |= motionInitSwapLeadFoot;
                }
                mtwkRigEvaluateControllers(work, &context, loopStart, localTime, flags, static_cast<MotionRegion>(region));
            }
            work->previousMotionId[region] = work->currentMotionId[region];
            work->currentMotionId[region] = work->queuedMotionId[region];
            work->queuedMotionId[region] = 0;
            if (region != 0) {
                if (!(work->regionFlags[region] & MRF_FollowLowerBody) || (work->regionFlags[region] & MRF_Independent))
                    work->regionFlags[region] |= MRF_Independent;
            } else {
                reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].x = 0.0f;
                reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].y = 0.0f;
                reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].z = 0.0f;
                if (!(work->regionFlags[region] & MRF_Transition)) {
                    if (0.0f > work->layerSampleFrame[region]) {
                        if (work->motionFlags & 0x2000U)
                            TranslatePelvisFromChannels(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody, MI_Hermite);
                    } else if (!(work->motionFlags & 0x4000U)) {
                        float start = work->layerSampleFrame[region];
                        Vector3 end, begin;
                        float scale;
                        TranslatePelvisFromChannels(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody, MI_Hermite);
                        SampleNativeMotionRoot(work->currentMotionId[region], work->rigBoneId, start, &end);
                        if (g_motionRegionClocks[region]->playbackSpeed > start) {
                            SampleNativeMotionRoot(work->currentMotionId[region], work->rigBoneId, 0.0f, &begin);
                            scale = 0.00000011920929f > start ? 1.0f : g_motionRegionClocks[region]->playbackSpeed / start;
                        } else {
                            start = start - g_motionRegionClocks[region]->playbackSpeed;
                            SampleNativeMotionRoot(work->currentMotionId[region], work->rigBoneId, start, &begin);
                            scale = 1.0f;
                        }
                        end.x = end.x - begin.x;
                        end.z = end.z - begin.z;
                        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].x =
                            end.x * scale + reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].x;
                        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].z =
                            end.z * scale + reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisTransform.rows[3].z;
                    }
                } else {
                    if (work->currentRootMode == 2) {
                        if (0.0f > work->layerSampleFrame[region]) {
                            if (work->motionFlags & 0x2000U)
                                TranslatePelvisFromChannels(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody, MI_Hermite);
                        } else if (!(work->motionFlags & 0x4000U)) {
                            TranslatePelvisFromChannels(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody, MI_Hermite);
                        }
                    }
                }
                work->motionFlags &= ~0x2000U;
                if (!(0.0f > work->gaitSplitFraction))
                    SetGaitSplitFraction(work->gaitSplitFraction);
            }
            work->regionFlags[region] |= MRF_Unknown01;
            if (work->currentRootMode == 2 || !(work->regionFlags[region] & MRF_Transition))
                g_motionLayerEvaluated[region] = 1;
        }
        std::uint16_t clear = work->regionFlags[region];
        clear &= ~MRF_FollowLowerBody;
        clear &= ~MRF_ReturnToLowerBody;
        clear &= ~MRF_LeadFoot;
        clear &= ~MRF_Reinitialize;
        work->regionFlags[region] = clear;
    }
    if (processed)
        work->overrideBlendTime = -1.0f;
}

void mtwkAdvanceAnimFrameAndLayers(MotionWork *work) {
    int reread = 0;
    Matrix4x4 channels;
    Vector3 end;
    if (work->overrideBlendTime > 0.0f)
        mtwkRigRunOverrideBlendPass(work);
    int result;
    if (!g_motionLayerEvaluated[0]) {
        if ((work->regionFlags[0] & MRF_Transition) && work->currentRootMode != 0 && work->currentRootMode != 2) {
            result = AnimClock_ForceJump(g_motionRegionClocks[0]);
        } else if ((work->regionFlags[0] & MRF_Transition) && work->currentRootMode == 0) {
            result = 4;
            g_motionRegionClocks[0]->current.ticksTime = g_motionRegionClocks[0]->current.maxTime;
        } else {
            std::uint32_t allowOverflow = work->regionFlags[0] & MRF_Loop;
            std::uint32_t allowReset = work->regionFlags[0] & MRF_ExcludeEndFrame;
            if (work->motionFlags & 0x200U)
                allowOverflow = allowReset = 0;
            result = TimestepUpdate(g_motionRegionClocks[0], allowOverflow, allowReset);
        }
    } else {
        result = 0;
    }
    if (result) {
        if ((work->regionFlags[0] & MRF_Transition) && work->currentRootMode != 0 && work->currentRootMode != 2) {
            work->regionFlags[0] &= 0xffefU;
            work->transitionTimer = -1.0f;
        } else if ((work->regionFlags[0] & MRF_Transition) && work->currentRootMode == 2) {
            work->regionFlags[0] &= 0xffefU;
            work->transitionTimer = -1.0f;
        }
    }
    if (result == 4) {
        BuildChannelTransformAtTime(FrameRig(work).lowerBody, &channels, g_motionRegionClocks[0]->current.maxTime, MI_HermiteAuto);
        end.x = channels.rows[3].x + FrameRig(work).lowerBody.pelvisTransform.rows[3].x;
        end.z = channels.rows[3].z + FrameRig(work).lowerBody.pelvisTransform.rows[3].z;
        g_motionRegionClocks[0]->current.ticksTime = 0.0f;
        mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, LowerBody);
        reread = 1;
        BuildChannelTransformAtTime(FrameRig(work).lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
        FrameRig(work).lowerBody.pelvisTransform.rows[3].x = -(channels.rows[3].x - end.x);
        FrameRig(work).lowerBody.pelvisTransform.rows[3].z = -(channels.rows[3].z - end.z);
        work->regionFlags[0] |= MRF_Unknown01;
    } else if (result == 3) {
        BuildChannelTransformAtTime(FrameRig(work).lowerBody, &channels, ::ceilf(g_motionRegionClocks[0]->current.maxTime), MI_HermiteAuto);
        end.x = channels.rows[3].x + FrameRig(work).lowerBody.pelvisTransform.rows[3].x;
        end.z = channels.rows[3].z + FrameRig(work).lowerBody.pelvisTransform.rows[3].z;
        mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, LowerBody);
        reread = 1;
        BuildChannelTransformAtTime(FrameRig(work).lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
        FrameRig(work).lowerBody.pelvisTransform.rows[3].x = -(channels.rows[3].x - end.x);
        FrameRig(work).lowerBody.pelvisTransform.rows[3].z = -(channels.rows[3].z - end.z);
    } else if (result == 2) {
        mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, LowerBody);
        reread = 1;
        BuildChannelTransformAtTime(FrameRig(work).lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
        FrameRig(work).lowerBody.pelvisTransform.rows[3].x = -channels.rows[3].x;
        FrameRig(work).lowerBody.pelvisTransform.rows[3].z = -channels.rows[3].z;
    }
    if (work->blendRemainingFrames[0] && work->currentRootMode == 2) {
        work->blendRemainingFrames[0]--;
        work->layerBlendAlpha[0] = 1.0f - static_cast<float>(work->blendRemainingFrames[0]) / static_cast<float>(work->blendTotalFrames[0]);
        if (!work->blendRemainingFrames[0]) {
            mtwkClearOverrideCallbacksForBodyRegion(work, LowerBody);
            work->blendTotalFrames[0] = 0;
            work->regionFlags[0] &= 0xffefU;
        }
    } else {
        work->layerBlendAlpha[0] = 1.0f;
        if (work->previousRootMode == 2 && work->currentRootMode != 2)
            mtwkClearOverrideCallbacksForBodyRegion(work, LowerBody);
    }
    for (int region = 1; region < 5; ++region) {
        if ((work->regionFlags[region] & MRF_Independent) && !g_motionLayerEvaluated[region]) {
            if ((work->regionFlags[region] & MRF_Transition) && work->currentRootMode != 0 && work->currentRootMode != 2) {
                result = AnimClock_ForceJump(g_motionRegionClocks[region]);
            } else if ((work->regionFlags[region] & MRF_Transition) && work->currentRootMode == 0) {
                result = 4;
                g_motionRegionClocks[region]->current.ticksTime = g_motionRegionClocks[region]->current.maxTime;
            } else {
                std::uint32_t allowOverflow = work->regionFlags[region] & MRF_Loop;
                std::uint32_t allowReset = work->regionFlags[region] & MRF_ExcludeEndFrame;
                if (work->motionFlags & 0x200U)
                    allowOverflow = allowReset = 0;
                result = TimestepUpdate(g_motionRegionClocks[region], allowOverflow, allowReset);
            }
            if (result) {
                if ((work->regionFlags[region] & MRF_Transition) && work->currentRootMode != 0 && work->currentRootMode != 2) {
                    work->regionFlags[region] &= 0xffefU;
                    if (result == 4) {
                        work->regionFlags[region] |= MRF_Unknown01;
                        if (work->regionFlags[region] & MRF_BlendToLowerBody) {
                            work->regionFlags[region] &= 0xf7ffU;
                            work->regionFlags[region] &= 0xfeffU;
                        }
                    }
                } else if ((work->regionFlags[region] & MRF_Transition) && work->currentRootMode == 0) {
                    work->regionFlags[region] &= 0xffefU;
                    work->regionFlags[region] |= MRF_Unknown01;
                    if (work->regionFlags[region] & MRF_BlendToLowerBody) {
                        work->regionFlags[region] &= 0xf7ffU;
                        work->regionFlags[region] &= 0xfeffU;
                    }
                }
            }
            if (result > 1) {
                g_motionRegionClocks[region]->current.ticksTime = 0.0f;
                mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
            }
        } else {
            g_motionRegionClocks[region]->current.ticksTime = g_motionRegionClocks[0]->current.ticksTime;
            if (reread) {
                mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
                if (work->regionFlags[0] & MRF_Unknown01)
                    work->regionFlags[region] |= MRF_Unknown01;
            }
            if (!(work->regionFlags[0] & MRF_Transition))
                work->regionFlags[region] &= 0xffefU;
        }
        if (work->blendRemainingFrames[region] && work->currentRootMode == 2) {
            work->blendRemainingFrames[region]--;
            work->layerBlendAlpha[region] = 1.0f - static_cast<float>(work->blendRemainingFrames[region]) / static_cast<float>(work->blendTotalFrames[region]);
            if (!work->blendRemainingFrames[region]) {
                mtwkClearOverrideCallbacksForBodyRegion(work, static_cast<MotionRegion>(region));
                work->blendTotalFrames[region] = 0;
                work->regionFlags[region] &= 0xffefU;
            }
            if (work->regionFlags[region] & MRF_Independent)
                work->layerBlendAlpha[region] = 1.0f;
        } else {
            work->layerBlendAlpha[region] = 1.0f;
            if (work->previousRootMode == 2 && work->currentRootMode != 2)
                mtwkClearOverrideCallbacksForBodyRegion(work, static_cast<MotionRegion>(region));
        }
    }
    if (work->transitionTimer >= 0.0f) {
        work->transitionTimer += work->baseTimeScale;
        if (work->transitionTimer > work->transitionTimerLimit) {
            work->transitionTimer = work->transitionTimerReset;
            work->previousMotionId[0] = work->currentMotionId[0];
        }
    }
}

void mtwkApplySequenceCommands(MotionWork *work) {
    for (std::int32_t region = 0; region < 5; ++region) {
        if ((work->regionFlags[region] & MRF_Unknown01) && (!(work->regionFlags[region] & MRF_Transition) || work->currentRootMode == 2)) {
            if (MOTN_GetLoopFrame(work->currentMotionId[region]) == 0xffffU)
                work->regionFlags[region] |= 4U;
            else if (work->regionFlags[region] & 0x1000U)
                work->regionFlags[region] |= 4U;
            else
                work->regionFlags[region] &= 0xfffbU;
            if (MOTN_HasHoldCommand(work->currentMotionId[region])) {
                float holdTime;
                work->regionFlags[region] |= 8U;
                mtwkCopyRegionTimes(work, 0, 0, &holdTime, static_cast<MotionRegion>(region));
                mtwkTimePairClampAndPropagate(work, -1.0f, -1.0f, holdTime - 0.001f, static_cast<MotionRegion>(region));
            } else
                work->regionFlags[region] &= 0xfff7U;
        }
        if ((work->regionFlags[region] & MRF_Transition) && work->currentRootMode != 2)
            continue;
        if (work->layerLoopStartTime[region] < 0.0f && work->pendingLayerStartFrame[region] < 0.0f && work->layerSampleFrame[region] < 0.0f &&
            work->layerEndFrame[region] < 0.0f)
            continue;
        const float previousTicks = g_motionRegionClocks[region]->current.ticksTime;
        if (work->layerSampleFrame[region] >= 0.0f)
            work->pendingLayerStartFrame[region] = work->layerSampleFrame[region];
        mtwkAssignStaticDataForBodyRegion(
            work, work->layerLoopStartTime[region], work->pendingLayerStartFrame[region], work->layerEndFrame[region], static_cast<MotionRegion>(region));
        const float current = mtwkTimePairClampAndPropagate(
            work, work->layerLoopStartTime[region], work->pendingLayerStartFrame[region], work->layerEndFrame[region], static_cast<MotionRegion>(region));
        if (previousTicks > current && (work->regionFlags[region] & MRF_Unknown01) == 0) {
            mtwkRigEvaluateControllers(work, 0, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
            mtwkTimePairClampAndPropagate(work, -1.0f, current, -1.0f, static_cast<MotionRegion>(region));
        }
        if (region == 0) {
            if (work->pendingLayerStartFrame[region] > 0.0f) {
                g_motionLayerEvaluated[region] = 1;
                if (work->motionFlags & 0x4000U) {
                    FrameRig(work).lowerBody.pelvisTransform.rows[3].x = 0.0f;
                    FrameRig(work).lowerBody.pelvisTransform.rows[3].y = 0.0f;
                    FrameRig(work).lowerBody.pelvisTransform.rows[3].z = 0.0f;
                } else if (work->regionFlags[region] & MRF_Unknown01) {
                    IKRig &rig = FrameRig(work);
                    Vector3 offset;
                    Matrix4x4 channels;
                    offset.x = rig.lowerBody.pelvisTransform.rows[3].x;
                    offset.z = rig.lowerBody.pelvisTransform.rows[3].z;
                    BuildChannelTransformAtTime(rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
                    offset.x += channels.rows[3].x;
                    offset.z += channels.rows[3].z;
                    TranslatePelvisFromChannels(rig.lowerBody, MI_Hermite);
                    rig.lowerBody.pelvisTransform.rows[3].x += offset.x;
                    rig.lowerBody.pelvisTransform.rows[3].z += offset.z;
                } else {
                    float earlier = 0.0f;
                    const float alpha = work->layerBlendAlpha[0];
                    if (current - FrameRig(work).lowerBody.clock.playbackSpeed > 0.0f)
                        earlier = current - FrameRig(work).lowerBody.clock.playbackSpeed;
                    mtwkTimePairClampAndPropagate(work, -1.0f, earlier, -1.0f, static_cast<MotionRegion>(region));
                    if (work->regionFlags[0] & MRF_Transition)
                        work->layerBlendAlpha[0] = 1.0f - static_cast<float>(work->blendRemainingFrames[0] + 1) / static_cast<float>(work->blendTotalFrames[0]);
                    TranslatePelvisFromChannels(FrameRig(work).lowerBody, MI_Hermite);
                    mtwkTimePairClampAndPropagate(work, -1.0f, current, -1.0f, static_cast<MotionRegion>(region));
                    work->layerBlendAlpha[0] = alpha;
                }
            }
            for (std::int32_t dependent = 1; dependent < 5; ++dependent)
                if (!(work->regionFlags[dependent] & MRF_Independent))
                    g_motionRegionClocks[dependent]->current.ticksTime = current;
        }
        work->layerLoopStartTime[region] = -1.0f;
        work->pendingLayerStartFrame[region] = -1.0f;
        work->layerEndFrame[region] = -1.0f;
        work->layerSampleFrame[region] = -1.0f;
    }
    work->motionFlags &= ~0x4000U;
    if (work->motionFlags & 0x200U) {
        FrameRig(work).lowerBody.pelvisTransform.rows[3].x = 0.0f;
        FrameRig(work).lowerBody.pelvisTransform.rows[3].y = 0.0f;
        FrameRig(work).lowerBody.pelvisTransform.rows[3].z = 0.0f;
    }
    if (work->regionFlags[0] & MRF_Unknown01)
        work->activeRootYawOffset = work->requestedRootYawOffset;
    if (work->activeRootYawOffset && !(work->regionFlags[0] & MRF_Transition)) {
        const MotionTimePair &pair = g_motionRegionClocks[0]->current;
        float span = (pair.maxTime - pair.splitTimeFrames) * 0.5f;
        const float elapsed = pair.ticksTime - pair.splitTimeFrames;
        std::int32_t angle;
        if (span > elapsed)
            angle = static_cast<std::int32_t>((static_cast<float>(static_cast<std::int16_t>(work->activeRootYawOffset) / 2) * elapsed) / span);
        else {
            const float yaw = static_cast<float>(static_cast<std::int16_t>(work->activeRootYawOffset));
            span += span;
            angle = static_cast<std::int16_t>(static_cast<std::int32_t>(yaw - ((span - elapsed) * yaw) / span));
        }
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(angle));
    }
}

void mtwkResolveAnimId(MotionWork *work, std::uint16_t motion, std::uint16_t remap, float begin, float end) {
    const std::int32_t walkFrames = static_cast<std::int32_t>(GetMotionFrameCount(0xf005U));
    const std::int32_t frames = static_cast<std::int32_t>(GetMotionFrameCount(motion));
    const float ratio = static_cast<float>(walkFrames - 1) / static_cast<float>(frames - 1);
    begin *= ratio;
    end *= ratio;
    const std::byte *const command = MOTN_FindFrameEvent(4, 0xf005U, begin, end, 1);
    if (command) {
        work->materialFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(command[3])));
        work->pendingFootstepCue = *reinterpret_cast<const std::uint32_t *>(command + 4);
        if (remap) {
            if (work->pendingFootstepCue == 0x800001U)
                work->pendingFootstepCue = 0x800004U;
            else if (work->pendingFootstepCue == 0x800000U)
                work->pendingFootstepCue = 0x800003U;
        }
    } else
        work->pendingFootstepCue = 0;
}

void mtwkUpdateBoneForTime(MotionWork *work, std::uint16_t motion, float begin, float end) {
    const std::byte *const command = MOTN_FindFrameEvent(4, motion, begin, end, 1);
    if (command) {
        work->materialFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(command[3])));
        work->pendingFootstepCue = reinterpret_cast<const std::uint32_t *>(command)[1];
    } else
        work->pendingFootstepCue = 0;
}

void mtwkSelectRandomFootstepCue(MotionWork *work, std::uint16_t motion, float begin, float end) {
    const std::byte *const command = MOTN_FindFrameEvent(49, motion, begin, end, 1);
    if (command) {
        work->materialFlags = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(command[3])));
        work->pendingFootstepCue = reinterpret_cast<const std::uint32_t *>(command)[1 + static_cast<std::int32_t>(dc::sdk::RandomFloat() * 3.0f)];
    } else
        work->pendingFootstepCue = 0;
}

const std::byte *MOTN_FindHandPoseEvent(std::uint16_t animationId, float begin, float end, std::int32_t occurrence) {
    const std::byte *const event = MOTN_FindFrameEvent(18, animationId, begin, end, occurrence);
    if (event)
        return event;
    return 0;
}

std::uint32_t MOTN_GetFrameEvent52Parameter(std::uint16_t animationId, float begin, float end) {
    const std::byte *const event = MOTN_FindFrameEvent(52, animationId, begin, end, 1);
    if (event)
        return *reinterpret_cast<const std::uint32_t *>(event + 4);
    return 0;
}

std::int32_t mtwkApplyInterpolatedTransforms(MotionWork *work) {
    if (work->regionFlags[0] & 1U) {
        work->layerPreviousTicks[0] = g_motionRegionClocks[0]->current.ticksTime - g_motionRegionClocks[0]->playbackSpeed;
        if (0.0f > work->layerPreviousTicks[0])
            work->layerPreviousTicks[0] = 0.0f;
    }
    float begin = work->layerPreviousTicks[0];
    std::int32_t index;
    for (index = 0; index < 5; ++index) {
        if (!(work->regionFlags[index] & MRF_Transition) || !work->currentRootMode || work->currentRootMode == 2) {
            MotionRigClock *const clock = g_motionRegionClocks[index];
            if (clock->current.ticksTime + clock->playbackSpeed > clock->current.maxTime)
                work->regionFlags[index] |= MRF_Unknown02;
            work->regionFlags[index] &= ~MRF_Unknown1000;
        }
        work->layerPreviousTicks[index] = g_motionRegionClocks[index]->current.ticksTime;
    }
    if (work->activeRootYawOffset && (work->regionFlags[0] & MRF_Unknown02) && (*reinterpret_cast<const std::uint32_t *>(work->regionFlags) & 0x0cU))
        work->activeRootYawOffset = 0;
    work->entityYaw = static_cast<std::int32_t>(mtwkApplyTransforms(&FrameRig(work).lowerBody.pelvisWorld));
    work->previousRootPosition.x = work->cachedRootPosition.x;
    work->previousRootPosition.z = work->cachedRootPosition.z;
    if ((work->motionFlags & 9U) == 1U) {
        work->cachedRootPosition.x = FrameRig(work).lowerBody.pelvisWorld.rows[3].x;
        work->cachedRootPosition.z = FrameRig(work).lowerBody.pelvisWorld.rows[3].z;
    }
    FrameRig(work).lowerBody.rootPrepassTransform.rows[3].x = work->cachedRootPosition.x;
    FrameRig(work).lowerBody.rootPrepassTransform.rows[3].z = work->cachedRootPosition.z;
    if (g_motionUseEnvironment && work->retainedMotionParameter && !((work->regionFlags[0] & MRF_Transition) && work->currentRootMode == 1)) {
        float end;
        std::uint16_t motion;
        if ((work->regionFlags[0] & MRF_Transition) && work->currentRootMode != 2) {
            begin = work->transitionTimer - work->baseTimeScale;
            end = work->transitionTimer;
            if (work->currentRootMode != 3)
                motion = work->previousMotionId[0];
            else
                motion = work->currentMotionId[0];
        } else {
            end = g_motionRegionClocks[0]->current.ticksTime;
            motion = work->currentMotionId[0];
        }
        switch (motion) {
        case 0xf006:
        case 0xf035 ... 0xf03a:
            if (begin > end) {
                mtwkResolveAnimId(work, motion, 1, begin, static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(motion))) - 1.0f);
                if (!work->pendingFootstepCue)
                    mtwkResolveAnimId(work, motion, 1, 0.0f, end);
            } else {
                mtwkResolveAnimId(work, motion, 1, begin, end);
            }
            break;
        case 0xf03b ... 0xf03d:
            if (begin > end) {
                mtwkResolveAnimId(work, motion, 0, begin, static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(motion))) - 1.0f);
                if (!work->pendingFootstepCue)
                    mtwkResolveAnimId(work, motion, 0, 0.0f, end);
            } else {
                mtwkResolveAnimId(work, motion, 0, begin, end);
            }
            break;
        default:
            if (begin > end) {
                mtwkUpdateBoneForTime(work, motion, begin, g_motionRegionClocks[0]->current.maxTime);
                if (!work->pendingFootstepCue)
                    mtwkSelectRandomFootstepCue(work, motion, begin, g_motionRegionClocks[0]->current.maxTime);
                if (!work->pendingFootstepCue) {
                    mtwkUpdateBoneForTime(work, motion, g_motionRegionClocks[0]->current.splitTimeFrames, end);
                    if (!work->pendingFootstepCue)
                        mtwkSelectRandomFootstepCue(work, motion, g_motionRegionClocks[0]->current.splitTimeFrames, end);
                }
            } else {
                mtwkUpdateBoneForTime(work, motion, begin, end);
                if (!work->pendingFootstepCue)
                    mtwkSelectRandomFootstepCue(work, motion, begin, end);
            }
            break;
        }
    }
    if (work->regionFlags[0] & MRF_Unknown01)
        work->environmentFlags &= 0x0fU;
    if (work->environmentFlags & 0x0fU)
        work->environmentFlags = static_cast<std::uint8_t>((work->environmentFlags & 0x0fU) << 4);
    work->environmentFlags &= 0xf0U;
    work->parentBoneSelector = 9;
    for (index = 0; index < 9; ++index)
        work->latchedConstraintTarget[index] = 0;
    if (!(work->regionFlags[0] & MRF_Transition))
        work->requestedRootYawOffset = 0;
    work->gaitSplitFraction = -1.0f;
    work->motionFlags &= ~0x20000U;
    mtwkUpdateCollisionCluster(work);
    return 1;
}

std::int32_t mtwkEvaluateMotionFrame(MotionWork *work) {
    if (!work)
        return 0;
    std::uint32_t interpolationMode = MI_Hermite;
    matrix_stack::PushUnitMatrix();
    mtwkApplyRootPos(work);
    mtwkGetRegionClocks(work, reinterpret_cast<MotionRegionClock **>(&g_motionRegionClocks[0]),
        reinterpret_cast<MotionRegionClock **>(&g_motionRegionClocks[1]), reinterpret_cast<MotionRegionClock **>(&g_motionRegionClocks[2]),
        reinterpret_cast<MotionRegionClock **>(&g_motionRegionClocks[3]), reinterpret_cast<MotionRegionClock **>(&g_motionRegionClocks[4]));
    mtwkApplyRootTranslation(work);
    mtwkProcessLayers(work);
    mtwkAdvanceAnimFrameAndLayers(work);
    mtwkApplySequenceCommands(work);
    mtwkUpdateOverrideCallbacks(work);
    mtwkAssignCallbacks(work);
    if (work->sequenceFlags & 0x1000000U)
        interpolationMode = MI_Hermite | MI_Linear;
    Vector3 bonePosition;
    Vector3 solved;
    if ((work->motionFlags & 9U) != 1U) {
        interpolationMode |= MI_Hermite;
        TranslatePelvisFromChannels(FrameRig(work).lowerBody, MI_Hermite);
        if (work->motionFlags & 8U) {
            Matrix4x4 matrix;
            BuildChannelTransformAtTime(FrameRig(work).lowerBody, &matrix, g_motionRegionClocks[0]->current.ticksTime, MI_HermiteAuto);
            FrameRig(work).lowerBody.pelvisTransform.rows[3].y = -matrix.rows[3].y;
        } else {
            FrameRig(work).lowerBody.pelvisTransform.rows[3].y = 0.0f;
        }
    }
    if ((work->parentBoneSelector & 0x1fU) < 9U && !(work->motionFlags & 8U))
        mtwkGetControllerTargetPosition(work, work->parentBoneSelector & 0x1fU, bonePosition);
    g_ikEnabled = 1;
    IK_BuildAndSolve(FrameRig(work), interpolationMode);
    g_ikEnabled = 0;
    if ((work->parentBoneSelector & 0x1fU) < 9U && !(work->motionFlags & 8U)) {
        mtwkGetControllerTargetPosition(work, work->parentBoneSelector & 0x1fU, solved);
        bonePosition.x -= solved.x;
        bonePosition.y -= solved.y;
        bonePosition.z -= solved.z;
        matrix_stack::Matrix4x4_Push();
        matrix_stack::InvertMatrix(0);
        Matrix4x4::Transform(bonePosition, bonePosition);
        if (work->parentBoneSelector & 0x80U)
            bonePosition.y = 0.0f;
        if (work->parentBoneSelector & 0x40U)
            bonePosition.x = 0.0f;
        if (work->parentBoneSelector & 0x20U)
            bonePosition.z = 0.0f;
        PopCurrentMatrix(1);
        Matrix4x4::Transform(bonePosition, bonePosition);
        if ((work->motionFlags & 1U) == 0) {
            bonePosition.x = 0.0f;
            bonePosition.z = 0.0f;
        }
        mtwkAdvanceInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        mtwkCopyInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        FrameRig(work).lowerBody.pelvisTransform.rows[3].y = bonePosition.y;
    }
    mtwkApplyInterpolatedTransforms(work);
    PopCurrentMatrix(1);
    if (work->ikTargetPosition) {
        mtwkGetBonePosition(work, 12, &bonePosition);
        Matrix4x4 matrix = mtwkBuildArmRebaseTransform(work);
        work->transformStart = &matrix;
    }
    if (work->transformStart)
        mtwkRebaseSolvedMatrices(work);
    if (work->ikTargetPosition) {
        const Vector3 *target = work->ikTargetPosition;
        bonePosition.x = target->x - bonePosition.x;
        bonePosition.y = target->y - bonePosition.y;
        bonePosition.z = target->z - bonePosition.z;
        mtwkAdvanceInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        work->ikTargetPosition = 0;
        work->transformStart = 0;
    }
    if (!(work->sequenceFlags & MSF_SearchHand)) {
        if (!camCtrlIsInsideDepthScaledRect(work->cachedRootPosition, work->depthScale))
            return 0;
    }
    return 1;
}

std::int32_t GetMotionBankRange(std::int32_t index, void **data, std::uint16_t *firstAnimation, std::uint16_t *endAnimation) {
    const MotionRegistryEntry *bank = g_motionData;
    for (std::int32_t i = 0; bank; bank = bank->next, ++i) {
        if (i == index) {
            *data = bank->data;
            *firstAnimation = bank->firstAnimation;
            *endAnimation = bank->endAnimation;
            return 1;
        }
    }
    return 0;
}

std::int32_t MOTN_IsEndlessMotion(std::uint16_t animation);

std::int32_t MOTN_IsEndlessAnimationId(std::uint16_t animation) {
    return MOTN_IsEndlessMotion(animation);
}

} // namespace s2
} // namespace shenmue

#else

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
        if (poseMode == MP_FromChannels)
            ApplyRotationChannelsZYX(parameters.channels);
        else if (poseMode == MP_FromAxisAngle)
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

// The override rig's two-channel toe joints.
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
            mtwkGetControllerTargetPosition(&work, controller, bone);
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
                mtwkGetControllerTargetPosition(&work, controller, bone);
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
float mtwkTimePairClampAndPropagate(MotionWork *work, float split, float ticks, float maximum, MotionRegion region) {
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
void mtwkAssignStaticDataForBodyRegion(MotionWork *work, float split, float ticks, float maximum, MotionRegion region) {
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
    g_motionInterpolationType = MI_HermiteAuto;
    g_motionCurrentAnimTime = 0.0f > frame ? g_motionMaximumAnimTime : frame * motionFrameSeconds;
    float sampled[3];
    for (std::size_t axis = 0; axis < 3; ++axis)
        sampled[axis] = SampleMotionRigChannelAdvance(channels[axis].channel);
    output = {sampled[0], sampled[1], sampled[2]};
    return true;
}

// @verified - p
void mtwkRigEvaluateControllers(MotionWork *work, const MotionStreamContext *context, float sourceTime, float time, std::uint32_t flags, MotionRegion region) {
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
void mtwkRigRunOverrideBlendPass(MotionWork *work) {
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
            mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
    }
    TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
    state.overrideBlendTime = -1.0f;
}

// @verified - p
void mtwkClearOverrideCallbacksForBodyRegion(MotionWork *work, MotionRegion region) {
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
void mtwkInstallActiveOverrideCallbacks(MotionWork *work) {
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
void mtwkInstallConstraintTargetCallback(MotionWork *work, std::uint32_t controller) {
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
void mtwkClearConstraintTargetCallbackIfInactive(MotionWork *work, std::uint32_t controller) {
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

// Steps the pelvis yaw toward its target, ramping the step in over five
// frames and publishing the clamped half angle the chest counter-rotates by.
// @verified - p
void mtwkUpdatePelvisYaw(MotionWork *work) {
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
std::int32_t mtwkUpdateOverrideCallbacks(MotionWork *work) {
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
            mtwkInstallConstraintTargetCallback(work, controller);
            continue;
        }
        const std::uint32_t bits = state.constraintStateBits;
        if (!((bits >> shift) & 7U))
            continue;
        if ((bits >> shift) & 6U) {
            state.constraintStateBits = (4U << shift) | (bits & ~(6U << shift));
            mtwkInstallConstraintTargetCallback(work, controller);
        } else {
            state.constraintStateBits = bits & ~(7U << shift);
            mtwkClearConstraintTargetCallbackIfInactive(work, controller);
        }
    }
    if (!rig.useOverride) {
        if (state.pelvisYawCurrent || state.pelvisYawHalf || state.pelvisYawTarget) {
            BindCallback(rig.lowerBody.pelvisRotationCallback, ApplyRotY_ThenCopyOrRotZYX, &work->rotations[0]);
            BindCallback(rig.chest.upperRotation.callback, ApplyChannelRotZYX_WithIKXBias, &work->rotations[12]);
            mtwkUpdatePelvisYaw(work);
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
    mtwkInstallActiveOverrideCallbacks(work);
    return 1;
}

// A lead-foot (opcode 19) motion reports the 0x20 region flag.
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
void mtwkApplyRootTranslation(MotionWork *work) {
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
                        mtwkSetSpeed(work, g_motionRegionClocks[0]->playbackSpeed, static_cast<MotionRegion>(region));
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
std::int32_t mtwkProcessLayers(MotionWork *work) {
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
            mtwkRigEvaluateControllers(work, &context, loopStart, localTime, flags, static_cast<MotionRegion>(region));
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
                            TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
                    } else if (!(state.motnFlags & 0x4000U)) {
                        TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
                    }
                }
            } else {
                const float start = state.layerSampleFrame[0];
                if (0.0f > start) {
                    if (state.motnFlags & 0x2000U)
                        TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
                } else if (!(state.motnFlags & 0x4000U)) {
                    TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
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
        mtwkRigRunOverrideBlendPass(work);
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
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.maxTime, MI_HermiteAuto);
        const float endX = channels.rows[3].x + pelvis.x;
        const float endZ = channels.rows[3].z + pelvis.z;
        g_motionRegionClocks[0]->current.ticksTime = 0.0f;
        mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, MotionRegion::LowerBody);
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
        state.regionFlags[0] |= MRF_Unknown01;
        pelvis.x = -(channels.rows[3].x - endX);
        pelvis.z = -(channels.rows[3].z - endZ);
        reread = true;
    } else if (result == 3) {
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, ceil_float(g_motionRegionClocks[0]->current.maxTime), MI_HermiteAuto);
        const float endX = channels.rows[3].x + pelvis.x;
        const float endZ = channels.rows[3].z + pelvis.z;
        mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, MotionRegion::LowerBody);
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
        pelvis.x = -(channels.rows[3].x - endX);
        pelvis.z = -(channels.rows[3].z - endZ);
        reread = true;
    } else if (result == 2) {
        mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, MotionRegion::LowerBody);
        BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
        pelvis.x = -channels.rows[3].x;
        pelvis.z = -channels.rows[3].z;
        reread = true;
    }
    if (state.blendRemainingFrames[0] && state.currentRootMode == 2) {
        const std::uint16_t remaining = static_cast<std::uint16_t>(state.blendRemainingFrames[0] - 1U);
        state.blendRemainingFrames[0] = remaining;
        state.layerBlendAlpha[0] = 1.0f - static_cast<float>(remaining) / static_cast<float>(state.blendTotalFrames[0]);
        if (!remaining) {
            mtwkClearOverrideCallbacksForBodyRegion(work, MotionRegion::LowerBody);
            state.regionFlags[0] &= ~MRF_Transition;
            state.blendTotalFrames[0] = 0;
        }
    } else {
        state.layerBlendAlpha[0] = 1.0f;
        if (state.previousRootMode == 2 && state.currentRootMode != 2)
            mtwkClearOverrideCallbacksForBodyRegion(work, MotionRegion::LowerBody);
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
                    mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
                }
            }
        } else {
            clock->current.ticksTime = g_motionRegionClocks[0]->current.ticksTime;
            if (reread) {
                mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
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
                mtwkClearOverrideCallbacksForBodyRegion(work, static_cast<MotionRegion>(region));
                state.blendTotalFrames[region] = 0;
                flags &= 0xffefU;
            }
            if (flags & 0x100U)
                state.layerBlendAlpha[region] = 1.0f;
        } else {
            state.layerBlendAlpha[region] = 1.0f;
            if (state.previousRootMode == 2 && state.currentRootMode != 2)
                mtwkClearOverrideCallbacksForBodyRegion(work, static_cast<MotionRegion>(region));
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

// Native Sequence: applies the motion's loop (opcode 1) and hold (opcode 24) commands,
// any requested frame range, and the authored root yaw offset ramp.
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
                mtwkTimePairClampAndPropagate(work, -1.0f, -1.0f, holdTime - 0.001f, static_cast<MotionRegion>(region));
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
        mtwkAssignStaticDataForBodyRegion(
            work, loopStart, state.pendingLayerStartFrame[region], state.layerEndFrame[region], static_cast<MotionRegion>(region));
        const float current = mtwkTimePairClampAndPropagate(
            work, state.layerLoopStartTime[region], state.pendingLayerStartFrame[region], state.layerEndFrame[region], static_cast<MotionRegion>(region));
        if (previousTicks > current && !(flags & 1U)) {
            mtwkRigEvaluateControllers(work, nullptr, 0.0f, 0.0f, motionInitChannels, static_cast<MotionRegion>(region));
            mtwkTimePairClampAndPropagate(work, -1.0f, current, -1.0f, static_cast<MotionRegion>(region));
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
                    BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.splitTimeFrames, MI_HermiteAuto);
                    const float translatedX = x + channels.rows[3].x;
                    const float translatedZ = z + channels.rows[3].z;
                    TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
                    pelvis.x = translatedX + pelvis.x;
                    pelvis.z = translatedZ + pelvis.z;
                } else {
                    const float alpha = state.layerBlendAlpha[0];
                    const float earlier = current - shenGetScaledDeltaTickTime(work->rig.lowerBody.clock.playbackSpeed);
                    mtwkTimePairClampAndPropagate(work, -1.0f, earlier > 0.0f ? earlier : 0.0f, -1.0f, MotionRegion::LowerBody);
                    if (state.regionFlags[0] & MRF_Transition)
                        state.layerBlendAlpha[0] = 1.0f - static_cast<float>(state.blendRemainingFrames[0] + 1) / static_cast<float>(state.blendTotalFrames[0]);
                    TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
                    mtwkTimePairClampAndPropagate(work, -1.0f, current, -1.0f, MotionRegion::LowerBody);
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
void mtwkAdvanceInterpolated(MotionWork *work, float x, float y, float z) {
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
void mtwkCopyInterpolated(MotionWork *work, float x, float y, float z) {
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
void mtwkResolveAnimId(MotionWork *work, std::uint16_t motion, std::uint16_t remap, float begin, float end) {
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

// Opcode 49 carries three cues; the shared stream picks one.
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

// The mean ground position of the solved targets and their spread.
// @verified - p
void mtwkUpdateCollisionCluster(MotionWork *work) {
    IKRig &rig = work->rig;
    const std::uint8_t count = rig.useOverride ? 9U : 7U;
    Vector3 points[9]{};
    points[0] = {rig.lowerBody.pelvisWorld.rows[3].x, rig.lowerBody.pelvisWorld.rows[3].y, rig.lowerBody.pelvisWorld.rows[3].z};
    points[1] = TargetOutput(rig.chest.target);
    mtwkGetBonePosition(work, MB_Head, &points[2]);
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
                mtwkResolveAnimId(work, motion, remap, begin, static_cast<float>(MotionFrameCount(motion)) - 1.0f);
                if (!state.pendingFootstepCue)
                    mtwkResolveAnimId(work, motion, remap, 0.0f, end);
            } else {
                mtwkResolveAnimId(work, motion, remap, begin, end);
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
    mtwkUpdateCollisionCluster(work);
    return 1;
}

// The rebase frame for an IK hand target: the right forearm basis re-aimed along the target
// segment, expressed relative to the solved forearm and anchored at the forearm position.
// @verified - p
Matrix4x4 *mtwkBuildArmRebaseTransform(Matrix4x4 *destination, MotionWork *work) {
    Vector3 anchor;
    mtwkGetBonePosition(work, MB_RightForearm, &anchor);
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
    mtwkApplyRootTranslation(work);
    mtwkProcessLayers(work);
    mtwkAdvanceAnimFrameAndLayers(work);
    mtwkApplySequenceCommands(work);
    mtwkUpdateOverrideCallbacks(work);
    mtwkAssignCallbacks(work);
    const std::uint32_t interpolationMode = ((state.seqFlags >> motionSequenceInterpolationShift) & MI_Linear) | MI_Hermite;
    if ((state.motnFlags & 9U) != 1U) {
        TranslatePelvisFromChannels(work->rig.lowerBody, MI_Hermite);
        if (state.motnFlags & 8U) {
            Matrix4x4 channels;
            BuildChannelTransformAtTime(work->rig.lowerBody, &channels, g_motionRegionClocks[0]->current.ticksTime, MI_HermiteAuto);
            work->rig.lowerBody.pelvisTransform.rows[3].y = -channels.rows[3].y;
        } else {
            work->rig.lowerBody.pelvisTransform.rows[3].y = 0.0f;
        }
    }
    Vector3 bonePosition;
    if ((state.channelFlags & 0x1fU) < 9U && !(state.motnFlags & 8U))
        mtwkGetControllerTargetPosition(work, state.channelFlags & 0x1fU, bonePosition);
    g_ikEnabled = 1;
    IK_BuildAndSolve(work->rig, interpolationMode);
    g_ikEnabled = 0;
    if ((state.channelFlags & 0x1fU) < 9U && !(state.motnFlags & 8U)) {
        Vector3 solved;
        mtwkGetControllerTargetPosition(work, state.channelFlags & 0x1fU, solved);
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
        mtwkAdvanceInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        mtwkCopyInterpolated(work, bonePosition.x, bonePosition.y, bonePosition.z);
        work->rig.lowerBody.pelvisTransform.rows[3].y = bonePosition.y;
    }
    mtwkApplyInterpolatedTransforms(work);
    Matrix4x4::Pop(1);
    Matrix4x4 rebase;
    if (state.ikTargetPosition) {
        mtwkGetBonePosition(work, MB_RightForearm, &bonePosition);
        Matrix4x4 arm;
        rebase = *mtwkBuildArmRebaseTransform(&arm, work);
        state.transformStart = &rebase;
    }
    if (state.transformStart)
        mtwkRebaseSolvedMatrices(work);
    if (state.ikTargetPosition) {
        const Vector3 &target = state.ikTargetPosition[0];
        mtwkAdvanceInterpolated(work, target.x - bonePosition.x, target.y - bonePosition.y, target.z - bonePosition.z);
        state.ikTargetPosition = nullptr;
        state.transformStart = nullptr;
    }
    if (state.seqFlags & MSF_SearchHand)
        return 1;
    return camCtrlIsInsideDepthScaledRect(state.rootPosition, state.depthScale) ? 1 : 0;
}

} // namespace shenmue::s2

#endif
