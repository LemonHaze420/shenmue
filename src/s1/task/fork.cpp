#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "s1/task/fork.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/boot.hpp"

#include "s1/collision.hpp"
#include "s1/input.hpp"
#include "s1/initialization.hpp"
#include "s1/player_control.hpp"
#include "s1/mt5_types.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/motion.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/scen.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/sond.hpp"

#include <array>
#include <bit>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace shenmue::s1 {

std::int32_t ExecuteAICACommand_Wrapper(std::uint32_t volume);
void ReleaseEnvironmentAicaBank(std::uint32_t bank);
std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);
std::int32_t COLS_GetSelectedCollisionRecordValue();
extern std::int32_t g_debugForceShowAllItems;
extern std::array<std::uint32_t, 3> g_uiRegisteredCallbackRequests;
void HLib_InertTaskCallback(HLib::TaskData *taskData);

std::uint8_t g_flmwPlayerDriveActive = 0;
std::uint8_t g_flmwForwardInputPressed = 0;
std::int16_t g_ryoVehicleAlphaSteps = 0;

FollowerLoadFogBlock g_followerLoadFogBlock{};

namespace {

constexpr std::uint32_t kAkira = SHEN_NAME(AKIR);
constexpr std::uint32_t kPlayerForklift = SHEN_NAME(FRK0);
constexpr std::uint32_t kRaceForklifts[5]{SHEN_NAME(FRK0), SHEN_NAME(FRK1), SHEN_NAME(FRK2), SHEN_NAME(FRK3), SHEN_NAME(FRK4)};
constexpr std::uint32_t kHelperLeft = SHEN_NAME(HNDL);
constexpr std::uint32_t kHelperRight = SHEN_NAME(HNDR);
constexpr std::uint32_t kFollowerLoadIdsToken = SHEN_NAME(FOKN);
constexpr std::uint32_t kScriptPending = 0xffffffffU;
constexpr std::uint32_t kNoFollowerId = 0xffffffffU;
constexpr std::uint32_t kEnvSetVolume = 0x10a5U;
constexpr std::uint32_t kEnvReleaseVolume = 0xa6U;
constexpr std::uint16_t kButtonB = 0x0200;
constexpr std::uint16_t kButtonA = 0x0400;
constexpr std::uint16_t kButtonUp = 0x1000;
constexpr std::uint16_t kButtonLeft = 0x4000;
constexpr std::uint16_t kButtonRight = 0x8000;
constexpr std::uint8_t kHelpTokenForklift = 0x17;
shen_name *const kNoFollowerList = reinterpret_cast<shen_name *>(UINTPTR_MAX);


const ForkliftDriveProfile g_defaultForkliftProfile{.maxSteerFixed16 = 7281,
    .steerStepFixed16 = 218,
    .steerCenterStep = 364,
    .throttleAccelStep = 0.005f,
    .maxForwardSpeed = 0.1f,
    .maxReverseSpeed = -0.08f,
    .brakeStepTightClearance = -0.03f,
    .coastDecayStep = -0.008f,
    .wheelDiameterA = 0.4884f,
    .wheelDiameterB = 0.39072f,
    .effectiveWheelbase = 1.28f};
const ForkliftDriveProfile g_playerForkliftProfile{.maxSteerFixed16 = 6553,
    .steerStepFixed16 = 291,
    .steerCenterStep = 163,
    .throttleAccelStep = 0.0049f,
    .maxForwardSpeed = 0.3055f,
    .maxReverseSpeed = -0.13f,
    .brakeStepTightClearance = -0.009f,
    .coastDecayStep = -0.0045f,
    .wheelDiameterA = 0.4884f,
    .wheelDiameterB = 0.39072f,
    .effectiveWheelbase = 1.28f};
constexpr ForkliftRegion g_forkliftRaceRegions[9]{{.minX = -69.6f, .minZ = 101.7f, .maxX = -64.6f, .maxZ = 110.7f},
    {.minX = -120.0f, .minZ = 90.0f, .maxX = -100.0f, .maxZ = 100.0f}, {.minX = -80.0f, .minZ = 40.0f, .maxX = -70.0f, .maxZ = 50.0f},
    {.minX = -20.0f, .minZ = 70.0f, .maxX = -10.0f, .maxZ = 80.0f}, {.minX = 30.0f, .minZ = 80.0f, .maxX = 40.0f, .maxZ = 90.0f},
    {.minX = 50.0f, .minZ = 110.0f, .maxX = 70.0f, .maxZ = 120.0f}, {.minX = 10.0f, .minZ = 150.0f, .maxX = 20.0f, .maxZ = 170.0f},
    {.minX = 0.0f, .minZ = 110.0f, .maxX = 10.0f, .maxZ = 120.0f}, {.minX = -64.6f, .minZ = 101.7f, .maxX = -59.6f, .maxZ = 110.7f}};

std::uint8_t g_flmwPlayerForkProximityFrames = 0;
std::uint8_t g_flmwSpacingAssistActive = 0;
std::int16_t g_flmwGlobalAicaBankId = 0;
std::uint8_t g_flmwNoLoadFootprintMathFnMask[0x20]{};
std::int32_t g_flmwRollbackYawSumFixed16 = 0;
std::int32_t g_flmwRollbackActive = 0;
std::uint32_t g_flmwRollbackExpiryFrame = 0;
Vector3 g_flmwRollbackPos{};
Vector3 g_flmwSavedForkliftPosition{};
Vector3 g_flmwSavedLoadPosition{};
shen_name g_flmwSavedLoadId{};
std::int32_t g_flmwSavedForkliftYaw = 0;
std::int32_t g_flmwSavedLoadYaw = 0;
std::int16_t g_flmwSavedLoadSequencePhase = 0;

void SetResult(ScenarioContext *context, std::uint32_t result) {
    context->stack->accumulator = result;
}

Character *FindGlobalCharacter(std::uint32_t id) {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr);
    CharacterIdLookupNode *node = scenario->characterIdRoot;
    while (node) {
        if (node->id == id)
            return node->character;
        node = static_cast<std::int32_t>(id) > static_cast<std::int32_t>(node->id) ? node->left : node->right;
    }
    return nullptr;
}

template <typename T> T *ScenarioPointer(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(g_pCurrentTaskData + offset) : nullptr;
}


std::int16_t SumEntityYaw(Character *character) {
    std::int16_t yaw = 0;
    if (character) {
        for (CharacterNode *node = &character->entity.node; node; node = node->child)
            yaw = static_cast<std::int16_t>(yaw + static_cast<std::int16_t>(node->rotation.y));
    }
    return yaw;
}

std::int16_t YawRelativeToParents(Character *character, std::int16_t worldYaw) {
    for (CharacterNode *node = character->entity.node.child; node; node = node->child)
        worldYaw = static_cast<std::int16_t>(worldYaw - static_cast<std::int16_t>(node->rotation.y));
    return worldYaw;
}


void SetWorldPositionAndYaw(Character *character, const Vector3 &position, std::int16_t worldYaw) {
    shenApplyTranslationToAll(character->entity.node, position);
    character->entity.node.rotation.y = YawRelativeToParents(character, worldYaw);
}

void HideCharacterMotion(Character *character) {
    if (!character)
        return;
    const bool wasVisible = (character->motionFlags & 0x3fU) != 0;
    character->motionFlags &= 0xc0U;
    const bool isVisible = (character->motionFlags & 0x3fU) != 0;
    if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && isVisible != wasVisible)
        CHAR_LogNPCVisibility(character, isVisible);
}

float FrameScaledStep(float step) {
    if (g_updateRenderFrameMode > 0)
        return static_cast<float>(static_cast<double>(step) * 1.1875);
    if (g_updateRenderFrameMode == 0)
        return static_cast<float>(static_cast<double>(step) * 0.0625);
    return step;
}


// @verified - p
void flmwProcessShadowCallback(Character *character) {
    if (!character)
        return;
    ForkliftWork *const fork = FLMW_GetWork(character);
    if (!fork->shadowCallback)
        return;
    shenGetNodeByChildIdx(static_cast<Mt5RuntimeNode *>(character->shadowModel), 3)->position.y = fork->forkHeight;
    fork->shadowCallback(character);
}


// @verified - p
void flmwSetMotionDeltaFromThrottleAndSteer(ScenarioContext *context, Character *character, float driveSpeed, std::int32_t steerAngle) {
    const float step = FrameScaledStep(driveSpeed);
    ForkliftWork *const fork = FLMW_GetWork(character);
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    fork->throttleDelta = step;
    std::int32_t yaw = character ? character->entity.node.rotation.y : 0;
    fork->sourcePositionX = position.x;
    fork->sourcePositionZ = position.z;
    fork->sourceYawFixed16 = yaw;
    if (steerAngle) {
        const float turnAngle = static_cast<float>(static_cast<std::uint16_t>(steerAngle)) * 6.2831855f * 1.52587890625e-05f;
        const float radius = fork->tuning->effectiveWheelbase / std::tan(turnAngle);
        const std::int32_t turn = atan2_fixed16(step, static_cast<float>(std::fabs(static_cast<double>(radius))));
        if (steerAngle < 0)
            yaw += turn;
        else
            yaw -= turn;
    }
    fork->moveDeltaX = -(sin_fixed16(static_cast<std::int16_t>(yaw)) * step);
    fork->moveDeltaZ = -(cos_fixed16(static_cast<std::uint16_t>(yaw)) * step);
    bool playerForklift = false;
    if (fork->interactionProfile)
        playerForklift = SCEN_FindCharacterByFunctionId(context, kPlayerForklift) == character;
    if (g_flmwRollbackActive && (!fork->interactionProfile || playerForklift)) {
        fork->moveDeltaX = fork->moveDeltaX * -0.5f;
        fork->moveDeltaZ = fork->moveDeltaZ * -0.5f;
    }
    fork->moveYawFixed16 = static_cast<std::int16_t>(yaw);
}

// @verified - p
void flmwApplyMotionDeltaToActor(Character *character, const ForkliftWork *fork) {
    if (!character)
        return;
    character->entity.node.rotation.y = fork->moveYawFixed16;
    character->entity.node.position.z += fork->moveDeltaZ;
    character->entity.node.position.x += fork->moveDeltaX;
}


// @verified - p
void flmwApplyOwnerPosRot(Character *character, ForkliftWork *fork) {
    Character *const owner = fork->visualOwner;
    const Vector3 position = owner ? GetPositionForEntity(owner->entity.node) : Vector3{};
    owner->entity.node.child = character ? &character->entity.node : nullptr;
    if (fork->visualOwner)
        shenApplyTranslationToAll(fork->visualOwner->entity.node, position);
    if (fork->visualOwner)
        fork->visualOwner->entity.node.rotation.y = 0;
    const std::int16_t forkliftYaw = SumEntityYaw(character);
    if (Character *const visualOwner = fork->visualOwner)
        visualOwner->entity.node.rotation.y = YawRelativeToParents(visualOwner, forkliftYaw);
}


// @verified - p
void flmwDetachChildChainPreserveWorldYaw(Character *character) {
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    const std::int16_t worldYaw = SumEntityYaw(character);
    character->entity.node.child = nullptr;
    SetWorldPositionAndYaw(character, position, worldYaw);
}

// @verified - p
void flmwRaiseForkHeightTowardTarget(ForkliftWork *fork, float targetHeight) {
    const float height = fork->forkHeight;
    if (!(targetHeight > height)) {
        fork->forkHeight = targetHeight;
        fork->forkHeightLerpDone = -1;
        return;
    }
    const float next = height + FrameScaledStep(fork->forkHeightStepRate);
    if (fork->hasAttachedLoad == 1 && fork->carriedLoad)
        fork->carriedLoad->entity.node.position.y = next;
    fork->forkHeight = next;
}

// @verified - p
void flmwLowerForkHeightTowardTarget(ForkliftWork *fork, float targetHeight) {
    const float height = fork->forkHeight;
    if (!(height > targetHeight)) {
        fork->forkHeight = targetHeight;
        fork->forkHeightLerpDone = -1;
        return;
    }
    const float next = height - FrameScaledStep(fork->forkHeightStepRate);
    if (fork->hasAttachedLoad == 1 && fork->carriedLoad)
        fork->carriedLoad->entity.node.position.y = next;
    fork->forkHeight = next;
}


// @verified - p
void flmwSetActorVisibility(ScenarioContext *context, std::uint32_t id, std::int32_t visible) {
    if (!id)
        return;
    Character *const character = SCEN_FindCharacterByFunctionId(context, id);
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *task = nullptr;
    auto *link = static_cast<CharacterBucketNode *>(control->characterBuckets[reinterpret_cast<std::uintptr_t>(character) % 13U]);
    for (; link; link = link->next) {
        if (link->character == character) {
            task = link->task;
            break;
        }
    }
    if (character) {
        if (visible)
            character->flags.value |= CharacterFlagVisible;
        else
            character->flags.value &= ~CharacterFlagVisible;
        const bool wasVisible = (character->motionFlags & 0x3fU) != 0;
        character->motionFlags = visible ? character->motionFlags | 0x3fU : character->motionFlags & 0xc0U;
        const bool isVisible = (character->motionFlags & 0x3fU) != 0;
        if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && isVisible != wasVisible)
            CHAR_LogNPCVisibility(character, isVisible);
    }
    if (task) {
        if (!task->initCallbackFnPtr) {
            task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
            task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        }
        task->mainExecutionIterations = static_cast<std::uint32_t>(visible);
    }
}

// @verified - p
void flmwSetHelperActorVisibility(ScenarioContext *context, const ForkliftWork *fork, std::int32_t visible) {
    flmwSetActorVisibility(context, fork->helperActorId0.name, visible);
    flmwSetActorVisibility(context, fork->helperActorId1.name, visible);
}

void shenSendAICAParamCommands(const ForkliftWork *fork, std::int16_t channelVolume, std::int16_t bankVolume) {
    shenExecuteAICACommand(0x10a0U, 4, channelVolume);
    shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), bankVolume);
    shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), bankVolume);
}



// @verified - p
void flmwUpdateAuthoredCameraBlend(ScenarioContext *context, ForkliftWork *fork) {
    if (fork->followerLoadNearCab || fork->manualCameraProbe) {
        const std::int16_t step = fork->cameraBlendStep;
        GSettings.movementFactor = 1.0f;
        if (step >= 20) {
            g_temporaryGameState = 1;
            ApplyPendingGameState(g_previousAuthorState);
            flmwSetActorVisibility(context, fork->helperActorId0.name, 1);
            flmwSetActorVisibility(context, fork->helperActorId1.name, 1);

            GSettings.rotationFactor = 0.2f;
            GSettings.flags = 1;
            GSettings.temporaryYaw = 0;
            GSettings.temporaryPitch = 1820;
            GSettings.fieldOfView = 14745;
            bNoBlendCameraInterp = 1;
            rotationOffsetVector.x = 0.0f;
            rotationOffsetVector.y = 0.0f;
        } else {
            fork->cameraBlendStep = static_cast<std::int16_t>(step + 1);
            const std::int32_t fieldOfView = currentPerspectiveFov + 182;
            const auto blend = static_cast<double>(static_cast<float>(step));
            CameraDistanceFromPlayer = static_cast<float>(4.3 - blend * 0.215);
            CameraHeightAbovePlayer = static_cast<float>(2.32 - blend * 0.057999999999999996);
            CameraHeightAbovePlayer_MAX = CameraHeightAbovePlayer;
            CAME_SetFieldOfView(fieldOfView);
        }
    } else {
        GSettings.movementFactor = 0.25f;
        g_temporaryGameState = 0;
        ApplyPendingGameState(g_previousAuthorState);
        CameraYAngle = 1.1f;
        CameraDistanceFromPlayer = 4.3f;
        CameraHeightAbovePlayer = 2.32f;
        CameraHeightAbovePlayer_MAX = 2.32f;
        CAME_SetFieldOfView(10922);
        g_bIsConversationCamInactive = 0;
        g_CameraCollisionEnabled = 1;
        flmwSetActorVisibility(context, fork->helperActorId0.name, 0);
        flmwSetActorVisibility(context, fork->helperActorId1.name, 0);
        if (fork->cameraBlendStep)
            fork->cameraBlendStep = static_cast<std::int16_t>(fork->cameraBlendStep - 1);
    }
    g_ryoVehicleAlphaSteps = fork->cameraBlendStep;
}


