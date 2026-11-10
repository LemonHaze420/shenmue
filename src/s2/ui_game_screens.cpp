#include "s2/ui_game_screens.hpp"
#include "s2/ui_internal.hpp"
#include "s2/ui_pawn_legacy.hpp"
#include "s2/ui_scroll_list.hpp"
#include "s2/date_time.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/item_wf.hpp"
#include "s2/navigation.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/system_modules.hpp"
#include <bit>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {
std::int32_t gameCollectionItemLookOpen{}, gameCollectionState{}, gameCollectionPreviousTab{}, gameCollectionSubResult{};
std::uint32_t gameCollectionRequest{};
UIViewConfiguration gameCollectionConfiguration{-1, -1, -1};
std::int32_t gameCollectionCollection{-1}, gameCollectionMainWindowIndex{-1};
std::array<std::int32_t, GAME_COLLECTION_ITEM_CAPACITY> gameCollectionKeyItems{}, gameCollectionKeyItemCounts{};
std::array<std::int32_t, GAME_COLLECTION_ITEM_CAPACITY> gameCollectionCapsuleToys{}, gameCollectionCapsuleToyCounts{};
std::int32_t gameCollectionKeyItemCount{}, gameCollectionCapsuleToyCount{}, gameCollectionPendingOpen{}, gameCollectionReserved{};
std::int32_t gameCollectionInputEnabled{};
d3t::UILabelWidget *gameCollectionSelectionLabel{};
d3t::UIWindow *gameCollectionMainWindow{}, *gameCollectionMenuBackground{}, *gameCollectionButtonWindow{}, *gameCollectionNamePanel{};
d3t::UIWindow *gameCollectionMenuBarWindow{};
UIButtonPanel *gameCollectionButtonPanel{};
std::array<d3t::UIWindow *, 4> gameCollectionContentPanels{};
const std::array<std::int32_t *, 4> gameCollectionPageItems{
    gameCollectionKeyItems.data(), gameCollectionCapsuleToys.data(), gameCollectionCapsuleToys.data(), gameCollectionCapsuleToys.data()};
const std::array<std::int32_t *, 2> gameCollectionTabCounts{&gameCollectionKeyItemCount, &gameCollectionCapsuleToyCount};
const std::array<std::int32_t *, 2> gameCollectionTabItems{gameCollectionKeyItems.data(), gameCollectionCapsuleToys.data()};

std::int32_t collectionMapsState{};
std::uint32_t collectionMapsRequest{};
UIViewConfiguration collectionMapsConfiguration{-1, -1, -1};
std::int32_t collectionMapsSprite{-1}, collectionMapsSheet{-1}, collectionMapsTransition{-1}, collectionMapsWindowIndex{-1};
std::int32_t collectionMapsFolderCount{}, collectionMapsRowCount{}, collectionMapsFolderOpen{}, collectionMapsFolder{};
std::array<std::int32_t, 3> collectionMapsFolderEntryCounts{}, collectionMapsFolderAvailable{};
std::array<std::int32_t, 6> collectionMapsFolder0{};
std::array<std::int32_t, 8> collectionMapsFolder1{};
std::array<std::int32_t, 11> collectionMapsFolder2{};
const std::array<std::int32_t *, 4> collectionMapsEntries{
    collectionMapsFolderAvailable.data(), collectionMapsFolder0.data(), collectionMapsFolder1.data(), collectionMapsFolder2.data()};
d3t::UIWindow *collectionMapsMainWindow{}, *collectionMapsNamePanel{}, *collectionMapsListWindow{};

std::int32_t collectionPawnshopFolderOpen{}, collectionPawnshopState{}, collectionPawnshopShopCount{};
std::uint32_t collectionPawnshopRequest{};
UIViewConfiguration collectionPawnshopConfiguration{-1, -1, 4};
std::int32_t collectionPawnshopWindowIndex{-1}, collectionPawnshopTransition{-1};
d3t::UIViewSlot<kUIViewImageCapacity> *collectionPawnshopView{};
UIScrollList *collectionPawnshopList{};
d3t::UILabelWidget *collectionPawnshopSelectionLabel{};
std::array<std::int32_t, GAME_COLLECTION_PAWNSHOP_COUNT> collectionPawnshopShops{};
d3t::UIWindow *collectionPawnshopMainWindow{}, *collectionPawnshopNamePanel{}, *collectionPawnshopListWindow{};

std::int32_t gameWatchState{}, gameWatchClosed{}, gameWatchPendingOpen{};
std::uint32_t gameWatchRequest{};
UIViewConfiguration gameWatchConfiguration{-1, -1, -1};

std::int32_t gameQuitState{}, gameQuitPromptPending{};
std::uint32_t gameQuitRequest{};
UIViewConfiguration gameQuitConfiguration{-1, -1, 1};
std::int32_t gameHomeSubMenu{}, gameHomeSubMenuOpen{}, gameHomeQuitRequested{};
std::int32_t gameHomeTransition{-1}, gameHomeSubMenuTransition{-1};

namespace {
void SetTextAlphaFromWindow(nk_context &context, const d3t::UIWindow *window) {
    const nk_color colour = context.style.text.color;
    if (window) {
        const std::uint32_t value = ConstructColour(colour.r, colour.g, colour.b, static_cast<std::int32_t>(window->alpha * 255.0f));
        std::memcpy(&context.style.text.color, &value, sizeof(value));
    }
}
} // namespace

std::int64_t CloseGameCollection() {
    if (static_cast<std::uint32_t>(gameCollectionState - 2) <= 1U) {
        UIWidget_Animate(gameCollectionConfiguration.enabled, 0, 0.3f, 0.0f);
        const UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(gameCollectionConfiguration.status));
        if (menuBar && menuBar->field184 == 2)
            CloseCollectionMaps();
    }
    gameCollectionPendingOpen = 0;
    return 1;
}

