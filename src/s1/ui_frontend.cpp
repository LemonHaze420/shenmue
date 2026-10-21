#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {
std::uint64_t currentNanoseconds();

float splashElapsed();

bool validateActiveRenderHandle();

void setSplashState(std::int32_t state);

bool CaptureFrontendOptionsSettingsFile();

void MainMenuWheel_RefreshProgressPreset();

std::int32_t MainMenuWheel_ReadInput(std::int32_t index);

void MainMenuWheel_Update();

void PlayMainMenuAcceptSound();
}

namespace shenmue::s1 {
std::int32_t ConfirmChanges();
}

namespace shenmue::s1::ui_runtime_detail {
bool UpdateStatByIdx(std::int32_t current, std::int32_t next);

void QuitGame_Callback(UIRetry70ManBattlePromptResult response);

void MainMenuWheel_ResetLayout();

void MainMenuWheel_ActivateItem(std::int32_t item);

void MainMenuWheel_Clicked(float *, void *data);

void MainMenuWheel_UpdateImages();

void MainMenu_UpdateTitle();

void PressStart_StartIntro();

void PressStart_EndIntro();

void PressStart_SelectLogo();

UIAnimSlot *YesNoPrompt_Slot(std::int32_t index);

void YesNoPrompt_StartTransition();

void YesNoPrompt_UpdateSelectionSprites();

void YesNoPrompt_OnSelectionChanged(float *, void *data);

void YesNoPrompt_OnSelectionConfirmed(float *rect, void *data);

void YesNoPrompt_Close();

void Finish70ManBattleFlow(bool leaveToDisplay);

void Do70MB_callback(UIRetry70ManBattlePromptResult result);

void Retry70ManBattle(UIRetry70ManBattlePromptResult result);
}

namespace shenmue::s1 {
bool UI_ShowRetry70ManBattlePrompt(UIRetry70ManBattlePromptCallback callback);
}

namespace shenmue::s1::ui_runtime_detail {
void GameMenu_SwitchSubmenu(std::uint32_t current, std::uint32_t next);

void GameMenu_TabClicked(float *, void *data);
}

namespace shenmue::s1 {
bool AskToKeepOrRevert();

void DoYouWantToQuit_UnsavedChangesLost(UIRetry70ManBattlePromptResult result);
}

namespace shenmue::s1::d3t::ui_runtime_detail {
std::int32_t CheckAllKeysAreBound();
}

namespace shenmue::s1::ui_runtime_detail {
void UI_ScriptNoOpCallback(float *, void *);

std::int32_t Options_ReadAudioDevices();

void AssignMultipleChoice();

bool Options_SettingsChanged();

void UpdateSettings();

void ReadConfig();

UIOption *Options_FindOption(std::int32_t index, UIOption *table, std::int32_t count);

void Options_ReadSettings(UIOption *table, std::int32_t count);

void Options_ApplyOption(UIOption &option);

void Options_SelectPreviousChoice(float *, void *data);

void Options_SelectNextChoice(float *, void *data);

void Options_SelectFirstChoice(float *, void *data);

void Options_SelectSecondChoice(float *, void *data);

void Options_BeginKeyBinding(float *, void *data);

void Options_DescribeOption(float *rect, void *data);

void Options_RedescribeOption();

void Options_InvalidateWindowRects();

void Options_RevertSettings();

void KeepOptions(UIRetry70ManBattlePromptResult result);

void KeepOrRevertSettingsInternal();
}

namespace shenmue::s1 {
void KeepOrRevertSettings(UIRetry70ManBattlePromptResult result);
}

namespace shenmue::s1::ui_runtime_detail {
void ResetSettings(UIRetry70ManBattlePromptResult result);

void ResetSettingsToDefaults(UIRetry70ManBattlePromptResult result);

void RevertSettings(UIRetry70ManBattlePromptResult result);

std::int32_t Options_FindBoundAction(std::int32_t action, std::int32_t key);

void FormatKeyString(const char *token, std::int32_t key, const UIOption *option);

void Options_ShowText(const char *text);

void CopyStringToUI(const char *text, const char *name, float seconds);

void BindOrUnbind(UIRetry70ManBattlePromptResult result);

std::int32_t KeyboardMapping(std::int32_t row, UIOption *table, std::int32_t count);

void Options_SkipHeading(UIGrid &grid, UIOption *table, std::int32_t count);

void Options_QueueRowCallback(UIGrid *grid, std::int32_t element, UIOption &option, std::int32_t window, d3t::UIMouseCallback clicked);

void Options_SetOverlay(UIButtonWidget *overlay, std::int32_t index);

void Options_SetAlphaByte(std::uint32_t &colour, std::uint32_t alpha);

void OptionsRow_DrawToggle(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option);

void OptionsRow_DrawList(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option);

void OptionsRow_DrawSlider(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option);

void OptionsRow_DrawKeyBinding(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option);

void OptionsRow_Draw(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *data, std::int32_t index);

void Options_DrawChildRow(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element,
    std::int32_t index, UIOption *table, std::int32_t count, UIGridDrawCell rowDraw);

void Options_SetUpChildGrid(UIGrid &grid, d3t::UIWindow *panel, std::int32_t rows, UIGridDrawCell drawCell, UIGridGetElement getElement, UIOption *table);

void Options_TickChild(std::int32_t elementIndex, UIGrid &grid);

std::int64_t Options_PressChild(UIGrid &grid, UIOption *table, std::int32_t count, UIRetry70ManBattlePromptCallback reset, bool controls);

void Options_AskAvailability(const UIOption *table, std::int32_t count);

void Options_RecordRows(const UIOption *table, std::int32_t count, std::uint64_t *recorded);

void Options_SwitchTab();

void Options_OnTabHovered(float *, void *data);

void Options_OnTabHoverLeave(float *, void *data);

void Options_OnTabClicked(float *, void *data);

void UI_ResetLabelScaleVariables(std::uint32_t slot);

void FrontendAutomation_OptionsTick();
}

namespace shenmue::s1 {
void ReturnToTitleScreen();
}

namespace shenmue::s1::ui_runtime_detail {

std::uint64_t currentNanoseconds() {
    const auto frequency = platform::performanceCounterFrequency();
    const auto counter = platform::performanceCounter();
    return static_cast<std::uint64_t>(1000000000LL * (counter / frequency) + 1000000000LL * (counter % frequency) / frequency);
}

float splashElapsed() {
    return static_cast<float>(currentNanoseconds() - g_splashStateStart) / 1000000000.0f;
}

bool validateActiveRenderHandle() {
    if (g_activeRenderHandle == 0) {
        return true;
    }
    if (d3t_IsRenderHandleValid(g_activeRenderHandle) && d3t_IsRenderHandlePending(g_activeRenderHandle)) {
        return false;
    }
    d3t_CloseRenderHandle(g_activeRenderHandle);
    return g_activeRenderHandle == 0;
}

void setSplashState(std::int32_t state) {
    g_splashState = state;
    g_splashStateStart = currentNanoseconds();
}

std::uint32_t LoadPressStart() {
    UIBaseRuntime &pressStart = g_uiElements[0x3c];
    if (!pressStart.setup) {
        pressStart.setup = 1;
        pressStart.someFlag3 = 1;
        constexpr char path[] = "/ui/mainmenu_New/MainMenu_PressStart.ui";
        std::memcpy(pressStart.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, pressStart);
        pressStart.config.screenWidth = static_cast<std::int16_t>(handle);
        pressStart.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        pressStart.config.init = 1;
    }
    return static_cast<std::uint32_t>(pressStart.setup);
}

std::uint32_t LoadMainMenu() {
    UIBaseRuntime &mainMenu = g_uiElements[4];
    if (!mainMenu.setup) {
        mainMenu.setup = 1;
        mainMenu.someFlag3 = 1;
        constexpr char path[] = "/ui/mainmenu_New/MainMenu.ui";
        std::memcpy(mainMenu.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, mainMenu);
        mainMenu.config.screenWidth = static_cast<std::int16_t>(handle);
        mainMenu.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        mainMenu.config.init = 1;
    }
    return static_cast<std::uint32_t>(mainMenu.setup);
}

bool CaptureFrontendOptionsSettingsFile() {
    g_frontendAutomationOriginalSettingsFile.clear();
    std::ifstream input("Settings", std::ios::binary);
    if (!input) {
        g_frontendAutomationSettingsFileExisted = false;
        g_frontendAutomationSettingsFileCaptured = true;
        return true;
    }
    g_frontendAutomationSettingsFileExisted = true;
    g_frontendAutomationOriginalSettingsFile.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    g_frontendAutomationSettingsFileCaptured = !input.bad();
    return g_frontendAutomationSettingsFileCaptured;
}

bool RestoreFrontendOptionsSettingsFile() {
    if (!g_frontendAutomationSettingsFileCaptured)
        return false;
    if (!g_frontendAutomationSettingsFileExisted) {
        std::ifstream existing("Settings", std::ios::binary);
        if (!existing)
            return true;
        existing.close();
        if (std::remove("Settings") != 0)
            return false;
        std::ifstream verify("Settings", std::ios::binary);
        return !verify;
    }

    std::ofstream output("Settings", std::ios::binary | std::ios::trunc);
    if (!output)
        return false;
    output.write(g_frontendAutomationOriginalSettingsFile.data(), static_cast<std::streamsize>(g_frontendAutomationOriginalSettingsFile.size()));
    output.flush();
    if (!output.good())
        return false;
    output.close();

    std::vector<char> restored;
    std::ifstream input("Settings", std::ios::binary);
    if (!input)
        return false;
    restored.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    return !input.bad() && restored == g_frontendAutomationOriginalSettingsFile;
}

void SetFrontendAutomationStage(FrontendAutomationStage stage, const char *optionsDiagnostic) {
    g_frontendAutomationStage = stage;
    g_frontendAutomationFrames = 0;
    if (g_frontendLaunchTarget == FrontendLaunchTarget::Options && optionsDiagnostic) {
    }
}

std::uint32_t LoadMainMenuWheel() {
    return LoadPresentationDefinition(kMainMenuWheelElement, "/ui/mainmenu_New/MainMenu_Wheel.ui");
}




void MainMenuWheel_RefreshProgressPreset() {
    UIButtonWheel *const wheel = g_activeMainMenuWheelLayout;
    if (!wheel)
        return;
    const bool progress = HPLG_HasBookkeepingProgress();
    wheel->items[2].field0[1] = progress ? 0 : 1;
    g_mainMenuWheelRefresh = 1;
    if (progress) {
        wheel->angles[1] = g_mainMenuWheelSnapshot.angles[1];
        wheel->angles[3] = g_mainMenuWheelSnapshot.angles[3];
    } else {
        wheel->angles[1] = 4.1887903f;
        wheel->angles[3] = 2.0943952f;
    }
}




std::int32_t MainMenuWheel_ReadInput(std::int32_t index) {
    std::uint16_t repeated = g_inputState.slots[0].repeatedPresses;
    std::int32_t step = 0;
    if (g_uiElements[kUiYesNoPrompt].isVisible || static_cast<std::uint32_t>(index) > 2U)
        return 0;
    UIButtonWheel &wheel = g_uiButtonWheels[static_cast<std::size_t>(index)];
    const auto turning = [&wheel] {
        const std::int32_t slot = wheel.items[static_cast<std::size_t>(wheel.selection & 7)].animSlots[0];
        return static_cast<std::uint32_t>(slot) < g_uiAnimSlots.size() && g_uiAnimSlots[static_cast<std::size_t>(slot)].status != 0;
    };
    if ((repeated & 0x8000U) != 0 && !turning()) {
        wheel.selection = wheel.selection - 1 < 0 ? wheel.count - 1 : wheel.selection - 1;
        repeated = 0;
        step = 1;
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        }

    if ((repeated & 0x4000U) != 0 && !turning()) {
        wheel.selection = wheel.selection + 1 >= wheel.count ? 0 : wheel.selection + 1;
        step = -1;
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        }

    return step;
}






void MainMenuWheel_Update() {
    if (g_mainMenuWheelIndex == -1)
        return;
    const std::int32_t step = MainMenuWheel_ReadInput(g_mainMenuWheelIndex);
    if (!step && !g_mainMenuWheelRefresh)
        return;
    UIAnim_Start(g_mainMenuWheelAnimSlot, 0.09f, 0.0f, 0.09f, 0.0f);
    UIButtonWheel *const wheel = g_activeMainMenuWheelLayout;
    g_mainMenuWheelRefresh = 0;
    if (!wheel)
        return;
    const auto skipped = [wheel](std::int32_t place) {
        return static_cast<std::uint32_t>(place) < wheel->items.size() && wheel->items[static_cast<std::size_t>(place)].field0[1] != 0;
    };
    for (std::int32_t index = 0; index < wheel->count && index < 8; ++index) {
        UIButtonWheelItem &item = wheel->items[static_cast<std::size_t>(index)];
        if (item.field0[1])
            continue;
        const std::int32_t previous = item.index;
        item.index = previous + step;
        if (skipped(previous + step))
            item.index = previous + step + step;
        if (item.index < 0)
            item.index = wheel->count - 1;
        else if (item.index >= wheel->count)
            item.index = 0;
        if (!item.index)
            wheel->selection = index;
        if (item.animSlots[0] == -1)
            item.animSlots[0] = UIAnimAllocSlot();
        if (item.animSlots[0] == -1)
            continue;
        const float from = wheel->angles[static_cast<std::size_t>(previous & 7)];
        float to = wheel->angles[static_cast<std::size_t>(item.index & 7)];
        if (step == -1) {
            if (from > to)
                to = to + 6.2831898f;
        } else if (to > from) {
            to = to + -6.2831898f;
        }
        UIAnim_Start(item.animSlots[0], from, to, 0.3f, 0.0f);
        if (!wheel->override)
            continue;
        if (item.animSlots[1] == -1)
            item.animSlots[1] = UIAnimAllocSlot();
        item.index84 += step;
        if (item.index84 < 0)
            item.index84 = wheel->count - 1;
        if (item.index84 >= wheel->count)
            item.index84 = 0;
        UIAnim_Start(item.animSlots[1], item.placeScale, wheel->scales[static_cast<std::size_t>(item.index84 & 7)][0], 0.3f, 0.0f);
    }
}

void PlayMainMenuAcceptSound() {
    PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
    }
}

namespace shenmue::s1 {



std::int32_t ConfirmChanges() {
    if (!g_uiElements[kUiSubMenuOptions].isVisible || !Options_SettingsChanged())
        return 0;
    d3t::MessageBox("UIOPTUNSAV", false, 1, KeepOrRevertSettings, 0.0f); // @text EN: "Do you wish to save your changes?" | JA: "変更した項目を保存しますか？"
    return 1;
}
}

namespace shenmue::s1::ui_runtime_detail {



bool UpdateStatByIdx(std::int32_t current, std::int32_t next) {
    if (current >= 0 && current < 4) {
        if (kMainMenuIds[static_cast<std::size_t>(current)] == kUiSubMenuOptions && ConfirmChanges() == 1)
            return false;
        if (!d3t::ResetElementByIdx(kMainMenuIds[static_cast<std::size_t>(current)]))
            return false;
    }
    if (next >= 0 && next < 4) {
        d3t::ActivateElementByIdx(kMainMenuIds[static_cast<std::size_t>(next)]);
        if (d3t::UIWindow *const page = g_mainMenuPageWindows[static_cast<std::size_t>(next)]; page && g_mainMenuContentPanel)
            std::snprintf(page->layout.parentId.data(), page->layout.parentId.size(), "%s", g_mainMenuContentPanel->id.data());
        g_mainMenuStatIndex = next;
        MainMenu_UpdateTitle();
    }
    return true;
}

void QuitGame_Callback(UIRetry70ManBattlePromptResult response) {
    if (response == UIRetry70ManBattlePromptResult::Yes)
        g_shouldQuit = true;
}




void MainMenuWheel_ResetLayout() {
    UIButtonWheel *const wheel = g_activeMainMenuWheelLayout;
    if (!wheel)
        return;
    MainMenuWheel_RefreshProgressPreset();
    wheel->items[0].index = 0;
    wheel->items[1].index = 1;
    wheel->items[2].index = 2;
    wheel->items[3].index = 3;
    for (std::size_t slot = 0; slot < 10; ++slot) {
        const S1SaveSummary *const summary = HPLG_GetSaveSummary(slot);
        if (!summary || !summary->numberOfSaves)
            continue;
        wheel->items[0].index = 3;
        wheel->items[1].index = 0;
        wheel->items[2].index = 1;
        wheel->items[3].index = HPLG_HasBookkeepingProgress() ? 2 : 1;
        break;
    }
    wheel->selection = 0;
    g_mainMenuWheelRefresh = 1;
}



void MainMenuWheel_ActivateItem(std::int32_t item) {
    PlayMainMenuAcceptSound();
    switch (item) {
    case 0:
        UI_CloseConversationUIAndReturnToFreeRoam();
        g_iconPresentation.savedHelpToken = 0;
        GetSaveSlotString();
        g_mainMenuNewGamePicked = 1;
        break;
    case 1:

        if (HPLG_IsStorageConnected()) {
            g_saveOrLoad = 0;
            g_loadType = 0;
            UpdateStatByIdx(g_mainMenuStatIndex, 1);
        }
        break;
    case 2:
        g_loadType = 3;
        g_saveOrLoad = 0;
        UpdateStatByIdx(g_mainMenuStatIndex, 3);
        if (g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_TITLE_LOAD) {
            g_saveDisplayModuleState = SAVE_DISPLAY_MODULE_TITLE_LOAD;
            if (g_timeValue)
                g_shouldTimeAdvance = 0;
        }
        break;
    case 3:
        UpdateStatByIdx(g_mainMenuStatIndex, 2);
        break;
    default:
        break;
    }
}


void MainMenuWheel_Clicked(float *, void *data) {
    if (!data)
        return;
    PlayMainMenuAcceptSound();
    MainMenuWheel_ActivateItem(static_cast<const UIButtonWheelItem *>(data)->field0[0]);
}




std::int64_t MainMenuWheelSelected() {
    g_mainMenuWheelAnimSlot = UIAnimAllocSlot();
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kMainMenuWheelElement].config.uiHandleIndex);
    g_mainMenuWheelIndex = UI_FindViewButtonWheel(slot, "MainMenuButtonWheel");
    if (g_mainMenuWheelIndex != -1) {
        const bool bound = static_cast<std::uint32_t>(g_mainMenuWheelIndex) <= 2U;
        g_activeMainMenuWheelLayout = bound ? &g_uiButtonWheels[static_cast<std::size_t>(g_mainMenuWheelIndex)] : nullptr;
        if (g_activeMainMenuWheelLayout)
            g_activeMainMenuWheelLayout->mouseCallbacks.clicked = MainMenuWheel_Clicked;
    }
    for (std::size_t image = 0; image < g_mainMenuWheelImages.size(); ++image)
        g_mainMenuWheelImages[image] = UI_FindViewImage(slot, kMainMenuWheelImages[image]);
    if (UIButtonWheel *const wheel = g_activeMainMenuWheelLayout) {
        for (std::int32_t item = 0; item < wheel->count && item < 8; ++item) {
            wheel->items[static_cast<std::size_t>(item)].field0[0] = item;
            wheel->items[static_cast<std::size_t>(item)].field0[1] = 0;
        }
        g_mainMenuWheelSnapshot = *wheel;
    }
    return 1;
}




