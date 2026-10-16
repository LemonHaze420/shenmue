#include "s1/motion.hpp"

#include "s1/motion_registry.hpp"
#include "s1/boot.hpp"
#include "s1/collision.hpp"
#include "s1/scene.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/came.hpp"
#include "s1/task/motf.hpp"
#include "s1/task/mbst.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/pack.hpp"
#include "s1/player_control.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/vfma.hpp"
#include "s1/hlib.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/timing.hpp"
#include "s1/mt5_types.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <bit>
#include <cmath>
#include <numbers>
#include <limits>

#include "s1/motion_internal.hpp"

namespace shenmue::s1 {



Vector3 g_RootMotionVector{};
std::int32_t g_motnHasYValue{};

std::int32_t g_motnRowBytesInUse{};

namespace motion_detail {

MotionNodeRuntime *FindRuntimeNode(MotionNodeRuntime *nodes, std::uint8_t nodeId) {
    if (!nodes)
        return nullptr;
    while (nodes->nodeId != 0xff) {
        if (nodes->nodeId == nodeId)
            return nodes;
        ++nodes;
    }
    return nullptr;
}

bool MaintainFrameRate(float &timer, float &timerReference, float &residualTime, bool resetMode, bool reverse, bool compensateWrap, float frameStart,
    float frameEnd, float frameDelta) {
    const float wholeEnd = ceil_float(frameEnd);
    timerReference = timer;
    residualTime = 0.0f;
    if (g_bEnableFrameTimeSmoothing)
        frameDelta = frameDelta * 30.0f * fFrameTimeMs;

    if (g_nAuthorState == 0x14)
        frameDelta = 0.0f;
    if (reverse) {
        timer -= frameDelta;
        if (frameStart > timer) {
            if (resetMode) {
                timer = frameEnd;
                timerReference = frameEnd;
            } else {
                timer = frameStart;
            }
            return true;
        }
        return false;
    }

    timer += frameDelta;
    if (!(timer > frameEnd))
        return false;
    if (compensateWrap) {
        residualTime = wholeEnd - timerReference;
        const float wrapped = timer - wholeEnd;
        timer = (0.0f > wrapped ? 0.0f : wrapped) + frameStart;
        timerReference = -1.0f;
    } else if (resetMode) {
        timer = frameStart;
        timerReference = -1.0f;
    } else {
        timer = frameEnd;
    }
    return true;
}

void AllocateMotionNodes(MotionNodeRuntime *&nodes, std::int64_t &cookie, Matrix4x4 **matrixPool);
void MOTN_FreeNodeRowsAndMatrixPool(void *&rows, std::int64_t &cookie, void **matrixPool);
void FreeMotionBlock(void *block);

Vector3 SampleNodePosition(MotionNodeRuntime &node, float frame, std::int32_t frameCount, bool mirrored) {
    const float time = frame * (1.0f / 30.0f);
    Vector3 result{};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const float bind = !node.bindPosition ? 0.0f : axis == 0 ? node.bindPosition->x : axis == 1 ? node.bindPosition->y : node.bindPosition->z;
        const MotnKeyframeChannel *const channel = node.translationKeys ? &node.translationKeys[axis] : nullptr;
        float value = bind;
        if (channel && channel->present) {
            value = MOTN_CalculateKeyframeForTime(const_cast<MotnKeyframeChannel &>(*channel), time, frameCount) * node.timeScale;
            if (axis == 0 && mirrored)
                value = -value;
            value += axis == 0 ? g_RootMotionVector.x : axis == 1 ? g_RootMotionVector.y : g_RootMotionVector.z;
        }
        if (axis == 0)
            result.x = value;
        else if (axis == 1)
            result.y = value;
        else
            result.z = value;
    }
    return result;
}




void ExtractRootMotion(MotionNodeRuntime &root, std::int32_t frameCount, const MotionSampleClock &clock, bool deltaUpdate, bool mirrored) {
    Vector3 extracted{};
    g_RootMotionVector = {};
    float timerReference = clock.reference;
    if (!deltaUpdate && clock.residual > 0.0f) {
        const float end = ceil_float(clock.end);
        const Vector3 beforeWrap = SampleNodePosition(root, end - clock.residual, frameCount, mirrored);
        const Vector3 atEnd = SampleNodePosition(root, end, frameCount, mirrored);
        extracted.x = atEnd.x - beforeWrap.x;
        extracted.z = atEnd.z - beforeWrap.z;
        timerReference = clock.start;
    }
    if (timerReference >= 0.0f) {
        const Vector3 previous = SampleNodePosition(root, timerReference, frameCount, mirrored);
        extracted.x -= previous.x;
        extracted.z -= previous.z;
    }

    g_RootMotionVector = extracted;
    root.effectorPosition = {};
    const Vector3 current = SampleNodePosition(root, clock.frame, frameCount, mirrored);
    root.effectorPosition = {current.x, current.y, current.z, 0.0f};

    g_RootMotionVector = extracted;
}

void UpdateRootMotionDelta(MotionInformationWork &motion, MotionNodeRuntime &root) {
    motion.previousRootPreTranslate = motion.rootPreTranslate;
    motion.rootWorldPosition = {
        root.transform->rows[3].x,
        root.transform->rows[3].y,
        root.transform->rows[3].z,
    };
    motion.rootMotionDelta = {};
    constexpr std::uint32_t kStateFrameReset = 1U << 24U;
    if (motion.entityEuler.x != 0 || motion.entityEuler.z != 0 || (motion.motionFlags & kStateFrameReset) != 0)
        return;
    if ((motion.motionFlags & 0x10U) != 0) {
        motion.rootPreTranslate.x = motion.rootWorldPosition.x;
        motion.rootPreTranslate.z = motion.rootWorldPosition.z;
    }
    motion.rootMotionDelta.x = motion.rootWorldPosition.x - motion.rootPreTranslate.x;
    motion.rootMotionDelta.z = motion.rootWorldPosition.z - motion.rootPreTranslate.z;
}

} // namespace

std::uint16_t MOTN_ExtractEulerAngle(const MotionInformationWork &motion, std::uint8_t nodeId) {
    MotionNodeRuntime *const node = FindRuntimeNode(static_cast<MotionNodeRuntime *>(motion.activeNodes), nodeId);
    if (!node || !node->transform)
        return 0;

    Matrix4x4 transform = *node->transform;
    transform.rows[3].x = 0.0f;
    transform.rows[3].y = 0.0f;
    transform.rows[3].z = 0.0f;
    Vector3 projection{};
    Matrix4x4_Push(&transform);
    Matrix4x4::TransformPoint({0.0f, 0.0f, -1.0f}, projection);
    Matrix4x4::Pop(1);
    if (std::fabs(projection.y) > 0.6000000238418579f) {
        const float direction = projection.y >= 0.0f ? -1.0f : 1.0f;
        Matrix4x4_Push(&transform);
        Matrix4x4::TransformPoint({0.0f, direction, 0.0f}, projection);
        Matrix4x4::Pop(1);
    }
    return Shenmue_Atan2Fixed(projection.z, -projection.x);
}

namespace motion_detail {

std::uint32_t MOTN_ExtractSpineYaw(const MotionInformationWork &motion) {
    MotionNodeRuntime *const node = FindRuntimeNode(static_cast<MotionNodeRuntime *>(motion.activeNodes), 2);
    if (!node)
        return 0;

    const Matrix4x4 &transform = *node->transform;
    const float x = transform.rows[2].x;
    const float z = transform.rows[2].z;
    if (std::sqrt(x * x + z * z) <= 0.00000011920929f)
        return Shenmue_Atan2Fixed(transform.rows[1].x, transform.rows[1].z);
    return static_cast<std::uint16_t>(Shenmue_Atan2Fixed(x, z) - 0x4000U);
}
#pragma pack(push, 1)
#pragma pack(pop)

} // namespace
std::uint16_t Shenmue_Atan2Fixed(float first, float direction) {
    const auto roundedRatio = [](float numerator, float denominator) {
        constexpr float kFullTurn = 65536.0f;
        constexpr float kTwoPi = 6.2831855f;
        return static_cast<std::int32_t>(std::atan(numerator / denominator) * kFullTurn / kTwoPi + 0.5f);
    };

    if (direction < first) {
        if (direction < -first) {
            if (direction == 0.0f)
                return 0x8000U;
            return static_cast<std::uint16_t>(roundedRatio(first, direction) - 0x8000);
        }
        if (first == 0.0f)
            return 0x4000U;
        return static_cast<std::uint16_t>(0x4000 - roundedRatio(direction, first));
    }
    if (direction < -first) {
        if (first == 0.0f)
            return 0xc000U;
        return static_cast<std::uint16_t>(-0x4000 - roundedRatio(direction, first));
    }
    if (direction == 0.0f)
        return 0;
    return static_cast<std::uint16_t>(roundedRatio(first, direction));
}

void MOTI_ApplyPositionOffset(MotionInformationWork &motion, const Vector3 &offset) {
    auto *const root = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    if (!root || !root->transform)
        return;
    motion.appliedPositionOffset.x += offset.x;
    motion.appliedPositionOffset.y += offset.y;
    motion.appliedPositionOffset.z += offset.z;
    for (MotionNodeRuntime *node = root; node->nodeId != 0xffU; ++node) {
        if ((node->bindPositionPacked[1] & 2U) == 0)
            continue;
        node->transform->rows[3].x += offset.x;
        node->transform->rows[3].y += offset.y;
        node->transform->rows[3].z += offset.z;
    }
    UpdateRootMotionDelta(motion, *root);
}
bool MOTN_GetEvaluatedNodeWorldPosition(const MotionInformationWork &motion, std::uint8_t nodeId, Vector3 &outPosition) {
    const MotionNodeRuntime *node = static_cast<const MotionNodeRuntime *>(motion.activeNodes);
    if (!node || motion.poseEvaluationSucceeded == 0)
        return false;
    while (node->nodeId != 0xffU && node->nodeId != nodeId)
        ++node;
    if (node->nodeId == 0xffU || !node->transform || (node->bindPositionPacked[1] & 2U) == 0) {
        return false;
    }
    outPosition = {node->transform->rows[3].x, node->transform->rows[3].y, node->transform->rows[3].z};
    return true;
}

const MotionRigType *MOTN_GetRigType(std::uint16_t rigTypeIndex) {
    const std::size_t index = rigTypeIndex & 0x7fffU;
    if (index >= std::size(motion_rigs::kMotionRigTypes))
        return nullptr;
    const MotionRigType &rig = motion_rigs::kMotionRigTypes[index];
    return rig.nodes && rig.nodeCount ? &rig : nullptr;
}

