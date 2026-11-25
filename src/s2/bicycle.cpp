#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/am2_vector.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "common/native_sqrt.hpp"
#endif

#include "s2/bicycle.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/math.hpp"
#include "s2/model_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/sound_program.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cfloat>
#include <cmath>
#include <math.h>

namespace shenmue {
namespace s2 {

typedef char Layout_bicycle_player_0[(offsetof(PlayerModelState, useExternalMovementState) == 0xc) ? 1 : -1];
typedef char Layout_bicycle_player_1[(offsetof(PlayerModelState, analogHistory0) == 0x6c) ? 1 : -1];
typedef char Layout_bicycle_player_2[(offsetof(PlayerModelState, yaw) == 0xdc) ? 1 : -1];
typedef char Layout_bicycle_player_3[(offsetof(PlayerModelState, inputSnapshot) == 0x1ac) ? 1 : -1];
typedef char Layout_bicycle_player_4[(offsetof(PlayerMovementState, pendingYawCorrection) == 0x2e) ? 1 : -1];

static void ResetBicycleRideState(BicycleRideWork *bicycle);
static void ClearBicycleWheelContact(BicycleWheelContact *wheel);
static void UpdateBicycleSteering(PlayerModelState *);
static void UpdateBicyclePedalInput(BicycleRideWork *bicycle);
static void UpdateBicycleSpeed(BicycleRideWork *bicycle);
static float ComputeBicycleTurnRadius(const Vector3 *previousPosition, std::int16_t previousYaw, const Vector3 *position, std::int16_t yaw);
static void ApplyBicycleTransform(BicycleRideWork *bicycle);
static void UpdateBicycleRiderAttachment(BicycleRideWork *bicycle);
static void PlaceBicycleBehindPlayer(PlayerModelState *player);
static std::int32_t UpdateBicycleRiderMotion(PlayerModelState *player);
static void ApplyBicycleStandPitch(BicycleRideWork *bicycle);
static void UpdateBicycleRideSounds(PlayerModelState *player);
static void UpdateBicycleNodes(BicycleRideWork *bicycle);
void SetBicycleStandMode(std::int32_t standing);
static void SnapRiderLimbsToBicycle(BicycleRideWork *bicycle);
static void UpdateBicycleHandlebarYaw(BicycleRideWork *bicycle);
void BicycleRideNullsub();

namespace player_detail {
std::int32_t TryRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested);
float SimdSolveVectorProjection(Vector3 &output, const Vector3 &base, const Vector3 &direction, const Vector3 &reference, const Vector3 &crossVector);
} // namespace player_detail

namespace model_component_detail {
float Vector3_ComputeSquaredDistance(const Vector3 &left, const Vector3 &right);
} // namespace model_component_detail

float FindFieldIntersectionHeight(const Vector3 *position);
std::uint32_t RaycastInWorldGrid(Vector3 *output, const Vector3 *position, float radius);
std::uint32_t TestFieldSphereRegionInWorld(Vector3 *output, const Vector3 *position, float radius);
bool flddHasActiveIntersection();
float FindFieldMinimumHeightAbove(const Vector3 *position);
void ReadMotiCompressed(const char *directory, const char *filename);
void ApplyCharacterNodeTransformAndScale(Character *character);
void ccmGetGroundAngles(Character *character, std::int16_t *angles);
void SetNativeGuiActive(std::int32_t active);
void GetMT7NodeWorldMatrixById(const MT7Node *root, std::int32_t id);
void GetTransformedForwardVector(Vector3 *output);
BicycleRideWork g_bicycleRideWork;
BicycleRideWork *g_bicycleRide = &g_bicycleRideWork;
static Vector3 bicycleWheelOffsets[2] = {
    {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.3124f, SHEN_FIELD(z) - 0.5907f}, {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.3124f, SHEN_FIELD(z) 0.5834f}};

std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount);

static inline float BicycleWheelBase() {
    float rear = bicycleWheelOffsets[0].z;
    if (rear < 0.0f)
        rear = -rear;
    return bicycleWheelOffsets[1].z < 0.0f ? rear - bicycleWheelOffsets[1].z : rear + bicycleWheelOffsets[1].z;
}

void InitializeBicycleRider(PlayerModelState *player) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    g_bicycleRide = bicycle;
    ResetBicycleRideState(bicycle);
    ReadMotiCompressed("misc", "PLYBCYL.MOT");
    Character *const bike = AddCharacterHandler(3);
    if (!bike)
        return;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(control->scenarioTask, shen_name(BICYCLE_CHARACTER_NAME), bike);
    bike->model = InitializeMT7Model(shenLoadFile("misc", "BIKE7SAG.MT7", shen_name(0)))->root;
    SetCharacterAlpha(bike, 1.0f);
    EV_SetCharDisplay(bike, static_cast<CharaVisibilityMode>(3), 1);
    bicycle->bike = bike;
    SetCharacterType(bike, 7);
    bike->shadowModel = bike->model;
    charaSetDisplayEnabledFlag(bike, 1);
    charaSetVisibilityFlagTo(bike, 1);
    GetCharacterWorldPosition(player->character, &bicycle->position);
    bicycle->rotation.y = static_cast<std::int16_t>(GetCharacterWorldYaw(player->character) + 0x4000);
    SetCharacterWorldPosition(bike, &bicycle->position);
    SetCharacterWorldYaw(bike, bicycle->rotation.y);
    AllocateCCM(bike);
    ccmClearFlags(bike, 14);
    SetBicycleStandMode(1);
    MT7Node *const lamp = FindMT7Node(bicycle->bike->model, 0xc0);
    bicycle->savedLampChild = lamp;
    if (lamp)
        bicycle->savedLampChild = lamp->child;
}

static void ResetBicycleRideState(BicycleRideWork *bicycle) {
    bicycle->flags = 0;
    bicycle->mounted = 0;
    bicycle->state = 0;
    bicycle->retained06 = 0;
    bicycle->previousState = -1;
    bicycle->position.x = 0.0f;
    bicycle->position.y = 0.0f;
    bicycle->position.z = 0.0f;
    bicycle->rotation.x = 0;
    bicycle->rotation.y = 0;
    bicycle->rotation.z = 0;
    bicycle->retained14[0] = 0.0f;
    bicycle->retained14[1] = 0.0f;
    bicycle->retained14[2] = 0.0f;
    bicycle->retained14[3] = 0.0f;
    bicycle->retained14[4] = 0.0f;
    bicycle->retained14[5] = 0.0f;
    bicycle->speed = 0.0f;
    bicycle->bike = 0;
    bicycle->pedalAngle = 0;
    bicycle->retained8e = 0;
    bicycle->standPitch = 0;
    bicycle->pedalForce = 0.0f;
    bicycle->brakeForce = 0.0f;
    bicycle->rollingResistance = 0.0f;
    bicycle->slopeForce = 0.0f;
    bicycle->standLift = 0.0f;
    bicycle->standBlend = 0.0f;
    bicycle->standVelocity = 0.0f;
    bicycle->handlebarYaw = 0;
    bicycle->handlebarPitch = 0;
    bicycle->riderHeight = 0.0f;
    bicycle->requestedMotion = 0;
    bicycle->currentMotion = 0;
    bicycle->pendingAction = 0;
    bicycle->idleFrames = 0;
    bicycle->rear = &bicycle->rearWheel;
    bicycle->front = &bicycle->frontWheel;
    ClearBicycleWheelContact(&bicycle->rearWheel);
    ClearBicycleWheelContact(bicycle->front);
    bicycle->flags |= 0x300U;
    bicycle->limbFlags = 0;
    bicycle->pedalCount = 0;
    bicycle->retained18a = 0;
    bicycle->savedLampChild = 0;
}

