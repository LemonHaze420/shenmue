#include "s2/character_transparency_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_model_dc.hpp"
#include "s2/hand_draw_dc.hpp"
#include "s2/model_clone_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/event_matrix_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

std::int32_t g_characterSearchMeshSuppressed = 0;
std::uint32_t g_motionDrawOverrideFlags = 0;
float g_characterTransparencyAlpha, g_searchHandBlend = 0.0f;
CharacterTransparencyMeshCallback g_characterTransparencyMeshCallback, g_characterHandMeshCallback = 0;

// @verified - d
float SearchIK_GetMaxBlend() {
    return g_searchHandBlend;
}

// @verified - d
void setSearchIKFlag(float blend) {
    g_searchHandBlend = blend;
}

// @verified - d
void DrawSingleLightCharacterMeshAlpha(void *mesh) {
    dc::sdk::DrawSingleLightMT7MeshAlpha(mesh, g_characterTransparencyAlpha);
}

// @verified - d
void DrawLitCharacterMeshAlpha(void *mesh) {
    dc::sdk::DrawLitMT7MeshAlpha(mesh, g_characterTransparencyAlpha);
}

// @verified - d
void DrawCharacterHandHierarchy(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (!g_characterClothDrawDisabled && node->mesh && node->parent && node->parent->mesh) {
        if (reinterpret_cast<const MT7LinkedVertexData *>(node + 1)->type == MODEL_CLOTH_CHUNK) {
            const Matrix4x4 *const bind = &reinterpret_cast<const MT7LinkedVertexData *>(node + 1)->bindTransform;
            std::int32_t index;
            const MT7VertexLink *link = reinterpret_cast<const MT7VertexLink *>(bind + 1);
            const std::int32_t count = *reinterpret_cast<const std::int32_t *>(link);
            link = reinterpret_cast<const MT7VertexLink *>(reinterpret_cast<const std::int32_t *>(link) + 1);
            if (count > 0) {
                matrix_stack::PushUnitMatrix();
                if (node->rotation.x)
                    Matrix4x4::RotateAroundX_Wrapper(-node->rotation.x);
                if (node->rotation.y)
                    Matrix4x4::RotateAroundY_Wrapper(-node->rotation.y);
                if (node->rotation.z)
                    Matrix4x4::RotateAroundZ_Wrapper(-node->rotation.z);
                Matrix4x4::preTranslate(-node->position.x, -node->position.y, -node->position.z);
                std::uint32_t *const destination = static_cast<std::uint32_t *>(node->mesh);
                const std::uint32_t *const source = static_cast<std::uint32_t *>(node->parent->mesh);
                for (index = 0; index < count; ++index, ++link) {
                    const std::uint32_t sourceWord = link->sourceOffset >> 2;
                    const std::uint32_t word = link->destinationOffset >> 2;
                    const Vector3 *const input = reinterpret_cast<const Vector3 *>(source + sourceWord);
                    Vector3 *const output = reinterpret_cast<Vector3 *>(destination + word);
                    Matrix4x4::TransformPoint(input[0], output[0]);
                    Matrix4x4::Transform(input[1], output[1]);
                    destination[word] |= 1U;
                }
                if (g_characterVertexBlendEnabled && *reinterpret_cast<const std::int32_t *>(link))
                    ApplyMT7BlendedVertices(node, link);
                PopCurrentMatrix(1);
            }
        }
    }
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (!g_characterSearchMeshSuppressed && node->mesh)
        g_characterHandMeshCallback(node->mesh);
    if (node->child)
        DrawCharacterHandHierarchy(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawCharacterHandHierarchy(node->sibling);
    }
}

