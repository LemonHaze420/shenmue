#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {

void ReleaseIconInput() {
    ReleasePadCapture(g_iconPresentation.input, g_iconPresentation.previousInput);
    ReleasePadCapture(g_iconPresentation.notificationInput, g_iconPresentation.notificationPreviousInput);
    ReleasePadCapture(g_iconPresentation.itemLookInput, g_iconPresentation.itemLookPreviousInput);
    ReleasePadCapture(g_icmcInput, g_cassetteUiCachedInput);
}

void EnsureIconRootTask() {
    if (!IsOwnedUiTask(g_iconPresentation.rootTask, 14, SHEN_NAME(ICON), UI_ICONCallback, UI_ICONMainCallback)) {
        g_iconPresentation.rootTask = nullptr;
        if (g_commonIconSprites.count == 0)
            LoadSpriteSet(g_commonIconSprites, "COMICON.SPR", SHEN_NAME(ICON));
        UI_GUITasksEnqueue();
    }
}

bool IsOwnedSicnTask(const HLib::Task *task) {
    return task && task->taskIndex == 15 && !task->task_data_ptr && task->taskToken == static_cast<std::int32_t>(SHEN_NAME(SICN)) &&
        task->mainTaskCallbackFnPtr == UI_SICNCallback;
}

void EnsureSicnTask() {
    if (!IsOwnedSicnTask(g_sicnTask)) {
        g_sicnTask = HLib_EnqueueTaskWithoutParameter(UI_SICNCallback, 15, 0, SHEN_NAME(SICN));
        if (g_sicnTask)
            g_sicnTaskActive = 1;
    }
}

std::uint16_t IconHudStateBit(std::uint8_t inputIndex) {
    constexpr std::array<std::uint16_t, 4> bits{0x4, 0x2, 0x8, 0x10};
    return inputIndex < bits.size() ? bits[inputIndex] : 0;
}

std::uint8_t GetSicnHudPromptScanLimit() {
    const auto *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask ? g_mapControlTask->task_data_ptr : nullptr);
    if ((control->flags & 1U) != 0)
        return 0;

    const bool notebookOverride = g_notebookHelpOverrideActive != 0;
    if (g_TMEX_CurrentMode > 0 && !notebookOverride)
        return 0;
    if (g_qteSessionActive)
        return 1;
    if (!notebookOverride) {
        if (g_sleepSaveTrainingMenu.scriptResult != 0 && g_isPlayerGameStateActive != 0 && !ShouldRun()) {
            return 1;
        }
        if (g_pSCNC && g_pSCNC->header.active)
            return 0;
    }
    return static_cast<std::uint8_t>(kSicnPromptAssetIds.size());
}

bool ShouldDrawStandaloneSicnIcon() {
    const IconPresentationOwner &icon = g_iconPresentation;
    if (!icon.active)
        return false;
    if (icon.kind == UIIconPromptKind::Directional)
        return true;
    if (icon.kind != UIIconPromptKind::Button || icon.expectedInput >= kIconButtonAssetIds.size()) {
        return true;
    }
    return (g_uiState & IconHudStateBit(icon.expectedInput)) == 0;
}

