#include "s2/camera.hpp"

#include "d3t/dx11_renderer.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/field_collision.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/hlib.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/mt7.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/search.hpp"
#include "s2/fight_stage.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/flag.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/ui.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/pause.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
// @verified - p
float camCtrlGetDeltaTime(float amount) noexcept {
    if (g_stabilizeFps) {
        const float scaled = (amount * 30.0f) * g_stabilizedFrameTime;
        return (amount - scaled) * 0.70700002f + scaled;
    }
    return amount;
}

float g_projectionNearClip{};
float g_projectionFarClip{};
std::uint32_t g_combatCameraFlags{};

namespace {
CameraMotionMode cameraMotionMode{};
float cameraMotionPlayerAlpha{};
float cameraMotionDistance{}, cameraMotionDoubleDistance{}, cameraMotionDistanceSquared{};
float cameraMotionDoubleDistanceSquared{}, cameraMotionSecondaryDistance{}, cameraMotionSecondaryDoubleDistance{};
std::uint8_t cameraMotionPatternSet{};
std::uint8_t fightCameraLinkedKind{};
Vector3 fightCameraEye{}, fightCameraTarget{}, fightCameraDesiredEye{}, fightCameraDesiredTarget{};
Vector3 fightCameraEyeDirection{}, fightCameraTargetDirection{}, fightCameraOffset{}, fightCameraGoalOffset{};
std::int32_t fightCameraFocusFrames{}, fightCameraSpeedDown{}, fightCameraSpeedUp{}, fightCameraGoalLift{};
std::int32_t fightCameraRayHit{}, fightCameraSide{}, fightCameraLinkedSide{}, fightCameraLinkedFrames{};
std::uint32_t fightCameraPatternFlags{}, fightCameraPatternState{};
bool fightCameraHasIntersectionTest{};
Character *fightCameraLinkedCharacter{};
VFActor *fightCameraLinkedActor{};
float fightCameraDistanceSetting{}, fightCameraHeightSetting{}, fightCameraCurrentSpeed{}, fightCameraHeightOffset{};
Vector4 fightCameraVelocity{};
float fightCameraVelocityDivisor{};
std::int32_t fightCameraVelocityCloseEnemy{};
float fightCameraPatternFrame{}, fightCameraPreviousPatternFrame{}, fightCameraPatternPhaseX{}, fightCameraPatternPhaseY{};
float fightCameraPatternSpeed{};
std::int32_t fightCameraPatternIndex{}, fightCameraPreviousPatternIndex{}, fightCameraPatternVariant{}, fightCameraPreviousPatternVariant{};
std::int32_t fightCameraRoll{}, fightCameraGoalRoll{};
Character *fightMotionCameraTarget{};
Vector3 fightMotionCameraSavedEye{}, fightMotionCameraSavedTarget{}, fightMotionCameraSegmentTarget{};
Vector3 fightMotionCameraWaypoints[6]{};
std::int32_t fightMotionCameraWaypointCount{}, fightMotionCameraWaypointIndex{}, fightMotionCameraState{}, fightMotionCameraUpdateState{};
float fightMotionCameraTime{};
std::uint16_t fightMotionCameraPreviousPerspective{};
bool fightMotionCameraInitializeEye{};
}

// 526010 / 526930: actual field/fight camera mode and its distance/pattern owner.
CameraMotionMode camCtrlGetInterpMode() noexcept {
    return cameraMotionMode;
}
void camCtrlSetInterpModeTo(CameraMotionMode mode) {
    const auto player = [&] {
        const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        return static_cast<Character *>(control->ryo);
    };
    if (cameraMotionMode == CameraMotionMode::FirstPerson)
        player()->alpha = 1;
    cameraMotionMode = mode;
    if (mode == CameraMotionMode::FieldFight) {
        cameraMotionDistance = 7.5f;
        cameraMotionSecondaryDistance = 6;
    } else if (mode == CameraMotionMode::FirstPerson) {
        cameraMotionDistance = cameraMotionSecondaryDistance = 1000;
        cameraMotionPlayerAlpha = std::max(0.30000001f, player()->alpha);
    } else {
        cameraMotionDistance = 2.5f;
        cameraMotionSecondaryDistance = 2;
    }
    cameraMotionDoubleDistance = cameraMotionDistance * 2;
    cameraMotionDistanceSquared = cameraMotionDistance * cameraMotionDistance;
    cameraMotionDoubleDistanceSquared = cameraMotionDoubleDistance * cameraMotionDoubleDistance;
    cameraMotionSecondaryDoubleDistance = cameraMotionSecondaryDistance * 2;
    cameraMotionPatternSet = 0;
    switch (g_currentMap.name) {
    case SHEN_NAME(Q100):
    case SHEN_NAME(Q300):
    case SHEN_NAME(AB00):
    case SHEN_NAME(QTB1):
    case SHEN_NAME(QSFA):
    case SHEN_NAME(QSFB):
    case SHEN_NAME(QSFD):
    case SHEN_NAME(QGBT):
    case SHEN_NAME(QJBT):
        cameraMotionPatternSet = 1;
        break;
    case SHEN_NAME(QLBT):
        cameraMotionPatternSet = 3;
        break;
    case SHEN_NAME(QSFC):
        cameraMotionPatternSet = 4;
        break;
    default:
        break;
    }
}
// 524ef0: the retained fight eye/target, ray mask and easing histories share
// one owner with the field/fight goal solver.
void resetIntersectionAndTest() {
    camCtrlGetNextCamPos(&fightCameraEye);
    camCtrlGetTargetPosition(&fightCameraTarget);
    if ((static_cast<std::uint8_t>(cameraMotionMode) <= 1 || cameraMotionMode == CameraMotionMode::Environment) && g_currentMap.name != SHEN_NAME(QKBT)) {
        Vector3 intersection;
        flddToggleFlag(2054);
        if (FindNearestRayIntersection(&intersection, &fightCameraEye, &fightCameraTarget))
            fightCameraEye = intersection;
    }
    camCtrlSetFovTransitionTo(true);
    camCtrlSetPerspective(camCtrlGetPerspective());
    fightCameraSide = 1;
    fightCameraFocusFrames = fightCameraSpeedDown = fightCameraSpeedUp = fightCameraGoalLift = 0;
    fightCameraRayHit = 0;
    fightCameraGoalOffset = {};
    fightCameraOffset = {};
    fightCameraEyeDirection = {};
    fightCameraTargetDirection = {};
    fightCameraLinkedKind = 0;
    fightCameraLinkedCharacter = nullptr;
    fightCameraLinkedActor = nullptr;
    fightCameraPatternFrame = fightCameraPreviousPatternFrame = 0;
    fightCameraPatternFlags = fightCameraPatternState = 0;
    fightCameraPatternPhaseX = fightCameraPatternPhaseY = 0;
    fightCameraRoll = fightCameraGoalRoll = 0;
    fightCameraPreviousPatternIndex = fightCameraPatternIndex = 0;
    fightCameraPatternSpeed = 160.0f / (static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f);
}


void resetCharaPosYandIntersection() {
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (auto *const player = static_cast<Character *>(control->ryo))
        player->alpha = 1;
    resetIntersectionAndTest();
}



void vfSetupLinkedFightCamera(VFActor *linked, std::uint8_t kind) {
    if (kind < 4)
        fightCameraLinkedKind = kind;
    fightCameraLinkedCharacter = linked->character;
    fightCameraLinkedActor = linked;
    VFMA_t *const game = linked->game;
    if (static_cast<std::uint8_t>(kind - 1) <= 2) {
        fightCameraLinkedSide = fightCameraSide;
        fightCameraLinkedFrames = 0;
    }
    if (cameraMotionMode != CameraMotionMode::AutoQuadratic2)
        game->mode |= 1;
    const float time = linked->groupIndex == game->playerSlot ? 1.0f : 0.2f;
    std::int8_t direction = 1;
    if (fightCameraLinkedKind == 3) {
        SetBlurModel(linked->character, nullptr, time);
        EV_figpAssignFlagsFor(linked->character, 0, 1);
        direction = 2;
    } else {
        if (cameraMotionMode == CameraMotionMode::AutoQuadratic || cameraMotionMode == CameraMotionMode::AutoQuadratic2)
            SetMotionEnvironmentEnabled(0);
        else
            SetMotionEnvironmentEnabled(linked->motion, 0);
        if (linked->groupIndex != game->playerSlot)
            SetBlurModel(linked->character, nullptr, time);
        EV_figpAssignFlagsFor(linked->character, 0, 1);
    }
    figpAssignMotScaleAndDirFor(linked->character, 5, direction);
    figpAssignModeForCharacter(linked->character, 1);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (!slot.character || slot.character == fightCameraLinkedCharacter)
            continue;
        if (shenCheckCharaHasAlphaManager(slot.character))
            slot.savedControlFlags |= 0x80000000;
        else
            slot.savedControlFlags &= ~0x80000000U;
        cpamSetFlag(slot.character, 1);
    }
    fightCameraHasIntersectionTest = true;
}


void vfUnknownCamRelated(VFMA_t *) {
    if ((cameraMotionMode != CameraMotionMode::AutoQuadratic && cameraMotionMode != CameraMotionMode::AutoQuadratic2) || !fightCameraLinkedCharacter)
        return;
    figpAssignModeForCharacter(fightCameraLinkedCharacter, 0);
    EV_figpAssignFlagsFor(fightCameraLinkedCharacter, 0, 0);
    SetMotionEnvironmentEnabled(1);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        FCMG_t &slot = g_pFightStage->enemySlots[i];
        if (slot.character && slot.character != fightCameraLinkedCharacter && !(slot.savedControlFlags & 0x80000000))
            cpamSetFlag(slot.character, 0);
    }
    camCtrlSetInterpModeTo(CameraMotionMode::Default);
    resetIntersectionAndTest();
}

void camCtrlSetFightDistance(float distance) {
    fightCameraDistanceSetting = distance;
}
void camCtrlSetFightHeight(float height) {
    fightCameraHeightSetting = height;
}



void ResetMotionCameraWaypoints() {
    fightMotionCameraWaypointCount = 0;
}
void AppendMotionCameraWaypoint(const Vector3 &point) {
    if (fightMotionCameraWaypointCount < 5)
        fightMotionCameraWaypoints[fightMotionCameraWaypointCount++] = point;
}
void BeginMotionInterpolationCamera(Character *target) {
    using enum CameraState;
    fightMotionCameraTarget = target;
    if (target) {
        GetCharacterMotionWorldPosition(target, 5, &fightMotionCameraWaypoints[fightMotionCameraWaypointCount]);
        ++fightMotionCameraWaypointCount;
    }
    SetPlayerControlEnabled(0);
    camCtrlSetInterpModeTo(CameraMotionMode::MotionInterp);
    resetIntersectionAndTest();
    camCtrlSetFovTransitionTo(false);
    camCtrlSetState(ChaseGoal);
    fightMotionCameraSavedEye = fightCameraEye;
    fightMotionCameraSavedTarget = fightCameraTarget;
    fightMotionCameraState = 0;
    fightMotionCameraWaypointIndex = 0;
    fightMotionCameraInitializeEye = true;
}

// 526070: the real player MTWK receives F01B; no synthetic camera actor.
bool BeginMotionUpdateCamera(Character *target) {
    using enum CameraState;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    auto *const motion = FindMotionWork(static_cast<Character *>(control->ryo));
    fightMotionCameraTarget = target;
    SetPlayerControlEnabled(0);
    camCtrlSetInterpModeTo(CameraMotionMode::MotionUpdate);
    resetIntersectionAndTest();
    camCtrlSetFovTransitionTo(false);
    camCtrlSetState(ChaseGoal);
    fightMotionCameraSavedEye = fightCameraEye;
    fightMotionCameraSavedTarget = fightCameraTarget;
    const bool result = mtwkRequestMotion(motion, 0xf01b, MotionRegion::LowerBody);
    fightMotionCameraUpdateState = 0;
    return result;
}

// @verified - p
std::uint32_t EV_camCtrlToggleCombatCamera(std::uint32_t mask, std::int32_t action) {
    const std::uint32_t previous = g_combatCameraFlags & mask;
    if (action > 0)
        g_combatCameraFlags |= mask;
    else if (action == 0)
        g_combatCameraFlags &= ~mask;
    return previous;
}

namespace {
float FightCameraLength(const Vector3 &v) {
    return std::sqrt((v.x * v.x + v.y * v.y) + v.z * v.z);
}
void ScaleFightCameraVector(Vector3 &v, float magnitude) {
    const float scale = magnitude / FightCameraLength(v);
    v.x *= scale;
    v.y *= scale;
    v.z *= scale;
}
void ClampFightCameraPoint(Vector3 &point, const Vector3 &centre) {
    constexpr float radii[3]{0.050000001f, 0.1f, 0.050000001f};
    for (std::uint8_t axis = 0; axis < 3; ++axis) {
        if (centre[axis] - radii[axis] <= point[axis]) {
            if (point[axis] > centre[axis] + radii[axis])
                point[axis] = centre[axis] + radii[axis];
        } else
            point[axis] = centre[axis] - radii[axis];
    }
}
Vector3 RotateFightCameraPoint(const Vector3 &relative, const Vector3 &centre, std::uint16_t yaw) {
    Vector3 output;
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(centre.x, centre.y, centre.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::TransformPoint(relative, output);
    Matrix4x4::Pop();
    return output;
}
bool FightCameraRaycast(Vector3 &result, const Vector3 &destination, const Vector3 &origin) {
    if (g_currentMap.name == SHEN_NAME(QKBT)) {
        result = destination;
        return false;
    }
    flddToggleFlag(2054);
    return FindNearestRayIntersection(&result, &destination, &origin);
}



void AccumulateFightCameraVelocity(VFMA_t *game, VFActor *self) {
    Vector4 sum{};
    float divisor = 1;
    std::int32_t closeEnemy = 0;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i] || i == game->playerSlot)
            continue;
        VFActor &other = *game->actors[i];
        const float x = other.contactPosition.x - self->contactPosition.x;
        const float z = other.contactPosition.z - self->contactPosition.z;
        const float y = other.targetGroundY - self->targetGroundY;
        const float vertical = std::fabs(y), distanceSquared = z * z + x * x;
        if (cameraMotionDoubleDistanceSquared <= distanceSquared || cameraMotionSecondaryDoubleDistance <= vertical)
            continue;
        const float distance = std::sqrt(distanceSquared);
        float weight = 1;
        if (distance > cameraMotionDistance)
            weight = (cameraMotionDoubleDistance - distance) / cameraMotionDistance;
        if (vertical > cameraMotionSecondaryDistance)
            weight *= (cameraMotionSecondaryDoubleDistance - vertical) / cameraMotionSecondaryDistance;
        const bool enemy = (static_cast<std::uint8_t>(game->slotTeams[i] - 1) & 0xfd) == 0;
        if (!enemy)
            weight *= 0.5f;
        divisor += weight;
        sum.y += (weight * y) * weight;
        sum.x += (weight * x) * weight;
        sum.w += (weight * distance) * weight;
        sum.z += (weight * z) * weight;
        if (cameraMotionDistance * 1.4f > distance && enemy)
            closeEnemy = 1;
    }
    if (std::fabs(divisor) >= 0.00000011920929f) {
        sum.x /= divisor;
        sum.y /= divisor;
        sum.z /= divisor;
    }
    fightCameraVelocity = sum;
    fightCameraVelocityDivisor = divisor;
    fightCameraVelocityCloseEnemy = closeEnemy;
}


void UpdateFightCameraPattern(VFMA_t *game) {
    if (static_cast<std::uint32_t>(game->state) > 3 || static_cast<std::int32_t>(game->frame) < 60) {
        g_cameraControl->nextRotation.roll = 0;
        return;
    }
    if (fightCameraLinkedKind != 0 || static_cast<std::int32_t>(game->mode) < 0)
        return;
    float perspective = static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f;
    if (perspective < 60)
        perspective -= (60 - perspective) * 0.5f;
    const float speedDivisor = perspective >= 10 ? std::fmin(90.0f, perspective) : 10;
    constexpr std::uint8_t patterns[5][8]{{0, 0, 0, 2, 3, 3, 4, 5}, {0, 0, 0, 2, 2, 2, 4, 5},
        {0,0,0,2,3,4,4,5},{0,0,2,2,5,5,6,7},{0,0,2,2,2,2,4,4}};
    constexpr float phaseTargets[8][2]{{0, 0}, {-0.2f, 0.2f}, {-0.30000001f, 0.1f}, {0, 0.5f},
        {1.5f,0},{2,0.5f},{3.5f,0.5f},{6,0.5f}};
    constexpr float speeds[5][3]{{80, 160, 240}, {80, 120, 160}, {80, 160, 240}, {80, 180, 300}, {80, 140, 200}};
    const auto frame = static_cast<std::int32_t>(fightCameraPatternFrame);
    const auto previous = static_cast<std::int32_t>(fightCameraPreviousPatternFrame);
    if (frame == 90 * (frame / 90) && previous != 90 * (previous / 90)) {
        fightCameraPreviousPatternIndex = fightCameraPatternIndex;
        fightCameraPatternIndex = patterns[cameraMotionPatternSet][static_cast<std::int32_t>(IncrementSharedPrngFloat() * 8)];
        fightCameraPreviousPatternVariant = fightCameraPatternVariant;
        fightCameraPatternVariant = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 4) % 2;
        if (fightCameraPatternIndex == 1 || static_cast<std::uint32_t>(fightCameraPatternIndex - 6) <= 1)
            fightCameraPatternVariant = 0;
    }
    if (fightCameraPatternIndex != fightCameraPreviousPatternIndex) {
        fightCameraPatternPhaseX += std::clamp(phaseTargets[fightCameraPatternIndex][0] - fightCameraPatternPhaseX, -0.25f, 0.25f);
        fightCameraPatternPhaseY += std::clamp(phaseTargets[fightCameraPatternIndex][1] - fightCameraPatternPhaseY, -0.25f, 0.25f);
    }
    if (fightCameraPatternVariant != fightCameraPreviousPatternVariant)
        fightCameraPatternSpeed +=
            std::clamp(std::fmin(9.5f, speeds[cameraMotionPatternSet][fightCameraPatternVariant] / speedDivisor) - fightCameraPatternSpeed, -0.25f, 0.25f);
    if (fightCameraGoalRoll != fightCameraRoll) {
        if (fightCameraRoll > fightCameraGoalRoll + 64)
            fightCameraRoll -= 64;
        else if (fightCameraRoll < fightCameraGoalRoll - 64)
            fightCameraRoll += 64;
        g_cameraControl->nextRotation.roll = static_cast<std::int16_t>(fightCameraRoll);
    }
    fightCameraPreviousPatternFrame = fightCameraPatternFrame;
    fightCameraPatternFrame += shenGetScaledDeltaTickTime(1);
}



void UpdateFieldFightCamera(VFActor *self, VFActor *other) {
    Vector3 delta{other->contactPosition.x - self->contactPosition.x, (other->targetGroundY + 1) - (self->targetGroundY + 1),
        other->contactPosition.z - self->contactPosition.z};
    fightCameraDesiredTarget = {self->contactPosition.x + delta.x * 0.5f, self->targetGroundY + 1 + delta.y * 0.5f, self->contactPosition.z + delta.z * 0.5f};
    const float separation = FightCameraLength(delta);
    const float distance = std::fmax(2.6600001f, separation * 1.3f);
    const auto currentYaw =
        static_cast<std::int16_t>(FixedAtan2(fightCameraDesiredTarget.x - fightCameraDesiredEye.x, fightCameraDesiredTarget.z - fightCameraDesiredEye.z));
    auto desiredYaw =
        static_cast<std::int16_t>(FixedAtan2(self->contactPosition.x - other->contactPosition.x, self->contactPosition.z - other->contactPosition.z) - 0x4000);
    if (separation > 5)
        desiredYaw = static_cast<std::int16_t>(desiredYaw + static_cast<std::int32_t>((separation <= 10 ? (separation - 5) * 18 : 90) * 182));
    auto difference = static_cast<std::int16_t>(desiredYaw - currentYaw);
    if (static_cast<std::int16_t>(difference - 0x4000) >= 0)
        difference = static_cast<std::int16_t>(difference + 0x8000);
    const auto outside = difference > 2048 ? difference - 2048 : difference < -2048 ? difference + 2048 : 0;
    const auto turn = static_cast<std::uint16_t>(std::clamp(outside / 4, -2048, 2048));
    delta = {fightCameraDesiredEye.x - fightCameraDesiredTarget.x, fightCameraDesiredEye.y - fightCameraDesiredTarget.y,
        fightCameraDesiredEye.z - fightCameraDesiredTarget.z};
    ScaleFightCameraVector(delta, distance);
    Vector3 eye = RotateFightCameraPoint(delta, fightCameraDesiredTarget, turn);
    eye.y = fightCameraDesiredTarget.y;
    if (g_currentMap.name != SHEN_NAME(QKBT)) {
        if (FightCameraRaycast(eye, eye, fightCameraDesiredTarget)) {
            const float x = eye.x - fightCameraDesiredTarget.x, z = eye.z - fightCameraDesiredTarget.z;
            eye.y += std::sqrt(distance * distance - (z * z + x * x));
        }
    }
    fightCameraDesiredEye = eye;
}



void UpdateFirstPersonFightCamera(VFMA_t *game, VFActor *self, VFActor *other) {
    const bool followup = (self->activeMotionFlags & 0x8000000) || (self->groundContactFlags & 1);
    Vector3 selfBone{}, otherBone{};
    GetCharacterMotionWorldPosition(self->character, 3, &selfBone);
    GetCharacterMotionWorldPosition(other->character, 3, &otherBone);
    const float selfHeight = (1.35000002f - 1.2f) + (selfBone.y - self->targetGroundY);
    const float otherHeight = (otherBone.y - other->targetGroundY) + (1.3f - 1.2f);
    const float perspectiveScale = 60 / (static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f);
    if ((self->hitStateFlags & 0xc0000) || ((self->hitStateFlags & 0x10000) && !(self->hitStateFlags & 0x8000))) {
        EV_figpAssignFlagsFor(self->character, 0, 1);
        figpAssignModeForCharacter(self->character, 0);
        GetCharacterMotionWorldPosition(self->character, 5, &fightCameraDesiredEye);
        GetCharacterMotionWorldPosition(other->character, 5, &fightCameraDesiredTarget);
        Vector3 delta{fightCameraDesiredEye.x - fightCameraDesiredTarget.x, fightCameraDesiredEye.y - fightCameraDesiredTarget.y,
            fightCameraDesiredEye.z - fightCameraDesiredTarget.z};
        const float distance = perspectiveScale + FightCameraLength(delta);
        ScaleFightCameraVector(delta, perspectiveScale);
        fightCameraDesiredEye.x += delta.x;
        fightCameraDesiredEye.y += delta.y;
        fightCameraDesiredEye.z += delta.z;
        const float minimumY = self->targetGroundY + 0.15000001f;
        if (fightCameraDesiredTarget.y <= minimumY)
            fightCameraDesiredTarget.y = minimumY;
        if (minimumY > fightCameraDesiredEye.y) {
            const float y = fightCameraDesiredTarget.y - minimumY;
            delta.y = 0;
            ScaleFightCameraVector(delta, std::sqrt(distance * distance - y * y));
            fightCameraDesiredEye = {fightCameraDesiredTarget.x + delta.x, minimumY, fightCameraDesiredTarget.z + delta.z};
        }
        return;
    }
    Vector3 average{};
    std::int32_t count = 0;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i] || game->slotTeams[i] != 1 || self->slot->facingDifferences[i] >= 0x4000)
            continue;
        ++count;
        average.x += game->actors[i]->position.x;
        average.y += game->actors[i]->targetGroundY;
        average.z += game->actors[i]->position.z;
    }
    if (count) {
        const float divisor = static_cast<float>(count);
        average.x /= divisor;
        average.y /= divisor;
        average.z /= divisor;
    }
    fightCameraDesiredTarget = fightCameraTarget;
    fightCameraFocusFrames = std::clamp(fightCameraFocusFrames + (followup ? 1 : -1), 0, 64);
    const float blend = count ? std::clamp(static_cast<float>(fightCameraFocusFrames) * 0.015625f, 0.0f, 1.0f) : 1;
    Vector3 delta{fightCameraTarget.x - self->contactPosition.x, fightCameraTarget.y - self->targetGroundY, fightCameraTarget.z - self->contactPosition.z};
    const float distance = FightCameraLength(delta);
    const float headingX = self->contactPosition.x - sin_fixed16(self->temporaryFacingAngle) * distance;
    const float headingZ = self->contactPosition.z - cos_fixed16(self->temporaryFacingAngle) * distance;
    const Vector3 desired{(1 - blend) * average.x + blend * headingX, (1 - blend) * average.y + blend * self->targetGroundY + otherHeight,
        (1 - blend) * average.z + blend * headingZ};
    ClampFightCameraPoint(fightCameraDesiredTarget, desired);
    delta = {fightCameraDesiredTarget.x - self->contactPosition.x, (fightCameraDesiredTarget.y - self->targetGroundY) - selfHeight,
        fightCameraDesiredTarget.z - self->contactPosition.z};
    const float eyeDistance = FightCameraLength(delta);
    ScaleFightCameraVector(delta, eyeDistance + 0.80000001f * perspectiveScale);
    const Vector3 desiredEye{fightCameraDesiredTarget.x - delta.x, std::fmax(fightCameraDesiredTarget.y - delta.y, self->targetGroundY + 0.15000001f),
        fightCameraDesiredTarget.z - delta.z};
    ClampFightCameraPoint(fightCameraDesiredEye, desiredEye);
    const auto angle =
        static_cast<std::int16_t>(FixedAtan2(fightCameraDesiredTarget.x - fightCameraDesiredEye.x, fightCameraDesiredTarget.z - fightCameraDesiredEye.z));
    const auto difference = static_cast<std::int16_t>(self->temporaryFacingAngle - angle + 0x8000);
    const auto outside = difference > 2048 ? difference - 2048 : difference < -2048 ? difference + 2048 : 0;
    delta = {fightCameraDesiredEye.x - fightCameraDesiredTarget.x, fightCameraDesiredEye.y - fightCameraDesiredTarget.y,
        fightCameraDesiredEye.z - fightCameraDesiredTarget.z};
    fightCameraDesiredEye = RotateFightCameraPoint(delta, fightCameraDesiredTarget, static_cast<std::uint16_t>(std::clamp(outside / 4, -1152, 1152)));
    if (static_cast<std::uint32_t>(game->state) < 4)
        cameraMotionPlayerAlpha = std::fmax(0.30000001f, cameraMotionPlayerAlpha - 0.1f);
    else
        cameraMotionPlayerAlpha = std::fmin(1.0f, cameraMotionPlayerAlpha + 0.050000001f);
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    static_cast<Character *>(control->ryo)->alpha = cameraMotionPlayerAlpha;
}



void UpdateEnvironmentFightCamera(VFMA_t *game, VFActor *self, VFActor *other) {
    if (!game->characters[game->playerSlot])
        return;
    if (static_cast<std::int32_t>(self->moveAngleTableIndex) < 30 && !(game->mode & 1))
        UpdateFightCameraPattern(game);
    const bool focus = (self->hitStateFlags & 0x20000000) != 0;
    const bool reduceOffset = (static_cast<std::uint8_t>(game->type - 1) & 0xfb) == 0;
    const Vector3 position{self->contactPosition.x, self->targetGroundY, self->contactPosition.z};
    const Vector3 raised{position.x, position.y + 1, position.z};
    if (cameraMotionMode == CameraMotionMode::Default1) {
        fightCameraCurrentSpeed = fightCameraDistanceSetting;
        fightCameraHeightOffset = fightCameraHeightSetting;
    } else {
        fightCameraCurrentSpeed = 2.1768f;
        fightCameraHeightOffset = cameraMotionMode == CameraMotionMode::Environment ? 0.5f : 0;
    }
    if (static_cast<std::int32_t>(game->mode) >= 0) {
        fightCameraHeightOffset = 0;
        if (fightCameraPatternSpeed != 0)
            fightCameraCurrentSpeed = fightCameraPatternSpeed;
    }
    AccumulateFightCameraVelocity(game, self);
    fightCameraFocusFrames = std::clamp(fightCameraFocusFrames + (focus ? 1 : -1), 0, 64);
    const bool speedUp = fightCameraVelocityCloseEnemy == 0 || focus;
    fightCameraSpeedUp = std::clamp(fightCameraSpeedUp + (speedUp ? 1 : -1), 0, 64);
    fightCameraSpeedDown = std::clamp(fightCameraSpeedDown + (speedUp ? -1 : 1), 0, 64);
    Vector3 offset = position;
    if (std::fabs(fightCameraVelocityDivisor) >= 0.00000011920929f) {
        offset.x += fightCameraVelocity.x / fightCameraVelocityDivisor;
        offset.z += fightCameraVelocity.z / fightCameraVelocityDivisor;
        offset.y += fightCameraVelocity.y / fightCameraVelocityDivisor;
    }
    offset = {offset.x - position.x, offset.y - position.y, offset.z - position.z};
    if (reduceOffset) {
        offset.x *= 0.60000002f;
        offset.y *= 0.60000002f;
        offset.z *= 0.60000002f;
    }
    const float focusBlend = static_cast<float>(fightCameraFocusFrames) * 0.015625f;
    const float unfocused = 1 - focusBlend;
    const float blend = std::clamp(unfocused, 0.0f, 1.0f);
    const float headingX = -sin_fixed16(self->temporaryFacingAngle), headingZ = -cos_fixed16(self->temporaryFacingAngle);
    const Vector3 desiredOffset{
        (1 - blend) * headingX + blend * offset.x, blend * offset.y + (1 - blend) * offset.y, blend * offset.z + (1 - blend) * headingZ};
    float speed = static_cast<float>(fightCameraSpeedUp) * 0.015625f + fightCameraCurrentSpeed;
    if (std::fabs(fightCameraVelocityDivisor) >= 0.00000011920929f)
        speed = ((fightCameraVelocity.w / fightCameraVelocityDivisor) * 0.75f) * unfocused + fightCameraCurrentSpeed +
            static_cast<float>(fightCameraSpeedUp) * 0.015625f;
    if (static_cast<std::int32_t>(game->mode) < 0)
        speed *= 60 / (static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f);
    if (static_cast<std::uint32_t>(game->state) >= 4)
        speed = FightCameraLength({fightCameraEye.x - fightCameraTarget.x, fightCameraEye.y - fightCameraTarget.y, fightCameraEye.z - fightCameraTarget.z});
    for (std::uint8_t axis = 0; axis < 3; ++axis) {
        const float change = desiredOffset[axis] - fightCameraGoalOffset[axis];
        if (std::fabs(change) >= 0.00000011920929f)
            fightCameraGoalOffset[axis] += change * 0.33333334f;
    }
    Vector3 desiredTarget{fightCameraGoalOffset.x + position.x, ((fightCameraGoalOffset.y + position.y) + 0.83999997f) + fightCameraPatternPhaseY,
        fightCameraGoalOffset.z + position.z};
    if (static_cast<std::uint32_t>(game->state) >= 4)
        desiredTarget.y = fightCameraTarget.y;
    ClampFightCameraPoint(fightCameraDesiredTarget, desiredTarget);
    Vector3 collision;
    if (FightCameraRaycast(collision, fightCameraDesiredTarget, raised)) {
        fightCameraDesiredTarget.x = collision.x;
        fightCameraDesiredTarget.z = collision.z;
    }
    Vector3 orbit{fightCameraEye.x - fightCameraTarget.x, 0, fightCameraEye.z - fightCameraTarget.z};
    const float orbitLength = FightCameraLength(orbit);
    if (std::fabs(orbitLength) < 0.00000011920929f)
        orbit = {sin_fixed16(self->temporaryFacingAngle), 0, cos_fixed16(self->temporaryFacingAngle)};
    else {
        orbit.x /= orbitLength;
        orbit.z /= orbitLength;
    }
    if (focus) {
        const auto angle = static_cast<std::int16_t>(FixedAtan2(-orbit.x, -orbit.z));
        const auto difference = static_cast<std::int16_t>(self->temporaryFacingAngle - angle + 0x8000);
        const auto outside = difference > 2048 ? difference - 2048 : difference < -2048 ? difference + 2048 : 0;
        const auto yaw = static_cast<std::uint16_t>(angle + std::clamp(outside / 4, -2048, 2048));
        orbit = {-sin_fixed16(yaw), 0, -cos_fixed16(yaw)};
    }
    const float radius = std::fmin(9.0f, speed);
    const Vector3 desiredEye{orbit.x * radius + fightCameraDesiredTarget.x,
        std::fmax((fightCameraDesiredTarget.y + 0.44000003f - (static_cast<float>(fightCameraSpeedDown) * 0.015625f) * 0.80000001f) +
                (fightCameraPatternPhaseX - fightCameraPatternPhaseY),
            (self->targetGroundY + 0.050000001f) + fightCameraHeightOffset),
        orbit.z * radius + fightCameraDesiredTarget.z};
    ClampFightCameraPoint(fightCameraDesiredEye, desiredEye);
    if (other && !focus) {
        const Vector3 originalEye = fightCameraDesiredEye;
        const auto current = static_cast<std::int16_t>(FixedAtan2(fightCameraDesiredTarget.x - originalEye.x, fightCameraDesiredTarget.z - originalEye.z));
        const auto contactAngle = static_cast<std::int16_t>(FixedAtan2(position.x - other->contactPosition.x, position.z - other->contactPosition.z));
        fightCameraSide = static_cast<std::int16_t>(contactAngle - current) > 0 ? 1 : -1;
        const float x = other->contactPosition.x - self->contactPosition.x;
        const float y = other->targetGroundY - self->targetGroundY;
        const float z = other->contactPosition.z - self->contactPosition.z;
        if (cameraMotionDistanceSquared > (y * y + x * x) + z * z) {
            auto side = static_cast<std::int16_t>(contactAngle + 0x4000);
            const auto back = static_cast<std::int16_t>(contactAngle + 0x8000);
            if (!(fightCameraPatternFlags & 2) && std::abs(static_cast<std::int32_t>(side) - current) > 0x4000)
                side = static_cast<std::int16_t>(contactAngle - 0x4000);
            std::uint16_t yaw = static_cast<std::uint16_t>(current);
            if (side == back) {
                if (std::abs(static_cast<std::int32_t>(side) - current) >= 5461)
                    yaw = static_cast<std::uint16_t>(side <= current ? side + 5461 : side - 5461);
            } else {
                const auto distance = std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(current - back)));
                if (distance < 8192)
                    yaw = static_cast<std::uint16_t>(side - back > 0 ? back + 8192 : back - 8192);
                else if (distance > 21845)
                    yaw = static_cast<std::uint16_t>(side - back > 0 ? back + 21845 : back - 21845);
            }
            const auto probeFor = [&](std::uint16_t angle) {
                return Vector3{fightCameraDesiredTarget.x - sin_fixed16(angle) * 3, originalEye.y, fightCameraDesiredTarget.z - cos_fixed16(angle) * 3};
            };
            Vector3 probe = probeFor(yaw);
            if (FightCameraRaycast(collision, probe, fightCameraDesiredTarget)) {
                yaw = static_cast<std::uint16_t>(yaw + 0x8000);
                probe = probeFor(yaw);
                if (FightCameraRaycast(collision, probe, fightCameraDesiredTarget))
                    yaw = static_cast<std::uint16_t>(current);
            }
            const auto difference = static_cast<std::int16_t>(yaw - current);
            const auto outside = difference > 2048 ? difference - 2048 : difference < -2048 ? difference + 2048 : 0;
            const auto turn = static_cast<std::uint16_t>(std::clamp(outside / 4, -2048, 2048));
            if (turn != 0) {
                const Vector3 delta{
                    originalEye.x - fightCameraDesiredTarget.x, originalEye.y - fightCameraDesiredTarget.y, originalEye.z - fightCameraDesiredTarget.z};
                const Vector3 turned = RotateFightCameraPoint(delta, fightCameraDesiredTarget, turn);
                if (!FightCameraRaycast(collision, turned, raised))
                    fightCameraDesiredEye = turned;
            }
        }
    }
    Vector3 resolvedEye = fightCameraDesiredEye;
    fightCameraRayHit = FightCameraRaycast(resolvedEye, fightCameraDesiredEye, raised) ? 1 : 0;
    if (fightCameraRayHit == 1) {
        if (!FightCameraRaycast(resolvedEye, fightCameraDesiredEye, fightCameraEye)) {
            const Vector3 relative{fightCameraDesiredEye.x - raised.x, fightCameraDesiredEye.y - raised.y, fightCameraDesiredEye.z - raised.z};
            const Vector3 positive = RotateFightCameraPoint(relative, raised, 8192);
            const Vector3 negative = RotateFightCameraPoint(relative, raised, 57344);
            if (!FightCameraRaycast(collision, positive, position))
                resolvedEye = positive;
            else {
                const float positiveDistance = FightCameraLength({collision.x - raised.x, collision.y - raised.y, collision.z - raised.z});
                if (FightCameraRaycast(collision, negative, position) &&
                    positiveDistance > FightCameraLength({collision.x - raised.x, collision.y - raised.y, collision.z - raised.z}))
                    resolvedEye = positive;
                else
                    resolvedEye = negative;
            }
        } else {
            fightCameraDesiredEye.x = resolvedEye.x;
            fightCameraDesiredEye.z = resolvedEye.z;
        }
        fightCameraGoalLift = std::min(64, fightCameraGoalLift + 1);
    } else
        fightCameraGoalLift = std::max(0, fightCameraGoalLift - 1);
    const float changeX = (resolvedEye.x - fightCameraDesiredEye.x) - fightCameraOffset.x;
    const float changeZ = (resolvedEye.z - fightCameraDesiredEye.z) - fightCameraOffset.z;
    if (std::fabs(changeX) >= 0.00000011920929f)
        fightCameraOffset.x += changeX * 0.33333334f;
    if (std::fabs(changeZ) >= 0.00000011920929f)
        fightCameraOffset.z += changeZ * 0.33333334f;
    fightCameraDesiredEye.x += fightCameraOffset.x;
    fightCameraDesiredEye.y += (static_cast<float>(fightCameraGoalLift) * 0.015625f) * 1.5f;
    fightCameraDesiredEye.z += fightCameraOffset.z;
}

// 6b8a90: camera/auth owner predicate, not an alternate actor or test view.
bool IsEarlyAuthFightCamera() {
    const auto *const game = vfmaGetTaskParam();
    return game && (game->mode & 0x80000) && game->parameter570 < 5;
}

void ResetFightCameraToParticipants(VFActor *self, VFActor *other) {
    if (!self || !other || self == other)
        return;
    fightCameraTarget = {self->contactPosition.x, self->targetGroundY + 1.4f, self->contactPosition.z};
    Vector3 direction{other->contactPosition.x - fightCameraTarget.x, 0, other->contactPosition.z - fightCameraTarget.z};
    ScaleFightCameraVector(direction, (60 / (static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f)) * 1.5f);
    fightCameraEye = {fightCameraTarget.x + direction.x, self->targetGroundY + 1.3f, fightCameraTarget.z + direction.z};
}



