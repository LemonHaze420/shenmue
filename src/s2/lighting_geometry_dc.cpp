#include "s2/lighting_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/model_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/model_clone_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

typedef char Layout_lighting_geometry_dc_0[(sizeof(LightRuntimeEntry) == sizeof(dc::sdk::MT7GeometryLight)) ? 1 : -1];

typedef char Layout_lighting_geometry_dc_1[(offsetof(LightRuntimeEntry, vector20) == offsetof(dc::sdk::MT7GeometryLight, position)) ? 1 : -1];

typedef char Layout_lighting_geometry_dc_2[(offsetof(LightRuntimeEntry, values2c) == offsetof(dc::sdk::MT7GeometryLight, direction)) ? 1 : -1];

const Matrix4x4 *g_geometryLightTransform = 0;

namespace {

const LightRuntimeEntry *geometryLightEntries = 0;

} // namespace

namespace {

const Vector4 *geometryLightColour = 0;

} // namespace

namespace {

std::int32_t geometryLightEntryCount;

} // namespace

namespace {

std::uint32_t geometryLightingRequired;

} // namespace

namespace {

// @verified - d
void TraverseGeometryLightTree(MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::LoadMatrix(&g_characterModelMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(node->transformOverride);
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f) {
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            dc::sdk::SetModelScale(node->scale.x);
        }
    }
    if (node->mesh && !(node->kind & 0x10)) {
        dc::sdk::ResetMT7GeometryLightMesh(node->mesh);
        if (geometryLightingRequired) {
            const std::uint32_t alternateTransform = reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 0x40U;
            if (g_geometryLightTransform && alternateTransform) {
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::LoadMatrix(g_geometryLightTransform);
            }
            dc::sdk::BakeMT7GeometryLightMesh(node->mesh, geometryLightEntryCount, geometryLightEntries, geometryLightColour);
            if (g_geometryLightTransform && alternateTransform)
                PopCurrentMatrix(1);
        }
    }
    if (node->child)
        TraverseGeometryLightTree(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        TraverseGeometryLightTree(node->sibling);
    }
}

} // namespace

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetGeometryLightTree(MT7Node *node) {
    if (node->mesh && !(node->kind & 0x10))
        dc::sdk::ResetMT7GeometryLightMesh(node->mesh);
    if (node->child)
        ResetGeometryLightTree(node->child);
    if (node->sibling)
        ResetGeometryLightTree(node->sibling);
}
#endif

} // namespace

// @verified - d
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour) {
    geometryLightingRequired = 0;
    if (root) {
        const std::int32_t previous = dc::sdk::GetMT7VertexColourType();
        dc::sdk::SetMT7VertexColourType(1);
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::GetMatrix(&g_characterModelMatrix);
        geometryLightEntries = lights;
        geometryLightEntryCount = count;
        geometryLightColour = colour;
        if (GetCurrentActiveLight() != 1) {
            for (std::int32_t index = 0; index < geometryLightEntryCount; ++index) {
                if (geometryLightEntries[index].indexes[1] & 8U) {
                    geometryLightingRequired = 1;
                    break;
                }
            }
        }
        TraverseGeometryLightTree(root);
        PopCurrentMatrix(1);
        dc::sdk::SetMT7VertexColourType(previous);
    }
    return geometryLightingRequired;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void AssignLightProgram(std::uint32_t mapSlot) {
    if (mapSlot > 31)
        return;
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    AllocateGeometryLightPositionScratch();
    MT7Node *const root = GetMapModelRoot(static_cast<std::int32_t>(mapSlot));
    if (root) {
        Matrix4x4 transform;
        Vector3 position;
        AssignLightForMapLayer(&position, static_cast<std::int32_t>(mapSlot));
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        matrix_stack::GetMatrix(&transform);
        SetGeometryLightTreeTarget(&transform);
        const std::uint32_t result = ApplyGeometryLightTree(root, g_lightRuntimeEntries, 256, &colour);
        SetGeometryLightTreeTarget(0);
        PopCurrentMatrix(1);
        g_mapModelSlots[mapSlot].renderMode = result;
    }
    RestoreGeometryLightPositionScratch();
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void PushGlobalLightingProgram() {
    Vector4 colour;
    Matrix4x4 transform;
    Vector3 position;
    if (GetGeometryLightColour(&colour))
        return;
    AllocateGeometryLightPositionScratch();
    for (std::int32_t index = 0; index < 32; ++index) {
        MT7Node *const root = GetMapModelRoot(index);
        if (!root)
            continue;
        AssignLightForMapLayer(&position, index);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        matrix_stack::GetMatrix(&transform);
        SetGeometryLightTreeTarget(&transform);
        const std::uint32_t result = ApplyGeometryLightTree(root, g_lightRuntimeEntries, 256, &colour);
        SetGeometryLightTreeTarget(0);
        PopCurrentMatrix(1);
        g_mapModelSlots[index].renderMode = result;
    }
    MT7Node *const root = GetPropRoot();
    if (root) {
        GetPropVector4(&position);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        const std::uint32_t result = ApplyGeometryLightTree(root, g_lightRuntimeEntries, 256, &colour);
        PopCurrentMatrix(1);
        g_propState.lightingTraversalResult = result;
    }
    RestoreGeometryLightPositionScratch();
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetupModelDataDefaultVS(MT7Node *root) {
    if (!root)
        return;
    const std::int32_t previous = dc::sdk::GetMT7VertexColourType();
    dc::sdk::SetMT7VertexColourType(1);
    matrix_stack::Matrix4x4_Push(0);
    ResetGeometryLightTree(root);
    PopCurrentMatrix(1);
    dc::sdk::SetMT7VertexColourType(previous);
}
#endif

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" {
void __attribute__((renesas)) shenmue_s2_event_AssignLightProgram(std::uint32_t mapSlot) {
    shenmue::s2::AssignLightProgram(mapSlot);
}
void __attribute__((renesas)) shenmue_s2_event_PushGlobalLightingProgram() {
    shenmue::s2::PushGlobalLightingProgram();
}
void __attribute__((renesas)) shenmue_s2_event_ApplyCharacterGeometryLighting(shenmue::s2::Character *character, std::int32_t replaceDrawCallback) {
    shenmue::s2::ApplyCharacterGeometryLighting(character, replaceDrawCallback);
}
void __attribute__((renesas)) shenmue_s2_event_SetDefaultVS() {
    shenmue::s2::SetDefaultVS();
}
}
#endif

#endif
