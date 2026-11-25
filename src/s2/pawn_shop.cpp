#include "s2/pawn_shop.hpp"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/file_loader_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

#include "s2/scene_data.hpp"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/inventory_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/window.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/input_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/save.hpp"
#endif

#include "s2/pause.hpp"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <string.h>
#include "s2/pawn.hpp"
#include "s2/hlib.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/d3t_renderer.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
void ReleaseNamedTexture_Wrapper(std::int32_t index);
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
struct PawnShopTable {
    PawnDataHeader header;
    PawnShopRecord shops[1];
};

std::int32_t PawnItemActionSide(const PawnLegacyDrawState *draw, std::int32_t cursor, std::int32_t count);
std::int32_t PawnItemActionCursor(const PawnLegacyDrawState *draw, std::int32_t cursor, std::int32_t count);
std::uint32_t PawnVisibleRowCounts(std::int32_t scroll, std::int32_t height);
std::int32_t PawnItemScrolledActionCursor(const PawnLegacyDrawState *draw, std::int32_t scroll, std::int32_t cursor, std::int32_t count);
void PlayPawnInputSound(std::int32_t sound);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
std::array<std::int32_t, 6> pawnWindowTextures;
HLib::Task *pawnWindowTask{};
// Native static label sprite rebuilt by PWIN_Update; .data starts it at unit scale.
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
        shenSetClockAndCompassRenderTo(0);
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

// Native XOR_EAX at the kanji rasterizer boundary: PC v1.07 renders no glyphs into the PWNM line buffer.
std::uint32_t RasterizePawnWindowLineNullsub(const std::byte *, const char *) {
    return 0;
}

// Native PWIN texture release: the six line textures and the draw flag.
// @verified - p
void ReleasePawnWindowTextures() {
    for (std::int32_t &texture : pawnWindowTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    pawnWindow.flags = 0;
}
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

std::int32_t GetPawnSalePrice();
void PrepareDrawTextureWithColour(const LegacyIconSprite &sprite, std::uint32_t colour);
void PawnShopControl(void *);
void PWIN_Callback(void *);
std::int32_t CalculatePawnSelectedItemsPrice(
    std::int32_t priceCount, const PawnItemPrice *prices, const std::uint16_t *items, const std::int16_t *quantities, std::int32_t itemCount);
PawnShopRecord *FindPawnShop(PawnDataHeader *data, shen_name name);
PawnDataHeader *LoadPawnFile(const char *filename);
void CleanupPawnSale(PawnShopRuntime *runtime);
PawnSetReference *GetPawnShopSetReferences(const PawnShopRecord *shop);
PawnSetRecord *GetPawnShopSet(const PawnShopRecord *shop, std::int32_t index);
void BuildPawnSetAvailability(const PawnShopRecord *shop, std::int32_t *order, std::uint32_t *scores, PawnStockCount *scratch);

PawnSetReference *GetPawnShopSetReferences(const PawnShopRecord *shop);
PawnSetRecord *GetPawnShopSet(const PawnShopRecord *shop, std::int32_t index);
void BuildPawnSetAvailability(const PawnShopRecord *shop, std::int32_t *order, std::uint32_t *scores, PawnStockCount *scratch);
PawnWindowWork pawnWindow = {};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

// Native PWIN shop mask: one bit per owned shop flyer, in table order.
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
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
const PawnWindowShop pawnWindowShops[13] = {{9000, SHEN_NAME(ARP0)},
    {9001, SHEN_NAME(WSP0)},
        {9002, SHEN_NAME(WKP0)},
            {9003, SHEN_NAME(WKP1)},
                {9004, SHEN_NAME(WTP0)},
                    {9005, SHEN_NAME(WTP1)},
                        {9006, SHEN_NAME(WTP2)},
                            {9007, SHEN_NAME(WTP3)},
                                {9008, SHEN_NAME(WTP4)}, {9009, SHEN_NAME(WRP0)}, {9010, SHEN_NAME(QBL0)}, {9011, SHEN_NAME(QBR0)}, {-1, 0}};

std::int32_t pawnWindowTextures[6];
HLib::Task *pawnWindowTask = 0;
PawnWindowWork *activePawnWindow = &pawnWindow;

const PawnItemPrice *PawnPrices(const PawnShopRecord *shop) {
    return reinterpret_cast<const PawnItemPrice *>(static_cast<std::uintptr_t>(shop->itemsOffset));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native CreatePVRs: re-uploads the six visible shop rows; owned rows look up their PAWNDATA title for the stub rasterizer.
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

// Native PWIN_Update: six label rows (legacy-sprite nullsub on PC) with their prompt icons, the 9x6 item window and the
// selected-row highlight, drawn while flag bit 0 is set.
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

// Native PWIN_Callback: slide in, browse the twelve shops (scrolling re-uploads the rows), open the selected owned shop's
// flyer and wait for it to close, slide out and release.
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
#endif

} // namespace

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

std::array<PawnFlyerSetRow, 12> pawnFlyerRows{};

// @verified - p
PawnFlyerSetRow GetPawnFlyerSetRow(std::int32_t index) {
    return pawnFlyerRows.data()[index];
}

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

const char *GetPawnFlyerShopLabel(std::int32_t index) {
    return GetPawnShopLabel(GetPawnFlyerShopName(index));
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
        if (shenIsClockAndCompassSuppressed()) {
            shenSetClockAndCompassRenderTo(1);
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
#endif
 // namespace

// @verified - d
const char *GetPawnShopLabel(shen_name name) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    PawnShopRuntime *const runtime = &pawnShop;
    const char *label = 0;
    if (runtime && runtime->data && runtime->data->signature == SHEN_NAME(PAWN) && runtime->data->version == SHEN_NAME(0201)) {
        const PawnShopRecord *const shop = FindPawnShop(runtime->data, name);
        if (shop && shop->label[0])
            label = shop->label;
    }
    return label;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (!pawnShopData || pawnShopData->signature.name != SHEN_NAME(PAWN) || pawnShopData->version.name != SHEN_NAME(0201))
        return nullptr;
    const PawnShopRecord *const shop = FindPawnShop(pawnShopData, name);
    return shop && shop->label[0] ? shop->label : nullptr;
#endif
}

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
shen_name GetPawnShopName(std::int32_t index) {
    PawnShopRuntime *const runtime = &pawnShop;
    shen_name name = 0;
    if (runtime && runtime->data &&
        runtime->data->signature == SHEN_NAME(PAWN) && runtime->data->version == SHEN_NAME(0201) && runtime->data->shopCount > index) {
        name = reinterpret_cast<const PawnShopTable *>(runtime->data)->shops[index].name;
    }
    return name;
}


std::int32_t GetPawnShopCount() {
    PawnShopRuntime *runtime = &pawnShop;
    if (!runtime || !runtime->data || runtime->data->signature != SHEN_NAME4('P', 'A', 'W', 'N') || runtime->data->version != SHEN_NAME4('0', '2', '0', '1'))
        return 0;
    return runtime->data->shopCount;
}


// @verified - d
void PawnShopRelease(void *) {
    PawnShopRuntime *runtime = &pawnShop;
    while (runtime && (runtime->selection.flags & 0x40000000U)) {
        if (runtime->releaseFrames > 0)
            --runtime->releaseFrames;
        else if (!runtime->releaseFrames) {
            runtime->releaseFrames = -1;
            CleanupPawnSale(runtime);
            runtime->selection.flags &= ~0x40000000U;
        }
        break;
    }
}


// @verified - d
void CreatePAWNShops() {
    PawnShopRuntime *runtime = &pawnShop;
    std::int32_t index;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(PawnShopRuntime)); ++index)
        reinterpret_cast<char *>(runtime)[index] = 0;
    std::int32_t failed = 1;
    HLib::Task *main = 0, *draw = 0, *release = 0;
    PawnDataHeader *data = 0;
    std::int32_t *order = 0;
    std::uint32_t *scores = 0;
    PawnStockCount *scratch = 0;
    std::uint16_t *items = 0;
    std::int16_t *quantities = 0;
    while (runtime && (main = HLib::EnqueueTaskWithoutParameter(PawnShopControl, 8, 0, SHEN_NAME4('P', 'A', 'W', 'N'))) &&
        (draw = HLib::EnqueueTaskWithoutParameter(PawnShopDraw, 15, 0, SHEN_NAME4('P', 'A', 'W', 'N')))) {
        if ((release = HLib::EnqueueTaskWithoutParameter(PawnShopRelease, 15, 0, SHEN_NAME4('P', 'A', 'W', 'N'))) && (data = LoadPawnFile("PAWNDATA.BIN")) &&
            data->signature == SHEN_NAME4('P', 'A', 'W', 'N') && data->version == SHEN_NAME4('0', '2', '0', '1')) {
            std::int32_t maxSets = 0, maxItems = 0;
            for (index = 0; index < data->shopCount; ++index) {
                const PawnShopRecord *const shop = FindPawnShop(data, reinterpret_cast<PawnShopRecord *>(data + 1)[index].name);
                if (shop) {
                    if (shop->setCount > maxSets)
                        maxSets = shop->setCount;
                    for (std::int32_t set = 0; set < shop->setCount; ++set) {
                        const PawnSetRecord *const record = GetPawnShopSet(shop, set);
                        if (record && record->itemCount > maxItems)
                            maxItems = record->itemCount;
                    }
                }
            }
            std::int32_t quantityCount = 1000;
            const std::int32_t slotBytes = (maxSets + 1) * 4;
            const std::int32_t quantityBytes = (quantityCount + 1) * 2;
            const std::int32_t itemBytes = (maxItems + 1) * 8;
            order = static_cast<std::int32_t *>(HLib::AllocateTaskData(
                (slotBytes - quantityBytes) * 2 + itemBytes > 0 ? slotBytes * 2 + itemBytes : quantityBytes * 2, SHEN_NAME4('P', 'A', 'W', 'N')));
            if (order) {
                scores = reinterpret_cast<std::uint32_t *>(reinterpret_cast<char *>(order) + slotBytes);
                scratch = reinterpret_cast<PawnStockCount *>(reinterpret_cast<char *>(scores) + slotBytes);
                items = reinterpret_cast<std::uint16_t *>(order);
                quantities = reinterpret_cast<std::int16_t *>(reinterpret_cast<char *>(order) + quantityBytes);
                failed = 0;
            }
        }
        break;
    }
    if (failed) {
        if (order) {
            HLib::FreeTaskData(order);
            order = 0;
            scores = 0;
            scratch = 0;
            items = 0;
            quantities = 0;
        }
        if (data) {
            HLib::FreeTaskData(data);
            data = 0;
        }
        if (release) {
            HLib::DestroyTaskInternal(release);
            release = 0;
        }
        if (main) {
            HLib::DestroyTaskInternal(main);
            main = 0;
        }
        if (draw) {
            HLib::DestroyTaskInternal(draw);
            draw = 0;
        }
    }
    pawnShop.mainTask = main;
    pawnShop.drawTask = draw;
    pawnShop.releaseTask = release;
    pawnShop.releaseFrames = -1;
    pawnShop.data = data;
    pawnShop.shopName = 0xffffffffU;
    pawnShop.draw.background = -1;
    for (index = 0; index < 20; ++index) {
        pawnShop.draw.primaryLabels[index] = -1;
        pawnShop.draw.secondaryLabels[index] = -1;
        pawnShop.draw.labelOwners[index] = 0;
    }
    pawnShop.draw.actionLabels[0] = -1;
    pawnShop.draw.actionLabels[1] = -1;
    pawnShop.draw.actionLabels[2] = -1;
    pawnShop.draw.actionLabels[3] = -1;
    pawnShop.draw.actionLabels[4] = -1;
    pawnShop.draw.actionLabels[5] = -1;
    pawnShop.setOrder = order;
    pawnShop.setScores = scores;
    pawnShop.stockScratch = scratch;
    pawnShop.items = items;
    pawnShop.quantities = quantities;
}


