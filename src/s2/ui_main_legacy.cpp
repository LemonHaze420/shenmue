#include "s2/ui_legacy_menus.hpp"
#include "s2/ui_internal.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/input.hpp"
#include "s2/save.hpp"
#include "s2/ui_scroll_list.hpp"
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {

std::int32_t legacyMainMenuInitState{};
std::uint32_t legacyMainMenuRequest{};
UIViewConfiguration legacyMainMenuConfiguration{-1, -1, -1};
UIElement legacyMainMenuTarget = UIElement::MainMenuNew;
std::int32_t legacyMainMenuSelectorIndex = -1;
float legacyMainMenuPromptSeconds = 15.0f;
std::int32_t legacyMainMenuWheelIndex{};
UIButtonWheel legacyMainMenuSavedWheel{};
std::int32_t legacyMainMenuSubmenuActive{};
UISelector *legacyMainMenuSelector{};
char legacyMainMenuPromptText[128]{};
char legacyMainMenuUserId[32]{};

std::int32_t mainLoadInitState{};
std::uint32_t mainLoadRequest{};
UIViewConfiguration mainLoadConfiguration{-1, -1, -1};
std::int32_t mainLoadScrollList = -1;
std::int32_t mainLoadMode{}, mainLoadSource{}, mainLoadPromptOpen{};
UIButtonPanel *mainLoadButtonPanel{};
TitleControlData *mainLoadTitleControl{};
char mainLoadFailedText[256]{};

// @verified - p
std::int64_t GetMainLoadRowCount() {
    return 11;
}

// @verified - p
void SetMainLoadListMode(std::int32_t mode) {
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(mainLoadScrollList));
    if (!list)
        return;
    mainLoadMode = mode;
    list->field74 = {0, 0};
    if (mode == 0)
        mainLoadSource = 0;
    else if (mode == 2)
        mainLoadSource = 1;
    else
        return;
    UI_GetScrollList(static_cast<std::uint32_t>(mainLoadScrollList))->field6C = static_cast<std::int32_t>(GetMainLoadRowCount());
    FormatAllSaveDataInfo(0, mainLoadSource);
}

// @verified - p
void SetMainLoadButtonLabels() {
    if (!mainLoadButtonPanel &&
        !(mainLoadButtonPanel = UI_FindViewButtonPanel(static_cast<std::uint32_t>(mainLoadConfiguration.uiHandleIndex), "MainLoad_Button_Panel")))
        return;
    mainLoadButtonPanel->entries[0].field0 = 61;
    mainLoadButtonPanel->entries[0].field4 = 0;
    std::snprintf(mainLoadButtonPanel->entries[0].field8.data(), 64, "%s", GetStringForUILabel("UIGNRSCROL"));
    mainLoadButtonPanel->entries[1].field0 = 0;
    mainLoadButtonPanel->entries[1].field4 = 0;
    std::snprintf(mainLoadButtonPanel->entries[1].field8.data(), 64, "%s", GetStringForUILabel("UIGNRLSLCT"));
}

// @verified - p
void InitializeMainLoad() {
    SetMainLoadButtonLabels();
}

// @verified - p
void BindMainLoad() {
    const std::uint32_t slot = static_cast<std::uint32_t>(mainLoadConfiguration.uiHandleIndex);
    mainLoadScrollList = -1;
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
        if (view->widgetKinds[widget] == 27)
            mainLoadScrollList = view->widgetPoolIndices[widget];
        if (mainLoadScrollList != -1)
            break;
    }
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(mainLoadScrollList));
    list->field6C = static_cast<std::int32_t>(GetMainLoadRowCount());
    FormatAllSaveDataInfo(0, mainLoadSource);
    list->field3A8 = UIWidget_Bind(mainLoadScrollList, 1, 2, {0.0f, 0.0f}, {-156.0f, 0.0f});
    mainLoadButtonPanel = UI_FindViewButtonPanel(slot, "MainLoad_Button_Panel");
    SetMainLoadButtonLabels();
}

// @verified - p
std::uint32_t LoadMainLoad() {
    if (mainLoadInitState == 0) {
        mainLoadInitState = LoadUIFile("/ui/mainsubmenus/load/MainLoad.ui", static_cast<std::int32_t>(UIElement::MainMenuContinue), &mainLoadRequest);
    } else if (mainLoadInitState == 1) {
        mainLoadInitState = GetNewUIFileInitFor(&mainLoadRequest, mainLoadConfiguration);
        if (mainLoadInitState == 2)
            BindMainLoad();
    } else if (mainLoadInitState == 2) {
        mainLoadInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(mainLoadConfiguration.uiHandleIndex));
    }
    return static_cast<std::uint32_t>(mainLoadInitState);
}

