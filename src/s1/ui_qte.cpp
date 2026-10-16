#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {

void EnsureSfxOverlayTask() {
    if (!g_sfxOverlayTask)
        EnsureIconRootTask();
}

std::int16_t AddSfxOverlayFrames(std::int16_t lhs, std::int16_t rhs) {
    return static_cast<std::int16_t>(static_cast<std::uint16_t>(lhs) + static_cast<std::uint16_t>(rhs));
}

std::uint8_t SfxOverlayModeFlags(std::uint8_t mode) {
    switch (mode) {
    case 2:
        return kSfxPlayPrimingCue | kSfxShowOverlay | kSfxWaitForExternalClose;
    case 3:
        return kSfxPlayPrimingCue | kSfxShowOverlay | kSfxAlternateResultSound;
    case 4:
        return kSfxPlayPrimingCue | kSfxShowOverlay | kSfxWaitForExternalClose | kSfxCloseImmediately;
    case 5:
        return kSfxPlayPrimingCue | kSfxShowOverlay | kSfxWaitForExternalClose | kSfxCloseImmediately | kSfxSubsetMaskMode;
    default:
        return kSfxPlayPrimingCue | kSfxShowOverlay;
    }
}

std::uint16_t PrioritizeSfxExpectedMask(std::uint16_t mask) {
    for (std::uint8_t bit = 0; bit < 12; ++bit) {
        const std::uint16_t candidate = static_cast<std::uint16_t>(1U << bit);
        if ((mask & candidate) != 0)
            return candidate;
    }
    return 0x1000;
}

// SfxOverlay_HandlePromptInputAndTimeout and its subset form read ICON's pressed-priority copy.
std::uint16_t SfxOverlayPressedInput() {
    return static_cast<std::uint16_t>(g_sfxInputFlags);
}

std::uint16_t MapSfxDirectionalInput(std::uint16_t pressed) {
    return kSfxMaskLut[(pressed >> 12U) & 0x0fU];
}

// The overlay's result cue: UiAccept for an answer and UiPromptFailOrTimeout otherwise, swapped when the alternate-result flag is set
// with the priming cue.
void PlaySfxOverlayResultSound(bool success) {
    const bool alternate = (g_sfxOverlayFlags & (kSfxPlayPrimingCue | kSfxAlternateResultSound)) == (kSfxPlayPrimingCue | kSfxAlternateResultSound);
    shenExecuteAICACommand(success != alternate ? 0x900abU : 0xa00abU, 0, 0);
}



void AdvanceSfxOverlayFlash() {
    if (g_sfxOverlayState != UISfxOverlayState::Active)
        return;
    ++g_sfxFlashCounter;
    if (g_sfxStateDirty == 1) {
        if (static_cast<std::int8_t>(g_sfxFlashCounter) < g_sfxFlashOnFrames) {
            if (g_sfxFlashCounter == 1 && (g_sfxOverlayFlags & kSfxPlayPrimingCue) != 0)
                shenExecuteAICACommand(0x700abU, 0, 0);
        } else {
            g_sfxStateDirty = 0;
            g_sfxFlashCounter = 0;
        }
    } else if (static_cast<std::int8_t>(g_sfxFlashCounter) >= g_sfxFlashOffFrames) {
        g_sfxStateDirty = 1;
        g_sfxFlashCounter = 0;
    }
}

// Both prompt handlers' timeout: unless the timeout is 0x80 (none), the frame count runs past it and the overlay resolves with
// 0xA000, which also becomes the timed-out marker ICQu's close reads.
void TickSfxOverlayTimeout() {
    if (g_sfxTimeoutFrames == 0x80)
        return;
    g_sfxFrameCounter = AddSfxOverlayFrames(g_sfxFrameCounter, 1);
    if (g_sfxFrameCounter <= g_sfxTimeoutFrames)
        return;
    g_sfxOverlayState = UISfxOverlayState::Resolved;
    g_sfxOverlayResult[0] = static_cast<std::int16_t>(0xa000U);
    g_sfxTimedOutMarker = static_cast<std::int32_t>((static_cast<std::uint32_t>(g_sfxTimedOutMarker) & 0xffff0000U) | 0xa000U);
    g_sfxTimeoutFrames = 0;
    PlaySfxOverlayResultSound(false);
    UpdateAveragedRuntimeStatPageSlot(1, g_sfxFrameCounter);
    g_sfxStateDirty = 1;
}




