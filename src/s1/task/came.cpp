#include "s1/task/came.hpp"
#include "s1/task/conv.hpp"
#include "s1/task/char.hpp"
#include "s1/simple_shader.hpp"

#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/motion.hpp"
#include "s1/input.hpp"

#include "s1/d3t/renderer.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/gull.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/mdlm.hpp"

#include "s1/task/scen.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/kdan.hpp"
#include "s1/task/vfcl.hpp"

#include "s1/task/obje.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/boot.hpp"
#include "s1/timing.hpp"
#include "s1/d3t/settings.hpp"

#include <bit>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <cstdio>

#include <limits>
#include <string_view>

namespace shenmue::s1 {

extern std::uint8_t g_globalFlags[1];
CameraRuntimeState g_cameraRuntime{};
Matrix4x4 g_tmp_cam_xform{};
std::int32_t g_fCamAccumYaw;
std::int32_t g_nAuthorState;
std::int32_t currentPerspectiveFov;
Vector3 g_CameraEyePos{};
Vector3 g_CameraTargetPos{};
std::uint8_t bLockCamera;
CharacterNode g_cameraTargetNode{};
std::uint8_t g_bIsConversationCamInactive;
std::int32_t g_CamState;
std::uint32_t *ASEQ_current_chunk_ptr = nullptr;
std::int32_t ZANGLE;
std::int32_t g_temp_angle_deg;
std::int32_t g_temp_angle_deg2;
Vector2 rotationOffsetVector{};
CameraPosRot tmp_cam_posrot{};
std::int32_t CurrentFrameCount;
std::uint8_t g_ASEQ_busy_flag;
std::array<float, 32> smoothed_cam_rots{};
std::array<float, 32> filtered_cam_rots{};
float cameraInterpProgress;
std::int16_t blend_cameraAngleState1;
std::int16_t blend_cameraAngleState2;
std::int32_t g_NeedsReinit;
std::int32_t g_authorInputYawScale;
std::int32_t g_authorInputPitchScale;
std::uint8_t g_aseq_do_coli_check;
std::int16_t g_tmp_camera_ang_y;
std::array<std::int16_t, 4> g_cameraSmoothingAccumulator{};
std::int32_t g_look_around_angle_x;
std::int32_t g_look_around_angle_y;
std::uint8_t g_cam1_intersect_status;
std::uint8_t g_cam1_flag;
float g_CameraBlendProgress;
float g_CameraBlendSpeed;
std::int32_t g_cam_frm_counter;
std::int32_t g_numFramesActive;
float g_cameraDeltaTimeRampStep;
Character *g_pCEWP = nullptr;
std::array<Character *, 14> g_pASEQ_charas{};
Vector3 g_tmpCamEyePos{};
Vector3 g_tmpCamTargetPos{};
std::int32_t g_DisableCamStateUpdate = 0;
std::int32_t g_seqActive = 0;
std::int32_t g_prev_temp_dir = 0;
std::int32_t initialPerspectiveFov = 0;
float initialCameraDistance = 0.0f;
std::int32_t isScriptedCameraActive = 0;
std::int32_t isPerspectiveTransitionActive = 0;
Vector3 g_tmp_interpolated_camrot{};
std::int32_t g_tmp_cam_rot_z_adj = 0;
Vector3 g_script_cam_offs{};
Vector3 g_script_cam_offs2{};
std::uint32_t nCamState = 0;
float CameraYAngle = 1.2f;
Vector3 g_combatCameraEye{};
Vector3 g_combatCameraTarget{};
Vector3 g_combatCameraEyeStep{};
Vector3 g_combatCameraTargetStep{};
std::int32_t useAnalogInput = 0;
std::uint8_t rawAnalogInputValue = 0;
std::int32_t computedCameraPitch = 0;
CameraFollowState GCameraSettings2{};
CameraOrbitSettings GSettings{.fieldOfView = 0, .movementFactor = 0.0f};
CameraInterpolatedOrbit GCamSettings{};
Vector3 g_CameraPos{};
Vector3 g_tmpCameraLookAtV3{};
Vector3 g_tmp_seqdata_pos{};
Vector3i g_tmp_seqdata_rot{};
std::int32_t g_VF_camera_active = 0;
std::int32_t g_tmp_interp_count = 0;
std::int32_t g_tmp_interp_count_0 = 0;
Vector3 g_tmp_velocity_vec_0{};
Vector3 g_tmp_velocity_vec{};
Vector4 g_tmp_next_cam_pos{};
Vector4 g_tmp_cam_pos{};
float g_cam_damping_step_scale = 0.0f;
Vector3 targetCameraStepPosition{};
Vector3 g_tmp_interpolated_cam_pos{};
Vector3 g_tmp_cam_rot{};
Vector3 g_tmp_next_cam_rot{};
CameraInterpolatedOrbit g_cam_target_orbit_state{};
Vector3 GCameraPos{};
CameraInterpolatedOrbit g_tmp_cam_fstep_0{};
std::int32_t g_ctrlLookAtEnabled = 0;
std::int32_t g_ctrl_set_lookat = 0;
std::int32_t g_ctrlLookAtYaw = 0;
std::int32_t g_ctrlLookAtPitch = 0;
std::int32_t g_ctrlLookAtAppliedYaw = 0;
std::int32_t g_ctrlLookAtAppliedPitch = 0;
Vector3 g_ctrlLookAtPoint{};
std::int32_t g_playerCameraAngles_0 = 0;
std::int32_t g_playerCameraAngles_1 = 0;
Vector3 cameraStartPosition{};
std::int32_t initialHorizontalFov = 0;
float horizontalCamDistance = 0.0f;
float halfViewHeightWorld = 0.0f;
float totalViewHeightWorld = 0.0f;
float viewHeightIncrementPerStep = 0.0f;
Vector3 cameraDirectionUnitVector{};
float g_tmp_cam_factor = 0.0f;
Vector3 g_followCameraDesiredEye{};
Vector3 g_CameraLookDirection{};
CharacterNode g_cameraEyeNode{};
std::int32_t playerFacingAngle = 0;
float g_curr_look_around_factor = 0.0f;
float previousDistanceFactor = 0.0f;
std::int16_t g_curr_look_around_frms = 0;
std::int32_t g_cam_input_base_yaw = 0;
std::int32_t g_cam_input_pitch = 0;
std::int32_t g_combat_cam_coli_hit_flag = 0;
std::int32_t g_combat_cam_range_bias_frms = 0;
std::int32_t g_combat_cam_duel_low_angle_frms = 0;
Vector3 g_combat_cam_collision_offset_smooth{};
std::int32_t g_combat_cam_focus_blend_frms = 0;
Vector3 g_combat_cam_offset_smooth{};
std::int32_t g_combat_cam_collision_lift_frms = 0;
float g_CameraRetractionLimit = 0.0f;
float g_CameraElevationSmoothing = 0.0f;
float distanceFromRyoForCamera = 0.0f;
float g_CameraZoomFOVModifier = 0.0f;
float g_fCurrMaxDistance = 1000.0f;
std::int32_t currentRelativeZoomTranslationStep = 0;
std::int32_t relativeZoomMode = 0;
Vector3 g_tmp_zoom_vec{};
Vector2 screenSpaceToWorldScale{};
Vector2 prevScreenSpaceToWorldScale{};
float LOD_Distance_0 = 0.0f;
float LOD_Distance_2 = 0.0f;
float NPCDisplayRadius = 0.0f;
float g_CurrLODDistance = 0.0f;
float g_CurrLODDistance_0 = 0.0f;
float g_CurrLODDistance_1 = 0.0f;
float g_CurrLODDisplayRadius = 0.0f;
float g_fCurrWorldToGridFactor = 0.0f;
std::int16_t g_rawAnalogMagnitudeFixed = 0;
float distanceDampingFactor = 0.0f;
std::int32_t cameraInterpStepsRemaining = 0;
std::int32_t isCameraInterpolationActive = 0;
std::int32_t g_previousPerspectiveFov = 0;
std::int32_t tmpFov = 0;
std::int32_t FieldOfView = 0;
std::int32_t g_CameraZoomState = 0;
std::int32_t g_CameraFOVState = -1;
std::int32_t g_Fov_CameraMode = 0;
std::int32_t g_LookAroundViewMode = 0;
float g_cam_shake_prev = 0.0f;
float g_cam_shake_curr = 0.0f;
float g_cam_shake_damp = 0.0f;
std::int32_t g_arcade_cam_tight_follow_frms = 0;
std::int32_t g_cameraOverrideFlipDirectionActive = 0;
std::int32_t g_cameraTargetYaw = 0;
std::int32_t g_cameraTargetPitch = 0;
std::int32_t g_camera_rot_step_lock = 0;
std::int8_t g_CameraInterpolationTimer = 0;
std::uint8_t g_bCameraSkipFirstInterp = 0;
std::uint8_t g_bNegativeCamBlend = 0;
std::uint8_t g_EnableCameraHeightClamp = 0;
std::uint8_t g_CameraOverrideActiveFlag = 0;
std::uint8_t g_CameraCollisionEnabled = 0;
std::int32_t bNoBlendCameraInterp = 0;
std::uint8_t g_CameraIsTransitioning = 0;
std::int8_t g_CameraFadeInterpFrameCount = 0;
float CameraHeightAbovePlayer = 1.3f;
float CameraDistanceFromPlayer = 3.0999999f;
float CameraHeightAbovePlayer_MAX = 1.3f;
float MaxCameraDistanceOffset = 0.0f;
std::uint8_t bInterpretRuntime = 0;
Vector3 g_CurrentCameraPosV3{};
Vector3 currentRelativeZoomCameraTranslation{};
float g_cameraNearClip = 0.03f;
std::uint32_t g_cameraStateFlags = 0;
std::int32_t g_previousAuthorState = 0;
std::int32_t g_cameraAuthorStateSnapshot = 0;
std::int32_t g_hazukiResidenceGatedFlag = 1;
std::int32_t g_temporaryGameState = 0;
std::uint8_t g_bHasSetupAutoExposure = 0;
AnimatedCameraSequence g_animatedCameraSequence{};
extern float g_screenWidthOffset;
extern float g_screenHeightOffset;

constexpr float kAngleToRadians = 6.2831853071795864769f / 65536.0f;

std::int64_t ForEachCewpEntityApplyCallback(std::int64_t parameter, void (*callback)(void *));
std::uint32_t GetDebugLogFlag();

bool CAME_SetCollisionMode(std::uint32_t mode) {
    const bool previous = g_CameraCollisionEnabled;
    if (mode <= 2)
        g_CameraCollisionEnabled = static_cast<std::uint8_t>(mode);
    return previous;
}

void CAME_ClearSequenceActive() {
    g_seqActive = 0;
}

void CAME_SetRotationStepLock(std::uint32_t rawValue) {
    std::int32_t value = static_cast<std::int32_t>(rawValue);
    if (rawValue > 10)
        value = value < 0 ? 0 : 10;
    g_camera_rot_step_lock = value;
}

void CAME_SetTemporaryFactor(float value) {
    g_tmp_cam_factor = value;
}

float CAME_GetTemporaryAngleDegrees(bool secondary) {
    const std::int32_t angle = secondary ? g_temp_angle_deg2 : g_temp_angle_deg;
    return static_cast<float>(angle) / 182.04445f;
}

float CAME_SetCameraYAngle(float value) {
    const float previous = CameraYAngle;
    if (!(value < 0.0f))
        CameraYAngle = value;
    return previous;
}

std::uint32_t CAME_GetScriptedState() {
    if (!isCameraInterpolationActive && !isScriptedCameraActive && !isPerspectiveTransitionActive)
        return 0;
    if (isPerspectiveTransitionActive && cameraInterpStepsRemaining <= 0)
        return 3;
    return isScriptedCameraActive || isPerspectiveTransitionActive ? 2U : 1U;
}
void CAME_SetZoomState(std::int32_t state) {
    const auto *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    ImageModuleWork *const image = IMGM_GetWork(ryo);
    if (!image || !image->images || image->requestedImage >= image->imageCount)
        return;
    MdlmImageSlot *const imageSlot = image->images[image->requestedImage];
    if (!imageSlot || !imageSlot->activeModel)
        return;

    MotionManagerWork *const motion = MOTM_GetWork(ryo);
    auto *node = motion ? static_cast<MotionNodeRuntime *>(motion->motion.previousNodes) : nullptr;
    while (node && node->nodeId != 5 && node->nodeId != 0xff)
        ++node;
    if (!node || node->nodeId != 5 || !node->transform)
        return;

    g_CameraZoomState = state;
    if (state) {
        g_previousPerspectiveFov = currentPerspectiveFov;
        std::int32_t fov = 3640;
        if (g_Fov_CameraMode == 11)
            fov = 4551;
        else if (g_Fov_CameraMode == 9)
            fov = 4187;
        tmpFov = fov;
        CAME_SetFieldOfView(fov);
    } else {
        CAME_SetFieldOfView(g_previousPerspectiveFov);
    }
}

bool CAME_SetAnimatedSequenceCollisionCheck(bool enabled) {
    const bool previous = g_aseq_do_coli_check;
    g_aseq_do_coli_check = enabled;
    return previous;
}

namespace {
void UpdateThirdPersonCamera();
void UpdateScriptedCamera();

std::int16_t CharacterWorldYaw(const Character &character);
bool finite(const Vector3 &value);

bool IsProtectedAuthorState(std::int32_t state) {
    return state >= 0 && state <= 0x13 && ((0x8dffcU >> static_cast<std::uint32_t>(state)) & 1U) != 0;
}

Character *CurrentRyo() {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

void CAME_CewpToggleVisibility(void *payload) {
    auto *const character = static_cast<Character *>(payload);
    if (character)
        character->flags.value |= CharacterFlagVisible;
}

void CAME_CewpHide(void *payload) {
    auto *const character = static_cast<Character *>(payload);
    if (character)
        character->flags.value &= ~CharacterFlagVisible;
}

void au_re_CEWP_Execute() {
    g_DisableCamStateUpdate = 0;
    ForEachCewpEntityApplyCallback(1, CAME_CewpToggleVisibility);

    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control && control->scenarioTask)
        SetMainCallbackToInitForCleanup(control->scenarioTask);
}

std::uint32_t UppercaseAreaToken(std::uint32_t token) {
    std::uint32_t result = 0;
    for (std::uint32_t byteIndex = 0; byteIndex != 4; ++byteIndex) {
        std::uint32_t value = (token >> (byteIndex * 8U)) & 0xffU;
        if (value >= 'a' && value <= 'z')
            value -= 'a' - 'A';
        result |= value << (byteIndex * 8U);
    }
    return result;
}

void SetScene80CameraMode() {
    g_Fov_CameraMode = 0;
    g_LookAroundViewMode = 0;
    if (g_sceneId != 80)
        return;

    switch (UppercaseAreaToken(g_areaId)) {
    case SHEN_NAME(SYPF):
        g_Fov_CameraMode = 1;
        break;
    case SHEN_NAME(CHAF):
        g_Fov_CameraMode = 2;
        break;
    case SHEN_NAME(KOTF):
        g_Fov_CameraMode = 3;
        break;
    case SHEN_NAME(GICF):
        g_Fov_CameraMode = 4;
        break;
    case SHEN_NAME(HGRF):
        g_Fov_CameraMode = 5;
        break;
    case SHEN_NAME(HGYF):
        g_Fov_CameraMode = 6;
        break;
    case SHEN_NAME(HGMF):
        g_Fov_CameraMode = 7;
        break;
    case SHEN_NAME(HGSF):
        g_Fov_CameraMode = 8;
        break;
    case SHEN_NAME(HGCF):
        g_Fov_CameraMode = 9;
        break;
    case SHEN_NAME(HGOF):
        g_Fov_CameraMode = 10;
        break;
    case SHEN_NAME(HGNF):
        g_Fov_CameraMode = 11;
        break;
    default:
        return;
    }
    g_LookAroundViewMode = 1;
}

const MapControlData *CurrentMapControl() {
    return g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}


bool HasActiveQueueNineTask() {
    const HLib::Task *const root = &g_TaskInfo.taskQueue.rootTask;
    const HLib::Task *task = root;
    while (task->r8b != 9) {
        task = task->nextInQueue;
        if (!task || task == root)
            return false;
    }
    return task->task_data_ptr != nullptr;
}

std::uint8_t GetLookingAroundFactor() {
    const MapControlData *const control = CurrentMapControl();
    const auto *const pad = control ? static_cast<const PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
    if (!pad || !control->ryo)
        return 0;
    const bool lowTrigger = g_applicationSettings.savedInput.swapTriggers != 0 && g_keyboardActive != 1;
    const std::uint8_t pressure = static_cast<std::uint8_t>(lowTrigger ? pad->triggers : pad->triggers >> 8U);
    return pressure >= 10U ? static_cast<std::uint8_t>(pressure - 10U) : 0;
}

void UpdateLookAroundAnalogInput() {
    const MapControlData *const control = CurrentMapControl();
    const PadCaptureEntry *pad = nullptr;
    if (!HasActiveQueueNineTask()) {
        pad = control ? static_cast<const PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
    } else {
        Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
        const VFFightStage *const stage = g_pFightStage;
        if (!ryo || !stage)
            return;
        std::uint8_t index = 0;
        while (index < stage->maxEnemies && stage->enemySlots[index].character != ryo)
            ++index;
        if (index >= stage->maxEnemies || !stage->enemySlots[index].fighter)
            return;
        pad = static_cast<const PadCaptureEntry *>(stage->enemySlots[index].fighter->game->padCaptureEntry);
    }
    if (!pad)
        return;

    std::int8_t angleX = 0;
    std::int8_t angleY = 0;
    if (ShouldRun()) {
        angleX = g_pControlPadController->rightAngleX;
        angleY = g_pControlPadController->rightAngleY;
    } else {
        angleX = static_cast<std::int8_t>(static_cast<std::uint8_t>(pad->leftStick) + 0x80U);
        angleY = static_cast<std::int8_t>(static_cast<std::uint8_t>(pad->leftStick >> 8U) + 0x80U);
    }

    std::int32_t yaw = 291 * angleX;
    if (static_cast<std::uint32_t>(yaw + 0x8000) > 0xffffU)
        yaw = yaw < -32768 ? -32768 : 32767;
    g_look_around_angle_x = yaw;
    std::int32_t pitch = 291 * angleY;
    if (static_cast<std::uint32_t>(pitch + 0x8000) > 0xffffU)
        pitch = pitch < -32768 ? -32768 : 32767;
    g_look_around_angle_y = pitch;

    const std::int32_t yawDelta = yaw - g_playerCameraAngles_0;
    const std::int32_t pitchDelta = pitch - g_playerCameraAngles_1;
    std::int32_t yawStep = (yawDelta >= 0 ? yawDelta + 15 : yawDelta - 15) / 16;
    if (static_cast<std::uint32_t>(yawStep + 2560) > 0x1400U)
        yawStep = yawStep < -2560 ? -2560 : 2560;
    g_playerCameraAngles_0 += yawStep;
    std::int32_t pitchStep = (pitchDelta >= 0 ? pitchDelta + 15 : pitchDelta - 15) / 16;
    if (static_cast<std::uint32_t>(pitchStep + 2560) > 0x1400U)
        pitchStep = pitchStep < -2560 ? -2560 : 2560;
    g_playerCameraAngles_1 += pitchStep;

    std::int32_t pressure = GetLookingAroundFactor();
    if (!useAnalogInput)
        pressure = 0;
    const std::int32_t delta = pressure - rawAnalogInputValue;
    std::int32_t step = delta / 16;
    if (std::abs(delta) < 16 && delta != 0)
        step = delta > 0 ? 1 : -1;
    const std::uint8_t value = static_cast<std::uint8_t>(rawAnalogInputValue + step);
    rawAnalogInputValue = value > 245U ? 245U : value;
}

void shenTransitionToCam3() {
    g_combatCameraEye = g_CameraEyePos;
    g_combatCameraTarget = g_CameraTargetPos;
    g_CurrentCameraPosV3 = g_CameraEyePos;
    g_bIsConversationCamInactive = 1;
    g_combat_cam_focus_blend_frms = 0;
    g_combat_cam_duel_low_angle_frms = 0;
    g_combat_cam_range_bias_frms = 0;
    g_combat_cam_collision_lift_frms = 0;
    g_combat_cam_coli_hit_flag = 0;
    g_combat_cam_offset_smooth = {};
    g_combat_cam_collision_offset_smooth = {};
    g_combatCameraEyeStep = {};
    g_combatCameraTargetStep = {};
    g_CameraOverrideActiveFlag = 0;
    CAME_SetFieldOfView(FieldOfView);
    if (Character *const ryo = CurrentRyo())
        ryo->renderOpacity = 1.0f;
}

} // namespace

void shenResetCombatCameraForTransition() {
    g_temporaryGameState = 0;
    ApplyPendingGameState(0);
    if (g_areaId == SHEN_NAME(GMCT)) {
        g_CameraPos = g_CameraEyePos;
        g_tmpCameraLookAtV3 = g_CameraTargetPos;
        g_CurrentCameraPosV3 = g_CameraEyePos;
        g_bIsConversationCamInactive = 1;
        g_CameraOverrideActiveFlag = 0;
        CAME_SetFieldOfView(FieldOfView);
    } else {
        shenTransitionToCam3();
    }
}

namespace {


std::uint32_t *FindSequenceByChunk(std::int32_t sequenceIndex) {
    std::uint32_t payloadSize = 0;
    const std::byte *entry = MAPINFO_FindChunkPayload(SHEN_NAME(ECAM), &payloadSize);
    if (!entry)
        return nullptr;
    char name[12];
    std::snprintf(name, sizeof(name), "%04d", sequenceIndex);
    std::uint32_t token = 0;
    std::memcpy(&token, name, sizeof(token));
    const std::byte *const end = entry + payloadSize;
    while (end - entry >= 8) {
        std::uint32_t entryName = 0;
        std::uint32_t entrySize = 0;
        std::memcpy(&entryName, entry, sizeof(entryName));
        std::memcpy(&entrySize, entry + 4, sizeof(entrySize));
        if (entryName == token)
            return reinterpret_cast<std::uint32_t *>(const_cast<std::byte *>(entry + 8));
        if (!entrySize)
            break;
        entry += entrySize;
    }
    return nullptr;
}


void Camera_UpdateCameraXYZ(float x, float y, float z) {
    if (bLockCamera)
        return;
    g_CameraEyePos.x += x;
    g_CameraEyePos.y += y;
    g_CameraEyePos.z += z;
    shenApplyTranslationToAll(g_cameraEyeNode, g_CameraEyePos);
}




void UpdateCameraPosAndOrientation() {
    constexpr std::uint16_t kBackAwayButton = 0x40;
    constexpr std::uint16_t kPushInButton = 0x80;
    constexpr std::uint16_t kUpButton = 0x1000;
    constexpr std::uint16_t kDownButton = 0x2000;
    constexpr std::uint16_t kLeftButton = 0x4000;
    constexpr std::uint16_t kRightButton = 0x8000;
    const auto distance = [](const Vector3 &eye, const Vector3 &target) {
        const float dx = eye.x - target.x;
        const float dy = eye.y - target.y;
        const float dz = eye.z - target.z;
        float value = std::sqrt(dy * dy + dx * dx + dz * dz);
        if (0.0f > value)
            value = -value;
        return value;
    };
    const auto horizontalLength = [](float dx, float dz) {
        const float squared = dz * dz + dx * dx;
        return 0.0f > squared ? -std::sqrt(-squared) : std::sqrt(squared);
    };

    std::uint16_t buttons = g_inputState.slots[0].held;
    std::int32_t mode = g_Fov_CameraMode;
    Vector3 eye = g_CameraEyePos;
    g_temp_angle_deg = 0;
    g_temp_angle_deg2 = 0;
    if ((buttons & kBackAwayButton) != 0) {
        float minimum = 1.1f;
        if (mode == 2 || mode == 3 || mode == 6 || mode == 9 || mode == 10)
            minimum = 1.3f;
        else if (mode == 4 || mode == 7)
            minimum = 1.4f;
        else if (mode == 11)
            minimum = 0.8f;
        const Vector3 target = g_CameraTargetPos;
        if (minimum > distance(eye, target)) {
            const float rate = mode == 11 ? 0.005f : 0.01f;
            Camera_UpdateCameraXYZ(rate * (eye.x - target.x), rate * (eye.y - target.y), rate * (eye.z - target.z));
            buttons = g_inputState.slots[0].held;
            eye = g_CameraEyePos;
        }
    }
    Vector3 target = g_CameraTargetPos;
    if ((buttons & kPushInButton) != 0) {
        float maximum = 0.3f;
        if (mode == 3 || mode == 10)
            maximum = 0.4f;
        else if (mode == 4)
            maximum = 0.36f;
        else if (mode == 2 || mode == 9)
            maximum = 0.53f;
        if (distance(eye, target) > maximum) {
            const float rate = mode == 11 ? 0.005f : 0.01f;
            Camera_UpdateCameraXYZ(-(rate * (eye.x - target.x)), -(rate * (eye.y - target.y)), -(rate * (eye.z - target.z)));
            buttons = g_inputState.slots[0].held;
            target = g_CameraTargetPos;
            eye = g_CameraEyePos;
        }
    }

    std::int32_t orbit = relativeZoomMode;
    if ((buttons & kLeftButton) != 0)
        orbit = relativeZoomMode = -1;
    else if ((buttons & kRightButton) != 0)
        orbit = relativeZoomMode = 1;
    if (orbit) {
        const double turn = static_cast<double>(distance(eye, target)) / 3.0 * 8192.0 / 45.0;
        const std::int32_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(turn)) * orbit;
        g_temp_angle_deg = angle;
        const std::uint16_t wrapped = static_cast<std::uint16_t>(angle);
        const std::int32_t folded = wrapped <= 0x8000U ? wrapped : 0x10000 - wrapped;
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        const float dx = eye.x - target.x;
        const float dz = eye.z - target.z;
        TranslateCameraRelative({cosine * dx - sine * dz + target.x, eye.y, sine * dx + cosine * dz + target.z});
        buttons = g_inputState.slots[0].held;
        mode = g_Fov_CameraMode;
        target = g_CameraTargetPos;
        eye = g_CameraEyePos;
    }

    // Pitch about the target by the signed step; the move is kept only while the result stays inside the mode's pitch limit.
    const auto pitchedEye = [&](std::int32_t step, float horizontal) {
        const std::uint16_t wrapped = static_cast<std::uint16_t>(step);
        const std::int32_t folded = wrapped <= 0x8000U ? wrapped : 0x10000 - wrapped;
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        const float sine = sin_fixed16(static_cast<std::int16_t>(step));
        const float dy = eye.y - target.y;
        const float scale = (cosine * horizontal - sine * dy) / horizontal;
        return Vector3{scale * (eye.x - target.x) + target.x, sine * horizontal + target.y + cosine * dy, scale * (eye.z - target.z) + target.z};
    };
    if ((buttons & kUpButton) != 0) {
        const float pitch = distance(eye, target) / g_runStartSpeed;
        const std::int32_t step = static_cast<std::uint16_t>(static_cast<std::int32_t>(pitch * 8192.0f / 45.0f));
        const Vector3 position = pitchedEye(step, horizontalLength(eye.x - target.x, eye.z - target.z));
        std::int32_t limit = 0x2aaa;
        if (mode == 2 || mode == 3 || mode == 9)
            limit = 9102;
        else if (mode == 4)
            limit = 10012;
        else if (mode == 1)
            limit = 0x2000;
        const float rise = position.y - target.y;
        if (!(rise > 0.0f) || Shenmue_Atan2Fixed(std::fabs(rise), pitch) < limit) {
            g_temp_angle_deg2 = step;
            TranslateCameraRelative(position);
            eye = g_CameraEyePos;
            target = g_CameraTargetPos;
            mode = g_Fov_CameraMode;
            buttons = g_inputState.slots[0].held;
        }
    }
    if ((buttons & kDownButton) != 0) {
        const float pitch = distance(eye, target) / g_runStartSpeed;
        const std::int32_t step = -static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::int32_t>(pitch * 8192.0f / 45.0f)));
        const Vector3 position = pitchedEye(step, horizontalLength(eye.x - target.x, eye.z - target.z));
        std::int32_t limit = 10012;
        if (mode == 3)
            limit = 9102;
        else if (mode == 2 || mode == 9)
            limit = 7281;
        else if (mode == 4)
            limit = 10376;
        const float rise = position.y - target.y;
        if (!(0.0f > rise) || Shenmue_Atan2Fixed(std::fabs(rise), pitch) < limit) {
            g_temp_angle_deg2 = step;
            TranslateCameraRelative(position);
        }
    }
}

