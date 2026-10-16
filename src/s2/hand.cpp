#include "s2/hand.hpp"

#include "d3t/object_store.hpp"
#include "s2/character.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/hlib.hpp"

#include <algorithm>
#include <bit>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
namespace {
constexpr float epsilon = 0.00000011920929f;
constexpr Vector3 defaultOffset{0.09f, 0.0f, 0.0f};

constexpr float jointTables[4][2][8] = {{{0, -1.0471975803375244f, -0.6981317400932312f, 0, 1.7453293800354004f, 0, 0, 0},
                                            {0, -1.0471975803375244f, 0.6981317400932312f, 0, 1.7453293800354004f, 0, 0, 0}},
    {{0, -0.7853981852531433f, -0.3490658700466156f, 0, 1.5707963705062866f, 0.7853981852531433f, 0, 0},
        {0, -0.7853981852531433f, 0.3490658700466156f, 0, 1.5707963705062866f, -0.7853981852531433f, 0, 0}},
    {{0, -0.8726646900177002f, -0.6981317400932312f, 0.7853981852531433f, 1.7453293800354004f, -0.5235987901687622f, 0, 0},
        {0, -0.8726646900177002f, 0.6981317400932312f, -0.7853981852531433f, 1.7453293800354004f, 0.5235987901687622f, 0, 0}},
    {{-0.3490658700466156f, -0.3490658700466156f, -0.6981317400932312f, -1.3962634801864624f, 1.7453293800354004f, 0, 0, 0},
        {0.3490658700466156f, -0.3490658700466156f, 0.6981317400932312f, 1.3962634801864624f, 1.7453293800354004f, 0, 0, 0}}};

constexpr float jointLimits[2][8][2] = {
    {{-.8726646900177002f, .8726646900177002f}, {-1.7453293800354004f, 1.7453293800354004f}, {-1.2217305898666382f, .3490658700466156f},
        {-1.5707963705062866f, 1.5707963705062866f}, {0, 2.967059850692749f}, {-2.094395160675049f, 2.094395160675049f},
        {-1.3962634801864624f, 1.3962634801864624f}, {-1.5707963705062866f, 1.5707963705062866f}},
    {{-.8726646900177002f, .8726646900177002f}, {-1.7453293800354004f, 1.7453293800354004f}, {-.3490658700466156f, 1.2217305898666382f},
        {-1.5707963705062866f, 1.5707963705062866f}, {0, 2.967059850692749f}, {-2.094395160675049f, 2.094395160675049f},
        {-1.3962634801864624f, 1.3962634801864624f}, {-1.5707963705062866f, 1.5707963705062866f}}};
Character *handCharacter{};
MotionWork *handWork{};
HandIKConfig configurations[2]{};
bool overrideEnabled{};
bool chestOverride{};
bool pairedTarget{};
Matrix4x4 pairTransform{};
Matrix4x4 chestReference{};
Matrix4x4 chestPrevious{};
Vector3 chestPosition{};
Vector3i chestRotation{};
float handBlend{};
std::uint8_t currentSolveStatus{};
std::uint8_t previousSolveStatus{};

Vector3 Row(const Matrix4x4 &matrix, std::size_t row) {
    const auto &value = matrix.rows[row];
    return {value.x, value.y, value.z};
}
Matrix4x4 Identity() {
    return {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}
Matrix4x4 Multiply(const Matrix4x4 &a, const Matrix4x4 &b) {
    Matrix4x4 result{};
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t col = 0; col < 4; ++col)
            for (std::size_t k = 0; k < 4; ++k)
                result.rows[row][col] += a.rows[row][k] * b.rows[k][col];
    return result;
}
Vector3 TransformVector(const Matrix4x4 &matrix, Vector3 value) {
    return Vector3::Add(
        Vector3::Add(Vector3::Scale(Row(matrix, 0), value.x), Vector3::Scale(Row(matrix, 1), value.y)), Vector3::Scale(Row(matrix, 2), value.z));
}
Vector3 TransformPoint(const Matrix4x4 &matrix, Vector3 value) {
    return Vector3::Add(TransformVector(matrix, value), Row(matrix, 3));
}
void Translate(Matrix4x4 &matrix, Vector3 value) {
    const auto p = TransformPoint(matrix, value);
    matrix.rows[3] = {p.x, p.y, p.z, 1};
}
Matrix4x4 Basis(Vector3 x, Vector3 z, Vector3 position) {
    const auto y = Vector3::Cross(z, x);
    return {{{x.x, x.y, x.z, 0}, {y.x, y.y, y.z, 0}, {z.x, z.y, z.z, 0}, {position.x, position.y, position.z, 1}}};
}
std::uint16_t Angle(float radians) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>((radians * 57.29578f) * 182.04443f));
}
void Rotate(std::uint8_t axis, std::uint16_t angle) {
    if (axis == 0)
        Matrix4x4::RotateAroundX_Wrapper(angle);
    else if (axis == 1)
        Matrix4x4::RotateAroundY_Wrapper(angle);
    else if (axis == 2)
        Matrix4x4::RotateAroundZ_Wrapper(angle);
}
MotionArmRig &Arm(MotionWork &work, std::uint8_t side) {
    return side ? work.rig.leftArm : work.rig.rightArm;
}
void SetBlend(float blend) {
    handWork->rig.lowerBody.clock.playbackSpeed = blend;
    handWork->rig.chest.clock.playbackSpeed = blend;
    handWork->rig.head.clock.playbackSpeed = blend;
    handWork->rig.rightArm.clock.playbackSpeed = blend;
    handWork->rig.leftArm.clock.playbackSpeed = blend;
}
void CaptureTarget(HandIKConfig &config, const Matrix4x4 &transform) {
    config.currentTarget = TransformPoint(transform, config.localTargetOffset);
    config.currentAxisX = Row(transform, 0);
    config.currentAxisZ = Row(transform, 2);
}
float Ease(float time, float bias) {
    time = std::clamp(time, 0.0f, 1.0f);
    return time < 0.5f ? ((1.0f - bias) * 2.0f * time) * time + time * bias : (4.0f - 3.0f * bias) * time + ((1.0f - bias) * -2.0f * time) * time + bias - 1.0f;
}
float Derivative(const MotionRigChannel &channel, float time) {
    const auto &a = channel.current;
    const auto &b = channel.next;
    return (6 * time * time - 6 * time) * a.value + (3 * time * time - 4 * time + 1) * a.tangentOut + (-6 * time * time + 6 * time) * b.value +
        (3 * time * time - 2 * time) * b.tangentIn;
}
float PathSpeed(const HandIKConfig &config, float time) {
    return Vector3::CalcMagnitude(
        {Derivative(config.path[0].channel, time), Derivative(config.path[1].channel, time), Derivative(config.path[2].channel, time)});
}
float PathLength(const HandIKConfig &config) {
    float result = 0;
    for (int i = 0; i < 100; ++i)
        result += PathSpeed(config, static_cast<float>(i) / 100.0f) * 0.01f;
    return result;
}
Vector3 SamplePath(const HandIKConfig &config, float fraction) {
    const float target = config.pathLength * fraction;
    float length = 0, time = 0;
    for (int i = 0; i <= 100; ++i) {
        time = static_cast<float>(i) / 100.0f;
        const float step = PathSpeed(config, time) * 0.01f;
        length += step;
        if (length > target) {
            if (step > epsilon)
                time += (target - (length - step)) * 0.01f / step;
            break;
        }
    }
    Vector3 result{};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const auto &a = config.path[axis].channel.current;
        const auto &b = config.path[axis].channel.next;
        result[axis] = ((2 * time - 3) * time * time + 1) * a.value + ((time - 2) * time + 1) * time * a.tangentOut + (-2 * time + 3) * time * time * b.value +
            (time - 1) * time * time * b.tangentIn;
    }
    return result;
}
Vector3 NormalizeMotionAxis(Vector3 axis) {
    const float magnitude = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (magnitude > 0.0f) {
        const float scale = 1.0f / magnitude;
        axis.x *= scale;
        axis.y *= scale;
        axis.z *= scale;
    }
    return axis;
}