static void ClearBicycleWheelContact(BicycleWheelContact *wheel) {
    wheel->flags = 0;
    wheel->retained14 = 0.0f;
    wheel->verticalVelocity = 0.0f;
    wheel->retained1c = 0.0f;
}

static void UpdateBicycleFramePose(BicycleRideWork *bicycle) {
    float rearLength = bicycleWheelOffsets[0].z;
    if (rearLength < 0.0f)
        rearLength = -rearLength;
    float wheelBase;
    if (bicycleWheelOffsets[1].z < 0.0f)
        wheelBase = rearLength - bicycleWheelOffsets[1].z;
    else
        wheelBase = rearLength + bicycleWheelOffsets[1].z;
    Vector3 rear, front;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&bicycle->position);
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(bicycle->rotation.x);
    Matrix4x4::TransformPoint(bicycleWheelOffsets[0], rear);
    Matrix4x4::TransformPoint(bicycleWheelOffsets[1], front);
    PopCurrentMatrix(1);
    const Vector3 savedRear = rear, savedFront = front;
    BicycleWheelContact *const rearWheel = &bicycle->rearWheel;
    BicycleWheelContact *const frontWheel = &bicycle->frontWheel;
    if (bicycle->flags & 0x200U) {
        if ((rearWheel->flags & 1U) == 0)
            rearWheel->verticalVelocity -= 0.010896279f;
        if ((frontWheel->flags & 1U) == 0)
            frontWheel->verticalVelocity -= 0.010896279f;
    }
    rearWheel->flags &= ~1U;
    frontWheel->flags &= ~1U;
    Vector3 offset;
    offset.x = 0.0f;
    offset.y = rearWheel->verticalVelocity;
    offset.z = -bicycle->speed;
    const std::int16_t steer = bicycle->steer;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y + steer);
    Matrix4x4::Transform(offset, offset);
    PopCurrentMatrix(1);
    am2Vector3Add(&rear, &rear, &offset);
    offset.x = 0.0f;
    offset.y = frontWheel->verticalVelocity;
    offset.z = -(bicycle->speed * 0.0f);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
    Matrix4x4::Transform(offset, offset);
    PopCurrentMatrix(1);
    am2Vector3Add(&front, &front, &offset);
    Vector3 scaled, direction;
    am2Vector3Subtract(&direction, &front, &rear);
    am2Vector3NormalizeInPlace(&direction);
    am2Vector3Scale(&scaled, &direction, wheelBase);
    am2Vector3Add(&front, &rear, &scaled);
    std::int16_t angles[2];
    Vector3 ground;
    float height;
    std::uint32_t hit;
    if (bicycle->flags & 0x100U) {
        height = FindFieldIntersectionHeight(&rear);
        hit = TestFieldSphereRegionInWorld(&ground, &rear, 0.3119825f);
        am2Vector3Subtract(&scaled, &ground, &rear);
        if (scaled.y > 0.0f) {
            rearWheel->verticalVelocity = 0.0f;
            rearWheel->flags |= 1U;
        }
        if (hit && !flddHasActiveIntersection())
            ground.y = savedRear.y;
        if (height == -FLT_MAX) {
            ground.y = savedRear.y;
            rearWheel->verticalVelocity = 0.0f;
        }
        RaycastInWorldGrid(&ground, &ground, 0.3119825f);
        rear = ground;
    }
    if (bicycle->flags & 0x100U) {
        height = FindFieldIntersectionHeight(&front);
        hit = TestFieldSphereRegionInWorld(&ground, &front, 0.3119825f);
        am2Vector3Subtract(&scaled, &ground, &front);
        if (scaled.y > 0.0f) {
            frontWheel->verticalVelocity = 0.0f;
            frontWheel->flags |= 1U;
        }
        if (hit && !flddHasActiveIntersection())
            ground.y = savedFront.y;
        if (height == -FLT_MAX) {
            ground.y = savedFront.y;
            frontWheel->verticalVelocity = 0.0f;
        }
        RaycastInWorldGrid(&ground, &front, 0.3119825f);
        front = ground;
    }
    am2Vector3Subtract(&direction, &front, &rear);
    am2Vector3NormalizeInPlace(&direction);
    am2Vector3Scale(&scaled, &direction, wheelBase);
    am2Vector3Add(&front, &rear, &scaled);
    am2Vector3Subtract(&scaled, &rear, &front);
    Vector3_ToFixed16EulerAngles(scaled, reinterpret_cast<Vector3s &>(angles));
    bicycle->rotation.x = angles[0];
    bicycle->rotation.y = angles[1];
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&rear);
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(bicycle->rotation.x);
    Matrix4x4::preTranslate(-bicycleWheelOffsets[0].x, -bicycleWheelOffsets[0].y, -bicycleWheelOffsets[0].z);
    matrix_stack::GetTranslation(&bicycle->position);
    PopCurrentMatrix(1);
    Vector3 travelled;
    am2Vector3Subtract(&travelled, &rear, &savedRear);
    if (std::fabs(rear.y - savedRear.y) < 1.1920929e-07f)
        bicycle->speed = am2Vector3Length(&travelled);
    bicycle->speed = bicycle->speed < 0.0f || bicycle->speed < 0.2777778f ? (bicycle->speed < 0.0f ? 0.0f : bicycle->speed) : 0.2777778f;
    rearWheel->position = rear;
    frontWheel->position = front;
}

void UpdateBicycleRider(PlayerModelState *player) {
    BicycleRideWork *const bicycle = g_bicycleRide;
    const dc::InputSlot *const input = player->inputSnapshot;
    bicycle->input = input;
    bicycle->rider = player->character;
    Vector3 position;
    GetCharacterWorldPosition(bicycle->bike, &position);
    am2Vector3Subtract(&bicycle->positionDelta, &position, &bicycle->position);
    bicycle->position = position;
    Vector3i rotation;
    GetCharacterWorldRotation(bicycle->bike, &rotation);
    if (!rotation.x && !rotation.z)
        bicycle->rotation.y = static_cast<std::int16_t>(rotation.y);
    BicycleRideNullsub();
    if (!(player->flags & 0x10000U))
        return;
    const std::int32_t motionPlaced = UpdateBicycleRiderMotion(player);
    ApplyBicycleStandPitch(bicycle);
    switch (bicycle->mounted) {
    case 0:
        if (!(bicycle->flags & 0x40U) && !motionPlaced)
            UpdateBicycleSteering(player);
        break;
    case 1:
        if (!motionPlaced)
            PlaceBicycleBehindPlayer(player);
        break;
    case 2:
        break;
    }
    ApplyBicycleTransform(bicycle);
    UpdateBicycleNodes(bicycle);
    UpdateBicycleRiderAttachment(bicycle);
}

