// Included by ui_frontend.cpp inside ui_runtime_detail.
template <mods::Id id>
UIOption ModOption() {
    const auto &spec = mods::Spec(id);
    UIOption option{};
    option.type = spec.kind == mods::Kind::Toggle ? UIOptionType::Toggle : spec.kind == mods::Kind::List ? UIOptionType::List : UIOptionType::Slider;
    option.name = spec.label;
    option.text = [](std::int32_t) { return mods::Spec(id).description; };
    option.choice = [](std::int32_t index) { return mods::ChoiceLabel(id, index); };
    option.choiceValue = [](std::int32_t index) { return mods::ChoiceValue(id, index); };
    option.localizeChoices = spec.kind == mods::Kind::List ? 0 : 1;
    if (spec.kind == mods::Kind::Slider) {
        option.getFloat = [] { const auto &s = mods::Spec(id); return (mods::Get(id) - s.minimum) / (s.maximum - s.minimum); };
        option.setFloat = [](float value) { const auto &s = mods::Spec(id); mods::Set(id, value * (s.maximum - s.minimum) + s.minimum); };
    } else {
        option.getInt = [] { return static_cast<std::int32_t>(mods::Get(id)); };
        option.setInt = [](std::int32_t value) { mods::Set(id, static_cast<float>(value)); };
    }
    return option;
}
UIOption g_modOptions[]{
#define SHEN_MOD(id, ...) ModOption<mods::Id::id>(),
#include "enhancements/misc_mods.def"
#undef SHEN_MOD
    {.type = UIOptionType::Heading, .name = "No enhancements compiled", .available = [] { return mods::Options().empty() ? 1 : 0; }}
};
bool ModsInGame() { return g_isInGame != 0; }
bool ModsInputBlocked() { return g_uiElements[kUiSubMenuOptions].isHandledExternally != 0; }
nk_context &ModsUiContext() { return GNuklearWindow; }
std::int32_t ModsSelectedTab() { return g_optionsTabList.selected; }
std::uint32_t ModsTabButtonColour(bool selected) { return selected ? g_optionsTabButtonSelectedColour : g_optionsTabButtonColour; }
std::uint32_t ModsTabBackgroundColour(bool selected) { return selected ? g_optionsTabBackgroundSelectedColour : g_optionsTabBackgroundColour; }
void BindModsTabSelector(d3t::UIWindow **windows, d3t::UILabelWidget **labels, std::int32_t last) {
    g_optionsTabList.windows = windows;
    g_optionsTabList.labels = labels;
    g_optionsTabList.last = last;
}
#include "enhancements/mods_tab.inl"

bool IsModOption(const UIOption &option) {
    for (const UIOption &mod : g_modOptions)
        if (&mod == &option)
            return true;
    return false;
}
void ResetMods(UIRetry70ManBattlePromptResult result) {
    if (result != UIRetry70ManBattlePromptResult::Yes)
        return;
    mods::Defaults();
    ReadSettingsFile();
    Options_ReadSettings(g_modOptions, static_cast<std::int32_t>(std::size(g_modOptions)));
    Options_RedescribeOption();
}

void HandleOptionsTabMouse() {
    if (g_uiElements[kUiSubMenuOptions].isHandledExternally)
        return;
    const auto &input = GNuklearWindow.input;
    const bool clicked = nk_input_is_mouse_pressed(&input, NK_BUTTON_LEFT) != 0;
    if (!clicked && input.mouse.pos.x == input.mouse.prev.x && input.mouse.pos.y == input.mouse.prev.y)
        return;
    UIFocusList &list = g_optionsTabList;
    const auto slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptions].config.uiHandleIndex);
    for (std::int32_t tab = list.first; tab <= list.last; ++tab) {
        d3t::UIWindow *window = &g_modTabWindow;
        if (tab != ModsTabIndex()) {
            char id[64]{};
            std::snprintf(id, sizeof(id), "SubMenu_Options::Tab%d", tab == 4 ? 4 : tab + 1);
            window = UI_GetWindow(UI_FindViewWindow(slot, id));
        }
        if (!window)
            continue;
        UILayoutRect rect{};
        UI_GetWindowRect(window, rect);
        if (!nk_input_is_mouse_hovering_rect(&input, nk_rect(rect.x, rect.y, rect.width, rect.height)))
            continue;
        if (list.focused != tab)
            PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        list.focused = tab;
        list.lastFocused = -1;
        if (clicked) {
            list.selected = tab;
            list.hasFocus = 0;
            g_uiGridInputEnabled = 1;
            UI_RestartCurrentGridSelection();
            PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
        }
        UIFocusList_ColourLabels(list);
        return;
    }
    list.focused = -1;
    UIFocusList_ColourLabels(list);
}