void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
std::int32_t OpenPAWNShop(shen_name name, std::uint32_t mode) {
    PawnShopRuntime *runtime = &pawnShop;
    std::int32_t result = 0;
    std::uint32_t flags;
    std::int32_t topic;
    const PawnShopRecord *shop;
    std::int32_t index;
    std::int32_t height;
    std::int32_t count;
    const PawnItemPrice *prices;
    std::int32_t value;
    while (runtime && runtime->mainTask && runtime->drawTask && runtime->releaseTask && (runtime->selection.flags >> 31) == 0 &&
        !(runtime->selection.flags & 0x40000000U)) {
        if (!runtime->data || !runtime->setOrder || !runtime->setScores)
            break;
        switch (mode) {
        case 0:
            flags = 0x80000003U;
            topic = 78;
            break;
        case 1:
            flags = 0x80000007U;
            topic = 78;
            break;
        case 2:
            flags = 0x80000002U;
            topic = 80;
            break;
        case 4:
            flags = 0x80000006U;
            topic = 80;
            break;
        case 3:
            flags = 0x80000000U;
            topic = 81;
            break;
        case 5:
            flags = 0x80000004U;
            topic = 81;
            break;
        default:
            flags = 0;
            topic = 0;
            break;
        }
        if (!flags)
            break;
        runtime->draw.view = 4;
        runtime->draw.left = 0;
        runtime->draw.right = 639;
        runtime->draw.top = 16;
        runtime->draw.bottom = 495;
        runtime->draw.targetScroll = 0;
        runtime->draw.interpolationFrames = 0;
        runtime->draw.previousTarget = 0;
        runtime->draw.scrollStart = 0;
        runtime->draw.scroll = 0;
        runtime->draw.modeLabel = -1;
        runtime->draw.shopLabel = -1;
        runtime->draw.titleOffset = 2;
        shop = FindPawnShop(runtime->data, name);
        if (!shop)
            break;
        if (flags & 2) {
            runtime->setCursor = 0;
            BuildPawnSetAvailability(shop, runtime->setOrder, runtime->setScores, runtime->stockScratch);
            runtime->draw.view = (flags & 4) ? 3 : 2;
            height = 122;
            for (index = 0; index < shop->setCount; ++index) {
                const PawnSetRecord *const set = GetPawnShopSet(shop, index);
                if (set) {
                    value = (set->itemCount + 9) / 10;
                    height += value * 47 + 16;
                }
            }
            if (height < runtime->draw.bottom - runtime->draw.top - 31)
                height = runtime->draw.bottom - runtime->draw.top - 31;
        } else {
            runtime->itemCursor = 0;
            if (shop->itemCount < 0)
                break;
            const PawnItemPrice *const found = reinterpret_cast<const PawnItemPrice *>(static_cast<std::uintptr_t>(shop->itemsOffset));
            if (!found)
                break;
            prices = found;
            count = 0;
            for (index = 0; index < 999; ++index) {
                const std::int16_t item = static_cast<std::int16_t>(index + 4000);
                value = GetItemState(item);
                if (value > 0) {
                    runtime->items[count] = item;
                    runtime->quantities[count] = -256;
                    for (value = 0; value < shop->itemCount; ++value)
                        if (static_cast<std::int16_t>(prices[value].item) == item) {
                            runtime->quantities[count] = 0;
                            break;
                        }
                    ++count;
                }
            }
            reinterpret_cast<std::int16_t *>(runtime->items)[count] = -1;
            runtime->itemCount = count;
            runtime->draw.view = (flags >> 2) & 1;
            height = ((runtime->itemCount + 8) / 9) * 52 + 122;
            if (height < runtime->draw.bottom - runtime->draw.top - 101)
                height = runtime->draw.bottom - runtime->draw.top - 101;
        }
        runtime->draw.contentHeight = height;
        for (index = 0; shop->label[index]; ++index)
            runtime->draw.title[index] = shop->label[index];
        runtime->draw.title[index] = 0;
        if (runtime->draw.background == -1) {
            TextureChunk *image;
            while (shenOpenFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "PAWNBG03.SPR") &&
                (image = static_cast<TextureChunk *>(
                     shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "PAWNBG03.SPR", SHEN_NAME4('P', 'A', 'W', 'N'))))) {
                runtime->draw.background = LoadAndFindTEXN_Wrapper(image);
                HLib::FreeTaskData(image);
                break;
            }
        }
        if (runtime->draw.background == -1)
            break;
        runtime->pendingResult = 0;
        runtime->savedNpcLoadState = GetNpcNewLoadState();
        EV_NPC_NewLoadSw(0);
        PrepareNpcAllFromArray();
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        runtime->savedCharacterDisplay = IsOfThisVisibilityType(control->ryo, CVM_DisplayEnabled);
        runtime->savedHelpVisible = GetPersistentHelpIconFlags(0);
        runtime->savedHelpSelected = GetPersistentHelpIconFlags(1);
        if (flags & 1)
            CheckHelpIcon(0x23, 0, CheckHelpIconOp_Set);
        else
            CheckHelpIcon(0x21, 0, CheckHelpIconOp_Set);
        if (CheckTimeHudVisibility()) {
            shenSetClockAndCompassRenderTo(1);
            flags |= 0x10000;
        }
        LoadLegacyCollectionSprites(SHEN_NAME4('P', 'A', 'W', 'N'));
        const std::int8_t previousHelp = static_cast<std::int8_t>(helpGetCurrentIconIdx(1));
        EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(topic), 0);
        runtime->shopName = name;
        runtime->selection.flags = flags;
        runtime->selection.helpTopic = previousHelp;
        runtime->selection.result = 0;
        runtime->transitionFrames = 12;
        if (flags & 1)
            PlayPawnInputSound(1);
        result = 1;
        break;
    }
    if (!result && runtime->draw.background != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.background);
        runtime->draw.background = -1;
    }
    return result;
}


