#pragma once

#include "s1/hlib.hpp"
#include "s1/task/char.hpp"
#include "s1/task/telm.hpp"
#include "common/math.hpp"

#include <array>
#include <cstdint>
#include <cstddef>

namespace shenmue::s1 {
struct Character;
struct CharacterNode;
struct ScenarioContext;

extern std::uint8_t g_bHasSetupAutoExposure;
void TranslateCameraRelative(const Vector3 &relativePosition);

// Map camera region whose ground the default camera stays above.
inline constexpr shen_name REGION_CAM3{SHEN_NAME(CAM3)};


enum CameraStateFlags : std::uint32_t {
    CF_DampingOverride = 0x01U,
    CF_LookAroundActive = 0x02U,
    CF_Unknown = 0x04U,
    CF_CameraOverrideActive = 0x08U,
    CF_LookAtDirty = 0x10U,
    CF_KeepRyoAlwaysVisible = 0x20U,
    CF_LookAroundMask = 0x3fU,
};


enum LookAroundTickResult : std::int32_t {
    LOOKAROUND_DISABLED = 0,
    LOOKAROUND_IDLE_OR_FINISHED = 1,
    LOOKAROUND_ACTIVE = 2,
};


struct CameraFollowState {
    Vector3 position{};
    Vector3 headAnchor{};
    Vector3 motionDelta{};
    std::int32_t animationState = 0;
    std::int32_t yaw = 0;
    std::int32_t previousYaw = 0;
    Character *dampingOwner = nullptr;
    CharacterNode *node = nullptr;
};
static_assert(offsetof(CameraFollowState, headAnchor) == 0x0c);
static_assert(offsetof(CameraFollowState, motionDelta) == 0x18);
static_assert(offsetof(CameraFollowState, animationState) == 0x24);
static_assert(offsetof(CameraFollowState, yaw) == 0x28);
static_assert(offsetof(CameraFollowState, dampingOwner) == 0x30);
static_assert(sizeof(CameraFollowState) == 0x40);



struct CameraOrbitState {
    Vector4 orbit{};
    std::int32_t targetYaw = 0;
    std::int32_t targetPitch = 0;
    std::int32_t interpolationEnabled = 0;
    std::int32_t interpolationState = 0;
};
static_assert(offsetof(CameraOrbitState, targetYaw) == 0x10);
static_assert(offsetof(CameraOrbitState, interpolationEnabled) == 0x18);
static_assert(sizeof(CameraOrbitState) == 0x20);


struct CameraOrbitSettings {
    float rotationFactor = 0.0f;
    std::uint32_t flags = 0;
    std::int32_t temporaryYaw = 0;
    std::int32_t temporaryPitch = 0;
    std::int32_t fieldOfView = 10922;
    std::uint32_t executionFlags = 0;
    CameraOrbitState state{};
    float movementFactor = 0.25f;
};
static_assert(offsetof(CameraOrbitSettings, state) == 0x18);
static_assert(sizeof(CameraOrbitSettings) == 0x3c);


struct CameraInterpolatedOrbit : CameraOrbitState {
    Vector3 result{};
};
static_assert(sizeof(CameraInterpolatedOrbit) == 0x2c);


struct CameraPosRot {
    Vector4 pos;
    std::array<Vector3, 2> target;
};

static_assert(sizeof(CameraPosRot) == 0x28);

struct DeveloperCameraState {
    std::uint16_t flags = 0;
    std::uint16_t pressedButtons = 0;
    std::uint8_t analogX = 128;
    std::uint8_t analogY = 128;
    bool setDefaultView = false;
    bool initialized = false;
    std::int32_t mode = 0;
    std::int32_t viewOffsetMode = 0;
    float viewHeightOffset = 0.1f;
    float viewHeightVelocity = 0.0f;
    std::int32_t selectionAccumulator = 0;
    std::int32_t selectionScanFlags = 0;
    std::int32_t selectionBucketIndex = 0;
    CharacterBucketNode *selectionCursor = nullptr;
    Character *selectedCharacter = nullptr;
    std::int32_t pitchStep = 0;
    std::int32_t yawStep = 0;
    std::int32_t zAngle = 0;
    std::int32_t fieldOfView = 10922;
    float dolly = 0.0f;
    float lift = 0.0f;
    float strafe = 0.0f;
    float pan = 0.0f;
    Vector3 origin{};
    Vector3 lookAt{};
    Vector3 viewOrigin{};
    Vector3 viewLookAt{};
};

struct CameraRuntimeState {
    bool viewReady = false;


