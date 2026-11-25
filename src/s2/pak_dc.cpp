#include "s2/pak_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/map_info.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/frame_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/texture_dc.hpp"
#include <cstring>
#include "s2/file_loader_dc.hpp"
#include "s2/stream_audio_dc.hpp"
#include <cri_adxf.h>
#include "s2/character_table_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include <zlib.h>
#include "platform/dc/sdk.hpp"
#include "platform/dc/mt7.hpp"
#include "platform/dc/am2_vector.hpp"
#include <math.h>
#include <string.h>
#define SHENMUE_LIGHT_DATA(name) name
#define SHENMUE_LIGHT_COUNT(name) (sizeof(name) / sizeof(name[0]))
#define SHENMUE_LIGHT_STORAGE(type, name, count) type name[count]
#define SHENMUE_LIGHT_INITIALIZER = {}
#define SHENMUE_LIGHT_MEMSET ::memset
#define SHENMUE_LIGHT_FABS ::fabsf
#define SHENMUE_LIGHT_NULL 0
#include "s2/hlib.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/search_dc.hpp"
#endif
#include "s2/model_dc.hpp"
#include "s2/character_table_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/stream_audio_dc.hpp"
#include <cstring>
#include <cstdio>
#include <cctype>
namespace shenmue {
namespace s2 {

void UnregisterLightData(std::int32_t identifier);
void ReleasePackageMapModel(void *model);
void ReleasePackageProp(void *model, std::uint32_t handle);
void ReleasePackageCharacters(PakGlobalData *package);
std::int32_t GetPakSlotIndex(const PakGlobalData *slot);
void PakLoadCallback(void *parameter);
PakGlobalData *RequestMapPackage(const char *directory, const char *name);
std::int32_t FreePakSlot(PakGlobalData *slot);
void pakLoadFlush();
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex, std::uintptr_t allocation);
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex);
void pakLoad(PakGlobalData *package);
void SetNewPakLoadedBuffer(PakGlobalData *package);
PakAfsPartition *MountPakAfs(const char *directory, const char *filename);
AudioPartition *MountAudioPartitionDefault(const char *directory, const char *filename, std::int32_t entries);
std::int32_t IsPakSlotIdle(const PakGlobalData *slot);
PakGlobalData *FindPakByEntryName(const char *name);
PakGlobalData *FindPakInAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex);
PakGlobalData *GetPakLoadTaskDataPointer();
PakGlobalData *GetPakLoadBuffer();
PakGlobalData *pakLoadGetNext();
std::int32_t PakLoadWait();
std::int32_t *GetReadyPakMapHandles(PakGlobalData *package);
MapInfoChunk *GetCurrentEventCameraChunk();
void SetupPakSpace(std::int32_t budget);
std::int32_t GetPakLoadBudget();
std::uint32_t GetPakLoadFlags();
void SetPakLoadFlags(std::uint32_t flags);

bool RegisterLightData(const void *block, std::int32_t identifier);
std::int32_t FindMapModelSlot(const MT7Model *model);
void FreeMapModelSlot(std::int32_t index);
void *LoadPackageMapModel(PakGlobalData *package, void *data, std::int32_t visible);
void *LoadPackageProp(void *data, std::uint32_t *handle);
HLib::Task *pakLoadTask;
PakLoadTaskData *pakLoadTaskData;
PakGlobalData pakSlots[kPakSlotCount];
PakAfsPartition pakPartitions[4];
std::uint32_t pakReadSize = 0;
char pakReadName[24] = {};
const void *pakReadData = 0;
std::int32_t pakReadHeaderCount = 0, pakReadAllocationSize = 0, pakReadBytes = 0;
std::int16_t g_mstsValue0 = 0, g_mstsValue1 = 0, g_mstsValue2 = 0;

namespace {

char pakFilename[32];

} // namespace

void *LoadPackageAuthor(void *data, PakGlobalData *slot);
void ReleasePackageAuthor(PakGlobalData *slot);
void *LoadPackageSearchData(void *data);
void ReleasePackageSearchData(void *data);
void *LoadPackageLockData(void *data);
void ReleasePackageLockData();
std::uint32_t RegisterMotionPropBank(void *data);
std::int32_t ReleaseMotionPropBank(std::uint32_t handle);
void *LoadPackageCharacterModel(void *data, const char *name, PakGlobalData *slot);
void *LoadPackageMapModel(PakGlobalData *slot, void *data, std::int32_t visible);
void *LoadPackageProp(void *data, std::uint32_t *handle);

namespace {

char pakEntryName[12];
std::int32_t pakEntryIndex;
shen_name pakEntryType;

} // namespace

void RemovePackagePropReader(std::uint32_t index);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void *LoadPackageFieldData(void *data);
#endif
static void *LoadPackageLightData(void *data);
static std::uint32_t LoadPackageMotionBank(void *data);
static std::uint32_t LoadPackageMotionPropBank(void *data);
static void ReleasePackageFieldData(void *data);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ReleasePackageLightData(void *data);
static void ReleasePackageEventScript(const char *name, std::int32_t type);
#endif
static void ReleasePackageMotionBank(std::uint32_t handle);
static void ReleasePackageMotionPropBank(std::uint32_t handle);
static void ReleasePackageBuffer(void *data);

// @verified - d
void MSTS_Reader(std::int16_t value0, std::int16_t value1, std::int16_t value2) {
    g_mstsValue0 = value0;
    g_mstsValue1 = value1;
    g_mstsValue2 = value2;
}

void GetMSTSValues(std::int16_t *values) {
    values[0] = g_mstsValue0;
    values[1] = g_mstsValue1;
    values[2] = g_mstsValue2;
}

// @verified - d
const char *BuildPakFilename(const char *name, const char *extension) {
    char separator[2];
    separator[0] = '.';
    separator[1] = 0;
    std::strcpy(pakFilename, name);
    std::strcat(pakFilename, separator);
    std::strcat(pakFilename, extension);
    return pakFilename;
}

// @verified - d
void ResetPakSlot(PakGlobalData *slot) {
    std::size_t size = sizeof(*slot);
    std::memset(slot, 0, size);
}

// @verified - d
void LoadPackageCharacters(void *data, shen_name allocationName) {
    std::uint32_t savedFlags;
    PakLoadTaskData *work;
    if (pakLoadTaskData)
        work = pakLoadTaskData;
    else
        work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    savedFlags = GetPakLoadFlags();
    SetPakLoadFlags(1);
    work->unknown04 = 1;
    LoadCharacters(data, g_entryPoint, allocationName);
    work->unknown04 = 0;
    SetPakLoadFlags(savedFlags);
}

