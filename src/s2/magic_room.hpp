#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct MT7Model;
inline constexpr shen_name TASK_MAGIC_ROOM{SHEN_NAME(ROOM)};
enum class MagicRoomPlacementKind : std::int32_t { Default = 0, Light = 1, Rotating = 2, Clock = 3 };

struct MagicRoomHeader {
    std::uint32_t tag;
    std::uint32_t size;
    std::uint32_t version;
    std::uint32_t reserved;
    std::uint32_t groupCount;
    std::uint32_t groupOffset;
    std::uint32_t modelCount;
    std::uint32_t modelOffset;
};
struct MagicRoomGroup {
    std::int32_t count;
    std::int32_t start;
};
struct MagicRoomModel {
    Vector3 dimensions;
    std::int32_t category;
    std::uint8_t parameters[16];
};
struct MagicRoomPlacement {
    std::int32_t kind;
    Vector3 position;
    std::uint16_t extraRotationX;
    std::uint16_t yaw;
    std::uint16_t extraRotationZ;
    std::uint16_t modelIndex;
    float halfWidth;
    float height;
    float halfDepth;
    std::uint32_t reserved24;
    MT7Model *modelHandle;
    float fadeAlpha;
    std::uint32_t reserved34;
};
struct MagicRoomCursorState {
    Vector3 firstThree;
    float x3;
    float d2;
    float x3Boundary;
    float d3Boundary;
    std::int32_t reserved;
};
static_assert(sizeof(MagicRoomHeader) == 0x20 && offsetof(MagicRoomHeader, groupOffset) == 0x14);
static_assert(sizeof(MagicRoomGroup) == 0x08 && offsetof(MagicRoomGroup, start) == 0x04);
static_assert(sizeof(MagicRoomModel) == 0x20 && offsetof(MagicRoomModel, category) == 0x0c);
static_assert(sizeof(MagicRoomPlacement) == 0x38 && offsetof(MagicRoomPlacement, modelHandle) == 0x28);
static_assert(offsetof(MagicRoomPlacement, position) == 0x04 && offsetof(MagicRoomPlacement, yaw) == 0x12);
static_assert(offsetof(MagicRoomPlacement, modelIndex) == 0x16 && offsetof(MagicRoomPlacement, fadeAlpha) == 0x30);
static_assert(sizeof(MagicRoomCursorState) == 0x20 && offsetof(MagicRoomCursorState, d2) == 0x10);

struct MagicRoomDescriptor {
    Vector2 quad[4];
    Vector2 center;
    Vector2 size;
    std::uint16_t yaw;
    std::uint16_t reserved32;
    std::int32_t type;
};
struct MagicRoomFloorGroup {
    std::int32_t entryCount;
    std::int32_t reserved04;
    Vector2 heightRange;
    const MagicRoomDescriptor *entries;
};
static_assert(sizeof(MagicRoomDescriptor) == 0x38);
static_assert(offsetof(MagicRoomDescriptor, center) == 0x20);
static_assert(offsetof(MagicRoomDescriptor, size) == 0x28);
static_assert(offsetof(MagicRoomDescriptor, yaw) == 0x30);
static_assert(offsetof(MagicRoomDescriptor, type) == 0x34);
static_assert(sizeof(MagicRoomFloorGroup) == 0x18);
static_assert(offsetof(MagicRoomFloorGroup, heightRange) == 0x08);
static_assert(offsetof(MagicRoomFloorGroup, entries) == 0x10);

struct MagicRoomEntityLight {
    std::byte reserved00[8];
    std::uint16_t enabled;
    std::int16_t flags;
    Vector3 position;
    Vector3 direction;
    Vector4 colour;
    float sceneIntensity;
    float range;
    std::uint32_t coneAngle;
};
static_assert(sizeof(MagicRoomEntityLight) == 0x40 && offsetof(MagicRoomEntityLight, position) == 0x0c);
static_assert(offsetof(MagicRoomEntityLight, direction) == 0x18 && offsetof(MagicRoomEntityLight, colour) == 0x24);
static_assert(offsetof(MagicRoomEntityLight, sceneIntensity) == 0x34 && offsetof(MagicRoomEntityLight, range) == 0x38);
inline constexpr std::uint32_t magicRoomDefaultLightWords[16]{
    0, 0, 0x00040001, 0, 0x3fd9999a, 0xc03a3d71, 0, 0x3f800000,
    0xbebd70a4, 0x3f800000, 0x3f451eb8, 0x3f800000, 0x3f400000, 0x3f666666, 0x40933333, 0x00003700
};

inline constexpr std::int8_t magicRoomFurnitureCategories[64]{
    4, 5, 9, 6, 13, 11, 17, 8, 5, 9, 7, 15, 16, 21, 22, 20,
    4, 5, 6, 10, 12, 9, 40, 43, 37, 39, 0, 0, 0, 0, 0, 0,
    4, 5, 6, 7, 10, 9, 11, 15, 18, 31, 32, 30, 35, 43, 25, 37,
    4, 5, 9, 6, 7, 15, 18, 35, 43, 31, 25, 0, 11, 43, 41, 38
};
extern MagicRoomHeader *g_magicRoomDatabase;
extern MagicRoomGroup *g_magicRoomGroups;
extern MagicRoomModel *g_magicRoomModels;

std::uint64_t ReadMagicRoomDatabase(MagicRoomHeader *database);
void InitializeMagicRoomGeneration();
std::uint64_t FillMagicRoomCategorySeeds();
void SetMagicRoomDimensions(float width, float height, float depth);
void GetMagicRoomDimensions(Vector3 *dimensions);
std::uint64_t GenerateMagicRoomPlacements(MagicRoomPlacement *placements, std::int32_t capacity, std::int32_t seed);
void SetMagicRoomEntityBuffer(void *buffer, std::int32_t capacity);
std::uint64_t GetMagicRoomEntityCount();
MagicRoomEntityLight *GetMagicRoomEntityParameters(std::int32_t index);
void AddMagicRoomEntityMatrix(const Matrix4x4 *matrix);

void InitializeMagicRoom();
void ReleaseMagicRoomRuntime();
void SelectMagicRoomForPlayer();
void SelectMagicRoomAtPosition(const Vector3 *position);
void LeaveMagicRoom();
std::uint64_t GetMagicRoomActiveFlag();
bool IsMagicRoomLoadingModels();
bool IsMagicRoomReady();
void SetMagicRoomActiveFlag(std::int32_t enabled);
void QueueMagicRoom(const char *directory, const char *archive, const MagicRoomFloorGroup *groups, std::int32_t seed);

} // namespace shenmue::s2
