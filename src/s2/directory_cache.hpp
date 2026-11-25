#pragma once

#include "shenmue.h"
#include "common/types.hpp"

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <array>
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue { namespace s2 {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
using ::shenmue::shen_name;
#define DIRECTORY_CACHE_MEMORY shen_name(SHEN_NAME4('D', 'I', 'R', 'E'))
#else
inline constexpr shen_name DIRECTORY_CACHE_MEMORY{SHEN_NAME(DIRE)};
#endif

// The PC v1.07 table (13 x 0x38 bytes) is cleared and compared but never filled, so
// only the cleared pointer, the compared path and the retained flag are observable.
struct DirectoryCacheEntry {
#if SHENMUE_IS_DC
    void *allocation;
    char path[32];
    void *directory;
    std::int32_t retained;
#else
    void *directory;
    char path[40];
    std::int32_t retained;
    std::int32_t reserved34;
#endif
};
#if SHENMUE_IS_DC
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char DirectoryCacheEntrySizeCheck[sizeof(DirectoryCacheEntry) == 0x2c ? 1 : -1];
typedef char DirectoryCachePathOffsetCheck[offsetof(DirectoryCacheEntry, path) == 0x04 ? 1 : -1];
typedef char DirectoryCacheDirectoryOffsetCheck[offsetof(DirectoryCacheEntry, directory) == 0x24 ? 1 : -1];
typedef char DirectoryCacheRetainedOffsetCheck[offsetof(DirectoryCacheEntry, retained) == 0x28 ? 1 : -1];
#else
static_assert(sizeof(DirectoryCacheEntry) == 0x2c);
static_assert(offsetof(DirectoryCacheEntry, path) == 0x04);
static_assert(offsetof(DirectoryCacheEntry, directory) == 0x24);
static_assert(offsetof(DirectoryCacheEntry, retained) == 0x28);
#endif
void InitializeDirectoryCache();
void *GetCachedDiscDirectory(const char *path);
#else
static_assert(sizeof(DirectoryCacheEntry) == 0x38);
static_assert(offsetof(DirectoryCacheEntry, path) == 0x08);
static_assert(offsetof(DirectoryCacheEntry, retained) == 0x30);
#endif

void NormalizeDirectoryPath(const char *path, char *normalized);
void ResetDirectoryCache(std::int32_t all);
void AllocateDIRE(const char *path, std::int32_t unused, std::int32_t entryCount);
#if !SHENMUE_IS_DC
void FreeDIRE(const char *path);
#endif

} // namespace s2
} // namespace shenmue
