#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {




void ActionSelectorSetSelectedChoice(std::int32_t choice) {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    if (selector.hasSelectedChoice && !selector.valueModeActive)
        return;
    if (g_actionSelectorSelectedChoiceSlot && g_actionSelectorSelectedChoiceSlot->slotUi)
        g_actionSelectorSelectedChoiceSlot->slotUi->sprite = 0;
    ActionSelectorChoiceUISlot &slot = g_actionSelectorChoiceSlots[static_cast<std::size_t>(choice & 3)];
    g_actionSelectorSelectedChoiceSlot = &slot;
    if (slot.slotUi)
        slot.slotUi->sprite = 1;
    selector.hasSelectedChoice = true;
    UIAnimConfigureSelectedChoice(g_actionSelectorElement.selectedChoiceAnimSlot);
    if (g_uiNavigateSoundFrame + 5 <= g_FrameCounter) {
        g_uiNavigateSoundFrame = g_FrameCounter;
        shenExecuteAICACommand(kUiNavigateSound, 0, 0);
    }
}

const char *ActionSelectorLocalizedToken(std::int32_t token) {
    if (token < 0 || static_cast<std::size_t>(token) >= kActionSelectorStringIds.size()) {
        return "";
    }
    const char *const stringId = kActionSelectorStringIds[static_cast<std::size_t>(token)];
    SubtitleStringId id{};
    std::memcpy(&id, stringId, (std::min)(std::strlen(stringId), sizeof(id)));
    const char *const localized = ReadStringFromStringTableById(id);
    return localized && *localized ? localized : stringId;
}

// A choice shows or hides: its button and label take full or no alpha, its brace shows or not.
void ActionSelector_ShowChoice(ActionSelectorChoiceUISlot &slot, bool shown) {
    const std::uint32_t alpha = shown ? 0xff000000U : 0U;
    if (slot.slotUi)
        slot.slotUi->colour = (slot.slotUi->colour & 0x00ffffffU) | alpha;
    if (slot.braceUi)
        slot.braceUi->field44 = shown ? 1 : 0;
    if (slot.labelUi)
        slot.labelUi->colour = (slot.labelUi->colour & 0x00ffffffU) | alpha;
}



void ActionSelectorSetChoiceLabelsFromTokenIds(const std::array<std::int32_t, 4> &tokens) {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    selector.helpDetailsIndex = -1;
    selector.simulatedDirectionInput = 0;
    selector.displayChoiceTokens = tokens;
    for (std::size_t index = 0; index < tokens.size(); ++index) {
        ActionSelectorChoiceUISlot &slot = g_actionSelectorChoiceSlots[index];
        const bool shown = tokens[index] > -1;
        ActionSelector_ShowChoice(slot, shown);
        std::snprintf(slot.text.data(), slot.text.size(), "%s", shown ? ActionSelectorLocalizedToken(tokens[index]) : " ");
    }
    if (g_actionSelectorPhoenixImage)
        g_actionSelectorPhoenixImage->field44 = tokens[0] == -1 ? 1 : 0;
    selector.labelsDirty = false;
}




void ActionSelectorSetValueChoiceLabels(std::int32_t maximumValue) {
    constexpr std::array<std::int32_t, 4> kValueTokens{75, 100, -100, 74};
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    selector.helpDetailsIndex = -1;
    selector.simulatedDirectionInput = 0;
    if (g_actionSelectorPhoenixImage)
        g_actionSelectorPhoenixImage->field44 = 0;
    selector.displayChoiceTokens = kValueTokens;
    selector.valueModeActive = true;
    selector.displayedValue = 0;
    selector.maximumValue = maximumValue;
    selector.valueStep = kValueTokens[1];
    GameMessage_SetContent(UIGameMessageContentType::Money, 0, "0");
    d3t::ActivateElementByIdx(kUiGameMessage);
    for (std::size_t index = 0; index < kValueTokens.size(); ++index) {
        ActionSelectorChoiceUISlot &slot = g_actionSelectorChoiceSlots[index];
        ActionSelector_ShowChoice(slot, true);
        if (index == 1 || index == 2) {
            if (slot.labelUi)
                std::snprintf(slot.text.data(), slot.text.size(), "%s", FormatCashOverlayText(kValueTokens[index]).c_str());
        } else if (kValueTokens[index] == 75) {
            if (slot.labelUi)
                std::snprintf(slot.text.data(), slot.text.size(), "%s (%s)", ActionSelectorLocalizedToken(kValueTokens[index]),
                    FormatCashOverlayText(maximumValue).c_str());
        } else {
            std::snprintf(slot.text.data(), slot.text.size(), "%s", ActionSelectorLocalizedToken(kValueTokens[index]));
        }
    }
    selector.labelsDirty = false;
}