void SetGameCollectionElement(std::int32_t record, std::int32_t slot) {
    UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!collection)
        return;
    const std::int32_t tab = collection->pages[collection->field94].field68;
    const std::int32_t *const items = gameCollectionPageItems[tab];
    UICollectionElement &element = collection->elements[slot];
    GetGameCollectionIconRect(tab, static_cast<std::uint32_t>(items[record]), &element.u0, &element.u1, &element.v0, &element.v1);
    element.u0 *= 0.125f;
    element.v0 *= 0.125f;
    element.u1 *= 0.125f;
    element.v1 *= 0.125f;
    element.image = GetGameCollectionIconImage(tab, static_cast<std::uint32_t>(items[record]));
}

void OpenGameCollection() {
    if (static_cast<std::uint32_t>(gameCollectionState - 2) > 1U) {
        gameCollectionPendingOpen = 1;
        return;
    }
    SetGameCollectionButtonPanel(1);
    gameCollectionReserved = 0;
    UIWidget_Animate(gameCollectionConfiguration.enabled, 1, 0.3f, 0.0f);
    UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(gameCollectionConfiguration.status));
    UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!menuBar || !collection)
        return;
    menuBar->field184 = 0;
    gameCollectionKeyItemCount = CollectOwnedKeyItems(gameCollectionKeyItems.data(), gameCollectionKeyItemCounts.data());
    gameCollectionCapsuleToyCount = CollectOwnedCapsuleToys(gameCollectionCapsuleToys.data(), gameCollectionCapsuleToyCounts.data());
    collection->field94 = 0;
    UI_SetCollectionCount(static_cast<std::uint32_t>(gameCollectionCollection), *gameCollectionTabCounts[0]);
    RefreshGameCollectionPage();
}

void SetGameCollectionContentVisible(std::int32_t visible) {
    for (d3t::UIWindow *const panel : gameCollectionContentPanels)
        if (panel)
            panel->alpha = static_cast<float>(visible);
    UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (collection)
        collection->field44[0] = visible ^ 1;
}

void SelectGameCollectionItem() {
    if (!(g_inputBuffer[0].pressedPriority & 0x0400U))
        return;
    const UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!collection)
        return;
    const std::int32_t tab = collection->pages[collection->field94].field68;
    const std::uint32_t item =
        GetInventoryItemId(static_cast<std::uint32_t>(tab), static_cast<std::uint32_t>(gameCollectionTabItems[tab][collection->field64[4]]));
    if (OpenItemLook(item, 1)) {
        SetGameCollectionInputEnabled(0);
        gameCollectionItemLookOpen = 1;
        SetUIRenderSuppression(1);
    }
}

namespace {
std::uint32_t GameCollectionIconCategory(std::int32_t tab) {
    if (tab == 1)
        return 1;
    if (tab == 2)
        return 3;
    if (tab == 3)
        return 5;
    return 0;
}
} // namespace

void GetGameCollectionIconRect(std::int32_t tab, std::uint32_t index, float *u0, float *u1, float *v0, float *v1) {
    const std::int16_t icon = static_cast<std::int16_t>(GetInventoryItemIcon(GameCollectionIconCategory(tab), index));
    const std::array<std::uint8_t, 4> rect = GetCommonIconRect(static_cast<std::uint16_t>(icon));
    *u0 = static_cast<float>(static_cast<std::int8_t>(rect[0]));
    *u1 = static_cast<float>(static_cast<std::int8_t>(rect[2]));
    *v0 = static_cast<float>(static_cast<std::int8_t>(rect[1]));
    *v1 = static_cast<float>(static_cast<std::int8_t>(rect[3]));
}

std::int32_t GetGameCollectionIconImage(std::int32_t tab, std::uint32_t index) {
    const UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!collection)
        return 0;
    const std::int32_t icon = static_cast<std::int16_t>(GetInventoryItemIcon(GameCollectionIconCategory(tab), index));
    const UITexturePage *const page = UI_GetTexturePage(static_cast<std::uint32_t>(collection->pages[collection->field94].texture));
    if (!page)
        return 0;
    for (std::int32_t entry = 0; entry < page->count; ++entry)
        if (icon >= page->min[entry] && icon < page->max[entry])
            return page->images[entry];
    return 0;
}

std::int64_t RenderGameCollection() {
    if (gameCollectionState != 3) {
        LoadGameCollection();
        if (static_cast<std::uint32_t>(gameCollectionState - 2) > 1U)
            return 0;
    }
    LoadCollectionMaps();
    const UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(gameCollectionConfiguration.status));
    const UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!menuBar || !collection)
        return 0;
    const char *label = "";
    if (collection->field64[0] > 0 && static_cast<std::uint32_t>(collection->field94) <= 1U)
        label = GetShenmueItem(static_cast<std::uint32_t>(GAME_COLLECTION_TAB_CATEGORIES[menuBar->field184]),
            static_cast<std::uint32_t>(gameCollectionTabItems[menuBar->field184][collection->field64[4]]));
    if (gameCollectionSelectionLabel)
        gameCollectionSelectionLabel->text = GetStringForUILabel(label);
    UpdateGameCollectionPanelAlpha();
    nk_context &context = *GetNativeUiContext();
    const nk_color colour = context.style.text.color;
    SetTextAlphaFromWindow(context, gameCollectionMainWindow);
    if (static_cast<std::uint32_t>(collection->field94) <= 1U)
        SetGameCollectionContentVisible(1);
    uiUpdateWithColour(context, static_cast<std::uint32_t>(gameCollectionConfiguration.uiHandleIndex));
    if (collection->field94 == 2) {
        SetGameCollectionContentVisible(0);
        EnableElement(UIElement::GameCollectionMaps);
    } else if (collection->field94 == 3) {
        SetGameCollectionContentVisible(0);
        EnableElement(UIElement::CollectionPawnshop);
    }
    context.style.text.color = colour;
    return 0;
}

void UpdateGameCollectionPanelAlpha() {
    if (!gameCollectionMainWindow)
        return;
    for (d3t::UIWindow *const panel : {gameCollectionMenuBackground, gameCollectionButtonWindow, gameCollectionNamePanel, gameCollectionMenuBarWindow})
        if (panel)
            panel->alpha = gameCollectionMainWindow->alpha;
}