void HandleSfxOverlayExactInput(std::uint16_t pressed) {
    std::uint16_t inputMask = MapSfxDirectionalInput(pressed);
    std::uint8_t faceButtonCount = 0;
    if ((pressed & 0x400U) != 0) {
        inputMask |= 0x100U;
        ++faceButtonCount;
    }
    if ((pressed & 0x200U) != 0) {
        inputMask |= 0x200U;
        ++faceButtonCount;
    }
    if ((pressed & 0x002U) != 0) {
        inputMask |= 0x800U;
        ++faceButtonCount;
    }
    if ((pressed & 0x004U) != 0) {
        inputMask |= 0x400U;
        ++faceButtonCount;
    }
    if ((inputMask & 0x0f00U) == 0) {
        switch (g_currentSfxMask) {
        case 0x001:
            if ((pressed & 0x1000U) != 0)
                inputMask = 0x001;
            break;
        case 0x010:
            if ((pressed & 0x2000U) != 0)
                inputMask = 0x010;
            break;
        case 0x040:
            if ((pressed & 0x4000U) != 0)
                inputMask = 0x040;
            break;
        case 0x004:
            if ((pressed & 0x8000U) != 0)
                inputMask = 0x004;
            break;
        default:
            break;
        }
    }
    if (VFMA_IsPlayerGodmodeForced())
        inputMask = g_currentSfxMask;
    if (inputMask) {
        g_sfxOverlayResult[0] = static_cast<std::int16_t>(inputMask);
        bool success = false;
        if (faceButtonCount >= 2 || ((inputMask & 0x0f00U) != 0 && (inputMask & 0x00ffU) != 0)) {
            PlaySfxOverlayResultSound(false);
            g_sfxOverlayResult[0] = 0;
        } else if (inputMask == g_currentSfxMask) {
            g_sfxOverlayResult[0] = static_cast<std::int16_t>(inputMask | 0x2000U);
            PlaySfxOverlayResultSound(true);
            success = true;
        } else {
            g_sfxOverlayResult[0] = 0;
            PlaySfxOverlayResultSound(false);
        }
        UpdateAveragedRuntimeStatPageSlot(success ? 0 : 1, g_sfxFrameCounter);
        g_sfxOverlayState = UISfxOverlayState::Resolved;
        g_sfxStateDirty = 1;
    } else {
        TickSfxOverlayTimeout();
    }
    AdvanceSfxOverlayFlash();
}



void HandleSfxOverlaySubsetInput(std::uint16_t pressed) {
    std::uint16_t inputMask = MapSfxDirectionalInput(pressed);
    if ((g_currentSfxMask & 0x040U) != 0) {
        inputMask &= 0x00eeU;
        if ((pressed & 0x4000U) != 0)
            inputMask = 0x040;
        if ((pressed & 0x8000U) != 0)
            inputMask = 0x004;
    } else {
        inputMask &= 0x00bbU;
        if ((pressed & 0x1000U) != 0)
            inputMask = 0x001;
        if ((pressed & 0x2000U) != 0)
            inputMask = 0x010;
    }
    if (VFMA_IsPlayerGodmodeForced())
        inputMask = g_currentSfxMask;
    if ((g_currentSfxMask & inputMask) != 0) {
        g_sfxOverlayResult[0] = static_cast<std::int16_t>(inputMask | 0x2000U);
        PlaySfxOverlayResultSound(true);
        g_sfxOverlayState = UISfxOverlayState::Resolved;
        UpdateAveragedRuntimeStatPageSlot(0, g_sfxFrameCounter);
    } else {
        TickSfxOverlayTimeout();
    }
    AdvanceSfxOverlayFlash();
}

