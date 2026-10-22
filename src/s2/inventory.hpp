#pragma once

#include "s2/save.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

enum ItemId : std::uint32_t {
    ITEM_AKIRA1 = 4000,
    ITEM_AKIRA2 = 4001,
    ITEM_JACKY1 = 4002,
    ITEM_JACKY2 = 4003,
    ITEM_SARAH1 = 4004,
    ITEM_SARAH2 = 4005,
    ITEM_LAU1 = 4006,
    ITEM_LAU2 = 4007,
    ITEM_PAI1 = 4008,
    ITEM_PAI2 = 4009,
    ITEM_WOLF1 = 4010,
    ITEM_WOLF2 = 4011,
    ITEM_JEFFRY1 = 4012,
    ITEM_JEFFRY2 = 4013,
    ITEM_KAGE1 = 4014,
    ITEM_KAGE2 = 4015,
    ITEM_DURAL_SILVER = 4016,
    ITEM_DURAL_GOLD = 4017,
};
struct S1ImportedItem {
    std::int16_t index, item;
};
static_assert(sizeof(S1ImportedItem) == 4);
inline constexpr S1ImportedItem S1_IMPORTED_ITEMS[]{
    {.index = 4, .item = 3},
    {.index = 3, .item = 2},
    {.index = 5, .item = 4},
    {.index = 6, .item = 5},
    {.index = 2, .item = 1},
    {.index = 8, .item = 8005},
    {.index = 7, .item = 6},
    {.index = 9, .item = 8014},
    {.index = 16, .item = 8013},
    {.index = 40, .item = 7},
    {.index = 10, .item = 8006},
    {.index = 11, .item = 8007},
    {.index = 12, .item = 8008},
    {.index = 13, .item = 8009},
    {.index = 14, .item = 8010},
    {.index = 15, .item = 8011},
    {.index = 17, .item = 3000},
    {.index = 19, .item = 3001},
    {.index = 21, .item = 3002},
    {.index = 23, .item = 3003},
    {.index = 25, .item = 3004},
    {.index = 27, .item = 3005},
    {.index = 29, .item = 3006},
    {.index = 31, .item = 3007},
    {.index = 33, .item = 3008},
    {.index = 35, .item = 3010},
    {.index = 36, .item = 3009},
    {.index = 52, .item = 4258},
    {.index = 53, .item = 4257},
    {.index = 55, .item = 4259},
    {.index = 56, .item = 4260},
    {.index = 37, .item = 4261},
    {.index = 38, .item = 4262},
};

std::int64_t ImportS1CapsuleStock(const std::uint8_t *source);
std::int64_t ImportS1KeyItems(const std::uint8_t *source);
inline constexpr std::uint32_t InventoryPresentationTypes[10]{5, 5, 5, 9, 8, 8, 6, 5, 0x900, 4};
extern std::int32_t g_inventoryStockBiasEnabled;


struct InventoryItemDefinition {
    std::uint16_t icon;
    std::uint16_t id;
    std::int16_t minimumDistance;
    std::int16_t maximumDistance;
    const char *model;
    const char *label;
    std::uint32_t flags;
    std::uint32_t reserved1c;
};

static_assert(sizeof(InventoryItemDefinition) == 0x20);
static_assert(offsetof(InventoryItemDefinition, model) == 0x08);
static_assert(offsetof(InventoryItemDefinition, label) == 0x10);
static_assert(offsetof(InventoryItemDefinition, flags) == 0x18);

std::span<const InventoryItemDefinition> GetInventoryItemDefinitions(S2InventoryBank bank) noexcept;
const InventoryItemDefinition *FindInventoryItemDefinition(std::uint32_t itemId) noexcept;
std::int64_t GetItemState(std::uint32_t itemId) noexcept;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) noexcept;
std::uint16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) noexcept;
std::uint16_t GetInventoryItemIconById(std::uint32_t itemId) noexcept;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) noexcept;
std::uint32_t GetInventoryItemId(std::uint32_t category, std::uint32_t index) noexcept;
std::uint32_t GetInventoryItemPresentationType(std::uint32_t itemId) noexcept;
std::uint32_t hasItemRegistered(std::uint32_t itemId) noexcept;
const char *GetInventoryItemModel(std::uint32_t itemId) noexcept;
const char *GetShenmueItem(std::uint32_t category, std::uint32_t index) noexcept;
const char *shenGetAmtOf(std::uint32_t itemId) noexcept;
void GetInventoryItemDistances(std::uint32_t itemId, float *minimum, float *maximum) noexcept;


std::int64_t AddInventoryItem(std::uint32_t itemId, std::uint8_t amount, std::uint8_t flags = 0);
std::int32_t RemoveInventoryItem(std::uint32_t itemId, std::uint8_t amount) noexcept;



void InitItems();

void RegisterItems();

} // namespace shenmue::s2
