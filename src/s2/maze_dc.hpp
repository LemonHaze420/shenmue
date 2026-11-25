#pragma once

#include "shenmue.h"
#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/hlib.hpp"

namespace shenmue { namespace s2 {
struct Character;
struct MT7Model;
struct MazePropRecord {
    std::uint32_t flags;
    std::int32_t kind;
    std::uint32_t reserved;
    float chance, minX, maxX, height;
    std::uint32_t drawState;
};
struct MazeRoad {
    std::uint32_t flags;
    std::int32_t block, index, unit, yaw;
    Vector3 position, leftJoin, rightJoin;
    float cameraDistance;
    Vector3 rightStart, rightEnd, leftStart, leftEnd;
    Vector3 leftCentre, rightCentre;
};
struct MazeUnit {
    MazeRoad *road;
    std::int32_t numLists;
    Vector3 **pointLists;
    std::int32_t modelIndex;
    std::int16_t pointCount, wallCount;
    Vector3 quad[4], points[3], walls[3][2];
};
struct MazePartition {
    std::int32_t count;
    Vector3 *points;
};
struct MazeMarker {
    std::int32_t id, partition, element, reserved;
};
struct MazeTaskData {
    std::uint32_t reserved;
    HLib::Task *task;
};
typedef char MazeRoadSizeCheck[sizeof(MazeRoad) == 0x84 ? 1 : -1];
typedef char MazeRoadPositionCheck[offsetof(MazeRoad, position) == 0x14 ? 1 : -1];
typedef char MazeRoadLeftCentreCheck[offsetof(MazeRoad, leftCentre) == 0x6c ? 1 : -1];
typedef char MazeRoadRightCentreCheck[offsetof(MazeRoad, rightCentre) == 0x78 ? 1 : -1];
typedef char MazeUnitSizeCheck[sizeof(MazeUnit) == 0xb0 ? 1 : -1];
typedef char MazeUnitPathsCheck[offsetof(MazeUnit, pointLists) == 8 ? 1 : -1];
typedef char MazeUnitQuadCheck[offsetof(MazeUnit, quad) == 0x14 ? 1 : -1];
typedef char MazeUnitWallsCheck[offsetof(MazeUnit, walls) == 0x68 ? 1 : -1];
typedef char MazePartitionSizeCheck[sizeof(MazePartition) == 8 ? 1 : -1];
typedef char MazeMarkerSizeCheck[sizeof(MazeMarker) == 0x10 ? 1 : -1];
typedef char MazeTaskDataSizeCheck[sizeof(MazeTaskData) == 8 ? 1 : -1];
extern MazeUnit *MUZT_Data;
extern std::int32_t g_mazeMode, g_mazeUnitCount, g_mazePartitionCount, g_mazeMarkerCount;
extern MazeRoad *g_mazeRoadBlocks[16];
extern MazePartition g_mazePartitions[32];
extern MazeMarker *g_mazeMarkers;
extern MT7Model *g_mazeRoadModels[24], *g_mazeUnitModels[64], *g_mazeMediumModels[35], *g_mazeTerrainModel;
extern MazePropRecord *g_mazePropTables[6];
extern Matrix4x4 g_mazeViewTransform;
extern MazeRoad *g_currentMazeRoads[120], *g_currentMazeUnits[9];
extern std::int32_t g_mazeRoadDrawEnabled, g_mazeUnitDrawEnabled, g_mazeRoadPropsEnabled, g_mazeCullingSteps;
extern std::int32_t g_mazeWireframeEnabled, g_mazeUnitWallsEnabled, g_mazeModelOverride, g_mazeRouteDrawEnabled;
void InitializeMazeRuntime(std::int32_t act, shen_name area);
void mazeInit();
MazeUnit *GetMazeUnit(std::int32_t index);
MazeRoad *GetMazeRoad(std::int32_t block, std::int32_t index);
void GetMazeRoadPosition(std::int32_t block, std::int32_t index, Vector3 *output);
std::uint32_t GetMazeRoadYaw(std::int32_t block, std::int32_t index);
std::int32_t GetMazeRoadCount(std::int32_t block);
std::uint32_t GetMazeUnitCount();
std::uint32_t GetMazeMarkerCount();
void FindNearestMazeRoad(const Vector3 *position, std::int32_t *block, std::int32_t *index);
std::uint32_t FindNearestMazeFirstBlockRoad(const Vector3 *position);
std::uint32_t FindCharacterNearestMazeFirstBlockRoad(Character *character);
shen_name GetMazeUnitAuthoredName(std::int32_t index);
void GetMazeUnitPosition(std::int32_t index, Vector3 *output);
std::uint32_t GetMazeUnitYaw(std::int32_t index);
std::int32_t GetCharacterMazeUnitIndex(Character *character);
std::int32_t FindMazeUnit(const Vector3 *position);
std::int32_t mazeGetNumPartitions();
std::int32_t GetMZNRDataForID(std::int32_t partition);
void mazeFindClosestTarget(std::int32_t *partition, std::int32_t *element, const Vector3 *point);
void mazeSamplePartition(std::int32_t partition, std::int32_t element, Vector3 *output);
std::int32_t mazeFindPartitionDirection(std::int32_t sourcePartition, std::int32_t sourceElement,
    std::int32_t targetPartition, std::int32_t targetElement, std::int32_t *nextPartition, std::int32_t *nextElement);
bool mazeGetMarkerPosition(std::int32_t marker, Vector3 *output);
void GetMazeMarkerRouteCoordinates(std::int32_t marker, std::int32_t *partition, std::int32_t *element);
std::int32_t FindMazeMarkerWithinRadius(const Vector3 *position, float radius);
std::int32_t FindCharacterMazeMarkerWithinRadius(Character *character, float radius);
bool IsPositionWithinMazeMarkerRadius(const Vector3 *position, std::int32_t marker, float radius);
bool IsCharacterWithinMazeMarkerRadius(Character *character, std::int32_t marker, float radius);
void ResetMazeHourAnimation(std::int32_t frame);
void SetMazeHourAnimationFrame(std::int32_t frame);
void InitializeMazeFishPath();
void InitializeMazeNavigationTrail();
std::int32_t InitializeMazeObjectTransforms(const Vector3 *camera, float *nearestDistance);
struct MT7Node;
void DrawMazeFish(const Vector3 *boxMin, const Vector3 *boxMax, std::int32_t count, std::int32_t seed, const MT7Node *fish);
} } // namespace shenmue::s2
