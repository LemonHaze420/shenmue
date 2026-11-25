#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"

#define SHENMUE_CAMERA_LAYOUT_NAME_(line) ShenmueCameraLayoutLayoutCheck##line
#define SHENMUE_CAMERA_LAYOUT_NAME(line) SHENMUE_CAMERA_LAYOUT_NAME_(line)
#define SHENMUE_CAMERA_LAYOUT(condition) typedef char SHENMUE_CAMERA_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {
using ::shenmue::shen_name;

extern std::uint32_t g_combatCameraFlags;
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct Character;
struct FightActorRuntimePrefix;
struct FightRuntimePrefix;
std::uint8_t camCtrlGetInterpMode();
void camCtrlSetInterpModeTo(std::uint8_t mode);
void camCtrlSetFightDistance(float distance);
void camCtrlSetFightHeight(float height);
void vfSetupLinkedFightCamera(FightActorRuntimePrefix *actor, std::uint8_t kind);
void vfUnknownCamRelated(FightRuntimePrefix *game);
bool IsAuthFightCamera();
bool IsEarlyAuthFightCamera();
std::int32_t vfPrepareAuthoredDefeatCamera();
void BeginMotionUpdateCamera(Character *target);
void ResetMotionCameraWaypoints();
void AppendMotionCameraWaypoint(const Vector3 &position);
void BeginMotionInterpolationCamera(Character *target);
#endif
void CameraControlNullsub();

