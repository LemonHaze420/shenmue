#include "s2/ui.hpp"
#include "s2/easy_option.hpp"
#include "s2/opening_staff_roll.hpp"
#include "s2/magic_room.hpp"
#include "s2/ctrl_speak_icon.hpp"
#include "s2/now_qte.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/ui_scene.hpp"
#include "s2/ui_grid.hpp"
#include "s2/ui_collection.hpp"
#include "s2/ui_moves.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/ui_prompt.hpp"
#include "s2/input_bindings.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/area_0001.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/omak_collection_ui.hpp"

#include "d3t/content.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/object_store.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "platform/performance_counter.hpp"
#include "s2/input.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/item_wf.hpp"
#include "s2/item_selector.hpp"
#include "s2/main_icon.hpp"
#include "s2/flag.hpp"
#include "s2/item_selector_actions.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/math.hpp"
#include "s2/screen_film.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/conversation.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/scene_data.hpp"
#include "s2/font.hpp"
#include "s2/archive_data.hpp"
#include "s2/pak.hpp"
#include "s2/directory_cache.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/inventory.hpp"
#include "s2/date_time.hpp"
#include "s2/gauge.hpp"
#include "s2/npc_package.hpp"
#include "s2/sound_program.hpp"
#include "s2/navigation.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/author.hpp"
#include "s2/help.hpp"
#include "s2/pause.hpp"
#include "s2/memo.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/display_configuration.hpp"
#include "s2/audio_data.hpp"
#include "s2/hand.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/pawn.hpp"
#include "s2/game_runtime.hpp"
#include <json/json.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <charconv>
#include <cctype>
#include <cmath>
#include <limits>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <stdexcept>
#include <vector>
#include "s2/area_names.hpp"

#include "s2/ui_internal.hpp"

namespace shenmue::s2 {
using namespace ui_detail;

namespace ui_detail {

std::uint64_t CurrentNanoseconds() {
    const std::int64_t frequency = platform::performanceCounterFrequency();
    const std::int64_t counter = platform::performanceCounter();
    return static_cast<std::uint64_t>(1000000000LL * (counter / frequency) + 1000000000LL * (counter % frequency) / frequency);
}

// @verified - p
float SplashElapsed() {
    const std::int64_t elapsed = std::bit_cast<std::int64_t>(CurrentNanoseconds() - splashStateStart);
    return (static_cast<float>(elapsed) / 1000000.0f) / 1000.0f;
}

void SetSplashMode(std::int32_t mode) {
    splashScreenMode = mode;
    splashStateStart = CurrentNanoseconds();
}

// @verified - p
std::uint32_t LoadSplashImage(SplashRuntime &runtime) {
    const std::int32_t language = GetLoadingScreenTextType(ResetSceneTextLanguage());
    const char *path = runtime.definition->texturePath;
    if (!path)
        return static_cast<std::uint32_t>(language);
    std::array<char, 260> localized{};
    if (language != 0) {
        const char *const extension = d3t::GetFileExtension(path);
        if (extension) {
            std::snprintf(localized.data(), localized.size(), "%.*s_%s.%s", static_cast<int>(extension - path - 1), path,
                SPLASH_LANGUAGE_SUFFIXES[static_cast<std::size_t>(language)], extension);
            if (d3t::AuthoredAssetExists(localized.data()))
                path = localized.data();
        }
    }
    runtime.request = SetupUsageForTexture(path, 0);
    return runtime.request;
}

// @verified - p
bool ValidateSplashImage(SplashRuntime &runtime) {
    if (runtime.textureHandle)
        return true;
    runtime.textureHandle = GetShaderDataByIndex(runtime.request);
    if (!runtime.textureHandle)
        return false;
    runtime.image = {};
    UI_SetImageFromTexture(runtime.image, runtime.textureHandle);
    return true;
}
// @verified - p
void InitializeSplashScreens(std::uint32_t) {

    splashScreenIndex = GetS2IsEnglishVersion() ? 0 : 2;
    for (std::size_t index = static_cast<std::size_t>(splashScreenIndex); index < splashRuntime.size(); ++index) {
        SplashRuntime &runtime = splashRuntime[index];
        runtime.definition = &splashImages[index];
        LoadSplashImage(runtime);
        if (runtime.definition->audioPath)
            runtime.audioData = static_cast<std::byte *>(d3t::LoadLegacyFile(runtime.definition->audioPath, &runtime.audioSize));
    }
    SetSplashMode(0);
}

// @verified - p
void DrawSplashImage(const SplashRuntime &runtime, float opacity) {
    if (!splashWindow)
        return;
    nk_context &context = *GetNativeUiContext();
    UILayoutRect bounds{};
    UI_GetWindowRect(splashWindow, bounds);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &splashPanel, "UISplash", "UISplash", nk_rect(bounds.x, bounds.y, bounds.width, bounds.height), NK_WINDOW_NO_SCROLLBAR);
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    }
    if (runtime.textureHandle) {
        const float width = static_cast<float>(runtime.image.region[2]);
        const float height = static_cast<float>(runtime.image.region[3]);
        const float x = ((bounds.width - width) * 0.5f) + bounds.x;
        const float y = ((bounds.height - height) * 0.5f) + bounds.y;
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, nk_rect(x, y, width, height));
        const std::uint32_t colour = ConstructColour(255, 255, 255, static_cast<std::int32_t>(opacity * 255.0f));
        nk_color tint;
        std::memcpy(&tint, &colour, sizeof(tint));
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, runtime.image, tint);
    }
    if (g_waitingForNextTick == 1) {
        nk_layout_space_end(&context);
        nk_end(&context);
    }
}

std::int64_t SplashRender() {
    ProcessSplashScreen();
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::Splash)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    return 0;
}

// @verified - p
std::int64_t HandleSplashInput() {
    if (g_inputBuffer[0].pressedPriority & PAD_BUTTON_X)
        splashSkipPressed = true;
    const nk_context &context = *GetNativeUiContext();
    if (context.input.mouse.buttons[NK_BUTTON_LEFT].clicked != 0)
        splashSkipPressed = true;
    if (context.input.mouse.buttons[NK_BUTTON_MIDDLE].clicked != 0)
        splashSkipPressed = true;
    if (context.input.mouse.buttons[NK_BUTTON_RIGHT].clicked != 0)
        splashSkipPressed = true;
    return 0;
}

void ProcessSplashScreen() {
    if (splashScreenIndex < 0 || splashScreenIndex >= 6)
        return;
    SplashRuntime &runtime = splashRuntime[static_cast<std::size_t>(splashScreenIndex)];
    const SplashImageDefinition &definition = *runtime.definition;
    const float elapsed = SplashElapsed();
    float opacity = 0.0f;

    switch (splashScreenMode) {
    case 0:
        if (elapsed > 0.5f && ValidateSplashImage(runtime)) {
            SetSplashMode(1);
            if (runtime.audioData)
                PlayWaveBuffer(runtime.audioData, static_cast<std::int32_t>(runtime.audioSize), GetS2AudioSettings().sfxVolume);
        }

        break;
    case 1:
        opacity = definition.duration > 0.0f ? elapsed / definition.duration : 1.0f;
        if (elapsed > definition.duration) {
            opacity = 1.0f;
            SetSplashMode(2);
        }
        break;
    case 2:
        opacity = 1.0f;
        if (forceSkipSplashScreens || elapsed > definition.maximumDuration || (splashSkipPressed && elapsed > definition.minimumDuration)) {
            SetSplashMode(3);
        }
        break;
    case 3:
        opacity = definition.fadeOutDuration > 0.0f ? 1.0f - elapsed / definition.fadeOutDuration : 0.0f;
        if (elapsed > definition.fadeOutDuration) {
            splashSkipPressed = false;
            ++splashScreenIndex;
            if (splashScreenIndex < 6) {
                SetSplashMode(0);
            } else {
                DestroyElementByIndex(UIElement::Splash);
                ShowElementByIndex(UIElement::PressStart);
                return;
            }
        }
        break;
    default:
        break;
    }

    DrawSplashImage(runtime, std::clamp(opacity, 0.0f, 1.0f));
}

