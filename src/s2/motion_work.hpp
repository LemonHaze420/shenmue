#pragma once

#include "s2/motion_rig.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct Character;
struct MotionWork;
using MotionTargetCallback = void (*)(std::uint32_t controller, Vector3 *position, MotionWork *work, void *user);
struct MotionPose;
struct MT7Model;

inline constexpr shen_name motionWorkToken{SHEN_NAME(MTWK)};
inline constexpr shen_name motionOverrideMemoryToken{SHEN_NAME(UNFO)};

inline constexpr std::array<std::uint16_t, 18> motionControllerModelNodeIds{1, 2, 0xffbd, 4, 5, 6, 0xffbf, 9, 10, 11, 0xffbe, 14, 16, 17, 18, 21, 22, 23};

extern std::int32_t g_motionDrawOverridesEnabled;
extern std::int32_t g_motionDeformationEnabled;
extern std::uint32_t g_motionDrawOverrideFlags;
extern std::array<const Matrix4x4 *, 7> g_motionDeformationControllers;
extern MotionWork *g_activeMotionWork;
extern std::int32_t g_mtwkCurrentIsRyo;
void SetActiveMTWK(MotionWork *work);
void mtwkGetArmHandPoseChanges(MotionWork *work, std::int32_t *shapes, std::int32_t *frames);
std::int64_t mtwkSetOverridesEnabled(std::int32_t enabled);

enum MotionBoneId : std::int32_t {
    MB_Pelvis = 0x0U,
    MB_Chest = 0x1U,
    MB_ChestTarget = 0x2U,
    MB_UpperChest = 0x3U,
    MB_Neck = 0x4U,
    MB_Head = 0x5U,
    MB_Eyes = 0x6U,
    MB_HeadTarget = 0x7U,
    MB_RightShoulder = 0x8U,
    MB_RightUpperArm = 0x9U,
    MB_RightArmTarget = 0xaU,
    MB_RightForearmTarget = 0xbU,
    MB_RightForearm = 0xcU,
    MB_RightWrist = 0xdU,
    MB_RightHand = 0xeU,
    MB_LeftShoulder = 0xfU,
    MB_LeftUpperArm = 0x10U,
    MB_LeftArmTarget = 0x11U,
    MB_LeftForearmTarget = 0x12U,
    MB_LeftForearm = 0x13U,
    MB_LeftWrist = 0x14U,
    MB_LeftHand = 0x15U,
    MB_Hips = 0x16U,
    MB_LeftThigh = 0x17U,
    MB_LeftShin = 0x18U,
    MB_LeftFootTarget = 0x19U,
    MB_LeftAnkle = 0x1aU,
    MB_LeftFoot = 0x1bU,
    MB_RightThigh = 0x1cU,
    MB_RightShin = 0x1dU,
    MB_RightFootTarget = 0x1eU,
    MB_RightAnkle = 0x1fU,
    MB_RightFoot = 0x20U,
    MB_HeadAimSource = 0x21U,
    MB_HeadAimTarget = 0x22U,
    MB_HeadOverrideA = 0x23U,
    MB_HeadOverrideB = 0x24U,
    MB_HeadOverrideC = 0x25U,
    MB_PelvisOverrideTarget = 0x26U,
    MB_LeftHipOverride = 0x27U,
    MB_RightHipOverride = 0x28U,
    MB_RightToe1 = 0x29U,
    MB_RightToe2 = 0x2aU,
    MB_RightToe3 = 0x2bU,
};

enum MotionController : std::uint32_t {
    MC_Pelvis = 0x0U,
    MC_Chest = 0x1U,
    MC_Head = 0x2U,
    MC_RightArm = 0x3U,
    MC_LeftArm = 0x4U,
    MC_LeftFoot = 0x5U,
    MC_RightFoot = 0x6U,
    MC_PelvisOverride = 0x7U,
    MC_HeadAim = 0x8U,
};

