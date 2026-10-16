#include "s2/motion_rig.hpp"

#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"

#include <bit>
#include <cmath>
#include <cstdio>

namespace shenmue::s2 {

namespace {


constexpr float motionRigDimensionTable[17][6]{
    {0.81f, 0.3977f, 0.1806f, 0.4686f, 0.7225f, 0.09f},
    {0.936f, 0.3237f, 0.178f, 0.4831f, 0.8455f, 0.09f},
    {0.841f, 0.4198f, 0.178f, 0.463f, 0.7584f, 0.09f},
    {0.429f, 0.2657f, 0.125f, 0.2958f, 0.3269f, 0.0893f},
    {0.578f, 0.2815f, 0.139f, 0.3793f, 0.473f, 0.0903f},
    {0.718f, 0.3215f, 0.1461f, 0.4071f, 0.6285f, 0.0899f},
    {0.799f, 0.4223f, 0.1985f, 0.5149f, 0.6784f, 0.1199f},
    {0.8823f, 0.4088f, 0.2094f, 0.5072f, 0.7636f, 0.1202f},
    {0.96f, 0.377f, 0.2073f, 0.5278f, 0.841f, 0.1195f},
    {0.965f, 0.446f, 0.2094f, 0.5447f, 0.8187f, 0.1198f},
    {1.028f, 0.5065f, 0.223f, 0.5802f, 0.8796f, 0.12f},
    {1.11f, 0.5776f, 0.2753f, 0.6736f, 0.9711f, 0.1395f},
    {0.87f, 0.4079f, 0.221f, 0.5354f, 0.8135f, 0.1201f},
    {0.81f, 0.3462f, 0.177f, 0.4592f, 0.7797f, 0.0908f},
    {0.952f, 0.3367f, 0.1851f, 0.5025f, 0.8625f, 0.0891f},
    {0.9805f, 0.4561f, 0.2073f, 0.5278f, 0.841f, 0.14f},
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
};

constexpr float motionFrameSeconds = 0.033333335f;


std::uint16_t RigAngle(float radians) {
    return FloatToShort((radians * 57.29578f) * 182.04443f);
}

void ApplyChannelRotationZYX(MotionRigAxis *axes) {
    Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[0].channel)));
    Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[1].channel)));
    Matrix4x4::RotateAroundX_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[2].channel)));
}

// The solvers' inlined joint evaluation: override callback, channels or blend basis.
void EvaluateJointRotation(MotionRigAxis *axes, MotionRotationBasis &basis, MotionCallback &callback, std::uint32_t poseMode) {
    if (callback.evaluate) {
        callback.evaluate(callback.parameters);
    } else if (poseMode == motionPoseFromChannels) {
        ApplyChannelRotationZYX(axes);
    } else if (poseMode == motionPoseFromAxisAngle) {
        AxisAngleToBasisSlerp(basis);
    }
}

void EvaluateTwoJointRotation(MotionRigTwoJoint &node, std::uint32_t poseMode) {
    if (node.callback.evaluate) {
        node.callback.evaluate(node.callback.parameters);
    } else if (poseMode == motionPoseFromChannels) {
        Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(node.axes[0].channel)));
        Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(node.axes[1].channel)));
    } else if (poseMode == motionPoseFromAxisAngle) {
        AxisAngleToBasisSlerp(node.basis);
    }
}

void CaptureSourceColumns(MotionRotationBasis &basis) {
    const Matrix4x4 &work = workTransformMatrix;
    basis.sourceAxis = {work.rows[0].x, work.rows[1].x, work.rows[2].x};
    basis.sourceTangent = {work.rows[0].y, work.rows[1].y, work.rows[2].y};
}

void CaptureTargetColumns(MotionRotationBasis &basis) {
    const Matrix4x4 &work = workTransformMatrix;
    basis.targetAxis = {work.rows[0].x, work.rows[1].x, work.rows[2].x};
    basis.targetTangent = {work.rows[0].y, work.rows[1].y, work.rows[2].y};
}

Vector3 TransformWorkPoint(const Vector3 &point) {
    Vector3 output;
    Matrix4x4::TransformPoint(point, output);
    return output;
}

void SampleTargetChannels(MotionRigTarget &target, Vector3 &point) {
    point.x = SampleMotionRigChannelAdvance(target.axes[0].channel);
    point.y = SampleMotionRigChannelAdvance(target.axes[1].channel);
    point.z = SampleMotionRigChannelAdvance(target.axes[2].channel);
}

// A target callback receives its output pointer in the first parameter word.
void EvaluateTargetCallback(MotionCallback &callback, Vector3 *output) {
    *static_cast<Vector3 **>(callback.parameters) = output;
    callback.evaluate(callback.parameters);
}

void StoreTargetOutput(MotionRigTarget &target, const Vector3 &point) {
    target.axes[2].dimensions[0] = point.x;
    target.axes[2].dimensions[1] = point.y;
    target.outputZ = point.z;
}


float ClampCosine(float cosine) {
    if (cosine > 1.0f)
        return 1.0f;
    return -1.0f > cosine ? -1.0f : cosine;
}

void Matrix4x3_MatrixToAxisAngle(const MotionRotationBasis &basis, float time);
void Matrix4x3_OrthonormalBasisFromAxis(const MotionRotationBasis &basis, float time);

Vector3 g_motionRotationAxis{};
Vector3 g_motionBasisAxis{};
Vector3 g_motionBasisBinormal{};
Vector3 g_motionBasisTangent{};
Matrix4x4 g_motionAxisAngleBasis{.rows = {{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};

} // namespace

float g_motionCurrentAnimTime{};
float g_motionMaximumAnimTime{};
float g_motionPreviousAnimTime{};
std::uint32_t g_motionInterpolationType{};
std::uint32_t g_motionInitFlags{};
std::uint16_t g_motionAnimFlags{};
const std::uint8_t *g_motionKeyCounts{};
const std::uint16_t *g_motionWideKeyCounts{};
const std::uint16_t *g_motionKeyData{};
std::int8_t g_motionGraphFrames{};
std::int32_t g_motionUseOverride{};
std::uint32_t g_motionRigBoneId{};
std::uint32_t g_motionPoseMode{};
float g_motionOverrideBlendTime = -1.0f;
float g_motionGaitSplitFraction = 0.5f;
Matrix4x4 g_motionOverrideRootTransform{};
Matrix4x4 g_motionOverrideBakeTransform{};
MotionOverride *g_customMotionOverride{};
const float (*g_motionRigDimensions)[6] = motionRigDimensionTable;

// @verified - p
void ReadMotionKeyBlock(MotionRigChannel &channel) {
    std::uint32_t count;
    if (g_motionInitFlags & motionInitStream) {
        if (g_motionAnimFlags & motionWideKeyCounts) {
            count = *g_motionWideKeyCounts + 2U;
            channel.keyCount = count;
            ++g_motionWideKeyCounts;
        } else {
            count = *g_motionKeyCounts + 2U;
            channel.keyCount = count;
            ++g_motionKeyCounts;
        }
        channel.keyBlock = g_motionKeyData;
    } else {
        count = channel.keyCount;
        g_motionKeyData = channel.keyBlock;
    }
    std::uint16_t first[3], second[3];
    first[0] = *g_motionKeyData++;
    first[1] = *g_motionKeyData++;
    first[2] = (first[0] & 1U) ? *g_motionKeyData++ : first[1];
    float firstTime = 0.0f;
    second[0] = *g_motionKeyData++;
    second[1] = *g_motionKeyData++;
    second[2] = (second[0] & 1U) ? *g_motionKeyData++ : second[1];
    std::uint32_t remaining = count - 2U;
    float secondTime = g_motionMaximumAnimTime;
    while (remaining != 0) {
        const float time = static_cast<float>(static_cast<std::int16_t>(*g_motionKeyData++)) * motionFrameSeconds;
        if (!(g_motionPreviousAnimTime > time)) {
            secondTime = time;
            break;
        }
        first[0] = second[0];
        first[1] = second[1];
        first[2] = second[2];
        firstTime = time;
        second[0] = *g_motionKeyData++;
        second[1] = *g_motionKeyData++;
        second[2] = (second[0] & 1U) ? *g_motionKeyData++ : second[1];
        --remaining;
    }
    channel.current.value = DecodeMotnFloat(first[0]) * channel.scale + channel.bias;
    float tangent = DecodeMotnFloat(first[1]) * channel.scale;
    channel.current.tangentIn = tangent;
    if (first[0] & 1U)
        tangent = DecodeMotnFloat(first[2]) * channel.scale;
    channel.current.tangentOut = tangent;
    channel.current.timeSeconds = firstTime;
    channel.next.value = DecodeMotnFloat(second[0]) * channel.scale + channel.bias;
    tangent = DecodeMotnFloat(second[1]) * channel.scale;
    channel.next.tangentIn = tangent;
    if (second[0] & 1U)
        tangent = DecodeMotnFloat(second[2]) * channel.scale;
    channel.next.tangentOut = tangent;
    channel.next.timeSeconds = secondTime;
    const float span = secondTime - firstTime;
    channel.inverseDuration = std::fabs(span) > 0.000001f ? 1.0f / span : 0.0f;
    channel.keysRemaining = remaining;
    channel.frameCursor = g_motionKeyData;
    if ((g_motionInitFlags & motionInitStream) && remaining != 0) {
        for (;;) {
            const std::uint16_t value = *g_motionKeyData;
            --remaining;
            g_motionKeyData += 2;
            if (value & 1U)
                ++g_motionKeyData;
            if (remaining == 0)
                break;
            ++g_motionKeyData;
        }
    }
}

// InitMemoryForNaomiLibMotion: step the streams past one channel.
// @verified - p
void SkipMotionKeyBlock() {
    if (!(g_motionInitFlags & motionInitStream))
        return;
    std::uint32_t count;
    if (g_motionAnimFlags & motionWideKeyCounts)
        count = *g_motionWideKeyCounts++;
    else
        count = *g_motionKeyCounts++;
    std::uint16_t value = *g_motionKeyData;
    g_motionKeyData += 2;
    if (value & 1U)
        ++g_motionKeyData;
    for (;;) {
        value = *g_motionKeyData;
        g_motionKeyData += 2;
        if (value & 1U)
            ++g_motionKeyData;
        if (count == 0)
            return;
        --count;
        ++g_motionKeyData;
    }
}

// SetNaomiLibMotionFlags.
// @verified - p
void SetMotionStreamFlags(std::int32_t channels, std::uint32_t initFlags, std::uint16_t animFlags) {
    g_motionInitFlags = initFlags;
    g_motionAnimFlags = animFlags;
    for (; channels != 0; --channels)
        SkipMotionKeyBlock();
}

// Eval_HermiteOrLinear.
// @verified - p
float EvaluateMotionRigChannelValue(const MotionRigChannel &channel) {
    const float time = g_motionCurrentAnimTime - channel.current.timeSeconds;
    const float inverse = channel.inverseDuration;
    if (g_motionInterpolationType & motionInterpolationLinear)
        return ((channel.next.value - channel.current.value) * inverse) * time + channel.current.value;
    const float timeSquared = time * time;
    const float inverseSquared = inverse * inverse;
    const float cubic = (timeSquared * time) * inverseSquared;
    const float twiceCubic = (cubic + cubic) * inverse;
    const float tripleSquared = (timeSquared * 3.0f) * inverseSquared;
    const float scaledSquared = timeSquared * inverse;
    return (((twiceCubic - tripleSquared + 1.0f) * channel.current.value + (tripleSquared - twiceCubic) * channel.next.value) +
               ((cubic - scaledSquared - scaledSquared) + time) * channel.current.tangentOut) +
        (cubic - scaledSquared) * channel.next.tangentIn;
}

// Channel_SampleAdvance.
// @verified - p
float SampleMotionRigChannelAdvance(MotionRigChannel &channel) {
    if (channel.current.timeSeconds >= g_motionCurrentAnimTime)
        return channel.current.value;
    if (channel.next.timeSeconds > g_motionCurrentAnimTime)
        return EvaluateMotionRigChannelValue(channel);
    while (!(std::fabs(channel.next.timeSeconds - g_motionCurrentAnimTime) < 0.000001f) && channel.keysRemaining != 0) {
        channel.current = channel.next;
        const std::uint16_t rawValue = *channel.frameCursor++;
        channel.next.value = channel.scale * DecodeMotnFloat(rawValue) + channel.bias;
        const float tangent = channel.scale * DecodeMotnFloat(*channel.frameCursor++);
        channel.next.tangentIn = tangent;
        channel.next.tangentOut = (rawValue & 1U) != 0 ? channel.scale * DecodeMotnFloat(*channel.frameCursor++) : tangent;
        --channel.keysRemaining;
        channel.next.timeSeconds =
            channel.keysRemaining != 0 ? static_cast<float>(static_cast<std::int16_t>(*channel.frameCursor++)) * motionFrameSeconds : g_motionMaximumAnimTime;
        const float duration = channel.next.timeSeconds - channel.current.timeSeconds;
        channel.inverseDuration = std::fabs(duration) > 0.000001f ? 1.0f / duration : 0.0f;
        if (channel.next.timeSeconds > g_motionCurrentAnimTime)
            return EvaluateMotionRigChannelValue(channel);
    }
    return channel.next.value;
}

// Channel_InterpPVA_Scalar: value and half tangents at the current time.
void SampleMotionRigChannelPVA(MotionRigChannel &channel, float *value, float *tangentIn, float *tangentOut) {
    float now = g_motionCurrentAnimTime;
    if (!(channel.current.timeSeconds <= now)) {
        *value = channel.current.value;
        *tangentOut = 0.0f;
        *tangentIn = 0.0f;
        return;
    }
    float next = channel.next.timeSeconds;
    if (!(next > now)) {
        for (;;) {
            if (std::fabs(next - now) < 1.1754944e-38f) {
                *value = channel.next.value;
                *tangentIn = channel.next.tangentIn * 0.5f;
                *tangentOut = channel.next.tangentOut * 0.5f;
                return;
            }
            const std::uint32_t keys = channel.keysRemaining;
            if (keys == 0) {
                *value = channel.next.value;
                *tangentOut = 0.0f;
                *tangentIn = 0.0f;
                return;
            }
            channel.current = channel.next;
            const std::uint16_t rawValue = *channel.frameCursor++;
            channel.next.value = channel.scale * DecodeMotnFloat(rawValue) + channel.bias;
            const float tangent = channel.scale * DecodeMotnFloat(*channel.frameCursor++);
            channel.next.tangentIn = tangent;
            channel.next.tangentOut = (rawValue & 1U) != 0 ? channel.scale * DecodeMotnFloat(*channel.frameCursor++) : tangent;
            channel.keysRemaining = keys - 1U;
            const float time =
                keys == 1U ? g_motionMaximumAnimTime : static_cast<float>(static_cast<std::int16_t>(*channel.frameCursor++)) * motionFrameSeconds;
            channel.next.timeSeconds = time;
            next = time;
            channel.inverseDuration = 1.0f / (time - channel.current.timeSeconds);
            now = g_motionCurrentAnimTime;
            if (time > now)
                break;
        }
    }
    const float elapsed = now - channel.current.timeSeconds;
    if (g_motionInterpolationType & motionInterpolationLinear) {
        const float slope = (channel.next.value - channel.current.value) * channel.inverseDuration;
        const float half = slope * 0.5f;
        *value = elapsed * slope + channel.current.value;
        *tangentOut = half;
        *tangentIn = half;
        return;
    }
    const float inverse = channel.inverseDuration;
    const float nextValue = channel.next.value, nextTangent = channel.next.tangentIn;
    const float currentValue = channel.current.value, currentTangent = channel.current.tangentOut;
    const float inverseSquared = inverse * inverse;
    const float elapsedSquared = elapsed * elapsed;
    const float cubic = (elapsedSquared * elapsed) * inverseSquared;
    const float tripleSquared = (elapsedSquared * 3.0f) * inverseSquared;
    const float acceleration = ((elapsedSquared * 6.0f) * inverseSquared) * inverse;
    const float sixfold = (elapsed * 6.0f) * inverseSquared;
    const float twiceScaled = (elapsed + elapsed) * inverse;
    const float tangent = ((((sixfold - acceleration) * nextValue + (acceleration - sixfold) * currentValue) +
                               ((tripleSquared - twiceScaled) - twiceScaled + 1.0f) * currentTangent) +
                              (tripleSquared - twiceScaled) * nextTangent) *
        0.5f;
    *tangentOut = tangent;
    *tangentIn = tangent;
    const float scaledSquared = elapsedSquared * inverse;
    *value = ((((cubic + cubic) * inverse - tripleSquared + 1.0f) * currentValue + (tripleSquared - (cubic + cubic) * inverse) * nextValue) +
                 ((cubic - scaledSquared - scaledSquared) + elapsed) * currentTangent) +
        (cubic - scaledSquared) * nextTangent;
}

// @verified - p
void UpdatePointersForCurrentFrame(MotionStreamContext *context, const std::uint16_t *header, void **armTrack, void **regionTrack) {
    context->version = DecodeMotnFloat(header[0]);
    const std::uint16_t flags = header[1];
    context->flags = flags;
    const bool overrideRig = static_cast<std::uint8_t>(flags) == 16U;
    context->frameCount = header[2];
    const std::int16_t keyData = static_cast<std::int16_t>(header[3]);
    if (flags & motionWideKeyCounts)
        g_motionWideKeyCounts = header + 4;
    else
        g_motionKeyCounts = reinterpret_cast<const std::uint8_t *>(header + 4);
    g_motionKeyData = reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(header) + keyData);
    const std::int32_t regionChannels[4]{overrideRig ? 42 : 27, overrideRig ? 6 : 9, overrideRig ? 24 : 9, 12};
    for (std::size_t region = 0; region < 5; ++region) {
        context->regions[region].keyCounts =
            (context->flags & motionWideKeyCounts) ? static_cast<const void *>(g_motionWideKeyCounts) : static_cast<const void *>(g_motionKeyCounts);
        context->regions[region].keyData = g_motionKeyData;
        if (region < 4)
            SetMotionStreamFlags(regionChannels[region], motionInitStream, context->flags);
    }
    const std::uint16_t contextFlags = context->flags;
    if (!(contextFlags & 0xc000U)) {
        if (armTrack)
            *armTrack = nullptr;
        *regionTrack = nullptr;
        return;
    }
    SetMotionStreamFlags(12, motionInitStream, contextFlags);
    if (contextFlags & 0x4000U) {
        std::uint16_t *const arm = const_cast<std::uint16_t *>(g_motionKeyData);
        if (armTrack)
            *armTrack = arm;
        if (contextFlags & 0x8000U) {
            const std::uint16_t firstCount = arm[0];
            const std::uint16_t secondCount = arm[1 + firstCount * 2U];
            *regionTrack = arm + 2 + (firstCount + secondCount) * 2U;
            return;
        }
        *regionTrack = nullptr;
        return;
    }
    if (!(contextFlags & 0x8000U))
        return;
    if (armTrack)
        *armTrack = nullptr;
    *regionTrack = const_cast<std::uint16_t *>(g_motionKeyData);
}