void HandleCameraZoomStates() {
    CAME_SetFieldOfView(tmpFov);
    if (!bLockCamera)
        ZANGLE = 0;

    Matrix4x4_Push();
    const Matrix4x4 *anchorTransform = g_matrixStack.top.stack;
    Character *const ryo = CurrentRyo();
    if (MotionManagerWork *const manager = MOTM_GetWork(ryo)) {
        const MotionNodeRuntime *row = static_cast<const MotionNodeRuntime *>(manager->motion.previousNodes);
        while (row && row->nodeId != 0xffU) {
            if (row->nodeId == 5U) {
                if (row->transform)
                    anchorTransform = row->transform;
                break;
            }
            ++row;
        }
    }
    workTransformMatrix = *anchorTransform;
    Vector3 anchor{
        anchorTransform->rows[3].x,
        anchorTransform->rows[3].y,
        anchorTransform->rows[3].z,
    };
    Matrix4x4::Pop(1);

    switch (g_Fov_CameraMode) {
    case 1:
    case 6:
    case 8:
        anchor.y += 0.12f;
        break;
    case 2:
        anchor.y += 0.13f;
        break;
    case 3:
    case 10:
        anchor.y += 0.06f;
        break;
    case 4:
    case 7:
        anchor.y += 0.11f;
        break;
    case 5:
    case 9:
    case 11:
        anchor.y += 0.1f;
        break;
    default:
        break;
    }
    if (!bLockCamera) {
        g_CameraTargetPos = anchor;
        shenApplyTranslationToAll(g_cameraTargetNode, anchor);
    }

    switch (g_CameraZoomState) {
    case 1: {
        float outerDistance = 0.9f;
        float innerDistance = 0.8f;
        if (g_Fov_CameraMode == 1) {
            outerDistance = 0.7f;
            innerDistance = 0.6f;
        } else if (g_Fov_CameraMode == 11) {
            outerDistance = 0.6f;
            innerDistance = 0.5f;
        }
        const std::int32_t yaw = ryo ? ryo->entity.node.rotation.y : 0;
        const auto inverseYaw = static_cast<std::int16_t>(-yaw);
        const float sine = sin_fixed16(inverseYaw);
        std::int16_t folded = static_cast<std::int16_t>(-inverseYaw);
        if (static_cast<std::uint16_t>(inverseYaw) <= 0x8000U)
            folded = inverseYaw;
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        currentRelativeZoomCameraTranslation = {
            anchor.x + sine * outerDistance,
            anchor.y,
            anchor.z - cosine * outerDistance,
        };
        const Vector3 inner{
            anchor.x + sine * innerDistance,
            anchor.y,
            anchor.z - cosine * innerDistance,
        };
        g_tmp_zoom_vec = {
            (currentRelativeZoomCameraTranslation.x - inner.x) / 20.0f,
            anchor.y,
            (currentRelativeZoomCameraTranslation.z - inner.z) / 20.0f,
        };
        TranslateCameraRelative(inner);
        if (!bLockCamera) {
            g_CameraTargetPos = anchor;
            shenApplyTranslationToAll(g_cameraTargetNode, anchor);
        }
        currentRelativeZoomTranslationStep = 0;
        relativeZoomMode = 0;
        ++g_CameraZoomState;
        break;
    }
    case 2:
        TranslateCameraRelative(g_CurrentCameraPosV3);
        if (currentRelativeZoomTranslationStep < 20) {
            TranslateCameraRelative({
                g_CameraEyePos.x + g_tmp_zoom_vec.x,
                g_CameraEyePos.y,
                g_CameraEyePos.z + g_tmp_zoom_vec.z,
            });
            ++currentRelativeZoomTranslationStep;
        } else {
            currentRelativeZoomTranslationStep = 0;
            TranslateCameraRelative(currentRelativeZoomCameraTranslation);
            ++g_CameraZoomState;
        }
        break;
    case 3:
        TranslateCameraRelative(g_CurrentCameraPosV3);
        UpdateCameraPosAndOrientation();
        break;
    default:
        break;
    }
    g_CurrentCameraPosV3 = g_CameraEyePos;
    g_cameraRuntime.viewReady = true;
}

void ryoApplyCameraSmoothingAndCollisionTrans();

void HandleScriptedCamera() {
    UpdateScriptedCamera();
}

std::int32_t Angle16FromRadians(float radians) {
    return static_cast<std::int32_t>(radians * 10430.37835047046f);
}

MotionNodeRuntime *FindActiveMotionNode(Character *character, std::uint8_t nodeId) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    auto *const rows = manager ? static_cast<MotionNodeRuntime *>(manager->motion.activeNodes) : nullptr;
    for (std::size_t index = 0; rows && index != 128; ++index) {
        if (rows[index].nodeId == 0xffU)
            break;
        if (rows[index].nodeId == nodeId)
            return &rows[index];
    }
    return nullptr;
}

} // namespace

std::int32_t ReturnCurrentFixed16Angle() {
    const MapControlData *const control = CurrentMapControl();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!ryo)
        return 0;
    const std::uint32_t state = static_cast<std::uint32_t>(g_ryoCurrentAnimationState);
    if (ShouldRun() && state <= 11U && ((0xc12U >> state) & 1U) != 0)
        return static_cast<std::int16_t>(charaGetWorldYaw(ryo));
    return CharacterWorldYaw(*ryo);
}

namespace {

bool UpdateStates(CameraFollowState &state) {
    Character *const ryo = CurrentRyo();
    if (!ryo) {
        state.dampingOwner = nullptr;
        return false;
    }
    const Vector3 position = GetPositionForEntity(ryo->entity.node);
    if (state.dampingOwner)
        state.motionDelta = {position.x - state.position.x, position.y - state.position.y, position.z - state.position.z};
    else
        state.motionDelta = {};
    mtwkGetCharaBonePosition(ryo, 0x2f, &state.headAnchor);
    state.dampingOwner = ryo;
    state.headAnchor.y = static_cast<float>(static_cast<double>(state.headAnchor.y) + 0.12);
    state.position = position;
    state.previousYaw = state.yaw;
    const std::int32_t yaw = ReturnCurrentFixed16Angle();
    state.previousYaw -= yaw;
    state.yaw = yaw;
    state.animationState = g_ryoCurrentAnimationState;
    state.node = ryo->entity.node.child;
    return true;
}

std::int32_t ApplyAnalogStickRotationSmoothing(std::int32_t angle, std::int32_t target, std::int32_t frames) {
    const bool mouseLook = g_keyboardActive == 1 && UpdateMouseHistoryAndGetMouseIsMoving() && g_hasMovedMouse != 0;
    const std::int32_t maximumStep = mouseLook ? 0x888 : 0x444;
    std::int32_t step = static_cast<std::int16_t>(target - angle) / (mouseLook ? 2 : frames);
    if (std::abs(step) > maximumStep)
        step = step > 0 ? maximumStep : -maximumStep;
    return static_cast<std::int16_t>(angle + step);
}

void InterpolateAnalogStickValues(CameraOrbitState &state, const Vector3 &anchor, float distance, Vector3 &output) {
    const std::int32_t enabled = state.interpolationEnabled;
    state.orbit.y = distance;
    const float delta = distance - state.orbit.x;
    if (enabled && static_cast<double>(std::fabs(delta)) > 0.01) {
        const float step = delta / 10.0f;
        state.interpolationState = std::bit_cast<std::int32_t>(step);
        state.orbit.y = step + distance;
        state.orbit.x = state.orbit.y;
    } else {
        state.orbit.x = distance;
    }
    if (enabled) {
        state.orbit.z = static_cast<float>(ApplyAnalogStickRotationSmoothing(static_cast<std::int32_t>(state.orbit.z), state.targetYaw, 32));
        state.orbit.w = static_cast<float>(ApplyAnalogStickRotationSmoothing(static_cast<std::int32_t>(state.orbit.w), state.targetPitch, 32));
    } else {
        state.orbit.z = static_cast<float>(state.targetYaw);
        state.orbit.w = static_cast<float>(state.targetPitch);
    }
    output = {0.0f, 0.0f, -state.orbit.x};
    Matrix4x4::SetActiveTransformToPrePass();
    const std::uint16_t yaw = static_cast<std::uint16_t>(g_cam_input_base_yaw + static_cast<std::int32_t>(state.orbit.z));
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), cos_fixed16(yaw));
    const std::uint16_t pitch = static_cast<std::uint16_t>(g_cam_input_pitch + static_cast<std::int32_t>(state.orbit.w));
    Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(pitch)), cos_fixed16(pitch));
    Matrix4x4::TransformPoint(output, output);
    Matrix4x4::Pop(1);
    output = {anchor.x + output.x, anchor.y + output.y, anchor.z + output.z};
}

void UpdateCameraDistanceDamping(Character *character, float proximity, float scale) {
    if (!character)
        return;
    const float unclampedWeight = 1.0f - scale * 10.0f;
    const float minimumWeight = unclampedWeight > 0.0f ? unclampedWeight : 0.0f;
    const Vector3 position = GetPositionForEntity(character->entity.node);
    const float vertical = static_cast<float>(static_cast<double>(position.y) + 1.7 - static_cast<double>(g_CameraEyePos.y));
    const float dx = position.x - g_CameraEyePos.x;
    const float dz = position.z - g_CameraEyePos.z;
    const float horizontal = std::sqrt((dx * dx + 0.0f) + dz * dz);
    double distance = (static_cast<double>(horizontal) - 0.45) / static_cast<double>(distanceDampingFactor);
    const double previous = previousDistanceFactor;
    if (!(distance >= previous) || !(1.0 >= distance))
        distance = previous > distance ? previous : 1.0;
    float weight = static_cast<float>(distance);
    if (vertical > 2.5f || 0.5 > static_cast<double>(proximity))
        weight = 1.0f;
    character->renderOpacity = weight >= minimumWeight ? weight : minimumWeight;
}

void ProcessCameraAngleSmoothing() {
    Character *const character = GCameraSettings2.dampingOwner;
    if (!character)
        return;
    GSettings.executionFlags |= 2U;
    const float dx = g_CameraEyePos.x - GCameraSettings2.position.x;
    const float dz = g_CameraEyePos.z - GCameraSettings2.position.z;
    if (std::fabs(GSettings.state.orbit.y) < 0.24f || dx * dx + dz * dz < 0.24f)
        GSettings.executionFlags &= ~2U;

    const bool visible = (GSettings.executionFlags & 2U) != 0;
    if (visible) {
        const bool wasVisible = (character->motionFlags & 4U) != 0;
        character->motionFlags |= 4U;
        if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
            CHAR_LogNPCVisibility(character, true);
    }
    distanceDampingFactor = 0.8f;
    previousDistanceFactor = 0.0f;
    UpdateCameraDistanceDamping(character, 1.0f, 1.0f);
    if (visible) {
        character->flags.value &= ~0x200U;
    } else {
        const bool wasVisible = (character->motionFlags & 4U) != 0;
        character->motionFlags = static_cast<std::uint8_t>(character->motionFlags & ~4U);
        if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && wasVisible)
            CHAR_LogNPCVisibility(character, false);
        character->flags.value |= 0x200U;
    }
}

void camCtrlBlendCameraAngles(
    std::int32_t yawAngle, std::int32_t pitchAngle, float rotationWeight, float scale, std::int32_t &blendAngle, std::int32_t &velocity) {
    const std::int16_t yawInput = static_cast<std::int16_t>(yawAngle);
    const std::int16_t pitchInput = static_cast<std::int16_t>(pitchAngle);
    const float pitchAmount = static_cast<float>(std::abs(static_cast<std::int32_t>(pitchInput))) * 0.000030517578f;
    std::int32_t pitchTarget = pitchInput >= 0 ? g_cam_input_pitch + 12743 : g_cam_input_pitch - 7281;
    const std::int32_t previousVelocity = velocity;
    if (previousVelocity)
        pitchTarget = pitchInput >= 0 ? previousVelocity + 12743 : -7281 - previousVelocity;
    const float yaw = static_cast<float>(yawInput);
    const std::int32_t weighted = (static_cast<std::int32_t>(yaw * rotationWeight) << 14) >> 15;
    const float partial = static_cast<float>(blendAngle) - static_cast<float>(weighted) * scale;
    const std::int32_t complement = (13653 * static_cast<std::int32_t>((1.0f - rotationWeight) * yaw)) >> 15;
    blendAngle = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(partial)) - static_cast<float>(complement) * scale);
    const std::int32_t damped = static_cast<std::int32_t>((1.0f - pitchAmount) * static_cast<float>(previousVelocity));
    velocity = static_cast<std::int32_t>(static_cast<float>(damped) - static_cast<float>(pitchTarget) * pitchAmount);
}

Vector3 BlendInterpolatedCameraAngles(float rotationWeight) {
    std::int32_t blendAngle = 0;
    std::int32_t velocity = 0;
    camCtrlBlendCameraAngles(g_playerCameraAngles_0, g_playerCameraAngles_1, rotationWeight, 1.0f, blendAngle, velocity);
    const std::uint16_t pitch = static_cast<std::uint16_t>((velocity * 10012) >> 15);
    const std::int32_t yaw = playerFacingAngle + blendAngle;
    const float pitchCosine = cos_fixed16(pitch);
    Vector3 offset{};
    const double horizontal = static_cast<double>(-(pitchCosine * rotationWeight)) * 0.2;
    offset.x = static_cast<float>(horizontal * static_cast<double>(sin_fixed16(static_cast<std::int16_t>(yaw))));
    const double yawCosine = cos_fixed16(static_cast<std::uint16_t>(yaw));
    offset.z = static_cast<float>(yawCosine * horizontal);
    const float sway = sin_fixed16(static_cast<std::int16_t>(velocity)) * rotationWeight;
    offset.y = static_cast<float>(static_cast<double>(sway) * 0.4 * static_cast<double>(rotationWeight));
    return offset;
}

bool FallbackCameraUpdate() {
    auto &orbit = GSettings;
    orbit.movementFactor = 0.25f;
    GCameraSettings2.dampingOwner = nullptr;
    orbit.executionFlags = (orbit.executionFlags & ~3U) | 2U;
    orbit.fieldOfView = FieldOfView;
    bNoBlendCameraInterp = 0;
    rotationOffsetVector = {};
    orbit.rotationFactor = 0.0f;
    if (!UpdateStates(GCameraSettings2))
        return false;

    const auto playerYaw = static_cast<std::int16_t>(GCameraSettings2.yaw);
    const Vector3 playerAnchor{
        GCameraSettings2.position.x,
        GCameraSettings2.headAnchor.y,
        GCameraSettings2.position.z,
    };
    const Vector3 targetDelta{
        g_CameraEyePos.x - playerAnchor.x,
        g_CameraEyePos.y - playerAnchor.y,
        g_CameraEyePos.z - playerAnchor.z,
    };
    const float targetDistance = std::sqrt(targetDelta.x * targetDelta.x + targetDelta.y * targetDelta.y + targetDelta.z * targetDelta.z);
    cameraInterpProgress = targetDistance > 30.0f ? 1.0f : 0.1f;
    GCameraPos = playerAnchor;
    g_cam_input_base_yaw = playerYaw;
    g_cam_input_pitch = 0;

    Vector3 direction{
        g_CameraTargetPos.x - g_CameraEyePos.x,
        g_CameraTargetPos.y - g_CameraEyePos.y,
        g_CameraTargetPos.z - g_CameraEyePos.z,
    };
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0.0f) {
        const float inverse = 1.0f / length;
        direction.x *= inverse;
        direction.y *= inverse;
        direction.z *= inverse;
    }
    const Vector3 eyeSeed{
        g_CameraEyePos.x + direction.x * 5.0f,
        g_CameraEyePos.y + direction.y * 5.0f,
        g_CameraEyePos.z + direction.z * 5.0f,
    };
    const Vector3 eyeDelta{
        eyeSeed.x - playerAnchor.x,
        eyeSeed.y - playerAnchor.y,
        eyeSeed.z - playerAnchor.z,
    };
    const float eyeDistance = std::sqrt(eyeDelta.x * eyeDelta.x + eyeDelta.y * eyeDelta.y + eyeDelta.z * eyeDelta.z);
    std::int32_t eyePitch = eyeDistance > 0.001f ? asin_fixed16(eyeDelta.y / eyeDistance) : 0;
    if (static_cast<float>(static_cast<std::int16_t>(eyePitch)) / 182.04445f < -16.0f) {
        eyePitch = static_cast<std::int16_t>(62624U);
    }
    GCamSettings = {};
    GCamSettings.interpolationEnabled = 1;
    GCamSettings.orbit.x = eyeDistance;
    GCamSettings.orbit.y = eyeDistance;
    GCamSettings.orbit.w = static_cast<float>(static_cast<std::int16_t>(eyePitch));
    GCamSettings.targetPitch = static_cast<std::int16_t>(eyePitch);
    orbit.state.interpolationEnabled = 1;

    std::int32_t targetYaw = 0;
    std::int32_t targetPitch = 0;
    if (targetDistance > 0.001f) {
        targetYaw = -static_cast<std::int16_t>(atan2_fixed16(targetDelta.x, -targetDelta.z));
        targetPitch = asin_fixed16(targetDelta.y / targetDistance);
    }
    std::int32_t relativeYaw = static_cast<std::int16_t>(targetYaw - playerYaw);
    std::int32_t relativePitch = static_cast<std::int16_t>(targetPitch);
    orbit.state.orbit.x = targetDistance;
    orbit.state.orbit.y = targetDistance;
    if (std::abs(relativeYaw) > 0x4000) {
        relativeYaw = static_cast<std::int16_t>(relativeYaw + 0x8000);
        relativePitch = -relativePitch;
        orbit.state.orbit.y = -targetDistance;
    }
    orbit.state.orbit.z = static_cast<float>(relativeYaw);
    orbit.state.orbit.w = static_cast<float>(relativePitch);
    orbit.state.targetYaw = relativeYaw;
    orbit.state.targetPitch = relativePitch;
    orbit.executionFlags |= 1U;
    orbit.state.interpolationState = 0;
    distanceDampingFactor = 0.8f;
    previousDistanceFactor = 0.0f;
    g_CameraOverrideActiveFlag = 0;
    return true;
}

void UpdateCameraPositionAndRotation() {
    const float analogFactor = useAnalogInput ? static_cast<float>(rawAnalogInputValue) / 245.0f : 0.0f;
    if ((GSettings.executionFlags & 1U) == 0) {
        FallbackCameraUpdate();
        return;
    }
    if (!UpdateStates(GCameraSettings2))
        return;


    cameraInterpProgress = static_cast<float>(static_cast<double>(cameraInterpProgress) + 0.01);
    if (cameraInterpProgress >= 1.0f)
        cameraInterpProgress = 1.0f;
    ProcessCameraAngleSmoothing();
    g_cam_input_base_yaw = ApplyAnalogStickRotationSmoothing(g_cam_input_base_yaw, GCameraSettings2.yaw, 6);
    g_cam_input_pitch = ApplyAnalogStickRotationSmoothing(g_cam_input_pitch, 0, 6);

    Vector3 target{
        GCameraSettings2.position.x,
        GCameraSettings2.headAnchor.y,
        GCameraSettings2.position.z,
    };
    blend_cameraAngleState1 = static_cast<std::int16_t>(g_playerCameraAngles_0);
    blend_cameraAngleState2 = static_cast<std::int16_t>(g_playerCameraAngles_1);
    if (bNoBlendCameraInterp) {
        const auto yaw = static_cast<std::int16_t>(GCameraSettings2.yaw);
        const float sine = sin_fixed16(yaw);
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - yaw));
        target.x += cosine * rotationOffsetVector.x - sine * GSettings.rotationFactor;
        target.y += rotationOffsetVector.y;
        target.z += -cosine * GSettings.rotationFactor - sine * rotationOffsetVector.x;
    } else {
        const Vector3 offset = BlendInterpolatedCameraAngles(analogFactor);
        target.x += offset.x;
        target.y += offset.y;
        target.z += offset.z;
    }

    const std::int16_t fovDelta = static_cast<std::int16_t>(GSettings.fieldOfView - currentPerspectiveFov);
    const double relaxedFov = static_cast<double>(fovDelta) * 0.075 + static_cast<double>(currentPerspectiveFov);
    CAME_SetFieldOfView(static_cast<std::int16_t>(static_cast<std::int32_t>(relaxedFov)));
    float x = GCameraPos.x;
    float z = GCameraPos.z;
    if (std::fabs(target.x - x) > 10.0f)
        x += (target.x - x) * GSettings.movementFactor * 2.0f;
    if (std::fabs(target.z - z) > 10.0f)
        z += (target.z - z) * GSettings.movementFactor * 2.0f;
    GCameraPos.x = x + (target.x - x) * GSettings.movementFactor;
    GCameraPos.z = z + (target.z - z) * GSettings.movementFactor;
    GCameraPos.y += (target.y - GCameraPos.y) * 0.1f;
    g_cameraStateFlags |= 8U;

    std::int32_t finalYaw = 0;
    std::int32_t finalPitch = 0;
    if (bNoBlendCameraInterp) {
        finalYaw = -GSettings.temporaryYaw;
        finalPitch = -GSettings.temporaryPitch;
        if (GSettings.flags == 1) {
            blend_cameraAngleState1 = 0;
            blend_cameraAngleState2 = 0;
        }
    }
    GSettings.state.targetYaw = finalYaw;
    GSettings.state.targetPitch = finalPitch;
    GCamSettings.targetYaw = finalYaw;
    GCamSettings.targetPitch = finalPitch;

    Vector3 cameraEye{};
    Vector3 cameraTarget{};
    InterpolateAnalogStickValues(GSettings.state, GCameraPos, 0.0f, cameraEye);
    InterpolateAnalogStickValues(GCamSettings, GCameraPos, 1.0f, cameraTarget);
    TranslateCameraRelative(cameraEye);
    if (!bLockCamera) {
        g_CameraTargetPos = cameraTarget;
        shenApplyTranslationToAll(g_cameraTargetNode, cameraTarget);
    }
    distanceDampingFactor = 0.8f;
    previousDistanceFactor = 0.0f;
    UpdateCameraDistanceDamping(GCameraSettings2.dampingOwner, 1.0f, 1.0f);
    bNoBlendCameraInterp = 0;
    GSettings.fieldOfView = FieldOfView;
    g_cameraRuntime.viewReady = true;
}

void HandleCameraForAuthorState() {
    const Vector3 eye = g_CameraEyePos;
    const Vector3 target = g_CameraTargetPos;
    CameraInterpolatedOrbit &orbit = g_tmp_cam_fstep_0;
    g_tmp_interpolated_cam_pos = eye;
    GCameraPos = eye;
    orbit.result = target;
    const float dx = target.x - eye.x;
    const float dy = target.y - eye.y;
    const float dz = target.z - eye.z;
    const float distance = std::sqrt(dy * dy + dx * dx + dz * dz);
    if (distance > 0.001) {
        g_cam_input_base_yaw = -static_cast<std::int16_t>(atan2_fixed16(dx, -dz));
        g_cam_input_pitch = asin_fixed16(dy / distance);
    } else {
        g_cam_input_base_yaw = 0;
        g_cam_input_pitch = 0;
    }
    orbit.orbit.y = distance;
    orbit.orbit.x = distance;
    std::int32_t pitchInput = 0;
    if (g_camera_rot_step_lock > 5) {
        const float stepScale = static_cast<float>(g_camera_rot_step_lock) / 5.0f;
        const std::int32_t yawScale = static_cast<std::int32_t>(static_cast<float>(g_authorInputYawScale) * stepScale);
        orbit.targetYaw = -((yawScale * g_playerCameraAngles_0) >> 15);
        pitchInput = static_cast<std::int32_t>(static_cast<float>(g_authorInputPitchScale) * stepScale) * g_playerCameraAngles_1;
    } else {
        orbit.targetYaw = -((g_playerCameraAngles_0 * g_authorInputYawScale) >> 15);
        pitchInput = g_playerCameraAngles_1 * g_authorInputPitchScale;
    }
    orbit.targetPitch = -(pitchInput >> 15);
    InterpolateAnalogStickValues(orbit, GCameraPos, distance, orbit.result);

    const Vector3 lookAt = orbit.result;
    float directionX = lookAt.x - eye.x;
    float directionY = lookAt.y - eye.y;
    float directionZ = lookAt.z - eye.z;
    const float squared = directionY * directionY + directionX * directionX + directionZ * directionZ;
    if (static_cast<double>(squared) > 1e-06) {
        const float length = std::sqrt(squared);
        directionX /= length;
        directionY /= length;
        directionZ /= length;
    }
    const float analog = useAnalogInput ? static_cast<float>(rawAnalogInputValue) / 245.0f : 0.0f;
    const float factor = g_tmp_cam_factor;
    const Vector3 offset{analog * directionX * factor, analog * directionY * factor, analog * directionZ * factor};
    orbit.result = {lookAt.x + offset.x, lookAt.y + offset.y, lookAt.z + offset.z};
    g_tmp_interpolated_cam_pos = {offset.x + eye.x, offset.y + eye.y, offset.z + eye.z};
    GCameraPos = g_tmp_interpolated_cam_pos;
    TranslateCameraRelative(g_tmp_interpolated_cam_pos);
    if (!bLockCamera) {
        g_CameraTargetPos = orbit.result;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    g_cameraRuntime.viewReady = true;
}

void ApplyCameraShake();




void HandleFreeCameraState() {
    matrix_stack::SetupStack();
    SetActiveStack(MatrixStackType::Transparent);
    workTransformMatrix = transformMatrix_prePass;
    // The field-of-view setter owns the cull extents (screenSpaceToWorldScale) for the display aspect.
    CAME_SetFieldOfView(currentPerspectiveFov);
    const DisplayModeInfo *const screen = GetScreenResolution();
    float aspectRatio = static_cast<float>(screen->width) / static_cast<float>(GetScreenResolution()->height);
    std::int32_t pitch = computedCameraPitch;
    if (g_globalFlags[0] == 80) {
        aspectRatio *= 1.16f;
        pitch = static_cast<std::int32_t>(static_cast<float>(computedCameraPitch) * 0.9f);
    }
    // g_fNearClip is 0.03; the E1049C010/E1049A008 voice lines move it.
    Matrix4x4_SetupCameraProjectionMatrix(pitch, aspectRatio, g_cameraNearClip, 2200.0f);
    SetActiveStack(MatrixStackType::DepthPrepass);
    workTransformMatrix = transformMatrix_prePass;
    ApplyCameraShake();

    const Vector3 rawCameraOrigin = GetPositionForEntity(g_cameraEyeNode);
    const Vector3 rawLookAt = GetPositionForEntity(g_cameraTargetNode);
    g_CameraEyePos = rawCameraOrigin;
    g_CameraTargetPos = rawLookAt;

    Vector3 cameraOrigin = rawCameraOrigin;
    Vector3 lookAt = rawLookAt;
    const float dx = cameraOrigin.x - lookAt.x;
    const float dz = cameraOrigin.z - lookAt.z;
    if (std::fabs(dx) >= std::numeric_limits<float>::epsilon() || std::fabs(dz) >= std::numeric_limits<float>::epsilon()) {
        const float horizontal = std::sqrt(dx * dx + dz * dz);
        const float directionX = dx / horizontal;
        const float directionZ = dz / horizontal;
        const Vector3 local = g_tmp_interpolated_camrot;
        const float frameScale = g_bEnableFrameTimeSmoothing ? 30.0f * fFrameTimeMs : 1.0f;
        const float moveX = (local.x * directionZ - local.z * directionX) * frameScale;
        const float moveZ = (-local.x * directionX - local.z * directionZ) * frameScale;

        Vector3 rotatedLookAt = lookAt;
        if (g_tmp_cam_rot_z_adj != 0) {
            const std::int16_t yaw = static_cast<std::int16_t>(g_tmp_cam_rot_z_adj);
            const float sine = sin_fixed16(yaw);
            const float cosine = cos_fixed16(static_cast<std::uint16_t>(yaw));
            const float relativeX = lookAt.x - cameraOrigin.x;
            const float relativeZ = lookAt.z - cameraOrigin.z;
            rotatedLookAt.x = cameraOrigin.x + cosine * relativeX + sine * relativeZ;
            rotatedLookAt.z = cameraOrigin.z + cosine * relativeZ - sine * relativeX;
        }
        const float moveY = local.y * frameScale;
        cameraOrigin.x += moveX;
        cameraOrigin.y += moveY;
        cameraOrigin.z += moveZ;
        lookAt = {
            rotatedLookAt.x + moveX,
            rotatedLookAt.y + moveY,
            rotatedLookAt.z + moveZ,
        };
    }

    const auto blendPoint = [](auto &current, const Vector3 &desired, const Vector3 &factors) {
        current.x += (desired.x - current.x) * factors.x;
        current.y += (desired.y - current.y) * factors.y;
        current.z += (desired.z - current.z) * factors.z;
    };
    blendPoint(tmp_cam_posrot.pos, cameraOrigin, g_script_cam_offs);
    blendPoint(tmp_cam_posrot.target[0], lookAt, g_script_cam_offs2);

    CalcCameraTransform({tmp_cam_posrot.pos.x, tmp_cam_posrot.pos.y, tmp_cam_posrot.pos.z, 0.0f},
        {tmp_cam_posrot.target[0].x, tmp_cam_posrot.target[0].y, tmp_cam_posrot.target[0].z, 0.0f}, static_cast<std::int16_t>(-ZANGLE));
    g_tmp_cam_xform = workTransformMatrix;

    const float yawDeltaZ = rawLookAt.z - rawCameraOrigin.z;
    const float yawDeltaX = rawLookAt.x - rawCameraOrigin.x;
    if (std::fabs(yawDeltaZ) >= 0.001f || std::fabs(yawDeltaX) >= 0.001f)
        g_fCamAccumYaw = static_cast<std::int16_t>(atan2_fixed16(yawDeltaZ, yawDeltaX) + 0x4000);
    else
        g_fCamAccumYaw = 0;

    SetActiveStack(MatrixStackType::PostRender);
    workTransformMatrix = g_tmp_cam_xform;
    SetActiveStack(MatrixStackType::DepthPrepass);
    g_cameraRuntime.viewReady = true;
}

void ResetCameraControlTaskState() {
    g_cameraStateFlags &= ~CF_LookAroundMask;
    GCameraSettings2.dampingOwner = nullptr;
    g_NeedsReinit = 0;
    g_cameraAuthorStateSnapshot = -1;
    if (Character *const ryo = CurrentRyo())
        ryo->renderOpacity = 1.0f;
}

bool InitializeCameraAuthorControlState() {
    if (Character *const ryo = CurrentRyo())
        ryo->renderOpacity = 1.0f;
    g_cameraStateFlags &= ~CF_LookAroundMask;
    g_cameraAuthorStateSnapshot = g_nAuthorState;
    g_cam_target_orbit_state.interpolationEnabled = 1;
    g_cam_target_orbit_state.orbit.x = 0.0f;
    g_cam_target_orbit_state.orbit.y = 0.0f;
    g_tmp_cam_fstep_0.interpolationEnabled = 1;
    g_tmp_cam_fstep_0.orbit.x = 1.0f;
    g_tmp_cam_fstep_0.orbit.y = 1.0f;
    g_tmp_cam_fstep_0.orbit.z = 0.0f;
    g_cam_damping_step_scale = 0.083333336f;
    g_tmp_next_cam_pos.w = 0.083333336f;
    g_NeedsReinit = 1;
    g_authorInputYawScale = 2730;
    g_authorInputPitchScale = 2730;
    g_look_around_angle_x = 0;
    g_look_around_angle_y = 0;
    g_playerCameraAngles_0 = 0;
    g_playerCameraAngles_1 = 0;
    g_tmp_cam_fstep_0.targetYaw = 0;
    g_tmp_cam_fstep_0.targetPitch = 0;
    g_cam_target_orbit_state.targetYaw = 0;
    g_cam_target_orbit_state.targetPitch = 0;
    g_tmp_cam_fstep_0.orbit.w = 0.0f;
    g_cam_target_orbit_state.orbit.z = 0.0f;
    g_cam_target_orbit_state.orbit.w = 0.0f;
    GCameraSettings2.dampingOwner = nullptr;
    return true;
}

bool GetHasAuthorState() {
    if (std::bit_cast<std::uint32_t>(g_tmp_cam_pos.w) == 0U) {
        if (g_NeedsReinit)
            ResetCameraControlTaskState();
        return false;
    }

    if (!g_NeedsReinit && g_nAuthorState != 6 && g_nAuthorState != 8) {
        return false;
    }

    const MapControlData *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control && control->padCaptureEntry == reinterpret_cast<const void *>(static_cast<std::intptr_t>(-4))) {
        return false;
    }

    if (!g_NeedsReinit)
        return InitializeCameraAuthorControlState();
    if (g_cameraAuthorStateSnapshot == g_nAuthorState)
        return true;
    ResetCameraControlTaskState();
    return false;
}

