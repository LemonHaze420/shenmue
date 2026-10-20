#include "s2/pawn_shop.hpp"

#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/archive_data.hpp"
#include "s2/inventory.hpp"
#include "s2/pak.hpp"
#include "s2/pawn.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/scene_data.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/save.hpp"
#include "s2/pause.hpp"
#include "s2/d3t_renderer.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>

namespace shenmue::s2 {
namespace {

HLib::Task *pawnMainTask{};
HLib::Task *pawnDrawTask{};
HLib::Task *pawnReleaseTask{};
std::int32_t pawnReleaseFrames{};
PawnDataHeader *pawnShopData{};
PawnSaleSelection pawnSelection{};
shen_name pawnShopName{};
std::int32_t pawnSaleItemCount{};
std::uint32_t pawnPreviousNpcLoadState{};
std::int32_t pawnPreviousDisplay{};
std::uint16_t pawnPendingResult{};
std::int32_t pawnTransitionFrames{};
std::int32_t *pawnSetOrder{};
std::uint32_t *pawnSetScores{};
PawnStockCount *pawnStockScratch{};
std::uint16_t *pawnSaleItems{};
std::int16_t *pawnSaleQuantities{};
std::uint32_t pawnSaleRunningPrice{};
PawnWindowWork pawnWindow{};
std::array<std::int32_t, 6> pawnWindowTextures{};
HLib::Task *pawnWindowTask{};

LegacyIconSprite pawnWindowLabel{.x = 0.0f,
    .y = 0.0f,
    .scaleX = 1.0f,
    .scaleY = 1.0f,
    .depth = 0.0f,
    .flags = 0,
    .reserved16 = 0,
    .rotation = 0,
    .icon = 0,
    .width = 0,
    .height = 0,
    .alignment = 0,
    .reserved23 = {}};

const PawnItemPrice *ResolveShopItems(const PawnShopRecord *shop) {
    if (!shop || !shop->itemsOffset)
        return nullptr;
    const std::intptr_t base = reinterpret_cast<std::intptr_t>(HLib::DynamicHeapBase) + 0x40;
    return reinterpret_cast<const PawnItemPrice *>(base + static_cast<std::intptr_t>(shop->itemsOffset));
}

#include "pawn_legacy_draw.inl"

void pawn_PawnShopMain(void *) {
    // pawn_PawnShopMain deliberately queues a null main callback. Admission and
    // completion are driven by the shop command functions, not this task.
}

void pawn_PawnShopDraw(void *) {
    DrawPawnShopLegacy();
}

void CleanupPawnSale() {
    if (!pawnMainTask || !pawnDrawTask || static_cast<std::int32_t>(pawnSelection.flags) >= 0)
        return;
    ReleasePawnDrawState();
    if ((pawnSelection.flags & 0x10000U) != 0) {
        SetRenderClockAndCompassTo(0);
        pawnSelection.flags &= ~0x10000U;
    }
    if (pawnSelection.helpTopic != 0)
        EV_EventSetUpHelpIcon_0(pawnSelection.helpTopic, 0);
    if ((pawnSelection.flags & 0x20001U) == 0x20000U)
        ResetHelpIconsToItemPrompt();
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    EV_SetCharDisplay(static_cast<Character *>(control->ryo), CVM_DisplayEnabled, pawnPreviousDisplay);
    EV_NPC_NewLoadSw(pawnPreviousNpcLoadState);
    pawnSelection.flags &= ~0x80000000U;
}

void pawn_PawnShopRelease(void *) {
    if ((pawnSelection.flags & 0x40000000U) == 0)
        return;
    if (pawnReleaseFrames > 0) {
        --pawnReleaseFrames;
        return;
    }
    if (pawnReleaseFrames == 0) {
        pawnReleaseFrames = -1;
        FlushQueuedLegacyDrawPasses();
        FlushQueuedStripType3Batches();
        if (pawnMainTask && pawnDrawTask && static_cast<std::int32_t>(pawnSelection.flags) < 0) {
            CleanupPawnSale();
            pawnSelection.flags &= ~0x40000000U;
        } else
            pawnSelection.flags &= ~0x40000000U;
    }
}


std::uint32_t RasterizePawnWindowLineNullsub(const std::byte *, const char *) {
    return 0;
}


// @verified - p
void ReleasePawnWindowTextures() {
    for (std::int32_t &texture : pawnWindowTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    pawnWindow.flags = 0;
}


// @verified - p
std::uint32_t GetPawnWindowShopMask() {
    std::uint32_t mask = 0;
    std::uint32_t bit = 1;
    for (std::size_t index = 0; PAWN_WINDOW_SHOPS[index].item != -1; ++index) {
        if (GetItemState(static_cast<std::uint32_t>(PAWN_WINDOW_SHOPS[index].item)))
            mask |= bit;
        bit = std::rotl(bit, 1);
    }
    return mask;
}


// @verified - p
void CreatePVRs(PawnWindowWork &window) {
    std::uint32_t mask = std::rotl(1U, window.first);
    for (std::size_t slot = 0; slot < pawnWindowTextures.size(); ++slot) {
        const char *title = nullptr;
        if (mask & window.available)
            title = GetPawnShopLabel(PAWN_WINDOW_SHOPS[static_cast<std::size_t>(window.first) + slot].shop);
        RasterizePawnWindowLineNullsub(window.ptx, title);
        ReplacePtxTexture(static_cast<std::uint32_t>(pawnWindowTextures[slot]), window.ptx);
        mask = std::rotl(mask, 1);
    }
}



// @verified - p
void PWIN_Update(void *) {
    pawnWindowLabel.depth = 0.80000007f;
    pawnWindowLabel.flags = 0;
    pawnWindowLabel.rotation = 0;
    pawnWindowLabel.width = 256;
    pawnWindowLabel.height = 32;
    pawnWindowLabel.scaleX = 1.0f;
    pawnWindowLabel.scaleY = 1.0f;
    pawnWindowLabel.alignment = 5;
    LegacyIconSprite prompt;
    InitializeLegacyIconSprite(prompt);
    prompt.depth = 0.81000006f;
    prompt.icon = 4;
    prompt.x = pawnWindow.x - 124.0f;
    if (!(pawnWindow.flags & 1U))
        return;
    for (std::int32_t offset = 0, slot = 0; offset < 240; offset += 40, ++slot) {
        pawnWindowLabel.icon = static_cast<std::uint16_t>(pawnWindowTextures[static_cast<std::size_t>(slot)]);
        pawnWindowLabel.y = (pawnWindow.y - 100.0f) + static_cast<float>(offset);
        prompt.y = pawnWindowLabel.y;
        pawnWindowLabel.x = pawnWindow.x + 52.0f;
        if (pawnWindowLabel.icon != 0xffffU)
            PrepareDrawTextureWithMode(pawnWindowLabel, ScreenMode::Centered);
        PrepareDrawWithMode(prompt, ScreenMode::Centered);
    }
    DrawLegacyItemWindow({pawnWindow.x, pawnWindow.y}, 0.60000002f, 9, 6, ScreenMode::Centered);
    shenSubmitQuadsUI(pawnWindowLabel.x, static_cast<float>(40 * pawnWindow.row) + (pawnWindow.y - 100.0f), 0x1f0000ffU, 0xbf0000ffU, 244, 40, 0.70000005f);
}



// @verified - p
void PWIN_Callback(void *) {
    const std::uint16_t pressed = GetPadControlData(1);
    switch (pawnWindow.state) {
    case 1:
        pawnWindow.state = 2;
        break;
    case 2:
        if (AdvanceIconPosition(pawnWindow.x, pawnWindow.velocity, 320.0f))
            pawnWindow.state = 3;
        break;
    case 3: {
        if (pressed & 0x400U) {
            const std::uint32_t bit = static_cast<std::uint8_t>(pawnWindow.row + pawnWindow.first) & 31U;
            if (pawnWindow.available >> bit & 1U) {
                pawnWindow.flags &= ~1U;
                pawnWindow.state = 5;
            } else {
                ExecuteAicaCommand(0x800abU, 0, 0);
            }
            set_icon_select_flags_to(0x600U);
            break;
        }
        const std::int32_t first = pawnWindow.first;
        const std::uint16_t repeated = GetPadControlData(2);
        const std::int32_t previousFirst = pawnWindow.first;
        const std::int32_t previousRow = pawnWindow.row;
        if (repeated & 0x1000U) {
            pawnWindow.row = previousRow - 1;
            if (pawnWindow.row < 0) {
                pawnWindow.row = 0;
                pawnWindow.first = previousFirst - 1 < 0 ? 0 : previousFirst - 1;
            }
        }
        if ((repeated & 0x2000U) && ++pawnWindow.row >= 6) {
            ++pawnWindow.first;
            pawnWindow.row = 5;
            if (pawnWindow.first + 5 >= 12)
                pawnWindow.first = pawnWindow.first - 1;
        }
        if (pawnWindow.first != previousFirst || pawnWindow.row != previousRow) {
            if (first != pawnWindow.first)
                CreatePVRs(pawnWindow);
        } else if (pressed & 0x200U) {
            pawnWindow.state = 6;
        }
        set_icon_select_flags_to(0x600U);
        break;
    }
    case 4:
        if (!IsPAWNShopOpen()) {
            CreatePVRs(pawnWindow);
            pawnWindow.flags |= 1U;
            pawnWindow.state = 3;
        }
        break;
    case 5:
        OpenPAWNShop(PAWN_WINDOW_SHOPS[static_cast<std::size_t>(pawnWindow.row + pawnWindow.first)].shop, 0);
        pawnWindow.state = 4;
        break;
    case 6:
        if (RetreatIconPosition(pawnWindow.x, pawnWindow.velocity, -320.0f)) {
            ReleasePawnWindowTextures();
            pawnWindow.state = 0;
            icsToggleIconState(4, 0);
            pawnWindow.flags &= ~1U;
        }
        break;
    default:
        break;
    }
    HLib::ExecuteCallbackNow(PWIN_Update, nullptr);
}

} // namespace

char *CreatePAWNShops() {
    pawnMainTask = pawnDrawTask = pawnReleaseTask = nullptr;
    pawnReleaseFrames = 0;
    pawnShopData = nullptr;
    pawnSelection = {};
    pawnShopName = {};
    pawnSaleItemCount = 0;
    pawnPreviousNpcLoadState = 0;
    pawnPreviousDisplay = 0;
    pawnPendingResult = 0;
    pawnTransitionFrames = 0;
    pawnSetOrder = nullptr;
    pawnSetScores = nullptr;
    pawnStockScratch = nullptr;
    pawnSaleItems = nullptr;
    pawnSaleQuantities = nullptr;
    InitializePawnDrawState();
    HLib::Task *main = HLib::EnqueueTaskWithoutParameter(pawn_PawnShopMain, 8, 0, TASK_PAWN.name);
    HLib::Task *draw = nullptr;
    HLib::Task *release = nullptr;
    PawnDataHeader *data = nullptr;
    std::int32_t *order = nullptr;
    std::uint32_t *scores = nullptr;
    PawnStockCount *scratch = nullptr;
    std::int16_t *quantities = nullptr;
    if (main) {
        draw = HLib::EnqueueTaskWithoutParameter(pawn_PawnShopDraw, 15, 0, TASK_PAWN.name);
        if (draw)
            release = HLib::EnqueueTaskWithoutParameter(pawn_PawnShopRelease, 15, 0, TASK_PAWN.name);
        if (release) {
            data = GetPawnData();
            if (data && data->signature.name == SHEN_NAME(PAWN) && data->version.name == SHEN_NAME(0201)) {
                std::int32_t maxSets = 0, maxItems = 0;
                const PawnShopRecord *record = reinterpret_cast<const PawnShopRecord *>(data + 1);
                for (std::int32_t index = 0; index < data->shopCount; ++index, ++record) {
                    const PawnShopRecord *const shop = FindPawnShop(data, record->name);
                    if (!shop)
                        continue;
                    if (shop->setCount > maxSets)
                        maxSets = shop->setCount;
                    for (std::int32_t setIndex = 0; setIndex < shop->setCount; ++setIndex) {
                        const PawnSetRecord *const set = GetPawnShopSet(shop, setIndex);
                        if (set && set->itemCount > maxItems)
                            maxItems = set->itemCount;
                    }
                }
                const std::uint32_t setBytes = static_cast<std::uint32_t>(maxSets) * 4U + 4U;
                const std::uint32_t requested = 2U * setBytes + static_cast<std::uint32_t>(maxItems) * 8U + 8U;
                const std::uint32_t size = static_cast<std::int32_t>(requested - 4004U) > 0 ? requested : 4004U;
                order = static_cast<std::int32_t *>(HLib::AllocateTaskData(size, TASK_PAWN));
                if (order) {
                    scores = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(order) + setBytes);
                    scratch = reinterpret_cast<PawnStockCount *>(reinterpret_cast<std::byte *>(scores) + setBytes);
                    quantities = reinterpret_cast<std::int16_t *>(reinterpret_cast<std::byte *>(order) + 2002);
                }
            }
            if (!order) {
                if (data)
                    HLib::FreeTaskData(data);
                data = nullptr;
                HLib::DestroyTask(release);
                release = nullptr;
            }
        }
        if (!release) {
            HLib::DestroyTask(main);
            main = nullptr;
            if (draw)
                HLib::DestroyTask(draw);
            draw = nullptr;
        }
    }
    pawnMainTask = main;
    pawnDrawTask = draw;
    pawnReleaseTask = release;
    pawnReleaseFrames = -1;
    pawnShopData = data;
    pawnShopName.name = 0xffffffffU;
    pawnSetOrder = order;
    pawnSetScores = scores;
    pawnStockScratch = scratch;
    pawnSaleItems = reinterpret_cast<std::uint16_t *>(order);
    pawnSaleQuantities = quantities;
    return reinterpret_cast<char *>(pawnDraw.primaryLabels.data() + pawnDraw.primaryLabels.size());
}

shen_name GetPawnFlyerShopName(std::int32_t index) {
    shen_name name;
    const std::byte *const table = reinterpret_cast<const std::byte *>(&PAWN_FLYER_SHOP_NAMES);
    std::memcpy(&name, table + 12 + 8LL * index, sizeof(name));
    return name;
}
void SelectPawnFlyerShop(std::int32_t index) {
    pawnShopName = GetPawnFlyerShopName(index);
    BuildPawnSetAvailability(FindPawnShop(pawnShopData, pawnShopName), pawnSetOrder, pawnSetScores, pawnStockScratch);
}

void ReleasePAWNShops() {
    pawnTransitionFrames = 0;
    CleanupPawnSale();
    ClosePAWNShop();
    if (pawnReleaseTask)
        HLib::DestroyTask(pawnReleaseTask);
    if (pawnDrawTask)
        HLib::DestroyTask(pawnDrawTask);
    if (pawnMainTask)
        HLib::DestroyTask(pawnMainTask);
    if (pawnSetOrder)
        HLib::FreeTaskData(pawnSetOrder);
    pawnMainTask = pawnDrawTask = pawnReleaseTask = nullptr;
    pawnSetOrder = nullptr;
    pawnSetScores = nullptr;
    pawnStockScratch = nullptr;
    pawnSaleItems = nullptr;
    pawnSaleQuantities = nullptr;
    pawnShopData = nullptr;
    pawnSelection = {};
    pawnReleaseFrames = -1;
}
void RefreshPawnFlyerSetRows(std::int32_t firstRow, std::int32_t inspection) {
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    const std::int32_t setCount = GetPawnSaleSetCount();
    const std::int32_t count = setCount < 12 ? setCount : 12;
    for (std::int32_t ordinal = 0; ordinal < count; ++ordinal) {
        PawnFlyerSetRow &row = pawnFlyerRows[ordinal];
        const std::int32_t setIndex = pawnSetOrder[ordinal + firstRow];
        const PawnSetRecord *const set = GetPawnShopSet(shop, setIndex);
        row.count = set->itemCount;
        row.price = GetPawnSaleSetPrice(setIndex);
        std::snprintf(row.label, sizeof(row.label), "%s", set->label);
        const std::uint16_t *const items = reinterpret_cast<const std::uint16_t *>(set + 1);
        for (std::int32_t item = 0; item < row.count; ++item) {
            row.icons[item] = GetInventoryItemIconById(items[item]);
            row.dictionary[item] = item;
        }
        if (inspection == 1)
            std::memset(row.collected.data(), 1, sizeof(row.collected));
    }
}

bool OpenPAWNShop(shen_name shopName, std::uint32_t mode) {
    bool opened = false;
    std::int32_t shopIndex = -1, flyerMode = 0;
    do {
        if (!pawnMainTask || !pawnDrawTask || !pawnReleaseTask || (pawnSelection.flags & 0xc0000000U) != 0 || !pawnShopData || !pawnSetOrder ||
            !pawnSetScores || mode > 5)
            break;
        std::uint32_t flags = PAWN_SALE_MODE_FLAGS[mode];
        flyerMode = mode == 3 || mode == 5 ? 2 : mode == 2 || mode == 4 ? 1 : 0;
        pawnDraw.view = 4;
        pawnDraw.left = 0;
        pawnDraw.right = 639;
        pawnDraw.top = 16;
        pawnDraw.bottom = 495;
        pawnDraw.targetScroll = 0;
        pawnDraw.interpolationFrames = 0;
        pawnDraw.previousTarget = 0;
        pawnDraw.scrollStart = 0;
        pawnDraw.scroll = 0;
        pawnDraw.modeLabel = -1;
        pawnDraw.shopLabel = -1;
        pawnDraw.titleOffset = 2;
        const PawnShopRecord *const shop = FindPawnShop(pawnShopData, shopName);
        const PawnShopRecord *const shops = reinterpret_cast<const PawnShopRecord *>(pawnShopData + 1);
        for (std::int32_t index = 0; index < pawnShopData->shopCount; ++index) {
            if (shops[index].name.name == shopName.name) {
                shopIndex = index;
                break;
            }
        }
        if (!shop)
            break;
        if ((flags & 2U) != 0) {
            pawnSetCursor = 0;
            BuildPawnSetAvailability(shop, pawnSetOrder, pawnSetScores, pawnStockScratch);
        } else {
            pawnItemCursor = 0;
            if (shop->itemCount < 0 || !ResolveShopItems(shop))
                break;
            RefreshPawnSaleItems(shopName);
        }
        OpenPawnDrawState(shop, flags);
        if (pawnDraw.background == -1)
            break;
        pawnPendingResult = 0;
        pawnPreviousNpcLoadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        pawnPreviousDisplay = IsOfThisVisibilityType(static_cast<Character *>(control->ryo), 3);
        pawnSelection.flags &= ~0x20000U;
        if (IsClockAndCompassSuppressed()) {
            SetRenderClockAndCompassTo(1);
            flags |= 0x10000U;
        }
        LoadLegacyCollectionSprites(TASK_PAWN);
        pawnShopName = shopName;
        pawnSelection.flags = flags;
        pawnSelection.helpTopic = 0;
        pawnSelection.result = 0;
        pawnTransitionFrames = 12;
        if ((flags & 1U) != 0)
            ExecuteAicaCommand(0xabU, 0, 0);
        opened = true;
    } while (false);
    if (flyerMode == 2) {
        PreparePawnItemFlyer(shopIndex);
        ShowElementByIndex(UIElement::PawnshopFlyer1);
    } else {
        PreparePawnSetFlyer(shopIndex, 0);
        ShowElementByIndex(UIElement::PawnshopFlyer2);
    }
    if (!opened)
        ReleasePawnNamedTexture(pawnDraw.background);
    return opened;
}

void ClosePAWNShop() {
    if ((pawnSelection.flags & 1U) != 0) {
        FlushQueuedLegacyDrawPasses();
        FlushQueuedStripType3Batches();
        CleanupPawnSale();
    } else if ((pawnSelection.flags & 0xc0000000U) == 0x80000000U) {
        if (pawnReleaseTask && pawnTransitionFrames > 0) {
            pawnReleaseFrames = pawnTransitionFrames;
            pawnSelection.flags |= 0x40000000U;
        } else {
            FlushQueuedLegacyDrawPasses();
            FlushQueuedStripType3Batches();
            CleanupPawnSale();
        }
    }
    DestroyElementByIndex(UIElement::PawnshopFlyer1);
    DestroyElementByIndex(UIElement::PawnshopFlyer2);
}

bool IsPAWNShopOpen() noexcept {
    return pawnMainTask && pawnDrawTask && (pawnSelection.flags & 0xc0000000U) == 0x80000000U;
}

// @verified - p
std::int32_t GetDailyTaskIndex() noexcept {
    const std::uint32_t flags = pawnSelection.flags;
    if (!pawnMainTask || !pawnDrawTask || (flags & 0xc0000000U) != 0x80000000U)
        return 0;
    if (flags & 1U)
        return 1;
    return ((~flags & 2U) | 4U) >> 1U;
}

PawnSaleSelection &GetPawnSaleSelection() {
    return pawnSelection;
}
std::uint16_t *GetPawnSaleItems() {
    return pawnSaleItems;
}
std::int16_t *GetPawnSaleQuantities() {
    return pawnSaleQuantities;
}
std::int32_t GetPawnSaleItemCount() {
    return pawnSaleItemCount;
}

// @verified - p
std::uint64_t CopyPawnSaleItemsAndQuantities(std::uint16_t *items, std::int16_t *quantities) {
    for (std::int32_t index = 0; index < pawnSaleItemCount; ++index) {
        items[index] = pawnSaleItems[index];
        quantities[index] = pawnSaleQuantities[index];
    }
    return static_cast<std::uint32_t>(pawnSaleItemCount) | (static_cast<std::uint64_t>(pawnPreviousNpcLoadState) << 32);
}

// @verified - p
const PawnSetRecord *GetPawnOrderedSetDetails(std::int32_t ordinal, std::int32_t *itemCount, std::int32_t *price) {
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    const std::intptr_t base = reinterpret_cast<std::intptr_t>(HLib::DynamicHeapBase) + 0x40;
    const PawnSetReference *const references = GetPawnShopSetReferences(shop);
    const std::int32_t index = pawnSetOrder[ordinal];
    const PawnSetRecord *const set = references[index].offset ? reinterpret_cast<const PawnSetRecord *>(base + references[index].offset) : nullptr;
    *itemCount = set->itemCount;
    *price = static_cast<std::int16_t>(references[pawnSetOrder[ordinal]].price);
    return set;
}

// @verified - p
std::int64_t CopyPawnOrderedSetIcons(std::int32_t ordinal, std::int32_t *icons) {
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    const std::intptr_t base = reinterpret_cast<std::intptr_t>(HLib::DynamicHeapBase) + 0x40;
    const PawnSetReference *const references = GetPawnShopSetReferences(shop);
    const std::int32_t index = pawnSetOrder[ordinal];
    const PawnSetRecord *const set = references[index].offset ? reinterpret_cast<const PawnSetRecord *>(base + references[index].offset) : nullptr;
    const std::int16_t *const items = reinterpret_cast<const std::int16_t *>(set + 1);
    for (std::int32_t item = 0; item < set->itemCount; ++item) {
        icons[item] = static_cast<std::int16_t>(GetInventoryItemIconById(static_cast<std::uint32_t>(static_cast<std::int32_t>(items[item]))));
    }
    return 0;
}

// @verified - p
std::uint32_t GetPawnSetItem(std::int32_t set, std::uint32_t ordinal) {
    const PawnSetRecord *const record = GetPawnShopSet(FindPawnShop(pawnShopData, pawnShopName), set);
    if (ordinal < static_cast<std::uint32_t>(record->itemCount))
        return reinterpret_cast<const std::uint16_t *>(record + 1)[ordinal];
    return 0;
}

// @verified - p
std::uint32_t GetPawnShopSetCount() {
    return static_cast<std::uint32_t>(FindPawnShop(pawnShopData, pawnShopName)->setCount);
}

// @verified - p
void SetPawnSetCursor(std::int32_t cursor) {
    pawnSetCursor = cursor;
}

std::int32_t *GetPawnSaleSetOrder() {
    return pawnSetOrder;
}
// @verified - p
std::int32_t GetPawnSaleSetCount() {
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    const std::intptr_t base = reinterpret_cast<std::intptr_t>(HLib::DynamicHeapBase) + 0x40;
    return shop->setCount > 0 && shop->setsOffset != 0 && base + shop->setsOffset != 0 ? shop->setCount : 0;
}
std::int32_t GetPawnSaleRunningPrice() {
    return static_cast<std::int32_t>(pawnSaleRunningPrice);
}

std::uint32_t GetPawnSaleSelectedTotal() {
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    return CalculatePawnSelectedItemsPrice(
        shop->itemCount, ResolveShopItems(shop), pawnSaleItems, pawnSaleQuantities, static_cast<std::uint32_t>(pawnSaleItemCount));
}
const char *GetPawnShopLabel(shen_name name) {
    if (!pawnShopData || pawnShopData->signature.name != SHEN_NAME(PAWN) || pawnShopData->version.name != SHEN_NAME(0201))
        return nullptr;
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, name);
    return shop && shop->label[0] ? shop->label : nullptr;
}
std::uint32_t GetPawnAvailableSaleModes(shen_name name) {
    if (!pawnShopData || static_cast<std::int32_t>(pawnSelection.flags) < 0)
        return 0;
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, name);
    if (!shop)
        return 0;
    std::uint32_t modes = 0;
    const PawnItemPrice *const prices = ResolveShopItems(shop);
    if (prices)
        for (std::int32_t item = 0; item < shop->itemCount; ++item) {
            if (GetItemState(static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(prices[item].item)))) > 0) {
                modes = 2;
                break;
            }
        }
    for (std::int32_t index = 0; index < shop->setCount; ++index) {
        const PawnSetRecord *const set = GetPawnShopSet(shop, index);
        if (!set)
            continue;
        const std::uint16_t *const items = reinterpret_cast<const std::uint16_t *>(set + 1);
        std::int32_t item = 0;
        for (; item < set->itemCount; ++item)
            if (GetItemState(static_cast<std::uint32_t>(static_cast<std::int16_t>(items[item]))) <= 0)
                break;
        if (item == set->itemCount) {
            modes |= 1U;
            break;
        }
    }
    return modes;
}
void RefreshPawnSaleSets(shen_name name) {
    pawnItemCursor = 0;
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, name);
    BuildPawnSetAvailability(shop, pawnSetOrder, pawnSetScores, pawnStockScratch);
}
void RefreshPawnSaleItems(shen_name name) {
    pawnItemCursor = 0;
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, name);
    const PawnItemPrice *const prices = ResolveShopItems(shop);
    if (shop->itemCount < 0 || !prices)
        return;
    std::int32_t count = 0;
    for (std::uint16_t item = 4000; item < 4999; ++item) {
        if (GetItemState(item) <= 0)
            continue;
        pawnSaleItems[count] = item;
        pawnSaleQuantities[count] = -256;
        for (std::int32_t price = 0; price < shop->itemCount; ++price) {
            if (prices[price].item == item) {
                pawnSaleQuantities[count] = 0;
                break;
            }
        }
        ++count;
    }
    pawnSaleItems[count] = 0xffff;
    pawnSaleItemCount = count;
}
// @verified - p
std::int32_t GetPawnSaleSetPrice(std::int32_t set) {
    const PawnSetReference *const sets = GetPawnShopSetReferences(FindPawnShop(pawnShopData, pawnShopName));
    return static_cast<std::int16_t>(sets[set].price);
}
const PawnSetRecord *GetPawnSaleSet(std::int32_t set) {
    return GetPawnShopSet(FindPawnShop(pawnShopData, pawnShopName), set);
}

