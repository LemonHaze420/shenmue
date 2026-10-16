#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {



void GameTraining_SetResultText(std::int32_t focus, std::int32_t move) {
    std::array<char, 512> &text = g_gameTrainingElement.resultText;
    if (move == -1) {
        constexpr std::array<const char *, 5> kResultIds{nullptr, "UITRNSETAL", "UITRNSETHN", "UITRNSETLG",
            "UITRNSETTH"}; // @text UITRNSETAL EN: "Train All Moves Equally" | JA: "全技をまんべんなく鍛錬します。" ; UITRNSETHN EN: "Train Hand Moves Only" | JA: "手技のみ鍛錬します。" ; UITRNSETLG EN: "Train Leg Moves Only" | JA: "足技のみ鍛錬します。" ; UITRNSETTH EN: "Train Throw Moves Only" | JA: "投技のみ鍛錬します。"
        if (focus >= 1 && focus <= 4)
            std::snprintf(text.data(), text.size(), "%s", InitAndRetrieveStringFromID(kResultIds[static_cast<std::size_t>(focus)]));
    } else {
        std::array<char, 512> name{};
        FightMoveInfo info{};
        if (GetFightAnimInfos(move, name.data(), &info) != 1)
            return;
        std::snprintf(text.data(), text.size(), "%s",
            InitAndRetrieveStringFromID(
                "UITRNSETMV")); // @text EN: "[[MOVE_NAME]] will be focused from now on." | JA: "今後は[[MOVE_NAME]]を中心に鍛錬します。"
        TextFormat(text.data(), static_cast<std::int32_t>(text.size()), "[[MOVE_NAME]]", name.data());
    }
    if (g_gameTrainingResultLabel)
        g_gameTrainingResultLabel->text = text.data();
}



void CompleteMenuInteraction(std::int32_t move) {
    GameTrainingElement &training = g_gameTrainingElement;
    if (SetTrainingFocusMoveSet(training.focus, move) != 1)
        return;
    g_inputState.slots[0].pressedPriority = 0;
    if (((static_cast<std::uint32_t>(g_currUIState) + 1U) & ~8U) == 0) {
        UIBaseRuntime &background = g_uiElements[kUiGameMenuBackground];
        if (!background.isVisible || !background.destroy || background.destroy() == 1 || g_busy_reset == 1)
            background.isVisible = 0;
    }
    UIBaseRuntime &moves = g_uiElements[kUiSubMenuMoves];
    if ((g_currUIState == -1 || g_currUIState == kUiSubMenuMoves) && (!moves.isVisible || !moves.destroy || moves.destroy() == 1 || g_busy_reset == 1))
        moves.isVisible = 0;
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    training.state = 2;
    GameTraining_SetResultText(training.focus, move);
    PlayGatedUiSound(g_trainingMovePickSoundFrame, 0x208abU);
}




void GameTraining_UpdateLabelImages() {
    GameTrainingElement &training = g_gameTrainingElement;
    if (g_uiLoadedLanguage == -1)
        ReadSubtitleGlyphsAndSubs(English);
    constexpr std::array<std::int32_t, 7> kLanguageOffsets{0, 72, 12, 24, 36, 48, 60};
    if (g_uiLoadedLanguage >= 0 && g_uiLoadedLanguage < static_cast<std::int32_t>(kLanguageOffsets.size()))
        training.languageImageOffset = kLanguageOffsets[static_cast<std::size_t>(g_uiLoadedLanguage)];
    constexpr std::array<std::array<std::int32_t, 5>, 4> kFocusImages{{{8, 4, 1, 3, 2}, {9, 0, 5, 3, 2}, {10, 0, 1, 3, 6}, {11, 0, 1, 7, 2}}};
    if (training.focus < 1 || training.focus > 4)
        return;
    for (std::size_t index = 0; index < g_gameTrainingLabelWidgets.size(); ++index) {
        if (d3t::UIImageWidget *const widget = g_gameTrainingLabelWidgets[index]) {
            const std::int32_t entry = kFocusImages[static_cast<std::size_t>(training.focus - 1)][index];
            widget->image = g_gameTrainingLabelImageIndices[static_cast<std::size_t>(entry)] + training.languageImageOffset;
        }
    }
}