void PopulateSicnHudPromptIcons() {
    if (g_updateRenderFrameMode == 0)
        return;

    const std::uint8_t scanLimit = GetSicnHudPromptScanLimit();
    std::uint16_t state = g_uiState;
    std::int32_t drawX = 48 * (13 - (g_iconPresentation.clockSlotReserved ? 1 : 0) - (g_sicnTaskActive ? 1 : 0));
    bool firstPrompt = true;

    for (std::size_t bit = 0; bit < g_sicnPromptSlots.size(); ++bit) {
        SicnPromptSlot &slot = g_sicnPromptSlots[bit];
        if (bit >= scanLimit || (state & 1U) == 0) {
            state = static_cast<std::uint16_t>(state >> 1U);
            continue;
        }

        slot.active = true;
        g_hudPromptFlagsThisFrame = static_cast<std::uint16_t>(g_hudPromptFlagsThisFrame | (1U << bit));
        if (firstPrompt && ShouldDrawStandaloneSicnIcon())
            drawX -= 48;
        firstPrompt = false;
        slot.assetId = kSicnPromptAssetIds[bit];
        slot.width = kSicnPromptWidths[bit];
        slot.height = kSicnPromptHeights[bit];
        slot.x = bit == 10 ? 80.0f : static_cast<float>(drawX);
        slot.y = 424.0f;
        slot.scaleX = bit == 0 ? 1.0f : 1.2f;
        slot.scaleY = slot.scaleX;
        slot.depth = 8.0f;
        if (slot.assetId == 302U)
            g_uiElements[kUiGameHudPractice].isActive = 1;
        if (bit == 0) {
            slot.assetId = 295;
            slot.width = 32;
            slot.height = 32;
            slot.y = 420.0f;
            slot.scaleX = 1.7f;
            slot.scaleY = 1.7f;
            slot.depth = 7.0f;
        }
        if (bit != 10)
            drawX -= 48;
        state = static_cast<std::uint16_t>(state >> 1U);
    }
    g_sicnPopulatedHudPromptsThisFrame = true;
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

bool UI_ICONRequestNotification(std::uint16_t inputMask, std::int16_t timeoutFrames, std::int16_t alphaFrames, std::int32_t ownerAndCaptureMode) {
    IconPresentationOwner &icon = g_iconPresentation;
    const bool scriptOwner = (ownerAndCaptureMode & 0x8000) != 0;
    const std::int16_t captureMode = static_cast<std::int16_t>(ownerAndCaptureMode & 0x7fff);
    icon.inputCaptureMode = captureMode;
    if (captureMode == 2) {
        const bool scntOwnerWasPresent = (icon.ownerMask & kIconOwnerScnt) != 0;
        icon.ownerMask &= static_cast<std::uint16_t>(scriptOwner ? ~kIconOwnerScript : ~kIconOwnerScnt);
        if (!icon.ownerMask) {
            icon.state = UIIconNotificationState::CloseRequested;
        }
        return scntOwnerWasPresent;
    }

    icon.timeoutFrames = timeoutFrames;
    icon.bobStep = 3;
    icon.alpha = 0;
    icon.ownerMask |= scriptOwner ? kIconOwnerScript : kIconOwnerScnt;
    icon.bobOffset = 0;
    icon.alphaStep = static_cast<std::int16_t>(255 / (alphaFrames != 0 ? alphaFrames : 1));
    std::uint8_t expectedInput = 0;
    if ((inputMask & 0xff00U) != 0) {
        while (expectedInput < 4 && (inputMask & (0x100U << expectedInput)) == 0) {
            ++expectedInput;
        }
        icon.kind = UIIconPromptKind::Button;
        icon.promptRotation = 0;
        icon.promptDescriptor = kIconButtonAssetIds[expectedInput];
    } else {
        while (expectedInput < 8 && (inputMask & (1U << expectedInput)) == 0) {
            ++expectedInput;
        }
        icon.kind = UIIconPromptKind::Directional;
        icon.promptRotation = static_cast<std::int32_t>(expectedInput) << 13U;
        icon.promptDescriptor = 271;
    }
    icon.inputMask = inputMask;
    icon.expectedInput = expectedInput;
    icon.state = UIIconNotificationState::OpenRequested;
    if (static_cast<std::int16_t>(icon.ownerMask) <= 1)
        icon.active = false;
    return icon.active;
}

bool UI_ICONNotificationIsFadingOut() {
    return g_iconPresentation.state == UIIconNotificationState::FadeOut;
}

void UI_ICONClear() {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.ownerMask = 0;
    if (icon.active) {
        icon.state = UIIconNotificationState::CloseRequested;
    } else {
        icon.state = UIIconNotificationState::Idle;
        icon.kind = UIIconPromptKind::None;
        icon.alpha = 0;
    }
}


bool UI_ICONRequestSystemMenu() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (g_isPlayerGameStateActive != 7 || !SCEN_IsConversationPromptEnabled())
        return false;
    icon.cachedPlayerControlMode = 0;
    g_isPlayerGameStateActive = 0;
    CapturePad(icon.input, icon.previousInput);
    icon.menuState = UIIconSystemMenuState::OpenRequested;
    UI_ICONRecomputeInventoryGrid();
    icon.iconFlags |= 1U;
    icon.bufferedInitialPressMask = 0;
    icon.bufferedFollowupPressMask = 0;
    icon.overlayMode = 0;
    icon.overlayTransitionStage = 3;
    return true;
}

