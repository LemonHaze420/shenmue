#pragma once

#include "d3t/archive_data.hpp"
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

using PlatformFileHandle = std::intptr_t;

inline constexpr std::uint8_t AIF_Urgent = 0x20;
inline constexpr std::uint64_t kArchiveBufferBudget = 150U * 1024U * 1024U;
inline constexpr std::uint64_t kArchiveSourceDistanceCost = 0xa00000000ULL;
enum ArchiveJobState : std::int32_t {
    AJS_Free = 0,
    AJS_Reserved = 1,
    AJS_Queued = 2,
    AJS_Claimed = 3,
    AJS_Pending = 4,
};

enum ArchiveOpenFlag : std::uint8_t {
    AOF_Read = 0x01,
    AOF_Write = 0x02,
    AOF_Truncate = 0x04,
    AOF_SkipDecompression = 0x10,
};

inline constexpr std::uintptr_t ARCHIVE_IO_PENDING = 0x103;

struct ArchiveOverlapped {
    std::uintptr_t internal;
    std::uintptr_t transferred;
    std::uint64_t offset;
    PlatformFileHandle event;
};
static_assert(sizeof(ArchiveOverlapped) == 0x20);
static_assert(offsetof(ArchiveOverlapped, transferred) == 8 && offsetof(ArchiveOverlapped, offset) == 16 && offsetof(ArchiveOverlapped, event) == 24);
struct ArchiveHandleEntry {
    char path[0x104];
    std::uint32_t handle;
    ArchiveSourceDesc source;
    std::uint8_t accessMode;
    std::uint8_t archiveLookupFlag;
    std::byte reserved_12a[6];
    void *bufferedData;
    std::uint64_t bufferedDataSize;
    std::uint8_t preserveBufferedData;
    std::byte reserved_141[7];
    std::uint8_t readSucceeded;
    std::byte reserved_149[7];
    PlatformFileHandle platformHandle;
    ArchiveOverlapped overlapped;
};

struct ArchiveHandleTable {
    std::array<ArchiveHandleEntry, 2048> entries;
    std::array<std::uint32_t, 2048> handleIds;
};

struct AsyncArchiveRequest {
    std::atomic<std::int32_t> state;
    std::uint32_t archiveHandle;
    std::uint8_t flags;
    char path[0x107];
    ArchiveSourceDesc source;
    std::uint64_t offset;
    std::uint64_t size;
    std::int32_t operation;
    std::byte reserved_144[4];
};

struct LargeFileHandle {
    std::int64_t field_00;
    std::int32_t entryIndex;
    std::int32_t fileId;
    std::int64_t offsetBytes;
};

struct LargeFileTocEntry {
    std::uint32_t offset;
    std::uint32_t size;
};

struct LargeFileDataEntry {
    std::uint8_t inUse;
    char cacheKey[0x103];
    std::uint32_t archiveHandle;
    LargeFileHandle handle;
    char openPath[0x108];
    std::uint32_t tocHeader;
    std::uint32_t tocEntryCount;
    LargeFileTocEntry *tocEntries;
    std::int32_t childReferenceCount;
    std::int32_t reserved_23c;
    LargeFileDataEntry *parent;
    std::int32_t tocIndex;
    std::int32_t reserved_24c;
};

static_assert(offsetof(ArchiveHandleEntry, handle) == 0x104 && offsetof(ArchiveHandleEntry, source) == 0x108);
static_assert(offsetof(ArchiveHandleEntry, bufferedData) == 0x130);
static_assert(offsetof(ArchiveHandleEntry, accessMode) == 0x128);
static_assert(offsetof(ArchiveHandleEntry, archiveLookupFlag) == 0x129);
static_assert(offsetof(ArchiveHandleEntry, bufferedDataSize) == 0x138);
static_assert(offsetof(ArchiveHandleEntry, preserveBufferedData) == 0x140);
static_assert(offsetof(ArchiveHandleEntry, readSucceeded) == 0x148);
static_assert(offsetof(ArchiveHandleEntry, platformHandle) == 0x150);
static_assert(offsetof(ArchiveHandleEntry, overlapped) == 0x158);
static_assert(sizeof(ArchiveHandleEntry) == 0x178);
static_assert(offsetof(ArchiveHandleTable, handleIds) == 0xbc000);
static_assert(offsetof(AsyncArchiveRequest, path) == 9);
static_assert(offsetof(AsyncArchiveRequest, source) == 0x110);
static_assert(offsetof(AsyncArchiveRequest, offset) == 0x130);
static_assert(offsetof(AsyncArchiveRequest, operation) == 0x140);
static_assert(sizeof(AsyncArchiveRequest) == 0x148);
static_assert(sizeof(LargeFileHandle) == 0x18);
static_assert(offsetof(LargeFileHandle, entryIndex) == 8);
static_assert(sizeof(LargeFileDataEntry) == 0x250);
static_assert(offsetof(LargeFileDataEntry, archiveHandle) == 0x104);
static_assert(offsetof(LargeFileDataEntry, handle) == 0x108);

extern ArchiveHandleTable g_archiveHandles;
extern std::atomic<std::int32_t> g_archiveHandleReaders;
extern std::atomic<std::uint8_t> g_archiveHandleWriter;
extern std::uint32_t g_invalidArchiveHandle;
extern std::array<AsyncArchiveRequest, 2048> g_asyncArchiveRequests;
extern std::array<LargeFileDataEntry, 2048> g_largeFileData;

void LoadContent(std::uint16_t threadIndex);
AsyncArchiveRequest *GetBestArchiveIoJob(const ArchiveSourceDesc &source);
ArchiveHandleEntry *Archive_GetHandle(std::uint32_t handle);
bool Archive_DoesHandleExist(std::uint32_t handle);
std::uint32_t Archive_AllocateHandle(std::uint8_t archiveLookupFlag);
std::int32_t Archive_ClosePlatformHandle(std::uint32_t handle);
void *Archive_GetHandleBuffer(std::uint32_t handle, std::uint64_t size);
void *Archive_BindBuffer(std::uint32_t handle, void *destination, std::uint64_t size, bool preserveBuffer);
void *Archive_DetachBuffer(std::uint32_t handle);
void Archive_QueueRead(std::uint32_t handle, std::int32_t operation, const char *path, std::uint64_t offset, std::uint64_t size, std::uint8_t flags);
bool Archive_HasPendingRead(std::uint32_t handle);
const char *GetFileExtension(const char *path);
bool Archive_FileExists(const char *path);
void *LoadLegacyFile(const char *path, std::uint64_t *size);
void ReadFile_0(std::uint64_t unused, std::uint32_t *fileState);
void ReadCachedFile(void *file);
std::int64_t GetFileFromCachedBuffer(LargeFileHandle *handle, std::uint64_t blockCount, void *destination, std::int32_t forceWait);
std::int64_t LargeFile_LoadADFSArchiveToc(std::int32_t fileId, const char *path);
std::int32_t CheckFileInfo(void *file);
LargeFileHandle *LargeFile_OpenPathHandle(const char *path);
LargeFileHandle *LargeFile_OpenTocEntryHandle(std::int32_t fileId, std::int32_t tocIndex);
std::int64_t LargeFile_GetSizeInBlocks(LargeFileHandle *handle);

} // namespace shenmue::d3t
