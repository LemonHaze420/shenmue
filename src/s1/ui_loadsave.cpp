#include "s1/ui_internal.hpp"

namespace shenmue::s1 {
void UI_ICSVRequest(std::int8_t flags);
}

namespace shenmue::s1::ui_runtime_detail {
std::int32_t SaveTextLanguage();

void ConstructSaveDataInfo(const S1SaveSummary &summary, std::int32_t slot);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void CreateStringForSaveDataMenuLabel(char *destination, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t dayOfWeek, std::int32_t hour,
    std::int32_t minute, std::int32_t second, std::uint8_t mode);

const char *GetIDForAreaID(std::uint32_t areaId);
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void LoadSaveShowMessage(const char *stringId, float seconds);

std::int32_t UI_GetLoadedLanguage();

void LoadSaveGrid_UpdateSelectedSlotPreview();

void LoadSaveGrid_OnSlotSelected(float *rect, void *data);

void SaveGame(UIRetry70ManBattlePromptResult response);

void LoadGame(UIRetry70ManBattlePromptResult response);

void LoadOrSaveGame(std::int32_t slot);

void LoadSaveGrid_OnSlotConfirmed(float *, void *data);

void *LoadSaveGrid_GetSlotData(void *elements, std::int32_t index);

void LoadSave_SetTextureAlphas(float front, float game);

void LoadSaveGrid_DrawSlot(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *, std::int32_t index);

void LoadScreen_ReleaseHeader();

void LoadScreen_BindHeader();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void LoadLoadingScreenImage(std::int32_t name, std::int32_t loadIndex, std::int32_t specialDateIndex, std::int32_t year, std::int32_t month, std::int32_t day,
    std::int32_t dayOfWeek, std::int32_t hours, std::int32_t minutes, bool pm, const char *filename, UICdttMode mode);
}