void GameTraining_SetPanelAlpha(float picker, float result) {
    for (std::size_t index = 3; index < g_gameTrainingWindows.size(); ++index) {
        if (g_gameTrainingWindows[index])
            g_gameTrainingWindows[index]->alpha = picker;
    }
    for (std::size_t index = 0; index < 3; ++index) {
        if (g_gameTrainingWindows[index])
            g_gameTrainingWindows[index]->alpha = result;
    }
}


void GameTraining_CloseMoveList() {
    InputSlot &input = g_inputState.slots[0];
    if ((input.pressedPriority & 0x200U) == 0)
        return;
    input.pressedPriority = 0;
    d3t::ResetElementByIdx(kUiGameMenuBackground);
    d3t::ResetElementByIdx(kUiSubMenuMoves);
    d3t::ResetElementByIdx(kUiGameMenuInformationBar);
    g_gameTrainingElement.state = 0;
}

// HandleGameTrainingResult_0: the "train specific moves?" answer. Anything but Yes commits the whole focused set. Yes opens the Moves
// list over the Game Background with the Information Bar, lists the focused set's moves (every move for the whole-set focus) and makes the
// list's A pick the move (CompleteMenuInteraction).
void GameTraining_HandleSpecificMovesPrompt(UIRetry70ManBattlePromptResult result) {
    GameTrainingElement &training = g_gameTrainingElement;
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (result != UIRetry70ManBattlePromptResult::Yes) {
        training.state = 2;
        GameTraining_SetResultText(training.focus, -1);
        SetTrainingFocusMoveSet(training.focus, -1);
        if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
            informationBar.isVisible = 0;
        return;
    }
    training.state = 1;
    if (((static_cast<std::uint32_t>(g_currUIState) + 1U) & ~8U) == 0)
        UI_ActivateElement(g_uiElements[kUiGameMenuBackground]);
    if (g_currUIState == -1 || g_currUIState == kUiSubMenuMoves)
        UI_ActivateElement(g_uiElements[kUiSubMenuMoves]);
    UI_ActivateElement(informationBar);
    if (training.focus == 1)
        UpdateWidget();
    else if (training.focus >= 2 && training.focus <= 4)
        UpdateState(training.focus - 1);
    g_movesScreen.adjustFightMove = CompleteMenuInteraction;
}

// ChangeFocusMoveType: the directions pick the focused set (up all moves, left hand, right leg, down throw), A commits all moves or
// asks about specific moves of the set, B closes the element; X opens the focused set's help page (31 to 34, SetHelpDetailsByIndex).
void GameTraining_ChangeFocusMoveType() {
    GameTrainingElement &training = g_gameTrainingElement;
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if ((pressed & 0xf000U) != 0) {
        const std::int32_t previous = training.focus;
        std::int32_t focus = 1;
        if ((pressed & 0x8000U) != 0)
            focus = 3;
        else if ((pressed & 0x4000U) != 0)
            focus = 2;
        else if ((pressed & 0x2000U) != 0)
            focus = 4;
        training.focus = focus;
        if (focus != previous)
            UI_PlaySound(6);
        std::snprintf(training.focusTitle.data(), training.focusTitle.size(), "%s",
            InitAndRetrieveStringFromID(kGameTrainingFocusTitles[static_cast<std::size_t>(focus)]));
    }
    if ((pressed & 0x400U) != 0) {
        if (training.focus == 1) {
            training.state = 2;
            SetTrainingFocusMoveSet(1, -1);
            GameTraining_SetResultText(training.focus, -1);
            UI_PlaySound(10);
        } else if (training.focus >= 2 && training.focus <= 4) {
            const char *const specific = InitAndRetrieveStringFromID("UIWAZASPEC"); // @text EN: "specific moves?" | JA: "鍛錬しますか？"
            const char *const wish = InitAndRetrieveStringFromID("UIWAZAWISH");     // @text EN: "Wish to train" | JA: "特定の技を"
            std::array<char, 512> message{};
            std::snprintf(message.data(), message.size(), "%s %s", wish, specific);
            d3t::MessageBox(message.data(), true, 1, GameTraining_HandleSpecificMovesPrompt, 0.0f);
        }
    } else if ((pressed & 0x200U) != 0) {
        d3t::ResetElementByIdx(kUiGameTraining);
        UI_PlaySound(9);
    }
    if ((g_inputState.slots[0].pressedPriority & 4U) != 0 && training.focus >= 1 && training.focus <= 4)
        SetHelpDetailsByIndex(30 + training.focus);
}



