#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {


std::int64_t WristwatchOverlay_IsPressed() {
    if ((g_inputState.slots[0].pressedPriority & 4U) != 0)
        SetHelpDetailsByIndex(24);
    return 1;
}


std::int64_t WristwatchOverlay_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    constexpr std::array<std::int16_t, 4> kPrompts{10, 44, 58, -1};
    for (std::size_t slot = 0; slot < kPrompts.size(); ++slot)
        g_searchPromptIconSlots[slot] = kPrompts[slot];
    for (std::size_t slot = 0; slot < prompts.size(); ++slot)
        prompts[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(prompts.data());
}

// The pulse hudSetPromptIconOverrideSet runs for a changed prompt: 1 to 0 over 1.2 s.
void HudPromptStartIconPulse(std::int32_t slotIndex) {
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 2;
    slot.start[0] = 1.0f;
    slot.current[0] = 1.0f;
    slot.end[0] = 0.0f;
    slot.delay = 0.0f;
    slot.elapsed = 0.0f;
    slot.duration = 1.2f;
    slot.reserved40 = -1;
}

// The shared highlight a changed prompt's alpha follows: 1 to 0.1 over 0.2 s after 0.01 s.
void HudPromptStartHighlight(std::int32_t slotIndex) {
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
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



void HudSetPromptIconOverrideSet(const std::int32_t *prompts) {
    if (prompts) {
        for (std::size_t index = 0; index < g_hudPromptRequestedSlots.size(); ++index)
            g_hudPromptRequestedSlots[index] = prompts[index];
        for (std::size_t index = 0; index < 4; ++index) {
            std::int32_t value = g_hudPromptRequestedSlots[index];
            const std::int32_t previous = g_hudPromptOverrideSlots[index];
            const std::int32_t pulse = g_hudPromptIconAnimSlots[index];
            const auto icon = static_cast<std::uint16_t>(value);

            if (((previous & 0x10000) != 0 && g_uiAnimSlots[static_cast<std::size_t>(pulse)].status != 0) ||
                (icon != static_cast<std::uint16_t>(previous) && icon != 0xffffU))
                value |= 0x10000;
            if ((value & 0x10000) != 0 && g_uiAnimSlots[static_cast<std::size_t>(pulse)].status == 0) {
                if (pulse != -1)
                    HudPromptStartIconPulse(pulse);
                if (g_uiAnimSlots[static_cast<std::size_t>(g_hudPromptHighlightAnimSlot)].status == 0 && g_hudPromptHighlightAnimSlot != -1)
                    HudPromptStartHighlight(g_hudPromptHighlightAnimSlot);
            }
            g_hudPromptOverrideSlots[index] = value;
        }
    }
    g_hudPromptOverrideActive = prompts != nullptr;
}



void UI_UpdateSearchIcons() {
    const IconPresentationOwner &icon = g_iconPresentation;
    const bool itemLookPrompt = g_uiElements[kUiSubMenuItems].isVisible && (icon.sharedItemPreviewValue == 1 || icon.sharedItemPreviewValue == 51);
    if (icon.sharedItemMenuState && !itemLookPrompt) {
        HudSetPromptIconOverrideSet(kSharedItemMenuPromptSet.data());
        return;
    }
    const std::int32_t *prompts = nullptr;
    if (g_uiDebugElementDispatch == 1) {
        for (std::uint32_t index = 79; index; --index) {
            const UIBaseRuntime &element = g_uiElements[index];
            if (!element.config.status || !element.getSearchType)
                continue;
            prompts = reinterpret_cast<const std::int32_t *>(element.getSearchType());
            if (prompts)
                break;
        }
        if (prompts) {
            g_uiElements[kUiGameHud].config.isEnabled = 1;
            g_uiElements[kUiGameHud].config.status = 1;
        }
    }
    HudSetPromptIconOverrideSet(prompts);
}



bool HudPopulatePromptIcons(const std::array<std::int32_t, 5> &slots) {
    std::array<UIButtonIconWidget *, 6> &sprites = g_gameHudPromptSprites;
    for (const std::size_t prompt : {kHudPromptCancel, kHudPromptConfirm, kHudPromptSystem, kHudPromptEvent}) {
        if (sprites[prompt])
            sprites[prompt]->colour &= 0x00ffffffU;
    }
    const std::uint32_t flags = g_hudPromptFlagsThisFrame;
    if (g_suppressDefaultHudPromptIcons != 0 && !flags)
        return false;
    const auto show = [](UIButtonIconWidget *sprite, std::int32_t prompt) {
        if (sprite && (sprite->sprite == -1 || !(sprite->colour >> 24U))) {
            sprite->sprite = prompt;
            sprite->colour |= 0xff000000U;
        }
    };
    for (std::uint32_t bit = 0; bit < 13; ++bit) {
        if ((flags & (1U << bit)) == 0)
            continue;
        switch (bit) {
        case 1:
            show(sprites[ResolveCurrentInputForGame(-1) ? kHudPromptConfirm : kHudPromptCancel], 44);
            break;
        case 2:
            show(sprites[ResolveCurrentInputForGame(-1) ? kHudPromptCancel : kHudPromptConfirm], 45);
            break;
        case 3:
            show(sprites[kHudPromptSystem], 46);
            break;
        case 4:
            show(sprites[kHudPromptEvent], 45);
            break;
        case 9:
            show(sprites[kHudPromptEvent], 9);
            break;
        default:
            break;
        }
    }
    if (!g_suppressDefaultHudPromptIcons && convCanOpen() && !MMTK_HasManualSession()) {
        show(sprites[kHudPromptEvent], 51);
        if (SRCH_AllowsIconInputPriority())
            show(sprites[kHudPromptSystem], 46);
    }
    for (std::size_t index = 0; index < 4; ++index) {
        const auto prompt = static_cast<std::int16_t>(slots[index]);
        if (prompt != -1 && sprites[index]) {
            sprites[index]->sprite = prompt;
            sprites[index]->colour |= 0xff000000U;
        }
    }
    return true;
}



std::int32_t UI_FindViewIconWidget(std::int32_t kind, std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75U || !id)
        return -1;
    const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != kind)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        const UIButtonIconWidget *const record = UI_ButtonIconRecord(index);
        if (record && record->id[0] && !std::strcmp(record->id.data(), id) && index != -1)
            return index;
    }
    return -1;
}