void UpdateQuadraticFightCamera(VFMA_t *game, VFActor *self, VFActor *other) {
    if (IsEarlyAuthFightCamera()) {
        fightCameraLinkedKind = 2;
        fightCameraLinkedCharacter = self->character;
        fightCameraLinkedActor = self;
    }
    if (static_cast<std::uint8_t>(fightCameraLinkedKind - 1) > 2)
        return;
    if (cameraMotionMode == CameraMotionMode::AutoQuadratic &&
        (!other || (other->hitStateFlags & 0x8000) || (game->state == 4 && GetBtlStatus(game->playerCharacter) == 3))) {
        vfUnknownCamRelated(game);
        ResetFightCameraToParticipants(self, other);
        return;
    }
    float x = self->contactPosition.x, z = self->contactPosition.z;
    if (IsEarlyAuthFightCamera()) {
        x = other->targetPosition.x;
        z = other->targetPosition.z;
    }
    const Vector3 linkedPosition = fightCameraLinkedActor->contactPosition;
    if (IsEarlyAuthFightCamera())
        fightCameraDesiredTarget = {other->targetPosition.x, linkedPosition.y, other->targetPosition.z};
    else
        ClampFightCameraPoint(fightCameraDesiredTarget, linkedPosition);
    float radius = 60 / (static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f);
    if (fightCameraLinkedKind != 3)
        radius += radius;
    Vector3 direction{fightCameraEye.x - fightCameraTarget.x, 0, fightCameraEye.z - fightCameraTarget.z};
    ScaleFightCameraVector(direction, radius);
    Vector3 desired{direction.x + fightCameraDesiredTarget.x, fightCameraDesiredEye.y, direction.z + fightCameraDesiredTarget.z};
    if (fightCameraLinkedKind == 1)
        desired.y = other->targetGroundY + 0.2f;
    else if (fightCameraLinkedKind == 2)
        desired.y = std::fmin(other->animationTime * 0.050000001f + (other->targetGroundY + 0.1f), other->targetGroundY + 2.75f);
    ClampFightCameraPoint(fightCameraDesiredEye, desired);
    const auto current =
        static_cast<std::int16_t>(FixedAtan2(fightCameraDesiredTarget.x - fightCameraDesiredEye.x, fightCameraDesiredTarget.z - fightCameraDesiredEye.z));
    auto angle = static_cast<std::int16_t>(FixedAtan2(x - other->contactPosition.x, z - other->contactPosition.z));
    if (fightCameraLinkedKind == 1)
        angle = static_cast<std::int16_t>(angle - 4096 * fightCameraLinkedSide);
    else if (fightCameraLinkedKind == 2)
        angle = static_cast<std::int16_t>(angle + fightCameraLinkedSide * (208 * fightCameraLinkedFrames + 1024) + 0x8000);
    else {
        float spawnX, spawnZ;
        const auto *const bytes = reinterpret_cast<const std::byte *>(self->game);
        std::memcpy(&spawnX, bytes + 0x2f4 + 12 * self->nearestSpawnPoint, sizeof(spawnX));
        std::memcpy(&spawnZ, bytes + 0x2f8 + 12 * self->nearestSpawnPoint, sizeof(spawnZ));
        angle = static_cast<std::int16_t>(FixedAtan2(spawnX - linkedPosition.x, spawnZ - linkedPosition.z));
        const auto difference = static_cast<std::int16_t>(std::abs(static_cast<std::int32_t>(angle) - current));
        if (difference < 8192) {
            const Vector3 lifted{fightCameraDesiredEye.x, (g_combatCameraFlags & 2) ? fightCameraDesiredEye.y : 126.0f, fightCameraDesiredEye.z};
            ClampFightCameraPoint(fightCameraDesiredEye, lifted);
        } else if (difference > 0x4000)
            g_combatCameraFlags |= 2;
    }
    const auto difference = static_cast<std::int16_t>(angle - current);
    const Vector3 relative{fightCameraDesiredEye.x - fightCameraDesiredTarget.x, fightCameraDesiredEye.y - fightCameraDesiredTarget.y,
        fightCameraDesiredEye.z - fightCameraDesiredTarget.z};
    Vector3 result = RotateFightCameraPoint(
        relative, fightCameraDesiredTarget, static_cast<std::uint16_t>(std::clamp(static_cast<std::int32_t>(difference), -24576, 24576)));
    if (fightCameraLinkedKind != 3) {
        if (FightCameraRaycast(result, result, fightCameraEye)) {
            fightCameraHasIntersectionTest = false;
            return;
        }
        if (fightCameraHasIntersectionTest)
            ++fightCameraLinkedFrames;
    }
    fightCameraDesiredEye = result;
    if (fightCameraLinkedKind == 3) {
        auto roll = g_cameraControl->nextRotation.roll;
        if (roll < 8192)
            roll = static_cast<std::int16_t>(roll + 64);
        g_cameraControl->nextRotation.roll = roll;
        if (linkedPosition.y < 121 && !(g_combatCameraFlags & 1))
            g_combatCameraFlags |= 1;
    }
    if (IsEarlyAuthFightCamera()) {
        fightCameraEye = fightCameraDesiredEye;
        fightCameraTarget = fightCameraDesiredTarget;
    }
}



void UpdateMotionInterpolationFightCamera() {
    using enum CameraState;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    auto *const player = static_cast<Character *>(control->ryo);
    Vector3 playerBone{};
    GetCharacterMotionWorldPosition(player, 5, &playerBone);
    playerBone.y += 0.5f;
    fightCameraDesiredEye = playerBone;
    if (fightMotionCameraInitializeEye) {
        fightCameraEye = playerBone;
        fightMotionCameraInitializeEye = false;
        const auto inverseYaw = static_cast<std::uint16_t>(-GetAllAngYForNode(&player->entity.node));
        fightCameraTarget = {sin_fixed16(inverseYaw) * 5 + playerBone.x, playerBone.y, cos_fixed16(inverseYaw) * -5 + playerBone.z};
    }
    fightMotionCameraPreviousPerspective = camCtrlGetPerspective();
    camCtrlSetPerspective(3640);
    if (fightMotionCameraState == 0) {
        fightMotionCameraSegmentTarget = fightCameraTarget;
        if (fightMotionCameraWaypointIndex == fightMotionCameraWaypointCount)
            fightMotionCameraState = 3 - (fightMotionCameraTarget != nullptr);
        else
            fightMotionCameraState = 1;
        fightMotionCameraTime = fightMotionCameraTarget && fightMotionCameraWaypointIndex == fightMotionCameraWaypointCount - 1 ? 10 : 60;
    } else if (fightMotionCameraState == 1) {
        const float blend = std::clamp(static_cast<float>(std::max(15, static_cast<std::int32_t>(fightMotionCameraTime)) - 15) / 45.0f, 0.0f, 1.0f);
        const Vector3 &waypoint = fightMotionCameraWaypoints[fightMotionCameraWaypointIndex];
        const Vector3 target{(1 - blend) * waypoint.x + fightMotionCameraSegmentTarget.x * blend,
            (1 - blend) * waypoint.y + fightMotionCameraSegmentTarget.y * blend, (1 - blend) * waypoint.z + fightMotionCameraSegmentTarget.z * blend};
        ClampFightCameraPoint(fightCameraDesiredTarget, target);
        fightMotionCameraTime -= shenGetScaledDeltaTickTime(1);
        if (fightMotionCameraTime <= 0) {
            ++fightMotionCameraWaypointIndex;
            fightMotionCameraTime = 0;
            fightMotionCameraState = 0;
        }
    } else if (fightMotionCameraState == 2 || fightMotionCameraState == 3) {
        if (fightMotionCameraState == 2) {
            GetCharacterMotionWorldPosition(fightMotionCameraTarget, 5, &fightCameraDesiredTarget);
            Vector3 relative{fightCameraDesiredEye.x - fightMotionCameraSegmentTarget.x, fightCameraDesiredEye.y - fightMotionCameraSegmentTarget.y,
                fightCameraDesiredEye.z - fightMotionCameraSegmentTarget.z};
            ScaleFightCameraVector(relative, 1.5f);
            const Vector3 eye{
                relative.x + fightMotionCameraSegmentTarget.x, relative.y + fightMotionCameraSegmentTarget.y, relative.z + fightMotionCameraSegmentTarget.z};
            ClampFightCameraPoint(fightCameraDesiredEye, eye);
        }
        fightMotionCameraTime -= shenGetScaledDeltaTickTime(1);
        if (fightMotionCameraTime <= 0) {
            fightMotionCameraTime = 0;
            fightMotionCameraState = 4;
        }
    } else if (fightMotionCameraState == 4) {
        fightCameraDesiredEye = fightMotionCameraSavedEye;
        fightCameraDesiredTarget = fightMotionCameraSavedTarget;
        camCtrlSetPerspective(fightMotionCameraPreviousPerspective);
        camCtrlSetInterpModeTo(CameraMotionMode::Default);
        camCtrlSetFovTransitionTo(false);
        camCtrlSetState(Init);
        cpamSetFlag(player, 0);
    }
    const float distance = FightCameraLength({fightCameraEye.x - playerBone.x, fightCameraEye.y - playerBone.y, fightCameraEye.z - playerBone.z}) - 0.2f;
    player->alpha = distance >= 0 ? std::fmin(1.0f, distance) : 0;
}

// 522470: player F01B/F01A camera transaction, bound to real input mediation,
// help-icon masks, player MTWK and the saved field camera.
void UpdateMotionFightCamera(VFActor *self, VFActor *other) {
    using enum CameraState;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    auto *const player = static_cast<Character *>(control->ryo);
    auto *const motion = FindMotionWork(player);
    Vector3 playerBone{};
    GetCharacterMotionWorldPosition(player, 5, &playerBone);
    Vector3 focus;
    if (fightMotionCameraTarget)
        focus = GetCharacterWorldPosition(fightMotionCameraTarget);
    else
        focus = {(self->position.x + other->position.x) * 0.5f, (self->position.y + other->position.y) * 0.5f, (self->position.z + other->position.z) * 0.5f};
    focus.y += 1;
    switch (fightMotionCameraUpdateState) {
    case 0:
        if (GetMotionAnimationId(motion, MotionRegion::LowerBody) == 0xf01b && GetMotionFrame(motion, MotionRegion::LowerBody) > 20) {
            cpamSetFlag(player, 1);
            fightMotionCameraUpdateState = 1;
        }
        break;
    case 1:
        fightCameraDesiredEye = playerBone;
        fightCameraDesiredTarget = focus;
        if (GetMotionAnimationId(motion, MotionRegion::LowerBody) == 0xf01b && SetMotionRegionFlag(motion, 65, 1, MotionRegion::LowerBody)) {
            mtwkRequestMotion(motion, 0xf01a, MotionRegion::LowerBody);
            SetHelpIconMenuFlags(320);
            g_cameraControl->flags |= 0x20000;
            g_cameraControl->motionScaleFactor = 1.5f;
            fightMotionCameraUpdateState = 2;
        }
        break;
    case 2: {
        Vector3 direction{focus.x - playerBone.x, 0, focus.z - playerBone.z};
        ScaleFightCameraVector(direction, 1);
        fightCameraDesiredEye = {playerBone.x + direction.x, playerBone.y, playerBone.z + direction.z};
        fightCameraDesiredTarget = focus;
        SetMotionRegionEvalTime(motion, 0, MotionRegion::LowerBody);
        float perspective = static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f;
        if ((g_inputBuffer[0].held & 0x40) && perspective < 60)
            perspective += 2;
        else if ((g_inputBuffer[0].held & 0x80) && perspective > 10)
            perspective -= 2;
        camCtrlSetPerspective(static_cast<std::uint16_t>(static_cast<std::int32_t>(perspective * 182)));
        if (GetBattleDataConfig(2, SHEN_NAME(BTCC)) || shenGetMinutesUntilNextFrameMinute() == 0) {
            g_cameraControl->flags &= ~0x20000U;
            g_cameraControl->motionScaleFactor = 1;
            g_cameraControl->yOffset = 0;
            ResetHelpIconsToItemPrompt();
            fightMotionCameraUpdateState = 3;
        } else
            GetBattleDataConfig(0, SHEN_NAME(BTCC));
        break;
    }
    case 3: {
        fightCameraDesiredEye = playerBone;
        fightCameraDesiredTarget = focus;
        SetMotionRegionEvalTime(motion, 0, MotionRegion::LowerBody);
        const float perspective = static_cast<float>(static_cast<std::int16_t>(camCtrlGetPerspective())) / 182.04445f;
        if (perspective >= 56)
            fightMotionCameraUpdateState = 4;
        else
            camCtrlSetPerspective(static_cast<std::uint16_t>(static_cast<std::int32_t>((perspective + 4) * 182)));
        break;
    }
    case 4:
        fightCameraDesiredEye = fightMotionCameraSavedEye;
        fightCameraDesiredTarget = fightMotionCameraSavedTarget;
        if (GetMotionAnimationId(motion, MotionRegion::LowerBody) == 0xf01a && SetMotionRegionFlag(motion, 65, 1, MotionRegion::LowerBody)) {
            camCtrlSetInterpModeTo(CameraMotionMode::Default);
            camCtrlSetFovTransitionTo(false);
            camCtrlSetState(Init);
            cpamSetFlag(player, 0);
            SetPlayerControlEnabled(1);
            g_combatCameraFlags &= ~4U;
            fightMotionCameraUpdateState = 5;
            FinishLoading();
        }
        break;
    default:
        break;
    }
    const float distance = FightCameraLength({fightCameraEye.x - playerBone.x, fightCameraEye.y - playerBone.y, fightCameraEye.z - playerBone.z}) - 0.2f;
    player->alpha = distance >= 0 ? std::fmin(1.0f, distance) : 0;
}



void MoveFightCameraToward(const Vector3 &desired, Vector3 &retained, Vector3 &direction) {
    Vector3 delta{(desired.x - retained.x) / 5, (desired.y - retained.y) / 5, (desired.z - retained.z) / 5};
    const float distance = FightCameraLength(delta);
    if (std::fabs(distance) < 0.00000011920929f) {
        direction = {};
        return;
    }
    float velocity = ((delta.y * direction.y + delta.x * direction.x) + delta.z * direction.z) / distance;
    if ((velocity > 0 && distance <= 0) || (velocity <= 0 && distance > 0))
        velocity = 0;
    velocity += 0.032000002f;
    if (std::fabs(distance) > std::fabs(velocity))
        ScaleFightCameraVector(delta, velocity);
    const Vector3 position{delta.x + retained.x, delta.y + retained.y, delta.z + retained.z};
    direction = {position.x - retained.x, position.y - retained.y, position.z - retained.z};
    retained.x += direction.x;
    retained.y += direction.y;
    retained.z += direction.z;
}

constexpr std::uint32_t CameraDisableInterpolation = 0x00000001U;
constexpr std::uint32_t CameraRebuildPositionSpline = 0x00000002U;
constexpr std::uint32_t CameraRebuildRotationSpline = 0x00000004U;
constexpr std::uint32_t CameraDisableFovInterpolation = 0x00000200U;
constexpr std::uint32_t CameraRaycastEnabled = 0x00000400U;
constexpr std::uint32_t CameraRaycastHit = 0x00040000U;
constexpr std::uint32_t CameraInterpolationSnap = 0x00000100U;
constexpr std::uint32_t CameraSmoothMotion = 0x00020000U;
constexpr std::uint32_t CameraAutomaticDistanceFov = 0x08000000U;

CameraRuntime cameraStorage{};
float nearClip = 0.015f;
float farClip = 2200.0f;
float translationScaleFactor = 0.0f;
Matrix4x4 cachedCameraTransform{};
CameraRuntime savedCameraRuntime{};
SearchCameraState searchCamera{};

alignas(8) std::array<std::byte, 0x300> conversationPacketScratch{};

struct ConversationParticipantAlpha {
    std::uint32_t flags{};
    shen_name name{};
};

struct ConversationShotHistory {
    std::int32_t candidate{-1};
    std::int16_t duplicateCandidate{-1};
    std::int16_t shotType{-1};
    shen_name eyeName{};
    shen_name targetName{};
};

struct ConversationCandidateState {
    std::uint8_t flags{};
    std::uint8_t group{};
    std::uint8_t groupIndex{};
    std::uint8_t collisionClass{};
};





struct ConversationCameraState {
    std::uint32_t flags{};
    std::uint32_t sceneFlags{};
    std::uint32_t descriptorFlags{};
    Character *root{};
    Character *specialTarget{};
    Character *packetSpeaker{};
    Character *packetListener{};
    shen_name specialTargetName{};
    shen_name rootScenarioName{}, targetScenarioName{}, eyeScenarioName{};
    const CameraDialogueRecord *source{};
    const CameraDialogueRecord *preparedStream{};
    std::array<const CameraDialogueRecord *, 24> shots{};
    std::int8_t shotCount{};
    std::int8_t memberCount{};
    std::int8_t shotIndex{};
    std::array<Character *, 5> members{};
    std::array<std::uint8_t, 5> memberSeen{};
    std::array<ConversationParticipantAlpha, 6> alphaParticipants{};
    std::array<std::uint8_t, 24> shotFlags{};
    std::array<std::int8_t, 24> shotTypes{};
    std::array<Character *, 24> targetAnchors{};
    std::array<Character *, 24> eyeAnchors{};
    std::array<Character *, 24> partners{};
    std::int16_t totalFrames{};
    std::uint16_t viewUpdateCount{};
    std::int16_t entryBlendFrames{};
    std::int8_t shotType{};
    std::uint8_t currentShotFlags{};
    Vector3 lookLocation{};
    std::int16_t lookDelayFrames{};
    std::int16_t lookTurnFrames{};
    std::int8_t orientationClass{};
    std::int8_t candidateIndex{-1};
    std::int8_t forcedCandidate{-1};
    std::uint8_t collisionMode{3};
    std::uint8_t mirrorFlags{};
    bool side{};
    std::uint8_t shotGenerationCount{};
    std::int16_t activationFrames{};
    std::int16_t specialOverrideFrames{};
    std::uint32_t shotStartTime{};
    std::uint32_t pauseStartTime{};
    float shotFrame{};
    float shotDurationFrames{};
    float shotProgress{};
    std::uint32_t shotScratch{};
    float actorDistance{};
    float actorHeightDelta{};
    float rootPartnerHeightDelta{};
    Vector3 previousActorEye{};
    Vector3 previousActorTarget{};
    std::int16_t actorYaw{};
    std::int16_t previousActorYaw{};
    std::int16_t actorSmoothingFrames{};
    Vector3s actorDirection{};
    Vector3 actorEye{};
    Vector3 actorTarget{};
    Vector3 eye{};
    Vector3 target{};
    Vector3 framePreviousEye{};
    Vector3 framePreviousTarget{};
    std::uint16_t framePreviousFieldOfView{};
    Vector3 previousEye{};
    Vector3 previousTarget{};
    std::uint16_t fieldOfView{};
    std::uint16_t previousFieldOfView{};
    float groundBias{0.1f};
    float groundOffset{};
    float groundPenetration{};
    std::array<std::uint8_t, 4> groupSelections{};
    std::array<ConversationCandidateState, 75> candidates{};
    std::array<ConversationShotHistory, 4> history{};
    std::uint8_t historyCursor{};
    Vector3 positionOverride{};
} conversationCamera;

struct ConversationCameraDescriptor {
    std::uint32_t flags;
    std::uint16_t bankOffset;
    std::uint16_t id;
};

constexpr std::array<std::uint32_t, 75> ConversationDescriptorFlags{0x00000401, 0x00000401, 0x00000401, 0x00000801, 0x00000801, 0x00000801, 0x00001001,
    0x00001001, 0x00001001, 0x00100401, 0x00100401, 0x00100801, 0x10000401, 0x10000401, 0x00800401, 0x00800401, 0x0000a402, 0x0000a402, 0x0000a402, 0x0000a802,
    0x0000a802, 0x0000a802, 0x0000a802, 0x0000a802, 0x0000a802, 0x0000b002, 0x0000b002, 0x0000b002, 0x0000b002, 0x0000b002, 0x0000b002, 0x0000b002, 0x0000b002,
    0x0000b002, 0x0000a802, 0x0000a802, 0x0000a802, 0x00040802, 0x00040802, 0x00040802, 0x00041002, 0x00041002, 0x00041002, 0x03004402, 0x03000802, 0x01004402,
    0x01000802, 0x02004402, 0x02000802, 0x03041002, 0x01041002, 0x02041002, 0x00200402, 0x00200402, 0x00200402, 0x00200802, 0x00200802, 0x00200802, 0x00400802,
    0x00400802, 0x00400802, 0x00800802, 0x00800802, 0x00800802, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000441, 0x00000441,
    0x00000442, 0x00000442, 0x40000000};

constexpr auto MakeConversationDescriptors() {
    std::array<ConversationCameraDescriptor, 75> result{};
    for (std::size_t index = 0; index < result.size(); ++index) {
        result[index].flags = ConversationDescriptorFlags[index];
        if (index < 16) {
            result[index].bankOffset = static_cast<std::uint16_t>(0x1a8 + index * 0x38);
            result[index].id = static_cast<std::uint16_t>(index + 1);
        } else if (index < 64) {
            result[index].bankOffset = static_cast<std::uint16_t>(0x530 + (index - 16) * 0x70);
            result[index].id = static_cast<std::uint16_t>(index + 85);
        } else if (index < 70) {
            result[index].bankOffset = static_cast<std::uint16_t>((index - 64) * 0x10);
            result[index].id = static_cast<std::uint16_t>(index + 137);
        } else if (index < 72) {
            result[index].bankOffset = static_cast<std::uint16_t>(0x60 + (index - 70) * 0x38);
            result[index].id = static_cast<std::uint16_t>(index + 231);
        } else if (index < 74) {
            result[index].bankOffset = static_cast<std::uint16_t>(0xd0 + (index - 72) * 0x70);
            result[index].id = static_cast<std::uint16_t>(index + 231);
        } else {
            result[index].bankOffset = 0xffff;
            result[index].id = 999;
        }
    }
    return result;
}

constexpr auto ConversationDescriptors = MakeConversationDescriptors();

consteval std::uint8_t ConversationHexDigit(char value) {
    return static_cast<std::uint8_t>(value <= '9' ? value - '0' : value - 'a' + 10);
}

template <std::size_t Size> consteval auto DecodeConversationBank(const char (&hex)[Size]) {
    static_assert((Size & 1U) != 0);
    std::array<std::uint8_t, (Size - 1) / 2> result{};
    for (std::size_t index = 0; index < result.size(); ++index) {
        result[index] = static_cast<std::uint8_t>(ConversationHexDigit(hex[index * 2]) * 16 + ConversationHexDigit(hex[index * 2 + 1]));
    }
    return result;
}



constexpr auto ConversationGeometryBank =
    DecodeConversationBank("1c071c07aa2a0000000080409a99993e1c07aa2aaa2a0000000040409a99993e1c0756d5aa2a0000000040409a99993e28fd34f3aa2a00005c8f5240"
                           "3333b33e34f3dd3d711c00008fc215400ad7233db00539ee711c00006666e63fae47e13daa2aaa2acdcc4c3e0ad7a33d7b14aebe8fc2f5bc8fc2f53c"
                           "ec51b8bd9a99193e295c8f3d14aec7be0ad7a3bc8fc2f53c0ad7a3bd00000000aa2aaa2a295c0f3ecdcc4c3dcdccccbe000000808fc2f53c8fc2f5bd"
                           "cdcccc3d0ad7a33cf628dcbe000000808fc2f53cae47e1bd00000000aa2aaa2ab81e853e83af0000cdcccc3d713d8a3e69d50000cdccccbd52b89e3e"
                           "19b400008fc2753de17a943eb5d50000ae47e1bdaa2aaa2ab81e853e2ba10000cdcccc3dd7a3303f74d100003d0a17bf9a99993ec4a500000ad7a33c"
                           "a4703d3f88cf0000ae47e1be0000000000000000aa2aaa2a1f856b3e19ae00000ad7233cb81e853e34c90000295c8fbd52b89e3edfaf00008fc2f5bc"
                           "9a99993e37ca0000cdcc4cbdaa2aaa2ae17a943e12a30000cdcc4cbd3333333f5fc800008fc2f5bc3333b33e05a60000cdcc4cbd0000803f0aca0000"
                           "8fc2f5bc711c711c3333b3be8fc2f53d5c8f02bf8fc2f53c8fc2f53cec51b8bd000080beec51b83d3d0a17bf0ad7a33c8fc2f53c0ad7a3bd00000000"
                           "711c711c8fc275be8fc2753d3d0a17bf000000808fc2f53c8fc2f5bd0ad723be0ad7a33c52b81ebf000000808fc2f53cae47e1bd00000000711c711c"
                           "6666e6beb81e053e3d0a17bf8fc2f53c0ad7a33c295c8fbd9a9999becdcccc3d0ad723bf0ad7a33c0ad7a33c295c8fbd00000000711c711c0ad7a3be"
                           "0ad7a33cc3f528bf0ad7a33c0ad7a33c295c8fbd7b142ebe000000005c8f42bf0ad7233c0ad7a33c8fc275bd00000000711c711c3333b3becdcc4cbe"
                           "ae4721bf0ad7233c0ad7233c8fc275bd3d0a57be295c0fbeec5138bf0ad7233c0ad7233c8fc275bd00000000711c711c14ae07bfec51b8bd0ad723bf"
                           "0ad7a33c0ad7a33cec51b8bd85ebd1beae4761be333333bf0ad7a33c0ad7a33cec51b8bd00000000711c711c14ae07bfec51b8bd0ad723bf0ad7a33c"
                           "0ad7a33cec51b8bd85ebd1beae4761be333333bf0ad7a33c0ad7a33cec51b8bd00000000711c711ce17a14bf0ad7233ef6285cbf0ad7233dcdcc4cbd"
                           "0ad7a3bd5c8fc2beec51b83d3d0a57bf0ad7233d8fc2f5bc0ad7a3bd00000000711c711c713d0abf0ad723becdcc4cbf8fc2f53c0ad723bd295c8fbd"
                           "1f85ebbee17a94be7b146ebf8fc2f53ccdcc4cbd0ad7a3bd00000000711c711ccdcc4cbeec5138be333393bf8fc275bd8fc2f5be14ae07bfae47e1bd"
                           "cdcc4cbe7b148ebf8fc275bd48e1fabe713d0abf00000000711c711cd7a3f0becdcc4cbe333373bf295c8fbd8fc2f5be5c8f02bfcdccccbecdcc4cbe"
                           "000080bf0ad7a3bd000000bfb81e05bf00000000711c711c0ad723beb81e05be3d0ab7bfcdcc4cbd8fc2f5be6666e6be295c0fbe5c8f42beae47a1bf"
                           "cdcc4cbd48e1fabe6666e6be00000000711c711c295c0f3e8fc2f5bcae4721bf0ad723bc8fc2f53ccdcc4cbd0ad723bd8fc2f53c666626bf0ad723bc"
                           "8fc2f53ccdcc4cbd00000000711c711cae47e13d0ad7a33c666626bf0ad723bc8fc2f53ccdcc4cbd0ad7233e0ad7a33dae4721bf0ad723bc8fc2f53c"
                           "cdcc4cbd00000000711c711c713d8abe295c8f3eb81e05bf8fc275bd0ad7a33c0ad7233d1f856bbe7b14ae3e85ebd1becdcc4cbd0ad7a33c0ad7233d"
                           "00000000711c711cec5138be5c8f023fd7a330bfec51b8bd0ad723bc0ad7233dae4761be713d8a3eae4721bfec51b8bd00000000cdcc4c3d00000000"
                           "0000000000000000711c711c713d0a3fd3a100008fc2f53d0000803e4fd50000295c8fbd713d0a3f2ba600000ad7233d0000803e4ed50000295c8fbd"
                           "711c711cd7a3f03e45950000cdcccc3d5c8f023fdbd1000085ebd1becdcccc3e749a00000ad7a33cb81e053f7ed000009a9999be0000000000000000"
                           "711c711cf628dc3e01a000000ad7a3bcae47613ed4c700008fc2f5bc295c0f3fdea20000295c8fbd0000803ec8c800008fc2f5bc711c711ccdcccc3e"
                           "1a9a00000ad7a3bc48e1fa3ecac700008fc2f5bc3d0ad73e919c0000cdcc4cbdd7a3303f79c800008fc2f5bc0000000000000000711c711c14aec73e"
                           "e2b00000ae47e1bd7b14ae3e17b000000ad723bc3333b33ec8a80000295c8fbdc3f5a83ec8ae00000ad723bc711c711c3333b33e3eab0000295c8fbd"
                           "cdcc0c3f5bba00000ad723bce17a943e83a000000ad723bd6666263fb7bc0000ec51b83d0000000000000000711c711ccdcc4c3fb39c0000b81e853e"
                           "6666e63e19f800000ad723bd52b85e3ffda30000295c0f3ef628dc3ec8f800000ad723bd711c711c85eb513ffb9500001f856b3e295c4f3fc2e10000"
                           "3d0ad7be3d0a573fe49a0000cdcc4c3e6666663f3bdf000014aec7be0000000000000000711c711c713d4a3f0d9f000000000000295c8f3e8ddb0000"
                           "000000003d0a173fda9d00008fc2f53cb81e853e00dc000000000000711c711ccdcc4c3fdf96000000000000e17a543f07d0000000000000cdcc0c3f"
                           "649800000000000048e13a3f63d00000000000000000000000000000711c711c295c4f3f14a40000cdccccbd713d8a3e0ed100000ad7a3bc5c8f423f"
                           "b7ac00009a9919be0ad7a33e97d200000ad7a3bc711c711c1f852b3fcc9b00009a9919bed7a3303f10cc00001f856b3e8fc2353fd8a10000ae4761be"
                           "cdcc4c3f41cc00001f856b3e0000000000000000711c711c295caf3f1fb000008fc2f53ca470bd3ea4ea00009a9919beae47a13ff0a70000e17a943e"
                           "a470bd3e98eb0000ec5138be711c711c295caf3f899900008fc2753dd7a3703ffedd0000ae47e1beb81e853fb6940000b81e053ea4703d3fffe30000"
                           "5c8f02bf0000000000000000711c711c8fc2b53f53ad00003333b3beec51b83e48de00008fc275bdcdccac3fe1a100001f856bbe7b14ae3e0fde0000"
                           "cdccccbd711c711cf628bc3f229b00008fc2f5bd3333733f3cd50000ae47e1bdf6289c3f609600008fc2f5bd0000403fa4d60000295c8fbd00000000"
                           "00000000711c711c295caf3f07a80000295c0fbe3333b33ea6df0000cdccccbd0000c03f2fb0000048e1fabe14aec73e4ae000000ad7a3bc711c711c"
                           "a470fd3fc79e0000c3f5a8bec3f5883f79d800008fc2f5bd3d0a07406aa40000cdcc4cbfb81e853f48da00000000803e0000000000000000711c711c"
                           "14ae17403a9800000000803ee17a943ee2c3000014aec7bec3f5284096a20000cdcc0c3fcdcccc3ecace000048e1fabe711c711c3d0a4740639a0000"
                           "e17a143fec51383fc1ce0000d7a370bfc3f5484082a10000295c4f3f5c8f823fc5cf0000333373bf0000000000000000711c711cc3f5e83f97a60000"
                           "52b89ebe3333b33e76eb00005c8f42be0ad72340edb30000b81e05bff628dc3e87ea000052b89ebe711c711ca470fd3ff49600000ad7a3bed7a3703f"
                           "f5db00000ad723bc713d1a40b69f0000e17a54bf3333933fc2db0000b81e053e0000000000000000711c711c85eb1140e09f000048e13abf5c8fc23e"
                           "ebec00005c8f42bee17af43f19b000000ad7a3be3d0ad73e87ea00000ad723be711c711c85eb214030980000000040bfa4707d3fdfdd00000ad7a33d"
                           "85eb2140219e00008fc2f5be7b148e3f11dc00003d0a57be0000000000000000aa2aaa2aae473140fe940000cdcccc3d9a99993e3be000000ad723bf"
                           "ae473140b2a000003333b33ea470bd3e7adf00000ad723bfaa2aaa2a8fc23540859a0000cdcccc3d14ae873f04d60000ec5178bfec51384054a30000"
                           "8fc2f53e0ad7a33faad70000713d8abf0000000000000000aa2aaa2a7b142e404aa60000000080be5c8f023f62ff0000295c0fbfcdcc2c4060a10000"
                           "f6281cbf0000003f49ff0000295c0fbfaa2aaa2a6666364042a800008fc275be48e1ba3f6be60000a4703dbf713d3a4057a00000f6281cbf7b14ae3f"
                           "98e3000048e1fabe0000000000000000aa2aaa2ae17a34401e9e0000000080bfec51b83e02da0000ae47e1be3d0a374007a40000000080bf6666e63e"
                           "5edb0000ae47e1beaa2aaa2a52b82e409aa10000000080bfae47a13fced600001f856bbe5c8f32409ba50000000080bf5c8fc23fb2db00001f856bbe"
                           "0000000000000000aa2aaa2a9a9969400f9e00008fc2753ec3f5a83e43d90000ae47e1be0ad77340f2a40000ae47213f5c8fc23e9cdb0000ae47e1be"
                           "aa2aaa2af6288440019d0000295c8f3e14ae873f25d600009a9919bf295c87407ba5000014ae073fcdccac3f7bd900007b142ebf0000000000000000"
                           "aa2aaa2a14ae77404c9d0000000040bf3333333fc70700000ad7a3be52b8864054990000a470bdbe3333333f16080000000080beaa2aaa2a33336340"
                           "9d9f0000b81e45bfa4709d3f63ec00000ad7a3be333373406a9b00003d0ad7be3d0a973f13ec000085ebd1be0000000000000000aa2aaa2a295c6f40"
                           "9fa90000ae4781bf6666263f560a00007b14aebe14ae7740e0a10000666686bfd7a3303f5606000052b89ebeaa2aaa2aa4705d40acaa0000000080bf"
                           "ae47c13ffbea0000ae4761be9a996940a0a3000014ae87bfd7a3903fbbef0000295c8fbe0000000000000000711c711ccdcc8c3fb39c0000cdcc4c3d"
                           "0ad7a33eb3e700008fc2f5bdf628dc3f04ab0000ae4761be14aec73e88e70000ec5138be711c711cf628bc3f8e990000ec51b8bdd7a3703f95d00000"
                           "b81e05bea4701d4050a10000cdcc0cbf3333933f05dc00007b142ebe0000000000000000711c711c48e19a3f87a900006666e6be0ad7a33e96ef0000"
                           "1f856bbea4703d3f39a200000ad7233de17a943e34ee0000cdccccbd711c711c52b89e3f739d000085ebd1be713d4a3fd9e40000ae4721bf48e17a3f"
                           "1c9700008fc2f5bcec51383f7ce3000052b89ebe0000000000000000711c711c6666e63f149d00007b142ebe7b14ae3ef2ef00005c8f42be8fc2f53f"
                           "5cb000000ad7a33ef628dc3ebfe800000ad7a3be711c711c85eb2140ef970000ae4761be8fc2054028000000ae47e1be295c3f40d8a600000000003f"
                           "5c8f0240f00100008fc2f5be0000000000000000711c711cae47213f75aa0000b81e05bfb81e853e02e90000295c8f3dd7a3303f4cb000009a9919bf"
                           "b81e853e02e90000295c8f3d711c711c7b146e3f9c9d0000f628dcbeae47213ff7d50000295c8f3dec51783f67a00000713d0abf3333333f21d50000"
                           "295c8f3d0000000000000000711c711c48e13a3fe5aa0000666626bfb81e853e4ce900000ad7233cb81e453fdeae0000000040bf52b89e3e20e70000"
                           "0ad7233c711c711c295c8f3fcb9d00000ad723bf85eb513f31d500000ad7233cae47a13fb3a100008fc235bfd7a3703f09d500000ad7233c00000000"
                           "00000000711c711c3333733ffda7000052b85ebfe17a943e93e700008fc2f5bc7b142e3f08ab00009a9959bf0000803e97e900008fc2f5bc711c711c"
                           "0000a03ff19f0000ae4761bf48e13a3f5fd700008fc2f5bc0ad7633f30a30000b81e45bfae47213febd800008fc2f5bc0000000000000000711c711c"
                           "1f858b3fa2b50000e17a54bfb81e853e97db0000cdcc4cbe85ebb13f2dbf00000ad763bf9a99993e23db00003d0a57be711c711c48e19a3f769f0000"
                           "713d4abf6666263f3ed10000ae4761beae47e13f1da30000000080bf7b146e3f7dd10000000080be0000000000000000711c711cf628bc3f15b40000"
                           "f628bcbf7b142e3eb8d70000a470bdbe5c8fe23f17b900005c8f82bf7b142e3e36d80000a470bdbe711c711c14ae0740bd9e00000000a0bfd7a3703f"
                           "37d40000c3f528bff6282c40efa40000ae4761bf5c8f823fd1d4000048e13abf0000000000000000711c711cf6289c3f18b3000052b8bebfae47613e"
                           "31b70000295c8fbeae47813f43c70000b81e85bf5c8f423e68c200005c8f42be711c711cae47a13fa59c00005c8fa2bf6666663f9edf0000b81e85be"
                           "6666663f4cab0000d7a390bff6285c3fc0e000008fc275be0000000000000000aa2aaa2aae47e13e98b700000ad7233dd7a3f03ec9db00000ad7a3bd"
                           "0ad7833f9fd000007b142ebe7b142e3f93e000008fc2f5bdaa2aaa2ac3f5a83e62ad00000ad7233c8fc2d53f54f10000ae4761be48e13a3fe2cd0000"
                           "7b142ebe48e13a40cced00000ad7233d0000000000000000aa2aaa2aae47a13ff5bd00009a9919be85eb113ffede00001f856bbe1f85ab3f68c60000"
                           "ae4761be52b81e3f87e00000ec5138beaa2aaa2ac3f56840e9c200000ad723bf00000040bedc00000ad723be48e17a40bccb0000000040bfec510840"
                           "93de0000ae47e1bd0000000000000000aa2aaa2af628dc3e07a700000ad7233ccdcc4c3e5edb00000ad7a3bc14ae073fdab000000ad7233cb81e853e"
                           "32dd00008fc2f5bcaa2aaa2a0ad7a33e399500000ad7233ccdcc4c3e5fdb00000ad7a3bcf628dc3edaa200000ad7233c1f852b3f66cf00000ad7a3bc"
                           "0000000000000000aa2aaa2aec51783fe69d00008fc2f53c85ebd13e1aee00005c8f42be7b146e3fe0980000b81e053e3333b33ea7f300005c8f42be"
                           "aa2aaa2aec51783fda9d00009a9919bea4707d3fa0d900005c8f42be1f856b3f8a9600008fc2753dc3f5683f4cdb00008fc2f5be0000000000000000"
                           "aa2aaa2aa470bd3e20a400000ad723bc85eb113f73a400000ad7233d6666e63e05af00000ad723bc0ad7233f22af00000ad7233daa2aaa2aa470bd3e"
                           "41a400000ad723bc0ad7833f23b800000ad7233d3d0ad73e47aa00000ad723bc8fc2b53fcabf00000ad7233d0000000000000000aa2aaa2a3d0a973f"
                           "46a200008fc275bd6666263f95b500005c8fc2bed7a3903fb6b00000ae47e13d8fc2353f92b8000052b89ebeaa2aaa2a3d0a973f47a200008fc275bd"
                           "52b8be3f0dc700005c8fc2be14aea73f0aab0000295c0f3eae47c13f1fca0000b81e05bf0000000000000000aa2aaa2aae47a13ff5bd00009a9919be"
                           "85eb113ffede00001f856bbe1f85ab3f68c60000ae4761be52b81e3f87e00000ec5138beaa2aaa2ac3f56840e9c200000ad723bf00000040bedc0000"
                           "0ad723be48e17a40bccb0000000040bfec51084093de0000ae47e1bd0000000000000000aa2aaa2aec51783fe69d00008fc2f53c85ebd13e1aee0000"
                           "5c8f42be7b146e3fe0980000b81e053e3333b33ea7f300005c8f42beaa2aaa2aec51783fda9d00009a9919bea4707d3fa0d900005c8f42be1f856b3f"
                           "8a9600008fc2753dc3f5683f4cdb00008fc2f5be0000000000000000aa2aaa2a3d0a973f46a200008fc275bd6666263f95b500005c8fc2bed7a3903f"
                           "b6b00000ae47e13d8fc2353f92b8000052b89ebeaa2aaa2a3d0a973f47a200008fc275bd52b8be3f0dc700005c8fc2be14aea73f0aab0000295c0f3e"
                           "ae47c13f1fca0000b81e05bf0000000000000000711c711c85eb113ff79e00008fc2753d0ad7233e54c800000ad723bd7b142e3f01a800008fc2f53c"
                           "0000803edece00008fc2f5bc711c711c52b81e3f959d00008fc2f53c0ad7a33ea0f000008fc275bd713d4a3f3ba600000ad7233d52b89e3e71f10000"
                           "8fc275bd0000000000000000711c711c6666663fe3b600000ad7233ce17a943ef9f90000cdcc4cbd85eb513fa4a90000295c8f3d295c8f3efbfa0000"
                           "cdcc4cbd711c711cf628bc3f84b400009a9919be0000003f36030000295c0fbe14aea73fcba700000ad723bd5c8f023f240200008fc2f5bd00000000"
                           "00000000711c711c713d4a3fa9ad00008fc275beb81e853ec9f90000cdcc4cbdd7a3303f96a50000ae47e1bdb81e853ec3fb00008fc2f5bc711c711c"
                           "6666863fe0aa0000c3f5a8be0000003f2f010000cdcc4cbd295c4f3f49a40000b81e05be5c8f023f31010000cdcc4cbd0000000000000000711c711c"
                           "9a99b93fd7a400008fc2753e9a99993ebaf20000ec5138beae47c13f9aad0000cdcc4c3d295c8f3e36f200000ad723be711c711c9a99b93fdca40000"
                           "8fc2753ea470bd3ec8ee0000ae4761be5c8fc23fb8ae0000cdcc4c3d6666e63e76ff0000cdcc4cbe0000000000000000711c711c0ad7c33fbfa80000"
                           "0ad723be1f856b3e25e100000ad723beae47813fe4a100000ad7a3bd9a99193ea9e300008fc275bd711c711c0ad7c33fb7a800000ad723be7b14ae3e"
                           "8ae000000ad723be0000803f0ca200000ad7a3bd295c8f3ec4dc00008fc275bd0000000000000000711c711c0000a03f8aaa0000ec51b8bec3f5a83e"
                           "1ec2000000000000ec51d83f7bb100003333b3be52b89e3ef7d60000cdcc4cbe711c711c0000a03f7baa0000ec51b8be5c8fc23e92c600000ad723bc"
                           "14aee73fdcb000003333b3becdcccc3eb0da0000ec5138be0000000000000000711c711c6666863f69bd00000ad763bf0ad7a33e71060000a470bdbe"
                           "48e17a3fdbc800008fc235bf0ad7a33e0d0900007b14aebe711c711c6666863f5cbd00000ad763bf0ad7a33e6b060000a470bdbea4707d3ff2c80000"
                           "8fc235bf52b89e3e1a090000c3f5a8be0000000000000000711c711c0ad7833fc7c900007b14aebfae47613edab70000b81e85bea470dd3f31c00000"
                           "8fc2d5bf8fc2753eb0bd0000e17a94be711c711c1f858b3fdea40000e17ab4bf1f85eb3f250100007b142ebf14ae074057ab00001f85cbbfcdccec3f"
                           "bd0000007b142ebf0000000000000000711c711c48e19a3fd0c90000a470ddbfe17a943e95bf00003333b3be295caf3f82cb0000b81e85bf0000803e"
                           "32c30000b81e85be711c711c9a99993fe9a100003333d3bf8fc2353f31d10000ec51b8be3333b33f76a400007b148ebfcdcc4c3fb7cf000014ae07bf"
                           "0000000000000000711c711cb81ec53fabbc0000ae4781bf3d0ad73ec6ca000052b89ebea470bd3f9bc600005c8f42bff628dc3edecd0000b81e85be"
                           "711c711c7b14ee3f50a500005c8fa2bff628bc3f51f70000f6285cbfe17ad43fdeab0000000080bf0000c03f77f60000cdcc4cbf0000000000000000"
                           "711c711cb81e453f84cf00005c8fe2bfae47e13ef0e00000295c8fbe0ad7833fd5cd0000cdccccbfae47e13ef1e00000295c8fbe711c711c9a99593f"
                           "6b9c00009a99d9bf52b80e406afd000052b85ebf8fc2753f15a30000713dcabfa4700d40befd0000c3f568bf0000000000000000711c711cc3f50840"
                           "c8cb0000ae47a1bf5c8f023f3af500007b14aebe333303400fc300005c8fa2bf0000003ffef40000ec51b8be711c711cd7a310404dad000052b8debf"
                           "14aee73f86fa00005c8fc2be66661640a9a400007b14cebf713dea3f98fa0000a470bdbed018d24001000000");