enum MotionRegionFlag : std::uint16_t {
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

enum MotionCopyFlag : std::uint32_t {
    MCF_Unknown01 = 0x1U,
    MCF_Unknown02 = 0x2U,
    MCF_LeadFoot = 0x4U,
    MCF_SampledLeadFoot = 0x8U,
    MCF_Persistent = 0x20U,
    MCF_Unknown80 = 0x80U,
    MCF_LookPending = 0x100U,
    MCF_Running = 0x200U,
    MCF_ExtraStepRequested = 0x400U,
    MCF_ExtraStepComplete = 0x800U,
    MCF_Unknown1000 = 0x1000U,
    MCF_Unknown2000 = 0x2000U,
    MCF_Unknown4000 = 0x4000U,
    MCF_AlternateLocomotion = 0x8000U,
    MCF_Unknown10000 = 0x10000U,
    MCF_Unknown20000 = 0x20000U,
    MCF_Unknown40000 = 0x40000U,
    MCF_Unknown80000 = 0x80000U,
    MCF_LookSuppressed = 0x100000U,
    MCF_TurnStepPending = 0x200000U,
    MCF_Unknown400000 = 0x400000U,
};

inline constexpr std::uint32_t motionControllerIndices[9]{0, 1, 2, 3, 4, 5, 6, 7, 8};

inline constexpr std::int32_t motionTargetIgnoreZ = 0x10000000;
inline constexpr std::int32_t motionTargetIgnoreX = 0x20000000;
inline constexpr std::int32_t motionTargetIgnoreY = 0x40000000;
inline constexpr std::uint32_t motionSequenceInterpolationShift = 24;

enum MotionSequenceFlag : std::uint32_t {
    MSF_LimitConstraintStep = 0x800000U,
    MSF_LinearInterpolation = 0x1000000U,
    MSF_ModelMatricesBound = 0x2000000U,
    MSF_PositionTransition = 0x4000000U,
    MSF_LookAt = 0x8000000U,
    MSF_TargetFacing = 0x10000000U,
    MSF_SearchHand = 0x20000000U,
};

struct MotionCollisionCluster {
    std::array<Vector3, 9> points{};
    std::uint16_t validPointMask{};
    std::uint8_t pointCount{};
    std::uint8_t reserved{};
    Vector3 center{};
    float radius{};
};




#pragma pack(push, 4)
struct MotionRuntimeState {
    void *unfo;
    std::uint32_t motnFlags;
    std::uint32_t seqFlags;
    std::uint16_t regionFlags[5];
    std::byte reserved01a[0x027];
    std::uint8_t requestedRootMode;
    std::uint8_t previousRootMode;
    std::uint8_t currentRootMode;
    std::uint8_t channelFlags;
    std::uint8_t environmentFlags;
    std::uint8_t handPoseIndices[2];
    std::int16_t lookAtYaw;
    std::int16_t pelvisYawTarget;
    std::int16_t pelvisYawCurrent;
    std::int16_t pelvisYawHalf;
    std::uint16_t pelvisYawStep;
    std::int16_t pelvisYawVelocity;
    std::int16_t pelvisYawClamp;
    std::int16_t lookAtYawStep;
    std::int16_t reserved058;
    std::uint16_t previousMotionId[5];
    std::uint16_t currentMotionId[5];
    std::uint16_t queuedMotionId[5];
    std::uint16_t blendTotalFrames[5];
    std::uint16_t blendRemainingFrames[5];
    std::uint16_t regionStepFrames[5];
    std::uint16_t defaultRegionStepFrames[5];
    std::uint16_t constraintBlendTotalFrames[9];
    std::uint16_t constraintBlendRemainingFrames[9];
    std::uint32_t constraintStateBits;
    std::uint32_t pendingFootstepCue;
    std::uint32_t enableEnvironmentMotion;
    std::uint32_t materialFlags;
    float transitionTimerReset;
    float transitionTimer;
    float transitionTimerLimit;
    float baseTimeScale;
    float depthScale;
    Vector3 previousRootPosition;
    Vector3 rootPosition;
    Vector3 rootDelta;
    Vector3i entityRotation;
    std::int32_t activeRootYawOffset;
    std::int32_t requestedRootYawOffset;
    float layerLoopStartTime[5];
    float pendingLayerStartFrame[5];
    float layerEndFrame[5];
    float layerPreviousTicks[5];
    float layerSampleFrame[5];
    float regionPreviousTicksTime[5];
    float gaitSplitFraction;
    float layerBlendAlpha[5];
    float constraintStepLimit[9];
    float constraintStepRate[9];
    float overrideBlendTime;
    float pelvisYawRamp;
    float clusterRadius;
    Vector3 clusterCenter;
    Vector3 *activeConstraintTarget[9];
    Vector3 *latchedConstraintTarget[9];
    float projectionCotangentNear;
    float projectionCotangentMiddle;
    float projectionCotangentFar;
    std::uint32_t reserved2ac;
    const Matrix4x4 *transformStart; // +0x2b0, borrowed solved-root override
    Matrix4x4 *transformSecondary;
    Vector3 *ikTargetPosition;
    std::uint8_t copyOperationState;
    std::uint8_t copyPhase;
    std::uint8_t transitionSubstate;
    std::uint8_t transitionClass;
    std::uint8_t copyDelayFrames;
    std::uint8_t copyScaleCountdown;
    std::uint8_t copyStepsRemaining;
    std::uint8_t copyStepState;
    std::uint8_t copyRegionSelector;
    std::uint8_t transitionVariant;
    std::uint16_t reserved2d2;
    std::int16_t copyTravelYaw;
    std::uint16_t transitionStartFrame;
    std::uint16_t transitionEndFrame;
    std::int16_t copySplineFrame;
    std::uint16_t copySplineStatus;
    std::uint16_t transitionFrameTime;
    std::uint32_t copyFlags;
    float copyLayerStartFrame;
    float motionScale;
    float insideRectangleScale;
    float copyExtraStepDistance;
    Vector3 copySourcePosition;
    Vector3 copyDestinationPosition;
    Vector3 copySplinePoints[5];
    Vector3 copySplinePosition;
    std::uint16_t extraMotionIds[6];
    std::uint8_t transitionState;
    std::uint8_t extraMotionSpeed;
    std::uint16_t reserved362;
    std::uint16_t locomotionMotionSet;
    std::uint16_t extraMotionMask;
    Vector3 extraMotionPosition;
    float extraBlendTime;
    float extraBlendScale;
    float lookAtBlendTime;
    Vector3 lookAtTarget;
    Vector3 leftControllerTarget;
    Vector3 rightControllerTarget;
    std::uint8_t targetFacingState;
    std::uint8_t targetFacingMotionClass;
    std::uint16_t targetFacingFlags;
    std::int16_t targetFacingYaw;
    std::int16_t targetFacingYawDelta;
    float chestBlendScale;
    float chestRotationOverrideBlendTime;
    Vector3 targetFacingPosition;
    const Matrix4x4 *drawOverrideData; // +0x3c0, borrowed current frame
    void *field3c8;
    void *rightArmTrack;
    void *leftArmTrack;
    void *regionTrack[5];
    void *previousRegionTrack[5];
    std::uint32_t reserved430;
    Vector3 constraintBlendSource[9];
    std::uint32_t callbackFlags;
    std::uint32_t field4a4;
    void *transformSource;
    std::int32_t transformSourceIndex;
    Vector3 transformOffsetPosition;
    Vector3i transformOffsetRotation;
    Matrix4x4 overrideViewSpaceTransform;
    std::uint32_t field50c;
};



struct MotionTargetParameters {
    Vector3 *output;
    MotionRigChannel *channels[3];
    Vector4 *pva;
    const std::uint32_t *controller;
    MotionTargetCallback callback;
    void *user;
};


struct MotionRotationParameters {
    MotionRigChannel *channels[3];
    MotionRotationBasis *basis;
    float *blendAlpha;
};

struct MotionWork {
    IKRig rig;
    MotionTargetParameters targets[9]; // controllers 0, 7, 5, 6, 1, 8, 2, 3, 4
    MotionRotationParameters rotations[27];
    MotionRuntimeState runtime;
};
#pragma pack(pop)

enum class MotionConstraintSlot : std::uint8_t {
    Pelvis,
    Chest,
    Head,
    RightArm,
    LeftArm,
    LeftFoot,
    RightFoot,
    LeftToe,
    HeadAim,
    Count,
};

static_assert(offsetof(MotionRuntimeState, requestedRootMode) == 0x041);
static_assert(offsetof(MotionRuntimeState, environmentFlags) == 0x045);
static_assert(offsetof(MotionRuntimeState, currentMotionId) == 0x064);
static_assert(offsetof(MotionRuntimeState, constraintBlendTotalFrames) == 0x0a0);
static_assert(offsetof(MotionRuntimeState, constraintBlendRemainingFrames) == 0x0b2);
static_assert(offsetof(MotionRuntimeState, constraintStateBits) == 0x0c4);
static_assert(offsetof(MotionRuntimeState, pendingFootstepCue) == 0x0c8);
static_assert(offsetof(MotionRuntimeState, enableEnvironmentMotion) == 0x0cc);
static_assert(offsetof(MotionRuntimeState, handPoseIndices) == 0x046);
static_assert(offsetof(MotionRuntimeState, materialFlags) == 0x0d0);
static_assert(offsetof(MotionRuntimeState, transitionTimer) == 0x0d8);
static_assert(offsetof(MotionRuntimeState, layerLoopStartTime) == 0x120);
static_assert(offsetof(MotionRuntimeState, constraintStepLimit) == 0x1b0);
static_assert(offsetof(MotionRuntimeState, activeConstraintTarget) == 0x210);
static_assert(offsetof(MotionRuntimeState, latchedConstraintTarget) == 0x258);
static_assert(offsetof(MotionRuntimeState, constraintBlendSource) == 0x434);
static_assert(offsetof(MotionRuntimeState, projectionCotangentNear) == 0x2a0);
static_assert(offsetof(MotionRuntimeState, motionScale) == 0x2e8);
static_assert(offsetof(MotionRuntimeState, copyOperationState) == 0x2c8);
static_assert(offsetof(MotionRuntimeState, copyDelayFrames) == 0x2cc);
static_assert(offsetof(MotionRuntimeState, copyTravelYaw) == 0x2d4);
static_assert(offsetof(MotionRuntimeState, copySplinePoints) == 0x30c);
static_assert(offsetof(MotionRuntimeState, copySplinePosition) == 0x348);
static_assert(offsetof(MotionRuntimeState, targetFacingPosition) == 0x3b4);
static_assert(offsetof(MotionRuntimeState, callbackFlags) == 0x4a0);
static_assert(offsetof(MotionRuntimeState, transformSource) == 0x4a8);
static_assert(offsetof(MotionRuntimeState, overrideViewSpaceTransform) == 0x4cc);
static_assert(sizeof(MotionRuntimeState) == 0x510);
static_assert(sizeof(MotionTargetParameters) == 0x40);
static_assert(sizeof(MotionRotationParameters) == 0x28);
static_assert(offsetof(MotionWork, targets) == 0x2658);
static_assert(offsetof(MotionWork, rotations) == 0x2898);
static_assert(offsetof(MotionWork, runtime) == 0x2cd0);
static_assert(sizeof(MotionWork) == 0x31e0);

// MotionWork::targets index of each controller's parameter record (controllers 0..8).
inline constexpr std::uint8_t motionControllerTargets[9]{0, 4, 6, 7, 8, 2, 3, 1, 5};




extern MotionRigClock *g_motionRegionClocks[5];
extern std::uint8_t g_motionLayerEvaluated[8];
extern std::int32_t g_ikEnabled;
void cbEvaluateTargetRotOverride(void *parameters);
void cbEvaluateTwoJointRotationOverride(void *parameters);
void cbEvaluateXRotationOverride(void *parameters);
void cbEvaluateZYRotationOverride(void *parameters);
void ApplyRotY_ThenCopyOrRotZYX(void *parameters);
void ApplyChannelRotZYX_WithIKXBias(void *parameters);
void cbEvaluateChestOverride(void *parameters);
void ApplyChannelRotZYX_WithIKBlend(void *parameters);
void cbEvaluateTargetPosOverride(void *parameters);
void Motionwork_PelvisOverride(void *parameters);
void cbEvaluateLookAtOverride(void *parameters);
void Arm_TargetEffectorOverride(void *parameters);
void Motionwork_ChestIKEffectorOverride(void *parameters);
void ApplyConstraint(MotionWork &work, Vector3 *position, std::uint32_t controller);
void EvaluateHeadLookAt(MotionWork &work, Vector3 *target, std::uint32_t controller);
void mtwkCopyRigFlags(
    MotionWork *work, MotionRigClock **lowerBody, MotionRigClock **chest, MotionRigClock **head, MotionRigClock **rightArm, MotionRigClock **leftArm);
float TimePair_ClampAndPropagate(MotionWork *work, float split, float ticks, float maximum, MotionRegion region);
void AssignStaticDataForBodyRegion(MotionWork *work, float split, float ticks, float maximum, MotionRegion region);
std::uint32_t TimestepUpdate(MotionRigClock *clock, std::uint32_t allowOverflow, std::uint32_t allowReset);
std::uint32_t AnimClock_ForceJump(MotionRigClock *clock);
void Rig_EvaluateControllers(MotionWork *work, const MotionStreamContext *context, float sourceTime, float time, std::uint32_t flags, MotionRegion region);
bool Rig_SetupMotionGraph(IKRig &rig, const MotionStreamContext *context, float time, std::uint32_t flags, std::uint32_t region,
    MotionTargetParameters *targets, MotionRotationParameters *rotations);
void Rig_RunOverrideBlendPass(MotionWork *work);
void ClearOverrideCallbacksForBodyRegion(MotionWork *work, MotionRegion region);
void InstallActiveOverrideCallbacks(MotionWork *work);
void InstallConstraintTargetCallback(MotionWork *work, std::uint32_t controller);
void ClearConstraintTargetCallbackIfInactive(MotionWork *work, std::uint32_t controller);
void UpdatePelvisYaw(MotionWork *work);
std::int32_t UpdateMotionOverrideCallbacks(MotionWork *work);
std::uint32_t HasLeadFootCommand(std::uint16_t motion);
void mtwkApplyRootPos(MotionWork *work);
void applyRootTranslation(MotionWork *work);
std::int32_t MOTN_ProcessLayers(MotionWork *work);
void mtwkAdvanceAnimFrameAndLayers(MotionWork *work);
void mtwkApplySequenceCommands(MotionWork *work);
void motnAdvanceInterpolated(MotionWork *work, float x, float y, float z);
void motnCopyInterpolated(MotionWork *work, float x, float y, float z);
std::uint32_t mtwkApplyTransforms(const Matrix4x4 *pelvis);
void resolveAnimId(MotionWork *work, std::uint16_t motion, std::uint16_t remap, float begin, float end);
void mtwkUpdateBoneForTime(MotionWork *work, std::uint16_t motion, float begin, float end);
void mtwkSelectRandomFootstepCue(MotionWork *work, std::uint16_t motion, float begin, float end);
void UpdateMotionCollisionCluster(MotionWork *work);
std::int32_t mtwkApplyInterpolatedTransforms(MotionWork *work);
Matrix4x4 *mtwkBuildArmRebaseTransform(Matrix4x4 *destination, MotionWork *work);
// Returns whether the character is on screen (always for search-hand motion).
std::int32_t mtwkEvaluateMotionFrame(MotionWork *work);



void SetSharedPrngSeed(std::uint32_t seed) noexcept;
std::uint16_t IncrementSharedPrngUInt16() noexcept;
float IncrementSharedPrngFloat() noexcept;
std::int32_t shenGetRandomInt32(std::int32_t bound) noexcept;
float GetRandomNumberInRange(float bound) noexcept;
inline constexpr std::array<std::uint16_t, 10> motionOverrideModelNodeIds{0, 15, 20, 55, 56, 57, 0xffb1, 58, 59, 119};
inline constexpr std::array<std::uint16_t, 3> motionNormalModelNodeIds{0, 7, 12};
void BindMotionControllerMatricesToModelNodes(Character *character, MotionWork *work, MT7Model *model);



extern std::int32_t g_usingEnv;
void SetMotionEnvironmentEnabled(std::int32_t enabled) noexcept;

bool AssignMotionTargetCallback(MotionWork *work, MotionTargetCallback callback, std::int32_t selector);
bool AssignMotionTargetParameter(MotionWork *work, void *parameter, std::int32_t selector);
MotionTargetCallback GetMotionTargetCallback(const MotionWork *work, std::int32_t selector);
void *GetMotionTargetParameter(const MotionWork *work, std::int32_t selector);
void TransformWithoutIKEnabled(MotionWork *work, Vector3 *foot, Vector3 *pelvis);
bool GetMotionControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position);