float RotateMatrixAroundYAndApply(const Vector3 &object, std::int16_t angle, float scaleFactor) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angle));
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 direction{((m.rows[0].x * 0.0f + m.rows[1].x * 0.0f) - m.rows[2].x * 1.0f) + m.rows[3].x,
        ((m.rows[0].y * 0.0f + m.rows[1].y * 0.0f) - m.rows[2].y * 1.0f) + m.rows[3].y,
        ((m.rows[1].z * 0.0f + m.rows[0].z * 0.0f) - m.rows[2].z * 1.0f) + m.rows[3].z};
    Matrix4x4::Pop(1);
    return CalculateNavigationCollisionResponse(object, direction, nullptr, scaleFactor, 4);
}

// The oriented box query from three transformed local corners, offset by a world position.
QueryShape BuildOrientedBoxQuery(const Vector3 &origin, const Vector3 &edgeA, const Vector3 &edgeB, float x, float z) {
    QueryShape query{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
    query.packed[0].x = origin.x + x;
    query.packed[0].y = origin.z + z;
    query.packed[0].z = (edgeA.x + x) - (origin.x + x);
    query.packed[1].x = (edgeA.z + z) - (origin.z + z);
    query.packed[1].y = (edgeB.x + x) - (origin.x + x);
    query.packed[1].z = (edgeB.z + z) - (origin.z + z);
    return query;
}


// @verified - p
QueryShape flmwBuildLoadFootprintQuery(Character *character, const Vector3 &position) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(SumEntityYaw(character)));
    Vector3 origin{};
    Vector3 edgeA{};
    Vector3 edgeB{};
    Matrix4x4::TransformPoint({-0.75f, 0.0f, -0.75f}, origin);
    Matrix4x4::TransformPoint({0.75f, 0.0f, -0.75f}, edgeA);
    Matrix4x4::TransformPoint({-0.75f, 0.0f, 0.75f}, edgeB);
    Matrix4x4::Pop(1);
    QueryShape query{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
    query.packed[0].x = origin.x + position.x;
    query.packed[0].y = origin.z + position.z;
    query.packed[0].z = (edgeA.x + position.x) - query.packed[0].x;
    query.packed[1].x = (edgeA.z + position.z) - query.packed[0].y;
    query.packed[1].y = (edgeB.x + position.x) - query.packed[0].x;
    query.packed[1].z = (edgeB.z + position.z) - query.packed[0].y;
    return query;
}


bool MapFootprintHit(const QueryShape &query, const std::uint8_t *mask) {
    return ExecuteMathFnWrapper(&query, mask) && COLS_GetSelectedCollisionRecordValue() >= 0;
}


// @verified - p
void flmwUpdateObstacleAndFollowerCollision(ScenarioContext *context, Character *character, ForkliftWork *fork) {
    const Vector3 world = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(fork->moveYawFixed16));
    float x = world.x + fork->moveDeltaX;
    float z = world.z + fork->moveDeltaZ;
    Vector3 origin{};
    Vector3 edgeA{};
    Vector3 edgeB{};
    if (static_cast<double>(fork->driveSpeed) >= 0.000001) {
        Matrix4x4::TransformPoint({-0.4f, 0.0f, -1.85f}, origin);
        Matrix4x4::TransformPoint({0.4f, 0.0f, -1.85f}, edgeA);
        Matrix4x4::TransformPoint({-0.4f, 0.0f, -0.9f}, edgeB);
    } else {
        Matrix4x4::TransformPoint({-0.6f, 0.0f, -0.75f}, origin);
        Matrix4x4::TransformPoint({0.6f, 0.0f, -0.75f}, edgeA);
        Matrix4x4::TransformPoint({-0.6f, 0.0f, 1.2f}, edgeB);
    }
    QueryShape moving = BuildOrientedBoxQuery(origin, edgeA, edgeB, x, z);
    bool blocked = MapFootprintHit(moving, fork->hasAttachedLoad ? nullptr : g_flmwNoLoadFootprintMathFnMask);
    if (fork->hasAttachedLoad == 1 && fork->driveSpeed >= 0.0f) {
        Vector3 loadOffset{};
        Matrix4x4::TransformPoint(fork->carriedLoad ? fork->carriedLoad->entity.node.position : Vector3{}, loadOffset);
        const Vector3 current = character ? GetPositionForEntity(character->entity.node) : Vector3{};
        z = loadOffset.z + (current.z + fork->moveDeltaZ);
        x = loadOffset.x + (current.x + fork->moveDeltaX);
        Matrix4x4_Push();
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(fork->carriedLoad ? fork->carriedLoad->entity.node.rotation.y : 0));
        Matrix4x4::TransformPoint({-0.8f, 0.0f, -0.8f}, origin);
        Matrix4x4::TransformPoint({0.8f, 0.0f, -0.8f}, edgeA);
        Matrix4x4::TransformPoint({-0.8f, 0.0f, 0.8f}, edgeB);
        Matrix4x4::Pop(1);
        moving = BuildOrientedBoxQuery(origin, edgeA, edgeB, x, z);
        if (MapFootprintHit(moving, fork->hasAttachedLoad ? nullptr : g_flmwNoLoadFootprintMathFnMask))
            blocked = true;
    }
    const Vector3 current = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    x = current.x + fork->moveDeltaX;
    z = current.z + fork->moveDeltaZ;
    Matrix4x4::TransformPoint({-0.6f, 0.0f, -0.75f}, origin);
    Matrix4x4::TransformPoint({0.6f, 0.0f, -0.75f}, edgeA);
    Matrix4x4::TransformPoint({-0.6f, 0.0f, 1.2f}, edgeB);
    const QueryShape body = BuildOrientedBoxQuery(origin, edgeA, edgeB, x, z);
    Matrix4x4::TransformPoint({-0.4f, 0.0f, -1.85f}, origin);
    Matrix4x4::TransformPoint({0.4f, 0.0f, -1.85f}, edgeA);
    Matrix4x4::TransformPoint({-0.4f, 0.0f, -0.9f}, edgeB);
    const QueryShape forks = BuildOrientedBoxQuery(origin, edgeA, edgeB, x, z);
    if (fork->followerLoadIds != kNoFollowerList && fork->followerLoadIds[0].name != kNoFollowerId) {
        for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
            Character *const follower = SCEN_FindCharacterByFunctionId(context, fork->followerLoadIds[index].name);
            const Vector3 position = follower ? GetPositionForEntity(follower->entity.node) : Vector3{};
            if (fork->carriedLoad == follower)
                continue;
            const QueryShape load = flmwBuildLoadFootprintQuery(follower, position);
            const auto height = static_cast<double>(fork->forkHeight);
            if (fork->hasAttachedLoad == 1 && ((height > 1.49 && static_cast<double>(position.y) > 1.49) || 1.51 > height) &&
                CheckVectorWithinConditions(load, moving)) {
                blocked = true;
            }
            if (CheckVectorWithinConditions(load, body))
                blocked = true;
            const float forkHeight = fork->forkHeight;
            if ((static_cast<double>(forkHeight) > 0.010999999776482583 && 1.4900000002235174 > static_cast<double>(forkHeight)) ||
                (forkHeight > 1.5f && static_cast<double>(position.y) > 0.010999999776482583)) {
                if (CheckVectorWithinConditions(load, forks))
                    blocked = true;
            }
        }
    }
    Matrix4x4::Pop(1);
    if (blocked) {
        if (!g_flmwRollbackActive) {
            const auto throttle = static_cast<double>(fork->throttleDelta);
            if ((throttle > 0.04 || -0.02 > throttle) && fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0x1004a9U, 0, 0);
            fork->moveYawFixed16 = SumEntityYaw(character);
            fork->driveSpeed = fork->driveSpeed * 0.5f;
            g_flmwRollbackActive = 1;
            g_flmwRollbackExpiryFrame = GetFramesPastCopy() + 2;
        }
        if (g_flmwRollbackActive && GetFramesPastCopy() > g_flmwRollbackExpiryFrame) {
            if (character)
                shenApplyTranslationToAll(character->entity.node, g_flmwRollbackPos);
            fork->moveYawFixed16 = g_flmwRollbackYawSumFixed16;
            fork->driveSpeed = 0.0f;
            fork->moveDeltaX = 0.0f;
            fork->moveDeltaZ = 0.0f;
            g_flmwRollbackActive = 0;
            if (character)
                character->entity.node.rotation.y = YawRelativeToParents(character, static_cast<std::int16_t>(g_flmwRollbackYawSumFixed16));
        }
    } else if (g_flmwRollbackActive) {
        if (GetFramesPastCopy() > g_flmwRollbackExpiryFrame) {
            fork->driveSpeed = fork->driveSpeed * 0.5f;
            fork->moveDeltaX = 0.0f;
            fork->moveDeltaZ = 0.0f;
            if (0.03f > fork->driveSpeed)
                g_flmwRollbackActive = 0;
        }
    } else if (character) {
        g_flmwRollbackPos = GetPositionForEntity(character->entity.node);
        g_flmwRollbackYawSumFixed16 = SumEntityYaw(character);
    } else {
        g_flmwRollbackPos = {};
        g_flmwRollbackYawSumFixed16 = 0;
    }
}


// @verified - p
void au_re_ApplyRotationMatrix(Character *character, SteeringShapeXZ &shape) {
    shape.center = character ? character->entity.node.position : Vector3{};
    shape.yaw = SumEntityYaw(character);
    shape.radius = 0.0f;
    const std::array<Vector3, 4> corners = CycleManager::BuildForkliftSteeringQuad(shape.center, shape.yaw);
    for (std::size_t index = 0; index < corners.size(); ++index)
        shape.quad_corners[index] = corners[index];
}


// @verified - p
void flmwApplyForkliftSpacingAssist(ScenarioContext *context, Character *character) {
    SteeringShapeXZ center{};
    au_re_ApplyRotationMatrix(character, center);
    ForkliftWork *const fork = FLMW_GetWork(character);
    center.push_resistance = fork->sideMode ? 0xffU : 0x7fU;
    bool overlapped = false;
    for (const std::uint32_t name : kRaceForklifts) {
        Character *const other = SCEN_FindCharacterByFunctionId(context, name);
        if (other == character)
            continue;
        SteeringShapeXZ shape{};
        au_re_ApplyRotationMatrix(other, shape);
        shape.push_resistance = FLMW_GetWork(other)->sideMode ? 0xffU : 0x7fU;
        overlapped = shenResolveSteeringShapeOverlapXZ(center, shape) || overlapped;
    }
    if (overlapped) {
        if (character)
            character->entity.node.position = center.center;
        if (!g_flmwSpacingAssistActive) {
            if (fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0xb04a9U, 0, 0);
            g_flmwSpacingAssistActive = 1;
        }
    } else if (fork->ownerCharacter.name == kAkira) {
        g_flmwSpacingAssistActive = 0;
    }
}


// @verified - p
void flmwCheckFollowerLoadsNearCab(ScenarioContext *context, Character *character) {
    ForkliftWork *const fork = FLMW_GetWork(character);
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    const std::int32_t yaw = character ? character->entity.node.rotation.y : 0;
    fork->followerLoadNearCab = 0;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    const shen_name *const ids = fork->followerLoadIds;
    for (std::size_t index = 0; ids[index].name != kNoFollowerId; ++index) {
        Character *const load = SCEN_FindCharacterByFunctionId(context, ids[index].name);
        const Vector3 loadPosition = load ? load->entity.node.position : Vector3{};
        Vector3 relative{};
        Matrix4x4::TransformPoint({position.x - loadPosition.x, position.y, position.z - loadPosition.z}, relative);
        if (static_cast<double>(relative.x) > -1.2 && 1.2 > static_cast<double>(relative.x) && 6.0f > relative.z && relative.z > 0.0f &&
            !fork->hasAttachedLoad) {
            fork->followerLoadNearCab = 1;
        }
    }
    Matrix4x4::Pop(1);
}

bool LoadAtForkHeight(const ForkliftWork *fork, float loadY) {
    const auto height = static_cast<double>(fork->forkHeight);
    return static_cast<double>(loadY) >= height - 0.01 && height + 0.01 >= static_cast<double>(loadY);
}



// @verified - p
void flmwSelectLoadCandidateInForkChannel(ScenarioContext *context, Character *character) {
    ForkliftWork *const fork = FLMW_GetWork(character);
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    const std::int32_t yaw = character ? character->entity.node.rotation.y : 0;
    const float forkHeight = fork->forkHeight;
    fork->pendingLoadCandidateId = shen_name(kNoFollowerId);
    fork->loadActionBlockState = LOAD_ACTION_CLEAR;
    const float heightBelow = forkHeight - 0.01f;
    const bool atGround = !(static_cast<double>(heightBelow) > 0.001) && static_cast<double>(forkHeight + 0.01f) >= 0.001;
    const bool atApproach = !(heightBelow > 1.5f) && static_cast<double>(forkHeight) + 0.01 >= 1.5;
    if (!atGround && !atApproach)
        return;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
        const shen_name *const ids = fork->followerLoadIds;
        Character *const load = SCEN_FindCharacterByFunctionId(context, ids[index].name);
        const Vector3 loadPosition = load ? load->entity.node.position : Vector3{};
        Vector3 relative{};
        Matrix4x4::TransformPoint({position.x - loadPosition.x, position.y, position.z - loadPosition.z}, relative);
        const float side = relative.x;
        const float ahead = relative.z;
        if (side > -0.4f && 0.4f > side && 2.65 > static_cast<double>(ahead) && static_cast<double>(ahead) > 1.95 && LoadAtForkHeight(fork, loadPosition.y)) {
            fork->loadActionBlockState = LOAD_ACTION_SOFT_BLOCK;
            continue;
        }
        if ((static_cast<double>(side) > -1.25 && -0.4f >= side && 2.4499999999999997 > static_cast<double>(ahead) && ahead > 1.0f) ||
            (side >= 0.4f && 1.25 > static_cast<double>(side) && 2.4499999999999997 > static_cast<double>(ahead) && ahead > 1.0f &&
                LoadAtForkHeight(fork, loadPosition.y))) {
            fork->loadActionBlockState = LOAD_ACTION_SOFT_BLOCK;
            continue;
        }
        if (!(side > -0.4f && 0.4f > side && ahead > 1.0f && 1.95f > ahead && LoadAtForkHeight(fork, loadPosition.y)))
            continue;
        bool stacked = false;
        for (std::size_t other = 0; ids[other].name != kNoFollowerId; ++other) {
            if (other == index)
                continue;
            Character *const otherLoad = SCEN_FindCharacterByFunctionId(context, fork->followerLoadIds[other].name);
            const Vector3 otherPosition = otherLoad ? GetPositionForEntity(otherLoad->entity.node) : Vector3{};
            const float dx = otherPosition.x - loadPosition.x;
            const float dz = otherPosition.z - loadPosition.z;
            if (0.8 > static_cast<double>(std::sqrt(dx * dx + dz * dz)) && otherPosition.y > loadPosition.y) {
                fork->loadActionBlockState = LOAD_ACTION_HARD_BLOCK;
                stacked = true;
                break;
            }
        }
        if (stacked || fork->loadActionBlockState)
            continue;
        fork->pendingLoadCandidateId = fork->followerLoadIds[index];
        break;
    }
    Matrix4x4::Pop(1);
}


