#include "s2/ui_moves_legacy.hpp"
#include "s2/ui_internal.hpp"
#include "s2/ui_scroll_list.hpp"
#include "s2/ui_scene.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/item_wf.hpp"
#include "s2/moves_scroll.hpp"
#include <cstdio>

namespace shenmue::s2 {
UIViewConfiguration legacyMovesConfiguration{-1, -1, -1};
std::int32_t legacyMovesScrollList = -1, legacyMovesKind28Widget = -1, legacyMovesButtonPanelWindowIndex = -1;
std::array<std::int32_t, 26> legacyMovesScrollSlots{};
std::int32_t legacyMovesInitState{}, legacyMovesTab{}, legacyMovesSkipRender{};
std::uint32_t legacyMovesRequest{};
std::array<d3t::UILayoutItem *, 3> legacyMovesStatLayouts{};
d3t::UIWindow *legacyMovesButtonPanelWindow{};
std::array<d3t::UIWindow *, 5> legacyMovesWindows{};
std::int32_t legacyMovesPendingShow{}, legacyMovesHidden{}, legacyMovesSceneLoaded{}, legacyMovesSceneWidth{}, legacyMovesSceneHeight{};
UIButtonPanel *legacyMovesButtonPanel{};
std::int32_t legacyMovesInspectFromItem{};

std::int64_t ResetLegacyMovesView() {
    ResetUIViewSlot(legacyMovesConfiguration.uiHandleIndex);
    legacyMovesConfiguration.uiHandleIndex = -1;
    legacyMovesInitState = 0;
    legacyMovesRequest = 0;
    legacyMovesStatLayouts.fill(nullptr);
    legacyMovesConfiguration.status = -1;
    legacyMovesScrollList = -1;
    legacyMovesKind28Widget = -1;
    legacyMovesButtonPanelWindowIndex = -1;
    return 0;
}

std::int64_t HideLegacyMoves() {
    if (static_cast<std::uint32_t>(legacyMovesInitState - 2) <= 1U) {
        legacyMovesHidden = 1;
        if (legacyMovesSceneLoaded == 1) {
            ReleaseWazamakiPreviewModels();
            legacyMovesSceneLoaded = 0;
            SetUISceneCallback(nullptr);
        }
        UIWidget_Animate(legacyMovesConfiguration.enabled, 0, 0.30000001f, 0.0f);
    }
    legacyMovesPendingShow = 0;
    return gameMovesItemActionState != 1 ? 1 : 0;
}

void FillScrollListScrollData(std::int32_t row) {
    const UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    const std::int32_t index = row + list->field74[0];
    if (index >= list->field6C)
        return;
    ScrollListScrollData &data = g_scrollListScrollData[row];
    const std::int32_t slot = legacyMovesScrollSlots[index];
    if (slot <= -1) {
        data.text = GetStringForUILabel("UIGNRLQSTN"); // @text EN: "? ? ?" | JA: "？？？"
        data.frame = 8;
        return;
    }
    data.text = GetStringForUILabel(shenGetAmtOf(GetWfScrollItem(slot)));
    if (data.text)
        data.frame = slot == 3010 ? 10 : 9;
}

void ShowLegacyMoves() {
    if (static_cast<std::uint32_t>(legacyMovesInitState - 2) > 1U) {
        legacyMovesPendingShow = 1;
        return;
    }
    UpdateLegacyMovesButtonPanel();
    legacyMovesHidden = 0;
    if (!legacyMovesSceneLoaded) {
        d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex));
        const std::int32_t image = UI_FindViewImageWidget(static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex), "RyoAttackID");
        const d3t::UIImageWidget *const widget = static_cast<std::uint32_t>(image) < g_uiImageWidgets.size() ? &g_uiImageWidgets[image] : nullptr;
        if (!legacyMovesSceneLoaded && view && widget && !g_debugImGuiRenderMode) {
            BindUISceneImage(0, &view->images[widget->image]);
            LoadRyoSceneForMovesScreen();
            legacyMovesSceneLoaded = 1;
            SetUISceneCallback(LegacyMovesSceneCallback);
        }
    }
    UIWidget_Animate(legacyMovesConfiguration.enabled, 1, 0.30000001f, 0.0f);
    RefreshLegacyMovesMoveRows();
    RefreshWazamakiLearnedGroups();
}

std::int64_t RenderLegacyMoves() {
    if (legacyMovesInitState == 3 || (AdvanceLegacyMovesInit(), static_cast<std::uint32_t>(legacyMovesInitState - 2) <= 1U)) {
        if (legacyMovesSkipRender == 1)
            legacyMovesSkipRender = 0;
        else {
            SyncLegacyMovesPanelAlpha();
            uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex));
        }
    }
    return 0;
}