// @verified - d
std::int32_t IsPAWNShopOpen() {
    std::int32_t open = 0;
    if (pawnShop.mainTask && pawnShop.drawTask)
        open = (pawnShop.selection.flags & 0xc0000000U) == 0x80000000U;
    return open;
}


// @verified - d
void ClosePAWNShop() {
    PawnShopRuntime *runtime = &pawnShop;
    if (runtime->selection.flags & 1)
        CleanupPawnSale(runtime);
    else {
        const std::uint32_t flags = runtime->selection.flags;
        if ((flags & 0xc0000000U) == 0x80000000U) {
            if (runtime->releaseTask && runtime->transitionFrames > 0) {
                runtime->selection.flags = flags | 0x40000000U;
                runtime->releaseFrames = runtime->transitionFrames;
            } else
                CleanupPawnSale(&pawnShop);
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif


std::int32_t GetPawnSaleResult() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    PawnShopRuntime *const runtime = &pawnShop;
    while (runtime->selection.result != 0xffff && !(runtime->selection.flags & 1)) {
        if (runtime->selection.flags & 2)
            return runtime->selection.result;
        else
            return runtime->selection.result | 0x40000000;
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (pawnSelection.result == 0xffff || (pawnSelection.flags & 1U) != 0)
#endif
    return -2;

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return (pawnSelection.flags & 2U) != 0 ? pawnSelection.result : pawnSelection.result | 0x40000000;
#endif
}

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif


// @verified - d
std::int32_t CommitPawnSale() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    PawnShopRuntime *const runtime = &pawnShop;
#endif
    const std::int32_t price = GetPawnSalePrice();

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    while (price > 0) {
        if (runtime->selection.flags & 2) {
            const PawnShopRecord *const shop = FindPawnShop(runtime->data, runtime->shopName);
            if (!shop)
                break;
            const PawnSetRecord *const set = GetPawnShopSet(shop, runtime->selection.result);
            if (!set)
                break;
            for (std::int32_t index = 0; index < set->itemCount; ++index)
                RemoveInventoryItem(reinterpret_cast<const std::int16_t *>(set + 1)[index], 1);
        } else if (runtime->items && runtime->quantities) {
            for (std::int32_t index = 0; index < runtime->itemCount; ++index)
                if (runtime->quantities[index] > 0) {
                    RemoveInventoryItem(static_cast<std::int16_t>(runtime->items[index]), static_cast<std::uint8_t>(runtime->quantities[index]));
                    runtime->quantities[index] = 0;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif
                }
        }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
        break;
    }
#endif
    return price;
}

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t GetPawnSalePrice() {
    PawnShopRuntime *runtime = &pawnShop;
    std::int32_t price = -1;
    const PawnShopRecord *shop;
    std::uint32_t flags;
    while (runtime && runtime->data && runtime->selection.result != 0xffff &&
        (flags = runtime->selection.flags, (shop = FindPawnShop(runtime->data, runtime->shopName)) != 0)) {
        if (flags & 2) {
            const PawnSetReference *const sets = reinterpret_cast<const PawnSetReference *>(static_cast<std::uintptr_t>(shop->setsOffset));
            if (sets)
                price = static_cast<std::int16_t>(sets[runtime->selection.result].price);
        } else {
            const PawnItemPrice *const prices = reinterpret_cast<const PawnItemPrice *>(static_cast<std::uintptr_t>(shop->itemsOffset));
            if (prices && runtime->items && runtime->quantities)
                price = CalculatePawnSelectedItemsPrice(shop->itemCount, prices, runtime->items, runtime->quantities, runtime->itemCount);
        }
        break;
    }
    return price;
}


std::uint32_t GetPawnAvailableSaleModes(shen_name name) {
    PawnShopRuntime *runtime = &pawnShop;
    std::uint32_t modes = 0;
    const PawnShopRecord *shop;
    while (runtime && runtime->data && !(runtime->selection.flags & 0x80000000U) && (shop = FindPawnShop(runtime->data, name)) != 0) {
        std::int32_t index;
        for (index = 0; index <= 998; ++index)
            if (GetItemState(index + 4000) > 0) {
                modes |= 2;
                break;
            }
        if (shop->setCount > 0 && shop->setsOffset)
            modes |= 1;
        break;
    }
    return modes;
}
#endif


// @verified - d
std::int32_t SetPawnSaleTransition(std::int32_t frames) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    PawnShopRuntime *runtime = &pawnShop;
    if (runtime) {
        const std::int32_t previous = runtime->transitionFrames;
        if (frames > 0xffff) {
            if (runtime->releaseFrames > 0)
                runtime->releaseFrames = frames - 0x10000;
        } else if (frames > 0)
            runtime->transitionFrames = frames;
        else
            runtime->transitionFrames = 12;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const std::int32_t previous = pawnTransitionFrames;
    if (frames <= 0xffff)
        pawnTransitionFrames = frames > 0 ? frames : 12;
    else if (pawnReleaseFrames > 0)
        pawnReleaseFrames = frames - 65536;
#endif
        return previous;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    }
    return 12;
#endif
}

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
std::int32_t UpdatePawnSetSelection(PawnDataHeader *data, shen_name name, PawnLegacyDrawState *draw, std::int32_t *order, PawnStockCount *stock,
    std::int32_t *cursor, std::uint16_t input, std::uint16_t *pending, std::uint8_t firstTrigger, std::uint8_t secondTrigger, std::int32_t inspect,
    std::uint16_t *result) {
    std::int32_t keepOpen, visible, moveScroll, sound, previous, selected, scroll, boundary, index, item, unique;
    const PawnShopRecord *shop;
    const PawnSetRecord *set;
    const std::int16_t *items;
    keepOpen = 0;
    sound = 0;
    while (data && (shop = FindPawnShop(data, name)) != 0 && draw && order && cursor && pending && result && (shop = FindPawnShop(data, name)) != 0) {
        keepOpen = 1;
        visible = draw->bottom - draw->top - 31;
        selected = *cursor;
        previous = selected;
        if (input & 0x400) {
            if (!inspect) {
                keepOpen = 0;
                set = GetPawnShopSet(shop, order[selected]);
                if (set) {
                    sound = 1;
                    *result = order[selected];
                    unique = 0;
                    items = reinterpret_cast<const std::int16_t *>(set + 1);
                    for (index = 0; index < set->itemCount; ++index) {
                        for (item = 0; unique > item; ++item)
                            if (stock[item].item == items[index]) {
                                ++stock[item].remaining;
                                break;
                            }
                        if (unique <= item) {
                            stock[unique].item = items[index];
                            stock[unique].remaining = 1;
                            ++unique;
                        }
                    }
                    for (index = 0; index < unique; ++index) {
                        const std::int32_t owned = GetItemState(stock[index].item);
                        if (stock[index].remaining > owned) {
                            sound = 2;
                            keepOpen = 1;
                        }
                    }
                } else
                    *result = 0xffff;
            }
        } else if (input & 0x200) {
            sound = 1;
            *result = 0xffff;
            keepOpen = 0;
        } else if (input & 0xf000) {
            moveScroll = 0;
            scroll = draw->scroll;
            if (input & 0x1000) {
                if (selected > 0) {
                    --selected;
                    if (inspect) {
                        scroll = 94;
                        if (!selected)
                            scroll = 0;
                        for (index = 0; index < selected; ++index) {
                            set = GetPawnShopSet(shop, order[index]);
                            scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                        }
                        moveScroll = 1;
                    } else {
                        boundary = 94;
                        if (!selected)
                            boundary = 0;
                        for (index = 0; index < selected; ++index) {
                            set = GetPawnShopSet(shop, order[index]);
                            boundary += ((set->itemCount + 9) / 10) * 47 + 16;
                        }
                        if (boundary < draw->scroll) {
                            scroll = boundary;
                            moveScroll = 1;
                        }
                    }
                }
            } else if (input & 0x2000) {
                if (shop->setCount > selected + 1) {
                    ++selected;
                    if (inspect) {
                        scroll = 94;
                        for (index = 0; index < selected; ++index) {
                            set = GetPawnShopSet(shop, order[index]);
                            scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                        }
                        moveScroll = 1;
                    } else {
                        boundary = 94;
                        for (index = 0; index < selected + 1; ++index) {
                            set = GetPawnShopSet(shop, order[index]);
                            boundary += ((set->itemCount + 9) / 10) * 47 + 16;
                        }
                        if (draw->scroll + visible <= boundary) {
                            scroll = boundary - visible;
                            if (scroll < 0)
                                scroll = 0;
                            moveScroll = 1;
                        }
                    }
                    if (selected + 1 == shop->setCount)
                        scroll = draw->contentHeight;
                }
            } else if (input & 0x4000) {
                moveScroll = 1;
                selected = 0;
                scroll = 0;
            } else if (input & 0x8000) {
                moveScroll = 1;
                selected = shop->setCount - 1;
                scroll = 94;
                for (index = 0; index < selected; ++index) {
                    set = GetPawnShopSet(shop, order[index]);
                    scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                    if (visible + scroll > draw->contentHeight) {
                        scroll = draw->contentHeight;
                        break;
                    }
                }
            }
            if (moveScroll) {
                if (visible + scroll <= draw->contentHeight)
                    draw->targetScroll = scroll;
                else {
                    draw->targetScroll = draw->contentHeight - visible;
                    if (inspect) {
                        scroll = 94;
                        for (selected = 0; visible + scroll < draw->contentHeight && shop->setCount > selected; ++selected) {
                            set = GetPawnShopSet(shop, order[selected]);
                            scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                        }
                    }
                }
            }
        } else {
            const std::int32_t delta = (firstTrigger >> 4) - (secondTrigger >> 4);
            scroll = 94;
            if (delta > 0) {
                boundary = delta + draw->targetScroll;
                if (visible + boundary < draw->contentHeight)
                    draw->targetScroll = boundary;
                else
                    draw->targetScroll = draw->contentHeight - visible + 1;
                index = 0;
                while (scroll < draw->targetScroll) {
                    set = GetPawnShopSet(shop, order[index]);
                    scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                    ++index;
                }
                if (inspect)
                    selected = index;
                else if (selected < index)
                    selected = index;
            } else if (delta < 0) {
                boundary = draw->targetScroll + delta;
                if (boundary < 0)
                    boundary = 0;
                draw->targetScroll = boundary;
                index = 0;
                while (scroll < draw->targetScroll) {
                    set = GetPawnShopSet(shop, order[index]);
                    scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                    ++index;
                }
                if (!inspect) {
                    while (scroll < draw->targetScroll + visible && shop->setCount > index) {
                        set = GetPawnShopSet(shop, order[index]);
                        scroll += ((set->itemCount + 9) / 10) * 47 + 16;
                        ++index;
                    }
                    if (index > 1)
                        index -= 2;
                    else
                        index = 0;
                    if (index < selected)
                        selected = index;
                } else
                    selected = index;
            }
            if (delta) {
                draw->previousTarget = draw->targetScroll;
                draw->scroll = draw->targetScroll;
            }
        }
        *cursor = selected;
        if (selected != previous && !inspect)
            sound = 1;
        break;
    }
    PlayPawnInputSound(sound);
    return keepOpen;
}


std::int32_t UpdatePawnItemSelection(PawnDataHeader *data, shen_name name, PawnLegacyDrawState *draw, std::uint16_t *items, std::int16_t *quantities,
    std::int32_t *cursor, std::uint16_t input, std::uint16_t *pending, std::uint8_t firstTrigger, std::uint8_t secondTrigger, std::int32_t count, std::int32_t,
    std::int32_t, std::uint16_t *result) {
    std::int32_t sound, keepOpen, visible, previous, selected, column, row, lastColumn, finalRow, upperHalf, index, quantity, side, horizontal, vertical;
    std::int32_t last, next, nextColumn, nextRow, y, rowY, delta;
    (void)name;
    sound = 0;
    keepOpen = 0;
    if (data && draw && cursor && pending && result) {
        visible = draw->bottom - draw->top - 101;
        selected = *cursor;
        previous = selected;
        keepOpen = 1;
        column = previous % 10;
        row = previous / 10;
        last = count - 1;
        lastColumn = last % 9;
        finalRow = last / 9;
        y = row * 52 + 94;
        upperHalf = y - draw->targetScroll < visible / 2;
        if (input & 0x400) {
            if (column == 9) {
                if (upperHalf) {
                    for (index = 0; index < count && quantities[index] < 1; ++index)
                        ;
                    if (index < count) {
                        sound = 1;
                        keepOpen = 0;
                        *result = 0xfffe;
                    } else
                        sound = 2;
                } else {
                    sound = 1;
                    *result = 0xffff;
                    keepOpen = 0;
                }
            } else {
                index = column + row * 9;
                quantity = quantities[index];
                if (quantity >= 0 && quantity < GetItemState(static_cast<std::int16_t>(items[index]))) {
                    ++quantities[index];
                    sound = 1;
                } else
                    sound = 2;
            }
        } else if (input & 0x200) {
            if (column == 9 && PawnItemActionSide(draw, row * 10 + 9, count) < 0) {
                if (!(*pending & 0x200)) {
                    sound = 1;
                    *result = 0xffff;
                    keepOpen = 0;
                }
            } else if (quantities[row * 9 + column] > 0) {
                --quantities[row * 9 + column];
                sound = 1;
                *pending |= 0x200;
            } else if (!(*pending & 0x200)) {
                while ((side = PawnItemActionSide(draw, row * 10 + 9, count)) != -1)
                    if (side < 0)
                        --row;
                    else
                        ++row;
                selected = row * 10 + 9;
                *pending |= 0x200;
            }
        } else if (input & 0xf000) {
            do {
                horizontal = input >> 15;
                if (input & 0x4000)
                    --horizontal;
                if (horizontal) {
                    vertical = 0;
                    input &= 0x3fff;
                } else {
                    vertical = (input & 0x2000) != 0;
                    if (input & 0x1000)
                        --vertical;
                }
                if (column == 9) {
                    if (vertical) {
                        next = PawnItemActionCursor(draw, selected, count);
                        nextColumn = next % 10;
                        nextRow = next / 10;
                    } else {
                        nextColumn = column;
                        nextRow = row;
                        if (horizontal < 0) {
                            if (row < finalRow || column <= lastColumn)
                                nextColumn = 8;
                            else
                                nextColumn = lastColumn;
                        } else if (horizontal)
                            nextColumn = 0;
                    }
                } else {
                    nextColumn = column + horizontal;
                    nextRow = vertical + row;
                    if (nextColumn < 0)
                        nextColumn = 9;
                    if (nextColumn > 9)
                        nextColumn = 0;
                    if (nextRow < 0)
                        nextRow = 0;
                    if (nextRow > finalRow)
                        nextRow = finalRow;
                    if (nextRow * 9 + nextColumn >= count) {
                        nextColumn = column;
                        if (horizontal)
                            nextColumn = 9;
                        nextRow = row;
                    }
                }
                selected = nextColumn + nextRow * 10;
                y = nextRow * 52 + 94;
                if (vertical == 1) {
                    if (visible + draw->targetScroll < y + 60) {
                        if (nextRow == finalRow)
                            draw->targetScroll = draw->contentHeight + draw->top - draw->bottom + 101;
                        else
                            draw->targetScroll = y - (visible - 60);
                    }
                } else if (!nextRow)
                    draw->targetScroll = 0;
                else if (draw->targetScroll > y)
                    draw->targetScroll = y;
                column = nextColumn;
                row = nextRow;
            } while (horizontal);
        } else {
            delta = (firstTrigger >> 4) - (secondTrigger >> 4);
            if (delta) {
                y = delta + draw->targetScroll;
                if (y < 0)
                    y = 0;
                else if (visible + y > draw->contentHeight)
                    y = draw->contentHeight - visible;
                if (column == 9) {
                    selected = PawnItemScrolledActionCursor(draw, y, selected, count);
                    previous = selected;
                } else if (draw->targetScroll != y) {
                    rowY = (selected / 10) * 52 + 94;
                    if (delta < 0)
                        while (rowY - visible + 51 > y) {
                            rowY -= 52;
                            selected -= 10;
                        }
                    else
                        while (rowY < y) {
                            rowY += 52;
                            selected += 10;
                        }
                }
                draw->targetScroll = y;
                draw->previousTarget = y;
                draw->scroll = y;
            }
        }
        *cursor = selected;
        if (selected != previous) {
            sound = 1;
            if (draw->actionLabels[5] != -1) {
                ReleaseNamedTexture_Wrapper(draw->actionLabels[5]);
                draw->actionLabels[5] = -1;
            }
        }
    }
    PlayPawnInputSound(sound);
    return keepOpen;
}
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

// @verified - d
void PawnShopControl(void *) {
    const std::uint16_t masks[12] = {0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 4, 2, 0x80, 0x40, 0};
    PawnShopRuntime *runtime = &pawnShop;
    if (!runtime)
        return;
    const std::uint32_t flags = runtime->selection.flags;
    if (static_cast<std::int32_t>(flags) >= 0 || (flags & 0x40000000U))
        return;
    LegacyInputSlot *pad = &g_inputBuffer[0];
    std::uint16_t input = pad->repeatedPresses;
    const std::uint8_t leftTrigger = pad->triggers[0];
    const std::uint8_t rightTrigger = pad->triggers[1];
    input &= ~0x0406;
    if ((flags & 1) == 0) {
        RegisterButtonMediation(SHEN_NAME(BTBA), 0, 0x10004);
        if (GetMediatedButtonBindings(SHEN_NAME(BTBA)) && btnCheck(SHEN_NAME(BTBA)))
            input |= 0x400;
    }
    RegisterButtonMediation(SHEN_NAME(BTCC), 0, 0x20004);
    if (GetMediatedButtonBindings(SHEN_NAME(BTCC)) && btnCheck(SHEN_NAME(BTCC)))
        input |= 0x200;
    std::int32_t keepOpen;
    if (runtime->selection.flags & 2)
        keepOpen = UpdatePawnSetSelection(runtime->data, runtime->shopName, &runtime->draw, runtime->setOrder, runtime->stockScratch, &runtime->setCursor,
            input, &runtime->pendingResult, leftTrigger, rightTrigger, runtime->selection.flags & 1, &runtime->selection.result);
    else
        keepOpen = UpdatePawnItemSelection(runtime->data, runtime->shopName, &runtime->draw, runtime->items, runtime->quantities, &runtime->itemCursor, input,
            &runtime->pendingResult, leftTrigger, rightTrigger, runtime->itemCount, runtime->selection.flags & 4, runtime->selection.flags & 1,
            &runtime->selection.result);
    if (!keepOpen)
        ClosePAWNShop();
    const std::uint16_t held = pad->held;
    for (std::int32_t index = 0; masks[index]; ++index)
        if ((runtime->pendingResult & masks[index]) && !(held & masks[index]))
            runtime->pendingResult &= ~masks[index];
}
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
std::uint32_t GetPawnWindowShopMask() {
    std::uint32_t result = 0;
    for (std::int32_t index = 0; pawnWindowShops[index].item != -1; ++index)
        if (GetItemState(pawnWindowShops[index].item))
            result |= 1U << index;
    return result;
}


// @verified - d
void CreatePawnWindowTextures(PawnWindowWork *window) {
    for (std::int32_t index = 0; index < 6; ++index) {
        window->ptx = SetupPtxTexture(256, SHEN_NAME4('P', 'W', 'N', 'M'), 0x303030U | (static_cast<std::uint32_t>(index + '0') << 24));
        pawnWindowTextures[index] = LoadAndFindTEXN_Wrapper(window->ptx);
    }
}


// @verified - d
void RefreshPawnWindowRows(PawnWindowWork *window) {
    std::int32_t row = window->first;
    for (std::int32_t index = 0; index < 6; ++index, ++row) {
        const char *text;
        if ((1U << row) & window->available)
            text = GetPawnShopLabel(pawnWindowShops[row].shop);
        else
            text = "\xa1\xa9\xa1\xa9\xa1\xa9";
        if (!text)
            text = "\xa1\xa9\xa1\xa9\xa1\xa9";
        RasterizeWindowTextLine4bpp(reinterpret_cast<std::byte *>(window->ptx + 1), 256, text, 1, 1);
        ReplacePtxTexture(pawnWindowTextures[index], window->ptx);
    }
}


// @verified - d
void ReleasePawnWindowTextures() {
    for (std::int32_t index = 0; index < 6; ++index) {
        if (pawnWindowTextures[index] != -1)
            ReleaseNamedTexture_Wrapper(pawnWindowTextures[index]);
        pawnWindowTextures[index] = -1;
    }
    activePawnWindow->ptx = 0;
}


// @verified - d
bool UpdatePawnWindowSelection(PawnWindowWork *window, std::uint16_t input) {
    const std::int32_t first = window->first, row = window->row;
    if ((input & 0x1000) && --window->row < 0) {
        window->row = 0;
        if (--window->first < 0)
            window->first = 0;
    }
    if ((input & 0x2000) && ++window->row > 5) {
        window->row = 5;
        ++window->first;
        if (window->first + 5 > 11)
            --window->first;
    }
    if (window->first == first && window->row == row)
        return false;
    ExecuteAicaCommand(0x200ab, 0, 0);
    return true;
}
#endif

} // namespace
 // namespace

