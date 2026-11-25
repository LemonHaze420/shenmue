#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hand_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/character_transparency_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include <cmath>
#include <cstring>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

constexpr float epsilon = 0.00000011920929f;

constexpr Vector3 defaultOffset{0.09f, 0.0f, 0.0f};

// Native jointTables, table-major/right-before-left; radians.
constexpr float jointTables[4][2][8] = {{{0, -1.0471975803375244f, -0.6981317400932312f, 0, 1.7453293800354004f, 0, 0, 0},
                                            {0, -1.0471975803375244f, 0.6981317400932312f, 0, 1.7453293800354004f, 0, 0, 0}},
    {{0, -0.7853981852531433f, -0.3490658700466156f, 0, 1.5707963705062866f, 0.7853981852531433f, 0, 0},
        {0, -0.7853981852531433f, 0.3490658700466156f, 0, 1.5707963705062866f, -0.7853981852531433f, 0, 0}},
    {{0, -0.8726646900177002f, -0.6981317400932312f, 0.7853981852531433f, 1.7453293800354004f, -0.5235987901687622f, 0, 0},
        {0, -0.8726646900177002f, 0.6981317400932312f, -0.7853981852531433f, 1.7453293800354004f, 0.5235987901687622f, 0, 0}},
    {{-0.3490658700466156f, -0.3490658700466156f, -0.6981317400932312f, -1.3962634801864624f, 1.7453293800354004f, 0, 0, 0},
        {0.3490658700466156f, -0.3490658700466156f, 0.6981317400932312f, 1.3962634801864624f, 1.7453293800354004f, 0, 0, 0}}};

// Native jointLimits: lower/upper limits, not target angles.
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

} // namespace

MotionOverride g_searchHandMotionOverride{};

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

Vector3 Row(const Matrix4x4 &matrix, std::size_t row);
Matrix4x4 Multiply(const Matrix4x4 &a, const Matrix4x4 &b);
Vector3 TransformVector(const Matrix4x4 &matrix, Vector3 value);
Vector3 TransformPoint(const Matrix4x4 &matrix, Vector3 value);
void Translate(Matrix4x4 &matrix, Vector3 value);
Matrix4x4 Basis(Vector3 x, Vector3 z, Vector3 position);
std::uint16_t Angle(float radians);
MotionArmRig &Arm(MotionWork &work, std::uint8_t side);
void SetBlend(float blend);
float Ease(float time, float bias);
float Derivative(const MotionRigChannel &channel, float time);
float PathSpeed(const HandIKConfig &config, float time);
float PathLength(const HandIKConfig &config);
Vector3 SamplePath(const HandIKConfig &config, float fraction);
Matrix4x4 SampleBasis(const HandIKConfig &config, float time);
void SubmitBeginPose(HandIKConfig &config, std::uint8_t side);
void SubmitEndPose(HandIKConfig &config, std::uint8_t side);
bool AdvancePath(HandIKConfig &config, std::uint8_t side, Matrix4x4 &target);
void BuildError(const Matrix4x4 &current, const Matrix4x4 &target, float (&error)[6]);
void BuildTargetError(HandIKConfig &config, std::uint8_t side, float (&error)[6]);
void BuildJacobian(const HandIKConfig &config, std::uint8_t side, float (&jacobian)[6][8]);
float Determinant(const float *matrix, std::size_t order);
bool InvertSix(float (&matrix)[6][6]);
float SolveWeighted(const float (&jacobian)[6][8], const float (&weights)[8], const float (&error)[6], float (&delta)[8]);

} // namespace

void SearchIK_SetTarget(const Matrix4x4 *transform);
bool BeginSearchHand(Character *character, std::int32_t handSelection, std::uint32_t jointTable);
bool SetSearchHandOverrideEnabled(bool enabled);
bool mtwkApplyHand(HandIKMode mode, std::uint8_t side);
bool srchSetTargetsWithParams(std::int32_t side, const Matrix4x4 *transform, const Vector3 *tangentOrigin, const Vector3 *pathOrigin, std::uint8_t ticks);
bool get_x_from_target(Vector3 *targetOffset, std::uint8_t side);
bool srchSetXformFor(const Vector3 *targetOffset, std::uint8_t side);
bool srchSetSearchJointAngles(std::uint8_t side, std::byte *beginCommand, std::byte *endCommand, std::int32_t fastTiming);
bool SetSearchHandRotatingTarget(
    std::uint8_t side, const Matrix4x4 *transform, const Matrix4x4 *rotationTransform, std::uint8_t axis, std::uint16_t angle, std::uint8_t ticks);
std::uint8_t srchSetJointTable(std::uint8_t table, std::uint8_t side);
bool srchSetNewTicksTimeFor(std::uint8_t ticks, std::uint8_t side);
std::int32_t srchGetTicksDifference(std::uint8_t side);
std::int32_t srchGetCurrentTotalTicks(std::uint8_t side);
Matrix4x4 *mtwkGetSearchArmTransform(std::uint8_t side);
void GetSearchHandJointRotation(std::uint8_t side, Vector3i *rotation);
bool SetSearchHandCurrentTransform(const Matrix4x4 *transform, std::uint8_t side);
void SetSearchHandChestTransform(const Vector3 *position, const Vector3i *rotation);
void ResetSearchHandState();
bool EvaluateSearchHandArm(MotionWork &work, std::uint8_t side, std::uint32_t interpolationMode);
void MotionCommonCallback(std::int64_t mode, std::int32_t interpolationMode);
HandParams *GetCharacterHand(Character *character, std::int32_t side);
std::uint64_t ParseHandPoseCommand(HandParams *hand, std::byte *command, std::int16_t ticks);
std::uint64_t ApplyCharacterHandPoseCommand(Character *character, std::int32_t side, std::byte *command, std::int16_t ticks);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool g_searchHandPairedTarget;

Matrix4x4 g_searchHandPairTransform;