// @verified - p
std::int64_t RenderMainLoad() {
    if (mainLoadInitState == 3 || (LoadMainLoad(), static_cast<std::uint32_t>(mainLoadInitState - 2) <= 1U)) {
        uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(mainLoadConfiguration.uiHandleIndex));
        if (mainLoadPromptOpen == 1)
            EnableElement(UIElement::YesNoPrompt);
    }
    return 0;
}

// @verified - p
std::int64_t LoadGame() {
    if (static_cast<std::uint32_t>(mainLoadInitState - 2) > 1U)
        return 0;
    UIScrollList *const list = UI_GetScrollList(static_cast<std::uint32_t>(mainLoadScrollList));
    const SaveDataInfoText *const info = GetSaveDataInfoText(static_cast<std::uint32_t>(list->field74[0] + list->field74[1]));
    if (!mainLoadPromptOpen) {
        UI_UpdateScrollListSelection(static_cast<std::uint32_t>(mainLoadScrollList));
        FormatAllSaveDataInfo(list->field74[0], mainLoadSource);
        if ((g_inputBuffer[0].pressedPriority & 0x400U) != 0 && info && info->valid) {
            mainLoadPromptOpen = 1;
            const char *const no = GetStringForUILabel("UIGNRLNO00");
            const char *const yes = GetStringForUILabel("UIGNRLYES0");
            const char *const question = GetStringForUILabel("UILOADDOYO"); // @text EN: "Do you want to load this game?"
            MessageBoxTimed(question, yes, no, 0, 0.0f);
            ShowElementByIndex(UIElement::YesNoPrompt);
        }
        return 0;
    }
    const PromptResponse response = static_cast<PromptResponse>(ExecuteElementCallback(UIElement::YesNoPrompt));
    if (response != PromptResponse::Yes) {
        if (response == PromptResponse::No || response == PromptResponse::TimedOut || response == PromptResponse::Cancelled) {
            DestroyElementByIndex(UIElement::YesNoPrompt);
            mainLoadPromptOpen = 0;
        }
        return 0;
    }
    DestroyElementByIndex(UIElement::YesNoPrompt);
    mainLoadPromptOpen = 0;
    DestroyElementByIndex(UIElement::MainMenuContinue);
    g_titleFileSelect->selectedSlot = static_cast<std::int8_t>(static_cast<std::uint8_t>(list->field74[0]) + static_cast<std::uint8_t>(list->field74[1]));
    if (mainLoadMode == 1) {
        StartTitleCollectionGame();
        SetLegacyMainMenuSubmenuActive(0);
    } else if (mainLoadMode == 0) {
        if (!StartTitleLoadGame())
            std::snprintf(mainLoadFailedText, sizeof(mainLoadFailedText), "%s", GetStringForUILabel("UILOADFAILED"));
    } else if (mainLoadMode == 2) {
        StartTitleImportedGame(mainLoadTitleControl);
    }
    return 0;
}

// @verified - p
std::int64_t GetMainLoadMode() {
    return static_cast<std::uint32_t>(mainLoadMode);
}

// @verified - p
void SetLegacyMainMenuSubmenuActive(std::int32_t active) {
    legacyMainMenuSubmenuActive = active;
}

// @verified - p
std::uint32_t LoadLegacyMainMenu() {
    if (legacyMainMenuInitState == 0) {
        LoadUIFile("/ui/mainmenu/MainMenu.ui", static_cast<std::int32_t>(UIElement::MainMenu), &legacyMainMenuRequest);
        legacyMainMenuInitState = 1;
        return 1;
    }
    if (legacyMainMenuInitState == 1) {
        legacyMainMenuInitState = GetNewUIFileInitFor(&legacyMainMenuRequest, legacyMainMenuConfiguration);
        if (legacyMainMenuInitState != 2)
            return static_cast<std::uint32_t>(legacyMainMenuInitState);
        const std::uint32_t slot = static_cast<std::uint32_t>(legacyMainMenuConfiguration.uiHandleIndex);
        const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 23)
                continue;
            legacyMainMenuWheelIndex = view->widgetPoolIndices[widget];
            legacyMainMenuSavedWheel = g_uiButtonWheels[static_cast<std::size_t>(legacyMainMenuWheelIndex)];
            break;
        }
        legacyMainMenuSelectorIndex = UI_FindViewSelector(slot, "MainMenu_Main_Selector");
        legacyMainMenuSelector =
            static_cast<std::uint32_t>(legacyMainMenuSelectorIndex) < g_uiSelectors.size() ? &g_uiSelectors[legacyMainMenuSelectorIndex] : nullptr;
        HideUIWindow(UI_GetWindow(UI_FindViewWindow(slot, "MainMenu_SwitchAccountButtonIcon")));
        HideUIWindow(UI_GetWindow(UI_FindViewWindow(slot, "MainMenu_SwitchAccountButtonLabel")));
        return static_cast<std::uint32_t>(legacyMainMenuInitState);
    }
    if (legacyMainMenuInitState == 2)
        legacyMainMenuInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(legacyMainMenuConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(legacyMainMenuInitState);
}

