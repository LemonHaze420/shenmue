#include "s2/pak.hpp"
#include "s2/afs.hpp"

#include "s2/archive_data.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_image.hpp"
#include "s2/mt7.hpp"
#include "s2/motion.hpp"
#include "s2/search.hpp"
#include "s2/scene_data.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/ui.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/effect_texture.hpp"

#include "d3t/archive_handles.hpp"
#include "d3t/content.hpp"
#include "d3t/file_state.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/object_store.hpp"
#include "d3t/pvr_decoder.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <limits>
#include <new>
#include <thread>
#include <utility>

#include <zlib.h>

namespace shenmue::s2 {
struct PakGlobalData {
    shen_name name;
    std::uint32_t padding04;
    void *data;
    std::uint64_t dataSize;
    std::array<char, 0x18> entryType;
    void *field30;
    std::int32_t *textureHandles;
    const std::byte *textureCursor;
    const std::byte *soundTable;
    std::uint32_t *motionHandles;
    std::uint32_t *propHandles;
    std::uint64_t motionProperty;
    const void *dynamicMapData;
    void **modelHandles;
    std::int32_t *mapHandles;
    void *status;

    union {
        d3t::FileState *fileLoad;
        d3t::LargeFileHandle *afsFile;
    };
    std::int16_t field90;
    std::int16_t field92;
    std::int16_t chunkIndex;
    std::int16_t textureIndex;
    std::int16_t motionIndex;
    std::int16_t modelIndex;
    std::int16_t propIndex;
    std::int16_t reserved9e;
    std::uint32_t flags;
    std::int32_t state;
};

static_assert(sizeof(PakGlobalData) == 0xa8);
static_assert(offsetof(PakGlobalData, data) == 0x08);
static_assert(offsetof(PakGlobalData, entryType) == 0x18);
static_assert(offsetof(PakGlobalData, textureHandles) == 0x38);
static_assert(offsetof(PakGlobalData, textureCursor) == 0x40);
static_assert(offsetof(PakGlobalData, soundTable) == 0x48);
static_assert(offsetof(PakGlobalData, motionHandles) == 0x50);
static_assert(offsetof(PakGlobalData, propHandles) == 0x58);
static_assert(offsetof(PakGlobalData, modelHandles) == 0x70);
static_assert(offsetof(PakGlobalData, fileLoad) == 0x88);
static_assert(offsetof(PakGlobalData, chunkIndex) == 0x94);
static_assert(offsetof(PakGlobalData, motionIndex) == 0x98);
static_assert(offsetof(PakGlobalData, propIndex) == 0x9c);
static_assert(offsetof(PakGlobalData, flags) == 0xa0);

namespace {

constexpr std::uint32_t pakS = SHEN_NAME(PAKS);
constexpr std::uint32_t pakF = SHEN_NAME(PAKF);
constexpr std::uint32_t ipac = SHEN_NAME(IPAC);
constexpr std::uint32_t texN = SHEN_NAME(TEXN);
constexpr std::uint32_t dumY = SHEN_NAME(DUMY);
constexpr std::uint32_t gbix = SHEN_NAME(GBIX);
constexpr std::uint32_t pvrt = SHEN_NAME(PVRT);
constexpr std::uint32_t fldd = SHEN_NAME(FLDD);
constexpr std::uint32_t lght = SHEN_NAME(LGHT);
constexpr std::uint32_t lock = SHEN_NAME(LOCK);
constexpr std::uint32_t motp = SHEN_NAME(MOTP);
constexpr std::uint32_t srch = SHEN_NAME(SRCH);
constexpr std::uint32_t auth = SHEN_NAME(AUTH);
constexpr std::uint32_t chrt = SHEN_NAME(CHRT);
constexpr std::uint32_t ev1 = make_shen_name('E', 'V', '1', ' ');
constexpr std::uint32_t ev2 = make_shen_name('E', 'V', '2', ' ');
constexpr std::uint32_t ev3 = make_shen_name('E', 'V', '3', ' ');
constexpr std::size_t nativePakSlotCount = 52;

enum class PakPhase : std::uint8_t {
    Empty,
    ReadingPakf,
    ParsingPakf,
    ReadingPaks,
    ParsingPaks,
    Ready,
    Failed,
};

struct PakRuntime {
    PakPhase phase{};
#if SHENMUE_IS_DC
    void *destination{};
#endif
    std::array<char, 0x80> folder{};
    bool memoryPackage{};
    std::size_t soundTableSize{};
    std::span<const std::byte> characterTable;
    std::uint32_t modelCapacity{};
};

HLib::Task *pakLoadTask = nullptr;
PakLoadTaskData *pakLoadTaskData = nullptr;


std::array<char, 0x18> pakReadName{};
std::uint64_t pakReadSize{};
void *pakReadBuffer{};
std::int32_t pakReadHeader{};
std::uint64_t pakReadAllocationSize{};
std::int64_t pakReadResult{};
PakLoadTaskData *detachedPakLoadTaskData = nullptr;
std::array<PakGlobalData, nativePakSlotCount> pakSlots{};
std::array<PakRuntime, nativePakSlotCount> pakRuntimes{};
std::array<LoadedMapPackage, nativePakSlotCount> loadedMapPackages{};
std::size_t loadedMapPackageCount = 0;
#if !SHENMUE_IS_DC
std::array<PakAfsPartition, 4> pakAfsPartitions{};
#endif

constexpr std::size_t namedTextureSlotCount = 752;
std::array<NamedTextureSlot, namedTextureSlotCount> namedTextureSlots{};
std::array<std::uint32_t, namedTextureSlotCount> namedTextureHandles{};
std::array<bool, namedTextureSlotCount> namedTextureEffectOwners{};
NamedTextureSlot *namedTextureFree = nullptr;
std::array<NamedTextureSlot *, 2> namedTextureBanks{};
std::int32_t namedTextureBank = 0;
bool namedTexturesInitialized = false;

std::uint32_t ReadU32(const std::byte *data) {
    std::uint32_t value{};
    std::memcpy(&value, data, sizeof(value));
    return value;
}

// PKF TEXN chunks carry their GBIX/PVRT image at +0x1c; MT7 TXT7 entries begin with it.
const d3t::PvrHeader *FindPvrHeader(std::span<const std::byte> texn, std::size_t imageOffset) {
    if (texn.size() < imageOffset + sizeof(d3t::PvrHeader)) {
        return nullptr;
    }
    const std::byte *cursor = texn.data() + imageOffset;
    std::size_t remaining = texn.size() - imageOffset;
    if (ReadU32(cursor) == gbix) {
        if (remaining < 8)
            return nullptr;
        const std::uint32_t size = ReadU32(cursor + 4);
        const std::uint64_t advance = static_cast<std::uint64_t>(size) + 8;
        if (advance > remaining)
            return nullptr;
        cursor += advance;
        remaining -= static_cast<std::size_t>(advance);
    }
    if (remaining < sizeof(d3t::PvrHeader) || ReadU32(cursor) != pvrt) {
        return nullptr;
    }
    return reinterpret_cast<const d3t::PvrHeader *>(cursor);
}

void InitializeNamedTextureSlots() {
    for (std::size_t index = 0; index < namedTextureSlots.size(); ++index) {
        namedTextureSlots[index] = {.index = static_cast<std::int16_t>(index),
            .references = 0,
            .idLow = 0,
            .idHigh = 0,
            .reserved = 0,
            .next = index + 1 < namedTextureSlots.size() ? &namedTextureSlots[index + 1] : nullptr};
    }
    namedTextureFree = namedTextureSlots.data();
    namedTexturesInitialized = true;
}

NamedTextureSlot *FindNamedTexture(std::uint32_t idLow, std::uint32_t idHigh) {
    for (NamedTextureSlot *slot = namedTextureBanks[namedTextureBank != 0]; slot; slot = slot->next) {
        if (slot->idLow == idLow && slot->idHigh == idHigh)
            return slot;
    }
    return nullptr;
}

NamedTextureSlot *FindOrAllocateNamedTexture(std::uint32_t idLow, std::uint32_t idHigh, bool &created) {
    if (!namedTexturesInitialized)
        InitializeNamedTextureSlots();
    created = false;
    NamedTextureSlot *slot = FindNamedTexture(idLow, idHigh);
    if (slot) {
        ++slot->references;
        return slot;
    }
    slot = namedTextureFree;
    if (!slot)
        return nullptr;
    namedTextureFree = slot->next;
    NamedTextureSlot *&head = namedTextureBanks[namedTextureBank != 0];
    slot->next = head;
    head = slot;
    slot->idLow = idLow;
    slot->idHigh = idHigh;
    slot->references = 1;
    created = true;
    return slot;
}

NamedTextureSlot *AcquireNamedTexture(std::uint32_t idLow, std::uint32_t idHigh, std::span<const std::byte> image, std::size_t imageOffset) {
    bool created{};
    NamedTextureSlot *const slot = FindOrAllocateNamedTexture(idLow, idHigh, created);
    if (!slot || !created)
        return slot;
    namedTextureHandles[slot->index] = UploadTexn(image, imageOffset);
    if (namedTextureHandles[slot->index])
        return slot;
    ReleaseNamedTexture(slot->index);
    return nullptr;
}

std::size_t PakSlotIndex(const PakGlobalData *slot) {
    if (!slot)
        return nativePakSlotCount;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(slot);
    const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(pakSlots.data());
    const std::uintptr_t end = base + sizeof(pakSlots);
    if (address < base || address >= end || (address - base) % sizeof(PakGlobalData) != 0) {
        return nativePakSlotCount;
    }
    return (address - base) / sizeof(PakGlobalData);
}

PakRuntime *RuntimeFor(PakGlobalData *slot) {
    const std::size_t index = PakSlotIndex(slot);
    return index < loadedMapPackageCount ? &pakRuntimes[index] : nullptr;
}

LoadedMapPackage *PackageFor(PakGlobalData *slot) {
    const std::size_t index = PakSlotIndex(slot);
    return index < loadedMapPackageCount ? &loadedMapPackages[index] : nullptr;
}

PakContainerView *ActiveContainer(PakGlobalData *slot) {
    PakRuntime *const runtime = RuntimeFor(slot);
    LoadedMapPackage *const package = PackageFor(slot);
    if (!runtime || !package)
        return nullptr;
    if (runtime->phase == PakPhase::ParsingPakf)
        return &package->pkf;
    if (runtime->phase == PakPhase::ParsingPaks)
        return &package->pks;
    return nullptr;
}

bool HasThreadHandle(std::uint32_t handle) {
    return !d3t::d3t_IsRenderHandlePending(handle);
}

void LockThreadHandle(std::uint32_t &handle) {
    d3t::d3t_CloseRenderHandle(handle);
}

// @verified - p
void PakLoadingDiagnosticNullsub() noexcept {
}



bool IsWithinCallbackBudget(const PakLoadTaskData &task, float started) {
    const float now = static_cast<float>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta));
    const std::int64_t elapsed = static_cast<std::int32_t>((started - now) / 12.5f);
    return elapsed <= task.timeBudget || task.timeBudget == -1;
}


bool IsWithinCallbackStageBudget(const PakLoadTaskData &task, std::uint32_t started) {
    const std::uint32_t now = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    const std::int64_t elapsed = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(started - now)) / 12.5f);
    return elapsed <= task.timeBudget || task.timeBudget == -1;
}

bool EqualsName(const PakContainerEntry &entry, const char *filename) {
    if (!filename) {
        return false;
    }
    std::array<char, 14> entryName{};
    std::size_t stemLength = entry.stem.size();
    while (stemLength && (entry.stem[stemLength - 1] == 0 || entry.stem[stemLength - 1] == ' '))
        --stemLength;
    std::size_t extensionLength = entry.extension.size();
    while (extensionLength && (entry.extension[extensionLength - 1] == 0 || entry.extension[extensionLength - 1] == ' '))
        --extensionLength;
    std::size_t length = 0;
    for (std::size_t index = 0; index < stemLength; ++index)
        entryName[length++] = entry.stem[index];
    if (extensionLength) {
        entryName[length++] = '.';
        for (std::size_t index = 0; index < extensionLength; ++index)
            entryName[length++] = entry.extension[index];
    }
    const std::size_t requestedLength = std::strlen(filename);
    if (requestedLength != length) {
        return false;
    }
    for (std::size_t index = 0; index < length; ++index) {
        const auto left = static_cast<unsigned char>(entryName[index]);
        const auto right = static_cast<unsigned char>(filename[index]);
        if (std::toupper(left) != std::toupper(right)) {
            return false;
        }
    }
    return true;
}
bool EqualsPackageName(const LoadedMapPackage &package, const char *requested) {
    if (!requested)
        return false;
    const std::size_t requestedLength = std::strlen(requested);
    const std::size_t loadedLength = std::strlen(package.name.data());
    if (requestedLength != loadedLength)
        return false;
    for (std::size_t index = 0; index < loadedLength; ++index) {
        const auto loaded = static_cast<unsigned char>(package.name[index]);
        const auto wanted = static_cast<unsigned char>(requested[index]);
        if (std::toupper(loaded) != std::toupper(wanted))
            return false;
    }
    return true;
}

bool ParseContainer(std::span<const std::byte> bytes, PakContainerView &result) {
    result = {};
    if (bytes.size() < 0x10) {
        return false;
    }
    const std::uint32_t signature = ReadU32(bytes.data());
    if (signature != pakS && signature != pakF) {
        return false;
    }
    const std::uint32_t ipacOffset = ReadU32(bytes.data() + 4);
    if (ipacOffset > bytes.size() || bytes.size() - ipacOffset < 0x10 || ReadU32(bytes.data() + ipacOffset) != ipac) {
        return false;
    }
    const std::uint32_t dictionaryRelative = ReadU32(bytes.data() + ipacOffset + 4);
    const std::uint32_t entryCount = ReadU32(bytes.data() + ipacOffset + 8);
    const std::uint64_t dictionaryOffset = static_cast<std::uint64_t>(ipacOffset) + dictionaryRelative;
    const std::uint64_t dictionarySize = static_cast<std::uint64_t>(entryCount) * sizeof(PakContainerEntry);
    if (dictionaryOffset > bytes.size() || dictionarySize > bytes.size() - dictionaryOffset) {
        return false;
    }
    const auto *const entries = reinterpret_cast<const PakContainerEntry *>(bytes.data() + dictionaryOffset);
    for (std::uint32_t index = 0; index < entryCount; ++index) {
        const std::uint64_t offset = static_cast<std::uint64_t>(ipacOffset) + entries[index].relativeOffset;
        if (offset > bytes.size() || entries[index].size > bytes.size() - offset) {
            return false;
        }
    }
    result.bytes = bytes;
    result.entries = {entries, entryCount};
    result.signature = signature;
    result.ipacOffset = ipacOffset;
    result.textureCount = signature == pakF ? ReadU32(bytes.data() + 0x0c) : 0;
    return true;
}