static void UpdateBicycleSteering(PlayerModelState *) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    if (bicycle->state != 3)
        return;
    UpdateBicyclePedalInput(bicycle);
    UpdateBicycleSpeed(bicycle);
    bicycle->previousPosition.x = bicycle->position.x;
    bicycle->previousPosition.y = bicycle->position.y;
    bicycle->previousPosition.z = bicycle->position.z;
    bicycle->previousRotation.x = bicycle->rotation.x;
    bicycle->previousRotation.y = bicycle->rotation.y;
    bicycle->previousRotation.z = bicycle->rotation.z;
    UpdateBicycleFramePose(bicycle);
    std::int16_t target = static_cast<std::int32_t>((bicycle->cornerForce / 0.015f) * 2730.0f);
    if (bicycle->steer < 0)
        target = -target;
    target = (target < -0xaaa ? -0xaaa : target) < 0xaaa ? (target < -0xaaa ? -0xaaa : target) : 0xaaa;
    bicycle->rotation.z = CameraLerpAngle(target, bicycle->rotation.z, 0.4f);
}

static void UpdateBicyclePedalInput(BicycleRideWork *bicycle) {
    const dc::InputSlot *const input = bicycle->input;
    const float previousBrake = bicycle->brakeInput;
    std::uint32_t pedal = 0;
    if (input->triggers[1] > 0xfa) {
        pedal = 0x20000000U;
        if (pedal & ~(bicycle->flags & 0x20000000U))
            pedal |= 0x40000000U;
    }
    bicycle->flags = pedal | (bicycle->flags & 0x9fffffffU);
    if (bicycle->flags & 0x40000000U) {
        ++bicycle->pedalCount;
        if (bicycle->pedalCount > 8)
            bicycle->pedalCount = 8;
    }
    bicycle->brakeInput = static_cast<float>(input->triggers[0]) / 255.0f;
    if (input->held & 0x200U)
        bicycle->brakeInput = 1.5f;
    if (bicycle->brakeInput > 0.0f || (input->pressedPriority & 0x200U)) {
        if (bicycle->pedalCount > 1)
            bicycle->pedalCount = 1;
        if (std::fabs(bicycle->speed) < 1.1920929e-07f)
            bicycle->pedalCount = 0;
    }
    bicycle->pedalInput = 0.0f;
    if (bicycle->pedalCount) {
        bicycle->pedalInput = 0.5f;
        if (bicycle->flags & 0x400U)
            bicycle->pedalInput = 1.0f;
    }
    bicycle->steerLimit = static_cast<std::int16_t>(static_cast<std::int32_t>((1.0f - bicycle->speed / 0.2777778f) * 4096.0f) + 0x1000);
    if (input->held & 0xc000U) {
        if (input->held & 0x4000U)
            bicycle->steer = static_cast<std::int16_t>(bicycle->steer + 0x100);
        if (static_cast<std::int16_t>(input->held) < 0)
            bicycle->steer = static_cast<std::int16_t>(bicycle->steer - 0x100);
        std::int16_t *const steer = &bicycle->steer;
        const std::int32_t minimum = -bicycle->steerLimit;
        std::int32_t value = *steer;
        if (value < minimum)
            value = minimum;
        *steer = bicycle->steerLimit > value ? value : bicycle->steerLimit;
    } else {
        bicycle->steer = CameraLerpAngle(0, bicycle->steer, 0.1f);
    }
    if (input->pressedPriority & 2U)
        Loop3DSoundEffect(0x1fd);
    if (std::fabs(previousBrake) < 1.1920929e-07f && !(std::fabs(bicycle->brakeInput) < 1.1920929e-07f))
        Loop3DSoundEffect(0x26c);
}

static void UpdateBicycleSpeed(BicycleRideWork *bicycle) {
    bicycle->turnRadius = ComputeBicycleTurnRadius(&bicycle->previousPosition, bicycle->previousRotation.y, &bicycle->position, bicycle->rotation.y);
    bicycle->pedalForce = bicycle->pedalInput;
    bicycle->brakeForce = bicycle->brakeInput * 3.0f;
    std::int16_t ground[2];
    ccmGetGroundAngles(bicycle->bike, ground);
    Vector3 forward;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(ground[1]);
    Matrix4x4::RotateAroundX_Wrapper(ground[0]);
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y - ground[1]);
    GetTransformedForwardVector(&forward);
    PopCurrentMatrix(1);
    bicycle->slopeForce = forward.y * 0.010896279f * 80.0f;
    if (!(std::fabs(bicycle->pedalInput) < 1.1920929e-07f))
        bicycle->rollingResistance = 0.0f;
    else
        bicycle->rollingResistance = 0.25f;
    float scale;
    if (std::fabs(forward.y) < 1.1920929e-07f)
        scale = 1.0f;
    else if (forward.y > 0.0f)
        scale = 1.0f;
    else {
        scale = 1.0f - (forward.y < 0.0f ? -forward.y : forward.y) * 5.0f;
        scale = (scale < 0.0f || scale < 0.75f) ? (scale < 0.0f ? 0.0f : scale) : 0.75f;
    }
    bicycle->rollingResistance *= scale;
    float force = bicycle->pedalForce - bicycle->brakeForce - bicycle->rollingResistance - bicycle->slopeForce;
    force = 1.0f > force ? force : 1.0f;
    bicycle->netForce = force;
    bicycle->acceleration = force * 0.004340278f;
    bicycle->speed += bicycle->acceleration;
    bicycle->speed = (bicycle->speed < 0.0f || bicycle->speed < 0.2777778f) ? (bicycle->speed < 0.0f ? 0.0f : bicycle->speed) : 0.2777778f;
    bicycle->cornerForce = 0.0f;
    if (!(std::fabs(bicycle->turnRadius) < 1.1920929e-07f))
        bicycle->cornerForce = (bicycle->speed * bicycle->speed) / bicycle->turnRadius;
}

static float ComputeBicycleTurnRadius(const Vector3 *previousPosition, std::int16_t previousYaw, const Vector3 *position, std::int16_t yaw) {
    const std::int16_t previousAngle = static_cast<std::int16_t>(previousYaw + 0x4000);
    const std::int16_t angle = static_cast<std::int16_t>(yaw + 0x4000);
    Vector3 previousAxis, axis, centre;
    previousAxis.x = CalculateFixedSine(static_cast<std::uint32_t>(previousAngle));
    previousAxis.y = 0.0f;
    previousAxis.z = CalculateFixedCosine(static_cast<std::uint32_t>(previousAngle));
    axis.x = CalculateFixedSine(static_cast<std::uint32_t>(angle));
    axis.y = 0.0f;
    axis.z = CalculateFixedCosine(static_cast<std::uint32_t>(angle));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (player_detail::SimdSolveVectorProjection(centre, *previousPosition, previousAxis, *position, axis) == 0.0f)
        return 1000.0f;
#else
    player_detail::SimdSolveVectorProjection(centre, *previousPosition, previousAxis, *position, axis);
    if (centre.x == previousPosition->x && centre.y == previousPosition->y && centre.z == previousPosition->z)
        return 1000.0f;
#endif
    const float x = centre.x - previousPosition->x;
    const float z = centre.z - previousPosition->z;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float radius = shenmue::NativeSqrt(x * x + z * z);
#else
    float radius = std::sqrt(x * x + z * z);
#endif
    if (radius > 1000.0f)
        radius = 1000.0f;
    return radius;
}

