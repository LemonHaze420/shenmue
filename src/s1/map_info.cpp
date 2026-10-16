#include "s1/task/ctrl.hpp"

#include "s1/archive_data.hpp"
#include "s1/file_loader.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/hlib.hpp"
#include "s1/task/pack.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

namespace {

const std::byte *FindMapInfoChunkRaw(const std::byte *bytes, std::uint32_t size, std::uint32_t token) {
    if (!bytes || size < 8)
        return nullptr;

    std::uint32_t offset = 0;
    for (std::uint32_t guard = 0; guard < size / 8U + 1U && offset + 8U <= size; ++guard) {
        std::uint32_t current = 0;
        std::uint32_t next = 0;
        std::memcpy(&current, bytes + offset, sizeof(current));
        std::memcpy(&next, bytes + offset + 4U, sizeof(next));
        if (current == token) {
            if (next == 0U || (next >= 8U && next <= size - offset))
                return bytes + offset;
            return nullptr;
        }
        if (next < 8U || next > size - offset)
            break;
        offset += next;
    }
    return nullptr;
}

} // namespace

const std::byte *MAPINFO_FindChunkIn(const void *mapInfo, std::uint32_t mapInfoSize, std::uint32_t token) {
    return FindMapInfoChunkRaw(static_cast<const std::byte *>(mapInfo), mapInfoSize, token);
}

FileState *g_mapInfoFileState = nullptr;
std::uint32_t g_mapInfoSize = 0;

void MAPINFO_InitializeSceneParameters(const void *mapInfo) {
    float blockSize = 45.0f;
    std::uint32_t payloadSize = 0;
    if (const std::byte *const payload = MAPINFO_FindChunkPayloadIn(mapInfo, g_mapInfoSize, SHEN_NAME(BLKS), &payloadSize);
        payload && payloadSize >= sizeof(blockSize)) {
        std::memcpy(&blockSize, payload, sizeof(blockSize));
    }
    g_mapBlockScale = static_cast<float>(1.0 / static_cast<double>(blockSize));
}

const std::byte *MAPINFO_FindChunkPayloadIn(const void *mapInfo, std::uint32_t mapInfoSize, std::uint32_t token, std::uint32_t *payloadSize) {
    if (payloadSize)
        *payloadSize = 0;
    const auto *const bytes = static_cast<const std::byte *>(mapInfo);
    const std::byte *const chunk = MAPINFO_FindChunkIn(mapInfo, mapInfoSize, token);
    if (!chunk)
        return nullptr;

    const auto chunkOffset = static_cast<std::uint32_t>(chunk - bytes);
    std::uint32_t next = 0;
    std::memcpy(&next, chunk + 4, sizeof(next));
    const std::uint32_t chunkSize = next ? next : mapInfoSize - chunkOffset;
    if (chunkSize < 8U || chunkSize > mapInfoSize - chunkOffset)
        return nullptr;
    if (payloadSize)
        *payloadSize = chunkSize - 8U;
    return chunk + 8U;
}

const std::byte *MAPINFO_FindChunkPayload(std::uint32_t token, std::uint32_t *payloadSize) {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr) {
        if (payloadSize)
            *payloadSize = 0;
        return nullptr;
    }
    const auto *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
    return MAPINFO_FindChunkPayloadIn(control->mapInfo, g_mapInfoSize, token, payloadSize);
}

std::uint32_t MAPINFO_ApplyDynamicMapDisableChunk() {
    std::uint32_t payloadSize = 0;
    const std::byte *const payload = MAPINFO_FindChunkPayload(SHEN_NAME(MAPD), &payloadSize);
    if (!payload)
        return 0;
    std::uint32_t disabled = 0;
    for (std::uint32_t offset = 0; offset + sizeof(std::int32_t) <= payloadSize; offset += sizeof(std::int32_t)) {
        std::int32_t mapIndex = -1;
        std::memcpy(&mapIndex, payload + offset, sizeof(mapIndex));
        if (mapIndex == -1)
            break;
        if (mapIndex < 0 || mapIndex >= static_cast<std::int32_t>(g_dynamicMapSlots.size())) {
            continue;
        }

        g_dynamicMapSlots[static_cast<std::size_t>(mapIndex)].renderEnabled = 0;
        ++disabled;
    }
    return disabled;
}

void MAPINFO_Release() {
    g_mapInfoFileState = nullptr;
    g_mapInfoSize = 0;
}

void *MAPINFO_Load() {
    FileState *const file = Archive_GetNextUnusedFileState();
    const char areaCharacters[4]{
        static_cast<char>(g_areaId),
        static_cast<char>(g_areaId >> 8),
        static_cast<char>(g_areaId >> 16),
        static_cast<char>(g_areaId >> 24),
    };
    char directory[64];
    std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_sceneId, areaCharacters[0], areaCharacters[1], areaCharacters[2], areaCharacters[3]);

    constexpr char filename[] = "MAPINFO.BIN";
    PrepareDirectoryPathForFile(file, directory, filename);
    const std::uint64_t size = OpenFileInternal(file->path);
    void *const data = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(size), static_cast<std::int32_t>(SHEN_NAME(INFO)));
    OpenFile(file, data, static_cast<std::int32_t>(size));
    g_mapInfoFileState = file;
    g_mapInfoSize = static_cast<std::uint32_t>(size);
    return data;
}

} // namespace shenmue::s1