namespace shenmue::s1::ui_runtime_detail {

// The Load Screen's Nuklear windows, drawn while the element is shown or its windows fade: each bound window at its alpha, its labels
// with the texts GenerateLoadingScreenText_0 binds (TIME, DATE, AREA, SUBAREA and LOADING) and Image_Header with the area's header once
// SetRenderShaderFlags binds it. The labels use the subtitle font; UILoadScreenFont and the H1/H2 sizes are not modelled.

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

void UI_ICSVRequest(std::int8_t flags) {
    SleepSaveTrainingMenu &menu = g_sleepSaveTrainingMenu;
    menu.requested = 1;
    menu.scriptResult = 0;
    if (flags != -1)
        menu.flags = flags;
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {


std::int32_t SaveTextLanguage() {
    return g_uiLoadedLanguage < 0 ? 0 : g_uiLoadedLanguage;
}

void ConstructSaveDataInfo(const S1SaveSummary &summary, std::int32_t slot) {
    SaveSlotInfo &row = g_saveSlotInfo[static_cast<std::size_t>(slot)];
    row.slotIndex = slot;
    row.valid = 1;
    CreateStringForSaveDataMenuLabel(row.gameTime.data(), summary.gameTime.year, summary.gameTime.month, summary.gameTime.day, summary.gameTime.dayOfWeek,
        summary.gameTime.hour, summary.gameTime.minute, summary.gameTime.second, 2);
    CreateStringForSaveDataMenuLabel(row.saveTime.data(), summary.systemTime.year, summary.systemTime.month, summary.systemTime.day,
        summary.systemTime.dayOfWeek, summary.systemTime.hour, summary.systemTime.minute, summary.systemTime.second, 0);
    if (summary.saveState == 6) {
        std::snprintf(
            row.placeName.data(), row.placeName.size(), "%s", InitAndRetrieveStringFromID("UIMARECLDA")); // @text EN: "Cleared Game File" | JA: "クリアデータ"
        return;
    }
    if (!summary.areaId) {
        std::snprintf(row.placeName.data(), row.placeName.size(), "%s", "INVALID SAVE: AREA CODE IS 0");
        return;
    }
    std::array<char, 256> name{};
    const char *const location = GetIDForAreaID(summary.areaId);
    std::snprintf(name.data(), name.size(), "%s", location ? location : "");
    for (std::size_t index = 0; index + 1 < name.size() && name[index] && name[index + 1]; ++index) {
        if (name[index] == '\xa1' && name[index + 1] == '\xf5') {
            name[index] = '\0';
            break;
        }
    }
    row.reserved0c = 0;
    row.areaId = summary.areaId;
    std::snprintf(row.placeName.data(), row.placeName.size(), summary.numberOfSaves == 99 ? "DBG: %s" : "%s", name.data());
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

void GenerateLoadingScreenText(
    char *destination, std::size_t size, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t dayOfWeek, std::uint8_t mode) {
    const std::int32_t shortYear = year > 1900 ? year - 1900 : year;
    const std::uint32_t displayYear = static_cast<std::uint32_t>((mode & 4) != 0 ? shortYear : shortYear + 1900);
    const std::int32_t language = SaveTextLanguage();
    const char *const monthName = month >= 1 && month <= 12 ? InitAndRetrieveStringFromID(kLoadMonthIds[static_cast<std::size_t>(month - 1)]) : "";
    if (language == 1 || (language >= 4 && language <= 6))
        std::snprintf(destination, size, "%u/%02u/%02u", displayYear, static_cast<unsigned>(month), day);
    else if (language == 2 && (mode & 1) != 0)
        std::snprintf(destination, size, "%02u %s %u", day, monthName, displayYear);
    else if (language == 3 && (mode & 1) != 0)
        std::snprintf(destination, size, "%02u. %s %u", day, monthName, displayYear);
    else if (language == 2 || language == 3)
        std::snprintf(destination, size, "%02u/%02u/%u", day, static_cast<unsigned>(month), displayYear);
    else if ((mode & 1) != 0)
        std::snprintf(destination, size, "%s. %02u, %u", monthName, day, displayYear);
    else
        std::snprintf(destination, size, "%02u/%02u/%d", day, static_cast<unsigned>(month), static_cast<std::int32_t>(displayYear));
    if ((mode & 2) != 0 && dayOfWeek >= 0 && dayOfWeek < static_cast<std::int32_t>(kLoadDayIds.size())) {
        const char *const space = language >= 4 && language <= 6 ? "" : " ";
        std::array<char, 32> suffix{};
        std::snprintf(suffix.data(), suffix.size(), "%s(%s)", space, InitAndRetrieveStringFromID(kLoadDayIds[static_cast<std::size_t>(dayOfWeek)]));
        const std::size_t length = std::strlen(destination);
        std::snprintf(destination + length, size - length, "%s", suffix.data());
    }
}

void CreateStringForSaveDataMenuLabel(char *destination, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t dayOfWeek, std::int32_t hour,
    std::int32_t minute, std::int32_t second, std::uint8_t mode) {
    std::array<char, 256> date{};
    std::array<char, 256> time{};
    GenerateLoadingScreenText(date.data(), date.size(), year, month, day, dayOfWeek, mode);
    if ((mode & 8) != 0)
        std::snprintf(time.data(), time.size(), "%02u:%02u:%02u", static_cast<unsigned>(hour), static_cast<unsigned>(minute), static_cast<unsigned>(second));
    else
        std::snprintf(time.data(), time.size(), "%02u:%02u", static_cast<unsigned>(hour), static_cast<unsigned>(minute));
    const std::int32_t language = SaveTextLanguage();
    const bool dateFirst = language >= 4 && language <= 6;
    std::snprintf(destination, 128, "%s %s", dateFirst ? date.data() : time.data(), dateFirst ? time.data() : date.data());
}

const char *GetIDForAreaID(std::uint32_t areaId) {
    for (const AreaLocationName &entry : kAreaLocationNames) {
        std::uint32_t name = 0;
        std::memcpy(&name, entry.area, sizeof(name));
        if (name == areaId)
            return InitAndRetrieveStringFromID(entry.stringId);
    }
    return nullptr;
}

void CreateSaveDataInfoForMenu(std::int32_t loadType) {
    const bool useSummaries = (loadType & ~3) == 0 && loadType != 2;
    for (std::int32_t slot = 0; slot < static_cast<std::int32_t>(g_saveSlotInfo.size()); ++slot) {
        const S1SaveSummary *const summary = useSummaries ? HPLG_GetSaveSummary(static_cast<std::size_t>(slot)) : nullptr;
        if (summary && summary->numberOfSaves)
            ConstructSaveDataInfo(*summary, slot);
        else
            g_saveSlotInfo[static_cast<std::size_t>(slot)].valid = 0;
    }
}

void UpdateInGameStatus() {
    const std::int32_t inGame = g_iIsInGame;
    if (inGame == g_isInGame)
        return;
    g_isInGame = inGame;
    for (UIBaseRuntime &element : g_uiElements) {
        if ((!element.setup || element.config.init == 3) && element.getContentPanel)
            element.getContentPanel();
    }
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

// LoadOrSaveGame's and CreateSaveDataInfo's timed Information Bar message: the current message with the localized text, without its colour
// override, for the given seconds (d3t_ShowMessageStr).
void LoadSaveShowMessage(const char *stringId, float seconds) {
    InformationBarMessage &message = g_informationBarLabels.message;
    message.text = InitAndRetrieveStringFromID(stringId);
    message.textLiteral = 1;
    message.colourOverride = 0;
    d3t::ShowMessageStr(&message, seconds);
}


std::int32_t UI_GetLoadedLanguage() {
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = 0;
        ReadSubtitleGlyphsAndSubs(0);
    }
    return g_uiLoadedLanguage;
}





void LoadSaveGrid_UpdateSelectedSlotPreview() {
    const std::int32_t selected = UIGrid_SelectedIndex(g_loadSaveGrid);
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiLoadSave].config.uiHandleIndex);
    if (selected < 0 || selected >= 10 || !g_saveSlotInfo[static_cast<std::size_t>(selected)].valid) {
        if (g_loadSaveMapImage && g_loadSaveAreaImage) {
            g_loadSaveMapImage->image = 7;
            g_loadSaveAreaImage->image = 7;
        }
        return;
    }
    if (!g_loadSaveMapImage)
        return;
    const std::int32_t map = g_saveSlotInfo[static_cast<std::size_t>(selected)].reserved0c + 25 * UI_GetLoadedLanguage();
    const d3t::UIViewImage *const mapImage = map < 25 ? g_loadSaveMapImages[static_cast<std::size_t>(map)] : g_loadSaveViewImages[5];
    g_loadSaveMapImage->image = UI_FindViewImage(slot, mapImage->name);
    const std::int32_t language = UI_GetLoadedLanguage();
    const d3t::UIViewImage *areaImage = nullptr;
    if (6 * language >= 6) {
        if (!g_loadSaveAreaImage)
            return;
        areaImage = g_loadSaveViewImages[4];
    } else {
        areaImage = g_loadSaveAreaImages[static_cast<std::size_t>(6 * language)];
    }
    g_loadSaveAreaImage->image = UI_FindViewImage(slot, areaImage->name);
}




void LoadSaveGrid_OnSlotSelected(float *rect, void *data) {
    InformationBarMessage &message = g_informationBarLabels.message;
    message.outlinePanel = -1;
    message.noOutline = g_isInGame == 0;
    message.outline = {rect[0], rect[1], rect[2], rect[3]};
    const std::int32_t selected = UIGrid_SelectedIndex(g_loadSaveGrid);
    if (selected >= 0 && selected < 10)
        message.text = !g_isInGame ?
            "UIMAINHLCN" :
            g_saveSlotInfo[static_cast<std::size_t>(selected)].valid ?
            "UIMAINHLSO" :
            "UIMAINHLSC"; // @text UIMAINHLCN EN: "Load and start a saved Game Data." | JA: "記録したゲームデータを読み込みます。" ; UIMAINHLSO EN: "Overwrite the save in this slot." | JA: "このスロットのゲームデータを上書きします。" ; UIMAINHLSC EN: "Create a new game save in this slot." | JA: "このスロットに新しくゲームデータをセーブします。"
    message.textLiteral = 0;
    d3t::CopyStringToUILabel(&message);
    for (std::int32_t index = 0; index < g_loadSaveGrid.elementCount; ++index) {
        if (data == (index >= 10 ? nullptr : &g_saveSlotInfo[static_cast<std::size_t>(index)])) {
            LoadSaveGrid_UpdateSelectedSlotPreview();
            break;
        }
    }
}

void SaveGame(UIRetry70ManBattlePromptResult response) {
    if (response != UIRetry70ManBattlePromptResult::Yes)
        return;
    BeginMovesScreenViewTransition();
    if (g_movesScreenViewActive == 1) {
        (void)VFMA_RequestMove(VFMoveRequestFlags::ReturnAbsolute);
        g_movesScreenTransitionFlags = 0;
        g_pendingMovePopupMoveId = 0;
        g_pendingMovePopupAnimSetId = 0;
        g_movesScreenViewActive = 0;
    }
    const std::int32_t slot = g_currentSlotIndex;
    if (slot < 0 || slot > 9)
        return;
    SaveGame_0(slot);
    g_hasSaved = 1;
    if (g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_SLEEP_SAVE || g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_ALT_SAVE)
        g_loadSaveConfirmLocked = true;
    PlayGatedUiSound(g_loadSaveSaveSoundFrame, kPlaySuccessSound);
    }

void LoadGame(UIRetry70ManBattlePromptResult response) {
    if (response != UIRetry70ManBattlePromptResult::Yes)
        return;
    const std::int32_t slot = g_currentSlotIndex;
    if (slot < 0 || slot > 9)
        return;
    // SteamStats(slot) is the third-party achievement hook.
    g_loadRequested = 1;
    if (g_loadType == 3) {
        g_actNumber = 3;
        g_is70ManBattleLoad = 2;
        g_70ManBattleLoadBeforeSettings = g_settingFlagsCaptured == 0;
    }
}


void LoadOrSaveGame(std::int32_t slot) {
    const SaveSlotInfo &row = g_saveSlotInfo[static_cast<std::size_t>(slot)];
    g_currentSlotIndex = slot;
    if (g_saveOrLoad) {
        if (g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_SAVE_GRID && (kSaveGridSaveBlocked || g_saveScreenFlag != 1)) {
            PlayGatedUiSound(g_loadSaveBlockedSoundFrame, kUiNavigateRejectSound);
        } else if (row.valid) {
            d3t::MessageBox("UISAVECONF", false, 0, SaveGame, 0.0f); // @text EN: "Are you sure you want to save?" | JA: "セーブしてもよろしいですか？"
        } else {
            SaveGame(UIRetry70ManBattlePromptResult::Yes);
        }
        return;
    }
    const S1SaveSummary *const summary = HPLG_GetSaveSummary(static_cast<std::size_t>(slot));
    if (summary && summary->saveState == 6 && g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_TITLE_LOAD) {
        LoadSaveShowMessage("UICANTLOADCG",
            3.0f); // @text EN: "Cleared game files cannot be loaded\nSelect another game file or select New Game from the Start Menu" | JA: "クリア済みのゲームデータはロードできません。別の\nデータを選択するか、新しくゲームを始めてください。"
    } else if (row.valid) {
        d3t::MessageBox(
            "UILOADDOYO", false, 1, LoadGame, 0.0f); // @text EN: "Do you want to load this game?" | JA: "このゲームデータを\nロードしてもよろしいですか？"
    }
}



void LoadSaveGrid_OnSlotConfirmed(float *, void *data) {
    const SaveSlotInfo *const slot = static_cast<const SaveSlotInfo *>(data);
    if (!slot)
        return;
    if ((slot->valid || g_saveOrLoad == 1) && !g_loadSaveConfirmLocked) {
        LoadOrSaveGame(static_cast<std::int32_t>(slot - g_saveSlotInfo.data()));
    } else {
        PlayGatedUiSound(g_loadSaveRejectFrame, kUiRejectSound);
    }
}


void *LoadSaveGrid_GetSlotData(void *elements, std::int32_t index) {
    return elements && index < 10 ? &g_saveSlotInfo[static_cast<std::size_t>(index)] : nullptr;
}

std::uint32_t LoadSave_ParseDefinition() {
    return LoadPresentationDefinition(kUiLoadSave, "/ui/mainmenu_new/LoadScreen.ui");
}



void LoadSave_SetTextureAlphas(float front, float game) {
    for (const std::size_t index : {std::size_t{1}, std::size_t{4}}) {
        if (UIColourImageWidget *const image = g_loadSaveColourImages[index])
            image->alpha = front;
    }
    for (const std::size_t index : {std::size_t{0}, std::size_t{2}, std::size_t{3}}) {
        if (UIColourImageWidget *const image = g_loadSaveColourImages[index])
            image->alpha = game;
    }
}








std::int64_t LoadSave_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiLoadSave].config.uiHandleIndex);
    const auto viewImage = [slot](const char *id) -> d3t::UIViewImage * {
        const std::int32_t image = UI_FindViewImage(slot, id);
        return image == -1 || slot > 0x4bU ? nullptr : &g_uiViewSlots[slot].images[static_cast<std::size_t>(image)];
    };
    for (std::size_t window = 0; window < g_loadSaveWindows.size(); ++window) {
        const char *const id = kLoadSaveWindowIds[window];
        g_loadSaveWindows[window] = id && id[0] ? UI_GetWindow(UI_FindViewWindow(slot, id)) : nullptr;
    }
    for (std::size_t image = 0; image < g_loadSaveViewImages.size(); ++image)
        g_loadSaveViewImages[image] = viewImage(kLoadSaveViewImageIds[image]);
    for (std::size_t image = 0; image < g_loadSaveColourImages.size(); ++image)
        g_loadSaveColourImages[image] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kLoadSaveColourImageIds[image]));
    for (std::size_t item = 0; item < g_loadSaveLayoutItems.size(); ++item) {
        const std::int32_t index = UI_FindViewSpacePush(slot, kLoadSaveLayoutItemIds[item]);
        g_loadSaveLayoutItems[item] = static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiLayoutItems[static_cast<std::size_t>(index)];
    }
    for (std::size_t label = 0; label < g_loadSaveLabels.size(); ++label) {
        d3t::UILabelWidget *const record = UI_LabelRecord(UI_FindViewLabel(slot, kLoadSaveLabelIds[label]));
        g_loadSaveLabels[label] = record;
        if (record)
            record->text = g_loadSaveLabelTexts[label].data();
    }
    for (std::size_t button = 0; button < g_loadSaveButtons.size(); ++button) {
        const std::uint32_t index = static_cast<std::uint32_t>(UI_FindViewButton(slot, kLoadSaveButtonIds[button]));
        g_loadSaveButtons[button] = index < g_uiButtonWidgets.size() ? &g_uiButtonWidgets[index] : nullptr;
    }
    for (std::size_t image = 0; image < g_loadSaveMapImages.size(); ++image)
        g_loadSaveMapImages[image] = viewImage(kLoadSaveMapImageIds[image]);
    for (std::size_t image = 0; image < g_loadSaveAreaImages.size(); ++image)
        g_loadSaveAreaImages[image] = viewImage(kLoadSaveAreaImageIds[image]);
    const auto imageWidget = [slot](const char *id) -> d3t::UIImageWidget * {
        const std::int32_t index = UI_FindViewImageWidget(slot, id);
        return static_cast<std::uint32_t>(index) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(index)] : nullptr;
    };
    g_loadSaveMapImage = imageWidget("LoadScreen::Image_Map");
    g_loadSaveAreaImage = imageWidget("LoadScreen::Image_Area");
    g_loadSaveGridHolder = UI_GetWindow(UI_FindViewWindow(slot, "LoadScreen::Panel_GridHolder"));
    UIGrid &grid = g_loadSaveGrid;
    grid.columns = 1;
    grid.visibleRows = 4;
    grid.rows = 10;
    grid.showEmptyCells = 1;
    grid.wrap = 0;
    grid.pagedScroll = 0;
    grid.style = nullptr;
    grid.holder = g_loadSaveWindows[0];
    grid.rect = {};
    grid.drawCell = LoadSaveGrid_DrawSlot;
    grid.mouseCallbacks = {
        .entered = LoadSaveGrid_OnSlotSelected, .hovered = nullptr, .left = nullptr, .clicked = LoadSaveGrid_OnSlotConfirmed, .isHovered = 0};
    grid.getElement = LoadSaveGrid_GetSlotData;
    grid.rowHeight = nullptr;
    grid.navigateSound = 3;
    grid.reserved74 = 0;
    UIGrid_ResetRuntime(grid);
    if (grid.holder) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        grid.holder->layout.paddingX = style.holderPadding;
        grid.holder->layout.paddingY = style.holderPadding;
    }
    grid.animSlot = UIAnimAllocSlot();
    grid.scrollSpeed = 0.25f;
    UIGridTemplate &cellTemplate = g_loadSaveGridTemplate;
    cellTemplate.initialized = 0;
    cellTemplate.viewSlot = static_cast<std::int32_t>(slot);
    cellTemplate.windows = g_loadSaveWindows.data();
    cellTemplate.ids = kLoadSaveWindowIds.data();
    cellTemplate.layoutItems = g_loadSaveLayoutItems.data();
    cellTemplate.labels = g_loadSaveLabels.data();
    cellTemplate.first = 2;
    cellTemplate.end = 9;
    cellTemplate.clipWindows[0] = 4;
    cellTemplate.clipWindows[1] = -1;
    cellTemplate.layoutFirst = 0;
    cellTemplate.layoutEnd = 12;
    cellTemplate.labelFirst = 0;
    cellTemplate.labelEnd = 7;
    g_frontendEmblemImages[static_cast<std::size_t>(g_frontendEmblemImageCount++)] = g_loadSaveColourImages[1];
    g_frontendEmblemImages[static_cast<std::size_t>(g_frontendEmblemImageCount++)] = g_loadSaveColourImages[4];
    return 1;
}