void GameHud_ButtonClicked(float *, void *data) {
    const std::int32_t *const slots = g_hudPromptOverrideActive ? g_hudPromptOverrideSlots.data() : g_hudPromptEffectiveSlots.data();
    const d3t::UIWindow *const window = static_cast<const d3t::UIWindow *>(data);
    if (window == g_gameHudWindows[6] && (slots[0] & 0x20000) == 0)
        g_pendingInputFlags |= ResolveCurrentInputForGame(-1) ? 0x200U : 0x400U;
    if (window == g_gameHudWindows[3] && (slots[3] & 0x20000) == 0)
        g_pendingInputFlags |= ResolveCurrentInputForGame(-1) ? 0x400U : 0x200U;
    if (window == g_gameHudWindows[12] && (slots[3] & 0x20000) == 0)
        g_pendingInputFlags |= 4U;
    if (window == g_gameHudWindows[9] && (slots[3] & 0x20000) == 0)
        g_pendingInputFlags |= 2U;
    if (window == g_gameHudWindows[14] && !g_uiElements[kUiGameMenu].isVisible)
        g_pendingInputFlags |= 8U;
}




std::int64_t GameHud_IsSelected() {
    g_hudPromptKeyboardLayoutApplied = 0;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHud].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_gameHudWindows.size(); ++window)
        g_gameHudWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kGameHudWindowIds[window]));
    for (const std::size_t button : {std::size_t{6}, std::size_t{3}, std::size_t{12}, std::size_t{9}, std::size_t{14}}) {
        if (g_gameHudWindows[button])
            g_gameHudWindows[button]->mouseCallbacks.clicked = GameHud_ButtonClicked;
    }
    g_gameHudCameraLabel = UI_LabelRecord(UI_FindViewLabel(slot, "Camera_Label"));
    for (std::size_t sprite = 0; sprite < g_gameHudPromptSprites.size(); ++sprite)
        g_gameHudPromptSprites[sprite] = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, kGameHudSpriteIds[sprite]));
    g_gameHudCameraBase = UI_ColourImageRecord(UI_FindViewColourImage(slot, "HUDCameraBase"));
    for (std::size_t icon = 0; icon < g_gameHudButtonIcons.size(); ++icon)
        g_gameHudButtonIcons[icon] = UI_ButtonIconRecord(UI_FindViewIconWidget(21, slot, kGameHudButtonIconIds[icon]));
    if (g_gameHudPromptSprites[5])
        g_gameHudPromptSprites[5]->fieldAC = 1;
    g_hudPromptHighlightAnimSlot = UIAnimAllocSlot();
    if (g_hudPromptHighlightAnimSlot != -1) {
        g_uiAnimSlots[static_cast<std::size_t>(g_hudPromptHighlightAnimSlot)].loopMode = 2;
        HudPromptStartHighlight(g_hudPromptHighlightAnimSlot);
    }
    for (std::int32_t &pulse : g_hudPromptIconAnimSlots)
        pulse = UIAnimAllocSlot();
    return 1;
}