// @verified - p
std::int64_t SplashSelected() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::Splash)].config.uiHandleIndex;
    splashWindow = UI_GetWindow(UI_FindViewWindow(slot, SPLASH_WINDOW_ID));
    return 1;
}

// @verified - p
std::int64_t ResetSplash() {
    splashWindow = nullptr;
    return 0;
}

// @verified - p
std::int64_t DestroySplash() {
    SetupAllShaders();
    return 1;
}

std::uint32_t LoadSplash() {
    return ReadUIDefinitionFile(UIElement::Splash, "/ui/Splash/Splash.ui", 0);
}

std::uint32_t LoadPressStart() {
    return ReadUIDefinitionFile(UIElement::PressStart, "/ui/mainmenu_New/MainMenu_PressStart.ui", 1);
}

std::uint32_t LoadMainMenu() {
    return ReadUIDefinitionFile(UIElement::MainMenu, "/ui/mainmenu_New/MainMenu.ui", 1);
}

std::uint32_t LoadMainMenuWheel() {
    return ReadUIDefinitionFile(UIElement::MainMenuWheel, "/ui/mainmenu_New/MainMenu_Wheel.ui", 1);
}

std::uint32_t LoadMainMenuNew() {
    return ReadUIDefinitionFile(UIElement::MainMenuNew, "/ui/mainmenu_New/MainMenu_New.ui", 1);
}

std::uint32_t LoadMainMenuContinue() {
    return ReadUIDefinitionFile(UIElement::MainMenuContinue, "/ui/mainmenu_New/MainMenu_Continue.ui", 1);
}

std::uint32_t LoadMainMenuCollection() {
    return ReadUIDefinitionFile(UIElement::ShenmueCollection, "/ui/mainmenu_New/SMCollection.ui", 1);
}

void ApplyPressStartDefaultOpacity() {
    PressStartRuntime &runtime = pressStartRuntime;
    for (const std::size_t index : {20U, 21U, 3U, 13U})
        ShowUIWindow(runtime.windows[index]);
    for (const std::size_t index : {5U, 7U, 8U})
        HideUIWindow(runtime.windows[index]);
    for (std::size_t index = 14; index < 20; ++index)
        ShowUIWindow(runtime.windows[index]);
    if (runtime.windows[6])
        runtime.windows[6]->colour &= 0x00ffffffU;
}

std::int64_t TickPressStart() {
    shenSetForcedFilmbars(1);
    ResetPressStartLogoImages();
    return 0;
}

std::int64_t PressStartSelected() {
    PressStartRuntime &runtime = pressStartRuntime;
    const std ::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::PressStart)].config.uiHandleIndex;
    for (std::size_t index = 0; index < runtime.images.size(); ++index)
        runtime.images[index] = UI_FindViewImage(slot, PRESS_START_LOGO_IMAGES[index]);
    if (HasUnlockedRenderHandle())
        SetD3TRenderSettings(0);
    runtime.prompt = UI_GetLabel(static_cast<std::uint32_t>(UI_FindViewLabel(slot, "PressStart::PressStart")));
    runtime.blinkTransition = UIAnimAllocSlot();
    StartUIIntegerTransition(runtime.blinkTransition, 0, 2, 1.0f, 0.0f);
    SetUITransitionLoopMode(runtime.blinkTransition, 1);
    for (std::size_t index = 0; index < runtime.windows.size(); ++index) {
        runtime.windows[index] = UI_FindViewWindowPointer(slot, PRESS_START_PANEL_IDS[index].data());
        runtime.transitions[index] = -1;
    }
    for (std::size_t index = 0; index < runtime.murals.size(); ++index) {
        const std::int32_t image = UI_FindViewColourImage(slot, PRESS_START_MURAL_IMAGES[index]);
        runtime.murals[index] = image == -1 ? nullptr : &g_uiColourImageWidgets[static_cast<std::size_t>(image)];
    }
    runtime.angleTransition = -1;
    for (std::size_t index = 0; index < runtime.icons.size(); ++index)
        runtime.icons[index] = UI_FindViewIconWidget(slot, PRESS_START_ICON_WIDGETS[index], 21);
    for (std::size_t index = 0; index < runtime.logos.size(); ++index) {
        const std::int32_t image = UI_FindViewImageWidget(slot, PRESS_START_LOGO_WIDGETS[index]);
        runtime.logos[index] = image == -1 ? nullptr : &g_uiImageWidgets[static_cast<std::size_t>(image)];
    }
    ResetPressStartLogoImages();
    ApplyPressStartDefaultOpacity();
    if (runtime.frozen)
        FreezePressStartPresentation();
    return 1;
}

std::int64_t PressStartInput() {
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    if (pressStartRuntime.frozen || (!enterPressed && (pressed & 0x0400U) == 0))
        return 0;
    DestroyElementByIndex(UIElement::PressStart);
    ShowElementByIndex(UIElement::MainMenu);
    (void)SelectTitlePressStart();
    AicaExecuteWrapper(2);
    areas::NotifyTitleIntroPressStart();
    enterPressed = false;
    g_inputBuffer[0].pressedPriority &= 0xfbffU;
    return 0;
}

// @verified - p
std::int64_t PressStartRender() {
    PressStartRuntime &runtime = pressStartRuntime;
    const bool keyboard = GetIsKeyboardActive() != 0;
    for (std::size_t index = 0; index < runtime.icons.size(); ++index) {
        UIButtonIconWidget *const icon = runtime.icons[index];
        if (icon) {
            const std::uint32_t alpha = (keyboard == (index != 0)) ? 0xff000000U : 0U;
            icon->colour = (icon->colour & 0x00ffffffU) | alpha;
            icon->fontColour = (icon->fontColour & 0x00ffffffU) | alpha;
        }
    }
    if (runtime.blinkTransition != -1) {
        bool visible = false;
        if (!runtime.frozen)
            visible = GetUIIntegerTransitionValue(runtime.blinkTransition) % 4 != 0;
        if (runtime.prompt)
            runtime.prompt->field44 = visible;
        if (runtime.windows[22])
            runtime.windows[22]->alpha = visible ? 1.0f : 0.0f;
    }
    UpdatePressStartTransitions();
    UpdatePressStartLight(runtime.lights[0], 14);
    UpdatePressStartLight(runtime.lights[1], 17);
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::PressStart)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    return 0;
}

std::int32_t SaveMenuAreaIndex(shen_name map) {
    std::int32_t best = -1, exact = 0;
    for (std::size_t index = 0; index < UI_SAVE_AREAS.size(); ++index) {
        const shen_name pattern = UI_SAVE_AREAS[index].pattern;
        std::int32_t matches = 0, wildcards = 0;
        for (std::size_t byte = 0; byte < 4; ++byte) {
            if (pattern.characters[byte] == map.characters[byte])
                ++matches;
            else if (pattern.characters[byte] == '*')
                ++wildcards;
        }
        if (matches + wildcards >= 4 && matches > exact) {
            exact = matches;
            best = static_cast<std::int32_t>(index);
        }
    }
    if (exact < 4) {
        for (const SaveMenuArea &alias : UI_SAVE_AREA_ALIASES) {
            std::int32_t matches = 0, wildcards = 0;
            for (std::size_t byte = 0; byte < 4; ++byte) {
                if (alias.pattern.characters[byte] == map.characters[byte])
                    ++matches;
                else if (alias.pattern.characters[byte] == '*')
                    ++wildcards;
            }
            if (matches + wildcards >= 4 && matches > exact) {
                exact = matches;
                best = alias.image;
            }
        }
    }
    return best;
}

