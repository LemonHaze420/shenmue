#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct MapControlData;
struct MotionInformationWork;
struct InputSlot;

struct PlayerMotionMetadata {
    std::uint32_t animationId;
    std::uint32_t flags;
    float playbackRate;
};
static_assert(sizeof(PlayerMotionMetadata) == 0x0c);

inline constexpr Vector3i kContextBagRotation{0x28e3, 0x38e3, 0};
inline constexpr Vector3i kContextLanternRotation{0x28e3, 0x38e3, 0};
inline constexpr Vector3i kContextRaincoatRotation{0x360b, 0xb999, 0x9c71};
inline constexpr Vector3i kContextRaincoatFoldedRotation{0x18e3, 0xc000, 0xa38e};
enum class PlayerMovementState : std::int32_t {
    Idle = 0x00,
    TurnExitToIdle = 0x01,
    TurnExitFromForward = 0x02,
    WalkForward = 0x03,
    SnapFailFallback = 0x04,
    TurnAccumRight = 0x05,
    TurnAccumLeft = 0x06,
    TurnCommitFamily7 = 0x07,
    TurnCommitFamily8 = 0x08,
    TurnCommitFamily9 = 0x09,
    TurnCommitFamily10 = 0x0a,
    TurnCommitRight = 0x0b,
    TurnCommitLeft = 0x0c,
    Turn180InPlace = 0x0d,
    TurnHoldPreCommit = 0x0e,
    ResolveBlockedA = 0x0f,
    AutoRotateForcedNegative = 0x10,
    AutoRotateForcedPositive = 0x11,
    TransitionToWalkForward = 0x12,
    RunForward = 0x13,
    ResolveBlockedB = 0x14,
    TurnHoldAlt = 0x15,
};
static_assert(sizeof(PlayerMovementState) == sizeof(std::int32_t));

#pragma pack(push, 1)
struct PlayerControlWork {
    std::uint8_t controlMode;
    std::int8_t previousAutoRotateYawDelta;
    std::int8_t inputYawDelta8;
    std::int16_t previousInputYawPair;
    std::uint8_t numActiveFrames;
    std::uint16_t pressedPriority;
    std::uint32_t heldInput;
    std::uint32_t desiredFacingYaw;
    std::uint32_t cameraAccumulatedYaw;
    std::uint16_t flags;
    std::byte reserved016[2];
    std::byte reserved018[4];
    std::int32_t controlGate;
    std::uint32_t state;
    PlayerMotionMetadata motionMetadata[33];
    float turnIntensitySmoothed;
    float turnBurstAccum;
    std::uint16_t idleLoopFrames;
    std::uint16_t idleLoopTarget;
    std::uint8_t savedBlendValid;
    std::uint8_t savedBlendMode;
    std::uint8_t savedBlendFrameCount;
    std::int8_t rightAngleX;
    std::int8_t rightAngleY;
    std::int8_t stateFlags;
    std::int8_t scriptState;
    std::byte reserved1c3;
    std::int32_t forcedYaw;
    std::int32_t navProbeYawLastStable;
    std::int32_t autoRotateYawDelta;
    std::int32_t inputYawDelta;
    std::uint16_t action;
    std::byte reserved1d6;
    std::uint8_t forcedYawSnap;
    std::byte reserved1d8;
    std::uint8_t modeState;
    std::byte reserved1da[2];
    std::int32_t yawOffset;
    std::int32_t committedTurnAccumYaw;
    float startAnimationSpeed;
    float speedAdjustWindow;
    float speedTime;
    std::uint8_t resetSpeedBlend;
    std::uint8_t applyDeferredTranslation;
    std::byte reserved1f2[2];
    std::uint32_t currentMode;
    std::uint32_t blockedCanControl;
    std::uint8_t isRunning;
    std::int8_t wasRunning;
    std::uint8_t turnCommitCanMove;
    std::byte reserved1ff;
    std::int32_t turnAngleAdjustment;
    std::int32_t turnAdjustSmoothed;
    std::int32_t turnVelocity;
    std::uint8_t hasNavProbeYawTarget;
    std::byte reserved20d;
    std::int16_t tatehizaState;
    std::int32_t idleTurnActive;
    std::int32_t secondaryYawOffset;
    InputSlot *inputSource;
    std::uint8_t allowTurnInPlace;
    std::int8_t currentFrameCount;
    std::byte reserved222[2];
    std::int32_t idleTurnCounter;
    std::uint8_t enableNavYawRecompute;
    std::uint8_t facingProbeClear;
    std::byte reserved22a[2];
    std::int32_t newYaw;
    std::int32_t facingYawDelta;
    std::int32_t lateralSnapYaw;
    std::uint8_t idleTurnMode;
    std::int8_t idleTurnIntensity;
    std::uint8_t idleTurnTimer;
    std::byte reserved23b;
    std::uint32_t ticks;
    std::int32_t navProbeYaw;
    std::int16_t yawBlendBackTimer;
    std::int16_t yawDriftTimer;
    std::uint8_t footPlantingEnabled;
    std::uint8_t movementInputActive;
    std::uint8_t fightOnly;
    std::uint8_t lookRunMode;
    std::int32_t blockedResolveTurnAccumYaw;
    float resolvedMovementSpeed;
    std::uint8_t triggerHigh;
    std::uint8_t lookYawUsesMovementInput;
    std::uint8_t lookPitchUsesMovementInput;
    std::uint8_t pressedPriorityDown;
    std::uint8_t runOverride;
    std::byte reserved259[3];
    std::int8_t ticksHeld;
    std::int8_t busy;
    std::uint8_t loadHoldActive;
    std::byte reserved25f;
    std::int16_t walkVolumeGraceFrames;
    std::byte reserved262[2];
    std::uint32_t inputMode;
    float speedScale;
    std::uint32_t areaId;
};
#pragma pack(pop)