// @verified - p
std::uint32_t GetPawnSetItemCount(std::int32_t set) {
    const PawnSetRecord *const record = GetPawnShopSet(FindPawnShop(pawnShopData, pawnShopName), set);
    return record ? static_cast<std::uint32_t>(record->itemCount) : 0;
}

const char *GetPawnSaleSetLabel(std::int32_t set) {
    const PawnSetRecord *const record = GetPawnSaleSet(set);
    char token[12];
    const std::ptrdiff_t offset = reinterpret_cast<const std::byte *>(record) - reinterpret_cast<const std::byte *>(pawnShopData);
    std::snprintf(token, sizeof(token), "UICSET%04u", static_cast<unsigned int>(offset));
    return GetLocalizedSceneText(token).data();
}
shen_name GetPawnSaleShopName() {
    return pawnShopName;
}
std::int32_t GetPawnSaleItemPrice(std::int32_t item) {
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    const PawnItemPrice *const prices = ResolveShopItems(shop);
    for (std::int32_t price = 0; price < shop->itemCount; ++price)
        if (prices[price].item == pawnSaleItems[item])
            return prices[price].price;
    return 0;
}
std::int64_t IncrementPawnSaleQuantity(std::int32_t item) {
    std::int16_t &quantity = pawnSaleQuantities[item];
    if (quantity >= 0) {
        quantity = static_cast<std::int16_t>(static_cast<std::uint16_t>(quantity) + 1U);
        pawnSaleRunningPrice += static_cast<std::uint32_t>(GetPawnSaleItemPrice(item));
    }
    if (quantity > GetItemState(static_cast<std::uint32_t>(static_cast<std::int16_t>(pawnSaleItems[item])))) {
        pawnSaleRunningPrice -=
            GetPawnSaleItemPrice(item) * static_cast<std::int32_t>(GetItemState(static_cast<std::uint32_t>(static_cast<std::int16_t>(pawnSaleItems[item]))));
        quantity = 0;
    }
    return static_cast<std::uint16_t>(~static_cast<std::uint16_t>(quantity)) >> 15;
}
bool DecrementPawnSaleQuantity(std::int32_t item) {
    std::int16_t &quantity = pawnSaleQuantities[item];
    if (quantity <= 0) {
        if (quantity >= 0) {
            pawnSaleRunningPrice = 0;
            quantity = 0;
        }
        return false;
    }
    pawnSaleRunningPrice -=
        GetPawnSaleItemPrice(item) * static_cast<std::int32_t>(GetItemState(static_cast<std::uint32_t>(static_cast<std::int16_t>(pawnSaleItems[item]))));
    --quantity;
    return true;
}
std::int32_t GetPawnSaleResult() {
    if (pawnSelection.result == 0xffff || (pawnSelection.flags & 1U) != 0)
        return -2;
    return (pawnSelection.flags & 2U) != 0 ? pawnSelection.result : pawnSelection.result | 0x40000000;
}
void CompletePawnSaleSelection(std::int16_t result) {
    pawnSelection.result = static_cast<std::uint16_t>(result);
    ClosePAWNShop();
}
// @verified - p
std::int32_t GetPawnSalePrice() {
    if (!pawnShopData || pawnSelection.result == 0xffff)
        return -1;
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, pawnShopName);
    if (!shop)
        return -1;
    if ((pawnSelection.flags & 2U) != 0) {
        const PawnSetReference *const sets = GetPawnShopSetReferences(shop);
        if (!sets)
            return -1;
        return static_cast<std::int16_t>(sets[pawnSelection.result].price);
    }
    return static_cast<std::int32_t>(CalculatePawnSelectedItemsPrice(
        shop->itemCount, ResolveShopItems(shop), pawnSaleItems, pawnSaleQuantities, static_cast<std::uint32_t>(pawnSaleItemCount)));
}
std::int32_t CommitPawnSale() {
    const std::int32_t price = GetPawnSalePrice();
    if (price <= 0)
        return price;
    if ((pawnSelection.flags & 2U) != 0) {
        const PawnSetRecord *const set = GetPawnShopSet(FindPawnShop(pawnShopData, pawnShopName), pawnSelection.result);
        if (set) {
            const std::uint16_t *const items = reinterpret_cast<const std::uint16_t *>(set + 1);
            for (std::int32_t item = 0; item < set->itemCount; ++item)
                RemoveInventoryItem(static_cast<std::uint32_t>(static_cast<std::int16_t>(items[item])), 1);
        }
    } else
        for (std::int32_t item = 0; item < pawnSaleItemCount; ++item) {
            if (pawnSaleQuantities[item] > 0) {
                RemoveInventoryItem(
                    static_cast<std::uint32_t>(static_cast<std::int16_t>(pawnSaleItems[item])), static_cast<std::uint8_t>(pawnSaleQuantities[item]));
                pawnSaleQuantities[item] = 0;
            }
        }
    return price;
}
std::int32_t SetPawnSaleTransition(std::int32_t frames) {
    const std::int32_t previous = pawnTransitionFrames;
    if (frames <= 0xffff)
        pawnTransitionFrames = frames > 0 ? frames : 12;
    else if (pawnReleaseFrames > 0)
        pawnReleaseFrames = frames - 65536;
    return previous;
}