void MOTN_InitRigNodes(MotionInformationWork &motion, MotionNodeRuntime *nodes, std::uint16_t rigTypeIndex, float timeScale, MotionNodeRuntime *pairedNodes) {
    if (!nodes)
        return;
    const MotionRigType *const rig = MOTN_GetRigType(rigTypeIndex);
    if (!rig) {
        nodes[0].nodeId = 0xff;
        return;
    }

    auto *const matrices = static_cast<Matrix4x4 *>(motion.nodeMatrixPool);
    for (std::size_t index = 0; index < rig->nodeCount; ++index) {
        const MotionRigNode &source = rig->nodes[index];
        MotionNodeRuntime &destination = nodes[index];
        destination.nodeId = source.nodeId;
        destination.templateParentOrMode = source.parentMode;
        destination.flipFlags = source.flipFlags;
        destination.flags = source.flags;
        destination.childCount = source.childCount;
        destination.channelSet = source.positionMode;
        destination.nodeKind = source.nodeKind;
        destination.reserved07 = 0;
        destination.bindPositionPacked[0] = 0;
        destination.bindPositionPacked[1] = source.keyframeSlots != 0;
        destination.bindEuler = {source.euler[0], source.euler[1], source.euler[2]};
        destination.effectorPosition = {source.position.x, source.position.y, source.position.z, 0.0f};
        destination.translationKeys = nullptr;
        destination.rotationKeys = nullptr;
        destination.bindPosition = &source.position;
        destination.bindEulerSource = reinterpret_cast<const Vector3s *>(source.euler);
        for (std::size_t child = 0; child < 3; ++child) {
            destination.children[child] = child < source.childCount ? &nodes[source.children[child]] : nullptr;
        }
        destination.pairedSourceNode = pairedNodes ? &pairedNodes[index] : nullptr;
        destination.interpolationAmount = 0.0f;
        destination.timeScale = timeScale;
        destination.transform = matrices ? &matrices[index] : nullptr;
    }
    nodes[rig->nodeCount].nodeId = 0xff;
}

float MOTI_GetPlayRateScaledRemain(const MotionInformationWork &motion) {
    const float frameTimer = motion.frameTimer;
    if (g_updateRenderFrameMode < 0)
        return frameTimer;

    const double playbackRate = static_cast<double>(motion.playbackRate);
    if (playbackRate > 0.99 && playbackRate < 1.01) {
        const std::int32_t remainder = static_cast<std::int32_t>(static_cast<double>(frameTimer) * 100.0) % 100;
        const auto result = static_cast<float>(static_cast<std::int32_t>(static_cast<double>(frameTimer) + 0.01));
        if (remainder != 0 && ((remainder / 10) & 1) == 0)
            return static_cast<float>(static_cast<double>(result) + 1.0);
        return result;
    }

    if (g_updateRenderFrameMode == 1)
        return frameTimer;
    std::int32_t frames = g_updateRenderFrameMode - 1;
    if (frames < 0)
        frames = 5;
    return static_cast<float>(static_cast<double>(frameTimer) - playbackRate * 0.1875 * static_cast<double>(static_cast<float>(frames)));
}

bool MOTI_RefreshInterpolationState(MotionInformationWork &motion) {
    if (motion.interpolationMode != 4U)
        return false;

    float transitionFrame = -1.0f;
    if (motion.lookupAnimId) {
        const float lookupFrames = static_cast<float>(MOTN_getFrameCountFromAnimId(motion.lookupAnimId) - motnGetRootMotionEndMarker(motion.lookupAnimId));
        const float currentFrames = static_cast<float>(MOTN_getFrameCountFromAnimId(motion.currentAnimId) - motnGetRootMotionEndMarker(motion.currentAnimId));
        transitionFrame = lookupFrames / currentFrames * motion.frameTimer;
    }
    motion.forcedFrameTime = transitionFrame;

    const bool hasCue19 = shenLookupAnimationIndex(motion.lookupAnimId, 19) != nullptr;
    if (((motion.motionFlags >> 19U) & 1U) != static_cast<std::uint32_t>(hasCue19)) {
        motion.motionFlags |= 0x80U;
    }
    motion.pendingAnimId = motion.lookupAnimId;
    return true;
}

void MOTI_SetRootTransformBlendBits(MotionInformationWork &motion, std::uint32_t value) {
    motion.rootTransformBlend = std::bit_cast<float>(value);
}

namespace motion_detail {} // namespace

void MOTN_UpdateFrameTimer(MotionInformationWork &motion) {
    const float forcedFrame = motion.forcedFrameTime;
    if (forcedFrame >= motion.frameStart && forcedFrame <= motion.frameEnd) {
        motion.frameTimer = forcedFrame;
        motion.forcedFrameTime = -1.0f;
        if ((motion.motionFlags & 0x01000800U) == 0) {
            const float previous = forcedFrame - motion.playbackRate;
            motion.frameTimerRef = previous < 0.0f ? forcedFrame : previous;
        } else {
            motion.frameTimerRef = -1.0f;
        }
    }
    if (motion.frameTimer < motion.frameStart)
        motion.frameTimer = motion.frameStart;
    if (motion.frameTimer > motion.frameEnd)
        motion.frameTimer = motion.frameEnd;
}

MotionInformationWork *g_pMotion = nullptr;               // g_pMotion
std::uint32_t g_motnBasePoseOnly = 0;                     // g_bBasePoseOnly
std::uint32_t g_motnIsRyo = 0;                            // Shenmue::MOTN_n_g_bIsRyo
float g_motnClipAngle = 0.0f;                             // clipAngle
MotionCubicSequenceScratch *g_motnCubicScratch = nullptr; // g_motn_data
std::uint8_t g_motnSoundBank = 0;                         // g_tmp_motn_snd_bank

namespace motion_detail {

constexpr std::array<Vector3, 8> kBoneDirections{{
    {1.0f, -1.0f, -1.0f},
    {0.0f, 0.0f, 0.0f},
    {-1.0f, -1.0f, -1.0f},
    {0.0f, 0.5f, 0.0f},
    {1.0f, -1.0f, 1.0f},
    {0.0f, -0.5f, 0.0f},
    {-1.0f, -1.0f, 1.0f},
    {0.0f, 0.0f, 0.0f},
}};

#pragma pack(push, 1)
#pragma pack(pop)

MotionInformationWork *g_cubicSequenceMotion = nullptr;
bool g_initializeCubicSequence = false; // g_initializeCubicSequence
bool g_cubicSequenceDirection = false;  // g_has_remaining_steps
float g_cubicSequenceDistance = 0.0f;   // MOTN::CurrentVectorDistance

Vector3 g_ryoFootGroundHeights{};

float g_ryoLeftFootPreviousRawY = 0.0f;
float g_ryoRightFootPreviousRawY = 0.0f;

constexpr std::array<std::int16_t, 9> kHeadAccelerationLimits{
    640,
    1024,
    512,
    384,
    768,
    352,
    336,
    640,
    288,
};
constexpr std::array<float, 9> kHeadAccelerationScales{
    1.2f,
    1.28f,
    1.24f,
    1.08f,
    1.14f,
    1.12f,
    1.06f,
    1.1f,
    1.1f,
};
constexpr std::array<float, 3> kHeadCurveBias{0.09f, 0.1f, 0.08f};


constexpr std::array<std::uint32_t, 11> kMotnTargetNodeIds{0, 3, 6, 11, 17, 21, 26, 33, 38, 42, 48};



Vector3i *g_motnEntityEuler = nullptr;           // MOTN::g_Motion
Vector3i *g_motnNextEntityEuler = nullptr;       // MOTN::g_next_rot
std::uint32_t g_motnReverse = 0;                 // MOTN::g_bShouldDeltaUpdate (STATE_SHOULD_UPDATE: reverse playback)
float g_motnTimerRef = 0.0f;                     // MOTN::g_fTimerRef
float g_motnFrameStart = 0.0f;                   // MOTN::g_fCurrentFrameAccum
float g_motnFrameEnd = 0.0f;                     // MOTN::currentAnimNumFrames
float g_motnResidualTime = 0.0f;                 // MOTN::g_fCurrentResidualTime
std::int32_t g_motnFrameCount = 0;               // MOTN::gFrameCnt
std::uint32_t g_motnMirror = 0;                  // MOTN::currentFrameIdxMaybe
std::uint32_t g_motnNeedsRotation = 0;           // MOTN::NeedsRotation
std::uint32_t g_motnIgnoreArmIK = 0;             // MOTN::g_bIsIgnoreArmIK
std::uint8_t g_currentMotionUpdateMode = 0;      // g_curr_motn_mode
std::int32_t g_motnArmBlendMode = 0;             // g_tmp_mode
Matrix4x4 g_skinningTransformMatrix{};           // Shenmue::skinningTransformMatrix
Matrix4x4 g_motnBlendStart{};                    // g_tmp_start_xform
Matrix4x4 g_motnBlendEnd{};                      // g_tmp_end_xform
Matrix4x4 g_motnInterpTransform{};               // Matrix4x4::workInterpXForm
Matrix4x4 g_motnFinalInterpTransform{};          // Matrix4x4::workFinalInterpXForm
Matrix4x4 g_motnRootInterpTransform{};           // Matrix4x4::IdentityMatrixWithPosition
Vector3 g_motnTrackedNodePosition{};             // tempValX
std::array<Vector3, 9> g_motnSecondaryTargets{}; // InVector
Matrix4x4 g_motnSecondaryAnchor{};               // g_pMotionTempXform
Vector3 g_motnLookTarget{};                      // g_motn_temp_v3
// Previous-frame knee/ankle positions captured by mainMotionUpdate before PlayMotionForRyo replaces the rows.
std::int32_t g_motnRightKneeValid = 0;
Vector3 g_motnRightKnee{}; // g_motn_tmp_pos
std::int32_t g_motnRightAnkleValid = 0;
Vector3 g_motnRightAnkle{}; // MOTN::UnknownTemporaryVector
std::int32_t g_motnLeftKneeValid = 0;
Vector3 g_motnLeftKnee{};
std::int32_t g_motnLeftAnkleValid = 0; // g_b_ik_flag_maybe_arms
Vector3 g_motnLeftAnkle{};             // g_arm_pos_x
// TranslateVectorByConditions / BlendPositionWithGlobalTarget parameters.
const Vector3 *g_motnAnkleKnee = nullptr; // g_tmp_MOTN_vec4
std::int32_t g_motnAnkleKneeValid = 0;
float g_motnAnkleReach = 0.0f;
const Vector3 *g_motnAnkleRelease = nullptr; // g_tmp_mot_vec3
std::int32_t g_motnAnkleReleaseValid = 0;    // g_motn_dirty_flag
// ApplyArmTargetsAndLengthClamp_MOTN scratch.
MotionNodeRuntime *g_motnRightAnkleRow = nullptr;       // g_arm_one_mot
MotionNodeRuntime *g_motnLeftAnkleRow = nullptr;        // g_arm_two_mot
MotionNodeRuntime *g_motnRightAnkleSourceRow = nullptr; // g_motion_struct_0
MotionNodeRuntime *g_motnLeftAnkleSourceRow = nullptr;  // g_motion_struct_2
Vector3 g_motnRightAnkleSampled{};                      // g_arm_one
Vector3 g_motnLeftAnkleSampled{};                       // g_arm_two
Vector3 g_motnRightAnkleSourceSampled{};
Vector3 g_motnLeftAnkleSourceSampled{};
Vector3 g_motnRightAnkleCubic{};      // arm1_eff_pos_v3
Vector3 g_motnLeftAnkleCubic{};       // arm2_eff_pos_v3
float g_motnContactRadiusBias = 0.0f; // MOTN::ArmHeightMaxZ
// MOTN_ProcessSequenceInterpolation outputs consumed by InterpolateJointPosition_CubicPolynomial and Shenmue_CubicInterp.
float g_motnSequenceRootLift = 0.0f;      // g_temp_motn_val
float g_motnSequenceLeadHandLift = 0.0f;  // g_temp_motn_val_0
float g_motnSequenceTrailHandLift = 0.0f; // MOTM::framesPerSecond_1
float g_motnSequenceActiveFrame = 0.0f;   // MOTM::framesPerSecond_0
float g_motnSequencePreviousFrame = 0.0f; // interpolatedFramesPerSecond

} // namespace