std::int64_t MainMenuWheelInput() {
    InputSlot &input = g_inputState.slots[0];
    if ((input.pressedPriority & 0x200U) != 0) {
        if (g_currUIState == -1 || g_currUIState == 4)
            d3t::ResetElementByIdx(4);
        if (g_currUIState == -1 || g_currUIState == 0x3c)
            UI_ActivateElement(g_uiElements[0x3c]);
        UIBaseRuntime &mainMenu = g_uiElements[4];
        mainMenu.isHandledExternally = 1;
        if (mainMenu.elementDraw)
            mainMenu.elementDraw();
        UIBaseRuntime &hud = g_uiElements[kUiGameHud];
        hud.isHandledExternally = 1;
        if (hud.elementDraw)
            hud.elementDraw();
        if (!hud.isVisible || !hud.destroy || hud.destroy() == 1 || g_busy_reset == 1)
            hud.isVisible = 0;
        if (g_currUIState == -1 || g_currUIState == 0x3c)
            UI_ActivateElement(g_uiElements[0x3c]);
        PlayMainMenuAcceptSound();
        g_returnToPressStart = 1;
        input.pressedPriority = 0;
    }
    const bool animating = g_mainMenuWheelAnimSlot >= 0 && g_uiAnimSlots[static_cast<std::size_t>(g_mainMenuWheelAnimSlot)].status != 0;
    UIButtonWheel *const wheel = g_activeMainMenuWheelLayout;
    if ((input.pressedPriority & 0x400U) == 0 || animating) {
        g_mainMenuWheelArmed = 1;
    } else {
        input.pressedPriority = 0;
        if (g_mainMenuWheelArmed && wheel)
            MainMenuWheel_ActivateItem(wheel->selection);
    }
    if ((input.pressedPriority & 0x2U) != 0)
        d3t::MessageBox("UIQUITGAME", false, 0, QuitGame_Callback, 0.0f); // @text EN: "Exit to Windows?" | JA: "終了してWindowsに戻りますか？"
    if (!wheel)
        return 0;
    if (g_mainMenuWheelPreviousSelection != wheel->selection) {
        InformationBarMessage message{};
        message.noOutline = 1;
        message.text = wheel->items[static_cast<std::size_t>(wheel->selection & 7)].description.token;
        d3t::CopyStringToUILabel(&message);
    }
    g_mainMenuWheelPreviousSelection = wheel->selection;
    return 0;
}


std::int64_t MainMenuWheelTick() {
    const UIBaseRuntime &element = g_uiElements[kMainMenuWheelElement];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    MainMenuWheel_Update();
    return 0;
}



void MainMenuWheel_UpdateImages() {
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = English;
        ReadSubtitleGlyphsAndSubs(English);
    }
    UIButtonWheel *const wheel = g_activeMainMenuWheelLayout;
    if (!wheel || static_cast<std::uint32_t>(g_uiLoadedLanguage) >= g_mainMenuWheelImages.size() / 4)
        return;
    for (std::size_t item = 0; item < 4; ++item)
        wheel->items[item].field48[0] = g_mainMenuWheelImages[4 * static_cast<std::size_t>(g_uiLoadedLanguage) + item];
}




void MainMenuWheelRender() {
    LoadMainMenuWheel();
    g_inputState.slots[0].pressedPriority = 0;
    g_mainMenuNewGamePicked = 0;
    g_mainMenuWheelArmed = 0;
    UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
    g_iIsInGame = 0;
    MainMenuWheel_RefreshProgressPreset();
    MainMenuWheel_Update();
    InformationBarMessage message{};
    if (g_activeMainMenuWheelLayout)
        message.text = g_activeMainMenuWheelLayout->items[static_cast<std::size_t>(g_activeMainMenuWheelLayout->selection & 7)].description.token;
    message.noOutline = 1;
    d3t::CopyStringToUILabel(&message);
    MainMenuWheel_UpdateImages();
}


std::int64_t MainMenuWheel_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    constexpr std::array<std::int16_t, 4> kPrompts{45, 44, -1, 5};
    for (std::size_t slot = 0; slot < kPrompts.size(); ++slot)
        g_searchPromptIconSlots[slot] = kPrompts[slot];
    for (std::size_t slot = 0; slot < prompts.size(); ++slot)
        prompts[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(prompts.data());
}


void FrontendAutomation_MainMenuWheelTick() {
    if (g_frontendLaunchTarget != FrontendLaunchTarget::NewGame && g_frontendLaunchTarget != FrontendLaunchTarget::Continue &&
        g_frontendLaunchTarget != FrontendLaunchTarget::Options) {
        return;
    }
    if (g_frontendAutomationStage == FrontendAutomationStage::MainMenu) {
        SetFrontendAutomationStage(FrontendAutomationStage::Wheel, g_frontendLaunchTarget == FrontendLaunchTarget::Options ? "main-menu-ready" : nullptr);
    }
    if (g_frontendAutomationStage != FrontendAutomationStage::Wheel || !g_activeMainMenuWheelLayout)
        return;
    const std::int32_t selection = g_activeMainMenuWheelLayout->selection;
    if (g_frontendLaunchTarget == FrontendLaunchTarget::Options) {
        QueueFrontendAutomationInput(selection == 3 ? 0x400U : 0x4000U);
        return;
    }
    const std::int32_t target = g_frontendLaunchTarget == FrontendLaunchTarget::Continue ? 1 : 0;
    if (selection != target)
        QueueFrontendAutomationInput(0x4000U);
    else if (g_mainMenuStatIndex == 0 && !g_hasPickedSaveData)
        QueueFrontendAutomationInput(0x400U);
}


std::int64_t MainMenuWheelDestroy() {
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    return 1;
}

// @verified - p
void UpdateFrontendEmblemRotation() {
    if (g_isInGame == 1) {
        if (g_frontendEmblemAnimSlot != -1) {
            UI_ReleaseAnimSlot(g_frontendEmblemAnimSlot);
            g_frontendEmblemAnimSlot = -1;
            for (std::int32_t index = 0; index < g_frontendEmblemImageCount; ++index) {
                if (g_frontendEmblemImages[static_cast<std::size_t>(index)])
                    g_frontendEmblemImages[static_cast<std::size_t>(index)]->angle = 0.0f;
            }
        }
    } else if (g_isInGame == 0) {
        if (g_frontendEmblemAnimSlot == -1) {
            g_frontendEmblemAnimSlot = UIAnimAllocSlot();
            g_uiAnimSlots[static_cast<std::size_t>(g_frontendEmblemAnimSlot)].loopMode = 1;
        }
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_frontendEmblemAnimSlot)];
        if (slot.status == 0 && g_frontendEmblemAnimSlot != -1) {
            slot.allocated = 1;
            slot.interpolation = 2;
            slot.status = 1;
            slot.start[0] = 0.0f;
            slot.current[0] = 0.0f;
            slot.end[0] = 6.2831855f;
            slot.duration = 140.0f;
            slot.delay = 0.0f;
            slot.elapsed = 0.0f;
            slot.reserved40 = -1;
        }
        for (std::int32_t index = 0; index < g_frontendEmblemImageCount; ++index) {
            if (g_frontendEmblemImages[static_cast<std::size_t>(index)])
                g_frontendEmblemImages[static_cast<std::size_t>(index)]->angle = slot.current[0];
        }
    }
}
std::int64_t MainMenuSelected() {
    g_mainMenuPulseSlot = UIAnimAllocSlot();
    g_mainMenuContentPanel = UI_FindWindowById("MainMenu::ContentPanel");
    for (std::size_t page = 0; page < g_mainMenuPageWindows.size(); ++page)
        g_mainMenuPageWindows[page] = UI_FindWindowById(kMainMenuPageWindowIds[page]);
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[4].config.uiHandleIndex);
    const std::int32_t title = UI_FindViewImageWidget(slot, "Image_Main_Menu_Title");
    g_mainMenuTitleImage = static_cast<std::uint32_t>(title) < g_uiImageWidgets.size() ? &g_uiImageWidgets[static_cast<std::size_t>(title)] : nullptr;
    g_switchAccountButtonIcon = UI_GetWindow(UI_FindViewWindow(slot, "SwitchAccountButtonIcon"));
    g_switchAccountButtonLabel = UI_GetWindow(UI_FindViewWindow(slot, "SwitchAccountButtonLabel"));
    if (g_switchAccountButtonIcon)
        g_switchAccountButtonIcon->alpha = 0.0f;
    if (g_switchAccountButtonLabel)
        g_switchAccountButtonLabel->alpha = 0.0f;
    for (std::size_t image = 0; image < g_mainMenuTitleImageIndices.size(); ++image)
        g_mainMenuTitleImageIndices[image] = UI_FindViewImage(slot, kMainMenuTitleImages[image]);
    for (std::size_t image = 0; image < g_mainMenuFrontImages.size(); ++image) {
        g_mainMenuFrontImages[image] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kMainMenuFrontImageIds[image]));
        if (g_mainMenuFrontImages[image])
            g_mainMenuFrontImageAlphas[image] = static_cast<std::uint8_t>(g_mainMenuFrontImages[image]->colour >> 24U);
    }
    g_frontendEmblemImages[static_cast<std::size_t>(g_frontendEmblemImageCount++)] =
        UI_ColourImageRecord(UI_FindViewColourImage(slot, "Main_Menu::Emblem_Panel::Emblem_Image"));
    g_mainMenuUserIdLabel = UI_LabelRecord(UI_FindViewLabel(slot, "USER_ID"));
    g_mainMenuUserIdText[0] = 0;
    if (g_mainMenuUserIdLabel)
        g_mainMenuUserIdLabel->text = g_mainMenuUserIdText.data();
    return 1;
}




void MainMenu_UpdateTitle() {
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = English;
        ReadSubtitleGlyphsAndSubs(English);
    }
    const std::int32_t page = g_mainMenuStatIndex >= 1 && g_mainMenuStatIndex <= 3 ? g_mainMenuStatIndex : 0;
    if (g_mainMenuTitleImage)
        g_mainMenuTitleImage->image = page + 4 * g_uiLoadedLanguage + 6;
    for (std::size_t image = 0; image < g_mainMenuFrontImages.size(); ++image) {
        if (g_mainMenuFrontImages[image]) {
            const std::uint32_t alpha = page != 0 ? g_mainMenuFrontImageAlphas[image] : 0U;
            g_mainMenuFrontImages[image]->colour = (g_mainMenuFrontImages[image]->colour & 0x00ffffffU) | (alpha << 24U);
        }
    }
}



std::int64_t MainMenuTick() {
    if (static_cast<std::uint32_t>(g_mainMenuStatIndex) < g_mainMenuPageWindows.size()) {
        d3t::UIWindow *const page = g_mainMenuPageWindows[static_cast<std::size_t>(g_mainMenuStatIndex)];
        if (page && !page->layout.parentId[0] && g_mainMenuContentPanel)
            std::snprintf(page->layout.parentId.data(), page->layout.parentId.size(), "%s", g_mainMenuContentPanel->id.data());
    }
    const UIBaseRuntime &element = g_uiElements[4];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    g_mainMenuUserIdText[0] = 0;
    if (g_mainMenuUserIdLabel)
        g_mainMenuUserIdLabel->text = g_mainMenuUserIdText.data();
    return 0;
}


std::int64_t MainMenuReset() {
    g_mainMenuPageWindows = {};
    g_mainMenuTitleImage = nullptr;
    return UpdateStatByIdx(g_mainMenuStatIndex, g_mainMenuStatIndex) ? 1 : 0;
}



std::int64_t MainMenuInput() {
    if ((g_inputState.slots[0].pressedPriority & 0x200U) == 0)
        return 0;
    if (g_mainMenuStatIndex) {
        if (!UpdateStatByIdx(g_mainMenuStatIndex, 0))
            return 0;
    } else {
        if (g_mainMenuNewGamePicked)
            return 0;
        d3t::ResetElementByIdx(kUiGameHud);
        d3t::ActivateElementByIdx(0x3c);
    }
    PlayMainMenuAcceptSound();
    return 0;
}

std::int64_t FrontendAutomation_MainMenuTick() {
    // Continue: on the Load/Save panel, confirm the first slot and then the load prompt until DISP takes the load.
    if (g_frontendLaunchTarget == FrontendLaunchTarget::Continue && g_frontendAutomationStage == FrontendAutomationStage::Wheel) {
        if (g_loadRequested)
            SetFrontendAutomationStage(FrontendAutomationStage::Complete);
        else if (g_mainMenuStatIndex == 1 && g_uiElements[kUiLoadSave].config.init == 3)
            QueueFrontendAutomationInput(0x400U);
        return 0;
    }
    if (g_frontendLaunchTarget == FrontendLaunchTarget::Options)
        FrontendAutomation_OptionsTick();
    return 0;
}





void MainMenuRender() {
    LoadMainMenu();
    g_isRenderSuppressed = 1;
    UpdateStatByIdx(g_mainMenuStatIndex, 0);
    g_mainMenuOpened = 0;
    UI_ActivateElement(g_uiElements[kUiGameHud]);
    constexpr std::array<std::int16_t, 4> kMainMenuPrompts{45, 44, -1, -1};
    for (std::size_t slot = 0; slot < kMainMenuPrompts.size(); ++slot)
        g_searchPromptIconSlots[slot] = kMainMenuPrompts[slot];
    SRCH_ApplySearchPromptSlots();
    SND_SelectFreeWeird(1);
    if (g_mainMenuPulseSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_mainMenuPulseSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.start[0] = 0.7f;
        slot.current[0] = 0.7f;
        slot.end[0] = 1.0f;
        slot.delay = 0.0f;
        slot.duration = 0.6f;
        slot.reserved40 = -1;
        slot.loopMode = 2;
    }
}



std::int64_t MainMenuDestroy() {
    for (std::size_t slot = 0; slot < 4; ++slot)
        g_searchPromptIconSlots[slot] = -1;
    SRCH_ApplySearchPromptSlots();
    for (std::int32_t &slot : g_hudPromptRequestedSlots)
        slot = static_cast<std::int32_t>(static_cast<std::uint32_t>(slot) | 0xffffU);
    d3t::ResetElementByIdx(kMainMenuIds[static_cast<std::size_t>(g_mainMenuStatIndex)]);
    UIBaseRuntime &gameHud = g_uiElements[kUiGameHud];
    if (!gameHud.isVisible || !gameHud.destroy || gameHud.destroy() == 1 || g_busy_reset == 1)
        gameHud.isVisible = 0;
    g_mainMenuStatIndex = 0;
    return 1;
}