std::int64_t ReloadGameCollection() {
    ResetUIViewSlot(gameCollectionConfiguration.uiHandleIndex);
    gameCollectionConfiguration.uiHandleIndex = -1;
    gameCollectionState = 0;
    gameCollectionRequest = 0;
    gameCollectionConfiguration.status = -1;
    gameCollectionCollection = -1;
    gameCollectionMainWindowIndex = -1;
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), gameCollectionConfiguration.uiHandleIndex);
    gameCollectionState = 2;
    return 2;
}

void BindGameCollection() {
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(static_cast<std::uint32_t>(gameCollectionConfiguration.uiHandleIndex));
    std::int32_t menuBarIndex = -1, collectionIndex = -1;
    gameCollectionConfiguration.status = -1;
    gameCollectionCollection = -1;
    gameCollectionMainWindowIndex = -1;
    if (view->widgetCount > 0) {
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] == 24)
                menuBarIndex = view->widgetPoolIndices[widget];
            if (view->widgetKinds[widget] == 25)
                collectionIndex = view->widgetPoolIndices[widget];
            if (menuBarIndex != -1 && collectionIndex != -1)
                break;
        }
        gameCollectionConfiguration.status = menuBarIndex;
        gameCollectionCollection = collectionIndex;
    }
    UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(menuBarIndex));
    if (menuBar)
        menuBar->field184 = 0;
    gameCollectionKeyItemCount = CollectOwnedKeyItems(gameCollectionKeyItems.data(), gameCollectionKeyItemCounts.data());
    gameCollectionCapsuleToyCount = CollectOwnedCapsuleToys(gameCollectionCapsuleToys.data(), gameCollectionCapsuleToyCounts.data());
    UI_SetCollectionCount(static_cast<std::uint32_t>(gameCollectionCollection), gameCollectionKeyItemCount);
    UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!collection)
        return;
    const std::int32_t *items = gameCollectionKeyItems.data();
    collection->pages[0].field5C = 1;
    std::memcpy(collection->pages[0].field60.data(), &items, sizeof(items));
    collection->pages[0].field68 = 0;
    items = gameCollectionCapsuleToys.data();
    collection->pages[1].field5C = 1;
    std::memcpy(collection->pages[1].field60.data(), &items, sizeof(items));
    collection->pages[1].field68 = 1;
    collection->field44[0] = 0;
    RefreshGameCollectionPage();
    const std::uint32_t viewIndex = static_cast<std::uint32_t>(gameCollectionConfiguration.uiHandleIndex);
    gameCollectionMainWindowIndex = UI_FindViewWindow(viewIndex, "GameCollection_Main_Panel");
    gameCollectionMainWindow = UI_GetWindow(gameCollectionMainWindowIndex);
    gameCollectionMenuBackground = UI_FindViewWindowPointer(viewIndex, "GameCollection_MenuBg_Panel");
    gameCollectionButtonWindow = UI_FindViewWindowPointer(viewIndex, "GameCollection_Button_Panel");
    gameCollectionNamePanel = UI_FindViewWindowPointer(viewIndex, "GameCollection_CollectionNamePanel");
    if (menuBar)
        gameCollectionMenuBarWindow = UI_GetWindow(menuBar->window);
    gameCollectionConfiguration.enabled = UIWidget_Bind(gameCollectionMainWindowIndex, 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
    if (gameCollectionPendingOpen)
        OpenGameCollection();
    gameCollectionPendingOpen = 0;
    gameCollectionSelectionLabel = UI_GetLabel(static_cast<std::uint32_t>(UI_FindViewLabel(viewIndex, "Selection_Name")));
    gameCollectionSelectionLabel->token.alreadyLocalized = 1;
    gameCollectionButtonPanel = UI_FindViewButtonPanel(viewIndex, "GameCollection_Button_Panel");
    if (gameCollectionButtonPanel || (gameCollectionButtonPanel = UI_FindViewButtonPanel(viewIndex, "GameCollection_Button_Panel")) != nullptr) {
        gameCollectionButtonPanel->entries[0].field0 = 60;
        gameCollectionButtonPanel->entries[0].field4 = 1;
        std::snprintf(gameCollectionButtonPanel->entries[0].field8.data(), 64, "%s", GetStringForUILabel("UIGNRSCROL"));
        gameCollectionButtonPanel->entries[1].field0 = 0;
        gameCollectionButtonPanel->entries[1].field4 = 1;
        std::snprintf(gameCollectionButtonPanel->entries[1].field8.data(), 64, "%s", GetStringForUILabel("UIGNRLSLCT"));
    }
    for (std::size_t panel = 0; panel < gameCollectionContentPanels.size(); ++panel)
        gameCollectionContentPanels[panel] = UI_FindViewWindowPointer(viewIndex, GAME_COLLECTION_CONTENT_PANELS[panel].data());
}

std::int64_t GetGameCollectionInitState() {
    return static_cast<std::uint32_t>(gameCollectionState);
}

void SetGameCollectionButtonPanel(std::int32_t select) {
    if (!gameCollectionButtonPanel &&
        !(gameCollectionButtonPanel =
                UI_FindViewButtonPanel(static_cast<std::uint32_t>(gameCollectionConfiguration.uiHandleIndex), "GameCollection_Button_Panel")))
        return;
    gameCollectionButtonPanel->entries[0].field0 = 60;
    gameCollectionButtonPanel->entries[0].field4 = 1;
    std::snprintf(gameCollectionButtonPanel->entries[0].field8.data(), 64, "%s", GetStringForUILabel("UIGNRSCROL"));
    if (select == 1) {
        gameCollectionButtonPanel->entries[1].field0 = 0;
        gameCollectionButtonPanel->entries[1].field4 = 1;
        std::snprintf(gameCollectionButtonPanel->entries[1].field8.data(), 64, "%s", GetStringForUILabel("UIGNRLSLCT"));
    } else {
        gameCollectionButtonPanel->entries[1].field0 = -1;
        gameCollectionButtonPanel->entries[1].field4 = 1;
        std::snprintf(gameCollectionButtonPanel->entries[1].field8.data(), 64, " ");
    }
}