// STATIC_QTE_MiniGame_parseDefinitionFromFile.
std::uint32_t QteMinigame_ParseDefinition() {
    return LoadPresentationDefinition(static_cast<std::int32_t>(kQteMinigameUiElement), "/ui/QTEMinigame/QTEMinigame.ui");
}


std::int64_t QteMinigame_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kQteMinigameUiElement].config.uiHandleIndex);
    for (std::size_t index = 0; index < kQteMinigameIndicatorObjectNames.size(); ++index)
        g_qteMinigameIndicatorWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, kQteMinigameIndicatorObjectNames[index]));
    for (std::size_t index = 0; index < kQteMinigameKeyIconIds.size(); ++index)
        g_qteMinigameKeyIcons[index] = UI_ButtonIconRecord(UI_FindViewIconWidget(21, slot, kQteMinigameKeyIconIds[index]));
    for (std::size_t index = 0; index < kQteMinigamePadIconIds.size(); ++index)
        g_qteMinigamePadIcons[index] = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, kQteMinigamePadIconIds[index]));
    return 1;
}


std::int64_t QteMinigame_Reset() {
    for (d3t::UIWindow *&window : g_qteMinigameIndicatorWindows)
        window = nullptr;
    return 0;
}


void QteMinigame_Render() {
    GameUtils_ClearTitlIndicators();
    g_qteMinigameIndicatorModel = nullptr;
    g_gameUtilsTitlIndicatorEnabledPtr = nullptr;
}

// STATIC_QTE_MiniGame_Destroy.
std::int64_t QteMinigame_Destroy() {
    QteMinigame_Render();
    return 1;
}



void QteMinigame_SelectPromptIcons(bool pad) {
    const std::uint32_t keyAlpha = pad ? 0U : 0xff000000U;
    const std::uint32_t padAlpha = pad ? 0xff000000U : 0U;
    for (UIButtonIconWidget *const icon : g_qteMinigameKeyIcons) {
        if (!icon)
            continue;
        icon->colour = (icon->colour & 0x00ffffffU) | keyAlpha;
        icon->fontColour = (icon->fontColour & 0x00ffffffU) | keyAlpha;
    }
    for (UIButtonIconWidget *const icon : g_qteMinigamePadIcons) {
        if (!icon)
            continue;
        icon->colour = (icon->colour & 0x00ffffffU) | padAlpha;
        icon->fontColour = (icon->fontColour & 0x00ffffffU) | padAlpha;
    }
}



std::int64_t QteMinigame_Tick() {
    QteMinigame_SelectPromptIcons(g_keyboardActive != 1);
    GameUtils_UpdateTitlIndicators();
    const UIBaseRuntime &element = g_uiElements[kQteMinigameUiElement];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    GameUtils_ClearTitlIndicators();
    g_qteMinigameIndicatorModel = nullptr;
    g_gameUtilsTitlIndicatorEnabledPtr = nullptr;
    return 0;
}


std::uint32_t GameQte_ParseDefinition() {
    return LoadPresentationDefinition(kUiGameQte, "/ui/gameqte/GameQTE.ui");
}



