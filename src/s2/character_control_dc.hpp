#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

using ::shenmue::shen_name;
#define CHARACTER_CONTROL shen_name(SHEN_NAME4('C', 'C', 'M', '_'))
#define MODEL_FIGURE_POSE shen_name(SHEN_NAME4('F', 'I', 'G', 'P'))
static const std::uint32_t figurePoseDataSize = 0x38;
static const char *const characterCollisionTokens[] = {"Level", "Adjust", "Contact", "Size", "Weight", 0};
typedef std::uint32_t CharacterControlModelFlags;
enum {
    CCMF_UpdateHeight = 1,
    CCMF_ApplyGravity = 2,
    CCMF_EvaluateMotion = 4,
    CCMF_VoxelSweep = 8,
    CCMF_SnapToGround = 0x40,
    CCMF_FineRaycast = 0x80,
    CCMF_FieldDataDebug = 0x100,
    CCMF_RootOwner = 0x200,
    CharacterControlPairImmovable = 0x800,
    CCMF_ResetMotion = 0x00200000,
    CCMF_AlternateProbe = 0x00400000,
    CCMF_HeightStep = 0x01000000,
    CCMF_PreserveCollisionRadius = 0x02000000,
    CharacterControlUseConfiguredCapsule = 0x04000000,
    CCMF_ConversationMember = 0x08000000
};
struct CharacterControlModel {
    std::uint32_t flags;
    std::uint32_t field04;
    float verticalVelocity;
    std::uint32_t field0c;
    Vector3 pairAdjustment;
    Vector3 appliedMovement;
    Vector3 collisionAdjustment;
    Character *owner;
    float groundHeight, previousGroundHeight, adjustedGroundHeight, previousAdjustedGroundHeight;
    std::int16_t groundPitch, groundYaw;
    float maximumStepHeight;
    float collisionRadius;
    float collisionHeight;
    Vector3 previousWorldPosition;
    Vector3 frameDisplacement;
    float displacementSquared;
    std::byte retainedShapeState[8];
    Vector3 collisionCenter;
    float collisionCenterRadius;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t toggleCCMflagsByAmt(Character *character, std::uint32_t flags);
#else
void toggleCCMflagsByAmt(Character *character, std::uint32_t flags);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t SetCharacterCollisionEnabled(Character *character, std::int32_t enabled);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ResolveCharacterCylinderOverlap(Vector3 *result, const Vector3 *position, float radius);
#else
bool ResolveCharacterCylinderOverlap(Vector3 *result, const Vector3 *position, float radius);
#endif
std::int32_t CheckPointInCylinder(const Vector3 *start, const Vector3 *end);
bool CheckCharacterCylinderIntersection(const Vector3 &start, const Vector3 &end);
bool CheckCharacterSphereIntersection(const Vector3 &point, float radius);
void ccmUpdateEntityFlag(Character *character);
Character *GetPointFound();
float GetCharacterCollisionRadius(const Character *character);
float GetCharacterCollisionCapsuleMargin(const Character *character);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
float SetCharacterCollisionCapsuleMargin(Character *character, float margin);
#else
CharacterControlModel *SetCharacterCollisionCapsuleMargin(Character *character, float margin);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t ccmGetFlags(Character *character);
Vector3 *ccmGetAppliedMovement(Character *character, Vector3 *movement);
std::uint32_t ccmReplaceFlags(Character *character, std::uint32_t flags);
std::uint32_t ccmClearFlags(Character *character, std::uint32_t flags);
#endif
typedef char sizeofCharacterControlModelCheck[sizeof(CharacterControlModel) == 0x8c ? 1 : -1];
typedef char offsetofCharacterControlModelfield04Check[offsetof(CharacterControlModel, field04) == 4 ? 1 : -1];
typedef char offsetofCharacterControlModelverticalVelocityCheck[offsetof(CharacterControlModel, verticalVelocity) == 8 ? 1 : -1];
typedef char offsetofCharacterControlModelfield0cCheck[offsetof(CharacterControlModel, field0c) == 0x0c ? 1 : -1];
typedef char offsetofCharacterControlModelpairAdjustmentCheck[offsetof(CharacterControlModel, pairAdjustment) == 0x10 ? 1 : -1];
typedef char offsetofCharacterControlModelappliedMovementCheck[offsetof(CharacterControlModel, appliedMovement) == 0x1c ? 1 : -1];
typedef char offsetofCharacterControlModelcollisionAdjustmentCheck[offsetof(CharacterControlModel, collisionAdjustment) == 0x28 ? 1 : -1];
typedef char offsetofCharacterControlModelownerCheck[offsetof(CharacterControlModel, owner) == 0x34 ? 1 : -1];
typedef char offsetofCharacterControlModelgroundHeightCheck[offsetof(CharacterControlModel, groundHeight) == 0x38 ? 1 : -1];
typedef char offsetofCharacterControlModelpreviousGroundHeightCheck[offsetof(CharacterControlModel, previousGroundHeight) == 0x3c ? 1 : -1];
typedef char offsetofCharacterControlModeladjustedGroundHeightCheck[offsetof(CharacterControlModel, adjustedGroundHeight) == 0x40 ? 1 : -1];
typedef char offsetofCharacterControlModelpreviousAdjustedGroundHeightCheck[offsetof(CharacterControlModel, previousAdjustedGroundHeight) == 0x44 ? 1 : -1];
typedef char offsetofCharacterControlModelgroundPitchCheck[offsetof(CharacterControlModel, groundPitch) == 0x48 ? 1 : -1];
typedef char offsetofCharacterControlModelgroundYawCheck[offsetof(CharacterControlModel, groundYaw) == 0x4a ? 1 : -1];
typedef char offsetofCharacterControlModelmaximumStepHeightCheck[offsetof(CharacterControlModel, maximumStepHeight) == 0x4c ? 1 : -1];
typedef char offsetofCharacterControlModelcollisionRadiusCheck[offsetof(CharacterControlModel, collisionRadius) == 0x50 ? 1 : -1];
typedef char offsetofCharacterControlModelcollisionHeightCheck[offsetof(CharacterControlModel, collisionHeight) == 0x54 ? 1 : -1];
typedef char offsetofCharacterControlModelpreviousWorldPositionCheck[offsetof(CharacterControlModel, previousWorldPosition) == 0x58 ? 1 : -1];
typedef char offsetofCharacterControlModelframeDisplacementCheck[offsetof(CharacterControlModel, frameDisplacement) == 0x64 ? 1 : -1];
typedef char offsetofCharacterControlModeldisplacementSquaredCheck[offsetof(CharacterControlModel, displacementSquared) == 0x70 ? 1 : -1];
typedef char offsetofCharacterControlModelcollisionCenterCheck[offsetof(CharacterControlModel, collisionCenter) == 0x7c ? 1 : -1];
typedef char offsetofCharacterControlModelcollisionCenterRadiusCheck[offsetof(CharacterControlModel, collisionCenterRadius) == 0x88 ? 1 : -1];

} // namespace s2
} // namespace shenmue

#endif