void GameTraining_HoverFocus(float *, void *data) {
    GameTrainingElement &training = g_gameTrainingElement;
    if (data == g_gameTrainingWindows[7])
        training.focus = 1;
    else if (data == g_gameTrainingWindows[8])
        training.focus = 2;
    else if (data == g_gameTrainingWindows[9])
        training.focus = 4;
    else if (data == g_gameTrainingWindows[10])
        training.focus = 3;
    std::snprintf(training.focusTitle.data(), training.focusTitle.size(), "%s",
        InitAndRetrieveStringFromID(kGameTrainingFocusTitles[static_cast<std::size_t>(training.focus)]));
    UI_PlaySound(6);
}



void GameTraining_ClickFocus(float *, void *) {
    GameTrainingElement &training = g_gameTrainingElement;
    if (training.focus == 1) {
        training.state = 2;
        GameTraining_SetResultText(1, -1);
        SetTrainingFocusMoveSet(training.focus, -1);
        UI_PlaySound(10);
    } else if (static_cast<std::uint32_t>(training.focus - 2) <= 2U) {
        const char *const specific = InitAndRetrieveStringFromID("UIWAZASPEC"); // @text EN: "specific moves?" | JA: "鍛錬しますか？"
        const char *const wish = InitAndRetrieveStringFromID("UIWAZAWISH");     // @text EN: "Wish to train" | JA: "特定の技を"
        std::array<char, 512> message{};
        std::snprintf(message.data(), message.size(), "%s %s", wish, specific);
        d3t::MessageBox(message.data(), true, 1, GameTraining_HandleSpecificMovesPrompt, 0.0f);
    }
}




std::int64_t GameTraining_IsSelected() {
    GameTrainingElement &training = g_gameTrainingElement;
    training.focus = 0;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameTraining].config.uiHandleIndex);
    const std::int32_t result = UI_FindViewLabel(slot, "GameTraining::ResultLabel");
    g_gameTrainingResultLabel = static_cast<std::uint32_t>(result) <= 0x1ffU ? &g_uiLabelWidgets[static_cast<std::size_t>(result)] : nullptr;
    for (std::size_t index = 0; index < kGameTrainingLabelIds.size(); ++index) {
        const std::int32_t widget = UI_FindViewImageWidget(slot, kGameTrainingLabelIds[index]);
        g_gameTrainingLabelWidgets[index] = static_cast<std::uint32_t>(widget) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(widget)] : nullptr;
    }
    for (std::size_t index = 0; index < g_gameTrainingWindows.size(); ++index) {
        const char *const id = index < kGameTrainingResultPanels.size() ? kGameTrainingResultPanels[index] : kGameTrainingPickerPanels[index - 3];
        d3t::UIWindow *const window = UI_GetWindow(UI_FindViewWindow(slot, id));
        g_gameTrainingWindows[index] = window;
        if (window && index >= 7) {
            window->mouseCallbacks.entered = GameTraining_HoverFocus;
            window->mouseCallbacks.clicked = GameTraining_ClickFocus;
        }
    }
    d3t::UIWindow *const rootPanel = g_gameTrainingWindows[3];
    UILayoutRect rect{};
    if (rootPanel)
        UI_GetWindowRect(rootPanel, rect);
    training.rootPanelStart = (rect.x - (rect.x + 1920.0f)) / 1920.0f;
    training.animSlot = UIAnimAllocSlot();
    if (rootPanel)
        rootPanel->layout.offsetX = training.rootPanelStart;
    if (slot <= 0x4bU) {
        const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &view = g_uiViewSlots[slot];
        std::size_t entry = 0;
        for (std::size_t image = 0; image < view.images.size() && entry < kGameTrainingLabelImages.size(); ++image) {
            if (view.images[image].name && !std::strcmp(view.images[image].name, kGameTrainingLabelImages[entry]))
                g_gameTrainingLabelImageIndices[entry++] = static_cast<std::int32_t>(image);
        }
    }
    return 1;
}