void ActionSelector_InjectChoiceDirectionInput(float *, void *data) {
    constexpr std::array<std::uint16_t, 4> kDirections{0x1000U, 0x4000U, 0x8000U, 0x2000U};
    for (std::size_t choice = 0; choice < g_actionSelectorChoicePanels.size(); ++choice) {
        if (data != g_actionSelectorChoicePanels[choice])
            continue;
        g_inputState.slots[0].pressedPriority |= kDirections[choice];
        g_actionSelectorPresentation.simulatedDirectionInput |= kDirections[choice];
        return;
    }
}





std::int64_t ActionSelector_IsSelected() {
    UIActionSelectorElement &element = g_actionSelectorElement;
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    element.centerIconAnimSlot = UIAnimAllocSlot();
    selector.valueModeActive = false;
    selector.displayedValue = 0;
    selector.valueStep = 0;
    if (element.centerIconAnimSlot != -1)
        g_uiAnimSlots[static_cast<std::size_t>(element.centerIconAnimSlot)].loopMode = 2;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiActionSelector].config.uiHandleIndex);
    g_actionSelectorCenterIcon = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, "ActionSelector::CentreIcon"));
    if (slot > 75U)
        return 1;
    g_uiViewSlots[slot].keepLoaded = 1;
    for (std::size_t choice = 0; choice < g_actionSelectorChoiceSlots.size(); ++choice) {
        ActionSelectorChoiceUISlot &choiceSlot = g_actionSelectorChoiceSlots[choice];
        choiceSlot.slotUi = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, kActionSelectorButtonIds[choice]));
        const std::int32_t brace = UI_FindViewImageWidget(slot, kActionSelectorBraceIds[choice]);
        choiceSlot.braceUi = static_cast<std::uint32_t>(brace) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(brace)] : nullptr;
        g_actionSelectorChoicePanels[choice] = UI_GetWindow(UI_FindViewWindow(slot, kActionSelectorChoicePanelIds[choice]));
        if (g_actionSelectorChoicePanels[choice])
            g_actionSelectorChoicePanels[choice]->mouseCallbacks.clicked = ActionSelector_InjectChoiceDirectionInput;
        choiceSlot.labelUi = UI_LabelRecord(UI_FindViewLabel(slot, kActionSelectorLabelIds[choice]));
        if (choiceSlot.labelUi)
            choiceSlot.labelUi->text = choiceSlot.text.data();
    }
    element.selectedChoiceAnimSlot = UIAnimAllocSlot();
    for (std::size_t panel = 0; panel < g_actionSelectorMainPanels.size(); ++panel)
        g_actionSelectorMainPanels[panel] = UI_GetWindow(UI_FindViewWindow(slot, kActionSelectorMainPanelIds[panel]));
    element.highlightAnimSlot = UIAnimAllocSlot();
    const std::int32_t phoenix = UI_FindViewImageWidget(slot, "ActionSelector::Phoenix");
    g_actionSelectorPhoenixImage = static_cast<std::uint32_t>(phoenix) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(phoenix)] : nullptr;
    if (selector.labelsDirty) {
        if (selector.useValueLabels == 0)
            ActionSelectorSetChoiceLabelsFromTokenIds(selector.displayChoiceTokens);
        else if (selector.useValueLabels == 1)
            ActionSelectorSetValueChoiceLabels(0);
    }
    selector.labelsDirty = false;
    return 1;
}




