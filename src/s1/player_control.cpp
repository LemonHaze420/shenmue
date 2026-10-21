#include "s1/player_control.hpp"

#include "s1/date_time.hpp"
#include "s1/boot.hpp"
#include "s1/collision.hpp"
#include "s1/input.hpp"
#include "s1/player_stats.hpp"
#include "s1/motion.hpp"
#include "s1/task/save.hpp"
#include "s1/task/came.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/task/film.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/modu.hpp"
#include "s1/task/ppad.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/tmex.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/wzog.hpp"

#include <array>
#include <bit>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace shenmue::s1 {

PlayerControlWork *g_pControlPadController = nullptr;
std::int32_t g_suppressDefaultHudPromptIcons = 0;
std::uint8_t g_tmexValHigh = 0;
bool g_needsDestroyForSleepPrompt = true;
std::int32_t g_skipNavRegionFiltering = 0;
std::int32_t g_ryoCurrentAnimationState = 0;
std::int32_t g_ryoIdleTurnInPlaceCounter = 0;
std::int32_t g_areaIdleFrameCounterIndex = -1;
PlayerMovementState g_playerMovementState = PlayerMovementState::Idle;
float g_ryoRawMovementInputSpeed = 0.0F;
Vector3 g_ryoFootPositions{};
std::int32_t g_ryoFootPlantingApplied = 0;
float g_playerNavigationClearanceRadius = 0.40000001F;
std::uint8_t g_playerLookRunSwapMode = 0;
std::uint8_t g_cpctOperandEnabled = 1;
std::uint32_t g_cpctLastOperand = 0;
std::uint32_t g_cpctPreviousOperand = 0;
bool g_cpctSleepPromptReady = true;
bool g_cpctInputUpdatePending = false;
LocomotionInit g_locomotionInitState = LocomotionInit::Disabled;
std::int32_t g_turnYawOffsetFromAccum = 0;
std::int32_t g_turnYawOffsetActive = 0;
std::int32_t g_previousPlayerNodeYaw = -1056;
std::int32_t g_playerNodeYaw = -1056;
extern std::uint32_t g_scenarioRuntimeFlags;
extern std::int32_t g_actNumber;
extern shen_name g_homeAreaId;
extern std::uint32_t g_requestedEntryPoint;
extern std::int32_t g_currentSearchState;
extern std::int32_t g_searchCandidateId;
extern std::uint32_t g_searchTemporaryFocusedCharacter;
extern std::uint8_t g_shouldSleep;
extern std::int32_t g_sleepPromptWindowStartMinutes;
extern std::int32_t g_sleepPromptWindowEndMinutes;
extern std::int32_t g_forcedSleepWindowStartMinutes;
extern std::int32_t g_forcedSleepWindowEndMinutes;
extern std::int32_t g_homeSceneId;
std::int32_t g_scenarioControllerConnected = 0;
Character *g_playerCharacter = nullptr;
MotionInformationWork *g_playerMotion = nullptr;
std::int32_t g_smoothedAnimationYaw = 0;



void InitFlags() {
    g_homeSceneId = g_sceneId;
    g_shouldSleep = 0;
    g_sleepPromptWindowStartMinutes = 1350;
    g_sleepPromptWindowEndMinutes = 1360;
    g_forcedSleepWindowStartMinutes = 1410;
    g_forcedSleepWindowEndMinutes = 1919;
    g_homeAreaId = SHEN_NAME(JOMO);
    g_requestedEntryPoint = 3;
    g_needsDestroyForSleepPrompt = true;
    SetShouldSleepFlag(2);
    SCEN_ResetTimedTransitions();
}


bool ShouldRun() {
    const MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (g_NumFramesProcessed || !control)
        return false;
    if (!control->ryo || !g_pControlPadController || g_tmex_val > 1U)
        return false;
    return g_pControlPadController->blockedCanControl == 1;
}

namespace {

constexpr std::uint16_t kInputUp = 0x1000U;
constexpr std::uint16_t kInputDown = 0x2000U;
constexpr std::uint16_t kInputLeft = 0x4000U;
constexpr std::uint16_t kInputRight = 0x8000U;
constexpr std::uint16_t kMovementInputMask = kInputUp | kInputDown | kInputLeft | kInputRight;
constexpr std::uint16_t kForceWalk = 0x0002U;
constexpr std::uint16_t kScriptAlternateMode = 0x0004U;
constexpr std::uint8_t kRunGateForceRun = 4;

constexpr std::int32_t kAnimOjigi = 44;
constexpr std::int32_t kAnimRunLoop = 65;
constexpr std::int32_t kAnimTatehizaExit = 79;
constexpr std::int32_t kAnimTatehizaLoop = 80;
constexpr std::int32_t kAnimStandLoop = 89;
constexpr std::int32_t kAnimWalkLoop = 102;
constexpr std::int32_t kAnimRunTurn180L = 591;
constexpr std::int32_t kAnimWalkStart = 593;
constexpr std::int32_t kAnimWalk045L = 603;
constexpr std::int32_t kAnimWalk180L = 606;
constexpr std::int32_t kAnimWalk180R = 610;
constexpr std::int32_t kAnimWalkTurn180L = 623;
constexpr std::int32_t kAnimWalkHighLoop = 653;
constexpr std::int32_t kAnimTurn180RSp3 = 709;
constexpr std::int32_t kAnimStairsUpLoop = 710;
constexpr std::int32_t kAnimStairsDownLoop = 711;
constexpr std::int32_t kAnimStairsUp2StepLoop = 715;
constexpr std::int32_t kAnimRunMidLoop = 724;
constexpr std::int32_t kAnimWalkSp1Loop = 735;
constexpr std::int32_t kAnimStairsDown2StepLoop = 761;
constexpr std::int32_t kAnimSekkinKamaeLoop = 850;
constexpr std::int32_t kAnimRunKick = 902;
constexpr std::int32_t kAnimIdleRoot = 903;
constexpr std::int32_t kAnimIdleRootReturn = 904;
constexpr std::int32_t kAnimSlowRunLoop = 1042;
constexpr std::int32_t kAnimTurn090WadL = 1043;
constexpr std::int32_t kAnimTurn045WadL = 1068;
constexpr std::int32_t kAnimTurn022WadL = 1070;
constexpr std::int32_t kAnimTurn180WadL = 1071;
constexpr std::int32_t kAnimTurn022WadR = 1075;
constexpr std::int32_t kAnimTurn180WadR = 1076;
constexpr std::int32_t kAnimMimawasu = 1192;
constexpr std::int32_t kAnimStandFightLoop = 1193;
constexpr std::int32_t kAnimWalkFastKLoop = 1194;
constexpr std::int32_t kAnimWalkSlowKLoop = 1195;
constexpr std::int32_t kAnimWalkSp2Loop = 1374;
constexpr std::int32_t kAnimRunMidTurn180L = 1407;
constexpr std::int32_t kAnimRunSlowKLoop = 1418;
constexpr std::int32_t kAnimRequestStand = 0xfffe;
constexpr std::int32_t kAnimRequestLocomotion = 0xffff;

constexpr std::int32_t kStateStand = 0;
constexpr std::int32_t kStateIdle = 1;
constexpr std::int32_t kStateWalk = 2;
constexpr std::int32_t kStateWalkBlend = 3;
constexpr std::int32_t kStateWalkTurn180 = 4;
constexpr std::int32_t kStateIdleAlt = 5;
constexpr std::int32_t kStateRun = 6;
constexpr std::int32_t kStateRunBlend = 7;
constexpr std::int32_t kStatePlayRateDirect = 8;
constexpr std::int32_t kStateFight = 10;
constexpr std::int32_t kStateFightFallback = 11;
constexpr std::int32_t kStateIdleRootVariant = 12;

constexpr std::uint32_t kMotionStuck = 0x2U;
constexpr std::uint32_t kMotionUnknown8 = 0x8U;
constexpr std::uint32_t kMotionUnknown20 = 0x20U;
constexpr std::uint32_t kMotionFrameBlendReady = 0x40U;
constexpr std::uint32_t kMotionDirtyAnimState = 0x80U;
constexpr std::uint32_t kMotionForceBlendOut = 0x20000U;

enum class TurnPhase : std::int32_t { None, TargetYawSelected, TurnAnimActive, TurnFinishing };

enum class TurnMode : std::int32_t {
    Mode0,
    Mode1,
    Mode2,
    Mode3,
    Mode4,
    Mode5,
    Mode6,
    Mode7,
    Mode8,
    Mode9,
    YawStepPositive,
    YawStepNegative,
    Mode12,
    Mode13,
    Mode14,
    Mode15,
    Mode16,
};

enum class YawResolveMode : std::int32_t {
    OctantSnapWalk,
    TurnSectorDelta,
    Skeleton,
    CharacterWorldOrBlocked,
    SkeletonPlusOffset,
    KeepSmoothed,
    Forced,
    StrafeStepSector,
};

enum class NavProbeSector : std::int32_t {
    Positive120Fan,
    Positive120FanAlt,
    FullSweep,
    Negative120Fan,
    Negative120FanAlt,
    NarrowCandidateBias = 6,
};

// blend_t: one looped locomotion clip and its frame windows.
struct AnimBlendEntry {
    std::int32_t animationId;
    float frames;
    float blendRate;
    float rootMotionZ;
    float frameOffset;
    float startFrame;
    float blendStartLeft;
    float blendStartRight;
    float blendEndThreshold;
    float end2;
    float end3;
    float endBlendCutoff;
    float endFrame;
};
static_assert(sizeof(AnimBlendEntry) == 0x34);

// Request context shared by ryoUpdateAnimById and its selection/blend helpers.
struct LocomotionAnimRequest {
    std::int32_t animationId;
    std::int32_t animationState;
    std::int32_t frameCount;
    float frame;
    std::int32_t blendSlot;
    std::int32_t blendOutLatched;
    std::int32_t requestAnimationId;
    std::int32_t resolvedAnimationId;
    float blendFrame;
    std::int32_t forceBlendOut;
    float playRateMultiplier;
    std::int32_t shouldTransitionToIdle;
    std::int32_t metadataAnimationId;
    std::int32_t metadataFlags;
    float metadataPlaybackRate;
    std::int32_t turnBlendParam;
};
static_assert(sizeof(LocomotionAnimRequest) == 0x40);

struct ResolvedLocomotion {
    std::int32_t animationId;
    std::int32_t chosenYawOffset;
    std::int32_t deltaYaw;
    std::int32_t residualYaw;
};

struct IdleAnimationRule {
    std::uint16_t rateMode;
    std::uint16_t requestAnimationId;
    std::uint16_t animationId;
    float blendFrame;
};

constexpr std::array<IdleAnimationRule, 1> kIdleAnimationRules{{{10, 0xffff, kAnimStandLoop, 0.0F}}};
constexpr std::array<std::int32_t, 11> kNavSteeringClampAngles{5460, 5460, 4914, 4368, 3822, 3276, 2730, 2184, 1638, 1092, 546};
constexpr std::array<std::int16_t, 8> kOctantCardinalSnapOffset{0, 0, -8192, -8192, 0, 8192, 8192, 0};
constexpr std::array<std::int32_t, 8> kOctantTurnBucketWalk{9, 9, 10, 11, 13, 15, 16, 9};
constexpr std::array<std::int32_t, 8> kOctantSectorIndexDefault{9, 1, 2, 3, 4, 5, 6, 7};
constexpr std::array<std::int32_t, 16> kYawSliceTurnBucket{-1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, -2};
constexpr std::array<std::int32_t, 8> kOctantTurnBucketRun{9, 9, 10, 11, 4, 15, 16, 9};
constexpr std::array<std::int32_t, 33> kSectorYawOffsets{0, 8192, 16384, 24576, 32768, -24576, -16384, -8192, 0, 0, 8192, 16384, 24576, 32768, -24576, -16384,
    -8192, 32768, 32768, 0, 0, 8192, 16384, 24576, 32768, -24576, -16384, -8192, 32768, 0, 0, 0, 0};
constexpr std::array<std::int32_t, 13> kNavProbeYawOffsetsPositive{0, 1820, 3640, 5461, 7281, 9102, 10922, 12743, 14563, 16384, 18204, 20024, 21845};
constexpr std::array<std::int32_t, 36> kNavProbeYawOffsetsWide{0, 1820, -1820, 3640, -3640, 5461, -5461, 7281, -7281, 9102, -9102, 10922, -10922, 12743, -12743,
    14563, -14563, 16384, -16384, 18204, -18204, 20024, -20024, 21845, -21845, 23665, -23665, 25486, -25486, 27306, -27306, 29127, -29127, 30947, -30947,
    -32768};
constexpr std::array<std::int32_t, 5> kNavProbeYawOffsetsNarrow{0, 3640, -3640, 7281, -7281};
constexpr std::array<std::int32_t, 13> kNavProbeYawOffsetsNegative{
    0, -1820, -3640, -5461, -7281, -9102, -10922, -12743, -14563, -16384, -18204, -20024, -21845};
constexpr std::array<std::int32_t, 14> kNavResolveProbeYawOffsets{182, -182, 728, -728, 1456, -1456, 2184, -2184, 2912, -2912, 3640, -3640, 4369, -4369};

CharacterNode *g_playerRootNode = nullptr;
std::array<AnimBlendEntry, 7> g_animBlendTable{};
std::int32_t g_currentAnimBlendIndex = 0;
ResolvedLocomotion g_resolvedLocomotion{};
float g_animPlayRate = 0.0F;
std::int32_t g_locomotionTrackedNode = 10;
std::int32_t g_blendFrameIterations = 100;
std::int32_t g_ryoIdleAnimationIterations = 100;
std::int32_t g_currentAnimationId = 0;
std::int32_t g_processAnimationState = -1;
std::int32_t g_previousPlayerAnimationState = 0;
float g_playerAnimationRateScale = 0.0F;
std::int16_t g_ryoIdleResetCounter = 0;
std::int16_t g_playerAnimationActiveFrames = 0;
std::uint8_t g_areaIdleCountBlocked = 0;

std::int32_t g_navigationAnimationState = 0;
TurnPhase g_turningPhase = TurnPhase::None;
TurnMode g_currentTurnMode = TurnMode::Mode0;
YawResolveMode g_turnYawResolveMode = YawResolveMode::OctantSnapWalk;
std::uint8_t g_navStrafeBlocked = 0;
std::int8_t g_angleWrapPolicy = 0;
std::int16_t g_turnFrameCount = 0;
std::int16_t g_turnHoldFrameCount = 0;
std::int16_t g_turnModeYawFraction = 0;
std::int32_t g_turnModeYaw = 0;
std::int32_t g_turnPhaseTracking = 0;
std::uint16_t g_turnDirectionMask = 0;
Vector3 g_previousTurnNavigationPosition{};
std::int32_t g_targetPlayerYaw = 0;
std::int32_t g_targetPlayerYawSnap = 0;
std::int32_t g_currentPlayerYaw = 0;
std::int32_t g_previousPlayerYaw = 0;
std::int32_t g_playerYawVelocity = 0;
std::int32_t g_playerYawAngularTarget = 0;
std::int32_t g_playerYawAngularSmoothed = 0;
std::int32_t g_playerTurnStep = 0;
std::int32_t g_navigationTurnEnabled = 0;
std::uint16_t g_blockedDirectionMask = 0;
std::int16_t g_blockedDirectionFrames = 0;
std::int32_t g_movementBlockedFrames = 0;
std::int32_t g_navHasNoLegalFacing = 0;
std::int32_t g_navSteeringClampIndex = 0;
std::int32_t g_navRegionFilterDisabled = 0;
std::int32_t g_navTemporaryYaw = 0;
float g_locomotionSpeed = 0.0F;
Vector3 g_playerNavigationPosition{};
Vector3 g_playerEntityPosition{};
std::int32_t g_navFilterDisabled = 0;
std::int32_t g_navFilterProjectPositionOnce = 0;
std::int32_t g_forceYawRecalculation = 0;
std::int32_t g_underMinimumSnapSpeed = 0;
std::int32_t g_hasNavSnapBudget = 0;
std::int32_t g_speedWasLimited = 0;
std::int32_t g_deltaYaw = 0;
std::int32_t g_turnBiasProbeYaw = 0;
std::int32_t g_navProbeYawDelta = 0;
std::int32_t g_navProbeBaseYaw = 0;
std::int32_t g_navStateCurrent = 0;
std::int32_t g_navMetricCurrent = 0;
Vector3 g_lastProbePosition{};
Vector3 g_forwardProbePosition{};
std::int32_t g_navProbeYawOffset = 0;
std::int32_t g_navDirectionFlags = 0;
float g_navLateralDistance = 0.0F;
float g_navProbeSpeed = 0.0F;
float g_navRecomputedSpeed = 0.0F;
float g_navSpeedClampMaximum = 0.0F;
std::int32_t g_navUseCachedYaw = 0;
std::int32_t g_navSnapYawAngle = 0;
Vector3 g_navPreviousDeltaPosition{};
Vector3 g_navDeltaPosition{};
Vector3 g_navForwardDelta{};
Vector2 g_navLastEntityPosition{};
float g_navRadiusAdd = 0.0F;
float g_navLastZ = 0.0F;
std::int32_t g_navTemporaryProbeYaw = 0;
std::int32_t g_navSuggestedYaw = 0;
std::int32_t g_minimumYawDeltaNegative = 0;
std::int32_t g_bestYawDeltaPositive = 0;
std::array<NavAnchorPoint *, 101> g_navAnchorCandidates{};
std::int32_t g_navAnchorCandidateCount = 0;
NavAnchorPoint g_navProbeAnchor{};
NavAnchorPoint g_navClearanceAnchor{};
NavRegion *g_currentNavRegion = nullptr;
std::int32_t g_navCachedDeltaYaw = 0;
std::int32_t g_cameraAccumulatedYawSample = 0;
std::int8_t g_turnInputLockoutFrames = 0;
std::int8_t g_snapFailCounter = 0;
PlayerMovementState g_lastNonHoldMovementState = PlayerMovementState::Idle;
std::int32_t g_inputStateHoldFrames = 0;
std::int32_t g_lastYawSampleForSnap = 0;
std::int32_t g_tempDesiredFacingYaw = 0;
Matrix4x4 g_probePoints{};
float g_probeDeltaZ = 0.0F;
float g_probeDeltaX = 0.0F;
Vector4 g_probeDeltaAccumulated{};
float g_probeRightPreviousZ = 0.0F;
float g_probeRightPreviousW = 0.0F;
std::int32_t g_probeZCompareClass = 0;
std::int32_t g_probeXCompareClass = 0;
float g_probeRightDeltaZ = 0.0F;
float g_ryoContextMotionSpeed = -0.15000001F;
std::int32_t g_sleepPromptPending = 0;
std::int32_t g_forcedControlMode = 0;
const NavRegionControlPoint *g_currentControlPoint = nullptr;
std::uint32_t g_currentControlPointCode = 0;
Vector3 g_currentControlPointPosition{};
std::int32_t g_currentControlPointYaw = 0;
InputSlot g_filteredPlayerInput{};
std::uint16_t g_directionChordState = 0;
std::uint16_t g_directionChordMask = 0;
std::int32_t g_cpctLoadHoldFrameCount = 0;
bool g_cpctLoadHoldInputCleared = false;
std::int32_t g_cpctLoadHoldCollisionLatch = 0;
std::uint8_t g_debugAnimationFlag = 0;

CharacterCollisionWork *GetPlayerCollisionWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CCOW));
    return component ? reinterpret_cast<CharacterCollisionWork *>(component + 1) : nullptr;
}

MapControlData *CurrentMapControl() {
    return g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

Character *CurrentRyo() {
    MapControlData *const control = CurrentMapControl();
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

PadCaptureEntry *CurrentMapPad() {
    MapControlData *const control = CurrentMapControl();
    return control ? static_cast<PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
}

constexpr std::uint16_t kTmexCancelInput = 0x0200U;
constexpr std::uint16_t kTmexSleepTransitionFlag = 0x00c2U;
constexpr std::uint16_t kTmexWatchAnimation = 1193U;

bool ShenmueFlagIsSet(std::uint16_t flag) {
    return (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0;
}

void InjectTMEXCancelInput() {
    InputSlot &input = g_inputState.slots[0];
    input.held = kTmexCancelInput;
    input.previousHeld = kTmexCancelInput;
    input.pressedPriority = kTmexCancelInput;
    input.repeatedPresses = kTmexCancelInput;
    input.triggers = 0;
    input.rightStick = 0x8080U;
    input.leftStick = 0;

    if (PadCaptureEntry *const pad = CurrentMapPad()) {
        pad->enabled = kTmexCancelInput;
        pad->unknownMask = kTmexCancelInput;
        pad->held = kTmexCancelInput;
        pad->previousHeld = kTmexCancelInput;
        pad->pressedPriority = 0;
        pad->repeatedPresses = 0;
        pad->triggers = 0;
        pad->rightStick = 0x8080U;
        pad->leftStick = 0;
    }
    g_ppadNeedsSetup = 1;
}

void AbortTMEXForceCancel(PlayerControlWork &control, TMEXRuntimeState &tmex) {
    g_suppressDefaultHudPromptIcons = 1;
    control.blockedCanControl = 0;
    control.state = 2;
    g_tmexValHigh = 1;
    if (g_playerMotion && g_playerMotion->secondaryAnimId == kTmexWatchAnimation) {
        if (g_tmex_val == 13U)
            MOTN_RequestSecondaryMotionStop(*g_playerMotion);
        else
            MOTN_ClearSecondaryMotion(*g_playerMotion);
    }
    TMEX_ResetForceCancelRequest();
}


bool TMEXModalOwnsCancelInput() {
    return g_cassetteCollectionUiState != 0;
}

bool NoActiveCharacterWithinOneMetre(const Character *player) {
    const MapControlData *const control = CurrentMapControl();
    for (void *const head : control->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(head); node; node = node->next) {
            const Character *const character = node->character;
            if (!character || character->entity.name != SHEN_NAME(CEWP)) {
                continue;
            }
            const std::uint32_t flags = character->flags.value;
            if ((flags & 1U) == 0 && ((flags & 2U) == 0 || (character->motionFlags & 1U) == 0)) {
                continue;
            }
            if (!MOTM_GetWork(const_cast<Character *>(character)) || character == player) {
                continue;
            }
            const float dx = player->entity.node.position.x - character->entity.node.position.x;
            const float dz = player->entity.node.position.z - character->entity.node.position.z;
            if (dx * dx + dz * dz < 1.0F)
                return false;
        }
    }
    return true;
}

float ryoUpdateCachedStates();

std::int32_t Fold16(std::int32_t yaw) {
    return static_cast<std::int16_t>(static_cast<std::uint16_t>(yaw));
}

std::int32_t Abs16(std::int32_t yaw) {
    return std::abs(Fold16(yaw));
}

std::int32_t NodeChainYaw(const CharacterNode *node) {
    std::uint16_t yaw = 0;
    for (; node; node = node->child)
        yaw = static_cast<std::uint16_t>(yaw + static_cast<std::uint16_t>(node->rotation.y));
    return static_cast<std::int16_t>(yaw);
}

std::int32_t PlayerChainYaw() {
    return g_playerCharacter ? NodeChainYaw(&g_playerCharacter->entity.node) : 0;
}

Vector3 PlayerWorldPosition() {
    return g_playerCharacter ? GetPositionForEntity(g_playerCharacter->entity.node) : Vector3{};
}

void SetLocalYawFromWorld(Character *character, std::int32_t worldYaw) {
    std::int16_t yaw = static_cast<std::int16_t>(worldYaw);
    for (CharacterNode *node = character->entity.node.child; node; node = node->child)
        yaw = static_cast<std::int16_t>(yaw - node->rotation.y);
    character->entity.node.rotation.y = yaw;
}

Vector3 TransformByWorkMatrix(const Vector3 &point) {
    const Matrix4x4 &m = workTransformMatrix;
    return {((m.rows[1].x * point.y) + (m.rows[0].x * point.x)) + (m.rows[2].x * point.z) + m.rows[3].x,
        ((m.rows[1].y * point.y) + (m.rows[0].y * point.x)) + (m.rows[2].y * point.z) + m.rows[3].y,
        ((m.rows[1].z * point.y) + (m.rows[0].z * point.x)) + (m.rows[2].z * point.z) + m.rows[3].z};
}

void RotateWorkMatrixY(std::int32_t yaw) {
    const auto angle = static_cast<std::uint16_t>(yaw);
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(angle)), cos_fixed16(angle));
}

CharacterCollisionWork *PlayerCollisionWork() {
    return g_playerCharacter ? GetPlayerCollisionWork(g_playerCharacter) : nullptr;
}

float PlayerCollisionRadius() {
    const CharacterCollisionWork *const work = PlayerCollisionWork();
    return work ? work->radialAabbExtent : 0.40000001F;
}

std::int32_t ryoCalcCurrentYaw() {
    Character *const ryo = CurrentRyo();
    std::int16_t yaw = 0;
    if (ryo) {
        g_playerNavigationPosition = GetPositionForEntity(ryo->entity.node);
        yaw = static_cast<std::int16_t>(NodeChainYaw(&ryo->entity.node));
    } else {
        g_playerNavigationPosition = {};
    }
    g_currentPlayerYaw = Fold16(yaw + 0x8000);
    return g_currentPlayerYaw;
}

void ClearForcedControlPoint() {
    g_currentControlPoint = nullptr;
    g_forcedControlMode = 0;
}

void cewpUpdatePlayerYaw(Character *character) {
    ClearForcedControlPoint();
    g_playerNavigationPosition = PlayerWorldPosition();
    const std::int32_t yaw = character ? NodeChainYaw(&character->entity.node) : 0;
    g_navSteeringClampIndex = 10;
    g_currentPlayerYaw = Fold16(yaw + 0x8000);
}

std::int32_t getNextAnimState_Internal(std::int32_t animation) {
    if (animation > kAnimRunMidLoop) {
        if (animation > kAnimTurn180WadR) {
            if (animation == kAnimStandFightLoop)
                return kStateStand;
            if (animation < kAnimStandFightLoop)
                return kStateFightFallback;
            if (animation <= kAnimWalkSlowKLoop || animation == kAnimWalkSp2Loop)
                return kStateWalk;
            return animation == kAnimRunSlowKLoop ? kStateRun : kStateFightFallback;
        }
        if (animation >= kAnimTurn045WadL)
            return kStateFight;
        if (animation == kAnimStairsDown2StepLoop)
            return kStateRun;
        if (animation <= kAnimRunKick)
            return kStateFightFallback;
        if (animation > kAnimIdleRootReturn) {
            if (animation == kAnimSlowRunLoop)
                return kStateRun;
            return animation == kAnimTurn090WadL ? kStateFight : kStateFightFallback;
        }
        return kStateStand;
    }
    if (animation == kAnimRunMidLoop)
        return kStateRun;
    if (animation <= kAnimWalk180R) {
        if (animation >= kAnimWalk045L)
            return kStateIdle;
        if (animation == kAnimRunLoop)
            return kStateRun;
        if (animation == kAnimStandLoop)
            return kStateStand;
        if (animation == kAnimWalkLoop)
            return kStateWalk;
        return animation == kAnimWalkStart ? kStateIdle : kStateFightFallback;
    }
    if (animation == kAnimWalkTurn180L)
        return kStateWalkTurn180;
    if (animation == kAnimWalkHighLoop)
        return kStateWalk;
    if (animation <= kAnimTurn180RSp3)
        return kStateFightFallback;
    if (animation <= kAnimStairsDownLoop)
        return kStateWalk;
    return animation == kAnimStairsUp2StepLoop ? kStateRun : kStateFightFallback;
}

} // namespace

std::int32_t ryoUpdateTurnMode(Character *ryo) {
    std::int32_t yaw = 0;
    if (ryo) {
        MotionManagerWork *const manager = MOTM_GetWork(ryo);
        MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
        yaw = NodeChainYaw(&ryo->entity.node);
        if (motion) {
            const std::int32_t animation = motion->pendingAnimId ? motion->pendingAnimId : motion->currentAnimId;
            const std::int32_t state = getNextAnimState_Internal(animation);
            if ((state & ~kStateRun) != 0 || state == kStateWalkTurn180)
                yaw = Fold16(charaGetWorldYaw(ryo));
        }
    }
    g_turnYawResolveMode = YawResolveMode::OctantSnapWalk;
    g_navStrafeBlocked = 0;
    g_turnModeYawFraction = 0;
    g_turnModeYaw = yaw;
    g_smoothedAnimationYaw = yaw;
    g_turnFrameCount = 9999;
    g_turnHoldFrameCount = 0;
    g_angleWrapPolicy = 0;
    return yaw;
}