void MaintainHorizontalDistance(Vector3 &point, const Vector3 &origin, float distance);


bool IsNormalOrZero(const Vector3 &value) {
    return (std::isnormal(value.x) || value.x == 0.0f) && (std::isnormal(value.y) || value.y == 0.0f) && (std::isnormal(value.z) || value.z == 0.0f);
}

void RecalculateCameraTargetYawAndPitch() {
    const Vector3 &eye = g_followCameraDesiredEye;
    const Vector3 &lookAt = g_CameraLookDirection;
    const float dy = lookAt.y - eye.y;
    const float dz = lookAt.z - eye.z;
    const float dx = lookAt.x - eye.x;
    const float distance = std::sqrt(dy * dy + dx * dx + dz * dz);
    if (distance <= 0.001) {
        g_cameraTargetPitch = 0;
        g_cameraTargetYaw = 0;
        return;
    }
    g_cameraTargetYaw = -static_cast<std::int16_t>(atan2_fixed16(dx, -dz));
    g_cameraTargetPitch = asin_fixed16(dy / distance);
}

void CalculateCameraAngle(CameraInterpolatedOrbit &orbit, const Vector3 &point) {
    const Vector3 &anchor = GCameraPos;
    const float dy = point.y - anchor.y;
    const float dz = point.z - anchor.z;
    const float dx = point.x - anchor.x;
    orbit.interpolationEnabled = 1;
    const float distance = std::sqrt(dy * dy + dx * dx + dz * dz);
    std::int32_t yaw = 0;
    std::int32_t pitch = 0;
    if (distance > 0.001) {
        yaw = -static_cast<std::int16_t>(atan2_fixed16(dx, -dz));
        pitch = asin_fixed16(dy / distance);
    }
    orbit.orbit.x = distance;
    orbit.orbit.y = distance;
    orbit.interpolationState = 0;
    orbit.orbit.z = static_cast<float>(static_cast<std::int16_t>(yaw - g_cam_input_base_yaw));
    orbit.orbit.w = static_cast<float>(static_cast<std::int16_t>(pitch - g_cam_input_pitch));
}



std::uint32_t CheckIntersection(const Vector3 &start, const Vector3 &end, float radius, float &fraction) {
    const MapControlData *const control = CurrentMapControl();
    std::array<std::uint8_t, sizeof(control->cycleMask)> mask{};
    std::memcpy(mask.data(), control->cycleMask, mask.size());
    if (!cpctIsTatehizaStateActive())
        mask[1] &= 0xfcU;
    CharacterVectorCollisionResult result{};
    COLS_TraceMapVectorWithMask(start, end, radius, result, mask.data());
    std::uint32_t hitKind = result.flags & 7U;
    coliTestCharacterShapes(10, start, end, radius, result, hitKind);
    coliTestAuxiliaryShapes(10, start, end, radius, result, hitKind);
    result.flags = (result.flags & ~7U) | (hitKind & 7U);
    fraction = result.fraction;
    return result.flags & 7U;
}

float RaycastIntoScene(const Vector3 &start, const Vector3 &direction, float radius) {
    const Vector3 end{start.x + direction.x * 5.0f, start.y, start.z + direction.z * 5.0f};
    float fraction = 0.0f;
    const std::uint32_t hit = CheckIntersection(start, end, radius, fraction);
    if (hit != 1U)
        return hit == 2U ? 0.0f : 5.0f;
    if (fraction < 0.0f)
        return 5.0f;
    const float distance = fraction * 5.0f;
    return distance < 5.0f ? distance : 5.0f;
}

float HandleColi(std::uint16_t angle) {
    Character *const ryo = CurrentRyo();
    if (!ryo)
        return 0.0f;
    const Vector3 position = GetPositionForEntity(ryo->entity.node);
    const float probeX = sin_fixed16(static_cast<std::int16_t>(angle)) + position.x;
    const float probeZ = cos_fixed16(angle) + position.z;
    if (!g_CameraCollisionEnabled)
        return 0.0f;
    Vector3 start = position;
    Vector3 end{probeX, position.y, probeZ};
    if (ryo->entity.node.child) {
        start = WorldToEntityLocalPoint(*ryo->entity.node.child, start);
        end = WorldToEntityLocalPoint(*ryo->entity.node.child, end);
    }
    const Vector3 direction{end.x - start.x, 0.0f, end.z - start.z};

    if (std::sqrt((probeX * probeX + position.y * position.y) + probeZ * probeZ) == 0.0f)
        return 0.0f;
    return RaycastIntoScene(start, direction, 0.3f);
}

void SmoothCameraMovement(bool incremental) {
    std::int32_t blendAngle = 0;
    std::int32_t velocity = 0;
    const float rotationWeight = useAnalogInput ? static_cast<float>(rawAnalogInputValue) / 245.0f : 0.0f;
    camCtrlBlendCameraAngles(g_playerCameraAngles_0, g_playerCameraAngles_1, rotationWeight, 1.0f, blendAngle, velocity);
    std::array<float, 32> &radii = smoothed_cam_rots;
    std::array<float, 32> &filtered = filtered_cam_rots;
    const std::int32_t sector = (g_cam_input_base_yaw + blendAngle) / 2047;
    std::int32_t first = 0;
    std::int32_t last = 31;
    if (incremental) {
        first = sector - 2;
        last = sector + 2;
        if (GCameraSettings2.previousYaw <= 0)
            last += 2;
        else
            first -= 2;
    }

    for (std::int32_t index = first; index <= last; ++index) {
        const float hit = HandleColi(static_cast<std::uint16_t>(2047 * index));
        const float radius = static_cast<float>(static_cast<double>(hit) * 0.9 - 0.31);
        const float weight = static_cast<float>(static_cast<double>(std::abs(sector - index)) * 0.1 + 0.3);
        float &slot = radii[static_cast<std::uint32_t>(index) & 31U];
        slot = incremental ? (1.0f - weight) * slot + weight * radius : radius;
    }

    std::uint16_t yaw = static_cast<std::uint16_t>(g_cam_input_base_yaw - 2047 * first);
    for (std::int32_t index = first; index <= last; ++index) {
        const double lift = static_cast<double>(cos_fixed16(yaw)) * 0.15;
        const double floor = (lift >= 0.0 ? lift : 0.0) + 0.45;
        float &slot = radii[static_cast<std::uint32_t>(index) & 31U];
        const double value = slot;
        if (!(value >= floor) || !(3.02 >= value))
            slot = static_cast<float>(floor > value ? floor : 3.02);
        yaw = static_cast<std::uint16_t>(yaw - 2047);
    }

    yaw = static_cast<std::uint16_t>(g_cam_input_base_yaw);
    for (std::uint32_t index = 0; index < 32; ++index) {
        const double lift = static_cast<double>(cos_fixed16(yaw)) * 0.15;
        const double floor = (lift >= 0.0 ? lift : 0.0) + 0.45;
        const double value = radii[index];
        filtered[index] = static_cast<float>(value >= floor ? value : floor);
        yaw = static_cast<std::uint16_t>(yaw - 2047);
    }

    std::array<float, 32> minimum = filtered;
    for (std::uint32_t index = 0; index < 32; ++index) {
        float value = filtered[index];
        for (std::uint32_t offset = 0; offset < 5; ++offset) {
            const float candidate = filtered[(index + offset - 2U) & 31U];
            if (value > candidate)
                value = candidate;
        }
        minimum[index] = value;
    }
    for (std::uint32_t index = 0; index < 32; ++index) {
        float sum = minimum[(index - 2U) & 31U] / 5.0f + 0.0f;
        sum += minimum[(index - 1U) & 31U] / 5.0f;
        sum += minimum[index] / 5.0f;
        sum += minimum[(index + 1U) & 31U] / 5.0f;
        sum += minimum[(index + 2U) & 31U] / 5.0f;
        filtered[index] = sum;
    }
}

void RotateAndNormalise(CameraInterpolatedOrbit &orbit, const Vector3 &anchor, bool forward, Vector3 &output) {
    const std::int32_t pitchBase = g_cam_input_pitch;
    const std::int32_t yawBase = g_cam_input_base_yaw;
    const std::int32_t sampleYaw = static_cast<std::int32_t>(orbit.orbit.z) + yawBase;
    const PlayerControlWork *const control = g_pControlPadController;
    const std::int32_t yawFrames = control && control->lookYawUsesMovementInput == 1 ? 16 : 6;
    orbit.orbit.z = static_cast<float>(ApplyAnalogStickRotationSmoothing(static_cast<std::int32_t>(orbit.orbit.z), orbit.targetYaw, yawFrames));
    const std::int32_t pitchFrames = control && control->lookPitchUsesMovementInput == 1 ? 16 : 6;
    orbit.orbit.w = static_cast<float>(ApplyAnalogStickRotationSmoothing(static_cast<std::int32_t>(orbit.orbit.w), orbit.targetPitch, pitchFrames));

    float negativePitchDepth = -2.0f;
    float positivePitchDepth = -2.0f;
    if (forward) {
        float depth = 0.0f;
        const std::int32_t bucket = sampleYaw / 2047;
        for (std::int32_t offset = -4; offset <= 4; ++offset) {
            const std::int32_t sector = static_cast<std::int32_t>(static_cast<std::uint32_t>(offset + bucket) & 31U);
            const std::int32_t startScaled = static_cast<std::int16_t>(2047 * sector - sampleYaw - 1023) * 32;
            std::int32_t start = startScaled / 6;
            if (start < -32768 || startScaled >= 0x30000)
                start = start < -32768 ? -32768 : 32767;
            const std::int32_t endScaled = static_cast<std::int16_t>(2047 * sector - sampleYaw + 1024) * 32;
            std::int32_t end = endScaled / 6;
            if (end < -32768 || endScaled >= 0x30000)
                end = end < -32768 ? -32768 : 32767;
            const float endSine = sin_fixed16(static_cast<std::int16_t>(end));
            const float startSine = sin_fixed16(static_cast<std::int16_t>(start));
            const float endRadians = static_cast<float>(static_cast<double>(static_cast<float>(end)) * 3.14159265358979 * 3.0517578125e-05);
            const float startRadians = static_cast<float>(static_cast<double>(static_cast<float>(start)) * 3.14159265358979 * 3.0517578125e-05);
            const float chord = endSine - (startSine + endRadians - startRadians);
            const double radius = filtered_cam_rots[static_cast<std::uint32_t>(sector)];
            depth = static_cast<float>(static_cast<double>(chord) / 6.28318530717958 * radius + static_cast<double>(depth));
        }

        const float current = orbit.orbit.x;
        const float delta = depth - current;
        if (!orbit.interpolationEnabled || !(static_cast<double>(std::fabs(delta)) > 0.01)) {
            orbit.orbit.x = depth;
        } else {
            const float magnitude = 0.0f > current ? -current : current;
            const float scale = magnitude > std::fabs(depth) ? g_cam_damping_step_scale : g_tmp_next_cam_pos.w;
            float step = delta * scale;
            if (static_cast<double>(std::fabs(step)) > 0.35)
                step = static_cast<float>((step > 0.0f ? 1.0 : -1.0) * 0.35);
            if (0.01 > static_cast<double>(std::fabs(step)))
                orbit.orbit.x = static_cast<float>(static_cast<double>(current) + (step > 0.0f ? 0.01 : -0.01));
            else
                orbit.orbit.x = current + step;
        }

        const float rise = anchor.y - GCameraSettings2.position.y;
        const double lowered = static_cast<double>(rise) - 2.0;
        negativePitchDepth = static_cast<float>(-1.0 > lowered ? -1.0 : lowered);
        const double clearance = static_cast<double>(-rise) - 0.01;
        const double deepest = static_cast<double>(static_cast<float>(static_cast<double>(depth) > clearance ? static_cast<double>(depth) : clearance));
        positivePitchDepth = static_cast<float>(deepest < -0.45 ? deepest : -0.45);
    } else {
        orbit.orbit.x = -2.0f;
    }

    Vector3 offset{0.0f, 0.0f, -orbit.orbit.x};
    Matrix4x4::SetActiveTransformToPrePass();
    const std::uint16_t yaw = static_cast<std::uint16_t>(yawBase + static_cast<std::int32_t>(orbit.orbit.z));
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), cos_fixed16(yaw));
    const std::uint16_t pitch = static_cast<std::uint16_t>(pitchBase + static_cast<std::int32_t>(orbit.orbit.w));
    Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(pitch)), cos_fixed16(pitch));
    Matrix4x4::TransformPoint(offset, output);
    Matrix4x4::Pop(1);

    const float x = output.x;
    const float y = output.y;
    const float z = output.z;
    const float length = std::sqrt(y * y + x * x + z * z);
    const float scale = static_cast<std::int16_t>(pitch) >= 0 ? length / positivePitchDepth : length / negativePitchDepth;
    const float scaledY = y * scale;
    output.y = scaledY;
    const float squared = scaledY * scaledY + x * x + z * z;
    if (static_cast<double>(squared) > 1e-06) {
        const float norm = std::sqrt(squared);
        output = {x / norm, scaledY / norm, z / norm};
    }
    output = {length * output.x, length / scale * output.y, length * output.z};
    if (!forward)
        output = {-output.x, -output.y, -output.z};
    output = {anchor.x + output.x, anchor.y + output.y, anchor.z + output.z};
}

void camBeginDampedAnalogControl() {
    g_rawAnalogMagnitudeFixed = 0;
    UpdateStates(GCameraSettings2);
    if (!GCameraSettings2.dampingOwner)
        return;
    g_turnYawOffsetActive = 1;
    SmoothCameraMovement(false);
    g_cameraStateFlags |= CF_DampingOverride | CF_CameraOverrideActive;
    if (HasActiveQueueNineTask())
        g_cameraStateFlags |= CF_KeepRyoAlwaysVisible;

    const Vector3 eye = g_CameraEyePos;
    const Vector3 target = g_CameraTargetPos;
    g_tmp_next_cam_pos = {eye.x, eye.y, eye.z, g_tmp_next_cam_pos.w};
    g_tmp_next_cam_rot = target;
    g_tmp_cam_rot = target;
    g_cam_damping_step_scale = 0.25f;
    g_tmp_next_cam_pos.w = 0.125f;
    g_tmp_cam_pos = {eye.x, eye.y, eye.z, g_tmp_cam_pos.w};
    g_curr_look_around_factor = 0.0f;
    g_cam_input_base_yaw = playerFacingAngle;
    g_cam_input_pitch = 0;
    RecalculateCameraTargetYawAndPitch();
    GCameraPos = {target.x * 0.5f + eye.x * 0.5f, target.y * 0.5f + eye.y * 0.5f, target.z * 0.5f + eye.z * 0.5f};

    CameraInterpolatedOrbit &eyeOrbit = g_cam_target_orbit_state;
    CalculateCameraAngle(eyeOrbit, eye);
    if (std::abs(static_cast<std::int32_t>(eyeOrbit.orbit.z)) > 0x4000) {
        eyeOrbit.orbit.z = static_cast<float>(static_cast<std::int16_t>(static_cast<std::int32_t>(eyeOrbit.orbit.z) + 0x8000));
        eyeOrbit.orbit.x = -eyeOrbit.orbit.x;
        eyeOrbit.orbit.w = -eyeOrbit.orbit.w;
    }
    CameraInterpolatedOrbit &targetOrbit = g_tmp_cam_fstep_0;
    CalculateCameraAngle(targetOrbit, target);
    targetOrbit.orbit.x = 1.0f;
    if (std::abs(static_cast<std::int32_t>(targetOrbit.orbit.z)) > 0x4000) {
        targetOrbit.orbit.z = static_cast<float>(static_cast<std::int16_t>(static_cast<std::int32_t>(targetOrbit.orbit.z) + 0x8000));
        targetOrbit.orbit.w = -targetOrbit.orbit.w;
        targetOrbit.orbit.x = -1.0f;
    }
    distanceDampingFactor = 0.8f;
    g_curr_look_around_frms = 0;
    previousDistanceFactor = 0.0f;
}


void ctrlSeedLookAtFromHeadAngles() {
    Character *const owner = GCameraSettings2.dampingOwner;
    MotionManagerWork *const manager = owner ? MOTM_GetWork(owner) : nullptr;
    if (!manager)
        return;
    g_ctrl_set_lookat = 1;
    g_ctrlLookAtYaw = -manager->motion.headLookAngleBase;
    g_ctrlLookAtPitch = -manager->motion.lookAtMode;
}

bool shenGetFreeCameraMode() {
    if (!g_hazukiResidenceGatedFlag || !CurrentRyo())
        return false;
    if (!ShouldRun() && !HasActiveQueueNineTask())
        return false;
    return g_nAuthorState == 0 || g_nAuthorState == 10;
}

bool shenIsPressingLookAroundInput() {
    if (g_look_around_angle_x || g_look_around_angle_y)
        return true;
    const std::uint8_t pressure = GetLookingAroundFactor();
    const float held = useAnalogInput && pressure >= 1U ? 1.0f : 0.0f;
    return static_cast<double>(held) > 0.05;
}

std::int32_t ctrlUpdateCameraLookAtVectors() {
    Character *const ryo = CurrentRyo();
    GCameraSettings2.dampingOwner = ryo;
    if (!g_ctrlLookAtEnabled || g_nAuthorState == 10 || g_VF_camera_active || !ryo || !shenGetFreeCameraMode())
        return 0;
    if (!g_ctrl_set_lookat)
        ctrlSeedLookAtFromHeadAngles();

    Vector3 &head = g_cam_target_orbit_state.result;
    mtwkGetCharaBonePosition(ryo, 0x2f, &head);
    const std::uint32_t flags = g_cameraStateFlags;
    if ((flags & CF_KeepRyoAlwaysVisible) == 0)
        head.y = static_cast<float>(static_cast<double>(head.y) + 0.12);

    const Vector3 &motion = GCameraSettings2.motionDelta;
    const float speed = static_cast<float>(static_cast<double>(std::sqrt(motion.y * motion.y + motion.x * motion.x + motion.z * motion.z)) / 0.23);
    const float speedWeight = speed > 1.0f ? 1.0f : (0.0f > speed ? 0.0f : speed);
    const std::int32_t active = g_turnYawOffsetActive;
    std::int32_t yawInput = (g_playerCameraAngles_0 * 10012) >> 16;
    const std::int32_t pitchLift = static_cast<std::int32_t>(static_cast<double>(speedWeight) * 15.0 * 8192.0 / 45.0);
    const std::int32_t pitchBias = -static_cast<std::int32_t>(static_cast<std::uint16_t>(pitchLift));
    std::int32_t yawBase = playerFacingAngle;
    if (active) {
        const std::int32_t accumulated = g_turnYawOffsetFromAccum;
        yawBase = g_previousPlayerNodeYaw;
        yawInput = -((1 - active) * yawInput + accumulated * active);
        g_turnYawOffsetFromAccum = 0;
        if (accumulated)
            yawBase = g_playerNodeYaw;
    } else {
        yawInput = -yawInput;
    }

    const std::int32_t pitchInput = -((g_playerCameraAngles_1 << 13) >> 15);
    std::uint32_t dirty = 0;
    if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(g_ctrlLookAtYaw - yawInput))) < 364 &&
        std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(g_ctrlLookAtPitch - pitchInput))) < 364)
        dirty = CF_LookAtDirty;
    g_ctrlLookAtYaw = yawInput;
    g_cameraStateFlags = (flags & ~CF_LookAtDirty) | dirty;
    g_ctrlLookAtPitch = ApplyAnalogStickRotationSmoothing(g_ctrlLookAtPitch, pitchInput, 10);
    const std::int32_t nodeYaw = static_cast<std::int16_t>(yawBase + yawInput);
    const std::int32_t nodePitch = static_cast<std::int16_t>(g_ctrlLookAtPitch + pitchBias);
    g_ctrlLookAtAppliedPitch = nodePitch;
    g_ctrlLookAtAppliedYaw = nodeYaw;

    Matrix4x4::SetActiveTransformToPrePass();
    Vector3 point{};
    mtwkGetCharaBonePosition(GCameraSettings2.dampingOwner, 0x2f, &point);
    Matrix4x4::preTranslate(point.x, point.y, point.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(nodeYaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(nodePitch + 0x1f4a0));
    point = {0.0f, 0.0f, -1.0f};
    Matrix4x4::TransformPoint(point, point);
    if (g_nAuthorState == 0x11) {
        g_ctrlLookAtPoint = point;
    } else {
        Matrix4x4::RotateAroundX(sin_fixed16(1456), sin_fixed16(14928));
        point = {0.0f, 0.0f, -1.0f};
        Matrix4x4::TransformPoint(point, point);
        g_ctrlLookAtPoint = {point.x, static_cast<float>(static_cast<double>(point.y) + 0.12), point.z};
    }
    Matrix4x4::Pop(1);
    SetLookPointDetails(GCameraSettings2.dampingOwner, 182, &g_ctrlLookAtPoint, 0);
    g_turnYawOffsetActive = active - 1 < 0 ? 0 : active - 1;
    return 1;
}

void ctrlResetCharaLookAtPositionIfNeeded(std::int32_t updated) {
    Character *const ryo = CurrentRyo();
    if (updated || !g_ctrl_set_lookat)
        return;
    if (ryo) {
        const Vector3 target{};
        SetCharaLookAtPosition(ryo, -1, &target, 0);
    }
    g_ctrl_set_lookat = 0;
}

LookAroundTickResult srchTickLookAround() {
    if (g_NeedsReinit)
        return LOOKAROUND_DISABLED;
    if (!shenGetFreeCameraMode()) {
        g_cameraStateFlags &= ~CF_LookAroundMask;
        GCameraSettings2.dampingOwner = nullptr;
        return LOOKAROUND_DISABLED;
    }

    const bool pressing = shenIsPressingLookAroundInput();
    const float factor = g_curr_look_around_factor;
    std::uint32_t flags = g_cameraStateFlags;
    if (pressing) {
        flags &= ~CF_LookAroundActive;
        g_cameraStateFlags = flags;
    } else {
        if ((flags & CF_CameraOverrideActive) == 0)
            return LOOKAROUND_IDLE_OR_FINISHED;
        if ((flags & CF_LookAroundActive) == 0) {
            flags |= CF_LookAroundActive;
            g_curr_look_around_frms = 0;
            g_cameraStateFlags = flags;
        } else if (0.001 > static_cast<double>(factor)) {
            ++g_curr_look_around_frms;
        }
    }

    if ((flags & CF_CameraOverrideActive) == 0 || (flags & CF_LookAroundActive) == 0)
        return LOOKAROUND_ACTIVE;
    const std::int16_t frames = g_curr_look_around_frms;
    const bool settled = 0.001 > static_cast<double>(factor);
    if ((flags & CF_KeepRyoAlwaysVisible) == 0 || !settled || frames <= 20) {
        if (!settled || frames <= 50)
            return LOOKAROUND_ACTIVE;
        if (g_nAuthorState == 0x11) {
            FallbackCameraUpdate();
            flags = g_cameraStateFlags;
        }
    }
    GCameraSettings2.dampingOwner = nullptr;
    g_cameraStateFlags = flags & ~CF_LookAroundMask;
    return LOOKAROUND_IDLE_OR_FINISHED;
}

