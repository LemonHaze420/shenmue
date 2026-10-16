#pragma once

#include "s2/date_time.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct Character;
struct MotionWork;
struct MotnPlayerMotionFlagCommand;
struct LegacyInputSlot;
using PlayerFlagCallback = std::int64_t (*)();
inline constexpr shen_name PLAYER_RETURN_AREA = SHEN_NAME(AR02);
inline constexpr shen_name PLAYER_COMPANION_REN{SHEN_NAME(REN_)};
inline constexpr shen_name PLAYER_COMPANION_JOY{SHEN_NAME(JOP_)};
inline constexpr shen_name PLAYER_COMPANION_SHENHUA{SHEN_NAME(SIN_)};
inline constexpr shen_name PLAYER_COMPANION_RETRY_AREA{SHEN_NAME(QR00)};

std::uint64_t ReturnZero() noexcept;

enum class PlayerGameFlagMode : std::int32_t {
    RestoreControl = 0,
    Bit2 = 2,
    Bit5 = 4,
    Bit26 = 5,
    SearchBlocked = 6,
    Bit29 = 7,
    Bit25 = 8,
    Bit30 = 9,
    Bit22 = 22,
    SetInteractionDistanceLimit = 23,
    GetInteractionDistanceLimit = 24,
    Bit12 = 25,
    DisableAreaControl = 30,
    EnableAreaControl = 31,
    QueryAreaControl = 32,
    SetPlacementTarget = 33,
    RetainMovementMode = 64,
};

enum class HeldStateMode : std::uint8_t {
    None = 0,
    SystemHold = 1,
    WristwatchSceneChange = 2,
    ScheduledSceneChange = 3,
    AdvanceTime = 5,
    Callback = 6,
    RetrySceneChange = 7,
};

inline constexpr std::uint16_t heldMode1Latched = 0x0002;
inline constexpr std::uint16_t heldMode2Latched = 0x0004;
inline constexpr std::uint16_t heldTimeAdvanceActive = 0x0008;
inline constexpr std::uint16_t heldScheduleEnabled = 0x0010;
inline constexpr std::uint16_t heldRetryEnabled = 0x0040;
inline constexpr std::uint16_t heldCallbackActive = 0x0080;
inline constexpr std::uint16_t heldTimeAdvanced = 0x0100;
inline constexpr std::uint16_t heldApplyRequestedClock = 0x0400;
inline constexpr std::uint16_t heldPreserveAcrossDay = 0x1000;
inline constexpr std::uint16_t heldTimedCallback = 0x4000;
inline constexpr std::uint16_t heldAlternateCallbackGate = 0x8000;
inline constexpr std::uint32_t controlHeldTransitionActive = 0x0080;
inline constexpr std::uint32_t controlHoldStateEnabled = 0x0100;



#pragma pack(push, 4)
struct PlayerMovementState {
    std::uint32_t flags;
    std::uint32_t field004;
    Character *character;
    MotionWork *motionWork;
    std::uint32_t dispatchMode;
    std::uint32_t requestedMode;
    std::uint32_t previousRequestedMode;
    std::byte reserved024[0x00c];
    std::int16_t currentYaw;
    std::int16_t targetYaw;
    std::int16_t field034;
    std::byte reserved036[0x002];
    std::int16_t rootYawOffset;
    std::int16_t pendingYawCorrection;
    std::int16_t motionTransitionParameter;
    std::int16_t reserved03e;
    float motionStartFrame;
    std::byte reserved044[0x004];
    float yawInterpolationRate;
    float field04c;
    std::byte reserved050[0x002];
    std::int8_t leadFootDirection;
    std::byte reserved053[0x001];
    std::uint32_t playerMotionId;
    std::uint32_t pendingMotionRequest;
    std::uint8_t rootYawMode;
    std::uint8_t field05d;
    std::uint8_t field05e;
    std::uint8_t field05f;
    std::int32_t parentBoneSelector;
    std::byte reserved064[0x008];
    std::uint32_t playerMotionFlags;
    std::int16_t motionEntityYaw;
    std::int16_t motionNodeYaw;
    std::int32_t reserved074;
    Vector3 motionRootEntityPosition;
    Vector3 motionCharacterPosition;
    std::byte reserved090[0x00c];
    std::int8_t groundedStepIndex;
    std::int8_t upperBodyStepIndex;
    std::int8_t previousStepIndex;
    std::int8_t stepDirection;
    std::int8_t targetHeightScale;
    std::byte reserved0a1[0x003];
    Vector3 targetWorldPosition;
    std::int16_t targetWorldYaw;
    std::byte reserved0b2[0x002];
    float targetStairEdgeLength;
    Vector3 rootConsumerPosition;
    Vector3 stairBoundaryAnchor;
    std::int16_t stairBoundaryYaw;
    std::byte reserved0d2[0x006];
    std::int8_t sceneGroundIndex;
    std::byte reserved0d9[0x003];
    std::uint32_t cachedCharacterControlFlags;
    float controllerGroundBlend[2];
    float controllerVerticalOffset[2];
    Vector3 controllerPreviousLocal[2];
    Vector3 controllerDelta[2];
    std::byte reserved120[0x008];
    float obstacleDistance;
    std::byte reserved12c[0x004];
    float pendingPositionCorrection;
    float correctionCycleDistance;
    std::byte reserved138[0x00c];
    std::int16_t correctionCycleCount;
    std::byte reserved146[0x002];
    const MotnPlayerMotionFlagCommand *playerMotionFlagCommands;
    const MotnPlayerMotionFlagCommand *playerMotionFlagCursor;
    float playerMotionFrame;
    float field15c;
    float field160;
    float rootMotionStep;
    float previousField15c;
    float field16c;
    float previousRigScale;
    std::uint32_t motionAnimationId;
    float motionSpeed;
    float movementResult;
    Vector3 transitionTargetPosition;
    std::uint16_t transitionTargetYaw;
    std::byte reserved18e[0x006];
    std::int16_t movingFrames;
    std::int16_t stationaryFrames;
    std::int16_t turningFrames;
    std::byte reserved19a[0x006];
};