// @verified - p
float mtwkGetScaleFactorFor(std::uint32_t targetRig, std::uint32_t sourceRig, std::uint32_t region) {
    if (targetRig == sourceRig)
        return 1.0f;
    return g_motionRigDimensions[static_cast<std::int32_t>(targetRig)][static_cast<std::int32_t>(region)] /
        g_motionRigDimensions[static_cast<std::int32_t>(sourceRig)][static_cast<std::int32_t>(region)];
}

// @verified - p
void SetOverrideBlendTime(float time) {
    g_motionOverrideBlendTime = std::fmax(0.1f, time);
}

// @verified - p
void SetCustomMotionOverride(MotionOverride *override) {
    g_customMotionOverride = override;
}

// @verified - p
void SetGaitSplitFraction(float fraction) {
    if (fraction >= 0.1f)
        g_motionGaitSplitFraction = std::fmin(1.0f, fraction);
    else
        g_motionGaitSplitFraction = 0.1f;
}

// Aims the work frame's X axis at a world point; returns the point's distance.
// @verified - p
float AimBasisAtTarget(const Vector3 &target) {
    const Matrix4x4 &work = workTransformMatrix;
    const float x = target.x - work.rows[3].x;
    const float y = target.y - work.rows[3].y;
    const float z = target.z - work.rows[3].z;
    const float localX = (work.rows[0].y * y + work.rows[0].x * x) + work.rows[0].z * z;
    const float localY = (work.rows[1].y * y + work.rows[1].x * x) + work.rows[1].z * z;
    const float localZ = (work.rows[2].y * y + work.rows[2].x * x) + work.rows[2].z * z;
    const float planarSquared = localY * localY + localX * localX;
    const float planar = std::sqrt(planarSquared);
    const float planarInverse = 1.0f / planar;
    Matrix4x4::RotateAroundZ(planarInverse * localY, planarInverse * localX);
    const float distance = std::sqrt(localZ * localZ + planarSquared);
    const float inverse = 1.0f / distance;
    Matrix4x4::RotateAroundY(-(inverse * localZ), inverse * planar);
    return distance;
}

// Aims the work frame's X axis along a local direction; returns its length.
// @verified - p
float AlignFrameToTarget_Vector(const Vector3 &target) {
    const float x = target.x, y = target.y, z = target.z;
    const float planarSquared = y * y + x * x;
    const float planar = std::sqrt(planarSquared);
    const float planarInverse = 1.0f / planar;
    Matrix4x4::RotateAroundZ(planarInverse * y, planarInverse * x);
    const float length = std::sqrt(z * z + planarSquared);
    const float inverse = 1.0f / length;
    Matrix4x4::RotateAroundY(-(inverse * z), inverse * planar);
    return length;
}

namespace {

// Matrix4x3::MatrixToAxisAngle: rotate the source axis toward the target axis by the
// normalized time, through a pushed prepass frame.
void Matrix4x3_MatrixToAxisAngle(const MotionRotationBasis &basis, float time) {
    const Vector3 &target = basis.targetAxis;
    const Vector3 &source = basis.sourceAxis;
    const float axisX = target.z * source.y - source.z * target.y;
    const float dot = (target.x * source.x + target.y * source.y) + target.z * source.z;
    g_motionRotationAxis.x = axisX;
    const float axisY = source.z * target.x - source.x * target.z;
    g_motionRotationAxis.y = axisY;
    const float axisZ = source.x * target.y - target.x * source.y;
    g_motionRotationAxis.z = axisZ;
    const float length = std::sqrt((axisY * axisY + axisX * axisX) + axisZ * axisZ);
    std::uint16_t angle;
    if (std::fabs(axisX) > 0.000001f || std::fabs(axisY) > 0.000001f || std::fabs(axisZ) > 0.000001f) {
        angle = FloatToShort(static_cast<float>(FixedAtan2(length, dot)) * time);
    } else {
        g_motionRotationAxis.x = (basis.sourceTangent.x + basis.targetTangent.x) * 0.5f;
        g_motionRotationAxis.y = (basis.sourceTangent.y + basis.targetTangent.y) * 0.5f;
        g_motionRotationAxis.z = (basis.sourceTangent.z + basis.targetTangent.z) * 0.5f;
        angle = dot > 0.0f ? 0 : FloatToShort(time * 32768.0f);
    }
    Matrix4x4_PushActiveTransform();
    Matrix4x4_ApplyAngleRotationToVector(g_motionRotationAxis, angle);
    g_motionRotationAxis = source;
    const Matrix4x4 &work = workTransformMatrix;
    g_motionBasisAxis.x = (work.rows[1].x * g_motionRotationAxis.y + work.rows[0].x * g_motionRotationAxis.x) + work.rows[2].x * g_motionRotationAxis.z;
    g_motionBasisAxis.y = (work.rows[1].y * g_motionRotationAxis.y + work.rows[0].y * g_motionRotationAxis.x) + work.rows[2].y * g_motionRotationAxis.z;
    g_motionBasisAxis.z = (work.rows[1].z * g_motionRotationAxis.y + work.rows[0].z * g_motionRotationAxis.x) + work.rows[2].z * g_motionRotationAxis.z;
    Matrix4x4::Pop(1);
    NormalizeVector3(&g_motionBasisAxis);
}

void Matrix4x3_OrthonormalBasisFromAxis(const MotionRotationBasis &basis, float time) {
    const Vector3 &axis = g_motionBasisAxis;
    g_motionBasisTangent.x = (1.0f - time) * basis.sourceTangent.x + time * basis.targetTangent.x;
    g_motionBasisTangent.y = (1.0f - time) * basis.sourceTangent.y + time * basis.targetTangent.y;
    const float tangentZ = (1.0f - time) * basis.sourceTangent.z + time * basis.targetTangent.z;
    const float binormalX = axis.y * tangentZ - axis.z * g_motionBasisTangent.y;
    const float binormalY = axis.z * g_motionBasisTangent.x - axis.x * tangentZ;
    g_motionBasisBinormal.x = binormalX;
    const float binormalZ = axis.x * g_motionBasisTangent.y - axis.y * g_motionBasisTangent.x;
    g_motionBasisBinormal.y = binormalY;
    g_motionBasisBinormal.z = binormalZ;
    g_motionBasisTangent.z = axis.y * binormalX - axis.x * binormalY;
    g_motionBasisTangent.x = axis.z * binormalY - axis.y * binormalZ;
    g_motionBasisTangent.y = axis.x * binormalZ - axis.z * binormalX;
    NormalizeVector3(&g_motionBasisTangent);
    NormalizeVector3(&g_motionBasisBinormal);
}

} // namespace

// Writes the slerped basis into the retained tmp_rotation_matrix, preserving its other
// cells, then multiplies it into the work frame.
// @verified - p
void AxisAngleToBasisSlerp(MotionRotationBasis &basis) {
    const float time = g_motionCurrentAnimTime / g_motionMaximumAnimTime;
    Matrix4x3_MatrixToAxisAngle(basis, g_motionCurrentAnimTime / g_motionMaximumAnimTime);
    Matrix4x3_OrthonormalBasisFromAxis(basis, time);
    Matrix4x4 &rotation = g_motionAxisAngleBasis;
    rotation.rows[0].x = g_motionBasisAxis.x;
    rotation.rows[0].z = g_motionBasisBinormal.x;
    rotation.rows[0].y = g_motionBasisTangent.x;
    rotation.rows[1].y = g_motionBasisTangent.y;
    rotation.rows[1].x = g_motionBasisAxis.y;
    rotation.rows[2].x = g_motionBasisAxis.z;
    rotation.rows[1].z = g_motionBasisBinormal.y;
    rotation.rows[2].z = g_motionBasisBinormal.z;
    rotation.rows[2].y = g_motionBasisTangent.z;
    Matrix4x4::MultiplyWithPrePassMatrix(&rotation);
}

// Restores the XYZ of three saved rows and zeroes their W; row 3 is left alone.
// @verified - p
void SwizzledBasis3x4IntoActiveFrom(const Vector4 (&rows)[3]) {
    Matrix4x4 &work = workTransformMatrix;
    for (std::size_t row = 0; row < 3; ++row) {
        work.rows[row].x = rows[row].x;
        work.rows[row].y = rows[row].y;
        work.rows[row].z = rows[row].z;
    }
    work.rows[0].w = 0.0f;
    work.rows[1].w = 0.0f;
    work.rows[2].w = 0.0f;
}

// ChannelsToPoseColumns: reads a joint's key blocks for a new motion and captures its
// first frame as the blend target. Joints 9-11 are the two-channel toes, 22 the forearm
// twist and 23 the wrist pair.
// @verified - p
void ChannelsToPoseColumns(MotionRigAxis *axes, MotionRotationBasis &basis, MotionCallback &callback, std::int32_t joint) {
    if (callback.evaluate) {
        if (joint >= 9 && joint <= 11) {
            ReadMotionKeyBlock(axes[0].channel);
            ReadMotionKeyBlock(axes[1].channel);
        } else if (joint == 22) {
            ReadMotionKeyBlock(axes[0].channel);
        } else {
            if (joint != 23)
                ReadMotionKeyBlock(axes[0].channel);
            ReadMotionKeyBlock(axes[1].channel);
            ReadMotionKeyBlock(axes[2].channel);
        }
        callback.evaluate(callback.parameters);
    } else if (g_motionPoseMode == motionPoseFromChannels) {
        if (joint >= 9 && joint <= 11) {
            ReadMotionKeyBlock(axes[0].channel);
            ReadMotionKeyBlock(axes[1].channel);
            Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[0].channel)));
            Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[1].channel)));
        } else if (joint == 22) {
            ReadMotionKeyBlock(axes[0].channel);
            Matrix4x4::RotateAroundX_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[0].channel)));
        } else if (joint == 23) {
            ReadMotionKeyBlock(axes[1].channel);
            ReadMotionKeyBlock(axes[2].channel);
            Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[1].channel)));
            Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[2].channel)));
        } else {
            for (std::size_t axis = 0; axis < 3; ++axis)
                ReadMotionKeyBlock(axes[axis].channel);
            ApplyChannelRotationZYX(axes);
        }
    }
    CaptureTargetColumns(basis);
}

