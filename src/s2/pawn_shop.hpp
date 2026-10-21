#pragma once

#include "s2/hlib.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_PAWN = SHEN_NAME(PAWN);
inline constexpr std::array<std::uint32_t, 6> PAWN_SALE_MODE_FLAGS{0x80000003U, 0x80000007U, 0x80000002U, 0x80000000U, 0x80000006U, 0x80000004U};
inline constexpr std::uint16_t PAWN_CURSOR_ICON = 0x405;

// CreatePAWNShops publishes the three-task PAWN shop owner only after its
// PAWNDATA-backed workspace has been sized and allocated successfully.
char *CreatePAWNShops();
void ReleasePAWNShops();
bool OpenPAWNShop(shen_name shopName, std::uint32_t mode = 0);
void ClosePAWNShop();
bool IsPAWNShopOpen() noexcept;
std::int32_t GetDailyTaskIndex() noexcept;

struct PawnSaleSelection {
    std::uint32_t flags;
    std::uint8_t helpTopic, reserved05;
    std::uint16_t result;
};
static_assert(sizeof(PawnSaleSelection) == 8);
static_assert(offsetof(PawnSaleSelection, helpTopic) == 4);
static_assert(offsetof(PawnSaleSelection, result) == 6);

struct PawnLegacyDrawState {
    std::int32_t view, left, right, top, bottom;
    std::int32_t targetScroll, interpolationFrames, previousTarget, scrollStart, scroll, contentHeight;
    std::int32_t background, modeLabel, shopLabel;
    std::array<std::int32_t, 20> primaryLabels, secondaryLabels;
    std::array<std::uint32_t, 20> labelOwners;
    std::array<std::int32_t, 6> actionLabels;
    std::int32_t frame, titleOffset;
    char title[24];
};
static_assert(sizeof(PawnLegacyDrawState) == 352);
static_assert(offsetof(PawnLegacyDrawState, background) == 44);
static_assert(offsetof(PawnLegacyDrawState, primaryLabels) == 56);
static_assert(offsetof(PawnLegacyDrawState, secondaryLabels) == 136);
static_assert(offsetof(PawnLegacyDrawState, labelOwners) == 216);
static_assert(offsetof(PawnLegacyDrawState, actionLabels) == 296);
static_assert(offsetof(PawnLegacyDrawState, frame) == 320);
static_assert(offsetof(PawnLegacyDrawState, title) == 328);
struct PawnFlyerSetRow {
    std::array<std::int32_t, 64> icons, dictionary, collected;
    std::int32_t reserved, count;
    char label[64];
    std::int32_t price;
};
static_assert(sizeof(PawnFlyerSetRow) == 844);
static_assert(offsetof(PawnFlyerSetRow, collected) == 512);
static_assert(offsetof(PawnFlyerSetRow, count) == 772);
static_assert(offsetof(PawnFlyerSetRow, label) == 776);
extern std::array<PawnFlyerSetRow, 12> pawnFlyerRows;
PawnFlyerSetRow GetPawnFlyerSetRow(std::int32_t index);
std::uint32_t GetPawnAvailableSaleModes(shen_name shopName);
std::int32_t GetPawnSaleResult();
std::int32_t GetPawnSalePrice();
std::int32_t CommitPawnSale();
std::int32_t SetPawnSaleTransition(std::int32_t frames);
void CompletePawnSaleSelection(std::int16_t result);
struct PawnSetRecord;
PawnSaleSelection &GetPawnSaleSelection();
std::uint16_t *GetPawnSaleItems();
std::int16_t *GetPawnSaleQuantities();
std::int32_t GetPawnSaleItemCount();
std::uint64_t CopyPawnSaleItemsAndQuantities(std::uint16_t *items, std::int16_t *quantities);
const PawnSetRecord *GetPawnOrderedSetDetails(std::int32_t ordinal, std::int32_t *itemCount, std::int32_t *price);
std::int64_t CopyPawnOrderedSetIcons(std::int32_t ordinal, std::int32_t *icons);
std::uint32_t GetPawnSetItem(std::int32_t set, std::uint32_t ordinal);
std::uint32_t GetPawnShopSetCount();
void SetPawnSetCursor(std::int32_t cursor);
std::int32_t *GetPawnSaleSetOrder();
std::int32_t GetPawnSaleSetCount();
const PawnSetRecord *GetPawnSaleSet(std::int32_t set);
std::uint32_t GetPawnSetItemCount(std::int32_t set);
const char *GetPawnSaleSetLabel(std::int32_t set);
shen_name GetPawnSaleShopName();
std::int32_t GetPawnSaleSetPrice(std::int32_t set);
std::int32_t GetPawnSaleItemPrice(std::int32_t item);
std::int64_t IncrementPawnSaleQuantity(std::int32_t item);
bool DecrementPawnSaleQuantity(std::int32_t item);
std::int32_t GetPawnSaleRunningPrice();
std::uint32_t GetPawnSaleSelectedTotal();
void RefreshPawnSaleItems(shen_name shopName);
void RefreshPawnSaleSets(shen_name shopName);
const char *GetPawnShopLabel(shen_name shopName);