bool InflateGzip(std::span<const std::byte> source, void *&allocation, std::size_t &size, shen_name name, void *destination = nullptr) {
    allocation = nullptr;
    size = 0;
    if (source.size() < 18 || source[0] != std::byte{0x1f} || source[1] != std::byte{0x8b}) {
        void *const copy = destination ? (HLib::GetTaskDataSize(destination) >= source.size() ? destination : nullptr) :
                                         HLib::AllocateStaticOrDynamicBlock(source.size(), name);
        if (!copy) {
            return false;
        }
        std::memcpy(copy, source.data(), source.size());
        allocation = copy;
        size = source.size();
        return true;
    }

    const std::uint32_t outputSize = ReadU32(source.data() + source.size() - 4);
    if (!outputSize) {
        return false;
    }
    void *const output =
        destination ? (HLib::GetTaskDataSize(destination) >= outputSize ? destination : nullptr) : HLib::AllocateStaticOrDynamicBlock(outputSize, name);
    if (!output) {
        return false;
    }
    z_stream stream{};
    stream.next_in = reinterpret_cast<Bytef *>(const_cast<std::byte *>(source.data()));
    stream.avail_in = static_cast<uInt>(source.size());
    stream.next_out = static_cast<Bytef *>(output);
    stream.avail_out = outputSize;
    const int initialized = inflateInit2(&stream, MAX_WBITS + 16);
    const int inflated = initialized == Z_OK ? inflate(&stream, Z_FINISH) : Z_DATA_ERROR;
    if (initialized == Z_OK) {
        inflateEnd(&stream);
    }
    if (inflated != Z_STREAM_END || stream.total_out != outputSize) {
        if (output != destination)
            HLib::FreeTaskData(output);
        return false;
    }
    allocation = output;
    size = outputSize;
    return true;
}

bool IsGzip(std::span<const std::byte> source) {
    return source.size() >= 18 && source[0] == std::byte{0x1f} && source[1] == std::byte{0x8b};
}

bool InflateOwnedPackage(void *packed, std::size_t packedSize, void *&allocation, std::size_t &size, shen_name allocationName
#if SHENMUE_IS_DC
    ,
    bool packedOwned = true
#endif
) {
    allocation = nullptr;
    size = 0;
    if (!packed || !packedSize)
        return false;
    const auto source = std::span<const std::byte>(static_cast<const std::byte *>(packed), packedSize);
    if (!IsGzip(source)) {
        allocation = packed;
        size = packedSize;
        return true;
    }
    const bool inflated = InflateGzip(source, allocation, size, allocationName);
#if SHENMUE_IS_DC
    if (packedOwned)
#endif
        HLib::FreeTaskData(packed);
    return inflated;
}


struct PakRecord {
    std::uint32_t name;
    std::span<const std::byte> data;
};

std::vector<PakRecord> PakRecords(std::span<const std::byte> chunk) {
    std::vector<PakRecord> records;
    std::size_t cursor = 8;
    while (cursor <= chunk.size() && chunk.size() - cursor >= 8) {
        const std::uint32_t size = ReadU32(chunk.data() + cursor + 4);
        if (size < 8 || size > chunk.size() - cursor)
            break;
        records.push_back({.name = ReadU32(chunk.data() + cursor), .data = chunk.subspan(cursor + 8, size - 8)});
        cursor += size;
    }
    return records;
}

void ClosePakFile(PakGlobalData &slot) {
    if ((slot.flags & 1U) != 0) {
        if (slot.afsFile)
            d3t::ReadCachedFile(slot.afsFile);
        slot.afsFile = nullptr;
        return;
    }
    d3t::FileState *const file = slot.fileLoad;
    if (!file)
        return;
    d3t::Archive_DetachBuffer(file->handle);
    d3t::ReadFile_0(0, &file->handle);
    slot.fileLoad = nullptr;
}

void ReadPakCharacterTable(PakLoadTaskData &task, std::span<const std::byte> table, shen_name owner) {
    const std::uint32_t savedFlags = task.flags;
    task.flags = 1;
    task.unknown04 = 1;
    CHRS_ReadTable(table, g_entryPoint, owner);
    task.unknown04 = 0;
    task.flags = savedFlags;
}

void RetirePakCharacterTable(std::size_t index) {
    PakGlobalData *const pak = &pakSlots[index];
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    PakGlobalData *const savedNext = task->next;
    task->next = pak;
    task->unknown04 = 1;
    KillCharactersByModelName(shen_name{make_shen_name('P', 'A', 'K', static_cast<char>('a' + index))}, 1);
    CleanupMapControlCharacters();
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    ReleasePackageModelImages(control->modelManagerTask, pak);
    task->next = savedNext;
    task->unknown04 = 0;
}

void ReleasePakTextures(PakGlobalData &slot) {
    for (std::int32_t index = 0; index < slot.textureIndex; ++index)
        ReleaseNamedTexture(slot.textureHandles[index]);
    if (slot.textureHandles) {
        HLib::FreeTaskData(slot.textureHandles);
        slot.textureHandles = nullptr;
    }
}

void ReleasePackageContents(std::size_t index) {
    if (index >= loadedMapPackageCount || !pakSlots[index].data)
        return;
    PakGlobalData &slot = pakSlots[index];
    LoadedMapPackage &package = loadedMapPackages[index];
    std::size_t processed = static_cast<std::uint16_t>(slot.chunkIndex);
    if (!processed || processed > package.pks.entries.size())
        processed = package.pks.entries.size();
    if (slot.modelHandles) {
        for (std::int32_t modelIndex = static_cast<std::int32_t>(pakRuntimes[index].modelCapacity) - 1; modelIndex >= 0; --modelIndex) {
            const MT7Node *const data = GetMT7ReaderData(static_cast<MT7Model *>(slot.modelHandles[modelIndex]));
            d3t::ReleaseObjectSource(data);
            d3t::ReleaseObjectSource(data);
        }
    }
    std::int32_t modelIndex = 0;
    std::int32_t motionIndex = 0;
    std::int32_t propIndex = 0;
    for (std::size_t chunk = 0; chunk < processed; ++chunk) {
        const PakContainerEntry &entry = package.pks.entries[chunk];
        const std::uint32_t tag = ReadU32(reinterpret_cast<const std::byte *>(entry.extension.data()));
        const std::span<const std::byte> bytes = package.pks.bytes.subspan(package.pks.ipacOffset + entry.relativeOffset, entry.size);
        if (tag == SHEN_NAME(CHRM) || tag == SHEN_NAME(MAPM) || tag == SHEN_NAME(MAPD) || tag == SHEN_NAME(PROP)) {
            MT7Model *const model = modelIndex < slot.modelIndex ? static_cast<MT7Model *>(slot.modelHandles[modelIndex]) : nullptr;
            ++modelIndex;
            if (tag == SHEN_NAME(PROP)) {
                if (model)
                    RemovePackagePropReader(slot.propHandles[propIndex]);
                ++propIndex;
            }
            if (model) {
                RetirePackageMapModel(*model);
                ReleaseModelFormat7Storage(*model);
                delete model;
            }
        } else if (tag == SHEN_NAME(MOTN)) {
            if (motionIndex < slot.motionIndex)
                DeleteBorrowedMotion(static_cast<std::int32_t>(slot.motionHandles[motionIndex]));
            ++motionIndex;
        } else if (tag == fldd) {
            for (const PakRecord &record : PakRecords(bytes))
                RemoveFieldPart(shen_name{record.name});
        } else if (tag == lght) {
            for (const PakRecord &record : PakRecords(bytes))
                UnregisterLightData(static_cast<std::int32_t>(record.name));
        } else if (tag == motp) {
            MOTPParser(static_cast<std::int32_t>(slot.motionProperty));
        } else if (tag == srch) {
            SRCHParser(bytes.data());
        } else if (tag == lock) {
            srchPushPopTarget();
        } else if (tag == ev1 || tag == ev2 || tag == ev3) {
            std::array<char, 16> eventFile{};
            std::snprintf(eventFile.data(), eventFile.size(), "%.8s.EV%01d", entry.stem.data(), tag == ev1 ? 1 : tag == ev2 ? 2 : 3);
            scenFreeEvent(eventFile.data());
        } else if (tag == auth) {
            FreeAUTH(&slot);
        }
    }
    if (slot.flags & 2U)
        RetirePakCharacterTable(index);
    if (slot.modelHandles) {
        for (std::uint32_t model = 0; model < pakRuntimes[index].modelCapacity; ++model)
            slot.modelHandles[model] = nullptr;
    }
}

void ReleasePackageBuffers(std::size_t index) {
    PakGlobalData &slot = pakSlots[index];
    LoadedMapPackage &package = loadedMapPackages[index];
    if (slot.modelHandles) {
        HLib::FreeTaskData(slot.modelHandles);
        slot.modelHandles = nullptr;
    }
    if (slot.mapHandles) {
        HLib::FreeTaskData(slot.mapHandles);
        slot.mapHandles = nullptr;
    }
    if (slot.motionHandles) {
        HLib::FreeTaskData(slot.motionHandles);
        slot.motionHandles = nullptr;
    }
    if (slot.propHandles) {
        HLib::FreeTaskData(slot.propHandles);
        slot.propHandles = nullptr;
    }
#if SHENMUE_IS_DC
    if (!pakRuntimes[index].destination) {
#endif
        if (slot.data && slot.data != slot.status && slot.data != package.pkfAllocation && slot.data != package.pksAllocation)
            HLib::FreeTaskData(slot.data);
        if (package.pkfAllocation && package.pkfAllocation != slot.status)
            HLib::FreeTaskData(package.pkfAllocation);
        if (package.pksAllocation && package.pksAllocation != slot.status)
            HLib::FreeTaskData(package.pksAllocation);
#if SHENMUE_IS_DC
    }
#endif
    slot.name = {};
    slot.data = nullptr;
}

void ReleasePackageStorage(std::size_t index) {
    if (index >= loadedMapPackageCount)
        return;
    PakGlobalData &slot = pakSlots[index];
    ReleasePakTextures(slot);
    ReleasePackageContents(index);
    ReleasePackageBuffers(index);
    ClosePakFile(slot);
    loadedMapPackages[index] = {};
    pakRuntimes[index] = {};
}

void RemovePackage(std::size_t index) {
    if (index >= loadedMapPackageCount)
        return;
    ReleasePackageStorage(index);
    if (pakLoadTaskData) {
        if (pakLoadTaskData->linkedPakEntry == &pakSlots[index])
            pakLoadTaskData->linkedPakEntry = nullptr;
        if (pakLoadTaskData->field70 == reinterpret_cast<std::uintptr_t>(&pakSlots[index]))
            pakLoadTaskData->field70 = 0;
    }
    while (loadedMapPackageCount && !pakSlots[loadedMapPackageCount - 1].name.name)
        --loadedMapPackageCount;
}

void FailPackage(PakLoadTaskData &task, PakGlobalData &slot) {
    if (task.pakBuffer == &slot)
        task.pakBuffer = nullptr;
    if (task.pakt == &slot)
        task.pakt = nullptr;
    if (task.next == &slot)
        task.next = nullptr;
    const std::size_t index = PakSlotIndex(&slot);
    if (index < loadedMapPackageCount) {
        pakRuntimes[index].phase = PakPhase::Failed;
        RemovePackage(index);
    }
}

bool PAK_Reading(PakLoadTaskData &task, PakGlobalData &slot, const char *path, PakPhase readingPhase, std::uint64_t offset = 0, std::uint64_t size = 0) {
#if SHENMUE_IS_PC
    task.retryCount = 0;
    slot.flags &= ~1U;
#endif
    if (!path || !*path)
        return false;
    const std::uint64_t packedSize = size ? size : ArchiveData_GetFileSize(path);
    if (!packedSize) {
#if SHENMUE_IS_PC
        task.pakBuffer = nullptr;
        PakLoadingDiagnosticNullsub();
#endif
        return false;
    }
    if (packedSize > (std::numeric_limits<std::size_t>::max)() - 63U)
        return false;
    d3t::FileState *const file = d3t::Archive_GetNextUnusedFileState();
    if (!file)
        return false;
    const int pathLength = std::snprintf(file->path, sizeof(file->path), "%s", path);
    if (pathLength < 0 || static_cast<std::size_t>(pathLength) >= sizeof(file->path)) {
        d3t::ReadFile_0(0, &file->handle);
        return false;
    }
    const std::size_t index = PakSlotIndex(&slot);
    if (index >= loadedMapPackageCount) {
        d3t::ReadFile_0(0, &file->handle);
        return false;
    }
    const shen_name allocationName{make_shen_name('P', 'A', 'K', static_cast<char>('a' + index))};
#if SHENMUE_IS_DC
    const std::uint64_t alignment = (slot.flags & 1U) != 0 ? 2048U : 64U;
    const std::uint64_t allocationSize = (packedSize + alignment - 1U) & ~(alignment - 1U);
    void *const packed = pakRuntimes[index].destination && (readingPhase == PakPhase::ReadingPakf || (slot.flags & 8U) == 0) ?
        pakRuntimes[index].destination :
        HLib::AllocateStaticOrDynamicBlock(allocationSize, allocationName);
#else
    // The PKS after a compressed PKF reads into a temporary static block, otherwise into the caller's status or a
    // fresh task block; every other read takes a 64-byte-aligned static block.
#if !SHENMUE_IS_PC
    slot.flags &= ~1U;
#endif
    void *const packed = (slot.flags & 4U) != 0 && (task.loadFlags & 4U) == 0 ?
        ((slot.flags & 8U) != 0 ? HLib::AllocateStaticOrDynamicBlock(packedSize, allocationName) :
                slot.status     ? slot.status :
                                  HLib::AllocateTaskData(packedSize, allocationName)) :
        HLib::AllocateStaticOrDynamicBlock((packedSize + 63U) & ~std::uint64_t{63}, allocationName);
#endif
    if (!packed) {
        d3t::ReadFile_0(0, &file->handle);
        return false;
    }
    slot.data = packed;
    slot.dataSize = packedSize;
    slot.fileLoad = file;
    pakRuntimes[index].phase = readingPhase;
#if !SHENMUE_IS_PC
    task.retryCount = 0;
#endif
    task.pakBuffer = &slot;
    if (!d3t::Archive_BindBuffer(file->handle, packed, packedSize, true)) {
#if SHENMUE_IS_DC
        if (!pakRuntimes[index].destination)
#endif
            if (packed != slot.status)
                HLib::FreeTaskData(packed);
        slot.data = nullptr;
        slot.dataSize = 0;
        slot.fileLoad = nullptr;
        if (task.pakBuffer == &slot)
            task.pakBuffer = nullptr;
        d3t::ReadFile_0(0, &file->handle);
        return false;
    }
    d3t::Archive_QueueRead(file->handle, 0, file->path, offset, packedSize, 0x21);
    std::size_t length = 0;
    for (; length + 1 < pakReadName.size() && slot.entryType[length]; ++length)
        pakReadName[length] = slot.entryType[length];
    pakReadName[length] = '\0';
    pakReadSize = packedSize;
    pakReadBuffer = packed;
    pakReadHeader = -1;
    pakReadAllocationSize = HLib::GetTaskDataSize(packed);
    pakReadResult = -1;
    return true;
}