namespace {

void DecompressPackage(const void *source, void *destination, void *, void *, std::uint32_t destinationBytes) {
    z_stream stream = {};
    stream.next_in = static_cast<Bytef *>(const_cast<void *>(source));
    stream.avail_in = UINT32_MAX;
    stream.next_out = static_cast<Bytef *>(destination);
    stream.avail_out = destinationBytes;
    if (inflateInit2(&stream, MAX_WBITS + 16) == Z_OK) {
        inflate(&stream, Z_FINISH);
        inflateEnd(&stream);
    }
}

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ComparePakNamesN(const char *left, const char *right, std::int32_t length) {
    const std::int32_t rightLength = static_cast<std::int32_t>(std::strlen(right));
    if (length < rightLength)
        length = rightLength;
    while (length > 0) {
        std::int32_t c = *left;
        if (isupper(c))
            c += 'a' - 'A';
        const std::int32_t a = c;
        const std::int32_t b = isupper(*right) ? *right + ('a' - 'A') : *right;
        ++left;
        ++right;
        if (a != b)
            return 1;
        --length;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ComparePakNames(const char *left, const char *right) {
    return ComparePakNamesN(left, right, static_cast<std::int32_t>(std::strlen(left)));
}
#endif

// @verified - d
const char *GetPakEntryName(const PakDirectoryEntry *entry) {
    std::strncpy(pakEntryName, entry->name, 8);
    return pakEntryName;
}

// @verified - d
std::int32_t FindPakEntryByType(const PakDirectory *directory, shen_name type) {
    if (type) {
        pakEntryIndex = 0;
        pakEntryType = type;
    }
    while (pakEntryIndex < directory->entryCount) {
        if (reinterpret_cast<const PakDirectoryEntry *>(
                reinterpret_cast<const std::byte *>(directory) + directory->entriesOffset + pakEntryIndex * sizeof(PakDirectoryEntry))
                    ->tag == pakEntryType ||
            pakEntryType == PACKAGE_ANY_EXTENSION)
            return pakEntryIndex++;
        ++pakEntryIndex;
    }
    return -1;
}

// @verified - d
std::int32_t LoadPakTextures(PakLoadTaskData *task, PakGlobalData *slot) {
    const PakFileHeader *const header = reinterpret_cast<const PakFileHeader *>(slot->data);
    const std::int32_t count = header->resourceCount;
    if (!count)
        return 1;
    std::int32_t index;
    const PakChunkHeader *chunk;
    if (!slot->textureHandles) {
        index = 0;
        if (slot->flags & PakAlternateHeap)
            slot->textureHandles = static_cast<std::int32_t *>(HLib::AllocateTaskData((count + 1) * 4, PACKAGE_TEXTURE_HANDLES));
        else
            slot->textureHandles = static_cast<std::int32_t *>(HLib::AllocateStaticOrDynamicBlock((count + 1) * 4, PACKAGE_TEXTURE_HANDLES));
        if (!slot->textureHandles)
            return 1;
        chunk = reinterpret_cast<const PakChunkHeader *>(
            reinterpret_cast<const std::byte *>(slot->data) + sizeof(PakFileHeader) + reinterpret_cast<std::uintptr_t>(slot->textureCursor));
        if (chunk->name == SHEN_NAME4('D', 'U', 'M', 'Y'))
            chunk = reinterpret_cast<const PakChunkHeader *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
    } else {
        index = slot->textureIndex;
        chunk = reinterpret_cast<const PakChunkHeader *>(slot->textureCursor);
    }
    float start;
    start = static_cast<float>(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister));
    if (slot->flags & PakKeepRetained)
        SetNamedTextureBank(2);
    for (; count > index; ++index, chunk = reinterpret_cast<const PakChunkHeader *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size)) {
        const float current = static_cast<float>(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister));
        const std::int32_t elapsed = static_cast<std::int32_t>((start - current) / 12.5f);
        if (elapsed > task->timeBudget && task->timeBudget != -1)
            break;
        slot->textureHandles[index] = LoadAndFindTEXN_Wrapper(reinterpret_cast<const TextureChunk *>(chunk));
        if (slot->textureHandles[index] < 0) {
            slot->state = -2;
            break;
        }
    }
    if (slot->flags & PakKeepRetained)
        ClearNamedTextureBank();
    slot->textureIndex = static_cast<std::int16_t>(index);
    slot->textureCursor = reinterpret_cast<const std::byte *>(chunk);
    if (index >= count) {
        slot->textureHandles[count] = -1;
        return 1;
    }
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define PAK_DIRECTORY_ENTRY(directory, index)                                                                                                                  \
    reinterpret_cast<PakDirectoryEntry *>((index) * sizeof(PakDirectoryEntry) + (directory)->entriesOffset + reinterpret_cast<std::uintptr_t>(directory))
#define PAK_ENTRY_DATA(directory, index) (reinterpret_cast<std::byte *>(directory) + PAK_DIRECTORY_ENTRY(directory, index)->dataOffset)

std::int32_t LoadPakResources(PakLoadTaskData *task, PakGlobalData *slot) {
    PakDirectory *directory;
    std::int32_t count;
    std::int32_t index;
    std::int32_t modelIndex = 0;
    std::int32_t motionIndex = 0;
    std::int32_t propIndex = 0;
    float start;
    directory = reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(slot->data) + static_cast<PakFileHeader *>(slot->data)->directoryOffset);
    count = directory->entryCount;
    index = slot->chunkIndex;
    if (!count)
        return 1;
    if (static_cast<PakFileHeader *>(slot->data)->name != PACKAGE_PKF_HEADER) {
        if (!index) {
            const std::int32_t resourceCount = static_cast<PakFileHeader *>(static_cast<volatile PakGlobalData *>(slot)->data)->resourceCount;
            modelIndex = 0;
            if (resourceCount) {
                if (slot->flags & 0x30U)
                    slot->modelHandles = static_cast<void **>(HLib::AllocateStaticOrDynamicBlock(resourceCount * 4U, PACKAGE_MODEL_HANDLES));
                else
                    slot->modelHandles = static_cast<void **>(HLib::AllocateTaskData(resourceCount * 4U, PACKAGE_MODEL_HANDLES));
                if (!slot->modelHandles)
                    return 1;
            } else
                slot->modelHandles = 0;
            std::int32_t mapCount = 0;
            std::int32_t scan;
            for (scan = FindPakEntryByType(directory, SHEN_NAME(MAPM)); scan >= 0; scan = FindPakEntryByType(directory, shen_name()))
                ++mapCount;
            for (scan = FindPakEntryByType(directory, SHEN_NAME(MAPD)); scan >= 0; scan = FindPakEntryByType(directory, shen_name()))
                ++mapCount;
            if (mapCount) {
                slot->mapHandles = static_cast<std::int32_t *>(HLib::AllocateTaskData((mapCount + 1) * 4U, PACKAGE_MODEL_HANDLES));
                if (!slot->mapHandles)
                    return 1;
                for (scan = 0; scan < mapCount + 1; ++scan)
                    slot->mapHandles[scan] = -1;
            } else
                slot->mapHandles = 0;
            motionIndex = 0;
            for (scan = FindPakEntryByType(directory, SHEN_NAME(MOTN)); scan >= 0; scan = FindPakEntryByType(directory, shen_name()))
                ++motionIndex;
            if (motionIndex) {
                slot->motionHandles = static_cast<std::uint32_t *>(HLib::AllocateTaskData(motionIndex * 4U, PACKAGE_MODEL_HANDLES));
                if (!slot->motionHandles)
                    return 1;
            } else
                slot->motionHandles = 0;
            slot->motionIndex = 0;
            motionIndex = 0;
            propIndex = 0;
            for (scan = FindPakEntryByType(directory, SHEN_NAME(PROP)); scan >= 0; scan = FindPakEntryByType(directory, shen_name()))
                ++propIndex;
            if (propIndex) {
                slot->propHandles = static_cast<std::uint32_t *>(HLib::AllocateTaskData(propIndex * 4U, PACKAGE_PROP_HANDLES));
                if (!slot->propHandles)
                    return 1;
            } else
                slot->propHandles = 0;
            slot->propIndex = 0;
            propIndex = 0;
        } else {
            modelIndex = slot->modelIndex;
            motionIndex = slot->motionIndex;
            propIndex = slot->propIndex;
        }
    }
    start = static_cast<float>(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister));
    for (; count > index; ++index) {
        const float current = static_cast<float>(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister));
        if (static_cast<std::int32_t>((start - current) / 12.5f) > task->timeBudget && task->timeBudget != -1)
            break;
        switch (PAK_DIRECTORY_ENTRY(directory, index)->tag) {
        case SHEN_NAME4('M', 'O', 'T', 'N'):
            slot->motionHandles[motionIndex] = LoadPackageMotionBank(PAK_ENTRY_DATA(directory, index));
            ++motionIndex;
            break;
        case SHEN_NAME4('M', 'O', 'T', 'P'):
            slot->motionProperty = LoadPackageMotionPropBank(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('S', 'R', 'C', 'H'):
            LoadPackageSearchData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('L', 'O', 'C', 'K'):
            LoadPackageLockData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('F', 'L', 'D', 'D'):
            LoadPackageFieldData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('D', 'Y', 'N', 'M'):
            if (!slot->dynamicMapData)
                slot->dynamicMapData = PAK_ENTRY_DATA(directory, index);
            break;
        case SHEN_NAME4('S', 'N', 'D', 'P'):
            if (!slot->soundTable)
                slot->soundTable = reinterpret_cast<const std::byte *>(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('A', 'U', 'T', 'H'):
            LoadPackageAuthor(PAK_ENTRY_DATA(directory, index), slot);
            break;
        case SHEN_NAME4('L', 'G', 'H', 'T'):
            LoadPackageLightData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('C', 'H', 'R', 'M'):
            slot->modelHandles[modelIndex++] =
                LoadPackageCharacterModel(PAK_ENTRY_DATA(directory, index), GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)), slot);
            break;
        case SHEN_NAME4('M', 'A', 'P', 'M'):
            slot->modelHandles[modelIndex++] = LoadPackageMapModel(slot, PAK_ENTRY_DATA(directory, index), 1);
            break;
        case SHEN_NAME4('M', 'A', 'P', 'D'):
            slot->modelHandles[modelIndex++] = LoadPackageMapModel(slot, PAK_ENTRY_DATA(directory, index), 0);
            break;
        case SHEN_NAME4('P', 'R', 'O', 'P'):
            slot->modelHandles[modelIndex++] = LoadPackageProp(PAK_ENTRY_DATA(directory, index), slot->propHandles + propIndex);
            ++propIndex;
            break;
        case SHEN_NAME4('C', 'H', 'R', 'T'):
            if (!(slot->flags & PakCharactersLoaded)) {
                task->data = PAK_ENTRY_DATA(directory, index);
                task->dataSize = PAK_DIRECTORY_ENTRY(directory, index)->auxiliary;
                slot->flags |= PakCharactersLoaded;
            }
            break;
        case SHEN_NAME4('E', 'V', '1', ' '):
            InstallEventOverlay(PAK_ENTRY_DATA(directory, index), GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)),
                PAK_DIRECTORY_ENTRY(directory, index)->auxiliary, static_cast<EventFunctionType>(1));
            break;
        case SHEN_NAME4('E', 'V', '2', ' '):
            InstallEventOverlay(PAK_ENTRY_DATA(directory, index), GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)),
                PAK_DIRECTORY_ENTRY(directory, index)->auxiliary, static_cast<EventFunctionType>(2));
            break;
        case SHEN_NAME4('E', 'V', '3', ' '):
            InstallEventOverlay(PAK_ENTRY_DATA(directory, index), GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)),
                PAK_DIRECTORY_ENTRY(directory, index)->auxiliary, static_cast<EventFunctionType>(3));
            break;
        case SHEN_NAME4('S', 'C', 'R', '0'):
        case SHEN_NAME4('S', 'C', 'R', '1'):
        case SHEN_NAME4('S', 'C', 'R', 'L'):
        case SHEN_NAME4('B', 'I', 'N', ' '):
        case SHEN_NAME4('C', 'O', 'L', 'I'):
        case SHEN_NAME4('E', 'C', 'A', 'M'):
            break;
        }
    }
    if (static_cast<PakFileHeader *>(slot->data)->name == PACKAGE_PKS_HEADER) {
        slot->modelIndex = static_cast<std::int16_t>(modelIndex);
        slot->motionIndex = static_cast<std::int16_t>(motionIndex);
        slot->propIndex = static_cast<std::int16_t>(propIndex);
    }
    if (index >= count) {
        slot->chunkIndex = 0;
        return 1;
    }
    slot->chunkIndex = static_cast<std::int16_t>(index);
    return 0;
}
#undef PAK_ENTRY_DATA
#undef PAK_DIRECTORY_ENTRY
#else
std::int32_t LoadPakResources(PakLoadTaskData *task, PakGlobalData *slot) {
    std::byte *const data = static_cast<std::byte *>(slot->data);
    const PakFileHeader *const header = reinterpret_cast<const PakFileHeader *>(data);
    PakDirectory *const directory = reinterpret_cast<PakDirectory *>(data + header->directoryOffset);
    if (!directory->entryCount)
        return 1;
    std::int32_t index = slot->chunkIndex;
    std::int32_t modelIndex = 0;
    std::int32_t motionIndex = 0;
    std::int32_t propIndex = 0;
    if (header->name != PACKAGE_PKF_HEADER) {
        if (!index) {
            if (!header->resourceCount)
                slot->modelHandles = 0;
            else {
                const std::uint32_t bytes = static_cast<std::uint32_t>(header->resourceCount) * 4U;
                slot->modelHandles = static_cast<void **>(slot->flags & 0x30U ? HLib::AllocateStaticOrDynamicBlock(bytes, PACKAGE_MODEL_HANDLES) :
                                                                                HLib::AllocateTaskData(bytes, PACKAGE_MODEL_HANDLES));
                if (!slot->modelHandles)
                    return 1;
            }
            std::int32_t count = 0;
            if (FindPakEntryByType(directory, SHEN_NAME(MAPM)) >= 0) {
                count = 1;
                while (FindPakEntryByType(directory, shen_name()) >= 0)
                    ++count;
            }
            std::int32_t found = FindPakEntryByType(directory, SHEN_NAME(MAPD));
            while (found >= 0) {
                ++count;
                found = FindPakEntryByType(directory, shen_name());
            }
            if (!count)
                slot->mapHandles = 0;
            else {
                slot->mapHandles = static_cast<std::int32_t *>(HLib::AllocateTaskData((count + 1) * 4U, PACKAGE_MODEL_HANDLES));
                if (!slot->mapHandles)
                    return 1;
                for (std::int32_t map = 0; map < count + 1; ++map)
                    slot->mapHandles[map] = -1;
            }
            count = 0;
            if (FindPakEntryByType(directory, SHEN_NAME(MOTN)) >= 0) {
                count = 1;
                while (FindPakEntryByType(directory, shen_name()) >= 0)
                    ++count;
            }
            slot->motionHandles = count ? static_cast<std::uint32_t *>(HLib::AllocateTaskData(count * 4U, PACKAGE_MODEL_HANDLES)) : 0;
            if (count && !slot->motionHandles)
                return 1;
            slot->motionIndex = 0;
            count = 0;
            if (FindPakEntryByType(directory, SHEN_NAME(PROP)) >= 0) {
                count = 1;
                while (FindPakEntryByType(directory, shen_name()) >= 0)
                    ++count;
            }
            slot->propHandles = count ? static_cast<std::uint32_t *>(HLib::AllocateTaskData(count * 4U, PACKAGE_PROP_HANDLES)) : 0;
            if (count && !slot->propHandles)
                return 1;
            slot->propIndex = 0;
        } else {
            modelIndex = slot->modelIndex;
            motionIndex = slot->motionIndex;
            propIndex = slot->propIndex;
        }
    }
    std::byte *const base = reinterpret_cast<std::byte *>(directory);
    const PakDirectoryEntry *const entries = reinterpret_cast<const PakDirectoryEntry *>(base + directory->entriesOffset);
    const float start = static_cast<float>(*reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister));
    for (; index < directory->entryCount; ++index) {
        const float current = static_cast<float>(*reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister));
        const std::int32_t elapsed = static_cast<std::int32_t>((start - current) / 12.5f);
        if (task->timeBudget < elapsed && task->timeBudget != -1)
            break;
        const PakDirectoryEntry &entry = entries[index];
        void *const payload = base + entry.dataOffset;
        switch (entry.tag) {
        case SHEN_NAME4('E', 'V', '1', ' '):
        case SHEN_NAME4('E', 'V', '2', ' '):
        case SHEN_NAME4('E', 'V', '3', ' '):
            InstallEventOverlay(payload, GetPakEntryName(&entry), entry.auxiliary, static_cast<EventFunctionType>(((entry.tag >> 16U) & 0xffU) - '0'));
            break;
        case SHEN_NAME4('F', 'L', 'D', 'D'): {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            LoadPackageFieldData(payload);
#else
            PakChunkHeader *chunk = reinterpret_cast<PakChunkHeader *>(static_cast<std::byte *>(payload) + 8);
            while (chunk->size) {
                ParseFLDD(chunk + 1, chunk->name);
                chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
            }
#endif
            break;
        }
        case SHEN_NAME4('A', 'U', 'T', 'H'):
            LoadPackageAuthor(payload, slot);
            break;
        case SHEN_NAME4('L', 'G', 'H', 'T'):
            LoadPackageLightData(payload);
            break;
        case SHEN_NAME4('M', 'O', 'T', 'N'):
            slot->motionHandles[motionIndex++] = LoadPackageMotionBank(payload);
            break;
        case SHEN_NAME4('M', 'O', 'T', 'P'):
            slot->motionProperty = LoadPackageMotionPropBank(payload);
            break;
        case SHEN_NAME4('S', 'R', 'C', 'H'):
            LoadPackageSearchData(payload);
            break;
        case SHEN_NAME4('L', 'O', 'C', 'K'):
            LoadPackageLockData(payload);
            break;
        case SHEN_NAME4('C', 'H', 'R', 'M'):
            slot->modelHandles[modelIndex++] = LoadPackageCharacterModel(payload, GetPakEntryName(&entry), slot);
            break;
        case SHEN_NAME4('M', 'A', 'P', 'M'):
        case SHEN_NAME4('M', 'A', 'P', 'D'):
            slot->modelHandles[modelIndex++] = LoadPackageMapModel(slot, payload, entry.tag == SHEN_NAME(MAPM));
            break;
        case SHEN_NAME4('P', 'R', 'O', 'P'):
            slot->modelHandles[modelIndex++] = LoadPackageProp(payload, slot->propHandles + propIndex++);
            break;
        case SHEN_NAME4('S', 'N', 'D', 'P'):
            if (!slot->soundTable)
                slot->soundTable = reinterpret_cast<const std::byte *>(payload);
            break;
        case SHEN_NAME4('D', 'Y', 'N', 'M'):
            if (!slot->dynamicMapData)
                slot->dynamicMapData = payload;
            break;
        case SHEN_NAME4('C', 'H', 'R', 'T'):
            if (!(slot->flags & PakCharactersLoaded)) {
                task->data = payload;
                task->dataSize = entry.auxiliary;
                slot->flags |= PakCharactersLoaded;
            }
            break;
        }
    }
    if (header->name == PACKAGE_PKS_HEADER) {
        slot->modelIndex = static_cast<std::int16_t>(modelIndex);
        slot->motionIndex = static_cast<std::int16_t>(motionIndex);
        slot->propIndex = static_cast<std::int16_t>(propIndex);
    }
    slot->chunkIndex = index < directory->entryCount ? static_cast<std::int16_t>(index) : 0;
    return index >= directory->entryCount;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BuildAfsPakName(const char *filename, std::int32_t firstIndex, std::int32_t secondIndex, char *output) {
    std::sprintf(output, "%s%04ld%04ld", filename, static_cast<long>(firstIndex), static_cast<long>(secondIndex));
}
#endif

