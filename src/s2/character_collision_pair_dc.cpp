#include "s2/character_collision_task_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include <cmath>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "platform/dc/matrix.hpp"
#include "s2/character_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/motion_work_dc.hpp"
#endif

namespace shenmue {
namespace s2 {

// @verified - d
std::int32_t ResolveCharacterControlPair(CharacterControlModel *first, CharacterControlModel *second) {
    Vector3 firstCenter, secondCenter, direction, share;
    am2Vector3Add(&firstCenter, &first->collisionCenter, &first->pairAdjustment);
    am2Vector3Add(&secondCenter, &second->collisionCenter, &second->pairAdjustment);
    if ((firstCenter.y - secondCenter.y < 0.0f ? -(firstCenter.y - secondCenter.y) : firstCenter.y - secondCenter.y) > 2.5f)
        return 0;
    am2Vector3Subtract(&direction, &secondCenter, &firstCenter);
    direction.y = 0.0f;
    float squared = direction.x * direction.x;
    squared += direction.y * direction.y;
    squared += direction.z * direction.z;
    const float radius = first->collisionCenterRadius + second->collisionCenterRadius;
    if (!(radius * radius > squared))
        return 0;
    const float overlap = radius - std::sqrt(squared);
    if (std::fabs(squared) >= kCharacterControlEpsilon) {
        am2Vector3NormalizeInPlace(&direction);
    } else {
        direction.x = 1.0f;
        direction.y = 0.0f;
        direction.z = 0.0f;
    }
    const float sum = first->displacementSquared + second->displacementSquared;
    float firstWeight, secondWeight;
    if (std::fabs(sum) >= kCharacterControlEpsilon) {
        firstWeight = first->displacementSquared / sum;
        if (firstWeight < 0.0f)
            firstWeight = -firstWeight;
        secondWeight = second->displacementSquared / sum;
        if (secondWeight < 0.0f)
            secondWeight = -secondWeight;
    } else {
        firstWeight = 0.5f;
        secondWeight = 0.5f;
    }
    if (first->flags & CharacterControlPairImmovable) {
        secondWeight = firstWeight = 0.0f;
        if (!(second->flags & CharacterControlPairImmovable))
            secondWeight = 1.0f;
    } else if (second->flags & CharacterControlPairImmovable) {
        secondWeight = 0.0f;
        firstWeight = 1.0f;
    }
    am2Vector3Scale(&share, &direction, -overlap * firstWeight);
    am2Vector3AddInPlace(&first->pairAdjustment, &share);
    am2Vector3Scale(&share, &direction, overlap * secondWeight);
    am2Vector3AddInPlace(&second->pairAdjustment, &share);
    return 1;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// Sphere chain and count at offsets 0x74 and 0x78.
struct CharacterControlSphereChain {
    Vector4 *spheres;
    std::int32_t count;
};

std::int32_t ccmBuildCollisionSpheres(CharacterControlModel *ccm) {
    Vector3 position, head;
    Vector4 *sphere = reinterpret_cast<CharacterControlSphereChain *>(ccm->retainedShapeState)->spheres;
    Vector4 *middle = 0;
    std::int32_t count = 0;
    GetCharacterWorldPosition(ccm->owner, &position);
    matrix_stack::PushUnitMatrix();
    matrix_stack::LoadMatrix(mtwkGetBoneTransform(charFindMotionwork(ccm->owner), 5));
    Matrix4x4::preTranslate(0.025f, 0.0f, 0.05f);
    matrix_stack::GetTranslation(&head);
    PopCurrentMatrix(1);
    float distance = position.x - head.x;
    const float dz = position.z - head.z;
    distance = distance * distance + dz * dz;
    if (distance > 0.2f * 0.2f) {
        middle = sphere++;
        ++count;
    }
    sphere->x = position.x;
    sphere->y = position.y;
    sphere->z = position.z;
    sphere->w = 0.4f;
    ++sphere;
    ++count;
    if (middle) {
        sphere->x = head.x;
        sphere->y = head.y;
        sphere->z = head.z;
        sphere->w = 0.15f;
        ++sphere;
        ++count;
    }
    sphere->w = 0.0f;
    if (middle) {
        middle->x = (position.x + head.x) * 0.5f;
        middle->y = position.y;
        middle->z = (position.z + head.z) * 0.5f;
        middle->w = CalculateDistanceBetweenPoints(reinterpret_cast<const Vector3 *>(middle), &position) + 0.4f;
    }
    reinterpret_cast<CharacterControlSphereChain *>(ccm->retainedShapeState)->count = count;
    return count;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
