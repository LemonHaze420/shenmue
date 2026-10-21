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

// @verified - p
void SetupScalingForGUI() {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    cash.selectorMessagePosition = {-516.0f, 340.0f, 1.0f, 0.0f};
    cash.selectorPulseSlot = 2;
    cash.currencyIcon.halfScreenY = 340.0f;
    cash.currencyIcon.scaledHeight = 40;
    cash.currencyIcon.depthOrScale = 2.0f;
    cash.selectorAmountPrefix.halfScreenY = 350.0f;
    cash.selectorMessageDepth = 3.0f;
    cash.selectorAmountPrefix.flags14 = 0;
    cash.selectorAmountPrefix.halfScreenX = -516.0f;
    cash.selectorAmountPrefix.scaledWidth = 16;
    cash.selectorAmountPrefix.scaledHeight = 16;
    cash.selectorAmountPrefix.positionScaleX = 1.0f;
    cash.selectorAmountPrefix.positionScaleY = 1.0f;
    cash.selectorAmountPrefix.angle = 0;
    cash.selectorAmountPrefix.depthOrScale = 3.0f;
    cash.selectorAmountPrefix.alignmentType = 5;
    cash.currencyIcon.pvrSurfaceIndex = 275;
    cash.currencyIcon.scaledWidth = 80;
    cash.currencyIcon.flags14 = 0;
    cash.currencyIcon.halfScreenX = -516.0f;
    cash.currencyIcon.positionScaleX = 1.0f;
    cash.currencyIcon.positionScaleY = 1.0f;
    cash.currencyIcon.angle = 0;
    cash.currencyIcon.alignmentType = 5;
    for (ScreenSpaceParams &digit : cash.selectorDigits) {
        digit.pvrSurfaceIndex = 277;
        digit.scaledWidth = 16;
        digit.flags14 = 0;
        digit.halfScreenX = -516.0f;
        digit.halfScreenY = 350.0f;
        digit.scaledHeight = 16;
        digit.positionScaleX = 1.0f;
        digit.positionScaleY = 1.0f;
        digit.angle = 0;
        digit.depthOrScale = 3.0f;
        digit.alignmentType = 5;
    }
    for (std::size_t index = 0; index < cash.selectorChoiceMarkers.size(); ++index) {
        ScreenSpaceParams &marker = cash.selectorChoiceMarkers[index];
        marker.flags14 = 0;
        marker.halfScreenX = -516.0f;
        marker.halfScreenY = 340.0f;
        marker.scaledWidth = 16;
        marker.scaledHeight = 16;
        marker.pvrSurfaceIndex = static_cast<std::int16_t>(index == 0 ? 290 : 289);
        marker.positionScaleX = 1.0f;
        marker.positionScaleY = 1.5f;
        marker.angle = 0;
        marker.depthOrScale = 3.0f;
        marker.alignmentType = 5;
    }
    cash.selectorHintPosition = {-176.0f, 91.0f, 1.0f, 0.0f};
    cash.selectorSprite.pvrSurfaceIndex = -1;
    cash.selectorSprite.depthOrScale = 2.0f;
    cash.selectorSprite.halfScreenX = -176.0f;
    cash.selectorSprite.halfScreenY = 91.0f;
    cash.selectorHintFlags = 2;
    cash.selectorPulseScale = 1.0f;
    cash.selectorSprite.positionScaleX = 1.0f;
    cash.selectorSprite.positionScaleY = 1.0f;
    cash.selectorSprite.scaledWidth = 128;
    cash.selectorSprite.scaledHeight = 32;
    cash.selectorSprite.alignmentType = 5;
    cash.selectorSprite.angle = 0;
    cash.selectorSprite.flags14 = 0;
    for (ScreenSpaceParams &arrow : cash.selectorArrows) {
        arrow.halfScreenY = 91.0f;
        arrow.halfScreenX = -176.0f;
        arrow.scaledWidth = 96;
        arrow.scaledHeight = 50;
        arrow.positionScaleX = 1.0f;
        arrow.positionScaleY = 1.0f;
        arrow.angle = 0;
        arrow.depthOrScale = 2.0f;
        arrow.alignmentType = 5;
        arrow.pvrSurfaceIndex = 275;
    }
}

// @verified - p
void BuildCashSelectorDigits(std::int32_t value) {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    for (std::size_t index = 0; index < cash.selectorDigits.size(); ++index) {
        std::size_t digit = 0;
        while (value >= kCashDenominations[index]) {
            value -= kCashDenominations[index];
            ++digit;
        }
        cash.selectorDigits[index].pvrSurfaceIndex = kCashDigitSprites[digit];
    }
}

// @verified - p
void DrawCashSelectorDigits(float x, float y, float scale, float depth, std::int32_t value, std::uint32_t flags, std::int32_t screenMode) {
    BuildCashSelectorDigits(value);
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    for (std::size_t reverse = cash.selectorDigits.size(); reverse != 0;) {
        ScreenSpaceParams &digit = cash.selectorDigits[--reverse];
        digit.halfScreenX = x;
        x -= scale * 16.0f;
        digit.halfScreenY = y;
        digit.depthOrScale = depth;
        digit.positionScaleX = scale;
        digit.positionScaleY = scale;
    }
    std::size_t first = 0;
    while (first < 7 && cash.selectorDigits[first].pvrSurfaceIndex == kCashDigitSprites[0])
        ++first;
    for (std::size_t index = first; index < cash.selectorDigits.size(); ++index)
        AdjustScreenSpaceWithParams(&cash.selectorDigits[index], flags, screenMode);
}

