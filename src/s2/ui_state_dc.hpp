#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/legacy_sprite_types.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/item_effect.hpp"
#include "common/math.hpp"
#endif

#define UI_NOEXCEPT
#define UI_DATA const
#define UI_INIT =
#define UI_LAYOUT_NAME_(line) UiLayoutCheck##line
#define UI_LAYOUT_NAME(line) UI_LAYOUT_NAME_(line)
#define UI_LAYOUT(condition) typedef char UI_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {
struct PadControlEntry;

enum RenderIconSlot { Any = 0, ItemSelect = 1, ActionSelector = 2, Qte = 3, MoneyMessage = 4, ItemEffectMessage = 5, SleepAction = 6 };
extern std::uint32_t renderIconFlags, activeIconFlags;
extern std::uint32_t currentItemSelectState;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::uint8_t mainIconPhase;
#else
extern std::uint32_t mainIconPhase;
#endif
struct IconPadControls {
    PadControlEntry *current, *previous;
};
extern IconPadControls g_iconPads;
struct SleepActionState {
    std::uint8_t phase, retained01;
    std::int8_t targetSelection, currentSelection;
    std::uint8_t retained04, inputMode, result, retained07;
    float x, y, velocity, reservedScale, depth;
};
UI_LAYOUT(sizeof(SleepActionState) == 0x1c);
UI_LAYOUT(offsetof(SleepActionState, inputMode) == 5);
UI_LAYOUT(offsetof(SleepActionState, x) == 8);
UI_LAYOUT(offsetof(SleepActionState, depth) == 0x18);
struct SleepActionPadControls {
    PadControlEntry *previous, *current;
};
extern SleepActionState g_sleepAction;
extern SleepActionPadControls g_sleepActionPads;
extern std::uint32_t sleepActionFlags;
extern std::uint16_t sleepActionPublishedSelection;
extern std::int32_t sleepActionRetargetFrames;
extern std::int32_t helpIconItemEffectFrames, helpIconItemEffectState;
extern std::uint8_t moneyIconPhase, moneyIconDrawMode;
extern std::int32_t moneyIconTarget;
extern std::int32_t moneyIconMaximum, moneyIconStepAmount;
extern std::uint32_t moneyIconFlags;
extern float moneyIconFrames;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ConversationBuffer;
struct TextureChunk;
extern HLib::Task *itemEffectIconTask;
extern ConversationBuffer *itemEffectAudio;
extern std::byte *guiPtxBacking;
extern std::int32_t commonItemTexture, itemEffectAmount;
extern std::int16_t itemEffectLabelTexture;
extern std::uint8_t itemEffectIconPhase, itemEffectColumns;
extern std::int8_t itemEffectIconFrames;
extern ItemEffectPosition itemEffectPosition;
extern LegacyIconSprite itemEffectSprite, itemSelectorHeadingSprite;
void InitializeItemSelectorWindow();
std::uint32_t GetItemSelectionMode(std::uint8_t slot);
bool AdvanceIconPosition(ItemEffectPosition *position, float target);
bool RetreatIconPosition(ItemEffectPosition *position, float target);
void AllocateGuiPtxBacking();
TextureChunk *SetupPtxTexture(std::int32_t width, std::uint32_t idLow, std::uint32_t idHigh);
std::int32_t PublishLegacyItemLabel(const char *text, std::int32_t width, std::int32_t texture, std::int32_t mode);
std::int32_t PrepareLegacyInventoryItemLabel(std::int16_t *label, std::uint32_t item);
bool InitializeCommonItemTexture();
bool ToggleItemEffect(std::uint32_t item, std::int32_t amount);
void DrawLegacyItemLabel(const std::int16_t *texture, const Vector2 *position, float depth, std::uint32_t secondaryColour);
void DrawItemEffectWindow(std::int32_t columns, float x, float y, float depth);
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth);
void ItemEffectRender_Callback(void *parameter);
void CtrlItemEffectIcon_Callback(void *parameter);
HLib::Task *QueueItemEffectIconTask(std::uint8_t queue);
bool CloseItemEffect(std::int32_t force);
bool CloseItemSelectIcon(std::int32_t force);
bool ResetQuickIconState(std::int32_t force);
bool AbortMoneyIcon(std::int32_t force);
std::uint32_t RetireIconInputSlots(std::uint32_t mask, std::int32_t force);
std::int32_t RetireIconInputSlot(std::uint32_t slot, std::int32_t force);
std::int32_t CloseWristwatchTransitionIcons();
std::int32_t CloseHeldStateIcons();

