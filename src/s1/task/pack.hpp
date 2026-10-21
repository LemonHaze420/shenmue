#pragma once

#include "s1/d3t/namespace.hpp"
#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr shen_name PACK_FILE_BIN = make_name('B', 'I', 'N', ' ');
struct PackSlot;
struct PackOwner;
struct PackModel;
struct PackAdfsWrapper;
struct Character;

enum PackSlotFlags : std::uint32_t {
    PackSlotAsyncCachedRead = 0x1,
    PackSlotHasChrt = 0x2,
    PackSlotPaksPhase = 0x4,
    PackSlotGzip = 0x8,
    PackSlotSpecialAllocation = 0x10,
};

enum PackDataFlags : std::uint32_t {
    PackDataSkipPksFollowup = 0x2,
    PackDataLoadPksDirect = 0x4,
};

#pragma pack(push, 1)
struct PackTaskData {
    std::uint32_t tag;
    std::uint32_t chidParseGuard;
    std::uint32_t usePackCache;
    char rootDirectory[0x24];
    void *chrtData;
    std::uint32_t chrtSize;
    std::uint32_t field_3c;
    PackSlot *readyPack;
    PackSlot *loadingSlot;
    PackSlot *pendingPakf;
    PackSlot *activeReader;
    std::uint32_t renderHandleOrReadMode;
    std::uint32_t flags;
    std::int32_t callbackBudget;
    std::int32_t adfsRetryCount;
};

struct PackOwner {
    PackAdfsWrapper *adfsWrapper;
    char directory[0x20];
    char filename[0x10];
    std::int32_t referenceCount;
    std::byte reserved_3c[4];
};

struct PackAdfsWrapper {
    std::int32_t status;
    std::byte reserved_04[4];
    HLib::TaskData *taskData;
};

extern std::array<PackAdfsWrapper, 12> g_adfsWrappers;

struct PackOpenFile {
    char path[0x104];
    std::uint32_t fileId;
    std::int64_t field_108;
};

struct PakFileHeader {
    std::uint32_t magic;
    std::uint32_t directoryOffset;
    std::uint32_t field_08;
    std::uint32_t count;
};

struct PakChunkHeader {
    std::uint32_t tag;
    std::uint32_t size;
};

struct PakDirectory {
    std::uint32_t tag;
    std::uint32_t size;
    std::int32_t entryCount;
    std::uint32_t entriesOffset;
};

struct PakDirectoryEntry {
    char name[8];
    std::uint32_t tag;
    std::uint32_t dataOffset;
    std::uint32_t auxiliary;
};

struct PackModel {
    PakChunkHeader *pakf;
    void *model;
    std::uint32_t textureCount;
    std::int32_t field_14;
};

struct PackSlot {
    std::uint32_t tag;
    std::uint32_t field_04;
    PakFileHeader *pakData;
    std::uint32_t pakSize;
    char name[28];
    PackOwner *ownerOrHumans;
    std::int32_t *textureIds;
    PakChunkHeader *textureIterator;
    void *collisionData;
    std::int32_t *motionHeaders;
    void *dynamicMapData;
    PackModel **pakfResults;
    std::int32_t *mapIndices;
    void *asyncFile;
    std::int16_t pkfInnerIndex;
    std::int16_t pksInnerIndex;
    std::int16_t chunkIndex;
    std::int16_t textureIndex;
    std::int16_t motionIndex;
    std::int16_t pakfResultIndex;
    std::uint32_t flags;
};
#pragma pack(pop)

static_assert(sizeof(PackTaskData) == 0x70);
static_assert(offsetof(PackTaskData, loadingSlot) == 0x48);
static_assert(offsetof(PackTaskData, activeReader) == 0x58);
static_assert(offsetof(PackTaskData, callbackBudget) == 0x68);
static_assert(sizeof(PackOwner) == 0x40);
static_assert(offsetof(PackOwner, referenceCount) == 0x38);
static_assert(sizeof(PackSlot) == 0x88);
static_assert(sizeof(PackAdfsWrapper) == 0x10);
static_assert(sizeof(PackOpenFile) == 0x110);
static_assert(offsetof(PackOpenFile, fileId) == 0x104);
static_assert(sizeof(PakFileHeader) == 0x10);
static_assert(sizeof(PakChunkHeader) == 8);
static_assert(sizeof(PakDirectory) == 0x10);
static_assert(sizeof(PakDirectoryEntry) == 0x14);
static_assert(offsetof(PakDirectoryEntry, tag) == 8);
static_assert(sizeof(PackModel) == 0x18);
static_assert(offsetof(PackModel, model) == 8);
static_assert(offsetof(PackOwner, directory) == 8);
static_assert(offsetof(PackOwner, filename) == 0x28);
static_assert(offsetof(PackSlot, ownerOrHumans) == 0x30);
static_assert(offsetof(PackSlot, asyncFile) == 0x70);
static_assert(offsetof(PackSlot, flags) == 0x84);

