#include "s2/prop_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/model_render_dc.hpp"

namespace shenmue {
namespace s2 {

__attribute__((aligned(32))) Matrix4x4 g_propDrawBase = {};
void (*g_propDrawMesh)(void *mesh) = 0;

// @verified - d
void DrawPropObjectHierarchy(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        am2LoadXmtrxPairs(&g_propDrawBase);
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    }
    if (node->mesh && !node->kind) {
        if (reinterpret_cast<const MT7MeshHeader *>(node->mesh)->flags & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        g_propDrawMesh(node->mesh);
    }
    if (node->child)
        DrawPropObjectHierarchy(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawPropObjectHierarchy(node->sibling);
    }
}

std::int32_t IsCharacterMirrorEnabled();

// @verified - d
void DrawPropObjectCallback(void *) {
    Vector3 position;
    const std::int32_t vertexColourType = dc::sdk::GetMT7VertexColourType();
    MT7Node *node = GetPropRoot();
    if (!node)
        return;
    const Matrix4x4 &depthTransform = GetFieldDepthTransform();
    Matrix4x4 &depthModelTransform = GetFieldDepthModelTransform();
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&depthTransform);
    matrix_stack::GetMatrix(&depthModelTransform);
    PopCurrentMatrix(1);
    am2StoreXmtrx(&g_propDrawBase);
    dc::sdk::SetMT7VertexColourType(1);
    matrix_stack::Matrix4x4_Push(0);
    GetPropVector4(&position);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    if (GetPreviousActiveLight() == 1 || GetPropLightingTraversalResult())
        g_propDrawMesh = dc::sdk::DrawSingleLightMT7Mesh;
    else
        g_propDrawMesh = dc::sdk::DrawLitMT7Mesh;
    node = node->child;
    position.x = position.y = position.z = 0.0f;
    for (; node; node = node->sibling) {
        if (camCtrlIsSphereVisible(node->position, kPropObjectRadius) && camCtrlIsModelDepthVisible(position, *node, kPropDepthRadius)) {
            MT7Node *const sibling = node->sibling;
            node->sibling = 0;
            matrix_stack::Matrix4x4_Push(0);
            DrawPropObjectHierarchy(node);
            PopCurrentMatrix(1);
            node->sibling = sibling;
        }
    }
    PopCurrentMatrix(1);
    if (IsCharacterMirrorEnabled()) {
        matrix_stack::Matrix4x4_Push(0);
        dc::sdk::SetMirroredModelDrawMode(1);
        Matrix4x4::MultiplyWithPrePassMatrix(GetCharacterMirrorMatrix());
        node = GetPropRoot()->child;
        position.x = position.y = position.z = 0.0f;
        for (; node; node = node->sibling) {
            if (camCtrlIsSphereVisible(node->position, kPropObjectRadius) && camCtrlIsModelDepthVisible(position, *node, kPropDepthRadius)) {
                MT7Node *const sibling = node->sibling;
                node->sibling = 0;
                matrix_stack::Matrix4x4_Push(0);
                DrawPropObjectHierarchy(node);
                PopCurrentMatrix(1);
                node->sibling = sibling;
            }
        }
        dc::sdk::SetMirroredModelDrawMode(0);
        PopCurrentMatrix(1);
    }
    dc::sdk::SetMT7VertexColourType(vertexColourType);
}

} // namespace s2
} // namespace shenmue

#endif
