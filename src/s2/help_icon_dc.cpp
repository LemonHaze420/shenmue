#include "s2/help_icon_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/pause.hpp"
#include "s2/ui_state_dc.hpp"

namespace shenmue {
namespace s2 {

std::uint32_t GetQuickIconLoadingReleaseState();

HelpIconFlags helpIconPersistent, helpIconFrameOverlay;
HelpDirectionBlink helpDirectionBlinks[4] = {};
std::uint32_t helpIconDeferredRemoval;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t helpIconItemPromptFrames = 0;
std::uint32_t iconSelectFlags;

// @verified - d
void RequestHelpIconItemPrompt() {
    helpIconItemPromptFrames = 150;
    CheckHelpIcon(0x200, 0x200, CheckHelpIconOp_Add);
}

// @verified - d
void RequestHelpIconSelection(std::uint32_t mask) {
    iconSelectFlags |= mask;
}
#endif

// @verified - d
void MarkHelpIconsForRemoval(std::uint32_t mask) {
    helpIconDeferredRemoval |= mask & helpIconPersistent.selected;
}

// @verified - d
void ConfigureHelpDirectionBlink(std::uint32_t visible) {
    std::int32_t count = 0;
    std::uint32_t missing = 0;
    for (std::uint32_t index = 0; index < 4; ++index) {
        if (visible & (1U << (index + 12)))
            ++count;
        else
            missing = kHelpDirectionMissingOrder[index];
    }
    if (!count)
        return;
    if (count == 3) {
        for (std::uint32_t index = 0; index < 4; ++index) {
            HelpDirectionBlink &blink = helpDirectionBlinks[kHelpDirectionRingOrder[(missing + index) % 4]];
            blink.onFrames = kHelpDirectionOnFrames[2][index];
            blink.offFrames = blink.onFrames;
            blink.remaining = kHelpDirectionPhases[2][index];
            blink.mode = index == 1 ? 2 : blink.remaining != 0;
        }
        return;
    }
    std::int32_t selected = 0;
    for (std::uint32_t index = 0; index < 4; ++index) {
        HelpDirectionBlink &blink = helpDirectionBlinks[index];
        blink.mode = 0;
        if (!(visible & (1U << (index + 12))))
            continue;
        blink.onFrames = kHelpDirectionOnFrames[count - 1][selected];
        blink.offFrames = blink.onFrames;
        blink.remaining = kHelpDirectionPhases[count - 1][selected];
        blink.mode = 1;
        ++selected;
    }
}

// @verified - d
void CheckHelpIcon(std::uint32_t visible, std::uint32_t selected, CheckHelpIconOp operation) {
    std::uint32_t blink;
    switch (operation) {
    case CheckHelpIconOp_Set:
        helpIconPersistent.visible = visible;
        helpIconPersistent.selected = selected;
        break;
    case CheckHelpIconOp_Add:
        helpIconPersistent.visible |= visible;
        helpIconPersistent.selected |= selected;
        break;
    case CheckHelpIconOp_Remove:
        blink = (selected & helpIconPersistent.selected) & (helpIconPersistent.visible & visible);
        if (blink)
            MarkHelpIconsForRemoval(blink);
        helpIconPersistent.visible = (~visible | blink) & helpIconPersistent.visible;
        break;
    case CheckHelpIconOp_FrameOverlay:
        helpIconFrameOverlay.visible |= visible;
        helpIconFrameOverlay.selected |= selected;
        break;
    }
    helpIconPersistent.selected &= helpIconPersistent.visible;
    helpIconFrameOverlay.selected &= helpIconFrameOverlay.visible;
    ConfigureHelpDirectionBlink(helpIconPersistent.visible);
}

// @verified - d
std::uint32_t GetPersistentHelpIconFlags(std::uint32_t index) {
    if (index <= 1) {
        const std::uint32_t *flags = &helpIconPersistent.visible;
        return flags[index];
    }
    return 0;
}

// @verified - d
std::uint32_t GetUiFlag(std::uint8_t mode) {
    switch (mode) {
    case 0:
        return GetPersistentHelpIconFlags(0) & 0xffffU;
    case 2:
        if (!GetPersistentHelpIconFlags(0))
            return 0;
    case 1:
        return 1;
    }
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
#else
void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
#endif

// @verified - d
void AddHelpIconMenuFlags(std::uint16_t flags, std::uint8_t) {
    if (GetQuickIconLoadingReleaseState() && !(g_quickIconSavedLoadingFlags & 0x20U) && (g_loadingFlag & 0x20U)) {
        g_quickIconSavedLoadingFlags = g_loadingFlag;
        FinishLoading();
    } else {
        g_quickIconSavedLoadingFlags = g_loadingFlag;
    }
    if (flags & 0x400U)
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        ExecuteAicaCommand(0xaab, 0, 0);
#else
        shenExecuteAICACommand(0xaab, 0, 0);
#endif
    if (GetPersistentHelpIconFlags(0))
        CheckHelpIcon(flags, 0, CheckHelpIconOp_Add);
    else
        CheckHelpIcon(flags, 0, CheckHelpIconOp_Set);
}

// @verified - d
void SetHelpIconMenuFlags(std::uint16_t flags, std::uint8_t) {
    if (!GetQuickIconLoadingReleaseState() && (g_loadingFlag & 0x20U))
        flags &= 0xfffeU;
    CheckHelpIcon(flags, 0, CheckHelpIconOp_Set);
}

// @verified - d
void ResetHelpIconsToItemPrompt() {
    const std::int32_t flags = (GetPersistentHelpIconFlags(0) & 0x200U) ? 0x200 : 0;
    CheckHelpIcon(flags, flags, CheckHelpIconOp_Set);
}

void RemoveHelpIconMenuFlags(std::uint16_t flags) {
    std::uint32_t visible = GetPersistentHelpIconFlags(0);
    visible &= ~flags;
    if (GetQuickIconLoadingReleaseState() && (g_quickIconSavedLoadingFlags & 0x20U)) {
        g_loadingFlag = g_quickIconSavedLoadingFlags;
        g_quickIconSavedLoadingFlags = 0;
    }
    CheckHelpIcon(visible, 0, CheckHelpIconOp_Set);
}

} // namespace s2
} // namespace shenmue

#endif