// @verified - d
void ClosePakRead(PakGlobalData *slot, std::int32_t releaseBuffer) {
    if (slot->flags & PakAfsRead) {
        if (slot->afsFile) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            if (slot->afsFile->stm)
#else
            if (ADXF_GetStat(slot->afsFile))
#endif
                ADXF_Stop(slot->afsFile);
            ADXF_Close(slot->afsFile);
            slot->afsFile = 0;
        }
    } else if (slot->fileLoad) {
        CloseFile(slot->fileLoad);
        slot->fileLoad = 0;
    }
    if (releaseBuffer) {
        if (!slot->destination)
            HLib::FreeTaskData(slot->data);
        slot->data = 0;
    }
}

// @verified - d
void OpenPakLooseRead(PakLoadTaskData *task, PakGlobalData *slot, const char *directory, const char *filename) {
    task->retryCount = 0;
    slot->flags &= ~PakAfsRead;
    slot->fileLoad = Archive_GetNextUnusedFileState();
    if (!OpenFile(slot->fileLoad, directory, filename)) {
        task->pakBuffer = 0;
        return;
    }
    const std::uint32_t size = slot->fileLoad->size;
    slot->dataSize = size;
    const shen_name token = make_name('P', 'A', 'K', static_cast<char>('a' + GetPakSlotIndex(slot)));
    if (!(slot->flags & PakPksPhase) || (task->loadFlags & 4U)) {
        const std::uint32_t aligned = (size + 31U) & ~31U;
        const std::uint32_t prefixBytes = size < 0x801U ? aligned : 0x800U;
        void *prefix;
        if (!slot->destination) {
            prefix = HLib::AllocateTaskData(aligned, token);
        } else {
            prefix = slot->destination;
            if (HLib::GetTaskDataSize(prefix) < aligned)
                prefix = 0;
        }
        if (!prefix) {
            task->pakBuffer = 0;
            ClosePakRead(slot, 0);
            return;
        }
        CopyFileToHLibMemory(slot->fileLoad, prefix, prefixBytes);
        FileStatAndProcessIfExists(slot->fileLoad);
        const shen_name expected = task->loadFlags & 4U ? PACKAGE_PKS_HEADER : PACKAGE_PKF_HEADER;
        if (*reinterpret_cast<const std::uint32_t *>(prefix) == expected) {
            slot->data = prefix;
        } else {
            const std::uint8_t *const bytes = reinterpret_cast<const std::uint8_t *>(prefix);
            if (bytes[0] == 0x1f && bytes[1] == 0x8b && bytes[2] == 8 && !(bytes[3] & 0x10U)) {
                slot->flags |= PakCompressed;
                if (prefixBytes < size && !slot->destination)
                    HLib::ShrinkTaskData(prefix, prefixBytes);
                slot->data = HLib::AllocateStaticOrDynamicBlock(aligned, token);
                if (!slot->data) {
                    if (!slot->destination)
                        HLib::FreeTaskData(prefix);
                    task->pakBuffer = 0;
                    ClosePakRead(slot, 0);
                    return;
                }
                std::memcpy(slot->data, prefix, prefixBytes);
                if (!slot->destination)
                    HLib::FreeTaskData(prefix);
            }
        }
        if (prefixBytes < size)
            CopyFileToHLibMemory(slot->fileLoad, static_cast<std::byte *>(slot->data) + prefixBytes, size - prefixBytes);
    } else {
        if (!(slot->flags & PakCompressed))
            slot->data = slot->destination ? slot->destination : HLib::AllocateTaskData(size, token);
        else
            slot->data = HLib::AllocateStaticOrDynamicBlock(size, token);
        if (!slot->data) {
            task->pakBuffer = 0;
            ClosePakRead(slot, 0);
            return;
        }
        CopyFileToHLibMemory(slot->fileLoad, slot->data, size);
    }
    std::strcpy(pakReadName, slot->entryType);
    pakReadSize = slot->dataSize;
    pakReadData = slot->data;
    pakReadHeaderCount = -1;
    pakReadAllocationSize = static_cast<std::int32_t>(HLib::GetTaskDataSize(slot->data));
    pakReadBytes = -1;
}