// @verified - p
bool forkAttachSelectedLoad(ScenarioContext *context, Character *character, ForkliftWork *fork) {
    const shen_name id = fork->pendingLoadCandidateId;
    if (id.name == kNoFollowerId)
        return false;
    fork->hasAttachedLoad = 1;
    Character *const load = SCEN_FindCharacterByFunctionId(context, id.name);
    fork->carriedLoad = load;
    fork->attachedLoadId = id;
    const Vector3 position = load ? GetPositionForEntity(load->entity.node) : Vector3{};
    const auto worldYaw = static_cast<std::int16_t>(charaGetWorldYaw(fork->carriedLoad));
    load->entity.node.child = character ? &character->entity.node : nullptr;
    if (fork->carriedLoad)
        shenApplyTranslationToAll(fork->carriedLoad->entity.node, position);
    if (fork->carriedLoad)
        fork->carriedLoad->entity.node.rotation.y = YawRelativeToParents(fork->carriedLoad, worldYaw);
    ccowDisableCharaFlags(fork->carriedLoad, 7);
    return true;
}


// @verified - p
void forkliftDetach(ForkliftWork *fork) {
    ccowEnableCharaFlags(fork->carriedLoad, 7);
    fork->hasAttachedLoad = 0;
    Vector3 position = fork->carriedLoad ? GetPositionForEntity(fork->carriedLoad->entity.node) : Vector3{};
    position.y = fork->forkHeight;
    const std::int16_t worldYaw = SumEntityYaw(fork->carriedLoad);
    fork->carriedLoad->entity.node.child = nullptr;
    if (fork->carriedLoad)
        SetWorldPositionAndYaw(fork->carriedLoad, position, worldYaw);
    fork->carriedLoad = nullptr;
    fork->attachedLoadId = {};
    fork->driveSpeed = 0.0f;
}


// @verified - p
bool forkCheckPlacementAndRelease(ScenarioContext *context, Character *character, ForkliftWork *fork) {
    bool blocked = false;
    if (fork->followerLoadIds == kNoFollowerList || fork->hasAttachedLoad)
        return false;
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(SumEntityYaw(character)));
    Vector3 origin{};
    Vector3 edgeA{};
    Vector3 edgeB{};
    Matrix4x4::TransformPoint({-0.4f, 0.0f, -1.85f}, origin);
    Matrix4x4::TransformPoint({0.4f, 0.0f, -1.85f}, edgeA);
    Matrix4x4::TransformPoint({-0.4f, 0.0f, -0.9f}, edgeB);
    Matrix4x4::Pop(1);
    const QueryShape forks = BuildOrientedBoxQuery(origin, edgeA, edgeB, position.x, position.z);
    for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
        const std::uint32_t id = fork->followerLoadIds[index].name;
        if (fork->attachedLoadId.name == id)
            continue;
        Character *const load = SCEN_FindCharacterByFunctionId(context, id);
        const Vector3 loadPosition = load ? GetPositionForEntity(load->entity.node) : Vector3{};
        if (0.011 > static_cast<double>(loadPosition.y) && CheckVectorWithinConditions(flmwBuildLoadFootprintQuery(load, loadPosition), forks))
            blocked = true;
    }
    return blocked;
}


// @verified - p
bool forkCheckRyoAgainstFollowerLoads(ScenarioContext *context, ForkliftWork *fork) {
    bool overlapped = false;
    if (!fork->hasAttachedLoad || fork->followerLoadIds == kNoFollowerList)
        return false;
    const Vector3 carried = fork->carriedLoad ? GetPositionForEntity(fork->carriedLoad->entity.node) : Vector3{};
    const QueryShape carriedShape = flmwBuildLoadFootprintQuery(fork->carriedLoad, carried);
    for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
        const std::uint32_t id = fork->followerLoadIds[index].name;
        if (fork->attachedLoadId.name == id)
            continue;
        Character *const load = SCEN_FindCharacterByFunctionId(context, id);
        const Vector3 loadPosition = load ? GetPositionForEntity(load->entity.node) : Vector3{};
        if (!(1.49 > static_cast<double>(loadPosition.y)) || !CheckVectorWithinConditions(flmwBuildLoadFootprintQuery(load, loadPosition), carriedShape))
            continue;
        const float dx = carried.x - loadPosition.x;
        const float dz = carried.z - loadPosition.z;
        if (static_cast<double>(std::sqrt(dx * dx + dz * dz)) > 0.6) {
            if (fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0x704a9U, 0, 0);
            overlapped = true;
        }
    }
    return overlapped;
}


// @verified - p
std::int16_t flmwUpdateDriveLiftAndCarryState(ScenarioContext *context, Character *character) {
    ForkliftWork *const fork = FLMW_GetWork(character);
    g_flmwForwardInputPressed = 0;
    const InputSlot &input = g_inputState.slots[0];
    const auto buttons = static_cast<std::uint16_t>(input.held & fork->inputMask);
    const auto pressed = static_cast<std::uint16_t>(input.pressedPriority & fork->inputMask);
    const auto forwardTrigger = static_cast<std::uint8_t>(input.triggers & (fork->inputMask >> 24));
    const auto reverseTrigger = static_cast<std::uint8_t>((input.triggers >> 8) & (fork->inputMask >> 16));
    if ((buttons & 0xff3fU) == kButtonUp && (pressed & 0xff3fU) == kButtonUp) {
        if (fork->ownerCharacter.name == kAkira)
            shenExecuteAICACommand(fork->engineEnabledFrames ? 0x504a9U : 0x904a9U, 0, 0);
        g_flmwForwardInputPressed = 1;
    }
    if (pressed & kButtonB)
        fork->manualCameraProbe ^= 1U;
    flmwUpdateAuthoredCameraBlend(context, fork);

    auto stick = static_cast<std::uint8_t>(input.leftStick);
    if ((fork->inputMask & 0xc000U) == 0)
        stick = 0x80;
    if (stick > 0x80U || (buttons & kButtonRight) != 0) {
        fork->steerRecenterMode = stick == 0x80U;
        const std::int32_t steer = fork->steerOffsetAngle - fork->steerStep;
        fork->steerOffsetAngle = steer < -fork->steerLimit ? -fork->steerLimit : steer;
    } else if (stick < 0x80U || (buttons & kButtonLeft) != 0) {
        fork->steerRecenterMode = stick == 0x80U;
        fork->steerOffsetAngle += fork->steerStep;
        if (fork->steerOffsetAngle > fork->steerLimit)
            fork->steerOffsetAngle = fork->steerLimit;
    } else if (!fork->steerRecenterMode) {
        if (fork->steerOffsetAngle < 0) {
            const std::int32_t steer = fork->steerOffsetAngle + 2 * fork->tuning->steerCenterStep;
            fork->steerOffsetAngle = steer > 0 ? 0 : steer;
        }
        if (fork->steerOffsetAngle > 0) {
            fork->steerOffsetAngle -= 2 * fork->tuning->steerCenterStep;
            if (fork->steerOffsetAngle < 0)
                fork->steerOffsetAngle = 0;
        }
    }

    float forwardClearance = 10.0f;
    float reverseClearance = 10.0f;
    if (!fork->interactionProfile) {
        SelectCharacterMapQuery(character);
        const Vector3 position = character ? character->entity.node.position : Vector3{};
        const std::int32_t yaw = character ? character->entity.node.rotation.y : 0;
        const std::int32_t steerYaw = fork->steerOffsetAngle + yaw;
        const float centerProbe = RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(steerYaw), 0.9f);
        std::int32_t spread = fork->steerOffsetAngle >> 1;
        if (spread < 0)
            spread = -spread;
        if (spread < 455)
            spread = 455;
        const float leftProbe = RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(steerYaw - spread), 0.6f);
        const float rightProbe = RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(spread + steerYaw), 0.6f);
        forwardClearance = static_cast<float>(static_cast<double>(rightProbe + (leftProbe + centerProbe)) * 0.3333333333333333);
        const float straightProbe = RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(character ? character->entity.node.rotation.y : 0), 0.7f);
        if (static_cast<std::uint32_t>(fork->steerOffsetAngle + 0xe38) > 0x1c70U)
            forwardClearance = straightProbe < forwardClearance ? straightProbe : forwardClearance;
        if (2.0f > forwardClearance || (fork->hasAttachedLoad == 1 && 2.5f > forwardClearance))
            forwardClearance = -1.0f;
        const std::int32_t reverseYaw = (character ? character->entity.node.rotation.y : 0) + 0x8000;
        const float rearProbe = RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(reverseYaw), 0.7f);
        const float rearSteerProbe = RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(reverseYaw - fork->steerOffsetAngle), 0.7f);
        reverseClearance = rearSteerProbe < rearProbe ? rearSteerProbe : rearProbe;
        if (const std::int32_t steer = fork->steerOffsetAngle) {
            const std::int32_t steerProbeYaw = (character ? character->entity.node.rotation.y : 0) - steer;
            if (1.8 > static_cast<double>(RotateMatrixAroundYAndApply(position, static_cast<std::int16_t>(steerProbeYaw), 0.6f)))
                reverseClearance = -1.0f;
        }
        if (1.3 > static_cast<double>(reverseClearance))
            reverseClearance = -1.0f;
        if (0.0f > forwardClearance && 0.0f > reverseClearance) {
            if (CycleManager::GetRelativePositionToCycles(character) & 1U)
                reverseClearance = 1.0f;
            else
                forwardClearance = 1.5f;
        }
    }

    const ForkliftDriveProfile *const tuning = fork->tuning;
    bool forwardDecayed = false;
    if (forwardTrigger) {
        const float speed = fork->driveSpeed;
        const auto target = static_cast<float>(
            static_cast<double>(forwardTrigger) / 255.0 * static_cast<double>(tuning->maxForwardSpeed) * (static_cast<double>(forwardClearance) / 10.0));
        if (target > speed) {
            if (fork->forkHeightMoveState != FORK_MOVE_LOWER_TARGET || !fork->hasAttachedLoad)
                fork->driveSpeed = static_cast<float>(forwardTrigger >> 3) * 0.03125f * tuning->throttleAccelStep + speed;
        } else {
            fork->driveSpeed = speed + (5.0f > forwardClearance ? tuning->brakeStepTightClearance : tuning->coastDecayStep);
            forwardDecayed = true;
        }
    } else if (fork->driveSpeed > 0.0f) {
        fork->driveSpeed = fork->driveSpeed + (5.0f > forwardClearance ? tuning->brakeStepTightClearance : tuning->coastDecayStep);
        forwardDecayed = true;
    }
    if (forwardDecayed) {
        const float speed = fork->driveSpeed;
        if (0.0f > speed || 0.0f > forwardClearance) {
            fork->driveSpeed = 0.0f;
            if (static_cast<double>(speed) > 0.15)
                return FORK_UPDATE_FORWARD_IMPACT;
        }
    }

    bool reverseDecayed = false;
    if (reverseTrigger) {
        const float speed = fork->driveSpeed;
        const auto target = static_cast<float>(
            static_cast<double>(reverseTrigger) / 255.0 * static_cast<double>(tuning->maxReverseSpeed) * (static_cast<double>(reverseClearance) / 10.0));
        if (speed > target) {
            if (fork->forkHeightMoveState != FORK_MOVE_LOWER_TARGET || !fork->hasAttachedLoad)
                fork->driveSpeed = speed - static_cast<float>(reverseTrigger >> 3) * 0.03125f * tuning->throttleAccelStep;
        } else {
            fork->driveSpeed = speed - (3.0f > reverseClearance ? tuning->brakeStepTightClearance : tuning->coastDecayStep);
            reverseDecayed = true;
        }
    } else if (0.0f > fork->driveSpeed) {
        fork->driveSpeed = fork->driveSpeed - (3.0f > reverseClearance ? tuning->brakeStepTightClearance : tuning->coastDecayStep);
        reverseDecayed = true;
    }
    if (reverseDecayed && (fork->driveSpeed > 0.0f || 0.0f > reverseClearance))
        fork->driveSpeed = 0.0f;

    if (forwardTrigger && reverseTrigger) {
        const float speed = fork->driveSpeed;
        if (speed > 0.0f) {
            fork->driveSpeed = speed + tuning->brakeStepTightClearance;
            if (0.0f > fork->driveSpeed)
                fork->driveSpeed = 0.0f;
        } else {
            fork->driveSpeed = speed - tuning->brakeStepTightClearance;
            if (fork->driveSpeed > 0.0f)
                fork->driveSpeed = 0.0f;
        }
    }
    if (fork->driveSpeed > tuning->maxForwardSpeed)
        fork->driveSpeed = tuning->maxForwardSpeed;
    if (tuning->maxReverseSpeed > fork->driveSpeed)
        fork->driveSpeed = tuning->maxReverseSpeed;
    const std::uint32_t inputMask = fork->inputMask;
    if ((inputMask & 0xff000000U) == 0 && fork->driveSpeed > 0.0f) {
        fork->driveSpeed = 0.0f;
        fork->throttleDelta = 0.0f;
        fork->moveDeltaX = 0.0f;
        fork->moveDeltaZ = 0.0f;
    }
    if ((inputMask & 0x00ff0000U) == 0 && 0.0f > fork->driveSpeed) {
        fork->driveSpeed = 0.0f;
        fork->throttleDelta = 0.0f;
        fork->moveDeltaX = 0.0f;
        fork->moveDeltaZ = 0.0f;
    }
    if (fork->followerLoadIds != kNoFollowerList) {
        flmwCheckFollowerLoadsNearCab(context, character);
        flmwSelectLoadCandidateInForkChannel(context, character);
    }

    if ((pressed & kButtonA) && fork->forkHeightMoveState == FORK_MOVE_IDLE) {
        if (fork->loadActionBlockState) {
            if (fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0x804a9U, 0, 0);
            if (fork->loadActionBlockState == LOAD_ACTION_HARD_BLOCK) {
                fork->driveSpeed = 0.0f;
                return FORK_UPDATE_LOAD_HARD_BLOCKED;
            }
        } else if (fork->engineEnabledFrames != 2 && fork->hasAttachedLoad == 1) {
            fork->driveSpeed = 0.0f;
            return static_cast<std::int16_t>(static_cast<std::int8_t>(fork->engineEnabledFrames) + 1);
        } else {
            const double speed = fork->driveSpeed;
            const bool stationary = 0.01 > speed && speed > -0.01;
            switch (fork->loadSequencePhase) {
            case LOAD_SEQ_IDLE: {
                const bool attached = forkAttachSelectedLoad(context, character, fork);
                fork->forkHeightMoveState = FORK_MOVE_RAISE_TARGET;
                if (attached) {
                    fork->targetForkHeight = 1.8f;
                    fork->loadSequencePhase = LOAD_SEQ_CARRIED_RAISED_WAIT_DROP;
                } else {
                    fork->loadSequencePhase = static_cast<ForkliftLoadSequencePhase>(fork->loadSequencePhase + 1);
                    fork->targetForkHeight = 1.5f;
                }
                break;
            }
            case LOAD_SEQ_RETRY_PICKUP_AT_APPROACH_HEIGHT:
                forkAttachSelectedLoad(context, character, fork);
                fork->loadSequencePhase = static_cast<ForkliftLoadSequencePhase>(fork->loadSequencePhase + 1);
                fork->targetForkHeight = 1.8f;
                fork->forkHeightMoveState = FORK_MOVE_RAISE_TARGET;
                break;
            case LOAD_SEQ_CARRIED_RAISED_WAIT_DROP:
                if ((stationary || !fork->hasAttachedLoad) && !forkCheckRyoAgainstFollowerLoads(context, fork)) {
                    fork->loadSequencePhase = static_cast<ForkliftLoadSequencePhase>(fork->loadSequencePhase + 1);
                    fork->targetForkHeight = 1.5f;
                    fork->forkHeightMoveState = FORK_MOVE_LOWER_TARGET;
                }
                break;
            case LOAD_SEQ_DROP_PLACEMENT_CHECK:
                if (stationary || !fork->hasAttachedLoad) {
                    if (forkAttachSelectedLoad(context, character, fork) || forkCheckPlacementAndRelease(context, character, fork)) {
                        fork->forkHeightMoveState = FORK_MOVE_RAISE_TARGET;
                        fork->targetForkHeight = 1.8f;
                        fork->loadSequencePhase = LOAD_SEQ_CARRIED_RAISED_WAIT_DROP;
                    } else {
                        fork->targetForkHeight = 0.001f;
                        fork->loadSequencePhase = LOAD_SEQ_IDLE;
                        fork->forkHeightMoveState = FORK_MOVE_LOWER_TARGET;
                    }
                }
                break;
            default:
                break;
            }
            if (fork->forkHeightMoveState == FORK_MOVE_RAISE_TARGET) {
                if (fork->ownerCharacter.name == kAkira)
                    shenExecuteAICACommand(0x1304a9U, 0, 0);
            } else if (fork->forkHeightMoveState != FORK_MOVE_IDLE && fork->ownerCharacter.name == kAkira) {
                shenExecuteAICACommand(0x1104a9U, 0, 0);
            }
        }
    }

    if (fork->forkHeightMoveState == FORK_MOVE_RAISE_TARGET) {
        const float target = fork->targetForkHeight;
        if (target > fork->forkHeight) {
            flmwRaiseForkHeightTowardTarget(fork, target);
        } else {
            fork->forkHeight = target;
            fork->forkHeightMoveState = FORK_MOVE_IDLE;
            if (fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0x604a9U, 0, 0);
        }
    } else if (fork->forkHeightMoveState == FORK_MOVE_LOWER_TARGET) {
        const float target = fork->targetForkHeight;
        if (fork->forkHeight > target) {
            flmwLowerForkHeightTowardTarget(fork, target);
        } else {
            fork->forkHeight = target;
            fork->forkHeightMoveState = FORK_MOVE_IDLE;
            if (fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0x404a9U, 0, 0);
            if (fork->hasAttachedLoad == 1) {
                const auto height = static_cast<double>(fork->forkHeight);
                if (height <= 0.010999999776482583) {
                    forkliftDetach(fork);
                    if (fork->ownerCharacter.name == kAkira)
                        shenExecuteAICACommand(0xa04a9U, 0, 0);
                } else if (height - 0.01 <= 1.5 && height + 0.01 >= 1.5) {
                    const Vector3 carried = fork->carriedLoad ? GetPositionForEntity(fork->carriedLoad->entity.node) : Vector3{};
                    for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
                        Character *const load = SCEN_FindCharacterByFunctionId(context, fork->followerLoadIds[index].name);
                        if (fork->carriedLoad == load)
                            continue;
                        const Vector3 loadPosition = load ? GetPositionForEntity(load->entity.node) : Vector3{};
                        const float dx = loadPosition.x - carried.x;
                        const float dz = loadPosition.z - carried.z;
                        if (0.8 > static_cast<double>(std::sqrt(dx * dx + dz * dz)) && carried.y > loadPosition.y) {
                            forkliftDetach(fork);
                            if (fork->ownerCharacter.name == kAkira)
                                shenExecuteAICACommand(0xa04a9U, 0, 0);
                            break;
                        }
                    }
                }
            }
        }
    }

    const float speed = fork->driveSpeed;
    if ((static_cast<double>(speed) > 0.001 || -0.001 > static_cast<double>(speed)) && fork->steerOffsetAngle) {
        if (fork->steerOffsetAngle < 0) {
            const std::int32_t steer = fork->steerOffsetAngle + fork->tuning->steerCenterStep;
            fork->steerOffsetAngle = steer > 0 ? 0 : steer;
        }
        if (fork->steerOffsetAngle > 0) {
            const std::int32_t steer = fork->steerOffsetAngle - fork->tuning->steerCenterStep;
            fork->steerOffsetAngle = steer < 0 ? 0 : steer;
        }
    }
    const auto maxForward = static_cast<double>(fork->tuning->maxForwardSpeed);
    const auto speedHeadroom = static_cast<double>(fork->tuning->maxForwardSpeed - speed);
    fork->steerLimit = static_cast<std::uint16_t>(static_cast<std::int32_t>(speedHeadroom * 20.0 / maxForward * 8192.0 / 45.0)) + 2912;
    fork->steerStep = static_cast<std::uint16_t>(static_cast<std::int32_t>(speedHeadroom * 0.9 / maxForward * 8192.0 / 45.0)) + 236;
    return FORK_UPDATE_OK;
}


