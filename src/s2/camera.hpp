#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {
struct Character;
struct MT7Node;
struct MapCellOffset;
namespace HLib {
struct Task;
}

struct CameraRuntime;
enum class CameraState : std::int32_t {
    Init = 0,
    Locked = 1,
    SnapYaw = 2,
    Snap = 3,
    Scripted = 6,
    Conversation = 7,
    SplineInterpolation = 8,
    Event = 9,
    ChaseGoal = 10,
    InterpolationFollow = 14,
    Unknown15 = 15,
    LookAround = 22,
    Unknown23 = 23,
    LockedOn = 24,
    InterpolationFlagOnly = 25,
    Callback = 26,
    SnapYawTarget = 27,
    Default = -1,
};

enum class CameraComputeMode : std::uint8_t {
    UsePreviousPosition = 0,
    FixedPositionLookAtTarget = 1,
    OrbitTarget = 2,
    AnchorOffset = 3,
    Unknown4 = 4,
};

enum class CameraInterpolationMode : std::int16_t {
    Default = -1,
    None = 0,
    RestorePrevious = 1,
    PreviousToCurrent = 3,
    ToTargetContinuous = 7,
    ToTargetTimed = 8,
};

// Independent of CAMT's interpolation mode: E10C80 drives field/fight motion.
enum class CameraMotionMode : std::uint8_t {
    Default = 0,
    Default1 = 1,
    FieldFight = 2,
    FirstPerson = 3,
    AutoQuadratic = 4,
    MotionUpdate = 5,
    MotionInterp = 6,
    Environment = 7,
    AutoQuadratic2 = 8
};
CameraMotionMode camCtrlGetInterpMode() noexcept;
void camCtrlSetInterpModeTo(CameraMotionMode mode);
struct VFActor;
struct VFMA_t;
void resetIntersectionAndTest();
void resetCharaPosYandIntersection();
bool camCtrlCheckIsEventCam() noexcept;
void camCtrlSetSplineShouldRebuildRotTo(bool enabled) noexcept;
void camCtrlSetSkipInterpTo(std::int32_t enabled) noexcept;
void camCtrlSetXform(const Matrix4x4 *transform) noexcept;
CameraRuntime *camCtrlSetRotationXYZ(float x, float y, float duration) noexcept;
void camCtrlDisableFilmBars() noexcept;
void vfSetupLinkedFightCamera(VFActor *actor, std::uint8_t kind);
void vfUnknownCamRelated(VFMA_t *game);
void camCtrlSetFightDistance(float distance);
void camCtrlSetFightHeight(float height);
void camCtrlEvaluateGoal();
void ResetMotionCameraWaypoints();
void AppendMotionCameraWaypoint(const Vector3 &point);
void BeginMotionInterpolationCamera(Character *target);
bool BeginMotionUpdateCamera(Character *target);

struct EulerRotation16 {
    std::int16_t pitch;
    std::int16_t yaw;
    std::int16_t roll;
};

#pragma pack(push, 1)
struct CameraPreset {
    std::uint32_t tag;
    Vector3 position;
    EulerRotation16 rotation;
    std::uint16_t fovAngle;
    std::byte reserved018[0x10];
    Matrix4x4 projectionMatrix;
    Matrix4x4 viewMatrix;
    Vector3 backwardsDirection;
    std::byte reserved0b4[0x14];
};
#pragma pack(pop)
static_assert(sizeof(CameraPreset) == 0xc8);
static_assert(offsetof(CameraPreset, projectionMatrix) == 0x28);
static_assert(offsetof(CameraPreset, backwardsDirection) == 0xa8);
extern CameraPreset g_cameraCachedPresets[3];

inline constexpr float CAMERA_MIN_MOTION_SCALE_THRESHOLD = 0.0f;
inline constexpr float CAMERA_DEFAULT_MAX_MOTION_SCALE = 0.083999999f;
inline constexpr float CAMERA_DEFAULT_MIN_MOTION_SCALE = 0.093999997f;
inline constexpr float CAMERA_CLOSE_MAX_MOTION_SCALE = 0.001f;
inline constexpr float CAMERA_CLOSE_MIN_MOTION_SCALE = 0.0020000001f;

