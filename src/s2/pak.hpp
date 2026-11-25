#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/pak_dc.hpp"
#else
#include "s2/hlib.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace shenmue::d3t {
struct FileState;
}

namespace shenmue::s2 {

struct PakGlobalData;
struct MT7Model;
struct MT7Node;
PakGlobalData *FindRetainingModelPackage(const MT7Model *model);
inline constexpr shen_name MODEL_PACKAGE_MEMORY{SHEN_NAME(PAKM)};
inline constexpr shen_name PACKAGE_CHARACTER_TABLE_TEMP{SHEN_NAME(PAKS)}, PACKAGE_CHARACTER_TABLE_MEMORY{SHEN_NAME(PAKC)};
inline constexpr shen_name PACKAGE_TEXTURE_INDEX_MEMORY{SHEN_NAME(PAKT)};
inline constexpr shen_name PROP_PACKAGE_MEMORY{SHEN_NAME(PAKP)};
inline constexpr shen_name HAND_MODEL_MEMORY{SHEN_NAME(HNDT)};
inline constexpr shen_name DEFAULT_HAND_MODEL_MEMORY{SHEN_NAME(HAND)};
inline constexpr shen_name HAND_BANK_EXTENSION{make_shen_name("BIN ")};
inline constexpr shen_name HAND_MESH_MEMORY{SHEN_NAME(HNDV)};
inline constexpr shen_name HAND_POSE_MEMORY{SHEN_NAME(HNDX)};

#pragma pack(push, 1)
struct PakLoadTaskData {
    shen_name name;
    std::int32_t unknown04;
    std::uint32_t flags;
    std::array<char, 0x1c> path;
    std::array<char, 8> nameStart;
    HLib::TaskData *data;
    std::uint32_t dataSize;
    std::uint32_t padding3c;
    PakGlobalData *linkedPakEntry;
    PakGlobalData *pakBuffer;
    PakGlobalData *pakt;
    PakGlobalData *next;
    std::uint32_t renderThreadHandle;
    std::uint32_t padding64;
    std::uint64_t field68;
    std::uint64_t field70;
    // Native request selection persists through completion: 0 = PKF then PKS,
    // 2 = PKF only, 4 = PKS only. Directory wrappers replace it before admission.
    std::uint32_t loadFlags;
    std::int32_t padding7c;
    std::int64_t timeBudget;
    std::uint32_t retryCount;
    std::int32_t padding8c;
};
#pragma pack(pop)

static_assert(sizeof(PakLoadTaskData) == 0x90);
static_assert(offsetof(PakLoadTaskData, path) == 0x0c);
static_assert(offsetof(PakLoadTaskData, data) == 0x30);
static_assert(offsetof(PakLoadTaskData, linkedPakEntry) == 0x40);
static_assert(offsetof(PakLoadTaskData, pakBuffer) == 0x48);
static_assert(offsetof(PakLoadTaskData, next) == 0x58);
static_assert(offsetof(PakLoadTaskData, renderThreadHandle) == 0x60);
static_assert(offsetof(PakLoadTaskData, field68) == 0x68);
static_assert(offsetof(PakLoadTaskData, loadFlags) == 0x78);
static_assert(offsetof(PakLoadTaskData, timeBudget) == 0x80);
static_assert(offsetof(PakLoadTaskData, retryCount) == 0x88);

struct PakContainerEntry {
    std::array<char, 8> stem;
    std::array<char, 4> extension;
    std::uint32_t relativeOffset;
    std::uint32_t size;
};

static_assert(sizeof(PakContainerEntry) == 0x14);

struct PakContainerView {
    std::span<const std::byte> bytes;
    std::span<const PakContainerEntry> entries;
    std::uint32_t signature{};
    std::uint32_t ipacOffset{};
    std::uint32_t textureCount{};

