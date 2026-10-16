#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <array>

namespace shenmue::s2 {

struct MT7Model;

inline constexpr shen_name PATI_MODEL_MEMORY{SHEN_NAME(PATI)};
inline constexpr char PATI_TABLE_FILENAME[] = "PATI_TBL";
inline constexpr shen_name PATI_HAND_MODEL{SHEN_NAME(hand)};


struct PatiGameplayPlacement {
    Vector3 position;
    std::int32_t yaw;
};
static_assert(sizeof(PatiGameplayPlacement) == 0x10);
static_assert(offsetof(PatiGameplayPlacement, yaw) == 0x0c);
extern std::array<PatiGameplayPlacement, 9> patiGameplayPlacements;
inline constexpr std::size_t PATI_TABLE_CAPACITY = 150, PATI_BOARD_CAPACITY = 300, PATI_MODEL_CAPACITY = 150;

#pragma pack(push, 1)
struct PatiTableEntry {
    shen_name area;
    std::uint8_t tableIndex;
    char tableName[5];
    std::byte reserved0a[2];
    Vector3 position;
    Vector3i rotation;
    char character[5];
    char npc[5];
    char surface[5];
    std::byte reserved33;
    float scaleX;
    float scaleY;
    char gameOptions[5];
    char gameType[5];
    char stake[10];
    char payout[10];
    char selectedBoard[6];
    std::byte reserved60[4];
    std::uint32_t firstBoard;
    std::int32_t boardCount;
};
struct PatiBoardEntry {
    char character[5];
    std::byte reserved05[3];
    float positionX;
    char size[2];
    char model[9];
    char labelModel[9];
    char prizes[11];
    char prizeModes[7];
    char prizeItems[7];
    std::byte reserved39[3];
};
#pragma pack(pop)
struct PatiTableData {
    PatiTableEntry tables[PATI_TABLE_CAPACITY];
    PatiBoardEntry boards[PATI_BOARD_CAPACITY];
};
static_assert(sizeof(PatiTableEntry) == 108);
static_assert(offsetof(PatiTableEntry, position) == 12);
static_assert(offsetof(PatiTableEntry, character) == 36);
static_assert(offsetof(PatiTableEntry, gameOptions) == 60);
static_assert(offsetof(PatiTableEntry, firstBoard) == 100);
static_assert(sizeof(PatiBoardEntry) == 60);
static_assert(offsetof(PatiBoardEntry, model) == 14);
static_assert(offsetof(PatiBoardEntry, prizes) == 32);
static_assert(offsetof(PatiBoardEntry, prizeItems) == 50);
static_assert(offsetof(PatiTableData, boards) == 16200);
static_assert(sizeof(PatiTableData) == 34200);

struct PatiPlacementNode {
    std::int32_t signature;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::int32_t meshOffset;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    const Matrix4x4 *transform;
};
static_assert(sizeof(PatiPlacementNode) == 64);
static_assert(offsetof(PatiPlacementNode, meshOffset) == 40);
static_assert(offsetof(PatiPlacementNode, parentOffset) == 52);
static_assert(offsetof(PatiPlacementNode, transform) == 56);

struct PatiModelCacheEntry {
    char name[16];
    MT7Model *model;
    std::int32_t expandedRadius;
    std::uint32_t reserved1c;
};
struct PatiModelCache {
    std::int32_t count;
    std::uint32_t reserved04;
    PatiModelCacheEntry entries[PATI_MODEL_CAPACITY];
};
static_assert(sizeof(PatiModelCacheEntry) == 32);
static_assert(offsetof(PatiModelCacheEntry, model) == 16);
static_assert(offsetof(PatiModelCacheEntry, expandedRadius) == 24);
static_assert(offsetof(PatiModelCache, entries) == 8);
static_assert(sizeof(PatiModelCache) == 4808);

std::uint32_t ParsePatiTable(PatiTableData *destination, std::span<const std::byte> source);
std::uint32_t PATI_TBL_Parser(PatiTableData *destination);
MT7Model *LoadPatiModel(const char *name, std::int32_t expandedRadius, PatiModelCache *cache);
void InitPati();
void ReleasePatiProps();
void ReleasePatiRuntime();

} // namespace shenmue::s2