void UI_ICONRequestSystemMenuClose() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (icon.menuState != UIIconSystemMenuState::Closed)
        icon.menuCloseRequested = true;
}

bool UI_ICONSystemMenuIsClosed() {
    return g_iconPresentation.menuState == UIIconSystemMenuState::Closed;
}


void UI_ICONCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.rootTask = CurrentTask;
    g_isPlayerGameStateActive = 7;
    icon.previousInput = nullptr;
    icon.overlayTransitionStage = 0;
    icon.overlayMode = 0;
    icon.iconFlags = 0;
    g_conversationPromptActivationLock = 0;
    icon.input = AllocateClearedPADCEntry();
    InstallOwnedTaskMain(UI_ICONMainCallback);
}


void iconUpdateInputBuffer() {
    IconPresentationOwner &icon = g_iconPresentation;
    if (icon.input && (icon.input->flags & 2U) != 0 && icon.previousInput) {
        shenUpdateInputsInto(icon.previousInput);
        icon.previousInput = nullptr;
    }
    g_isPlayerGameStateActive = 7;
}


void UI_ICONMainCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    const PadCaptureEntry *const input = icon.input;
    if ((Shenmue_nPauseState & 2) != 0) {
        // One dword store clears the held copy with the pressed half, then the repeated half; the stick copies are kept.
        g_tmpPadcHeld = 0;
        g_sfxInputFlags = 0;
    } else if (input) {
        g_tmpPadcHeld = input->held;
        g_sfxInputFlags = input->pressedPriority | (static_cast<std::uint32_t>(input->repeatedPresses) << 16U);
        g_tmpRightStickHi = static_cast<std::uint8_t>(input->rightStick);
        g_tmpRightStickLo = static_cast<std::uint8_t>(input->rightStick >> 8U);
    }
    if (icon.overlayTransitionStage != 0) {
        if (icon.overlayTransitionStage == 5) {
            iconUpdateInputBuffer();
            navRequestNewMode(static_cast<std::int32_t>(icon.cachedPlayerControlMode));
            icon.iconFlags &= static_cast<std::uint16_t>(~1U);
            icon.cachedPlayerControlMode = 0;
            icon.overlayTransitionStage = 0;
        }
        return;
    }

    const auto *const control =
        g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const auto *const pad = control ? static_cast<const PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
    const std::uint16_t pressed = pad ? pad->pressedPriority : 0;
    if ((pressed & 2U) == 0 || !convCanOpen() || !SCEN_IsConversationPromptEnabled() || g_conversationPromptActivationLock || !SRCH_AllowsIconInputPriority()) {
        return;
    }
    icon.cachedPlayerControlMode = g_pControlPadController ? g_pControlPadController->currentMode : 0U;
    navRequestNewMode(5);
    g_isPlayerGameStateActive = 0;
    CapturePad(icon.input, icon.previousInput);
    icon.menuState = UIIconSystemMenuState::OpenRequested;
    UI_ICONRecomputeInventoryGrid();
    icon.iconFlags |= 1U;
    icon.bufferedInitialPressMask = 0;
    icon.bufferedFollowupPressMask = 0;
    icon.overlayTransitionStage = 3;
    icon.overlayMode = 0;
}