std::int64_t LoadSave_GetContentPanel() {
    const bool inGame = g_isInGame != 0;
    if (d3t::UIWindow *const panel = g_loadSaveWindows[1])
        std::snprintf(panel->layout.parentId.data(), panel->layout.parentId.size(), "%s", inGame ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
    if (UIButtonWidget *const panel = g_loadSaveButtons[0])
        panel->colour = inGame ? 0xff322b1cU : 0xffa2997cU;
    if (UIButtonWidget *const row = g_loadSaveButtons[1])
        row->colour = inGame ? 0xdc272415U : 0xc8565250U;
    if (inGame)
        LoadSave_SetTextureAlphas(0.0f, 1.0f);
    else
        LoadSave_SetTextureAlphas(1.0f, 0.0f);
    return 0;
}







void LoadSaveGrid_DrawSlot(
    UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float, std::int32_t element, void *, std::int32_t index) {
    nk_context &context = GNuklearWindow;
    SaveSlotInfo *const slot = index >= 10 ? nullptr : &g_saveSlotInfo[static_cast<std::size_t>(index)];
    const std::int32_t selected = UIGrid_SelectedIndex(*grid);
    context.style.window.fixed_background.data.color.a = 0;
    UI_WindowMouseCallbacks(grid->mouseCallbacks, element, *cell, slot);
    static_cast<void>(UIGrid_PlaceCellWidgets(g_loadSaveGridTemplate, index, *holder, *cell));
    UIColourImageWidget *const shadingGame = g_loadSaveColourImages[5];
    UIColourImageWidget *const shadingFront = g_loadSaveColourImages[6];
    UIColourImageWidget *const shadingSelected = g_loadSaveColourImages[7];
    if (shadingFront && shadingSelected) {
        if (g_isInGame) {
            shadingGame->alpha = 1.0f;
            shadingFront->alpha = 0.0f;
            shadingSelected->alpha = 0.0f;
        } else {
            shadingGame->alpha = 0.0f;
            shadingFront->alpha = selected == index ? 0.0f : 1.0f;
            shadingSelected->alpha = selected == index ? 1.0f : 0.0f;
        }
    }
    const auto setAlphaByte = [](std::uint32_t &colour, std::int32_t value) {
        colour = (colour & 0x00ffffffU) | static_cast<std::uint32_t>(static_cast<std::uint8_t>(value)) << 24;
    };
    if (UIColourImageWidget *const icon = g_loadSaveColourImages[8])
        setAlphaByte(icon->colour, selected == index ? 0xff : 0x99);
    const bool saved = slot->valid == 1;
    const auto show = [&setAlphaByte, selected, index](d3t::UILabelWidget *label, float alpha) {
        if (selected != index)
            alpha = alpha * 0.6f;
        setAlphaByte(label->colour, static_cast<std::int32_t>(alpha * 255.0f));
        setAlphaByte(label->strokeColour, alpha > 0.0f ? 0xff : 0);
    };
    const auto text = [](std::size_t label, const char *value) {
        std::snprintf(g_loadSaveLabelTexts[label].data(), g_loadSaveLabelTexts[label].size(), "%s", value);
    };
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[1])
        show(label, 1.0f);
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[2])
        show(label, 1.0f);
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[0]) {
        std::snprintf(g_loadSaveLabelTexts[0].data(), g_loadSaveLabelTexts[0].size(), "%i", index + 1);
        show(label, 1.0f);
    }
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[3]) {
        label->alignment = saved ? 9U : 17U;
        text(3, saved ? slot->placeName.data() : InitAndRetrieveStringFromID("UIEMPTYSLOT")); // @text EN: "Empty Slot" | JA: "空きスロット"
        show(label, 1.0f);
    }
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[4]) {
        if (saved)
            text(4, slot->gameTime.data());
        show(label, saved ? 1.0f : 0.0f);
    }
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[5])
        show(label, saved ? 1.0f : 0.0f);
    if (d3t::UILabelWidget *const label = g_loadSaveLabels[6]) {
        if (saved)
            text(6, slot->saveTime.data());
        show(label, saved ? 1.0f : 0.0f);
    }
    if (g_saveOrLoad == 1 && g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_SAVE_GRID && (kSaveGridSaveBlocked || g_saveScreenFlag != 1)) {
        for (d3t::UILabelWidget *const label : g_loadSaveLabels) {
            setAlphaByte(label->colour, static_cast<std::int32_t>(static_cast<float>(label->colour >> 24) * 0.1f));
            setAlphaByte(label->strokeColour, static_cast<std::int32_t>(static_cast<float>(label->strokeColour >> 24) * 0.1f));
        }
    }
    UIGrid_AfterDraw(g_loadSaveGridTemplate, grid, index, 2, -1, *holder, panel);
}