static_assert(ConversationGeometryBank.size() == 0x1a30);

template <typename T> T ReadConversationBank(std::uint16_t offset) {
    T value{};
    std::memcpy(&value, ConversationGeometryBank.data() + offset, sizeof(value));
    return value;
}

Character *CameraRootCharacter() {
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

const LegacyInputSlot *CameraInput() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    PadControlEntry *const pad = control ? static_cast<PadControlEntry *>(control->padControl) : nullptr;
    return pad ? ctrlGetPadInputBuffer0(pad) : nullptr;
}

float CameraDistance(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x;
    const float y = first.y - second.y;
    const float z = first.z - second.z;
    return std::sqrt(x * x + y * y + z * z);
}

float CameraLerp(float target, float base, float amount) {
    return (target - base) * amount + base;
}

std::int16_t CameraLerpAngle(std::int16_t target, std::int16_t base, float amount) {
    return static_cast<std::int16_t>(
        static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(target - base)) * amount + static_cast<float>(base)));
}

float FixedAngleToRadians(std::int32_t angle);

float SmoothConversationProgress(float progress) {
    float clamped = std::fmax(0.0f, progress);
    if (clamped >= 1.0f)
        clamped = 1.0f;
    float scale = std::fmax(0.001f, 0.94999999f);
    if (scale >= 1.0f)
        scale = 0.99989998f;
    const float angle = scale * 16384.0f;
    const float tangent = std::tan(FixedAngleToRadians(static_cast<std::uint16_t>(static_cast<std::int32_t>(angle))));
    const std::int16_t fixed = static_cast<std::int16_t>(static_cast<std::int32_t>((std::atan(tangent * clamped) * 65536.0f) / 6.2831855f + 0.5f));
    return static_cast<float>(fixed) / angle;
}

std::int16_t CameraTransitionAngle(std::int16_t target, std::int16_t base, float amount, CameraInterpolationMode mode) {
    if (mode != CameraInterpolationMode::PreviousToCurrent)
        return CameraLerpAngle(target, base, amount);

    if (std::abs(amount) < std::numeric_limits<float>::epsilon())
        return base;

    const std::int16_t delta = static_cast<std::int16_t>(target - base);
    return static_cast<std::int16_t>(base + static_cast<std::int32_t>(static_cast<float>(delta) * amount));
}

float SearchCameraEase(const CameraRuntime &camera) {
    return std::fmax(0.25f, 1.0f - CameraDistance(camera.nextPosition, searchCamera.cameraPosition) * 0.13600001f);
}

// InitDynamicSceneData publishes aligned static storage, not heap ownership.
alignas(32) std::array<CameraFieldDepthSample, 257> worldFieldDepth{};
alignas(32) std::array<CameraFieldDepthSample, 257> cameraFieldDepth{};
std::uint32_t fieldDepthPointCount{};
Matrix4x4 fieldDepthTransform{};
Matrix4x4 fieldDepthModelTransform{};
float fieldCellScale{};
std::int32_t fieldInverseYaw{};
struct FieldDepthScale {
    float farDistance{};
    float nearDistance{};
    float horizontalTangent{};
    float verticalTangent{};
    float npcRadius{};
    float savedHorizontalTangent{};
    float savedVerticalTangent{};
} fieldDepthScale;
float projectedShadowFarDistance{};

Vector3 TransformFieldDepthPoint(const Vector3 &point) {
    const Matrix4x4 &matrix = workTransformMatrix;
    return {point.x * matrix.rows[0].x + point.y * matrix.rows[1].x + point.z * matrix.rows[2].x + matrix.rows[3].x,
        point.x * matrix.rows[0].y + point.y * matrix.rows[1].y + point.z * matrix.rows[2].y + matrix.rows[3].y,
        point.x * matrix.rows[0].z + point.y * matrix.rows[1].z + point.z * matrix.rows[2].z + matrix.rows[3].z};
}

// camCtrlRasterizeDangerSphereSpan increments the point id before testing whether any sample
// changes, and tests the right endpoint before filling back toward the left.
void PublishFieldDepthSphereSpan(float x, float depth, float radius, std::uint32_t id) {
    ++fieldDepthPointCount;
    const float reciprocal = 1.0f / depth;
    const std::int32_t first = std::clamp(static_cast<std::int32_t>(((x - radius) * reciprocal) * 128.0f), -128, 128) + 128;
    const std::int32_t last = std::clamp(static_cast<std::int32_t>(((x + radius) * reciprocal) * 128.0f), -128, 128) + 128;
    CameraFieldDepthSample &right = worldFieldDepth[last];
    if (depth < right.depth) {
        right = {id, depth};
    } else if (worldFieldDepth[first].id == right.id) {
        return;
    }
    for (std::int32_t sample = last - 1; sample >= first; --sample) {
        if (worldFieldDepth[sample].depth > depth) {
            worldFieldDepth[sample] = {id, depth};
        }
    }
}



void PublishFieldDepthSphere(const Vector4 &sphere) {
    Vector3 point = TransformFieldDepthPoint({sphere.x, sphere.y, sphere.z});
    if (point.z >= 0.0f)
        return;

    const float depth = -point.z;
    const float back = sphere.w + depth;
    if (back >= 0.0f && depth - sphere.w <= fieldDepthScale.farDistance) {
        const float horizontal = back * fieldDepthScale.horizontalTangent + sphere.w;

        if (point.x <= horizontal && -horizontal < point.x) {
            const float vertical = back * fieldDepthScale.verticalTangent + sphere.w;

            if (point.y <= vertical && -vertical < point.y) {
                PublishFieldDepthSphereSpan(point.x, depth, sphere.w, fieldDepthPointCount);
            }
        }
    }
}

// camCtrlRasterizeWallDepth clips the authored segment in depth, then rasterizes its
// perspective X/Z span. WALL writes world depth; WALC writes the later copy.
void PublishFieldDepthWall(const MapFieldDepthWall &wall, bool cameraOnly) {
    Vector3 first = TransformFieldDepthPoint(wall.first);
    Vector3 second = TransformFieldDepthPoint(wall.second);
    first.z = -first.z;
    second.z = -second.z;
    const float farDistance = fieldDepthScale.farDistance;
    if (!((first.z >= 0.1f || second.z >= 0.1f) && (first.z <= farDistance || second.z <= farDistance))) {
        return;
    }
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    const float depthDelta = second.z - first.z;
    if (std::abs(depthDelta) >= epsilon) {
        const float oldDepth = first.z;
        const float oldX = first.x;
        const float slope = (second.x - first.x) / depthDelta;
        if (first.z < 0.1f) {
            first.z = 0.1f;
            first.x += (0.1f - oldDepth) * slope;
        }
        if (first.z > farDistance) {
            first.z = farDistance;
            first.x = (farDistance - oldDepth) * slope + oldX;
        }
        if (second.z < 0.1f) {
            second.z = 0.1f;
            second.x = (0.1f - oldDepth) * slope + oldX;
        }
        if (second.z > farDistance) {
            second.z = farDistance;
            second.x = (farDistance - oldDepth) * slope + oldX;
        }
    }

    if (!(std::abs(first.z) >= epsilon && std::abs(second.z) >= epsilon))
        return;

    std::int32_t begin = static_cast<std::int32_t>((first.x / first.z) * 128.0f);
    std::int32_t end = static_cast<std::int32_t>((second.x / second.z) * 128.0f);
    if (begin > end) {
        std::swap(begin, end);
        std::swap(first, second);
    }

    if (begin > 128 || end < -128)
        return;

    begin = std::max(begin, -128);
    end = std::min(end, 128);
    const float xDelta = second.x - first.x;
    if (!(std::abs(xDelta) >= epsilon))
        return;

    const float slope = (second.z - first.z) / xDelta;
    const float intercept = first.z - first.x * slope;
    auto &samples = cameraOnly ? cameraFieldDepth : worldFieldDepth;
    for (std::int32_t x = begin; x <= end; ++x) {
        const float denominator = 1.0f - ((static_cast<float>(x) * slope) * 0.0078125f);
        if (std::abs(denominator) >= epsilon) {
            const float depth = intercept / denominator;
            CameraFieldDepthSample &sample = samples[x + 128];
            if (depth >= 0.0f && sample.depth > depth) {
                sample = {fieldDepthPointCount++, depth};
            }
        }
    }
}

float FixedAngleToRadians(std::int32_t angle) {
    return static_cast<float>(angle) * 6.2831855f * 0.000015258789f;
}

std::int32_t RadiansToFixedAngle(float radians) {
    return static_cast<std::int32_t>(radians * (65536.0f / 6.2831853071795864769f));
}

Vector3 ConversationActorPosition(Character *character) {
    Vector3 position{};
    GetCharacterMotionWorldPosition(character, 5, &position);
    return position;
}

std::int16_t ConversationActorYaw(const Character *character) {
    return character ? GetAllAngYForNode(&character->entity.node) : std::int16_t{};
}

Vector3 TransformConversationPoint(const Vector3 &point, const Vector3 &origin, std::int16_t yaw, std::int16_t pitch = 0) {
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    if (pitch)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pitch));
    const Vector3 transformed = TransformFieldDepthPoint(point);
    Matrix4x4::Pop(1);
    return transformed;
}
void ApplyConversationSpecialModeOverride(Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    ConversationCameraState &state = conversationCamera;
    const PlayerModelState *const model = g_playerRuntime.model;
    const bool requested = (g_playerRuntime.flags & 0x00040000U) != 0 && model && model->specialModeTargetId != 0 && (model->specialModeFlags & 4U) != 0 &&
        static_cast<std::uint8_t>(model->specialModeCounter) == 2U;
    bool firstFrame = false;
    if (requested) {
        if (!(state.flags & 0x1000U) && state.specialOverrideFrames == 0) {
            firstFrame = true;
            state.flags |= 0x1000U;
        }
    } else {
        state.flags &= ~0x1000U;
    }

    const std::int16_t elapsed = state.specialOverrideFrames;
    if (elapsed > 75)
        state.flags &= ~0x1000U;
    if (!(state.flags & 0x1000U) || !state.root)
        return;
    state.specialOverrideFrames = static_cast<std::int16_t>(elapsed + 1);

    // The target sits on the right hand (rig bone 14); the eye looks along the hand's rotated
    // axis, pitched by 0x2e38, from half a unit back.
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(charaGetBoneTransform(FindMotionWork(state.root), 14));
    Matrix4x4::preTranslate(0.17f, 0.050000001f, -0.029999999f);
    Vector3 specialTarget{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::SetViewMatrixFromEuler(16384, 25486, 16384);
    Matrix4x4::preTranslate(0.0f, 0.0f, 1.0f);
    const Vector3 facing{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(specialTarget.x, specialTarget.y, specialTarget.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(InverseFixedAtan2(facing.x - specialTarget.x, facing.z - specialTarget.z)));
    Matrix4x4::RotateAroundX_Wrapper(0x2e38);
    Matrix4x4::preTranslate(0.0f, 0.0f, -0.5f);
    Vector3 specialEye{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    fieldOfView = 7281;
    if (!firstFrame) {
        const float time = camCtrlGetDeltaTime(0.050000001f);
        specialEye = {CameraLerp(specialEye.x, state.framePreviousEye.x, time), CameraLerp(specialEye.y, state.framePreviousEye.y, time),
            CameraLerp(specialEye.z, state.framePreviousEye.z, time)};
        specialTarget = {CameraLerp(specialTarget.x, state.framePreviousTarget.x, time), CameraLerp(specialTarget.y, state.framePreviousTarget.y, time),
            CameraLerp(specialTarget.z, state.framePreviousTarget.z, time)};
    }
    eye = specialEye;
    target = specialTarget;
}

bool ConversationCollisionSegment(const Vector3 &start, const Vector3 &end, float radius) {
    bool hit = false;
    if (conversationCamera.collisionMode & 1U) {
        if (conversationCamera.collisionMode & 4U)
            flddToggleFlag(4096);
        hit = GlobalRayIntersectionTest(start, end, radius);
    }
    if (conversationCamera.collisionMode & 2U) {
        const std::size_t shot = static_cast<std::size_t>(conversationCamera.shotIndex);
        ccmUpdateEntityFlag(conversationCamera.targetAnchors[shot]);
        ccmUpdateEntityFlag(conversationCamera.eyeAnchors[shot]);
        hit |= CheckCharacterCylinderIntersection(start, end);
    }
    conversationCamera.collisionMode = 3;
    return hit;
}

bool ConversationCollisionPoint(const Vector3 &point, float radius) {
    bool hit = false;
    if (conversationCamera.collisionMode & 1U) {
        if (conversationCamera.collisionMode & 4U)
            flddToggleFlag(4096);
        hit = IsWithinRadiusFromVector3PolyIntersectTest(point, radius) != 0;
    }
    if (conversationCamera.collisionMode & 2U) {
        const std::size_t shot = static_cast<std::size_t>(conversationCamera.shotIndex);
        ccmUpdateEntityFlag(conversationCamera.targetAnchors[shot]);
        ccmUpdateEntityFlag(conversationCamera.eyeAnchors[shot]);
        hit |= CheckCharacterSphereIntersection(point, radius);
    }
    conversationCamera.collisionMode = 3;
    return hit;
}

float ConversationBankFloat(const ConversationCameraDescriptor &descriptor, std::uint16_t offset) {
    return ReadConversationBank<float>(static_cast<std::uint16_t>(descriptor.bankOffset + offset));
}

std::int16_t ConversationBankI16(const ConversationCameraDescriptor &descriptor, std::uint16_t offset) {
    return ReadConversationBank<std::int16_t>(static_cast<std::uint16_t>(descriptor.bankOffset + offset));
}

std::int32_t ConversationBankI32(const ConversationCameraDescriptor &descriptor, std::uint16_t offset) {
    return ReadConversationBank<std::int32_t>(static_cast<std::uint16_t>(descriptor.bankOffset + offset));
}

Vector3 ConversationPolarPoint(float distance, float height, std::int16_t angle) {
    return {
        -sin_fixed16(angle) * distance, height, -sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(angle)))) * distance};
}

// @verified - p
float CameraLerpWithEpsilon(float from, float to, float amount) {
    if (std::fabs(amount) < std::numeric_limits<float>::epsilon() || std::fabs(to - from) < std::numeric_limits<float>::epsilon())
        return from;
    return (to - from) * amount + from;
}

// @verified - p
std::int16_t CameraAngleLerpWithEpsilon(std::int32_t from, std::int16_t to, float amount) {
    const std::int16_t delta = static_cast<std::int16_t>(to - from);
    if (std::fabs(amount) >= std::numeric_limits<float>::epsilon() && delta)
        return static_cast<std::int16_t>(from + static_cast<std::int32_t>(static_cast<float>(delta) * amount));
    return static_cast<std::int16_t>(from);
}

void GenerateConversationTypeOne(const ConversationCameraDescriptor &descriptor, float progress, Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    const ConversationCameraState &state = conversationCamera;
    const float mirrorX = state.mirrorFlags & 1U ? -1.0f : 1.0f, mirrorZ = state.mirrorFlags & 2U ? -1.0f : 1.0f;
    const auto value = [&](std::uint16_t first, std::uint16_t last, float scale) {
        return CameraLerpWithEpsilon(ConversationBankFloat(descriptor, first) * scale, ConversationBankFloat(descriptor, last) * scale, progress);
    };
    eye = {value(4, 28, mirrorX), value(8, 32, 1.0f), value(12, 36, mirrorZ)};
    target = {value(16, 40, mirrorX), value(20, 44, 1.0f), value(24, 48, mirrorZ)};
    fieldOfView = static_cast<std::uint16_t>(static_cast<std::int32_t>(
        CameraLerpWithEpsilon(static_cast<float>(ConversationBankI16(descriptor, 0)), static_cast<float>(ConversationBankI16(descriptor, 2)), progress)));
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(state.actorTarget.x, state.actorTarget.y, state.actorTarget.z);
    if ((descriptor.flags & 0x80U) || (state.flags & 0x200000U))
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.actorYaw));
    else {
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.actorDirection.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(state.actorDirection.x));
    }
    Matrix4x4::TransformPoint(target, target);
    Matrix4x4::TransformPoint(eye, eye);
    Matrix4x4::Pop(1);
}

void GenerateConversationTypeTwo(const ConversationCameraDescriptor &descriptor, float progress, Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    const ConversationCameraState &state = conversationCamera;
    const float distanceBlend = descriptor.flags & 0x200000U ? (0.89999998f - state.actorDistance) / 0.29999995f : (3.0f - state.actorDistance) / 2.0999999f;
    const float sign = state.mirrorFlags & 1U ? -1.0f : 1.0f;
    const auto radial = [&](std::uint16_t farOffset, std::uint16_t nearOffset) {
        return CameraLerpWithEpsilon(ConversationBankFloat(descriptor, farOffset), ConversationBankFloat(descriptor, nearOffset), distanceBlend);
    };
    const auto angle = [&](std::uint16_t farOffset, std::uint16_t nearOffset) {
        return static_cast<std::int32_t>(static_cast<float>(CameraAngleLerpWithEpsilon(ConversationBankI32(descriptor, farOffset),
                                             static_cast<std::int16_t>(ConversationBankI32(descriptor, nearOffset)), distanceBlend)) *
            sign);
    };
    const float firstEyeDistance = radial(0x38, 0x04), lastEyeDistance = radial(0x50, 0x1c);
    const float firstEyeHeight = radial(0x40, 0x0c) + firstEyeDistance * 0.0f, lastEyeHeight = radial(0x58, 0x24) + lastEyeDistance * 0.0f;
    const std::int32_t firstEyeAngle = angle(0x3c, 0x08), lastEyeAngle = angle(0x54, 0x20);
    const float firstTargetDistance = radial(0x44, 0x10), lastTargetDistance = radial(0x5c, 0x28);
    const float firstTargetHeight = radial(0x4c, 0x18) + firstTargetDistance * 0.0f;
    const float lastTargetHeight = radial(0x64, 0x30) + lastTargetDistance * 0.0f;
    const std::int32_t firstTargetAngle = angle(0x48, 0x14), lastTargetAngle = angle(0x60, 0x2c);
    const float eyeDistance = CameraLerpWithEpsilon(firstEyeDistance, lastEyeDistance, progress);
    const float eyeHeight = CameraLerpWithEpsilon(firstEyeHeight, lastEyeHeight, progress);
    const std::uint16_t eyeAngle = static_cast<std::uint16_t>(CameraAngleLerpWithEpsilon(firstEyeAngle, static_cast<std::int16_t>(lastEyeAngle), progress));
    const float targetDistance = CameraLerpWithEpsilon(firstTargetDistance, lastTargetDistance, progress);
    const float targetHeight = CameraLerpWithEpsilon(firstTargetHeight, lastTargetHeight, progress);
    const std::uint16_t targetAngle =
        static_cast<std::uint16_t>(CameraAngleLerpWithEpsilon(firstTargetAngle, static_cast<std::int16_t>(lastTargetAngle), progress));
    eye = {-(FixedAngleSin(eyeAngle) * eyeDistance), eyeHeight, -(FixedAngleCos(eyeAngle) * eyeDistance)};
    target = {-(FixedAngleSin(targetAngle) * targetDistance), targetHeight, -(FixedAngleCos(targetAngle) * targetDistance)};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(state.actorTarget.x, state.actorTarget.y, state.actorTarget.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.actorDirection.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(state.actorDirection.x));
    Matrix4x4::TransformPoint(target, target);
    Matrix4x4::Pop(1);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(state.actorEye.x, state.actorEye.y, state.actorEye.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.actorDirection.y + 0x8000));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-state.actorDirection.x));
    Matrix4x4::TransformPoint(eye, eye);
    Matrix4x4::Pop(1);
    fieldOfView = static_cast<std::uint16_t>(CameraAngleLerpWithEpsilon(ConversationBankI16(descriptor, 0), ConversationBankI16(descriptor, 2), progress));
}

void GenerateConversationTypeEight(const ConversationCameraDescriptor &descriptor, Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    const ConversationCameraState &state = conversationCamera;
    target = {
        (state.actorTarget.x + state.actorEye.x) * 0.5f, (state.actorTarget.y + state.actorEye.y) * 0.5f, (state.actorTarget.z + state.actorEye.z) * 0.5f};
    target.y -= ConversationBankFloat(descriptor, 12);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(target.x, target.y, target.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(ConversationBankI16(descriptor, 2) + state.actorYaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(ConversationBankI16(descriptor, 0)));
    Matrix4x4::preTranslate(0.0f, 0.0f, -ConversationBankFloat(descriptor, 8));
    eye = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    fieldOfView = static_cast<std::uint16_t>(ConversationBankI16(descriptor, 4));
}

void GenerateConversationCandidate(std::int32_t candidate, float progress, Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    if (candidate < 0 || candidate >= static_cast<std::int32_t>(ConversationDescriptors.size())) {
        eye = conversationCamera.previousEye;
        target = conversationCamera.previousTarget;
        fieldOfView = conversationCamera.previousFieldOfView;
        return;
    }
    const ConversationCameraDescriptor &descriptor = ConversationDescriptors[static_cast<std::size_t>(candidate)];
    switch (descriptor.flags & 0xfU) {
    case 1:
        GenerateConversationTypeOne(descriptor, progress, eye, target, fieldOfView);
        break;
    case 2:
        GenerateConversationTypeTwo(descriptor, progress, eye, target, fieldOfView);
        break;
    case 8:
        GenerateConversationTypeEight(descriptor, eye, target, fieldOfView);
        break;
    default:
        break;
    }
}

void CorrectConversationGround(Vector3 &eye, const Vector3 &target) {
    ConversationCameraState &state = conversationCamera;
    Vector3 player{};
    plymUpdatePlayerSearchLocation(&player);
    float maximumY = eye.y + 0.5f;
    if (player.y > maximumY)
        maximumY = player.y + 0.5f;
    const bool directCorrection = g_currentMap.name == SHEN_NAME(KWW4);
    if (directCorrection)
        maximumY += 3.0f;

    const float noIntersection = -std::numeric_limits<float>::max();
    const Vector3 probe{eye.x, maximumY, eye.z};
    float groundY = shenFindIntersectionWithScene(probe, maximumY);
    if (directCorrection && groundY != noIntersection && groundY + 0.0099999998f > eye.y) {
        eye.y = groundY + 0.0099999998f;
    }

    const float baseY = eye.y;
    float correctedY = baseY;
    float penetration{};
    bool clear = true;
    const auto applyGround = [&](float height) {
        if (!(height + 0.15000001f > correctedY))
            return;
        clear = false;
        penetration = height + 0.15000001f - baseY;
        correctedY = height + 0.050000001f + state.groundBias;
    };
    if (groundY != noIntersection)
        applyGround(groundY);

    if (state.descriptorFlags & 8U) {
        flddPushIntersectionFlag(8);
        Vector3 collision{};
        const Vector3 correctedEye{eye.x, correctedY, eye.z};
        if (RaycastForCollision(&collision, &correctedEye, &target)) {
            const Vector3 upperProbe{eye.x, correctedY + 1.5f, eye.z};
            const float alternate = shenFindIntersectionWithScene(upperProbe, correctedEye.z);
            applyGround(alternate == noIntersection ? collision.y : std::fmax(collision.y, alternate));
        }
    }

    state.groundOffset = CameraLerp(correctedY - baseY, state.groundOffset, camCtrlGetDeltaTime(0.25f));
    eye.y = baseY + state.groundOffset;
    const float penetrationDelta = penetration - state.groundPenetration;
    state.groundPenetration = penetration;
    if (clear) {
        state.groundBias = 0.1f;
    } else {
        state.groundBias = CameraLerp(penetrationDelta < 0.0f ? 0.1f : 0.0f, state.groundBias, camCtrlGetDeltaTime(0.02f));
    }
}

void ResetConversationCandidates() {
    std::uint8_t groupZero{};
    std::uint8_t groupOne{};
    for (std::size_t index = 0; index < conversationCamera.candidates.size(); ++index) {
        ConversationCandidateState &state = conversationCamera.candidates[index];
        state = {};
        const std::uint32_t flags = ConversationDescriptors[index].flags;
        if (flags & 1U) {
            state.group = 0;
            state.groupIndex = groupZero++;
        } else if (flags & 0xaU) {
            state.group = 1;
            state.groupIndex = groupOne++;
        } else {
            state.group = 0xff;
            state.groupIndex = 0xff;
        }
    }
}

// @verified - p
void InitializeConversationCameraState() {
    conversationCamera = {};
    conversationCamera.candidateIndex = 0;
    conversationCamera.collisionMode = 0;
    conversationCamera.groundBias = 0.0f;
    ResetConversationCandidates();
}

// @verified - p
void UpdateConversationAngleClass() {
    ConversationCameraState &state = conversationCamera;
    state.orientationClass = 0;
    if (!(state.flags & 0x00100000U) || state.root == state.specialTarget)
        return;
    const Vector3 origin = GetCharacterWorldPosition(state.root);
    Vector3 partner = GetCharacterWorldPosition(state.specialTarget);
    const Vector3 direction{partner.x - origin.x, partner.y - origin.y, partner.z - origin.z};
    const std::int16_t axis = static_cast<std::int16_t>(FixedAtan2(direction.x, direction.z) + 0x8000);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-axis));
    Matrix4x4::preTranslate(-origin.x, -origin.y, -origin.z);
    Vector3 local;
    Matrix4x4::TransformPoint(state.lookLocation, local);
    Matrix4x4::TransformPoint(partner, partner);
    Matrix4x4::Pop(1);
    std::int32_t kind;
    if (partner.z <= local.z) {
        if (local.z >= 0.0f) {
            const std::uint16_t angle = Abs16(FixedAtan2(local.x, local.z));
            kind = angle >= 5461 ? (angle < 10922) + 5 : 7;
        } else
            kind = 4;
    } else {
        const Vector3 delta{local.x - partner.x, local.y - partner.y, local.z - partner.z};
        const std::uint16_t angle = Abs16(FixedAtan2(delta.x, delta.z) + 0x8000);
        kind = angle >= 5461 ? (angle >= 10922) + 2 : 1;
    }
    if (local.x < 0.0f)
        kind = -kind;
    state.orientationClass = static_cast<std::int8_t>(kind);
}

// @verified - p
void FilterConversationSceneCandidates() {
    ConversationCameraState &state = conversationCamera;
    for (std::size_t index = 0; index < state.candidates.size(); ++index) {
        if (ConversationDescriptors[index].flags & 8U)
            state.candidates[index].flags |= 1U;
    }

    std::uint32_t required = state.shotType == 0 ? 1U : state.shotType == 1 ? 2U : 0U;
    std::uint32_t disallowed = (state.flags & 0x00040000U) ? 0x2040U : 0x40040U;
    if ((state.flags & 0x00040000U) && state.shotType == 1 && state.targetAnchors[static_cast<std::size_t>(state.shotIndex)] == state.root &&
        state.eyeAnchors[static_cast<std::size_t>(state.shotIndex)] != state.root) {
        disallowed = 0x2440U;
    }
    std::uint32_t angleMask{};
    if ((state.currentShotFlags & 8U) && state.shotType == 1) {
        const std::int32_t angleClass = std::abs(static_cast<std::int32_t>(conversationCamera.orientationClass));
        constexpr std::array<std::uint8_t, 8> angleGroups{
            0, 1, 2, 3, 3, 3, 2, 1};
        if (angleClass < static_cast<std::int32_t>(angleGroups.size()))
            angleMask = static_cast<std::uint32_t>(angleGroups[static_cast<std::size_t>(angleClass)] & 7U) << 24U;
    } else if ((state.currentShotFlags & 8U) == 0) {
        disallowed |= 0x07000000U;
    }
    if (plymHasRyoBag(0)) {
        disallowed |= 0x4000U;
        if (state.shotType == 1 && state.targetAnchors[static_cast<std::size_t>(state.shotIndex)] != state.root &&
            state.eyeAnchors[static_cast<std::size_t>(state.shotIndex)] == state.root) {
            disallowed |= 0x400U;
        }
    }
    if ((state.sceneFlags & 0x10U) && state.shotType == 0)
        disallowed |= 0xc00U;
    disallowed |= state.actorDistance < 0.89999998f ? 0x108000U : 0x300000U;
    if ((state.sceneFlags & 0x80U) == 0) {
        disallowed |= 0x400000U;
    } else {
        const std::uint32_t value = state.shotType == 1 ? required | 0x400000U : required;
        required = value;
    }
    if ((state.sceneFlags & 0x40U) == 0 || state.targetAnchors[static_cast<std::size_t>(state.shotIndex)] == state.root) {
        disallowed |= 0x800000U;
    } else {
        required |= 0x800000U;
    }
    if (state.sceneFlags & 8U) {
        if (state.shotType == 0 && state.targetAnchors[static_cast<std::size_t>(state.shotIndex)] == state.root) {
            disallowed |= 0x10001800U;
        } else if (state.shotType == 0) {
            required |= 0x10000000U;
        }
    } else {
        disallowed |= 0x10000000U;
    }
    if ((state.sceneFlags & 0x20U) && state.shotType == 0 && state.targetAnchors[static_cast<std::size_t>(state.shotIndex)] != state.root) {
        disallowed |= 0x1800U;
    }
    for (std::size_t index = 0; index < state.candidates.size(); ++index) {
        ConversationCandidateState &candidate = state.candidates[index];
        const std::uint32_t flags = ConversationDescriptors[index].flags;
        if ((flags & disallowed) != 0 || (required && (flags & required) != required) || (angleMask && (flags & 0x07000000U) != angleMask)) {
            candidate.flags |= 0x11U;
        }
    }
}

// @verified - p
void CheckConversationCandidateCollisions() {
    constexpr std::array<std::uint8_t, 16> collisionClasses{0, 1, 2, 7, 3, 1, 8, 5, 4, 1, 2, 7, 9, 9, 6, 10};
    for (std::size_t index = 0; index < conversationCamera.candidates.size(); ++index) {
        ConversationCandidateState &candidate = conversationCamera.candidates[index];
        if (candidate.flags & 1U)
            continue;
        if (candidate.group == 0) {
            Vector3 firstEye{}, firstTarget{}, lastEye{}, lastTarget{};
            std::uint16_t fov{};
            GenerateConversationCandidate(static_cast<std::int32_t>(index), 0.0f, firstEye, firstTarget, fov);
            GenerateConversationCandidate(static_cast<std::int32_t>(index), 1.0f, lastEye, lastTarget, fov);
            if (ConversationCollisionSegment(firstEye, lastEye, 0.22f))
                candidate.flags = 1U;
            continue;
        }
        if (candidate.group != 1)
            continue;
        std::array<Vector3, 5> eyes{}, targets{};
        std::uint16_t fov{};
        for (std::size_t sample = 0; sample < eyes.size(); ++sample)
            GenerateConversationCandidate(static_cast<std::int32_t>(index), static_cast<float>(sample) * 0.25f, eyes[sample], targets[sample], fov);
        std::array<bool, 5> targetHits{}, actorTargetHits{}, actorEyeHits{}, pointHits{};
        std::array<bool, 4> edgeHits{};
        for (std::size_t sample = 0; sample < eyes.size(); ++sample)
            targetHits[sample] = ConversationCollisionSegment(eyes[sample], targets[sample], 0.22f);
        for (std::size_t sample = 0; sample < eyes.size(); ++sample)
            actorTargetHits[sample] = ConversationCollisionSegment(eyes[sample], conversationCamera.actorTarget, 0.22f);
        for (std::size_t sample = 0; sample < eyes.size(); ++sample)
            actorEyeHits[sample] = ConversationCollisionSegment(eyes[sample], conversationCamera.actorEye, 0.22f);
        for (std::size_t window = 0; window < edgeHits.size(); ++window)
            edgeHits[window] = ConversationCollisionSegment(eyes[window], eyes[window + 1], 0.22f);
        for (std::size_t sample = 0; sample < eyes.size(); ++sample)
            pointHits[sample] = ConversationCollisionPoint(eyes[sample], 0.25f);
        std::uint8_t mask = 0;
        for (std::size_t window = 0; window < edgeHits.size(); ++window) {
            const std::size_t next = window + 1;
            if (!(targetHits[window] | targetHits[next] | actorTargetHits[window] | actorTargetHits[next] | actorEyeHits[window] | actorEyeHits[next] |
                    edgeHits[window] | pointHits[window] | pointHits[next]))
                mask |= static_cast<std::uint8_t>(1U << window);
        }
        candidate.collisionClass = collisionClasses[mask];
        if (!candidate.collisionClass)
            candidate.flags |= 3U;
    }
}