namespace motion_detail {

void MOTN_ApplyBoneHierarchyTransform(MotionNodeRuntime &node, MotionNodeRuntime &source);

void MOTN_UpdateRawSecondaryPose(MotionInformationWork &motion, MotionNodeRuntime &rows);

bool MOTN_EvaluateCubicSequenceBlend(MotionInformationWork &motion);
bool MOTN_EvaluateRawMotion(MotionInformationWork &motion);

bool IsRyoWalkLoopAnimation(std::uint16_t animationId) {
    return animationId == 735 || animationId == 1374 || animationId == 102 || animationId == 653 || animationId == 655 ||
        static_cast<std::uint16_t>(animationId - 1194U) <= 1U;
}

// PlayMotionForRyo: loads the transition rows a walk-loop request or queued lookup needs, defers a BasePoseThenDeferredAnimation
// request behind the base pose, and applies the requested animation.
std::int32_t PlayMotionForRyo(MotionInformationWork &motion, std::uint16_t animationId) {
    if (!animationId)
        return static_cast<std::int32_t>((motion.motionFlags >> 12U) & 1U);
    if ((motion.rootAngleFlags & 0x800000U) == 0 && IsRyoWalkLoopAnimation(animationId) && (motion.motionFlags & 0x100U) != 0) {
        MOTN_LoadTransitionRows(motion, motion.walkBlendAnimId);
    } else if (motion.queuedBlendAnimId) {
        MOTN_LoadTransitionRows(motion, motion.queuedBlendAnimId);
        motion.queuedBlendAnimId = 0;
    } else if (motion.transitionNodes) {
        MOTN_FreeNodeRowsAndMatrixPool(motion.transitionNodes, motion.transitionNodesCookie, nullptr);
        motion.lookupAnimId = 0;
    }
    bool deferred = false;
    Vector3i savedTurn{};
    std::uint32_t savedMirror = 0;
    float savedStart = 0.0f;
    float savedEnd = 0.0f;
    float savedForced = 0.0f;
    if (motion.blendMode == 8 && !motion.deferredPrimaryAnimId) {
        deferred = true;
        motion.deferredPrimaryAnimId = animationId;
        savedTurn = motion.pendingTurnEulerDelta;
        animationId = kBasePoseAnimationIds[motion.basePoseTemplateIndex];
        motion.pendingTurnEulerDelta = {};
        savedStart = motion.scriptStartFrame;
        savedEnd = motion.scriptEndFrame;
        savedForced = motion.forcedFrameTime;
        savedMirror = (motion.motionFlags >> 7U) & 1U;
        motion.scriptStartFrame = 0.0f;
        motion.scriptEndFrame = 0.0f;
        motion.forcedFrameTime = -1.0f;
    }
    const std::int32_t result = ApplyBasePose(motion, animationId);
    motion.motionFlags &= ~0x80U;
    motion.pendingAnimId = 0;
    if (deferred) {
        motion.pendingTurnEulerDelta = savedTurn;
        if (savedMirror)
            motion.motionFlags |= 0x80U;
        motion.scriptStartFrame = savedStart;
        motion.scriptEndFrame = savedEnd;
        motion.forcedFrameTime = savedForced;
    }
    return result;
}

// GetRandomSoundEffectForAnim: picks one of the three commands of a random-sound event with the shared environment PRNG.
std::uint32_t GetRandomSoundEffectForAnim(std::uint16_t animationId, float first, float last) {
    const std::byte *const event = MOTN_FindFrameEvent(animationId, 49U, first, last, 1U);
    if (!event)
        return 0;
    g_motnSoundBank = static_cast<std::uint8_t>(event[3]);
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const auto choice = static_cast<std::int32_t>(static_cast<float>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125f * 3.0f);
    std::uint32_t command{};
    std::memcpy(&command, event + 4U * static_cast<std::size_t>(choice + 1), sizeof(command));
    return command;
}

// GetSoundCommandForMotion.
std::uint32_t GetSoundCommandForMotion(std::uint16_t animationId, float first, float last) {
    const std::byte *const event = MOTN_FindFrameEvent(animationId, 4U, first, last, 1U);
    if (!event)
        return 0;
    g_motnSoundBank = static_cast<std::uint8_t>(event[3]);
    std::uint32_t command{};
    std::memcpy(&command, event + 4, sizeof(command));
    return command;
}

// The mainMotionUpdate sound-command selection (VF combat only).
void SelectMotionSoundCommand(MotionInformationWork &motion, float previousTime, float previousFrameTimer) {
    const float currentTime = motion.frameTimer + motion.residualFrameTime;
    const std::uint16_t animationId = motion.currentAnimId;
    if (previousTime <= currentTime) {
        motion.soundCommand = static_cast<std::int32_t>(GetSoundCommandForMotion(animationId, previousTime, currentTime));
        if (!motion.soundCommand)
            motion.soundCommand = static_cast<std::int32_t>(GetRandomSoundEffectForAnim(animationId, previousTime, currentTime));
    } else {
        motion.soundCommand = static_cast<std::int32_t>(GetSoundCommandForMotion(animationId, previousTime, motion.frameEnd));
        if (!motion.soundCommand)
            motion.soundCommand = static_cast<std::int32_t>(GetSoundCommandForMotion(animationId, motion.frameStart, currentTime));
    }
    if (motion.soundCommand)
        return;
    constexpr std::uint16_t kWalkSp2Loop = 1374;    // MN_AKI_AKI_WALK_SP2_LP
    constexpr std::uint16_t kStandFoldedArms = 903; // MN_AKI_AKI_TATI_IWA_L
    std::uint32_t command = 0;
    if (g_motnBasePoseOnly && motion.requestedAnimId == kWalkSp2Loop && animationId == kStandFoldedArms && motion.interpolationMode) {
        const float previousEnd = previousFrameTimer + motion.smoothedFrameDelta;
        const std::byte *event = MOTN_FindFrameEvent(kWalkSp2Loop, 4U, previousFrameTimer, previousEnd, 1U);
        if (event) {
            g_motnSoundBank = static_cast<std::uint8_t>(event[3]);
            std::memcpy(&command, event + 4, sizeof(command));
        }
        motion.soundCommand = static_cast<std::int32_t>(command);
        if (!command && previousEnd > motion.previousFrameEnd) {
            event = MOTN_FindFrameEvent(kWalkSp2Loop, 4U, motion.previousFrameStart, (motion.previousFrameStart + previousEnd) - motion.previousFrameEnd, 1U);
            if (event) {
                g_motnSoundBank = static_cast<std::uint8_t>(event[3]);
                std::memcpy(&command, event + 4, sizeof(command));
            } else {
                command = 0;
            }
            motion.soundCommand = static_cast<std::int32_t>(command);
        }
    }
    if (motion.soundCommand)
        return;
    const std::uint16_t requested = motion.requestedAnimId;
    if ((requested == kWalkSp2Loop || static_cast<std::uint16_t>(requested - 603U) <= 7U) && animationId == kStandFoldedArms && motion.interpolationMode &&
        (motion.motionFlags & 0x10000U) == 0) {
        if (const std::byte *const event = MOTN_FindFrameEvent(kWalkSp2Loop, 4U, 0.0f, motion.previousPlaybackRate, 1U)) {
            std::memcpy(&command, event + 4, sizeof(command));
            g_motnSoundBank = static_cast<std::uint8_t>(event[3]);
        }
        motion.soundCommand = static_cast<std::int32_t>(command);
    }
}

// MOTN_UpdateRootMotionDelta.
void MOTN_UpdateRootMotionDelta(MotionInformationWork &motion) {
    const MotionNodeRuntime &root = *static_cast<MotionNodeRuntime *>(motion.activeNodes);
    motion.previousRootPreTranslate = motion.rootPreTranslate;
    motion.rootWorldPosition = {root.transform->rows[3].x, root.transform->rows[3].y, root.transform->rows[3].z};
    motion.rootMotionDelta = {};
    if (motion.entityEuler.x || motion.entityEuler.z || (motion.motionFlags & 0x1000000U) != 0)
        return;
    if ((motion.motionFlags & 0x10U) != 0) {
        motion.rootPreTranslate.x = motion.rootWorldPosition.x;
        motion.rootPreTranslate.z = motion.rootWorldPosition.z;
    }
    motion.rootMotionDelta.x = motion.rootWorldPosition.x - motion.rootPreTranslate.x;
    motion.rootMotionDelta.z = motion.rootWorldPosition.z - motion.rootPreTranslate.z;
}

// CalculateDifferenceResult: spreads a pending turn over the clip, adding the portion due at timer to next.y.
void CalculateDifferenceResult(const Vector3i &delta, Vector3i &next, float start, float timer, float end) {
    const float elapsed = timer - start;
    const float half = (end - start) * 0.5f;
    const auto turn = static_cast<std::int16_t>(delta.y);
    std::int32_t step = 0;
    if (half <= elapsed)
        step = static_cast<std::int32_t>(static_cast<float>(turn) - (((half + half) - elapsed) * static_cast<float>(turn)) / (half + half));
    else
        step = static_cast<std::int32_t>((static_cast<float>(turn / 2) * elapsed) / half);
    next.y += step;
}



constexpr std::array<std::uint16_t, 16> kMotnSequenceLiftValues{
    0x0000, 0xace7, 0x2f7c, 0x363f, 0x363f, 0x318f, 0xb4d1, 0xb4d1, 0x34bc, 0xb408, 0xb455, 0x3140, 0xace7, 0xace7, 0x2f7c, 0x0000};
constexpr std::array<std::uint8_t, 5> kMotnSequenceLiftFrames{0, 15, 21, 30, 36};
constexpr std::array<std::uint8_t, 16> kMotnSequenceLiftTypes{0xff, 0xff, 0x00, 0x00, 0xf1, 0x00, 0xec, 0xf1, 0x00, 0xec, 0xf9, 0x00, 0xf6, 0x00, 0x00, 0xc4};
MotnKeyframeChannel g_motnSequenceLiftCurve{};
std::int32_t g_motnBlendAngleDifference = 0; // g_temp_angle_diff
float g_motnBlendAngleSine = 0.0f;           // g_temp_normalised_ang

std::uint32_t g_motnRawIKFlag = 0; // g_bIKFlag
std::uint32_t g_motnRawParentUnrotated = 0;
Matrix4x4 g_motnRawLookEnd{};   // g_tmp_lerped_end_xform
Matrix4x4 g_motnRawLookBlend{}; // g_tmp_xform_0

void MOTN_EvaluateRawBlendRow(
    MotionNodeRuntime &source, MotionNodeRuntime &row, MotnRawFrameContext &context, const std::int16_t *current, const std::int16_t *next);

} // namespace