void LoadSave_Render() {
    LoadSave_ParseDefinition();
    if (g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_SAVE_GRID) {
        g_saveDisplayModuleState = SAVE_DISPLAY_MODULE_SAVE_GRID;
        if (g_timeValue)
            g_shouldTimeAdvance = 0;
    }
    CreateSaveDataInfoForMenu(g_loadType);
    UIGrid &grid = g_loadSaveGrid;
    grid.elements = g_saveSlotInfo.data();
    grid.elementCount = 10;
    const std::int32_t lastRow = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (grid.columns * (lastRow + 1) - 1 < 10)
        grid.rows = 10 / grid.columns + (10 % grid.columns > 0 ? 1 : 0);
    UI_ActivateElement(g_uiElements[kUiGameMenuInformationBar]);
    InformationBarMessage message{};
    message.noOutline = 1;
    message.text = "UIMAINHLCN"; // @text EN: "Load and start a saved Game Data." | JA: "記録したゲームデータを読み込みます。"
    d3t::CopyStringToUILabel(&message);
    UIGrid_ResetRuntime(grid);
    g_uiGridInputEnabled = 1;
    g_uiFocusedList = nullptr;
    UI_RestartCurrentGridSelection();
    LoadSaveGrid_UpdateSelectedSlotPreview();
    g_loadSaveConfirmLocked = false;
}