struct QuickIconState {
    std::int8_t phase;
    std::byte retained01;
    std::uint16_t flags, requiredStatus, retained06;
    float maximumFrames, elapsedFrames;
    std::uint16_t currentStatus;
    std::uint16_t retained12, legacyMode, legacyReserved;
    std::int16_t legacyTexture;
    std::uint16_t legacyWidth;
    std::byte retained1c[4];
};
UI_LAYOUT(sizeof(QuickIconState) == 0x20);
UI_LAYOUT(offsetof(QuickIconState, flags) == 2);
UI_LAYOUT(offsetof(QuickIconState, requiredStatus) == 4);
UI_LAYOUT(offsetof(QuickIconState, maximumFrames) == 8);
UI_LAYOUT(offsetof(QuickIconState, elapsedFrames) == 0x0c);
UI_LAYOUT(offsetof(QuickIconState, currentStatus) == 0x10);
UI_DATA std::uint16_t kQuickIconButtonTypes[11] UI_INIT{3, 7, 23, 11, 55, 67, 131, 135, 151, 769, 1047};
UI_DATA std::uint16_t kQuickIconFaceIcons[5] UI_INIT{0x700, 0x701, 0x702, 0x703, 0};
extern QuickIconState g_quickIconState;
extern std::int8_t g_quickIconUnpressed;
extern std::int8_t g_quickIconBlinkPhase;
extern std::uint16_t g_quickIconTerminalStatus;
extern std::uint32_t g_quickIconPositionFlags, g_quickIconLoadingReleaseFlag;
extern std::uint16_t g_quickIconButtonMask;
extern std::uint32_t g_quickIconSavedLoadingFlags;
extern LegacyIconSprite g_quickIconSprite;
extern std::int32_t moneyIconAmount, moneyIconResult, moneyIconDisplay;
extern float sleepActionScale;
extern std::uint32_t g_helpIconState;
extern HLib::Task *mainIconTask;
extern std::int32_t nativeGuiActive, collectionSpriteOwners[2];
static const std::uint16_t guiRegisteredItems[4] = {0, 2, 20, 21};
void GUI_Tasks_Enqueue();
void InitMainIcon_Callback(void *data);
void CtrlMainIcon_Callback(void *data);
struct NativeItemSelectorState {
    std::int8_t row, column, visibleRows, columns;
    std::uint8_t phase, glowBrightening;
    std::int16_t count, quantitySum;
    std::uint16_t retained0a;
    std::int32_t activeMenu;
    std::uint32_t actionResult, glowColour, exitFlags;
    void *context;
    std::int32_t selectedItem, selectedIndex;
};
UI_LAYOUT(sizeof(NativeItemSelectorState) == 0x28);
UI_LAYOUT(offsetof(NativeItemSelectorState, phase) == 4);
UI_LAYOUT(offsetof(NativeItemSelectorState, context) == 0x1c);
UI_LAYOUT(offsetof(NativeItemSelectorState, selectedItem) == 0x20);
struct NativeUniqueItemAction {
    std::int32_t item;
    std::int32_t (*begin)();
    std::int32_t (*update)();
    void (*complete)();
    void (*cancel)(std::int32_t force);
};
UI_LAYOUT(sizeof(NativeUniqueItemAction) == 0x14);
UI_LAYOUT(offsetof(NativeUniqueItemAction, begin) == 4);
struct MT7Node;
struct MT7Model;
struct NativeItemLookState {
    std::uint16_t phase, uiVisible, uiSelected;
    std::int16_t uiMode;
    std::uint32_t selectedItem;
    float scale, maximumScale, minimumScale;
    std::int32_t worldYaw;
    Vector3 eventPosition;
    std::int32_t lightYaw, cameraState, perspective, playerVisible, playerDisplay;
    Vector3 eye, target;
    MT7Node *root;
    MT7Model *model;
    PadControlEntry *previousPad, *pad;
    std::uint32_t itemFlags;
};
UI_LAYOUT(sizeof(NativeItemLookState) == 0x68);
UI_LAYOUT(offsetof(NativeItemLookState, eventPosition) == 0x1c);
UI_LAYOUT(offsetof(NativeItemLookState, lightYaw) == 0x28);
UI_LAYOUT(offsetof(NativeItemLookState, eye) == 0x3c);
UI_LAYOUT(offsetof(NativeItemLookState, root) == 0x54);
UI_LAYOUT(offsetof(NativeItemLookState, pad) == 0x60);
UI_LAYOUT(offsetof(NativeItemLookState, itemFlags) == 0x64);
extern NativeItemSelectorState g_itemSelectorState;
extern NativeItemLookState g_itemLookState, *g_itemLook;
extern std::uint8_t g_itemLookShortcutRequest, g_itemLookHelpTopic, g_itemLookHelpMode;
extern Vector3i g_itemLookRotation;
extern HLib::Task *g_itemSelectTask, *g_uniqueItemActionTask;
struct NativeItemSelectorCamera {
    Vector3 eye, savedEye, goalEye, eyeStep;
    Vector3 target, savedTarget, goalTarget, targetStep;
    float alpha, savedAlpha, alphaStep;
    std::byte retained6c[8];
    std::int32_t duration, frame;
    std::uint8_t mode;
    std::byte retained7d[3];
};
UI_LAYOUT(sizeof(NativeItemSelectorCamera) == 0x80);
UI_LAYOUT(offsetof(NativeItemSelectorCamera, duration) == 0x74);
UI_LAYOUT(offsetof(NativeItemSelectorCamera, mode) == 0x7c);
extern NativeItemSelectorCamera g_itemSelectorCamera;
extern std::uint8_t g_itemSelectorOwnedMask[64];
extern std::uint8_t &g_itemSelectorNavigationInput, &g_itemSelectorCameraActive;
extern std::int16_t g_itemSelectorTemporaryStatus[2], g_itemSelectorDelayedResult;
void RefreshItemSelectorOwnedMask();
std::uint32_t RefreshItemSelectorInventory(std::uint32_t bank);
void SetLegacyItemSelectorExitFlag();
void SetItemSelectorDelayedResult(std::int16_t result);
void DrawItemSelectorInventory(std::int32_t columns, std::int32_t maximum, float x, float y, float depth, std::int32_t first, std::uint32_t bank);
void RefreshItemSelectorSelection();
void CtrlItemSelectIcon_Callback(void *data);
void ICIT_UniqAction_Callback(void *data);
void ICSU_InitItemLook_Callback(void *data);
void ICSU_CtrlItemLook_Callback(void *data);
bool OpenItemLook(std::int32_t item);
void BeginItemSelector();
std::int32_t RequestTitleMenu();
void UpdateSelectedItemStateFlag(std::int16_t item);
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
void InitItemSelectIcon_Callback(void *data);
void InitializeMoneyIconParameters();
std::int32_t AcquireCtrlSpeakTextHandle(const char *text, std::int32_t centered);
void CtrlMoneyIcon_Callback(void *data);
void DrawMoneyAmount(const Vector2 *position, float depth, std::int32_t amount, std::int8_t currency, std::uint32_t colour, float scale);
struct DateTime;
void DrawSelectorDate(const Vector2 &position, float depth, const DateTime &date);
void InitSpeakIcon(HLib::Task *parent, std::uint8_t queue);
void InitQuickIcon_Callback(void *data);
void CtrlQuickIcon_Callback(void *data);
std::uint16_t GetPadControlData(std::uint8_t mode);
extern ItemEffectPosition quickIconPosition;
void InitNowQte(std::int8_t queue);
HLib::Task *HP_SetUpHelpIcon();
std::uint8_t GetSleepActionIconFlags();
void tmx_CtrlHandWatch(std::int8_t queue);
void MM_MemoCtrl(std::int8_t queue);
void NAVI_DummyWait_Enqueue(std::int8_t queue);
HLib::Task *CtrlSleepActionIcon(std::int8_t queue);
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::int8_t queue);
void RegisterItems();
void CreatePAWNShops();
HLib::Task *Queue_pw_CtrlPawnWindow();
std::uint32_t GetNumBet() UI_NOEXCEPT;
std::uint32_t AnimateMoneyAmount(std::int32_t initial, std::int32_t target, std::int32_t modeIndex) UI_NOEXCEPT;
bool SetSleepActionIcon(std::int32_t flags);
std::uint32_t GetSleepActionIconResult();
void SetHelpIconState() UI_NOEXCEPT;
void SetShowQteFlag(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint16_t buttonFlags) UI_NOEXCEPT;
void SetShowQteFlagForButtonType(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint32_t buttonType) UI_NOEXCEPT;
void SetQteButtonState(std::uint16_t mask) UI_NOEXCEPT;
std::int32_t GetCurrentQteStatus() UI_NOEXCEPT;
#endif
void SetMoneyIconPhase(std::uint8_t phase);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void KeepMoneyIconOpen();
void CloseMoneyIcon();
#endif
void SetPlayerCashOrMoney(std::int32_t amount, std::int32_t mode);
void SetMoneyRelatedUIElement(std::int16_t maximum, std::int16_t duration, std::int16_t step);
void SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags);
bool GetShouldRenderIcons(RenderIconSlot slot);
std::uint8_t GetParentMenu();
std::uint32_t GetCurrentItemSelectState();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool IsModeBitActive(std::uint32_t mode);
std::uint32_t ConsumeItemMenuRequest();
#endif
std::int32_t IsItemSelectable();
std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled);
bool ToggleRenderIcons(RenderIconSlot slot, bool enabled);
void iconToggleStateFor(std::uint8_t slot, std::uint8_t enabled);
void ResetMainIconState();
void ResetSleepActionDefaultState();
void doItemEffectStuff();

extern std::int16_t g_fightMoveTrainingGains[3][40];
void vfResetMoveTrainingGains();
void ResetWazamakiState();
void InitializeEventWindowState();

} // namespace s2
} // namespace shenmue
#undef UI_LAYOUT_NAME
#undef UI_LAYOUT_NAME_
#undef UI_DATA
#undef UI_INIT
#undef UI_NOEXCEPT
#undef UI_LAYOUT

#endif