static void ApplyBicycleTransform(BicycleRideWork *bicycle) {
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&bicycle->position);
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(bicycle->rotation.x);
    Matrix4x4::RotateAroundZ_Wrapper(bicycle->rotation.z);
    matrix_stack::GetMatrix(&bicycle->transform);
    PopCurrentMatrix(1);
    ExtractMatrixEulerAnglesFixed(&bicycle->transform, &bicycle->worldRotation);
    SetCharacterWorldPosition(bicycle->bike, &bicycle->position);
    charaSetWorldRot(bicycle->bike, bicycle->worldRotation);
}

static void UpdateBicycleRiderAttachment(BicycleRideWork *bicycle) {
    if (bicycle->flags & 2U) {
        Vector3 seat;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&bicycle->transform);
        Matrix4x4::preTranslate(0.0f, 0.0f, 0.33f);
        Matrix4x4::preTranslate(0.0f, bicycle->riderHeight, 0.0f);
        matrix_stack::GetTranslation(&seat);
        PopCurrentMatrix(1);
        SetCharacterWorldPosition(bicycle->rider, &seat);
        charaSetWorldRot(bicycle->rider, bicycle->worldRotation);
    }
    SnapRiderLimbsToBicycle(bicycle);
    UpdateBicycleHandlebarYaw(bicycle);
}

static void PlaceBicycleBehindPlayer(PlayerModelState *player) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    if (bicycle->state == 4)
        return;
    Vector3 position, delta;
    GetCharacterWorldPosition(player->character, &position);
    GetCharacterPositionDelta(player->character, &delta);
    const std::int16_t yaw = static_cast<std::int16_t>(GetCharacterWorldYaw(player->character));
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&position);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::preTranslate(0.4f, 0.0f, -0.3f);
    matrix_stack::GetTranslation(&bicycle->position);
    PopCurrentMatrix(1);
    bicycle->rotation.y = yaw;
    bicycle->speed = am2Vector3Length(&delta);
    bicycle->steer = static_cast<std::int16_t>(yaw - bicycle->rotation.y);
}

static std::int32_t UpdateBicycleRiderMotion(PlayerModelState *player) {
    MotionWork *const motion = player->motion;
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    Vector3 rightHand, leftHand, grip, rider, span;
    Vector2s angles;
    Vector3 seat, bar, handlebar;
    Vector3 pedalRightHand, pedalLeftHand, pedalGrip, pedal, pedalSpan;
    Vector2s pedalAngles, restAngles;
    std::int32_t placed = 0;
    switch (mtwkGetAnimationId(motion, static_cast<MotionRegion>(0))) {
    case 0xfa16: {
        seat.x = 0.0f;
        seat.y = 1.1494f;
        seat.z = -0.2254f;
        bar.x = 0.0f;
        bar.y = 0.31241f;
        bar.z = 0.58337f;
        mtwkGetControllerTargetPosition(motion, 4, rightHand);
        mtwkGetControllerTargetPosition(motion, 3, leftHand);
        am2Vector3Add(&grip, &rightHand, &leftHand);
        am2Vector3Scale(&grip, &grip, 0.5f);
        am2Vector3Subtract(&span, &rightHand, &leftHand);
        Vector3_ToFixed16EulerAngles(span, reinterpret_cast<Vector3s &>(angles));
        const std::int32_t yaw = static_cast<std::int16_t>(angles.y + 0xc000);
        GetCharacterWorldPosition(player->character, &rider);
        const float reach = CalculateDistanceBetweenPoints(&seat, &bar);
        float height = grip.y - rider.y;
        if (height < 0.0f)
            height = -height;
        const std::int16_t lift = static_cast<std::int16_t>(ComputeInverseAngleRatio((height - 0.3119825f) / reach));
        am2Vector3Subtract(&span, &bar, &seat);
        Vector3_ToFixed16EulerAngles(span, reinterpret_cast<Vector3s &>(angles));
        const std::int16_t pitch = static_cast<std::int16_t>(-angles.x - lift);
        const float forward = reach * dc::sdk::SinFixed(lift);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&grip);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::preTranslate(0.0f, 0.0f, forward);
        matrix_stack::GetTranslation(&handlebar);
        PopCurrentMatrix(1);
        matrix_stack::PushUnitMatrix();
        handlebar.y = rider.y + 0.3119825f;
        matrix_stack::TranslateVector(&handlebar);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::RotateAroundX_Wrapper(pitch);
        const Vector3 *const front = &bicycleWheelOffsets[1];
        Matrix4x4::preTranslate(-front->x, -front->y, -front->z);
        matrix_stack::GetTranslation(&bicycle->position);
        PopCurrentMatrix(1);
        bicycle->rotation.x = pitch;
        bicycle->rotation.y = static_cast<std::int16_t>(yaw);
        placed = 1;
        break;
    }
    case 0xfa08:
    case 0xfa09:
    case 0xfa0a:
    case 0xfa0c:
    case 0xfa0d:
        pedalGrip.x = -0.2848f;
        pedalGrip.y = 1.1494f;
        pedalGrip.z = -0.2254f;
        pedal.x = 0.0f;
        pedal.y = 0.8491f;
        pedal.z = 0.4328f;
        mtwkGetControllerTargetPosition(motion, 4, pedalRightHand);
        mtwkGetControllerTargetPosition(motion, 3, pedalLeftHand);
        am2Vector3Subtract(&pedalSpan, &pedalGrip, &pedal);
        Vector3_ToFixed16EulerAngles(pedalSpan, reinterpret_cast<Vector3s &>(restAngles));
        am2Vector3Subtract(&pedalSpan, &pedalRightHand, &pedalLeftHand);
        Vector3_ToFixed16EulerAngles(pedalSpan, reinterpret_cast<Vector3s &>(pedalAngles));
        pedalAngles.x = static_cast<std::int16_t>(-pedalAngles.x + restAngles.x);
        pedalAngles.y = static_cast<std::int16_t>(pedalAngles.y - restAngles.y);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&pedalLeftHand);
        Matrix4x4::RotateAroundY_Wrapper(pedalAngles.y);
        Matrix4x4::RotateAroundX_Wrapper(pedalAngles.x);
        Matrix4x4::preTranslate(-pedal.x, -pedal.y, -pedal.z);
        matrix_stack::GetTranslation(&bicycle->position);
        PopCurrentMatrix(1);
        bicycle->rotation.x = pedalAngles.x;
        bicycle->rotation.y = pedalAngles.y;
        if (mtwkSetRegionFlag(motion, 65, 1, static_cast<MotionRegion>(0)))
            bicycle->rotation.x = 0;
        placed = 1;
        break;
    case 0xfa21: {
        const float frame = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
        if (!(93.0f > frame) && !(frame > 103.0f)) {
            const float blend = 1.0f - (frame - 93.0f) / 10.0f;
            bicycle->standBlend = blend;
        }
        if (frame > 95.0f) {
            bicycle->standVelocity -= 0.010896279f;
            bicycle->standLift += bicycle->standVelocity;
            if (0.0f > bicycle->standLift) {
                bicycle->standVelocity *= -0.65f;
                bicycle->standLift += bicycle->standVelocity;
                if (0.0f > bicycle->standLift)
                    bicycle->standVelocity = 0.0f;
                if (0.001f > bicycle->standVelocity) {
                    bicycle->standVelocity = 0.0f;
                    bicycle->standLift = 0.0f;
                }
            }
        }
        if (mtwkSetRegionFlag(motion, 65, 1, static_cast<MotionRegion>(0)))
            bicycle->flags &= ~0x40U;
        placed = 1;
        break;
    }
    case 0xfa22: {
        const float frame = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
        if (!(107.0f > frame) && !(frame > 120.0f)) {
            const float blend = (frame - 107.0f) / 13.0f;
            bicycle->standLift = blend * 0.1f;
            bicycle->standBlend = blend;
        }
        mtwkSetRegionFlag(motion, 65, 1, static_cast<MotionRegion>(0));
        bicycle->flags |= 0x40U;
        placed = 1;
        break;
    }
    }
    return placed;
}