bool CanLearnLegacyMovesScroll(std::int32_t scroll) {
    const std::uint32_t bit = 1U << (scroll & 31);
    const std::uint32_t owned = GetWfOwnedScrolls();
    const std::uint32_t learned = GetWfLearnedScrolls();
    return (owned & bit) != 0 && scroll != 24 && (learned & bit) == 0;
}

std::int64_t ReloadLegacyMoves() {
    ResetLegacyMovesView();
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), legacyMovesConfiguration.uiHandleIndex);
    legacyMovesInitState = 2;
    return 2;
}

void UpdateLegacyMovesStats(std::int32_t row) {
    std::int32_t category;
    const NativeWazamakiGroup *const group = FindWazamakiGroup(row, &category);
    const std::array<std::uint8_t, 3> &stats = GetWazamakiMoveCategory(static_cast<std::uint8_t>(category))[group->moves[group->selected]].stats;
    if (legacyMovesStatLayouts[0])
        legacyMovesStatLayouts[0]->width = static_cast<float>(stats[0]) / 90.0f * 0.083333336f;
    if (legacyMovesStatLayouts[1])
        legacyMovesStatLayouts[1]->width = (1.0f - static_cast<float>(stats[1]) / 28.0f) * 0.083333336f;
    if (legacyMovesStatLayouts[2])
        legacyMovesStatLayouts[2]->width = static_cast<float>(stats[2]) / 40.0f * 0.083333336f;
}

void SyncLegacyMovesPanelAlpha() {
    if (!legacyMovesButtonPanelWindow)
        return;
    for (d3t::UIWindow *const window : legacyMovesWindows)
        if (window)
            window->alpha = legacyMovesButtonPanelWindow->alpha;
}

void LegacyMovesSceneCallback() {
    if (!g_debugImGuiRenderMode) {
        UpdateWazamakiPreviewFrame();
        DrawWazamakiPreview();
    }
}

std::int64_t GetLegacyMovesInitState() {
    return static_cast<std::uint32_t>(legacyMovesInitState);
}

void BindLegacyMovesSceneImage() {
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex));
    const std::int32_t image = UI_FindViewImageWidget(static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex), "RyoAttackID");
    const d3t::UIImageWidget *const widget = static_cast<std::uint32_t>(image) < g_uiImageWidgets.size() ? &g_uiImageWidgets[image] : nullptr;
    if (!view || !widget)
        return;
    legacyMovesSceneWidth = view->images[widget->image].width;
    legacyMovesSceneHeight = view->images[widget->image].height;
    BindUISceneImage(0, &view->images[widget->image]);
}

void BindLegacyMoves() {
    legacyMovesScrollList = -1;
    legacyMovesKind28Widget = -1;
    legacyMovesConfiguration.status = -1;
    legacyMovesButtonPanelWindowIndex = -1;
    const std::uint32_t viewIndex = static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(viewIndex);
    view->keepLoaded = 1;
    if (view->widgetCount > 0) {
        std::int32_t scrollList = legacyMovesScrollList, kind28 = legacyMovesKind28Widget, menuBar = legacyMovesConfiguration.status;
        for (std::int32_t index = 0; index < view->widgetCount; ++index) {
            const std::int32_t kind = view->widgetKinds[index];
            if (kind == 27)
                scrollList = view->widgetPoolIndices[index];
            if (kind == 28)
                kind28 = view->widgetPoolIndices[index];
            if (kind == 24)
                menuBar = view->widgetPoolIndices[index];
            if (scrollList != -1 && kind28 != -1 && menuBar != -1)
                break;
        }
        legacyMovesConfiguration.status = menuBar;
        legacyMovesScrollList = scrollList;
        legacyMovesKind28Widget = kind28;
    }
    for (std::size_t index = 0; index < legacyMovesStatLayouts.size(); ++index)
        legacyMovesStatLayouts[index] = UI_FindViewLayoutItem(viewIndex, LEGACY_MOVES_STAT_LAYOUTS[index]);
    RefreshWazamakiLearnedGroups();
    RefreshLegacyMovesMoveRows();
    legacyMovesScrollSlots.fill(-1);
    BuildOwnedWfScrollSlots(legacyMovesScrollSlots);
    legacyMovesButtonPanelWindowIndex = UI_FindViewWindow(viewIndex, "GameMoves_Button_Panel");
    legacyMovesButtonPanelWindow = UI_GetWindow(legacyMovesButtonPanelWindowIndex);
    legacyMovesWindows[0] = UI_FindViewWindowPointer(viewIndex, "GameMoves_Ryo_Panel");
    legacyMovesWindows[1] = UI_FindViewWindowPointer(viewIndex, "GameMoves_Button_Panel");
    legacyMovesWindows[2] = UI_FindViewWindowPointer(viewIndex, "GameMoves_Menu_Bar_Panel");
    if (const UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(legacyMovesConfiguration.status)))
        legacyMovesWindows[3] = UI_GetWindow(menuBar->window);
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    if (list)
        legacyMovesWindows[4] = UI_GetWindow(list->window);
    legacyMovesConfiguration.enabled = UIWidget_Bind(legacyMovesButtonPanelWindowIndex, 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
    if (list) {
        list->field3A8 = UIWidget_Bind(legacyMovesScrollList, 1, 2, {0.0f, 0.0f}, {-68.0f, 0.0f});
        list->field3D4 = UIWidget_Bind(legacyMovesScrollList, 2, 2, {0.0f, 0.0f}, {1.0f, 0.0f});
    }
    if (legacyMovesPendingShow)
        ShowLegacyMoves();
    legacyMovesPendingShow = 0;
    legacyMovesButtonPanel = UI_FindViewButtonPanel(viewIndex, "GameMoves_Button_Panel");
    UpdateLegacyMovesButtonPanel();
}

