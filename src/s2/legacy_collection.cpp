#include "s2/legacy_collection.hpp"

#include "s2/full_map_menu.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/item_wf.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <array>

namespace shenmue::s2 {
namespace {
constexpr std::array<std::uint32_t, 3> itemBanks{1, 6, 2};
constexpr std::array<std::array<std::int8_t, 2>, 3> pageDimensions{{{5, 11}, {3, 4}, {2, 6}}};
constexpr std::array<float, 3> openPositions{80, 60, 180};
constexpr std::array<std::uint8_t, 3> helpTopics{61, 61, 49};
constexpr shen_name collectionOwner{SHEN_NAME(ICCL)};
LegacyCollectionWindow collectionWindow{};
LegacyCollectionWork work{};
float horizontalPosition{}, verticalPosition{}, velocity{};
std::int16_t pageCount{};
std::int32_t pendingPage{};
std::uint32_t selectionColour{};
bool selectionBrightening{};
LegacyIconSprite itemSprite{};

std::size_t ModeIndex() {
    return static_cast<std::size_t>(work.mode);
}
std::int32_t PageCapacity() {
    const auto &dimension = pageDimensions[ModeIndex()];
    return dimension[0] * dimension[1];
}
void UpdatePageCount() {
    pageCount = static_cast<std::int16_t>(std::min(PageCapacity(),work.count-PageCapacity()*work.page));
}
void UpdateWindowRows() {
    work.window->rows = static_cast<std::int8_t>(pageCount / work.window->columns);
    if (pageCount % work.window->columns)
        ++work.window->rows;
    verticalPosition = 214.0f - static_cast<float>(work.window->rows) * 20.0f;
}
void SelectCollectionItem() {
    const auto bank = itemBanks[ModeIndex()];
    const auto stock = S2Save_GetInventoryBank(static_cast<S2InventoryBank>(bank));
    const auto max = GetMaxItemsForItemType(bank);
    const auto first = work.page * PageCapacity();
    const auto selected = first + work.window->columns * work.window->row + work.window->column;
    work.selectedItem = work.firstItem = -1;
    std::int32_t visible{};
    for (std::uint32_t i = 0; i < max && i < stock.size(); ++i) {
        if (!std::to_integer<std::uint8_t>(stock[i]))
            continue;
        if (visible == first)
            work.firstItem = static_cast<std::int32_t>(i);
        if (visible == selected)
            work.selectedItem = static_cast<std::int32_t>(GetInventoryItemId(bank, i));
        ++visible;
    }
}
void PublishCollectionLabel() {
    const char *label = shenGetAmtOf(static_cast<std::uint32_t>(work.selectedItem));
    if (!label)
        label = "";
    work.window->labelTexture = -1;
    work.window->labelTexture = static_cast<std::int16_t>(PublishLegacyItemLabel(label, 512));
}
void DrawCollectionTitle(float x, float y, float depth) {
    if (work.window->labelTexture == -1)
        return;
    LegacySpritePacket packet{.quad = {.flags = 0x400}, .colour = 0xcdcdcdcdU, .secondaryColour = 0xff000000U, .uv = {0, 0, 1, 1}};
    TransformScreenSpace(
        packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(y), 512, 32, 1, 1, 0, ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint16_t>(work.window->labelTexture), depth);
}
void DrawCollectionWindow() {
    const float x = work.window->x, y = work.window->y;
    const std::int32_t columns = std::max<std::int32_t>(work.window->columns, 1);
    const std::int32_t rows = std::max<std::int32_t>(work.window->rows, 1);
    const float depth = work.window->depth - 0.0099999998f;
    const float height = static_cast<float>(rows) * 40.0f;
    DrawLegacyWindowBackground(x + 15, y + 4, static_cast<float>(columns) * 40, height + 48, depth - 0.0099999998f, 1024, ScreenMode::Centered);
    DrawLegacyWindowEdges(x + 11, y + 27, rows, columns, depth, 1024, ScreenMode::Centered);
    DrawLegacyWindowTiles(x + 35, y + 24, columns, rows, depth - 0.02f, 1024, ScreenMode::Centered);
    DrawLegacyWindowHorizontal(x, y, columns, 1, 0, depth, 1024, ScreenMode::Centered);
    const float separatorY = y + height + 8;
    DrawLegacyWindowHorizontal(x, separatorY, columns, 0, 0, depth, 1024, ScreenMode::Centered);
    const float bottomY = separatorY + 48;
    DrawLegacyWindowHorizontal(x, bottomY, columns, 1, 1, depth, 1024, ScreenMode::Centered);
    DrawLegacyWindowTiles(x + 35, bottomY - 24, columns, 1, depth - 0.02f, 1024, ScreenMode::Centered);
    DrawLegacyWindowEdges(x + 11, bottomY - 44, 1, columns, depth, 1024, ScreenMode::Centered);
    DrawCollectionTitle((static_cast<float>(columns) - 1) * 20 + x + 35, bottomY - 24, depth);
    if (work.window->quantity > 1)
        DrawLegacyItemQuantity(static_cast<float>(40 * columns) + x + 7, bottomY - 20, work.window->quantity, depth);
}
void DrawCollectionItems() {
    const auto mask = GetItemSelectorOwnedMask();
    const auto bank = itemBanks[ModeIndex()];
    const auto max = static_cast<std::int32_t>(GetMaxItemsForItemType(bank));
    std::int32_t drawn{}, column{};
    itemSprite.x = horizontalPosition + 35;
    itemSprite.y = verticalPosition + 24;
    itemSprite.depth = collectionWindow.depth;
    for (std::int32_t i = work.firstItem; i < max; ++i) {
        if (!(mask[static_cast<std::size_t>(i) / 8] & (1U << (i & 7))))
            continue;
        itemSprite.icon = GetInventoryItemIcon(bank, static_cast<std::uint32_t>(i));
        PrepareDrawWithMode(itemSprite, ScreenMode::Centered);
        ++column;
        itemSprite.x += 40;
        if (column >= work.window->columns) {
            column = 0;
            itemSprite.x = horizontalPosition + 35;
            itemSprite.y += 40;
        }
        if (++drawn >= pageCount)
            break;
    }
}
void DrawLegacyCollection_Callback(void *) {
    DrawLegacySelectionGlow(static_cast<float>(40 * work.window->column) + horizontalPosition + 35,
        static_cast<float>(40 * work.window->row) + verticalPosition + 24, collectionWindow.depth + 0.2f, selectionColour, selectionBrightening,
        ScreenMode::Centered);
    work.window->x = horizontalPosition;
    work.window->y = verticalPosition;
    DrawCollectionWindow();
    work.window->quantity = static_cast<std::int16_t>(GetItemState(static_cast<std::uint32_t>(work.selectedItem)));
    DrawCollectionItems();
}
bool NavigateCollection(std::uint16_t input) {
    auto &window = *work.window;
    const auto previousRow = window.row, previousColumn = window.column;
    if (pageCount <= 1)
        input = 0;
    if (pageCount <= window.columns)
        input &= 0xcfffU;
    if (window.columns * window.row + window.column == pageCount - 1 && window.column == 0)
        input &= 0x3fffU;
    if (input & 0xf000U) {
        if (input & 0x4000U) {
            if (--window.column < 0)
                window.column = static_cast<std::int8_t>(window.columns - 1);
        } else if (input & 0x8000U) {
            ++window.column;
            if (window.columns * window.row + window.column >= pageCount || window.column >= window.columns)
                window.column = 0;
        }
        if (input & 0x1000U) {
            if (--window.row < 0) {
                window.row = 0;
                while (window.column + window.columns * window.row < pageCount)
                    ++window.row;
                --window.row;
            }
        } else if (input & 0x2000U) {
            if (pageCount - window.column - window.columns * previousRow > window.columns) {
                ++window.row;
                if (window.row >= window.rows || window.column + window.columns * window.row >= pageCount)
                    window.row = 0;
            } else
                window.row = 0;
        }
        if (window.columns * window.row + window.column >= pageCount) {
            while (window.row > 0 && window.columns * window.row > pageCount)
                --window.row;
            window.column = static_cast<std::int8_t>(pageCount - window.row * window.columns - 1);
        }
    }
    return previousRow != window.row || previousColumn != window.column;
}
void UpdateCollectionInput() {
    std::uint16_t input = GetPadControlData(1);
    if (g_loadingFlag & 2U)
        input = 0;
    const std::uint16_t actions = work.selectedItem == -1 ? 512 : 1536;
    set_icon_select_flags_to(actions);
    if (input & 0x200U)
        work.phase = 6;
    else if (input & 0x400U) {
        if (actions & 0x400U) {
            work.flags &= ~1U;
            work.phase = 4;
            OpenItemLook(static_cast<std::uint32_t>(work.selectedItem), 1);
        }
    } else if (work.flags & 2U) {
        const auto pages = (work.count + PageCapacity() - 1) / PageCapacity();
        const auto previous = work.page;
        if (input & 0x40U)
            work.page = std::max(work.page - 1, 0);
        if (input & 0x80U)
            work.page = std::min(work.page+1,pages-1);
        if (work.page != previous) {
            pendingPage = work.page;
            work.page = previous;
            work.phase = 7;
            return;
        }
    }
    if (NavigateCollection(input)) {
        SelectCollectionItem();
        PublishCollectionLabel();
    }
}
void CompleteCollection() {
    work.phase = 0;
    work.flags &= ~1U;
    work.window->labelTexture = -1;
    horizontalPosition = -660;
    EV_EventSetUpHelpIcon_0(work.previousHelpTopic, work.previousHelpMode);
    CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
    icsToggleIconState(2, 0);
    ReleaseLegacyCollectionSprites(collectionOwner);
}
} // namespace

void InitializeLegacyCollectionMenu() {
    work.window = &collectionWindow;
    work.selectedItem = work.firstItem = -1;
    work.phase = 0;
    work.flags = 0;
    work.count = 0;
    collectionWindow = {.depth = 0.5f, .columns = 11, .rows = 5, .labelTexture = -1};
    horizontalPosition = -672;
    verticalPosition = 214;
    velocity = 1;
    selectionColour = 0;
    selectionBrightening = true;
    InitializeLegacyIconSprite(itemSprite);
    itemSprite.depth = 0.5f;
    InitializeFullMapMenu();
}
std::int64_t OpenLegacyCollectionMenu(std::int32_t mode) {
    work.phase = 0;
    work.flags = 0;
    const auto index = static_cast<std::uint8_t>(mode);
    if (index >= 3)
        return 0;
    work.count = static_cast<std::int32_t>(RefreshItemSelectorInventory(itemBanks[index]));
    if (!work.count)
        return 0;
    work.previousNpcLoad = static_cast<std::uint32_t>(GetNpcNewLoadState());
    EV_ResetFreeTalkFlags();
    EV_NPC_NewLoadSw(0);
    work.flags = 4;
    work.mode = index;
    work.phase = 9;
    icsToggleIconState(2, 1);
    work.window->row = work.window->column = 0;
    work.window->rows = pageDimensions[index][0];
    work.window->columns = pageDimensions[index][1];
    if (work.count > work.window->columns * work.window->rows)
        work.flags |= 2;
    return 1;
}
std::int64_t UpdateLegacyCollectionMenu() {
    std::int64_t done{};
    switch (work.phase) {
    case 9:
        work.phase = 10;
        break;
    case 10:
        work.phase = 1;
        break;
    case 1:
        LoadLegacyCollectionSprites(collectionOwner);
        EV_NPC_NewLoadSw(work.previousNpcLoad);
        work.flags &= ~4U;
        work.phase = 2;
        work.page = 0;
        UpdatePageCount();
        work.flags |= 1;
        horizontalPosition = -660;
        SelectCollectionItem();
        PublishCollectionLabel();
        work.previousHelpTopic = helpGetCurrentIconIdx(1);
        work.previousHelpMode = helpGetCurrentIconIdx(0);
        EV_EventSetUpHelpIcon_0(helpTopics[ModeIndex()], 0);
        CheckHelpIcon((32 * (work.flags & 2U)) | 0x21U, 0, CheckHelpIconOp::Set);
        UpdateWindowRows();
        break;
    case 2:
        if (!(g_loadingFlag & 2U) && AdvanceIconPosition(horizontalPosition, velocity, openPositions[ModeIndex()]))
            work.phase = 3;
        break;
    case 3:
        UpdateCollectionInput();
        break;
    case 4:
        if (!(GetCurrentItemSelectState() & 1U)) {
            work.flags |= 1;
            work.phase = 3;
        }
        break;
    case 6:
        if (!(g_loadingFlag & 2U)) {
            if (GetPadControlData(1) & 0x200U) {
                CompleteCollection();
                SetLegacyItemSelectorExitFlag();
                done = 1;
            } else if (RetreatIconPosition(horizontalPosition, velocity, -660)) {
                CompleteCollection();
                done = 1;
            }
        }
        break;
    case 7:
        if (RetreatIconPosition(horizontalPosition, velocity, -660)) {
            horizontalPosition = -660;
            work.phase = 8;
            work.page = pendingPage;
            UpdatePageCount();
            work.window->row = work.window->column = 0;
            UpdateWindowRows();
            const auto pages = (work.count + PageCapacity() - 1) / PageCapacity();
            std::uint32_t help = work.page ? 289U : 33U;
            if (work.page < pages - 1)
                help |= 0x40;
            CheckHelpIcon(help, 0, CheckHelpIconOp::Set);
            SelectCollectionItem();
            PublishCollectionLabel();
        }
        break;
    case 8:
        if (AdvanceIconPosition(horizontalPosition, velocity, openPositions[ModeIndex()])) {
            horizontalPosition = openPositions[ModeIndex()];
            work.phase = 3;
        }
        break;
    default:
        break;
    }
    if (work.flags & 1U)
        HLib::ExecuteCallbackNow(DrawLegacyCollection_Callback, nullptr);
    return done;
}
std::int64_t CloseLegacyCollectionMenu(std::int32_t force) {
    if (!work.phase || !force)
        return 0;
    if (GetCurrentItemSelectState() & 1U)
        CloseItemLook();
    if (work.flags & 4U)
        EV_NPC_NewLoadSw(work.previousNpcLoad);
    work.flags &= ~4U;
    work.window->labelTexture = -1;
    ReleaseLegacyCollectionSprites(collectionOwner);
    InitializeLegacyCollectionMenu();
    icsToggleIconState(2, 0);
    return 1;
}
} // namespace shenmue::s2