Vector3 CrossMotionAxes(const Vector3 &a, const Vector3 &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// AxisAngleToBasisSlerp rotates its source under the reset prepass matrix,
// then multiplies the resulting basis into the caller's transform.
void ApplyMotionRotationBasis(Matrix4x4 &transform, const MotionRotationBasis &basis, float time) {
    Vector3 axis = CrossMotionAxes(basis.sourceAxis, basis.targetAxis);
    const float dot = basis.sourceAxis.x * basis.targetAxis.x + basis.sourceAxis.y * basis.targetAxis.y + basis.sourceAxis.z * basis.targetAxis.z;
    std::uint16_t angle{};
    if (std::fabs(axis.x) > 0.000001f || std::fabs(axis.y) > 0.000001f || std::fabs(axis.z) > 0.000001f) {
        const float magnitude = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(FixedAtan2(magnitude, dot)) * time));
    } else {
        axis = {(basis.sourceTangent.x + basis.targetTangent.x) * 0.5f, (basis.sourceTangent.y + basis.targetTangent.y) * 0.5f,
            (basis.sourceTangent.z + basis.targetTangent.z) * 0.5f};
        if (dot <= 0.0f) {
            angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(time * 32768.0f));
        }
    }
    axis = NormalizeMotionAxis(axis);
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    const float xy = axis.x * axis.y * (1.0f - cosine);
    const float xz = axis.x * axis.z * (1.0f - cosine);
    const float yz = axis.y * axis.z * (1.0f - cosine);
    const Matrix4x4 rotation{{{(1.0f - axis.x * axis.x) * cosine + axis.x * axis.x, xy + axis.z * sine, xz - axis.y * sine, 0.0f},
        {xy - axis.z * sine, (1.0f - axis.y * axis.y) * cosine + axis.y * axis.y, yz + axis.x * sine, 0.0f},
        {xz + axis.y * sine, yz - axis.x * sine, (1.0f - axis.z * axis.z) * cosine + axis.z * axis.z, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};
    axis = {rotation.rows[0].x * basis.sourceAxis.x + rotation.rows[1].x * basis.sourceAxis.y + rotation.rows[2].x * basis.sourceAxis.z,
        rotation.rows[0].y * basis.sourceAxis.x + rotation.rows[1].y * basis.sourceAxis.y + rotation.rows[2].y * basis.sourceAxis.z,
        rotation.rows[0].z * basis.sourceAxis.x + rotation.rows[1].z * basis.sourceAxis.y + rotation.rows[2].z * basis.sourceAxis.z};
    axis = NormalizeMotionAxis(axis);
    Vector3 tangent{(1.0f - time) * basis.sourceTangent.x + time * basis.targetTangent.x, (1.0f - time) * basis.sourceTangent.y + time * basis.targetTangent.y,
        (1.0f - time) * basis.sourceTangent.z + time * basis.targetTangent.z};
    Vector3 binormal = CrossMotionAxes(axis, tangent);
    tangent = NormalizeMotionAxis(CrossMotionAxes(binormal, axis));
    binormal = NormalizeMotionAxis(binormal);
    const Matrix4x4 interpolated{
        {{axis.x, tangent.x, binormal.x, 0.0f}, {axis.y, tangent.y, binormal.y, 0.0f}, {axis.z, tangent.z, binormal.z, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};
    const Matrix4x4 source = transform;
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            transform.rows[row][column] = interpolated.rows[row].x * source.rows[0][column] + interpolated.rows[row].y * source.rows[1][column] +
                interpolated.rows[row].z * source.rows[2][column];
        }
    }
}
Matrix4x4 SampleBasis(const HandIKConfig &config, float time) {
    MotionRotationBasis basis{config.destinationAxisX, Vector3::Cross(config.destinationAxisZ, config.destinationAxisX), config.currentAxisX,
        Vector3::Cross(config.currentAxisZ, config.currentAxisX)};
    Matrix4x4 result = Identity();
    ApplyMotionRotationBasis(result, basis, time);
    return result;
}
void SubmitBeginPose(HandIKConfig &config, std::uint8_t side) {
    if (!config.beginPoseCommand)
        return;
    const std::uint16_t ticks =
        static_cast<std::uint16_t>((config.flags & HandIKFastCommandTiming) ? (config.ticksRemaining + config.commandDelay) / 4U : config.ticksRemaining);
    ApplyCharacterHandPoseCommand(handCharacter, side, config.beginPoseCommand, static_cast<std::int16_t>(ticks));
    config.beginPoseCommand = nullptr;
}
void SubmitEndPose(HandIKConfig &config, std::uint8_t side) {
    const unsigned threshold = (config.totalTicks >> 2U) * ((config.flags & HandIKFastCommandTiming) ? 3U : 1U);
    if (config.endPoseCommand && config.ticksRemaining < threshold) {
        ApplyCharacterHandPoseCommand(handCharacter, side, config.endPoseCommand, config.ticksRemaining);
        config.endPoseCommand = nullptr;
    }
}
// AdvancePath: command-delay ticks do not advance the path or its clock.
bool AdvancePath(HandIKConfig &config, std::uint8_t side, Matrix4x4 &target) {
    if (!(config.flags & HandIKPathActive))
        return false;
    SubmitBeginPose(config, side);
    if (config.commandDelay) {
        --config.commandDelay;
        target = SampleBasis(config, 0);
        const auto p = config.blendTicksRemaining ? config.currentTarget : SamplePath(config, 0);
        target.rows[3] = {p.x, p.y, p.z, 1};
        return true;
    }
    SubmitEndPose(config, side);
    const float time = Ease(static_cast<float>(config.totalTicks - config.ticksRemaining) / config.totalTicks, config.pathBias);
    target = SampleBasis(config, time);
    Vector3 position;
    if (config.flags & HandIKRotatingTarget) {
        Matrix4x4_Push(&config.rotationTransform);
        Rotate(config.rotationAxis, static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(config.rotationAngle) * time)));
        position = TransformPoint(workTransformMatrix, config.rotationOrigin);
        Matrix4x4::Pop(1);
        config.destination = position;
        config.destinationAxisX = Row(target, 0);
        config.destinationAxisZ = Row(target, 2);
    } else if (config.blendTicksRemaining) {
        const float blend = Ease(static_cast<float>(config.blendTotalTicks - config.blendTicksRemaining) / config.blendTotalTicks, config.pathBias);
        const Vector3 start{config.path[0].channel.current.value, config.path[1].channel.current.value, config.path[2].channel.current.value};
        position = Vector3::Add(Vector3::Scale(config.currentTarget, 1 - blend), Vector3::Scale(start, blend));
        --config.blendTicksRemaining;
    } else {
        const float pathTime = Ease(static_cast<float>(config.totalTicks - config.blendTotalTicks - config.ticksRemaining) /
                static_cast<float>(config.totalTicks - config.blendTotalTicks),
            config.pathBias);
        position = SamplePath(config, pathTime);
    }
    target.rows[3] = {position.x, position.y, position.z, 1};
    if (!config.ticksRemaining)
        return false;
    --config.ticksRemaining;
    return true;
}
void TickPose(HandIKConfig &config, std::uint8_t side) {
    if (!config.ticksRemaining) {
        CaptureTarget(config, Arm(*handWork, side).handWorld);
        config.mode = config.pendingMode;
        return;
    }
    SubmitBeginPose(config, side);
    if (config.commandDelay) {
        --config.commandDelay;
        return;
    }
    SubmitEndPose(config, side);
    const float weight = static_cast<float>(config.ticksRemaining - 1U) / config.ticksRemaining;
    for (std::size_t joint = 0; joint < 8; ++joint) {
        const float target = jointTables[config.jointTable][side][joint];
        config.jointAngles[joint] = (config.jointAngles[joint] - target) * weight + target;
    }
    --config.ticksRemaining;
}
Vector3 RotateVectorAroundAxis(Vector3 value, Vector3 axis, float radians) {
    const float length = Vector3::CalcMagnitude(axis);
    if (length <= epsilon)
        return value;
    axis = Vector3::Scale(axis, 1 / length);
    const auto angle = Angle(radians);
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    return Vector3::Add(Vector3::Add(Vector3::Scale(value, cosine), Vector3::Scale(Vector3::Cross(axis, value), sine)),
        Vector3::Scale(axis, Vector3::Dot(axis, value) * (1 - cosine)));
}
void BuildError(const Matrix4x4 &current, const Matrix4x4 &target, float (&error)[6]) {
    const auto position = Vector3::Subtract(Row(target, 3), Row(current, 3));
    const auto axis = Vector3::Cross(Row(current, 0), Row(target, 0));
    const float magnitude = Vector3::CalcMagnitude(axis);
    const auto z = magnitude > epsilon ? RotateVectorAroundAxis(Row(current, 2), axis, magnitude) : Row(current, 2);
    const auto rotation = Vector3::Add(axis, Vector3::Cross(z, Row(target, 2)));
    for (std::size_t i = 0; i < 3; ++i) {
        error[i] = position[i];
        error[i + 3] = rotation[i];
    }
}
void BuildTargetError(HandIKConfig &config, std::uint8_t side, float (&error)[6]) {
    if (config.mode != HandIKMode::Disabled) {
        auto current = Arm(*handWork, side).handWorld;
        Translate(current, config.localTargetOffset);
        if (pairedTarget && side == 1) {
            auto right = handWork->rig.rightArm.handWorld;
            Translate(right, configurations[0].localTargetOffset);
            const auto target = Multiply(pairTransform, right);
            BuildError(current, target, error);
            config.currentTarget = Row(target, 3);
            config.currentAxisX = Row(target, 0);
            config.currentAxisZ = Row(target, 2);
        } else if (config.mode == HandIKMode::DeltaVector) {
            const auto delta = Vector3::Subtract(config.currentTarget, Row(current, 3));
            for (std::size_t i = 0; i < 3; ++i)
                error[i] = delta[i];
        } else {
            Matrix4x4 target{};
            const bool path = config.mode == HandIKMode::PathFollow;
            if (!path || !AdvancePath(config, side, target)) {
                if (path) {
                    config.currentTarget = config.destination;
                    config.currentAxisX = config.destinationAxisX;
                    config.currentAxisZ = config.destinationAxisZ;
                    config.mode = config.pendingMode;
                    config.flags &= static_cast<std::uint8_t>(~HandIKPathActive);
                }
                target = Basis(config.currentAxisX, config.currentAxisZ, config.currentTarget);
            }
            BuildError(current, target, error);
        }
    }
    for (std::size_t half = 0; half < 2; ++half) {
        const float magnitude = Vector3::CalcMagnitude({error[half * 3], error[half * 3 + 1], error[half * 3 + 2]});
        const float limit = half ? 0.17453294f : 0.03f;
        if (magnitude > limit)
            for (std::size_t i = 0; i < 3; ++i)
                error[half * 3 + i] *= limit / magnitude;
    }
}
void BuildJacobian(const HandIKConfig &config, std::uint8_t side, float (&jacobian)[6][8]) {
    const auto &arm = Arm(*handWork, side);
    const auto end = TransformPoint(arm.handWorld, config.localTargetOffset);
    const Vector3 lever[4] = {Vector3::Subtract(end, Row(arm.shoulderWorld, 3)), Vector3::Subtract(end, Row(arm.upperArmWorld, 3)),
        Vector3::Subtract(end, Row(arm.forearmWorld, 3)), TransformVector(arm.handWorld, config.localTargetOffset)};
    Vector3 axes[8];
    axes[0] = Row(workTransformMatrix, 0);
    Matrix4x4_Push(&workTransformMatrix);
    Matrix4x4::RotateAroundZ_Wrapper(0xc000U);
    AlignMotionFrameToTarget(workTransformMatrix, {0, -GetMotionRigArmAxisScale(handWork->rig.boneId), 0});
    axes[1] = Row(workTransformMatrix, 2);
    Matrix4x4::RotateAroundZ_Wrapper(Angle(config.jointAngles[1]));
    axes[2] = Row(workTransformMatrix, 1);
    Matrix4x4::Pop(1);
    axes[3] = Row(arm.upperArmWorld, 0);
    axes[4] = Row(arm.forearmWorld, 2);
    axes[5] = Row(arm.wristWorld, 0);
    axes[6] = Row(arm.wristWorld, 2);
    axes[7] = Row(arm.handWorld, 1);
    for (std::size_t joint = 0; joint < 8; ++joint) {
        const auto linear = Vector3::Cross(axes[joint], lever[joint == 0 ? 0 : joint < 4 ? 1 : joint == 4 ? 2 : 3]);
        for (std::size_t axis = 0; axis < 3; ++axis) {
            jacobian[axis][joint] = linear[axis];
            jacobian[axis + 3][joint] = axes[joint][axis];
        }
    }
}
float Determinant(const float *matrix, std::size_t order) {
    if (order == 1)
        return *matrix;
    float result = 0;
    float minor[25];
    for (std::size_t col = 0; col < order; ++col) {
        std::size_t at = 0;
        for (std::size_t r = 1; r < order; ++r)
            for (std::size_t c = 0; c < order; ++c)
                if (c != col)
                    minor[at++] = matrix[r * order + c];
        result += (col & 1U ? -1.0f : 1.0f) * matrix[col] * Determinant(minor, order - 1);
    }
    return result;
}
// InvertSix chooses the smallest nonzero pivot, rather than a largest-pivot variant.
bool InvertSix(float (&matrix)[6][6]) {
    float inverse[6][6]{};
    for (std::size_t i = 0; i < 6; ++i)
        inverse[i][i] = 1;
    for (std::size_t col = 0; col < 6; ++col) {
        if (std::fabs(matrix[col][col]) < epsilon) {
            std::size_t row = col + 1;
            while (row < 6 && std::fabs(matrix[row][col]) <= epsilon)
                ++row;
            if (row == 6)
                return false;
            for (std::size_t k = 0; k < 6; ++k) {
                std::swap(matrix[col][k], matrix[row][k]);
                std::swap(inverse[col][k], inverse[row][k]);
            }
        }
        for (std::size_t row = col + 1; row < 6; ++row)
            if (std::fabs(matrix[row][col]) > epsilon && std::fabs(matrix[col][col]) > std::fabs(matrix[row][col]))
                for (std::size_t k = 0; k < 6; ++k) {
                    std::swap(matrix[col][k], matrix[row][k]);
                    std::swap(inverse[col][k], inverse[row][k]);
                }
        const float pivot = matrix[col][col];
        for (std::size_t k = 0; k < 6; ++k) {
            matrix[col][k] /= pivot;
            inverse[col][k] /= pivot;
        }
        for (std::size_t row = 0; row < 6; ++row)
            if (row != col) {
                const float factor = matrix[row][col];
                for (std::size_t k = 0; k < 6; ++k) {
                    matrix[row][k] -= factor * matrix[col][k];
                    inverse[row][k] -= factor * inverse[col][k];
                }
            }
    }
    std::memcpy(matrix, inverse, sizeof(inverse));
    return true;
}
float SolveWeighted(const float (&jacobian)[6][8], const float (&weights)[8], const float (&error)[6], float (&delta)[8]) {
    float normal[6][6]{};
    for (std::size_t row = 0; row < 6; ++row)
        for (std::size_t col = 0; col < 6; ++col)
            for (std::size_t joint = 0; joint < 8; ++joint)
                normal[row][col] += jacobian[row][joint] * jacobian[col][joint] / weights[joint];
    const float determinant = Determinant(&normal[0][0], 6);
    if (InvertSix(normal)) {
        float solved[6]{};
        for (std::size_t row = 0; row < 6; ++row)
            for (std::size_t col = 0; col < 6; ++col)
                solved[row] += normal[row][col] * error[col];
        for (std::size_t joint = 0; joint < 8; ++joint) {
            delta[joint] = 0;
            for (std::size_t row = 0; row < 6; ++row)
                delta[joint] += jacobian[row][joint] * solved[row];
            delta[joint] /= weights[joint];
        }
    }
    return determinant;
}
void SolveHand(HandIKConfig &config, std::uint8_t side) {
    float error[6]{};
    BuildTargetError(config, side, error);
    float magnitude = 0;
    for (float value : error)
        magnitude += value * value;
    if (magnitude < epsilon) {
        currentSolveStatus = previousSolveStatus;
        return;
    }
    float jacobian[6][8];
    BuildJacobian(config, side, jacobian);
    float weights[8], delta[8]{};
    const auto ratio = [&](std::size_t joint) {
        const float target = jointTables[config.jointTable][side][joint];
        const float difference = target - config.jointAngles[joint];
        return std::fabs(difference) / (difference > 0 ? target - jointLimits[side][joint][0] : jointLimits[side][joint][1] - target);
    };
    for (std::size_t joint = 0; joint < 8; ++joint)
        weights[joint] = ratio(joint) * 0.9f + 0.1f;
    const float determinant = std::fabs(SolveWeighted(jacobian, weights, error, delta));
    if (determinant < 1.5f) {
        if (determinant < 0.5f)
            currentSolveStatus = 2;
        else if (!currentSolveStatus)
            currentSolveStatus = 1;
    }
    for (std::size_t joint = 0; joint < 8; ++joint) {
        const float difference = jointTables[config.jointTable][side][joint] - config.jointAngles[joint];
        weights[joint] = difference * delta[joint] < 0 ? ratio(joint) * 0.45f + 0.55f : 0.55f - ratio(joint) * 0.45f;
        delta[joint] = 0;
    }
    if (std::fabs(SolveWeighted(jacobian, weights, error, delta)) > 0.01f)
        for (std::size_t joint = 0; joint < 8; ++joint)
            config.jointAngles[joint] = std::clamp(config.jointAngles[joint] + delta[joint], jointLimits[side][joint][0], jointLimits[side][joint][1]);
}