std::int64_t GameQte_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameQte].config.uiHandleIndex);
    g_gameQteIcons[0] = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, "DefaultIconSheet"));
    g_gameQteIcons[2] = UI_ButtonIconRecord(UI_FindViewIconWidget(21, slot, "PlatformIconSheet"));
    g_gameQteIcons[1] = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, "MouseIconSheet"));
    constexpr std::array<const char *, 3> kPanels{"DefaultPanel", "MousePanel", "PlatformPanel"};
    for (std::size_t index = 0; index < kPanels.size(); ++index) {
        d3t::UIWindow *const window = UI_GetWindow(UI_FindViewWindow(slot, kPanels[index]));
        g_gameQtePanelOffsets[index] = window ? window->layout.offsetX : 0.0f;
        g_gameQteWindows[kGameQteDefaultPanel + index] = window;
    }
    std::array<char, 32> name{};
    for (std::size_t index = 0; index < kGameQteDirections.size(); ++index) {
        std::snprintf(name.data(), name.size(), "%s%s%s", "MultiPanel_", "Mouse_", kGameQteDirections[index]);
        g_gameQteWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, name.data()));
    }
    for (std::size_t index = 0; index < kGameQteDirections.size(); ++index) {
        std::snprintf(name.data(), name.size(), "%s%s%s", "MultiPanel_", "Default_", kGameQteDirections[index]);
        g_gameQteWindows[8 + index] = UI_GetWindow(UI_FindViewWindow(slot, name.data()));
    }
    g_gameQteAnimSlot = UIAnimAllocSlot();
    const std::uint32_t black = ConstructColour(0, 0, 0, 255);
    for (UIButtonIconWidget *const icon : g_gameQteIcons) {
        if (icon)
            icon->fontColour = black;
    }
    return 1;
}

bool GameQte_SlideOutRunning() {
    return g_gameQteAnimSlot >= 0 && g_uiAnimSlots[static_cast<std::size_t>(g_gameQteAnimSlot)].status != 0;
}



std::int64_t GameQte_Tick() {
    const std::uint64_t frame = d3t::GetNumRenderedFrames();
    if (g_uiLegacyDrawFrame < frame)
        g_uiLegacyDrawFrame = frame;
    if (GameQte_SlideOutRunning()) {
        const float offset = g_uiAnimSlots[static_cast<std::size_t>(g_gameQteAnimSlot)].current[0];
        for (std::size_t index = kGameQteDefaultPanel; index <= kGameQtePlatformPanel; ++index) {
            if (g_gameQteWindows[index])
                g_gameQteWindows[index]->layout.offsetX = offset;
        }
    }
    if (!g_gameQteHidden) {
        const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameQte].config.uiHandleIndex);
        if (slot <= 0x4bU)
            UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    }
    return 0;
}



void GameQte_StartSlideOut() {
    if (g_gameQteAnimSlot != -1 && !GameQte_SlideOutRunning())
        UIAnim_Start(g_gameQteAnimSlot, g_gameQtePanelOffsets[0], -0.6f, 0.25f, 0.0f);
}



void GameQte_ShowDirections(std::uint16_t mask, bool pad) {
    const std::size_t first = pad ? 8 : 0;
    for (std::size_t window = first; window < first + 7; ++window) {
        if (g_gameQteWindows[window] && ((mask >> (window - first)) & 1U) != 0)
            g_gameQteWindows[window]->alpha = 1.0f;
    }
}


void GameQte_HideDirections(bool defaultSet) {
    const std::size_t first = defaultSet ? 8 : 0;
    for (std::size_t window = first; window < first + 7; ++window) {
        if (g_gameQteWindows[window])
            g_gameQteWindows[window]->alpha = 0.0f;
    }
}





