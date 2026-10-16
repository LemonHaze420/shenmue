#pragma once
#include "shenmue.h"

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {
namespace HLib {
struct Task;
}
struct Character;
struct MT7Model;

extern std::uint32_t g_mazePropDrawCount;
extern std::uint32_t g_mazeUnitWallHitCount;

inline constexpr shen_name TASK_MAZE{SHEN_NAME(MAZE)};
inline constexpr shen_name TASK_FISH{SHEN_NAME(FISH)};
inline constexpr shen_name MEMORY_MAZE_FISH{SHEN_NAME(MZFS)};
inline constexpr shen_name MEMORY_MAZE_UNITS{SHEN_NAME(MUZT)};
inline constexpr shen_name MEMORY_MAZE_ROUTES{SHEN_NAME(MZRT)};
inline constexpr std::size_t MAZE_UNIT_CAPACITY = 64;
inline constexpr shen_name AREA_MAZE_FISH{SHEN_NAME(KWM1)};
inline constexpr Vector2 mazeFishControlPath[] = {
    {-1.91311002f, -0.219183996f}, {-1.92379701f, -0.572017014f}, {-1.91311002f, -0.924848974f},
    {-1.84898305f, -1.341833f}, {-1.560413f, -1.68397403f}, {-0.983273983f, -1.85504401f},
    {-0.491636992f, -1.75881696f}, {-0.106877998f, -1.52359605f}, {0.224443004f, -0.989000976f},
    {0.427511007f, -0.219183996f}, {0.427511007f, 0.657552004f}, {-0.480949014f, 0.850005984f},
    {-0.972585976f, 0.882081985f}, {-1.06877601f, 1.48082805f}, {-0.598514974f, 1.79089296f},
    {0.513013005f, 1.87642801f}, {1.76348102f, 1.61982298f}, {1.82760799f, 0.539941013f},
    {1.02602506f, 0.0481140018f}, {1.710042f, -0.775162995f}, {1.69935405f, -1.63051498f},
    {1.16496599f, -1.85504401f}, {0.491636992f, -1.92988801f}, {-0.171003997f, -1.64120698f},
    {-0.395446986f, -1.13868701f}, {-0.470261991f, -0.572017014f}, {-0.887084007f, 0.197799996f},
    {-1.11152697f, 0.700318992f}, {-0.662640989f, 1.24560595f}, {-1.12221503f, 1.73743403f},
    {-1.77416897f, 1.416677f}, {-1.87035799f, 0.850005984f}, {-1.92379701f, 0.539941013f},
    {-1.92379701f, 0.176415995f}, {-1.92379701f, -0.197799996f}, {-1.91311002f, -0.219183996f},
    {-1.92379701f, -0.572017014f},
    {1000.0f, 1000.0f},
};
static_assert(sizeof(Vector2) == 8);
static_assert(sizeof(Vector3) == 12);
static_assert(sizeof(mazeFishControlPath) == 304);

inline constexpr Vector3 mazeButterflyPath[] = {
    {-0.02f, 0.02f, 0.01f}, {-0.04f, 0.05f, 0.02f}, {-0.05f, 0.07f, 0.0f}, {-0.03f, 0.08f, -0.01f},
    {0.03f, 0.08f, 0.01f}, {0.05f, 0.07f, 0.0f}, {0.04f, 0.05f, -0.02f}, {0.02f, 0.02f, -0.01f},
};

struct MazePropRecord {
    std::uint32_t flags;
    std::int32_t kind;
    std::uint32_t reserved;
    float chance;
    float minX;
    float maxX;
    float height;
    std::uint32_t drawState;
};
static_assert(sizeof(MazePropRecord) == 32);
struct MazePropState {
    std::int32_t kind{-1};
    std::uint32_t flags{};
    std::uint32_t drawState{};
    std::int32_t yaw{};
    float x{};
    float z{};
    float height{};
    std::uint32_t reserved{};
    const void *placement{};
};
#if SHENMUE_IS_PC
static_assert(sizeof(MazePropState) == 40);
static_assert(offsetof(MazePropState, yaw) == 12);
static_assert(offsetof(MazePropState, reserved) == 28);
static_assert(offsetof(MazePropState, placement) == 32);
#endif

#pragma pack(push, 4)
struct MazeRoad {
    std::uint32_t flags;
    std::int32_t block;
    std::int32_t index;
    std::int32_t unit;
    std::int32_t yaw;
    Vector3 position;
    Vector3 leftJoin;
    Vector3 rightJoin;
    float cameraDistance;
    Vector3 rightStart;
    Vector3 rightEnd;
    Vector3 leftStart;
    Vector3 leftEnd;
    Vector3 leftCentre;
    Vector3 rightCentre;
    float alpha;
    std::int32_t alphaDirection;
};
#pragma pack(pop)

struct MazeUnit {
    MazeRoad *road;
    std::int32_t numLists;
    std::int32_t reserved0c;
    Vector3 **pointLists;
    std::int32_t modelIndex;
    std::int16_t pointCount;
    std::int16_t wallCount;
    Vector3 quad[4];
    Vector3 points[3];
    Vector3 walls[3][2];
    std::uint32_t reservedbc;
};

extern MazeUnit *MUZT_Data;
static_assert(sizeof(MazeUnit) * MAZE_UNIT_CAPACITY == 0x3000);

struct MazePartition {
    std::int32_t count;
    std::int32_t reserved04;
    Vector3 *points;
};
struct AuthoredPath {
    std::span<const Vector2> points;
};
struct AuthoredUnit {
    const char *model;
    std::span<const AuthoredPath> paths;
    std::int32_t areaIndex;
};
struct RoadBlock {
    std::span<const std::byte> authored;
    Vector3 *controlPoints{};
    MazeRoad *roads{};
    std::int32_t controlCount{};
    std::int32_t roadCount{};
};
struct EndpointNeighbor {
    std::int16_t partition{-1};
    std::int16_t element{-1};
};
struct EndpointLinks {
    EndpointNeighbor neighbors[2];
    std::uint16_t visited{};
};
static_assert(sizeof(EndpointLinks) == 10);
struct Marker {
    std::int32_t id;
    std::int32_t partition;
    std::int32_t element;
    std::int32_t reserved;
};

struct MazeTaskData {
    std::uint32_t reserved;
#if SHENMUE_IS_PC
    std::uint32_t padding;
#endif
    HLib::Task *task;
};
#if SHENMUE_IS_PC
static_assert(sizeof(MazeTaskData) == 0x10);
static_assert(offsetof(MazeTaskData, task) == 0x08);
#else
static_assert(sizeof(MazeTaskData) == 0x08);
static_assert(offsetof(MazeTaskData, task) == 0x04);
#endif

static_assert(sizeof(MazeRoad) == 0x8c);
static_assert(offsetof(MazeRoad, position) == 0x14);
static_assert(offsetof(MazeRoad, alpha) == 0x84);
static_assert(offsetof(MazeRoad, cameraDistance) == 0x38);
static_assert(sizeof(MazeUnit) == 0xc0);
static_assert(offsetof(MazeUnit, pointLists) == 0x10);
static_assert(offsetof(MazeUnit, quad) == 0x20);
static_assert(offsetof(MazeUnit, walls) == 0x74);
static_assert(sizeof(MazePartition) == 0x10);

bool InitializeMazeRuntime(std::int32_t act, shen_name area);
void RetireMazeRuntime();
std::span<MazeRoad *const> GetCurrentMazeObjects();
std::span<MazeRoad *const> GetCurrentMazeUnitObjects();
std::int32_t InitializeMazeObjectTransforms(const Vector3 *camera, float *nearestDistance);
std::span<const MazeUnit> GetMazeUnits();
std::span<const MazeRoad> GetMazeRoads(std::int32_t block);
MazeRoad *GetMazeRoad(std::int32_t block, std::int32_t index);
std::uint32_t GetMazeRoadPosition(std::int32_t block, std::int32_t index, Vector3 *output);
std::uint32_t GetMazeRoadYaw(std::int32_t block, std::int32_t index);
std::uint32_t GetMazeUnitCount();
std::uint32_t GetMazeMarkerCount();
void FindNearestMazeRoad(const Vector3 *position, std::int32_t *block, std::int32_t *index);
std::uint32_t FindNearestMazeFirstBlockRoad(const Vector3 *position);
std::uint32_t FindCharacterNearestMazeFirstBlockRoad(Character *character);
shen_name GetMazeUnitAuthoredName(std::int32_t index);
bool HasMazeUnitOfKind(shen_name name);
void SetMazeDefaults();
void SetMazeUnitDrawDistance(float distance);
void SetMazeTerrainDrawEnabled(std::int32_t enabled);
std::int32_t mazeGetNumPartitions();
std::int32_t GetMZNRDataForID(std::int32_t partition);
void mazeFindClosestTarget(std::int32_t *partition, std::int32_t *element, const Vector3 *point);
void mazeSamplePartition(std::int32_t partition, std::int32_t element, Vector3 *output);
std::int32_t mazeFindPartitionDirection(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *nextElement);
bool mazeGetMarkerPosition(std::int32_t marker, Vector3 *output);
void GetMazeMarkerRouteCoordinates(std::int32_t marker, std::int32_t *partition, std::int32_t *element);
std::int32_t FindMazeMarkerWithinRadius(const Vector3 *position, float radius);
std::int32_t FindCharacterMazeMarkerWithinRadius(Character *character, float radius);
bool IsCharacterWithinMazeMarkerRadius(Character *character, std::int32_t marker, float radius);
void GetMazeUnitPosition(std::int32_t index, Vector3 *output);
std::uint32_t GetMazeUnitYaw(std::int32_t index);
std::uint64_t GetCharacterMazeUnitIndex(Character *character);
std::int64_t InitializeMazeNavigationTrail();
void ResetMazeHourAnimation(std::int32_t frame);
void SetMazeHourAnimationFrame(std::int32_t frame);

std::uint32_t mazeGetCurrentRefCount();
void mazeSetCurrentRefCount(std::uint32_t count);
HLib::Task *MAZ_fishCtrl();
void MAZ_fishCtrl_Callback(void *taskData);
void MAZ_fishCtrlUpdate(const Vector3 *boxMin, const Vector3 *boxMax, std::uint32_t count, std::int32_t seed, const MT7Model *fish);

void SetFish(const Vector3 *boxMin, const Vector3 *boxMax, std::int32_t count, std::int32_t seed, Character *fish);

} // namespace shenmue::s2