void EvaluateArm(MotionWork &work, std::uint8_t side, std::uint32_t interpolationMode) {
    auto &arm = Arm(work, side);
    auto &config = configurations[side];
    const auto &angles = config.jointAngles;
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = arm.clock.current.ticksTime * 0.033333335f;
    g_motionMaximumAnimTime = arm.clock.authored.maxTime * 0.033333335f;
    if (arm.poseMode == motionPoseFromAxisAngle)
        g_motionMaximumAnimTime = arm.clock.current.maxTime * 0.033333335f;
    Matrix4x4::preTranslate(arm.shoulder.axes[0].dimensions[0], 0, arm.shoulder.axes[2].dimensions[0]);
    if (config.flags & HandIKEnabled) {
        if (config.mode == HandIKMode::HandPose)
            TickPose(config, side);
        else
            SolveHand(config, side);
    }
    Matrix4x4::RotateAroundX_Wrapper(Angle(angles[0]));
    arm.shoulderWorld = workTransformMatrix;
    Matrix4x4::preTranslate(0, 0, arm.elbow.axes[2].dimensions[0]);
    Matrix4x4::RotateAroundZ_Wrapper(0xc000U);
    AlignMotionFrameToTarget(workTransformMatrix, {0, -GetMotionRigArmAxisScale(work.rig.boneId), 0});
    Matrix4x4::RotateAroundZ_Wrapper(Angle(angles[1]));
    Matrix4x4::RotateAroundY_Wrapper(Angle(angles[2]));
    Matrix4x4::RotateAroundX_Wrapper(Angle(angles[3]));
    arm.upperArmWorld = workTransformMatrix;
    Matrix4x4::preTranslate(arm.target.axes[1].dimensions[0], 0, 0);
    Matrix4x4::RotateAroundZ_Wrapper(Angle(angles[4]));
    arm.forearmWorld = workTransformMatrix;
    Matrix4x4::preTranslate(arm.wristRotation[1].dimensions[0], 0, 0);
    Matrix4x4::RotateAroundX_Wrapper(Angle(angles[5]));
    arm.wristWorld = workTransformMatrix;
    Matrix4x4::preTranslate(arm.wristRotation[0].dimensions[0], 0, 0);
    Matrix4x4::RotateAroundZ_Wrapper(Angle(angles[6]));
    Matrix4x4::RotateAroundY_Wrapper(Angle(angles[7]));
    arm.handWorld = workTransformMatrix;
}
Matrix4x4 ChestOverrideMatrix() {
    auto inverse = chestReference;
    Matrix4x4::InvertInPlace(&inverse);
    const auto relative = Multiply(chestPrevious, inverse);
    Matrix4x4_Push(nullptr);
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(chestPosition.x, chestPosition.y, chestPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(chestRotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(chestRotation.x));
    Matrix4x4::RotateAroundX_Wrapper(0x4000U);
    Matrix4x4::RotateAroundY_Wrapper(0x4000U);
    const auto result = Multiply(relative, workTransformMatrix);
    Matrix4x4::Pop(1);
    return result;
}
} // namespace

