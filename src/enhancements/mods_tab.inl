// Included inside the game's Options implementation.
d3t::UIWindow g_modTabWindow{};
d3t::UILabelWidget g_modTabLabel{};
std::array<d3t::UIWindow *, 5> g_modTabWindows{};
std::array<d3t::UILabelWidget *, 5> g_modTabLabels{};
d3t::UIWindowLayout g_optionsTabOriginalLayout{};

void SaveOptionsTabLayout() {
    const auto slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptions].config.uiHandleIndex);
    if (auto *const first = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab1")))
        g_optionsTabOriginalLayout = first->layout;
}

constexpr std::int32_t ModsTabIndex() { return 3; }
bool ModsTabSelected() { return ModsSelectedTab() == ModsTabIndex(); }

void ConfigureModsTabs() {
    const auto slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptions].config.uiHandleIndex);
    auto *const first = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab1"));
    auto *const second = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab2"));
    auto *const column = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Left_Panel"));
    if (first) {
        first->layout = g_optionsTabOriginalLayout;
        UI_InvalidateWindowRect(*first);
        if (ModsInGame() && second && column) {
            UILayoutRect top{}, next{}, bounds{};
            UI_GetWindowRect(first, top);
            UI_GetWindowRect(second, next);
            UI_GetWindowRect(column, bounds);
            const float gap = (std::max)(0.0f, next.y - top.y - top.height);
            const float margin = (std::max)(0.0f, top.y - bounds.y);
            const float rowHeight = (bounds.height - margin * 2.0f - gap * 4.0f) / 5.0f;
            if (rowHeight > 0.0f && top.height > rowHeight) {
                const float scale = rowHeight / top.height;
                first->layout.width.scale *= scale;
                first->layout.height.scale *= scale;
                first->layout.alignX = 1;
                UI_InvalidateWindowRect(*first);
            }
        }
    }
    for (std::size_t tab = 0; tab < 4; ++tab) {
        g_modTabWindows[tab] = g_optionsWindows[tab];
        g_modTabLabels[tab] = g_optionsLabels[9 + tab];
        if (g_optionsWindows[tab])
            g_optionsWindows[tab]->mouseCallbacks = {};
    }
    g_modTabWindows[4] = g_optionsWindows[3];
    g_modTabLabels[4] = g_optionsLabels[12];
    g_modTabLabel = g_optionsLabels[9] ? *g_optionsLabels[9] : d3t::UILabelWidget{};
    g_modTabLabel.text = "Mods";
    g_modTabLabel.token.token = nullptr;
    const std::size_t tab = static_cast<std::size_t>(ModsTabIndex());
    g_modTabWindows[tab] = &g_modTabWindow;
    g_modTabLabels[tab] = &g_modTabLabel;
    BindModsTabSelector(g_modTabWindows.data(), g_modTabLabels.data(), ModsInGame() ? 4 : 3);
}

void PositionExitTab() {
    if (!ModsInGame())
        return;
    const auto slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptions].config.uiHandleIndex);
    auto *const first = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab1"));
    auto *const second = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab2"));
    auto *const exit = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab4"));
    if (!first || !second || !exit)
        return;
    UILayoutRect top{}, next{}, rect{};
    UI_GetWindowRect(first, top);
    UI_GetWindowRect(second, next);
    UI_InvalidateWindowRect(*exit);
    UI_GetWindowRect(exit, rect);
    rect.y += next.y - top.y;
    exit->cachedRect = {rect.x, rect.y, rect.width, rect.height};
    exit->rectCached = 1;
}