// ChannelsToPoseColumns_0: evaluates a joint's current pose and captures it as the blend
// source. Joints 9-11 rotate Z/Y/X from the first two channels, 22 X only and 23 Z/Y
// from the second and third.
// @verified - p
void CaptureChannelsToSourcePose(MotionRigAxis *axes, MotionRotationBasis &basis, MotionCallback &callback, std::int32_t joint) {
    if (callback.evaluate) {
        callback.evaluate(callback.parameters);
    } else if (g_motionPoseMode == motionPoseFromChannels) {
        if (joint >= 9 && joint <= 11) {
            Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[0].channel)));
            Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[1].channel)));
            Matrix4x4::RotateAroundX_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[0].channel)));
        } else if (joint == 22) {
            Matrix4x4::RotateAroundX_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[0].channel)));
        } else if (joint == 23) {
            Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[1].channel)));
            Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(axes[2].channel)));
        } else {
            ApplyChannelRotationZYX(axes);
        }
    } else if (g_motionPoseMode == motionPoseFromAxisAngle) {
        AxisAngleToBasisSlerp(basis);
    }
    CaptureSourceColumns(basis);
}

// ChannelsToWorld_PVA: reads three position channels for a new motion and replaces them
// with one Hermite segment from the previous PVA to the new first frame, in this frame.
// @verified - p
void ChannelsToWorld_PVA(MotionRigAxis *axes, const Vector4 (&pva)[3], MotionCallback &callback) {
    Vector3 position{}, tangentIn{}, tangentOut{};
    Vector3 world{}, worldIn{}, worldOut{};
    const Matrix4x4 &work = workTransformMatrix;
    if (callback.evaluate) {
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(axes[axis].channel);
        EvaluateTargetCallback(callback, &position);
        world = TransformWorkPoint(position);
    } else if (g_motionPoseMode == motionPoseFromChannels) {
        for (std::size_t axis = 0; axis < 3; ++axis) {
            ReadMotionKeyBlock(axes[axis].channel);
            SampleMotionRigChannelPVA(axes[axis].channel, &position[axis], &tangentIn[axis], &tangentOut[axis]);
        }
        world = TransformWorkPoint(position);
        Matrix4x4::Transform(tangentOut, worldOut);
        Matrix4x4::Transform(tangentIn, worldIn);
    }
    const std::int8_t frames = g_motionGraphFrames;
    const float duration = ((frames < 0 ? 20.0f : static_cast<float>(frames)) - 1.0f) * motionFrameSeconds;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        MotionRigChannel &channel = axes[axis].channel;
        channel.current = {.timeSeconds = 0.0f, .tangentIn = pva[axis].y, .tangentOut = pva[axis].z, .value = pva[axis].w};
        channel.next = {.timeSeconds = duration, .tangentIn = worldIn[axis], .tangentOut = worldOut[axis], .value = world[axis]};
        channel.keysRemaining = 0;
        channel.frameCursor = nullptr;
        channel.inverseDuration = 1.0f / duration;
    }
}

// EvaluateObjectMotionAndTransform: the current target position, value and half tangents
// transformed into this frame, published as the PVA a following transition starts from.
// @verified - p
void EvaluateObjectMotionAndTransform(MotionRigAxis *axes, Vector4 (&pva)[3], MotionCallback &callback) {
    Vector3 position{}, tangentIn{}, tangentOut{};
    Vector3 world{}, worldIn{}, worldOut{};
    if (callback.evaluate) {
        EvaluateTargetCallback(callback, &position);
        world = TransformWorkPoint(position);
    } else if (g_motionPoseMode <= motionPoseFromAxisAngle) {
        for (std::size_t axis = 0; axis < 3; ++axis)
            SampleMotionRigChannelPVA(axes[axis].channel, &position[axis], &tangentIn[axis], &tangentOut[axis]);
        world = TransformWorkPoint(position);
        Matrix4x4::Transform(tangentIn, worldIn);
        Matrix4x4::Transform(tangentOut, worldOut);
    }
    for (std::size_t axis = 0; axis < 3; ++axis) {
        pva[axis].w = world[axis];
        pva[axis].y = worldIn[axis];
        pva[axis].z = worldOut[axis];
    }
}

namespace {

// nlError: the library formats its diagnostic into a stack buffer and discards it.
void MotionLibraryError(const char *file, std::int32_t line, const char *error) {
    char message[1024];
    std::snprintf(message, sizeof(message), "%s(%d): nlError(%s)\n", file, line, error);
}

// The split-window gait clocks shared by InitBaseRig and SolveBase.
void MotionGaitWindow(float time, float length, std::uint32_t flags, float &left, float &right) {
    const float inverse = 1.0f / g_motionGaitSplitFraction;
    left = inverse * time;
    if (!(length > left))
        left = length;
    const float start = (1.0f - g_motionGaitSplitFraction) * length;
    right = start > time ? 0.0f : (time - start) * inverse;
    right = right < length ? right : length;
    const float clamp = length * 0.41999999f;
    if (left - time > clamp)
        left = clamp + time;
    if (time - right > clamp)
        right = time - clamp;
    if (flags & motionInitSwapLeadFoot) {
        const float swap = left;
        left = right;
        right = swap;
    }
}

} // namespace

// @verified - p
void UpdateAnimRate(MotionRigChannel &channel, float scale, float bias) {
    channel.scale = scale;
    channel.bias = bias;
}