// @verified - p
void flmwUpdateMotionAndHelperActors(ScenarioContext *context, Character *character) {
    g_uiRegisteredCallbackRequests[2] |= 0x8000U;
    ForkliftWork *const fork = FLMW_GetWork(character);
    SelectCharacterMapQuery(character);
    const std::int32_t steerAngle = fork->driveSpeed >= 0.0f ? -12 * fork->steerOffsetAngle / 16 : -fork->steerOffsetAngle;
    if (!fork->interactionProfile)
        flmwUpdateObstacleAndFollowerCollision(context, character, fork);
    flmwSetMotionDeltaFromThrottleAndSteer(context, character, fork->driveSpeed, steerAngle);
    if (fork->followerLoadNearCab || fork->manualCameraProbe) {
        if (fork->helperActorId0.name) {
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(2 * fork->steerOffsetAngle));
            const Matrix4x4 &m = workTransformMatrix;
            const Vector3 wheelRim{((m.rows[0].x * 0.0f + m.rows[1].x * 0.0f) - m.rows[2].x * 0.18f) + m.rows[3].x,
                ((m.rows[0].y * 0.0f + m.rows[1].y * 0.0f) - m.rows[2].y * 0.18f) + m.rows[3].y,
                ((m.rows[0].z * 0.0f + m.rows[1].z * 0.0f) - m.rows[2].z * 0.18f) + m.rows[3].z};
            Matrix4x4::RotateAroundX(sin_fixed16(10922), sin_fixed16(5462));
            Vector3 grip{};
            Matrix4x4::TransformPoint(wheelRim, grip);
            Matrix4x4::Pop(1);
            Character *const leftHelper = SCEN_FindCharacterByFunctionId(context, fork->helperActorId0.name);
            if (leftHelper) {
                leftHelper->entity.node.position = {static_cast<float>(static_cast<double>(grip.x) - 0.03),
                    static_cast<float>(static_cast<double>(grip.y) + 1.2), static_cast<float>(static_cast<double>(grip.z) + 0.02)};
                leftHelper->entity.node.rotation = {17294 - std::abs(fork->steerOffsetAngle), 0xe71c, 0};
                if (CharacterComponentNode *const component = HLib_GetCharacterComponent(leftHelper, kHelperLeft)) {
                    if (Mt5RuntimeNode *const hand = reinterpret_cast<HandModelWork *>(component + 1)->primaryRoot) {
                        const std::int32_t steer = fork->steerOffsetAngle;
                        hand->rotation.y = 0;
                        hand->rotation.z = 0;
                        hand->rotation.x = -(steer * ((steer >> 31) + 4));
                    }
                }
            }
            if (g_nAuthorState != 0x11)
                HideCharacterMotion(leftHelper);
        }
        if (fork->helperActorId1.name) {
            Character *const rightHelper = SCEN_FindCharacterByFunctionId(context, fork->helperActorId1.name);
            if (rightHelper) {
                rightHelper->entity.node.position = {0.2729f, 1.1f, -0.0587f};
                rightHelper->entity.node.rotation = {51882, 31857, 0};
            }
            if (g_nAuthorState != 0x11)
                HideCharacterMotion(rightHelper);
        }
    }
    if (fork->forwardSoundBankId >= 0 && fork->reverseSoundBankId >= 0) {
        const float scaledThrottle = fork->throttleDelta * (fork->inputMask ? 123.0f : 127.0f);
        const std::int32_t inputBias = fork->inputMask ? 4 : 0;
        std::int32_t forwardVolume = static_cast<std::int32_t>(scaledThrottle / fork->tuning->maxForwardSpeed);
        if (forwardVolume < 0)
            forwardVolume = -forwardVolume;
        forwardVolume += inputBias;
        std::int32_t reverseVolume = static_cast<std::int32_t>(scaledThrottle / fork->tuning->maxReverseSpeed);
        if (reverseVolume < 0)
            reverseVolume = -reverseVolume;
        reverseVolume += inputBias;
        if (fork->throttleDelta >= 0.0f) {
            if (fork->activeDriveAudioBank != DRIVE_AUDIO_FORWARD_BANK) {
                if (fork->ownerCharacter.name == kAkira)
                    shenExecuteAICACommand(0xe04a9U, 0, 0);
                shenExecuteAICACommand(kEnvReleaseVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), 0);
            }
            if (forwardVolume != static_cast<std::int8_t>(fork->forwardBankVolumeCache)) {
                shenExecuteAICACommand(kEnvReleaseVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), forwardVolume);
                fork->forwardBankVolumeCache = static_cast<std::uint8_t>(forwardVolume);
            }
            fork->activeDriveAudioBank = DRIVE_AUDIO_FORWARD_BANK;
            fork->reverseBankVolumeCache = 0;
        } else {
            if (fork->activeDriveAudioBank == DRIVE_AUDIO_FORWARD_BANK) {
                if (fork->ownerCharacter.name == kAkira)
                    shenExecuteAICACommand(0xd04a9U, 0, 0);
                shenExecuteAICACommand(kEnvReleaseVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), 0);
            }
            if (reverseVolume != static_cast<std::int8_t>(fork->reverseBankVolumeCache)) {
                shenExecuteAICACommand(kEnvReleaseVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), reverseVolume);
                fork->reverseBankVolumeCache = static_cast<std::uint8_t>(reverseVolume);
            }
            fork->activeDriveAudioBank = DRIVE_AUDIO_REVERSE_BANK;
            fork->forwardBankVolumeCache = 0;
        }
    }
    flmwApplyMotionDeltaToActor(character, fork);
    if (fork->interactionProfile)
        flmwApplyForkliftSpacingAssist(context, character);
}


// @verified - p
void flmwHandleInteractionState(ScenarioContext *context, Character *character) {
    static constexpr std::int16_t kForkliftItems[5]{43, 44, 45, 46, 47};
    std::int16_t result = FORK_UPDATE_OK;
    ForkliftWork *const fork = FLMW_GetWork(character);
    if (fork->interactionProfile) {
        if ((g_inputState.slots[0].pressedPriority & 4U) != 0 && fork->interactionState) {
            fork->interactionState = INTERACTION_INACTIVE;
            UI_ICLISetSharedItemPreview(48);
            flmwSetActorVisibility(context, fork->helperActorId0.name, 0);
            flmwSetActorVisibility(context, fork->helperActorId1.name, 0);
            shenExecuteAICACommand(0x10a0U, 4, 0);
        }
        if (!UI_ICLISharedItemMenuIsActive()) {
            fork->interactionState = INTERACTION_ACTIVE;
            SetNewAuthorGameState(0);
            flmwSetActorVisibility(context, fork->helperActorId0.name, 1);
            flmwSetActorVisibility(context, fork->helperActorId1.name, 1);
            shenExecuteAICACommand(0x10a0U, 4, 127);
        }
    }
    if ((Shenmue_nPauseState & 2) == 0 && fork->interactionState) {
        std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
        if (fork->suppressActionHotkeys || !fork->inputMask)
            pressed = 0;
        if ((pressed & 4U) != 0 && fork->interactionPendingState == INTERACT_PENDING_NONE) {
            for (const std::int16_t item : kForkliftItems) {
                if (g_debugForceShowAllItems != 1 && !GetInventoryItemCount(static_cast<std::uint16_t>(item)))
                    continue;
                UI_ICLISetSharedItemPreview(item);
                fork->interactionPendingState = INTERACT_PENDING_ACTION_RETURN;
                flmwSetActorVisibility(context, fork->helperActorId0.name, 0);
                flmwSetActorVisibility(context, fork->helperActorId1.name, 0);
                shenExecuteAICACommand(0x10a0U, 4, 0);
                shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), 63);
                shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), 63);
                break;
            }
        }
        if ((pressed & 2U) != 0 && fork->interactionPendingState == INTERACT_PENDING_NONE) {
            UI_ICONRequestSystemMenu();
            fork->interactionPendingState = INTERACT_PENDING_CANCEL_RETURN;
            flmwSetActorVisibility(context, fork->helperActorId0.name, 0);
            flmwSetActorVisibility(context, fork->helperActorId1.name, 0);
            shenExecuteAICACommand(0x10a0U, 4, 0);
            shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), 63);
            shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), 63);
        }
        if (fork->interactionPendingState) {
            const bool returned =
                fork->interactionPendingState == INTERACT_PENDING_CANCEL_RETURN ? g_isPlayerGameStateActive == 7 : !UI_ICLISharedItemMenuIsActive();
            if (returned) {
                g_previousHelpTokenId = g_currentHelpTokenId;
                g_defaultHelpTokenId = kHelpTokenForklift;
                g_currentHelpTokenId = kHelpTokenForklift;
                g_helpButtonId = 0;
                g_currentHelpIconState = 0;
                fork->interactionPendingState = INTERACT_PENDING_NONE;
                SetNewAuthorGameState(0);
                flmwSetHelperActorVisibility(context, fork, 1);
                shenSendAICAParamCommands(fork, 127, 116);
            }
        } else {
            result = flmwUpdateDriveLiftAndCarryState(context, character);
            flmwUpdateMotionAndHelperActors(context, character);
        }
    }
    SetResult(context, static_cast<std::uint32_t>(static_cast<std::int32_t>(result)));
}