#pragma pack(push, 1)
struct PlayerAutomaticLookState {
    std::uint8_t flags;
    std::uint8_t reserved01;
    std::int16_t pitchOffset;
    std::int16_t yawOffset;
    float elapsedFrames;
    float targetDurationFrames;
    std::int16_t authorChangeFramesRemaining;
    std::uint16_t reserved10;
    shen_name previousAuthorName;
};
#pragma pack(pop)
static_assert(sizeof(PlayerAutomaticLookState) == 0x16);

struct PlayerPlacementNode {
    std::uint32_t flags;
    std::uint32_t reserved004;
    Character *character;
    Vector3 position;
    Vector3 previousPosition;
    Vector3 delta;
    std::int16_t targetYaw;
    std::int16_t characterBearingYaw;
    float distance;
    float minimumDistance;
    std::byte reserved040[0x00a];
    std::int16_t movementBearingYaw;
    Vector3 targetPosition;
    Vector3 targetCharacterPosition;
    std::int16_t stationaryFrames;
    std::uint16_t reserved066;
    PlayerPlacementNode *next;
};
static_assert(sizeof(PlayerPlacementNode) == 0x70);
static_assert(offsetof(PlayerPlacementNode, position) == 0x10);
static_assert(offsetof(PlayerPlacementNode, targetYaw) == 0x34);
static_assert(offsetof(PlayerPlacementNode, next) == 0x68);

struct PlayerPlacementTarget {
    const Vector3 *position;
    std::uint16_t yaw;
};
static_assert(offsetof(PlayerPlacementTarget, yaw) == 0x08);

struct RyoBagModel;

struct RyoBagForm {
    shen_name name;
    std::uint32_t reserved04;
    RyoBagModel *bag;
    RyoBagModel *shadow;
};
static_assert(sizeof(RyoBagForm) == 0x18);

inline std::int16_t playerInteractionPlacementAngles[5]{0x2000, 0x1000, -0x1000, -0x2000, 0};
inline Vector3 playerInteractionPlacementOffset{0.0f, 0.0f, -1.25f};

inline std::int32_t playerSpecialRightHandPoseCommand[19][3]{
    {1999,65354,62444}, {1638,180,63900}, {546,64719,3365}, {0,64810,60984}, {0,64628,61893}, {0,0,64261},
    {0,0,64628}, {0,0,60074}, {0,0,59710}, {0,65172,63353}, {0,0,65354}, {0,729,61165}, {0,65173,58073},
    {0,65172,64444}, {0,0,0}, {0,547,57161}, {0,1639,61165}, {0,64810,64080}, {64080,182,0}
};