std::int64_t GameHud_Tick() {
    if (LoadScreen.isVisible || CDTT_IsLoadingUIActive())
        return 0;
    std::array<std::int32_t, 5> effective{g_hudPromptEffectiveSlots[0], g_hudPromptEffectiveSlots[1], g_hudPromptEffectiveSlots[2],
        g_hudPromptEffectiveSlots[3], static_cast<std::uint16_t>(SRCH_GetPromptIconSlots()[4])};
    std::array<std::int32_t, 5> slots = g_hudPromptOverrideActive ? g_hudPromptOverrideSlots : effective;
    if (ResolveCurrentInputForGame(-1))
        std::swap(slots[0], slots[1]);
    if (g_keyboardActive == 1 ? !g_hudPromptKeyboardLayoutApplied : g_hudPromptKeyboardLayoutApplied == 1) {
        const bool keyboard = g_keyboardActive == 1;
        g_hudPromptKeyboardLayoutApplied = keyboard ? 1 : 0;
        const float shift = keyboard ? 0.0092592593f : -0.0092592593f;
        if (g_gameHudWindows[0])
            g_gameHudWindows[0]->layout.offsetY = g_gameHudWindows[0]->layout.offsetY - shift;
        for (const std::size_t window : {std::size_t{14}, std::size_t{7}, std::size_t{4}, std::size_t{13}, std::size_t{10}}) {
            if (g_gameHudWindows[window])
                g_gameHudWindows[window]->layout.offsetY = g_gameHudWindows[window]->layout.offsetY + shift;
        }
        if (g_gameHudCameraBase)
            g_gameHudCameraBase->colour = (g_gameHudCameraBase->colour & 0x00ffffffU) | (keyboard ? 0xff000000U : 0U);
        if (g_gameHudButtonIcons[4])
            g_gameHudButtonIcons[4]->colour = (g_gameHudButtonIcons[4]->colour & 0x00ffffffU) | (keyboard ? 0U : 0xff000000U);
    }
    std::array<UIButtonIconWidget *, 6> &sprites = g_gameHudPromptSprites;
    const float highlight = g_hudPromptHighlightAnimSlot != -1 ? g_uiAnimSlots[static_cast<std::size_t>(g_hudPromptHighlightAnimSlot)].current[0] : 1.0f;
    bool anyHighlighted = false;
    for (std::size_t index = 0; index < 5; ++index) {
        const bool highlighted = (slots[index] & 0x10000) != 0;
        const std::uint32_t alpha = highlighted ? static_cast<std::uint8_t>(static_cast<std::int32_t>(highlight * 255.0f)) : 255U;
        if (sprites[index])
            sprites[index]->colour = (sprites[index]->colour & 0x00ffffffU) | (alpha << 24U);
        anyHighlighted = anyHighlighted || highlighted;
    }
    if (!anyHighlighted && g_hudPromptHighlightAnimSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_hudPromptHighlightAnimSlot)];
        slot.status = 0;
        slot.elapsed = 0.0f;
        slot.allocated = 1;
    }
    for (std::size_t index = 0; index < 5; ++index) {
        UIButtonIconWidget *const sprite = sprites[index];
        if (!sprite)
            continue;
        if (index == kHudPromptCamera) {
            sprite->sprite = -1;
            continue;
        }
        sprite->sprite = static_cast<std::int16_t>(slots[index]);
        if ((slots[index] & 0x20000) != 0)
            sprite->colour = (sprite->colour & 0x00ffffffU) | (100U << 24U);
    }
    if (!g_hudPromptOverrideActive && !HudPopulatePromptIcons(effective))
        return 0;
    if (sprites[kHudPromptEvent] && sprites[kHudPromptEvent]->sprite == 51 && g_hudPromptJinguruIconOverrideActive == 1)
        sprites[kHudPromptEvent]->sprite = 9;
    bool anyPrompt = false;
    for (std::size_t index = 0; index < g_gameHudButtonIcons.size(); ++index) {
        UIButtonIconWidget *const icon = g_gameHudButtonIcons[index];
        if (!icon)
            continue;
        icon->controlState = static_cast<std::int32_t>(GetCurrentInputType());
        const bool prompt = sprites[index] && sprites[index]->sprite != -1;
        icon->colour = (icon->colour & 0x00ffffffU) | (prompt ? 0xff000000U : 0xa8000000U);
        anyPrompt = anyPrompt || prompt;
    }
    const std::int16_t item = g_iconPresentation.sharedItemPreviewValue;
    if (item >= 0 && item <= 66 && (g_playerStats.ownedItemMenuFlags0To66[static_cast<std::size_t>(item)] & 2U) != 0) {
        UIButtonIconWidget *const sprite = sprites[ResolveCurrentInputForGame(-1) ? kHudPromptCancel : kHudPromptConfirm];
        if (sprite)
            sprite->sprite = 45;
    }
    if (anyPrompt) {
        const UIBaseRuntime &element = g_uiElements[kUiGameHud];
        const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
        g_currActiveUIIndex = element.index;
        if (slot <= 0x4bU)
            UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    }
    g_hudPromptFlagsThisFrame = 0;
    for (std::size_t index = 0; index < 5; ++index) {
        if (sprites[index]) {
            sprites[index]->sprite = -1;
            sprites[index]->colour &= 0x00ffffffU;
        }
    }
    return 0;
}



std::int64_t GameHudWatch_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudWatch].config.uiHandleIndex);
    if (slot > 0x4bU)
        return 1;
    d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &view = g_uiViewSlots[slot];
    view.keepLoaded = 1;
    for (std::size_t image = 1; image <= 3; ++image) {
        view.images[image].rotate = 1;
        view.images[image].angle = 0.0f;
    }
    for (std::int32_t widget = 0; widget < view.widgetCount; ++widget) {
        if (view.widgetKinds[static_cast<std::size_t>(widget)] != 15)
            continue;
        d3t::UILabelWidget *const label = UI_LabelRecord(view.widgetPoolIndices[static_cast<std::size_t>(widget)]);
        if (label && label->text && !std::strcmp(label->text, "TIME"))
            label->text = g_gameHudWatchTime.data();
    }
    return 1;
}



std::int64_t GameHudWatch_Tick() {
    if (LoadScreen.isVisible || CDTT_IsLoadingUIActive() || g_uiElements[kUiMoveLearned].isVisible)
        return 0;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudWatch].config.uiHandleIndex);
    if (slot > 0x4bU)
        return 0;
    d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &view = g_uiViewSlots[slot];
    view.images[1].angle = g_secondHandAngle;
    view.images[3].angle = g_hourHandRadians;
    view.images[2].angle = g_minuteHandRadians;
    view.images[1].rotate = 1;
    view.images[2].rotate = 1;
    view.images[3].rotate = 1;
    std::snprintf(g_gameHudWatchTime.data(), g_gameHudWatchTime.size(), "%s",
        InitAndRetrieveStringFromID(
            g_currentDateTime.hours < 12 ? "UILOADAM00" : "UILOADPM00")); // @text UILOADAM00 EN: "am" | JA: "午前" ; UILOADPM00 EN: "pm" | JA: "午後"
    UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, view.widgetCount);
    return 0;
}

// A Gauge widget record (g_fightGaugeWidgetStates) by its pool index; none past the five records.
d3t::UIGaugeState *FightGaugeWidget(std::int32_t index) {
    return static_cast<std::uint32_t>(index) < g_fightGaugeWidgetStates.size() ? &g_fightGaugeWidgetStates[static_cast<std::size_t>(index)] : nullptr;
}