MotionWork *AllocateMotionWork(Character *character);
MotionWork *FindMotionWork(const Character *character);

bool mtwkSetSequenceFlags(Character *character);
std::uint32_t GetCharacterMotionYaw(const Character *character);
float AssignCharacterMotionYaw(Character *character, std::int32_t yaw, const std::uint16_t *customMotions, std::uint32_t flags);
// SetMotionDrawOverrideData. The bank owner retains the matrices through draw and clears
// this pointer before releasing the bank. Records follow drawable-node
// preorder, not bone IDs; each XB01-bearing node consumes one 0x40 record.
bool SetMotionDrawOverrideData(MotionWork *motionWork, const Matrix4x4 *frame);
const Matrix4x4 *GetMotionDrawOverrideData(const MotionWork *motionWork);
bool SetMotionIkTargetPosition(MotionWork *work, Vector3 *position);

void SetMotionRootTransformOverride(MotionWork *motionWork, const Matrix4x4 *transform);

bool BuildMotionRootRebaseTransform(const MotionWork *motionWork, Matrix4x4 &transform);
bool EvaluateCurrentMotionPose(const MotionWork *motionWork, MotionPose &pose);
Matrix4x4 *charaGetBoneTransform(MotionWork *motion, std::int32_t boneId);
void RebaseMotionSolvedMatrices(MotionWork *work);
bool CopyMotionBoneTransform(MotionWork *motion, std::int32_t boneId, Matrix4x4 *transform);
bool GetMotionBonePosition(MotionWork *motion, MotionBoneId boneId, Vector3 *position);