// @verified - d
void OpenPakAfsRead(PakLoadTaskData *task, PakGlobalData *slot, std::int32_t fileIndex) {
    slot->flags |= PakAfsRead;
    slot->afsFile = ADXF_OpenAfs(slot->partition->afs->id, fileIndex);
    const std::uint32_t bytes = ADXF_GetFsizeSct(slot->afsFile) * 0x800;
    if (!slot->data) {
        if (!slot->destination) {
            if (slot->flags & PakAlternateHeap)
                slot->data =
                    HLib::AllocateStaticOrDynamicBlock(bytes, static_cast<shen_name>('a' + GetPakSlotIndex(slot)) << 24 | SHEN_NAME4('P', 'A', 'K', 0));
            else
                slot->data = HLib::AllocateTaskData(bytes, static_cast<shen_name>('a' + GetPakSlotIndex(slot)) << 24 | SHEN_NAME4('P', 'A', 'K', 0));
        } else {
            slot->data = slot->destination;
            if (HLib::GetTaskDataSize(slot->data) < bytes)
                slot->data = 0;
        }
        if (!slot->data) {
            task->pakBuffer = 0;
            ClosePakRead(slot, 0);
            return;
        }
    }
    ADXF_ReadNw32(slot->afsFile, bytes / 0x800, slot->data);
    slot->dataSize = bytes;
    std::strcpy(pakReadName, slot->entryType);
    pakReadSize = slot->dataSize;
    pakReadData = slot->data;
    pakReadHeaderCount = -1;
    pakReadAllocationSize = static_cast<std::int32_t>(HLib::GetTaskDataSize(slot->data));
    pakReadBytes = -1;
}

void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
void ReleasePakTextures(PakGlobalData *slot) {
    std::int32_t remaining = slot->textureIndex;
    const std::int32_t *texture = slot->textureHandles;
    while (--remaining != -1)
        ReleaseNamedTexture_Wrapper(*texture++);
    if (slot->textureHandles) {
        HLib::FreeTaskData(slot->textureHandles);
        slot->textureHandles = 0;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define PAK_DIRECTORY_ENTRY(directory, index)                                                                                                                  \
    reinterpret_cast<PakDirectoryEntry *>((index) * sizeof(PakDirectoryEntry) + (directory)->entriesOffset + reinterpret_cast<std::uintptr_t>(directory))
#define PAK_ENTRY_DATA(directory, index) (reinterpret_cast<std::byte *>(directory) + PAK_DIRECTORY_ENTRY(directory, index)->dataOffset)

void ReleasePakResources(PakGlobalData *slot) {
    PakDirectory *directory;
    std::int32_t count;
    std::int32_t index;
    std::int32_t modelIndex;
    std::int32_t motionIndex;
    std::int32_t propIndex;
    if (slot < pakSlots || slot > &pakSlots[kPakSlotCount - 1])
        return;
    if (slot->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(slot)) << 24)) || !slot->data)
        return;
    directory = reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(slot->data) + static_cast<PakFileHeader *>(slot->data)->directoryOffset);
    count = slot->chunkIndex;
    if (!count)
        count = directory->entryCount;
    modelIndex = 0;
    motionIndex = 0;
    propIndex = 0;
    for (index = 0; count > index; ++index) {
        switch (PAK_DIRECTORY_ENTRY(directory, index)->tag) {
        case SHEN_NAME4('F', 'L', 'D', 'D'):
            ReleasePackageFieldData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('M', 'O', 'T', 'N'):
            ReleasePackageMotionBank(slot->motionHandles[motionIndex++]);
            break;
        case SHEN_NAME4('M', 'O', 'T', 'P'):
            ReleasePackageMotionPropBank(slot->motionProperty);
            break;
        case SHEN_NAME4('S', 'R', 'C', 'H'):
            ReleasePackageSearchData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('L', 'O', 'C', 'K'):
            ReleasePackageLockData();
            break;
        case SHEN_NAME4('S', 'N', 'D', 'P'):
        case SHEN_NAME4('D', 'Y', 'N', 'M'):
            break;
        case SHEN_NAME4('A', 'U', 'T', 'H'):
            ReleasePackageAuthor(slot);
            break;
        case SHEN_NAME4('L', 'G', 'H', 'T'):
            ReleasePackageLightData(PAK_ENTRY_DATA(directory, index));
            break;
        case SHEN_NAME4('C', 'H', 'R', 'M'):
            ReleasePackageBuffer(slot->modelHandles[modelIndex++]);
            break;
        case SHEN_NAME4('M', 'A', 'P', 'M'):
        case SHEN_NAME4('M', 'A', 'P', 'D'):
            ReleasePackageMapModel(slot->modelHandles[modelIndex++]);
            break;
        case SHEN_NAME4('P', 'R', 'O', 'P'):
            ReleasePackageProp(slot->modelHandles[modelIndex++], slot->propHandles[propIndex++]);
            break;
        case SHEN_NAME4('E', 'V', '1', ' '):
            ReleasePackageEventScript(GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)), 1);
            break;
        case SHEN_NAME4('E', 'V', '2', ' '):
            ReleasePackageEventScript(GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)), 2);
            break;
        case SHEN_NAME4('E', 'V', '3', ' '):
            ReleasePackageEventScript(GetPakEntryName(PAK_DIRECTORY_ENTRY(directory, index)), 3);
            break;
        case SHEN_NAME4('S', 'C', 'R', '1'):
        case SHEN_NAME4('S', 'C', 'R', 'L'):
        case SHEN_NAME4('B', 'I', 'N', ' '):
        case SHEN_NAME4('C', 'H', 'R', 'T'):
        case SHEN_NAME4('S', 'C', 'R', '0'):
        case SHEN_NAME4('C', 'O', 'L', 'I'):
        case SHEN_NAME4('E', 'C', 'A', 'M'):
            break;
        }
    }
    if (slot->flags & PakCharactersLoaded)
        ReleasePackageCharacters(slot);
}
#undef PAK_ENTRY_DATA
#undef PAK_DIRECTORY_ENTRY
#else
void ReleasePakResources(PakGlobalData *slot) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(slot);
    if (address < reinterpret_cast<std::uintptr_t>(pakSlots) || address > reinterpret_cast<std::uintptr_t>(pakSlots + kPakSlotCount - 1))
        return;
    const std::int32_t slotIndex = GetPakSlotIndex(slot);
    if (slot->name != make_name('H', 'P', 'M', static_cast<char>('a' + slotIndex)) || !slot->data)
        return;
    const PakFileHeader *const header = reinterpret_cast<const PakFileHeader *>(slot->data);
    std::byte *const base = static_cast<std::byte *>(slot->data) + header->directoryOffset;
    const PakDirectory *const directory = reinterpret_cast<const PakDirectory *>(base);
    const PakDirectoryEntry *const entries = reinterpret_cast<const PakDirectoryEntry *>(base + directory->entriesOffset);
    const std::int32_t count = slot->chunkIndex ? slot->chunkIndex : directory->entryCount;
    std::int32_t model = 0, motion = 0, prop = 0;
    for (std::int32_t index = 0; index < count; ++index) {
        const PakDirectoryEntry &entry = entries[index];
        void *const data = base + entry.dataOffset;
        switch (entry.tag) {
        case SHEN_NAME4('E', 'V', '1', ' '):
        case SHEN_NAME4('E', 'V', '2', ' '):
        case SHEN_NAME4('E', 'V', '3', ' '):
            scenFreeEvent(GetPakEntryName(&entry));
            break;
        case SHEN_NAME4('F', 'L', 'D', 'D'):
            ReleasePackageFieldData(data);
            break;
        case SHEN_NAME4('L', 'G', 'H', 'T'): {
            PakChunkHeader *chunk = reinterpret_cast<PakChunkHeader *>(static_cast<std::byte *>(data) + 8);
            while (chunk->size) {
                UnregisterLightData(static_cast<std::int32_t>(chunk->name));
                chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
            }
            break;
        }
        case SHEN_NAME4('A', 'U', 'T', 'H'):
            ReleasePackageAuthor(slot);
            break;
        case SHEN_NAME4('M', 'O', 'T', 'N'):
            ReleasePackageMotionBank(slot->motionHandles[motion++]);
            break;
        case SHEN_NAME4('M', 'O', 'T', 'P'):
            ReleasePackageMotionPropBank(slot->motionProperty);
            break;
        case SHEN_NAME4('S', 'R', 'C', 'H'):
            ReleasePackageSearchData(data);
            break;
        case SHEN_NAME4('L', 'O', 'C', 'K'):
            ReleasePackageLockData();
            break;
        case SHEN_NAME4('C', 'H', 'R', 'M'):
            ReleasePackageBuffer(slot->modelHandles[model++]);
            break;
        case SHEN_NAME4('M', 'A', 'P', 'M'):
        case SHEN_NAME4('M', 'A', 'P', 'D'):
            ReleasePackageMapModel(slot->modelHandles[model++]);
            break;
        case SHEN_NAME4('P', 'R', 'O', 'P'):
            ReleasePackageProp(slot->modelHandles[model++], slot->propHandles[prop++]);
            break;
        }
    }
    if (slot->flags & PakCharactersLoaded)
        ReleasePackageCharacters(slot);
}
#endif

