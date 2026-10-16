#include "s2/directory_cache.hpp"

#include "s2/flag.hpp"
#include "s2/hlib.hpp"

namespace shenmue::s2 {
namespace {

std::array<DirectoryCacheEntry, 13> directoryCache{};


// @verified - p
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

} // namespace

// @verified - p
void ResetDirectoryCache(std::int32_t all) {
    for (DirectoryCacheEntry &entry : directoryCache) {
        if (entry.retained != 0 && all == 0)
            continue;
        entry.directory = nullptr;
        entry.path[0] = '\0';
    }
}


// @verified - p
void AllocateDIRE(const char *path, std::int32_t, std::int32_t entryCount) {
    char normalized[64];
    NormalizeDirectoryPath(path, normalized);
    const std::int32_t count = entryCount >= 1 ? entryCount : 1;
    for (const DirectoryCacheEntry &entry : directoryCache) {
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

} // namespace shenmue::s2