    DeveloperCameraState developerCamera{};
};





struct AnimatedCameraSequence {
    std::uint32_t *chunk = nullptr;
    std::uint32_t flags = 0;
    Character *eyeAnchorOwner = nullptr;
    Character *targetAnchorOwner = nullptr;
    Vector3 eyeAnchorPosition{};
    Vector3i eyeAnchorRotation{};
    Vector3 targetAnchorPosition{};
    Vector3i targetAnchorRotation{};
    std::array<FcvSplineReader, 3> eyeReaders{};
    std::array<FcvSplineReader, 3> targetReaders{};
    FcvSplineReader zAngleReader{};
    FcvSplineReader fovReader{};
    std::array<std::array<float, 400>, 3> eyeCoefficients{};
    std::array<std::array<float, 400>, 3> targetCoefficients{};
    std::array<float, 400> zAngleCoefficients{};
    std::array<float, 400> fovCoefficients{};
    Vector3 heldEye{};
    Vector3 heldTarget{};
    std::int32_t previousZAngle = 0;
    std::int32_t fov = 0;
};

extern CameraRuntimeState g_cameraRuntime;


extern Matrix4x4 g_tmp_cam_xform;


extern std::int32_t g_fCamAccumYaw;
extern std::int32_t g_nAuthorState;
extern std::int32_t currentPerspectiveFov;


extern Vector3 g_CameraEyePos;
extern Vector3 g_CameraTargetPos;


extern std::uint8_t bLockCamera;
extern CharacterNode g_cameraTargetNode;
extern std::uint8_t g_bIsConversationCamInactive;


extern std::int32_t g_CamState;


extern std::uint32_t *ASEQ_current_chunk_ptr;

extern std::int32_t ZANGLE;


extern std::int32_t g_temp_angle_deg;
extern std::int32_t g_temp_angle_deg2;
extern Vector2 rotationOffsetVector;
extern CameraPosRot tmp_cam_posrot;


extern std::int32_t CurrentFrameCount;
extern std::uint8_t g_ASEQ_busy_flag;


extern std::array<float, 32> smoothed_cam_rots;
extern std::array<float, 32> filtered_cam_rots;


extern float cameraInterpProgress;
extern std::int16_t blend_cameraAngleState1;
extern std::int16_t blend_cameraAngleState2;


extern std::int32_t g_NeedsReinit;
extern std::int32_t g_authorInputYawScale;
extern std::int32_t g_authorInputPitchScale;
extern std::uint8_t g_aseq_do_coli_check;

extern std::int16_t g_tmp_camera_ang_y;

extern std::array<std::int16_t, 4> g_cameraSmoothingAccumulator;

extern std::int32_t g_look_around_angle_x;
extern std::int32_t g_look_around_angle_y;


extern std::uint8_t g_cam1_intersect_status;
extern std::uint8_t g_cam1_flag;
extern float g_CameraBlendProgress;
extern float g_CameraBlendSpeed;
extern std::int32_t g_cam_frm_counter;
extern std::int32_t g_numFramesActive;
extern float g_cameraDeltaTimeRampStep;

extern Character *g_pCEWP;
extern std::array<Character *, 14> g_pASEQ_charas;



extern Vector3 g_tmpCamEyePos;
extern Vector3 g_tmpCamTargetPos;
extern std::int32_t g_DisableCamStateUpdate;
extern std::int32_t g_seqActive;
extern std::int32_t g_prev_temp_dir;

extern std::int32_t initialPerspectiveFov;
extern float initialCameraDistance;

extern std::int32_t isScriptedCameraActive;
extern std::int32_t isPerspectiveTransitionActive;




extern Vector3 g_tmp_interpolated_camrot;
extern std::int32_t g_tmp_cam_rot_z_adj;
extern Vector3 g_script_cam_offs;
extern Vector3 g_script_cam_offs2;
extern std::uint32_t nCamState;
extern float CameraYAngle;


extern Vector3 g_combatCameraEye;
extern Vector3 g_combatCameraTarget;
extern Vector3 g_combatCameraEyeStep;
extern Vector3 g_combatCameraTargetStep;


extern std::int32_t useAnalogInput;
extern std::uint8_t rawAnalogInputValue;
extern std::int32_t computedCameraPitch;



extern CameraFollowState GCameraSettings2;
extern CameraOrbitSettings GSettings;
extern CameraInterpolatedOrbit GCamSettings;


extern Vector3 g_CameraPos;
extern Vector3 g_tmpCameraLookAtV3;
extern Vector3 g_tmp_seqdata_pos;
extern Vector3i g_tmp_seqdata_rot;

extern std::int32_t g_VF_camera_active;


extern std::int32_t g_tmp_interp_count;
extern std::int32_t g_tmp_interp_count_0;


extern Vector3 g_tmp_velocity_vec_0;
extern Vector3 g_tmp_velocity_vec;



extern Vector4 g_tmp_next_cam_pos;
extern Vector4 g_tmp_cam_pos;
extern float g_cam_damping_step_scale;



extern Vector3 targetCameraStepPosition;
extern Vector3 g_tmp_interpolated_cam_pos;
extern Vector3 g_tmp_cam_rot;
extern Vector3 g_tmp_next_cam_rot;


extern CameraInterpolatedOrbit g_cam_target_orbit_state;


extern Vector3 GCameraPos;
extern CameraInterpolatedOrbit g_tmp_cam_fstep_0;



extern std::int32_t g_ctrlLookAtEnabled;
extern std::int32_t g_ctrl_set_lookat;
extern std::int32_t g_ctrlLookAtYaw;
extern std::int32_t g_ctrlLookAtPitch;
extern std::int32_t g_ctrlLookAtAppliedYaw;
extern std::int32_t g_ctrlLookAtAppliedPitch;
extern Vector3 g_ctrlLookAtPoint;
extern std::int32_t g_playerCameraAngles_0;
extern std::int32_t g_playerCameraAngles_1;


extern Vector3 cameraStartPosition;
extern std::int32_t initialHorizontalFov;
extern float horizontalCamDistance;
extern float halfViewHeightWorld;
extern float totalViewHeightWorld;
extern float viewHeightIncrementPerStep;
extern Vector3 cameraDirectionUnitVector;



extern float g_tmp_cam_factor;
extern Vector3 g_followCameraDesiredEye;
extern Vector3 g_CameraLookDirection;
extern CharacterNode g_cameraEyeNode;


extern std::int32_t playerFacingAngle;
extern float g_curr_look_around_factor;
extern float previousDistanceFactor;
extern std::int16_t g_curr_look_around_frms;
extern std::int32_t g_cam_input_base_yaw;
extern std::int32_t g_cam_input_pitch;




extern std::int32_t g_combat_cam_coli_hit_flag;
extern std::int32_t g_combat_cam_range_bias_frms;
extern std::int32_t g_combat_cam_duel_low_angle_frms;
extern Vector3 g_combat_cam_collision_offset_smooth;
extern std::int32_t g_combat_cam_focus_blend_frms;
extern Vector3 g_combat_cam_offset_smooth;
extern std::int32_t g_combat_cam_collision_lift_frms;
extern float g_CameraRetractionLimit;
extern float g_CameraElevationSmoothing;
extern float distanceFromRyoForCamera;
extern float g_CameraZoomFOVModifier;
extern float g_fCurrMaxDistance;
extern std::int32_t currentRelativeZoomTranslationStep;
extern std::int32_t relativeZoomMode;
extern Vector3 g_tmp_zoom_vec;
extern Vector2 screenSpaceToWorldScale;
extern Vector2 prevScreenSpaceToWorldScale;
extern float LOD_Distance_0;
extern float LOD_Distance_2;
extern float NPCDisplayRadius;

extern float g_CurrLODDistance;
extern float g_CurrLODDistance_0;
extern float g_CurrLODDistance_1;
extern float g_CurrLODDisplayRadius;
extern float g_fCurrWorldToGridFactor;
extern std::int16_t g_rawAnalogMagnitudeFixed;
extern float distanceDampingFactor;
extern std::int32_t cameraInterpStepsRemaining;
extern std::int32_t isCameraInterpolationActive;



extern std::int32_t g_previousPerspectiveFov;
extern std::int32_t tmpFov;
extern std::int32_t FieldOfView;
extern std::int32_t g_CameraZoomState;
extern std::int32_t g_CameraFOVState;
extern std::int32_t g_Fov_CameraMode;
extern std::int32_t g_LookAroundViewMode;
extern float g_cam_shake_prev;
extern float g_cam_shake_curr;
extern float g_cam_shake_damp;
extern std::int32_t g_arcade_cam_tight_follow_frms;
extern std::int32_t g_cameraOverrideFlipDirectionActive;
extern std::int32_t g_cameraTargetYaw;
extern std::int32_t g_cameraTargetPitch;
extern std::int32_t g_camera_rot_step_lock;
extern std::int8_t g_CameraInterpolationTimer;
extern std::uint8_t g_bCameraSkipFirstInterp;
extern std::uint8_t g_bNegativeCamBlend;
extern std::uint8_t g_EnableCameraHeightClamp;
extern std::uint8_t g_CameraOverrideActiveFlag;
extern std::uint8_t g_CameraCollisionEnabled;
extern std::int32_t bNoBlendCameraInterp;
extern std::uint8_t g_CameraIsTransitioning;
extern std::int8_t g_CameraFadeInterpFrameCount;
extern float CameraHeightAbovePlayer;
extern float CameraDistanceFromPlayer;
extern float CameraHeightAbovePlayer_MAX;
extern float MaxCameraDistanceOffset;
extern std::uint8_t bInterpretRuntime;
extern Vector3 g_CurrentCameraPosV3;
extern Vector3 currentRelativeZoomCameraTranslation;
extern AnimatedCameraSequence g_animatedCameraSequence;

extern float g_cameraNearClip;

extern std::uint32_t g_cameraStateFlags;
extern std::int32_t g_previousAuthorState;
extern std::int32_t g_cameraAuthorStateSnapshot;
extern std::int32_t g_hazukiResidenceGatedFlag;
extern std::int32_t g_temporaryGameState;

HLib::Task *CAME_Init();
void CAME_Callback(HLib::TaskData *taskData);
void CAME_HandleRootAuthorTransition(bool vehicleInputActive);
void shenResetCombatCameraForTransition();
void CAME_ClearSequenceActive();
bool ctrlGetNeedsCameraDamping();
std::int32_t ReturnCurrentFixed16Angle();
bool CAME_GetView(Vector3 *eye, Vector3 *target, float *verticalFovRadians);
float CAME_SetLodDistance0(float value);
float CAME_GetLodDistance0();
float CAME_SetLodDistance2(float value);
float CAME_GetLodDistance2();
float CAME_SetNpcDisplayRadius(float value);
float CAME_GetNpcDisplayRadius();
float CAME_GetCurrentLodDistance0();
void CAME_ExecuteLodSnapshotCommand(std::int32_t command);
void CAME_SetScriptedView(const Vector3 &eye, const Vector3 &target, std::int32_t perspectiveFov, std::int16_t zAngle);
void CAME_SetCameraFlags(std::int32_t value);
void CAME_SetFieldOfView(std::int32_t fieldOfView);
void CAME_SetShake(float previous, float current, float damping);
bool CAME_BuildScriptAuthorSequence(const std::uint32_t *params, const std::byte *taskData);
void CAME_StartBuiltScriptAuthorSequence(Character *eyeOwner, Character *targetOwner);
void StartAnimatedCamera();
void AllocSPLI_ForCamera(std::int32_t frame);
std::uint32_t EventCam_Spline_ReadFCVKey(std::int32_t sequenceIndex, Character *eyeOwner, Character *targetOwner, Character *character);
void LogNpcVisibility();
void ApplyPendingGameState(std::int32_t overrideState);
void SetNewAuthorGameState(std::int32_t state);
void CAME_SetEventCameraInput(void *chunk, Character *target, Character *eye);
void CAME_SetSequencePlayRoot(const Vector3 &position, const Vector3i &rotation);
std::int32_t CAME_GetCurrentFrameCount();
std::int32_t CAME_GetInterpolationActive();
std::int32_t CAME_GetAnimatedSequenceBusy();
bool CAME_SetCollisionMode(std::uint32_t mode);
void CAME_SetRotationStepLock(std::uint32_t value);
void CAME_SetTemporaryFactor(float value);
float CAME_GetTemporaryAngleDegrees(bool secondary);
float CAME_SetCameraYAngle(float value);
std::uint32_t CAME_GetScriptedState();
void CAME_SetZoomState(std::int32_t state);
bool CAME_SetAnimatedSequenceCollisionCheck(bool enabled);
void Script_shenSetCameraPosAndTarget(ScenarioContext *context, void *arguments);
void Script_ScreenToWorldAssign(ScenarioContext *context, void *arguments);
void Script_CamCtrl(ScenarioContext *context, void *arguments);

} // namespace shenmue::s1