std::int64_t GameCollectionInput() {
    if (static_cast<std::uint32_t>(gameCollectionState - 2) > 1U)
        return 0;
    if (!UI_GetMenuInputBlock()) {
        UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(gameCollectionConfiguration.status));
        UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
        if (!menuBar || !collection)
            return 0;
        gameCollectionPreviousTab = menuBar->field184;
        UI_UpdateMenuBarSelection(static_cast<std::uint32_t>(gameCollectionConfiguration.status), 16, 8);
        const std::int32_t tab = menuBar->field184;
        if (tab != gameCollectionPreviousTab) {
            collection->field94 = tab;
            collection->field64[3] = 0;
            collection->field64[4] = 0;
            if (menuBar->field184 >= 0) {
                if (menuBar->field184 <= 1) {
                    UI_SetCollectionCount(static_cast<std::uint32_t>(gameCollectionCollection), *gameCollectionTabCounts[collection->field94]);
                    RefreshGameCollectionPage();
                } else if (menuBar->field184 == 2) {
                    OpenCollectionMaps();
                } else if (menuBar->field184 == 3) {
                    OpenCollectionPawnshop();
                }
            }
        }
        const std::int32_t current = menuBar->field184;
        if (current >= 0) {
            if (current <= 1) {
                if (*gameCollectionTabCounts[current] > 0) {
                    const std::int32_t page = collection->field64[3];
                    UI_UpdateCollectionInput(static_cast<std::uint32_t>(gameCollectionCollection));
                    if (page != collection->field64[3])
                        RefreshGameCollectionPage();
                }
                if (!gameCollectionItemLookOpen)
                    SelectGameCollectionItem();
            } else if (current == 2) {
                gameCollectionSubResult = CollectionMapsInput();
            } else if (current == 3) {
                gameCollectionSubResult = CollectionPawnshopInput();
            }
        }
    }
    if (gameCollectionItemLookOpen == 1 && (g_inputBuffer[0].pressedPriority & 0x0200U)) {
        SetUIRenderSuppression(0);
        SetGameCollectionInputEnabled(1);
        gameCollectionItemLookOpen = 0;
        g_inputBuffer[0].pressedPriority = 0;
    }
    return 0;
}

std::uint32_t LoadGameCollection() {
    if (gameCollectionState == 0)
        gameCollectionState = LoadUIFile("/ui/gamesubmenus/gamecollection/GameCollection.ui", 46, &gameCollectionRequest);
    else if (gameCollectionState == 1) {
        gameCollectionState = GetNewUIFileInitFor(&gameCollectionRequest, gameCollectionConfiguration);
        if (gameCollectionState == 2)
            BindGameCollection();
    } else if (gameCollectionState == 2)
        gameCollectionState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(gameCollectionConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(gameCollectionState);
}

void RefreshGameCollectionPage() {
    UICollection *const collection = UI_GetCollection(static_cast<std::uint32_t>(gameCollectionCollection));
    if (!collection || !collection->pages[collection->field94].field5C)
        return;
    for (std::int32_t row = 0; row < collection->pages[collection->field94].rows; ++row) {
        for (std::int32_t column = 0; column < collection->pages[collection->field94].columns; ++column) {
            const UICollectionPage &page = collection->pages[collection->field94];
            const std::int32_t slot = column + row * page.columns;
            const std::int32_t record = column + page.columns * (row + collection->field64[3] * page.rows);
            collection->elements[slot].count = record;
            SetGameCollectionElement(record, slot);
        }
    }
}

void SetCollectionMapsSheet(std::int32_t sheet) {
    UIButtonIconWidget *const sprite = UI_GetButtonIconWidget(static_cast<std::uint32_t>(collectionMapsSprite));
    if (!sprite)
        return;
    sprite->sheet = sheet;
    const d3t::UISpriteSheet *const record = UI_GetSpriteSheet(static_cast<std::uint32_t>(sheet));
    if (record)
        std::snprintf(sprite->image.data(), sprite->image.size(), "%s", record->imageName);
}

std::int64_t CloseCollectionMaps() {
    if (static_cast<std::uint32_t>(collectionMapsState - 2) <= 1U)
        UIWidget_Animate(collectionMapsTransition, 0, 0.3f, 0.0f);
    return 1;
}

void BuildCollectionMapFolders() {
    std::int32_t folders = 0;
    collectionMapsFolderAvailable.fill(-1);
    collectionMapsFolder0.fill(-1);
    collectionMapsFolder1.fill(-1);
    collectionMapsFolder2.fill(-1);
    for (std::int32_t folder = 0; folder < 3; ++folder) {
        const std::int32_t count = NaviFolderEntryCount(folder, 2);
        if (count <= 0)
            continue;
        collectionMapsFolderAvailable[folder] = 1;
        const std::uint32_t mask = NaviFolderEntryMask(folder, 2);
        std::int32_t available = 0;
        std::uint32_t bit = 1;
        std::int32_t *const entries = collectionMapsEntries[folder + 1];
        for (std::int32_t entry = 0; entry < count; ++entry) {
            if (bit & mask) {
                entries[entry] = entry;
                ++available;
            } else {
                entries[entry] = -1;
            }
            bit = std::rotl(bit, 1);
        }
        collectionMapsFolderEntryCounts[folder] = available;
        if (available > 0)
            ++folders;
    }
    collectionMapsRowCount = folders;
    collectionMapsFolderCount = folders;
}

void OpenCollectionMaps() {
    if (static_cast<std::uint32_t>(collectionMapsState - 2) > 1U)
        return;
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(collectionMapsConfiguration.status));
    list->field74 = {0, 0};
    list->field3CC = 0;
    BuildCollectionMapFolders();
    RefreshCollectionMapRows();
    if (collectionMapsMainWindow)
        collectionMapsMainWindow->alpha = 1.0f;
}