struct PlayerModelState {
    std::uint32_t flags; // input mode flags
    std::uint32_t padFlags;
    std::uint32_t previousPadFlags;
    std::uint8_t useExternalMovementState;
    std::uint8_t inputActive;
    std::uint8_t motionInputState;
    std::uint8_t turnaroundState;
    std::int8_t stickX;
    std::int8_t stickY;
    std::int16_t accumulatedStickX;
    std::int16_t accumulatedStickY;
    std::uint16_t direction;
    std::byte reserved018[0x002];
    std::uint16_t risingDirection;
    std::uint16_t fallingDirection;
    std::byte reserved01e[0x002];
    float stickMagnitude;
    float accumulatedMagnitude;
    std::uint16_t horizontalHold;
    std::uint16_t verticalHold;
    std::int8_t stickHistoryX;
    std::int8_t previousStickX;
    std::int8_t previousPreviousStickX;
    std::int8_t stickHistoryY;
    std::int8_t previousStickY;
    std::int8_t previousPreviousStickY;
    std::int8_t smoothedStickX;
    std::int8_t smoothedStickY;
    std::uint32_t buttonHoldFrames[4];
    std::int16_t isLookAround;
    std::int16_t lookAroundFrames;
    std::int16_t inputBaseYaw;
    std::int16_t turnInput;
    std::int16_t inputTargetYaw;
    std::int16_t currentYaw;
    std::byte reserved050[0x002];
    std::int16_t desiredYaw;
    std::int16_t committedYaw;
    std::int16_t yawDelta;
    Vector3 searchLocation;
    std::uint16_t directionLatchState;
    std::uint16_t directionLatchValue;
    std::uint16_t packedDirection;
    std::byte reserved06a[0x002];
    std::uint8_t analogHistory0;
    std::uint8_t analogHistory1;
    std::uint8_t analogHistory2;
    std::uint8_t field06f;
    float field070;
    float movementSpeedRatio;
    std::byte reserved078[0x00c];
    float turnSpeedScale;
    std::int32_t field088;
    float interpolationValue;
    float normalizedMotionInput;
    float motionInputEcho;
    float movementBlendOutput;
    float movementBlendLimit;
    std::byte reserved0a0[0x004];
    std::uint8_t lookZoomInput;
    std::uint8_t lookAroundState;
    std::int8_t lookInputGraceFrames;
    std::uint8_t lookInputFlags;
    std::byte reserved0a8[0x002];
    std::int16_t lookPitchDelta;
    std::int16_t lookYawDelta;
    std::int16_t lookBasePitch;
    std::int16_t lookBaseYaw;
    std::byte reserved0b2[0x002];
    Vector3 lookAtPosition;
    std::uint8_t lookTargetMode;
    std::uint8_t lookTargetFlags;
    PlayerAutomaticLookState automaticLook;
    float automaticLookIdleTimer;
    std::uint16_t forcedYaw;
    std::uint16_t field0de;
    std::uint16_t yaw;
    std::int16_t overrideYaw;
    std::uint32_t movementMode;
    std::uint32_t previousInputMovementMode;
    std::uint32_t latchedMovementMode;
    std::uint32_t activeMovementMode;
    std::uint32_t requestedMovementMode;
    std::uint32_t frameCount;
    std::uint32_t activeFrameCount;
    std::byte reserved100[0x004];
    std::uint8_t collisionProbeState;
    std::byte reserved105[0x003];
    float forwardProbeDistance;
    std::int32_t field10c;
    float desiredYawProbeDistance;
    std::byte reserved114[0x008];
    std::int16_t forwardSurfaceYaw;
    std::byte reserved11e[0x002];
    float nearestCharaProbeDistance;
    std::byte reserved124[0x006];
    std::int16_t requestedTurnYaw;
    std::int32_t field12c;
    std::uint32_t movementSelectionFlags;
    std::uint32_t previousMovementSelectionFlags;
    std::byte reserved138[0x00c];
    float leftSideProbeDistance;
    float rightSideProbeDistance;
    std::int16_t hitNormalYaw;
    std::int16_t collisionBoundaryYaw;
    std::int16_t desiredProbeYaw;
    std::int16_t secondaryCollisionBoundaryYaw;
    std::int16_t collisionLimitYawA;
    std::int16_t collisionLimitYawB;
    std::byte reserved158[0x00c];
    std::int16_t resolvedTurnYaw;
    std::int16_t pendingTurnDelta;
    std::int16_t inactiveDirection168;
    std::uint16_t collisionReferenceYaw;
    std::int16_t collisionLatchYaw;
    std::byte reserved16e[0x006];
    std::int16_t collisionTurnDelta;
    std::int16_t previousCollisionTurnDelta;
    std::int32_t field178;
    float limitingCollisionDistance;
    std::byte reserved180[0x002];
    std::int16_t inactiveDirection182;
    std::byte reserved184[0x004];
    std::int16_t inactiveDirection188;
    std::int16_t turnYawDelta;
    std::int16_t field18c;
    std::int16_t collisionSideCounter;
    std::int16_t collisionAvoidanceYaw;
    std::byte reserved192[0x006];
    std::int8_t movementSubtype;
    std::byte reserved199[0x001];
    std::int16_t groundBoundaryTurnYawDelta;
    std::uint32_t collisionData;
    std::uint8_t helperFlags;
    std::int8_t activeHelperMode;
    std::int8_t pendingHelperMode;
    std::byte reserved1a3[0x005];
    Character *helperCharacter;                // +0x1a8, FORM bag owner
    Character *characterNode;
    MotionWork *motionWork;
    const LegacyInputSlot *inputSnapshot;
    std::uint32_t rootYawState;
    std::uint32_t previousRootYawState;
    PlayerMovementState *movementState;
    PlayerMovementState embeddedMovementState;
    shen_name interactionTarget;
    float field37c;
    float interactionRangeRatio;
    float secondaryInteractionDistanceLimit;
    std::uint16_t idleNearTargetFrames;
    std::uint16_t interactionTargetCounter;
    shen_name specialModeAuthorName;
    Vector3 specialModeTargetPosition;
    std::uint32_t specialModeFlags;
    std::uint8_t specialModeState;
    std::uint8_t specialModeAuxState;
    std::uint16_t specialModeCounter;
    std::byte reserved3a4[0x005];
    std::uint8_t specialModeTurnSide;
    std::byte reserved3aa[0x002];
    std::uint32_t specialModeTargetId;
    std::int32_t specialModeOrbitPhase;
    float specialModeOrbitMinimum;
    float specialModeOrbitMaximum;
    std::uint32_t specialModeFrame;
    std::uint8_t specialModeStep;
    std::uint8_t reserved3c1;
    std::uint16_t heldStateFlags;
    std::uint8_t heldStateMode;
    std::uint8_t heldStateStep;
    std::uint8_t pendingHeldStateMode;
    std::byte reserved3c7[0x001];
    std::int16_t heldStateTimer;
    std::int16_t heldStateTriggerMinute;
    std::byte reserved3cc[0x004];
    PlayerFlagCallback heldStateCallback;
    std::int16_t holdWindowStartMinute;
    std::int16_t holdWindowEndMinute;
    std::int16_t sceneActId;
    std::byte reserved3de[0x002];
    shen_name sceneAreaId;
    std::int16_t sceneEntryId;
    std::int16_t scheduledTransitionMinute;
    std::int16_t requestedActId;
    std::byte reserved3ea[0x002];
    shen_name requestedAreaId;
    std::int16_t requestedEntryId;
    std::byte reserved3f2[0x01e];
    std::int16_t retryActId;
    std::byte reserved412[0x002];
    shen_name retryAreaId;
    std::int16_t retryEntryId;
    std::uint8_t timeAdvanceTargetHour;
    std::uint8_t timeAdvanceTargetMinute;
    std::uint8_t timeAdvanceMinutesPerTick;
    std::uint8_t requestedClockHour;
    std::uint8_t requestedClockMinute;
    std::uint8_t rotationOnlyWaitFrames;
    std::uint16_t turnaroundFrames;
    std::uint16_t turnaroundPhase;
    std::byte reserved424[0x012];
    std::uint8_t field436;
    std::byte reserved437[0x019];
    PlayerPlacementNode placementRoot;
    PlayerPlacementNode placementScratch;
    PlayerPlacementNode *activePlacementNode;
    PlayerPlacementNode *selectedPlacementNode;
    float movementResult;
    float previousMovementResult;
    float approachStepDistance;
    float remainingMotionTime;
    float speed;
    std::uint32_t reserved554;
    std::uint32_t searchRegionKey;
    std::uint8_t regionInteractionState;
    std::byte reserved55d[0x003];
    std::uint32_t regionInteractionMotionId;
    shen_name lockOnCharacterName;
    Vector3 worldPosition;
    std::uint32_t movementQueryState;
    Vector3 fieldCollisionPoint;
    std::uint32_t reserved584;
    Character *collisionCharacter;
    float collisionDistance;
    std::uint8_t collisionFlags;
    std::uint8_t collisionType;
    std::byte reserved596[0x002];
    Matrix4x4 worldTransform;
    Matrix4x4 inverseWorldTransform;
    Character *limitingCollisionCharacter;
    Character *nearestCollisionCharacter;
    Character *primaryRayCollisionCharacter;
    std::uint8_t placementHistoryWriteIndex;
    std::uint8_t placementHistoryCount;
    std::byte reserved632[2];
    Vector3 placementHistory[32];
    Vector3 previousPlacementSample;
    float placementHistoryDistanceAccumulator;
    float field7c4;
    std::int32_t placementQueryHandle;
    std::uint8_t placementTargetState;
    std::byte reserved7cd[3];
};