void UpdateLegacyMovesButtonPanel() {
    if (!legacyMovesButtonPanel) {
        legacyMovesButtonPanel = UI_FindViewButtonPanel(static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex), "GameMoves_Button_Panel");
        if (!legacyMovesButtonPanel)
            return;
    }
    UIButtonPanelEntry &scroll = legacyMovesButtonPanel->entries[0];
    UIButtonPanelEntry &select = legacyMovesButtonPanel->entries[1];
    scroll.field0 = 61;
    scroll.field4 = 1;
    std::snprintf(scroll.field8.data(), scroll.field8.size(), "%s", GetStringForUILabel("UIGNRSCROL"));
    if (legacyMovesTab == LegacyMovesTabScrolls) {
        select.field0 = 0;
        select.field4 = 1;
        std::snprintf(select.field8.data(), select.field8.size(), "%s", GetStringForUILabel("UIGNRLSLCT"));
    } else {
        select.field0 = -1;
        select.field4 = -1;
        select.field8[0] = '\0';
    }
}

std::int64_t LegacyMovesInput() {
    if (static_cast<std::uint32_t>(legacyMovesInitState - 2) > 1U)
        return 0;
    if (gameMovesItemActionState == 1) {
        if (legacyMovesInspectFromItem == 1 && !(GetCurrentItemSelectState() & 1U)) {
            gameMovesItemActionState = 0;
            SetUIRenderSuppression(0);
        }
        g_inputBuffer[0].pressedPriority &= ~0x0200U;
        return 0;
    }
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    if (!list)
        return 0;
    const std::int32_t previousRow = list->field74[1] + list->field74[0];
    UI_UpdateScrollListSelection(static_cast<std::uint32_t>(legacyMovesScrollList));
    if (legacyMovesTab == LegacyMovesTabMoves)
        UpdateLegacyMovesMoveTab(previousRow);
    else if (legacyMovesTab == LegacyMovesTabScrolls)
        UpdateLegacyMovesScrollTab(previousRow);
    UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(legacyMovesConfiguration.status));
    const std::int32_t previousTab = menuBar->field184;
    UI_UpdateMenuBarSelection(static_cast<std::uint32_t>(legacyMovesConfiguration.status), 16, 8);
    if (menuBar->field184 == previousTab)
        return 0;
    list->field74 = {0, 0};
    list->field3CC = 0;
    legacyMovesTab = menuBar->field184;
    if (legacyMovesTab == LegacyMovesTabMoves) {
        list->field7C = 0;
        RefreshLegacyMovesMoveRows();
    } else if (legacyMovesTab == LegacyMovesTabScrolls) {
        list->field7C = 1;
        OpenWfScrollWidget();
        RefreshLegacyMovesScrollRows();
    }
    UpdateLegacyMovesButtonPanel();
    return 0;
}

std::uint32_t AdvanceLegacyMovesInit() {
    if (legacyMovesInitState == 0)
        legacyMovesInitState =
            LoadUIFile("/ui/gamesubmenus/gamemoves/GameMoves_Legacy.ui", static_cast<std::int32_t>(UIElement::GameMoves), &legacyMovesRequest);
    else if (legacyMovesInitState == 1) {
        legacyMovesInitState = GetNewUIFileInitFor(&legacyMovesRequest, legacyMovesConfiguration);
        if (legacyMovesInitState == 2)
            BindLegacyMoves();
    } else if (legacyMovesInitState == 2) {
        legacyMovesInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(legacyMovesConfiguration.uiHandleIndex));
        if (legacyMovesInitState == 3)
            BindLegacyMovesSceneImage();
    }
    return static_cast<std::uint32_t>(legacyMovesInitState);
}