#pragma pack(push, 1)
struct CameraRuntime {
    std::uint32_t flags;
    CameraState state;
    CameraState previousState;
    std::int16_t reserved0c;
    std::int8_t stateField0e;
    std::int8_t stateField0f;
    std::int8_t stateField10;
    CameraComputeMode computeMode;
    std::int16_t reserved12;
    Matrix4x4 projectionMatrix;
    Matrix4x4 viewMatrix;
    Vector3 backwardsDirection;
    std::uint16_t currentFovAngle;
    std::uint16_t fovAngle;
    std::uint16_t fullVerticalFov;
    std::int16_t pitch;
    float halfVerticalFovTangent;
    float pitchAngleTangent;
    float viewHeightScale;
    float farDistance;
    float nearDistance;
    float npcRadius;
    shen_name cameraId;
    std::int32_t fieldC4;
    std::int64_t splineBoundary;
    std::int64_t splineCoefficients;
    std::byte reservedD8[0x68];
    Vector3 nextPosition;
    EulerRotation16 nextRotation;
    std::int16_t reserved152;
    Vector3 nextTargetPosition;
    Vector3 previousPosition;
    Vector3 targetPosition;
    float orbitDistance;
    Vector3 currentPosition;
    Vector3 field188;
    EulerRotation16 currentRotation;
    std::int16_t currentFovInterpolationAngle;
    Vector3 field19c;
    Vector3 field1a8;
    float zOffset;
    Vector3 position;
    Vector3 target;
    EulerRotation16 rotation;
    std::int16_t targetFovAngle;
    float targetOrbitDistance;
    Vector3 rotationOffset;
    std::int16_t pitchOffset;
    std::int16_t yawOffset;
    std::int16_t rollOffset;
    std::byte reserved1ee[0x0a];
    std::int8_t savedPresetIndex;
    std::int8_t field1f9;
    std::int8_t field1fa;
    std::int8_t reserved1fb;
    Vector3 shakeAmplitude;
    Vector3 jitter;
    float field214;
    float field218;
    float field21c;
    float field220;
    float field224;
    float field228;
    std::uint32_t reserved22c;
    void (*stateCallback)(CameraRuntime *);
    void (*updateCallback)(CameraRuntime *);
    std::uint32_t field240;
    std::uint32_t field244;
    void (*rotationCallback)();
    Vector3 scale;
    float scaleFactor;
    Vector3 motionVector;
    Vector3 adjustedMotionVector;
    Vector3s motionAngles;
    std::int8_t fovLerpIterations;
    std::int8_t field27f;
    std::int16_t ticksRemaining;
    std::int16_t pitchBase;
    std::int16_t yawBase;
    std::int16_t lookPitchDelta;
    std::int16_t lookYawDelta;
    std::int16_t reserved28a;
    float currentMaximumScale;
    float cameraMinimumScale;
    std::int16_t interpolatedMaximumSpeed;
    std::int16_t currentMinimumSpeed;
    std::uint32_t cumulativeMaximumSpeed;
    std::uint32_t cumulativeMinimumSpeed;
    Vector3 oscillation;
    Vector3 interpolationScale;
    Vector3 motionScale;
    float motionScaleFactor;
    float yOffset;
    CameraInterpolationMode interpolationMode;
    std::int16_t transitionStepCount;
    std::int16_t transitionStep;
    std::int16_t transitionField;
    float interpolationEaseFactor;
    float interpolationProgress;
    float field2e4;
    Vector3 previousCameraPosition;
    Vector3 transitionTargetPosition;
    EulerRotation16 transitionTargetRotation;
    std::int16_t transitionTargetFovAngle;
    float transitionTargetOrbitDistance;
    std::byte reserved304[0x10];
    float targetHeightDistance;
    float defaultOrbitDistance;
    std::int16_t basePitchBias;
    std::int8_t contextMode;
    std::int8_t collisionPhase;
    std::int8_t interpolationTicks;
    std::byte reserved321[3];
    shen_name targetCharacter;
    Matrix4x4 finalTransform;
    std::uint32_t showFilmBars;
    std::int32_t field36c;
};
#pragma pack(pop)

