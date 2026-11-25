#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/math.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#include "s2/camera_task_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/camera_roll_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include <math.h>
#include <float.h>
#include "shenmue.h"
#include "s2/battle_qte_sprites.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/window.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/frame_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#endif
#include "s2/motion_registry_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/osage_dc.hpp"
#include <cmath>
#include "s2/character_control_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/hlib.hpp"
#include "s2/pak_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scenario_dc.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include <cstdio>
#include "s2/model_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/conversation_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>

namespace shenmue {
namespace s2 {

namespace {

typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);
typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);
typedef std::int32_t (*FightMarkerHandler)(FightActorRuntimePrefix *);
typedef std::int32_t (*FightFrameCallback)(FightActorRuntimePrefix *);

} // namespace

void camCtrlInitialize(CameraRuntime &camera);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t ResolveFieldSphereVolumeInWorld(Vector3 *output, const Vector3 *position, float radius);
#endif
void camCtrlCalculateFov(CameraRuntime &camera);
void camCtrlSetRotationXYZ(float x, float y, float duration);
void camCtrlSetFovTransitionTo(std::int32_t enabled);
std::int32_t camCtrlSetContextMode(std::int32_t mode);
void camCtrlSetDistanceFromPlayerDefaults();
std::int32_t camCtrlToggleSmoothing(std::int32_t enabled);
std::int32_t camCtrlSetInterpolationMode(std::int32_t mode);
void camCtrlSetInterpolationFrameCount(std::int32_t frames);
void ResetCameraPresetCache(CameraRuntime &camera);
void CameraControlNullsub2();
void InitializeConversationCameraState();
std::int32_t SetConversationCameraEnabled(std::int32_t enabled);
void UpdateConversationCamera();
void InitializeCameraGoal();
void camCtrlEvaluateGoal();
std::int32_t SetConversationCameraFixedPosition(const Vector3 *position);
std::int32_t IsConversationCameraShotEstablished();
void camCtrlSwitchColiPhase(CameraRuntime &camera);
void camCtrlSetCamSnapAngY(CameraRuntime &camera);
void camCtrlHandleLookAroundCam(CameraRuntime &camera);
void UpdateLockedOnCamera();
void camCtrlCallback(void *);

void InitializeConversationCameraState();
std::int32_t SetConversationCameraEnabled(std::int32_t enabled);
void UpdateConversationCamera();
void InitializeCameraGoal();
void camCtrlEvaluateGoal();
std::int32_t SetConversationCameraFixedPosition(const Vector3 *position);
std::int32_t IsConversationCameraShotEstablished();
void camCtrlSwitchColiPhase(CameraRuntime &camera);
void camCtrlSetCamSnapAngY(CameraRuntime &camera);
void camCtrlHandleLookAroundCam(CameraRuntime &camera);
void UpdateLockedOnCamera();
void camCtrlCallback(void *);
std::int32_t SetConversationCameraFixedPosition(const Vector3 *position);
std::int32_t IsConversationCameraShotEstablished();
void InitializeConversationCameraState();
std::int32_t SetConversationCameraEnabled(std::int32_t enabled);
void UpdateConversationCamera();
void InitializeCameraGoal();
void camCtrlEvaluateGoal();
void camCtrlSwitchColiPhase(CameraRuntime &camera);
void camCtrlSetCamSnapAngY(CameraRuntime &camera);
void camCtrlHandleLookAroundCam(CameraRuntime &camera);
void UpdateLockedOnCamera();
void camCtrlCallback(void *);
std::int32_t SetConversationCameraFixedPosition(const Vector3 *position);
std::int32_t IsConversationCameraShotEstablished();
void InitializeConversationCameraState();
std::int32_t SetConversationCameraEnabled(std::int32_t enabled);
void UpdateConversationCamera();
void InitializeCameraGoal();
void camCtrlEvaluateGoal();
void camCtrlSwitchColiPhase(CameraRuntime &camera);
void camCtrlSetCamSnapAngY(CameraRuntime &camera);
void camCtrlHandleLookAroundCam(CameraRuntime &camera);
void UpdateLockedOnCamera();
void camCtrlCallback(void *);
std::uint32_t g_combatCameraFlags;
CameraRuntime g_cameraStorage;
CameraRuntime *g_cameraControl = &g_cameraStorage;
CameraPreset g_cameraCachedPresets[3];
FieldDepthScale fieldDepthScale = {};
FieldDepthSavedScale fieldDepthSavedScale = {};
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);
static float FightCameraLength(const Vector3 &value);
static void ScaleFightCameraVector(Vector3 &value, float length);
static bool FightCameraRaycast(Vector3 &result, const Vector3 &destination, const Vector3 &origin);
static void ResetFightCameraPattern();
void resetIntersectionAndTest();
void InitializeCameraGoal();
static void MoveFightCameraToward(const Vector3 &desired, Vector3 &retained, Vector3 &direction);
void camCtrlEvaluateGoal();
static std::int32_t CameraAbsInteger(std::int32_t value);
template <class T> static T CameraClamp(T value, T minimum, T maximum);
template <class T> static T CameraMin(T first, T second);
template <class T> static T CameraMax(T first, T second);
static Vector3 CameraVector(float x, float y, float z);
static void ClampFightCameraPoint(Vector3 &point, const Vector3 &centre);
static Vector3 RotateFightCameraPoint(const Vector3 &relative, const Vector3 &centre, std::uint16_t yaw);
static Vector3 FightCameraProbeFor(const Vector3 &target, float y, std::uint16_t angle);
void AccumulateFightCameraVelocity(FightRuntimePrefix *game, FightActorRuntimePrefix *self);
void UpdateFightCameraPattern(FightRuntimePrefix *game);
void UpdateEnvironmentFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);
void UpdateFieldFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);
void UpdateFirstPersonFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);
std::uint8_t camCtrlGetInterpMode();
void BeginMotionUpdateCamera(Character *target);
void ResetMotionCameraWaypoints();
void AppendMotionCameraWaypoint(const Vector3 &position);
void BeginMotionInterpolationCamera(Character *target);
void camCtrlSetFightDistance(float distance);
void camCtrlSetFightHeight(float height);
void camCtrlSetInterpModeTo(std::uint8_t mode);
bool IsEarlyAuthFightCamera();
bool IsAuthFightCamera();
std::int32_t vfPrepareAuthoredDefeatCamera();
void SetFightCameraLinkedKind(std::uint8_t kind);
void vfSetupLinkedFightCamera(FightActorRuntimePrefix *actor, std::uint8_t kind);
void vfUnknownCamRelated(FightRuntimePrefix *game);
void ResetFightCameraToParticipants(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);
void UpdateQuadraticFightCamera(FightRuntimePrefix *game, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);
static void PublishFightCameraPlayerAlpha(Character *player, const Vector3 &bone);
void UpdateMotionFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *self, FightActorRuntimePrefix *other);
void UpdateMotionInterpolationFightCamera(FightRuntimePrefix *, FightActorRuntimePrefix *, FightActorRuntimePrefix *);
std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount);
static std::int16_t CameraTransitionAngle(std::int16_t base, std::int16_t target, float amount);
static void camCtrlRetainPreviousPose(CameraRuntime &camera);
static void camCtrlResetDefaultState(CameraRuntime &camera);
static void camCtrlComputePreviousPositionNullsub(CameraRuntime &);
static void camCtrlComputeOrbitPosition(CameraRuntime &camera);
static void camCtrlComputeLookAtPosition(CameraRuntime &camera);
static void camCtrlComputeAnchorPosition(CameraRuntime &camera);
void UpdateCameraTransitions(CameraRuntime &camera);
void camCtrlInterpolateNextPositionRotation(CameraRuntime &camera);
void camCtrlComputeNextPosition(CameraRuntime &camera);
static void CameraSplineProjectionNullsub();
static void CameraMatrixProjectionNullsub();
static void ConversationCameraProjectionNullsub();
static void CameraProjectionPostCallback(CameraRuntime &);
static void camCtrlProjectCameraMatrices(void *);
static void camCtrlSetFloats(CameraRuntime &camera, float position, float angle);
static Vector3 plymComputeConvTargetAnchor();
static void CameraScaleTo(Vector3 &value, float length);
static float CameraDistance(const Vector3 &first, const Vector3 &second);
std::int32_t camCtrlGetIdealCameraPosition(CameraRuntime &camera);
std::int32_t camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt);
static void camCtrlInterpolateApplyCameraAngY(CameraRuntime &camera);
static void camCtrlUpdateFollowCamera(CameraRuntime &camera);
static void camCtrlAutoCenterTargetInView(CameraRuntime &camera);
void camCtrlSwitchColiPhase(CameraRuntime &camera);
void camCtrlSetCamSnapAngY(CameraRuntime &camera);
static float ClampCameraValue(float value, float minimum, float maximum);
void camCtrlHandleLookAroundCam(CameraRuntime &camera);
static float SearchCameraEase(const SearchCameraState &state);
static void ResetLockedOnCameraPosition(CameraRuntime &camera, SearchCameraState &state);
void UpdateLockedOnCamera();
void camCtrlCallback(void *);
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action);