struct PlayerRuntimeState {
    std::uint32_t flags;
    std::int32_t field04;
    std::int32_t field08;
    std::int32_t currentDay;
    Character *character;
    std::uint64_t field18;
    PlayerModelState *model;
    std::uint16_t savedHeldStateFlags;
    std::int16_t scheduledCallbackMinute;
    std::uint32_t field2c;
    std::uint64_t field30[2];
};

struct PlayerPersistentState {
    std::uint32_t present;
    std::int32_t runtimeState;
    std::uint16_t playerStateFlags;
    std::uint16_t callbackValue;
    std::uint32_t reserved;
};
#pragma pack(pop)

static_assert(sizeof(PlayerMovementState) == 0x1a0);
static_assert(offsetof(PlayerMovementState, previousRigScale) == 0x170);
static_assert(offsetof(PlayerMovementState, motionAnimationId) == 0x174);
static_assert(offsetof(PlayerMovementState, movingFrames) == 0x194);
static_assert(offsetof(PlayerMovementState, rootYawOffset) == 0x038);
static_assert(offsetof(PlayerMovementState, yawInterpolationRate) == 0x048);
static_assert(offsetof(PlayerMovementState, stairBoundaryAnchor) == 0x0c4);
static_assert(offsetof(PlayerMovementState, stairBoundaryYaw) == 0x0d0);
static_assert(offsetof(PlayerMovementState, sceneGroundIndex) == 0x0d8);
static_assert(offsetof(PlayerMovementState, rootYawMode) == 0x05c);
static_assert(offsetof(PlayerMovementState, character) == 0x008);
static_assert(offsetof(PlayerMovementState, motionWork) == 0x010);
static_assert(offsetof(PlayerMovementState, dispatchMode) == 0x018);
static_assert(offsetof(PlayerMovementState, requestedMode) == 0x01c);
static_assert(offsetof(PlayerMovementState, currentYaw) == 0x030);
static_assert(offsetof(PlayerMovementState, targetYaw) == 0x032);
static_assert(offsetof(PlayerMovementState, pendingYawCorrection) == 0x03a);
static_assert(offsetof(PlayerMovementState, field05f) == 0x05f);
static_assert(offsetof(PlayerMovementState, playerMotionId) == 0x054);
static_assert(offsetof(PlayerMovementState, playerMotionFlags) == 0x06c);
static_assert(offsetof(PlayerMovementState, playerMotionFlagCommands) == 0x148);
static_assert(offsetof(PlayerMovementState, playerMotionFlagCursor) == 0x150);
static_assert(offsetof(PlayerMovementState, playerMotionFrame) == 0x158);
static_assert(offsetof(PlayerMovementState, controllerGroundBlend) == 0x0e0);
static_assert(offsetof(PlayerMovementState, controllerVerticalOffset) == 0x0e8);
static_assert(offsetof(PlayerMovementState, controllerPreviousLocal) == 0x0f0);
static_assert(offsetof(PlayerMovementState, controllerDelta) == 0x108);
static_assert(offsetof(PlayerMovementState, parentBoneSelector) == 0x060);
static_assert(offsetof(PlayerMovementState, targetHeightScale) == 0x0a0);
static_assert(offsetof(PlayerMovementState, targetWorldPosition) == 0x0a4);
static_assert(offsetof(PlayerMovementState, targetWorldYaw) == 0x0b0);
static_assert(offsetof(PlayerMovementState, groundedStepIndex) == 0x09c);
static_assert(offsetof(PlayerMovementState, previousStepIndex) == 0x09e);
static_assert(offsetof(PlayerMovementState, stepDirection) == 0x09f);
static_assert(offsetof(PlayerMovementState, targetStairEdgeLength) == 0x0b4);
static_assert(offsetof(PlayerMovementState, rootConsumerPosition) == 0x0b8);
static_assert(offsetof(PlayerMovementState, sceneGroundIndex) == 0x0d8);
static_assert(offsetof(PlayerMovementState, cachedCharacterControlFlags) == 0x0dc);
static_assert(offsetof(PlayerMovementState, field15c) == 0x15c);
static_assert(offsetof(PlayerMovementState, obstacleDistance) == 0x128);
static_assert(offsetof(PlayerMovementState, pendingPositionCorrection) == 0x130);
static_assert(offsetof(PlayerMovementState, correctionCycleDistance) == 0x134);
static_assert(offsetof(PlayerMovementState, correctionCycleCount) == 0x144);
static_assert(offsetof(PlayerMovementState, rootMotionStep) == 0x164);
static_assert(offsetof(PlayerMovementState, previousField15c) == 0x168);
static_assert(offsetof(PlayerMovementState, field16c) == 0x16c);
static_assert(offsetof(PlayerModelState, yaw) == 0x0e0);
static_assert(offsetof(PlayerModelState, searchLocation) == 0x058);
static_assert(offsetof(PlayerModelState, inputActive) == 0x00d);
static_assert(offsetof(PlayerModelState, stickX) == 0x010);
static_assert(offsetof(PlayerModelState, risingDirection) == 0x01a);
static_assert(offsetof(PlayerModelState, fallingDirection) == 0x01c);
static_assert(offsetof(PlayerModelState, buttonHoldFrames) == 0x034);
static_assert(offsetof(PlayerModelState, isLookAround) == 0x044);
static_assert(offsetof(PlayerModelState, inputBaseYaw) == 0x048);
static_assert(offsetof(PlayerModelState, currentYaw) == 0x04e);
static_assert(offsetof(PlayerModelState, desiredYaw) == 0x052);
static_assert(offsetof(PlayerModelState, yawDelta) == 0x056);
static_assert(offsetof(PlayerModelState, directionLatchState) == 0x064);
static_assert(offsetof(PlayerModelState, field070) == 0x070);
static_assert(offsetof(PlayerModelState, motionInputEcho) == 0x094);
static_assert(offsetof(PlayerModelState, lookZoomInput) == 0x0a4);
static_assert(offsetof(PlayerModelState, automaticLook) == 0x0c2);
static_assert(offsetof(PlayerModelState, automaticLookIdleTimer) == 0x0d8);
static_assert(offsetof(PlayerModelState, searchRegionKey) == 0x558);
static_assert(offsetof(PlayerModelState, forcedYaw) == 0x0dc);
static_assert(offsetof(PlayerModelState, frameCount) == 0x0f8);
static_assert(offsetof(PlayerModelState, forwardProbeDistance) == 0x108);
static_assert(offsetof(PlayerModelState, desiredYawProbeDistance) == 0x110);
static_assert(offsetof(PlayerModelState, forwardSurfaceYaw) == 0x11c);
static_assert(offsetof(PlayerModelState, nearestCharaProbeDistance) == 0x120);
static_assert(offsetof(PlayerModelState, hitNormalYaw) == 0x14c);
static_assert(offsetof(PlayerModelState, collisionBoundaryYaw) == 0x14e);
static_assert(offsetof(PlayerModelState, collisionLimitYawB) == 0x156);
static_assert(offsetof(PlayerModelState, limitingCollisionDistance) == 0x17c);
static_assert(offsetof(PlayerModelState, collisionData) == 0x19c);
static_assert(offsetof(PlayerModelState, helperFlags) == 0x1a0);
static_assert(offsetof(PlayerModelState, pendingHelperMode) == 0x1a2);
static_assert(offsetof(PlayerModelState, helperCharacter) == 0x1a8);
static_assert(offsetof(PlayerModelState, characterNode) == 0x1b0);
static_assert(offsetof(PlayerModelState, inputSnapshot) == 0x1c0);
static_assert(offsetof(PlayerModelState, rootYawState) == 0x1c8);
static_assert(offsetof(PlayerModelState, movementState) == 0x1d0);
static_assert(offsetof(PlayerModelState, embeddedMovementState) == 0x1d8);
static_assert(offsetof(PlayerModelState, embeddedMovementState) + offsetof(PlayerMovementState, previousRigScale) == 0x348);
static_assert(offsetof(PlayerModelState, field37c) == 0x37c);
static_assert(offsetof(PlayerModelState, specialModeAuthorName) == 0x38c);
static_assert(offsetof(PlayerModelState, specialModeFlags) == 0x39c);
static_assert(offsetof(PlayerModelState, specialModeFrame) == 0x3bc);
static_assert(offsetof(PlayerModelState, heldStateFlags) == 0x3c2);
static_assert(offsetof(PlayerModelState, heldStateMode) == 0x3c4);
static_assert(offsetof(PlayerModelState, heldStateStep) == 0x3c5);
static_assert(offsetof(PlayerModelState, pendingHeldStateMode) == 0x3c6);
static_assert(offsetof(PlayerModelState, heldStateTimer) == 0x3c8);
static_assert(offsetof(PlayerModelState, heldStateTriggerMinute) == 0x3ca);
static_assert(offsetof(PlayerModelState, heldStateCallback) == 0x3d0);
static_assert(offsetof(PlayerModelState, holdWindowStartMinute) == 0x3d8);
static_assert(offsetof(PlayerModelState, holdWindowEndMinute) == 0x3da);
static_assert(offsetof(PlayerModelState, sceneActId) == 0x3dc);
static_assert(offsetof(PlayerModelState, sceneAreaId) == 0x3e0);
static_assert(offsetof(PlayerModelState, sceneEntryId) == 0x3e4);
static_assert(offsetof(PlayerModelState, scheduledTransitionMinute) == 0x3e6);
static_assert(offsetof(PlayerModelState, requestedActId) == 0x3e8);
static_assert(offsetof(PlayerModelState, requestedAreaId) == 0x3ec);
static_assert(offsetof(PlayerModelState, requestedEntryId) == 0x3f0);
static_assert(offsetof(PlayerModelState, retryActId) == 0x410);
static_assert(offsetof(PlayerModelState, retryAreaId) == 0x414);
static_assert(offsetof(PlayerModelState, retryEntryId) == 0x418);
static_assert(offsetof(PlayerModelState, timeAdvanceTargetHour) == 0x41a);
static_assert(offsetof(PlayerModelState, timeAdvanceMinutesPerTick) == 0x41c);
static_assert(offsetof(PlayerModelState, rotationOnlyWaitFrames) == 0x41f);
static_assert(offsetof(PlayerModelState, turnaroundPhase) == 0x422);
static_assert(offsetof(PlayerModelState, movementSelectionFlags) == 0x130);
static_assert(offsetof(PlayerModelState, leftSideProbeDistance) == 0x144);
static_assert(offsetof(PlayerModelState, resolvedTurnYaw) == 0x164);
static_assert(offsetof(PlayerModelState, collisionLatchYaw) == 0x16c);
static_assert(offsetof(PlayerModelState, collisionTurnDelta) == 0x174);
static_assert(offsetof(PlayerModelState, turnYawDelta) == 0x18a);
static_assert(offsetof(PlayerModelState, collisionAvoidanceYaw) == 0x190);
static_assert(offsetof(PlayerModelState, movementSubtype) == 0x198);
static_assert(offsetof(PlayerModelState, groundBoundaryTurnYawDelta) == 0x19a);
static_assert(offsetof(PlayerModelState, turnSpeedScale) == 0x084);
static_assert(offsetof(PlayerModelState, movementBlendLimit) == 0x09c);
static_assert(offsetof(PlayerModelState, lookBaseYaw) == 0x0b0);
static_assert(offsetof(PlayerModelState, activeMovementMode) == 0x0f0);
static_assert(offsetof(PlayerModelState, movementResult) == 0x540);
static_assert(offsetof(PlayerModelState, field436) == 0x436);
static_assert(offsetof(PlayerModelState, placementRoot) == 0x450);
static_assert(offsetof(PlayerModelState, worldPosition) == 0x568);
static_assert(offsetof(PlayerModelState, fieldCollisionPoint) == 0x578);
static_assert(offsetof(PlayerModelState, collisionCharacter) == 0x588);
static_assert(offsetof(PlayerModelState, collisionDistance) == 0x590);
static_assert(offsetof(PlayerModelState, collisionFlags) == 0x594);
static_assert(offsetof(PlayerModelState, collisionType) == 0x595);
static_assert(offsetof(PlayerModelState, placementScratch) + offsetof(PlayerPlacementNode, next) == 0x528);
static_assert(offsetof(PlayerModelState, activePlacementNode) == 0x530);
static_assert(offsetof(PlayerModelState, selectedPlacementNode) == 0x538);
static_assert(offsetof(PlayerModelState, placementScratch) == 0x4c0);
static_assert(offsetof(PlayerModelState, placementQueryHandle) == 0x7c8);
static_assert(offsetof(PlayerModelState, remainingMotionTime) == 0x54c);
static_assert(offsetof(PlayerModelState, speed) == 0x550);
static_assert(offsetof(PlayerModelState, regionInteractionState) == 0x55c);
static_assert(offsetof(PlayerModelState, regionInteractionMotionId) == 0x560);
static_assert(offsetof(PlayerModelState, worldPosition) == 0x568);
static_assert(offsetof(PlayerModelState, worldTransform) == 0x598);
static_assert(offsetof(PlayerModelState, inverseWorldTransform) == 0x5d8);
static_assert(offsetof(PlayerModelState, limitingCollisionCharacter) == 0x618);
static_assert(offsetof(PlayerModelState, primaryRayCollisionCharacter) == 0x628);
static_assert(offsetof(PlayerModelState, placementQueryHandle) == 0x7c8);
static_assert(offsetof(PlayerMovementState, pendingMotionRequest) == 0x058);
static_assert(offsetof(PlayerModelState, interactionRangeRatio) == 0x380);
static_assert(offsetof(PlayerModelState, lockOnCharacterName) == 0x564);
static_assert(offsetof(PlayerModelState, placementHistoryWriteIndex) == 0x630);
static_assert(offsetof(PlayerModelState, placementHistoryCount) == 0x631);
static_assert(offsetof(PlayerModelState, placementHistory) == 0x634);
static_assert(offsetof(PlayerModelState, previousPlacementSample) == 0x7b4);
static_assert(offsetof(PlayerModelState, placementHistoryDistanceAccumulator) == 0x7c0);
static_assert(sizeof(PlayerModelState) == 0x7d0);
static_assert(offsetof(PlayerRuntimeState, field08) == 0x08);
static_assert(offsetof(PlayerRuntimeState, character) == 0x10);
static_assert(offsetof(PlayerRuntimeState, model) == 0x20);
static_assert(offsetof(PlayerRuntimeState, savedHeldStateFlags) == 0x28);
static_assert(offsetof(PlayerRuntimeState, scheduledCallbackMinute) == 0x2a);
static_assert(sizeof(PlayerRuntimeState) == 0x40);
static_assert(sizeof(PlayerPersistentState) == 0x10);