void PressStart_StartIntro() {
    for (std::size_t window = 0; window < g_pressStartWindows.size(); ++window) {
        if (g_pressStartWindows[window])
            g_pressStartWindows[window]->alpha = 0.0f;
        if (g_pressStartFadeSlots[window] == -1)
            g_pressStartFadeSlots[window] = UIAnimAllocSlot();
    }
    if (g_uiElements[0x3c].config.init == 3) {
        constexpr std::array<float, 4> kDelays{0.0f, 0.5f, 1.5f, 1.5f};
        for (std::size_t window = 0; window < kDelays.size(); ++window) {
            if (g_pressStartFadeSlots[window] == -1)
                continue;
            UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_pressStartFadeSlots[window])];
            slot.allocated = 1;
            slot.status = 1;
            slot.interpolation = 2;
            slot.start[0] = 0.0f;
            slot.current[0] = 0.0f;
            slot.end[0] = 1.0f;
            slot.delay = kDelays[window];
            slot.elapsed = 0.0f;
            slot.duration = 1.0f;
            slot.reserved40 = -1;
        }
    }
    g_pressStartIntroTime = 0.0f;
}



void PressStart_EndIntro() {
    if (g_pressStartIntroActive) {
        g_pressStartIntroActive = false;
        for (std::int32_t &slotIndex : g_pressStartFadeSlots) {
            if (slotIndex == -1)
                continue;
            UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
            slotIndex = -1;
            slot.allocated = 0;
            slot.interpolation = 0;
            slot.status = 0;
            slot.start[0] = 0.0f;
            slot.current[0] = 0.0f;
            slot.end[0] = 0.0f;
            slot.duration = 0.0f;
            slot.delay = 0.0f;
            slot.elapsed = 0.0f;
            slot.loopMode = 0;
            slot.reserved40 = -1;
        }
    }
    for (std::size_t window = 0; window < 4; ++window) {
        if (g_pressStartWindows[window])
            g_pressStartWindows[window]->alpha = 1.0f;
    }
}



void PressStart_SelectLogo() {
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = English;
        ReadSubtitleGlyphsAndSubs(English);
    }
    std::int32_t image = g_pressStartLogoImages[0];
    if (g_uiLoadedLanguage == Korean)
        image = g_pressStartLogoImages[1];
    else if (g_uiLoadedLanguage == ChineseTrad || g_uiLoadedLanguage == ChineseSimp)
        image = g_pressStartLogoImages[2];
    if (g_pressStartLogoImage)
        g_pressStartLogoImage->image = image;
}






std::int64_t PressStartSelected() {
    if (validateActiveRenderHandle())
        Shenmue_DispatchRenderPipeline();
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[0x3c].config.uiHandleIndex);
    for (std::size_t image = 0; image < g_pressStartLogoImages.size(); ++image)
        g_pressStartLogoImages[image] = UI_FindViewImage(slot, kPressStartLogoImages[image]);
    for (std::size_t window = 0; window < g_pressStartWindows.size(); ++window) {
        g_pressStartWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kPressStartWindows[window]));
        g_pressStartFadeSlots[window] = -1;
    }
    g_pressStartEmblemImage = UI_ColourImageRecord(UI_FindViewColourImage(slot, "Press_Start::Emblem_Panel::Emblem_Image"));
    for (std::size_t icon = 0; icon < g_pressStartIcons.size(); ++icon)
        g_pressStartIcons[icon] = UI_ButtonIconRecord(UI_FindViewIconWidget(21, slot, kPressStartIconIds[icon]));
    g_pressStartLabel = UI_LabelRecord(UI_FindViewLabel(slot, "PressStart::PressStart"));
    g_pressStartBlinkSlot = UIAnimAllocSlot();
    if (g_pressStartBlinkSlot != -1) {
        UIAnimSlot &blink = g_uiAnimSlots[static_cast<std::size_t>(g_pressStartBlinkSlot)];
        blink.allocated = 1;
        blink.status = 1;
        blink.interpolation = 3;
        blink.start[0] = std::bit_cast<float>(0);
        blink.current[0] = std::bit_cast<float>(0);
        blink.end[0] = std::bit_cast<float>(2);
        blink.delay = 0.0f;
        blink.elapsed = 0.0f;
        blink.duration = 1.0f;
        blink.reserved40 = -1;
        blink.loopMode = 1;
    }
    const std::int32_t logo = UI_FindViewImageWidget(slot, "Press_Start::Logo_Panel::Logo_Image");
    g_pressStartLogoImage = static_cast<std::uint32_t>(logo) < g_uiImageWidgets.size() ? &g_uiImageWidgets[static_cast<std::size_t>(logo)] : nullptr;
    PressStart_SelectLogo();
    if (g_pressStartIntroActive)
        PressStart_StartIntro();
    g_frontendEmblemImages[static_cast<std::size_t>(g_frontendEmblemImageCount++)] = g_pressStartEmblemImage;
    return 1;
}


std::int64_t PressStartInput() {
    if (g_pressStartIntroActive) {
        if ((g_inputState.slots[0].pressedPriority & 0xfec6U) != 0)
            PressStart_EndIntro();
        return 0;
    }
    if ((g_inputState.slots[0].pressedPriority & 0x400U) == 0) {
        return 0;
    }
    MainMenuWheel_ResetLayout();
    PlayMainMenuAcceptSound();
    if (g_currUIState == -1 || g_currUIState == 0x3c)
        d3t::ResetElementByIdx(0x3c);
    if (g_currUIState == -1 || g_currUIState == 4) {
        d3t::ActivateElementByIdx(4);
        if (g_frontendLaunchTarget != FrontendLaunchTarget::None && g_frontendAutomationStage == FrontendAutomationStage::PressStart) {
            SetFrontendAutomationStage(
                FrontendAutomationStage::MainMenu, g_frontendLaunchTarget == FrontendLaunchTarget::Options ? "press-start-accepted" : nullptr);
        }
    }
    g_mainMenuOpened = 1;
    return 0;
}





std::int64_t PressStartTick() {
    const std::uint8_t console = g_keyboardActive != 0 ? 0x00U : 0xffU;
    const auto setIconAlpha = [](UIButtonIconWidget *icon, std::uint8_t alpha) {
        if (!icon)
            return;
        icon->colour = (icon->colour & 0x00ffffffU) | (static_cast<std::uint32_t>(alpha) << 24U);
        icon->fontColour = (icon->fontColour & 0x00ffffffU) | (static_cast<std::uint32_t>(alpha) << 24U);
    };
    setIconAlpha(g_pressStartIcons[0], console);
    setIconAlpha(g_pressStartIcons[1], static_cast<std::uint8_t>(~console));
    d3t::UIWindow *const box = g_pressStartWindows[4];
    if (g_pressStartIntroActive) {
        for (std::size_t window = 0; window < g_pressStartWindows.size(); ++window) {
            if (g_pressStartWindows[window] && g_pressStartFadeSlots[window] != -1)
                g_pressStartWindows[window]->alpha = g_uiAnimSlots[static_cast<std::size_t>(g_pressStartFadeSlots[window])].current[0];
        }
        g_pressStartIntroTime += g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f;
        if (g_pressStartIntroTime >= 3.0f)
            PressStart_EndIntro();
        if (g_pressStartLabel)
            g_pressStartLabel->field44 = 0;
        if (box)
            box->alpha = 0.0f;
    } else if (g_pressStartBlinkSlot != -1) {
        const std::int32_t step = std::bit_cast<std::int32_t>(g_uiAnimSlots[static_cast<std::size_t>(g_pressStartBlinkSlot)].current[0]) % 4;
        if (g_pressStartLabel)
            g_pressStartLabel->field44 = step != 0 ? 1 : 0;
        if (box)
            box->alpha = step != 0 ? 1.0f : 0.0f;
    }
    const UIBaseRuntime &element = g_uiElements[0x3c];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    if (--g_pressStartIdleFrames < 0) {
        g_bIsNewGame = 1;
        g_pressStartIdleFrames = 600;
        UIBaseRuntime &pressStart = g_uiElements[0x3c];
        if ((g_currUIState == -1 || g_currUIState == 0x3c) && (!pressStart.isVisible || !pressStart.destroy || pressStart.destroy() == 1 || g_busy_reset == 1))
            pressStart.isVisible = 0;
    }
    return 0;
}



void PressStartRender() {
    g_iIsInGame = 0;
    LoadPressStart();

    if (g_pressStartIntroPending) {
        g_pressStartIntroActive = true;
        PressStart_StartIntro();
        SND_SelectFreeWeird(0);
    } else {
        PressStart_EndIntro();
    }
    PressStart_SelectLogo();
    g_pressStartIdleFrames = 600;
}


std::int64_t PressStartDestroy() {
    g_pressStartIntroActive = false;
    g_pressStartIdleFrames = 600;
    return 1;
}

std::int64_t Splash() {
    g_splashPanel = true;
    return 1;
}

std::int64_t ResetSplash() {
    g_splashPanel = false;
    return 1;
}

std::int64_t InitShaders() {
    return 1;
}

std::int64_t GetInput() {
    const auto &input = g_inputState.slots[0];
    const nk_mouse &mouse = GNuklearWindow.input.mouse;
    g_skipSplash = g_skipSplash || g_skipFrontendSplash || (input.pressedPriority & 0x400U) != 0 || mouse.buttons[NK_BUTTON_LEFT].clicked ||
        mouse.buttons[NK_BUTTON_MIDDLE].clicked || mouse.buttons[NK_BUTTON_RIGHT].clicked;
    return 0;
}

void InitLoadingSplash() {
    LoadSplash();
    const std::size_t firstSplash = g_dbg_is_jp_ver != 0 ? 0U : 2U;
    g_currentSplashScreenIndex = static_cast<std::int32_t>(firstSplash);
    for (std::size_t index = firstSplash; index < g_splashScreens.size(); ++index) {
        SplashScreenRuntime &screen = g_splashScreens[index];
        screen.definition = &kSplashImages[index];
        d3t::LoadTextureFromFilepath(screen.definition->texturePath, 0, &screen.textureHandle, g_activeRenderHandle);
    }
    if (g_skipFrontendSplash) {

        g_currentSplashScreenIndex = static_cast<std::int32_t>(g_splashScreens.size()) - 1;
        g_skipSplash = false;
        g_forceSkipSplashScreens = 1;
        setSplashState(2);
        return;
    }
    setSplashState(0);
}

std::int64_t ProcessGlobalGameState() {
    if (g_currentSplashScreenIndex < 0 || g_currentSplashScreenIndex >= static_cast<std::int32_t>(g_splashScreens.size())) {
        return 0;
    }
    SplashScreenRuntime &screen = g_splashScreens[static_cast<std::size_t>(g_currentSplashScreenIndex)];
    const SplashImageDefinition &definition = *screen.definition;
    const float elapsed = splashElapsed();
    float opacity = 0.0f;
    switch (g_splashState) {
    case 0:
        if (elapsed > .5f && screen.textureHandle != 0) {
            screen.resolvedTextureHandle = screen.textureHandle;
            std::uint32_t width = 0;
            std::uint32_t height = 0;
            if (!GetTexture2DDimensions(screen.resolvedTextureHandle, &width, &height)) {
                screen.resolvedTextureHandle = 0;
                break;
            }
            std::memset(screen.imageRegion, 0, sizeof(screen.imageRegion));
            std::memcpy(screen.imageRegion, &screen.resolvedTextureHandle, sizeof(screen.resolvedTextureHandle));
            const auto regionWidth = static_cast<std::uint16_t>(width);
            const auto regionHeight = static_cast<std::uint16_t>(height);
            std::memcpy(screen.imageRegion + 8, &regionWidth, sizeof(regionWidth));
            std::memcpy(screen.imageRegion + 10, &regionHeight, sizeof(regionHeight));
            std::memcpy(screen.imageRegion + 16, &width, sizeof(width));
            std::memcpy(screen.imageRegion + 20, &height, sizeof(height));
            setSplashState(1);
        }
        break;
    case 1:
        opacity = definition.duration > 0.0f ? elapsed / definition.duration : 1.0f;
        if (elapsed > definition.duration) {
            setSplashState(2);
            opacity = 1.0f;
        }
        break;
    case 2:
        opacity = 1.0f;
        if ((g_forceSkipSplashScreens || elapsed > definition.maximumDuration || (g_skipSplash && elapsed > definition.minimumDuration)) &&
            (g_currentSplashScreenIndex != 5 || validateActiveRenderHandle())) {
            setSplashState(3);
        }
        break;
    case 3:
        opacity = definition.fadeOutDuration > 0.0f ? 1.0f - elapsed / definition.fadeOutDuration : 0.0f;
        if (elapsed > definition.fadeOutDuration) {
            g_skipSplash = false;
            if (++g_currentSplashScreenIndex < 6) {
                setSplashState(0);
            } else {
                ResetSplash();
                d3t::ResetElementByIdx(0x3b);
                LoadPressStart();
                d3t::ActivateElementByIdx(0x3c);
            }
        }
        break;
    default:
        break;
    }
    g_splashOpacity = std::clamp(opacity, 0.0f, 1.0f);
    return 0;
}


UIAnimSlot *YesNoPrompt_Slot(std::int32_t index) {
    return static_cast<std::uint32_t>(index) < g_uiAnimSlots.size() ? &g_uiAnimSlots[static_cast<std::size_t>(index)] : nullptr;
}


void YesNoPrompt_StartTransition() {
    UIAnimSlot *const slot = YesNoPrompt_Slot(g_yesNoPromptTransitionSlot);
    if (!slot)
        return;
    slot->allocated = 1;
    slot->status = 1;
    slot->interpolation = 3;
    slot->start[0] = 0.0f;
    slot->current[0] = 0.0f;
    slot->end[0] = std::bit_cast<float>(6);
    slot->delay = 0.0f;
    slot->elapsed = 0.0f;
    slot->duration = 0.75f;
    slot->reserved40 = -1;
}



void YesNoPrompt_UpdateSelectionSprites() {
    const bool left = g_messageBoxSelection == 2 && !g_messageBoxState.blinkOff;
    const bool right = g_messageBoxSelection == 1 && !g_messageBoxState.blinkOff;
    const auto show = [](UIButtonIconWidget *sprite, bool shown) { sprite->colour = (sprite->colour & 0x00ffffffU) | (shown ? 0xff000000U : 0U); };
    if (g_yesNoPromptSprites[0] && g_yesNoPromptSprites[1]) {
        show(g_yesNoPromptSprites[0], left);
        show(g_yesNoPromptSprites[1], !left);
    }
    if (g_yesNoPromptSprites[2] && g_yesNoPromptSprites[3]) {
        show(g_yesNoPromptSprites[2], right);
        show(g_yesNoPromptSprites[3], !right);
    }
}



void YesNoPrompt_OnSelectionChanged(float *, void *data) {
    if (g_messageBoxState.confirmed)
        return;
    const std::int32_t previous = g_messageBoxSelection;
    g_messageBoxSelection = (data == g_yesNoPromptWindows[0]) + 1;
    if (previous != g_messageBoxSelection)
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
    YesNoPrompt_UpdateSelectionSprites();
}



void YesNoPrompt_OnSelectionConfirmed(float *rect, void *data) {
    if (g_messageBoxState.confirmed)
        return;
    YesNoPrompt_OnSelectionChanged(rect, data);
    g_messageBoxState.confirmed = 1;
    YesNoPrompt_StartTransition();
    PlayMainMenuAcceptSound();
}



std::int64_t YesNoPrompt_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiYesNoPrompt].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_yesNoPromptWindows.size(); ++window)
        g_yesNoPromptWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kYesNoPromptWindowIds[window]));
    for (std::size_t image = 0; image < g_yesNoPromptTextImages.size(); ++image) {
        const std::int32_t index = UI_FindViewImageWidget(slot, kYesNoPromptTextImageIds[image]);
        g_yesNoPromptTextImages[image] = static_cast<std::uint32_t>(index) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(index)] : nullptr;
    }
    for (std::size_t sprite = 0; sprite < g_yesNoPromptSprites.size(); ++sprite)
        g_yesNoPromptSprites[sprite] = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, kYesNoPromptSpriteIds[sprite]));
    for (std::size_t image = 0; image < g_yesNoPromptImages.size(); ++image)
        g_yesNoPromptImages[image] = UI_FindViewImage(slot, kYesNoPromptImageIds[image]);
    for (std::size_t side = 0; side < 2; ++side) {
        if (d3t::UIWindow *const window = g_yesNoPromptWindows[side]) {
            window->mouseCallbacks.clicked = YesNoPrompt_OnSelectionConfirmed;
            window->mouseCallbacks.entered = YesNoPrompt_OnSelectionChanged;
        }
    }
    g_yesNoPromptTimeoutSlot = UIAnimAllocSlot();
    return 1;
}


std::int64_t YesNoPrompt_Reset() {
    g_yesNoPromptWindows.fill(nullptr);
    g_yesNoPromptTextImages.fill(nullptr);
    g_yesNoPromptSprites.fill(nullptr);
    return 0;
}




