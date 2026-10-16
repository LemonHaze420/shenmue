#pragma once

#include "s1/task/char.hpp"
#include "s1/task/scen.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr std::uint32_t kForkliftComponent = SHEN_NAME(FLMW);
inline constexpr std::uint32_t kVehicleComponent = SHEN_NAME(VEHI);

enum ForkliftGoalArrivalMode : std::uint8_t {
    GOAL_ARRIVAL_KEEP_MOTION = 0x0,
    GOAL_ARRIVAL_STOP = 0x1,
};

enum ForkliftGoalDriveMode : std::uint8_t {
    GOAL_DRIVE_NONE_OR_DIRECT_SPEED = 0x0,
    GOAL_DRIVE_PLAYER_INTERACTION = 0x3,
    GOAL_DRIVE_ACCEL_FROM_STOP = 0xa,
    GOAL_DRIVE_ACCEL_FROM_CURRENT = 0xb,
};

enum ForkliftForkHeightMoveState : std::uint8_t {
    FORK_MOVE_IDLE = 0x0,
    FORK_MOVE_RAISE_TARGET = 0x1,
    FORK_MOVE_LOWER_TARGET = 0x2,
};

enum ForkliftLoadActionBlockState : std::uint8_t {
    LOAD_ACTION_CLEAR = 0x0,
    LOAD_ACTION_SOFT_BLOCK = 0x1,
    LOAD_ACTION_HARD_BLOCK = 0x2,
};

enum ForkliftLoadSequencePhase : std::uint8_t {
    LOAD_SEQ_IDLE = 0x0,
    LOAD_SEQ_RETRY_PICKUP_AT_APPROACH_HEIGHT = 0x1,
    LOAD_SEQ_CARRIED_RAISED_WAIT_DROP = 0x2,
    LOAD_SEQ_DROP_PLACEMENT_CHECK = 0x3,
};

enum ForkliftInteractionPendingState : std::uint8_t {
    INTERACT_PENDING_NONE = 0x0,
    INTERACT_PENDING_CANCEL_RETURN = 0x1,
    INTERACT_PENDING_ACTION_RETURN = 0x2,
};

enum ForkliftDriveAudioBankSide : std::uint8_t {
    DRIVE_AUDIO_FORWARD_BANK = 0x0,
    DRIVE_AUDIO_REVERSE_BANK = 0x1,
};

enum ForkliftInteractionState : std::uint8_t {
    INTERACTION_INACTIVE = 0x0,
    INTERACTION_ACTIVE = 0x1,
};

enum ForkliftDriveUpdateResult : std::int16_t {
    FORK_UPDATE_OK = 0x0,
    FORK_UPDATE_LOADED_ACTION_DENIED0 = 0x1,
    FORK_UPDATE_LOADED_ACTION_DENIED1 = 0x2,
    FORK_UPDATE_LOAD_HARD_BLOCKED = 0x3,
    FORK_UPDATE_FORWARD_IMPACT = 0x4,
};


enum ForkliftUtilTargetMode : std::int32_t {
    FLMW_MODE_RETURN_ARG = -21,
    FLMW_MODE_SET_GLOBAL_FLAG = -15,
    FLMW_MODE_CLEAR_GLOBAL_FLAGS = -12,
    FLMW_MODE_GET_GLOBAL_FLAG = -11,
    FLMW_MODE_CLEAR_FOLLOWER_SNAPSHOT = -8,
};

enum ForkliftUtilCommand : std::int32_t {
    FORK_CMD_SOUND_CONTROL = -109,
    FORK_CMD_CALC_START_LINE_CROSSING_TIME = -108,
    FORK_CMD_UPDATE_PROXIMITY_STATE = -107,
    FORK_CMD_DISTANCE_TO_POINT_LIST_SQ = -106,
    FORK_CMD_FIND_NEAREST_POINT_INDEX = -104,
    FORK_CMD_GET_REGION_INDEX = -103,
    FORK_CMD_SET_GOAL_SPEED_STEP = -102,
    FORK_CMD_SET_INTERACTION_PROFILE = -101,
    FORK_CMD_CALC_ANGLE_BETWEEN_POINTS = -100,
    FORK_CMD_HIDE_HELPER_ACTORS = -22,
    FORK_CMD_CLEAR_SAVED_LOAD_STATE = -20,
    FORK_CMD_RESTORE_LOAD_STATE = -19,
    FORK_CMD_SAVE_LOAD_STATE = -18,
    FORK_CMD_GET_ATTACHED_LOAD_ID = -17,
    FORK_CMD_ENGINE_BANK_ENABLE = -16,
    FORK_CMD_SET_ENGINE_AUDIO_VOLUME = -14,
    FORK_CMD_GET_THROTTLE_DELTA = -13,
    FORK_CMD_GET_SET_STEER_OFFSET = -10,
    FORK_CMD_SET_INTERACTION_STATE = -9,
    FORK_CMD_RESTORE_FOLLOWER_LOADS = -7,
    FORK_CMD_SNAPSHOT_FOLLOWER_LOADS = -6,
    FORK_CMD_SUPPRESS_ACTION_HOTKEYS = -5,
    FORK_CMD_GET_SET_FORK_HEIGHT = -4,
    FORK_CMD_RESET_DRIVE_INPUTS = -2,
    FORK_CMD_GET_FORK_HEIGHT_INTERP = -1,
    FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE = 3,
};

