#include "s2/character_pose_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/math.hpp"
#include "s2/motion_work_dc.hpp"
#include <cstring>

#define SHENMUE_CHARACTER_POSE_NULL 0
namespace shenmue {
namespace s2 {

void GetCharacterLocalPosition(const Character *character, Vector3 *position);

// @verified - d
void TransformPointByCharacterNode(const CharacterNode *node, Vector3 *position) {
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    matrix_stack::UnitCurrentMatrix();
    ApplyRecursiveCharacterNodeTransforms(node);
    Vector4 transformed;
    matrix_stack::TransformPosition(position, &transformed);
    matrix_stack::LoadMatrix(&g_characterNodeSavedTransform);
    position->z = transformed.z;
    position->y = transformed.y;
    position->x = transformed.x;
}

// @verified - d
void InverseTransformPointByCharacterNode(const CharacterNode *node, Vector3 *position) {
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    matrix_stack::UnitCurrentMatrix();
    ApplyRecursiveCharacterNodeTransforms(node);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    matrix_stack::LoadMatrix(&g_characterNodeSavedTransform);
    const Vector4 delta = {position->x - transform.rows[3].x, position->y - transform.rows[3].y, position->z - transform.rows[3].z, 0.0f};
    for (std::int32_t index = 2; index >= 0; --index)
        (*position)[index] = matrix_stack::InnerProduct4(delta, transform.rows[index]);
}

} // namespace s2
} // namespace shenmue
#undef SHENMUE_CHARACTER_POSE_NULL

#endif