// @verified - p
std::int64_t RenderLegacyMainMenu() {
    if (legacyMainMenuInitState != 3 && (LoadLegacyMainMenu(), static_cast<std::uint32_t>(legacyMainMenuInitState - 2) > 1U))
        return 0;
    RefreshNotebook();
    nk_context &context = *GetNativeUiContext();
    legacyMainMenuUserId[0] = 0;
    const std::uint32_t slot = static_cast<std::uint32_t>(legacyMainMenuConfiguration.uiHandleIndex);
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    if (view) {
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 15)
                continue;
            d3t::UILabelWidget *const label = UI_GetLabel(static_cast<std::uint32_t>(view->widgetPoolIndices[widget]));
            if (std::memcmp(label->id.data(), "USER_ID", 8) == 0)
                label->text = legacyMainMenuUserId;
        }
    }
    if (legacyMainMenuSubmenuActive) {
        if (legacyMainMenuTarget == UIElement::Options)
            EnableElement(UIElement::OptionsBackground);
        EnableElement(legacyMainMenuTarget);
        return 0;
    }
    DrawUIEntity(context, slot, static_cast<std::int32_t>(UIElement::MainMenu));
    if (legacyMainMenuTarget == UIElement::OptionsGamma && legacyMainMenuSelector) {
        legacyMainMenuSelector->field44 = 1;
        const char *const seconds = GetStringForUILabel("UIGNRLSECS");
        const std::int32_t remaining = static_cast<std::int32_t>(legacyMainMenuPromptSeconds);
        std::snprintf(legacyMainMenuPromptText, sizeof(legacyMainMenuPromptText), "%s\n%d %s", GetStringForUILabel("UIMAINUSES1"), remaining, seconds);
        legacyMainMenuSelector->label.token = legacyMainMenuPromptText;
        legacyMainMenuSelector->label.alreadyLocalized = 1;
    }
    return 0;
}

// @verified - p
std::uint8_t UpdateLegacyMainMenuS1Prompt() {
    std::uint8_t result = 0;
    if (!legacyMainMenuSelector) {
        legacyMainMenuTarget = UIElement::MainMenuNew;
        return 0;
    }
    UI_UpdateSelectorInput(static_cast<std::uint32_t>(legacyMainMenuSelectorIndex));
    bool confirmed = false, declined = false;
    std::uint16_t input = g_inputBuffer[0].pressedPriority;
    legacyMainMenuPromptSeconds = legacyMainMenuPromptSeconds + -0.033330001f;
    if ((input & 0x400U) != 0) {
        confirmed = true;
        if (legacyMainMenuSelector->pos == 1) {
            declined = true;
            result = 1;
        } else {
            g_inputBuffer[0].pressedPriority &= ~0x400U;
            if (IsArchiveReadComplete() || IsArchiveReadPending()) {
                result = 1;
            } else {
                declined = true;
                LegacyUICallbackNullsub();
            }
            input = g_inputBuffer[0].pressedPriority;
        }
    }
    if ((input & 0x200U) != 0 || legacyMainMenuPromptSeconds <= 0.0f) {
        legacyMainMenuTarget = UIElement::MainMenuNew;
    } else {
        if (!confirmed)
            return result;
        legacyMainMenuTarget = declined ? UIElement::MainMenuNew : UIElement::GameHudMoney;
    }
    legacyMainMenuSelector->field44 = 0;
    return result;
}