void ActionSelector_Render() {
    UIActionSelectorElement &element = g_actionSelectorElement;
    if (element.centerIconAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(element.centerIconAnimSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.start[0] = 1.0f;
        slot.current[0] = 1.0f;
        slot.end[0] = 0.1f;
        slot.delay = 0.01f;
        slot.elapsed = 0.0f;
        slot.duration = 0.2f;
        slot.reserved40 = -1;
    }
    if (g_actionSelectorCenterIcon)
        g_actionSelectorCenterIcon->sprite = 45;
    g_actionSelectorInputCode = 13;
    for (ActionSelectorChoiceUISlot &choice : g_actionSelectorChoiceSlots) {
        if (choice.slotUi)
            choice.slotUi->sprite = 0;
    }
    g_actionSelectorSelectedChoiceSlot = nullptr;
    if (element.highlightAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(element.highlightAnimSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.start[0] = 0.0f;
        slot.current[0] = 0.0f;
        slot.end[0] = 1.0f;
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.duration = 0.5f;
        slot.reserved40 = -1;
    }
    g_actionSelectorPresentation.hasSelectedChoice = false;
    if (g_uiNavigateSoundFrame + 5 <= g_FrameCounter) {
        g_uiNavigateSoundFrame = g_FrameCounter;
        shenExecuteAICACommand(kUiNavigateSound, 0, 0);
    }
}

// STATIC_Action_Selector_IsPressed: once the panel has slid fully in, turn the frame's presses into the selector input code. In value
// mode the directions step, fill or clear the amount. X opens the selector's help page (SetHelpDetailsByIndex) when it has one.
std::int64_t ActionSelector_IsPressed() {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    const InputSlot &input = g_inputState.slots[0];
    const auto repeated = static_cast<std::uint16_t>(~g_actionSelectorPreviousPressed & input.repeatedPresses);
    const std::uint16_t pressed = input.pressedPriority;
    g_actionSelectorPreviousPressed = pressed;
    const std::int32_t highlight = g_actionSelectorElement.highlightAnimSlot;
    if (highlight == -1 || g_uiAnimSlots[static_cast<std::size_t>(highlight)].current[0] < 1.0f)
        return 1;
    const auto setValue = [&selector](std::int32_t value) {
        selector.displayedValue = value;
        std::snprintf(g_gameMessagePresentation.labelText.data(), g_gameMessagePresentation.labelText.size(), "%s", FormatCashOverlayText(value).c_str());
    };
    const std::uint16_t directions = repeated | pressed;
    std::int32_t code = g_actionSelectorInputCode;
    if ((directions & 0x4000U) != 0 && selector.displayChoiceTokens[1] != -1) {
        ActionSelectorSetSelectedChoice(1);
        if (selector.valueModeActive)
            setValue(
                selector.displayedValue + selector.valueStep > selector.maximumValue ? selector.maximumValue : selector.displayedValue + selector.valueStep);
        code = 6;
    }
    if ((directions & 0x8000U) != 0 && selector.displayChoiceTokens[2] != -1) {
        code = 2;
        ActionSelectorSetSelectedChoice(2);
        if (selector.valueModeActive)
            setValue(selector.displayedValue - selector.valueStep < 0 ? 0 : selector.displayedValue - selector.valueStep);
    }
    if ((directions & 0x1000U) != 0 && selector.displayChoiceTokens[0] != -1) {
        ActionSelectorSetSelectedChoice(0);
        if (selector.valueModeActive)
            setValue(selector.maximumValue);
        code = 0;
    }
    if ((directions & 0x2000U) != 0 && selector.displayChoiceTokens[3] != -1) {
        if (selector.valueModeActive)
            setValue(0);
        ActionSelectorSetSelectedChoice(3);
        code = 4;
    }
    if ((pressed & 0x200U) != 0)
        code = 9;
    if ((pressed & 0x400U) != 0)
        code = 8;
    if ((pressed & 2U) != 0)
        code = 11;
    g_actionSelectorInputCode = code;
    if (selector.helpDetailsIndex != -1 && (pressed & 4U) != 0)
        SetHelpDetailsByIndex(selector.helpDetailsIndex);
    return 1;
}





std::int64_t ActionSelector_Tick() {
    UIActionSelectorElement &element = g_actionSelectorElement;
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    if (g_actionSelectorCenterIcon && element.centerIconAnimSlot != -1) {
        const float alpha = g_uiAnimSlots[static_cast<std::size_t>(element.centerIconAnimSlot)].current[0] * 255.0f;
        const auto byte = static_cast<std::uint32_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(alpha)));
        g_actionSelectorCenterIcon->colour = (g_actionSelectorCenterIcon->colour & 0x00ffffffU) | (byte << 24U);
    }
    const UIBaseRuntime &uiElement = g_uiElements[kUiActionSelector];
    const std::uint32_t slot = static_cast<std::uint32_t>(uiElement.config.uiHandleIndex);
    if (slot > 75U)
        return 0;
    ActionSelectorChoiceUISlot *const selected = g_actionSelectorSelectedChoiceSlot;
    if (selected && selected->slotUi && element.selectedChoiceAnimSlot != -1) {
        UIAnimSlot &blink = g_uiAnimSlots[static_cast<std::size_t>(element.selectedChoiceAnimSlot)];
        const std::int32_t frame = std::bit_cast<std::int32_t>(blink.current[0]);
        if (frame != selected->slotUi->sprite && !selector.valueModeActive) {
            blink.duration = 0.001f;
            blink.delay = 0.15f;
            blink.allocated = 1;
            blink.status = 1;
            blink.interpolation = 3;
            blink.start[0] = std::bit_cast<float>(frame);
            blink.current[0] = std::bit_cast<float>(frame);
            blink.end[0] = std::bit_cast<float>(selected->slotUi->sprite);
            blink.elapsed = 0.0f;
            blink.reserved40 = -1;
        }
        selected->slotUi->sprite = frame;
    }
    d3t::UIWindow *const out = g_actionSelectorMainPanels[0];
    d3t::UIWindow *const in = g_actionSelectorMainPanels[1];
    d3t::UIWindow *const main = g_actionSelectorMainPanels[2];
    if (out && in && main && element.highlightAnimSlot != -1) {
        UILayoutRect outRect{};
        UI_GetWindowRect(out, outRect);
        UILayoutRect inRect{};
        UI_GetWindowRect(in, inRect);
        const float progress = g_uiAnimSlots[static_cast<std::size_t>(element.highlightAnimSlot)].current[0];
        main->layout.offsetX = (inRect.x - outRect.x) * progress / static_cast<float>(UI_GetAspectWidth());
        main->layout.offsetY = (inRect.y - outRect.y) * progress / 1080.0f;
    }
    g_currActiveUIIndex = uiElement.index;
    UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    if (selector.valueModeActive)
        g_uiElements[kUiGameMessage].isActive = 1;
    return 0;
}