namespace {

void ryoResetTurnMode() {
    g_turningPhase = TurnPhase::None;
    g_blockedDirectionMask = 0;
    g_navigationAnimationState = 0;
    g_movementBlockedFrames = 0;
    ryoUpdateTurnMode(CurrentRyo());
    g_currentTurnMode = TurnMode::Mode0;
    g_playerYawAngularSmoothed = 0;
    g_playerYawAngularTarget = 0;
    g_playerTurnStep = 8192;
    g_navigationTurnEnabled = 1;
}

void ryoUpdateTurnModeEx() {
    Character *const ryo = CurrentRyo();
    MotionManagerWork *const manager = ryo ? MOTM_GetWork(ryo) : nullptr;
    manager->motion.playbackRate = 1.0F;
    ryoUpdateTurnMode(ryo);
    g_navigationTurnEnabled = 0;
}

bool motiCheckLocomotionState(const MotionInformationWork *motion) {
    const auto mode = static_cast<std::int8_t>(motion->interpolationMode);
    if (mode <= 0)
        return false;
    if (mode == 4)
        return motion->interpolationAmount != 0.0F;
    if (mode > 9)
        return false;
    return motion->blendFramesRemaining != motion->blendFramesTotal && motion->blendFramesRemaining != 0;
}

std::int32_t ComputeNextRyoStateFromAnim() {
    const std::int32_t requested = g_playerMotion->requestedAnimId;
    const std::int32_t requestedState = getNextAnimState_Internal(requested);
    const std::int32_t blendAnimation = g_animBlendTable[static_cast<std::size_t>(g_currentAnimBlendIndex)].animationId;
    const std::int32_t current = g_currentAnimationId;
    if (current == blendAnimation && static_cast<double>(g_playerMotion->interpolationAmount) > 0.9)
        return kStateRun;
    if (static_cast<std::uint32_t>(current - kAnimIdleRoot) <= 1U || current == kAnimStandLoop || current == kAnimStandFightLoop) {
        const auto iterations = static_cast<std::uint32_t>(g_ryoIdleAnimationIterations);
        if ((requested == blendAnimation && iterations <= 6U) || (((requestedState - kStateIdle) & ~4) == 0 && iterations <= 6U))
            return kStateWalkBlend;
    }
    if (current == blendAnimation)
        return kStateWalk;
    return getNextAnimState_Internal(current);
}

PlayerControlWork *PlayerControl() {
    Character *const ryo = CurrentRyo();
    CharacterComponentNode *const component = ryo ? HLib_GetCharacterComponent(ryo, SHEN_NAME(CPCT)) : nullptr;
    return component ? reinterpret_cast<PlayerControlWork *>(component + 1) : nullptr;
}

// TryStartTurnInPlaceAnimation: publishes the frame's player animation state and activity statistics.
void TryStartTurnInPlaceAnimation() {
    g_currentAnimationId = g_playerMotion->currentAnimId;
    const std::int32_t state = ComputeNextRyoStateFromAnim();
    if (state > 0 && (state <= kStateWalkBlend || static_cast<std::uint32_t>(state - kStateIdleAlt) <= 2U)) {
        if (g_ryoIdleTurnInPlaceCounter < 9999)
            ++g_ryoIdleTurnInPlaceCounter;
        g_ryoIdleResetCounter = 0;
    } else {
        g_ryoIdleTurnInPlaceCounter = 0;
        if (g_ryoIdleResetCounter < 9999)
            ++g_ryoIdleResetCounter;
    }
    if (((state - kStateWalkTurn180) & ~6) != 0 || state == kStateRun)
        g_playerAnimationActiveFrames = 0;
    else if (g_playerAnimationActiveFrames < 9999)
        ++g_playerAnimationActiveFrames;

    if (!ShouldRun()) {
        g_ryoCurrentAnimationState = state;
        return;
    }
    const PlayerControlWork *const ryoControl = PlayerControl();
    const InputSlot *input = ryoControl ? ryoControl->inputSource : nullptr;
    if (!input)
        input = reinterpret_cast<const InputSlot *>(&CurrentMapPad()->enabled);
    const bool swapTriggers = g_applicationSettings.savedInput.swapTriggers != 0 && g_keyboardActive != 1;
    const auto primaryTrigger = static_cast<std::uint8_t>(input->triggers >> (swapTriggers ? 0U : 8U));
    const auto secondaryTrigger = static_cast<std::uint8_t>(input->triggers >> (swapTriggers ? 8U : 0U));
    const auto stickX = static_cast<std::int8_t>(static_cast<std::uint8_t>(input->rightStick) + 0x80U);
    const auto stickY = static_cast<std::int8_t>(static_cast<std::uint8_t>(input->rightStick >> 8U) + 0x80U);
    PlayerStatsPages &stats = g_playerStats.pages;
    if ((input->held & 0xfec6U) == 0 && primaryTrigger == 0 && std::abs(stickX) < 5 && std::abs(stickY) < 5 && stats.noInputFrameCount != 0xffffffffU)
        ++stats.noInputFrameCount;
    if (state != kStateStand) {
        if (state <= kStateWalkTurn180 || (state > kStatePlayRateDirect && state != kStateFightFallback)) {
            if (stats.nonNeutralNonRunAnimFrameCount != 0xffffffffU)
                ++stats.nonNeutralNonRunAnimFrameCount;
        } else if (state <= kStatePlayRateDirect && stats.runAnimFrameCount != 0xffffffffU) {
            ++stats.runAnimFrameCount;
        }
    }
    if (g_ryoCurrentAnimationState != kStateWalkBlend) {
        g_areaIdleCountBlocked = 0;
        g_ryoCurrentAnimationState = state;
        return;
    }
    const bool directional = (input->held & 0xf000U) != 0;
    if ((input->held & 0x0200U) != 0 || primaryTrigger != 0 || (!directional && secondaryTrigger == 0)) {
        g_areaIdleCountBlocked = 1;
        g_ryoCurrentAnimationState = state;
        return;
    }
    if (state == kStateStand && g_areaIdleCountBlocked == 0 && g_navStateCurrent != 2 && static_cast<std::uint32_t>(g_areaIdleFrameCounterIndex) <= 0x7fU) {
        std::uint16_t &count = stats.areaIdleFrameCountSlots[static_cast<std::size_t>(g_areaIdleFrameCounterIndex)];
        if (count < 0xffffU)
            ++count;
    }
    g_ryoCurrentAnimationState = state;
}

bool ryoCanMove() {
    const PlayerControlWork &control = *g_pControlPadController;
    if (g_playerLookRunSwapMode != 0) {
        if (g_playerLookRunSwapMode != 1 || control.modeState != 0)
            return false;
        return control.previousAutoRotateYawDelta != 0 || control.inputYawDelta8 != 0 || control.numActiveFrames != 0 ||
            g_playerMotion->interpolationAmount > 0.0F;
    }
    if (control.modeState != 0 && (control.modeState != 1 || static_cast<std::uint16_t>(control.currentMode) != 3U))
        return false;
    const auto state = static_cast<std::uint32_t>(g_playerMovementState);
    return state <= 20U && (0x188018U & (1U << state)) != 0;
}

bool cpctGetFlagSetActive() {
    const PlayerControlWork &control = *g_pControlPadController;
    return ((control.heldInput & 0x0200U) != 0 || control.triggerHigh != 0) && control.fightOnly == 0;
}

std::int32_t cpctGetNavYawRecomputeEnabled() {
    const PlayerControlWork *const control = PlayerControl();
    return control ? static_cast<std::int8_t>(control->enableNavYawRecompute) : 0;
}

std::int32_t cpctGetNewYaw() {
    const PlayerControlWork *const control = PlayerControl();
    return control ? control->newYaw : 0;
}

bool cpctGetCurrFrmCnt() {
    const PlayerControlWork *const control = PlayerControl();
    return control && static_cast<std::uint8_t>(control->currentFrameCount) > 1U;
}

bool cpctGetIsRunningRequested() {
    const PlayerControlWork &control = *g_pControlPadController;
    return control.numActiveFrames >= 120U || (control.numActiveFrames != 0 && control.isRunning != 0 && control.wasRunning != 0) ||
        g_playerMotion->interpolationAmount > 0.0F;
}

std::int32_t GetLookingAroundFactor() {
    const PadCaptureEntry *const pad = CurrentMapPad();
    if (!pad || !CurrentRyo())
        return 0;
    const bool swapTriggers = g_applicationSettings.savedInput.swapTriggers != 0 && g_keyboardActive != 1;
    const auto trigger = static_cast<std::uint8_t>(pad->triggers >> (swapTriggers ? 0U : 8U));
    return trigger >= 10U ? trigger - 10 : 0;
}

std::int32_t Atan2Angle(float x, float z) {
    return Shenmue_Atan2Fixed(x, z);
}

const std::uint8_t *CurrentCycleMask() {
    MapControlData *const control = CurrentMapControl();
    return control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr;
}

Vector3 ApplyRotatedBackwardDisplacement(const Vector3 &position, std::int32_t yaw, float distance) {
    Matrix4x4::SetActiveTransformToPrePass();
    RotateWorkMatrixY(yaw);
    const Matrix4x4 &m = workTransformMatrix;
    const float x = ((m.rows[0].x * 0.0F + m.rows[1].x * 0.0F) + m.rows[2].x * -distance) + m.rows[3].x;
    const float z = ((m.rows[0].z * 0.0F + m.rows[1].z * 0.0F) + m.rows[2].z * -distance) + m.rows[3].z;
    Matrix4x4::Pop();
    return {x + position.x, position.y, z + position.z};
}


NavAnchorPoint navComputeProbePointFromYaw(const Vector3 &position, std::int32_t yaw, float magnitude) {
    workTransformMatrix = transformMatrix_prePass;
    RotateWorkMatrixY(yaw);
    const Matrix4x4 &m = workTransformMatrix;
    const float y = ((m.rows[0].y * 0.0F + m.rows[1].y * 0.0F) + m.rows[2].y * magnitude) + m.rows[3].y;
    const float z = ((m.rows[0].z * 0.0F + m.rows[1].z * 0.0F) + m.rows[2].z * magnitude) + m.rows[3].z;
    const float x = ((m.rows[0].x * 0.0F + m.rows[1].x * 0.0F) + m.rows[2].x * magnitude) + m.rows[3].x;
    return {x + position.x, y + position.y, z + position.z, 0.0F};
}

Vector3 ryoTransformWorldToPlayerYawSpace(const Vector3 &point) {
    const Vector3 origin = PlayerWorldPosition();
    Matrix4x4::SetActiveTransformToPrePass();
    RotateWorkMatrixY(0x8000 - g_currentPlayerYaw);
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    const Vector3 result = TransformByWorkMatrix(point);
    Matrix4x4::Pop();
    return result;
}

void ryoComputeYawOffsProbeWithIntersection(const Vector3 &point, std::int32_t yaw, float distance) {
    Matrix4x4::SetActiveTransformToPrePass();
    RotateWorkMatrixY(yaw);
    const Matrix4x4 &m = workTransformMatrix;
    const float x = ((m.rows[0].x * 0.0F + m.rows[1].x * 0.0F) + m.rows[2].x * -distance) + m.rows[3].x;
    const float y = ((m.rows[0].y * 0.0F + m.rows[1].y * 0.0F) + m.rows[2].y * -distance) + m.rows[3].y;
    const float z = ((m.rows[0].z * 0.0F + m.rows[1].z * 0.0F) + m.rows[2].z * -distance) + m.rows[3].z;
    Matrix4x4::Pop();
    mapUpdateIntersectionStateAtLocalPoint({x + point.x, y + point.y, z + point.z});
}

bool navIsProbePosClear(const Vector3 &probe, float radius) {
    Vector3 local = probe;
    if (g_playerRootNode->child)
        local = WorldToEntityLocalPoint(*g_playerRootNode->child, probe);
    if (cewpTestPointWithinRadius(local, radius, CurrentCycleMask()))
        return false;
    const float ground = mapUpdateIntersectionStateAtLocalPoint(probe);
    return static_cast<double>(ground) < static_cast<double>(probe.y) + 0.5;
}

bool navIsPlayerProbeValid(const NavAnchorPoint &probe, float clearanceRadius) {
    NavRegion *head = nullptr;
    if (!g_skipNavRegionFiltering) {
        head = g_navRegionListHead;
        g_currentNavRegion = head;
    }
    const Vector3 point{probe.x, probe.y, probe.z};
    if (!navIsProbePosClear(point, clearanceRadius))
        return false;
    if (!head || g_navRegionFilterDisabled == 1)
        return true;
    NavRegion *region = g_currentNavRegion;
    while (!navIsPointInsideRegion(head, &point)) {
        if (g_skipNavRegionFiltering)
            return false;
        if (region) {
            region = region->next;
            g_currentNavRegion = region;
        }
        head = region;
        if (!region)
            return false;
    }
    return true;
}

bool ProbeFacingClearance(const Vector3 &position, std::int32_t yaw, float distance) {
    Matrix4x4::SetActiveTransformToPrePass();
    RotateWorkMatrixY(yaw + 0x8000);
    const Vector3 offset = TransformByWorkMatrix({0.0F, 0.0F, distance});
    Matrix4x4::Pop();
    const NavAnchorPoint probe{position.x + offset.x, position.y + offset.y, position.z + offset.z, 0.0F};
    return navIsPlayerProbeValid(probe, g_playerNavigationClearanceRadius);
}

bool navIsBehindProbeBlocked(std::int32_t yawOffset, float distance) {
    const Vector3 probe = ApplyRotatedBackwardDisplacement(PlayerWorldPosition(), PlayerChainYaw() + yawOffset, 0.40000001F);
    return !navIsProbePosClear(probe, distance);
}

std::int32_t navGetStrafeClearanceState() {
    const bool left = navIsBehindProbeBlocked(0x4000, 0.40000001F);
    const bool right = navIsBehindProbeBlocked(-0x4000, 0.40000001F);
    if (!left)
        return right ? 1 : 0;
    return right ? 0 : 2;
}

bool navFindActiveRegionForPlayer() {
    if (g_skipNavRegionFiltering)
        return false;
    NavRegion *next = g_navRegionListHead;
    g_currentNavRegion = next;
    NavRegion *region = next;
    if (!region)
        return false;
    while (!navIsPointInsideRegion(region, &g_playerNavigationPosition)) {
        if (next) {
            next = next->next;
            g_currentNavRegion = next;
        }
        region = next;
        if (!next)
            return false;
    }
    return true;
}

bool AnchorInHeightBand(const NavAnchorPoint &anchor, float height) {
    return static_cast<double>(anchor.y) >= static_cast<double>(height) - 2.0 && static_cast<double>(height) + 2.0 >= static_cast<double>(anchor.y);
}

NavAnchorPoint *navFindNearestAnchor(const Vector3 &position) {
    if (g_skipNavRegionFiltering)
        return nullptr;
    NavAnchorPoint *nearest = nullptr;
    float nearestDistance = 99999.0F;
    NavRegion *region = g_navRegionListHead;
    g_currentNavRegion = region;
    const float height = g_playerNavigationPosition.y;
    while (region) {
        NavAnchorPoint *const anchors = region->activeAnchors;
        for (std::int32_t index = 0; index < region->anchors.count; ++index) {
            NavAnchorPoint &anchor = anchors[index];
            if (!AnchorInHeightBand(anchor, height))
                continue;
            const float magnitude = ((anchor.y - position.y) * (anchor.y - position.y) + (anchor.x - position.x) * (anchor.x - position.x)) +
                (anchor.z - position.z) * (anchor.z - position.z);
            if (!(anchor.radius * anchor.radius > magnitude && magnitude >= 0.0F))
                continue;
            const float distance = std::sqrt((position.x - anchor.x) * (position.x - anchor.x) + (position.z - anchor.z) * (position.z - anchor.z));
            if (nearestDistance > distance) {
                nearestDistance = distance;
                nearest = &anchor;
            }
        }
        region = region->next;
        g_currentNavRegion = region;
    }
    return nearest;
}

void navGatherNearbyAnchorsFromAreas() {
    std::int32_t count = g_navAnchorCandidateCount;
    const std::int32_t seeds = count;
    const float height = g_playerNavigationPosition.y;
    for (std::int32_t seed = 0; seed < seeds; ++seed) {
        const NavAnchorPoint &source = *g_navAnchorCandidates[static_cast<std::size_t>(seed)];
        if (g_skipNavRegionFiltering)
            continue;
        g_currentNavRegion = g_navRegionListHead;
        for (NavRegion *region = g_navRegionListHead; region; region = region->next) {
            NavAnchorPoint *const anchors = region->activeAnchors;
            if (region->anchors.count > 0) {
                for (std::int32_t index = 0; index < region->anchors.count; ++index) {
                    NavAnchorPoint &anchor = anchors[index];
                    if (!AnchorInHeightBand(anchor, height))
                        continue;
                    const float magnitude = ((anchor.y - source.y) * (anchor.y - source.y) + (anchor.x - source.x) * (anchor.x - source.x)) +
                        (anchor.z - source.z) * (anchor.z - source.z);
                    if ((source.radius + anchor.radius) * (source.radius + anchor.radius) > magnitude && magnitude >= 0.010000001F) {
                        g_navAnchorCandidates[static_cast<std::size_t>(count++)] = &anchor;
                        if (count >= 100)
                            break;
                    }
                }
                g_navAnchorCandidateCount = count;
            }
            g_currentNavRegion = region->next;
        }
        g_currentNavRegion = nullptr;
    }
}

NavAnchorPoint *navSelectBestAnchorByYawDelta(const Vector3 &position, std::int32_t baseYaw) {
    NavAnchorPoint *best = nullptr;
    std::int32_t negativeBound = -65536;
    std::int32_t positiveBound = 65536;
    std::int32_t bestDelta = 655360;
    g_minimumYawDeltaNegative = -65536;
    g_bestYawDeltaPositive = 65536;
    if (g_navAnchorCandidateCount <= 0)
        return nullptr;
    for (std::int32_t index = 0; index < g_navAnchorCandidateCount; ++index) {
        NavAnchorPoint *const anchor = g_navAnchorCandidates[static_cast<std::size_t>(index)];
        if (!(static_cast<double>((position.x - anchor->x) * (position.x - anchor->x) + (position.z - anchor->z) * (position.z - anchor->z)) > 0.01))
            continue;
        const std::int32_t yaw = Fold16(Atan2Angle(anchor->x - position.x, anchor->z - position.z) - baseYaw);
        if (yaw >= 0) {
            if (yaw < positiveBound)
                positiveBound = yaw;
            g_bestYawDeltaPositive = positiveBound;
        } else if (yaw > negativeBound) {
            negativeBound = yaw;
        }
        if (std::abs(yaw) < bestDelta) {
            bestDelta = std::abs(yaw);
            best = anchor;
        }
    }
    g_minimumYawDeltaNegative = negativeBound;
    return best;
}

NavAnchorPoint *navProbeClearancePattern(const Vector3 &position, std::int32_t baseYaw, float magnitude, NavProbeSector mode) {
    const std::int32_t *directions = kNavProbeYawOffsetsWide.data();
    std::size_t count = kNavProbeYawOffsetsWide.size();
    if (mode == NavProbeSector::Positive120Fan || mode == NavProbeSector::Positive120FanAlt) {
        directions = kNavProbeYawOffsetsPositive.data();
        count = kNavProbeYawOffsetsPositive.size();
    } else if (mode == NavProbeSector::Negative120Fan || mode == NavProbeSector::Negative120FanAlt) {
        directions = kNavProbeYawOffsetsNegative.data();
        count = kNavProbeYawOffsetsNegative.size();
    } else if (mode == NavProbeSector::NarrowCandidateBias) {
        directions = kNavProbeYawOffsetsNarrow.data();
        count = g_navAnchorCandidateCount ? 1 : 5;
    }
    for (std::size_t index = 0; index < count; ++index) {
        const NavAnchorPoint point = navComputeProbePointFromYaw(position, baseYaw + directions[index], magnitude);
        if (navIsPlayerProbeValid(point, g_playerNavigationClearanceRadius)) {
            g_navClearanceAnchor = {point.x, point.y, point.z, 0.30000001F};
            return &g_navClearanceAnchor;
        }
    }
    return nullptr;
}

NavAnchorPoint *navBuildAnchorCandidatesFromProbePattern(const Vector3 &position, std::int32_t baseYaw, NavProbeSector mode) {
    NavAnchorPoint *const anchor = navProbeClearancePattern(position, baseYaw, 0.44999999F, mode);
    if (anchor && navIsPlayerProbeValid(*anchor, g_playerNavigationClearanceRadius))
        g_navAnchorCandidates[static_cast<std::size_t>(g_navAnchorCandidateCount++)] = anchor;
    if (g_navAnchorCandidateCount <= 0)
        return nullptr;
    if (g_navAnchorCandidateCount != 1)
        return navSelectBestAnchorByYawDelta(position, baseYaw);
    g_minimumYawDeltaNegative = 0;
    return g_navAnchorCandidates[0];
}

bool navFindLegalFacingInSector(const Vector3 &position, std::int32_t baseYaw, NavProbeSector mode) {
    Matrix4x4::SetActiveTransformToPrePass();
    g_navAnchorCandidateCount = 0;
    if (NavAnchorPoint *const nearest = navFindNearestAnchor(position)) {
        g_navAnchorCandidates[0] = nearest;
        g_navAnchorCandidateCount = 1;
    }
    navGatherNearbyAnchorsFromAreas();
    const NavAnchorPoint *const anchor = navBuildAnchorCandidatesFromProbePattern(position, baseYaw, mode);
    Matrix4x4::Pop();
    if (!anchor) {
        g_targetPlayerYawSnap = 0;
        return false;
    }
    g_targetPlayerYawSnap = Atan2Angle(anchor->x - position.x, anchor->z - position.z);
    return true;
}

void PublishProbeAnchor(const NavAnchorPoint &point) {
    g_navProbeAnchor = {point.x, point.y, point.z, 0.30000001F};
    g_navAnchorCandidates[static_cast<std::size_t>(g_navAnchorCandidateCount++)] = &g_navProbeAnchor;
}

bool TryProbeAnchor(std::int32_t yaw) {
    const NavAnchorPoint point = navComputeProbePointFromYaw(g_lastProbePosition, yaw, 0.30000001F);
    if (!navIsPlayerProbeValid(point, g_playerNavigationClearanceRadius))
        return false;
    PublishProbeAnchor(point);
    return true;
}

void navResolveProbeDirection(std::int32_t probeCenterYaw) {
    PlayerControlWork *const control = g_pControlPadController;
    std::int32_t probeYaw = Fold16(control->forcedYaw + 0x8000);
    const NavAnchorPoint first = navComputeProbePointFromYaw(g_playerNavigationPosition, probeYaw, 0.30000001F);
    if (navIsPlayerProbeValid(first, g_playerNavigationClearanceRadius + 0.0F)) {
        PublishProbeAnchor(first);
        return;
    }

    const std::int32_t direction = g_navDirectionFlags;
    float lateral = -0.050000001F;
    if (g_navMetricCurrent != 2) {
        const auto directionWord = static_cast<std::uint16_t>(direction);
        const std::int16_t folded =
            directionWord > 0x8000U ? static_cast<std::int16_t>(-static_cast<std::int16_t>(directionWord)) : static_cast<std::int16_t>(directionWord);
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        const float inverse = cosine == 0.0F ? 0.0F : 0.050000001F / cosine;
        const float dx = g_lastProbePosition.x - g_forwardProbePosition.x;
        const float dz = g_lastProbePosition.z - g_forwardProbePosition.z;
        lateral = std::sqrt(dx * dx + dz * dz) - inverse;
    }
    const std::int32_t baseYaw = static_cast<std::int16_t>(g_navProbeBaseYaw);
    const Vector3 lateralPosition = ApplyRotatedBackwardDisplacement(g_lastProbePosition, baseYaw, lateral);
    const auto absoluteDirection = static_cast<std::uint16_t>(direction < 0 ? -direction : direction);
    const auto reflected = static_cast<std::uint16_t>(0x8000 - absoluteDirection);
    const auto foldedReflected = static_cast<std::int16_t>(reflected <= 0x8000U ? reflected : 0x8000 + absoluteDirection);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedReflected));

    std::uint32_t mode = 1;
    bool useProjected = true;
    NavAnchorPoint projected{};
    if (g_hasNavSnapBudget && g_navProbeSpeed < 0.30000001F) {
        useProjected = false;
    } else {
        const float discriminant = ((lateral * lateral) * cosine * cosine - lateral * lateral) + 0.090000004F;
        if (discriminant <= 0.0F) {
            const std::int32_t yaw = static_cast<std::int16_t>(baseYaw + direction);
            const Vector3 point = ApplyRotatedBackwardDisplacement(g_lastProbePosition, yaw, 0.30000001F);
            projected = {point.x, point.y, point.z, 0.0F};
            probeYaw = static_cast<std::int16_t>(yaw + 0x8000);
        } else {
            const Vector3 point = ApplyRotatedBackwardDisplacement(lateralPosition, direction + baseYaw, std::sqrt(discriminant) + cosine * lateral);
            projected = {point.x, point.y, point.z, 0.0F};
            probeYaw = static_cast<std::int16_t>(Atan2Angle(projected.x - g_lastProbePosition.x, projected.z - g_lastProbePosition.z));
        }
    }

    const std::int32_t delta = Fold16(probeYaw - probeCenterYaw);
    if (delta < -12743 && control->autoRotateYawDelta < 0) {
        if (cpctGetIsRunningRequested() || control->inputYawDelta < 0)
            useProjected = false;
        else
            mode = 3;
    } else if (delta > 12743 && control->autoRotateYawDelta > 0) {
        if (cpctGetIsRunningRequested() || control->inputYawDelta < 0) {
            mode = 2;
            useProjected = false;
        } else {
            mode = 4;
        }
    } else if (static_cast<std::uint32_t>(g_navigationAnimationState - 1) <= 3U || control->autoRotateYawDelta != 0) {
        mode = 5;
        if (Abs16(probeYaw - probeCenterYaw) > 12743)
            useProjected = false;
    } else {
        mode = 0;
        if (Abs16(probeYaw - probeCenterYaw) > 12743)
            useProjected = false;
    }

    if (mode == 1 || mode == 3) {
        for (std::int32_t offset = 1820; offset <= 12743; offset += 1820) {
            if (TryProbeAnchor(offset + probeCenterYaw))
                return;
        }
    } else if (mode == 2 || mode == 4) {
        for (std::int32_t offset = -1820; offset >= -12743; offset -= 1820) {
            if (TryProbeAnchor(offset + probeCenterYaw))
                return;
        }
    }

    if (mode <= 2 || mode == 5) {
        if (TryProbeAnchor(probeCenterYaw))
            return;
        if (g_navMetricCurrent == 2 || mode == 5) {
            for (const std::int32_t offset : kNavResolveProbeYawOffsets) {
                if (TryProbeAnchor(probeCenterYaw + offset))
                    return;
            }
        }
    }

    if (g_navMetricCurrent != 2 && !cpctGetCurrFrmCnt() && g_hasNavSnapBudget && mode - 1U > 1U)
        return;
    if (useProjected && (mode <= 2 || mode == 5) && navIsPlayerProbeValid(projected, g_playerNavigationClearanceRadius)) {
        PublishProbeAnchor(projected);
        return;
    }
    if (mode != 0 || g_navStateCurrent == 2)
        return;
    for (std::int32_t positive = probeCenterYaw + 1820, negative = -1820;; positive += 1820) {
        if (TryProbeAnchor(positive) || TryProbeAnchor(negative + probeCenterYaw))
            return;
        negative -= 1820;
        if (negative < -12743)
            return;
    }
}

NavAnchorPoint *navGatherAnchorsFromRegions(std::int32_t desiredYaw, std::int32_t probeYaw, const Vector3 &position, float radiusAdd) {
    if (!g_skipNavRegionFiltering && g_navRegionListHead) {
        g_currentNavRegion = g_navRegionListHead;
        std::int32_t count = g_navAnchorCandidateCount;
        const Vector3 origin = g_playerNavigationPosition;
        for (NavRegion *region = g_navRegionListHead; region; region = g_currentNavRegion) {
            NavAnchorPoint *const anchors = region->activeAnchors;
            if (region->anchors.count > 0) {
                for (std::int32_t index = 0; index < region->anchors.count; ++index) {
                    NavAnchorPoint &anchor = anchors[index];
                    if (!AnchorInHeightBand(anchor, origin.y))
                        continue;
                    const float magnitude = ((anchor.y - position.y) * (anchor.y - position.y) + (anchor.x - position.x) * (anchor.x - position.x)) +
                        (anchor.z - position.z) * (anchor.z - position.z);
                    if ((radiusAdd + anchor.radius) * (radiusAdd + anchor.radius) > magnitude && magnitude >= 0.0F &&
                        (origin.x != anchor.x || origin.y != anchor.y || origin.z != anchor.z) &&
                        Abs16(Atan2Angle(anchor.x - origin.x, anchor.z - origin.z) - desiredYaw) < 16384) {
                        g_navAnchorCandidates[static_cast<std::size_t>(count++)] = &anchor;
                        if (count >= 100)
                            break;
                    }
                }
                g_navAnchorCandidateCount = count;
            }
            g_currentNavRegion = region->next;
        }
    }
    navResolveProbeDirection(desiredYaw);
    if (g_navAnchorCandidateCount <= 0)
        return nullptr;
    if (g_navAnchorCandidateCount != 1)
        return navSelectBestAnchorByYawDelta(g_playerNavigationPosition, static_cast<std::int16_t>(probeYaw));
    g_minimumYawDeltaNegative = 0;
    return g_navAnchorCandidates[0];
}