struct ForkliftDriveProfile {
    std::int32_t maxSteerFixed16;
    std::int32_t steerStepFixed16;
    std::int32_t steerCenterStep;
    float throttleAccelStep;
    float maxForwardSpeed;
    float maxReverseSpeed;
    float brakeStepTightClearance;
    float coastDecayStep;
    float wheelDiameterA;
    float wheelDiameterB;
    float effectiveWheelbase;
};



struct ForkliftWork {
    const ForkliftDriveProfile *tuning;
    void (*shadowCallback)(Character *);
    float forkHeight;
    std::int32_t steerOffsetAngle;
    float driveSpeed;
    float previousForkHeight;
    float targetForkHeight;
    Vector3 goalPosition;
    Character *carriedLoad;
    Character *visualOwner;
    shen_name ownerCharacter;
    std::uint8_t hasAttachedLoad;
    std::int8_t forkHeightLerpDone;
    ForkliftGoalArrivalMode stopOnGoalArrival;
    ForkliftGoalDriveMode goalDriveMode;
    std::uint8_t interactionProfile;
    std::uint8_t sideMode;
    std::byte reserved04a[2];
    float throttleDelta;
    float moveDeltaX;
    float moveDeltaZ;
    float forkHeightStepRate;
    std::int32_t moveYawFixed16;
    float goalDriveSpeedStep;
    float sourcePositionX;
    float sourcePositionZ;
    std::int32_t sourceYawFixed16;
    shen_name *followerLoadIds;
    shen_name pendingLoadCandidateId;
    shen_name attachedLoadId;
    std::uint32_t inputMask;
    shen_name helperActorId0;
    shen_name helperActorId1;
    std::int16_t cameraBlendStep;
    std::uint8_t followerLoadNearCab;
    std::uint8_t manualCameraProbe;
    ForkliftForkHeightMoveState forkHeightMoveState;
    ForkliftLoadActionBlockState loadActionBlockState;
    ForkliftLoadSequencePhase loadSequencePhase;
    ForkliftInteractionPendingState interactionPendingState;
    std::int16_t forwardSoundBankId;
    std::int16_t reverseSoundBankId;
    std::int32_t steerLimit;
    std::int32_t steerStep;
    std::uint8_t reserved0a0;
    std::uint8_t suppressActionHotkeys;
    ForkliftDriveAudioBankSide activeDriveAudioBank;
    ForkliftInteractionState interactionState;
    std::uint8_t steerRecenterMode;
    std::uint8_t forwardBankVolumeCache;
    std::uint8_t reverseBankVolumeCache;
    std::uint8_t engineEnabledFrames;
};

struct VehicleWork {
    std::int32_t state;
    std::int32_t reserved004;
    Character *attachedCharacter;
    Vector2 *pathPoints;
    float maxSpeed;
    float speedScale;
    float currentSpeed;
    std::int32_t controlProfile;
    std::int32_t reserved028;
    std::int32_t reserved02c;
    std::int32_t reserved030;
    std::int32_t reserved034;
    std::int32_t rollPhaseFixed16;
    std::int32_t smoothedYawDelta;
    std::int32_t targetYawDelta;
    std::int32_t yawFixed16;
    std::int32_t stopAtPathEnd;
    std::int32_t taskState;
    std::int32_t riderPoseMode;
    std::int32_t pathCount;
    std::int32_t pathIndex;
    std::uint32_t rideMotionIds[4];
    std::int32_t centerConstraintEnabled;
};