std::int64_t ActionSelector_Destroy() {
    UIActionSelectorElement &element = g_actionSelectorElement;
    if (element.centerIconAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(element.centerIconAnimSlot)];
        slot.status = 0;
        slot.elapsed = 0.0f;
        slot.allocated = 1;
    }
    if (element.highlightAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(element.highlightAnimSlot)];
        const float progress = slot.current[0];
        bool slide = false;
        if (progress >= 1.0f) {
            slot.start[0] = 1.0f;
            slot.current[0] = 1.0f;
            slot.delay = 0.45f;
            slot.elapsed = 0.0f;
            slot.duration = 0.5f;
            slide = true;
        } else if (progress * 0.5f > 0.0f) {
            slot.start[0] = progress;
            slot.current[0] = progress;
            slot.duration = progress * 0.5f;
            slot.delay = 0.0f;
            slot.elapsed = 0.0f;
            slide = true;
        }
        if (slide) {
            slot.reserved40 = -1;
            slot.end[0] = 0.0f;
            slot.interpolation = 2;
            slot.status = 1;
            slot.allocated = 1;
        }
    }
    UIBaseRuntime &gameMessage = g_uiElements[kUiGameMessage];
    if (g_actionSelectorPresentation.valueModeActive && (g_currUIState == -1 || g_currUIState == kUiGameMessage) &&
        (!gameMessage.isVisible || !gameMessage.destroy || gameMessage.destroy() == 1 || g_busy_reset == 1))
        gameMessage.isVisible = 0;
    g_actionSelectorPresentation.valueModeActive = false;
    return 1;
}