std::int32_t navUpdateYawSnapAndResolveFacing(std::int32_t baseYaw) {
    Vector2 last{g_playerNavigationPosition.x, g_playerNavigationPosition.y};
    g_navPreviousDeltaPosition = g_playerNavigationPosition;
    if (g_navSteeringClampIndex == 10) {
        workTransformMatrix = transformMatrix_prePass;
        RotateWorkMatrixY(baseYaw);
        const Matrix4x4 &m = workTransformMatrix;
        const float x = ((m.rows[0].x * 0.0F + m.rows[1].x * 0.0F) - m.rows[2].x * 0.30000001F) + m.rows[3].x;
        const float y = ((m.rows[0].y * 0.0F + m.rows[1].y * 0.0F) - m.rows[2].y * 0.30000001F) + m.rows[3].y;
        const float z = ((m.rows[0].z * 0.0F + m.rows[1].z * 0.0F) - m.rows[2].z * 0.30000001F) + m.rows[3].z;
        g_navForwardDelta = {g_playerNavigationPosition.x + x, g_playerNavigationPosition.y + y, g_playerNavigationPosition.z + z};
        last = {g_playerNavigationPosition.x, g_playerNavigationPosition.y};
    }
    g_navLastEntityPosition = last;
    g_navRadiusAdd = 0.5F;
    g_navLastZ = g_playerNavigationPosition.z;
    g_navTemporaryProbeYaw += static_cast<std::int16_t>(g_navSnapYawAngle + baseYaw - g_navTemporaryProbeYaw) / 2;
    const std::int32_t probe = g_navTemporaryProbeYaw;
    g_playerMotion->motionFlags &= ~kMotionUnknown8;
    const bool inside = navFindActiveRegionForPlayer();
    g_navHasNoLegalFacing = 0;
    g_navRegionFilterDisabled = inside ? 0 : 1;
    g_navAnchorCandidateCount = 0;
    const NavAnchorPoint *const anchor =
        navGatherAnchorsFromRegions(baseYaw, probe, {g_navLastEntityPosition.x, g_navLastEntityPosition.y, g_navLastZ}, g_navRadiusAdd);
    if (g_navAnchorCandidateCount <= 0) {
        g_navHasNoLegalFacing = 1;
        return -1;
    }
    float nextX = 0.0F;
    float nextZ = 0.0F;
    if (anchor) {
        g_navDeltaPosition = {anchor->x, anchor->y, anchor->z};
        nextX = anchor->x;
        nextZ = anchor->z;
    } else {
        if (navFindLegalFacingInSector(g_playerNavigationPosition, probe, NavProbeSector::FullSweep)) {
            g_navHasNoLegalFacing = 1;
            return -1;
        }
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_pControlPadController->forcedYaw + 0x8000));
        const Vector3 delta = TransformByWorkMatrix({0.0F, 0.0F, 0.30000001F});
        nextX = g_playerNavigationPosition.x + delta.x;
        nextZ = g_playerNavigationPosition.z + delta.z;
        g_navDeltaPosition = {nextX, g_playerNavigationPosition.y + delta.y, nextZ};
    }
    g_navSuggestedYaw = Atan2Angle(nextX - g_navPreviousDeltaPosition.x, nextZ - g_navPreviousDeltaPosition.z);
    g_navAnchorCandidateCount = 0;
    g_navSpeedClampMaximum = -1.0F;
    return 0;
}

std::int32_t navUpdateSteeringAndResolveFacing(std::int32_t currentPlayerYaw) {
    Matrix4x4::SetActiveTransformToPrePass();
    g_navUseCachedYaw = 0;
    std::int32_t snap = g_currentTurnMode == TurnMode::Mode14 ? 0 : Fold16(g_pControlPadController->secondaryYawOffset - currentPlayerYaw + 0x8000);
    snap = std::min(std::max(snap, -10922), 10922);
    if (std::abs(snap) < 1820)
        snap = 0;
    g_navSnapYawAngle = snap;
    const std::int32_t result = navUpdateYawSnapAndResolveFacing(currentPlayerYaw);
    Matrix4x4::Pop();
    return result < 0 ? result : 0;
}

std::int32_t navUpdateSteeringYawFromResolvedDelta(float cachedYaw, Vector3 &projected) {
    if (g_navHasNoLegalFacing == 1 ||
        (g_navPreviousDeltaPosition.x == g_navDeltaPosition.x && g_navPreviousDeltaPosition.y == g_navDeltaPosition.y &&
            g_navPreviousDeltaPosition.z == g_navDeltaPosition.z)) {
        return -1;
    }
    std::int32_t yaw = std::bit_cast<std::int32_t>(cachedYaw);
    const std::int32_t base = g_navTemporaryYaw;
    if (!g_navUseCachedYaw) {
        const float dx = g_navDeltaPosition.x - g_navPreviousDeltaPosition.x;
        const float dz = g_navDeltaPosition.z - g_navPreviousDeltaPosition.z;
        const std::int32_t heading = static_cast<std::int16_t>(Atan2Angle(dx, dz));
        const auto scale = static_cast<float>(static_cast<double>(g_locomotionSpeed) * 3.333333333333333);
        projected.x = dx * scale + projected.x;
        projected.z = dz * scale + projected.z;
        yaw = Fold16(heading - base);
        g_navCachedDeltaYaw = 0;
    }
    const PlayerControlWork *const control = PlayerControl();
    if (!control || static_cast<std::uint8_t>(control->idleTurnActive) == 0) {
        const std::int32_t clamp = kNavSteeringClampAngles[static_cast<std::size_t>(g_navSteeringClampIndex)];
        if (yaw > clamp)
            yaw = clamp;
        else if (yaw < -clamp)
            yaw = -clamp;
        g_navTemporaryYaw = Fold16(yaw + base);
        return 0;
    }
    std::int32_t velocity = control->turnVelocity;
    if (g_updateRenderFrameMode > 0)
        velocity = static_cast<std::int32_t>(static_cast<double>(static_cast<float>(velocity)) * 1.1875);
    else if (g_updateRenderFrameMode == 0)
        velocity = static_cast<std::int32_t>(static_cast<double>(static_cast<float>(velocity)) * 0.0625);
    if (velocity >= 0) {
        if (yaw > velocity)
            yaw = velocity;
    } else if (yaw < velocity) {
        yaw = velocity;
    }
    if (yaw < 0) {
        if (yaw < -910)
            yaw = -910;
    } else if (yaw > 910) {
        yaw = 910;
    }
    g_navTemporaryYaw = Fold16(yaw + base);
    return 0;
}

void FindNearestControlPointForPos(const Vector3 &position) {
    NavRegion *region = nullptr;
    NavRegion *head = nullptr;
    if (g_skipNavRegionFiltering) {
        region = g_currentNavRegion;
    } else {
        head = g_navRegionListHead;
        region = head;
        g_currentNavRegion = head;
    }
    const float dy = g_playerNavigationPosition.y - position.y;
    const float dx = g_playerNavigationPosition.x - position.x;
    const float dz = g_playerNavigationPosition.z - position.z;
    ClearForcedControlPoint();
    const float reach = static_cast<float>(static_cast<double>(std::sqrt((dy * dy + dx * dx) + dz * dz)) + 0.1);
    Matrix4x4::SetActiveTransformToPrePass();
    while (head) {
        const auto *const points = static_cast<const NavRegionControlPoint *>(head->namedCircles.data);
        const std::int32_t count = head->namedCircles.count;
        workTransformMatrix = transformMatrix_prePass;
        RotateWorkMatrixY(-head->yaw);
        Matrix4x4::preTranslate(-head->origin.x, -head->origin.y, -head->origin.z);
        const Vector3 local = TransformByWorkMatrix(position);
        for (std::int32_t index = 0; index < count; ++index) {
            const auto *const record = reinterpret_cast<const std::byte *>(points) + static_cast<std::size_t>(index) * 20U;
            Vector3 pointPosition{};
            std::int16_t pointYaw = 0;
            std::uint32_t pointCode = 0;
            std::memcpy(&pointPosition, record, sizeof(pointPosition));
            std::memcpy(&pointYaw, record + 12, sizeof(pointYaw));
            std::memcpy(&pointCode, record + 16, sizeof(pointCode));
            const float distance =
                std::sqrt(((local.y - pointPosition.y) * (local.y - pointPosition.y) + (local.x - pointPosition.x) * (local.x - pointPosition.x)) +
                    (local.z - pointPosition.z) * (local.z - pointPosition.z));
            if (distance > 1.0F || Abs16(pointYaw + static_cast<std::int16_t>(head->yaw - g_currentPlayerYaw)) > 10922)
                continue;
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(head->origin.x, head->origin.y, head->origin.z);
            RotateWorkMatrixY(head->yaw);
            g_currentControlPoint = reinterpret_cast<const NavRegionControlPoint *>(record);
            g_currentControlPointPosition = TransformByWorkMatrix(pointPosition);
            g_currentControlPointYaw = static_cast<std::int16_t>(pointYaw + head->yaw);
            g_currentControlPointCode = pointCode;
            if (reach >= distance) {
                g_forcedControlMode = 1;
                break;
            }
            g_forcedControlMode = 2;
        }
        if (g_skipNavRegionFiltering)
            break;
        if (region)
            region = region->next;
        head = region;
        g_currentNavRegion = region;
    }
    Matrix4x4::Pop();
}

void ryoUpdateEntityPositionForNavigation(Character *ryo) {
    if (g_navFilterDisabled) {
        g_playerEntityPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
        g_playerNavigationPosition = g_playerEntityPosition;
        return;
    }
    if (g_navHasNoLegalFacing == 1 || !g_forceYawRecalculation) {
        if (g_navFilterProjectPositionOnce) {
            g_playerNavigationPosition = g_playerMotion->rootWorldPosition;
            g_playerNavigationPosition.y = mapUpdateIntersectionStateAtLocalPoint(g_playerNavigationPosition);
            g_navFilterProjectPositionOnce = 0;
        }
    } else {
        ryoCalcCurrentYaw();
        g_navSteeringClampIndex = 10;
        ClearForcedControlPoint();
        g_forceYawRecalculation = 0;
    }
    g_playerEntityPosition = g_playerNavigationPosition;
}

// ryoRecomputeYawAccum: returns the navigation clearance at the adjusted yaw.
float ryoRecomputeYawAccum(float radius, std::int32_t yawOffset) {
    std::int32_t yaw = yawOffset + 0x8000;
    if (g_playerCharacter)
        yaw += g_playerCharacter->entity.node.rotation.y - NodeChainYaw(&g_playerCharacter->entity.node);
    g_navRecomputedSpeed = ryoComputeNavAngleDeltaAtYaw(g_lastProbePosition, static_cast<std::uint16_t>(yaw), nullptr, radius);
    return g_navRecomputedSpeed;
}

float ryoComputeSpeed01FromYawAndClearance(const Vector3 &probe, std::int32_t probeMode, std::int32_t probeYaw, std::int32_t baseYaw) {
    g_speedWasLimited = 0;
    float limit = 10.0F;
    if (g_currentTurnMode == TurnMode::Mode1 || g_currentTurnMode == TurnMode::Mode3) {
        const std::int32_t bias = g_currentTurnMode == TurnMode::Mode1 ? 3640 : 0xf1c8;
        const float distance = ryoComputeNavAngleDeltaAtYaw(probe, static_cast<std::uint16_t>(bias + probeYaw), &g_turnBiasProbeYaw, 0.0F);
        if (std::abs(g_turnBiasProbeYaw) < 0x2000)
            limit = sin_fixed16(static_cast<std::int16_t>(std::abs(0x4000 - g_turnBiasProbeYaw))) * distance;
    }
    g_navProbeYawDelta = Fold16(g_navProbeYawDelta - baseYaw + g_navProbeBaseYaw);
    const CharacterCollisionWork *const collision = PlayerCollisionWork();
    const float distance =
        ryoComputeNavAngleDeltaAtYaw(probe, static_cast<std::uint16_t>(probeYaw), &g_navProbeYawDelta, collision ? collision->radialAabbExtent : 0.40000001F);
    g_navStateCurrent = g_navStateNext;
    g_navMetricCurrent = g_navMetricNext;
    g_navProbeBaseYaw = baseYaw;
    if (probeMode == 0) {
        g_lastProbePosition = probe;
        g_forwardProbePosition = ApplyRotatedBackwardDisplacement(probe, baseYaw, distance);
    }
    if (probeMode == 0 || probeMode == 2)
        g_navProbeYawOffset = 0;
    const std::int32_t yawDelta = g_navProbeYawDelta;
    const std::int32_t absoluteYaw = std::abs(yawDelta);
    g_navDirectionFlags = 0;
    g_navLateralDistance = 9999.0F;
    g_navProbeSpeed = distance;
    const std::int32_t facingDelta = std::abs(Fold16(g_pControlPadController->secondaryYawOffset - baseYaw + yawDelta));
    if (absoluteYaw <= 0x4000) {
        g_navLateralDistance = sin_fixed16(static_cast<std::int16_t>(std::abs(0x4000 - yawDelta))) * distance;
        const std::int32_t direction = static_cast<std::int16_t>(yawDelta > 0 ? yawDelta - 0x4000 : yawDelta - 0xc000);
        g_navDirectionFlags = direction;
        const std::int32_t probeDirection = direction >= 0 ? baseYaw - 0x8000 + direction : direction + baseYaw;
        if (probeMode == 0 || probeMode == 2)
            g_navProbeYawOffset = probeDirection - baseYaw;
    }
    if (collision && collision->avoidanceResult == 2)
        return 0.0F;
    float speed = distance;
    if (absoluteYaw <= 0x4000 && (absoluteYaw >= 0x1555 || facingDelta >= 0x1555)) {
        if (speed > limit) {
            g_speedWasLimited = 1;
            return limit;
        }
        speed = speed > g_navRecomputedSpeed ? speed : g_navRecomputedSpeed;
    }
    return 10.0F > speed ? static_cast<float>(static_cast<double>(speed)) : static_cast<float>(10.0);
}

void ryoUpdateSpeedAndRemainingCount() {
    PlayerControlWork &control = *g_pControlPadController;
    if (control.forcedYawSnap == 1) {
        g_underMinimumSnapSpeed = 0;
        g_hasNavSnapBudget = 0;
        return;
    }
    float minimum = 0.1F;
    if (g_ryoCurrentAnimationState == kStateWalk || g_ryoCurrentAnimationState == kStateWalkBlend)
        minimum = 0.25F;
    else if (g_ryoCurrentAnimationState == kStateRun || g_ryoCurrentAnimationState == kStateRunBlend)
        minimum = 0.73000002F;
    g_underMinimumSnapSpeed = minimum >= g_ryoRawMovementInputSpeed ? 1 : 0;
    const std::int32_t input = control.inputYawDelta;
    if (input < 0 || control.numActiveFrames >= 120U || (control.numActiveFrames != 0 && control.isRunning != 0 && control.wasRunning != 0) ||
        g_playerMotion->interpolationAmount > 0.0F || (input == 0 && control.autoRotateYawDelta == 0)) {
        g_hasNavSnapBudget = std::abs(g_navDirectionFlags) > 12743 ? 1 : 0;
    } else if (control.autoRotateYawDelta > 0) {
        g_hasNavSnapBudget = static_cast<std::uint32_t>(g_navDirectionFlags - 5462) <= 7280U ? 1 : 0;
    } else if (control.autoRotateYawDelta < 0) {
        g_hasNavSnapBudget = static_cast<std::uint32_t>(g_navDirectionFlags + 12742) <= 7280U ? 1 : 0;
    }
}

void ryoUpdateLocomotionSpeedScalar(Character *character) {
    g_navFilterDisabled = !g_navigationCollisionDataTable || g_skipNavRegionFiltering ? 1 : 0;
    ryoUpdateEntityPositionForNavigation(character);
    Vector3 local = g_playerNavigationPosition;
    if (g_playerRootNode->child)
        local = WorldToEntityLocalPoint(*g_playerRootNode->child, local);
    const std::int32_t rootYaw = g_playerCharacter ? g_playerCharacter->entity.node.rotation.y : 0;
    std::int32_t probeYaw = 0;
    std::int32_t probeBase = 0;
    std::int32_t probeMode = 0;
    switch (g_playerMovementState) {
    case PlayerMovementState::TurnAccumRight:
    case PlayerMovementState::TurnAccumLeft:
    case PlayerMovementState::TurnHoldPreCommit:
    case PlayerMovementState::ResolveBlockedA:
    case PlayerMovementState::ResolveBlockedB:
    case PlayerMovementState::TurnHoldAlt: {
        const std::int32_t forcedYaw = g_pControlPadController->forcedYaw;
        probeYaw = static_cast<std::uint16_t>(forcedYaw + rootYaw - PlayerChainYaw());
        probeBase = static_cast<std::int16_t>(forcedYaw);
        probeMode = g_pControlPadController->yawOffset < 0 ? 2 : 1;
        break;
    }
    default:
        probeYaw = static_cast<std::uint16_t>(rootYaw + g_currentPlayerYaw - PlayerChainYaw());
        probeBase = Fold16(g_currentPlayerYaw + 0x8000);
        break;
    }
    const float speed = ryoComputeSpeed01FromYawAndClearance(local, probeMode, probeYaw, probeBase);
    g_ryoRawMovementInputSpeed = speed;
    if (g_navSpeedClampMaximum >= 0.0F && g_navSpeedClampMaximum <= speed)
        g_ryoRawMovementInputSpeed = g_navSpeedClampMaximum;
    ryoUpdateSpeedAndRemainingCount();
}

// CEWP_MathFuncs_0: segment clearance in the player's parent space plus the step-height test.
bool CEWP_SegmentBlocked(const Vector3 &start, const Vector3 &end) {
    Vector3 localStart = start;
    Vector3 localEnd = end;
    if (g_playerRootNode->child) {
        localStart = WorldToEntityLocalPoint(*g_playerRootNode->child, localStart);
        localEnd = WorldToEntityLocalPoint(*g_playerRootNode->child, localEnd);
    }
    if (CEWP_TestSegment(localStart, localEnd, CurrentCycleMask()))
        return true;
    const float ground = mapUpdateIntersectionStateAtLocalPoint(end);
    return static_cast<double>(ground) >= static_cast<double>(start.y) + 0.5;
}

// CEWP_MathFuncs: classifies side clearance around the forward probe and returns the lateral snap yaw.
std::int32_t ComputeLateralClearanceYaw(PlayerControlWork &control, float maxSpeed, std::int32_t yaw) {
    if (maxSpeed >= 10.0F || g_navStateCurrent != 1) {
        control.currentFrameCount = 0;
        return 0;
    }
    const float width = PlayerCollisionRadius() - 0.0099999998F;
    const std::int32_t direction = std::abs(g_navDirectionFlags);
    const float sine = sin_fixed16(static_cast<std::int16_t>(direction));
    float forward = 0.0F;
    if (!(FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(sine))))) {
        const float cosine = cos_fixed16(static_cast<std::uint16_t>(direction));
        forward = ((1.0F - cosine) * 0.40000001F) / sine;
    }
    const Vector3 origin = PlayerWorldPosition();
    const Vector3 left = ApplyRotatedBackwardDisplacement(origin, yaw + 0x4000, width);
    const Vector3 right = ApplyRotatedBackwardDisplacement(origin, yaw - 0x4000, width);
    const Vector3 ahead = ApplyRotatedBackwardDisplacement(g_forwardProbePosition, yaw, forward);
    const Vector3 aheadLeft = ApplyRotatedBackwardDisplacement(ahead, yaw + 0x4000, width);
    const Vector3 aheadRight = ApplyRotatedBackwardDisplacement(ahead, yaw - 0x4000, width);
    const std::int32_t positiveYaw = yaw + 12743;
    const std::int32_t negativeYaw = yaw - 12743;
    const auto test = [&](float reach, bool &positive, bool &negative) {
        const Vector3 leftPositive = ApplyRotatedBackwardDisplacement(aheadLeft, positiveYaw, reach);
        const Vector3 rightPositive = ApplyRotatedBackwardDisplacement(aheadRight, positiveYaw, reach);
        const Vector3 leftNegative = ApplyRotatedBackwardDisplacement(aheadLeft, negativeYaw, reach);
        const Vector3 rightNegative = ApplyRotatedBackwardDisplacement(aheadRight, negativeYaw, reach);
        positive = CEWP_SegmentBlocked(left, leftPositive) || CEWP_SegmentBlocked(right, rightPositive);
        negative = CEWP_SegmentBlocked(left, leftNegative) || CEWP_SegmentBlocked(right, rightNegative);
    };
    bool positive = false;
    bool negative = false;
    test(2.0F, positive, negative);
    if (positive == negative)
        test(positive ? 0.5F : 4.0F, positive, negative);
    std::int32_t result = 0;
    if (positive && negative) {
        control.currentFrameCount = 1;
    } else if (positive) {
        control.currentFrameCount = 3;
        result = -10922;
    } else if (negative) {
        control.currentFrameCount = 2;
        result = 10922;
    } else {
        control.currentFrameCount = 0;
    }
    control.idleTurnCounter = result;
    return result;
}

std::int32_t ryoComputeTurnAngleAdjust(PlayerControlWork &control, std::int32_t yaw, bool firstGear) {
    const float threshold = firstGear ? 1.0F : 2.0F;
    const std::int32_t direction = g_navDirectionFlags;
    const std::int32_t lateral = ComputeLateralClearanceYaw(control, g_navProbeSpeed, g_navProbeBaseYaw);
    const float maxSpeed = g_navProbeSpeed;
    std::int32_t adjust = lateral;
    float reach = 0.0F;
    bool lateralSnap = false;
    if ((static_cast<std::uint32_t>(direction - 1) <= 0x31c5U && lateral > 0) || (static_cast<std::uint32_t>(direction + 0x31c6) <= 0x31c5U && lateral < 0) ||
        lateral == 0) {
        reach = g_navLateralDistance;
        adjust = direction;
    } else {
        lateralSnap = true;
        reach = sin_fixed16(0x31c7) * maxSpeed;
    }
    if (adjust == 0) {
        control.lateralSnapYaw = direction;
    } else {
        control.lateralSnapYaw = adjust;
        if ((adjust > 0 && control.previousAutoRotateYawDelta > 0) || (adjust < 0 && control.previousAutoRotateYawDelta < 0))
            adjust = 0;
    }
    const std::int32_t target = g_navProbeBaseYaw + adjust;
    if (std::abs(adjust) > 0x31c7 && std::abs(Fold16(target - control.secondaryYawOffset)) > 0x31c7) {
        control.turnAdjustSmoothed = 0;
        control.currentFrameCount = 1;
        return 0;
    }
    if (reach >= threshold || maxSpeed >= 5.0F) {
        control.turnAdjustSmoothed = 0;
        control.currentFrameCount = 1;
        return 0;
    }
    control.currentFrameCount = static_cast<std::int8_t>(adjust < 0 ? 3 : 2);
    const float motionSpeed = ryoUpdateCachedStates();
    if (FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(motionSpeed)))) {
        control.turnAdjustSmoothed = 0;
        return 0;
    }
    float scale = 1.0F;
    const float ratio = maxSpeed / motionSpeed;
    if (ratio > 0.0F) {
        scale = 1.0F / ratio;
        if (!(1.0F > scale))
            scale = 1.0F;
    }
    std::int32_t result = static_cast<std::int32_t>(static_cast<float>(Fold16(target - yaw)) * scale);
    if (lateralSnap) {
        const std::int32_t delta = Fold16(result - control.turnAdjustSmoothed);
        result = Fold16(control.turnAdjustSmoothed - static_cast<std::int32_t>(static_cast<double>(delta) * -0.2));
        control.turnAdjustSmoothed = result;
    }
    control.speedScale = 0.0F;
    if (lateralSnap)
        return result;
    const float clearance = ryoRecomputeYawAccum(cewpGetSpeedFor(g_playerCharacter), result + yaw);
    control.speedScale = clearance;
    return maxSpeed > clearance ? 0 : result;
}

std::int32_t ryoGetYaw(const PlayerControlWork &control, std::int32_t yaw) {
    const float lateral = static_cast<float>(-control.previousAutoRotateYawDelta);
    const float forward = static_cast<float>(control.inputYawDelta8);
    if (std::fabs(forward) < 0.001F && std::fabs(lateral) < 0.001F)
        return static_cast<std::int16_t>(yaw);
    return static_cast<std::int16_t>(yaw + Atan2Angle(forward, lateral) + 0x4000);
}

void ryoGetYaw_0(Character *character, PlayerControlWork &control) {
    std::int32_t mode = 0;
    const std::int32_t state = g_ryoCurrentAnimationState;
    if (static_cast<std::uint32_t>(g_navigationAnimationState - 1) <= 3U || g_playerMovementState == PlayerMovementState::SnapFailFallback ||
        g_currentTurnMode == TurnMode::Mode14 || (g_playerMovementState == PlayerMovementState::Idle && state == kStateWalk) ||
        static_cast<std::uint32_t>(state - kStateRun) <= 1U || state == kStateWalkBlend) {
        mode = 1;
    } else if (g_playerMovementState == PlayerMovementState::TurnHoldPreCommit) {
        mode = 4;
    } else if (control.previousAutoRotateYawDelta != 0 || control.inputYawDelta8 != 0) {
        mode = 0;
    } else {
        mode = g_playerMotion->previousAnimId == 0xffffU ? 3 : 2;
    }
    std::int32_t yaw = 0;
    switch (mode) {
    case 0:
        yaw = ryoGetYaw(control, -static_cast<std::int32_t>(control.cameraAccumulatedYaw));
        break;
    case 1:
        yaw = g_cameraAccumulatedYawSample;
        break;
    case 2:
        yaw = g_playerMotion->extractedYaw;
        break;
    case 3:
        yaw = character ? NodeChainYaw(&character->entity.node) : 0;
        break;
    default:
        yaw = Fold16((character ? NodeChainYaw(&character->entity.node) : 0) + control.yawOffset);
        break;
    }
    g_cameraAccumulatedYawSample = yaw;
    control.secondaryYawOffset = yaw;
}

void ryoUpdateControllerInputPhase1(Character *, PlayerControlWork &control) {
    control.navProbeYawLastStable = control.forcedYaw;
    Character *const ryo = CurrentRyo();
    MotionInformationWork *const motion = g_playerMotion;
    control.turnAngleAdjustment = 0;
    const std::int32_t chainYaw = ryo ? NodeChainYaw(&ryo->entity.node) : 0;
    control.idleTurnActive = control.idleTurnActive & ~0xff;
    switch (g_playerMovementState) {
    case PlayerMovementState::TurnAccumRight:
    case PlayerMovementState::TurnAccumLeft:
    case PlayerMovementState::TurnHoldPreCommit:
    case PlayerMovementState::ResolveBlockedA:
    case PlayerMovementState::ResolveBlockedB:
    case PlayerMovementState::TurnHoldAlt:
        control.hasNavProbeYawTarget = 1;
        control.forcedYaw = Fold16(chainYaw + control.yawOffset);
        control.turnVelocity = 0;
        return;
    default:
        break;
    }

    const auto setForced = [&](std::int32_t yaw) {
        control.forcedYaw = Fold16(yaw);
        control.hasNavProbeYawTarget = 1;
        control.turnVelocity = 0;
    };
    switch (g_ryoCurrentAnimationState) {
    case kStateIdle:
    case kStateIdleAlt: {
        const std::int32_t animation = motion->currentAnimId;
        std::uint32_t queued = 0;
        std::memcpy(&queued, MOTM_GetWork(ryo)->cue20Values + 4, sizeof(queued));
        if (queued != 0 || static_cast<std::uint32_t>(animation - 1307) <= 3U) {
            control.forcedYaw = motion->extractedYaw;
            return;
        }
        if (control.autoRotateYawDelta != 0)
            return;
        setForced(chainYaw + control.yawOffset);
        return;
    }
    case kStateWalk:
    case kStateWalkBlend:
    case kStateRun:
    case kStateRunBlend:
        if ((g_navigationAnimationState == 6 && control.inputYawDelta > 0 && control.autoRotateYawDelta == 0) ||
            ((static_cast<std::int32_t>(g_currentTurnMode) - 14) & ~2) == 0) {
            control.hasNavProbeYawTarget = 1;
            control.turnVelocity = 0;
            return;
        }
        break;
    case kStateWalkTurn180:
        control.forcedYaw = motion->extractedYaw;
        control.hasNavProbeYawTarget = 0;
        control.turnVelocity = 0;
        return;
    case kStateFight:
        if (!cpctGetFlagSetActive() || motion->currentAnimId == kAnimTurn180WadL || motion->currentAnimId == kAnimTurn180WadR) {
            control.forcedYaw = motion->extractedYaw;
            control.hasNavProbeYawTarget = 0;
            control.turnVelocity = 0;
            return;
        }
        setForced(chainYaw + control.yawOffset);
        return;
    default:
        break;
    }

    control.currentFrameCount = 0;
    control.idleTurnCounter = 0;
    if (static_cast<std::uint32_t>(g_navigationAnimationState - 1) <= 3U ||
        (control.previousAutoRotateYawDelta == 0 && control.inputYawDelta8 <= 0 && control.idleTurnMode == 0)) {
        const std::int32_t adjust = ryoComputeTurnAngleAdjust(control, chainYaw, control.numActiveFrames < 120U);
        control.turnAngleAdjustment = adjust;
        setForced(adjust + chainYaw);
        return;
    }

    std::int32_t adjust = ryoComputeTurnAngleAdjust(control, chainYaw, true);
    std::int32_t stickYaw = Fold16(ryoGetYaw(control, -static_cast<std::int32_t>(control.cameraAccumulatedYaw)) - chainYaw);
    if (control.previousAutoRotateYawDelta < 0) {
        if (stickYaw < 0)
            stickYaw += 0x10000;
    } else if (control.previousAutoRotateYawDelta > 0 && stickYaw > 0) {
        stickYaw -= 0x10000;
    }

    std::int32_t velocity = 0;
    if ((control.idleTurnMode == 0 && control.hasNavProbeYawTarget == 0) || cpctGetFlagSetActive()) {
        adjust = 0;
    } else {
        float step = 0.0F;
        float maximum = 0.0F;
        float bound = 0.0F;
        std::int32_t target = stickYaw;
        if (g_ryoIdleTurnInPlaceCounter < 10 && (control.autoRotateYawDelta != 0 || static_cast<std::uint8_t>(control.idleTurnMode - 1) <= 1U)) {
            step = 182.0F;
            maximum = 1274.0F;
            bound = 546.0F;
            control.idleTurnActive = (control.idleTurnActive & ~0xff) | 1;
            if (g_ryoIdleTurnInPlaceCounter == 0) {
                control.idleTurnIntensity = 0;
                control.idleTurnTimer = 0;
                control.idleTurnMode = static_cast<std::uint8_t>(control.autoRotateYawDelta >= 0 ? 2 : 1);
            }
            const std::uint8_t mode = control.idleTurnMode;
            if (mode != 0) {
                if (control.inputYawDelta < 0)
                    control.idleTurnIntensity = 1;
                else if (control.inputYawDelta > 0)
                    control.idleTurnIntensity = 2;
                target = 5461;
                if (control.idleTurnIntensity == 1) {
                    target = 1820;
                    maximum = 546.0F;
                } else if (control.idleTurnIntensity == 2) {
                    target = 8192;
                    maximum = 2548.0F;
                }
                if (mode == 1)
                    target = 5461;
                else if (mode == 2)
                    target = -5461;
            }
        } else {
            control.idleTurnActive = (control.idleTurnActive & ~0xff) | 1;
            const std::int32_t absoluteStick = std::abs(stickYaw);
            if (motion->interpolationAmount > 0.5F) {
                step = 43.0F;
                maximum = absoluteStick < 0x3555 ? 291.0F : 728.0F;
            } else {
                step = 145.0F;
                maximum = absoluteStick < 0x2000 ? 262.0F : 728.0F;
            }
            if (std::abs(g_navDirectionFlags) >= 5461 && 0.5F >= g_navLateralDistance) {
                float ramp = 0.5F - (g_navLateralDistance - 0.1F);
                ramp += ramp;
                maximum += (1.0F < ramp ? 1.0F : ramp) * (1456.0F - maximum);
            }
            bound = 145.0F;
        }
        const float targetF = static_cast<float>(target);
        if (target < 0) {
            float value = -bound;
            if (control.turnVelocity < 0 || !(value >= targetF)) {
                const float slowed = static_cast<float>(control.turnVelocity) - step;
                const float floor = -maximum > slowed ? -maximum : slowed;
                value = targetF > floor ? targetF : floor;
            }
            velocity = static_cast<std::int32_t>(value);
        } else {
            float value = bound;
            if (control.turnVelocity > 0 || !(targetF >= value)) {
                const float quickened = static_cast<float>(control.turnVelocity) + step;
                const float ceiling = maximum < quickened ? maximum : quickened;
                value = targetF < ceiling ? targetF : ceiling;
            }
            velocity = static_cast<std::int32_t>(value);
        }
    }
    control.hasNavProbeYawTarget = 1;
    control.turnVelocity = velocity;
    if (g_updateRenderFrameMode > 0)
        velocity = static_cast<std::int32_t>(static_cast<double>(static_cast<float>(velocity)) * 1.1875);
    else if (g_updateRenderFrameMode == 0)
        velocity = static_cast<std::int32_t>(static_cast<double>(static_cast<float>(velocity)) * 0.0625);
    control.turnAngleAdjustment = adjust;
    control.forcedYaw = Fold16(chainYaw + velocity + adjust);
}