struct ForkliftUtilsParameters {
    std::int32_t mode;
    std::int32_t command;
    std::int32_t argument0;
    std::int32_t argument1;
    std::int32_t argument2;
};

struct VehicleCommandParameters {
    std::int32_t mode;
    std::uint32_t targetId;
    std::uint32_t argument2;
    std::uint32_t argument3;
    std::uint32_t argument4;
};


struct FollowerLoadSnapshot {
    Vector3 position;
    std::int32_t yaw;
};



struct FollowerLoadFogBlock {
    FollowerLoadSnapshot followerLoadSnapshots[20];
    alignas(16) float fogIntensityTable[128];
};
static_assert(offsetof(FollowerLoadFogBlock, fogIntensityTable) == 0x140);
static_assert(sizeof(FollowerLoadFogBlock) == 0x340);

extern FollowerLoadFogBlock g_followerLoadFogBlock;


struct ForkliftRegion {
    float minX;
    float minZ;
    float maxX;
    float maxZ;
};

static_assert(sizeof(ForkliftDriveProfile) == 0x2c);
static_assert(sizeof(ForkliftWork) == 0xa8);
static_assert(offsetof(ForkliftWork, forkHeight) == 0x10);
static_assert(offsetof(ForkliftWork, carriedLoad) == 0x30);
static_assert(offsetof(ForkliftWork, hasAttachedLoad) == 0x44);
static_assert(offsetof(ForkliftWork, throttleDelta) == 0x4c);
static_assert(offsetof(ForkliftWork, followerLoadIds) == 0x70);
static_assert(offsetof(ForkliftWork, inputMask) == 0x80);
static_assert(offsetof(ForkliftWork, cameraBlendStep) == 0x8c);
static_assert(offsetof(ForkliftWork, followerLoadNearCab) == 0x8e);
static_assert(offsetof(ForkliftWork, forkHeightMoveState) == 0x90);
static_assert(offsetof(ForkliftWork, forwardSoundBankId) == 0x94);
static_assert(offsetof(ForkliftWork, steerLimit) == 0x98);
static_assert(offsetof(ForkliftWork, engineEnabledFrames) == 0xa7);
static_assert(sizeof(VehicleWork) == 0x70);
static_assert(sizeof(ForkliftUtilsParameters) == 0x14);
static_assert(sizeof(VehicleCommandParameters) == 0x14);
static_assert(sizeof(FollowerLoadSnapshot) == 0x10);
static_assert(sizeof(ForkliftRegion) == 0x10);



extern std::uint8_t g_flmwPlayerDriveActive;
extern std::uint8_t g_flmwForwardInputPressed;
extern std::int16_t g_ryoVehicleAlphaSteps;

void AllocFLMW(Character *character);
ForkliftWork *FLMW_GetWork(Character *character);
VehicleWork *VEHI_Allocate(Character *character);
bool FLMW_IsWatchInteractionActive();

void FLMW_Callback(Character *character, void *work);
void Script_flmwAttachLoadActor(ScenarioContext *context, void *params);
void Script_flmwFinalizeLoadDetach(ScenarioContext *context, void *params);
void Script_forkSetForkHeightTarget(ScenarioContext *context, void *params);
void Script_forkUpdateForkHeightLerp(ScenarioContext *context, void *params);
void Script_forkAttachLoadActor(ScenarioContext *context, void *params);
void Script_forkAdjustCharacterMovementBasedOnDistance(ScenarioContext *context, void *params);
void Script_forkSetForkHeightStepRate(ScenarioContext *context, void *params);
void Script_forkSetSoundBankDetails(ScenarioContext *context, void *params);
void Script_vehiUtils(ScenarioContext *context, void *params);

extern std::int32_t g_bCameraVehicleFlag;
extern std::int32_t g_vehicleGlobalFlag;
extern std::int32_t g_vehicleGlobalFlagGate;
void Script_flmwTriggerInteraction(ScenarioContext *context, void *params);
void Script_flmwSetInputMask(ScenarioContext *context, void *params);
void Script_flmwHandleInteractionState(ScenarioContext *context, void *params);
void Script_flmwSetFollowerLoadIdList(ScenarioContext *context, void *params);
void Script_flmwFreeFollowerLoadIds(ScenarioContext *context, void *params);
void Script_ForkliftUtils(ScenarioContext *context, void *params);

} // namespace shenmue::s1
