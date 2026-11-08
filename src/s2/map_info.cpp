#include "s2/map_info.hpp"

#include "s2/archive_data.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <string_view>

namespace shenmue::s2 {

std::uint64_t g_mapInfoSize = 0;

namespace {

constexpr std::array currentMapOperandNames{
    std::string_view{"AKK0"},
    std::string_view{"AR01"},
    std::string_view{"ARR0"},
    std::string_view{"ARQ0"},
    std::string_view{"AB00"},
    std::string_view{"WSSS"},
    std::string_view{"WKK0"},
    std::string_view{"WEE0"},
    std::string_view{"WN00"},
    std::string_view{"WTT0"},
    std::string_view{"WBB0"},
    std::string_view{"WRR0"},
    std::string_view{"SM00"},
    std::string_view{"QR00"},
    std::string_view{"????"},
    std::string_view{"Q200"},
    std::string_view{"Q300"},
    std::string_view{"????"},
    std::string_view{"QB00"},
    std::string_view{"QC10"},
    std::string_view{"QD10"},
    std::string_view{"????"},
    std::string_view{"????"},
    std::string_view{"QA10"},
    std::string_view{"QA14"},
    std::string_view{"QA12"},
    std::string_view{"QA13"},
    std::string_view{"QE10"},
    std::string_view{"QE11"},
    std::string_view{"QE12"},
    std::string_view{"????"},
    std::string_view{"ARSF"},
    std::string_view{"AS00"},
    std::string_view{"AK10"},
    std::string_view{"AK11"},
    std::string_view{"????"},
    std::string_view{"AKS1"},
    std::string_view{"ARA0"},
    std::string_view{"AA10"},
    std::string_view{"WA10"},
    std::string_view{"WA11"},
    std::string_view{"WEM1"},
    std::string_view{"WRS2"},
    std::string_view{"AR12"},
    std::string_view{"AR13"},
    std::string_view{"AR14"},
    std::string_view{"AR15"},
    std::string_view{"AB10"},
    std::string_view{"QD20"},
    std::string_view{"QF10"},
    std::string_view{"QF11"},
    std::string_view{"Q110"},
    std::string_view{"Q120"},
    std::string_view{"QF12"},
    std::string_view{"WSS4"},
    std::string_view{"WTTB"},
    std::string_view{"WTS3"},
    std::string_view{"WTK0"},
    std::string_view{"WTKB"},
    std::string_view{"WNS5"},
    std::string_view{"WNDA"},
    std::string_view{"WND0"},
    std::string_view{"????"},
    std::string_view{"WTR0"},
    std::string_view{"WTK2"},
    std::string_view{"????"},
    std::string_view{"WTK1"},
    std::string_view{"ARX6"},
    std::string_view{"ARXA"},
    std::string_view{"ARX7"},
    std::string_view{"AKS0"},
    std::string_view{"WSC1"},
    std::string_view{"WSC0"},
    std::string_view{"WNC0"},
    std::string_view{"????"},
    std::string_view{"????"},
    std::string_view{"WNS4"},
    std::string_view{"WR10"},
    std::string_view{"WTX0"},
    std::string_view{"WNNA"},
    std::string_view{"WNN6"},
    std::string_view{"WNN9"},
    std::string_view{"WNN7"},
    std::string_view{"WSS2"},
    std::string_view{"WSX9"},
    std::string_view{"WSS0"},
    std::string_view{"WSS5"},
    std::string_view{"WSS1"},
    std::string_view{"WSS4"},
    std::string_view{"WSS3"},
    std::string_view{"WRS1"},
    std::string_view{"WECF"},
    std::string_view{"ARCO"},
    std::string_view{"ARX0"},
    std::string_view{"ARK0"},
    std::string_view{"Q210"},
    std::string_view{"Q220"},
    std::string_view{"Q310"},
    std::string_view{"Q320"},
    std::string_view{"WB10"},
    std::string_view{"WB20"},
    std::string_view{"WNX7"},
    std::string_view{"QGBT"},
    std::string_view{"KESS"},
    std::string_view{"QAAQ"},
    std::string_view{"QAAR"},
    std::string_view{"Q330"},
    std::string_view{"Q130"},
    std::string_view{"Q140"},
    std::string_view{"QSFD"},
    std::string_view{"QSFC"},
    std::string_view{"QSFB"},
    std::string_view{"QSFA"},
    std::string_view{"QSB1"},
    std::string_view{"QB10"},
    std::string_view{"QB11"},
    std::string_view{"QCFE"},
    std::string_view{"QEFO"},
    std::string_view{"QEFP"},
    std::string_view{"QEFQ"},
    std::string_view{"QEFR"},
    std::string_view{"QEFS"},
    std::string_view{"QA15"},
    std::string_view{"WSY0"},
    std::string_view{"WTA0"},
    std::string_view{"AB20"},
    std::string_view{"AR30"},
    std::string_view{"WT10"},
    std::string_view{"WN10"},
    std::string_view{"ARKL"},
    std::string_view{"WEWE"},
    std::string_view{"WEEA"},
    std::string_view{"WN20"},
    std::string_view{"WEG1"},
    std::string_view{"WEG2"},
    std::string_view{"WEG3"},
    std::string_view{"AKB0"},
    std::string_view{"QTBB"},
    std::string_view{"WSKB"},
};
static_assert(currentMapOperandNames.size() == 139);

std::uint32_t ReadChunkName(const char name[4]) {
    std::uint32_t value{};
    std::memcpy(&value, name, sizeof(value));
    return value;
}

const MapInfoChunk *FindMapInfoChunkInternal(const char name[4], const void *mapInfo) {
    if (!name || !mapInfo || g_mapInfoSize < sizeof(MapInfoChunk)) {
        return nullptr;
    }
    const auto *const begin = static_cast<const std::byte *>(mapInfo);
    const auto *chunk = static_cast<const MapInfoChunk *>(mapInfo);
    const std::uint32_t wanted = ReadChunkName(name);
    for (;;) {
        const auto *const current = reinterpret_cast<const std::byte *>(chunk);
        if (current < begin || static_cast<std::uint64_t>(current - begin) + sizeof(MapInfoChunk) > g_mapInfoSize) {
            return nullptr;
        }
        if (chunk->name.name == wanted) {
            return chunk;
        }
        if (chunk->nextByteOffset == 0) {
            return nullptr;
        }
        const std::uint64_t byteOffset = static_cast<std::uint64_t>(chunk->nextByteOffset);
        if (byteOffset < sizeof(MapInfoChunk) || byteOffset > g_mapInfoSize - static_cast<std::uint64_t>(current - begin)) {
            return nullptr;
        }
        chunk = reinterpret_cast<const MapInfoChunk *>(current + byteOffset);
    }
}

} // namespace

void *SceneMapinfoReader(std::int32_t actNumber, shen_name areaName) {
    const std::array<char, 4> area{
        static_cast<char>(areaName.name),
        static_cast<char>(areaName.name >> 8U),
        static_cast<char>(areaName.name >> 16U),
        static_cast<char>(areaName.name >> 24U),
    };
    std::array<char, 64> path{};
    const int length = std::snprintf(path.data(), path.size(), "scene/%02d/%c%c%c%c/MAPINFO.BIN", actNumber, area[0], area[1], area[2], area[3]);
    if (length < 0 || static_cast<std::size_t>(length) >= path.size()) {
        return nullptr;
    }

    g_mapInfoSize = ArchiveData_GetFileSize(path.data());
    if (g_mapInfoSize == 0) {
        return nullptr;
    }
    void *const data = HLib::AllocateTaskData(g_mapInfoSize, SHEN_NAME(INFO));
    if (!data || !ArchiveData_Read(path.data(), 0, data, g_mapInfoSize)) {
        if (data) {
            HLib::FreeTaskData(data);
        }
        g_mapInfoSize = 0;
        return nullptr;
    }
    return data;
}

MapInfoChunk *FindMapInfoChunk(const char name[4], void *mapInfo) {
    return const_cast<MapInfoChunk *>(FindMapInfoChunkInternal(name, mapInfo));
}

const MapInfoChunk *FindMapInfoChunk(const char name[4], const void *mapInfo) {
    return FindMapInfoChunkInternal(name, mapInfo);
}

// @verified - p
MapInfoChunk *FindNativeChunk(const char name[4], MapInfoChunk *first) {
    std::uint32_t token;
    std::memcpy(&token, name, sizeof(token));
    for (;;) {
        if (first->name.name == token)
            return first;
        if (!first->nextByteOffset)
            return nullptr;
        first = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(first) + first->nextByteOffset);
    }
}

// @verified - p
std::uint32_t *GetNativeChunkSizeField(const char name[4], MapInfoChunk *first) {
    return reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uintptr_t>(FindNativeChunk(name, first)) + sizeof(std::uint32_t));
}

void MAPINFORead(void *mapInfo) {
    MapInfoChunk *const end = FindMapInfoChunk("END\0", mapInfo);
    if (end) {
        end->nextByteOffset = 0;
    }
}

bool GetCurrentMapOperandName(std::uint16_t index, shen_name &name) {
    if (index >= currentMapOperandNames.size()) {
        name = {};
        return false;
    }
    const std::string_view value = currentMapOperandNames[index];
    name = shen_name{make_shen_name(value[0], value[1], value[2], value[3])};
    return true;
}

bool ResolveCurrentMapOperand(std::uint16_t index, std::int32_t &value) {
    shen_name operandName{};
    if (!GetCurrentMapOperandName(index, operandName)) {
        value = 0;
        return true;
    }
    value = operandName.name == g_currentMap.name ? 1 : 0;
    return true;
}

} // namespace shenmue::s2