// @verified - p
void flmwUpdateForkliftProximityState(ScenarioContext *context) {
    bool playerBlocked = false;
    for (std::int32_t index = 0; index < 5; ++index) {
        const std::uint32_t name = kRaceForklifts[index];
        Character *const character = SCEN_FindCharacterByFunctionId(context, name);
        ForkliftWork *const fork = FLMW_GetWork(character);
        SelectCharacterMapQuery(character);
        Vector3 position = character ? character->entity.node.position : Vector3{};
        std::int16_t yaw = SumEntityYaw(character);
        const std::array<Vector3, 4> corners = CycleManager::BuildForkliftSteeringQuad(position, yaw);
        QueryShape query{.mode = OrientedBox_OrientedRectXZ_IsPointInTriangle};
        query.packed[0] = {corners[0].x, corners[0].z, corners[3].x - corners[0].x};
        query.packed[1] = {corners[3].z - corners[0].z, corners[1].x - corners[0].x, corners[1].z - corners[0].z};
        auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        if (ExecuteMathFnWrapper(&query, reinterpret_cast<const std::uint8_t *>(control->cycleMask))) {
            if (static_cast<std::int8_t>(fork->sideMode) > 60)
                continue;
            fork->sideMode = static_cast<std::uint8_t>(index ? fork->sideMode + 1 : 30);
            const auto throttle = static_cast<double>(fork->throttleDelta);
            if ((throttle > 0.04 || -0.02 > throttle) && fork->ownerCharacter.name == kAkira)
                shenExecuteAICACommand(0x1004a9U, 0, 0);
            if (name == kPlayerForklift) {
                playerBlocked = true;
            } else {
                fork->driveSpeed = 0.0f;
                fork->throttleDelta = 0.0f;
                fork->moveDeltaX = 0.0f;
                fork->moveDeltaZ = 0.0f;
                position = {fork->sourcePositionX, 0.0f, fork->sourcePositionZ};
                yaw = static_cast<std::int16_t>(fork->sourceYawFixed16);
                if (character) {
                    character->entity.node.position = position;
                    character->entity.node.rotation.y = yaw;
                }
            }
            if (index)
                continue;
        } else if (index) {
            fork->sideMode = 0;
            continue;
        } else if (fork->sideMode) {
            --fork->sideMode;
        }
        g_flmwPlayerForkProximityFrames = fork->sideMode;
    }

    Character *const player = SCEN_FindCharacterByFunctionId(context, kPlayerForklift);
    ForkliftWork *const playerFork = FLMW_GetWork(player);
    const Vector3 position = player ? player->entity.node.position : Vector3{};
    static_cast<void>(CycleManager::BuildForkliftSteeringQuad(position, SumEntityYaw(player)));
    if (playerBlocked) {
        const std::uint32_t frame = GetFramesPastCopy();
        if (!g_flmwRollbackActive) {
            g_flmwRollbackActive = 1;
            g_flmwRollbackExpiryFrame = frame + 2;
            playerFork->driveSpeed = playerFork->driveSpeed * 0.5f;
        }
        if (!g_flmwRollbackActive || frame <= g_flmwRollbackExpiryFrame)
            return;
        playerFork->moveYawFixed16 = g_flmwRollbackYawSumFixed16;
        playerFork->driveSpeed = 0.0f;
        playerFork->moveDeltaX = 0.0f;
        playerFork->moveDeltaZ = 0.0f;
        if (player) {
            player->entity.node.position = g_flmwRollbackPos;
            player->entity.node.rotation.y = static_cast<std::int16_t>(g_flmwRollbackYawSumFixed16);
        }
        g_flmwRollbackActive = 0;
        return;
    }
    if (g_flmwRollbackActive) {
        if (GetFramesPastCopy() <= g_flmwRollbackExpiryFrame)
            return;
        playerFork->driveSpeed = playerFork->driveSpeed * 0.5f;
        playerFork->moveDeltaX = 0.0f;
        playerFork->moveDeltaZ = 0.0f;
        if (0.03f > playerFork->driveSpeed)
            g_flmwRollbackActive = 0;
        return;
    }
    if (player) {
        g_flmwRollbackPos = GetPositionForEntity(player->entity.node);
        g_flmwRollbackYawSumFixed16 = SumEntityYaw(player);
    } else {
        g_flmwRollbackPos = {};
        g_flmwRollbackYawSumFixed16 = 0;
    }
}


// @verified - p
std::int32_t CalculateAngleBetweenPoints(Character *character, std::int32_t current, std::int32_t count, const Vector2 *points) {
    const Vector2 *point = points;
    std::int32_t previous = count - 1;
    if (current) {
        point = &points[current];
        previous = current - 1;
    }
    const float segmentX = point->x - points[previous].x;
    const float segmentZ = point->y - points[previous].y;
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    const float pointX = point->x - position.x;
    const float pointZ = point->y - position.z;
    const float dot = pointZ * segmentZ + pointX * segmentX;
    float segmentLength = std::sqrt(segmentZ * segmentZ + segmentX * segmentX);
    float pointLength = std::sqrt(pointZ * pointZ + pointX * pointX);
    if (0.001 > static_cast<double>(segmentLength))
        segmentLength = 0.001f;
    if (0.001 > static_cast<double>(pointLength))
        pointLength = 0.001f;
    const float alignment = dot / (pointLength * segmentLength);
    if (static_cast<double>(alignment) > 0.7071 && 1.0f > alignment)
        return current;
    return current == count - 1 ? 0 : current + 1;
}


// @verified - p
std::uint32_t FindNearestPointIndex(Character *character, std::int32_t route, std::int32_t volume, const Vector2 *points) {
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    std::int32_t second = 1;
    std::int32_t nearest = 1;
    std::int32_t index = 1;
    float secondDistance = 1000000.0f;
    float nearestDistance = 1000000.0f;
    const Vector2 *point = points;
    bool nearestIsFirst = true;
    if (10000.0f > point->x) {
        do {
            const float dx = point->x - position.x;
            const float dz = point->y - position.z;
            const float distance = dx * dx + dz * dz;
            if (nearestDistance > distance) {
                second = nearest;
                secondDistance = nearestDistance;
                nearest = index;
                nearestDistance = distance;
            } else if (secondDistance > distance) {
                second = index;
                secondDistance = distance;
            }
            ++point;
            ++index;
        } while (10000.0f > point->x);
        nearestIsFirst = nearest == 1;
    }
    if (nearestIsFirst) {
        const float dx = point[1].x - position.x;
        const float dz = point[1].y - position.z;
        if (secondDistance > dx * dx + dz * dz)
            second = 0;
    } else if (nearest == index - 1) {
        const float dx = point[2].x - position.x;
        const float dz = point[2].y - position.z;
        if (secondDistance > dx * dx + dz * dz)
            second = nearest + 1;
    }
    return static_cast<std::uint32_t>((((route << 8) | volume) << 16) | (nearest << 8) | second);
}


// @verified - p
float normalizeDifference(const float *start, const float *finish) {
    double finishDistance = static_cast<double>(*finish) - -64.6;
    if (0.0 > finishDistance)
        finishDistance = -finishDistance;
    const auto finishOffset = static_cast<float>(finishDistance);
    double startDistance = -64.6 - static_cast<double>(*start);
    if (0.0 > startDistance)
        startDistance = -startDistance;
    const auto startOffset = static_cast<float>(startDistance);
    float total = startOffset + finishOffset;
    if (0.000001 > static_cast<double>(total))
        total = 1e-07f;
    return static_cast<float>(static_cast<double>(startOffset / total) * 0.03333333);
}


// @verified - p
void PlaySoundAtDistance(ScenarioContext *context) {
    Character *const player = SCEN_FindCharacterByFunctionId(context, kPlayerForklift);
    const Vector3 source = player ? player->entity.node.position : Vector3{};
    Character *nearest = nullptr;
    Vector3 position{};
    float nearestDistance = 1000000.0f;
    for (std::size_t index = 1; index < 5; ++index) {
        Character *const candidate = SCEN_FindCharacterByFunctionId(context, kRaceForklifts[index]);
        const Vector3 candidatePosition = candidate ? candidate->entity.node.position : Vector3{};
        const float dy = source.y - candidatePosition.y;
        const float dx = source.x - candidatePosition.x;
        const float dz = source.z - candidatePosition.z;
        const float distance = (dy * dy + dx * dx) + dz * dz;
        if (nearestDistance > distance) {
            position = candidatePosition;
            nearest = candidate;
            nearestDistance = distance;
        }
    }
    const float throttle = FLMW_GetWork(nearest)->throttleDelta;
    double volume = static_cast<double>(throttle) * 123.0 / 0.3055;
    if (static_cast<std::int32_t>(volume) < 0)
        volume = static_cast<double>(throttle) * -123.0 / 0.3055;
    const auto releaseVolume = static_cast<std::uint32_t>(static_cast<std::int32_t>(volume) + 4);
    Execute3DSoundCommand(0xffffffffU, static_cast<std::uint32_t>(g_flmwGlobalAicaBankId), &position, 5.0f, 20.0f, releaseVolume);
}

VehicleWork *GetVehicleWork(Character *character) {
    if (!character)
        return nullptr;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kVehicleComponent);
    return component ? reinterpret_cast<VehicleWork *>(component + 1) : nullptr;
}


// @verified - p
bool PointInTriXZ(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &point) {
    const float first = (b.x - a.x) * (point.z - a.z) - (point.x - a.x) * (b.z - a.z);
    const float second = (c.x - b.x) * (point.z - b.z) - (c.z - b.z) * (point.x - b.x);
    const float third = (point.z - c.z) * (a.x - c.x) - (a.z - c.z) * (point.x - c.x);
    if (!(first <= 0.0f))
        return second >= 0.0f && third >= 0.0f;
    if (second > 0.0f)
        return first >= 0.0f && third >= 0.0f;
    return third <= 0.0f;
}

bool BelowNativeEpsilon(float value) {
    return 0.001 > static_cast<double>(std::fabs(value));
}

float NormalizeInPlace(Vector3 &value) {
    const float magnitude = std::sqrt((value.x * value.x + value.y * value.y) + value.z * value.z);
    if (magnitude > 0.0f) {
        value.x = value.x * (1.0f / magnitude);
        value.y = value.y * (1.0f / magnitude);
        value.z = value.z * (1.0f / magnitude);
    }
    return magnitude;
}

bool VEHI_TestMapLine(const Vector3 &start, const Vector3 &end) {
    QueryShape query{};
    query.packed[0] = Vector3{start.x, start.z, end.x};
    query.packed[1].x = end.z;
    query.mode = CalculateLineIntersection_0;
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    return ExecuteMathFnWrapper(&query, reinterpret_cast<const std::uint8_t *>(control->cycleMask)) != 0;
}


// @verified - p
std::int32_t vehiConstrainStepTowardCenter(Vector3 &position, std::int32_t yaw, float step, const Vector2 &center) {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    Character *const ryo = static_cast<Character *>(control->ryo);
    const Vector3 ryoPosition = ryo ? ryo->entity.node.position : Vector3{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    const float yawCosine = cos_fixed16(static_cast<std::uint16_t>(yaw));
    const float yawSine = sin_fixed16(static_cast<std::int16_t>(yaw));
    Matrix4x4::RotateAroundY(yawSine, yawCosine);
    Vector3 local{0.0f, 0.0f, -step};
    Matrix4x4::TransformPoint(local, position);
    Vector3 farForward{};
    local.z = step * -45.0f;
    Matrix4x4::TransformPoint(local, farForward);
    Vector3 rearCenter{};
    local.z = step;
    Matrix4x4::TransformPoint(local, rearCenter);
    Vector3 rearRight{};
    local.x = 1.5f;
    Matrix4x4::TransformPoint(local, rearRight);
    Vector3 rearLeft{};
    local.x = -1.5f;
    Matrix4x4::TransformPoint(local, rearLeft);
    Matrix4x4::Pop(1);
    if (PointInTriXZ(farForward, rearRight, rearLeft, ryoPosition)) {
        const bool leftHalf = !PointInTriXZ(farForward, rearRight, rearCenter, ryoPosition);
        Vector3 toRyo{ryoPosition.x - position.x, ryoPosition.y - position.y, ryoPosition.z - position.z};
        bool closeToRyo = true;
        if (!BelowNativeEpsilon(toRyo.x) || !BelowNativeEpsilon(toRyo.y) || !BelowNativeEpsilon(toRyo.z))
            closeToRyo = 5.0f > NormalizeInPlace(toRyo);
        if (closeToRyo) {
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundY(yawSine, yawCosine);
            Vector3 rightProbe{};
            Vector3 leftProbe{};
            Matrix4x4::TransformPoint({3.0f, 0.0f, 0.0f}, rightProbe);
            Matrix4x4::TransformPoint({-3.0f, 0.0f, 0.0f}, leftProbe);
            Matrix4x4::Pop(1);
            SelectCharacterMapQuery(ryo);
            const bool rightBlocked = VEHI_TestMapLine(ryoPosition, {rightProbe.x + ryoPosition.x, 0.0f, rightProbe.z + ryoPosition.z});
            const bool leftBlocked = VEHI_TestMapLine(ryoPosition, {leftProbe.x + ryoPosition.x, 0.0f, leftProbe.z + ryoPosition.z});
            float scale = leftHalf ? 0.30000001f : -0.30000001f;
            if (rightBlocked != leftBlocked)
                scale = rightBlocked ? -0.30000001f : 0.30000001f;
            Vector3 lateral{scale * step, 0.0f, 0.0f};
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundY(yawSine, yawCosine);
            Matrix4x4::TransformPoint(lateral, lateral);
            Matrix4x4::Pop(1);
            position = {lateral.x + position.x, lateral.y + position.y, lateral.z + position.z};
        }
    }
    Vector3 toRyo{ryoPosition.x - position.x, ryoPosition.y - position.y, ryoPosition.z - position.z};
    float pushX = 0.0f;
    float pushZ = 0.0f;
    if (!BelowNativeEpsilon(toRyo.x) || !BelowNativeEpsilon(toRyo.y) || !BelowNativeEpsilon(toRyo.z)) {
        if (!(1.0f > NormalizeInPlace(toRyo)))
            return 0;
        pushX = toRyo.x;
        pushZ = toRyo.z;
    }
    const float centerX = ryoPosition.x - center.x;
    const float centerZ = ryoPosition.z - center.y;
    position.x = ryoPosition.x - pushX;
    position.z = ryoPosition.z - pushZ;
    return 2.25f > centerZ * centerZ + centerX * centerX ? 1 : 0;
}


// @verified - p
void setMOTNFlags_1(Character *character, std::int32_t nodeId, const Vector3 &target) {
    Vector3 point = target;
    if (character->entity.node.child)
        point = TransformPointByEntity(*character->entity.node.child, point);
    MOTM_SetTarget(character, nodeId, &point, 0x40000000U);
}

// @verified - p
void vehiUpdateRollPhaseFromStep(Mt5RuntimeNode *root, float wheelDiameter, float step, std::int32_t &rollPhase) {
    const float circumference = wheelDiameter * 3.1400001f;
    if (FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(circumference))))
        rollPhase = 0;
    else
        rollPhase += static_cast<std::int32_t>(step / circumference * 65536.0f);
    if (Mt5RuntimeNode *const rear = shenGetNodeByChildIdx(root, 23))
        rear->rotation.x = -rollPhase;
    if (Mt5RuntimeNode *const front = shenGetNodeByChildIdx(root, 18))
        front->rotation.x = -rollPhase;
}