// @verified - p
bool AreConversationAngleCandidatesBlocked() {
    ResetConversationCandidates();
    constexpr std::array<std::uint8_t, 8> angleGroups{0, 1, 2, 3, 3, 3, 2, 1};
    const std::int32_t kind = std::abs(static_cast<std::int32_t>(conversationCamera.orientationClass));
    const std::uint32_t mask = std::uint32_t(angleGroups[kind] & 7U) << 24U;
    for (std::size_t index = 0; index < conversationCamera.candidates.size(); ++index) {
        const std::uint32_t flags = ConversationDescriptors[index].flags;
        if (!(flags & 2U) || !(flags & 0x07000000U) || (flags & 0x07000000U) != mask)
            conversationCamera.candidates[index].flags = 1;
    }
    CheckConversationCandidateCollisions();
    for (const ConversationCandidateState &candidate : conversationCamera.candidates)
        if (!(candidate.flags & 1U))
            return false;
    return true;
}

// @verified - p
void SelectConversationCameraSide() {
    ConversationCameraState &state = conversationCamera;
    const bool previousSide = state.side;
    if (state.sceneFlags & 8U) {
        const std::int32_t answerSide = drGetAnswerSide(nullptr);
        if (answerSide >= 1 && answerSide <= 3) {
            state.side = false;
            return;
        }
        if (answerSide == 4) {
            state.side = true;
            return;
        }
    }
    if (state.flags & 0x80000U) {
        state.side = state.orientationClass >= 0;
        if (!AreConversationAngleCandidatesBlocked())
            return;
    }
    constexpr shen_name CHARACTER_CONVERSATION_CLJ{SHEN_NAME(CLJ_)};
    if (state.specialTargetName.name == CHARACTER_CONVERSATION_CLJ.name) {
        state.side = false;
        return;
    }
    if (!(state.flags & 4U)) {
        state.side = previousSide;
        return;
    }
    if (state.sceneFlags & 0x40U) {
        state.side = true;
        return;
    }
    state.side = false;
    if (state.memberCount <= 1)
        return;
    Vector3 root, partner;
    GetCharacterMotionWorldPosition(state.root, 5, &root);
    GetCharacterMotionWorldPosition(state.specialTarget, 5, &partner);
    const Vector3 direction{partner.x - root.x, partner.y - root.y, partner.z - root.z};
    Vector3s angles;
    Vector3_ToFixed16EulerAngles(direction, angles);
    const float spacing = Vector3::CalcMagnitude(direction) / 11.0f;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(root.x, root.y, root.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angles.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(angles.x));
    std::int32_t positiveHits = 0, negativeHits = 0;
    for (std::int32_t depth = 0; depth < 15; ++depth) {
        const float z = -(static_cast<float>(depth - 2) * spacing);
        for (std::int32_t side = 0; side < 10; ++side) {
            Vector3 point{static_cast<float>(side) * 0.2f, 0.0f, z};
            Matrix4x4::TransformPoint(point, point);
            state.collisionMode |= 4U;
            if (ConversationCollisionSegment(root, point, 0.1f))
                ++positiveHits;
            point = {static_cast<float>(side) * -0.2f, 0.0f, z};
            Matrix4x4::TransformPoint(point, point);
            state.collisionMode |= 4U;
            if (ConversationCollisionSegment(root, point, 0.1f))
                ++negativeHits;
        }
    }
    Matrix4x4::Pop(1);
    if (std::abs(positiveHits - negativeHits) > 30) {
        state.side = positiveHits >= negativeHits;
        return;
    }
    root = GetCharacterWorldPosition(state.root);
    partner = GetCharacterWorldPosition(state.specialTarget);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-ConversationActorYaw(state.root)));
    Matrix4x4::preTranslate(-root.x, -root.y, -root.z);
    Matrix4x4::TransformPoint(partner, partner);
    Matrix4x4::Pop(1);
    state.side = partner.x < 0.0f;
    if (std::fabs(static_cast<double>(partner.x)) < 0.001)
        state.side = false;
}

// @verified - p
void RetireRecentConversationCandidates() {
    for (const auto &history : conversationCamera.history) {
        if (history.candidate >= 0 && history.candidate < static_cast<std::int32_t>(conversationCamera.candidates.size())) {
            conversationCamera.candidates[static_cast<std::size_t>(history.candidate)].flags |= 5U;
        }
    }
}

// @verified - p
void RestrictConversationCandidateAngles() {
    ConversationCameraState &state = conversationCamera;
    const std::uint32_t mask = (state.flags & 0x40000U) && state.actorHeightDelta > 0.0f && state.shotType == 1 ? 0x40000U : 0U;
    std::uint32_t admittedFlags = 0;
    for (std::size_t index = 0; index < state.candidates.size(); ++index) {
        if (!(state.candidates[index].flags & 1U))
            admittedFlags |= mask & ConversationDescriptors[index].flags;
    }
    if (!(admittedFlags & 0x07000000U))
        return;
    for (std::size_t index = 0; index < state.candidates.size(); ++index) {
        if ((ConversationDescriptors[index].flags & 0x07000000U) == (admittedFlags & 0x07000000U))
            state.candidates[index].flags &= ~1U;
        else
            state.candidates[index].flags |= 1U;
    }
}

// @verified - p
void ReadmitConversationSearchCandidates() {
    ConversationCameraState &state = conversationCamera;
    for (const ConversationCandidateState &candidate : state.candidates)
        if (!(candidate.flags & 1U))
            return;
    if ((state.sceneFlags & 0x10U) || !plymIsSearchStartRequested())
        return;
    const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
    for (std::size_t index = 0; index < state.candidates.size(); ++index) {
        if ((ConversationDescriptors[index].flags & 0x40U) && state.candidates[index].group == static_cast<std::uint8_t>(state.shotType) &&
            (!((state.sceneFlags & 0x01000000U) && state.shotType == 1 && state.eyeAnchors[shot] != state.root && ConversationDescriptors[index].id == 304)))
            state.candidates[index].flags &= ~1U;
    }
}

// @verified - p
void FilterConversationCandidates() {
    ConversationCameraState &state = conversationCamera;
    if (state.flags & 0x2000U) {
        for (ConversationCandidateState &candidate : state.candidates)
            if (candidate.group || candidate.groupIndex)
                candidate.flags |= 1U;
        return;
    }
    if ((state.flags & 0x400U) && state.shotType == 1)
        state.flags |= 0x08000000U;
    if ((state.flags & 0x400000U) && state.shotType == 1) {
        for (std::size_t index = 0; index < state.candidates.size(); ++index) {
            if (state.candidates[index].group != 1 || !(ConversationDescriptors[index].flags & 8U))
                state.candidates[index].flags = 1U;
        }
        return;
    }
    FilterConversationSceneCandidates();
    CheckConversationCandidateCollisions();
    RetireRecentConversationCandidates();
    if ((state.sceneFlags & 8U) && !state.shotType && state.targetAnchors[static_cast<std::size_t>(state.shotIndex)] != state.root) {
        for (std::size_t index = 0; index < state.candidates.size(); ++index) {
            if (ConversationDescriptors[index].flags & 0x10000000U)
                state.candidates[index].flags &= ~1U;
            else
                state.candidates[index].flags |= 0x21U;
        }
    }
    RestrictConversationCandidateAngles();
    ReadmitConversationSearchCandidates();
}

// @verified - p
std::int32_t SelectConversationCandidate() {
    ConversationCameraState &state = conversationCamera;
    const std::size_t group =
        static_cast<std::uint8_t>(state.shotType) < state.groupSelections.size() ? static_cast<std::size_t>(static_cast<std::uint8_t>(state.shotType)) : 0;
    const std::uint8_t selection = ++state.groupSelections[group];
    std::array<std::int8_t, 75> available{};
    std::size_t count{};
    for (std::size_t index = 0; index < state.candidates.size(); ++index) {
        if ((state.candidates[index].flags & 1U) == 0)
            available[count++] = static_cast<std::int8_t>(index);
    }
    std::int32_t chosen{};
    if (state.shotType == 0 || state.shotType == 1 || state.shotType == 3) {
        if (count)
            chosen = available[selection % count];
        else {
            for (std::size_t index = 0; index < state.candidates.size(); ++index) {
                if (state.candidates[index].group == static_cast<std::uint8_t>(state.shotType) && state.candidates[index].groupIndex == (selection & 1U))
                    chosen = static_cast<std::int32_t>(index);
            }
        }
    }
    const ConversationShotHistory &previous = state.history[(state.historyCursor + state.history.size() - 1) % state.history.size()];
    if ((state.flags & 0x10000U) && previous.candidate >= 0 && previous.shotType == state.shotType) {
        chosen = previous.candidate;
    }
    if (state.forcedCandidate >= 0) {
        chosen = state.forcedCandidate;
        state.shotType = static_cast<std::int8_t>(state.candidates[static_cast<std::size_t>(chosen)].group);
        state.shotTypes[static_cast<std::size_t>(state.shotIndex)] = state.shotType;
    }
    state.candidateIndex = static_cast<std::int8_t>(chosen);
    return chosen;
}

void SetupProjection(std::int32_t pitch, float aspectRatio, float nearDistance, float farDistance) {
    Matrix4x4 projection{};
    std::int32_t halfPitch = pitch >> 1;
    if (((halfPitch - 0x4000) & 0xffff7fff) == 0) {
        halfPitch = (halfPitch & ~0xffff) | static_cast<std::uint16_t>(halfPitch - 1);
    }
    const float yScale = 1.0f / std::tan(FixedAngleToRadians(static_cast<std::uint16_t>(halfPitch)));
    projection.rows[0].x = yScale / aspectRatio;
    projection.rows[1].y = yScale;
    projection.rows[2].z = nearDistance / (farDistance - nearDistance);
    projection.rows[2].w = -1.0f;
    projection.rows[3].z = (nearDistance * farDistance) / ((farDistance - nearDistance) * 0.40000001f);
    Matrix4x4::Multiply(&projection);
    SetLegacyStripProjection(workTransformMatrix, nearDistance);
    g_projectionNearClip = nearDistance;
    g_projectionFarClip = farDistance;
}

void TransformBackwardDirection(Vector3 &output) {


    output = {-workTransformMatrix.rows[2].x, -workTransformMatrix.rows[2].y, -workTransformMatrix.rows[2].z};
}

void camCtrlComputeNextPosition(CameraRuntime &camera) {
    // camCtrlCompute dispatches the mode selected by the state
    // callback; mode zero must not rewrite the retained rotation/target.
    switch (camera.computeMode) {
    case CameraComputeMode::UsePreviousPosition:
        camera.nextPosition = camera.previousPosition;
        return;
    case CameraComputeMode::OrbitTarget: {
        if (camera.orbitDistance >= 0.0f) {
            const auto pitch = static_cast<std::int16_t>(-camera.nextRotation.pitch);
            const float horizontal = camera.orbitDistance * sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(pitch))));
            camera.nextPosition = {camera.targetPosition.x + sin_fixed16(camera.nextRotation.yaw) * horizontal,
                camera.targetPosition.y + sin_fixed16(pitch) * camera.orbitDistance,
                camera.targetPosition.z +
                    sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(camera.nextRotation.yaw)))) * horizontal};
        }
        camera.previousPosition = camera.nextPosition;
        break;
    }
    case CameraComputeMode::AnchorOffset:
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(camera.field1a8.x, camera.field1a8.y, camera.field1a8.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.yaw));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.pitch));
        Matrix4x4::preTranslate(0.0f, 0.0f, -camera.zOffset);
        camera.nextPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        return;
    default: {
        const float deltaX = camera.targetPosition.x - camera.previousPosition.x;
        const float deltaY = camera.targetPosition.y - camera.previousPosition.y;
        const float deltaZ = camera.targetPosition.z - camera.previousPosition.z;
        const float horizontal = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
        camera.nextPosition = camera.previousPosition;
        camera.nextRotation.pitch = static_cast<std::int16_t>(RadiansToFixedAngle(std::atan2(deltaY, horizontal)));
        camera.nextRotation.yaw = static_cast<std::int16_t>(RadiansToFixedAngle(std::atan2(deltaX, deltaZ)) + 0x8000);
        break;
    }
    }
    camera.nextTargetPosition = camera.targetPosition;
}

void UpdateCameraTransitions(CameraRuntime &camera) {
    const CameraInterpolationMode mode = camera.interpolationMode;
    float progress = 0.0f;
    switch (mode) {
    case CameraInterpolationMode::RestorePrevious:
    case CameraInterpolationMode::ToTargetContinuous:
        if (mode == CameraInterpolationMode::ToTargetContinuous && camera.computeMode != CameraComputeMode::OrbitTarget) {
            camera.interpolationMode = CameraInterpolationMode::None;
            return;
        }
        progress = CameraLerp(1.0f, camera.interpolationProgress, camera.interpolationEaseFactor);
        camera.interpolationProgress = progress;
        if (progress > 0.99989998f) {
            progress = 1.0f;
            camera.interpolationMode = CameraInterpolationMode::None;
        }
        break;
    case CameraInterpolationMode::PreviousToCurrent:
    case CameraInterpolationMode::ToTargetTimed:
        if (mode == CameraInterpolationMode::ToTargetTimed && camera.computeMode != CameraComputeMode::OrbitTarget) {
            camera.interpolationMode = CameraInterpolationMode::None;
            return;
        }
        if (++camera.transitionStep >= camera.transitionStepCount) {
            camera.transitionStep = camera.transitionStepCount;
            camera.interpolationMode = CameraInterpolationMode::None;
        }
        progress = static_cast<float>(camera.transitionStep) / static_cast<float>(camera.transitionStepCount);
        break;
    default:
        camera.interpolationMode = CameraInterpolationMode::None;
        return;
    }
    const float amount = progress * HLib::GetScaledDeltaTick();
    if (mode == CameraInterpolationMode::ToTargetContinuous || mode == CameraInterpolationMode::ToTargetTimed) {
        const Vector3 target{CameraLerp(camera.targetPosition.x, camera.transitionTargetPosition.x, amount),
            CameraLerp(camera.targetPosition.y, camera.transitionTargetPosition.y, amount),
            CameraLerp(camera.targetPosition.z, camera.transitionTargetPosition.z, amount)};
        const float distance = CameraLerp(camera.orbitDistance, camera.transitionTargetOrbitDistance, amount);
        const std::int16_t pitch = CameraLerpAngle(camera.nextRotation.pitch, camera.transitionTargetRotation.pitch, amount);
        const std::int16_t yaw = CameraLerpAngle(camera.nextRotation.yaw, camera.transitionTargetRotation.yaw, amount);
        const float horizontal = distance * sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(-pitch))));
        if (distance >= 0.0f) {
            camera.nextPosition = {target.x + sin_fixed16(yaw) * horizontal, target.y + sin_fixed16(static_cast<std::int16_t>(-pitch)) * distance,
                target.z + sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw)))) * horizontal};
        }
        camera.nextRotation.pitch = pitch;
        camera.nextRotation.yaw = yaw;
    } else {
        const auto interpolate = [amount, mode](float target, float base) {
            if (mode == CameraInterpolationMode::PreviousToCurrent &&
                (std::abs(amount) < std::numeric_limits<float>::epsilon() || std::abs(target - base) < std::numeric_limits<float>::epsilon()))
                return base;
            return CameraLerp(target, base, amount);
        };
        camera.nextPosition = {interpolate(camera.nextPosition.x, camera.previousCameraPosition.x),
            interpolate(camera.nextPosition.y, camera.previousCameraPosition.y), interpolate(camera.nextPosition.z, camera.previousCameraPosition.z)};
        camera.nextRotation.pitch = CameraTransitionAngle(camera.nextRotation.pitch, camera.transitionTargetRotation.pitch, amount, mode);
        camera.nextRotation.yaw = CameraTransitionAngle(camera.nextRotation.yaw, camera.transitionTargetRotation.yaw, amount, mode);
    }
    camera.nextRotation.roll = CameraTransitionAngle(camera.nextRotation.roll, camera.transitionTargetRotation.roll, amount, mode);
    camera.currentFovAngle =
        static_cast<std::uint16_t>(CameraTransitionAngle(static_cast<std::int16_t>(camera.currentFovAngle), camera.transitionTargetFovAngle, amount, mode));
}

void UpdateLockedOnCamera(CameraRuntime &camera) {
    camera.flags &= ~CameraInterpolationSnap;
    const LegacyInputSlot *const input = CameraInput();
    if (!input)
        return;
    SearchCameraState &state = searchCamera;
    if (state.phase == 1) {
        const float vertical = state.lookTarget.y - state.lookBase.y;
        float radius = 0.75f;
        if (static_cast<std::uint32_t>(state.fovStyle - 2) > 3U) {
            state.fov = 10922;
            state.zoomFov = IsSpecialSignModeForCurrentArea() ? 2730 : 5461;
        } else {
            radius = CameraDistance(state.lookTarget, state.lookBase);
            if (state.fovStyle == 5) {
                state.fov = 10012;
                state.zoomFov = 5461;
            } else {
                const float scale = state.fovStyle == 2 ? 0.69999999f : state.fovStyle == 4 ? 1.15f : 0.85000002f;
                const float distanceScale = (std::sqrt(radius) - 2.0f) * 0.30000001f;
                state.fov = std::max(3640, static_cast<std::int32_t>(FixedAtan2(std::fmax(scale, distanceScale) * 0.57735002f, radius)));
                state.zoomFov = std::max(1820, static_cast<std::int32_t>(FixedAtan2(0.28867501f, radius)));
            }
        }
        std::uint16_t pitch = static_cast<std::uint16_t>(FixedAtan2(vertical, radius));
        if (!(state.flags & 1U)) {
            if (vertical > 0.0f && pitch > 3640U)
                pitch = 3640;
            else if (vertical < 0.0f && pitch < 61895U)
                pitch = 61895;
        }
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(state.lookBase.x, state.lookBase.y, state.lookBase.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.rotationY));
        Matrix4x4::RotateAroundX_Wrapper(pitch);
        Matrix4x4::preTranslate(0.0f, 0.0f, -radius - 7.5f);
        state.offsetPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        state.cameraPosition = state.lookTarget;
        camera.previousPosition = state.cameraPosition;
        camera.targetPosition = state.lookBase;
        if (state.flags & 2U)
            camCtrlSetFovTransitionTo(true);
        else
            camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
        camera.currentFovAngle = 10922;
        camera.interpolationEaseFactor = static_cast<std::uint32_t>(state.fovStyle - 2) > 3U ? 0.25f : SearchCameraEase(camera);
        state.distance = state.field44[0] = 7.5f;
        state.field44[1] = 0.0f;
        state.field3a = state.field3e = 0;
        state.phase = 2;
    } else if (state.phase == 2) {
        camera.previousPosition = state.cameraPosition;
        state.phase = 3;
        camCtrlGetRot(&state.initialRotation);
    } else if (state.phase == 3 && state.fovStyle == 6) {
        state.cameraPosition = state.lookTarget;
        camera.computeMode = CameraComputeMode::FixedPositionLookAtTarget;
        camera.previousPosition = state.cameraPosition;
        camera.targetPosition = state.lookBase;
        camera.currentFovAngle = 10922;
        camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
        camera.interpolationEaseFactor = SearchCameraEase(camera);
        state.distance = state.field44[0] = 7.5f;
        state.field44[1] = 0.0f;
        state.field3a = state.field3e = 0;
    }
    if (!state.phase) {
        state.zoomScale = -1.0f;
        state.pitch = state.yaw = 0;
    } else {
        const std::int32_t yaw = state.zoomScale >= 0.0f ? (128 - static_cast<std::int32_t>(input->analog0e)) * 16384 / 128 : 0;
        const std::int32_t pitch = state.zoomScale >= 0.0f ? (128 - static_cast<std::int32_t>(input->analog0f)) * 10922 / 128 : 0;
        state.yaw = static_cast<std::int16_t>(static_cast<std::int32_t>(CameraLerp(static_cast<float>(yaw), static_cast<float>(state.yaw), 0.059999999f)));
        state.pitch =
            static_cast<std::int16_t>(static_cast<std::int32_t>(CameraLerp(static_cast<float>(pitch), static_cast<float>(state.pitch), 0.059999999f)));
        const float zoom = static_cast<std::uint32_t>(state.fovStyle - 6) <= 1U ?
            static_cast<float>(AreS2TriggersSwapped() ? input->analog0c : input->analog0d) :
            static_cast<float>(plymGetLookZoomScale()) * 0.00390625f;
        const float requestedZoom = std::fmin(1.0f, std::fmax(0.0f, zoom));
        if (state.zoomScale >= 0.0f)
            state.zoomScale = CameraLerp(requestedZoom, state.zoomScale, 0.2f);
        else
            state.zoomScale = requestedZoom < 0.039999999f ? 0.0f : -0.001f;
        Vector3 target = state.lookBase;
        Vector3 position = state.cameraPosition;
        Vector3 pivot = state.lookTarget;
        if (state.lockedOnAngle > 0.0f) {
            const float distance = CameraDistance(position, state.lookBase);
            const float maximum = state.lockedOnAngle / std::tan(FixedAngleToRadians(5461));
            if (distance > maximum) {
                const float factor = (maximum - distance) / distance;
                position.x += factor * (position.x - state.lookBase.x);
                position.y += factor * (position.y - state.lookBase.y);
                position.z += factor * (position.z - state.lookBase.z);
            }
            pivot = position;
        }
        const float sine = sin_fixed16(static_cast<std::int16_t>(state.pitch));
        const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(state.pitch))));
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(pivot.x, pivot.y, pivot.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.yaw));
        Matrix4x4_Push();
        const float distance = CameraDistance(position, pivot);
        Matrix4x4::preTranslate(cosine * (position.x - pivot.x), position.y - pivot.y, cosine * (position.z - pivot.z));
        position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y + distance * sine, workTransformMatrix.rows[3].z};
        position.y = std::fmax(position.y, pivot.y - 1.5f);
        Matrix4x4::Pop(1);
        const float targetDistance = CameraDistance(target, pivot);
        Matrix4x4::preTranslate(cosine * (target.x - pivot.x), target.y - pivot.y, cosine * (target.z - pivot.z));
        target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y + targetDistance * sine, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        camera.currentFovAngle = static_cast<std::uint16_t>(
            state.fov - (state.zoomScale > 0.0f ? static_cast<std::int32_t>(static_cast<float>(state.fov - state.zoomFov) * state.zoomScale) : 0));
        if (state.fovStyle != 6) {
            camera.previousPosition = position;
            camera.targetPosition = target;
        }
    }
    if (static_cast<std::uint32_t>(state.fovStyle - 2) <= 3U && state.phase) {
        if (const Character *const root = CameraRootCharacter())
            ctrlSetLookOverlayWeight(std::fmax(0.0f, root->alpha - 0.1f));
    }
    state.flags &= ~2U;
}

void camCtrlGetIdealCameraPosition(CameraRuntime &camera) {


    const auto scaleTo = [](Vector3 &vector, float magnitude) {
        const float length = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
        if (length != 0.0f) {
            const float scale = magnitude / length;
            vector.x *= scale;
            vector.y *= scale;
            vector.z *= scale;
        }
    };
    float limit = camera.orbitDistance;
    const float distance = limit < 0.0f ? 0.1f : limit;
    const auto pitch = static_cast<std::int16_t>(-camera.nextRotation.pitch);
    const float horizontal = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(pitch)))) * distance;
    const auto yaw = camera.nextRotation.yaw;
    Vector3 position{sin_fixed16(yaw) * horizontal + camera.targetPosition.x, sin_fixed16(pitch) * distance + camera.targetPosition.y,
        sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw)))) * horizontal + camera.targetPosition.z};
    Vector3 target = camera.targetPosition;
    const float yawDifference = std::fmin(1.0f, std::fmax(0.0f, static_cast<float>(std::abs(static_cast<std::int16_t>(yaw - plymGetAngY()))) * 0.00012207031f));
    const float speedCap = (std::fmax(1.0f, plymGetSpeedZ()) - 1.0f) * 0.40000001f * (1.0f - yawDifference) + 0.2f;
    if (camera.orbitDistance > speedCap) {
        Vector3 delta{target.x - position.x, target.y - position.y, target.z - position.z};
        scaleTo(delta, -speedCap);
        target.x += delta.x;
        target.y += delta.y;
        target.z += delta.z;
    }

    Vector3 hit{};
    if (RaycastForCollision(&hit, &position, &target)) {
        if (CameraDistance(target, hit) < 0.1f) {
            Vector3 delta{hit.x - target.x, hit.y - target.y, hit.z - target.z};
            scaleTo(delta, 0.0099999998f);
            const Vector3 start{hit.x + delta.x, hit.y + delta.y, hit.z + delta.z};
            Vector3 precise{};
            if (RaycastForCollision(&precise, &position, &start))
                hit = precise;
        }
        Vector3 delta{target.x - hit.x, target.y - hit.y, target.z - hit.z};
        const Vector3 &normal = flddGetLastIntersection().planeNormal;
        const float product =
            std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) * std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        std::uint16_t angle = 0;
        if (std::abs(product) >= 0.00000011920929f) {
            const float ratio = (delta.x * normal.x + delta.y * normal.y + delta.z * normal.z) / product;
            if (std::abs(ratio) > 1.0f) {
                angle = ratio > 0.0f ? 0 : 0x8000U;
            } else {
                const float adjacent = std::sqrt(1.0f - ratio * ratio);
                const auto sineAngle = adjacent == 0.0f ?
                    static_cast<std::uint16_t>(ratio > 0.0f ? 0x4000 : 0xc000) :
                    static_cast<std::uint16_t>(static_cast<std::int32_t>(std::atan(ratio / adjacent) * 65536.0f / 6.2831855f + 0.5f));
                angle = static_cast<std::uint16_t>(16384 - sineAngle);
            }
        }
        const float cosine = sin_fixed16(static_cast<std::int16_t>(16384 - angle));
        const float offset = std::abs(cosine) < 0.00000011920929f ? 0.0f : std::fmin(0.2f / cosine, 0.30000001f);
        scaleTo(delta, 1.0f);
        position = {hit.x + delta.x * offset, hit.y + delta.y * offset, hit.z + delta.z * offset};
    } else {
        Vector3 rightStart{0.2f, 0.0f, -distance};
        Vector3 leftStart{-0.2f, 0.0f, -distance};
        Vector3 rightEnd{0.2f, 0.0f, 0.0f};
        Vector3 leftEnd{-0.2f, 0.0f, 0.0f};
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(target.x, target.y, target.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw + 32768));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pitch));
        const auto transformPoint = [](const Vector3 &point) {
            const auto &rows = workTransformMatrix.rows;
            return Vector3{point.x * rows[0].x + point.y * rows[1].x + point.z * rows[2].x + rows[3].x,
                point.x * rows[0].y + point.y * rows[1].y + point.z * rows[2].y + rows[3].y,
                point.x * rows[0].z + point.y * rows[1].z + point.z * rows[2].z + rows[3].z};
        };
        rightStart = transformPoint(rightStart);
        leftStart = transformPoint(leftStart);
        rightEnd = transformPoint(rightEnd);
        leftEnd = transformPoint(leftEnd);
        Matrix4x4::Pop(1);
        float rightDistance = distance;
        float leftDistance = distance;
        if (RaycastForCollision(&hit, &rightStart, &rightEnd)) {
            rightDistance = CameraDistance(rightEnd, hit);
            if (rightDistance < 0.1f)
                rightDistance = distance;
        }
        if (RaycastForCollision(&hit, &leftStart, &leftEnd)) {
            leftDistance = CameraDistance(leftEnd, hit);
            if (leftDistance < 0.1f)
                leftDistance = distance;
        }
        if (!(distance > rightDistance || distance > leftDistance))
            return;
        limit = std::fmin(rightDistance, leftDistance);
    }
    camera.orbitDistance = CameraDistance(camera.targetPosition, position);
    if (camera.orbitDistance > limit)
        camera.orbitDistance = limit;
}

void camCtrlCheckCollision(CameraRuntime &camera) {
    // camCtrlCheckCollision casts back from the requested camera
    // position toward its target; FLDD keeps its own front-face convention.
    camera.flags &= ~CameraRaycastHit;
    const auto mode = static_cast<std::uint8_t>(camera.computeMode);
    if (!(camera.flags & CameraRaycastEnabled) || mode < 1 || mode > 4 || camera.computeMode == CameraComputeMode::AnchorOffset) {
        return;
    }
    Vector3 direction{camera.nextTargetPosition.x - camera.nextPosition.x, camera.nextTargetPosition.y - camera.nextPosition.y,
        camera.nextTargetPosition.z - camera.nextPosition.z};
    const float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (distance != 0.0f) {
        direction.x /= distance;
        direction.y /= distance;
        direction.z /= distance;
    }
    Vector3 rayEnd = camera.nextTargetPosition;
    if (distance > 0.5f) {
        rayEnd.x -= direction.x * 0.029999999f;
        rayEnd.y -= direction.y * 0.029999999f;
        rayEnd.z -= direction.z * 0.029999999f;
    }
    Vector3 intersection{};
    if (RaycastForCollision(&intersection, &camera.nextPosition, &rayEnd)) {
        camera.nextPosition = {
            intersection.x + direction.x * 0.0099999998f, intersection.y + direction.y * 0.0099999998f, intersection.z + direction.z * 0.0099999998f};
        camera.flags |= CameraRaycastHit;
    }
}

// @verified - p
void camCtrlInterpolateNextPositionRotation(CameraRuntime &camera) {
    if (!(camera.flags & CameraInterpolationSnap) || (camera.flags & CameraDisableInterpolation)) {
        return;
    }
    const float positionRate = shenGetScaledDeltaTickTime(0.23f);
    const auto interpolatePosition = [positionRate](float &next, float &current) {
        if (std::fabs(next - current) < 0.0001f)
            current = next;
        next = (next - current) * positionRate + current;
    };
    interpolatePosition(camera.nextPosition.x, camera.currentPosition.x);
    interpolatePosition(camera.nextPosition.y, camera.currentPosition.y);
    interpolatePosition(camera.nextPosition.z, camera.currentPosition.z);
    const auto interpolateAngle = [](std::int16_t next, std::int16_t &current, float rate) {
        if (std::abs(static_cast<int>(next) - current) < 2)
            current = next;
        return CameraLerpAngle(next, current, rate);
    };
    const float angleRate = shenGetScaledDeltaTickTime(0.30000001f);
    camera.nextRotation.pitch = interpolateAngle(camera.nextRotation.pitch, camera.currentRotation.pitch, angleRate);
    camera.nextRotation.yaw = interpolateAngle(camera.nextRotation.yaw, camera.currentRotation.yaw, angleRate);
    camera.nextRotation.roll = interpolateAngle(camera.nextRotation.roll, camera.currentRotation.roll, angleRate);
    camera.currentFovAngle = static_cast<std::uint16_t>(
        interpolateAngle(static_cast<std::int16_t>(camera.currentFovAngle), camera.currentFovInterpolationAngle, shenGetScaledDeltaTickTime(0.5f)));
}

void camCtrlApply(CameraRuntime &camera) {
    // camCtrlApply publishes both generations before projection.
    camera.interpolationScale = camera.motionVector;
    camera.motionScale = camera.adjustedMotionVector;
    camera.motionVector = {camera.nextPosition.x - camera.position.x, camera.nextPosition.y - camera.position.y, camera.nextPosition.z - camera.position.z};
    camera.adjustedMotionVector = {
        camera.targetPosition.x - camera.target.x, camera.targetPosition.y - camera.target.y, camera.targetPosition.z - camera.target.z};
    camera.motionAngles = {static_cast<std::int16_t>(camera.nextRotation.pitch - camera.rotation.pitch),
        static_cast<std::int16_t>(camera.nextRotation.yaw - camera.rotation.yaw), static_cast<std::int16_t>(camera.nextRotation.roll - camera.rotation.roll)};
}

// @verified - p
void RefreshConversationSceneFlags() {
    ConversationCameraState &state = conversationCamera;
    state.sceneFlags &= 7U;
    if (state.flags & 0x80000U)
        state.sceneFlags |= 2U;
    if (QueryNpcRouteLiveState(state.specialTargetName) == 13)
        state.sceneFlags |= 8U;
    Vector3 root, partner;
    GetCharacterMotionWorldPosition(state.root, 6, &root);
    GetCharacterMotionWorldPosition(state.specialTarget, 6, &partner);
    flddPushIntersectionFlag(4096);
    if (flddFindIntersections(root, partner))
        state.sceneFlags |= 0x20U;
    if (QueryNpcCameraMotionProfile(state.specialTargetName, 1))
        state.sceneFlags |= 0x80U;
    if (QueryNpcCameraMotionProfile(state.specialTargetName, 2))
        state.sceneFlags |= 0x40U;
    if (QueryNpcCameraMotionProfile(state.specialTargetName, 4))
        state.sceneFlags = (state.sceneFlags & ~0x1c0U) | 0x100U;
    if (QueryNpcSceneMotionPlaying(state.specialTargetName))
        state.sceneFlags |= 0x200U;
    if (plymCheckAuthorTargetRequested())
        state.sceneFlags |= 0x10U;
    if (state.sceneFlags & 0x10U)
        state.flags |= 0x400000U;
    state.sceneFlags &= ~0x01000400U;
    if (QueryNpcCameraMotionProfile(state.specialTargetName, 8))
        state.sceneFlags |= 0x400U;
    if ((state.currentShotFlags & 8U) && (state.sceneFlags & 0x400U))
        state.sceneFlags |= 0x01000000U;
}

void BindConversationShotActors() {
    ConversationCameraState &state = conversationCamera;
    const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
    state.packetSpeaker = state.shots[shot]->speaker;
    state.packetListener = state.shots[shot]->listener;
    Character *const speaker = state.targetAnchors[shot];
    Character *const listener = state.eyeAnchors[shot];
    state.specialTarget = listener == state.root ? speaker : listener;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    state.specialTargetName = scenGetCharaName(control->scenarioTask, state.specialTarget);
    state.targetScenarioName = scenGetCharaName(control->scenarioTask, speaker);
    state.eyeScenarioName = scenGetCharaName(control->scenarioTask, listener);
    if (speaker == listener || !IsCharacterInCurrentMap(speaker) || !IsCharacterInCurrentMap(listener))
        state.flags |= 0x10U;
}

// @verified - p
void ProjectConversationActorToScreen(const Vector3 &world, Vector3 &screen) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(camCtrlGetProjectionMatrix(nullptr));
    Matrix4x4::TransformPoint(world, screen);
    Matrix4x4::Pop(1);
    if (std::fabs(screen.z) >= std::numeric_limits<float>::epsilon()) {
        const float scale = g_cameraControl->viewHeightScale / -screen.z;
        screen.x = scale * screen.x;
        screen.y = scale * screen.y;
    }
}

void ReadConversationActorPose() {
    ConversationCameraState &state = conversationCamera;
    const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
    Character *const speaker = state.targetAnchors[shot];
    Character *const listener = state.eyeAnchors[shot];
    if (state.shotType == 3) {
        Vector3 sum{};
        std::int32_t count = 0;
        for (std::int32_t member = 0; member < state.memberCount; ++member) {
            Character *const character = state.members[static_cast<std::size_t>(member)];
            if (character == listener)
                continue;
            const Vector3 position = ConversationActorPosition(character);
            sum.x += position.x;
            sum.y += position.y;
            sum.z += position.z;
            ++count;
        }
        state.actorTarget = {sum.x / static_cast<float>(count), sum.y / static_cast<float>(count), sum.z / static_cast<float>(count)};
        state.actorEye = ConversationActorPosition(listener);
        state.actorYaw = ConversationActorYaw(listener);
    } else {
        state.actorTarget = ConversationActorPosition(speaker);
        state.actorEye = ConversationActorPosition(listener);
        state.actorYaw = ConversationActorYaw(speaker);
        if (state.sceneFlags & 0x10U)
            (void)plymGetAuthorPlacementYaw(state.specialTargetName);
    }
}