void ryoUpdateControllerInputPhase2(Character *character, PlayerControlWork &control) {
    const auto state = static_cast<std::uint32_t>(g_playerMovementState);
    if (state <= 21U && (0x30c060U & (1U << state)) != 0) {
        control.navProbeYaw = Fold16((character ? NodeChainYaw(&character->entity.node) : 0) + control.yawOffset);
        control.yawDriftTimer = 9999;
        return;
    }
    if (control.turnAngleAdjustment == 0 && control.enableNavYawRecompute != 0) {
        const std::int16_t drift = control.yawDriftTimer;
        const std::int32_t animation = g_ryoCurrentAnimationState;
        if (drift > 30 || animation <= kStateStand || (animation > kStateWalkBlend && static_cast<std::uint32_t>(animation - kStateIdleAlt) > 2U) ||
            control.autoRotateYawDelta != 0 || control.inputYawDelta > 0) {
            control.navProbeYaw = control.navProbeYawLastStable;
        } else {
            const float clearance = ryoRecomputeYawAccum(cewpGetSpeedFor(g_playerCharacter), control.navProbeYaw);
            if (static_cast<double>(clearance) >= 1.3 && clearance >= g_navProbeSpeed) {
                control.forcedYaw = control.navProbeYaw;
                control.secondaryYawOffset = control.navProbeYaw;
                control.speedScale = clearance;
            }
            if (drift >= 10) {
                const std::int32_t delta = Fold16(control.forcedYaw - control.navProbeYaw);
                if (delta > 182)
                    control.navProbeYaw += 182;
                else if (delta < -182)
                    control.navProbeYaw -= 182;
                else
                    control.navProbeYaw = control.forcedYaw;
            }
        }
        control.yawBlendBackTimer = 0;
        if (drift < 9999)
            control.yawDriftTimer = static_cast<std::int16_t>(drift + 1);
        return;
    }
    const std::int16_t blendBack = control.yawBlendBackTimer;
    if (blendBack >= 10) {
        const std::int32_t delta = Fold16(control.forcedYaw - control.navProbeYaw);
        if (delta > 182)
            control.navProbeYaw += 182;
        else if (delta < -182)
            control.navProbeYaw -= 182;
        else
            control.navProbeYaw = control.forcedYaw;
    }
    control.yawDriftTimer = 0;
    if (blendBack < 9999)
        control.yawBlendBackTimer = static_cast<std::int16_t>(blendBack + 1);
}

void ryoUpdateControllerInputPhase3(Character *character, PlayerControlWork &control) {
    constexpr float epsilon = 0.00000011920929F;
    float acceleration = 0.0F;
    if (control.numActiveFrames >= 120U) {
        acceleration = static_cast<float>(static_cast<std::int32_t>(control.numActiveFrames) - 120) / 135.0F;
        if (std::fabs(acceleration) < epsilon)
            acceleration = 0.0F;
    }
    MotionManagerWork *const manager = MOTM_GetWork(character);
    std::uint32_t cueValue0 = 0;
    std::uint32_t cueValue1 = 0;
    std::memcpy(&cueValue0, manager->cue20Values, sizeof(cueValue0));
    std::memcpy(&cueValue1, manager->cue20Values + sizeof(cueValue0), sizeof(cueValue1));
    if (cueValue0 - 5U <= 1U) {
        cueValue0 = acceleration <= 0.5F ? 6U : 5U;
        std::memcpy(manager->cue20Values, &cueValue0, sizeof(cueValue0));
        std::memcpy(manager->cue20Values + sizeof(cueValue0), &cueValue1, sizeof(cueValue1));
    }
    const bool preserveIntensity =
        control.controlMode == 2 ? ((g_ryoCurrentAnimationState - kStateIdle) & ~4) == 0 : g_playerMotion->currentAnimId == kAnimWalkStart;
    if (!preserveIntensity) {
        control.turnIntensitySmoothed += (acceleration * 0.0078740157F - control.turnIntensitySmoothed) * 0.2F;
        if (std::fabs(control.turnIntensitySmoothed) < epsilon)
            control.turnIntensitySmoothed = 0.0F;
    }
    control.turnBurstAccum += std::max(-0.1F, acceleration * control.turnIntensitySmoothed - control.turnBurstAccum);
    if (control.turnBurstAccum < 0.0F)
        control.turnBurstAccum = 0.0F;
    if (control.previousAutoRotateYawDelta != 0 || control.inputYawDelta8 != 0) {
        if (control.turnBurstAccum > 0.9F)
            control.flags = static_cast<std::uint16_t>((control.flags & ~0x0008U) | 0x0001U);
        else
            control.flags = static_cast<std::uint16_t>((control.flags & ~0x0009U) | 0x0008U);
    } else {
        control.flags &= static_cast<std::uint16_t>(~0x0009U);
        if (std::fabs(acceleration) < epsilon) {
            control.turnIntensitySmoothed = 0.0F;
            control.turnBurstAccum = 0.0F;
        }
    }
    g_pControlPadController->startAnimationSpeed = acceleration;
    g_pControlPadController->speedAdjustWindow = 8.0F;
}

bool cpctValidateAndSetYawDeltaFromEntityPosition(PlayerControlWork &control) {
    const bool clear = ProbeFacingClearance(PlayerWorldPosition(), g_pControlPadController->secondaryYawOffset, 0.1F);
    control.facingProbeClear = clear ? 1 : 0;
    return clear;
}

bool cpctUpdateFacingFromNavAndYawDelta(PlayerControlWork &state, std::int32_t desiredYaw) {
    const bool probeClear = cpctValidateAndSetYawDeltaFromEntityPosition(state);
    const std::int32_t facingYaw = PlayerChainYaw();
    if ((state.heldInput & 0x0200U) != 0 || state.triggerHigh != 0) {
        state.newYaw = facingYaw;
        state.facingYawDelta = 0;
        state.enableNavYawRecompute = 0;
        return false;
    }
    const Vector3 position = PlayerWorldPosition();
    PlayerControlWork &control = *g_pControlPadController;
    std::int32_t maxSnapDelta = 1820;
    std::int32_t baseYaw = desiredYaw;
    std::int32_t forcedYaw = 0;
    bool compareBoth = false;
    if (cpctGetFlagSetActive() || g_turnInputLockoutFrames > 0) {
        switch (g_playerMovementState) {
        case PlayerMovementState::TurnAccumRight:
        case PlayerMovementState::TurnAccumLeft:
        case PlayerMovementState::TurnHoldPreCommit:
        case PlayerMovementState::ResolveBlockedA:
        case PlayerMovementState::ResolveBlockedB:
        case PlayerMovementState::TurnHoldAlt:
            forcedYaw = control.forcedYaw;
            break;
        default:
            maxSnapDelta = 12743;
            forcedYaw = !probeClear || (state.inputYawDelta > 0 && state.autoRotateYawDelta == 0) ? control.secondaryYawOffset : control.forcedYaw;
            break;
        }
    } else if (((g_ryoCurrentAnimationState - kStateWalk) & ~5) != 0 || g_ryoCurrentAnimationState == kStateRunBlend) {
        forcedYaw = control.secondaryYawOffset;
    } else {
        baseYaw = control.secondaryYawOffset;
        compareBoth = true;
        forcedYaw = control.forcedYaw;
    }

    std::int32_t snapYaw = static_cast<std::int16_t>(forcedYaw);
    bool found = navFindLegalFacingInSector(position, static_cast<std::int16_t>(forcedYaw + 0x8000), NavProbeSector::NarrowCandidateBias);
    if (found)
        snapYaw = static_cast<std::int16_t>(g_targetPlayerYawSnap);
    std::int32_t yaw = 0;
    if (compareBoth && found) {
        const std::int32_t spread = Fold16(baseYaw - forcedYaw);
        if (spread > 0x4000)
            baseYaw = forcedYaw + 0x4000;
        else if (spread < -0x4000)
            baseYaw = forcedYaw - 0x4000;
        found = navFindLegalFacingInSector(position, static_cast<std::int16_t>(baseYaw + 0x8000), NavProbeSector::NarrowCandidateBias);
    }
    if (found) {
        yaw = Fold16(snapYaw - facingYaw + 0x8000);
        if (!probeClear && cpctGetFlagSetActive() && std::abs(yaw) > maxSnapDelta && state.inputYawDelta <= 0)
            found = false;
    }
    if (!found) {
        const auto movement = static_cast<std::uint32_t>(g_playerMovementState);
        if ((movement > 21U || (0x204060U & (1U << movement)) == 0) && g_ryoCurrentAnimationState != kStateFight && g_navHasNoLegalFacing != 1 &&
            (!g_hasNavSnapBudget || !g_underMinimumSnapSpeed)) {
            const std::int8_t input = state.inputYawDelta8;
            if (input < 0 || (input <= 0 && state.numActiveFrames != 0)) {
                yaw = g_underMinimumSnapSpeed ? Fold16(g_navSuggestedYaw - facingYaw + 0x8000) : 0;
                found = true;
            }
        }
    }
    state.facingYawDelta = yaw;
    state.newYaw = Fold16(facingYaw + yaw);
    state.enableNavYawRecompute = found ? 1 : 0;
    return found;
}

bool ryoUpdateTatehizaStateAndControlLock(PlayerControlWork &control, const InputSlot *input) {
    MotionInformationWork *const motion = g_playerMotion;
    if (!ShouldRun()) {
        const PlayerControlWork *const ryoControl = PlayerControl();
        return ryoControl && ryoControl->tatehizaState != 0;
    }
    switch (control.tatehizaState) {
    case 0:
        return false;
    case 2:
        if ((motion->motionFlags & kMotionStuck) != 0) {
            motion->playbackRate = 1.0F;
            motion->motionFlags &= ~(kMotionStuck | kMotionFrameBlendReady);
            motion->pendingAnimId = kAnimTatehizaLoop;
            control.tatehizaState = 3;
        }
        return true;
    case 3:
        if ((input->held & 0xf204U) != 0 || control.numActiveFrames != 0) {
            motion->motionFlags &= ~(kMotionStuck | kMotionFrameBlendReady);
            motion->pendingAnimId = kAnimTatehizaExit;
            motion->playbackRate = 1.0F;
            control.tatehizaState = 4;
        }
        return true;
    case 4:
        if ((motion->motionFlags & kMotionStuck) == 0)
            return true;
        motion->trackedNodeIndex = 10;
        ryoCalcCurrentYaw();
        motion->motionFlags |= kMotionFrameBlendReady;
        control.tatehizaState = 0;
        g_forceYawRecalculation = 1;
        return false;
    default:
        return false;
    }
}

void MaskPlayerInput(InputSlot &input, std::uint16_t mask) {
    input.held &= mask;
    input.previousHeld &= mask;
    input.pressedPriority &= mask;
    input.repeatedPresses &= mask;
}

InputSlot *cpctRefreshInputFromPad(PlayerControlWork &control, PadCaptureEntry &pad) {
    control.previousInputYawPair = static_cast<std::int16_t>(
        static_cast<std::uint8_t>(control.previousAutoRotateYawDelta) | (static_cast<std::uint16_t>(static_cast<std::uint8_t>(control.inputYawDelta8)) << 8U));
    std::uint16_t directions = pad.held & kMovementInputMask;
    if (g_directionChordState == 1) {
        if ((g_directionChordMask & kInputLeft) != 0) {
            if ((pad.held & 0x6000U) != 0x6000U && (pad.held & 0x5000U) != 0x5000U)
                directions = kInputLeft;
        } else if ((g_directionChordMask & kInputRight) != 0) {
            if ((pad.held & 0xa000U) != 0xa000U && (pad.held & 0x9000U) != 0x9000U)
                directions = kInputRight;
        } else if ((g_directionChordMask & kInputDown) != 0 && (pad.held & 0x6000U) != 0x6000U && (pad.held & 0xa000U) != 0xa000U) {
            directions = kInputDown;
        }
        g_directionChordState = 2;
        g_directionChordMask = 0;
    } else if (g_directionChordState == 2) {
        if (directions == 0)
            g_directionChordState = 0;
    } else if ((pad.previousHeld & kMovementInputMask) == 0 && ((static_cast<std::uint16_t>(directions - kInputDown) & 0x9fffU) == 0) &&
        directions != 0x6000U) {
        g_directionChordMask = directions;
        directions &= kInputUp;
        g_directionChordState = 1;
    }
    pad.held = static_cast<std::uint16_t>(directions | (pad.held & 0x0fffU));
    const bool swapTriggers = g_applicationSettings.savedInput.swapTriggers != 0 && g_keyboardActive != 1;
    control.triggerHigh = static_cast<std::uint8_t>(pad.triggers >> (swapTriggers ? 0U : 8U));
    control.pressedPriorityDown = (pad.pressedPriority & kMovementInputMask) == kInputDown ? 1 : 0;
    std::uint32_t filterMode = 0;
    if (control.modeState != 0) {
        switch (static_cast<std::uint16_t>(control.currentMode)) {
        case 1:
        case 2:
        case 4:
        case 5:
            filterMode = 1;
            break;
        case 3:
            filterMode = 2;
            break;
        default:
            break;
        }
    }
    const MapControlData *const mapControl = CurrentMapControl();
    const bool ownsFrame = ShouldRun();
    const auto *const film = mapControl && mapControl->filmTask && mapControl->filmTask->task_data_ptr ?
        reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(mapControl->filmTask->task_data_ptr) + 4) :
        nullptr;
    if (ownsFrame && film && film->current[0] != 0) {
        filterMode = 1;
    } else if (control.busy != 0) {
        if (ownsFrame)
            control.busy = 0;
        if (filterMode == 0)
            filterMode = 1;
    }
    auto *input = reinterpret_cast<InputSlot *>(&pad.enabled);
    if (filterMode == 2) {
        g_filteredPlayerInput.previousHeld &= g_filteredPlayerInput.held;
        g_filteredPlayerInput.triggers &= swapTriggers ? 0xff00U : 0x00ffU;
        g_filteredPlayerInput.held &= swapTriggers ? 0x1040U : 0x1080U;
        g_filteredPlayerInput.pressedPriority = 0;
        g_filteredPlayerInput.repeatedPresses = 0;
        input = &g_filteredPlayerInput;
    } else {
        std::memcpy(&g_filteredPlayerInput, &pad.enabled, sizeof(InputSlot));
        if (filterMode == 1) {
            std::uint16_t mask = 0;
            const bool focused = shenHasFocusedNotLockedOn();
            if (control.controlGate >= 0 && ((g_cameraStateFlags & 8U) != 0 || focused)) {
                if (control.tatehizaState != 0) {
                    mask = 0x0204U;
                } else if ((pad.held & 0x0200U) != 0 || control.triggerHigh != 0) {
                    mask = focused ? 0x0200U : 0xf200U;
                    if (focused) {
                        control.yawOffset = 0;
                        control.committedTurnAccumYaw = 0;
                    }
                }
            }
            MaskPlayerInput(g_filteredPlayerInput, mask);
            g_filteredPlayerInput.triggers &= swapTriggers ? 0x00ffU : 0xff00U;
            input = &g_filteredPlayerInput;
        }
    }
    if (control.ticksHeld > 0) {
        --control.ticksHeld;
        input->triggers = 0;
        MaskPlayerInput(*input, 0x0606U);
    } else if (static_cast<std::int32_t>(control.ticks) <= 0) {
        MaskPlayerInput(*input, 0xf9f9U);
    }
    control.inputYawDelta = (input->held & kInputUp) != 0 ? -127 : (input->held & kInputDown) != 0 ? 127 : 0;
    control.autoRotateYawDelta = (input->held & kInputLeft) != 0 ? -127 : (input->held & kInputRight) != 0 ? 127 : 0;
    const auto rightX = static_cast<std::int8_t>(static_cast<std::uint8_t>(input->rightStick) + 0x80U);
    const auto rightY = static_cast<std::int8_t>(static_cast<std::uint8_t>(input->rightStick >> 8U) + 0x80U);
    if (g_playerLookRunSwapMode == 0) {
        control.previousAutoRotateYawDelta = static_cast<std::int8_t>(control.autoRotateYawDelta);
        control.inputYawDelta8 = static_cast<std::int8_t>(control.inputYawDelta);
        control.flags &= ~0x0200U;
        control.rightAngleX = rightX;
        control.rightAngleY = rightY;
        control.lookYawUsesMovementInput = 0;
        control.lookPitchUsesMovementInput = 0;
        if (((control.heldInput & 0x0200U) != 0 || control.triggerHigh != 0) && control.fightOnly == 0 && control.inputYawDelta != 0 &&
            control.lookRunMode == 1) {
            control.lookPitchUsesMovementInput = 1;
            control.rightAngleY = static_cast<std::int8_t>(control.inputYawDelta);
        }
    } else if (g_playerLookRunSwapMode == 1) {
        control.flags |= 0x0200U;
        control.previousAutoRotateYawDelta = rightX;
        control.inputYawDelta8 = rightY;
        control.rightAngleX = static_cast<std::int8_t>(control.autoRotateYawDelta);
        control.rightAngleY = static_cast<std::int8_t>(control.inputYawDelta);
        control.lookYawUsesMovementInput = 1;
        control.lookPitchUsesMovementInput = 1;
    }
    control.wasRunning = static_cast<std::int8_t>(control.isRunning);
    control.isRunning = control.numActiveFrames;
    control.numActiveFrames = static_cast<std::uint8_t>(input->triggers >> (swapTriggers ? 8U : 0U));
    if ((control.flags & kScriptAlternateMode) != 0) {
        const auto movement = static_cast<std::uint32_t>(g_playerMovementState);
        const bool neutral = control.controlMode == 2 ? (movement & ~static_cast<std::uint32_t>(PlayerMovementState::TurnExitFromForward)) == 0 :
                                                        control.previousAutoRotateYawDelta == 0 && control.inputYawDelta8 == 0;
        control.numActiveFrames = control.numActiveFrames == 0 && neutral ? 0 : 0xffU;
    } else if ((control.flags & kForceWalk) != 0) {
        control.numActiveFrames = std::min<std::uint8_t>(control.numActiveFrames, 119U);
    }
    control.pressedPriority = input->pressedPriority;
    control.heldInput = (control.heldInput & 0xffff0000U) | input->held;
    control.inputSource = input;
    return input;
}

void ryoUpdateControllerInputPhase0(Character *character, PlayerControlWork &control) {
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    if (CheckIsPointIntersectWith(position, nullptr, SHEN_NAME(WALK)))
        control.walkVolumeGraceFrames = 0;
    else if (control.walkVolumeGraceFrames < 9999)
        ++control.walkVolumeGraceFrames;
    control.flags &= ~(kForceWalk | kScriptAlternateMode);
    if (control.walkVolumeGraceFrames < 30)
        control.flags |= kForceWalk;
}

void UpdatePlayerInteractionTarget(Character *character, PlayerControlWork &control) {
    if ((control.pressedPriority & 0x0400U) == 0)
        return;
    (void)COLS_SignalBestInteractionTarget(character);
    control.flags |= 0x0040U;
}

void UpdateAuthorSearchControl(PlayerControlWork &control) {
    if (g_CameraOverrideActiveFlag == 0 ||
        (control.previousAutoRotateYawDelta == 0 && control.inputYawDelta8 == 0 && control.numActiveFrames == 0 && control.rightAngleX == 0 &&
            control.rightAngleY == 0 && (control.pressedPriority & 0x0200U) == 0)) {
        return;
    }
    g_bIsConversationCamInactive = 0;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 0;
    ApplyPendingGameState(0);
}

void ApplyDeferredPlayerTranslation(Character *character, PlayerControlWork &control) {
    if (!character || !g_playerMotion || control.applyDeferredTranslation == 0 || g_playerMotion->pendingAnimId != 0) {
        return;
    }
    std::int16_t localYaw = g_playerMotion->extractedYaw;
    for (CharacterNode *node = character->entity.node.child; node; node = node->child) {
        localYaw = static_cast<std::int16_t>(localYaw - static_cast<std::int16_t>(node->rotation.y));
    }
    character->entity.node.rotation.y = localYaw;
    Vector3 position = GetPositionForEntity(character->entity.node);
    position.x = g_playerMotion->rootWorldPosition.x;
    position.z = g_playerMotion->rootWorldPosition.z;
    shenApplyTranslationToAll(character->entity.node, position);
    control.applyDeferredTranslation = 0;
}
void UpdateLoadHoldInputLatch(PlayerControlWork &control) {
    if (control.loadHoldActive == 0)
        return;

    const std::int32_t frames = ++g_cpctLoadHoldFrameCount;
    if (frames >= 5 && (g_perFrameNpcWorkCnt == 0 || frames >= 90)) {
        control.loadHoldActive = 0;
        g_cpctLoadHoldInputCleared = false;
        return;
    }

    g_cpctLoadHoldCollisionLatch = 1;
    PadCaptureEntry *const pad = CurrentMapPad();
    if (!pad || (pad->flags & 2U) == 0) {
        g_cpctLoadHoldInputCleared = false;
        return;
    }
    if (!g_cpctLoadHoldInputCleared) {
        InputSlot &input = g_inputState.slots[0];
        input.held = 0;
        input.previousHeld = 0;
        input.pressedPriority = 0;
        input.repeatedPresses = 0;
        input.rightStick = 0x8080U;
        input.leftStick &= 0xffff0000U;
        input.triggers = 0;

        pad->held = 0;
        pad->previousHeld = 0;
        pad->pressedPriority = 0;
        pad->repeatedPresses = 0;
        pad->rightStick = 0x8080U;
        pad->leftStick &= 0xffff0000U;
        pad->triggers = 0;
    }
    g_moduleInputSuppressFrames = 1;
    g_cpctLoadHoldInputCleared = true;
}

const MotionNodeRuntime *FindPreviousMotionNode(const MotionInformationWork &motion, std::uint8_t nodeId) {
    const auto *rows = static_cast<const MotionNodeRuntime *>(motion.previousNodes);
    if (!rows)
        return nullptr;
    for (; rows->nodeId != nodeId; ++rows) {
        if (rows->nodeId == 0xffU)
            return nullptr;
    }
    return rows;
}

bool ryoCanAcceptMovementInput() {
    if ((g_cameraStateFlags & 8U) != 0 || g_searchCandidateId >= 0)
        return false;
    if (((g_currentSearchState - 2) & ~3) == 0 && g_currentSearchState != static_cast<std::int32_t>(SearchState::AutoCharacterLockOn))
        return false;
    if (g_searchTemporaryFocusedCharacter != 0 || camCtrlGetState() > 0)
        return false;
    if ((CurrentMapPad()->flags & 2U) == 0)
        return false;
    if (g_ryoFootPlantingApplied && static_cast<double>(std::fabs(g_ryoFootPositions.y - g_ryoFootPositions.x)) > 0.02)
        return false;
    const auto movement = static_cast<std::uint32_t>(g_playerMovementState);
    return movement > 21U || (0x204060U & (1U << movement)) == 0;
}

float ryoUpdateCachedStates() {
    if (!g_playerMotion)
        return 0.0F;
    const std::int32_t state = g_ryoCurrentAnimationState;
    float rate = 0.0F;
    if (state != kStateStand) {
        if (((state - kStateWalkBlend) & ~4) == 0 && state != g_previousPlayerAnimationState) {
            rate = g_playerAnimationRateScale;
        } else {
            const Vector3 &delta = g_playerMotion->rootMotionDelta;
            rate = std::sqrt(delta.x * delta.x + delta.z * delta.z);
            if (0.003 > static_cast<double>(rate))
                rate = 0.0F;
        }
    } else {
        const auto movement = static_cast<std::int32_t>(g_playerMovementState);
        if (movement >= 3 && (movement <= 4 || static_cast<std::uint32_t>(movement - 19) <= 1U))
            rate = 0.0099999998F;
    }
    g_previousPlayerAnimationState = state;
    g_playerAnimationRateScale = rate;
    return rate;
}

void ConfigureBlendAnimation(std::size_t slot, std::int32_t animation, float blendRate, float frameOffset) {
    AnimBlendEntry &entry = g_animBlendTable[slot];
    entry.animationId = animation;
    const std::int32_t frameCount = MOTN_GetFrameCountFromAnimId(static_cast<std::uint16_t>(animation));
    entry.blendRate = blendRate;
    g_motnHasYValue = 0;
    const auto total = static_cast<float>(static_cast<double>(static_cast<float>(frameCount)) - 1.0);
    entry.frames = total;
    Vector3 root{};
    MOTN_ReadRootPositionAtFrame(static_cast<std::uint16_t>(animation), static_cast<std::uint16_t>(g_playerMotion->basePoseTemplateIndex), 0.0F, root, false);
    entry.rootMotionZ = -root.z;
    entry.frameOffset = frameOffset;
    const auto window = [&](double fraction) {
        float value = static_cast<float>(static_cast<double>(total) * fraction + static_cast<double>(frameOffset));
        if (value >= total)
            value -= total;
        return value;
    };
    entry.startFrame = window(0.8);
    entry.blendStartLeft = window(0.536);
    entry.blendStartRight = window(0.036);
    entry.blendEndThreshold = window(0.22222222);
    entry.end2 = entry.blendEndThreshold;
    entry.end3 = window(0.77777777);
    entry.endBlendCutoff = window(0.5);
    entry.endFrame = window(1.0);
}

void SetupRyoBlendAnimations(std::int32_t primary, std::int32_t secondary) {
    const std::int32_t animation = static_cast<std::int16_t>(primary);
    float frameOffset = 0.0F;
    if (animation == kAnimWalkFastKLoop)
        frameOffset = 30.0F;
    else if (animation == kAnimWalkSlowKLoop)
        frameOffset = 23.0F;
    ConfigureBlendAnimation(0, animation, 1.2857143F, frameOffset);
    ConfigureBlendAnimation(1, kAnimSlowRunLoop, 1.0F, 12.0F);
    ConfigureBlendAnimation(2, static_cast<std::int16_t>(secondary), 1.2F, 15.0F);
    ConfigureBlendAnimation(3, kAnimStairsUpLoop, 1.2F, 17.0F);
    ConfigureBlendAnimation(4, kAnimStairsDownLoop, 1.2F, 24.0F);
    ConfigureBlendAnimation(5, kAnimStairsUp2StepLoop, 1.2F, 8.0F);
    ConfigureBlendAnimation(6, kAnimStairsDown2StepLoop, 1.2F, 16.0F);
    g_currentAnimBlendIndex = 0;
}

