#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/legacy_types.hpp"
#define INV_NOEXCEPT
#define INV_LAYOUT_NAME_(line) InventoryLayoutCheck##line
#define INV_LAYOUT_NAME(line) INV_LAYOUT_NAME_(line)
#define INV_LAYOUT(condition) typedef char INV_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

struct InventoryItemDefinition {
    std::uint16_t icon;
    std::uint16_t id;
    std::int16_t minimumDistance;
    std::int16_t maximumDistance;
    const char *model;
    const char *label;
    std::uint32_t flags;
};
INV_LAYOUT(sizeof(InventoryItemDefinition) == 0x14);
INV_LAYOUT(offsetof(InventoryItemDefinition, id) == 2);
INV_LAYOUT(offsetof(InventoryItemDefinition, model) == 8);
INV_LAYOUT(offsetof(InventoryItemDefinition, label) == 0xc);
INV_LAYOUT(offsetof(InventoryItemDefinition, flags) == 0x10);
extern const std::int32_t inventoryItemCounts[6];
extern const std::int32_t itemTypeByThousands[10];
extern const InventoryItemDefinition keyItems[42];
extern const InventoryItemDefinition capsuleToys[275];
extern const InventoryItemDefinition photos[18];
extern const InventoryItemDefinition mapItems[26];
extern const InventoryItemDefinition moveItems[26];
extern const InventoryItemDefinition pawnItems[12];
extern const InventoryItemDefinition *const itemBanks[6];

struct InventoryItemState {
    std::uint8_t count;
    std::uint8_t item;
};
INV_LAYOUT(sizeof(InventoryItemState) == 2);
extern const std::uint16_t initialItems[12];
extern InventoryItemState g_inventoryItemStates[64];
extern std::uint8_t g_keyItemStock[384];
extern std::uint8_t g_capsuleStock[512];
extern std::uint8_t g_photoStock[64];
extern std::uint8_t g_mapStock[128];
extern std::uint8_t g_moveStock[28];
extern std::uint8_t g_pawnStock[16];
extern std::uint8_t *const inventoryStockBanks[6];
const InventoryItemDefinition *FindInventorySlot(std::uint32_t item, std::int32_t &bank, std::int32_t &index) INV_NOEXCEPT;
std::int32_t GetItemState(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetMaxItemsForItemType(std::uint32_t category) INV_NOEXCEPT;
std::int16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::int16_t GetInventoryItemIconById(std::uint32_t item) INV_NOEXCEPT;
std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t GetInventoryItemId(std::uint32_t category, std::uint32_t index) INV_NOEXCEPT;
std::uint32_t hasItemRegistered(std::uint32_t item) INV_NOEXCEPT;
std::int32_t AddInventoryItem(std::uint32_t item, std::uint8_t amount, std::uint32_t flags = 0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
enum InventoryBankIndex { IB_KeyItems, IB_Capsules, IB_Photos, IB_Maps, IB_Moves, IB_Pawn };
enum InventoryItemFlag { IIF_Stackable = 0x01, IIF_RetainOne = 0x02, IIF_HelpNotice = 0x10 };
enum { kKeyItemStateCount = 384 };
extern std::uint8_t g_keyItemStateFlags[kKeyItemStateCount];
std::int32_t RemoveInventoryItem(std::uint32_t item, std::uint8_t amount);
std::uint8_t UpdateItemStateFlags(std::uint32_t item, std::uint8_t mask, std::int32_t enabled);
std::uint8_t GetItemStateFlags(std::uint32_t item);
void ResetItemStateFlags();
const char *GetInventoryItemModel(std::uint32_t item);
void GetInventoryItemDistances(std::uint32_t item, float *minimum, float *maximum);
const char *shenGetAmtOf(std::uint32_t item, std::int32_t unused = 0);
bool GiveItemOf(std::uint32_t item);
#endif
void InitializeInventoryStock();
void InitItems();

} // namespace s2
} // namespace shenmue

#undef INV_NOEXCEPT
#undef INV_LAYOUT
#undef INV_LAYOUT_NAME
#undef INV_LAYOUT_NAME_

#endif
