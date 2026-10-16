#include "s2/pati.hpp"
#include "s2/pak.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"
#include "logging.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {

std::array<PatiGameplayPlacement, 9> patiGameplayPlacements{{
    {{188.57000732421875f, 129.6999969482422f, 147.75f}, static_cast<std::int32_t>(PATI_HAND_MODEL.name)},
    {{223.30999755859375f, 129.6999969482422f, 177.8699951171875f}, 180},
    {{188.57000732421875f, 129.6999969482422f, 146.75f}, 270},
    {{188.57000732421875f, 128.8000030517578f, 146.64999389648438f}, 90},
    {{188.47000122070312f, 128.8000030517578f, 148.75f}, 310},
    {{223.30999755859375f, 128.8000030517578f, 177.8699951171875f}, 180},
    {{223.30999755859375f, 129.6999969482422f, 178.77000427246094f}, -1},
    {{222.39999389648438f, 128.8000030517578f, 179.27000427246094f}, 270},
    {{222.39999389648438f, 128.8000030517578f, 177.3699951171875f}, 0},
}};
namespace {
MT7Model patiModelReaders[PATI_MODEL_CAPACITY];
void *patiModelAllocations[PATI_MODEL_CAPACITY];
std::int32_t patiModelReaderCount;
float ReadPatiNumber(const char *&cursor) {
    char text[256];
    std::uint32_t count = 0;
    while (*cursor != ',' && count < 255)
        text[count++] = *cursor++;
    text[count] = 0;
    ++cursor;
    return static_cast<float>(std::atof(text));
}
void ReadPatiField(const char *&cursor, char *destination, std::uint32_t capacity, char delimiter) {
    std::uint32_t count = 0;
    while (*cursor != delimiter && count < capacity)
        destination[count++] = *cursor++;
    destination[count] = 0;
    ++cursor;
}
}

// @verified - p
std::uint32_t ParsePatiTable(PatiTableData *destination, std::span<const std::byte> source) {
    const char *cursor = reinterpret_cast<const char *>(source.data());
    const char *const end = cursor + source.size();
    std::uint32_t tableCount = 0;
    std::uint32_t boardCount = 0;
    shen_name area;
    std::uint8_t tableIndex;
    while (cursor < end) {
        const char token = *cursor++;
        if (token == '[') {
            std::memcpy(&area, cursor, sizeof(area));
            tableIndex = 0;
            cursor += 5;
        } else if (token == '(') {
            PatiTableEntry &table = destination->tables[tableCount];
            table.area = area;
            table.tableIndex = tableIndex;
            std::memcpy(table.tableName, cursor, 4);
            table.tableName[4] = 0;
            cursor += 5;
            table.position.x = ReadPatiNumber(cursor);
            table.position.y = ReadPatiNumber(cursor);
            table.position.z = ReadPatiNumber(cursor);
            table.rotation.x = static_cast<std::uint16_t>(static_cast<std::int32_t>((ReadPatiNumber(cursor) * 8192.0f) / 45.0f));
            table.rotation.y = static_cast<std::uint16_t>(static_cast<std::int32_t>((ReadPatiNumber(cursor) * 8192.0f) / 45.0f));
            table.rotation.z = static_cast<std::uint16_t>(static_cast<std::int32_t>((ReadPatiNumber(cursor) * 8192.0f) / 45.0f));
            std::memcpy(table.character, cursor, 4);
            table.character[4] = 0;
            cursor += 5;
            std::memcpy(table.npc, cursor, 4);
            table.npc[4] = 0;
            cursor += 5;
            std::memcpy(table.surface, cursor, 4);
            table.surface[4] = 0;
            cursor += 5;
            table.scaleX = ReadPatiNumber(cursor);
            table.scaleY = ReadPatiNumber(cursor);
            std::memcpy(table.gameOptions, cursor, 4);
            table.gameOptions[4] = 0;
            cursor += 5;
            std::memcpy(table.gameType, cursor, 4);
            table.gameType[4] = 0;
            cursor += 5;
            ReadPatiField(cursor, table.stake, 9, ',');
            ReadPatiField(cursor, table.payout, 9, ',');
            ReadPatiField(cursor, table.selectedBoard, 5, ')');
            table.firstBoard = boardCount;
            table.boardCount = 0;
            ++tableCount;
        } else if (token == '{') {
            PatiBoardEntry &board = destination->boards[boardCount];
            std::memcpy(board.character, cursor, 4);
            board.character[4] = 0;
            cursor += 5;
            board.positionX = ReadPatiNumber(cursor);
            board.size[0] = cursor[0];
            board.size[1] = 0;
            cursor += 2;
            std::memcpy(board.model, cursor, 8);
            board.model[8] = 0;
            cursor += 9;
            ReadPatiField(cursor, board.labelModel, 8, ',');
            ReadPatiField(cursor, board.prizes, 10, ',');
            ReadPatiField(cursor, board.prizeModes, 6, ',');
            ReadPatiField(cursor, board.prizeItems, 6, '}');
            ++boardCount;
            ++destination->tables[tableCount - 1].boardCount;
        }
    }
    reinterpret_cast<char *>(&destination->tables[tableCount])[0] = 0;
    destination->boards[boardCount].model[0] = 0;
    return boardCount;
}