extern PackTaskData *g_packTaskData;
extern HLib::Task *g_packTask;
extern PackOwner g_packOwners[8];
extern PackSlot g_packSlots[32];
extern std::uint64_t g_currentPak[3];
extern char *g_globalEntityName;


extern bool g_packBudgetIgnored;

void PACK_Init();
void PACK_Callback(HLib::TaskData *taskData);
void PACK_Drain();
void PACK_Shutdown();
std::int64_t PACK_HasValidPack();
PackSlot *PACK_FindSlotByName(const char *name);
void PACK_SetReadyPack(PackSlot *slot);
std::int64_t PACK_MarkFree();
bool PACK_IsValidSlot(PackSlot *slot);
void PACK_LoadFile(PackSlot *slot);
PackOwner *PACK_ReadAFS(const char *rootDirectory, const char *filename);
PackSlot *PACK_FindAfsEntrySlot(PackOwner *owner, std::int32_t pkfEntryIndex, std::int32_t pksEntryIndex);
PackSlot *PACK_LoadHumanByIndex(PackOwner *owner, std::uint32_t pkfEntryIndex, std::uint32_t pksEntryIndex);
std::int64_t PACK_FreeOwnerADFSData(void *owner);
PackSlot *PACK_QueueFile(const char *rootDirectory, const char *packageName);
void FreePVR(void *modelPointer);
void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);
PackSlot *PACK_QueueAFSPair(
    const char *rootDirectory, const char *afsFilename, std::int32_t pkfEntryIndex, std::int32_t pksEntryIndex, const char *packageName);
PackModel *PACK_FindCharacterModel(PackSlot *slot, const char *filename);
PackSlot *PACK_FindModelOwner(PackModel *model);
void *PACK_FindFileData(PackSlot *slot, const char *filename, std::uint32_t tag);
void *LoadFileIntoHeapMem(PackSlot *slot, const char *filename, std::uint32_t tag);
std::uint32_t PACK_FindBinFileSize(PackSlot *slot, const char *filename);
std::byte *MT5_ModelBase();
std::byte *MT5_SetModelBaseOverride(std::byte *modelBase);
std::byte *MT5_ModelBaseForNode(const void *node);
void *MT5_SceneBindingForNode(const void *node, std::byte **modelBase);
void HMDL_NormalizeCommandHeader(void *binding, std::byte *modelBase);
void MT5_ResetModelOwnershipRegistry();
bool MT5_IsRegisteredModelNode(const void *node);
void MT5_UnregisterModelAllocation(const void *payload);
bool HMDL_NodeOffsetsUseTaskData(const void *node);
void HMDL_SetTemporaryTaskRelativeOffsets(const void *node, bool enabled);
void *HMDL_CycleDataForNode(const void *node);
bool HMDL_SetCycleDataForNode(void *node, void *cycleData);
const Matrix4x4 *HMDL_TransformForNode(const void *node, std::int32_t transformOffset);
std::int32_t HMDL_TransformOffsetForNode(const void *node, const Matrix4x4 *transform);
void HMDL_ClearPreparationFlagRecursive(void *node);
void *hmdlAssignFor(PackModel *sourceModel, std::int32_t childIndex = 0, std::int32_t nodeKind = -1, std::int32_t occurrence = 0);
void HMDL_FreeAssignedModel(void *model);
void *HMDL_CloneNodeTree(const void *sourceNode);
bool MT5_LoadRawModel(const char *rootDirectory, const char *filename, const std::int32_t *textureIds);
bool MT5_LoadRawModelPlaced(const char *rootDirectory, const char *filename, const std::int32_t *textureIds, float x, float y, float z, std::int16_t yaw);
bool MT5_LoadCharacterModel(Character *character, const char *rootDirectory, const char *filename, const std::int32_t *textureIds);
PackModel *MT5_LoadPackModel(const char *rootDirectory, const char *filename);
PackModel *MT5_InitRawModelAndTextures(void *fileData, const char *entityName, const char *remapContext);
namespace d3t {
PackModel *ModelLoad_FBX_Replace(const char *rootDirectory, const char *filename);
} // namespace d3t
void MT5_PrepareStaticCharacterModel(void *model);
void LoadAlternateMapModels(const char *rootDirectory, const std::array<std::int32_t, 32> &mapIds);
void LoadMapModels(std::int32_t totalMapPacks, const char *rootDirectory);
void ReadMAPR(const char *rootDirectory, const char *filename, std::int32_t mapIndex);
void PACK_EnsureMapPreparationStorage(std::int32_t mapIndex);
void PACK_EnsureAllMapPreparationStorage();
void *PACK_GetReadyCollisionData();
void FreeClip_PREP(std::int32_t mapIndex);
const std::byte *PACK_GetCurrentDYNAChunk();

} // namespace shenmue::s1