// STATIC_Game_Training_Render: the element's activation. It frames Ryo for the picker (shenExitSearchAndMoveCameraRelative) and slides
// the RootPanel in over half a second.
void GameTraining_Render() {
    GameTrainingElement &training = g_gameTrainingElement;
    training.state = 0;
    training.focus = 1;
    std::snprintf(training.focusTitle.data(), training.focusTitle.size(), "%s", InitAndRetrieveStringFromID(kGameTrainingFocusTitles[1]));
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto *const ryo = control ? static_cast<const Character *>(control->ryo) : nullptr;
    const std::int32_t yaw = ryo ? ryo->entity.node.rotation.y : 0;
    ExitSearchAndMoveCameraRelative(1.6f, 0.17f, static_cast<std::uint16_t>(-yaw));
    if (training.animSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(training.animSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.start[0] = 0.0f;
        slot.current[0] = 0.0f;
        slot.end[0] = 1.0f;
        slot.delay = 0.0f;
        slot.duration = 0.5f;
        slot.reserved40 = -1;
    }
    UI_PlaySound(8);
}

// STATIC_Game_Training_IsPressed: input waits while the Yes/No prompt is up.
std::int64_t GameTraining_IsPressed() {
    if (g_uiElements[kUiYesNoPrompt].isVisible)
        return 1;
    GameTrainingElement &training = g_gameTrainingElement;
    if (training.state == 0) {
        GameTraining_ChangeFocusMoveType();
    } else if (training.state == 1) {
        GameTraining_CloseMoveList();
    } else if (training.state == 2 && (g_inputState.slots[0].pressedPriority & 0x400U) != 0) {
        training.state = 0;
        d3t::ResetElementByIdx(kUiGameTraining);
    }
    return 1;
}



std::int64_t GameTraining_Tick() {
    GameTrainingElement &training = g_gameTrainingElement;
    if (d3t::UIWindow *const rootPanel = g_gameTrainingWindows[3])
        rootPanel->layout.offsetX = (1.0f - g_uiAnimSlots[static_cast<std::size_t>(training.animSlot)].current[0]) * training.rootPanelStart;
    GameTraining_UpdateLabelImages();
    if (training.state == 0)
        GameTraining_SetPanelAlpha(1.0f, 0.0f);
    else if (training.state == 1)
        GameTraining_SetPanelAlpha(0.0f, 0.0f);
    else if (training.state == 2)
        GameTraining_SetPanelAlpha(0.0f, 1.0f);
    const UIBaseRuntime &element = g_uiElements[kUiGameTraining];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}

// au_re_TranslatesCamera, the element's Destroy: restore the camera (srchSetToFree) and hand control back to the bedroom menu.
std::int64_t GameTraining_Destroy() {
    ResetSearchToFree();
    g_sleepSaveTrainingMenu.requested = 1;
    g_sleepSaveTrainingMenu.scriptResult = 0;
    return 1;
}



void GameTrainingLegacy_BindWidgets() {
    const std::int32_t slot = g_gameTrainingLegacyUIViewSlot;
    if (static_cast<std::uint32_t>(slot) > 0x4bU) {
        g_gameTrainingLegacyView = nullptr;
        return;
    }
    g_gameTrainingLegacyView = &g_uiViewSlots[static_cast<std::size_t>(slot)];
    const std::uint32_t view = static_cast<std::uint32_t>(slot);
    for (std::size_t set = 0; set < kGameTrainingLegacySets.size(); ++set) {
        const char *const name = kGameTrainingLegacySets[set];
        std::array<char, 24> id{};
        g_gameTrainingLegacyLabels[2 * set] = UI_LabelRecord(UI_FindViewLabel(view, name));
        std::snprintf(id.data(), id.size(), "%s_%s", name, "Moves");
        g_gameTrainingLegacyLabels[2 * set + 1] = UI_LabelRecord(UI_FindViewLabel(view, id.data()));
        std::snprintf(id.data(), id.size(), "%s_%s", name, "Arrow");
        g_gameTrainingLegacyArrows[2 * set] = UI_ColourImageRecord(UI_FindViewColourImage(view, id.data()));
        std::snprintf(id.data(), id.size(), "%s_%s_%s", name, "Arrow", "Shadow");
        g_gameTrainingLegacyArrows[2 * set + 1] = UI_ColourImageRecord(UI_FindViewColourImage(view, id.data()));
        std::snprintf(id.data(), id.size(), "%s_%s", name, "Glow");
        g_gameTrainingLegacyGlows[set] = UI_ColourImageRecord(UI_FindViewColourImage(view, id.data()));
    }
    g_gameTrainingLegacyDescriptions[0] = UI_LabelRecord(UI_FindViewLabel(view, "Description0"));
    g_gameTrainingLegacyDescriptions[1] = UI_LabelRecord(UI_FindViewLabel(view, "Description1"));
}



void GameTrainingLegacy_SelectFirst() {
    if (g_gameTrainingLegacySelection != -1) {
        const std::size_t set = static_cast<std::size_t>(g_gameTrainingLegacySelection);
        for (d3t::UILabelWidget *const label : {g_gameTrainingLegacyLabels[2 * set], g_gameTrainingLegacyLabels[2 * set + 1]}) {
            if (label)
                label->colour = 0x40ffffffU;
        }
        if (g_gameTrainingLegacyArrows[2 * set])
            g_gameTrainingLegacyArrows[2 * set]->colour = 0x40ffffffU;
        if (g_gameTrainingLegacyGlows[set])
            g_gameTrainingLegacyGlows[set]->colour = 0x00ffffffU;
    }
    g_gameTrainingLegacySelection = 0;
    for (d3t::UILabelWidget *const label : {g_gameTrainingLegacyLabels[0], g_gameTrainingLegacyLabels[1]}) {
        if (label)
            label->colour = 0xff55d6faU;
    }
    if (g_gameTrainingLegacyArrows[0])
        g_gameTrainingLegacyArrows[0]->colour = 0xff55d6faU;
    if (g_gameTrainingLegacyGlows[0])
        g_gameTrainingLegacyGlows[0]->colour = 0x80ffffffU;
    if (g_gameTrainingLegacyDescriptions[0])
        g_gameTrainingLegacyDescriptions[0]->text = InitAndRetrieveStringFromID("UITRTEALL0"); // @text EN: "Train All" | JA: "すべてを鍛錬する"
    if (g_gameTrainingLegacyDescriptions[1])
        g_gameTrainingLegacyDescriptions[1]->text = InitAndRetrieveStringFromID("UITRTEEQUA"); // @text EN: "Moves Equally" | JA: "まんべんなく技を鍛錬する"
}



void GameTrainingLegacy_InitWidgets() {
    GameTrainingLegacy_BindWidgets();
    for (std::size_t set = 0; set < kGameTrainingLegacySets.size(); ++set) {
        for (std::size_t part = 0; part < 2; ++part) {
            if (g_gameTrainingLegacyLabels[2 * set + part])
                g_gameTrainingLegacyLabels[2 * set + part]->colour = ConstructColour(255, 255, 255, 64);
            if (g_gameTrainingLegacyArrows[2 * set + part])
                g_gameTrainingLegacyArrows[2 * set + part]->colour = ConstructColour(255, 255, 255, 64);
        }
        if (g_gameTrainingLegacyGlows[set])
            g_gameTrainingLegacyGlows[set]->colour = ConstructColour(255, 255, 255, 0);
    }
    GameTrainingLegacy_SelectFirst();
}



std::uint32_t LoadGameTrainingLegacyUI() {
    if (g_gameTrainingLegacyUILoadState == 0) {
        g_gameTrainingLegacyUIFileHandle = OpenLegacyUIFile("/ui/gametraining/GameTraining_Legacy.ui", kUiGameTraining);
        g_gameTrainingLegacyUILoadState = 1;
        return 1;
    }
    if (g_gameTrainingLegacyUILoadState == 1) {
        g_gameTrainingLegacyUILoadState = TryLoadUIFromFileHandle(kUiGameTraining, g_gameTrainingLegacyUIViewSlot);
        if (g_gameTrainingLegacyUILoadState == 2) {
            GameTrainingLegacy_InitWidgets();
            return static_cast<std::uint32_t>(g_gameTrainingLegacyUILoadState);
        }
    } else if (g_gameTrainingLegacyUILoadState == 2) {
        const bool finalized = static_cast<std::uint32_t>(g_gameTrainingLegacyUIViewSlot) < d3t::kUIViewSlotCount &&
            TryFinalizeUIViewTextures(g_gameTrainingLegacyUIViewSlot) == 1;
        g_gameTrainingLegacyUILoadState = finalized ? 3 : 2;
    }
    return static_cast<std::uint32_t>(g_gameTrainingLegacyUILoadState);
}





std::int64_t GameTrainingWidgetUpdate() {
    if (g_gameTrainingLegacyUILoadState == 3 || (LoadGameTrainingLegacyUI(), static_cast<std::uint32_t>(g_gameTrainingLegacyUILoadState - 2) <= 1U)) {
        const std::uint64_t frame = d3t::GetNumRenderedFrames();
        if (g_uiLegacyDrawFrame < frame)
            g_uiLegacyDrawFrame = frame;
        if (static_cast<std::uint32_t>(g_gameTrainingLegacyUIViewSlot) <= 75U) {
            const std::size_t slot = static_cast<std::size_t>(g_gameTrainingLegacyUIViewSlot);
            UI_InputHandler(GNuklearWindow, g_gameTrainingLegacyUIViewSlot, 0, g_uiViewSlots[slot].widgetCount);
        }
    }
    return 0;
}




std::int64_t GameTraining_ReloadLegacyUI() {
    ResetUIViewSlot(g_gameTrainingLegacyUIViewSlot);
    g_gameTrainingLegacyUIViewSlot = -1;
    g_gameTrainingLegacyUILoadState = 0;
    g_gameTrainingLegacyUIFileHandle = 0;
    g_gameTrainingLegacyView = nullptr;
    g_gameTrainingLegacyLabels.fill(nullptr);
    LoadUIFromMemoryIntoViewSlot(g_uiReloadFileMemory, g_gameTrainingLegacyUIViewSlot);
    g_gameTrainingLegacyUILoadState = 2;
    return 0;
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {


std::uint32_t ParseGameTraining() {
    UIBaseRuntime &element = g_uiElements[kUiGameTraining];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/gametraining/GameTraining.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}

} // namespace shenmue::s1