// @verified - d
void DrawCharacterAlphaNode(const MT7Node *node) {
    if (!node->mesh)
        return;
    const float previousAlpha = g_characterTransparencyAlpha;
    const float factor = hmdlGetDrawAlphaScaleEffect(node, previousAlpha);
    g_characterTransparencyAlpha *= factor;
    if (g_characterTransparencyAlpha > kCharacterVisibleAlpha) {
        if (!g_characterClothDrawDisabled && node->transformOverride && node->parent && node->parent->mesh && node->parent->transformOverride)
            ApplyMT7ParentLinkedVertices(node, node->parent, node->transformOverride, node->parent->transformOverride);
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1) {
            if (g_characterTransparencyAlpha < 1.0f)
                g_characterHandMeshCallback = DrawSingleLightCharacterMeshAlpha;
            else
                g_characterHandMeshCallback = dc::sdk::DrawSingleLightMT7Mesh;
        } else {
            if (g_characterTransparencyAlpha < 1.0f)
                g_characterHandMeshCallback = DrawLitCharacterMeshAlpha;
            else
                g_characterHandMeshCallback = dc::sdk::DrawLitMT7Mesh;
        }
        if (!g_characterSearchMeshSuppressed && node->mesh)
            g_characterHandMeshCallback(node->mesh);
        if (node->child)
            DrawCharacterHandHierarchy(node->child);
    }
    g_characterTransparencyAlpha = previousAlpha;
}

// @verified - d
void DrawCharacterSearchNode(const MT7Node *node) {
    if (!g_characterSearchMeshSuppressed && node->mesh)
        g_characterTransparencyMeshCallback(node->mesh);
    if (node->child)
        DrawCharacterTransparencyHierarchy(node->child);
}

// @verified - d
void DrawCharacterSkinnedSearchNode(const MT7Node *node) {
    bool restoreSuppression = false;
    if (node->mesh) {
        if (!g_characterClothDrawDisabled) {
            if (IsMT7ClothNode(node))
                ApplyMT7ClothDeformation(node);
            else if (node->transformOverride && node->parent && node->parent->mesh && node->parent->transformOverride)
                ApplyMT7ParentLinkedVertices(node, node->parent, node->transformOverride, node->parent->transformOverride);
        }
        if ((node->id == 5 && (g_motionDrawOverrideFlags & 1U)) || (node->id == 10 && (g_motionDrawOverrideFlags & 2U))) {
            g_characterSearchMeshSuppressed = 0;
            restoreSuppression = true;
        }
        if (!g_characterSearchMeshSuppressed)
            g_characterTransparencyMeshCallback(node->mesh);
    }
    if (node->child)
        DrawCharacterTransparencyHierarchy(node->child);
    if (restoreSuppression)
        g_characterSearchMeshSuppressed = 1;
}

// @verified - d
void DrawCharacterTransparencyHierarchy(const MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        am2LoadXmtrxPairs(&g_characterModelMatrix);
        dc::sdk::MultiplyMT7ProjectionRows(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    }
    const std::uint16_t kind = node->kind;
    if (kind) {
        kCharacterTransparencyNodeCallbacks[kind - 1](node);
    } else {
        if (node->mesh) {
            if (!g_characterClothDrawDisabled && node->transformOverride && node->parent && node->parent->mesh && node->parent->transformOverride)
                ApplyMT7ParentLinkedVertices(node, node->parent, node->transformOverride, node->parent->transformOverride);
            if ((!g_characterSearchMeshSuppressed || node->id == 1) && node->mesh)
                g_characterTransparencyMeshCallback(node->mesh);
        }
        if (node->child)
            DrawCharacterTransparencyHierarchy(node->child);
    }
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawCharacterTransparencyHierarchy(node->sibling);
    }
}

// @verified - d
void mtUpdateTransparencyCallback(const MT7Node *node, float alpha) {
    matrix_stack::Matrix4x4_Push(0);
    am2StoreXmtrx(&g_characterModelMatrix);
    g_characterTransparencyAlpha = alpha;
    if (g_motionDrawOverrideFlags) {
        g_characterSearchMeshSuppressed = 1;
        g_characterTransparencyAlpha = g_searchHandBlend;
    } else {
        g_characterSearchMeshSuppressed = 0;
    }
    if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1 && static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) != 1) {
        if (g_characterTransparencyAlpha < 1.0f)
            g_characterTransparencyMeshCallback = DrawSingleLightCharacterMeshAlpha;
        else
            g_characterTransparencyMeshCallback = dc::sdk::DrawSingleLightMT7Mesh;
    } else {
        if (g_characterTransparencyAlpha < 1.0f)
            g_characterTransparencyMeshCallback = DrawLitCharacterMeshAlpha;
        else
            g_characterTransparencyMeshCallback = dc::sdk::DrawLitMT7Mesh;
    }
    DrawCharacterTransparencyHierarchy(node);
    PopCurrentMatrix(1);
}

} // namespace s2
} // namespace shenmue

#endif
