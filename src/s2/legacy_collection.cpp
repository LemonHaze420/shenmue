#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/legacy_collection.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/hlib.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/item_selection_queries_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui_state_dc.hpp"
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

void DrawLegacyCollection_Callback(void *);

} // namespace

void InitializeLegacyCollectionMenu();
std::int64_t OpenLegacyCollectionMenu(std::int32_t mode);
std::int64_t UpdateLegacyCollectionMenu();
std::int64_t CloseLegacyCollectionMenu(std::int32_t force);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::size_t ModeIndex() {
    return static_cast<std::size_t>(work.mode);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t PageCapacity() {
    const auto &dimension = pageDimensions[ModeIndex()];
    return dimension[0] * dimension[1];
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void UpdatePageCount() {
    pageCount = static_cast<std::int16_t>(std::min(PageCapacity(), work.count - PageCapacity() * work.page));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void UpdateWindowRows() {
    work.window->rows = static_cast<std::int8_t>(pageCount / work.window->columns);
    if (pageCount % work.window->columns)
        ++work.window->rows;
    verticalPosition = 214.0f - static_cast<float>(work.window->rows) * 20.0f;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void PublishCollectionLabel() {
    const char *label = shenGetAmtOf(static_cast<std::uint32_t>(work.selectedItem));
    if (!label)
        label = "";
    work.window->labelTexture = -1;
    work.window->labelTexture = static_cast<std::int16_t>(PublishLegacyItemLabel(label, 512));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void DrawCollectionTitle(float x, float y, float depth) {
    if (work.window->labelTexture == -1)
        return;
    LegacySpritePacket packet{.quad = {.flags = 0x400}, .colour = 0xcdcdcdcdU, .secondaryColour = 0xff000000U, .uv = {0, 0, 1, 1}};
    TransformScreenSpace(
        packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(y), 512, 32, 1, 1, 0, ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint16_t>(work.window->labelTexture), depth);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint8_t helpGetCurrentIconIdx(std::int32_t slot);

static const std::uint8_t collectionPageDimensions[3][2] = {{5, 11}, {3, 4}, {2, 6}};

static const std::uint32_t collectionItemBanks[3] = {1, 6, 2};

static const float collectionOpenPositions[3] = {60.0f, 60.0f, 220.0f};

static const std::uint8_t collectionHelpTopics[3] = {0x3d, 0x3d, 0x31};

static std::uint8_t *const collectionStocks[3] = {g_capsuleStock, 0, g_photoStock};

static LegacyCollectionWindow collectionWindow;

static LegacyCollectionWork collectionWorkStorage;

static LegacyCollectionWork *collectionWork = &collectionWorkStorage;

static ItemEffectPosition collectionPosition;

static std::uint32_t collectionSelectionColour;

static bool collectionSelectionBrightening;

static std::int16_t collectionPageCount;

static std::int32_t collectionPendingPage;

static LegacyIconSprite collectionItemSprite;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t IsLegacyCollectionMenuOpen();
static void ResetLegacyCollectionLabel();
static void UpdateLegacyCollectionPageCount(LegacyCollectionWork &work);
static void SelectLegacyCollectionItem(LegacyCollectionWork &work);
static void SetUpLegacyCollectionHelp(LegacyCollectionWork &work);
static void UpdateLegacyCollectionRows(LegacyCollectionWindow &window);
static void DrawLegacyCollectionWindow(LegacyCollectionWindow &window);
static void DrawLegacyCollection_Callback(void *);
static std::int32_t NavigateLegacyCollection(LegacyCollectionWindow &window, std::uint16_t input, std::int16_t count);
static void UpdateLegacyCollectionInput();
static void CompleteLegacyCollection();
void InitializeLegacyCollectionMenu();
std::int32_t CloseLegacyCollectionMenu(std::int32_t force);
std::int32_t OpenLegacyCollectionMenu(std::int32_t mode);
std::uint8_t UpdateLegacyCollectionMenu();
#else
namespace {

std::size_t ModeIndex();
std::int32_t PageCapacity();
void UpdatePageCount();
void UpdateWindowRows();
void SelectCollectionItem();
void PublishCollectionLabel();
void DrawCollectionTitle(float x, float y, float depth);
void DrawCollectionWindow();
void DrawCollectionItems();
void DrawLegacyCollection_Callback(void *);
bool NavigateCollection(std::uint16_t input);
void UpdateCollectionInput();
void CompleteCollection();

} // namespace

void InitializeLegacyCollectionMenu();
std::int64_t CloseLegacyCollectionMenu(std::int32_t force);
std::int64_t OpenLegacyCollectionMenu(std::int32_t mode);
std::int64_t UpdateLegacyCollectionMenu();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void DrawLegacyCollectionWindow(LegacyCollectionWindow &window) {
    Vector2 label;
    std::int32_t rows = window.rows, columns = window.columns;
    const float depth = window.depth;
    if (columns <= 0)
        columns = 1;
    if (rows <= 0)
        rows = 1;
    const volatile float x = window.x;
    float y = window.y;
    const float left = x + 11.0f;
    DrawLegacyWindowBackground(left + 4.0f, 4.0f + y, static_cast<float>(columns * 40), static_cast<float>(rows * 40 + 48), depth - 1.0f, 1024);
    DrawLegacyWindowEdges(left, y + 27.0f, rows, columns, depth, 1024);
    const float tileInset = 24.0f;
    float labelX = left + tileInset;
    const float tileDepth = depth - 2.0f;
    DrawLegacyWindowTiles(labelX, y + tileInset, columns, rows, tileDepth, 1024);
    DrawLegacyWindowHorizontal(x, y, columns, 1, 0, depth, 1024);
    y += static_cast<float>(rows * 40 + 8);
    DrawLegacyWindowHorizontal(x, y, columns, 0, 0, depth, 1024);
    y += 48.0f;
    DrawLegacyWindowHorizontal(x, y, columns, 1, 1, depth, 1024);
    const float labelY = y - tileInset;
    DrawLegacyWindowTiles(labelX, labelY, columns, 1, tileDepth, 1024);
    y -= 44.0f;
    DrawLegacyWindowEdges(left, y, 1, columns, depth, 1024);
    labelX += static_cast<float>((columns - 1) * 20);
    label.x = labelX;
    label.y = labelY;
    DrawLegacyItemLabel(&window.labelTexture, &label, depth, 0xff000000U);
    if (window.quantity > 1)
        DrawLegacyItemQuantity(left + static_cast<float>(columns * 40) + 8.0f - 4.0f - 8.0f, 4.0f + labelY, window.quantity, depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void DrawLegacyCollection_Callback(void *) {
    LegacyCollectionWork *const work = collectionWork;
    LegacyCollectionWindow &window = *work->window;
    Vector2 glow;
    glow.x = collectionPosition.x + static_cast<float>(window.column * 40) + 11.0f + 24.0f;
    glow.y = static_cast<float>(window.row * 40) + collectionPosition.y + 24.0f;
    const float depth = collectionPosition.depth + 2.0f;
    DrawLegacySelectionGlow(glow, collectionSelectionColour, collectionSelectionBrightening, depth);
    window.x = collectionPosition.x;
    window.y = collectionPosition.y;
    DrawLegacyCollectionWindow(window);
    window.quantity = static_cast<std::int16_t>(GetItemState(work->selectedItem));
    DrawItemSelectorInventory(work->window->columns, collectionPageCount, collectionPosition.x + 35.0f, collectionPosition.y + 24.0f, collectionPosition.depth,
        work->firstItem, collectionItemBanks[work->mode]);
}

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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
            work.page = std::min(work.page + 1, pages - 1);
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void InitializeLegacyCollectionMenu() {
    collectionWork->phase = 0;
    collectionWork->window = &collectionWindow;
    collectionWork->count = 0;
    collectionWork->firstItem = collectionWork->selectedItem = -1;
    collectionPosition.x = -672.0f;
    collectionPosition.y = 214.0f;
    collectionPosition.velocity = 1.0f;
    collectionPosition.reservedScale = 0.0f;
    collectionPosition.depth = 5.0f;
    collectionPosition.selectedDirection = 4;
    collectionWindow.labelTexture = -1;
    collectionWindow.depth = 5.0f;
    collectionWindow.row = 0;
    collectionWindow.column = 0;
    collectionWindow.rows = 5;
    collectionWindow.columns = 11;
    collectionWindow.quantity = 0;
    collectionSelectionColour = 0;
    collectionSelectionBrightening = true;
    InitializeLegacyIconSprite(collectionItemSprite);
    collectionItemSprite.depth = collectionPosition.depth;
    InitializeFullMapMenu();
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t CloseLegacyCollectionMenu(std::int32_t force) {
    if (!IsLegacyCollectionMenuOpen() || !force)
        return 0;
    ResetLegacyCollectionLabel();
    InitializeLegacyCollectionMenu();
    return 1;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t IsLegacyCollectionMenuOpen() {
    if (collectionWork->phase)
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t OpenLegacyCollectionMenu(std::int32_t mode) {
    const std::int8_t index = static_cast<std::int8_t>(mode);
    collectionWork->phase = 0;
    if (index <= 2) {
        if ((collectionWork->count = static_cast<std::int32_t>(RefreshItemSelectorInventory(collectionItemBanks[index]))) != 0) {
            EV_ResetFreeTalkFlags();
            if (!LoadLegacyCollectionSprites(SHEN_NAME4('I', 'C', 'C', 'L')))
                return 0;
            collectionWork->mode = index;
            collectionWork->phase = 1;
            collectionWork->flags = 0;
            icsToggleIconState(2, 1);
            collectionWork->window->row = collectionWork->window->column = 0;
            collectionWork->window->rows = static_cast<std::int8_t>(collectionPageDimensions[collectionWork->mode][0]);
            collectionWork->window->columns = static_cast<std::int8_t>(collectionPageDimensions[collectionWork->mode][1]);
            if (collectionWork->count > collectionWork->window->rows * collectionWork->window->columns)
                collectionWork->flags |= 2U;
            return 1;
        }
    }
    return 0;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void SetUpLegacyCollectionHelp(LegacyCollectionWork &work) {
    work.previousHelpTopic = helpGetCurrentIconIdx(1);
    work.previousHelpMode = helpGetCurrentIconIdx(0);
    EV_EventSetUpHelpIcon_0(collectionHelpTopics[work.mode], 0);
    CheckHelpIcon(work.flags & 2U ? 0x61 : 0x21, 0, CheckHelpIconOp_Set);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static inline void UpdateLegacyCollectionRows(LegacyCollectionWindow &window) {
    window.rows = static_cast<std::int8_t>(collectionPageCount / window.columns);
    if (collectionPageCount != collectionPageCount / window.columns * window.columns)
        ++window.rows;
    collectionPosition.y = 214.0f - static_cast<float>(window.rows * 20);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t UpdateLegacyCollectionMenu() {
    LegacyCollectionWork *const work = collectionWork;
    LegacyCollectionWindow &window = *work->window;
    std::uint8_t done = 0;
    switch (work->phase) {
    case 0:
        break;
    case 1:
        collectionWork->phase = 2;
        collectionWork->page = 0;
        UpdateLegacyCollectionPageCount(*collectionWork);
        collectionWork->flags |= 1U;
        collectionPosition.x = -660.0f;
        SelectLegacyCollectionItem(*collectionWork);
        PrepareLegacyInventoryItemLabel(&collectionWork->window->labelTexture, collectionWork->selectedItem);
        SetUpLegacyCollectionHelp(*collectionWork);
        UpdateLegacyCollectionRows(window);
        break;
    case 2:
        if (!(g_loadingFlag & 2U) && AdvanceIconPosition(&collectionPosition, collectionOpenPositions[work->mode]))
            collectionWork->phase = 3;
        break;
    case 3:
        UpdateLegacyCollectionInput();
        break;
    case 7:
        if (RetreatIconPosition(&collectionPosition, -660.0f)) {
            std::uint32_t help = 33;
            collectionPosition.x = -660.0f;
            collectionWork->phase = 8;
            collectionWork->page = collectionPendingPage;
            UpdateLegacyCollectionPageCount(*work);
            window.row = 0;
            window.column = 0;
            UpdateLegacyCollectionRows(window);
            const std::int32_t capacity = collectionPageDimensions[collectionWork->mode][0] * collectionPageDimensions[collectionWork->mode][1];
            std::int32_t pages = collectionWork->count / capacity;
            if (collectionWork->count % capacity)
                ++pages;
            if (collectionWork->page)
                help |= 0x100U;
            if (collectionWork->page < pages - 1)
                help |= 0x40U;
            CheckHelpIcon(help, 0, CheckHelpIconOp_Set);
            SelectLegacyCollectionItem(*collectionWork);
            PrepareLegacyInventoryItemLabel(&collectionWork->window->labelTexture, collectionWork->selectedItem);
        }
        break;
    case 8:
        if (AdvanceIconPosition(&collectionPosition, collectionOpenPositions[collectionWork->mode])) {
            collectionPosition.x = collectionOpenPositions[collectionWork->mode];
            collectionWork->phase = 3;
        }
        break;
    case 4:
        if (!IsAnyItemSelectActive()) {
            ExecuteAicaCommand(0x100ab, 0, 0);
            collectionWork->phase = 3;
            collectionWork->flags |= 1U;
        }
        break;
    case 6:
        if (!(g_loadingFlag & 2U)) {
            if (GetPadControlData(1) & 0x200U) {
                CompleteLegacyCollection();
                SetLegacyItemSelectorExitFlag();
                done = 1;
            } else if (RetreatIconPosition(&collectionPosition, -660.0f)) {
                CompleteLegacyCollection();
                done = 1;
            }
        }
        break;
    default:
        break;
    }
    if (work->flags & 1U)
        HLib::ExecuteCallbackNow(DrawLegacyCollection_Callback, 0);
    return done;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void UpdateLegacyCollectionInput() {
    LegacyCollectionWork *const work = collectionWork;
    std::uint16_t input = GetPadControlData(1);
    if (shenIsGamePaused())
        input = 0;
    std::uint32_t actions = 0x200;
    if (work->selectedItem != -1)
        actions |= 0x400;
    RequestHelpIconSelection(actions);
    if (input & 0x200U) {
        collectionWork->phase = 6;
        ExecuteAicaCommand(0x100ab, 0, 0);
    } else if (input & 0x400U) {
        if (actions & 0x400U) {
            collectionWork->flags &= ~1U;
            collectionWork->phase = 4;
            OpenItemLook(work->selectedItem);
            ExecuteAicaCommand(0x500ab, 0, 0);
        }
    } else if (collectionWork->flags & 2U) {
        const std::int32_t capacity = collectionPageDimensions[collectionWork->mode][0] * collectionPageDimensions[collectionWork->mode][1];
        std::int32_t pages = collectionWork->count / capacity;
        if (collectionWork->count % capacity)
            ++pages;
        const std::int32_t previous = collectionWork->page;
        if (input & 0x40U) {
            collectionWork->page = previous - 1;
            if (collectionWork->page < 0)
                collectionWork->page = 0;
        }
        if (input & 0x80U) {
            collectionWork->page = collectionWork->page + 1;
            if (collectionWork->page >= pages)
                collectionWork->page = pages - 1;
        }
        if (previous != collectionWork->page) {
            collectionWork->phase = 7;
            collectionPendingPage = collectionWork->page;
            collectionWork->page = previous;
            return;
        }
    }
    if (NavigateLegacyCollection(*collectionWork->window, input, collectionPageCount)) {
        SelectLegacyCollectionItem(*collectionWork);
        PrepareLegacyInventoryItemLabel(&collectionWork->window->labelTexture, collectionWork->selectedItem);
        ExecuteAicaCommand(0x500ab, 0, 0);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void CompleteLegacyCollection() {
    LegacyCollectionWork *const work = collectionWork;
    work->phase = 0;
    ResetLegacyCollectionLabel();
    collectionWork->flags &= ~1U;
    collectionPosition.x = -660.0f;
    EV_EventSetUpHelpIcon_0(work->previousHelpTopic, work->previousHelpMode);
    CheckHelpIcon(33, 0, CheckHelpIconOp_Set);
    icsToggleIconState(2, 0);
    ReleaseLegacyCollectionSprites(SHEN_NAME(ICCL));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t NavigateLegacyCollection(LegacyCollectionWindow &window, std::uint16_t input, std::int16_t count) {
    const std::int8_t previousRow = window.row, previousColumn = window.column;
    if (count <= 1)
        input = 0;
    if (count <= window.columns)
        input &= 0xcfffU;
    if (window.row * window.columns + window.column == count - 1 && !window.column)
        input &= 0x3fffU;
    if (input & 0xf000U) {
        if (input & 0x4000U) {
            if (--window.column < 0)
                window.column = static_cast<std::int8_t>(window.columns - 1);
        } else if (input & 0x8000U) {
            ++window.column;
            if (window.row * window.columns + window.column >= count)
                window.column = 0;
            if (window.column >= window.columns)
                window.column = 0;
        }
        if (input & 0x1000U) {
            if (--window.row < 0) {
                window.row = 0;
                while (window.row * window.columns + window.column < count)
                    ++window.row;
                --window.row;
            }
        } else if (input & 0x2000U) {
            if (count - (window.row * window.columns + window.column) <= window.columns) {
                window.row = 0;
            } else {
                ++window.row;
                if (window.row >= window.rows)
                    window.row = 0;
                if (window.row * window.columns + window.column >= count)
                    window.row = 0;
            }
        }
        if (window.row * window.columns + window.column >= count) {
            while (window.row * window.columns > count) {
                if (--window.row <= 0) {
                    window.row = 0;
                    break;
                }
            }
            window.column = static_cast<std::int8_t>(count - static_cast<std::int8_t>(window.row * window.columns + 1));
        }
    }
    if ((previousColumn ^ window.column) | (window.row ^ previousRow))
        return 1;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void ResetLegacyCollectionLabel() {
    collectionWindow.labelTexture = -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void SelectLegacyCollectionItem(LegacyCollectionWork &work) {
    const LegacyCollectionWindow &window = *work.window;
    const std::int32_t mode = work.mode;
    volatile std::int32_t first = collectionPageDimensions[mode][1] * work.page;
    first = first * collectionPageDimensions[mode][0];
    const std::int32_t row = window.row * window.columns;
    std::int32_t selected = first;
    selected += row;
    selected += window.column;
    std::int32_t visible = 0;
    const volatile std::uint32_t bank = collectionItemBanks[mode];
    const std::int32_t count = static_cast<std::int32_t>(GetMaxItemsForItemType(bank));
    const std::uint8_t *stock = collectionStocks[work.mode];
    work.firstItem = work.selectedItem = -1;
    if (!stock)
        return;
    for (std::int32_t index = 0; index < count; ++index, ++stock) {
        if (!*stock)
            continue;
        if (visible == first)
            work.firstItem = index;
        if (visible == selected)
            work.selectedItem = static_cast<std::int32_t>(GetInventoryItemId(bank, index));
        ++visible;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void UpdateLegacyCollectionPageCount(LegacyCollectionWork &work) {
    const std::int32_t capacity = collectionPageDimensions[work.mode][1] * collectionPageDimensions[work.mode][0];
    const std::int32_t count = work.count;
    const std::int32_t page = work.page;
    if ((page + 1) * capacity >= count)
        collectionPageCount = static_cast<std::int16_t>(count - page * capacity);
    else
        collectionPageCount = static_cast<std::int16_t>(capacity);
}

#endif

} // namespace s2
} // namespace shenmue