float g_maxHandRotationBlend = 0.5f;

void MOTN_FootPlantingCallback(MotionInformationWork *motion, void *) {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    const bool previousApplied = g_ryoFootPlantingApplied != 0;
    g_ryoFootPlantingApplied = 0;
    if (!ryo) {
        g_ryoFootGroundHeights = {0.0f, 0.0f, 0.0f};
        return;
    }
    CharacterComponentNode *const cpctComponent = HLib_GetCharacterComponent(ryo, SHEN_NAME(CPCT));
    CharacterComponentNode *const ccowComponent = HLib_GetCharacterComponent(ryo, SHEN_NAME(CCOW));
    const auto *const playerControl = cpctComponent ? reinterpret_cast<const PlayerControlWork *>(cpctComponent + 1) : nullptr;
    const auto *const ccow = ccowComponent ? reinterpret_cast<const CharacterCollisionWork *>(ccowComponent + 1) : nullptr;
    if (!playerControl || !playerControl->footPlantingEnabled || !ccow || (ccow->stateFlags & 0x80U) != 0 || VFMA_IsFighting() ||
        IsFootPlantingExcludedAnimation(motion->currentAnimId) || ccow->changed) {
        const float y = GetPositionForEntity(ryo->entity.node).y;
        g_ryoFootGroundHeights = {y, 0.0f, y};
        return;
    }

    const auto mode = static_cast<std::int8_t>(motion->interpolationMode);
    const bool activeOnly = mode <= 0 || (mode > 4 && static_cast<std::uint8_t>(mode - 5) > 4U);
    MotionNodeRuntime *const activeLeft = FindRuntimeNode(static_cast<MotionNodeRuntime *>(motion->activeNodes), 26U);
    MotionNodeRuntime *const activeRight = FindRuntimeNode(static_cast<MotionNodeRuntime *>(motion->activeNodes), 33U);
    if (!activeLeft || !activeRight)
        return;
    MotionNodeRuntime *blendLeft = nullptr;
    MotionNodeRuntime *blendRight = nullptr;
    Vector3 leftTarget{activeLeft->effectorPosition.x, activeLeft->effectorPosition.y, activeLeft->effectorPosition.z};
    Vector3 rightTarget{activeRight->effectorPosition.x, activeRight->effectorPosition.y, activeRight->effectorPosition.z};
    if (!activeOnly) {
        auto *const blendRows = static_cast<MotionNodeRuntime *>(mode == 4 ? motion->transitionNodes : motion->blendTargetNodes);
        blendLeft = FindRuntimeNode(blendRows, 26U);
        blendRight = FindRuntimeNode(blendRows, 33U);
        if (!blendLeft || !blendRight)
            return;
        leftTarget = BlendFootPlantingTarget(*blendLeft, *activeLeft);
        rightTarget = BlendFootPlantingTarget(*blendRight, *activeRight);
    }

    FindHighestPointBelow100ForIKMotion(leftTarget, 0.06f, 0.06f, g_ryoFootGroundHeights.x);
    const float leftY = SolveRyoFootPlantHeight(leftTarget.y, g_ryoFootGroundHeights.x, previousApplied, g_ryoFootPositions.y, g_ryoLeftFootPreviousRawY);
    FindHighestPointBelow100ForIKMotion(rightTarget, 0.06f, 0.06f, g_ryoFootGroundHeights.z);
    const float rightY = SolveRyoFootPlantHeight(rightTarget.y, g_ryoFootGroundHeights.z, previousApplied, g_ryoFootPositions.x, g_ryoRightFootPreviousRawY);
    g_ryoFootPlantingApplied = 1;
    activeLeft->effectorPosition.y = leftY;
    activeRight->effectorPosition.y = rightY;
    if (!activeOnly) {
        blendLeft->effectorPosition.y = leftY;
        blendRight->effectorPosition.y = rightY;
    }
}