float motComputeBlendedAnimationFrame(const LocomotionAnimRequest &request, const MotionInformationWork &motion) {
    const AnimBlendEntry &source = g_animBlendTable[static_cast<std::size_t>(request.blendSlot)];
    float elapsed = request.frame - source.frameOffset;
    if (elapsed < 0.0F)
        elapsed += source.frames;
    float fraction = elapsed / source.frames;
    if ((motion.motionFlags & kMotionForceBlendOut) != 0) {
        fraction += 0.5F;
        if (fraction >= 1.0F)
            fraction -= 1.0F;
    }
    const AnimBlendEntry &target = g_animBlendTable[static_cast<std::size_t>(g_currentAnimBlendIndex)];
    return fraction * target.frames + target.frameOffset;
}

bool ShouldTransitionToIdleState(LocomotionAnimRequest &request) {
    const std::int32_t animation = request.animationId;
    const std::int32_t state = request.animationState;
    const std::int32_t resolved = request.resolvedAnimationId;
    const double frame = request.frame;
    const MotionInformationWork &motion = *g_playerMotion;
    if (motiCheckLocomotionState(&motion))
        return true;
    const std::size_t slot = static_cast<std::size_t>(g_currentAnimBlendIndex);
    const AnimBlendEntry &entry = g_animBlendTable[slot];
    const bool forceBlendOut = (motion.motionFlags & kMotionForceBlendOut) != 0;
    if (animation != entry.animationId) {
        if ((motion.motionFlags & kMotionStuck) != 0)
            return false;
        if (state == kStateWalkBlend)
            return true;
        if (state == kStateIdle) {
            const double frames = request.frameCount;
            if (resolved == -2) {
                if (frames - 6.0 > frame)
                    return true;
            } else if (resolved == kAnimStandLoop) {
                if (animation == kAnimWalkStart)
                    return frames - 6.0 > frame;
                if (frames - 3.0 > frame)
                    return true;
            }
        } else if (state == kStateIdleAlt) {
            if (resolved == -2 || resolved == kAnimStandLoop)
                return true;
        } else if (state == kStateStand) {
            if (resolved == kAnimStandLoop)
                return true;
        }
        return animation == kAnimWalkTurn180L || state == kStateFight;
    }
    if (g_playerMovementState == PlayerMovementState::Idle)
        return false;
    if (state == kStateIdle)
        return true;
    if (slot == 2 && ((state - kStateWalk) & ~4) == 0) {
        if (forceBlendOut)
            request.blendOutLatched = 1;
        return false;
    }
    const double rate = static_cast<double>(entry.blendRate) * 3.0;
    const double cutoff = entry.endBlendCutoff;
    bool checkTurn = false;
    if (frame < cutoff - rate || cutoff + static_cast<double>(entry.blendRate) < frame || g_navigationAnimationState == 6) {
        const double end = entry.endFrame;
        if (frame < end - rate || end + static_cast<double>(entry.blendRate) < frame)
            return true;
        if (slot == 0 ? forceBlendOut : (slot - 1U <= 5U && !forceBlendOut))
            request.blendOutLatched = 1;
        if (resolved != kAnimWalkTurn180L || g_navigationAnimationState == 6)
            return false;
        checkTurn = true;
    } else {
        if (slot == 0 ? !forceBlendOut : (slot - 1U <= 5U && forceBlendOut))
            request.blendOutLatched = 1;
        if (resolved != kAnimWalkTurn180L)
            return false;
        checkTurn = true;
    }
    if (!checkTurn)
        return false;
    const PlayerControlWork *const control = g_pControlPadController;
    if (request.blendOutLatched)
        return control && control->autoRotateYawDelta > 0;
    return control && control->autoRotateYawDelta < 0;
}

std::int32_t UpdateRyoIdleAnimationWithRootMotion(PlayerControlWork &control, std::int32_t &blendOut) {
    const bool canAccept = ryoCanAcceptMovementInput();
    MotionInformationWork &motion = *g_playerMotion;
    const std::int32_t current = motion.currentAnimId;
    blendOut = 0;
    std::int32_t animation = kAnimIdleRoot;
    switch (current) {
    case kAnimStandLoop:
        animation = kAnimStandLoop;
        break;
    case kAnimIdleRoot:
        if ((motion.motionFlags & kMotionForceBlendOut) != 0)
            blendOut = 1;
        animation = canAccept && control.idleLoopFrames >= control.idleLoopTarget ? kAnimIdleRootReturn : kAnimIdleRoot;
        break;
    case kAnimIdleRootReturn:
        if ((motion.motionFlags & kMotionForceBlendOut) == 0)
            blendOut = 1;
        animation = kAnimIdleRoot;
        break;
    default:
        TryStartTurnInPlaceAnimation();
        if (motion.currentAnimId == 0)
            animation = kAnimIdleRoot;
        else if (g_ryoCurrentAnimationState == kStateFightFallback || g_ryoCurrentAnimationState == kStateIdleRootVariant)
            animation = kAnimStandLoop;
        break;
    }
    if (canAccept && current == animation) {
        ++control.idleLoopFrames;
    } else {
        control.idleLoopFrames = 1;
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        const auto random = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
        control.idleLoopTarget = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(random) * 4.0) + 2);
    }
    return animation;
}

std::int32_t yawDeltaToStrafeStepSectors(std::int32_t yaw) {
    return static_cast<std::int32_t>(((static_cast<std::uint32_t>(static_cast<std::uint16_t>(yaw + 0x1000)) >> 8U) & 0xffU) >> 5U) + 9;
}

std::int32_t yawDeltaToTurnSector(std::int32_t yaw) {
    if (std::abs(yaw) <= 910)
        return 20;
    const std::uint32_t octant = ((static_cast<std::uint32_t>(static_cast<std::uint16_t>(yaw + 0x1000)) >> 8U) & 0xffU) >> 5U;
    if (octant == 0)
        return yaw > 0 ? 21 : 27;
    return static_cast<std::int32_t>(octant) + 20;
}

std::uint32_t YawOctant(std::int32_t yaw) {
    return ((static_cast<std::uint32_t>(static_cast<std::uint16_t>(yaw + 0x1000)) >> 8U) & 0xffU) >> 5U;
}

const ResolvedLocomotion &ryoResolveTurnAnimFromRequestedYaw(YawResolveMode mode, std::int32_t requestedYaw) {
    PlayerControlWork &control = *g_pControlPadController;
    const std::int32_t delta = static_cast<std::int16_t>(requestedYaw - PlayerChainYaw());
    std::int32_t yaw = delta;
    std::int32_t sector = 0;
    bool walkLoop = false;
    switch (mode) {
    case YawResolveMode::OctantSnapWalk: {
        const std::uint32_t octant = YawOctant(delta);
        yaw = kOctantCardinalSnapOffset[octant] + delta;
        sector = kOctantTurnBucketWalk[octant];
        break;
    }
    case YawResolveMode::TurnSectorDelta:
        sector = yawDeltaToTurnSector(delta);
        break;
    case YawResolveMode::Skeleton:
        if (YawOctant(delta) != 4)
            walkLoop = true;
        else
            sector = 17;
        break;
    case YawResolveMode::CharacterWorldOrBlocked:
        if (YawOctant(delta) != 4)
            walkLoop = true;
        else
            sector = 28;
        break;
    case YawResolveMode::SkeletonPlusOffset:
        sector = kOctantSectorIndexDefault[YawOctant(delta)];
        break;
    case YawResolveMode::KeepSmoothed:
        sector = kYawSliceTurnBucket[static_cast<std::uint16_t>(delta) >> 12U];
        break;
    case YawResolveMode::Forced: {
        const std::uint32_t octant = YawOctant(delta);
        yaw = kOctantCardinalSnapOffset[octant] + delta;
        sector = kOctantTurnBucketRun[octant];
        break;
    }
    case YawResolveMode::StrafeStepSector:
        sector = yawDeltaToStrafeStepSectors(delta);
        break;
    }
    if (walkLoop || sector == 8) {
        g_resolvedLocomotion.animationId = kAnimWalkSp1Loop;
        g_resolvedLocomotion.chosenYawOffset = 0;
    } else if (sector == -2) {
        g_resolvedLocomotion.animationId = kAnimTurn022WadR;
        g_resolvedLocomotion.chosenYawOffset = -4096;
    } else if (sector == -1) {
        g_resolvedLocomotion.animationId = kAnimTurn022WadL;
        g_resolvedLocomotion.chosenYawOffset = 4096;
    } else {
        g_resolvedLocomotion.chosenYawOffset = kSectorYawOffsets[static_cast<std::size_t>(sector)];
        g_resolvedLocomotion.animationId = static_cast<std::int32_t>(control.motionMetadata[static_cast<std::size_t>(sector)].animationId);
    }
    control.desiredFacingYaw = static_cast<std::uint32_t>(Fold16(PlayerChainYaw() + yaw));
    g_resolvedLocomotion.deltaYaw = yaw;
    g_resolvedLocomotion.residualYaw = Fold16(yaw - g_resolvedLocomotion.chosenYawOffset);
    return g_resolvedLocomotion;
}

std::int32_t ryoResolveYawDrivenAnim(LocomotionAnimRequest &request, YawResolveMode mode, std::int32_t yaw) {
    const ResolvedLocomotion &resolved = ryoResolveTurnAnimFromRequestedYaw(mode, yaw);
    request.turnBlendParam = resolved.residualYaw;
    std::int32_t animation = resolved.animationId;
    if (animation == kAnimWalkSp2Loop)
        animation = -2;
    return animation == kAnimWalkStart ? -2 : animation;
}

std::int32_t ryoResolveNextLocomotionAnim(LocomotionAnimRequest &request, std::int32_t animation, std::int32_t rateMode, std::int32_t resolved) {
    PlayerControlWork *const control = g_pControlPadController;
    Character *const ryo = g_playerCharacter;
    const bool holdingStop = (!control || control->inputYawDelta <= 0) && g_navStateCurrent == 2;
    const PlayerMovementState movement = g_playerMovementState;
    const auto movementBits = static_cast<std::uint32_t>(movement);
    std::int32_t newYaw = 0;
    if (movementBits <= 20U && (0x10c060U & (1U << movementBits)) != 0) {
        const PlayerControlWork *const ryoControl = PlayerControl();
        newYaw = ryoControl ? ryoControl->newYaw : 0;
    } else {
        newYaw = control->secondaryYawOffset;
    }
    request.blendFrame = 0.0F;
    std::int32_t desiredYaw = static_cast<std::int16_t>(newYaw + 0x8000);
    const float speed = g_ryoRawMovementInputSpeed;
    const PlayerControlWork *const ryoControl = PlayerControl();
    bool lookupRule = (ryoControl && static_cast<std::int8_t>(ryoControl->enableNavYawRecompute) != 0) || request.requestAnimationId != 0xffff ||
        g_turnInputLockoutFrames > 0;
    bool resolveTurn = false;
    bool afterRate = false;
    if (lookupRule) {
        for (const IdleAnimationRule &rule : kIdleAnimationRules) {
            if (rule.rateMode == rateMode && rule.requestAnimationId == request.requestAnimationId) {
                resolved = rule.animationId;
                request.blendFrame = rule.blendFrame;
                break;
            }
        }
    } else if (rateMode == 2 || rateMode == 6) {
        if ((rateMode == 2 ? 0.25F : 0.73000002F) >= speed)
            resolved = kAnimStandLoop;
        afterRate = true;
    } else if (rateMode == 0 || rateMode == 10) {
        if (!(0.1F < speed)) {
            if (rateMode == 10) {
                resolved = kAnimStandLoop;
                afterRate = true;
            } else if (cpctGetFlagSetActive() || (control && control->movementInputActive != 0 && g_ryoIdleResetCounter < 5)) {
                resolved = -3;
            } else {
                resolved = ryoResolveYawDrivenAnim(request, YawResolveMode::SkeletonPlusOffset, static_cast<std::int16_t>(desiredYaw + 0x8000));
                control->fightOnly = 1;
                if (resolved == -2)
                    resolved = -3;
            }
        } else if (!(0.34999999F < speed) && rateMode == 0) {
            resolved = -2;
            resolveTurn = true;
        }
    }

    if (!resolveTurn) {
        const bool turnRoute = afterRate ? false : (rateMode == 11 || rateMode == 0);
        if (!turnRoute && animation != kAnimTurn022WadL && animation != kAnimTurn022WadR) {
            if (resolved == -2) {
                if (!cpctGetNavYawRecomputeEnabled() && g_navHasNoLegalFacing == 1)
                    resolved = kAnimStandLoop;
            }
        } else if (resolved == -2) {
            resolveTurn = true;
        }
    }

    if (resolveTurn) {
        const std::int32_t noLegal = g_navHasNoLegalFacing;
        bool handled = false;
        if (!g_underMinimumSnapSpeed && noLegal != 1) {
            const bool eligible = !holdingStop || static_cast<double>(speed) - 0.6 > 0.0 || (control && control->autoRotateYawDelta != 0);
            if (eligible && cpctGetNavYawRecomputeEnabled()) {
                const YawResolveMode mode = movement == PlayerMovementState::ResolveBlockedA || movement == PlayerMovementState::ResolveBlockedB ?
                    YawResolveMode::StrafeStepSector :
                    YawResolveMode::Forced;
                resolved = ryoResolveYawDrivenAnim(request, mode, static_cast<std::int16_t>(desiredYaw + 0x8000));
                handled = true;
            }
        }
        if (!handled) {
            if (static_cast<std::uint32_t>(g_navigationAnimationState - 1) <= 3U) {
                if ((g_playerMotion->motionFlags & kMotionStuck) != 0) {
                    if (static_cast<std::uint32_t>(g_navigationAnimationState - 4) > 2U)
                        resolved = kAnimStandLoop;
                } else {
                    resolved = -1;
                }
            } else if (cpctGetNavYawRecomputeEnabled()) {
                const std::int32_t yaw = static_cast<std::int16_t>(cpctGetNewYaw());
                desiredYaw = static_cast<std::int16_t>(yaw + 0x8000);
                resolved = ryoResolveYawDrivenAnim(request, YawResolveMode::OctantSnapWalk, yaw);
                if (resolved == -2 && noLegal == 1) {
                    resolved = ryoResolveYawDrivenAnim(request, YawResolveMode::StrafeStepSector, yaw);
                    control->fightOnly = 1;
                }
            } else {
                const bool held = (control->heldInput & 0x0200U) != 0 || control->triggerHigh != 0;
                const bool movementActive = control->movementInputActive != 0;
                if ((!held || control->fightOnly != 0) && movement == PlayerMovementState::WalkForward && (!movementActive || g_ryoIdleResetCounter >= 5)) {
                    const std::int32_t chainYaw = ryo ? NodeChainYaw(&ryo->entity.node) : 0;
                    std::int32_t relative = static_cast<std::int16_t>(desiredYaw - chainYaw - 0x8000);
                    relative = relative > 0x4000 ? 0x4000 : std::max(relative, -0x4000);
                    desiredYaw = static_cast<std::int16_t>(relative + chainYaw - 0x8000);
                    resolved = ryoResolveYawDrivenAnim(request, YawResolveMode::SkeletonPlusOffset, static_cast<std::int16_t>(desiredYaw + 0x8000));
                    control->fightOnly = 1;
                } else if ((held && control->fightOnly == 0) || g_turnInputLockoutFrames <= 0) {
                    resolved = -3;
                } else {
                    const std::int32_t chainYaw = ryo ? NodeChainYaw(&ryo->entity.node) : 0;
                    desiredYaw = static_cast<std::int16_t>(control->blockedResolveTurnAccumYaw + chainYaw - 0x8000);
                    resolved = ryoResolveYawDrivenAnim(request, YawResolveMode::KeepSmoothed, static_cast<std::int16_t>(desiredYaw + 0x8000));
                    control->fightOnly = 1;
                }
            }
        }
    }

    const std::uint8_t debugFlags = g_debugAnimationFlag;
    if (((rateMode > 0 && (rateMode <= 8 || static_cast<std::uint32_t>(rateMode - 10) <= 1U)) || resolved == -2 || resolved == kAnimStandLoop ||
            resolved == kAnimWalkTurn180L) &&
        request.shouldTransitionToIdle == 1) {
        resolved = -1;
    } else if (resolved == kAnimStandLoop) {
        resolved = static_cast<std::int32_t>(control->motionMetadata[0].animationId);
        if (resolved == kAnimIdleRoot) {
            if (CurrentRyo() && (debugFlags & 0xfcU) == 0 && debugFlags != 2)
                resolved = UpdateRyoIdleAnimationWithRootMotion(*control, request.forceBlendOut);
            else
                resolved = kAnimStandLoop;
        } else {
            request.metadataAnimationId = resolved;
            request.metadataFlags = static_cast<std::int32_t>(control->motionMetadata[0].flags);
            request.metadataPlaybackRate = control->motionMetadata[0].playbackRate;
        }
    }
    g_tempDesiredFacingYaw = desiredYaw;
    if (debugFlags == 0) {
        if (resolved == kAnimIdleRoot) {
            if (static_cast<std::uint32_t>(animation - 1072) <= 4U)
                request.forceBlendOut = 1;
        } else if (resolved == -2 && animation == kAnimIdleRoot && (g_playerMotion->motionFlags & kMotionForceBlendOut) != 0) {
            request.forceBlendOut = 1;
        }
    }
    g_currentAnimBlendIndex = request.animationId == kAnimRunMidLoop ? 2 : 0;
    if (resolved != -2)
        return resolved;
    g_currentAnimBlendIndex = 0;
    const std::int32_t loop = g_animBlendTable[0].animationId;
    request.metadataAnimationId = loop;
    request.metadataFlags = static_cast<std::int32_t>(control->motionMetadata[8].flags);
    request.metadataPlaybackRate = control->motionMetadata[8].playbackRate;
    request.turnBlendParam = 0;
    return loop;
}

void ryoPrimeAnimBlendState(LocomotionAnimRequest &request) {
    const std::int32_t requested = request.resolvedAnimationId;
    std::uint16_t frameCount = 5;
    MotionInformationWork &motion = *g_playerMotion;
    const AnimBlendEntry &entry = g_animBlendTable[static_cast<std::size_t>(g_currentAnimBlendIndex)];
    std::uint8_t blendMode = 1;
    g_navStrafeBlocked = 0;
    std::int32_t trackedNode = 10;
    const auto enterWalkBlend = [&](std::int32_t node) {
        g_ryoCurrentAnimationState = kStateWalkBlend;
        request.blendFrame = 0.0F;
        blendMode = 9;
        g_ryoIdleAnimationIterations = 0;
        frameCount = 6;
        trackedNode = node;
    };
    const auto carryFrame = [&] {
        blendMode = 3;
        frameCount = 8;
        request.blendFrame = motComputeBlendedAnimationFrame(request, motion);
    };
    const auto walkRequest = [&](std::int32_t value) {
        return value == kAnimWalkHighLoop || value == kAnimStairsUpLoop || value == kAnimStairsDownLoop || value == kAnimRunMidLoop ||
            value == kAnimSlowRunLoop || value == kAnimWalkFastKLoop || value == kAnimWalkSlowKLoop || value == kAnimWalkSp2Loop || value == kAnimRunSlowKLoop;
    };
    const std::int32_t current = request.animationId;
    if (requested != entry.animationId) {
        if (requested != kAnimIdleRoot && requested != kAnimStandFightLoop && requested != kAnimStandLoop) {
            if (requested == kAnimWalkTurn180L) {
                if (request.blendOutLatched) {
                    g_navStrafeBlocked = navGetStrafeClearanceState() == 1 ? 1 : 0;
                } else {
                    request.forceBlendOut = 1;
                    g_navStrafeBlocked = navGetStrafeClearanceState() == 2 ? 1 : 0;
                }
            }
        } else if (current == kAnimSekkinKamaeLoop || current == kAnimMimawasu) {
            blendMode = 0;
        } else if (current >= kAnimWalk045L && current <= kAnimWalk180L) {
            enterWalkBlend(6);
        } else if (current > kAnimWalk180L && current <= kAnimWalk180R) {
            enterWalkBlend(7);
        } else if (((getNextAnimState_Internal(current) - kStateWalk) & ~4) == 0) {
            enterWalkBlend(10);
        }
    } else if (current >= kAnimWalk045L && current <= kAnimWalk180R) {
        request.blendFrame = current <= kAnimWalk180L ? entry.blendStartLeft : entry.blendStartRight;
        blendMode = 3;
        g_ryoCurrentAnimationState = kStateIdle;
        g_blendFrameIterations = 0;
    } else if (current == kAnimWalkTurn180L) {
        blendMode = 1;
        request.blendFrame = entry.blendEndThreshold;
        frameCount = 8;
        if ((motion.motionFlags & kMotionForceBlendOut) == 0)
            request.forceBlendOut = 1;
    } else if (current == kAnimWalkHighLoop || current == kAnimWalkFastKLoop || current == kAnimWalkSlowKLoop || current == kAnimWalkSp2Loop) {
        if (static_cast<std::uint32_t>(requested - kAnimStairsUpLoop) <= 1U || requested == kAnimSlowRunLoop)
            carryFrame();
    } else if (current == kAnimStairsUpLoop || current == kAnimStairsDownLoop) {
        if (requested == kAnimWalkFastKLoop || requested == kAnimWalkSlowKLoop || requested == kAnimWalkHighLoop || requested == kAnimRunMidLoop ||
            requested == kAnimSlowRunLoop || requested == kAnimWalkSp2Loop || requested == kAnimRunSlowKLoop)
            carryFrame();
    } else if (current == kAnimSlowRunLoop) {
        if (walkRequest(requested) && requested != kAnimSlowRunLoop)
            carryFrame();
    } else if (current == kAnimRunMidLoop || current == kAnimRunSlowKLoop) {
        if (requested >= kAnimStairsUpLoop) {
            if (requested <= kAnimStairsDownLoop || requested == kAnimSlowRunLoop) {
                carryFrame();
            } else if (requested == kAnimWalkSp2Loop) {
                blendMode = 3;
                frameCount = 8;
                if (request.blendSlot == 2) {
                    request.blendFrame = motComputeBlendedAnimationFrame(request, motion);
                } else {
                    request.blendFrame = 31.0F;
                    if ((motion.motionFlags & kMotionForceBlendOut) != 0)
                        request.forceBlendOut = 1;
                }
                motion.interpolationAmount = 1.0F;
            }
        }
    } else {
        blendMode = 1;
        if (g_playerMovementState == PlayerMovementState::RunForward)
            frameCount = 4;
        request.blendFrame = entry.startFrame;
        g_blendFrameIterations = 0;
        trackedNode = request.forceBlendOut != 0 ? 7 : 6;
    }
    motion.motionFlags |= kMotionFrameBlendReady;
    motion.forcedFrameTime = request.blendFrame;
    motion.blendMode = blendMode;
    motion.blendFrameCount = frameCount;
    if ((g_pControlPadController->motionMetadata[8].flags & 0x80U) != 0) {
        if (trackedNode == 6) {
            g_locomotionTrackedNode = 7;
            return;
        }
        if (trackedNode == 7)
            trackedNode = 6;
    }
    g_locomotionTrackedNode = trackedNode;
}

std::int32_t ryoUpdateAnimById(std::int32_t requestedAnimation) {
    LocomotionAnimRequest request{};
    request.animationId = g_currentAnimationId;
    request.animationState = g_ryoCurrentAnimationState;
    request.frameCount = static_cast<std::int32_t>(static_cast<float>(MOTN_GetFrameCountFromAnimId(static_cast<std::uint16_t>(g_currentAnimationId))));
    MotionInformationWork &motion = *g_playerMotion;
    request.frame = MOTI_GetPlayRateScaledRemain(motion);
    request.blendOutLatched = 0;
    request.blendSlot = g_currentAnimationId == kAnimRunMidLoop ? 2 : 0;
    std::int32_t requested = requestedAnimation;
    if (requested < 0) {
        requested = -requested;
        request.forceBlendOut = 1;
    }
    request.requestAnimationId = requested;
    request.playRateMultiplier = 1.0F;
    request.metadataAnimationId = -1;
    request.metadataFlags = 0;
    request.metadataPlaybackRate = 1.0F;
    request.turnBlendParam = 0;
    request.resolvedAnimationId = requested == kAnimRequestStand ? kAnimStandLoop : requested == kAnimRequestLocomotion ? -2 : requested;
    if (g_blendFrameIterations < 100)
        ++g_blendFrameIterations;
    if (g_ryoIdleAnimationIterations < 100)
        ++g_ryoIdleAnimationIterations;
    if (static_cast<std::uint32_t>(requested - kAnimRequestStand) <= 1U)
        request.playRateMultiplier = 1.0F;
    request.shouldTransitionToIdle = ShouldTransitionToIdleState(request) ? 1 : 0;
    request.blendOutLatched = g_probePoints.rows[2].x >= g_probePoints.rows[2].w ? 1 : 0;
    const std::int32_t resolved = ryoResolveNextLocomotionAnim(request, request.animationId, request.animationState, request.resolvedAnimationId);
    request.resolvedAnimationId = resolved;
    if (resolved < 0)
        return resolved;
    if (resolved == request.animationId && motion.previousAnimId == 0xffffU)
        return -1;
    motion.playbackRate = g_animPlayRate * request.playRateMultiplier;
    if (resolved == kAnimTurn180WadL) {
        if (navGetStrafeClearanceState() == 1)
            request.resolvedAnimationId = kAnimTurn180WadR;
    } else if (resolved != kAnimWalk180L) {
        if ((g_pControlPadController->motionMetadata[8].flags & 0x80U) != 0)
            request.forceBlendOut = static_cast<std::uint8_t>(request.metadataFlags) >> 7U;
    } else if (navGetStrafeClearanceState() == 1) {
        request.resolvedAnimationId = kAnimWalk180R;
    }
    const std::int32_t finalAnimation = request.resolvedAnimationId;
    g_playerMotion->pendingTurnEulerDelta.y = request.turnBlendParam;
    if (finalAnimation != kAnimStandLoop && static_cast<std::uint32_t>(finalAnimation - kAnimIdleRoot) > 1U && finalAnimation != kAnimStandFightLoop)
        g_playerMotion->headLookAngleCommand = 2048;
    g_playerMotion->motionFlags &= ~kMotionStuck;
    g_playerMotion->pendingAnimId = static_cast<std::uint16_t>(finalAnimation);
    ryoPrimeAnimBlendState(request);
    if (request.forceBlendOut)
        g_playerMotion->motionFlags |= kMotionDirtyAnimState;
    return request.resolvedAnimationId;
}

bool ryoRequestAnimWithTest(std::int32_t animation) {
    const std::int32_t resolved = ryoUpdateAnimById(animation);
    if (resolved == kAnimRunTurn180L || resolved == kAnimWalkTurn180L)
        g_navigationAnimationState = 6;
    return resolved >= 0;
}

void ryoUpdatePlayerClearance(const MotionInformationWork &motion) {
    float *const probe = &g_probePoints.rows[0].x;
    if (const MotionNodeRuntime *const left = FindPreviousMotionNode(motion, 0x1b)) {
        probe[0] = left->transform->rows[3].x;
        probe[1] = left->transform->rows[3].y;
        probe[2] = left->transform->rows[3].z;
    }
    if (const MotionNodeRuntime *const right = FindPreviousMotionNode(motion, 0x22)) {
        probe[3] = right->transform->rows[3].x;
        probe[4] = right->transform->rows[3].y;
        probe[5] = right->transform->rows[3].z;
    }
    const Vector3 leftLocal = ryoTransformWorldToPlayerYawSpace({probe[0], probe[1], probe[2]});
    probe[6] = leftLocal.x;
    probe[7] = leftLocal.y;
    probe[8] = leftLocal.z;
    const Vector3 rightLocal = ryoTransformWorldToPlayerYawSpace({probe[3], probe[4], probe[5]});
    probe[9] = rightLocal.x;
    probe[10] = rightLocal.y;
    probe[11] = rightLocal.z;
    g_probeDeltaZ = probe[7] - probe[13];
    g_probeDeltaX = probe[6] - probe[12];
    g_probeDeltaAccumulated.y = probe[9] - probe[15];
    g_probeDeltaAccumulated.x = probe[8] - probe[14];
    const float rightDeltaZ = probe[10] - g_probeRightPreviousZ;
    probe[12] = probe[6];
    probe[13] = probe[7];
    probe[14] = probe[8];
    probe[15] = probe[9];
    g_probeDeltaAccumulated.w = probe[11] - g_probeRightPreviousW;
    g_probeRightPreviousZ = probe[10];
    g_probeDeltaAccumulated.z = rightDeltaZ;
    g_probeRightPreviousW = probe[11];
    if (probe[10] <= probe[7])
        g_probeZCompareClass = probe[7] > probe[10] ? 2 : 0;
    else
        g_probeZCompareClass = 1;
    const float *groundProbe = probe;
    if (probe[11] <= probe[8]) {
        g_probeXCompareClass = probe[8] <= probe[11] ? 0 : 2;
        groundProbe = probe + 3;
    } else {
        g_probeXCompareClass = 1;
    }
    ryoComputeYawOffsProbeWithIntersection({groundProbe[0], groundProbe[1], groundProbe[2]}, motion.extractedYaw, 0.2F);
    g_probeRightDeltaZ = rightDeltaZ;
}