bool GetIsRHandAboveShoulder(MotionWork *motion);
// Whether the right shoulder (0x19) sits higher than the right hand (0x1e).
bool GetIsRHandBelowShoulder(MotionWork *motion);
void GetCharacterMotionLocalPosition(Character *character, std::int32_t bone, Vector3 *position);
void GetCharacterMotionNodePosition(Character *character, std::int32_t bone, Vector3 *position);
void GetCharacterMotionWorldPosition(Character *character, std::int32_t bone, Vector3 *position);
void TransformScenarioMotionBonePose(shen_name character, std::int32_t boneId, Vector3 *position, Vector3i *rotation);

bool ResolveAuthoredMotionBoneTransform(const Character *character, std::uint16_t boneId, Matrix4x4 &transform);
bool GetCharacterMotionBonePosition(Character *character, std::uint16_t boneId, Vector3 &position);
std::int16_t GetMotionBoneYaw(MotionWork *motion, std::int32_t bone);
bool SetMotionConstraintTarget(MotionWork *motionWork, MotionConstraintSlot slot, const Vector3 &worldTarget);
void ClearMotionConstraintTarget(MotionWork *motionWork, MotionConstraintSlot slot);
void ClearAllMotionConstraintTargets(MotionWork *motionWork);
bool IsMotionConstraintTargetSettled(const MotionWork *motionWork, MotionConstraintSlot slot);
bool mtwkRequestMotion(MotionWork *motionWork, std::uint16_t motionId, MotionRegion region);
std::uint16_t GetMotionAnimationId(const MotionWork *motionWork, MotionRegion region);
std::int64_t motnIsCurrentAnimOpenEnded(const MotionWork *motionWork, MotionRegion region);