void SmoothConversationActorPose() {
    ConversationCameraState &state = conversationCamera;
    if (state.flags & 0x40U)
        return;
    Matrix4x4_Push();
    const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
    Matrix4x4::SetActiveTransform(charaGetBoneTransform(FindMotionWork(state.targetAnchors[shot]), 5));
    Matrix4x4::preTranslate(0.06549f, 0.0f, 0.081969999f);
    const Vector3 head{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    Vector3 screen;
    ProjectConversationActorToScreen(head, screen);
    const float margin = std::fmax(std::fabs(0.06549f * 0.1f), 16.0f);
    std::uint32_t mode = std::fabs(screen.x) > static_cast<float>(d3t::g_originalScreenWidth) * 0.5f - margin ||
            std::fabs(screen.y) > static_cast<float>(d3t::g_originalScreenHeight) * 0.5f - margin ?
        2U :
        0U;
    if (state.flags & 0x8000U)
        mode = 0;
    if ((state.currentShotFlags & 8U) && (state.descriptorFlags & 0x400U))
        mode |= 4U;
    if (state.sceneFlags & 0x01000010U)
        mode |= 1U;
    if (mode) {
        if ((state.flags & 0x800U) && state.actorSmoothingFrames > 30)
            state.actorSmoothingFrames = 30;
        ++state.actorSmoothingFrames;
        state.flags |= 0x800U;
        float factor = std::fmax(0.0f, static_cast<float>(state.actorSmoothingFrames) / 60.0f);
        if (factor >= 1.0f)
            factor = 1.0f;
        const std::int32_t angle = static_cast<std::int32_t>((factor * 180.0f * 8192.0f) / 45.0f);
        factor = ((1.0f - FixedAngleCos(static_cast<std::uint16_t>(angle))) * 1.0f) * 0.5f;
        if (state.actorSmoothingFrames >= 60)
            state.actorSmoothingFrames = 60;
        if (state.sceneFlags & 0x01000010U)
            factor = 1.0f;
        const Vector3 targetDelta{state.actorTarget.x - state.previousActorTarget.x, state.actorTarget.y - state.previousActorTarget.y,
            state.actorTarget.z - state.previousActorTarget.z};
        const Vector3 eyeDelta{
            state.actorEye.x - state.previousActorEye.x, state.actorEye.y - state.previousActorEye.y, state.actorEye.z - state.previousActorEye.z};
        const auto speed = [](float value, float maximum, float minimum, float range, float width) {
            if (!(value >= 0.0099999998f))
                return minimum;
            if (value >= maximum)
                return 1.0f;
            return ((value - 0.0099999998f) / width) * range + minimum;
        };
        if (mode & 3U) {
            const float vertical = speed(std::fabs(targetDelta.y), 0.30000001f, 0.25f, 0.75f, 0.29000002f);
            const float horizontal = speed(std::sqrt(targetDelta.z * targetDelta.z + targetDelta.x * targetDelta.x), 0.1f, 0.25f, 0.75f, 0.090000004f);
            const float amount = horizontal * (factor * 0.5f);
            state.actorTarget.x = CameraLerp(state.actorTarget.x, state.previousActorTarget.x, camCtrlGetDeltaTime(amount));
            state.actorTarget.y = CameraLerp(state.actorTarget.y, state.previousActorTarget.y, camCtrlGetDeltaTime((factor * 0.5f) * vertical));
            state.actorTarget.z = CameraLerp(state.actorTarget.z, state.previousActorTarget.z, camCtrlGetDeltaTime(amount));
        } else
            state.actorTarget = state.previousActorTarget;
        if (mode & 5U) {
            const float vertical = speed(std::fabs(eyeDelta.y), 0.30000001f, 0.15000001f, 0.85000002f, 0.29000002f);
            const float horizontal = speed(std::sqrt(eyeDelta.z * eyeDelta.z + eyeDelta.x * eyeDelta.x), 0.1f, 0.25f, 0.75f, 0.090000004f);
            const float half = factor * 0.5f;
            const float amount = horizontal * half;
            state.actorEye.x = CameraLerp(state.actorEye.x, state.previousActorEye.x, camCtrlGetDeltaTime(amount));
            state.actorEye.y = CameraLerp(state.actorEye.y, state.previousActorEye.y, camCtrlGetDeltaTime(half * vertical));
            state.actorEye.z = CameraLerp(state.actorEye.z, state.previousActorEye.z, camCtrlGetDeltaTime(amount));
        } else
            state.actorEye = state.previousActorEye;
        state.actorYaw = CameraLerpAngle(state.actorYaw, state.previousActorYaw, camCtrlGetDeltaTime(0.02f));
    } else {
        if (!state.shotIndex) {
            state.actorTarget = {CameraLerp(state.actorTarget.x, state.previousActorTarget.x, camCtrlGetDeltaTime(0.02f)),
                CameraLerp(state.actorTarget.y, state.previousActorTarget.y, camCtrlGetDeltaTime(0.02f)),
                CameraLerp(state.actorTarget.z, state.previousActorTarget.z, camCtrlGetDeltaTime(0.02f))};
            state.actorEye = {CameraLerp(state.actorEye.x, state.previousActorEye.x, camCtrlGetDeltaTime(0.02f)),
                CameraLerp(state.actorEye.y, state.previousActorEye.y, camCtrlGetDeltaTime(0.02f)),
                CameraLerp(state.actorEye.z, state.previousActorEye.z, camCtrlGetDeltaTime(0.02f))};
        } else {
            state.actorTarget = state.previousActorTarget;
            state.actorEye = state.previousActorEye;
        }
        state.actorSmoothingFrames = 0;
    }
}

void UpdateConversationActorPose() {
    ConversationCameraState &state = conversationCamera;
    state.flags &= ~0x40000U;
    state.sceneFlags &= ~1U;
    state.previousActorTarget = state.actorTarget;
    state.previousActorEye = state.actorEye;
    state.previousActorYaw = state.actorYaw;
    if (!(state.flags & 0x10U)) {
        ReadConversationActorPose();
        SmoothConversationActorPose();
    } else {
        state.actorTarget = ConversationActorPosition(state.root);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(state.actorTarget.x, state.actorTarget.y, state.actorTarget.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(ConversationActorYaw(state.root)));
        Matrix4x4::preTranslate(0.0f, 0.0f, -3.0f);
        state.actorEye = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        state.actorYaw = ConversationActorYaw(state.root);
    }
    const float dx = state.actorTarget.x - state.actorEye.x, dz = state.actorTarget.z - state.actorEye.z;
    state.actorDistance = std::sqrt(dx * dx + dz * dz);
    state.actorHeightDelta = state.actorEye.y - state.actorTarget.y;
    const Vector3 rootPosition = ConversationActorPosition(state.root);
    const Vector3 partnerPosition = ConversationActorPosition(state.specialTarget);
    state.rootPartnerHeightDelta = partnerPosition.y - rootPosition.y;
    const Vector3 direction{state.actorEye.x - state.actorTarget.x, state.actorEye.y - state.actorTarget.y, state.actorEye.z - state.actorTarget.z};
    Vector3_ToFixed16EulerAngles(direction, state.actorDirection);
    if (std::fabs(state.actorHeightDelta) > 0.34999999f)
        state.flags |= 0x40000U;
    if ((state.flags & 0x40000U) && !(state.currentShotFlags & 8U) && state.actorHeightDelta < 0.0f)
        state.actorDirection.x /= 2;
}

// @verified - p
void RestoreConversationParticipantAlpha() {
    ConversationCameraState &state = conversationCamera;
    for (std::size_t index = 0; index < state.alphaParticipants.size(); ++index) {
        const shen_name name = state.alphaParticipants[index].name;
        if (name.name == UINT32_MAX)
            break;
        Character *const character = authFindChara(name);
        if (!character || !IsCharacterInCurrentMap(character))
            continue;
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (character != control->ryo)
            cpamSetFlag(character, 1);
        if (character == control->ryo || (!state.shotGenerationCount && (state.flags & 0x20U)))
            modelSetCharaAlpha(character, 1.0f);
    }
    state.alphaParticipants[0].name = shen_name{UINT32_MAX};
}

// @verified - p
void CaptureConversationParticipantAlpha() {
    ConversationCameraState &state = conversationCamera;
    if (!(state.flags & 4U))
        RestoreConversationParticipantAlpha();
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::int32_t index = 0; index < state.memberCount; ++index) {
        Character *const character = state.members[static_cast<std::size_t>(index)];
        ConversationParticipantAlpha &participant = state.alphaParticipants[static_cast<std::size_t>(index)];
        participant.name = scenGetCharaName(control->scenarioTask, character);
        participant.flags &= ~0x80U;
        if (shenCheckCharaHasAlphaManager(character))
            participant.flags |= 0x80U;
    }
    state.alphaParticipants[static_cast<std::size_t>(state.memberCount)].name = shen_name{UINT32_MAX};
}

// @verified - p
void UpdateConversationMirrorFlags() {
    ConversationCameraState &state = conversationCamera;
    if (state.flags & 0x20000000U)
        return;
    const ConversationShotHistory &previous = state.history[(state.historyCursor + state.history.size() - 1) % state.history.size()];
    if (previous.candidate >= 0 && (ConversationDescriptors[static_cast<std::size_t>(previous.candidate)].flags & 0x100U))
        state.side = !state.side;
    if (state.flags & 0x10000U) {
        const Vector3 root = GetCharacterWorldPosition(state.root), partner = GetCharacterWorldPosition(state.specialTarget);
        const Vector3 direction{partner.x - root.x, partner.y - root.y, partner.z - root.z};
        Vector3s angles;
        Vector3_ToFixed16EulerAngles(direction, angles);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-angles.y));
        Matrix4x4::preTranslate(-root.x, -root.y, -root.z);
        Vector3 localEye;
        Matrix4x4::TransformPoint(state.previousEye, localEye);
        Matrix4x4::Pop(1);
        state.side = localEye.x < 0.0f;
        if (std::fabs(localEye.x) < 0.001f)
            state.side = false;
    }
    state.mirrorFlags = state.eyeScenarioName.name == state.rootScenarioName.name ? state.side : !state.side;
}

// @verified - p
void BeginConversationShot() {
    ConversationCameraState &state = conversationCamera;
    const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
    ++state.shotGenerationCount;
    state.actorSmoothingFrames = 0;
    state.shotScratch = 0;
    state.shotType = state.shotTypes[shot];
    state.currentShotFlags = state.shotFlags[shot];
    if (state.flags & 0x2000U) {
        state.shotType = 0;
        state.currentShotFlags = 0x90U;
    }
    state.groundBias = 0.1f;
    state.groundOffset = 0.0f;
    state.groundPenetration = 0.0f;
    state.flags &= 0xf79cf7efU;
    BindConversationShotActors();
    RefreshConversationSceneFlags();
    if (state.flags & 0x400000U)
        state.flags |= 0x200000U;
    if (state.flags & 0x10U) {
        state.shotType = state.shotTypes[shot] = 0;
        ResetConversationCandidates();
        SelectConversationCandidate();
        return;
    }

    const ConversationShotHistory &previous = state.history[(state.historyCursor + state.history.size() - 1) % state.history.size()];
    if (previous.candidate >= 0 && (state.currentShotFlags & 2U) && state.eyeScenarioName.name == state.rootScenarioName.name &&
        previous.eyeName.name == state.rootScenarioName.name && previous.targetName.name != state.targetScenarioName.name && previous.shotType == 1)
        state.flags |= 0x10000U;
    UpdateConversationMirrorFlags();
    UpdateConversationActorPose();

    ResetConversationCandidates();
    FilterConversationCandidates();
    const std::int32_t candidate = SelectConversationCandidate();
    ConversationShotHistory &history = state.history[state.historyCursor];
    history.candidate = candidate;
    history.duplicateCandidate = static_cast<std::int16_t>(candidate);
    history.shotType = state.shotType;
    history.targetName = state.targetScenarioName;
    history.eyeName = state.eyeScenarioName;
    state.historyCursor = static_cast<std::uint8_t>((state.historyCursor + 1) % state.history.size());
}

// @verified - p
void AdvanceConversationShotClock() {
    ConversationCameraState &state = conversationCamera;
    state.flags &= ~0x40U;
    const std::uint32_t now = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    const float duration = state.shots[static_cast<std::size_t>(state.shotIndex)]->duration * 30.0f;
    const float elapsed = (static_cast<float>(static_cast<std::int32_t>(state.shotStartTime - now)) * 30.0f) / 12500000.0f;
    if (elapsed >= duration && state.shotIndex < state.shotCount - 1) {
        state.shotStartTime = now;
        camCtrlGetNextCamPos(&state.previousEye);
        camCtrlGetTargetPosition(&state.previousTarget);
        state.previousFieldOfView = camCtrlGetPerspective();
        ++state.shotIndex;
        state.entryBlendFrames = 0;
        state.actorDistance = 1.0f;
        state.actorHeightDelta = 0.0f;
        state.rootPartnerHeightDelta = 0.0f;
        if (!(state.flags & 0x4000U))
            state.flags &= ~0x8000U;
        state.shotFrame = 0.0f;
    }
    if (state.shotIndex == state.shotCount - 1 && elapsed >= duration)
        state.flags |= 0x40000000U;
    const float currentDuration = state.shots[static_cast<std::size_t>(state.shotIndex)]->duration * 30.0f;
    state.shotDurationFrames = static_cast<float>(static_cast<std::int16_t>(static_cast<std::int32_t>(currentDuration)));
    if (!(state.shotFrame >= std::numeric_limits<float>::epsilon()) || (state.flags & 0x2000U))
        state.flags |= 0x40U;
    if (state.flags & 0x40U)
        BeginConversationShot();
    state.flags &= ~0x2400U;
}




// @verified - p
void ApplyConversationLookTurn(Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    ConversationCameraState &state = conversationCamera;
    if (state.currentShotFlags & 8U)
        fieldOfView = camCtrlGetFov();
    if ((state.flags & 0x4000U) && !(state.flags & 0x8000U)) {
        if (!(state.currentShotFlags & 8U))
            return;
        state.flags |= 0x8000U;
    }
    if (!(state.currentShotFlags & 8U) || !(state.flags & 0x8000U) || ++state.lookDelayFrames < 10)
        return;
    float factor = std::fmax(0.0f, static_cast<float>(++state.lookTurnFrames) / 60.0f);
    if (factor >= 1.0f)
        factor = 1.0f;
    const std::int32_t angle = static_cast<std::int32_t>((factor * 180.0f * 8192.0f) / 45.0f);
    const float ease = ((1.0f - FixedAngleCos(static_cast<std::uint16_t>(angle))) * 1.0f) * 0.5f;
    Vector3s lookRotation;
    if ((state.descriptorFlags & 0x07000402U) == 0x402U) {
        const Vector3 direction{state.lookLocation.x - state.actorEye.x, state.lookLocation.y - state.actorEye.y, state.lookLocation.z - state.actorEye.z};
        Vector3_ToFixed16EulerAngles(direction, lookRotation);
        const std::int16_t from = static_cast<std::int16_t>(state.actorDirection.y + 0x8000);
        (void)CameraAngleLerpWithEpsilon(static_cast<std::int16_t>(-state.actorDirection.x), lookRotation.x, ease);
        const std::int16_t to = CameraAngleLerpWithEpsilon(from, lookRotation.y, ease);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(state.actorEye.x, state.actorEye.y, state.actorEye.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(to - from));
        Matrix4x4::preTranslate(-state.actorEye.x, -state.actorEye.y, -state.actorEye.z);
        Vector3 swung;
        Matrix4x4::TransformPoint(eye, swung);
        Matrix4x4::Pop(1);
        eye = {CameraLerpWithEpsilon(eye.x, swung.x, ease), CameraLerpWithEpsilon(eye.y, swung.y, ease), CameraLerpWithEpsilon(eye.z, swung.z, ease)};
    }
    const Vector3 view{target.x - eye.x, target.y - eye.y, target.z - eye.z};
    Vector3s rotation;
    Vector3_ToFixed16EulerAngles(view, rotation);
    const float length = Vector3::CalcMagnitude(view);
    const Vector3 direction{state.lookLocation.x - eye.x, state.lookLocation.y - eye.y, state.lookLocation.z - eye.z};
    Vector3_ToFixed16EulerAngles(direction, lookRotation);
    const std::int16_t pitch = CameraAngleLerpWithEpsilon(rotation.x, lookRotation.x, ease);
    const std::int16_t yaw = CameraAngleLerpWithEpsilon(rotation.y, lookRotation.y, ease);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(eye.x, eye.y, eye.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pitch));
    Matrix4x4::preTranslate(0.0f, 0.0f, -length);
    target = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
}

// @verified - p
void BuildConversationOverrideCamera(Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    const ConversationCameraState &state = conversationCamera;
    target = {
        (state.actorTarget.x + state.actorEye.x) * 0.5f, (state.actorTarget.y + state.actorEye.y) * 0.5f, (state.actorTarget.z + state.actorEye.z) * 0.5f};
    eye = state.positionOverride;
    fieldOfView = camCtrlGetFov();
}

// @verified - p
void BlendConversationEntryCamera(Vector3 &eye, Vector3 &target, std::uint16_t &fieldOfView) {
    ConversationCameraState &state = conversationCamera;
    if (!(state.flags & 0x20000U))
        return;
    const float amount = static_cast<float>(++state.entryBlendFrames) / 30.0f;
    if (amount > 1.0f)
        return;
    const auto blend = [amount](float first, float last) {
        const float delta = last - first;
        return std::abs(amount) < std::numeric_limits<float>::epsilon() || std::abs(delta) < std::numeric_limits<float>::epsilon() ? first :
                                                                                                                                     first + delta * amount;
    };
    eye = {blend(state.previousEye.x, eye.x), blend(state.previousEye.y, eye.y), blend(state.previousEye.z, eye.z)};
    target = {blend(state.previousTarget.x, target.x), blend(state.previousTarget.y, target.y), blend(state.previousTarget.z, target.z)};
    fieldOfView = static_cast<std::uint16_t>(CameraTransitionAngle(
        static_cast<std::int16_t>(fieldOfView), static_cast<std::int16_t>(state.previousFieldOfView), amount, CameraInterpolationMode::PreviousToCurrent));
}

// @verified - p
void UpdateConversationMemberAlpha() {
    const ConversationCameraState &state = conversationCamera;
    if (!(state.flags & 0x200U))
        return;
    if ((state.sceneFlags & 0x10U) && state.shotType == 0) {
        const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
        Vector3 eye = GetCharacterWorldPosition(state.eyeAnchors[shot]);
        Vector3 target = GetCharacterWorldPosition(state.targetAnchors[shot]);
        const Vector3 direction{state.target.x - state.eye.x, state.target.y - state.eye.y, state.target.z - state.eye.z};
        Vector3s rotation;
        Vector3_ToFixed16EulerAngles(direction, rotation);
        Matrix4x4_PushActiveTransform();
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-rotation.x));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-rotation.y));
        Matrix4x4::preTranslate(-state.eye.x, -state.eye.y, -state.eye.z);
        Matrix4x4::TransformPoint(eye, eye);
        Matrix4x4::TransformPoint(target, target);
        Matrix4x4::Pop(1);
    }
    for (std::int32_t index = 0; index < state.memberCount; ++index) {
        Character *const character = state.members[static_cast<std::size_t>(index)];
        if (character == state.root || !(state.alphaParticipants[static_cast<std::size_t>(index)].flags & 0x80U))
            modelSetCharaAlpha(character, 1.0f);
        else if (!shenCheckCharaHasAlphaManager(character))
            cpamSetFlag(character, 1);
        if (character != state.root && (state.flags & 0x1000U))
            modelSetCharaAlpha(character, 0.0f);
    }
}

void TickConversationActivation(CameraRuntime &);

// @verified - p
void EvaluateConversationShot() {
    ConversationCameraState &state = conversationCamera;
    if (state.shotFrame < state.shotDurationFrames) {
        state.shotFrame += shenGetScaledDeltaTickTime(1.0f);
        if (state.shotFrame > state.shotDurationFrames)
            state.shotFrame = state.shotDurationFrames;
    }
    float interpolationProgress = state.shotFrame / std::fmax(180.0f, state.shotDurationFrames);
    float generationProgress = std::fmax(0.0f, interpolationProgress);
    if (generationProgress >= 1.0f)
        generationProgress = 1.0f;
    if (state.flags & 0x10000U)
        interpolationProgress = generationProgress = SmoothConversationProgress(interpolationProgress);
    if (std::fabs(generationProgress) < std::numeric_limits<float>::epsilon())
        generationProgress = 0.0f;
    float progress = generationProgress;
    if (state.shotType == 1 && state.candidateIndex >= 0) {
        constexpr std::array<float, 22> collisionWeights{
            0.0f, 1.0f, 0.0f, 0.25f, 0.25f, 0.25f,
            0.5f, 0.25f, 0.75f, 0.25f, 0.0f, 0.75f,
            0.25f, 0.75f, 0.0f, 0.5f, 0.25f, 0.5f,
            0.5f, 0.5f, 0.0f, 1.0f};
        const std::uint8_t collisionClass = state.candidates[static_cast<std::size_t>(state.candidateIndex)].collisionClass;
        const std::size_t weight = std::min<std::size_t>(collisionClass, 10) * 2;
        progress = collisionWeights[weight] + progress * collisionWeights[weight + 1];
    }
    state.shotProgress = progress;
    UpdateConversationActorPose();
    Vector3 eye = state.eye, target = state.target;
    std::uint16_t fieldOfView = state.fieldOfView;
    state.descriptorFlags = 0;
    if (state.flags & 0x08000000U)
        BuildConversationOverrideCamera(eye, target, fieldOfView);
    else {
        if (state.candidateIndex >= 0)
            state.descriptorFlags = ConversationDescriptors[static_cast<std::size_t>(state.candidateIndex)].flags;
        GenerateConversationCandidate(state.candidateIndex, progress, eye, target, fieldOfView);
    }
    if ((state.flags & 0x10000U) && interpolationProgress <= 1.0f) {
        const float targetProgress = std::fmin(1.0f, std::fmax(0.0f, interpolationProgress * 1.2f));
        eye = {CameraLerpWithEpsilon(state.previousEye.x, eye.x, interpolationProgress),
            CameraLerpWithEpsilon(state.previousEye.y, eye.y, interpolationProgress), CameraLerpWithEpsilon(state.previousEye.z, eye.z, interpolationProgress)};
        target = {CameraLerpWithEpsilon(state.previousTarget.x, target.x, targetProgress),
            CameraLerpWithEpsilon(state.previousTarget.y, target.y, targetProgress), CameraLerpWithEpsilon(state.previousTarget.z, target.z, targetProgress)};
        fieldOfView = static_cast<std::uint16_t>(
            CameraAngleLerpWithEpsilon(static_cast<std::int16_t>(state.previousFieldOfView), static_cast<std::int16_t>(fieldOfView), interpolationProgress));
    }
    ApplyConversationLookTurn(eye, target, fieldOfView);
    ApplyConversationSpecialModeOverride(eye, target, fieldOfView);
    BlendConversationEntryCamera(eye, target, fieldOfView);
    CorrectConversationGround(eye, target);
    state.eye = eye;
    state.target = target;
    state.fieldOfView = static_cast<std::int16_t>(fieldOfView) >= 12743 ? 12743U : fieldOfView;
}

// @verified - p
void UpdateConversationCamera(CameraRuntime &camera) {
    ConversationCameraState &state = conversationCamera;
    state.flags &= ~0x100U;
    if (!(state.flags & 2U) || state.shotCount <= 0)
        return;
    state.root = CameraRootCharacter();
    ++state.viewUpdateCount;
    state.framePreviousEye = state.eye;
    state.framePreviousTarget = state.target;
    state.framePreviousFieldOfView = state.fieldOfView;
    TickConversationActivation(camera);
    state.sceneFlags &= ~0x01000400U;
    if (QueryNpcCameraMotionProfile(state.specialTargetName, 8))
        state.sceneFlags |= 0x400U;
    if ((state.currentShotFlags & 8U) && (state.sceneFlags & 0x400U))
        state.sceneFlags |= 0x01000000U;
    AdvanceConversationShotClock();
    EvaluateConversationShot();
    if (state.flags & 0x01000000U)
        return;
    camCtrlSetNextPosition(state.eye);
    camCtrlSetTargetPosition(state.target);
    camCtrlSetPerspective(state.fieldOfView);
    UpdateConversationMemberAlpha();
}

// @verified - p
void TickConversationActivation(CameraRuntime &) {
    using enum CameraState;
    ConversationCameraState &state = conversationCamera;
    if (state.activationFrames <= 0)
        return;
    state.flags |= 0x01000000U;
    if (--state.activationFrames == 0) {
        state.flags &= ~0x01000000U;
        camCtrlSetState(Conversation);
        camCtrlGetNextCamPos(&state.previousEye);
        camCtrlGetTargetPosition(&state.previousTarget);
        state.previousFieldOfView = camCtrlGetPerspective();
    }
}

struct CameraContextSetting {
    float distanceFromPlayer;
    std::int16_t basePitchBias;
};

// g_cached_context_settings: eleven 8-byte rows whose
// int32 pitch field is consumed as a WORD. The first-person row is the adjacent
// g_tmp_dist_from_player/g_tmp_base_pitch_bias pair that index 11 reads.
constexpr std::array<CameraContextSetting, 11> cameraContextSettings{{
    {0.66000003f, 2048}, {0.76800001f, 1588}, {0.87599999f, 1128}, {0.98400003f, 668}, {1.092f, 208}, {1.2f, -256},
    {1.388f, -601}, {1.576f, -946}, {1.7640001f, -1291}, {1.952f, -1636}, {2.1400001f, -1984},
}};
constexpr CameraContextSetting firstPersonContextSetting{1.67f, -2048};

constexpr std::uint32_t CameraStateChanged = 0x00000008U;
constexpr std::uint32_t CameraSplineRebuilt = 0x00000080U;
constexpr std::uint32_t CameraAllowContextSwitch = 0x00200000U;
constexpr std::uint32_t CameraAutoFovActive = 0x00800000U;

void camCtrlSetFloats(CameraRuntime &camera, float position, float angle) {
    if (position == 0.0f && angle == 0.0f) {
        position = 0.23f;
        angle = 0.30000001f;
    }
    camera.field214 = position;
    camera.field218 = position;
    camera.field21c = position;
    camera.field220 = angle;
    camera.field224 = angle;
    camera.field228 = angle;
}


Vector3 plymComputeConvTargetAnchor() {
    const Vector3 target = GetPlayerTargetWorldPosition();
    const float speed = std::fmax(1.0f, plymGetSpeedZ());
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(target.x, target.y, target.z);
    Matrix4x4::RotateAroundY_Wrapper(plymGetAngY());
    Matrix4x4::preTranslate(0.0f, 0.0f, -((speed - 1.0f) * 0.40000001f));
    const Vector3 anchor{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return anchor;
}

void camCtrlInterpolateApplyCameraAngY(CameraRuntime &camera) {
    const float orbitDistance = camera.orbitDistance;
    if (camera.flags & (CameraSplineRebuilt | CameraDisableFovInterpolation)) {
        camera.interpolationTicks = 8;
    } else if (orbitDistance <= camera.targetOrbitDistance) {
        if (static_cast<std::uint8_t>(camera.interpolationTicks - 1) > 6U) {
            camera.interpolationTicks = 8;
        } else {
            camera.orbitDistance = CameraLerp(orbitDistance, camera.targetOrbitDistance, camCtrlGetDeltaTime(0.1f));
            if (std::fabs(orbitDistance - camera.orbitDistance) < 0.001f)
                camera.orbitDistance = orbitDistance;
            if (++camera.interpolationTicks < 5)
                camera.interpolationTicks = 5;
        }
        if (++camera.interpolationTicks >= 8)
            camera.interpolationTicks = 8;
    } else {
        const float rate = camera.interpolationTicks <= 0 ? 0.1f : 0.0049999999f;
        camera.orbitDistance = CameraLerp(orbitDistance, camera.targetOrbitDistance, camCtrlGetDeltaTime(rate));
        if (std::fabs(orbitDistance - camera.orbitDistance) < 0.001f)
            camera.orbitDistance = orbitDistance;
        if (--camera.interpolationTicks < 0)
            camera.interpolationTicks = 0;
    }
}

void camCtrlUpdateFollowCamera(CameraRuntime &camera) {
    using enum CameraState;
    if (camera.flags & CameraAutoFovActive) {
        if (camera.previousState != Conversation) {
            camera.flags &= ~CameraAutoFovActive;
        } else if (!camCtrlIsTransitionActive()) {
            camera.flags &= ~CameraAutoFovActive;
        } else if (!plymIsLookAllowed(1)) {
            camCtrlSetInterpEaseFactor(-1.0f);
            camera.flags &= ~CameraAutoFovActive;
        }
    }
    if (camera.previousState == Conversation && camCtrlIsTransitionActive() && !plymIsLookAllowed(1))
        camCtrlSetInterpEaseFactor(-1.0f);
    camCtrlSetFloats(camera, 0.23f, 0.30000001f);
    camera.currentFovAngle = (camera.flags & CameraDisableFovInterpolation) ?
        camera.fovAngle :
        static_cast<std::uint16_t>(CameraLerpAngle(static_cast<std::int16_t>(camera.fovAngle), camera.targetFovAngle, camCtrlGetDeltaTime(0.5f)));
    if (!CameraRootCharacter())
        return;

    const std::int16_t yaw = static_cast<std::int16_t>(plymGetAngY());
    const Vector3 anchor = plymComputeConvTargetAnchor();
    std::int16_t basePitchBias = camera.basePitchBias;
    camera.targetPosition = {anchor.x, anchor.y + camera.targetHeightDistance, anchor.z};
    Vector3s ground{};
    camCtrlQueryPitchYaw(ground);
    if (Abs16(ground.x) <= 546U) {
        if (camera.ticksRemaining > 0)
            --camera.ticksRemaining;
        if (camera.ticksRemaining <= 0)
            camera.nextRotation.pitch = CameraLerpAngle(basePitchBias, camera.nextRotation.pitch, camCtrlGetDeltaTime(0.1f));
    } else {
        Vector3 backward{};
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(ground.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(ground.x));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw - ground.y));
        TransformBackwardDirection(backward);
        Matrix4x4::Pop(1);
        if (std::fabs(backward.x) >= 0.00000011920929f || std::fabs(backward.y) >= 0.00000011920929f || std::fabs(backward.z) >= 0.00000011920929f)
            ground.x = static_cast<std::int16_t>(FixedAtan2(backward.y, std::sqrt(backward.x * backward.x + backward.z * backward.z)));
        basePitchBias = static_cast<std::int16_t>(basePitchBias - ground.x);
        camera.nextRotation.pitch = CameraLerpAngle(basePitchBias, camera.nextRotation.pitch, camCtrlGetDeltaTime(0.079999998f));
        camera.ticksRemaining = 10;
    }
    camera.nextRotation.yaw = CameraLerpAngle(yaw, camera.nextRotation.yaw, camCtrlGetDeltaTime(0.30000001f));
    camera.nextRotation.roll = 0;
    if (camera.flags & CameraDisableFovInterpolation) {
        camera.nextRotation.yaw = yaw;
        camera.nextRotation.pitch = basePitchBias;
    }
    camera.orbitDistance = camera.defaultOrbitDistance;
    if (camera.nextRotation.pitch < 0) {
        const float heightLimit =
            std::fmax(0.2f, (2.6500001f - (camera.targetPosition.y - anchor.y)) / sin_fixed16(static_cast<std::int16_t>(-camera.nextRotation.pitch)));
        if (camera.orbitDistance > heightLimit)
            camera.orbitDistance = heightLimit;
    }
    if (camera.contextMode == 10)
        camera.orbitDistance = 0.0099999998f;
    camera.flags |= CameraInterpolationSnap | CameraRaycastEnabled;
    camera.computeMode = CameraComputeMode::OrbitTarget;
    camCtrlGetIdealCameraPosition(camera);
    if (!(camera.flags & CameraStateChanged))
        camCtrlInterpolateApplyCameraAngY(camera);
    if (camera.orbitDistance < 3.0999999f && camera.contextMode != 10)
        camera.targetPosition.y += std::fmin((3.0999999f - camera.orbitDistance) / 3.0999999f * 0.80000001f, 0.80000001f);
}

void camCtrlAutoCenterTargetInView(CameraRuntime &camera) {
    using enum CameraState;
    if (!CameraRootCharacter())
        return;
    if (camera.flags & CameraAutoFovActive) {
        if (camera.previousState != Conversation) {
            camera.flags &= ~CameraAutoFovActive;
        } else if (!camCtrlIsTransitionActive()) {
            camera.flags &= ~CameraAutoFovActive;
        } else if (!plymIsLookAllowed(1)) {
            camCtrlSetInterpEaseFactor(-1.0f);
            camera.flags &= ~CameraAutoFovActive;
        }
    }
    if (camera.previousState == Conversation && camCtrlIsTransitionActive() && !plymIsLookAllowed(1))
        camCtrlSetInterpEaseFactor(-1.0f);
    if (Abs16(static_cast<std::int32_t>(plymGetAngY()) - camera.nextRotation.yaw) >= 16384U) {
        camera.collisionPhase = 2;
        camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
        return;
    }
    camera.currentFovAngle = (camera.flags & CameraDisableFovInterpolation) ?
        camera.fovAngle :
        static_cast<std::uint16_t>(CameraLerpAngle(static_cast<std::int16_t>(camera.fovAngle), camera.targetFovAngle, camCtrlGetDeltaTime(0.5f)));
    Vector3 view = plymComputeConvTargetAnchor();
    view.y += 1.6f;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&camera.projectionMatrix);
    Matrix4x4::TransformPoint(view, view);
    Matrix4x4::Pop(1);
    if (view.z * view.z + view.x * view.x < 0.25f || std::fabs(view.z) < 0.2f)
        return;
    if (view.z > 0.0f) {
        camera.collisionPhase = 2;
        camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
        return;
    }
    const auto yawExcess = static_cast<std::int16_t>(Abs16(AtanToFixedAngle(view.x / -view.z)) - static_cast<std::int16_t>(camera.currentFovAngle) / 4);
    if (yawExcess > 0)
        camera.nextRotation.yaw = static_cast<std::int16_t>(view.x >= 0.0f ? camera.nextRotation.yaw - yawExcess : camera.nextRotation.yaw + yawExcess);
    const auto pitchExcess = static_cast<std::int16_t>(Abs16(AtanToFixedAngle(view.y / -view.z)) - camera.pitch / 4);
    if (pitchExcess > 0)
        camera.nextRotation.pitch =
            static_cast<std::int16_t>(view.y >= 0.0f ? camera.nextRotation.pitch + pitchExcess : camera.nextRotation.pitch - pitchExcess);
    camera.flags |= CameraInterpolationSnap | CameraRaycastEnabled;
    camera.computeMode = CameraComputeMode::UsePreviousPosition;
    const Vector3 anchor = plymComputeConvTargetAnchor();
    camera.targetPosition = {anchor.x, anchor.y + camera.targetHeightDistance, anchor.z};
}




void camCtrlSwitchColiPhase(CameraRuntime &camera) {
    if (plymIntersectionTest()) {
        if (!camera.collisionPhase)
            camera.collisionPhase = 1;
    } else if (camera.collisionPhase) {
        camera.collisionPhase = 0;
    }
    if (camera.collisionPhase == 1) {
        camCtrlAutoCenterTargetInView(camera);
    } else {
        if (camera.flags & CameraAllowContextSwitch) {
            const LegacyInputSlot *const input = CameraInput();
            const std::uint16_t triggerMask = AreS2TriggersSwapped() ? 0x40U : 0x80U;
            if (input && (input->pressedPriority & triggerMask) != 0) {
                const std::int8_t contextMode = camera.contextMode;
                const CameraContextSetting &setting = contextMode == 10 ? cameraContextSettings[5] : firstPersonContextSetting;
                camera.targetHeightDistance = setting.distanceFromPlayer;
                camera.basePitchBias = setting.basePitchBias;
                camera.flags |= CameraStateChanged;
                camera.contextMode = contextMode == 10 ? 0 : 10;
                camCtrlResetTransition(CameraInterpolationMode::PreviousToCurrent);
                camera.transitionStepCount = 30;
            }
        }
        camCtrlUpdateFollowCamera(camera);
    }
    const float current = ctrlGetLookOverlayWeight();
    float weight = 0.0f;
    if (camera.contextMode == 10) {
        if (!(camera.flags & CameraAllowContextSwitch)) {
            ctrlSetLookOverlayWeight(0.0f);
            return;
        }
        weight = CameraLerp(0.0f, current, camCtrlGetDeltaTime(0.1f));
        if (weight < 0.001f) {
            ctrlSetLookOverlayWeight(0.0f);
            return;
        }
    } else {
        if (current >= 1.0f)
            return;
        weight = current + (1.0f - current) * 0.090000004f;
        if (weight > 0.98000002f)
            weight = 1.0f;
    }
    ctrlSetLookOverlayWeight(weight);
}



void camCtrlApplyJitterAdjustment(CameraRuntime &camera) {
    if (std::fabs(camera.shakeAmplitude.x) < 0.003f)
        camera.shakeAmplitude.x = 0.0f;
    if (std::fabs(camera.shakeAmplitude.y) < 0.003f)
        camera.shakeAmplitude.y = 0.0f;
    camera.jitter.x = 0.0f;
    camera.jitter.y = 0.0f;
    if (camera.shakeAmplitude.x == 0.0f && camera.shakeAmplitude.y == 0.0f)
        return;
    camera.jitter.x = ((0.5f - IncrementSharedPrngFloat()) * 0.5f + 1.0f) * camera.shakeAmplitude.x;
    const float random = IncrementSharedPrngFloat();
    const float decayedX = camera.shakeAmplitude.z * camera.shakeAmplitude.x;
    const float decayedY = camera.shakeAmplitude.z * camera.shakeAmplitude.y;
    camera.jitter.y = camera.shakeAmplitude.y * ((0.5f - random) * 0.5f + 1.0f);
    camera.shakeAmplitude.x = decayedX;
    camera.shakeAmplitude.y = -decayedY;
    if (camera.shakeAmplitude.y != 0.0f || (IncrementSharedPrngUInt16() & 3U) != 0)
        camera.shakeAmplitude.x = -camera.shakeAmplitude.x;
}




