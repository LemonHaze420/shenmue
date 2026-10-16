#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {


struct CharacterCollisionTaskData {
    std::byte reserved00[0x4];
};

static_assert(sizeof(CharacterCollisionTaskData) == 0x4);

enum CharacterControlModelFlags : std::uint32_t {
    CharacterControlUpdateHeight = 0x00000001U,
    CharacterControlApplyGravity = 0x00000002U,
    CharacterControlEvaluateMotion = 0x00000004U,
    CharacterControlVoxelSweep = 0x00000008U,
    CharacterControlUnknown10 = 0x00000010U,
    CharacterControlSnapToGround = 0x00000040U,
    CharacterControlFineRaycast = 0x00000080U,
    CharacterControlFieldDataDebug = 0x00000100U,
    CharacterControlRootOwner = 0x00000200U,
    CharacterControlStairMode = 0x00000800U,
    CharacterControlImmovable = 0x00004000U,
    CharacterControlMoving = 0x00002000U,
    CharacterControlLookMode = 0x00008000U,
    CharacterControlIntersectionHit = 0x00020000U,
    CharacterControlNoGround = 0x00040000U,
    CharacterControlSearchBlocked = 0x00080000U,
    CharacterControlResetMotion = 0x00200000U,
    CharacterControlAlternateProbe = 0x00400000U,
    CharacterControlHeightStep = 0x01000000U,
    CharacterControlPreserveCollisionRadius = 0x02000000U,
    CharacterControlFineCollision = 0x04000000U,
    CharacterControlConversationMember = 0x08000000U,
    CharacterControlAttached = 0x40000000U,
};

#pragma pack(push, 4)



struct CharacterControlModel {
    std::uint32_t flags;
    std::uint32_t field04;
    float verticalVelocity;
    std::uint32_t field0c;
    Vector3 pairAdjustment;             // +0x10, CCT_ pair solver scratch
    Vector3 appliedMovement;
    Vector3 collisionAdjustment;
    Vector3 residualMovement;
    Character *owner;
    float groundHeight;
    float previousGroundHeight;
    float adjustedGroundHeight;
    float previousAdjustedGroundHeight;
    std::int16_t groundPitch;
    std::int16_t groundYaw;
    float maximumStepHeight;
    float collisionRadius;
    float collisionHeight;
    Vector3 previousWorldPosition;
    Vector3 frameDisplacement;
    float displacementSquared;
    std::byte reserved84[0x10];
    Vector3 collisionCenter;
    float collisionCenterRadius;
    std::uint32_t reservedA4;
};
#pragma pack(pop)

static_assert(sizeof(CharacterControlModel) == 0xa8);
static_assert(offsetof(CharacterControlModel, flags) == 0x00);
static_assert(offsetof(CharacterControlModel, pairAdjustment) == 0x10);
static_assert(offsetof(CharacterControlModel, collisionAdjustment) == 0x28);
static_assert(offsetof(CharacterControlModel, residualMovement) == 0x34);
static_assert(offsetof(CharacterControlModel, owner) == 0x40);
static_assert(offsetof(CharacterControlModel, groundHeight) == 0x48);
static_assert(offsetof(CharacterControlModel, groundPitch) == 0x58);
static_assert(offsetof(CharacterControlModel, maximumStepHeight) == 0x5c);
static_assert(offsetof(CharacterControlModel, previousWorldPosition) == 0x68);
static_assert(offsetof(CharacterControlModel, frameDisplacement) == 0x74);
static_assert(offsetof(CharacterControlModel, displacementSquared) == 0x80);
static_assert(offsetof(CharacterControlModel, collisionCenter) == 0x94);
static_assert(offsetof(CharacterControlModel, collisionCenterRadius) == 0xa0);

CharacterControlModel *FindCharacterControlModel(const Character *character);
void toggleCCMflagsByAmt(Character *character, std::uint32_t flags);
void ccmSetFlagsFor(Character *character, std::uint32_t flags);
void ccmUpdateEntityFlag(Character *character);
float GetCharacterCollisionRadius(const Character *character);
float GetCharacterCollisionCapsuleMargin(const Character *character);
CharacterControlModel *SetCharacterCollisionCapsuleMargin(Character *character, float margin);
std::size_t GatherCharacterControls(std::span<CharacterControlModel *> output);
extern std::uint8_t g_characterQueryPointFound;
extern CharacterControlModel *g_characterQueryPointControl;
extern Character *g_characterQueryPointOwner;
bool CheckCharacterCylinderIntersection(const Vector3 &start, const Vector3 &end);
bool ResolveCharacterCylinderOverlap(Vector3 &result, const Vector3 &position, float radius);
bool CheckCharacterSphereIntersection(const Vector3 &point, float radius);
Character *GetPointFound();


Character *CheckPointInCylinder_Wrapper(const Vector3 &start, const Vector3 &end);
bool AttachCharacterControlModel(Character *character);

bool QueueCharacterControlModelPairTask(std::uint8_t nextFunctionIndex);

void DestroyCharacterControlModelPairTask();

} // namespace shenmue::s2