enum class PlayerSearchHand : std::uint8_t {
    Right,
    Left,
};

extern PlayerRuntimeState g_playerRuntime;

void InitializePlayerMovementState(PlayerMovementState &movement);
std::int32_t InitializePlayerSceneContexts(PlayerModelState &state);
void plymInitializeRyoBagState(PlayerModelState &state);
void UpdatePlayerMovementState(PlayerMovementState &movement, bool motionWorkReplaced = false);
Vector3 GetPlayerTargetWorldPosition();
std::int32_t RequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested);

std::int64_t InitializePlayerRuntime();
void plymSetDefaultFlags();
bool AttachPlayerModel(Character *character);
bool plymHasRyoBag(std::int32_t mode);
std::uint32_t GetCharacterControlModelFlags(Character *character);
void SetCharacterControlModelFlags(Character *character, std::uint32_t flags);
std::uint64_t SetPlayerControlEnabled(std::int32_t mode);

std::uint64_t SetPlayerGameFlags(PlayerGameFlagMode mode, void *parameter);
bool SetPlayerRootPosition(const Vector3 &position);
bool IsPlayerPositionTransitionActive();


std::uint32_t plymGetCurrentFlags();
std::int32_t plymIntersectionTest();
bool plymSampleLookInput(Vector2s &facing, Vector2s &look);
std::int32_t plymGetLookInputState();
std::int32_t playerGetRotY();
Vector3 *plymUpdatePlayerSearchLocation(Vector3 *location);
const Vector3 &plymGetPublishedPlayerSearchLocation();
float plymGetSpeedZ();
void plymGetControllerVerticalAxis(float *vertical, float *verticalSecond);
void plymGetControllerGroundBlendedXZ(float *x, float *z);
void plymGetControllerDeltaXZ(Vector3 *first, Vector3 *second);
float plymGetInterpolationValue();
std::uint32_t plymGetAuxiliaryState();
std::uint64_t plymGetAreaExitState();
std::uint16_t plymGetAngY();
std::uint8_t plymGetLookZoomScale();
bool plymIsLookAllowed(std::uint32_t modes);