void camCtrlCalcNewLookAtPoint(Vector3 &output) {
    Character *const ryo = CurrentRyo();
    float originX = 0.0f;
    float originZ = 0.0f;
    if (ryo) {
        const Vector3 position = GetPositionForEntity(ryo->entity.node);
        originX = position.x;
        originZ = position.z;
    }
    if (g_nAuthorState == 10 || g_VF_camera_active) {
        mtwkGetCharaBonePosition(ryo, 0x2f, &output);
        g_cameraTargetYaw = playerFacingAngle;
        g_cameraTargetPitch = 0;
        return;
    }

    const std::int32_t lookYaw = g_playerCameraAngles_0;
    const std::int32_t lookPitch = g_playerCameraAngles_1;
    const float energy = static_cast<float>(lookYaw * lookYaw - 49000000 + lookPitch * lookPitch);
    float raise = energy > 0.0f ? std::sqrt(energy) / 50.0f : std::sqrt(-energy) / -20.0f;
    if (!(raise >= -200.0f) || !(200.0f >= raise))
        raise = -200.0f > raise ? -200.0f : 200.0f;
    std::int16_t accumulator = static_cast<std::int16_t>(g_cameraSmoothingAccumulator[0] + static_cast<std::int32_t>(raise));
    g_cameraSmoothingAccumulator[0] = accumulator;
    if (static_cast<std::uint16_t>(accumulator) > 12000U) {
        accumulator = accumulator < 0 ? 0 : 12000;
        g_cameraSmoothingAccumulator[0] = accumulator;
    }
    const float analog = useAnalogInput ? static_cast<float>(rawAnalogInputValue) / 245.0f : 0.0f;
    const std::int16_t magnitude = static_cast<std::int16_t>(static_cast<std::int32_t>(analog * 10000.0f));
    g_rawAnalogMagnitudeFixed = magnitude;
    float amount = static_cast<float>(accumulator + 2 * magnitude) / 5000.0f;
    if (!(amount >= 0.0f) || !(1.0f >= amount))
        amount = !(0.0f > amount) ? 1.0f : 0.0f;

    const Vector3 &eye = g_followCameraDesiredEye;
    const Vector3 &lookAt = g_CameraLookDirection;
    const float dx = lookAt.x - eye.x;
    const float dz = lookAt.z - eye.z;
    const float dy = lookAt.y - eye.y;
    const float targetDistance = std::sqrt(dy * dy + dx * dx + dz * dz);
    std::int32_t yaw = 0;
    std::int32_t pitch = 0;
    if (targetDistance > 0.001) {
        yaw = -static_cast<std::int16_t>(atan2_fixed16(dx, -dz));
        pitch = asin_fixed16(dy / targetDistance);
    }
    g_cameraTargetPitch = pitch;
    g_cameraTargetYaw = yaw;
    float headY = 0.0f;
    if (ryo) {
        Vector3 head{};
        mtwkGetCharaBonePosition(ryo, 0x2f, &head);
        headY = head.y;
    }

    const std::int32_t pitchSum = g_cam_input_pitch + static_cast<std::int32_t>(g_cam_target_orbit_state.orbit.w);
    const std::int32_t scaled = pitchSum < 0 ? pitchSum / 2 : pitchSum * 2;
    std::int32_t tilt = (scaled - 0x2000) / 3 + 0x1555;
    float offsetBias = 0.0f;
    float heightScale = 0.5f;
    if (tilt >= 0) {
        tilt *= 2;
    } else {
        offsetBias = 2.0f;
        heightScale = 0.7f;
    }
    const float tiltCosine = cos_fixed16(static_cast<std::uint16_t>(tilt));
    const float offsetFactor = (tiltCosine + offsetBias) / (offsetBias + 1.0f);
    const std::uint16_t baseYaw = static_cast<std::uint16_t>(g_cam_input_base_yaw);
    const double yawSine = sin_fixed16(static_cast<std::int16_t>(baseYaw));
    const float shiftedX = static_cast<float>(static_cast<double>(originX) - yawSine * 0.2 * static_cast<double>(offsetFactor));
    const double yawCosine = cos_fixed16(baseYaw);
    const float shiftedZ = static_cast<float>(static_cast<double>(originZ) - yawCosine * 0.2 * static_cast<double>(offsetFactor));
    const float tiltSine = sin_fixed16(static_cast<std::int16_t>(tilt)) * heightScale;
    const float shiftedY = static_cast<float>(static_cast<double>(tiltSine) - 0.1 + static_cast<double>(headY));

    const float toEyeX = originX - eye.x;
    const float toEyeZ = originZ - eye.z;
    const float eyeDistance = std::sqrt((toEyeX * toEyeX + 0.0f) + toEyeZ * toEyeZ);
    const float toTargetX = originX - lookAt.x;
    const float toTargetZ = originZ - lookAt.z;
    const float targetPlaneDistance = std::sqrt((toTargetX * toTargetX + 0.0f) + toTargetZ * toTargetZ);
    const float blend = eyeDistance / (targetPlaneDistance + eyeDistance);
    const float inverseBlend = 1.0f - blend;
    const float remaining = 1.0f - amount;
    output.x = (blend * lookAt.x + inverseBlend * eye.x) * remaining + shiftedX * amount;
    output.y = (lookAt.y * blend + inverseBlend * eye.y) * remaining + shiftedY * amount;
    output.z = (blend * lookAt.z + inverseBlend * eye.z) * remaining + shiftedZ * amount;

    const Vector3 swing = BlendInterpolatedCameraAngles(analog);
    const float swingY = swing.y > 0.0f ? swing.y / 3.0f : static_cast<float>(static_cast<double>(swing.y) * 1.3);
    output.x += swing.x;
    output.y += swingY;
    output.z += swing.z;
}

void camCtrlUpdateTargetFromInputAndDistance(float rotationWeight) {
    Character *const ryo = CurrentRyo();
    Vector3 origin{};
    if (ryo)
        origin = GetPositionForEntity(ryo->entity.node);
    const bool keepVisible = (g_cameraStateFlags & CF_KeepRyoAlwaysVisible) != 0;
    if (keepVisible)
        rotationWeight = 0.0f;

    std::int32_t blendAngle = 0;
    std::int32_t velocity = 0;
    camCtrlBlendCameraAngles(g_playerCameraAngles_0, g_playerCameraAngles_1, rotationWeight, 1.0f, blendAngle, velocity);
    const bool lookingDown = g_playerCameraAngles_1 > 0x7000 && useAnalogInput && static_cast<float>(rawAnalogInputValue) / 245.0f > 0.5f;
    std::int32_t &raiseFrames = g_tmp_interp_count;
    if (lookingDown) {
        if (++raiseFrames > 30)
            raiseFrames = 30;
    } else if (--raiseFrames < 0) {
        raiseFrames = 0;
    }
    const float velocityScale =
        raiseFrames >= 20 ? static_cast<float>(static_cast<double>(static_cast<float>(30 - raiseFrames)) / 30.0 * 0.09999999999999998 + 0.9) : 1.0f;

    CameraInterpolatedOrbit &eyeOrbit = g_cam_target_orbit_state;
    CameraInterpolatedOrbit &targetOrbit = g_tmp_cam_fstep_0;
    const std::int32_t frames = keepVisible ? 24 : 12;
    eyeOrbit.targetYaw = blendAngle;
    targetOrbit.targetYaw = blendAngle;
    const std::int32_t pitch = static_cast<std::int32_t>(static_cast<float>(velocity) * velocityScale);
    eyeOrbit.targetPitch = pitch;
    g_cam_input_base_yaw = ApplyAnalogStickRotationSmoothing(g_cam_input_base_yaw, g_cameraTargetYaw, frames);
    g_cam_input_pitch = ApplyAnalogStickRotationSmoothing(g_cam_input_pitch, g_cameraTargetPitch, frames);

    const Vector3 &motion = GCameraSettings2.motionDelta;
    const float speed = std::sqrt(motion.y * motion.y + motion.x * motion.x + motion.z * motion.z);
    const float stillness = static_cast<float>(1.0 - (static_cast<double>(speed) + static_cast<double>(speed)));
    const float stillWeight = stillness > 1.0f ? 1.0f : (0.0f > stillness ? 0.0f : stillness);
    targetOrbit.targetPitch = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(pitch) / 182.04445f * stillWeight * 8192.0f / 45.0f));

    Vector3 lookAt = origin;
    camCtrlCalcNewLookAtPoint(lookAt);
    std::int32_t &dropFrames = g_tmp_interp_count_0;
    if (lookingDown) {
        if (++dropFrames > 30)
            dropFrames = 30;
    } else {
        dropFrames = dropFrames - 1 < 0 ? 0 : dropFrames - 1;
    }
    const float loweredY = static_cast<float>(static_cast<double>(lookAt.y) - static_cast<double>(static_cast<float>(dropFrames)) / 30.0 * 0.6);
    Vector3 &anchor = GCameraPos;
    anchor.x = static_cast<float>(static_cast<double>(lookAt.x - anchor.x) * 0.5 + static_cast<double>(anchor.x));
    anchor.y = static_cast<float>(static_cast<double>(loweredY - anchor.y) * 0.5 + static_cast<double>(anchor.y));
    anchor.z = static_cast<float>(static_cast<double>(lookAt.z - anchor.z) * 0.5 + static_cast<double>(anchor.z));

    const Vector3 &eye = g_CameraEyePos;
    const float eyeDx = eye.x - origin.x;
    const float eyeDy = eye.y - origin.y;
    const float eyeDz = eye.z - origin.z;
    const float eyeDistance = std::sqrt(eyeDy * eyeDy + eyeDx * eyeDx + eyeDz * eyeDz);
    eyeOrbit.orbit.y = eyeDistance;
    const float speedNow = std::sqrt(motion.y * motion.y + motion.x * motion.x + motion.z * motion.z);
    const double reach = static_cast<double>(std::fabs(eyeDistance)) * 0.37 + static_cast<double>(speedNow) * 4.0;
    targetOrbit.orbit.y = static_cast<float>(reach > 0.3 ? reach : 0.3);
    const std::int32_t state = GCameraSettings2.animationState;
    if (state != 0 || !(0.03 > static_cast<double>(speedNow))) {
        g_tmp_next_cam_pos.w = 0.125f;
        eyeOrbit.interpolationEnabled = 1;
        targetOrbit.interpolationEnabled = 1;
        g_cam_damping_step_scale = state == 10 || state == 4 ? 0.375f : 0.25f;
        return;
    }
    if (0.01 > static_cast<double>(std::fabs(eyeDistance - eyeOrbit.orbit.x)))
        eyeOrbit.interpolationEnabled = 0;
    if (0.01 > static_cast<double>(std::fabs(targetOrbit.orbit.y - targetOrbit.orbit.x)))
        targetOrbit.interpolationEnabled = 0;
    g_tmp_next_cam_pos.w = 0.125f;
    g_cam_damping_step_scale = 0.25f;
}

void camHandleDamping() {
    if (g_bNegativeCamBlend || g_cameraOverrideFlipDirectionActive)
        return;
    const std::int32_t useAnalog = useAnalogInput;
    const float analog = useAnalog ? static_cast<float>(rawAnalogInputValue) / 245.0f : 0.0f;
    const float rotationWeight = static_cast<float>(static_cast<double>(analog) * 0.8);
    UpdateStates(GCameraSettings2);
    Character *const owner = GCameraSettings2.dampingOwner;
    if (!owner)
        return;
    owner->flags.value |= 0x40U;
    if ((g_cameraStateFlags & CF_LookAroundActive) == 0 && (g_cameraStateFlags & CF_DampingOverride) == 0)
        camBeginDampedAnalogControl();

    float nextFactor = 0.0f;
    if (shenIsPressingLookAroundInput()) {
        const float factor = g_curr_look_around_factor;
        nextFactor = static_cast<float>(static_cast<double>(factor) * 0.05 + 0.02 + static_cast<double>(factor));
        const std::uint8_t pressure = GetLookingAroundFactor();
        const float held = useAnalog && pressure >= 1U ? 1.0f : 0.0f;
        if (static_cast<double>(held) > 0.05)
            nextFactor = static_cast<float>(static_cast<double>(nextFactor) + 0.05);
    } else {
        const double factor = g_curr_look_around_factor;
        nextFactor = static_cast<float>(factor - (factor * 0.02 + 0.05));
    }
    if (!(nextFactor >= 0.0f) || !(2.0f >= nextFactor))
        nextFactor = 0.0f > nextFactor ? 0.0f : 2.0f;
    g_curr_look_around_factor = nextFactor;
    camCtrlUpdateTargetFromInputAndDistance(rotationWeight);

    const Vector3 eye = g_CameraEyePos;
    const Vector3 target = g_CameraTargetPos;
    SmoothCameraMovement(true);
    RotateAndNormalise(g_cam_target_orbit_state, GCameraPos, true, targetCameraStepPosition);
    RotateAndNormalise(g_tmp_cam_fstep_0, GCameraPos, false, g_tmp_cam_fstep_0.result);

    const Vector3 &step = targetCameraStepPosition;
    const Vector3 &anchor = GCameraPos;
    const float dx = step.x - anchor.x;
    const float dy = step.y - anchor.y;
    const float dz = step.z - anchor.z;
    const float distance = std::sqrt(dy * dy + dx * dx + dz * dz);
    const float distanceWeight = static_cast<float>(static_cast<double>(distance + 7.0f) / 10.1 * static_cast<double>(rotationWeight));
    const float ratio = distance * distanceWeight / (distance + 0.0f);
    const float keep = 1.0f - ratio;
    const float blendX = keep * step.x + anchor.x * ratio;
    const float blendY = keep * step.y + anchor.y * ratio;
    const float blendZ = keep * step.z + anchor.z * ratio;
    const float factor = g_curr_look_around_factor;
    const float mix = 1.0f >= factor ? factor : 1.0f;
    const float hold = 1.0f - mix;

    Vector3 &interpolated = g_tmp_interpolated_cam_pos;
    Vector3 &lookAt = g_tmp_cam_fstep_0.result;
    interpolated = {hold * eye.x + blendX * mix, hold * eye.y + blendY * mix, hold * eye.z + blendZ * mix};
    lookAt = {hold * target.x + lookAt.x * mix, hold * target.y + lookAt.y * mix, hold * target.z + lookAt.z * mix};
    const Vector3 blendedEye = interpolated;
    const Vector3 blendedTarget = lookAt;
    if (static_cast<double>(std::fabs(blendedEye.y - g_tmp_cam_pos.y)) > 0.3)
        interpolated.y = g_tmp_next_cam_pos.y;
    g_tmp_cam_pos = {blendedEye.x, blendedEye.y, blendedEye.z, g_tmp_cam_pos.w};
    if (static_cast<double>(std::fabs(blendedTarget.y - g_tmp_cam_rot.y)) > 0.3)
        lookAt.y = g_tmp_next_cam_rot.y;
    g_tmp_cam_rot = blendedTarget;

    if ((g_cameraStateFlags & CF_KeepRyoAlwaysVisible) != 0) {
        const float floor = static_cast<float>(static_cast<double>(mapUpdateIntersectionStateAtLocalPoint(interpolated)) + 0.55);
        if (floor > interpolated.y)
            interpolated.y = floor;
        if (Character *const ryo = CurrentRyo())
            MaintainHorizontalDistance(interpolated, GetPositionForEntity(ryo->entity.node), 0.53f);
    }
    g_tmp_next_cam_pos = {interpolated.x, interpolated.y, interpolated.z, g_tmp_next_cam_pos.w};
    g_tmp_next_cam_rot = lookAt;

    if (IsNormalOrZero(interpolated) && IsNormalOrZero(lookAt)) {
        TranslateCameraRelative(interpolated);
        if (!bLockCamera) {
            g_CameraTargetPos = lookAt;
            shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
        }
    }
    UpdateCameraDistanceDamping(GCameraSettings2.dampingOwner, 1.0f, distanceWeight);
}

} // namespace

bool ctrlGetNeedsCameraDamping() {
    const LookAroundTickResult result = srchTickLookAround();
    if (result == LOOKAROUND_ACTIVE) {
        ctrlResetCharaLookAtPositionIfNeeded(ctrlUpdateCameraLookAtVectors());
        return true;
    }
    if (result == LOOKAROUND_IDLE_OR_FINISHED && g_turnYawOffsetActive > 0) {
        ctrlResetCharaLookAtPositionIfNeeded(ctrlUpdateCameraLookAtVectors());
        return false;
    }
    Character *const ryo = CurrentRyo();
    if (g_ctrl_set_lookat) {
        if (ryo) {
            const Vector3 target{};
            SetCharaLookAtPosition(ryo, -1, &target, 0);
        }
        g_ctrl_set_lookat = 0;
    }
    return false;
}


void LogNpcVisibility() {
    g_cameraStateFlags &= ~8U;
    if (Character *const ryo = CurrentRyo()) {
        const bool wasVisible = (ryo->motionFlags & 4U) != 0;
        ryo->motionFlags |= 4U;
        if (g_bDebugLogVisibility && (ryo->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
            CHAR_LogNPCVisibility(ryo, true);
    }
}



void ApplyPendingGameState(std::int32_t overrideState) {
    std::int32_t state = g_previousAuthorState;
    if ((!g_pSCNC || !g_pSCNC->header.active) && !IsProtectedAuthorState(g_previousAuthorState) && g_temporaryGameState != 0) {
        state = g_temporaryGameState == 1 ? 0x11 : overrideState;
    }
    if (g_nAuthorState == state)
        return;

    if (g_nAuthorState == 0x11)
        LogNpcVisibility();
    nCamState |= 1U;
    g_nAuthorState = state;
    if (state == 6 || state == 8) {
        if (Character *const ryo = CurrentRyo())
            ryo->renderOpacity = 1.0f;
    }
}
void CAME_HandleRootAuthorTransition(bool vehicleInputActive) {
    if (!vehicleInputActive && g_nAuthorState == 0x11) {
        LogNpcVisibility();
        nCamState |= 1U;
        g_nAuthorState = 0;
    }
}

void SetNewAuthorGameState(std::int32_t state) {
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = state;
    ApplyPendingGameState(state);
}

void CAME_SetShake(float previous, float current, float damping) {
    if (damping > 0.0f) {
        g_cam_shake_damp = damping;
        g_cam_shake_curr = current;
        g_cam_shake_prev = previous;
    } else {
        g_cam_shake_damp = 1.0f;
        g_cam_shake_prev = 0.0f;
        g_cam_shake_curr = 0.0f;
    }
}

namespace {

alignas(4) std::array<std::byte, 4 + 8 * 28> g_scriptAuthorSequence{};

constexpr std::uint32_t ASF_USE_ZANGLE_ANIMATION = 0x20U;
constexpr std::uint32_t ASF_USE_FOV_ANIMATION = 0x40U;
constexpr std::uint32_t ASF_UNIT_COEFFICIENT_1 = 0x80U;
constexpr std::uint32_t ASF_UNIT_COEFFICIENT_2 = 0x100U;
constexpr std::uint32_t ASF_USE_OBJECT_POS_ROT_1 = 0x200U;
constexpr std::uint32_t ASF_INVERT_OBJECT_ROT_1 = 0x400U;
constexpr std::uint32_t ASF_USE_OBJECT_EULER_ROT_1 = 0x800U;
constexpr std::uint32_t ASF_USE_ALT_ROT_LOGIC_1 = 0x1000U;
constexpr std::uint32_t ASF_USE_OBJECT_POS_ROT_2 = 0x2000U;
constexpr std::uint32_t ASF_INVERT_OBJECT_ROT_2 = 0x4000U;
constexpr std::uint32_t ASF_USE_OBJECT_EULER_ROT_2 = 0x8000U;
constexpr std::uint32_t ASF_USE_ALT_ROT_LOGIC_2 = 0x10000U;
constexpr std::uint32_t ASF_SWAP_XZ_AND_ROT_Y_1 = 0x20000U;
constexpr std::uint32_t ASF_SWAP_XZ_AND_ROT_Y_2 = 0x40000U;
constexpr std::uint32_t ASF_USE_MOST_SIGNIFICANT_ROT_1 = 0x80000U;
constexpr std::uint32_t ASF_USE_MOST_SIGNIFICANT_ROT_2 = 0x100000U;
constexpr std::uint32_t ASF_USE_CURRENT_POSITION_1 = 0x200000U;
constexpr std::uint32_t ASF_USE_CURRENT_POSITION_2 = 0x400000U;
constexpr std::uint32_t ASF_USE_ZANGLE_MODIFIER = 0x800000U;
constexpr std::uint32_t ASF_USE_CURRENT_PERSPECTIVE = 0x1000000U;
constexpr std::uint32_t ASF_DISABLE_OBJECT_POS_Y_1 = 0x2000000U;
constexpr std::uint32_t ASF_DISABLE_OBJECT_POS_Y_2 = 0x4000000U;
constexpr float kSequenceFixedPerDegree = 182.04445f;

std::int32_t SequenceMotionNodeYaw(Character *character, std::uint8_t nodeId) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    return manager ? MOTN_ExtractEulerAngle(manager->motion, nodeId) : 0;
}

Vector3 SequenceCharacterPosition(Character *character, std::uint8_t nodeId, bool useEntity) {
    if (!character)
        return {};
    if (useEntity)
        return GetPositionForEntity(character->entity.node);
    Vector3 position{};
    mtwkGetCharaBonePosition(character, nodeId, &position);
    return position;
}



void ResolveSequenceAnchor(bool second, bool initialize) {
    AnimatedCameraSequence &sequence = g_animatedCameraSequence;
    const std::uint32_t flags = sequence.flags;
    Character *const character = second ? sequence.targetAnchorOwner : sequence.eyeAnchorOwner;
    Vector3 &position = second ? sequence.targetAnchorPosition : sequence.eyeAnchorPosition;
    Vector3i &rotation = second ? sequence.targetAnchorRotation : sequence.eyeAnchorRotation;
    const bool head = (flags & (second ? ASF_USE_OBJECT_EULER_ROT_2 : ASF_USE_OBJECT_EULER_ROT_1)) != 0;
    const bool entity = (flags & (second ? ASF_USE_MOST_SIGNIFICANT_ROT_2 : ASF_USE_MOST_SIGNIFICANT_ROT_1)) != 0;
    const std::uint32_t rotationFlag = second ? ASF_INVERT_OBJECT_ROT_2 : ASF_INVERT_OBJECT_ROT_1;
    const std::uint32_t keepYFlag = second ? ASF_DISABLE_OBJECT_POS_Y_2 : ASF_DISABLE_OBJECT_POS_Y_1;

    const Vector3 resolved = character ? SequenceCharacterPosition(character, head ? 5U : 0U, !head && entity) : g_tmp_seqdata_pos;
    position.x = resolved.x;
    position.z = resolved.z;
    if (initialize || (flags & keepYFlag) == 0)
        position.y = resolved.y;

    if ((flags & rotationFlag) == 0)
        return;
    if (!character) {
        rotation = {
            -g_tmp_seqdata_rot.x,
            -g_tmp_seqdata_rot.y,
            -g_tmp_seqdata_rot.z,
        };
        return;
    }

    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager) {
        rotation = GetEulerAnglesForEntity(character->entity.node);
        return;
    }

    rotation.x = 0;
    rotation.z = 0;
    if ((flags & ASF_USE_OBJECT_EULER_ROT_1) != 0) {
        rotation.y = SequenceMotionNodeYaw(character, 5U);
    } else if ((flags & ASF_USE_MOST_SIGNIFICANT_ROT_1) != 0) {
        if (initialize) {
            rotation.y = static_cast<std::int32_t>(static_cast<std::uint16_t>(manager->motion.extractedYaw) |
                (static_cast<std::uint32_t>(static_cast<std::uint16_t>(manager->motion.extractedYawAux)) << 16U));
        } else {
            rotation.y = charaGetWorldYaw(character);
        }
    } else {
        rotation.y = SequenceMotionNodeYaw(character, 0U);
    }
}

void RemapSequenceVector(Vector3 &value) {
    const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<double>(-value.y) - 90.0) * 8192.0 / 45.0));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = cos_fixed16(angle);
    const float x = value.x;
    value.x = cosine * x;
    value.y = value.z;
    value.z = sine * x;
}

Vector3 TransformSequencePoint(const Vector3 &value, const Vector3 &position, const Vector3i &rotation, bool rotate) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (rotate) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    }
    const Vector3 result{
        value.x * workTransformMatrix.rows[0].x + value.y * workTransformMatrix.rows[1].x + value.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        value.x * workTransformMatrix.rows[0].y + value.y * workTransformMatrix.rows[1].y + value.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        value.x * workTransformMatrix.rows[0].z + value.y * workTransformMatrix.rows[1].z + value.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z,
    };
    Matrix4x4::Pop();
    return result;
}

Vector3 TransformSequencePointToAnchor(const Vector3 &value, const Vector3 &position, const Vector3i &rotation, bool rotate) {
    Matrix4x4::SetActiveTransformToPrePass();
    if (rotate) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-rotation.x));
    }
    Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
    const Vector3 result{
        value.x * workTransformMatrix.rows[0].x + value.y * workTransformMatrix.rows[1].x + value.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        value.x * workTransformMatrix.rows[0].y + value.y * workTransformMatrix.rows[1].y + value.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        value.x * workTransformMatrix.rows[0].z + value.y * workTransformMatrix.rows[1].z + value.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z,
    };
    Matrix4x4::Pop();
    return result;
}

void ConvertSequencePointToPolar(Vector3 &value) {
    const float x = value.x;
    const float z = value.z;
    const std::int16_t angle = static_cast<std::int16_t>(-0x4000 - static_cast<std::int32_t>(Shenmue_Atan2Fixed(z, x)));
    value.z = value.y;
    value.x = std::sqrt(x * x + z * z);
    value.y = static_cast<float>(angle) / kSequenceFixedPerDegree;
}

void ApplySequenceFlags(Vector3 &position, Vector3 &target) {
    const AnimatedCameraSequence &sequence = g_animatedCameraSequence;
    const std::uint32_t flags = sequence.flags;
    if ((flags & ASF_SWAP_XZ_AND_ROT_Y_1) != 0)
        RemapSequenceVector(position);
    if ((flags & ASF_SWAP_XZ_AND_ROT_Y_2) != 0)
        RemapSequenceVector(target);
    if ((flags & ASF_USE_OBJECT_POS_ROT_1) != 0 && (flags & ASF_USE_ALT_ROT_LOGIC_1) == 0)
        ResolveSequenceAnchor(false, false);
    if ((flags & ASF_USE_OBJECT_POS_ROT_2) != 0 && (flags & ASF_USE_ALT_ROT_LOGIC_2) == 0)
        ResolveSequenceAnchor(true, false);
    if ((flags & ASF_USE_OBJECT_POS_ROT_1) != 0)
        position = TransformSequencePoint(position, sequence.eyeAnchorPosition, sequence.eyeAnchorRotation, (flags & ASF_INVERT_OBJECT_ROT_1) != 0);
    if ((flags & ASF_USE_OBJECT_POS_ROT_2) != 0)
        target = TransformSequencePoint(target, sequence.targetAnchorPosition, sequence.targetAnchorRotation, (flags & ASF_INVERT_OBJECT_ROT_2) != 0);
}


std::uint32_t *BindSequenceReader(FcvSplineReader &reader, std::uint32_t *cursor) {
    const std::int32_t count = static_cast<std::int32_t>(*cursor);
    reader.keyCount = count;
    reader.times = reinterpret_cast<float *>(cursor + 1);
    reader.values = reader.times + count;
    reader.slopes = reader.values + count;
    reader.keyOffset = 0;
    return reinterpret_cast<std::uint32_t *>(reader.slopes + count);
}

void AllocSequenceSegment(FcvSplineReader &reader) {
    const std::int32_t offset = reader.keyOffset;
    reader.segmentKeyCount = AllocSPLI(
        reader.coefficients, reader.keyCount - offset, reader.times + offset, reader.values + offset, reader.slopes ? reader.slopes + offset : nullptr);
}


void SeekSequenceReader(FcvSplineReader &reader, float time) {
    reader.keyOffset = 0;
    for (std::int32_t key = 0; key < reader.keyCount - 1; ++key) {
        if (reader.times[key] > time)
            break;
        if (reader.times[key] == reader.times[key + 1])
            reader.keyOffset = key + 1;
    }
    AllocSequenceSegment(reader);
}


bool EventCameraPathBlocked(const Vector3 &start, const Vector3 &end) {
    const MapControlData *const control = CurrentMapControl();
    if (!control)
        return false;
    std::array<std::uint8_t, 0x20> collisionMask{};
    std::memcpy(collisionMask.data(), control->cycleMask, collisionMask.size());
    collisionMask[1] &= 0xfcU;
    return ctrlCheckColi(start, end, 0.0f, collisionMask.data()) != 0;
}



