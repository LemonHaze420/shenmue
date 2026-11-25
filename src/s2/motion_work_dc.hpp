#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#endif

#define SHENMUE_MOTION_WORK_LAYOUT_NAME_(line) SHENMUE_MOTION_WORKLayoutCheck##line
#define SHENMUE_MOTION_WORK_LAYOUT_NAME(line) SHENMUE_MOTION_WORK_LAYOUT_NAME_(line)
#define SHENMUE_MOTION_WORK_LAYOUT(condition) typedef char SHENMUE_MOTION_WORK_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_MOTION_WORK_CONSTANT static const
#define SHENMUE_MOTION_WORK_INITIALIZER =
namespace shenmue {
namespace s2 {

typedef std::int32_t MotionBoneId;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef std::uint32_t MotionRegion;
#endif
struct MotionHeadRig;

struct MotionRegionClock {
    float splitTimeFrames, ticksTime, maxTime, playbackSpeed;
};
SHENMUE_MOTION_WORK_LAYOUT(sizeof(MotionRegionClock) == 0x10);

enum MotionSequenceFlag {
    MSF_LimitConstraintStep = 0x00800000U,
    MSF_LinearInterpolation = 0x01000000U,
    MSF_ModelMatricesBound = 0x02000000U,
    MSF_PositionTransition = 0x04000000U,
    MSF_LookAt = 0x08000000U,
    MSF_TargetFacing = 0x10000000U,
    MSF_SearchHand = 0x20000000U
};
enum MotionClipMode { MCM_None = 0, MCM_Low = 1, MCM_High = 2, MCM_Default = 3 };
extern const std::uint16_t motionBoneMatrixOffsets[32];
extern const std::uint16_t motionOverrideMatrixPointerOffsets[11];
extern const std::uint16_t motionControllerTargetOffsets[7];
extern const std::uint16_t motionControllerOverridePointerOffsets[2];
SHENMUE_MOTION_WORK_CONSTANT std::uint8_t motionTranslationBoneOrder[] SHENMUE_MOTION_WORK_INITIALIZER{
    0, 22, 23, 24, 26, 28, 29, 31, 3, 4, 5, 8, 9, 12, 14, 15, 16, 19, 21};
SHENMUE_MOTION_WORK_CONSTANT std::uint8_t motionTranslationOverrideOrder[] SHENMUE_MOTION_WORK_INITIALIZER{6, 7, 8, 9, 10, 0, 2, 3, 4};
SHENMUE_MOTION_WORK_CONSTANT std::uint16_t motionTranslationDefaultMatrixOffsets[] SHENMUE_MOTION_WORK_INITIALIZER{0x1030, 0x1980, 0x1f00, 0x898, 0xd88};
SHENMUE_MOTION_WORK_CONSTANT std::uint16_t kMotionOverrideTargetPositionOffset = 0xbc;
SHENMUE_MOTION_WORK_CONSTANT std::uint16_t kMotionHeadRigOffset = 0x11a8, kMotionHeadWorldMatrixOffset = 0x360, kMotionHeadEyeOffset = 0x3a0;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct MotionWork;
typedef void (*MotionTargetCallback)(std::uint32_t controller, Vector3 *position, MotionWork *work, void *user);
static const std::uint8_t motionTargetSlots[9] = {0, 4, 6, 7, 8, 2, 3, 1, 5};
bool mtwkAssignTargetCallback(MotionWork *work, MotionTargetCallback callback, std::int32_t selector);
bool mtwkAssignTargetParameter(MotionWork *work, void *parameter, std::int32_t selector);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MotionTargetCallback mtwkGetTargetCallback(const MotionWork *work, std::int32_t selector);
void *mtwkGetTargetParameter(const MotionWork *work, std::int32_t selector);
std::uint32_t mtwkGetConstraintAngle(const MotionWork *work);
bool mtwkSetConstraintAngle(MotionWork *work, std::int32_t angle);
std::uint32_t mtwkGetProjectionAngle(const MotionWork *work, std::int32_t slot);
#endif
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
struct MotionWorkBindings {
    IKRig rig;
    MotionTargetParameters targets[9];
    MotionRotationParameters rotations[27];
};
SHENMUE_MOTION_WORK_LAYOUT(sizeof(MotionTargetParameters) == 0x20);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionTargetParameters, output) == 0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionTargetParameters, channels) == 4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionTargetParameters, callback) == 0x18);
SHENMUE_MOTION_WORK_LAYOUT(sizeof(MotionRotationParameters) == 0x14);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWorkBindings, targets) == 0x2078);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWorkBindings, rotations) == 0x2198);
SHENMUE_MOTION_WORK_LAYOUT(sizeof(MotionWorkBindings) == 0x23b4);
#endif
struct MotionWork {
    std::uint32_t rigBoneId;
    std::uint32_t useMatrixOverrides;
    std::byte retainedLowerRig[0x88];
    MotionRegionClock lowerBodyClock;
    std::byte retainedLowerControllers[0xd7c];
    MotionRegionClock chestClock;
    std::byte retainedChestControllers[0x380];
    MotionRegionClock headClock;
    std::byte retainedHeadControllers[0x3c0];
    MotionRegionClock rightArmClock;
    std::byte retainedRightArmControllers[0x570];
    MotionRegionClock leftArmClock;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::byte retainedLeftArmControllers[0x8a8];
    MotionOverrideRig *overrides;
#else
    std::byte retainedLeftArmControllers[0x8ac];
#endif
    std::uint32_t motionFlags;
    std::uint32_t sequenceFlags;
    std::uint16_t regionFlags[5];
    std::byte retainedSequenceState[0x12];
    std::uint8_t clipMode;
    std::uint8_t requestedRootMode;
    std::uint8_t previousRootMode;
    std::uint8_t currentRootMode;
    std::uint8_t parentBoneSelector;
    std::uint8_t environmentFlags;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint8_t handPoseIndices[2];
    std::int16_t lookAtYaw, pelvisYawTarget, pelvisYawCurrent, pelvisYawHalf;
#else
    std::byte retainedMotionIdState[0x0a];
#endif
    std::uint16_t pelvisYawStep;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int16_t pelvisYawVelocity;
    std::uint16_t pelvisYawClamp;
    std::int16_t lookAtYawStep, retainedPreviousMotionId;
#else
    std::byte retainedPreviousMotionId[8];
#endif
    std::uint16_t previousMotionId[5];
    std::uint16_t currentMotionId[5], queuedMotionId[5];
    std::uint16_t blendTotalFrames[5], blendRemainingFrames[5];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint16_t regionStepFrames[5];
#else
    std::byte retainedRegionStepFrames[10];
#endif
    std::uint16_t defaultRegionStepFrames[5];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint16_t constraintBlendTotalFrames[9], constraintBlendRemainingFrames[9];
    std::uint32_t constraintStateBits;
    std::uint32_t pendingFootstepCue;
#else
    std::byte retainedControllerState[0x2c];
#endif
    std::uint32_t retainedMotionParameter;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t materialFlags;
    float transitionTimerReset;
#else
    std::byte retainedTransitionTimer[8];
#endif
    float transitionTimer;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float transitionTimerLimit, baseTimeScale;
    float depthScale;
    Vector3 previousRootPosition;
#else
    std::byte retainedRootPositionState[0x18];
#endif
    Vector3 cachedRootPosition;
    Vector3 accumulatedTranslation;
    std::int32_t entityRotationX;
    std::int32_t entityYaw;
    std::int32_t entityRotationZ;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int32_t activeRootYawOffset;
    std::int32_t requestedRootYawOffset;
#else
    std::byte retainedEntityState[8];
#endif
    float layerLoopStartTime[5], pendingLayerStartFrame[5], layerEndFrame[5];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float layerPreviousTicks[5];
#else
    std::byte retainedConstraintState[0x14];
#endif
    float layerSampleFrame[5];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float previousLayerSampleFrame[5];
    float gaitSplitFraction, layerBlendAlpha[5];
#else
    std::byte retainedConstraintRates[0x2c];
#endif
    float constraintStepLimit[9];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float constraintStepRate[9];
    float overrideBlendTime;
    float pelvisYawRamp;
#else
    std::byte retainedClusterState[0x2c];
#endif
    float clusterRadius;
    Vector3 clusterCenter;
    const Vector3 *activeConstraintTarget[9], *latchedConstraintTarget[9];
    float retainedCollisionValues[3];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const Matrix4x4 *transformStart;
    std::byte retainedCollisionState[4];
    const Vector3 *ikTargetPosition;
    std::uint8_t copyOperationState, copyPhase, transitionSubstate, transitionClass;
    std::uint8_t copyDelayFrames, copyScaleCountdown, copyStepsRemaining, copyStepState;
    std::uint8_t copyRegionSelector, transitionVariant;
    std::uint16_t retainedCopyState;
    std::int16_t copyTravelYaw;
    std::uint16_t transitionStartFrame, transitionEndFrame;
    std::int16_t copySplineFrame;
    std::uint16_t copySplineStatus, transitionFrameTime;
#else
    std::byte retainedCollisionState[0x24];
#endif
    std::uint32_t copyFlags;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float copyLayerStartFrame, motionScale, insideRectangleScale, copyExtraStepDistance;
    Vector3 copySourcePosition, copyDestinationPosition, copySplinePoints[5], copySplinePosition;
#else
    std::byte retainedTargetMode[0x70];
#endif
    std::uint16_t defaultAnimations[6];
    std::uint8_t transitionState, extraMotionSpeed;
    std::uint16_t retainedLocomotionState, locomotionMotionSet;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint16_t extraMotionMask;
    Vector3 extraMotionPosition;
    float extraBlendTime;
#else
    std::byte retainedTargetBlend[0x12];
#endif
    float extraBlendScale;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float lookAtBlendTime;
#else
    std::byte retainedTargetPosition[4];
#endif
    Vector3 lookAtTarget, leftControllerTarget, rightControllerTarget;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint8_t targetFacingState, targetFacingMotionClass;
    std::uint16_t targetFacingFlags;
    std::int16_t targetFacingYaw, targetFacingYawDelta;
    float chestBlendScale, chestRotationOverrideBlendTime;
    Vector3 targetFacingPosition;
    const Matrix4x4 *drawOverrideData;
    std::byte retainedExecutionStateTail[4];
    const std::uint16_t *armHandTracks[2];
    const std::uint16_t *regionTrack[5], *previousRegionTrack[5];
    std::byte retainedHandTrackState[4];
#else
    std::byte retainedExecutionState[0x58];
#endif
    std::uint32_t footstepAttributes[2][2];
    Vector3 constraintBlendSource[9];
    std::uint32_t executionFlags;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    Character *transformSource;
    std::int32_t transformSourceIndex;
    Vector3 transformOffsetPosition;
    Vector3i transformOffsetRotation;
    Matrix4x4 overrideViewSpaceTransform;
#else
    std::byte retainedState[0x60];
#endif
};
SHENMUE_MOTION_WORK_LAYOUT(sizeof(MotionWork) == 0x2820);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, useMatrixOverrides) == 4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, lowerBodyClock) == 0x90);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, chestClock) == 0xe1c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, headClock) == 0x11ac);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, rightArmClock) == 0x157c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, leftArmClock) == 0x1afc);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, sequenceFlags) == 0x23bc);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, motionFlags) == 0x23b8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, executionFlags) == 0x27bc);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, clipMode) == 0x23dc);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, regionFlags) == 0x23c0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, requestedRootMode) == 0x23dd);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, currentRootMode) == 0x23df);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, parentBoneSelector) == 0x23e0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, retainedMotionParameter) == 0x2468);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, environmentFlags) == 0x23e1);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, pelvisYawStep) == 0x23ec);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, currentMotionId) == 0x2400);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, previousMotionId) == 0x23f6);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, queuedMotionId) == 0x240a);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, blendTotalFrames) == 0x2414);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, blendRemainingFrames) == 0x241e);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, defaultRegionStepFrames) == 0x2432);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, cachedRootPosition) == 0x2490);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transitionTimer) == 0x2474);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, accumulatedTranslation) == 0x249c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, entityRotationX) == 0x24a8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, entityYaw) == 0x24ac);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, entityRotationZ) == 0x24b0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, activeRootYawOffset) == 0x24b4);
#else
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, retainedEntityState) == 0x24b4);
#endif
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, layerLoopStartTime) == 0x24bc);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, pendingLayerStartFrame) == 0x24d0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, layerEndFrame) == 0x24e4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, constraintStepLimit) == 0x254c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, layerSampleFrame) == 0x250c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, clusterRadius) == 0x259c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, clusterCenter) == 0x25a0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, activeConstraintTarget) == 0x25ac);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, latchedConstraintTarget) == 0x25d0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, retainedCollisionValues) == 0x25f4);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transformStart) == 0x2600);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, retainedCollisionState) == 0x2604);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, ikTargetPosition) == 0x2608);
#else
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, retainedCollisionState) == 0x2600);
#endif
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, constraintBlendSource) == 0x2750);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, copyOperationState) == 0x260c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transitionStartFrame) == 0x261a);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transitionFrameTime) == 0x2622);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, motionScale) == 0x262c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, copySourcePosition) == 0x2638);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, copyDestinationPosition) == 0x2644);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, copySplinePosition) == 0x268c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, constraintStateBits) == 0x2460);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, extraMotionMask) == 0x26aa);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, extraMotionPosition) == 0x26ac);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, extraBlendTime) == 0x26b8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, lookAtBlendTime) == 0x26c0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, targetFacingState) == 0x26e8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, targetFacingFlags) == 0x26ea);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, targetFacingPosition) == 0x26f8);
#else
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, retainedExecutionState) == 0x26e8);
#endif
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, footstepAttributes) == 0x2740);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, copyFlags) == 0x2624);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, defaultAnimations) == 0x2698);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transitionState) == 0x26a4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, extraMotionSpeed) == 0x26a5);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, locomotionMotionSet) == 0x26a8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, extraBlendScale) == 0x26bc);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, lookAtTarget) == 0x26c4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, leftControllerTarget) == 0x26d0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, rightControllerTarget) == 0x26dc);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, handPoseIndices) == 0x23e2);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, armHandTracks) == 0x270c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, regionTrack) == 0x2714);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, previousRegionTrack) == 0x2728);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, previousLayerSampleFrame) == 0x2520);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, pendingFootstepCue) == 0x2464);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, materialFlags) == 0x246c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transitionTimerReset) == 0x2470);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transitionTimerLimit) == 0x2478);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, baseTimeScale) == 0x247c);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, depthScale) == 0x2480);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, overrides) == sizeof(MotionWorkBindings));
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, pelvisYawClamp) == 0x23f0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, pelvisYawTarget) == 0x23e6);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, pelvisYawCurrent) == 0x23e8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, gaitSplitFraction) == 0x2534);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, layerBlendAlpha) == 0x2538);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, overrideBlendTime) == 0x2594);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, insideRectangleScale) == 0x2630);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, drawOverrideData) == 0x2704);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, previousRootPosition) == 0x2484);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, requestedRootYawOffset) == 0x24b8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transformSource) == 0x27c0);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transformSourceIndex) == 0x27c4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transformOffsetPosition) == 0x27c8);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, transformOffsetRotation) == 0x27d4);
SHENMUE_MOTION_WORK_LAYOUT(offsetof(MotionWork, overrideViewSpaceTransform) == 0x27e0);
const std::byte *mtwkFindFrameEvent(const MotionWork *work, std::int32_t eventType, std::int32_t occurrence);
const std::byte *mtwkFindHandPoseEvent(const MotionWork *work, std::int32_t occurrence);
static const std::uint16_t targetFacingMotions[5][20] = {{0xf095, 0xf091, 0xf096, 0xf092, 0xf08c, 0xf08a, 0xf08e, 0xf123, 0xf127, 0xf125, 0xf094, 0xf090,
                                                             0xf093, 0xf08f, 0xf08b, 0xf089, 0xf08d, 0xf122, 0xf126, 0xf124},
    {0xf04e, 0xf049, 0xf04f, 0xf04a, 0xf044, 0xf042, 0xf046, 0xf123, 0xf127, 0xf125, 0xf04d, 0xf048, 0xf04c, 0xf047, 0xf043, 0xf041, 0xf045, 0xf122, 0xf126,
        0xf124},
    {0xf06f, 0xf06b, 0xf070, 0xf06c, 0xf066, 0xf064, 0xf068, 0xf123, 0xf127, 0xf125, 0xf06e, 0xf06a, 0xf06d, 0xf069, 0xf065, 0xf063, 0xf067, 0xf122, 0xf126,
        0xf124},
    {0xf0e0, 0xf0dc, 0xf0e1, 0xf0dd, 0xf0d7, 0xf0d5, 0xf0d9, 0xf123, 0xf127, 0xf125, 0xf0df, 0xf0db, 0xf0de, 0xf0da, 0xf0d6, 0xf0d4, 0xf0d8, 0xf122, 0xf126,
        0xf124},
    {0}};