std::int32_t SaveMenuLocationIndex(std::int32_t area) {
    if (area < 0)
        return 0;
    const shen_name pattern = UI_SAVE_AREAS[static_cast<std::size_t>(area)].pattern;
    if (pattern.name == SAVE_MAP_0200.name)
        return 2;
    if (pattern.name == SAVE_MAP_WAT0.name)
        return 1;
    if (pattern.name == SAVE_MAP_KRF1.name)
        return 3;
    if (pattern.name == SAVE_MAP_CLS1.name)
        return 4;
    if (pattern.name == SAVE_MAP_CLS2.name)
        return 5;
    switch (pattern.characters[0]) {
    case 'A':
        return 0;
    case 'K':
        return 3;
    case 'Q':
        return 2;
    case 'W':
        return 1;
    default:
        return -1;
    }
}

void FormatSaveMenuTimestamp(std::span<char> output, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t week, std::uint32_t hour,
    std::uint32_t minute, std::uint8_t flags) {
    std::array<char, 256> date{}, time{};
    FormatLoadingScreenDate(date, year, month, day, week, flags);
    std::snprintf(time.data(), time.size(), "%02u:%02u", hour, minute);
    const bool dateFirst = g_sceneTextLanguage >= 4 && g_sceneTextLanguage <= 6;
    std::snprintf(output.data(), output.size(), "%s %s", dateFirst ? date.data() : time.data(), dateFirst ? time.data() : date.data());
}
} // namespace ui_detail