void GameQte_ShowMask(std::uint16_t mask, std::uint8_t modeFlags) {
    if (g_uiElements[kUiGameQte].config.init != 3)
        return;
    const auto setAlpha = [](std::size_t window, float alpha) {
        if (g_gameQteWindows[window])
            g_gameQteWindows[window]->alpha = alpha;
    };
    const auto showPanel = [&setAlpha](std::size_t window) {
        setAlpha(kGameQteDefaultPanel, 0.0f);
        setAlpha(kGameQteMousePanel, 0.0f);
        setAlpha(kGameQtePlatformPanel, 0.0f);
        setAlpha(window, 1.0f);
    };
    if ((modeFlags & kSfxSubsetMaskMode) != 0) {
        const bool pad = g_keyboardActive != 1;
        GameQte_ShowDirections(mask, pad);
        GameQte_HideDirections(!pad);
        setAlpha(kGameQteDefaultPanel, 0.0f);
        setAlpha(kGameQteMousePanel, 0.0f);
        setAlpha(kGameQtePlatformPanel, 0.0f);
        return;
    }
    for (std::size_t window = 0; window < 16; ++window)
        setAlpha(window, 0.0f);
    g_gameQteMask = mask;
    if (UIButtonIconWidget *const platform = g_gameQteIcons[2]) {
        const std::int32_t action = mask == 0x100 ? 0 : mask == 0x200 ? 1 : mask == 0x400 ? 2 : mask == 0x800 ? 3 : -1;
        if (action >= 0) {
            showPanel(kGameQtePlatformPanel);
            platform->action = action;
            return;
        }
        showPanel(kGameQteDefaultPanel);
    }
    UIButtonIconWidget *const icons = g_gameQteIcons[0];
    if (!icons)
        return;
    std::int32_t frame = -1;
    switch (mask) {
    case 0x001:
        frame = 7;
        break;
    case 0x002:
        frame = 11;
        break;
    case 0x004:
        frame = 5;
        break;
    case 0x008:
        frame = 9;
        break;
    case 0x010:
        frame = 6;
        break;
    case 0x020:
        frame = 10;
        break;
    case 0x040:
        frame = 4;
        break;
    case 0x080:
        frame = 8;
        break;
    case 0x100:
        frame = 1;
        break;
    case 0x200:
        frame = 2;
        break;
    case 0x400:
        frame = 3;
        break;
    case 0x800:
        frame = 0;
        break;
    default:
        return;
    }
    showPanel(kGameQteDefaultPanel);
    icons->sprite = frame;
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {




void UI_SFXOverlayBegin(const UISfxOverlayRequest &request) {
    g_sfxOverlayFlags = SfxOverlayModeFlags(request.mode);
    g_sfxFlashOnFrames = 3;
    g_sfxFlashOffFrames = 2;
    std::int16_t timeoutFrames = request.timeoutFrames;
    if (request.mode != 4 && timeoutFrames != 0x7fff)
        timeoutFrames = AddSfxOverlayFrames(timeoutFrames, static_cast<std::int16_t>(request.bonusFrames));
    if (timeoutFrames <= 0)
        timeoutFrames = 1;
    if (timeoutFrames != 0x7fff && g_sfxOverlayTimeoutBoostEnabled && g_sfxOverlayTimeoutBoostCount >= 1) {
        if (g_sfxOverlayTimeoutPendingBonusFrames <= -1) {
            timeoutFrames = AddSfxOverlayFrames(timeoutFrames, g_qteOverlay.accumulatedFrames);
        } else {
            timeoutFrames = AddSfxOverlayFrames(timeoutFrames, g_sfxOverlayTimeoutPendingBonusFrames);
            g_sfxOverlayTimeoutPendingBonusFrames = -1;
        }
        if (timeoutFrames >= g_qteOverlayTimeoutMaxFrames)
            timeoutFrames = g_qteOverlayTimeoutMaxFrames;
        if (timeoutFrames < 1)
            timeoutFrames = 1;
    }
    g_sfxTimeoutFrames = timeoutFrames;
    g_sfxFrameCounter = 0;
    g_sfxOverlayResult[0] = -1;
    g_sfxOverlayState = UISfxOverlayState::Requested;
    g_currentSfxMask = (g_sfxOverlayFlags & kSfxSubsetMaskMode) != 0 ? request.expectedMask : PrioritizeSfxExpectedMask(request.expectedMask);
    g_sfxStateDirty = 1;
    g_sfxFlashCounter = 0;
    for (std::size_t index = 0; index < g_gameQtePanelOffsets.size(); ++index) {
        if (g_gameQteWindows[kGameQteDefaultPanel + index])
            g_gameQteWindows[kGameQteDefaultPanel + index]->layout.offsetX = g_gameQtePanelOffsets[index];
    }
    if (g_gameQteAnimSlot >= 0) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_gameQteAnimSlot)];
        slot.status = 0;
        slot.elapsed = 0.0f;
        slot.allocated = 1;
    }
}