// @verified - p
HLib::Task *Queue_pw_CtrlPawnWindow() {
    pawnWindowTextures.fill(-1);
    pawnWindow.state = 0;
    pawnWindow.flags = 0;
    pawnWindow.ptx = nullptr;
    pawnWindowTask = HLib::EnqueueTaskWithoutParameter(PWIN_Callback, 8, 0, static_cast<std::int32_t>(PAWN_WINDOW_TASK.name));
    return pawnWindowTask;
}



// @verified - p
std::int64_t OpenPawnWindow(float) {
    if (!pawnWindowTask || pawnWindow.state != 0)
        return 0;
    pawnWindow.row = 0;
    pawnWindow.reserved14 = 0;
    pawnWindow.first = 0;
    pawnWindow.available = GetPawnWindowShopMask();
    pawnWindow.ptx = nullptr;
    pawnWindow.x = -320.0f;
    pawnWindow.y = 240.0f;
    pawnWindow.velocity = 1.0f;
    pawnWindow.reserved2c = 0.0f;
    if (!pawnWindow.available)
        return 0;
    EV_ResetFreeTalkFlags();
    for (std::int32_t slot = 0; slot < 6; ++slot) {
        pawnWindow.ptx = SetupPtxTexture(256, PAWN_WINDOW_TEXT.name, make_shen_name('0', '0', '0', static_cast<char>('0' + slot))).data();
        pawnWindowTextures[static_cast<std::size_t>(slot)] = static_cast<std::int32_t>(ReadTexnTexture(pawnWindow.ptx));
    }
    CreatePVRs(pawnWindow);
    pawnWindow.flags |= 1U;
    pawnWindow.state = 1;
    icsToggleIconState(4, 1);
    return 1;
}


// @verified - p
std::int64_t ClosePawnWindow(std::int32_t force) {
    if (!force || (GetCurrentItemSelectState() & 0x10U) == 0)
        return 0;
    ReleasePawnWindowTextures();
    pawnWindow.state = 0;
    icsToggleIconState(4, 0);
    pawnWindow.flags = 0;
    if (IsPAWNShopOpen())
        ClosePAWNShop();
    return 1;
}


// @verified - p
bool IsPawnWindowOpen() noexcept {
    return pawnWindow.state != 0;
}

void ReleasePawnTaskOwners() {
    ReleasePawnWindowTextures();
    if (pawnWindowTask)
        HLib::DestroyTask(pawnWindowTask);
    pawnWindowTask = nullptr;
    pawnWindow = {};
    ReleasePAWNShops();
}

} // namespace shenmue::s2