std::int32_t CheckAllSaveFiles(std::int32_t first, std::int32_t source) {
    for (std::size_t index = 0; index < loadSaveRuntime.slots.size(); ++index) {
        SaveMenuSlot &slot = loadSaveRuntime.slots[index];
        const std::int32_t ordinal = first + static_cast<std::int32_t>(index);
        const S2SaveDataInfo *const save = ordinal >= 0 && ordinal < 10 ? source >= 0 && source <= 1 ? &g_backupSaveDataInfos[ordinal] :
                source == 2                                                                          ? &g_backupS1SaveDataInfos[ordinal] :
                                                                                                       nullptr :
                                                                          nullptr;
        slot.valid = save && save->saveKind != 0;
        if (!slot.valid)
            continue;
        const S2SaveDataInfo &data = *save;
        slot.index = static_cast<std::int32_t>(index);
        slot.map = data.mapName;
        slot.area = source <= 1 ? SaveMenuAreaIndex(data.mapName) : -1;
        FormatSaveMenuTimestamp(slot.gameTime, data.gameYear, data.gameMonth, data.gameDay, data.gameWeek, data.gameHour, data.gameMinute, 2);
        FormatSaveMenuTimestamp(slot.saveTime, data.systemYear, data.systemMonth, data.systemDay, data.systemWeek, data.systemHour, data.systemMinute, 0);
        const std::int32_t location = source <= 1 ? SaveMenuLocationIndex(slot.area) : -1;
        const char* const locationToken = data.clearState == 6 ? "UISAVES1LC" : location < 0 ? "UISAVES1CL" : UI_SAVE_LOCATION_TOKENS[location]; // @text UISAVES1LC EN: "Cleared Game" | JA: "クリアデータ" ; UISAVES1CL EN: "SM1 Save" | JA: "Ｓ１セーブデータ"
        const std::string_view name = locationToken ? GetLocalizedSceneText(locationToken) : std::string_view{};
        std::snprintf(slot.locationName.data(), slot.locationName.size(), "%s%.*s", data.clearState != 6 && location >= 0 && data.saveKind == 99 ? "DBG: " : "",
            static_cast<int>(name.size()), name.data());
        const char* const areaToken = data.clearState == 6 ? "UISAVES1LC" : slot.area < 0 ? "UISAVES1NC" : UI_SAVE_AREA_TOKENS[slot.area]; // @text UISAVES1LC EN: "Cleared Game" | JA: "クリアデータ" ; UISAVES1NC EN: "Non-Cleared Game" | JA: "未クリアゲームデータ"
        const std::string_view areaName = GetLocalizedSceneText(areaToken);
        std::snprintf(slot.areaName.data(), slot.areaName.size(), "%.*s", static_cast<int>(areaName.size()), areaName.data());
    }
    return 10;
}
namespace ui_detail {

const std::int32_t *SaveMenuSelectionData(UIGrid *, std::int32_t row) {
    return reinterpret_cast<const std::int32_t *>(&loadSaveRuntime.slots[static_cast<std::size_t>(row)]);
}

void FillSaveMenuPreview() {
    LoadSaveRuntime &runtime = loadSaveRuntime;
    const std::int32_t row = GetUIGridSelection(runtime.grid);
    const SaveMenuSlot *const slot = row >= 0 && row < runtime.grid.dictionaryCount ? &runtime.slots[static_cast<std::size_t>(row)] : nullptr;
    const std::uint32_t viewSlot = g_uiElements[static_cast<std::size_t>(UIElement::LoadSave)].config.uiHandleIndex;
    d3t::UIViewImage *areaImage = runtime.images[7];
    std::int32_t mapImage = areaImage ? UI_FindViewImage(viewSlot, areaImage->name) : -1;
    if (slot && slot->valid) {
        mapImage = slot->area >= 0 && slot->area < 22 ? runtime.mapImages[slot->area] :
            runtime.images[5]                         ? UI_FindViewImage(viewSlot, runtime.images[5]->name) :
                                                        -1;
        const std::int32_t location = SaveMenuLocationIndex(SaveMenuAreaIndex(slot->map));
        areaImage = location >= 0 && location < 6 ? runtime.areaImages[location] : runtime.images[4];
    }
    if (runtime.mapImage)
        runtime.mapImage->image = mapImage;
    if (runtime.areaImage)
        runtime.areaImage->image = areaImage ? UI_FindViewImage(viewSlot, areaImage->name) : -1;
}

void SelectSaveMenuRow(float *bounds, void *) {
    UIMessageBox message = GetCurrentUIMessageBox();
    message.widgetIndex = -1;
    if (bounds)
        std::memcpy(&message.bounds, bounds, sizeof(message.bounds));
    message.hideTarget = IsCameraIconHidden() == 0;
    const std::int32_t row = GetUIGridSelection(loadSaveRuntime.grid);
    if (row >= 0 && row < loadSaveRuntime.grid.dictionaryCount)
        message.primaryText = IsCameraIconHidden() ? loadSaveRuntime.slots[row].valid ? "UIMAINHLSO" : "UIMAINHLSC" : "UIMAINHLCN"; // @text UIMAINHLSO EN: "Overwrite the save in this slot." | JA: "このスロットのゲームデータを上書きします。" ; UIMAINHLSC EN: "Create a new game save in this slot." | JA: "このスロットに新しくゲームデータをセーブします。" ; UIMAINHLCN EN: "Load and start a saved game." | JA: "記録したゲームデータを読み込みます。"
    message.primaryLocalized = 0;
    CopyUIMessageBox(&message);
    FillSaveMenuPreview();
}

void SaveMenuResponse(std::int32_t response) {
    if (response != 2)
        return;
    for (std::size_t index = 0; index < loadSaveRuntime.slots.size(); ++index) {
        if (loadSaveRuntime.selected != &loadSaveRuntime.slots[index])
            continue;
        SaveGame(static_cast<std::uint32_t>(index));
        loadSaveRuntime.writing = true;
        AicaExecuteWrapper(9);
        return;
    }
}

void ActivateSaveMenuRow(float *, void *data) {
    const SaveMenuSlot *const slot = static_cast<const SaveMenuSlot *>(data);
    if (!slot || (!slot->valid && !g_saveFrontendMode)) {
        AicaExecuteWrapper(8);
        return;
    }
    loadSaveRuntime.selected = slot;
    if (g_saveFrontendMode == 0) {
        loadSaveSelectedSlot = static_cast<std::size_t>(slot - loadSaveRuntime.slots.data());
        S2SaveScreen_ConfirmSlot(loadSaveSelectedSlot);
        return;
    }
    if (EV_CanDoAreaJump() != 1) {
        AicaExecuteWrapper(8);
        return;
    }
    if (slot->valid) NativeMessageBox("UISAVECONF", false, 0, SaveMenuResponse); // @text EN: "Are you sure you want to save?" | JA: "セーブしてもよろしいですか？"
    else
        SaveMenuResponse(2);
}

void DrawSaveMenuRow(
    UIGrid *grid, void *panel, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *, std::int32_t ordinal) {
    LoadSaveRuntime &runtime = loadSaveRuntime;
    GetNativeUiContext()->style.window.fixed_background.data.color.a = 0;
    const UILayoutRect holder{.x = outer->x, .y = outer->y, .width = outer->z, .height = outer->w};
    const UILayoutRect cell{.x = bounds->x, .y = bounds->y, .width = bounds->z, .height = bounds->w};
    PlaceUIGridCellWidgets(runtime.cellTemplate, ordinal, holder, cell);
    const bool selected = ordinal == GetUIGridSelection(*grid), game = IsCameraIconHidden() != 0;
    const float opacity = selected ? 1.0f : 0.60000002f;
    if (runtime.colourImages[6])
        runtime.colourImages[6]->alpha = game ? 1.0f : 0.0f;
    if (runtime.colourImages[7])
        runtime.colourImages[7]->alpha = !game && !selected ? 1.0f : 0.0f;
    if (runtime.colourImages[8])
        runtime.colourImages[8]->alpha = !game && selected ? 1.0f : 0.0f;
    if (runtime.colourImages[9])
        runtime.colourImages[9]->colour = (runtime.colourImages[9]->colour & 0xffffffU) | (static_cast<std::uint32_t>(opacity * 255.0f) << 24);
    const SaveMenuSlot &slot = runtime.slots[static_cast<std::size_t>(ordinal)];
    std::snprintf(runtime.text[0].data(), 256, "%i", ordinal + 1);
    const std::string_view empty = GetLocalizedSceneText("UIEMPTYSLOT"); // @text EN: "Empty Slot" | JA: "空きスロット"
    std::snprintf(runtime.text[3].data(), 256, "%.*s", slot.valid ? static_cast<int>(std::strlen(slot.areaName.data())) : static_cast<int>(empty.size()),
        slot.valid ? slot.areaName.data() : empty.data());
    if (runtime.labels[3])
        runtime.labels[3]->alignment = slot.valid ? 9 : 17;
    if (slot.valid) {
        std::snprintf(runtime.text[4].data(), 256, "%s", slot.gameTime.data());
        std::snprintf(runtime.text[6].data(), 256, "%s", slot.saveTime.data());
    }
    for (std::size_t index = 0; index < runtime.labels.size(); ++index) {
        d3t::UILabelWidget *const label = runtime.labels[index];
        if (!label)
            continue;
        const float alpha = index < 4 || slot.valid ? opacity : 0.0f;
        std::uint32_t colourAlpha = static_cast<std::uint32_t>(alpha * 255.0f), strokeAlpha = alpha > 0.0f ? 255U : 0U;
        if (g_saveFrontendMode == 1 && EV_CanDoAreaJump() != 1) {
            colourAlpha = static_cast<std::uint32_t>(colourAlpha * 0.1f);
            strokeAlpha = static_cast<std::uint32_t>(strokeAlpha * 0.1f);
        }
        label->colour = (label->colour & 0xffffffU) | (colourAlpha << 24);
        label->strokeColour = (label->strokeColour & 0xffffffU) | (strokeAlpha << 24);
    }
    AfterDrawUIGridCell(runtime.cellTemplate, *grid, ordinal, 2, -1, holder, static_cast<nk_panel *>(panel));
}

std::int64_t LoadSaveSelected() {
    LoadSaveRuntime &runtime = loadSaveRuntime;
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::LoadSave)].config.uiHandleIndex;
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    for (std::size_t index = 0; index < runtime.windows.size(); ++index)
        runtime.windows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_SAVE_WINDOWS[index]));
    for (std::size_t index = 0; index < runtime.images.size(); ++index) {
        const std::int32_t image = UI_FindViewImage(slot, UI_SAVE_IMAGES[index]);
        runtime.images[index] = view && image >= 0 ? &view->images[image] : nullptr;
    }
    for (std::size_t index = 0; index < runtime.colourImages.size(); ++index) {
        const std::int32_t image = UI_FindViewColourImage(slot, UI_SAVE_COLOUR_IMAGES[index]);
        runtime.colourImages[index] = image >= 0 ? &g_uiColourImageWidgets[image] : nullptr;
    }
    for (std::size_t index = 0; index < runtime.labels.size(); ++index) {
        const std::int32_t label = UI_FindViewLabel(slot, UI_SAVE_LABELS[index]);
        runtime.labels[index] = label >= 0 ? &g_uiLabelWidgets[label] : nullptr;
        if (runtime.labels[index])
            runtime.labels[index]->text = runtime.text[index].data();
    }
    for (std::size_t index = 0; index < runtime.layouts.size(); ++index) {
        runtime.layouts[index] = nullptr;
        if (!view)
            continue;
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 13)
                continue;
            d3t::UILayoutItem &item = g_uiLayoutItems[view->widgetPoolIndices[widget]];
            if (!std::strcmp(item.id.data(), UI_SAVE_LAYOUTS[index])) {
                runtime.layouts[index] = &item;
                break;
            }
        }
    }
    for (std::size_t index = 0; index < runtime.buttons.size(); ++index) {
        runtime.buttons[index] = nullptr;
        if (!view)
            continue;
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 17)
                continue;
            UIButtonWidget &button = g_uiButtonWidgets[view->widgetPoolIndices[widget]];
            if (!std::strcmp(button.id.data(), UI_SAVE_BUTTONS[index])) {
                runtime.buttons[index] = &button;
                break;
            }
        }
    }
    for (std::size_t index = 0; index < runtime.mapImages.size(); ++index)
        runtime.mapImages[index] = UI_FindViewImage(slot, UI_SAVE_MAP_IMAGES[index]);
    for (std::size_t index = 0; index < runtime.areaImages.size(); ++index) {
        const std::int32_t image = UI_FindViewImage(slot, UI_SAVE_AREA_IMAGES[index]);
        runtime.areaImages[index] = view && image >= 0 ? &view->images[image] : nullptr;
    }
    const std::int32_t map = UI_FindViewImageWidget(slot, "LoadScreen::Image_Map"), area = UI_FindViewImageWidget(slot, "LoadScreen::Image_Area");
    runtime.mapImage = map >= 0 ? &g_uiImageWidgets[map] : nullptr;
    runtime.areaImage = area >= 0 ? &g_uiImageWidgets[area] : nullptr;
    InitializeUIGrid(runtime.grid,
        {.columns = 1,
            .visibleRows = 4,
            .rows = 10,
            .drawEmpty = 1,
            .panel = runtime.windows[0],
            .drawRow = DrawSaveMenuRow,
            .actions = {SelectSaveMenuRow, nullptr, nullptr, ActivateSaveMenuRow},
            .selectionData = SaveMenuSelectionData,
            .sound = 3});
    runtime.cellTemplate = {.viewSlot = static_cast<std::int32_t>(slot),
        .first = 2,
        .end = 9,
        .clipWindows = {4, -1},
        .windows = runtime.windows.data(),
        .ids = UI_SAVE_WINDOWS.data(),
        .layoutFirst = 0,
        .layoutEnd = 12,
        .layoutItems = runtime.layouts.data(),
        .labelFirst = 0,
        .labelEnd = 7,
        .labels = runtime.labels.data()};
    runtime.bound = true;
    return 1;
}

std::int64_t LoadSaveResetNullsub() {
    return 0;
}