#if !SHENMUE_IS_DC


bool ReadPakFromAfs(PakLoadTaskData &task, PakGlobalData &slot, std::int32_t member, PakPhase readingPhase) {
    slot.flags |= 1U;
    const PakAfsPartition *const partition = static_cast<const PakAfsPartition *>(slot.field30);
    slot.afsFile = d3t::LargeFile_OpenTocEntryHandle(partition->afs->index, member);
    const std::uint64_t sectors = static_cast<std::uint64_t>(d3t::LargeFile_GetSizeInBlocks(slot.afsFile));
    const std::uint64_t size = static_cast<std::uint32_t>(sectors << 11U);
    const std::size_t index = PakSlotIndex(&slot);
    if (!slot.data) {
        const shen_name allocationName{make_shen_name('P', 'A', 'K', static_cast<char>('a' + index))};
        if (slot.status)
            slot.data = HLib::GetTaskDataSize(slot.status) >= size ? slot.status : nullptr;
        else
            slot.data = (slot.flags & 0x20U) != 0 ? HLib::AllocateStaticOrDynamicBlock(size, allocationName) : HLib::AllocateTaskData(size, allocationName);
    }
    if (!slot.data) {
        task.pakBuffer = nullptr;
        ClosePakFile(slot);
        return false;
    }
    d3t::GetFileFromCachedBuffer(slot.afsFile, sectors, slot.data, 0);
    slot.dataSize = size;
    if (index < loadedMapPackageCount)
        pakRuntimes[index].phase = readingPhase;
    task.pakBuffer = &slot;
    std::size_t length = 0;
    for (; length + 1 < pakReadName.size() && slot.entryType[length]; ++length)
        pakReadName[length] = slot.entryType[length];
    pakReadName[length] = '\0';
    pakReadSize = size;
    pakReadBuffer = slot.data;
    pakReadHeader = -1;
    pakReadAllocationSize = HLib::GetTaskDataSize(slot.data);
    pakReadResult = -1;
    return true;
}
#endif

bool CompletePakRead(PakLoadTaskData &task, PakGlobalData &slot) {
    std::uint32_t handle;
    if ((slot.flags & 1U) != 0) {
        if (!slot.afsFile || static_cast<std::uint32_t>(slot.afsFile->entryIndex) >= d3t::g_largeFileData.size())
            return false;
        handle = d3t::g_largeFileData[static_cast<std::size_t>(slot.afsFile->entryIndex)].archiveHandle;
    } else {
        if (!slot.fileLoad)
            return false;
        handle = slot.fileLoad->handle;
    }
    if (d3t::Archive_HasPendingRead(handle))
        return false;
    const d3t::ArchiveHandleEntry *const archive = d3t::Archive_GetHandle(handle);
    const bool succeeded = archive && archive->readSucceeded != 0;
    pakReadResult = 0;
    task.pakBuffer = nullptr;
    if (!succeeded) {
        FailPackage(task, slot);
        return false;
    }

    const std::size_t index = PakSlotIndex(&slot);
    if (index >= loadedMapPackageCount)
        return false;
    PakRuntime &runtime = pakRuntimes[index];
    LoadedMapPackage &package = loadedMapPackages[index];
    bool pakf = runtime.phase == PakPhase::ReadingPakf;
    void *allocation = nullptr;
    std::size_t size = 0;
    void *const packed = slot.data;
    const std::size_t packedSize = static_cast<std::size_t>(slot.dataSize);
    slot.data = nullptr;
    slot.dataSize = 0;
#if SHENMUE_IS_DC
    const shen_name allocationName{pakf ? pakF : pakS};
    if (IsGzip({static_cast<const std::byte *>(packed), packedSize}))
        slot.flags |= 8U;
    if (!InflateOwnedPackage(packed, packedSize, allocation, size, allocationName, packed != runtime.destination)) {
        FailPackage(task, slot);
        return false;
    }
#else
    // A first read that is not the expected container but is gzip marks the package compressed; the PKS that
    // follows a PKF inherits the mark without being inspected.
    if ((slot.flags & 4U) == 0 || (task.loadFlags & 4U) != 0) {
        const std::uint32_t expected = (task.loadFlags & 4U) != 0 ? pakS : pakF;
        if (ReadU32(static_cast<const std::byte *>(packed)) != expected && IsGzip({static_cast<const std::byte *>(packed), packedSize}))
            slot.flags |= 8U;
    }
    allocation = packed;
    size = packedSize;
    if ((slot.flags & 8U) != 0) {
        PakLoadingDiagnosticNullsub();


        size = ReadU32(static_cast<const std::byte *>(packed) + packedSize - 4);
        void *output = slot.status;
        if (output) {
            if (HLib::GetTaskDataSize(output) < size)
                output = nullptr;
        } else {
            output = HLib::AllocateTaskData(size, shen_name{make_shen_name('P', 'A', 'K', static_cast<char>('a' + index))});
        }
        void *const window = HLib::AllocateStaticOrDynamicBlock(0x3000, {});
        void *const work = HLib::AllocateStaticOrDynamicBlock(0x1000, {});
        if (!output || !window || !work || !InflateGzip({static_cast<const std::byte *>(packed), packedSize}, allocation, size, {}, output)) {
            slot.data = packed;
            FailPackage(task, slot);
            PakLoadingDiagnosticNullsub();
            return false;
        }
        HLib::FreeTaskData(work);
        HLib::FreeTaskData(window);
        HLib::FreeTaskData(packed);
        PakLoadingDiagnosticNullsub();
    }
#endif
    PakContainerView container{};
    if (!ParseContainer({static_cast<const std::byte *>(allocation), size}, container)) {
#if SHENMUE_IS_DC
        if (!runtime.destination)
#endif
            if (allocation != slot.status)
                HLib::FreeTaskData(allocation);
        FailPackage(task, slot);
        return false;
    }
    // 531570 dispatches from the actual decoded signature. An AFS member
    // may be a standalone PKS; it is not forced through a fictitious PKF pair.
    pakf = container.signature == pakF;
    pakReadBuffer = allocation;
    pakReadAllocationSize = HLib::GetTaskDataSize(allocation);
    pakReadHeader = static_cast<std::int32_t>(ReadU32(static_cast<const std::byte *>(allocation) + 8));
    slot.data = allocation;
    slot.dataSize = size;
    slot.chunkIndex = 0;
    task.field68 = 0;
    if (pakf) {
        package.pkfAllocation = allocation;
        package.pkfSize = size;
        package.pkf = container;
        runtime.phase = PakPhase::ParsingPakf;
        task.pakt = &slot;
    } else {
        package.pksAllocation = allocation;
        package.pksSize = size;
        package.pks = container;
        runtime.phase = PakPhase::ParsingPaks;
        task.next = &slot;
    }
    return true;
}

bool AllocatePAKT(PakLoadTaskData &task, PakGlobalData &slot) {
    LoadedMapPackage *const package = PackageFor(&slot);
    if (!package || package->pkf.signature != pakF) {
        slot.state = -2;
        return true;
    }
    const std::size_t textureCount = package->pkf.textureCount;
    if (textureCount > static_cast<std::size_t>((std::numeric_limits<std::int16_t>::max)())) {
        slot.state = -2;
        return true;
    }
    if (!textureCount)
        return true;
    if (!slot.textureHandles) {
        if (textureCount > ((std::numeric_limits<std::size_t>::max)() / sizeof(std::int32_t)) - 1) {
            slot.state = -2;
            return true;
        }
        const std::size_t allocationSize = (textureCount + 1) * sizeof(std::int32_t);
        slot.textureHandles =
            static_cast<std::int32_t *>((slot.flags & 0x20U) ? HLib::AllocateTaskData(allocationSize, PACKAGE_TEXTURE_INDEX_MEMORY) :
                                                               HLib::AllocateStaticOrDynamicBlock(allocationSize, PACKAGE_TEXTURE_INDEX_MEMORY));
        if (!slot.textureHandles)
            return true;
        package->textures.reserve(textureCount);
        slot.textureIndex = 0;
        slot.textureCursor = package->pkf.bytes.data() + 0x10;
        if (package->pkf.ipacOffset >= 0x18 && ReadU32(slot.textureCursor) == dumY) {
            const std::uint32_t size = ReadU32(slot.textureCursor + 4);
            if (size < 8 || size > package->pkf.ipacOffset - 0x10) {
                slot.state = -2;
                return true;
            }
            slot.textureCursor += size;
        }
    }

    if ((slot.flags & 0x10U) != 0)
        SetNamedTextureBank(2);
    const float started = static_cast<float>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta));
    while (slot.textureIndex < static_cast<std::int32_t>(textureCount)) {
        if (!IsWithinCallbackBudget(task, started))
            break;
        const std::size_t cursor = static_cast<std::size_t>(slot.textureCursor - package->pkf.bytes.data());
        if (cursor > package->pkf.ipacOffset || package->pkf.ipacOffset - cursor < 0x10 || ReadU32(slot.textureCursor) != texN) {
            slot.state = -2;
            if ((slot.flags & 0x10U) != 0)
                SetNamedTextureBank(0);
            return true;
        }
        const std::uint32_t chunkSize = ReadU32(slot.textureCursor + 4);
        if (chunkSize < 0x10 || chunkSize > package->pkf.ipacOffset - cursor) {
            slot.state = -2;
            if ((slot.flags & 0x10U) != 0)
                SetNamedTextureBank(0);
            return true;
        }
        const std::span<const std::byte> chunk = package->pkf.bytes.subspan(cursor, chunkSize);
        const std::uint32_t idLow = ReadU32(chunk.data() + 8);
        const std::uint32_t idHigh = ReadU32(chunk.data() + 12);
        const std::int32_t textureIndex = static_cast<std::int32_t>(ReadTexnTexture(chunk.data()));
        slot.textureHandles[slot.textureIndex] = textureIndex;
        if (textureIndex < 0) {
            slot.state = -2;
            break;
        }
        const std::uint32_t handle = ResolveNamedTextureHandle(textureIndex);
        ++slot.textureIndex;
        package->textures.push_back({.idLow = idLow, .idHigh = idHigh, .handle = handle, .textureIndex = static_cast<std::int16_t>(textureIndex)});
        slot.textureCursor += chunkSize;
    }
    if ((slot.flags & 0x10U) != 0)
        SetNamedTextureBank(0);
    if (slot.textureIndex < static_cast<std::int32_t>(textureCount))
        return false;
    slot.textureHandles[textureCount] = -1;
    return true;
}