MotionOverride g_searchHandMotionOverride{};

float SearchIK_GetMaxBlend() {
    return handBlend;
}
void setSearchIKFlag(float blend) {
    handBlend = blend;
}
HandIKMode getSearchHandConfigModeFor(std::uint8_t side) {
    return handWork && side < 2 ? configurations[side].mode : HandIKMode::Unavailable;
}
void SearchIK_SetTarget(const Matrix4x4 *transform) {
    if (handWork && transform && (configurations[0].flags & HandIKEnabled) && (configurations[1].flags & HandIKEnabled)) {
        pairedTarget = true;
        pairTransform = *transform;
    }
}
bool BeginSearchHand(Character *character, std::int32_t handSelection, std::uint32_t jointTable) {
    if (!character)
        return false;
    handCharacter = character;
    handWork = FindMotionWork(character);
    if (!handWork) {
        handCharacter = nullptr;
        return false;
    }
    if (handWork->runtime.seqFlags & MotionSequenceSearchHand)
        return true;
    for (auto flags : handWork->runtime.regionFlags)
        if (flags & 0x10U)
            return false;
    if (jointTable > 3)
        jointTable = 0;
    handWork->runtime.seqFlags |= MotionSequenceSearchHand;
    g_searchHandMotionOverride = {.flags = static_cast<std::uint32_t>(handSelection + 1), .reserved04 = 0, .evaluate = MotionCommonCallback};
    std::memset(configurations, 0, sizeof(configurations));
    const bool models = SetLowQualityHands(character) != 0;
    for (std::uint8_t side = 0; side < 2; ++side) {
        auto &config = configurations[side];
        config.localTargetOffset = defaultOffset;
        config.mode = HandIKMode::HoldTransform;
        config.jointTable = static_cast<std::uint8_t>(jointTable);
        std::copy(std::begin(jointTables[jointTable][side]), std::end(jointTables[jointTable][side]), config.jointAngles);
        for (MotionRigAxis &axis : config.path) {
            axis.channel.next.timeSeconds = 1;
            axis.channel.inverseDuration = 1;
        }
        if (!(g_searchHandMotionOverride.flags & (1U << side)))
            continue;
        Matrix4x4_Push(&handWork->rig.chest.upperChestWorld);
        EvaluateArm(*handWork, side, motionInterpolationHermiteAuto);
        CaptureTarget(config, Arm(*handWork, side).handWorld);
        Matrix4x4::Pop(1);
        config.flags |= HandIKEnabled;
        if (models)
            SetCharacterHandRegionEnabled(character, side, true, 1);
    }
    currentSolveStatus = previousSolveStatus = 0;
    setSearchIKFlag(1);
    return true;
}
bool SetSearchHandOverrideEnabled(bool enabled) {
    if (!handWork)
        return false;
    overrideEnabled = enabled;
    return enabled;
}
bool mtwkApplyHand(HandIKMode mode, std::uint8_t side) {
    if (!handWork || side >= 2)
        return false;
    auto &config = configurations[side];
    switch (mode) {
    case HandIKMode::Disabled:
    case HandIKMode::HoldTransform:
    case HandIKMode::DeltaVector:
        SetBlend(1);
        config.mode = mode;
        break;
    case HandIKMode::Unblended:
        SetBlend(0);
        config.mode = mode;
        break;
    case HandIKMode::PathFollow:
        if (config.flags & HandIKPathActive) {
            config.pendingMode = config.mode;
            config.mode = mode;
        }
        break;
    case HandIKMode::HandPose:
        if (config.mode != HandIKMode::PathFollow)
            config.pendingMode = config.mode;
        config.flags &= static_cast<std::uint8_t>(~HandIKPathActive);
        config.ticksRemaining = config.totalTicks;
        config.mode = mode;
        break;
    default:
        break;
    }
    return true;
}
bool srchSetTargetsWithParams(std::int32_t side, const Matrix4x4 *transform, const Vector3 *tangentOrigin, const Vector3 *pathOrigin, std::uint8_t ticks) {
    if (!handWork || !transform || static_cast<unsigned>(side) >= 2)
        return false;
    auto &config = configurations[side];
    config.destinationAxisX = Row(*transform, 0);
    config.destinationAxisZ = Row(*transform, 2);
    config.destination = Row(*transform, 3);
    config.totalTicks = config.ticksRemaining = ticks;
    config.flags |= HandIKPathActive;
    const auto start = pathOrigin ? *pathOrigin : config.currentTarget;
    auto incoming = Vector3::Subtract(config.destination, tangentOrigin ? *tangentOrigin : start);
    auto outgoing = Vector3::Subtract(pathOrigin ? *pathOrigin : config.destination, config.currentTarget);
    const float tangentLength = Vector3::CalcMagnitude(incoming);
    const float outgoingLength = Vector3::CalcMagnitude(outgoing);
    outgoing = Vector3::Scale(outgoing, outgoingLength > epsilon ? tangentLength * 2.5f / outgoingLength : 0);
    incoming = Vector3::Scale(incoming, tangentLength > epsilon ? tangentLength * 2.5f / tangentLength : 0);
    for (std::size_t axis = 0; axis < 3; ++axis) {
        config.path[axis] = {};
        MotionRigChannel &channel = config.path[axis].channel;
        channel.current = {0, 0, outgoing[axis], start[axis]};
        channel.next = {1, incoming[axis], 0, config.destination[axis]};
        channel.inverseDuration = 1;
    }
    config.pathLength = PathLength(config);
    if (pathOrigin)
        config.flags |= HandIKBlendToPath;
    else
        config.flags &= static_cast<std::uint8_t>(~HandIKBlendToPath);
    const float blendLength = pathOrigin ? Vector3::CalcMagnitude(Vector3::Subtract(*pathOrigin, config.currentTarget)) : 0;
    config.blendTotalTicks = config.blendTicksRemaining =
        static_cast<std::uint8_t>(pathOrigin && config.pathLength + blendLength > epsilon ? blendLength / (config.pathLength + blendLength) * ticks : 0);
    config.flags &= static_cast<std::uint8_t>(~(HandIKFastCommandTiming | HandIKRotatingTarget));
    config.commandDelay = 0;
    config.beginPoseCommand = config.endPoseCommand = nullptr;
    return true;
}
bool get_x_from_target(Vector3 *targetOffset, std::uint8_t side) {
    if (!handWork || !targetOffset || side >= 2)
        return false;
    *targetOffset = configurations[side].localTargetOffset;
    return true;
}
bool srchSetXformFor(const Vector3 *targetOffset, std::uint8_t side) {
    if (!handWork || side >= 2)
        return false;
    configurations[side].localTargetOffset = targetOffset ? *targetOffset : defaultOffset;
    return true;
}
bool srchSetSearchJointAngles(std::uint8_t side, std::byte *beginCommand, std::byte *endCommand, std::int32_t fastTiming) {
    if (!handWork || side >= 2 || !beginCommand || !configurations[side].ticksRemaining)
        return false;
    auto &config = configurations[side];
    if (fastTiming)
        config.flags |= HandIKFastCommandTiming;
    else
        config.flags &= static_cast<std::uint8_t>(~HandIKFastCommandTiming);
    config.commandDelay = fastTiming ? 5U : 0U;
    config.beginPoseCommand = beginCommand;
    config.endPoseCommand = endCommand;
    return true;
}
bool SetSearchHandRotatingTarget(
    std::uint8_t side, const Matrix4x4 *transform, const Matrix4x4 *rotationTransform, std::uint8_t axis, std::uint16_t angle, std::uint8_t ticks) {
    if (!handWork || side >= 2 || !transform || !rotationTransform)
        return false;
    auto &config = configurations[side];
    config.destinationAxisX = Row(*transform, 0);
    config.destinationAxisZ = Row(*transform, 2);
    config.totalTicks = config.ticksRemaining = ticks;
    config.rotationAngle = angle;
    config.rotationAxis = axis;
    config.rotationTransform = *rotationTransform;
    auto inverse = *rotationTransform;
    Matrix4x4::InvertInPlace(&inverse);
    config.rotationOrigin = TransformPoint(inverse, TransformPoint(Arm(*handWork, side).handWorld, config.localTargetOffset));
    config.flags = static_cast<std::uint8_t>((config.flags & ~(HandIKBlendToPath | HandIKFastCommandTiming)) | HandIKPathActive | HandIKRotatingTarget);
    config.commandDelay = 0;
    config.beginPoseCommand = config.endPoseCommand = nullptr;
    return true;
}
std::uint8_t srchSetJointTable(std::uint8_t table, std::uint8_t side) {
    if (!handWork || side >= 2)
        return 4;
    const auto old = configurations[side].jointTable;
    if (table < 4)
        configurations[side].jointTable = table;
    return old;
}
bool srchSetNewTicksTimeFor(std::uint8_t ticks, std::uint8_t side) {
    if (!handWork || side >= 2)
        return false;
    auto &config = configurations[side];
    if (config.mode != HandIKMode::PathFollow && config.mode != HandIKMode::HandPose)
        config.pendingMode = config.mode;
    config.flags &= static_cast<std::uint8_t>(~HandIKPathActive);
    config.mode = HandIKMode::HandPose;
    config.totalTicks = config.ticksRemaining = ticks;
    return true;
}
std::int32_t srchGetTicksDifference(std::uint8_t side) {
    if (!handWork || side >= 2)
        return 0;
    const auto &config = configurations[side];
    return config.mode == HandIKMode::PathFollow || config.mode == HandIKMode::HandPose ? config.totalTicks - config.ticksRemaining : 0;
}
std::int32_t srchGetCurrentTotalTicks(std::uint8_t side) {
    if (!handWork || side >= 2)
        return 0;
    const auto &config = configurations[side];
    return config.mode == HandIKMode::PathFollow || config.mode == HandIKMode::HandPose ? config.totalTicks : 0;
}
Matrix4x4 *mtwkGetSearchArmTransform(std::uint8_t side) {
    return handWork && side < 2 ? &Arm(*handWork, side).handWorld : nullptr;
}
void GetSearchHandJointRotation(std::uint8_t side, Vector3i *rotation) {
    if (!handWork || side >= 2 || !rotation)
        return;
    const auto &angles = configurations[side].jointAngles;
    *rotation = {Angle(angles[5]), Angle(angles[7]), Angle(angles[6])};
}
bool SetSearchHandCurrentTransform(const Matrix4x4 *transform, std::uint8_t side) {
    if (!handWork || side >= 2 || !transform)
        return false;
    auto &config = configurations[side];
    config.currentTarget = Row(*transform, 3);
    config.currentAxisX = Row(*transform, 0);
    config.currentAxisZ = Row(*transform, 2);
    return true;
}
void SetSearchHandChestTransform(const Vector3 *position, const Vector3i *rotation) {
    if (!handWork || !position || !rotation)
        return;
    chestPosition = *position;
    chestRotation = *rotation;
    if (!chestOverride)
        chestReference = chestPrevious = handWork->rig.chest.upperChestWorld;
    const auto chest = ChestOverrideMatrix();
    for (std::uint8_t side = 0; side < 2; ++side)
        if (g_searchHandMotionOverride.flags & (1U << side)) {
            configurations[side].flags &= static_cast<std::uint8_t>(~HandIKEnabled);
            Matrix4x4_Push(&chest);
            EvaluateArm(*handWork, side, motionInterpolationHermiteAuto);
            CaptureTarget(configurations[side], Arm(*handWork, side).handWorld);
            configurations[side].flags |= HandIKEnabled;
            Matrix4x4::Pop(1);
        }
    chestOverride = true;
}
void mtwkResetBlends() {
    if (handWork) {
        handWork->runtime.seqFlags &= ~MotionSequenceSearchHand;
        SetBlend(1);
    }
    if (handCharacter)
        for (std::uint8_t side = 0; side < 2; ++side)
            if (g_searchHandMotionOverride.flags & (1U << side))
                SetCharacterHandRegionEnabled(handCharacter, side, false, 1);
    ResetSearchHandState();
}
void ResetSearchHandState() {
    handCharacter = nullptr;
    handWork = nullptr;
    overrideEnabled = false;
    chestOverride = false;
    pairedTarget = false;
}
bool EvaluateSearchHandArm(MotionWork &work, std::uint8_t side, std::uint32_t interpolationMode) {
    if (!GetCustomMotionOverrideFlags(&work) || side >= 2 || !(g_searchHandMotionOverride.flags & (1U << side)))
        return false;
    if (chestOverride) {
        chestPrevious = workTransformMatrix;
        workTransformMatrix = ChestOverrideMatrix();
        work.rig.chest.upperChestWorld = workTransformMatrix;
    }
    EvaluateArm(work, side, interpolationMode);
    if (side == 1)
        pairedTarget = false;
    return true;
}
// MotionCommonCallback: the search-hand arm solver IK_BuildAndSolve runs in place of an
// arm's two-bone solve; the mode bits select the right (1) and left (2) arms.
void MotionCommonCallback(std::int64_t mode, std::int32_t interpolationMode) {
    if (!handWork)
        return;
    if (mode & 1)
        EvaluateSearchHandArm(*handWork, 0, static_cast<std::uint32_t>(interpolationMode));
    if (mode & 2)
        EvaluateSearchHandArm(*handWork, 1, static_cast<std::uint32_t>(interpolationMode));
}
// @verified - p
void AssignMotionCallbacks(MotionWork *work) {
    if (work == handWork && (work->runtime.seqFlags & MotionSequenceSearchHand))
        SetCustomMotionOverride(&g_searchHandMotionOverride);
    previousSolveStatus = currentSolveStatus;
    currentSolveStatus = 0;
}
std::uint32_t GetCustomMotionOverrideFlags(const MotionWork *work) {
    return work && work == handWork && (work->runtime.seqFlags & MotionSequenceSearchHand) && overrideEnabled ? g_searchHandMotionOverride.flags : 0;
}
bool ClearSearchHandMotionRequests(MotionWork *work) {
    if (!work || work != handWork || !(work->runtime.seqFlags & MotionSequenceSearchHand))
        return false;
    auto &state = work->runtime;
    for (std::size_t region = 0; region < 5; ++region) {
        state.queuedMotionId[region] = 0;
        state.pendingLayerStartFrame[region] = -1;
        state.layerLoopStartTime[region] = -1;
        state.layerEndFrame[region] = -1;
    }
    state.channelFlags = 9;
    state.requestedRootYawOffset = 0;
    state.overrideBlendTime = -1;
    std::fill(std::begin(state.activeConstraintTarget), std::end(state.activeConstraintTarget), nullptr);
    std::fill(std::begin(state.latchedConstraintTarget), std::end(state.latchedConstraintTarget), nullptr);
    return true;
}
// @verified - p
void GetSearchHandWristAngles(MotionWork &work, std::int16_t (&angles)[4]) {
    g_motionInterpolationType = (work.runtime.seqFlags >> motionSequenceInterpolationShift) & motionInterpolationLinear;
    const std::uint32_t flags = GetCustomMotionOverrideFlags(&work);
    for (std::uint8_t side = 0; side < 2; ++side) {
        MotionArmRig &arm = Arm(work, side);
        float yaw, roll;
        if (flags & (1U << side)) {
            yaw = configurations[side].jointAngles[7];
            roll = configurations[side].jointAngles[6];
        } else {
            g_motionCurrentAnimTime = arm.clock.current.ticksTime * 0.033333335f;
            g_motionMaximumAnimTime = arm.clock.authored.maxTime * 0.033333335f;
            yaw = SampleMotionRigChannelAdvance(arm.wristRotation[2].channel);
            roll = SampleMotionRigChannelAdvance(arm.wristRotation[1].channel);
        }
        angles[side * 2] = std::bit_cast<std::int16_t>(FloatToShort((yaw * 57.29578f) * 182.04443f));
        angles[side * 2 + 1] = std::bit_cast<std::int16_t>(FloatToShort((roll * 57.29578f) * 182.04443f));
    }
}