void HUD_FightGauge_SetDisplayLayout(std::int32_t layout) {
    if (g_fightGaugeDisplayMode == layout)
        return;
    d3t::UIGaugeState *const player = FightGaugeWidget(g_fightGaugeWidgetIndices[0]);
    d3t::UIGaugeState *const enemy = FightGaugeWidget(g_fightGaugeWidgetIndices[1]);
    if (layout == 0) {
        if (player) {
            for (std::size_t index = 0; index < 12; ++index)
                player->orbColours[index] = index < 8 ? 2 : index < 11 ? 3 : 1;
            player->translucentLastOrb = 0;
        }
    } else if (layout == 1) {
        if (player) {
            player->orbColours.fill(0);
            player->translucentLastOrb = 1;
        }
        if (enemy) {
            enemy->orbColours.fill(1);
            enemy->translucentLastOrb = 1;
        }
    }
    g_fightGaugeDisplayMode = layout;
}


std::uint32_t GameHudGauge_LoadUI() {
    UIBaseRuntime &element = g_uiElements[kUiGameHudGauge];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/gamehud/GameHudGauge.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}





std::int64_t GameHudGauge_IsSelected() {
    g_fightGaugeWidgetIndices = {-1, -1};
    g_fightGaugeScissorIndices = {-1, -1};
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudGauge].config.uiHandleIndex);
    const std::int32_t count = slot <= 0x4bU ? g_uiViewSlots[slot].widgetCount : 0;
    std::size_t gauges = 0;
    std::size_t scissors = 0;
    for (std::int32_t widget = 0; widget < count; ++widget) {
        const std::int32_t kind = g_uiViewSlots[slot].widgetKinds[static_cast<std::size_t>(widget)];
        const std::int32_t index = g_uiViewSlots[slot].widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (kind == 35) {
            if (gauges > 2)
                return 0;
            if (gauges < g_fightGaugeWidgetIndices.size())
                g_fightGaugeWidgetIndices[gauges] = index;
            ++gauges;
        } else if (kind == 22) {
            if (scissors < g_fightGaugeScissorIndices.size())
                g_fightGaugeScissorIndices[scissors] = index;
            ++scissors;
        }
    }
    for (const std::int32_t scissor : g_fightGaugeScissorIndices) {
        if (static_cast<std::uint32_t>(scissor) <= 9U)
            g_uiScissorWidgets[static_cast<std::size_t>(scissor)].width = 0.0f;
    }
    const std::int32_t layout = g_fightGaugeDisplayMode;
    g_fightGaugeDisplayMode = -1;
    HUD_FightGauge_SetDisplayLayout(layout);
    return 1;
}



std::int64_t GameHudGauge_Tick() {
    d3t::UIGaugeState *const player = FightGaugeWidget(g_fightGaugeWidgetIndices[0]);
    d3t::UIGaugeState *const enemy = FightGaugeWidget(g_fightGaugeWidgetIndices[1]);
    if (g_fightGaugeFrameFlags & 1U) {
        if (player)
            player->hidden = 0;
        if (enemy)
            enemy->hidden = 1;
    } else if (g_fightGaugeFrameFlags & 2U) {
        if (player)
            player->hidden = 0;
        if (enemy)
            enemy->hidden = 0;
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudGauge].config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    g_fightGaugeFrameFlags = 0;
    if (static_cast<std::uint32_t>(g_fightGaugeScissorIndices[1]) <= 9U)
        g_uiScissorWidgets[static_cast<std::size_t>(g_fightGaugeScissorIndices[1])].width = 0.0f;
    return 0;
}



std::int64_t ResetGlobalInput() {
    g_helpNewEmulatorContext = 0;
    g_helpNewEmulatorPrompt = 0;
    g_windowInput.lastKeyIndex = 0;
    ClearWindowInput();
    InitializeGlobalInput();
    UIBaseRuntime &watch = g_uiElements[kUiGameHudWatch];
    watch.isHandledExternally = 0;
    if (watch.elementDraw)
        watch.elementDraw();
    if (g_helpNewBackground)
        g_helpNewBackground->colour = {40, 40, 40, 200};
    return 1;
}


std::uint32_t SeventyManBattle_LoadUI() {
    UIBaseRuntime &element = g_uiElements[kUi70ManBattle];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/70ManBattle/70ManBattle.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}



std::int64_t SeventyManBattle_SetupUI() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUi70ManBattle].config.uiHandleIndex);
    if (slot > 0x4bU)
        return 0;
    for (std::size_t index = 0; index < kSeventyManPanelIds.size(); ++index) {
        const std::int32_t window = UI_FindViewWindow(slot, kSeventyManPanelIds[index]);
        d3t::UIWindow *const record = UI_GetWindow(window);
        g_70mbPanelWindows[index] = window;
        g_70mbPanelBindings[index] = UIWidget_Bind(window, record ? &record->alpha : nullptr, {0.0f, 0.0f}, {1.0f, 1.0f});
        if (record)
            record->alpha = 0.0f;
    }
    for (std::size_t index = 0; index < kSeventyManLabelIds.size(); ++index) {
        const std::int32_t label = UI_FindViewLabel(slot, kSeventyManLabelIds[index]);
        g_70mbLabels[index] = label;
        if (static_cast<std::uint32_t>(label) < g_uiLabelWidgets.size())
            g_uiLabelWidgets[static_cast<std::size_t>(label)].text = g_70mbLabelText[index].data();
    }
    return 1;
}