bool AllocatePAKStuff(PakLoadTaskData &task, PakGlobalData &slot) {
    PakContainerView *const container = ActiveContainer(&slot);
    LoadedMapPackage *const package = PackageFor(&slot);
    if (!container || !package) {
        slot.state = -2;
        return true;
    }
    if (container->entries.size() > static_cast<std::size_t>((std::numeric_limits<std::int16_t>::max)())) {
        slot.state = -2;
        return true;
    }
    if (container->entries.empty() || task.field68 == reinterpret_cast<std::uintptr_t>(&slot))
        return true;
    if (container->signature == pakS && !slot.chunkIndex && !slot.modelHandles) {
        PakRuntime &runtime = *RuntimeFor(&slot);
        runtime.modelCapacity = ReadU32(container->bytes.data() + 12);
        slot.modelIndex = 0;
        if (runtime.modelCapacity) {
            const std::uint64_t size = sizeof(void *) * static_cast<std::uint64_t>(runtime.modelCapacity);
            slot.modelHandles = static_cast<void **>(
                (slot.flags & 0x30U) ? HLib::AllocateStaticOrDynamicBlock(size, MODEL_PACKAGE_MEMORY) : HLib::AllocateTaskData(size, MODEL_PACKAGE_MEMORY));
            if (!slot.modelHandles)
                return true;
            std::memset(slot.modelHandles, 0, static_cast<std::size_t>(size));
        }
        std::uint32_t mapCount = 0;
        for (const PakContainerEntry &entry : container->entries)
            if (!std::memcmp(entry.extension.data(), "MAPM", 4) || !std::memcmp(entry.extension.data(), "MAPD", 4))
                ++mapCount;
        if (mapCount) {
            slot.mapHandles = static_cast<std::int32_t *>(HLib::AllocateTaskData(4ULL * (mapCount + 1), MODEL_PACKAGE_MEMORY));
            if (!slot.mapHandles)
                return true;
            for (std::uint32_t index = 0; index <= mapCount; ++index)
                slot.mapHandles[index] = -1;
        }
    }
    if (container->signature == pakS && !slot.chunkIndex && !slot.motionHandles) {
        std::uint32_t motionCount = 0;
        for (const PakContainerEntry &entry : container->entries) {
            if (!std::memcmp(entry.extension.data(), "MOTN", 4)) {
                ++motionCount;
            }
        }
        slot.motionIndex = 0;
        if (motionCount) {
            slot.motionHandles = static_cast<std::uint32_t *>(HLib::AllocateTaskData(4ULL * motionCount, MODEL_PACKAGE_MEMORY));
            if (!slot.motionHandles)
                return true;
        }
    }
    if (container->signature == pakS && !slot.chunkIndex && !slot.propHandles) {
        std::uint32_t propCount = 0;
        for (const PakContainerEntry &entry : container->entries)
            if (!std::memcmp(entry.extension.data(), "PROP", 4))
                ++propCount;
        slot.propIndex = 0;
        if (propCount) {
            slot.propHandles = static_cast<std::uint32_t *>(HLib::AllocateTaskData(4ULL * propCount, PROP_PACKAGE_MEMORY));
            if (!slot.propHandles)
                return true;
        }
    }
    const float started = static_cast<float>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta));
    std::size_t index = static_cast<std::uint16_t>(slot.chunkIndex);
    while (index < container->entries.size()) {
        if (!IsWithinCallbackBudget(task, started))
            return false;
        const PakContainerEntry &entry = container->entries[index];
        std::uint32_t tag{};
        std::memcpy(&tag, entry.extension.data(), sizeof(tag));
        const std::size_t offset = static_cast<std::size_t>(container->ipacOffset) + entry.relativeOffset;
        const std::span<const std::byte> bytes = container->bytes.subspan(offset, entry.size);
        if (tag == SHEN_NAME(CHRM) || tag == SHEN_NAME(MAPM) || tag == SHEN_NAME(MAPD) || tag == SHEN_NAME(PROP)) {
            PakRuntime &runtime = *RuntimeFor(&slot);
            if (slot.modelHandles && static_cast<std::uint32_t>(slot.modelIndex) < runtime.modelCapacity) {
                MT7Model *model = nullptr;
                if (bytes.size() >= sizeof(MT7Header)) {
                    const std::uint32_t signature = ReadU32(bytes.data());
                    if (((signature ^ SHEN_NAME(MD07)) & 0xff00ffffU) == 0 || ((signature ^ SHEN_NAME(MD0X)) & 0xff00ffffU) == 0) {
                        std::array<char, 512> path{};
                        std::snprintf(path.data(), path.size(), "%s/%.24s/%.8s.%.4s_", runtime.folder.data(), slot.entryType.data(), entry.stem.data(),
                            tag == SHEN_NAME(MAPD) ? "MAPM" : entry.extension.data());
                        model = new (std::nothrow) MT7Model{};
                        if (model && !ReadModelFormat7(bytes, path.data(), *model)) {
                            delete model;
                            model = nullptr;
                        }
                        if (model) {
                            void *const sourceAllocation = container == &package->pks ? package->pksAllocation : package->pkfAllocation;
                            BindMT7WritableRuntimeMeshes(*model, {static_cast<std::byte *>(sourceAllocation) + offset, entry.size});
                        }
                        if (model && tag == SHEN_NAME(CHRM))
                            AssignFlags(model);
                        if (model && tag == SHEN_NAME(PROP)) {
                            slot.propHandles[slot.propIndex] = static_cast<std::uint32_t>(RegisterPackagePropReader(model));
                        }
                        if (model && (tag == SHEN_NAME(MAPM) || tag == SHEN_NAME(MAPD))) {
                            const std::int32_t mapIndex = GetNextEmptyMapModelSlot();
                            AssignPackageMapModel(mapIndex, *model);
                            if (tag == SHEN_NAME(MAPD))
                                SetMapModelSlotEnabled(mapIndex, 0);
                            if (slot.mapHandles) {
                                std::uint32_t mapEntry = 0;
                                while (mapEntry < 33 && slot.mapHandles[mapEntry] != -1)
                                    ++mapEntry;
                                if (mapEntry < 33)
                                    slot.mapHandles[mapEntry] = mapIndex;
                            }
                        }
                    }
                }
                slot.modelHandles[slot.modelIndex++] = model;
                if (model) {
                    char stem[9]{};
                    std::strncpy(stem, entry.stem.data(), 8);
                    FilepathBuilder(GetMT7ReaderData(model), runtime.folder.data(), slot.entryType.data(), stem, tag == SHEN_NAME(CHRM) ? 0x22U : 0x2001U,
                        task.renderThreadHandle);
                }
                if (tag == SHEN_NAME(PROP))
                    ++slot.propIndex;
            }
        } else if (tag == SHEN_NAME(MOTN)) {
            slot.motionHandles[slot.motionIndex++] = static_cast<std::uint32_t>(LoadMoti(bytes));
        } else if (tag == fldd) {
            for (const PakRecord &record : PakRecords(bytes))
                ParseFLDD(record.data, shen_name{record.name});
        } else if (tag == lght) {
            for (const PakRecord &record : PakRecords(bytes))
                RegisterLightData(record.data.data(), static_cast<std::int32_t>(record.name), record.data.size());
        } else if (tag == ev1 || tag == ev2 || tag == ev3) {
            const std::int32_t type = tag == ev1 ? 1 : tag == ev2 ? 2 : 3;
#if SHENMUE_IS_DC
            InstallEventOverlay(bytes.data(), entry.stem.data(), bytes.size(), static_cast<EventFunctionType>(type));
#else
            std::array<char, 16> eventFile{};
            std::snprintf(eventFile.data(), eventFile.size(), "%.8s.EV%01d", entry.stem.data(), type);
            shenLoadEventFrom(eventFile.data(), static_cast<EventFunctionType>(type));
#endif
        } else if (tag == motp) {
            slot.motionProperty = (slot.motionProperty & ~std::uint64_t{0xffffffff}) | static_cast<std::uint32_t>(MotionPropFBX(bytes));
        } else if (tag == srch) {
            ReadSRCH(bytes);
        } else if (tag == lock) {
            ReadLOCK(reinterpret_cast<SearchTarget *>(const_cast<std::byte *>(bytes.data())));
        } else if (tag == auth) {
            ReadAUTH(bytes.data(), &slot);
        } else if (tag == SHEN_NAME(DYNM) && !slot.dynamicMapData) {
            slot.dynamicMapData = bytes.data();
        } else if (tag == chrt && (slot.flags & 2U) == 0) {
            RuntimeFor(&slot)->characterTable = bytes;
            task.data = reinterpret_cast<HLib::TaskData *>(const_cast<std::byte *>(bytes.data()));
            task.dataSize = static_cast<std::uint32_t>(bytes.size());
            slot.flags |= 2U;
        } else if (tag == SHEN_NAME(SNDP) && !slot.soundTable) {
            // AllocatePAKStuff retains the first SNDP across both containers.
            slot.soundTable = bytes.data();
            RuntimeFor(&slot)->soundTableSize = bytes.size();
        }
        slot.chunkIndex = static_cast<std::int16_t>(++index);
    }
    slot.chunkIndex = 0;
    task.field68 = reinterpret_cast<std::uintptr_t>(&slot);
    return true;
}

bool QueuePaksAfterPakf(PakLoadTaskData &task, PakGlobalData &slot) {
    PakRuntime *const runtime = RuntimeFor(&slot);
    LoadedMapPackage *const package = PackageFor(&slot);
    if (!runtime || !package)
        return false;
    slot.flags |= 4U;
    if (runtime->memoryPackage) {
        slot.data = package->pksAllocation;
        slot.dataSize = package->pksSize;
        slot.chunkIndex = 0;
        task.field68 = 0;
        runtime->phase = PakPhase::ParsingPaks;
        task.next = &slot;
        return true;
    }
#if !SHENMUE_IS_DC
    if ((slot.flags & 1U) != 0)
        return ReadPakFromAfs(task, slot, slot.field92, PakPhase::ReadingPaks);
#endif
    std::array<char, 0x120> path{};
    const int pathLength = std::snprintf(path.data(), path.size(), "%s/%s.pks", runtime->folder.data(), package->name.data());
    return pathLength >= 0 && static_cast<std::size_t>(pathLength) < path.size() && PAK_Reading(task, slot, path.data(), PakPhase::ReadingPaks);
}

// @verified - p
void PakLoadTaskEntry_Callback(void *taskData) {
    PakLoadingDiagnosticNullsub();
    PakLoadTaskData *task = static_cast<PakLoadTaskData *>(taskData);
    if (!task)
        task = detachedPakLoadTaskData;
    if (!task) {
        PakLoadingDiagnosticNullsub();
        return;
    }

    if (!task->pakBuffer && !task->pakt && !task->next && HasThreadHandle(task->renderThreadHandle)) {
        LockThreadHandle(task->renderThreadHandle);
        HLib::SetupTimedTask(pakLoadTask, 0);
        PakLoadingDiagnosticNullsub();
        return;
    }
    const std::uint32_t started = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);

    if (PakGlobalData *const reading = task->pakBuffer) {
        if ((reading->flags & 1U) != 0) {


            if (GetAfsFileLoadState(reading->afsFile) == 3)
                CompletePakRead(*task, *reading);
        } else if (reading->fileLoad && !d3t::Archive_HasPendingRead(reading->fileLoad->handle)) {
            CompletePakRead(*task, *reading);
        }
    }

    PakGlobalData *pendingTextures = task->pakt;
    if (pendingTextures) {
        PakLoadingDiagnosticNullsub();
        if (AllocatePAKT(*task, *pendingTextures)) {
            task->pakt = nullptr;
            if (pendingTextures->state < 0) {
                FailPackage(*task, *pendingTextures);
                PakLoadingDiagnosticNullsub();
                PakLoadingDiagnosticNullsub();
                return;
            }
            task->next = pendingTextures;
        }
        PakLoadingDiagnosticNullsub();
    }

    PakGlobalData *const active = task->next;
    if (!active || !IsWithinCallbackStageBudget(*task, started)) {
        PakLoadingDiagnosticNullsub();
        return;
    }
    if (!AllocatePAKStuff(*task, *active) || !IsWithinCallbackStageBudget(*task, started)) {
        PakLoadingDiagnosticNullsub();
        return;
    }
    if (active->state < 0) {
        FailPackage(*task, *active);
        PakLoadingDiagnosticNullsub();
        return;
    }
    PakRuntime *const runtime = RuntimeFor(active);
    task->next = nullptr;
    if (!runtime) {
        PakLoadingDiagnosticNullsub();
        return;
    }
    if (runtime->phase == PakPhase::ParsingPakf) {
        active->flags |= 4U;
        LoadedMapPackage &package = *PackageFor(active);
        const std::size_t tableSize = runtime->characterTable.size();
        void *temporaryTable = nullptr;
        const std::uint64_t copiedSize = (tableSize + 63ULL) & ~63ULL;
        if (tableSize) {
            temporaryTable = (active->flags & 0x20U) ? HLib::AllocateTaskData(copiedSize, PACKAGE_CHARACTER_TABLE_TEMP) :
                                                       HLib::AllocateStaticOrDynamicBlock(copiedSize, PACKAGE_CHARACTER_TABLE_TEMP);
            if (!temporaryTable) {
                PakLoadingDiagnosticNullsub();
                return;
            }
            std::memcpy(temporaryTable, runtime->characterTable.data(), static_cast<std::size_t>(copiedSize));
        }
        runtime->characterTable = {};
        ClosePakFile(*active);
        if (package.pkfAllocation != active->status)
            HLib::FreeTaskData(package.pkfAllocation);
        package.pkfAllocation = nullptr;
        package.pkfSize = 0;
        package.pkf = {};
        active->data = nullptr;
        active->textureCursor = nullptr;
        if (temporaryTable) {
            task->data =
                static_cast<HLib::TaskData *>((active->flags & 0x20U) ? HLib::AllocateStaticOrDynamicBlock(copiedSize, PACKAGE_CHARACTER_TABLE_MEMORY) :
                                                                        HLib::AllocateTaskData(copiedSize, PACKAGE_CHARACTER_TABLE_MEMORY));
            if (!task->data) {
                HLib::FreeTaskData(temporaryTable);
                PakLoadingDiagnosticNullsub();
                return;
            }
            std::memcpy(task->data, temporaryTable, static_cast<std::size_t>(copiedSize));
            HLib::FreeTaskData(temporaryTable);
            task->dataSize = static_cast<std::uint32_t>(tableSize);
            runtime->characterTable = {reinterpret_cast<const std::byte *>(task->data), tableSize};
        }
        if (active->textureHandles && (active->flags & 0x10U) == 0) {
            const std::uint64_t textureBytes = (HLib::GetTaskDataSize(active->textureHandles) + 63ULL) & ~63ULL;
            std::int32_t *const textureHandles =
                static_cast<std::int32_t *>((active->flags & 0x20U) ? HLib::AllocateStaticOrDynamicBlock(textureBytes, PACKAGE_TEXTURE_INDEX_MEMORY) :
                                                                      HLib::AllocateTaskData(textureBytes, PACKAGE_TEXTURE_INDEX_MEMORY));
            std::memcpy(textureHandles, active->textureHandles, textureBytes);
            HLib::FreeTaskData(active->textureHandles);
            active->textureHandles = textureHandles;
        }
        if ((task->loadFlags & 2U) != 0) {
            runtime->phase = PakPhase::Ready;
            active->state = 1;
            PakLoadingDiagnosticNullsub();
            return;
        }
        if (!QueuePaksAfterPakf(*task, *active)) {
            FailPackage(*task, *active);
        }
        PakLoadingDiagnosticNullsub();
        return;
    }
    if (runtime->phase == PakPhase::ParsingPaks) {
        ClosePakFile(*active);


        if (!runtime->memoryPackage && !runtime->characterTable.empty()) {
            task->next = active;
            const char letter = static_cast<char>('a' + PakSlotIndex(active));
            ReadPakCharacterTable(*task, runtime->characterTable, shen_name{make_shen_name('P', 'A', 'K', letter)});
            task->next = nullptr;
        }
        runtime->characterTable = {};
        if (task->data) {
            HLib::FreeTaskData(task->data);
            task->data = nullptr;
        }
        runtime->phase = PakPhase::Ready;
        active->state = 1;
        if ((active->flags & 0x10U) != 0) {
            HLib::SetTaskDataMotionFlag(active->data);
            HLib::SetTaskDataMotionFlag(active->textureHandles);
        }
        task->linkedPakEntry = active;
    }
    PakLoadingDiagnosticNullsub();
}

// @verified - p
std::int32_t FindPakFileIndex(const std::byte *index, const char *filename) {
    const std::uint32_t count = ReadU32(index + 8);
    if (!count || std::strlen(filename) > 15)
        return -1;
    unsigned char name[8]{};
    for (std::size_t i = 0; i < 8 && filename[i] && filename[i] != '.'; ++i)
        name[i] = static_cast<unsigned char>(std::toupper(filename[i]));
    const std::uint32_t first =
        (static_cast<std::uint32_t>(name[0]) << 24U) | (static_cast<std::uint32_t>(name[1]) << 16U) | (static_cast<std::uint32_t>(name[2]) << 8U) | name[3];
    const std::uint32_t second =
        (static_cast<std::uint32_t>(name[4]) << 24U) | (static_cast<std::uint32_t>(name[5]) << 16U) | (static_cast<std::uint32_t>(name[6]) << 8U) | name[7];
    const std::byte *const entries = index + ReadU32(index + 12);
    std::uint32_t lower = 0, upper = count - 1, selected = 0;
    while (lower < upper) {
        selected = (lower + upper) / 2;
        const std::byte *const entry = entries + 20U * selected;
        const std::uint32_t a = ReadU32(entry), b = ReadU32(entry + 4);
        const std::uint32_t entryFirst = ((a & 0xffU) << 24U) | ((a & 0xff00U) << 8U) | ((a & 0xff0000U) >> 8U) | (a >> 24U);
        const std::uint32_t entrySecond = ((b & 0xffU) << 24U) | ((b & 0xff00U) << 8U) | ((b & 0xff0000U) >> 8U) | (b >> 24U);
        const std::int32_t difference = std::bit_cast<std::int32_t>(entryFirst - first);
        const std::int32_t tailDifference = std::bit_cast<std::int32_t>(entrySecond - second);
        if (difference == 0 && tailDifference == 0)
            return static_cast<std::int32_t>(selected);
        if ((difference != 0 ? difference : tailDifference) < 0)
            lower = selected + 1;
        else
            upper = selected;
    }
    return std::memcmp(entries + 20U * lower, name, 8) == 0 ? static_cast<std::int32_t>(lower) : -1;
}

} // namespace

