#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#include "common/math.hpp"
#endif
#include <cstdint>

struct _adx_fs;

namespace shenmue {
namespace s2 {

struct AudioPartition;
#define MAGIC_ROOM_TASK SHEN_NAME4('R', 'O', 'O', 'M')
#define MAGIC_ROOM_DEBUG_AREA SHEN_NAME4('O', 'P', '0', '2')
#define MAGIC_ROOM_PLAYER_AREA SHEN_NAME4('A', 'R', '0', '1')
static const std::int16_t MAGIC_ROOM_MODEL_VARIANT_FLAG = 1009;
extern AudioPartition *g_magicRoomArchive;
extern _adx_fs *g_magicRoomFile;
extern std::uint8_t g_magicRoomAreaJumpBlocked;
void InitMagicRoomPtrs();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

typedef std::int32_t MagicRoomInteger;
typedef std::uint32_t MagicRoomUnsigned;
typedef std::int8_t MagicRoomLimitValue;
struct MT7Model;
struct MagicRoomHeader {
    std::uint32_t tag, size, version, reserved, groupCount, groupOffset, modelCount, modelOffset;
};
struct MagicRoomGroup {
    std::int32_t count, start;
};
struct MagicRoomModel {
    Vector3 dimensions;
    std::int32_t category;
    std::uint8_t parameters[16];
};
struct MagicRoomPlacement {
    std::int32_t kind;
    Vector3 position;
    std::uint16_t extraRotationX, yaw, extraRotationZ, modelIndex;
    float halfWidth, height, halfDepth;
    MT7Model *modelHandle;
};
struct MagicRoomCursorState {
    Vector3 firstThree;
    float x3, d2, x3Boundary, d3Boundary;
    std::int32_t reserved;
};
struct MagicRoomGenerationState {
    std::uint8_t modes[9];
    Vector3 dimensions;
    MagicRoomLimitValue capacity[45], usage[45], modeSeed;
    float boundaryX;
    MagicRoomCursorState cursor;
};
struct MagicRoomEntityLight {
    std::byte reserved00[8];
    std::uint16_t enabled;
    std::int16_t flags;
    Vector3 position, direction;
    Vector4 colour;
    float sceneIntensity, range;
    std::uint32_t coneAngle;
};
typedef char MagicRoomHeaderCheck[sizeof(MagicRoomHeader) == 0x20 ? 1 : -1];
typedef char MagicRoomModelCheck[sizeof(MagicRoomModel) == 0x20 ? 1 : -1];
typedef char MagicRoomPlacementCheck[sizeof(MagicRoomPlacement) == 0x28 ? 1 : -1];
typedef char MagicRoomPlacementModelCheck[offsetof(MagicRoomPlacement, modelHandle) == 0x24 ? 1 : -1];
typedef char MagicRoomGenerationStateCheck[sizeof(MagicRoomGenerationState) == 0x98 ? 1 : -1];
typedef char MagicRoomGenerationUsageCheck[offsetof(MagicRoomGenerationState, usage) == 0x45 ? 1 : -1];
typedef char MagicRoomGenerationCursorCheck[offsetof(MagicRoomGenerationState, cursor) == 0x78 ? 1 : -1];
typedef char MagicRoomEntityLightCheck[sizeof(MagicRoomEntityLight) == 0x40 ? 1 : -1];
#define TASK_MAGIC_ROOM shen_name(SHEN_NAME(ROOM))
static const std::int8_t magicRoomLowerCornerCategories[6] = {4, 5, 9, 6, 13, 11};
static const std::int8_t magicRoomLowerLeftFallbackCategories[4] = {5, 9, 7, 15};
static const std::int8_t magicRoomUpperCornerCategories[10] = {4, 5, 6, 10, 12, 9, 40, 43, 37, 39};
static const std::int8_t magicRoomSideWallCategories[16] = {4, 5, 6, 7, 10, 9, 11, 15, 18, 31, 32, 30, 35, 43, 25, 37};
static const std::int8_t magicRoomLowerWallCategories[11] = {4, 5, 9, 6, 7, 15, 18, 35, 43, 31, 25};
static const std::int8_t magicRoomInteriorCategories[5] = {11, 43, 41, 38, 18};
static const std::int8_t magicRoomGroupedCategories[1] = {17};
static const std::int8_t magicRoomSecondaryCategories[1] = {8};
static const std::int8_t magicRoomSurfaceDecorationCategories[4] = {16, 21, 22, 20};
static const std::uint32_t magicRoomDefaultLightWords[16] = {
    0, 0, 0x00040001, 0, 0x3fd9999a, 0xc03a3d71, 0, 0x3f800000, 0xbebd70a4, 0x3f800000, 0x3f451eb8, 0x3f800000, 0x3f400000, 0x3f666666, 0x40933333, 0x00003700};
extern MagicRoomHeader *g_magicRoomDatabase;
extern MagicRoomGroup *g_magicRoomGroups;
extern MagicRoomModel *g_magicRoomModels;
MagicRoomUnsigned ReadMagicRoomDatabase(MagicRoomHeader *database);
void InitializeMagicRoomGeneration();
void FillMagicRoomCategorySeeds();
MagicRoomInteger GenerateMagicRoomPlacements(MagicRoomPlacement *placements, std::int32_t capacity, std::int32_t seed);
void SetMagicRoomEntityBuffer(void *buffer, std::int32_t capacity);
MagicRoomUnsigned GetMagicRoomEntityCount();
MagicRoomEntityLight *GetMagicRoomEntityParameters(std::int32_t index);
void AddMagicRoomEntityMatrix(const Matrix4x4 *matrix);

struct MagicRoomDescriptor {
    Vector2 quad[4], center, size;
    std::uint32_t yaw;
    std::int32_t type;
};
struct MagicRoomFloorGroup {
    std::int32_t entryCount, retained04;
    Vector2 heightRange;
    const MagicRoomDescriptor *entries;
};
typedef char MagicRoomNativeLayout0[(sizeof(MagicRoomDescriptor) == 0x38) ? 1 : -1];
typedef char MagicRoomNativeLayout1[(sizeof(MagicRoomFloorGroup) == 0x14) ? 1 : -1];
extern const MagicRoomFloorGroup *g_magicRoomFloors;
extern Vector3 g_magicRoomOrigin, g_magicRoomCollisionDimensions;
extern std::uint32_t g_magicRoomYaw;
extern std::int32_t g_magicRoomSeedBase, g_magicRoomSeed;
std::int32_t SelectRoomDescriptor(Vector3 *position, std::uint32_t *yaw, Vector3 *dimensions);
void SelectMagicRoomAtPosition(const Vector3 *position);
void SetMagicRoomDimensions(float width, float height, float depth);
void GetMagicRoomDimensions(Vector3 *dimensions);
void SelectMagicRoomForPlayer();
void LeaveMagicRoom();
void QueueMagicRoom(const char *directory, const char *archive, const MagicRoomFloorGroup *groups, std::int32_t seed);
#endif
void MagicRoomModelVariant_Callback(void *data);
void RestoreMagicRoomModelVariant();

} // namespace s2
} // namespace shenmue

#endif