// @verified - d
void FreePakSlotStorage(PakGlobalData *slot) {
    if (slot->modelHandles) {
        HLib::FreeTaskData(slot->modelHandles);
        slot->modelHandles = 0;
    }
    if (slot->mapHandles) {
        HLib::FreeTaskData(slot->mapHandles);
        slot->mapHandles = 0;
    }
    if (slot->motionHandles) {
        HLib::FreeTaskData(slot->motionHandles);
        slot->motionHandles = 0;
    }
    if (slot->propHandles) {
        HLib::FreeTaskData(slot->propHandles);
        slot->propHandles = 0;
    }
    if (!slot->destination)
        HLib::FreeTaskData(slot->data);
    slot->data = 0;
    slot->name = __typeof__(slot->name)();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void *LoadPackageFieldData(void *data) {
    PakChunkHeader *chunk = reinterpret_cast<PakChunkHeader *>(static_cast<std::byte *>(data) + 8);
    while (chunk->size) {
        ParseFLDD(chunk + 1, chunk->name);
        chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
    }
    return data;
}
#endif

static void *LoadPackageLightData(void *data) {
    PakChunkHeader *chunk = reinterpret_cast<PakChunkHeader *>(static_cast<std::byte *>(data) + 8);
    while (chunk->size) {
        RegisterLightData(chunk + 1, static_cast<std::int32_t>(chunk->name));
        chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
    }
    return data;
}

static std::uint32_t LoadPackageMotionBank(void *data) {
    return RegisterMotionBank(data);
}

static std::uint32_t LoadPackageMotionPropBank(void *data) {
    return RegisterMotionPropBank(data);
}

static void ReleasePackageFieldData(void *data) {
    PakChunkHeader *chunk = reinterpret_cast<PakChunkHeader *>(static_cast<std::byte *>(data) + 8);
    while (chunk->size) {
        RemoveFieldPart(chunk->name);
        chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ReleasePackageLightData(void *data) {
    PakChunkHeader *chunk = reinterpret_cast<PakChunkHeader *>(static_cast<std::byte *>(data) + 8);
    while (chunk->size) {
        UnregisterLightData(static_cast<std::int32_t>(chunk->name));
        chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
    }
}
#endif

// @verified - d
void UnregisterLightData(std::int32_t identifier) {
    int index;
    for (index = 0; index < 24; ++index) {
        if (g_lightDefinitions[index].identifier == static_cast<std::uint32_t>(identifier)) {
            break;
        }
    }
    if (index < 24) {
        g_lightDefinitions[index].block = SHENMUE_LIGHT_NULL;
        g_lightDefinitions[index].identifier = UINT32_MAX;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ReleasePackageEventScript(const char *name, std::int32_t type) {
    char filename[16];
    char extension[8];
    std::strcpy(filename, name);
    std::sprintf(extension, ".EV%01d", type);
    std::strcat(filename, extension);
    scenFreeEvent(filename);
}
#endif

static void ReleasePackageMotionBank(std::uint32_t handle) {
    ReleaseMotionBank(handle);
}

static void ReleasePackageMotionPropBank(std::uint32_t handle) {
    ReleaseMotionPropBank(handle);
}

static void ReleasePackageBuffer(void *data) {
    HLib::FreeTaskData(data);
}

// @verified - d
void ReleasePackageMapModel(void *model) {
    const std::int32_t index = FindMapModelSlot(static_cast<MT7Model *>(model));
    FreeMapModelSlot(index);
    HLib::FreeTaskData(model);
}

// @verified - d
void ReleasePackageProp(void *model, std::uint32_t handle) {
    RemovePackagePropReader(handle);
    HLib::FreeTaskData(model);
}

// @verified - d
void ReleasePackageCharacters(PakGlobalData *package) {
    PakLoadTaskData *work;
    if (pakLoadTaskData)
        work = pakLoadTaskData;
    else
        work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    const shen_name name = SHEN_NAME4('P', 'A', 'K', 0) | static_cast<shen_name>('a' + GetPakSlotIndex(package)) << 24;
    PakGlobalData *const savedNext = work->next;
    work->next = package;
    work->unknown04 = 1;
    KillCharactersByModelName(name, 1);
    CleanupMapControlCharacters();
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    ReleasePackageModelImages(control->modelManagerTask, package);
    work->unknown04 = 0;
    work->next = savedNext;
}

// @verified - d
void ResetPakAfsPartition(PakAfsPartition *partition) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(partition);
    if (address >= reinterpret_cast<std::uintptr_t>(pakPartitions) && address <= reinterpret_cast<std::uintptr_t>(pakPartitions + 3)) {
        partition->afs = 0;
        partition->folder[0] = '\0';
        partition->filename[0] = '\0';
        partition->references = 0;
    }
}

// @verified - d
std::int32_t GetPakSlotIndex(const PakGlobalData *slot) {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index) {
        if (pakSlots + index == slot)
            return index;
    }
    return -1;
}

// @verified - d
PakGlobalData *AllocatePakSlot(const char *name, PakAfsPartition *partition, std::int32_t firstFile, std::int32_t secondFile, std::uintptr_t allocation) {
    PakGlobalData *slot = 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index) {
        if (!pakSlots[index].name) {
            slot = pakSlots + index;
            break;
        }
    }
    if (!slot)
        return 0;
    ResetPakSlot(slot);
    const std::int32_t index = GetPakSlotIndex(slot);
    slot->name = (static_cast<std::uint32_t>('a' + index) << 24) | SHEN_NAME4('H', 'P', 'M', 0);
    std::strcpy(slot->entryType, name);
    slot->partition = partition;
    slot->firstFile = firstFile;
    slot->secondFile = secondFile;
    if (allocation > 0x0c000000U)
        slot->destination = reinterpret_cast<void *>(allocation);
    else if (allocation == 1)
        slot->flags |= PakAlternateHeap;
    slot->motionProperty = UINT32_MAX;
    return slot;
}

// @verified - d
void PakLoadTaskEntry_Enqueue(std::int8_t index, std::int32_t useSceneTextHeap) {
    PakLoadTaskData *work;
    if (useSceneTextHeap) {
        pakLoadTask = HLib::EnqueueTaskWithParameter(PakLoadCallback, index, 2, 0, PACKAGE_TASK);
        work = reinterpret_cast<PakLoadTaskData *>(allocateInGlobalHeap(sizeof(PakLoadTaskData)));
        pakLoadTaskData = work;
    } else {
        pakLoadTask = HLib::EnqueueTaskWithParameter(PakLoadCallback, index, 2, sizeof(PakLoadTaskData), PACKAGE_TASK);
        work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
        pakLoadTaskData = 0;
    }
    work->name = PACKAGE_TASK;
    work->unknown04 = 0;
    work->flags = 0;
    work->path[0] = '\0';
    work->data = 0;
    work->dataSize = 0;
    work->linkedPakEntry = 0;
    work->pakBuffer = 0;
    work->pakt = 0;
    work->next = 0;
    work->selectedPackage = 0;
    work->loadFlags = 0;
    work->timeBudget = 3000;
    work->retryCount = 0;
    std::int32_t slotIndex;
    for (slotIndex = 0; slotIndex < static_cast<std::int32_t>(sizeof(pakSlots) / sizeof(pakSlots[0])); ++slotIndex)
        ResetPakSlot(&pakSlots[slotIndex]);
    for (slotIndex = 0; slotIndex < static_cast<std::int32_t>(sizeof(pakPartitions) / sizeof(pakPartitions[0])); ++slotIndex)
        ResetPakAfsPartition(&pakPartitions[slotIndex]);
    HLib::SetupTimedTask(pakLoadTask, 0);
    std::memset(pakReadName, 0, sizeof(pakReadName));
    pakReadSize = UINT32_MAX;
    pakReadHeaderCount = -1;
    pakReadData = 0;
    pakReadAllocationSize = -1;
    pakReadBytes = -1;
}

void PakLoadCallback(void *parameter) {
    PakLoadTaskData *task = static_cast<PakLoadTaskData *>(parameter);
    void *temporary = 0;
    std::int32_t complete;
    std::uint32_t bytes = 0;
    if (!task) {
        PakLoadTaskData *const work = pakLoadTaskData;
        if (!work)
            return;
        task = work;
    }
    if (!task->pakBuffer && !task->pakt && !task->next) {
        HLib::SetupTimedTask(pakLoadTask, 0);
        return;
    }
    PakGlobalData *slot;
    if (task->pakBuffer) {
        slot = task->pakBuffer;
        complete = 0;
        if (slot->flags & PakAfsRead) {
            const std::int32_t status = ADXF_GetStat(slot->afsFile);
            switch (status) {
            case 3:
                pakReadBytes = ADXF_GetNumReadSct(slot->afsFile) * 0x800;
                complete = 1;
                break;
            case 2:
                break;
            case 1:
            case 4:
            default: {
                PakGlobalData *const failed = task->pakBuffer;
                PakAfsPartition *const partition = failed->partition;
                if (failed->afsFile) {
                    ClosePakRead(failed, 1);
                    failed->afsFile = 0;
                }
                if (partition) {
                    if (partition->afs) {
                        ReleaseAudioPartition(partition->afs);
                        partition->afs = 0;
                    }
                    if (std::strlen(partition->folder) && std::strlen(partition->filename) && std::strlen(partition->filename) < 0x10 &&
                        task->retryCount++ < 0x78) {
                        partition->afs = MountAudioPartitionDefault(partition->folder, partition->filename, 0x800);
                        if (partition->afs)
                            OpenPakAfsRead(task, failed, failed->flags & PakPksPhase ? failed->secondFile : failed->firstFile);
                        else
                            task->pakBuffer = 0;
                    } else {
                        task->pakBuffer = 0;
                        UnmountPakAfsPartition(partition);
                        FreePakSlotStorage(failed);
                    }
                } else {
                    task->pakBuffer = 0;
                    FreePakSlotStorage(failed);
                }
                if (!task->pakBuffer)
                    CheckDiscTray();
                break;
            }
            }
        } else if (IsFileReadIdle(slot->fileLoad)) {
            pakReadBytes = static_cast<std::int32_t>(GetFilePosition(slot->fileLoad));
            complete = 1;
        }
        if (complete) {
            slot = task->pakBuffer;
            task->pakBuffer = 0;
            if (slot->flags & PakCompressed) {
                void *const compressed = slot->data;
                bytes = am2ReadUnalignedLe32(compressed, static_cast<std::int32_t>(slot->dataSize));
                void *output;
                if (!slot->destination) {
                    output = HLib::AllocateTaskData(bytes, SHEN_NAME4('P', 'A', 'K', 0) | static_cast<shen_name>('a' + GetPakSlotIndex(slot)) << 24);
                } else {
                    output = slot->destination;
                    if (HLib::GetTaskDataSize(output) < bytes)
                        output = 0;
                }
                void *const workspace = HLib::AllocateStaticOrDynamicBlock(0x3000, shen_name());
                void *const scratch = HLib::AllocateStaticOrDynamicBlock(0x1000, shen_name());
                if (!output || !workspace || !scratch) {
                    FreePakSlot(slot);
                    return;
                }
                DecompressPackage(compressed, output, workspace, static_cast<std::byte *>(scratch) + 0x1000, bytes);
                HLib::FreeTaskData(scratch);
                HLib::FreeTaskData(workspace);
                HLib::FreeTaskData(compressed);
                slot->data = output;
            }
            pakReadData = slot->data;
            pakReadAllocationSize = static_cast<std::int32_t>(HLib::GetTaskDataSize(pakReadData));
            pakReadHeaderCount = static_cast<std::int32_t>(reinterpret_cast<const PakFileHeader *>(slot->data)->reserved08);
            switch (reinterpret_cast<const PakFileHeader *>(slot->data)->name) {
            case PACKAGE_PKF_HEADER:
                task->pakt = slot;
                break;
            case PACKAGE_PKS_HEADER:
                task->next = slot;
                break;
            }
        }
    }
    if (task->pakt) {
        slot = task->pakt;
        if (LoadPakTextures(task, slot)) {
            task->pakt = 0;
            task->next = slot;
        }
    }
    if (!task->next)
        return;
    slot = task->next;
    if (!LoadPakResources(task, slot))
        return;
    switch (reinterpret_cast<const PakFileHeader *>(slot->data)->name) {
    case PACKAGE_PKF_HEADER:
        slot->flags |= PakPksPhase;
        if (task->data) {
            bytes = (task->dataSize + 31U) & ~31U;
            if (slot->flags & PakAlternateHeap)
                temporary = HLib::AllocateTaskData(bytes, PACKAGE_PKS_HEADER);
            else
                temporary = HLib::AllocateStaticOrDynamicBlock(bytes, PACKAGE_PKS_HEADER);
            if (!temporary) {
                task->next = 0;
                task->pakBuffer = 0;
                return;
            }
            std::memcpy(temporary, task->data, bytes);
        }
        ClosePakRead(slot, 1);
        if (task->data) {
            if (slot->flags & PakAlternateHeap)
                task->data = HLib::AllocateStaticOrDynamicBlock(bytes, PACKAGE_CHARACTER_DATA);
            else
                task->data = HLib::AllocateTaskData(bytes, PACKAGE_CHARACTER_DATA);
            if (!task->data) {
                task->next = 0;
                task->pakBuffer = 0;
                return;
            }
            CopyEventOverlayAligned(task->data, temporary, bytes);
            HLib::FreeTaskData(temporary);
        }
        if (slot->textureHandles && !(slot->flags & PakKeepRetained)) {
            std::uint32_t textureBytes = HLib::GetTaskDataSize(slot->textureHandles) + 31U;
            textureBytes &= ~31U;
            void *const textures = slot->flags & PakAlternateHeap ? HLib::AllocateStaticOrDynamicBlock(textureBytes, PACKAGE_TEXTURE_HANDLES) :
                                                                    HLib::AllocateTaskData(textureBytes, PACKAGE_TEXTURE_HANDLES);
            CopyEventOverlayAligned(textures, slot->textureHandles, textureBytes);
            HLib::FreeTaskData(slot->textureHandles);
            slot->textureHandles = static_cast<std::int32_t *>(textures);
        }
        if (!(task->loadFlags & 2U)) {
            if (slot->flags & PakAfsRead)
                OpenPakAfsRead(task, slot, slot->secondFile);
            else
                OpenPakLooseRead(task, slot, task->path, BuildPakFilename(slot->entryType, "PKS"));
            task->pakBuffer = slot;
        }
        break;
    case PACKAGE_PKS_HEADER:
        ClosePakRead(slot, 0);
        if (task->data) {
            LoadPackageCharacters(task->data, SHEN_NAME4('P', 'A', 'K', 0) | static_cast<shen_name>('a' + GetPakSlotIndex(slot)) << 24);
            HLib::FreeTaskData(task->data);
            task->data = 0;
        }
        if (slot->flags & PakKeepRetained) {
            if (slot->data)
                HLib::SetTaskDataMotionFlag(slot->data);
            if (slot->textureHandles)
                HLib::SetTaskDataMotionFlag(slot->textureHandles);
        }
        task->linkedPakEntry = slot;
        break;
    }
    task->next = 0;
}

// @verified - d
PakGlobalData *LoadMapPackage(const char *directory, const char *name, std::uintptr_t allocation) {
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (work->pakBuffer || work->pakt || work->next)
        return 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index) {
        if (pakSlots[index].name && !CompareCharacterTableTokens(name, pakSlots[index].entryType))
            return &pakSlots[index];
    }
    PakGlobalData *const slot = AllocatePakSlot(name, 0, 0, 0, allocation);
    if (!slot)
        return 0;
    work->pakBuffer = slot;
    std::strcpy(work->path, directory);
    if (std::strlen(work->path) > 1 && work->path[std::strlen(work->path) - 1] == '/')
        work->path[std::strlen(work->path) - 1] = '\0';
    if (!(work->loadFlags & 4U)) {
        OpenPakLooseRead(work, slot, work->path, BuildPakFilename(name, "PKF"));
    } else {
        slot->flags |= PakPksPhase;
        OpenPakLooseRead(work, slot, work->path, BuildPakFilename(name, "PKS"));
    }
    HLib::ResolveDeferredCallback(pakLoadTask);
    return slot;
}

// @verified - d
PakGlobalData *ReadPakFileFromDirectory(const char *directory, const char *name, std::int32_t flags) {
    PakLoadTaskData *work;
    PakGlobalData *slot;
    if (pakLoadTaskData)
        work = pakLoadTaskData;
    else
        work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    work->loadFlags = 0;
    if (flags & 2U)
        work->loadFlags = 2;
    else if (flags & 4U)
        work->loadFlags = 4;
    slot = LoadMapPackage(directory, name, 0);
    if (flags & 1U)
        slot->flags |= PakKeepRetained;
    return slot;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakGlobalData *RequestMapPackage(const char *directory, const char *name, std::uintptr_t allocation) {
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    work->loadFlags = 0;
    return LoadMapPackage(directory, name, allocation);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakGlobalData *RequestMapPackage(const char *directory, const char *name) {
    return RequestMapPackage(directory, name, 0);
}
#endif

// @verified - d
std::int32_t FreePakSlot(PakGlobalData *slot) {
    if (!IsPakSlotIdle(slot))
        return 0;
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    ReleasePakTextures(slot);
    ReleasePakResources(slot);
    FreePakSlotStorage(slot);
    if (slot == work->linkedPakEntry)
        work->linkedPakEntry = 0;
    if (slot == work->selectedPackage)
        work->selectedPackage = 0;
    return 1;
}

void FreePakLoadTask() {
    if (!pakLoadTask)
        return;
    pakLoadFlush();
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index) {
        if (pakSlots[index].name && static_cast<std::int8_t>(static_cast<std::int8_t>(pakSlots[index].flags << 3) >> 7) == 0)
            FreePakSlot(&pakSlots[index]);
    }
    for (std::int32_t index = 0; index < 4; ++index) {
        if (pakPartitions[index].afs) {
            while (UnmountPakAfsPartition(&pakPartitions[index])) {
            }
        }
    }
    HLib::DestroyTaskInternal(pakLoadTask);
    pakLoadTask = 0;
    if (pakLoadTaskData)
        SetTaskHeapData(pakLoadTaskData);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t CancelPakSlot(PakGlobalData *package) {
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    std::int32_t stage;
    if (work->next == package)
        stage = package->flags & PakPksPhase ? 5 : 3;
    else if (work->pakt == package)
        stage = package->flags & PakPksPhase ? 10 : 2;
    else if (work->pakBuffer == package)
        stage = package->flags & PakPksPhase ? 4 : 1;
    else
        return FreePakSlot(package);
    std::int32_t releaseData = 1;
    if (stage == 5) {
        ReleasePakResources(package);
        FreePakSlotStorage(package);
        releaseData = 0;
        stage = 2;
    }
    if (stage == 4)
        stage = 2;
    if (stage == 3)
        stage = 2;
    if (stage == 2) {
        ReleasePakTextures(package);
        stage = 1;
    }
    if (stage == 1)
        ClosePakRead(package, releaseData);
    if (work->data) {
        HLib::FreeTaskData(work->data);
        work->data = 0;
    }
    package->name = __typeof__(package->name)();
    work->pakBuffer = 0;
    work->pakt = 0;
    work->next = 0;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::intptr_t EV_pakUtils(std::int32_t category, std::uint32_t mode, std::uintptr_t source, std::uintptr_t target, std::int32_t secondaryIndex) {
    PakGlobalData *package;
    switch (category) {
    case PakCategory::Package:
        switch (mode) {
        case PakPackageOp::Load:
            package = FindPakByEntryName(reinterpret_cast<const char *>(target));
            if (package)
                return 0;
            if (RequestMapPackage(reinterpret_cast<const char *>(source), reinterpret_cast<const char *>(target)))
                return 1;
            return 0;
        case PakPackageOp::IsLoadPending:
            if (PakLoadWait())
                return 1;
            return 0;
        case PakPackageOp::IsLoaded:
            if (!FindPakByEntryName(reinterpret_cast<const char *>(source)))
                return 0;
            return 1;
        case PakPackageOp::Free:
            package = FindPakByEntryName(reinterpret_cast<const char *>(source));
            FreePakSlot(package);
            break;
        case PakPackageOp::FinishLoading:
            pakLoadFlush();
            break;
        case PakPackageOp::LoadAfs:
            package = FindPakInAfs(reinterpret_cast<PakAfsPartition *>(source), static_cast<std::int32_t>(target), secondaryIndex);
            if (package || GetPakLoadBuffer())
                return 0;
            RequestPakFromAfs(reinterpret_cast<PakAfsPartition *>(source), static_cast<std::int32_t>(target), secondaryIndex);
            break;
        case PakPackageOp::GetMapHandles:
            package = FindPakByEntryName(reinterpret_cast<const char *>(source));
            return reinterpret_cast<std::intptr_t>(GetReadyPakMapHandles(package));
        case PakPackageOp::SetLoadBudget:
            SetupPakSpace(static_cast<std::int32_t>(source));
            break;
        case PakPackageOp::GetLoadBudget:
            return GetPakLoadBudget();
        case PakPackageOp::FreeAfs:
            package = FindPakInAfs(reinterpret_cast<PakAfsPartition *>(source), static_cast<std::int32_t>(target), secondaryIndex);
            FreePakSlot(package);
            break;
        case PakPackageOp::DestroyAfs:
            package = FindPakInAfs(reinterpret_cast<PakAfsPartition *>(source), static_cast<std::int32_t>(target), secondaryIndex);
            if (package)
                CancelPakSlot(package);
            break;
        }
        break;
    case PakCategory::Partition:
        switch (mode) {
        case PakPartitionOp::Mount:
            return reinterpret_cast<std::intptr_t>(MountPakAfs(reinterpret_cast<const char *>(source), reinterpret_cast<const char *>(target)));
        case PakPartitionOp::Unmount:
            return UnmountPakAfsPartition(reinterpret_cast<PakAfsPartition *>(source));
        }
        break;
    case PakCategory::Task:
        switch (mode) {
        case PakTaskOp::GetFlags:
            return static_cast<std::int32_t>(GetPakLoadFlags());
        case PakTaskOp::SetFlags:
            SetPakLoadFlags(static_cast<std::uint32_t>(source));
            break;
        case PakTaskOp::LoadNamed:
            package = FindPakByEntryName(reinterpret_cast<const char *>(source));
            pakLoad(package);
            break;
        case PakTaskOp::IsCurrentNamed:
            package = FindPakByEntryName(reinterpret_cast<const char *>(source));
            if (!package)
                return 0;
            if (GetPakLoadTaskDataPointer() != package)
                return 0;
            break;
        case PakTaskOp::SetCurrentNamed:
            package = FindPakByEntryName(reinterpret_cast<const char *>(source));
            SetNewPakLoadedBuffer(package);
            break;
        }
        break;
    case PakCategory::Reserved:
        break;
    case PakCategory::LoadPackageWithMode:
        package = FindPakByEntryName(reinterpret_cast<const char *>(target));
        if (package)
            return 0;
        if (!RequestMapPackage(reinterpret_cast<const char *>(source), reinterpret_cast<const char *>(target), mode))
            return 0;
        break;
    case PakCategory::LoadAfsWithMode:
        package = FindPakInAfs(reinterpret_cast<PakAfsPartition *>(source), static_cast<std::int32_t>(target), secondaryIndex);
        if (package || GetPakLoadBuffer())
            return 0;
        RequestPakFromAfs(reinterpret_cast<PakAfsPartition *>(source), static_cast<std::int32_t>(target), secondaryIndex, mode);
        break;
    }
    return 1;
}
#endif

// @verified - d
void pakLoadFlush() {
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    while (work->pakBuffer || work->pakt || work->next) {
        ProcessDiscFileReads();
        PakLoadCallback(work);
    }
}

PakGlobalData *RequestPakFromAfsWithAllocation(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex) {
    return RequestPakFromAfs(partition, firstIndex, secondIndex, 1);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex, std::uintptr_t allocation) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(partition);
    if (address < reinterpret_cast<std::uintptr_t>(pakPartitions) || address > reinterpret_cast<std::uintptr_t>(pakPartitions + 3) || !partition->afs)
        return 0;
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (work->pakBuffer || work->pakt || work->next)
        return 0;
    // GameJam scans with this unwritten local before formatting the new package name.
    char name[24];
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index) {
        if (pakSlots[index].name && CompareCharacterTableTokens(name, pakSlots[index].entryType) == 0)
            return &pakSlots[index];
    }
    BuildAfsPakName(partition->filename, firstIndex, secondIndex, name);
    do {
        PakGlobalData *const slot = AllocatePakSlot(name, partition, firstIndex, secondIndex, allocation);
        if (!slot)
            return 0;
        work->pakBuffer = slot;
        OpenPakAfsRead(work, slot, firstIndex);
        HLib::ResolveDeferredCallback(pakLoadTask);
        return slot;
    } while (0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex) {
    return RequestPakFromAfs(partition, firstIndex, secondIndex, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void pakLoad(PakGlobalData *package) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(package);
    if (address < reinterpret_cast<std::uintptr_t>(pakSlots) || address > reinterpret_cast<std::uintptr_t>(pakSlots + kPakSlotCount - 1))
        return;
    const std::int32_t index = GetPakSlotIndex(package);
    if (package->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + index) << 24)) || !package->data)
        return;
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    work->linkedPakEntry = package;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetNewPakLoadedBuffer(PakGlobalData *package) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(package);
    if (address < reinterpret_cast<std::uintptr_t>(pakSlots) || address > reinterpret_cast<std::uintptr_t>(pakSlots + kPakSlotCount - 1))
        return;
    const std::int32_t index = GetPakSlotIndex(package);
    if (package->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + index) << 24)) || !package->data)
        return;
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    work->selectedPackage = package;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakAfsPartition *MountPakAfs(const char *directory, const char *filename) {
    std::int32_t index;
    for (index = 0; index < 4; ++index)
        if (pakPartitions[index].afs && ComparePakNames(pakPartitions[index].folder, directory) == 0 &&
            ComparePakNames(pakPartitions[index].filename, filename) == 0)
            break;
    if (index > 3) {
        for (index = 0; index < 4 && pakPartitions[index].afs; ++index)
            ;
        if (index > 3)
            return 0;
        pakPartitions[index].afs = MountAudioPartitionDefault(directory, filename, 0x800);
        if (!pakPartitions[index].afs)
            return 0;
        std::strcpy(pakPartitions[index].folder, directory);
        std::strcpy(pakPartitions[index].filename, filename);
        pakPartitions[index].references = 1;
    } else
        pakPartitions[index].references++;
    return &pakPartitions[index];
}
#endif