void ReadCameraSequenceKeyframes() {
    AnimatedCameraSequence &sequence = g_animatedCameraSequence;
    const float time = static_cast<float>(static_cast<double>(CurrentFrameCount) * (1.0 / 60.0));
    if (CurrentFrameCount < 0x6fffffff)
        CurrentFrameCount += 2;

    Vector3 position{};
    Vector3 target{};
    if (g_ASEQ_busy_flag) {
        bool keysPending = false;
        float value = 0.0f;
        if ((sequence.flags & ASF_USE_ZANGLE_ANIMATION) != 0) {
            keysPending = Sequence_ReadFCVKey(&sequence.zAngleReader, time, &value) > 0;
            sequence.previousZAngle = static_cast<std::uint16_t>(static_cast<std::int32_t>(value * 8192.0f / 45.0f));
            if (!bLockCamera)
                ZANGLE = sequence.previousZAngle;
        }
        if ((sequence.flags & ASF_USE_FOV_ANIMATION) != 0) {
            if (Sequence_ReadFCVKey(&sequence.fovReader, time, &value) > 0)
                keysPending = true;
            sequence.fov = static_cast<std::uint16_t>(static_cast<std::int32_t>(value * 8192.0f / 45.0f));
            if (value > 0.0f)
                CAME_SetFieldOfView(sequence.fov);
        }
        float *const positionValues[3]{&position.x, &position.y, &position.z};
        float *const targetValues[3]{&target.x, &target.y, &target.z};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (Sequence_ReadFCVKey(&sequence.eyeReaders[axis], time, positionValues[axis]) > 0)
                keysPending = true;
            if (Sequence_ReadFCVKey(&sequence.targetReaders[axis], time, targetValues[axis]) > 0)
                keysPending = true;
        }
        g_ASEQ_busy_flag = keysPending ? 1 : 0;
        sequence.heldEye = position;
        sequence.heldTarget = target;
    } else {
        if ((sequence.flags & ASF_USE_ZANGLE_ANIMATION) != 0 && !bLockCamera)
            ZANGLE = sequence.previousZAngle;
        if ((sequence.flags & ASF_USE_FOV_ANIMATION) != 0 && sequence.fov > 0)
            CAME_SetFieldOfView(sequence.fov);
        position = sequence.heldEye;
        target = sequence.heldTarget;
    }
    ApplySequenceFlags(position, target);

    if (g_aseq_do_coli_check) {
        if (const MapControlData *const control = CurrentMapControl()) {
            std::array<std::uint8_t, 0x20> collisionMask{};
            std::memcpy(collisionMask.data(), control->cycleMask, collisionMask.size());
            collisionMask[1] &= 0xfcU;
            if (ctrlCheckColi(g_CameraEyePos, position, 0.001f, collisionMask.data()) != 0) {
                g_ASEQ_busy_flag = 0;
                return;
            }
        }
    }
    TranslateCameraRelative(position);
    if (!bLockCamera) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    g_cameraRuntime.viewReady = true;
}


void authUpdate(std::int32_t sequenceIndex, Character *eyeOwner, Character *targetOwner) {
    g_animatedCameraSequence.eyeAnchorOwner = eyeOwner;
    g_animatedCameraSequence.targetAnchorOwner = targetOwner;
    g_animatedCameraSequence.chunk = FindSequenceByChunk(sequenceIndex);
    PCCTL_ResetForEventCamera(CurrentRyo());
    StartAnimatedCamera();
}

