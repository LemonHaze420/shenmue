#include "s2/map_draw_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/character_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

float GetReservedWeatherOverride0();
float GetReservedWeatherOverride1();

__attribute__((aligned(32))) Matrix4x4 objectCommandPlacementTransform = {};
Vector3 mapDrawCameraPosition = {};
MapCellOffset mapDrawCameraCell;
MapMeshDrawCallback g_mapMeshDraw = 0;
MapMeshAlphaDrawCallback g_mapMeshAlphaDraw = 0;

// @verified - d
void SetObjectCommandPlacementTransform() {
    matrix_stack::GetMatrix(&objectCommandPlacementTransform);
}

// @verified - d
void DrawSingleLightMapMesh(void *mesh) {
    const std::uint32_t placement = reinterpret_cast<const MT7MeshHeader *>(mesh)->flags & kMapPlacementMatrixMesh;
    if (placement) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&objectCommandPlacementTransform);
    }
    dc::sdk::DrawSingleLightMT7Mesh(mesh);
    if (placement)
        PopCurrentMatrix(1);
}

// @verified - d
void DrawSingleLightMapMeshAlpha(void *mesh, float alpha) {
    const std::uint32_t placement = reinterpret_cast<const MT7MeshHeader *>(mesh)->flags & kMapPlacementMatrixMesh;
    if (placement) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&objectCommandPlacementTransform);
    }
    dc::sdk::DrawSingleLightMT7MeshAlpha(mesh, alpha);
    if (placement)
        PopCurrentMatrix(1);
}

// @verified - d
void DrawLitMapMesh(void *mesh) {
    const std::uint32_t placement = reinterpret_cast<const MT7MeshHeader *>(mesh)->flags & kMapPlacementMatrixMesh;
    if (placement) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&objectCommandPlacementTransform);
    }
    dc::sdk::DrawLitMT7Mesh(mesh);
    if (placement)
        PopCurrentMatrix(1);
}

// @verified - d
void DrawLitMapMeshAlpha(void *mesh, float alpha) {
    const std::uint32_t placement = reinterpret_cast<const MT7MeshHeader *>(mesh)->flags & kMapPlacementMatrixMesh;
    if (placement) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&objectCommandPlacementTransform);
    }
    dc::sdk::DrawLitMT7MeshAlpha(mesh, alpha);
    if (placement)
        PopCurrentMatrix(1);
}