void camCtrlQueryPitchYaw(Vector3s &angles);
void ctrlSetLookOverlayWeight(float alpha);
float ctrlGetLookOverlayWeight();
std::uint32_t plymConsumeFlags8();
std::int32_t plymGetSpecialFlag();
bool plymIsSearchStartRequested();
bool plymIsSearchable();
bool plymIsSearchBlocked();
bool plymCanEvaluateSearchTargets();
bool IsPlayerPartyGuideBlocked();
bool plymIsInterpolationActive();
bool plymPublishSearchHandTarget(PlayerSearchHand hand, const Vector3 *worldTarget);
void plymClearSearchHandTargets();
bool plymIsSearchHandTargetSettled(PlayerSearchHand hand);
std::uint32_t plymGetSpecialMode();
std::uint32_t plymSetFlags(std::uint32_t flags);
std::uint32_t plymAddFlags(std::uint32_t flags);
std::uint32_t plymRemoveFlags(std::uint32_t flags);
std::int32_t plymExportPersistentState(PlayerPersistentState *output);
std::int32_t plymSetSceneContext(
    std::int16_t holdWindowStartMinute, std::int16_t holdWindowEndMinute, std::int16_t sceneActId, shen_name sceneAreaId, std::int16_t sceneEntryId);
std::uint32_t plymSetSpecialMode(std::int32_t enabled);
std::int32_t plymBeginSpecialMode();
std::int32_t plymEndSpecialMode(std::int32_t mode);
std::int16_t plymGetAuthorPlacementYaw(shen_name name);
Character *authFindChara(shen_name name);
std::int32_t plymRequestSpecialAuthorTarget(shen_name name);
std::int32_t plymSetSpecialModeTargetPosition(const Vector3 *position, std::int32_t mode);
std::int32_t plymBeginSpecialTargetMotionSequence(shen_name name);
std::int32_t plymContinueSpecialTargetMotionSequence();
std::int32_t plymRequestSpecialPoseSequence();
std::int32_t plymGetMovingFrames();
std::int32_t plymGetTurningFrames();
bool plymCheckAuthorTargetRequested();
std::uint64_t shenToggleAreaFlags(std::int32_t mode);
std::uint64_t EV_setHoldStatesFlags(std::int32_t mode);

