#include "s2/ui_main_collection.hpp"
#include "s2/ui_internal.hpp"
#include "s2/ui_scroll_list.hpp"
#include "s2/areas/area_omak.hpp"
#include <cstring>

namespace shenmue::s2 {

std::int32_t mainCollectionInitState{};
std::uint32_t mainCollectionRequest{};
UIViewConfiguration mainCollectionConfiguration{-1, -1, -1};
std::int32_t mainCollectionPreviewIcon{-1};
std::int32_t mainCollectionShownTab{};
std::int32_t collectionBackgroundInitState{};
std::uint32_t collectionBackgroundRequest{};
UIViewConfiguration collectionBackgroundConfiguration{-1, 0, 0};

// @verified - p
std::int64_t RenderMainCollection() {
    if (mainCollectionInitState == 3 || (LoadMainCollection(), static_cast<std::uint32_t>(mainCollectionInitState - 2) <= 1U)) {
        if (!omakeCollectionSuppressed) {
            SetUIElementIndex(static_cast<std::int32_t>(UIElement::ShenmueCollection));
            uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(mainCollectionConfiguration.uiHandleIndex));
            SetUIElementIndex(-1);
        }
    }
    return 0;
}

// @verified - p
std::int64_t MainCollectionInput() {
    if (static_cast<std::uint32_t>(mainCollectionInitState - 2) > 1U || mainCollectionConfiguration.enabled < 0 || mainCollectionConfiguration.status < 0 ||
        mainCollectionPreviewIcon < 0) {
        if (mainCollectionInitState == 3)
            BindMainCollection();
        return 0;
    }
    if (omakeCollectionSuppressed)
        return 0;
    const UIMenuBar *const menuBar = UI_GetMenuBar(static_cast<std::uint32_t>(mainCollectionConfiguration.enabled));
    UI_UpdateMenuBarSelection(static_cast<std::uint32_t>(mainCollectionConfiguration.enabled), 16, 8);
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(mainCollectionConfiguration.status));
    UI_UpdateScrollListSelection(static_cast<std::uint32_t>(mainCollectionConfiguration.status));
    std::int32_t tab = menuBar->field184;
    if (mainCollectionShownTab != tab) {
        list->field74 = {0, 0};
        UIWidget_Retarget(list->field3D4, {list->selectorX, list->selectorY}, {0.0f, 13.0f});
        UIWidget_Animate(list->field3D4, 1, 0.15f, 0.0f);
        tab = menuBar->field184;
        mainCollectionShownTab = tab;
    }
    const std::int32_t selection = list->field74[1] < 0 ? 0 : list->field74[1];
    const std::int32_t category = tab ? (tab != 2 ? 1 : 0) : 2;
    const char **const texts = list->texts.data();
    list->field6C = 0;
    for (std::int32_t row = 0; row < 13; ++row) {
        const char *const name = areas::GetOmakeEntryName(category, static_cast<std::uint32_t>(row + list->field74[0]));
        if (list->field6C > 0) {
            texts[row] = "";
        } else if (name) {
            texts[row] = name;
        } else {
            if (!list->field6C)
                list->field6C = row + list->field74[0];
            texts[row] = "";
        }
    }
    const std::uint32_t entry = static_cast<std::uint32_t>(selection + list->field74[0]);
    d3t::UIViewVariable *variable = UI_GetViewVariable(static_cast<std::uint32_t>(mainCollectionConfiguration.uiHandleIndex), 1);
    if (variable)
        variable->value.stringValue = areas::GetOmakeEntryDescription(category, entry);
    variable = UI_GetViewVariable(static_cast<std::uint32_t>(mainCollectionConfiguration.uiHandleIndex), 2);
    if (variable)
        variable->value.stringValue = areas::GetOmakeEntryDescriptionEnd(category, entry);
    UIButtonIconWidget *const preview = UI_GetButtonIconWidget(static_cast<std::uint32_t>(mainCollectionPreviewIcon));
    if (preview) {
        if (!std::strncmp(texts[selection], "UIMCOLLOCK", 10) || !std::strncmp(texts[selection], "???", 3))
            preview->sprite = MAIN_COLLECTION_LOCKED_PREVIEW;
        else
            preview->sprite = MAIN_COLLECTION_PREVIEWS.data()[12LL * menuBar->field184 + static_cast<std::uint32_t>(selection + list->field74[0])];
    }
    return 0;
}