void RefreshPawnFlyerSetRows(std::int32_t firstRow, std::int32_t inspection);

inline constexpr shen_name PAWN_WINDOW_TASK{SHEN_NAME(PWIN)};
inline constexpr shen_name PAWN_WINDOW_TEXT{SHEN_NAME(PWNM)};



struct PawnWindowWork {
    std::int32_t state;
    std::int32_t first;
    std::int32_t row;
    std::uint32_t flags;
    std::uint32_t available;
    std::uint32_t reserved14;
    const std::byte *ptx;
    float x;
    float y;
    float velocity;
    float reserved2c;
};
static_assert(sizeof(PawnWindowWork) == 0x30);
static_assert(offsetof(PawnWindowWork, available) == 0x10);
static_assert(offsetof(PawnWindowWork, ptx) == 0x18);
static_assert(offsetof(PawnWindowWork, x) == 0x20);
static_assert(offsetof(PawnWindowWork, velocity) == 0x28);


struct PawnWindowShop {
    std::int32_t item;
    shen_name shop;
};
inline constexpr std::array<PawnWindowShop, 13> PAWN_WINDOW_SHOPS{{
    {.item = 9000, .shop = SHEN_NAME(ARP0)},
    {.item = 9001, .shop = SHEN_NAME(WSP0)},
    {.item = 9002, .shop = SHEN_NAME(WKP0)},
    {.item = 9003, .shop = SHEN_NAME(WKP1)},
    {.item = 9004, .shop = SHEN_NAME(WTP0)},
    {.item = 9005, .shop = SHEN_NAME(WTP1)},
    {.item = 9006, .shop = SHEN_NAME(WTP2)},
    {.item = 9007, .shop = SHEN_NAME(WTP3)},
    {.item = 9008, .shop = SHEN_NAME(WTP4)},
    {.item = 9009, .shop = SHEN_NAME(WRP0)},
    {.item = 9010, .shop = SHEN_NAME(QBL0)},
    {.item = 9011, .shop = SHEN_NAME(QBR0)},
    {.item = -1, .shop = {}},
}};

struct PawnFlyerShopNames {
    std::uint64_t prefix;
    std::array<PawnWindowShop, 13> shops;
};
inline constexpr PawnFlyerShopNames PAWN_FLYER_SHOP_NAMES{.prefix = 0, .shops = PAWN_WINDOW_SHOPS};
static_assert(offsetof(PawnFlyerShopNames, shops) == 8);
static_assert(sizeof(PawnFlyerShopNames) == 112);
shen_name GetPawnFlyerShopName(std::int32_t index);
void SelectPawnFlyerShop(std::int32_t index);

// PWIN: the queue-8 null-payload pawn-shop list owned by GUI_Tasks_Enqueue; the ICIS pawn item action opens, polls and
// cancels it.
HLib::Task *Queue_pw_CtrlPawnWindow();
std::int64_t OpenPawnWindow(float transition);
std::int64_t ClosePawnWindow(std::int32_t force);
bool IsPawnWindowOpen() noexcept;

void ReleasePawnTaskOwners();

} // namespace shenmue::s2
