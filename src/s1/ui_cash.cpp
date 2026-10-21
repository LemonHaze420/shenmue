#include "s1/ui_internal.hpp"

#include "s1/task/telm.hpp"
namespace shenmue::s1::ui_runtime_detail {

std::string FormatCashOverlayText(std::int32_t value) {
    const bool negative = value < 0;
    const std::uint64_t magnitude = negative ? static_cast<std::uint64_t>(-static_cast<std::int64_t>(value)) : static_cast<std::uint64_t>(value);
    const char separator = g_uiLoadedLanguage == 2 || g_uiLoadedLanguage == 3 ? ' ' : ',';

    std::string digits = std::to_string(magnitude);
    for (std::ptrdiff_t index = static_cast<std::ptrdiff_t>(digits.size()) - 3; index > 0; index -= 3) {
        digits.insert(static_cast<std::size_t>(index), 1, separator);
    }
    if (negative)
        digits.insert(digits.begin(), '-');

    SubtitleStringId id{};
    constexpr std::string_view kMoneyStringId = "UIMONEYVAL"; // @text EN: "¥[[MONEY_VAL]]" | JA: "[[MONEY_VAL]]円"
    std::memcpy(&id, kMoneyStringId.data(), kMoneyStringId.size());
    const char *const localized = ReadStringFromStringTableById(id);
    if (!localized || !*localized)
        return digits;

    std::string text(localized);
    constexpr std::string_view kMoneyValueTag = "[[MONEY_VAL]]";
    const std::size_t position = text.find(kMoneyValueTag);
    if (position == std::string::npos)
        return digits;
    text.replace(position, kMoneyValueTag.size(), digits);
    return text;
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

void UI_ICMoWaitCloseAnimation() {
    if (g_isPlayerGameStateActive == 5)
        g_cashOverlayPresentation.state = UICashOverlayState::WaitCloseAnimation;
}

void UI_ICMoRequest(std::int32_t value, bool useCash) {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    cash.useCash = useCash;
    const std::int32_t current = useCash ? g_playerStats.cash : static_cast<std::int32_t>(g_playerStats.segaCoins);
    const std::int32_t wrappedTarget = static_cast<std::int32_t>(static_cast<std::uint32_t>(current) + static_cast<std::uint32_t>(value));
    cash.targetBalance = std::clamp(wrappedTarget, 0, 99999999);
    cash.balanceAnimating = true;
    cash.displayMode = UICashOverlayDisplayMode::BalanceDeltaAutoClose;
    cash.state = UICashOverlayState::OpenRequested;
    GameMessage_SetContent(useCash ? UIGameMessageContentType::Money : UIGameMessageContentType::Token, current, "0");
    if (g_currUIState == -1 || g_currUIState == kUiGameMessage)
        UI_ActivateElement(g_uiElements[kUiGameMessage]);
    cash.selectorActive = false;
}

void UI_ICMoShowCurrentBalance(bool show) {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    cash.displayMode = UICashOverlayDisplayMode::ManualCurrentBalance;
    if (show) {
        cash.state = UICashOverlayState::OpenRequested;
        GameMessage_SetContent(UIGameMessageContentType::Money, g_playerStats.cash, "0");
    } else {
        cash.state = UICashOverlayState::CloseRequested;
    }
}

void UI_ICMoBeginSelector(std::int32_t maximumValue, std::uint16_t timeoutFrames) {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    cash.maximumValue = static_cast<std::int16_t>(maximumValue);
    cash.timeoutFrames = timeoutFrames == 0 ? 600 : static_cast<std::int16_t>(timeoutFrames);
    cash.state = UICashOverlayState::OpenRequested;
    cash.selectorActive = true;
    GameMessage_SetContent(UIGameMessageContentType::Money, g_playerStats.cash, "0");
    if (g_currUIState == -1 || g_currUIState == kUiGameMessage)
        UI_ActivateElement(g_uiElements[kUiGameMessage]);
}

void UI_ICMoRequestClose() {
    if (g_cashOverlayPresentation.state != UICashOverlayState::Idle) {
        g_cashOverlayPresentation.state = UICashOverlayState::CloseRequested;
    }
}

bool UI_ICMoIsIdle() {
    return g_cashOverlayPresentation.state == UICashOverlayState::Idle;
}

bool UI_ICMoPeekResult(std::int32_t &result) {
    const CashOverlayOwner &cash = g_cashOverlayPresentation;
    if (cash.state != UICashOverlayState::Idle)
        return false;
    result = cash.selectedValue;
    return true;
}

std::int32_t CashAnimationStep(std::int32_t difference) {
    constexpr std::array<std::int32_t, 8> denominations{10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
    std::int32_t remainder = std::abs(difference);
    std::int32_t step = 0;
    for (const std::int32_t denomination : denominations) {
        if (remainder < denomination)
            continue;
        step += denomination;
        remainder %= denomination;
    }
    return difference < 0 ? -step : step;
}

void UI_ICMoCallback(HLib::TaskData *) {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    IconPresentationOwner &icon = g_iconPresentation;
    RefreshBatteryLifeForCassettes();
    switch (cash.state) {
    case UICashOverlayState::OpenRequested:
        ActivateOwnedElement(kUiGameMessage, "/ui/gamemessage/GameMessage.ui");
        icon.iconFlags |= 0x1000U;
        cash.value = cash.useCash ? g_playerStats.cash : static_cast<std::int32_t>(g_playerStats.segaCoins);
        cash.state = UICashOverlayState::WaitOpenAnimation;

        if (cash.displayMode == UICashOverlayDisplayMode::ManualCurrentBalance) {
            g_isPlayerGameStateActive = 5;
        } else {
            g_telmIsActive = 0;
            ForEachCewpEntityApplyCallback(0, telmGetIsActive);
            g_isPlayerGameStateActive = 5;
            if (!g_telmIsActive)
                CapturePad(icon.input, icon.previousInput);
        }
        cash.selectedValue = 0;
        cash.elapsedFrames = 0;
        cash.resetRequested = false;
        if (cash.selectorActive) {
            ActivateOwnedElement(kUiActionSelector, "/ui/actionselector/ActionSelector.ui");
            ActionSelectorSetValueChoiceLabels(cash.maximumValue);
            if (cash.selectorSurface != -1)
                PVR_FreeAllocatedSurfaceByIndex(cash.selectorSurface);
            cash.selectorSurface = -1;
            cash.selectorSurface = AllocatePVRSurface("Cancel", 128, 0xffff, 0xf000, 0, true);
        }
        break;
    case UICashOverlayState::WaitOpenAnimation:
        if (!GameMessageAnimIdle())
            break;
        if (cash.selectorActive) {
            cash.savedSicnState = g_uiState;
            cash.savedSicnPausePolicy = g_currentSicn;
            cash.savedSicnActive = UI_SICNIsTaskActive();
            if (cash.savedSicnActive)
                UI_SICNSetState(32, UISicnPausePolicy::KeepOnPause);
            else
                UI_SICNEnqueue(32, UISicnPausePolicy::KeepOnPause);
        }
        cash.state = UICashOverlayState::DisplayActive;
        break;
    case UICashOverlayState::DisplayActive:
        cash.elapsedFrames = static_cast<std::int16_t>(static_cast<std::uint16_t>(cash.elapsedFrames) + 1U);
        if (cash.selectorActive) {
            if (cash.elapsedFrames >= cash.timeoutFrames) {
                static_cast<void>(ResetOwnedElement(kUiGameMessage));
                static_cast<void>(ResetOwnedElement(kUiActionSelector));
                cash.state = UICashOverlayState::CloseRequested;
                cash.selectedValue = 0;
                break;
            }
            // CashOverlay_UpdateSelectorInput: the ActionSelector's value mode owns the amount; down or B on an empty amount and A on a
            // non-empty one close the overlay, and a direction that changed nothing plays the reject cue.
            const std::int32_t code = g_actionSelectorInputCode;
            const std::int32_t previousValue = cash.selectedValue;
            const std::int32_t value = g_actionSelectorPresentation.displayedValue;
            cash.selectedValue = value;
            g_actionSelectorInputCode = 13;
            if (((code == 4 || code == 9) && previousValue == 0) || (code == 8 && value != 0)) {
                cash.state = UICashOverlayState::CloseRequested;
            }
            if (previousValue == value && ((code == 2 && value <= 0) || ((code == 6 || code == 0) && value >= cash.maximumValue)))
                shenExecuteAICACommand(kUiRejectSound, 0, 0);
        } else if (cash.elapsedFrames >= 45) {
            cash.elapsedFrames = 45;
            if (cash.balanceAnimating) {
                if (cash.value != cash.targetBalance) {
                    const std::int32_t difference = cash.targetBalance - cash.value;
                    const std::int32_t step = CashAnimationStep(difference);
                    cash.lastBalanceAnimationStep = std::abs(step);
                    cash.value += step;
                    if ((difference < 0 && cash.value < cash.targetBalance) || (difference > 0 && cash.value > cash.targetBalance)) {
                        cash.value = cash.targetBalance;
                    }
                }
                if (cash.useCash)
                    g_playerStats.cash = cash.value;
                else
                    g_playerStats.segaCoins = static_cast<std::uint32_t>(cash.value);
                shenExecuteAICACommand(0xd00abU, 0, 0);
                GameMessage_SetContent(cash.useCash ? UIGameMessageContentType::Money : UIGameMessageContentType::Token, cash.value, "0");
            }
            if (cash.value == cash.targetBalance) {
                cash.balanceAnimating = false;
                if (cash.displayMode != UICashOverlayDisplayMode::ManualCurrentBalance) {
                    cash.autoCloseFrames += g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;
                    if (cash.autoCloseFrames >= 30.0f) {
                        cash.state = UICashOverlayState::CloseRequested;
                        cash.autoCloseFrames = 0.0f;
                        static_cast<void>(ResetOwnedElement(kUiGameMessage));
                    }
                }
            }
        }
        break;
    case UICashOverlayState::CloseRequested:
        cash.resetRequested = true;
        if (!ResetOwnedElement(kUiGameMessage))
            break;
        if (cash.selectorActive && !ResetOwnedElement(kUiActionSelector)) {
            break;
        }
        cash.state = UICashOverlayState::WaitCloseAnimation;
        break;
    case UICashOverlayState::WaitCloseAnimation:
        if (!GameMessageAnimIdle())
            break;
        icon.iconFlags &= static_cast<std::uint16_t>(~0x1000U);
        cash.state = UICashOverlayState::Idle;
        if (cash.displayMode == UICashOverlayDisplayMode::ManualCurrentBalance)
            g_isPlayerGameStateActive = 7;
        else {
            g_telmIsActive = 0;
            ForEachCewpEntityApplyCallback(0, telmGetIsActive);
            if (g_telmIsActive)
                g_isPlayerGameStateActive = 7;
            else
                iconUpdateInputBuffer();
        }
        icon.overlayMode = 0;
        cash.balanceAnimating = false;
        cash.useCash = true;
        cash.displayMode = UICashOverlayDisplayMode::None;
        if (cash.selectorActive) {
            if (cash.savedSicnActive)
                UI_SICNSetState(cash.savedSicnState, cash.savedSicnPausePolicy);
            else
                UI_SICNClearState(static_cast<std::uint16_t>(((~g_uiState) & 0x200U) | 0x0dffU));
        }
        cash.selectorActive = false;
        if (cash.selectorSurface != -1)
            PVR_FreeAllocatedSurfaceByIndex(cash.selectorSurface);
        cash.selectorSurface = -1;
        cash.resetRequested = false;
        break;
    case UICashOverlayState::Idle:
        break;
    }
    if ((icon.iconFlags & 0x1000U) != 0) {
        g_uiElements[kUiGameMessage].isActive = 1;
        if (cash.selectorActive)
            g_uiElements[kUiActionSelector].isActive = 1;
    }
}

} // namespace shenmue::s1
