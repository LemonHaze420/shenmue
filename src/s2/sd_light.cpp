#include "s2/sd_light.hpp"

#include "d3t/content.hpp"
#include "d3t/render_handles.hpp"
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace shenmue::s2 {

SDLightDataHeader g_sdLightDataHeader{};
SDLightMetadataTable g_sdLightMetadata{};
SDLightOverrides g_sdLightOverrides{};
std::int32_t g_sdLightSceneId{};
shen_name g_sdLightArea{};
std::uint32_t g_sdShadowPassCount{};

namespace {
std::atomic<std::int32_t> sdLightReaderCount{};
std::atomic<std::uint8_t> sdLightWriteLock{};

void AcquireSDLightWrite() {
    for (;;) {
        while (sdLightReaderCount.load() != 0) {
        }
        std::uint8_t expected = 0;
        if (!sdLightWriteLock.compare_exchange_strong(expected, 1))
            continue;
        if (sdLightReaderCount.load() == 0)
            return;
        sdLightWriteLock.store(0);
    }
}

bool InsertSDLightMetadata(SDLightMetadataTable &table, std::uint64_t value, std::uint32_t hash);

// @verified - p
bool ResizeSDLightMetadata(SDLightMetadataTable &table, std::uint32_t capacity) {
    SDLightMetadataTable replacement{.values = static_cast<std::uint64_t *>(std::malloc(8ULL * capacity)),
        .hashes = static_cast<std::uint32_t *>(std::malloc(4ULL * capacity)),
        .links = static_cast<std::uint16_t *>(std::malloc(2ULL * capacity)),
        .capacity = capacity,
        .count = 0};
    if (!replacement.values || !replacement.hashes) {
        std::free(replacement.values);
        std::free(replacement.hashes);
        std::free(replacement.links);
        return false;
    }
    std::memset(replacement.values, 0, 8ULL * capacity);
    std::memset(replacement.hashes, 0, 4ULL * capacity);
    std::memset(replacement.links, 0, 2ULL * capacity);
    if (table.values && table.hashes) {
        for (std::uint32_t index = 0; index < table.capacity; ++index)
            if (table.hashes[index])
                InsertSDLightMetadata(replacement, table.values[index], table.hashes[index]);
        std::free(table.values);
        std::free(table.hashes);
        std::free(table.links);
    }
    table.values = replacement.values;
    table.hashes = replacement.hashes;
    table.links = replacement.links;
    table.capacity = replacement.capacity;
    return true;
}

// @verified - p
bool InsertSDLightMetadata(SDLightMetadataTable &table, std::uint64_t value, std::uint32_t hash) {
    if (!table.capacity || table.count >= table.capacity - (table.capacity >> 2U))
        if (!ResizeSDLightMetadata(table, table.capacity >= 64 ? table.capacity + (table.capacity >> 2U) : 64))
            return false;
    std::uint32_t slot = hash % table.capacity;
    std::uint32_t empty = UINT32_MAX;
    for (;;) {
        if (table.hashes[slot] == hash)
            return true;
        if (!table.hashes[slot]) {
            if (empty == UINT32_MAX)
                empty = slot;
            if (!table.links[slot]) {
                ++table.count;
                table.hashes[slot] = hash;
                table.values[slot] = value;
                return true;
            }
        }
        if (!table.links[slot])
            break;
        slot = (slot + table.links[slot]) % table.capacity;
    }
    if (empty == UINT32_MAX) {
        empty = (slot + 1U) % table.capacity;
        while (empty != slot && table.hashes[empty])
            empty = (empty + 1U) % table.capacity;
        if (empty == slot)
            return false;
        table.links[slot] = static_cast<std::uint16_t>(empty >= slot ? empty - slot : empty + table.capacity - slot);
    }
    ++table.count;
    table.hashes[empty] = hash;
    table.values[empty] = value;
    return true;
}
} // namespace

void InitializeSDLightMetadata() {
    AcquireSDLightWrite();
    if (!g_sdLightMetadata.capacity && !g_sdLightMetadata.count)
        ResizeSDLightMetadata(g_sdLightMetadata, 512);
    sdLightWriteLock.store(0);
}

void ReleaseSDLightMetadata() {
    AcquireSDLightWrite();
    std::free(g_sdLightMetadata.values);
    std::free(g_sdLightMetadata.hashes);
    std::free(g_sdLightMetadata.links);
    g_sdLightMetadata = {};
    sdLightWriteLock.store(0);
}

// @verified - p
void CompleteSDLightDataLoad(void *, std::uint32_t contentHandle, void *data, std::uint64_t, const char *) {
    AcquireSDLightWrite();
    if (g_sdLightMetadata.count) {
        for (std::uint32_t index = 0; index < g_sdLightMetadata.capacity; ++index) {
            g_sdLightMetadata.values[index] = 0;
            g_sdLightMetadata.hashes[index] = 0;
            g_sdLightMetadata.links[index] = 0;
        }
        g_sdLightMetadata.count = 0;
    }
    if (data) {
        std::memcpy(&g_sdLightDataHeader, data, sizeof(g_sdLightDataHeader));
        if (g_sdLightDataHeader.version == 1) {
            const std::byte *const records = static_cast<const std::byte *>(data) + g_sdLightDataHeader.recordsOffset;
            for (std::uint32_t index = 0; index < g_sdLightDataHeader.count; ++index) {
                std::uint64_t value;
                std::memcpy(&value, records + index * g_sdLightDataHeader.stride, sizeof(value));
                const std::uint32_t hash = static_cast<std::uint32_t>(value >> 32U);
                if (hash)
                    InsertSDLightMetadata(g_sdLightMetadata, value, hash);
            }
        }
    }
    d3t::ProcessThreadStates(contentHandle, 1);
    sdLightWriteLock.store(0);
}

// @verified - p
std::int64_t LoadSDLightMetadata(std::int32_t sceneId, shen_name area) {
    AcquireSDLightWrite();
    g_sdLightSceneId = sceneId;
    g_sdLightArea = area;
    sdLightWriteLock.store(0);
    const std::uint32_t renderHandle = d3t::d3t_AllocateRenderHandle();
    sdLightReaderCount.fetch_add(1);
    while (sdLightWriteLock.load() != 0) {
    }
    const char *const name = g_sdLightArea.characters;
    char areaName[16];
    std::snprintf(areaName, sizeof(areaName), "%c%c%c%c", name[0], name[1], name[2], name[3]);
    char path[272];
    std::snprintf(path, sizeof(path), "/SceneMetaData/%s/SDLightMetaData_%s.sdlightdata", areaName, areaName);
    sdLightReaderCount.fetch_sub(1);
    const d3t::D3TShaderLoadTask task{.path = path,
        .renderHandle = renderHandle,
        .reserved_0c = 0,
        .thread = {.callback = CompleteSDLightDataLoad, .data = nullptr},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    return d3t::PushShaderLoadToThreadQueue(&task);
}

// @verified - p
void InitializeSDLightFlags() {
    g_sdLightOverrides.count = 0;
    g_sdShadowPassCount = 2;
}

} // namespace shenmue::s2