// @verified - p
void VEHI_Control(Character *character, VehicleWork &vehicle) {
    MotionManagerWork *const manager = MOTM_GetWork(character);
    MotionInformationWork &motion = manager->motion;
    const float frameEnd = motion.frameEnd;
    if (FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(frameEnd))))
        return;
    const float playbackRate = vehicle.currentSpeed / 15.0f;
    auto *const root = static_cast<Mt5RuntimeNode *>(vehicle.attachedCharacter->model);
    if (static_cast<double>(playbackRate) <= 1.1111111111111112) {
        if (motion.currentAnimId != vehicle.rideMotionIds[0]) {
            motion.motionFlags &= ~2U;
            motion.forcedFrameTime = MOTI_GetPlayRateScaledRemain(motion);
            motion.pendingAnimId = static_cast<std::uint16_t>(vehicle.rideMotionIds[0]);
        }
    } else if (motion.currentAnimId == vehicle.rideMotionIds[0]) {
        motion.motionFlags &= ~2U;
        motion.forcedFrameTime = MOTI_GetPlayRateScaledRemain(motion);
        motion.pendingAnimId = static_cast<std::uint16_t>(vehicle.rideMotionIds[1]);
    }
    motion.playbackRate = playbackRate;
    const auto wheelPhase = static_cast<std::int32_t>(MOTI_GetPlayRateScaledRemain(motion) / frameEnd * 65536.0f);
    if (Mt5RuntimeNode *const pedals = shenGetNodeByChildIdx(root, 1)) {
        pedals->rotation.x = -wheelPhase;
        std::byte *const base = MT5_ModelBaseForNode(pedals);
        if (pedals->childOffset) {
            auto *const crank = reinterpret_cast<Mt5RuntimeNode *>(base + pedals->childOffset);
            crank->rotation.x = wheelPhase;
            if (crank->siblingOffset)
                reinterpret_cast<Mt5RuntimeNode *>(base + crank->siblingOffset)->rotation.x = wheelPhase;
        }
    }
    if (!vehicle.pathPoints || vehicle.pathCount < 2)
        return;
    switch (vehicle.state) {
    case 1:
        if (FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(vehicle.currentSpeed)))) {
            vehicle.currentSpeed = 1.0f;
        } else {
            vehicle.currentSpeed = vehicle.currentSpeed * vehicle.speedScale;
            if (vehicle.currentSpeed >= vehicle.maxSpeed) {
                vehicle.currentSpeed = vehicle.maxSpeed;
                vehicle.state = 2;
            }
        }
        break;
    case 2:
        break;
    case 3:
        vehicle.currentSpeed = vehicle.speedScale * vehicle.currentSpeed;
        if (1.0f >= vehicle.currentSpeed) {
            vehicle.currentSpeed = 0.0f;
            vehicle.state = 4;
        }
        break;
    default:
        return;
    }

    Vector3 position = character ? character->entity.node.position : Vector3{};
    const float step = vehicle.currentSpeed * 0.0092592593f;
    if (!(FLT_EPSILON > static_cast<float>(std::fabs(static_cast<double>(step))))) {
        const Vector2 &center = vehicle.pathPoints[vehicle.pathIndex];
        Vector3 toCenter{center.x - position.x, 0.0f, center.y - position.z};
        float remaining = -step;
        std::int32_t desiredYaw = 0;
        if (!BelowNativeEpsilon(toCenter.x) || !BelowNativeEpsilon(toCenter.z)) {
            remaining = NormalizeInPlace(toCenter) - step;
            desiredYaw = atan2_fixed16(-toCenter.x, -toCenter.z);
        }
        const auto yawDelta = static_cast<std::int16_t>(desiredYaw - vehicle.yawFixed16);
        std::int32_t yaw = static_cast<std::int16_t>(desiredYaw);
        if (static_cast<std::uint16_t>(yawDelta + 364) > 728U) {
            vehicle.targetYawDelta = yawDelta >> 2;
            yaw = static_cast<std::int16_t>(vehicle.yawFixed16 + static_cast<std::int16_t>(yawDelta >> 3));
        }
        std::int32_t reachedCenter = 0;
        if (vehicle.centerConstraintEnabled && ((static_cast<std::uint32_t>(g_ryoCurrentAnimationState) - 2U) & ~4U) != 0) {
            reachedCenter = vehiConstrainStepTowardCenter(position, yaw, step, center);
        } else {
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
            Matrix4x4::TransformPoint({0.0f, 0.0f, -step}, position);
            Matrix4x4::Pop(1);
        }
        position.y = mapUpdateIntersectionStateAtLocalPoint(position);
        if (character)
            character->entity.node.position = position;
        if (Character *const attached = vehicle.attachedCharacter)
            attached->entity.node.position = position;
        vehicle.yawFixed16 = yaw;
        if (1.0f > remaining || step * 18.0f > remaining || reachedCenter) {
            if (++vehicle.pathIndex >= vehicle.pathCount) {
                if (vehicle.stopAtPathEnd == 1) {
                    --vehicle.pathIndex;
                    if (step > remaining)
                        vehicle.state = 0;
                    return;
                }
                vehicle.pathIndex = 0;
            }
        }
    }

    const std::int32_t smoothingStep = (vehicle.targetYawDelta - vehicle.smoothedYawDelta) >> 3;
    if (smoothingStep) {
        vehicle.smoothedYawDelta += smoothingStep;
    } else {
        vehicle.smoothedYawDelta = vehicle.targetYawDelta;
        vehicle.targetYawDelta = 0;
    }
    const float bank = static_cast<float>(4 * vehicle.smoothedYawDelta);
    const auto leanPhase = static_cast<std::int32_t>(static_cast<double>(step * 16384.0f / 0.50925928f) / 1.8);
    Vector3i euler{0, vehicle.yawFixed16, static_cast<std::int32_t>(sin_fixed16(static_cast<std::int16_t>(leanPhase)) * bank)};
    if (Mt5RuntimeNode *const fork = shenGetNodeByChildIdx(root, 14)) {
        const float cosine = cos_fixed16(static_cast<std::uint16_t>(leanPhase));
        fork->rotation.y = static_cast<std::int32_t>(cosine * bank);
    }
    vehiUpdateRollPhaseFromStep(root, 0.69999999f, step, vehicle.rollPhaseFixed16);
    if (vehicle.riderPoseMode && BuildMOTNNodeWorldTransforms(root, position, euler, {}, 0.0f)) {
        if (vehicle.riderPoseMode != 2)
            setMOTNFlags_1(character, 17, g_vehicleRightAnkleTarget);
        if (vehicle.riderPoseMode != 3)
            setMOTNFlags_1(character, 11, g_vehicleLeftAnkleTarget);
    }
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(vehicle.yawFixed16));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(euler.z));
    const Matrix4x4 pose = workTransformMatrix;
    Matrix4x4::Pop();
    const Vector3i rotation = CHAR_EulerAnglesFromMatrix(pose);
    if (character)
        character->entity.node.rotation = rotation;
    if (Character *const attached = vehicle.attachedCharacter) {
        attached->entity.node.rotation = rotation;
        attached->entity.node.position = character ? character->entity.node.position : Vector3{};
    }
}

// @verified - p
void VEHI_Callback(Character *character, void *rawWork) {
    auto &vehicle = *static_cast<VehicleWork *>(rawWork);
    if (vehicle.state && static_cast<std::uint32_t>(vehicle.taskState) <= 2U)
        VEHI_Control(character, vehicle);
}
} // namespace

bool FLMW_IsWatchInteractionActive() {
    return g_flmwPlayerDriveActive != 0;
}

ForkliftWork *FLMW_GetWork(Character *character) {
    if (!character)
        return nullptr;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kForkliftComponent);
    return component ? reinterpret_cast<ForkliftWork *>(component + 1) : nullptr;
}


// @verified - p
void AllocFLMW(Character *character) {
    auto *const fork = static_cast<ForkliftWork *>(AllocateHeapMemoryMODL(character, kForkliftComponent, sizeof(ForkliftWork), FLMW_Callback));
    void (*shadowCallback)(Character *) = nullptr;
    g_flmwPlayerDriveActive = 0;
    g_flmwForwardInputPressed = 0;
    g_flmwSpacingAssistActive = 0;
    fork->tuning = &g_defaultForkliftProfile;
    fork->forkHeight = 0.001f;
    fork->steerOffsetAngle = 0;
    fork->driveSpeed = 0.0f;
    fork->carriedLoad = nullptr;
    fork->visualOwner = nullptr;
    fork->ownerCharacter = {};
    fork->hasAttachedLoad = 0;
    fork->forkHeightLerpDone = -1;
    fork->throttleDelta = 0.0f;
    fork->forkHeightStepRate = 0.033333335f;
    fork->stopOnGoalArrival = static_cast<ForkliftGoalArrivalMode>(0xff);
    fork->followerLoadIds = kNoFollowerList;
    fork->pendingLoadCandidateId = shen_name(kNoFollowerId);
    fork->forkHeightMoveState = FORK_MOVE_IDLE;
    fork->loadActionBlockState = LOAD_ACTION_CLEAR;
    fork->inputMask = 0xffffffffU;
    fork->loadSequencePhase = LOAD_SEQ_IDLE;
    fork->reverseSoundBankId = -1;
    fork->forwardSoundBankId = -1;
    fork->reserved0a0 = 0;
    fork->activeDriveAudioBank = DRIVE_AUDIO_FORWARD_BANK;
    fork->steerLimit = g_defaultForkliftProfile.maxSteerFixed16;
    fork->steerStep = g_defaultForkliftProfile.steerStepFixed16;
    fork->goalDriveSpeedStep = 0.0f;
    fork->sourcePositionX = 0.0f;
    fork->sourcePositionZ = 0.0f;
    fork->sourceYawFixed16 = 0;
    fork->interactionProfile = 0;
    fork->sideMode = 0;
    fork->engineEnabledFrames = 0;
    g_flmwPlayerForkProximityFrames = 0;
    g_flmwGlobalAicaBankId = -1;
    if (character) {
        shadowCallback = character->processShadowCallback;
        character->processShadowCallback = flmwProcessShadowCallback;
    }
    fork->shadowCallback = shadowCallback;
    std::memset(g_flmwNoLoadFootprintMathFnMask, 0xff, sizeof(g_flmwNoLoadFootprintMathFnMask));
    g_flmwNoLoadFootprintMathFnMask[0] = 0xdf;
}

// @verified - p
VehicleWork *VEHI_Allocate(Character *character) {
    auto *const vehicle = static_cast<VehicleWork *>(AllocateHeapMemoryMODL(character, kVehicleComponent, sizeof(VehicleWork), VEHI_Callback));
    vehicle->state = 0;
    vehicle->attachedCharacter = nullptr;
    vehicle->pathPoints = nullptr;
    vehicle->maxSpeed = 0.0f;
    vehicle->speedScale = 0.0f;
    vehicle->currentSpeed = 0.0f;
    vehicle->reserved028 = 0;
    vehicle->reserved02c = 0;
    vehicle->reserved030 = 0;
    vehicle->reserved034 = 0;
    vehicle->rollPhaseFixed16 = 0;
    vehicle->smoothedYawDelta = 0;
    vehicle->targetYawDelta = 0;
    vehicle->stopAtPathEnd = 0;
    vehicle->taskState = 0;
    vehicle->riderPoseMode = 1;
    vehicle->pathCount = 0;
    vehicle->pathIndex = 0;
    vehicle->rideMotionIds[0] = 0;
    vehicle->rideMotionIds[1] = 0xa003;
    vehicle->rideMotionIds[2] = 0;
    vehicle->rideMotionIds[3] = 0;
    vehicle->centerConstraintEnabled = 1;
    if (character)
        HLib_GetCharacterComponent(character, kVehicleComponent)->destroyCallback = std::bit_cast<void (*)(Character *, void *)>(&HLib_InertTaskCallback);
    return vehicle;
}


// @verified - p
void FLMW_Callback(Character *character, void *rawWork) {
    const auto *const fork = static_cast<ForkliftWork *>(rawWork);
    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    Mt5RuntimeNode *const steeringLink = shenGetNodeByChildIdx(model, 154);
    Mt5RuntimeNode *const wheelB = shenGetNodeByChildIdx(model, 8);
    Mt5RuntimeNode *const wheelA = shenGetNodeByChildIdx(model, 18);
    Mt5RuntimeNode *const steeringWheel = shenGetNodeByChildIdx(model, 23);
    shenGetNodeByChildIdx(model, 3)->position.y = fork->forkHeight;
    steeringLink->rotation.z = 2 * fork->steerOffsetAngle;
    wheelA->rotation.y = -fork->steerOffsetAngle;
    steeringWheel->rotation.y = -fork->steerOffsetAngle;
    const auto throttle = static_cast<double>(fork->throttleDelta);
    wheelB->rotation.x += static_cast<std::int32_t>(throttle / (static_cast<double>(fork->tuning->wheelDiameterA) * 3.14) * -65536.0);
    wheelA->rotation.x += static_cast<std::int32_t>(throttle / (static_cast<double>(fork->tuning->wheelDiameterB) * 3.14) * -65536.0);
    steeringWheel->rotation.x = wheelA->rotation.x;
}


// @verified - p
void Script_flmwAttachLoadActor(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = SCEN_FindCharacterByFunctionId(context, params[0]);
    ForkliftWork *const fork = FLMW_GetWork(character);
    if (fork->hasAttachedLoad) {
        SetResult(context, 0);
        return;
    }
    fork->hasAttachedLoad = 1;
    Character *const load = SCEN_FindCharacterByFunctionId(context, params[1]);
    fork->carriedLoad = load;
    const Vector3 position = load ? GetPositionForEntity(load->entity.node) : Vector3{};
    const auto worldYaw = static_cast<std::int16_t>(charaGetWorldYaw(fork->carriedLoad));
    load->entity.node.child = character ? &character->entity.node : nullptr;
    if (fork->carriedLoad)
        shenApplyTranslationToAll(fork->carriedLoad->entity.node, position);
    if (fork->carriedLoad)
        fork->carriedLoad->entity.node.rotation.y = YawRelativeToParents(fork->carriedLoad, worldYaw);
    ccowDisableCharaFlags(fork->carriedLoad, 7);
    SetResult(context, kScriptPending);
}


// @verified - p
void Script_flmwFinalizeLoadDetach(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    if (fork->hasAttachedLoad != 1) {
        SetResult(context, 0);
        return;
    }
    ccowEnableCharaFlags(fork->carriedLoad, 7);
    fork->hasAttachedLoad = 0;
    flmwDetachChildChainPreserveWorldYaw(fork->carriedLoad);
    SetResult(context, kScriptPending);
}


// @verified - p
void Script_forkSetForkHeightTarget(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    const float forkHeight = fork->forkHeight;
    fork->previousForkHeight = forkHeight;
    fork->forkHeightLerpDone = static_cast<std::int8_t>(params[1]);
    if (params[1] == 0)
        fork->targetForkHeight = 0.001f;
    else if (params[1] == 1)
        fork->targetForkHeight = 1.5f;
    else if (params[1] == 2)
        fork->targetForkHeight = 1.8f;
    if (forkHeight == fork->targetForkHeight) {
        fork->forkHeightLerpDone = -1;
        SetResult(context, 0);
    } else {
        SetResult(context, kScriptPending);
    }
}


// @verified - p
void Script_forkUpdateForkHeightLerp(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    if (fork->forkHeightLerpDone < 0) {
        fork->forkHeight = fork->targetForkHeight;
        SetResult(context, 0);
        return;
    }
    const float target = fork->targetForkHeight;
    if (fork->forkHeight == target)
        fork->forkHeightLerpDone = -1;
    else if (target > fork->previousForkHeight)
        flmwRaiseForkHeightTowardTarget(fork, target);
    else
        flmwLowerForkHeightTowardTarget(fork, target);
    if (fork->forkHeightLerpDone < 0) {
        fork->forkHeight = fork->targetForkHeight;
        SetResult(context, 0);
    } else {
        SetResult(context, kScriptPending);
    }
}


// @verified - p
void Script_forkAttachLoadActor(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = SCEN_FindCharacterByFunctionId(context, params[0]);
    ForkliftWork *const fork = FLMW_GetWork(character);
    const float *const goal = ScenarioPointer<const float>(static_cast<std::int32_t>(params[1]));
    fork->goalPosition.x = goal[0];
    fork->goalPosition.z = goal[2];
    fork->goalDriveMode = static_cast<ForkliftGoalDriveMode>(params[3] & 0xffU);
    if (fork->goalDriveMode == GOAL_DRIVE_ACCEL_FROM_STOP) {
        fork->goalDriveSpeedStep = std::bit_cast<float>(params[2]);
        fork->driveSpeed = 0.0f;
    } else if (fork->goalDriveMode != GOAL_DRIVE_ACCEL_FROM_CURRENT) {
        fork->driveSpeed = std::bit_cast<float>(params[2]);
    }
    fork->stopOnGoalArrival = static_cast<ForkliftGoalArrivalMode>(params[4] & 0xffU);
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    const float dx = position.x - fork->goalPosition.x;
    const float dz = position.z - fork->goalPosition.z;
    SetResult(context, 1.0f >= std::sqrt(dx * dx + dz * dz) ? 0 : kScriptPending);
}