static_assert(sizeof(EulerRotation16) == 0x6);
static_assert(sizeof(CameraRuntime) == 0x370);
static_assert(offsetof(CameraRuntime, projectionMatrix) == 0x14);
static_assert(offsetof(CameraRuntime, farDistance) == 0xb4);
static_assert(offsetof(CameraRuntime, nearDistance) == 0xb8);
static_assert(offsetof(CameraRuntime, npcRadius) == 0xbc);
static_assert(offsetof(CameraRuntime, nextPosition) == 0x140);
static_assert(offsetof(CameraRuntime, targetPosition) == 0x16c);
static_assert(offsetof(CameraRuntime, rotation) == 0x1d0);
static_assert(offsetof(CameraRuntime, interpolationMode) == 0x2cc);
static_assert(offsetof(CameraRuntime, finalTransform) == 0x328);
static_assert(offsetof(CameraRuntime, rotationOffset) == 0x1dc);
static_assert(offsetof(CameraRuntime, stateCallback) == 0x230);
static_assert(offsetof(CameraRuntime, updateCallback) == 0x238);
static_assert(offsetof(CameraRuntime, rotationCallback) == 0x248);
static_assert(offsetof(CameraRuntime, scale) == 0x250);
static_assert(offsetof(CameraRuntime, motionScaleFactor) == 0x2c4);



struct CameraSplineTrack {
    std::uint32_t count;        // +0x00 totalFrameCount
    std::int32_t firstKey;      // +0x04 baseIndex
    std::uint32_t segmentCount; // +0x08 size
    std::uint32_t reserved0c;
    float *times;
    float *values;
    float *tangents;     // +0x20 boundary
    float *coefficients;
};
static_assert(sizeof(CameraSplineTrack) == 0x30);
std::uint32_t AllocSpline(CameraSplineTrack &track);
std::int32_t DoKeyframeCubicHermiteInterpolation(CameraSplineTrack &track, float time, float *out);

enum class CameraSplineChannel : std::uint32_t {
    EyeX,
    EyeY,
    EyeZ,
    TargetX,
    TargetY,
    TargetZ,
    Roll,
    Fov,
};




struct CameraSplineState {
    std::uint32_t state;
    std::uint32_t reserved0004;
    std::uint16_t eyeMode;     // +0x0008 field_8
    std::uint16_t targetMode;  // +0x000a field_A
    std::uint32_t name;
    std::uint32_t *record;     // +0x0010 ECAM
    std::uint32_t authorFlags;
    std::uint32_t reserved001c;
    std::array<CameraSplineTrack, 8> tracks;
    std::array<std::array<float, 400>, 8> coefficientStorage;
    void *eyeNode;
    void *targetNode;
    Vector3 eyeBasePosition;
    Vector3 targetBasePosition;
    Vector3i eyeBaseRotation;
    Vector3i targetBaseRotation;
    Vector3 fallbackPosition;                                 // +0x33e0 aaa[0..2]
    Vector3i fallbackRotation;                                // +0x33ec aaa[3..5]
    std::uint32_t roll;                                       // +0x33f8 yaw
    std::int32_t fov;
    Vector3 cachedEye;
    Vector3 cachedTarget;
    std::int32_t currentFrame;                                // +0x3418 currFrmCnt
    float currentTime;
    Vector3 eye;                                              // +0x3420 finalPos
    Vector3 target;                                           // +0x342c endFocalPoint
    float reserved3438;
    Vector3 localEye;            // +0x343c v2
    Vector3 localTarget;         // +0x3448 unkPos
    std::uint16_t publishedFov;
    std::uint16_t publishedRoll;
    std::array<std::int32_t, 2> reserved3458;
};
static_assert(offsetof(CameraSplineState, record) == 0x10);
static_assert(offsetof(CameraSplineState, tracks) == 0x20);
static_assert(offsetof(CameraSplineState, coefficientStorage) == 0x1a0);
static_assert(offsetof(CameraSplineState, eyeNode) == 0x33a0);
static_assert(offsetof(CameraSplineState, eyeBaseRotation) == 0x33c8);
static_assert(offsetof(CameraSplineState, fallbackPosition) == 0x33e0);
static_assert(offsetof(CameraSplineState, roll) == 0x33f8);
static_assert(offsetof(CameraSplineState, cachedEye) == 0x3400);
static_assert(offsetof(CameraSplineState, currentFrame) == 0x3418);
static_assert(offsetof(CameraSplineState, eye) == 0x3420);
static_assert(offsetof(CameraSplineState, localEye) == 0x343c);
static_assert(sizeof(CameraSplineState) == 0x3460);