void camCtrlHandleLookAroundCam(CameraRuntime &camera) {
    using enum CameraState;
    float zoomOrbit = 0.0f;
    bool transitionToTarget = false;
    if (!CameraRootCharacter())
        return;
    camera.flags |= CameraInterpolationSnap | CameraRaycastEnabled;
    camera.computeMode = CameraComputeMode::OrbitTarget;
    if (plymIntersectionTest()) {
        if (camera.collisionPhase != 2) {
            camera.lookPitchDelta = 0;
            camera.lookYawDelta = 0;
            camCtrlSetState(Init);
            camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
            return;
        }
    } else if (camera.collisionPhase) {
        camera.collisionPhase = 0;
    }
    Vector2s base{};
    Vector2s look{};
    plymSampleLookInput(base, look);
    const std::int32_t inputState = plymGetLookInputState();
    if (inputState == -1) {
        camera.lookPitchDelta = 0;
        camera.lookYawDelta = 0;
        camCtrlSetState(Init);
        return;
    }
    if (inputState <= 0) {
        camera.lookPitchDelta = CameraLerpAngle(0, camera.lookPitchDelta, shenGetScaledDeltaTickTime(0.079999998f));
        camera.lookYawDelta = CameraLerpAngle(0, camera.lookYawDelta, shenGetScaledDeltaTickTime(0.079999998f));
        camera.pitchBase = camera.basePitchBias;
        const auto yaw = static_cast<std::int16_t>(plymGetAngY());
        const std::int16_t pitchDelta = camera.lookPitchDelta;
        const std::int16_t yawDelta = camera.lookYawDelta;
        camera.nextRotation.pitch = static_cast<std::int16_t>(camera.pitchBase + pitchDelta);
        camera.yawBase = yaw;
        camera.nextRotation.yaw = static_cast<std::int16_t>(yaw + yawDelta);
        if ((std::abs(pitchDelta) < 64 && std::abs(yawDelta) < 64) || !plymIsLookAllowed(1)) {
            camera.lookPitchDelta = 0;
            camera.lookYawDelta = 0;
            camCtrlSetState(Init);
            transitionToTarget = true;
        }
    } else {
        camera.nextRotation.pitch = static_cast<std::int16_t>(look.x + base.x + camera.basePitchBias);
        camera.pitchBase = base.x;
        camera.yawBase = base.y;
        camera.nextRotation.yaw = static_cast<std::int16_t>(look.y + base.y);
        camera.lookPitchDelta = look.x;
        camera.lookYawDelta = look.y;
    }
    float pitchRatio = std::fmax(-1.0f, static_cast<float>(camera.nextRotation.pitch) / 10922.0f);
    if (pitchRatio >= 1.0f)
        pitchRatio = 1.0f;
    const float zoom = static_cast<float>(plymGetLookZoomScale()) * 0.00390625f;
    const Vector3 anchor = plymComputeConvTargetAnchor();
    const bool firstPerson = camera.contextMode == 10;
    float heightLift = 0.0f;
    float zoomLift = 0.0f;
    float y = anchor.y + camera.targetHeightDistance;
    Vector3 target{anchor.x, y, anchor.z};
    if (!firstPerson) {
        zoomLift = zoom * 0.40000001f;
        if (camera.nextRotation.pitch < 0) {
            const float pitchWeight = std::fabs(pitchRatio);
            zoomLift = (1.0f - pitchWeight) * zoomLift + pitchWeight * 0.69999999f;
        }
        target.y = y + zoomLift;
        camera.targetPosition = target;
        camera.orbitDistance = 3.0999999f;
        camCtrlGetIdealCameraPosition(camera);
        camCtrlInterpolateApplyCameraAngY(camera);
        if (camera.orbitDistance >= 3.0999999f) {
            y = target.y;
        } else {
            heightLift = std::fmin((3.0999999f - camera.orbitDistance) / 3.0999999f * 0.80000001f, 0.80000001f) * (1.0f - zoom);
            if (camera.nextRotation.pitch < 0)
                heightLift *= 1.0f - std::fabs(pitchRatio);
            y = target.y + heightLift;
            target.y += heightLift;
        }
    }
    float distance = firstPerson ? 0.0099999998f : camera.orbitDistance;
    Vector3 eyeTarget{target.x, y, target.z};
    if (zoom > 0.0f) {
        zoomOrbit = 2.5f;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(target.x, target.y, target.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.yaw));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.pitch));
        Matrix4x4::preTranslate(0.0f, 0.0f, -2.5f);
        eyeTarget = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        y = eyeTarget.y;
    }
    const std::int16_t pitch = camera.nextRotation.pitch;
    if (pitch > 0) {
        const float floor = anchor.y + 0.55000001f;
        const float sine = sin_fixed16(static_cast<std::int16_t>(-pitch));
        if (floor > sine * distance + target.y)
            distance = std::fmin((floor - target.y) / sine, distance);
        y = eyeTarget.y;
    } else if (pitch < 0) {
        distance = std::fmin((1.0f - std::fabs(pitchRatio)) * 2.99f + 0.11f, distance);
    }
    camera.orbitDistance = (1.0f - zoom) * (distance + zoomOrbit);
    if (zoom > 0.0f)
        camera.flags &= ~CameraRaycastEnabled;
    if (camera.flags & CameraAutomaticDistanceFov) {
        const auto perspective = static_cast<std::int16_t>(camera.currentFovAngle);
        const std::uint16_t fov = camera.fovAngle;
        std::uint16_t fovAngle = fov;
        const float orbit = camera.orbitDistance;
        const float minimum = zoomOrbit + 0.0099999998f;
        if (minimum > orbit) {
            camera.orbitDistance = minimum;
            fovAngle = static_cast<std::uint16_t>(fov - static_cast<std::int32_t>((minimum - orbit) / minimum * (static_cast<float>(fov) - 5461.0f)));
        }
        const float time = std::fabs(zoom) < 0.00000011920929f ? 0.25f : 0.5f;
        const std::int16_t lerped = CameraLerpAngle(static_cast<std::int16_t>(fovAngle), perspective, camCtrlGetDeltaTime(time));
        if (lerped)
            camera.currentFovAngle = static_cast<std::uint16_t>(lerped);
        y = eyeTarget.y;
    }
    float blend = std::fmax(0.0f, zoom / (distance / (distance + zoomOrbit)));
    if (blend >= 1.0f)
        blend = 1.0f;
    const std::int16_t finalPitch = camera.nextRotation.pitch;
    float lift = 0.0f;
    float zoomBlendLift = blend * 0.40000001f;
    const float baseY = y - (heightLift + zoomLift);
    if (finalPitch < 0) {
        const float pitchWeight = std::fabs(pitchRatio);
        zoomBlendLift = (1.0f - pitchWeight) * zoomBlendLift + pitchWeight * 0.69999999f;
    }
    if (distance < 3.0999999f) {
        lift = std::fmin((3.0999999f - distance) / 3.0999999f * 0.80000001f, 0.80000001f) * (1.0f - blend);
        if (finalPitch < 0)
            lift *= 1.0f - std::fabs(pitchRatio);
    }
    if (firstPerson) {
        lift = 0.0f;
        zoomBlendLift = 0.0f;
    }
    camera.targetPosition = {eyeTarget.x, baseY + (lift + zoomBlendLift), eyeTarget.z};
    const float current = ctrlGetLookOverlayWeight();
    float maximum = 1.0f;
    if (zoom > 0.0f) {
        maximum = std::fmax(0.0f, (camera.orbitDistance - zoomOrbit) / 3.0999999f);
        if (maximum >= 1.0f)
            maximum = 1.0f;
    }
    float weight{};
    if (current <= maximum) {
        weight = CameraLerp(maximum, current, camCtrlGetDeltaTime(0.1f));
        if (weight > 0.99900001f)
            weight = 1.0f;
    } else {
        weight = CameraLerp(maximum, current, camCtrlGetDeltaTime(0.40000001f));
        if (weight < 0.001f)
            weight = 0.0f;
    }
    if (firstPerson)
        weight = 0.0f;
    ctrlSetLookOverlayWeight(weight);
    if (transitionToTarget) {
        camCtrlResetTransition(CameraInterpolationMode::ToTargetContinuous);
        camCtrlSetInterpEaseFactor(0.15000001f);
    }
}

CameraSplineState cameraSplines{};

constexpr std::uint32_t CameraSkipInterpolationOnce = 0x00080000U;
constexpr std::uint32_t SplineRecompute = 0x2U;
constexpr std::uint32_t SplineWorldSpace = 0x4U;
constexpr std::uint32_t SplineChanged = 0x8U;
constexpr std::uint32_t SplineCheckEyeCollision = 0x10U;
constexpr std::uint32_t SplineDriveRoll = 0x20U;
constexpr std::uint32_t SplineDriveFov = 0x40U;
constexpr std::uint32_t SplineAutoAdvance = 0x08000000U;




std::int32_t buildHermiteCoefficients(float *out, std::int32_t count, const float *times, const float *values, const float *tangents) {
    std::int32_t used = count;
    for (std::int32_t index = 0; index < count - 1; ++index) {
        if (times[index] == times[index + 1]) {
            used = index + 1;
            break;
        }
    }
    if (used < 2)
        return used == 1;
    auto *const h = static_cast<float *>(HLib::AllocateTaskData(28ULL * static_cast<std::uint64_t>(used), SHEN_NAME(SPLI)));
    float *const diagonal = h + used;
    float *const reduced = diagonal + used;
    float *const lower = reduced + used;
    float *const dy = lower + used;
    float *const rhs = dy + used;
    float *const slope = rhs + used;
    const std::int32_t inner = used - 2;
    for (std::int32_t index = 0; index < inner; ++index) {
        h[index] = times[index + 1] - times[index];
        const float span = times[index + 2] - times[index];
        diagonal[index] = span + span;
        dy[index] = values[index + 1] - values[index];
    }
    h[inner] = times[inner + 1] - times[inner];
    dy[inner] = values[inner + 1] - values[inner];
    slope[0] = tangents ? tangents[0] : 0.0f;
    slope[used - 1] = tangents ? tangents[used - 1] : 0.0f;
    if (used >= 3) {
        reduced[0] = diagonal[0];
        for (std::int32_t index = 1; index < inner; ++index) {
            lower[index] = h[index + 1] / reduced[index - 1];
            reduced[index] = diagonal[index] - lower[index] * h[index - 1];
        }
        rhs[0] = (h[0] * dy[1] / h[1] + h[1] * dy[0] / h[0]) * 3.0f;
        for (std::int32_t index = 1; index < inner; ++index)
            rhs[index] = (h[index + 1] * dy[index] / h[index] + h[index] * dy[index + 1] / h[index + 1]) * 3.0f - lower[index] * rhs[index - 1];
        slope[used - 2] = rhs[used - 3] / reduced[used - 3];
        for (std::int32_t index = used - 4; index >= 0; --index)
            slope[index + 1] = (rhs[index] - slope[index + 2] * h[index]) / reduced[index];
    }
    for (std::int32_t index = 1; index < used; ++index) {
        out[2 * index] = slope[index - 1] * h[index - 1] + values[index - 1] * 3.0f;
        out[2 * index + 1] = values[index] * 3.0f - slope[index] * h[index - 1];
    }
    out[0] = 0.0f;
    HLib::FreeTaskData(h);
    return used;
}

} // namespace

// @verified - p
std::uint32_t AllocSpline(CameraSplineTrack &track) {
    const float *const tangents = track.tangents ? &track.tangents[track.firstKey] : nullptr;
    return static_cast<std::uint32_t>(buildHermiteCoefficients(
        track.coefficients, static_cast<std::int32_t>(track.count) - track.firstKey, &track.times[track.firstKey], &track.values[track.firstKey], tangents));
}

namespace {
float SplineFrameTime(float time) {
    return static_cast<float>(static_cast<std::int32_t>(time * 60.0f + 0.0083333338f)) / 60.0f;
}



std::int32_t CubicHermiteEvaluate(float *out, float time, std::int32_t count, const float *times, const float *values, const float *coefficients) {
    const float now = SplineFrameTime(time);
    if (SplineFrameTime(times[0]) > now || count < 2) {
        *out = values[0];
        return -1;
    }
    std::int32_t key = 1;
    while (key < count && SplineFrameTime(times[key]) <= now)
        ++key;
    if (key == count) {
        *out = values[count - 1];
        return 0;
    }
    const std::int32_t previous = key - 1;
    const float next = times[key] - time;
    const float elapsed = time - times[previous];
    const float span = times[key] - times[previous];
    *out = (next * next * next * values[previous] + elapsed * (next * next) * coefficients[2 * key] + elapsed * elapsed * next * coefficients[2 * key + 1] +
               elapsed * elapsed * elapsed * values[key]) /
        (span * span * span);
    return key;
}

} // namespace

std::int32_t DoKeyframeCubicHermiteInterpolation(CameraSplineTrack &track, float time, float *out) {
    std::int32_t result = CubicHermiteEvaluate(
        out, time, static_cast<std::int32_t>(track.segmentCount), &track.times[track.firstKey], &track.values[track.firstKey], track.coefficients);
    if (result)
        return result;
    while (track.segmentCount + static_cast<std::uint32_t>(track.firstKey) < track.count) {
        track.firstKey += static_cast<std::int32_t>(track.segmentCount);
        track.segmentCount = AllocSpline(track);
        result = CubicHermiteEvaluate(
            out, time, static_cast<std::int32_t>(track.segmentCount), &track.times[track.firstKey], &track.values[track.firstKey], track.coefficients);
        if (result)
            return result;
    }
    return 0;
}

namespace {
// 52e230: authored attachment modes borrow the real entity or solved rig bone.
void GetPosRotUtil(
    Vector3 &position, Vector3i &rotation, std::uint8_t mode, const void *node, const Vector3 &fallbackPosition, const Vector3i &fallbackRotation) {
    if (!(mode & 1))
        return;
    if (!node) {
        position = fallbackPosition;
        if (mode & 2)
            rotation = {-fallbackRotation.x, -fallbackRotation.y, -fallbackRotation.z};
        return;
    }
    auto *const character = const_cast<Character *>(static_cast<const Character *>(node));
    const auto bone = static_cast<std::uint16_t>((mode & 4) ? 5 : 0);
    if (!(mode & 4) && (mode & 0x20))
        position = GetCharacterWorldPosition(character);
    else
        GetCharacterMotionWorldPosition(character, bone, &position);
    if (!(mode & 2))
        return;
    if (auto *const motion = FindMotionWork(character)) {
        rotation.x = rotation.z = 0;
        rotation.y = !(mode & 4) && (mode & 0x20) ? motion->runtime.entityRotation.y : GetMotionBoneYaw(motion, bone);
    } else
        rotation = GetCharacterWorldRotation(character);
}



void spliEvaluate(Vector3 &value, std::uint8_t mode, const Vector3 &position, const Vector3i &rotation) {
    if (mode & 1U) {
        Matrix4x4_Push(&transformMatrix_prePass);
        if (mode & 2U) {
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-rotation.z));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-rotation.y));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-rotation.x));
        }
        Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
        Matrix4x4::TransformPoint(value, value);
        Matrix4x4::Pop(1);
    }
    if (mode & 0x10U) {
        const float radius = std::sqrt(value.x * value.x + value.z * value.z);
        const std::uint16_t angle = FixedAtan2(value.z, value.x);
        value.x = radius;
        value.z = value.y;
        value.y = static_cast<float>(static_cast<std::int16_t>(-16384 - angle)) / 182.04445f;
    }
}


void spliEvaluate2(Vector3 &value, std::uint8_t mode, const void *node, const Vector3 &fallbackPosition, const Vector3i &fallbackRotation,
    const Vector3 &basePosition, const Vector3i &baseRotation) {
    if (mode & 0x10U) {
        const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<double>(-value.y) - 90.0) * 8192.0 / 45.0));
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
        const float radius = value.x;
        value.y = value.z;
        value.x = cosine * radius;
        value.z = sine * radius;
    }
    if ((mode & 1U) == 0)
        return;
    Vector3 position{};
    Vector3i rotation{};
    GetPosRotUtil(position, rotation, mode, node, fallbackPosition, fallbackRotation);
    if (mode & 8U) {
        position = basePosition;
        rotation = baseRotation;
    } else if (mode & 0x40U) {
        position.y = basePosition.y;
    }
    if (mode & 0x80U)
        value.x = -value.x;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (mode & 2U) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    }
    Matrix4x4::TransformPoint(value, value);
    Matrix4x4::Pop(1);
}

void spliEvaluate2_Wrapper(CameraSplineState &spline) {
    spline.eyeMode &= static_cast<std::uint16_t>(~0x80U);
    spline.targetMode &= static_cast<std::uint16_t>(~0x80U);
    if (spline.state & SplineWorldSpace) {
        spline.eyeMode |= 0x80U;
        spline.targetMode |= 0x80U;
    }
    spliEvaluate2(spline.eye, static_cast<std::uint8_t>(spline.eyeMode), spline.eyeNode, spline.fallbackPosition, spline.fallbackRotation,
        spline.eyeBasePosition, spline.eyeBaseRotation);
    spliEvaluate2(spline.target, static_cast<std::uint8_t>(spline.targetMode), spline.targetNode, spline.fallbackPosition, spline.fallbackRotation,
        spline.targetBasePosition, spline.targetBaseRotation);
}

std::uint16_t SplineModeFromFlags(std::uint32_t flags, std::uint32_t polar, std::uint32_t translate, std::uint32_t base, std::uint32_t heightOnly,
    std::uint32_t bone, std::uint32_t entity, std::uint32_t rotate, bool worldSpace) {
    std::uint16_t mode = static_cast<std::uint16_t>(polar);
    if (flags & translate)
        mode |= 1U;
    if (flags & base)
        mode |= 8U;
    if (flags & heightOnly)
        mode |= 0x40U;
    if (flags & bone)
        mode |= 4U;
    if (flags & entity)
        mode |= 0x20U;
    if (flags & rotate)
        mode |= 2U;
    if (worldSpace)
        mode |= 0x80U;
    return mode;
}



void camCtrlCreateSpline_0(CameraSplineState &spline) {
    CameraRuntime &camera = *g_cameraControl;
    spline.state |= SplineRecompute;
    spline.currentFrame = 0;
    spline.currentTime = 0.0f;
    if (spline.state & SplineChanged)
        spline.state = (spline.state & ~(SplineWorldSpace | SplineChanged)) | SplineWorldSpace;
    std::uint32_t *cursor = spline.record;
    const std::uint32_t flags = *cursor++;
    spline.authorFlags = flags;
    const bool worldSpace = (spline.state & SplineWorldSpace) != 0;
    spline.eyeMode = SplineModeFromFlags(flags, (flags >> 13) & 0x10U, 0x200U, 0x1000U, 0x2000000U, 0x800U, 0x80000U, 0x400U, worldSpace);
    spline.targetMode = SplineModeFromFlags(flags, (flags >> 14) & 0x10U, 0x2000U, 0x10000U, 0x4000000U, 0x8000U, 0x100000U, 0x4000U, worldSpace);
    GetPosRotUtil(spline.eyeBasePosition, spline.eyeBaseRotation, static_cast<std::uint8_t>(spline.eyeMode), spline.eyeNode, spline.fallbackPosition,
        spline.fallbackRotation);
    GetPosRotUtil(spline.targetBasePosition, spline.targetBaseRotation, static_cast<std::uint8_t>(spline.targetMode), spline.targetNode,
        spline.fallbackPosition, spline.fallbackRotation);
    Vector3 eye = camera.nextPosition;
    Vector3 target = camera.targetPosition;
    spliEvaluate(eye, static_cast<std::uint8_t>(spline.eyeMode), spline.eyeBasePosition, spline.eyeBaseRotation);
    spliEvaluate(target, static_cast<std::uint8_t>(spline.targetMode), spline.targetBasePosition, spline.targetBaseRotation);
    for (std::size_t index = 0; index < spline.tracks.size(); ++index)
        spline.tracks[index].coefficients = spline.coefficientStorage[index].data();
    auto bindTrack = [&cursor](CameraSplineTrack &track) {
        const std::uint32_t count = *cursor;
        auto *const keys = reinterpret_cast<float *>(cursor + 1);
        track.count = count;
        track.firstKey = 0;
        track.times = keys;
        track.values = keys + count;
        track.tangents = keys + 2 * count;
        cursor = reinterpret_cast<std::uint32_t *>(keys + 3 * count);
    };
    for (std::size_t index = 0; index < 6; ++index) {
        bindTrack(spline.tracks[index]);
        *spline.tracks[index].coefficients = (flags & (index < 3 ? 0x80U : 0x100U)) ? 1.0f : 0.0f;
    }
    if (flags & SplineDriveRoll) {
        bindTrack(spline.tracks[6]);
        *spline.tracks[6].coefficients = 0.0f;
    }
    if (flags & SplineDriveFov) {
        bindTrack(spline.tracks[7]);
        *spline.tracks[7].coefficients = 0.0f;
    }
    std::array<float, 3> eyeLocal{eye.x, eye.y, eye.z};
    std::array<float, 3> targetLocal{target.x, target.y, target.z};
    for (std::size_t axis = 0; axis < 3; ++axis) {
        CameraSplineTrack &eyeTrack = spline.tracks[axis];
        if ((flags & 0x200000U) || !eyeTrack.count)
            eyeTrack.values[0] = eyeLocal[axis];
        eyeTrack.segmentCount = static_cast<std::int32_t>(eyeTrack.count) <= 0 ? 1U : AllocSpline(eyeTrack);
        CubicHermiteEvaluate(&eyeLocal[axis], 0.0f, static_cast<std::int32_t>(eyeTrack.segmentCount), &eyeTrack.times[eyeTrack.firstKey],
            &eyeTrack.values[eyeTrack.firstKey], eyeTrack.coefficients);
        CameraSplineTrack &targetTrack = spline.tracks[3 + axis];
        if ((flags & 0x400000U) || !targetTrack.count)
            targetTrack.values[0] = targetLocal[axis];
        targetTrack.segmentCount = static_cast<std::int32_t>(targetTrack.count) <= 0 ? 1U : AllocSpline(targetTrack);
        CubicHermiteEvaluate(&targetLocal[axis], 0.0f, static_cast<std::int32_t>(targetTrack.segmentCount), &targetTrack.times[targetTrack.firstKey],
            &targetTrack.values[targetTrack.firstKey], targetTrack.coefficients);
    }
    float value{};
    if (flags & SplineDriveRoll) {
        CameraSplineTrack &roll = spline.tracks[6];
        if ((flags & 0x800000U) || !roll.count)
            roll.values[0] = static_cast<float>(camera.nextRotation.roll) / 182.04445f;
        roll.segmentCount = roll.count ? AllocSpline(roll) : 1U;
        CubicHermiteEvaluate(
            &value, 0.0f, static_cast<std::int32_t>(roll.segmentCount), &roll.times[roll.firstKey], &roll.values[roll.firstKey], roll.coefficients);
        camera.nextRotation.roll = static_cast<std::int16_t>(static_cast<std::int32_t>(value * 182.0f));
    }
    if (flags & SplineDriveFov) {
        CameraSplineTrack &fov = spline.tracks[7];
        if ((flags & 0x1000000U) || !fov.count)
            fov.values[0] = static_cast<float>(static_cast<std::int16_t>(camera.currentFovAngle)) / 182.04445f;
        fov.segmentCount = fov.count ? AllocSpline(fov) : 1U;
        CubicHermiteEvaluate(&value, 0.0f, static_cast<std::int32_t>(fov.segmentCount), &fov.times[fov.firstKey], &fov.values[fov.firstKey], fov.coefficients);
        camCtrlSetPerspective(static_cast<std::uint16_t>(static_cast<std::int32_t>(value * 182.0f)));
    }
    spline.eye = {eyeLocal[0], eyeLocal[1], eyeLocal[2]};
    spline.target = {targetLocal[0], targetLocal[1], targetLocal[2]};
    spliEvaluate2_Wrapper(spline);
    camCtrlSetNextPosition(spline.eye);
    camera.targetPosition = spline.target;
}

void camCtrlCreateSpline(std::uint32_t *record, void *eyeNode, void *targetNode) {
    CameraSplineState &spline = *g_currentCameraSplines;
    spline.name = 0;
    spline.eyeNode = eyeNode;
    spline.targetNode = targetNode;
    spline.record = record;
    camCtrlCreateSpline_0(spline);
}


void camCtrlBuildScriptCamSpline(std::int32_t index, void *eye, void *target) {
    CameraSplineState &spline = *g_currentCameraSplines;
    spline.state &= ~6U;
    spline.name = static_cast<std::uint32_t>(index);
    spline.eyeNode = eye;
    spline.targetNode = target;
    spline.record = pakFindEventCam(index);
    camCtrlCreateSpline_0(spline);
}

// @verified - p
std::uint64_t TestScriptCameraSplineCollision(std::int32_t cameraId, void *eyeNode, void *targetNode, Character *obstacle) {
    CameraSplineState &spline = *g_currentCameraSplines;
    if (spline.state & SplineRecompute)
        return UINT32_MAX;
    spline.state &= ~(SplineRecompute | SplineWorldSpace);
    spline.name = static_cast<std::uint32_t>(cameraId);
    spline.eyeNode = eyeNode;
    spline.targetNode = targetNode;
    spline.record = pakFindEventCam(cameraId);
    camCtrlCreateSpline_0(spline);
    Vector3 obstaclePosition;
    if (obstacle)
        obstaclePosition = GetCharacterWorldPosition(obstacle);
    sctlAuthAllocSplines(0);
    for (unsigned axis = 0; axis < 3; ++axis) {
        DoKeyframeCubicHermiteInterpolation(spline.tracks[axis], 0.0f, &spline.eye[axis]);
        DoKeyframeCubicHermiteInterpolation(spline.tracks[3 + axis], 0.0f, &spline.target[axis]);
    }
    spliEvaluate2_Wrapper(spline);
    Vector3 previousEye = spline.eye;
    Vector3 intersection;
    std::int32_t frame = 15;
    std::uint64_t result;
    for (;;) {
        bool continuing = false;
        sctlAuthAllocSplines(frame);
        const float time = static_cast<float>(frame) * 0.016666668f;
        for (unsigned axis = 0; axis < 3; ++axis) {
            if (DoKeyframeCubicHermiteInterpolation(spline.tracks[axis], time, &spline.eye[axis]) > 0)
                continuing = true;
            if (DoKeyframeCubicHermiteInterpolation(spline.tracks[3 + axis], time, &spline.target[axis]) > 0)
                continuing = true;
        }
        spliEvaluate2_Wrapper(spline);
        if (FindNearestRayIntersection(&intersection, &spline.eye, &previousEye) ||
            (obstacle && FindNearestRayIntersection(&intersection, &spline.eye, &obstaclePosition))) {
            result = 1;
            break;
        }
        frame += 15;
        previousEye = spline.eye;
        if (!continuing) {
            result = 0;
            break;
        }
    }
    spline.state &= ~SplineRecompute;
    return result;
}



void DoCameraSplineInterpolation(CameraSplineState &spline) {
    CameraRuntime &camera = *g_cameraControl;
    const std::int32_t frame = spline.currentFrame;
    const float time = static_cast<float>(frame) * 0.016666668f;
    if (frame < 0x6fffffff)
        spline.currentFrame = frame + 2;
    const std::uint32_t state = spline.state;
    spline.currentTime = time;
    Vector3 eye{};
    Vector3 target{};
    if (state & SplineRecompute) {
        spline.state = state & ~SplineRecompute;
        float value{};
        if (spline.authorFlags & SplineDriveRoll) {
            if (DoKeyframeCubicHermiteInterpolation(spline.tracks[6], time, &value) > 0)
                spline.state |= SplineRecompute;
            const auto roll = static_cast<std::uint16_t>(static_cast<std::int32_t>(value * 8192.0f / 45.0f));
            spline.roll = roll;
            camera.nextRotation.roll = static_cast<std::int16_t>(roll);
        }
        if (spline.authorFlags & SplineDriveFov) {
            if (DoKeyframeCubicHermiteInterpolation(spline.tracks[7], time, &value) > 0)
                spline.state |= SplineRecompute;
            const auto fov = static_cast<std::uint16_t>(static_cast<std::int32_t>(value * 8192.0f / 45.0f));
            spline.fov = fov;
            if (value > 0.0f)
                camCtrlSetPerspective(fov);
        }
        std::array<float, 3> eyeValues{};
        std::array<float, 3> targetValues{};
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (DoKeyframeCubicHermiteInterpolation(spline.tracks[axis], time, &eyeValues[axis]) > 0)
                spline.state |= SplineRecompute;
            if (DoKeyframeCubicHermiteInterpolation(spline.tracks[3 + axis], time, &targetValues[axis]) > 0)
                spline.state |= SplineRecompute;
        }
        eye = {eyeValues[0], eyeValues[1], eyeValues[2]};
        target = {targetValues[0], targetValues[1], targetValues[2]};
        spline.cachedEye = eye;
        spline.cachedTarget = target;
    } else {
        if (spline.authorFlags & SplineDriveRoll)
            camera.nextRotation.roll = static_cast<std::int16_t>(spline.roll);
        if ((spline.authorFlags & SplineDriveFov) && spline.fov > 0)
            camCtrlSetPerspective(static_cast<std::uint16_t>(spline.fov));
        eye = spline.cachedEye;
        target = spline.cachedTarget;
    }
    spline.localEye = eye;
    spline.publishedRoll = static_cast<std::uint16_t>(spline.roll);
    spline.localTarget = target;
    spline.publishedFov = static_cast<std::uint16_t>(spline.fov);
    if ((spline.authorFlags & SplineAutoAdvance) && (spline.state & SplineRecompute) == 0) {
        spline.state |= SplineRecompute;
        spline.currentFrame = 0;
    }
}

// @verified - p
void camCtrlHandleQueuedSpline() {
    CameraSplineState &spline = *g_currentCameraSplines;
    if (spline.state & SplineChanged)
        spline.state = (spline.state & ~(SplineRecompute | SplineChanged)) | SplineRecompute;
    DoCameraSplineInterpolation(spline);
    spline.eye = spline.localEye;
    spline.target = spline.localTarget;
    spliEvaluate2_Wrapper(spline);
    if (spline.state & SplineCheckEyeCollision) {
        const Vector3 eye = spline.eye;
        Vector3 previousEye, intersection;
        camCtrlGetNextCamPos(&previousEye);
        if (FindNearestRayIntersection(&intersection, &eye, &previousEye)) {
            spline.state &= ~SplineRecompute;
            return;
        }
    }
    camCtrlSetNextPosition(spline.eye);
    g_cameraControl->targetPosition = spline.target;
}

} // namespace

CameraSplineState *g_currentCameraSplines = &cameraSplines;

// @verified - p
CameraSplineState *camCtrlInitSplines() noexcept {
    cameraSplines.state = 0;
    g_currentCameraSplines = &cameraSplines;
    return g_currentCameraSplines;
}

// @verified - p
bool camCtrlCheckIsEventCam() noexcept {
    using enum CameraState;
    return (g_currentCameraSplines->state & SplineRecompute) && camCtrlGetState() == Scripted;
}

// @verified - p
std::uint64_t spliGetCurrentFrmCnt() noexcept {
    return static_cast<std::uint32_t>(g_currentCameraSplines->currentFrame);
}

// @verified - p
std::uint32_t GetEventCamName() noexcept {
    return g_currentCameraSplines->name;
}

// @verified - p
void camCtrlSetSplineShouldRebuildRotTo(bool enabled) noexcept {
    if (enabled)
        g_currentCameraSplines->state |= 0xc;
    else
        g_currentCameraSplines->state &= ~0xcU;
}

// @verified - p
CameraRuntime *camCtrlSetRotationXYZ(float x, float y, float duration) noexcept {
    CameraRuntime *const camera = g_cameraControl;
    camera->shakeAmplitude = duration > 0 ? Vector3{x, y, duration} : Vector3{0, 0, 1};
    camera->jitter.x = 0;
    camera->jitter.y = 0;
    return camera;
}

// 5206f0: this belongs to CAMT, not the global film-bar publication.
void camCtrlDisableFilmBars() noexcept {
    g_cameraControl->showFilmBars = 0;
}

namespace {



// @verified - p
void CameraProjectionPostCallbackNullsub() {
}

void camCtrlProjectCameraMatrices(void *) {
    CameraRuntime &camera = *g_cameraControl;
    camCtrlRecalcFov(camera);
    camCtrlCreatePerspectiveMatrix(camera);
    camCtrlSetup(camera);
    if (camera.updateCallback)
        camera.updateCallback(&camera);
    CameraProjectionPostCallbackNullsub();
    SetActiveStack(MatrixStackType::PostRender);
    Matrix4x4::SetActiveTransform(&camera.projectionMatrix);
    SetActiveStack(MatrixStackType::DepthPrepass);
}

// @verified - p
void camCtrlSetCamSnapAngY(CameraRuntime &camera) {
    camCtrlSetFloats(*g_cameraControl, 0.23f, 0.30000001f);
    Character *const target = FindCharacterByScenarioName(camera.targetCharacter);
    if (!target)
        return;
    const Vector3 position = GetCharacterWorldPosition(target);
    camera.targetPosition = {position.x, position.y + camera.targetHeightDistance, position.z};
    camera.orbitDistance = 3.0999999f;
    camera.nextRotation.yaw = GetAllAngYForNode(&target->entity.node);
    camera.flags |= CameraInterpolationSnap | CameraRaycastEnabled;
    camera.nextRotation.roll = 0;
    camera.nextRotation.pitch = camera.basePitchBias;
    camera.computeMode = CameraComputeMode::OrbitTarget;
}

void camCtrlCallback(void *) {
    using enum CameraState;
    CameraRuntime &camera = *g_cameraControl;
    camera.flags &= ~0x00000588U;
    camera.position = camera.nextPosition;
    camera.target = camera.targetPosition;
    camera.rotation = camera.nextRotation;
    camera.targetFovAngle = static_cast<std::int16_t>(camera.currentFovAngle);
    camera.targetOrbitDistance = camera.orbitDistance;
    camera.computeMode = CameraComputeMode::FixedPositionLookAtTarget;
    camera.stateField0e = -1;
    camera.stateField0f = -1;
    if (camera.fovLerpIterations > 0) {
        camera.flags |= CameraDisableFovInterpolation;
        --camera.fovLerpIterations;
    }
    const std::uint32_t rebuild = camera.flags & (CameraRebuildPositionSpline | CameraRebuildRotationSpline);
    camera.flags &= ~(CameraRebuildPositionSpline | CameraRebuildRotationSpline);
    if (rebuild)
        camera.flags |= 0x80U;
    // camCtrlHandleStates leaves the film-bar publication
    // unchanged for unhandled states, including CS_LOCKED.
    switch (camera.state) {
    case SplineInterpolation:
    case InterpolationFollow:
        g_cameraFilmBarsEnabled = camera.showFilmBars != 0;
        break;
    case Conversation:
        g_cameraFilmBarsEnabled = 0;
        break;
    case Init:
    case Scripted:
    case Event:
    case ChaseGoal:
    case Unknown15:
    case LookAround:
    case Unknown23:
    case LockedOn:
    case InterpolationFlagOnly:
    case Callback:
    case SnapYawTarget:
        g_cameraFilmBarsEnabled = 0;
        break;
    default:
        break;
    }
    if (camera.state == Conversation) {
        UpdateConversationCamera(camera);
    } else if (camera.state == Init) {
        camCtrlSwitchColiPhase(camera);
    } else if (camera.state == Scripted) {
        if (rebuild)
            camCtrlBuildScriptCamSpline(static_cast<std::int32_t>(camera.cameraId.name), reinterpret_cast<void *>(camera.splineBoundary),
                reinterpret_cast<void *>(camera.splineCoefficients));
        camCtrlHandleQueuedSpline();
    } else if (camera.state == SplineInterpolation) {
        if (rebuild) {
            std::uint32_t *record = nullptr;
            std::memcpy(&record, &camera.cameraId, sizeof(record));
            camCtrlCreateSpline(record, reinterpret_cast<void *>(camera.splineBoundary), reinterpret_cast<void *>(camera.splineCoefficients));
            camCtrlHandleQueuedSpline();
            camera.flags &= ~CameraSkipInterpolationOnce;
        }
        if ((camera.flags & CameraSkipInterpolationOnce) == 0)
            camCtrlHandleQueuedSpline();
    } else if (camera.state == InterpolationFollow) {
        camCtrlHandleQueuedSpline();
    } else if (camera.state == ChaseGoal) {
        camera.flags |= CameraRaycastEnabled;
        if (rebuild)
            resetCharaPosYandIntersection();
        camCtrlEvaluateGoal();
    } else if (camera.state == LookAround) {
        camCtrlHandleLookAroundCam(camera);
    } else if (camera.state == Callback && camera.stateCallback) {
        camera.stateCallback(&camera);
    } else if (camera.state == LockedOn) {
        UpdateLockedOnCamera(camera);
    } else if (camera.state == InterpolationFlagOnly) {
        camera.flags |= CameraInterpolationSnap;
    } else if (camera.state == SnapYawTarget) {
        camCtrlSetCamSnapAngY(camera);
    }
    if (camera.state == SplineInterpolation && (camera.flags & 0x00400000U)) {
        Matrix4x4_Push(&camera.finalTransform);
        camera.previousPosition = TransformFieldDepthPoint(camera.previousPosition);
        camera.targetPosition = TransformFieldDepthPoint(camera.targetPosition);
        Matrix4x4::Pop(1);
    }
    camera.flags &= ~0x00400000U;
    if (camera.flags & CameraDisableInterpolation) {
        camera.flags &= ~CameraInterpolationSnap;
    }
    if (camera.flags & CameraDisableFovInterpolation) {
        camera.flags &= ~(CameraInterpolationSnap | CameraDisableFovInterpolation);
        camera.interpolationMode = CameraInterpolationMode::None;
    }
    camCtrlApplyJitterAdjustment(camera);
    camCtrlSmoothMotion(camera);
    camCtrlComputeNextPosition(camera);
    UpdateCameraTransitions(camera);
    camCtrlCheckCollision(camera);
    camCtrlInterpolateNextPositionRotation(camera);
    camera.currentPosition = camera.nextPosition;
    camera.currentRotation = camera.nextRotation;
    camera.currentFovInterpolationAngle = static_cast<std::int16_t>(camera.currentFovAngle);
    camera.nextPosition.x += camera.jitter.x;
    camera.nextPosition.y += camera.jitter.y;
    if (camera.rotationCallback)
        camera.rotationCallback();
    camera.nextPosition.x += camera.rotationOffset.x;
    camera.nextPosition.y += camera.rotationOffset.y;
    camera.nextPosition.z += camera.rotationOffset.z;
    camera.nextRotation.pitch = static_cast<std::int16_t>(camera.nextRotation.pitch + camera.pitchOffset);
    camera.nextRotation.yaw = static_cast<std::int16_t>(camera.nextRotation.yaw + camera.yawOffset);
    camera.nextRotation.roll = static_cast<std::int16_t>(camera.nextRotation.roll + camera.rollOffset);
    ApplyRotationMatrixFromTransform(camera);
    camCtrlApply(camera);
    HLib::ExecuteCallbackNow(camCtrlProjectCameraMatrices, &camera);
    UpdateLegacyLightTransforms();
    camCtrlSetupDepthScaling();
    camCtrlSubmitFieldDepth(camera);
    camera.flags &= ~CameraDisableInterpolation;
}

} // namespace

CameraRuntime *g_cameraControl = nullptr;
std::int32_t g_cameraExposureInitialized = 0;
Vector3 g_cameraTargetPublication{};
Matrix4x4 g_cameraProjectionMatrix{};
CameraPreset g_cameraCachedPresets[3]{};
std::int32_t g_cameraFilmBarsEnabled = 0;
// 522a20: real CAMT state-10 consumer. Motion interpolation owns its path
// without VFMA; every fight mode borrows the actual six-slot producer.
void camCtrlEvaluateGoal() {
    g_cameraControl->flags |= CameraRaycastEnabled;
    if (IsWazamakiInactive() != 0)
        return;
    camCtrlSetNextPosition(fightCameraEye);
    camCtrlSetTargetPosition(fightCameraTarget);
    if (cameraMotionMode == CameraMotionMode::MotionInterp) {
        fightCameraDesiredTarget = fightCameraTarget;
        fightCameraDesiredEye = fightCameraEye;
        UpdateMotionInterpolationFightCamera();
        MoveFightCameraToward(fightCameraDesiredEye, fightCameraEye, fightCameraEyeDirection);
        MoveFightCameraToward(fightCameraDesiredTarget, fightCameraTarget, fightCameraTargetDirection);
        camCtrlSetNextPosition(fightCameraEye);
        camCtrlSetTargetPosition(fightCameraTarget);
        return;
    }
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || game->playerSlot == -1)
        return;
    Character *const character = game->characters[game->playerSlot];
    if (!character || (static_cast<std::uint8_t>(game->slotTeams[game->playerSlot] - 1) & 0xfd) == 0 || (game->mode & 0x10))
        return;
    VFActor *const self = game->actors[game->playerSlot];
    VFActor *const other = fightCameraLinkedActor ? fightCameraLinkedActor : self->actor;
    fightCameraDesiredTarget = fightCameraTarget;
    fightCameraDesiredEye = fightCameraEye;
    switch (cameraMotionMode) {
    case CameraMotionMode::Default:
    case CameraMotionMode::Default1:
    case CameraMotionMode::Environment:
        UpdateEnvironmentFightCamera(game, self, other);
        break;
    case CameraMotionMode::FieldFight:
        UpdateFieldFightCamera(self, other);
        break;
    case CameraMotionMode::FirstPerson:
        UpdateFirstPersonFightCamera(game, self, other);
        break;
    case CameraMotionMode::AutoQuadratic:
    case CameraMotionMode::AutoQuadratic2:
        UpdateQuadraticFightCamera(game, self, other);
        break;
    case CameraMotionMode::MotionUpdate:
        UpdateMotionFightCamera(self, other);
        break;
    case CameraMotionMode::MotionInterp:
        UpdateMotionInterpolationFightCamera();
        break;
    default:
        break;
    }
    if (static_cast<std::uint32_t>(game->state) >= 4 && (g_combatCameraFlags & 4))
        return;
    Vector3 ray{fightCameraDesiredEye.x - fightCameraDesiredTarget.x, fightCameraDesiredEye.y - fightCameraDesiredTarget.y,
        fightCameraDesiredEye.z - fightCameraDesiredTarget.z};
    const float distance = FightCameraLength(ray);
    ScaleFightCameraVector(ray, distance + 0.30000001f);
    Vector3 result{fightCameraDesiredTarget.x + ray.x, fightCameraDesiredTarget.y + ray.y, fightCameraDesiredTarget.z + ray.z};
    const Vector3 origin =
        cameraMotionMode == CameraMotionMode::FirstPerson ? Vector3{self->contactPosition.x, result.y, self->contactPosition.z} : fightCameraDesiredTarget;
    const bool hit = FightCameraRaycast(result, result, origin);
    ray = {result.x - fightCameraDesiredTarget.x, result.y - fightCameraDesiredTarget.y, result.z - fightCameraDesiredTarget.z};
    const float clippedDistance = FightCameraLength(ray) - 0.1f;
    if (distance > clippedDistance) {
        ScaleFightCameraVector(ray, clippedDistance);
        fightCameraDesiredEye = {fightCameraDesiredTarget.x + ray.x, fightCameraDesiredTarget.y + ray.y, fightCameraDesiredTarget.z + ray.z};
    }
    if (static_cast<std::uint32_t>(game->state) >= 4 && hit)
        g_combatCameraFlags |= 4;
    MoveFightCameraToward(fightCameraDesiredEye, fightCameraEye, fightCameraEyeDirection);
    MoveFightCameraToward(fightCameraDesiredTarget, fightCameraTarget, fightCameraTargetDirection);
    if (cameraMotionMode != CameraMotionMode::FirstPerson && g_currentMap.name != SHEN_NAME(QKBT)) {
        const auto query = [&] {
            flddToggleFlag(2054);
            return g_currentMap == AREA_QF06 ? RaycastForCollision(&result, &fightCameraEye, &fightCameraTarget) :
                                               FindNearestRayIntersection(&result, &fightCameraEye, &fightCameraTarget);
        };
        if (query()) {
            fightCameraEye = fightCameraDesiredEye;
            if (query())
                fightCameraEye = result;
        }
    }
    float maximumY = 100000;
    switch (g_currentMap.name) {
    case SHEN_NAME(TOFB):
    case SHEN_NAME(WK00):
    case SHEN_NAME(QDTG):
    case SHEN_NAME(CWON):
    case SHEN_NAME(GABT):
        flddGetIntersectionFor(character, self->contactPosition);
        maximumY = GetQronTemporaryFloorHeight() + 2.5f;
        break;
    case SHEN_NAME(QF40):
        maximumY = flddGetIntersectionFor(character, self->contactPosition) + 3;
        break;
    case SHEN_NAME(WEG0): {
        const float minimumY = flddGetIntersectionFor(character, self->contactPosition) + 1.5f;
        if (minimumY > fightCameraEye.y)
            fightCameraEye.y = minimumY;
        break;
    }
    case SHEN_NAME(SYW0):
        maximumY = flddGetIntersectionFor(character, self->contactPosition) + 2;
        break;
    default:
        if (static_cast<std::int32_t>(GetQronNextStream()) != -1000)
            maximumY = GetQronTemporaryFloorHeight() + 2.5f;
        break;
    }
    if (fightCameraEye.y > maximumY)
        fightCameraEye.y = maximumY;
    if (fightCameraTarget.y > maximumY)
        fightCameraTarget.y = maximumY;
    camCtrlSetNextPosition(fightCameraEye);
    camCtrlSetTargetPosition(fightCameraTarget);
}