    std::span<const std::byte> Find(const char *filename) const;
};

struct NamedTextureSlot {
    std::int16_t index;
    std::int16_t references;
    std::uint32_t idLow;
    std::uint32_t idHigh;
    std::uint32_t reserved;
    NamedTextureSlot *next;
};
static_assert(sizeof(NamedTextureSlot) == 0x18);
static_assert(offsetof(NamedTextureSlot, references) == 2);
static_assert(offsetof(NamedTextureSlot, idLow) == 4);
static_assert(offsetof(NamedTextureSlot, next) == 0x10);

struct PakTexture {
    std::uint32_t idLow{};
    std::uint32_t idHigh{};
    std::uint32_t handle{};
    std::int16_t textureIndex{-1};
};
struct LoadedMapPackage {
#if SHENMUE_IS_DC
    std::array<char, 8> name{};
#else
    std::array<char, 24> name{};
#endif
    void *pksAllocation{};
    std::size_t pksSize{};
    PakContainerView pks;
    void *pkfAllocation{};
    std::size_t pkfSize{};
    PakContainerView pkf;
    std::vector<PakTexture> textures;
};

HLib::Task *PakLoadTaskEntry_Enqueue(std::uint8_t nextFunctionIndex, std::int32_t taskFlag);
bool IsPakLoadTaskReady();
// Native LoadFileFromFolder: the loaded or newly requested slot, or null while a load is in progress.
#if SHENMUE_IS_DC
PakGlobalData *LoadMapPackage(const char *folder, const char *packageName, void *destination = nullptr);
#else
PakGlobalData *LoadMapPackage(const char *folder, const char *packageName);
// Mask 0x2 takes precedence over 0x4; mask 0x1 sets package flag 0x10.
// PKS completion publishes linkedPakEntry; PKF-only completion does not.
PakGlobalData *ReadPakFileFromDirectory(const char *directory, const char *name, std::uint8_t flags);
#endif
bool PakLoadWait();
#if !SHENMUE_IS_DC
struct AfsArchiveSlot;
// PC FTBL254 and NPC humans use four 0x40-byte mounted AFS descriptors over the LoadAfs archive bank. Packages
// borrow the archive; the event caller must retire its packages before unmounting.
struct PakAfsPartition {
    AfsArchiveSlot *afs;
    std::array<char, 32> folder;
    std::array<char, 16> filename;
    std::int32_t references;
    std::uint32_t reserved3c;
};
static_assert(sizeof(PakAfsPartition) == 0x40);
static_assert(offsetof(PakAfsPartition, filename) == 0x28);
static_assert(offsetof(PakAfsPartition, references) == 0x38);
// Native LoadAFS, UnloadAFS and LoadFromAfsInternal (mode 0 is LoadFromAfs, mode 1 LoadFromHumansAfs, above 15 a status).
PakAfsPartition *MountPakAfs(const char *folder, const char *filename);
std::int64_t UnmountPakAfs(PakAfsPartition *partition);
PakGlobalData *RequestPakFromAfs(PakAfsPartition *partition, std::int32_t firstIndex, std::int16_t secondIndex, std::uint64_t mode);

namespace PakCategory {
enum Value : std::int32_t {
    Package = 0,
    Partition = 1,
    Task = 2,
    Reserved = 3,
    LoadPackageWithMode = 4,
    LoadAfsWithMode = 5,
};
}

namespace PakPackageOp {
enum Value : std::uint64_t {
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
enum Value : std::uint64_t {
    Mount = 0,
    Unmount = 1,
};
}

namespace PakTaskOp {
enum Value : std::uint64_t {
    GetFlags = 0,
    SetFlags = 1,
    LoadNamed = 2,
    IsCurrentNamed = 3,
    SetCurrentNamed = 4,
    SetCurrentAfs = 5,
};
}

// ECX category, RDX mode, R8 source, R9 target/index,
// stack DWORD secondary index (the AFS producer consumes its low signed WORD).
// Pointer results remain 64-bit; categories 0/11 and 2/0 sign-extend DWORDs.
// Category 3 and unsupported modes are genuine return-one branches.
std::int64_t EV_pakUtils(std::int32_t category, std::uint64_t mode, std::uintptr_t source, std::uintptr_t target, std::int32_t secondaryIndex = 0);
#endif
std::int32_t GetPakReadiness(PakGlobalData *package);
void DestroyPakFileAFS(PakGlobalData *package);
const LoadedMapPackage *LoadPakPackageFromMemory(const char *packageName, std::span<const std::byte> packedPkf, std::span<const std::byte> packedPks);
void pakLoadFlush();
void FreePakLoadTask();
bool UnloadMapPackage(const char *packageName);
PakGlobalData *GetPakLoadTaskDataPointer();
std::uint32_t GetPakLoadParams();
std::uint32_t GetPakTaskDataField();
PakGlobalData *pakLoadGetNext();
PakGlobalData *GetNextReadyModelPackage(PakGlobalData *previous);
PakGlobalData *FindPakByEntryName(const char *name);
void SetNewPakLoadedBuffer(PakGlobalData *package);
void pakLoad(PakGlobalData *package);
inline constexpr shen_name GZIP_MEMORY{SHEN_NAME(GZIP)};
void SetupPakSpace(std::int64_t mode);
void InitializeNamedTextureSlots();
void InitEffect();
std::int64_t GetPakLoadBudget();
// Native FileLoader borrows an entry from the selected HPM slot's PKS allocation.
void *FileLoader(PakGlobalData *package, const char *filename, shen_name extension);
std::span<const std::byte> FileLoaderView(PakGlobalData *package, const char *filename, shen_name extension);
std::int64_t GetPakFileSize(PakGlobalData *package, const char *filename, shen_name extension);
void *LoadCurrentPakBinary(const char *filename);
std::int64_t GetCurrentPakBinarySize(const char *filename);
MT7Model *LoadGameModel(PakGlobalData *package, const char *filename);
const MT7Model *FindLoadedModelNodeOwner(const MT7Node *node, std::size_t &index);
// Native package-context-first ECAM lookup; borrows PKS or current INFO.
std::uint32_t *pakFindEventCam(std::int32_t index);
// Native LoadFile; omitting size selects FTBL545's LoadFile_Wrapper boundary.
void *LoadLegacySceneFile(const char *directory, const char *filename, shen_name token, std::uint32_t *size = nullptr);
void *FileRead(const char *directory, const char *filename, shen_name token, std::uint32_t *size = nullptr);
bool DoesFileExist(const char *directory, const char *filename);
std::int64_t ReadLegacySceneFileIntoBuffer(d3t::FileState *file, void *buffer, std::uint64_t size);
std::int64_t AssignLegacySceneFilePath(d3t::FileState *file, const char *directory, const char *filename);
bool FlushLegacyFileReadAndRenderQueue(d3t::FileState *file);
std::int64_t LoadLegacyFiles(const char *directory, const char *filename, void *buffer, std::uint64_t size);

// Native LoadSprite. The caller owns the HLib allocation.
bool LoadLegacySprite(const char *directory, const char *filename, shen_name token, void *&allocation, std::size_t &size);

std::span<const LoadedMapPackage> GetLoadedMapPackages();
// Native ready-package iteration and first-SNDP view. Views borrow the
// package allocation; pending PACK buffers are excluded. Header is included.
const LoadedMapPackage *GetNextReadySoundPackage(const LoadedMapPackage *previous);
std::span<const std::byte> GetPackageSoundTable(const LoadedMapPackage &package);
std::span<const std::byte> FindLoadedMapPackageEntry(const char *filename);
// Borrowed DYNM child header from the first ready package.
const std::byte *FindPackageDynamicMap(const std::uint8_t name[4]);
std::uint32_t ResolveLoadedMapTexture(std::uint32_t idLow, std::uint32_t idHigh);
// Native MT7_TXT7_Reader admission and model-release decrement of one
// embedded texture ID; a package texture with the same ID is shared.
std::uint32_t AcquireModelTexture(std::uint32_t idLow, std::uint32_t idHigh, std::span<const std::byte> image);
void ReleaseModelTexture(std::uint32_t idLow, std::uint32_t idHigh);
// Native CreatePVRForDirectX gives the image loader this fixed source size rather than the chunk size.
inline constexpr std::size_t PVR_IMAGE_LOADER_SOURCE_SIZE = 0x100100;
// imageOffset locates the GBIX/PVRT image: 0x1c inside a PKF TEXN chunk, zero for an MT7 TXT7 entry.
std::uint32_t UploadTexn(std::span<const std::byte> texn, std::size_t imageOffset = 0x1c);

// Native TEXN admission returns a signed slot index, not a D3T handle. Every successful admission owns one reference.
std::int64_t ReadTexnTexture(const void *texn);
// Native ReadTEXN_Recursive: admits consecutive TEXN chunks into textures until a non-TEXN tag; returns the count.
std::uint32_t ReadTEXN_Recursive(const void *texn, std::int32_t *textures);
// Native TEXNReader: walks chunks up to data + size, stopping at a zero tag. Each chunk goes through the TEXN reader
// (a non-TEXN tag stores -1) and advances by its own size field. Returns the number of slots written.
std::uint32_t TEXNReader(const void *data, std::int64_t size, std::int32_t *textures);
// Native TXMO_Reader: an empty (-1-first) entry list or a listed entry point enables the texture-motion flag.
void TXMO_Reader(const void *txmo, std::int32_t entryPoint);
void MSTS_Reader(std::int16_t value0, std::int16_t value1, std::int16_t value2);
std::int32_t texGetMotionFlag();
void texSetMotionFlag(std::int32_t enabled);
std::int64_t ReleaseNamedTexture(std::int32_t textureIndex);
std::uint32_t ResolveNamedTextureHandle(std::int32_t textureIndex);
struct NamedSpriteTextureRecord {
    std::uint16_t width;
    std::uint16_t height;
    std::uint32_t format;
    std::int32_t imageOffset;
    std::int32_t textureIndex;
};
static_assert(sizeof(NamedSpriteTextureRecord) == 0x10);
static_assert(offsetof(NamedSpriteTextureRecord, imageOffset) == 8);
// MOVI records borrow a GBIX/PVRT allocation: imageOffset points at its
// pixels (+0x1c), relative to GetDynamicUseStoragePtr; imageBytes excludes that header.
std::int32_t ReadNamedSpriteTexture(const NamedSpriteTextureRecord &record, const char *filename, std::size_t imageBytes);
void RetireSceneNamedTextures();
std::uint32_t InitSceneEffects();
// Native generated surfaces share the named TEXN slot bank and own one reference.
std::int32_t AllocateGeneratedTextureSlot(std::uint32_t width, std::uint32_t height, std::uint16_t format);
bool UploadGeneratedTextureSlot(std::int32_t textureIndex, const void *rgbaPixels);
struct GeneratedTextureSurface {
    std::uint16_t width, height;
    std::uint8_t layout, pixelMode;
    std::uint16_t mapCount;
    std::int16_t slots[2];
    std::uint32_t reserved0c;
    void *pixels;
    std::uint8_t activeSlot;
    std::byte reserved19[7];
};
static_assert(sizeof(GeneratedTextureSurface) == 0x20);
static_assert(offsetof(GeneratedTextureSurface, pixels) == 0x10);
static_assert(offsetof(GeneratedTextureSurface, activeSlot) == 0x18);
GeneratedTextureSurface *AllocateSHAD_SRUF(std::uint32_t width, std::uint32_t height, std::int32_t layout, std::int32_t pixelMode);
void *GetGeneratedSurfacePixels(GeneratedTextureSurface *surface);
void CommitGeneratedSurface(GeneratedTextureSurface *surface);
// Retiring the scratch allocation transfers, rather than releases, slot zero.
std::int32_t RetireGeneratedSurface(GeneratedTextureSurface *surface);
void SetNamedTextureBank(std::int32_t bank);
// Native indexed update starts at +0x2c; the admitted PTX's TEXN size may already be cleared. Returns textureIndex on upload failure.
std::uint32_t ReplacePtxTexture(std::uint32_t textureIndex, const void *ptx);
// Native get_total_size_for_tex: a PVR image's byte size in the format, 0 for a family without one.
std::uint32_t GetPvrTextureSize(std::uint16_t format, std::int32_t width, std::int32_t height);
// Writes a PTX's PVR dimensions, and its DDS dimensions for the 0x8000 family; false for an unaligned PTX or a sizeless format.
bool SetPtxDimensions(std::byte *ptx, std::int32_t height, std::int32_t width, std::uint16_t format);

} // namespace shenmue::s2

#endif