// mainMotionUpdate: one MOTI evaluation per MOTM tick.
// Returns the pose-evaluated byte (MOTI+0xAD) that MOTM_Callback hands to NPCLogger slot 5, or 0 on the early and culled exits.
std::int32_t mainMotionUpdate(MotionInformationWork &motion) {
    g_pMotion = &motion;
    std::uint8_t savedStage = 0;
    if (motion.externalTransform) {
        savedStage = motion.poseEvaluationStage;
        motion.poseEvaluationStage = 0;
    }
    const std::uint32_t flags = motion.motionFlags;
    if ((flags & 0x1000000U) != 0)
        motion.frameContactOverlay = 0;
    motion.soundCommand = 0;
    g_motnIgnoreArmIK = 0;
    if (!motion.activeNodes)
        return 0;
    const std::uint16_t pending = motion.pendingAnimId;
    if (pending == 0xffffU) {
        motion.motionFlags = flags | 0x10000000U;
        return 0;
    }
    if (pending) {
        if (const MotnAnimationHeader *const header = MOTN_GetDataForAnimId(pending); header && header->sourceRig == 0xffffU)
            return 0;
    }
    MOTN_CaptureTrackedNodePosition(motion);
    if (static_cast<std::uint32_t>(motion.basePoseTemplateIndex) > 20U)
        motion.basePoseTemplateIndex = 0;
    motion.rootAngleFlags &= ~0x20U;
    const std::int32_t basePose = motion.basePoseTemplateIndex;
    if (basePose != motion.previousBasePoseTemplateIndex) {
        if ((motion.motionFlags & 1U) == 0) {
            if (!motion.pendingAnimId)
                motion.pendingAnimId = motion.currentAnimId;
            motion.rootAngleFlags |= 0x20U;
            motion.motionFlags |= 0x10000000U;
        }
        motion.previousBasePoseTemplateIndex = basePose;
        if (static_cast<std::uint32_t>(basePose - 16) <= 2U || basePose == 20)
            motion.rootAngleFlags |= 0x80U;
        else
            motion.rootAngleFlags &= ~0x80U;
    }
    motion.motionFlags &= ~0x202U;
    motion.blendScratch[3] =
        (motion.rootAngleFlags & 0x200000U) != 0 ? std::min(1.0f, motion.blendScratch[3] + 0.0625f) : std::max(0.0f, motion.blendScratch[3] - 0.0625f);
    motion.rootMotionDelta = {};
    if (motion.poseEvaluationStage == 3) {
        motion.poseEvaluationSucceeded = IsVectorWithinRadius(motion.rootPreTranslate, 2.0f);
        if (!motion.poseEvaluationSucceeded) {
            motion.updateMode = 3;
            return 0;
        }
    }
    motion.rootTransformCache = workTransformMatrix;
    if (MOTF_IsAnimationInvalidated(motion.currentAnimId)) {
        if (!motion.pendingAnimId)
            motion.pendingAnimId = kBasePoseAnimationIds[motion.basePoseTemplateIndex];
        motion.motionFlags |= 0x10000000U;
    }
    if (!motion.pendingAnimId && !MOTN_GetDataForAnimId(motion.currentAnimId))
        motion.pendingAnimId = kBasePoseAnimationIds[motion.basePoseTemplateIndex];
    g_motnLeftKneeValid = 0;
    g_motnRightKneeValid = 0;
    g_motnLeftAnkleValid = 0;
    g_motnRightAnkleValid = 0;
    if (auto *const activeRows = static_cast<MotionNodeRuntime *>(motion.activeNodes)) {
        if (MotionNodeRuntime *const knee = CaptureAnkleScratch(activeRows, 15, g_motnRightKneeValid, g_motnRightKnee))
            CaptureAnkleScratch(knee, 17, g_motnRightAnkleValid, g_motnRightAnkle);
        if (MotionNodeRuntime *const knee = CaptureAnkleScratch(activeRows, 9, g_motnLeftKneeValid, g_motnLeftKnee))
            CaptureAnkleScratch(knee, 11, g_motnLeftAnkleValid, g_motnLeftAnkle);
    }
    const std::int32_t playResult = PlayMotionForRyo(motion, motion.pendingAnimId);
    ClearEvaluatedFlags(static_cast<MotionNodeRuntime *>(motion.secondaryNodes));
    ClearEvaluatedFlags(static_cast<MotionNodeRuntime *>(motion.transitionNodes));
    ClearEvaluatedFlags(static_cast<MotionNodeRuntime *>(motion.blendTargetNodes));
    ClearEvaluatedFlags(static_cast<MotionNodeRuntime *>(motion.activeNodes));
    if ((motion.motionFlags & 0x10000U) != 0) {
        if (!MOTN_GetDataForAnimId(motion.requestedAnimId))
            motion.motionFlags &= ~0x10000U;
        if (motion.updateMode)
            motion.motionFlags &= ~0x10000U;
    }
    g_currentMotionUpdateMode = motion.updateMode;
    motion.updateMode = 0;
    MOTN_UpdateFrameTimer(motion);
    const Vector3i savedEntityEuler = motion.entityEuler;
    if (motion.activeTurnEulerDelta.y)
        CalculateDifferenceResult(motion.activeTurnEulerDelta, motion.entityEuler, motion.frameStart, motion.frameTimer, motion.frameEnd);
    const Vector3i savedNextEntityEuler = motion.nextEntityEuler;
    if (motion.previousTurnEulerDelta.y)
        CalculateDifferenceResult(
            motion.previousTurnEulerDelta, motion.nextEntityEuler, motion.previousFrameStart, motion.previousFrameTimer, motion.previousFrameEnd);
    const float previousTime = motion.residualFrameTime == 0.0f ? motion.frameTimer : motion.residualFrameTime + motion.frameStart;
    const std::uint32_t evaluationFlags = motion.motionFlags;
    const float previousFrameTimer = motion.previousFrameTimer;
    if ((evaluationFlags & 4U) != 0)
        motion.previousAnimId = 0xffffU;
    if ((evaluationFlags & 0x1000000U) != 0)
        motion.frameTimerRef = -1.0f;
    if (!motion.secondaryAnimId && motion.queuedSecondaryAnimId) {
        motion.secondaryBlendFrames = motion.queuedSecondaryBlendFrames;
        MOTN_PlaySecondaryMotion(motion, motion.queuedSecondaryAnimId, motion.queuedSecondaryNodeId, motion.queuedSecondaryStartFrame,
            motion.queuedSecondaryEndFrame, motion.queuedSecondaryFlags);
        motion.queuedSecondaryAnimId = 0;
        motion.queuedSecondaryNodeId = 0;
        motion.queuedSecondaryStartFrame = 0.0f;
        motion.queuedSecondaryEndFrame = 0.0f;
        motion.queuedSecondaryFlags = 0;
    }
    if (motion.secondaryAnimId) {
        if (MOTN_GetDataForAnimId(motion.secondaryAnimId))
            MOTN_UpdateSecondaryMotion(motion);
        else
            MOTN_ClearSecondaryMotion(motion);
    }
    if ((motion.motionFlags & 0x10000U) == 0)
        motion.motionFlags &= ~0x40000000U;
    if (playResult) {
        motion.poseEvaluationSucceeded = MOTN_EvaluateRawMotion(motion);
        if (!motion.poseEvaluationSucceeded) {
            const auto stage = static_cast<std::int8_t>(motion.poseEvaluationStage);
            if (stage >= 2) {
                motion.updateMode = 2;
                return 0;
            }
            if (stage == 1)
                motion.updateMode = 1;
        }
    } else {
        const float amount = motion.interpolationAmount;
        bool resolved = false;
        if (amount != 0.0f && (motion.rootAngleFlags & 0x800000U) == 0 && (motion.motionFlags & 0x100U) != 0 && IsRyoWalkLoopAnimation(motion.currentAnimId)) {
            motion.interpolationMode = 4;
            motion.motionFlags &= ~0x10000U;
            resolved = true;
        }
        if (!resolved) {
            if (amount != 0.0f && motion.lookupAnimId && motion.transitionNodes) {
                motion.motionFlags &= ~0x10000U;
                motion.interpolationMode = 4;
            } else if ((motion.motionFlags & 0x10000U) == 0 || static_cast<MotionNodeRuntime *>(motion.blendTargetNodes)->nodeId == 0xff) {
                motion.interpolationMode = 0;
            } else {
                const std::uint8_t requested = motion.requestedInterpolationMode;
                if ((requested == 4 && (!motion.lookupAnimId || !motion.transitionNodes)) || requested == 10 || (requested == 11 && !g_motnCubicScratch))
                    motion.interpolationMode = 1;
                else
                    motion.interpolationMode = requested;
            }
        }
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        bool evaluated = false;
        switch (motion.interpolationMode) {
        case 0:
            motion.motionFlags &= ~0x10000U;
            evaluated = MOTN_EvaluateActiveMotion(motion);
            break;
        case 2:
        case 5:
        case 6:
        case 9:
            evaluated = MOTN_EvaluatePreviousPoseBlend(motion);
            break;
        case 3:
            evaluated = MOTN_EvaluateFrameCarryBlend(motion);
            break;
        case 4:
            evaluated = MOTN_EvaluateTransitionRowsBlend(motion);
            break;
        case 7:
        case 10:
        case 11:
            evaluated = MOTN_EvaluateCubicSequenceBlend(motion);
            break;
        case 8:
            evaluated = MOTN_EvaluateStandardBlend(motion);
            if ((motion.motionFlags & 0x10000U) == 0 && motion.deferredPrimaryAnimId) {
                motion.pendingAnimId = motion.deferredPrimaryAnimId;
                motion.deferredPrimaryAnimId = 0;
                motion.blendMode = 1;
            }
            break;
        default:
            evaluated = MOTN_EvaluateStandardBlend(motion);
            break;
        }
        Matrix4x4::Pop(1);
        if (!evaluated)
            return 0;
    }
    const bool slowRamp = (motion.rootAngleFlags & 0x1000000U) != 0;
    if (motion.worldTargets[3])
        motion.leftAnkleTargetBlendFrames = slowRamp ? std::min(motion.leftAnkleTargetBlendFrames + 1, 8) : 8;
    else if (motion.leftAnkleTargetBlendFrames > 0)
        motion.leftAnkleTargetBlendFrames = slowRamp ? motion.leftAnkleTargetBlendFrames - 1 : 0;
    if (motion.worldTargets[4])
        motion.rightAnkleTargetBlendFrames = slowRamp ? std::min(motion.rightAnkleTargetBlendFrames + 1, 8) : 8;
    else if (motion.rightAnkleTargetBlendFrames > 0)
        motion.rightAnkleTargetBlendFrames = slowRamp ? motion.rightAnkleTargetBlendFrames - 1 : 0;
    if (motion.secondaryAnimId) {
        if (MaintainFrameRate(motion.secondaryFrameTimer, motion.secondaryFrameTimerRef, motion.secondaryResidualFrameTime,
                motion.secondaryLookupAnimId == 0xffffU, false, motnGetRootMotionEndMarker(motion.secondaryAnimId) != 0, motion.secondaryFrameStart,
                motion.secondaryFrameEnd, MOTN_GetSmoothedFrameDelta(motion.secondaryPlaybackRate))) {
            motion.motionFlags |= 0x200U;
            if ((motion.motionFlags & 0x4000000U) != 0)
                MOTN_ClearSecondaryMotion(motion);
        }
        if (motion.secondaryBlendCountdown) {
            --motion.secondaryBlendCountdown;
            if (!motion.secondaryBlendCountdown)
                MOTN_ClearSecondaryMotion(motion);
        }
    }
    auto *const root = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    Vector3 viewRoot{};
    Matrix4x4::TransformPoint({root->transform->rows[3].x, root->transform->rows[3].y, root->transform->rows[3].z}, viewRoot);
    const float blend = std::sqrt((viewRoot.y * viewRoot.y + viewRoot.x * viewRoot.x) + viewRoot.z * viewRoot.z) >= 10.0f ?
        std::max(0.0f, motion.handRotationBlend - 0.050000001f) :
        std::min(motion.handRotationBlend + 0.050000001f, g_maxHandRotationBlend);
    motion.handRotationBlend = blend;
    if (blend <= 0.0f) {
        motion.rightHandRotationAngle = 0;
        motion.leftHandRotationAngle = 0;
    } else {
        MotionNodeRuntime *const rightWrist = FindRowFrom(root, 7);
        if (rightWrist && (rightWrist->bindPositionPacked[1] & 2U) != 0)
            motion.rightHandRotationAngle = AdjustBoneRotation(*rightWrist, blend, static_cast<std::int16_t>(motion.rightHandRotationAngle));
        MotionNodeRuntime *const leftWrist = FindRowFrom(static_cast<MotionNodeRuntime *>(motion.activeNodes), 13);
        if (leftWrist && (leftWrist->bindPositionPacked[1] & 2U) != 0)
            motion.leftHandRotationAngle = AdjustBoneRotation(*leftWrist, motion.handRotationBlend, static_cast<std::int16_t>(motion.leftHandRotationAngle));
    }
    bool allEvaluated = true;
    for (MotionNodeRuntime *row = static_cast<MotionNodeRuntime *>(motion.activeNodes); row->nodeId != 0xff; ++row) {
        if ((row->bindPositionPacked[1] & 2U) == 0)
            allEvaluated = false;
    }
    if (allEvaluated) {
        if (std::fabs(1.0f - motion.currentMotionScale) > 0x1p-23f) {
            Matrix4x4_Push();
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(motion.rootPreTranslate.x, motion.rootPreTranslate.y, motion.rootPreTranslate.z);
            Matrix4x4::SetViewMatrixFromEuler(motion.entityEuler.y, motion.entityEuler.x, motion.entityEuler.z);
            Matrix4x4_Push();
            Matrix4x4::Scale(motion.currentMotionScale, motion.currentMotionScale, motion.currentMotionScale);
            const Matrix4x4 scaledFrame = workTransformMatrix;
            Matrix4x4::Pop(1);
            Matrix4x4::InvertInPlace(nullptr);
            for (MotionNodeRuntime *row = static_cast<MotionNodeRuntime *>(motion.activeNodes); row->nodeId != 0xff; ++row) {
                Matrix4x4_Push();
                Matrix4x4::Multiply(row->transform);
                *MotionRowMatrix(*row) = workTransformMatrix;
                Matrix4x4::Pop(1);
            }
            workTransformMatrix = scaledFrame;
            for (MotionNodeRuntime *row = static_cast<MotionNodeRuntime *>(motion.activeNodes); row->nodeId != 0xff; ++row) {
                Matrix4x4_Push();
                Matrix4x4::Multiply(row->transform);
                *MotionRowMatrix(*row) = workTransformMatrix;
                Matrix4x4::Pop(1);
            }
            Matrix4x4::Pop(1);
        }
        const float currentScale = motion.currentMotionScale;
        const float targetScale = motion.targetMotionScale;
        if (std::fabs(currentScale - targetScale) > 0x1p-23f && std::fabs(currentScale) > 0x1p-23f) {
            const float ratio = targetScale / currentScale;
            if (MotionNodeRuntime *const head = FindRowFrom(static_cast<MotionNodeRuntime *>(motion.activeNodes), 4)) {
                Matrix4x4_Push();
                workTransformMatrix = *MotionRowMatrix(*head);
                Matrix4x4_Push();
                Matrix4x4::Scale(ratio, ratio, ratio);
                const Matrix4x4 scaledHead = workTransformMatrix;
                Matrix4x4::Pop(1);
                Matrix4x4::InvertInPlace(nullptr);
                Matrix4x4_RecursiveRotateMatrix(*head, scaledHead);
                Matrix4x4::Pop(1);
            }
        }
    }
    motion.previousPlaybackRate = motion.playbackRate;
    motion.entityEuler = savedEntityEuler;
    motion.nextEntityEuler = savedNextEntityEuler;
    MOTN_UpdateRootMotionDelta(motion);
    motion.previousEntityEuler = motion.entityEuler;
    motion.lastTurnEulerDelta = motion.activeTurnEulerDelta;
    motion.previousNodes = motion.activeNodes;
    const std::uint32_t extracted = MOTN_ExtractEulerAngle(motion, 2);
    motion.extractedYaw = static_cast<std::int16_t>(extracted);
    motion.extractedYawAux = static_cast<std::int16_t>(extracted >> 16U);
    motion.spineYaw = MOTN_ExtractSpineYaw(motion);
    motion.trackedNodeIndex = 10;
    if (Matrix4x4 *const external = motion.externalTransform) {
        auto *const rows = static_cast<MotionNodeRuntime *>(motion.activeNodes);
        if (motion.externalTransformAnchorNodeId == -1) {
            const Vector3 translation{external->rows[3].x, external->rows[3].y, external->rows[3].z};
            Matrix4x4_Push();
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(translation.x, translation.y, translation.z);
            Matrix4x4::InvertInPlace(nullptr);
            for (MotionNodeRuntime *row = rows; row->nodeId != 0xff; ++row) {
                Matrix4x4_Push();
                Matrix4x4::Multiply(row->transform);
                *MotionRowMatrix(*row) = workTransformMatrix;
                Matrix4x4::Pop(1);
            }
            Matrix4x4::Pop(1);
        } else {
            MotionNodeRuntime *anchor = FindTrackedRow(rows, static_cast<std::uint32_t>(motion.externalTransformAnchorNodeId));
            if (!anchor)
                anchor = rows;
            AddFloatToIntArray(*rows, {-anchor->transform->rows[3].x, -anchor->transform->rows[3].y, -anchor->transform->rows[3].z});
        }
        ApplyBoneTransformRecursive(*rows, motion.externalTransform);
        motion.poseEvaluationStage = savedStage;
        motion.poseEvaluationSucceeded = 1;
    }
    if (g_vfPlayerAggroCombat)
        SelectMotionSoundCommand(motion, previousTime, previousFrameTimer);
    motion.rootAngleFlags &= ~0x800U;
    return static_cast<std::int8_t>(motion.poseEvaluationSucceeded);
}

