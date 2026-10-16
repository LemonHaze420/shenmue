#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

template <typename T> struct D3TRawArray {
    T *data;
    std::uint32_t count;
    std::uint32_t capacity;
};

struct D3TAssetUnique {
    std::uint64_t contentMd5Low;
    std::uint64_t contentMd5High;
    std::uint32_t textureHash;
    std::uint32_t fileSize;
};

struct D3TAssetLocation {
    std::uint32_t hasHashCollision;
    std::uint32_t hashCollision;
    std::uint32_t locationStringIndex;
    std::uint32_t locationHash;
    std::uint32_t uniqueIndex;
};

struct D3TAssetRemappingState {
    char root[260];
    bool hasFullLocationPaths;
    std::byte reserved_105[3];
    std::atomic<std::int32_t> state;
    std::uint32_t reserved_10c;
    D3TRawArray<char> collisionStrings;
    D3TRawArray<D3TAssetUnique> uniques;
    D3TRawArray<D3TAssetLocation> locations;
};

static_assert(sizeof(D3TRawArray<char>) == 0x10);
static_assert(sizeof(D3TAssetUnique) == 0x18);
static_assert(sizeof(D3TAssetLocation) == 0x14);
static_assert(offsetof(D3TAssetRemappingState, hasFullLocationPaths) == 0x104);
static_assert(offsetof(D3TAssetRemappingState, state) == 0x108);
static_assert(offsetof(D3TAssetRemappingState, collisionStrings) == 0x110);
static_assert(offsetof(D3TAssetRemappingState, uniques) == 0x120);
static_assert(offsetof(D3TAssetRemappingState, locations) == 0x130);
static_assert(sizeof(D3TAssetRemappingState) == 0x140);

extern D3TAssetRemappingState g_d3tAssetRemapping;

bool d3t_ReadAssetRemapping();
const D3TAssetUnique *LoadShadersForRemap(std::uint64_t info, const char *path, bool *sourceUsesRoot);
bool genRemapPath(std::uint64_t info, const char *path, char *outputPath, std::uint64_t unknown, bool copyRemap);
const char *FindHashedPathSuffix(const char *path);
int GenerateFullHashString(const char *source, char *outputPath);

} // namespace shenmue::d3t