void YesNoPrompt_Render() {
    g_messageBoxState.rendered = 1;
    g_yesNoPromptTransitionSlot = UIAnimAllocSlot();
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = English;
        ReadSubtitleGlyphsAndSubs(English);
    }
    const std::size_t language = static_cast<std::size_t>(g_uiLoadedLanguage);
    if (g_yesNoPromptTextImages[0] && 2 + language < g_yesNoPromptImages.size())
        g_yesNoPromptTextImages[0]->image = g_yesNoPromptImages[2 + language];
    if (g_yesNoPromptTextImages[1] && 9 + language < g_yesNoPromptImages.size())
        g_yesNoPromptTextImages[1]->image = g_yesNoPromptImages[9 + language];
    YesNoPrompt_UpdateSelectionSprites();
    PlayMainMenuAcceptSound();
}



std::int64_t YesNoPrompt_Destroy() {
    g_messageBoxState = {};
    InformationBarLabels &labels = g_informationBarLabels;
    labels.savedMessage.textLiteral = 1;
    if (UIAnimSlot *const slot = YesNoPrompt_Slot(g_yesNoPromptTransitionSlot)) {
        slot->allocated = 0;
        slot->interpolation = 0;
        slot->status = 0;
        slot->start[0] = 0.0f;
        slot->current[0] = 0.0f;
        slot->end[0] = 0.0f;
        slot->duration = 0.0f;
        slot->delay = 0.0f;
        slot->elapsed = 0.0f;
        slot->loopMode = 0;
        slot->reserved40 = -1;
    }
    labels.savedMessage.text = labels.savedText.data();
    d3t::CopyStringToUILabel(&labels.savedMessage);
    return 1;
}

// The YesNo prompt closes as an element does (its Destroy, unless busy resetting, may keep it).
void YesNoPrompt_Close() {
    UIBaseRuntime &yesNo = g_uiElements[kUiYesNoPrompt];
    if (!yesNo.isVisible || !yesNo.destroy || yesNo.destroy() == 1 || g_busy_reset == 1)
        yesNo.isVisible = 0;
}





std::int64_t YesNoPrompt_ProcessMessageBox() {
    const auto answer = [](std::int32_t result) {
        if (g_messageBoxCallback)
            g_messageBoxCallback(static_cast<UIRetry70ManBattlePromptResult>(result));
    };
    UIAnimSlot *const timeout = YesNoPrompt_Slot(g_yesNoPromptTimeoutSlot);
    if (g_messageBoxState.hasTimeLimit == 1 && timeout && timeout->current[0] <= 0.0f) {
        YesNoPrompt_Close();
        answer(3);
        timeout->status = 0;
        timeout->elapsed = 0.0f;
        timeout->allocated = 1;
    }
    std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if ((pressed & 0x8000U) != 0 || (pressed & 0x4000U) != 0 || (pressed & 0x200U) != 0) {
        if (!g_messageBoxState.confirmed) {
            if ((pressed & 0x8000U) != 0) {
                if (g_messageBoxSelection != 1) {
                    g_messageBoxSelection = 1;
                    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
                }
            } else if ((pressed & 0x4000U) != 0 && g_messageBoxSelection != 2) {
                g_messageBoxSelection = 2;
                PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
            }
            if ((g_inputState.slots[0].pressedPriority & 0x200U) != 0) {
                PlayMainMenuAcceptSound();
                YesNoPrompt_Close();
                answer(4);
            }
        }
        YesNoPrompt_UpdateSelectionSprites();
        pressed = g_inputState.slots[0].pressedPriority;
    }
    if (g_messageBoxState.confirmed) {
        const UIAnimSlot *const transition = YesNoPrompt_Slot(g_yesNoPromptTransitionSlot);
        const std::int32_t step = transition ? std::bit_cast<std::int32_t>(transition->current[0]) : 0;
        g_messageBoxState.blinkOff = step % 2 != 1;
        if (step == 6) {
            YesNoPrompt_Close();
            answer(g_messageBoxSelection);
        }
        YesNoPrompt_UpdateSelectionSprites();
        return 0;
    }
    if ((pressed & 0x400U) == 0)
        return 0;
    PlayMainMenuAcceptSound();
    g_messageBoxState.confirmed = 1;
    YesNoPrompt_StartTransition();
    return 0;
}



std::int64_t YesNoPrompt_Tick() {
    if (g_messageBoxState.hasTimeLimit == 1) {
        InformationBarMessage message = g_informationBarLabels.message;
        const UIAnimSlot *const timeout = YesNoPrompt_Slot(g_yesNoPromptTimeoutSlot);
        const float remaining = timeout ? timeout->current[0] : 0.0f;
        const char *const text = g_messageBoxUseString == 1 ? g_messageBoxToken : InitAndRetrieveStringFromID(g_messageBoxToken);
        const std::int32_t seconds = g_messageBoxUseString == 1 ? static_cast<std::int32_t>(remaining) : static_cast<std::int32_t>(remaining) + 1;
        std::snprintf(g_messageBoxTimedText.data(), g_messageBoxTimedText.size(), "%s%i", text, seconds);
        message.text = g_messageBoxTimedText.data();
        d3t::ShowMessageStr(&message, -1.0f);
    }
    const UIBaseRuntime &element = g_uiElements[kUiYesNoPrompt];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}


std::int64_t YesNoPrompt_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    constexpr std::array<std::int16_t, 4> kPrompts{45, 44, -1, -1};
    for (std::size_t slot = 0; slot < kPrompts.size(); ++slot)
        g_searchPromptIconSlots[slot] = kPrompts[slot];
    for (std::size_t slot = 0; slot < prompts.size(); ++slot)
        prompts[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(prompts.data());
}
}

namespace shenmue::s1::d3t::ui_runtime_detail {




bool MessageBox(const char *token, bool useString, std::int32_t clearAfterDone, UIRetry70ManBattlePromptCallback callback, float timeBeforeInactive) {
    if (!callback || g_messageBoxState.rendered)
        return false;
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    informationBar.isHandledExternally = 1;
    if (informationBar.elementDraw)
        informationBar.elementDraw();
    g_messageBoxTimeout = timeBeforeInactive;
    g_messageBoxUseString = useString ? 1 : 0;
    g_messageBoxToken = token;
    g_messageBoxState.requested = 1;
    g_messageBoxCallback = callback;
    g_messageBoxSelection = (clearAfterDone == 1) + 1;
    g_messageBoxState.hasTimeLimit = 0;
    if (timeBeforeInactive > 0.0f) {
        g_messageBoxState.hasTimeLimit = 1;
        UIAnim_Start(g_yesNoPromptTimeoutSlot, timeBeforeInactive, 0.0f, timeBeforeInactive, 0.0f);
    }
    if (token) {
        InformationBarMessage message{};
        message.textLiteral = g_messageBoxUseString;
        message.text = token;
        message.noOutline = 1;
        d3t::ActivateElementByIdx(kUiGameMenuInformationBar);
        d3t::ShowMessageStr(&message, -1.0f);
    }
    d3t::ActivateElementByIdx(kUiYesNoPrompt);
    return true;
}
}

namespace shenmue::s1::ui_runtime_detail {



void Finish70ManBattleFlow(bool leaveToDisplay) {
    const UIRetry70ManBattlePromptCallback callback = g_retry70ManBattleFlowCallback;
    g_retry70ManBattleFlowCallback = nullptr;
    HideOwnedElement(kUiGameMenuInformationBar);
    g_uiElements[kUiGameMenuInformationBar].isHandledExternally = 1;

    if (leaveToDisplay) {
        HLib_UpdateTaskQueue(SystemModule::Display);
        return;
    }
    if (callback)
        callback(UIRetry70ManBattlePromptResult::Yes);
}



void Do70MB_callback(UIRetry70ManBattlePromptResult result) {
    if (!g_retry70ManBattleFlowCallback)
        return;
    if (result == UIRetry70ManBattlePromptResult::Yes) {
        shenUpdateFighterStats(ActiveSaveImage());
        g_saveWriteStatus.exchange(1);
        const d3t::D3TTaskDescriptor task{.data = nullptr, .callback = BOOK_KEEP, .completion = nullptr, .priority = 2};
        d3t::d3t_QueueTask(&task, 4);
        while (g_saveWriteStatus == 1) {
        }
        if (g_saveWriteStatus == 2)
            CreateSaveDataInfoForMenu(g_loadType);
        if (g_saveWriteStatus != 2) {
            if (!d3t::MessageBox("UI70MBSAVE", false, 1, Do70MB_callback, 0.0f)) // @text EN: "Do you wish to save?" | JA: "今回の記録をセーブしますか？"
                Finish70ManBattleFlow(true);
            return;
        }
    }
    Finish70ManBattleFlow(true);
}

void Retry70ManBattle(UIRetry70ManBattlePromptResult result) {
    if (result == UIRetry70ManBattlePromptResult::Yes) {
        Finish70ManBattleFlow(false);
        return;
    }
    if (!d3t::MessageBox("UI70MBSAVE", false, 1, Do70MB_callback, 0.0f)) { // @text EN: "Do you wish to save?" | JA: "今回の記録をセーブしますか？"
        Finish70ManBattleFlow(true);
    }
}
}

namespace shenmue::s1 {

bool UI_ShowRetry70ManBattlePrompt(UIRetry70ManBattlePromptCallback callback) {
    if (!callback || g_retry70ManBattleFlowCallback)
        return false;
    g_retry70ManBattleFlowCallback = callback;
    if (d3t::MessageBox("UI70MBRETY", false, 1, Retry70ManBattle, 0.0f)) // @text EN: "Do you wish to retry?" | JA: "もう一度チャレンジしますか？"
        return true;
    g_retry70ManBattleFlowCallback = nullptr;
    return false;
}


void UI_CloseRetry70ManBattlePrompt() {
    g_retry70ManBattleFlowCallback = nullptr;
    YesNoPrompt_Close();
    g_uiElements[kUiYesNoPrompt].isHandledExternally = 1;
    HideOwnedElement(kUiGameMenuInformationBar);
    g_uiElements[kUiGameMenuInformationBar].isHandledExternally = 1;
}
}

namespace shenmue::s1::ui_runtime_detail {




void GameMenu_SwitchSubmenu(std::uint32_t current, std::uint32_t next) {
    g_gameMenuPendingSubmenu = static_cast<std::int32_t>(next);
    if (current < 5) {
        if ((kGameMenuSubmenus[current] == kUiSubMenuOptions && ConfirmChanges() == 1) || !d3t::ResetElementByIdx(kGameMenuSubmenus[current]))
            return;
        d3t::CopyStringToUILabel(nullptr);
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
    }
    if (next < 5) {
        g_gameMenuSubmenu = static_cast<std::int32_t>(next);
        d3t::ActivateElementByIdx(kGameMenuSubmenus[next]);
        if (d3t::UIWindow *const selected = g_gameMenuWindows[5]; selected && g_gameMenuWindows[next])
            std::snprintf(selected->layout.parentId.data(), selected->layout.parentId.size(), "%s", g_gameMenuWindows[next]->id.data());
    }
    for (std::size_t tab = 0; tab < g_gameMenuIcons.size(); ++tab) {
        const std::uint32_t alpha = static_cast<std::uint8_t>(static_cast<std::int32_t>((next == tab ? 1.0f : 0.25f) * 255.0f));
        if (d3t::UILabelWidget *const label = g_gameMenuLabels[3 + tab])
            label->colour = (label->colour & 0x00ffffffU) | (alpha << 24U);
        if (UIColourImageWidget *const icon = g_gameMenuIcons[tab])
            icon->colour = (icon->colour & 0x00ffffffU) | (alpha << 24U);
    }
    g_gameMenuPendingSubmenu = -1;
}


void GameMenu_TabClicked(float *, void *data) {
    std::uint32_t next = static_cast<std::uint32_t>(g_gameMenuSubmenu);
    for (std::uint32_t tab = 0; tab < 5; ++tab) {
        if (data == g_gameMenuWindows[tab]) {
            next = tab;
            break;
        }
    }
    if (static_cast<std::uint32_t>(g_gameMenuSubmenu) != next)
        GameMenu_SwitchSubmenu(static_cast<std::uint32_t>(g_gameMenuSubmenu), next);
}
}

namespace shenmue::s1 {


void WriteInput_Callback(const UIGridEntry &entry) {
    if (entry.id != 3)
        return;
    const bool searchIdle =
        g_searchCandidateId < 0 ? ((g_currentSearchState - 2) & ~3) != 0 || g_currentSearchState == 4 || g_searchCandidateId == -1 : g_searchCandidateId == 0;
    if (!searchIdle)
        return;
    g_isPlayerGameStateActive = 6;
    EnqueueTMEX();
    g_userResponse = 1;
    d3t::Shared_Items_Tick(entry.type, entry.id);
    UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
    gameMenu.isHandledExternally = 1;
    if (gameMenu.elementDraw)
        gameMenu.elementDraw();
}


bool AskToKeepOrRevert() {
    if (g_uiElements[kUiSubMenuOptions].isVisible && Options_SettingsChanged()) {
        d3t::MessageBox("UIOPTUNSAV", false, 1, KeepOrRevertSettings, 0.0f); // @text EN: "Do you wish to save your changes?" | JA: "変更した項目を保存しますか？"
        return false;
    }
    if (g_currUIState != -1 && g_currUIState != kUiGameMenu)
        return false;
    UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
    if (gameMenu.isVisible && gameMenu.destroy && gameMenu.destroy() != 1 && g_busy_reset != 1)
        return false;
    gameMenu.isVisible = 0;
    shenExecuteAICACommand(0x100abU, 0, 0);
    g_iconPresentation.menuState = UIIconSystemMenuState::CloseRequested;
    return true;
}



void DoYouWantToQuit_UnsavedChangesLost(UIRetry70ManBattlePromptResult result) {
    if (result != UIRetry70ManBattlePromptResult::Yes || (g_currUIState != -1 && g_currUIState != kUiGameMenu))
        return;
    UIBaseRuntime &gameMenu = g_uiElements[kUiGameMenu];
    if (gameMenu.isVisible && gameMenu.destroy && gameMenu.destroy() != 1 && g_busy_reset != 1)
        return;
    gameMenu.isVisible = 0;
    if (g_uiElements[kUiSubMenuOptions].isVisible && Options_SettingsChanged()) {
        d3t::MessageBox("UIOPTUNSAV", false, 1, KeepOrRevertSettings, 0.0f); // @text EN: "Do you wish to save your changes?" | JA: "変更した項目を保存しますか？"
        return;
    }
    d3t::ResetElementByIdx(kUiSubMenuOptions);
    d3t::ResetElementByIdx(kUiGameMenuBackground);
    d3t::ResetElementByIdx(kUiGameMenuInformationBar);
    g_optionsPendingAction = -1;
    ReturnToTitleScreen();
}



std::int64_t GameMenu_IsPressed() {
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if ((pressed & 0x200U) != 0 || g_gameMenuCloseState == 1) {
        if ((pressed & 0x200U) != 0)
            g_gameMenuCloseState = 1;
        if (!g_iconPresentation.sharedItemMenuState && !g_tmex_suppressKeepOrRevertPrompt)
            g_gameMenuCloseState = AskToKeepOrRevert() ? 2 : g_gameMenuCloseState;
        return 0;
    }
    if (g_gameMenuCloseState == 2)
        return 0;
    std::uint32_t next = static_cast<std::uint32_t>(g_gameMenuSubmenu);
    if ((pressed & 0x88U) != 0) {
        next = static_cast<std::uint32_t>(g_gameMenuSubmenu) + 1U;
        if (next >= 5)
            next = 0;
    }
    if ((pressed & 0x50U) != 0 && --next >= 5)
        next = 4;
    if (static_cast<std::uint32_t>(g_gameMenuSubmenu) != next || g_gameMenuPendingSubmenu != -1)
        GameMenu_SwitchSubmenu(static_cast<std::uint32_t>(g_gameMenuSubmenu),
            g_gameMenuSubmenu != static_cast<std::int32_t>(next) ? next : static_cast<std::uint32_t>(g_gameMenuPendingSubmenu));
    return 1;
}



std::int64_t GameMenu_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameMenu].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_gameMenuWindows.size(); ++window)
        g_gameMenuWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kGameMenuWindowIds[window]));
    for (std::size_t tab = 0; tab < 5; ++tab) {
        if (g_gameMenuWindows[tab])
            g_gameMenuWindows[tab]->mouseCallbacks.clicked = GameMenu_TabClicked;
    }
    for (std::size_t label = 0; label < g_gameMenuLabels.size(); ++label)
        g_gameMenuLabels[label] = UI_LabelRecord(UI_FindViewLabel(slot, kGameMenuLabelIds[label]));
    const std::array<char *, 3> texts{g_gameMenuMoneyText.data(), g_gameMenuTokenText.data(), g_gameMenuDateText.data()};
    for (std::size_t value = 0; value < texts.size(); ++value) {
        if (d3t::UILabelWidget *const label = g_gameMenuLabels[value]) {
            label->text = texts[value];
            label->token.alreadyLocalized = 1;
        }
    }
    for (std::size_t icon = 0; icon < g_gameMenuIcons.size(); ++icon)
        g_gameMenuIcons[icon] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kGameMenuIconIds[icon]));
    return 1;
}