namespace motion_detail {} // namespace

void UNFO_Init(MotionInformationWork &motion) {


    std::memset(&motion, 0, 0x3f8);
    motion.motionFlags = 0x10004141U;
    motion.blendMode = 1;
    motion.requestedInterpolationMode = 1;
    motion.interpolationMode = 0;
    motion.poseEvaluationStage = 1;
    motion.blendFrameCount = 5;
    motion.walkBlendAnimId = 724;
    motion.secondaryBlendFrames = 8;
    motion.secondaryBlendFramesPrevious = 8;
    motion.forcedFrameTime = -1.0f;
    motion.trackedNodeIndex = 10;
    motion.nextBasePoseAnimIndex = 2;
    motion.headLookAngleCommand = 2048;
    const float initialRate = static_cast<float>(Shenmue_nNumRenderedFrames) * 0.5f;
    motion.playbackRate = initialRate;
    motion.previousPlaybackRate = initialRate;
    motion.smoothedFrameDelta = initialRate;
    motion.secondaryPlaybackRate = initialRate;
    motion.handRotationBlend = g_maxHandRotationBlend;
    motion.previousFootContactTargetIndex[0] = -1;
    motion.previousFootContactTargetIndex[1] = -1;
    motion.contactTargetRadiusBias = 0.02f;
    motion.updateMode = 4;
    motion.currentMotionScale = 1.0f;
    motion.targetMotionScale = 1.0f;
    motion.baseMotionScale = 1.0f;
    motion.motionScaleFrames = 10;
    motion.contactIKEnabled = 1;
    motion.rootTransformBlend = 0.5f;
    motion.pendingAnimId = 89;
}

namespace motion_detail {

void AllocateMotionNodes(MotionNodeRuntime *&nodes, std::int64_t &cookie, Matrix4x4 **matrixPool) {
    g_motnRowBytesInUse += 0x26bc;
    if (matrixPool) {
        g_motnRowBytesInUse += 0xb40;
        *matrixPool = static_cast<Matrix4x4 *>(HLib_AllocMemBlock(0xb40, SHEN_NAME(UNFO)));
        std::memset(*matrixPool, 0, 0xb40);
    }
    nodes = static_cast<MotionNodeRuntime *>(HLib_AllocMemBlock(0x26bc, SHEN_NAME(UNFO)));
    std::memset(nodes, 0, 0x26bc);
    cookie = reinterpret_cast<std::int64_t>(reinterpret_cast<std::byte *>(nodes) + 0x13b4);
}

void MOTN_FreeNodeRowsAndMatrixPool(void *&rows, std::int64_t &cookie, void **matrixPool) {
    if (matrixPool && *matrixPool) {
        g_motnRowBytesInUse -= 0xb40;
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(*matrixPool), 0);
        *matrixPool = nullptr;
    }
    if (rows) {
        g_motnRowBytesInUse -= 0x26bc;
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(rows), 0);
    }
    rows = nullptr;
    cookie = 0;
}

void FreeMotionBlock(void *block) {
    if (block)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(block), 0);
}

#pragma pack(push, 1)
#pragma pack(pop)

constexpr std::array<std::int32_t, 28> kSequenceEndpointNodeMap{
    0,
    5,
    7,
    9,
    10,
    12,
    13,
    15,
    16,
    18,
    24,
    25,
    27,
    31,
    32,
    34,
    0,
    3,
    6,
    11,
    17,
    21,
    26,
    33,
    38,
    42,
    48,
    0,
};
constexpr std::array<std::int32_t, 6> kSequenceStateNodeMap{
    3,
    6,
    11,
    17,
    26,
    33,
};

template <typename T> void StoreSequenceConstraintField(std::byte *output, std::size_t offset, const T &value) {
    std::memcpy(output + offset, &value, sizeof(value));
}

void DecodeSequenceConstraintRecord(const SequenceNodeConstraintRecord &record, std::byte *output) {
    const std::int32_t endpointNode0 = kSequenceEndpointNodeMap[record.endpointNode[0]];
    const std::int32_t endpointNode1 = kSequenceEndpointNodeMap[record.endpointNode[1]];
    const std::int32_t stateNode0 = kSequenceStateNodeMap[record.stateNode[0]];
    const std::int32_t stateNode1 = kSequenceStateNodeMap[record.stateNode[1]];
    const float endpointRadius0 = F16ToF32(record.endpointRadius[0]);
    const float endpointRadius1 = F16ToF32(record.endpointRadius[1]);
    const float stateRadius0 = F16ToF32(record.stateRadiusBias[0]);
    const float stateRadius1 = F16ToF32(record.stateRadiusBias[1]);

    StoreSequenceConstraintField(output, 0x00, stateNode0);
    StoreSequenceConstraintField(output, 0x28, stateRadius0);
    StoreSequenceConstraintField(output, 0x2d, record.stateMode[0]);
    StoreSequenceConstraintField(output, 0x30, stateNode1);
    StoreSequenceConstraintField(output, 0x58, stateRadius1);
    StoreSequenceConstraintField(output, 0x5d, record.stateMode[1]);
    StoreSequenceConstraintField(output, 0x60, record.sourceId);
    StoreSequenceConstraintField(output, 0x64, endpointNode0);
    StoreSequenceConstraintField(output, 0x74, endpointRadius0);
    StoreSequenceConstraintField(output, 0x7c, record.sourceId);
    StoreSequenceConstraintField(output, 0x80, endpointNode1);
    StoreSequenceConstraintField(output, 0x90, endpointRadius1);
}

} // namespace
void MOTM_UpdateSequenceNodeConstraints(const void *records, std::uint32_t count, std::int32_t frameIndex) {
    if ((count != 0 && !records) || frameIndex < 0 || !g_mapControlTask || !g_mapControlTask->task_data_ptr) {
        return;
    }

    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    auto *const scenario =
        control->scenarioTask && control->scenarioTask->task_data_ptr ? reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr) : nullptr;
    if (!scenario) {
        return;
    }

    const auto *const entries = static_cast<const SequenceNodeConstraintRecord *>(records);
    constexpr std::uint32_t kCharacterEntity = SHEN_NAME(CEWP);

    for (void *const bucketHead : control->characterBuckets) {
        for (auto *link = static_cast<CharacterBucketNode *>(bucketHead); link; link = link->next) {
            Character *const character = link->character;
            if (!character || character->entity.name != kCharacterEntity ||
                ((character->flags.value & 1U) == 0U && ((character->flags.value & 2U) == 0U || (character->motionFlags & 1U) == 0U))) {
                continue;
            }

            MotionManagerWork *const manager = MOTM_GetWork(character);
            if (!manager) {
                continue;
            }
            MotionInformationWork &motion = manager->motion;
            motion.nodeOffsetPairCursor = nullptr;

            SCEN_PromoteCharacterIndexNode(control->scenarioTask, character);
            const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
            const CharacterIdLookupNode *characterNode = scenario->characterBuckets[bucket];
            while (characterNode && characterNode->character != character) {
                characterNode = characterNode->bucketNext;
            }
            const std::uint32_t characterId = characterNode ? characterNode->id : 0;

            for (std::uint32_t index = 0; index < count; ++index) {
                const SequenceNodeConstraintRecord &record = entries[index];
                if (record.characterId != characterId) {
                    continue;
                }
                if (frameIndex == record.startFrame) {
                    if (!motion.nodeConstraints) {
                        motion.nodeConstraints = HLib_AllocMemBlock(0x98, SHEN_NAME(UNFO));
                        if (motion.nodeConstraints) {
                            std::memset(motion.nodeConstraints, 0, 0x98);
                        }
                    }
                    if (motion.nodeConstraints) {
                        DecodeSequenceConstraintRecord(record, static_cast<std::byte *>(motion.nodeConstraints));
                    }
                }
                if (frameIndex == record.endFrame) {
                    FreeMotionBlock(motion.nodeConstraints);
                    motion.nodeConstraints = nullptr;
                }
            }
        }
    }
}

void MOTN_ClearSecondaryMotion(MotionInformationWork &motion) {
    motion.secondaryAnimId = 0;
    motion.secondaryNodeId = 0;
    MOTN_FreeNodeRowsAndMatrixPool(motion.secondaryNodes, motion.secondaryNodesCookie, nullptr);
    motion.sourceNodes = nullptr;
}