void UI_ICNQCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    icon.notificationTask = CurrentTask;
    icon.notificationInput = AllocateClearedPADCEntry();
    icon.state = UIIconNotificationState::Idle;
    icon.kind = UIIconPromptKind::None;
    icon.alpha = 0;
    icon.bobOffset = 0;
    icon.bobStep = 3;
    icon.timeoutFrames = 0;
    icon.active = false;
    icon.clockSlotReserved = false;
    icon.notificationDrawX = 576.0f;
    icon.ownerMask = 0;

    g_sicnTask = nullptr;
    g_sicnTaskActive = 0;
    g_sicnClearPendingUntilUnpause = false;
    Shenmue_nPrevPauseState = 0;
    g_uiState = 0;
    g_currentSicn = UISicnPausePolicy::KeepOnPause;
    InstallOwnedTaskMain(UI_ICNQMainCallback);
    ActivateOwnedElement(kUiGameHud, "/ui/gamehud/GameHud.ui");
}

void UI_ICNQMainCallback(HLib::TaskData *) {
    IconPresentationOwner &icon = g_iconPresentation;
    switch (icon.state) {
    case UIIconNotificationState::FadeIn:
        icon.alpha = static_cast<std::int16_t>(icon.alpha + icon.alphaStep);
        if (icon.alpha >= 255) {
            icon.alpha = 255;
            icon.state = UIIconNotificationState::Visible;
        }
        break;
    case UIIconNotificationState::OpenRequested:
        if (icon.inputCaptureMode == 0) {
            CapturePad(icon.notificationInput, icon.notificationPreviousInput);
        }
        icon.active = true;
        icon.state = UIIconNotificationState::FadeIn;
        break;
    case UIIconNotificationState::Visible:
        if (icon.inputCaptureMode == 0) {
            const std::uint16_t pressed = icon.notificationInput ? icon.notificationInput->pressedPriority : 0;
            std::uint8_t inputCode = kDirectionInputCodes[pressed >> 12U];
            if (inputCode >= 8) {
                inputCode = 13;
                if ((pressed & 0x200U) != 0)
                    inputCode = 9;
                else if ((pressed & 0x400U) != 0)
                    inputCode = 8;
                else if ((pressed & 2U) != 0)
                    inputCode = 11;
                else if ((pressed & 4U) != 0)
                    inputCode = 10;
            }
            const bool matched = (icon.kind == UIIconPromptKind::Directional && inputCode == icon.expectedInput) ||
                (icon.kind == UIIconPromptKind::Button && inputCode == kIconButtonMatchCodes[icon.expectedInput]);
            if (matched) {
                shenExecuteAICACommand(0x900abU, 0, 0);
                icon.state = UIIconNotificationState::CloseRequested;
            }
            if (icon.timeoutFrames != 0x7fff) {
                --icon.timeoutFrames;
                if (icon.timeoutFrames <= 0) {
                    icon.timeoutFrames = 0;
                    icon.state = UIIconNotificationState::CloseRequested;
                }
            }
        }
        break;
    case UIIconNotificationState::CloseRequested:
        icon.state = UIIconNotificationState::FadeOut;
        icon.alpha = 0;
        break;
    case UIIconNotificationState::FadeOut:
        icon.alpha = static_cast<std::int16_t>(icon.alpha - icon.alphaStep);
        if (icon.alpha <= 0) {
            icon.alpha = 0;
            if (icon.inputCaptureMode == 0)
                RestorePad(icon.notificationPreviousInput);
            icon.active = false;
            icon.state = UIIconNotificationState::Idle;
        }
        break;
    case UIIconNotificationState::Idle:
        break;
    }

    icon.bobOffset = static_cast<std::int16_t>(icon.bobOffset + icon.bobStep);
    if (icon.bobOffset <= 0) {
        icon.bobOffset = 0;
        icon.bobStep = 3;
    } else if (icon.bobOffset >= 63) {
        icon.bobOffset = 63;
        icon.bobStep = -3;
    }




    const bool clockSlot = shenGetCanUpdate() != 0;
    icon.clockSlotReserved = clockSlot;
    if (ShouldDrawStandaloneSicnIcon()) {
        if (icon.kind != UIIconPromptKind::Button && icon.promptDescriptor == 271)
            g_hudPromptFlagsThisFrame = static_cast<std::uint16_t>(g_hudPromptFlagsThisFrame | 0x1000U);
        icon.notificationDrawX = clockSlot ? 528.0f : 576.0f;
    }
    if (!icon.active && !clockSlot && !g_sicnPopulatedHudPromptsThisFrame) {
        if (!g_uiElements[kUiGameTraining].isVisible) {
            g_uiElements[kUiGameHud].isHandledExternally = 1;
            if (g_uiElements[kUiGameHud].elementDraw)
                g_uiElements[kUiGameHud].elementDraw();
            g_uiElements[kUiGameHudWatch].isHandledExternally = 1;
            if (g_uiElements[kUiGameHudWatch].elementDraw)
                g_uiElements[kUiGameHudWatch].elementDraw();
        }
        return;
    }
    if (icon.kind == UIIconPromptKind::Button && icon.active && icon.expectedInput < 4) {
        constexpr std::array<std::uint8_t, 4> kButtonPromptBits{2, 1, 3, 4};
        g_hudPromptFlagsThisFrame = static_cast<std::uint16_t>(g_hudPromptFlagsThisFrame | (1U << kButtonPromptBits[icon.expectedInput]));
    }
    if (!g_uiElements[kUiMoveLearned].isVisible) {
        g_uiElements[kUiGameHud].isHandledExternally = 0;
        if (g_uiElements[kUiGameHud].elementDraw)
            g_uiElements[kUiGameHud].elementDraw();
        g_uiElements[kUiGameHudWatch].isHandledExternally = 0;
        if (g_uiElements[kUiGameHudWatch].elementDraw)
            g_uiElements[kUiGameHudWatch].elementDraw();
    }
    g_sicnPopulatedHudPromptsThisFrame = false;
}