// @verified - p
void DrawCashSelectorAmount(float x, float y, float depth, std::int32_t value, std::int8_t type) {
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    ScreenSpaceParams &prefix = cash.selectorAmountPrefix;
    prefix.depthOrScale = depth;
    if (type != 0) {
        prefix.scaledWidth = 24;
        prefix.scaledHeight = 24;
        prefix.pvrSurfaceIndex = 288;
        prefix.halfScreenX = (x + 8.0f) + 12.0f;
    } else {
        const float right = x + 16.0f;
        prefix.pvrSurfaceIndex = 287;
        prefix.scaledWidth = 16;
        prefix.scaledHeight = 16;
        std::size_t index = 0;
        while (index < kCashDenominations.size() && value < kCashDenominations[index])
            ++index;
        prefix.halfScreenX = index >= kCashDenominations.size() ? right - 16.0f : right - static_cast<float>(16 * (8 - index));
    }
    prefix.halfScreenY = y - kCashAmountOffsets[static_cast<std::size_t>(type)];
    AdjustScreenSpaceWithParams(&prefix, 0xff656565U, 0);
    DrawCashSelectorDigits(type != 0 ? x : x + 16.0f, y, 1.0f, depth, value, 0xff656565U, 0);
}

// @verified - p
void CashOverlay_DrawSelector() {
    if (!g_updateRenderFrameMode)
        return;
    CashOverlayOwner &cash = g_cashOverlayPresentation;
    float x = static_cast<float>(static_cast<double>(cash.selectorMessagePosition.x) - -54.0 - 37.0);
    float scale = cash.selectorPulseScale;
    for (std::int32_t index = 0; index < 2; ++index) {
        ScreenSpaceParams &arrow = cash.selectorArrows[static_cast<std::size_t>(index)];
        arrow.halfScreenX = x;
        if (cash.selectorPulseSlot == index && (Shenmue_nPauseState & 2) == 0) {
            scale = static_cast<float>(static_cast<double>(scale) + 0.25);
            cash.selectorPulseScale = scale;
            if (scale >= 2.0f) {
                cash.selectorPulseScale = 1.0f;
                scale = 1.0f;
                cash.selectorPulseSlot = 2;
            }
            arrow.positionScaleX = scale;
            arrow.positionScaleY = scale;
        }
        if (g_updateRenderFrameMode) {
            AdjustScreenSpaceWithParams(&arrow, 0xb0000000U, 0);
            scale = cash.selectorPulseScale;
        }
        x += 94.0f;
    }
    const float right = static_cast<float>(static_cast<double>(cash.selectorMessagePosition.x) - -54.0);
    DrawCashSelectorDigits(cash.selectorArrows[0].halfScreenX + 16.0f, cash.selectorArrows[0].halfScreenY, cash.selectorArrows[0].positionScaleX,
        cash.selectorArrows[0].depthOrScale + 1.0f, 100, 0xff656565U, 0);
    DrawCashSelectorAmount(right - 6.0f, 171.0f, 3.0f, cash.selectedValue, static_cast<std::int8_t>(cash.selectorAmountType));
    cash.selectorSprite.halfScreenX = cash.selectorArrows[1].halfScreenX;
    cash.selectorSprite.halfScreenY = cash.selectorArrows[1].halfScreenY;
    cash.selectorSprite.positionScaleX = cash.selectorArrows[1].positionScaleX;
    cash.selectorSprite.positionScaleY = cash.selectorArrows[1].positionScaleY;
    if (cash.selectorSprite.pvrSurfaceIndex != -1)
        AdjustScreenSpaceWithParams_0(&cash.selectorSprite, 0);
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

    std::int32_t remainder = std::abs(difference);
    std::int32_t step = 0;
    for (const std::int32_t denomination : kCashDenominations) {
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
        cash.currencyIcon.scaledHeight = 40;
        cash.currencyIcon.pvrSurfaceIndex = cash.useCash ? 275 : 276;
        cash.currencyIcon.scaledWidth = cash.useCash ? 80 : 40;
        cash.value = cash.useCash ? g_playerStats.cash : static_cast<std::int32_t>(g_playerStats.segaCoins);
        cash.selectorPulseSlot = 2;
        cash.selectorMessagePosition.x = -516.0f;
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
            if (cash.selectorSprite.pvrSurfaceIndex != -1)
                PVR_FreeAllocatedSurfaceByIndex(cash.selectorSprite.pvrSurfaceIndex);
            cash.selectorSprite.pvrSurfaceIndex = -1;
            cash.selectorSprite.pvrSurfaceIndex = AllocatePVRSurface("Cancel", 128, 0xffff, 0xf000, 0, true);
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
                cash.selectorPulseSlot = 1;
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
        if (cash.selectorPulseSlot == 2) {
            cash.state = UICashOverlayState::WaitCloseAnimation;
            static_cast<void>(ResetOwnedElement(kUiGameMessage));
            if (cash.selectorActive)
                static_cast<void>(ResetOwnedElement(kUiActionSelector));
        }

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
        if (cash.selectorSprite.pvrSurfaceIndex != -1)
            PVR_FreeAllocatedSurfaceByIndex(cash.selectorSprite.pvrSurfaceIndex);
        cash.selectorSprite.pvrSurfaceIndex = -1;
        cash.resetRequested = false;
        break;
    case UICashOverlayState::Idle:
        break;
    }
    if ((icon.iconFlags & 0x1000U) != 0) {
        g_uiElements[kUiGameMessage].isActive = 1;
        if (cash.selectorActive) {
            g_uiElements[kUiActionSelector].isActive = 1;
            CashOverlay_DrawSelector();
        }
    }
}

} // namespace shenmue::s1