void MOTN_RequestSecondaryMotionStop(MotionInformationWork &motion) {
    if (!motion.secondaryAnimId || motion.motionControlLock || motion.secondaryBlendCountdown) {
        return;
    }
    motion.secondaryBlendFramesPrevious = motion.secondaryBlendFrames;
    const std::int32_t countdown = static_cast<std::int32_t>(static_cast<float>(motion.secondaryBlendFrames) * motion.secondaryBlendFrameScale);
    motion.secondaryBlendCountdown = static_cast<std::uint8_t>(std::clamp(countdown, 0, 0xff));
    if (!motion.secondaryBlendCountdown)
        MOTN_ClearSecondaryMotion(motion);
}

bool MOTN_PlaySecondaryMotion(
    MotionInformationWork &motion, std::uint16_t animationId, std::int16_t nodeId, float frameStart, float frameEnd, std::uint32_t flags) {
    if (!motion.currentAnimId)
        return false;
    g_pMotion = &motion;
    if (motion.secondaryAnimId) {
        motion.queuedSecondaryNodeId = nodeId;
        motion.queuedSecondaryFlags = flags;
        motion.queuedSecondaryBlendFrames = motion.secondaryBlendFrames;
        motion.queuedSecondaryStartFrame = frameStart;
        motion.queuedSecondaryEndFrame = frameEnd;
        motion.queuedSecondaryAnimId = animationId;
        motion.secondaryBlendFrames = 5;
        if (!motion.motionControlLock && !motion.secondaryBlendCountdown) {
            motion.secondaryBlendFramesPrevious = 5;
            motion.secondaryBlendCountdown = static_cast<std::uint8_t>(static_cast<std::int64_t>(motion.secondaryBlendFrameScale * 5.0f));
        }
        return true;
    }
    if (!motion.activeNodes)
        return false;
    if (!MOTN_GetDataForAnimId(animationId)) {
        animationId = 1;
        if (!MOTN_GetDataForAnimId(animationId))
            return false;
    }
    const std::int32_t integerFrameCount = MOTN_GetFrameCountFromAnimId(animationId);
    if (integerFrameCount <= 0)
        return false;
    float frameCount = static_cast<float>(integerFrameCount);
    if (MotionHasRootEndMarker(animationId))
        frameCount = static_cast<float>(static_cast<double>(integerFrameCount) - 0.0001);

    motion.secondaryNodeId = nodeId;
    motion.secondaryAnimId = animationId;
    std::uint16_t lookupAnimId = 0;
    FindAnimationExtraWord(animationId, 1, lookupAnimId);
    motion.secondaryLookupAnimId = lookupAnimId;
    motion.secondaryFrameStart = frameStart;
    if (motion.secondaryFrameStart >= frameCount)
        motion.secondaryFrameStart = frameCount - 1.0f;
    motion.secondaryFrameTimer = motion.secondaryFrameStart;
    motion.secondaryFrameEnd = frameEnd;
    if (motion.secondaryFrameStart > frameEnd || frameEnd >= frameCount) {
        motion.secondaryFrameEnd = frameCount - 1.0f;
    }
    const float timerReference = motion.secondaryFrameStart - motion.secondaryPlaybackRate;
    motion.secondaryFrameTimerRef = timerReference < 0.0f ? -1.0f : timerReference;
    motion.blendScratch[0] = 0.0f;
    motion.blendScratch[1] = 0.0f;
    motion.secondaryBlendSeed = motion.secondaryBlendFrames;
    motion.secondaryBlendFramesPrevious = motion.secondaryBlendFrames;

    if (!motion.secondaryNodes) {
        AllocateMotionNodes(reinterpret_cast<MotionNodeRuntime *&>(motion.secondaryNodes), motion.secondaryNodesCookie, nullptr);
    }
    auto *const secondaryNodes = static_cast<MotionNodeRuntime *>(motion.secondaryNodes);
    auto *keyStorage = reinterpret_cast<MotnKeyframeTriplet *>(motion.secondaryNodesCookie);
    std::uint16_t unused{};
    const bool hasArmCue = FindAnimationExtraWord(animationId, 19, unused);
    if (flags == static_cast<std::uint32_t>(hasArmCue))
        motion.motionFlags &= ~0x00100000U;
    else
        motion.motionFlags |= 0x00100000U;
    if (!MOTN_LoadAnimation(motion, secondaryNodes, animationId, static_cast<std::uint16_t>(motion.basePoseTemplateIndex), keyStorage,
            static_cast<MotionNodeRuntime *>(motion.activeNodes))) {
        MOTN_ClearSecondaryMotion(motion);
        return false;
    }
    if ((motion.motionFlags & 0x00100000U) != 0 && (MOTN_GetDataForAnimId(animationId)->sourceRig & 0x8000U) == 0)
        MOTN_IncrementDataOffsets(secondaryNodes);
    motion.motionFlags &= ~0x04000000U;
    motion.secondaryBlendCountdown = 0;
    return true;
}

std::int32_t FIGP_PlayFightAnim_Wrapper(Character *character, std::uint16_t animationId, std::int16_t nodeId, float startFrame, std::uint32_t flags) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager || manager->motion.basePoseTemplateIndex > 20)
        return 0;
    MotionInformationWork &motion = manager->motion;
    if (motion.secondaryAnimId == animationId && motion.secondaryNodeId == nodeId && motion.secondaryFrameStart == startFrame) {
        const bool mirrorMismatch = (motion.motionFlags & 0x00100000U) != 0;
        if (mirrorMismatch == (flags != 0))
            return 0;
    }
    return MOTN_PlaySecondaryMotion(motion, animationId, nodeId, startFrame, -1.0f, flags) ? 1 : 0;
}

namespace motion_detail {

std::int32_t EstimateLookPointSecondaryBlendFrames(MotionInformationWork &motion) {
    const std::uint32_t savedRootAngleFlags = motion.rootAngleFlags;
    const std::uint32_t savedMotionFlags = motion.motionFlags;
    std::int16_t acceleration = motion.turnCurveState[2];
    std::int16_t decelerationOrigin = motion.turnCurveState[3];
    std::int16_t velocity = motion.turnCurveState[1];
    std::int16_t previousTarget = motion.turnCurveState[0];
    std::int16_t current = motion.currentTurnAngle;
    const std::int16_t target = motion.targetTurnAngle;
    std::int16_t output = current;
    std::int32_t frames = 0;
    if ((motion.rootAngleFlags & 0x2000U) == 0) {
        do {
            motnUpdate(motion, {&acceleration, &decelerationOrigin, &velocity, &previousTarget, &current}, target, output, 0);
            current = output;
            if (frames > 100) {
                break;
            }
            ++frames;
            if ((motion.rootAngleFlags & 0x2000U) != 0) {
                break;
            }
        } while (target != current);
    }
    motion.motionFlags = savedMotionFlags;
    motion.rootAngleFlags = savedRootAngleFlags;
    return frames;
}

MotionNodeRuntime *FindLookPointRuntimeRow(MotionInformationWork &motion, std::uint8_t nodeId) {
    auto *row = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    while (row && row->nodeId != 0xffU && row->nodeId != nodeId) {
        ++row;
    }
    return row && row->nodeId == nodeId ? row : nullptr;
}

Vector3 LookPointWorldDeltaToLocal(const Matrix4x4 &matrix, const Vector3 &delta) {
    return {
        matrix.rows[0].x * delta.x + matrix.rows[0].y * delta.y + matrix.rows[0].z * delta.z,
        matrix.rows[1].x * delta.x + matrix.rows[1].y * delta.y + matrix.rows[1].z * delta.z,
        matrix.rows[2].x * delta.x + matrix.rows[2].y * delta.y + matrix.rows[2].z * delta.z,
    };
}

} // namespace

std::uint32_t motCalculateAnimationToPlay(Character *character, const Vector3 *target, bool sinusRig) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager || !target) {
        return 0;
    }
    MotionInformationWork &motion = manager->motion;
    motion.secondaryBlendFrames = 15;

    const float xDelta = target->x - motion.rootWorldPosition.x;
    const float zDelta = target->z - motion.rootWorldPosition.z;
    std::int16_t direction = 0;
    if (std::fabs(xDelta) >= 0.001F || std::fabs(zDelta) >= 0.001F) {
        direction = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-xDelta, -zDelta) - static_cast<std::uint16_t>(motion.entityEuler.y));
    }

    std::int16_t turnRemainder = 0;
    const float absoluteDirection = std::fabs(static_cast<float>(direction));
    if (absoluteDirection > 8192.0F) {
        if (direction >= 0) {
            if (absoluteDirection < 16384.0F) {
                turnRemainder = static_cast<std::int16_t>(direction - 0x2000);
                direction = 0x2000;
            } else {
                turnRemainder = 0x2000;
                direction = static_cast<std::int16_t>(direction - 0x2000);
            }
        } else if (absoluteDirection < 16384.0F) {
            turnRemainder = static_cast<std::int16_t>(-0x2000 - direction);
            direction = -0x2000;
        } else {
            turnRemainder = -0x2000;
            direction = static_cast<std::int16_t>(direction + 0x2000);
        }
    }

    motion.headLookAngleCommand = 0x8066U;
    motion.targetTurnAngle = turnRemainder;
    const std::int32_t estimatedBlendFrames = EstimateLookPointSecondaryBlendFrames(motion) + 5;
    if (estimatedBlendFrames > 15) {
        motion.secondaryBlendFrames = static_cast<std::uint8_t>(estimatedBlendFrames);
    }

    const std::uint32_t leftSide = direction >= 0 ? 1U : 0U;
    if (direction < 0) {
        direction = static_cast<std::int16_t>(-direction);
    }

    std::uint16_t animationId{};
    if (sinusRig) {
        animationId = direction < 0x1000 ? 0x05d0U : direction < 0x3000 ? 0x05d1U : 0x05d2U;
    } else {
        PushMotionMatrix();
        MotionNodeRuntime *const limb = FindLookPointRuntimeRow(motion, leftSide != 0 ? 11U : 17U);
        MotionNodeRuntime *const spine = FindLookPointRuntimeRow(motion, 2U);
        bool useNewAnimations = false;
        if (limb && limb->transform && spine && spine->transform) {
            const Vector3 limbDelta{
                limb->transform->rows[3].x - spine->transform->rows[3].x,
                limb->transform->rows[3].y - spine->transform->rows[3].y,
                limb->transform->rows[3].z - spine->transform->rows[3].z,
            };
            const Vector3 local = LookPointWorldDeltaToLocal(*spine->transform, limbDelta);
            useNewAnimations = std::fabs(local.x) < 0.05F && local.y > 0.1F && std::fabs(local.z) < 0.07F;
        }
        if (!MOTN_GetDataForAnimId(0x0615U)) {
            useNewAnimations = false;
        }

        std::int16_t targetPitch = 0;
        if (spine && spine->transform) {
            const Vector3 targetDelta{
                target->x - spine->transform->rows[3].x,
                target->y - spine->transform->rows[3].y,
                target->z - spine->transform->rows[3].z,
            };
            const Vector3 local = LookPointWorldDeltaToLocal(*spine->transform, targetDelta);
            const float horizontal = std::sqrt(local.z * local.z + local.y * local.y);
            if (std::fabs(local.x) >= 0.001F || std::fabs(horizontal) >= 0.001F) {
                targetPitch = static_cast<std::int16_t>(-static_cast<std::int16_t>(Shenmue_Atan2Fixed(local.x, horizontal)));
            }
        }
        PopMotionMatrix();

        if (static_cast<std::int32_t>(-targetPitch) < 3072) {
            if (useNewAnimations) {
                animationId = direction < 0x1000 ? 0x0615U : direction < 0x3000 ? 0x0616U : 0x0617U;
            } else {
                animationId = direction < 0x1000 ? 0x02daU : direction < 0x3000 ? 0x02dcU : 0x02deU;
            }
        } else {
            animationId = direction < 0x1000 ? 0x05a9U : direction < 0x3000 ? 0x02dbU : 0x02ddU;
        }
    }

    const std::int16_t nodeId = leftSide != 0 ? 7 : 13;
    FIGP_PlayFightAnim_Wrapper(character, animationId, nodeId, 0.0F, leftSide);
    return leftSide;
}

