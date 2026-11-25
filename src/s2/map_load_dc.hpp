#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/lighting.hpp"
#include "s2/map_info.hpp"

namespace shenmue {
namespace s2 {

struct MapLoadState {
    MapLightingPlacement placements[32];
    std::int32_t layers[32];
};
typedef char Layout_map_load_dc_0[(offsetof(MapLoadState, layers) == 0x380) ? 1 : -1];
typedef char Layout_map_load_dc_1[(sizeof(MapLoadState) == 0x400) ? 1 : -1];

struct MapModelCache {
    std::int32_t enabled, gridCulling, fieldDepth;
    float farDistance, nearDistance, npcRadius;
};
typedef char Layout_map_load_dc_2[(offsetof(MapModelCache, farDistance) == 0xc) ? 1 : -1];
typedef char Layout_map_load_dc_3[(sizeof(MapModelCache) == 0x18) ? 1 : -1];

#define PROP_MODEL_STORAGE SHEN_NAME4('P', 'R', 'O', 'P')
extern MapLoadState g_mapLoadState;
extern MapModelCache g_mapModelCache;
extern shen_name g_mapAreaOverride;

void CacheMapCameraDistances(std::int32_t restore);
void CacheMapModelEnabled(std::int32_t restore);
void CacheMapFieldDepth(std::int32_t restore);
void CacheMapGridCulling(std::int32_t restore);
void MapModelSlotCacheUtil(std::int32_t operation);
void ResetMapModelDrawCache();
void InitializeMapLightingPlacement(MapLightingPlacement *placement);
void SetupOwnedMapModel(void *source, std::int32_t index);
void LoadEntryMapLayers(const char *directory);
void LoadMapPacks(std::int32_t count, const char *directory);
void LoadMapPackages(std::int32_t count, const char *directory);
std::int32_t ReadMapPackCount(const MapInfoChunk *chunk);
const shen_name *ReadMapAreaOverride(const MapInfoChunk *chunk);
void ReadMapBlockSize(const MapInfoChunk *chunk);
bool ReadEntryMapLayers(const MapInfoChunk *chunk);
void ReadMapDisabledLayers(const MapInfoChunk *chunk);
std::int32_t ReadMapPackageMode(const MapInfoChunk *chunk);
void LoadMap(std::int32_t act, shen_name area, MapInfoChunk *mapInfo);
void LoadPROP(const char *directory, const char *name);
void LoadPROPs(std::int32_t act, shen_name area);
void Mapinfo_MAP_Reader(std::int32_t act, shen_name area, void *mapInfo);
void InitializeMapVertexCache();
struct MapModelClipRecord;
struct MapModelCommandCell;
void SetMapModelClipRecords(MapModelClipRecord *value, std::int32_t index);
std::uint32_t GetMapModelEnabled(std::int32_t index);
float GetMapModelHeight(std::int32_t index);
void SetMapModelHeight(std::int32_t index, float height);
float GetMapModelAlpha(std::int32_t index);
void SetMapModelAlpha(std::int32_t index, float alpha);
std::uint32_t GetMapModelWeatherLightingOverride(std::int32_t index);
std::uint32_t GetMapModelCommandCellCount(std::int32_t index);
void SetMapModelCommandCellCount(std::uint32_t value, std::int32_t index);
MapModelCommandCell *GetMapModelCommandCells(std::int32_t index);
void SetMapModelCommandCells(MapModelCommandCell *value, std::int32_t index);
MapModelClipRecord **GetMapModelCommandRouting(std::int32_t index);
void SetMapModelCommandRouting(MapModelClipRecord **value, std::int32_t index);
std::uint32_t GetMapModelAlternateTraversalEnabled(std::int32_t index);
std::uint32_t GetMapModelDrawableRecordCount(std::int32_t index);
void SetMapModelDrawableRecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelDangerRecordCount(std::int32_t index);
void SetMapModelDangerRecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelScreenSpaceRecordCount(std::int32_t index);
void SetMapModelScreenSpaceRecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelNearRecordCount(std::int32_t index);
void SetMapModelNearRecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelFarRecordCount(std::int32_t index);
void SetMapModelFarRecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelType4RecordCount(std::int32_t index);
void SetMapModelType4RecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelWallRecordCount(std::int32_t index);
void SetMapModelWallRecordCount(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelWallCRecordCount(std::int32_t index);
void SetMapModelWallCRecordCount(std::uint32_t value, std::int32_t index);
void SetMapLightingPlacementActive(std::uint32_t value, std::int32_t index);
std::uint32_t GetMapModelDefaultVSIndex(std::uint32_t index);
std::uint32_t GetMapModelVertexShaderType(std::int32_t index);

} // namespace s2
} // namespace shenmue

#endif