std::int64_t LoadSave_IsPressed() {
    if (!g_hasSaved) {
        if ((g_inputState.slots[0].pressedPriority & 0x200U) != 0 && !g_uiElements[kUiGameMenu].isVisible && g_saveOrLoad == 1)
            g_hasConfirmedButton = 1;
        const std::int32_t selectedRow = g_loadSaveGrid.selectedRow;
        const std::int64_t pressed = UIGrid_IsPressed(g_loadSaveGrid);
        if (selectedRow != g_loadSaveGrid.selectedRow)
            LoadSaveGrid_UpdateSelectedSlotPreview();
        return pressed;
    }
    if (g_saveWriteStatus == 1)
        return 0;
    if (g_saveWriteStatus == 2)
        CreateSaveDataInfoForMenu(g_loadType);
    g_hasSaved = 0;
    if (g_saveWriteStatus == 2)
        LoadSaveShowMessage("UISAVEOK", 4.0f); // @text EN: "Saved successfully" | JA: "セーブが完了しました"
    else if (g_saveWriteStatus == 3 || g_saveWriteStatus == 4)
        LoadSaveShowMessage("UISAVEERRDS", 4.0f); // @text EN: "Save failed. No storage space available" | JA: "空きスペースがないため、セーブできません。"
    return 0;
}


std::int64_t LoadSave_Tick() {
    const UIBaseRuntime &element = g_uiElements[kUiLoadSave];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    d3t::DrawGridItem(&g_loadSaveGrid, element.index);
    return 0;
}



