#include "s2/character_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/math.hpp"

namespace shenmue {
namespace s2 {

__attribute__((aligned(32))) Matrix4x4 g_characterNodeSavedTransform;

// @verified - d
void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node) {
    if (node->parent)
        ApplyRecursiveCharacterNodeTransforms(node->parent);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
}

// @verified - d
void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node) {
    if (node->parent)
        ApplyRecursiveCharacterNodeRotations(node->parent);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
}

// @verified - d
void GetNodeWorldPosition(const CharacterNode *node, Vector3 *position) {
    if (!node->parent) {
        *position = node->position;
        return;
    }
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    matrix_stack::UnitCurrentMatrix();
    ApplyRecursiveCharacterNodeTransforms(node);
    matrix_stack::GetTranslation(position);
    matrix_stack::LoadMatrix(&g_characterNodeSavedTransform);
}

// @verified - d
void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation) {
    if (!node->parent) {
        *rotation = node->rotation;
        return;
    }
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    const Matrix4x4 saved = g_characterNodeSavedTransform;
    matrix_stack::UnitCurrentMatrix();
    ApplyRecursiveCharacterNodeTransforms(node);
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    matrix_stack::LoadMatrix(&saved);
    ExtractMatrixEulerAnglesFixed(&g_characterNodeSavedTransform, rotation);
}

// @verified - d
void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position) {
    if (!node->parent) {
        node->position = position;
        return;
    }
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    matrix_stack::UnitCurrentMatrix();
    ApplyRecursiveCharacterNodeTransforms(node->parent);
    Matrix4x4 parent;
    matrix_stack::GetMatrix(&parent);
    matrix_stack::LoadMatrix(&g_characterNodeSavedTransform);
    const Vector4 delta = {SHEN_FIELD(x) position.x - parent.rows[3].x, SHEN_FIELD(y) position.y - parent.rows[3].y,
        SHEN_FIELD(z) position.z - parent.rows[3].z, SHEN_FIELD(w) 0.0f};
    for (std::uint32_t index = 0; index < 3; ++index) {
        const float *axis = parent.rows[index].data();
        const float *translation = delta.data();
        float local;
        asm volatile("fmov @%1+,fr4\n\tfmov @%1+,fr5\n\tfmov @%1+,fr6\n\tfmov @%1,fr7\n\t"
                     "fmov @%2+,fr8\n\tfmov @%2+,fr9\n\tfmov @%2+,fr10\n\tfmov @%2,fr11\n\tfipr fv4,fv8\n\tfmov fr11,%0"
            : "=&f"(local), "+r"(translation), "+r"(axis)
            :
            : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "memory");
        node->position[index] = local;
    }
}

// @verified - d
void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation) {
    Matrix4x4 transform;
    Vector3 position;
    if (node->parent) {
        matrix_stack::PushUnitMatrix();
        ApplyRecursiveCharacterNodeTransforms(node->parent);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        matrix_stack::GetTranslation(&position);
        matrix_stack::InvertMatrix(0);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
        matrix_stack::GetMatrix(&transform);
        PopCurrentMatrix(1);
        ExtractMatrixEulerAnglesFixed(&transform, &node->rotation);
    } else {
        node->rotation.x = rotation.x;
        node->rotation.y = rotation.y;
        node->rotation.z = rotation.z;
    }
}

} // namespace s2
} // namespace shenmue

#endif