void UI_SFXOverlayBegin(std::int16_t timeoutFrames) {
    UI_SFXOverlayBegin({0, timeoutFrames, 0, 0});
}

void UI_SFXOverlayResolve(std::int16_t result) {
    g_sfxOverlayState = UISfxOverlayState::Resolved;
    g_sfxOverlayResult[0] = result;
}


bool UI_SFXOverlayConsumeResultWhileOpen(std::int16_t &result) {
    if (g_sfxOverlayState == UISfxOverlayState::Idle)
        return false;
    result = g_sfxOverlayResult[0];
    g_sfxOverlayResult[0] = -1;
    return true;
}


bool UI_SFXOverlayConsumeResultIfIdle(std::int16_t &result) {
    if (g_sfxOverlayState != UISfxOverlayState::Idle)
        return false;
    result = g_sfxOverlayResult[0];
    g_sfxOverlayResult[0] = -1;
    return true;
}




void UI_SFXOverlayCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    if (g_sfxOverlayState == UISfxOverlayState::Opening) {
        g_sfxOverlayState = UISfxOverlayState::Active;
    } else if (g_sfxOverlayState == UISfxOverlayState::Requested) {
        if ((g_sfxOverlayFlags & kSfxPlayPrimingCue) != 0)
            shenExecuteAICACommand(0x700abU, 0, 0);
        g_isPlayerGameStateActive = 2;
        CapturePad(icon.input, icon.previousInput);
        icon.iconFlags |= 8U;
        g_sfxTimedOutMarker = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_sfxTimedOutMarker) | 0xffffU);
        g_sfxOverlayState = UISfxOverlayState::Opening;
        icon.overlayMode = 11;
        g_sfxStateDirty = 1;
    } else if (g_sfxOverlayState == UISfxOverlayState::Active) {
        const std::uint16_t pressed = SfxOverlayPressedInput();
        if ((g_sfxOverlayFlags & kSfxSubsetMaskMode) != 0)
            HandleSfxOverlaySubsetInput(pressed);
        else
            HandleSfxOverlayExactInput(pressed);
    } else if (g_sfxOverlayState == UISfxOverlayState::Resolved) {
        if ((g_sfxOverlayFlags & kSfxCloseImmediately) != 0) {
            icon.iconFlags &= static_cast<std::uint16_t>(~8U);
            g_sfxOverlayState = UISfxOverlayState::Idle;
            icon.overlayMode = 0;
            iconUpdateInputBuffer();
        } else {
            g_sfxOverlayState = UISfxOverlayState::Closing;
            if (static_cast<std::uint16_t>(g_sfxTimedOutMarker) == 0xa000U)
                GameQte_StartSlideOut();
        }
    } else if (g_sfxOverlayState == UISfxOverlayState::Closing &&
        (((g_sfxOverlayFlags & kSfxWaitForExternalClose) != 0 && static_cast<std::uint16_t>(g_sfxTimedOutMarker) != 0xa000U) || !GameQte_SlideOutRunning())) {
        iconUpdateInputBuffer();
        icon.iconFlags &= static_cast<std::uint16_t>(~8U);
        g_sfxOverlayState = UISfxOverlayState::Idle;
        icon.overlayMode = 0;
    }
    if ((g_sfxOverlayFlags & kSfxShowOverlay) != 0 && (icon.iconFlags & 8U) != 0 && g_sfxStateDirty) {
        GameQte_ShowMask(g_currentSfxMask, g_sfxOverlayFlags);
        g_gameQteHidden = 0;
        TryInvokeRegisteredCallback(kUiGameQte);
        g_uiElements[kUiGameQte].isActive = 1;
    } else {
        g_gameQteHidden = 1;
    }
}