// @verified - p
PakGlobalData *FindRetainingModelPackage(const MT7Model *model) {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    for (PakGlobalData &pak : pakSlots) {
        if (!pak.name.name || &pak == task->pakBuffer || !pak.modelHandles)
            continue;
        for (std::int32_t index = 0; index < pak.modelIndex; ++index)
            if (pak.modelHandles[index] == model)
                return &pak;
    }
    return nullptr;
}
// @verified - p
PakGlobalData *GetPakLoadTaskDataPointer() {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return task->linkedPakEntry;
}

// @verified - p
std::uint32_t GetPakLoadParams() {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return static_cast<std::uint32_t>(task->unknown04);
}

// @verified - p
std::uint32_t GetPakTaskDataField() {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return task->flags;
}

// @verified - p
PakGlobalData *pakLoadGetNext() {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    return task->next;
}

PakGlobalData *GetNextReadyModelPackage(PakGlobalData *previous) {
    if (!pakLoadTaskData)
        return nullptr;
    std::size_t index = previous ? PakSlotIndex(previous) + 1 : 0;
    for (; index < nativePakSlotCount; ++index) {
        PakGlobalData *const slot = &pakSlots[index];
        if (slot->name.name && slot->data && slot != pakLoadTaskData->pakBuffer && slot != pakLoadTaskData->pakt && slot != pakLoadTaskData->next)
            return slot;
    }
    return nullptr;
}

const std::byte *FindPackageDynamicMap(const std::uint8_t name[4]) {
    const std::uint32_t wanted = std::uint32_t{name[0]} | (std::uint32_t{name[1]} << 8U) | (std::uint32_t{name[2]} << 16U) | (std::uint32_t{name[3]} << 24U);
    for (const PakGlobalData &slot : pakSlots) {
        if (!slot.name.name || !slot.data)
            continue;
        const PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
        if (&slot == task->pakBuffer || &slot == task->pakt || &slot == task->next || !slot.dynamicMapData)
            continue;
        const auto *const begin = static_cast<const std::byte *>(slot.dynamicMapData);
        const std::byte *const end = begin + ReadU32(begin + 4);
        for (const std::byte *chunk = begin + 8; chunk < end; chunk += ReadU32(chunk + 4)) {
            if (ReadU32(chunk) == wanted)
                return chunk;
        }
    }
    return nullptr;
}

const MT7Model *FindLoadedModelNodeOwner(const MT7Node *node, std::size_t &index) {
    const std::uintptr_t pointer = reinterpret_cast<std::uintptr_t>(node);
    for (const PakGlobalData &slot : pakSlots) {
        if (!slot.name.name || !slot.modelHandles)
            continue;
        for (std::int32_t modelIndex = 0; modelIndex < slot.modelIndex; ++modelIndex) {
            const MT7Model *const model = static_cast<const MT7Model *>(slot.modelHandles[modelIndex]);
            if (!model || model->nodes.empty())
                continue;
            const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(model->nodes.data());
            if (pointer < begin || pointer - begin >= model->nodes.size() * sizeof(MT7Node) || (pointer - begin) % sizeof(MT7Node))
                continue;
            index = (pointer - begin) / sizeof(MT7Node);
            return model;
        }
    }
    return nullptr;
}

MT7Model *LoadGameModel(PakGlobalData *package, const char *filename) {
    const std::size_t index = PakSlotIndex(package);
    if (index >= nativePakSlotCount || !filename)
        return nullptr;
    if (package->name.name != (SHEN_NAME(HPMa) + static_cast<std::uint32_t>(index << 24U)) || !package->data)
        return nullptr;
    const PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (!task)
        return nullptr;
    const bool ready = package != task->pakBuffer && package != task->pakt && package != task->next;
    if (!ready && package != task->next)
        return nullptr;
    const PakContainerView *const container = ready ? &loadedMapPackages[index].pks : ActiveContainer(package);
    if (!container)
        return nullptr;
    unsigned char name[8]{};
    for (std::size_t character = 0; character < 8 && filename[character] && filename[character] != '.'; ++character)
        name[character] = static_cast<unsigned char>(std::toupper(static_cast<unsigned char>(filename[character])));
    const std::byte *modelSource = nullptr;
    for (const PakContainerEntry &entry : container->entries) {
        if (std::memcmp(entry.stem.data(), name, 8))
            continue;
        std::uint32_t tag;
        std::memcpy(&tag, entry.extension.data(), sizeof(tag));
        if (tag == SHEN_NAME(CHRM) || tag == SHEN_NAME(MAPM) || tag == SHEN_NAME(MAPD))
            modelSource = container->bytes.data() + container->ipacOffset + entry.relativeOffset;
        break;
    }
    if (!modelSource || !package->modelHandles)
        return nullptr;
    for (std::int32_t modelIndex = 0; modelIndex < package->modelIndex; ++modelIndex) {
        MT7Model *const model = static_cast<MT7Model *>(package->modelHandles[modelIndex]);
        if (model && model->source.data() == modelSource)
            return model;
    }
    return nullptr;
}

std::span<const std::byte> FileLoaderView(PakGlobalData *package, const char *filename, shen_name extension) {
    std::size_t slot = 0;
    while (slot < pakSlots.size() && &pakSlots[slot] != package)
        ++slot;
    if (slot == pakSlots.size())
        return {};
    if (package->name.name != (SHEN_NAME(HPMa) + static_cast<std::uint32_t>(slot << 24U)))
        return {};
    const std::byte *const data = static_cast<const std::byte *>(package->data);
    if (!data || ReadU32(data) != pakS)
        return {};
    const std::byte *const index = data + ReadU32(data + 4);
    const std::int32_t selected = FindPakFileIndex(index, filename);
    if (selected == -1)
        return {};
    const std::byte *const entry = index + ReadU32(index + 12) + 20LL * selected;
    if (ReadU32(entry + 8) != extension.name && extension.name != SHEN_NAME(****))
        return {};
    return {index + ReadU32(entry + 12), ReadU32(entry + 16)};
}

// @verified - p
void *FileLoader(PakGlobalData *package, const char *filename, shen_name extension) {
    return const_cast<std::byte *>(FileLoaderView(package, filename, extension).data());
}

// @verified - p
std::int64_t GetPakFileSize(PakGlobalData *package, const char *filename, shen_name extension) {
    const std::size_t slot = PakSlotIndex(package);
    if (slot >= nativePakSlotCount || package->name.name != (SHEN_NAME(HPMa) + static_cast<std::uint32_t>(slot << 24U)))
        return 0;
    const std::byte *const data = static_cast<const std::byte *>(package->data);
    if (!data)
        return 0;
    const std::byte *const index = data + ReadU32(data + 4);
    const std::int32_t selected = FindPakFileIndex(index, filename);
    if (selected < 0)
        return 0;
    const std::byte *const entry = index + ReadU32(index + 12) + 20LL * selected;
    if (ReadU32(entry + 8) != extension.name && extension.name != SHEN_NAME(****))
        return 0;
    return std::bit_cast<std::int32_t>(ReadU32(entry + 16));
}

// @verified - p
void *LoadCurrentPakBinary(const char *filename) {
    PakGlobalData *const package = GetPakLoadTaskDataPointer();
    return package ? FileLoader(package, filename, HAND_BANK_EXTENSION) : nullptr;
}

// @verified - p
std::int64_t GetCurrentPakBinarySize(const char *filename) {
    PakGlobalData *const package = GetPakLoadTaskDataPointer();
    return package ? GetPakFileSize(package, filename, HAND_BANK_EXTENSION) : 0;
}

// 52e960/624460/5a54b0: the active package takes precedence over map ECAM.
// The returned record borrows the selected PKS or INFO allocation.
std::uint32_t *pakFindEventCam(std::int32_t index) {
    const auto findRecord = [index](void *ecam) -> std::uint32_t * {
        if (!ecam)
            return nullptr;
        char name[16];
        std::snprintf(name, sizeof(name), "%04d", index);
        std::uint32_t wanted;
        std::memcpy(&wanted, name, sizeof(wanted));
        auto *chunk = static_cast<std::byte *>(ecam) + 8;
        for (;;) {
            std::uint32_t token, next;
            std::memcpy(&token, chunk, sizeof(token));
            if (token == wanted)
                return reinterpret_cast<std::uint32_t *>(chunk + 8);
            std::memcpy(&next, chunk + 4, sizeof(next));
            if (!next)
                return nullptr;
            chunk += next;
        }
    };
    auto *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    auto *package = task ? reinterpret_cast<PakGlobalData *>(task->field70) : nullptr;
    if (!package && task)
        package = task->linkedPakEntry;
    if (package)
        if (auto *const record = findRecord(FileLoader(package, "CAMERA", SHEN_NAME(ECAM))))
            return record;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return nullptr;
    const shen_name ecam{SHEN_NAME(ECAM)};
    return findRecord(FindMapInfoChunk(ecam.characters, control->mapInfo));
}

// @verified - p
std::int64_t ReadLegacySceneFileIntoBuffer(d3t::FileState *file, void *buffer, std::uint64_t size) {
    if (d3t::Archive_DoesHandleExist(file->handle)) {
        if (size == 0xffffffffU)
            size = ArchiveData_GetFileSize(file->path);
        d3t::Archive_BindBuffer(file->handle, buffer, size, true);
        d3t::Archive_QueueRead(file->handle, 0, file->path, file->offset, size, 0x21);
        if (g_skipArchiveReadWait == 0) {
            while (d3t::Archive_HasPendingRead(file->handle))
                std::this_thread::yield();
        }
        file->offset += size;
    }
    return 0;
}

// @verified - p
std::int64_t AssignLegacySceneFilePath(d3t::FileState *file, const char *directory, const char *filename) {
    if (d3t::Archive_DoesHandleExist(file->handle)) {
        char name[16];
        std::strcpy(name, filename);
        if (char *const extension = std::strstr(name, ".MT5"))
            std::strcpy(extension, ".MT7");
#if defined(_MSC_VER)
        sprintf_s(file->path, sizeof(file->path), "%s/%s", directory, name);
#else
        std::snprintf(file->path, sizeof(file->path), "%s/%s", directory, name);
#endif
        file->offset = 0;
    }
    return 1;
}

// @verified - p
bool FlushLegacyFileReadAndRenderQueue(d3t::FileState *file) {
    while (d3t::Archive_HasPendingRead(file->handle))
        FlushFramePacingRenderQueue();
    return true;
}

void LegacyFileLoadFailureNullsub() noexcept {
}

// @verified - p
std::int64_t LoadLegacyFiles(const char *directory, const char *filename, void *buffer, std::uint64_t size) {
    char name[16];
    std::strcpy(name, filename);
    if (char *const extension = std::strstr(name, ".MT5"))
        std::strcpy(extension, ".MT7");
    d3t::FileState *const file = d3t::Archive_GetNextUnusedFileState();
    if (file && AssignLegacySceneFilePath(file, directory, name)) {
        (void)ReadLegacySceneFileIntoBuffer(file, buffer, size);
        (void)FlushLegacyFileReadAndRenderQueue(file);
        d3t::ReadFile_0(0, &file->handle);
        return 1;
    }
    LegacyFileLoadFailureNullsub();
    return 0;
}

void *LoadLegacySceneFile(const char *directory, const char *filename, shen_name token, std::uint32_t *size) {
    char name[16];
    std::strcpy(name, filename);
    if (char *const extension = std::strstr(name, ".MT5"))
        std::strcpy(extension, ".MT7");
    char path[260];
    std::snprintf(path, sizeof(path), "%s/%s", directory, name);
    const std::uint32_t fileSize = static_cast<std::uint32_t>(ArchiveData_GetFileSize(path));
    void *allocation = nullptr;
    if (fileSize != 0) {
        allocation = HLib::AllocateTaskData(fileSize, token);
        if (allocation) {
            d3t::FileState *const file = d3t::Archive_GetNextUnusedFileState();
            if (!file) {
                HLib::FreeTaskData(allocation);
                return nullptr;
            }
            if (d3t::Archive_DoesHandleExist(file->handle)) {
                std::strcpy(file->path, path);
                file->offset = 0;
            }
            (void)ReadLegacySceneFileIntoBuffer(file, allocation, fileSize);
            while (d3t::Archive_HasPendingRead(file->handle))
                FlushFramePacingRenderQueue();
            d3t::ReadFile_0(0, &file->handle);
        }
    }
    if (size)
        *size = fileSize;
    return allocation;
}

bool LoadLegacySprite(const char *directory, const char *filename, shen_name token, void *&allocation, std::size_t &size) {
    allocation = nullptr;
    size = 0;
    char name[32];
    const int nameLength = std::snprintf(name, sizeof(name), "%s", filename);
    if (nameLength < 0 || static_cast<std::size_t>(nameLength) >= sizeof(name))
        return false;
    if (char *const extension = std::strstr(name, ".MT5"))
        std::memcpy(extension, ".MT7", 5);
    char path[256];
    const int pathLength = std::snprintf(path, sizeof(path), "/%s/%s", directory, name);
    if (pathLength < 0 || static_cast<std::size_t>(pathLength) >= sizeof(path))
        return false;
    const std::size_t packedSize = static_cast<std::size_t>(ArchiveData_GetFileSize(path));
    if (!packedSize)
        return false;
    void *const packed = HLib::AllocateStaticOrDynamicBlock(packedSize, token);
    if (!packed)
        return false;
    if (!ArchiveData_Read(path, 0, packed, packedSize)) {
        HLib::FreeTaskData(packed);
        return false;
    }
    return InflateOwnedPackage(packed, packedSize, allocation, size, token);
}