enum CameraFlag { CameraConversationActive = 2U, CameraSmoothMotion = 0x20000U };
#define SHENMUE_CAMERA_NOEXCEPT
struct CameraRuntime {
    std::uint32_t flags;
    std::int16_t state, previousState, retainedState, defaultState;
    std::int8_t computeMode, interpolationSourceMode;
    std::byte retainedModeState[2];
    Matrix4x4 projectionMatrix, viewMatrix;
    Vector3 backwardsDirection;
    std::int16_t currentFovAngle, fovAngle, fullVerticalFov, verticalFovAngle;
    float halfVerticalFovTangent, pitchAngleTangent, viewHeightScale, farDistance, nearDistance, npcRadius;
    shen_name cameraId;
    std::byte splineState[0x3c];
    Vector3 nextPosition;
    Vector3s nextRotation;
    std::int16_t retainedNextRotation;
    Vector3 nextTargetPosition, previousPosition, targetPosition;
    float orbitDistance;
    Vector3 currentPosition;
    std::byte retainedPositionState[12];
    Vector3s currentRotation;
    std::int16_t currentFovInterpolationAngle;
    std::byte retainedTargetState[12];
    Vector3 anchorOffset;
    float zOffset;
    Vector3 position, target;
    Vector3s rotation;
    std::int16_t targetFovAngle;
    float targetOrbitDistance;
    Vector3 rotationOffset;
    Vector3s angularOffset;
    std::byte retainedAngularState[10];
    std::int8_t savedPresetIndex, presetState, presetFlags, retainedPresetState;
    Vector3 shakeAmplitude;
    Vector2 jitter;
    std::uint32_t retained1cc;
    Vector3 positionInterpolationRate, rotationInterpolationRate;
    void (*updateCallback)(CameraRuntime *);
    std::byte retainedCallbackState[8];
    void (*rotationCallback)();
    Vector3 scale;
    float scaleFactor;
    Vector3 motionVector, adjustedMotionVector;
    Vector3s motionRotation;
    std::uint8_t fovLerpIterations;
    std::byte retained227;
    std::int16_t ticksRemaining;
    std::int16_t pitchBase, yawBase, lookPitchDelta, lookYawDelta, retained232;
    float currentMaximumScale, cameraMinimumScale;
    std::int16_t interpolatedMaximumSpeed, currentMinimumSpeed;
    std::int32_t cumulativeMaximumSpeed, cumulativeMinimumSpeed;
    Vector3 oscillation, motionScale, interpolationScale;
    float motionScaleFactor;
    std::int16_t interpolationMode, interpolationFrameCount, interpolationPendingState;
    std::byte retainedInterpolationState[2];
    float interpolationSpeed, interpolationElapsed;
    std::uint32_t retainedInterpolationView;
    Vector3 interpolationEye, interpolationTarget;
    Vector3s interpolationRotation;
    std::int16_t interpolationFov;
    float interpolationDistance;
    std::byte retainedInterpolationTail[0x10];
    float targetHeightDistance, defaultOrbitDistance;
    std::int16_t basePitchBias;
    std::int8_t contextMode;
    std::int8_t collisionPhase, interpolationTicks;
    std::byte retainedContextState[3];
    shen_name targetCharacter;
    Matrix4x4 finalTransform;
};
struct CameraPreset {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::uint32_t tag;
    Vector3 position;
#else
    union {
        std::uint32_t tag;
        Vector3 position;
    };
#endif
    Vector3s rotation;
    std::int16_t fovAngle;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::byte retained18[0x10];
#else
    std::byte retained14[0x10];
#endif
    Matrix4x4 projectionMatrix, viewMatrix;
    Vector3 backwardsDirection;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::byte retainedb4[0x14];
#else
    std::byte retainedb0[0x18];
#endif
};
struct FieldDepthScale {
    float farDistance, nearDistance, horizontalTangent, verticalTangent;
};
struct FieldDepthSavedScale {
    float npcRadius, horizontalTangent, verticalTangent;
};
SHENMUE_CAMERA_LAYOUT(sizeof(CameraRuntime) == 0x30c);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationSourceMode) == 0x0d);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, projectionMatrix) == 0x10);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, viewMatrix) == 0x50);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, currentFovAngle) == 0x9c);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, halfVerticalFovTangent) == 0xa4);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, npcRadius) == 0xb8);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, nextPosition) == 0xfc);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, nextRotation) == 0x108);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, nextTargetPosition) == 0x110);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, targetPosition) == 0x128);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, orbitDistance) == 0x134);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, currentRotation) == 0x150);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, targetFovAngle) == 0x192);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, targetOrbitDistance) == 0x194);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, savedPresetIndex) == 0x1b4);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, motionScaleFactor) == 0x26c);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationMode) == 0x270);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationEye) == 0x284);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationTarget) == 0x290);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationRotation) == 0x29c);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationFov) == 0x2a2);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, anchorOffset) == 0x164);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, scaleFactor) == 0x204);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, motionVector) == 0x208);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, currentMaximumScale) == 0x234);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, oscillation) == 0x248);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, motionScale) == 0x254);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationScale) == 0x260);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, interpolationDistance) == 0x2a4);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, updateCallback) == 0x1e8);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, rotationCallback) == 0x1f4);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, scale) == 0x1f8);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, shakeAmplitude) == 0x1b8);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, jitter) == 0x1c4);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, fovLerpIterations) == 0x226);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, targetHeightDistance) == 0x2b8);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, basePitchBias) == 0x2c0);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, contextMode) == 0x2c2);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, targetCharacter) == 0x2c8);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, finalTransform) == 0x2cc);
SHENMUE_CAMERA_LAYOUT(sizeof(CameraPreset) == 0xc8);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, position) == 0x04);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, rotation) == 0x10);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, fovAngle) == 0x16);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, projectionMatrix) == 0x28);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, viewMatrix) == 0x68);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, backwardsDirection) == 0xa8);
#else
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, projectionMatrix) == 0x24);
SHENMUE_CAMERA_LAYOUT(offsetof(CameraPreset, backwardsDirection) == 0xa4);
#endif
SHENMUE_CAMERA_LAYOUT(sizeof(FieldDepthScale) == 0x10);
SHENMUE_CAMERA_LAYOUT(sizeof(FieldDepthSavedScale) == 0x0c);