std::int64_t SetLoadSaveTheme() {
    LoadSaveRuntime &runtime = loadSaveRuntime;
    const bool game = IsCameraIconHidden() != 0;
    if (runtime.windows[1])
        std::snprintf(runtime.windows[1]->layout.parentId.data(), 64, "%s", game ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
    for (std::size_t index = 0; index < runtime.buttons.size(); ++index)
        if (runtime.buttons[index])
            runtime.buttons[index]->colour = game ? UI_SAVE_GAME_COLOURS[index] : UI_SAVE_FRONT_COLOURS[index];
    for (std::int32_t index : UI_SAVE_FRONT_IMAGES)
        if (runtime.colourImages[index])
            runtime.colourImages[index]->alpha = game ? 0.0f : 1.0f;
    for (std::int32_t index : UI_SAVE_GAME_IMAGES)
        if (runtime.colourImages[index])
            runtime.colourImages[index]->alpha = game ? 1.0f : 0.0f;
    return 0;
}

std::int64_t LoadSaveHints() {
    return CopyHudHintIds(UI_SAVE_HINTS);
}

std::int64_t LoadSaveInput() {
    if (loadSaveRuntime.writing) {
        const std::int32_t status = GetGameSaveStatus();
        if (status == 1)
            return 0;
        loadSaveRuntime.writing = false;
        if (status >= 2 && status <= 4) {
            const std::string_view text = GetLocalizedSceneText(status == 2 ? "UISAVEOK" : "UISAVEERRDS"); // @text UISAVEOK EN: "Saved successfully" | JA: "セーブが完了しました。" ; UISAVEERRDS EN: "Save failed. No storage space available" | JA: "空きスペースがないため、セーブできません。"
            std::snprintf(loadSaveRuntime.resultText.data(), loadSaveRuntime.resultText.size(), "%.*s", static_cast<int>(text.size()), text.data());
            UIMessageBox message = GetCurrentUIMessageBox();
            message.primaryText = loadSaveRuntime.resultText.data();
            message.primaryLocalized = 1;
            message.hideTarget = 0;
            ShowTemporaryUIMessageBox(&message, 4.0f);
        }
        return 0;
    }
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const bool cancel = cancelPressed || (pressed & 0x0200U) != 0;
    if (cancel && S2SaveScreen_HasPrompt()) {
        cancelPressed = false;
        g_inputBuffer[0].pressedPriority &= 0xfdffU;
        S2SaveScreen_CancelPrompt();
        return 0;
    }
    if (S2SaveScreen_HasPrompt()) {
        if (enterPressed || (pressed & 0x0400U) != 0) {
            enterPressed = false;
            g_inputBuffer[0].pressedPriority &= 0xfbffU;
            S2SaveScreen_ConfirmSlot(loadSaveSelectedSlot);
        }
        return 0;
    }
    if (!loadSaveRuntime.bound)
        return 0;
    const std::int32_t selected = GetUIGridSelection(loadSaveRuntime.grid);
    const std::int64_t result = ProcessUIGridInput(loadSaveRuntime.grid, uiPointer, GetFrameTime());
    loadSaveSelectedSlot = static_cast<std::size_t>(GetUIGridSelection(loadSaveRuntime.grid));
    S2SaveScreen_SelectSlot(loadSaveSelectedSlot);
    if (selected != GetUIGridSelection(loadSaveRuntime.grid))
        FillSaveMenuPreview();
    return result;
}

std::int64_t LoadSaveTick() {
    using enum CameraState;
    LoadSaveRuntime &runtime = loadSaveRuntime;
    if (!runtime.bound)
        return 0;
    CheckAllSaveFiles(0, static_cast<std::int32_t>(GetSaveMenuSource()));
    SetUIGridDictionary(runtime.grid, 10, reinterpret_cast<const std::int32_t *>(runtime.slots.data()));
    UIMessageBox message{};
    message.primaryText = GetSaveMenuSource() == 1 && g_saveFrontendMode == 0 ? "UIMAINHLCOL" : "UIMAINHLCN"; // @text UIMAINHLCOL EN: "Select the game file to load." | JA: "ロードするゲームデータを選んでください。" ; UIMAINHLCN EN: "Load and start a saved game." | JA: "記録したゲームデータを読み込みます。"
    message.hideTarget = 1;
    CopyUIMessageBox(&message);
    ResetUIGrid(runtime.grid);
    SetUIGridTabSelector(nullptr, 1);
    FillSaveMenuPreview();
    ShowElementByIndex(UIElement::GameInfoBar);
    SetLoadSaveTheme();
    if (IsCameraIconHidden() == 1 && g_saveScreenAdmission != 0) {
        loadSaveSavedCameraState = camCtrlGetState();
        camCtrlSetState(Event);
    }
    return 0;
}

std::int64_t LoadSaveRender() {
    if (!loadSaveRuntime.bound)
        return 0;
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::LoadSave)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    RenderNativeUIGrid(loadSaveRuntime.grid, element.index, uiPointer);
    return 0;
}

std::int64_t LoadSaveDestroy() {
    if (IsCameraIconHidden() == 0)
        DestroyElementByIndex(UIElement::GameInfoBar);
    currentUIState = -1;
    SetSaveMenuSource(0);
    S2SaveScreen_CancelPrompt();
    CloseSaveScreenFromLoadSave();
    DestroyElementByIndex(UIElement::YesNoPrompt);
    if (g_saveScreenAdmission != 0)
        camCtrlSetState(loadSaveSavedCameraState);
    g_saveScreenAdmission = 0;
    SetEndingStaffRollFlag(0);
    return 1;
}

// @verified - p
std::int64_t MainMenuRender() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::MainMenu)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    d3t::UILabelWidget *const label = mainMenuUserIdLabel;
    mainMenuUserIdText = 0;
    label->text = &mainMenuUserIdText;
    return reinterpret_cast<std::intptr_t>(label);
}

std::int64_t MainMenuSelected() {
    return BindMainMenuDefinition();
}

std::int64_t TickMainMenu() {
    shenSetForcedFilmbars(1);
    frontendRoute = FrontendRoute::Wheel;
    SelectMainMenuPage(0);
    ShowElementByIndex(UIElement::GameHud);
    return 0;
}

void MainMenuWheelClicked(float *, void *data) {
    const std::int32_t *const selection = static_cast<const std::int32_t *>(data);
    if (selection)
        ActivateMainMenuChoice(static_cast<MainMenuItem>(*selection));
}

std::int64_t BindMainMenuWheel() {
    UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::MainMenuWheel)];
    d3t::UIWindow *const root = UI_GetWindow(UI_FindViewWindow(element.config.uiHandleIndex, "MainMenu_WheelContainer"));
    if (root) {
        std::memcpy(root->layout.parentId.data(), "MainMenu::ContentPanel", sizeof("MainMenu::ContentPanel"));
        root->rectCached = 0;
    }
    g_mainMenuWheelInputBlock = UIAnimAllocSlot();
    g_mainMenuWheelIndex = UI_FindViewButtonWheel(element.config.uiHandleIndex, "MainMenuButtonWheel");
    if (g_mainMenuWheelIndex != -1) {
        g_mainMenuWheelWidget = &g_uiButtonWheels[static_cast<std::size_t>(g_mainMenuWheelIndex)];
        g_mainMenuWheelWidget->mouseCallbacks.clicked = MainMenuWheelClicked;
    }
    for (std::size_t image = 0; image < MAIN_MENU_WHEEL_IMAGES.size(); ++image)
        g_mainMenuWheelImages[image] = UI_FindViewImage(element.config.uiHandleIndex, MAIN_MENU_WHEEL_IMAGES[image]);
    if (g_mainMenuWheelWidget)
        for (std::int32_t ordinal = 0; ordinal < g_mainMenuWheelWidget->count; ++ordinal)
            g_mainMenuWheelWidget->items[static_cast<std::size_t>(ordinal)].field0[0] = ordinal;
    return 1;
}

// @verified - p
std::int64_t MainMenuWheelRender() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::MainMenuWheel)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    UpdateMainMenuWheel();
    return 0;
}

std::int64_t TickMainMenuWheel() {
    ShowElementByIndex(UIElement::GameInfoBar);
    SetDebugHideCameraIcon(0);
    if (g_mainMenuWheelWidget) {
        UpdateMainMenuWheel();
        mainMenuSelection = static_cast<MainMenuItem>(g_mainMenuWheelWidget->selection);
        RefreshMainMenuWheelDescription();
        RefreshMainMenuWheelLanguage();
    }
    return 0;
}