std::uint32_t UploadTexn(std::span<const std::byte> texn, std::size_t imageOffset) {
    const d3t::PvrHeader *const pvr = FindPvrHeader(texn, imageOffset);
    if (!pvr || !pvr->width || !pvr->height) {
        return 0;
    }
    const auto *payload = reinterpret_cast<const std::byte *>(pvr + 1);
    std::size_t payloadOffset = static_cast<std::size_t>(payload - texn.data());
    // CreatePVRForDirectX routes the 0x8000 and 0x8700 families (format-table flag 0x80) through the image loader,
    // skipping one DWORD when the payload is not DDS; CreateDirectXTexture2D admits only pixel codes 0..2 and
    // 0x80..0x89 in them. This also covers ParsePTX's 0x8089 dynamic NAVI textures.
    if (pvr->dataFormat == 0x80 || pvr->dataFormat == 0x87) {
        if (pvr->pixelFormat > 2 && (pvr->pixelFormat < 0x80 || pvr->pixelFormat > 0x89))
            return 0;
        if (payloadOffset >= texn.size())
            return 0;
        if (*payload != std::byte{'D'}) {
            if (texn.size() - payloadOffset <= sizeof(std::uint32_t))
                return 0;
            payload += sizeof(std::uint32_t);
            payloadOffset += sizeof(std::uint32_t);
        }
        return d3t::CreateImageTextureFromMemory(payload, texn.size() - payloadOffset, true);
    }
    // BuildMemoTextures emits 0x0989 for MEMT: CreatePVRForDirectX selects
    // upload flags zero for the 0x0900 format family, so it consumes these RGBA
    // pixels directly rather than treating them as 16-bit or encoded data.
    if (pvr->pixelFormat == 0x89 && pvr->dataFormat == 9) {
        const std::uint64_t bytes = static_cast<std::uint64_t>(pvr->width) * pvr->height * 4;
        if (bytes > texn.size() - payloadOffset)
            return 0;
        const d3t::Texture2DDescription description{pvr->width, pvr->height, 1, 1, 2, 0, 0, 1, 0, 0, payload};
        return d3t::CreateTexture2D(&description);
    }
    if (pvr->pixelFormat > 4)
        return 0;
    const std::uint64_t pixelCount = static_cast<std::uint64_t>(pvr->width) * pvr->height;
    if (pixelCount > (std::numeric_limits<std::size_t>::max)() / 4) {
        return 0;
    }
    std::vector<std::byte> pixels(static_cast<std::size_t>(pixelCount) * 4);
    d3t::DecodePvr(pixels.data(), pvr);
    const d3t::Texture2DDescription description{pvr->width, pvr->height, 1, 1, 2, 0, 0, 1, 0, 0, pixels.data()};
    return d3t::CreateTexture2D(&description);
}

std::span<const std::byte> PakContainerView::Find(const char *filename) const {
    for (const PakContainerEntry &entry : entries) {
        if (!EqualsName(entry, filename)) {
            continue;
        }
        const std::uint64_t offset = static_cast<std::uint64_t>(ipacOffset) + entry.relativeOffset;
        return bytes.subspan(static_cast<std::size_t>(offset), entry.size);
    }
    return {};
}

HLib::Task *PakLoadTaskEntry_Enqueue(std::uint8_t nextFunctionIndex, std::int32_t taskFlag) {
    pakLoadTask = HLib::EnqueueTaskWithParameter(PakLoadTaskEntry_Callback, nextFunctionIndex, 2, taskFlag ? 0 : sizeof(PakLoadTaskData), SHEN_NAME(PACK));
    if (taskFlag && pakLoadTask) {
#if SHENMUE_IS_PC
        detachedPakLoadTaskData = reinterpret_cast<PakLoadTaskData *>(allocateInGlobalHeap(sizeof(PakLoadTaskData)));
#else
        detachedPakLoadTaskData = new (std::nothrow) PakLoadTaskData{};
#endif
    }
    pakLoadTaskData = taskFlag ? detachedPakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (!pakLoadTaskData) {
        if (pakLoadTask)
            HLib::DestroyTask(pakLoadTask);
        pakLoadTask = nullptr;
        return nullptr;
    }
    pakLoadTaskData->name = SHEN_NAME(PACK);
    pakLoadTaskData->unknown04 = 0;
    pakLoadTaskData->flags = 0;
    pakLoadTaskData->path[0] = 0;
    pakLoadTaskData->data = nullptr;
    pakLoadTaskData->dataSize = 0;
    pakLoadTaskData->linkedPakEntry = nullptr;
    pakLoadTaskData->pakBuffer = nullptr;
    pakLoadTaskData->pakt = nullptr;
    pakLoadTaskData->next = nullptr;
    pakLoadTaskData->renderThreadHandle = d3t::d3t_AllocateRenderHandle();
    pakLoadTaskData->field70 = 0;
    pakLoadTaskData->loadFlags = 0;
    pakLoadTaskData->timeBudget = 500;
    pakLoadTaskData->retryCount = 0;
    pakSlots = {};
    pakReadName = {};
    pakReadSize = ~std::uint64_t{0};
    pakReadHeader = -1;
    pakReadBuffer = nullptr;
    pakReadAllocationSize = ~std::uint64_t{0};
    pakReadResult = -1;
    pakRuntimes = {};
    loadedMapPackages = {};
    loadedMapPackageCount = 0;
#if !SHENMUE_IS_DC
    for (PakAfsPartition &partition : pakAfsPartitions) {
        partition.afs = nullptr;
        partition.folder[0] = 0;
        partition.filename[0] = 0;
        partition.references = 0;
    }
#endif
    HLib::SetupTimedTask(pakLoadTask, 0);
    return pakLoadTask;
}
bool IsPakLoadTaskReady() {
    return pakLoadTaskData != nullptr;
}

#if SHENMUE_IS_DC
PakGlobalData *LoadMapPackage(const char *folder, const char *packageName, void *destination) {
#else
static PakGlobalData *RequestMapPackage(const char *folder, const char *packageName, std::uint64_t mode) {
#endif
    if (!pakLoadTaskData || !folder || !*folder || !packageName || !*packageName || pakLoadTaskData->pakBuffer || pakLoadTaskData->pakt ||
        pakLoadTaskData->next) {
        return nullptr;
    }
    const std::size_t nameLength = std::strlen(packageName);
    if (nameLength >= loadedMapPackages[0].name.size() || std::strlen(folder) >= pakLoadTaskData->path.size()) {
        return nullptr;
    }
    for (std::size_t index = 0; index < loadedMapPackageCount; ++index) {
        if (EqualsPackageName(loadedMapPackages[index], packageName)) {
            return &pakSlots[index];
        }
    }

    std::size_t index = 0;
    while (index < nativePakSlotCount && pakSlots[index].name.name)
        ++index;
    if (index == nativePakSlotCount)
        return nullptr;
    if (loadedMapPackageCount <= index)
        loadedMapPackageCount = index + 1;
    LoadedMapPackage &package = loadedMapPackages[index];
    PakRuntime &runtime = pakRuntimes[index];
    PakGlobalData &slot = pakSlots[index];
    slot = {};
    slot.motionProperty = 0xffffffffU;
#if SHENMUE_IS_DC
    runtime.destination = destination;
#endif
    std::memcpy(package.name.data(), packageName, nameLength + 1);
    std::memcpy(runtime.folder.data(), folder, std::strlen(folder) + 1);
    slot.name = shen_name{make_shen_name('H', 'P', 'M', static_cast<char>('a' + index))};
    std::memcpy(slot.entryType.data(), packageName, nameLength + 1);
    std::memcpy(pakLoadTaskData->path.data(), folder, std::strlen(folder) + 1);
    std::memcpy(pakLoadTaskData->nameStart.data(), packageName, nameLength + 1);
#if !SHENMUE_IS_DC
    if (mode == 1)
        slot.flags |= 0x20U;
    else if (mode > 15)
        slot.status = reinterpret_cast<void *>(mode);
#endif

    const bool paksOnly = (pakLoadTaskData->loadFlags & 4U) != 0;
    if (paksOnly)
        slot.flags |= 4U;
    const PakPhase readingPhase = paksOnly ? PakPhase::ReadingPaks : PakPhase::ReadingPakf;
    std::array<char, 0x120> path{};
    const int pathLength = std::snprintf(path.data(), path.size(), "%s/%s.%s", folder, packageName, paksOnly ? "pks" : "pkf");
    if (pathLength < 0 || static_cast<std::size_t>(pathLength) >= path.size() || !PAK_Reading(*pakLoadTaskData, slot, path.data(), readingPhase)) {
        RemovePackage(index);
        return nullptr;
    }
    HLib::ResolveDeferredCallback(pakLoadTask);
    return &slot;
}
#if !SHENMUE_IS_DC
PakGlobalData *LoadMapPackage(const char *folder, const char *packageName) {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    if (task)
        task->loadFlags = 0;
    return RequestMapPackage(folder, packageName, 0);
}

// @verified - p
PakGlobalData *ReadPakFileFromDirectory(const char *directory, const char *name, std::uint8_t flags) {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    task->loadFlags = (flags & 2U) != 0 ? 2U : (flags & 4U) != 0 ? 4U : 0U;
    PakGlobalData *const result = RequestMapPackage(directory, name, 0);
    if ((flags & 1U) != 0)
        result->flags |= 0x10U;
    return result;
}
#endif

bool PakLoadWait() {
    return pakLoadTaskData->pakBuffer || pakLoadTaskData->pakt || pakLoadTaskData->next;
}

#if !SHENMUE_IS_DC
namespace {

std::size_t PakAfsPartitionIndex(const PakAfsPartition *partition) {
    for (std::size_t index = 0; index < pakAfsPartitions.size(); ++index)
        if (&pakAfsPartitions[index] == partition)
            return index;
    return pakAfsPartitions.size();
}

bool PakNameMatches(const char *stored, const char *requested, bool exact) {
    const std::size_t storedLength = std::strlen(stored), requestedLength = std::strlen(requested);
    if (exact && storedLength != requestedLength)
        return false;
    const std::size_t length = storedLength < requestedLength ? requestedLength : storedLength;
    for (std::size_t index = 0; index < length; ++index) {
        if (std::tolower(static_cast<unsigned char>(stored[index])) != std::tolower(static_cast<unsigned char>(requested[index])))
            return false;
    }
    return true;
}

PakGlobalData *FindPakInAfs(PakAfsPartition *partition, std::int32_t firstIndex) {
    if (PakAfsPartitionIndex(partition) == pakAfsPartitions.size())
        return nullptr;
    char entryName[24];
    const int length = std::snprintf(entryName, sizeof(entryName), "%s/afs%05d_", partition->filename.data(), firstIndex);
    if (length < 0 || static_cast<std::size_t>(length) >= sizeof(entryName))
        return nullptr;
    for (auto &slot : pakSlots) {
        if (slot.name.name && PakNameMatches(slot.entryType.data(), entryName, true))
            return &slot;
    }
    return nullptr;
}

} // namespace


PakAfsPartition *MountPakAfs(const char *folder, const char *filename) {
    if (!folder || !filename || std::strlen(folder) >= 32 || std::strlen(filename) >= 16)
        return nullptr;
    for (PakAfsPartition &partition : pakAfsPartitions) {
        if (partition.afs && PakNameMatches(partition.folder.data(), folder, false) && PakNameMatches(partition.filename.data(), filename, false)) {
            ++partition.references;
            return &partition;
        }
    }
    std::size_t index = 0;
    while (index < pakAfsPartitions.size() && pakAfsPartitions[index].afs)
        ++index;
    if (index >= pakAfsPartitions.size())
        return nullptr;
    PakAfsPartition &partition = pakAfsPartitions[index];
    partition.afs = LoadAfsArchive(folder, filename, 2048);
    if (!partition.afs)
        return nullptr;
    std::memcpy(partition.folder.data(), folder, std::strlen(folder) + 1);
    std::memcpy(partition.filename.data(), filename, std::strlen(filename) + 1);
    partition.references = 1;
    return &partition;
}


std::int64_t UnmountPakAfs(PakAfsPartition *partition) {
    if (PakAfsPartitionIndex(partition) < pakAfsPartitions.size() && partition->afs) {
        if (--partition->references <= 0)
            FreeAfsArchive(partition->afs);
        partition->afs = nullptr;
        partition->folder[0] = 0;
        partition->filename[0] = 0;
        partition->references = 0;
    }
    return 0;
}


PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int16_t secondIndex, std::uint64_t mode) {
    if (PakAfsPartitionIndex(partition) == pakAfsPartitions.size() || !partition->afs || !pakLoadTaskData || PakLoadWait())
        return nullptr;
    if (PakGlobalData *const existing = FindPakInAfs(partition, firstIndex))
        return existing;
    std::size_t index = 0;
    while (index < nativePakSlotCount && pakSlots[index].name.name)
        ++index;
    if (index == nativePakSlotCount)
        return nullptr;
    PakGlobalData &slot = pakSlots[index];
    slot = {};
    const int length = std::snprintf(slot.entryType.data(), slot.entryType.size(), "%s/afs%05d_", partition->filename.data(), firstIndex);
    if (length < 0 || static_cast<std::size_t>(length) >= slot.entryType.size()) {
        slot = {};
        return nullptr;
    }
    if (loadedMapPackageCount <= index)
        loadedMapPackageCount = index + 1;
    std::memcpy(loadedMapPackages[index].name.data(), slot.entryType.data(), static_cast<std::size_t>(length) + 1);
    slot.name = shen_name{make_shen_name('H', 'P', 'M', static_cast<char>('a' + index))};
    slot.field30 = partition;
    slot.field90 = static_cast<std::int16_t>(firstIndex);
    slot.field92 = secondIndex;
    if (mode == 1)
        slot.flags |= 0x20U;
    else if (mode > 15)
        slot.status = reinterpret_cast<void *>(mode);
    slot.motionProperty = 0xffffffffU;
    std::snprintf(pakRuntimes[index].folder.data(), pakRuntimes[index].folder.size(), "%s", partition->folder.data());
    std::memcpy(pakLoadTaskData->path.data(), partition->folder.data(), std::strlen(partition->folder.data()) + 1);
    if (!ReadPakFromAfs(*pakLoadTaskData, slot, firstIndex, PakPhase::ReadingPakf)) {
        RemovePackage(index);
        return nullptr;
    }
    HLib::ResolveDeferredCallback(pakLoadTask);
    return &slot;
}

namespace {

void FreeReadyPak(PakGlobalData *package) {
    if (GetPakReadiness(package) == 1)
        DestroyPakFileAFS(package);
}

} // namespace
PakGlobalData *FindPakByEntryName(const char *name) {
    if (!name)
        return nullptr;
    for (auto &slot : pakSlots) {
        if (!slot.name.name)
            continue;
        if (PakNameMatches(slot.entryType.data(), name, false))
            return &slot;
    }
    return nullptr;
}