void BindCollectionMaps() {
    const std::uint32_t viewIndex = static_cast<std::uint32_t>(collectionMapsConfiguration.uiHandleIndex);
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(viewIndex);
    std::int32_t listIndex = -1, spriteIndex = -1;
    collectionMapsConfiguration.status = -1;
    collectionMapsSprite = -1;
    if (view->widgetCount > 0) {
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] == 27)
                listIndex = view->widgetPoolIndices[widget];
            if (view->widgetKinds[widget] == 20)
                spriteIndex = view->widgetPoolIndices[widget];
            if (listIndex != -1 && spriteIndex != -1)
                break;
        }
        collectionMapsConfiguration.status = listIndex;
        collectionMapsSprite = spriteIndex;
    }
    BuildCollectionMapFolders();
    RefreshCollectionMapRows();
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(collectionMapsConfiguration.status));
    if (list) {
        list->field3A8 = UIWidget_Bind(collectionMapsConfiguration.status, 1, 2, {0.0f, 0.0f}, {-68.0f, 0.0f});
        list->field3D4 = UIWidget_Bind(collectionMapsConfiguration.status, 2, 2, {0.0f, 0.0f}, {1.0f, 0.0f});
    }
    const UIButtonIconWidget *const sprite = UI_GetButtonIconWidget(static_cast<std::uint32_t>(collectionMapsSprite));
    if (sprite)
        collectionMapsSheet = sprite->sheet;
    collectionMapsWindowIndex = UI_FindViewWindow(viewIndex, "CollectionMaps_Main_Panel");
    collectionMapsMainWindow = UI_GetWindow(collectionMapsWindowIndex);
    collectionMapsNamePanel = UI_FindViewWindowPointer(viewIndex, "CollectionMaps_Maps_Name_Panel");
    if (list)
        collectionMapsListWindow = UI_GetWindow(list->window);
    collectionMapsTransition = UIWidget_Bind(collectionMapsWindowIndex, 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
}

std::int64_t RenderCollectionMaps() {
    if (collectionMapsState != 3) {
        LoadCollectionMaps();
        if (static_cast<std::uint32_t>(collectionMapsState - 2) > 1U)
            return 0;
    }
    if (collectionMapsMainWindow) {
        if (collectionMapsNamePanel)
            collectionMapsNamePanel->alpha = collectionMapsMainWindow->alpha;
        if (collectionMapsListWindow)
            collectionMapsListWindow->alpha = collectionMapsMainWindow->alpha;
    }
    uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(collectionMapsConfiguration.uiHandleIndex));
    return 0;
}

std::int64_t GetCollectionMapsInitState() {
    return static_cast<std::uint32_t>(collectionMapsState);
}

std::int32_t CollectionMapsInput() {
    if (collectionMapsState != 3)
        return 0;
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(collectionMapsConfiguration.status));
    const std::int32_t top = list->field74[0];
    UI_UpdateScrollListSelection(static_cast<std::uint32_t>(collectionMapsConfiguration.status));
    if (list->field74[0] != top)
        RefreshCollectionMapRows();
    UIButtonIconWidget *const sprite = UI_GetButtonIconWidget(static_cast<std::uint32_t>(collectionMapsSprite));
    if (!sprite)
        return 0;
    sprite->sprite = list->field74[0] + list->field74[1];
    sprite->fieldAC = 0;
    if (!collectionMapsFolderOpen) {
        if (g_inputBuffer[0].pressedPriority & 0x0400U) {
            collectionMapsFolder = list->field74[1];
            collectionMapsFolderOpen = 1;
            collectionMapsRowCount = GAME_COLLECTION_MAP_FOLDER_SIZES[collectionMapsFolder];
            SetCollectionMapsSheet(collectionMapsSheet + 1 + collectionMapsFolder);
            list->field74 = {0, 0};
            list->field3CC = 0;
            RefreshCollectionMapRows();
            SetGameCollectionButtonPanel(0);
        }
        return 0;
    }
    sprite->fieldAC = collectionMapsEntries[collectionMapsFolder + 1][list->field74[0] + list->field74[1]] == -1;
    if (g_inputBuffer[0].pressedPriority & 0x0200U) {
        collectionMapsFolder = list->field74[1];
        collectionMapsFolderOpen = 0;
        collectionMapsRowCount = 3;
        SetCollectionMapsSheet(collectionMapsSheet);
        list->field74 = {0, 0};
        list->field3CC = 0;
        RefreshCollectionMapRows();
        SetGameCollectionButtonPanel(1);
    }
    return 1;
}

std::uint32_t LoadCollectionMaps() {
    if (collectionMapsState == 0)
        collectionMapsState = LoadUIFile("/ui/gamesubmenus/gamecollection/CollectionMaps.ui", 46, &collectionMapsRequest);
    else if (collectionMapsState == 1) {
        collectionMapsState = GetNewUIFileInitFor(&collectionMapsRequest, collectionMapsConfiguration);
        if (collectionMapsState == 2)
            BindCollectionMaps();
    } else if (collectionMapsState == 2)
        collectionMapsState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(collectionMapsConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(collectionMapsState);
}

namespace {
const char *CollectionMapEntryText(std::int32_t folder, std::int32_t entry) {
    return collectionMapsEntries[folder + 1][entry] <= -1 ? "UIGNRLQSTN" : GetNaviFolderEntryLabel(folder, entry);
}
} // namespace

void RefreshCollectionMapRows() {
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(collectionMapsConfiguration.status));
    list->texts.fill("");
    if (!collectionMapsFolderOpen) {
        for (std::int32_t folder = 0; folder < 3; ++folder)
            list->texts[folder] = collectionMapsFolderAvailable[folder] ? GetNaviFolderLabel(folder) : "UIGNRLQSTN";
        list->field6C = 3;
        return;
    }
    const std::int32_t visible = collectionMapsRowCount < list->rows ? collectionMapsRowCount : list->rows;
    std::int32_t row = 0;
    for (; row < visible; ++row)
        list->texts[row] = CollectionMapEntryText(collectionMapsFolder, row + list->field74[0]);
    const std::int32_t top = list->field74[0];
    if (collectionMapsRowCount > list->rows + top + 1)
        list->texts[row] = CollectionMapEntryText(collectionMapsFolder, top + row);
    if (list->field74[0] > 0)
        list->texts[row + 1] = CollectionMapEntryText(collectionMapsFolder, list->field74[0] - 1);
    UIButtonIconWidget *const sprite = UI_GetButtonIconWidget(static_cast<std::uint32_t>(collectionMapsSprite));
    if (collectionMapsEntries[collectionMapsFolder + 1][list->field74[0] + list->field74[1]] == -1)
        sprite->fieldAC = 1;
    list->field6C = collectionMapsRowCount;
}