// @verified - p
std::int64_t DestroyMainMenuWheel() {
    DestroyElementByIndex(UIElement::GameInfoBar);
    SetTitleMenuTransitionCallback(nullptr);
    return 1;
}

std::int64_t FrontendOptionsInput() {
    if (nativePrompt.active || options.editing >= 0 || HasUnboundUserMappings())
        return 0;
    if (cancelPressed || (g_inputBuffer[0].pressedPriority & 0x0200U) != 0) {
        if (AskKeepOrRevertChanges() || !ReturnToTitleMenuHandoff())
            return 0;
        cancelPressed = false;
        g_inputBuffer[0].pressedPriority &= ~0x0200U;
    }
    if (enterPressed)
        enterPressed = false;
    return 0;
}

void MainMenuQuitResponse(std::int32_t response) {
    if (response == 2)
        RequestFrontendQuit();
}

std::int64_t MainMenuWheelInput() {
    SetContinueAvailability(ContinueModuleIsAvailable());
    if (!g_uiElements[static_cast<std::size_t>(UIElement::MainMenuWheel)].isVisible) {
        if (g_uiElements[static_cast<std::size_t>(UIElement::MainMenu)].isVisible && (cancelPressed || (g_inputBuffer[0].pressedPriority & 0x0200U) != 0)) {
            if (SelectMainMenuPage(0))
                AicaExecuteWrapper(2);
            cancelPressed = false;
            g_inputBuffer[0].pressedPriority &= ~0x0200U;
        }
        return 0;
    }
    if ((g_inputBuffer[0].pressedPriority & 0x0002U) != 0) {
        g_inputBuffer[0].pressedPriority &= 0xfffdU;
        NativeMessageBox("UIQUITGAME", false, 0, MainMenuQuitResponse); // @text EN: "Exit to Windows?" | JA: "終了してWindowsに戻りますか？"
        return 0;
    }
    if (cancelPressed || (g_inputBuffer[0].pressedPriority & 0x0200U)) {
        if (!ReturnToTitlePressStartTask())
            return 0;
        ResetMainMenuWheel();
        cancelPressed = false;
        g_inputBuffer[0].pressedPriority &= ~0x0200U;
        frontendRoute = FrontendRoute::Wheel;
        return 0;
    }

    if (g_mainMenuWheelWidget) {
        const MainMenuItem selection = static_cast<MainMenuItem>(g_mainMenuWheelWidget->selection);

        if (selection != mainMenuSelection)
            RefreshMainMenuWheelDescription();
        mainMenuSelection = selection;
    }
    const bool selected = enterPressed || (g_inputBuffer[0].pressedPriority & 0x0400U) != 0;
    if (!selected || IsUITransitionActive(g_mainMenuWheelInputBlock))
        return 0;
    return ActivateMainMenuChoice(mainMenuSelection);
}

std::int64_t ActivateMainMenuChoice(MainMenuItem selection) {

    FrontendRoute nextRoute = FrontendRoute::Wheel;
    bool transitioned = false;
    switch (selection) {
    case MainMenuItem::NewGame:
        nextRoute = FrontendRoute::NewGame;
        transitioned = BeginNewGameModuleHandoff();
        break;
    case MainMenuItem::Continue:
        nextRoute = FrontendRoute::Continue;
        transitioned = BeginContinueModuleHandoff();
        break;
    case MainMenuItem::Collection:
        nextRoute = FrontendRoute::TitleLoad;
        transitioned = BeginTitleLoadModuleHandoff();
        break;
    case MainMenuItem::Options:
        nextRoute = FrontendRoute::Options;
        transitioned = SelectMainMenuPage(2);
        break;
    case MainMenuItem::Shots:
        nextRoute = FrontendRoute::Wheel;
        transitioned = SelectMainMenuPage(3);
        break;
    }
    if (!transitioned)
        return 0;

    enterPressed = false;
    g_inputBuffer[0].pressedPriority &= ~0x0400U;
    if (selection != MainMenuItem::Options && selection != MainMenuItem::Shots) {
        DestroyElementByIndex(UIElement::MainMenu);
        DestroyElementByIndex(UIElement::MainMenuWheel);
    }
    frontendRoute = nextRoute;
    return 0;
}

float EvaluateTitleNoise(float value) {


    constexpr std::array<std::uint8_t, 256> gradients{{
        0x97,
        0xa0,
        0x89,
        0x5b,
        0x5a,
        0x0f,
        0x83,
        0x0d,
        0xc9,
        0x5f,
        0x60,
        0x35,
        0xc2,
        0xe9,
        0x07,
        0xe1,
        0x8c,
        0x24,
        0x67,
        0x1e,
        0x45,
        0x8e,
        0x08,
        0x63,
        0x25,
        0xf0,
        0x15,
        0x0a,
        0x17,
        0xbe,
        0x06,
        0x94,
        0xf7,
        0x78,
        0xea,
        0x4b,
        0x00,
        0x1a,
        0xc5,
        0x3e,
        0x5e,
        0xfc,
        0xdb,
        0xcb,
        0x75,
        0x23,
        0x0b,
        0x20,
        0x39,
        0xb1,
        0x21,
        0x58,
        0xed,
        0x95,
        0x38,
        0x57,
        0xae,
        0x14,
        0x7d,
        0x88,
        0xab,
        0xa8,
        0x44,
        0xaf,
        0x4a,
        0xa5,
        0x47,
        0x86,
        0x8b,
        0x30,
        0x1b,
        0xa6,
        0x4d,
        0x92,
        0x9e,
        0xe7,
        0x53,
        0x6f,
        0xe5,
        0x7a,
        0x3c,
        0xd3,
        0x85,
        0xe6,
        0xdc,
        0x69,
        0x5c,
        0x29,
        0x37,
        0x2e,
        0xf5,
        0x28,
        0xf4,
        0x66,
        0x8f,
        0x36,
        0x41,
        0x19,
        0x3f,
        0xa1,
        0x01,
        0xd8,
        0x50,
        0x49,
        0xd1,
        0x4c,
        0x84,
        0xbb,
        0xd0,
        0x59,
        0x12,
        0xa9,
        0xc8,
        0xc4,
        0x87,
        0x82,
        0x74,
        0xbc,
        0x9f,
        0x56,
        0xa4,
        0x64,
        0x6d,
        0xc6,
        0xad,
        0xba,
        0x03,
        0x40,
        0x34,
        0xd9,
        0xe2,
        0xfa,
        0x7c,
        0x7b,
        0x05,
        0xca,
        0x26,
        0x93,
        0x76,
        0x7e,
        0xff,
        0x52,
        0x55,
        0xd4,
        0xcf,
        0xce,
        0x3b,
        0xe3,
        0x2f,
        0x10,
        0x3a,
        0x11,
        0xb6,
        0xbd,
        0x1c,
        0x2a,
        0xdf,
        0xb7,
        0xaa,
        0xd5,
        0x77,
        0xf8,
        0x98,
        0x02,
        0x2c,
        0x9a,
        0xa3,
        0x46,
        0xdd,
        0x99,
        0x65,
        0x9b,
        0xa7,
        0x2b,
        0xac,
        0x09,
        0x81,
        0x16,
        0x27,
        0xfd,
        0x13,
        0x62,
        0x6c,
        0x6e,
        0x4f,
        0x71,
        0xe0,
        0xe8,
        0xb2,
        0xb9,
        0x70,
        0x68,
        0xda,
        0xf6,
        0x61,
        0xe4,
        0xfb,
        0x22,
        0xf2,
        0xc1,
        0xee,
        0xd2,
        0x90,
        0x0c,
        0xbf,
        0xb3,
        0xa2,
        0xf1,
        0x51,
        0x33,
        0x91,
        0xeb,
        0xf9,
        0x0e,
        0xef,
        0x6b,
        0x31,
        0xc0,
        0xd6,
        0x1f,
        0xb5,
        0xc7,
        0x6a,
        0x9d,
        0xb8,
        0x54,
        0xcc,
        0xb0,
        0x73,
        0x79,
        0x32,
        0x2d,
        0x7f,
        0x04,
        0x96,
        0xfe,
        0x8a,
        0xec,
        0xcd,
        0x5d,
        0xde,
        0x72,
        0x43,
        0x1d,
        0x18,
        0x48,
        0xf3,
        0x8d,
        0x80,
        0xc3,
        0x4e,
        0x42,
        0xd7,
        0x3d,
        0x9c,
        0xb4,
    }};
    std::int32_t integer = static_cast<std::int32_t>(value);
    if (static_cast<float>(integer) > value)
        --integer;
    const std::uint8_t firstIndex = static_cast<std::uint8_t>(integer);
    const std::uint8_t secondIndex = static_cast<std::uint8_t>(integer + 1);
    const float fraction = value - static_cast<float>(integer);
    const float shifted = fraction - 1.0f;
    const auto gradient = [&gradients](std::uint8_t index) {
        float result = static_cast<float>(gradients[index] & 7U) + 1.0f;
        if ((gradients[index] & 8U) != 0)
            result = -result;
        return result;
    };
    const float firstCurve = 1.0f - fraction * fraction;
    const float secondCurve = 1.0f - shifted * shifted;
    const float firstSquared = firstCurve * firstCurve;
    const float first = (gradient(firstIndex) * fraction) * (firstSquared * firstSquared);
    const float secondSquared = secondCurve * secondCurve;
    const float second = (gradient(secondIndex) * shifted) * (secondSquared * secondSquared);
    return (second + first) * 0.39500001f;
}