// @verified - d
std::int32_t IsPakSlotIdle(const PakGlobalData *package) {
    const PakGlobalData *const slot = package;
    if (slot < pakSlots || slot > &pakSlots[kPakSlotCount - 1])
        return 0;
    const std::int32_t index = GetPakSlotIndex(package);
    if (slot->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + index) << 24)))
        return 0;
    if (!slot)
        return 0;
    std::int32_t slotIndex;
    PakLoadTaskData *task = pakLoadTaskData;
    if (!task)
        task = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    const PakLoadTaskData *work = task;
    do {
        for (slotIndex = 0; slotIndex < static_cast<std::int32_t>(kPakSlotCount); ++slotIndex) {
            if (&pakSlots[slotIndex] == slot && slot->name && slot != work->pakBuffer && slot != work->pakt && slot != work->next)
                return 1;
        }
    } while (0);
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetPakReadiness(PakGlobalData *package) {
    PakGlobalData *const pak = package;
    if (pak < pakSlots || pak > &pakSlots[kPakSlotCount - 1])
        return -1;
    if (pak->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)))
        return -1;
    if (!pak)
        return -1;
    std::int32_t slot;
    PakLoadTaskData *work = pakLoadTaskData;
    if (!work)
        work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    for (slot = 0; slot < static_cast<std::int32_t>(kPakSlotCount); ++slot) {
        if (pakSlots + slot != pak)
            continue;
        if (pak->name && pak != work->pakBuffer && pak != work->pakt && pak != work->next)
            return 1;
        if (pak->state)
            return pak->state;
        return 0;
    }
    return -1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakGlobalData *FindPakByEntryName(const char *name) {
    if (!pakLoadTaskData)
        HLib::GetTaskParameter(pakLoadTask);
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(pakSlots) / sizeof(pakSlots[0])); ++index) {
        if (pakSlots[index].name && ComparePakNames(pakSlots[index].entryType, name) == 0)
            return &pakSlots[index];
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakGlobalData *FindPakInAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int32_t secondIndex) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(partition);
    if (address < reinterpret_cast<std::uintptr_t>(pakPartitions) || address > reinterpret_cast<std::uintptr_t>(pakPartitions + 3))
        return 0;
    if (!pakLoadTaskData)
        HLib::GetTaskParameter(pakLoadTask);
    char name[24];
    BuildAfsPakName(partition->filename, firstIndex, secondIndex, name);
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index) {
        if (pakSlots[index].name && ComparePakNames(pakSlots[index].entryType, name) == 0)
            return &pakSlots[index];
    }
    return 0;
}
#endif