namespace {
void BuildCollectionPawnshopShops() {
    const std::uint32_t mask = GetPawnWindowShopMask();
    std::int32_t count = 0;
    std::uint32_t bit = 1;
    for (std::int32_t shop = 0; shop < GAME_COLLECTION_PAWNSHOP_COUNT; ++shop) {
        if (bit & mask) {
            collectionPawnshopShops[shop] = shop;
            ++count;
        } else {
            collectionPawnshopShops[shop] = -1;
        }
        bit = std::rotl(bit, 1);
    }
    collectionPawnshopShopCount = count;
}
} // namespace

void OpenCollectionPawnshop() {
    if (static_cast<std::uint32_t>(collectionPawnshopState - 2) > 1U)
        return;
    if (collectionPawnshopSelectionLabel)
        collectionPawnshopSelectionLabel->text = "";
    collectionPawnshopFolderOpen = 0;
    if (collectionPawnshopList) {
        collectionPawnshopList->field64 = 0;
        collectionPawnshopList->field74 = {0, 0};
        UIWidget_Retarget(collectionPawnshopList->field3A8, {0.0f, 0.0f}, {-68.0f, 0.0f});
    }
    BuildCollectionPawnshopShops();
    RefreshCollectionPawnshopRows();
    if (collectionPawnshopMainWindow)
        collectionPawnshopMainWindow->alpha = 1.0f;
}

void BindCollectionPawnshop() {
    const std::uint32_t viewIndex = static_cast<std::uint32_t>(collectionPawnshopConfiguration.uiHandleIndex);
    collectionPawnshopView = GetNuklearElementByIdx(viewIndex);
    if (!collectionPawnshopView)
        return;
    collectionPawnshopConfiguration.enabled = UI_FindViewScrollList(viewIndex, "Pawn_Scroll_List");
    collectionPawnshopList = UI_GetScrollList(static_cast<std::uint32_t>(collectionPawnshopConfiguration.enabled));
    collectionPawnshopSelectionLabel = UI_GetLabel(static_cast<std::uint32_t>(UI_FindViewLabel(viewIndex, "Selection_Name_Label")));
    if (collectionPawnshopList) {
        collectionPawnshopList->field3A8 = UIWidget_Bind(collectionPawnshopConfiguration.enabled, 1, 2, {0.0f, 0.0f}, {-68.0f, 0.0f});
        collectionPawnshopList->field3D4 = UIWidget_Bind(collectionPawnshopConfiguration.enabled, 2, 2, {0.0f, 0.0f}, {1.0f, 0.0f});
    }
    BuildCollectionPawnshopShops();
    RefreshCollectionPawnshopRows();
    if (collectionPawnshopSelectionLabel)
        collectionPawnshopSelectionLabel->text = "";
    collectionPawnshopWindowIndex = UI_FindViewWindow(viewIndex, "CollectionPawnshop_Pawn_Name_Panel");
    collectionPawnshopMainWindow = UI_GetWindow(collectionPawnshopWindowIndex);
    collectionPawnshopNamePanel = UI_FindViewWindowPointer(viewIndex, "CollectionPawnshop_Pawn_Name_Panel");
    if (collectionPawnshopList)
        collectionPawnshopListWindow = UI_GetWindow(collectionPawnshopList->window);
    collectionPawnshopTransition = UIWidget_Bind(collectionPawnshopWindowIndex, 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
}

std::int64_t RenderCollectionPawnshop() {
    if (collectionPawnshopState != 3) {
        LoadCollectionPawnshop();
        if (static_cast<std::uint32_t>(collectionPawnshopState - 2) > 1U)
            return 0;
    }
    if (collectionPawnshopMainWindow) {
        if (collectionPawnshopNamePanel)
            collectionPawnshopNamePanel->alpha = collectionPawnshopMainWindow->alpha;
        if (collectionPawnshopListWindow)
            collectionPawnshopListWindow->alpha = collectionPawnshopMainWindow->alpha;
    }
    uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(collectionPawnshopConfiguration.uiHandleIndex));
    return 0;
}

std::int64_t GetCollectionPawnshopInitState() {
    return static_cast<std::uint32_t>(collectionPawnshopState);
}

std::int32_t CollectionPawnshopInput() {
    if (static_cast<std::uint32_t>(collectionPawnshopState - 2) > 1U || !collectionPawnshopList)
        return 0;
    std::int32_t rows = 0;
    const std::int32_t top = collectionPawnshopList->field74[0];
    if (collectionPawnshopFolderOpen == 1) {
        rows = collectionPawnshopList->rows;
        collectionPawnshopList->rows = 1;
    }
    UI_UpdateScrollListSelection(static_cast<std::uint32_t>(collectionPawnshopConfiguration.enabled));
    if (collectionPawnshopFolderOpen == 1)
        collectionPawnshopList->rows = rows;
    if (collectionPawnshopList->field74[0] != top)
        RefreshCollectionPawnshopRows();
    if (!collectionPawnshopFolderOpen) {
        const std::int32_t shop = collectionPawnshopList->field74[0] + collectionPawnshopList->field74[1];
        if (collectionPawnshopShops[shop] > -1 && (g_inputBuffer[0].pressedPriority & 0x0400U)) {
            ConfigureLegacyPawnFlyer(shop, PawnFlyerInspect);
            SetLegacyPawnFlyerResult(1);
            SetUIRenderSuppression(1);
        }
        return 0;
    }
    if (g_inputBuffer[0].pressedPriority & 0x0200U) {
        if (collectionPawnshopSelectionLabel)
            collectionPawnshopSelectionLabel->text = "";
        collectionPawnshopFolderOpen = 0;
        collectionPawnshopList->field64 = 0;
        collectionPawnshopList->field74 = {0, 0};
        RefreshCollectionPawnshopRows();
        UIWidget_Retarget(collectionPawnshopList->field3A8, {0.0f, 0.0f}, {-68.0f, 0.0f});
    }
    return 1;
}