std::int64_t SeventyManBattle_Tick() {
    std::int32_t status = g_70mbShownIconStatus;
    if (g_70ManIconTransitionState != g_70mbShownIconStatus) {
        const std::size_t previous = static_cast<std::size_t>(g_70mbShownIconStatus);
        status = g_70ManIconTransitionState;
        g_70mbShownIconStatus = status;
        for (std::size_t index = 0; index < g_70mbPanelWindows.size(); ++index) {
            if (!UI_GetWindow(g_70mbPanelWindows[index]))
                continue;
            const std::uint8_t visible = kSeventyManPanelVisibility[static_cast<std::size_t>(status)][index];
            if (visible != kSeventyManPanelVisibility[previous][index])
                UIWidget_Animate(g_70mbPanelBindings[index], visible != 0, 0.3f, 0.0f);
        }
    }
    d3t::UIWindow *const overlay = UI_GetWindow(g_70mbPanelWindows[0]);
    if (status == 4) {
        if (d3t::UIWindow *const icon = UI_GetWindow(g_70mbPanelWindows[1]))
            icon->alpha = g_70mbStartPromptIconVisible ? 1.0f : 0.0f;
        if (overlay)
            overlay->alpha = 1.0f;
    } else {
        if (overlay)
            overlay->alpha = 0.0f;
        if (status == 1) {
            std::snprintf(g_70mbLabelText[0].data(), g_70mbLabelText[0].size(), "%u %s",
                static_cast<std::uint32_t>(static_cast<std::int8_t>(g_mfbtCurrentDefeatedEnemies)), InitAndRetrieveStringFromID(k70mbStringIds[0]));
            const VFMAData *const game = vfMainGetCurrent();
            const float seconds = static_cast<float>(game ? static_cast<std::int32_t>(game->runtimeTicks) : 0) / 30.0f;
            const float remainder = std::fmod(seconds, 60.0f);
            const float minutes = std::fmod(seconds / 60.0f, 60.0f);
            std::snprintf(g_70mbLabelText[1].data(), g_70mbLabelText[1].size(), "%02u:%05.2f", static_cast<unsigned>(static_cast<std::int32_t>(minutes)),
                static_cast<double>(remainder));
            std::snprintf(g_70mbLabelText[2].data(), g_70mbLabelText[2].size(), "%s", InitAndRetrieveStringFromID(k70mbStringIds[4]));
        }
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUi70ManBattle].config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}



std::int64_t SeventyManBattle_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    if (g_70mbShownIconStatus == 1)
        return 0;
    g_searchPromptIconSlots = {-1, 44, -1, static_cast<std::int16_t>(g_70mbShownIconStatus == 4 ? 63 : -1), -1};
    for (std::size_t slot = 0; slot < prompts.size(); ++slot)
        prompts[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(prompts.data());
}


std::uint32_t GameOver_LoadUI() {
    UIBaseRuntime &element = g_uiElements[kUiVfGameOver];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/GameOver/GameOver.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}


std::int64_t GameOver_Tick() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiVfGameOver].config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}


std::uint32_t PracticeHud_LoadUI() {
    UIBaseRuntime &element = g_uiElements[kUiGameHudPractice];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/gamehud/GameHudTraining.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}


std::int64_t PracticeHud_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiGameHudPractice];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}


std::int64_t PracticeHud_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    for (std::size_t slot = 0; slot < kPracticeHudPromptIcons.size(); ++slot)
        g_searchPromptIconSlots[slot] = kPracticeHudPromptIcons[slot];
    for (std::size_t slot = 0; slot < prompts.size(); ++slot)
        prompts[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(prompts.data());
}


std::int64_t GameBackground_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiGameMenuBackground];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}



std::int64_t GameMoveLearned_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiMoveLearned].config.uiHandleIndex);
    for (std::size_t line = 0; line < g_moveLearnedLabels.size(); ++line)
        g_moveLearnedLabels[line] = UI_LabelRecord(UI_FindViewLabel(slot, kEwzoUiLabelNames[line]));
    const std::array<const char *, 3> texts{
        g_moveLearnedPresentation.trainingText.data(), g_moveLearnedPresentation.moveNameText.data(), g_moveLearnedPresentation.resultText.data()};
    for (std::size_t line = 0; line < g_moveLearnedLabels.size(); ++line) {
        if (d3t::UILabelWidget *const label = g_moveLearnedLabels[line]) {
            label->text = texts[line];
            label->token.alreadyLocalized = 1;
        }
    }
    return 1;
}



std::int64_t GameMoveLearned_Tick() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiMoveLearned].config.uiHandleIndex);
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}


std::int64_t GameMoveLearned_Reset() {
    g_moveLearnedLabels = {};
    return 0;
}

std::int64_t HandleEwzoUiEvent(EwzoUiEvent event, const EwzoUiBinding &binding) {
    MoveLearnedPresentationOwner &popup = g_moveLearnedPresentation;
    switch (event) {
    case EwzoUiEvent::Activate:
        if (!popup.active) {
            popup.savedUiState = g_uiState;
        }
        popup.mode = binding.mode;
        popup.animationSetId = binding.animationSetId;
        popup.moveId = binding.moveId;
        popup.active = true;
        ActivateOwnedElement(kUiMoveLearned, "/ui/gamehud/GameHudMoveLearned.ui");
        break;
    case EwzoUiEvent::BindLabels:
        if (binding.trainingText)
            popup.trainingText = *binding.trainingText;
        if (binding.moveNameText)
            popup.moveNameText = *binding.moveNameText;
        if (binding.resultText)
            popup.resultText = *binding.resultText;
        break;
    case EwzoUiEvent::Close: {
        HideOwnedElement(kUiMoveLearned);
        if (!popup.active)
            break;
        std::uint16_t restored = popup.savedUiState;
        const std::uint16_t current = g_uiState;
        if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20) != 0) {
            restored &= 0xfffeU;
        }
        if ((current & 0x200U) != 0 && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive != 0) {
            restored |= 0x200U;
        }
        g_currentSicn = UISicnPausePolicy::KeepOnPause;
        g_uiState = restored;
        if (restored != 0) {
            EnsureSicnTask();
        } else {
            UI_SICNClearState(0x0fffU);
            DestroyOwnedTask(g_sicnTask, SHEN_NAME(SICN));
            g_sicnClearPendingUntilUnpause = false;
        }
        popup = {};
        break;
    }
    }
    return 1;
}