void InitializeCameraGoal();
void camCtrlBuildScriptCamSpline(std::int32_t name, const void *eye, const void *target);
void camCtrlCreateSpline(const std::uint32_t *record, const void *eye, const void *target);
void camCtrlHandleQueuedSpline();

// @verified - d
HLib::Task *Queue_CameraCtrl(std::uint8_t nextFunctionIndex) {
    const std::int8_t functionIndex = nextFunctionIndex;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(camCtrlCallback, functionIndex, 7, 0, TASK_CAMT);
    g_cameraControl = &g_cameraStorage;
    camCtrlInitialize(g_cameraStorage);
    g_cameraStorage.flags |= 0x407U;
    g_cameraStorage.flags |= 0x08000000U;
    g_cameraStorage.state = 0;
    g_cameraStorage.fovAngle = 0x2aaa;
    g_cameraStorage.currentFovAngle = 0x2aaa;
    camCtrlCalculateFov(g_cameraStorage);
    camCtrlSetRotationXYZ(0.0f, 0.0f, 1.0f);
    InitializeConversationCameraState();
    camCtrlInitSplines();
    camCtrlSetDistanceFromPlayerDefaults();
    camCtrlSetFovTransitionTo(1);
    CameraControlNullsub2();
    return task;
}

// @verified - d
void camCtrlInitialize(CameraRuntime &camera) {
    std::memset(static_cast<void *>(&camera), 0, sizeof(camera));
    camera.flags = 0;
    camera.state = 0;
    camera.previousState = 0;
    camera.defaultState = -1;
    camera.computeMode = 0;
    camera.retained1cc = 0;
    camera.updateCallback = 0;
    reinterpret_cast<std::uint32_t *>(camera.retainedCallbackState)[0] = 0;
    reinterpret_cast<std::uint32_t *>(camera.retainedCallbackState)[1] = 0;
    camera.rotationCallback = 0;
    camera.nextPosition.x = 0.0f;
    camera.nextPosition.y = 0.0f;
    camera.nextPosition.z = 0.0f;
    camera.nextRotation.x = 0;
    camera.nextRotation.y = 0;
    camera.nextRotation.z = 0;
    camera.nextTargetPosition.x = 0.0f;
    camera.nextTargetPosition.y = 0.0f;
    camera.nextTargetPosition.z = 0.0f;
    camera.previousPosition.x = 0.0f;
    camera.previousPosition.y = 0.0f;
    camera.previousPosition.z = 0.0f;
    camera.targetPosition.x = 0.0f;
    camera.targetPosition.y = 0.0f;
    camera.targetPosition.z = 0.0f;
    camera.orbitDistance = 3.1f;
    camera.targetFovAngle = 0x2aaa;
    camera.targetOrbitDistance = 3.1f;
    camera.rotationOffset.x = 0.0f;
    camera.rotationOffset.y = 0.0f;
    camera.rotationOffset.z = 0.0f;
    camera.angularOffset.x = 0;
    camera.angularOffset.y = 0;
    camera.angularOffset.z = 0;
    camera.fovAngle = 0x2aaa;
    camera.currentFovAngle = 0x2aaa;
    camCtrlCalculateFov(camera);
    camera.motionVector.x = 0.0f;
    camera.motionVector.y = 0.0f;
    camera.motionVector.z = 0.0f;
    camera.motionScale.x = 0.0f;
    camera.motionScale.y = 0.0f;
    camera.motionScale.z = 0.0f;
    camera.motionRotation.x = 0;
    camera.motionRotation.y = 0;
    camera.motionRotation.z = 0;
    camera.fovLerpIterations = 0;
    camera.ticksRemaining = 0;
    camera.cumulativeMaximumSpeed = 0;
    camera.cumulativeMinimumSpeed = 0;
    camCtrlToggleSmoothing(0);
    camera.interpolationMode = 0;
    camera.interpolationFrameCount = 0;
    camera.interpolationPendingState = 0;
    camera.interpolationSpeed = 0.0f;
    camera.interpolationFov = 0;
    camera.interpolationDistance = 0.0f;
    matrix_stack::PushUnitMatrix();
    matrix_stack::GetMatrix(&camera.projectionMatrix);
    matrix_stack::GetMatrix(&camera.viewMatrix);
    matrix_stack::GetMatrix(&camera.finalTransform);
    PopCurrentMatrix(1);
    camera.savedPresetIndex = 0;
    ResetCameraPresetCache(camera);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera) {
    matrix_stack::SetupStack();
    matrix_stack::SetActiveStack(static_cast<matrix_stack::MatrixStackType>(3));
    matrix_stack::UnitCurrentMatrix();
    camCtrlCalculateFov(camera);
    dc::sdk::SetMT7Perspective(camera.verticalFovAngle, 1.3333333730697632f, 0.01f, 200000.0f);
    matrix_stack::SetActiveStack(static_cast<matrix_stack::MatrixStackType>(1));
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
    Matrix4x4::RotateAroundZ_Wrapper(camera.nextRotation.z);
    GetTransformedForwardVector(&camera.backwardsDirection);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::RotateAroundZ_Wrapper(-camera.nextRotation.z);
    Matrix4x4::RotateAroundX_Wrapper(-camera.nextRotation.x);
    Matrix4x4::RotateAroundY_Wrapper(-camera.nextRotation.y);
    Matrix4x4::preTranslate(-camera.nextPosition.x, -camera.nextPosition.y, -camera.nextPosition.z);
    matrix_stack::GetMatrix(&camera.projectionMatrix);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::preTranslate(camera.nextPosition.x, camera.nextPosition.y, camera.nextPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
    Matrix4x4::RotateAroundZ_Wrapper(camera.nextRotation.z);
    matrix_stack::GetMatrix(&camera.viewMatrix);
    matrix_stack::LoadMatrix(&camera.projectionMatrix);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlApply(CameraRuntime &camera) {
    camera.motionScale.x = camera.motionVector.x;
    camera.motionScale.y = camera.motionVector.y;
    camera.motionScale.z = camera.motionVector.z;
    camera.interpolationScale.x = camera.adjustedMotionVector.x;
    camera.interpolationScale.y = camera.adjustedMotionVector.y;
    camera.interpolationScale.z = camera.adjustedMotionVector.z;
    camera.motionVector.x = camera.nextPosition.x - camera.position.x;
    camera.motionVector.y = camera.nextPosition.y - camera.position.y;
    camera.motionVector.z = camera.nextPosition.z - camera.position.z;
    camera.adjustedMotionVector.x = camera.targetPosition.x - camera.target.x;
    camera.adjustedMotionVector.y = camera.targetPosition.y - camera.target.y;
    camera.adjustedMotionVector.z = camera.targetPosition.z - camera.target.z;
    camera.motionRotation.x = static_cast<std::int16_t>(camera.nextRotation.x - camera.rotation.x);
    camera.motionRotation.y = static_cast<std::int16_t>(camera.nextRotation.y - camera.rotation.y);
    camera.motionRotation.z = static_cast<std::int16_t>(camera.nextRotation.z - camera.rotation.z);
}

#endif

// @verified - d
void camCtrlCalculateFov(CameraRuntime &camera) {
    camera.fullVerticalFov = camera.currentFovAngle;
    camera.halfVerticalFovTangent = FixedAngleTangent(camera.fullVerticalFov / 2);
    camera.verticalFovAngle = static_cast<std::int16_t>(dc::sdk::ArcTanFixed(camera.halfVerticalFovTangent * 0.75f) * 2U);
    camera.pitchAngleTangent = FixedAngleTangent(camera.verticalFovAngle / 2);
    camera.viewHeightScale = 240.0f / camera.pitchAngleTangent;
}

// @verified - d
std::int32_t camCtrlIsConversationActive() {
    return static_cast<std::int32_t>(g_conversationCameraState->flags) >> 1 & 1;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void camCtrlRecalcFov(CameraRuntime &camera) {
    if (!(camera.flags & 0x8000U))
        return;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundZ_Wrapper(-camera.nextRotation.z);
    Matrix4x4::RotateAroundX_Wrapper(-camera.nextRotation.x);
    Matrix4x4::RotateAroundY_Wrapper(-camera.nextRotation.y);
    Matrix4x4::preTranslate(-camera.nextPosition.x, -camera.nextPosition.y, -camera.nextPosition.z);
    Vector3 local;
    Matrix4x4::TransformPoint(camera.scale, local);
    PopCurrentMatrix(1);
    float scale = camera.scaleFactor;
    if (camera.flags & 0x10000U) {
        camera.flags &= ~0x10000U;
        if ((local.z < 0.0f ? -local.z : local.z) < 0.001f) {
            camera.flags &= ~0x8000U;
            return;
        }
        camCtrlCalculateFov(camera);
        scale = camera.viewHeightScale / local.z;
        camera.scaleFactor = scale;
    }
    camera.currentFovAngle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(320.0f, scale * local.z) * 2U);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlSetup(CameraRuntime &camera) {
    if (!camera.savedPresetIndex)
        return;
    CameraPreset *preset = g_cameraCachedPresets;
    CameraPreset &saved = g_cameraCachedPresets[0];
    saved.position.x = camera.nextPosition.x;
    saved.position.y = camera.nextPosition.y;
    saved.position.z = camera.nextPosition.z;
    saved.rotation.x = camera.nextRotation.x;
    saved.rotation.y = camera.nextRotation.y;
    saved.rotation.z = camera.nextRotation.z;
    saved.fovAngle = camera.currentFovAngle;
    Vector3 *savedDirection = &preset->backwardsDirection;
    Vector3 *direction = &camera.backwardsDirection;
    *savedDirection = *direction;
    matrix_stack::CopyMatrix(&saved.projectionMatrix, &camera.projectionMatrix);
    matrix_stack::CopyMatrix(&saved.viewMatrix, &camera.viewMatrix);
    preset += camera.savedPresetIndex;
    const CameraPreset &selected = *preset;
    camera.nextPosition.x = selected.position.x;
    camera.nextPosition.y = selected.position.y;
    camera.nextPosition.z = selected.position.z;
    camera.nextRotation.x = selected.rotation.x;
    camera.nextRotation.y = selected.rotation.y;
    camera.nextRotation.z = selected.rotation.z;
    camera.currentFovAngle = selected.fovAngle;
    camCtrlCreatePerspectiveMatrix(camera);
    camera.nextPosition.x = saved.position.x;
    camera.nextPosition.y = saved.position.y;
    camera.nextPosition.z = saved.position.z;
    camera.nextRotation.x = saved.rotation.x;
    camera.nextRotation.y = saved.rotation.y;
    camera.nextRotation.z = saved.rotation.z;
    camera.currentFovAngle = saved.fovAngle;
    *direction = *savedDirection;
    matrix_stack::CopyMatrix(&camera.projectionMatrix, &saved.projectionMatrix);
    matrix_stack::CopyMatrix(&camera.viewMatrix, &saved.viewMatrix);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void camCtrlHandleStates(CameraRuntime &camera) {
    const std::uint32_t rebuild = camera.flags & 6U;
    camera.flags &= ~6U;
    if (rebuild)
        camera.flags |= 0x80U;
    switch (camera.state) {
    case 0:
        camCtrlSwitchColiPhase(camera);
        break;
    case 27:
        camCtrlSetCamSnapAngY(camera);
        break;
    case 6:
        if (rebuild)
            camCtrlBuildScriptCamSpline(static_cast<std::int32_t>(camera.cameraId), *reinterpret_cast<const void *const *>(camera.splineState),
                *reinterpret_cast<const void *const *>(camera.splineState + 4));
        camCtrlHandleQueuedSpline();
        break;
    case 8:
        if (rebuild) {
            camCtrlCreateSpline(reinterpret_cast<const std::uint32_t *>(static_cast<std::uint32_t>(camera.cameraId)),
                *reinterpret_cast<const void *const *>(camera.splineState), *reinterpret_cast<const void *const *>(camera.splineState + 4));
            camCtrlHandleQueuedSpline();
            camera.flags &= ~0x80000U;
        }
        if (!(camera.flags & 0x80000U))
            camCtrlHandleQueuedSpline();
        if (camera.flags & 0x400000U) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(&camera.finalTransform);
            Matrix4x4::TransformPoint(camera.previousPosition, camera.previousPosition);
            Matrix4x4::TransformPoint(camera.targetPosition, camera.targetPosition);
            PopCurrentMatrix(1);
        }
        break;
    case 14:
        camCtrlHandleQueuedSpline();
        break;
    case 7:
        UpdateConversationCamera();
        SetConversationCameraEnabled(-1);
        break;
    case 10:
        camera.flags |= 0x400U;
        if (rebuild)
            InitializeCameraGoal();
        camCtrlEvaluateGoal();
        break;
    case 22:
        camCtrlHandleLookAroundCam(camera);
        break;
    case 24:
        UpdateLockedOnCamera();
        break;
    case 26:
        if (camera.updateCallback)
            camera.updateCallback(&camera);
        break;
    case 25:
        camera.flags |= 0x100U;
        break;
    }
    camera.flags &= ~0x400000U;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float GenerateOscillationForCamera(std::int32_t phase) {
    const float first = dc::sdk::SinFixed(22 * phase);
    const float second = dc::sdk::SinFixed(31 * phase);
    const float third = dc::sdk::SinFixed(103 * phase);
    const float fourth = dc::sdk::SinFixed(147 * phase);
    return first * 0.30000001f + second * 0.37f + third * 0.15000001f + fourth * 0.18000001f;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const volatile float cameraDefaultMaximumScale = 0.08399999886751175f;
static const volatile std::int32_t cameraDefaultMaximumSpeed = 8;
static const volatile float cameraDefaultMinimumScale = 0.09399999678134918f;
static const volatile std::int32_t cameraDefaultMinimumSpeed = 6;
static const volatile float cameraMediumMaximumScale = 0.227f;
static const volatile std::int32_t cameraMediumMaximumSpeed = 21;
static const volatile float cameraMediumMinimumScale = 0.212f;
static const volatile std::int32_t cameraMediumMinimumSpeed = 16;
static const volatile float cameraNearTargetMaximumScale = 0.001f;
static const volatile std::int32_t cameraNearTargetMaximumSpeed = 6;
static const volatile float cameraNearTargetMinimumScale = 0.002f;
static const volatile std::int32_t cameraNearTargetMinimumSpeed = 4;
static const volatile float cameraMinimumMotionScale = 0.0f;
static const volatile float cameraMediumMotionScale = 0.125f;
static const volatile float cameraMaximumMotionScale = 0.5f;
static const volatile float cameraNearTargetDistance = 0.5f;

void camCtrlSmoothMotion(CameraRuntime &camera) {
    if (!(camera.flags & CameraSmoothMotion))
        return;
    const float eyeMagnitude = Vector3::CalcMagnitude(camera.motionVector);
    const float targetMagnitude = Vector3::CalcMagnitude(camera.adjustedMotionVector);
    const float magnitude = eyeMagnitude > targetMagnitude ? eyeMagnitude : targetMagnitude;
    float minimumScale = 0.0f;
    float maximumScale = minimumScale;
    std::int32_t minimumSpeed = 0, maximumSpeed = 0;
    if (magnitude < cameraMinimumMotionScale) {
        maximumSpeed = cameraDefaultMaximumSpeed;
        minimumSpeed = cameraDefaultMinimumSpeed;
        maximumScale = cameraDefaultMaximumScale;
        minimumScale = cameraDefaultMinimumScale;
    } else {
        float span = cameraMediumMotionScale;
        if (magnitude < span) {
            const float mediumMaximum = cameraMediumMaximumScale;
            const float defaultMaximum = cameraDefaultMaximumScale;
            const float minimumMotion = cameraMinimumMotionScale;
            span -= minimumMotion;
            const float offset = magnitude - minimumMotion;
            maximumScale = ((mediumMaximum - defaultMaximum) / span) * offset + defaultMaximum;
            const float mediumMinimum = cameraMediumMinimumScale;
            const float defaultMinimum = cameraDefaultMinimumScale;
            minimumScale = ((mediumMinimum - defaultMinimum) / span) * offset + defaultMinimum;
            const std::int32_t mediumMaxSpeed = cameraMediumMaximumSpeed;
            const std::int32_t defaultMaxSpeed = cameraDefaultMaximumSpeed;
            maximumSpeed = static_cast<std::int32_t>((static_cast<float>(mediumMaxSpeed - defaultMaxSpeed) / span) * offset + defaultMaxSpeed);
            const std::int32_t mediumMinSpeed = cameraMediumMinimumSpeed;
            const std::int32_t defaultMinSpeed = cameraDefaultMinimumSpeed;
            minimumSpeed = static_cast<std::int32_t>((static_cast<float>(mediumMinSpeed - defaultMinSpeed) / span) * offset + defaultMinSpeed);
        } else {
            span = cameraMaximumMotionScale;
            if (magnitude < span) {
                const float mediumMaximum = cameraMediumMaximumScale;
                const float mediumMotion = cameraMediumMotionScale;
                span -= mediumMotion;
                const float offset = magnitude - mediumMotion;
                maximumScale = (-mediumMaximum / span) * offset + mediumMaximum;
                const float mediumMinimum = cameraMediumMinimumScale;
                minimumScale = (-mediumMinimum / span) * offset + mediumMinimum;
                const std::int32_t mediumMaxSpeed = cameraMediumMaximumSpeed;
                maximumSpeed = static_cast<std::int32_t>((static_cast<float>(-mediumMaxSpeed) / span) * offset + mediumMaxSpeed);
                const std::int32_t mediumMinSpeed = cameraMediumMinimumSpeed;
                minimumSpeed = static_cast<std::int32_t>((static_cast<float>(-mediumMinSpeed) / span) * offset + mediumMinSpeed);
            }
        }
    }
    if (GetEuclideanDistance(&camera.nextPosition, &camera.targetPosition) < cameraNearTargetDistance) {
        maximumSpeed = cameraNearTargetMaximumSpeed;
        minimumSpeed = cameraNearTargetMinimumSpeed;
        maximumScale = cameraNearTargetMaximumScale;
        minimumScale = cameraNearTargetMinimumScale;
    }
    const float scaleFactor = camera.motionScaleFactor;
    maximumScale *= scaleFactor;
    minimumScale *= scaleFactor;
    float previousMagnitude = Vector3::CalcMagnitude(camera.motionScale);
    const float previousTarget = Vector3::CalcMagnitude(camera.interpolationScale);
    if (previousTarget > previousMagnitude)
        previousMagnitude = previousTarget;
    if (::fabsf(previousMagnitude - magnitude) < cameraMaximumMotionScale) {
        camera.currentMaximumScale = (maximumScale - camera.currentMaximumScale) * 0.5f + camera.currentMaximumScale;
        camera.cameraMinimumScale = (minimumScale - camera.cameraMinimumScale) * 0.5f + camera.cameraMinimumScale;
        camera.interpolatedMaximumSpeed += (maximumSpeed - camera.interpolatedMaximumSpeed) >> 1;
        camera.currentMinimumSpeed += (minimumSpeed - camera.currentMinimumSpeed) >> 1;
    }
    const std::int32_t maximumPhase = static_cast<std::int32_t>(static_cast<std::uint32_t>(camera.cumulativeMaximumSpeed) + camera.interpolatedMaximumSpeed);
    camera.cumulativeMaximumSpeed = maximumPhase;
    camera.cumulativeMinimumSpeed = static_cast<std::int32_t>(static_cast<std::uint32_t>(camera.cumulativeMinimumSpeed) + camera.currentMinimumSpeed);
    camera.oscillation.x = camera.currentMaximumScale * GenerateOscillationForCamera(maximumPhase);
    camera.oscillation.y = camera.cameraMinimumScale * GenerateOscillationForCamera(camera.cumulativeMinimumSpeed);
    camera.oscillation.z = 0.0f;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ApplyRotationMatrixFromTransform(CameraRuntime &camera) {
    if (!(camera.flags & CameraSmoothMotion))
        return;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y);
    Matrix4x4::RotateAroundX_Wrapper(camera.nextRotation.x);
    Matrix4x4::RotateAroundZ_Wrapper(camera.nextRotation.z);
    Vector3 offset;
    Matrix4x4::TransformPoint(camera.oscillation, offset);
    PopCurrentMatrix(1);
    camera.nextPosition.x += offset.x;
    camera.nextPosition.y += offset.y;
    camera.nextPosition.z += offset.z;
}

#endif

std::int32_t FindFieldSegmentOffsetInWorld(Vector3 *output, const Vector3 *destination, const Vector3 *origin);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void camCtrlCheckCollision(CameraRuntime &camera) {
    float pull = 0.0f;
    camera.flags &= ~0x40000U;
    if (!(camera.flags & 0x400U) || (static_cast<std::uint8_t>(camera.interpolationSourceMode - 1) > 1 && camera.interpolationSourceMode != 4))
        return;
    Vector3 hit;
    Vector3 delta;
    Vector3 direction;
    am2Vector3Subtract(&delta, &camera.nextTargetPosition, &camera.nextPosition);
    float distance = am2Vector3Normalize(&delta, &direction);
    Vector3 target = camera.nextTargetPosition;
    if (distance > 0.5f) {
        pull = -0.03f;
        am2Vector3Scale(&delta, &direction, pull);
        am2Vector3Add(&target, &camera.nextTargetPosition, &delta);
    }
    std::int32_t found = FindFieldSegmentOffsetInWorld(&hit, &camera.nextPosition, &target);
    if (!found) {
        Vector3 intersection;
        Vector3 rightStart, leftStart, rightEnd, leftEnd;
        rightStart.x = 0.1f, rightStart.y = 0.0f, rightStart.z = 0.0f;
        leftStart.x = -0.1f, leftStart.y = 0.0f, leftStart.z = 0.0f;
        rightEnd.x = 0.1f, rightEnd.y = 0.0f, rightEnd.z = 0.0f;
        leftEnd.x = -0.1f, leftEnd.y = 0.0f, leftEnd.z = 0.0f;
        distance -= pull;
        rightStart.z = leftStart.z = -distance;
        float leftDistance = distance, rightDistance = distance;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&target);
        Matrix4x4::RotateAroundY_Wrapper(camera.nextRotation.y + 0x8000);
        Matrix4x4::RotateAroundX_Wrapper(-camera.nextRotation.x);
        Matrix4x4::TransformPoint(rightStart, rightStart);
        Matrix4x4::TransformPoint(leftStart, leftStart);
        Matrix4x4::TransformPoint(rightEnd, rightEnd);
        Matrix4x4::TransformPoint(leftEnd, leftEnd);
        PopCurrentMatrix(1);
        if (FindFieldSegmentOffsetInWorld(&intersection, &rightStart, &rightEnd))
            rightDistance = CalculateDistanceBetweenPoints(&rightEnd, &intersection);
        if (FindFieldSegmentOffsetInWorld(&intersection, &leftStart, &leftEnd))
            leftDistance = CalculateDistanceBetweenPoints(&leftEnd, &intersection);
        if (distance > rightDistance || distance > leftDistance) {
            found = 1;
            distance = rightDistance < leftDistance ? rightDistance : leftDistance;
            am2Vector3Scale(&delta, &direction, -distance);
            am2Vector3Add(&hit, &target, &delta);
        }
    }
    if (found) {
        am2Vector3Scale(&delta, &direction, 0.01f);
        am2Vector3Add(&camera.nextPosition, &hit, &delta);
        camera.flags |= 0x40000U;
    }
}
#endif

// @verified - d
void CameraControlNullsub() {
}

// @verified - d
std::int32_t camCtrlGetContextMode() {
    return g_cameraControl->contextMode;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t camCtrlGetActiveContextMode() {
    return g_cameraControl->contextMode;
}

void camCtrlSetTargetHeightDistance(float distance) {
    g_cameraControl->targetHeightDistance = distance;
}

void camCtrlSetBasePitchBias(std::int16_t bias) {
    g_cameraControl->basePitchBias = bias;
}
#endif

// @verified - d
std::int32_t camCtrlSetContext(std::int32_t enabled, std::int32_t retainSettings) {
    CameraRuntime *const camera = g_cameraControl;
    std::int32_t previous;
    if (enabled) {
        previous = camCtrlSetContextMode(10);
        if (!retainSettings) {
            camera->contextMode = 0;
            camera->targetHeightDistance = cameraContextSettings[5].distanceFromPlayer;
            camera->basePitchBias = cameraContextSettings[5].basePitchBias;
        }
    } else
        previous = camCtrlSetContextMode(0);
    if (!camera->state) {
        camCtrlSetInterpolationMode(3);
        camCtrlSetInterpolationFrameCount(30);
    }
    return previous;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void camCtrlApplyJitterAdjustment(CameraRuntime &camera) {
    if (::fabsf(camera.shakeAmplitude.x) < 0.003f)
        camera.shakeAmplitude.x = 0.0f;
    if (::fabsf(camera.shakeAmplitude.y) < 0.003f)
        camera.shakeAmplitude.y = 0.0f;
    camera.jitter.x = 0.0f;
    camera.jitter.y = 0.0f;
    if (camera.shakeAmplitude.x == 0.0f && camera.shakeAmplitude.y == 0.0f)
        return;
    camera.jitter.x = camera.shakeAmplitude.x * ((0.5f - dc::sdk::RandomFloat()) * 0.5f + 1.0f);
    camera.jitter.y = camera.shakeAmplitude.y * ((0.5f - dc::sdk::RandomFloat()) * 0.5f + 1.0f);
    camera.shakeAmplitude.x *= camera.shakeAmplitude.z;
    camera.shakeAmplitude.y *= -camera.shakeAmplitude.z;
    if (camera.shakeAmplitude.y != 0.0f || (dc::sdk::RandomInt() & 3U))
        camera.shakeAmplitude.x = -camera.shakeAmplitude.x;
}
#endif

// @verified - d
void camCtrlSetShakeAmplitude(float x, float y, float duration) {
    CameraRuntime *const camera = g_cameraControl;
    if (!(duration > 0.0f)) {
        camera->shakeAmplitude.x = 0.0f;
        camera->shakeAmplitude.y = 0.0f;
        camera->shakeAmplitude.z = 1.0f;
    } else {
        camera->shakeAmplitude.x = x;
        camera->shakeAmplitude.y = y;
        camera->shakeAmplitude.z = duration;
    }
    camera->jitter.x = 0.0f;
    camera->jitter.y = 0.0f;
}

// @verified - d
void camCtrlSetRotationXYZ(float x, float y, float duration) {
    camCtrlSetShakeAmplitude(x, y, duration);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlResolveNextPositionCollision(CameraRuntime &camera) {
    ResolveFieldSphereVolumeInWorld(&camera.nextPosition, &camera.nextPosition, 0.1f);
}

std::uint32_t camCtrlExchangeFlags(std::uint32_t flags) {
    CameraRuntime *const camera = g_cameraControl;
    const std::uint32_t previous = camera->flags;
    camera->flags = flags;
    return previous;
}
#endif

// @verified - d
std::uint32_t camCtrlGetFlags() {
    return g_cameraControl->flags;
}

std::uint32_t camCtrlClearFlags(std::uint32_t mask) {
    const std::uint32_t previous = g_cameraControl->flags;
    g_cameraControl->flags &= ~mask;
    return previous;
}

std::uint32_t camCtrlOrFlags(std::uint32_t mask) {
    const std::uint32_t previous = g_cameraControl->flags;
    g_cameraControl->flags |= mask;
    return previous;
}

// @verified - d
std::int32_t camCtrlSetState(std::int32_t state) {
    CameraRuntime *const camera = g_cameraControl;
    if (camera->state == state)
        return state;
    camera->previousState = camera->state;
    camera->state = static_cast<std::int16_t>(state);
    camera->flags = (camera->flags | 2U) & ~0x800000U;
    switch (state) {
    case 0:
        if (camera->collisionPhase == 1)
            camera->collisionPhase = 2;
        if (camera->previousState == 7)
            camera->flags |= 0x800000U;
        break;
    case 22:
    case 24:
        break;
    case 26:
    default:
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        control->ryo->alpha = 1.0f;
        break;
    }
    camCtrlSetInterpolationMode(-1);
    return camera->previousState;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlRestorePreviousState() {
    camCtrlSetState(g_cameraControl->previousState);
}

void camCtrlStateNullsub() {
}

std::int32_t camCtrlSetDefaultState(std::int16_t state) {
    CameraRuntime *const camera = g_cameraControl;
    const std::int32_t previous = camera->defaultState;
    camera->defaultState = state;
    return previous;
}
#endif

// @verified - d
std::int32_t camCtrlGetState() {
    return g_cameraControl->state;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t camCtrlGetDefaultState() {
    return g_cameraControl->defaultState;
}
#endif

void camCtrlSetInterpolationSourceMode(std::uint8_t mode) {
    g_cameraControl->interpolationSourceMode = mode;
}

// @verified - d
void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source) {
    std::memcpy(&g_cameraControl->cameraId, source, static_cast<std::uint32_t>(count) << 2U);
    g_cameraControl->flags |= 4U;
}

// @verified - d
void camCtrlSetNextPosition(const Vector3 &position) {
    g_cameraControl->previousPosition.x = position.x;
    g_cameraControl->previousPosition.y = position.y;
    g_cameraControl->previousPosition.z = position.z;
}

// @verified - d
void camCtrlSetTargetPosition(const Vector3 &position) {
    g_cameraControl->targetPosition.x = position.x;
    g_cameraControl->targetPosition.y = position.y;
    g_cameraControl->targetPosition.z = position.z;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlSetAnchorOffset(const Vector3 &offset) {
    g_cameraControl->anchorOffset.x = offset.x;
    g_cameraControl->anchorOffset.y = offset.y;
    g_cameraControl->anchorOffset.z = offset.z;
}
#endif

// @verified - d
Vector3 *camCtrlGetNextCamPos(Vector3 *position) {
    CameraRuntime *const camera = g_cameraControl;
    if (position) {
        position->x = camera->nextPosition.x;
        position->y = camera->nextPosition.y;
        position->z = camera->nextPosition.z;
    }
    return &camera->nextPosition;
}

// @verified - d
Vector3 *camCtrlGetTargetPosition(Vector3 *position) {
    CameraRuntime *const camera = g_cameraControl;
    if (position) {
        position->x = camera->targetPosition.x;
        position->y = camera->targetPosition.y;
        position->z = camera->targetPosition.z;
    }
    return &camera->targetPosition;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlSetOrbitDistance(float distance) {
    g_cameraControl->orbitDistance = distance;
}

void camCtrlSetZOffset(float offset) {
    g_cameraControl->zOffset = offset;
}

Vector3 *camCtrlGetCameraPosition(Vector3 *position) {
    return camCtrlGetNextCamPos(position);
}

Vector3 *camCtrlGetPreviousPosition(Vector3 *position) {
    CameraRuntime *const camera = g_cameraControl;
    if (position) {
        position->x = camera->previousPosition.x;
        position->y = camera->previousPosition.y;
        position->z = camera->previousPosition.z;
    }
    return &camera->previousPosition;
}

Vector3 *camCtrlGetPos(Vector3 *position) {
    CameraRuntime *const camera = g_cameraControl;
    if (position) {
        position->x = camera->targetPosition.x;
        position->y = camera->targetPosition.y;
        position->z = camera->targetPosition.z;
    }
    return &camera->targetPosition;
}

void camCtrlGetPreviousPositionOffset(Vector3 *offset) {
    CameraRuntime *const camera = g_cameraControl;
    if (offset)
        am2Vector3Subtract(offset, &camera->previousPosition, &camera->nextPosition);
}

void camCtrlGetTargetPositionOffset(Vector3 *offset) {
    CameraRuntime *const camera = g_cameraControl;
    if (offset)
        am2Vector3Subtract(offset, &camera->targetPosition, &camera->nextTargetPosition);
}

void camCtrlAddNextPosition(const Vector3 *offset) {
    CameraRuntime *const control = g_cameraControl;
    if (offset)
        am2Vector3AddInPlace(&control->previousPosition, offset);
}

void camCtrlAddTargetPosition(const Vector3 *offset) {
    CameraRuntime *const control = g_cameraControl;
    if (offset)
        am2Vector3AddInPlace(&control->targetPosition, offset);
}
#endif

// @verified - d
void camCtrlSetNextCamRot(const Vector3s *rotation) {
    const std::int16_t *source = &rotation->x;
    g_cameraControl->nextRotation.x = *source++;
    g_cameraControl->nextRotation.y = *source++;
    g_cameraControl->nextRotation.z = *source;
}

// @verified - d
void camCtrlSetNextRoll(std::int16_t roll) {
    g_cameraControl->nextRotation.z = roll;
}

std::int32_t camCtrlGetNextRoll() {
    return g_cameraControl->nextRotation.z;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlSetNextRollWrapper(std::int32_t roll) {
    camCtrlSetNextRoll(roll);
}
#endif

// @verified - d
Vector3s *camCtrlGetRot(Vector3s *rotation) {
    CameraRuntime *const camera = g_cameraControl;
    if (rotation) {
        rotation->x = camera->nextRotation.x;
        rotation->y = camera->nextRotation.y;
        rotation->z = camera->nextRotation.z;
    }
    return &camera->nextRotation;
}

// @verified - d
std::int32_t camCtrlGetNextRotY() {
    return g_cameraControl->nextRotation.y;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlGetOffsetFromBase(Vector3 *translation, Vector3s *rotation) {
    CameraRuntime *const camera = g_cameraControl;
    if (translation) {
        translation->x = camera->nextPosition.x - camera->position.x;
        translation->y = camera->nextPosition.y - camera->position.y;
        translation->z = camera->nextPosition.z - camera->position.z;
    }
    if (rotation) {
        rotation->x = static_cast<std::int16_t>(camera->nextRotation.x - camera->rotation.x);
        rotation->y = static_cast<std::int16_t>(camera->nextRotation.y - camera->rotation.y);
        rotation->z = static_cast<std::int16_t>(camera->nextRotation.z - camera->rotation.z);
    }
}
#endif

// @verified - d
void camCtrlGetAdjustedPitchOrYaw(std::uint16_t *base, std::uint16_t *adjusted) {
    const CameraRuntime &camera = *g_cameraControl;
    if (base) {
        base[0] = static_cast<std::uint16_t>(camera.basePitchBias);
        base[1] = static_cast<std::uint16_t>(camera.nextRotation.y);
    }
    if (adjusted) {
        adjusted[0] = static_cast<std::uint16_t>(camera.nextRotation.x - camera.basePitchBias);
        adjusted[1] = 0;
    }
}

// @verified - d
std::int32_t camCtrlGetCurrentFov() {
    return g_cameraControl->currentFovAngle;
}

// @verified - d
void camCtrlSetRotation(const Vector3 *translation, const Vector3s *rotation) {
    CameraRuntime *const camera = g_cameraControl;
    if (translation) {
        camera->rotationOffset.x = translation->x;
        camera->rotationOffset.y = translation->y;
        camera->rotationOffset.z = translation->z;
    } else {
        camera->rotationOffset.x = 0.0f;
        camera->rotationOffset.y = 0.0f;
        camera->rotationOffset.z = 0.0f;
    }
    if (rotation) {
        camera->angularOffset.x = rotation->x;
        camera->angularOffset.y = rotation->y;
        camera->angularOffset.z = rotation->z;
    } else {
        camera->angularOffset.x = 0;
        camera->angularOffset.y = 0;
        camera->angularOffset.z = 0;
    }
}

// @verified - d
void camCtrlGetRotationXform(Vector3 *translation, Vector3s *rotation) {
    CameraRuntime *const camera = g_cameraControl;
    if (translation) {
        translation->x = camera->rotationOffset.x;
        translation->y = camera->rotationOffset.y;
        translation->z = camera->rotationOffset.z;
    }
    if (rotation) {
        rotation->x = camera->angularOffset.x;
        rotation->y = camera->angularOffset.y;
        rotation->z = camera->angularOffset.z;
    }
}

// @verified - d
CameraRotationCallback camCtrlSetRotationCallback(CameraRotationCallback callback) {
    const CameraRotationCallback previous = g_cameraControl->rotationCallback;
    g_cameraControl->rotationCallback = callback;
    return previous;
}

// @verified - d
std::int16_t camCtrlGetFov() {
    return g_cameraControl->fovAngle;
}

// @verified - d
std::int32_t camCtrlSetFov(std::int16_t angle) {
    const std::int32_t previous = g_cameraControl->fovAngle;
    g_cameraControl->fovAngle = angle;
    return previous;
}

// @verified - d
std::int32_t camCtrlSetPerspective(std::int16_t perspective) {
    const std::int32_t previous = g_cameraControl->currentFovAngle;
    g_cameraControl->currentFovAngle = perspective;
    return previous;
}

// @verified - d
std::int16_t camCtrlGetPerspective() {
    return g_cameraControl->currentFovAngle;
}

std::int32_t camCtrlSetPerspectiveWrapper(std::int16_t perspective) {
    return camCtrlSetPerspective(perspective);
}

std::int32_t camCtrlGetPerspectiveWrapper() {
    return camCtrlGetPerspective();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float camCtrlGetViewHeightScale() {
    return g_cameraControl->viewHeightScale;
}
#endif

// @verified - d
Matrix4x4 *camCtrlGetProjectionMatrix(Matrix4x4 *output) {
    if (output)
        std::memcpy(output, &g_cameraControl->projectionMatrix, sizeof(*output));
    return &g_cameraControl->projectionMatrix;
}

// @verified - d
void shenSetDepthScale(float farDistance, float nearDistance, float npcRadius, float horizontalTangent, float verticalTangent) {
    fieldDepthScale.farDistance = farDistance;
    fieldDepthScale.nearDistance = nearDistance;
    fieldDepthScale.horizontalTangent = horizontalTangent;
    fieldDepthScale.verticalTangent = verticalTangent;
    fieldDepthSavedScale.npcRadius = npcRadius;
    fieldDepthSavedScale.horizontalTangent = horizontalTangent;
    fieldDepthSavedScale.verticalTangent = verticalTangent;
}

// @verified - d
bool camCtrlIsInsideDepthScaledRect(const Vector3 &position, float radius) {
    Vector4 transformed;
    matrix_stack::TransformPosition(&position, &transformed);
    const float back = radius + -transformed.z;
    if (0.0f > back)
        return false;
    const float horizontal = fieldDepthSavedScale.horizontalTangent * back + radius;
    if (transformed.x > horizontal || !(transformed.x > -horizontal))
        return false;
    const float vertical = fieldDepthSavedScale.verticalTangent * back + radius;
    if (transformed.y > vertical || !(transformed.y > -vertical))
        return false;
    return true;
}

// @verified - d
bool camCtrlIsSphereVisible(const Vector3 &position, float radius) {
    Vector4 transformed;
    matrix_stack::TransformPosition(&position, &transformed);
    const float depth = -transformed.z, extent = radius + depth;
    if (0.0f > extent || depth - radius > fieldDepthScale.farDistance)
        return false;
    const float horizontal = fieldDepthScale.horizontalTangent * extent + radius;
    if (transformed.x > horizontal || !(transformed.x > -horizontal))
        return false;
    const float vertical = fieldDepthScale.verticalTangent * extent + radius;
    return !(transformed.y > vertical) && transformed.y > -vertical;
}

// @verified - d
bool camCtrlIsNearSphereVisible(const Vector3 &position, float radius) {
    Vector4 transformed;
    matrix_stack::TransformPosition(&position, &transformed);
    const float depth = -transformed.z, extent = radius + depth;
    if (0.0f > extent || depth - radius > fieldDepthScale.nearDistance)
        return false;
    const float horizontal = fieldDepthScale.horizontalTangent * extent + radius;
    if (transformed.x > horizontal || !(transformed.x > -horizontal))
        return false;
    const float vertical = fieldDepthScale.verticalTangent * extent + radius;
    return !(transformed.y > vertical) && transformed.y > -vertical;
}

// @verified - d
bool camCtrlIsFarSphereVisible(const Vector3 &position, float radius) {
    Vector4 transformed;
    matrix_stack::TransformPosition(&position, &transformed);
    const float depth = -transformed.z;
    if (depth - radius > fieldDepthScale.farDistance)
        return false;
    const float extent = radius + depth;
    if (fieldDepthScale.nearDistance > extent)
        return false;
    const float horizontal = fieldDepthScale.horizontalTangent * extent + radius;
    if (transformed.x > horizontal || !(transformed.x > -horizontal))
        return false;
    const float vertical = fieldDepthScale.verticalTangent * extent + radius;
    return !(transformed.y > vertical) && transformed.y > -vertical;
}

// @verified - d
bool camCtrlIsScreenSpaceVisible(const Vector3 &position, float radius) {
    Vector4 transformed;
    matrix_stack::TransformPosition(&position, &transformed);
    const float extent = radius - transformed.z;
    if (0.0f > extent)
        return false;
    const float horizontal = fieldDepthScale.horizontalTangent * extent + radius;
    if (transformed.x > horizontal || !(transformed.x > -horizontal))
        return false;
    const float vertical = fieldDepthScale.verticalTangent * extent + radius;
    return !(transformed.y > vertical) && transformed.y > -vertical;
}

// @verified - d
bool ScreenVisCheck(const Vector3 &position, float radius, float depth) {
    Vector4 transformed;
    matrix_stack::TransformPosition(&position, &transformed);
    const float back = -transformed.z;
    const float extent = radius + back;
    if (0.0f > extent || back - radius > depth)
        return false;
    const float horizontal = fieldDepthScale.horizontalTangent * extent + radius;
    if (transformed.x > horizontal || !(transformed.x > -horizontal))
        return false;
    const float vertical = fieldDepthScale.verticalTangent * extent + radius;
    return !(transformed.y > vertical) && transformed.y > -vertical;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
Matrix4x4 *camCtrlGetViewMatrix(Matrix4x4 *output) {
    if (output)
        *output = g_cameraControl->viewMatrix;
    return &g_cameraControl->viewMatrix;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Matrix4x4 *camCtrlSetXform(const Matrix4x4 *transform) {
    if (transform) {
        g_cameraControl->finalTransform = *transform;
        g_cameraControl->flags |= 0x400000U;
    }
    return &g_cameraControl->finalTransform;
}

Vector3 *camCtrlGetBackwardsDir(Vector3 *direction) {
    if (direction) {
        CameraRuntime *camera;
        camera = g_cameraControl;
        *direction = camera->backwardsDirection;
    }
    return &g_cameraControl->backwardsDirection;
}

std::int32_t camCtrlIsUnknown23State() {
    if (g_cameraControl->state == 23)
        return 1;
    return 0;
}
#endif

// @verified - d
CameraUpdateCallback camCtrlSetUpdateCallback(CameraUpdateCallback callback) {
    const CameraUpdateCallback previous = g_cameraControl->updateCallback;
    g_cameraControl->updateCallback = callback;
    return previous;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
CameraUpdateCallback camCtrlSetProjectionCallback(CameraUpdateCallback callback) {
    const CameraUpdateCallback previous = *reinterpret_cast<CameraUpdateCallback *>(g_cameraControl->retainedCallbackState);
    *reinterpret_cast<CameraUpdateCallback *>(g_cameraControl->retainedCallbackState) = callback;
    return previous;
}
#endif

// @verified - d
void camCtrlSetScaleTo(const Vector3 *scale) {
    if (g_cameraControl->flags & 0x8000U) {
        g_cameraControl->scale.x = scale->x;
        g_cameraControl->scale.y = scale->y;
        g_cameraControl->scale.z = scale->z;
        g_cameraControl->flags |= 0x10000U;
    }
}

// @verified - d
void camCtrlSetScaleToAndRecalcFov(const Vector3 *scale) {
    g_cameraControl->scale.x = scale->x;
    g_cameraControl->scale.y = scale->y;
    g_cameraControl->scale.z = scale->z;
    g_cameraControl->flags |= 0x18000U;
}

// @verified - d
void camCtrlSetFlags() {
    g_cameraControl->flags &= 0xfffe7fffU;
}

// @verified - d
void camCtrlSetFovTransitionTo(std::int32_t enabled) {
    CameraRuntime *const camera = g_cameraControl;
    if (enabled) {
        camera->flags |= 0x200U;
        camera->fovLerpIterations = 5;
    } else {
        camera->flags &= ~0x200U;
        camera->fovLerpIterations = 0;
    }
}

// @verified - d
std::int32_t camCtrlSetContextMode(std::int32_t mode) {
    CameraRuntime *const camera = g_cameraControl;
    const std::int32_t previous = camera->contextMode;
    camera->flags &= ~0x200000U;
    if (camera->contextMode == mode && mode == 10)
        camera->flags |= 0x200000U;
    if (camera->contextMode == mode)
        return previous;
    if (mode == 10) {
        camera->contextMode = static_cast<std::int8_t>(mode);
        camera->targetHeightDistance = cameraContextSettings[11].distanceFromPlayer;
        camera->basePitchBias = cameraContextSettings[11].basePitchBias;
        camera->flags |= 0x200000U;
    } else {
        std::int32_t index = static_cast<std::int32_t>(static_cast<std::uint32_t>(mode) + 5U);
        if (index < 0)
            index = 0;
        if (index > 10)
            index = 10;
        camera->contextMode = static_cast<std::int8_t>(mode);
        camera->targetHeightDistance = cameraContextSettings[index].distanceFromPlayer;
        camera->basePitchBias = cameraContextSettings[index].basePitchBias;
    }
    return previous;
}

// @verified - d
shen_name camCtrlSetTargetChara(shen_name name) {
    const shen_name previous = g_cameraControl->targetCharacter;
    g_cameraControl->targetCharacter = name;
    return previous;
}

// @verified - d
void camCtrlSetDistanceFromPlayerDefaults() {
    CameraRuntime *camera = g_cameraControl;
    camera->targetHeightDistance = 1.2f;
    camera->defaultOrbitDistance = 3.1f;
    camera->basePitchBias = -0x100;
    camera->contextMode = 0;
    camera->collisionPhase = 2;
    camera->interpolationTicks = 0;
    camera->targetCharacter = __typeof__(camera->targetCharacter)();
}

// @verified - d
std::int32_t camCtrlToggleSmoothing(std::int32_t enabled) {
    std::int32_t previous;
    previous = (g_cameraControl->flags >> 17) & 1;
    if (enabled)
        g_cameraControl->flags |= CameraSmoothMotion;
    else {
        g_cameraControl->flags &= ~CameraSmoothMotion;
        g_cameraControl->motionScaleFactor = 1.0f;
    }
    return previous;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void camCtrlSetMotionOscillation(float maximumScale, float minimumScale, std::int16_t maximumSpeed, std::int16_t minimumSpeed) {
    g_cameraControl->currentMaximumScale = maximumScale;
    g_cameraControl->cameraMinimumScale = minimumScale;
    g_cameraControl->interpolatedMaximumSpeed = maximumSpeed;
    g_cameraControl->currentMinimumSpeed = minimumSpeed;
}

void camCtrlResetMotionscaling() {
    g_cameraControl->cumulativeMaximumSpeed = 0;
    g_cameraControl->cumulativeMinimumSpeed = 0;
    g_cameraControl->motionScaleFactor = 1.0f;
}
#endif

// @verified - d
float camCtrlSetMotionScaleFactor(float factor) {
    const float previous = g_cameraControl->motionScaleFactor;
    g_cameraControl->motionScaleFactor = factor;
    return previous;
}

// @verified - d
std::int32_t camCtrlSetInterpolationMode(std::int32_t mode) {
    CameraRuntime *const camera = g_cameraControl;
    const std::int32_t previous = camera->interpolationMode;
    camCtrlGetNextCamPos(&camera->interpolationEye);
    camCtrlGetTargetPosition(&camera->interpolationTarget);
    camCtrlGetRot(&camera->interpolationRotation);
    camera->interpolationFov = camera->currentFovAngle;
    camera->interpolationDistance = camera->orbitDistance;
    camera->interpolationElapsed = 0.0f;
    camera->interpolationPendingState = 0;
    switch (mode) {
    case 7:
        if (camera->interpolationSourceMode != 2)
            camera->interpolationDistance = CalculateDistanceBetweenPoints(&camera->interpolationEye, &camera->interpolationTarget);

    case 1:
        camera->interpolationSpeed = 0.1f;
        break;
    case 8:
        if (camera->interpolationSourceMode != 2)
            camera->interpolationDistance = CalculateDistanceBetweenPoints(&camera->interpolationEye, &camera->interpolationTarget);

    case 3:
        camera->interpolationFrameCount = 30;
        break;
    case 0:
        break;
    case -1:
    default:
        mode = 0;
        break;
    }
    camera->interpolationMode = static_cast<std::int16_t>(mode);
    return previous;
}

// @verified - d
void camCtrlSetInterpolationSpeed(float speed) {
    if (speed < 0.0f)
        speed = 0.1f;
    g_cameraControl->interpolationSpeed = speed;
}

// @verified - d
void camCtrlSetInterpolationFrameCount(std::int32_t frames) {
    if (frames < 1)
        frames = 1;
    g_cameraControl->interpolationFrameCount = static_cast<std::int16_t>(frames);
}

// @verified - d
bool camCtrlIsTransitionActive() {
    if (g_cameraControl->interpolationMode)
        return true;
    return false;
}

// @verified - d
std::int32_t camCtrlUtil(std::int32_t operation, void *parameter) {
    std::int32_t result = 0;
    CameraRuntime *const camera = g_cameraControl;
    const std::int32_t *const settings = reinterpret_cast<const std::int32_t *>(parameter);
    switch (operation) {
    case 10:
        camCtrlSetInterpolationMode(1);
        break;
    case 11:
        camCtrlSetInterpolationSpeed(*reinterpret_cast<const float *>(parameter));
        break;
    case 12:
        camera->targetHeightDistance = *reinterpret_cast<const float *>(parameter);
        break;
    case 13:
        camera->defaultOrbitDistance = *reinterpret_cast<const float *>(parameter);
        break;
    case 14:
        camera->basePitchBias = static_cast<std::int16_t>(reinterpret_cast<std::intptr_t>(parameter));
        break;
    case 15:
        camCtrlSetDistanceFromPlayerDefaults();
        break;
    case 16:
        result = camCtrlGetContextMode();
        break;
    case 17:
        result = camCtrlSetContext(settings[0], settings[1]);
        break;
    case 18:
        camCtrlSetInterpolationMode(3);
        break;
    case 19:
        camCtrlSetInterpolationFrameCount(static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(parameter)));
        break;
    case 20:
        camera->flags |= 0x100U;
        break;
    case 30:
        result = SetConversationCameraFixedPosition(reinterpret_cast<const Vector3 *>(parameter));
        break;
    case 31:
        result = IsConversationCameraShotEstablished();
        break;
    }
    return result;
}

std::int32_t camCtrlSetFlag80000(std::int32_t enabled) {
    CameraRuntime *const camera = g_cameraControl;
    const std::int32_t previous = static_cast<std::int32_t>(camera->flags) >> 19 & 1;
    if (enabled)
        camera->flags |= 0x80000U;
    else
        camera->flags &= ~0x80000U;
    return previous;
}

std::int32_t camCtrlSetFlag80000Wrapper(std::int32_t enabled) {
    return camCtrlSetFlag80000(enabled);
}

// @verified - d
void ResetCameraPresetCache(CameraRuntime &camera) {
    camera.savedPresetIndex = 0;
    camera.presetState = 1;
    camera.presetFlags = 0;
    CameraPreset *preset = g_cameraCachedPresets;
    for (std::int32_t index = 0; index < 3; ++index, ++preset)
        preset->tag = 0;
}

void CameraControlNullsub2() {
}

} // namespace s2
} // namespace shenmue

#endif
