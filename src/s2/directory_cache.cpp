#include "s2/directory_cache.hpp"

#if SHENMUE_IS_DC
#include <sg_gd.h>
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include <string.h>
#define SHENMUE_DIRECTORY_STRCMP ::strcmp
#define SHENMUE_DIRECTORY_STRCPY ::strcpy
#else
#include <cstring>
#define SHENMUE_DIRECTORY_STRCMP std::strcmp
#define SHENMUE_DIRECTORY_STRCPY std::strcpy
#endif
#else
#include "s2/flag.hpp"
#include <cstring>
#endif
#include "s2/hlib.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/simm.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHENMUE_DIRECTORY_NULL 0
#else
#define SHENMUE_DIRECTORY_NULL nullptr
#endif

namespace shenmue {
namespace s2 {
namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
DirectoryCacheEntry directoryCache[9] = {};
#else
std::array<DirectoryCacheEntry, 13> directoryCache{};
#endif

} // namespace

// Native path canonicalization: one leading '/', repeated and trailing separators dropped, ASCII upper case.
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void NormalizeDirectoryPath(const char *path, char *normalized) {
    *normalized = '/';
    if (!path || !*path) {
        normalized[1] = '\0';
        return;
    }
    const char *in = path;
    char *out = normalized + 1;
    for (;;) {
        while (*in == '/')
            ++in;
        while (*in && *in != '/')
            *out++ = static_cast<char>(SIMM_UppercaseAscii(*in++));
        if (!*in || (*in == '/' && !in[1])) {
            *out = '\0';
            return;
        }
        *out++ = '/';
    }
}
#else
// @verified - pd
void NormalizeDirectoryPath(const char *path, char *normalized) {
    *normalized = '/';
    if (!path || !*path) {
        normalized[1] = '\0';
        return;
    }
    char *out = normalized + 1;
    for (;;) {
        char c = *path;
        if (!c)
            break;
        while (c == '/') {
            c = *++path;
            if (!c) {
                *out = '\0';
                return;
            }
        }
        while (*path != '/') {
            const std::uint32_t value = static_cast<std::uint32_t>(static_cast<std::int32_t>(*path++));
            *out++ = static_cast<char>(value - 'a' > 25U ? value : value - 32U);
            if (!*path) {
                *out = '\0';
                return;
            }
        }
        if (!*path || (*path == '/' && !path[1]))
            break;
        *out++ = '/';
    }
    *out = '\0';
}
#endif

// @verified - p
void ResetDirectoryCache(std::int32_t all) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    DirectoryCacheEntry *entry = directoryCache;
    for (std::int32_t count = sizeof(directoryCache) / sizeof(directoryCache[0]); count; --count, ++entry) {
        if (entry->retained != 0 && all == 0)
            continue;
        entry->allocation = SHENMUE_DIRECTORY_NULL;
        entry->path[0] = '\0';
    }
#else
    for (DirectoryCacheEntry &entry : directoryCache) {
        if (entry.retained != 0 && all == 0)
            continue;
#if SHENMUE_IS_DC
        entry.allocation = SHENMUE_DIRECTORY_NULL;
#else
        entry.directory = SHENMUE_DIRECTORY_NULL;
#endif
        entry.path[0] = '\0';
    }
#endif
}

#if SHENMUE_IS_DC

static GDFS_DIRREC LoadDiscDirectory(const char *path, void *allocation, std::int32_t entryCount);