HandParams *GetCharacterHand(Character *character, std::int32_t side) {
    if (!character)
        return nullptr;
    return static_cast<HandParams *>(FindModelComponent(character, shen_name{side ? SHEN_NAME(HNDL) : SHEN_NAME(HNDR)}));
}

bool IsHandPoseInactive(const HandParams &hand) {
    return (hand.poseFlags & HandPoseInactive) != 0;
}

std::uint8_t SetHandPoseFlags(HandParams &hand, std::int16_t enabled, std::uint8_t flags) {
    const std::uint8_t previous = hand.poseFlags;
    std::int16_t result = previous & HandPoseReverse;
    hand.poseFlags = enabled ? previous | flags : previous & static_cast<std::uint8_t>(~flags);
    bool reset = false;
    if ((hand.poseFlags & HandPoseReverse) && !(previous & HandPoseReverse)) {
        result = hand.firstPoseFrame;
        if (hand.currentPoseFrame <= result) {
            result = hand.lastPoseFrame;
            hand.currentPoseFrame = result;
            reset = true;
        }
    }
    if (!(hand.poseFlags & HandPoseReverse) && (previous & HandPoseReverse)) {
        result = hand.lastPoseFrame;
        if (hand.currentPoseFrame >= result) {
            result = hand.firstPoseFrame;
            hand.currentPoseFrame = result;
            reset = true;
        }
    }
    if (reset) {
        hand.previousPoseFrame = hand.currentPoseFrame;
        hand.poseFrame = static_cast<float>(hand.currentPoseFrame);
    }
    return static_cast<std::uint8_t>(result);
}