void DrawModsTab() {
    if (ModsInputBlocked() || !g_optionsWindows[0] || !g_optionsWindows[3])
        return;
    const auto slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptions].config.uiHandleIndex);
    if (slot >= g_uiViewSlots.size())
        return;
    const auto &view = g_uiViewSlots[slot];
    const std::array<const char *, 4> ids{"SubMenu_Options::Tab1", "SubMenu_Options::Tab1Button",
        "SubMenu_Options::TabIcon1", "SubMenu_Options::TabLabel1"};
    std::array<d3t::UIWindow *, 4> windows{};
    std::array<UILayoutRect, 4> sourceRects{};
    for (std::size_t index = 0; index < windows.size(); ++index) {
        windows[index] = UI_GetWindow(UI_FindViewWindow(slot, ids[index]));
        if (!windows[index])
            return;
        UI_GetWindowRect(windows[index], sourceRects[index]);
    }
    d3t::UIWindow *const second = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Options::Tab2"));
    if (!second)
        return;
    UILayoutRect next{};
    UI_GetWindowRect(second, next);
    UILayoutRect rect = sourceRects[0];
    rect.y += (next.y - sourceRects[0].y) * static_cast<float>(ModsTabIndex());
    g_modTabWindow.cachedRect = {rect.x, rect.y, rect.width, rect.height};
    g_modTabWindow.initialLayout = g_modTabWindow.layout;
    g_modTabWindow.rectCached = 1;

    std::array<std::array<std::int32_t, 2>, 4> ranges{};
    for (std::size_t panel = 0; panel < windows.size(); ++panel) {
        ranges[panel] = {-1, -1};
        for (std::int32_t widget = 0; widget < view.widgetCount; ++widget) {
            const auto index = static_cast<std::size_t>(widget);
            if (view.widgetKinds[index] != 1 || UI_GetWindow(view.widgetPoolIndices[index]) != windows[panel])
                continue;
            ranges[panel][0] = widget && view.widgetKinds[index - 1] == 6 ? widget - 1 : widget;
            for (++widget; widget < view.widgetCount; ++widget) {
                if (view.widgetKinds[static_cast<std::size_t>(widget)] == 2) {
                    ranges[panel][1] = widget + 1;
                    break;
                }
            }
            break;
        }
        if (ranges[panel][0] == -1 || ranges[panel][1] == -1)
            return;
    }
    if (!g_optionsLabels[9])
        return;

    // Draw another instance of the native tab, preserving its icon, label stroke and layout.
    std::array<d3t::UIWindow, 4> savedWindows{};
    for (std::size_t index = 0; index < windows.size(); ++index) {
        savedWindows[index] = *windows[index];
        const auto &source = sourceRects[index];
        windows[index]->cachedRect = {source.x + rect.x - sourceRects[0].x, source.y + rect.y - sourceRects[0].y, source.width, source.height};
        windows[index]->initialLayout = windows[index]->layout;
        windows[index]->rectCached = 1;
        windows[index]->flags |= NK_WINDOW_ROM;
        windows[index]->mouseCallbacks = {};
    }
    const auto savedLabel = *g_optionsLabels[9];
    *g_optionsLabels[9] = g_modTabLabel;
    g_optionsLabels[9]->window = savedLabel.window;
    g_optionsLabels[9]->token = {};
    g_optionsLabels[9]->token.alreadyLocalized = 1;
    g_optionsLabels[9]->dataBinding = -1;
    g_optionsLabels[9]->scaleVar = -1;
    const auto savedIconColour = g_optionsColourImages[0] ? g_optionsColourImages[0]->colour : 0U;
    const auto savedButtonColour = g_optionsButtons[15] ? g_optionsButtons[15]->colour : 0U;
    auto *const background = g_optionsColourImages[static_cast<std::size_t>(g_optionsTabBackgrounds)];
    const auto savedBackgroundColour = background ? background->colour : 0U;
    if (g_optionsColourImages[0])
        g_optionsColourImages[0]->colour = (g_optionsColourImages[0]->colour & 0x00ffffffU) | ((ModsTabSelected() ? 255U : 153U) << 24);
    if (g_optionsButtons[15])
        g_optionsButtons[15]->colour = ModsTabButtonColour(ModsTabSelected());
    if (background)
        background->colour = ModsTabBackgroundColour(ModsTabSelected());
    for (const auto &range : ranges)
        UI_InputHandler(ModsUiContext(), static_cast<std::int32_t>(slot), range[0], range[1]);
    for (std::size_t index = 0; index < windows.size(); ++index) {
        const auto panel = windows[index]->panel;
        const auto drawCount = windows[index]->drawCount;
        *windows[index] = savedWindows[index];
        windows[index]->panel = panel;
        windows[index]->drawCount = drawCount;
    }
    *g_optionsLabels[9] = savedLabel;
    if (g_optionsColourImages[0])
        g_optionsColourImages[0]->colour = savedIconColour;
    if (g_optionsButtons[15])
        g_optionsButtons[15]->colour = savedButtonColour;
    if (background)
        background->colour = savedBackgroundColour;
}