const char *GetIdlePakEntryName(PakGlobalData *package) {
    PakGlobalData *const slot = package;
    if (slot < pakSlots || slot > pakSlots + kPakSlotCount - 1)
        return 0;
    if (slot->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)))
        return 0;
    if (!slot->data)
        return 0;
    if (!pakLoadTaskData)
        HLib::GetTaskParameter(pakLoadTask);
    if (!IsPakSlotIdle(slot))
        return 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kPakSlotCount); ++index)
        if (&pakSlots[index] == slot)
            return pakSlots[index].entryType;
    return 0;
}

// @verified - d
PakGlobalData *GetNextReadyModelPackage(PakGlobalData *previous) {
    PakGlobalData *slot = previous;
    PakGlobalData *end = pakSlots + kPakSlotCount;
    if (!slot)
        slot = pakSlots;
    else
        ++slot;
    for (; slot < end; ++slot) {
        if (!slot->name || !slot->data)
            continue;
        const PakLoadTaskData *work;
        do {
            if (pakLoadTaskData)
                work = pakLoadTaskData;
            else
                work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
        } while (0);
        if (slot != work->pakBuffer && slot != work->pakt && slot != work->next)
            return slot;
    }
    return 0;
}

// @verified - d
PakGlobalData *FindRetainingModelPackage(const MT7Model *model) {
    for (std::int32_t slot = 0; slot < static_cast<std::int32_t>(sizeof(pakSlots) / sizeof(pakSlots[0])); ++slot) {
        PakGlobalData &package = pakSlots[slot];

        if (!package.name || &package == GetPakLoadBuffer() || !package.modelHandles)
            continue;
        for (std::int32_t index = 0; index < package.modelIndex; ++index)
            if (package.modelHandles[index] == model)
                return &package;
    }
    return 0;
}