// @verified - p
std::uint32_t LoadMainCollection() {
    if (mainCollectionInitState == 0) {
        mainCollectionInitState =
            LoadUIFile("/ui/mainsubmenus/collection/MainCollection.ui", static_cast<std::int32_t>(UIElement::ShenmueCollection), &mainCollectionRequest);
    } else if (mainCollectionInitState == 1) {
        mainCollectionInitState = GetNewUIFileInitFor(&mainCollectionRequest, mainCollectionConfiguration);
        if (mainCollectionInitState == 2)
            BindMainCollection();
    } else if (mainCollectionInitState == 2) {
        mainCollectionInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(mainCollectionConfiguration.uiHandleIndex));
    }
    return static_cast<std::uint32_t>(mainCollectionInitState);
}

// @verified - p
std::int64_t ReloadMainCollection() {
    ResetUIViewSlot(mainCollectionConfiguration.uiHandleIndex);
    mainCollectionConfiguration.uiHandleIndex = -1;
    mainCollectionInitState = 0;
    mainCollectionRequest = 0;
    mainCollectionConfiguration.enabled = -1;
    mainCollectionConfiguration.status = -1;
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), mainCollectionConfiguration.uiHandleIndex);
    mainCollectionInitState = 2;
    return 2;
}

// @verified - p
void BindMainCollection() {
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(static_cast<std::uint32_t>(mainCollectionConfiguration.uiHandleIndex));
    std::int32_t scrollList = mainCollectionConfiguration.status;
    if (view->widgetCount > 0) {
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            const std::int32_t kind = view->widgetKinds[widget];
            if (kind == 24)
                mainCollectionConfiguration.enabled = view->widgetPoolIndices[widget];
            if (kind == 27)
                scrollList = view->widgetPoolIndices[widget];
            if (kind == 20)
                mainCollectionPreviewIcon = view->widgetPoolIndices[widget];
        }
        mainCollectionConfiguration.status = scrollList;
    }
    if (scrollList != -1) {
        UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(scrollList));
        list->field3A8 = UIWidget_Bind(mainCollectionConfiguration.status, 1, 2, {0.0f, 0.0f}, {-65.0f, 0.0f});
        list->field3D4 = UIWidget_Bind(mainCollectionConfiguration.status, 2, 2, {0.0f, 0.0f}, {1.0f, 0.0f});
    }
}

// @verified - p
std::int64_t GetMainCollectionInitState() {
    return static_cast<std::uint32_t>(mainCollectionInitState);
}

// @verified - p
std::int64_t GetCollectionBackgroundInitState() {
    return static_cast<std::uint32_t>(collectionBackgroundInitState);
}

// @verified - p
std::int64_t RenderCollectionBackground() {
    if (collectionBackgroundInitState == 3 || (LoadCollectionBackground(), static_cast<std::uint32_t>(collectionBackgroundInitState - 2) <= 1U)) {
        if (RefreshNotebook())
            uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(collectionBackgroundConfiguration.uiHandleIndex));
    }
    return 0;
}

// @verified - p
std::uint32_t LoadCollectionBackground() {
    if (collectionBackgroundInitState == 0) {
        collectionBackgroundInitState = LoadUIFile("/ui/mainsubmenus/collection/MainCollectionBackground.ui",
            static_cast<std::int32_t>(UIElement::CollectionBackground), &collectionBackgroundRequest);
    } else if (collectionBackgroundInitState == 1) {
        collectionBackgroundInitState = GetNewUIFileInitFor(&collectionBackgroundRequest, collectionBackgroundConfiguration);
    } else if (collectionBackgroundInitState == 2) {
        collectionBackgroundInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(collectionBackgroundConfiguration.uiHandleIndex));
    }
    return static_cast<std::uint32_t>(collectionBackgroundInitState);
}
} // namespace shenmue::s2
