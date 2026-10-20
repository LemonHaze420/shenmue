#pragma once

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

namespace shenmue::s1 {
namespace motion_detail {}
using namespace motion_detail;
}

namespace shenmue::s1::motion_detail {

struct MotnParseCursor {
    const std::uint16_t *block1;
    const std::uint8_t *block2Bytes;
    const std::uint16_t *block2Words;
    const std::uint8_t *block3Bytes;
    const std::uint16_t *block3Words;
    const std::uint8_t *block4;
    const std::uint16_t *block5;
};

#pragma pack(push, 1)
struct TmnmSequenceHeader {
    std::int16_t frameCount;
    std::uint16_t marker;
    std::int16_t defaultFrameStep;
    std::uint16_t formatFlags;
    std::int16_t nodeCount;
    std::uint16_t reserved0a;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct TmnmWork {
    std::uint32_t state;
    std::int32_t reserved04;
    const std::byte *sequence;
    Mt5RuntimeNode *model;
    float frame;
    float frameStep;
    float startFrame;
    float endFrame;
    std::uint16_t flags;
    std::uint16_t reserved2a;
    std::uint32_t reserved2c;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MotionContactTarget {
    Vector3 position;
    float radius;
    std::uint8_t type;
    std::byte reserved11[3];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionContactTargetList {
    std::byte reserved00[2];
    std::uint8_t count;
    std::byte reserved03[0x15];
    MotionContactTarget *entries;
};
#pragma pack(pop)

struct HeadAngleUpdateRefs {
    std::int16_t *acceleration;
    std::int16_t *decelerationOrigin;
    std::int16_t *velocity;
    std::int16_t *previousTarget;
    std::int16_t *current;
};

struct MotnKeyRef {
    const std::uint16_t *data;
    float time;
    std::uint8_t type;
};


struct MotnKeySpan {
    MotnKeyRef first;
    MotnKeyRef second;
};

#pragma pack(push, 1)
struct SequenceNodeConstraintRecord {
    std::uint32_t characterId;
    std::uint32_t sourceId;
    std::uint8_t endpointNode[2];
    std::uint8_t stateNode[2];
    std::int16_t endpointRadius[2];
    std::int16_t stateRadiusBias[2];
    std::uint16_t startFrame;
    std::uint16_t endFrame;
    std::uint8_t reserved18[2];
    std::uint8_t stateMode[2];
};
#pragma pack(pop)

static_assert(sizeof(TmnmSequenceHeader) == 0x0c);

static_assert(sizeof(TmnmWork) == 0x30);

static_assert(offsetof(TmnmWork, sequence) == 0x08);

static_assert(offsetof(TmnmWork, model) == 0x10);

static_assert(offsetof(TmnmWork, frame) == 0x18);

static_assert(offsetof(TmnmWork, flags) == 0x28);

static_assert(sizeof(MotionContactTarget) == 0x14);

static_assert(offsetof(MotionContactTarget, type) == 0x10);

static_assert(sizeof(MotionContactTargetList) == 0x20);

static_assert(offsetof(MotionContactTargetList, count) == 2);

static_assert(offsetof(MotionContactTargetList, entries) == 0x18);

using MotionEvaluationCallback = void (*)(MotionInformationWork *, void *);

static_assert(sizeof(SequenceNodeConstraintRecord) == 28);



constexpr std::array<std::uint8_t, 256> MakePackedValueCounts() {
    std::array<std::uint8_t, 256> counts{};
    for (std::size_t descriptor = 0; descriptor < counts.size(); ++descriptor) {
        counts[descriptor] = static_cast<std::uint8_t>(((descriptor >> 6U) & 3U) + ((descriptor >> 4U) & 3U) + ((descriptor >> 2U) & 3U) + (descriptor & 3U));
    }
    return counts;
}

inline constexpr auto kPackedValueCounts = MakePackedValueCounts();

inline constexpr double kMotnFrameTime = 0.03333333333333333; // MOTM::constantFrameTime

inline constexpr float kMotnFrameTimeF = 0.033333335f; // SEQ_constant30fps

inline constexpr std::uint32_t kMotionPointComponent = SHEN_NAME(WMPT);

inline constexpr std::uint16_t kApproachingStanceAnimation = 0x0352U;

inline constexpr std::uint32_t kMoveRequestComponent = SHEN_NAME(XMPT);

inline constexpr std::uint32_t kMoveSequenceComponent = SHEN_NAME(YMPT);

inline constexpr std::uint32_t kCubicScratchComponent = SHEN_NAME(VMPT);

inline constexpr std::uint32_t kLookPointComponent = SHEN_NAME(LKPT);


inline constexpr std::uint16_t kCommonWalkAnimationId = 1374;
}

namespace shenmue::s1 {
extern Vector3 g_RootMotionVector;

extern std::int32_t g_motnHasYValue;

extern MotionInformationWork *g_pMotion;

extern std::uint32_t g_motnBasePoseOnly;

extern std::uint32_t g_motnIsRyo;

extern float g_motnClipAngle;

extern MotionCubicSequenceScratch *g_motnCubicScratch;
}

namespace shenmue::s1::motion_detail {
extern const std::array<Vector3, 8> kBoneDirections;

extern MotionInformationWork *g_cubicSequenceMotion;

extern bool g_initializeCubicSequence;

extern bool g_cubicSequenceDirection;

extern float g_cubicSequenceDistance;

extern const std::array<std::int16_t, 9> kHeadAccelerationLimits;

extern const std::array<float, 9> kHeadAccelerationScales;

extern const std::array<float, 3> kHeadCurveBias;

extern const std::array<std::uint32_t, 11> kMotnTargetNodeIds;

extern Vector3i *g_motnEntityEuler;

extern Vector3i *g_motnNextEntityEuler;

extern std::uint32_t g_motnReverse;

extern float g_motnTimerRef;

extern float g_motnFrameStart;

extern float g_motnFrameEnd;

extern float g_motnResidualTime;

extern std::int32_t g_motnFrameCount;

extern std::uint32_t g_motnMirror;

extern std::uint32_t g_motnNeedsRotation;

extern std::uint32_t g_motnIgnoreArmIK;

extern std::uint8_t g_currentMotionUpdateMode;

extern std::int32_t g_motnArmBlendMode;

extern Matrix4x4 g_skinningTransformMatrix;

extern Matrix4x4 g_motnBlendStart;

extern Matrix4x4 g_motnBlendEnd;

extern Matrix4x4 g_motnInterpTransform;

extern Matrix4x4 g_motnFinalInterpTransform;

extern Matrix4x4 g_motnRootInterpTransform;

extern Vector3 g_motnTrackedNodePosition;

extern std::array<Vector3, 9> g_motnSecondaryTargets;

extern Matrix4x4 g_motnSecondaryAnchor;

extern Vector3 g_motnLookTarget;

extern std::int32_t g_motnRightKneeValid;

extern Vector3 g_motnRightKnee;

extern std::int32_t g_motnRightAnkleValid;

extern Vector3 g_motnRightAnkle;

extern std::int32_t g_motnLeftKneeValid;

extern Vector3 g_motnLeftKnee;

extern std::int32_t g_motnLeftAnkleValid;

extern Vector3 g_motnLeftAnkle;

extern const Vector3 *g_motnAnkleKnee;

extern std::int32_t g_motnAnkleKneeValid;

extern float g_motnAnkleReach;

extern const Vector3 *g_motnAnkleRelease;

extern std::int32_t g_motnAnkleReleaseValid;

extern MotionNodeRuntime *g_motnRightAnkleNode;

extern MotionNodeRuntime *g_motnLeftAnkleNode;

extern MotionNodeRuntime *g_motnRightAnkleSourceNode;

extern MotionNodeRuntime *g_motnLeftAnkleSourceNode;

extern Vector3 g_motnRightAnkleSampled;

extern Vector3 g_motnLeftAnkleSampled;

extern Vector3 g_motnRightAnkleSourceSampled;

extern Vector3 g_motnLeftAnkleSourceSampled;

extern Vector3 g_motnRightAnkleCubic;

extern Vector3 g_motnLeftAnkleCubic;

extern float g_motnContactRadiusBias;

extern float g_motnSequenceRootLift;

extern float g_motnSequenceLeadHandLift;

extern float g_motnSequenceTrailHandLift;

extern float g_motnSequenceActiveFrame;

extern float g_motnSequencePreviousFrame;

extern const std::array<std::uint16_t, 16> kMotnSequenceLiftValues;

extern const std::array<std::uint8_t, 5> kMotnSequenceLiftFrames;

extern const std::array<std::uint8_t, 16> kMotnSequenceLiftTypes;

extern MotnKeyframeChannel g_motnSequenceLiftCurve;

extern std::int32_t g_motnBlendAngleDifference;

extern float g_motnBlendAngleSine;

extern std::uint32_t g_motnRawIKFlag;

extern std::uint32_t g_motnRawParentUnrotated;

extern Matrix4x4 g_motnRawLookEnd;

extern Matrix4x4 g_motnRawLookBlend;

extern const std::array<std::uint16_t, 7> kDefaultTurnAnimationIds;

void ReadKeyframeData(MotnParseCursor &cursor, MotnKeyframeChannel &output);

MotionNodeRuntime *FindRuntimeNode(MotionNodeRuntime *nodes, std::uint8_t nodeId);

bool MotionHasRootEndMarker(std::uint16_t animationId);

bool FindAnimationExtraWord(std::uint16_t animationId, std::uint8_t tag, std::uint16_t &value);

bool MaintainFrameRate(float &timer, float &timerReference, float &residualTime, bool resetMode, bool reverse, bool compensateWrap, float frameStart,
    float frameEnd, float frameDelta);

float F16ToF32(std::int16_t value);
}

namespace shenmue::s1 {
std::uint16_t Shenmue_Atan2Fixed(float first, float direction);

const std::uint8_t *shenLookupAnimationIndex(std::uint16_t animationId, std::uint8_t cueType);

const MotionRigType *MOTN_GetRigType(std::uint16_t rigTypeIndex);

void MOTN_InitRigNodes(MotionInformationWork &motion, MotionNodeRuntime *nodes, std::uint16_t rigTypeIndex, float timeScale, MotionNodeRuntime *pairedNodes);

const MotnAnimationHeader *MOTN_GetDataForAnimId(std::uint16_t animationId);

std::uint32_t MOTN_GetAnimationFlag(std::uint16_t animationId);
}

namespace shenmue::s1::motion_detail {
const std::byte *MOTN_FindFrameEvent(
    std::uint16_t animationId, std::uint8_t eventType, float firstFrameInclusive, float lastFrameExclusive, std::uint32_t occurrenceOneBased);
}

namespace shenmue::s1 {
bool MOTN_LoadAnimation(MotionInformationWork &motion, MotionNodeRuntime *nodes, std::uint16_t animationId, std::uint16_t targetRigIndex,
    MotnKeyframeTriplet *&keyStorage, MotionNodeRuntime *pairedNodes);

void MOTN_IncrementDataOffsets(MotionNodeRuntime *motionNodes);

std::int32_t MOTN_LoadTransitionNodes(MotionInformationWork &motion, std::uint16_t animationId);
}

namespace shenmue::s1::motion_detail {
float VectorAxis(const Vector3 &value, std::size_t axis);

void PushMotionMatrix();

void PopMotionMatrix();

Vector3 LocalDirectionTo(const Matrix4x4 &matrix, const Vector3 &target);

std::int16_t AdjustBoneRotation(MotionNodeRuntime &node, float blend, std::int16_t previousAngle);

bool IsFootPlantingExcludedAnimation(std::uint16_t animationId);

Vector3 BlendFootPlantingTarget(const MotionNodeRuntime &blendMotionNode, const MotionNodeRuntime &activeMotionNode);

float SolveRyoFootPlantHeight(float targetY, float groundY, bool previousApplied, float &solvedY, float &previousRawY);

void motnUpdate(MotionInformationWork &motion, HeadAngleUpdateRefs refs, std::int16_t target, std::int16_t &output, std::int32_t type);

void MOTN_ClampAnimationAngles(MotionInformationWork &motion, std::int16_t &yaw, std::int16_t &pitch);

Matrix4x4 *MotionNodeMatrix(MotionNodeRuntime &node);

std::int16_t Fixed16(float value);

float MOTN_readAndConvertData(MotnKeyframeChannel &channel, float currentTime);
}

namespace shenmue::s1 {
float MOTN_CalculateKeyframeForTime(MotnKeyframeChannel &channel, float currentTime, std::int32_t frameCount);
}

namespace shenmue::s1::motion_detail {
bool IsVectorWithinRadius(const Vector3 &point, float radius);

MotionNodeRuntime *FindTrackedNode(MotionNodeRuntime *motionNodes, std::uint32_t nodeId);

void MOTN_CaptureTrackedNodePosition(MotionInformationWork &motion);

MotionNodeRuntime *FindMotionNodeFrom(MotionNodeRuntime *motionNode, std::uint8_t nodeId);

Vector3 WorkLocalPoint(const Vector3 &point);

bool Matrix4x4_Lerp(Matrix4x4 &output, float t, const Matrix4x4 &start, const Matrix4x4 &end);

void MOTN_RotateX();

void MOTN_ApplyBoneHierarchyTransform(MotionNodeRuntime &node, MotionNodeRuntime &nodeSource);

float ComputeLinearRamp01(std::int32_t current, std::int32_t total);

float MOTN_GetSmoothedFrameDelta(float delta);

void MOTN_UpdateSecondaryMotion(MotionInformationWork &motion);

bool MOTN_EvaluateActiveMotion(MotionInformationWork &motion);

bool MOTN_EvaluateStandardBlend(MotionInformationWork &motion);

bool MOTN_EvaluatePreviousPoseBlend(MotionInformationWork &motion);

bool MOTN_EvaluateFrameCarryBlend(MotionInformationWork &motion);

bool MOTN_EvaluateTransitionNodesBlend(MotionInformationWork &motion);

std::int32_t ApplyBasePose(MotionInformationWork &motion, std::uint16_t animationId);

void Matrix4x4_RecursiveRotateMatrix(MotionNodeRuntime &node, const Matrix4x4 &scaledFrame);

void ApplyBoneTransformRecursive(MotionNodeRuntime &node, const Matrix4x4 *transform);

void AddFloatToIntArray(MotionNodeRuntime &node, const Vector3 &offset);

MotionNodeRuntime *CaptureAnkleScratch(MotionNodeRuntime *motionNode, std::uint8_t nodeId, std::int32_t &valid, Vector3 &position);

void ClearEvaluatedFlags(MotionNodeRuntime *motionNodes);

float MOTN_EvaluateKeyframeSlope(MotnKeyframeChannel &channel, float time);

bool MOTN_EvaluateCubicSequenceBlend(MotionInformationWork &motion);

bool MOTN_EvaluateRawMotion(MotionInformationWork &motion);

void MOTN_UpdateRawSecondaryPose(MotionInformationWork &motion, MotionNodeRuntime &motionNodes);
}

namespace shenmue::s1 {
std::int32_t MOTN_GetFrameCountFromAnimId(std::uint16_t animationId);

std::int32_t motnGetRootMotionEndMarker(std::uint16_t animationId);

bool MOTN_ReadRootPositionAtFrame(std::uint16_t animationId, std::uint32_t basePoseTemplateIndex, float frame, Vector3 &outPosition, bool includeY);

bool WMPT_Request(Character *character, const Vector3 &target, std::uint32_t animationId, float timeOffset);

bool WMPT_IsActive(Character *character);

void ManageMotionAnimations(Character *character);
}

namespace shenmue::s1::motion_detail {
void AllocateMotionNodes(MotionNodeRuntime *&nodes, std::int64_t &cookie, Matrix4x4 **matrixPool);
}

namespace shenmue::s1 {
MotionNodeRuntime *MOTN_FindNode(MotionNodeRuntime *motionNode, std::int32_t nodeId);
}

namespace shenmue::s1::motion_detail {

template <typename T> T ReadTmnm(const std::byte *address) {
    T value{};
    std::memcpy(&value, address, sizeof(value));
    return value;
}
}