std::uint16_t GetQueuedMotionAnimationId(const MotionWork *motionWork, MotionRegion region);
// MOTN_SampleTrack: a motion's root translation at a frame (negative selects the last), scaled to the rig.
bool SampleNativeMotionRoot(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 &output);
bool SampleNativeMotionRootAndRotation(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *position, Vector3i *rotation);
bool ApplyCharacterMotionToPose(Character *character, Vector3 *position, Vector3i *rotation, float frame, std::uint16_t motion);
bool vfClsampleCurrentMotion(MotionWork *motionWork, float frame, Vector3 &output);
Vector3 GetMotionRootPosition(const MotionWork *motion);
void SetMotionRootPosition(MotionWork *motion, const Vector3 &position);
void ApplyMotionCollisionTranslation(MotionWork *motion, float x, float y, float z);
void RequestMotionRegionDetach(MotionWork *motionWork, MotionRegion region);
std::int32_t GetMotionRigBoneId(const MotionWork *motionWork);
MotionTimePair *GetMotionLayerTimePair(MotionWork *motionWork, MotionRegion region);
const MotionTimePair *GetMotionLayerTimePair(const MotionWork *motionWork, MotionRegion region);
std::uint32_t GetMotionTrackedRootMode(const MotionWork *work);
float GetMotionTransitionFramesRemaining(const MotionWork *work, MotionRegion region);
void CopyMotionRegionTimes(const MotionWork *work, float *split, float *current, float *maximum, float *speed, MotionRegion region);
float GetMotionSplitFrame(const MotionWork *work, MotionRegion region);
float GetMotionFrame(const MotionWork *motionWork, MotionRegion region);
float GetMotionPoseFrame(const MotionWork *motionWork, MotionRegion region);
float GetMotionEndFrame(const MotionWork *motionWork, MotionRegion region);
void SetMotionEntityRotation(MotionWork *motionWork, const Vector3i &rotation);