// @verified - p
void Script_forkAdjustCharacterMovementBasedOnDistance(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = SCEN_FindCharacterByFunctionId(context, params[0]);
    ForkliftWork *const fork = FLMW_GetWork(character);
    const Vector3 start = character ? character->entity.node.position : Vector3{};
    const float deltaX = start.x - fork->goalPosition.x;
    const float deltaZ = start.z - fork->goalPosition.z;
    const auto inverseYaw = static_cast<std::uint16_t>(-(character ? character->entity.node.rotation.y : 0));
    const float sine = sin_fixed16(static_cast<std::int16_t>(inverseYaw));
    const float cosine = cos_fixed16(inverseYaw);
    const std::int32_t goalAngle = atan2_fixed16(cosine * deltaX + sine * deltaZ, cosine * deltaZ - sine * deltaX);
    const ForkliftDriveProfile *const tuning = fork->tuning;
    const std::int32_t maxSteer = tuning->maxSteerFixed16;
    if (static_cast<std::uint8_t>(fork->goalDriveMode - GOAL_DRIVE_ACCEL_FROM_STOP) <= 1U) {
        if (fork->sideMode && g_flmwPlayerForkProximityFrames) {
            fork->driveSpeed = 0.0f;
        } else {
            const float speedStep = fork->goalDriveSpeedStep;
            const float speed = speedStep + fork->driveSpeed;
            fork->driveSpeed = speed;
            if (speedStep > 0.0f) {
                float maxSpeed = 0.2778f;
                if (fork->ownerCharacter.name == SHEN_NAME(TAKE))
                    maxSpeed = 0.2963f;
                else if (fork->ownerCharacter.name == SHEN_NAME(YOHI))
                    maxSpeed = 0.2593f;
                else if (fork->ownerCharacter.name == SHEN_NAME(HTSI))
                    maxSpeed = 0.287f;
                if (speed > maxSpeed)
                    fork->driveSpeed = maxSpeed;
            } else if (0.0f > speedStep && 0.1852 > static_cast<double>(speed)) {
                fork->driveSpeed = 0.1852f;
            }
        }
    }
    const float driveSpeed = fork->driveSpeed;
    const bool steerRight = driveSpeed >= 0.0f ? goalAngle > 0x8000 && goalAngle <= 50972 : goalAngle > 0x8000 && goalAngle >= 40049;
    const bool steerLeft = driveSpeed >= 0.0f ? goalAngle <= 0x8000 && goalAngle >= 14563 : goalAngle <= 0x8000 && goalAngle < 25486;
    if (steerRight) {
        fork->steerOffsetAngle = fork->steerOffsetAngle > -maxSteer ? fork->steerOffsetAngle - tuning->steerStepFixed16 : -maxSteer;
    } else if (steerLeft) {
        fork->steerOffsetAngle = fork->steerOffsetAngle < maxSteer ? fork->steerOffsetAngle + tuning->steerStepFixed16 : maxSteer;
    } else if (driveSpeed >= 0.0f) {
        fork->steerOffsetAngle = goalAngle > 0x8000 ? -((0x10000 - goalAngle) / 2) : goalAngle / 2;
    } else {
        fork->steerOffsetAngle = goalAngle > 0x8000 ? -((goalAngle - 0x8000) / 2) : (0x8000 - goalAngle) / 2;
    }
    const Vector3 current = character ? character->entity.node.position : Vector3{};
    const float currentX = current.x - fork->goalPosition.x;
    const float currentZ = current.z - fork->goalPosition.z;
    const float distance = std::sqrt(currentX * currentX + currentZ * currentZ);
    flmwSetMotionDeltaFromThrottleAndSteer(context, character, driveSpeed, -fork->steerOffsetAngle);
    flmwApplyMotionDeltaToActor(character, fork);
    if (fork->interactionProfile) {
        flmwApplyForkliftSpacingAssist(context, character);
    } else if (fork->ownerCharacter.name == kAkira) {
        const Vector3 moved = character ? character->entity.node.position : Vector3{};
        const float movedX = moved.x - fork->goalPosition.x;
        const float movedZ = moved.z - fork->goalPosition.z;
        if (std::sqrt(movedX * movedX + movedZ * movedZ) > distance) {
            flmwSetMotionDeltaFromThrottleAndSteer(context, character, -(fork->driveSpeed + fork->driveSpeed), fork->steerOffsetAngle);
            flmwApplyMotionDeltaToActor(character, fork);
        }
        flmwUpdateAuthoredCameraBlend(context, fork);
    }
    if (1.0f >= distance) {
        if (fork->stopOnGoalArrival) {
            fork->throttleDelta = 0.0f;
            fork->driveSpeed = 0.0f;
        }
        SetResult(context, 0);
    } else {
        SetResult(context, kScriptPending);
    }
}


// @verified - p
void Script_forkSetForkHeightStepRate(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    fork->forkHeightStepRate = params[2] ? 0.033333335f : std::bit_cast<float>(params[1]);
}


// @verified - p
void Script_forkSetSoundBankDetails(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    if (!fork->visualOwner)
        return;
    ccowEnableCharaFlags(fork->visualOwner, 7);
    flmwDetachChildChainPreserveWorldYaw(fork->visualOwner);
    if (fork->ownerCharacter.name == kAkira) {
        SCEN_FindCharacterByFunctionId(context, fork->helperActorId0.name)->entity.node.child = nullptr;
        Character *const rightHelper = SCEN_FindCharacterByFunctionId(context, fork->helperActorId1.name);
        g_CameraZoomFOVModifier = 1.0f;
        g_EnableCameraHeightClamp = 0;
        g_CameraInterpolationTimer = 30;
        rightHelper->entity.node.child = nullptr;
        fork->helperActorId0 = {};
        fork->helperActorId1 = {};
        if (fork->forwardSoundBankId >= 0 && fork->forwardSoundBankId <= 15)
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(fork->forwardSoundBankId));
        if (fork->reverseSoundBankId >= 0 && fork->reverseSoundBankId <= 15)
            ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(fork->reverseSoundBankId));
        g_flmwPlayerDriveActive = 0;
        fork->reverseSoundBankId = -1;
        fork->forwardSoundBankId = -1;
        g_flmwForwardInputPressed = 0;
    }
    fork->visualOwner = nullptr;
    fork->ownerCharacter = {};
}

std::int32_t g_bCameraVehicleFlag = 0;
std::int32_t g_vehicleGlobalFlag = 1;
std::int32_t g_vehicleGlobalFlagGate = 1;

namespace {
// @verified - p
std::int32_t atan2_fixed16_safe(float x, float z) {
    if (BelowNativeEpsilon(x) && BelowNativeEpsilon(z))
        return 0;
    return atan2_fixed16(x, z);
}

// @verified - p
std::int32_t vehiAttachRiderToVehicle(Character *self, Character *rider, std::int32_t mode) {
    if (mode || !self || !rider)
        return -1;
    VehicleWork *const vehicle = GetVehicleWork(self);
    vehicle->attachedCharacter = rider;
    vehicle->taskState = 0;
    vehicle->yawFixed16 = self->entity.node.rotation.y;
    if (VehicleWork *const riderVehicle = GetVehicleWork(rider))
        riderVehicle->state = 1;
    return 0;
}

// @verified - p
std::int32_t vehiDetachRiderFromVehicle(Character *character) {
    if (!character)
        return -1;
    VehicleWork *const vehicle = GetVehicleWork(character);
    character->entity.node.rotation.y = vehicle->yawFixed16;
    if (VehicleWork *const riderVehicle = GetVehicleWork(vehicle->attachedCharacter))
        riderVehicle->state = 0;
    vehicle->attachedCharacter = nullptr;
    return 0;
}

// @verified - p
std::int32_t vehiStartFollowPathFromScript(Character *character, const Vector2 *pathPoints, std::int32_t pathCount, std::int32_t stopAtPathEnd) {
    if (!character)
        return -1;
    VehicleWork *const vehicle = GetVehicleWork(character);
    if (!vehicle->attachedCharacter)
        return -1;
    MOTM_GetWork(character)->motion.playbackRate = 0.0f;
    vehicle->pathPoints = const_cast<Vector2 *>(pathPoints);
    vehicle->pathCount = pathCount;
    vehicle->pathIndex = 1;
    vehicle->stopAtPathEnd = stopAtPathEnd;
    const Vector2 start = pathPoints[0];
    const float height = mapUpdateIntersectionStateAtLocalPoint({start.x, 0.0f, start.y});
    character->entity.node.position = {start.x, height, start.y};
    if (Character *const rider = vehicle->attachedCharacter)
        rider->entity.node.position = {start.x, height, start.y};
    if (vehicle->pathCount > 1)
        vehicle->yawFixed16 = atan2_fixed16_safe(start.x - pathPoints[vehicle->pathIndex].x, start.y - pathPoints[vehicle->pathIndex].y);
    vehicle->state = 4;
    return 0;
}

// @verified - p
std::int32_t vehiSetGaugeDecayTier(Character *character, std::uint32_t tier) {
    if (!character)
        return -1;
    FBGGWork *const work = FBGG_GetWork(character);
    if (!work)
        return 0;
    work->initializeRequested = 1;
    work->exitRequested = 0;
    if (tier < 2)
        work->gaugeDecay = std::bit_cast<float>(0x3d4ccccdU);
    else if (tier > 5)
        work->gaugeDecay = std::bit_cast<float>(0x3b449ba6U);
    else
        work->gaugeDecay = 0.050000001f - static_cast<float>(tier - 1) * 0.0099999998f;
    work->gaugeDecayTier = tier;
    return 0;
}
} // namespace

// @verified - p
void Script_vehiUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const VehicleCommandParameters *>(rawParams);
    std::int32_t result = -1;
    Character *const character = static_cast<std::uint32_t>(params->mode - 9) > 2U ? FindGlobalCharacter(params->targetId) : nullptr;
    switch (params->mode) {
    case 0:
        result = vehiAttachRiderToVehicle(character, FindGlobalCharacter(params->argument2), static_cast<std::int32_t>(params->argument3));
        break;
    case 1:
        result = vehiDetachRiderFromVehicle(character);
        break;
    case 2:
        if (!character)
            break;
        GetVehicleWork(character)->riderPoseMode = static_cast<std::int32_t>(params->argument2);
        result = 0;
        break;
    case 3:
        result = vehiStartFollowPathFromScript(character, ScenarioPointer<const Vector2>(params->argument2), static_cast<std::int32_t>(params->argument3),
            static_cast<std::int32_t>(params->argument4));
        break;
    case 4:
        if (!character)
            break;
        GetVehicleWork(character)->maxSpeed = std::bit_cast<float>(params->argument2);
        result = 0;
        break;
    case 5:
        if (!character || static_cast<std::int32_t>(params->argument2) >= 4)
            break;
        GetVehicleWork(character)->rideMotionIds[static_cast<std::int32_t>(params->argument2)] = params->argument3;
        result = 0;
        break;
    case 6: {
        if (!character || static_cast<std::int32_t>(params->argument2) >= 5)
            break;
        VehicleWork *const vehicle = GetVehicleWork(character);
        const float profile = std::bit_cast<float>(params->argument3);
        vehicle->state = static_cast<std::int32_t>(params->argument2);
        vehicle->speedScale = 1.0f;
        if (vehicle->state == 1)
            vehicle->speedScale = profile > 1.0f ? profile : 1.05f;
        else if (vehicle->state == 2)
            vehicle->currentSpeed = vehicle->maxSpeed;
        else if (vehicle->state == 3)
            vehicle->speedScale = profile >= 1.0f ? 0.94999999f : profile;
        result = 0;
        break;
    }
    case 7:
        result = character ? GetVehicleWork(character)->state : 0;
        break;
    case 8:
        if (!character)
            break;
        GetVehicleWork(character)->centerConstraintEnabled = static_cast<std::int32_t>(params->argument2);
        result = 0;
        break;
    case 9:
        if (g_bCameraVehicleFlag)
            result = 0;
        break;
    case 10:
        if (g_vehicleGlobalFlag)
            result = 0;
        break;
    case 11:
        *ScenarioPointer<float>(params->targetId) = static_cast<float>(static_cast<double>(g_fbggGauge) * 108.0);
        result = 0;
        break;
    case 12:
        result = vehiSetGaugeDecayTier(character, params->argument2);
        break;
    case 13:
        if (!character)
            break;
        result = 0;
        if (FBGGWork *const work = FBGG_GetWork(character)) {
            *ScenarioPointer<Vector3>(params->argument2) = work->lienProxyPosition;
            *ScenarioPointer<Vector3i>(params->argument3) = work->lienProxyRotation;
        }
        break;
    case 14:
        if (!character)
            break;
        result = 0;
        if (FBGGWork *const work = FBGG_GetWork(character)) {
            work->exitRequested = 1;
            std::uint32_t &best = g_playerStats.pages.motorcycleRaceBestTimeFrames;
            if (g_entryPoint != 2 && (best > g_vehicleRideFrameCount || !best))
                best = g_vehicleRideFrameCount;
        }
        break;
    case 15:
        if (!character)
            break;
        result = 0;
        if (FBGGWork *const work = FBGG_GetWork(character))
            *ScenarioPointer<Vector3>(params->argument2) = work->roadEdgeContactPosition;
        break;
    case 16:
        if (FBGGWork *const work = character ? FBGG_GetWork(character) : nullptr)
            result = static_cast<std::int32_t>(work->roadEdgeContact);
        break;
    case 17:
        if (!character)
            break;
        result = 0;
        if (FBGGWork *const work = FBGG_GetWork(character)) {
            *ScenarioPointer<Vector3>(params->argument2) = work->shadowProxyPosition;
            *ScenarioPointer<Vector3i>(params->argument3) = work->shadowProxyRotation;
        }
        break;
    default:
        break;
    }
    SetResult(context, static_cast<std::uint32_t>(result));
}

// @verified - p
void Script_flmwSetInputMask(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]))->inputMask = ~params[1];
}


