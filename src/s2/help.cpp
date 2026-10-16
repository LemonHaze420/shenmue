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

namespace shenmue::s2 {
namespace {

HelpUiState helpState{};
std::uint8_t helpIconIndex{}, helpPreviousIconIndex{}, helpStringIndex{}, helpIconMode{}, helpShowCount{}, helpSavedIconIndex{};
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

void HelpNullsub(void *) {
}

bool NameMatches(std::uint32_t name, const char *code) {
    return std::memcmp(&name, code, 4) == 0;
}

// @verified - p
std::uint32_t RasterizeHelpPauseNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) noexcept {
    return 0;
}

// @verified - p
void helpTextReset() {
    helpTextFormatted.fill(0);
    helpTextFormattedCount = 0;
    if (helpTextWindow) {
        const std::int32_t dirty = 1;
        std::memcpy(helpTextWindow + 0x558, &dirty, sizeof(dirty));
    }
}

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

void helpTextSetLocation(const char *label) {
    std::sprintf(helpTextLocation.data(), "%s", label);
}

// @verified - p
bool helpIsMoveButtonOption() {
    return (GetS2GameConfigurationFlags() & 0x100U) != 0;
}


// @verified - p
bool helpIsFreeQuestGuideActive() {
    constexpr std::uint8_t guideBit = 0x10;
    return !EV_GetShenmueFlag(0x305) && !EV_GetShenmueFlag(0x342) && !EV_GetShenmueFlag(0x375) && !EV_GetShenmueFlag(0x377) && !EV_GetShenmueFlag(0x115) &&
        (guideBit & helpFreeQuestGuideFlags) != 0 && GetCurrentStep() >= 40000U && GetCurrentStep() < 370000U;
}

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

std::int64_t GetLoadingFlagState() {
    return (g_loadingFlag >> 4U) & 1U;
}

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

void helpPrepareWindow(bool setWindowFlag) {
    KanjiFontLoad("DC_KANJI.FON");
    if (setWindowFlag) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        wndSetFlag(control->windowTask, 1);
    }
}

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

void helpAppend(std::string_view bytes) {
    std::memcpy(helpLineCursor, bytes.data(), bytes.size());
    helpLineCursor += bytes.size();
}

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

void EV_EventSetUpHelpIcon_0(std::uint8_t topic, std::uint8_t mode) {
    helpIconMode = mode;
    if (helpIconIndex != HelpUnknownOn02Help)
        helpSavedIconIndex = helpIconIndex;
    const std::uint8_t clamped = topic < HelpTopicCount ? topic : HelpFreeQuestLookActionMoneyConversation;
    helpPreviousIconIndex = clamped;
    helpIconIndex = clamped;
}

void SetCurrentHelpString(std::uint8_t topic) {
    helpEnabled = 1;
    if (helpStringIds[topic]) {
        helpStringIndex = topic;
        helpState = HelpShowingPrep;
    }
}

void DisableHelpIcon() {
    helpEnabled = 0;
}

std::uint32_t GetHelpIconEnabled() noexcept {
    return static_cast<std::uint32_t>(helpEnabled);
}

std::uint8_t helpGetCurrentHelp() {
    return helpStringIndex;
}

std::uint8_t helpGetCurrentIconIdx(std::int32_t slot) {
    return slot == 1 ? helpPreviousIconIndex : helpIconMode;
}

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

} // namespace shenmue::s2