std::uint64_t GetHandPoseAnimationResult(const HandParams &hand) {
    return static_cast<std::uint32_t>(hand.poseMode ? -static_cast<std::int32_t>(hand.poseTicks) : hand.currentPoseFrame + 1);
}

void SetHandPoseStep(HandParams &hand, float step) {
    if (step > 0.0f)
        hand.poseStep = step;
}

void SetHandSecondaryPoseWork(HandParams &hand, void *work) {
    if (hand.secondaryPoseWork == work)
        return;
    hand.secondaryPoseWork = work;
    std::fill(std::begin(hand.boundsWork), std::end(hand.boundsWork), std::numeric_limits<float>::max());
}

void GetHandEndpointPosition(const HandParams &hand, std::int32_t endpoint, Vector3 &position) {
    position = hand.jointPositions[handEndBoneIds[endpoint]];
}

void GetHandPrimaryNodeRotation(const HandParams &hand, Vector3i &rotation) {
    rotation = hand.primaryNode ? hand.primaryNode->rotation : Vector3i{};
}

MT7Node *SetHandPrimaryNodeRotation(HandParams &hand, const Vector3i &rotation) {
    if (hand.primaryNode)
        hand.primaryNode->rotation = rotation;
    return hand.primaryNode;
}

// @verified - p
std::byte *WriteHandPoseCommand(const HandParams &hand, std::byte *command) {
    for (std::size_t index = 0; index < 19; ++index) {
        const std::int16_t *const angles = hand.currentJointAngles[19 - index];
        const std::int32_t values[3]{angles[0], angles[1], angles[2]};
        std::memcpy(command + index * sizeof(values), values, sizeof(values));
    }
    return command + 236;
}