shen_name GetPakAllocationName(PakGlobalData *package) {
    PakGlobalData *const slot = package;
    if (slot < pakSlots || slot > pakSlots + kPakSlotCount - 1)
        return 0;
    if (slot->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)))
        return 0;
    if (!slot->data)
        return 0;
    if (!(slot->flags & 2))
        return 0;
    const shen_name letter = 'a' + GetPakSlotIndex(slot);
    return SHEN_NAME4('P', 'A', 'K', 0) | letter << 24;
}

// @verified - d
PakGlobalData *GetPakLoadTaskDataPointer() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->linkedPakEntry;
}

// @verified - d
void *FileLoader(PakGlobalData *package, const char *filename, shen_name extension) {
    PakGlobalData *const pak = package;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pak);
    if (address < reinterpret_cast<std::uintptr_t>(pakSlots) || address > reinterpret_cast<std::uintptr_t>(pakSlots + kPakSlotCount - 1))
        return 0;
    if (package->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(pak)) << 24)) || !package->data)
        return 0;
    if (reinterpret_cast<const std::uint32_t *>(package->data)[0] != PACKAGE_PKS_HEADER)
        return 0;
    PakDirectory *const directory =
        reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(package->data) + reinterpret_cast<const std::uint32_t *>(package->data)[1]);
    const std::int32_t index = PACK_FindNamedEntry(directory, filename);
    if (index != -1) {
        shen_name tag;
        tag = reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
                  ->tag;
        if (tag == extension)
            goto found;
        if (extension == PACKAGE_ANY_EXTENSION)
            goto found;
    }
    return 0;
found:
    return reinterpret_cast<std::byte *>(directory) +
        reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
            ->dataOffset;
}

// @verified - d
std::uint32_t GetPakFileSize(PakGlobalData *package, const char *filename, shen_name extension) {
    PakGlobalData *const pak = package;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pak);
    if (address < reinterpret_cast<std::uintptr_t>(pakSlots) || address > reinterpret_cast<std::uintptr_t>(pakSlots + kPakSlotCount - 1))
        return 0;
    if (package->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(pak)) << 24)) || !package->data)
        return 0;
    const std::uint32_t *const header = reinterpret_cast<const std::uint32_t *>(package->data);
    PakDirectory *const directory = reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(package->data) + header[1]);
    const std::int32_t index = PACK_FindNamedEntry(directory, filename);
    if (index >= 0) {
        shen_name tag;
        tag = reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
                  ->tag;
        if (tag == extension)
            goto found;
        if (extension == PACKAGE_ANY_EXTENSION)
            goto found;
    }
    return 0;
found:
    return reinterpret_cast<const PakDirectoryEntry *>(
        reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
        ->auxiliary;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetPakEntryNameByType(PakGlobalData *package, shen_name type, char *name) {
    PakGlobalData *const pak = package;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(pak);
    if (address < reinterpret_cast<std::uintptr_t>(pakSlots) || address > reinterpret_cast<std::uintptr_t>(pakSlots + kPakSlotCount - 1))
        return 0;
    if (package->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(pak)) << 24)) || !package->data)
        return 0;
    const std::uint32_t *const header = reinterpret_cast<const std::uint32_t *>(package->data);
    PakDirectory *const directory = reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(package->data) + header[1]);
    const std::int32_t index = FindPakEntryByType(directory, type);
    if (index == -1)
        return 0;
    std::strcpy(name,
        GetPakEntryName(
            reinterpret_cast<PakDirectoryEntry *>(index * sizeof(PakDirectoryEntry) + directory->entriesOffset + reinterpret_cast<std::uintptr_t>(directory))));
    return 1;
}
#endif

// @verified - d
PakGlobalData *GetPakLoadBuffer() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->pakBuffer;
}

const std::byte *GetIdlePakSoundTable(PakGlobalData *slot) {
    return IsPakSlotIdle(slot) ? slot->soundTable : 0;
}

// @verified - d
MT7Model *LoadGameModel(PakGlobalData *package, const char *filename) {
    PakGlobalData *const pak = package;
    if (pak < pakSlots || pak > pakSlots + kPakSlotCount - 1)
        return 0;
    if (pak->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)) || !pak->data)
        return 0;
    if (!IsPakSlotIdle(pak) && pakLoadGetNext() != pak)
        return 0;
    PakDirectory *const directory =
        reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(pak->data) + reinterpret_cast<const std::uint32_t *>(pak->data)[1]);
    const std::int32_t index = PACK_FindNamedEntry(directory, filename);
    if (index == -1)
        return 0;
    const std::byte *const source = reinterpret_cast<const std::byte *>(directory) +
        reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
            ->dataOffset;
    if (reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
                ->tag != SHEN_NAME4('C', 'H', 'R', 'M') &&
        reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
                ->tag != SHEN_NAME4('M', 'A', 'P', 'M') &&
        reinterpret_cast<const PakDirectoryEntry *>(
            reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
                ->tag != SHEN_NAME4('M', 'A', 'P', 'D'))
        return 0;
    for (std::int32_t modelIndex = 0; pak->modelHandles && modelIndex < pak->modelIndex; ++modelIndex) {
        MT7Model *const model = static_cast<MT7Model *>(pak->modelHandles[modelIndex]);
        if (model->source == source)
            return model;
    }
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
PakGlobalData *FindPakContainingEntryData(const void *data) {
    PakLoadTaskData *work;
    if (pakLoadTaskData)
        work = pakLoadTaskData;
    else
        work = static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    for (std::int32_t slot = 0; slot < static_cast<std::int32_t>(kPakSlotCount); ++slot) {
        PakGlobalData *const package = &pakSlots[slot];
        if (!package->name || package == GetPakLoadBuffer())
            continue;
        const std::uint32_t *const header = reinterpret_cast<const std::uint32_t *>(package->data);
        PakDirectory *const directory = reinterpret_cast<PakDirectory *>(static_cast<std::byte *>(package->data) + header[1]);
        for (std::int32_t index = 0; index < directory->entryCount; ++index)
            if (data ==
                reinterpret_cast<std::byte *>(directory) +
                    reinterpret_cast<const PakDirectoryEntry *>(
                        reinterpret_cast<const std::byte *>(directory) + (index * sizeof(PakDirectoryEntry) + directory->entriesOffset))
                        ->dataOffset)
                return package;
    }
    return 0;
}
#endif

// @verified - d
PakGlobalData *pakLoadGetNext() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->next;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PakGlobalData *GetSelectedPakPackage() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->selectedPackage;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t *FindPakEventCamera(PakGlobalData *package, std::int32_t index) {
    PakGlobalData *const pak = package;
    if (pak < pakSlots || pak > pakSlots + kPakSlotCount - 1)
        return 0;
    if (pak->name != (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)) || !pak->data)
        return 0;
    MapInfoChunk *chunk = static_cast<MapInfoChunk *>(FileLoader(pak, "CAMERA", shen_name(SHEN_NAME4('E', 'C', 'A', 'M'))));
    if (!chunk)
        return 0;
    char name[8];
    std::sprintf(name, "%04d", index);
    ++chunk;
    chunk = FindNativeChunk(name, chunk);
    if (chunk)
        ++chunk;
    return reinterpret_cast<std::uint32_t *>(chunk);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t *pakFindEventCam(std::int32_t index) {
    PakGlobalData *package = GetSelectedPakPackage();
    if (!package)
        package = GetPakLoadTaskDataPointer();
    MapInfoChunk *chunk;
    if (package) {
        chunk = reinterpret_cast<MapInfoChunk *>(FindPakEventCamera(package, index));
        if (chunk)
            return reinterpret_cast<std::uint32_t *>(chunk);
    }
    chunk = GetCurrentEventCameraChunk();
    if (!chunk)
        return 0;
    ++chunk;
    char name[8];
    std::sprintf(name, "%04d", index);
    chunk = FindNativeChunk(name, chunk);
    if (chunk)
        ++chunk;
    return reinterpret_cast<std::uint32_t *>(chunk);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const std::byte *FindPackageDynamicMap(const std::uint8_t name[4]) {
    PakGlobalData *package = 0;
    shen_name wanted = name[0] | (name[1] << 8) | (name[2] << 16) | (name[3] << 24);
    for (;;) {
        package = GetNextReadyModelPackage(package);
        if (!package)
            return 0;
        if (!package->dynamicMapData)
            continue;
        const MapInfoChunk *const first = reinterpret_cast<const MapInfoChunk *>(package->dynamicMapData);
        const std::byte *const end = reinterpret_cast<const std::byte *>(first) + first->nextByteOffset;
        for (const MapInfoChunk *chunk = first + 1; reinterpret_cast<const std::byte *>(chunk) < end;
            chunk = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset)) {
            if (chunk->name == wanted)
                return reinterpret_cast<const std::byte *>(chunk);
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t PakLoadWait() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (work->pakBuffer || work->pakt || work->next)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t *GetReadyPakMapHandles(PakGlobalData *package) {
    return IsPakSlotIdle(package) ? package->mapHandles : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetupPakSpace(std::int32_t budget) {
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (!budget)
        budget = 3000;
    work->timeBudget = budget;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetPakLoadBudget() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->timeBudget;
}
#endif

// @verified - d
std::int32_t GetPakLoadParams() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->unknown04;
}

void *GetPakData(PakGlobalData *package) {
    const PakGlobalData *const slot = package;
    if (slot < pakSlots || slot > pakSlots + kPakSlotCount - 1)
        return 0;
    if (slot->name == (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)))
        return slot->data;
    return 0;
}

std::int32_t *GetPakTextureHandles(PakGlobalData *package) {
    const PakGlobalData *const slot = package;
    if (slot < pakSlots || slot > pakSlots + kPakSlotCount - 1)
        return 0;
    if (slot->name == (SHEN_NAME4('H', 'P', 'M', 0) | (static_cast<std::uint32_t>('a' + GetPakSlotIndex(package)) << 24)))
        return slot->textureHandles;
    return 0;
}

// @verified - d
std::uint32_t GetPakLoadFlags() {
    const PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return work->flags;
}

// @verified - d
void SetPakLoadFlags(std::uint32_t flags) {
    PakLoadTaskData *const work = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    work->flags = flags;
}

void PakLoadNullsub0() {
}

void PakLoadNullsub1() {
}

} // namespace s2
} // namespace shenmue
#endif
