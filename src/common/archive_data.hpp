#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::archive_data {

struct ArchiveSourceDesc {
    std::uint32_t sourceIndex;
    std::uint32_t reserved04;
    std::uint64_t sourceOffset;
    std::uint32_t fileSize;
    std::uint16_t flags;
    std::uint16_t reserved16;
    const void *remapEntry;
};
static_assert(sizeof(ArchiveSourceDesc) == 0x20);
static_assert(offsetof(ArchiveSourceDesc, sourceOffset) == 8);
static_assert(offsetof(ArchiveSourceDesc, fileSize) == 0x10);
static_assert(offsetof(ArchiveSourceDesc, remapEntry) == 0x18);

inline constexpr ArchiveSourceDesc EMPTY_ARCHIVE_SOURCE{};
struct ArchiveDataLoadResult {
    std::uint32_t archiveCount;
    std::uint32_t entryCount;
    std::uint64_t packedSize;
};

std::uint32_t d3t_HashData(const void *data, std::uint32_t size);
std::uint32_t d3t_GenerateHash(const char *path);
char *FilepathCleaner(char *path);
bool ArchiveData_LoadArchives(const char *root, const char *backend, std::uint32_t type, ArchiveDataLoadResult *result = nullptr);
void ArchiveData_ClearArchives();
bool ArchiveData_HasArchives();
std::uint64_t ArchiveData_GetPathHash(const char *path);
std::uint64_t ArchiveData_GetFileSize(const char *path);

bool ArchiveData_IsPackedFile(const char *path);
void ArchiveData_GetSourceDesc(const char *path, ArchiveSourceDesc &source);
bool ArchiveData_Read(const char *path, std::uint64_t offset, void *destination, std::uint64_t size);

} // namespace shenmue::archive_data