namespace {

// The cosine-rule joint angle: zero for degenerate lengths, else the clamped cosine and
// the root of 1 - cos^2 (negated for the lower joint, but never the degenerate zero).
void SolveJointAngle(float adjacent, float other, float oppositeSquared, bool negateSine, float &sine, float &cosine) {
    const float otherSquared = other * other;
    if (adjacent * adjacent < 0.00000011920929f || otherSquared < 0.00000011920929f) {
        sine = 0.0f;
        cosine = 0.0f;
        return;
    }
    cosine = ClampCosine(((otherSquared + adjacent * adjacent) - oppositeSquared) / ((adjacent * other) + (adjacent * other)));
    sine = std::sqrt(1.0f - cosine * cosine);
    if (negateSine)
        sine = -sine;
}

// Pulls a foot target toward the hip until it is within the leg's reach.
void ClampLegReach(Vector3 &target, const Matrix4x4 &hip, float reach) {
    const float x = target.x, y = target.y, z = target.z;
    const float distance =
        std::sqrt(((y - hip.rows[3].y) * (y - hip.rows[3].y) + (x - hip.rows[3].x) * (x - hip.rows[3].x)) + (z - hip.rows[3].z) * (z - hip.rows[3].z));
    if (!(distance > reach))
        return;
    target.x = x + ((hip.rows[3].x - x) * (distance - reach)) / distance;
    target.y = y + ((hip.rows[3].y - y) * (distance - reach)) / distance;
    target.z = z + ((hip.rows[3].z - z) * (distance - reach)) / distance;
}

// InitBaseRig: evaluates the current lower-body pose under the inverted root and stores
// each joint's source columns and target PVA for the transition into a new motion.
void InitBaseRig(MotionBaseRig &base) {
    g_motionMaximumAnimTime = base.clock.authored.maxTime * motionFrameSeconds;
    if (base.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = base.clock.current.maxTime * motionFrameSeconds;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionPoseMode = base.poseMode;
    Matrix4x4_Push();
    InvertModelWorkTransform();
    Matrix4x4::MultiplyWithPrePassMatrix(&base.rootPrepassTransform);
    Matrix4x4::MultiplyWithPrePassMatrix(&base.pelvisTransform);
    const float time = base.clock.current.ticksTime * motionFrameSeconds;
    float left, right;
    if (base.poseMode == motionPoseFromAxisAngle && (base.motionFlags & motionInitSplitWindow)) {
        MotionGaitWindow(time, g_motionMaximumAnimTime, base.motionFlags, left, right);
    } else {
        right = base.clock.current.ticksTime * motionFrameSeconds;
        left = right;
    }
    g_motionCurrentAnimTime = left;
    if (g_motionUseOverride) {
        Matrix4x4_PushActiveTransform();
        CaptureChannelsToSourcePose(base.leftHipNode->axes, base.leftHipNode->basis, base.leftHipNode->callback, 3);
        Matrix4x4::Pop(1);
    }
    Matrix4x4_PushActiveTransform();
    CaptureChannelsToSourcePose(base.leftThigh.axes, base.leftThigh.basis, base.leftThigh.callback, 4);
    Matrix4x4::Pop(1);
    EvaluateObjectMotionAndTransform(base.leftFoot.axes, base.leftFoot.pva, base.leftFoot.callback);
    Matrix4x4_Push();
    CaptureChannelsToSourcePose(base.leftAnkleRotation, base.leftAnkleBasis, base.leftAnkleCallback, 5);
    Matrix4x4::Pop(1);
    g_motionCurrentAnimTime = right;
    if (g_motionUseOverride) {
        Matrix4x4_PushActiveTransform();
        CaptureChannelsToSourcePose(base.rightHipNode->axes, base.rightHipNode->basis, base.rightHipNode->callback, 6);
        Matrix4x4::Pop(1);
    }
    Matrix4x4_PushActiveTransform();
    CaptureChannelsToSourcePose(base.rightThigh.axes, base.rightThigh.basis, base.rightThigh.callback, 7);
    Matrix4x4::Pop(1);
    EvaluateObjectMotionAndTransform(base.rightFoot.axes, base.rightFoot.pva, base.rightFoot.callback);
    Matrix4x4_Push();
    CaptureChannelsToSourcePose(base.rightAnkleRotation, base.rightAnkleBasis, base.rightAnkleCallback, 8);
    Matrix4x4::Pop(1);
    g_motionCurrentAnimTime = time;
    EvaluateObjectMotionAndTransform(base.pelvisPosition, base.pelvisPva, base.pelvisPositionCallback);
    Matrix4x4_Push();
    CaptureChannelsToSourcePose(base.pelvisRotation, base.pelvisBasis, base.pelvisRotationCallback, 0);
    Matrix4x4::Pop(1);
    Matrix4x4_PushActiveTransform();
    if (g_motionUseOverride) {
        CaptureChannelsToSourcePose(base.overrideHips->axes, base.overrideHips->basis, base.overrideHips->callback, 2);
        Matrix4x4::Pop(1);
        MotionRigTarget &target = *base.overridePelvisTarget;
        EvaluateObjectMotionAndTransform(target.axes, target.pva, target.callback);
        Matrix4x4_PushActiveTransform();
        CaptureChannelsToSourcePose(base.rightToe1Node->axes, base.rightToe1Node->basis, base.rightToe1Node->callback, 9);
        Matrix4x4::Pop(1);
        Matrix4x4_PushActiveTransform();
        CaptureChannelsToSourcePose(base.rightToe2Node->axes, base.rightToe2Node->basis, base.rightToe2Node->callback, 10);
        Matrix4x4::Pop(1);
        Matrix4x4_PushActiveTransform();
        CaptureChannelsToSourcePose(base.rightToe3Node->axes, base.rightToe3Node->basis, base.rightToe3Node->callback, 11);
        Matrix4x4::Pop(1);
    } else {
        CaptureChannelsToSourcePose(base.hips.axes, base.hips.basis, base.hips.callback, 1);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
}

} // namespace

// EvaluateBase: a positive override blend time scales the new motion's channels toward
// the rig's rest pose; returns 0 when that blend is already applied.
std::int32_t EvaluateBase(MotionBaseRig &base, std::uint8_t sourceRig, float scale, float blend) {
    if (std::fabs(blend - base.pelvisRotation[2].channel.scale) < 1.1754944e-38f)
        return 0;
    const float positionScale = scale * blend;
    const float height = g_motionRigDimensions[sourceRig][0];
    const float foot = g_motionRigDimensions[sourceRig][5];
    base.pelvisPosition[1].channel.scale = scale * blend;
    const float rest = (1.0f - blend) * scale;
    base.pelvisPosition[1].channel.bias = rest * height;
    UpdateAnimRate(base.pelvisPosition[2].channel, scale * blend, 0.0f);
    const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
    const float signedBlend = mirrored ? -blend : blend;
    base.pelvisRotation[0].channel.bias = 0.0f;
    base.pelvisRotation[0].channel.scale = signedBlend;
    base.pelvisRotation[1].channel.scale = signedBlend;
    base.pelvisRotation[1].channel.bias = 0.0f;
    UpdateAnimRate(base.pelvisRotation[2].channel, blend, 0.0f);
    base.hips.axes[0].channel.bias = 0.0f;
    base.hips.axes[0].channel.scale = signedBlend;
    base.hips.axes[1].channel.scale = signedBlend;
    base.hips.axes[1].channel.bias = mirrored ? (blend + 1.0f) * 1.5707964f : (1.0f - blend) * 1.5707964f;
    base.hips.axes[2].channel.scale = signedBlend;
    base.hips.axes[2].channel.bias = 0.0f;
    UpdateAnimRate(base.leftThigh.axes[0].channel, blend, 0.0f);
    base.leftThigh.axes[1].channel.bias = 0.0f;
    base.leftThigh.axes[1].channel.scale = signedBlend;
    UpdateAnimRate(base.leftThigh.axes[2].channel, signedBlend, 0.0f);
    base.leftFoot.axes[1].channel.scale = positionScale;
    const float footRest = rest * foot;
    base.leftFoot.axes[1].channel.bias = footRest;
    UpdateAnimRate(base.leftFoot.axes[2].channel, positionScale, 0.0f);
    base.leftAnkleRotation[0].channel.bias = 0.0f;
    base.leftAnkleRotation[0].channel.scale = signedBlend;
    UpdateAnimRate(base.leftAnkleRotation[1].channel, signedBlend, 0.0f);
    UpdateAnimRate(base.leftAnkleRotation[2].channel, blend, 0.0f);
    UpdateAnimRate(base.rightThigh.axes[0].channel, blend, 0.0f);
    base.rightThigh.axes[1].channel.bias = 0.0f;
    base.rightThigh.axes[1].channel.scale = signedBlend;
    UpdateAnimRate(base.rightThigh.axes[2].channel, signedBlend, 0.0f);
    base.rightFoot.axes[1].channel.scale = positionScale;
    base.rightFoot.axes[1].channel.bias = footRest;
    UpdateAnimRate(base.rightFoot.axes[2].channel, positionScale, 0.0f);
    base.rightAnkleRotation[0].channel.bias = 0.0f;
    base.rightAnkleRotation[0].channel.scale = signedBlend;
    UpdateAnimRate(base.rightAnkleRotation[1].channel, signedBlend, 0.0f);
    UpdateAnimRate(base.rightAnkleRotation[2].channel, blend, 0.0f);
    return 1;
}

namespace {

// InterpolateAnimationFrame: reads the pelvis channels of a new motion and replaces its
// position channels with one segment from the current pelvis PVA.
void InterpolateAnimationFrame(MotionBaseRig &base, float sourceTime) {
    const float savedTime = g_motionCurrentAnimTime;
    float anchor[3]{};
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(base.pelvisPosition[axis].channel);
    if (sourceTime >= 0.0f) {
        g_motionCurrentAnimTime = sourceTime * motionFrameSeconds;
        for (std::size_t axis = 0; axis < 3; ++axis)
            anchor[axis] = SampleMotionRigChannelAdvance(base.pelvisPosition[axis].channel);
    }
    g_motionCurrentAnimTime = savedTime;
    Vector3 position, tangentIn, tangentOut;
    for (std::size_t axis = 0; axis < 3; ++axis)
        SampleMotionRigChannelPVA(base.pelvisPosition[axis].channel, &position[axis], &tangentIn[axis], &tangentOut[axis]);
    g_motionPreviousAnimTime = g_motionCurrentAnimTime;
    float rotation[3];
    for (std::size_t axis = 0; axis < 3; ++axis) {
        ReadMotionKeyBlock(base.pelvisRotation[axis].channel);
        rotation[axis] = SampleMotionRigChannelAdvance(base.pelvisRotation[axis].channel);
    }
    Matrix4x4::preTranslate(-anchor[0], 0.0f, -anchor[2]);
    if (g_motionInitFlags & motionInitTranslation)
        Matrix4x4::preTranslate(anchor[0] - position.x, 0.0f, anchor[2] - position.z);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundZ_Wrapper(RigAngle(rotation[0]));
    Matrix4x4::RotateAroundY_Wrapper(RigAngle(rotation[1]));
    Matrix4x4::RotateAroundX_Wrapper(RigAngle(rotation[2]));
    CaptureTargetColumns(base.pelvisBasis);
    Matrix4x4::Pop(1);
    if (g_motionUseOverride)
        g_motionOverrideBakeTransform = workTransformMatrix;
    const Vector3 world = TransformWorkPoint(position);
    Vector3 worldIn, worldOut;
    Matrix4x4::Transform(tangentIn, worldIn);
    Matrix4x4::Transform(tangentOut, worldOut);
    const std::int8_t frames = g_motionGraphFrames;
    const float duration = ((frames < 0 ? 20.0f : static_cast<float>(frames)) - 1.0f) * motionFrameSeconds;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        MotionRigChannel &channel = base.pelvisPosition[axis].channel;
        const Vector4 &pva = base.pelvisPva[axis];
        channel.current = {.timeSeconds = 0.0f, .tangentIn = pva.y, .tangentOut = pva.z, .value = pva.w};
        channel.next = {.timeSeconds = duration, .tangentIn = worldIn[axis], .tangentOut = worldOut[axis], .value = world[axis]};
        channel.keysRemaining = 0;
        channel.frameCursor = nullptr;
        channel.inverseDuration = 1.0f / duration;
    }
}

void BakeRightLeg(MotionBaseRig &base) {
    if (g_motionUseOverride) {
        Matrix4x4_PushActiveTransform();
        ChannelsToPoseColumns(base.rightHipNode->axes, base.rightHipNode->basis, base.rightHipNode->callback, 6);
        Matrix4x4::Pop(1);
    }
    Matrix4x4_PushActiveTransform();
    ChannelsToPoseColumns(base.rightThigh.axes, base.rightThigh.basis, base.rightThigh.callback, 7);
    Matrix4x4::Pop(1);
    ChannelsToWorld_PVA(base.rightFoot.axes, base.rightFoot.pva, base.rightFoot.callback);
    Matrix4x4_Push();
    ChannelsToPoseColumns(base.rightAnkleRotation, base.rightAnkleBasis, base.rightAnkleCallback, 8);
    Matrix4x4::Pop(1);
}

// nl_motion_base_0: bakes a new motion's first pose into each lower-body joint's blend
// target and switches the rig to the axis-angle transition for g_motionGraphFrames.
void BakeBaseChannelsToPose(MotionBaseRig &base, float sourceTime, float time, float frameCount) {
    const float splitFrames = base.clock.authored.splitTimeFrames;
    g_motionInitFlags |= motionInitBaking;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionPoseMode = motionPoseFromChannels;
    g_motionPreviousAnimTime = splitFrames * motionFrameSeconds;
    g_motionMaximumAnimTime = (frameCount - 1.0f) * motionFrameSeconds;
    g_motionCurrentAnimTime = time * motionFrameSeconds;
    Matrix4x4_PushActiveTransform();
    InterpolateAnimationFrame(base, sourceTime);
    Matrix4x4_PushActiveTransform();
    if (g_motionUseOverride) {
        ChannelsToPoseColumns(base.overrideHips->axes, base.overrideHips->basis, base.overrideHips->callback, 2);
        Matrix4x4::Pop(1);
        MotionRigTarget &target = *base.overridePelvisTarget;
        ChannelsToWorld_PVA(target.axes, target.pva, target.callback);
    } else {
        ChannelsToPoseColumns(base.hips.axes, base.hips.basis, base.hips.callback, 1);
        Matrix4x4::Pop(1);
    }
    if (g_motionInitFlags & motionInitMirror)
        BakeRightLeg(base);
    if (g_motionUseOverride) {
        Matrix4x4_PushActiveTransform();
        ChannelsToPoseColumns(base.leftHipNode->axes, base.leftHipNode->basis, base.leftHipNode->callback, 3);
        Matrix4x4::Pop(1);
    }
    Matrix4x4_PushActiveTransform();
    ChannelsToPoseColumns(base.leftThigh.axes, base.leftThigh.basis, base.leftThigh.callback, 4);
    Matrix4x4::Pop(1);
    ChannelsToWorld_PVA(base.leftFoot.axes, base.leftFoot.pva, base.leftFoot.callback);
    Matrix4x4_Push();
    ChannelsToPoseColumns(base.leftAnkleRotation, base.leftAnkleBasis, base.leftAnkleCallback, 5);
    Matrix4x4::Pop(1);
    if (!(g_motionInitFlags & motionInitMirror))
        BakeRightLeg(base);
    if (g_motionUseOverride) {
        Matrix4x4_PushActiveTransform();
        ChannelsToPoseColumns(base.rightToe1Node->axes, base.rightToe1Node->basis, base.rightToe1Node->callback, 9);
        Matrix4x4::Pop(1);
        Matrix4x4_PushActiveTransform();
        ChannelsToPoseColumns(base.rightToe2Node->axes, base.rightToe2Node->basis, base.rightToe2Node->callback, 10);
        Matrix4x4::Pop(1);
        Matrix4x4_PushActiveTransform();
        ChannelsToPoseColumns(base.rightToe3Node->axes, base.rightToe3Node->basis, base.rightToe3Node->callback, 11);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
    std::int8_t frames = g_motionGraphFrames;
    if (frames < 0)
        frames = 20;
    base.poseMode = motionPoseFromAxisAngle;
    base.clock.current.splitTimeFrames = 0.0f;
    base.clock.current.ticksTime = 0.0f;
    g_motionGraphFrames = frames;
    base.clock.current.maxTime = static_cast<float>(frames - 1);
    g_motionInitFlags &= ~motionInitBaking;
}

} // namespace

// @verified - p
void ReadMotionBase(IKRig &rig, MotionBaseRig &base, const MotionStreamContext *context, float sourceTime, float time, std::uint32_t flags) {
    g_motionInitFlags = flags;
    if (flags & motionInitStream) {
        g_motionAnimFlags = context->flags;
        if (std::fabs(static_cast<double>(1.2f - context->version)) > 0.01)
            MotionLibraryError("motion\\src\\nlmotion_base.c", 87, "NL_ERROR_MOTION_VERSION");
        base.clock.authored.splitTimeFrames = 0.0f;
        const float last = static_cast<float>(static_cast<std::int16_t>(context->frameCount) - 1);
        base.clock.authored.ticksTime = last;
        base.clock.authored.maxTime = last;
    }
    if (!(g_motionInitFlags & motionInitStream) && (g_motionInitFlags & motionInitGraphFrames))
        MotionLibraryError("motion\\src\\nlmotion_base.c", 99, "NL_ERROR_MOTION_UNEXPECTED_INIT_FALG");
    if (!(g_motionInitFlags & motionInitChannels))
        MotionLibraryError("motion\\src\\nlmotion_base.c", 103, "NL_ERROR_MOTION_UNEXPECTED_INIT_FALG");
    std::uint32_t graphFrames = 0;
    const std::int32_t useOverride = g_motionUseOverride;
    if (g_motionInitFlags & motionInitStream) {
        graphFrames = g_motionInitFlags >> 8;
        g_motionGraphFrames = static_cast<std::int8_t>(g_motionInitFlags >> 8);
        if (static_cast<std::uint8_t>(g_motionInitFlags >> 8) || (g_motionInitFlags & motionInitCallbacks)) {
            InitBaseRig(base);
            graphFrames = (graphFrames & ~0xffU) | static_cast<std::uint8_t>(g_motionGraphFrames);
        }
        base.motionFlags = g_motionInitFlags;
        const std::uint8_t sourceRig = static_cast<std::uint8_t>(g_motionAnimFlags);
        const std::int32_t boneId = static_cast<std::int32_t>(rig.boneId);
        const float scale = boneId == sourceRig ? 1.0f : g_motionRigDimensions[boneId][0] / g_motionRigDimensions[sourceRig][0];
        const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
        const float sign = mirrored ? -1.0f : 1.0f;
        UpdateAnimRate(base.pelvisPosition[0].channel, mirrored ? -scale : scale, 0.0f);
        UpdateAnimRate(base.pelvisPosition[1].channel, scale, 0.0f);
        UpdateAnimRate(base.pelvisPosition[2].channel, scale, 0.0f);
        UpdateAnimRate(base.pelvisRotation[0].channel, sign, 0.0f);
        UpdateAnimRate(base.pelvisRotation[1].channel, sign, 0.0f);
        UpdateAnimRate(base.pelvisRotation[2].channel, 1.0f, 0.0f);
        if (useOverride) {
            MotionRigRotation &hips = *base.overrideHips;
            UpdateAnimRate(hips.axes[0].channel, sign, 0.0f);
            UpdateAnimRate(hips.axes[1].channel, sign, mirrored ? -3.1415927f : 0.0f);
            UpdateAnimRate(hips.axes[2].channel, sign, 0.0f);
            MotionRigTarget &target = *base.overridePelvisTarget;
            UpdateAnimRate(target.axes[0].channel, mirrored ? -scale : scale, 0.0f);
            UpdateAnimRate(target.axes[1].channel, scale, 0.0f);
            UpdateAnimRate(target.axes[2].channel, scale, 0.0f);
            UpdateAnimRate(base.leftHipNode->axes[0].channel, 1.0f, 0.0f);
            UpdateAnimRate(base.leftHipNode->axes[1].channel, sign, 0.0f);
            UpdateAnimRate(base.leftHipNode->axes[2].channel, sign, 0.0f);
        } else {
            UpdateAnimRate(base.hips.axes[0].channel, sign, 0.0f);
            UpdateAnimRate(base.hips.axes[1].channel, sign, mirrored ? 3.1415927f : 0.0f);
            UpdateAnimRate(base.hips.axes[2].channel, sign, 0.0f);
        }
        UpdateAnimRate(base.leftThigh.axes[0].channel, 1.0f, 0.0f);
        UpdateAnimRate(base.leftThigh.axes[1].channel, sign, 0.0f);
        UpdateAnimRate(base.leftThigh.axes[2].channel, sign, 0.0f);
        UpdateAnimRate(base.leftFoot.axes[0].channel, mirrored ? -scale : scale, 0.0f);
        UpdateAnimRate(base.leftFoot.axes[1].channel, scale, 0.0f);
        UpdateAnimRate(base.leftFoot.axes[2].channel, scale, 0.0f);
        UpdateAnimRate(base.leftAnkleRotation[0].channel, sign, 0.0f);
        UpdateAnimRate(base.leftAnkleRotation[1].channel, sign, 0.0f);
        UpdateAnimRate(base.leftAnkleRotation[2].channel, 1.0f, 0.0f);
        if (useOverride) {
            UpdateAnimRate(base.rightHipNode->axes[0].channel, 1.0f, 0.0f);
            UpdateAnimRate(base.rightHipNode->axes[1].channel, sign, 0.0f);
            UpdateAnimRate(base.rightHipNode->axes[2].channel, sign, 0.0f);
        }
        UpdateAnimRate(base.rightThigh.axes[0].channel, 1.0f, 0.0f);
        UpdateAnimRate(base.rightThigh.axes[1].channel, sign, 0.0f);
        UpdateAnimRate(base.rightThigh.axes[2].channel, sign, 0.0f);
        UpdateAnimRate(base.rightFoot.axes[0].channel, mirrored ? -scale : scale, 0.0f);
        UpdateAnimRate(base.rightFoot.axes[1].channel, scale, 0.0f);
        UpdateAnimRate(base.rightFoot.axes[2].channel, scale, 0.0f);
        UpdateAnimRate(base.rightAnkleRotation[0].channel, sign, 0.0f);
        UpdateAnimRate(base.rightAnkleRotation[1].channel, sign, 0.0f);
        UpdateAnimRate(base.rightAnkleRotation[2].channel, 1.0f, 0.0f);
        if (useOverride) {
            for (MotionRigTwoJoint *toe : {base.rightToe1Node, base.rightToe2Node, base.rightToe3Node}) {
                UpdateAnimRate(toe->axes[0].channel, 1.0f, 0.0f);
                UpdateAnimRate(toe->axes[1].channel, sign, 0.0f);
            }
        }
        if (g_motionOverrideBlendTime > 0.0f)
            EvaluateBase(base, sourceRig, scale, g_motionOverrideBlendTime);
        if (g_motionAnimFlags & motionWideKeyCounts)
            g_motionWideKeyCounts = static_cast<const std::uint16_t *>(context->regions[0].keyCounts);
        else
            g_motionKeyCounts = static_cast<const std::uint8_t *>(context->regions[0].keyCounts);
        g_motionKeyData = context->regions[0].keyData;
        base.clock.current.ticksTime = base.clock.authored.splitTimeFrames;
    } else {
        g_motionGraphFrames = 0;
    }
    base.rootPrepassTransform = workTransformMatrix;
    g_motionGaitSplitFraction = 0.5f;
    if (graphFrames) {
        BakeBaseChannelsToPose(base, sourceTime, time, static_cast<float>(static_cast<std::int16_t>(context->frameCount)));
        return;
    }
    const float splitFrames = base.clock.authored.splitTimeFrames;
    base.clock.current.splitTimeFrames = splitFrames;
    base.poseMode = motionPoseFromChannels;
    base.clock.current.maxTime = base.clock.authored.ticksTime;
    g_motionPreviousAnimTime = splitFrames * motionFrameSeconds;
    g_motionMaximumAnimTime = base.clock.authored.maxTime * motionFrameSeconds;
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(base.pelvisPosition[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(base.pelvisRotation[axis].channel);
    if (useOverride) {
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.overrideHips->axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.overridePelvisTarget->axes[axis].channel);
    } else {
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.hips.axes[axis].channel);
    }
    const auto readRightLeg = [&] {
        if (useOverride)
            for (std::size_t axis = 0; axis < 3; ++axis)
                ReadMotionKeyBlock(base.rightHipNode->axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.rightThigh.axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.rightFoot.axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.rightAnkleRotation[axis].channel);
    };
    if (g_motionInitFlags & motionInitMirror)
        readRightLeg();
    if (useOverride)
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(base.leftHipNode->axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(base.leftThigh.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(base.leftFoot.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(base.leftAnkleRotation[axis].channel);
    if (!(g_motionInitFlags & motionInitMirror))
        readRightLeg();
    if (useOverride) {

        ReadMotionKeyBlock(base.rightToe1Node->axes[0].channel);
        ReadMotionKeyBlock(base.rightToe1Node->axes[1].channel);
        ReadMotionKeyBlock(base.rightToe1Node->axes[0].channel);
        ReadMotionKeyBlock(base.rightToe1Node->axes[1].channel);
        ReadMotionKeyBlock(base.rightToe3Node->axes[0].channel);
        ReadMotionKeyBlock(base.rightToe3Node->axes[1].channel);
    }
    g_motionCurrentAnimTime = g_motionPreviousAnimTime;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    Matrix4x4_PushActiveTransform();
    if (g_motionInitFlags & motionInitChannels) {
        const float z = -SampleMotionRigChannelAdvance(base.pelvisPosition[2].channel);
        const float x = SampleMotionRigChannelAdvance(base.pelvisPosition[0].channel);
        Matrix4x4::preTranslate(-x, 0.0f, z);
    }
    base.pelvisTransform = workTransformMatrix;
    Matrix4x4::Pop(1);
}

// @verified - p
void TranslatePelvisFromChannels(MotionBaseRig &base, std::uint32_t interpolationMode) {
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = base.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = base.clock.authored.maxTime * motionFrameSeconds;
    if (base.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = base.clock.current.maxTime * motionFrameSeconds;
    Matrix4x4_PushActiveTransform();
    Vector3 offset;
    if (base.pelvisPositionCallback.evaluate) {
        EvaluateTargetCallback(base.pelvisPositionCallback, &offset);
    } else {
        offset.x = SampleMotionRigChannelAdvance(base.pelvisPosition[0].channel);
        offset.z = SampleMotionRigChannelAdvance(base.pelvisPosition[2].channel);
    }
    Matrix4x4::preTranslate(-offset.x, 0.0f, -offset.z);
    base.pelvisTransform = workTransformMatrix;
    Matrix4x4::Pop(1);
}

// @verified - p
void BuildChannelTransformAtTime(MotionBaseRig &base, Matrix4x4 *output, float time, std::uint32_t interpolationMode) {
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = time * motionFrameSeconds;
    g_motionMaximumAnimTime = base.clock.authored.maxTime * motionFrameSeconds;
    if (base.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = base.clock.current.maxTime * motionFrameSeconds;
    Matrix4x4_PushActiveTransform();
    const float z = SampleMotionRigChannelAdvance(base.pelvisPosition[2].channel);
    const float y = SampleMotionRigChannelAdvance(base.pelvisPosition[1].channel);
    const float x = SampleMotionRigChannelAdvance(base.pelvisPosition[0].channel);
    Matrix4x4::preTranslate(x, y, z);
    ApplyChannelRotationZYX(base.pelvisRotation);
    Matrix4x4::GetTransformMatrix(output);
    Matrix4x4::Pop(1);
}

// @verified - p
void SolveBase(MotionBaseRig &base, std::uint32_t interpolationMode) {
    const float reach = g_motionRigDimensions[g_motionRigBoneId][4] - 0.001f;
    g_motionInterpolationType = interpolationMode;
    float length = base.clock.authored.maxTime * motionFrameSeconds;
    g_motionMaximumAnimTime = length;
    if (base.poseMode == motionPoseFromAxisAngle) {
        length = base.clock.current.maxTime * motionFrameSeconds;
        g_motionMaximumAnimTime = length;
    }
    const float time = base.clock.current.ticksTime * motionFrameSeconds;
    float left, right;
    const bool splitWindow = base.poseMode == motionPoseFromAxisAngle && (base.motionFlags & motionInitSplitWindow);
    if (splitWindow) {
        MotionGaitWindow(time, length, base.motionFlags, left, right);
    } else {
        right = base.clock.current.ticksTime * motionFrameSeconds;
        left = right;
    }
    const Vector4 savedRows[3]{workTransformMatrix.rows[0], workTransformMatrix.rows[1], workTransformMatrix.rows[2]};
    g_motionCurrentAnimTime = left;
    Vector3 leftTarget, rightTarget, pelvisTarget{};
    if (base.leftFoot.callback.evaluate)
        EvaluateTargetCallback(base.leftFoot.callback, &leftTarget);
    else
        SampleTargetChannels(base.leftFoot, leftTarget);
    leftTarget = TransformWorkPoint(leftTarget);
    g_motionCurrentAnimTime = right;
    if (base.rightFoot.callback.evaluate)
        EvaluateTargetCallback(base.rightFoot.callback, &rightTarget);
    else
        SampleTargetChannels(base.rightFoot, rightTarget);
    rightTarget = TransformWorkPoint(rightTarget);
    g_motionCurrentAnimTime = time;
    if (g_motionUseOverride) {
        MotionRigTarget &target = *base.overridePelvisTarget;
        if (target.callback.evaluate)
            EvaluateTargetCallback(target.callback, &pelvisTarget);
        else
            SampleTargetChannels(target, pelvisTarget);
        pelvisTarget = TransformWorkPoint(pelvisTarget);
    }
    Vector3 offset;
    if (base.pelvisPositionCallback.evaluate) {
        EvaluateTargetCallback(base.pelvisPositionCallback, &offset);
    } else {
        offset.x = SampleMotionRigChannelAdvance(base.pelvisPosition[0].channel);
        offset.y = SampleMotionRigChannelAdvance(base.pelvisPosition[1].channel);
        offset.z = SampleMotionRigChannelAdvance(base.pelvisPosition[2].channel);
    }
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(-offset.x, 0.0f, -offset.z);
    base.pelvisTransform = workTransformMatrix;
    Matrix4x4::Pop(1);
    EvaluateJointRotation(base.pelvisRotation, base.pelvisBasis, base.pelvisRotationCallback, base.poseMode);
    base.pelvisWorld = workTransformMatrix;
    Matrix4x4_Push();
    if (g_motionUseOverride) {
        MotionRigRotation &hips = *base.overrideHips;
        Matrix4x4::preTranslate(0.0f, 0.0f, hips.axes[2].dimensions[0]);
        EvaluateJointRotation(hips.axes, hips.basis, hips.callback, base.poseMode);
        StoreTargetOutput(*base.overridePelvisTarget, pelvisTarget);
        AimBasisAtTarget(pelvisTarget);
        base.hipsWorld = workTransformMatrix;
        Matrix4x4::preTranslate(base.overridePelvisTarget->axes[0].dimensions[0], 0.0f, 0.0f);
    } else {
        Matrix4x4::preTranslate(0.0f, base.hips.axes[1].dimensions[0], 0.0f);
        EvaluateJointRotation(base.hips.axes, base.hips.basis, base.hips.callback, base.poseMode);
        base.hipsWorld = workTransformMatrix;
    }
    g_motionCurrentAnimTime = left;
    Matrix4x4_Push();
    if (g_motionUseOverride) {
        MotionRigRotation &hip = *base.leftHipNode;
        Matrix4x4::preTranslate(hip.axes[0].dimensions[0], hip.axes[1].dimensions[0], hip.axes[2].dimensions[0]);
        EvaluateJointRotation(hip.axes, hip.basis, hip.callback, base.poseMode);
        Matrix4x4::GetTransformMatrix(base.leftHipWorld);
        Matrix4x4::preTranslate(base.leftThigh.axes[0].dimensions[0], base.leftThigh.axes[1].dimensions[0], 0.0f);
    } else {
        Matrix4x4::preTranslate(0.0f, base.leftThigh.axes[1].dimensions[0], base.leftThigh.axes[2].dimensions[0]);
    }
    EvaluateJointRotation(base.leftThigh.axes, base.leftThigh.basis, base.leftThigh.callback, base.poseMode);
    if (splitWindow) {
        base.leftThighWorld = workTransformMatrix;
        ClampLegReach(leftTarget, base.leftThighWorld, reach);
    }
    StoreTargetOutput(base.leftFoot, leftTarget);
    const float leftDistance = AimBasisAtTarget(leftTarget);
    const float leftDistanceSquared = leftDistance * leftDistance;
    float sine, cosine;
    SolveJointAngle(
        base.leftFoot.axes[1].dimensions[0], leftDistance, base.leftFoot.axes[0].dimensions[0] * base.leftFoot.axes[0].dimensions[0], false, sine, cosine);
    Matrix4x4::RotateAroundZ(sine, cosine);
    base.leftThighWorld = workTransformMatrix;
    Matrix4x4::preTranslate(base.leftFoot.axes[1].dimensions[0], 0.0f, 0.0f);
    SolveJointAngle(base.leftFoot.axes[1].dimensions[0], base.leftFoot.axes[0].dimensions[0], leftDistanceSquared, true, sine, cosine);
    Matrix4x4::RotateAroundZ(sine, -cosine);
    base.leftShinWorld = workTransformMatrix;
    Matrix4x4::preTranslate(base.leftFoot.axes[0].dimensions[0], 0.0f, 0.0f);
    SwizzledBasis3x4IntoActiveFrom(savedRows);
    EvaluateJointRotation(base.leftAnkleRotation, base.leftAnkleBasis, base.leftAnkleCallback, base.poseMode);
    base.leftAnkleWorld = workTransformMatrix;
    if (!g_motionUseOverride) {
        Matrix4x4::preTranslate(0.0f, base.leftFootRotation[1].dimensions[0], base.leftFootRotation[2].dimensions[0]);
        base.leftFootWorld = workTransformMatrix;
    }
    Matrix4x4::Pop(1);
    g_motionCurrentAnimTime = right;
    if (g_motionUseOverride) {
        Matrix4x4_Push();
        MotionRigRotation &hip = *base.rightHipNode;
        Matrix4x4::preTranslate(hip.axes[0].dimensions[0], hip.axes[1].dimensions[0], hip.axes[2].dimensions[0]);
        EvaluateJointRotation(hip.axes, hip.basis, hip.callback, base.poseMode);
        Matrix4x4::GetTransformMatrix(base.rightHipWorld);
        Matrix4x4::preTranslate(base.rightThigh.axes[0].dimensions[0], base.rightThigh.axes[1].dimensions[0], 0.0f);
    } else {
        Matrix4x4::preTranslate(0.0f, base.rightThigh.axes[1].dimensions[0], base.rightThigh.axes[2].dimensions[0]);
    }
    EvaluateJointRotation(base.rightThigh.axes, base.rightThigh.basis, base.rightThigh.callback, base.poseMode);
    if (splitWindow) {
        base.rightThighWorld = workTransformMatrix;
        ClampLegReach(rightTarget, base.rightThighWorld, reach);
    }
    StoreTargetOutput(base.rightFoot, rightTarget);
    const float rightDistance = AimBasisAtTarget(rightTarget);
    const float rightDistanceSquared = rightDistance * rightDistance;
    SolveJointAngle(
        base.rightFoot.axes[1].dimensions[0], rightDistance, base.rightFoot.axes[0].dimensions[0] * base.rightFoot.axes[0].dimensions[0], false, sine, cosine);
    Matrix4x4::RotateAroundZ(sine, cosine);
    base.rightThighWorld = workTransformMatrix;
    Matrix4x4::preTranslate(base.rightFoot.axes[1].dimensions[0], 0.0f, 0.0f);
    SolveJointAngle(base.rightFoot.axes[1].dimensions[0], base.rightFoot.axes[0].dimensions[0], rightDistanceSquared, true, sine, cosine);
    Matrix4x4::RotateAroundZ(sine, -cosine);
    base.rightShinWorld = workTransformMatrix;
    Matrix4x4::preTranslate(base.rightFoot.axes[0].dimensions[0], 0.0f, 0.0f);
    SwizzledBasis3x4IntoActiveFrom(savedRows);
    EvaluateJointRotation(base.rightAnkleRotation, base.rightAnkleBasis, base.rightAnkleCallback, base.poseMode);
    base.rightAnkleWorld = workTransformMatrix;
    Matrix4x4 *last;
    if (!g_motionUseOverride) {
        Matrix4x4::preTranslate(0.0f, base.rightFootRotation[1].dimensions[0], base.rightFootRotation[2].dimensions[0]);
        last = &base.rightFootWorld;
    } else {
        Matrix4x4::Pop(1);
        Matrix4x4::preTranslate(base.rightToe1Node->axes[0].dimensions[0], base.rightToe1Node->axes[1].dimensions[0], 0.0f);
        EvaluateTwoJointRotation(*base.rightToe1Node, base.poseMode);
        Matrix4x4::GetTransformMatrix(base.rightToe1World);
        Matrix4x4::preTranslate(base.rightToe2Node->axes[0].dimensions[0], base.rightToe2Node->axes[1].dimensions[0], 0.0f);
        EvaluateTwoJointRotation(*base.rightToe2Node, base.poseMode);
        Matrix4x4::GetTransformMatrix(base.rightToe2World);
        Matrix4x4::preTranslate(base.rightToe3Node->axes[0].dimensions[0], base.rightToe3Node->axes[1].dimensions[0], 0.0f);
        EvaluateTwoJointRotation(*base.rightToe3Node, base.poseMode);
        last = base.rightToe3World;
    }
    Matrix4x4::GetTransformMatrix(last);
    Matrix4x4::Pop(1);
}

// EvaluateChestBlend: the chest counterpart of EvaluateBase.
std::int32_t EvaluateChestBlend(MotionChestRig &chest, std::uint8_t sourceRig, float scale, float blend) {
    if (std::fabs(blend - chest.upperRotation.axes[0].channel.scale) < 1.1754944e-38f)
        return 0;
    const float height = g_motionRigDimensions[sourceRig][1];
    const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
    MotionRigAxis *const rotation = chest.rotation.axes;
    rotation[0].channel.bias = 0.0f;
    rotation[0].channel.scale = mirrored ? -blend : blend;
    rotation[1].channel.scale = mirrored ? -blend : blend;
    rotation[1].channel.bias = mirrored ? (blend + 1.0f) * 1.5707964f : (1.0f - blend) * 1.5707964f;
    rotation[2].channel.scale = mirrored ? -blend : blend;
    rotation[2].channel.bias = 0.0f;
    MotionRigAxis *const target = chest.target.axes;
    target[0].channel.bias = 0.0f;
    const float positionScale = scale * blend;
    target[0].channel.scale = scale * blend;
    target[1].channel.scale = scale * blend;
    target[1].channel.bias = ((1.0f - blend) * scale) * height;
    target[2].channel.bias = 0.0f;
    target[2].channel.scale = mirrored ? -positionScale : positionScale;
    MotionRigAxis *const upper = chest.upperRotation.axes;
    upper[0].channel.scale = blend;
    upper[0].channel.bias = 0.0f;
    upper[1].channel.bias = 0.0f;
    upper[1].channel.scale = mirrored ? -blend : blend;
    upper[2].channel.bias = 0.0f;
    upper[2].channel.scale = mirrored ? -blend : blend;
    return 1;
}

namespace {

// EvaluateChest: captures the chest's current pose as the blend source.
void EvaluateChest(MotionChestRig &chest) {
    g_motionCurrentAnimTime = chest.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = chest.clock.authored.maxTime * motionFrameSeconds;
    if (chest.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = chest.clock.current.maxTime * motionFrameSeconds;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionPoseMode = chest.poseMode;
    if (g_motionUseOverride) {
        Matrix4x4_Push();
        workTransformMatrix = g_motionOverrideRootTransform;
        Matrix4x4_PushActiveTransform();
    } else {
        Matrix4x4_PushActiveTransform();
        Matrix4x4_Push();
    }
    CaptureChannelsToSourcePose(chest.rotation.axes, chest.rotation.basis, chest.rotation.callback, 0);
    Matrix4x4::Pop(1);
    EvaluateObjectMotionAndTransform(chest.target.axes, chest.target.pva, chest.target.callback);
    if (!g_motionUseOverride) {
        Matrix4x4_Push();
        CaptureChannelsToSourcePose(chest.upperRotation.axes, chest.upperRotation.basis, chest.upperRotation.callback, 0);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
}

void FinishBake(std::uint32_t &poseMode, MotionRigClock &clock) {
    std::int8_t frames = g_motionGraphFrames;
    if (frames < 0)
        frames = 20;
    poseMode = motionPoseFromAxisAngle;
    clock.current.splitTimeFrames = 0.0f;
    clock.current.ticksTime = 0.0f;
    g_motionGraphFrames = frames;
    clock.current.maxTime = static_cast<float>(frames - 1);
    g_motionInitFlags &= ~motionInitBaking;
}

void BeginBake(float time, float frameCount) {
    g_motionInitFlags |= motionInitBaking;
    g_motionCurrentAnimTime = time * motionFrameSeconds;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionPreviousAnimTime = time * motionFrameSeconds;
    g_motionMaximumAnimTime = (frameCount - 1.0f) * motionFrameSeconds;
    g_motionPoseMode = motionPoseFromChannels;
    if (g_motionUseOverride) {
        Matrix4x4_Push();
        workTransformMatrix = g_motionOverrideBakeTransform;
        Matrix4x4_PushActiveTransform();
    } else {
        Matrix4x4_PushActiveTransform();
        Matrix4x4_Push();
    }
}

// BakeChestChannelsToPose.
void BakeChestChannelsToPose(MotionChestRig &chest, float time, float frameCount) {
    BeginBake(time, frameCount);
    ChannelsToPoseColumns(chest.rotation.axes, chest.rotation.basis, chest.rotation.callback, 0);
    Matrix4x4::Pop(1);
    ChannelsToWorld_PVA(chest.target.axes, chest.target.pva, chest.target.callback);
    if (!g_motionUseOverride) {
        Matrix4x4_Push();
        ChannelsToPoseColumns(chest.upperRotation.axes, chest.upperRotation.basis, chest.upperRotation.callback, 0);
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
    FinishBake(chest.poseMode, chest.clock);
}

} // namespace

// EvaluateHead: the head counterpart of EvaluateBase.
std::int32_t EvaluateHead(MotionHeadRig &head, std::uint8_t sourceRig, float scale, float blend) {
    if (std::fabs(blend - head.baseRotation.axes[0].channel.scale) < 1.1754944e-38f)
        return 0;
    const float height = g_motionRigDimensions[sourceRig][2];
    const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
    MotionRigAxis *const base = head.baseRotation.axes;
    base[0].channel.bias = 0.0f;
    base[0].channel.scale = blend;
    base[1].channel.bias = 0.0f;
    base[1].channel.scale = mirrored ? -blend : blend;
    base[2].channel.scale = mirrored ? -blend : blend;
    base[2].channel.bias = 0.0f;
    MotionRigAxis *const mid = head.midRotation.axes;
    mid[0].channel.scale = blend;
    mid[0].channel.bias = 0.0f;
    float rest;
    if (mirrored) {
        rest = blend + 1.0f;
        mid[1].channel.scale = -blend;
        mid[1].channel.bias = (blend + 1.0f) * -1.5707964f;
        mid[2].channel.scale = -blend;
    } else {
        mid[1].channel.scale = blend;
        rest = 1.0f - blend;
        mid[1].channel.bias = (1.0f - blend) * -1.5707964f;
        mid[2].channel.scale = blend;
    }
    mid[2].channel.bias = rest * 3.1415927f;
    MotionRigAxis *const target = head.target.axes;
    const float positionScale = scale * blend;
    target[0].channel.bias = 0.0f;
    target[0].channel.scale = mirrored ? -positionScale : positionScale;
    target[1].channel.scale = positionScale;
    target[1].channel.bias = ((1.0f - blend) * scale) * height;
    target[2].channel.bias = 0.0f;
    target[2].channel.scale = positionScale;
    return 1;
}

namespace {

// SolveHead: captures the head's current pose as the blend source.
void SolveHead(MotionHeadRig &head) {
    g_motionCurrentAnimTime = head.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = head.clock.authored.maxTime * motionFrameSeconds;
    if (head.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = head.clock.current.maxTime * motionFrameSeconds;
    std::int32_t useOverride = g_motionUseOverride;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionPoseMode = head.poseMode;
    if (g_motionUseOverride) {
        Matrix4x4_Push();
        workTransformMatrix = g_motionOverrideRootTransform;
        Matrix4x4_PushActiveTransform();
    } else {
        Matrix4x4_PushActiveTransform();
        Matrix4x4_Push();
    }
    if (head.baseRotation.callback.evaluate)
        useOverride = g_motionUseOverride;
    CaptureChannelsToSourcePose(head.baseRotation.axes, head.baseRotation.basis, head.baseRotation.callback, 0);
    Matrix4x4::Pop(1);
    if (useOverride) {
        Matrix4x4_PushActiveTransform();
        MotionRigRotation &source = *head.aimSourceNode;
        CaptureChannelsToSourcePose(source.axes, source.basis, source.callback, 0);
        Matrix4x4::Pop(1);
        MotionRigTarget &target = *head.aimTargetNode;
        EvaluateObjectMotionAndTransform(target.axes, target.pva, target.callback);
        if (g_motionUseOverride)
            workTransformMatrix = transformMatrix_prePass;
    }
    Matrix4x4_Push();
    CaptureChannelsToSourcePose(head.midRotation.axes, head.midRotation.basis, head.midRotation.callback, 0);
    Matrix4x4::Pop(1);
    EvaluateObjectMotionAndTransform(head.target.axes, head.target.pva, head.target.callback);
    if (g_motionUseOverride) {
        for (MotionRigRotation *node : {head.nodeA, head.nodeB, head.nodeC}) {
            Matrix4x4_Push();
            CaptureChannelsToSourcePose(node->axes, node->basis, node->callback, 0);
            Matrix4x4::Pop(1);
        }
    }
    Matrix4x4::Pop(1);
}


void BakeHeadChannelsToPose(MotionHeadRig &head, float time, float frameCount) {
    std::int32_t useOverride = g_motionUseOverride;
    BeginBake(time, frameCount);
    if (head.baseRotation.callback.evaluate)
        useOverride = g_motionUseOverride;
    ChannelsToPoseColumns(head.baseRotation.axes, head.baseRotation.basis, head.baseRotation.callback, 0);
    Matrix4x4::Pop(1);
    if (useOverride) {
        Matrix4x4_PushActiveTransform();
        MotionRigRotation &source = *head.aimSourceNode;
        ChannelsToPoseColumns(source.axes, source.basis, source.callback, 0);
        Matrix4x4::Pop(1);
        MotionRigTarget &target = *head.aimTargetNode;
        ChannelsToWorld_PVA(target.axes, target.pva, target.callback);
        if (g_motionUseOverride)
            workTransformMatrix = transformMatrix_prePass;
    }
    Matrix4x4_Push();
    ChannelsToPoseColumns(head.midRotation.axes, head.midRotation.basis, head.midRotation.callback, 0);
    Matrix4x4::Pop(1);
    ChannelsToWorld_PVA(head.target.axes, head.target.pva, head.target.callback);
    if (g_motionUseOverride) {
        for (MotionRigRotation *node : {head.nodeA, head.nodeB, head.nodeC}) {
            Matrix4x4_Push();
            ChannelsToPoseColumns(node->axes, node->basis, node->callback, 0);
            Matrix4x4::Pop(1);
        }
    }
    Matrix4x4::Pop(1);
    FinishBake(head.poseMode, head.clock);
}

} // namespace

// EvaluateArm: the arm counterpart of EvaluateBase.
std::int32_t EvaluateArm(MotionArmRig &arm, std::uint8_t sourceRig, float scale, float blend) {
    if (std::fabs(blend - arm.shoulder.axes[0].channel.scale) < 1.1754944e-38f)
        return 0;
    const float length = -g_motionRigDimensions[sourceRig][3];
    const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
    MotionRigAxis *const shoulder = arm.shoulder.axes;
    shoulder[0].channel.bias = 0.0f;
    shoulder[0].channel.scale = blend;
    shoulder[1].channel.bias = 0.0f;
    shoulder[1].channel.scale = mirrored ? -blend : blend;
    shoulder[2].channel.scale = mirrored ? -blend : blend;
    shoulder[2].channel.bias = 0.0f;
    MotionRigAxis *const elbow = arm.elbow.axes;
    elbow[0].channel.scale = blend;
    elbow[0].channel.bias = (1.0f - blend) * -1.5707964f;
    elbow[1].channel.bias = 0.0f;
    elbow[1].channel.scale = mirrored ? -blend : blend;
    elbow[2].channel.scale = mirrored ? -blend : blend;
    elbow[2].channel.bias = 0.0f;
    MotionRigAxis *const target = arm.target.axes;
    target[0].channel.bias = 0.0f;
    const float positionScale = scale * blend;
    target[0].channel.scale = scale * blend;
    target[1].channel.scale = scale * blend;
    target[1].channel.bias = ((1.0f - blend) * scale) * length;
    target[2].channel.bias = 0.0f;
    target[2].channel.scale = mirrored ? -positionScale : positionScale;
    arm.wristRotation[0].channel.bias = 0.0f;
    arm.wristRotation[0].channel.scale = mirrored ? -blend : blend;
    arm.wristRotation[1].channel.scale = blend;
    arm.wristRotation[1].channel.bias = 0.0f;
    arm.wristRotation[2].channel.bias = 0.0f;
    arm.wristRotation[2].channel.scale = mirrored ? -blend : blend;
    return 1;
}

namespace {

// Arm_ChannelsToBasis_Prepass: captures the arm's current pose as the blend source.
void Arm_ChannelsToBasis_Prepass(MotionArmRig &arm) {
    g_motionCurrentAnimTime = arm.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = arm.clock.authored.maxTime * motionFrameSeconds;
    if (arm.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = arm.clock.current.maxTime * motionFrameSeconds;
    std::int32_t useOverride = g_motionUseOverride;
    g_motionInterpolationType = motionInterpolationHermiteAuto;
    g_motionPoseMode = arm.poseMode;
    if (g_motionUseOverride) {
        Matrix4x4_Push();
        workTransformMatrix = g_motionOverrideRootTransform;
        Matrix4x4_PushActiveTransform();
    } else {
        Matrix4x4_PushActiveTransform();
        Matrix4x4_Push();
    }
    if (arm.shoulder.callback.evaluate)
        useOverride = g_motionUseOverride;
    CaptureChannelsToSourcePose(arm.shoulder.axes, arm.shoulder.basis, arm.shoulder.callback, 0);
    Matrix4x4::Pop(1);
    if (useOverride)
        Matrix4x4_PushActiveTransform();
    else
        Matrix4x4_Push();
    CaptureChannelsToSourcePose(arm.elbow.axes, arm.elbow.basis, arm.elbow.callback, 0);
    Matrix4x4::Pop(1);
    EvaluateObjectMotionAndTransform(arm.target.axes, arm.target.pva, arm.target.callback);
    Matrix4x4_Push();
    if (g_motionUseOverride) {
        CaptureChannelsToSourcePose(arm.wristRotation, arm.wristBasis, arm.wristCallback, 0);
    } else {
        CaptureChannelsToSourcePose(arm.wristRotation, arm.forearmBasis, arm.forearmCallback, 22);
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        CaptureChannelsToSourcePose(arm.wristRotation, arm.wristBasis, arm.wristCallback, 23);
    }
    Matrix4x4::Pop(2);
}

// Arm_ChannelsToTransforms: bakes the arm's first pose of a new motion.
void Arm_ChannelsToTransforms(MotionArmRig &arm, float time, float frameCount) {
    std::int32_t useOverride = g_motionUseOverride;
    BeginBake(time, frameCount);
    if (arm.shoulder.callback.evaluate)
        useOverride = g_motionUseOverride;
    ChannelsToPoseColumns(arm.shoulder.axes, arm.shoulder.basis, arm.shoulder.callback, 0);
    Matrix4x4::Pop(1);
    if (useOverride)
        Matrix4x4_PushActiveTransform();
    else
        Matrix4x4_Push();
    ChannelsToPoseColumns(arm.elbow.axes, arm.elbow.basis, arm.elbow.callback, 0);
    Matrix4x4::Pop(1);
    ChannelsToWorld_PVA(arm.target.axes, arm.target.pva, arm.target.callback);
    Matrix4x4_Push();
    if (g_motionUseOverride) {
        ChannelsToPoseColumns(arm.wristRotation, arm.wristBasis, arm.wristCallback, 0);
    } else {
        ChannelsToPoseColumns(arm.wristRotation, arm.forearmBasis, arm.forearmCallback, 22);
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        ChannelsToPoseColumns(arm.wristRotation, arm.wristBasis, arm.wristCallback, 23);
    }
    Matrix4x4::Pop(2);
    FinishBake(arm.poseMode, arm.clock);
}

void SetRegionStreams(const MotionStreamContext *context, std::size_t region) {
    if (g_motionAnimFlags & motionWideKeyCounts)
        g_motionWideKeyCounts = static_cast<const std::uint16_t *>(context->regions[region].keyCounts);
    else
        g_motionKeyCounts = static_cast<const std::uint8_t *>(context->regions[region].keyCounts);
    g_motionKeyData = context->regions[region].keyData;
}

float RegionScale(const IKRig &rig, std::size_t column) {
    const std::uint8_t sourceRig = static_cast<std::uint8_t>(g_motionAnimFlags);
    const std::int32_t boneId = static_cast<std::int32_t>(rig.boneId);
    if (boneId == sourceRig)
        return 1.0f;
    const std::size_t index = g_motionUseOverride ? 0 : column;
    return g_motionRigDimensions[boneId][index] / g_motionRigDimensions[sourceRig][index];
}

} // namespace

// @verified - p
void ReadMotionChest(IKRig &rig, MotionChestRig &chest, const MotionStreamContext *context, float time, std::uint32_t flags) {
    g_motionInitFlags = flags;
    if (flags & motionInitStream) {
        g_motionAnimFlags = context->flags;
        if (std::fabs(static_cast<double>(1.2f - context->version)) > 0.01)
            MotionLibraryError("motion\\src\\nlmotion_chest.c", 84, "NL_ERROR_MOTION_VERSION");
        chest.clock.authored.splitTimeFrames = 0.0f;
        const float last = static_cast<float>(static_cast<std::int16_t>(context->frameCount) - 1);
        chest.clock.authored.ticksTime = last;
        chest.clock.authored.maxTime = last;
    }
    std::int32_t useOverride;
    if (g_motionInitFlags & motionInitStream) {
        const std::uint32_t graphFrames = g_motionInitFlags >> 8;
        g_motionGraphFrames = static_cast<std::int8_t>(g_motionInitFlags >> 8);
        if (static_cast<std::uint8_t>(g_motionInitFlags >> 8) || (g_motionInitFlags & motionInitCallbacks))
            EvaluateChest(chest);
        useOverride = g_motionUseOverride;
        const float scale = RegionScale(rig, 1);
        const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
        const float sign = mirrored ? -1.0f : 1.0f;
        UpdateAnimRate(chest.rotation.axes[0].channel, sign, 0.0f);
        UpdateAnimRate(chest.rotation.axes[1].channel, sign, mirrored ? 3.1415927f : 0.0f);
        UpdateAnimRate(chest.rotation.axes[2].channel, sign, 0.0f);
        if (useOverride) {
            UpdateAnimRate(chest.target.axes[0].channel, mirrored ? -scale : scale, 0.0f);
            UpdateAnimRate(chest.target.axes[1].channel, scale, 0.0f);
            UpdateAnimRate(chest.target.axes[2].channel, scale, 0.0f);
        } else {
            UpdateAnimRate(chest.target.axes[0].channel, scale, 0.0f);
            UpdateAnimRate(chest.target.axes[1].channel, scale, 0.0f);
            UpdateAnimRate(chest.target.axes[2].channel, mirrored ? -scale : scale, 0.0f);
            UpdateAnimRate(chest.upperRotation.axes[0].channel, 1.0f, 0.0f);
            UpdateAnimRate(chest.upperRotation.axes[1].channel, sign, 0.0f);
            UpdateAnimRate(chest.upperRotation.axes[2].channel, sign, 0.0f);
        }
        if (g_motionOverrideBlendTime > 0.0f)
            EvaluateChestBlend(chest, static_cast<std::uint8_t>(g_motionAnimFlags), scale, g_motionOverrideBlendTime);
        SetRegionStreams(context, 1);
        chest.clock.current.ticksTime = chest.clock.authored.splitTimeFrames;
        if (graphFrames) {
            BakeChestChannelsToPose(chest, time, static_cast<float>(static_cast<std::int16_t>(context->frameCount)));
            return;
        }
    } else {
        useOverride = g_motionUseOverride;
        g_motionGraphFrames = 0;
    }
    chest.clock.current.splitTimeFrames = chest.clock.authored.splitTimeFrames;
    chest.poseMode = motionPoseFromChannels;
    chest.clock.current.maxTime = chest.clock.authored.ticksTime;
    g_motionPreviousAnimTime = time * motionFrameSeconds;
    g_motionMaximumAnimTime = chest.clock.authored.maxTime * motionFrameSeconds;
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(chest.rotation.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(chest.target.axes[axis].channel);
    if (!useOverride)
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(chest.upperRotation.axes[axis].channel);
}

// @verified - p
void ReadMotionHead(IKRig &rig, MotionHeadRig &head, const MotionStreamContext *context, float time, std::uint32_t flags) {
    g_motionInitFlags = flags;
    if (flags & motionInitStream) {
        g_motionAnimFlags = context->flags;
        if (std::fabs(static_cast<double>(1.2f - context->version)) > 0.01)
            MotionLibraryError("motion\\src\\nlmotion_head.c", 84, "NL_ERROR_MOTION_VERSION");
        head.clock.authored.splitTimeFrames = 0.0f;
        const float last = static_cast<float>(static_cast<std::int16_t>(context->frameCount) - 1);
        head.clock.authored.ticksTime = last;
        head.clock.authored.maxTime = last;
    }
    std::int32_t useOverride;
    if (g_motionInitFlags & motionInitStream) {
        const std::uint32_t graphFrames = g_motionInitFlags >> 8;
        g_motionGraphFrames = static_cast<std::int8_t>(g_motionInitFlags >> 8);
        if (static_cast<std::uint8_t>(g_motionInitFlags >> 8) || (g_motionInitFlags & motionInitCallbacks))
            SolveHead(head);
        useOverride = g_motionUseOverride;
        const float scale = RegionScale(rig, 2);
        const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
        const float sign = mirrored ? -1.0f : 1.0f;
        UpdateAnimRate(head.baseRotation.axes[0].channel, 1.0f, 0.0f);
        UpdateAnimRate(head.baseRotation.axes[1].channel, sign, 0.0f);
        UpdateAnimRate(head.baseRotation.axes[2].channel, sign, 0.0f);
        if (useOverride) {
            MotionRigRotation &source = *head.aimSourceNode;
            UpdateAnimRate(source.axes[0].channel, 1.0f, 0.0f);
            UpdateAnimRate(source.axes[1].channel, sign, 0.0f);
            UpdateAnimRate(source.axes[2].channel, sign, 0.0f);
            MotionRigTarget &target = *head.aimTargetNode;
            UpdateAnimRate(target.axes[0].channel, mirrored ? -scale : scale, 0.0f);
            UpdateAnimRate(target.axes[1].channel, scale, 0.0f);
            UpdateAnimRate(target.axes[2].channel, scale, 0.0f);
        }
        UpdateAnimRate(head.midRotation.axes[0].channel, 1.0f, 0.0f);
        UpdateAnimRate(head.midRotation.axes[1].channel, sign, mirrored ? -3.1415927f : 0.0f);
        UpdateAnimRate(head.midRotation.axes[2].channel, 1.0f, 0.0f);
        UpdateAnimRate(head.target.axes[0].channel, mirrored ? -scale : scale, 0.0f);
        UpdateAnimRate(head.target.axes[1].channel, scale, 0.0f);
        UpdateAnimRate(head.target.axes[2].channel, scale, 0.0f);
        if (useOverride) {
            for (MotionRigRotation *node : {head.nodeA, head.nodeB, head.nodeC}) {
                UpdateAnimRate(node->axes[1].channel, 1.0f, 0.0f);
                UpdateAnimRate(node->axes[0].channel, sign, 0.0f);
                UpdateAnimRate(node->axes[2].channel, sign, mirrored ? 3.1415927f : 0.0f);
            }
        }
        if (g_motionOverrideBlendTime > 0.0f)
            EvaluateHead(head, static_cast<std::uint8_t>(g_motionAnimFlags), scale, g_motionOverrideBlendTime);
        SetRegionStreams(context, 2);
        head.clock.current.ticksTime = head.clock.authored.splitTimeFrames;
        if (graphFrames) {
            BakeHeadChannelsToPose(head, time, static_cast<float>(static_cast<std::int16_t>(context->frameCount)));
            return;
        }
    } else {
        useOverride = g_motionUseOverride;
        g_motionGraphFrames = 0;
    }
    head.clock.current.splitTimeFrames = head.clock.authored.splitTimeFrames;
    head.poseMode = motionPoseFromChannels;
    head.clock.current.maxTime = head.clock.authored.ticksTime;
    g_motionPreviousAnimTime = time * motionFrameSeconds;
    g_motionMaximumAnimTime = head.clock.authored.maxTime * motionFrameSeconds;
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(head.baseRotation.axes[axis].channel);
    if (useOverride) {
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(head.aimSourceNode->axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(head.aimTargetNode->axes[axis].channel);
    }
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(head.midRotation.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(head.target.axes[axis].channel);
    if (useOverride) {
        const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
        if (mirrored)
            for (std::size_t axis = 0; axis < 3; ++axis)
                ReadMotionKeyBlock(head.nodeB->axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(head.nodeA->axes[axis].channel);
        if (!mirrored)
            for (std::size_t axis = 0; axis < 3; ++axis)
                ReadMotionKeyBlock(head.nodeB->axes[axis].channel);
        for (std::size_t axis = 0; axis < 3; ++axis)
            ReadMotionKeyBlock(head.nodeC->axes[axis].channel);
    }
}

// @verified - p
void ReadMotionArm(IKRig &rig, MotionArmRig &arm, const MotionStreamContext *context, float time, std::uint32_t flags) {
    g_motionInitFlags = flags;
    std::uint32_t graphFrames = 0;
    if (flags & motionInitStream) {
        g_motionAnimFlags = context->flags;
        if (std::fabs(static_cast<double>(1.2f - context->version)) > 0.01)
            MotionLibraryError("motion\\src\\nlmotion_arm.c", 84, "NL_ERROR_MOTION_VERSION");
        arm.clock.authored.splitTimeFrames = 0.0f;
        const float last = static_cast<float>(static_cast<std::int16_t>(context->frameCount) - 1);
        arm.clock.authored.ticksTime = last;
        arm.clock.authored.maxTime = last;
    }
    if (!(g_motionInitFlags & motionInitStream)) {
        g_motionGraphFrames = 0;
    } else {
        graphFrames = g_motionInitFlags >> 8;
        g_motionGraphFrames = static_cast<std::int8_t>(g_motionInitFlags >> 8);
        if (static_cast<std::uint8_t>(g_motionInitFlags >> 8) || (g_motionInitFlags & motionInitCallbacks))
            Arm_ChannelsToBasis_Prepass(arm);
        const std::int32_t useOverride = g_motionUseOverride;
        const float scale = RegionScale(rig, 3);
        const bool mirrored = (g_motionInitFlags & motionInitMirror) != 0;
        const float sign = mirrored ? -1.0f : 1.0f;
        UpdateAnimRate(arm.shoulder.axes[0].channel, 1.0f, 0.0f);
        UpdateAnimRate(arm.shoulder.axes[1].channel, sign, 0.0f);
        UpdateAnimRate(arm.shoulder.axes[2].channel, sign, 0.0f);
        UpdateAnimRate(arm.elbow.axes[0].channel, 1.0f, 0.0f);
        UpdateAnimRate(arm.elbow.axes[1].channel, sign, 0.0f);
        UpdateAnimRate(arm.elbow.axes[2].channel, sign, 0.0f);
        if (useOverride) {
            UpdateAnimRate(arm.target.axes[0].channel, mirrored ? -scale : scale, 0.0f);
            UpdateAnimRate(arm.target.axes[1].channel, scale, 0.0f);
            UpdateAnimRate(arm.target.axes[2].channel, scale, 0.0f);
            UpdateAnimRate(arm.wristRotation[0].channel, sign, 0.0f);
            UpdateAnimRate(arm.wristRotation[1].channel, sign, 0.0f);
            UpdateAnimRate(arm.wristRotation[2].channel, 1.0f, 0.0f);
        } else {
            UpdateAnimRate(arm.target.axes[0].channel, scale, 0.0f);
            UpdateAnimRate(arm.target.axes[1].channel, scale, 0.0f);
            UpdateAnimRate(arm.target.axes[2].channel, mirrored ? -scale : scale, 0.0f);
            UpdateAnimRate(arm.wristRotation[0].channel, sign, 0.0f);
            UpdateAnimRate(arm.wristRotation[1].channel, 1.0f, 0.0f);
            UpdateAnimRate(arm.wristRotation[2].channel, sign, 0.0f);
        }
        if (g_motionOverrideBlendTime > 0.0f)
            EvaluateArm(arm, static_cast<std::uint8_t>(g_motionAnimFlags), scale, g_motionOverrideBlendTime);
        bool otherArm = (g_motionInitFlags & motionInitLeftArm) != 0;
        if (g_motionInitFlags & motionInitMirror)
            otherArm = !otherArm;
        SetRegionStreams(context, otherArm ? 4 : 3);
        arm.clock.current.ticksTime = arm.clock.authored.splitTimeFrames;
        if (graphFrames) {
            Arm_ChannelsToTransforms(arm, time, static_cast<float>(static_cast<std::int16_t>(context->frameCount)));
            return;
        }
    }
    arm.clock.current.splitTimeFrames = arm.clock.authored.splitTimeFrames;
    arm.poseMode = motionPoseFromChannels;
    arm.clock.current.maxTime = arm.clock.authored.ticksTime;
    g_motionPreviousAnimTime = time * motionFrameSeconds;
    g_motionMaximumAnimTime = arm.clock.authored.maxTime * motionFrameSeconds;
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(arm.shoulder.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(arm.elbow.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(arm.target.axes[axis].channel);
    for (std::size_t axis = 0; axis < 3; ++axis)
        ReadMotionKeyBlock(arm.wristRotation[axis].channel);
}

// @verified - p
void SolveChestLookAt(MotionChestRig &chest, std::uint32_t interpolationMode) {
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = chest.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = chest.clock.authored.maxTime * motionFrameSeconds;
    if (chest.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = chest.clock.current.maxTime * motionFrameSeconds;
    std::int32_t useOverride = g_motionUseOverride;
    Vector3 target{};
    if (useOverride) {
        const Matrix4x4 saved = workTransformMatrix;
        Matrix4x4::Pop(1);
        if (chest.target.callback.evaluate) {
            EvaluateTargetCallback(chest.target.callback, &target);
            useOverride = g_motionUseOverride;
        } else {
            SampleTargetChannels(chest.target, target);
        }
        target = TransformWorkPoint(target);
        Matrix4x4_Push();
        workTransformMatrix = saved;
        Matrix4x4::preTranslate(0.0f, 0.0f, chest.rotation.axes[2].dimensions[0]);
    } else {
        Matrix4x4::preTranslate(0.0f, chest.rotation.axes[1].dimensions[0], 0.0f);
    }
    if (chest.rotation.callback.evaluate) {
        chest.rotation.callback.evaluate(chest.rotation.callback.parameters);
        useOverride = g_motionUseOverride;
    } else if (chest.poseMode == motionPoseFromChannels) {
        ApplyChannelRotationZYX(chest.rotation.axes);
    } else if (chest.poseMode == motionPoseFromAxisAngle) {
        AxisAngleToBasisSlerp(chest.rotation.basis);
    }
    if (useOverride) {
        StoreTargetOutput(chest.target, target);
        AimBasisAtTarget(target);
        chest.upperChestWorld = workTransformMatrix;
        Matrix4x4::preTranslate(chest.target.axes[0].dimensions[0], 0.0f, 0.0f);
        return;
    }
    Vector3 local;
    if (chest.target.callback.evaluate)
        EvaluateTargetCallback(chest.target.callback, &local);
    else
        SampleTargetChannels(chest.target, local);
    StoreTargetOutput(chest.target, TransformWorkPoint(local));
    AlignFrameToTarget_Vector(local);
    chest.chestWorld = workTransformMatrix;
    Matrix4x4::preTranslate(chest.upperRotation.axes[0].dimensions[0] + chest.target.axes[0].dimensions[0], 0.0f, 0.0f);
    EvaluateJointRotation(chest.upperRotation.axes, chest.upperRotation.basis, chest.upperRotation.callback, chest.poseMode);
    chest.upperChestWorld = workTransformMatrix;
}

// @verified - p
void SolveHeadLookAt(MotionHeadRig &head, std::uint32_t interpolationMode) {
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = head.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = head.clock.authored.maxTime * motionFrameSeconds;
    if (head.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = head.clock.current.maxTime * motionFrameSeconds;
    Vector3 aimTarget{};
    if (g_motionUseOverride) {
        const Matrix4x4 saved = workTransformMatrix;
        Matrix4x4::Pop(2);
        MotionRigTarget &target = *head.aimTargetNode;
        if (target.callback.evaluate)
            EvaluateTargetCallback(target.callback, &aimTarget);
        else
            SampleTargetChannels(target, aimTarget);
        aimTarget = TransformWorkPoint(aimTarget);
        Matrix4x4_Push();
        workTransformMatrix = saved;
        Matrix4x4_Push();
        Matrix4x4::preTranslate(0.0f, head.baseRotation.axes[1].dimensions[0], 0.0f);
    } else {
        Matrix4x4::preTranslate(head.baseRotation.axes[0].dimensions[0], 0.0f, 0.0f);
    }
    EvaluateJointRotation(head.baseRotation.axes, head.baseRotation.basis, head.baseRotation.callback, head.poseMode);
    head.neckWorld = workTransformMatrix;
    float midY = 0.0f;
    if (g_motionUseOverride) {
        MotionRigRotation &source = *head.aimSourceNode;
        Matrix4x4::preTranslate(source.axes[0].dimensions[0], source.axes[1].dimensions[0], 0.0f);
        EvaluateJointRotation(source.axes, source.basis, source.callback, head.poseMode);
        StoreTargetOutput(*head.aimTargetNode, aimTarget);
        AimBasisAtTarget(aimTarget);
        Matrix4x4::GetTransformMatrix(head.aimSourceWorld);
        Matrix4x4::preTranslate(head.aimTargetNode->axes[0].dimensions[0], 0.0f, 0.0f);
        midY = head.midRotation.axes[1].dimensions[0];
    }
    Matrix4x4::preTranslate(head.midRotation.axes[0].dimensions[0], midY, 0.0f);
    EvaluateJointRotation(head.midRotation.axes, head.midRotation.basis, head.midRotation.callback, head.poseMode);
    Vector3 local;
    if (head.target.callback.evaluate)
        EvaluateTargetCallback(head.target.callback, &local);
    else
        SampleTargetChannels(head.target, local);
    head.lookTarget = local;
    StoreTargetOutput(head.target, TransformWorkPoint(local));
    AlignFrameToTarget_Vector(local);
    head.headWorld = workTransformMatrix;
    if (!g_motionUseOverride)
        return;
    Matrix4x4::preTranslate(head.target.axes[0].dimensions[0], 0.0f, 0.0f);
    Matrix4x4_Push();
    Matrix4x4::preTranslate(0.0f, head.nodeA->axes[1].dimensions[0], head.nodeA->axes[2].dimensions[0]);
    EvaluateJointRotation(head.nodeA->axes, head.nodeA->basis, head.nodeA->callback, head.poseMode);
    Matrix4x4::GetTransformMatrix(head.nodeAWorld);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    Matrix4x4::preTranslate(0.0f, head.nodeB->axes[1].dimensions[0], head.nodeB->axes[2].dimensions[0]);
    EvaluateJointRotation(head.nodeB->axes, head.nodeB->basis, head.nodeB->callback, head.poseMode);
    Matrix4x4::GetTransformMatrix(head.nodeBWorld);
    Matrix4x4::Pop(1);
    Matrix4x4::preTranslate(head.nodeC->axes[0].dimensions[0], 0.0f, head.nodeC->axes[2].dimensions[0]);
    EvaluateJointRotation(head.nodeC->axes, head.nodeC->basis, head.nodeC->callback, head.poseMode);
    Matrix4x4::GetTransformMatrix(head.nodeCWorld);
}

// @verified - p
void SolveArmIK2Bone_FKBlend(MotionArmRig &arm, std::uint32_t interpolationMode) {
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = arm.clock.current.ticksTime * motionFrameSeconds;
    g_motionMaximumAnimTime = arm.clock.authored.maxTime * motionFrameSeconds;
    if (arm.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = arm.clock.current.maxTime * motionFrameSeconds;
    std::int32_t useOverride = g_motionUseOverride;
    Vector3 target{};
    Vector4 rootRows[3]{};
    float shoulderY = 0.0f;
    if (useOverride) {
        const Matrix4x4 saved = workTransformMatrix;
        Matrix4x4::Pop(2);
        if (arm.target.callback.evaluate) {
            EvaluateTargetCallback(arm.target.callback, &target);
            useOverride = g_motionUseOverride;
        } else {
            SampleTargetChannels(arm.target, target);
        }
        target = TransformWorkPoint(target);
        rootRows[0] = workTransformMatrix.rows[0];
        rootRows[1] = workTransformMatrix.rows[1];
        rootRows[2] = workTransformMatrix.rows[2];
        Matrix4x4_Push();
        workTransformMatrix = saved;
        Matrix4x4_Push();
        if (useOverride)
            shoulderY = arm.shoulder.axes[1].dimensions[0];
    }
    Matrix4x4::preTranslate(arm.shoulder.axes[0].dimensions[0], shoulderY, arm.shoulder.axes[2].dimensions[0]);
    if (arm.shoulder.callback.evaluate) {
        arm.shoulder.callback.evaluate(arm.shoulder.callback.parameters);
        useOverride = g_motionUseOverride;
    } else if (arm.poseMode == motionPoseFromChannels) {
        ApplyChannelRotationZYX(arm.shoulder.axes);
    } else if (arm.poseMode == motionPoseFromAxisAngle) {
        AxisAngleToBasisSlerp(arm.shoulder.basis);
    }
    arm.shoulderWorld = workTransformMatrix;
    float elbowX = 0.0f, elbowY = 0.0f;
    if (useOverride) {
        elbowY = arm.elbow.axes[1].dimensions[0];
        elbowX = arm.elbow.axes[0].dimensions[0];
    }
    Matrix4x4::preTranslate(elbowX, elbowY, arm.elbow.axes[2].dimensions[0]);
    if (arm.elbow.callback.evaluate) {
        arm.elbow.callback.evaluate(arm.elbow.callback.parameters);
        useOverride = g_motionUseOverride;
    } else if (arm.poseMode == motionPoseFromChannels) {
        ApplyChannelRotationZYX(arm.elbow.axes);
    } else if (arm.poseMode == motionPoseFromAxisAngle) {
        AxisAngleToBasisSlerp(arm.elbow.basis);
    }
    float distance;
    if (useOverride) {
        StoreTargetOutput(arm.target, target);
        distance = AimBasisAtTarget(target);
    } else {
        Vector3 local;
        if (arm.target.callback.evaluate) {
            EvaluateTargetCallback(arm.target.callback, &local);
            useOverride = g_motionUseOverride;
        } else {
            SampleTargetChannels(arm.target, local);
        }
        StoreTargetOutput(arm.target, TransformWorkPoint(local));
        distance = AlignFrameToTarget_Vector(local);
    }
    const float upper = arm.target.axes[1].dimensions[0];
    const float lower = arm.target.axes[0].dimensions[0];
    const float distanceSquared = distance * distance;
    float sine, cosine;
    SolveJointAngle(upper, distance, lower * lower, true, sine, cosine);
    if (useOverride)
        sine = -sine;
    Matrix4x4::RotateAroundZ(sine, cosine);
    arm.upperArmWorld = workTransformMatrix;
    Matrix4x4::preTranslate(arm.target.axes[1].dimensions[0], 0.0f, 0.0f);
    SolveJointAngle(arm.target.axes[1].dimensions[0], arm.target.axes[0].dimensions[0], distanceSquared, false, sine, cosine);
    if (useOverride)
        sine = -sine;
    Matrix4x4::RotateAroundZ(sine, -cosine);
    arm.forearmWorld = workTransformMatrix;
    if (!useOverride) {
        Matrix4x4::preTranslate(arm.wristRotation[1].dimensions[0], 0.0f, 0.0f);
        if (arm.forearmCallback.evaluate)
            arm.forearmCallback.evaluate(arm.forearmCallback.parameters);
        else if (arm.poseMode == motionPoseFromChannels)
            Matrix4x4::RotateAroundX_Wrapper(RigAngle(SampleMotionRigChannelAdvance(arm.wristRotation[0].channel)));
        else if (arm.poseMode == motionPoseFromAxisAngle)
            AxisAngleToBasisSlerp(arm.forearmBasis);
        arm.wristWorld = workTransformMatrix;
        Matrix4x4::preTranslate(arm.wristRotation[0].dimensions[0], 0.0f, 0.0f);
        if (arm.wristCallback.evaluate) {
            arm.wristCallback.evaluate(arm.wristCallback.parameters);
        } else if (arm.poseMode == motionPoseFromChannels) {
            Matrix4x4::RotateAroundZ_Wrapper(RigAngle(SampleMotionRigChannelAdvance(arm.wristRotation[1].channel)));
            Matrix4x4::RotateAroundY_Wrapper(RigAngle(SampleMotionRigChannelAdvance(arm.wristRotation[2].channel)));
        } else if (arm.poseMode == motionPoseFromAxisAngle) {
            AxisAngleToBasisSlerp(arm.wristBasis);
        }
    } else {
        Matrix4x4::preTranslate(arm.target.axes[0].dimensions[0], 0.0f, 0.0f);
        SwizzledBasis3x4IntoActiveFrom(rootRows);
        EvaluateJointRotation(arm.wristRotation, arm.wristBasis, arm.wristCallback, arm.poseMode);
    }
    arm.handWorld = workTransformMatrix;
}

// @verified - p
void IK_BuildAndSolve(IKRig &rig, std::uint32_t interpolationMode) {
    const std::int32_t useOverride = static_cast<std::int32_t>(rig.useOverride);
    g_motionRigBoneId = rig.boneId;
    g_motionUseOverride = useOverride;
    Matrix4x4_Push();
    if (interpolationMode & motionInterpolationHermite)
        rig.lowerBody.rootPrepassTransform = workTransformMatrix;
    else
        workTransformMatrix = rig.lowerBody.rootPrepassTransform;
    Matrix4x4::MultiplyWithPrePassMatrix(&rig.lowerBody.pelvisTransform);
    if (useOverride)
        Matrix4x4_Push();
    SolveBase(rig.lowerBody, interpolationMode);
    SolveChestLookAt(rig.chest, interpolationMode);
    Matrix4x4_Push();
    SolveHeadLookAt(rig.head, interpolationMode);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    if (g_customMotionOverride && (g_customMotionOverride->flags & 1U))
        g_customMotionOverride->evaluate(1, static_cast<std::int32_t>(interpolationMode));
    else
        SolveArmIK2Bone_FKBlend(rig.rightArm, interpolationMode);
    Matrix4x4::Pop(1);
    if (g_motionUseOverride) {
        Matrix4x4_Push();
        SolveArmIK2Bone_FKBlend(rig.leftArm, interpolationMode);
        Matrix4x4::Pop(2);
    } else if (g_customMotionOverride && (g_customMotionOverride->flags & 2U)) {
        g_customMotionOverride->evaluate(2, static_cast<std::int32_t>(interpolationMode));
    } else {
        SolveArmIK2Bone_FKBlend(rig.leftArm, interpolationMode);
    }
    Matrix4x4::Pop(1);
    g_customMotionOverride = nullptr;
}

// @verified - p
void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output) {
    Matrix4x4_Push(&head.headWorld);
    Matrix4x4::preTranslate(head.eyeOffset.x, 0.0f, head.eyeOffset.z);
    Matrix4x4::GetTransformMatrix(output);
    Matrix4x4::Pop(1);
}

} // namespace shenmue::s2