// @verified - d
void DrawOrdinaryMapCallback(void *) {
    std::int32_t index;
    const MapCellOffset *visible;
    std::int32_t previousLight = 0;
    float previousIntensity = 0.0f;
    const Matrix4x4 &depthTransform = GetFieldDepthTransform();
    Matrix4x4 &depthModelTransform = GetFieldDepthModelTransform();
    camCtrlGetNextCamPos(&mapDrawCameraPosition);
    GetFieldDepthCell(mapDrawCameraPosition, mapDrawCameraCell);
    for (index = 0; index < 32; ++index) {
        Vector3 position;
        Vector3 local;
        MapCellOffset cell;
        Vector4 previousColour;
        Vector4 colour;
        std::int32_t enabled[16];
        Matrix4x4 placement;
        Vector2 angle;
        if (!GetMapModelRoot(index) || !GetMapModelEnabled(index))
            continue;
        const std::int32_t vertexColourType = dc::sdk::GetMT7VertexColourType();
        if (GetMapModelVertexShaderType(index))
            dc::sdk::SetMT7VertexColourType(2);
        else
            dc::sdk::SetMT7VertexColourType(1);
        matrix_stack::Matrix4x4_Push(0);
        if (GetMapModelWeatherLightingOverride(index)) {
            previousLight = GetCurrentActiveLight();
            GetGlobalLighting(&previousColour);
            previousIntensity = GetSelectedLightSceneIntensity(0);
            for (std::int32_t light = 0; light < 16; ++light) {
                enabled[light] = static_cast<std::int16_t>(dc::sdk::g_lightRecords[light][0]);
                dc::sdk::SetLightEnabled(light, 0);
            }
            dc::sdk::SetLightEnabled(0, 1);
            SetCurrentActiveLight(1);
            colour.x = colour.y = colour.z = colour.w = GetReservedWeatherOverride0();
            UpdateGlobalLighting(colour);
            SetSelectedLightSceneIntensity(0, GetReservedWeatherOverride1());
            PublishGlobalLighting();
        }
        if (GetCurrentActiveLight() == 1 || GetMapModelDefaultVSIndex(index)) {
            g_mapMeshDraw = DrawSingleLightMapMesh;
            g_mapMeshAlphaDraw = DrawSingleLightMapMeshAlpha;
        } else {
            g_mapMeshDraw = DrawLitMapMesh;
            g_mapMeshAlphaDraw = DrawLitMapMeshAlpha;
        }
        const std::int32_t mirrored = IsMapModelMirrored(index);
        if (!GetMapModelCommandRoutingEnabled(index) || mirrored) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(&depthTransform);
            matrix_stack::GetMatrix(&depthModelTransform);
            PopCurrentMatrix(1);
            DrawMapModelRoot(GetMapModelRoot(index), index);
            if (mirrored) {
                PopCurrentMatrix(1);
                matrix_stack::Matrix4x4_Push(0);
                dc::sdk::SetMirroredModelDrawMode(1);
                Matrix4x4::MultiplyWithPrePassMatrix(GetCharacterMirrorMatrix());
                DrawMapModelRoot(GetMapModelRoot(index), index);
                dc::sdk::SetMirroredModelDrawMode(0);
            }
        } else {
            const std::int32_t yaw = static_cast<std::int32_t>(GetMapModelRotationY(index));
            const std::int32_t direction = ((fieldInverseYaw - yaw) & 0xffff) >> 12;
            visible = GetMapCellDirectionOffsets(direction);
            const std::int32_t visibleCount = GetMapVisibleCellCount(direction, g_currentDrawTier);
            const MapModelCommandCell *const cells = GetMapModelCommandCells(index);
            const std::int32_t cellCount = static_cast<std::int32_t>(GetMapModelCommandCellCount(index));
            MapModelClipRecord *const *const routing = GetMapModelCommandRouting(index);
            std::int16_t cellX, cellZ;
            GetMapModelPosition(&position, index);
            if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw) {
                const float x = mapDrawCameraPosition.x - position.x, z = mapDrawCameraPosition.z - position.z;
                CalculateFixedSinCos(yaw, &angle);
                local.x = x * angle.y + angle.x * z;
                local.z = x * -angle.x + angle.y * z;
                GetFieldDepthCell(local, cell);
                matrix_stack::PushUnitMatrix();
                Matrix4x4::preTranslate(position.x, position.y, position.z);
                Matrix4x4::RotateAroundY_Wrapper(yaw);
                matrix_stack::GetMatrix(&placement);
                PopCurrentMatrix(1);
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::LoadMatrix(&depthTransform);
                Matrix4x4::MultiplyWithPrePassMatrix(&placement);
                matrix_stack::GetMatrix(&depthModelTransform);
                PopCurrentMatrix(1);
                Matrix4x4::MultiplyWithPrePassMatrix(&placement);
                cellX = cell.x;
                cellZ = cell.z;
            } else {
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::LoadMatrix(&depthTransform);
                matrix_stack::GetMatrix(&depthModelTransform);
                PopCurrentMatrix(1);
                cellX = mapDrawCameraCell.x;
                cellZ = mapDrawCameraCell.z;
            }
            SetObjectCommandPlacementTransform();
            const float alpha = GetMapModelAlpha(index);
            if (alpha != 1.0f) {
                if (!(std::fabs(alpha) < kMapAlphaEpsilon))
                    DrawMapCommandCellsAlpha(visibleCount, visible, cells, routing, cellCount, cellX, cellZ, alpha);
            } else if (g_currentFieldPointCount && GetMapModelGridCullingEnabled(index)) {
                DrawMapCommandCellsDepth(visibleCount, visible, cells, routing, cellCount, cellX, cellZ);
            } else {
                DrawMapCommandCells(visibleCount, visible, cells, routing, cellCount, cellX, cellZ);
            }
        }
        if (GetMapModelWeatherLightingOverride(index)) {
            for (std::int32_t light = 0; light < 16; ++light)
                dc::sdk::SetLightEnabled(light, enabled[light]);
            UpdateGlobalLighting(previousColour);
            SetSelectedLightSceneIntensity(0, previousIntensity);
            SetCurrentActiveLight(previousLight);
            PublishGlobalLighting();
        }
        PopCurrentMatrix(1);
        dc::sdk::SetMT7VertexColourType(vertexColourType);
    }
}

