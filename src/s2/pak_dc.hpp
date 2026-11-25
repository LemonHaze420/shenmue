#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hlib.hpp"
#include "common/types.hpp"
#include "platform/dc/disc.hpp"
#include "platform/dc/package.hpp"
struct _adx_fs;
#define SHENMUE_PAK_LAYOUT_JOIN_(a, b) a##b
#define SHENMUE_PAK_LAYOUT_JOIN(a, b) SHENMUE_PAK_LAYOUT_JOIN_(a, b)
#define SHENMUE_PAK_LAYOUT(condition) typedef char SHENMUE_PAK_LAYOUT_JOIN(PakLayout_, __LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {
struct AudioPartition;
struct PakAfsPartition {
    AudioPartition *afs;
    char folder[32];
    char filename[16];
    std::int32_t references;
};
struct PakGlobalData {
    shen_name name;
    void *data;
    std::uint32_t dataSize;
    char entryType[24];
    PakAfsPartition *partition;
    std::int32_t *textureHandles;
    const std::byte *textureCursor;
    const std::byte *soundTable;
    std::uint32_t *motionHandles;
    std::uint32_t *propHandles;
    std::uint32_t motionProperty;
    const void *dynamicMapData;
    void **modelHandles;
    std::int32_t *mapHandles;
    void *destination;
    union {
        FileState *fileLoad;
        _adx_fs *afsFile;
    };
    std::int16_t firstFile;
    std::int16_t secondFile;
    std::int16_t chunkIndex;
    std::int16_t textureIndex;
    std::int16_t motionIndex;
    std::int16_t modelIndex;
    std::int16_t propIndex;
    std::int16_t reserved62;
    std::uint32_t flags;
    std::int32_t state;
};
struct PakLoadTaskData {
    shen_name name;
    std::int32_t unknown04;
    std::uint32_t flags;
    char path[32];
    void *data;
    std::uint32_t dataSize;
    PakGlobalData *linkedPakEntry;
    PakGlobalData *pakBuffer;
    PakGlobalData *pakt;
    PakGlobalData *next;
    PakGlobalData *selectedPackage;
    std::uint32_t loadFlags;
    std::int32_t timeBudget;
    std::int32_t retryCount;
};
struct PakFileHeader {
    shen_name name;
    std::uint32_t directoryOffset;
    std::uint32_t reserved08;
    std::int32_t resourceCount;
};
struct PakChunkHeader {
    shen_name name;
    std::uint32_t size;
};
SHENMUE_PAK_LAYOUT(sizeof(PakFileHeader) == 0x10);
SHENMUE_PAK_LAYOUT(sizeof(PakChunkHeader) == 8);
SHENMUE_PAK_LAYOUT(sizeof(PakAfsPartition) == 0x38);
SHENMUE_PAK_LAYOUT(offsetof(PakAfsPartition, folder) == 4);
SHENMUE_PAK_LAYOUT(offsetof(PakAfsPartition, filename) == 0x24);
SHENMUE_PAK_LAYOUT(offsetof(PakAfsPartition, references) == 0x34);
SHENMUE_PAK_LAYOUT(sizeof(PakGlobalData) == 0x6c);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, entryType) == 0x0c);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, partition) == 0x24);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, textureHandles) == 0x28);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, motionProperty) == 0x3c);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, destination) == 0x4c);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, fileLoad) == 0x50);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, firstFile) == 0x54);
SHENMUE_PAK_LAYOUT(offsetof(PakGlobalData, flags) == 0x64);
SHENMUE_PAK_LAYOUT(sizeof(PakLoadTaskData) == 0x54);
SHENMUE_PAK_LAYOUT(offsetof(PakLoadTaskData, data) == 0x2c);
SHENMUE_PAK_LAYOUT(offsetof(PakLoadTaskData, linkedPakEntry) == 0x34);
SHENMUE_PAK_LAYOUT(offsetof(PakLoadTaskData, pakBuffer) == 0x38);
SHENMUE_PAK_LAYOUT(offsetof(PakLoadTaskData, next) == 0x40);
SHENMUE_PAK_LAYOUT(offsetof(PakLoadTaskData, loadFlags) == 0x48);
SHENMUE_PAK_LAYOUT(offsetof(PakLoadTaskData, timeBudget) == 0x4c);
using ::shenmue::shen_name;
#define PACKAGE_TASK shen_name(SHEN_NAME4('P', 'A', 'C', 'K'))
#define PACKAGE_BINARY_EXTENSION shen_name(SHEN_NAME4('B', 'I', 'N', ' '))
#define PACKAGE_ANY_EXTENSION shen_name(SHEN_NAME4('*', '*', '*', '*'))
#define PACKAGE_PKF_HEADER shen_name(SHEN_NAME4('P', 'A', 'K', 'F'))
#define PACKAGE_PKS_HEADER shen_name(SHEN_NAME4('P', 'A', 'K', 'S'))
#define PACKAGE_TEXTURE_HANDLES shen_name(SHEN_NAME4('P', 'A', 'K', 'T'))
#define PACKAGE_MODEL_HANDLES shen_name(SHEN_NAME4('P', 'A', 'K', 'M'))
#define PACKAGE_PROP_HANDLES shen_name(SHEN_NAME4('P', 'A', 'K', 'P'))
#define PACKAGE_CHARACTER_DATA shen_name(SHEN_NAME4('P', 'A', 'K', 'C'))
static const std::size_t kPakSlotCount = 52;
#define SHENMUE_PAK_ENUM_TYPE(type)
std::int32_t FindPakEntryByType(const PakDirectory *directory, shen_name type);
const char *GetPakEntryName(const PakDirectoryEntry *entry);
enum PakGlobalFlag SHENMUE_PAK_ENUM_TYPE(std::uint32_t) {
    PakAfsRead = 1,
    PakCharactersLoaded = 2,
    PakPksPhase = 4,
    PakCompressed = 8,
    PakKeepRetained = 0x10,
    PakAlternateHeap = 0x20
};
extern HLib::Task *pakLoadTask;
extern PakLoadTaskData *pakLoadTaskData;
extern PakGlobalData pakSlots[kPakSlotCount];
extern PakAfsPartition pakPartitions[4];
extern std::uint32_t pakReadSize;
extern char pakReadName[24];
extern const void *pakReadData;
extern std::int32_t pakReadHeaderCount, pakReadAllocationSize, pakReadBytes;
void PakLoadTaskEntry_Enqueue(std::int8_t index, std::int32_t useSceneTextHeap);
void PakLoadCallback(void *parameter);
std::int32_t LoadPakTextures(PakLoadTaskData *task, PakGlobalData *slot);
std::int32_t LoadPakResources(PakLoadTaskData *task, PakGlobalData *slot);
void LoadPackageCharacters(void *data, shen_name allocationName);
extern std::int16_t g_mstsValue0, g_mstsValue1, g_mstsValue2;
void MSTS_Reader(std::int16_t value0, std::int16_t value1, std::int16_t value2);
void ResetPakSlot(PakGlobalData *slot);
void ResetPakAfsPartition(PakAfsPartition *partition);
std::int32_t GetPakSlotIndex(const PakGlobalData *slot);
PakGlobalData *AllocatePakSlot(const char *name, PakAfsPartition *partition, std::int32_t firstFile, std::int32_t secondFile, std::uintptr_t allocation);
void *FileLoader(PakGlobalData *package, const char *filename, shen_name extension);
std::uint32_t GetPakFileSize(PakGlobalData *package, const char *filename, shen_name extension);
struct MT7Model;
MT7Model *LoadGameModel(PakGlobalData *package, const char *filename);
PakGlobalData *FindRetainingModelPackage(const MT7Model *model);
PakGlobalData *GetNextReadyModelPackage(PakGlobalData *previous);
PakGlobalData *GetPakLoadTaskDataPointer();
PakGlobalData *pakLoadGetNext();
PakGlobalData *GetPakLoadBuffer();
std::int32_t GetPakLoadParams();
std::uint32_t GetPakLoadFlags();
void SetPakLoadFlags(std::uint32_t flags);
void pakLoadFlush();
PakGlobalData *LoadMapPackage(const char *directory, const char *name, std::uintptr_t allocation = 0);
PakGlobalData *ReadPakFileFromDirectory(const char *directory, const char *name, std::int32_t flags);
void FreePakLoadTask();
std::int32_t UnmountPakAfsPartition(PakAfsPartition *partition);
std::int32_t IsPakSlotIdle(const PakGlobalData *slot);
const std::byte *GetIdlePakSoundTable(PakGlobalData *slot);
std::int32_t FreePakSlot(PakGlobalData *slot);
void FreePakSlotStorage(PakGlobalData *slot);
void ReleasePakTextures(PakGlobalData *slot);
void ReleasePakResources(PakGlobalData *slot);
void ClosePakRead(PakGlobalData *slot, std::int32_t releaseBuffer);
void OpenPakAfsRead(PakLoadTaskData *task, PakGlobalData *slot, std::int32_t fileIndex);
const char *BuildPakFilename(const char *name, const char *extension);
void OpenPakLooseRead(PakLoadTaskData *task, PakGlobalData *slot, const char *directory, const char *filename);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace PakCategory {
enum Value SHENMUE_PAK_ENUM_TYPE(std::int32_t) { Package = 0, Partition = 1, Task = 2, Reserved = 3, LoadPackageWithMode = 4, LoadAfsWithMode = 5 };
}
namespace PakPackageOp {
enum Value SHENMUE_PAK_ENUM_TYPE(std::uint32_t) {
    Load = 0,
    IsLoadPending = 2,
    IsLoaded = 5,
    Free = 6,
    FinishLoading = 7,
    LoadAfs = 8,
    GetMapHandles = 9,
    SetLoadBudget = 10,
    GetLoadBudget = 11,
    FreeAfs = 12,
    DestroyAfs = 13,
};
}
namespace PakPartitionOp {
enum Value SHENMUE_PAK_ENUM_TYPE(std::uint32_t) { Mount = 0, Unmount = 1 };
}
namespace PakTaskOp {
enum Value SHENMUE_PAK_ENUM_TYPE(std::uint32_t) { GetFlags = 0, SetFlags = 1, LoadNamed = 2, IsCurrentNamed = 3, SetCurrentNamed = 4 };
}