extern CameraRuntime g_cameraStorage;
extern CameraRuntime *g_cameraControl;
extern CameraPreset g_cameraCachedPresets[3];
extern FieldDepthScale fieldDepthScale;
extern FieldDepthSavedScale fieldDepthSavedScale;
void camCtrlInitialize(CameraRuntime &camera);
std::int32_t camCtrlGetNextRotY();
std::int32_t camCtrlSetState(std::int32_t state);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt);
#endif
void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source);
std::int32_t EVS_LoopCamera(std::int32_t cameraId, shen_name eye, shen_name target);
std::int32_t camCtrlSetInterpolationMode(std::int32_t mode);
void camCtrlSetInterpolationSpeed(float speed);
void camCtrlSetInterpolationFrameCount(std::int32_t frames);
void camCtrlCalculateFov(CameraRuntime &camera);
void ResetCameraPresetCache(CameraRuntime &camera);
std::int32_t camCtrlToggleSmoothing(std::int32_t enabled);
std::int32_t camCtrlIsConversationActive();
std::int16_t camCtrlGetFov();
std::int32_t camCtrlSetFov(std::int16_t angle);
std::int32_t camCtrlGetCurrentFov();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 *camCtrlGetViewMatrix(Matrix4x4 *output) SHENMUE_CAMERA_NOEXCEPT;
SHENMUE_CAMERA_LAYOUT(offsetof(CameraRuntime, viewHeightScale) == 0xac);
float camCtrlGetViewHeightScale();
#endif
Matrix4x4 *camCtrlGetProjectionMatrix(Matrix4x4 *output);
void camCtrlSetNextPosition(const Vector3 &position);
void camCtrlSetTargetPosition(const Vector3 &position);
void camCtrlSetNextCamRot(const Vector3s *rotation);
Vector3s *camCtrlGetRot(Vector3s *rotation);
std::int32_t camCtrlSetPerspective(std::int16_t perspective);
std::int16_t camCtrlGetPerspective();
void camCtrlSetRotation(const Vector3 *translation, const Vector3s *rotation);
void camCtrlGetRotationXform(Vector3 *translation, Vector3s *rotation);
void camCtrlGetAdjustedPitchOrYaw(std::uint16_t *base, std::uint16_t *adjusted);
typedef void (*CameraRotationCallback)();
typedef void (*CameraUpdateCallback)(CameraRuntime *);
CameraRotationCallback camCtrlSetRotationCallback(CameraRotationCallback callback);
CameraUpdateCallback camCtrlSetUpdateCallback(CameraUpdateCallback callback);
void camCtrlSetScaleTo(const Vector3 *scale);
void camCtrlSetScaleToAndRecalcFov(const Vector3 *scale);
void camCtrlSetFlags();
float camCtrlSetMotionScaleFactor(float factor);
void camCtrlSetRotationXYZ(float x, float y, float duration);
void camCtrlSetShakeAmplitude(float x, float y, float duration);
void camCtrlSetFovTransitionTo(std::int32_t enabled);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 *camCtrlSetXform(const Matrix4x4 *transform);
#endif
std::int32_t camCtrlSetContextMode(std::int32_t mode);
std::int32_t camCtrlGetContextMode();
std::int32_t camCtrlSetContext(std::int32_t enabled, std::int32_t retainSettings);
void camCtrlSetDistanceFromPlayerDefaults();
std::int32_t camCtrlUtil(std::int32_t operation, void *parameter);
std::int32_t camCtrlHandleContextSwitch(std::int32_t mode);
shen_name camCtrlSetTargetChara(shen_name name);
struct CameraContextSetting {
    float distanceFromPlayer;
    std::int16_t basePitchBias;
    std::uint16_t retained;
};
SHENMUE_CAMERA_LAYOUT(sizeof(CameraContextSetting) == 8);
extern const CameraContextSetting cameraContextSettings[];
float camCtrlGetNpcRadius() SHENMUE_CAMERA_NOEXCEPT;
float camCtrlGetFarDistance() SHENMUE_CAMERA_NOEXCEPT;
float camCtrlGetNearDistance() SHENMUE_CAMERA_NOEXCEPT;
float camCtrlGetHalfVerticalFovTangent() SHENMUE_CAMERA_NOEXCEPT;
float camCtrlGetPitchAngleTangent() SHENMUE_CAMERA_NOEXCEPT;
float camCtrlSetFarDistance(float distance);
float camCtrlSetNearDistance(float distance);
float camCtrlSetNpcRadius(float radius);
Vector3 *camCtrlGetNextCamPos(Vector3 *position);
Vector3 *camCtrlGetTargetPosition(Vector3 *position);
void shenSetDepthScale(float farDistance, float nearDistance, float npcRadius, float horizontalTangent, float verticalTangent);
bool camCtrlIsInsideDepthScaledRect(const Vector3 &position, float radius);
bool camCtrlIsSphereVisible(const Vector3 &position, float radius);
bool camCtrlIsNearSphereVisible(const Vector3 &position, float radius);
bool camCtrlIsFarSphereVisible(const Vector3 &position, float radius);
bool camCtrlIsScreenSpaceVisible(const Vector3 &position, float radius);
bool ScreenVisCheck(const Vector3 &position, float radius, float depth);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_CAMERA_NOEXCEPT
#undef SHENMUE_CAMERA_LAYOUT_NAME
#undef SHENMUE_CAMERA_LAYOUT_NAME_
#undef SHENMUE_CAMERA_LAYOUT

#endif