std::int64_t ActionSelector_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    const ActionSelectorOwner &selector = g_actionSelectorPresentation;
    const std::int16_t select = selector.valueModeActive && selector.displayedValue > 0 ? 45 : -1;
    const std::int16_t help = selector.helpDetailsIndex != -1 ? 58 : -1;
    const std::array<std::int16_t, 5> set{select, 44, help, -1, -1};
    for (std::size_t slot = 0; slot < set.size(); ++slot) {
        g_searchPromptIconSlots[slot] = set[slot];
        prompts[slot] = static_cast<std::uint16_t>(set[slot]);
    }
    return reinterpret_cast<std::int64_t>(prompts.data());
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

bool UI_ICSpRequest(const UIActionSelectorRequest &request) {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    ActionSelectorSetChoiceLabelsFromTokenIds({request.up, request.left, request.right, request.down});
    selector.openRequested = true;
    selector.selectedHelpToken = -1;
    selector.elapsedFrames = 0;
    selector.request = request;
    if (selector.request.timeoutFrames == 0x7fff)
        selector.request.timeoutFrames = 600;
    auto sanitize = [](std::int16_t &token) {
        if (token >= 70)
            token = -1;
    };
    sanitize(selector.request.up);
    sanitize(selector.request.left);
    sanitize(selector.request.right);
    sanitize(selector.request.down);
    if (selector.request.up == selector.request.left && selector.request.up == selector.request.right && selector.request.up == selector.request.down) {
        selector.request.up = 0;
        selector.request.left = 0;
        selector.request.right = 0;
        selector.request.down = 0;
    }
    const std::array<std::int16_t, 4> choices{selector.request.up, selector.request.left, selector.request.down, selector.request.right};
    for (std::size_t index = 0; index < choices.size(); ++index) {
        float width = 128.0f;
        if (static_cast<std::uint16_t>(choices[index]) != 0xffffU) {
            const std::size_t length = std::strlen(ActionSelectorLocalizedToken(static_cast<std::uint16_t>(choices[index])));
            if (length > 8)
                width += static_cast<float>(14U * length - 112U);
        }
        selector.choiceWidths[index] = static_cast<std::int16_t>(static_cast<std::int32_t>(width));
    }
    const std::int16_t horizontalWidth = selector.choiceWidths[1] > selector.choiceWidths[3] ? selector.choiceWidths[1] : selector.choiceWidths[3];
    selector.choiceWidths[1] = horizontalWidth;
    selector.choiceWidths[3] = horizontalWidth;
    if (selector.request.mode == 0)
        selector.request.timeoutHelpToken = -1;
    const float extraWidth = static_cast<float>(horizontalWidth) - 128.0f;
    selector.panelTargetX = extraWidth + 144.0f;
    selector.panelHalfWidth = static_cast<float>(static_cast<double>(extraWidth) * 0.5);
    selector.panelStartX = static_cast<float>(-176.0 - static_cast<double>(extraWidth));
    return true;
}

bool UI_ICSpIsIdle() {
    return g_actionSelectorPresentation.state == UIActionSelectorState::Idle;
}

