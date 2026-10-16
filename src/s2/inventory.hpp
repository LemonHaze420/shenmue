#pragma once

#include "s2/save.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

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