void Script_SetSfxOverlayTimeoutTuning(ScenarioContext *context, void *rawParams) {
    (void)context;
    if (!rawParams)
        return;
    static_assert(sizeof(UISfxOverlayTimeoutTuningParameters) == 8);

    const auto &params = *static_cast<const UISfxOverlayTimeoutTuningParameters *>(rawParams);
    switch (params.mode) {
    case UISfxOverlayTimeoutTuningMode::Session: {
        const auto action = static_cast<UISfxOverlayTimeoutSessionAction>(static_cast<std::uint8_t>(params.value));
        if (action == UISfxOverlayTimeoutSessionAction::Begin) {
            g_qteSessionActive = 1;
            g_qteOverlay.accumulatedFrames = 0;
            g_sfxOverlayTimeoutBoostCount = 0;
        } else if (action == UISfxOverlayTimeoutSessionAction::Advance) {
            if (g_qteSessionActive == 1 && ++g_sfxOverlayTimeoutBoostCount <= 2)
                g_qteOverlay.accumulatedFrames = AddSfxOverlayFrames(g_qteOverlay.accumulatedFrames, g_qteOverlay.stepFrames);
        } else if (action == UISfxOverlayTimeoutSessionAction::Reset) {
            Shenmue_nPauseState &= ~0x20;
            g_sfxOverlayTimeoutBoostEnabled = 1;
            g_sfxOverlayTimeoutPendingBonusFrames = -1;
            g_qteOverlayTimeoutMaxFrames = 32;
            g_qteOverlay = {.stepFrames = 4, .accumulatedFrames = 0};
            g_qteSessionActive = 0;
            g_sfxOverlayTimeoutBoostCount = 0;
        }
        break;
    }
    case UISfxOverlayTimeoutTuningMode::DisableBoost:
        g_sfxOverlayTimeoutBoostEnabled = 0;
        break;
    case UISfxOverlayTimeoutTuningMode::SetStepFrames: {
        const std::int16_t value = static_cast<std::int16_t>(static_cast<std::uint16_t>(params.value));
        g_qteOverlay.stepFrames = value < 0 ? 4 : value;
        break;
    }
    case UISfxOverlayTimeoutTuningMode::SetPendingBonusFrames:
        g_sfxOverlayTimeoutPendingBonusFrames = static_cast<std::int16_t>(static_cast<std::uint16_t>(params.value));
        break;
    case UISfxOverlayTimeoutTuningMode::SetMaximumFrames:
        g_qteOverlayTimeoutMaxFrames = static_cast<std::int16_t>(static_cast<std::uint16_t>(params.value));
        break;
    }
}



void UI_ICQuCallback(HLib::TaskData *) {
    if (g_uiElements[kUiGameQte].parseDefinitionFromFile)
        g_uiElements[kUiGameQte].parseDefinitionFromFile();
    g_sfxOverlayTask = CurrentTask;
    g_sfxOverlayTimeoutPendingBonusFrames = -1;
    g_qteOverlayTimeoutMaxFrames = 32;
    g_sfxOverlayState = UISfxOverlayState::Idle;
    g_sfxOverlayTimeoutBoostCount = 0;
    g_qteSessionActive = 0;
    g_sfxOverlayTimeoutBoostEnabled = 1;
    g_qteOverlay = {.stepFrames = 4, .accumulatedFrames = 0};
    InstallOwnedTaskMain(UI_ICQuMainCallback);
}

void UI_ICQuMainCallback(HLib::TaskData *taskData) {
    UI_SFXOverlayCallback(taskData);
}

} // namespace shenmue::s1