std::uint64_t ParseHandPoseCommand(HandParams *hand, std::byte *command, std::int16_t ticks) {
    if (d3t::GetObjectModelPathHash(GetMT7ReaderData(hand->owner ? static_cast<const MT7Model *>(hand->owner->model) : nullptr)) ==
            HAND_POSE_REPLACEMENT_OWNER_HASH &&
        std::memcmp(command, handPoseReplacementMatch, sizeof(handPoseReplacementMatch)) == 0)
        command = reinterpret_cast<std::byte *>(handPoseReplacementCommand);
    std::uint64_t result = 0;
    hand->poseMode = 1;
    std::memset(hand->targetJointAngles, 0, sizeof(hand->targetJointAngles));
    for (std::size_t index = 0; index < 19; ++index) {
        const std::size_t joint = 19 - index;
        std::int32_t values[3];
        std::memcpy(values, command + index * sizeof(values), sizeof(values));
        if (joint > 17 || ((joint - 1) & 3U)) {
            for (std::size_t axis = 0; axis < 2; ++axis)
                if ((values[axis] & HAND_POSE_COMMAND_RANGE_MASK) != 0 && (values[axis] & HAND_POSE_COMMAND_RANGE_MASK) != HAND_POSE_COMMAND_RANGE_MASK)
                    values[axis] = 0;
            if (!(values[2] & HAND_POSE_COMMAND_SIGN_BIT) && (values[2] & HAND_POSE_COMMAND_RANGE_MASK))
                values[2] = 0;
            std::memcpy(command + index * sizeof(values), values, sizeof(values));
        }
        for (std::size_t axis = 0; axis < 3; ++axis)
            hand->targetJointAngles[joint][axis] = static_cast<std::int16_t>(values[axis]);
        result = static_cast<std::uint16_t>(values[2]);
    }
    hand->poseTicks = ticks;
    if (ticks < 1) {
        std::memcpy(hand->currentJointAngles, hand->targetJointAngles, sizeof(hand->currentJointAngles));
        StageHandPose(*hand);
        hand->immediatePoseResult = static_cast<std::uint8_t>(StageImmediateHandPose(hand));
        hand->poseTicks = 1;
        return 1;
    }
    return result;
}