void UI_SICNEnqueue(std::uint16_t state, UISicnPausePolicy pausePolicy) {
    const std::int32_t previousPauseState = Shenmue_nPauseState;
    if (!IsOwnedSicnTask(g_sicnTask)) {
        if ((Shenmue_nPauseState & 2) != 0)
            return;
        EnsureSicnTask();
        if (!g_sicnTask)
            return;
        UI_SICNSetState(state, pausePolicy);
        g_hudPromptFlagsThisFrame = 0;
        g_sicnPopulatedHudPromptsThisFrame = false;
        g_sicnTransientStateTimeoutFrames = 300;
        g_sicnClearPendingUntilUnpause = false;
        g_sicnTaskActive = 1;
        Shenmue_nPrevPauseState = previousPauseState;
        if (g_qteSessionActive && (previousPauseState & 0x20) != 0) {
            Shenmue_nPauseState = previousPauseState & ~0x20;
        }
        if ((state & 0x400U) != 0)
            shenExecuteAICACommand(0xaabU, 0, 0);
        return;
    }

    g_currentSicn = static_cast<UISicnPausePolicy>(static_cast<std::int32_t>(g_currentSicn) | static_cast<std::int32_t>(pausePolicy));
    std::uint16_t mergedState = static_cast<std::uint16_t>(state | g_uiState);
    if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20) != 0) {
        mergedState = static_cast<std::uint16_t>(mergedState & ~1U);
    }
    if ((g_uiState & kUiStateTransientNotification) != 0 && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive != 0) {
        mergedState = static_cast<std::uint16_t>(mergedState | kUiStateTransientNotification);
    }
    g_uiState = mergedState;
    if (!mergedState)
        UI_SICNClearState(0x0fffU);

    if (g_qteSessionActive && (Shenmue_nPrevPauseState & 0x20) == 0 && (previousPauseState & 0x20) != 0) {
        Shenmue_nPrevPauseState = previousPauseState;
        Shenmue_nPauseState = previousPauseState & ~0x20;
    } else {
        Shenmue_nPrevPauseState = previousPauseState;
    }
    g_sicnTransientStateTimeoutFrames = 300;
}