bool finite(const Vector3 &value) {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

void SmoothCameraPoint(const Vector3 &desired, Vector3 &current, Vector3 &velocity, const Vector3 &acceleration) {
    Vector3 step{
        (desired.x - current.x) * acceleration.x,
        (desired.y - current.y) * acceleration.y,
        (desired.z - current.z) * acceleration.z,
    };
    const float magnitude = std::sqrt(step.x * step.x + step.y * step.y + step.z * step.z);
    if (magnitude < 0.00000011920929F) {
        velocity = {};
        return;
    }
    float projectedVelocity = (step.x * velocity.x + step.y * velocity.y + step.z * velocity.z) / magnitude;
    if (projectedVelocity <= 0.0F)
        projectedVelocity = 0.0F;
    const double maximumMagnitude = static_cast<double>(projectedVelocity) + 0.032;
    if (static_cast<double>(magnitude) > std::fabs(maximumMagnitude)) {
        const float scale = static_cast<float>(maximumMagnitude / static_cast<double>(magnitude));
        step.x *= scale;
        step.y *= scale;
        step.z *= scale;
    }
    current.x += step.x;
    current.y += step.y;
    current.z += step.z;
    velocity = step;
}
std::int16_t CharacterWorldYaw(const Character &character) {
    std::int16_t yaw = 0;
    for (const CharacterNode *node = &character.entity.node; node; node = node->child) {
        yaw = static_cast<std::int16_t>(yaw + static_cast<std::int16_t>(node->rotation.y));
    }
    return yaw;
}

std::int32_t g_cameraPerspectiveCopy = 10922;


Vector3 CAMERA_CalcRotation() {
    const float baseHeight = CameraYAngle;
    const float vertical = CameraHeightAbovePlayer - baseHeight;
    std::uint16_t direction = 0;
    float squared = 0.0F;
    if (std::fabs(vertical) >= 0.00000011920929F) {
        const float distance = CameraDistanceFromPlayer;
        direction = atan2_fixed16(vertical, distance);
        squared = distance * distance + vertical * vertical;
    } else {
        squared = CameraDistanceFromPlayer * CameraDistanceFromPlayer;
    }
    const float length = std::sqrt(squared);
    float ratio = 0.0F;
    if (std::fabs(length) >= 0.00000011920929F) {
        std::int32_t fov = currentPerspectiveFov;
        if (fov)
            g_cameraPerspectiveCopy = fov;
        else
            fov = g_cameraPerspectiveCopy;
        ratio = length * (std::tan(0.52356684F) / std::tan(static_cast<float>(static_cast<std::uint16_t>(fov / 2)) * 6.2831855F * 0.000015258789F));
    }
    const float sine = sin_fixed16(static_cast<std::int16_t>(direction));
    const std::uint16_t folded = direction <= 0x8000U ? direction : static_cast<std::uint16_t>(-direction);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    if (std::fabs(ratio) >= 0.00000011920929F)
        return {0.0F, sine * ratio + baseHeight, cosine * ratio};
    return {0.0F, baseHeight, 0.0F};
}

void MaintainHorizontalDistance(Vector3 &point, const Vector3 &origin, float distance) {
    float x = point.x - origin.x;
    float z = point.z - origin.z;
    const float magnitude = std::sqrt(x * x + z * z);
    if (distance <= magnitude)
        return;
    if (magnitude < 0.001F) {
        x = 0.0F;
        z = 0.0F;
    } else {
        const float scale = 1.0F / magnitude;
        x *= scale;
        z *= scale;
    }
    point.x = origin.x + x * distance;
    point.z = origin.z + z * distance;
}



float CAMERA_Related_10(const Vector3 &origin, const Vector3 &direction, float radius) {
    const MapControlData *const control = CurrentMapControl();
    std::array<std::uint8_t, sizeof(control->cycleMask)> mask{};
    std::memcpy(mask.data(), control->cycleMask, mask.size());
    mask[1] &= 0xfcU;
    const Vector3 end{origin.x + direction.x * 5.0f, origin.y, origin.z + direction.z * 5.0f};
    CharacterVectorCollisionResult result{};
    COLS_TraceMapVectorWithMask(origin, end, radius, result, mask.data());
    std::uint32_t hitKind = result.flags & 7U;
    coliTestCharacterShapes(10, origin, end, radius, result, hitKind);
    coliTestAuxiliaryShapes(10, origin, end, radius, result, hitKind);
    if (hitKind != 1U)
        return hitKind == 2U ? 0.0f : 5.0f;
    if (result.fraction < 0.0f)
        return 5.0f;
    const float distance = result.fraction * 5.0f;
    return distance < 5.0f ? distance : 5.0f;
}





void charaComputeCameraPositionRelativeToPlayer(Character &chara, Vector3 &position, Vector3 &lookAt) {
    lookAt = GetPositionForEntity(chara.entity.node);
    const Vector3 offset = CAMERA_CalcRotation();
    const bool flipped = g_cameraOverrideFlipDirectionActive;
    if (flipped) {
        Vector3 root{};
        charaGetPosForNode(&chara, 0, &root);
        lookAt.x = root.x;
        lookAt.z = root.z;
    }
    const std::uint16_t facing = static_cast<std::uint16_t>(playerFacingAngle);
    position = RotateAroundYAndTransformWorldCoords(offset, lookAt, facing);
    if (!flipped) {
        if (g_CameraCollisionEnabled) {
            Matrix4x4_TranslateAndRotate(*reinterpret_cast<const CharacterIdTransform *>(&chara.entity.node));
            CharacterNode *const parent = chara.entity.node.child;
            const Vector3 local = parent ? WorldToEntityLocalPoint(*parent, position) : position;
            Vector3 origin = parent ? WorldToEntityLocalPoint(*parent, lookAt) : lookAt;
            const float distance = CameraDistanceFromPlayer;
            Vector3 direction{local.x - origin.x, 0.0f, local.z - origin.z};
            float probed = distance;
            if (direction.z * direction.z + direction.x * direction.x != 0.0f) {

                const float magnitude = std::sqrt((direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z);
                if (magnitude > 0.0f) {
                    const float inverse = 1.0f / magnitude;
                    direction = {direction.x * inverse, direction.y * inverse, direction.z * inverse};
                }
                probed = CAMERA_Related_10(origin, direction, 0.16f);
            }
            const float allowed = static_cast<float>(static_cast<double>(probed) + 0.07);
            float maximum = distance < allowed ? distance : allowed;
            if (maximum > g_fCurrMaxDistance) {
                float step = static_cast<float>(static_cast<double>(maximum - g_fCurrMaxDistance) * 0.02);
                if (static_cast<double>(step) > 0.3)
                    step = 0.30000001F;
                maximum = step + g_fCurrMaxDistance;
            }
            g_fCurrMaxDistance = maximum;
            if (distance > maximum) {
                const float retracted = distance - maximum;
                position = {maximum * direction.x + origin.x, local.y, maximum * direction.z + origin.z};
                origin.x = origin.x - retracted * direction.x;
                origin.z = origin.z - retracted * direction.z;
                if (parent)
                    position = TransformPointByEntity(*parent, position);
                lookAt = parent ? TransformPointByEntity(*parent, origin) : origin;
            }
        }
        const Vector3 rear = RotateAroundYAndTransformWorldCoords(CAMERA_CalcRotation(), lookAt, static_cast<std::uint16_t>(facing + 0x8000U));
        const float rearX = rear.x - lookAt.x;
        const float rearZ = rear.z - lookAt.z;
        Vector3 ahead{};
        if (!(0.01 > static_cast<double>(rearX * rearX + rearZ * rearZ))) {
            const float scale = MaxCameraDistanceOffset / std::sqrt((rearX * rearX + 0.0f) + rearZ * rearZ);
            ahead = {scale * rearX, scale * 0.0f, scale * rearZ};
        }
        lookAt = {lookAt.x + ahead.x, ahead.y + lookAt.y, lookAt.z + ahead.z};
    }
    lookAt.y = CameraYAngle + lookAt.y;
    if (g_EnableCameraHeightClamp) {
        g_CameraElevationSmoothing = 0.0F;
        return;
    }

    const Vector3 ryo = GetPositionForEntity(chara.entity.node);
    const float dx = position.x - ryo.x;
    const float dz = position.z - ryo.z;
    const float horizontal = std::sqrt(dx * dx + dz * dz);
    float smoothing = 0.0F;
    if (1.5F > horizontal) {
        smoothing = static_cast<float>(static_cast<double>(g_CameraElevationSmoothing) + 0.1);
        if (smoothing > 1.0F)
            smoothing = 1.0F;
    } else {
        smoothing = static_cast<float>(static_cast<double>(g_CameraElevationSmoothing) - 0.1);
        if (0.0F > smoothing)
            smoothing = 0.0F;
    }
    g_CameraElevationSmoothing = smoothing;
    float lift = 1.5F - horizontal < 0.60000002F ? 1.5F - horizontal : 0.60000002F;
    const float headroom = ryo.y + 1.95F - position.y;
    if (!(headroom > lift))
        lift = headroom;
    const float rise = lift > 0.0F ? lift : 0.0F;
    position.y = static_cast<float>(static_cast<double>(rise) * 1.01 * static_cast<double>(smoothing) + static_cast<double>(position.y));
    lookAt.y = rise * 0.40000001F * smoothing + lookAt.y;
    if (ryo.y >= position.y)
        position.y = ryo.y;
    if (ryo.y >= lookAt.y)
        lookAt.y = ryo.y;
    if (CheckIsPointIntersectWith(position, nullptr, REGION_CAM3.name)) {
        const float ground = static_cast<float>(static_cast<double>(mapUpdateIntersectionStateAtLocalPoint(position)) + 0.4);
        if (ground > position.y)
            position.y = ground;
    }
}

void ComputeCameraYaw(Character &ryo, bool smooth) {
    std::int16_t current = 0;
    if (g_CameraIsTransitioning && ShouldRun() && g_pControlPadController && g_pControlPadController->controlMode == 2) {
        current = CurrentRyo() ? static_cast<std::int16_t>(g_smoothedAnimationYaw) : 0;
        if (!CurrentRyo())
            g_smoothedAnimationYaw = 0;
    } else if (g_ryoCurrentAnimationState == 4 || g_ryoCurrentAnimationState == 10) {
        current = static_cast<std::int16_t>(charaGetWorldYaw(&ryo));
    } else if (g_cameraOverrideFlipDirectionActive) {
        std::int16_t follow = static_cast<std::int16_t>(g_motnCameraFollowAngle << 4);
        follow = follow > 0x1000 ? static_cast<std::int16_t>(0x1000) : follow < -0x1000 ? static_cast<std::int16_t>(-0x1000) : follow;
        const std::int32_t lag = g_tmp_camera_ang_y;
        g_tmp_camera_ang_y = static_cast<std::int16_t>(lag + (follow - lag) / 6);
        current = static_cast<std::int16_t>(g_fbggRideHeading - g_tmp_camera_ang_y);
    } else {
        current = CharacterWorldYaw(ryo);
    }
    if (!smooth) {
        playerFacingAngle = current;
        return;
    }
    if (g_cameraOverrideFlipDirectionActive) {
        const std::int16_t delta = static_cast<std::int16_t>(current - static_cast<std::int16_t>(playerFacingAngle));
        playerFacingAngle += static_cast<std::int16_t>(delta / 6);
        return;
    }
    std::int32_t maximumStep = 2048;
    if (g_CameraFadeInterpFrameCount > 20) {
        maximumStep = 0;
    } else if (g_CameraFadeInterpFrameCount != 0) {
        maximumStep = static_cast<std::int32_t>(static_cast<float>(20 - g_CameraFadeInterpFrameCount) / 20.0F * 2048.0F);
    }
    const std::int16_t delta = static_cast<std::int16_t>(current - static_cast<std::int16_t>(playerFacingAngle));
    const std::int32_t step = std::clamp(static_cast<std::int32_t>(delta), -maximumStep, maximumStep);
    playerFacingAngle = static_cast<std::int16_t>(playerFacingAngle + step);
}

bool CameraSegmentBlocked(const Vector3 &start, const Vector3 &end, float radius, const std::uint8_t *mask) {
    return ctrlCheckColi(start, end, radius, mask) != 0;
}

void ResolveCameraEyeCollision(Vector3 &position, const Vector3 &previousEye) {
    if (g_cameraOverrideFlipDirectionActive || !g_CameraCollisionEnabled || (g_cameraStateFlags & CF_CameraOverrideActive) != 0)
        return;
    const MapControlData *const control = CurrentMapControl();
    if (!control)
        return;
    std::array<std::uint8_t, sizeof(control->cycleMask)> mask{};
    std::memcpy(mask.data(), control->cycleMask, mask.size());
    mask[1] &= 0xfcU;
    constexpr float radius = 0.15000001F;
    if (!CameraSegmentBlocked(position, previousEye, radius, mask.data()))
        return;

    Vector3 direction{position.x - previousEye.x, position.y - previousEye.y, position.z - previousEye.z};
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length > 0.0F) {
        const float inverse = static_cast<float>(1.0 / static_cast<double>(length));
        direction.x *= inverse;
        direction.y *= inverse;
        direction.z *= inverse;
    }
    const double halfLength = static_cast<double>(length) * 0.5;
    const float zStep = static_cast<float>(static_cast<double>(direction.z) * halfLength);
    const float xStep = static_cast<float>(static_cast<double>(direction.x) * halfLength);
    Vector3 pointB{};
    Vector3 pointC{};
    bool foundRoute = false;

    pointB = {previousEye.x - zStep + xStep, previousEye.y, previousEye.z + xStep + zStep};
    foundRoute = !CameraSegmentBlocked(position, pointB, radius, mask.data()) && !CameraSegmentBlocked(pointB, previousEye, radius, mask.data());

    if (!foundRoute) {
        pointB = {previousEye.x + zStep + xStep, previousEye.y, previousEye.z - xStep + zStep};
        foundRoute = !CameraSegmentBlocked(position, pointB, radius, mask.data()) && !CameraSegmentBlocked(pointB, previousEye, radius, mask.data());
    }

    if (!foundRoute) {
        pointB = {previousEye.x - zStep, previousEye.y, previousEye.z + xStep};
        pointC = {previousEye.x - zStep + xStep + xStep, previousEye.y, previousEye.z + xStep + zStep + zStep};
        foundRoute = !CameraSegmentBlocked(position, pointB, radius, mask.data()) && !CameraSegmentBlocked(pointB, pointC, radius, mask.data()) &&
            !CameraSegmentBlocked(pointC, previousEye, radius, mask.data());
    }

    if (!foundRoute) {
        pointB = {previousEye.x + zStep, previousEye.y, previousEye.z - xStep};
        pointC = {previousEye.x + zStep + xStep + xStep, previousEye.y, previousEye.z - xStep + zStep + zStep};
        foundRoute = !CameraSegmentBlocked(position, pointB, radius, mask.data()) && !CameraSegmentBlocked(pointB, pointC, radius, mask.data()) &&
            !CameraSegmentBlocked(pointC, previousEye, radius, mask.data());
    }
    if (!foundRoute)
        return;

    constexpr float blend = 0.34999999F;
    if (std::fabs(pointB.x - position.x) >= std::numeric_limits<float>::epsilon()) {
        position.x += (pointB.x - position.x) * blend;
    }
    if (std::fabs(pointB.z - position.z) >= std::numeric_limits<float>::epsilon()) {
        position.z += (pointB.z - position.z) * blend;
    }
}




void UpdateCam1(const Vector3 &playerPosition) {
    if (!CheckIsPointIntersectWith(playerPosition, nullptr, SHEN_NAME(CAM1))) {
        g_cam1_flag = 0;
        g_cam1_intersect_status = 0;
    } else {
        g_cam1_flag = 1;
        if (g_cam1_intersect_status != 1) {
            const std::int16_t relativeYaw = static_cast<std::int16_t>(g_fCamAccumYaw + playerFacingAngle);
            if (g_cam1_intersect_status != 0 || (std::abs(static_cast<std::int32_t>(relativeYaw)) <= 12288 && g_cam_frm_counter <= 0)) {
                if (!g_bNegativeCamBlend)
                    g_CameraBlendSpeed = -0.125F;
                g_bNegativeCamBlend = 1;
                return;
            }
            g_cam1_intersect_status = 1;
        }
    }
    if (g_bNegativeCamBlend)
        g_CameraBlendSpeed = 0.125F;
    g_bNegativeCamBlend = 0;
}

void ryoApplyCameraSmoothingAndCollisionTrans() {
    Character *const ryo = CurrentRyo();
    if (!ryo)
        return;

    const Vector3 playerPosition = GetPositionForEntity(ryo->entity.node);
    ComputeCameraYaw(*ryo, true);
    MaxCameraDistanceOffset = std::max((distanceFromRyoForCamera - MaxCameraDistanceOffset) * 0.1F + MaxCameraDistanceOffset, 0.001F);
    if (ryo->renderOpacity < 1.0F) {
        ryo->renderOpacity += (1.0F - ryo->renderOpacity) * 0.090000004F;
        if (ryo->renderOpacity > 0.98F)
            ryo->renderOpacity = 1.0F;
    }

    const bool playerOwnsFrame = ShouldRun();
    float deltaTime = 0.07F;
    if ((playerOwnsFrame && g_pControlPadController->controlMode == 2) || g_cam_frm_counter > 0) {
        if (g_numFramesActive > 0) {
            --g_numFramesActive;
            deltaTime = static_cast<float>(static_cast<double>(static_cast<float>(g_numFramesActive) * g_cameraDeltaTimeRampStep) + 0.07);
        }
        if (g_bEnableFrameTimeSmoothing)
            deltaTime = deltaTime * kSeqFramesPerSecond * fFrameTimeMs;
    } else if (g_bEnableFrameTimeSmoothing) {
        deltaTime = fFrameTimeMs * 2.1F;
    }
    if (g_cam_frm_counter > 0)
        --g_cam_frm_counter;
    if (playerOwnsFrame && g_pControlPadController->controlMode == 2) {
        const std::int16_t fovDelta = static_cast<std::int16_t>(FieldOfView - currentPerspectiveFov);
        const double relaxedFov = static_cast<double>(fovDelta) * 0.075 + static_cast<double>(currentPerspectiveFov);
        CAME_SetFieldOfView(static_cast<std::int16_t>(static_cast<std::int32_t>(relaxedFov)));
    }

    CameraHeightAbovePlayer = CameraHeightAbovePlayer_MAX;
    Vector3 desiredEye{};
    Vector3 desiredTarget{};
    charaComputeCameraPositionRelativeToPlayer(*ryo, desiredEye, desiredTarget);

    Vector3 targetToEye{
        desiredEye.x - desiredTarget.x,
        desiredEye.y - desiredTarget.y,
        desiredEye.z - desiredTarget.z,
    };
    float targetToEyeDistance = std::sqrt(targetToEye.x * targetToEye.x + targetToEye.y * targetToEye.y + targetToEye.z * targetToEye.z);
    if (targetToEyeDistance > 0.0F) {
        const float inverse = 1.0F / targetToEyeDistance;
        targetToEye.x *= inverse;
        targetToEye.y *= inverse;
        targetToEye.z *= inverse;
    }
    targetToEyeDistance = std::min(targetToEyeDistance, CameraDistanceFromPlayer + MaxCameraDistanceOffset);
    desiredEye = {
        desiredTarget.x + targetToEye.x * targetToEyeDistance,
        desiredTarget.y + targetToEye.y * targetToEyeDistance,
        desiredTarget.z + targetToEye.z * targetToEyeDistance,
    };

    if (g_CameraRetractionLimit != 0.0F)
        MaintainHorizontalDistance(desiredEye, playerPosition, g_CameraRetractionLimit);
    const float analog = useAnalogInput ? static_cast<float>(rawAnalogInputValue) / 245.0F : 0.0F;
    if (analog < 0.5F)
        MaintainHorizontalDistance(desiredEye, playerPosition, 0.38F);
    ResolveCameraEyeCollision(desiredEye, g_CameraEyePos);

    Vector3 nextEye = g_CameraEyePos;
    Vector3 nextTarget = g_CameraTargetPos;
    UpdateCam1(playerPosition);
    float interpolation = deltaTime * g_CameraBlendProgress;
    if (g_CameraFadeInterpFrameCount != 0) {
        const std::int32_t remaining = --g_CameraFadeInterpFrameCount;
        if (remaining > 10) {
            interpolation = static_cast<float>(30 - remaining) * interpolation * 0.05F;
        }
    } else {
        g_bCameraSkipFirstInterp = 0;
    }

    if (g_EnableCameraHeightClamp) {
        if (g_CameraInterpolationTimer > 0)
            --g_CameraInterpolationTimer;
        const float horizontalAcceleration = static_cast<float>(30 - g_CameraInterpolationTimer) / 30.0F * 0.5F * g_CameraZoomFOVModifier;
        const Vector3 acceleration{horizontalAcceleration, interpolation * 1.8F, horizontalAcceleration};
        SmoothCameraPoint(desiredEye, nextEye, g_tmp_velocity_vec_0, acceleration);
        SmoothCameraPoint(desiredTarget, nextTarget, g_tmp_velocity_vec, acceleration);
        if (g_ryoVehicleAlphaSteps > 12)
            ryo->renderOpacity = static_cast<float>(static_cast<double>(static_cast<float>(20 - g_ryoVehicleAlphaSteps)) * 0.125);
    } else {
        if (g_cameraOverrideFlipDirectionActive) {
            const float vertical = std::max(interpolation * 5.4F, 1.0F);
            nextEye.x += (desiredEye.x - nextEye.x) * 0.80000001F;
            nextEye.y += (desiredEye.y - nextEye.y) * vertical;
            nextEye.z += (desiredEye.z - nextEye.z) * 0.80000001F;
            nextTarget.x += (desiredTarget.x - nextTarget.x) * 0.80000001F;
            nextTarget.y += (desiredTarget.y - nextTarget.y) * vertical;
            nextTarget.z += (desiredTarget.z - nextTarget.z) * 0.80000001F;
        } else {
            const float accelerationValue = interpolation * 1.8F;
            const Vector3 acceleration{accelerationValue, accelerationValue, accelerationValue};
            SmoothCameraPoint(desiredEye, nextEye, g_tmp_velocity_vec_0, acceleration);
            SmoothCameraPoint(desiredTarget, nextTarget, g_tmp_velocity_vec, acceleration);
        }

        const float x = playerPosition.x - nextEye.x;
        const float z = playerPosition.z - nextEye.z;
        const float distanceSquared = x * x + z * z;
        if (distanceSquared < 0.032400001F) {
            if (distanceSquared < 0.016899999F) {
                ryo->renderOpacity = 0.0F;
            } else {
                ryo->renderOpacity = (std::sqrt(distanceSquared) - 0.13F) * 20.0F;
            }
        }
    }

    g_followCameraDesiredEye = desiredEye;
    g_CameraLookDirection = desiredTarget;
    TranslateCameraRelative(nextEye);
    if (!bLockCamera) {
        g_CameraTargetPos = nextTarget;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    g_CameraBlendProgress = std::clamp(g_CameraBlendProgress + g_CameraBlendSpeed, 0.0F, 1.0F);
    g_CameraIsTransitioning = playerOwnsFrame && g_pControlPadController && g_pControlPadController->controlMode == 2;
    g_CameraOverrideActiveFlag = 0;
    g_cameraRuntime.viewReady = true;
}

void UpdateThirdPersonCamera() {
    g_CameraElevationSmoothing = 1.0F;
    g_CameraRetractionLimit = 0.0F;
    g_CameraBlendProgress = 1.0F;
    g_CameraBlendSpeed = 1.0F;

    Character *const ryo = CurrentRyo();
    if (!ryo)
        return;
    g_fCurrMaxDistance = 1000.0F;
    if (g_bCameraSkipFirstInterp) {
        g_bCameraSkipFirstInterp = 0;
    } else {
        ComputeCameraYaw(*ryo, false);
    }
    const Vector3 ryoPosition = GetPositionForEntity(ryo->entity.node);
    UpdateCam1(ryoPosition);
    distanceFromRyoForCamera = 0.80000001F;
    if (!bLockCamera)
        ZANGLE = 0;
    convUpdateCharaSeqVisibility();

    if (!g_bIsConversationCamInactive) {
        // The conversation camera ended with cam_mode set: settle the default camera, then ease the eye a fifth of the way toward Ryo.
        if (g_convCamMode) {
            g_convCamMode = 0;
            Vector3 cameraPosition{};
            Vector3 cameraTarget{};
            charaComputeCameraPositionRelativeToPlayer(*ryo, cameraPosition, cameraTarget);
            TranslateCameraRelative(cameraPosition);
            if (!bLockCamera) {
                g_CameraTargetPos = cameraTarget;
                shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
            }
            for (std::int32_t iteration = 0; iteration < 40; ++iteration)
                ryoApplyCameraSmoothingAndCollisionTrans();
            // A component within FLT_EPSILON stays put; a NaN difference still moves.
            const auto ease = [](float from, float to) {
                const float delta = to - from;
                return 0.00000011920929F > std::fabs(delta) ? from : delta * 0.2F + from;
            };
            const Vector3 settledTarget = g_CameraTargetPos;
            const Vector3 settledEye = g_CameraEyePos;
            TranslateCameraRelative({ease(settledEye.x, ryoPosition.x), ease(settledEye.y, ryoPosition.y), ease(settledEye.z, ryoPosition.z)});
            if (!bLockCamera) {
                g_CameraTargetPos = {
                    ease(settledTarget.x, g_CameraTargetPos.x), ease(settledTarget.y, g_CameraTargetPos.y), ease(settledTarget.z, g_CameraTargetPos.z)};
                shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
            }
            CAME_SetFieldOfView(FieldOfView);
            g_cameraRuntime.viewReady = true;
        }
        g_bIsConversationCamInactive = 1;
        return;
    }

    Vector3 eye{};
    Vector3 target{};
    ryoUpdateTurnMode(ryo);
    charaComputeCameraPositionRelativeToPlayer(*ryo, eye, target);
    TranslateCameraRelative(eye);
    if (!bLockCamera) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    for (std::int32_t iteration = 0; iteration < 40; ++iteration)
        ryoApplyCameraSmoothingAndCollisionTrans();
    CAME_SetFieldOfView(FieldOfView);
    g_cameraRuntime.viewReady = true;
}

void StepTowardTargetSmoothly(const Vector3 &target, Vector3 &current, Vector3 &step) {
    const Vector3 start = current;
    float stepX = static_cast<float>(static_cast<double>(target.x - start.x) / 5.0);
    float stepY = static_cast<float>(static_cast<double>(target.y - start.y) / 5.0);
    float stepZ = static_cast<float>(static_cast<double>(target.z - start.z) / 5.0);
    const float magnitude = std::sqrt(stepY * stepY + stepX * stepX + stepZ * stepZ);
    if (std::fabs(magnitude) < 0.00000011920929f) {
        step = {};
        return;
    }
    float dot = (stepY * step.y + stepX * step.x + stepZ * step.z) / magnitude;
    if ((dot > 0.0f && magnitude <= 0.0f) || (dot <= 0.0f && magnitude > 0.0f))
        dot = 0.0f;
    if (static_cast<double>(std::fabs(magnitude)) > std::fabs(static_cast<double>(dot) + 0.032)) {
        const auto ratio = static_cast<float>((static_cast<double>(dot) + 0.032) / magnitude);
        stepX *= ratio;
        stepY *= ratio;
        stepZ *= ratio;
    }
    current = {start.x + stepX, start.y + stepY, start.z + stepZ};
    step = {stepX, stepY, stepZ};
}


float CombatCameraLagClamp(float requested, float current, double lag) {
    double value = static_cast<double>(requested) - lag;
    if (value <= current)
        value = std::fmin(static_cast<double>(requested) + lag, static_cast<double>(current));
    return static_cast<float>(value);
}

// The combat camera's 2048 deadband on a yaw difference.
std::int16_t CombatCameraYawDeadband(std::int16_t delta) {
    if (delta > 2048)
        return static_cast<std::int16_t>(delta - 2048);
    return delta < -2048 ? static_cast<std::int16_t>(delta + 2048) : 0;
}

// The eye rotated about the pivot by a yaw through the pre-pass matrix.
Vector3 RotateCombatCameraEye(const Vector3 &pivot, std::int16_t yaw, const Vector3 &eye) {
    const Vector3 relative{eye.x - pivot.x, eye.y - pivot.y, eye.z - pivot.z};
    Vector3 rotated{};
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(pivot.x, pivot.y, pivot.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::TransformPoint(relative, rotated);
    Matrix4x4::Pop(1);
    return rotated;
}




void UpdateCombatCamera() {
    constexpr float kEpsilon = 0.00000011920929f;
    constexpr std::uint32_t kCameraFocusTarget = 0x20000000U;
    const VFMAData *const game = vfMainGetCurrent();
    if (!game || game->playerSlot == -1)
        return;
    const auto playerSlot = static_cast<std::size_t>(game->playerSlot);
    Character *const playerCharacter = game->characters[playerSlot];
    if (!playerCharacter || ((game->slotTeamId[playerSlot] - 1U) & 0xfdU) == 0)
        return;

    const VFActor *const fighter = vfFindFighterFor(playerCharacter);
    const Vector3 eyeAtEntry = g_combatCameraEye;
    const Vector3 fighterPosition{fighter->contactCurrentPosition.x, fighter->position.y, fighter->contactCurrentPosition.z};
    const VFActor *const other = fighter->targetActor;
    const bool focus = (fighter->hitStateFlags2 & kCameraFocusTarget) != 0;
    const bool specialBattle = (static_cast<std::uint8_t>(game->battleType - 1U) & 0xfbU) == 0;
    const bool seventyMan = specialBattle && g_areaId == SHEN_NAME(MFBT);
    g_combat_cam_focus_blend_frms = std::clamp(g_combat_cam_focus_blend_frms + (focus ? 1 : -1), 0, 64);

    float weightTotal = 1.0f;
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
    float sumDistance = 0.0f;
    float closeOpponents = 0.0f;
    for (std::size_t slot = 0; slot < kBattleSlots; ++slot) {
        if (!game->characters[slot] || slot == playerSlot)
            continue;
        const VFActor *const actor = vfFindFighterFor(game->characters[slot]);
        const float dx = actor->contactCurrentPosition.x - fighterPosition.x;
        const float dz = actor->contactCurrentPosition.z - fighterPosition.z;
        const float dy = actor->position.y - fighterPosition.y;
        const float height = std::fabs(dy);
        const float squared = dz * dz + dx * dx;
        if (squared >= 25.0f || height >= 4.0f)
            continue;
        float weight = 1.0f;
        const float distance = std::sqrt(squared);
        if (distance > 2.5f)
            weight = static_cast<float>((5.0 - distance) / 2.5);
        if (height > 2.0f)
            weight = static_cast<float>((4.0 - height) * 0.5 * weight);
        const bool opponent = ((game->slotTeamId[slot] - 1U) & 0xfdU) == 0;
        if (!opponent)
            weight = static_cast<float>(weight * 0.5);
        weightTotal += weight;
        sumX += weight * dx * weight;
        sumY += weight * dy * weight;
        sumZ += weight * dz * weight;
        sumDistance += weight * distance * weight;
        if (distance < 3.5f && opponent)
            closeOpponents += 1.0f;
    }

    const bool weighted = std::fabs(weightTotal) >= kEpsilon;
    Vector3 centroid = fighterPosition;
    if (weighted)
        centroid = {sumX / weightTotal + fighter->contactCurrentPosition.x, sumY / weightTotal + fighterPosition.y, sumZ / weightTotal + fighterPosition.z};
    float offsetX = centroid.x - fighter->contactCurrentPosition.x;
    float offsetY = centroid.y - fighterPosition.y;
    float offsetZ = centroid.z - fighterPosition.z;
    if (specialBattle) {
        offsetX = static_cast<float>(offsetX * 0.6);
        offsetY = static_cast<float>(offsetY * 0.6);
        offsetZ = static_cast<float>(offsetZ * 0.6);
    }
    if (closeOpponents != 0.0f && !focus)
        g_combat_cam_range_bias_frms = std::max(g_combat_cam_range_bias_frms - 1, 0);
    else
        g_combat_cam_range_bias_frms = std::min(g_combat_cam_range_bias_frms + 1, 64);

    const auto blend = static_cast<float>(static_cast<double>(g_combat_cam_focus_blend_frms) * 0.015625);
    const auto facing = static_cast<std::uint16_t>(fighter->temporaryFacingAngle);
    const float facingX = -sin_fixed16(static_cast<std::int16_t>(facing));
    const float facingZ = -cos_fixed16(facing);
    const double groupBlend = 1.0 - blend;
    const auto requestedY = static_cast<float>(offsetY * groupBlend);
    const auto requestedX = static_cast<float>(offsetX * groupBlend + facingX * blend);
    const auto requestedZ = static_cast<float>(offsetZ * groupBlend + facingZ * blend);
    const double rangeBias = static_cast<double>(g_combat_cam_range_bias_frms) * 0.015625;
    const auto distance =
        static_cast<float>(weighted ? static_cast<float>(sumDistance / weightTotal) * 0.75 * groupBlend + 2.1768 + rangeBias : rangeBias + 2.1768);

    Vector3 &smooth = g_combat_cam_offset_smooth;
    const float deltaX = requestedX - smooth.x;
    const float deltaY = requestedY - smooth.y;
    const float deltaZ = requestedZ - smooth.z;
    if (std::fabs(deltaX) >= kEpsilon)
        smooth.x += deltaX * 0.33333334f;
    if (std::fabs(deltaY) >= kEpsilon)
        smooth.y += deltaY * 0.33333334f;
    if (std::fabs(deltaZ) >= kEpsilon)
        smooth.z += deltaZ * 0.33333334f;
    const float requestedLookX = smooth.x + fighter->contactCurrentPosition.x;
    const float requestedLookZ = smooth.z + fighter->contactCurrentPosition.z;
    const auto requestedLookY = static_cast<float>((smooth.y + fighter->position.y) + 0.84);
    const Vector3 &lookAtState = g_combatCameraTarget;
    Vector3 lookAt{CombatCameraLagClamp(requestedLookX, lookAtState.x, 0.05), CombatCameraLagClamp(requestedLookY, lookAtState.y, 0.1),
        CombatCameraLagClamp(requestedLookZ, lookAtState.z, 0.05)};


    const MapControlData *const control = CurrentMapControl();
    std::array<std::uint8_t, sizeof(control->cycleMask)> mask{};
    std::memcpy(mask.data(), control->cycleMask, mask.size());
    mask[1] &= 0xfcU;
    Vector3 hit{};
    if (coliTestPlane(fighterPosition, lookAt, 0.4f, hit, mask.data()) == 1) {
        lookAt.x = hit.x;
        lookAt.z = hit.z;
    }
    if (closeOpponents != 1.0f || focus)
        g_combat_cam_duel_low_angle_frms = std::max(g_combat_cam_duel_low_angle_frms - 1, 0);
    else
        g_combat_cam_duel_low_angle_frms = std::min(g_combat_cam_duel_low_angle_frms + 1, 64);

    const Vector3 &eyeState = g_combatCameraEye;
    float directionX = eyeState.x - lookAtState.x;
    float directionZ = eyeState.z - lookAtState.z;
    if (focus) {
        const float length = std::sqrt(directionZ * directionZ + directionX * directionX);
        float biasX = 1.0f;
        float biasZ = 0.0f;
        if (std::fabs(length) >= kEpsilon) {
            biasX = directionX / length;
            biasZ = directionZ / length;
        }
        directionX = static_cast<float>(biasX * 3.2652 + lookAtState.x) - lookAt.x;
        directionZ = static_cast<float>(biasZ * 3.2652 + lookAtState.z) - lookAt.z;
    }
    const float directionLength = std::sqrt(directionZ * directionZ + directionX * directionX);
    float unitX = 1.0f;
    float unitZ = 0.0f;
    if (std::fabs(directionLength) >= kEpsilon) {
        unitX = directionX / directionLength;
        unitZ = directionZ / directionLength;
    }
    const float requestedEyeX = distance * unitX + lookAt.x;
    const float requestedEyeZ = distance * unitZ + lookAt.z;
    const auto lowAngle = static_cast<double>(g_combat_cam_duel_low_angle_frms) * 0.015625 * 0.8;
    const auto requestedEyeY = static_cast<float>(std::fmax(static_cast<float>(lookAt.y + 0.44 - lowAngle), static_cast<double>(fighter->position.y) + 0.15));
    Vector3 eye{CombatCameraLagClamp(requestedEyeX, eyeAtEntry.x, 0.05), CombatCameraLagClamp(requestedEyeY, eyeAtEntry.y, 0.1),
        CombatCameraLagClamp(requestedEyeZ, eyeAtEntry.z, 0.05)};

    if (!seventyMan || focus) {
        if (other && !focus) {
            const float ox = other->contactCurrentPosition.x - fighter->contactCurrentPosition.x;
            const float oy = other->contactCurrentPosition.y - fighter->contactCurrentPosition.y;
            const float oz = other->contactCurrentPosition.z - fighter->contactCurrentPosition.z;
            if (oy * oy + ox * ox + oz * oz < 6.25f) {
                auto relock = static_cast<std::uint16_t>(Shenmue_Atan2Fixed(fighter->contactCurrentPosition.x - other->contactCurrentPosition.x,
                                                             fighter->contactCurrentPosition.z - other->contactCurrentPosition.z) +
                    0x4000);
                const auto current = static_cast<std::int16_t>(Shenmue_Atan2Fixed(lookAt.x - eye.x, lookAt.z - eye.z));
                if (std::abs(static_cast<std::int16_t>(relock) - current) > 0x4000)
                    relock = static_cast<std::uint16_t>(relock + 0x8000);
                const auto side = [&relock, distance, &eye](float x, float z) {
                    return Vector3{x - sin_fixed16(static_cast<std::int16_t>(relock)) * distance, eye.y, z - cos_fixed16(relock) * distance};
                };
                Vector3 blocked{};
                if (coliTestPlane(lookAt, side(lookAt.x, lookAt.z), 0.4f, blocked, mask.data()) == 1) {
                    relock = static_cast<std::uint16_t>(relock + 0x8000);
                    if (coliTestPlane(lookAt, side(lookAt.x, lookAt.z), 0.4f, blocked, mask.data()) == 1)
                        relock = static_cast<std::uint16_t>(current);
                }
                const auto yaw = static_cast<std::int16_t>(std::clamp(CombatCameraYawDeadband(static_cast<std::int16_t>(relock - current)) / 4, -1152, 1152));
                const Vector3 rotated = RotateCombatCameraEye(lookAt, yaw, eye);
                if (coliTestPlane(fighterPosition, rotated, 0.4f, blocked, mask.data()) == 0)
                    eye = rotated;
            }
        }
    } else {
        float anchorX = 38.0f;
        float anchorZ = 9.1499996f;
        if (eyeState.z <= 94.0f) {
            const bool west = static_cast<double>(eyeState.x) - 24.9 + eyeState.z - 85.8 <= 0.0;
            anchorX = west ? 65.699997f : 26.4f;
            anchorZ = west ? 28.9f : 80.5f;
        }
        auto preferred = static_cast<std::int16_t>(Shenmue_Atan2Fixed(anchorX - eyeState.x, anchorZ - eyeState.z));
        const auto current = static_cast<std::int16_t>(Shenmue_Atan2Fixed(lookAt.x - eye.x, lookAt.z - eye.z));
        if (other && std::abs(preferred - current) < 0x4000) {
            auto toOther = static_cast<std::int16_t>(Shenmue_Atan2Fixed(
                fighter->contactCurrentPosition.x - other->contactCurrentPosition.x, fighter->contactCurrentPosition.z - other->contactCurrentPosition.z));
            const auto toEye =
                static_cast<std::int16_t>(Shenmue_Atan2Fixed(fighter->contactCurrentPosition.x - eyeState.x, fighter->contactCurrentPosition.z - eyeState.z));
            if (std::abs(toEye - toOther) > 0x4000)
                toOther = static_cast<std::int16_t>(toOther + 0x8000);
            if (toOther - 0x2000 < toEye && toEye < toOther + 0x2000)
                preferred = static_cast<std::int16_t>(preferred + (toEye <= toOther ? toOther - toEye - 0x2000 : toOther - toEye + 0x2000));
        }
        const auto yaw = static_cast<std::int16_t>(std::clamp(CombatCameraYawDeadband(static_cast<std::int16_t>(preferred - current)) / 2, -1536, 1536));
        eye = RotateCombatCameraEye(lookAt, yaw, eye);
    }

    const float radius = g_combat_cam_coli_hit_flag == 1 ? 0.2f : 0.1f;
    g_combat_cam_coli_hit_flag = coliTestPlane(fighterPosition, eye, radius, hit, mask.data());
    const bool blocked = g_combat_cam_coli_hit_flag == 1;
    g_combat_cam_collision_lift_frms = std::clamp(g_combat_cam_collision_lift_frms + (blocked ? 1 : -1), 0, 64);
    Vector3 &collisionSmooth = g_combat_cam_collision_offset_smooth;
    const float pushX = ((blocked ? hit.x : eye.x) - eye.x) - collisionSmooth.x;
    const float pushZ = ((blocked ? hit.z : eye.z) - eye.z) - collisionSmooth.z;
    if (std::fabs(pushX) >= kEpsilon)
        collisionSmooth.x += pushX * 0.33333334f;
    if (std::fabs(pushZ) >= kEpsilon)
        collisionSmooth.z += pushZ * 0.33333334f;
    eye.x += collisionSmooth.x;
    eye.z += collisionSmooth.z;
    const double lift = static_cast<double>(g_combat_cam_collision_lift_frms) * 0.015625;
    eye.y = static_cast<float>((seventyMan || g_areaId != SHEN_NAME(JHD0) ? lift * 1.5 : lift) + eye.y);
    StepTowardTargetSmoothly(eye, g_combatCameraEye, g_combatCameraEyeStep);
    StepTowardTargetSmoothly(lookAt, g_combatCameraTarget, g_combatCameraTargetStep);
    TranslateCameraRelative(g_combatCameraEye);
    if (!bLockCamera) {
        g_CameraTargetPos = g_combatCameraTarget;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    g_cameraRuntime.viewReady = true;
}

void UpdateArcadeCombatCamera() {
    constexpr float epsilon = 0.00000011920929F;
    constexpr std::uint32_t kCameraFocusTarget = 0x20000000U;
    const VFMAData *const game = vfMainGetCurrent();
    if (!game || game->playerSlot < 0 || game->playerSlot >= static_cast<std::int32_t>(kBattleSlots)) {
        return;
    }

    const std::size_t playerSlot = static_cast<std::size_t>(game->playerSlot);
    if (!game->characters[playerSlot] || ((game->slotTeamId[playerSlot] - 1U) & 0xfdU) == 0) {
        return;
    }
    const VFActor *const player = vfFindFighterFor(game->characters[playerSlot]);
    const VFActor *const target = player ? player->targetActor : nullptr;
    if (!player || !target)
        return;

    const bool focusTarget = (player->hitStateFlags2 & kCameraFocusTarget) != 0;
    g_arcade_cam_tight_follow_frms = std::clamp(g_arcade_cam_tight_follow_frms + (focusTarget ? 1 : -1), 0, 64);

    const Vector3 midpoint{
        (player->contactCurrentPosition.x + target->contactCurrentPosition.x) * 0.5F,
        (player->contactCurrentPosition.y + target->contactCurrentPosition.y) * 0.5F,
        (player->contactCurrentPosition.z + target->contactCurrentPosition.z) * 0.5F,
    };
    const Vector3 separation{
        target->contactCurrentPosition.x - player->contactCurrentPosition.x,
        target->contactCurrentPosition.y - player->contactCurrentPosition.y,
        target->contactCurrentPosition.z - player->contactCurrentPosition.z,
    };
    const float cameraDistance = std::max(std::sqrt(separation.x * separation.x + separation.y * separation.y + separation.z * separation.z), 2.0F) * 1.2F;
    const float railBlend = std::clamp(std::sqrt((midpoint.x - 2.04F) * (midpoint.x - 2.04F) + (midpoint.z + 3.0F) * (midpoint.z + 3.0F)) - 1.5F, 0.0F, 1.0F);
    const Vector3 railPoint{
        railBlend * 2.02F + 0.02F,
        1.11F,
        railBlend * 0.11999989F - 3.1199999F,
    };
    Vector3 direction{
        railPoint.x - midpoint.x,
        railPoint.y - midpoint.y,
        railPoint.z - midpoint.z,
    };
    const float directionLength = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (std::fabs(directionLength) < epsilon) {
        direction = {1.0F, 0.0F, 0.0F};
    } else {
        direction.x /= directionLength;
        direction.y /= directionLength;
        direction.z /= directionLength;
    }
    const Vector3 distancePoint{
        midpoint.x + direction.x * cameraDistance,
        midpoint.y + direction.y * cameraDistance,
        midpoint.z + direction.z * cameraDistance,
    };
    Vector3 requestedTarget = railPoint;
    const Vector3 remaining{
        railPoint.x - distancePoint.x,
        railPoint.y - distancePoint.y,
        railPoint.z - distancePoint.z,
    };
    const Vector3 distanceOffset{
        direction.x * cameraDistance,
        direction.y * cameraDistance,
        direction.z * cameraDistance,
    };
    if (remaining.x * distanceOffset.x + remaining.y * distanceOffset.y + remaining.z * distanceOffset.z > 0.0F) {
        requestedTarget = distancePoint;
    }

    const float divisor = focusTarget ? 5.0F : 8.0F;
    g_CameraPos = {
        g_CameraPos.x + (requestedTarget.x - g_CameraPos.x) / divisor,
        g_CameraPos.y + (requestedTarget.y - g_CameraPos.y) / divisor,
        g_CameraPos.z + (requestedTarget.z - g_CameraPos.z) / divisor,
    };
    g_tmpCameraLookAtV3 = {
        g_tmpCameraLookAtV3.x + (midpoint.x - g_tmpCameraLookAtV3.x) / divisor,
        g_tmpCameraLookAtV3.y + (midpoint.y - g_tmpCameraLookAtV3.y) / divisor,
        g_tmpCameraLookAtV3.z + (midpoint.z - g_tmpCameraLookAtV3.z) / divisor,
    };
    TranslateCameraRelative(g_CameraPos);
    if (!bLockCamera) {
        g_CameraTargetPos = g_tmpCameraLookAtV3;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    g_cameraRuntime.viewReady = true;
}



void camCtrlStartPerspective(std::int32_t horizontalFov, std::int32_t verticalHeight) {
    const float baseY = g_CameraTargetPos.y;
    const float deltaX = g_CameraEyePos.x - g_CameraTargetPos.x;
    const float deltaZ = g_CameraEyePos.z - g_CameraTargetPos.z;
    cameraStartPosition = g_CameraTargetPos;
    const std::int32_t halfHeight = verticalHeight / 2;
    initialHorizontalFov = horizontalFov;
    isCameraInterpolationActive = 1;
    cameraInterpStepsRemaining = halfHeight;
    const float horizontalDistance = std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
    horizontalCamDistance = horizontalDistance;
    cameraDirectionUnitVector = {deltaX / horizontalDistance, (g_CameraEyePos.y - baseY) / horizontalDistance, deltaZ / horizontalDistance};
    const float tanHalfFov = std::tan(static_cast<float>(currentPerspectiveFov / 2) * 6.2831855f * 0.000015258789f);
    halfViewHeightWorld = tanHalfFov * horizontalDistance;
    totalViewHeightWorld = tanHalfFov * horizontalDistance * 2.0f;
    viewHeightIncrementPerStep = ((std::tan(static_cast<float>(horizontalFov / 2) * 6.2831855f * 0.000015258789f) + tanHalfFov) * horizontalDistance -
                                     tanHalfFov * horizontalDistance * 2.0f) /
        static_cast<float>(halfHeight);
}

void UpdateAuthorPerspectiveTransition() {
    if (cameraInterpStepsRemaining <= 0) {
        isCameraInterpolationActive = 0;
        return;
    }
    totalViewHeightWorld = totalViewHeightWorld + viewHeightIncrementPerStep;
    const float distance = horizontalCamDistance * totalViewHeightWorld / (totalViewHeightWorld - halfViewHeightWorld);
    const std::int32_t fov = 2 * static_cast<std::int32_t>(atan2_fixed16(totalViewHeightWorld, distance));
    const float movement = distance - horizontalCamDistance;
    const Vector3 &direction = cameraDirectionUnitVector;
    const Vector3 &start = cameraStartPosition;
    TranslateCameraRelative({direction.x * movement + start.x, direction.y * movement + start.y, direction.z * movement + start.z});
    g_cameraRuntime.viewReady = true;
    CAME_SetFieldOfView(--cameraInterpStepsRemaining ? fov : initialHorizontalFov);
}

void UpdateScriptedCamera() {
    if (isPerspectiveTransitionActive) {
        float x = g_CameraEyePos.x - g_CameraTargetPos.x;
        float y = g_CameraEyePos.y - g_CameraTargetPos.y;
        float z = g_CameraEyePos.z - g_CameraTargetPos.z;
        const float norm = std::sqrt(y * y + x * x + z * z);
        if (norm > 0.0f) {
            const float inverse = 1.0f / norm;
            x *= inverse;
            y *= inverse;
            z *= inverse;
        }
        float height = totalViewHeightWorld;
        if (cameraInterpStepsRemaining >= 0) {
            height = totalViewHeightWorld + viewHeightIncrementPerStep;
            --cameraInterpStepsRemaining;
            totalViewHeightWorld = totalViewHeightWorld + viewHeightIncrementPerStep;
        }
        const float distance = horizontalCamDistance * height / (height - halfViewHeightWorld);
        const std::int32_t fov = 2 * static_cast<std::int32_t>(atan2_fixed16(height, distance));
        const float movement = distance - horizontalCamDistance;
        const Vector3 &target = g_CameraTargetPos;
        TranslateCameraRelative({movement * x + target.x, movement * y + target.y, movement * z + target.z});
        g_cameraRuntime.viewReady = true;
        CAME_SetFieldOfView(fov);
        return;
    }

    const float z = g_CameraEyePos.z - g_CameraTargetPos.z;
    const float x = g_CameraEyePos.x - g_CameraTargetPos.x;
    const float y = g_CameraEyePos.y - g_CameraTargetPos.y;
    const float height = std::tan(static_cast<float>(static_cast<std::uint16_t>(initialPerspectiveFov)) * 6.2831855f * 0.000015258789f) * initialCameraDistance;
    const float ratio = height / std::sqrt(y * y + x * x + z * z);
    const auto fov = static_cast<std::int32_t>(std::atan(ratio) * 65536.0f / 6.2831855f + 0.5f);
    if (static_cast<std::uint32_t>(fov - 11) <= 16372U)
        CAME_SetFieldOfView(fov);
}

constexpr std::uint16_t kDeveloperButtonA = 0x0400;
constexpr std::uint16_t kDeveloperButtonB = 0x0200;
constexpr std::uint16_t kDeveloperTranslate = 0x0002;
constexpr std::uint16_t kDeveloperOrbit = 0x0004;
constexpr std::uint16_t kDeveloperDollyPositive = 0x0040;
constexpr std::uint16_t kDeveloperDollyMask = 0x00c0;
constexpr std::uint16_t kDeveloperPitchPositive = 0x1000;
constexpr std::uint16_t kDeveloperPitchMask = 0x3000;
constexpr std::uint16_t kDeveloperYawPositive = 0x4000;
constexpr std::uint16_t kDeveloperYawMask = 0xc000;

float DeveloperLength(const Vector3 &value) {
    return std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
}

Vector3 DeveloperNormalize(const Vector3 &value, const Vector3 &fallback) {
    const float length = DeveloperLength(value);
    if (length <= 0.000001f)
        return fallback;
    const float inverseLength = 1.0f / length;
    return {value.x * inverseLength, value.y * inverseLength, value.z * inverseLength};
}

float DeveloperDot(const Vector3 &left, const Vector3 &right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

void RotateDeveloperPitch(Vector3 &point, const Vector3 &pivot, std::int16_t cameraYaw, std::int16_t angle) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(pivot.x, pivot.y, pivot.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(cameraYaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(angle));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-cameraYaw));
    Matrix4x4::preTranslate(-pivot.x, -pivot.y, -pivot.z);
    Matrix4x4::TransformPoint(point, point);
    Matrix4x4::Pop(1);
}

void RotateDeveloperYaw(Vector3 &point, const Vector3 &pivot, std::int16_t angle) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(pivot.x, pivot.y, pivot.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angle));
    Matrix4x4::preTranslate(-pivot.x, -pivot.y, -pivot.z);
    Matrix4x4::TransformPoint(point, point);
    Matrix4x4::Pop(1);
}

std::int32_t AdvanceDeveloperRotation(std::int32_t value, bool positive) {
    if (positive) {
        if (value <= 0)
            return 9;
        return value < 546 ? value + 4 : value;
    }
    if (value >= 0)
        return -9;
    return value > -546 ? value - 4 : value;
}

float AdvanceDeveloperTranslation(float value, bool positive) {
    if (positive) {
        if (value <= 0.0f)
            return 0.005f;
        return value < 2.0f ? static_cast<float>(static_cast<double>(value) + 0.0025) : value;
    }
    if (value >= 0.0f)
        return -0.005f;
    return value > -2.0f ? static_cast<float>(static_cast<double>(value) - 0.0025) : value;
}

bool DeveloperCharacterEligible(const DeveloperCameraState &camera, Character *character) {
    if ((camera.selectionScanFlags & 1) != 0)
        return true;
    if (!character || character->entity.name != SHEN_NAME(CEWP))
        return false;
    const std::uint32_t flags = character->flags.value;
    return (flags & 1U) != 0U || ((flags & 2U) != 0U && (character->motionFlags & 1U) != 0U);
}

bool InitializeDeveloperSelection(DeveloperCameraState &camera) {
    const MapControlData *const control = CurrentMapControl();
    if (!control) {
        camera.selectionCursor = nullptr;
        camera.selectedCharacter = nullptr;
        return false;
    }
    for (std::int32_t bucketIndex = 0; bucketIndex < static_cast<std::int32_t>(control->characterBuckets.size()); ++bucketIndex) {
        for (CharacterBucketNode *node = static_cast<CharacterBucketNode *>(control->characterBuckets[static_cast<std::size_t>(bucketIndex)]); node;
             node = node->next) {
            if (!DeveloperCharacterEligible(camera, node->character))
                continue;
            camera.selectionBucketIndex = bucketIndex;
            camera.selectionCursor = node;
            camera.selectedCharacter = node->character;
            return true;
        }
    }
    camera.selectionCursor = nullptr;
    camera.selectedCharacter = nullptr;
    return false;
}

bool AdvanceDeveloperSelection(DeveloperCameraState &camera) {
    const MapControlData *const control = CurrentMapControl();
    if (!control || !camera.selectionCursor)
        return false;
    CharacterBucketNode *node = camera.selectionCursor->next;
    for (std::int32_t bucketIndex = camera.selectionBucketIndex; bucketIndex < static_cast<std::int32_t>(control->characterBuckets.size()); ++bucketIndex) {
        if (bucketIndex != camera.selectionBucketIndex)
            node = static_cast<CharacterBucketNode *>(control->characterBuckets[static_cast<std::size_t>(bucketIndex)]);
        for (; node; node = node->next) {
            if (!DeveloperCharacterEligible(camera, node->character))
                continue;
            camera.selectionBucketIndex = bucketIndex;
            camera.selectionCursor = node;
            camera.selectedCharacter = node->character;
            return true;
        }
    }
    camera.selectionCursor = nullptr;
    camera.selectedCharacter = nullptr;
    return false;
}

void CopyDeveloperSelection(DeveloperCameraState &destination, const DeveloperCameraState &source) {
    destination.selectionScanFlags = source.selectionScanFlags;
    destination.selectionBucketIndex = source.selectionBucketIndex;
    destination.selectionCursor = source.selectionCursor;
    destination.selectedCharacter = source.selectedCharacter;
}

void ReverseDeveloperSelection(DeveloperCameraState &camera) {
    DeveloperCameraState cursor{};
    cursor.selectionScanFlags = camera.selectionScanFlags;
    if (!InitializeDeveloperSelection(cursor)) {
        CopyDeveloperSelection(camera, cursor);
        return;
    }
    if (cursor.selectedCharacter == camera.selectedCharacter) {
        while (AdvanceDeveloperSelection(camera)) {
        }
        return;
    }
    DeveloperCameraState previous = cursor;
    while (AdvanceDeveloperSelection(cursor)) {
        if (cursor.selectedCharacter == camera.selectedCharacter) {
            CopyDeveloperSelection(camera, previous);
            return;
        }
        previous = cursor;
    }
    CopyDeveloperSelection(camera, previous);
}

void UpdateCameraTransformWithMapControlOffset(Vector3 &origin, Vector3 &lookAt) {
    Matrix4x4 ::SetActiveTransformToPrePass();
    const MapControlData *const control = CurrentMapControl();
    Character *const ryo = static_cast<Character *>(control->ryo);
    const Vector3 position = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
    Matrix4x4::TransformPoint(origin, origin);
    Matrix4x4::TransformPoint(lookAt, lookAt);
    Matrix4x4::Pop(1);
}

void GetCurrentCameraPositionAndDirection(Vector3 &origin, Vector3 &lookAt) {
    const DeveloperCameraState &camera = g_cameraRuntime.developerCamera;
    if (g_CamState != 0) {
        origin = camera.origin;
        lookAt = camera.lookAt;
        return;
    }
    const MapControlData *const control = CurrentMapControl();
    Character *const ryo = static_cast<Character *>(control->ryo);
    const Vector3 position = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::TransformPoint(camera.origin, origin);
    Matrix4x4::TransformPoint(camera.lookAt, lookAt);
    Matrix4x4::Pop(1);
}

void CaptureDeveloperCameraView(DeveloperCameraState &camera) {
    camera.viewOrigin = g_CameraEyePos;
    camera.viewLookAt = g_CameraTargetPos;
    camera.zAngle = ZANGLE;
    camera.fieldOfView = currentPerspectiveFov;
    if (g_CamState == 0 || g_CamState == 1) {
        camera.origin = camera.viewOrigin;
        camera.lookAt = camera.viewLookAt;
        if (g_CamState == 0)
            UpdateCameraTransformWithMapControlOffset(camera.origin, camera.lookAt);
    }
    camera.initialized = true;
}

Vector3 DeveloperCharacterAnchor(Character *character, bool head) {
    if (!character)
        return g_tmp_seqdata_pos;
    if (!head)
        return GetPositionForEntity(character->entity.node);
    Vector3 position{};
    mtwkGetCharaBonePosition(character, 5U, &position);
    return position;
}

Vector3 DeveloperCameraDisplayPoint(const Vector3 &point, const DeveloperCameraState &camera) {
    if (camera.mode <= 0 || camera.mode > 8)
        return point;
    Character *const character = camera.selectedCharacter;
    const bool faceMode = camera.mode <= 4;
    const Vector3 anchor = DeveloperCharacterAnchor(character, faceMode);
    Vector3 result = point;
    Matrix4x4::SetActiveTransformToPrePass();
    if ((camera.mode & 1) == 0) {
        std::int32_t pitch = g_tmp_seqdata_rot.x;
        std::int32_t yaw = g_tmp_seqdata_rot.y;
        std::int32_t roll = g_tmp_seqdata_rot.z;
        if (character) {
            pitch = 0;
            roll = 0;
            if (faceMode) {
                MotionManagerWork *const manager = MOTM_GetWork(character);
                yaw = manager ? MOTN_ExtractEulerAngle(manager->motion, 5U) : charaGetWorldYaw(character);
            } else {
                yaw = charaGetWorldYaw(character);
            }
        }
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-roll));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-pitch));
    }
    Matrix4x4::preTranslate(-anchor.x, -anchor.y, -anchor.z);
    Matrix4x4::TransformPoint(result, result);
    Matrix4x4::Pop(1);
    if (camera.mode == 3 || camera.mode == 4 || camera.mode == 7 || camera.mode == 8) {
        const float radius = std::sqrt(result.x * result.x + result.z * result.z);
        const std::int16_t yaw = static_cast<std::int16_t>(atan2_fixed16(result.z, result.x));
        return {
            radius,
            static_cast<float>(static_cast<std::int16_t>(-0x4000 - yaw)) / 182.04445f,
            result.y,
        };
    }
    return result;
}