bool UI_ICSpPeekSelectedHelpToken(std::int16_t &helpToken) {
    const ActionSelectorOwner &selector = g_actionSelectorPresentation;
    if (selector.state != UIActionSelectorState::Idle)
        return false;
    helpToken = selector.selectedHelpToken;
    return true;
}

bool UI_ICSpPeekSelectedChoiceIndex(std::int32_t &choiceIndex) {
    const ActionSelectorOwner &selector = g_actionSelectorPresentation;
    if (selector.state != UIActionSelectorState::Idle)
        return false;
    const std::int16_t token = selector.selectedHelpToken;
    if (token == selector.request.up && selector.request.up != -1)
        choiceIndex = 0;
    else if (token == selector.request.right && selector.request.right != -1)
        choiceIndex = 2;
    else if (token == selector.request.down && selector.request.down != -1)
        choiceIndex = 3;
    else if (token == selector.request.left && selector.request.left != -1)
        choiceIndex = 1;
    else
        choiceIndex = -1;
    return true;
}

void UI_ICSpCancel() {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    if (selector.state == UIActionSelectorState::Idle)
        return;
    selector.selectedHelpToken = -1;
    selector.selectedChoice = -1;
    selector.state = UIActionSelectorState::SelectionCommitted;
}

void UI_ICSpCallback(HLib::TaskData *) {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    selector = {};
    selector.request = {-1, -1, -1, -1, 600, -1, 0};
    selector.state = UIActionSelectorState::Idle;
    selector.selectedHelpToken = -1;
    selector.selectedChoice = -1;
    selector.panelStartX = -176.0f;
    selector.panelTargetX = 144.0f;
    selector.panelX = -176.0f;
    selector.panelVelocity = 1.0f;
    g_iconPresentation.actionSelectorTask = CurrentTask;
    InstallOwnedTaskMain(UI_ICSpMainCallback);
}