// @verified - p
void Script_flmwTriggerInteraction(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    Character *const character = SCEN_FindCharacterByFunctionId(context, params[0]);
    ForkliftWork *const fork = FLMW_GetWork(character);
    if (fork->visualOwner)
        return;
    fork->ownerCharacter = shen_name(params[1]);
    fork->visualOwner = SCEN_FindCharacterByFunctionId(context, params[1]);
    ccowDisableCharaFlags(fork->visualOwner, 7);
    flmwApplyOwnerPosRot(character, fork);
    fork->helperActorId0 = {};
    fork->helperActorId1 = {};
    if (params[1] != kAkira)
        return;
    fork->goalDriveMode = GOAL_DRIVE_PLAYER_INTERACTION;
    fork->tuning = &g_playerForkliftProfile;
    fork->forkHeightStepRate = 0.016666668f;
    g_flmwPlayerDriveActive = 1;
    useAnalogInput = 0;
    g_bIsConversationCamInactive = 0;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 9;
    ApplyPendingGameState(g_previousAuthorState);
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 0;
    ApplyPendingGameState(g_previousAuthorState);
    distanceFromRyoForCamera = 2.0f;
    g_temporaryGameState = 0;
    ApplyPendingGameState(g_previousAuthorState);
    fork->interactionState = INTERACTION_ACTIVE;
    fork->steerRecenterMode = 0;
    fork->helperActorId0 = shen_name(kHelperLeft);
    fork->helperActorId1 = shen_name(kHelperRight);
    fork->cameraBlendStep = 0;
    fork->followerLoadNearCab = 0;
    fork->manualCameraProbe = 0;
    Character *const leftHelper = SCEN_FindCharacterByFunctionId(context, kHelperLeft);
    CharacterNode *const forkliftNode = character ? &character->entity.node : nullptr;
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    const auto worldYaw = static_cast<std::int16_t>(charaGetWorldYaw(character));
    leftHelper->entity.node.child = forkliftNode;
    if (leftHelper)
        SetWorldPositionAndYaw(leftHelper, position, worldYaw);
    HideCharacterMotion(SCEN_FindCharacterByFunctionId(context, fork->helperActorId0.name));
    Character *const rightHelper = SCEN_FindCharacterByFunctionId(context, fork->helperActorId1.name);
    rightHelper->entity.node.child = forkliftNode;
    if (rightHelper)
        SetWorldPositionAndYaw(rightHelper, position, worldYaw);
    HideCharacterMotion(SCEN_FindCharacterByFunctionId(context, fork->helperActorId1.name));
    g_CameraZoomFOVModifier = 1.0f;
    g_EnableCameraHeightClamp = 1;
    g_CameraInterpolationTimer = 30;
    fork->forwardSoundBankId = static_cast<std::int16_t>(ExecuteAICACommand_Wrapper(1026));
    fork->reverseSoundBankId = static_cast<std::int16_t>(ExecuteAICACommand_Wrapper(1025));
    shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), 116);
    shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), 116);
    fork->interactionPendingState = INTERACT_PENDING_NONE;
    fork->suppressActionHotkeys = 0;
}

// @verified - p
void Script_flmwHandleInteractionState(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    flmwHandleInteractionState(context, SCEN_FindCharacterByFunctionId(context, params[0]));
}


// @verified - p
void Script_flmwSetFollowerLoadIdList(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    std::uint32_t replaced = 0;
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    std::int32_t terminator = 0;
    do
        ++terminator;
    while (params[terminator] != kNoFollowerId);
    if (fork->followerLoadIds != kNoFollowerList) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fork->followerLoadIds), 0);
        replaced = 1;
    }
    auto *const ids = static_cast<shen_name *>(HLib_AllocHeapMemBlock(4 * terminator, static_cast<std::int32_t>(kFollowerLoadIdsToken)));
    fork->followerLoadIds = ids;
    for (std::int32_t index = 0; index < terminator - 1; ++index)
        ids[index] = shen_name(params[index + 1]);
    ids[terminator - 1] = shen_name(kNoFollowerId);
    SetResult(context, replaced);
}

// @verified - p
void Script_flmwFreeFollowerLoadIds(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    ForkliftWork *const fork = FLMW_GetWork(SCEN_FindCharacterByFunctionId(context, params[0]));
    if (fork->followerLoadIds == kNoFollowerList)
        return;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fork->followerLoadIds), 0);
    fork->followerLoadIds = kNoFollowerList;
}


// @verified - p
void Script_ForkliftUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ForkliftUtilsParameters *>(rawParams);
    switch (params->mode) {
    case FLMW_MODE_RETURN_ARG:
        SetResult(context, static_cast<std::uint32_t>(params->command));
        return;
    case FLMW_MODE_SET_GLOBAL_FLAG:
        g_flmwPlayerDriveActive = 1;
        return;
    case FLMW_MODE_CLEAR_GLOBAL_FLAGS:
        g_flmwPlayerDriveActive = 0;
        g_flmwForwardInputPressed = 0;
        return;
    case FLMW_MODE_GET_GLOBAL_FLAG:
        SetResult(context, static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(g_flmwPlayerDriveActive))));
        return;
    case FLMW_MODE_CLEAR_FOLLOWER_SNAPSHOT:
        for (FollowerLoadSnapshot &snapshot : g_followerLoadFogBlock.followerLoadSnapshots)
            snapshot.position.y = -1000.0f;
        return;
    default:
        break;
    }

    Character *const character = SCEN_FindCharacterByFunctionId(context, static_cast<std::uint32_t>(params->mode));
    ForkliftWork *const fork = FLMW_GetWork(character);
    switch (params->command) {
    case FORK_CMD_SOUND_CONTROL:
        if (params->argument0 == 0) {
            g_flmwGlobalAicaBankId = static_cast<std::int16_t>(ExecuteAICACommand_Wrapper(0x402));
            if (g_flmwGlobalAicaBankId >= 0)
                shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(g_flmwGlobalAicaBankId), 127);
        } else if (params->argument0 == 1) {
            if (g_flmwGlobalAicaBankId >= 0)
                ReleaseEnvironmentAicaBank(static_cast<std::uint32_t>(g_flmwGlobalAicaBankId));
            g_flmwGlobalAicaBankId = -1;
        } else if (params->argument0 == 2) {
            PlaySoundAtDistance(context);
        } else if (params->argument0 == 3) {
            if (g_flmwGlobalAicaBankId >= 0)
                shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(g_flmwGlobalAicaBankId), params->argument1);
        } else if (params->argument0 == 4 && fork->forwardSoundBankId >= 0 && fork->reverseSoundBankId >= 0) {
            shenExecuteAICACommand(kEnvReleaseVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), 4);
            shenExecuteAICACommand(kEnvReleaseVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), 0);
        }
        return;
    case FORK_CMD_CALC_START_LINE_CROSSING_TIME:
        SetResult(context,
            std::bit_cast<std::uint32_t>(
                normalizeDifference(ScenarioPointer<const float>(params->argument0), ScenarioPointer<const float>(params->argument1))));
        return;
    case FORK_CMD_UPDATE_PROXIMITY_STATE:
        flmwUpdateForkliftProximityState(context);
        return;
    case FORK_CMD_DISTANCE_TO_POINT_LIST_SQ: {
        const Vector2 *point = ScenarioPointer<const Vector2>(params->argument0);
        const Vector3 position = character ? character->entity.node.position : Vector3{};
        float nearest = 1000000.0f;
        for (; 10000.0f > point->x; ++point) {
            const float dx = point->x - position.x;
            const float dz = point->y - position.z;
            const float distance = dx * dx + dz * dz;
            nearest = distance < nearest ? distance : nearest;
        }
        SetResult(context, std::bit_cast<std::uint32_t>(nearest));
        return;
    }
    case FORK_CMD_FIND_NEAREST_POINT_INDEX:
        SetResult(context, FindNearestPointIndex(character, params->argument0, params->argument1, ScenarioPointer<const Vector2>(params->argument2)));
        return;
    case FORK_CMD_GET_REGION_INDEX: {
        const Vector3 position = character ? character->entity.node.position : Vector3{};
        std::int32_t region = -1;
        for (std::int32_t index = 0; index < 9; ++index) {
            const ForkliftRegion &bounds = g_forkliftRaceRegions[index];
            if (position.x >= bounds.minX && bounds.maxX >= position.x && position.z >= bounds.minZ && bounds.maxZ >= position.z) {
                region = index;
                break;
            }
        }
        SetResult(context, static_cast<std::uint32_t>(region));
        return;
    }
    case FORK_CMD_SET_GOAL_SPEED_STEP:
        fork->goalDriveSpeedStep = std::bit_cast<float>(params->argument0);
        return;
    case FORK_CMD_SET_INTERACTION_PROFILE:
        fork->interactionProfile = static_cast<std::uint8_t>(params->argument0);
        return;
    case FORK_CMD_CALC_ANGLE_BETWEEN_POINTS: {
        const Vector2 *const points = ScenarioPointer<const Vector2>(params->argument2);
        SetResult(context, static_cast<std::uint32_t>(CalculateAngleBetweenPoints(character, params->argument0, params->argument1, points)));
        return;
    }
    case FORK_CMD_CLEAR_SAVED_LOAD_STATE:
        g_flmwSavedLoadSequencePhase = -1;
        return;
    case FORK_CMD_RESTORE_LOAD_STATE:
        if (g_flmwSavedLoadSequencePhase < 0) {
            fork->loadSequencePhase = LOAD_SEQ_IDLE;
            fork->forkHeight = 0.001f;
            SetResult(context, 0);
            return;
        }
        if (character)
            SetWorldPositionAndYaw(character, g_flmwSavedForkliftPosition, static_cast<std::int16_t>(g_flmwSavedForkliftYaw));
        fork->loadSequencePhase = static_cast<ForkliftLoadSequencePhase>(static_cast<std::uint8_t>(g_flmwSavedLoadSequencePhase));
        if (fork->loadSequencePhase == LOAD_SEQ_IDLE)
            fork->forkHeight = 0.001f;
        else if (fork->loadSequencePhase == LOAD_SEQ_CARRIED_RAISED_WAIT_DROP)
            fork->forkHeight = 1.8f;
        else
            fork->forkHeight = 1.5f;
        if (g_flmwSavedLoadId.name) {
            if (Character *const load = SCEN_FindCharacterByFunctionId(context, g_flmwSavedLoadId.name))
                SetWorldPositionAndYaw(load, g_flmwSavedLoadPosition, static_cast<std::int16_t>(g_flmwSavedLoadYaw));
            fork->pendingLoadCandidateId = g_flmwSavedLoadId;
            forkAttachSelectedLoad(context, character, fork);
        }
        SetResult(context, 1);
        return;
    case FORK_CMD_SAVE_LOAD_STATE:
        g_flmwSavedForkliftPosition = character ? GetPositionForEntity(character->entity.node) : Vector3{};
        g_flmwSavedLoadId = fork->attachedLoadId;
        g_flmwSavedForkliftYaw = SumEntityYaw(character);
        g_flmwSavedLoadSequencePhase = static_cast<std::int8_t>(fork->loadSequencePhase);
        if (fork->carriedLoad) {
            g_flmwSavedLoadPosition = GetPositionForEntity(fork->carriedLoad->entity.node);
            g_flmwSavedLoadYaw = SumEntityYaw(fork->carriedLoad);
        }
        return;
    case FORK_CMD_GET_ATTACHED_LOAD_ID:
        SetResult(context, fork->attachedLoadId.name);
        return;
    case FORK_CMD_ENGINE_BANK_ENABLE: {
        if (params->argument0 < 0) {
            SetResult(context, static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(fork->engineEnabledFrames))));
            return;
        }
        fork->engineEnabledFrames = static_cast<std::uint8_t>(params->argument0);
        const std::int32_t volume = fork->engineEnabledFrames ? 20 : 0;
        shenExecuteAICACommand(0x40a5U, static_cast<std::uint32_t>(fork->forwardSoundBankId), volume);
        shenExecuteAICACommand(0x40a5U, static_cast<std::uint32_t>(fork->reverseSoundBankId), volume);
        return;
    }
    case FORK_CMD_SET_ENGINE_AUDIO_VOLUME:
        if (fork->forwardSoundBankId >= 0)
            shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->forwardSoundBankId), params->argument0);
        if (fork->reverseSoundBankId >= 0)
            shenExecuteAICACommand(kEnvSetVolume, static_cast<std::uint32_t>(fork->reverseSoundBankId), params->argument0);
        return;
    case FORK_CMD_GET_THROTTLE_DELTA:
        SetResult(context, std::bit_cast<std::uint32_t>(fork->throttleDelta));
        return;
    case FORK_CMD_GET_SET_STEER_OFFSET: {
        const std::int32_t previous = fork->steerOffsetAngle;
        if (params->argument0)
            fork->steerOffsetAngle = params->argument0;
        SetResult(context, static_cast<std::uint32_t>(previous));
        return;
    }
    case FORK_CMD_SET_INTERACTION_STATE:
        fork->interactionState = static_cast<ForkliftInteractionState>(params->argument0 & 0xff);
        return;
    case FORK_CMD_RESTORE_FOLLOWER_LOADS:
        if (fork->followerLoadIds == kNoFollowerList || fork->followerLoadIds[0].name == kNoFollowerId)
            return;
        for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
            const FollowerLoadSnapshot &snapshot = g_followerLoadFogBlock.followerLoadSnapshots[index];
            if (!(snapshot.position.y > -300.0f))
                continue;
            Character *const load = SCEN_FindCharacterByFunctionId(context, fork->followerLoadIds[index].name);
            if (!load)
                continue;
            SetWorldPositionAndYaw(load, snapshot.position, static_cast<std::int16_t>(snapshot.yaw));
            const bool wasVisible = (load->motionFlags & 0x3fU) != 0;
            load->flags.value |= CharacterFlagVisible;
            load->motionFlags |= 0x3fU;
            if (g_bDebugLogVisibility && (load->flags.value & CharacterFlagHasCpct) != 0 && !wasVisible)
                CHAR_LogNPCVisibility(load, true);
        }
        return;
    case FORK_CMD_SNAPSHOT_FOLLOWER_LOADS:
        if (fork->followerLoadIds == kNoFollowerList || fork->followerLoadIds[0].name == kNoFollowerId)
            return;
        for (std::size_t index = 0; fork->followerLoadIds[index].name != kNoFollowerId; ++index) {
            FollowerLoadSnapshot &snapshot = g_followerLoadFogBlock.followerLoadSnapshots[index];
            Character *const load = SCEN_FindCharacterByFunctionId(context, fork->followerLoadIds[index].name);
            if (load == fork->carriedLoad) {
                snapshot.position.y = -500.0f;
                continue;
            }
            snapshot.position = load ? GetPositionForEntity(load->entity.node) : Vector3{};
            snapshot.yaw = SumEntityYaw(load);
        }
        return;
    case FORK_CMD_SUPPRESS_ACTION_HOTKEYS:
        fork->suppressActionHotkeys = static_cast<std::uint8_t>(params->argument0);
        return;
    case FORK_CMD_GET_SET_FORK_HEIGHT: {
        const float previous = fork->forkHeight;
        const float requested = std::bit_cast<float>(params->argument0);
        if (requested >= 0.0f)
            fork->forkHeight = requested;
        SetResult(context, std::bit_cast<std::uint32_t>(previous));
        return;
    }
    case FORK_CMD_RESET_DRIVE_INPUTS:
        fork->steerOffsetAngle = 0;
        fork->driveSpeed = 0.0f;
        fork->throttleDelta = 0.0f;
        return;
    case FORK_CMD_GET_FORK_HEIGHT_INTERP:
        SetResult(context, static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(fork->followerLoadNearCab))));
        return;
    case FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE:
        fork->cameraBlendStep = 0;
        fork->followerLoadNearCab = 0;
        fork->manualCameraProbe = 0;
        GSettings.movementFactor = 0.25f;
        g_ryoVehicleAlphaSteps = 0;
        g_temporaryGameState = 0;
        ApplyPendingGameState(g_previousAuthorState);
        CameraYAngle = 1.1f;
        CameraDistanceFromPlayer = 4.3f;
        CameraHeightAbovePlayer = 2.32f;
        CameraHeightAbovePlayer_MAX = 2.32f;
        CAME_SetFieldOfView(10922);
        g_bIsConversationCamInactive = 0;
        flmwSetActorVisibility(context, fork->helperActorId0.name, 0);
        flmwSetActorVisibility(context, fork->helperActorId1.name, 0);
        return;
    case FORK_CMD_HIDE_HELPER_ACTORS:
        flmwSetActorVisibility(context, fork->helperActorId0.name, 0);
        flmwSetActorVisibility(context, fork->helperActorId1.name, 0);
        return;
    default:
        return;
    }
}

} // namespace shenmue::s1