void RestoreDeveloperCameraVisibility() {
    ForEachCewpEntityApplyCallback(1, CAME_CewpToggleVisibility);
    const MapControlData *const control = CurrentMapControl();
    if (control->scenarioTask)
        SetMainCallbackToInitForCleanup(control->scenarioTask);
}

void FreezeForDeveloperCamera() {
    ForEachCewpEntityApplyCallback(0, CAME_CewpHide);
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *const scenarioTask = control ? control->scenarioTask : nullptr;
    if (!scenarioTask)
        return;
    if (!scenarioTask->initCallbackFnPtr) {
        scenarioTask->initCallbackFnPtr = scenarioTask->mainTaskCallbackFnPtr;
        scenarioTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
    }
    scenarioTask->mainExecutionIterations = 0;
}

void QueueDeveloperCameraText(float x, float y, std::string_view text) {
    if (GetDebugLogFlag())
        QueueDebugText(x, y, text);
}

void ClearDeveloperCameraText() {
    for (std::int32_t row = 3; row <= 15; ++row)
        QueueDeveloperCameraText(2.0f, static_cast<float>(row), "                    ");
    for (std::int32_t row = 21; row <= 25; ++row)
        QueueDeveloperCameraText(0.0f, static_cast<float>(row), "                                          ");
}

std::array<char, 5> DeveloperCharacterName(Character *character) {
    std::array<char, 5> name{'P', 'O', 'S', ' ', '\0'};
    if (!character)
        return name;
    const MapControlData *const control = CurrentMapControl();
    HLib::Task *const scenarioTask = control ? control->scenarioTask : nullptr;
    ScenarioTaskData *const scenario = scenarioTask ? reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr) : nullptr;
    if (!scenario)
        return {};
    SCEN_PromoteCharacterIndexNode(scenarioTask, character);
    const CharacterIdLookupNode *const node = scenario->characterBuckets[reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size()];
    const std::uint32_t id = node && node->character == character ? node->id : 0U;
    std::memcpy(name.data(), &id, sizeof(id));
    name[4] = '\0';
    return name;
}

void QueueDeveloperCameraValue(float x, float y, const char *format, float value) {
    std::array<char, 32> text{};
    const int length = std::snprintf(text.data(), text.size(), format, value);
    if (length > 0) {
        const std::size_t boundedLength = std::min(static_cast<std::size_t>(length), text.size() - 1);
        QueueDeveloperCameraText(x, y, std::string_view(text.data(), boundedLength));
    }
}

void QueueDeveloperCameraState() {
    const std::string_view state = g_CamState == 0 ? "CAM FIXED " : (g_CamState == 1 ? " CAM FREE " : "CAM NORMAL");
    QueueDeveloperCameraText(15.0f, 2.0f, state);
}

void QueueDeveloperCameraDetails(const DeveloperCameraState &camera) {
    static constexpr std::array<std::string_view, 9> kModeNames{
        "ABS",
        "relative_face",
        "local_face",
        "relative_face_pole",
        "local_face_pole",
        "relative_step",
        "local_step",
        "relative_step_pole",
        "local_step_pole",
    };
    if (camera.mode >= 0 && camera.mode < static_cast<std::int32_t>(kModeNames.size())) {
        const std::string_view modeName = kModeNames[static_cast<std::size_t>(camera.mode)];
        if (camera.mode == 0) {
            QueueDeveloperCameraText(2.0f, 3.0f, modeName);
        } else {
            const std::array<char, 5> characterName = DeveloperCharacterName(camera.selectedCharacter);
            std::array<char, 48> text{};
            const int length =
                std::snprintf(text.data(), text.size(), "%-13.*s ID:%s", static_cast<int>(modeName.size()), modeName.data(), characterName.data());
            if (length > 0)
                QueueDeveloperCameraText(2.0f, 3.0f, std::string_view(text.data(), std::min(static_cast<std::size_t>(length), text.size() - 1)));
        }
    }
    if (camera.mode == 9)
        return;
    const Vector3 displayOrigin = DeveloperCameraDisplayPoint(camera.viewOrigin, camera);
    const Vector3 displayTarget = DeveloperCameraDisplayPoint(camera.viewLookAt, camera);
    QueueDeveloperCameraText(2.0f, 4.0f, "CAM :");
    QueueDeveloperCameraValue(2.0f, 5.0f, "%3.4f", displayOrigin.x);
    QueueDeveloperCameraValue(2.0f, 6.0f, "%3.4f", displayOrigin.y);
    QueueDeveloperCameraValue(2.0f, 7.0f, "%3.4f", displayOrigin.z);
    QueueDeveloperCameraText(2.0f, 8.0f, "TAR :");
    QueueDeveloperCameraValue(2.0f, 9.0f, "%3.4f", displayTarget.x);
    QueueDeveloperCameraValue(2.0f, 10.0f, "%3.4f", displayTarget.y);
    QueueDeveloperCameraValue(2.0f, 11.0f, "%3.4f", displayTarget.z);
    QueueDeveloperCameraText(2.0f, 12.0f, "ZANG :");
    QueueDeveloperCameraValue(2.0f, 13.0f, "%3.5f", static_cast<float>(camera.zAngle) / 182.04445f);
    QueueDeveloperCameraText(2.0f, 14.0f, "PERS :");
    QueueDeveloperCameraValue(2.0f, 15.0f, "%3.5f", camera.fieldOfView / 182.04445f);
}

bool UpdateDeveloperCameraController() {
    DeveloperCameraState &camera = g_cameraRuntime.developerCamera;
    if (camera.setDefaultView) {
        CaptureDeveloperCameraView(camera);
        if (g_CamState == 0 || g_CamState == 1) {
            camera.mode = 9;
            camera.viewOffsetMode = 1;
            camera.pitchStep = 0;
            camera.yawStep = 0;
            camera.dolly = 0.0f;
            camera.lift = 0.0f;
            camera.strafe = 0.0f;
            camera.pan = 0.0f;
            camera.selectionScanFlags = 0;
            InitializeDeveloperSelection(camera);
        }
        camera.setDefaultView = false;
    }

    std::uint16_t flags = camera.flags;
    if ((flags & kDeveloperOrbit) != 0)
        flags &= ~kDeveloperTranslate;
    if ((flags & (kDeveloperTranslate | kDeveloperOrbit)) == 0 && (camera.pressedButtons & kDeveloperButtonB) != 0) {
        g_CamState = g_CamState < 2 ? g_CamState + 1 : 0;
        if (g_CamState == 0) {
            CaptureDeveloperCameraView(camera);
            Matrix4x4::Pop(1);
        } else if (g_CamState == 1) {
            camera.origin = camera.viewOrigin;
            camera.lookAt = camera.viewLookAt;
        } else if (g_CamState == 2) {
            ClearDeveloperCameraText();
            RestoreDeveloperCameraVisibility();
        }
    }

    if (!camera.initialized) {
        CaptureDeveloperCameraView(camera);
        if (!camera.selectedCharacter)
            InitializeDeveloperSelection(camera);
    }
    QueueDeveloperCameraState();
    if (g_CamState == 2)
        return false;

    FreezeForDeveloperCamera();
    if ((flags & (kDeveloperTranslate | kDeveloperOrbit)) == 0 && (camera.pressedButtons & kDeveloperButtonA) != 0) {
        camera.mode = camera.mode == 9 ? 0 : camera.mode + 1;
    }

    const bool orbit = (flags & kDeveloperOrbit) != 0;
    const bool translate = (flags & kDeveloperTranslate) != 0;
    Vector3 direction{
        camera.lookAt.x - camera.origin.x,
        camera.lookAt.y - camera.origin.y,
        camera.lookAt.z - camera.origin.z,
    };
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(Shenmue_Atan2Fixed(direction.x, direction.z)));
    Matrix4x4::Pop(1);
    camera.viewHeightVelocity = static_cast<float>(static_cast<double>(camera.viewHeightVelocity) + (camera.viewHeightOffset > 0.0f ? -0.0001 : 0.0001));
    camera.viewHeightOffset += camera.viewHeightVelocity;
    if (!translate || orbit) {
        if ((flags & kDeveloperPitchMask) != 0) {
            camera.pitchStep = AdvanceDeveloperRotation(camera.pitchStep, (flags & kDeveloperPitchPositive) == 0);
            const std::int16_t cameraYaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(direction.x, direction.z));
            if (orbit)
                RotateDeveloperPitch(camera.origin, camera.lookAt, cameraYaw, camera.pitchStep);
            else
                RotateDeveloperPitch(camera.lookAt, camera.origin, cameraYaw, camera.pitchStep);
        } else {
            camera.pitchStep = 0;
        }

        if ((flags & kDeveloperYawMask) != 0) {
            camera.yawStep = AdvanceDeveloperRotation(camera.yawStep, (flags & kDeveloperYawPositive) != 0);
            if (orbit)
                RotateDeveloperYaw(camera.origin, camera.lookAt, camera.yawStep);
            else
                RotateDeveloperYaw(camera.lookAt, camera.origin, camera.yawStep);
        } else {
            camera.yawStep = 0;
        }

        direction = {
            camera.lookAt.x - camera.origin.x,
            camera.lookAt.y - camera.origin.y,
            camera.lookAt.z - camera.origin.z,
        };
        if ((flags & kDeveloperDollyMask) != 0) {
            camera.dolly = AdvanceDeveloperTranslation(camera.dolly, orbit ? (flags & kDeveloperDollyPositive) == 0 : (flags & kDeveloperDollyPositive) != 0);
            const Vector3 unit = DeveloperNormalize(direction, {0.0f, 0.0f, 1.0f});
            if (orbit) {
                const Vector3 previous = camera.origin;
                camera.origin = {
                    camera.origin.x + unit.x * camera.dolly,
                    camera.origin.y + unit.y * camera.dolly,
                    camera.origin.z + unit.z * camera.dolly,
                };
                const Vector3 remaining{
                    camera.lookAt.x - camera.origin.x,
                    camera.lookAt.y - camera.origin.y,
                    camera.lookAt.z - camera.origin.z,
                };
                if (DeveloperDot(remaining, direction) <= 0.0f || DeveloperLength(remaining) < 0.2f) {
                    camera.origin = previous;
                }
            } else {
                const Vector3 previous = camera.lookAt;
                camera.lookAt = {
                    camera.lookAt.x + unit.x * camera.dolly,
                    camera.lookAt.y + unit.y * camera.dolly,
                    camera.lookAt.z + unit.z * camera.dolly,
                };
                const Vector3 remaining{
                    camera.lookAt.x - camera.origin.x,
                    camera.lookAt.y - camera.origin.y,
                    camera.lookAt.z - camera.origin.z,
                };
                if (DeveloperDot(remaining, direction) <= 0.0f || DeveloperLength(remaining) < 0.2f) {
                    camera.lookAt = previous;
                }
            }
        } else {
            camera.dolly = 0.0f;
        }
    } else {
        camera.pitchStep = 0;
        camera.yawStep = 0;
        camera.dolly = 0.0f;
    }

    int stickX = 128 - static_cast<int>(camera.analogX);
    if (std::abs(stickX) < 5)
        stickX = 0;
    if (stickX != 0 && camera.mode >= 1 && camera.mode <= 8) {
        camera.selectionAccumulator -= stickX;
        if (camera.selectionAccumulator > 250) {
            if (camera.selectedCharacter)
                AdvanceDeveloperSelection(camera);
            else
                InitializeDeveloperSelection(camera);
            camera.selectionAccumulator = 0;
        } else if (camera.selectionAccumulator < -250) {
            ReverseDeveloperSelection(camera);
            camera.selectionAccumulator = 0;
        }
    } else {
        camera.selectionAccumulator = 0;
    }
    if (orbit) {
        const std::int32_t delta = static_cast<std::int32_t>((static_cast<double>(stickX) / 500.0) * -8192.0 / 45.0);
        camera.zAngle = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(camera.zAngle) - delta));
    }
    if (translate) {
        direction = {
            camera.lookAt.x - camera.origin.x,
            camera.lookAt.y - camera.origin.y,
            camera.lookAt.z - camera.origin.z,
        };
        const Vector3 forward = DeveloperNormalize({direction.x, 0.0f, direction.z}, {0.0f, 0.0f, 1.0f});
        const Vector3 right{forward.z, 0.0f, -forward.x};
        if ((flags & kDeveloperPitchMask) != 0) {
            camera.lift = AdvanceDeveloperTranslation(camera.lift, (flags & kDeveloperPitchPositive) != 0);
            camera.origin.y += camera.lift;
            camera.lookAt.y += camera.lift;
        } else {
            camera.lift = 0.0f;
        }
        if ((flags & kDeveloperYawMask) != 0) {
            camera.strafe = AdvanceDeveloperTranslation(camera.strafe, (flags & kDeveloperYawPositive) == 0);
            camera.origin.x += right.x * camera.strafe;
            camera.origin.z += right.z * camera.strafe;
            camera.lookAt.x += right.x * camera.strafe;
            camera.lookAt.z += right.z * camera.strafe;
        } else {
            camera.strafe = 0.0f;
        }
        if ((flags & kDeveloperDollyMask) != 0) {
            camera.pan = AdvanceDeveloperTranslation(camera.pan, (flags & kDeveloperDollyPositive) != 0);
            camera.origin.x += forward.x * camera.pan;
            camera.origin.z += forward.z * camera.pan;
            camera.lookAt.x += forward.x * camera.pan;
            camera.lookAt.z += forward.z * camera.pan;
        } else {
            camera.pan = 0.0f;
        }
        if ((camera.pressedButtons & kDeveloperButtonA) != 0)
            camera.viewOffsetMode = (camera.viewOffsetMode + 1) % 3;
        if ((camera.pressedButtons & kDeveloperButtonB) != 0 && camera.mode >= 1 && camera.mode <= 8) {
            if (camera.selectedCharacter)
                mtwkGetCharaBonePosition(camera.selectedCharacter, 5U, &camera.lookAt);
            else
                camera.lookAt = g_tmp_seqdata_pos;
        }
        int stickY = 128 - static_cast<int>(camera.analogY);
        if (std::abs(stickY) < 5)
            stickY = 0;
        const std::int32_t delta = static_cast<std::int32_t>((static_cast<double>(stickY) / 1000.0) * -8192.0 / 45.0);
        camera.fieldOfView = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(camera.fieldOfView) - delta));
    } else {
        camera.lift = 0.0f;
        camera.strafe = 0.0f;
        camera.pan = 0.0f;
    }

    g_cameraRuntime.viewReady = finite(camera.origin) && finite(camera.lookAt);
    return true;
}

void ApplyCameraShake() {
    float previous = g_cam_shake_prev;
    if (std::fabs(previous) < 0.003f) {
        previous = 0.0f;
        g_cam_shake_prev = 0.0f;
    }
    float current = g_cam_shake_curr;
    if (std::fabs(current) < 0.003f) {
        current = 0.0f;
        g_cam_shake_curr = 0.0f;
    }
    if (previous == 0.0f && current == 0.0f)
        return;
    const std::uint32_t first = LcgStep(g_EnvPrngSeed);
    const std::uint32_t second = LcgStep(first);
    g_EnvPrngSeed = second;
    const float randomY = static_cast<float>(static_cast<double>((first >> 16U) & 0x7fffU) * 0.000030517578125);
    const float randomX = static_cast<float>(static_cast<double>((second >> 16U) & 0x7fffU) * 0.000030517578125);
    Matrix4x4::preTranslate((((0.5f - randomX) * 0.5f) + 1.0f) * previous, (((0.5f - randomY) * 0.5f) + 1.0f) * current, 0.0f);
    const float dampedPrevious = previous * g_cam_shake_damp;
    g_cam_shake_prev = dampedPrevious;
    g_cam_shake_curr = -(g_cam_shake_damp * current);
    if (g_cam_shake_curr == 0.0f || (g_EnvPrngSeed = LcgStep(second), (g_EnvPrngSeed & 0x30000U) != 0U)) {
        g_cam_shake_prev = -dampedPrevious;
    }
}

void BuildDeveloperCameraView() {
    DeveloperCameraState &camera = g_cameraRuntime.developerCamera;
    GetCurrentCameraPositionAndDirection(camera.viewOrigin, camera.viewLookAt);
    SetActiveStack(MatrixStackType::Transparent);
    workTransformMatrix = transformMatrix_prePass;
    const DisplayModeInfo *const screen = GetScreenResolution();
    const float degrees = static_cast<float>(camera.fieldOfView) / 182.04445f * 0.5f;
    const float halfFov = std::tan(static_cast<float>(static_cast<double>(degrees) / 57.29577951308238));
    const float adjusted = static_cast<float>(static_cast<std::uint16_t>(
        static_cast<std::int32_t>(std::atan(static_cast<float>(static_cast<double>(halfFov) * 0.75)) * 65536.0f / 6.2831855f + 0.5f)));
    const float doubled = static_cast<float>((static_cast<double>(adjusted) / 10430.37835047046) * 2.0);
    const std::uint16_t projectionFov = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(doubled) * 10430.37835047046));

    const float aspectRatio = static_cast<float>(screen->width) / static_cast<float>(GetScreenResolution()->height);
    Matrix4x4_SetupCameraProjectionMatrix(projectionFov, aspectRatio, 0.1f, 200000.0f);

    SetActiveStack(MatrixStackType::DepthPrepass);
    workTransformMatrix = transformMatrix_prePass;
    CalcCameraTransform({camera.viewOrigin.x, camera.viewOrigin.y, camera.viewOrigin.z, 0.0f},
        {camera.viewLookAt.x, camera.viewLookAt.y, camera.viewLookAt.z, 0.0f},
        std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(0U - static_cast<std::uint16_t>(camera.zAngle))));
    matrix_stack::Matrix4x4_Push();
    if (camera.viewOffsetMode == 0)
        Matrix4x4 ::preTranslate(camera.viewLookAt.x, camera.viewLookAt.y + camera.viewHeightOffset, camera.viewLookAt.z);
    else if (camera.viewOffsetMode == 1)
        Matrix4x4 ::preTranslate(camera.viewLookAt.x, camera.viewLookAt.y, camera.viewLookAt.z);
    Matrix4x4::Pop(1);
    QueueDeveloperCameraDetails(camera);
}

} // namespace
void TranslateCameraRelative(const Vector3 &relativePosition) {
    if (bLockCamera)
        return;

    const Vector3 previousEye = g_CameraEyePos;
    g_CameraEyePos = relativePosition;
    shenApplyTranslationToAll(g_cameraEyeNode, g_CameraEyePos);

    if (g_areaId == SHEN_NAME(FBGG))
        return;

    const float deltaX = previousEye.x - g_CameraEyePos.x;
    const float deltaY = previousEye.y - g_CameraEyePos.y;
    const float deltaZ = previousEye.z - g_CameraEyePos.z;
    const float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    if (distance <= 1.2f) {
        g_bHasSetupAutoExposure = 0;
    } else if (!g_bHasSetupAutoExposure) {
        GPUPostEffect_InitializeAutoExposure(-1, 0.18000001f, 0.75f);
        GPUPostEffect_SetAutoExposure(-1);
        g_bHasSetupAutoExposure = 1;
    }
}

void CAME_Callback(HLib::TaskData *) {
    if (!g_LookAroundViewMode && g_updateRenderFrameMode)
        UpdateLookAroundAnalogInput();

    g_tmpCamEyePos = g_CameraEyePos;
    g_tmpCamTargetPos = g_CameraTargetPos;
    const bool stateUpdateAllowed = !g_DisableCamStateUpdate || g_CamState == 2;
    if (stateUpdateAllowed) {
        const std::uint32_t needsReset = nCamState & 3U;
        nCamState &= ~3U;

        switch (g_nAuthorState) {
        case 0:
            g_seqActive = 0;
            if (g_VF_camera_active) {
                if (needsReset || g_CameraFOVState != 1) {
                    shenTransitionToCam3();
                    g_CameraFOVState = 1;
                }
                if (g_updateRenderFrameMode)
                    UpdateCombatCamera();
            } else {
                if (needsReset || g_CameraFOVState != 0) {
                    g_CameraOverrideActiveFlag = 0;
                    UpdateThirdPersonCamera();
                    g_CameraFOVState = 0;
                }
                if (g_updateRenderFrameMode)
                    ryoApplyCameraSmoothingAndCollisionTrans();
            }
            break;

        case 6:
            g_seqActive = ShouldRun() ? 0 : 1;
            if (needsReset)
                authUpdate(static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(ASEQ_current_chunk_ptr)), g_pCEWP, g_pASEQ_charas[0]);
            ReadCameraSequenceKeyframes();
            break;

        case 7:
            g_seqActive = 0;
            HandleDynamicInteractionState();
            break;

        case 8:
            g_seqActive = 1;
            if (needsReset) {
                g_animatedCameraSequence.eyeAnchorOwner = g_pCEWP;
                g_animatedCameraSequence.targetAnchorOwner = g_pASEQ_charas[0];
                g_animatedCameraSequence.chunk = ASEQ_current_chunk_ptr;
                StartAnimatedCamera();
                ReadCameraSequenceKeyframes();
                bInterpretRuntime = 0;
                ReadCameraSequenceKeyframes();
            } else if (!bInterpretRuntime) {
                ReadCameraSequenceKeyframes();
            }
            break;

        case 0x0a:
            g_seqActive = 0;
            if (needsReset)
                shenResetCombatCameraForTransition();
            if (g_updateRenderFrameMode) {
                if (g_areaId == SHEN_NAME(GMCT))
                    UpdateArcadeCombatCamera();
                else
                    UpdateCombatCamera();
            }
            break;

        case 0x0e:
            g_seqActive = 1;
            ReadCameraSequenceKeyframes();
            break;

        case 9:
        case 0x0f:
            g_seqActive = 0;
            break;

        case 0x10:
            g_seqActive = 0;
            if (needsReset)
                camCtrlStartPerspective(static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(ASEQ_current_chunk_ptr)),
                    static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(g_pCEWP)));
            UpdateAuthorPerspectiveTransition();
            break;

        case 0x11:
            g_seqActive = 0;
            if (needsReset)
                FallbackCameraUpdate();
            if (g_updateRenderFrameMode)
                UpdateCameraPositionAndRotation();
            break;

        default:
            break;
        }
    }

    if (g_CameraZoomState)
        HandleCameraZoomStates();
    if (ctrlGetNeedsCameraDamping() && g_updateRenderFrameMode)
        camHandleDamping();
    if (isScriptedCameraActive || isPerspectiveTransitionActive)
        HandleScriptedCamera();
    if (GetHasAuthorState())
        HandleCameraForAuthorState();


    if (!g_DisableCamStateUpdate) {
        HandleFreeCameraState();
    } else {
        if (UpdateDeveloperCameraController())
            BuildDeveloperCameraView();
        if (!g_DisableCamStateUpdate || g_CamState == 2)
            HandleFreeCameraState();
    }

    Shenmue_InitEffectByIndexExact();
    g_CurrLODDistance = LOD_Distance_0;
    g_CurrLODDistance_0 = LOD_Distance_0;
    g_CurrLODDistance_1 = LOD_Distance_2;
    g_CurrLODDisplayRadius = NPCDisplayRadius;
    g_fCurrWorldToGridFactor = g_mapBlockScale;
    prevScreenSpaceToWorldScale = {
        screenSpaceToWorldScale.y,
        screenSpaceToWorldScale.x,
    };
    g_prev_temp_dir = g_fCamAccumYaw;
    BuildFrustumOcclusionTable();
}