void UI_ICSpMainCallback(HLib::TaskData *) {
    ActionSelectorOwner &selector = g_actionSelectorPresentation;
    IconPresentationOwner &icon = g_iconPresentation;
    switch (selector.state) {
    case UIActionSelectorState::Idle:
        if (!selector.openRequested)
            break;
        selector.state = UIActionSelectorState::Opening;
        g_isPlayerGameStateActive = 4;
        CapturePad(icon.input, icon.previousInput);
        selector.hasSelectedChoice = false;
        selector.valueModeActive = false;
        icon.iconFlags |= 0x400U;
        selector.selectedHelpToken = -1;
        selector.savedHelpToken = g_defaultHelpTokenId;
        selector.savedHelpButton = g_helpButtonId;
        icon.overlayMode = 10;
        selector.savedPauseState = Shenmue_nPauseState;
        Shenmue_nPauseState &= ~0x20;
        selector.panelX = selector.panelStartX;
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_defaultHelpTokenId = 36;
        g_currentHelpTokenId = 36;
        g_helpButtonId = 0;
        g_currentHelpIconState = 0;
        ActivateOwnedElement(kUiActionSelector, "/ui/actionselector/ActionSelector.ui");
        break;
    case UIActionSelectorState::Opening: {
        const float distance = std::fabs(selector.panelTargetX - selector.panelX);
        float velocity = distance * 0.5f;
        if (velocity > 64.0f)
            velocity = selector.panelVelocity * 2.0f;
        selector.panelVelocity = std::clamp(velocity, 1.0f, 64.0f);
        selector.panelX += selector.panelVelocity;
        if (selector.panelX >= selector.panelTargetX) {
            selector.panelX = selector.panelTargetX;
            selector.state = UIActionSelectorState::Active;
        }
        break;
    }
    case UIActionSelectorState::Active: {
        if (icon.overlayMode != 10)
            break;
        // ActionSelector_ResolveChoice.
        const std::uint8_t code = static_cast<std::uint8_t>(g_actionSelectorInputCode);
        g_actionSelectorInputCode = 13;
        if ((code == 2 || code == 9) && (g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kQteMinigameUiElement))) {
            UIBaseRuntime &qte = g_uiElements[kQteMinigameUiElement];
            if (!qte.isVisible || !qte.destroy || qte.destroy() == 1 || g_busy_reset == 1)
                qte.isVisible = 0;
        }
        const std::array<std::int16_t, 4> stored{selector.request.up, selector.request.left, selector.request.down, selector.request.right};
        constexpr std::array<std::int8_t, 4> kDisplayChoice{0, 1, 3, 2};
        bool countTimeout = code == 13;
        if ((code & 0xf9) == 0) {
            const std::uint8_t index = kActionSelectorChoiceByInputCode[static_cast<std::size_t>(code)];
            if (stored[index] == -1) {
                countTimeout = true;
            } else {
                selector.selectedChoice = kDisplayChoice[index];
                selector.selectedHelpToken = stored[index];
                selector.state = UIActionSelectorState::SelectionCommitted;
            }
        } else if (code == 9 && (selector.request.mode == 2 || selector.request.mode == 5)) {
            selector.selectedChoice = -1;
            selector.selectedHelpToken = -1;
            selector.state = UIActionSelectorState::SelectionCommitted;
        }
        if (countTimeout && ++selector.elapsedFrames >= static_cast<std::int16_t>(selector.request.timeoutFrames)) {
            selector.state = UIActionSelectorState::SelectionCommitted;
            if (selector.request.timeoutHelpToken == -1) {
                selector.selectedHelpToken = -1;
            } else {
                for (std::size_t index = 0; index < stored.size(); ++index) {
                    if (stored[index] == selector.request.timeoutHelpToken) {
                        selector.selectedChoice = kDisplayChoice[index];
                        selector.selectedHelpToken = selector.request.timeoutHelpToken;
                        break;
                    }
                }
            }
        }
        break;
    }
    case UIActionSelectorState::SelectionCommitted:
        selector.resetRequested = true;
        if (ResetOwnedElement(kUiActionSelector))
            selector.state = UIActionSelectorState::Closing;
        break;
    case UIActionSelectorState::Closing: {
        const std::int32_t highlight = g_actionSelectorElement.highlightAnimSlot;
        if (highlight != -1 && g_uiAnimSlots[static_cast<std::size_t>(highlight)].current[0] > 0.0f)
            break;
        const float distance = std::fabs(selector.panelX - selector.panelStartX);
        float velocity = distance * 0.5f;
        if (velocity > 64.0f)
            velocity = selector.panelVelocity * 2.0f;
        selector.panelVelocity = std::clamp(velocity, 1.0f, 64.0f);
        selector.panelX -= selector.panelVelocity;
        if (selector.panelX > selector.panelStartX)
            break;
        selector.panelX = selector.panelStartX;
        selector.state = UIActionSelectorState::Idle;
        iconUpdateInputBuffer();
        icon.iconFlags &= static_cast<std::uint16_t>(~0x400U);
        icon.overlayMode = 0;
        g_previousHelpTokenId = g_currentHelpTokenId;
        selector.openRequested = false;
        g_currentHelpIconState = 0;
        g_defaultHelpTokenId = static_cast<std::int8_t>(selector.savedHelpToken) < 68 ? selector.savedHelpToken : 0;
        g_currentHelpTokenId = g_defaultHelpTokenId;
        g_helpButtonId = selector.savedHelpButton;
        if ((selector.savedPauseState & 0x20) != 0) {
            Shenmue_nPauseState = selector.savedPauseState;
            selector.savedPauseState = 0;
        }
        selector.resultAvailable = true;
        selector.resetRequested = false;
        break;
    }
    case UIActionSelectorState::OpenRequested:
        break;
    }
    if ((icon.iconFlags & 0x400U) != 0)
        g_uiElements[kUiActionSelector].isActive = 1;
}

} // namespace shenmue::s1