std::int64_t GameMenu_Reset() {
    g_gameMenuWindows.fill(nullptr);
    g_gameMenuLabels.fill(nullptr);
    g_gameMenuIcons.fill(nullptr);
    return 0;
}





std::int64_t GameMenu_Tick() {
    std::snprintf(g_gameMenuMoneyText.data(), g_gameMenuMoneyText.size(), "%s", FormatCashOverlayText(g_playerStats.cash).c_str());
    std::snprintf(
        g_gameMenuTokenText.data(), g_gameMenuTokenText.size(), "%s", FormatGroupedNumber(static_cast<std::int32_t>(g_playerStats.segaCoins)).c_str());
    GenerateLoadingScreenText(g_gameMenuDateText.data(), g_gameMenuDateText.size(), g_currentDateTime.year, g_currentDateTime.month, g_currentDateTime.day,
        g_currentDateTime.dayOfWeek, 2);
    UILayoutRect rect{};
    g_waitingForNextTick = 0;
    if (g_gameMenuWindows[8])
        UI_GetWindowRect(g_gameMenuWindows[8], rect);
    const float aspectWidth = static_cast<float>(UI_GetAspectWidth());
    const std::array<const char *, 3> texts{g_gameMenuDateText.data(), g_gameMenuTokenText.data(), g_gameMenuMoneyText.data()};
    for (std::size_t value = 0; value < texts.size(); ++value) {
        const d3t::UILabelWidget *const label = g_gameMenuLabels[2 - value];
        d3t::UIWindow *const window = g_gameMenuWindows[9 + value];
        if (!label || !window)
            continue;
        if (g_uiFontFace != label->fontFamily && static_cast<std::uint32_t>(label->fontFamily) < 4U) {
            g_uiFontFace = label->fontFamily;
            UI_SelectFont();
        }
        if (g_uiFontSize != label->size && static_cast<std::uint32_t>(label->size) < 6U) {
            g_uiFontSize = label->size;
            UI_SelectFont();
        }
        const nk_flags alignment = label->alignment;
        const UILabelFit fit = UI_FitLabelScale(GNuklearWindow, texts[value], -1, alignment, rect, rect.width, rect.height, 0);
        const UILayoutRect placed = UI_DrawLabelText(GNuklearWindow, texts[value], -1, alignment, rect, fit);
        window->layout.width.value.fraction = placed.width / aspectWidth;
        window->layout.height.value.fraction = rect.height / 1080.0f;
    }
    g_waitingForNextTick = 1;
    const UIBaseRuntime &element = g_uiElements[kUiGameMenu];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}


void GameMenu_Render() {
    ExitSearchAndMoveCameraRelative(1.6f, 0.17f, static_cast<std::uint16_t>(g_fCamAccumYaw));
    g_gameMenuCloseState = 0;
    UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
    g_iIsInGame = 1;
    g_saveOrLoad = 1;
    GameMenu_SwitchSubmenu(0xffffffffU, static_cast<std::uint32_t>(g_gameMenuSubmenu));
    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
}


std::int64_t GameMenu_Destroy() {
    if (!d3t::ResetElementByIdx(kGameMenuSubmenus[static_cast<std::size_t>(g_gameMenuSubmenu)])) {
        g_gameMenuCloseState = 1;
        return 0;
    }
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    ResetSearchToFree();
    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
    return 1;
}


std::int64_t GameMenu_StartDraw() {
    const std::int32_t element = kGameMenuSubmenus[static_cast<std::size_t>(g_gameMenuSubmenu)];
    const std::int32_t external = g_uiElements[kUiGameMenu].isHandledExternally ? 1 : 0;
    UIBaseRuntime &submenu = g_uiElements[static_cast<std::size_t>(element)];
    submenu.isHandledExternally = external;
    if (submenu.elementDraw)
        submenu.elementDraw();
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    informationBar.isHandledExternally = external;
    return informationBar.elementDraw ? informationBar.elementDraw() : 0;
}
}

namespace shenmue::s1::d3t::ui_runtime_detail {

std::int32_t CheckAllKeysAreBound() {
    for (std::size_t mapping = 0; mapping < kSavedInputMappingCount; ++mapping) {
        if (GetSavedInputMapping(g_applicationSettings.savedInput, mapping) == kUnboundInputMapping)
            return 1;
    }
    return 0;
}
}

namespace shenmue::s1::ui_runtime_detail {


void UI_ScriptNoOpCallback(float *, void *) {
}

// The inlined description lookup of the Options texts: a string id of up to 12 characters read from the string table.
const char *Options_ReadString(const char *id) {
    SubtitleStringId key{};
    std::size_t length = 0;
    while (length < sizeof(key) && id[length])
        ++length;
    std::memcpy(&key, id, length);
    return ReadStringFromStringTableById(key);
}




std::int32_t Options_ReadAudioDevices() {
    const char *const none = GetAudioDeviceName(0);
    std::int32_t count = static_cast<std::int32_t>(GetAudioDeviceCount());
    if (count > 10)
        count = 10;
    std::snprintf(g_optionsAudioDeviceTexts[0].data(), g_optionsAudioDeviceTexts[0].size(), "%s", InitAndRetrieveStringFromID(none));
    g_optionsAudioDeviceChoices[0] = g_optionsAudioDeviceTexts[0].data();
    std::int32_t device = 1;
    if (count > 1) {
        for (; device < count; ++device) {
            char *const text = g_optionsAudioDeviceTexts[static_cast<std::size_t>(device)].data();
            const char *const name = GetAudioDeviceName(static_cast<std::uint32_t>(device));
            if (!name) {
                std::snprintf(text, 16, "%d. Unknown Output", device + 1);
            } else if (std::strlen(name) <= 12) {
                std::snprintf(text, 18, "%d. %s", device, name);
            } else {
                std::array<char, 10> cut{};
                std::memcpy(cut.data(), name, 9);
                std::snprintf(text, 18, "%d. %s...", device, cut.data());
            }
            g_optionsAudioDeviceChoices[static_cast<std::size_t>(device)] = text;
        }
        if (device >= 6)
            return device;
    }
    g_optionsAudioDeviceChoices[static_cast<std::size_t>(device)] = nullptr;
    return device;
}


const char *Options_AudioDeviceText(std::int32_t choice) {
    if (choice >= 0) {
        const char *const none = GetAudioDeviceName(0);
        if (choice < static_cast<std::int32_t>(GetAudioDeviceCount())) {
            if (!choice)
                return InitAndRetrieveStringFromID(none);
            if (const char *const name = GetAudioDeviceName(static_cast<std::uint32_t>(choice)))
                return name;
        }
    }
    return InitAndRetrieveStringFromID("UIOPTAODDSC"); // @text EN: "Select which device is used for audio." | JA: "音声に使うデバイスを選択します。"
}


void AssignMultipleChoice() {
    constexpr std::array<std::int32_t, 1> kJapaneseLanguages{1};
    constexpr std::array<std::int32_t, 6> kInternationalLanguages{0, 2, 3, 4, 5, 6};
    constexpr std::array<std::int32_t, 7> kAllLanguages{0, 1, 2, 3, 4, 5, 6};
    std::array<std::int32_t, 7> languages{};
    std::size_t count = 0;
    if (g_dbg_is_jp_ver) {
        count = kJapaneseLanguages.size();
        languages[0] = kJapaneseLanguages[0];
    } else {
        count = kInternationalLanguages.size();
        for (std::size_t index = 0; index < count; ++index)
            languages[index] = kInternationalLanguages[index];
    }
    if (g_d3t_dbg_forceAllUiLanguages) {
        count = kAllLanguages.size();
        languages = kAllLanguages;
    }
    std::size_t index = 0;
    for (; index < count; ++index) {
        const std::int32_t language = languages[index];
        g_languageChoiceValues[index] = language;
        if (static_cast<std::uint32_t>(language) <= 6U)
            g_languageChoiceLabels[index] = kOptionsLanguageNames[static_cast<std::size_t>(language)];
    }
    g_languageChoiceLabels[index] = "UIOTGLDEF0"; // @text EN: "Default" | JA: "デフォルト"
    g_languageChoiceValues[index] = -1;
    UIOption &subtitles = g_optionsAudioTable[2];
    subtitles.choice = [](std::int32_t choice) { return g_languageChoiceLabels[static_cast<std::size_t>(choice)]; };
    subtitles.choiceValue = [](std::int32_t choice) { return g_languageChoiceValues[static_cast<std::size_t>(choice)]; };
    g_languageChoiceLabels[index + 1] = nullptr;
    g_languageChoiceValues[index + 1] = 0;
}


bool Options_SettingsChanged() {
    const auto *const settings = reinterpret_cast<const std::byte *>(&g_applicationSettings) + kOptionsSettingsOffset;
    return std::memcmp(g_tmpUserKeyMapping.data(), settings, kOptionsSettingsSize) != 0;
}


void UpdateSettings() {
    std::memcpy(g_tmpUserKeyMapping.data(), reinterpret_cast<const std::byte *>(&g_applicationSettings) + kOptionsSettingsOffset, kOptionsSettingsSize);
}


void ReadConfig() {
    std::memcpy(reinterpret_cast<std::byte *>(&g_applicationSettings) + kOptionsSettingsOffset, g_tmpUserKeyMapping.data(), kOptionsSettingsSize);
    ReadConfigFromSave();
}



UIOption *Options_FindOption(std::int32_t index, UIOption *table, std::int32_t count) {
    if (index < 0)
        return nullptr;
    std::int32_t target = index;
    std::int32_t hidden = 0;
    for (std::int32_t entry = 0; entry < count; ++entry) {
        const UIOption &option = table[entry];
        if (!option.available || option.available()) {
            if (entry >= target)
                break;
        } else {
            ++hidden;
            ++target;
        }
    }
    return hidden + index >= count ? nullptr : &table[hidden + index];
}




void Options_ReadSettings(UIOption *table, std::int32_t count) {
    for (std::int32_t entry = 0; entry < count; ++entry) {
        UIOption &option = table[entry];
        if (option.getInt) {
            const std::int32_t value = option.getInt();
            if (option.choice && option.choice(0)) {
                std::int32_t choice = 0;
                for (;;) {
                    if (value == (option.choiceValue ? option.choiceValue(choice) : 0)) {
                        option.current = choice;
                        break;
                    }
                    ++choice;
                    if (!option.choice || !option.choice(choice))
                        break;
                }
            }
        }
        if (option.getFloat)
            option.value = option.getFloat();
    }
}



void Options_ApplyOption(UIOption &option) {
    if (option.setInt)
        option.setInt(option.choiceValue ? option.choiceValue(option.current) : 0);
    if (option.setFloat)
        option.setFloat(option.value);
    ReadSettingsFile();
}


void Options_SelectPreviousChoice(float *, void *data) {
    UIOption *const option = static_cast<UIOption *>(data);
    if (!option)
        return;
    g_optionsPendingAction = -1;
    if (--option->current < 0) {
        std::int32_t last = 0;
        while (option->choice && option->choice(last + 1))
            ++last;
        option->current = last;
    }
    Options_ApplyOption(*option);
    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
}


void Options_SelectNextChoice(float *, void *data) {
    UIOption *const option = static_cast<UIOption *>(data);
    if (!option)
        return;
    g_optionsPendingAction = -1;
    ++option->current;
    if (!option->choice || !option->choice(option->current))
        option->current = 0;
    Options_ApplyOption(*option);
    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
}



void Options_SelectFirstChoice(float *, void *data) {
    UIOption *const option = static_cast<UIOption *>(data);
    if (!option)
        return;
    g_optionsPendingAction = -1;
    option->current = 0;
    Options_ApplyOption(*option);
    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
}

void Options_SelectSecondChoice(float *, void *data) {
    UIOption *const option = static_cast<UIOption *>(data);
    if (!option)
        return;
    g_optionsPendingAction = -1;
    option->current = 1;
    Options_ApplyOption(*option);
    PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
}


void Options_BeginKeyBinding(float *, void *data) {
    const UIOption *const option = static_cast<const UIOption *>(data);
    if (!option)
        return;
    std::int32_t row = -1;
    for (std::int32_t entry = 0; entry < static_cast<std::int32_t>(g_optionsControlsTable.size()); ++entry) {
        const UIOption &binding = g_optionsControlsTable[static_cast<std::size_t>(entry)];
        if (binding.type == UIOptionType::KeyBinding && binding.current == option->current) {
            row = entry;
            break;
        }
    }
    g_optionsPendingAction = row;
}



void Options_DescribeOption(float *rect, void *data) {
    UIOption *const option = static_cast<UIOption *>(data);
    g_optionsDescribedOption = option;
    InformationBarMessage message{};
    message.iconAction = -1;
    message.outlinePanel = -1;
    message.noOutline = g_isInGame == 0;
    message.outline = {rect[0], rect[1], rect[2], rect[3]};
    if (option && option->text) {
        message.text = option->text(-1);
        message.textLiteral = 1;
    }
    d3t::CopyStringToUILabel(&message);
}


void Options_RedescribeOption() {
    if (!g_optionsDescribedOption)
        return;
    InformationBarMessage message{};
    message.noOutline = g_isInGame == 0;
    message.iconAction = -1;
    message.outlinePanel = -1;
    message.outline = g_informationBarLabels.message.outline;
    if (g_optionsDescribedOption->text) {
        message.text = g_optionsDescribedOption->text(-1);
        message.textLiteral = 1;
    }
    d3t::CopyStringToUILabel(&message);
}

// The rect of every window drops its cache (inlined in KeepOptions and the revert paths after the display settings changed).
void Options_InvalidateWindowRects() {
    for (d3t::UIWindow &window : g_uiWindows)
        window.rectCached = 0;
}



void Options_RevertSettings() {
    ReadConfig();
    g_informationBarLabels.message.noOutline = 1;
    Options_InvalidateWindowRects();
    Options_ReadSettings(g_optionsControlsTable.data(), static_cast<std::int32_t>(g_optionsControlsTable.size()));
    Options_ReadSettings(g_optionsAudioTable.data(), static_cast<std::int32_t>(g_optionsAudioTable.size()));
    Options_ReadSettings(g_optionsGraphicsTable.data(), static_cast<std::int32_t>(g_optionsGraphicsTable.size()));
}



void KeepOptions(UIRetry70ManBattlePromptResult result) {
    const std::int32_t answer = static_cast<std::int32_t>(result);
    if (answer < 0 || (answer > 1 && static_cast<std::uint32_t>(answer - 3) > 1U)) {
        g_informationBarLabels.message.noOutline = 1;
        Options_InvalidateWindowRects();
        d3t::WriteSettingsFile();
        UpdateSettings();
        PlayGatedUiSound(g_optionsSuccessSoundFrame, kPlaySuccessSound);
    } else {
        Options_RevertSettings();
    }
    Options_RedescribeOption();
}



void KeepOrRevertSettingsInternal() {
    if (!Options_SettingsChanged() || d3t::CheckAllKeysAreBound())
        return;
    const bool display = IsValidDisplaySettings();
    ReadConfigFromSave();
    if (display) {
        d3t::MessageBox("UIOPTSKEEP", false, 1, KeepOptions, 15.0f); // @text EN: "Keep changes? Reverting in " | JA: "変更を適用しますか？元に戻るまであと： "
    } else {
        d3t::WriteSettingsFile();
        UpdateSettings();
        PlayGatedUiSound(g_optionsSuccessSoundFrame, kPlaySuccessSound);
    }
    g_optionsPendingAction = -1;
}
}

namespace shenmue::s1 {


void KeepOrRevertSettings(UIRetry70ManBattlePromptResult result) {
    if (result == UIRetry70ManBattlePromptResult::Yes)
        KeepOrRevertSettingsInternal();
    else
        Options_RevertSettings();
}
}