// @verified - p
void UpdatePressStartTransitions() {

    PressStartRuntime &runtime = pressStartRuntime;
    if (!runtime.frozen)
        return;
    d3t ::UIViewSlot<kUIViewImageCapacity> *const view =
        GetNuklearElementByIdx(g_uiElements[static_cast<std::size_t>(UIElement::PressStart)].config.uiHandleIndex);
    for (std::size_t index = 0; index < 14; ++index) {
        d3t::UIWindow *const window = runtime.windows[index];
        const std::int32_t transition = runtime.transitions[index];
        if (transition == -1 || !window)
            continue;
        if (IsUITransitionActive(transition))
            window->alpha = GetUITransitionValue(transition);
        if (index >= 7 && index <= 8 && runtime.angleTransition != -1) {
            d3t::UIViewImage &image = view->images[static_cast<std::size_t>(runtime.murals[index - 7]->image)];
            image.rotate = 1;
            image.angle = GetUITransitionValue(runtime.angleTransition);
        }
        if (index == 3) {
            runtime.windows[20]->alpha = window->alpha;
            runtime.windows[21]->alpha = window->alpha;
        }
    }
}

void UpdatePressStartLight(PressStartLightState &light, std::int32_t firstWindow) {
    PressStartRuntime &runtime = pressStartRuntime;
    float opacity = 0.0f;
    if (runtime.transitions[6] == -1) {
        if (!runtime.frozen)
            opacity = 1.0f;
    } else {
        opacity = GetUITransitionValue(runtime.transitions[6]);
    }
    if (!(light.duration > 0.0f))
        return;
    if (light.paused == 0)
        light.phase = std::fmax(0.0f, std ::fmin(1.0f, ((1.0f / light.duration) * (GetFrameTime() * static_cast<float>(light.direction))) + light.phase));
    const float noise = EvaluateTitleNoise(light.phase);
    const float wave = std::sin((noise + light.phase) * 3.1415927f);
    d3t::UIWindow &shadow = *runtime.windows[static_cast<std::size_t>(light.shadowWindow)];
    shadow.layout.offsetX = ((wave * light.shadowAmplitude) * light.shadowScale.x) / 1920.0f;
    shadow.layout.offsetY = ((wave * light.shadowAmplitude) * light.shadowScale.y) / 1080.0f;
    for (std::size_t index = 0; index < light.bands.size(); ++index) {
        PressStartLightBand &band = light.bands[index];
        const float width = band.end - band.begin;
        if (width <= 0.0f)
            band.value = 0.0f;
        else
            band.value = std ::fmax(0.0f, std::fmin(1.0f, (wave - band.begin) / width));
        float amount;
        if (index == 0)
            amount = 1.0f - (std::sin((band.value * 3.1415927f) + 1.5707964f) + 1.0f) * 0.5f;
        else if (index == 1)
            amount = (std::sin((band.value * 6.2831855f) - 1.5707964f) + 1.0f) * 0.5f;
        else
            amount = (std::sin((band.value * 3.1415927f) - 1.5707964f) + 1.0f) * 0.5f;
        runtime.windows[static_cast<std::size_t>(firstWindow) + index]->alpha = amount * opacity;
    }
    if (light.phase == 1.0f)
        light.direction = -1;
    if (light.phase == 0.0f)
        light.direction = 1;
}
}
// namespace ui_detail
// @verified - p
std::int32_t GetButtonWheelDirection(std::int32_t index) {
    std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    if (IsHelpOverlayVisible(UIElement::YesNoPrompt))
        return 0;
    UIButtonWheel &wheel = g_uiButtonWheels[static_cast<std::size_t>(index)];
    std::int32_t direction = 0;
    if ((input & 0x8000U) != 0 && !IsUITransitionActive(wheel.items[static_cast<std::size_t>(wheel.selection)].animSlots[0])) {
        if (--wheel.selection < 0)
            wheel.selection = wheel.count - 1;
        direction = 1;
        input = 0;
        AicaExecuteWrapper(3);
    }
    if ((input & 0x4000U) != 0 && !IsUITransitionActive(wheel.items[static_cast<std::size_t>(wheel.selection)].animSlots[0])) {
        if (++wheel.selection >= wheel.count)
            wheel.selection = 0;
        direction = -1;
        AicaExecuteWrapper(3);
    }
    return direction;
}

// @verified - p
namespace ui_detail {
std::array<std::int16_t, 4> mainMenuWheelHintIds = MAIN_MENU_WHEEL_HINTS;

// @verified - p
std::int64_t MainMenuWheelHints() {
    mainMenuWheelHintIds[3] = 3845;
    return CopyHudHintIds(mainMenuWheelHintIds);
}
} // namespace ui_detail
void UpdateMainMenuWheel() {
    if (g_mainMenuWheelIndex == -1)
        return;
    const std::int32_t direction = GetButtonWheelDirection(g_mainMenuWheelIndex);
    if (direction == 0 && g_mainMenuWheelPending == 0)
        return;
    UIAnim_Start(g_mainMenuWheelInputBlock, 0.090000004f, 0.0f, 0.090000004f, 0.0f);
    g_mainMenuWheelPending = 0;
    for (std::int32_t ordinal = 0; ordinal < g_mainMenuWheelWidget->count; ++ordinal) {
        UIButtonWheelItem &item = g_mainMenuWheelWidget->items[static_cast<std::size_t>(ordinal)];
        const std::int32_t previous = item.index;
        item.index += direction;
        if (item.index < 0)
            item.index = g_mainMenuWheelWidget->count - 1;
        else if (item.index >= g_mainMenuWheelWidget->count)
            item.index = 0;
        if (item.index == 0)
            g_mainMenuWheelWidget->selection = ordinal;
        if (item.animSlots[0] == -1)
            item.animSlots[0] = UIAnimAllocSlot();
        if (item.animSlots[0] == -1)
            continue;
        StartUIAngleTransition(item.animSlots[0], g_mainMenuWheelWidget->angles[static_cast<std::size_t>(previous)],
            g_mainMenuWheelWidget->angles[static_cast<std::size_t>(item.index)], direction, 0.30000001f, 0.0f);
        if (g_mainMenuWheelWidget->override != 0) {
            if (item.animSlots[1] == -1)
                item.animSlots[1] = UIAnimAllocSlot();
            item.index84 += direction;
            if (item.index84 < 0)
                item.index84 = g_mainMenuWheelWidget->count - 1;
            if (item.index84 >= g_mainMenuWheelWidget->count)
                item.index84 = 0;
            UIAnim_Start(item.animSlots[1], item.placeScale, g_mainMenuWheelWidget->scales[static_cast<std::size_t>(item.index84)][0], 0.30000001f, 0.0f);
        }
    }
}