static Vector3 g_searchHandDefaultTargetOffset = {0.09f, 0, 0};

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 HandMatrixRow(const Matrix4x4 &matrix, std::int32_t row) {
    Vector3 result;
    switch (row) {
    case 0:
        result.x = matrix.rows[0].x;
        result.y = matrix.rows[0].y;
        result.z = matrix.rows[0].z;
        break;
    case 1:
        result.x = matrix.rows[1].x;
        result.y = matrix.rows[1].y;
        result.z = matrix.rows[1].z;
        break;
    case 2:
        result.x = matrix.rows[2].x;
        result.y = matrix.rows[2].y;
        result.z = matrix.rows[2].z;
        break;
    }
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
float HandPathLength(const MotionRigAxis *path, float limit) {
    float step = 0.01f;
    float length = 0.0f;
    float time;
    for (std::int32_t sample = 0; sample < 100 && (time = static_cast<float>(sample) / 100.0f) <= limit; ++sample) {
        Vector3 velocity;
        velocity.x = EvaluateCurveSlopeCubic(&path[0].channel.current, &path[0].channel.next, time);
        velocity.y = EvaluateCurveSlopeCubic(&path[1].channel.current, &path[1].channel.next, time);
        velocity.z = EvaluateCurveSlopeCubic(&path[2].channel.current, &path[2].channel.next, time);
        length += Vector3::CalcMagnitude(velocity) * step;
    }
    return length;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool mtwkApplyHand(std::uint8_t mode, std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return false;
    switch (mode) {
    case HandIK_Disabled:
    case HandIK_HoldTransform:
    case HandIK_DeltaVector:
        g_searchHandMotionWork->lowerBodyClock.playbackSpeed = 1.0f;
        g_searchHandMotionWork->chestClock.playbackSpeed = 1.0f;
        g_searchHandMotionWork->headClock.playbackSpeed = 1.0f;
        g_searchHandMotionWork->rightArmClock.playbackSpeed = 1.0f;
        g_searchHandMotionWork->leftArmClock.playbackSpeed = 1.0f;
        g_searchHandConfigurations[side].mode = mode;
        break;
    case HandIK_Unblended:
        g_searchHandMotionWork->lowerBodyClock.playbackSpeed = 0.0f;
        g_searchHandMotionWork->chestClock.playbackSpeed = 0.0f;
        g_searchHandMotionWork->headClock.playbackSpeed = 0.0f;
        g_searchHandMotionWork->rightArmClock.playbackSpeed = 0.0f;
        g_searchHandMotionWork->leftArmClock.playbackSpeed = 0.0f;
        g_searchHandConfigurations[side].mode = mode;
        break;
    case HandIK_PathFollow:
        if ((side + g_searchHandConfigurations)->flags & HandIKPathActive) {
            g_searchHandConfigurations[side].pendingMode = g_searchHandConfigurations[side].mode;
            g_searchHandConfigurations[side].mode = mode;
        }
        break;
    case HandIK_HandPose:
        if (g_searchHandConfigurations[side].mode != HandIK_PathFollow)
            g_searchHandConfigurations[side].pendingMode = g_searchHandConfigurations[side].mode;
        (side + g_searchHandConfigurations)->flags &= ~HandIKPathActive;
        g_searchHandConfigurations[side].mode = mode;
        g_searchHandConfigurations[side].ticksRemaining = g_searchHandConfigurations[side].totalTicks;
        break;
    }
    return true;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool srchSetTargetsWithParams(std::int32_t side, const Matrix4x4 *transform, const Vector3 *tangentOrigin, const Vector3 *pathOrigin, std::uint8_t ticks) {
    Vector3 outgoing, incoming, start, blend, tangent, destination;
    const std::uint8_t hand = static_cast<std::uint8_t>(side);
    if (!g_searchHandMotionWork || !transform)
        return false;
    const Vector3 axisX = HandMatrixRow(*transform, 0);
    HandIKConfig *configs = g_searchHandConfigurations;
    configs[hand].destinationAxisX = axisX;
    const Vector3 axisZ = HandMatrixRow(*transform, 2);
    {
        HandIKConfig *const config = &g_searchHandConfigurations[hand];
        config->destinationAxisZ = axisZ;
    }
    destination.x = transform->rows[3].x;
    destination.y = transform->rows[3].y;
    destination.z = transform->rows[3].z;
    g_searchHandConfigurations[hand].totalTicks = g_searchHandConfigurations[hand].ticksRemaining = ticks;
    (hand + g_searchHandConfigurations)->flags |= HandIKPathActive;
    if (pathOrigin) {
        incoming = destination;
        if (tangentOrigin) {
            incoming.x -= tangentOrigin->x;
            incoming.y -= tangentOrigin->y;
            incoming.z -= tangentOrigin->z;
        } else {
            incoming.x -= pathOrigin->x;
            incoming.y -= pathOrigin->y;
            incoming.z -= pathOrigin->z;
        }
        outgoing = *pathOrigin;
        outgoing.x -= g_searchHandConfigurations[hand].currentTarget.x;
        outgoing.y -= g_searchHandConfigurations[hand].currentTarget.y;
        outgoing.z -= g_searchHandConfigurations[hand].currentTarget.z;
        start = *pathOrigin;
        blend = outgoing;
        tangent = incoming;
        (hand + g_searchHandConfigurations)->flags |= HandIKBlendToPath;
    } else {
        incoming = destination;
        if (tangentOrigin) {
            incoming.x -= tangentOrigin->x;
            incoming.y -= tangentOrigin->y;
            incoming.z -= tangentOrigin->z;
        } else {
            incoming.x -= g_searchHandConfigurations[hand].currentTarget.x;
            incoming.y -= g_searchHandConfigurations[hand].currentTarget.y;
            incoming.z -= g_searchHandConfigurations[hand].currentTarget.z;
        }
        outgoing = destination;
        outgoing.x -= g_searchHandConfigurations[hand].currentTarget.x;
        outgoing.y -= g_searchHandConfigurations[hand].currentTarget.y;
        outgoing.z -= g_searchHandConfigurations[hand].currentTarget.z;
        tangent = incoming;
        const HandIKConfig *const config = &g_searchHandConfigurations[hand];
        start = config->currentTarget;
        (hand + g_searchHandConfigurations)->flags &= ~HandIKBlendToPath;
    }
    const float tangentLength = Vector3::CalcMagnitude(tangent);
    float scale = Vector3::CalcMagnitude(outgoing);
    if (scale > 0.00000011920929f)
        scale = tangentLength * 2.5f / scale;
    else
        scale = 0.0f;
    outgoing.x *= scale;
    outgoing.y *= scale;
    outgoing.z *= scale;
    scale = Vector3::CalcMagnitude(incoming);
    if (scale > 0.00000011920929f)
        scale = tangentLength * 2.5f / scale;
    else
        scale = 0.0f;
    incoming.x *= scale;
    incoming.y *= scale;
    incoming.z *= scale;
    {
        Vector3 *const target = &g_searchHandConfigurations[hand].destination;
        *target = destination;
    }
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        g_searchHandConfigurations[hand].path[axis].channel.current.timeSeconds = 0.0f;
        g_searchHandConfigurations[hand].path[axis].channel.current.tangentIn = 0.0f;
        g_searchHandConfigurations[hand].path[axis].channel.current.tangentOut = (&outgoing.x)[axis];
        g_searchHandConfigurations[hand].path[axis].channel.current.value = (&start.x)[axis];
        g_searchHandConfigurations[hand].path[axis].channel.next.timeSeconds = 1.0f;
        g_searchHandConfigurations[hand].path[axis].channel.next.tangentIn = (&incoming.x)[axis];
        g_searchHandConfigurations[hand].path[axis].channel.next.tangentOut = 0.0f;
        g_searchHandConfigurations[hand].path[axis].channel.next.value = (&destination.x)[axis];
        g_searchHandConfigurations[hand].path[axis].channel.inverseDuration = 1.0f / g_searchHandConfigurations[hand].path[axis].channel.next.timeSeconds;
        g_searchHandConfigurations[hand].path[axis].channel.keysRemaining = 0;
        g_searchHandConfigurations[hand].path[axis].channel.frameCursor = 0;
    }
    g_searchHandConfigurations[hand].pathLength = HandPathLength(g_searchHandConfigurations[hand].path, 1.0f);
    const float pathLength = g_searchHandConfigurations[hand].pathLength;
    if (g_searchHandConfigurations[hand].flags & HandIKBlendToPath) {
        const float blendLength = Vector3::CalcMagnitude(blend);
        const float total = pathLength + blendLength;
        if (total > 0.00000011920929f) {
            g_searchHandConfigurations[hand].blendTotalTicks = static_cast<std::uint8_t>(static_cast<float>(ticks) * (blendLength / total));
            g_searchHandConfigurations[hand].blendTicksRemaining = g_searchHandConfigurations[hand].blendTotalTicks;
        } else {
            g_searchHandConfigurations[hand].blendTotalTicks = 0;
            g_searchHandConfigurations[hand].blendTicksRemaining = 0;
        }
    } else {
        g_searchHandConfigurations[hand].blendTotalTicks = 0;
        g_searchHandConfigurations[hand].blendTicksRemaining = 0;
    }
    g_searchHandConfigurations[hand].commandDelay = 0;
    g_searchHandConfigurations[hand].beginPoseCommand = 0;
    g_searchHandConfigurations[hand].endPoseCommand = 0;
    (hand + g_searchHandConfigurations)->flags &= ~HandIKFastCommandTiming;
    (hand + g_searchHandConfigurations)->flags &= ~HandIKRotatingTarget;
    return true;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchSetSearchJointAngles(std::uint8_t side, std::byte *beginCommand, std::byte *endCommand, std::int32_t fastTiming) {
    if (!g_searchHandMotionWork || !beginCommand || !g_searchHandConfigurations[side].ticksRemaining)
        return false;
    if (fastTiming) {
        g_searchHandConfigurations[side].commandDelay = 5;
        (side + g_searchHandConfigurations)->flags |= HandIKFastCommandTiming;
    } else {
        g_searchHandConfigurations[side].commandDelay = 0;
        (side + g_searchHandConfigurations)->flags &= ~HandIKFastCommandTiming;
    }
    g_searchHandConfigurations[side].beginPoseCommand = beginCommand;
    g_searchHandConfigurations[side].endPoseCommand = endCommand;
    return true;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const Matrix4x4 *mtwkGetSearchArmTransform(std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return 0;
    return mtwkGetBoneTransform(g_searchHandMotionWork, side ? 21 : 14);
}

#else

Matrix4x4 *mtwkGetSearchArmTransform(std::uint8_t side) {
    return handWork && side < 2 ? &Arm(*handWork, side).handWorld : nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool SetSearchHandRotatingTarget(
    std::uint8_t side, const Matrix4x4 *transform, const Matrix4x4 *rotationTransform, std::uint8_t axis, std::int32_t angle, std::uint8_t ticks) {
    if (!g_searchHandMotionWork || !transform || !rotationTransform)
        return false;
    const Vector3 axisX = HandMatrixRow(*transform, 0);
    HandIKConfig *configs = g_searchHandConfigurations;
    configs[side].destinationAxisX = axisX;
    const Vector3 axisZ = HandMatrixRow(*transform, 2);
    configs[side].destinationAxisZ = axisZ;
    configs[side].totalTicks = configs[side].ticksRemaining = ticks;
    configs[side].rotationAngle = angle;
    configs[side].rotationAxis = axis;
    configs[side].rotationTransform = *rotationTransform;
    configs[side].flags |= HandIKPathActive;
    matrix_stack::Matrix4x4_Push(mtwkGetSearchArmTransform(side));
    Matrix4x4::preTranslate(g_searchHandConfigurations[side].localTargetOffset.x, g_searchHandConfigurations[side].localTargetOffset.y,
        g_searchHandConfigurations[side].localTargetOffset.z);
    matrix_stack::GetTranslation(&g_searchHandConfigurations[side].rotationOrigin);
    matrix_stack::LoadMatrix(&g_searchHandConfigurations[side].rotationTransform);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::TransformPoint(g_searchHandConfigurations[side].rotationOrigin, g_searchHandConfigurations[side].rotationOrigin);
    PopCurrentMatrix(1);
    configs[side].commandDelay = 0;
    g_searchHandConfigurations[side].beginPoseCommand = 0;
    g_searchHandConfigurations[side].endPoseCommand = 0;
    configs[side].flags &= ~HandIKFastCommandTiming;
    configs[side].flags &= ~HandIKBlendToPath;
    configs[side].flags |= HandIKRotatingTarget;
    return true;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float srchSetPathBiasFor(std::uint8_t side, float bias) {
    if (!g_searchHandMotionWork)
        return -1.0f;
    const float previous = g_searchHandConfigurations[side].pathBias;
    g_searchHandConfigurations[side].pathBias = bias;
    return previous;
}

float srchGetPathBiasFor(std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return -1.0f;
    return g_searchHandConfigurations[side].pathBias;
}

// @verified - d
std::uint8_t srchSetJointTable(std::uint8_t table, std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return 4;
    const std::uint8_t previous = g_searchHandConfigurations[side].jointTable;
    if (table < 4)
        g_searchHandConfigurations[side].jointTable = table;
    return previous;
}

std::uint8_t srchGetJointTable(std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return 4;
    return g_searchHandConfigurations[side].jointTable;
}

#else

std::uint8_t srchSetJointTable(std::uint8_t table, std::uint8_t side) {
    if (!handWork || side >= 2)
        return 4;
    const auto old = configurations[side].jointTable;
    if (table < 4)
        configurations[side].jointTable = table;
    return old;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchSetXformFor(const Vector3 *targetOffset, std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return false;
    if (!targetOffset) {
        Vector3 *const offset = &g_searchHandConfigurations[side].localTargetOffset;
        *offset = g_searchHandDefaultTargetOffset;
    } else {
        Vector3 *const offset = &g_searchHandConfigurations[side].localTargetOffset;
        *offset = *targetOffset;
    }
    return true;
}

#else

bool srchSetXformFor(const Vector3 *targetOffset, std::uint8_t side) {
    if (!handWork || side >= 2)
        return false;
    configurations[side].localTargetOffset = targetOffset ? *targetOffset : defaultOffset;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool get_x_from_target(Vector3 *targetOffset, std::uint8_t side) {
    if (!g_searchHandMotionWork || !targetOffset)
        return false;
    const Vector3 *const offset = &g_searchHandConfigurations[side].localTargetOffset;
    *targetOffset = *offset;
    return true;
}

#else

bool get_x_from_target(Vector3 *targetOffset, std::uint8_t side) {
    if (!handWork || !targetOffset || side >= 2)
        return false;
    *targetOffset = configurations[side].localTargetOffset;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool SetSearchHandCurrentTransform(const Matrix4x4 *transform, std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return false;
    HandIKConfig *configs = g_searchHandConfigurations;
    g_searchHandConfigurations[side].currentTarget.x = transform->rows[3].x;
    g_searchHandConfigurations[side].currentTarget.y = transform->rows[3].y;
    g_searchHandConfigurations[side].currentTarget.z = transform->rows[3].z;
    const Vector3 axisX = HandMatrixRow(*transform, 0);
    configs[side].currentAxisX = axisX;
    const Vector3 axisZ = HandMatrixRow(*transform, 2);
    configs[side].currentAxisZ = axisZ;
    return true;
}

#else

bool SetSearchHandCurrentTransform(const Matrix4x4 *transform, std::uint8_t side) {
    if (!handWork || side >= 2 || !transform)
        return false;
    auto &config = configurations[side];
    config.currentTarget = Row(*transform, 3);
    config.currentAxisX = Row(*transform, 0);
    config.currentAxisZ = Row(*transform, 2);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool srchSetNewTicksTimeFor(std::int32_t ticks, std::uint8_t side) {
    if (!g_searchHandMotionWork)
        return false;
    if (g_searchHandConfigurations[side].mode != HandIK_PathFollow && g_searchHandConfigurations[side].mode != HandIK_HandPose)
        g_searchHandConfigurations[side].pendingMode = g_searchHandConfigurations[side].mode;
    (side + g_searchHandConfigurations)->flags &= ~HandIKPathActive;
    g_searchHandConfigurations[side].mode = HandIK_HandPose;
    g_searchHandConfigurations[side].ticksRemaining = ticks;
    g_searchHandConfigurations[side].totalTicks = ticks;
    return true;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t srchGetTicksDifference(std::uint8_t side) {
    if (g_searchHandMotionWork) {
        if (g_searchHandConfigurations[side].mode == HandIK_PathFollow || g_searchHandConfigurations[side].mode == HandIK_HandPose)
            return g_searchHandConfigurations[side].totalTicks - g_searchHandConfigurations[side].ticksRemaining;
    }
    return 0;
}

#else

std::int32_t srchGetTicksDifference(std::uint8_t side) {
    if (!handWork || side >= 2)
        return 0;
    const auto &config = configurations[side];
    return config.mode == HandIKMode::PathFollow || config.mode == HandIKMode::HandPose ? config.totalTicks - config.ticksRemaining : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t srchGetCurrentTotalTicks(std::uint8_t side) {
    if (g_searchHandMotionWork) {
        if (g_searchHandConfigurations[side].mode == HandIK_PathFollow || g_searchHandConfigurations[side].mode == HandIK_HandPose)
            return g_searchHandConfigurations[side].totalTicks;
    }
    return 0;
}

#else

std::int32_t srchGetCurrentTotalTicks(std::uint8_t side) {
    if (!handWork || side >= 2)
        return 0;
    const auto &config = configurations[side];
    return config.mode == HandIKMode::PathFollow || config.mode == HandIKMode::HandPose ? config.totalTicks : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool SearchIK_SetTarget(const Matrix4x4 *transform) {
    if (g_searchHandMotionWork && transform && (g_searchHandConfigurations[0].flags & HandIKEnabled) &&
        ((g_searchHandConfigurations + 1)->flags & HandIKEnabled)) {
        g_searchHandPairTransform = *transform;
        g_searchHandPairedTarget = true;
        return true;
    }
    return false;
}

std::int8_t srchGetSolveStatus() {
    if (!g_searchHandMotionWork)
        return -1;
    return static_cast<std::int8_t>(g_searchHandSolveStatus);
}

#else

void SearchIK_SetTarget(const Matrix4x4 *transform) {
    if (handWork && transform && (configurations[0].flags & HandIKEnabled) && (configurations[1].flags & HandIKEnabled)) {
        pairedTarget = true;
        pairTransform = *transform;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetSearchHandOverrideEnabled(std::uint8_t enabled) {
    if (g_searchHandMotionWork) {
        if (enabled)
            g_searchHandOverrideEnabled = 1;
        else
            g_searchHandOverrideEnabled = 0;
    }
}

#else

bool SetSearchHandOverrideEnabled(bool enabled) {
    if (!handWork)
        return false;
    overrideEnabled = enabled;
    return enabled;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool IsSearchHandOverrideEnabled() {
    if (!g_searchHandMotionWork)
        return false;
    if (g_searchHandOverrideEnabled)
        return true;
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void GetSearchHandJointRotation(std::uint8_t side, Vector3i *rotation) {
    if (!g_searchHandMotionWork)
        return;
    const float fixedRadians = 10430.3779296875f;
    rotation->x = static_cast<std::int32_t>(g_searchHandConfigurations[side].jointAngles[5] * fixedRadians) & 0xffff;
    rotation->y = static_cast<std::int32_t>(g_searchHandConfigurations[side].jointAngles[7] * fixedRadians) & 0xffff;
    rotation->z = static_cast<std::int32_t>(g_searchHandConfigurations[side].jointAngles[6] * fixedRadians) & 0xffff;
}

#else

void GetSearchHandJointRotation(std::uint8_t side, Vector3i *rotation) {
    if (!handWork || side >= 2 || !rotation)
        return;
    const auto &angles = configurations[side].jointAngles;
    *rotation = {Angle(angles[5]), Angle(angles[7]), Angle(angles[6])};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void SetSearchHandJointRotation(std::uint8_t side, const Vector3i *rotation) {
    if (!g_searchHandMotionWork)
        return;
    const float fixedRadians = 10430.3779296875f;
    g_searchHandConfigurations[side].jointAngles[5] = rotation->x / fixedRadians;
    g_searchHandConfigurations[side].jointAngles[7] = rotation->y / fixedRadians;
    g_searchHandConfigurations[side].jointAngles[6] = rotation->z / fixedRadians;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint8_t searchHandSide;

Matrix4x4 searchHandChestReference, searchHandChestPrevious;

Vector3 searchHandChestPosition;

Vector3i searchHandChestRotation;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

MotionArmRig &SearchArm(std::uint8_t side) {
    IKRig &rig = reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig;
    return side ? rig.leftArm : rig.rightArm;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

const float epsilon = 0.00000011920929f;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float HandClamp(float value, float low, float high) {
    return low > value ? low : value > high ? high : value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void HandSwap(float &a, float &b) {
    const float saved = a;
    a = b;
    b = saved;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector4 HandRow(const Vector3 &v) {
    Vector4 row = {v.x, v.y, v.z, 1};
    return row;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 Row(const Matrix4x4 &m, std::int32_t row) {
    return HandMatrixRow(m, row);
}

} // namespace

#else

namespace {

Vector3 Row(const Matrix4x4 &matrix, std::size_t row) {
    const auto &value = matrix.rows[row];
    return {value.x, value.y, value.z};
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Matrix4x4 Identity() {
    return {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Matrix4x4 Multiply(const Matrix4x4 &a, const Matrix4x4 &b) {
    matrix_stack::Matrix4x4_Push(&b);
    Matrix4x4::MultiplyWithPrePassMatrix(&a);
    Matrix4x4 result;
    matrix_stack::GetMatrix(&result);
    PopCurrentMatrix(1);
    return result;
}

} // namespace

#else

namespace {

Matrix4x4 Multiply(const Matrix4x4 &a, const Matrix4x4 &b) {
    Matrix4x4 result{};
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t col = 0; col < 4; ++col)
            for (std::size_t k = 0; k < 4; ++k)
                result.rows[row][col] += a.rows[row][k] * b.rows[k][col];
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 TransformVector(const Matrix4x4 &m, const Vector3 &v) {
    matrix_stack::Matrix4x4_Push(&m);
    Vector3 result;
    Matrix4x4::Transform(v, result);
    PopCurrentMatrix(1);
    return result;
}

} // namespace

#else

namespace {

Vector3 TransformVector(const Matrix4x4 &matrix, Vector3 value) {
    return Vector3::Add(
        Vector3::Add(Vector3::Scale(Row(matrix, 0), value.x), Vector3::Scale(Row(matrix, 1), value.y)), Vector3::Scale(Row(matrix, 2), value.z));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 TransformPoint(const Matrix4x4 &m, const Vector3 &v) {
    matrix_stack::Matrix4x4_Push(&m);
    Vector3 result;
    Matrix4x4::TransformPoint(v, result);
    PopCurrentMatrix(1);
    return result;
}

} // namespace

#else

namespace {

Vector3 TransformPoint(const Matrix4x4 &matrix, Vector3 value) {
    return Vector3::Add(TransformVector(matrix, value), Row(matrix, 3));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void Translate(Matrix4x4 &m, const Vector3 &v) {
    matrix_stack::Matrix4x4_Push(&m);
    matrix_stack::TranslateVector(&v);
    matrix_stack::GetMatrix(&m);
    PopCurrentMatrix(1);
}

} // namespace

#else

namespace {

void Translate(Matrix4x4 &matrix, Vector3 value) {
    const auto p = TransformPoint(matrix, value);
    matrix.rows[3] = {p.x, p.y, p.z, 1};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Matrix4x4 Basis(Vector3 x, Vector3 z, Vector3 position) {
    const Vector3 y = Vector3::Cross(z, x);
    Matrix4x4 result = {{{x.x, x.y, x.z, 0}, {y.x, y.y, y.z, 0}, {z.x, z.y, z.z, 0}, {position.x, position.y, position.z, 1}}};
    return result;
}

} // namespace

#else

namespace {

Matrix4x4 Basis(Vector3 x, Vector3 z, Vector3 position) {
    const auto y = Vector3::Cross(z, x);
    return {{{x.x, x.y, x.z, 0}, {y.x, y.y, y.z, 0}, {z.x, z.y, z.z, 0}, {position.x, position.y, position.z, 1}}};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t Angle(float radians) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>((radians * 57.29578f) * 182.04443f));
}

} // namespace

#else

namespace {

std::uint16_t Angle(float radians) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>((radians * 57.29578f) * 182.04443f));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void Rotate(std::uint8_t axis, std::uint16_t angle) {
    if (axis == 0)
        Matrix4x4::RotateAroundX_Wrapper(angle);
    else if (axis == 1)
        Matrix4x4::RotateAroundY_Wrapper(angle);
    else if (axis == 2)
        Matrix4x4::RotateAroundZ_Wrapper(angle);
}

} // namespace

#else

namespace {

void Rotate(std::uint8_t axis, std::uint16_t angle) {
    if (axis == 0)
        Matrix4x4::RotateAroundX_Wrapper(angle);
    else if (axis == 1)
        Matrix4x4::RotateAroundY_Wrapper(angle);
    else if (axis == 2)
        Matrix4x4::RotateAroundZ_Wrapper(angle);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

MotionArmRig &Arm(MotionWork &work, std::uint8_t side) {
    return side ? work.rig.leftArm : work.rig.rightArm;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetBlend(float blend) {
    handWork->rig.lowerBody.clock.playbackSpeed = blend;
    handWork->rig.chest.clock.playbackSpeed = blend;
    handWork->rig.head.clock.playbackSpeed = blend;
    handWork->rig.rightArm.clock.playbackSpeed = blend;
    handWork->rig.leftArm.clock.playbackSpeed = blend;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void CaptureTarget(HandIKConfig &config, const Matrix4x4 &transform) {
    config.currentTarget = TransformPoint(transform, config.localTargetOffset);
    config.currentAxisX = Row(transform, 0);
    config.currentAxisZ = Row(transform, 2);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float Ease(float time, float bias) {
    if (time < 0.0f)
        time = 0.0f;
    else if (time > 1.0f)
        time = 1.0f;
    return time < 0.5f ? (1.0f - bias) * 2.0f * time * time + bias * time : (1.0f - bias) * -2.0f * time * time + (4.0f - bias * 3.0f) * time + (bias - 1.0f);
}

} // namespace

#else

namespace {

float Ease(float time, float bias) {
    time = std::clamp(time, 0.0f, 1.0f);
    return time < 0.5f ? ((1.0f - bias) * 2.0f * time) * time + time * bias : (4.0f - 3.0f * bias) * time + ((1.0f - bias) * -2.0f * time) * time + bias - 1.0f;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float Derivative(const MotionRigChannel &channel, float time) {
    const MotionRigKey &a = channel.current, &b = channel.next;
    time -= a.timeSeconds;
    const float inverse = 1.0f / (b.timeSeconds - a.timeSeconds), inverseSquared = inverse * inverse;
    const float squared = time * time;
    const float three = (squared * 3.0f) * inverseSquared;
    const float six = ((squared * 6.0f) * inverseSquared) * inverse;
    const float linear = (time * 6.0f) * inverseSquared;
    const float twice = (time + time) * inverse;
    float value = b.value * (-six + linear);
    value += a.value * (six - linear);
    const float incoming = three - twice;
    value += a.tangentOut * ((incoming - twice) + 1.0f);
    value += b.tangentIn * incoming;
    return value;
}

} // namespace

#else

namespace {

float Derivative(const MotionRigChannel &channel, float time) {
    const auto &a = channel.current;
    const auto &b = channel.next;
    return (6 * time * time - 6 * time) * a.value + (3 * time * time - 4 * time + 1) * a.tangentOut + (-6 * time * time + 6 * time) * b.value +
        (3 * time * time - 2 * time) * b.tangentIn;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float PathSpeed(const HandIKConfig &config, float time) {
    const Vector3 velocity = {Derivative(config.path[0].channel, time), Derivative(config.path[1].channel, time), Derivative(config.path[2].channel, time)};
    return Vector3::CalcMagnitude(velocity);
}

} // namespace

#else

namespace {

float PathSpeed(const HandIKConfig &config, float time) {
    return Vector3::CalcMagnitude(
        {Derivative(config.path[0].channel, time), Derivative(config.path[1].channel, time), Derivative(config.path[2].channel, time)});
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float PathLength(const HandIKConfig &config) {
    float result = 0;
    for (int i = 0; i < 100; ++i)
        result += PathSpeed(config, static_cast<float>(i) / 100.0f) * 0.01f;
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float FindSearchHandPathTime(const MotionRigAxis *path, float target) {
    float sampleInterval = 0.01f;
    float length = 0, time = 0;
    for (int i = 0; i <= 100; ++i) {
        time = static_cast<float>(i) / 100.0f;
        Vector3 velocity;
        velocity.x = EvaluateCurveSlopeCubic(&path[0].channel.current, &path[0].channel.next, time);
        velocity.y = EvaluateCurveSlopeCubic(&path[1].channel.current, &path[1].channel.next, time);
        velocity.z = EvaluateCurveSlopeCubic(&path[2].channel.current, &path[2].channel.next, time);
        const float step = Vector3::CalcMagnitude(velocity) * sampleInterval;
        length += step;
        if (length > target) {
            if (step > epsilon)
                time += (target - (length - step)) * sampleInterval / step;
            break;
        }
    }
    return time;
}

void SamplePath(Vector3 *result, float fraction, std::uint8_t side) {
    const float time = FindSearchHandPathTime(g_searchHandConfigurations[side].path, g_searchHandConfigurations[side].pathLength * fraction);
    result->x =
        EvaluateCurveSegmentCubic(&g_searchHandConfigurations[side].path[0].channel.current, &g_searchHandConfigurations[side].path[0].channel.next, time);
    result->y =
        EvaluateCurveSegmentCubic(&g_searchHandConfigurations[side].path[1].channel.current, &g_searchHandConfigurations[side].path[1].channel.next, time);
    result->z =
        EvaluateCurveSegmentCubic(&g_searchHandConfigurations[side].path[2].channel.current, &g_searchHandConfigurations[side].path[2].channel.next, time);
}

void LerpSearchHandPath(Vector3 *result, float blend, std::uint8_t side) {
    const float inverse = 1.0f - blend;
    result->x = inverse * g_searchHandConfigurations[side].currentTarget.x + blend * g_searchHandConfigurations[side].path[0].channel.current.value;
    result->y = inverse * g_searchHandConfigurations[side].currentTarget.y + blend * g_searchHandConfigurations[side].path[1].channel.current.value;
    result->z = inverse * g_searchHandConfigurations[side].currentTarget.z + blend * g_searchHandConfigurations[side].path[2].channel.current.value;
}

void RotateTransformSearchHandPoint(Vector3 *result, float time, std::uint8_t side) {
    matrix_stack::Matrix4x4_Push(&g_searchHandConfigurations[side].rotationTransform);
    switch (g_searchHandConfigurations[side].rotationAxis) {
    case 0:
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(static_cast<std::int16_t>(g_searchHandConfigurations[side].rotationAngle) * time));
        break;
    case 1:
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(static_cast<std::int16_t>(g_searchHandConfigurations[side].rotationAngle) * time));
        break;
    case 2:
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(static_cast<std::int16_t>(g_searchHandConfigurations[side].rotationAngle) * time));
        break;
    }
    Matrix4x4::TransformPoint(g_searchHandConfigurations[side].rotationOrigin, *result);
    PopCurrentMatrix(1);
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Vector3 CrossMotionAxes(const Vector3 &a, const Vector3 &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void SampleBasis(Matrix4x4 *result, float time, std::uint8_t side) {
    const float oldTime = g_motionCurrentAnimTime, oldMaximum = g_motionMaximumAnimTime;
    g_motionMaximumAnimTime = 1.0f;
    g_motionCurrentAnimTime = time;
    MotionRotationBasis basis;
    Vector3 targetAxisY, sourceAxisY;
    am2Vector3Cross(&g_searchHandConfigurations[side].currentAxisZ, &g_searchHandConfigurations[side].currentAxisX, &sourceAxisY);
    am2Vector3Cross(&g_searchHandConfigurations[side].destinationAxisZ, &g_searchHandConfigurations[side].destinationAxisX, &targetAxisY);
    basis.sourceAxis.x = g_searchHandConfigurations[side].currentAxisX.x;
    basis.sourceAxis.y = sourceAxisY.x;
    basis.sourceAxis.z = g_searchHandConfigurations[side].currentAxisZ.x;
    basis.sourceTangent.x = g_searchHandConfigurations[side].currentAxisX.y;
    basis.sourceTangent.y = sourceAxisY.y;
    basis.sourceTangent.z = g_searchHandConfigurations[side].currentAxisZ.y;
    basis.targetAxis.x = g_searchHandConfigurations[side].destinationAxisX.x;
    basis.targetAxis.y = targetAxisY.x;
    basis.targetAxis.z = g_searchHandConfigurations[side].destinationAxisZ.x;
    basis.targetTangent.x = g_searchHandConfigurations[side].destinationAxisX.y;
    basis.targetTangent.y = targetAxisY.y;
    basis.targetTangent.z = g_searchHandConfigurations[side].destinationAxisZ.y;
    matrix_stack::PushUnitMatrix();
    AxisAngleToBasisSlerp(basis);
    matrix_stack::GetMatrix(result);
    PopCurrentMatrix(1);
    g_motionMaximumAnimTime = oldMaximum;
    g_motionCurrentAnimTime = oldTime;
}

} // namespace

#else

namespace {

Matrix4x4 SampleBasis(const HandIKConfig &config, float time) {
    MotionRotationBasis basis{config.destinationAxisX, Vector3::Cross(config.destinationAxisZ, config.destinationAxisX), config.currentAxisX,
        Vector3::Cross(config.currentAxisZ, config.currentAxisX)};
    Matrix4x4 result = Identity();
    ApplyMotionRotationBasis(result, basis, time);
    return result;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SubmitBeginPose(HandIKConfig &config, std::uint8_t side) {
    if (!config.beginPoseCommand)
        return;
    const std::uint16_t ticks =
        static_cast<std::uint16_t>((config.flags & HandIKFastCommandTiming) ? (config.ticksRemaining + config.commandDelay) / 4U : config.ticksRemaining);
    ApplyCharacterHandPoseCommand(handCharacter, side, config.beginPoseCommand, static_cast<std::int16_t>(ticks));
    config.beginPoseCommand = nullptr;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SubmitEndPose(HandIKConfig &config, std::uint8_t side) {
    const unsigned threshold = (config.totalTicks >> 2U) * ((config.flags & HandIKFastCommandTiming) ? 3U : 1U);
    if (config.endPoseCommand && config.ticksRemaining < threshold) {
        ApplyCharacterHandPoseCommand(handCharacter, side, config.endPoseCommand, config.ticksRemaining);
        config.endPoseCommand = nullptr;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline HandIKConfig *SearchHandConfig(std::uint8_t side) {
    return &g_searchHandConfigurations[side];
}

// AdvancePath: command-delay ticks do not advance the path or its clock.
bool AdvancePath(Vector3 *position, Matrix4x4 *target, std::uint8_t side) {
    if (!position || !target || !(SearchHandConfig(side)->flags & HandIKPathActive))
        return false;
    if (g_searchHandConfigurations[side].beginPoseCommand) {
        CharacterHandComponent *hand = GetCharacterHand(g_searchHandCharacter, side);
        std::uint16_t ticks = g_searchHandConfigurations[side].ticksRemaining;
        if (g_searchHandConfigurations[side].flags & HandIKFastCommandTiming)
            ticks = static_cast<unsigned>(ticks + g_searchHandConfigurations[side].commandDelay) >> 2;
        if (hand)
            ParseHandPoseCommand(*hand, g_searchHandConfigurations[side].beginPoseCommand, ticks);
        g_searchHandConfigurations[side].beginPoseCommand = 0;
    }
    if (g_searchHandConfigurations[side].commandDelay) {
        --g_searchHandConfigurations[side].commandDelay;
        if (g_searchHandConfigurations[side].blendTicksRemaining)
            LerpSearchHandPath(position, 0, side);
        else
            SamplePath(position, 0, side);
        SampleBasis(target, 0, side);
        return true;
    }
    if (g_searchHandConfigurations[side].endPoseCommand) {
        int threshold = g_searchHandConfigurations[side].totalTicks >> 2;
        if (g_searchHandConfigurations[side].flags & HandIKFastCommandTiming)
            threshold *= 3;
        if (g_searchHandConfigurations[side].ticksRemaining < threshold) {
            CharacterHandComponent *hand = GetCharacterHand(g_searchHandCharacter, side);
            if (hand)
                ParseHandPoseCommand(*hand, g_searchHandConfigurations[side].endPoseCommand, g_searchHandConfigurations[side].ticksRemaining);
            g_searchHandConfigurations[side].endPoseCommand = 0;
        }
    }
    SampleBasis(target,
        Ease(static_cast<float>(g_searchHandConfigurations[side].totalTicks - g_searchHandConfigurations[side].ticksRemaining) /
                g_searchHandConfigurations[side].totalTicks,
            g_searchHandConfigurations[side].pathBias),
        side);
    if (g_searchHandConfigurations[side].flags & HandIKRotatingTarget) {
        RotateTransformSearchHandPoint(position,
            Ease(static_cast<float>(g_searchHandConfigurations[side].totalTicks - g_searchHandConfigurations[side].ticksRemaining) /
                    g_searchHandConfigurations[side].totalTicks,
                g_searchHandConfigurations[side].pathBias),
            side);
        Vector3 axisX = HandMatrixRow(*target, 0);
        SearchHandConfig(side)->destinationAxisX = axisX;
        Vector3 axisZ = HandMatrixRow(*target, 2);
        SearchHandConfig(side)->destinationAxisZ = axisZ;
        SearchHandConfig(side)->destination = *position;
    } else if (g_searchHandConfigurations[side].blendTicksRemaining) {
        LerpSearchHandPath(position,
            Ease(static_cast<float>(g_searchHandConfigurations[side].blendTotalTicks - g_searchHandConfigurations[side].blendTicksRemaining) /
                    g_searchHandConfigurations[side].blendTotalTicks,
                g_searchHandConfigurations[side].pathBias),
            side);
        --g_searchHandConfigurations[side].blendTicksRemaining;
        --g_searchHandConfigurations[side].ticksRemaining;
        return true;
    } else {
        SamplePath(position,
            Ease(static_cast<float>(g_searchHandConfigurations[side].totalTicks - g_searchHandConfigurations[side].blendTotalTicks -
                     g_searchHandConfigurations[side].ticksRemaining) /
                    static_cast<float>(g_searchHandConfigurations[side].totalTicks - g_searchHandConfigurations[side].blendTotalTicks),
                g_searchHandConfigurations[side].pathBias),
            side);
    }
    if (g_searchHandConfigurations[side].ticksRemaining) {
        --g_searchHandConfigurations[side].ticksRemaining;
        return true;
    }
    return false;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void TickSearchHandPose() {
    Matrix4x4 matrix;
    if (!g_searchHandConfigurations[searchHandSide].ticksRemaining) {
        matrix_stack::Matrix4x4_Push(0);
        if (searchHandSide)
            matrix_stack::LoadMatrix(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld);
        else
            matrix_stack::LoadMatrix(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld);
        Matrix4x4::preTranslate(g_searchHandConfigurations[searchHandSide].localTargetOffset.x, g_searchHandConfigurations[searchHandSide].localTargetOffset.y,
            g_searchHandConfigurations[searchHandSide].localTargetOffset.z);
        matrix_stack::GetMatrix(&matrix);
        PopCurrentMatrix(1);
        const Vector3 axisX = HandMatrixRow(matrix, 0);
        HandIKConfig *const configX = &g_searchHandConfigurations[searchHandSide];
        configX->currentAxisX = axisX;
        const Vector3 axisZ = HandMatrixRow(matrix, 2);
        HandIKConfig *const configZ = &g_searchHandConfigurations[searchHandSide];
        configZ->currentAxisZ = axisZ;
        g_searchHandConfigurations[searchHandSide].currentTarget.x = matrix.rows[3].x;
        g_searchHandConfigurations[searchHandSide].currentTarget.y = matrix.rows[3].y;
        g_searchHandConfigurations[searchHandSide].currentTarget.z = matrix.rows[3].z;
        g_searchHandConfigurations[searchHandSide].mode = g_searchHandConfigurations[searchHandSide].pendingMode;
        return;
    }
    if (g_searchHandConfigurations[searchHandSide].beginPoseCommand) {
        CharacterHandComponent *hand = GetCharacterHand(g_searchHandCharacter, searchHandSide);
        int ticks = g_searchHandConfigurations[searchHandSide].ticksRemaining;
        if (g_searchHandConfigurations[searchHandSide].flags & HandIKFastCommandTiming)
            ticks = (ticks + g_searchHandConfigurations[searchHandSide].commandDelay) / 4;
        if (hand)
            ParseHandPoseCommand(*hand, g_searchHandConfigurations[searchHandSide].beginPoseCommand, ticks);
        g_searchHandConfigurations[searchHandSide].beginPoseCommand = 0;
    }
    if (g_searchHandConfigurations[searchHandSide].commandDelay) {
        --g_searchHandConfigurations[searchHandSide].commandDelay;
        return;
    }
    if (g_searchHandConfigurations[searchHandSide].endPoseCommand) {
        int threshold = g_searchHandConfigurations[searchHandSide].totalTicks >> 2;
        if (g_searchHandConfigurations[searchHandSide].flags & HandIKFastCommandTiming)
            threshold *= 3;
        if (g_searchHandConfigurations[searchHandSide].ticksRemaining < threshold) {
            CharacterHandComponent *hand = GetCharacterHand(g_searchHandCharacter, searchHandSide);
            if (hand)
                ParseHandPoseCommand(
                    *hand, g_searchHandConfigurations[searchHandSide].endPoseCommand, g_searchHandConfigurations[searchHandSide].ticksRemaining);
            g_searchHandConfigurations[searchHandSide].endPoseCommand = 0;
        }
    }
    float weight = static_cast<float>(g_searchHandConfigurations[searchHandSide].ticksRemaining - 1);
    weight /= static_cast<float>(g_searchHandConfigurations[searchHandSide].ticksRemaining);
    for (int joint = 0; joint < 8; ++joint) {
        const float delta = g_searchHandConfigurations[searchHandSide].jointAngles[joint] -
            searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][joint];
        g_searchHandConfigurations[searchHandSide].jointAngles[joint] =
            delta * weight + searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][joint];
    }
    --g_searchHandConfigurations[searchHandSide].ticksRemaining;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void BuildError(float (&error)[6], const Matrix4x4 &current, const Matrix4x4 &target) {
    error[0] = target.rows[3].x - current.rows[3].x;
    error[1] = target.rows[3].y - current.rows[3].y;
    error[2] = target.rows[3].z - current.rows[3].z;
    Vector3 currentAxisX = HandMatrixRow(current, 0);
    Vector3 targetAxisX = HandMatrixRow(target, 0);
    Vector3 axis;
    am2Vector3Cross(&currentAxisX, &targetAxisX, &axis);
    float magnitude = Vector3::CalcMagnitude(axis);
    Matrix4x4 rotated;
    Vector3 currentAxisZ = magnitude > epsilon ?
        (matrix_stack::PushUnitMatrix(),
            Matrix4x4_ApplyAngleRotationToVector(axis, static_cast<std::uint16_t>(static_cast<std::int32_t>(magnitude * 10430.3779296875f))),
            Matrix4x4::MultiplyWithPrePassMatrix(&current), matrix_stack::GetMatrix(&rotated), PopCurrentMatrix(1), HandMatrixRow(rotated, 2)) :
        HandMatrixRow(current, 2);
    Vector3 targetAxisZ = HandMatrixRow(target, 2);
    Vector3 axisZ;
    am2Vector3Cross(&currentAxisZ, &targetAxisZ, &axisZ);
    error[3] = axis.x + axisZ.x;
    error[4] = axis.y + axisZ.y;
    error[5] = axis.z + axisZ.z;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void BuildTargetError(float (&error)[6]) {
    Matrix4x4 current;
    Matrix4x4 target;
    Matrix4x4 *matrix;
    Vector3 tangent;
    Vector3 position;
    if (g_searchHandConfigurations[searchHandSide].mode) {
        matrix_stack::Matrix4x4_Push(0);
        if (searchHandSide)
            matrix_stack::LoadMatrix(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld);
        else
            matrix_stack::LoadMatrix(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld);
        Matrix4x4::preTranslate(g_searchHandConfigurations[searchHandSide].localTargetOffset.x, g_searchHandConfigurations[searchHandSide].localTargetOffset.y,
            g_searchHandConfigurations[searchHandSide].localTargetOffset.z);
        matrix_stack::GetMatrix(&current);
        PopCurrentMatrix(1);
        if (g_searchHandPairedTarget && searchHandSide == 1) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld);
            Matrix4x4::preTranslate(g_searchHandConfigurations[0].localTargetOffset.x, g_searchHandConfigurations[0].localTargetOffset.y,
                g_searchHandConfigurations[0].localTargetOffset.z);
            Matrix4x4::MultiplyWithPrePassMatrix(&g_searchHandPairTransform);
            matrix_stack::GetMatrix(&target);
            PopCurrentMatrix(1);
            BuildError(error, current, target);
            g_searchHandConfigurations[searchHandSide].currentTarget.x = target.rows[3].x;
            g_searchHandConfigurations[searchHandSide].currentTarget.y = target.rows[3].y;
            g_searchHandConfigurations[searchHandSide].currentTarget.z = target.rows[3].z;
            const Vector3 axisX = HandMatrixRow(target, 0);
            HandIKConfig *const configX = &g_searchHandConfigurations[searchHandSide];
            configX->currentAxisX = axisX;
            const Vector3 axisZ = HandMatrixRow(target, 2);
            HandIKConfig *const configZ = &g_searchHandConfigurations[searchHandSide];
            configZ->currentAxisZ = axisZ;
        } else {
            switch (g_searchHandConfigurations[searchHandSide].mode) {
            case HandIK_PathFollow:
                if (AdvancePath(&position, &target, searchHandSide)) {
                    target.rows[3].x = position.x;
                    target.rows[3].y = position.y;
                    target.rows[3].z = position.z;
                    BuildError(error, current, target);
                    break;
                }
                {
                    HandIKConfig *configurations = g_searchHandConfigurations;
                    const std::uint8_t side = searchHandSide;
                    Vector3 *const currentTarget = &configurations[side].currentTarget;
                    *currentTarget = configurations[side].destination;
                    Vector3 *const currentAxisX = &configurations[side].currentAxisX;
                    *currentAxisX = configurations[side].destinationAxisX;
                    Vector3 *const currentAxisZ = &configurations[side].currentAxisZ;
                    *currentAxisZ = configurations[side].destinationAxisZ;
                    std::uint8_t *const mode = &configurations[side].mode;
                    *mode = configurations[side].pendingMode;
                    configurations[side].flags &= ~HandIKPathActive;
                }
            case HandIK_HoldTransform:
            case HandIK_Unblended:
            default:
                matrix = &target;
                matrix->rows[0].w = matrix->rows[1].w = matrix->rows[2].w = 0;
                matrix->rows[3].x = g_searchHandConfigurations[searchHandSide].currentTarget.x;
                matrix->rows[3].y = g_searchHandConfigurations[searchHandSide].currentTarget.y;
                matrix->rows[3].z = g_searchHandConfigurations[searchHandSide].currentTarget.z;
                matrix->rows[3].w = 1;
                matrix->rows[0].x = g_searchHandConfigurations[searchHandSide].currentAxisX.x;
                matrix->rows[0].y = g_searchHandConfigurations[searchHandSide].currentAxisX.y;
                matrix->rows[0].z = g_searchHandConfigurations[searchHandSide].currentAxisX.z;
                am2Vector3Cross(&g_searchHandConfigurations[searchHandSide].currentAxisZ, &g_searchHandConfigurations[searchHandSide].currentAxisX, &tangent);
                matrix->rows[1].x = tangent.x;
                matrix->rows[1].y = tangent.y;
                matrix->rows[1].z = tangent.z;
                matrix->rows[2].x = g_searchHandConfigurations[searchHandSide].currentAxisZ.x;
                matrix->rows[2].y = g_searchHandConfigurations[searchHandSide].currentAxisZ.y;
                matrix->rows[2].z = g_searchHandConfigurations[searchHandSide].currentAxisZ.z;
                BuildError(error, current, *matrix);
                break;
            case HandIK_DeltaVector:
                error[0] = g_searchHandConfigurations[searchHandSide].currentTarget.x - current.rows[3].x;
                error[1] = g_searchHandConfigurations[searchHandSide].currentTarget.y - current.rows[3].y;
                error[2] = g_searchHandConfigurations[searchHandSide].currentTarget.z - current.rows[3].z;
                break;
            }
        }
    }
    float magnitude = Vector3::CalcMagnitude(*reinterpret_cast<Vector3 *>(&error[0]));
    if (magnitude > 0.03f) {
        magnitude = 0.03f / magnitude;
        error[0] *= magnitude;
        error[1] *= magnitude;
        error[2] *= magnitude;
    }
    magnitude = Vector3::CalcMagnitude(*reinterpret_cast<Vector3 *>(&error[3]));
    if (magnitude > 0.17453294f) {
        magnitude = 0.17453294f / magnitude;
        error[3] *= magnitude;
        error[4] *= magnitude;
        error[5] *= magnitude;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void BuildJacobian(float (&jacobian)[6][8]) {
    const float (*dimensions)[6] = g_motionConstData.dimensions;
    Matrix4x4 matrix;
    Vector3 axes[8], lever[4], position;
    HandIKConfig *config = &g_searchHandConfigurations[searchHandSide];
    Vector3 offset = config->localTargetOffset;
    Vector3 direction;
    const Matrix4x4 *bone;
    std::int32_t joint, index;
    matrix_stack::Matrix4x4_Push(0);
    bone = mtwkGetBoneTransform(g_searchHandMotionWork, searchHandSide ? 21 : 14);
    matrix_stack::LoadMatrix(bone);
    Matrix4x4::TransformPoint(offset, position);
    Matrix4x4::Transform(offset, offset);
    PopCurrentMatrix(1);
    Vector3 *levers = lever;
    levers[0] = position;
    levers[1] = position;
    levers[2] = position;
    levers[3] = offset;
    mtwkGetBonePosition(g_searchHandMotionWork, static_cast<MotionBoneId>(searchHandSide ? 15 : 8), &position);
    lever[0].x -= position.x;
    lever[0].y -= position.y;
    lever[0].z -= position.z;
    mtwkGetBonePosition(g_searchHandMotionWork, static_cast<MotionBoneId>(searchHandSide ? 16 : 9), &position);
    lever[1].x -= position.x;
    lever[1].y -= position.y;
    lever[1].z -= position.z;
    mtwkGetBonePosition(g_searchHandMotionWork, static_cast<MotionBoneId>(searchHandSide ? 19 : 12), &position);
    lever[2].x -= position.x;
    lever[2].y -= position.y;
    lever[2].z -= position.z;
    matrix_stack::GetMatrix(&matrix);
    Vector3 row0 = HandMatrixRow(matrix, 0);
    axes[0] = row0;
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::RotateAroundZ_Wrapper(0xc000);
    direction.y = -dimensions[g_searchHandMotionWork->rigBoneId][3];
    direction.x = direction.z = 0;
    AlignFrameToTarget_Vector(direction);
    matrix_stack::GetMatrix(&matrix);
    Vector3 row1 = HandMatrixRow(matrix, 2);
    axes[1] = row1;
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(g_searchHandConfigurations[searchHandSide].jointAngles[1] * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&matrix);
    PopCurrentMatrix(1);
    Vector3 row2 = HandMatrixRow(matrix, 1);
    axes[2] = row2;
    bone = mtwkGetBoneTransform(g_searchHandMotionWork, searchHandSide ? 16 : 9);
    Vector3 row3 = HandMatrixRow(*bone, 0);
    axes[3] = row3;
    bone = mtwkGetBoneTransform(g_searchHandMotionWork, searchHandSide ? 19 : 12);
    Vector3 row4 = HandMatrixRow(*bone, 2);
    axes[4] = row4;
    bone = mtwkGetBoneTransform(g_searchHandMotionWork, searchHandSide ? 20 : 13);
    Vector3 row5 = HandMatrixRow(*bone, 0);
    axes[5] = row5;
    Vector3 row6 = HandMatrixRow(*bone, 2);
    axes[6] = row6;
    bone = mtwkGetBoneTransform(g_searchHandMotionWork, searchHandSide ? 21 : 14);
    Vector3 row7 = HandMatrixRow(*bone, 1);
    axes[7] = row7;
    for (joint = 0; joint < 8; ++joint) {
        if (joint == 0)
            index = 0;
        else if (joint < 4)
            index = 1;
        else if (joint == 4)
            index = 2;
        else
            index = 3;
        am2Vector3Cross(&axes[joint], &lever[index], &offset);
        jacobian[0][joint] = offset.x;
        jacobian[1][joint] = offset.y;
        jacobian[2][joint] = offset.z;
        jacobian[3][joint] = axes[joint].x;
        jacobian[4][joint] = axes[joint].y;
        jacobian[5][joint] = axes[joint].z;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void CopyMatrixMinor(const float *matrix, float *minor, std::int32_t order, std::int32_t column) {
    std::int32_t index, current, at;
    for (index = order, current = 0, at = 0; index < order * order; ++index, ++current) {
        if (current == order)
            current = 0;
        if (column != current)
            minor[at++] = matrix[index];
    }
}

float handDeterminantMinor5[25], handDeterminantMinor4[16], handDeterminantMinor3[9];
float *handDeterminantMinorBuffers[3] = {handDeterminantMinor3, handDeterminantMinor4, handDeterminantMinor5};

// @verified - d
float Determinant(const float *matrix, std::size_t order) {
    const std::int32_t dimension = static_cast<std::int32_t>(order);
    float result = 0;
    switch (dimension) {
    case 3: {
        result += (matrix[4] * matrix[8] - matrix[5] * matrix[7]) * matrix[0];
        const float negative = -matrix[1];
        result += negative * (matrix[3] * matrix[8] - matrix[5] * matrix[6]);
        result += matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
        break;
    }
    case 4:
    case 5:
    case 6:
        for (std::int32_t col = 0; col < dimension; ++col) {
            const float sign = col & 1 ? -1.0f : 1.0f;
            CopyMatrixMinor(matrix, handDeterminantMinorBuffers[dimension - 4], dimension, col);
            const float minor = Determinant(handDeterminantMinorBuffers[dimension - 4], dimension - 1);
            result += sign * matrix[col] * minor;
        }
        break;
    }
    return result;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// InvertSix chooses the smallest nonzero pivot, rather than a largest-pivot variant.
bool InvertSix(float (&matrix)[6][6]) {
    int order = 6;
    float work[6][6], inverse[6][6];
    std::memset(work, 0, order * order * sizeof(float));
    std::memset(inverse, 0, order * order * sizeof(float));
    int i, k;
    for (i = 0; i < order; ++i)
        for (k = 0; k < order; ++k) {
            work[0][i * order + k] = matrix[0][i * order + k];
            if (i == k)
                inverse[0][i * order + i] = 1.0f;
        }
    int col, row;
    for (col = 0; col < order; ++col) {
        float pivot = work[0][col * order + col];
        if (::fabsf(pivot) < epsilon) {
            for (row = col + 1;; ++row) {
                if (row == order)
                    return false;
                if (::fabsf(work[0][row * order + col]) > epsilon) {
                    for (k = 0; k < order; ++k) {
                        float value = work[0][row * order + k];
                        float inverseValue = inverse[0][row * order + k];
                        work[0][row * order + k] = work[0][col * order + k];
                        inverse[0][row * order + k] = inverse[0][col * order + k];
                        work[0][col * order + k] = value;
                        inverse[0][col * order + k] = inverseValue;
                    }
                    pivot = work[0][col * order + col];
                    break;
                }
            }
        }
        for (row = col + 1; row < order; ++row)
            if (::fabsf(work[0][row * order + col]) > epsilon && ::fabsf(work[0][row * order + col]) < ::fabsf(pivot)) {
                for (k = 0; k < order; ++k) {
                    float value = work[0][row * order + k];
                    float inverseValue = inverse[0][row * order + k];
                    work[0][row * order + k] = work[0][col * order + k];
                    inverse[0][row * order + k] = inverse[0][col * order + k];
                    work[0][col * order + k] = value;
                    inverse[0][col * order + k] = inverseValue;
                }
                pivot = work[0][col * order + col];
            }
        for (k = 0; k < order; ++k) {
            work[0][col * order + k] /= pivot;
            inverse[0][col * order + k] /= pivot;
        }
        for (row = 0; row < order; ++row)
            if (row != col) {
                float factor = work[0][row * order + col];
                for (k = 0; k < order; ++k) {
                    work[0][row * order + k] -= factor * work[0][col * order + k];
                    inverse[0][row * order + k] -= factor * inverse[0][col * order + k];
                }
            }
    }
    for (i = 0; i < order; ++i)
        for (k = 0; k < order; ++k)
            matrix[0][i * order + k] = inverse[0][i * order + k];
    return true;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
float SolveWeighted(const float (&jacobian)[6][8], const float (&weights)[8], float (&delta)[8], const float (&error)[6], std::int32_t computeDeterminant) {
    float normal[6][6], inverseWeights[8], solved[6];
    float determinant = 0;
    std::int32_t i, j, k;
    for (i = 0; i < 8; ++i)
        inverseWeights[i] = 1.0f / weights[i];
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 6; ++j) {
            normal[0][i * 6 + j] = 0;
            for (k = 0; k < 8; ++k)
                normal[0][i * 6 + j] += (jacobian[0][i * 8 + k] * jacobian[0][j * 8 + k]) * inverseWeights[k];
        }
    if (computeDeterminant)
        determinant = Determinant(&normal[0][0], 6);
    if (InvertSix(normal)) {
        for (i = 0; i < 6; ++i) {
            solved[i] = 0;
            for (j = 0; j < 6; ++j)
                solved[i] += normal[0][i * 6 + j] * error[j];
        }
        for (i = 0; i < 8; ++i) {
            delta[i] = 0;
            for (j = 0; j < 6; ++j)
                delta[i] += jacobian[0][j * 8 + i] * solved[j];
            delta[i] *= inverseWeights[i];
        }
    }
    return determinant;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {


void EvaluateArm(MotionWork &work, std::uint8_t side, std::uint32_t interpolationMode) {
    auto &arm = Arm(work, side);
    auto &config = configurations[side];
    const auto &angles = config.jointAngles;
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = arm.clock.current.ticksTime * 0.033333335f;
    g_motionMaximumAnimTime = arm.clock.authored.maxTime * 0.033333335f;
    if (arm.poseMode == MP_FromAxisAngle)
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

float SearchIK_GetMaxBlend() {
    return handBlend;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void setSearchIKFlag(float blend) {
    handBlend = blend;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HandIKMode getSearchHandConfigModeFor(std::uint8_t side) {
    return handWork && side < 2 ? configurations[side].mode : HandIKMode::Unavailable;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float g_searchHandSingularDeterminant = 0.5f;
float g_searchHandWeakDeterminant = 1.5f;

namespace {

void SolveSearchHand() {
    float jacobian[6][8];
    float weights[8];
    float delta[8];
    float error[6] = {0, 0, 0, 0, 0, 0};
    std::int32_t i;
    BuildTargetError(error);
    float magnitude = 0;
    for (i = 0; i < 6; ++i)
        magnitude += error[i] * error[i];
    if (magnitude < epsilon) {
        g_searchHandSolveStatus = g_searchHandPreviousSolveStatus;
        return;
    }
    BuildJacobian(jacobian);
    for (i = 0; i < 8; ++i) {
        weights[i] = searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i] -
            g_searchHandConfigurations[searchHandSide].jointAngles[i];
        if (weights[i] > 0)
            weights[i] = ::fabsf(weights[i]) /
                (searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i] - searchHandJointLimits[searchHandSide][i][0]);
        else
            weights[i] = ::fabsf(weights[i]) /
                (searchHandJointLimits[searchHandSide][i][1] - searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i]);
        weights[i] = weights[i] * 0.9f + 0.1f;
        delta[i] = 0;
    }
    const float determinant = ::fabsf(SolveWeighted(jacobian, weights, delta, error, 1));
    if (determinant < g_searchHandWeakDeterminant) {
        if (determinant < g_searchHandSingularDeterminant)
            g_searchHandSolveStatus = 2;
        else if (!g_searchHandSolveStatus)
            g_searchHandSolveStatus = 1;
    }
    for (i = 0; i < 8; ++i) {
        weights[i] = searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i] -
            g_searchHandConfigurations[searchHandSide].jointAngles[i];
        if (delta[i] * weights[i] < 0) {
            if (weights[i] > 0)
                weights[i] = ::fabsf(weights[i]) /
                    (searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i] -
                        searchHandJointLimits[searchHandSide][i][0]);
            else
                weights[i] = ::fabsf(weights[i]) /
                    (searchHandJointLimits[searchHandSide][i][1] -
                        searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i]);
            weights[i] = weights[i] * 0.45f + 0.55f;
        } else {
            if (weights[i] > 0)
                weights[i] = ::fabsf(weights[i]) /
                    (searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i] -
                        searchHandJointLimits[searchHandSide][i][0]);
            else
                weights[i] = ::fabsf(weights[i]) /
                    (searchHandJointLimits[searchHandSide][i][1] -
                        searchHandJointTables[g_searchHandConfigurations[searchHandSide].jointTable][searchHandSide][i]);
            weights[i] = 0.55f - weights[i] * 0.45f;
        }
        delta[i] = 0;
    }
    if (::fabsf(SolveWeighted(jacobian, weights, delta, error, 1)) > 0.01f) {
        for (i = 0; i < 8; ++i) {
            g_searchHandConfigurations[searchHandSide].jointAngles[i] += delta[i];
            if (g_searchHandConfigurations[searchHandSide].jointAngles[i] < searchHandJointLimits[searchHandSide][i][0])
                g_searchHandConfigurations[searchHandSide].jointAngles[i] = searchHandJointLimits[searchHandSide][i][0];
            else if (g_searchHandConfigurations[searchHandSide].jointAngles[i] > searchHandJointLimits[searchHandSide][i][1])
                g_searchHandConfigurations[searchHandSide].jointAngles[i] = searchHandJointLimits[searchHandSide][i][1];
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline void ApplySearchChestTransform() {
    matrix_stack::LoadMatrix(&searchHandChestReference);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&searchHandChestPrevious);
    Matrix4x4 relative;
    matrix_stack::GetMatrix(&relative);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(searchHandChestPosition.x, searchHandChestPosition.y, searchHandChestPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(searchHandChestRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(searchHandChestRotation.x);
    Matrix4x4::RotateAroundX_Wrapper(0x4000);
    Matrix4x4::RotateAroundY_Wrapper(0x4000);
    Matrix4x4::MultiplyWithPrePassMatrix(&relative);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void srchArmEvaluate(MotionArmRig &arm, std::uint32_t interpolationMode, const float *angles) {
    const float (*dimensions)[6] = g_motionConstData.dimensions;
    Vector3 axis;
    g_motionInterpolationType = interpolationMode;
    g_motionCurrentAnimTime = arm.clock.current.ticksTime * 0.033333335f;
    g_motionMaximumAnimTime = arm.clock.authored.maxTime * 0.033333335f;
    if (arm.poseMode == MP_FromAxisAngle)
        g_motionMaximumAnimTime = arm.clock.current.maxTime * 0.033333335f;
    Matrix4x4::preTranslate(arm.shoulder.axes[0].dimensions[0], 0, arm.shoulder.axes[2].dimensions[0]);
    if ((g_searchHandConfigurations + searchHandSide)->flags & HandIKEnabled) {
        if ((g_searchHandConfigurations + searchHandSide)->mode == HandIK_HandPose)
            TickSearchHandPose();
        else
            SolveSearchHand();
    }
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(angles[0] * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&arm.shoulderWorld);
    Matrix4x4::preTranslate(0, 0, arm.elbow.axes[2].dimensions[0]);
    Matrix4x4::RotateAroundZ_Wrapper(0xc000);
    axis.y = -dimensions[g_searchHandMotionWork->rigBoneId][3];
    axis.z = 0;
    axis.x = 0;
    AlignFrameToTarget_Vector(axis);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(angles[1] * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(angles[2] * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(angles[3] * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&arm.upperArmWorld);
    Matrix4x4::preTranslate(arm.target.axes[1].dimensions[0], 0, 0);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(angles[4] * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&arm.forearmWorld);
    Matrix4x4::preTranslate(arm.wristRotation[1].dimensions[0], 0, 0);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int32_t>(angles[5] * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&arm.wristWorld);
    Matrix4x4::preTranslate(arm.wristRotation[0].dimensions[0], 0, 0);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int32_t>(angles[6] * 57.29578f * 182.04443f));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int32_t>(angles[7] * 57.29578f * 182.04443f));
    matrix_stack::GetMatrix(&arm.handWorld);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void MotionCommonCallback(std::int32_t mode, std::int32_t interpolationMode) {
    searchHandSide = static_cast<std::uint8_t>(mode - 1);
    if (g_searchHandChestOverrideEnabled) {
        matrix_stack::GetMatrix(&searchHandChestPrevious);
        ApplySearchChestTransform();
        IKRig &rig = reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig;
        matrix_stack::GetMatrix(&rig.chest.upperChestWorld);
    }
    if (mode & 1)
        srchArmEvaluate(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm, interpolationMode,
            g_searchHandConfigurations[searchHandSide].jointAngles);
    if (mode & 2) {
        srchArmEvaluate(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm, interpolationMode,
            g_searchHandConfigurations[searchHandSide].jointAngles);
        g_searchHandPairedTarget = false;
    }
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void mtwkAssignCallbacks(MotionWork *work) {
    if (work == handWork && (work->runtime.seqFlags & MSF_SearchHand))
        SetCustomMotionOverride(&g_searchHandMotionOverride);
    previousSolveStatus = currentSolveStatus;
    currentSolveStatus = 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t mtwkGetCustomOverrideFlags(const MotionWork *work) {
    return work && work == handWork && (work->runtime.seqFlags & MSF_SearchHand) && overrideEnabled ? g_searchHandMotionOverride.flags : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool mtwkClearSearchHandRequests(MotionWork *work) {
    if (!work || work != handWork || !(work->runtime.seqFlags & MSF_SearchHand))
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void GetSearchHandWristAngles(MotionWork &work, std::int16_t (&angles)[4]) {
    g_motionInterpolationType = (work.runtime.seqFlags >> motionSequenceInterpolationShift) & MI_Linear;
    const std::uint32_t flags = mtwkGetCustomOverrideFlags(&work);
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HandParams *GetCharacterHand(Character *character, std::int32_t side) {
    if (!character)
        return nullptr;
    return static_cast<HandParams *>(
#if SHENMUE_IS_DC
charFindComp
#else
FindModelComponent
#endif
(character, shen_name{side ? SHEN_NAME(HNDL) : SHEN_NAME(HNDR)}));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool IsHandPoseInactive(const HandParams &hand) {
    return (hand.poseFlags & HandPoseInactive) != 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t GetHandPoseAnimationResult(const HandParams &hand) {
    return static_cast<std::uint32_t>(hand.poseMode ? -static_cast<std::int32_t>(hand.poseTicks) : hand.currentPoseFrame + 1);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetHandPoseStep(HandParams &hand, float step) {
    if (step > 0.0f)
        hand.poseStep = step;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetHandSecondaryPoseWork(HandParams &hand, void *work) {
    if (hand.secondaryPoseWork == work)
        return;
    hand.secondaryPoseWork = work;
    std::fill(std::begin(hand.boundsWork), std::end(hand.boundsWork), std::numeric_limits<float>::max());
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void GetHandEndpointPosition(const HandParams &hand, std::int32_t endpoint, Vector3 &position) {
    position = hand.jointPositions[handEndBoneIds[endpoint]];
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void GetHandPrimaryNodeRotation(const HandParams &hand, Vector3i &rotation) {
    rotation = hand.primaryNode ? hand.primaryNode->rotation : Vector3i{};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

MT7Node *SetHandPrimaryNodeRotation(HandParams &hand, const Vector3i &rotation) {
    if (hand.primaryNode)
        hand.primaryNode->rotation = rotation;
    return hand.primaryNode;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::byte *WriteHandPoseCommand(const HandParams &hand, std::byte *command) {
    for (std::size_t index = 0; index < 19; ++index) {
        const std::int16_t *const angles = hand.currentJointAngles[19 - index];
        const std::int32_t values[3]{angles[0], angles[1], angles[2]};
        std::memcpy(command + index * sizeof(values), values, sizeof(values));
    }
    return command + 236;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t ParseHandPoseCommand(HandParams *hand, std::byte *command, std::int16_t ticks) {
    if (d3t::GetObjectModelPathHash(GetMT7ReaderData(hand->owner ? static_cast<const MT7Model *>(hand->owner->model) : nullptr)) ==
            HAND_POSE_REPLACEMENT_OWNER_HASH &&
        std::memcmp(command, handPoseReplacementMatch, sizeof(handPoseReplacementMatch)) == 0)
        command = reinterpret_cast<std::byte *>(handPoseReplacementCommand);
    std::uint64_t result = 0;
    hand->poseMode = 1;
    std::memset(hand->targetJointAngles, 0, sizeof(hand->targetJointAngles));
    for (std::size_t index = 0; index < 19; ++index) {
        const std::size_t joint = 19 - index; // Native hand-pose command mapping.
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint64_t ApplyCharacterHandPoseCommand(Character *character, std::int32_t side, std::byte *command, std::int16_t ticks) {
    HandParams *const hand = GetCharacterHand(character, side);
    // The parser takes the region, command and tick count.
    
    return hand ? ParseHandPoseCommand(hand, command, ticks) : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool SetCharacterHandRegionEnabled(Character *character, std::uint8_t side, bool enabled, std::int8_t alphaPolicy) {
    HandParams *const hand = GetCharacterHand(character, side);
    return hand && SetHandRegionEnabled(*hand, enabled ? 1 : 0, 0, alphaPolicy);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

MT7Node *SetHighModelXYZ(Character *character, std::int8_t state, std::int32_t side, std::int16_t pose) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (!hand)
        return nullptr;
    SetHandRegionEnabled(*hand, state, pose, 1);
    return SetHandPrimaryNodeRotation(*hand, {});
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

MT7Node *SetHandRegionXYZWithFlags(Character *character, std::int8_t state, std::int32_t side, std::int16_t pose, std::int8_t alphaPolicy) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (!hand)
        return nullptr;
    SetHandRegionEnabled(*hand, state, pose, alphaPolicy);
    return SetHandPrimaryNodeRotation(*hand, {});
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool ApplyScenarioLeftHandPose(Character *character) {
    // ApplyScenarioLeftHandPose selects this authored pose only for RYO/InternalFlag123.
    static std::int32_t pose[19][3] = {{1999, 65354, 60806}, {1638, 180, 63171}, {546, 64719, 3365}, {0, 64810, 53705}, {0, 64810, 49335}, {0, 0, 60075},
        {0, 0, 64811}, {0, 0, 56800}, {0, 0, 47516}, {0, 65172, 56618}, {0, 0, 65354}, {0, 729, 55706}, {0, 65173, 49338}, {0, 64990, 56256}, {0, 0, 0},
        {0, 0, 56434}, {0, 1639, 49882}, {0, 182, 59349}, {64080, 65354, 64444}};
    return ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(pose), 0) != 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool BeginSearchHand(Character *character, std::int32_t handSelection, std::uint32_t jointTable) {
    if (!character)
        return false;
    g_searchHandCharacter = character;
    g_searchHandMotionWork = charFindMotionwork(character);
    if (!g_searchHandMotionWork) {
        g_searchHandCharacter = 0;
        return false;
    }
    if (g_searchHandMotionWork->sequenceFlags & MSF_SearchHand)
        return true;
    for (std::int32_t region = 0; region < 5; ++region)
        if (g_searchHandMotionWork->regionFlags[region] & 0x10)
            return false;
    if (jointTable > 3)
        jointTable = 0;
    g_searchHandMotionWork->sequenceFlags |= MSF_SearchHand;
    g_searchHandMotionOverride.flags = static_cast<std::uint32_t>(handSelection + 1);
    g_searchHandMotionOverride.evaluate = MotionCommonCallback;
    std::memset(&g_searchHandConfigurations[0], 0, sizeof(HandIKConfig));
    std::memset(&g_searchHandConfigurations[1], 0, sizeof(HandIKConfig));
    g_searchHandConfigurations[0].mode = HandIK_HoldTransform;
    g_searchHandConfigurations[1].mode = HandIK_HoldTransform;
    HandIKConfig *configs = g_searchHandConfigurations;
    configs[0].localTargetOffset = g_searchHandDefaultTargetOffset;
    configs[1].localTargetOffset = g_searchHandDefaultTargetOffset;
    g_searchHandConfigurations[0].jointTable = static_cast<std::uint8_t>(jointTable);
    g_searchHandConfigurations[1].jointTable = static_cast<std::uint8_t>(jointTable);
    g_searchHandConfigurations[0].pathBias = 0;
    g_searchHandConfigurations[1].pathBias = 0;
    for (std::int32_t joint = 0; joint < 8; ++joint) {
        g_searchHandConfigurations[0].jointAngles[joint] = searchHandJointTables[g_searchHandConfigurations[0].jointTable][0][joint];
        g_searchHandConfigurations[1].jointAngles[joint] = searchHandJointTables[g_searchHandConfigurations[1].jointTable][1][joint];
    }
    UpdateAnimRate(g_searchHandConfigurations[0].path[0].channel, 1.0f, 0.0f);
    UpdateAnimRate(g_searchHandConfigurations[0].path[1].channel, 1.0f, 0.0f);
    UpdateAnimRate(g_searchHandConfigurations[0].path[2].channel, 1.0f, 0.0f);
    UpdateAnimRate(g_searchHandConfigurations[1].path[0].channel, 1.0f, 0.0f);
    UpdateAnimRate(g_searchHandConfigurations[1].path[1].channel, 1.0f, 0.0f);
    UpdateAnimRate(g_searchHandConfigurations[1].path[2].channel, 1.0f, 0.0f);
    const std::uint32_t models = SetLowQualityHands(g_searchHandCharacter);
    if (g_searchHandMotionOverride.flags & 1) {
        searchHandSide = 0;
        matrix_stack::Matrix4x4_Push(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.chest.upperChestWorld);
        srchArmEvaluate(
            reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm, 0, g_searchHandConfigurations[searchHandSide].jointAngles);
        Matrix4x4::TransformPoint(g_searchHandConfigurations[searchHandSide].localTargetOffset, g_searchHandConfigurations[searchHandSide].currentTarget);
        Vector3 axisX = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld, 0);
        HandIKConfig *const configX = &g_searchHandConfigurations[searchHandSide];
        configX->currentAxisX = axisX;
        Vector3 axisZ = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld, 2);
        HandIKConfig *const configZ = &g_searchHandConfigurations[searchHandSide];
        configZ->currentAxisZ = axisZ;
        PopCurrentMatrix(1);
        g_searchHandConfigurations[searchHandSide].flags |= HandIKEnabled;
        if (models) {
            CharacterHandComponent *hand = AllocateHNDR(g_searchHandCharacter);
            if (hand)
                SetCharacterHandRegionEnabled(hand, 1, 0, 1);
        }
        g_searchHandConfigurations[searchHandSide].commandDelay = 0;
        g_searchHandConfigurations[searchHandSide].beginPoseCommand = 0;
        g_searchHandConfigurations[searchHandSide].endPoseCommand = 0;
        g_searchHandConfigurations[searchHandSide].flags &= ~HandIKFastCommandTiming;
    }
    if (g_searchHandMotionOverride.flags & 2) {
        searchHandSide = 1;
        matrix_stack::Matrix4x4_Push(&reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.chest.upperChestWorld);
        srchArmEvaluate(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm, 0, g_searchHandConfigurations[searchHandSide].jointAngles);
        Matrix4x4::TransformPoint(g_searchHandConfigurations[searchHandSide].localTargetOffset, g_searchHandConfigurations[searchHandSide].currentTarget);
        Vector3 axisX = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld, 0);
        HandIKConfig *const configX = &g_searchHandConfigurations[searchHandSide];
        configX->currentAxisX = axisX;
        Vector3 axisZ = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld, 2);
        HandIKConfig *const configZ = &g_searchHandConfigurations[searchHandSide];
        configZ->currentAxisZ = axisZ;
        PopCurrentMatrix(1);
        g_searchHandConfigurations[searchHandSide].flags |= HandIKEnabled;
        if (models) {
            CharacterHandComponent *hand = AllocateHNDL(g_searchHandCharacter);
            if (hand)
                SetCharacterHandRegionEnabled(hand, 1, 0, 1);
        }
        g_searchHandConfigurations[searchHandSide].commandDelay = 0;
        g_searchHandConfigurations[searchHandSide].beginPoseCommand = 0;
        g_searchHandConfigurations[searchHandSide].endPoseCommand = 0;
        g_searchHandConfigurations[searchHandSide].flags &= ~HandIKFastCommandTiming;
    }
    g_searchHandSolveStatus = 0;
    g_searchHandPreviousSolveStatus = 0;
    setSearchIKFlag(1.0f);
    return true;
}

#else

bool BeginSearchHand(Character *character, std::int32_t handSelection, std::uint32_t jointTable) {
    if (!character)
        return false;
    handCharacter = character;
    handWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(character);
    if (!handWork) {
        handCharacter = nullptr;
        return false;
    }
    if (handWork->runtime.seqFlags & MSF_SearchHand)
        return true;
    for (auto flags : handWork->runtime.regionFlags)
        if (flags & 0x10U)
            return false;
    if (jointTable > 3)
        jointTable = 0;
    handWork->runtime.seqFlags |= MSF_SearchHand;
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
        EvaluateArm(*handWork, side, MI_HermiteAuto);
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void SetSearchHandChestTransform(const Vector3 *position, const Vector3i *rotation) {
    if (!g_searchHandMotionWork)
        return;
    searchHandChestPosition = *position;
    searchHandChestRotation = *rotation;
    IKRig &rig = reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig;
    matrix_stack::Matrix4x4_Push(&rig.chest.upperChestWorld);
    if (!g_searchHandChestOverrideEnabled) {
        matrix_stack::GetMatrix(&searchHandChestPrevious);
        matrix_stack::GetMatrix(&searchHandChestReference);
    }
    matrix_stack::LoadMatrix(&searchHandChestReference);
    matrix_stack::InvertMatrix(0);
    Matrix4x4::MultiplyWithPrePassMatrix(&searchHandChestPrevious);
    Matrix4x4 relative;
    matrix_stack::GetMatrix(&relative);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(searchHandChestPosition.x, searchHandChestPosition.y, searchHandChestPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(searchHandChestRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(searchHandChestRotation.x);
    Matrix4x4::RotateAroundX_Wrapper(0x4000);
    Matrix4x4::RotateAroundY_Wrapper(0x4000);
    Matrix4x4::MultiplyWithPrePassMatrix(&relative);
    if (g_searchHandMotionOverride.flags & 1) {
        searchHandSide = 0;
        g_searchHandConfigurations[searchHandSide].flags &= ~HandIKEnabled;
        matrix_stack::Matrix4x4_Push(0);
        srchArmEvaluate(
            reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm, 0, g_searchHandConfigurations[searchHandSide].jointAngles);
        Matrix4x4::TransformPoint(g_searchHandConfigurations[searchHandSide].localTargetOffset, g_searchHandConfigurations[searchHandSide].currentTarget);
        const Vector3 axisX = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld, 0);
        HandIKConfig *configX = &g_searchHandConfigurations[searchHandSide];
        configX->currentAxisX = axisX;
        const Vector3 axisZ = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld, 2);
        HandIKConfig *configZ = &g_searchHandConfigurations[searchHandSide];
        configZ->currentAxisZ = axisZ;
        g_searchHandConfigurations[searchHandSide].flags |= HandIKEnabled;
        PopCurrentMatrix(1);
    }
    if (g_searchHandMotionOverride.flags & 2) {
        searchHandSide = 1;
        g_searchHandConfigurations[searchHandSide].flags &= ~HandIKEnabled;
        matrix_stack::Matrix4x4_Push(0);
        srchArmEvaluate(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm, 0, g_searchHandConfigurations[searchHandSide].jointAngles);
        Matrix4x4::TransformPoint(g_searchHandConfigurations[searchHandSide].localTargetOffset, g_searchHandConfigurations[searchHandSide].currentTarget);
        const Vector3 axisX = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld, 0);
        HandIKConfig *configX = &g_searchHandConfigurations[searchHandSide];
        configX->currentAxisX = axisX;
        HandIKConfig *configZ = &g_searchHandConfigurations[searchHandSide];
        const Vector3 axisZ = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld, 2);
        configZ->currentAxisZ = axisZ;
        g_searchHandConfigurations[searchHandSide].flags |= HandIKEnabled;
        PopCurrentMatrix(1);
    }
    PopCurrentMatrix(1);
    g_searchHandChestOverrideEnabled = 1;
}

std::int32_t GetSearchHandChestTransform(Vector3 *position, Vector3i *rotation) {
    std::int32_t result;
    if (!g_searchHandMotionWork)
        return 0;
    const std::int32_t enabled = g_searchHandChestOverrideEnabled;
    if (!enabled) {
        mtwkGetBonePosition(g_searchHandMotionWork, 3, position);
        rotation->y = g_searchHandMotionWork->entityYaw;
        rotation->z = 0;
        rotation->x = 0;
        result = 0;
    } else {
        *position = searchHandChestPosition;
        *rotation = searchHandChestRotation;
        result = 1;
    }
    return result;
}

void ClearSearchHandChestTransform() {
    if (!g_searchHandMotionWork)
        return;
    g_searchHandChestOverrideEnabled = 0;
    if (g_searchHandMotionOverride.flags & 1) {
        searchHandSide = 0;
        g_searchHandConfigurations[searchHandSide].flags &= ~HandIKEnabled;
        matrix_stack::Matrix4x4_Push(&searchHandChestPrevious);
        srchArmEvaluate(
            reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm, 0, g_searchHandConfigurations[searchHandSide].jointAngles);
        Matrix4x4::TransformPoint(g_searchHandConfigurations[searchHandSide].localTargetOffset, g_searchHandConfigurations[searchHandSide].currentTarget);
        const Vector3 axisX = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld, 0);
        g_searchHandConfigurations[searchHandSide].currentAxisX = axisX;
        const Vector3 axisZ = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.rightArm.handWorld, 2);
        g_searchHandConfigurations[searchHandSide].currentAxisZ = axisZ;
        PopCurrentMatrix(1);
        g_searchHandConfigurations[searchHandSide].flags |= HandIKEnabled;
    }
    if (g_searchHandMotionOverride.flags & 2) {
        searchHandSide = 1;
        g_searchHandConfigurations[searchHandSide].flags &= ~HandIKEnabled;
        matrix_stack::Matrix4x4_Push(&searchHandChestPrevious);
        srchArmEvaluate(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm, 0, g_searchHandConfigurations[searchHandSide].jointAngles);
        Matrix4x4::TransformPoint(g_searchHandConfigurations[searchHandSide].localTargetOffset, g_searchHandConfigurations[searchHandSide].currentTarget);
        const Vector3 axisX = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld, 0);
        g_searchHandConfigurations[searchHandSide].currentAxisX = axisX;
        const Vector3 axisZ = HandMatrixRow(reinterpret_cast<MotionWorkBindings *>(g_searchHandMotionWork)->rig.leftArm.handWorld, 2);
        g_searchHandConfigurations[searchHandSide].currentAxisZ = axisZ;
        PopCurrentMatrix(1);
        g_searchHandConfigurations[searchHandSide].flags |= HandIKEnabled;
    }
}

#else

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
            EvaluateArm(*handWork, side, MI_HermiteAuto);
            CaptureTarget(configurations[side], Arm(*handWork, side).handWorld);
            configurations[side].flags |= HandIKEnabled;
            Matrix4x4::Pop(1);
        }
    chestOverride = true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void mtwkResetBlends() {
    if (handWork) {
        handWork->runtime.seqFlags &= ~MSF_SearchHand;
        SetBlend(1);
    }
    if (handCharacter)
        for (std::uint8_t side = 0; side < 2; ++side)
            if (g_searchHandMotionOverride.flags & (1U << side))
                SetCharacterHandRegionEnabled(handCharacter, side, false, 1);
    ResetSearchHandState();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetSearchHandState() {
    handCharacter = nullptr;
    handWork = nullptr;
    overrideEnabled = false;
    chestOverride = false;
    pairedTarget = false;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool EvaluateSearchHandArm(MotionWork &work, std::uint8_t side, std::uint32_t interpolationMode) {
    if (!mtwkGetCustomOverrideFlags(&work) || side >= 2 || !(g_searchHandMotionOverride.flags & (1U << side)))
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

#endif

} // namespace s2
} // namespace shenmue