void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source);
extern CameraSplineState *g_currentCameraSplines;
CameraSplineState *camCtrlInitSplines() noexcept;
std::uint64_t spliGetCurrentFrmCnt() noexcept;
std::uint32_t GetEventCamName() noexcept;
std::uint64_t sctlAuthAllocSplines(std::int32_t frame);

struct CameraSequenceLease {
    CameraRuntime saved{};
    Vector3 authoredEye{};
    Vector3 authoredTarget{};
    shen_name eyeAttachment{};
    shen_name targetAttachment{};
    std::uint16_t authoredFov{};
    std::int16_t authoredRoll{};
    bool active{};
};
extern CameraRuntime *g_cameraControl;
extern Matrix4x4 g_cameraProjectionMatrix;
extern std::int32_t g_cameraFilmBarsEnabled;
void camCtrlSetFilmBarEnabled(std::int32_t enabled) noexcept;

#pragma pack(push, 1)
struct CameraDialogueRecord {
    std::uint16_t header;
    std::uint8_t type;
    std::uint8_t flags;
    std::uint32_t reserved04;
    float duration;
    std::uint32_t reserved0c;
    Character *speaker;
    Character *listener;
};
#pragma pack(pop)
static_assert(sizeof(CameraDialogueRecord) == 0x20);
static_assert(offsetof(CameraDialogueRecord, speaker) == 0x10);
static_assert(offsetof(CameraDialogueRecord, listener) == 0x18);



void PrepareConversationCameraStream(const CameraDialogueRecord *stream);


std::int64_t ActivateConversationCameraStream(const CameraDialogueRecord *stream);

void RetireConversationCamera();
void ResumeConversationCamera();
void EndConversationCameraStream();
std::int64_t camCtrlHoldConversationShot(std::int32_t release);
std::int64_t camCtrlSetConversationLookTarget(const Vector3 *location);
bool IsConversationCameraCharacter(const Character *character);
shen_name GetCameraSpecialTargetName();
shen_name camCtrlGetConversationEyeAnchorName();
bool camCtrlHasConversationFlag8000() noexcept;
std::int64_t camCtrlSetConversationFlag4000(std::int32_t enabled) noexcept;

void PauseConversationCameraClock(bool paused) noexcept;

extern std::uint32_t g_combatCameraFlags;
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);


#pragma pack(push, 1)
struct SearchCameraState {
    std::int32_t fovStyle;
    std::int32_t phase;
    Vector3 lookBase;
    std::int16_t rotationY;
    std::int16_t reserved16;
    float lockedOnAngle;
    Vector3 cameraPosition;
    Vector3 offsetPosition;
    EulerRotation16 initialRotation;
    std::int32_t field3a;
    std::int16_t field3e;
    float distance;
    float field44[7];
    Vector3 lookTarget;
    std::uint32_t flags;
    std::int32_t fov;
    std::int32_t zoomFov;
    float zoomScale;
    std::int32_t yaw;
    std::int32_t pitch;
    std::int32_t reserved84[3];
};
#pragma pack(pop)
static_assert(sizeof(SearchCameraState) == 0x90);
static_assert(offsetof(SearchCameraState, lookTarget) == 0x60);
static_assert(offsetof(SearchCameraState, flags) == 0x6c);
static_assert(offsetof(SearchCameraState, yaw) == 0x7c);

void srchSetupCameraFov();
void srchSetLookTarget(const Vector3 *target);
void srchSetLookBase(const Vector3 *base);
void srchGetLookTarget(Vector3 *target);
void srchGetLookBase(Vector3 *base);
void srchSetRotY(std::int16_t yaw);
void camCtrlSetLockedOnAngle(float angle);
bool camCtrlSwitchToLockedOn(std::int32_t fovStyle);
bool camCtrlSetLookAroundMode();
bool shenCheckCurrentFoVModeIs2();
bool camCtrlIsTransitionActive();
std::uint32_t shenApplyLockonFlag(std::uint32_t flags);