namespace shenmue::s1::ui_runtime_detail {



void ResetSettings(UIRetry70ManBattlePromptResult result) {
    if (result != UIRetry70ManBattlePromptResult::Yes)
        return;
    g_applicationSettings.savedAudioDevice = 1;
    g_applicationSettings.savedAudioLanguage = 0;
    g_applicationSettings.savedSubtitleLanguage = -1;
    g_applicationSettings.savedDialogueMode = 2;
    g_applicationSettings.savedDialogueVolume = 1.0f;
    g_applicationSettings.savedSfxVolume = 1.0f;
    g_applicationSettings.savedMusicVolume = 1.0f;
    ReadConfigFromSave();
    Options_ReadSettings(g_optionsAudioTable.data(), static_cast<std::int32_t>(g_optionsAudioTable.size()));
}



void ResetSettingsToDefaults(UIRetry70ManBattlePromptResult result) {
    if (result == UIRetry70ManBattlePromptResult::Yes) {
        SetDefaultSettings(&g_applicationSettings.savedInput);
        ReadConfigFromSave();
        Options_ReadSettings(g_optionsControlsTable.data(), static_cast<std::int32_t>(g_optionsControlsTable.size()));
    }
    Options_RedescribeOption();
}



void RevertSettings(UIRetry70ManBattlePromptResult result) {
    if (result != UIRetry70ManBattlePromptResult::Yes)
        return;
    SelectMonitorForDisplay(&g_applicationSettings.display);
    ReadConfigFromSave();
    Options_ReadSettings(g_optionsGraphicsTable.data(), static_cast<std::int32_t>(g_optionsGraphicsTable.size()));
}



std::int32_t Options_FindBoundAction(std::int32_t action, std::int32_t key) {
    const SavedInputSettings &saved = g_applicationSettings.savedInput;
    if (key == GetSavedInputMapping(saved, 7))
        return 7;
    std::int32_t first = 0;
    std::int32_t count = 18;
    if (action >= 32) {
        first = 32;
    } else if (action >= 22) {
        first = 22;
        count = 10;
    } else if (action >= 18) {
        first = 18;
        count = 4;
    }
    for (std::int32_t index = 0; index < count; ++index) {
        if (key == GetSavedInputMapping(saved, static_cast<std::size_t>(first + index)))
            return first + index;
    }
    return -1;
}



void FormatKeyString(const char *token, std::int32_t key, const UIOption *option) {
    std::snprintf(g_optionsKeyBindingText.data(), g_optionsKeyBindingText.size(), "%s", InitAndRetrieveStringFromID(token));
    if (key != -1) {
        std::array<char, 64> name{};
#if defined(_WIN32)
        const std::int32_t mapped =
            static_cast<std::int32_t>(ExtendedMapVirtualKey(static_cast<std::uint32_t>(key), reinterpret_cast<void *>(0x08090809), GetKeyboardLayout(0)));
#else
        const std::int32_t mapped = key;
#endif
        d3t::FormatVirtualKeyString(mapped, name);
        TextFormat(g_optionsKeyBindingText.data(), static_cast<std::int32_t>(g_optionsKeyBindingText.size()), "[KEY_NAME]", name.data());
    }
    if (option) {
        TextFormat(g_optionsKeyBindingText.data(), static_cast<std::int32_t>(g_optionsKeyBindingText.size()), "[INPUT_ACTION]",
            InitAndRetrieveStringFromID(option->name));
    }
}

// The Information Bar's current message takes `text` (localized) without its colour override and shows (inlined throughout Options).
void Options_ShowText(const char *text) {
    InformationBarMessage &message = g_informationBarLabels.message;
    message.text = text;
    message.textLiteral = 1;
    message.colourOverride = 0;
    d3t::CopyStringToUILabel(&message);
}


void CopyStringToUI(const char *text, const char *name, float seconds) {
    Options_ShowText(name);
    g_informationBarLabels.message.text = text;
    d3t::ShowMessageStr(&g_informationBarLabels.message, seconds);
}



void BindOrUnbind(UIRetry70ManBattlePromptResult result) {
    if (result == UIRetry70ManBattlePromptResult::Yes) {
        SavedInputSettings &saved = g_applicationSettings.savedInput;
        const std::int32_t bound = Options_FindBoundAction(g_optionsKeyMapAction, g_optionsLastKeyIndex);
        if (bound >= 0)
            SetSavedInputMapping(saved, static_cast<std::size_t>(bound), kUnboundInputMapping);
        SetSavedInputMapping(saved, static_cast<std::size_t>(g_optionsKeyMapAction), g_optionsLastKeyIndex);
        const UIOption *const action = g_optionsInputAction;
        FormatKeyString("UIKMAPBIND", g_optionsLastKeyIndex, action); // @text EN: "The [[KEY_NAME]] key has been successfully bound to the [[INPUT_ACTION]] input slot." | JA: "[[KEY_NAME]]キーは[[INPUT_ACTION]]の入力スロットにアサインされました。"
        Options_ShowText(InitAndRetrieveStringFromID(action ? action->name : nullptr));
        g_informationBarLabels.message.text = g_optionsKeyBindingText.data();
        d3t::ShowMessageStr(&g_informationBarLabels.message, 4.0f);
    }
    g_optionsPendingAction = -1;
}







std::int32_t KeyboardMapping(std::int32_t row, UIOption *table, std::int32_t count) {
    if (row == -1)
        return 0;
    UIOption *const option = Options_FindOption(row, table, count);
    g_optionsInputAction = option;
    if (d3t::CheckAllKeysAreBound() == 1 && !g_uiElements[kUiYesNoPrompt].isVisible) {
        InformationBarMessage &message = g_informationBarLabels.message;
        message.text = InitAndRetrieveStringFromID("UIKMAPUNBN"); // @text EN: "You have unbound keys." | JA: "アサインされていないキーがあります。"
        std::memcpy(message.colour.data(), &kOptionsKeyUnboundColour, sizeof(kOptionsKeyUnboundColour));
        message.textLiteral = 1;
        message.colourOverride = 1;
        d3t::CopyStringToUILabel(&message);
    }
    if (!option)
        return 0;
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if (g_optionsPendingAction == -1) {
        if ((pressed & 0x400U) != 0 && option->type != UIOptionType::Heading) {
            g_optionsPendingAction = row;
            g_optionsEditValue = option->value;
            g_optionsEditCurrent = option->current;
            Options_ApplyOption(*option);
            g_windowInput.lastKeyIndex = 0;
            ClearWindowInput();
            InitializeGlobalInput();
        }
        return 0;
    }
    if (row != g_optionsPendingAction) {
        g_optionsPendingAction = -1;
        return 0;
    }
    if (option->text)
        Options_ShowText(option->text(option->current));
    if (option->type != UIOptionType::KeyBinding) {
        bool done = false;
        if ((pressed & 0x200U) != 0) {
            option->value = g_optionsEditValue;
            option->current = g_optionsEditCurrent;
            g_optionsPendingAction = -1;
            Options_ApplyOption(*option);
            done = true;
        } else if ((pressed & 0x400U) != 0) {
            g_optionsPendingAction = -1;
            done = true;
        }
        if (done) {
            if (option->text)
                Options_ShowText(option->text(-1));
            return 1;
        }
    }
    switch (option->type) {
    case UIOptionType::Toggle:
        if ((pressed & 0x8000U) != 0)
            option->current = 1;
        else if ((pressed & 0x4000U) != 0)
            option->current = 0;
        else
            return 1;
        Options_ApplyOption(*option);
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        return 1;
    case UIOptionType::List:
        if ((pressed & 0x8000U) != 0) {
            ++option->current;
            if (!option->choice || !option->choice(option->current))
                option->current = 0;
        } else if ((pressed & 0x4000U) != 0) {
            if (--option->current < 0) {
                std::int32_t last = 0;
                while (option->choice && option->choice(last + 1))
                    ++last;
                option->current = last;
            }
        } else {
            return 1;
        }
        Options_ApplyOption(*option);
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        return 1;
    case UIOptionType::Slider: {
        const float minimum = static_cast<float>(option->choiceValue ? option->choiceValue(0) : 0);
        const std::int32_t maximum = option->choiceValue ? option->choiceValue(1) : 0;
        const float range = static_cast<float>(maximum) - minimum;
        const float units = range * option->value;
        const std::uint16_t held = g_inputState.slots[0].repeatedPresses;
        if ((held & 0x8000U) != 0) {
            option->value = std::fminf(range, units + 1.0f) / range;
            Options_ApplyOption(*option);
        } else if ((held & 0x4000U) != 0) {
            float stepped = units + -1.0f;
            if (stepped < 1.0f)
                stepped = 0.0f;
            option->value = stepped / range;
            Options_ApplyOption(*option);
        }
        return 1;
    }
    case UIOptionType::KeyBinding: {
        const std::int32_t action = option->current;
        g_optionsKeyMapAction = action;
        const std::int32_t key = g_windowInput.lastKeyIndex;
        if (key == 256 || (!g_windowInput.currentKeyStateChangedFlag && !g_windowInput.previousKeyStateChangedFlag)) {
            FormatKeyString("UIKMAPANYK", -1, option); // @text EN: "Press any key to bind it to the [[INPUT_ACTION]] input slot." | JA: "[[INPUT_ACTION]]の入力スロットにアサインするには、好きなキーを押してください。"
            Options_ShowText(g_optionsKeyBindingText.data());
            return 1;
        }
        g_optionsLastKeyIndex = key;
        if (IsSavedInputMappingKeyAllowed(static_cast<std::size_t>(action), key)) {
            const std::int32_t bound = Options_FindBoundAction(action, key);
            if (bound == -1 || bound == g_optionsKeyMapAction) {
                SetSavedInputMapping(g_applicationSettings.savedInput, static_cast<std::size_t>(g_optionsKeyMapAction), g_optionsLastKeyIndex);
                FormatKeyString("UIKMAPBIND", key, option); // @text EN: "The [[KEY_NAME]] key has been successfully bound to the [[INPUT_ACTION]] input slot." | JA: "[[KEY_NAME]]キーは[[INPUT_ACTION]]の入力スロットにアサインされました。"
                Options_ShowText(InitAndRetrieveStringFromID(option->name));
                g_informationBarLabels.message.text = g_optionsKeyBindingText.data();
                d3t::ShowMessageStr(&g_informationBarLabels.message, 4.0f);
                g_optionsPendingAction = -1;
                g_optionsLastKeyIndex = -1;
                ReadSettingsFile();
                Options_ReadSettings(g_optionsControlsTable.data(), static_cast<std::int32_t>(g_optionsControlsTable.size()));
                return 1;
            }
            Options_ShowText(InitAndRetrieveStringFromID(option->name));
            const UIOption *boundOption = nullptr;
            for (const UIOption &binding : g_optionsControlsTable) {
                if (binding.type == UIOptionType::KeyBinding && binding.current == bound) {
                    boundOption = &binding;
                    break;
                }
            }
            FormatKeyString("UIKMAPALBO", g_windowInput.lastKeyIndex, boundOption); // @text EN: "The [[KEY_NAME]] key is already bound to [[INPUT_ACTION]], do you want to unbind it?" | JA: "[[KEY_NAME]]キーはすでに[[INPUT_ACTION]]にアサインされています。解除しますか？"
            d3t::MessageBox(g_optionsKeyBindingText.data(), true, 1, BindOrUnbind, 0.0f);
            ReadSettingsFile();
            Options_ReadSettings(g_optionsControlsTable.data(), static_cast<std::int32_t>(g_optionsControlsTable.size()));
            return 1;
        }
        const char *const name = InitAndRetrieveStringFromID(option->name);
        CopyStringToUI(InitAndRetrieveStringFromID("UIKMAPINVK"), name, 4.0f); // @text EN: "This key cannot be bound. Please try another." | JA: "このキーはアサインできません。他のキーを選択してください。"
        g_optionsPendingAction = -1;
        g_optionsLastKeyIndex = -1;
        return 1;
    }
    default:
        return 1;
    }
}


void Options_SkipHeading(UIGrid &grid, UIOption *table, std::int32_t count) {
    const std::int32_t selected = UIGrid_SelectedIndex(grid);
    std::int32_t previous = -1;
    if (!g_uiFocusedList || (!g_uiFocusedList->hasFocus && !g_uiFocusedList->previousHasFocus)) {
        const std::int32_t row = grid.previousSelectedRow + grid.previousScrollRow < 0 ? 0 : grid.previousSelectedRow + grid.previousScrollRow;
        previous = grid.previousSelectedColumn + grid.columns * row;
    }
    const UIOption *const option = Options_FindOption(selected, table, count);
    if (option && !g_keyboardActive && option->type == UIOptionType::Heading)
        UIGrid_SelectElement(grid, selected <= previous ? selected - 1 : selected + 1);
}



void Options_QueueRowCallback(UIGrid *grid, std::int32_t element, UIOption &option, std::int32_t window, d3t::UIMouseCallback clicked) {
    if (option.cut)
        return;
    UILayoutRect rect{};
    UI_GetWindowRect(g_optionsWindows[static_cast<std::size_t>(window)], rect);
    UILayoutRect holder{};
    if (grid->holder)
        UI_GetWindowRect(grid->holder, holder);
    else
        holder = grid->rect;
    d3t::UIMouseCallbacks callbacks{.entered = grid->mouseCallbacks.entered,
        .hovered = grid->mouseCallbacks.hovered,
        .left = nullptr,
        .clicked = grid->mouseCallbacks.clicked,
        .isHovered = 0};
    rect.x = rect.x - holder.x;
    rect.y = rect.y - holder.y;
    if (clicked)
        callbacks.clicked = clicked;
    UI_WindowMouseCallbacks(callbacks, element, rect, &option);
}

// The row's control frame (a toggle's, list's, slider's or key binding's Overlay Button) shows only while another row is being edited.
void Options_SetOverlay(UIButtonWidget *overlay, std::int32_t index) {
    if (!overlay)
        return;
    std::uint32_t alpha = 0;
    if (g_optionsPendingAction != -1 && g_optionsPendingAction != index)
        alpha = 153;
    overlay->colour = (overlay->colour & 0x00ffffffU) | (alpha << 24U);
}

// The alpha byte of a widget colour.
void Options_SetAlphaByte(std::uint32_t &colour, std::uint32_t alpha) {
    colour = (colour & 0x00ffffffU) | ((alpha & 0xffU) << 24U);
}



void OptionsRow_DrawToggle(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option) {
    const float alpha = index == UIGrid_SelectedIndex(*grid) ? 1.0f : 0.6f;
    Options_QueueRowCallback(grid, element, option, 11, Options_SelectFirstChoice);
    Options_QueueRowCallback(grid, element, option, 10, Options_SelectSecondChoice);
    if (d3t::UILabelWidget *const name = g_optionsLabels[0])
        name->text = InitAndRetrieveStringFromID(option.name);
    const char *const second = option.choice ? option.choice(1) : nullptr;
    d3t::UILabelWidget *const right = g_optionsLabels[1];
    if (right)
        right->text = InitAndRetrieveStringFromID(second);
    const char *const first = option.choice ? option.choice(0) : nullptr;
    d3t::UILabelWidget *const left = g_optionsLabels[2];
    if (left)
        left->text = InitAndRetrieveStringFromID(first);
    if (option.current == 1) {
        if (right)
            Options_SetAlphaByte(right->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 255.0f)));
        if (left)
            Options_SetAlphaByte(left->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 38.25f)));
        if (UIButtonWidget *const button = g_optionsButtons[2])
            button->alpha = 1.0f;
        if (UIButtonWidget *const button = g_optionsButtons[3])
            button->alpha = 0.15f;
    } else if (option.current == 0) {
        if (right)
            Options_SetAlphaByte(right->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 38.25f)));
        if (left)
            Options_SetAlphaByte(left->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 255.0f)));
        if (UIButtonWidget *const button = g_optionsButtons[2])
            button->alpha = 0.15f;
        if (UIButtonWidget *const button = g_optionsButtons[3])
            button->alpha = 1.0f;
    }
    Options_SetOverlay(g_optionsButtons[4], index);
}



void OptionsRow_DrawList(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option) {
    Options_QueueRowCallback(grid, element, option, 20, Options_SelectPreviousChoice);
    Options_QueueRowCallback(grid, element, option, 19, Options_SelectNextChoice);
    Options_QueueRowCallback(grid, element, option, 21, Options_SelectNextChoice);
    if (d3t::UILabelWidget *const name = g_optionsLabels[3])
        name->text = InitAndRetrieveStringFromID(option.name);
    if (d3t::UILabelWidget *const selected = g_optionsLabels[4]) {
        const char *text = option.choice ? option.choice(option.current) : nullptr;
        while (!text && option.current > 0) {
            --option.current;
            text = option.choice ? option.choice(option.current) : nullptr;
        }
        if (option.localizeChoices == 1)
            text = InitAndRetrieveStringFromID(text);
        selected->text = text;
    }
    Options_SetOverlay(g_optionsButtons[7], index);
}




void OptionsRow_DrawSlider(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option) {
    UILayoutRect bar{};
    UI_GetWindowRect(g_optionsWindows[29], bar);
    UILayoutRect button{};
    UI_GetWindowRect(g_optionsWindows[30], button);
    const float left = bar.x;
    const float right = bar.width + bar.x - button.width;
    const std::int32_t aspectWidth = UI_GetAspectWidth();
    if (d3t::UIWindow *const knob = g_optionsWindows[30])
        knob->layout.offsetX = (right - left) * option.value / static_cast<float>(aspectWidth);
    button = {};
    UI_GetWindowRect(g_optionsWindows[30], button);
    const nk_context &context = GNuklearWindow;
    const float mouseX = context.input.mouse.pos.x;
    const float mouseY = context.input.mouse.pos.y;
    if (!option.dragging && context.input.mouse.buttons[NK_BUTTON_LEFT].clicked == 1 && button.x < mouseX && mouseX < button.x + button.width &&
        button.y < mouseY && mouseY < button.y + button.height) {
        option.dragging = 1;
    } else if (option.dragging == 1 && !context.input.mouse.buttons[NK_BUTTON_LEFT].down) {
        option.dragging = 0;
    }
    if (option.dragging == 1) {
        g_optionsPendingAction = -1;
        g_informationBarLabels.message.noOutline = 1;
        if (left <= mouseX) {
            if (mouseX > button.width + right) {
                option.value = 1.0f;
            } else {
                option.value = (mouseX - option.dragX) / (right - left) + option.value;
                if (option.value < 0.0f)
                    option.value = 0.0f;
                if (option.value > 1.0f)
                    option.value = 1.0f;
            }
        } else {
            option.value = 0.0f;
        }
        Options_ApplyOption(option);
    }
    option.dragX = mouseX;
    Options_QueueRowCallback(grid, element, option, 30, nullptr);
    if (d3t::UILabelWidget *const name = g_optionsLabels[5])
        name->text = InitAndRetrieveStringFromID(option.name);
    if (d3t::UILabelWidget *const percent = g_optionsLabels[6]) {
        const float maximum = static_cast<float>(option.choiceValue ? option.choiceValue(1) : 0);
        const float minimum = static_cast<float>(option.choiceValue ? option.choiceValue(0) : 0);
        std::snprintf(g_optionsSliderText.data(), g_optionsSliderText.size(), "%i%%", static_cast<std::int32_t>((maximum - minimum) * option.value + minimum));
        percent->text = g_optionsSliderText.data();
    }
    Options_SetOverlay(g_optionsButtons[11], index);
}