// @verified - p
void RefreshMainMenuWheelDescription() {
    UIMessageBox message{};
    if (g_mainMenuWheelWidget)
        message.primaryText = g_mainMenuWheelWidget->items[static_cast<std::size_t>(g_mainMenuWheelWidget->selection)].field58;
    message.hideTarget = 1;
    CopyUIMessageBox(&message);
}

// @verified - p
std::intptr_t RefreshMainMenuWheelLanguage() {
    const std::int32_t language = GetLoadingScreenTextType(ResetSceneTextLanguage());
    if (!g_mainMenuWheelWidget)
        return language;
    for (std::size_t item = 0; item < 5; ++item)
        g_mainMenuWheelWidget->items[item].field48[0] = g_mainMenuWheelImages[static_cast<std::size_t>(language) * 5 + item];
    return reinterpret_cast<std::intptr_t>(g_mainMenuWheelWidget);
}
// @verified - p
std::intptr_t ResetMainMenuWheel() {
    if (!g_mainMenuWheelWidget)
        return 0;
    for (std::int32_t ordinal = 0; ordinal < 5; ++ordinal)
        g_mainMenuWheelWidget->items[static_cast<std::size_t>(ordinal)].index = ordinal;
    if ((GetLocalBackupInfo() & 0x40U) != 0) {
        for (std::size_t ordinal = 0; ordinal < 5; ++ordinal)
            --g_mainMenuWheelWidget->items[ordinal].index;
        g_mainMenuWheelWidget->items[0].index = 4;
    }
    g_mainMenuWheelWidget->selection = 0;
    g_mainMenuWheelPending = 1;
    return reinterpret_cast<std::intptr_t>(g_mainMenuWheelWidget);
}

void FreezePressStartPresentation() {
    PressStartRuntime &runtime = pressStartRuntime;
    runtime.frozen = true;
    if (g_uiElements[static_cast<std::size_t>(UIElement::PressStart)].config.init != 3)
        return;
    for (std::size_t index = 0; index < runtime.transitions.size(); ++index) {
        HideUIWindow(runtime.windows[index]);
        if (runtime.transitions[index] != -1) {
            ResetUITransition(runtime.transitions[index]);
            runtime.transitions[index] = -1;
        }
    }

    if (runtime.windows[6])
        runtime.windows[6]->colour &= 0x00ffffffU;
}

// @verified - p: 4f8200 does not reset the retained angle or blink transitions.
void RestorePressStartPresentation() {
    pressStartRuntime.frozen = false;
    ApplyPressStartDefaultOpacity();
}

void AnimatePressStartWidget(std::int32_t widget, float duration, float from, float to) {
    std::int32_t &transition = pressStartRuntime.transitions[static_cast<std::size_t>(widget)];
    if (transition == -1)
        transition = UIAnimAllocSlot();
    UIAnim_Start(transition, from, to, duration, 0.0f);
}

void AnimatePressStartAngle(float duration, float from, float to) {
    if (pressStartRuntime.angleTransition == -1)
        pressStartRuntime.angleTransition = UIAnimAllocSlot();
    UIAnim_Start(pressStartRuntime.angleTransition, from, to, duration, 0.0f);
}
void ProcessTitleMenuInput() {
    ExecuteElementCallback(UIElement::MainMenu);
}

void ProcessTitleOptionsInput() {
    FrontendOptionsInput();
}

void ResetPressStartLogoImages() {
    PressStartRuntime &runtime = pressStartRuntime;
    const std::int32_t language = GetLoadingScreenTextType(ResetSceneTextLanguage());
    const std::size_t variant = language == 4 ? 1U : static_cast<std::uint32_t>(language - 5) <= 1U ? 2U : 0U;
    runtime.logos[0]->image = runtime.images[variant];
    runtime.logos[1]->image = runtime.images[3 + variant];
    runtime.logos[2]->image = runtime.images[6 + variant];
    runtime.logos[3]->image = runtime.images[6 + variant];
}

void SetForceSkipSplashScreens(bool enabled) {
    forceSkipSplashScreens = enabled;
}

bool PressStartIsReady() {
    constexpr std::size_t index = static_cast<std::size_t>(UIElement::PressStart);
    const UIElementState &element = g_uiElements[index];
    if (element.config.init != 3 || (gameUiSecondaryFlags[index / 32] & (1U << (index & 31U))) == 0)
        return false;
    const d3t::UIDefinition &definition = uiDefinitions[index];
    constexpr std::array<const char *, 4> requiredImages{"Background", "GameLogo", "GameLogo_Shadow", "SegaCopyright"};
    for (const char *name : requiredImages) {
        const d3t::UIImageDefinition *const image = definition.findImage(name);
        if (!image || !image->textureHandle)
            return false;

        std::uint32_t width = 0;
        std::uint32_t height = 0;
        if (!d3t::GetTexture2DDimensions(image->textureHandle, &width, &height) || !width || !height) {
            return false;
        }
    }
    return true;
}

bool MainMenuIsReady() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::MainMenu)];
    return element.isVisible != 0 && element.config.init == 3;
}

bool MainMenuFrameWasRendered() {
    return mainMenuFrameWasRendered && mainMenuWheelFrameWasRendered;
}

// @verified - p
bool SplashSequenceComplete() {
    return static_cast<std::uint32_t>(splashScreenIndex) >= 6U;
}

std::int32_t GetSplashScreenIndex() {
    return splashScreenIndex;
}
void SetContinueAvailability(bool available) {
    continueAvailable = available;
    if (!available && mainMenuSelection == MainMenuItem::Continue)
        mainMenuSelection = MainMenuItem::NewGame;
}

bool ContinueIsAvailable() {
    return continueAvailable;
}

MainMenuItem GetMainMenuSelection() {
    return mainMenuSelection;
}

bool SetMainMenuSelection(MainMenuItem item) {
    const std::int32_t value = static_cast<std::int32_t>(item);
    if (value < 0 || value > static_cast<std::int32_t>(MainMenuItem::Shots))
        return false;
    if (item == MainMenuItem::Continue && !continueAvailable)
        return false;
    mainMenuSelection = item;
    return true;
}

void MoveMainMenuSelection(std::int32_t delta) {
    if (delta == 0)
        return;

    constexpr std::int32_t count = static_cast<std::int32_t>(MainMenuItem::Shots) + 1;
    std::int32_t value = static_cast<std::int32_t>(mainMenuSelection);
    const std::int32_t direction = delta < 0 ? -1 : 1;
    for (std::int32_t step = 0; step < count; ++step) {
        value = (value + direction + count) % count;
        const auto item = static_cast<MainMenuItem>(value);
        if (SetMainMenuSelection(item))
            return;
    }
}

FrontendRoute GetFrontendRoute() {
    return frontendRoute;
}

void SetFrontendRoute(FrontendRoute route) {
    frontendRoute = route;
}

void ReturnToTitleWheel() {
    DestroyElementByIndex(UIElement::PressStart);
    DestroyElementByIndex(UIElement::SubMenuOptions);
    DestroyElementByIndex(UIElement::MainMenuNew);
    DestroyElementByIndex(UIElement::MainMenuContinue);
    DestroyElementByIndex(UIElement::ShenmueCollection);
    ShowElementByIndex(UIElement::MainMenu);
    ShowElementByIndex(UIElement::MainMenuWheel);
    frontendRoute = FrontendRoute::Wheel;
}
} // namespace shenmue::s2