static void ApplyBicycleStandPitch(BicycleRideWork *bicycle) {
    if (!(bicycle->flags & 0x40U))
        return;
    const float wheelBase = BicycleWheelBase();
    Vector3 rear;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&bicycle->position);
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(bicycle->rotation.x);
    Matrix4x4::TransformPoint(bicycleWheelOffsets[0], rear);
    PopCurrentMatrix(1);
    std::int16_t pitch = static_cast<std::int16_t>(bicycle->rotation.x - bicycle->standPitch);
    const std::int16_t stand = static_cast<std::int16_t>(-ComputeNormalizedAngleRatio(bicycle->standLift / wheelBase));
    pitch = static_cast<std::int16_t>(pitch + stand);
    bicycle->rotation.x = pitch;
    bicycle->standPitch = stand;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&rear);
    Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(bicycle->rotation.x);
    Matrix4x4::preTranslate(-bicycleWheelOffsets[0].x, -bicycleWheelOffsets[0].y, -bicycleWheelOffsets[0].z);
    matrix_stack::GetTranslation(&bicycle->position);
    PopCurrentMatrix(1);
}

static void AttachRiderToBicycle(PlayerModelState *player, std::int32_t mode) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    switch (mode) {
    case 0: {
        AssignCharacterControlFlags(player->character, bicycle->savedControlFlags);
        mtwkSetExtraFlag(player->motion, 0, 0);
        Vector3i rotation;
        rotation.x = 0;
        rotation.y = bicycle->rotation.y;
        rotation.z = 0;
        charaSetWorldRot(player->character, rotation);
        bicycle->flags &= ~2U;
        bicycle->limbFlags &= 0xfe;
        SetNativeGuiActive(1);
        break;
    }
    case 1:
        bicycle->savedControlFlags = ccmGetFlags(player->character);
        ccmClearFlags(player->character, 14);
        mtwkSetExtraFlag(player->motion, 0, 2);
        bicycle->flags |= 2U;
        bicycle->mounted = 0;
        bicycle->limbFlags = 63;
        SetNativeGuiActive(0);
        break;
    case 2:
        mtwkSetExtraFlag(player->motion, 0, 0);
        bicycle->limbFlags &= 0xd8;
        bicycle->flags &= ~2U;
        g_playerRuntimePointer->flags |= 0x80000U;
        SetNativeGuiActive(1);
        break;
    case 3:
        mtwkSetExtraFlag(player->motion, 0, 2);
        bicycle->limbFlags |= 39;
        bicycle->flags |= 2U;
        g_playerRuntimePointer->flags &= ~0x80000U;
        SetNativeGuiActive(0);
        break;
    }
}

std::int32_t IsPlayerNearBicycle(PlayerModelState *player) {
    Vector3 position;
    GetCharacterWorldPosition(player->character, &position);
    if (model_component_detail::Vector3_ComputeSquaredDistance(position, g_bicycleRideWork.position) > 2.25f)
        return 0;
    return 1;
}