namespace motion_detail {

std::uint32_t FinishLookPointAnimationSelection(Character *character, const Vector3 *target, bool sinusRig) {
    const std::uint32_t result = motCalculateAnimationToPlay(character, target, sinusRig);
    if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
        manager->motion.rootAngleFlags &= 0xffffdffcU;
    }
    return result;
}

} // namespace

std::uint32_t MOTI_UpdateLookPointAnimation(Character *character, const Vector3 *target) {
    return FinishLookPointAnimationSelection(character, target, true);
}

std::uint32_t MOTI_UpdateLookPointAnimationFullBody(Character *character, const Vector3 *target) {
    return FinishLookPointAnimationSelection(character, target, false);
}

void MOTI_StopLookPointAnimation(Character *character) {
    MOTI_BeginSecondaryBlend(character);
    if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
        manager->motion.rootAngleFlags &= 0xffffdffcU;
    }
}

MotionNodeRuntime *MOTN_FindNodeRow(MotionNodeRuntime *row, std::int32_t nodeId) {
    if (!row)
        return nullptr;

    for (;; ++row) {
        if (nodeId == static_cast<std::int8_t>(row->nodeId))
            return row;
        if (row->nodeId == 0xffU)
            return nullptr;
    }
}

Vector3 motGetEntityPos_Or_RotateEntity(Character *character, std::int32_t nodeId) {
    if (!character)
        return {};
    MotionManagerWork *const manager = MOTM_GetWork(character);
    const MotionNodeRuntime *const row = manager ? MOTN_FindNodeRow(static_cast<MotionNodeRuntime *>(manager->motion.activeNodes), nodeId) : nullptr;
    if (!row)
        return GetPositionForEntity(character->entity.node);
    return TransformPointByEntity(character->entity.node, {row->effectorPosition.x, row->effectorPosition.y, row->effectorPosition.z});
}

void MOTI_InitAndAllocPrimaryBuffers(MotionInformationWork &motion) {
    UNFO_Init(motion);
    AllocateMotionNodes(
        reinterpret_cast<MotionNodeRuntime *&>(motion.activeNodes), motion.activeNodesCookie, reinterpret_cast<Matrix4x4 **>(&motion.nodeMatrixPool));
    static_cast<MotionNodeRuntime *>(motion.activeNodes)->nodeId = 0xff;
    AllocateMotionNodes(reinterpret_cast<MotionNodeRuntime *&>(motion.blendTargetNodes), motion.blendTargetNodesCookie, nullptr);
    static_cast<MotionNodeRuntime *>(motion.blendTargetNodes)->nodeId = 0xff;
    motion.secondaryNodes = nullptr;
    motion.secondaryNodesCookie = 0;
    motion.transitionNodesCookie = 0;
    motion.transitionNodes = nullptr;
    motion.previousNodes = nullptr;
    motion.sourceNodes = nullptr;
}


void MOTI_Cleanup(MotionInformationWork &motion) {
    MOTN_FreeNodeRowsAndMatrixPool(motion.activeNodes, motion.activeNodesCookie, &motion.nodeMatrixPool);
    MOTN_FreeNodeRowsAndMatrixPool(motion.blendTargetNodes, motion.blendTargetNodesCookie, nullptr);
    MOTN_FreeNodeRowsAndMatrixPool(motion.transitionNodes, motion.transitionNodesCookie, nullptr);
    MOTN_FreeNodeRowsAndMatrixPool(motion.secondaryNodes, motion.secondaryNodesCookie, nullptr);
}


void MOTI_Free(MotionInformationWork &motion) {
    FreeMotionBlock(motion.nodeConstraints);
    FreeMotionBlock(motion.nodeOffsetPairCursor);
    MOTI_Cleanup(motion);
    MOTI_InitAndAllocPrimaryBuffers(motion);
}



void MOTI_TransferPrimaryRows(MotionInformationWork *source, MotionInformationWork *destination) {
    if (!source) {
        AllocateMotionNodes(reinterpret_cast<MotionNodeRuntime *&>(destination->activeNodes), destination->activeNodesCookie,
            reinterpret_cast<Matrix4x4 **>(&destination->nodeMatrixPool));
        static_cast<MotionNodeRuntime *>(destination->activeNodes)->nodeId = 0xff;
        AllocateMotionNodes(reinterpret_cast<MotionNodeRuntime *&>(destination->blendTargetNodes), destination->blendTargetNodesCookie, nullptr);
        static_cast<MotionNodeRuntime *>(destination->blendTargetNodes)->nodeId = 0xff;
        destination->secondaryNodes = nullptr;
        destination->secondaryNodesCookie = 0;
        destination->transitionNodes = nullptr;
        destination->transitionNodesCookie = 0;
        if (!destination->pendingAnimId)
            destination->pendingAnimId = destination->currentAnimId ? destination->currentAnimId : 89;
        return;
    }
    source->secondaryAnimId = 0;
    source->secondaryNodeId = 0;
    MOTN_FreeNodeRowsAndMatrixPool(source->secondaryNodes, source->secondaryNodesCookie, nullptr);
    source->sourceNodes = nullptr;
    MOTN_FreeNodeRowsAndMatrixPool(source->transitionNodes, source->transitionNodesCookie, nullptr);
    if (destination) {
        destination->nodeMatrixPool = source->nodeMatrixPool;
        destination->activeNodesCookie = source->activeNodesCookie;
        destination->activeNodes = source->activeNodes;
        static_cast<MotionNodeRuntime *>(destination->activeNodes)->nodeId = 0xff;
        destination->blendTargetNodesCookie = source->blendTargetNodesCookie;
        destination->blendTargetNodes = source->blendTargetNodes;
        static_cast<MotionNodeRuntime *>(destination->blendTargetNodes)->nodeId = 0xff;
        destination->motionFlags |= 0x10000000U;
        if (!destination->pendingAnimId)
            destination->pendingAnimId = destination->currentAnimId ? destination->currentAnimId : 89;
    } else {
        MOTN_FreeNodeRowsAndMatrixPool(source->activeNodes, source->activeNodesCookie, &source->nodeMatrixPool);
        MOTN_FreeNodeRowsAndMatrixPool(source->blendTargetNodes, source->blendTargetNodesCookie, nullptr);
    }
    source->activeNodesCookie = 0;
    source->nodeMatrixPool = nullptr;
    source->activeNodes = nullptr;
    source->blendTargetNodesCookie = 0;
    source->blendTargetNodes = nullptr;
    source->previousNodes = nullptr;
}

void MOTI_BeginSecondaryBlend(Character *character) {
    if (!character)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    MotionInformationWork &motion = manager->motion;
    if (motion.motionControlLock || motion.secondaryBlendCountdown)
        return;
    motion.secondaryBlendFramesPrevious = motion.secondaryBlendFrames;
    motion.secondaryBlendCountdown =
        static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(motion.secondaryBlendFrames) * motion.secondaryBlendFrameScale));
}

void MOTI_SetBaseMotionScale(Character *character, float scale) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    manager->motion.baseMotionScale = std::isnan(scale) ? 1.0f : std::clamp(scale, 0.0f, 1.0f);
}

void MOTI_SetBlendFrameCount(Character *character, std::uint16_t frames) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (manager)
        manager->motion.blendFrameCount = frames;
}

std::uint16_t MOTI_GetCurrentAnimationId(Character *character) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    return manager ? manager->motion.currentAnimId : 0;
}

void MOTI_SetTrackedNode(Character *character, std::int32_t nodeId) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        if (character)
            SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    switch (nodeId) {
    case 3:
        manager->motion.trackedNodeIndex = 1;
        break;
    case 6:
        manager->motion.trackedNodeIndex = 2;
        break;
    case 11:
        manager->motion.trackedNodeIndex = 3;
        break;
    case 17:
        manager->motion.trackedNodeIndex = 4;
        break;
    case 21:
        manager->motion.trackedNodeIndex = 5;
        break;
    case 26:
        manager->motion.trackedNodeIndex = 6;
        break;
    case 33:
        manager->motion.trackedNodeIndex = 7;
        break;
    default:
        break;
    }
}

void MOTI_SetSlowAngleRamp(Character *character, bool enabled) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        MapControlData *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    constexpr std::uint32_t kSlowRamp = 0x01000000U;
    if (enabled)
        manager->motion.rootAngleFlags |= kSlowRamp;
    else
        manager->motion.rootAngleFlags &= ~kSlowRamp;
}

bool MOTI_AccessDetailFlag(Character *character, bool query, std::uint32_t value, std::uint32_t &result) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return false;
    if (query) {
        result = manager->motion.contactIKEnabled;
        return true;
    }
    manager->motion.contactIKEnabled = value;
    return false;
}

void MOTM_Cleanup(Character *, void *payload) {
    auto *const manager = static_cast<MotionManagerWork *>(payload);
    if (!manager)
        return;


    if ((manager->flags & 2U) != 0)
        MBST_PrepareMotionManagerCleanup(manager);
    FreeMotionBlock(manager->motion.nodeConstraints);
    manager->motion.nodeConstraints = nullptr;
    FreeMotionBlock(manager->motion.nodeOffsetPairCursor);
    manager->motion.nodeOffsetPairCursor = nullptr;
    MOTI_Cleanup(manager->motion);
}

namespace motion_detail {

constexpr std::array<std::uint16_t, 7> kDefaultTurnAnimationIds{
    670,
    672,
    674,
    677,
    684,
    682,
    680,
};

} // namespace

} // namespace shenmue::s1