std::int64_t EV_ControlPackageResources(std::int32_t category, std::uint64_t mode, std::uintptr_t source, std::uintptr_t target, std::int32_t secondaryIndex) {
    const char *const name = reinterpret_cast<const char *>(source);
    auto *const partition = reinterpret_cast<PakAfsPartition *>(source);
    const std::int32_t firstIndex = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(target));
    const std::int16_t secondIndex = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(secondaryIndex));
    switch (category) {
    case PakCategory::Package:
        switch (mode) {
        case PakPackageOp::Load:
            if (FindPakByEntryName(reinterpret_cast<const char *>(target)))
                return 0;
            if (pakLoadTaskData)
                pakLoadTaskData->loadFlags = 0;
            return LoadMapPackage(name, reinterpret_cast<const char *>(target)) != nullptr;
        case PakPackageOp::IsLoadPending:
            return PakLoadWait();
        case PakPackageOp::IsLoaded:
            return FindPakByEntryName(name) != nullptr;
        case PakPackageOp::Free:
            FreeReadyPak(FindPakByEntryName(name));
            break;
        case PakPackageOp::FinishLoading: {
            const std::int64_t previousBudget = pakLoadTaskData->timeBudget;
            SetupPakSpace(-1);
            LoadPak();
            SetupPakSpace(previousBudget);
            break;
        }
        case PakPackageOp::LoadAfs:
            if (FindPakInAfs(partition, firstIndex) || pakLoadTaskData->pakBuffer)
                return 0;
            RequestPakFromAfs(partition, firstIndex, secondIndex, 0);
            break;
        case PakPackageOp::GetMapHandles: {
            PakGlobalData *const package = FindPakByEntryName(name);
            return GetPakReadiness(package) == 1 ? reinterpret_cast<std::intptr_t>(package->mapHandles) : 0;
        }
        case PakPackageOp::SetLoadBudget:
            SetupPakSpace(std::bit_cast<std::int64_t>(static_cast<std::uint64_t>(source)));
            break;
        case PakPackageOp::GetLoadBudget:
            return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(pakLoadTaskData->timeBudget));
        case PakPackageOp::FreeAfs:
            FreeReadyPak(FindPakInAfs(partition, firstIndex));
            break;
        case PakPackageOp::DestroyAfs:
            DestroyPakFileAFS(FindPakInAfs(partition, firstIndex));
            break;
        default:
            break;
        }
        break;
    case PakCategory::Partition:
        if (!mode)
            return reinterpret_cast<std::intptr_t>(MountPakAfs(name, reinterpret_cast<const char *>(target)));
        if (mode == PakPartitionOp::Unmount)
            return UnmountPakAfs(partition);
        break;
    case PakCategory::Task:
        switch (mode) {
        case PakTaskOp::GetFlags:
            return std::bit_cast<std::int32_t>(GetPakTaskDataField());
        case PakTaskOp::SetFlags:
            pakLoadTaskData->flags = static_cast<std::uint32_t>(source);
            break;
        case PakTaskOp::LoadNamed:
            pakLoad(FindPakByEntryName(name));
            break;
        case PakTaskOp::IsCurrentNamed: {
            PakGlobalData *const package = FindPakByEntryName(name);
            return package && GetPakLoadTaskDataPointer() == package;
        }
        case PakTaskOp::SetCurrentNamed:
            SetNewPakLoadedBuffer(FindPakByEntryName(name));
            break;
        case PakTaskOp::SetCurrentAfs:
            SetNewPakLoadedBuffer(FindPakInAfs(partition, firstIndex));
            break;
        default:
            break;
        }
        break;
    case PakCategory::LoadPackageWithMode:
        if (FindPakByEntryName(reinterpret_cast<const char *>(target)))
            return 0;
        if (pakLoadTaskData)
            pakLoadTaskData->loadFlags = 0;
        return RequestMapPackage(name, reinterpret_cast<const char *>(target), mode) != nullptr;
    case PakCategory::LoadAfsWithMode:
        if (FindPakInAfs(partition, firstIndex) || pakLoadTaskData->pakBuffer)
            return 0;
        RequestPakFromAfs(partition, firstIndex, secondIndex, mode);
        break;
    default:
        break;
    }
    return 1;
}

#endif

std::int32_t GetPakReadiness(PakGlobalData *package) {
    const std::size_t index = PakSlotIndex(package);
    if (index == nativePakSlotCount || package->name.name != (SHEN_NAME(HPMa) + static_cast<std::uint32_t>(index << 24U)))
        return -1;
    if (!pakLoadTaskData)
        return -1;
    if (!package->name.name || package == pakLoadTaskData->pakBuffer || package == pakLoadTaskData->pakt || package == pakLoadTaskData->next)
        return package->state;
    return 1;
}

void DestroyPakFileAFS(PakGlobalData *package) {
    const std::size_t index = PakSlotIndex(package);
    if (index == nativePakSlotCount || !package->name.name)
        return;
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    std::int32_t stage;
    if (task->next == package)
        stage = (package->flags & 4U) != 0 ? 5 : 3;
    else if (task->pakt == package)
        stage = 2 * static_cast<std::int32_t>((package->flags & 4U) | 1U);
    else if (task->pakBuffer == package)
        stage = (package->flags & 4U) != 0 ? 4 : 1;
    else {
        if (GetPakReadiness(package) == 1)
            RemovePackage(index);
        return;
    }
    bool releaseData = true;
    if (stage == 5) {
        ReleasePackageContents(index);
        ReleasePackageBuffers(index);
        stage = 2;
        releaseData = false;
    }
    if (stage == 4)
        stage = 2;
    if (stage == 2 || stage == 3)
        ReleasePakTextures(*package);
    if (stage == 1 || stage == 2 || stage == 3) {
        ClosePakFile(*package);
        if (releaseData) {
#if SHENMUE_IS_DC
            if (!pakRuntimes[index].destination)
#endif
                if (!package->status)
                    HLib::FreeTaskData(package->data);
            package->data = nullptr;
        }
    }
    if (task->data) {
        HLib::FreeTaskData(task->data);
        task->data = nullptr;
    }
    package->name = {};
    task->pakBuffer = nullptr;
    task->pakt = nullptr;
    task->next = nullptr;
    loadedMapPackages[index] = {};
    pakRuntimes[index] = {};
    while (loadedMapPackageCount && !pakSlots[loadedMapPackageCount - 1].name.name)
        --loadedMapPackageCount;
}


void SetNewPakLoadedBuffer(PakGlobalData *package) {
    const std::size_t index = PakSlotIndex(package);
    if (index >= nativePakSlotCount || !package->data || package->name.name != (SHEN_NAME(HPMa) + (static_cast<std::uint32_t>(index) << 24)))
        return;
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    task->field70 = reinterpret_cast<std::uintptr_t>(package);
}

void pakLoad(PakGlobalData *package) {
    const std::size_t index = PakSlotIndex(package);
    if (index >= nativePakSlotCount || !package->data || package->name.name != (SHEN_NAME(HPMa) + (static_cast<std::uint32_t>(index) << 24)))
        return;
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    task->linkedPakEntry = package;
}

void SetupPakSpace(std::int64_t mode) {
    PakLoadTaskData *const task = pakLoadTaskData ? pakLoadTaskData : static_cast<PakLoadTaskData *>(HLib::GetTaskParameter(pakLoadTask));
    task->timeBudget = mode == -1 ? -1 : 500;
}

const LoadedMapPackage *LoadPakPackageFromMemory(const char *packageName, std::span<const std::byte> packedPkf, std::span<const std::byte> packedPks) {
    if (!pakLoadTaskData || !packageName || !*packageName || packedPkf.empty() || packedPks.empty() || pakLoadTaskData->pakBuffer || pakLoadTaskData->pakt ||
        pakLoadTaskData->next) {
        return nullptr;
    }
    for (std::size_t index = 0; index < loadedMapPackageCount; ++index) {
        if (EqualsPackageName(loadedMapPackages[index], packageName)) {
            return pakRuntimes[index].phase == PakPhase::Ready ? &loadedMapPackages[index] : nullptr;
        }
    }
    const std::size_t nameLength = std::strlen(packageName);
    if (nameLength >= loadedMapPackages[0].name.size())
        return nullptr;
    std::size_t index = 0;
    while (index < nativePakSlotCount && pakSlots[index].name.name)
        ++index;
    if (index == nativePakSlotCount)
        return nullptr;

    LoadedMapPackage package{};
    std::memcpy(package.name.data(), packageName, nameLength + 1);
    if (!InflateGzip(packedPkf, package.pkfAllocation, package.pkfSize, SHEN_NAME(PAKF)) ||
        !ParseContainer({static_cast<const std::byte *>(package.pkfAllocation), package.pkfSize}, package.pkf) || package.pkf.signature != pakF) {
        if (package.pkfAllocation)
            HLib::FreeTaskData(package.pkfAllocation);
        return nullptr;
    }
    if (!InflateGzip(packedPks, package.pksAllocation, package.pksSize, SHEN_NAME(PAKS)) ||
        !ParseContainer({static_cast<const std::byte *>(package.pksAllocation), package.pksSize}, package.pks) || package.pks.signature != pakS) {
        HLib::FreeTaskData(package.pkfAllocation);
        if (package.pksAllocation)
            HLib::FreeTaskData(package.pksAllocation);
        return nullptr;
    }

    if (loadedMapPackageCount <= index)
        loadedMapPackageCount = index + 1;
    loadedMapPackages[index] = std::move(package);
    // This paired-memory admission always consumes both supplied containers.
    pakLoadTaskData->loadFlags = 0;
    pakRuntimes[index].phase = PakPhase::ParsingPakf;
    pakRuntimes[index].memoryPackage = true;
    PakGlobalData &slot = pakSlots[index];
    slot = {};
    slot.motionProperty = 0xffffffffU;
    slot.name = shen_name{make_shen_name('H', 'P', 'M', static_cast<char>('a' + index))};
    std::memcpy(slot.entryType.data(), packageName, nameLength + 1);
    slot.data = loadedMapPackages[index].pkfAllocation;
    slot.dataSize = loadedMapPackages[index].pkfSize;
    pakLoadTaskData->field68 = 0;
    pakLoadTaskData->pakt = &slot;
    HLib::ResolveDeferredCallback(pakLoadTask);
    while (index < loadedMapPackageCount && pakSlots[index].name.name && pakRuntimes[index].phase != PakPhase::Ready &&
        pakRuntimes[index].phase != PakPhase::Failed) {
        PakLoadTaskEntry_Callback(pakLoadTaskData);
    }
    return index < loadedMapPackageCount && pakRuntimes[index].phase == PakPhase::Ready ? &loadedMapPackages[index] : nullptr;
}

void LoadPak() {
    if (!pakLoadTaskData)
        return;
    while (pakLoadTaskData->pakBuffer || pakLoadTaskData->pakt || pakLoadTaskData->next || !HasThreadHandle(pakLoadTaskData->renderThreadHandle)) {
        d3t::ProcessRemappings();
        PakLoadingDiagnosticNullsub();
        PakLoadTaskEntry_Callback(pakLoadTaskData);
        FlushFramePacingRenderQueue();
        if (HasThreadHandle(pakLoadTaskData->renderThreadHandle))
            LockThreadHandle(pakLoadTaskData->renderThreadHandle);
    }
    LockThreadHandle(pakLoadTaskData->renderThreadHandle);
}

void FreePakLoadTask() {
    if (!pakLoadTask)
        return;


    LoadPak();
    for (std::size_t index = 0; index < nativePakSlotCount; ++index) {
        if (pakSlots[index].name.name && (pakSlots[index].flags & 0x10U) == 0)
            RemovePackage(index);
    }
#if !SHENMUE_IS_DC
    for (PakAfsPartition &partition : pakAfsPartitions) {
        if (!partition.afs)
            continue;
        if (--partition.references <= 0)
            FreeAfsArchive(partition.afs);
        partition.afs = nullptr;
        partition.folder[0] = 0;
        partition.filename[0] = 0;
        partition.references = 0;
    }
#endif
    HLib::DestroyTask(pakLoadTask);
#if SHENMUE_IS_PC
    if (detachedPakLoadTaskData)
        SetTaskHeapData(detachedPakLoadTaskData);
#else
    delete detachedPakLoadTaskData;
#endif
    detachedPakLoadTaskData = nullptr;
    pakLoadTask = nullptr;
    pakLoadTaskData = nullptr;
}

bool UnloadMapPackage(const char *packageName) {
    if (!packageName || !*packageName)
        return false;

    std::size_t removed = loadedMapPackageCount;
    for (std::size_t index = 0; index < loadedMapPackageCount; ++index) {
        if (EqualsPackageName(loadedMapPackages[index], packageName)) {
            removed = index;
            break;
        }
    }
    if (removed == loadedMapPackageCount)
        return false;

    if (pakRuntimes[removed].phase != PakPhase::Ready)
        return false;
    RemovePackage(removed);
    return true;
}

std::span<const LoadedMapPackage> GetLoadedMapPackages() {
    return {loadedMapPackages.data(), loadedMapPackageCount};
}

const LoadedMapPackage *GetNextReadySoundPackage(const LoadedMapPackage *previous) {


    if (!pakLoadTaskData)
        return nullptr;
    std::size_t index = 0;
    if (previous) {
        while (index < loadedMapPackageCount && &loadedMapPackages[index] != previous)
            ++index;
        if (index == loadedMapPackageCount)
            return nullptr;
        ++index;
    }
    for (; index < loadedMapPackageCount; ++index) {
        const PakGlobalData &slot = pakSlots[index];
        if (slot.name.name && slot.data && &slot != pakLoadTaskData->pakBuffer && &slot != pakLoadTaskData->pakt && &slot != pakLoadTaskData->next) {
            return &loadedMapPackages[index];
        }
    }
    return nullptr;
}

std::span<const std::byte> GetPackageSoundTable(const LoadedMapPackage &package) {


    if (!pakLoadTaskData)
        return {};
    for (std::size_t index = 0; index < loadedMapPackageCount; ++index) {
        if (&loadedMapPackages[index] != &package)
            continue;
        const PakGlobalData &slot = pakSlots[index];
        if (slot.name.name != make_shen_name('H', 'P', 'M', static_cast<char>('a' + index)) || &slot == pakLoadTaskData->pakBuffer ||
            &slot == pakLoadTaskData->pakt || &slot == pakLoadTaskData->next || !slot.soundTable)
            return {};
        return {slot.soundTable, pakRuntimes[index].soundTableSize};
    }
    return {};
}

std::span<const std::byte> FindLoadedMapPackageEntry(const char *filename) {
    for (std::size_t index = 0; index < loadedMapPackageCount; ++index) {
        if (const auto entry = loadedMapPackages[index].pks.Find(filename); !entry.empty()) {
            return entry;
        }
        if (const auto entry = loadedMapPackages[index].pkf.Find(filename); !entry.empty()) {
            return entry;
        }
    }
    return {};
}