struct PlayerTargetDate {
    DateTime dateTime;
    std::uint8_t reserved07;
    std::int16_t field08;
};
static_assert(sizeof(PlayerTargetDate) == 0x0a);
std::int32_t plymSetCallback(PlayerFlagCallback callback);
std::int32_t plymScheduleOneShotFlagCallback(std::int16_t minute);
std::int32_t plymScheduleCycleFlagCallback(std::int16_t minute);
void plymAdvanceTime(std::uint8_t hours, std::uint8_t minutes);
std::int32_t BuildTargetDateFromFrameClock(PlayerTargetDate &target);
bool ryoNotBusy();
std::int64_t plymFinishRegionInteraction();
bool shenCheckIsBusy();
bool shenGetIsPlayerInMenu();

std::int64_t ResetPlayerRetainedState() noexcept;

} // namespace shenmue::s2

#include <array>
#include <cstring>

namespace shenmue::s2::player_detail {

inline constexpr std::uint32_t plymPadUseForcedYaw = 0x00000200U;

inline constexpr std::uint32_t plymPadForceMotionState2 = 0x01000000U;

inline constexpr std::uint32_t plymPadUnknown08000000 = 0x08000000U;

inline constexpr std::uint32_t plymControlLockOnMovement = 0x08000000U;

struct PlayerMotionRequest {
    std::uint16_t previousMotion;
    std::uint32_t motion;
    std::uint8_t previousClass;
    float previousFrame;
    float previousFraction;
    float frameCount;
    float sampleFrame{};
    std::uint16_t rootMode{1};
    std::uint16_t blendFrames{5};
    bool mirrored{};
    std::int8_t parentSelector{};
    bool stopTranslation{};
};


inline constexpr std::array<std::uint16_t, 11> plymGaitMotions{
    0xf005U, 0xf03dU, 0xf03cU, 0xf03bU, 0xf03aU, 0xf039U, 0xf038U, 0xf037U, 0xf036U, 0xf035U, 0xf006U};

} // namespace shenmue::s2::player_detail