void UpdateLegacyMovesMoveTab(std::int32_t previousRow) {
    const UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    std::int32_t category;
    NativeWazamakiGroup *const group = FindWazamakiGroup(list->field74[1] + list->field74[0], &category);
    std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    if ((pressed & 0x4000U) && group->count > 1) {
        group->selected = static_cast<std::uint8_t>((group->count + group->selected - 1) % group->count);
        SetMoveEquipment(0, static_cast<std::uint8_t>(category), group->moves[group->selected]);
        RequestWazamakiPreviewMotion(static_cast<std::uint8_t>(category), group->moves[group->selected]);
        RefreshLegacyMovesMoveRows();
        pressed = g_inputBuffer[0].pressedPriority;
    }
    if ((pressed & 0x8000U) && group->count > 1) {
        group->selected = static_cast<std::uint8_t>((group->count + group->selected + 1) % group->count);
        SetMoveEquipment(0, static_cast<std::uint8_t>(category), group->moves[group->selected]);
        RequestWazamakiPreviewMotion(static_cast<std::uint8_t>(category), group->moves[group->selected]);
        RefreshLegacyMovesMoveRows();
    }
    if (list->field74[1] + list->field74[0] != previousRow) {
        RefreshLegacyMovesMoveRows();
        RequestWazamakiPreviewMotion(static_cast<std::uint8_t>(category), group->moves[group->selected]);
    }
}

void UpdateLegacyMovesScrollTab(std::int32_t previousRow) {
    const UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    if (!list)
        return;
    const std::int32_t row = list->field74[0] + list->field74[1];
    if (previousRow == row) {
        if (!(g_inputBuffer[0].pressedPriority & 0x0400U))
            return;
        const std::int32_t slot = legacyMovesScrollSlots[row];
        if (CanLearnLegacyMovesScroll(slot)) {
            gameMovesItemActionState = 1;
            legacyMovesInspectFromItem = 0;
            LearnWfScroll(row);
        } else if (GetWfOwnedScrolls() & (1U << (slot & 31))) {
            gameMovesItemActionState = 1;
            legacyMovesInspectFromItem = 1;
            InspectWfScroll(slot);
            SetUIRenderSuppression(1);
        }
        return;
    }
    std::int32_t scroll = legacyMovesScrollSlots[row];
    if (scroll >= 3000)
        scroll = FindWfScrollDefinition(scroll);
    if (scroll != -1) {
        const std::int32_t groupRow = GetWfScrollPreviewRow(scroll);
        UpdateLegacyMovesStats(groupRow);
        std::int32_t category;
        if (const NativeWazamakiGroup *const group = FindWazamakiGroup(groupRow, &category))
            RequestWazamakiPreviewMotion(static_cast<std::uint8_t>(category), group->moves[group->selected]);
    }
    RefreshLegacyMovesScrollRows();
}

void RefreshLegacyMovesMoveRows() {
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    UpdateLegacyMovesStats(list->field74[0] + list->field74[1]);
    const std::int32_t total = GetWazamakiGroupCount(0);
    list->field6C = total;
    const std::int32_t count = total < list->rows ? total : list->rows;
    std::int32_t row = 0;
    for (; row < count; ++row) {
        FillScrollListMoveData(row + list->field74[0], g_scrollListMoveData[row]);
        list->words[row] = row;
    }
    FillScrollListMoveData(row + list->field74[0], g_scrollListMoveData[row]);
    list->words[row] = row;
    if (list->field74[0] > 0) {
        ++row;
        FillScrollListMoveData(list->field74[0] - 1, g_scrollListMoveData[row]);
        list->words[row] = row;
    }
}

void RefreshLegacyMovesScrollRows() {
    legacyMovesScrollSlots.fill(-1);
    BuildOwnedWfScrollSlots(legacyMovesScrollSlots);
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(legacyMovesScrollList));
    list->field6C = 26;
    const std::int32_t count = list->rows > 26 ? 26 : list->rows;
    std::int32_t row = 0;
    for (; row < count; ++row) {
        FillScrollListScrollData(row);
        list->words[row] = row;
    }
    FillScrollListScrollData(row);
    list->words[row] = row;
    if (list->field74[0] > 0) {
        ++row;
        FillScrollListScrollData(row);
        list->words[row] = row;
    }
}
} // namespace shenmue::s2
