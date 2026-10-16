#pragma once
#include "s1/file_loader.hpp"
#include "s1/texture_loader.hpp"
#include "s1/texture_cache.hpp"
#include "s1/hlib.hpp"
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace shenmue::s1::ui_runtime_detail {

struct OwnedSpriteAsset {
    std::int32_t surface{-1};
    std::int32_t textureIdLow{};
    std::int32_t textureIdHigh{};
};

template <std::size_t Capacity> struct OwnedSpriteSet {
    std::array<OwnedSpriteAsset, Capacity> assets{};
    std::size_t count{};
    bool loadAttempted{};
};

template <std::size_t Capacity> void ReleaseSpriteSet(OwnedSpriteSet<Capacity> &set) {
    for (std::size_t index = 0; index < set.count; ++index) {
        OwnedSpriteAsset &asset = set.assets[index];
        if (asset.surface >= 0)
            PVR_FreeAllocatedSurfaceByIndex(asset.surface);
        asset = {};
    }
    set.count = 0;
    set.loadAttempted = false;
}

template <std::size_t Capacity> void ParseSpriteSet(OwnedSpriteSet<Capacity> &set, std::byte *bytes, std::size_t size) {
    std::size_t offset = 0;
    while (offset + 16 <= size && set.count < set.assets.size()) {
        std::uint32_t chunkTag = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&chunkTag, bytes + offset, sizeof(chunkTag));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (chunkTag != SHEN_NAME(TEXN) || chunkSize < 16 || chunkSize > size - offset) {
            break;
        }
        OwnedSpriteAsset asset;
        std::memcpy(&asset.textureIdLow, bytes + offset + 8, sizeof(asset.textureIdLow));
        std::memcpy(&asset.textureIdHigh, bytes + offset + 12, sizeof(asset.textureIdHigh));
        asset.surface = LoadTEXN(bytes + offset);
        if (asset.surface >= 0)
            set.assets[set.count++] = asset;
        offset += chunkSize;
    }
}

template <std::size_t Capacity> bool LoadSpriteSet(OwnedSpriteSet<Capacity> &set, const char *filename, std::int32_t token) {
    if (set.loadAttempted)
        return set.count != 0;
    set.loadAttempted = true;
    const std::size_t size = static_cast<std::size_t>(shenOpenFile("sprite", filename));
    void *const file = size ? ReadFile("sprite", filename, token) : nullptr;
    if (!file)
        return false;
    ParseSpriteSet(set, static_cast<std::byte *>(file), size);
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
    return set.count != 0;
}

template <std::size_t Capacity> const OwnedSpriteAsset *FindSpriteAsset(const OwnedSpriteSet<Capacity> &set, std::int32_t assetId) {
    for (std::size_t index = 0; index < set.count; ++index) {
        const OwnedSpriteAsset &asset = set.assets[index];
        if (asset.textureIdLow == assetId || asset.textureIdHigh == assetId) {
            return &asset;
        }
    }
    return nullptr;
}

template <std::size_t Capacity> const OwnedSpriteAsset *FindSpriteAsset(const OwnedSpriteSet<Capacity> &set, std::int32_t assetId, std::size_t fallbackIndex) {
    if (const OwnedSpriteAsset *const asset = FindSpriteAsset(set, assetId)) {
        return asset;
    }
    return fallbackIndex < set.count ? &set.assets[fallbackIndex] : nullptr;
}



struct UILegacyFile {
    std::vector<char> data;
    std::atomic<bool> loaded{false};
    bool consumed = false;
};

} // namespace shenmue::s1::ui_runtime_detail