std::uint32_t RaceHud_ParseDefinition() {
    UIBaseRuntime &element = g_uiElements[kUiGameHudRace];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 1;
        constexpr char path[] = "/ui/gamehud/GameHudRace.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}





std::int64_t RaceHud_IsSelected() {
    AtckNativeHudState &hud = ATCK;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameHudRace].config.uiHandleIndex);
    hud.dialMaterialA = UI_FindViewImage(slot, kRaceHudDialImages[0]);
    hud.field1e4[0] = UI_FindViewImage(slot, kRaceHudDialImages[1]);
    hud.field1e4[1] = UI_FindViewImage(slot, kRaceHudDialImages[2]);
    for (std::size_t index = 0; index < kRaceHudTextImages.size(); ++index)
        hud.field48[index] = UI_FindViewImage(slot, kRaceHudTextImages[index]);
    for (std::size_t index = 0; index < kRaceHudSprites.size(); ++index) {
        const std::int32_t sprite = UI_FindViewIconWidget(20, slot, kRaceHudSprites[index]);
        hud.nodes[index + 1] = static_cast<std::uint32_t>(sprite) <= 0xffU ? &g_uiButtonIconWidgets[static_cast<std::size_t>(sprite)] : nullptr;
    }
    for (std::size_t index = 0; index < kRaceHudInfoWindows.size(); ++index)
        hud.alphaNodeIndices[index] = UI_FindViewWindow(slot, kRaceHudInfoWindows[index]);
    const std::int32_t sheet = UI_FindSpriteSheetByImageName("RaceText_En");
    hud.nodes[0] = static_cast<std::uint32_t>(sheet) <= 0x37U ? &g_uiSpriteSheets[static_cast<std::size_t>(sheet)] : nullptr;
    const std::int32_t label = UI_FindViewLabel(slot, "RaceSpeedLabel");
    d3t::UILabelWidget *const speed = static_cast<std::uint32_t>(label) <= 0x1ffU ? &g_uiLabelWidgets[static_cast<std::size_t>(label)] : nullptr;
    hud.raceSpeedLabel = speed;
    if (speed)
        speed->text = hud.unknownString;
    hud.gameHudTopInfoParent = UI_FindViewWindow(slot, "TopInfoParent");
    hud.gameHudTopLeftInfoParent = UI_FindViewWindow(slot, "TopLeftInfoParent");
    hud.gameHudDial = UI_FindViewImageWidget(slot, "Dial");
    hud.gameHudRaceDialHand = UI_FindViewImageWidget(slot, "RaceDialHand");
    return 1;
}




std::int64_t RaceHud_Tick() {
    AtckNativeHudState &hud = ATCK;
    if (hud.dialPreset == 1) {
        g_xInputReserved1 = 1;
        g_forceXInputProcessing = 1;
    }
    const float current = hud.currentDialValue;
    const UIBaseRuntime &element = g_uiElements[kUiGameHudRace];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    if (slot <= 0x4bU && static_cast<std::uint32_t>(hud.gameHudRaceDialHand) < 0x200U) {
        g_uiImageWidgets[static_cast<std::size_t>(hud.gameHudRaceDialHand)].angle =
            (hud.currentDialValue - hud.previousDialValue) / (hud.dialTargetValue - hud.previousDialValue) * 1.43f + 0.0f;
    }
    std::snprintf(hud.unknownString, sizeof(hud.unknownString), "%i", static_cast<std::int32_t>(current));
    if (d3t::UILabelWidget *const speed = static_cast<d3t::UILabelWidget *>(hud.raceSpeedLabel))
        speed->token.token = hud.unknownString;
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}