void OptionsRow_DrawKeyBinding(std::int32_t index, UIGrid *grid, std::int32_t element, UIOption &option) {
    const std::int32_t action = option.current;
    const std::int32_t key = action < 50 ? GetSavedInputMapping(g_applicationSettings.savedInput, static_cast<std::size_t>(action)) : -1;
    const float alpha = index == UIGrid_SelectedIndex(*grid) ? 1.0f : 0.6f;
    Options_QueueRowCallback(grid, element, option, 32, Options_BeginKeyBinding);
    if (d3t::UILabelWidget *const name = g_optionsLabels[7]) {
        name->text = InitAndRetrieveStringFromID(option.name);
        name->colour = key == -1 ? kOptionsKeyUnboundColour : kOptionsKeyBoundColour;
        Options_SetAlphaByte(name->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 255.0f)));
    }
    if (g_optionsKeyBindingKey)
        g_optionsKeyBindingKey->action = option.current;
    Options_SetOverlay(g_optionsButtons[14], index);
}




void OptionsRow_Draw(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *data, std::int32_t index) {
    if (g_uiElements[kUiSubMenuOptions].isHandledExternally)
        return;
    UIOption &option = *static_cast<UIOption *>(data);
    option.cut = UIGrid_PlaceCellWidgets(g_optionsRowTemplate, index, *holder, *cell);
    std::int32_t endId = 16;
    std::int32_t firstId = 9;
    const float alpha = index == UIGrid_SelectedIndex(*grid) ? 1.0f : 0.6f;
    for (std::size_t label = 0; label < 8; ++label) {
        if (d3t::UILabelWidget *const widget = g_optionsLabels[label])
            Options_SetAlphaByte(widget->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 255.0f)));
    }
    switch (option.type) {
    case UIOptionType::Toggle:
        OptionsRow_DrawToggle(index, grid, element, option);
        break;
    case UIOptionType::List:
        firstId = 16;
        endId = 25;
        OptionsRow_DrawList(index, grid, element, option);
        break;
    case UIOptionType::Slider:
        endId = 32;
        firstId = 25;
        OptionsRow_DrawSlider(index, grid, element, option);
        break;
    case UIOptionType::KeyBinding:
        endId = 40;
        firstId = 32;
        OptionsRow_DrawKeyBinding(index, grid, element, option);
        break;
    case UIOptionType::Heading:
        firstId = 40;
        endId = -1;
        if (d3t::UILabelWidget *const title = g_optionsLabels[8])
            title->text = InitAndRetrieveStringFromID(option.name);
        break;
    default:
        break;
    }
    const std::int32_t selected = UIGrid_SelectedIndex(*grid);
    const auto show = [](std::size_t image, std::uint32_t alpha) {
        if (UIColourImageWidget *const widget = g_optionsColourImages[image])
            Options_SetAlphaByte(widget->colour, alpha);
    };
    if (g_isInGame) {
        for (const std::size_t game : {17U, 27U, 38U, 43U})
            show(game, 0xffU);
        for (const std::size_t other : {18U, 19U, 28U, 29U, 37U, 39U, 44U, 45U})
            show(other, 0);
    } else {
        for (const std::size_t front : {18U, 28U, 37U, 44U})
            show(front, 0xffU);
        for (const std::size_t other : {17U, 19U, 27U, 29U, 38U, 39U, 43U, 45U})
            show(other, 0);
        if (selected == index) {
            switch (option.type) {
            case UIOptionType::Toggle:
                show(18, 0);
                show(19, 0xffU);
                break;
            case UIOptionType::List:
                show(28, 0);
                show(29, 0xffU);
                break;
            case UIOptionType::Slider:
                show(37, 0);
                show(39, 0xffU);
                break;
            case UIOptionType::KeyBinding:
                show(44, 0);
                show(45, 0xffU);
                break;
            default:
                break;
            }
        }
    }
    UIGrid_AfterDraw(g_optionsRowTemplate, grid, index, firstId, endId, *holder, panel);
}



void Options_DrawChildRow(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element,
    std::int32_t index, UIOption *table, std::int32_t count, UIGridDrawCell rowDraw) {
    if (g_uiElements[kUiSubMenuOptions].isHandledExternally)
        return;
    UIOption *const option = Options_FindOption(index, table, count);
    if (!option)
        return;
    if (option->type != UIOptionType::Heading)
        UI_WindowMouseCallbacks(grid->mouseCallbacks, element, *cell, option);
    if (index < count && rowDraw)
        rowDraw(grid, panel, holder, cell, spacing, element, option, index);
}





void Options_SetUpChildGrid(UIGrid &grid, d3t::UIWindow *panel, std::int32_t rows, UIGridDrawCell drawCell, UIGridGetElement getElement, UIOption *table) {
    grid.columns = 1;
    grid.visibleRows = 8;
    grid.rows = rows;
    grid.showEmptyCells = 0;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = panel;
    grid.rect = {};
    grid.drawCell = drawCell;
    grid.mouseCallbacks = {.entered = Options_DescribeOption, .hovered = nullptr, .left = nullptr, .clicked = UI_ScriptNoOpCallback, .isHovered = 0};
    grid.getElement = getElement;
    grid.navigateSound = 3;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    grid.scrollSpeed = 0.25f;
    std::int32_t count = 0;
    for (std::int32_t entry = 0; entry < rows; ++entry) {
        if (!table[entry].available || table[entry].available())
            ++count;
    }
    grid.elementCount = count;
    grid.elements = table;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (count > grid.columns * (lastRow + 1) - 1)
        grid.rows = count / grid.columns + (count % grid.columns > 0 ? 1 : 0);
}



void Options_TickChild(std::int32_t elementIndex, UIGrid &grid) {
    if (g_uiElements[kUiSubMenuOptions].isHandledExternally)
        return;
    const UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(elementIndex)];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&grid, element.index);
}





std::int64_t Options_PressChild(UIGrid &grid, UIOption *table, std::int32_t count, UIRetry70ManBattlePromptCallback reset, bool controls) {
    if (g_uiElements[kUiSubMenuOptions].isHandledExternally)
        return 1;
    const std::int32_t row = UIGrid_SelectedIndex(grid);
    const UIOption *const capture = controls && g_optionsPendingAction != -1 ? Options_FindOption(g_optionsPendingAction, table, count) : nullptr;
    if (!capture || capture->type != UIOptionType::KeyBinding) {
        if ((g_inputState.slots[0].pressedPriority & 0x2U) != 0)
            KeepOrRevertSettingsInternal();
        if ((g_inputState.slots[0].pressedPriority & 0x4U) != 0)
            d3t::MessageBox("UIOPTSREVD", false, 1, reset, 0.0f); // @text EN: "Reset settings to default values?" | JA: "設定をデフォルトの値に戻しますか？"
    }
    if ((!controls || row > -1) && KeyboardMapping(row, table, count) == 1)
        return 0;
    const std::int64_t result = UIGrid_IsPressed(grid);
    Options_SkipHeading(grid, table, count);
    return result;
}

// Every option of a table is asked whether it shows (its answer unused), as the children's Renders do first.
void Options_AskAvailability(const UIOption *table, std::int32_t count) {
    for (std::int32_t entry = 0; entry < count; ++entry) {
        if (table[entry].available)
            table[entry].available();
    }
}

// The rows' choices and slider positions as each child's Render records them (never read).
void Options_RecordRows(const UIOption *table, std::int32_t count, std::uint64_t *recorded) {
    for (std::int32_t entry = 0; entry < count; ++entry)
        std::memcpy(&recorded[entry], &table[entry].current, sizeof(recorded[entry]));
}
}

namespace shenmue::s1 {


std::int64_t OptionsControls_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptionsControls].config.uiHandleIndex);
    g_optionsControlsPanel = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Controls::Panel_Root"));
    Options_SetUpChildGrid(
        g_optionsControlsGrid, g_optionsControlsPanel, 42,
        [](UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *,
            std::int32_t index) {
            Options_DrawChildRow(grid, panel, holder, cell, spacing, element, index, g_optionsControlsTable.data(), 42, g_optionsControlsRowDraw);
        },
        [](void *elements, std::int32_t index) -> void * { return elements ? Options_FindOption(index, g_optionsControlsTable.data(), 42) : nullptr; },
        g_optionsControlsTable.data());
    return 1;
}


std::int64_t OptionsControls_Reset() {
    g_optionsControlsPanel = nullptr;
    return 0;
}

std::int64_t OptionsControls_IsPressed() {
    return Options_PressChild(g_optionsControlsGrid, g_optionsControlsTable.data(), 42, ResetSettingsToDefaults, true);
}

std::int64_t OptionsControls_Tick() {
    Options_TickChild(kUiSubMenuOptionsControls, g_optionsControlsGrid);
    return 0;
}



void OptionsControls_Render() {
    UIOption *const table = g_optionsControlsTable.data();
    Options_AskAvailability(table, 42);
    std::int32_t count = 0;
    for (std::int32_t entry = 0; entry < 42; ++entry) {
        Options_ReadSettings(&table[entry], 1);
        if ((table[entry].available ? table[entry].available() : 1) == 1)
            ++count;
    }
    Options_RecordRows(table, 42, g_optionsControlsRecordedRows.data());
    UIGrid &grid = g_optionsControlsGrid;
    grid.elements = table;
    grid.elementCount = count;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (count > grid.columns * (lastRow + 1) - 1)
        grid.rows = count / grid.columns + (count % grid.columns > 0 ? 1 : 0);
    UIGrid_ResetRuntime(grid);
    Collection_ShowSelectedCell(grid, kUiSubMenuOptionsControls, table[0].text(table[0].current));
}



std::int64_t OptionsAudio_IsSelected() {
    AssignMultipleChoice();
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptionsAudio].config.uiHandleIndex);
    g_optionsAudioPanel = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Audio::Panel_Root"));
    Options_SetUpChildGrid(
        g_optionsAudioGrid, g_optionsAudioPanel, 7,
        [](UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *,
            std::int32_t index) {
            Options_DrawChildRow(grid, panel, holder, cell, spacing, element, index, g_optionsAudioTable.data(), 7, g_optionsAudioRowDraw);
        },
        [](void *elements, std::int32_t index) -> void * { return elements ? Options_FindOption(index, g_optionsAudioTable.data(), 7) : nullptr; },
        g_optionsAudioTable.data());
    Options_ReadAudioDevices();
    return 1;
}

std::int64_t OptionsAudio_Reset() {
    g_optionsAudioPanel = nullptr;
    return 0;
}

std::int64_t OptionsAudio_IsPressed() {
    return Options_PressChild(g_optionsAudioGrid, g_optionsAudioTable.data(), 7, ResetSettings, false);
}

std::int64_t OptionsAudio_Tick() {
    Options_TickChild(kUiSubMenuOptionsAudio, g_optionsAudioGrid);
    return 0;
}



void OptionsAudio_Render() {
    UIOption *const table = g_optionsAudioTable.data();
    Options_AskAvailability(table, 7);
    Options_ReadAudioDevices();
    Options_ReadSettings(table, 7);
    Options_RecordRows(table, 7, g_optionsAudioRecordedRows.data());
    UIGrid_ResetRuntime(g_optionsAudioGrid);
    Collection_ShowSelectedCell(g_optionsAudioGrid, kUiSubMenuOptionsAudio, table[0].text(-1));
}


std::int64_t OptionsGraphics_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptionsGraphics].config.uiHandleIndex);
    g_optionsGraphicsPanel = UI_GetWindow(UI_FindViewWindow(slot, "SubMenu_Graphics::Panel_Root"));
    Options_SetUpChildGrid(
        g_optionsGraphicsGrid, g_optionsGraphicsPanel, 11,
        [](UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element, void *,
            std::int32_t index) {
            Options_DrawChildRow(grid, panel, holder, cell, spacing, element, index, g_optionsGraphicsTable.data(), 11, g_optionsGraphicsRowDraw);
        },
        [](void *elements, std::int32_t index) -> void * { return elements ? Options_FindOption(index, g_optionsGraphicsTable.data(), 11) : nullptr; },
        g_optionsGraphicsTable.data());
    return 1;
}

std::int64_t OptionsGraphics_Reset() {
    g_optionsGraphicsPanel = nullptr;
    return 0;
}

std::int64_t OptionsGraphics_IsPressed() {
    return Options_PressChild(g_optionsGraphicsGrid, g_optionsGraphicsTable.data(), 11, RevertSettings, false);
}

std::int64_t OptionsGraphics_Tick() {
    Options_TickChild(kUiSubMenuOptionsGraphics, g_optionsGraphicsGrid);
    return 0;
}


void OptionsGraphics_Render() {
    UIOption *const table = g_optionsGraphicsTable.data();
    Options_AskAvailability(table, 11);
    Options_ReadSettings(table, 11);
    Options_RecordRows(table, 11, g_optionsGraphicsRecordedRows.data());
    UIGrid_ResetRuntime(g_optionsGraphicsGrid);
    Collection_ShowSelectedCell(g_optionsGraphicsGrid, kUiSubMenuOptionsGraphics, table[0].text(table[0].current));
}
}

namespace shenmue::s1::ui_runtime_detail {




void Options_SwitchTab() {
    UIFocusList &list = g_optionsTabList;
    if (kOptionsChildren[static_cast<std::size_t>(list.selected)] == -1) {
        list.selected = list.shownIndex;
        if (g_isInGame)
            d3t::MessageBox("UIQUITLAB2", false, 0, DoYouWantToQuit_UnsavedChangesLost, 0.0f); // @text EN: "Are you sure you want to quit?\nAny unsaved progress will be lost." | JA: "タイトル画面に戻ります。よろしいですか？\n（記録されていないゲームデータは失われます）"
        return;
    }
    if (list.shownIndex == list.selected)
        return;
    if (static_cast<std::uint32_t>(list.shownIndex) < 4U && !d3t::ResetElementByIdx(kOptionsChildren[static_cast<std::size_t>(list.shownIndex)]))
        return;
    list.shownIndex = list.selected;
    if (static_cast<std::uint32_t>(list.selected) < 4U)
        d3t::ActivateElementByIdx(kOptionsChildren[static_cast<std::size_t>(list.selected)]);
    for (std::size_t tab = 0; tab < 4; ++tab) {
        const bool shown = list.shownIndex == static_cast<std::int32_t>(tab);
        if (UIColourImageWidget *const icon = g_optionsColourImages[tab])
            Options_SetAlphaByte(icon->colour, static_cast<std::uint32_t>(static_cast<std::int32_t>((shown ? 1.0f : 0.6f) * 255.0f)));
        if (UIButtonWidget *const button = g_optionsButtons[15 + tab])
            button->colour = shown ? g_optionsTabButtonSelectedColour : g_optionsTabButtonColour;
        const std::size_t background = static_cast<std::size_t>(g_optionsTabBackgrounds) + tab;
        if (UIColourImageWidget *const image = g_optionsColourImages[background])
            image->colour = shown ? g_optionsTabBackgroundSelectedColour : g_optionsTabBackgroundColour;
    }
}




void Options_OnTabHovered(float *, void *data) {
    UIFocusList &list = g_optionsTabList;
    for (std::int32_t tab = 0; tab <= list.last - list.first; ++tab) {
        if (data != list.windows[tab])
            continue;
        list.lastFocused = -1;
        list.focused = tab;
        PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
    }
}

void Options_OnTabHoverLeave(float *, void *data) {
    UIFocusList &list = g_optionsTabList;
    for (std::int32_t tab = 0; tab <= list.last - list.first; ++tab) {
        if (data == list.windows[tab] && list.focused == tab) {
            list.focused = -1;
            UIFocusList_ColourLabels(list);
        }
    }
}

void Options_OnTabClicked(float *, void *data) {
    UIFocusList &list = g_optionsTabList;
    for (std::int32_t tab = 0; tab <= list.last - list.first; ++tab) {
        if (data != list.windows[tab])
            continue;
        list.selected = tab;
        PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
    }
}



void UI_ResetLabelScaleVariables(std::uint32_t slot) {
    if (slot > 75U)
        return;
    auto &variables = g_uiViewSlots[slot].variables;
    for (const d3t::UILabelWidget &label : g_uiLabelWidgets) {
        if (static_cast<std::uint32_t>(label.scaleVar) >= variables.size())
            continue;
        d3t::UIViewVariable &variable = variables[static_cast<std::size_t>(label.scaleVar)];
        if (variable.type == d3t::UIViewVariableType::Float)
            variable.value.floatValue = 1.0f;
    }
}
}

