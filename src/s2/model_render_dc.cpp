#include "s2/model_render_dc.hpp"
#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_model_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"

namespace shenmue {
namespace dc {
namespace sdk {

MT7DrawControls g_mt7DrawControls = {};

// @verified - d
void SetMirroredModelDrawMode(std::int32_t enabled) {
    if (!enabled)
        g_mt7DrawControls.flags &= ~MT7_DRAW_MIRRORED;
    else
        g_mt7DrawControls.flags |= MT7_DRAW_MIRRORED;
}

// @verified - d
void SetMT7PolygonFogMode(std::uint32_t mode) {
    g_mt7DrawControls.tsp = (g_mt7DrawControls.tsp & 0xff3fffffU) | (mode << 22U);
}

// @verified - d
void SetMT7PolygonColourClamp(std::uint32_t enabled) {
    g_mt7DrawControls.tsp = (g_mt7DrawControls.tsp & 0xffdfffffU) | (enabled << 21U);
}

// @verified - d
void SetMT7UserClipMode(std::uint32_t mode) {
    g_mt7DrawControls.flags = (g_mt7DrawControls.flags & 0xfffcffffU) | (mode << 16U);
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

namespace shenmue {
namespace s2 {

std::uint32_t g_characterModelDrawFlags = 0;

// @verified - d
void ResetModelDrawState() {
    g_characterModelDrawFlags = 0;
    dc::sdk::SetMirroredModelDrawMode(0);
}

// @verified - d
void DrawPreparedVertexColours(const MT7Node *node, float alpha) {
    const std::int32_t previous = dc::sdk::GetMT7VertexColourType();
    dc::sdk::SetMT7VertexColourType(0);
    charaPrepareNodesForDraw(node, alpha);
    dc::sdk::SetMT7VertexColourType(previous);
}

// @verified - d
void DrawAlphaCharacterModelBranch(MT7Node *node, float alpha) {
    if (node) {
        MT7Node *const sibling = node->sibling;
        node->sibling = 0;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::UnitCurrentMatrix();
        PrepareCharacterModelNodes(node, 0);
        PopCurrentMatrix(1);
        DrawAlphaCharacterModelNodes(node, alpha);
        PopCurrentMatrix(1);
        node->sibling = sibling;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawMT7ProceduralBranch(MT7Node *node, std::int16_t stop, float alpha) {
    if (!node)
        return;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::GetMatrix(&g_characterModelMatrix);
    MT7Node *const sibling = node->sibling;
    node->sibling = 0;
    MT7Node *end;
    if (stop == -1)
        end = 0;
    else
        end = FindMT7Node(node, stop);
    if (end) {
        MT7Node *const child = end->child;
        MT7Node *const next = end->sibling;
        end->child = 0;
        end->sibling = 0;
        DrawAlphaCharacterModelBranch(node, alpha);
        end->child = child;
        end->sibling = next;
    } else
        DrawAlphaCharacterModelBranch(node, alpha);
    PopCurrentMatrix(1);
    node->sibling = sibling;
}

void DrawMT7SingleNode(MT7Node *node, float alpha) {
    if (node) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::GetMatrix(&g_characterModelMatrix);
        MT7Node *const child = node->child;
        MT7Node *const sibling = node->sibling;
        node->child = 0;
        node->sibling = 0;
        DrawAlphaCharacterModelBranch(node, alpha);
        node->child = child;
        node->sibling = sibling;
        PopCurrentMatrix(1);
    }
}

MT7Node *GetMT7ModelRootNode(MT7Node *node) {
    if (!node)
        return 0;
    if (node->id == 0x3e || node->id > 0x7000) {
        MT7Node *child = node->child;
        if (!child)
            return 0;
        child = child->child;
        if (child)
            return child;
        return 0;
    }
    return node;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