static_assert(sizeof(PlayerControlWork) == 0x270);
static_assert(offsetof(PlayerControlWork, flags) == 0x14);
static_assert(offsetof(PlayerControlWork, controlGate) == 0x1c);
static_assert(offsetof(PlayerControlWork, state) == 0x20);
static_assert(offsetof(PlayerControlWork, motionMetadata) == 0x24);
static_assert(offsetof(PlayerControlWork, stateFlags) == 0x1c1);
static_assert(offsetof(PlayerControlWork, scriptState) == 0x1c2);
static_assert(offsetof(PlayerControlWork, motionMetadata) + 8 * sizeof(PlayerMotionMetadata) == 0x84);
static_assert(offsetof(PlayerControlWork, motionMetadata) + 19 * sizeof(PlayerMotionMetadata) == 0x108);
static_assert(offsetof(PlayerControlWork, turnIntensitySmoothed) == 0x1b0);
static_assert(offsetof(PlayerControlWork, idleLoopFrames) == 0x1b8);
static_assert(offsetof(PlayerControlWork, savedBlendValid) == 0x1bc);
static_assert(offsetof(PlayerControlWork, modeState) == 0x1d9);
static_assert(offsetof(PlayerControlWork, currentMode) == 0x1f4);
static_assert(offsetof(PlayerControlWork, blockedCanControl) == 0x1f8);
static_assert(offsetof(PlayerControlWork, turnAdjustSmoothed) == 0x204);
static_assert(offsetof(PlayerControlWork, secondaryYawOffset) == 0x214);
static_assert(offsetof(PlayerControlWork, inputSource) == 0x218);
static_assert(offsetof(PlayerControlWork, facingProbeClear) == 0x229);
static_assert(offsetof(PlayerControlWork, lateralSnapYaw) == 0x234);
static_assert(offsetof(PlayerControlWork, ticks) == 0x23c);
static_assert(offsetof(PlayerControlWork, footPlantingEnabled) == 0x248);
static_assert(offsetof(PlayerControlWork, movementInputActive) == 0x249);
static_assert(offsetof(PlayerControlWork, resolvedMovementSpeed) == 0x250);
static_assert(offsetof(PlayerControlWork, runOverride) == 0x258);
static_assert(offsetof(PlayerControlWork, inputMode) == 0x264);
extern PlayerControlWork *g_pControlPadController;
extern Character *g_playerCharacter;
extern MotionInformationWork *g_playerMotion;
extern std::int32_t g_smoothedAnimationYaw;
extern std::int32_t g_scenarioControllerConnected;
extern std::int32_t g_suppressDefaultHudPromptIcons;
extern std::uint8_t g_tmexValHigh;
extern bool g_needsDestroyForSleepPrompt;
extern std::int32_t g_skipNavRegionFiltering;
extern std::int32_t g_ryoCurrentAnimationState;
extern std::int32_t g_ryoIdleTurnInPlaceCounter;
extern std::int32_t g_areaIdleFrameCounterIndex;
extern PlayerMovementState g_playerMovementState;
extern float g_ryoRawMovementInputSpeed;
extern Vector3 g_ryoFootPositions;
extern std::int32_t g_ryoFootPlantingApplied;
extern float g_playerNavigationClearanceRadius;
extern std::uint8_t g_playerLookRunSwapMode;
extern std::uint8_t g_cpctOperandEnabled;
extern std::uint32_t g_cpctLastOperand;
extern std::uint32_t g_cpctPreviousOperand;
extern bool g_cpctSleepPromptReady;
extern bool g_cpctInputUpdatePending;
extern std::int32_t g_turnYawOffsetFromAccum;
extern std::int32_t g_turnYawOffsetActive;
extern std::int32_t g_previousPlayerNodeYaw;
extern std::int32_t g_playerNodeYaw;

