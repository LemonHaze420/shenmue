namespace ui_detail {
struct OmakeCollectionRuntime {
    CollectionAuthoredLayout layout;
    std::vector<std::uint8_t> rowPanels;
    std::array<UIGrid, 3> grids;
    std::array<std::array<OmakeCollectionRow, 12>, 3> rows;
    std::array<std::int32_t, 3> tabPanels{-1, -1, -1}, tabLabels{-1, -1, -1};
    std::int32_t holder{-1}, rowRoot{-1}, thumbnail{-1}, defaultThumbnail{-1};
    UIGridTabSelector tabs{};
    std::int32_t shown{-1}, pointerTab{-1};
    bool bound{}, returnReset{};
    d3t::UILabelDefinition rowBoldLabel, rowLabel;
};
OmakeCollectionRuntime omakeCollectionUI;
std::array<std::uint8_t, 3> mainMenuBackgroundAlpha{};
std::uint32_t mainMenuPage = UINT32_MAX;
std::int64_t BindMainMenuDefinition() {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::MainMenu)];
    mainMenuBackgroundAlpha.fill(0);
    for (d3t::UITextureObjectDefinition &object : definition.textureObjects) {
        for (std::size_t index = 0; index < mainMenuBackgroundAlpha.size(); ++index)
            if (object.id == OMAKE_MAIN_MENU_BACKGROUNDS[index])
                mainMenuBackgroundAlpha[index] = static_cast<std::uint8_t>(object.colour >> 24);
        if (object.id == "SwitchAccountButtonIcon")
            object.hidden = true;
    }
    for (d3t::UILabelDefinition &label : definition.labels) {
        if (label.id != "SwitchAccountButtonLabel" && label.id != "USER_ID")
            continue;
        label.text.clear();
        label.token.clear();
    }
    return 1;
}
const std::int32_t *OmakeCollectionSelectionData(UIGrid *grid, std::int32_t ordinal) {
    return grid->dictionary ? reinterpret_cast<const std::int32_t *>(reinterpret_cast<const OmakeCollectionRow *>(grid->dictionary) + ordinal) : nullptr;
}
void SelectOmakeCollectionRow(float *bounds, void *data) {
    if (!data)
        return;
    const OmakeCollectionRow *row = reinterpret_cast<const OmakeCollectionRow *>(data);
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ShenmueCollection)];
    UIMessageBox message{};
    message.imageIndex = -1;
    message.widgetIndex = -1;
    if (bounds)
        std::memcpy(&message.bounds, bounds, sizeof(message.bounds));
    message.hideTarget = 1;
    message.primaryText = row->description.data();
    message.primaryLocalized = 1;
    CopyUIMessageBox(&message);
    if (omakeCollectionUI.thumbnail < 0)
        return;
    std::int32_t image = omakeCollectionUI.defaultThumbnail;
    if (row->unlocked) {
        char name[64];
        std::snprintf(name, sizeof(name), "PreviewImage%02u", OMAKE_COLLECTION_PREVIEWS[row->category][row->index]);
        for (std::size_t index = 0; index < definition.images.size(); ++index)
            if (definition.images[index].name == name) {
                image = static_cast<std::int32_t>(index);
                break;
            }
    }
    d3t::UITextureObjectDefinition &thumbnail = definition.textureObjects[omakeCollectionUI.thumbnail];
    if (image >= 0)
        thumbnail.imageName = definition.images[image].name;
    (void)bounds;
}
void ActivateOmakeCollectionRow(float *, void *data) {
    if (!data)
        return;
    const OmakeCollectionRow *row = reinterpret_cast<const OmakeCollectionRow *>(data);
    if (row->category == 2 && row->index == 0) {
        ShowElementByIndex(UIElement::SubMenuMoves);
        EnableRenderableByIndex(UIElement::ShenmueCollection);
        AicaExecuteWrapper(2);
    } else {
        if (row->category == 0 && row->index == 0)
            SetHangOnCompletionEnabled(0);
        areas::SelectOmakeEntry(row->category, static_cast<std::uint32_t>(row->index));
    }
}
void DrawOmakeCollectionRow(
    UIGrid *grid, void *, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    if (!drawingUI || omakeCollectionUI.rowRoot < 0)
        return;
    OmakeCollectionRuntime &runtime = omakeCollectionUI;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ShenmueCollection)];
    const float sx = 1920.0f / GetUIViewportWidth();
    const d3t::UIRect rectangle{(outer->x + bounds->x) * sx, outer->y + bounds->y, bounds->z * sx, bounds->w};
    const float left = std::max(outer->x * sx, rectangle.x), top = std::max(outer->y, rectangle.y);
    const float right = std::min((outer->x + outer->z) * sx, rectangle.x + rectangle.width),
                bottom = std::min(outer->y + outer->w, rectangle.y + rectangle.height);
    const d3t::UIRect clip{left, top, right - left, bottom - top};
    const d3t::UIPanelBoundsOverride override{static_cast<std::size_t>(runtime.rowRoot), rectangle};
    ResolveCollectionNativeLayout(definition, runtime.layout, &override);
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        const std::int32_t panel = runtime.layout.objectPanels[index];
        if (panel < 0 || !runtime.rowPanels[panel])
            continue;
        d3t::UITextureObjectDefinition &object = definition.textureObjects[index];
        float opacity = 1.0f;
        if (!IsCameraIconHidden()) {
            if (object.id == "SMCollection::GridElementBackground_Front")
                opacity = ordinal == GetUIGridSelection(*grid) ? 0.0f : 1.0f;
            if (object.id == "SMCollection::GridElementBackground_Selected")
                opacity = ordinal == GetUIGridSelection(*grid) ? 1.0f : 0.0f;
        }
        d3t::UIResolvedTextureObject placement = runtime.layout.objects[index];
        placement.clip = clip;
        DrawNativeMenuObject(definition, object, placement, opacity);
    }
    if (!dictionary)
        return;
    const OmakeCollectionRow *row = reinterpret_cast<const OmakeCollectionRow *>(OmakeCollectionSelectionData(grid, ordinal));
    const d3t::UIResolvedLabel placement{.bounds = {rectangle.x + 50, rectangle.y, rectangle.width - 50, rectangle.height}, .clip = clip};
    DrawMovesLabel(runtime.rowBoldLabel, placement, row->name.data(), 1.0f);
    DrawMovesLabel(runtime.rowLabel, placement, row->name.data(), 1.0f);
}
void UpdateOmakeCollectionTabs() {
    OmakeCollectionRuntime &runtime = omakeCollectionUI;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ShenmueCollection)];
    if (runtime.shown != runtime.tabs.selected) {
        runtime.shown = runtime.tabs.selected;
        ResetUIGrid(runtime.grids[runtime.shown]);
        for (std::int32_t tab = 0; tab < 3; ++tab) {
            char id[64];
            std::snprintf(id, sizeof(id), "SMCollection::Image_Tab_%d", tab + 1);
            for (d3t::UITextureObjectDefinition &object : definition.textureObjects)
                if (object.id == id)
                    object.colour = (object.colour & 0xffffffU) | ((tab == runtime.tabs.selected ? 255U : 153U) << 24);
            std::snprintf(id, sizeof(id), "SMCollection::Button_Tab_%d", tab + 1);
            for (d3t::UITextureObjectDefinition &object : definition.textureObjects)
                if (object.id == id)
                    object.colour = tab == runtime.tabs.selected ? 0xc1163f4cU : 0x7f34595eU;
            std::snprintf(id, sizeof(id), "SMCollection::Button_TextureFront_%d", tab + 1);
            for (d3t::UITextureObjectDefinition &object : definition.textureObjects)
                if (object.id == id)
                    object.colour = tab == runtime.tabs.selected ? 0x1ea5e6ffU : 0x00ffffffU;
        }
    }
    if (!runtime.tabs.coloursInitialized || runtime.tabs.previousActive != runtime.tabs.active ||
        (runtime.tabs.previousHover != runtime.tabs.hovered && runtime.tabs.hovered != -1) || runtime.tabs.previousSelected != runtime.tabs.selected) {
        for (std::int32_t tab = 0; tab < 3; ++tab) {
            if (runtime.tabLabels[tab] < 0)
                continue;
            d3t::UILabelDefinition &label = definition.labels[runtime.tabLabels[tab]];
            const std::uint32_t tint = tab == runtime.tabs.hovered ? (IsCameraIconHidden() == 1 ? 0xffffffffU : 0xff258fe6U) :
                tab == runtime.tabs.selected                       ? 0xffffffffU :
                                                                     0x40ffffffU;
            label.colour = tint;
        }
        runtime.tabs.coloursInitialized = true;
    }
    runtime.tabs.previousActive = runtime.tabs.active;
    runtime.tabs.previousSelected = runtime.tabs.selected;
    if (runtime.tabs.hovered >= 0)
        runtime.tabs.previousHover = runtime.tabs.hovered;
}
}
std::int64_t OmakeCollectionHints() {
    return CopyHudHintIds(OMAKE_COLLECTION_HINTS);
}
std::int32_t omakeCollectionSuppressed;
void SetOmakeCollectionSuppressed(std::int32_t suppressed) {
    omakeCollectionSuppressed = suppressed;
}
std::int64_t BindOmakeCollectionUI() {
    OmakeCollectionRuntime &runtime = omakeCollectionUI;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ShenmueCollection)];
    BindCollectionLayout(definition, runtime.layout);
    runtime.rowRoot = MenuPanelIndex(definition, "SMCollection::GridElementParent_#");
    runtime.holder = MenuPanelIndex(definition, "SMCollection::GridHolder");
    runtime.rowPanels.resize(definition.panels.size());
    for (std::size_t index = 0; index < definition.panels.size(); ++index) {
        runtime.rowPanels[index] = 0;
        for (std::int32_t ancestor = static_cast<std::int32_t>(index); ancestor >= 0; ancestor = runtime.layout.parents[ancestor])
            if (ancestor == runtime.rowRoot) {
                runtime.rowPanels[index] = 1;
                break;
            }
    }
    for (std::int32_t tab = 0; tab < 3; ++tab) {
        char id[64];
        std::snprintf(id, sizeof(id), "SMCollection::Tab%dButton", tab + 1);
        runtime.tabPanels[tab] = MenuPanelIndex(definition, id);
        std::snprintf(id, sizeof(id), "SMCollection::Label_Tab_%d", tab + 1);
        runtime.tabLabels[tab] = -1;
        for (std::size_t index = 0; index < definition.labels.size(); ++index)
            if (definition.labels[index].id == id) {
                runtime.tabLabels[tab] = static_cast<std::int32_t>(index);
                break;
            }
        InitializeUIGrid(runtime.grids[tab],
            {.columns = 1,
                .visibleRows = 8,
                .rows = 12,
                .drawRow = DrawOmakeCollectionRow,
                .actions = {SelectOmakeCollectionRow, nullptr, nullptr, ActivateOmakeCollectionRow},
                .selectionData = OmakeCollectionSelectionData,
                .sound = 3});
    }
    runtime.tabs.first = 0;
    runtime.tabs.last = 2;
    runtime.thumbnail = -1;
    runtime.defaultThumbnail = -1;
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        d3t::UITextureObjectDefinition &object = definition.textureObjects[index];
        if (object.id == "SMCollection::Image_Thumbnail")
            runtime.thumbnail = static_cast<std::int32_t>(index);
        if (object.id == "SMCollection::MainPanelButton")
            object.colour = 0xff3f6a70U;
        if (object.id == "SMCollection::TextureFront")
            object.colour = 0xa5ffffffU;
    }
    for (std::size_t index = 0; index < definition.images.size(); ++index)
        if (definition.images[index].name == "PreviewImageLocked")
            runtime.defaultThumbnail = static_cast<std::int32_t>(index);
    runtime.rowBoldLabel.alignment = "Left";
    runtime.rowBoldLabel.colour = 0xff000000;
    runtime.rowBoldLabel.fontName = "UIStandardFontBold";
    runtime.rowLabel.alignment = "Left";
    runtime.rowLabel.colour = 0xffffffff;
    runtime.rowLabel.fontName = "UIStandardFont";
    runtime.bound = true;
    if (g_uiElements[static_cast<std::size_t>(UIElement::ShenmueCollection)].isVisible)
        TickOmakeCollectionUI();
    return 1;
}
std::int64_t TickOmakeCollectionUI() {
    OmakeCollectionRuntime &runtime = omakeCollectionUI;
    if (!runtime.bound)
        return 0;
    const std::uint32_t flags = static_cast<std::uint32_t>(GetCurrentGameFlags());
    for (std::int32_t category = 0; category < 3; ++category) {
        std::int32_t count = 0;
        for (; count < 12 && areas::OMAKE_COLLECTION_TABLES[category][count].callback; ++count) {
            const areas::OmakeEntry &entry = areas::OMAKE_COLLECTION_TABLES[category][count];
            OmakeCollectionRow &row = runtime.rows[category][count];
            row.category = category;
            row.index = count;
            row.unlocked = (flags & entry.requiredFlags) == entry.requiredFlags || areas::omakeUnlockOverride;
            const std::string_view name=GetLocalizedSceneText(row.unlocked?entry.name:"UIMCOLLOCK"); // @text EN: "???" | JA: "？？？"
            std::snprintf(row.name.data(), row.name.size(), "%.*s", static_cast<int>(name.size()), name.data());
            const std::string_view first=GetLocalizedSceneText(row.unlocked?entry.description:"UIMCOLLOCK"); // @text EN: "???" | JA: "？？？"
            const std::string_view last = row.unlocked && entry.descriptionEnd ? GetLocalizedSceneText(entry.descriptionEnd) : std::string_view{};
            const char *separator = row.unlocked && entry.description && entry.descriptionEnd ? "\xa1\xf5" : "";
            std::snprintf(row.description.data(), row.description.size(), "%.*s%s%.*s", static_cast<int>(first.size()), first.data(), separator,
                static_cast<int>(last.size()), last.data());
        }
        SetUIGridDictionary(runtime.grids[category], count, reinterpret_cast<const std::int32_t *>(runtime.rows[category].data()));
    }
    ShowElementByIndex(UIElement::GameInfoBar);
    ResetUIGridTabSelector();
    runtime.tabs.hovered = runtime.tabs.previousHover = runtime.tabs.selected = runtime.tabs.previousSelected = 0;
    runtime.tabs.active = runtime.tabs.previousActive = runtime.tabs.coloursInitialized = 0;
    SetUIGridTabSelector(&runtime.tabs, 1);
    runtime.shown = -1;
    UpdateOmakeCollectionTabs();
    runtime.returnReset = true;
    return 0;
}
std::int64_t InputOmakeCollectionUI() {
    OmakeCollectionRuntime &runtime = omakeCollectionUI;
    if (g_inputBuffer[0].pressedPriority & 0x200) {
        areas::RequestOmakeBack();
        SelectMainMenuPage(0);
    }
    if (runtime.tabs.active) {
        if (g_inputBuffer[0].repeatedPresses & 0x1000) {
            g_inputBuffer[0].repeatedPresses &= ~0x1000U;
            if (--runtime.tabs.hovered < 0)
                runtime.tabs.hovered = 2;
            AicaExecuteWrapper(3);
        }
        if (g_inputBuffer[0].repeatedPresses & 0x2000) {
            g_inputBuffer[0].repeatedPresses &= ~0x2000U;
            if (++runtime.tabs.hovered > 2)
                runtime.tabs.hovered = 0;
            AicaExecuteWrapper(3);
        }
        if (g_inputBuffer[0].pressedPriority & 0x400) {
            g_inputBuffer[0].pressedPriority &= ~0x400U;
            runtime.tabs.selected = runtime.tabs.hovered;
            runtime.tabs.hovered = -1;
            runtime.tabs.active = 0;
            SetUIGridTabSelector(&runtime.tabs, 1);
            AicaExecuteWrapper(2);
        }
        if (g_inputBuffer[0].pressedPriority & 0x8000) {
            g_inputBuffer[0].pressedPriority &= ~0x8000U;
            runtime.tabs.active = 0;
            runtime.tabs.hovered = -1;
            SetUIGridTabSelector(&runtime.tabs, 1);
            AicaExecuteWrapper(3);
        }
    }
    if (!runtime.tabs.coloursInitialized)
        runtime.tabs.hovered = -1;
    const float x = uiPointer.position.x * 1920.0f / GetUIViewportWidth();
    std::int32_t pointerTab = -1;
    for (std::int32_t tab = 0; tab < 3; ++tab) {
        if (runtime.tabPanels[tab] < 0)
            continue;
        const d3t::UIRect &bounds = runtime.layout.panels[runtime.tabPanels[tab]];
        if (x < bounds.x || x >= bounds.x + bounds.width || uiPointer.position.y < bounds.y || uiPointer.position.y >= bounds.y + bounds.height)
            continue;
        pointerTab = tab;
        if (uiPointer.leftClicks) {
            runtime.tabs.selected = tab;
            AicaExecuteWrapper(2);
        }
        if (runtime.pointerTab != tab) {
            runtime.tabs.previousHover = -1;
            runtime.tabs.hovered = tab;
            AicaExecuteWrapper(3);
        }
    }
    if (runtime.pointerTab >= 0 && pointerTab != runtime.pointerTab && runtime.tabs.hovered == runtime.pointerTab) {
        runtime.tabs.hovered = -1;
        runtime.tabs.coloursInitialized = 0;
    }
    runtime.pointerTab = pointerTab;
    UpdateOmakeCollectionTabs();
    UIGrid &grid = runtime.grids[runtime.tabs.selected];
    ProcessUIGridInput(grid, uiPointer, GetFrameTime());
    return 0;
}
std::int64_t RenderOmakeCollectionUI() {
    OmakeCollectionRuntime &runtime = omakeCollectionUI;
    if (!runtime.bound)
        return 0;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ShenmueCollection)];
    ResolveCollectionNativeLayout(definition, runtime.layout);
    if (runtime.holder >= 0) {
        const d3t::UIRect &holder = runtime.layout.panels[runtime.holder];
        const float sx = GetUIViewportWidth() / 1920.0f;
        runtime.grids[runtime.tabs.selected].bounds = {holder.x * sx, holder.y, holder.width * sx, holder.height};
    }
    DrawCollectionDefinition(UIElement::ShenmueCollection, runtime.layout, runtime.rowPanels);
    RenderUIGrid(runtime.grids[runtime.tabs.selected], static_cast<std::uint32_t>(UIElement::ShenmueCollection), uiPointer);
    if (drawingUI)
        DrawUIGridScrollbar(runtime.grids[runtime.tabs.selected], static_cast<std::uint32_t>(UIElement::ShenmueCollection), uiPointer);
    return 0;
}
std::int64_t DestroyOmakeCollectionUI() {
    if (!areas::IsOmakeSelectionPending()) {
        omakeCollectionUI.returnReset = false;
        SetHangOnCompletionEnabled(1);
    }
    return 1;
}
std::int64_t ResetOmakeCollectionUI() {
    omakeCollectionUI.bound = false;
    return 0;
}
std::int64_t ResetOmakeCollectionRender() {
    if (!g_uiElements[static_cast<std::size_t>(UIElement::ShenmueCollection)].isRenderable)
        SelectMainMenuPage(4);
    return 0;
}
void RefreshMainMenuTitle() {
    const std::uint32_t page = mainMenuPage;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::MainMenu)];
    const std::int32_t titlePage = page >= 1 && page <= 4 ? static_cast<std::int32_t>(page) : 0;
    for (d3t::UITextureObjectDefinition &object : definition.textureObjects) {
        if (object.id == "Image_Main_Menu_Title")
            object.frameIndex = titlePage + 5 * GetLoadingScreenTextType(ResetSceneTextLanguage()) + 7;
        if (object.id == "Image_Main_Menu_Title_Head_Back")
            object.frameIndex = (titlePage == 4 ? 1 : 0) + 1;
        for (std::size_t index = 0; index < mainMenuBackgroundAlpha.size(); ++index) {
            if (object.id != OMAKE_MAIN_MENU_BACKGROUNDS[index])
                continue;
            object.colour = (object.colour & 0xffffffU) | (titlePage ? static_cast<std::uint32_t>(mainMenuBackgroundAlpha[index]) << 24 : 0);
        }
    }
}
bool SelectMainMenuPage(std::uint32_t page) {
    if (page >= 6)
        return false;
    if (mainMenuPage < 6) {
        const UIElement previous = static_cast<UIElement>(OMAKE_MAIN_MENU_PAGES[mainMenuPage]);
        if (previous == UIElement::Options && AskKeepOrRevertChanges())
            return false;
        if (!DestroyElementByIndex(previous))
            return false;
    }
    ShowElementByIndex(static_cast<UIElement>(OMAKE_MAIN_MENU_PAGES[page]));
    mainMenuPage = page;
    d3t::UIDefinition &child = uiDefinitions[OMAKE_MAIN_MENU_PAGES[page]];
    if (OMAKE_MAIN_MENU_ROOTS[page])
        for (d3t::UIPanelDefinition &panel : child.panels)
            if (panel.id == OMAKE_MAIN_MENU_ROOTS[page])
                panel.parentId = "MainMenu::ContentPanel";
    RefreshMainMenuTitle();
    return true;
}
std::int64_t DestroyMainMenuPage() {
    if (mainMenuPage < 6)
        DestroyElementByIndex(static_cast<UIElement>(OMAKE_MAIN_MENU_PAGES[mainMenuPage]));
    DestroyElementByIndex(UIElement::GameHud);
    return 1;
}