std::uint64_t ApplyCharacterHandPoseCommand(Character *character, std::int32_t side, std::byte *command, std::int16_t ticks) {
    HandParams *const hand = GetCharacterHand(character, side);


    return hand ? ParseHandPoseCommand(hand, command, ticks) : 0;
}

namespace {
bool SetHandRegionEnabled(HandParams &hand, std::int8_t state, std::int16_t pose, std::int8_t snap) {
    if ((!hand.primaryNode || state < 0) && (!hand.secondaryNode || state > 0) && !hand.externalDrawControl)
        return false;
    if (!state && hand.regionState)
        hand.previousRegionState = hand.regionState;
    hand.regionState = state;
    if (snap >= 0) {
        hand.modelAlpha = state ? 1.0f : 0.0f;
        hand.previousRegionState = state;
    }
    if (!state)
        return true;
    if (pose)
        ConfigureHandPoseSequence(hand, pose, 0, 0, 0, 1, 1.0f, 0);
    else {
        std::memset(hand.currentJointAngles, 0, sizeof(hand.currentJointAngles));
        for (std::size_t index = 0; index < 19; ++index)
            for (std::size_t axis = 0; axis < 3; ++axis)
                hand.currentJointAngles[19 - index][axis] = static_cast<std::int16_t>(handDefaultJointAngles[index][axis]);
        std::memcpy(hand.targetJointAngles, hand.currentJointAngles, sizeof(hand.targetJointAngles));
        hand.poseMode = 1;
        hand.poseTicks = 1;
    }
    HandModelResource *const resource = GetHandModelResource(hand);
    if (resource)
        resource->poseDirty = true;
    return true;
}
}

bool SetCharacterHandRegionEnabled(Character *character, std::uint8_t side, bool enabled, std::int8_t alphaPolicy) {
    HandParams *const hand = GetCharacterHand(character, side);
    return hand && SetHandRegionEnabled(*hand, enabled ? 1 : 0, 0, alphaPolicy);
}

MT7Node *SetHighModelXYZ(Character *character, std::int8_t state, std::int32_t side, std::int16_t pose) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (!hand)
        return nullptr;
    SetHandRegionEnabled(*hand, state, pose, 1);
    return SetHandPrimaryNodeRotation(*hand, {});
}

MT7Node *SetHandRegionXYZWithFlags(Character *character, std::int8_t state, std::int32_t side, std::int16_t pose, std::int8_t alphaPolicy) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (!hand)
        return nullptr;
    SetHandRegionEnabled(*hand, state, pose, alphaPolicy);
    return SetHandPrimaryNodeRotation(*hand, {});
}

bool ApplyScenarioLeftHandPose(Character *character) {
    // ApplyScenarioLeftHandPose selects this authored pose only for RYO/InternalFlag123.
    static std::int32_t pose[19][3] = {{1999, 65354, 60806}, {1638, 180, 63171}, {546, 64719, 3365}, {0, 64810, 53705}, {0, 64810, 49335},
        {0, 0, 60075}, {0, 0, 64811}, {0, 0, 56800}, {0, 0, 47516}, {0, 65172, 56618}, {0, 0, 65354}, {0, 729, 55706}, {0, 65173, 49338}, {0, 64990, 56256},
        {0, 0, 0}, {0, 0, 56434}, {0, 1639, 49882}, {0, 182, 59349}, {64080, 65354, 64444}};
    return ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(pose), 0) != 0;
}

// @verified - p
std::int8_t AdvanceHandDrawState(HandParams &hand) {
    const float step = HLib::GetScaledDeltaTick() * 0.1f;
    if (hand.regionState) {
        hand.modelAlpha += step;
        if (hand.modelAlpha > 1.0f)
            hand.modelAlpha = 1.0f;
        return hand.regionState;
    }
    hand.modelAlpha -= step;
    if (hand.modelAlpha <= 0.0f) {
        hand.modelAlpha = 0.0f;
        hand.previousRegionState = 0;
    }
    return hand.previousRegionState;
}
} // namespace shenmue::s2
