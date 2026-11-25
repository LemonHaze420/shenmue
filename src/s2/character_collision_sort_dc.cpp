#include "s2/character_collision_task_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "s2/motion_work_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
std::int32_t CharCollisionSort(Character *character, void *) {
    CharacterControlModel *const control = static_cast<CharacterControlModel *>(charFindComp(character, CHARACTER_CONTROL));
    if (!control)
        return 1;
    const float zero = 0.0f;
    control->pairAdjustment.x = zero;
    control->pairAdjustment.y = zero;
    control->pairAdjustment.z = zero;
    control->flags &= ~0x00000400U;
    control->flags &= ~0x00100000U;
    Vector3 delta, world;
    GetCharacterWorldPosition(control->owner, &world);
    GetCharacterPositionDelta(control->owner, &delta);
    am2Vector3Subtract(&control->frameDisplacement, &world, &control->previousWorldPosition);
    am2Vector3Add(&control->previousWorldPosition, &world, &delta);
    if (control->flags & CCMF_ResetMotion) {
        *reinterpret_cast<float *>(&control->field04) = zero;
        control->verticalVelocity = zero;
        *reinterpret_cast<float *>(&control->field0c) = zero;
        control->frameDisplacement.x = zero;
        control->frameDisplacement.y = zero;
        control->frameDisplacement.z = zero;
        control->collisionAdjustment.x = zero;
        control->collisionAdjustment.y = zero;
        control->collisionAdjustment.z = zero;
    }
    control->flags &= ~CCMF_ResetMotion;
    control->displacementSquared = control->frameDisplacement.x * control->frameDisplacement.x + control->frameDisplacement.y * control->frameDisplacement.y +
        control->frameDisplacement.z * control->frameDisplacement.z;
    control->flags &= ~CCMF_ConversationMember;
    if (IsConversationCameraCharacter(character))
        control->flags |= CCMF_ConversationMember;
    float radius = mtwkGetClusterPosition(charFindMotionwork(character), &control->collisionCenter);
    control->collisionCenter.y = world.y;
    if (control->flags & CCMF_PreserveCollisionRadius)
        radius = control->collisionCenterRadius;
    control->collisionCenterRadius = radius;
    control->flags &= ~CCMF_PreserveCollisionRadius;
    if (control->flags & CharacterControlUseConfiguredCapsule) {
        control->collisionCenter.x = world.x;
        control->collisionCenter.y = world.y;
        control->collisionCenter.z = world.z;
        control->collisionCenterRadius = control->collisionRadius;
    }
    coliPositionAdd(control->owner, &control->collisionAdjustment);
    control->collisionAdjustment.x = 0.0f;
    control->collisionAdjustment.y = 0.0f;
    control->collisionAdjustment.z = 0.0f;
    return 1;
}

} // namespace s2
} // namespace shenmue

#endif