void mtwkGetArmHandPoseChanges(MotionWork *work, std::int32_t *shapes, std::int32_t *frames);
struct MT7Node;
void CopyRecursiveModelNodeTransform(MT7Node *node, std::uint32_t selector, Matrix4x4 *output);
void CopyCharacterSourceTransform(const Character *source, std::uint32_t selector, Matrix4x4 *output);
void mtwkApplyRootPos(MotionWork *work);
void mtwkAdvanceInterpolated(MotionWork *work, float x, float y, float z);
void mtwkCopyInterpolated(MotionWork *work, float x, float y, float z);
extern std::int32_t g_mtwkCurrentIsRyo;
void MotionWorkUpdate(Character *character, void *data);
void mtwkCreateForChara(Character *character, std::int32_t commonWork);
void BindMotionControllerMatricesToModelNodes(MotionWork *work, MT7Node *model);
void mtwkBindMirroredControllerMatricesToModelNodes(MotionWork *work, MT7Node *model);
void DrawMotionParts(Character *character, MotionWork *work);
bool mtwkCopyTransition(MotionWork *work, const Vector3 *position, std::int32_t yaw, std::int32_t operation, std::int32_t copyType);
bool mtwkRequestPositionTransition(MotionWork *work, const Vector3 *position, std::int32_t yaw, std::uint32_t motion, float startFrame);
bool mtwkRequestTargetFacing(MotionWork *work, const Vector3 *position, std::uint32_t flags);
void mtwkUpdateCollisionCluster(MotionWork *work);
std::uint32_t mtwkApplyTransforms(const Matrix4x4 *pelvis);
Matrix4x4 mtwkBuildArmRebaseTransform(MotionWork *work);
void mtwkRebaseSolvedMatrices(MotionWork *work);
bool mtwkSetIkTargetPosition(MotionWork *work, Vector3 *position);
void mtwkSetActive(MotionWork *work);
void mtwkGetRootPosition(const MotionWork *work, Vector3 *position);
void mtwkSetRootPosition(MotionWork *work, const Vector3 *position);
void mtwkUpdateRootDeltaVector(const MotionWork *work, Vector3 *delta);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void mtwkSampleCurrentMotion(MotionWork *work, float frame, Vector3 *output);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void mtwkTransformWithoutIKEnabled(MotionWork *work, Vector3 *foot, Vector3 *pelvis);
#endif
void mtwkGetEntityRotation(const MotionWork *work, Vector3i *rotation);
std::int32_t mtwkGetEntityYaw(const MotionWork *work);
bool charIsMotionworkEnabled(Character *character);
bool charIsMotionworkCulled(Character *character);
void ReleaseMotionRuntime(MotionWork *work);
void MotionWorkCleanup(Character *character, void *payload);
void InitializeMotionTargetOverrides(MotionWork *work);
void InitializeMotionRotationPointers(MotionWork *work);
void InitializeMotionClockRates(MotionWork *work);
void InitializeMotionRuntime(MotionWork *work, std::uint32_t rigType);
std::int32_t charGetMotionRigType(const Character *character);
void mtwkSetEntityRotation(MotionWork *work, const Vector3i *rotation);
std::uint32_t mtwkSetProjectionAngle(MotionWork *work, std::int32_t angle, std::int32_t slot);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
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
float mtwkTimePairClampAndPropagate(MotionWork *work, float split, float ticks, float maximum, MotionRegion region);
void mtwkAssignStaticDataForBodyRegion(MotionWork *work, float split, float ticks, float maximum, MotionRegion region);
std::uint32_t TimestepUpdate(MotionRigClock *clock, std::uint32_t allowOverflow, std::uint32_t allowReset);
std::uint32_t AnimClock_ForceJump(MotionRigClock *clock);
std::int32_t mtwkRigEvaluateControllers(
    MotionWork *work, const MotionStreamContext *context, float sourceTime, float time, std::uint32_t flags, MotionRegion region);