enum class LocomotionInit : std::int32_t { Disabled, Pending, Active };
extern LocomotionInit g_locomotionInitState;

bool ShouldRun();
bool PlayerEffectLightUsesAccumulatedYaw();
bool cpctIsTatehizaStateActive();
bool filmGetTaskFlag();
std::int32_t ryoUpdateTurnMode(Character *ryo);
void navResetForEventCamera(Character *character);
void navDisablePlayerControl();
void navEnablePlayerControl();
void navResetAuthAndInputStates();
void navRequestNewMode(std::int32_t mode);
void navSetSkipNavRegionFiltering(bool skip);
void navUpdateRyoNavYaw(Character *character);
void navMoveToHome();
void InitFlags();
std::uint8_t shenForceCancel(PlayerControlWork *control);
void CPCT_Toggle(Character *character);
PlayerControlWork *AllocateCPCT(Character *character);
void CPCT_RebindMotionWithNavParams(Character *character);
void navBeginLoadHold();
std::int32_t navGetForcedControlMode();
std::uint32_t navGetCurrentControlPoint();
Vector3 navGetCurrentControlPointPosition();
void navSetScriptWalkMode(std::uint32_t mode);

inline constexpr std::array<PlayerMotionMetadata, 33> kDefaultPlayerMotionMetadata{{
    {903, 0, 1.0f},
    {1068, 0, 1.0f},
    {1043, 0, 1.0f},
    {1069, 0, 1.0f},
    {1071, 0, 1.0f},
    {1074, 0, 1.0f},
    {1072, 0, 1.0f},
    {1073, 0, 1.0f},
    {1374, 0, 1.28571427f},
    {593, 0, 1.0f},
    {603, 0, 1.0f},
    {604, 0, 1.0f},
    {605, 0, 1.0f},
    {606, 0, 1.0f},
    {609, 0, 1.0f},
    {608, 0, 1.0f},
    {607, 0, 1.0f},
    {623, 0, 1.0f},
    {623, 0x80, 1.0f},
    {724, 0, 1.20000005f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0, 1.0f},
    {870, 0x80, 1.0f},
    {870, 0x80, 1.0f},
    {870, 0, 1.0f},
}};

} // namespace shenmue::s1
