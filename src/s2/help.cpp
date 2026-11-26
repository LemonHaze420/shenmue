#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/help_icon_dc.hpp"
#include "s2/help_data_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/warning.hpp"
#include "s2/window.hpp"
#include "s2/font.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/story_state.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/navigation.hpp"
#include "s2/item_wf.hpp"
#include "s2/pause.hpp"
#else
#include "s2/help.hpp"

#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/font.hpp"
#include "s2/input.hpp"
#include "s2/item_wf.hpp"
#include "s2/map_control.hpp"
#include "s2/npc.hpp"
#include "s2/navi.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/warning.hpp"
#include "s2/pak.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

HelpUiState helpState{};

std::uint8_t helpIconIndex, helpPreviousIconIndex{}, helpStringIndex{}, helpIconMode{}, helpShowCount{}, helpSavedIconIndex{};

std::int8_t helpClosingFrames{};

std::array<std::int32_t, 4> helpMenuRequest{};

std::int32_t helpEnabled{};

std::array<char, 128> helpLine{};

char *helpLineCursor{};

std::int16_t helpLegacyRow{}, helpLocationTexture{};

std::uint8_t helpLineCount{}, helpLineIndent{}, helpLineCharacters{}, helpLegacyIconCount{};

std::array<LegacyHelpIcon, 16> helpLegacyIcons{};

LegacyIconSprite helpLegacyTextSprite{}, helpLegacyPauseSprite{}, helpLegacyButtonSprite{};

std::uint32_t helpPauseTexture{};

std::array<char, 0x800> helpTextInput{};

std::array<char, 0x2000> helpTextFormatted{};

std::array<char, 128> helpTextTitle{};

std::array<char, 256> helpTextLocation{};

std::int32_t helpTextLineIndex{}, helpTextFormattedCount{};

std::uint8_t helpFreeQuestGuideFlags{};

std::byte *helpTextWindow{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

void CtrlHelpIcon_Callback_1(void *);

} // namespace