HLib::Task *CAME_Init() {
    bLockCamera = 0;
    g_tmp_interpolated_camrot = {};
    g_tmp_cam_rot_z_adj = 0;
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(CAME_Callback, 4, 7, SHEN_NAME(CAME));
    if (!task || task->task_data_ptr) {
        if (task) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
        }
        task = nullptr;
    } else {
        void *const data = HLib_AllocHeapMemBlock(0, SHEN_NAME(TASK));
        task->task_data_ptr = static_cast<HLib::TaskData *>(data);
        if (!data) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
            task = nullptr;
        } else {
            HLib_EnableTokenCheck(data);
        }
    }

    g_CameraEyePos = {0.0f, 0.0f, -30.0f};
    g_cameraEyeNode = {};
    g_cameraEyeNode.position = g_CameraEyePos;
    g_CameraTargetPos = {};
    g_cameraTargetNode = {};
    g_script_cam_offs2 = {1.0f, 1.0f, 1.0f};
    g_script_cam_offs = {1.0f, 1.0f, 1.0f};
    nCamState = 0x0b;
    g_nAuthorState = 0;
    g_temporaryGameState = 0;
    g_previousAuthorState = 0;
    currentPerspectiveFov = 10922;
    FieldOfView = 10922;
    computedCameraPitch = 8556;
    ZANGLE = 0;
    au_re_CEWP_Execute();
    g_cam_shake_damp = 1.0f;
    g_cam_shake_prev = 0.0f;
    g_cam_shake_curr = 0.0f;
    convCamInit();
    g_cameraStateFlags &= ~CF_LookAroundMask;
    CameraDistanceFromPlayer = 3.0999999f;
    CameraYAngle = 1.2f;
    g_curr_look_around_frms = 999;
    g_aseq_do_coli_check = 0;
    GCameraSettings2.dampingOwner = nullptr;
    g_hazukiResidenceGatedFlag = 1;
    g_camera_rot_step_lock = 5;
    useAnalogInput = 1;
    g_tmp_cam_pos.w = 0.0f;
    g_NeedsReinit = 0;
    rawAnalogInputValue = 0;
    g_look_around_angle_x = 0;
    g_look_around_angle_y = 0;
    g_playerCameraAngles_0 = 0;
    g_playerCameraAngles_1 = 0;
    g_ctrlLookAtEnabled = 1;
    g_ctrl_set_lookat = 0;
    g_tmp_camera_ang_y = 0;
    g_tmp_cam_factor = 0.0f;
    g_CameraOverrideActiveFlag = 0;
    g_VF_camera_active = 0;
    CameraHeightAbovePlayer = 1.3f;
    CameraHeightAbovePlayer_MAX = 1.3f;
    g_bIsConversationCamInactive = 1;
    g_CameraCollisionEnabled = 1;
    g_numFramesActive = 0;
    g_cameraDeltaTimeRampStep = 0.0f;
    g_cam_frm_counter = 0;
    g_bNegativeCamBlend = 0;
    g_EnableCameraHeightClamp = 0;
    g_CameraZoomFOVModifier = 1.0f;
    g_CameraInterpolationTimer = 30;
    g_shouldComputeBetweenTwoEntities = 0;
    convCamInit();
    isScriptedCameraActive = 0;
    isPerspectiveTransitionActive = 0;
    relativeZoomMode = 0;
    g_CameraZoomState = 0;
    g_temp_angle_deg = 0;
    g_temp_angle_deg2 = 0;

    g_cameraRuntime.viewReady = false;
    SetScene80CameraMode();
    return task;
}

bool CAME_GetView(Vector3 *eye, Vector3 *target, float *verticalFovRadians) {
    if (!eye || !target || !g_cameraRuntime.viewReady || !finite(g_CameraTargetPos) || !finite(g_CameraEyePos)) {
        return false;
    }
    const float dx = g_CameraTargetPos.x - g_CameraEyePos.x;
    const float dy = g_CameraTargetPos.y - g_CameraEyePos.y;
    const float dz = g_CameraTargetPos.z - g_CameraEyePos.z;
    if (dx * dx + dy * dy + dz * dz < 0.0001f)
        return false;
    *eye = g_CameraEyePos;
    *target = g_CameraTargetPos;
    if (!verticalFovRadians)
        return true;
    *verticalFovRadians = static_cast<float>(currentPerspectiveFov) * (6.2831853071795864769f / 65536.0f);
    return std::isfinite(*verticalFovRadians) && *verticalFovRadians > 0.05f && *verticalFovRadians < 3.0f;
}

float CAME_SetLodDistance0(float value) {
    const float previous = LOD_Distance_0;
    LOD_Distance_0 = value;
    return previous;
}



float CAME_GetLodDistance0() {
    return LOD_Distance_0;
}

float CAME_SetLodDistance2(float value) {
    const float previous = LOD_Distance_2;
    LOD_Distance_2 = value;
    return previous;
}

float CAME_GetLodDistance2() {
    return LOD_Distance_2;
}

float CAME_SetNpcDisplayRadius(float value) {
    const float previous = NPCDisplayRadius;
    NPCDisplayRadius = value;
    return previous;
}

float CAME_GetNpcDisplayRadius() {
    return NPCDisplayRadius;
}

float CAME_GetCurrentLodDistance0() {
    return g_CurrLODDistance_0;
}



void CAME_ExecuteLodSnapshotCommand(std::int32_t command) {
    DynamicMapLodSnapshot &snapshot = g_dynamicMapLodSnapshot;
    switch (command) {
    case 0:
    case 4:
        snapshot.lodDistance0 = LOD_Distance_0;
        snapshot.npcDisplayRadius = NPCDisplayRadius;
        snapshot.lodDistance2 = LOD_Distance_2;
        if (command == 4)
            break;
        [[fallthrough]];
    case 2:
        DYNM_TransferRenderEnabledMask(false);
        DYNM_TransferClipCellsReadyMask(false);
        DYNM_TransferOcclusionEnabledMask(false);
        break;
    case 1:
    case 5:
        LOD_Distance_0 = snapshot.lodDistance0;
        NPCDisplayRadius = snapshot.npcDisplayRadius;
        LOD_Distance_2 = snapshot.lodDistance2;
        if (command == 5)
            break;
        [[fallthrough]];
    case 3:
        DYNM_ExecuteLodSnapshotCommand(3);
        break;
    default:
        break;
    }
}

void CAME_SetScriptedView(const Vector3 &eye, const Vector3 &target, std::int32_t perspectiveFov, std::int16_t zAngle) {
    if (!finite(eye) || !finite(target))
        return;



    if (!bLockCamera) {
        TranslateCameraRelative(eye);
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, target);
    }
    currentPerspectiveFov = perspectiveFov;
    ZANGLE = zAngle;
    const float deltaZ = eye.z - target.z;
    const float deltaX = eye.x - target.x;
    if (std::fabs(deltaZ) >= 0.001f || std::fabs(deltaX) >= 0.001f) {
        g_fCamAccumYaw = static_cast<std::int16_t>(atan2_fixed16(deltaZ, deltaX) + 0x4000);
    } else {
        g_fCamAccumYaw = 0;
    }
    g_cameraRuntime.viewReady = true;
}

void CAME_SetCameraFlags(std::int32_t value) {
    g_tmp_cam_pos.w = std::bit_cast<float>(static_cast<std::uint32_t>(value) >> 31U);
}
void CAME_SetFieldOfView(std::int32_t fieldOfView) {
    if (bLockCamera)
        return;
    currentPerspectiveFov = fieldOfView;
    constexpr float kTwoPi = 6.2831855f;
    constexpr float kFixedToTurn = 1.52587890625e-05f;
    const auto toFixed = [](float radians) { return static_cast<std::uint16_t>(static_cast<std::int32_t>(radians * 65536.0f / kTwoPi + 0.5f)); };
    const float degrees = static_cast<float>(fieldOfView) / 182.04445f * 0.5f;
    const float halfFov = std::tan(static_cast<float>(static_cast<double>(degrees) / 57.29577951308238));
    const float adjusted = static_cast<float>(toFixed(std::atan(static_cast<float>(static_cast<double>(halfFov) * 0.75))));
    const auto doubled = static_cast<float>(static_cast<double>(adjusted) / 10430.37835047046 + static_cast<double>(adjusted) / 10430.37835047046);
    computedCameraPitch = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(doubled) * 10430.37835047046));

    const DisplayModeInfo *const screen = GetScreenResolution();
    const float nativeHalfFov = std::tan(static_cast<float>(static_cast<std::uint16_t>(fieldOfView / 2)) * kTwoPi * kFixedToTurn);
    const float aspect = static_cast<float>(screen->width) / static_cast<float>(GetScreenResolution()->height);
    const float verticalFixed = static_cast<float>(toFixed(std::atan(nativeHalfFov / (1.3333334f / aspect))));
    const auto verticalFull = static_cast<std::int16_t>(static_cast<std::int32_t>(verticalFixed + verticalFixed));
    const auto verticalHalf = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(verticalFull) * 0.5f));
    const float vertical = std::tan(static_cast<float>(verticalHalf) * kTwoPi * kFixedToTurn);
    screenSpaceToWorldScale.y = vertical;

    const float currentAspect = static_cast<float>(GetScreenResolution()->width) / static_cast<float>(GetScreenResolution()->height);
    const auto horizontalFixed = static_cast<std::uint16_t>(static_cast<std::int32_t>(std::atan(1.0f / currentAspect * vertical) * 65536.0f / kTwoPi + 0.5f));
    const auto horizontalFull = static_cast<std::int16_t>(static_cast<std::uint16_t>(horizontalFixed + horizontalFixed));
    const auto horizontalHalf = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(horizontalFull) * 0.5f));
    screenSpaceToWorldScale.x = std::tan(static_cast<float>(horizontalHalf) * kTwoPi * kFixedToTurn);
}

bool CAME_BuildScriptAuthorSequence(const std::uint32_t *params, const std::byte *taskData) {
    if (!params)
        return false;
    g_scriptAuthorSequence.fill(std::byte{0});
    std::uint32_t flags = params[0] | 0x20U;
    if (!params[2])
        flags |= 0x200000U;
    if (!params[4])
        flags |= 0x400000U;
    if (params[6])
        flags |= 0x180U;
    if (params[7])
        flags |= 0x1000040U;
    std::memcpy(g_scriptAuthorSequence.data(), &flags, sizeof(flags));
    std::size_t cursor = sizeof(flags);
    const float duration = std::bit_cast<float>(params[1]);
    const auto readVector = [taskData](std::uint32_t rawOffset, const Vector3 &fallback) {
        const std::int32_t offset = static_cast<std::int32_t>(rawOffset);
        if (!taskData || !offset)
            return fallback;
        Vector3 value{};
        std::memcpy(&value, taskData + offset, sizeof(value));
        return value;
    };
    const Vector3 targetStart = readVector(params[2], {});
    const Vector3 targetEnd = readVector(params[3], g_CameraEyePos);
    const Vector3 eyeStart = readVector(params[4], {});
    const Vector3 eyeEnd = readVector(params[5], g_CameraTargetPos);
    const auto appendChannel = [&](float start, float end) {
        const std::int32_t count = 2;
        const std::array<float, 2> times{0.0f, duration};
        const std::array<float, 2> values{start, end};
        const std::array<float, 2> slopes{0.0f, 0.0f};
        std::memcpy(g_scriptAuthorSequence.data() + cursor, &count, sizeof(count));
        cursor += sizeof(count);
        std::memcpy(g_scriptAuthorSequence.data() + cursor, times.data(), sizeof(times));
        cursor += sizeof(times);
        std::memcpy(g_scriptAuthorSequence.data() + cursor, values.data(), sizeof(values));
        cursor += sizeof(values);
        std::memcpy(g_scriptAuthorSequence.data() + cursor, slopes.data(), sizeof(slopes));
        cursor += sizeof(slopes);
    };
    appendChannel(targetStart.x, targetEnd.x);
    appendChannel(targetStart.y, targetEnd.y);
    appendChannel(targetStart.z, targetEnd.z);
    appendChannel(eyeStart.x, eyeEnd.x);
    appendChannel(eyeStart.y, eyeEnd.y);
    appendChannel(eyeStart.z, eyeEnd.z);
    appendChannel(0.0f, 0.0f);
    if (params[7])
        appendChannel(0.0f, std::bit_cast<float>(params[7]));
    return true;
}


void CAME_StartBuiltScriptAuthorSequence(Character *eyeOwner, Character *targetOwner) {
    g_animatedCameraSequence.eyeAnchorOwner = eyeOwner;
    g_animatedCameraSequence.chunk = reinterpret_cast<std::uint32_t *>(g_scriptAuthorSequence.data());
    g_animatedCameraSequence.targetAnchorOwner = targetOwner;
    StartAnimatedCamera();
}



void StartAnimatedCamera() {
    AnimatedCameraSequence &sequence = g_animatedCameraSequence;
    if (!sequence.chunk)
        return;
    const std::uint32_t flags = sequence.chunk[0];
    sequence.flags = flags;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        sequence.eyeReaders[axis].coefficients = sequence.eyeCoefficients[axis].data();
        sequence.targetReaders[axis].coefficients = sequence.targetCoefficients[axis].data();
        sequence.eyeCoefficients[axis][0] = (flags & ASF_UNIT_COEFFICIENT_1) != 0 ? 1.0f : 0.0f;
        sequence.targetCoefficients[axis][0] = (flags & ASF_UNIT_COEFFICIENT_2) != 0 ? 1.0f : 0.0f;
    }
    sequence.zAngleReader.coefficients = sequence.zAngleCoefficients.data();
    sequence.fovReader.coefficients = sequence.fovCoefficients.data();
    sequence.fovCoefficients[0] = 0.0f;
    sequence.zAngleCoefficients[0] = 0.0f;
    if ((flags & ASF_USE_OBJECT_POS_ROT_1) != 0)
        ResolveSequenceAnchor(false, true);
    if ((flags & ASF_USE_OBJECT_POS_ROT_2) != 0)
        ResolveSequenceAnchor(true, true);

    std::uint32_t *cursor = sequence.chunk + 1;
    for (FcvSplineReader &reader : sequence.eyeReaders)
        cursor = BindSequenceReader(reader, cursor);
    for (FcvSplineReader &reader : sequence.targetReaders)
        cursor = BindSequenceReader(reader, cursor);
    if ((flags & ASF_USE_ZANGLE_ANIMATION) != 0)
        cursor = BindSequenceReader(sequence.zAngleReader, cursor);
    if ((flags & ASF_USE_FOV_ANIMATION) != 0)
        BindSequenceReader(sequence.fovReader, cursor);


    Vector3 position = g_CameraEyePos;
    Vector3 target = g_CameraTargetPos;
    if ((flags & ASF_USE_OBJECT_POS_ROT_1) != 0)
        position = TransformSequencePointToAnchor(position, sequence.eyeAnchorPosition, sequence.eyeAnchorRotation, (flags & ASF_INVERT_OBJECT_ROT_1) != 0);
    if ((flags & ASF_SWAP_XZ_AND_ROT_Y_1) != 0)
        ConvertSequencePointToPolar(position);
    if ((flags & ASF_USE_OBJECT_POS_ROT_2) != 0)
        target = TransformSequencePointToAnchor(target, sequence.targetAnchorPosition, sequence.targetAnchorRotation, (flags & ASF_INVERT_OBJECT_ROT_2) != 0);
    if ((flags & ASF_SWAP_XZ_AND_ROT_Y_2) != 0)
        ConvertSequencePointToPolar(target);
    CurrentFrameCount = 0;

    float *const positionValues[3]{&position.x, &position.y, &position.z};
    float *const targetValues[3]{&target.x, &target.y, &target.z};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        FcvSplineReader &eyeReader = sequence.eyeReaders[axis];
        if ((flags & ASF_USE_CURRENT_POSITION_1) != 0 || eyeReader.keyCount == 0)
            eyeReader.values[0] = *positionValues[axis];
        if (eyeReader.keyCount <= 0)
            eyeReader.segmentKeyCount = 1;
        else
            AllocSequenceSegment(eyeReader);

        FcvSplineReader &targetReader = sequence.targetReaders[axis];
        if ((flags & ASF_USE_CURRENT_POSITION_2) != 0 || targetReader.keyCount == 0) {
            g_tmp_cam_fstep_0.targetYaw = 0;
            g_tmp_cam_fstep_0.targetPitch = 0;
            g_cam_target_orbit_state.targetYaw = 0;
            g_cam_target_orbit_state.targetPitch = 0;
            g_tmp_cam_fstep_0.orbit.z = 0.0f;
            g_tmp_cam_fstep_0.orbit.w = 0.0f;
            g_cam_target_orbit_state.orbit.z = 0.0f;
            g_cam_target_orbit_state.orbit.w = 0.0f;
            targetReader.values[0] = *targetValues[axis];
            g_look_around_angle_x = 0;
            g_look_around_angle_y = 0;
            g_playerCameraAngles_0 = 0;
            g_playerCameraAngles_1 = 0;
        }
        if (targetReader.keyCount <= 0)
            targetReader.segmentKeyCount = 1;
        else
            AllocSequenceSegment(targetReader);
    }
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const FcvSplineReader &eyeReader = sequence.eyeReaders[axis];
        const FcvSplineReader &targetReader = sequence.targetReaders[axis];
        HermiteSpline_InterpolateFCVKey(positionValues[axis], 0.0f, eyeReader.segmentKeyCount, eyeReader.times + eyeReader.keyOffset,
            eyeReader.values + eyeReader.keyOffset, eyeReader.coefficients);
        HermiteSpline_InterpolateFCVKey(targetValues[axis], 0.0f, targetReader.segmentKeyCount, targetReader.times + targetReader.keyOffset,
            targetReader.values + targetReader.keyOffset, targetReader.coefficients);
    }
    ApplySequenceFlags(position, target);

    if ((flags & ASF_USE_ZANGLE_ANIMATION) != 0) {
        FcvSplineReader &reader = sequence.zAngleReader;
        if ((flags & ASF_USE_ZANGLE_MODIFIER) != 0 || reader.keyCount == 0)
            reader.values[0] = static_cast<float>(static_cast<std::uint16_t>(ZANGLE)) / kSequenceFixedPerDegree;
        if (reader.keyCount <= 0)
            reader.segmentKeyCount = 1;
        else
            AllocSequenceSegment(reader);
        float zAngle = 0.0f;
        HermiteSpline_InterpolateFCVKey(
            &zAngle, 0.0f, reader.segmentKeyCount, reader.times + reader.keyOffset, reader.values + reader.keyOffset, reader.coefficients);
        if (!bLockCamera)
            ZANGLE = static_cast<std::uint16_t>(static_cast<std::int32_t>(zAngle * 8192.0f / 45.0f));
    }
    if ((flags & ASF_USE_FOV_ANIMATION) != 0) {
        FcvSplineReader &reader = sequence.fovReader;
        if ((flags & ASF_USE_CURRENT_PERSPECTIVE) != 0)
            reader.values[0] = static_cast<float>(currentPerspectiveFov) / kSequenceFixedPerDegree;
        if (reader.keyCount <= 0) {
            reader.values[0] = static_cast<float>(FieldOfView) / kSequenceFixedPerDegree;
            reader.segmentKeyCount = 1;
        } else {
            AllocSequenceSegment(reader);
        }
    }
    TranslateCameraRelative(position);
    if (!bLockCamera) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    g_cameraRuntime.viewReady = true;
    g_ASEQ_busy_flag = 1;
}

void CAME_SetEventCameraInput(void *chunk, Character *target, Character *eye) {
    ASEQ_current_chunk_ptr = static_cast<std::uint32_t *>(chunk);
    g_pCEWP = target;
    g_pASEQ_charas[0] = eye;
}



void AllocSPLI_ForCamera(std::int32_t frame) {
    if (g_nAuthorState != 6 && g_nAuthorState != 8)
        return;
    CurrentFrameCount = 2 * (frame / 2);
    const float time = static_cast<float>(static_cast<double>(CurrentFrameCount) * (1.0 / 60.0));
    AnimatedCameraSequence &sequence = g_animatedCameraSequence;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        SeekSequenceReader(sequence.eyeReaders[axis], time);
        SeekSequenceReader(sequence.targetReaders[axis], time);
    }
    if ((sequence.flags & ASF_USE_ZANGLE_ANIMATION) != 0)
        SeekSequenceReader(sequence.zAngleReader, time);
    if ((sequence.flags & ASF_USE_FOV_ANIMATION) != 0)
        SeekSequenceReader(sequence.fovReader, time);
}



std::uint32_t EventCam_Spline_ReadFCVKey(std::int32_t sequenceIndex, Character *eyeOwner, Character *targetOwner, Character *character) {
    authUpdate(sequenceIndex, eyeOwner, targetOwner);
    AnimatedCameraSequence &sequence = g_animatedCameraSequence;
    if (!sequence.chunk) {
        g_ASEQ_busy_flag = 0;
        return 0;
    }
    Vector3 characterPosition{};
    if (character)
        characterPosition = GetPositionForEntity(character->entity.node);
    AllocSPLI_ForCamera(0);
    Vector3 position{};
    Vector3 target{};
    float *const positionValues[3]{&position.x, &position.y, &position.z};
    float *const targetValues[3]{&target.x, &target.y, &target.z};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        Sequence_ReadFCVKey(&sequence.eyeReaders[axis], 0.0f, positionValues[axis]);
        Sequence_ReadFCVKey(&sequence.targetReaders[axis], 0.0f, targetValues[axis]);
    }
    ApplySequenceFlags(position, target);
    Vector3 previous = position;
    std::uint32_t result = 1;
    for (std::int32_t frame = 15;; frame += 15) {
        bool keysPending = false;
        AllocSPLI_ForCamera(frame);
        const float time = static_cast<float>(static_cast<double>(frame) * (1.0 / 60.0));
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (Sequence_ReadFCVKey(&sequence.eyeReaders[axis], time, positionValues[axis]) > 0)
                keysPending = true;
            if (Sequence_ReadFCVKey(&sequence.targetReaders[axis], time, targetValues[axis]) > 0)
                keysPending = true;
        }
        ApplySequenceFlags(position, target);
        if (EventCameraPathBlocked(previous, position) || (character && EventCameraPathBlocked(characterPosition, position)))
            break;
        previous = position;
        if (!keysPending) {
            result = 0;
            break;
        }
    }
    g_ASEQ_busy_flag = 0;
    return result;
}

void CAME_SetSequencePlayRoot(const Vector3 &position, const Vector3i &rotation) {
    g_tmp_seqdata_pos = position;
    g_tmp_seqdata_rot = rotation;
}

std::int32_t CAME_GetCurrentFrameCount() {
    return CurrentFrameCount;
}

std::int32_t CAME_GetInterpolationActive() {
    return isCameraInterpolationActive;
}

std::int32_t CAME_GetAnimatedSequenceBusy() {
    return g_ASEQ_busy_flag;
}

namespace {

const std::byte *CameraTaskPointer(std::uint32_t rawOffset) {
    const std::int32_t offset = static_cast<std::int32_t>(rawOffset);
    return offset && g_pCurrentTaskData ? g_pCurrentTaskData + offset : nullptr;
}

bool CamCtrlTargetPathCollides() {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!ryo)
        return false;

    const Vector3 ryoPosition = GetPositionForEntity(ryo->entity.node);
    std::array<std::uint8_t, 0x20> collisionMask{};
    std::memcpy(collisionMask.data(), control->cycleMask, collisionMask.size());
    collisionMask[1] &= 0xfcU;

    CharacterVectorCollisionResult result{};
    COLS_TraceMapVectorWithMask(g_CameraEyePos, ryoPosition, 0.0f, result, collisionMask.data());
    return (result.flags & 7U) != 0;
}

} // namespace




void Script_shenSetCameraPosAndTarget(ScenarioContext *context, void *arguments) {
    const auto *const params = static_cast<const std::uint32_t *>(arguments);
    if (!params)
        return;
    const bool scenario = context && context->task;
    Character *const eyeOwner = static_cast<std::int32_t>(params[0]) != -1 && params[1] && scenario ? CHID_Find(context->task->task, params[1]) : nullptr;
    Character *const targetOwner = static_cast<std::int32_t>(params[1]) != -1 && params[1] && scenario ? CHID_Find(context->task->task, params[1]) : nullptr;
    const Vector3 eye = g_CameraEyePos;
    g_cameraEyeNode.child = eyeOwner ? &eyeOwner->entity.node : nullptr;
    TranslateCameraRelative(eye);
    g_cameraTargetNode.child = targetOwner ? &targetOwner->entity.node : nullptr;
    if (!bLockCamera)
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
}

void Script_ScreenToWorldAssign(ScenarioContext *, void *arguments) {
    const auto *const params = static_cast<const std::uint32_t *>(arguments);
    if (!params)
        return;
    Matrix4x4_RotateAndPush();
    const auto *const world = reinterpret_cast<const Vector3 *>(CameraTaskPointer(params[0]));
    auto *const screen = reinterpret_cast<Vector2 *>(const_cast<std::byte *>(CameraTaskPointer(params[1])));
    if (world && screen)
        ConstructWorldSpaceFromScreen(*world, *screen);
}

void Script_CamCtrl(ScenarioContext *context, void *arguments) {
    const auto *const params = static_cast<const std::uint32_t *>(arguments);
    if (!params)
        return;
    switch (params[0]) {
    case 0:
        g_tmp_interpolated_camrot = {};
        g_tmp_cam_rot_z_adj = 0;
        g_script_cam_offs = {1.0f, 1.0f, 1.0f};
        g_script_cam_offs2 = {1.0f, 1.0f, 1.0f};
        break;
    case 1:
        g_tmp_interpolated_camrot = {
            std::bit_cast<float>(params[1]),
            std::bit_cast<float>(params[2]),
            std::bit_cast<float>(params[3]),
        };
        break;
    case 2:
        g_tmp_cam_rot_z_adj = static_cast<std::int32_t>(params[2]);
        break;
    case 3:
        g_script_cam_offs = {
            static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(params[1]))) / 100.0),
            static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(params[2]))) / 100.0),
            static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(params[3]))) / 100.0),
        };
        break;
    case 4:
        g_script_cam_offs2 = {
            static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(params[1]))) / 100.0),
            static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(params[2]))) / 100.0),
            static_cast<float>(static_cast<double>(static_cast<float>(static_cast<std::int32_t>(params[3]))) / 100.0),
        };
        break;
    case 5:
        if (context && context->stack) {
            context->stack->accumulator = CamCtrlTargetPathCollides() ? UINT32_MAX : 0;
        }
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