std::uint16_t camCtrlGetFov() noexcept;
Vector3 *camCtrlGetTargetPosition(Vector3 *output) noexcept;
float camCtrlGetDeltaTime(float amount) noexcept;
Matrix4x4 *camCtrlGetViewMatrix(Matrix4x4 *output) noexcept;
Matrix4x4 *camCtrlGetProjectionMatrix(Matrix4x4 *output) noexcept;
CameraState camCtrlGetState() noexcept;
std::uint16_t camCtrlGetPerspective() noexcept;
std::int64_t camCtrlGetAdjustedPitchOrYaw(std::uint16_t *yaw, std::uint16_t *pitch) noexcept;
std::int32_t camCtrlGetNextRotY() noexcept;
EulerRotation16 *camCtrlGetRot(EulerRotation16 *output) noexcept;
float camCtrlGetFarDistance() noexcept;
float camCtrlGetNearDistance() noexcept;
float camCtrlGetNpcRadius() noexcept;
float camCtrlGetHalfVerticalFovTangent() noexcept;
float camCtrlGetPitchAngleTangent() noexcept;



void camCtrlSetFov(std::int16_t fieldOfView) noexcept;
void camCtrlSetTargetPosition(const Vector3 &target) noexcept;
void camCtrlSetPerspective(std::uint16_t perspective) noexcept;
void camCtrlSetNextPosition(const Vector3 &position) noexcept;
std::uint64_t camCtrlSetState(CameraState state);
void camCtrlResetTransition(CameraInterpolationMode mode);
void camCtrlSetInterpEaseFactor(float factor);
void camCtrlUpdate();

bool camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt);

// InitDynamicSceneData publishes two aligned 257-sample
// buffers. camCtrlSubmitMatrices writes world depth, then camera-only walls.
struct CameraFieldDepthSample {
    std::uint32_t id;
    float depth;
};
static_assert(sizeof(CameraFieldDepthSample) == 8);

std::span<const CameraFieldDepthSample, 257> GetWorldFieldDepthSamples();
std::span<const CameraFieldDepthSample, 257> GetCameraFieldDepthSamples();
std::uint32_t GetFieldDepthPointCount();
const Matrix4x4 &GetFieldDepthTransform();
Matrix4x4 &GetFieldDepthModelTransform();
MapCellOffset GetFieldDepthCell(float x, float z);
std::span<const MapCellOffset> camCtrlGetMapVisibleCells(std::uint32_t rotationY, std::int32_t tier);
std::int32_t camCtrlGetDrawTier(float distance);
float camCtrlGetCurrentFarDistance();
void camCtrlSetupDepthScaling();
void camCtrlSubmitFieldDepth(const CameraRuntime &camera);
bool camCtrlIsModelDepthVisible(const Vector3 &centre, const MT7Node &node, float radius);
bool camCtrlIsInsideDepthScaledRect(const Vector3 &centre, float radius);
bool camCtrlIsInsideHorizontalDepthRect(const Vector3 &centre, float radius);
bool camCtrlIsSphereVisible(const Vector3 &centre, float radius);
bool ScreenVisCheck(const Vector3 &centre, float scale, float clipRange);
bool camCtrlIsNearSphereVisible(const Vector3 &centre, float radius);
bool camCtrlIsFarSphereVisible(const Vector3 &centre, float radius);
bool camCtrlIsScreenSpaceVisible(const Vector3 &centre, float radius);
bool camCtrlCheckPosition(const Vector3 &position, float radius);
void camCtrlSetProjectedShadowDepth(float distance);
bool camCtrlIsProjectedShadowVisible(const Vector3 &centre, float radius);

void camCtrlInitialize(CameraRuntime &camera);
float GenerateOscillationForCamera(std::int16_t phase);
void camCtrlSmoothMotion(CameraRuntime &camera);
void ApplyRotationMatrixFromTransform(CameraRuntime &camera);
void camCtrlRecalcFov(CameraRuntime &camera);
void camCtrlSetup(CameraRuntime &camera);
void camCtrlCalculateFov(CameraRuntime &camera);
void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera);
HLib::Task *camCtrlEnqueue(std::uint8_t nextFunctionIndex);
void camCtrlSetAuthoredView(const Vector3 &target, const Vector3 &eye, std::int16_t roll, std::uint16_t fieldOfView);