void UpdateBicycleRideState(PlayerModelState *player) {
    const dc::InputSlot *const input = player->inputSnapshot;
    PlayerMovementState *const movement = player->movementState;
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    std::int32_t motion = -1;
    if (mtwkGetAnimationId(player->motion, static_cast<MotionRegion>(0)) == 0xfa07 && (movement->flags & 0x400U))
        bicycle->flags |= 0x80U;
    if (mtwkGetAnimationId(player->motion, static_cast<MotionRegion>(0)) == 0xfa0b && (movement->flags & 0x400U)) {
        bicycle->flags &= ~0x80U;
        movement->flags &= ~2U;
    }
    std::int32_t previousState = bicycle->state;
    bicycle->requestedMotion = 0;
    switch (bicycle->state) {
    case 0:
        if (bicycle->previousState == -1) {
            movement->flags |= 2U;
            bicycle->state = 6;
        }
        break;
    case 1: {
        const std::uint16_t animation = mtwkGetAnimationId(player->motion, static_cast<MotionRegion>(0));
        if (animation == 0xf002 || animation == 0xf004 || animation == 0xf005) {
            if (!mtwkSetRegionFlag(player->motion, 72, 1, static_cast<MotionRegion>(1))) {
                mtwkRequestMotion(player->motion, 0xfa0f, static_cast<MotionRegion>(4));
                mtwkRequestMotion(player->motion, 0xfa0f, static_cast<MotionRegion>(3));
                mtwkRequestMotion(player->motion, 0xfa0f, static_cast<MotionRegion>(1));
            }
        } else if (mtwkSetRegionFlag(player->motion, 72, 1, static_cast<MotionRegion>(1))) {
            mtwkInitFlags(player->motion, 0);
        }
        if ((bicycle->flags & 0x80U) && static_cast<std::int8_t>(bicycle->pendingAction) == 0) {
            if (input->held & 0xc000U)
                bicycle->idleFrames = 0;
            else
                ++bicycle->idleFrames;
            if (bicycle->idleFrames > 30) {
                bicycle->pendingAction = 12;
                bicycle->idleFrames = 0;
            }
        }
        const std::int16_t held = static_cast<std::int16_t>(input->held);
        if (!(held & 0x1000) && !player->analogHistory0) {
            if (held & 0x2000) {
                if (held < 0) {
                    movement->flags |= 2U;
                    bicycle->pendingAction = 14;
                    bicycle->turnDirection = 0xc000;
                } else if (input->held & 0x4000U) {
                    movement->flags |= 2U;
                    bicycle->pendingAction = 14;
                    bicycle->turnDirection = 0x4000;
                } else {
                    movement->flags |= 2U;
                    bicycle->pendingAction = 14;
                    bicycle->turnDirection = 0x8000;
                }
            } else if (static_cast<std::int16_t>(input->held) < 0) {
                movement->flags |= 2U;
                bicycle->pendingAction = 14;
                bicycle->turnDirection = 0xe000;
            } else if (input->held & 0x4000U) {
                movement->flags |= 2U;
                bicycle->pendingAction = 14;
                bicycle->turnDirection = 0x2000;
            }
        }
        if (input->pressedPriority & 0x200U) {
            movement->flags |= 2U;
            bicycle->mounted = 0;
            bicycle->state = 8;
            mtwkInitFlags(player->motion, 0);
        }
        break;
    }
    case 6:
        if (bicycle->currentMotion == 1 && !mtwkSetExtraFlag(player->motion, 58, 1)) {
            bicycle->state = 2;
            player->flags |= 0x10000U;
        }
        break;
    case 2:
        if (mtwkGetAnimationId(player->motion, static_cast<MotionRegion>(0)) == 0xfa21 && (movement->flags & 0x400U)) {
            AttachRiderToBicycle(player, 1);
            bicycle->state = 3;
        }
        break;
    case 8:
        if (movement->flags & 0x400U) {
            AttachRiderToBicycle(player, 3);
            bicycle->state = 3;
        }
        break;
    case 3:
        if (std::fabs(bicycle->speed) < 1.1920929e-07f) {
            if ((bicycle->flags & 0x1000U) && (input->pressedPriority & 0x400U))
                bicycle->state = 9;
            if ((input->pressedPriority & 0x2000U) && !(input->held & 0xc000U))
                bicycle->state = 7;
        }
        break;
    case 10:
        if (movement->flags & 0x400U)
            bicycle->state = 3;
        break;
    case 9:
        if (bicycle->currentMotion == 6 && (movement->flags & 0x400U) && mtwkGetAnimationId(player->motion, static_cast<MotionRegion>(0)) == 0xfa22) {
            bicycle->state = 0;
            movement->pendingYawCorrection = static_cast<std::int16_t>(0xc000);
            movement->flags &= ~2U;
            player->useExternalMovementState = 0;
            player->flags &= ~0x10000U;
            previousState = -1;
        }
        break;
    case 4:
        if (bicycle->currentMotion == 5 && (movement->flags & 0x400U)) {
            bicycle->state = 1;
            movement->flags &= ~2U;
            previousState = -1;
            bicycle->mounted = 1;
        }
        break;
    case 7:
        if (bicycle->currentMotion == 10) {
            bicycle->limbFlags &= 0xc0;
            bicycle->flags &= ~2U;
            if (movement->flags & 0x400U) {
                bicycle->flags |= 2U;
                bicycle->limbFlags |= 63;
                bicycle->state = 3;
            }
        }
        break;
    }
    switch (bicycle->state) {
    case 0:
        break;
    case 1:
        if (static_cast<std::int8_t>(bicycle->pendingAction) == 13) {
            if (!(bicycle->flags & 0x80U)) {
                motion = 11;
            } else {
                motion = 13;
                bicycle->pendingAction = 0;
                bicycle->currentMotion = -1;
            }
        }
        if (static_cast<std::int8_t>(bicycle->pendingAction) == 14) {
            if (!(bicycle->flags & 0x80U)) {
                motion = 11;
            } else {
                motion = 14;
                bicycle->pendingAction = 0;
                bicycle->currentMotion = -1;
            }
        }
        if (static_cast<std::int8_t>(bicycle->pendingAction) == 12 && (bicycle->flags & 0x80U)) {
            motion = 12;
            bicycle->pendingAction = 0;
            bicycle->currentMotion = -1;
        }
        break;
    case 6:
        motion = 1;
        break;
    case 5:
        motion = 7;
        break;
    case 2:
        motion = 2;
        break;
    case 8:
        motion = 3;
        break;
    case 3:
        motion = 4;
        break;
    case 9:
        motion = 6;
        break;
    case 4:
        motion = 5;
        break;
    case 7:
        motion = 10;
        break;
    case 10:
        motion = 16;
        break;
    }
    if (bicycle->currentMotion != motion)
        bicycle->requestedMotion = static_cast<std::int8_t>(motion);
    if (bicycle->state != previousState)
        bicycle->previousState = static_cast<std::int8_t>(previousState);
}

void RequestBicycleRideMotion(PlayerModelState *player) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    std::int32_t requested = 0;
    UpdateBicycleRideSounds(player);
    switch (bicycle->requestedMotion) {
    case 2:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa21);
        break;
    case 3:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa12);
        break;
    case 4:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa20);
        break;
    case 6:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa22);
        if (requested)
            AttachRiderToBicycle(player, 0);
        break;
    case 7:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa15);
        break;
    case 10:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa16);
        break;
    case 5:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa10);
        break;
    case 11:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa07);
        break;
    case 12:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa0b);
        break;
    case 13:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa0c);
        break;
    case 16:
        requested = player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa06);
        break;
    case 14: {
        std::int32_t turns[8];
        turns[0] = -1;
        turns[1] = 0xfa09;
        turns[2] = 0xfa0a;
        turns[3] = -2;
        turns[4] = 0xfa08;
        turns[5] = -2;
        turns[6] = 0xfa0d;
        turns[7] = 0xfa0c;
        const std::int32_t sector = (static_cast<std::int32_t>(bicycle->turnDirection) + 0x1000) >> 12;
        const std::int32_t turn = turns[(sector / 2) & 7];
        if (turn >= 0)
            requested = player_detail::TryRequestPlayerMotion(*player->movementState, turn);
        break;
    }
    case 1: {
        Vector3 rider, mount;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&bicycle->position);
        Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y - 0x4000);
        Matrix4x4::preTranslate(0.23021f, 0.0f, 0.64075f);
        matrix_stack::GetTranslation(&mount);
        PopCurrentMatrix(1);
        const std::int16_t yaw = static_cast<std::int16_t>(bicycle->rotation.y - 0x4000);
        mount.y = FindFieldMinimumHeightAbove(&mount);
        GetCharacterWorldPosition(player->character, &rider);
        CalculateDistanceBetweenPoints(&mount, &rider);
        requested = mtwkCopyTransition(player->motion, &mount, yaw, 0, 0);
        break;
    }
    }
    if (requested)
        bicycle->currentMotion = bicycle->requestedMotion;
    bicycle->requestedMotion = 0;
}