void UI_SICNSetState(std::uint16_t state, UISicnPausePolicy pausePolicy) {
    g_currentSicn = pausePolicy;
    if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20) != 0) {
        state = static_cast<std::uint16_t>(state & ~1U);
    }
    if ((g_uiState & kUiStateTransientNotification) != 0 && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive != 0) {
        state = static_cast<std::uint16_t>(state | kUiStateTransientNotification);
    }
    g_uiState = state;
    if (!state)
        UI_SICNClearState(0x0fffU);
}

bool UI_SICNIsTaskActive() {
    return g_sicnTaskActive != 0;
}

void UI_SICNClearState(std::uint16_t state) {
    if (!g_sicnTask)
        return;
    if ((Shenmue_nPauseState & 2) != 0) {
        g_sicnClearPendingUntilUnpause = true;
        return;
    }
    g_uiState = static_cast<std::uint16_t>(g_uiState & static_cast<std::uint16_t>(~state));
    if (!g_uiState) {
        HLib_DestroyTask(g_sicnTask);
        g_sicnTask = nullptr;
        g_uiState = 0;
        g_sicnTaskActive = 0;
    }
    if (g_qteSessionActive && (Shenmue_nPrevPauseState & 0x20) != 0) {
        Shenmue_nPauseState = Shenmue_nPrevPauseState;
        Shenmue_nPrevPauseState = 0;
    }
}

void UI_SICNDispatch(std::int32_t mode, std::uint16_t state, UISicnPausePolicy pausePolicy) {
    switch (mode) {
    case 0:
        UI_SICNEnqueue(state, pausePolicy);
        break;
    case 1: {
        const std::uint16_t clearMask = static_cast<std::uint16_t>(((~g_uiState) & kUiStateTransientNotification) | 0x0dffU);
        UI_SICNClearState(clearMask);
        break;
    }
    case 2:
        UI_SICNClearState(state);
        break;
    default:
        break;
    }
}

void UI_SICNCallback(HLib::TaskData *) {
    if ((Shenmue_nPauseState & 2) != 0) {
        if (g_currentSicn == UISicnPausePolicy::ClearOnPause)
            g_sicnClearPendingUntilUnpause = true;
        return;
    }
    if (g_sicnClearPendingUntilUnpause) {
        const std::uint16_t clearMask = static_cast<std::uint16_t>(((~g_uiState) & kUiStateTransientNotification) | 0x0dffU);
        UI_SICNClearState(clearMask);
        return;
    }
    if ((g_uiState & kUiStateTransientNotification) != 0) {
        g_sicnTransientStateTimeoutFrames = static_cast<std::int16_t>(g_sicnTransientStateTimeoutFrames - 1);
        if (g_sicnTransientStateTimeoutFrames <= 0) {
            std::uint16_t state = static_cast<std::uint16_t>(g_uiState & static_cast<std::uint16_t>(~kUiStateTransientNotification));
            g_uiState = state;
            if (!state) {
                UI_SICNClearState(0x0fffU);
                return;
            }
            g_currentSicn = UISicnPausePolicy::KeepOnPause;
            std::uint16_t updatedState = state;
            if (!g_qteSessionActive && (Shenmue_nPauseState & 0x20) != 0) {
                updatedState = static_cast<std::uint16_t>(updatedState & ~1U);
            }
            if ((state & kUiStateTransientNotification) != 0 && !g_notebookHelpOverrideActive && g_isPlayerGameStateActive != 0) {
                updatedState = static_cast<std::uint16_t>(updatedState | kUiStateTransientNotification);
            }
            g_uiState = updatedState;
            if (!updatedState) {
                UI_SICNClearState(0x0fffU);
                return;
            }
        }
    }
    if (!g_qteSessionActive || (Shenmue_nPauseState & 0x20) == 0) {
        PopulateSicnHudPromptIcons();
    }
}

} // namespace shenmue::s1