bool Rig_SetupMotionGraph(IKRig &rig, const MotionStreamContext *context, float time, std::uint32_t flags, std::uint32_t region,
    MotionTargetParameters *targets, MotionRotationParameters *rotations);
void mtwkRigRunOverrideBlendPass(MotionWork *work);
bool mtwkClearOverrideCallbacksForBodyRegion(MotionWork *work, MotionRegion region);
void mtwkInstallActiveOverrideCallbacks(MotionWork *work);
void mtwkInstallConstraintTargetCallback(MotionWork *work, std::uint32_t controller);
void mtwkClearConstraintTargetCallbackIfInactive(MotionWork *work, std::uint32_t controller);
void mtwkUpdatePelvisYaw(MotionWork *work);
void mtwkSetPelvisYawStep(MotionWork *work, std::int16_t step);
std::int32_t mtwkUpdateOverrideCallbacks(MotionWork *work);
std::uint32_t HasLeadFootCommand(std::uint16_t motion);
void mtwkApplyRootPos(MotionWork *work);
void mtwkApplyRootTranslation(MotionWork *work);
void mtwkProcessLayers(MotionWork *work);
void mtwkInitFlags(MotionWork *work, std::uint32_t region);
bool mtwkHasRegionTracks(const MotionWork *work);
bool mtwkIsRightFootLower(MotionWork *work);
void mtwkSetRootTransformOverride(MotionWork *work, const Matrix4x4 *transform);
void mtwkSetEnvironmentEnabled(MotionWork *work, std::uint32_t enabled);
void mtwkTranslateRigInterpolated(MotionWork *work, float x, float y, float z);
bool mtwkSetDrawOverrideData(MotionWork *work, const Matrix4x4 *matrix);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const Matrix4x4 *mtwkGetDrawOverrideData(const MotionWork *work);
#endif
void mtwkAdvanceAnimFrameAndLayers(MotionWork *work);
void mtwkApplySequenceCommands(MotionWork *work);
void mtwkAdvanceInterpolated(MotionWork *work, float x, float y, float z);
void mtwkCopyInterpolated(MotionWork *work, float x, float y, float z);
std::uint32_t mtwkApplyTransforms(const Matrix4x4 *pelvis);
void mtwkResolveAnimId(MotionWork *work, std::uint16_t motion, std::uint16_t remap, float begin, float end);
void mtwkUpdateBoneForTime(MotionWork *work, std::uint16_t motion, float begin, float end);
void mtwkSelectRandomFootstepCue(MotionWork *work, std::uint16_t motion, float begin, float end);
void mtwkUpdateCollisionCluster(MotionWork *work);
std::int32_t mtwkApplyInterpolatedTransforms(MotionWork *work);
std::int32_t mtwkEvaluateMotionFrame(MotionWork *work);