std::int64_t LoadSave_Destroy() {
    UIBaseRuntime &informationBar = g_uiElements[kUiGameMenuInformationBar];
    if (!informationBar.isVisible || !informationBar.destroy || informationBar.destroy() == 1 || g_busy_reset == 1)
        informationBar.isVisible = 0;
    SAVE_SetDisplayModuleStateAndTimeFlags(SAVE_DISPLAY_MODULE_NONE);
    UIBaseRuntime &yesNo = g_uiElements[kUiYesNoPrompt];
    if (!yesNo.isVisible || !yesNo.destroy || yesNo.destroy() == 1 || g_busy_reset == 1)
        yesNo.isVisible = 0;
    g_loadSaveConfirmLocked = false;
    g_forceXInputAnalogRefresh = 0;
    return 1;
}

// @verified - p
void LoadScreen_ReleaseHeader() {
    LoadScreenPresentation &screen = g_loadScreenPresentation;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kLoadScreenElement].config.uiHandleIndex);
    if (slot > 75U || static_cast<std::uint32_t>(screen.headerImage) >= g_uiImageWidgets.size())
        return;
    d3t::UIImageWidget &widget = g_uiImageWidgets[static_cast<std::size_t>(screen.headerImage)];
    d3t::UIViewImage &image = g_uiViewSlots[slot].images[static_cast<std::size_t>(widget.image)];
    if (image.ready != 1)
        return;
    if (d3t::IsTextureHandleValid(screen.headerTexture)) {
        InitTexture2DResource(screen.headerTexture);
        screen.headerTexture = 0;
    }
    image.textureHandle = 0;
    image.ready = 0;
    widget.field44 = 0;
}