// @verified - p
std::int64_t LegacyMainMenuInput() {
    if (static_cast<std::uint32_t>(legacyMainMenuInitState - 2) > 1U)
        return 0;
    if (legacyMainMenuSubmenuActive) {
        if (!ExecuteElementCallback(legacyMainMenuTarget) && (g_inputBuffer[0].pressedPriority & 0x200U) != 0 &&
            DestroyElementByIndex(legacyMainMenuTarget) == 1) {
            legacyMainMenuSubmenuActive = 0;
            g_inputBuffer[0].pressedPriority = 0;
        }
        return 0;
    }
    if ((g_inputBuffer[0].pressedPriority & 0x200U) != 0) {
        g_inputBuffer[0].pressedPriority = 0x200;
        return 1;
    }
    UIButtonWheel &wheel = g_uiButtonWheels[static_cast<std::size_t>(legacyMainMenuWheelIndex)];
    if (legacyMainMenuTarget == UIElement::OptionsGamma) {
        if (UpdateLegacyMainMenuS1Prompt()) {
            if (legacyMainMenuTarget == UIElement::MainMenuNew) {
                ResetTitleImportSelection();
                return 1;
            }
            return 0;
        }
    } else if (const std::int32_t direction = GetButtonWheelDirection(legacyMainMenuWheelIndex); direction != 0) {
        for (std::int32_t ordinal = 0; ordinal < wheel.count; ++ordinal) {
            UIButtonWheelItem &item = wheel.items[static_cast<std::size_t>(ordinal)];
            const std::int32_t previous = item.index;
            item.index += direction;
            if (item.index < 0)
                item.index = wheel.count - 1;
            else if (item.index >= wheel.count)
                item.index = 0;
            if (item.index == 0)
                wheel.selection = ordinal;
            if (item.animSlots[0] == -1)
                item.animSlots[0] = UIAnimAllocSlot();
            if (item.animSlots[0] == -1)
                continue;
            StartUIAngleTransition(item.animSlots[0], wheel.angles[static_cast<std::size_t>(previous)], wheel.angles[static_cast<std::size_t>(item.index)],
                direction, 0.30000001f, 0.0f);
            if (wheel.override == 0)
                continue;
            if (item.animSlots[1] == -1)
                item.animSlots[1] = UIAnimAllocSlot();
            item.index84 += direction;
            if (item.index84 < 0)
                item.index84 = wheel.count - 1;
            if (item.index84 >= wheel.count)
                item.index84 = 0;
            UIAnim_Start(item.animSlots[1], item.placeScale, wheel.scales[static_cast<std::size_t>(item.index84)][0], 0.30000001f, 0.0f);
        }
    }
    std::int32_t mode = 0;
    if (legacyMainMenuTarget == UIElement::GameHudMoney) {
        if (!IsArchiveReadComplete() && !IsArchiveReadPending()) {
            LegacyUICallbackNullsub();
            return 0;
        }
        mode = 2;
    } else {
        if ((g_inputBuffer[0].pressedPriority & 0x400U) == 0)
            return 0;
        legacyMainMenuTarget = static_cast<UIElement>(wheel.selection + static_cast<std::int32_t>(UIElement::MainMenuNew));
        if (legacyMainMenuTarget == UIElement::ShenmueCollection) {
            if (!IsArchiveReadComplete() && !IsArchiveReadPending()) {
                LegacyUICallbackNullsub();
                return 0;
            }
            mode = 1;
        } else {
            if (legacyMainMenuTarget == UIElement::MainMenuContinue) {
                if (!IsArchiveReadComplete() && !IsArchiveReadPending()) {
                    LegacyUICallbackNullsub();
                    return 0;
                }
                SetMainLoadListMode(0);
            }
            const std::int32_t target = static_cast<std::int32_t>(legacyMainMenuTarget);
            if ((static_cast<std::uint32_t>(target - 28) & ~3U) == 0 && legacyMainMenuTarget != UIElement::ShenmueCollection) {
                ShowElementByIndex(legacyMainMenuTarget);
                legacyMainMenuSubmenuActive = 1;
                return 0;
            }
            const std::uint64_t backup = GetLocalBackupInfo();
            if (legacyMainMenuTarget != UIElement::MainMenuNew || (backup & 8U) == 0)
                return 1;
            legacyMainMenuTarget = UIElement::OptionsGamma;
            if (legacyMainMenuSelector) {
                legacyMainMenuSelector->field44 = 1;
                legacyMainMenuSelector->pos = 1;
                legacyMainMenuPromptSeconds = 15.0f;
            }
            return 0;
        }
    }
    SetMainLoadListMode(mode);
    legacyMainMenuTarget = UIElement::MainMenuContinue;
    ShowElementByIndex(legacyMainMenuTarget);
    legacyMainMenuSubmenuActive = 1;
    return 0;
}

// @verified - p
std::int64_t GetLegacyMainMenuInitState() {
    return static_cast<std::uint32_t>(legacyMainMenuInitState);
}

// @verified - p
void ResetLegacyMainMenuWheel() {
    legacyMainMenuSubmenuActive = 0;
    legacyMainMenuTarget = UIElement::MainMenuNew;
    if (legacyMainMenuWheelIndex >= 0 && legacyMainMenuSavedWheel.count)
        g_uiButtonWheels[static_cast<std::size_t>(legacyMainMenuWheelIndex)] = legacyMainMenuSavedWheel;
}

} // namespace shenmue::s2