namespace shenmue::s1 {







std::int64_t OptionsMenu_IsSelected() {
    g_optionsControlsRowDraw = OptionsRow_Draw;
    g_optionsAudioRowDraw = OptionsRow_Draw;
    g_optionsGraphicsRowDraw = OptionsRow_Draw;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSubMenuOptions].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_optionsWindows.size(); ++window) {
        d3t::UIWindow *const record = UI_GetWindow(UI_FindViewWindow(slot, kOptionsWindowIds[window]));
        g_optionsWindows[window] = record;
        if (record && window < 4) {
            record->mouseCallbacks.entered = Options_OnTabHovered;
            record->mouseCallbacks.left = Options_OnTabHoverLeave;
            record->mouseCallbacks.clicked = Options_OnTabClicked;
        }
    }
    for (std::size_t label = 0; label < g_optionsLabels.size(); ++label)
        g_optionsLabels[label] = UI_LabelRecord(UI_FindViewLabel(slot, kOptionsLabelIds[label]));
    if (d3t::UILabelWidget *const percent = g_optionsLabels[6]) {
        std::snprintf(g_optionsSliderText.data(), g_optionsSliderText.size(), "%s", percent->text ? percent->text : "");
        percent->text = g_optionsSliderText.data();
    }
    for (std::size_t item = 0; item < g_optionsLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kOptionsLayoutItemIds[item]);
        g_optionsLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    for (std::size_t image = 0; image < g_optionsColourImages.size(); ++image)
        g_optionsColourImages[image] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kOptionsColourImageIds[image]));
    for (std::size_t button = 0; button < g_optionsButtons.size(); ++button) {
        const std::uint32_t index = static_cast<std::uint32_t>(UI_FindViewButton(slot, kOptionsButtonIds[button]));
        g_optionsButtons[button] = index < g_uiButtonWidgets.size() ? &g_uiButtonWidgets[index] : nullptr;
    }
    g_optionsKeyBindingKey = UI_ButtonIconRecord(UI_FindViewIconWidget(21, slot, "SubMenu_Options::KeyBinding_Key"));
    UIFocusList &list = g_optionsTabList;
    UIGridTemplate &rowTemplate = g_optionsRowTemplate;
    list.labels = &g_optionsLabels[9];
    g_optionsTabButtonColour = kUIOptionsFrontEndColours.tabButton;
    g_optionsTabButtonSelectedColour = kUIOptionsFrontEndColours.tabButtonShown;
    g_optionsTabBackgroundColour = kUIOptionsFrontEndColours.tabBackground;
    g_optionsTabBackgroundSelectedColour = kUIOptionsFrontEndColours.tabBackgroundShown;
    list.windows = g_optionsWindows.data();
    list.first = 0;
    rowTemplate.initialized = 0;
    list.last = 3;
    rowTemplate.viewSlot = static_cast<std::int32_t>(slot);
    rowTemplate.ids = kOptionsWindowIds.data();
    rowTemplate.layoutItems = g_optionsLayoutItems.data();
    rowTemplate.windows = g_optionsWindows.data();
    rowTemplate.labels = g_optionsLabels.data();
    rowTemplate.layoutFirst = 0;
    rowTemplate.labelFirst = 0;
    rowTemplate.first = 8;
    rowTemplate.end = 41;
    rowTemplate.clipWindows[0] = 12;
    rowTemplate.clipWindows[1] = 22;
    rowTemplate.clipWindows[2] = 26;
    rowTemplate.clipWindows[3] = 33;
    rowTemplate.clipWindows[4] = -1;
    rowTemplate.layoutEnd = 47;
    rowTemplate.labelEnd = 9;
    for (const std::size_t index : {9U, 0U, 1U, 2U, 3U, 12U, 22U, 32U, 38U})
        g_frontendEmblemImages[static_cast<std::size_t>(g_frontendEmblemImageCount++)] = g_optionsColourImages[index];
    return 1;
}





std::int64_t OptionsMenu_GetContentPanel() {
    const bool inGame = g_isInGame != 0;
    const UIOptionsColours &colours = inGame ? kUIOptionsInGameColours : kUIOptionsFrontEndColours;
    g_optionsTabButtonColour = colours.tabButton;
    g_optionsTabButtonSelectedColour = colours.tabButtonShown;
    g_optionsTabBackgroundColour = colours.tabBackground;
    g_optionsTabBackgroundSelectedColour = colours.tabBackgroundShown;
    g_optionsTabBackgrounds = inGame ? 8 : 4;
    if (d3t::UIWindow *const root = g_optionsWindows[7])
        std::snprintf(root->layout.parentId.data(), root->layout.parentId.size(), "%s", inGame ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
    for (const std::size_t hidden : inGame ? std::array<std::size_t, 3>{4, 5, 6} : std::array<std::size_t, 3>{8, 9, 10}) {
        if (UIColourImageWidget *const image = g_optionsColourImages[hidden])
            Options_SetAlphaByte(image->colour, 0);
    }
    for (const std::size_t arrow : {32U, 33U}) {
        if (UIColourImageWidget *const image = g_optionsColourImages[arrow])
            image->colour = colours.arrow;
    }
    const auto colourButton = [](std::size_t button, std::uint32_t colour) {
        if (UIButtonWidget *const widget = g_optionsButtons[button])
            widget->colour = colour;
    };
    colourButton(0, colours.mainPanelButton);
    colourButton(1, colours.rowBackground);
    colourButton(2, colours.rowControl);
    colourButton(3, colours.rowControl);
    colourButton(5, colours.rowBackground);
    colourButton(6, colours.rowControl);
    colourButton(8, colours.rowBackground);
    colourButton(10, colours.scrollBar);
    colourButton(9, colours.scrollBar);
    colourButton(12, colours.rowBackground);
    colourButton(13, colours.rowControl);
    for (const std::int32_t image : kOptionsFrontEndImages) {
        if (UIColourImageWidget *const widget = g_optionsColourImages[static_cast<std::size_t>(image)])
            widget->alpha = inGame ? 0.0f : 1.0f;
    }
    for (const std::int32_t image : kOptionsInGameImages) {
        if (UIColourImageWidget *const widget = g_optionsColourImages[static_cast<std::size_t>(image)])
            widget->alpha = inGame ? 1.0f : 0.0f;
    }
    for (const std::size_t window : {3U, 6U, 4U, 5U}) {
        if (d3t::UIWindow *const quit = g_optionsWindows[window])
            quit->alpha = inGame ? 1.0f : 0.0f;
    }
    g_optionsTabList.last = inGame ? 3 : 2;
    return 0;
}


std::int64_t OptionsMenu_Reset() {
    g_optionsWindows = {};
    g_optionsLabels = {};
    g_optionsColourImages = {};
    return 0;
}



std::int64_t OptionsMenu_Tick() {
    g_currUIState = d3t::CheckAllKeysAreBound() ? kUiSubMenuOptionsControls : -1;
    const auto loadEnglish = [] {
        if (g_uiLoadedLanguage != -1)
            return;
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = English;
        ReadSubtitleGlyphsAndSubs(English);
    };
    loadEnglish();
    const UIBaseRuntime &element = g_uiElements[kUiSubMenuOptions];
    if (g_optionsShownLanguage != g_uiLoadedLanguage) {
        UI_ResetLabelScaleVariables(static_cast<std::uint32_t>(element.config.uiHandleIndex));
        loadEnglish();
        g_optionsShownLanguage = g_uiLoadedLanguage;
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}



void OptionsMenu_Render() {
    g_optionsClosePending = 0;
    g_optionsInputAction = nullptr;
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    UI_ActivateElement(informationBar);
    informationBar.isHandledExternally = 0;
    UIFocusList &list = g_optionsTabList;
    g_uiFocusedList = &list;
    g_optionsPendingAction = -1;
    list.focused = 0;
    list.lastFocused = 0;
    list.initialized = 0;
    list.hasFocus = 0;
    g_uiGridInputEnabled = 1;
    UI_RestartCurrentGridSelection();
    list.shownIndex = -1;
    Options_SwitchTab();
    UpdateSettings();
}




std::int64_t OptionsMenu_IsPressed() {
    if (g_optionsClosePending) {
        if (!g_isInGame) {
            UpdateStatByIdx(g_mainMenuStatIndex, 0);
            return 1;
        }
        if (g_easyOptionState.state != 0)
            ESOP_CloseOptionsMenu();
        g_optionsClosePending = 0;
    }
    UIFocusList &list = g_optionsTabList;
    UIFocusList_HandleInput(list);
    if (!g_isInGame && list.selected > list.last)
        list.selected = list.first;
    Options_SwitchTab();
    return 1;
}


std::int64_t OptionsMenu_Destroy() {
    if (Options_SettingsChanged())
        d3t::WriteSettingsFile();
    const bool closed = d3t::ResetElementByIdx(kOptionsChildren[static_cast<std::size_t>(g_optionsTabList.selected)]);
    if (!closed)
        g_optionsClosePending = 1;
    return closed ? 1 : 0;
}



std::int64_t OptionsMenu_GetPromptIconSet() {
    const std::int16_t apply = Options_SettingsChanged() && d3t::CheckAllKeysAreBound() != 1 ? 3 : -1;
    const std::array<std::int16_t, 5> prompts{45, 44, 2, apply, -1};
    g_searchPromptIconSlots = prompts;
    for (std::size_t slot = 0; slot < g_optionsPromptIconSet.size(); ++slot)
        g_optionsPromptIconSet[slot] = static_cast<std::uint16_t>(prompts[slot]);
    return reinterpret_cast<std::int64_t>(g_optionsPromptIconSet.data());
}
}

namespace shenmue::s1::ui_runtime_detail {






void FrontendAutomation_OptionsTick() {
    const UIBaseRuntime &options = g_uiElements[kUiSubMenuOptions];
    UIFocusList &list = g_optionsTabList;
    switch (g_frontendAutomationStage) {
    case FrontendAutomationStage::Wheel:
        if (g_mainMenuStatIndex == 2)
            SetFrontendAutomationStage(FrontendAutomationStage::Options, "options-opened-item-3");
        return;
    case FrontendAutomationStage::Options:
        if (g_frontendOptionsHold)
            return;
        if (!options.isVisible || options.config.init != 3) {
            QueueFrontendAutomationInput(0);
            return;
        }
        if (!CaptureFrontendOptionsSettingsFile()) {
            SetFrontendAutomationStage(FrontendAutomationStage::Failed);
            return;
        }
        g_frontendOptionsStep = 0;
        SetFrontendAutomationStage(FrontendAutomationStage::OptionsTab, "options-shown");
        return;
    case FrontendAutomationStage::OptionsClose:
        if (!options.isVisible && g_mainMenuStatIndex == 0 && g_uiElements[kMainMenuWheelElement].config.init == 3)
            SetFrontendAutomationStage(FrontendAutomationStage::Complete, "options-closed");
        else
            QueueFrontendAutomationInput(options.isVisible ? 0x200U : 0U);
        return;
    case FrontendAutomationStage::OptionsTab:
    case FrontendAutomationStage::OptionsRow:
    case FrontendAutomationStage::OptionsEdit:
    case FrontendAutomationStage::OptionsStep:
    case FrontendAutomationStage::OptionsEndEdit:
    case FrontendAutomationStage::OptionsKeep:
        break;
    default:
        return;
    }
    if (g_messageBoxState.rendered) {
        QueueFrontendAutomationInput(g_messageBoxState.confirmed ? 0U : g_messageBoxSelection != 2 ? 0x4000U : 0x400U);
        return;
    }
    const FrontendOptionsEdit &edit = kFrontendOptionsEdits[static_cast<std::size_t>(g_frontendOptionsStep / 2)];
    const bool restoring = (g_frontendOptionsStep & 1) != 0;
    const UIOption &option = edit.table[edit.entry];
    const float value = option.type == UIOptionType::Slider ? option.value : static_cast<float>(option.current);
    const UIBaseRuntime &child = g_uiElements[static_cast<std::size_t>(kOptionsChildren[static_cast<std::size_t>(edit.tab)])];
    switch (g_frontendAutomationStage) {
    case FrontendAutomationStage::OptionsTab:
        if (list.hasFocus)
            QueueFrontendAutomationInput(list.focused == edit.tab ? 0x400U : 0x2000U);
        else if (list.shownIndex != edit.tab)
            QueueFrontendAutomationInput(0x4000U);
        else if (child.isVisible && child.config.init == 3)
            SetFrontendAutomationStage(FrontendAutomationStage::OptionsRow);
        else
            QueueFrontendAutomationInput(0);
        break;
    case FrontendAutomationStage::OptionsRow: {
        const std::int32_t row = UIGrid_SelectedIndex(*edit.grid);
        const UIOption *const selected = row >= 0 ? Options_FindOption(row, edit.table, edit.count) : nullptr;
        if (selected == &option) {
            g_frontendOptionsRow = row;
            SetFrontendAutomationStage(FrontendAutomationStage::OptionsEdit);
        } else {
            QueueFrontendAutomationInput(!selected ? 0U : selected < &option ? 0x2000U : 0x1000U);
        }
        break;
    }
    case FrontendAutomationStage::OptionsEdit:
        if (g_optionsPendingAction != g_frontendOptionsRow) {
            QueueFrontendAutomationInput(0x400U);
            break;
        }
        if (!restoring)
            g_frontendOptionsOriginal = value;
        g_frontendOptionsStart = value;
        SetFrontendAutomationStage(FrontendAutomationStage::OptionsStep);
        break;
    case FrontendAutomationStage::OptionsStep:
        if (restoring ? std::fabs(value - g_frontendOptionsOriginal) >= 0.0001f : value == g_frontendOptionsOriginal) {
            QueueFrontendAutomationInput((g_frontendOptionsOriginal < 1.0f) != restoring ? 0x8000U : 0x4000U);
            break;
        }
        SetFrontendAutomationStage(FrontendAutomationStage::OptionsEndEdit);
        break;
    case FrontendAutomationStage::OptionsEndEdit:
        if (g_optionsPendingAction == -1)
            SetFrontendAutomationStage(FrontendAutomationStage::OptionsKeep);
        else
            QueueFrontendAutomationInput(0x400U);
        break;
    case FrontendAutomationStage::OptionsKeep: {
        if (Options_SettingsChanged()) {
            QueueFrontendAutomationInput(0x2U);
            break;
        }
        std::ifstream input("Settings", std::ios::binary);
        const std::vector<char> bytes{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
        input.close();
        if (++g_frontendOptionsStep < static_cast<std::int32_t>(kFrontendOptionsEdits.size() * 2)) {
            SetFrontendAutomationStage(FrontendAutomationStage::OptionsTab);
        } else if (!RestoreFrontendOptionsSettingsFile()) {
            SetFrontendAutomationStage(FrontendAutomationStage::Failed);
        } else {
            SetFrontendAutomationStage(FrontendAutomationStage::OptionsClose);
        }
        break;
    }
    default:
        break;
    }
}
}

namespace shenmue::s1 {


void ReturnToTitleScreen() {
    g_busy_reset = 1;
    for (UIBaseRuntime &element : g_uiElements) {
        if (element.isVisible && element.destroy)
            static_cast<void>(element.destroy());
        element.isVisible = 0;
        element.isActive = 0;
        element.isHandledExternally = 0;
    }
    UI_GUITasksShutdown();
    g_currUIState = -1;
    g_controllerHelpOverlayState = 0;
    g_busy_reset = 0;
    PressStart_SelectLogo();
    g_mainMenuOpened = 0;

    PAUS_ResetRuntime();
    SCNT_ClearSubtitleText();
    g_ClearColour = 0xff000000U;
    g_shouldUseTemporaryClearColour = false;
    g_temporaryClearColour = {.red = 0xff, .green = 0, .blue = 0, .alpha = 0};
    shenInitDefaultStats();
    SCRL_SetLuminanceEnabled(false);
    SCRL_SetLuminanceChannels(0, 0, 0);
    CVWK_Initialize();
    HLib_UpdateTaskQueue(SystemModule::Display);
    g_xinputReserved = 0;
    g_harrierExitedEmulator = 1;
    g_harrierShouldReturn = 1;
    HangOnDestroy();
}

std::int64_t LoadSplash() {
    UIBaseRuntime &splash = g_uiElements[0x3b];
    if (!splash.setup) {
        splash.setup = 1;
        splash.someFlag3 = 0;
        constexpr char path[] = "/ui/Splash/Splash.ui";
        std::memcpy(splash.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, splash);
        splash.config.screenWidth = static_cast<std::int16_t>(handle);
        splash.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        splash.config.init = 1;
    }
    return splash.setup;
}
}