// @verified - p
void LoadScreen_BindHeader() {
    LoadScreenPresentation &screen = g_loadScreenPresentation;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kLoadScreenElement].config.uiHandleIndex);
    if (slot > 75U || static_cast<std::uint32_t>(screen.headerImage) >= g_uiImageWidgets.size())
        return;
    d3t::UIImageWidget &widget = g_uiImageWidgets[static_cast<std::size_t>(screen.headerImage)];
    d3t::UIViewImage &image = g_uiViewSlots[slot].images[static_cast<std::size_t>(widget.image)];
    std::uint32_t width, height;
    if (image.ready != 0 || !screen.headerTexture || !GetTexture2DDimensions(screen.headerTexture, &width, &height))
        return;
    image.widthScale = static_cast<float>(width) / 1920.0f;
    image.heightScale = static_cast<float>(height) / 1080.0f;
    image.textureHandle = static_cast<std::int32_t>(screen.headerTexture);
    image.width = static_cast<std::uint16_t>(width);
    image.height = static_cast<std::uint16_t>(height);
    image.region = {0, 0, image.width, image.height};
    image.ready = 1;
    widget.field44 = 1;
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {


bool CDTT_IsLoadingUIActive() {
    for (const std::int32_t binding : g_loadScreenPresentation.widgetBindings) {
        if (binding >= 0 && binding < static_cast<std::int32_t>(g_uiWidgetBindings.size()) && g_uiWidgetBindings[static_cast<std::size_t>(binding)].animating)
            return true;
    }
    return false;
}




void LoadLoadingScreenImage(std::int32_t name, std::int32_t loadIndex, std::int32_t specialDateIndex, std::int32_t year, std::int32_t month, std::int32_t day,
    std::int32_t dayOfWeek, std::int32_t hours, std::int32_t minutes, bool pm, const char *filename, UICdttMode mode) {
    LoadScreenPresentation &screen = g_loadScreenPresentation;
    if (screen.imageSet)
        return;
    screen.time[0] = '\0';
    screen.date[0] = '\0';
    screen.area[0] = '\0';
    screen.subArea[0] = '\0';
    if (mode == UICdttMode::SkipLoadScreenRender || mode == UICdttMode::SkipLoadScreenRenderAlt)
        return;
    if (mode == UICdttMode::HideLoadingText) {
        UIWidget_Animate(screen.widgetBindings[kLoadScreenLoadingText], 1, 0.3f, 0.0f);
        return;
    }
    if (name < 0) {
        screen.area[0] = '\0';
        screen.subArea[0] = '\0';
        if (static_cast<std::uint32_t>(screen.headerImage) < g_uiImageWidgets.size())
            g_uiImageWidgets[static_cast<std::size_t>(screen.headerImage)].field44 = 0;
        return;
    }
    const std::int32_t hour = hours > 12 ? hours - 12 : hours;
    const char *const period =
        InitAndRetrieveStringFromID(pm ? "UILOADPM00" : "UILOADAM00"); // @text UILOADPM00 EN: "pm" | JA: "午後" ; UILOADAM00 EN: "am" | JA: "午前"
    if (SaveTextLanguage() == 1)
        std::snprintf(screen.time.data(), screen.time.size(), "%s %2u:%02u", period, static_cast<unsigned>(hour), static_cast<unsigned>(minutes));
    else
        std::snprintf(screen.time.data(), screen.time.size(), "%2u:%02u %s", static_cast<unsigned>(hour), static_cast<unsigned>(minutes), period);
    GenerateLoadingScreenText(screen.date.data(), screen.date.size(), year, month, static_cast<std::uint32_t>(day), dayOfWeek, 3);
    LoadScreen_ReleaseHeader();
    const char *areaName = nullptr;
    if (filename || loadIndex == 22) {
        if (specialDateIndex)
            areaName = InitAndRetrieveStringFromID(kLoadingUITokens[static_cast<std::size_t>(specialDateIndex - 1)]);
        else if (static_cast<std::uint32_t>(loadIndex) <= 84U)
            areaName = SaveTextLanguage() == 1 ? kMapLocationTokens[static_cast<std::size_t>(loadIndex)].areaName :
                                                 InitAndRetrieveStringFromID(kMapLocationTokens[static_cast<std::size_t>(loadIndex)].uiToken);
    }
    bool subArea = false;
    if (areaName) {
        if (const char *const mark = std::strstr(areaName, kLoadScreenSubAreaMark)) {
            subArea = true;
            std::snprintf(screen.area.data(), screen.area.size(), "%.*s", static_cast<int>(mark - areaName), areaName);
            std::snprintf(screen.subArea.data(), screen.subArea.size(), "%s", mark + 2);
        } else {
            std::snprintf(screen.area.data(), screen.area.size(), "%s", areaName);
        }
        if (filename && !specialDateIndex) {
            std::array<char, 260> path{};
            std::snprintf(path.data(), path.size(), "%s/%.*s%s?usage=0", "/ui/loadscreen/textures/headers", static_cast<int>(std::strlen(filename) - 3),
                filename, ".png");
            d3t::LoadTextureFromFilepath(path.data(), 0, &screen.headerTexture, g_activeRenderHandle);
        }
    } else {
        screen.area[0] = '\0';
        if (static_cast<std::uint32_t>(screen.headerImage) < g_uiImageWidgets.size())
            g_uiImageWidgets[static_cast<std::size_t>(screen.headerImage)].field44 = 0;
    }
    screen.loadingTextHeld = mode == UICdttMode::SpecialDateLoadScreen && screen.windows[kLoadScreenLoadingText] != nullptr;
    constexpr std::array<std::size_t, 7> kShownWindows{0, 1, 2, 3, 5, 4, 6};
    for (const std::size_t window : kShownWindows)
        UIWidget_Animate(screen.widgetBindings[window], 1, 0.3f, 0.0f);
    if (subArea)
        UIWidget_Animate(screen.widgetBindings[kLoadScreenLocationSubName], 1, 0.3f, 0.0f);
    if (!screen.loadingTextHeld)
        UIWidget_Animate(screen.widgetBindings[kLoadScreenLoadingText], 1, 0.3f, 0.0f);
    screen.imageSet = 1;
}
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



void LoadScreen_ResetUIStates() {
    LoadScreenPresentation &screen = g_loadScreenPresentation;
    screen.imageSet = 0;
    d3t::ResetElementByIdx(kUiHelpNew);
    d3t::ResetElementByIdx(4);
    d3t::ResetElementByIdx(kUiGameHud);
    d3t::ResetElementByIdx(kUiGameMenu);
    d3t::ResetElementByIdx(kUiWristwatchOverlay);
    if (!screen.imageSet) {
        screen.time[0] = '\0';
        screen.date[0] = '\0';
        screen.area[0] = '\0';
        screen.subArea[0] = '\0';

        if (static_cast<std::uint32_t>(screen.headerImage) < g_uiImageWidgets.size())
            g_uiImageWidgets[static_cast<std::size_t>(screen.headerImage)].field44 = 0;
    }
    for (d3t::UIWindow *const window : screen.windows) {
        if (window)
            window->alpha = 0.0f;
    }
    if (screen.windows[kLoadScreenBlackBackground])
        screen.windows[kLoadScreenBlackBackground]->alpha = 1.0f;
}

std::int64_t LoadScreen_Destroy() {
    constexpr std::array<std::size_t, 9> kHiddenWindows{0, 1, 2, 3, 6, 7, 8, 5, 4};
    for (const std::size_t window : kHiddenWindows)
        UIWidget_Animate(g_loadScreenPresentation.widgetBindings[window], 0, 2.0f, 0.0f);
    UI_ActivateElement(g_uiElements[kUiGameHud]);
    return 1;
}

// @verified - p
std ::int64_t LoadScreen_IsSelected() {
    LoadScreenPresentation &screen = g_loadScreenPresentation;
    const std ::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kLoadScreenElement].config.uiHandleIndex);
    if (slot > 75U)
        return 1;
    d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &view = g_uiViewSlots[slot];
    for (std::size_t window = 0; window < screen.windows.size(); ++window)
        screen.windows[window] = UI_GetWindow(UI_FindViewWindow(slot, kLoadScreenWindowIds[window]));
    screen.headerImage = UI_FindViewImageWidget(slot, "Image_Header");
    for (std::int32_t index = 0; index < view.widgetCount; ++index) {
        if (view.widgetKinds[static_cast<std::size_t>(index)] != 15)
            continue;
        d3t::UILabelWidget *const label = UI_LabelRecord(view.widgetPoolIndices[static_cast<

            std::size_t>(index)]);
        if (!label)
            continue;
        if (std::strcmp(label->text, "TIME") == 0)
            label->text = screen.time.data();
        else if (std::strcmp(label->text, "DATE") == 0)
            label->text = screen.date.data();
        else if (std::strcmp(label->text, "AREA") == 0)
            label->text = screen.area.data();
        else if (

            std::strcmp(label->text, "SUBAREA") == 0)
            label->text = screen.subArea.data();
        else if (std::strcmp(label->text, "LOADING") == 0)
            label->text = screen.nowLoading.data();
    }
    screen.widgetBindings[0] = -1;
    for (std::size_t window = 0; window < screen.widgetBindings.size(); ++window) {
        const std::int32_t handle = UI_FindViewWindow(slot, kLoadScreenWindowIds[window]);
        d3t::UIWindow *const record = UI_GetWindow(handle);
        screen.widgetBindings[window] = UIWidget_Bind(handle, record ? &record->alpha : nullptr, {0.0f, 0.0f}, {1.0f, 1.0f});
    }
    return 1;
}

std::int64_t LoadScreen_Reset() {
    g_loadScreenPresentation.windows.fill(nullptr);
    return 0;
}