bool GetParentBoneWrapper(MotionWork *motionWork, std::int32_t selector);
bool vfmaSetAngleFor(MotionWork *motionWork, std::int32_t angle);
bool SetMotionBoneTarget(MotionWork *motionWork, std::int32_t selector, const Vector3 *point);
bool LatchMotionBoneTarget(MotionWork *motionWork, std::int32_t selector, const Vector3 *point);
float GetMotionBoneTargetStepLimit(const MotionWork *work, std::int32_t selector);
bool SetMotionBoneTargetStepLimit(MotionWork *work, std::int32_t selector, float limit);

void SetMotionSpeed(MotionWork *motionWork, float speed, MotionRegion region);
float GetMotionSpeed(const MotionWork *motionWork, MotionRegion region);
// Shared by PLYM collision-speed and MTWK copy transitions; returns the previous pelvis scale ratio or -1.
float SetMotionRigScale(MotionWork *motionWork, float scale);
std::uint32_t mtwkSetExtraFlag(MotionWork *motionWork, std::uint32_t flag, std::int32_t operation);
std::uint16_t SetMotionRegionFlag(MotionWork *motionWork, std::uint32_t mode, std::int32_t operation, MotionRegion region);
bool mtwkSetRequestedRootMode(MotionWork *motionWork, std::uint32_t mode);
bool SetMotionExtraMotion(MotionWork *motionWork, std::uint32_t slot, std::uint16_t motionId);
void EV_mtwkSetEnabled(MotionWork *motionWork, std::int32_t enabled);
std::uint32_t IsMotionWorkEnabled(const MotionWork *motionWork);
void SetMotionReactFlags(MotionWork *work, std::uint8_t flags);
// SetMotionWorkCulled independently gates MTWK_Callback without changing root writeback.
void SetMotionWorkCulled(MotionWork *motionWork, bool culled);
void SetMotionEnvironmentEnabled(MotionWork *motionWork, std::uint32_t enabled);

