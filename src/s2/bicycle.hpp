#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "platform/dc/input.hpp"

namespace shenmue {
namespace s2 {

struct PlayerModelState;

#define BICYCLE_CHARACTER_NAME SHEN_NAME4('B', 'C', 'Y', 'L')

struct BicycleWheelContact {
    std::uint32_t flags;
    std::uint32_t retained04;
    Vector3 position;
    float retained14;
    float verticalVelocity;
    float retained1c;
};
typedef char Layout_bicycle_0[(sizeof(BicycleWheelContact) == 0x20) ? 1 : -1];

struct BicycleRideWork {
    std::uint32_t flags;
    std::int8_t mounted;
    std::int8_t state;
    std::uint8_t retained06;
    std::int8_t previousState;
    Vector3 position;
    float retained14[6];
    Vector3s rotation;
    std::int16_t steer, steerLimit;
    std::uint16_t retained36;
    float speed, acceleration, pedalForce, brakeForce, rollingResistance, cornerForce, slopeForce, netForce;
    Character *bike, *rider;
    const dc::InputSlot *input;
    std::uint32_t retained64;
    Vector3 previousPosition;
    Vector3s previousRotation;
    std::uint16_t retained7a;
    float turnRadius;
    Vector3i worldRotation;
    std::int16_t pedalAngle, retained8e, standPitch;
    std::uint16_t retained92;
    Vector3 leftFootTarget, rightFootTarget, leftHandTarget, rightHandTarget;
    Matrix4x4 transform;
    Vector3 positionDelta;
    float standLift, standBlend, standVelocity;
    std::uint32_t savedControlFlags;
    float pedalInput, brakeInput;
    std::uint32_t retained128;
    std::int16_t handlebarYaw, handlebarPitch;
    float riderHeight;
    std::int8_t requestedMotion, currentMotion;
    std::uint8_t retained136, pendingAction, limbFlags, retained139;
    std::uint16_t turnDirection;
    std::int16_t idleFrames;
    std::uint16_t retained13e;
    BicycleWheelContact rearWheel, frontWheel;
    BicycleWheelContact *rear, *front;
    std::uint8_t retained188, pedalCount, retained18a, retained18b;
    MT7Node *savedLampChild;
};
typedef char Layout_bicycle_1[(sizeof(BicycleRideWork) == 0x190) ? 1 : -1];
typedef char Layout_bicycle_2[(offsetof(BicycleRideWork, rotation) == 0x2c) ? 1 : -1];
typedef char Layout_bicycle_3[(offsetof(BicycleRideWork, speed) == 0x38) ? 1 : -1];
typedef char Layout_bicycle_4[(offsetof(BicycleRideWork, bike) == 0x58) ? 1 : -1];
typedef char Layout_bicycle_5[(offsetof(BicycleRideWork, previousPosition) == 0x68) ? 1 : -1];
typedef char Layout_bicycle_6[(offsetof(BicycleRideWork, turnRadius) == 0x7c) ? 1 : -1];
typedef char Layout_bicycle_7[(offsetof(BicycleRideWork, pedalAngle) == 0x8c) ? 1 : -1];
typedef char Layout_bicycle_8[(offsetof(BicycleRideWork, leftFootTarget) == 0x94) ? 1 : -1];
typedef char Layout_bicycle_9[(offsetof(BicycleRideWork, transform) == 0xc4) ? 1 : -1];
typedef char Layout_bicycle_10[(offsetof(BicycleRideWork, standLift) == 0x110) ? 1 : -1];
typedef char Layout_bicycle_11[(offsetof(BicycleRideWork, pedalInput) == 0x120) ? 1 : -1];
typedef char Layout_bicycle_12[(offsetof(BicycleRideWork, handlebarYaw) == 0x12c) ? 1 : -1];
typedef char Layout_bicycle_13[(offsetof(BicycleRideWork, requestedMotion) == 0x134) ? 1 : -1];
typedef char Layout_bicycle_14[(offsetof(BicycleRideWork, turnDirection) == 0x13a) ? 1 : -1];
typedef char Layout_bicycle_15[(offsetof(BicycleRideWork, rearWheel) == 0x140) ? 1 : -1];
typedef char Layout_bicycle_16[(offsetof(BicycleRideWork, rear) == 0x180) ? 1 : -1];
typedef char Layout_bicycle_17[(offsetof(BicycleRideWork, pedalCount) == 0x189) ? 1 : -1];

extern BicycleRideWork g_bicycleRideWork;
extern BicycleRideWork *g_bicycleRide;

void InitializeBicycleRider(PlayerModelState *player);
void UpdateBicycleRider(PlayerModelState *player);
std::int32_t IsPlayerNearBicycle(PlayerModelState *player);
void UpdateBicycleRideState(PlayerModelState *player);
void RequestBicycleRideMotion(PlayerModelState *player);
void SetBicycleStandMode(std::int32_t standing);
std::int32_t GetBicycleLimbTarget(Vector3 *target, std::int32_t limb);
std::int16_t GetBicycleRiderYaw();
std::int32_t GetBicycleHandlebarAngles(std::int16_t *angles);
void AlignBicycleToCharacter(Character *character);
std::int32_t AlignBicycleToScenarioCharacter(shen_name name);
shen_name GetBicycleScenarioName();
void MountBicycle(std::int32_t mount);
std::int32_t EV_MountBicycle(std::int32_t mount);
std::int32_t StopBicycle();
std::int32_t SetBicycleLampEnabled(std::int32_t enabled);
std::int32_t SetBicycleRideFlag1000(std::int32_t enabled);
void BicycleRideNullsub();

} // namespace s2
} // namespace shenmue

#endif