void ryoUpdateAnimPlayRateFromState() {
    MotionInformationWork &motion = *g_playerMotion;
    const std::int32_t state = g_ryoCurrentAnimationState;
    ryoUpdatePlayerClearance(motion);
    const PlayerControlWork &control = *g_pControlPadController;
    const auto blendedRate = [&] {
        double primary = g_animBlendTable[0].blendRate;
        if (control.stateFlags == 1)
            primary *= 1.2;
        const float secondary = g_animBlendTable[2].blendRate * motion.interpolationAmount;
        return static_cast<float>((1.0 - static_cast<double>(motion.interpolationAmount)) * primary + static_cast<double>(secondary)) * g_animPlayRate;
    };
    switch (state) {
    case kStateIdle:
    case kStateIdleAlt:
        if (control.stateFlags == 1 && g_playerMovementState == PlayerMovementState::RunForward)
            motion.playbackRate = g_animPlayRate * 2.0F;
        else if (motion.currentAnimId == kAnimWalk180L || motion.currentAnimId == kAnimWalk180R)
            motion.playbackRate = g_animPlayRate * 0.69999999F;
        else
            motion.playbackRate = g_animPlayRate * 1.0F;
        break;
    case kStateWalk:
    case kStateRun:
        g_locomotionTrackedNode = 10;
        motion.playbackRate = blendedRate();
        break;
    case kStateWalkBlend:
        if (control.stateFlags == 1)
            motion.playbackRate = g_animPlayRate * 1.2F;
        break;
    case kStateRunBlend:
        motion.playbackRate = blendedRate();
        break;
    case kStatePlayRateDirect:
        motion.playbackRate = g_animPlayRate;
        break;
    default:
        break;
    }
    const CharacterCollisionWork *const collision = PlayerCollisionWork();
    if (collision && collision->avoidanceResult != 0 && g_navStateCurrent != 2)
        return;
    if (g_navHasNoLegalFacing == 1)
        return;
    motion.trackedNodeIndex = g_locomotionTrackedNode;
    if (g_locomotionTrackedNode != 10) {
        ryoCalcCurrentYaw();
        g_forceYawRecalculation = 1;
    }
}

std::int32_t cpctCalcControlPointYaw(Character *character) {
    if (!character)
        return 0;
    const MotionManagerWork *const manager = MOTM_GetWork(character);
    const MotionNodeRuntime *const chest = manager ? FindPreviousMotionNode(manager->motion, 3) : nullptr;
    const Vector3 chestPosition =
        chest ? Vector3{chest->transform->rows[3].x, 0.0F, chest->transform->rows[3].z} : GetPositionForEntity(character->entity.node);
    const MotionNodeRuntime *const head = manager ? FindPreviousMotionNode(manager->motion, 6) : nullptr;
    const Vector3 headPosition = head ? Vector3{head->transform->rows[3].x, 0.0F, head->transform->rows[3].z} : GetPositionForEntity(character->entity.node);
    if (chestPosition.x != headPosition.x || chestPosition.z != headPosition.z)
        return Atan2Angle(chestPosition.x - headPosition.x, chestPosition.z - headPosition.z);
    return NodeChainYaw(&character->entity.node);
}

std::int32_t shenGetYawTarget(Character *character, YawResolveMode mode, std::int32_t smoothedYaw) {
    switch (mode) {
    case YawResolveMode::TurnSectorDelta:
        return cpctCalcControlPointYaw(character);
    case YawResolveMode::Skeleton:
        return character ? NodeChainYaw(&character->entity.node) : 0;
    case YawResolveMode::CharacterWorldOrBlocked:
        if (g_navStrafeBlocked == 1) {
            const std::int32_t chainYaw = character ? NodeChainYaw(&character->entity.node) : 0;
            return Fold16(chainYaw + chainYaw - charaGetWorldYaw(character));
        }
        return Fold16(charaGetWorldYaw(character));
    case YawResolveMode::SkeletonPlusOffset:
        return Fold16((character ? NodeChainYaw(&character->entity.node) : 0) + g_pControlPadController->yawOffset);
    case YawResolveMode::KeepSmoothed:
        return smoothedYaw;
    case YawResolveMode::Forced:
        return g_pControlPadController->forcedYaw;
    default:
        return 0;
    }
}

void ryoUpdateTurnAnimWithYaw(Character *character) {
    if (g_turnFrameCount < 9999)
        ++g_turnFrameCount;
    const std::int32_t yaw = g_smoothedAnimationYaw;
    YawResolveMode mode = YawResolveMode::Skeleton;
    switch (g_playerMovementState) {
    case PlayerMovementState::TurnAccumRight:
    case PlayerMovementState::TurnAccumLeft:
    case PlayerMovementState::TurnCommitFamily7:
    case PlayerMovementState::TurnCommitFamily8:
    case PlayerMovementState::TurnCommitFamily9:
    case PlayerMovementState::TurnCommitFamily10:
    case PlayerMovementState::TurnCommitRight:
    case PlayerMovementState::TurnCommitLeft:
    case PlayerMovementState::TurnHoldPreCommit:
    case PlayerMovementState::TurnHoldAlt:
        mode = YawResolveMode::SkeletonPlusOffset;
        break;
    case PlayerMovementState::ResolveBlockedA:
    case PlayerMovementState::ResolveBlockedB:
        mode = YawResolveMode::KeepSmoothed;
        break;
    default:
        switch (g_ryoCurrentAnimationState) {
        case kStateIdle:
        case kStateIdleAlt:
            mode = g_turnYawResolveMode == YawResolveMode::KeepSmoothed ? YawResolveMode::KeepSmoothed : YawResolveMode::CharacterWorldOrBlocked;
            break;
        case kStateWalkTurn180:
            mode = YawResolveMode::CharacterWorldOrBlocked;
            break;
        case kStateFight:
            mode = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_turnYawResolveMode) - 4) > 1U ? YawResolveMode::CharacterWorldOrBlocked :
                                                                                                          YawResolveMode::KeepSmoothed;
            break;
        case kStateIdleRootVariant:
            mode = YawResolveMode::KeepSmoothed;
            break;
        default:
            break;
        }
        break;
    }
    if (g_navigationAnimationState != 1) {
        const std::int32_t animation = g_playerMotion->currentAnimId;
        if (animation == kAnimWalk180L || animation == kAnimWalk180R || animation == kAnimWalkTurn180L || animation == kAnimTurn180WadL ||
            animation == kAnimTurn180WadR || g_playerMovementState == PlayerMovementState::Turn180InPlace) {
            if (g_turnPhaseTracking == 1)
                g_turnPhaseTracking = 2;
            g_turnHoldFrameCount = 0;
        } else {
            if (g_turnPhaseTracking == 2)
                g_turnPhaseTracking = 0;
            if (g_playerMovementState != PlayerMovementState::TurnExitToIdle && ((g_ryoCurrentAnimationState - kStateWalk) & ~4) == 0 &&
                g_currentTurnMode != TurnMode::Mode14 && g_pControlPadController && g_pControlPadController->inputYawDelta > 0) {
                if (g_turnHoldFrameCount < 9999)
                    ++g_turnHoldFrameCount;
            } else {
                g_turnHoldFrameCount = 0;
            }
        }
    }
    if (g_turnFrameCount < 20) {
        std::int32_t delta = Fold16(shenGetYawTarget(character, YawResolveMode::CharacterWorldOrBlocked, g_smoothedAnimationYaw) - yaw);
        const auto blend = static_cast<float>(static_cast<double>(static_cast<float>(g_turnFrameCount) / 20.0F) * 0.15000000000000002 + 0.05);
        if (g_angleWrapPolicy == 1) {
            if (delta >= 0)
                delta -= 0x10000;
        } else if (g_angleWrapPolicy == 2) {
            if (delta < 0)
                delta += 0x10000;
        } else if (g_angleWrapPolicy == 0 && delta <= -30947) {
            delta += 0x10000;
        }
        g_turnYawResolveMode = YawResolveMode::CharacterWorldOrBlocked;
        g_smoothedAnimationYaw = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(delta) * blend + static_cast<float>(yaw)));
        return;
    }
    if (static_cast<std::uint32_t>(static_cast<std::int32_t>(mode) - 4) <= 1U) {
        g_smoothedAnimationYaw = shenGetYawTarget(character, mode, g_smoothedAnimationYaw);
    } else {
        const std::int32_t delta = Fold16(shenGetYawTarget(character, mode, g_smoothedAnimationYaw) - yaw);
        g_smoothedAnimationYaw = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(delta) * 0.2 + static_cast<double>(yaw)));
    }
    g_turnYawResolveMode = mode;
}

void PlayerMovementInputStateController(InputSlot &input) {
    const TurnPhase phase = g_turningPhase;
    if (phase == TurnPhase::TurnFinishing)
        return;
    const auto turnMode = static_cast<std::int32_t>(g_currentTurnMode);
    if ((static_cast<std::uint32_t>(turnMode - 5) <= 2U || static_cast<std::uint32_t>(turnMode - 14) <= 2U) && !ryoCanMove())
        return;
    std::uint16_t blocked = static_cast<std::uint16_t>(g_turnDirectionMask & input.held);
    g_turnDirectionMask = blocked;
    if (blocked) {
        const float dx = g_previousTurnNavigationPosition.x - g_playerNavigationPosition.x;
        const float dz = g_previousTurnNavigationPosition.z - g_playerNavigationPosition.z;
        if (std::sqrt(dx * dx + dz * dz) >= 1.0F) {
            blocked = 0;
            g_turnDirectionMask = 0;
        }
    }
    input.held &= static_cast<std::uint16_t>(~blocked);
    input.pressedPriority &= static_cast<std::uint16_t>(~blocked);
    const std::uint16_t held = input.held;
    const PlayerMovementState movement = g_playerMovementState;
    const auto movementValue = static_cast<std::int32_t>(movement);
    const std::int32_t state = g_ryoCurrentAnimationState;
    if ((state == kStateStand || state == kStateFightFallback) &&
        (movement == PlayerMovementState::TurnExitToIdle || static_cast<std::uint32_t>(movementValue - 7) <= 6U)) {
        g_currentTurnMode = TurnMode::Mode5;
    } else if (movement == PlayerMovementState::SnapFailFallback) {
        g_currentTurnMode = TurnMode::Mode14;
    } else if (movement == PlayerMovementState::AutoRotateForcedNegative) {
        g_currentTurnMode = TurnMode::YawStepPositive;
    } else if (movement == PlayerMovementState::AutoRotateForcedPositive) {
        g_currentTurnMode = TurnMode::YawStepNegative;
    } else if (g_playerMotion->interpolationAmount > 0.0F && movement == PlayerMovementState::Idle) {
        g_currentTurnMode = TurnMode::Mode15;
    } else if (phase == TurnPhase::TargetYawSelected) {
        if ((held & 0x5000U) == 0x5000U)
            g_currentTurnMode = TurnMode::Mode6;
        else if ((held & 0x9000U) == 0x9000U)
            g_currentTurnMode = TurnMode::Mode7;
        else if ((held & 0x4000U) != 0)
            g_currentTurnMode = TurnMode::Mode6;
        else if ((held & 0x8000U) != 0)
            g_currentTurnMode = TurnMode::Mode7;
    } else if ((held & 0xc000U) != 0 && ryoCanMove()) {
        if ((held & 0x8000U) != 0)
            g_currentTurnMode = TurnMode::Mode3;
        else if ((held & 0x4000U) != 0)
            g_currentTurnMode = TurnMode::Mode1;
    } else {
        g_currentTurnMode = TurnMode::Mode0;
    }

    std::int32_t target = g_targetPlayerYaw;
    const auto selectTarget = [&](std::int32_t value, bool resetMovement) {
        target = value;
        if (resetMovement) {
            g_playerMovementState = PlayerMovementState::Idle;
            g_inputStateHoldFrames = 0;
        }
        g_targetPlayerYaw = target;
        g_turningPhase = TurnPhase::TargetYawSelected;
    };
    switch (g_currentTurnMode) {
    case TurnMode::Mode5:
        switch (movement) {
        case PlayerMovementState::TurnExitToIdle:
            selectTarget(Fold16(g_lastYawSampleForSnap + 0x8000), true);
            break;
        case PlayerMovementState::Turn180InPlace:
            selectTarget(Fold16(g_currentPlayerYaw + 0x8000), true);
            break;
        case PlayerMovementState::TurnCommitFamily7:
            selectTarget(Fold16(g_currentPlayerYaw + 0x1000), true);
            break;
        case PlayerMovementState::TurnCommitFamily8:
            selectTarget(Fold16(g_currentPlayerYaw - 0x1000), true);
            break;
        case PlayerMovementState::TurnCommitFamily9:
            selectTarget(Fold16(g_currentPlayerYaw + 0x2000), true);
            break;
        case PlayerMovementState::TurnCommitFamily10:
            selectTarget(Fold16(g_currentPlayerYaw - 0x2000), true);
            break;
        case PlayerMovementState::TurnCommitRight:
        case PlayerMovementState::TurnCommitLeft:
            selectTarget(Fold16(g_currentPlayerYaw + g_pControlPadController->yawOffset), false);
            break;
        default:
            if (navFindLegalFacingInSector(g_playerNavigationPosition, g_currentPlayerYaw - 0x8000, NavProbeSector::FullSweep)) {
                target = g_targetPlayerYawSnap;
                g_targetPlayerYaw = target;
                if (Abs16(target - g_currentPlayerYaw) <= 0x4000) {
                    target = Fold16(g_currentPlayerYaw + 0x8000);
                    g_targetPlayerYaw = target;
                }
            }
            break;
        }
        break;
    case TurnMode::Mode6:
        if (navFindLegalFacingInSector(g_playerNavigationPosition, g_currentPlayerYaw, NavProbeSector::Positive120Fan)) {
            target = g_targetPlayerYawSnap;
            g_targetPlayerYaw = target;
        }
        break;
    case TurnMode::Mode7:
        if (navFindLegalFacingInSector(g_playerNavigationPosition, g_currentPlayerYaw, NavProbeSector::Negative120Fan)) {
            target = g_targetPlayerYawSnap;
            g_targetPlayerYaw = target;
        }
        break;
    case TurnMode::Mode8:
        if (navFindLegalFacingInSector(g_playerNavigationPosition, g_currentPlayerYaw + 0x4000, NavProbeSector::FullSweep)) {
            target = g_targetPlayerYawSnap;
            g_targetPlayerYaw = target;
        }
        break;
    case TurnMode::Mode9:
        if (navFindLegalFacingInSector(g_playerNavigationPosition, g_currentPlayerYaw - 0x4000, NavProbeSector::FullSweep))
            g_targetPlayerYaw = g_targetPlayerYawSnap;
        target = g_targetPlayerYaw;
        break;
    case TurnMode::Mode14:
    case TurnMode::Mode16:
        target = Fold16(g_currentPlayerYaw + 0x8000);
        g_targetPlayerYaw = target;
        break;
    default:
        break;
    }
    if (g_currentTurnMode == TurnMode::Mode8)
        g_currentTurnMode = Abs16(target - g_currentPlayerYaw - 10922) > 0x2000 ? TurnMode::Mode0 : TurnMode::Mode6;
    else if (g_currentTurnMode == TurnMode::Mode9)
        g_currentTurnMode = Abs16(target - g_currentPlayerYaw + 10922) > 0x2000 ? TurnMode::Mode0 : TurnMode::Mode7;
    if (g_currentTurnMode == TurnMode::Mode6)
        g_turnDirectionMask = 0x4000;
    else if (g_currentTurnMode == TurnMode::Mode7)
        g_turnDirectionMask = 0x8000;
    else
        return;
    g_previousTurnNavigationPosition = g_playerNavigationPosition;
}

void cpctUpdateStartAnimSpeedAndMovementFlags(MotionInformationWork &motion) {
    PlayerControlWork *const control = g_pControlPadController;
    if (motion.currentAnimId == kAnimRunMidLoop && motiCheckLocomotionState(&motion)) {
        control->startAnimationSpeed = 1.0F;
        control->speedAdjustWindow = 1.0F;
        return;
    }
    if (static_cast<std::uint32_t>(static_cast<std::int32_t>(g_currentTurnMode) - 14) > 1U && g_playerMovementState != PlayerMovementState::Idle) {
        if (!g_hasNavSnapBudget)
            return;
        const float limited = cpctGetCurrFrmCnt() ? (control ? control->speedScale : 0.0F) : g_ryoRawMovementInputSpeed;
        const auto ramp = static_cast<float>((static_cast<double>(static_cast<float>(static_cast<double>(limited) - 1.0))) * 0.5);
        const float positive = ramp > 0.0F ? ramp : 0.0F;
        float target = static_cast<float>(1.0F > positive ? static_cast<double>(positive) : 1.0);
        if (g_deltaYaw != 0) {
            if (0.0F < motion.interpolationAmount)
                target = 0.0F;
            else
                g_deltaYaw = 0;
        }
        if (control->startAnimationSpeed > target) {
            float window = motion.walkBlendAnimId == kAnimRunLoop ? 3.0F : 2.0F;
            if (g_speedWasLimited)
                window = static_cast<float>(static_cast<double>(window) + static_cast<double>(window));
            control->startAnimationSpeed = target;
            control->speedAdjustWindow = window;
        }
        return;
    }
    const bool held = ((control->heldInput & 0x0200U) != 0 || control->triggerHigh != 0) && control->fightOnly == 0;
    const auto looking = static_cast<std::uint8_t>(GetLookingAroundFactor());
    if (held || (useAnalogInput && looking >= 1U)) {
        control->speedAdjustWindow = 8.0F;
        control->startAnimationSpeed = 0.0F;
    } else {
        control->speedAdjustWindow = 2.0F;
        control->startAnimationSpeed = 0.0F;
    }
}

float cpctUpdateMoveAndTurnFactor(const MotionInformationWork &motion) {
    const float interpolation = motion.interpolationAmount;
    PlayerControlWork &control = *g_pControlPadController;
    if (control.resetSpeedBlend || ((g_ryoCurrentAnimationState - kStateWalk) & ~4) != 0) {
        control.resetSpeedBlend = 0;
        control.speedAdjustWindow = 1.0F;
        control.speedTime = 0.0F;
        control.startAnimationSpeed = 0.0F;
        return 0.0F;
    }
    float step = std::max(std::min(control.startAnimationSpeed - interpolation, 1.0F), control.speedAdjustWindow * -0.050000001F);
    if (std::fabs(step) < 0.00000011920929F)
        step = 0.0F;
    float time = step * 0.19999999F + control.speedTime * 0.80000001F;
    if (std::fabs(time) < 0.00000011920929F)
        time = 0.0F;
    control.speedTime = time;
    return std::max(0.0F, std::min(1.0F, time + interpolation));
}

void shenGetAndSetPositionFor(Character *character, MotionInformationWork &motion) {
    if (!g_navigationTurnEnabled || g_navFilterDisabled)
        return;
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    g_playerNavigationPosition.y = position.y;
    const std::int32_t current = motion.currentAnimId;
    if (g_pControlPadController->forcedYawSnap == 1 && charaGetAvoidanceRes(character) == 1)
        return;
    if (((g_ryoCurrentAnimationState - kStateIdle) & ~4) == 0 && motion.blendFramesRemaining < motion.blendFrameCount && motion.blendFrameCount != 0) {
        const float fraction = static_cast<float>(motion.blendFramesRemaining) / static_cast<float>(motion.blendFrameCount);
        const Vector3 blended{(position.x - g_playerNavigationPosition.x) * fraction + g_playerNavigationPosition.x, position.y,
            (position.z - g_playerNavigationPosition.z) * fraction + g_playerNavigationPosition.z};
        if (character)
            shenApplyTranslationToAll(character->entity.node, blended);
        return;
    }
    if (((g_ryoCurrentAnimationState != kStateStand && g_ryoCurrentAnimationState != kStateFight && motion.blendMode != 2) || current == kAnimStandLoop) &&
        character) {
        shenApplyTranslationToAll(character->entity.node, g_playerNavigationPosition);
    }
}

void ProcessRequestedAnimation(MotionInformationWork &motion, const PlayerControlWork &control) {
    std::int32_t animation = static_cast<std::int32_t>(control.motionMetadata[0].animationId);
    std::int32_t blendOut = 0;
    if (animation == kAnimStandLoop || animation == kAnimIdleRoot) {
        if (CurrentRyo() && g_pControlPadController && (g_debugAnimationFlag & 0xfcU) == 0 && g_debugAnimationFlag != 2)
            animation = UpdateRyoIdleAnimationWithRootMotion(*g_pControlPadController, blendOut);
        else
            animation = kAnimStandLoop;
    }
    const std::int32_t current = motion.pendingAnimId ? motion.pendingAnimId : motion.currentAnimId;
    if (animation == current && motion.previousAnimId == 0xffffU)
        return;
    motion.motionFlags &= ~kMotionStuck;
    motion.playbackRate = control.motionMetadata[0].playbackRate;
    motion.blendFrameCount = 5;
    motion.blendMode = 1;
    motion.pendingAnimId = static_cast<std::uint16_t>(animation);
    if (blendOut)
        motion.motionFlags |= kMotionDirtyAnimState;
}

PlayerMovementState SelectForwardMovementState(const PlayerControlWork &control) {
    if ((((control.flags & kForceWalk) != 0 || control.numActiveFrames < 120U) && control.runOverride != kRunGateForceRun) ||
        g_ryoRawMovementInputSpeed < 2.0F || g_navStateCurrent == 2)
        return PlayerMovementState::WalkForward;
    return PlayerMovementState::RunForward;
}

void PublishTurnYawOffset(MotionInformationWork &motion, std::int32_t yaw) {
    g_turnYawOffsetFromAccum = yaw;
    motion.headLookAngleCommand = 2048;
    g_turnYawOffsetActive = 1;
}

void ryoUpdateMovementStateInternal(PlayerControlWork &ctx) {
    const PlayerMovementState currentInput = g_playerMovementState;
    const std::int32_t state = g_ryoCurrentAnimationState;
    bool commitTurn = false;
    if (g_turnInputLockoutFrames > 0)
        --g_turnInputLockoutFrames;
    const std::int32_t secondaryYaw = g_pControlPadController->secondaryYawOffset;
    if (state != kStateFight)
        ctx.fightOnly = 0;
    const std::int32_t snapDelta = std::abs(static_cast<std::int16_t>(g_playerMotion->extractedYaw - secondaryYaw));
    switch (state) {
    case kStateIdle:
    case kStateIdleAlt:
        if ((currentInput == PlayerMovementState::ResolveBlockedA || currentInput == PlayerMovementState::ResolveBlockedB) &&
            (g_playerMotion->motionFlags & kMotionStuck) != 0) {
            g_snapFailCounter = 0;
            return;
        }
        break;
    case kStateWalk:
    case kStateWalkBlend:
    case kStateRun:
    case kStateRunBlend:
        if (ctx.inputYawDelta > 0 && ctx.autoRotateYawDelta == 0 && (snapDelta > 0x6000 || g_navigationAnimationState == 6)) {
            if (g_snapFailCounter >= 3)
                g_playerMovementState = PlayerMovementState::SnapFailFallback;
            else
                ++g_snapFailCounter;
            return;
        }
        break;
    case kStateStand:
    case kStateFight:
        if ((state == kStateStand || g_playerAnimationActiveFrames > 15) && ctx.inputYawDelta > 0 && ctx.autoRotateYawDelta == 0 && snapDelta > 0x6000 &&
            !cpctGetFlagSetActive() && (((static_cast<std::int32_t>(currentInput) - 5) & ~9) != 0 || currentInput == PlayerMovementState::Turn180InPlace)) {
            g_playerMovementState = PlayerMovementState::Turn180InPlace;
            g_snapFailCounter = 0;
            return;
        }
        break;
    default:
        break;
    }

    g_snapFailCounter = 0;
    const bool facing = cpctUpdateFacingFromNavAndYawDelta(ctx, 0);
    const std::int32_t inputYawDelta = ctx.inputYawDelta;
    Character *const player = g_playerCharacter;
    const PlayerMovementState lastNonHold = g_lastNonHoldMovementState;
    MotionInformationWork &motion = *g_playerMotion;
    PlayerControlWork &control = *g_pControlPadController;
    PlayerMovementState next = g_playerMovementState;
    std::int32_t frames = 0;
    const auto store = [&](PlayerMovementState value) {
        next = value;
        g_playerMovementState = value;
    };
    if (inputYawDelta == 0 && ctx.autoRotateYawDelta == 0 && control.numActiveFrames < 120U &&
        (control.numActiveFrames == 0 || control.isRunning == 0 || control.wasRunning == 0) && !(motion.interpolationAmount > 0.0F)) {
        frames = g_inputStateHoldFrames;
        switch (next) {
        case PlayerMovementState::TurnExitFromForward:
            store(PlayerMovementState::TurnExitToIdle);
            break;
        case PlayerMovementState::TurnAccumRight:
        case PlayerMovementState::TurnAccumLeft:
            frames = 0;
            store(PlayerMovementState::TurnHoldPreCommit);
            break;
        case PlayerMovementState::Turn180InPlace:
            break;
        case PlayerMovementState::TurnHoldPreCommit:
            if (frames >= 5 && state != kStateFight)
                commitTurn = true;
            break;
        case PlayerMovementState::TransitionToWalkForward:
            store(PlayerMovementState::WalkForward);
            break;
        case PlayerMovementState::TurnHoldAlt:
            store(PlayerMovementState::ResolveBlockedA);
            break;
        default:
            store(PlayerMovementState::Idle);
            break;
        }
    } else {
        frames = next == PlayerMovementState::Idle ? 0 : g_inputStateHoldFrames;
        const auto movement = static_cast<std::int32_t>(next);
        const bool turnFamily = ((movement - 5) & ~9) == 0 && next != PlayerMovementState::Turn180InPlace;
        const bool held = ((control.heldInput & 0x0200U) != 0 || control.triggerHigh != 0) && control.fightOnly == 0;
        ctx.allowTurnInPlace = held && (!facing || (ctx.inputYawDelta8 == 0 && ctx.numActiveFrames == 0 && turnFamily)) ? 1 : 0;
        const std::uint8_t allow = ctx.allowTurnInPlace;
        if (allow == 1 && ctx.autoRotateYawDelta != 0)
            g_turnInputLockoutFrames = 2;
        if (facing && (inputYawDelta < 0 || ctx.numActiveFrames != 0)) {
            switch (next) {
            case PlayerMovementState::TurnAccumRight:
            case PlayerMovementState::TurnAccumLeft:
            case PlayerMovementState::TurnHoldPreCommit:
            case PlayerMovementState::TurnHoldAlt:
                store(((control.flags & kForceWalk) != 0 || control.numActiveFrames < 120U) && control.runOverride != kRunGateForceRun ?
                        PlayerMovementState::ResolveBlockedA :
                        PlayerMovementState::ResolveBlockedB);
                break;
            case PlayerMovementState::ResolveBlockedA:
            case PlayerMovementState::ResolveBlockedB:
                store(SelectForwardMovementState(control));
                break;
            default:
                if (static_cast<std::uint32_t>(g_navigationAnimationState - 1) <= 3U ||
                    (((state - kStateWalk) & ~4) == 0 && (player && GetPlayerCollisionWork(player)) &&
                        0.001 > static_cast<double>(control.resolvedMovementSpeed)))
                    store(PlayerMovementState::Idle);
                else
                    store(SelectForwardMovementState(control));
                break;
            }
        } else if (state != kStateFightFallback && state != kStateStand && (state != kStateFight || ctx.yawOffset == 0) && ctx.committedTurnAccumYaw == 0) {
            const std::int32_t animation = motion.currentAnimId;
            if (animation == kAnimTurn180WadL || animation == kAnimTurn180WadR)
                store(PlayerMovementState::Idle);
            else if (next != PlayerMovementState::WalkForward && next != PlayerMovementState::RunForward)
                store(facing && static_cast<std::uint32_t>(g_navigationAnimationState - 1) > 3U ? PlayerMovementState::WalkForward : PlayerMovementState::Idle);
            else if (!facing)
                store(PlayerMovementState::Idle);
            g_lastYawSampleForSnap = secondaryYaw;
        } else {
            const std::int32_t autoRotate = ctx.autoRotateYawDelta;
            if (autoRotate == 0) {
                if (movement >= 5 && movement <= 6) {
                    store(PlayerMovementState::ResolveBlockedA);
                } else if (next == PlayerMovementState::TurnHoldPreCommit) {
                    store(next);
                    if (frames >= 5 && state != kStateFight)
                        commitTurn = true;
                } else {
                    store(PlayerMovementState::Idle);
                }
            } else if (allow == 1) {
                const PlayerMovementState turn = autoRotate < 0 ? PlayerMovementState::TurnAccumRight : PlayerMovementState::TurnAccumLeft;
                if (next == PlayerMovementState::TurnHoldPreCommit && frames < 5 && lastNonHold == turn)
                    commitTurn = true;
                else
                    store(turn);
            } else if (turnFamily) {
                store(PlayerMovementState::ResolveBlockedA);
            } else {
                store(static_cast<std::uint32_t>(g_navigationAnimationState - 1) <= 3U ? PlayerMovementState::Idle : PlayerMovementState::WalkForward);
            }
            g_lastYawSampleForSnap = secondaryYaw;
        }
    }

    switch (next) {
    case PlayerMovementState::Idle:
        if (ctx.allowTurnInPlace == 1 && state == kStateFight && motion.currentAnimId != kAnimTurn180WadL && motion.currentAnimId != kAnimTurn180WadR)
            PublishTurnYawOffset(motion, -ctx.yawOffset);
        ctx.movementInputActive = ctx.inputYawDelta < 0 || ctx.numActiveFrames != 0 ? 1 : 0;
        break;
    case PlayerMovementState::TurnAccumRight:
    case PlayerMovementState::TurnAccumLeft:
        if (g_updateRenderFrameMode != 0) {
            const std::int32_t step = g_updateRenderFrameMode > 0 ? 600 : 500;
            ctx.yawOffset = Fold16(next == PlayerMovementState::TurnAccumRight ? ctx.yawOffset + step : ctx.yawOffset - step);
        }
        frames = 0;
        PublishTurnYawOffset(motion, -ctx.yawOffset);
        break;
    case PlayerMovementState::TurnCommitFamily7:
    case PlayerMovementState::TurnCommitFamily8:
    case PlayerMovementState::TurnCommitFamily9:
    case PlayerMovementState::TurnCommitFamily10:
    case PlayerMovementState::TurnCommitRight:
    case PlayerMovementState::TurnCommitLeft:
    case PlayerMovementState::TurnHoldPreCommit:
    case PlayerMovementState::TurnHoldAlt:
        PublishTurnYawOffset(motion, -ctx.yawOffset);
        break;
    case PlayerMovementState::Turn180InPlace:
        break;
    case PlayerMovementState::ResolveBlockedA:
    case PlayerMovementState::ResolveBlockedB:
        PublishTurnYawOffset(motion, -ctx.yawOffset);
        if ((((control.heldInput & 0x0200U) != 0 || control.triggerHigh != 0) && control.fightOnly == 0) || state == kStateFight) {
            store(PlayerMovementState::TurnHoldPreCommit);
        } else {
            ctx.blockedResolveTurnAccumYaw = ctx.yawOffset;
            ctx.yawOffset = 0;
            ctx.committedTurnAccumYaw = 0;
        }
        break;
    default:
        if (((state - kStateWalk) & ~4) == 0)
            ctx.yawOffset = 0;
        break;
    }

    if (state == kStateIdle)
        PublishTurnYawOffset(motion, (player ? NodeChainYaw(&player->entity.node) : 0) - static_cast<std::int32_t>(ctx.desiredFacingYaw));
    if (commitTurn) {
        const std::int32_t turnYaw = ctx.yawOffset;
        store(turnYaw <= 0 ? PlayerMovementState::TurnCommitLeft : PlayerMovementState::TurnCommitRight);
        if (ctx.committedTurnAccumYaw == 0)
            ctx.committedTurnAccumYaw = turnYaw;
        ctx.turnCommitCanMove = ctx.autoRotateYawDelta == 0 ? 1 : 0;
    } else if ((next == PlayerMovementState::Idle || next == PlayerMovementState::TurnCommitRight || next == PlayerMovementState::TurnCommitLeft) &&
        ctx.autoRotateYawDelta == 0) {
        ctx.turnCommitCanMove = 1;
    }
    g_inputStateHoldFrames = next == currentInput ? frames + 1 : 0;
    if (next != PlayerMovementState::TurnHoldPreCommit && next != PlayerMovementState::TurnHoldAlt)
        g_lastNonHoldMovementState = next;
}

