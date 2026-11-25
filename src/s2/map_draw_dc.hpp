#pragma once

#include "s2/field_depth_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

typedef void (*MapMeshDrawCallback)(void *mesh);
typedef void (*MapMeshAlphaDrawCallback)(void *mesh, float alpha);
typedef bool (*MapVisibilityTest)(const Vector3 &position, float radius);
static const MapVisibilityTest mapObjectVisibilityTests[17]= {camCtrlIsSphereVisible, 0, 0, 0,
    camCtrlIsSphereVisible, 0, 0, 0, camCtrlIsNearSphereVisible, 0, 0, 0,
    camCtrlIsFarSphereVisible, 0, 0, 0, camCtrlIsScreenSpaceVisible};
static const std::uint32_t kMapPlacementMatrixMesh = 0x40;
static const std::int16_t kMapScreenSpaceCell = 0x4fff;
static const float kMapAlphaEpsilon = 1.1920928955078125e-07f;
extern Matrix4x4 objectCommandPlacementTransform;
extern Vector3 mapDrawCameraPosition;
extern MapCellOffset mapDrawCameraCell;
extern MapMeshDrawCallback g_mapMeshDraw;
extern MapMeshAlphaDrawCallback g_mapMeshAlphaDraw;
extern std::uint32_t mapObjectClipRecordIndex;
extern const MapModelClipRecord *mapObjectClipRecords, *activeMapObjectClipRecord;
extern Matrix4x4 mapCommandMatrix, mapAlphaCommandMatrix;
void DrawOrdinaryMapCallback(void *data);
void DrawMapPlacementCallback(void *data);
void SetObjectCommandPlacementTransform();
void DrawSingleLightMapMesh(void *mesh);
void DrawSingleLightMapMeshAlpha(void *mesh, float alpha);
void DrawLitMapMesh(void *mesh);
void DrawLitMapMeshAlpha(void *mesh, float alpha);
void DrawMapModelRoot(const MT7Node *node, std::int32_t index);
void DrawMapModelNodes(const MT7Node *node, bool depthCull);
void CacheMapCommandMatrix();
void CacheAlphaMapCommandMatrix();
void DrawMapCommandRecords(MapModelClipRecord *const *records, std::int32_t count);
void DrawMapDepthCommandRecords(MapModelClipRecord *const *records, std::int32_t count);
void DrawMapScreenCommandRecords(MapModelClipRecord *const *records, std::int32_t count);
void DrawMapAlphaCommandRecords(MapModelClipRecord *const *records, std::int32_t count, float alpha);
void DrawMapScreenAlphaCommandRecords(MapModelClipRecord *const *records, std::int32_t count, float alpha);
void DrawMapCommandCells(std::int32_t visibleCount, const MapCellOffset *visible, const MapModelCommandCell *cells,
    MapModelClipRecord *const *routing, std::int32_t cellCount, std::int16_t cellX, std::int16_t cellZ);
void DrawMapCommandCellsDepth(std::int32_t visibleCount, const MapCellOffset *visible, const MapModelCommandCell *cells,
    MapModelClipRecord *const *routing, std::int32_t cellCount, std::int16_t cellX, std::int16_t cellZ);
void DrawMapCommandCellsAlpha(std::int32_t visibleCount, const MapCellOffset *visible, const MapModelCommandCell *cells,
    MapModelClipRecord *const *routing, std::int32_t cellCount, std::int16_t cellX, std::int16_t cellZ, float alpha);

} } // namespace shenmue::s2

#endif