void RaceHud_Render() {
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = English;
        ReadSubtitleGlyphsAndSubs(English);
    }
    if (d3t::UISpriteSheet *const sheet = static_cast<d3t::UISpriteSheet *>(ATCK.nodes[0])) {
        sheet->imageName = nullptr;
        sheet->image = ATCK.field48[g_uiLoadedLanguage];
    }
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {


void FormatTop5TimesText() {
    for (std::uint32_t index = 3; index < 8; ++index) {
        const float seconds = static_cast<float>(g_70mbBestTimesTicks[index - 3]) / 30.0f;
        const float remainder = std::fmod(seconds, 60.0f);
        const float minutes = std::fmod(seconds / 60.0f, 60.0f);
        std::snprintf(g_70mbLabelText[index].data(), g_70mbLabelText[index].size(), "#%u          %02u:%05.2f", index - 2,
            static_cast<unsigned>(static_cast<std::int32_t>(minutes)), static_cast<double>(remainder));
    }
}

// EnqueueVFGI's caller-owned sentinel reset before MFBT_Init.
void MFBT_ResetSurfaceIds() {
    for (MFBTSurface &surface : g_seventyManBattlePresentation.surfaces)
        surface.surface = -1;
}


// @verified - p
void MFBT_Init(std::uint32_t *bestTimes, std::uint32_t &defeatedEnemies) {
    SeventyManBattlePresentationOwner &presentation = g_seventyManBattlePresentation;
    defeatedEnemies = 0;
    std::array<std::uint32_t, 4> saved{};
    std::memcpy(saved.data(), g_playerStats.pages.save70MBSamples, sizeof(saved));
    if (saved[0] > 100U) {
        for (std::size_t index = 0; index < 4; ++index)
            bestTimes[index] = saved[index];
        bestTimes[4] = g_playerStats.pages.bestTimeTicks;
    } else {
        for (std::size_t index = 0; index < 5; ++index)
            bestTimes[index] = static_cast<std::uint32_t>((index + 1) * 18000);
    }
    FormatTop5TimesText();
    for (std::size_t index = 0; index < kMFBTSurfaceWidths.size(); ++index) {
        MFBTSurface &surface = presentation.surfaces[index];
        surface.width = kMFBTSurfaceWidths[index];
        surface.height = 32;
        if (surface.surface != -1)
            PVR_FreeAllocatedSurfaceByIndex(surface.surface);

        surface.surface = AllocatePVRSurface(".", surface.width, 0xffff, 0xf000, 0, true);
    }
    const std::int32_t fileSize = static_cast<std::int32_t>(shenOpenFile("sprite", "LOADNUM.SPR"));
    std::byte *const file = static_cast<std::byte *>(HLib_AllocHeapMemBlock(fileSize, static_cast<std::int32_t>(kMFBTDigitAllocationToken.name)));
    shenLoadFile("sprite", "LOADNUM.SPR", file, fileSize);
    if (file) {
        std::byte *cursor = file;
        std::size_t index = 0;
        while (cursor < file + fileSize) {
            std::uint32_t kind = 0;
            std::memcpy(&kind, cursor, sizeof(kind));
            if (!kind)
                break;
            presentation.digitSurfaces[index++] = LoadTEXN(cursor);
            std::uint32_t size = 0;
            std::memcpy(&size, cursor + 4, sizeof(size));
            cursor += size;
        }
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
}


void MFBT_ReleaseResources() {
    SeventyManBattlePresentationOwner &presentation = g_seventyManBattlePresentation;
    for (const std::int32_t surface : presentation.digitSurfaces)
        PVR_FreeAllocatedSurfaceByIndex(surface);
    for (MFBTSurface &surface : presentation.surfaces) {
        if (surface.surface == -1)
            continue;
        PVR_FreeAllocatedSurfaceByIndex(surface.surface);
        surface.surface = -1;
    }
    g_helpIconWidth = 0;
    g_helpIconHeight = 0;
}


// @verified - p
void AdjustAspectRatioMatrixHorizontalStretchAdjusted(std::int32_t num, std::int32_t horizontalPosition, std::int32_t verticalPosition) {
    if (!g_updateRenderFrameMode)
        return;
    const MFBTSurface &surface = g_seventyManBattlePresentation.surfaces[static_cast<std::size_t>(num)];
    if (surface.surface == -1)
        return;
    OriginalStrip strip{};
    strip.corner0 = 0xffffffffU;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    AdjustAspectRatioMatrix(&strip, 5, horizontalPosition, verticalPosition, surface.width, surface.height, 4);
}

// @verified - p
void MFBT_DrawElapsedTime(std::uint32_t currentTicks, std::int32_t horizontalPosition, std::int32_t verticalPosition) {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{};
    strip.flags = 0x400;
    strip.corner0 = 0xffffffffU;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    AdjustAspectRatioMatrixHorizontalStretchAdjusted(1, horizontalPosition + 74, verticalPosition);
    AdjustAspectRatioMatrixHorizontalStretchAdjusted(1, horizontalPosition + 162, verticalPosition);
    std::uint32_t remainder = 100U * (currentTicks <= 108000U ? currentTicks : 108000U);
    for (std::size_t index = 0; index < kMFBTElapsedDigitLayout.size(); ++index) {
        remainder %= kMFBTElapsedDigitRemainders[index];
        const std::array<std::uint16_t, 3> &layout = kMFBTElapsedDigitLayout[index];
        AdjustAspectRatioMatrix(&strip, 5, horizontalPosition + layout[0], verticalPosition + layout[1], layout[2], layout[2], 4);
    }
}


// @verified - p
void shenComputeNumEnemies(std::uint32_t currentTicks, std::uint32_t defeatedEnemies) {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{};
    strip.flags = 0x400;
    strip.corner0 = 0xffffffffU;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    std::int32_t enemies = static_cast<std::int32_t>(defeatedEnemies);
    if (enemies > 999)
        enemies = 999;
    std::int32_t skipped = 0;
    if (enemies / 10 / 10 % 10 <= 0)
        skipped = (enemies / 10 % 10 <= 0) + 1;
    std::int32_t position = 32 * skipped + 120;
    do {
        AdjustAspectRatioMatrix(&strip, 5, position, 432, 32, 32, 4);
        position += 32;
    } while (position < 216);
    MFBT_DrawElapsedTime(currentTicks, 420, 432);
    if (g_updateRenderFrameMode) {
        const MFBTSurface &surface = g_seventyManBattlePresentation.surfaces[0];
        if (surface.surface != -1) {
            strip.flags = 0;
            AdjustAspectRatioMatrix(&strip, 5, 340, 432, surface.width, surface.height, 4);
        }
    }
}





void UI_VFUpdateHealthPercentage(float healthPercentage, std::int32_t gaugeStatus, float pulsePosition) {
    g_vfGaugeHealthPercent = healthPercentage;
    g_fightGaugeFrameFlags = (g_fightGaugeFrameFlags & 0xfffffffcU) | 1U;
    d3t::UIGaugeState *const player = FightGaugeWidget(g_fightGaugeWidgetIndices[0]);
    if (!player)
        return;
    const float filled = static_cast<float>(player->count) * healthPercentage;
    const std::int32_t whole = static_cast<std::int32_t>(filled);
    player->filledCount = whole;
    if (filled - static_cast<float>(whole) > 0.0f && whole < player->count)
        player->filledCount = whole + 1;
    if (g_fightGaugeDisplayMode != 1)
        return;
    const std::int32_t first = g_fightGaugeWidgetIndices[0];
    if (d3t::UIGaugeState *const cleared = first < 2 ? FightGaugeWidget(g_fightGaugeWidgetIndices[static_cast<std::size_t>(first)]) : nullptr)
        cleared->orbColours.fill(0);
    if (gaugeStatus == 2) {
        const std::int32_t yellow = 3;
        if (player->filledCount > 0)
            player->orbColours[static_cast<std::size_t>(player->filledCount - 1)] = yellow;
        else if (player->filledCount == 0)
            std::memcpy(&player->height, &yellow, sizeof(yellow));
    }
    player->lastOrbAlpha = pulsePosition;
}

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



std::uint32_t LoadGameHudUI() {
    if (g_gameHudSkipUILoadState == 0) {
        g_gameHudSkipUIFileHandle = OpenLegacyUIFile("/ui/gamehud/GameHudSkip.ui", kUiGameHudSkip);
        g_gameHudSkipUILoadState = 1;
        return 1;
    }
    if (g_gameHudSkipUILoadState == 1) {
        g_gameHudSkipUILoadState = TryLoadUIFromFileHandle(kUiGameHudSkip, g_gameHudSkipUIViewSlot);
    } else if (g_gameHudSkipUILoadState == 2) {
        const bool finalized =
            static_cast<std::uint32_t>(g_gameHudSkipUIViewSlot) < d3t::kUIViewSlotCount && TryFinalizeUIViewTextures(g_gameHudSkipUIViewSlot) == 1;
        g_gameHudSkipUILoadState = finalized ? 3 : 2;
    }
    return static_cast<std::uint32_t>(g_gameHudSkipUILoadState);
}


std::int64_t GameHudSkip_LoadState() {
    return g_gameHudSkipUILoadState;
}





std::int64_t GameHudSkip_PostInit() {
    if (g_gameHudSkipUILoadState == 3 || (LoadGameHudUI(), static_cast<std::uint32_t>(g_gameHudSkipUILoadState - 2) <= 1U)) {
        const std::uint64_t frame = d3t::GetNumRenderedFrames();
        if (g_uiLegacyDrawFrame < frame)
            g_uiLegacyDrawFrame = frame;
        GNuklearWindow.style.window.fixed_background = nk_style_item{};
        if (static_cast<std::uint32_t>(g_gameHudSkipUIViewSlot) <= 75U) {
            const std::size_t slot = static_cast<std::size_t>(g_gameHudSkipUIViewSlot);
            UI_InputHandler(GNuklearWindow, g_gameHudSkipUIViewSlot, 0, g_uiViewSlots[slot].widgetCount);
        }
    }
    return 0;
}



std::int64_t GameHudSkip_ReloadUI() {
    ResetUIViewSlot(g_gameHudSkipUIViewSlot);
    g_gameHudSkipUIViewSlot = -1;
    g_gameHudSkipUILoadState = 0;
    g_gameHudSkipUIFileHandle = 0;
    LoadUIFromMemoryIntoViewSlot(g_uiReloadFileMemory, g_gameHudSkipUIViewSlot);
    g_gameHudSkipUILoadState = 2;
    return 0;
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

// icon changed (to anything but 0xffff), or whose pulse is still running, is flagged (bit 16); a newly flagged slot starts its pulse and
// the shared highlight, as hudSetPromptIconOverrideSet does for the override set. Without a set, the effective icons are cleared.
void SRCH_ApplyPromptIconTypeSet(const std::int32_t *prompts) {
    if (!prompts) {
        for (std::int32_t &slot : g_hudPromptEffectiveSlots)
            slot = static_cast<std::int32_t>(static_cast<std::uint32_t>(slot) | 0xffffU);
        return;
    }
    for (std::size_t index = 0; index < g_hudPromptRequestedSlots.size(); ++index)
        g_hudPromptRequestedSlots[index] = prompts[index];
    for (std::size_t index = 0; index < g_hudPromptEffectiveSlots.size(); ++index) {
        std::int32_t value = g_hudPromptRequestedSlots[index];
        const std::int32_t previous = g_hudPromptEffectiveSlots[index];
        const std::int32_t pulse = g_hudPromptIconAnimSlots[index];
        const auto icon = static_cast<std::uint16_t>(value);
        if (((previous & 0x10000) != 0 && g_uiAnimSlots[static_cast<std::size_t>(pulse)].status != 0) ||
            (icon != static_cast<std::uint16_t>(previous) && icon != 0xffffU))
            value |= 0x10000;
        if ((value & 0x10000) != 0 && g_uiAnimSlots[static_cast<std::size_t>(pulse)].status == 0) {
            if (pulse != -1)
                HudPromptStartIconPulse(pulse);
            if (g_uiAnimSlots[static_cast<std::size_t>(g_hudPromptHighlightAnimSlot)].status == 0 && g_hudPromptHighlightAnimSlot != -1)
                HudPromptStartHighlight(g_hudPromptHighlightAnimSlot);
        }
        g_hudPromptEffectiveSlots[index] = value;
    }
}



void SRCH_ApplySearchPromptSlots() {
    const std::array<std::int16_t, 5> &slots = SRCH_GetPromptIconSlots();
    const std::array<std::int32_t, 5> prompts{static_cast<std::uint16_t>(slots[0]), static_cast<std::uint16_t>(slots[1]), static_cast<std::uint16_t>(slots[2]),
        static_cast<std::uint16_t>(slots[3]), static_cast<std::uint16_t>(slots[4])};
    SRCH_ApplyPromptIconTypeSet(prompts.data());
}

} // namespace shenmue::s1