void camCtrlToggleSplineRebuildFlag(std::int32_t count, const void *source) {
    std::memcpy(&g_cameraControl->cameraId, source, 8U * static_cast<std::size_t>(count));
    g_cameraControl->flags |= CameraRebuildRotationSpline;
}

// @verified - p
std::uint64_t EVS_LoopCamera(std::int64_t cameraId, shen_name eye, shen_name target) {
    using enum CameraState;
    struct CameraRequest {
        std::int64_t id;
        void *eyeNode;
        void *targetNode;
    } request{cameraId, nullptr, nullptr};
    static_assert(sizeof(CameraRequest) == 0x18);
    if (eye.name != 0)
        request.eyeNode = FindCharacterByScenarioName(eye);
    if (target.name != 0)
        request.targetNode = FindCharacterByScenarioName(target);
    camCtrlToggleSplineRebuildFlag(3, &request);
    return camCtrlSetState(Scripted);
}




// @verified - p
std::uint64_t sctlAuthAllocSplines(std::int32_t frame) {
    const std::uint32_t state = static_cast<std::uint32_t>(g_cameraControl->state);
    std::uint32_t result = state - 6U;
    if ((result & 0xfffffffdU) != 0)
        return result;
    CameraSplineState &spline = *g_currentCameraSplines;
    spline.currentFrame = 2 * (frame / 2);
    const auto seek = [&spline](CameraSplineTrack &track) {
        const float time = static_cast<float>(spline.currentFrame) * 0.016666668f;
        const std::int32_t last = static_cast<std::int32_t>(track.count) - 1;
        track.firstKey = 0;
        for (std::int32_t index = 0; index < last; ++index) {
            if (track.times[index] > time)
                break;
            if (track.times[index] == track.times[index + 1])
                track.firstKey = index + 1;
        }
    };
    for (std::size_t axis = 0; axis < 3; ++axis) {
        seek(spline.tracks[axis]);
        seek(spline.tracks[3 + axis]);
        spline.tracks[axis].segmentCount = AllocSpline(spline.tracks[axis]);
        result = spline.tracks[3 + axis].segmentCount = AllocSpline(spline.tracks[3 + axis]);
    }
    if (spline.authorFlags & SplineDriveRoll) {
        seek(spline.tracks[6]);
        result = spline.tracks[6].segmentCount = AllocSpline(spline.tracks[6]);
    }
    if (spline.authorFlags & SplineDriveFov) {
        seek(spline.tracks[7]);
        result = spline.tracks[7].segmentCount = AllocSpline(spline.tracks[7]);
    }
    return result;
}

void camCtrlSetInterpEaseFactor(float factor) {
    g_cameraControl->interpolationEaseFactor = factor < 0.0f ? 0.1f : factor;
}

namespace {

// @verified - p
void AssignConversationShotTypes() {
    ConversationCameraState &state = conversationCamera;
    if (state.shotCount <= 0)
        return;

    const ConversationShotHistory &history = state.history[(state.historyCursor + state.history.size() - 1) % state.history.size()];
    std::int32_t previousType = history.candidate >= 0 ? history.shotType : -1;
    Character *previousTarget = history.candidate >= 0 && history.eyeName.name ? authFindChara(history.eyeName) : nullptr;
    std::int32_t lastSpecialShot = -1;
    for (std::int32_t shot = 0; shot < state.shotCount; ++shot) {
        if (state.shotFlags[static_cast<std::size_t>(shot)] & 8U)
            lastSpecialShot = shot;
    }
    for (std::int32_t shot = 0; shot < state.shotCount; ++shot) {
        const std::size_t index = static_cast<std::size_t>(shot);
        Character *target = state.targetAnchors[index];
        Character *eye = state.eyeAnchors[index];
        Character *const partner = state.partners[index];
        bool type = shot == 0 ? !(previousType == 1 && previousTarget == target) : previousType != 1 && previousType != 3;
        if (state.shotFlags[index] & 2U)
            type = true;
        if ((state.flags & 0x00040000U) && state.actorHeightDelta < 0.0f && target == state.root) {
            type = false;
            if (previousType == 0 && previousTarget == target) {
                type = true;
                target = partner;
                eye = state.root;
            }
        }
        if ((state.sceneFlags & 0x28U) && target == state.root)
            type = false;
        if (state.shotFlags[index] & 8U) {
            type = true;
            if (AreConversationAngleCandidatesBlocked()) {
                type = false;
                target = state.root;
                eye = partner;
            } else if (std::abs(static_cast<std::int32_t>(conversationCamera.orientationClass)) < 5) {
                target = partner;
                eye = state.root;
            } else {
                target = state.root;
                eye = partner;
            }
        }
        if (state.shotFlags[index] & 8U) {
            if ((state.sceneFlags & 0x28U) && type) {
                if (target == state.root)
                    type = false;
            }
            if (target == state.root) {
                if (type && std::abs(static_cast<std::int32_t>(conversationCamera.orientationClass)) >= 4) {
                    type = false;
                    target = state.root;
                    eye = partner;
                } else if (!type && std::abs(static_cast<std::int32_t>(conversationCamera.orientationClass)) < 4) {
                    type = true;
                    target = partner;
                    eye = state.root;
                }
            }
        }
        if (shot) {
            const bool oldType = type;
            if (previousType == static_cast<std::int32_t>(type) && previousTarget == target) {
                if ((state.sceneFlags & 0x28U) && !type && target == state.root) {
                    target = partner;
                    eye = state.root;
                }
                type = !type;
                if ((state.shotFlags[index] & 8U) && oldType) {
                    target = state.root;
                    eye = partner;
                }
            }
        } else {
            if (state.flags & 8U)
                type = false;
            if ((state.sceneFlags & 8U) && (state.flags & 4U))
                type = true;
            if (lastSpecialShot >= 0)
                type = (lastSpecialShot & 1) == 0;
        }
        state.shotTypes[index] = type ? 1 : 0;
        state.targetAnchors[index] = target;
        state.eyeAnchors[index] = eye;
        previousType = type ? 1 : 0;
        previousTarget = target;
    }
}

} // namespace

namespace {
// @verified - p
void CompressConversationCameraStream() {
    ConversationCameraState &state = conversationCamera;
    const CameraDialogueRecord *const stream = state.preparedStream;
    std::byte *output = conversationPacketScratch.data();
    CameraDialogueRecord *previous = nullptr;
    const CameraDialogueRecord *input = stream;
    std::int32_t merged = 0;
    std::int32_t processed = 0;
    bool retainFirst = false;
    bool mergeRemainder = false;
    while (!(input->flags & 1U)) {
        const std::size_t bytes = static_cast<std::uint8_t>(input->header) * 8U;
        CameraDialogueRecord *destination = reinterpret_cast<CameraDialogueRecord *>(output);
        if (processed && retainFirst) {
            destination = reinterpret_cast<CameraDialogueRecord *>(conversationPacketScratch.data());
            std::memcpy(destination, input, bytes);
            destination->duration += stream->duration;
            destination->flags |= stream->flags | 0x40U;
            output = conversationPacketScratch.data() + bytes;
            retainFirst = false;
            ++merged;
        } else if (processed && (mergeRemainder || (input->duration <= 2.0999999f && merged < 2 && previous->duration <= 6.3000002f))) {
            destination = previous;
            destination->duration = input->duration + destination->duration;
            destination->flags |= input->flags | 0x40U;
            ++merged;
        } else {
            std::memcpy(destination, input, bytes);
            if (!processed && input->duration <= 2.0999999f)
                retainFirst = true;
            output += bytes;
            merged = 0;
        }
        previous = destination;
        if (input->flags & 2U)
            mergeRemainder = true;
        input = reinterpret_cast<const CameraDialogueRecord *>(reinterpret_cast<const std::byte *>(input) + bytes);
        ++processed;
    }
    std::memcpy(output, input, static_cast<std::uint8_t>(input->header) * 8U);

    state.preparedStream = reinterpret_cast<const CameraDialogueRecord *>(conversationPacketScratch.data());
}

// @verified - p
void ReadConversationCameraPackets() {
    ConversationCameraState &state = conversationCamera;
    const CameraDialogueRecord *input = state.preparedStream;
    std::int8_t shotCount = 0, memberCount = 0;
    std::int16_t totalFrames = 0;
    const auto addMember = [&state, &memberCount](Character *character) {
        if (!character)
            return;
        for (std::int32_t index = 0; index < memberCount; ++index) {
            if (state.members[index] == character)
                return;
        }
        const std::size_t index = static_cast<std::size_t>(memberCount++);
        state.members[index] = character;
    };
    while (!(input->flags & 1U)) {
        const std::int32_t index = shotCount++;
        addMember(input->speaker);
        addMember(input->listener);
        std::uint8_t &flags = state.shotFlags[index];
        if (input->flags & 0x40U)
            flags |= 1U;
        if (input->flags & 2U) {
            flags |= 8U;
            state.flags |= 0x80000U;
            state.sceneFlags |= 2U;
        }
        if (input->flags & 8U) {
            flags |= 0x40U;
            state.sceneFlags |= 4U;
        }
        state.shots[index] = input;
        totalFrames = static_cast<std::int16_t>(totalFrames + static_cast<std::int32_t>(input->duration * 30.0f));
        input = reinterpret_cast<const CameraDialogueRecord *>(reinterpret_cast<const std::byte *>(input) + static_cast<std::uint8_t>(input->header) * 8U);
    }
    state.shotCount = shotCount;
    state.memberCount = memberCount;
    state.totalFrames = totalFrames;
    if (shotCount)
        state.shotFlags[shotCount - 1] |= 0x10U;
    if (memberCount > 2) {
        for (std::int32_t shot = 0; shot < shotCount; ++shot) {
            const CameraDialogueRecord &record = *state.shots[shot];
            for (std::int32_t member = 0; member < memberCount; ++member) {
                if (state.memberSeen[member] != 1 && (state.members[member] == record.speaker || state.members[member] == record.listener)) {
                    state.memberSeen[member] = 1;
                    state.shotFlags[shot] |= 2U;
                }
            }
        }
    }
}

// @verified - p
void AssignConversationShotAnchors() {
    ConversationCameraState &state = conversationCamera;
    for (std::int32_t shot = 0; shot < state.shotCount; ++shot) {
        const CameraDialogueRecord &record = *state.shots[shot];
        Character *target = record.listener;
        if (record.speaker != state.root && target != state.root)
            target = state.root;
        Character *const partner = record.speaker == state.root ? target : record.speaker;
        Character *eye = record.speaker;
        if (!shot && (state.flags & 0xcU) == 4U) {
            eye = partner;
            target = state.root;
        }
        if (state.flags & 0x20000000U) {
            eye = record.speaker;
            target = record.speaker != state.root && record.listener != state.root ? state.root : record.listener;
        }
        state.eyeAnchors[shot] = target;
        state.targetAnchors[shot] = eye;
        state.partners[shot] = partner;
    }
}
} // namespace

void PrepareConversationCameraStream(const CameraDialogueRecord *stream) {
    conversationCamera.source = conversationCamera.preparedStream = stream;
    conversationCamera.shotCount = conversationCamera.memberCount = conversationCamera.shotIndex = 0;
    conversationCamera.totalFrames = 0;
    conversationCamera.memberSeen = {1, 1, 0, 0, 0};
    conversationCamera.shotFlags.fill(0);
    conversationCamera.root = CameraRootCharacter();
    CompressConversationCameraStream();
    ReadConversationCameraPackets();
    AssignConversationShotAnchors();
}

void camCtrlSetFilmBarEnabled(std::int32_t enabled) noexcept {
    g_cameraFilmBarsEnabled = enabled;
}

void PauseConversationCameraClock(bool paused) noexcept {
    ConversationCameraState &state = conversationCamera;
    if ((state.flags & 2U) == 0)
        return;
    const std::uint32_t now = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    if (paused) {
        state.pauseStartTime = now;
    } else {
        state.shotStartTime += now - state.pauseStartTime;
    }
}

// @verified - p
std::int64_t ActivateConversationCameraStream(const CameraDialogueRecord *stream) {
    using enum CameraState;
    if (!stream || !g_cameraControl)
        return 0;
    const auto stateValue = static_cast<std::int32_t>(g_cameraControl->state);
    const std::int32_t guard = stateValue - static_cast<std::int32_t>(Scripted);
    if ((guard & static_cast<std::int32_t>(0xfffffffdU)) == 0)
        return guard;

    ConversationCameraState &state = conversationCamera;
    state.root = CameraRootCharacter();
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    state.rootScenarioName = scenGetCharaName(control->scenarioTask, state.root);
    if (!state.root)
        return state.rootScenarioName.name;
    const bool wasActive = state.shotGenerationCount != 0;
    state.flags &= 0xb0100400U;
    state.collisionMode = 3;
    state.shotFrame = state.shotDurationFrames = state.shotProgress = 0.0f;
    state.currentShotFlags = 0;
    state.shotTypes.fill(-1);
    state.totalFrames = state.viewUpdateCount = 0;
    state.activationFrames = state.specialOverrideFrames = 0;
    state.lookDelayFrames = state.lookTurnFrames = 0;
    state.actorDistance = state.actorHeightDelta = state.rootPartnerHeightDelta = 0.0f;
    if ((conversationCamera.flags & 0x20000000U) == 0)
        conversationCamera.flags &= ~0x00200000U;
    if (IsActiveScenarioCameraSuppressed())
        conversationCamera.flags |= 8U;
    if (!wasActive)
        conversationCamera.flags |= 4U;
    if (plymCheckAuthorTargetRequested())
        conversationCamera.flags |= 0x00400000U;
    conversationCamera.sceneFlags = plymCheckAuthorTargetRequested() ? 0x10U : 0U;
    state.shotStartTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    state.source = state.preparedStream = stream;
    state.shotCount = state.memberCount = state.shotIndex = 0;
    state.memberSeen = {1, 1, 0, 0, 0};
    state.shotFlags.fill(0);
    CompressConversationCameraStream();
    ReadConversationCameraPackets();
    AssignConversationShotAnchors();
    if (conversationCamera.shotCount <= 0)
        return 0;
    BindConversationShotActors();
    state.flags |= 0x40U;
    UpdateConversationActorPose();
    state.flags &= ~0x40U;
    RefreshConversationSceneFlags();
    if (state.flags & 0x80000U)
        UpdateConversationAngleClass();
    SelectConversationCameraSide();
    AssignConversationShotTypes();
    conversationCamera.activationFrames = (conversationCamera.sceneFlags & 0x10U) || !(conversationCamera.flags & 4U) ? 1 : 7;
    TickConversationActivation(*g_cameraControl);
    camCtrlGetNextCamPos(&conversationCamera.framePreviousEye);
    camCtrlGetTargetPosition(&conversationCamera.framePreviousTarget);
    conversationCamera.framePreviousFieldOfView = camCtrlGetPerspective();
    CaptureConversationParticipantAlpha();
    conversationCamera.flags |= 0x202U;
    return conversationCamera.memberCount;
}

// @verified - p
void RetireConversationCamera() {
    using enum CameraState;
    ConversationCameraState &state = conversationCamera;
    state.flags &= ~0x82U;
    state.forcedCandidate = -1;
    state.shotGenerationCount = 0;
    RestoreConversationParticipantAlpha();
    const std::size_t retained = (state.historyCursor + state.history.size() - 1) % state.history.size();
    for (std::size_t index = 0; index < state.history.size(); ++index) {
        if (index != retained) {
            state.history[index].candidate = -1;
            state.history[index].shotType = -1;
        }
    }
    if (!g_cameraControl || g_cameraControl->state != Conversation) {
        return;
    }
    const bool hasFollow = srchGetCurrentFollowCharacter().name != 0;
    if (hasFollow) {
        if (!(state.flags & 0x20U))
            return;
    } else {
        camCtrlSetState(Init);
        if (!(state.flags & 0x20U)) {
            camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
            g_cameraControl->interpolationEaseFactor = 0.02f;
            return;
        }
    }
    camCtrlSetFovTransitionTo(true);
}

void ResumeConversationCamera() {
    using enum CameraState;
    if (g_cameraControl && g_cameraControl->state == Conversation) {
        conversationCamera.flags |= 0x82U;
        if (conversationCamera.shotType)
            conversationCamera.flags |= 0x2000U;
    }
}

// The stream's last line has played. The camera keeps its shot for the retire only when that shot already frames the root
// from behind, or a look-location shot's orbit is clear; otherwise RetireConversationCamera cuts (0x20).
void EndConversationCameraStream() {
    ConversationCameraState &state = conversationCamera;
    if (!(state.flags & 2U))
        return;
    state.flags &= 0xf7effbfdU;
    const std::size_t shot = static_cast<std::size_t>(state.shotIndex);
    if (!(state.currentShotFlags & 8U)) {
        if (state.shotType == 1 && state.root && state.specialTargetName.name == GetCharacterScenarioName(state.targetAnchors[shot]).name) {
            Vector3 root{};
            GetCharacterMotionWorldPosition(state.root, 5, &root);
            const std::int16_t yaw = GetAllAngYForNode(&state.root->entity.node);
            Vector3 eye{};
            camCtrlGetNextCamPos(&eye);
            EulerRotation16 rotation{};
            camCtrlGetRot(&rotation);
            const Vector3 offset{eye.x - root.x, eye.y - root.y, eye.z - root.z};
            const std::int16_t bearing = static_cast<std::int16_t>(FixedAtan2(offset.x, offset.z) + 0x8000);
            if (std::abs(static_cast<std::int16_t>(bearing - yaw)) > 10922 && std::abs(static_cast<std::int16_t>(rotation.yaw - yaw)) < 0x2000 &&
                std::abs(rotation.pitch) < 0x2000)
                return;
        }
        state.flags |= 0x20U;
    } else if (state.shotType == 1 && GetCharacterScenarioName(state.eyeAnchors[shot]).name == state.specialTargetName.name) {
        const Vector3 target = GetPlayerTargetWorldPosition();
        Vector3 position{}, lookAt{};
        camCtrlComputeOrbitCameraFromTarget(&target, InverseFixedAtan2(state.lookLocation.x - target.x, state.lookLocation.z - target.z), &position, &lookAt);
        flddToggleFlag(4096);
        if (flddFindIntersections(state.eye, position))
            state.flags |= 0x20U;
    }
}


std::int64_t camCtrlHoldConversationShot(std::int32_t release) {
    ConversationCameraState &state = conversationCamera;
    if (!(state.flags & 2U))
        return 0;
    if (release) {
        state.flags &= ~0x100U;
        if (release >= 0 && g_cameraControl)
            TickConversationActivation(*g_cameraControl);
        return release;
    }
    state.flags |= 0x100U;
    if (g_cameraControl)
        TickConversationActivation(*g_cameraControl);
    return 0;
}


std::int64_t camCtrlSetConversationLookTarget(const Vector3 *location) {
    conversationCamera.lookLocation = *location;
    conversationCamera.flags |= 0x100000U;
    return 0;
}

bool IsConversationCameraCharacter(const Character *character) {
    if (!(conversationCamera.flags & 2U))
        return false;
    for (std::int32_t index = 0; index < conversationCamera.memberCount; ++index) {
        if (conversationCamera.members[index] == character)
            return true;
    }
    return false;
}

shen_name GetCameraSpecialTargetName() {
    return (conversationCamera.flags & 2U) ? conversationCamera.specialTargetName : shen_name{};
}



shen_name camCtrlGetConversationEyeAnchorName() {
    if (conversationCamera.shotIndex < 0)
        return {};
    return GetCharacterScenarioName(conversationCamera.eyeAnchors[static_cast<std::size_t>(conversationCamera.shotIndex)]);
}

// @verified - p
bool camCtrlHasConversationFlag8000() noexcept {
    return (conversationCamera.flags & 0x8000U) != 0;
}

// @verified - p
void camCtrlPublishTargetPosition(const Vector3 *target) noexcept {
    g_cameraTargetPublication = *target;
}

void camCtrlResetTransition(CameraInterpolationMode mode) {
    CameraRuntime &camera = *g_cameraControl;
    camera.previousCameraPosition = camera.nextPosition;
    camera.transitionTargetPosition = camera.targetPosition;
    camCtrlPublishTargetPosition(&camera.targetPosition);
    camera.transitionTargetRotation = camera.nextRotation;
    camera.transitionTargetFovAngle = static_cast<std::int16_t>(camera.currentFovAngle);
    camera.transitionTargetOrbitDistance = camera.orbitDistance;
    camera.interpolationProgress = 0.0f;
    camera.transitionStep = 0;
    switch (mode) {
    case CameraInterpolationMode::None:
        break;
    case CameraInterpolationMode::RestorePrevious:
        camera.interpolationEaseFactor = 0.1f;
        break;
    case CameraInterpolationMode::PreviousToCurrent:
        camera.transitionStepCount = 30;
        break;
    case CameraInterpolationMode::ToTargetContinuous:
    case CameraInterpolationMode::ToTargetTimed:
        if (camera.computeMode != CameraComputeMode::OrbitTarget) {
            camera.transitionTargetOrbitDistance = CameraDistance(camera.nextPosition, camera.targetPosition);
        }
        if (mode == CameraInterpolationMode::ToTargetContinuous)
            camera.interpolationEaseFactor = 0.1f;
        else
            camera.transitionStepCount = 30;
        break;
    default:
        mode = CameraInterpolationMode::None;
        break;
    }
    camera.interpolationMode = mode;
}

// @verified - p
std::uint64_t camCtrlHandleContextSwitch(std::uint8_t newContextMode) {
    CameraRuntime &camera = *g_cameraControl;
    camera.flags &= ~CameraAllowContextSwitch;
    const std::int32_t previous = camera.contextMode;
    if (previous == newContextMode && newContextMode == 10)
        camera.flags |= CameraAllowContextSwitch;
    if (previous != newContextMode) {
        camera.contextMode = static_cast<std::int8_t>(newContextMode);
        if (newContextMode == 10) {
            camera.targetHeightDistance = firstPersonContextSetting.distanceFromPlayer;
            camera.basePitchBias = firstPersonContextSetting.basePitchBias;
            camera.flags |= CameraAllowContextSwitch;
            return static_cast<std::uint32_t>(previous);
        }
        const std::uint32_t index = newContextMode + 5U < 10U ? newContextMode + 5U : 10U;
        camera.targetHeightDistance = cameraContextSettings[index].distanceFromPlayer;
        camera.basePitchBias = cameraContextSettings[index].basePitchBias;
    }
    return static_cast<std::uint32_t>(previous);
}

std::int32_t camCtrlSetContext(std::int32_t allowSwitch, std::int32_t firstPerson) {
    using enum CameraState;
    CameraRuntime &camera = *g_cameraControl;
    camera.flags &= ~CameraAllowContextSwitch;
    const std::int32_t contextMode = camera.contextMode;
    if (allowSwitch) {
        if (camera.contextMode != 10) {
            camera.contextMode = 10;
            camera.targetHeightDistance = firstPersonContextSetting.distanceFromPlayer;
            camera.basePitchBias = firstPersonContextSetting.basePitchBias;
        }
        camera.flags |= CameraAllowContextSwitch;
        if (!firstPerson) {
            camera.contextMode = 0;
            camera.targetHeightDistance = cameraContextSettings[5].distanceFromPlayer;
            camera.basePitchBias = cameraContextSettings[5].basePitchBias;
        }
    } else if (camera.contextMode != 0) {
        camera.contextMode = 0;
        camera.targetHeightDistance = cameraContextSettings[5].distanceFromPlayer;
        camera.basePitchBias = cameraContextSettings[5].basePitchBias;
    }
    if (camera.state == Init) {
        camCtrlResetTransition(CameraInterpolationMode::PreviousToCurrent);
        camera.transitionStepCount = 30;
    }
    return contextMode;
}

std::uint16_t camCtrlGetFov() noexcept {
    return g_cameraControl ? g_cameraControl->fovAngle : 0;
}

// @verified - p
Vector3 *camCtrlGetTargetPosition(Vector3 *output) noexcept {
    CameraRuntime *const camera = g_cameraControl;
    if (output)
        *output = camera->targetPosition;
    camCtrlPublishTargetPosition(&camera->targetPosition);
    return &camera->targetPosition;
}

// @verified - p
Matrix4x4 *camCtrlGetViewMatrix(Matrix4x4 *output) noexcept {
    if (output)
        *output = g_cameraControl->viewMatrix;
    return &g_cameraControl->viewMatrix;
}

// @verified - p
Matrix4x4 *camCtrlGetProjectionMatrix(Matrix4x4 *output) noexcept {
    if (output)
        *output = g_cameraControl->projectionMatrix;
    return &g_cameraControl->projectionMatrix;
}

CameraState camCtrlGetState() noexcept {
    return g_cameraControl->state;
}

// @verified - p
std::uint16_t camCtrlGetPerspective() noexcept {
    return g_cameraControl->currentFovAngle;
}

std::int64_t camCtrlGetAdjustedPitchOrYaw(std::uint16_t *yaw, std::uint16_t *pitch) noexcept {
    CameraRuntime *const camera = g_cameraControl;
    if (yaw) {
        yaw[0] = static_cast<std::uint16_t>(camera->basePitchBias);
        yaw[1] = static_cast<std::uint16_t>(camera->nextRotation.yaw);
        if (!pitch)
            return yaw[1];
    }
    if (pitch) {
        pitch[0] = static_cast<std::uint16_t>(static_cast<std::uint16_t>(camera->nextRotation.pitch) - static_cast<std::uint16_t>(camera->basePitchBias));
        pitch[1] = 0;
        return 0;
    }
#if defined(_MSC_VER)
    __assume(0);
#else
    __builtin_unreachable();
#endif
}

std::int32_t camCtrlGetNextRotY() noexcept {
    return g_cameraControl->nextRotation.yaw;
}

// @verified - p
EulerRotation16 *camCtrlGetRot(EulerRotation16 *output) noexcept {
    CameraRuntime *const camera = g_cameraControl;
    if (output)
        *output = camera->nextRotation;
    return &camera->nextRotation;
}

// @verified - p
float camCtrlGetFarDistance() noexcept {
    return g_cameraControl->farDistance;
}

// @verified - p
float camCtrlGetNearDistance() noexcept {
    return g_cameraControl->nearDistance;
}

// @verified - p
float camCtrlGetNpcRadius() noexcept {
    return g_cameraControl->npcRadius;
}

float camCtrlGetHalfVerticalFovTangent() noexcept {
    return g_cameraControl->halfVerticalFovTangent;
}

float camCtrlGetPitchAngleTangent() noexcept {
    return g_cameraControl->pitchAngleTangent;
}

void camCtrlSetFov(std::int16_t fieldOfView) noexcept {
    if (g_cameraControl)
        g_cameraControl->fovAngle = static_cast<std::uint16_t>(fieldOfView);
}

// @verified - p
void camCtrlSetTargetPosition(const Vector3 &target) noexcept {
    g_cameraControl->targetPosition = target;
}

// @verified - p
void camCtrlSetPerspective(std::uint16_t perspective) noexcept {
    if (perspective)
        g_cameraControl->currentFovAngle = perspective;
}

// PPFX's reset call is middleware; the game-owned distance latch remains live without PPFX.
void camCtrlSetNextPosition(const Vector3 &position) noexcept {
    CameraRuntime *const camera = g_cameraControl;
    const float dy = camera->previousPosition.y - position.y;
    const float dx = camera->previousPosition.x - position.x;
    const float dz = camera->previousPosition.z - position.z;
    camera->previousPosition = position;
    g_cameraExposureInitialized = std::sqrt((dy * dy + dx * dx) + dz * dz) <= 1.1f ? 0 : 1;
}

// @verified - p
std::uint64_t camCtrlSetState(CameraState state) {
    using enum CameraState;
    CameraRuntime &camera = *g_cameraControl;
    camera.showFilmBars = 1;
    if (camera.state == state)
        return static_cast<std::uint32_t>(state);
    camera.previousState = camera.state;
    camera.state = state;
    camera.flags = (camera.flags & ~0x00800000U) | CameraRebuildPositionSpline;
    if (state == Init) {
        if (camera.collisionPhase == 1)
            camera.collisionPhase = 2;
        if (camera.previousState == Conversation)
            camera.flags |= 0x00800000U;
    } else if (state != LookAround && state != LockedOn) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        static_cast<Character *>(control->ryo)->alpha = 1.0f;
    }
    camCtrlResetTransition(CameraInterpolationMode::Default);
    return static_cast<std::uint32_t>(camera.previousState);
}

void camCtrlUpdate() {
    if (g_cameraControl)
        camCtrlCallback(nullptr);
}

bool camCtrlComputeOrbitCameraFromTarget(const Vector3 *target, std::int16_t yaw, Vector3 *position, Vector3 *lookAt) {
    if (!g_cameraControl || !target || !position || !lookAt)
        return false;


    CameraRuntime camera{};
    camera.targetPosition = *target;
    camera.targetPosition.y += g_cameraControl->targetHeightDistance;
    camera.orbitDistance = g_cameraControl->defaultOrbitDistance;
    camera.nextRotation.pitch = g_cameraControl->basePitchBias;
    camera.nextRotation.yaw = yaw;
    const auto pitch = static_cast<std::int16_t>(-camera.nextRotation.pitch);
    if (camera.nextRotation.pitch < 0) {
        const float heightCap = std::fmax(0.2f, (2.6500001f - (camera.targetPosition.y - target->y)) / sin_fixed16(pitch));
        if (camera.orbitDistance > heightCap)
            camera.orbitDistance = heightCap;
    }
    if (g_cameraControl->contextMode == 10)
        camera.orbitDistance = 0.0099999998f;
    camCtrlGetIdealCameraPosition(camera);
    if (camera.orbitDistance < 3.0999999f && g_cameraControl->contextMode != 10) {
        camera.targetPosition.y += std::fmin((3.0999999f - camera.orbitDistance) / 3.0999999f * 0.80000001f, 0.80000001f);
    }
    const float horizontal = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(pitch)))) * camera.orbitDistance;
    *position = {sin_fixed16(yaw) * horizontal + camera.targetPosition.x, sin_fixed16(pitch) * camera.orbitDistance + camera.targetPosition.y,
        sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw)))) * horizontal + camera.targetPosition.z};
    *lookAt = camera.targetPosition;
    return true;
}

void srchSetupCameraFov() {
    searchCamera.zoomScale = -1.0f;
    searchCamera.fovStyle = searchCamera.phase = 0;
    searchCamera.lookBase = {};
    searchCamera.rotationY = 0;
    searchCamera.lockedOnAngle = 0.0f;
    searchCamera.pitch = searchCamera.yaw = 0;
    searchCamera.fov = 10922;
    searchCamera.zoomFov = 5461;
}

void srchSetLookTarget(const Vector3 *target) {
    if (target)
        searchCamera.lookTarget = *target;
    else {
        searchCamera.lookTarget = GetPlayerTargetWorldPosition();
        searchCamera.lookTarget.y += 1.6f;
    }
}
void srchSetLookBase(const Vector3 *base) {
    searchCamera.lookBase = *base;
}
void srchGetLookBase(Vector3 *base) {
    *base = searchCamera.lookBase;
}
void srchGetLookTarget(Vector3 *target) {
    *target = searchCamera.lookTarget;
}
void srchSetRotY(std::int16_t yaw) {
    searchCamera.rotationY = yaw;
}
void camCtrlSetLockedOnAngle(float angle) {
    searchCamera.lockedOnAngle = angle;
}
bool shenCheckCurrentFoVModeIs2() {
    return searchCamera.phase == 2;
}
bool camCtrlIsTransitionActive() {
    return g_cameraControl && g_cameraControl->interpolationMode != CameraInterpolationMode::None;
}
std::uint32_t shenApplyLockonFlag(std::uint32_t flags) {
    const std::uint32_t previous = searchCamera.flags & flags;
    searchCamera.flags |= flags;
    return previous;
}

bool camCtrlSwitchToLockedOn(std::int32_t fovStyle) {
    using enum CameraState;
    if (!g_cameraControl)
        return false;
    const CameraState previous = g_cameraControl->state;
    if (previous != LockedOn) {
        searchCamera.zoomScale = -1.0f;
        searchCamera.pitch = searchCamera.yaw = 0;
        searchCamera.fov = 10922;
        searchCamera.zoomFov = 5461;
    }
    camCtrlSetState(LockedOn);
    searchCamera.phase = 1;
    searchCamera.fovStyle = fovStyle;
    searchCamera.flags = previous == Conversation && (g_cameraControl->flags & CameraDisableFovInterpolation) ? 2U : 0U;
    return true;
}

bool camCtrlSetLookAroundMode() {
    using enum CameraState;
    searchCamera.zoomScale = -1.0f;
    searchCamera.pitch = searchCamera.yaw = 0;
    searchCamera.fov = 10922;
    searchCamera.zoomFov = 5461;
    if (!g_cameraControl || g_cameraControl->state != LockedOn)
        return false;
    camCtrlSetState(LookAround);
    camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
    searchCamera.fovStyle = searchCamera.phase = 0;
    searchCamera.lookBase = {};
    searchCamera.rotationY = 0;
    searchCamera.lockedOnAngle = 0.0f;
    searchCamera.lookTarget = {};
    return true;
}
MapCellOffset GetFieldDepthCell(float x, float z) {
    x *= fieldCellScale;
    z *= fieldCellScale;
    if (x < 0.0f)
        x -= 1.0f;
    if (z < 0.0f)
        z -= 1.0f;
    return {static_cast<std::int16_t>(static_cast<std::int32_t>(x)), static_cast<std::int16_t>(static_cast<std::int32_t>(z))};
}

std::span<const CameraFieldDepthSample, 257> GetWorldFieldDepthSamples() {
    return worldFieldDepth;
}

std::span<const CameraFieldDepthSample, 257> GetCameraFieldDepthSamples() {
    return cameraFieldDepth;
}

std::uint32_t GetFieldDepthPointCount() {
    return fieldDepthPointCount;
}

const Matrix4x4 &GetFieldDepthTransform() {
    return fieldDepthTransform;
}

Matrix4x4 &GetFieldDepthModelTransform() {
    return fieldDepthModelTransform;
}
// @verified - p
bool ScreenVisCheck(const Vector3 &centre, float scale, float clipRange) {
    Vector3 point;
    Matrix4x4::TransformPoint(centre, point);
    const float depth = point.z * -1.0f;
    const float back = depth + scale;
    if (back < 0.0f || depth - scale > clipRange)
        return false;
    const float horizontal = back * fieldDepthScale.horizontalTangent + scale;
    if (point.x > horizontal || -horizontal >= point.x)
        return false;
    const float vertical = back * fieldDepthScale.verticalTangent + scale;
    if (point.y > vertical)
        return false;
    return !(point.y <= -vertical);
}

std::span<const MapCellOffset> camCtrlGetMapVisibleCells(std::uint32_t rotationY, std::int32_t tier) {
    const std::uint32_t direction = ((static_cast<std::uint32_t>(fieldInverseYaw) - rotationY) >> 12U) & 15U;
    return GetMapVisibleCellOffsets(direction, static_cast<std::uint32_t>(tier));
}

// The visible-cell tier for a view distance, in map cells.
std::int32_t camCtrlGetDrawTier(float distance) {
    const std::int32_t tier = static_cast<std::int32_t>(fieldCellScale * distance);
    return tier < 0 ? 0 : tier >= 11 ? 10 : tier;
}

float camCtrlGetCurrentFarDistance() {
    return fieldDepthScale.farDistance;
}

void camCtrlSetupDepthScaling() {
    // camCtrlSetupDepthScaling -> shenSetDepthScale.
    fieldCellScale = GetMapCellScale();
    fieldInverseYaw = -camCtrlGetNextRotY();
    const float farDistance = camCtrlGetFarDistance();
    const float nearDistance = camCtrlGetNearDistance();
    const float halfVerticalFovTangent = camCtrlGetHalfVerticalFovTangent();
    const float pitchAngleTangent = camCtrlGetPitchAngleTangent();
    const float npcRadius = camCtrlGetNpcRadius();
    fieldDepthScale = {farDistance, nearDistance, halfVerticalFovTangent, pitchAngleTangent, npcRadius, halfVerticalFovTangent, pitchAngleTangent};
}

