#include "s2/ui_legacy_menus.hpp"
#include "s2/ui_internal.hpp"
#include "s2/frame_pacing.hpp"
#include <cstdio>

namespace shenmue::s2 {

std::int32_t legacyGameHudInitState{};
std::uint32_t legacyGameHudRequest{};
UIViewConfiguration legacyGameHudConfiguration{-1, -1, -1};
std::array<std::int32_t, 2> legacyGameHudPanels{};
std::array<std::array<std::int32_t, 4>, 5> legacyGameHudButtonImages{};
std::array<UIButtonIconWidget *, 5> legacyGameHudIcons{};
std::int32_t legacyGameHudBackgroundPanel{}, legacyGameHudBackgroundBinding{};
d3t::UIWindow *legacyGameHudBackgroundWindow{};

std::int32_t gameHelpLoadState{};

std::array<std::int32_t, 2> infoBarPanels{}, infoBarBindings{};
float infoBarRemaining{};
std::int32_t infoBarPanelShown{};

// @verified - p
std::int64_t LegacyUICallbackReturnZero() {
    return 0;
}

// @verified - p
std::int64_t LegacyUICallbackReturnOne() {
    return 1;
}

// @verified - p
void LegacyUICallbackNullsub() {
}

// @verified - p
void RefreshNotebookAndUpdateView(std::uint32_t slot) {
    RefreshNotebook();
    uiUpdateWithColour(*GetNativeUiContext(), slot);
}

std::int32_t FindViewButtonIconIndex(std::uint32_t slot, const char *id, std::int32_t kind) {
    const UIButtonIconWidget *const widget = UI_FindViewIconWidget(slot, id, kind);
    return widget ? static_cast<std::int32_t>(widget - g_uiButtonIconWidgets.data()) : -1;
}

// @verified - p
void BindLegacyGameHud() {
    const std::uint32_t slot = static_cast<std::uint32_t>(legacyGameHudConfiguration.uiHandleIndex);
    GetNuklearElementByIdx(slot)->keepLoaded = 1;
    for (std::size_t panel = 0; panel < legacyGameHudPanels.size(); ++panel)
        legacyGameHudPanels[panel] = UI_FindViewWindow(slot, UI_LEGACY_HUD_PANELS[panel]);
    std::array<char, 256> name{};
    for (std::size_t button = 0; button < UI_LEGACY_HUD_BUTTONS.size(); ++button) {
        for (std::size_t part = 0; part < UI_LEGACY_HUD_BUTTON_PARTS.size(); ++part) {
            std::snprintf(name.data(), name.size(), "Image_%s_%s", UI_LEGACY_HUD_BUTTONS[button], UI_LEGACY_HUD_BUTTON_PARTS[part]);
            legacyGameHudButtonImages[button][part] = part == 3 ? FindViewButtonIconIndex(slot, name.data(), 20) : UI_FindViewImageWidget(slot, name.data());
        }
    }
    for (std::size_t icon = 0; icon < UI_LEGACY_HUD_ICONS.size(); ++icon)
        legacyGameHudIcons[icon] = UI_FindViewIconWidget(slot, UI_LEGACY_HUD_ICONS[icon], 21);
    legacyGameHudBackgroundPanel = UI_FindViewWindow(slot, "GameHUDLegacy_HUD_Background_Panel");
    legacyGameHudBackgroundWindow = UI_GetWindow(legacyGameHudBackgroundPanel);
    legacyGameHudBackgroundBinding = UIWidget_Bind(legacyGameHudBackgroundPanel, 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
}

// @verified - p
std::uint32_t LoadLegacyGameHud() {
    if (legacyGameHudInitState == 0) {
        legacyGameHudInitState = LoadUIFile("/ui/gamehud/GameHud_Legacy.ui", static_cast<std::int32_t>(UIElement::GameHud), &legacyGameHudRequest);
    } else if (legacyGameHudInitState == 1) {
        legacyGameHudInitState = GetNewUIFileInitFor(&legacyGameHudRequest, legacyGameHudConfiguration);
        if (legacyGameHudInitState == 2)
            BindLegacyGameHud();
    } else if (legacyGameHudInitState == 2) {
        legacyGameHudInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(legacyGameHudConfiguration.uiHandleIndex));
    }
    return static_cast<std::uint32_t>(legacyGameHudInitState);
}

// @verified - p
std::int64_t ReloadLegacyGameHud() {
    if (legacyGameHudConfiguration.uiHandleIndex != -1)
        GetNuklearElementByIdx(static_cast<std::uint32_t>(legacyGameHudConfiguration.uiHandleIndex))->keepLoaded = 0;
    ResetUIViewSlot(legacyGameHudConfiguration.uiHandleIndex);
    legacyGameHudInitState = 0;
    legacyGameHudConfiguration.uiHandleIndex = -1;
    legacyGameHudRequest = 0;
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), legacyGameHudConfiguration.uiHandleIndex);
    legacyGameHudInitState = 2;
    return 2;
}

// @verified - p
std::int64_t GetLegacyGameHudInitState() {
    return static_cast<std::uint32_t>(legacyGameHudInitState);
}

// @verified - p
std::int64_t GetGameQteInitState() {
    return static_cast<std::uint32_t>(g_uiElements[static_cast<std::size_t>(UIElement::GameQte)].config.init);
}

// @verified - p
std::int64_t GetGameHelpLoadState() {
    return static_cast<std::uint32_t>(gameHelpLoadState);
}

// @verified - p
std::int64_t HideInfoBar() {
    UIWidget_Animate(infoBarBindings[0], 0, 0.3f, 0.0f);
    return 1;
}

// @verified - p
std::int64_t RenderInfoBar() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::InfoBar)];
    if (!GetNuklearElementByIdx(static_cast<std::uint32_t>(element.config.uiHandleIndex)))
        return 0;
    if (infoBarRemaining > 0.0f) {
        infoBarRemaining = infoBarRemaining - GetFrameTime();
        if (infoBarRemaining <= 0.0f)
            DestroyElementByIndex(static_cast<UIElement>(element.index));
    }
    d3t::UIWindow *const window = UI_GetWindow(infoBarPanels[1]);
    if (window)
        window->alpha = infoBarPanelShown ? 255.0f : 0.0f;
    uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(element.config.uiHandleIndex));
    return 0;
}

// @verified - p
std::int64_t IsInfoBarAnimating() {
    const std::uint32_t binding = static_cast<std::uint32_t>(infoBarBindings[0]);
    return binding < g_uiWidgetBindings.size() ? g_uiWidgetBindings[binding].animating : 0;
}

// @verified - p
std::uint32_t LoadInfoBar() {
    return ReadUIDefinitionFile(UIElement::InfoBar, "/ui/infobar/InfoBar.ui", 1);
}

// @verified - p
std::int64_t BindInfoBar() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[static_cast<std::size_t>(UIElement::InfoBar)].config.uiHandleIndex);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    if (!view)
        return 1;
    view->keepLoaded = 1;
    for (std::size_t panel = 0; panel < infoBarPanels.size(); ++panel) {
        infoBarPanels[panel] = UI_FindViewWindow(slot, UI_INFO_BAR_PANELS[panel]);
        infoBarBindings[panel] = UIWidget_Bind(infoBarPanels[panel], 0, 0, {0.0f, 0.0f}, {1.0f, 1.0f});
    }
    return 1;
}

// @verified - p
std::int64_t ShowInfoBar() {
    UIWidget_Animate(infoBarBindings[0], 1, 0.3f, 0.0f);
    return 0;
}

} // namespace shenmue::s2