PakGlobalData *GetSelectedPakPackage();
std::uint32_t *FindPakEventCamera(PakGlobalData *package, std::int32_t index);
std::uint32_t *pakFindEventCam(std::int32_t index);
const std::byte *FindPackageDynamicMap(const std::uint8_t name[4]);
std::int32_t ComparePakNames(const char *left, const char *right);
std::int32_t ComparePakNamesN(const char *left, const char *right, std::int32_t length);
void BuildAfsPakName(const char *filename, std::int32_t firstIndex, std::int32_t secondIndex, char *output);
PakGlobalData *FindPakByEntryName(const char *name);
PakGlobalData *FindPakInAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex);
PakAfsPartition *MountPakAfs(const char *directory, const char *filename);
PakGlobalData *RequestMapPackage(const char *directory, const char *name, std::uintptr_t allocation);
PakGlobalData *RequestMapPackage(const char *directory, const char *name);
PakGlobalData *RequestPakFromAfsWithAllocation(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex);
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex, std::uintptr_t allocation);
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex);
void pakLoad(PakGlobalData *package);
void SetNewPakLoadedBuffer(PakGlobalData *package);
std::int32_t PakLoadWait();
std::int32_t GetPakReadiness(PakGlobalData *package);
std::int32_t *GetReadyPakMapHandles(PakGlobalData *package);
void SetupPakSpace(std::int32_t budget);
std::int32_t GetPakLoadBudget();
std::int32_t CancelPakSlot(PakGlobalData *package);
std::intptr_t EV_pakUtils(std::int32_t category, std::uint32_t mode, std::uintptr_t source, std::uintptr_t target, std::int32_t secondaryIndex);
#endif
} // namespace s2
} // namespace shenmue
#undef SHENMUE_PAK_LAYOUT_JOIN_
#undef SHENMUE_PAK_LAYOUT_JOIN
#undef SHENMUE_PAK_LAYOUT
#undef SHENMUE_PAK_ENUM_TYPE
#endif