// @verified - p
std::int64_t LoadScreen_UpdateNowLoading() {

    const std::uint32_t slot = static_cast<std ::uint32_t>(g_uiElements[kLoadScreenElement].config.uiHandleIndex);
    if (slot > 75U)
        return 1;
    LoadScreen_BindHeader();
    const std::int32_t dots = static_cast<std::int32_t>(d3t ::GetSecondsByPerformanceQuery() * 2.0f) % 4;
    std::snprintf(g_loadScreenPresentation.nowLoading.data(), g_loadScreenPresentation.nowLoading.size(), "%s%s", InitAndRetrieveStringFromID("UILOADNOWL"),
        kLoadingDots[static_cast<std::size_t>(dots)]); // @text EN: "Now Loading" | JA: "ＮＯＷ　ＬＯＡＤＩＮＧ"
    if (g_debugLoadingWidgetEnabled != 0)
        UI_InputHandler(
            GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount); // @text EN: "Now Loading" | JA: "ＮＯＷ　ＬＯＡＤＩＮＧ"
    return 1;
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

std::uint8_t UI_ICSVNativeReadinessComposite() noexcept {
    const SleepSaveTrainingMenu &menu = g_sleepSaveTrainingMenu;
    return static_cast<std::uint8_t>((menu.requested & 7U) | (~(2U * static_cast<std::uint8_t>(menu.flags)) & 6U));
}

bool UI_ICSVPeekScriptResult(std::uint32_t &result) {
    const SleepSaveTrainingMenu &menu = g_sleepSaveTrainingMenu;
    if (menu.scriptResult == 0)
        return false;
    result = menu.scriptResult;
    return true;
}

void UI_ICSVCallback(HLib::TaskData *) {
    SleepSaveTrainingMenu &menu = g_sleepSaveTrainingMenu;
    menu = {};
    menu.task = CurrentTask;
    menu.input = AllocateClearedPADCEntry();
    EnqueueSYGH();
    InstallOwnedTaskMain(UI_ICSVMainCallback);
}



void UI_ICSVMainCallback(HLib::TaskData *) {
    SleepSaveTrainingMenu &menu = g_sleepSaveTrainingMenu;
    const auto highlightIdle = [] {
        const std::int32_t slot = g_actionSelectorElement.highlightAnimSlot;
        return slot == -1 || g_uiAnimSlots[static_cast<std::size_t>(slot)].current[0] <= 0.0f;
    };
    const auto restoreHelp = [&menu] { UI_HELPDispatch(0, menu.returnHelpToken, 0); };
    const auto clearPrompts = [] { UI_SICNDispatch(1, 0, UISicnPausePolicy::KeepOnPause); };
    switch (menu.phase) {
    case 0:
        if (menu.requested) {
            std::array<std::int32_t, 4> tokens{-1, 71, 72, -1};
            if ((menu.flags & 2) == 0) {
                tokens[0] = 70;
                tokens[3] = 73;
            }
            ActionSelectorSetChoiceLabelsFromTokenIds(tokens);
            g_actionSelectorPresentation.helpDetailsIndex = 10;
            ActivateOwnedElement(kUiActionSelector, "/ui/actionselector/ActionSelector.ui");
            LoadFont("WAZA.FON");
            menu.returnHelpToken = 45;
            UI_HELPDispatch(0, 10, 0);
            clearPrompts();
            UI_SICNEnqueue(3, UISicnPausePolicy::KeepOnPause);
            if ((menu.flags & 1) == 0)
                UI_SICNClearState(2);
            CapturePad(menu.input, menu.previousInput);
            menu.phase = 1;
            menu.selection = 4;
            menu.active = 1;
        }
        break;
    case 1:
        menu.phase = 2;
        break;
    case 2: {
        const std::int32_t code = g_actionSelectorInputCode;
        g_actionSelectorInputCode = 13;
        if ((menu.flags & 2) != 0 && (code & 0xfb) == 0)
            break;
        if (((menu.flags & 2) == 0 && code == 0) || (((code - 2) & 0xf9) == 0 && code != 8)) {
            menu.phase = 3;
            menu.highlightChoice = kActionSelectorChoiceByInputCode[static_cast<std::size_t>(code)];
            menu.selection = menu.highlightChoice;
            d3t::ResetElementByIdx(kUiActionSelector);
        } else if (code == 9 && (menu.flags & 1) != 0) {
            d3t::ResetElementByIdx(kUiActionSelector);
            menu.phase = 3;
        }
        break;
    }
    case 3:
        if (highlightIdle() && menu.highlightChoice == 4) {
            if (menu.selection == 4) {
                restoreHelp();
                clearPrompts();
                menu.phase = 5;
            } else {
                menu.phase = 4;
            }
        }
        break;
    case 4:
        menu.phase = 0;
        RestorePad(menu.previousInput);
        menu.active = 0;
        menu.requested = 0;
        switch (menu.selection) {
        case 0:
            LoadFont("DC_KANJI.FON");
            restoreHelp();
            menu.scriptResult = 2;
            clearPrompts();
            ActionSelectorSetSelectedChoice(0);
            g_defaultHelpTokenId = 0;
            g_currentHelpTokenId = 0;
            break;
        case 1:
            LoadFont("DC_KANJI.FON");
            if (g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_NONE) {
                SAVE_OpenDisplayModule(SAVE_DISPLAY_MODULE_SLEEP_SAVE);
                menu.scriptResult = 0;
            }
            UI_HELPDispatch(0, 11, 0);
            clearPrompts();
            ActionSelectorSetSelectedChoice(1);
            break;
        case 2:
            d3t::ActivateElementByIdx(kUiGameTraining);
            menu.scriptResult = 0;
            clearPrompts();
            ActionSelectorSetSelectedChoice(3);
            break;
        case 3:
            LoadFont("DC_KANJI.FON");
            restoreHelp();
            g_easyOptionState.state = 1;
            g_easyOptionState.embeddedOwner = 1;
            menu.scriptResult = 0;
            clearPrompts();
            UI_SICNEnqueue(39, UISicnPausePolicy::KeepOnPause);
            ActionSelectorSetSelectedChoice(2);
            break;
        default:
            return;
        }
        break;
    case 5:
        if (highlightIdle()) {
            menu.phase = 0;
            RestorePad(menu.previousInput);
            menu.active = 0;
            menu.requested = 0;
            menu.scriptResult = 1;
            LoadFont("DC_KANJI.FON");
        }
        break;
    default:
        break;
    }
    if (menu.active) {
        if (menu.highlightChoice != 4) {
            float &pulse = menu.choicePulse[menu.highlightChoice];
            pulse += 0.25f;
            if (pulse >= 2.0f) {
                pulse = 1.0f;
                menu.highlightChoice = 4;
            }
        }
        // shenRefreshActionSelectorIsActive.
        if (g_updateRenderFrameMode)
            g_uiElements[kUiActionSelector].isActive = 1;
    }
}

} // namespace shenmue::s1