static void UpdateBicycleRideSounds(PlayerModelState *player) {
    BicycleRideWork *const bicycle = g_bicycleRide;
    PlayerMovementState *const movement = player->movementState;
    MotionWork *const motion = player->motion;
    const std::int32_t animation = mtwkGetAnimationId(motion, static_cast<MotionRegion>(0));
    if ((animation == 0xfa20 || animation == 0xfa1a) && !bicycle->pedalCount && bicycle->speed > 0.0f) {
        if (!(bicycle->flags & 0x10000000U)) {
            Loop3DSoundEffect(0x499);
            bicycle->flags |= 0x10000000U;
        }
    } else if (bicycle->flags & 0x10000000U) {
        Loop3DSoundEffect(0x49a);
        bicycle->flags &= ~0x10000000U;
    }
    if (!(bicycle->flags & 0x400U)) {
        if (bicycle->pedalCount > 5)
            bicycle->flags |= 0x400U;
    } else if (!(bicycle->pedalCount > 2)) {
        bicycle->flags &= ~0x400U;
    }
    if (bicycle->flags & 0x400U) {
        if (animation == 0xfa20)
            player_detail::TryRequestPlayerMotion(*movement, 0xfa1a);
    } else if (animation == 0xfa1a) {
        player_detail::TryRequestPlayerMotion(*movement, 0xfa20);
    }
    movement->firstScale = 1.0f;
    if (animation == 0xfa20 || animation == 0xfa1a) {
        movement->firstScale = 0.0f;
        if (!bicycle->pedalCount)
            return;
        const float count = static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(animation)));
        const float frame = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
        const float phase = frame / count;
        const float half = count * 0.5f;
        const float end = half > frame ? half : count - 1.0f;
        const float remaining = end - frame;
        movement->firstScale = 1.0f > remaining ? remaining : 1.0f;
        if (!(remaining > 1.0f))
            --bicycle->pedalCount;
        if (mtwkSetRegionFlag(motion, 68, 1, static_cast<MotionRegion>(0)))
            movement->firstScale = mtwkGetSpeed(motion, static_cast<MotionRegion>(0));
        if (bicycle->pedalCount) {
            if (!mtwkSetRegionFlag(motion, 70, 1, static_cast<MotionRegion>(0))) {
                if (std::fabs(floorf(frame - 1.0f)) < 1.1920929e-07f)
                    Loop3DSoundEffect(0x6b2);
                if (std::fabs(floorf(frame - half)) < 1.1920929e-07f)
                    Loop3DSoundEffect(0x6b3);
            } else {
                if (std::fabs(floorf(frame - 1.0f)) < 1.1920929e-07f)
                    Loop3DSoundEffect(0x6b3);
                if (std::fabs(floorf(frame - half)) < 1.1920929e-07f)
                    Loop3DSoundEffect(0x6b2);
            }
        }
        bicycle->pedalAngle = static_cast<std::int16_t>(static_cast<std::int32_t>(phase * 65536.0f));
    }
    movement->firstScale = movement->firstScale;
}

static void UpdateBicycleNodes(BicycleRideWork *bicycle) {
    MT7Node *const root = bicycle->bike->model;
    float wheelTurn = bicycle->speed / 1.9602439f;
    wheelTurn *= 360.0f;
    MT7Node *node = FindMT7Node(root, 0xac);
    if (node)
        node->rotation.x -= static_cast<std::uint16_t>(static_cast<std::int32_t>((wheelTurn * 8192.0f) / 45.0f));
    node = FindMT7Node(root, 0xa4);
    if (node)
        node->rotation.x -= static_cast<std::uint16_t>(static_cast<std::int32_t>((wheelTurn * 8192.0f) / 45.0f));
    node = FindMT7Node(root, 0xa2);
    if (node)
        node->rotation.y = bicycle->steer;
    node = FindMT7Node(root, 0xb6);
    if (node)
        node->rotation.x = -bicycle->pedalAngle;
    node = FindMT7Node(root, 0xb8);
    if (node)
        node->rotation.x = bicycle->pedalAngle;
    node = FindMT7Node(root, 0xb7);
    if (node)
        node->rotation.x = bicycle->pedalAngle;
    node = FindMT7Node(root, 0xa5);
    if (node)
        node->rotation.x = static_cast<std::uint16_t>(static_cast<std::int32_t>(((bicycle->standBlend * 89.65f + 0.35f) * 8192.0f) / 45.0f));
    if (bicycle->savedLampChild) {
        node = FindMT7Node(root, 0xc0);
        if (node)
            node->child = (bicycle->flags & 0x800U) ? bicycle->savedLampChild : 0;
    }
}

void SetBicycleStandMode(std::int32_t standing) {
    BicycleRideWork *const bicycle = g_bicycleRide;
    MT7Node *const root = bicycle->bike->model;
    if (!root)
        return;
    MT7Node *const stand = FindMT7Node(root, 0xa5);
    if (!stand)
        return;
    if (standing) {
        stand->rotation.x = 0x4000;
        bicycle->flags |= 0x40U;
        bicycle->standLift = 0.1f;
        bicycle->standBlend = 1.0f;
        ApplyBicycleStandPitch(bicycle);
        ApplyBicycleTransform(bicycle);
    } else {
        stand->rotation.x = 63;
        bicycle->flags &= ~0x40U;
        bicycle->standLift = 0.0f;
        bicycle->standBlend = 0.0f;
    }
}

