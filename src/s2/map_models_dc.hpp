#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/model_dc.hpp"

#define SHENMUE_MAP_MODEL_CHECK(name, test) typedef char name[(test) ? 1 : -1]

namespace shenmue {
namespace s2 {

struct PakGlobalData;
struct MapModelClipRecord {
    Vector3 centre;
    float radius;
    std::int16_t cellX, cellZ;
    const void *object;
    std::uint32_t commandType;
    float modelScale;
};
struct MapCellOffset {
    std::int16_t x, z;
};
struct MapModelCommandCell {
    std::int16_t x, z;
    std::int32_t count;
    std::int32_t firstRecord;
};
struct MapFieldDepthWall {
    Vector3 first, second;
};
struct MapFieldDepthSlot {
    Vector4 *dangerSpheres;
    std::int32_t dangerCount;
    std::uint32_t enabled;
    std::int32_t dangerCellCount;
    MapModelCommandCell *dangerCells;
    MapFieldDepthWall *walls;
    std::int32_t wallCount;
    MapFieldDepthWall *cameraWalls;
    std::int32_t cameraWallCount;
};
struct MapModelRuntimeSlot {
    std::uint32_t enabled;
    MapModelClipRecord *clipRecords;
    std::uint32_t ownsReader;
    MT7Model *reader;
    std::uint32_t commandCellCount;
    MapModelCommandCell *commandCells;
    MapModelClipRecord **commandRouting;
    Vector3 position;
    std::uint32_t rotationY;
    std::uint32_t usesCommandRouting;
    std::uint32_t alternateTraversalEnabled;
    std::uint32_t gridCullingEnabled;
    std::uint32_t drawableRecordCount;
    std::uint32_t screenSpaceRecordCount;
    std::uint32_t nearRecordCount;
    std::uint32_t dangerRecordCount;
    std::uint32_t farRecordCount;
    std::uint32_t type4RecordCount;
    std::uint32_t wallRecordCount;
    std::uint32_t wallCRecordCount;
    void *auxiliaryAllocation;
    std::uint32_t renderMode;
    std::uint32_t drawMode;
    float alpha;
    std::uint32_t weatherLightingOverride;
};
SHENMUE_MAP_MODEL_CHECK(MapClipRecordSizeCheck, sizeof(MapModelClipRecord) == 0x20);
SHENMUE_MAP_MODEL_CHECK(MapClipRecordObjectCheck, offsetof(MapModelClipRecord, object) == 0x14);
SHENMUE_MAP_MODEL_CHECK(MapCommandCellSizeCheck, sizeof(MapModelCommandCell) == 0x0c);
SHENMUE_MAP_MODEL_CHECK(MapDepthWallSizeCheck, sizeof(MapFieldDepthWall) == 0x18);
SHENMUE_MAP_MODEL_CHECK(MapDepthSlotSizeCheck, sizeof(MapFieldDepthSlot) == 0x24);
SHENMUE_MAP_MODEL_CHECK(MapDepthDangerCellsCheck, offsetof(MapFieldDepthSlot, dangerCells) == 0x10);
SHENMUE_MAP_MODEL_CHECK(MapDepthWallsCheck, offsetof(MapFieldDepthSlot, walls) == 0x14);
SHENMUE_MAP_MODEL_CHECK(MapDepthCameraWallsCheck, offsetof(MapFieldDepthSlot, cameraWalls) == 0x1c);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeSlotSizeCheck, sizeof(MapModelRuntimeSlot) == 0x6c);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeReaderCheck, offsetof(MapModelRuntimeSlot, reader) == 0x0c);
SHENMUE_MAP_MODEL_CHECK(MapRuntimePositionCheck, offsetof(MapModelRuntimeSlot, position) == 0x1c);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeRotationCheck, offsetof(MapModelRuntimeSlot, rotationY) == 0x28);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeAllocationCheck, offsetof(MapModelRuntimeSlot, auxiliaryAllocation) == 0x58);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeRenderModeCheck, offsetof(MapModelRuntimeSlot, renderMode) == 0x5c);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeDrawModeCheck, offsetof(MapModelRuntimeSlot, drawMode) == 0x60);
SHENMUE_MAP_MODEL_CHECK(MapRuntimeAlphaCheck, offsetof(MapModelRuntimeSlot, alpha) == 0x64);
#define SHENMUE_MAP_MODEL_COMMAND_TYPES {0, 0, 255, 240, 4, 0, 8, 0, 244, 16, 0, 0}
extern const std::uint32_t mapModelCommandTypes[12];
inline shen_name MapClipStorageName() { return shen_name(SHEN_NAME4('C', 'L', 'I', 'P')); }
inline shen_name MapWallStorageName() { return shen_name(SHEN_NAME4('W', 'A', 'L', 'L')); }
inline shen_name MapCameraWallStorageName() { return shen_name(SHEN_NAME4('W', 'A', 'L', 'C')); }
inline shen_name MapCommandCellsName() { return shen_name(SHEN_NAME4('A', 'C', 'P', 'H')); }
inline shen_name MapCommandRoutingName() { return shen_name(SHEN_NAME4('A', 'C', 'P', 'D')); }
inline shen_name MapDangerCellsName() { return shen_name(SHEN_NAME4('D', 'A', 'P', 'H')); }
inline shen_name MapCommandTempStorageName() { return shen_name(SHEN_NAME4('C', 'T', 'M', 'P')); }
inline shen_name MapDangerTempStorageName() { return shen_name(SHEN_NAME4('D', 'T', 'M', 'P')); }
inline shen_name MapDangerStorageName() { return shen_name(SHEN_NAME4('D', 'A', 'N', 'G')); }
#define MAP_CLIP_STORAGE MapClipStorageName()
#define MAP_WALL_STORAGE MapWallStorageName()
#define MAP_CAMERA_WALL_STORAGE MapCameraWallStorageName()
#define MAP_COMMAND_CELLS MapCommandCellsName()
#define MAP_COMMAND_ROUTING MapCommandRoutingName()
#define MAP_DANGER_CELLS MapDangerCellsName()
#define MAP_COMMAND_TEMP_STORAGE MapCommandTempStorageName()
#define MAP_DANGER_TEMP_STORAGE MapDangerTempStorageName()
#define MAP_DANGER_STORAGE MapDangerStorageName()
extern float g_mapCellScale;
extern MapModelRuntimeSlot g_mapModelSlots[32];
extern MapFieldDepthSlot g_mapFieldDepthSlots[32];
void ClearMapModelSlot(MapModelRuntimeSlot &slot);
void InitializeMapFieldDepthSlots();
void FreeMapFieldDepthSlot(std::int32_t index);
void FreeMapModelSlot(std::int32_t index);
void SetupModel(std::int32_t index, MT7Model *model);
void RegisterPackageMapModel(std::int32_t index, MT7Model *model);
std::int32_t FindFreeMapModelSlot();
std::int32_t FindMapModelSlot(const MT7Model *model);
MT7Node *GetMapModelRoot(std::int32_t index);
MT7Model *GetMapModelReader(std::int32_t index);
const MapModelClipRecord *GetMapModelClipRecords(std::int32_t index);
void GetMapModelPosition(Vector3 *position, std::int32_t index);
std::uint32_t GetMapModelRotationY(std::int32_t index);
void SetMapModelReader(MT7Model *model, std::int32_t index);
void SetMapModelEnabled(std::uint32_t enabled, std::int32_t index);
void EV_AssignLightParams(std::uint32_t value, std::int32_t index);
void *LoadPackageMapModel(PakGlobalData *package, void *data, std::int32_t visible);
void ReleasePackageMapModel(void *model);
void AllocForCLIP(MT7Node *root, std::int32_t index);
void SetMapCellSize(float size);
float GetMapCellScale();
void CountMapClipRecords(const MT7Node *node);
std::int32_t AddMapCommandCell(std::int16_t x, std::int16_t z, MapCellOffset *cells);
void MoveScreenSpaceCellFirst(std::int32_t count, MapCellOffset *cells);
void FindMapWallEndpoints(const std::byte *vertices, Vector3 &first, Vector3 &second);
void PlaceMapWallEndpoints(const Vector3 &position, std::int32_t yaw, float scale, Vector3 &first, Vector3 &second);
void BuildMapClipRecords(MT7Node *node);
void BuildMapModelCommandCells(std::int32_t index, std::int32_t count, std::int32_t dangers, std::int32_t wallsCount, std::int32_t cameraWallsCount);
void SetMapFieldDepthEnabled(std::uint32_t enabled, std::int32_t index);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMapModelSlotPosition(const Vector3 *position, std::int32_t index);
std::uint32_t SetMapModelSlotRotation(std::int32_t rotationY, std::int32_t index);
std::uint32_t SetMapModelSlotTransform(std::int32_t index, const Vector3 *position, std::int32_t rotationY);
std::uint32_t SetMapModelVertexShaderType(std::int32_t index, std::int32_t value);
void SetDefaultVSByIndex(std::int32_t index);
void SetMapModelDataDefaultVertexShader();
#endif

#undef SHENMUE_MAP_MODEL_CHECK

} } // namespace shenmue::s2

#endif
