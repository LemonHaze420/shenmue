#pragma once

#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name DIRECTORY_CACHE_MEMORY{SHEN_NAME(DIRE)};



struct DirectoryCacheEntry {
    void *directory;
    char path[40];
    std::int32_t retained;
    std::int32_t reserved34;
};
static_assert(sizeof(DirectoryCacheEntry) == 0x38);
static_assert(offsetof(DirectoryCacheEntry, path) == 0x08);
static_assert(offsetof(DirectoryCacheEntry, retained) == 0x30);

void ResetDirectoryCache(std::int32_t all);
void AllocateDIRE(const char *path, std::int32_t unused, std::int32_t entryCount);

} // namespace shenmue::s2