bool camCtrlIsInsideDepthScaledRect(const Vector3 &centre, float radius) {
    // IsInsideDepthScaledRect excludes the negative boundary.
    const Vector3 point = TransformFieldDepthPoint(centre);
    const float back = -point.z + radius;
    if (back < 0.0f)
        return false;
    const float horizontal = back * fieldDepthScale.savedHorizontalTangent + radius;
    if (point.x > horizontal || -horizontal >= point.x)
        return false;
    const float vertical = back * fieldDepthScale.savedVerticalTangent + radius;
    return point.y <= vertical && -vertical < point.y;
}

bool camCtrlIsInsideHorizontalDepthRect(const Vector3 &centre, float radius) {

    const Vector3 point = TransformFieldDepthPoint(centre);
    const float back = -point.z + radius;
    if (!(back >= 0.0f))
        return false;
    const float horizontal = back * fieldDepthScale.savedHorizontalTangent + radius;
    return point.x <= horizontal && -horizontal < point.x;
}

// @verified - p
bool camCtrlIsSphereVisible(const Vector3 &centre, float radius) {
    // SphereWithinFrustumViewSpace includes the far-plane test.
    const Vector3 point = TransformFieldDepthPoint(centre);
    const float back = -point.z + radius;
    if (back < 0.0f || -point.z - radius > fieldDepthScale.farDistance)
        return false;
    const float horizontal = back * fieldDepthScale.horizontalTangent + radius;
    if (point.x > horizontal || -horizontal >= point.x)
        return false;
    const float vertical = back * fieldDepthScale.verticalTangent + radius;
    return point.y <= vertical && -vertical < point.y;
}
// @verified - p
bool camCtrlIsNearSphereVisible(const Vector3 &centre, float radius) {
    const Vector3 point = TransformFieldDepthPoint(centre);
    const float back = -point.z + radius;
    if (back < 0.0f || -point.z - radius > fieldDepthScale.nearDistance)
        return false;
    const float horizontal = back * fieldDepthScale.horizontalTangent + radius;
    if (point.x > horizontal || -horizontal >= point.x)
        return false;
    const float vertical = back * fieldDepthScale.verticalTangent + radius;
    return point.y <= vertical && -vertical < point.y;
}

// @verified - p
bool camCtrlIsFarSphereVisible(const Vector3 &centre, float radius) {
    const Vector3 point = TransformFieldDepthPoint(centre);
    if (-point.z - radius > fieldDepthScale.farDistance)
        return false;
    const float back = -point.z + radius;
    if (fieldDepthScale.nearDistance > back)
        return false;
    const float horizontal = back * fieldDepthScale.horizontalTangent + radius;
    if (point.x > horizontal || -horizontal >= point.x)
        return false;
    const float vertical = back * fieldDepthScale.verticalTangent + radius;
    return point.y <= vertical && -vertical < point.y;
}

// @verified - p
bool camCtrlIsScreenSpaceVisible(const Vector3 &centre, float radius) {
    const Vector3 point = TransformFieldDepthPoint(centre);
    const float back = -point.z + radius;
    if (back < 0.0f)
        return false;
    const float horizontal = back * fieldDepthScale.horizontalTangent + radius;
    if (point.x > horizontal || -horizontal >= point.x)
        return false;
    const float vertical = back * fieldDepthScale.verticalTangent + radius;
    return point.y <= vertical && -vertical < point.y;
}

// @verified - p
bool camCtrlCheckPosition(const Vector3 &position, float radius) {
    // camCtrlCheckPos/camCtrlCheckPosition use the WALC-augmented camera buffer,
    // not the primary world-model depth buffer or the ambient node matrix.
    Matrix4x4_Push(&fieldDepthTransform);
    const Vector3 point = TransformFieldDepthPoint(position);
    Matrix4x4::Pop(1);
    const float depth = -point.z;
    if (depth <= 0.0f)
        return true;
    const float threshold = depth - radius * 0.5f;
    const std::int32_t first = std::clamp(static_cast<std::int32_t>(((point.x - radius) / depth) * 128.0f), -128, 128) + 128;
    if (cameraFieldDepth[first].depth >= threshold)
        return true;
    const std::int32_t last = std::clamp(static_cast<std::int32_t>(((point.x + radius) / depth) * 128.0f), -128, 128) + 128;
    if (cameraFieldDepth[last].depth >= threshold)
        return true;
    for (std::int32_t sample = last - 1; sample >= first; --sample) {
        if (cameraFieldDepth[sample].depth >= threshold)
            return true;
    }
    return false;
}

void camCtrlSetProjectedShadowDepth(float distance) {
    projectedShadowFarDistance = distance;
    if (projectedShadowFarDistance > fieldDepthScale.farDistance)
        projectedShadowFarDistance = fieldDepthScale.farDistance;
}

bool camCtrlIsProjectedShadowVisible(const Vector3 &centre, float radius) {
    // SphereWithinProjectedShadowFrustum deliberately has no
    // vertical frustum test. It uses the current node TRS, not the yaw owner
    // used by the subsequent field-depth test.
    const Vector3 point = TransformFieldDepthPoint(centre);
    const float back = -point.z + radius;
    if (back < 0.0f || -point.z - radius > projectedShadowFarDistance)
        return false;
    const float horizontal = back * fieldDepthScale.horizontalTangent + radius;
    return point.x <= horizontal && -horizontal < point.x;
}

void camCtrlSubmitFieldDepth(const CameraRuntime &camera) {
    const std::uint16_t cameraYaw = static_cast<std::uint16_t>(-camCtrlGetNextRotY());
    // camCtrlBuildYawDepthTransform publishes a reset-basis, yaw-only view transform.
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(cameraYaw);
    Matrix4x4::preTranslate(-camera.nextPosition.x, -camera.nextPosition.y, -camera.nextPosition.z);
    Matrix4x4::GetTransformMatrix(&fieldDepthTransform);
    Matrix4x4::Pop(1);
    const float farDistance = camCtrlGetFarDistance();
    const std::uint16_t inverseYaw = static_cast<std::uint16_t>(-camCtrlGetNextRotY());

    // camCtrlSubmitMatrices. The buffers are process-owned;
    // every publication starts a new id/depth generation.
    fieldDepthPointCount = 0;
    worldFieldDepth.fill({0x10000U, 20000.0f});
    const MapCellOffset cameraCell = GetFieldDepthCell(camera.nextPosition.x, camera.nextPosition.z);
    const std::int32_t tier = camCtrlGetDrawTier(farDistance);
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const bool cellRouted = control && (control->statusFlags & 1U);
    const auto placements = GetMapModelSlots();
    const auto fields = GetMapFieldDepthSlots();
    Matrix4x4_Push(&fieldDepthTransform);
    for (std::size_t index = 0; index < fields.size(); ++index) {
        const MapFieldDepthSlot &field = fields[index];
        if (!field.enabled)
            continue;
        const MapModelRuntimeSlot &slot = placements[index];
        const Vector3 &position = slot.position;
        const bool translated = position.x != 0.0f || position.y != 0.0f || position.z != 0.0f;
        if (field.dangerSpheres) {
            Matrix4x4_Push();
            if (cellRouted) {
                MapCellOffset cell = cameraCell;



                if (translated || slot.rotationY == 0) {
                    const float x = camera.nextPosition.x - position.x;
                    const float z = camera.nextPosition.z - position.z;
                    const auto yaw = static_cast<std::uint16_t>(slot.rotationY);
                    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
                    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(yaw)));
                    cell = GetFieldDepthCell(sine * z + cosine * x, cosine * z - sine * x);
                    Matrix4x4::preTranslate(position.x, position.y, position.z);
                    Matrix4x4::RotateAroundY_Wrapper(yaw);
                }
                const std::uint32_t direction = (static_cast<std::uint32_t>(inverseYaw - static_cast<std::int32_t>(slot.rotationY)) >> 12U) & 15U;
                const auto offsets = GetMapVisibleCellOffsets(direction, static_cast<std::uint32_t>(tier));
                for (std::int32_t runIndex = 0; runIndex < field.dangerCellCount; ++runIndex) {
                    const MapModelCommandCell &run = field.dangerCells[runIndex];

                    const MapCellOffset delta{static_cast<std::int16_t>(run.x - cell.x), static_cast<std::int16_t>(run.z - cell.z)};
                    for (const MapCellOffset offset : offsets) {
                        if (delta.x != offset.x || delta.z != offset.z)
                            continue;
                        for (std::int32_t sphere = 0; sphere < run.count; ++sphere) {
                            PublishFieldDepthSphere(field.dangerSpheres[run.firstRecord + sphere]);
                        }
                        break;
                    }
                }
            } else {
                if (translated || slot.rotationY != 0) {
                    Matrix4x4::preTranslate(position.x, position.y, position.z);
                    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(slot.rotationY));
                }
                for (std::int32_t sphere = 0; sphere < field.dangerCount; ++sphere) {
                    PublishFieldDepthSphere(field.dangerSpheres[sphere]);
                }
            }
            Matrix4x4::Pop(1);
        }
        if (field.walls) {
            Matrix4x4_Push();
            if (translated || slot.rotationY != 0) {
                Matrix4x4::preTranslate(position.x, 0.0f, position.z);
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(slot.rotationY));
            }
            for (std::int32_t wall = 0; wall < field.wallCount; ++wall) {
                PublishFieldDepthWall(field.walls[wall], false);
            }
            Matrix4x4::Pop(1);
        }
    }
    // WALC must not alter the primary buffer used by model visibility.
    cameraFieldDepth = worldFieldDepth;
    for (std::size_t index = 0; index < fields.size(); ++index) {
        const MapFieldDepthSlot &field = fields[index];
        if (!field.enabled || !field.cameraWalls)
            continue;
        const MapModelRuntimeSlot &slot = placements[index];
        Matrix4x4_Push();
        if (slot.position.x != 0.0f || slot.position.y != 0.0f || slot.position.z != 0.0f || slot.rotationY != 0) {
            Matrix4x4::preTranslate(slot.position.x, 0.0f, slot.position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(slot.rotationY));
        }
        for (std::int32_t wall = 0; wall < field.cameraWallCount; ++wall) {
            PublishFieldDepthWall(field.cameraWalls[wall], true);
        }
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
}

bool camCtrlIsModelDepthVisible(const Vector3 &centre, const MT7Node &node, float radius) {
    // camCtrlIsModelDepthVisible uses OBJ's published yaw-view owner plus node TR, not
    // scale. Direct traversal publishes the base view; routed traversal
    // must publish its placement-composed yaw view before calling this.
    Matrix4x4_Push(&fieldDepthModelTransform);
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z != 0.0f) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.z)));
    }
    if (node.rotation.y != 0.0f) {
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.y)));
    }
    if (node.rotation.x != 0.0f) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(node.rotation.x)));
    }
    const Vector3 point = TransformFieldDepthPoint(centre);
    Matrix4x4::Pop(1);
    const float depth = -point.z;
    if (depth <= 0.0f)
        return true;
    const float inflatedRadius = radius * 1.1f;
    const float nearest = depth - inflatedRadius;
    const std::int32_t first = std::clamp(static_cast<std::int32_t>(((point.x - inflatedRadius) / depth) * 128.0f), -128, 128) + 128;
    if (!(worldFieldDepth[first].depth < nearest))
        return true;
    const std::int32_t last = std::clamp(static_cast<std::int32_t>(((point.x + inflatedRadius) / depth) * 128.0f), -128, 128) + 128;
    if (!(worldFieldDepth[last].depth < nearest))
        return true;
    for (std::int32_t sample = last - 1; sample > first; --sample) {
        if (worldFieldDepth[sample].depth >= nearest)
            return true;
    }
    return false;
}

// @verified - p
float GenerateOscillationForCamera(std::int16_t phase) {
    const float low = sin_fixed16(static_cast<std::int16_t>(22 * phase)) * 0.30000001f;
    const float medium = sin_fixed16(static_cast<std::int16_t>(31 * phase)) * 0.37f + low;
    const float high = sin_fixed16(static_cast<std::int16_t>(103 * phase)) * 0.15000001f + medium;
    return sin_fixed16(static_cast<std::int16_t>(147 * phase)) * 0.18000001f + high;
}

// @verified - p
void camCtrlSmoothMotion(CameraRuntime &camera) {
    if (!(camera.flags & CameraSmoothMotion))
        return;
    const float magnitude = std::fmax(Vector3::CalcMagnitude(camera.motionVector), Vector3::CalcMagnitude(camera.adjustedMotionVector));
    std::int32_t minimumSpeed = 0, maximumSpeed = 0;
    float minimumScale = 0.0f, maximumScale = 0.0f;
    if (CAMERA_MIN_MOTION_SCALE_THRESHOLD <= magnitude) {
        if (magnitude >= 0.125f) {
            if (magnitude < 0.5f) {
                maximumScale = (-0.227f / (0.5f - 0.125f)) * (magnitude - 0.125f) + 0.227f;
                minimumScale = (-0.212f / (0.5f - 0.125f)) * (magnitude - 0.125f) + 0.212f;
                maximumSpeed = static_cast<std::int32_t>((-21.0f / (0.5f - 0.125f)) * (magnitude - 0.125f)) + 21;
                minimumSpeed = static_cast<std::int32_t>((-16.0f / (0.5f - 0.125f)) * (magnitude - 0.125f)) + 16;
            }
        } else {
            maximumScale =
                ((0.227f - CAMERA_DEFAULT_MAX_MOTION_SCALE) / (0.125f - CAMERA_MIN_MOTION_SCALE_THRESHOLD)) * (magnitude - CAMERA_MIN_MOTION_SCALE_THRESHOLD) +
                CAMERA_DEFAULT_MAX_MOTION_SCALE;
            minimumScale =
                ((0.212f - CAMERA_DEFAULT_MIN_MOTION_SCALE) / (0.125f - CAMERA_MIN_MOTION_SCALE_THRESHOLD)) * (magnitude - CAMERA_MIN_MOTION_SCALE_THRESHOLD) +
                CAMERA_DEFAULT_MIN_MOTION_SCALE;
            maximumSpeed =
                static_cast<std::int32_t>((13.0f / (0.125f - CAMERA_MIN_MOTION_SCALE_THRESHOLD)) * (magnitude - CAMERA_MIN_MOTION_SCALE_THRESHOLD)) + 8;
            minimumSpeed =
                static_cast<std::int32_t>((10.0f / (0.125f - CAMERA_MIN_MOTION_SCALE_THRESHOLD)) * (magnitude - CAMERA_MIN_MOTION_SCALE_THRESHOLD)) + 6;
        }
    } else {
        maximumSpeed = 8;
        minimumSpeed = 6;
        maximumScale = CAMERA_DEFAULT_MAX_MOTION_SCALE;
        minimumScale = CAMERA_DEFAULT_MIN_MOTION_SCALE;
    }
    if (CameraDistance(camera.nextPosition, camera.targetPosition) < 0.5f) {
        maximumSpeed = 6;
        minimumSpeed = 4;
        maximumScale = CAMERA_CLOSE_MAX_MOTION_SCALE;
        minimumScale = CAMERA_CLOSE_MIN_MOTION_SCALE;
    }
    const float maximumTarget = maximumScale * camera.motionScaleFactor;
    const float minimumTarget = minimumScale * camera.motionScaleFactor;
    const float previousMagnitude = std::fmax(Vector3::CalcMagnitude(camera.motionScale), Vector3::CalcMagnitude(camera.interpolationScale));
    if (std::fabs(static_cast<double>(previousMagnitude - magnitude)) < 0.5) {
        camera.interpolatedMaximumSpeed = static_cast<std::int16_t>(camera.interpolatedMaximumSpeed + ((maximumSpeed - camera.interpolatedMaximumSpeed) >> 1));
        camera.currentMinimumSpeed = static_cast<std::int16_t>(camera.currentMinimumSpeed + ((minimumSpeed - camera.currentMinimumSpeed) >> 1));
        camera.currentMaximumScale = (maximumTarget - camera.currentMaximumScale) * 0.5f + camera.currentMaximumScale;
        camera.cameraMinimumScale = (minimumTarget - camera.cameraMinimumScale) * 0.5f + camera.cameraMinimumScale;
    }
    camera.cumulativeMaximumSpeed += camera.interpolatedMaximumSpeed;
    camera.cumulativeMinimumSpeed += camera.currentMinimumSpeed;
    camera.oscillation.x = GenerateOscillationForCamera(static_cast<std::int16_t>(camera.cumulativeMaximumSpeed)) * camera.currentMaximumScale;
    camera.oscillation.y = GenerateOscillationForCamera(static_cast<std::int16_t>(camera.cumulativeMinimumSpeed)) * camera.cameraMinimumScale;
    camera.oscillation.z = 0.0f;
}

// @verified - p
void ApplyRotationMatrixFromTransform(CameraRuntime &camera) {
    if (!(camera.flags & CameraSmoothMotion))
        return;
    Vector3 offset{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.yaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.pitch));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.roll));
    Matrix4x4::TransformPoint(camera.oscillation, offset);
    Matrix4x4::Pop(1);
    camera.nextPosition.x = offset.x + camera.nextPosition.x;
    camera.nextPosition.z = offset.z + camera.nextPosition.z;
    camera.nextPosition.y = (offset.y + camera.yOffset) + camera.nextPosition.y;
}

// @verified - p
void camCtrlRecalcFov(CameraRuntime &camera) {
    if (!(camera.flags & 0x8000U))
        return;
    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-camera.nextRotation.roll));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-camera.nextRotation.pitch));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-camera.nextRotation.yaw));
    Matrix4x4::preTranslate(-camera.nextPosition.x, -camera.nextPosition.y, -camera.nextPosition.z);
    Matrix4x4::TransformPoint(camera.scale, local);
    Matrix4x4::Pop(1);
    float scale = camera.scaleFactor;
    if (camera.flags & 0x10000U) {
        camera.flags &= ~0x10000U;
        if (std::fabs(static_cast<double>(local.z)) < 0.001) {
            camera.flags &= ~0x8000U;
            return;
        }
        camCtrlCalculateFov(camera);
        scale = camera.viewHeightScale / local.z;
        camera.scaleFactor = scale;
    }
    camera.currentFovAngle = static_cast<std::uint16_t>(2 * FixedAtan2(static_cast<float>(d3t::g_originalScreenWidth) * 0.5f, local.z * scale));
}

// @verified - p
void camCtrlSetup(CameraRuntime &camera) {
    if (!camera.savedPresetIndex)
        return;
    CameraPreset &saved = g_cameraCachedPresets[0];
    saved.position = camera.nextPosition;
    saved.rotation = camera.nextRotation;
    saved.fovAngle = camera.currentFovAngle;
    saved.backwardsDirection = camera.backwardsDirection;
    saved.projectionMatrix = camera.projectionMatrix;
    saved.viewMatrix = camera.viewMatrix;
    const CameraPreset &selected = g_cameraCachedPresets[camera.savedPresetIndex];
    camera.nextPosition = selected.position;
    camera.nextRotation = selected.rotation;
    camera.currentFovAngle = selected.fovAngle;
    camCtrlCreatePerspectiveMatrix(camera);
    camera.nextPosition = saved.position;
    camera.nextRotation = saved.rotation;
    camera.currentFovAngle = saved.fovAngle;
    camera.backwardsDirection = saved.backwardsDirection;
    camera.projectionMatrix = saved.projectionMatrix;
    camera.viewMatrix = saved.viewMatrix;
}

void camCtrlCalculateFov(CameraRuntime &camera) {
    const float halfHorizontalTangent = std::tan(FixedAngleToRadians(static_cast<std::int16_t>(camera.currentFovAngle) / 2));


    const float width = static_cast<float>(d3t::g_originalScreenWidth);
    const float height = static_cast<float>(d3t::g_originalScreenHeight);
    const float outputAspect = height != 0.0f ? width / height : 16.0f / 9.0f;
    const float adjustment = (4.0f / 3.0f) / outputAspect;
    const std::int32_t halfVertical = RadiansToFixedAngle(std::atan(halfHorizontalTangent / adjustment));
    camera.fullVerticalFov = static_cast<std::uint16_t>(halfVertical * 2);
    camera.halfVerticalFovTangent = std::tan(FixedAngleToRadians(static_cast<std::int16_t>(camera.fullVerticalFov) / 2));
    camera.pitch = static_cast<std::int16_t>(2 * RadiansToFixedAngle(std::atan((1.0f / (width / height)) * camera.halfVerticalFovTangent)));
    camera.pitchAngleTangent = std::tan(FixedAngleToRadians(camera.pitch / 2));
    camera.viewHeightScale = (height * 0.5f) / camera.pitchAngleTangent;
}

void camCtrlCreatePerspectiveMatrix(CameraRuntime &camera) {
    SetupStack();
    SetActiveStack(MatrixStackType::Transparent);
    Matrix4x4::SetPrePassToTempTransform();
    camCtrlCalculateFov(camera);
    const float width = static_cast<float>(d3t::g_originalScreenWidth);
    const float height = static_cast<float>(d3t::g_originalScreenHeight);
    SetupProjection(camera.pitch, height != 0.0f ? width / height : 1.0f, nearClip, farClip);

    SetActiveStack(MatrixStackType::DepthPrepass);
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.yaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.pitch));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.roll));
    TransformBackwardDirection(camera.backwardsDirection);

    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-camera.nextRotation.roll));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-camera.nextRotation.pitch));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-camera.nextRotation.yaw));
    Matrix4x4::preTranslate(-camera.nextPosition.x, -camera.nextPosition.y, -camera.nextPosition.z);
    if (translationScaleFactor != 0.0f) {
        Matrix4x4::preTranslate(translationScaleFactor * camera.backwardsDirection.x, translationScaleFactor * camera.backwardsDirection.y,
            translationScaleFactor * camera.backwardsDirection.z);
    }
    Matrix4x4::GetTransformMatrix(&camera.projectionMatrix);

    Matrix4x4::SetPrePassToTempTransform();
    if (translationScaleFactor != 0.0f) {
        Matrix4x4::preTranslate(-translationScaleFactor * camera.backwardsDirection.x, -translationScaleFactor * camera.backwardsDirection.y,
            -translationScaleFactor * camera.backwardsDirection.z);
    }
    Matrix4x4::preTranslate(camera.nextPosition.x, camera.nextPosition.y, camera.nextPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.yaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.pitch));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(camera.nextRotation.roll));
    Matrix4x4::GetTransformMatrix(&camera.viewMatrix);

    SetActiveStack(MatrixStackType::Opaque);
    Matrix4x4::SetActiveTransform(&camera.projectionMatrix);
    SetActiveStack(MatrixStackType::DepthPrepass);
    Matrix4x4::SetActiveTransform(&camera.projectionMatrix);
    g_cameraProjectionMatrix = camera.projectionMatrix;
}

void camCtrlInitialize(CameraRuntime &camera) {
    using enum CameraState;
    std::memset(static_cast<void *>(&camera), 0, sizeof(camera));


    camera.orbitDistance = 3.0999999f;
    camera.state = Init;
    camera.previousState = Init;
    camera.stateField0e = -1;
    camera.stateField0f = -1;
    camera.targetFovAngle = 0x2aaa;
    camera.currentFovAngle = 0x2aaa;
    camera.fovAngle = 0x2aaa;
    camera.targetOrbitDistance = 3.0999999f;
    camera.interpolationMode = CameraInterpolationMode::None;
    camera.savedPresetIndex = 0;
    camera.field1f9 = 1;
    nearClip = 0.015f;
    farClip = 2200.0f;
    translationScaleFactor = 0.0f;
    // camCtrlInit writes smoothing defaults through the selected global
    // owner, even when its explicit initialization target is another camera.
    if (g_cameraControl) {
        g_cameraControl->flags &= ~CameraSmoothMotion;
        g_cameraControl->motionScaleFactor = 1.0f;
        g_cameraControl->yOffset = 0.0f;
    }
    camCtrlCalculateFov(camera);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::GetTransformMatrix(&camera.projectionMatrix);
    Matrix4x4::GetTransformMatrix(&camera.viewMatrix);
    Matrix4x4::GetTransformMatrix(&camera.finalTransform);
    g_cameraProjectionMatrix = camera.projectionMatrix;
    Matrix4x4::Pop(1);
    g_cameraCachedPresets[0].tag = g_cameraCachedPresets[1].tag = g_cameraCachedPresets[2].tag = 0;
}

HLib::Task *camCtrlEnqueue(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(camCtrlCallback, nextFunctionIndex, 7, 0, SHEN_NAME(CAMT));
    g_cameraControl = &cameraStorage;
    camCtrlInitialize(cameraStorage);
    cameraStorage.flags |=
        CameraDisableInterpolation | CameraRebuildPositionSpline | CameraRebuildRotationSpline | CameraRaycastEnabled | CameraAutomaticDistanceFov;
    cameraStorage.shakeAmplitude = {0.0f, 0.0f, 1.0f};
    InitializeConversationCameraState();
    camCtrlInitSplines();
    camCtrlSetDistanceFromPlayerDefaults();
    cameraStorage.flags |= CameraDisableFovInterpolation;
    cameraStorage.fovLerpIterations = 4;
    return task;
}

void camCtrlSetAuthoredView(const Vector3 &target, const Vector3 &eye, std::int16_t roll, std::uint16_t fieldOfView) {
    if (!g_cameraControl)
        return;
    g_cameraControl->previousPosition = eye;
    g_cameraControl->targetPosition = target;
    g_cameraControl->nextRotation.roll = roll;
    if (fieldOfView) {
        g_cameraControl->currentFovAngle = fieldOfView;
        g_cameraControl->fovAngle = fieldOfView;
    }
}

void camCtrlCacheTransform() {
    if (g_cameraControl)
        cachedCameraTransform = g_cameraControl->finalTransform;
}

void camCtrlRestoreCachedTransform() {
    if (g_cameraControl)
        g_cameraControl->finalTransform = cachedCameraTransform;
}

// @verified - p: 51f3b0 copies the complete 0x370-byte CAMT owner.
void camCtrlSaveRuntime() {
    std::memcpy(&savedCameraRuntime, g_cameraControl, sizeof(savedCameraRuntime));
}

// @verified - p: 51f2c0 restores the same owner, including borrowed callbacks.
void camCtrlRestoreRuntime() {
    std::memcpy(g_cameraControl, &savedCameraRuntime, sizeof(savedCameraRuntime));
}

void camCtrlSetTranslationScale(float factor) {
    translationScaleFactor = factor;
}

void camCtrlResetTranslationScale() {
    translationScaleFactor = 0.0f;
}

void camCtrlSetClipDistances(float nearDistance, float farDistance) {
    nearClip = nearDistance;
    farClip = farDistance;
}

void camCtrlResetClipDistances() {
    nearClip = 0.015f;
    farClip = 2200.0f;
}

// @verified - p
float camCtrlSetFarDistance(float distance) {
    const float previous = g_cameraControl->farDistance;
    g_cameraControl->farDistance = distance;
    return previous;
}

// @verified - p
void camCtrlSetNearDistance(float distance) {
    g_cameraControl->nearDistance = distance;
}

// @verified - p
void camCtrlSetNpcRadius(float radius) {
    g_cameraControl->npcRadius = radius;
}

// @verified - p
bool camCtrlToggleSmoothing(std::int32_t enabled) noexcept {
    CameraRuntime &camera = *g_cameraControl;
    const bool previous = (camera.flags & CameraSmoothMotion) != 0;
    if (enabled)
        camera.flags |= CameraSmoothMotion;
    else {
        camera.flags &= ~CameraSmoothMotion;
        camera.motionScaleFactor = 1.0f;
        camera.yOffset = 0.0f;
    }
    return previous;
}

// @verified - p
CameraRuntime *camCtrlSetMotionScaleFactor(float factor) noexcept {
    g_cameraControl->motionScaleFactor = factor;
    return g_cameraControl;
}

// @verified - p
CameraRuntime *camCtrlSetYOffset(float offset) noexcept {
    g_cameraControl->yOffset = offset;
    return g_cameraControl;
}

// @verified - p
void camCtrlResetMotionscaling() noexcept {
    g_cameraControl->cumulativeMaximumSpeed = 0;
    g_cameraControl->cumulativeMinimumSpeed = 0;
    g_cameraControl->motionScaleFactor = 1.0f;
    g_cameraControl->yOffset = 0.0f;
}


// @verified - p
void camCtrlSetScaleTo(const Vector3 *scale) noexcept {
    CameraRuntime &camera = *g_cameraControl;
    if (camera.flags & 0x8000U) {
        camera.scale = *scale;
        camera.flags |= 0x10000U;
    }
}

// @verified - p
std::uint64_t camCtrlSetScaleToAndRecalcFov(const Vector3 *scale) noexcept {
    CameraRuntime &camera = *g_cameraControl;
    camera.scale = *scale;
    camera.flags |= 0x18000U;
    std::uint32_t result;
    std::memcpy(&result, &scale->z, sizeof(result));
    return result;
}

// @verified - p
CameraRuntime *camCtrlSetFlags() noexcept {
    g_cameraControl->flags &= ~0x18000U;
    return g_cameraControl;
}

// @verified - p
void camCtrlSetSkipInterpTo(std::int32_t enabled) noexcept {
    if (enabled)
        g_cameraControl->flags |= CameraSkipInterpolationOnce;
    else
        g_cameraControl->flags &= ~CameraSkipInterpolationOnce;
}

// @verified - p
void camCtrlSetXform(const Matrix4x4 *transform) noexcept {
    if (!transform)
        return;
    g_cameraControl->finalTransform = *transform;
    g_cameraControl->flags |= 0x00400000U;
}


// @verified - p
void camCtrlSetRotation(const Vector3 *translation, const EulerRotation16 *rotation) noexcept {
    CameraRuntime &camera = *g_cameraControl;
    camera.rotationOffset = translation ? *translation : Vector3{};
    if (rotation) {
        camera.pitchOffset = rotation->pitch;
        camera.yawOffset = rotation->yaw;
        camera.rollOffset = rotation->roll;
    } else {
        camera.pitchOffset = camera.yawOffset = camera.rollOffset = 0;
    }
}

// @verified - p
void camCtrlGetRotationXform(Vector3 *translation, EulerRotation16 *rotation) noexcept {
    const CameraRuntime &camera = *g_cameraControl;
    if (translation)
        *translation = camera.rotationOffset;
    if (rotation)
        *rotation = {camera.pitchOffset, camera.yawOffset, camera.rollOffset};
}

// @verified - p
void camCtrlSetRotationCallback(void (*callback)()) noexcept {
    g_cameraControl->rotationCallback = callback;
}


// @verified - p
void camCtrlSetUpdateCallback(void (*callback)(CameraRuntime *)) noexcept {
    g_cameraControl->stateCallback = callback;
}

// @verified - p
void camCtrlSetTargetChara(shen_name name) noexcept {
    g_cameraControl->targetCharacter = name;
}

// @verified - p
std::uint64_t camCtrlSetNextCamRot(const EulerRotation16 *rotation) noexcept {
    g_cameraControl->nextRotation = *rotation;
    return static_cast<std::uint16_t>(rotation->roll);
}


// @verified - p
void camCtrlSetNextRoll(std::int16_t roll) noexcept {
    g_cameraControl->nextRotation.roll = roll;
}

// @verified - p
void camCtrlSetDistanceFromPlayerDefaults() noexcept {
    CameraRuntime &camera = *g_cameraControl;
    camera.targetHeightDistance = 1.2f;
    camera.defaultOrbitDistance = 3.0999999f;
    camera.basePitchBias = -256;
    camera.contextMode = 0;
    camera.collisionPhase = 2;
    camera.interpolationTicks = 0;
    camera.targetCharacter = {};
}

// @verified - p
std::uint64_t camCtrlSetConversationPosition(const Vector3 *position) noexcept {
    if (position) {
        conversationCamera.positionOverride = *position;
        conversationCamera.flags |= 0x400U;
    } else
        conversationCamera.flags &= ~0x400U;
    return 0;
}

// SCEN's look-at-location request raises it and action 13 clears it.
// @verified - p
std::int64_t camCtrlSetConversationFlag4000(std::int32_t enabled) noexcept {
    if (enabled)
        conversationCamera.flags |= 0x4000U;
    else
        conversationCamera.flags &= ~0x4000U;
    return 0;
}

// @verified - p
bool camCtrlHasConversationFlaggedView() noexcept {
    return (conversationCamera.flags & 2U) != 0 && (conversationCamera.flags & 0x08000000U) != 0;
}

// @verified - p
std::uint32_t camCtrlIsConversationActive() noexcept {
    return (conversationCamera.flags >> 1U) & 1U;
}

// @verified - p
std::uint64_t camCtrlUtil(CameraUtilityMode mode, std::intptr_t parameter) {
    CameraRuntime &camera = *g_cameraControl;
    switch (mode) {
    case CameraUtilityMode::RestorePrevious:
        camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
        break;
    case CameraUtilityMode::SetEaseFactor: {
        float value;
        std::memcpy(&value, reinterpret_cast<const void *>(parameter), sizeof(value));
        camera.interpolationEaseFactor = value < 0.0f ? 0.1f : value;
        break;
    }
    case CameraUtilityMode::SetTargetHeight:
        std::memcpy(&camera.targetHeightDistance, reinterpret_cast<const void *>(parameter), sizeof(float));
        break;
    case CameraUtilityMode::SetOrbitDistance:
        std::memcpy(&camera.defaultOrbitDistance, reinterpret_cast<const void *>(parameter), sizeof(float));
        break;
    case CameraUtilityMode::SetPitchBias:
        camera.basePitchBias = static_cast<std::int16_t>(parameter);
        break;
    case CameraUtilityMode::ResetPlayerDistance:
        camCtrlSetDistanceFromPlayerDefaults();
        break;
    case CameraUtilityMode::GetContext:
        return static_cast<std::uint32_t>(camCtrlGetContextMode());
    case CameraUtilityMode::ForceSearchCam: {
        const CameraUtilityParameters *const values = reinterpret_cast<const CameraUtilityParameters *>(parameter);
        return static_cast<std::uint32_t>(camCtrlSetContext(static_cast<std::int32_t>(values->first), static_cast<std::int32_t>(values->second)));
    }
    case CameraUtilityMode::PreviousToCurrent:
        camCtrlResetTransition(CameraInterpolationMode::PreviousToCurrent);
        break;
    case CameraUtilityMode::SetTransitionSteps:
        camera.transitionStepCount = static_cast<std::int16_t>(parameter <= 0 ? 1 : parameter);
        break;
    case CameraUtilityMode::EnableInterpolationSnap:
        camera.flags |= CameraInterpolationSnap;
        break;
    case CameraUtilityMode::SetConversationPosition:
        return camCtrlSetConversationPosition(reinterpret_cast<const Vector3 *>(parameter));
    case CameraUtilityMode::HasConversationFlaggedView:
        return camCtrlHasConversationFlaggedView();
    default:
        break;
    }
    return 0;
}

std::int32_t camCtrlGetContextMode() {
    return g_cameraControl->contextMode;
}

std::int32_t camCtrlGetContextVal() {
    if ((g_cameraControl->flags & CameraAllowContextSwitch) == 0)
        return 0;
    return g_cameraControl->contextMode == 10 ? 2 : 1;
}

// @verified - p
void camCtrlGetNextCamPos(Vector3 *position) {
    if (position)
        *position = g_cameraControl->nextPosition;
}

// @verified - p
CameraRuntime *camCtrlSetFovTransitionTo(bool enabled) {
    if (enabled) {
        g_cameraControl->flags |= CameraDisableFovInterpolation;
        g_cameraControl->fovLerpIterations = 4;
    } else {
        g_cameraControl->flags &= ~CameraDisableFovInterpolation;
        g_cameraControl->fovLerpIterations = 0;
    }
    return g_cameraControl;
}

bool camCtrlAcquireSequenceLease(CameraSequenceLease &lease) {
    if (!g_cameraControl || lease.active)
        return false;
    lease = {};
    lease.saved = *g_cameraControl;
    lease.active = true;
    return true;
}

bool camCtrlSetSequenceView(CameraSequenceLease &lease, std::int32_t cameraId, shen_name eyeAttachment, shen_name targetAttachment, const Vector3 &eye,
    const Vector3 &target, std::int16_t roll, std::uint16_t fieldOfView, std::uint16_t transitionFrames, bool cut) {
    using enum CameraState;
    if (!lease.active || !g_cameraControl)
        return false;

    CameraRuntime &camera = *g_cameraControl;
    lease.authoredEye = eye;
    lease.authoredTarget = target;
    lease.eyeAttachment = eyeAttachment;
    lease.targetAttachment = targetAttachment;
    lease.authoredRoll = roll;
    lease.authoredFov = fieldOfView ? fieldOfView : camera.currentFovAngle;
    const bool transitioning = !cut && transitionFrames != 0;
    camCtrlResetTransition(transitioning ? CameraInterpolationMode::PreviousToCurrent : CameraInterpolationMode::None);
    camera.transitionStepCount = static_cast<std::int16_t>(transitioning ? transitionFrames : 0);

    camera.previousState = camera.state;
    camera.state = Scripted;
    camera.flags = (camera.flags & ~CameraAutomaticDistanceFov) | CameraRebuildPositionSpline | CameraRebuildRotationSpline;
    camera.cameraId.name = static_cast<std::uint32_t>(cameraId);
    camera.fieldC4 = 0;
    Character *const eyeCharacter = FindCharacterByScenarioName(eyeAttachment);
    Character *const targetCharacter = FindCharacterByScenarioName(targetAttachment);
    camera.splineBoundary = reinterpret_cast<std::intptr_t>(eyeCharacter);
    camera.splineCoefficients = reinterpret_cast<std::intptr_t>(targetCharacter);
    camera.showFilmBars = 0;
    camCtrlTickSequenceView(lease);
    return true;
}

bool camCtrlTickSequenceView(CameraSequenceLease &lease) {
    if (!lease.active || !g_cameraControl)
        return false;
    Vector3 eye = lease.authoredEye;
    Vector3 target = lease.authoredTarget;
    if (Character *const attached = FindCharacterByScenarioName(lease.eyeAttachment)) {
        eye.x += attached->entity.node.position.x;
        eye.y += attached->entity.node.position.y;
        eye.z += attached->entity.node.position.z;
    }
    if (Character *const attached = FindCharacterByScenarioName(lease.targetAttachment)) {
        target.x += attached->entity.node.position.x;
        target.y += attached->entity.node.position.y;
        target.z += attached->entity.node.position.z;
    }

    camCtrlSetAuthoredView(target, eye, lease.authoredRoll, lease.authoredFov);


    return g_cameraControl->interpolationMode == CameraInterpolationMode::PreviousToCurrent;
}

void camCtrlReleaseSequenceLease(CameraSequenceLease &lease) {
    if (!lease.active)
        return;
    if (g_cameraControl) {
        *g_cameraControl = lease.saved;
    }
    lease = {};
}

} // namespace shenmue::s2