extern std::int32_t g_mtwkCurrentIsRyo;
#endif
extern MotionWork *g_activeMotionWork;

void mtwkGetRegionClocks(MotionWork *work, MotionRegionClock **lower, MotionRegionClock **chest, MotionRegionClock **head, MotionRegionClock **rightArm,
    MotionRegionClock **leftArm);
std::uint32_t mtwkGetClipMode(const MotionWork *work);
void mtwkSetClipMode(MotionWork *work, std::int32_t mode);
bool mtwkSetSequenceFlags(Character *character);
void SetCharacterMotionEnabled(Character *character, std::int32_t enabled);
void mtwkSetEnabled(MotionWork *work, std::int32_t enabled);
void SetCharacterMotionClipMode(Character *character, std::int32_t mode);
const Matrix4x4 *mtwkGetBoneTransform(const MotionWork *work, std::int32_t boneIndex);
bool mtwkCopyBoneTransform(const MotionWork *work, std::int32_t boneIndex, Matrix4x4 *output);
const Matrix4x4 *mtwkGetBoneTransform(const Character *character, std::int32_t boneIndex);
MotionWork *charFindMotionwork(const Character *character);
MotionWork *scenFindMotionwork(shen_name name);
bool mtwkGetBonePosition(MotionWork *work, MotionBoneId bone, Vector3 *position);
std::uint32_t mtwkGetRigBoneId(const MotionWork *work);
std::uint32_t GetDefaultMotionTableIndex(std::uint32_t rig);
void mtwkSetUnknownField(MotionWork *work, std::uint32_t value);
bool mtwkGetParentBoneWrapper(MotionWork *work, std::int32_t selector);
std::uint32_t mtwkSetTimeRelated(MotionWork *work, std::uint32_t mode, std::uint16_t slot, std::uint16_t motionId);
extern const std::uint16_t defaultMotionIds[17][6];
std::uint16_t mtwkGetAnimationId(const MotionWork *work, MotionRegion region);
std::uint16_t mtwkGetQueuedAnimationId(const MotionWork *work, MotionRegion region);
bool mtwkIsMotionNonLooping(const MotionWork *work, std::uint32_t region);
bool mtwkRequestMotion(MotionWork *work, std::int32_t motion, MotionRegion region);
void mtwkRequestRegionDetach(MotionWork *work, MotionRegion region);
std::uint32_t mtwkSetExtraFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation);
std::uint32_t mtwkSetRegionFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation, MotionRegion region);
std::uint32_t mtwkUpdateWithPosition(MotionWork *work, const Vector3 *position, std::uint32_t mode);
extern std::int32_t g_motionUseEnvironment;
void mtwkSetUseEnvFlag(std::int32_t enabled);
extern std::int32_t g_motionDrawOverridesEnabled;
std::int32_t mtwkSetOverridesEnabled(std::int32_t enabled);
std::uint32_t mtwkIsEnabled(const MotionWork *work);
bool mtwkSetFrameTime(MotionWork *work, std::uint32_t frames, MotionRegion region);
void mtwkSetRegionEvalTime(MotionWork *work, float frame, MotionRegion region);
void mtwkSetLayerLoopStartTime(MotionWork *work, float frame, MotionRegion region);
void mtwkSetLayerEndFrame(MotionWork *work, float frame, MotionRegion region);
void mtwkSetLayerFrameRange(MotionWork *work, float loopStart, float start, float end, MotionRegion region);
void mtwkSetReactFlags(MotionWork *work, std::uint8_t flags);
bool mtwkSetRequestedRootMode(MotionWork *work, std::uint32_t mode);
bool mtwkAssignRigYawOffset(MotionWork *work, std::int32_t offset);
std::uint32_t mtwkGetTrackedRootMode(const MotionWork *work);
void mtwkCopyRegionTimes(const MotionWork *work, float *split, float *current, float *maximum, MotionRegion region);
float mtwkGetSplitFrame(const MotionWork *work, MotionRegion region);
float mtwkGetFrame(const MotionWork *work, MotionRegion region);
float mtwkGetEndFrame(const MotionWork *work, MotionRegion region);
float mtwkGetSpeed(const MotionWork *work, MotionRegion region);
float mtwkGetTransitionFramesRemaining(const MotionWork *work, MotionRegion region);
void mtwkSetSpeed(MotionWork *work, float speed, MotionRegion region);
float mtwkSetRigScale(MotionWork *work, float scale);
float mtwkGetBoneTargetStepLimit(const MotionWork *work, MotionBoneId bone);
bool mtwkSetBoneTargetStepLimit(MotionWork *work, MotionBoneId bone, float limit);
bool mtwkSetBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *position);
bool mtwkLatchBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *position);
bool mtwkLatchControllerTarget(MotionWork *work, std::uint32_t controller, const Vector3 *position);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool charaLatchBoneTarget(Character *character, std::int32_t selector, const Vector3 *position);
#endif
std::int32_t ParentBoneId(std::uint32_t selector);
bool mtwkGetBoneTargetPosition(const MotionWork *work, std::int32_t selector, Vector3 *position);
bool mtwkGetControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position);
void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output);
void charaGetMotWorldPos(Character *character, std::int32_t bone, Vector3 *position);
float mtwkGetClusterPosition(const MotionWork *work, Vector3 *position);

} // namespace s2
} // namespace shenmue

#undef SHENMUE_MOTION_WORK_CONSTANT
#undef SHENMUE_MOTION_WORK_INITIALIZER
#undef SHENMUE_MOTION_WORK_LAYOUT
#undef SHENMUE_MOTION_WORK_LAYOUT_NAME
#undef SHENMUE_MOTION_WORK_LAYOUT_NAME_

#endif