std::int64_t ExecuteMotionworkUtility(Character *character, std::uint32_t yaw, float angle, std::int32_t flags, const void *motionData);




bool BeginAuthoredCharacterMotion(
    Character *character, std::uint16_t motionId, const Vector3i *authoredRotation, float startFrame, float endFrame, float speed, std::uint32_t commandFlags);
// Normal completion restores speed and publishes solved root XZ, preserving
// the node's current Y. Abort only re-enables writeback and clears extra flag 9.
void FinishAuthoredCharacterMotion(Character *character, float speed, bool interrupted, bool applyYawOffset);

bool UpdateMotionWorkWithPosition(MotionWork *work, const Vector3 *target, std::uint16_t operation);
bool RequestMotionTargetFacing(MotionWork *work, const Vector3 *target, std::uint32_t flags);
bool SetMotionFrameTime(MotionWork *work, std::uint32_t frames, MotionRegion region);
void SetMotionRegionEvalTime(MotionWork *work, float frame, MotionRegion region);
void SetMotionLayerEndFrame(MotionWork *work, float frame, MotionRegion region);
void SetMotionLayerLoopStartTime(MotionWork *work, float frame, MotionRegion region);
void SetMotionLayerFrameRange(MotionWork *work, float loopStart, float startFrame, float endFrame, MotionRegion region);
bool AssignMotionRigYawOffset(MotionWork *work, std::uint32_t yaw);
void SetMotionPelvisYawStep(MotionWork *work, std::int16_t step);
std::uint64_t SetMotionTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);
bool HasMotionTrackCommand(const MotionWork *work, MotionRegion region);
bool HasMotionDependentTrack(const MotionWork *work);
bool ResetMotionWorkPositionTransition(MotionWork *motionWork);
bool CopyMotionTransition(MotionWork *motionWork, const Vector3 *position, std::uint16_t yaw, std::int16_t operation, std::int32_t copyType);
bool RequestMotionPositionTransition(MotionWork *work, const Vector3 *position, std::uint16_t yaw, std::uint32_t motion, float startFrame);



bool BeginMotionPositionTransition(MotionWork *motionWork, const Vector3 &target, std::uint16_t targetYaw);


bool CompleteMotionPositionTransitionRequest(MotionWork *motionWork);

} // namespace shenmue::s2