// @verified - d
void InitializeDirectoryCache() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    DirectoryCacheEntry *cursor = directoryCache;
    for (std::int32_t count = sizeof(directoryCache) / sizeof(directoryCache[0]); count; --count, ++cursor) {
        DirectoryCacheEntry &entry = *cursor;
#else
    for (DirectoryCacheEntry &entry : directoryCache) {
#endif
        entry.allocation = SHENMUE_DIRECTORY_NULL;
        entry.path[0] = '\0';
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void *GetCachedDiscDirectory(const char *path) {
    const DirectoryCacheEntry *entry = directoryCache;
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    for (std::int32_t count = sizeof(directoryCache) / sizeof(directoryCache[0]); count; --count, ++entry) {
        if (!SHENMUE_DIRECTORY_STRCMP(normalized, entry->path))
            return entry->directory;
    }
    return SHENMUE_DIRECTORY_NULL;
}
#else
// @verified - d
void *GetCachedDiscDirectory(const char *path) {
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    for (const DirectoryCacheEntry &entry : directoryCache) {
        if (!SHENMUE_DIRECTORY_STRCMP(normalized, entry.path))
            return entry.directory;
    }
    return SHENMUE_DIRECTORY_NULL;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AllocateDIRE(const char *path, std::int32_t retained, std::int32_t entryCount) {
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    if (entryCount < 1)
        entryCount = 1;
    DirectoryCacheEntry *entry = directoryCache;
    std::int32_t count;
    for (count = sizeof(directoryCache) / sizeof(directoryCache[0]); count; --count, ++entry) {
        if (!SHENMUE_DIRECTORY_STRCMP(entry->path, normalized))
            return;
    }
    for (std::int32_t capacity = entryCount; capacity <= 512; capacity += 25) {
        void *const allocation = HLib::AllocateTaskData(static_cast<std::size_t>(capacity) * 44 + 16, DIRECTORY_CACHE_MEMORY);
        GDFS_DIRREC directory = LoadDiscDirectory(normalized, allocation, capacity);
        if (directory) {
            entry = directoryCache;
            for (count = sizeof(directoryCache) / sizeof(directoryCache[0]); count; --count, ++entry) {
                if (!entry->allocation) {
                    entry->allocation = allocation;
                    SHENMUE_DIRECTORY_STRCPY(entry->path, normalized);
                    entry->directory = directory;
                    entry->retained = retained;
                    return;
                }
            }
        }
        HLib::FreeTaskData(allocation);
    }
}
#else
// @verified - d
void AllocateDIRE(const char *path, std::int32_t retained, std::int32_t entryCount) {
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    if (entryCount < 1)
        entryCount = 1;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    for (std::size_t index = 0; index < sizeof(directoryCache) / sizeof(directoryCache[0]); ++index) {
        const DirectoryCacheEntry &entry = directoryCache[index];
#else
    for (const DirectoryCacheEntry &entry : directoryCache) {
#endif
        if (!SHENMUE_DIRECTORY_STRCMP(entry.path, normalized))
            return;
    }
    for (; entryCount <= 512; entryCount += 25) {
        void *const allocation = HLib::AllocateTaskData(static_cast<std::size_t>(entryCount) * 44 + 16, DIRECTORY_CACHE_MEMORY);
        GDFS_DIRREC directory = LoadDiscDirectory(normalized, allocation, entryCount);
        if (directory) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            for (std::size_t index = 0; index < sizeof(directoryCache) / sizeof(directoryCache[0]); ++index) {
                DirectoryCacheEntry &entry = directoryCache[index];
#else
            for (DirectoryCacheEntry &entry : directoryCache) {
#endif
                if (!entry.allocation) {
                    entry.allocation = allocation;
                    SHENMUE_DIRECTORY_STRCPY(entry.path, normalized);
                    entry.directory = directory;
                    entry.retained = retained;
                    return;
                }
            }
        }
        HLib::FreeTaskData(allocation);
    }
}
#endif

static GDFS_DIRREC LoadDiscDirectory(const char *path, void *allocation, std::int32_t entryCount) {
    GDFS_DIRREC directory = gdFsCreateDirhn(allocation, entryCount);
    if (gdFsLoadDir(path, directory))
        directory = SHENMUE_DIRECTORY_NULL;
    return directory;
}

#else

// PC v1.07 keeps only the lookup: no entry is ever admitted, so every call ends in the native allocator probe and nullsub.
// @verified - p
void AllocateDIRE(const char *path, std::int32_t, std::int32_t entryCount) {
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    const std::int32_t count = entryCount >= 1 ? entryCount : 1;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    for (std::size_t index = 0; index < sizeof(directoryCache) / sizeof(directoryCache[0]); ++index) {
        const DirectoryCacheEntry &entry = directoryCache[index];
#else
    for (const DirectoryCacheEntry &entry : directoryCache) {
#endif
        const unsigned char *cached = reinterpret_cast<const unsigned char *>(entry.path);
        const unsigned char *wanted = reinterpret_cast<const unsigned char *>(normalized);
        std::int32_t difference;
        for (;;) {
            difference = static_cast<std::int32_t>(*cached) - static_cast<std::int32_t>(*wanted);
            if (difference != 0 || *wanted == 0)
                break;
            ++cached;
            ++wanted;
        }
        if (difference == 0)
            return;
    }
    if (count <= 512) {
        std::uint64_t size = static_cast<std::uint64_t>(count) * 44U + 16U;
        for (std::uint32_t probe = static_cast<std::uint32_t>(512 - count) / 25U + 1U; probe != 0; --probe) {
            HLib::FreeTaskData(HLib::AllocateTaskData(size, DIRECTORY_CACHE_MEMORY));
            size += 1100U;
        }
    }
    nullsub();
}

void FreeDIRE(const char *path) {
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    for (DirectoryCacheEntry &entry : directoryCache) {
        if (std::strcmp(entry.path, normalized))
            continue;
        if (!entry.retained) {
            HLib::FreeTaskData(entry.directory);
            entry.directory = nullptr;
            entry.path[0] = '\0';
        }
        return;
    }
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC
#undef SHENMUE_DIRECTORY_STRCMP
#undef SHENMUE_DIRECTORY_STRCPY
#endif
#undef SHENMUE_DIRECTORY_NULL