void camCtrlCacheTransform();
void camCtrlRestoreCachedTransform();
void camCtrlSaveRuntime();
void camCtrlRestoreRuntime();
void camCtrlSetTranslationScale(float factor);
void camCtrlResetTranslationScale();
// Projection globals, distinct from CameraRuntime's culling distances.
extern float g_projectionNearClip;
extern float g_projectionFarClip;
void camCtrlSetClipDistances(float nearDistance, float farDistance);
void camCtrlResetClipDistances();
float camCtrlSetFarDistance(float distance);
void camCtrlSetNearDistance(float distance);
void camCtrlSetNpcRadius(float radius);
std::int32_t camCtrlGetContextMode();
std::int32_t camCtrlGetContextVal();


std::int32_t camCtrlSetContext(std::int32_t allowSwitch, std::int32_t firstPerson);
std::uint64_t camCtrlHandleContextSwitch(std::uint8_t newContextMode);
std::uint64_t EVS_LoopCamera(std::int64_t cameraId, shen_name eye, shen_name target);
void camCtrlGetNextCamPos(Vector3 *position);
CameraRuntime *camCtrlSetFovTransitionTo(bool enabled);

enum class CameraUtilityMode : std::int32_t {
    RestorePrevious = 10,
    SetEaseFactor = 11,
    SetTargetHeight = 12,
    SetOrbitDistance = 13,
    SetPitchBias = 14,
    ResetPlayerDistance = 15,
    GetContext = 16,
    ForceSearchCam = 17,
    PreviousToCurrent = 18,
    SetTransitionSteps = 19,
    EnableInterpolationSnap = 20,
    SetConversationPosition = 30,
    HasConversationFlaggedView = 31,
};
struct CameraUtilityParameters {
    std::uint32_t first;
    std::uint32_t second;
};
static_assert(sizeof(CameraUtilityParameters) == 8);
extern std::int32_t g_cameraExposureInitialized;
extern Vector3 g_cameraTargetPublication;
void camCtrlPublishTargetPosition(const Vector3 *target) noexcept;
void camCtrlSetDistanceFromPlayerDefaults() noexcept;
std::uint64_t camCtrlSetConversationPosition(const Vector3 *position) noexcept;
bool camCtrlHasConversationFlaggedView() noexcept;
std::uint32_t camCtrlIsConversationActive() noexcept;
std::uint64_t camCtrlUtil(CameraUtilityMode mode, std::intptr_t parameter);
bool camCtrlToggleSmoothing(std::int32_t enabled) noexcept;
CameraRuntime *camCtrlSetMotionScaleFactor(float factor) noexcept;
CameraRuntime *camCtrlSetYOffset(float offset) noexcept;
void camCtrlResetMotionscaling() noexcept;
void camCtrlSetScaleTo(const Vector3 *scale) noexcept;
std::uint64_t camCtrlSetScaleToAndRecalcFov(const Vector3 *scale) noexcept;
CameraRuntime *camCtrlSetFlags() noexcept;
void camCtrlSetRotation(const Vector3 *translation, const EulerRotation16 *rotation) noexcept;
void camCtrlGetRotationXform(Vector3 *translation, EulerRotation16 *rotation) noexcept;
void camCtrlSetRotationCallback(void (*callback)()) noexcept;
void camCtrlSetUpdateCallback(void (*callback)(CameraRuntime *)) noexcept;
void camCtrlSetTargetChara(shen_name name) noexcept;
std::uint64_t camCtrlSetNextCamRot(const EulerRotation16 *rotation) noexcept;
void camCtrlSetNextRoll(std::int16_t roll) noexcept;
bool camCtrlAcquireSequenceLease(CameraSequenceLease &lease);
bool camCtrlSetSequenceView(CameraSequenceLease &lease, std::int32_t cameraId, shen_name eyeAttachment, shen_name targetAttachment, const Vector3 &eye,
    const Vector3 &target, std::int16_t roll, std::uint16_t fieldOfView, std::uint16_t transitionFrames, bool cut);
bool camCtrlTickSequenceView(CameraSequenceLease &lease);
void camCtrlReleaseSequenceLease(CameraSequenceLease &lease);

} // namespace shenmue::s2