void ResetTurnSmoothing() {
    g_currentTurnMode = TurnMode::Mode0;
    g_playerYawAngularSmoothed = 0;
    g_playerYawAngularTarget = 0;
    g_playerTurnStep = 0x2000;
    g_navigationTurnEnabled = 1;
    g_navSteeringClampIndex = 10;
}

void StartTurnFromTargetYaw() {
    if (static_cast<std::uint32_t>(static_cast<std::int32_t>(g_currentTurnMode) - 5) <= 2U) {
        const ResolvedLocomotion &resolved =
            ryoResolveTurnAnimFromRequestedYaw(YawResolveMode::KeepSmoothed, static_cast<std::int16_t>(g_targetPlayerYaw + 0x8000));
        ryoRequestAnimWithTest(resolved.animationId);
        g_turningPhase = TurnPhase::TurnAnimActive;
        g_playerMotion->pendingTurnEulerDelta.y = resolved.residualYaw;
        return;
    }
    const std::int32_t animation = ryoUpdateAnimById(kAnimRequestStand);
    if (animation == kAnimRunTurn180L || animation == kAnimWalkTurn180L)
        g_navigationAnimationState = 6;
    if (g_navHasNoLegalFacing != 1)
        g_turningPhase = TurnPhase::None;
}

void ryoUpdateLocomotion(Character *chara, InputSlot &input) {
    MotionInformationWork &motion = *g_playerMotion;
    const PlayerMovementState mode = g_playerMovementState;
    const std::int32_t startAnimation = motion.currentAnimId;
    ryoUpdateTurnAnimWithYaw(chara);
    if (mode == PlayerMovementState::ResolveBlockedA || mode == PlayerMovementState::ResolveBlockedB)
        g_navigationAnimationState = 0;
    PlayerControlWork *const pad = g_pControlPadController;
    switch (g_navigationAnimationState) {
    case 1:
        pad->autoRotateYawDelta = 0;
        pad->inputYawDelta = 0;
        if ((input.held & 0xf000U) == g_blockedDirectionMask && g_blockedDirectionFrames >= 0) {
            --g_blockedDirectionFrames;
        } else {
            g_blockedDirectionFrames = 0;
            g_navigationAnimationState = 0;
        }
        break;
    case 2:
        pad->autoRotateYawDelta = 0;
        pad->inputYawDelta = 0;
        if ((input.held & g_blockedDirectionMask) == 0)
            g_navigationAnimationState = 0;
        break;
    case 3: {
        const bool forcedSnap = pad->forcedYawSnap == 1;
        pad->autoRotateYawDelta = 0;
        pad->inputYawDelta = 0;
        if (forcedSnap || g_navHasNoLegalFacing != 1 || g_movementBlockedFrames >= 15 || g_blockedDirectionMask == 0 ||
            (input.held & static_cast<std::uint16_t>(~g_blockedDirectionMask) & 0xf000U) != 0)
            g_navigationAnimationState = 0;
        else
            g_blockedDirectionMask = static_cast<std::uint16_t>(input.held & 0xf000U);
        break;
    }
    case 4:
    case 5:
    case 6:
        if (!pad || pad->inputYawDelta <= 0)
            g_navigationAnimationState = 0;
        pad->autoRotateYawDelta = 0;
        pad->inputYawDelta = 0;
        break;
    default:
        break;
    }
    if (g_scenarioControllerConnected)
        cpctUpdateStartAnimSpeedAndMovementFlags(motion);
    motion.interpolationAmount = cpctUpdateMoveAndTurnFactor(motion);
    navUpdateRegionsFromControlPoints();
    PlayerMovementInputStateController(input);
    navUpdateSteeringAndResolveFacing(g_currentPlayerYaw);
    const float speed = g_navigationTurnEnabled ? ryoUpdateCachedStates() : 0.0F;
    g_locomotionSpeed = speed;

    PlayerControlWork &control = *g_pControlPadController;
    const TurnPhase phase = g_turningPhase;
    if (phase == TurnPhase::None || phase == TurnPhase::TurnFinishing ||
        (phase == TurnPhase::TurnAnimActive && static_cast<std::uint32_t>(g_ryoCurrentAnimationState - kStateFightFallback) > 1U)) {
        if (g_navHasNoLegalFacing != 0 && g_navHasNoLegalFacing != 1) {
            g_navSteeringClampIndex = 10;
        } else if (g_navHasNoLegalFacing == 1 && g_navRegionFilterDisabled == 1) {
            Vector3 projected = chara ? GetPositionForEntity(chara->entity.node) : Vector3{};
            std::int32_t yaw = g_currentPlayerYaw;
            const std::int32_t state = g_ryoCurrentAnimationState;
            if (control.forcedYawSnap == 1) {
                yaw = Fold16(control.forcedYaw + 0x8000);
            } else if (cpctGetNavYawRecomputeEnabled() && state > kStateStand &&
                (state <= kStateWalk || static_cast<std::uint32_t>(state - kStateIdleAlt) <= 1U)) {
                const std::int32_t delta = Fold16(g_navProbeBaseYaw - g_currentPlayerYaw + g_navDirectionFlags + 0x8000);
                if (delta > 364)
                    g_currentPlayerYaw += 364;
                else if (delta < -364)
                    g_currentPlayerYaw -= 364;
                else
                    g_currentPlayerYaw = Fold16(g_navProbeBaseYaw + g_navDirectionFlags + 0x8000);
                yaw = g_currentPlayerYaw;
            }
            g_navTemporaryYaw = yaw;
            FindNearestControlPointForPos(projected);
            g_currentPlayerYaw = yaw;
            g_playerNavigationPosition = projected;
        } else if (static_cast<std::uint32_t>(g_ryoCurrentAnimationState - kStateFightFallback) <= 1U) {
            control.autoRotateYawDelta = 0;
            control.inputYawDelta = 0;
        } else {
            Vector3 projected = g_playerNavigationPosition;
            g_navTemporaryYaw = control.forcedYawSnap == 1 ? Fold16(control.forcedYaw + 0x8000) : g_currentPlayerYaw;
            std::int32_t yaw = g_navTemporaryYaw;
            if (navUpdateSteeringYawFromResolvedDelta(g_playerNavigationPosition.x, projected) < 0 || speed == 0.0F)
                yaw = Fold16((chara ? NodeChainYaw(&chara->entity.node) : 0) + 0x8000);
            else
                yaw = g_navTemporaryYaw;
            FindNearestControlPointForPos(projected);
            g_currentPlayerYaw = yaw;
            g_playerNavigationPosition = projected;
        }
    }

    const std::int32_t yawDelta = g_currentPlayerYaw - g_previousPlayerYaw;
    g_previousPlayerYaw = g_currentPlayerYaw;
    std::int32_t velocity = yawDelta * 40;
    if (velocity < -32768)
        velocity = -32768;
    if (velocity > 32585)
        velocity = 32585;
    g_playerYawVelocity = velocity;
    const float speedSquared = speed * speed;
    g_playerYawAngularTarget = static_cast<std::int32_t>(
        static_cast<double>(static_cast<float>(g_playerTurnStep) * static_cast<float>(velocity) * 3.0517578e-05F * speedSquared) / 0.04000000000000001);
    g_playerYawAngularSmoothed =
        static_cast<std::int32_t>(static_cast<double>(static_cast<float>(g_playerYawAngularTarget - g_playerYawAngularSmoothed)) * 0.05 +
            static_cast<double>(g_playerYawAngularSmoothed));
    shenGetAndSetPositionFor(chara, motion);

    if (g_navigationTurnEnabled) {
        const TurnMode turnMode = g_currentTurnMode;
        switch (g_turningPhase) {
        case TurnPhase::None: {
            bool checkTurnAnimation = true;
            switch (turnMode) {
            case TurnMode::Mode14:
                if (0.0F < motion.interpolationAmount) {
                    ryoRequestAnimWithTest(kAnimRequestLocomotion);
                } else if (ryoRequestAnimWithTest(kAnimWalkTurn180L)) {
                    g_turningPhase = TurnPhase::TurnFinishing;
                }
                break;
            case TurnMode::Mode16:
                if (ryoRequestAnimWithTest(kAnimRunMidTurn180L)) {
                    g_turningPhase = TurnPhase::TurnFinishing;
                    g_pControlPadController->resetSpeedBlend = 1;
                    g_playerMotion->interpolationAmount = 0.0F;
                }
                break;
            case TurnMode::YawStepPositive:
                g_currentPlayerYaw = Fold16(g_currentPlayerYaw + 546);
                break;
            case TurnMode::YawStepNegative:
                g_currentPlayerYaw = Fold16(g_currentPlayerYaw - 546);
                break;
            default: {
                if (static_cast<std::uint32_t>(static_cast<std::int32_t>(turnMode) - 5) <= 2U) {
                    StartTurnFromTargetYaw();
                    checkTurnAnimation = false;
                    break;
                }
                const std::int32_t animationState = g_navigationAnimationState;
                const auto movement = static_cast<std::uint32_t>(mode);
                if (movement <= 20U && (0x188008U & (1U << movement)) != 0 && static_cast<std::uint32_t>(animationState - 4) > 2U && ryoCanMove()) {
                    ryoRequestAnimWithTest(kAnimRequestLocomotion);
                } else if (turnMode == TurnMode::Mode15) {
                    if (0.0F < g_playerMotion->interpolationAmount) {
                        ryoRequestAnimWithTest(kAnimRequestLocomotion);
                    } else {
                        ryoRequestAnimWithTest(kAnimRequestStand);
                        g_currentTurnMode = TurnMode::Mode0;
                    }
                } else if (static_cast<std::uint32_t>(animationState - 4) <= 1U || ryoCanMove()) {
                    ryoRequestAnimWithTest(kAnimRequestLocomotion);
                } else {
                    ryoRequestAnimWithTest(kAnimRequestStand);
                }
                break;
            }
            }
            if (checkTurnAnimation) {
                const std::int32_t current = g_playerMotion->currentAnimId;
                if (current == kAnimTurn022WadL || current == kAnimTurn022WadR || g_ryoCurrentAnimationState == kStateFight) {
                    g_turningPhase = TurnPhase::TurnAnimActive;
                    g_currentTurnMode = TurnMode::Mode5;
                }
            }
            break;
        }
        case TurnPhase::TargetYawSelected:
            StartTurnFromTargetYaw();
            break;
        case TurnPhase::TurnAnimActive:
            g_currentPlayerYaw = Fold16(motion.extractedYaw + 0x8000);
            if ((g_playerMotion->motionFlags & kMotionStuck) != 0) {
                control.yawOffset = Fold16(control.yawOffset - control.committedTurnAccumYaw);
                control.committedTurnAccumYaw = 0;
                g_forceYawRecalculation = 1;
                ryoRequestAnimWithTest(ryoCanMove() ? kAnimRequestLocomotion : kAnimRequestStand);
                ResetTurnSmoothing();
                g_turningPhase = TurnPhase::None;
            }
            break;
        case TurnPhase::TurnFinishing: {
            const bool requested = ryoRequestAnimWithTest(ryoCanMove() ? kAnimRequestLocomotion : kAnimRequestStand);
            const auto state = static_cast<std::uint32_t>(g_ryoCurrentAnimationState);
            if (state > 10U || (0x5baU & (1U << state)) == 0 || requested) {
                ResetTurnSmoothing();
                if (mode == PlayerMovementState::SnapFailFallback || !ryoCanMove()) {
                    g_playerMovementState = PlayerMovementState::Idle;
                    g_inputStateHoldFrames = 0;
                }
                g_turningPhase = TurnPhase::None;
                if (startAnimation != kAnimWalkStart)
                    g_currentPlayerYaw = Fold16(motion.extractedYaw + 0x8000);
            }
            break;
        }
        }
    }

    if (static_cast<std::uint32_t>(g_turningPhase) <= 1U) {
        if (chara)
            SetLocalYawFromWorld(chara, g_currentPlayerYaw + 0x8000);
        switch (g_ryoCurrentAnimationState) {
        case kStateIdle:
        case kStateWalkTurn180:
        case kStateIdleAlt:
        case kStatePlayRateDirect:
            g_turningPhase = TurnPhase::TurnFinishing;
            g_navSteeringClampIndex = 10;
            break;
        case kStateFight:
            g_navSteeringClampIndex = 10;
            break;
        default:
            break;
        }
    }
    ryoUpdateAnimPlayRateFromState();
    const auto state = static_cast<std::uint32_t>(g_ryoCurrentAnimationState);
    if (g_navHasNoLegalFacing == 1 && state <= 7U && (0x89U & (1U << state)) != 0) {
        if (g_movementBlockedFrames == 0) {
            g_navigationAnimationState = 3;
            g_blockedDirectionMask = static_cast<std::uint16_t>(input.held & 0xf000U);
            g_movementBlockedFrames = 1;
        } else if (g_movementBlockedFrames < 600) {
            ++g_movementBlockedFrames;
        }
    } else {
        g_movementBlockedFrames = 0;
    }
    g_navFilterProjectPositionOnce = motion.trackedNodeIndex != 10 ? 1 : 0;
}

void ryoUpdatePlayerControllerFrame(Character *chara, PlayerControlWork &state, InputSlot *input) {
    if (ryoUpdateTatehizaStateAndControlLock(state, input))
        return;
    if (chara) {
        chara->entity.node.rotation.x = 0;
        chara->entity.node.rotation.z = 0;
    }
    MotionInformationWork &motion = *g_playerMotion;
    motion.motionFlags |= kMotionFrameBlendReady;
    motion.motionFlags &= ~(kMotionUnknown8 | kMotionUnknown20);
    MOTM_GetWork(chara)->flags |= 1U;
    ryoUpdateControllerInputPhase0(chara, state);
    if (filmGetTaskFlag() && NoActiveCharacterWithinOneMetre(chara) && g_sleepPromptPending == 0 && g_cpctLastOperand != 0 && g_cpctOperandEnabled != 0) {
        EnqueueSCNT(g_cpctLastOperand, static_cast<std::int16_t>(-1));
        g_sleepPromptPending = 1;
    }
    state.cameraAccumulatedYaw = static_cast<std::uint32_t>(g_fCamAccumYaw);
    ryoGetYaw_0(chara, state);
    ryoUpdateControllerInputPhase1(chara, state);
    ryoUpdateControllerInputPhase2(chara, state);
    ryoUpdateLocomotionSpeedScalar(chara);
    const Character *const ryo = CurrentRyo();
    float distance = 0.80000001F;
    if ((ryo && (ryo->motionFlags & 4U) != 0) || g_ryoCurrentAnimationState != kStateStand || state.autoRotateYawDelta == 0) {
        ryoUpdateMovementStateInternal(state);
        if (g_ryoCurrentAnimationState == kStateRun)
            distance = 1.2F;
    } else {
        g_playerMovementState = state.autoRotateYawDelta >= 0 ? PlayerMovementState::AutoRotateForcedPositive : PlayerMovementState::AutoRotateForcedNegative;
    }
    distanceFromRyoForCamera = distance;
    if (g_locomotionInitState == LocomotionInit::Pending) {
        g_animPlayRate = 1.0F;
        ryoResetTurnMode();
        cewpUpdatePlayerYaw(chara);
        g_forceYawRecalculation = 0;
        g_navFilterProjectPositionOnce = 0;
        g_locomotionInitState = LocomotionInit::Active;
    } else if (g_locomotionInitState == LocomotionInit::Disabled) {
        return;
    }
    ryoUpdateControllerInputPhase3(chara, state);
    UpdatePlayerInteractionTarget(chara, state);
    ryoUpdateLocomotion(chara, *input);
    if (static_cast<std::int8_t>(state.idleTurnTimer) < 99)
        ++state.idleTurnTimer;
    if (static_cast<std::uint8_t>(state.idleTurnMode - 1) <= 1U && static_cast<std::int8_t>(state.idleTurnTimer) >= 10)
        state.idleTurnMode = 0;
}

void ProcessRequests(std::int32_t request) {
    Character *const ryo = CurrentRyo();
    if (!ryo)
        return;
    PlayerControlWork &control = *g_pControlPadController;
    MotionInformationWork &motion = *g_playerMotion;
    if (control.controlGate >= 0) {
        if (request < 0) {
            const bool defaultMovement = control.controlMode == 2;
            g_playerMovementState = PlayerMovementState::Idle;
            g_inputStateHoldFrames = 0;
            if (defaultMovement)
                ryoUpdateTurnModeEx();
            motion.trackedNodeIndex = 10;
            control.resetSpeedBlend = 1;
            motion.interpolationAmount = 0.0F;
            motion.pendingTurnEulerDelta.y = 0;
            if (control.savedBlendValid != 0) {
                motion.blendMode = control.savedBlendMode;
                motion.blendFrameCount = control.savedBlendFrameCount;
                control.savedBlendValid = 0;
            }
            control.yawOffset = 0;
            control.committedTurnAccumYaw = 0;
            distanceFromRyoForCamera = 0.0F;
        }
        control.controlGate = request;
        return;
    }
    if (request < 0) {
        control.controlGate = request;
        return;
    }
    control.previousAutoRotateYawDelta = 0;
    control.inputYawDelta8 = 0;
    control.previousInputYawPair = 0;
    control.autoRotateYawDelta = 0;
    control.inputYawDelta = 0;
    control.idleTurnMode = 0;
    control.idleTurnIntensity = 0;
    control.idleTurnTimer = 0;
    control.ticks = 0;
    control.yawBlendBackTimer = 0;
    control.navProbeYaw = NodeChainYaw(&ryo->entity.node);
    control.yawDriftTimer = 9999;
    control.footPlantingEnabled = 1;
    control.movementInputActive = 0;
    control.walkVolumeGraceFrames = 9999;
    std::memset(control.reserved018, 0, sizeof(control.reserved018));
    ryo->entity.node.rotation.x = 0;
    ryo->entity.node.rotation.z = 0;
    g_playerMovementState = PlayerMovementState::Idle;
    g_inputStateHoldFrames = 0;
    MOTM_GetWork(ryo)->flags |= 1U;
    motion.pendingTurnEulerDelta.y = 0;
    motion.trackedNodeIndex = 10;
    if (control.controlMode == 2) {
        if (!g_navigationTurnEnabled) {
            ryoCalcCurrentYaw();
            g_navSteeringClampIndex = 10;
            ClearForcedControlPoint();
        }
        g_navigationTurnEnabled = 1;
        ryoResetTurnMode();
        control.applyDeferredTranslation = 0;
        if (motion.currentAnimId == kAnimTatehizaLoop) {
            control.tatehizaState = 3;
        } else {
            control.tatehizaState = 0;
            const std::int32_t state = getNextAnimState_Internal(motion.currentAnimId);
            if ((state & ~kStateRun) != 0 || state == kStateWalkTurn180)
                control.applyDeferredTranslation = 1;
        }
    } else if (control.controlMode <= 1 || control.controlMode == 3) {
        if (!shenLookupAnimationIndex(motion.currentAnimId, 20))
            ProcessRequestedAnimation(motion, control);
        control.tatehizaState = 0;
        control.applyDeferredTranslation = 0;
    }
    control.lookRunMode = 1;
    control.savedBlendValid = 1;
    control.savedBlendMode = motion.blendMode;
    control.savedBlendFrameCount = static_cast<std::uint8_t>(motion.blendFrameCount);
    g_directionChordMask = 0;
    g_directionChordState = 0;
    motion.motionFlags = (motion.motionFlags & ~kMotionUnknown8) | kMotionFrameBlendReady;
    g_turnInputLockoutFrames = 0;
    g_playerNodeYaw = 0;
    g_previousPlayerNodeYaw = 0;
    g_snapFailCounter = 0;
    control.controlGate = request;
}

void CPCT_Process(PlayerControlWork &control) {
    std::int32_t mode = static_cast<std::uint16_t>(control.currentMode);
    if (mode != 0 && control.controlGate < 0)
        mode = 0;
    switch (mode) {
    case 0:
        control.modeState = 0;
        control.currentMode = 0;
        g_processAnimationState = -1;
        break;
    case 3:
        control.modeState = 1;
        g_processAnimationState = -1;
        break;
    case 1:
    case 2:
    case 4:
    case 5: {
        MotionInformationWork &motion = *g_playerMotion;
        const std::int32_t state = g_ryoCurrentAnimationState;
        if (g_processAnimationState == 0 && state == kStateStand && g_playerMovementState == PlayerMovementState::Idle && !motiCheckLocomotionState(&motion)) {
            g_processAnimationState = state;
            if ((control.currentMode & 0xffff0000U) == 0x10000U) {
                control.modeState = 2;
            } else {
                control.modeState = 0;
                control.currentMode = 0;
            }
        } else if (static_cast<std::uint32_t>(mode - 4) <= 1U && (state == kStateFight || state == kStateWalkTurn180)) {
            control.modeState = 0;
            control.currentMode = 0;
            PCCTL_UpdateRyoNavYaw(g_playerCharacter);
            if (mode == 5) {
                motion.motionFlags &= ~kMotionStuck;
                motion.pendingAnimId = kAnimIdleRoot;
                motion.playbackRate = 1.0F;
            }
            g_processAnimationState = state;
        } else {
            control.modeState = 1;
            g_processAnimationState = state;
        }
        break;
    }
    default:
        break;
    }
    const std::uint8_t modeState = control.modeState;
    if (modeState != 0 && (modeState != 1 || (control.currentMode & 0xffff0000U) != 0x10000U || control.state != 1))
        return;
    if (control.state == 1)
        ProcessRequests(0);
    else if (control.state == 2)
        ProcessRequests(-1);
    control.state = 0;
}

void cpctBindMotionWithNavParams(Character *character) {
    const MotionInformationWork *const previousMotion = g_playerMotion;
    g_playerCharacter = character;
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    g_playerMotion = manager ? &manager->motion : nullptr;
    g_playerRootNode = character ? &character->entity.node : nullptr;
    const CharacterCollisionWork *const collision = character ? GetPlayerCollisionWork(character) : nullptr;
    g_playerNavigationClearanceRadius = collision ? collision->radialAabbExtent : 0.40000001F;
    if (!previousMotion && g_playerMotion) {
        PlayerControlWork &control = *g_pControlPadController;
        for (std::size_t index = 0; index < kDefaultPlayerMotionMetadata.size(); ++index)
            control.motionMetadata[index] = kDefaultPlayerMotionMetadata[index];
        SetupRyoBlendAnimations(
            static_cast<std::int32_t>(control.motionMetadata[8].animationId), static_cast<std::int32_t>(control.motionMetadata[19].animationId));
        g_playerMotion->motionCallback = std::bit_cast<void *>(&MOTN_FootPlantingCallback);
        g_playerMotion->motionCallbackUser = character;
        ProcessRequestedAnimation(*g_playerMotion, control);
    }
    g_previousPlayerNodeYaw = g_playerNodeYaw;
    g_playerNodeYaw = character ? NodeChainYaw(&character->entity.node) : 0;
    SelectCharacterMapQuery(character);
}

void cpctSetDefaultIdle(PlayerControlWork &control) {
    control.controlMode = 2;
    control.previousAutoRotateYawDelta = 0;
    control.inputYawDelta8 = 0;
    control.previousInputYawPair = 0;
    control.numActiveFrames = 0;
    control.pressedPriority = 0;
    control.heldInput &= 0xffff0000U;
    control.desiredFacingYaw = 0;
    control.cameraAccumulatedYaw = 0;
    control.flags = 0;
    std::memset(control.reserved016, 0, sizeof(control.reserved016));
    std::memset(control.reserved018, 0, sizeof(control.reserved018));
    control.controlGate = -1;
    control.state = 0;
    control.turnIntensitySmoothed = 0.0F;
    control.turnBurstAccum = 0.0F;
    control.idleLoopFrames = 0;
    control.idleLoopTarget = 1;
    control.savedBlendValid = 0;
    control.savedBlendMode = 1;
    control.savedBlendFrameCount = 5;
    control.rightAngleX = 0;
    control.rightAngleY = 0;
    control.stateFlags = 0;
    control.scriptState = 0;
    control.forcedYaw = 0;
    control.navProbeYawLastStable = 0;
    control.autoRotateYawDelta = 0;
    control.inputYawDelta = 0;
    control.action = 0;
    control.reserved1d6 = std::byte{};
    control.forcedYawSnap = 0;
    control.modeState = 0;
    control.yawOffset = 0;
    control.committedTurnAccumYaw = 0;
    control.startAnimationSpeed = 0.0F;
    control.speedAdjustWindow = 1.0F;
    control.speedTime = 0.0F;
    control.resetSpeedBlend = 1;
    control.applyDeferredTranslation = 0;
    control.currentMode = 0;
    control.blockedCanControl = 0;
    control.isRunning = 0;
    control.wasRunning = 0;
    control.turnAngleAdjustment = 0;
    control.turnAdjustSmoothed = 0;
    control.turnVelocity = 0;
    control.hasNavProbeYawTarget = 1;
    control.tatehizaState = 0;
    control.idleTurnActive &= ~0xff;
    control.secondaryYawOffset = 0;
    control.inputSource = nullptr;
    control.allowTurnInPlace = 0;
    control.currentFrameCount = 0;
    control.idleTurnCounter = 0;
    control.enableNavYawRecompute = 0;
    control.facingProbeClear = 0;
    control.newYaw = 0;
    control.facingYawDelta = 0;
    control.lateralSnapYaw = 0;
    control.idleTurnMode = 0;
    control.idleTurnIntensity = 0;
    control.idleTurnTimer = 0;
    control.ticks = 0;
    control.navProbeYaw = 0;
    control.yawBlendBackTimer = 0;
    control.yawDriftTimer = 9999;
    control.footPlantingEnabled = 1;
    control.movementInputActive = 0;
    control.fightOnly = 0;
    control.resolvedMovementSpeed = 0.0F;
    control.runOverride = 0;
    control.ticksHeld = 0;
    control.busy = 0;
    control.loadHoldActive = 0;
    control.reserved25f = std::byte{};
    control.walkVolumeGraceFrames = 9999;
    control.inputMode = 0;
    control.speedScale = 0.0F;
    control.areaId = 0;
}