// @verified - d
HLib::Task *Queue_pw_CtrlPawnWindow() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    activePawnWindow->state = 0;
    activePawnWindow->flags = 0;
    activePawnWindow->ptx = 0;
    for (std::int32_t index = 0; index < 6; ++index)
        pawnWindowTextures[index] = -1;
    pawnWindowTask = HLib::EnqueueTaskWithoutParameter(PWIN_Callback, 8, 0, SHEN_NAME(PWIN));
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    pawnWindowTextures.fill(-1);
    pawnWindow.state = 0;
    pawnWindow.flags = 0;
    pawnWindow.ptx = nullptr;
    pawnWindowTask = HLib::EnqueueTaskWithoutParameter(PWIN_Callback, 8, 0, static_cast<std::int32_t>(PAWN_WINDOW_TASK.name));
#endif
    return pawnWindowTask;
}

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
bool IsPawnWindowOpen() {
    if (activePawnWindow->state)
        return true;
    return false;
}


// @verified - d
std::int32_t OpenPawnWindow(float) {
    std::int32_t result;
    if (!pawnWindowTask || (result = IsPawnWindowOpen()) != 0)
        return 0;
    activePawnWindow->flags = result;
    activePawnWindow->first = result;
    activePawnWindow->row = result;
    activePawnWindow->available = GetPawnWindowShopMask();
    activePawnWindow->ptx = reinterpret_cast<TextureChunk *>(result);
    activePawnWindow->x = -320.0f;
    activePawnWindow->y = 240.0f;
    activePawnWindow->velocity = 1;
    activePawnWindow->retained24 = 0;
    if (!activePawnWindow->available)
        return 0;
    result = 1;
    EV_ResetFreeTalkFlags();
    CreatePawnWindowTextures(activePawnWindow);
    RefreshPawnWindowRows(activePawnWindow);
    activePawnWindow->state = result;
    activePawnWindow->flags |= 1;
    icsToggleIconState(4, result);
    return result;
}