static void SnapRiderLimbsToBicycle(BicycleRideWork *bicycle) {
    Character *const bike = bicycle->bike;
    MT7Node *const root = bike->model;
    Vector3 leftFoot, rightFoot;
    matrix_stack::PushUnitMatrix();
    ApplyCharacterNodeTransformAndScale(bike);
    matrix_stack::Matrix4x4_Push(0);
    GetMT7NodeWorldMatrixById(root, 0xb8);
    Matrix4x4::preTranslate(-0.16079f, 0.12724f, 0.10471f);
    matrix_stack::GetTranslation(&leftFoot);
    PopCurrentMatrix(1);
    matrix_stack::Matrix4x4_Push(0);
    GetMT7NodeWorldMatrixById(root, 0xb7);
    Matrix4x4::preTranslate(0.19025f, 0.13593f, 0.08375f);
    matrix_stack::GetTranslation(&rightFoot);
    PopCurrentMatrix(1);
    GetMT7NodeWorldMatrixById(root, 0xa2);
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(-0.31975f, 0.46056f, 0.19122f);
    matrix_stack::GetTranslation(&bicycle->leftHandTarget);
    PopCurrentMatrix(1);
    Matrix4x4::preTranslate(0.31429f, 0.47151f, 0.1927f);
    matrix_stack::GetTranslation(&bicycle->rightHandTarget);
    PopCurrentMatrix(1);
    if (!(bicycle->flags & 4U)) {
        bicycle->leftFootTarget = leftFoot;
        bicycle->rightFootTarget = rightFoot;
    }
    if (0.00092592591f > bicycle->speed && 0.0f > bicycle->netForce) {
        Vector3 step;
        step.x = -0.24752f;
        step.y = 0.17474f;
        step.z = 0.38456002f;
        Vector3 *foot;
        Vector3 *target;
        if (leftFoot.y < rightFoot.y) {
            foot = &leftFoot;
            target = &bicycle->leftFootTarget;
            bicycle->flags |= 8U;
        } else {
            foot = &rightFoot;
            target = &bicycle->rightFootTarget;
            bicycle->flags |= 0x10U;
            step.x = -step.x;
        }
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&bicycle->position);
        Matrix4x4::RotateAroundY_Wrapper(bicycle->rotation.y);
        matrix_stack::TranslateVector(&step);
        matrix_stack::GetTranslation(foot);
        PopCurrentMatrix(1);
        foot->y = FindFieldIntersectionHeight(foot) + 0.12f;
        if (!(bicycle->flags & 4U)) {
            target->x = foot->x;
            target->y = foot->y;
            target->z = foot->z;
        }
        foot->x = InterpolateTo(foot->x, target->x, 0.3f);
        foot->y = InterpolateTo(foot->y, target->y, 0.3f);
        foot->z = InterpolateTo(foot->z, target->z, 0.3f);
    } else {
        if (bicycle->flags & 8U) {
            if (model_component_detail::Vector3_ComputeSquaredDistance(bicycle->leftFootTarget, leftFoot) < 0.0025000002f)
                bicycle->flags &= ~8U;
            leftFoot.x = InterpolateTo(leftFoot.x, bicycle->leftFootTarget.x, 0.5f);
            leftFoot.y = InterpolateTo(leftFoot.y, bicycle->leftFootTarget.y, 0.5f);
            leftFoot.z = InterpolateTo(leftFoot.z, bicycle->leftFootTarget.z, 0.5f);
        }
        if (bicycle->flags & 0x10U) {
            if (model_component_detail::Vector3_ComputeSquaredDistance(bicycle->rightFootTarget, rightFoot) < 0.0025000002f)
                bicycle->flags &= ~0x10U;
            rightFoot.x = InterpolateTo(rightFoot.x, bicycle->rightFootTarget.x, 0.5f);
            rightFoot.y = InterpolateTo(rightFoot.y, bicycle->rightFootTarget.y, 0.5f);
            rightFoot.z = InterpolateTo(rightFoot.z, bicycle->rightFootTarget.z, 0.5f);
        }
    }
    bicycle->leftFootTarget = leftFoot;
    bicycle->rightFootTarget = rightFoot;
    if (!(bicycle->flags & 4U))
        bicycle->flags |= 4U;
}

std::int32_t GetBicycleLimbTarget(Vector3 *target, std::int32_t limb) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    const Vector3 *source = 0;
    if ((bicycle->limbFlags & 1) == 0)
        return -1;
    if (!target)
        return -1;
    switch (limb) {
    case 0:
        if (bicycle->limbFlags & 2)
            source = &bicycle->leftFootTarget;
        break;
    case 1:
        if (bicycle->limbFlags & 4)
            source = &bicycle->rightFootTarget;
        break;
    case 2:
        if (bicycle->limbFlags & 8)
            source = &bicycle->leftHandTarget;
        break;
    case 3:
        if (bicycle->limbFlags & 0x10)
            source = &bicycle->rightHandTarget;
        break;
    }
    if (!source)
        return -1;
    target->x = source->x;
    target->y = source->y;
    target->z = source->z;
    return 0;
}

static void UpdateBicycleHandlebarYaw(BicycleRideWork *bicycle) {
    bicycle->handlebarYaw = CameraLerpAngle(static_cast<std::int16_t>(-bicycle->rotation.z), bicycle->handlebarYaw, 0.5f);
}

std::int16_t GetBicycleRiderYaw() {
    BicycleRideWork *const bicycle = g_bicycleRide;
    if (!(bicycle->flags & 2U) && bicycle->state != 1)
        return g_playerRuntimePointer->model->yaw;
    return bicycle->rotation.y;
}

std::int32_t GetBicycleHandlebarAngles(std::int16_t *angles) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    if ((bicycle->limbFlags & 1) == 0)
        return -1;
    if ((bicycle->limbFlags & 0x20) == 0)
        return -1;
    angles[0] = bicycle->handlebarYaw;
    angles[1] = bicycle->handlebarPitch;
    return 0;
}

void AlignBicycleToCharacter(Character *character) {
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    if (!character)
        return;
    bicycle->bike = character;
    GetCharacterWorldPosition(character, &bicycle->position);
    bicycle->rotation.y = static_cast<std::int16_t>(GetCharacterWorldYaw(character) + 0x4000);
}

std::int32_t AlignBicycleToScenarioCharacter(shen_name name) {
    BicycleRideWork *const bicycle = g_bicycleRide;
    Character *const character = ctrlGetScenarioChara(name);
    if (!character)
        return -1;
    bicycle->bike = character;
    GetCharacterWorldPosition(character, &bicycle->position);
    bicycle->rotation.y = static_cast<std::int16_t>(GetCharacterWorldYaw(character) + 0x4000);
    return 0;
}

shen_name GetBicycleScenarioName() {
    return ctrlGetScenarioCharaName(g_bicycleRide->bike);
}

void MountBicycle(std::int32_t mount) {
    PlayerModelState *const player = g_playerRuntimePointer->model;
    BicycleRideWork *bicycle = &g_bicycleRideWork;
    if (!mount) {
        player->movementState->flags &= ~2U;
        player->flags &= ~0x10000U;
        AttachRiderToBicycle(player, 0);
        SetBicycleStandMode(1);
        return;
    }
    AttachRiderToBicycle(player, 1);
    bicycle->state = 3;
    player_detail::TryRequestPlayerMotion(*player->movementState, 0xfa20);
    GetCharacterWorldPosition(player->character, &bicycle->position);
    bicycle->rotation.y = static_cast<std::int16_t>(GetCharacterWorldYaw(player->character));
    SetCharacterWorldPosition(bicycle->bike, &bicycle->position);
    SetCharacterWorldYaw(bicycle->bike, bicycle->rotation.y);
    SetBicycleStandMode(0);
    player->movementState->flags |= 2U;
    player->flags |= 0x10000U;
}

std::int32_t EV_MountBicycle(std::int32_t mount) {
    MountBicycle(mount);
    return 0;
}

std::int32_t StopBicycle() {
    BicycleRideWork *const bicycle = g_bicycleRide;
    bicycle->speed = 0.0f;
    bicycle->steer = 0;
    return 0;
}

std::int32_t SetBicycleLampEnabled(std::int32_t enabled) {
    BicycleRideWork *const bicycle = g_bicycleRide;
    const std::uint32_t previous = bicycle->flags;
    if (enabled)
        bicycle->flags = previous | 0x800U;
    else
        bicycle->flags &= ~0x800U;
    return (static_cast<std::int32_t>(previous) >> 11) & 1;
}

std::int32_t SetBicycleRideFlag1000(std::int32_t enabled) {
    BicycleRideWork *const bicycle = g_bicycleRide;
    const std::uint32_t previous = bicycle->flags;
    if (enabled)
        bicycle->flags = previous | 0x1000U;
    else
        bicycle->flags &= ~0x1000U;
    return (static_cast<std::int32_t>(previous) >> 12) & 1;
}

void BicycleRideNullsub() {
}

} // namespace s2
} // namespace shenmue

#endif