std::uint32_t LoadCollectionPawnshop() {
    if (collectionPawnshopState == 0)
        collectionPawnshopState = LoadUIFile("/ui/gamesubmenus/gamecollection/CollectionPawnshop.ui", 46, &collectionPawnshopRequest);
    else if (collectionPawnshopState == 1) {
        collectionPawnshopState = GetNewUIFileInitFor(&collectionPawnshopRequest, collectionPawnshopConfiguration);
        if (collectionPawnshopState == 2)
            BindCollectionPawnshop();
    } else if (collectionPawnshopState == 2)
        collectionPawnshopState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(collectionPawnshopConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(collectionPawnshopState);
}

namespace {
const char *CollectionPawnshopText(std::int32_t shop) {
    return collectionPawnshopShops[shop] <= -1 ? GetStringForUILabel("UIGNRLQSTN") : GetPawnFlyerShopLabel(shop);
}
} // namespace

void RefreshCollectionPawnshopRows() {
    UIScrollList *const list = collectionPawnshopList;
    if (!list || collectionPawnshopFolderOpen)
        return;
    list->rows = list->rowCount;
    std::int32_t row = 0;
    for (; row < list->rowCount; ++row) {
        list->texts[row] = CollectionPawnshopText(row + list->field74[0]);
        list->words[row] = -1;
    }
    list->field6C = GAME_COLLECTION_PAWNSHOP_COUNT;
    if (row + list->field74[0] < GAME_COLLECTION_PAWNSHOP_COUNT) {
        list->words[row] = -1;
        list->texts[row] = CollectionPawnshopText(row + list->field74[0]);
    }
    if (list->field74[0] > 0) {
        list->words[row + 1] = -1;
        list->texts[row + 1] = CollectionPawnshopText(list->field74[0] - 1);
    }
}

void SetGameCollectionInputEnabled(std::int32_t enabled) {
    gameCollectionInputEnabled = enabled;
}

std::int64_t CloseGameWatch() {
    if (static_cast<std::uint32_t>(gameWatchState - 2) <= 1U && !gameWatchClosed)
        UIWidget_Animate(gameWatchConfiguration.enabled, 0, 0.3f, 0.0f);
    gameWatchClosed = 1;
    if (GetHandWatchState() == 2)
        SetHandWatchState(3);
    gameWatchPendingOpen = 0;
    return 1;
}

void OpenGameWatch() {
    if (static_cast<std::uint32_t>(gameWatchState - 2) > 1U) {
        gameWatchPendingOpen = 1;
        return;
    }
    gameWatchClosed = 0;
    OpenWristwatch();
    UIWidget_Animate(gameWatchConfiguration.enabled, 1, 0.3f, 0.0f);
}

std::int64_t RenderGameWatch() {
    if (gameWatchState != 3) {
        LoadGameWatch();
        if (static_cast<std::uint32_t>(gameWatchState - 2) > 1U)
            return 0;
    }
    nk_context &context = *GetNativeUiContext();
    const nk_color colour = context.style.text.color;
    SetTextAlphaFromWindow(context, UI_GetWindow(gameWatchConfiguration.status));
    uiUpdateWithColour(context, static_cast<std::uint32_t>(gameWatchConfiguration.uiHandleIndex));
    context.style.text.color = colour;
    return 0;
}

void BindGameWatch() {
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(static_cast<std::uint32_t>(gameWatchConfiguration.uiHandleIndex));
    gameWatchConfiguration.status = -1;
    for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
        if (view->widgetKinds[widget] == 1) {
            gameWatchConfiguration.status = view->widgetPoolIndices[widget];
            break;
        }
    }
    gameWatchConfiguration.enabled = UIWidget_Bind(gameWatchConfiguration.status, 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
    if (gameWatchPendingOpen && static_cast<std::uint32_t>(gameWatchState - 2) <= 1U) {
        gameWatchClosed = 0;
        OpenWristwatch();
        UIWidget_Animate(gameWatchConfiguration.enabled, 1, 0.3f, 0.0f);
    }
    gameWatchPendingOpen = 0;
}

std::int64_t GameWatchInputNullsub() {
    return 0;
}

std::int64_t GetGameWatchInitState() {
    return static_cast<std::uint32_t>(gameWatchState);
}

std::uint32_t LoadGameWatch() {
    if (gameWatchState == 0)
        gameWatchState = LoadUIFile("/ui/gamesubmenus/gamewatch/GameWatch.ui", 47, &gameWatchRequest);
    else if (gameWatchState == 1) {
        gameWatchState = GetNewUIFileInitFor(&gameWatchRequest, gameWatchConfiguration);
        if (gameWatchState == 2)
            BindGameWatch();
    } else if (gameWatchState == 2)
        gameWatchState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(gameWatchConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(gameWatchState);
}

void RequestGameHomeQuit() {
    gameHomeQuitRequested = 1;
}

std::int64_t CloseGameHomeSubMenu() {
    std::int64_t result = 0;
    if (gameHomeSubMenuOpen == 1) {
        UIWidget_Animate(gameHomeTransition, 0, 0.3f, 0.0f);
        UIWidget_Animate(gameHomeSubMenuTransition, 0, 0.3f, 0.0f);
        result = DestroyElementByIndex(static_cast<UIElement>(GAME_HOME_SUBMENUS[gameHomeSubMenu]));
    }
    gameHomeSubMenuOpen = 0;
    return result;
}

std::int64_t CloseGameQuit() {
    gameQuitConfiguration.status = 0;
    return 1;
}

void ConfirmGameQuit() {
    gameQuitConfiguration.status = 0;
    RequestGameHomeQuit();
    CloseGameHomeSubMenu();
    ResetToTitleScreen();
}

void OpenGameQuit() {
    gameQuitConfiguration.status = 1;
}

std::int64_t RenderGameQuit() {
    if (gameQuitState != 3) {
        LoadGameQuit();
        if (static_cast<std::uint32_t>(gameQuitState - 2) > 1U)
            return 0;
    }
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(gameQuitConfiguration.enabled));
    list->rows = 1;
    list->field6C = 1;
    list->texts[0] = GetStringForUILabel("UIQUITLABL");
    uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(gameQuitConfiguration.uiHandleIndex));
    if (gameQuitPromptPending == 1)
        EnableElement(UIElement::YesNoPrompt);
    return 0;
}