// @verified - d
void PWIN_Update(void *) {
    LegacyIconSprite label;
    label.width = 256;
    label.height = 32;
    label.scaleX = 1;
    label.scaleY = 1;
    label.flags = 0;
    label.rotation = 0;
    label.alignment = 5;
    label.depth = 2.299999713897705f;
    LegacyIconSprite prompt;
    InitializeLegacyIconSprite(prompt);
    prompt.depth = 2.309999704360962f;
    prompt.icon = 4;
    prompt.x = activePawnWindow->x + -124.0f;
    if (activePawnWindow->flags & 1) {
        for (std::int32_t index = 0; index < 6; ++index) {
            label.icon = static_cast<std::uint16_t>(pawnWindowTextures[index]);
            prompt.y = label.y = (activePawnWindow->y + -100.0f) + static_cast<float>(40 * index);
            label.x = activePawnWindow->x + 52;
            if (static_cast<std::int16_t>(label.icon) != -1)
                PrepareDrawTextureWithMode(label);
            if ((1U << (activePawnWindow->first + index)) & activePawnWindow->available)
                PrepareDrawWithMode(prompt);
        }
        Vector2 position;
        position.x = activePawnWindow->x;
        position.y = activePawnWindow->y;
        DrawLegacyItemWindow(position, 0.009794319048523903f, 9, 6);
        shenSubmitQuadsUI(
            label.x, (activePawnWindow->y + -100.0f) + static_cast<float>(40 * activePawnWindow->row), 0x1f0000ff, 0xbf0000ff, 244, 40, 2.1999998092651367f);
    }
}