// @verified - d
void DrawMapPlacementCallback(void *) {
    std::int32_t visibleCount;
    const MapCellOffset *visible;
    const MapModelCommandCell *cells;
    MapModelClipRecord *const *routing;
    const Matrix4x4 *const depthTransform = &GetFieldDepthTransform();
    Matrix4x4 *const depthModelTransform = &GetFieldDepthModelTransform();
    for (std::int32_t index = 0; index < 32; ++index) {
        const std::uint32_t mapSlot = GetMagicRoomFlag(index);
        if (mapSlot == 0x7fff || !GetMapLightingPlacementEnabled(index) || !GetMapModelRoot(static_cast<std::int32_t>(mapSlot)))
            continue;
        const std::int32_t vertexColourType = dc::sdk::GetMT7VertexColourType();
        if (GetMapModelVertexShaderType(static_cast<std::int32_t>(mapSlot)))
            dc::sdk::SetMT7VertexColourType(2);
        else
            dc::sdk::SetMT7VertexColourType(1);
        matrix_stack::Matrix4x4_Push(0);
        if (GetPreviousActiveLight() == 1 || GetMapModelDefaultVSIndex(mapSlot)) {
            g_mapMeshDraw = DrawSingleLightMapMesh;
            g_mapMeshAlphaDraw = DrawSingleLightMapMeshAlpha;
        } else {
            g_mapMeshDraw = DrawLitMapMesh;
            g_mapMeshAlphaDraw = DrawLitMapMeshAlpha;
        }
        const std::int32_t yaw = static_cast<std::int32_t>(GetMapLightingPlacementRotation(index));
        const std::int32_t direction = ((fieldInverseYaw - yaw) & 0xffff) >> 12;
        visible = GetMapCellDirectionOffsets(direction);
        visibleCount = GetMapVisibleCellCount(direction, g_currentDrawTier);
        cells = GetMapModelCommandCells(static_cast<std::int32_t>(mapSlot));
        const std::int32_t cellCount = static_cast<std::int32_t>(GetMapModelCommandCellCount(static_cast<std::int32_t>(mapSlot)));
        routing = GetMapModelCommandRouting(static_cast<std::int32_t>(mapSlot));
        Vector3 position;
        Vector3 local;
        MapCellOffset cell;
        Matrix4x4 transform;
        Vector2 angle;
        std::int16_t cellX, cellZ;
        GetMapLightingPlacementPosition(&position, index);
        if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw) {
            const float x = mapDrawCameraPosition.x - position.x, z = mapDrawCameraPosition.z - position.z;
            CalculateFixedSinCos(yaw, &angle);
            local.x = x * angle.y + angle.x * z;
            local.z = x * -angle.x + angle.y * z;
            GetFieldDepthCell(local, cell);
            matrix_stack::PushUnitMatrix();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            Matrix4x4::RotateAroundY_Wrapper(yaw);
            matrix_stack::GetMatrix(&transform);
            PopCurrentMatrix(1);
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(depthTransform);
            Matrix4x4::MultiplyWithPrePassMatrix(&transform);
            matrix_stack::GetMatrix(depthModelTransform);
            PopCurrentMatrix(1);
            Matrix4x4::MultiplyWithPrePassMatrix(&transform);
            cellX = cell.x;
            cellZ = cell.z;
        } else {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(depthTransform);
            matrix_stack::GetMatrix(depthModelTransform);
            PopCurrentMatrix(1);
            cellX = mapDrawCameraCell.x;
            cellZ = mapDrawCameraCell.z;
        }
        SetObjectCommandPlacementTransform();
        if (g_currentFieldPointCount && GetMapLightingPlacementActive(index))
            DrawMapCommandCellsDepth(visibleCount, visible, cells, routing, cellCount, cellX, cellZ);
        else
            DrawMapCommandCells(visibleCount, visible, cells, routing, cellCount, cellX, cellZ);
        PopCurrentMatrix(1);
        dc::sdk::SetMT7VertexColourType(vertexColourType);
    }
}

} // namespace s2
} // namespace shenmue

#endif