void ryoSetInitialControl(Character *character) {
    SetShouldSleepFlag(1);
    g_animPlayRate = 1.0F;
    g_scenarioControllerConnected = 1;
    g_sleepPromptPending = 0;
    g_cpctLastOperand = 0;
    g_cpctOperandEnabled = 1;
    g_cpctInputUpdatePending = 0;
    g_cpctPreviousOperand = 0;
    g_cpctSleepPromptReady = true;
    ryoResetTurnMode();
    cewpUpdatePlayerYaw(character);
    g_playerStats.runtimeFlags &= ~1U;
    g_forceYawRecalculation = 0;
    g_navFilterProjectPositionOnce = 0;
    InputSlot &input = g_inputState.slots[0];
    input.held = 0;
    input.previousHeld = 0;
    input.pressedPriority = 0;
    input.repeatedPresses = 0;
    input.rightStick = 0x8080U;
    input.leftStick &= 0xffff0000U;
    input.triggers = 0;
    if (PadCaptureEntry *const pad = CurrentMapPad()) {
        pad->held = 0;
        pad->previousHeld = 0;
        pad->pressedPriority = 0;
        pad->repeatedPresses = 0;
        pad->rightStick = 0x8080U;
        pad->leftStick &= 0xffff0000U;
        pad->triggers = 0;
    }
    if (g_needsDestroyForSleepPrompt) {
        g_needsDestroyForSleepPrompt = false;
        DestroyTMEX();
        shenRefreshUI();
        UI_ResetConversationDetailsState();
        shenExecuteAICACommand(0x30babU, 0, 0);
        SetStatOrNotebookFlag(0x320U, false);
        SetStatOrNotebookFlag(0x0c2U, false);
        g_TMEX_Flag = 5;
        g_tmex_val = 0;
        g_TMEX_CurrentMode = 0;
        g_tmexValHigh = 0;
    }
}

Character *FindContextActionCharacter(std::uint32_t id) {
    MapControlData *const mapControl = CurrentMapControl();
    return mapControl && mapControl->scenarioTask ? CHID_Find(mapControl->scenarioTask, id) : nullptr;
}

void SetContextActionObjectVisible(Character *object, bool visible) {
    if (!object)
        return;
    const bool wasVisible = (object->motionFlags & 4U) != 0;
    if (visible)
        object->motionFlags |= 4U;
    else
        object->motionFlags &= static_cast<std::uint8_t>(~4U);
    if (g_bDebugLogVisibility && (object->flags.value & CharacterFlagHasCpct) != 0 && wasVisible != visible)
        CHAR_LogNPCVisibility(object, visible);
}

void BeginContextActionSecondary(PlayerControlWork &control, std::uint16_t animationId, std::int16_t nodeId, bool mirrored) {
    if (!g_playerMotion || g_playerMotion->basePoseTemplateIndex > 20) {
        control.reserved1d6 = std::byte{1};
        return;
    }
    constexpr std::uint32_t kSecondaryMirrorMismatch = 0x00100000U;
    const bool matching = g_playerMotion->secondaryAnimId == animationId && g_playerMotion->secondaryNodeId == nodeId &&
        g_playerMotion->secondaryFrameStart == 0.0F && ((g_playerMotion->motionFlags & kSecondaryMirrorMismatch) != 0) == mirrored;
    if (!matching) {
        MOTN_PlaySecondaryMotion(*g_playerMotion, animationId, nodeId, 0.0F, -1.0F, mirrored ? 1U : 0U);
    }
    control.reserved1d6 = std::byte{1};
}

void AttachContextActionObject(Character *object, Character *owner, std::int32_t nodeId, const Vector3 &offset) {
    if (!object || !owner)
        return;
    fixoSetCharaPos(object, owner, nodeId, &offset, nullptr);
}

void CompleteContextActionAttachment(Character *object, Character *owner, std::int32_t nodeId, const Vector3 &offset) {
    AttachContextActionObject(object, owner, nodeId, offset);
    SetContextActionObjectVisible(object, true);
}

void ResetContextActionHands(Character *character) {
    mhndApplyCharaHandPose(character, 2, 14, 1);
}

void StopContextAction(Character *character, std::uint16_t action) {
    if (action == 2) {
        if (g_playerMotion)
            MOTN_RequestSecondaryMotionStop(*g_playerMotion);
        ResetContextActionHands(character);
        return;
    }

    Character *object = nullptr;
    switch (action) {
    case 1:
        object = FindContextActionCharacter(SHEN_NAME(ABAG));
        break;
    case 3:
        object = FindContextActionCharacter(SHEN_NAME(LIGH));
        break;
    case 4:
        object = FindContextActionCharacter(SHEN_NAME(RUCO));
        break;
    case 5:
        object = FindContextActionCharacter(SHEN_NAME(RUCF));
        break;
    default:
        return;
    }

    if (!object)
        return;
    if (action != 3 && (object->motionFlags & 4U) == 0)
        return;

    fixoClearAttachment(object);
    if (action == 3) {
        if (!g_playerMotion)
            return;
        MOTN_RequestSecondaryMotionStop(*g_playerMotion);
        ResetContextActionHands(character);
        SetContextActionObjectVisible(object, false);
        return;
    }

    if (action != 5 && g_playerMotion)
        MOTN_RequestSecondaryMotionStop(*g_playerMotion);
    if (action != 5)
        ResetContextActionHands(character);

    const Vector3 position = GetPositionForEntity(object->entity.node);
    object->skipRootMotionDelta = 1;
    object->entity.node.position = {position.x, -0.30000001F, position.z};
    object->entity.node.rotation = {static_cast<std::int16_t>(47136), 0, static_cast<std::int16_t>(32768)};
    if (MotionManagerWork *const manager = MOTM_GetWork(object))
        manager->motion.entityEuler = object->entity.node.rotation;
}

void StartOrContinueContextAction(Character *character, PlayerControlWork &control, std::uint16_t action) {
    switch (action) {
    case 1:
    case 4: {
        const std::uint32_t id = action == 1 ? SHEN_NAME(ABAG) : SHEN_NAME(RUCO);
        Character *const object = FindContextActionCharacter(id);
        if (control.reserved1d6 == std::byte{}) {
            fixoClearAttachment(object);
            BeginContextActionSecondary(control, 657, 14, true);
            return;
        }
        const Vector3 offset = action == 4 ? Vector3{-0.030999999F, 0.34799999F, -0.088F} : Vector3{0.079999998F, 0.0F, -0.025F};
        AttachContextActionObject(object, character, 18, offset);
        mhndApplyCharaHandPose(character, 0, 0, 16);
        mhndApplyCharaHandPose(character, 1, 8, 16);
        SetContextActionObjectVisible(object, true);
        control.reserved1d6 = std::byte{};
        return;
    }
    case 2:
        if (control.reserved1d6 == std::byte{}) {
            BeginContextActionSecondary(control, 1143, 14, false);
            return;
        }
        mhndApplyCharaHandPose(character, 0, 0, 16);
        control.reserved1d6 = std::byte{};
        return;
    case 3: {
        Character *const object = FindContextActionCharacter(SHEN_NAME(LIGH));
        if (control.reserved1d6 == std::byte{}) {
            fixoClearAttachment(object);
            BeginContextActionSecondary(control, 1139, 8, true);
            return;
        }
        constexpr Vector3 kLanternOffset{0.079999998F, 0.0F, 0.025F};
        CompleteContextActionAttachment(object, character, 12, kLanternOffset);
        control.reserved1d6 = std::byte{};
        return;
    }
    case 5: {
        Character *const object = FindContextActionCharacter(SHEN_NAME(RUCF));
        fixoClearAttachment(object);
        constexpr Vector3 kObjectOffset{0.287F, 0.178F, 0.0F};
        CompleteContextActionAttachment(object, character, 2, kObjectOffset);
        return;
    }
    default:
        return;
    }
}

void UpdatePlayerContextAction(Character *character, PlayerControlWork &control) {
    if (control.reserved1d6 != std::byte{}) {
        StartOrContinueContextAction(character, control, control.action);
        return;
    }
    if ((control.flags & 0x0400U) == 0)
        return;

    std::uint16_t action = static_cast<std::uint16_t>(g_playerStats.contextActionMode);
    std::uint32_t requiredCharacter = 0;
    switch (action) {
    case 1:
        requiredCharacter = SHEN_NAME(ABAG);
        break;
    case 3:
        requiredCharacter = SHEN_NAME(LIGH);
        break;
    case 4:
        requiredCharacter = SHEN_NAME(RUCO);
        break;
    case 5:
        requiredCharacter = SHEN_NAME(RUCF);
        break;
    default:
        break;
    }
    if (requiredCharacter && !FindContextActionCharacter(requiredCharacter)) {
        action = 0;
    }
    if (action == control.action)
        return;
    StopContextAction(character, control.action);
    control.action = action;
    StartOrContinueContextAction(character, control, action);
}

void UpdateContextActionRootMotionTarget(Character *character) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    MotionInformationWork &motion = manager->motion;
    motion.rootAngleFlags |= 0x01000000U;

    Vector3 headDirection{};
    float headScale = 0.0F;
    const MotionNodeRuntime *const headTarget = FindPreviousMotionNode(motion, 6U);
    const MotionNodeRuntime *const headBase = FindPreviousMotionNode(motion, 4U);
    if (headTarget && headTarget->transform && headBase && headBase->transform) {
        headDirection = {headTarget->transform->rows[3].x - headBase->transform->rows[3].x, headTarget->transform->rows[3].y - headBase->transform->rows[3].y,
            headTarget->transform->rows[3].z - headBase->transform->rows[3].z};
        headScale = 0.25F / std::sqrt(headDirection.x * headDirection.x + headDirection.y * headDirection.y + headDirection.z * headDirection.z);
    }

    const float targetSpeed = std::max(0.0F, 0.30000001F - g_ryoRawMovementInputSpeed);
    g_ryoContextMotionSpeed += std::clamp(targetSpeed - g_ryoContextMotionSpeed, -0.02F, 0.059999999F);
    const float speed = g_ryoContextMotionSpeed - 0.15000001F;

    Vector3 origin = GetPositionForEntity(character->entity.node);
    if (const MotionNodeRuntime *const knee = FindPreviousMotionNode(motion, 9U); knee && knee->transform) {
        origin = {knee->transform->rows[3].x, knee->transform->rows[3].y, knee->transform->rows[3].z};
    }

    const std::int32_t yaw = -charaGetWorldYaw(character);
    const std::uint16_t unsignedYaw = static_cast<std::uint16_t>(yaw);
    const std::int16_t foldedYaw = unsignedYaw <= 0x8000U ? static_cast<std::int16_t>(yaw) : static_cast<std::int16_t>(-static_cast<std::int16_t>(yaw));
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedYaw));
    motion.targetPositions[3] = {origin.x + headDirection.x * headScale - (sine * speed + cosine * 0.050000001F),
        origin.y + headDirection.y * headScale - 0.25F, origin.z + cosine * speed - sine * 0.050000001F + headDirection.z * headScale};
    motion.localTargets[3] = nullptr;
    motion.worldTargets[3] = &motion.targetPositions[3];
}

void CPCT_Callback(Character *character, void *payload) {
    auto *const control = static_cast<PlayerControlWork *>(payload);
    if (!character || !control || character != CurrentRyo())
        return;
    cpctBindMotionWithNavParams(character);
    CEWP_ResetPointTestCache(character);
    PadCaptureEntry *const pad = CurrentMapPad();
    std::uint32_t inputMode = ~pad->flags & 2U;
    if (g_isPlayerGameStateActive == 0)
        inputMode |= 0x20U;
    if (g_notebookHelpOverrideActive)
        inputMode |= 0x40U;
    if (control->controlMode == 2 && (control->inputMode & 0x60U) != 0 && inputMode == 0)
        control->ticksHeld = 2;
    control->inputMode = inputMode;
    TryStartTurnInPlaceAnimation();
    shenForceCancel(control);
    SCEN_UpdatePlayerSleepScheduler(*control);
    SCEN_UpdateTimedTransitionScheduler();
    CPCT_Process(*control);
    UpdateLoadHoldInputLatch(*control);
    InputSlot *const input = cpctRefreshInputFromPad(*control, *CurrentMapPad());
    UpdatePlayerContextAction(character, *control);
    if (ShouldRun() && g_playerStats.contextActionMode == 3)
        UpdateContextActionRootMotionTarget(character);
    const PlayerControlWork &player = *g_pControlPadController;
    if (control->controlGate < 0 && player.modeState == 0) {
        control->flags &= static_cast<std::uint16_t>(~0x0059U);
        return;
    }
    if (static_cast<std::uint16_t>(player.currentMode) == 2 || g_tmex_val == 6U)
        return;
    if (static_cast<std::int32_t>(control->ticks) < 99999)
        ++control->ticks;
    UpdateAuthorSearchControl(*control);
    ApplyDeferredPlayerTranslation(character, *control);
    if (control->controlMode == 0 || control->controlMode == 2)
        ryoUpdatePlayerControllerFrame(character, *control, input);
    else if (control->controlMode == 3)
        ryoUpdateFollower(character, input);
    if (g_sleepPromptPending != 0 && SCEN_TryFinalizeReadyRuntimeObject())
        g_sleepPromptPending = 0;
    SCEN_FinalizeReadyPlayerControlRuntimeObjects();
}

} // namespace

bool cpctIsTatehizaStateActive() {
    const PlayerControlWork *const control = PlayerControl();
    return control && control->tatehizaState != 0;
}

bool PlayerEffectLightUsesAccumulatedYaw() {
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(g_turnYawResolveMode) - 4) <= 1U;
}



bool filmGetTaskFlag() {
    if (!g_pControlPadController || (g_pControlPadController->flags & 0x40U) == 0)
        return false;
    g_pControlPadController->flags = static_cast<std::uint16_t>(g_pControlPadController->flags & ~0x40U);
    MapControlData *const control = CurrentMapControl();
    if (!ShouldRun() || g_searchCandidateId >= 0)
        return false;
    const bool blockedSearchState =
        ((g_currentSearchState - 2) & ~3) == 0 && g_currentSearchState != static_cast<std::int32_t>(SearchState::AutoCharacterLockOn);
    if (blockedSearchState || camCtrlGetState() > 0 || g_isPlayerGameStateActive == 0 || g_notebookHelpOverrideActive || !UI_ICSpIsIdle() ||
        g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_NONE || g_NumFramesProcessed != 0)
        return false;
    const auto *const film = control->filmTask && control->filmTask->task_data_ptr ?
        reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4) :
        nullptr;
    return film && film->current[0] == 0;
}

std::int32_t PCCTL_GetForcedControlMode() {
    return g_pControlPadController && g_pControlPadController->controlMode == 2 ? g_forcedControlMode : 0;
}

void PCCTL_SetSkipNavRegionFiltering(bool skip) {
    g_skipNavRegionFiltering = skip ? 1 : 0;
}

std::uint32_t PCCTL_GetCurrentControlPointCode() {
    return g_currentControlPoint ? g_currentControlPointCode : 0;
}

Vector3 PCCTL_GetCurrentControlPointPosition() {
    return g_currentControlPoint ? g_currentControlPointPosition : Vector3{};
}

void PCCTL_SetScriptWalkMode(std::uint32_t mode) {
    if (!g_pControlPadController)
        return;
    if (mode == 2) {
        g_pControlPadController->flags = static_cast<std::uint16_t>((g_pControlPadController->flags & ~kForceWalk) | kScriptAlternateMode);
        return;
    }
    if (static_cast<std::uint8_t>(mode) != 0)
        g_pControlPadController->flags |= kForceWalk;
    else
        g_pControlPadController->flags &= ~kForceWalk;
    g_pControlPadController->flags &= ~kScriptAlternateMode;
}

void CPCT_RebindMotionWithNavParams(Character *character) {
    cpctBindMotionWithNavParams(character);
}

void PCCTL_BeginLoadHold() {
    PlayerControlWork *const control = PlayerControl();
    if (!control)
        return;
    g_cpctLoadHoldFrameCount = 0;
    control->loadHoldActive = 1;
}

void PCCTL_ResetForEventCamera(Character *character) {
    if (!character || character != CurrentRyo() || !g_pControlPadController || !g_playerMotion)
        return;
    g_pControlPadController->modeState = 0;
    g_pControlPadController->currentMode = 0;
    CPCT_Process(*g_pControlPadController);
}

std::uint8_t shenForceCancel(PlayerControlWork *control) {
    TMEXRuntimeState &tmex = TMEX_GetRuntimeState();
    if (!control || g_TMEX_Flag == 3U || g_TMEX_Flag == 5U || g_tmex_val == 0U)
        return g_tmex_val;

    MapControlData *const map = CurrentMapControl();
    if ((map && (map->flags & 1U) != 0) || g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_NONE)
        return g_tmex_val;

    PadCaptureEntry *const pad = CurrentMapPad();
    const auto padReady = [pad] { return pad && (pad->flags & 2U) != 0; };
    const auto filmColourActive = [map] {
        const HLib::Task *const film = map ? map->filmTask : nullptr;
        return film && film->task_data_ptr &&
            reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(film->task_data_ptr) + 4)->current[0] != 0;
    };
    const auto playNavigateSound = [] { shenExecuteAICACommand(0x00000babU, 0, 0); };
    const auto resetConversationPresentation = [] {
        shenRefreshUI();
        UI_ResetConversationDetailsState();
    };
    const auto startWatchMotion = [&] {
        g_tmexTransitionFrames = 1;
        if (!g_playerMotion || g_playerMotion->secondaryAnimId != 0U) {
            g_tmex_val = 11;
            return;
        }
        if (g_playerMotion->basePoseTemplateIndex <= 20 && MOTN_PlaySecondaryMotion(*g_playerMotion, kTmexWatchAnimation, 1, 0.0F, -1.0F, 0)) {
            g_tmex_val = 6;
        } else {
            g_tmex_val = 11;
        }
    };
    const auto enqueueWatch = [&] {
        d3t::ResetElementByIdx(kUiGameMenu);
        d3t::ResetElementByIdx(kUiSubMenuCassettes);
        d3t::ResetElementByIdx(kUiGameMenuBackground);
        EnqueueTMEX();
        g_tmex_val = 12;
    };

    if (shenHasFocusedNotLockedOn()) {
        if (g_isPlayerGameStateActive == 0)
            resetConversationPresentation();
        InjectTMEXCancelInput();
        if (g_tmex_val != 7U) {
            if (camCtrlGetState() > 0) {
                g_tmex_val = 14;
            } else {
                if (g_TMEX_Flag == 2U)
                    playNavigateSound();
                g_tmex_val = 7;
            }
        }
    } else if (g_TMEX_State != 0) {
        if (g_TMEX_Flag == 0U) {
            TMEXUtilities(static_cast<std::uint32_t>(TMEXUtilityMode::CloseSetResult0));
        } else if (g_TMEX_Flag <= 2U) {
            if (g_tmex_val == 2U) {
                if (g_TMEX_Flag == 2U)
                    playNavigateSound();
                g_tmex_val = 9;
                g_suppressDefaultHudPromptIcons = 1;
                control->blockedCanControl = 0;
                control->state = 2;
                g_tmexValHigh = 0;
            }
            if (g_playerMotion && g_playerMotion->secondaryAnimId == kTmexWatchAnimation) {
                if (g_tmex_val == 13U)
                    MOTN_RequestSecondaryMotionStop(*g_playerMotion);
                else
                    MOTN_ClearSecondaryMotion(*g_playerMotion);
            }
        }
    } else if (g_isPlayerGameStateActive == 0) {
        resetConversationPresentation();
        if (g_tmex_val == 2U && g_TMEX_Flag == 2U)
            playNavigateSound();
        g_tmex_val = 8;
    } else if (control->blockedCanControl == 1U && g_notebookHelpOverrideActive != 0U) {
        shenEndNotebook();
        UI_SICNClearState(0x0dffU);
        if (g_tmex_val == 2U && g_TMEX_Flag == 2U)
            playNavigateSound();
        g_tmex_val = 10;
    } else if (TMEXModalOwnsCancelInput()) {
        InjectTMEXCancelInput();
        if (g_tmex_val == 2U && g_TMEX_Flag == 2U)
            playNavigateSound();
        g_tmex_val = 7;
    } else if (control->blockedCanControl == 1U && padReady() && !SCEN_HasActiveScript()) {
        constexpr std::uint32_t validStates = (1U << 2U) | (1U << 3U) | (1U << 4U) | (1U << 7U) | (1U << 14U);
        if (g_tmex_val <= 14U && (validStates & (1U << g_tmex_val)) != 0U) {
            if (ShenmueFlagIsSet(kTmexSleepTransitionFlag) || g_NumFramesProcessed != 0 || filmColourActive()) {
                InjectTMEXCancelInput();
            } else if (control->controlGate >= 0) {
                if (g_TMEX_Flag == 2U && g_tmex_val != 3U && g_tmex_val != 7U && g_tmexRequestFrames < 360U)
                    playNavigateSound();
                PCCTL_RequestNewMode(1);
                g_suppressDefaultHudPromptIcons = 1;
                g_pControlPadController->blockedCanControl = 0;
                g_pControlPadController->state = 2;
                g_tmex_val = 5;
                g_tmexValHigh = 0;
            }
        }
    } else if (g_tmex_val == 2U && control->blockedCanControl != 1U && g_tmexValHigh != 0U) {
        g_tmex_val = 15;
    } else if (g_TMEX_Flag >= 1U && g_TMEX_Flag <= 2U && g_tmex_val == 2U) {
        if (g_TMEX_Flag == 2U && (g_scenarioRuntimeFlags & 0x00020000U) != 0U) {
            playNavigateSound();
            g_tmex_val = 3;
        } else {
            g_tmex_val = 4;
        }
    }

    constexpr std::int32_t kAuthorScripted = 9;
    if (g_TMEX_Flag == 0U) {
        bool abort = false;
        switch (g_tmex_val) {
        case 0:
        case 1:
            break;
        case 5:
            abort = !g_pControlPadController || g_pControlPadController->modeState == 0;
            break;
        case 7:
        case 14:
            abort = control->blockedCanControl == 1U && padReady() && g_nAuthorState != kAuthorScripted && g_tmexAuthorFrames >= 25U;
            break;
        case 8:
            abort = control->blockedCanControl == 1U;
            break;
        case 10:
            abort = padReady();
            break;
        case 15:
            abort = true;
            break;
        default:
            abort = control->blockedCanControl == 1U && padReady() && g_nAuthorState == 0 && !SCEN_HasActiveScript();
            break;
        }
        if (abort)
            AbortTMEXForceCancel(*control, tmex);
    } else if (g_TMEX_Flag <= 2U) {
        bool startMotion = false;
        bool enqueue = false;
        switch (g_tmex_val) {
        case 5:
            startMotion = (!g_pControlPadController || g_pControlPadController->modeState == 0) && g_tmexRequestFrames >= 25U;
            break;
        case 7:
        case 15:
            startMotion = true;
            break;
        case 14:
            startMotion = control->blockedCanControl == 1U && padReady() && g_nAuthorState != kAuthorScripted && g_tmexAuthorFrames >= 25U;
            break;
        case 6:
            if (g_NumFramesProcessed == 0 && !filmColourActive() && (!g_playerMotion || g_playerMotion->secondaryAnimId == 0U))
                startMotion = true;
            else
                enqueue = g_playerMotion && g_playerMotion->secondaryFrameTimer > 35.0F;
            break;
        case 8:
            enqueue = control->blockedCanControl == 1U && padReady();
            break;
        case 10:
            enqueue = padReady();
            break;
        case 11:
            enqueue = g_tmexTransitionFrames > 60U;
            break;
        case 9:
            if (g_tmexRequestFrames >= 60U || g_tmexTransitionFrames > 85U)
                g_tmex_val = 12;
            break;
        default:
            break;
        }
        if (!startMotion && !enqueue && g_tmex_val != 12U && g_tmexTransitionFrames > 85U)
            enqueue = true;

        if (startMotion) {
            startWatchMotion();
        } else {
            if (enqueue)
                enqueueWatch();
            if (g_tmex_val == 12U) {
                if (static_cast<std::uint8_t>(g_currentDateTime.hours - 6U) <= 11U) {
                    if (g_TMEX_CurrentMode > 42U)
                        AbortTMEXForceCancel(*control, tmex);
                } else if (g_TMEX_CurrentMode <= 68U) {
                    if (g_TMEX_CurrentMode > 42U)
                        g_TMEX_forceShowWatchNode162 = 1;
                } else {
                    AbortTMEXForceCancel(*control, tmex);
                }
            }
        }
    }

    if (g_tmexTransitionFrames > 0U)
        ++g_tmexTransitionFrames;
    ++g_tmexRequestFrames;
    if (g_TMEX_CurrentMode > 0U && g_TMEX_State != 0)
        ++g_TMEX_CurrentMode;
    g_tmexAuthorFrames = g_nAuthorState == 0 ? g_tmexAuthorFrames + 1U : 0U;
    return static_cast<std::uint8_t>(g_tmexAuthorFrames);
}

void PCCTL_RequestNewMode(std::int32_t mode) {
    PlayerControlWork *const control = PlayerControl();
    if (!control || !g_playerMotion)
        return;
    if (mode != 0 && control->controlGate < 0)
        return;
    control->currentMode = static_cast<std::uint32_t>(mode);
    const auto requested = static_cast<std::uint16_t>(mode);
    if (requested == 0) {
        control->modeState = 0;
        control->currentMode = 0;
    } else if (requested == 2) {
        ProcessRequestedAnimation(*g_playerMotion, *control);
    }
    CPCT_Process(*control);
}

void PCCTL_DisablePlayerControl() {
    g_suppressDefaultHudPromptIcons = 1;
    if (g_pControlPadController) {
        g_pControlPadController->blockedCanControl = 0;
        g_pControlPadController->state = 2;
    }
    g_tmexValHigh = 0;
}

void PCCTL_ResetAuthAndInputStates() {
    g_suppressDefaultHudPromptIcons = 0;
    CAME_ClearSequenceActive();
    g_pControlPadController->blockedCanControl = 1;
    g_pControlPadController->state = 1;
    constexpr std::uint16_t kRetainedInputMask = 0xf1f9U;
    InputSlot &input = g_inputState.slots[0];
    input.held &= kRetainedInputMask;
    input.previousHeld &= kRetainedInputMask;
    input.pressedPriority &= kRetainedInputMask;
    input.repeatedPresses &= kRetainedInputMask;
    PadCaptureEntry *const pad = CurrentMapPad();
    pad->held &= kRetainedInputMask;
    pad->previousHeld &= kRetainedInputMask;
    pad->pressedPriority &= kRetainedInputMask;
    pad->repeatedPresses &= kRetainedInputMask;
}

void PCCTL_EnablePlayerControl() {
    if (!g_pControlPadController)
        return;
    g_pControlPadController->modeState = 0;
    g_pControlPadController->runOverride = 0;
    g_pControlPadController->blockedCanControl = 1;
    g_pControlPadController->state = 1;
    g_suppressDefaultHudPromptIcons = 0;
}

PlayerControlWork *AllocateCPCT(Character *character) {
    auto *const control = static_cast<PlayerControlWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(CPCT), sizeof(PlayerControlWork), CPCT_Callback));
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, SHEN_NAME(CPCT)) : nullptr;
    g_pControlPadController = component ? reinterpret_cast<PlayerControlWork *>(component + 1) : nullptr;
    g_playerMotion = nullptr;
    cpctBindMotionWithNavParams(character);
    CurrentMapControl()->ryo = character;
    if (component)
        component->flags |= 1U;
    cpctSetDefaultIdle(*control);
    ryoSetInitialControl(character);
    return control;
}

void CPCT_Toggle(Character *character) {
    PCCTL_DisablePlayerControl();
    CurrentMapControl()->ryo = character;
    if (character) {
        if (CharacterComponentNode *const cpct = HLib_GetCharacterComponent(character, SHEN_NAME(CPCT)))
            cpct->flags |= 1U;
    }
}

void PCCTL_UpdateRyoNavYaw(Character *character) {
    if (!character)
        return;
    const bool isRyo = CurrentRyo() == character;
    if (isRyo && (g_ryoCurrentAnimationState & ~6) == 0 && g_ryoCurrentAnimationState != 4)
        return;

    std::int16_t yaw = static_cast<std::int16_t>(charaGetWorldYaw(character));
    for (CharacterNode *node = character->entity.node.child; node; node = node->child)
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(node->rotation.y));
    character->entity.node.rotation.y = yaw;
}

void PCCTL_MoveToHome() {
    const auto flagIsSet = [](std::uint16_t flag) { return (g_shenmueFlags[flag >> 3U] & static_cast<std::uint8_t>(1U << (flag & 7U))) != 0; };

    DetermineActNum();
    std::uint32_t destinationArea = g_homeAreaId.name;
    std::uint32_t destinationEntry = g_requestedEntryPoint;
    if (flagIsSet(280) && !flagIsSet(283)) {
        destinationArea = SHEN_NAME(JHD0);
        destinationEntry = 1;
    } else {
        const bool ordinaryHome = flagIsSet(790) || flagIsSet(500) || (g_currentDateTime.month == 12 && g_currentDateTime.day < 26) ||
            g_currentDateTime.hours != 23 || g_areaId == SHEN_NAME(JOMO) || g_areaId == SHEN_NAME(JHD0) || g_areaId == SHEN_NAME(YDB1) ||
            g_areaId == SHEN_NAME(JU00) || (!flagIsSet(160) || flagIsSet(345)) && !flagIsSet(350);
        if (!ordinaryHome) {
            destinationArea = SHEN_NAME(JD00);
            destinationEntry = 30;
        }
    }
    LoadGameModule(SystemModule::Map, g_actNumber, destinationArea, destinationEntry);
}

} // namespace shenmue::s1