// @verified - d
void PWIN_Callback(void *) {
    const std::uint16_t pressed = GetPadControlData(1);
    switch (activePawnWindow->state) {
    case 0:
        break;
    case 1:
        activePawnWindow->state = 2;
        break;
    case 2:
        if (AdvanceIconPosition(reinterpret_cast<ItemEffectPosition *>(&activePawnWindow->x), 320))
            activePawnWindow->state = 3;
        break;
    case 3: {
        std::uint32_t buttons = 0x200;
        if ((1U << (activePawnWindow->row + activePawnWindow->first)) & activePawnWindow->available)
            buttons |= 0x400;
        if (pressed & 0x400) {
            if (buttons & 0x400) {
                activePawnWindow->flags &= ~1U;
                activePawnWindow->state = 5;
            } else
                ExecuteAicaCommand(0x800ab, 0, 0);
        } else {
            const std::int32_t first = activePawnWindow->first;
            if (UpdatePawnWindowSelection(activePawnWindow, GetPadControlData(2))) {
                if (first != activePawnWindow->first)
                    RefreshPawnWindowRows(activePawnWindow);
            } else if (pressed & 0x200) {
                ExecuteAicaCommand(0x100ab, 0, 0);
                activePawnWindow->state = 6;
            }
        }
        RequestHelpIconSelection(buttons);
        break;
    }
    case 5:
        OpenPAWNShop(pawnWindowShops[activePawnWindow->row + activePawnWindow->first].shop, 0);
        activePawnWindow->state = 4;
        break;
    case 4:
        if (!IsPAWNShopOpen()) {
            RefreshPawnWindowRows(activePawnWindow);
            activePawnWindow->state = 3;
            activePawnWindow->flags |= 1;
        }
        break;
    case 6:
        if (RetreatIconPosition(reinterpret_cast<ItemEffectPosition *>(&activePawnWindow->x), -320)) {
            ReleasePawnWindowTextures();
            activePawnWindow->state = 0;
            icsToggleIconState(4, 0);
            activePawnWindow->flags &= ~1U;
        }
        break;
    }
    HLib::ExecuteCallbackNow(PWIN_Update, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native PWIN open (ICIS pawn action begin): opens at -320 when any shop flyer is owned, admitting six PWNM line
// textures from the shared ITPH buffer.
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

// Native PWIN cancel (ICIS pawn action cancel): only a forced cancel while the selector reports state bit 0x10.
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

// Native PWIN poll (ICIS pawn action poll).
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
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
std::int32_t PawnItemActionSide(const PawnLegacyDrawState *draw, std::int32_t cursor, std::int32_t count) {
    std::int32_t result = 0;
    if (draw && cursor >= 0) {
        std::int32_t visible = draw->bottom - draw->top - 101;
        std::int32_t sets = count / 9;
        std::int32_t rest = count % 9;
        std::int32_t end = rest + sets * 10;
        std::int32_t column = cursor % 10;
        std::int32_t row = cursor / 10;
        if (column == 9 || end <= cursor) {
            std::int32_t rowTop = row * 52;
            std::int32_t scroll = draw->targetScroll - 94;
            std::int32_t y = rowTop - scroll;
            if (y < 0)
                y = 0;
            if (y < visible / 2) {
                const std::int32_t position = draw->targetScroll <= 94 ? (row + 1) * 52 : y + 52;
                result = position / 52;
            } else {
                y = visible / 2 - y - 52;
                result = y / 52;
            }
        }
    }
    return result;
}


// @verified - d
std::int32_t PawnItemActionCursor(const PawnLegacyDrawState *draw, std::int32_t cursor, std::int32_t count) {
    std::int32_t row = 0;
    std::int32_t side;
    while (draw && (side = PawnItemActionSide(draw, cursor, count)) != 0) {
        const std::uint32_t rows = PawnVisibleRowCounts(draw->targetScroll, draw->bottom - draw->top - 101);
        const std::int32_t upper = rows >> 16, lower = rows & 0xffff;
        if (upper && lower) {
            std::int32_t first = (draw->targetScroll - 43) / 52;
            if (first < 0)
                first = 0;
            if (side > 0) {
                row = lower * side / upper;
                if (row < 1)
                    row = 1;
                row += upper;
                row = row + first - 1;
            } else if (side < 0) {
                row = -(upper * side) / lower;
                if (row < 1)
                    row = 1;
                row = row + first - 1;
            } else
                row = cursor / 10;
        } else
            row = cursor / 10;
        row = row * 10 + 9;
        break;
    }
    return row;
}


std::uint32_t PawnVisibleRowCounts(std::int32_t scroll, std::int32_t height) {
    if (scroll < 0 || height <= 0)
        return 0;
    std::int32_t gap, top;
    if (scroll <= 94) {
        top = height / 2 + scroll - 94;
        gap = 94 - scroll;
    } else {
        const std::int32_t remainder = (scroll - 94) % 52;
        gap = remainder ? 52 - remainder : 0;
        top = height / 2 - gap;
    }
    const std::int32_t upper = (top + 51) / 52;
    const std::int32_t lower = (height - (upper * 52 + gap)) / 52;
    return (static_cast<std::uint32_t>(upper) << 16) | (static_cast<std::uint32_t>(lower) & 0xffff);
}


// @verified - d
std::int32_t PawnItemScrolledActionCursor(const PawnLegacyDrawState *draw, std::int32_t scroll, std::int32_t cursor, std::int32_t count) {
    std::int32_t starts[2], upper[2], lower[2];
    std::int32_t row = cursor;
    starts[0] = draw->targetScroll;
    starts[1] = scroll;
    const std::int32_t height = draw->bottom - draw->top - 101;
    std::int32_t value = -1;
    for (std::int32_t index = 0; index < 2; ++index) {
        value = PawnVisibleRowCounts(starts[index], height);
        if (value < 0)
            break;
        starts[index] -= 94;
        if (starts[index] <= 0)
            starts[index] = 0;
        else
            starts[index] = (starts[index] + 51) / 52;
        upper[index] = static_cast<std::uint32_t>(value) >> 16;
        lower[index] = value & 0xffff;
    }
    if (value >= 0) {
        value = PawnItemActionSide(draw, cursor, count);
        if (value > 0) {
            row = upper[1] * (value - 1) / upper[0];
            if (row >= upper[1])
                row = upper[1] - 1;
            row += starts[1];
            row = row * 10 + 9;
        } else if (value < 0) {
            row = lower[1] * (-1 - value) / lower[0];
            if (row >= lower[1])
                row = lower[1] - 1;
            row += starts[1];
            row += upper[1];
            if (row > (count + 8) / 9)
                row = (count + 8) / 9;
            row = row * 10 + 9;
        }
    }
    return row;
}
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

#include "s2/pawn_draw_dc.inl"

// @verified - p
std::int32_t CalculatePawnSelectedItemsPrice(
    std::int32_t priceCount, const PawnItemPrice *prices, const std::uint16_t *items, const std::int16_t *quantities, std::int32_t itemCount) {
    std::int32_t result = -1;
    std::int32_t item, price, total;
    if (priceCount > 0 && prices && items && quantities && itemCount > 0) {
        total = 0;
        for (item = 0; item < itemCount; ++item) {
            if (quantities[item] > 0) {
                const std::int16_t id = items[item];
                for (price = 0; price < priceCount; ++price)
                    if (static_cast<std::int16_t>(prices[price].item) == id) {
                        total += quantities[item] * prices[price].price;
                        break;
                    }
            }
        }
        do {
            if (total > 0)
                result = total;
        } while (0);
    }
    return result;
}


// @verified - d
PawnShopRecord *FindPawnShop(PawnDataHeader *data, shen_name name) {
    PawnShopRecord *shop = 0;
    if (data) {
        std::int32_t index = 0;
        const std::int32_t count = data->shopCount;
        PawnShopRecord *const shops = reinterpret_cast<PawnShopRecord *>(data + 1);
        for (; index < count; ++index) {
            if (shops[index].name == name) {
                shop = &reinterpret_cast<PawnShopRecord *>(data + 1)[index];
                break;
            }
        }
    }
    return shop;
}


// @verified - d
PawnDataHeader *LoadPawnFile(const char *filename) {
    std::int32_t failed = 1;
    PawnDataHeader *data = 0;
    while (filename &&
        (data = static_cast<PawnDataHeader *>(shenLoadFile(Filepath_Gen(FilepathGenMode::MiscDirectory), filename, SHEN_NAME4('P', 'A', 'W', 'N')))) != 0 &&
        data->signature == SHEN_NAME4('P', 'A', 'W', 'N') && data->version == SHEN_NAME4('0', '2', '0', '1')) {
        std::int32_t ok = 1;
        for (std::int32_t index = 0; index < data->shopCount; ++index) {
            PawnShopRecord *const shop = &reinterpret_cast<PawnShopRecord *>(data + 1)[index];
            std::uintptr_t setsOffset = shop->setsOffset;
            std::uintptr_t itemsOffset = shop->itemsOffset;
            if (setsOffset) {
                setsOffset += reinterpret_cast<std::uintptr_t>(data);
                for (std::int32_t set = 0; set < shop->setCount; ++set) {
                    std::uintptr_t offset = reinterpret_cast<PawnSetReference *>(setsOffset)[set].offset;
                    if (offset)
                        offset += reinterpret_cast<std::uintptr_t>(data);
                    reinterpret_cast<PawnSetReference *>(setsOffset)[set].offset = offset;
                }
            }
            if (itemsOffset)
                itemsOffset += reinterpret_cast<std::uintptr_t>(data);
            shop->setsOffset = setsOffset;
            shop->itemsOffset = itemsOffset;
        }
        if (ok)
            failed = 0;
        break;
    }
    if (failed && data) {
        HLib::FreeTaskData(data);
        data = 0;
    }
    return data;
}
#endif

namespace {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
void PlayPawnInputSound(std::int32_t sound) {
    switch (sound) {
    case 1:
        ExecuteAicaCommand(0xab, 0, 0);
        break;
    case 2:
        ExecuteAicaCommand(0x800ab, 0, 0);
        break;
    }
}
#endif

} // namespace

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

void CleanupPawnSale(PawnShopRuntime *runtime) {
    if (!runtime || !runtime->mainTask || !runtime->drawTask || static_cast<std::int32_t>(runtime->selection.flags) >= 0)
        return;
    if (runtime->draw.background != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.background);
        runtime->draw.background = -1;
    }
    if (runtime->draw.modeLabel != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.modeLabel);
        runtime->draw.modeLabel = -1;
    }
    if (runtime->draw.shopLabel != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.shopLabel);
        runtime->draw.shopLabel = -1;
    }
    if (runtime->draw.actionLabels[0] != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.actionLabels[0]);
        runtime->draw.actionLabels[0] = -1;
    }
    if (runtime->draw.actionLabels[1] != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.actionLabels[1]);
        runtime->draw.actionLabels[1] = -1;
    }
    if (runtime->draw.actionLabels[2] != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.actionLabels[2]);
        runtime->draw.actionLabels[2] = -1;
    }
    if (runtime->draw.actionLabels[3] != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.actionLabels[3]);
        runtime->draw.actionLabels[3] = -1;
    }
    if (runtime->draw.actionLabels[4] != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.actionLabels[4]);
        runtime->draw.actionLabels[4] = -1;
    }
    if (runtime->draw.actionLabels[5] != -1) {
        ReleaseNamedTexture_Wrapper(runtime->draw.actionLabels[5]);
        runtime->draw.actionLabels[5] = -1;
    }
    for (std::int32_t index = 0; index < 20; ++index) {
        if (runtime->draw.primaryLabels[index] != -1) {
            ReleaseNamedTexture_Wrapper(runtime->draw.primaryLabels[index]);
            runtime->draw.primaryLabels[index] = -1;
        }
        if (runtime->draw.secondaryLabels[index] != -1) {
            ReleaseNamedTexture_Wrapper(runtime->draw.secondaryLabels[index]);
            runtime->draw.secondaryLabels[index] = -1;
        }
        runtime->draw.labelOwners[index] = 0;
    }
    ReleaseLegacyCollectionSprites(SHEN_NAME(PAWN));
    if (runtime->selection.flags & 0x10000) {
        shenSetClockAndCompassRenderTo(0);
        runtime->selection.flags &= ~0x10000U;
    }
    EV_EventSetUpHelpIcon_0(runtime->selection.helpTopic, 0);
    CheckHelpIcon(runtime->savedHelpVisible, runtime->savedHelpSelected, CheckHelpIconOp_Set);
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    EV_SetCharDisplay(control->ryo, CVM_DisplayEnabled, runtime->savedCharacterDisplay);
    EV_NPC_NewLoadSw(runtime->savedNpcLoadState);
    runtime->selection.flags &= ~0x80000000U;
}


PawnShopRuntime pawnShop;
#endif

} // namespace s2

} // namespace shenmue