#include <array>
#include <cstring>

namespace shenmue::s2::player_detail {

// The pull-back threshold and consecutive large-turn counter owned by plymUpdateCameraInput.
inline constexpr std::int8_t rotationOnlyPullThreshold = 120;

inline constexpr std::uint8_t lookAroundInactive = 0;

inline constexpr std::uint8_t lookAroundEntering = 1;

inline constexpr std::uint8_t lookAroundActive = 2;

inline constexpr std::uint8_t lookAroundLeaving = 3;

inline constexpr std::uint8_t lookAroundCommitYawAndLeave = 4;

inline constexpr std::uint8_t lookAroundRootYawRecenter = 5;

inline constexpr std::uint8_t lookInputDigitalDirections = 1;

inline constexpr std::uint8_t lookInputAnalogStick = 2;

inline constexpr std::uint8_t lookInputZoom = 4;

inline constexpr std::uint8_t lookTargetSpecialFast = 5;

inline constexpr std::uint8_t lookTargetForcedYaw = 10;

inline constexpr std::uint8_t lookTargetCancel = 0xff;

inline constexpr std::uint8_t automaticLookAuthorTarget = 1;

inline constexpr std::uint8_t automaticLookRegionTarget = 2;

inline constexpr std::uint8_t automaticLookIdleWander = 4;

inline constexpr std::uint8_t lookTargetAmbientSlow = 1;

} // namespace shenmue::s2::player_detail