std::uint32_t ResolveLoadedMapTexture(std::uint32_t idLow, std::uint32_t idHigh) {
    const NamedTextureSlot *const slot = FindNamedTexture(idLow, idHigh);
    return slot ? ResolveNamedTextureHandle(slot->index) : 0;
}

std::uint32_t AcquireModelTexture(std::uint32_t idLow, std::uint32_t idHigh, std::span<const std::byte> image) {
    const NamedTextureSlot *const slot = AcquireNamedTexture(idLow, idHigh, image, 0);
    return slot ? ResolveNamedTextureHandle(slot->index) : 0;
}

void ReleaseModelTexture(std::uint32_t idLow, std::uint32_t idHigh) {
    if (const NamedTextureSlot *const slot = FindNamedTexture(idLow, idHigh))
        ReleaseNamedTexture(slot->index);
}

std::int64_t ReadTexnTexture(const void *texn) {
    const std::byte *const bytes = static_cast<const std::byte *>(texn);
    if (ReadU32(bytes) != texN)
        return -1;
    const NamedTextureSlot *const slot = AcquireNamedTexture(ReadU32(bytes + 8), ReadU32(bytes + 12), {bytes, ReadU32(bytes + 4)}, 0x1c);
    if (slot)
        ApplySpriteTextureOverride(texn, ResolveNamedTextureHandle(slot->index));
    return slot ? slot->index : -1;
}

// @verified - p
std::uint32_t ReadTEXN_Recursive(const void *texn, std::int32_t *textures) {
    std::uint32_t count = 0;
    for (const std::byte *chunk = static_cast<const std::byte *>(texn); ReadU32(chunk) == texN; chunk += ReadU32(chunk + 4)) {
        *textures++ = static_cast<std::int32_t>(ReadTexnTexture(chunk));
        ++count;
    }
    return count;
}

// @verified - p
std::uint32_t TEXNReader(const void *data, std::int64_t size, std::int32_t *textures) {
    const std::byte *const end = static_cast<const std::byte *>(data) + size;
    std::uint32_t count = 0;
    for (const std::byte *chunk = static_cast<const std::byte *>(data); chunk < end; chunk += ReadU32(chunk + 4)) {
        if (!ReadU32(chunk))
            break;
        *textures++ = static_cast<std::int32_t>(ReadTexnTexture(chunk));
        ++count;
    }
    return count;
}

namespace {
std::int32_t textureMotionFlag{};
std::int16_t mstsValue0{};
std::int16_t mstsValue1{};
std::int16_t mstsValue2{};
}

// @verified - p
void TXMO_Reader(const void *txmo, std::int32_t entryPoint) {
    if (!txmo) {
        textureMotionFlag = 0;
        return;
    }
    const std::int32_t *entry = reinterpret_cast<const std::int32_t *>(static_cast<const std::byte *>(txmo) + 8);
    if (*entry != -1 && *entry != entryPoint) {
        do {
            if (*++entry == -1) {
                textureMotionFlag = 0;
                return;
            }
        } while (*entry != entryPoint);
    }
    textureMotionFlag = 1;
}

// @verified - p
void MSTS_Reader(std::int16_t value0, std::int16_t value1, std::int16_t value2) {
    mstsValue0 = value0;
    mstsValue1 = value1;
    mstsValue2 = value2;
}

// @verified - p
std::int32_t texGetMotionFlag() {
    return textureMotionFlag;
}

// @verified - p
void texSetMotionFlag(std::int32_t enabled) {
    textureMotionFlag = enabled;
}


// @verified - p
std::int32_t ReadNamedSpriteTexture(const NamedSpriteTextureRecord &record, const char *filename, std::size_t imageBytes) {
    std::uint32_t idLow{}, idHigh{};
    std::memcpy(&idLow, filename, sizeof(idLow));
    std::memcpy(&idHigh, filename + 4, sizeof(idHigh));
    bool created{};
    NamedTextureSlot *const slot = FindOrAllocateNamedTexture(idLow, idHigh, created);
    if (!slot)
        return -1;
    if (!created)
        return slot->index;
    const auto *const image =
        reinterpret_cast<const std::byte *>(reinterpret_cast<std::uintptr_t>(HLib::GetDynamicUseStoragePtr()) + static_cast<std::intptr_t>(record.imageOffset));
    LegacyEffectTexture texture{};
    if (InitializeLegacyEffectTexture(texture, record.width, record.height, static_cast<std::uint16_t>(record.format)) != 0 ||
        UploadLegacyEffectPvr(texture, {image - 0x1c, imageBytes + 0x1c}) != 0) {
        if (texture.handle) {
            d3t::InitTexture2DResource(static_cast<std::uint32_t>(texture.handle));
            --g_legacyEffectTextureCount;
        }
        ReleaseNamedTexture(slot->index);
        return -1;
    }
    namedTextureHandles[slot->index] = static_cast<std::uint32_t>(texture.handle);
    namedTextureEffectOwners[slot->index] = true;
    return slot->index;
}


// @verified - p
void RetireSceneNamedTextures() {
    NamedTextureSlot *slot = namedTextureBanks[0];
    while (slot) {
        NamedTextureSlot *const next = slot->next;
        d3t::InitTexture2DResource(namedTextureHandles[slot->index]);
        namedTextureHandles[slot->index] = 0;
        if (namedTextureEffectOwners[slot->index]) {
            --g_legacyEffectTextureCount;
            namedTextureEffectOwners[slot->index] = false;
        }
        slot->next = namedTextureFree;
        namedTextureFree = slot;
        slot->references = 0;
        slot->idLow = 0;
        slot->idHigh = 0;
        slot = next;
    }
    namedTextureBanks[0] = nullptr;
}

// @verified - p
std::uint32_t InitSceneEffects() {
    RetireSceneNamedTextures();
    return MarkLegacyEffectTextureStateDirty();
}

std::int64_t ReleaseNamedTexture(std::int32_t textureIndex) {
    NamedTextureSlot *previous = nullptr;
    NamedTextureSlot *&head = namedTextureBanks[namedTextureBank != 0];
    NamedTextureSlot *slot = head;
    while (slot && slot->index != textureIndex) {
        previous = slot;
        slot = slot->next;
    }
    if (!slot || slot->index < 0)
        return -1;
    if (--slot->references <= 0) {
        d3t::InitTexture2DResource(namedTextureHandles[slot->index]);
        namedTextureHandles[slot->index] = 0;
        if (namedTextureEffectOwners[slot->index]) {
            --g_legacyEffectTextureCount;
            namedTextureEffectOwners[slot->index] = false;
        }
        if (previous)
            previous->next = slot->next;
        else
            head = slot->next;
        slot->next = namedTextureFree;
        namedTextureFree = slot;
        slot->references = 0;
        slot->idLow = 0;
        slot->idHigh = 0;
    }
    return 0;
}

std::uint32_t ResolveNamedTextureHandle(std::int32_t textureIndex) {
    if (textureIndex < 0 || textureIndex >= static_cast<std::int32_t>(namedTextureSlots.size()))
        return 0;
    return namedTextureHandles[textureIndex];
}

// @verified-s2: 0x7ff61867a4f0 -> 0x7ff61867a270 -> 0x7ff618445260
std::int32_t AllocateGeneratedTextureSlot(std::uint32_t width, std::uint32_t height, std::uint16_t format) {
    if (!namedTexturesInitialized)
        InitializeNamedTextureSlots();
    if (!namedTextureFree || namedTextureFree->index < 0)
        return -1;
    const std::uint32_t id = static_cast<std::uint32_t>(namedTextureFree->index);
    NamedTextureSlot *slot = FindNamedTexture(id, UINT32_MAX);
    if (slot) {
        ++slot->references;
    } else {
        slot = namedTextureFree;
        namedTextureFree = slot->next;
        NamedTextureSlot *&head = namedTextureBanks[namedTextureBank != 0];
        slot->next = head;
        head = slot;
        slot->idLow = id;
        slot->idHigh = UINT32_MAX;
        slot->references = 1;
    }
    const std::uint32_t decodedFormat = d3t::DecodeS1LegacyTextureFormat(format);
    if (namedTextureHandles[slot->index] || decodedFormat == d3t::kInvalidLegacyTextureFormat) {
        ReleaseNamedTexture(slot->index);
        return -1;
    }
    const d3t::Texture2DDescription description{width, height, 1, 1, decodedFormat, 0, 0, 1, 0, 0, nullptr};
    namedTextureHandles[slot->index] = d3t::CreateTexture2D(&description);
    if (!namedTextureHandles[slot->index]) {
        ReleaseNamedTexture(slot->index);
        return -1;
    }
    return slot->index;
}

// @verified-s2: 0x7ff61844fe00 -> 0x7ff618424760; caller retains the generated slot.
bool UploadGeneratedTextureSlot(std::int32_t textureIndex, const void *rgbaPixels) {
    const std::uint32_t handle = ResolveNamedTextureHandle(textureIndex);
    d3t::Texture2DDescription description{};
    if (!handle || !d3t::GetTexture2DDescription(handle, &description))
        return false;
    description.initialData = rgbaPixels;
    return d3t::ReplaceTexture2DResource(handle, &description);
}

// @verified-s2: 0x7ff618679830; SHAD and SURF are separate native task allocations.
GeneratedTextureSurface *AllocateSHAD_SRUF(std::uint32_t width, std::uint32_t height, std::int32_t layout, std::int32_t pixelMode) {
    constexpr std::array<std::uint16_t, 3> families{0x0900, 0x0100, 0x8000};
    constexpr std::array<std::uint8_t, 6> modes{0, 1, 2, 3, 0x85, 0x89};
    if (layout >= 3 || pixelMode >= 6)
        return nullptr;
    const std::uint16_t format = families[layout] | modes[pixelMode];
    const std::int32_t slot = AllocateGeneratedTextureSlot(width, height, format);
    if (slot < 0)
        return nullptr;
    const std::uint32_t size = GetTextureStorageSize(format, static_cast<std::int32_t>(width), static_cast<std::int32_t>(height));
    void *const pixels = HLib::AllocateTaskData(size, SHEN_NAME(SHAD));
    std::memset(pixels, 0, size);
    if (layout == 2) {
        auto *const header = static_cast<std::uint32_t *>(pixels);
        header[0] = (SHEN_NAME(DDS_) & 0x00ffffffU) | 0x20000000U;
        header[1] = 124;
        header[2] = 4103;
        header[3] = height;
        header[4] = width;
        header[27] = 4096;
        const std::uint8_t mode = modes[pixelMode];
        if (mode == 1 || mode == 2 || mode == 0x85 || mode == 0x89) {
            header[2] = 4111;
            header[5] = (mode == 0x89 ? 4U : 2U) * width;
            header[19] = 32;
            header[20] = mode == 1 ? 0x40 : 0x41;
            header[22] = mode == 0x89 ? 32 : 16;
            if (mode == 1) {
                header[23] = 0xf800;
                header[24] = 0x07e0;
                header[25] = 0x001f;
            } else if (mode == 0x89) {
                header[23] = 0x00ff0000;
                header[24] = 0x0000ff00;
                header[25] = 0x000000ff;
                header[26] = 0xff000000;
            } else {
                header[23] = 0x0f00;
                header[24] = 0x00f0;
                header[25] = 0x000f;
                header[26] = 0xf000;
            }
        }
    }
    auto *const surface = static_cast<GeneratedTextureSurface *>(HLib::AllocateTaskData(sizeof(GeneratedTextureSurface), SHEN_NAME(SURF)));
    if (surface) {
        surface->width = static_cast<std::uint16_t>(width);
        surface->height = static_cast<std::uint16_t>(height);
        surface->layout = static_cast<std::uint8_t>(layout);
        surface->pixelMode = static_cast<std::uint8_t>(pixelMode);
        surface->mapCount = 0;
        surface->slots[0] = static_cast<std::int16_t>(slot);
        surface->slots[1] = -1;
        surface->pixels = pixels;
        surface->activeSlot = 0;
    }
    return surface;
}

// @verified-s2: 0x7ff618679e70
void *GetGeneratedSurfacePixels(GeneratedTextureSurface *surface) {
    ++surface->mapCount;
    return static_cast<std::byte *>(surface->pixels) + (surface->layout == 2 ? 128 : 0);
}

// @verified-s2: 0x7ff61867a1c0
void CommitGeneratedSurface(GeneratedTextureSurface *surface) {
    if (!surface || surface->mapCount-- != 1)
        return;
    const std::int32_t slot = surface->slots[surface->activeSlot];
    if (surface->layout == 2) {
        d3t::ReplaceDecodedImage(ResolveNamedTextureHandle(slot), surface->pixels, 0x100100U, true);
    } else {
        UploadGeneratedTextureSlot(slot, surface->pixels);
    }
}

// @verified-s2: 0x7ff618679a60
std::int32_t RetireGeneratedSurface(GeneratedTextureSurface *surface) {
    if (!surface)
        return -1;
    const std::int32_t slot = surface->slots[0];
    if (surface->slots[1] != -1)
        ReleaseNamedTexture(surface->slots[1]);
    HLib::FreeTaskData(surface->pixels);
    HLib::FreeTaskData(surface);
    return slot;
}

void SetNamedTextureBank(std::int32_t bank) {
    namedTextureBank = bank;
}

std::uint32_t ReplacePtxTexture(std::uint32_t textureIndex, const void *ptx) {
    if (textureIndex >= namedTextureSlots.size() || !namedTextureHandles[textureIndex])
        return textureIndex;
    const std::byte *const bytes = static_cast<const std::byte *>(ptx);
    const d3t::PvrHeader *const pvr = reinterpret_cast<const d3t::PvrHeader *>(bytes + 0x1c);
    const std::byte *pixels = bytes + 0x2c;
    if ((pvr->pixelFormat & 0x80U) != 0 && pvr->dataFormat == 0x80) {
        if (*pixels != std::byte{'D'})
            pixels += sizeof(std::uint32_t);
        d3t::ReplaceDecodedImage(namedTextureHandles[textureIndex], pixels, 0x100100U, true);
    } else {
        const std::span<const std::byte> image{bytes, ReadU32(bytes + 4)};
        const std::uint32_t handle = UploadTexn(image);
        if (!handle)
            return textureIndex;
        d3t::InitTexture2DResource(namedTextureHandles[textureIndex]);
        namedTextureHandles[textureIndex] = handle;
        for (std::size_t index = 0; index < loadedMapPackageCount; ++index) {
            for (PakTexture &texture : loadedMapPackages[index].textures) {
                if (texture.textureIndex == static_cast<std::int16_t>(textureIndex))
                    texture.handle = handle;
            }
        }
    }
    return textureIndex;
}

} // namespace shenmue::s2