void FormatHelpTopicText(std::string_view text);
HLib::Task *HP_SetUpHelpIcon();
void EV_EventSetUpHelpIcon_0(std::uint8_t topic, std::uint8_t mode);
std::uint32_t GetHelpIconEnabled() noexcept;
const char *GetLocationLabelId(std::uint32_t mapCode);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void HelpNullsub(void *) {
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool NameMatches(std::uint32_t name, const char *code) {
    return std::memcmp(&name, code, 4) == 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
std::uint32_t RasterizeHelpPauseNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) noexcept {
    return 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void helpTextReset() {
    helpTextFormatted.fill(0);
    helpTextFormattedCount = 0;
    if (helpTextWindow) {
        const std::int32_t dirty = 1;
        std::memcpy(helpTextWindow + 0x558, &dirty, sizeof(dirty));
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpTextSetBody(std::string_view text) {
    std::sprintf(helpTextInput.data(), "%.*s", static_cast<int>(text.size()), text.data());
    if (helpTextInput[0] != '[')
        return;
    const char *const at = std::strchr(helpTextInput.data(), '@');
    if (!at)
        return;
    const std::size_t length = static_cast<std::size_t>(at - helpTextInput.data());
    std::strncpy(helpTextTitle.data(), helpTextInput.data(), length);
    if (length >= helpTextTitle.size())
        std::abort();
    helpTextTitle[length] = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpTextSetLocation(const char *label) {
    std::sprintf(helpTextLocation.data(), "%s", label);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
bool helpIsMoveButtonOption() {
    return (GetS2GameConfigurationFlags() & 0x100U) != 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// This native BSS byte has no writer; preserve its storage and short-circuit flag reads.
// @verified - p
bool helpIsFreeQuestGuideActive() {
    constexpr std::uint8_t guideBit = 0x10;
    return !EV_GetShenmueFlag(0x305) && !EV_GetShenmueFlag(0x342) && !EV_GetShenmueFlag(0x375) && !EV_GetShenmueFlag(0x377) && !EV_GetShenmueFlag(0x115) &&
        (guideBit & helpFreeQuestGuideFlags) != 0 && GetCurrentStep() >= 40000U && GetCurrentStep() < 370000U;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// scenIsInArea: the current map, the current author, or a dynamic SCEN name.
bool helpIsInArea(std::uint32_t name) {
    if (name == g_currentMap.name || name == GetCurrentAuthorId().name)
        return true;
    for (const shen_name alias : GetScenarioEventObjectMapNames()) {
        if (alias.name == name)
            return true;
    }
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int64_t GetLoadingFlagState() {
    return (g_loadingFlag >> 4U) & 1U;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void helpDrawLegacyIcon(void *) {
    if (GetDebugForceUIEnabled(UIElement::GameHelp))
        return;
    const std::int16_t *const textures = WRN_GetWarningTextures();
    vfSubmitQuad(0xaf000000, 15.1f);
    helpLegacyPauseSprite.x = 630.0f;
    helpLegacyPauseSprite.y = 204.0f;
    helpLegacyPauseSprite.icon = static_cast<std::uint16_t>(textures[20]);
    PrepareDrawTextureWithMode(helpLegacyPauseSprite, ScreenMode::Centered);
    for (std::size_t row = 0; row < HELP_LEGACY_ROW_OFFSETS.size(); ++row) {
        helpLegacyPauseSprite.y = (HELP_LEGACY_ROW_OFFSETS[row] + 232.0f) + 22.0f;
        helpLegacyPauseSprite.icon = static_cast<std::uint16_t>(textures[22 + row * 2]);
        PrepareDrawTextureWithMode(helpLegacyPauseSprite, ScreenMode::Centered);
    }
    LegacySolidRect rect{.x = 330.0f, .y = 235.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 15.2f, .colour = 0x3b8c0000, .width = 500, .height = 165};
    DrawLegacySolidRect(rect);
    rect.colour = 0xffc04040;
    rect.height = 2;
    rect.y = 235.0f - 82.0f;
    DrawLegacySolidRect(rect);
    rect.y = 235.0f + 82.0f;
    DrawLegacySolidRect(rect);
    rect.width = 2;
    rect.height = 165;
    rect.y = 235.0f;
    rect.x = 330.0f - 250.0f;
    DrawLegacySolidRect(rect);
    rect.x = 330.0f + 250.0f;
    DrawLegacySolidRect(rect);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpPrepareWindow(bool setWindowFlag) {
    KanjiFontLoad("DC_KANJI.FON");
    if (setWindowFlag) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        wndSetFlag(control->windowTask, 1);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// helpUpdateMenuRequest: requests the pause menu when the active pad disconnects.
void helpUpdateMenuRequest() {
    if (helpMenuRequest[0]) {
        if (g_inputBuffer[0].connectionState)
            return;
        helpMenuRequest[0] = 0;
    } else if (g_inputBuffer[0].connectionState) {
        helpMenuRequest[0] = 1;
        helpMenuRequest[1] = 0;
        return;
    }
    if (helpMenuRequest[1])
        return;
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentAuthor) == SHEN_NAME(YORU) && !authGetFinished())
        Author_RequestAbort();
    if (helpMenuRequest[1] || vfqIsBattleQteActive())
        return;
    const bool notPaused = (g_loadingFlag & 0x402U) == 0;
    if (!GetNotPausedOrLoading() && notPaused)
        return;
    if (notPaused)
        SetHelpIconState();
    if (helpIconIndex == HelpSystemMenuSaveOptionsSleep || helpIconIndex == HelpSystemMenuSaveOptions || helpIconIndex == HelpMovesScroll) {
        KanjiFontLoad("DC_KANJI.FON");
    } else if (helpIconIndex == HelpNotebook) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        wndSetFlag(control->windowTask, 1);
    } else {
        helpPrepareWindow(true);
    }
    helpMenuRequest[1] = 1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpSelectTopic() {
    helpState = HelpShowingPrep;
    helpPreviousIconIndex = helpIconIndex;
    const std::uint8_t topic = helpIconIndex;
    if (topic == HelpFreeQuestLookActionMoneyConversation || topic == HelpArcade || static_cast<std::uint8_t>(topic - 44) <= 2) {
        if (!topic) {
            if (helpIsFreeQuestGuideActive()) {
                if (helpIsMoveButtonOption())
                    helpIconIndex = HelpFreeQuestMoveAction;
            } else {
                helpIconIndex = helpIsMoveButtonOption() ? HelpFreeQuestMoveAButtonAction : HelpFreeQuestLookAction;
            }
        }
        const shen_name focus = srchGetCurrentFocus();
        if (focus.name && HasNpcRuntimeEntry(focus) && static_cast<std::uint32_t>(QueryNpcRouteLiveState(focus) - 10) <= 1U) {
            helpIconIndex = HelpGuideFollow;
        }
        if (EV_GetShenmueFlag(0x305))
            helpIconIndex = HelpChaseGuideDestination;
        if (EV_GetShenmueFlag(0x342))
            helpIconIndex = HelpChaseGuideDestination;
        if (EV_GetShenmueFlag(0x2de))
            helpIconIndex = HelpChaseGuideDestination;
        if (EV_GetShenmueFlag(0x105) && (NameMatches(g_currentMap.name, "WB00") || NameMatches(g_currentMap.name, "WT00"))) {
            helpIconIndex = HelpChaseGuideDestination;
        }
        if (EV_GetShenmueFlag(0x375))
            helpIconIndex = HelpChaseGuideDestination;
        if (EV_GetShenmueFlag(0x377))
            helpIconIndex = HelpChaseGuideDestination;
        if (EV_GetShenmueFlag(0x115))
            helpIconIndex = HelpChasingYuan;
        for (const AreaHelpTopicOverride &entry : areaHelpTopicOverrides) {
            if (!entry.area.name)
                break;
            if (helpIsInArea(entry.area.name)) {
                helpIconIndex = static_cast<std::uint8_t>(entry.topic);
                break;
            }
        }
        if (helpIconIndex == HelpSlotHouseTokens && GetInternalFlag(1014))
            helpIconIndex = HelpSlotHousePlay;
        if (camCtrlGetContextVal())
            helpIconIndex = HelpLockOnInvestigateControls;
        if (GetMediatedButtonBindings(SHEN_NAME(BTSP)) && (!camCtrlGetContextVal() || camCtrlGetContextMode() != 10)) {
            helpIconIndex = NameMatches(g_currentMap.name, "WSY0") && !EV_GetShenmueFlag(0x1f6) ? HelpSaveOptionsPrompt : HelpSaveOptionsSleepPrompt;
        }
        const std::uint8_t sleep = GetSleepActionIconFlags();
        if (sleep & 1U)
            helpIconIndex = static_cast<std::uint8_t>((~(8U * sleep) & 0x20U) | 0x0aU);
        if (srchHasActiveLockonTarget() && !IsInvestigateMode())
            helpIconIndex = 0x01;
    }
    if (srchHasActiveLockonTarget() && IsInvestigateMode())
        helpIconIndex = HelpInvestigateObject;
    if (IsModeBitActive(16))
        helpIconIndex = HelpPawnshopFolder;
    switch (GetDailyTaskIndex()) {
    case 1:
        helpIconIndex = HelpPawnshopFliers;
        break;
    case 2:
        helpIconIndex = HelpSellCollectionSets;
        break;
    case 3:
        helpIconIndex = HelpSellSingleItem;
        break;
    default:
        break;
    }
    if (IsModeBitActive(8))
        helpIconIndex = HelpMapFolder;
    if (IsModeBitActive(64))
        helpIconIndex = HelpGeneralMap;
    if (IsWristwatchOpenable())
        helpIconIndex = HelpWristwatch;
    if (IsItemSelectable())
        helpIconIndex = HelpNotebook;
    if (IsEasyOptionActive())
        helpIconIndex = HelpOptions;
    if (IsSaveAllowed())
        helpIconIndex = HelpSaveGameData;
    if (!IsCtrlSpeakIconInactive()) {
        helpIconIndex = GetMediatedButtonBindings(SHEN_NAME(BTMM)) ? HelpActionSelector51XB : HelpActionSelector37XB;
    }
    if (IsNaviActive())
        helpIconIndex = HelpNavigationMark;
    if (GetwfMainControlState()) {
        helpIconIndex = IsModeBitActive(1) ? HelpItemInspect : HelpMovesFolder;
        helpStringIndex = helpIconIndex;
    } else {
        helpStringIndex = helpIconIndex;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpShowWidget() {
    switch (helpIconIndex) {
    case HelpSystemMenuSaveOptionsSleep:
    case HelpSystemMenuSaveOptions:
    case HelpMovesScroll:
        KanjiFontLoad("DC_KANJI.FON");
        break;
    case HelpNotebook: {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        wndSetFlag(control->windowTask, 1);
        break;
    }
    default:
        helpPrepareWindow(true);
        break;
    }
    helpLegacyRow = 0;
    helpLineCursor = helpLine.data();
    helpLineCount = 0;
    helpLineIndent = 0;
    helpLineCharacters = 0;
    helpLegacyIconCount = 0;
    if (!HandleGameUIVisibility() && helpEnabled != 1)
        return;
    ShowElementByIndex(UIElement::GameHelp);
    helpTextReset();
    HelpWidgetResetRuns();
    helpTextLineIndex = 0;
    const std::string_view text = GetLocalizedSceneText(helpStringIds[helpStringIndex]);
    FormatHelpTopicText(text);
    helpTextSetBody(GetLocalizedSceneText(helpStringIds[helpStringIndex]));
    for (; helpLegacyRow < 20; helpLegacyRow = static_cast<std::int16_t>(helpLegacyRow + 2))
        WRN_UploadWarningLine(helpLegacyRow);
    const char *location = GetLocationLabelId(g_currentMap.name);
    const shen_name mapName = GetScenarioEventObjectMapNames()[0];
    const char *const regionLabel = GetLocationLabelId(mapName.name);
    if (const char *const authorLabel = GetLocationLabelId(GetCurrentAuthorId().name))
        location = authorLabel;
    // The contiguous label rows preserve the native pointer-priority order and aliases.
    if (location) {
        if (regionLabel && regionLabel < location)
            location = regionLabel;
    } else {
        location = regionLabel;
    }
    if (location) {
        helpTextSetLocation(location);
        helpLocationTexture = static_cast<std::int16_t>(WRN_UploadWarningLine(28));
    }
    switch (helpIconIndex) {
    case HelpSystemMenuSaveOptionsSleep:
    case HelpSystemMenuSaveOptions:
    case HelpMovesScroll:
        KanjiFontLoad("DC_KANJI.FON");
        break;
    case HelpNotebook: {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        wndSetFlag(control->windowTask, 0);
        break;
    }
    default:
        helpPrepareWindow(true);
        break;
    }
    ++helpShowCount;
    helpState = HelpShowing;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Tail of CtrlHelpIcon_Callback_1 (LABEL_136).
void helpUpdateTail() {
    helpUpdateMenuRequest();
    std::uint32_t flags = g_loadingFlag;
    if (helpMenuRequest[1] && (flags & 0x402U)) {
        if (flags & 0x20U) {
            helpMenuRequest[3] = 1;
            helpMenuRequest[1] = 0;
        } else {
            helpMenuRequest[3] = 0;
            SetInputDeviceSelectionSuspended(1);
            HLib::ExecuteCallbackNow(helpDrawLegacyIcon, nullptr);
            flags = g_loadingFlag;
        }
    }
    if (helpShowCount && (flags & 0x600U) == 0 && helpState == HelpShowing && !GetLoadingFlagState()) {
        ResetSelectedInputDevice();
        CountConnectedInputDevices();
        HLib::ExecuteCallbackNow(HelpNullsub, nullptr);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void CtrlHelpIcon_Callback_1(void *) {
    if (!HandleGameUIVisibility() && !helpEnabled && !IsHelpOverlayVisible(UIElement::GameHelp))
        return;
    const std::int32_t enabled = helpEnabled;
    switch (helpState) {
    case HelpIdle:
        if ((g_loadingFlag & 0xe02U) == 2U || enabled == 1)
            helpSelectTopic();
        break;
    case HelpShowingPrep:
        helpShowWidget();
        if (helpState != HelpShowing)
            return;
        break;
    case HelpReserved2:
        helpShowCount = 0;
        helpState = HelpIdle;
        break;
    case HelpShowing:
        if (((g_loadingFlag & 2U) == 0 || (g_loadingFlag & 0x620U) == 0x20U) && !enabled) {
            helpState = HelpClosing;
            helpClosingFrames = 4;
            DestroyElementByIndex(UIElement::GameHelp);
            if (helpIconIndex == HelpUnknownOn02Help) {
                helpPreviousIconIndex = helpSavedIconIndex;
                helpIconIndex = helpSavedIconIndex;
            }
        }
        break;
    case HelpClosing:
        helpClosingFrames = static_cast<std::int8_t>(static_cast<std::uint8_t>(helpClosingFrames) - 1U);
        if (helpClosingFrames >= 0)
            break;
        helpShowCount = static_cast<std::uint8_t>(helpShowCount - 1U);
        if (helpShowCount & 0x80U)
            helpShowCount = 0;
        helpIconIndex = helpPreviousIconIndex;
        helpState = HelpIdle;
        break;
    case HelpIncrement:
        ++helpShowCount;
        helpState = HelpIdle;
        break;
    case HelpReset:
        helpState = HelpIdle;
        break;
    default:
        break;
    }
    helpUpdateTail();
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void CtrlHelpIcon_Callback(void *) {
    helpLegacyTextSprite.x = 328.0f;
    helpLegacyTextSprite.scaleX = 1.0f;
    helpLegacyTextSprite.scaleY = 1.0f;
    helpLegacyTextSprite.depth = 15.0f;
    helpLegacyTextSprite.flags = 0;
    helpLegacyTextSprite.rotation = 0;
    helpLegacyTextSprite.icon = 0xffff;
    helpLegacyTextSprite.width = 1024;
    helpLegacyTextSprite.height = 32;
    helpLegacyTextSprite.alignment = 5;
    helpLegacyPauseSprite.x = 328.0f;
    helpLegacyPauseSprite.scaleX = 1.0f;
    helpLegacyPauseSprite.scaleY = 1.0f;
    helpLegacyPauseSprite.depth = 15.3f;
    helpLegacyPauseSprite.flags = 0;
    helpLegacyPauseSprite.rotation = 0;
    helpLegacyPauseSprite.icon = 0xffff;
    helpLegacyPauseSprite.width = 1024;
    helpLegacyPauseSprite.height = 32;
    helpLegacyPauseSprite.alignment = 5;
    helpLegacyButtonSprite.x = 96.0f;
    helpLegacyButtonSprite.scaleX = 1.1f;
    helpLegacyButtonSprite.scaleY = 1.1f;
    helpLegacyButtonSprite.depth = 15.0f;
    helpLegacyButtonSprite.flags = 0;
    helpLegacyButtonSprite.rotation = 0;
    helpLegacyButtonSprite.icon = 32;
    helpLegacyButtonSprite.width = 768;
    helpLegacyButtonSprite.height = 32;
    helpLegacyButtonSprite.alignment = 5;
    helpLineCursor = helpLine.data();
    helpLegacyRow = 0;
    helpLineCount = 0;
    helpLineIndent = 0;
    helpLineCharacters = 0;
    helpLegacyIconCount = 0;
    HLib::SetCurrentTaskTo(CtrlHelpIcon_Callback_1);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpCommitLine() {
    *helpLineCursor = 0;
    if (GetDebugForceUIEnabled(UIElement::GameHelp))
        HelpWidgetParseLine(helpLine.data());
    else
        WRN_UploadWarningLine(helpLegacyRow);
    helpLegacyRow = static_cast<std::int16_t>(helpLegacyRow + 2);
    ++helpLineCount;
    helpLineCursor = helpLine.data();
    helpLineIndent = 0;
    helpLineCharacters = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void helpAppend(std::string_view bytes) {
    std::memcpy(helpLineCursor, bytes.data(), bytes.size());
    helpLineCursor += bytes.size();
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t helpIconCode(char code) {
    switch (code) {
    case '0':
        return 23;
    case '2':
        return 20;
    case '4':
        return 17;
    case '5':
        return 21;
    case '6':
        return 18;
    case '7':
        return 22;
    case '8':
        return 19;
    case '9':
        return 25;
    case 'A':
        return 2;
    case 'B':
        return 3;
    case 'C':
        return 0;
    case 'D':
        return 26;
    case 'G':
        return 16;
    case 'H':
        return 15;
    case 'K':
        return 14;
    case 'L':
        return 6;
    case 'M':
        return 9;
    case 'N':
        return 1;
    case 'P':
        return 13;
    case 'Q':
        return g_s2JapaneseVersion != 0 ? 7 : 6;
    case 'R':
        return 7;
    case 'S':
        return 8;
    case 'T':
        return 10;
    case 'U':
        return 24;
    case 'W':
        return g_s2JapaneseVersion == 0 ? 7 : 6;
    case 'X':
        return 4;
    case 'Y':
        return 5;
    case 'b':
        return 12;
    case 'w':
        return 11;
    default:
        return 0;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::size_t DecodeHelpCodepoint(std::string_view text, std::uint32_t &codepoint) {
    if (!text.data() || text.empty())
        return 0;
    codepoint = 0xfffd;
    const std::uint8_t lead = static_cast<std::uint8_t>(text.front());
    std::size_t kind = 0;
    for (; kind < HELP_UTF8_MASKS.size(); ++kind) {
        if ((lead & HELP_UTF8_MASKS[kind]) == HELP_UTF8_PREFIXES[kind])
            break;
    }
    if (kind == 0 || kind >= HELP_UTF8_MASKS.size())
        return 1;
    std::uint32_t value = lead & static_cast<std::uint8_t>(~HELP_UTF8_MASKS[kind]);
    std::size_t consumed = 1;
    while (consumed < kind && consumed < text.size()) {
        const std::uint8_t next = static_cast<std::uint8_t>(text[consumed]);
        std::size_t nextKind = 0;
        for (; nextKind < HELP_UTF8_MASKS.size(); ++nextKind) {
            if ((next & HELP_UTF8_MASKS[nextKind]) == HELP_UTF8_PREFIXES[nextKind])
                break;
        }
        const std::uint32_t part = nextKind < HELP_UTF8_MASKS.size() ? next & static_cast<std::uint8_t>(~HELP_UTF8_MASKS[nextKind]) : 0;
        value = (value << 6U) | part;
        if (nextKind != 0)
            return consumed;
        ++consumed;
    }
    if (consumed < kind)
        return 0;
    if (value >= HELP_UTF8_MINIMUM[kind] && value <= HELP_UTF8_MAXIMUM[kind] && (value - 0xd800U) > 0x7ffU)
        codepoint = value;
    return kind;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// FormatHelpTopicText: splits the topic text into lines ('&'), button icons
// ('^' + code, at most 16) and escaped characters ('@').
void FormatHelpTopicText(std::string_view text) {
    FormatColoursForHelpTopic(helpStringIndex);
    while (!text.empty()) {
        std::uint32_t codepoint = 0;
        const std::size_t length = DecodeHelpCodepoint(text, codepoint);
        if (!length)
            break;
        const char ch = static_cast<char>(codepoint);
        if (length != 1 || codepoint >= 0x80) {
            helpAppend(text.substr(0, length));
            ++helpLineCharacters;
            text.remove_prefix(length);
            continue;
        }
        text.remove_prefix(1);
        if (ch == '&') {
            helpCommitLine();
            continue;
        }
        if (ch == '^') {
            if (GetDebugForceUIEnabled(UIElement::GameHelp))
                helpAppend("^");
            if (helpLegacyIconCount == 16)
                continue;
            std::uint32_t codepoint = 0;
            const std::size_t codeLength = DecodeHelpCodepoint(text, codepoint);
            const char code = static_cast<char>(codepoint);
            if (codeLength)
                text.remove_prefix(codeLength);
            const std::int32_t icon = helpIconCode(code);
            LegacyHelpIcon &legacy = helpLegacyIcons[helpLegacyIconCount];
            legacy.line = helpLineCount;
            legacy.icon = static_cast<std::uint8_t>(icon);
            legacy.x = static_cast<float>(helpLineCharacters) * 14.0f + 42.0f + 10.0f;
            if (helpLineIndent == 1)
                legacy.x += 52.0f;
            else if (helpLineIndent == 2)
                legacy.x += 42.0f;
            HelpWidgetAddButtonIcon(icon);
            if (code != 'D')
                ++helpLegacyIconCount;
            continue;
        }
        if (ch == '@') {
            std::uint32_t skipped = 0;
            const std::size_t skipLength = DecodeHelpCodepoint(text, skipped);
            if (skipLength)
                text.remove_prefix(skipLength);
            continue;
        }
        helpAppend(std::string_view(&ch, 1));
        ++helpLineCharacters;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint8_t helpIconIndex, helpPreviousIconIndex = 0, helpIconMode = 0;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EV_EventSetUpHelpIcon_0(std::uint8_t topic, std::uint8_t mode) {
    if (topic >= kNativeHelpTopicCount)
        topic = 0;
    helpIconIndex = helpPreviousIconIndex = topic;
    helpIconMode = mode;
}

#else

void EV_EventSetUpHelpIcon_0(std::uint8_t topic, std::uint8_t mode) {
    helpIconMode = mode;
    if (helpIconIndex != HelpUnknownOn02Help)
        helpSavedIconIndex = helpIconIndex;
    const std::uint8_t clamped = topic < HelpTopicCount ? topic : HelpFreeQuestLookActionMoneyConversation;
    helpPreviousIconIndex = clamped;
    helpIconIndex = clamped;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetCurrentHelpString(std::uint8_t topic) {
    helpEnabled = 1;
    if (helpStringIds[topic]) {
        helpStringIndex = topic;
        helpState = HelpShowingPrep;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void DisableHelpIcon() {
    helpEnabled = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static std::uint8_t helpState, helpStringIndex;

static std::int8_t helpShowCount, helpClosingFrames;

static char helpLine[52], *helpLineCursor;

static std::int16_t helpLegacyRow, helpLocationWidth;

static LegacyIconSprite helpLegacyTextSprite, helpLegacyButtonSprite;

static const float helpRowOffsets[11] = {-176.0f, -139.0f, -102.0f, -65.0f, -28.0f, 9.0f, 46.0f, 83.0f, 120.0f, 157.0f, 204.0f};

static const std::uint16_t helpButtonIcons[11] = {0x300, 0x306, 0x308, 0x309, 0x30a, 0x30b, 0x304, 0x305, 0x801, 0, 0x400};

static const std::uint32_t helpButtonColours[11] = {
    0xff0f0f0fU, 0xff0f0f0fU, 0xff3f0000U, 0xff00003fU, 0xff3f3f00U, 0xff003f00U, 0xbf000000U, 0xbf000000U, 0xff0f0f0fU, 0xff000000U, 0xff000000U};

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void PrepareHelpWindow(std::uint8_t close) {
    switch (helpIconIndex) {
    case 19:
        if (close)
            wndSetFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, 0);
        else
            wndSetFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, 1);
        break;
    case 10:
    case 51:
        KanjiFontLoad("DC_KANJI.FON");
        break;
    default:
        KanjiFontLoad("DC_KANJI.FON");
        wndSetFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, 1);
        break;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void DrawLegacyColourRectAspect(const LegacyColourRect &rect);
void DrawMainIconOverlay(std::uint32_t colour, float depth);
std::uint32_t camCtrlIsFlag21Set();
std::uint8_t UpdateSelectorMemo();

// @verified - d
static void DrawHelpFrame(std::int8_t verticalMode, std::int8_t palette, std::int8_t xIndex, std::int8_t row, std::int8_t widthIndex, std::int8_t heightIndex) {
    static const float topY[10] = {56.0f, 93.0f, 130.0f, 167.0f, 204.0f, 241.0f, 278.0f, 315.0f, 352.0f, 389.0f};
    static const float bottomY[10] = {75.0f, 112.0f, 149.0f, 186.0f, 223.0f, 260.0f, 297.0f, 334.0f, 371.0f, 0.0f};
    static const float xPositions[4] = {320.0f, 165.0f, 475.0f, 210.0f};
    static const std::int16_t heights[9] = {43, 80, 117, 154, 191, 228, 265, 302, 339}, widths[3] = {580, 290, 380};
    static const std::uint32_t fills[4] = {0x3b8c0000U, 0x3b00008cU, 0x3b008c00U, 0x3b8c8c00U};
    static const std::uint32_t borders[4] = {0xffc04040U, 0xff4040c0U, 0xff40c040U, 0xffc0c040U};
    LegacyColourRect fill, border;
    fill.x = xPositions[xIndex];
    fill.width = widths[widthIndex];
    fill.height = heights[heightIndex];
    fill.scaleX = 1.0f;
    fill.scaleY = 1.0f;
    fill.rotation = 0;
    fill.colour = fills[palette];
    fill.depth = 14.0f;
    if (!verticalMode)
        fill.y = topY[row];
    else
        fill.y = bottomY[row];
    border.width = static_cast<std::int16_t>(widths[widthIndex] + 2.0f);
    border.height = 2;
    border.scaleX = 1.0f;
    border.scaleY = 1.0f;
    border.rotation = 0;
    border.colour = borders[palette];
    border.depth = 14.0f;
    border.x = xPositions[xIndex];
    if (!verticalMode)
        border.y = (topY[row] - heights[heightIndex] * 0.5f) - 1.0f;
    else
        border.y = (bottomY[row] - heights[heightIndex] * 0.5f) - 1.0f;
    DrawLegacyColourRectAspect(fill);
    DrawLegacyColourRectAspect(border);
    if (!verticalMode)
        border.y = topY[row] + heights[heightIndex] * 0.5f + 1.0f;
    else
        border.y = bottomY[row] + heights[heightIndex] * 0.5f + 1.0f;
    DrawLegacyColourRectAspect(border);
    border.x = (xPositions[xIndex] - widths[widthIndex] * 0.5f) - 1.0f;
    if (!verticalMode)
        border.y = topY[row];
    else
        border.y = bottomY[row];
    border.width = 2;
    border.height = static_cast<std::int16_t>(heights[heightIndex] + 2.0f);
    border.colour = borders[palette];
    DrawLegacyColourRectAspect(border);
    border.x = xPositions[xIndex] + widths[widthIndex] * 0.5f + 1.0f;
    DrawLegacyColourRectAspect(border);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void DrawHelpButton(std::int8_t row, std::int8_t icon, float x) {
    helpLegacyButtonSprite.x = x;
    helpLegacyButtonSprite.y = 232.0f;
    helpLegacyButtonSprite.width = 32;
    helpLegacyButtonSprite.height = 32;
    helpLegacyButtonSprite.scaleX = 1.1f;
    helpLegacyButtonSprite.scaleY = 1.1f;
    if (icon == 8) {
        helpLegacyButtonSprite.width = 64;
        helpLegacyButtonSprite.height = 64;
        helpLegacyButtonSprite.scaleX = 1.0f;
        helpLegacyButtonSprite.scaleY = 0.8f;
        helpLegacyButtonSprite.y += 5.0f;
    }
    helpLegacyButtonSprite.icon = helpButtonIcons[icon];
    helpLegacyButtonSprite.y = helpRowOffsets[row] + 232.0f;
    PrepareDrawSimpleOpaque(helpLegacyButtonSprite, helpButtonColours[icon]);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void DrawHelpMarker(std::int32_t row, float x, float offset) {
    LegacyColourRect rect;
    rect.width = 10;
    rect.height = 10;
    rect.scaleX = 1.0f;
    rect.scaleY = 1.0f;
    rect.rotation = 0;
    rect.colour = 0xffff0000U;
    rect.depth = 17.0f;
    rect.x = x;
    rect.y = (helpRowOffsets[static_cast<std::int8_t>(row)] + 232.0f) + offset;
    DrawLegacyColourRectAspect(rect);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static const std::uint32_t helpAreaTopics[16][2] = {{SHEN_NAME4('A', 'R', 'P', '0'), 29}, {SHEN_NAME4('W', 'S', 'P', '0'), 29},
    {SHEN_NAME4('W', 'K', 'P', '0'), 29}, {SHEN_NAME4('W', 'K', 'P', '1'), 29}, {SHEN_NAME4('W', 'T', 'P', '0'), 29}, {SHEN_NAME4('W', 'T', 'P', '1'), 29},
    {SHEN_NAME4('W', 'T', 'P', '2'), 29}, {SHEN_NAME4('W', 'T', 'P', '3'), 29}, {SHEN_NAME4('W', 'T', 'P', '4'), 29}, {SHEN_NAME4('W', 'R', 'P', '0'), 29},
    {SHEN_NAME4('Q', 'B', '1', '0'), 29}, {SHEN_NAME4('Q', 'B', '1', '1'), 29}, {SHEN_NAME4('W', 'E', 'G', '0'), 37}, {SHEN_NAME4('W', 'E', 'S', '1'), 37},
    {SHEN_NAME4('Q', 'A', '2', '2'), 37}, {0, 0}};

static void SelectHelpTopicForArea() {
    for (const std::uint32_t (*rule)[2] = helpAreaTopics; (*rule)[0]; ++rule) {
        if (scenIsInArea((*rule)[0])) {
            helpIconIndex = static_cast<std::uint8_t>((*rule)[1]);
            break;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void HelpUnusedNullsub() {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void DrawNativeHelpPanel(void *) {
    std::int32_t index, row;
    DrawMainIconOverlay(0x9f000000U, 13.0f);
    const std::int16_t *const textures = WRN_GetWarningTextures();
    index = 0;
    do {
        if (static_cast<std::int16_t>(helpLegacyTextSprite.icon = textures[index]) >= 0) {
            if (index & 1) {
                helpLegacyTextSprite.width = 128;
                helpLegacyTextSprite.x = 608.0f;
            } else {
                helpLegacyTextSprite.width = 512;
                helpLegacyTextSprite.x = 288.0f;
            }
            helpLegacyTextSprite.y = helpRowOffsets[index / 2] + 232.0f;
            PrepareDrawTextureWithMode(helpLegacyTextSprite);
        }
    } while (++index < 20);
    LegacyIconSprite &text = helpLegacyTextSprite;
    if (static_cast<std::int16_t>(text.icon = textures[20]) >= 0) {
        text.width = 1024;
        text.x = static_cast<float>(832 - helpLocationWidth / 2);
        text.y = helpRowOffsets[10] + 232.0f;
        PrepareDrawTextureWithMode(text);
    }
    LegacyIconSprite &button = helpLegacyButtonSprite;
    button.x = 96.0f;
    button.y = 232.0f;
    button.depth = 16.0f;
    button.width = 32;
    button.height = 32;
    button.scaleX = 1.1f;
    button.scaleY = 1.1f;
    button.rotation = 0;
    DrawHelpButton(0, 8, 503.0f);
    switch (helpStringIndex) {
    case 0:
    case 30:
    case 38:
    case 43:
        DrawHelpFrame(0, 0, 1, 3, 1, 2);
        DrawHelpFrame(0, 2, 2, 3, 1, 2);
        DrawHelpFrame(0, 1, 1, 7, 1, 2);
        if (helpStringIndex == 38 || helpStringIndex == 43)
            DrawHelpFrame(0, 3, 2, 7, 1, 2);
        else
            DrawHelpFrame(1, 3, 2, 7, 1, 3);
        if (GetNormalTriggerMapping()) {
            DrawHelpButton(6, 6, 60.0f);
            DrawHelpButton(2, 7, 60.0f);
        } else {
            DrawHelpButton(6, 7, 60.0f);
            DrawHelpButton(2, 6, 60.0f);
        }
        DrawHelpButton(2, 2, 375.5f);
        DrawHelpButton(3, 0, 60.0f);
        DrawHelpButton(3, 3, 375.5f);
        DrawHelpButton(4, 3, 60.0f);
        DrawHelpButton(6, 4, 375.5f);
        DrawHelpButton(8, 1, 60.0f);
        DrawHelpButton(8, 5, 375.5f);
        break;
    case 1:
        DrawHelpFrame(1, 2, 1, 3, 1, 1);
        DrawHelpFrame(0, 1, 2, 4, 1, 2);
        DrawHelpButton(3, 4, 60.0f);
        DrawHelpButton(3, 7, 375.5f);
        DrawHelpButton(4, 3, 60.0f);
        DrawHelpButton(4, 1, 375.5f);
        DrawHelpButton(5, 0, 375.5f);
        break;
    case 2:
        DrawHelpFrame(1, 2, 1, 3, 1, 1);
        DrawHelpFrame(1, 1, 2, 3, 1, 1);
        DrawHelpButton(3, 4, 60.0f);
        DrawHelpButton(3, 7, 375.5f);
        DrawHelpButton(4, 3, 60.0f);
        DrawHelpButton(4, 0, 375.5f);
        break;
    case 5:
        DrawHelpButton(6, 4, 60.0f);
        DrawHelpButton(8, GetNormalTriggerMapping() ? 7 : 6, 60.0f);
        break;
    case 8:
        DrawHelpButton(2, 0, 60.0f);
        break;
    case 9:
        DrawHelpButton(3, 0, 60.0f);
        DrawHelpButton(4, 7, 60.0f);
        break;
    case 6:
        DrawHelpFrame(0, 2, 1, 5, 1, 0);
        DrawHelpFrame(1, 1, 2, 6, 1, 3);
        DrawHelpButton(5, 4, 60.0f);
        DrawHelpButton(5, 6, 375.5f);
        DrawHelpButton(6, 7, 375.5f);
        DrawHelpButton(7, 1, 375.5f);
        DrawHelpButton(8, 0, 375.5f);
        break;
    case 7:
        DrawHelpFrame(0, 0, 1, 5, 1, 0);
        DrawHelpFrame(0, 1, 2, 5, 1, 0);
        DrawHelpFrame(0, 2, 2, 7, 1, 0);
        DrawHelpButton(5, 0, 60.0f);
        DrawHelpButton(5, 1, 375.5f);
        DrawHelpButton(7, 4, 375.5f);
        break;
    case 10:
        DrawHelpFrame(0, 2, 0, 3, 0, 2);
        break;
    case 13:
        DrawHelpButton(6, 3, 216.0f);
        break;
    case 16:
        DrawHelpFrame(1, 2, 0, 4, 0, 1);
        DrawHelpButton(4, 4, 60.0f);
        DrawHelpButton(4, 5, 372.0f);
        DrawHelpButton(5, 2, 60.0f);
        break;
    case 3:
        DrawHelpFrame(0, 2, 1, 6, 1, 0);
        DrawHelpButton(6, 2, 60.0f);
        break;
    case 37:
        DrawHelpButton(1, 4, 240.2f);
        DrawHelpFrame(0, 2, 1, 5, 1, 0);
        DrawHelpButton(5, 4, 60.0f);
        break;
    case 17:
        row = 5;
        DrawHelpFrame(0, 0, 1, row, 1, 0);
        DrawHelpFrame(1, 2, 2, row, 1, 1);
        DrawHelpButton(row, 0, 60.0f);
        DrawHelpButton(row, 2, 375.5f);
        DrawHelpButton(row + 1, 3, 375.5f);
        break;
    case 18:
        DrawHelpFrame(0, 1, 1, 5, 1, 2);
        DrawHelpFrame(0, 2, 2, 4, 1, 0);
        DrawHelpButton(4, 6, 60.0f);
        DrawHelpButton(5, 7, 60.0f);
        DrawHelpButton(6, 0, 60.0f);
        DrawHelpButton(4, 3, 375.5f);
        DrawHelpButton(8, 2, 56.0f);
        break;
    case 19:
        row = 4;
        DrawHelpFrame(0, 0, 1, row + 1, 1, 2);
        DrawHelpFrame(0, 2, 2, row, 1, 0);
        DrawHelpButton(row, 0, 60.0f);
        DrawHelpButton(row + 1, 0, 60.0f);
        DrawHelpButton(row + 2, 0, 60.0f);
        DrawHelpButton(row, 3, 375.5f);
        DrawHelpMarker(row, 48.0f, 0.0f);
        DrawHelpMarker(row, 72.0f, 0.0f);
        DrawHelpMarker(row + 1, 60.0f, -12.0f);
        DrawHelpMarker(row + 2, 60.0f, 12.0f);
        break;
    case 14:
        DrawHelpFrame(0, 0, 1, 5, 1, 0);
        DrawHelpFrame(1, 2, 2, 6, 1, 3);
        DrawHelpFrame(0, 3, 1, 7, 1, 0);
        DrawHelpButton(5, 0, 60.0f);
        DrawHelpButton(5, 2, 375.5f);
        DrawHelpButton(6, 3, 375.5f);
        DrawHelpButton(7, 9, 60.0f);
        DrawHelpButton(7, 4, 375.5f);
        DrawHelpButton(8, 5, 375.5f);
        break;
    case 20:
        row = 2;
        DrawHelpFrame(0, 0, 1, row + 1, 1, 2);
        DrawHelpFrame(1, 2, 2, row + 1, 1, 3);
        DrawHelpButton(row, 6, 60.0f);
        DrawHelpButton(row, 2, 375.5f);
        DrawHelpButton(row + 1, 0, 60.0f);
        DrawHelpButton(row + 1, 3, 375.5f);
        DrawHelpButton(row + 2, 4, 375.5f);
        DrawHelpButton(row + 3, 5, 375.5f);
        break;
    case 24:
        row = 3;
        DrawHelpFrame(0, 3, 1, row, 1, 0);
        DrawHelpFrame(0, 2, 2, row, 1, 0);
        DrawHelpButton(row, 2, 60.0f);
        DrawHelpButton(row, 3, 375.5f);
        helpLegacyButtonSprite.icon = helpButtonIcons[10];
        helpLegacyButtonSprite.width = 128;
        helpLegacyButtonSprite.height = 64;
        helpLegacyButtonSprite.scaleX = 0.55f;
        helpLegacyButtonSprite.scaleY = 0.55f;
        helpLegacyButtonSprite.x = 96.0f;
        helpLegacyButtonSprite.y = helpRowOffsets[1] + 232.0f;
        helpLegacyButtonSprite.x += 155.1f;
        PrepareDrawSimpleOpaque(helpLegacyButtonSprite, helpButtonColours[10]);
        break;
    case 25:
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpFrame(1, 2, 2, 4, 1, 1);
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(4, 4, 375.5f);
        DrawHelpButton(5, 3, 375.5f);
        break;
    case 28:
        DrawHelpFrame(1, 2, 2, 8, 1, 1);
        DrawHelpFrame(1, 0, 1, 8, 1, 1);
        DrawHelpButton(8, 0, 60.0f);
        DrawHelpButton(8, 4, 375.5f);
        DrawHelpButton(9, 5, 60.0f);
        DrawHelpButton(9, 3, 375.5f);
        break;
    case 51:
        DrawHelpFrame(1, 0, 1, 4, 1, 1);
        DrawHelpFrame(1, 1, 2, 4, 1, 1);
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(4, 1, 375.5f);
        DrawHelpButton(5, 2, 60.0f);
        DrawHelpButton(5, 3, 375.5f);
        DrawHelpButton(9, 2, 60.0f);
        break;
    case 27:
    case 111:
        DrawHelpFrame(0, 2, 0, 6, 0, 0);
        for (index = 2; index < 5; ++index)
            DrawHelpButton(6, index, (index - 1) * 49.0f + 246.0f);
        break;
    case 4:
    case 35:
    case 112:
        row = helpStringIndex == 35 ? 5 : 6;
        DrawHelpFrame(0, 2, 0, row, 0, 0);
        DrawHelpButton(row, 0, 296.0f);
        for (index = 2; index < 6; ++index)
            DrawHelpButton(row, index, helpLegacyButtonSprite.x + 49.0f);
        break;
    case 36:
    case 50:
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpFrame(0, 3, 2, 4, 1, 0);
        DrawHelpButton(4, 0, 60.0f);
        for (index = 6; index < 8; ++index) {
            DrawHelpButton(4, index, (index - 6) * 33.0f + 362.6f);
            DrawHelpButton(6, index, (index - 5) * 45.0f + 2.0f);
        }
        row = 8;
        DrawHelpButton(row, 6, 44.0f);
        DrawHelpButton(row, 7, 99.0f);
        if (helpStringIndex == 50)
            DrawHelpButton(row + 1, 5, 60.0f);
        break;
    case 46:
        DrawHelpButton(1, GetNormalTriggerMapping() ? 6 : 7, 188.0f);
        DrawHelpButton(3, 2, 136.0f);
        DrawHelpButton(8, GetNormalTriggerMapping() ? 7 : 6, 60.0f);
        break;
    case 47:
    case 76:
        DrawHelpFrame(1, 2, 0, 7, 0, 3);
        DrawHelpButton(1, 2, 60.0f);
        break;
    case 48:
        DrawHelpFrame(0, 2, 0, 8, 0, 0);
        DrawHelpButton(8, 2, 60.0f);
        DrawHelpButton(8, 3, 372.0f);
        break;
    case 11:
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpFrame(1, 2, 2, 4, 1, 1);
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(4, 2, 375.5f);
        DrawHelpButton(5, 3, 375.5f);
        break;
    case 45:
        DrawHelpButton(1, 2, 215.0f);
        break;
    case 52:
        DrawHelpFrame(1, 0, 3, 4, 2, 1);
        DrawHelpButton(4, 1, 56.0f);
        DrawHelpButton(5, 2, 56.0f);
        DrawHelpButton(5, 3, 101.0f);
        DrawHelpButton(5, 4, 146.0f);
        DrawHelpButton(5, 5, 191.0f);
        DrawHelpButton(7, 3, 241.0f);
        break;
    case 53:
        DrawHelpFrame(1, 0, 0, 4, 0, 1);
        DrawHelpButton(4, 1, 60.0f);
        DrawHelpButton(4, 7, 375.5f);
        DrawHelpButton(5, 6, 375.5f);
        DrawHelpButton(7, 3, 241.0f);
        break;
    case 22:
        DrawHelpFrame(1, 0, 0, 4, 0, 1);
        DrawHelpButton(4, 1, 60.0f);
        DrawHelpButton(4, 7, 375.5f);
        DrawHelpButton(5, 6, 375.5f);
        DrawHelpButton(7, 3, 241.0f);
        break;
    case 23:
        DrawHelpFrame(0, 0, 0, 4, 0, 2);
        DrawHelpButton(3, 1, 60.0f);
        DrawHelpButton(4, 7, 60.0f);
        DrawHelpButton(5, 6, 60.0f);
        DrawHelpButton(3, 2, 375.5f);
        DrawHelpButton(4, 3, 375.5f);
        DrawHelpButton(7, 3, 241.0f);
        break;
    case 57:
        DrawHelpButton(4, 4, 56.0f);
        break;
    case 60:
        DrawHelpButton(3, 2, 60.0f);
        break;
    case 12:
        DrawHelpFrame(0, 0, 1, 3, 1, 0);
        DrawHelpFrame(0, 2, 2, 3, 1, 0);
        DrawHelpButton(3, 0, 60.0f);
        DrawHelpButton(3, 2, 375.5f);
        DrawHelpButton(3, 3, 428.5f);
        break;
    case 61:
        row = 6;
        DrawHelpButton(4, 6, 56.0f);
        DrawHelpButton(4, 7, 101.0f);
        DrawHelpFrame(0, 0, 1, row, 1, 0);
        DrawHelpFrame(1, 2, 2, row, 1, 1);
        DrawHelpButton(row, 0, 60.0f);
        DrawHelpButton(row, 2, 375.5f);
        DrawHelpButton(row + 1, 3, 375.5f);
        break;
    case 65:
        DrawHelpButton(1, 2, 60.0f);
        break;
    case 66:
        DrawHelpFrame(0, 2, 0, 4, 0, 0);
        DrawHelpButton(4, 2, 61.0f);
        DrawHelpButton(4, 4, 326.0f);
        break;
    case 67:
        row = 4;
        DrawHelpFrame(1, 0, 1, row, 1, 1);
        DrawHelpFrame(0, 2, 2, row, 1, 0);
        DrawHelpButton(row, 2, 376.0f);
        DrawHelpButton(row, 0, 60.0f);
        DrawHelpButton(row + 1, 0, 60.0f);
        DrawHelpMarker(row, 48.0f, 0.0f);
        DrawHelpMarker(row, 72.0f, 0.0f);
        DrawHelpMarker(row + 1, 60.0f, -12.0f);
        DrawHelpMarker(row + 1, 60.0f, 12.0f);
        DrawHelpButton(row + 3, 5, 60.0f);
        break;
    case 68:
        DrawHelpButton(2, 2, 164.0f);
        break;
    case 69:
        row = 3;
        DrawHelpButton(row, 0, 60.0f);
        DrawHelpMarker(row, 60.0f, -12.0f);
        DrawHelpMarker(row, 60.0f, 12.0f);
        DrawHelpButton(row + 1, 2, 60.0f);
        break;
    case 70:
        DrawHelpButton(2, 0, 268.0f);
        DrawHelpButton(4, 0, 242.0f);
        DrawHelpMarker(4, 242.0f, -12.0f);
        break;
    case 71:
        DrawHelpButton(2, 0, 60.0f);
        DrawHelpButton(3, 2, 60.0f);
        break;
    case 72:
        DrawHelpButton(2, 0, 60.0f);
        DrawHelpButton(3, 2, 60.0f);
        break;
    case 73:
        DrawHelpButton(1, 2, 242.0f);
        DrawHelpButton(4, 2, 60.0f);
        DrawHelpButton(7, 2, 60.0f);
        break;
    case 75:
        DrawHelpButton(1, 1, 60.0f);
        DrawHelpButton(5, 2, 60.0f);
        break;
    case 77:
        DrawHelpButton(1, 2, 164.0f);
        DrawHelpButton(3, 0, 60.0f);
        DrawHelpMarker(3, 60.0f, -12.0f);
        DrawHelpButton(4, GetNormalTriggerMapping() ? 7 : 6, 60.0f);
        break;
    case 49:
        DrawHelpFrame(0, 0, 1, 3, 1, 0);
        DrawHelpFrame(1, 2, 2, 3, 1, 1);
        DrawHelpButton(3, 0, 60.0f);
        DrawHelpButton(3, 2, 375.5f);
        DrawHelpButton(4, 3, 375.5f);
        break;
    case 78:
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpFrame(1, 2, 2, 4, 1, 1);
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(4, 2, 375.5f);
        DrawHelpButton(5, 3, 375.5f);
        break;
    case 79:
        row = 3;
        DrawHelpFrame(0, 0, 1, row + 2, 1, 4);
        DrawHelpFrame(0, 2, 2, row, 1, 0);
        DrawHelpButton(row, 0, 60.0f);
        DrawHelpMarker(row, 60.0f, -12.0f);
        DrawHelpMarker(row, 60.0f, 12.0f);
        DrawHelpButton(row + 1, 0, 60.0f);
        DrawHelpMarker(row + 1, 48.0f, 0.0f);
        DrawHelpButton(row + 2, 0, 60.0f);
        DrawHelpMarker(row + 2, 72.0f, 0.0f);
        DrawHelpButton(row + 3, 6, 60.0f);
        DrawHelpButton(row + 4, 7, 60.0f);
        DrawHelpButton(row, 3, 375.5f);
        break;
    case 80:
        DrawHelpFrame(0, 0, 1, 7, 1, 4);
        DrawHelpFrame(1, 2, 2, 5, 1, 1);
        DrawHelpButton(5, 0, 60.0f);
        DrawHelpMarker(5, 60.0f, -12.0f);
        DrawHelpMarker(5, 60.0f, 12.0f);
        DrawHelpButton(6, 0, 60.0f);
        DrawHelpMarker(6, 48.0f, 0.0f);
        DrawHelpButton(7, 0, 60.0f);
        DrawHelpMarker(7, 72.0f, 0.0f);
        DrawHelpButton(8, 6, 60.0f);
        DrawHelpButton(9, 7, 60.0f);
        DrawHelpButton(5, 2, 375.5f);
        DrawHelpButton(6, 3, 375.5f);
        DrawHelpButton(3, 2, 60.0f);
        break;
    case 81:
    case 82:
        DrawHelpFrame(0, 0, 1, 8, 1, 0);
        DrawHelpFrame(1, 2, 2, 8, 1, 1);
        DrawHelpButton(8, 0, 60.0f);
        DrawHelpButton(8, 6, 375.5f);
        DrawHelpButton(9, 7, 375.5f);
        DrawHelpButton(2, 2, 60.0f);
        DrawHelpButton(3, 3, 60.0f);
        DrawHelpButton(6, 2, 60.0f);
        break;
    case 85:
        DrawHelpFrame(0, 0, 1, 7, 1, 0);
        DrawHelpFrame(1, 2, 2, 7, 1, 1);
        DrawHelpButton(7, 0, 60.0f);
        DrawHelpButton(7, 4, 375.5f);
        DrawHelpButton(8, 3, 375.5f);
        DrawHelpButton(1, 4, 268.0f);
        break;
    case 88:
        DrawHelpButton(4, 2, 242.0f);
        DrawHelpButton(5, GetNormalTriggerMapping() ? 7 : 6, 60.0f);
        break;
    case 89:
    case 93:
    case 95:
    case 97:
    case 99:
    case 103:
    case 106:
    case 109:
        DrawHelpFrame(0, 2, 0, 8, 0, 2);
        DrawHelpButton(7, 6, 112.0f);
        DrawHelpButton(7, 5, 424.0f);
        DrawHelpButton(8, 4, 398.0f);
        DrawHelpButton(8, 3, 450.0f);
        DrawHelpButton(9, 0, 112.0f);
        DrawHelpButton(9, 2, 424.0f);
        break;
    case 91:
        DrawHelpFrame(0, 2, 0, 8, 0, 2);
        DrawHelpButton(3, 3, 294.0f);
        DrawHelpButton(7, 6, 112.0f);
        DrawHelpButton(7, 5, 424.0f);
        DrawHelpButton(8, 4, 398.0f);
        DrawHelpButton(8, 3, 450.0f);
        DrawHelpButton(9, 0, 112.0f);
        DrawHelpButton(9, 2, 424.0f);
        break;
    case 92:
        DrawHelpButton(3, 3, 294.0f);
        DrawHelpButton(4, 3, 372.0f);
        break;
    case 94:
    case 96:
        DrawHelpButton(3, 5, 86.0f);
        break;
    case 100:
        DrawHelpButton(4, 0, 86.0f);
        DrawHelpButton(4, 5, 164.0f);
        DrawHelpFrame(0, 2, 0, 8, 0, 2);
        DrawHelpButton(7, 6, 112.0f);
        DrawHelpButton(7, 5, 424.0f);
        DrawHelpButton(8, 4, 398.0f);
        DrawHelpButton(8, 3, 450.0f);
        DrawHelpButton(9, 0, 112.0f);
        DrawHelpButton(9, 2, 424.0f);
        break;
    case 90:
        DrawHelpFrame(0, 2, 0, 7, 0, 0);
        DrawHelpButton(2, 0, 56.0f);
        DrawHelpButton(7, 0, 86.0f);
        DrawHelpButton(7, 4, 398.0f);
        break;
    case 113:
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(5, 4, 60.0f);
        DrawHelpButton(8, 3, 268.0f);
        DrawHelpButton(9, 5, 60.0f);
        break;
    case 118:
        DrawHelpButton(5, 0, 60.0f);
        break;
    case 117:
        DrawHelpButton(7, 3, 60.0f);
        DrawHelpButton(7, GetNormalTriggerMapping() ? 7 : 6, 294.0f);
        break;
    case 119:
    case 120:
    case 121:
    case 122:
    case 123:
    case 131:
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(4, 2, 294.0f);
        DrawHelpButton(6, 0, 60.0f);
        DrawHelpButton(6, 2, 398.0f);
        break;
    case 124:
    case 125:
    case 126:
    case 139:
    case 140:
    case 141:
    case 142:
        DrawHelpButton(5, 0, 60.0f);
        DrawHelpButton(5, 2, 294.0f);
        DrawHelpButton(7, 0, 60.0f);
        DrawHelpButton(7, 2, 398.0f);
        break;
    case 127:
    case 128:
    case 129:
    case 130:
        DrawHelpButton(7, 0, 60.0f);
        DrawHelpButton(7, 2, 294.0f);
        DrawHelpButton(9, 0, 60.0f);
        DrawHelpButton(9, 2, 398.0f);
        break;
    case 132:
    case 133:
    case 134:
    case 135:
    case 136:
    case 137:
        DrawHelpButton(8, 0, 60.0f);
        DrawHelpButton(8, 2, 294.0f);
        DrawHelpButton(9, 0, 60.0f);
        DrawHelpButton(9, 2, 398.0f);
        break;
    case 143:
    case 146:
        DrawHelpButton(7, 0, 60.0f);
        DrawHelpButton(7, 2, 398.0f);
        break;
    case 144:
        DrawHelpButton(3, 0, 60.0f);
        DrawHelpButton(3, 2, 294.0f);
        break;
    case 147:
        DrawHelpButton(4, 0, 60.0f);
        DrawHelpButton(4, 2, 320.0f);
        break;
    case 138:
        DrawHelpButton(3, 2, 60.0f);
        DrawHelpButton(4, 4, 60.0f);
        DrawHelpButton(5, 5, 60.0f);
        break;
    default:
        break;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void FormatNativeHelpText(const char *text) {
    char value;
    while ((value = *text++) != 0) {
        if (value == '0') {
            *helpLineCursor = 0;
            WRN_UploadWarningLine(helpLegacyRow, helpLine);
            helpLineCursor = helpLine;
            helpLegacyRow += 2;
        } else if (value == '\t') {
            *helpLineCursor++ = '\t';
        } else if (value >= '1' && value <= '8') {
            *helpLineCursor++ = static_cast<char>(value - '0');
        } else if (value >= 'A' && value <= 'Z') {
            FormatNativeHelpText(helpStrings[value - 'A']);
        } else if (value >= 'a' && value <= 'z') {
            FormatNativeHelpText(helpStrings[value - 'a' + 26]);
        } else {
            *helpLineCursor++ = static_cast<char>(value);
            *helpLineCursor++ = *text++;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static const char *FindHelpLocationLabel(int name) {
    union {
        int value;
        char characters[4];
    } key, area;
    const HelpLocationLabel *label = helpLocationLabels;
    const char *text = 0;
    key.value = name;
    while ((area.value = label->area) != 0) {
        if ((area.characters[0] == '*' || area.characters[0] == key.characters[0]) && (area.characters[1] == '*' || area.characters[1] == key.characters[1]) &&
            (area.characters[2] == '*' || area.characters[2] == key.characters[2]) && (area.characters[3] == '*' || area.characters[3] == key.characters[3])) {
            text = label->text;
            break;
        }
        ++label;
    }
    return text;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetHelpIconEnabled() {
    return 1;
}

#else

std::uint32_t GetHelpIconEnabled() noexcept {
    return static_cast<std::uint32_t>(helpEnabled);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint8_t helpGetCurrentHelp() {
    return helpStringIndex;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint8_t helpGetCurrentIconIdx(std::int32_t slot) {
    return slot == 1 ? helpPreviousIconIndex : helpIconMode;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetupCurrentHelpIcon() {
    helpLineCursor = helpLine.data();
    helpLegacyRow = 0;
    helpLineCount = 0;
    helpLineIndent = 0;
    helpLineCharacters = 0;
    helpLegacyIconCount = 0;
    helpStringIndex = helpIconIndex;
    HelpWidgetResetRuns();
    FormatHelpTopicText(GetLocalizedSceneText(helpStringIds[helpStringIndex]));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

const char *GetLocationLabelId(std::uint32_t mapCode) {
    for (const HelpLocationLabel &entry : helpLocationLabels) {
        if (!entry.mapCode.name)
            break;
        bool match = true;
        for (std::size_t index = 0; index < 4; ++index) {
            const std::uint8_t expected = static_cast<std::uint8_t>(entry.mapCode.name >> (index * 8));
            if (expected != '*' && expected != static_cast<std::uint8_t>(mapCode >> (index * 8)))
                match = false;
        }
        if (match)
            return entry.labelId;
    }
    return nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void HelpPanelCallback(void *) {
    switch (helpState) {
    case 0:
        if (GetHelpIconEnabled() && (g_loadingFlag & 0xe02U) == 2U) {
            helpState = 1;
            helpPreviousIconIndex = helpIconIndex;
            switch (helpIconIndex) {
            case 0:
            case 37:
            case 45:
            case 46:
                if (!helpIconIndex) {
                    if (IsScenarioFollowPromptAvailable()) {
                        if (GetS2ControlMode())
                            helpIconIndex = 30;
                    } else
                        helpIconIndex = GetS2ControlMode() ? 43 : 38;
                }
                SelectHelpTopicForArea();
                if (camCtrlIsFlag21Set())
                    helpIconIndex = 46;
                if (GetMediatedButtonBindings(SHEN_NAME4('B', 'T', 'S', 'P')))
                    helpIconIndex = 45;
                if (GetSleepActionIconFlags() & 1U)
                    helpIconIndex = 10;
                const std::uint32_t step = GetCurrentStep();
                if (step - 120100U <= 99U || step - 170000U <= 99U)
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x2de))
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x105))
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x375))
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x376))
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x377))
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x378))
                    helpIconIndex = 13;
                if (EV_GetShenmueFlag(0x115))
                    helpIconIndex = 15;
                break;
            }
            if (srchHasActiveLockonTarget()) {
                if (srchIsPropIKActive())
                    helpIconIndex = 2;
                else
                    helpIconIndex = 1;
            }
            if (IsModeBitActive(0x10))
                helpIconIndex = 78;
            if (IsModeBitActive(8))
                helpIconIndex = 116;
            if (IsModeBitActive(0x40))
                helpIconIndex = 83;
            if (IsWristwatchOpenable())
                helpIconIndex = 24;
            if (UpdateSelectorMemo())
                helpIconIndex = 19;
            if (IsEasyOptionActive())
                helpIconIndex = 12;
            if (IsSaveAllowed())
                helpIconIndex = 11;
            if (!IsCtrlSpeakIconInactive())
                helpIconIndex = GetMediatedButtonBindings(SHEN_NAME4('B', 'T', 'M', 'M')) ? 50 : 36;
            if (IsNaviActive())
                helpIconIndex = 138;
            if (GetwfMainControlState())
                helpIconIndex = 60;
            PrepareHelpWindow(0);
            helpStringIndex = helpIconIndex;
        }
        break;
    case 1: {
        helpLineCursor = helpLine;
        helpLegacyRow = 0;
        const char *const *topics = helpStrings + 26;
        FormatNativeHelpText(topics[helpStringIndex]);
        while (helpLegacyRow < 20) {
            WRN_UploadWarningLine(helpLegacyRow, "");
            helpLegacyRow += 2;
        }
        const char *location = 0;
        shen_name name = drGetCurrentAuthorId();
        if (name)
            location = FindHelpLocationLabel(drGetCurrentAuthorId());
        if (!location) {
            name = scenFindMapRegionName(0);
            if (name)
                location = FindHelpLocationLabel(scenFindMapRegionName(0));
        }
        if (!location)
            location = FindHelpLocationLabel(g_currentMap);
        helpLocationWidth = static_cast<std::int16_t>(WRN_UploadWarningLine(20, location ? location : ""));
        helpState = 3;
        helpShowCount = static_cast<std::int8_t>(static_cast<std::uint8_t>(helpShowCount) + 1U);
        break;
    }
    case 3:
        if (!(g_loadingFlag & 2U) || (g_loadingFlag & 0x620U) == 0x20U) {
            PrepareHelpWindow(1);
            helpState = 4;
            helpClosingFrames = 4;
        }
        break;
    case 4:
        if (--helpClosingFrames < 0) {
            helpState = 0;
            helpShowCount = static_cast<std::int8_t>(static_cast<std::uint8_t>(helpShowCount) - 1U);
            if (helpShowCount < 0)
                helpShowCount = 0;
            helpIconIndex = helpPreviousIconIndex;
        }
        break;
    case 2:
        helpShowCount = 0;
        helpState = 0;
        break;
    case 5:
        helpState = 0;
        helpShowCount = static_cast<std::int8_t>(static_cast<std::uint8_t>(helpShowCount) + 1U);
        break;
    case 7:
        helpState = 0;
        break;
    default:
        break;
    }
    if (helpShowCount && !(g_loadingFlag & 0x600U) && helpState == 3 && !IsLoadingFlag10Set())
        HLib::ExecuteCallbackNow(DrawNativeHelpPanel, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void InitializeHelpPanel(void *) {
    helpLegacyTextSprite.flags = 0;
    helpLegacyTextSprite.x = 328.0f;
    helpLegacyTextSprite.width = 1024;
    helpLegacyTextSprite.height = 32;
    helpLegacyTextSprite.scaleX = 1.0f;
    helpLegacyTextSprite.scaleY = 1.0f;
    helpLegacyTextSprite.rotation = 0;
    helpLegacyTextSprite.depth = 15.0f;
    helpLegacyTextSprite.alignment = 5;
    helpLegacyButtonSprite.icon = *static_cast<const volatile std::uint16_t *>(helpButtonIcons);
    helpLegacyButtonSprite.flags = 0;
    helpLegacyButtonSprite.x = 96.0f;
    helpLegacyButtonSprite.width = 32;
    helpLegacyButtonSprite.height = 32;
    helpLegacyButtonSprite.scaleX = 1.1f;
    helpLegacyButtonSprite.scaleY = 1.1f;
    helpLegacyButtonSprite.rotation = 0;
    helpLegacyButtonSprite.depth = 16.0f;
    helpLegacyButtonSprite.alignment = 5;
    HLib::SetCurrentTaskTo(HelpPanelCallback);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
HLib::Task *HP_SetUpHelpIcon() {
    helpState = 0;
    helpIconIndex = helpPreviousIconIndex = helpStringIndex = 0;
    helpIconMode = 0;
    helpShowCount = 0;
    helpClosingFrames = 0;
    return HLib::EnqueueTaskWithoutParameter(InitializeHelpPanel, 15, 0, SHEN_NAME(HELP));
}

#else

HLib::Task *HP_SetUpHelpIcon() {
    helpState = HelpIdle;
    helpMenuRequest = {1, 0, 0, 0};
    helpStringIndex = HelpFreeQuestLookActionMoneyConversation;
    helpPreviousIconIndex = 0;
    helpIconIndex = HelpFreeQuestLookActionMoneyConversation;
    helpIconMode = 0;
    helpShowCount = 0;
    helpClosingFrames = 0;
    std::byte *const ptx = static_cast<std::byte *>(HLib::AllocateTaskData(HELP_PTX_WORK_BYTES, HELP_PTX_WORK));
    ParseGuiPtx(ptx, 256, 16, TASK_HELP.name, HELP_PAUSE_TEXTURE.name);
    helpPauseTexture = static_cast<std::uint32_t>(ReadTexnTexture(ptx));
    RasterizeHelpPauseNullsub(ptx, 256, "Game paused", 0, 1);
    ReplacePtxTexture(helpPauseTexture, ptx);
    HLib::FreeTaskData(ptx);
    return HLib::EnqueueTaskWithoutParameter(CtrlHelpIcon_Callback, 15, 0, static_cast<std::int32_t>(TASK_HELP.name));
}

#endif

} // namespace s2
} // namespace shenmue
