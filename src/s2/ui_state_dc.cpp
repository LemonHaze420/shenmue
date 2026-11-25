#include "s2/ui_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/player_stats_dc.hpp"

#define UI_ZERO
#define UI_ICON(name) name
namespace shenmue {
namespace s2 {

void iconToggleStateFor(std::uint8_t slot, std::uint8_t enabled);

bool GetShouldRenderIcons(RenderIconSlot slot);

bool ToggleRenderIcons(RenderIconSlot slot, bool enabled);

void SetMoneyIconPhase(std::uint8_t phase);

std::uint8_t GetParentMenu();

SleepActionState g_sleepAction;

SleepActionPadControls g_sleepActionPads;

std::uint32_t sleepActionFlags;

std::uint16_t sleepActionPublishedSelection;

std::int32_t sleepActionRetargetFrames;

std::int32_t helpIconItemEffectFrames = 0, helpIconItemEffectState = 0;

float moneyIconFrames;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
HLib::Task *itemEffectIconTask = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
ConversationBuffer *itemEffectAudio = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::byte *guiPtxBacking;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t commonItemTexture = -1, itemEffectAmount = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t itemEffectLabelTexture;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t itemEffectIconPhase = 0, itemEffectColumns = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int8_t itemEffectIconFrames;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
ItemEffectPosition itemEffectPosition UI_ZERO;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
LegacyIconSprite itemEffectSprite UI_ZERO;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
LegacyIconSprite itemSelectorHeadingSprite UI_ZERO;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
QuickIconState g_quickIconState UI_ZERO;
std::int8_t g_quickIconUnpressed;
std::int8_t g_quickIconBlinkPhase;
std::uint16_t g_quickIconTerminalStatus;
std::uint32_t g_quickIconPositionFlags, g_quickIconLoadingReleaseFlag;
std::uint16_t g_quickIconButtonMask;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_quickIconSavedLoadingFlags = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
LegacyIconSprite g_quickIconSprite UI_ZERO;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t moneyIconAmount, moneyIconResult, moneyIconDisplay = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float sleepActionScale = 1.0f;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t g_helpIconState = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
NativeItemLookState g_itemLookState;
NativeItemLookState *g_itemLook = &g_itemLookState;
std::uint8_t g_itemLookShortcutRequest, g_itemLookHelpTopic, g_itemLookHelpMode;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Vector3i g_itemLookRotation UI_ZERO;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
HLib::Task *g_itemSelectTask, *g_uniqueItemActionTask;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
NativeItemSelectorCamera g_itemSelectorCamera;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t g_itemSelectorOwnedMask[64];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t &g_itemSelectorNavigationInput = itemEffectIconPhase;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t &g_itemSelectorCameraActive = g_itemSelectorCamera.mode;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t g_itemSelectorTemporaryStatus[2], g_itemSelectorDelayedResult = 0;
#endif

std::int16_t g_fightMoveTrainingGains[3][40] UI_ZERO;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void CloseMoneyIcon() {
    SetMoneyIconPhase(4);
    moneyIconFrames = 0.0f;
}
#endif

// @verified - d
void SetPlayerCashOrMoney(std::int32_t amount, std::int32_t mode) {
    if (mode) {
        moneyIconDrawMode = 2;
        moneyIconTarget = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_nativePlayerStats.cash) + static_cast<std::uint32_t>(amount));
    } else {
        moneyIconDrawMode = 0;
        moneyIconTarget = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_nativePlayerStats.segaCoins) + static_cast<std::uint32_t>(amount));
    }
    if (moneyIconTarget > 99999999)
        moneyIconTarget = 99999999;
    else if (moneyIconTarget < 0)
        moneyIconTarget = 0;
    SetMoneyIconPhase(2);
    ToggleRenderIcons(UI_ICON(MoneyMessage), true);
    moneyIconFlags = 0x14;
    moneyIconFrames = 75.0f;
}

// @verified - d
void SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags) {
    if (GetShouldRenderIcons(UI_ICON(Any)))
        return;
    moneyIconFlags = flags;
    moneyIconMaximum = maximum;
    if (duration)
        moneyIconFrames = static_cast<float>(duration);
    else
        moneyIconFrames = 10800.0f;
    moneyIconStepAmount = step;
    if (moneyIconMaximum < 1)
        moneyIconMaximum = 1;
    if (moneyIconMaximum > g_nativePlayerStats.cash)
        moneyIconMaximum = g_nativePlayerStats.cash;
    if (moneyIconStepAmount < 1)
        moneyIconStepAmount = 1;
    if (moneyIconStepAmount > moneyIconMaximum)
        moneyIconStepAmount = moneyIconMaximum;
    moneyIconMaximum = (moneyIconMaximum / moneyIconStepAmount) * moneyIconStepAmount;
    SetMoneyIconPhase(2);
    ToggleRenderIcons(UI_ICON(MoneyMessage), true);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsModeBitActive(std::uint32_t mode) {
    if (mode & currentItemSelectState)
        return true;
    return false;
}
#endif

// @verified - d
void ResetSleepActionDefaultState() {
    g_sleepAction.phase = 0;
    g_sleepAction.retained01 = 0;
    g_sleepAction.currentSelection = 4;
    g_sleepAction.inputMode = 0;
    g_sleepAction.result = 0;
    ToggleRenderIcons(UI_ICON(SleepAction), false);
    iconToggleStateFor(6, false);
    sleepActionFlags = 0x10f;
    sleepActionPublishedSelection = 0xffff;
}

// @verified - d
void doItemEffectStuff() {
    helpIconItemEffectFrames = 150;
    helpIconItemEffectState = 1;
}

// @verified - d
void vfResetMoveTrainingGains() {
    for (std::int32_t index = 0; index < 40; ++index) {
        g_fightMoveTrainingGains[0][index] = 0;
        g_fightMoveTrainingGains[1][index] = 0;
        g_fightMoveTrainingGains[2][index] = 0;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetPlayerCashOrMoney(std::int32_t amount, std::int32_t mode, std::int32_t step) {
    SetPlayerCashOrMoney(amount, mode);
    if (!mode && moneyIconTarget > 999999)
        moneyIconTarget = 999999;
    moneyIconFlags = 0x54;
    moneyIconStepAmount = step;
}

#endif

} // namespace s2
} // namespace shenmue
#undef UI_ZERO
#undef UI_ICON

#endif