// @verified - p
std::uint32_t PATI_TBL_Parser(PatiTableData *destination) {
    return ParsePatiTable(destination,
        {static_cast<const std::byte *>(LoadCurrentPakBinary(PATI_TABLE_FILENAME)), static_cast<std::size_t>(GetCurrentPakBinarySize(PATI_TABLE_FILENAME))});
}

MT7Model *LoadPatiModel(const char *name, std::int32_t expandedRadius, PatiModelCache *cache) {
    for (std::int32_t index = 0; index < cache->count; ++index) {
        const PatiModelCacheEntry &entry = cache->entries[index];
        if (!std::strcmp(entry.name, name) && entry.expandedRadius == expandedRadius)
            return entry.model;
    }
    if (cache->count + 1 >= static_cast<std::int32_t>(PATI_MODEL_CAPACITY))
        Shenmue_LogPrintf(stderr, "Error : Too many Pati Models\n");
    char filename[32], path[272];
    std::snprintf(filename, sizeof(filename), "%s.MT7", name);
    std::snprintf(path, sizeof(path), "model/pati/%s.MT7", name);
    void *source = nullptr;
    std::size_t size = 0;
    if (!LoadLegacySprite("model/pati", filename, PATI_MODEL_MEMORY, source, size))
        return nullptr;
    MT7Model &reader = patiModelReaders[patiModelReaderCount];
    patiModelAllocations[patiModelReaderCount++] = source;
    if (!ReadModelFormat7({static_cast<const std::byte *>(source), size}, path, reader, 0x22))
        return nullptr;
    BindMT7WritableRuntimeMeshes(reader, {static_cast<std::byte *>(source), size});
    PatiModelCacheEntry &entry = cache->entries[cache->count];
    std::memcpy(entry.name, name, 8);
    entry.name[8] = 0;
    entry.model = &reader;
    if (expandedRadius && !reader.nodes.empty() && reader.nodes[0].runtimeMesh) {
        std::byte *const mesh = reader.nodes[0].runtimeMesh;
        float radius;
        std::memcpy(&radius, mesh + 20, sizeof(radius));
        radius *= 8.0f;
        std::memcpy(mesh + 20, &radius, sizeof(radius));
    }
    entry.expandedRadius = expandedRadius;
    ++cache->count;
    return &reader;
}

void ReleasePatiRuntime() {
    ReleasePatiProps();
    for (std::int32_t index = 0; index < patiModelReaderCount; ++index) {
        ReleaseModelFormat7(patiModelReaders[index]);
        HLib::FreeTaskData(patiModelAllocations[index]);
        patiModelAllocations[index] = nullptr;
    }
    patiModelReaderCount = 0;
}

} // namespace shenmue::s2