std::int64_t ReloadGameQuit() {
    ResetUIViewSlot(gameQuitConfiguration.uiHandleIndex);
    gameQuitConfiguration.uiHandleIndex = -1;
    gameQuitState = 0;
    gameQuitRequest = 0;
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), gameQuitConfiguration.uiHandleIndex);
    gameQuitState = 2;
    return 2;
}

void BindGameQuit() {
    const std::uint32_t viewIndex = static_cast<std::uint32_t>(gameQuitConfiguration.uiHandleIndex);
    gameQuitConfiguration.enabled = UI_FindViewScrollList(viewIndex, "ScrollList");
    UIScrollList *list = UI_GetScrollList(static_cast<std::uint32_t>(gameQuitConfiguration.enabled));
    if (list) {
        list->field3A8 = UIWidget_Bind(gameQuitConfiguration.enabled, 1, 2, {0.0f, 0.0f}, {-68.0f, 0.0f});
        list->field3D4 = UIWidget_Bind(gameQuitConfiguration.enabled, 2, 2, {0.0f, 0.0f}, {1.0f, 0.0f});
    }
    list = UI_GetScrollList(static_cast<std::uint32_t>(gameQuitConfiguration.enabled));
    list->rows = 1;
    list->field6C = 1;
    list->texts[0] = GetStringForUILabel("UIQUITLABL");
}

std::int64_t GetGameQuitInitState() {
    return static_cast<std::uint32_t>(gameQuitState);
}

std::int64_t GameQuitInput() {
    if (static_cast<std::uint32_t>(gameQuitState - 2) > 1U)
        return static_cast<std::uint32_t>(gameQuitConfiguration.status);
    if (gameQuitPromptPending) {
        if (static_cast<std::uint32_t>(ExecuteElementCallback(UIElement::YesNoPrompt)) - 1U <= 3U) {
            DestroyElementByIndex(UIElement::YesNoPrompt);
            gameQuitPromptPending = 0;
        }
        return static_cast<std::uint32_t>(gameQuitConfiguration.status);
    }
    UI_UpdateScrollListSelection(static_cast<std::uint32_t>(gameQuitConfiguration.enabled));
    if (g_inputBuffer[0].pressedPriority & 0x400U) {
        const UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(gameQuitConfiguration.enabled));
        if (list && !list->field74[1]) {
            const char *const no = GetStringForUILabel("UIGNRLNO00");
            const char *const yes = GetStringForUILabel("UIGNRLYES0");
            MessageBoxTimed(GetStringForUILabel("UIQUITCONF"), yes, no, 0, 0.0f);
            SetResponseCallbackForWidget(PromptResponse::Yes, ConfirmGameQuit);
            ShowElementByIndex(UIElement::YesNoPrompt);
            gameQuitPromptPending = 1;
        }
    }
    return static_cast<std::uint32_t>(gameQuitConfiguration.status);
}

std::uint32_t LoadGameQuit() {
    if (gameQuitState == 0)
        gameQuitState = LoadUIFile("/ui/gamesubmenus/gamequit/GameQuit.ui", 48, &gameQuitRequest);
    else if (gameQuitState == 1) {
        gameQuitState = GetNewUIFileInitFor(&gameQuitRequest, gameQuitConfiguration);
        if (gameQuitState == 2)
            BindGameQuit();
    } else if (gameQuitState == 2)
        gameQuitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(gameQuitConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(gameQuitState);
}

void RegisterLegacyGameScreens() {
    UIElementState &collection = g_uiElements[static_cast<std::size_t>(UIElement::GameCollection)];
    collection.name = "GAME_COLLECTION";
    collection.parseDefinitionFromFile = LoadGameCollection;
    collection.unknown28 = GameCollectionInput;
    collection.postInit = RenderGameCollection;
    collection.initialize = OpenGameCollection;
    collection.unknown60 = CloseGameCollection;
    collection.unknown70 = ReloadGameCollection;
    collection.unknown78 = GetGameCollectionInitState;
    UIElementState &watch = g_uiElements[static_cast<std::size_t>(UIElement::GameWatch)];
    watch.name = "GAME_WATCH";
    watch.parseDefinitionFromFile = LoadGameWatch;
    watch.unknown28 = GameWatchInputNullsub;
    watch.postInit = RenderGameWatch;
    watch.initialize = OpenGameWatch;
    watch.unknown60 = CloseGameWatch;
    watch.unknown78 = GetGameWatchInitState;
    UIElementState &quit = g_uiElements[static_cast<std::size_t>(UIElement::GameQuit)];
    quit.name = "GAME_QUIT";
    quit.parseDefinitionFromFile = LoadGameQuit;
    quit.unknown28 = GameQuitInput;
    quit.postInit = RenderGameQuit;
    quit.initialize = OpenGameQuit;
    quit.unknown60 = CloseGameQuit;
    quit.unknown70 = ReloadGameQuit;
    quit.unknown78 = GetGameQuitInitState;
    UIElementState &maps = g_uiElements[static_cast<std::size_t>(UIElement::GameCollectionMaps)];
    maps.name = "GAME_COLLECTION_MAPS";
    maps.parseDefinitionFromFile = LoadCollectionMaps;
    maps.postInit = RenderCollectionMaps;
    maps.unknown78 = GetCollectionMapsInitState;
    UIElementState &pawnshop = g_uiElements[static_cast<std::size_t>(UIElement::CollectionPawnshop)];
    pawnshop.name = "GAME_COLLECTION_PAWNSHOP";
    pawnshop.parseDefinitionFromFile = LoadCollectionPawnshop;
    pawnshop.postInit = RenderCollectionPawnshop;
    pawnshop.unknown78 = GetCollectionPawnshopInitState;
}
} // namespace shenmue::s2
