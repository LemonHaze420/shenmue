#include "s2/ui_internal.hpp"
#include "s2/ui_credits.hpp"
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
#include "s2/ui_loading.hpp"
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

    namespace shenmue::s2 {
using namespace ui_detail;

std::array<std::array<char, 256>, 3> g_moveLearnedUiText{};
std::int32_t g_loadScreenSpriteTexture = -1;
std::uint32_t g_helpIconState{};

std::array<UIElementState, static_cast<std::size_t>(UIElement::Count)> g_uiElements{};
std::uint32_t g_nuklearTexture = 0;
std::int32_t requestedCameraIconMode{};
std::array<std::uint32_t, 3> g_uiRefreshedElements{};
std::array<std::uint32_t, 3> g_uiPreviousRefreshedElements{};
std::array<std::uint32_t, 3> g_uiPreviousInputElements{};
std::array<std::int32_t, static_cast<std::size_t>(UIElement::Count)> g_uiFrameElementHistory{};
std::int32_t g_uiFrameElementHistoryIndex{};
std::int32_t g_uiDebugRevealAspectBars{};
std::int32_t g_uiHelpAdmission{};
float g_uiHelpAdmissionValue{};
std::int32_t g_uiFreeInputOverride{};
UIButtonWheel *g_mainMenuWheelWidget{};
std::int32_t g_mainMenuWheelIndex{};
std::int32_t g_mainMenuWheelInputBlock{};
std::uint8_t g_mainMenuWheelPending{};
std::array<std::int32_t, 35> g_mainMenuWheelImages{};

namespace ui_detail {
#include "s2/realui_data.inl"

std::array<std::vector<std::byte>, static_cast<std::size_t>(UIElement::Count)> uiDefinitionData;
std::array<d3t::UIDefinition, static_cast<std::size_t>(UIElement::Count)> uiDefinitions;
std::array<std::atomic<bool>, static_cast<std::size_t>(UIElement::Count)> uiDefinitionReady{};
std::atomic<bool> loadSaveDefinitionFailed{false};
std::size_t loadSaveSelectedSlot = 0;
CameraState loadSaveSavedCameraState{};
std::int32_t currentUIState = -1;
std::int32_t splashScreenIndex = 0;
std::uint32_t previousRenderHandle = 0;
bool enterPressed = false;
bool forceSkipSplashScreens = false;
PressStartRuntime pressStartRuntime;
bool mainMenuFrameWasRendered = false;
bool mainMenuWheelFrameWasRendered = false;
bool cancelPressed = false;
bool sleepSelectionActive = false;
std::int32_t sleepSelectionIconId = 0;
DialogueSelectionPromptState dialogueSelectionPrompt{};
bool dialogueSelectionOwned{};
SubtitlePresentationState subtitlePresentation{};
std::array<std::array<std::uint32_t, 3>, 4> subtitleVoicePalettes{};
std::array<std::uint32_t, 3> subtitleVoiceColours{};
float subtitleCinemaOffset{};
std::vector<d3t::UIResolvedLabel> subtitleLabelRectangles;
std::int32_t uiFontLanguage = -1;
std::uint32_t uiFontRenderHandle = 0;
MainMenuItem mainMenuSelection = MainMenuItem::NewGame;

LoadingScreenRuntime loadingScreen{};
LoadSaveRuntime loadSaveRuntime{};

struct GameMovesRuntime {
    UIGrid grid{};
    std::array<std::int32_t, 59> dictionary{};
    std::array<d3t::UIWindow *, 23> windows{};
    std::array<d3t::UILayoutItem *, 25> layoutItems{};
    std::array<d3t::UILabelWidget *, 5> labels{};
    std::array<std::array<char, 256>, 2> labelText{};
    std::array<UIColourImageWidget *, 5> images{};
    std::array<UIButtonIconWidget *, 6> icons{};
    std::array<UIButtonWidget *, 6> buttons{};
    UIButtonIconWidget *critical{};
    UIGridTemplate cellTemplate{};
    std::int32_t criticalTransition{-1}, arrowTransition{-1}, criticalFrame{}, selectedMove{};
    bool previewLoaded{}, bound{};
};

GameMovesRuntime gameMovesRuntime{};
QteMinigameRuntime qteMinigame{};

std::array<BigKanjiGlyph, 8> bigKanjiGlyphs{};
std::array<d3t::UIWindow *, 8> bigKanjiPanels{};
std::array<UIColourImageWidget *, 8> bigKanjiImages{};

struct ParentMenuLayout {
    std::vector<std::int32_t> parents;
    std::vector<d3t::UIRect> panels;
};
std::array<ParentMenuLayout, static_cast<std::size_t>(UIElement::Count)> parentMenuLayouts{};
std::array<d3t::UIWindow *, 13> gameMenuWindows{};
std::array<d3t::UILabelWidget *, 9> gameMenuNativeLabels{};
std::array<UIColourImageWidget *, 6> gameMenuNativeIcons{};
std::array<std::array<char, 256>, 3> gameMenuInformationText{};
std::int32_t gameMenuCurrent{}, gameMenuPending{-1};
std::uint8_t gameMenuClosing{};
std::int32_t gameMenuActive{};
struct ItemsMenuRuntime {
    UIGrid grid{};
    std::array<std::array<std::int32_t, 8>, 384> dictionary{};
    std::array<d3t::UIWindow *, 5> windows{};
    std::array<d3t::UILayoutItem *, 2> layoutItems{};
    UIGridTemplate cellTemplate{};
    std::int32_t count{};
    bool bound{};
    std::array<char, 32> quantityText{};
};
ItemsMenuRuntime itemsMenu{};
std::int32_t gameMovesItemActionState{};
WindowUIInput uiPointer{};
bool drawingUI{true};
std::array<std::array<std::uint32_t, 5>, 4> menuInputHintHistory{};
std::int32_t informationBarVisibilityMode{}, legacyMenuInputHintState{};
UIAction legacyMenuInputHintCallback{};

void ClearMenuInputHints() noexcept {
    for (auto &frame : menuInputHintHistory)
        for (auto &entry : frame)
            entry = (entry & 0xffff0000U) | 0xffffU;
}
void SetInformationBarVisibilityMode(std::int32_t mode) noexcept {
    informationBarVisibilityMode = mode;
    if (mode)
        legacyMenuInputHintCallback = nullptr;
    else
        legacyMenuInputHintState = 1;
}

InformationBarRuntime informationBar{};

void RestorePreviousInformationBarMessage() {
    informationBar.previous.primaryLocalized = 1;
    informationBar.previous.primaryText = informationBar.previousText.data();
    CopyUIMessageBox(&informationBar.previous);
}

std::array<std::array<char, 256>, 4> streetFighterHudText{};
std::array<d3t ::UIWindow *, 2> streetFighterHudWindows{};
std::array<d3t::UILabelWidget *, 4> streetFighterHudLabels{};

std::array<std::array<char, 256>, 7> duckRaceHudText{};
std::array<d3t ::UIWindow *, 13> duckRaceHudWindows{};
std::array<d3t ::UILabelWidget *, 7> duckRaceHudLabels{};
std::array<UIButtonIconWidget *, 6> duckRaceHudBowties{};

std::array<d3t ::UILabelWidget *, 3> moveLearnedLabels{};

std::array<SnapshotImageRuntime, 126> snapshotImages{};
std::array<std::uint32_t, 126> snapshotOrder{};
std::uint32_t snapshotOrderCount{};
FrontendRoute frontendRoute = FrontendRoute::Wheel;
bool continueAvailable = false;

HLib::Task *openingStaffRollTask = nullptr;
std::array<d3t::UITextureObjectDefinition *, 2> introCreditSprites{};
d3t::UIPanelDefinition *introLogoPanel = nullptr;
d3t::UITextureObjectDefinition *introLogoImage = nullptr;
std::vector<d3t::UIResolvedTextureObject> introObjectRectangles;
HLib::Task *endingStaffRollTask = nullptr;

std::int32_t endingStaffRollFlag{};

// SetIntroCredit writes the authored widget, not the legacy OPST texture.
void SetIntroCredit(std::int32_t item, float alpha, std::int32_t side) {
    if (item < 0) {
        if (introLogoPanel)
            introLogoPanel->imageOpacity = alpha;
    } else if (d3t::UITextureObjectDefinition *const sprite = introCreditSprites[side]) {
        sprite->frameIndex = item;
        sprite->colour = 0x00ffffffU | (static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 255.0f)) << 24);
    }
}

void DrawOpeningStaffRoll(void *parameter) {
    const OpeningStaffRollTaskData &data = *static_cast<OpeningStaffRollTaskData *>(parameter);
    EnableElement(UIElement::IntroCutsceneCredits);
    const std::uint32_t colour = (static_cast<std::uint32_t>(TruncateSpriteCoordinate(data.alpha * 255.0f)) << 24) | 0x00ffffffU;
    for (const std::int8_t item : OPENING_STAFF_ROLL_ITEMS[data.item + 1]) {
        if (item == -1)
            continue;
        LegacySpritePacket packet{};
        packet.quad.flags = 0x400;
        packet.colour = colour;
        packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
        std::int32_t texture = 0;
        if (item == 10 || item == 11) {
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 320, item == 10 ? 176 : 304, 512, 128, ScreenMode::Centered);
            texture = item == 10 ? 1 : 2;
            if (item == 10)
                SetIntroCredit(-1, data.alpha, -1);
        } else {
            const float left = item >= 5 ? 0.546875f : 0.0f;
            const float right = (item / 5 != 0 ? 0.453125f : 0.546875f) + left;
            const float top = item == 3 ? 0.625f : item == 4 ? 0.375f : 1.0f - static_cast<float>(item % 5) * 0.125f;
            const float bottom = item == 3 ? 0.375f : item == 4 ? 0.25f : top - 0.125f;
            packet.uv = {left, bottom, right, top};
            const std::int32_t side = OPENING_STAFF_ROLL_SIDES[item];
            const ScreenMode mode = side == 0 ? static_cast<ScreenMode>(0) :
                side == 1                     ? ScreenMode::Stretched :
                side == 2                     ? ScreenMode::Centered :
                                                ScreenMode::AdjustedWithOffset;
            AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(OPENING_STAFF_ROLL_ALIGNMENTS[side]),
                TruncateSpriteCoordinate(OPENING_STAFF_ROLL_POSITIONS[side][0]), TruncateSpriteCoordinate(OPENING_STAFF_ROLL_POSITIONS[side][1]),
                TruncateSpriteCoordinate((right - left) * 512.0f), TruncateSpriteCoordinate((top - bottom) * 512.0f), mode);
            SetIntroCredit(item, data.alpha, side);
        }
        if (data.textures[texture] >= 0)
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(data.textures[texture]), 0.1f);
    }
}

// @verified - p
void OP_StaffRoll_Callback(void *parameter) {
    OpeningStaffRollTaskData &data = *static_cast<OpeningStaffRollTaskData *>(parameter);
    if (data.item < 0) {
        HLib::DestroyCurrentTask();
        openingStaffRollTask = nullptr;
        return;
    }
    if (data.clock >= 0.0f) {
        if (data.clock >= static_cast<float>(OPENING_STAFF_ROLL_INTERVALS[data.item][1]) && data.phase == 3) {
            data.phase = 1;
            data.alpha = 1.0f;
        } else if (data.clock >= static_cast<float>(OPENING_STAFF_ROLL_INTERVALS[data.item][0]) && data.phase == 2) {
            data.phase = 0;
            data.alpha = 0.0f;
        }
    }
    if (data.phase == 0) {
        data.alpha += 0.033333335f;
        if (data.alpha > 1.0f) {
            data.alpha = 1.0f;
            data.phase = 3;
        }
    } else if (data.phase == 1 && data.alpha > 0.0f) {
        data.alpha -= 0.033333335f;
        if (data.alpha < 0.0f) {
            data.alpha = 0.0f;
            data.phase = 2;
            if (++data.item >= 8)
                data.item = -1;
        }
    }
    data.clock = shenGetScaledDeltaTickTime(1.0f) + data.clock;
    HLib::ExecuteCallbackNow(DrawOpeningStaffRoll, &data);
}

std::uint32_t currentItemSelectState{};

std::array<CommonIconAtlas, 16> commonIconAtlases{};


constexpr std::array<std::uint8_t, 0x280> commonIconRectangles{
    0x00,
    0x06,
    0x08,
    0x08,
    0x00,
    0x04,
    0x02,
    0x06,
    0x06,
    0x05,
    0x08,
    0x06,
    0x02,
    0x04,
    0x04,
    0x06,
    0x04,
    0x04,
    0x06,
    0x06,
    0x00,
    0x00,
    0x02,
    0x02,
    0x02,
    0x03,
    0x03,
    0x04,
    0x03,
    0x03,
    0x04,
    0x04,
    0x04,
    0x03,
    0x05,
    0x04,
    0x05,
    0x03,
    0x06,
    0x04,
    0x06,
    0x03,
    0x07,
    0x04,
    0x07,
    0x03,
    0x08,
    0x04,
    0x02,
    0x02,
    0x03,
    0x03,
    0x03,
    0x02,
    0x04,
    0x03,
    0x04,
    0x02,
    0x05,
    0x03,
    0x05,
    0x02,
    0x06,
    0x03,
    0x06,
    0x02,
    0x07,
    0x03,
    0x07,
    0x02,
    0x08,
    0x03,
    0x02,
    0x01,
    0x03,
    0x02,
    0x03,
    0x01,
    0x04,
    0x02,
    0x04,
    0x01,
    0x05,
    0x02,
    0x05,
    0x01,
    0x06,
    0x02,
    0x06,
    0x01,
    0x07,
    0x02,
    0x07,
    0x01,
    0x08,
    0x02,
    0x02,
    0x00,
    0x03,
    0x01,
    0x03,
    0x00,
    0x04,
    0x01,
    0x04,
    0x00,
    0x05,
    0x01,
    0x05,
    0x00,
    0x06,
    0x01,
    0x06,
    0x00,
    0x07,
    0x01,
    0x06,
    0x04,
    0x07,
    0x05,
    0x07,
    0x04,
    0x08,
    0x05,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0x04,
    0x08,
    0x04,
    0x04,
    0x05,
    0x08,
    0x05,
    0x04,
    0x06,
    0x08,
    0x06,
    0x04,
    0x07,
    0x08,
    0x07,
    0x04,
    0x08,
    0x08,
    0x00,
    0x03,
    0x02,
    0x04,
    0x00,
    0x01,
    0x02,
    0x03,
    0x05,
    0x01,
    0x08,
    0x02,
    0x02,
    0x03,
    0x05,
    0x04,
    0x02,
    0x02,
    0x05,
    0x03,
    0x02,
    0x01,
    0x05,
    0x02,
    0x02,
    0x00,
    0x05,
    0x01,
    0x05,
    0x03,
    0x08,
    0x04,
    0x05,
    0x02,
    0x08,
    0x03,
    0x05,
    0x00,
    0x06,
    0x01,
    0x06,
    0x00,
    0x07,
    0x01,
    0x00,
    0x04,
    0x02,
    0x06,
    0x00,
    0x02,
    0x02,
    0x04,
    0x02,
    0x04,
    0x04,
    0x06,
    0x02,
    0x02,
    0x04,
    0x04,
    0x04,
    0x04,
    0x06,
    0x06,
    0x04,
    0x02,
    0x06,
    0x04,
    0x06,
    0x04,
    0x08,
    0x06,
    0x06,
    0x02,
    0x08,
    0x04,
    0x02,
    0x06,
    0x06,
    0x08,
    0x00,
    0x07,
    0x02,
    0x08,
    0x00,
    0x06,
    0x02,
    0x07,
    0x00,
    0x00,
    0x02,
    0x02,
    0x02,
    0x00,
    0x04,
    0x02,
    0x04,
    0x01,
    0x05,
    0x02,
    0x04,
    0x00,
    0x05,
    0x01,
    0x05,
    0x01,
    0x06,
    0x02,
    0x05,
    0x00,
    0x06,
    0x01,
    0x06,
    0x01,
    0x07,
    0x02,
    0x06,
    0x07,
    0x07,
    0x08,
    0x07,
    0x07,
    0x08,
    0x08,
    0x00,
    0x04,
    0x04,
    0x08,
    0x04,
    0x04,
    0x08,
    0x08,
    0x00,
    0x00,
    0x04,
    0x04,
    0x04,
    0x00,
    0x08,
    0x04,
    0x00,
    0x06,
    0x02,
    0x08,
    0x02,
    0x06,
    0x04,
    0x08,
    0x04,
    0x06,
    0x06,
    0x08,
    0x06,
    0x06,
    0x08,
    0x08,
    0x00,
    0x04,
    0x04,
    0x06,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x01,
    0x08,
    0x01,
    0x07,
    0x02,
    0x08,
    0x02,
    0x07,
    0x03,
    0x08,
    0x03,
    0x07,
    0x04,
    0x08,
    0x04,
    0x07,
    0x05,
    0x08,
    0x05,
    0x07,
    0x06,
    0x08,
    0x06,
    0x07,
    0x07,
    0x08,
    0x07,
    0x07,
    0x08,
    0x08,
    0x00,
    0x06,
    0x01,
    0x07,
    0x01,
    0x06,
    0x02,
    0x07,
    0x02,
    0x06,
    0x03,
    0x07,
    0x03,
    0x06,
    0x04,
    0x07,
    0x04,
    0x06,
    0x05,
    0x07,
    0x05,
    0x06,
    0x06,
    0x07,
    0x06,
    0x06,
    0x07,
    0x07,
    0x07,
    0x06,
    0x08,
    0x07,
    0x00,
    0x05,
    0x01,
    0x06,
    0x01,
    0x05,
    0x02,
    0x06,
    0x02,
    0x05,
    0x03,
    0x06,
    0x03,
    0x05,
    0x04,
    0x06,
    0x04,
    0x05,
    0x05,
    0x06,
    0x05,
    0x05,
    0x06,
    0x06,
    0x06,
    0x05,
    0x07,
    0x06,
    0x07,
    0x05,
    0x08,
    0x06,
    0x00,
    0x04,
    0x01,
    0x05,
    0x01,
    0x04,
    0x02,
    0x05,
    0x02,
    0x04,
    0x03,
    0x05,
    0x03,
    0x04,
    0x04,
    0x05,
    0x04,
    0x04,
    0x05,
    0x05,
    0x05,
    0x04,
    0x06,
    0x05,
    0x06,
    0x04,
    0x07,
    0x05,
    0x07,
    0x04,
    0x08,
    0x05,
    0x00,
    0x03,
    0x01,
    0x04,
    0x01,
    0x03,
    0x02,
    0x04,
    0x02,
    0x03,
    0x03,
    0x04,
    0x03,
    0x03,
    0x04,
    0x04,
    0x04,
    0x03,
    0x05,
    0x04,
    0x05,
    0x03,
    0x06,
    0x04,
    0x06,
    0x03,
    0x07,
    0x04,
    0x07,
    0x03,
    0x08,
    0x04,
    0x00,
    0x02,
    0x01,
    0x03,
    0x01,
    0x02,
    0x02,
    0x03,
    0x02,
    0x02,
    0x03,
    0x03,
    0x03,
    0x02,
    0x04,
    0x03,
    0x04,
    0x02,
    0x05,
    0x03,
    0x05,
    0x02,
    0x06,
    0x03,
    0x06,
    0x02,
    0x07,
    0x03,
    0x07,
    0x02,
    0x08,
    0x03,
    0x00,
    0x01,
    0x01,
    0x02,
    0x01,
    0x01,
    0x02,
    0x02,
    0x02,
    0x01,
    0x03,
    0x02,
    0x03,
    0x01,
    0x04,
    0x02,
    0x04,
    0x01,
    0x05,
    0x02,
    0x05,
    0x01,
    0x06,
    0x02,
    0x06,
    0x01,
    0x07,
    0x02,
    0x07,
    0x01,
    0x08,
    0x02,
    0x00,
    0x00,
    0x01,
    0x01,
    0x01,
    0x00,
    0x02,
    0x01,
    0x02,
    0x00,
    0x03,
    0x01,
    0x03,
    0x00,
    0x04,
    0x01,
    0x04,
    0x00,
    0x05,
    0x01,
    0x05,
    0x00,
    0x06,
    0x01,
    0x06,
    0x00,
    0x07,
    0x01,
    0x07,
    0x00,
    0x08,
    0x01,
    0x00,
    0x06,
    0x02,
    0x08,
    0x02,
    0x06,
    0x04,
    0x08,
    0x04,
    0x06,
    0x06,
    0x08,
    0x06,
    0x06,
    0x08,
    0x08,
    0x00,
    0x04,
    0x02,
    0x06,
    0x02,
    0x04,
    0x04,
    0x06,
    0x04,
    0x04,
    0x06,
    0x06,
    0x06,
    0x04,
    0x08,
    0x06,
    0x00,
    0x02,
    0x02,
    0x04,
    0x02,
    0x02,
    0x04,
    0x04,
    0x04,
    0x02,
    0x06,
    0x04,
    0x06,
    0x02,
    0x08,
    0x04,
    0x00,
    0x00,
    0x02,
    0x02,
    0x02,
    0x00,
    0x04,
    0x02,
    0x04,
    0x00,
    0x06,
    0x02,
    0x06,
    0x00,
    0x08,
    0x02,
};
constexpr std::array<std::uint16_t, 16> commonIconRectangleOffsets{
    0x140,
    0x240,
    0x240,
    0x240,
    0,
    0x80,
    0xc0,
    0x110,
    0x110,
    0x120,
    0x140,
    0x140,
    0x140,
    0x140,
    0x240,
    0x140,
};
std::uint32_t commonIconAtlasCount{};
std::uint32_t renderIconFlags{};
std::uint32_t activeIconFlags{};
PadControlEntry *iconPadControl{};
PadControlEntry *previousIconPadControl{};
static_assert(sizeof(GuiPtxHeader) == 0x30);
static_assert(offsetof(GuiPtxHeader, width) == 0x28);
std::vector<std::int8_t> actionSelectorPanelChoices;
std::vector<std::int8_t> actionSelectorObjectChoices;
std::vector<std::int8_t> actionSelectorLabelChoices;
std::array<std::array<char, 256>, 4> actionSelectorText{};
AuthoredMenuLayout actionSelectorLayout;
UIViewConfiguration moneyHudConfiguration{-1, 0, -1};
std::int32_t moneyHudState{};
std::uint32_t moneyHudRequest{};
d3t::UILabelWidget *moneyHudLabel{};
std::array<char, 512> moneyHudText{};
AuthoredMenuLayout cutsceneSkipLayout;

std::array<char, 256> gameHudTimerText{};
d3t::UIWindow *gameHudTimerPanel{};
d3t::UILabelWidget *gameHudTimerLabel{};
std::int32_t moneyHudCashAdjustment{};

d3t::UIPanelDefinition *actionSelectorMainPanel{};
std::int32_t actionSelectorOutPanel{-1};
std::int32_t actionSelectorInPanel{-1};
std::int32_t actionSelectorMotionTransition{-1};
std::int32_t actionSelectorPulseTransition{-1};
std::int32_t actionSelectorButtonTransition{-1};
std::int32_t actionSelectorSelectedButton{-1};
std::int32_t actionSelectorButtonFrame{};
bool actionSelectorSelectionLatched{};
std::uint16_t actionSelectorPreviousInput{};
std::uint8_t actionSelectorInput{13};
NumericActionSelector numericActionSelector{};

constexpr std::array<const char *, 32> sharedItemImageNames{
    "Icon01",
    "Icon02",
    "Icon03",
    "item_128_c",
    "item_128_d",
    "item_128_f",
    "item_128_g",
    "-",
    "-",
    "item_128_h",
    "Prize1",
    "Prize2",
    "Prize3",
    "Prize4",
    "Prize5",
    "Icon04",
    "Mask_Icon01",
    "Mask_Icon02",
    "Mask_Icon03",
    "Mask_item_128_c",
    "Mask_item_128_d",
    "Mask_item_128_f",
    "Mask_item_128_g",
    "Mask_-",
    "Mask_-",
    "Mask_item_128_h",
    "Mask_Prize1",
    "Mask_Prize2",
    "Mask_Prize3",
    "Mask_Prize4",
    "Mask_Prize5",
    "Mask_Icon04",
};
std::array<const d3t::UIImageDefinition *, 32> sharedItemImages{};
struct SharedItemsWidgetState {
    std::uint16_t active{};
    std::uint32_t itemType{};
    std::int32_t owner{-1};
    std::uint32_t published{};
};
SharedItemsWidgetState sharedItemsWidget{};
AuthoredMenuLayout gameMessageLayout{};
std::int32_t gameMessageMainPanel{-1}, gameMessageStartPanel{-1}, gameMessageEndPanel{-1};
std::array<char, 256> gameMessageText{};
std::int32_t gameMessageTransition{};
float gameMessageNextDelay{};
std::int32_t gameMessageMode{};
std::int32_t gameMessageIcon{6};
bool gameMessagePrepared{};
std::byte *guiPtxBacking{};
alignas(4) std::array<std::byte, NOW_QTE_PTX_STORAGE_SIZE> nowQtePtx{};
std::uint32_t nowQteTexture{};
std::array<std::int32_t, 4> ctrlSpeakTexturePoolHandles{}, ctrlSpeakChoiceTextHandles{};
std::array<std::uint32_t, 4> ctrlSpeakTexturePoolFlags{};
HLib::Task *speakIconTask{};
HLib::Task *nowQteTask{};

HLib::Task *mainIconTask{};
HLib::Task *itemSelectTask{};
HLib::Task *itemLookTask{};
HLib::Task *uniqueItemActionTask{};
HLib::Task *moneyIconTask{};
HLib::Task *helpIconTask{};
HLib::Task *itemEffectIconTask{};
ConversationBuffer *itemEffectAudio{};
HLib::Task *sleepActionIconTask{};
ItemSelectorCamera itemSelectorCamera{};
ItemSelectorContext itemSelectorContext{};
bool itemSelectorCameraActive{};
std::uint32_t itemSelectorNavigationInput{};
std::uint16_t itemSelectorDelayedResult{};
std::array<std::uint8_t, 64> itemSelectorOwnedMask{};
const std::uint8_t *itemSelectorPublishedMask{};
std::int32_t mainIconPostFxBank{};
std::uint8_t mainIconPhase{};
std::int16_t itemSelectorLabelTexture{};
alignas(8) ItemSelectorSelection itemSelectorSelection{};
const char *itemSelectorLabel{};

void ClampItemSelectorSelection() {
    ItemSelectorSelection &selection = itemSelectorSelection;
    if (selection.row + 1 > selection.visibleRows && selection.columns * selection.row > selection.count) {
        do {
            --selection.row;
        } while (selection.row >= 0 && selection.columns * selection.row > selection.count);
        if (selection.row < 0)
            selection.row = 0;
    }
    if (selection.column + 1 + selection.columns * selection.row > selection.count) {
        do {
            if (--selection.column < 0) {
                selection.column = selection.columns - 1;
                --selection.row;
            }
        } while (selection.row >= 0 && selection.column + selection.columns * selection.row >= selection.count);
        if (selection.row < 0)
            selection.row = selection.column = 0;
    }
}

void RefreshItemSelectorSelection() {
    ItemSelectorSelection &selection = itemSelectorSelection;
    selection.quantitySum = 0;
    for (const std::byte stock : S2Save_GetInventoryBank(S2InventoryBank::Type0))
        selection.quantitySum += std::to_integer<std::uint8_t>(stock);
    selection.count = static_cast<std::int16_t>(RefreshItemSelectorInventory(0));
    selection.visibleRows = 1;
    selection.columns = 9;
    while (selection.visibleRows * selection.columns < selection.count) {
        if (++selection.visibleRows > 6) {
            selection.visibleRows = 1;
            ++selection.columns;
        }
    }
    ClampItemSelectorSelection();
}

void RefreshItemSelectorSelectedItem() {
    ItemSelectorSelection &selection = itemSelectorSelection;
    selection.selectedItem = selection.selectedIndex = -1;
    std::int32_t ordinal = 0;
    for (std::int32_t index = 0; index < 384; ++index) {
        if (!(itemSelectorOwnedMask[index >> 3] & (1U << (index & 7))))
            continue;
        if (ordinal++ != selection.column + selection.row * selection.columns)
            continue;
        selection.selectedIndex = index;
        selection.selectedItem = static_cast<std::int32_t>(GetInventoryItemId(0, index));
        break;
    }
}

void RefreshItemSelectorLabel() {
    const ItemSelectorSelection &selection = itemSelectorSelection;
    itemSelectorLabelTexture = -1;
    const char *label;
    const std::int32_t menu = selection.activeMenu == 9 && selection.actionResult == 0 ? 9 : ITEM_SELECTOR_MENU_MAP[selection.lowerRow][selection.activeMenu];
    if (menu == 9) {
        itemSelectorLabel = shenGetAmtOf(selection.selectedItem);
        if (!itemSelectorLabel)
            itemSelectorLabel = "";
        label = itemSelectorLabel;
    } else
        label = ITEM_SELECTOR_LABELS[menu];
    itemSelectorLabelTexture = static_cast<std::int16_t>(PublishLegacyItemLabel(label, 512));
}
PadControlEntry *sleepActionPadControl{};
PadControlEntry *sleepActionPreviousPadControl{};
std::int32_t mainIconDelay{5};
std::uint32_t mainIconOverlayActive{};
std::int8_t itemSelectPhase{};
std::uint8_t moneyIconPhase{}, moneyIconDrawMode{}, moneyIconCurrency{};
std::int32_t moneyIconFlags{}, moneyIconTarget{}, moneyIconDisplay{}, moneyIconMaximum{}, moneyIconStepAmount{};
std::int32_t moneyIconFixedStep{}, moneyIconAmount{}, moneyIconResult{}, moneyIconLastIncrement{};
float moneyIconFrames{}, moneyIconSettleFrames{}, moneyIconSelectionScale{};
std::uint16_t moneyIconSavedHelpSelected{}, moneyIconSavedHelpVisible{}, moneyIconSavedHelpPresent{};
MoneyIconPosition moneyIconPosition{}, moneyIconSelectorPosition{};
LegacyIconSprite moneyIconSprite{};
std::array<LegacyIconSprite, 4> moneyIconDirections{};
std::array<LegacyIconSprite, 2> moneyIconTextSprites{}, moneyIconSigns{};
std::uint16_t sleepActionStatus{};
std::int32_t sleepActionSubmenuState{};
std::uint8_t quickIconPhase{}, quickIconUnpressed{};
std::uint16_t quickIconFlags{}, quickIconRequiredStatus{}, quickIconButtonMask{};
std::int16_t quickIconCurrentStatus{}, quickIconTerminalStatus{};
float quickIconMaximumFrames{}, quickIconElapsedFrames{}, quickIconDrawFrames{}, quickIconPreviousDrawFrames{};
std::array<float, 4> quickIconPosition{};
float quickIconPositionDepth{};
std::uint16_t quickIconPositionFlags{};
LegacyIconSprite quickIconSprite{};
std::uint32_t quickIconLegacyMode{}, quickIconLegacyReserved{};
std::int16_t quickIconLegacyTexture{}, quickIconLegacyWidth{};
std::uint32_t quickIconLoadingReleaseFlag{};
std::int32_t itemEffectIconPhase{};
std::int8_t itemEffectIconFrames{};
ItemEffectPosition itemEffectPosition{};
std::int16_t itemEffectLabelTexture{};
std::uint8_t itemEffectColumns{};
LegacyIconSprite itemEffectSprite{};
std::int32_t itemEffectAmount{};
const char *itemEffectName{};
std::int32_t sleepActionFlags{};
std::uint16_t sleepActionPhase{};
std::int8_t sleepActionCurrentSelection{4}, sleepActionTargetSelection{4};
std::int32_t sleepActionRetargetFrames{};
std::int16_t sleepActionPublishedSelection{-1};
std::array<float, 4> sleepActionPosition{};
float sleepActionScale{1.0f};
std::array<LegacyIconSprite, 4> sleepActionArrowSprites{}, sleepActionTextSprites{};
std::array<LegacyIconSprite, 4> ctrlSpeakChoiceDrawSprites{};
std::array<std::uint16_t, 3> ctrlSpeakRadialFrameIds{1286, 42, 1543};
const std::uint16_t *ctrlSpeakSelectedFrameIds{};
std::int32_t nativeGuiActive{};
std::array<std::uint32_t, 3> gameUiVisibilityFlags{};
std::array<std::uint32_t, 3> gameUiSecondaryFlags{};
std::int32_t uiSafeAreaHeight = -1;
float uiSafeAreaWidth = std::bit_cast<float>(0xffffffffU);
std::int32_t previousCameraIconMode{};
std::int32_t currentCameraIconMode{};
// The watch view: its compass ring and hand angles, the AM/PM text its TIME label shows, and the
// CompassImage widget with the CompassPanel window whose alpha fades the ring in and out.
d3t::UIImageWidget *gameHudCompassImage{};
d3t::UIWindow *gameHudCompassWindow{};
float gameHudCompassRotation{};
float gameHudMinuteHandRotation{};
float gameHudHourHandRotation{};
std::array<char, 0x200> gameHudWatchTimeText{};
std::int8_t easyOptionControllerState{};
std::int8_t easyOptionMode{};
bool easyOptionShowsOptions{};
bool easyOptionPreviousClockSuppression{};
float easyOptionPosition{};
float easyOptionVelocity{};
PadControlEntry *easyOptionPadControl{};
PadControlEntry *easyOptionPreviousPadControl{};
HLib::Task *easyOptionTask{};
std::uint16_t easyOptionSavedHelpVisible{};
std::uint8_t easyOptionSavedHelpSelected{};
std::array<std::uint8_t, 3> easyOptionDraft{};
std::array<std::int16_t, 2> easyOptionDescriptionTextures{};
std::array<std::int16_t, 15> easyOptionLabelTextures{};
EasyOptionCaption easyOptionValueCaption{.first = 7, .second = -1, .size = 2, .x = 300, .y = 0, .condition = 0, .value = 0};
EasyOptionCaption easyOptionDescriptionCaption{.first = 0, .second = -1, .size = 0, .x = 184, .y = 280, .condition = 1, .value = 0};
LegacyIconSprite easyOptionTextSprite{}, easyOptionButtonSprite{};
std::int16_t easyOptionPulse{}, easyOptionPulseStep{};
static_assert(sizeof(PosePreviewRecord) == 0x28);
static_assert(offsetof(PosePreviewRecord, characterName) == 0x10);
static_assert(offsetof(PosePreviewRecord, state) == 0x20);
std::array<PosePreviewRecord, 8> posePreviewRecords{};
std::uint8_t easyOptionSelectedRow{};
std::uint8_t easyOptionPreviousValue{};
std::uint32_t easyOptionPreviousNpcState{};

void SetGameUiFlag(std::array<std::uint32_t, 3> &flags, std::size_t index, bool enabled) noexcept {
    if (index >= g_uiElements.size())
        return;
    const std::uint32_t mask = 1U << static_cast<std::uint32_t>(index & 31U);
    std::uint32_t &word = flags[index >> 5U];
    if (enabled)
        word |= mask;
    else
        word &= ~mask;
}
std::byte *scenarioItemPtx{};
std::uint32_t commonItemTexture{};
std::uint32_t scenarioItemTexture{};

} // namespace


std::span<const std::byte> SetupPtxTexture(std::uint16_t width, std::uint32_t idLow, std::uint32_t idHigh) {
    std::memset(guiPtxBacking, 0, guiPtxBackingSize);
    ParseGuiPtx(guiPtxBacking, width, 16, idLow, idHigh);
    return {guiPtxBacking, guiPtxPrefixSize + width * 16U * 4U};
}

namespace ui_detail {

void ReleaseSpeakTextSlots() {
    for (std::int32_t &handle : ctrlSpeakChoiceTextHandles) {
        ReleaseCtrlSpeakTextHandle(handle);
        handle = -1;
    }
}

std::int8_t ctrlSpeakIconState{};
bool ctrlSpeakOpenRequested{}, ctrlSpeakHasSpecialLabel{}, ctrlSpeakForceTextLabels{};
std::int32_t ctrlSpeakMarkerGlyphIndex{}, ctrlSpeakDedicatedCancelEnabled{};
std::array<std::int16_t, 4> ctrlSpeakChoiceIds{}, ctrlSpeakChoiceWidths{}, ctrlSpeakTargetChoiceWidths{}, ctrlSpeakChoiceWidthDeltas{};
std::uint16_t ctrlSpeakMode{}, ctrlSpeakDefaultChoiceId{}, ctrlSpeakResultChoiceId{}, ctrlSpeakSelectedChoiceSlot{};
std::int16_t ctrlSpeakElapsedFrames{}, ctrlSpeakTimeoutFrames{};
std::int32_t ctrlSpeakSelectionAnimationFrame{};
std::uint32_t ctrlSpeakPulseAngle{}, ctrlSpeakSavedLoadingFlags{};
std::array<float, 4> ctrlSpeakSelectorOrigin{};
float ctrlSpeakSpecialLabelWidthScale{}, ctrlSpeakHalfWidthExpansion{}, ctrlSpeakLeftOriginX{}, ctrlSpeakRightOriginX{};
float ctrlSpeakResizeFrame{}, ctrlSpeakSelectorScale{}, ctrlSpeakRadialScale{}, ctrlSpeakRetainedMotion{};
bool dialogueSelectionTimedOut{};

HelpIconState helpIconState{};
std::uint32_t helpIconDeferredRemoval{}, iconSelectFlags{}, helpIconPreviousLoadingFlags{}, helpIconPublishedVisible{};
std::int32_t helpIconItemEffectState{}, helpIconItemEffectFrames{}, helpIconItemPromptFrames{}, helpIconBlinkFrames{}, helpIconBlinkVisible{};
std::array<HelpDirectionBlink, 4> helpDirectionBlinks{};
float currentIconDeltaTime{};
std::int16_t iconDeltaTime{};
std::uint32_t helpIconDrawVisible{}, helpIconDrawSelected{}, qteForcedHelp{};
std::int32_t helpIconHudDelay{}, helpIconRefreshDelay{}, qteCameraAtTop{};
std::array<std::int16_t, 4> helpIconHintIds{};
std::array<std::uint16_t, 4> helpIconHintSelected{};
std::uint32_t helpIconHintAngle{};
std::array<std::uint32_t, 4> helpIconHintAngles{};
std::int32_t qteControlState{};
std::uint32_t qteControlMask{};
std::int32_t nowQteDrawSuppression{};
std::int32_t qteDrawFrames{};
std::uint64_t notebookRefreshFrame{};

bool IsDialogueSelectionSlotAvailable(std::size_t slot) {
    if (slot >= dialogueSelectionPrompt.flagIds.size())
        return false;
    const std::uint16_t flagId = dialogueSelectionPrompt.flagIds[slot];
    const bool validId = flagId != 0xffff && flagId != 470 && flagId < 486;
    return validId && (!dialogueSelectionPrompt.textAvailable || !dialogueSelectionPrompt.labels[slot].empty());
}

std::int32_t FirstDialogueSelectionSlot() {
    for (std::size_t slot = 0; slot < dialogueSelectionPrompt.flagIds.size(); ++slot) {
        if (IsDialogueSelectionSlotAvailable(slot))
            return static_cast<std::int32_t>(slot);
    }
    return -1;
}

std::uint32_t gameScreenFadeColour{};
nk_panel screenFadePanel{};
std::array<nk_panel, 2> cinemaBarPanels{};
float cinemaBarAmount{};
float cinemaBarDirection{};
std::int32_t cinemaBarMode{};
std::array<d3t::UIWindow *, 5> subtitlePanels{};
std::array<d3t::UIImageWidget *, 2> subtitleBackgrounds{};
std::array<std::array<std::int32_t, 3>, 3> subtitleNativeLabels{};
std::array<std::uint32_t, 20> subtitleNativeVoicePalettes{};
std::int32_t subtitleActivityTransition{};
std::array<std::int32_t, 2> subtitleAlphaBindings{};

std::int64_t RenderCinemaBars();
std::int64_t RenderScreenFilm();
std::int64_t ShowCinemaBars() {
    cinemaBarDirection = 1.0f;
    return 0;
}
std::int64_t CloseCinemaBars() {
    cinemaBarDirection = -1.0f;
    return 1;
}
std::int64_t CinemaBarsAreActive() {
    return cinemaBarAmount > 0.0f;
}
std::int64_t ScreenFadeAcceptsInput() {
    return 1;
}
// ST fade quads (shenSubmitQuad, full 640x480 at z 11) in submission order.
std::array<std::uint32_t, 64> screenColourOverlays{};
std::size_t screenColourOverlayCount = 0;
std::uint32_t screenBlackTexture = 0;

constexpr std::array<SplashImageDefinition, 6> splashImages{{
    {"/ui/Splash/Textures/jp_cesa.png?usage=0", nullptr, 0.2f, 5.0f, 0.4f, 3.0f},
    {"/ui/Splash/Textures/jp_photo.png?usage=0", nullptr, 0.2f, 3.0f, 0.4f, 0.0f},
    {"/ui/Splash/Textures/segalogo_01.png?usage=0", "/misc/SegaLogo.wav", 0.1f, 5.0f, 0.4f, 5.0f},
    {"/ui/Splash/Textures/d3tlogo.png?usage=0", nullptr, 0.2f, 5.0f, 0.4f, 2.5f},
    {"/ui/Splash/Textures/legal.png?usage=0", nullptr, 0.2f, 3.0f, 0.4f, 0.0f},
    {"/ui/Splash/Textures/legal_02.png?usage=0", nullptr, 0.2f, 3.0f, 0.4f, 0.0f},
}};

std::array<SplashRuntime, 6> splashRuntime{};
d3t::UIWindow *splashWindow{};
nk_panel splashPanel{};
std::int32_t splashScreenMode = 0;
std::uint64_t splashStateStart = 0;
bool splashSkipPressed = false;

void DrainUIFontRequests() {
    while (d3t::d3t_IsRenderHandlePending(uiFontRenderHandle))
        d3t::ProcessRemappings();
    uiFontRenderHandle = 0;
}

// @verified - p
std::int32_t LoadUIFile(const char *path, std::int32_t elementIndex, std::uint32_t *request) {
    *request = QueueUIFileLoad(GetUIFilePath(path), elementIndex, previousRenderHandle);
    return 1;
}
void UIFileLoaded(void *data, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *) {
    UIElementState *const element = static_cast<UIElementState *>(data);
    if (!element || element->index < 0 || element->index >= static_cast<std::int32_t>(UIElement::Count))
        return;
    if (!loadedData || !loadedSize) {
        if (element->index == static_cast<std::int32_t>(UIElement::LoadSave))
            loadSaveDefinitionFailed.store(true, std::memory_order_release);
        return;
    }
    std::vector<std::byte> &bytes = uiDefinitionData[static_cast<std::size_t>(element->index)];
    bytes.resize(static_cast<std::size_t>(loadedSize) + 1);
    std::memcpy(bytes.data(), loadedData, static_cast<std::size_t>(loadedSize));
    bytes.back() = std::byte{};
    uiDefinitionReady[static_cast<std::size_t>(element->index)].store(true, std::memory_order_release);
}

void ParseLoadedUIDefinition(UIElementState &element) {
    const std::size_t index = static_cast<std::size_t>(element.index);
    const std::vector<std::byte> &bytes = uiDefinitionData[index];
    LoadUIFromMemoryIntoViewSlot(reinterpret_cast<const char *>(bytes.data()), element.config.uiHandleIndex);
    if (element.config.uiHandleIndex == -1) {
        element.config.init = 2;
        return;
    }
    d3t::UIDefinition &definition = uiDefinitions[index];

    const bool parsed = definition.parse(bytes.data(), bytes.size() - 1, true) || definition.document;
    element.config.init = 2;
    if (!parsed) {
        if (index == static_cast<std::size_t>(UIElement::LoadSave))
            loadSaveDefinitionFailed.store(true, std::memory_order_release);
        return;
    }
    if (index == static_cast<std::size_t>(UIElement::LoadSave))
        loadSaveDefinitionFailed.store(false, std::memory_order_release);
    auto &parentLayout = parentMenuLayouts[index];
    parentLayout.parents.assign(definition.panels.size(), -1);
    parentLayout.panels.resize(definition.panels.size());
    for (std::size_t panel = 0; panel < definition.panels.size(); ++panel)
        for (std::size_t parent = 0; parent < definition.panels.size(); ++parent)
            if (definition.panels[parent].id == definition.panels[panel].parentId) {
                parentLayout.parents[panel] = static_cast<std::int32_t>(parent);
                break;
            }
    if (index == static_cast<std::size_t>(UIElement::CutsceneSkip))
        BindAuthoredMenuLayout(definition, cutsceneSkipLayout);
}

bool AreUIDefinitionImagesReady(const UIElementState &element) {
    const std::uint32_t slotIndex = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    if (slotIndex >= d3t::kUIViewSlotCount || TryFinalizeUIViewTextures(static_cast<std::int32_t>(slotIndex)) != 1)
        return false;
    const d3t::UIViewSlot<80> &slot = g_uiViewSlots[slotIndex];
    for (d3t::UIImageDefinition &image : uiDefinitions[static_cast<std::size_t>(element.index)].images) {
        const std::int32_t index = UI_FindViewImage(slotIndex, image.name.c_str());
        if (index != -1)
            image.textureHandle = static_cast<std::uint32_t>(slot.images[static_cast<std::size_t>(index)].textureHandle);
    }
    return true;
}

std::uint32_t ReadUIDefinitionFile(UIElement elementIndex, const char *path, std::int32_t flags) {
    const std::size_t index = static_cast<std::size_t>(elementIndex);
    if (index >= g_uiElements.size() || !path) {
        return 0;
    }
    UIElementState &element = g_uiElements[index];
    if (!element.setup) {
        element.someFlag3 = flags;
        element.setup = 1;
        const std::size_t pathLength = std::min(std::strlen(path), sizeof(element.uiConfigFilepath) - 1);
        std::memcpy(element.uiConfigFilepath, path, pathLength);
        element.uiConfigFilepath[pathLength] = '\0';
        if ((flags & 1) && previousRenderHandle)
            d3t::d3t_AddRenderHandleReference(previousRenderHandle);
        const d3t::D3TShaderLoadTask task{
            path,
            previousRenderHandle,
            0,
            {UIFileLoaded, &element},
            nullptr,
            0,
            0,
            {},
        };
        const std::uint32_t handle = static_cast<std::uint32_t>(d3t::PushShaderLoadToThreadQueue(&task));
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}

std::uint32_t LoadIntroCutsceneCredits() {
    return ReadUIDefinitionFile(UIElement::IntroCutsceneCredits, "/ui/introcutscene/IntroCutscene.ui", 0);
}

std::uint32_t LoadGameQte() {
    return ReadUIDefinitionFile(UIElement::GameQte, "/ui/gameqte/GameQTE.ui", 0);
}

std::int64_t BindGameQte() {
    SetupBattleQteWidgets();
    return 1;
}

std::int64_t GameQteInputNullsub() {
    return 0;
}

std::int64_t RenderGameQte() {
    if (!drawingUI)
        return 0;
    UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::GameQte)];
    if (element.config.init != 3)
        LoadGameQte();
    if (element.config.init == 2 || element.config.init == 3)
        RenderBattleQteWidgets();
    return 0;
}

std::intptr_t DrawGameQteFrame() {
    RenderGameQte();
    Nuklear_BindRenderTarget();
    NuklearUpdateForAspectRatio(GetAspectRatioModeShouldScale());
    CopyNuklearTexToBackBuffer();
    return 0;
}

void RenderGameQteFrame() {
    if (!drawingUI)
        return;
#if defined(_WIN32)
    ExecuteCurrentCommandList();
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(1);
    const NuklearFrameCallback previous = g_nuklearFrameCallback;
    g_nuklearFrameCallback = DrawGameQteFrame;
    InvokeNuklearFrame(d3t::DX11::g_ppD3DDeviceContext);
    FinishNuklearFrame();
    g_nuklearFrameCallback = previous;
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    d3t::BindUIFrameRenderTarget();
#endif
}

std::uint32_t LoadEndingCredits() {
    return ReadUIDefinitionFile(UIElement::Credits, "/ui/Credits/Credits.ui", 0);
}

std::uint32_t LoadLoadingScreenDefinition() {
    return ReadUIDefinitionFile(UIElement::LoadingScreen, "/ui/LoadScreen/LoadScreen.ui", 0);
}

void ResetLoadingLocationStateNoOp() {
}
void PrepareLoadingLocationStateNoOp() {
}

std::string ResolveLoadingAreaLabel(std::string_view key) {
    const auto found =
        std::find_if(loadingAreaLabelEntries.begin(), loadingAreaLabelEntries.end(), [key](const LoadingAreaLabelEntry &entry) { return entry.key == key; });
    if (found == loadingAreaLabelEntries.end())
        return {};
    return std::string(GetLocalizedSceneText(found->label));
}

const LoadingPreviousAreaEntry *FindPreviousLoadingArea(shen_name area, std::int32_t entry) {
    const auto found = std::find_if(loadingPreviousAreaEntries.begin(), loadingPreviousAreaEntries.end(),
        [area, entry](const LoadingPreviousAreaEntry &candidate) { return candidate.area.name == area.name && candidate.entry == entry; });
    return found == loadingPreviousAreaEntries.end() ? nullptr : &*found;
}

void ReleaseLoadingHeader(std::size_t header) {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::LoadingScreen)].config.uiHandleIndex;
    d3t::UIViewSlot<80> *const view = GetNuklearElementByIdx(slot);
    const std::int32_t index = loadingScreen.images[header == 0 ? 0 : 2];
    if (!view || index < 0)
        return;
    d3t::UIImageWidget &widget = g_uiImageWidgets[index];
    d3t::UIViewImage &image = view->images[widget.image];
    if (image.ready != 1)
        return;
    const std::uint32_t texture = GetShaderDataByIndex(loadingScreen.headerRequests[header]);
    if (texture) {
        d3t::InitTexture2DResource(texture);
        loadingScreen.headerRequests[header] = 0;
    }
    image.textureHandle = 0;
    image.ready = 0;
    widget.field44 = 0;
}

void PublishLoadingHeader(std::int32_t widgetIndex, std::uint32_t request, std::uint32_t renderHandle) {
    if (d3t::d3t_IsRenderHandlePending(renderHandle))
        return;
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::LoadingScreen)].config.uiHandleIndex;
    d3t::UIViewSlot<80> *const view = GetNuklearElementByIdx(slot);
    if (!view || widgetIndex < 0)
        return;
    d3t::UIImageWidget &widget = g_uiImageWidgets[widgetIndex];
    const std::uint32_t texture = GetShaderDataByIndex(request);
    if (!texture)
        return;
    std::uint32_t width{}, height{};
    if (d3t::GetTexture2DDimensions(texture, &width, &height)) {
        d3t::UIViewImage &image = view->images[widget.image];
        image.widthScale = static_cast<float>(width) / 1920.0f;
        image.heightScale = static_cast<float>(height) / 1080.0f;
        image.textureHandle = static_cast<std::int32_t>(texture);
        image.width = static_cast<std::uint16_t>(width);
        image.height = static_cast<std::uint16_t>(height);
        image.region = {0, 0, image.width, image.height};
        image.ready = 1;
        widget.field44 = 1;
    }
    d3t::d3t_CloseRenderHandle(renderHandle);
}

void UpdateLoadingScreenLabels() {
    for (std::size_t index = 0; index < loadingScreen.labels.size(); ++index) {
        const std::int32_t label = loadingScreen.labels[index];
        if (label >= 0)
            g_uiLabelWidgets[label].text = loadingScreen.text[index].c_str();
    }
}

std::int64_t BindLoadingScreenDefinition() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::LoadingScreen)].config.uiHandleIndex;
    d3t::UIViewSlot<80> *const view = GetNuklearElementByIdx(slot);
    if (!view)
        return 1;
    view->keepLoaded = 1;
    for (std::size_t index = 0; index < loadingScreen.images.size(); ++index)
        loadingScreen.images[index] = UI_FindViewImageWidget(slot, LOADING_IMAGE_WIDGETS[index]);
    for (std::size_t index = 0; index < loadingScreen.windows.size(); ++index)
        loadingScreen.windows[index] = UI_FindViewWindow(slot, LOADING_WINDOWS[index]);
    loadingScreen.labels.fill(-1);
    for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
        if (view->widgetKinds[widget] != 15)
            continue;
        const std::int32_t label = view->widgetPoolIndices[widget];
        for (std::size_t index = 0; index < LOADING_LABEL_TOKENS.size(); ++index) {
            if (g_uiLabelWidgets[label].text && !std::strcmp(g_uiLabelWidgets[label].text, LOADING_LABEL_TOKENS[index])) {
                loadingScreen.labels[index] = label;
                break;
            }
        }
    }
    UpdateLoadingScreenLabels();
    for (std::size_t index = 0; index < loadingScreen.animations.size(); ++index)
        loadingScreen.animations[index] = UIWidget_Bind(loadingScreen.windows[index], 0, 0, {0.0f, 0.0f}, {1.0f, 1.0f});
    loadingScreen.labelsBound = true;
    return 1;
}

std::int64_t RenderLoadingScreenDefinition() {
    if (loadingScreen.collectionPoster) {
        PublishLoadingHeader(loadingScreen.images[1], loadingScreen.headerRequests[0], loadingScreen.renderHandles[0]);
        if (loadingScreen.images[0] >= 0)
            g_uiImageWidgets[loadingScreen.images[0]].field44 = 0;
        if (loadingScreen.images[2] >= 0)
            g_uiImageWidgets[loadingScreen.images[2]].field44 = 0;
    } else {
        PublishLoadingHeader(loadingScreen.images[0], loadingScreen.headerRequests[0], loadingScreen.renderHandles[0]);
        PublishLoadingHeader(loadingScreen.images[2], loadingScreen.headerRequests[1], loadingScreen.renderHandles[1]);
        if (loadingScreen.images[1] >= 0)
            g_uiImageWidgets[loadingScreen.images[1]].field44 = 0;
    }
    const std::size_t phase = static_cast<std::uint32_t>(GetLoadingElapsedSeconds() * 2.0f) % LOADING_DOTS.size();
    std::string &text = loadingScreen.text[static_cast<std::size_t>(LoadingLabelSlot::Loading)];
    text.assign(GetLocalizedSceneText("UILOADNOWL")); // @text EN: "Now Loading" | JA: "ＮＯＷ　ＬＯＡＤＩＮＧ"
    text.append(LOADING_DOTS[phase]);
    UpdateLoadingScreenLabels();
    uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::LoadingScreen)].config.uiHandleIndex);
    return 1;
}

std::int64_t TickLoadingScreenDefinition() {
    DestroyElementByIndex(UIElement::GameHelp);
    DestroyElementByIndex(UIElement::WristwatchOverlay);
    SetupLoadingScreen(shen_name{0xffffffffU}, shen_name{0xffffffffU}, DateTime{}, nullptr, 0, 0, 0, 0);
    DestroyElementByIndex(UIElement::MainMenu);
    DestroyElementByIndex(UIElement::GameMenu);
    for (const LoadingWindow index : LOADING_SHOW_RESET_ORDER) {
        if (d3t::UIWindow *const window = UI_GetWindow(loadingScreen.windows[index]))
            window->alpha = index == LoadingBlackBackground ? 1.0f : 0.0f;
    }
    return 1;
}

std::int64_t DestroyLoadingScreenDefinition() {
    for (const LoadingWindow index : LOADING_BACKGROUNDS) {
        const d3t::UIWindow *const window = UI_GetWindow(loadingScreen.windows[index]);
        if (window && window->alpha > 0.0f)
            UIWidget_Animate(loadingScreen.animations[index], 0, 2.0f, 0.0f);
    }
    const LoadingWindow header = loadingScreen.collectionPoster ? LoadingCollectionPoster : LoadingHeader;
    if (const d3t::UIWindow *const window = UI_GetWindow(loadingScreen.windows[header]); window && window->alpha > 0.0f)
        UIWidget_Animate(loadingScreen.animations[header], 0, 2.0f, 0.0f);
    for (const LoadingWindow index : LOADING_FADE_OUT_ORDER) {
        const d3t::UIWindow *const window = UI_GetWindow(loadingScreen.windows[index]);
        if (window && window->alpha > 0.0f)
            UIWidget_Animate(loadingScreen.animations[index], 0, 2.0f, 0.0f);
    }
    return 1;
}

std::int64_t ResetLoadingScreenDefinition() {
    loadingScreen.windows.fill(-1);
    loadingScreen.animations.fill(-1);
    return 1;
}
std::int64_t BindIntroCutsceneCredits() {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::IntroCutsceneCredits)];
    introCreditSprites.fill(nullptr);
    introLogoPanel = nullptr;
    introLogoImage = nullptr;
    for (d3t::UITextureObjectDefinition &object : definition.textureObjects) {
        if (object.id == "Credit_Left_Panel::Sprite")
            introCreditSprites[0] = &object;
        else if (object.id == "Credit_Right_Panel::Sprite")
            introCreditSprites[1] = &object;
        else if (object.id == "Logo_Panel::Image")
            introLogoImage = &object;
    }
    for (d3t::UITextureObjectDefinition *const sprite : introCreditSprites) {
        if (sprite)
            sprite->colour = 0x00ffffffU;
    }
    for (d3t::UIPanelDefinition &panel : definition.panels) {
        if (panel.id != "Logo_Panel")
            continue;
        introLogoPanel = &panel;
        panel.imageOpacity = 0.0f;
    }
    introObjectRectangles = d3t::GetUITextureObjectRectangles(definition);
    if (introLogoImage) {
        introLogoImage->imageName = g_sceneTextLanguage == 4       ? "Image_Logo_KR" :
            (g_sceneTextLanguage == 5 || g_sceneTextLanguage == 6) ? "Image_Logo_SC" :
                                                                     "Image_Logo";
    }
    return 1;
}

std::int64_t ResetIntroCutsceneCredits() {
    introLogoPanel = nullptr;
    introCreditSprites.fill(nullptr);
    return 1;
}

std::int64_t DrawIntroCutsceneCredits() {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::IntroCutsceneCredits)];
    if (introLogoImage) {
        const char *const image = g_sceneTextLanguage == 4         ? "Image_Logo_KR" :
            (g_sceneTextLanguage == 5 || g_sceneTextLanguage == 6) ? "Image_Logo_SC" :
                                                                     "Image_Logo";
        if (introLogoImage->imageName != image)
            introLogoImage->imageName = image;
    }
    for (d3t::UITextureObjectDefinition *const sprite : introCreditSprites) {
        if (sprite && (sprite->colour >> 24) <= 10)
            sprite->colour &= 0x00ffffffU;
    }
    if (introLogoPanel && introLogoPanel->imageOpacity <= 0.039215688f)
        introLogoPanel->imageOpacity = 0.0f;
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        const d3t::UITextureObjectDefinition &object = definition.textureObjects[index];
        const float opacity = &object == introLogoImage && introLogoPanel ? introLogoPanel->imageOpacity : 1.0f;
        d3t::DrawUITextureObject(definition, object, introObjectRectangles[index], opacity);
    }
    return 1;
}
std::uint32_t LoadSharedItems() {
    return ReadUIDefinitionFile(UIElement::SharedItems, "/ui/New/GameMenu/SharedItems.ui", 1);
}

std::uint32_t LoadGameMessage() {
    return ReadUIDefinitionFile(UIElement::GameMessage, "/ui/gamemessage/GameMessage.ui", 1);
}

std::uint32_t LoadFightGaugeDefinition() {
    return ReadUIDefinitionFile(UIElement::FightGauge, "/ui/gamehud/GameHudGauge.ui", 0);
}

std::int64_t BindFightGaugeUI() {
    BindFightGaugeDefinition(uiDefinitions[static_cast<std::size_t>(UIElement::FightGauge)]);
    return 1;
}

std::uint32_t LoadGameHudWatch() {
    return ReadUIDefinitionFile(UIElement::GameHudWatch, "/ui/gamehud/GameHudWatch.ui", 0);
}

// The TIME label takes the AM/PM text, and the CompassImage widget and its window are kept for
// UpdateCompassImage. Images 1-3 (compass ring, minute and hour hands) turn in RenderGameHudWatch.
// @verified - p
std::int64_t BindGameHudWatch() {
    const std ::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::GameHudWatch)].config.uiHandleIndex;
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    if (view) {
        view->keepLoaded = 1;
        for (std::size_t index = 1; index < 4; ++index) {
            view->images[index].rotate = 1;
            view->images[index].angle = 0.0f;
        }
        for (std::int32_t index = 0; index < view->widgetCount; ++index) {
            if (view->widgetKinds[index] != 15)
                continue;
            d3t::UILabelWidget *const label = UI_GetLabel(view->widgetPoolIndices[index]);
            if (!std ::strcmp(label->text, UI_HUD_WATCH_TIME_MARKER))
                label->text = gameHudWatchTimeText.data();
        }
    }
    const std::int32_t image = UI_FindViewImageWidget(slot, UI_HUD_WATCH_COMPASS_IMAGE);
    gameHudCompassImage = static_cast<std::uint32_t>(image) < g_uiImageWidgets.size() ? &g_uiImageWidgets[image] : nullptr;
    if (gameHudCompassImage)
        gameHudCompassWindow = UI_GetWindow(gameHudCompassImage->window);

    return 1;
}

std::uint32_t LoadGameBackground() {
    return ReadUIDefinitionFile(UIElement::GameBackground, "/ui/gamemenu_New/GameMenu_Background.ui", 0);
}

std::uint32_t LoadMoveLearned() {
    return ReadUIDefinitionFile(UIElement::MoveLearned, "/ui/gamehud/GameHudMoveLearned.ui", 0);
}

// @verified - p
std::int64_t BindMoveLearnedDefinition() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::MoveLearned)].config.uiHandleIndex;
    for (std::size_t index = 0; index < moveLearnedLabels.size(); ++index)
        moveLearnedLabels[index] = UI_GetLabel(UI_FindViewLabel(slot, UI_MOVE_LEARNED_LABELS[index]));

    for (std::size_t index = 0; index < moveLearnedLabels.size(); ++index) {
        if (moveLearnedLabels[index]) {
            moveLearnedLabels[index]->text = g_moveLearnedUiText[index].data();
            moveLearnedLabels[index]->token.alreadyLocalized = 1;
        }
    }
    return 1;
}

// @verified - p
std::int64_t MoveLearnedInput() {
    return 0;
}

// @verified - p
std::int64_t EnableMoveLearnedMenuLayer() {
    EnableRenderableByIndex(UIElement::GameMenu);

    return 1;
}

std::int64_t DisableMoveLearnedMenuLayer() {
    DisableElement(UIElement::GameMenu);
    return 1;
}

// @verified - p
std::int64_t ResetMoveLearnedBindings() {
    moveLearnedLabels.fill(nullptr);

    return 0;
}

std::uint32_t LoadGameMoves() {
    gameMovesRuntime.bound = false;
    return ReadUIDefinitionFile(UIElement::SubMenuMoves, "/ui/gamemenu_New/SubMenu_Moves.ui", 0);
}

void DrawGameMovesRow(UIGrid *, void *, const Vector4 *, const Vector4 *, std::int32_t, std::uint32_t, const std::int32_t *, std::int32_t);
void SelectGameMoveRow(float *, void *);
void ActivateGameMoveVariant(float *, void *);
const std::int32_t *GameMovesSelectionData(UIGrid *, std::int32_t row) {
    return gameMovesRuntime.dictionary.data() + row;
}

std::int64_t SetGameMovesTheme();

std::int64_t BindGameMovesDefinition() {
    GameMovesRuntime &runtime = gameMovesRuntime;
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuMoves)].config.uiHandleIndex;
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    for (std::size_t index = 0; index < runtime.windows.size(); ++index)
        runtime.windows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_MOVES_WINDOWS[index]));
    for (std::size_t index = 0; index < runtime.labels.size(); ++index) {
        const std::int32_t label = UI_FindViewLabel(slot, UI_MOVES_LABELS[index]);
        runtime.labels[index] = label >= 0 ? &g_uiLabelWidgets[label] : nullptr;
    }
    for (std::size_t index = 3; index < 5; ++index)
        if (runtime.labels[index])
            runtime.labels[index]->text = runtime.labelText[index - 3].data();
    for (std::size_t index = 0; index < runtime.images.size(); ++index) {
        const std::int32_t image = UI_FindViewColourImage(slot, UI_MOVES_IMAGES[index]);
        runtime.images[index] = image >= 0 ? &g_uiColourImageWidgets[image] : nullptr;
    }
    for (std::size_t index = 0; index < runtime.layoutItems.size(); ++index) {
        runtime.layoutItems[index] = nullptr;
        if (!view)
            continue;
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 13)
                continue;
            d3t::UILayoutItem &item = g_uiLayoutItems[view->widgetPoolIndices[widget]];
            if (!std::strcmp(item.id.data(), UI_MOVES_ROW_LAYOUTS[index])) {
                runtime.layoutItems[index] = &item;
                break;
            }
        }
    }
    for (std::size_t index = 0; index < runtime.icons.size(); ++index)
        runtime.icons[index] = UI_FindViewIconWidget(slot, UI_MOVES_ICONS[index], 21);
    runtime.critical = UI_FindViewIconWidget(slot, "GameMoves::DummyMoveBar_Crit", 20);
    for (std::size_t index = 0; index < runtime.buttons.size(); ++index) {
        runtime.buttons[index] = nullptr;
        if (!view)
            continue;
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 17)
                continue;
            UIButtonWidget &button = g_uiButtonWidgets[view->widgetPoolIndices[widget]];
            if (!std::strcmp(button.id.data(), UI_MOVES_COLOUR_BUTTONS[index])) {
                runtime.buttons[index] = &button;
                break;
            }
        }
    }
    runtime.criticalTransition = UIAnimAllocSlot();
    runtime.criticalFrame = 0;
    UIAnim_Start(runtime.criticalTransition, 0.0f, 1.0f, 0.001f, 0.050000001f);
    runtime.arrowTransition = UIAnimAllocSlot();
    g_uiAnimSlots[static_cast<std::size_t>(runtime.arrowTransition)].loopMode = 2;
    UIAnim_Start(runtime.arrowTransition, 1.0f, 0.15000001f, 0.30000001f, 0.0f);
    if (runtime.images[3] && view) {
        d3t::UIViewImage &image = view->images[runtime.images[3]->image];
        CreateTexture2DForNuklear(image.width, image.height);
        BindUISceneImage(0, &image);
    }
    InitializeUIGrid(runtime.grid,
        {.columns = 1,
            .visibleRows = 8,
            .rows = 59,
            .panel = runtime.windows[1],
            .drawRow = DrawGameMovesRow,
            .actions = {SelectGameMoveRow, nullptr, nullptr, ActivateGameMoveVariant},
            .selectionData = GameMovesSelectionData,
            .sound = 5});
    SetUIGridDictionary(runtime.grid, 59, runtime.dictionary.data());
    runtime.cellTemplate = {.viewSlot = static_cast<std::int32_t>(slot),
        .first = 5,
        .end = 23,
        .clipWindows = {7, -1},
        .windows = runtime.windows.data(),
        .ids = UI_MOVES_WINDOWS.data(),
        .layoutFirst = 0,
        .layoutEnd = 25,
        .layoutItems = runtime.layoutItems.data(),
        .labelFirst = 3,
        .labelEnd = 5,
        .labels = runtime.labels.data()};
    runtime.bound = true;
    return 1;
}

void GameMovesSceneCallback() {
    UpdateWazamakiPreviewFrame();
    DrawWazamakiPreview();
}

std::int64_t DestroyGameMoves() {
    using enum CameraState;
    if (!g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isVisible) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (control && control->ryo) {
            Character *const player = static_cast<Character *>(control->ryo);
            EV_SetCharDisplay(player, CVM_DisplayEnabled, 1);
            charaToggleFlag(player, 1);
        }
        camCtrlSetState(Init);
    }
    SetUISceneCallback(nullptr);
    ReleaseWazamakiPreviewModels();
    gameMovesRuntime.previewLoaded = false;
    return 1;
}

std::int64_t RenderGameMoves() {
    GameMovesRuntime &runtime = gameMovesRuntime;
    if (!runtime.bound)
        return 0;
    if (!runtime.previewLoaded && g_waitingForNextTick == 1) {
        runtime.previewLoaded = true;
        if (BeginUISceneRenderQueue()) {
            LoadRyoSceneForMovesScreen();
            ExecuteCurrentCommandList();
        }
    }
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuMoves)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    RenderNativeUIGrid(runtime.grid, element.index, uiPointer);
    if (g_uiAnimSlots[static_cast<std::size_t>(runtime.criticalTransition)].status == 1) {
        if (++runtime.criticalFrame > 7)
            runtime.criticalFrame = 0;
        UIAnim_Start(runtime.criticalTransition, 0.0f, 1.0f, 0.001f, 0.050000001f);
    }
    return 0;
}

const WazamakiMoveInfo *GetGameMove(std::int32_t row) {
    const std::span<const WazamakiLearnedGroup> groups = GetWazamakiLearnedGroups();
    if (row < 0 || static_cast<std::size_t>(row) >= groups.size())
        return nullptr;
    const WazamakiLearnedGroup &group = groups[static_cast<std::size_t>(row)];
    return &GetWazamakiMoveCategory(group.category)[group.moves[group.selected]];
}

void SetGameMoveStats(std::int32_t row, bool minimum) {
    const WazamakiMoveInfo *const move = GetGameMove(row);
    if (!move)
        return;
    const std::array<float, 3> stats{
        static_cast<float>(move->stats[0]) / 90.0f, 1.0f - static_cast<float>(move->stats[1]) / 28.0f, static_cast<float>(move->stats[2]) / 40.0f};
    for (std::size_t index = 0; index < stats.size(); ++index) {
        float value = stats[index];
        if (minimum && value < 0.12f)
            value = value > 0.0f ? 0.12f : 0.0f;
        if (gameMovesRuntime.windows[index + 2])
            gameMovesRuntime.windows[index + 2]->layout.width.scale = value;
    }
}

void SelectGameMoveRow(float *bounds, void *data) {
    UIMessageBox message = GetCurrentUIMessageBox();
    message.imageIndex = message.widgetIndex = -1;
    if (bounds)
        std::memcpy(&message.bounds, bounds, sizeof(message.bounds));
    message.primaryLocalized = message.hideTarget = 0;
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    if (row) {
        PlayWazamakiGroupMove(static_cast<std::uint32_t>(*row));
        SetGameMoveStats(*row, true);
        gameMovesRuntime.selectedMove = *row;
        const WazamakiMoveInfo *const move = GetGameMove(*row);
        if (move)
            message.primaryText = move->textToken.data();
    }
    CopyUIMessageBox(&message);
}

bool ChangeGameMoveVariant(std::int32_t row, bool left, bool right) {
    const std::span<const WazamakiLearnedGroup> groups = GetWazamakiLearnedGroups();
    if (row < 0 || static_cast<std::size_t>(row) >= groups.size())
        return false;
    const WazamakiLearnedGroup &group = groups[row];
    if (group.count <= 1)
        return false;
    if (left) {
        SelectWazamakiMove(group.category, group.moves[(group.count + group.selected - 1U) % group.count]);
        PlayWazamakiGroupMove(static_cast<std::uint32_t>(row));
        AicaExecuteWrapper(6);
        SelectGameMoveRow(nullptr, &row);
    }
    if (right) {
        SelectWazamakiMove(group.category, group.moves[(group.count + group.selected + 1U) % group.count]);
        PlayWazamakiGroupMove(static_cast<std::uint32_t>(row));
        AicaExecuteWrapper(6);
        SelectGameMoveRow(nullptr, &row);
    }
    return left || right;
}

void ActivateGameMoveVariant(float *bounds, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    if (row && ChangeGameMoveVariant(*row, false, true))
        SelectGameMoveRow(bounds, data);
}

void MovesConfirmNullsub(std::int32_t) {
}

std::int64_t PressGameMoves() {
    if (!gameMovesRuntime.bound)
        return 1;
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const std::int32_t row = GetUIGridSelection(gameMovesRuntime.grid);
    ChangeGameMoveVariant(row, (pressed & 0x4000U) != 0, (pressed & 0x8000U) != 0);
    if (pressed & 0x400U)
        MovesConfirmNullsub(row);
    if (!IsCameraIconHidden() && (pressed & 0x200U)) {
        DestroyElementByIndex(UIElement::SubMenuMoves);
        DisableElement(UIElement::ShenmueCollection);
        return 0;
    }
    if (pressed & 4U)
        SetCurrentHelpString(0x33);
    return ProcessUIGridInput(gameMovesRuntime.grid, uiPointer, GetFrameTime());
}
std::int64_t GameMovesHints() {
    const std::int32_t selection = GetUIGridSelection(gameMovesRuntime.grid);
    const std::span<const WazamakiLearnedGroup> groups = GetWazamakiLearnedGroups();
    const std::int32_t row = selection >= 0 && selection < gameMovesRuntime.grid.dictionaryCount ? gameMovesRuntime.dictionary[selection] : -1;
    const bool variants = row >= 0 && static_cast<std::size_t>(row) < groups.size() && groups[row].count > 1;
    const std::array<std::int16_t, 5> hints{static_cast<std::int16_t>(variants ? 45 : -1), 44, 58, -1, -1};
    return CopyHudHintIds(hints);
}

std::int64_t SetGameMovesTheme() {
    GameMovesRuntime &runtime = gameMovesRuntime;
    const bool game = IsCameraIconHidden() != 0;
    if (runtime.windows[0]) {
        std::snprintf(runtime.windows[0]->layout.parentId.data(), runtime.windows[0]->layout.parentId.size(), "%s",
            game ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
    }
    for (std::size_t index = 0; index < runtime.buttons.size(); ++index)
        if (runtime.buttons[index])
            runtime.buttons[index]->colour = game ? UI_MOVES_GAME_COLOURS[index] : UI_MOVES_FRONT_COLOURS[index];
    if (runtime.images[4])
        runtime.images[4]->colour = game ? 0x37ffffffU : 0xe13c3778U;
    if (runtime.images[1])
        runtime.images[1]->alpha = game ? 0.0f : 1.0f;
    if (runtime.images[0])
        runtime.images[0]->alpha = game ? 1.0f : 0.0f;
    if (runtime.images[2])
        runtime.images[2]->alpha = game ? 1.0f : 0.0f;
    return 0;
}

std::int64_t ResetGameMoves() {
    gameMovesRuntime.windows.fill(nullptr);
    gameMovesRuntime.layoutItems.fill(nullptr);
    gameMovesRuntime.labels.fill(nullptr);
    gameMovesRuntime.images.fill(nullptr);
    gameMovesRuntime.bound = false;
    return 0;
}

std::int64_t TickGameMoves() {
    ShowElementByIndex(UIElement::GameInfoBar);
    if (!gameMovesRuntime.bound)
        return 0;
    GameMovesRuntime &runtime = gameMovesRuntime;
    RefreshWazamakiLearnedGroups();
    const std::span<const WazamakiLearnedGroup> groups = GetWazamakiLearnedGroups();
    std::int32_t count = 0;
    for (std::int32_t row = 0; row < 59; ++row)
        if (static_cast<std::size_t>(row) < groups.size())
            runtime.dictionary[count++] = row;
    SetUIGridDictionary(runtime.grid, count, runtime.dictionary.data());
    SetUIGridTabSelector(nullptr, 1);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view =
        GetNuklearElementByIdx(g_uiElements[static_cast<std::size_t>(UIElement::SubMenuMoves)].config.uiHandleIndex);
    if (view) {
        for (std::size_t index = 0; index < 3; ++index)
            if (runtime.labels[index])
                view->variables[runtime.labels[index]->scaleVar].value.floatValue = 1.0f;
        if (runtime.images[3])
            BindUISceneImage(0, &view->images[runtime.images[3]->image]);
    }
    SetUISceneCallback(runtime.images[3] ? GameMovesSceneCallback : nullptr);
    if (runtime.windows[2])
        SetGameMoveStats(0, false);
    ResetUIGrid(runtime.grid);
    SetUIGridTabSelector(nullptr, 1);
    return 0;
}

std::array<float, 2> GetUILayoutPaddingScale() {
    const float width = GetUIViewportWidth();
    const float scale = GetS2UserInterfaceSafeArea() * (width < 1920.0f ? width / 1920.0f : 1.0f);
    return {scale * 1920.0f / width, scale};
}

std::int32_t uiLabelSplitActive{};

std::int32_t FindUILabelSplit(std::string_view text) {
    std::int32_t split = -1, distance = 0x7fffffff;
    const std::int32_t middle = static_cast<std::int32_t>(text.size()) / 2;
    for (std::size_t index = 0; index < text.size(); ++index) {
        if (text[index] != ' ')
            continue;
        std::int32_t current = static_cast<std::int32_t>(index) - middle;
        if (current < 0)
            current = -current;
        if (current < distance) {
            split = static_cast<std::int32_t>(index);
            distance = current;
        }
    }
    return split;
}

float FitUILabelText(
    const UIFontSelection &font, std::string_view text, const d3t::UIRect &bounds, float referenceWidth, float referenceHeight, std::int32_t &split) {
    float scale = 0.33000001f;
    split = -1;
    if (referenceWidth > bounds.width && referenceWidth != -1.0f)
        scale = bounds.width / referenceWidth;
    if (referenceHeight <= bounds.height || referenceHeight == -1.0f)
        scale = 1.0f;
    else if (bounds.height / referenceHeight > scale)
        scale = bounds.height / referenceHeight;
    const float height = GetUIFontHeight(font);
    const float measuredWidth = GetUIFontTextWidth(font, height * scale, text);
    const float measuredHeight = height * scale;
    if (measuredWidth > bounds.width && referenceWidth != -1.0f)
        scale *= bounds.width / measuredWidth;
    if (measuredHeight > bounds.height && referenceHeight != -1.0f && scale > bounds.height / measuredHeight)
        scale *= bounds.height / measuredHeight;
    if (scale < 0.33000001f)
        scale = 0.33000001f;
    if (scale < 0.5f && !uiLabelSplitActive) {
        uiLabelSplitActive = 1;
        split = FindUILabelSplit(text);
        if (split >= 0) {
            d3t::UIRect half = bounds;
            half.height *= 0.5f;
            std::int32_t ignored;
            scale = FitUILabelText(font, text.substr(0, static_cast<std::size_t>(split)), half, referenceWidth, referenceHeight * 0.5f, ignored);
            half.y += half.height;
            const float second = FitUILabelText(font, text.substr(static_cast<std::size_t>(split) + 1), half, referenceWidth, referenceHeight * 0.5f, ignored);
            if (scale > second)
                scale = second;
        }
        uiLabelSplitActive = 0;
    }
    return scale;
}

// @verified - p
std::int32_t CompareUIStringsCaseInsensitive(const char *first, const char *second) {
    if (!first || !second)
        return 0;
    for (;;) {
        const char left = static_cast<char>(std::tolower(*first++));
        const char right = static_cast<char>(std::tolower(*second++));
        if (left != right)
            return left - right;
        if (!left)
            return 0;
    }
}

UIFontSelection GetUILabelFont(const d3t::UILabelDefinition &label) {
    std::uint32_t size = 4;
    for (std::size_t index = 0; index < UI_FONT_SIZE_NAMES.size(); ++index)
        if (!CompareUIStringsCaseInsensitive(label.fontSize.c_str(), UI_FONT_SIZE_NAMES[index].data())) {
            size = static_cast<std::uint32_t>(index);
            break;
        }
    const std::uint32_t family = !CompareUIStringsCaseInsensitive(label.fontName.c_str(), "UIStandardFontBold") ? 1U :
        !CompareUIStringsCaseInsensitive(label.fontName.c_str(), "UILoadScreenFont")                            ? 2U :
        !CompareUIStringsCaseInsensitive(label.fontName.c_str(), "UINotePadFont")                               ? 3U :
                                                                                                                  0U;
    return {uiFontLanguage, family, size};
}

void DrawMovesLabel(
    const d3t::UILabelDefinition &label, const d3t::UIResolvedLabel &placement, std::string_view text, float opacity, d3t::UIDefinition *definition) {
    if (!drawingUI)
        return;
    const UIFontSelection font = GetUILabelFont(label);
    const float height = GetUIFontHeight(font);
    if (!(height > 0.0f))
        return;
    const d3t::UIRect &bounds = placement.bounds;
    float scale = 1.0f;
    std::int32_t split = -1;
    if (definition) {
        const d3t::UIPanelDefinition *const panel = label.panelIndex < 0 ? nullptr : &definition->panels[label.panelIndex];
        const float referenceWidth = panel ? panel->width * label.width / 1920.0f : -1.0f;
        const float referenceHeight = panel ? panel->height * label.height / 1080.0f : -1.0f;
        scale = FitUILabelText(font, text, bounds, referenceWidth, referenceHeight, split);
        if (label.scaleVar >= 0) {
            d3t::UIViewVariable &variable = definition->variables[label.scaleVar];
            const float screenHeight = static_cast<float>(static_cast<std::int32_t>(GetS2UserInterfaceSafeArea() * 1080.0f));
            const float screenWidth = static_cast<float>(static_cast<std::int32_t>(GetUIViewportWidth() - (1080.0f - screenHeight)));
            if (label.scaleScreenWidth != screenWidth || label.scaleScreenHeight != screenHeight)
                variable.value = variable.initialValue;
            label.scaleScreenWidth = screenWidth;
            label.scaleScreenHeight = screenHeight;
            if (variable.value.floatValue > scale && label.scaleVarAssign == 1)
                variable.value.floatValue = scale;
            else
                scale = variable.value.floatValue;
        }
    } else {
        scale = label.textScale;
        const float measuredWidth = GetUIFontTextWidth(font, height, text);
        if (measuredWidth * scale > bounds.width)
            scale = bounds.width / measuredWidth;
        if (height > bounds.height && scale > bounds.height / height)
            scale = bounds.height / height;
        if (scale < 0.33f)
            scale = 0.33f;
    }
    const float sx = static_cast<float>(d3t::g_originalScreenWidth) / 1920.0f;
    const float sy = static_cast<float>(d3t::g_originalScreenHeight) / 1080.0f;
    const std::uint32_t colour = (label.colour & 0x00ffffffU) | (static_cast<std::uint32_t>((label.colour >> 24U) * opacity) << 24U);
    const d3t::UIRect &clip = placement.clip;
    const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(clip.x * sx), static_cast<std::int32_t>(clip.y * sy),
        static_cast<std::int32_t>((clip.x + clip.width) * sx), static_cast<std::int32_t>((clip.y + clip.height) * sy)};
    std::uint32_t alignment = 18;
    if (label.alignment == "Left")
        alignment = 17;
    else if (label.alignment == "Right")
        alignment = 20;
    else if (label.alignment == "LeftTop")
        alignment = 9;
    else if (label.alignment == "LeftDown")
        alignment = 33;
    else if (label.alignment == "RightTop")
        alignment = 12;
    else if (label.alignment == "RightDown")
        alignment = 36;
    else if (label.alignment == "CentreUp")
        alignment = 10;
    else if (label.alignment == "CentreDown")
        alignment = 34;
    const std::int32_t passes = label.hasStroke ? 2 : 1;
    for (std::int32_t pass = 0; pass < passes; ++pass) {
        const bool stroke = label.hasStroke && pass == 0;
        const UIFontSelection selected{font.language, stroke ? 1U : label.hasStroke ? 0U : font.family, font.image};
        const float drawHeight = GetUIFontHeight(selected) * scale;
        const std::uint32_t drawColour =
            stroke ? (label.strokeColour & 0x00ffffffU) | (static_cast<std::uint32_t>((label.strokeColour >> 24U) * opacity) << 24U) : colour;
        const std::int32_t lines = split < 0 ? 1 : 2;
        for (std::int32_t line = 0; line < lines; ++line) {
            const std::string_view part = split < 0 ? text :
                line == 0                           ? text.substr(0, static_cast<std::size_t>(split)) :
                                                      text.substr(static_cast<std::size_t>(split) + 1);
            const float width = GetUIFontTextWidth(selected, drawHeight, part);
            const float rowHeight = bounds.height / static_cast<float>(lines);
            float x = bounds.x, y = bounds.y + rowHeight * static_cast<float>(line);
            if (alignment & 2U)
                x += (bounds.width - width) * 0.5f;
            else if (alignment & 4U)
                x += bounds.width - width;
            if (alignment & 16U)
                y += (rowHeight - drawHeight) * 0.5f;
            else if (alignment & 32U)
                y += rowHeight - drawHeight;
            DrawUIFontText(selected, drawHeight * sy, part, x * sx, y * sy, drawColour, scissor);
        }
    }
}

// @verified - p
std::int64_t RenderMoveLearnedDefinition() {
    uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::MoveLearned)].config.uiHandleIndex);

    return 0;
}

void DrawMovesButton(const d3t::UITextureObjectDefinition &object, const d3t::UIResolvedTextureObject &placement, std::int32_t action, float opacity) {
    if (!drawingUI)
        return;
    const d3t::UIDefinition &icons = uiDefinitions[static_cast<std::size_t>(UIElement::PlatformIcons)];
    const d3t::UIImageDefinition *const image = icons.findImage("ButtonIcons");
    if (!image || icons.spriteSheets.empty())
        return;
    const bool keyboard = GetIsKeyboardActive() == 1;
    std::int32_t frame = action == 24 ? 20 : action == 22 ? 18 : action == 55 ? 13 : action == 54 ? 11 : action == 56 ? 0 : action == 23 ? 19 : 21;
    std::int32_t key = -1, labelKey = -1;
    if (keyboard) {
        const std::int32_t type = GetCurrentUIInputType();
        std::uint32_t mapped;
        if (action == 54 || action == 55)
            mapped = GetFightKeyMapping(action == 54 ? 16 : 17);
        else if (action == 56)
            mapped = GetGameKeyMapping(8);
        else if (type <= 1)
            mapped = GetMenuKeyMapping(action);
        else
            mapped = GetGameKeyMapping(action);
        key = static_cast<std::int32_t>(MapInputKeyToCurrentLayout(mapped));
        labelKey = action == 54 || action == 55 ? static_cast<std::int32_t>(GetGameKeyMapping(action == 54 ? 16 : 17)) : key;
        switch (key) {
        case -1:
            frame = 40;
            break;
        case 37:
            frame = 36;
            break;
        case 38:
            frame = 37;
            break;
        case 39:
            frame = 38;
            break;
        case 40:
            frame = 39;
            break;
        case 256:
            frame = 41;
            break;
        case 257:
            frame = 43;
            break;
        case 258:
            frame = 42;
            break;
        default:
            frame = 26;
            break;
        }
    }
    const d3t::UISpriteFrame &crop = icons.spriteSheets[0].frames[static_cast<std::size_t>(frame)];
    const d3t::UISharedSpriteImage shared{image->textureHandle, {crop.x, crop.y, crop.width, crop.height}};
    const d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::SubMenuMoves)];
    d3t::DrawUITextureObject(definition, object, placement, opacity, &shared);
    if (keyboard && frame == 26) {
        std::array<char, 64> name{};
        if (!FormatUIInputKeyName(labelKey, name))
            return;
        d3t::UILabelDefinition label;
        label.alignment = "Centre";
        label.colour = object.fontColour;
        label.fontSize = "H3";
        DrawMovesLabel(label, {placement.bounds, placement.clip}, name.data(), opacity);
    }
}

void DrawGameMovesRow(
    UIGrid *grid, void *panel, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    GameMovesRuntime &runtime = gameMovesRuntime;
    nk_context &context = *GetNativeUiContext();
    context.style.button.rounding = 0.0f;
    const UILayoutRect holder{.x = outer->x, .y = outer->y, .width = outer->z, .height = outer->w};
    const UILayoutRect cell{.x = bounds->x, .y = bounds->y, .width = bounds->z, .height = bounds->w};
    PlaceUIGridCellWidgets(runtime.cellTemplate, ordinal, holder, cell);
    const float opacity = ordinal == GetUIGridSelection(*grid) ? 1.0f : 0.60000002f;
    const WazamakiMoveInfo *const move = dictionary ? GetGameMove(dictionary[ordinal]) : nullptr;
    d3t::UIViewSlot<kUIViewImageCapacity> *const view =
        GetNuklearElementByIdx(g_uiElements[static_cast<std::size_t>(UIElement::SubMenuMoves)].config.uiHandleIndex);
    if (move && view) {
        const std::string_view name = GetLocalizedSceneText(UI_MOVES_NAMES[move->category][move->id]);
        for (std::size_t index = 3; index < 5; ++index) {
            d3t::UILabelWidget *const label = runtime.labels[index];
            if (!label)
                continue;
            std::snprintf(runtime.labelText[index - 3].data(), 256, "%.*s", static_cast<int>(name.size()), name.data());
            label->colour = (label->colour & 0x00ffffffU) | (static_cast<std::uint32_t>(opacity * 255.0f) << 24);
        }
        if (runtime.windows[7]) {
            runtime.windows[7]->layout.height.value.fraction = runtime.windows[6]->layout.height.value.fraction;
            runtime.windows[7]->layout.width.value.fraction = static_cast<float>(move->proficiency) / 200.0f * runtime.windows[6]->layout.width.value.fraction;
            runtime.windows[7]->alpha = opacity;
        }
        if (move->critical) {
            d3t::UILabelWidget *const label = runtime.labels[4];
            if (g_uiFontSize != 2) {
                g_uiFontSize = 2;
                UI_SelectFont();
            }
            g_uiFontScale = view->variables[label->scaleVar].value.floatValue;
            UI_SelectFont();
            const nk_user_font *const font = context.style.font;
            const std::size_t length = strnlen(label->text, 64);
            runtime.windows[22]->layout.offsetX =
                font->width(font->userdata, font->height, label->text, static_cast<std::int32_t>(length)) / static_cast<float>(UI_GetAspectWidth());
            runtime.windows[22]->alpha = opacity;
            if (runtime.critical) {
                std::int32_t frame = runtime.criticalFrame - static_cast<std::int32_t>(static_cast<float>(24U * ordinal % 7U) / -6.0f * 7.0f);
                if (frame > 7)
                    frame -= 7;
                runtime.critical->sprite = frame;
            }
        } else if (runtime.windows[22])
            runtime.windows[22]->alpha = 0.0f;
        for (std::size_t index = 11; index < 22; ++index)
            if (runtime.windows[index])
                runtime.windows[index]->alpha = 0.0f;
        std::size_t button = 0;
        for (std::int32_t command = static_cast<std::int8_t>(move->inputCommands[0]) - 1; command >= 0; --command) {
            const std::uint8_t code = move->inputCommands[command + 1];
            const std::int32_t action = code < MOVE_INPUT_ICON_ACTIONS.size() ? MOVE_INPUT_ICON_ACTIONS[code] : -1;
            if (action == -1)
                continue;
            if (action == 70) {
                if (button && runtime.windows[16 + button])
                    runtime.windows[16 + button]->alpha = opacity;
            } else {
                if (runtime.icons[button])
                    runtime.icons[button]->action = action;
                if (runtime.windows[11 + button])
                    runtime.windows[11 + button]->alpha = opacity;
                ++button;
            }
        }
        const bool variants = GetWazamakiLearnedGroups()[dictionary[ordinal]].count > 1;
        for (std::size_t index = 8; index < 10; ++index)
            if (runtime.windows[index])
                runtime.windows[index]->alpha = variants ? 1.0f : 0.0f;
        const float pulse = GetUITransitionValue(runtime.arrowTransition);
        for (std::size_t index = 8; index < 10; ++index) {
            if (!runtime.windows[index])
                continue;
            const std::uint32_t alpha =
                ordinal == runtime.selectedMove ? static_cast<std::uint8_t>((index == 8 ? pulse : (1.0f - pulse) + 0.15000001f) * 255.0f) : 146U;
            runtime.windows[index]->colour = (runtime.windows[index]->colour & 0x00ffffffU) | (alpha << 24);
        }
    }
    AfterDrawUIGridCell(runtime.cellTemplate, *grid, ordinal, 5, -1, holder, static_cast<nk_panel *>(panel));
}
bool FindUIParentBounds(std::string_view id, d3t::UIRect &bounds, const d3t::UIPanelDefinition **authored) {
    for (std::size_t definitionIndex = 0; definitionIndex < uiDefinitions.size(); ++definitionIndex) {
        const auto &definition = uiDefinitions[definitionIndex];
        std::int32_t found = -1;
        for (std::size_t index = 0; index < definition.panels.size(); ++index)
            if (definition.panels[index].id == id) {
                found = static_cast<std::int32_t>(index);
                break;
            }
        if (found < 0)
            continue;
        auto &layout = parentMenuLayouts[definitionIndex];
        if (layout.parents.size() != definition.panels.size())
            continue;
        d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, {}, {}, {}, {});
        bounds = layout.panels[found];
        const auto &panel = definition.panels[found];
        if (authored)
            *authored = &panel;
        std::int32_t root = found;
        while (layout.parents[root] >= 0)
            root = layout.parents[root];
        const auto &rootPanel = definition.panels[root];
        d3t::UIRect external{};
        const d3t::UIPanelDefinition *externalSource{};
        if (!rootPanel.parentId.empty() && FindUIParentBounds(rootPanel.parentId, external, &externalSource)) {
            const d3t::UIPanelBoundsOverride override{
                static_cast<std::size_t>(root), d3t::ResolveUIPanelBounds(rootPanel, external, externalSource->width, externalSource->height)};
            d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, {}, {}, {}, {}, std::span{&override, 1});
            bounds = layout.panels[found];
        }
        return true;
    }
    return false;
}

std::int32_t MenuPanelIndex(const d3t::UIDefinition &definition, std::string_view id) {
    for (std::size_t index = 0; index < definition.panels.size(); ++index)
        if (definition.panels[index].id == id)
            return static_cast<std::int32_t>(index);
    return -1;
}

void BindAuthoredMenuLayout(const d3t::UIDefinition &definition, AuthoredMenuLayout &layout) {
    layout.parents.resize(definition.panels.size());
    layout.panels.resize(definition.panels.size());
    layout.objectPanels.resize(definition.textureObjects.size());
    layout.objects.resize(definition.textureObjects.size());
    layout.labelPanels.resize(definition.labels.size());
    layout.labels.resize(definition.labels.size());
    for (std::size_t index = 0; index < definition.panels.size(); ++index)
        layout.parents[index] = MenuPanelIndex(definition, definition.panels[index].parentId);
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index)
        layout.objectPanels[index] = MenuPanelIndex(definition, definition.textureObjects[index].panelId);
    for (std::size_t index = 0; index < definition.labels.size(); ++index)
        layout.labelPanels[index] = MenuPanelIndex(definition, definition.labels[index].panelId);
}

std::uint32_t LoadBigKanjiText() {
    return ReadUIDefinitionFile(UIElement::BigKanjiText, "/ui/BigKanjiText/BigKanjiText.ui", 0);
}

// @verified - p
std::int64_t BindBigKanjiText() {
    const std ::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::BigKanjiText)].config.uiHandleIndex;
    for (std::size_t index = 0; index < bigKanjiPanels.size(); ++index)
        bigKanjiPanels[index] = UI_GetWindow(UI_FindViewWindow(slot, BIG_KANJI_PANEL_IDS[index].data()));
    for (std::size_t index = 0; index < bigKanjiImages.size(); ++index)
        bigKanjiImages[index] = UI_GetColourImageWidget(UI_FindViewColourImage(slot, BIG_KANJI_IMAGE_IDS[index].data()));

    return 1;
}

std::int64_t ResetBigKanjiText() {
    bigKanjiPanels.fill(nullptr);
    bigKanjiImages.fill(nullptr);
    return 0;
}

std::int64_t BigKanjiTextEnabled() {
    return 1;
}
std::int64_t BigKanjiTextTickNullsub() {
    return 0;
}

// @verified - p
void UpdateBigKanjiImages() {
    for (std::size_t index = 0; index < bigKanjiGlyphs.size(); ++index) {
        BigKanjiGlyph &glyph = bigKanjiGlyphs[index];
        if (glyph.admittedTexture)
            continue;
        glyph.admittedTexture = GetShaderDataByIndex(glyph.texture);
        if (!glyph.admittedTexture)
            continue;
        const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::BigKanjiText)];
        d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(element.config.uiHandleIndex);
        if (!view)
            continue;
        d3t::UIViewImage &image = view->images[index];
        image = {};
        struct nk_image raster {};
        const bool described = UI_SetImageFromTexture(raster, glyph.admittedTexture);
        std::memcpy(&image.textureHandle, &raster, sizeof(raster));
        if (!described)
            continue;
        d3t::Texture2DDescription description;
        if (!d3t::GetTexture2DDescription(glyph.admittedTexture, &description))
            continue;
        image.widthScale = static_cast<float>(description.width) / 1920.0f;
        image.heightScale = static_cast<float>(description.height) / 1080.0f;
        image.ready = 1;
        if (bigKanjiImages[index])
            bigKanjiImages[index]->image = static_cast<std::int32_t>(index);
    }
}

// @verified - p
void UpdateBigKanjiPositionAndColour() {
    for (std::size_t index = 0; index < bigKanjiGlyphs.size(); ++index) {
        UIColourImageWidget *const image = bigKanjiImages[index];
        d3t::UIWindow *const panel = bigKanjiPanels[index];
        if (!image || !panel)
            continue;
        panel->alpha = 0.0f;
        const BigKanjiGlyph &glyph = bigKanjiGlyphs[index];
        if (!glyph.requested || !glyph.admittedTexture || !glyph.sprite)
            continue;
        panel->alpha = 1.0f;
        image->colour = glyph.colour;
        const float halfHeight = panel->layout.height.value.fraction * 0.5f;
        panel->layout.offsetX = (glyph.sprite->x / 640.0f) - (panel->layout.width.value.fraction * 0.5f);
        panel->layout.offsetY = (glyph.sprite->y / 480.0f) - halfHeight;
        UI_InvalidateWindowRect(*panel);
    }
}

// @verified - p
std::int64_t RenderBigKanjiText() {
    UpdateBigKanjiImages();
    UpdateBigKanjiPositionAndColour();
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::BigKanjiText)];
    uiUpdateWithColour(*GetNativeUiContext(), element.config.uiHandleIndex);

    for (BigKanjiGlyph &glyph : bigKanjiGlyphs)
        glyph.requested = 0;
    return 0;
}

constexpr std::array<UIElement, 6> gameSubmenus{
    UIElement::SubMenuItems, UIElement::SubMenuCollection, UIElement::SubMenuPhotos, UIElement::SubMenuMoves, UIElement::LoadSave, UIElement::SubMenuOptions};
constexpr std::array<std::string_view, 6> gameMenuLabels{"GameMenu::Label_Items", "GameMenu::Label_Collection", "GameMenu::Label_Photos",
    "GameMenu::Label_Moves", "GameMenu::Label_Save", "GameMenu::Label_Options"};
constexpr std::array<std::string_view, 6> gameMenuIcons{
    "GameMenu::Icon_Items", "GameMenu::Icon_Collection", "GameMenu::Icon_Photos", "GameMenu::Icon_Moves", "GameMenu::Icon_Save", "GameMenu::Icon_Options"};

std::uint32_t LoadGameMenu() {
    return ReadUIDefinitionFile(UIElement::GameMenu, "/ui/gamemenu_New/GameMenu_New.ui", 0);
}

void SwitchGameSubmenu(std::int32_t previous, std::int32_t next) {
    gameMenuPending = next;
    if (previous >= 0 && previous < 6) {
        if ((gameSubmenus[previous] == UIElement::SubMenuOptions && AskKeepOrRevertChanges()) || DestroyElementByIndex(gameSubmenus[previous]) == 0)
            return;
        CopyUIMessageBox(nullptr);
        AicaExecuteWrapper(3);
    }
    if (next >= 0 && next < 6) {
        gameMenuCurrent = next;
        ShowElementByIndex(gameSubmenus[next]);
    }
    if (next >= 0 && next < 6 && gameMenuWindows[6] && gameMenuWindows[next])
        std::snprintf(gameMenuWindows[6]->layout.parentId.data(), gameMenuWindows[6]->layout.parentId.size(), "%s", gameMenuWindows[next]->id.data());
    for (std::size_t tab = 0; tab < gameSubmenus.size(); ++tab) {
        const std::uint32_t alpha = tab == static_cast<std::size_t>(next) ? 255U : 63U;
        if (gameMenuNativeLabels[3 + tab])
            gameMenuNativeLabels[3 + tab]->colour = (gameMenuNativeLabels[3 + tab]->colour & 0xffffffU) | (alpha << 24U);
        if (gameMenuNativeIcons[tab])
            gameMenuNativeIcons[tab]->colour = (gameMenuNativeIcons[tab]->colour & 0xffffffU) | (alpha << 24U);
    }
    gameMenuPending = -1;
}

void GameMenuTabClicked(float *, void *data) {
    std::int32_t next = gameMenuCurrent;
    for (std::int32_t tab = 0; tab < 6; ++tab) {
        if (data == gameMenuWindows[tab]) {
            next = tab;
            break;
        }
    }
    if (next != gameMenuCurrent)
        SwitchGameSubmenu(gameMenuCurrent, next);
}

std::int64_t BindGameMenu() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].config.uiHandleIndex;
    constexpr std::array<const char *, 13> windows{"GameMenu::BackgroundTab_00", "GameMenu::BackgroundTab_01", "GameMenu::BackgroundTab_02",
        "GameMenu::BackgroundTab_03", "GameMenu::BackgroundTab_04", "GameMenu::BackgroundTab_05", "GameMenu::SelectedTab", "GameMenu::LeftIconPanel",
        "GameMenu::RightIconPanel", "GameMenu::PlayerInfoParent", "GameMenu::PlayerInformationBar_Date_Value", "GameMenu::PlayerInformationBar_Tokens_Value",
        "GameMenu::PlayerInformationBar_Money_Value"};
    constexpr std::array<const char *, 3> labels{"Label_PlayerInfoBar_Money", "Label_PlayerInfoBar_Tokens", "Label_PlayerInfoBar_Calendar"};
    for (std::size_t index = 0; index < windows.size(); ++index)
        gameMenuWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, windows[index]));
    for (std::size_t tab = 0; tab < 6; ++tab) {
        if (gameMenuWindows[tab])
            gameMenuWindows[tab]->mouseCallbacks.clicked = GameMenuTabClicked;
        const std::int32_t label = UI_FindViewLabel(slot, gameMenuLabels[tab].data());
        gameMenuNativeLabels[3 + tab] = label >= 0 ? &g_uiLabelWidgets[label] : nullptr;
        const std::int32_t icon = UI_FindViewColourImage(slot, gameMenuIcons[tab].data());
        gameMenuNativeIcons[tab] = icon >= 0 ? &g_uiColourImageWidgets[icon] : nullptr;
    }
    for (std::size_t index = 0; index < labels.size(); ++index) {
        const std::int32_t label = UI_FindViewLabel(slot, labels[index]);
        gameMenuNativeLabels[index] = label >= 0 ? &g_uiLabelWidgets[label] : nullptr;
        if (gameMenuNativeLabels[index]) {
            gameMenuNativeLabels[index]->text = gameMenuInformationText[index].data();
            gameMenuNativeLabels[index]->token.alreadyLocalized = 1;
        }
    }
    return 1;
}

std::int64_t ResetGameMenu() {
    gameMenuWindows.fill(nullptr);
    gameMenuNativeLabels.fill(nullptr);
    gameMenuNativeIcons.fill(nullptr);
    return 0;
}

std::int64_t GameMenuInput() {
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    if (pressed & 0x200U)
        gameMenuClosing = 1;
    if (gameMenuClosing) {
        if (ConfirmGameMenuClose())
            gameMenuClosing = 0;
        return 0;
    }
    std::int32_t next = gameMenuCurrent;
    if (pressed & 0x88U) {
        if (++next >= 6)
            next = 0;
    }
    if (pressed & 0x50U) {
        if (--next < 0)
            next = 5;
    }
    if (next != gameMenuCurrent)
        SwitchGameSubmenu(gameMenuCurrent, next);
    else if (gameMenuPending != -1)
        SwitchGameSubmenu(gameMenuCurrent, gameMenuPending);
    return 1;
}

std::int64_t TickGameMenu() {
    using enum CameraState;
    const auto *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const player = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (player) {
        EV_SetCharDisplay(player, CVM_DisplayEnabled, 0);
        charaSetVisibilityFlagTo(player, 0);
        camCtrlSetState(Event);
        camCtrlSetPerspective(0x2666);
        Vector3 target = GetCharacterWorldPosition(player);
        target.y += 1.6f;
        Vector3 eye = target;
        const auto yaw = static_cast<std::int16_t>(-static_cast<std::int16_t>(playerGetRotY()));
        eye.x -= sin_fixed16(yaw) * 0.2f;
        eye.z += cos_fixed16(static_cast<std::uint16_t>(yaw)) * 0.2f;
        camCtrlSetNextPosition(eye);
        camCtrlSetTargetPosition(target);
    }
    CopyUIMessageBox(nullptr);
    ClearMenuInputHints();
    gameMenuClosing = 0;
    ShowElementByIndex(UIElement::GameInfoBar);
    SetDebugHideCameraIcon(1);
    SetInformationBarVisibilityMode(1);
    SetSaveFrontendMode(1);
    SwitchGameSubmenu(-1, gameMenuCurrent);
    gameMenuActive = 1;
    AicaExecuteWrapper(3);
    return 0;
}

std::int64_t DestroyGameMenu() {
    if (gameMenuCurrent >= 0 && gameMenuCurrent < 6 && DestroyElementByIndex(gameSubmenus[gameMenuCurrent]) == 0) {
        gameMenuClosing = 1;
        return 0;
    }
    DestroyElementByIndex(UIElement::GameInfoBar);
    gameMenuActive = 0;
    AicaExecuteWrapper(3);
    return 1;
}

std::int64_t ResetGameMenuRender() {
    const bool suppressed = g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isRenderable != 0;
    const auto apply = [suppressed](UIElement id) {
        auto &element = g_uiElements[static_cast<std::size_t>(id)];
        if (suppressed && !element.isRenderable) {
            element.isRenderable = 1;
            if (element.resetRender)
                element.resetRender();
        } else if (!suppressed)
            DisableElement(id);
    };
    if (gameMenuCurrent >= 0 && gameMenuCurrent < 6)
        apply(gameSubmenus[gameMenuCurrent]);
    apply(UIElement::GameInfoBar);
    return 0;
}

void FormatGameMenuNumber(std::span<char> output, std::int32_t value) {
    output[0] = 0;
    if (value < 0)
        return;
    std::array<char, 16> digits{};
    const auto result = std::to_chars(digits.data(), digits.data() + digits.size(), value);
    const auto length = static_cast<std::size_t>(result.ptr - digits.data());
    const auto language = GetLoadingScreenTextType(g_s2SelectedSubtitleLanguage);
    const char separator = language == 2 || language == 3 ? ' ' : ',';
    std::size_t written = 0;
    for (std::size_t index = 0; index < length; ++index) {
        if (index && (length - index) % 3 == 0)
            output[written++] = separator;
        output[written++] = digits[index];
    }
    output[written] = 0;
}

std::int64_t RenderGameMenu() {
    const S2PersistentPlayerStats &stats = S2Save_GetPersistentPlayerStats();
    std::array<char, 32> amount{};
    const std::uint32_t unsignedCash = static_cast<std::uint32_t>(stats.cash);
    FormatGameMenuNumber(amount, static_cast<std::int32_t>(stats.cash < 0 ? 0U - unsignedCash : unsignedCash));
    const std::int32_t language = GetLoadingScreenTextType(g_s2SelectedSubtitleLanguage);
    std::snprintf(gameMenuInformationText[0].data(), 256, language == 2 || language == 3 ? "%s%s HK$" : "%sHK$ %s", stats.cash < 0 ? "-" : "", amount.data());
    FormatGameMenuNumber(gameMenuInformationText[1], stats.segaCoins);
    DateTime date{};
    GetCurrentDateTime(date);
    FormatLoadingScreenDate(gameMenuInformationText[2], date.year, date.month, date.day, date.week, 2);
    nk_context &context = *GetNativeUiContext();
    g_waitingForNextTick = 0;
    UILayoutRect rect{};
    UI_GetWindowRect(gameMenuWindows[9], rect);
    const float aspectWidth = static_cast<float>(UI_GetAspectWidth());
    for (std::size_t value = 0; value < 3; ++value) {
        const d3t::UILabelWidget *const label = gameMenuNativeLabels[2 - value];
        d3t::UIWindow *const window = gameMenuWindows[10 + value];
        if (!label || !window)
            continue;
        if (g_uiFontFace != label->fontFamily && static_cast<std::uint32_t>(label->fontFamily) < 4U) {
            g_uiFontFace = label->fontFamily;
            UI_SelectFont();
        }
        if (g_uiFontSize != label->size && static_cast<std::uint32_t>(label->size) < 6U) {
            g_uiFontSize = label->size;
            UI_SelectFont();
        }
        const char *const text = gameMenuInformationText[2 - value].data();
        const UILabelFit fit = UI_FitLabelScale(context, text, -1, label->alignment, rect, rect.width, rect.height, 0);
        const UILayoutRect placed = UI_DrawLabelText(context, text, -1, label->alignment, rect, fit);
        window->layout.width.value.fraction = placed.width / aspectWidth;
        window->layout.height.value.fraction = rect.height / 1080.0f;
    }
    g_waitingForNextTick = 1;
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)];
    DrawUIEntity(context, element.config.uiHandleIndex, element.index);
    return 0;
}

std::intptr_t DrawGameMenuFrame() {
    RenderGameMenu();
    for (std::size_t index = static_cast<std::size_t>(UIElement::SubMenuItems); index <= 23; ++index) {
        if (index > 15 && index < 20)
            continue;
        const UIElementState &element = g_uiElements[index];
        if (element.config.enabled && element.render)
            element.render();
    }
    const UIElementState &save = g_uiElements[static_cast<std::size_t>(UIElement::LoadSave)];
    if (save.config.enabled && save.render)
        save.render();
    const UIElementState &prompt = g_uiElements[static_cast<std::size_t>(UIElement::YesNoPrompt)];
    if (prompt.config.enabled && prompt.render && !g_uiElements[static_cast<std::size_t>(UIElement::SubMenuOptions)].isVisible)
        prompt.render();
    const UIElementState &information = g_uiElements[static_cast<std::size_t>(UIElement::GameInfoBar)];
    if (information.config.enabled && information.render && !g_uiElements[static_cast<std::size_t>(UIElement::SubMenuOptions)].isVisible)
        information.render();
    Nuklear_BindRenderTarget();
    NuklearUpdateForAspectRatio(GetAspectRatioModeShouldScale());
    CopyNuklearTexToBackBuffer();
    return 0;
}

void RenderGameMenuFrame() {
    if (!drawingUI)
        return;
#if defined(_WIN32)
    ExecuteCurrentCommandList();
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(1);
    const NuklearFrameCallback previous = g_nuklearFrameCallback;
    g_nuklearFrameCallback = DrawGameMenuFrame;
    InvokeNuklearFrame(d3t::DX11::g_ppD3DDeviceContext);
    FinishNuklearFrame();
    g_nuklearFrameCallback = previous;
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    d3t::BindUIFrameRenderTarget();
#endif
}

std::uint32_t LoadItemsMenu() {
    return ReadUIDefinitionFile(UIElement::SubMenuItems, "/ui/gamemenu_New/SubMenu_Items.ui", 0);
}
const std::int32_t *ItemsSelectionData(UIGrid *, std::int32_t row) {
    return itemsMenu.dictionary[row].data();
}

void SelectItemsRow(float *bounds, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    UIMessageBox message{};
    message.imageIndex = message.widgetIndex = -1;
    if (bounds)
        std::memcpy(&message.bounds, bounds, sizeof(message.bounds));
    if (row) {
        message.primaryText = row[2] ? GetShenmueItem(row[0], row[1]) : "? ? ?";
        message.primaryLocalized = row[2] == 0;
        if (row[2] > 1) {
            std::snprintf(itemsMenu.quantityText.data(), itemsMenu.quantityText.size(), "(%u)", static_cast<unsigned>(row[2]));
            message.tertiaryText = itemsMenu.quantityText.data();
            message.tertiaryLocalized = 1;
        }
        d3t::UISharedSpriteImage image{};
        if (ResolveSharedItemImage(GetInventoryItemIcon(row[0], row[1]), image)) {
            std::uint32_t width{}, height{};
            d3t::GetTexture2DDimensions(image.textureHandle, &width, &height);
            message.texture = image.textureHandle;
            message.textureWidth = static_cast<std::uint16_t>(width);
            message.textureHeight = static_cast<std::uint16_t>(height);
            message.cropX = static_cast<std::uint16_t>(image.source.x);
            message.cropY = static_cast<std::uint16_t>(image.source.y);
            message.cropWidth = static_cast<std::uint16_t>(image.source.width);
            message.cropHeight = static_cast<std::uint16_t>(image.source.height);
        }
    }
    CopyUIMessageBox(&message);
}

void ActivateItemsRow(float *, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    if (!row) {
        AicaExecuteWrapper(8);
        return;
    }
    if (row[0] == 0 && row[1] <= 3) {
        ShowSharedItemsWidget(row[0], row[1]);
        if (row[1] == 0)
            ResolveUniqueItemAction(2);
        else if (row[1] == 1)
            ResolveUniqueItemAction(0);
        else if (row[1] == 3) {
            ResolveUniqueItemAction(1);
            AicaExecuteWrapper(2);
        }
        if (row[1] != 2)
            SetItemLookShowingGameMenu();
    } else if (row[2]) {
        ShowSharedItemsWidget(row[0], row[1]);
        SetItemLookShowingGameMenu();
        if (!OpenItemLook(GetInventoryItemId(row[0], row[1]), 1)) {
            AicaExecuteWrapper(8);
            return;
        }
        AicaExecuteWrapper(2);
    } else {
        AicaExecuteWrapper(8);
        return;
    }
    if (row[1] != 2) {
        auto &menu = g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)];
        menu.isRenderable = 1;
        ResetGameMenuRender();
    }
}

void DrawInventoryGridCell(UIGrid *grid, const Vector4 *bounds, const std::int32_t *dictionary, std::int32_t ordinal) {
    const UILayoutRect cell{.x = bounds->x, .y = bounds->y, .width = bounds->z, .height = bounds->w};
    nk_context &context = *GetNativeUiContext();
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        context.current->layout->row.item = nk_rect(cell.x, cell.y, cell.width, cell.height);
    if (g_waitingForNextTick == 1) {
        const UIGridStyle &style = grid->style ? *grid->style : IsCameraIconHidden() ? UI_GRID_GAME_STYLE : UI_GRID_FRONTEND_STYLE;
        nk_color colour{};
        std::memcpy(&colour, &style.borderColour, sizeof(colour));
        nk_button_color(&context, colour);
    }
    if (!dictionary)
        return;
    std::int32_t *const entry = const_cast<std::int32_t *>(dictionary) + ordinal * 8;
    struct nk_image image {};
    RenderingRelatedInventory(image, GetInventoryItemIcon(entry[0], entry[1]), 0);
    UILayoutRect fitted = cell;
    UI_ScaleSize(fitted.width, fitted.height, 2, cell.width, cell.height, image.region[2], image.region[3], nullptr);
    UI_AlignInArea(1, 1, fitted.width, fitted.height, fitted.x, fitted.y, cell.width, cell.height);
    AdjustForSafeArea(fitted, 12.0f, 12.0f);
    RenderingRelatedInventory(image, GetInventoryItemIcon(entry[0], entry[1]), entry[2] ? 0 : 1);
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        context.current->layout->row.item = nk_rect(fitted.x, fitted.y, fitted.width, fitted.height);
    if (g_waitingForNextTick == 1) {
        const std::uint32_t tint = entry[2] ? 0xffffffffU : 0xa5463c3cU;
        nk_color colour{};
        std::memcpy(&colour, &tint, sizeof(colour));
        nk_image_color(&context, image, colour);
    }
    entry[6] = ordinal;
}

void DrawItemsRow(
    UIGrid *grid, void *panel, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    const UILayoutRect holder{.x = outer->x, .y = outer->y, .width = outer->z, .height = outer->w};
    const UILayoutRect cell{.x = bounds->x, .y = bounds->y, .width = bounds->z, .height = bounds->w};
    PlaceUIGridCellWidgets(itemsMenu.cellTemplate, ordinal, holder, cell);
    AfterDrawUIGridCell(itemsMenu.cellTemplate, *grid, ordinal, 1, -1, holder, static_cast<nk_panel *>(panel));
    DrawInventoryGridCell(grid, bounds, dictionary, ordinal);
}

std::int64_t BindItemsMenu() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuItems)].config.uiHandleIndex;
    for (std::size_t index = 0; index < itemsMenu.windows.size(); ++index)
        itemsMenu.windows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_ITEMS_GRID_WINDOWS[index]));
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    for (std::size_t index = 0; index < itemsMenu.layoutItems.size(); ++index) {
        itemsMenu.layoutItems[index] = nullptr;
        if (!view)
            continue;
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 13)
                continue;
            d3t::UILayoutItem &item = g_uiLayoutItems[view->widgetPoolIndices[widget]];
            if (!std::strcmp(item.id.data(), UI_ITEMS_GRID_LAYOUT_ITEMS[index])) {
                itemsMenu.layoutItems[index] = &item;
                break;
            }
        }
    }
    InitializeUIGrid(itemsMenu.grid,
        {.columns = 7,
            .visibleRows = 3,
            .rows = 54,
            .drawEmpty = 1,
            .panel = itemsMenu.windows[0],
            .drawRow = DrawItemsRow,
            .actions = {SelectItemsRow, nullptr, nullptr, ActivateItemsRow},
            .selectionData = ItemsSelectionData,
            .sound = 3});
    itemsMenu.cellTemplate = {.initialized = 0,
        .viewSlot = static_cast<std::int32_t>(slot),
        .first = 1,
        .end = 5,
        .clipWindows = {3, -1},
        .windows = itemsMenu.windows.data(),
        .ids = UI_ITEMS_GRID_WINDOWS.data(),
        .layoutFirst = 0,
        .layoutEnd = 2,
        .layoutItems = itemsMenu.layoutItems.data()};
    itemsMenu.bound = true;
    return 1;
}

std::int64_t TickItemsMenu() {
    ResetUIGridTabSelector();
    const auto stock = S2Save_GetInventoryBank(S2InventoryBank::Type0);
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < 384; ++index) {
        const auto quantity = index < static_cast<std::int32_t>(stock.size()) ? std::to_integer<std::uint8_t>(stock[index]) : 0;
        if (!quantity || index == 2)
            continue;
        auto &row = itemsMenu.dictionary[count++];
        row[0] = 0;
        row[1] = index;
        row[2] = quantity;
    }
    itemsMenu.count = count;
    SetUIGridDictionary(itemsMenu.grid, count, itemsMenu.dictionary[0].data());
    ResetUIGrid(itemsMenu.grid);
    return 0;
}

std::int64_t ItemsMenuInput() {
    ProcessUIGridInput(itemsMenu.grid, uiPointer, GetFrameTime());
    return 1;
}

std::int64_t ResetItemsMenu() {
    itemsMenu.bound = false;
    return 0;
}
std::int64_t DestroyItemsMenu() {
    return 1;
}

std::int64_t RenderItemsMenu() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuItems)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    RenderNativeUIGrid(itemsMenu.grid, element.index, uiPointer);
    return 0;
}

NativePromptRuntime nativePrompt;
constexpr std::array<OptionSpec, 7> audioOptions{{
    {OptionKind::List, "UIOPTADEV", "UIOPTAODDSC", AudioOptionOffset(offsetof(S2AudioSettings, device)), OptionChoices::AudioOutputs}, // @text UIOPTADEV EN: "Audio Output Device" | JA: "音声出力デバイス" ; UIOPTAODDSC EN: "Select which device is used for audio." | JA: "音声に使うデバイスを選択します。"
    {OptionKind::Toggle, "UIOPTADLAN", "UIOPTALDSC", AudioOptionOffset(offsetof(S2AudioSettings, audioLanguage)), OptionChoices::AudioLanguages}, // @text UIOPTADLAN EN: "Audio Language" | JA: "音声言語" ; UIOPTALDSC EN: "Toggles the language used for audio." | JA: "音声言語の切り換えを行います。"
    {OptionKind::List, "UIOTTXTLNG", "UIOPTTLDSC", AudioOptionOffset(offsetof(S2AudioSettings, subtitleLanguage)), OptionChoices::SubtitleLanguages}, // @text UIOTTXTLNG EN: "Text Language" | JA: "字幕言語" ; UIOPTTLDSC EN: "Select the language used for subtitles & UI." | JA: "字幕とUIの言語を選択します。"
    {OptionKind::List, "UIOPTSBSEN", "UIOPTDTDSC", AudioOptionOffset(offsetof(S2AudioSettings, dialogueMode)), OptionChoices::Dialogue}, // @text UIOPTSBSEN EN: "Dialogue & Text" | JA: "声＆字幕" ; UIOPTDTDSC EN: "Select which mode dialogue & subtitles\nare presented in." | JA: "声と字幕の表示方法を選択します。"
    {OptionKind::Slider, "UIOPTSSVOL", "UIOPTSFXVDSC", AudioOptionOffset(offsetof(S2AudioSettings, sfxVolume)), {}, 0, 100}, // @text UIOPTSSVOL EN: "SFX Volume" | JA: "SE音量" ; UIOPTSFXVDSC EN: "Adjusts the volume of sound effects." | JA: "効果音の音量を調節します。"
    {OptionKind::Slider, "UIOPTSMVOL", "UIOPTMUSDSC", AudioOptionOffset(offsetof(S2AudioSettings, musicVolume)), {}, 0, 100}, // @text UIOPTSMVOL EN: "Music Volume" | JA: "BGM音量" ; UIOPTMUSDSC EN: "Adjusts the volume of music." | JA: "音楽の音量を調節します。"
    {OptionKind::Slider, "UIOPTSTVOL", "UIOPTSPEDSC", AudioOptionOffset(offsetof(S2AudioSettings, dialogueVolume)), {}, 0, 100}, // @text UIOPTSTVOL EN: "Speech Volume" | JA: "ボイス音量" ; UIOPTSPEDSC EN: "Adjusts the volume of speech." | JA: "声の音量を調節します。"
}};
constexpr std::array<OptionSpec, 31> controlsOptions{{
    {OptionKind::Toggle, "UISWAPTRIG", "UISWAPTRDS", offsetof(S2UserConfiguration280, swapTriggers), OptionChoices::OffOn, 0, 0, true}, // @text UISWAPTRIG EN: "Swap Look & Run" | JA: "走りボタン変更" ; UISWAPTRDS EN: "Swap the inputs used for look & run." | JA: "「走る」と「ズーム」の機能を入れ替えます。\n「はい」にすると、L2ボタンで走ります。"
    {OptionKind::Toggle, "UIOPTSINVY", "UIOPTLYADSC", offsetof(S2UserConfiguration280, invertLookY), OptionChoices::OffOn, 0, 0, true}, // @text UIOPTSINVY EN: "Invert Look Y Axis" | JA: "上下反転" ; UIOPTLYADSC EN: "Inverts the Y axis when looking around." | JA: "見回す操作を行う際に、\n入力に対して上下を反転させます。"
    {OptionKind::Heading, "UIKEYGENER", "", 0}, // @text EN: "Key Bindings - General" | JA: "キーアサイン（一般）"
    KeyOption("UIBUT2CONF", 0), KeyOption("UIBUT2CANC", 1), KeyOption("UIBUT2EVEN", 2), KeyOption("UIBUT2SYST", 3), // @text UIBUT2CONF EN: "Talk / Action" | JA: "話す/アクション" ; UIBUT2CANC EN: "Cancel / Nav Mark" | JA: "キャンセル/ナビマーク" ; UIBUT2EVEN EN: "Notebook / Action" | JA: "メモ帳/アクション" ; UIBUT2SYST EN: "Inventory / Menu" | JA: "アイテム/メニュー"
    KeyOption("UIBUTTFILT", 4), KeyOption("UIBUTTSHOT", 5), KeyOption("UIBUTTHELP", 7), // @text UIBUTTFILT EN: "Change Filter" | JA: "フィルタ変更" ; UIBUTTSHOT EN: "Snapshot" | JA: "スナップショット" ; UIBUTTHELP EN: "Pause/Help" | JA: "ポーズ/ヘルプ"
    {OptionKind::Heading, "UIKEYMOVCA", "", 0}, // @text EN: "Key Bindings - Movement / Camera" | JA: "キーアサイン（移動＆カメラ）"
    KeyOption("UIBUTTLOOK", 8), KeyOption("UIBUTTRUN0", 9), KeyOption("UIMOVEUP00", 14), KeyOption("UIMOVEDOWN", 15), // @text UIBUTTLOOK EN: "Look Zoom" | JA: "ズーム／ロックオン" ; UIBUTTRUN0 EN: "Run" | JA: "走る" ; UIMOVEUP00 EN: "Walk Forward" | JA: "前に進む" ; UIMOVEDOWN EN: "Turnaround" | JA: "振り向く"
    KeyOption("UIMOVELEFT", 16), KeyOption("UIMOVERIGH", 17), KeyOption("UILOOKUP00", 10), KeyOption("UILOOKDOWN", 11), // @text UIMOVELEFT EN: "Turn Left" | JA: "左に曲がる" ; UIMOVERIGH EN: "Turn Right" | JA: "右に曲がる" ; UILOOKUP00 EN: "Look Up" | JA: "上を見る" ; UILOOKDOWN EN: "Look Down" | JA: "下を見る"
    KeyOption("UILOOKLEFT", 12), KeyOption("UILOOKRIGH", 13), // @text UILOOKLEFT EN: "Look Left" | JA: "左を見る" ; UILOOKRIGH EN: "Look Right" | JA: "右を見る"
    {OptionKind::Heading, "UIKEYQTETT", "", 0}, // @text EN: "Key Bindings - QTE" | JA: "キーアサイン（ＱＴＥ）"
    KeyOption("UIQTE00001", 18), KeyOption("UIQTE00002", 19), KeyOption("UIQTE00003", 20), KeyOption("UIQTE00004", 21), // @text UIQTE00001 EN: "Quick Time Action 1" | JA: "クイックタイムアクション１" ; UIQTE00002 EN: "Quick Time Action 2" | JA: "クイックタイムアクション２" ; UIQTE00003 EN: "Quick Time Action 3" | JA: "クイックタイムアクション３" ; UIQTE00004 EN: "Quick Time Action 4" | JA: "クイックタイムアクション４"
    {OptionKind::Heading, "UIKEYFIGHT", "", 0}, // @text EN: "Key Bindings - Fight" | JA: "キーアサイン（バトル）"
    KeyOption("UIKICK0000", 22), KeyOption("UITHROW000", 23), KeyOption("UIPUNCH000", 24), KeyOption("UIBLOCK000", 25), // @text UIKICK0000 EN: "Kick" | JA: "キック" ; UITHROW000 EN: "Throw" | JA: "投げ" ; UIPUNCH000 EN: "Punch" | JA: "パンチ" ; UIBLOCK000 EN: "Guard" | JA: "防御"
}};
constexpr std::array<OptionSpec, 11> graphicsOptions{{
    {OptionKind::List, "UIOPTSFULL", "UIOPTDMDSC", offsetof(S2UserConfiguration280, windowMode), OptionChoices::WindowModes}, // @text UIOPTSFULL EN: "Display Mode" | JA: "フルスクリーンモード" ; UIOPTDMDSC EN: "Select the display mode." | JA: "表示モードを選択します。"
    {OptionKind::List, "UIOPTSMONI", "UIOPTMONSDC", offsetof(S2UserConfiguration280, displayOutput), OptionChoices::Displays}, // @text UIOPTSMONI EN: "Monitor" | JA: "モニター" ; UIOPTMONSDC EN: "Select which monitor the game should appear on." | JA: "ゲームを表示するモニターを選択します。"
    {OptionKind::List, "UIOPTSRESO", "UIOPTRESDSC", offsetof(S2UserConfiguration280, resolutionIndex), OptionChoices::Resolutions}, // @text UIOPTSRESO EN: "Resolution" | JA: "解像度" ; UIOPTRESDSC EN: "Select the games resolution." | JA: "ゲームの解像度を選択します。"
    {OptionKind::Slider, "UIOPTRESSCAL", "UIOPTRESSCDS", offsetof(S2UserConfiguration280, resolutionScale), {}, 40, 100}, // @text UIOPTRESSCAL EN: "Resolution Scale" | JA: "解像度スケール" ; UIOPTRESSCDS EN: "Scale resolution down to emulate original\nresolution and/or improve performance." | JA: "解像度を低くします。オリジナルの解像度でプレイしたい、\nまたはパフォーマンスを向上させたい場合に最適です。"
    {OptionKind::Slider, "UIOPTSUPSA", "UIOPTSUPSADS", offsetof(S2UserConfiguration280, superSampling), {}, 100, 200}, // @text UIOPTSUPSA EN: "Super Sampling" | JA: "スーパーサンプリング" ; UIOPTSUPSADS EN: "Enable super sampling. Higher values\nrequire more memory and may impact performance." | JA: "スーパーサンプリングを有効にします。高く設定するとより多くの\nメモリが必要となり、パフォーマンスに影響が出る場合があります。"
    {OptionKind::Toggle, "UIOPTSFXAA", "UIOPTSFXAA", offsetof(S2UserConfiguration280, fxaa), OptionChoices::OffOn, 0, 0, true}, // @text EN: "FXAA" | JA: "FXAA"
    {OptionKind::List, "UIOTGLBLOO", "UIOPBLMTOGDC", offsetof(S2UserConfiguration280, bloomMode), OptionChoices::Bloom}, // @text UIOTGLBLOO EN: "Bloom" | JA: "ブルーム" ; UIOPBLMTOGDC EN: "Toggle bloom post FX." | JA: "ブルームを「はい」にすることで、光源から光が\n溢れ出るようなエフェクトが有効になります。"
    {OptionKind::Slider, "UIGAMMGAMM", "UIOPBRGHTDSC", offsetof(S2UserConfiguration280, gamma), {}, -100, 100}, // @text UIGAMMGAMM EN: "Gamma" | JA: "ガンマ" ; UIOPBRGHTDSC EN: "Adjust gameplay brightness level." | JA: "鮮明度を調節します。"
    {OptionKind::Slider, "UIOPTSCONT", "UIOPTSCONDSC", offsetof(S2UserConfiguration280, contrast), {}, -100, 100}, // @text UIOPTSCONT EN: "Contrast" | JA: "コントラスト" ; UIOPTSCONDSC EN: "Adjust contrast level." | JA: "コントラストを調節します。"
    {OptionKind::Toggle, "UIOPTSSD43", "UIOPTASPDSC", offsetof(S2UserConfiguration280, aspectRatio), OptionChoices::Aspect}, // @text UIOPTSSD43 EN: "Aspect Ratio" | JA: "アスペクト比" ; UIOPTASPDSC EN: "Select the aspect ratio used during gameplay." | JA: "ゲームプレイ時の縦横比を選択します。"
    {OptionKind::Slider, "UIOPTSCALE", "UIOPUISCADSC", offsetof(S2UserConfiguration280, uiSafeArea), {}, 80, 100}, // @text UIOPTSCALE EN: "UI Display Area" | JA: "UIサイズ" ; UIOPUISCADSC EN: "Adjust the screen area that\nthe UI is contained within." | JA: "UIが含まれるスクリーンエリアを調節します。"
}};
OptionsRuntime options;
const OptionSpec *selectedOptionDescription{};
std::array<OptionChildRuntime, 3> optionChildren;
constexpr std::array<UIElement, 3> optionChildElements{UIElement::OptionsControls, UIElement::OptionsAudio, UIElement::OptionsGraphics};

#include "ui_gamehud.inl"
#include "ui_game_notebook.inl"

static_assert(sizeof(CollectionItemRow) == 0x20);
static_assert(sizeof(CollectionMapRow) == 0x18);
constexpr std::array<UIElement, 7> collectionChildren{static_cast<UIElement>(11), static_cast<UIElement>(12), static_cast<UIElement>(13),
    static_cast<UIElement>(14), static_cast<UIElement>(21), UIElement::PhotosSnapshot, static_cast<UIElement>(23)};
constexpr std::array<const char *, 7> collectionFiles{"/ui/gamemenu_New/SubMenu_Collection_Gacha.ui", "/ui/gamemenu_New/SubMenu_Collection_Maps.ui",
    "/ui/gamemenu_New/SubMenu_Collection_Pawnshop.ui", "/ui/gamemenu_New/SubMenu_Collection_Scrolls.ui", "/ui/gamemenu_New/SubMenu_Photos_Event.ui",
    "/ui/gamemenu_New/SubMenu_Photos_Snapshot.ui", "/ui/gamemenu_New/SubMenu_Photos_Secret.ui"};
constexpr std::array<const char *, 7> collectionHolders{"Submenu_Collection_Gacha_Panel_GridHolder", "Panel_Submenu_Collection_Maps_GridHolder",
    "Submenu_Collection::Pawnshop_GridHolder", "Submenu_Collection_Scrolls_Holder", "SubMenu_Photos_Event::Panel_GridHolder",
    "SubMenu_Photos_SnapShot::Panel_GridHolder", "SubMenu_Photos_Secret::Panel_GridHolder"};
constexpr std::array<const char *, 7> collectionRowRoots{"Submenu_Collection_Gacha::GridElementParent_#", "Submenu_Collection_Maps::GridElementParent_#",
    "Submenu_Collection_Pawnshop::GridElementParent_#", "Submenu_Collection_Scrolls::GridElementParent_#", "SubMenu_Photos_Event::GridElementParent_#",
    "SubMenu_Photos_SnapShot::GridElementParent_#", "SubMenu_Photos_Secret::GridElementParent_#"};
constexpr std::array<const char *, 29> collectionMapImages{"A000_00", "A000_01", "A000_02", "A000_03", "A000_04", "A000_05", "W000_01", "W000_02", "W000_03",
    "W000_04", "W000_05", "W000_00", "W000_06", "W000_07", "Q000_00", "Q000_01", "Q000_02", "Q000_03", "Q000_04", "Q000_05", "Q000_06", "Q000_07", "Q000_08",
    "Q000_09", "Q000_10", "Locked", "Aberdeen", "WanChai", "Kowloon"};
constexpr std::array<shen_name, 12> collectionPawnShops{SHEN_NAME(ARP0), SHEN_NAME(WSP0), SHEN_NAME(WKP0), SHEN_NAME(WKP1), SHEN_NAME(WTP0), SHEN_NAME(WTP1),
    SHEN_NAME(WTP2), SHEN_NAME(WTP3), SHEN_NAME(WTP4), SHEN_NAME(WRP0), SHEN_NAME(QBL0), SHEN_NAME(QBR0)};
constexpr std::array<shen_name, 48> collectionBioNames{SHEN_NAME(JOP_), SHEN_NAME(WON_), SHEN_NAME(SAM_), SHEN_NAME(LLY_), SHEN_NAME(CLJ_), SHEN_NAME(MM5_),
    SHEN_NAME(GOI_), SHEN_NAME(HTK_), SHEN_NAME(SYE_), SHEN_NAME(KEM_), SHEN_NAME(BKN_), SHEN_NAME(CHY_), SHEN_NAME(KEK_), SHEN_NAME(TNZ_), SHEN_NAME(SOS_),
    SHEN_NAME(TSK_), SHEN_NAME(KUN_), SHEN_NAME(SAI_), SHEN_NAME(SIZ_), SHEN_NAME(KNK_), SHEN_NAME(ANA_), SHEN_NAME(TKR_), SHEN_NAME(MYS_), SHEN_NAME(LKN_),
    SHEN_NAME(EDL_), SHEN_NAME(XGH_), SHEN_NAME(CKT_), SHEN_NAME(YAN_), SHEN_NAME(GAI_), SHEN_NAME(CHO_), SHEN_NAME(SF7_), SHEN_NAME(CEN_), SHEN_NAME(REN_),
    SHEN_NAME(AM1_), SHEN_NAME(AM2_), SHEN_NAME(AW3_), SHEN_NAME(SF6_), SHEN_NAME(SF3_), SHEN_NAME(SF4_), SHEN_NAME(SD4_), SHEN_NAME(SIN_), SHEN_NAME(SRY_),
    SHEN_NAME(TGY_), SHEN_NAME(YUA_), SHEN_NAME(RNP_), SHEN_NAME(AW2_), SHEN_NAME(DM2_), SHEN_NAME(CH8_)};
std::array<CollectionChildRuntime, 7> collectionChild;
d3t::UIImageWidget *collectionMapPreview;
std::array<std::int32_t, 29> collectionMapImageIndices{};
std::array<CollectionItemRow, 512> collectionGacha;
std::array<CollectionItemRow, 64> collectionEventPhotos;
std::array<CollectionItemRow, 12> collectionPawn;
std::array<CollectionMapRow, 28> collectionMaps;
std::array<WfMenuEntry, 25> collectionScrolls;
std::array<std::array<std::int32_t, 2>, 54> collectionSecretRows;
std::array<CollectionAlbum, 48> collectionAlbums;
std::array<std::uint32_t, 126> collectionViewerOrder;
std::uint32_t collectionViewerCount{}, collectionViewerIndex{};
UIElement collectionViewerReturn{UIElement::PhotosSnapshot};
bool collectionViewerWrap{}, collectionViewerBack{};
CollectionAuthoredLayout collectionViewerLayout, collectionPresentLayout;
std::array<std::array<char, 256>, 7> collectionViewerText;
std::vector<std::uint32_t> collectionPresentTextures;
std::uint32_t collectionPresentIndex{};
std::array<bool, 2> collectionTabFocus{true, true};
std::array<std::uint8_t, 126> collectionSnapshotPriority{};
float collectionSnapshotLoadingTime{};
std::uint32_t collectionSnapshotDeleteClick{std::numeric_limits<std::uint32_t>::max()};
UIElement collectionPresentReturn{static_cast<UIElement>(23)};
float collectionSnapshotDeleteAlpha = 0.2f;
CollectionTabsRuntime collectionTabs, photosTabs;

std::array<UIElementState *, 7> collectionMapLanguageElements{};
std::array<const d3t::UIImageDefinition *, 25> collectionLocalizedMaps{};
std::int32_t collectionMapLanguage{};
std::int32_t pendingCollectionMapLanguage{};
bool collectionMapLanguagePending{};
std::array<CollectionUnderlayAlpha, 3> collectionUnderlayAlpha;
bool collectionUnderlayHidden{};
std::uint32_t collectionPendingDelete{std::numeric_limits<std::uint32_t>::max()};
bool collectionPresentFramed{};
CollectionPawnFlyer collectionPawnFlyer;

// @verified - p
std::uint32_t AdvanceMoneyHudInit() {
    if (moneyHudState == 0) {
        moneyHudState = LoadUIFile("/ui/gamehud/GameHudMoney.ui", 33, &moneyHudRequest);
    }

    else if (moneyHudState == 1) {
        moneyHudState = GetNewUIFileInitFor(&moneyHudRequest, moneyHudConfiguration);
        if (moneyHudState == 2) {
            moneyHudLabel = UI_GetLabel(UI_FindViewLabel(moneyHudConfiguration.uiHandleIndex, UI_HUD_MONEY_LABEL));
            if (moneyHudLabel)
                moneyHudLabel->text = moneyHudText.data();
        }
    } else if (moneyHudState == 2) {
        moneyHudState = GetShouldInitUIFileFor(moneyHudConfiguration.uiHandleIndex);
    }
    return static_cast<std::uint32_t>(moneyHudState);
}

// @verified - p
std ::int64_t ReloadMoneyHud() {
    ResetUIViewSlot(moneyHudConfiguration.uiHandleIndex);
    moneyHudConfiguration.uiHandleIndex = -1;
    moneyHudState = 0;
    moneyHudRequest = 0;
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), moneyHudConfiguration.uiHandleIndex);
    moneyHudState = 2;
    return 2;
}
// @verified - p
std::int64_t GetMoneyHudInitState() {
    return static_cast<std::uint32_t>(moneyHudState);
}

// @verified - p
std::int64_t RenderMoneyHud() {
    if (moneyHudState != 3) {
        AdvanceMoneyHudInit();
        if (static_cast<std::uint32_t>(moneyHudState) - 2U > 1U)
            return 0;
    }
    RefreshNotebook();
    const std::int32_t amount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetCurrentCash()) + static_cast<std::uint32_t>(moneyHudCashAdjustment));
    std::snprintf(moneyHudText.data(), moneyHudText.size(), "%i", amount);
    uiUpdateWithColour(*GetNativeUiContext(), moneyHudConfiguration.uiHandleIndex);

    return 0;
}

UIButtonIconWidget *hudSnapshotCameraState{};

// @verified - p
std::uint32_t LoadHudSnapshot() {

    return ReadUIDefinitionFile(UIElement::HudSnapshot, "/ui/gamehud/GameHudSnapshot.ui", 1);
}

// @verified - p
std::int64_t BindHudSnapshot() {
    hudSnapshotCameraState =
        UI_FindViewIconWidget(g_uiElements[static_cast<std::size_t>(UIElement::HudSnapshot)].config.uiHandleIndex, UI_HUD_SNAPSHOT_SPRITE, 20);
    return 1;
}

// @verified - p
std::int64_t RenderHudSnapshot() {
    uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::HudSnapshot)].config.uiHandleIndex);
    return 0;
}

// @verified - p
void ConstructHudCameraState(std::uint32_t mode, UIButtonIconWidget *widget) {
    if (widget) {
        if ((IsSnapshotSaveInProgress() || mode == 2307) && GetQteControlState()) {
            widget->sprite = mode == 0 || mode == 1 ? 2306 : 2307;
            widget->inventoryIcon = 1;
            widget->fieldAC = 0;
        } else {
            widget->fieldAC = 1;
        }
    }
}
std::uint32_t LoadInformationBar() {
    informationBar.bound = false;
    return ReadUIDefinitionFile(UIElement::GameInfoBar, "/ui/gamemenu_New/GameMenu_InformationBar.ui", 1);
}

std::int64_t SetInformationBarTheme() {
    InformationBarRuntime &runtime = informationBar;
    const bool game = IsCameraIconHidden() != 0;
    d3t::UIWindow *const labels = runtime.windows[5];
    const d3t::UIWindow *const parent = runtime.windows[game ? 2 : 0];
    if (labels && parent)
        std::snprintf(labels->layout.parentId.data(), labels->layout.parentId.size(), "%s", parent->id.data());
    if (runtime.windows[1])
        runtime.windows[1]->alpha = game ? 0.0f : 1.0f;
    if (runtime.windows[3])
        runtime.windows[3]->alpha = game ? 1.0f : 0.0f;
    if (runtime.windows[4])
        runtime.windows[4]->alpha = game ? 1.0f : 0.0f;
    for (d3t::UILabelWidget *label : runtime.labels)
        if (label)
            label->strokeColour = game ? 0xc8000000U : 0xff000000U;
    return 1;
}

std::int64_t BindInformationBarLayout() {
    InformationBarRuntime &runtime = informationBar;
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::GameInfoBar)].config.uiHandleIndex;
    for (std::size_t index = 0; index < runtime.windows.size(); ++index)
        runtime.windows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_INFORMATION_PANELS[index]));
    for (std::size_t index = 0; index < runtime.labels.size(); ++index) {
        const std::int32_t label = UI_FindViewLabel(slot, UI_INFORMATION_LABELS[index]);
        runtime.labels[index] = label < 0 ? nullptr : &g_uiLabelWidgets[label];
        if (runtime.labels[index]) {
            runtime.labels[index]->text = runtime.text[index].data();
            runtime.labels[index]->token.alreadyLocalized = 1;
        }
    }
    for (std::size_t index = 0; index < runtime.images.size(); ++index) {
        const std::int32_t image = UI_FindViewImageWidget(slot, UI_INFORMATION_IMAGES[index]);
        runtime.images[index] = image < 0 ? nullptr : &g_uiImageWidgets[image];
        runtime.backgrounds[index] = UI_FindViewImage(slot, UI_INFORMATION_BACKGROUNDS[index]);
    }
    runtime.bound = true;
    return 1;
}

std::int64_t ResetInformationBar() {
    informationBar.windows.fill(nullptr);
    informationBar.labels.fill(nullptr);
    informationBar.images.fill(nullptr);
    informationBar.bound = false;
    return 1;
}

std::int64_t TickInformationBar() {
    CopyUIMessageBox(nullptr);
    return 0;
}

std::int64_t InformationBarEnabled() {
    return 1;
}

std::int64_t RenderInformationBar() {
    InformationBarRuntime &runtime = informationBar;
    if (runtime.current.hideTarget == 1 && runtime.windows[6])
        runtime.windows[6]->alpha = 0.0f;
    if (runtime.remaining > 0.0f) {
        runtime.remaining -= GetFrameTime();
        if (runtime.remaining <= 0.0f)
            RestorePreviousInformationBarMessage();
    }
    if (runtime.bound)
        uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::GameInfoBar)].config.uiHandleIndex);
    return 0;
}

std::uint32_t LoadGameHudTimer() {
    return ReadUIDefinitionFile(UIElement::GameHudTimer, "/ui/gamehud/GameHudTimer.ui", 0);
}

// @verified - p
std::int64_t BindGameHudTimer() {
    const std ::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::GameHudTimer)].config.uiHandleIndex;
    gameHudTimerPanel = UI_GetWindow(UI_FindViewWindow(slot, UI_HUD_TIMER_PANEL));
    gameHudTimerLabel = UI_GetLabel(UI_FindViewLabel(slot, UI_HUD_TIMER_LABEL));
    if (gameHudTimerLabel) {
        gameHudTimerLabel->text = gameHudTimerText.data();
        gameHudTimerLabel->token.alreadyLocalized = 1;
    }
    return 1;
}

std::int64_t ResetGameHudTimer() {
    gameHudTimerPanel = nullptr;
    gameHudTimerLabel = nullptr;
    return 0;
}

// @verified - p
std::int64_t ClearGameHudTimer() {
    if (gameHudTimerPanel)
        gameHudTimerPanel->alpha = 0.0f;
    gameHudTimerText[0] = '\0';
    return 0;
}

// @verified - p
std::int64_t RenderGameHudTimer() {
    uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::GameHudTimer)].config.uiHandleIndex);

    return 0;
}

std::int64_t GameHudTimerDebugGate() {
    return GetDebugForceUIEnabled(UIElement::GameHudTimer);
}

std::uint32_t LoadStreetFighterHud() {
    return ReadUIDefinitionFile(UIElement::StreetFighterHud, "/ui/gamehud/GameHudStreetFighter.ui", 0);
}

// @verified - p
std::int64_t BindStreetFighterHud() {

    const std ::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::StreetFighterHud)].config.uiHandleIndex;

    for (std::size_t index = 0; index < streetFighterHudWindows.size(); ++index)
        streetFighterHudWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_STREET_FIGHTER_PANELS[index]));
    for (std::size_t index = 0; index < streetFighterHudLabels.size(); ++index)
        streetFighterHudLabels[index] = UI_GetLabel(UI_FindViewLabel(slot, UI_STREET_FIGHTER_LABELS[index]));
    for (std::size_t index = 0; index < streetFighterHudLabels.size(); ++index) {
        if (streetFighterHudLabels[index]) {
            streetFighterHudLabels[index]->text = streetFighterHudText[index].data();
            streetFighterHudLabels[index]->token.alreadyLocalized = 1;
        }
    }
    if (streetFighterHudWindows[0])
        HideUIWindow(streetFighterHudWindows[0]);
    if (streetFighterHudWindows[1])
        streetFighterHudWindows[1]->alpha = 0.0f;
    return 1;
}

// @verified - p
std::int64_t ResetStreetFighterHud() {
    streetFighterHudWindows.fill(nullptr);
    streetFighterHudLabels.fill(nullptr);
    return 0;
}

// @verified - p
std::int64_t ClearStreetFighterHud() {
    for (d3t ::UILabelWidget *label : streetFighterHudLabels)
        if (label)
            label->colour &= 0x00ffffffU;
    if (streetFighterHudWindows[0])
        ShowUIWindow(streetFighterHudWindows[0]);
    if (streetFighterHudWindows[1])
        streetFighterHudWindows[1]->alpha = 0.0f;
    return 0;
}
// @verified - p
std::int64_t DestroyStreetFighterHud() {
    ClearStreetFighterHud();
    return 1;
}
// @verified - p
std::int64_t RenderStreetFighterHud() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement ::StreetFighterHud)];
    uiUpdateWithColour(*GetNativeUiContext(), element.config.uiHandleIndex);
    if (streetFighterHudWindows[0])
        HideUIWindow(streetFighterHudWindows[0]);
    ConstructNumStrikesLeftText(0);

    return 0;
}
std::int64_t StreetFighterHudInput() {
    return GetDebugForceUIEnabled(UIElement::StreetFighterHud);
}

std::uint32_t LoadDuckRaceHud() {
    return ReadUIDefinitionFile(UIElement::DuckRace, "/ui/gamehud/GameDuckRace.ui", 0);
}
// @verified - p
std ::int64_t BindDuckRaceHud() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::DuckRace)].config.uiHandleIndex;
    for (std::size_t index = 0; index < duckRaceHudWindows.size(); ++index)
        duckRaceHudWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_DUCK_RACE_PANELS[index]));
    for (std::size_t index = 0; index < duckRaceHudLabels.size(); ++index)
        duckRaceHudLabels[index] = UI_GetLabel(UI_FindViewLabel(slot, UI_DUCK_RACE_LABELS[index]));
    for (std::size_t index = 0; index < duckRaceHudLabels.size(); ++index) {
        if (duckRaceHudLabels[index]) {
            duckRaceHudLabels[index]->text = duckRaceHudText[index].data();
            duckRaceHudLabels[index]->token.alreadyLocalized = 1;
        }
    }
    for (std::size_t index = 0; index < duckRaceHudBowties.size(); ++index)
        duckRaceHudBowties[index] = UI_FindViewIconWidget(slot, UI_DUCK_RACE_BOWTIES[index], 20);
    return 1;
}
// @verified - p
std::int64_t ResetDuckRaceHud() {
    duckRaceHudWindows.fill(nullptr);
    duckRaceHudLabels.fill(nullptr);
    return 0;
}
// @verified - p
std::int64_t ClearDuckRaceHud() {
    for (d3t::UIWindow *window : duckRaceHudWindows)
        if (window)
            window->alpha = 0.0f;
    std::strcpy(duckRaceHudText[6].data(), "0");
    return 0;
}

// @verified - p
std::int64_t RenderDuckRaceHud() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::DuckRace)];
    uiUpdateWithColour(*GetNativeUiContext(), element.config.uiHandleIndex);

    return 0;
}
std::int64_t DuckRaceHudInput() {
    return GetDebugForceUIEnabled(UIElement::DuckRace);
}

std::int64_t TickGameBackground() {
    ShowElementByIndex(UIElement::GameInfoBar);
    ShowElementByIndex(UIElement::GameHud);
    return 0;
}

std::int64_t DestroyGameBackground() {
    DestroyElementByIndex(UIElement::GameInfoBar);
    DestroyElementByIndex(UIElement::GameHud);
    return 1;
}

// @verified - p
std::int64_t DrawScreenFilm() {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::GameBackground)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    return 0;
}

std::int64_t ResetGameBackgroundNullsub() {
    return 0;
}


std::uint32_t LoadLoadSave() {
    return ReadUIDefinitionFile(UIElement::LoadSave, "/ui/mainmenu_new/LoadScreen.ui", 1);
}

std::uint32_t LoadCutsceneSkip() {
    return ReadUIDefinitionFile(UIElement::CutsceneSkip, "/ui/gamehud/GameHudSkip.ui", 1);
}

std::int64_t RenderCutsceneSkip() {
    if (!drawingUI)
        return 0;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::CutsceneSkip)];
    AuthoredMenuLayout &layout = cutsceneSkipLayout;
    d3t::ResolveUIDefinitionLayout(
        definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels, {}, GetUILayoutPaddingScale());
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index)
        DrawNativeMenuObject(definition, definition.textureObjects[index], layout.objects[index], 1.0f);
    for (std::size_t index = 0; index < definition.labels.size(); ++index) {
        const d3t::UILabelDefinition &label = definition.labels[index];
        DrawMovesLabel(label, layout.labels[index], label.token.empty() ? std::string_view(label.text) : GetLocalizedSceneText(label.token), 1.0f, &definition);
    }
    return 0;
}

std::uint32_t LoadQteMinigame() {
    return ReadUIDefinitionFile(UIElement::QteMinigame, "/ui/QTEMinigame/QTEMinigame.ui", 0);
}

std::int64_t BindQteMinigame() {
    QteMinigameRuntime &runtime = qteMinigame;
    const d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::QteMinigame)];
    runtime.panels.fill(-1);
    runtime.directions.fill(-1);
    for (std::size_t slot = 0; slot < runtime.panels.size(); ++slot)
        for (std::size_t index = 0; index < definition.panels.size(); ++index)
            if (definition.panels[index].id == QTE_MINIGAME_PANEL_IDS[slot])
                runtime.panels[slot] = static_cast<std::int32_t>(index);
    for (std::size_t slot = 0; slot < runtime.directions.size(); ++slot)
        for (std::size_t index = 0; index < definition.textureObjects.size(); ++index)
            if (definition.textureObjects[index].id == QTE_MINIGAME_DIRECTION_IDS[slot])
                runtime.directions[slot] = static_cast<std::int32_t>(index);
    BindAuthoredMenuLayout(definition, runtime.layout);
    return 1;
}

std::int64_t ResetQteMinigame() {
    qteMinigame.panels.fill(-1);
    return 0;
}
std::int64_t ClearQteMinigame() {
    qteMinigame.opacity.fill(0.0f);
    qteMinigame.model = nullptr;
    return 0;
}
std::int64_t DestroyQteMinigame() {
    ClearQteMinigame();
    return 1;
}
std::int64_t QteMinigameInput() {
    return GetDebugForceUIEnabled(UIElement::QteMinigame);
}

float QteMinigamePanelOpacity(std::int32_t panel) {
    float opacity = 1.0f;
    while (panel >= 0) {
        for (std::size_t slot = 0; slot < qteMinigame.panels.size(); ++slot)
            if (qteMinigame.panels[slot] == panel)
                opacity *= qteMinigame.opacity[slot];
        panel = qteMinigame.layout.parents[panel];
    }
    return opacity;
}

std::int64_t RenderQteMinigame() {
    QteMinigameRuntime &runtime = qteMinigame;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::QteMinigame)];
    const bool keyboard = GetIsKeyboardActive() == 1;
    for (std::size_t slot = 0; slot < runtime.directions.size(); ++slot) {
        const std::int32_t index = runtime.directions[slot];
        if (index < 0)
            continue;
        d3t::UITextureObjectDefinition &object = definition.textureObjects[index];
        const std::uint32_t alpha = (slot < 4) == keyboard ? 0xff000000U : 0;
        object.fontColour = (object.fontColour & 0x00ffffffU) | alpha;
        object.colour = (object.colour & 0x00ffffffU) | alpha;
    }
    runtime.opacity.fill(0.0f);
    if (runtime.model) {
        for (std::uint16_t slot = 0; slot < 4; ++slot) {
            const MT7Node *const node = GetChildObjectByIndex(*runtime.model, &runtime.model->nodes[0], 183 + slot);
            if (node && node->position.y > 0.001f)
                runtime.opacity[slot] = 1.0f;
        }
        const MT7Node *const direction = GetChildObjectByIndex(*runtime.model, &runtime.model->nodes[0], 187);
        if (direction && direction->position.y > 0.001f) {
            const std::int32_t yaw = direction->rotation.z;
            runtime.opacity[yaw == 49152 ? 7 : yaw == 16384 ? 4 : yaw == 32768 ? 6 : 5] = 1.0f;
        }
    }
    if (runtime.titleButtons)
        for (std::size_t slot = 0; slot < 3; ++slot)
            if (runtime.titleButtons[slot])
                runtime.opacity[8 + slot] = 1.0f;
    AuthoredMenuLayout &layout = runtime.layout;
    d3t::ResolveUIDefinitionLayout(
        definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels, {}, GetUILayoutPaddingScale());
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index)
        if (drawingUI)
            DrawNativeMenuObject(definition, definition.textureObjects[index], layout.objects[index], QteMinigamePanelOpacity(layout.objectPanels[index]));
    for (std::size_t index = 0; index < definition.labels.size(); ++index) {
        const d3t::UILabelDefinition &label = definition.labels[index];
        DrawMovesLabel(label, layout.labels[index], label.token.empty() ? std::string_view(label.text) : GetLocalizedSceneText(label.token),
            QteMinigamePanelOpacity(layout.labelPanels[index]), &definition);
    }
    return 0;
}

std::uint32_t LoadActionSelector() {
    return ReadUIDefinitionFile(UIElement::ActionSelector, "/ui/actionselector/ActionSelector.ui", 0);
}

void PublishActionSelectorText(std::size_t slot, std::string_view text) {
    std::array<char, 256> &buffer = actionSelectorText[slot];
    if (!text.empty())
        std::memcpy(buffer.data(), text.data(), text.size());
    buffer[text.size()] = '\0';
    dialogueSelectionPrompt.labels[slot] = {buffer.data(), text.size()};
}

float ActionSelectorPosition() {
    return actionSelectorMotionTransition < 0 ? 0.0f : GetUITransitionValue(actionSelectorMotionTransition);
}

std::int64_t StartActionSelector() {
    actionSelectorInput = 13;
    actionSelectorSelectedButton = -1;
    actionSelectorButtonFrame = 0;
    actionSelectorSelectionLatched = false;
    if (actionSelectorMotionTransition >= 0) {
        UIAnim_Start(actionSelectorMotionTransition, 0.0f, 1.0f, 0.5f, 0.0f);
        UIAnim_Start(actionSelectorPulseTransition, 1.0f, 0.1f, 0.2f, 0.01f);
    }
    AicaExecuteWrapper(3);
    return 1;
}

std::int64_t ActionSelectorDestroy() {
    const float position = ActionSelectorPosition();
    if (actionSelectorMotionTransition >= 0) {
        ResetUITransition(actionSelectorPulseTransition);
        if (position > 0.0f)
            UIAnim_Start(actionSelectorMotionTransition, position, 0.0f, position * 0.5f, position >= 1.0f ? 0.45f : 0.0f);
    }
    if (numericActionSelector.mode == 1) {
        SetMoneyHudCashAdjustment(0);
        DestroyElementByIndex(UIElement::GameMessage);
    }
    numericActionSelector.mode = 0;
    return 1;
}

void SelectActionSelectorButton(std::int32_t slot) {
    if (actionSelectorSelectionLatched && numericActionSelector.mode == 0)
        return;
    actionSelectorSelectedButton = slot;
    actionSelectorButtonFrame = 1;
    actionSelectorSelectionLatched = true;
    UIAnim_Start(actionSelectorButtonTransition, std::bit_cast<float>(1), std::bit_cast<float>(0), 0.001f, 0.15f);
    g_uiAnimSlots[static_cast<std::size_t>(actionSelectorButtonTransition)].interpolation = 3;
    AicaExecuteWrapper(3);
}

bool IsActionSelectorInputEnabled(std::size_t slot) {
    return numericActionSelector.mode == 1 ? numericActionSelector.choices[slot] != -1 : dialogueSelectionPrompt.flagIds[slot] != 0xffff;
}

std::int64_t ProcessActionSelectorInput() {
    const LegacyInputSlot &input = g_inputBuffer[0];
    const std::uint16_t repeated = static_cast<std::uint16_t>(~actionSelectorPreviousInput & input.repeatedPresses);
    actionSelectorPreviousInput = input.pressedPriority;
    if (ActionSelectorPosition() < 1.0f)
        return 1;
    const std::uint16_t buttons = repeated | input.pressedPriority;

    if ((buttons & 0x4000U) && IsActionSelectorInputEnabled(1)) {
        SelectActionSelectorButton(1);
        if (numericActionSelector.mode == 1) {
            numericActionSelector.amount = std::min(numericActionSelector.amount + numericActionSelector.step, numericActionSelector.maximum);
            UpdateGameMessageMoney(numericActionSelector.amount);
        }
        actionSelectorInput = 6;
    }
    if ((buttons & 0x8000U) && IsActionSelectorInputEnabled(2)) {
        SelectActionSelectorButton(2);
        if (numericActionSelector.mode == 1) {
            numericActionSelector.amount = std::max(numericActionSelector.amount - numericActionSelector.step, 0);
            UpdateGameMessageMoney(numericActionSelector.amount);
        }
        actionSelectorInput = 2;
    }
    if ((buttons & 0x1000U) && IsActionSelectorInputEnabled(0)) {
        SelectActionSelectorButton(0);
        if (numericActionSelector.mode == 1) {
            numericActionSelector.amount = numericActionSelector.maximum;
            UpdateGameMessageMoney(numericActionSelector.amount);
        }
        actionSelectorInput = 0;
    }
    if ((buttons & 0x2000U) && IsActionSelectorInputEnabled(3)) {
        SelectActionSelectorButton(3);
        if (numericActionSelector.mode == 1) {
            numericActionSelector.amount = 0;
            UpdateGameMessageMoney(0);
        }
        actionSelectorInput = 4;
    }
    if (input.pressedPriority & 0x0200U) {
        if (numericActionSelector.mode == 1) {
            numericActionSelector.amount = 0;
            UpdateGameMessageMoney(0);
        }
        actionSelectorInput = 9;
    }
    if (input.pressedPriority & 0x0400U)
        actionSelectorInput = 8;
    if (input.pressedPriority & 0x0002U)
        actionSelectorInput = 11;
    return 1;
}

std::int32_t ActionSelectorChoiceForPanel(const d3t::UIDefinition &definition, std::string_view id) {
    constexpr std::array<std::string_view, 4> names{"ActionSelector::Up", "ActionSelector::Left", "ActionSelector::Right", "ActionSelector::Down"};
    for (std::size_t depth = 0; depth <= definition.panels.size(); ++depth) {
        for (std::size_t slot = 0; slot < names.size(); ++slot)
            if (id.starts_with(names[slot]))
                return static_cast<std::int32_t>(slot);
        const std::int32_t panel = MenuPanelIndex(definition, id);
        if (panel < 0 || definition.panels[panel].parentId.empty())
            break;
        id = definition.panels[panel].parentId;
    }
    return -1;
}

std::int64_t BindActionSelectorLayout() {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ActionSelector)];
    BindAuthoredMenuLayout(definition, actionSelectorLayout);
    const std::int32_t mainPanel = MenuPanelIndex(definition, "ActionSelector::MainPanel");
    actionSelectorMainPanel = mainPanel < 0 ? nullptr : &definition.panels[mainPanel];
    actionSelectorOutPanel = MenuPanelIndex(definition, "ActionSelector::MainPanel_OUT");
    actionSelectorInPanel = MenuPanelIndex(definition, "ActionSelector::MainPanel_IN");
    actionSelectorPulseTransition = UIAnimAllocSlot();
    g_uiAnimSlots[static_cast<std::size_t>(actionSelectorPulseTransition)].loopMode = 2;
    actionSelectorButtonTransition = UIAnimAllocSlot();
    actionSelectorMotionTransition = UIAnimAllocSlot();
    actionSelectorPanelChoices.resize(definition.panels.size());
    actionSelectorObjectChoices.resize(definition.textureObjects.size());
    actionSelectorLabelChoices.resize(definition.labels.size());
    for (std::size_t index = 0; index < definition.panels.size(); ++index)
        actionSelectorPanelChoices[index] = static_cast<std::int8_t>(ActionSelectorChoiceForPanel(definition, definition.panels[index].id));
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        const d3t::UITextureObjectDefinition &object = definition.textureObjects[index];
        actionSelectorObjectChoices[index] = static_cast<std::int8_t>(ActionSelectorChoiceForPanel(definition, object.id));
    }
    constexpr std::array<std::string_view, 4> labels{
        "ActionSelector::UpButtonLabel", "ActionSelector::LeftButtonLabel", "ActionSelector::RightButtonLabel", "ActionSelector::DownButtonLabel"};
    for (std::size_t index = 0; index < definition.labels.size(); ++index) {
        actionSelectorLabelChoices[index] = -1;
        for (std::size_t slot = 0; slot < labels.size(); ++slot)
            if (definition.labels[index].id == labels[slot])
                actionSelectorLabelChoices[index] = static_cast<std::int8_t>(slot);
    }
    if (g_uiElements[static_cast<std::size_t>(UIElement::ActionSelector)].isVisible)
        StartActionSelector();
    return 1;
}

std::int64_t ActionSelectorPressed() {
    if (!uiPointer.leftClicks)
        return ProcessActionSelectorInput();
    const float x = uiPointer.position.x * 1920.0f / GetUIViewportWidth();
    constexpr std::array<std::uint16_t, 4> masks{0x1000, 0x4000, 0x8000, 0x2000};
    for (std::size_t index = 0; index < actionSelectorPanelChoices.size(); ++index) {
        const std::int32_t slot = actionSelectorPanelChoices[index];
        if (slot < 0)
            continue;
        if (numericActionSelector.mode != 1 && dialogueSelectionPrompt.flagIds[static_cast<std::size_t>(slot)] == 0xffff)
            continue;
        const d3t::UIRect &rect = actionSelectorLayout.panels[index];
        if (x >= rect.x && x < rect.x + rect.width && uiPointer.position.y >= rect.y && uiPointer.position.y < rect.y + rect.height) {
            InjectInputButtons(masks[static_cast<std::size_t>(slot)]);
            break;
        }
    }
    return ProcessActionSelectorInput();
}

std::int64_t DrawActionSelector() {
    if (!GetItemSelectionMode(2) && !GetItemSelectionMode(6) && !(GetItemSelectionMode(4) && (moneyIconFlags & 8)))
        return 0;
    if (g_uiElements[static_cast<std::size_t>(UIElement::ActionSelector)].config.init != 3)
        return 0;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::ActionSelector)];
    AuthoredMenuLayout &layout = actionSelectorLayout;
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, {}, {}, {}, {}, {}, GetUILayoutPaddingScale());
    const float originalX = actionSelectorMainPanel ? actionSelectorMainPanel->x.offset : 0.0f;
    const float originalY = actionSelectorMainPanel ? actionSelectorMainPanel->y.offset : 0.0f;
    if (actionSelectorMainPanel && actionSelectorOutPanel >= 0 && actionSelectorInPanel >= 0) {
        const d3t::UIRect &from = layout.panels[actionSelectorOutPanel];
        const d3t::UIRect &to = layout.panels[actionSelectorInPanel];
        actionSelectorMainPanel->x.offset = (to.x - from.x) * ActionSelectorPosition();
        actionSelectorMainPanel->y.offset = (to.y - from.y) * ActionSelectorPosition();
    }
    d3t::ResolveUIDefinitionLayout(
        definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels, {}, GetUILayoutPaddingScale());
    if (actionSelectorMainPanel) {
        actionSelectorMainPanel->x.offset = originalX;
        actionSelectorMainPanel->y.offset = originalY;
    }
    const bool numeric = numericActionSelector.mode == 1;
    const std::int32_t buttonFrame = actionSelectorSelectedButton < 0 ? 0 : std::bit_cast<std::int32_t>(GetUITransitionValue(actionSelectorButtonTransition));
    if (!numeric && actionSelectorSelectedButton >= 0 && buttonFrame != actionSelectorButtonFrame) {
        UIAnim_Start(actionSelectorButtonTransition, std::bit_cast<float>(buttonFrame), std::bit_cast<float>(actionSelectorButtonFrame), 0.001f, 0.15f);
        g_uiAnimSlots[static_cast<std::size_t>(actionSelectorButtonTransition)].interpolation = 3;
    }
    actionSelectorButtonFrame = buttonFrame;
    constexpr std::array<std::int32_t, 5> markerFrames{45, 42, 55, 56, 62};
    std::size_t objectIndex = 0, labelIndex = 0;
    for (std::size_t panel = 0; panel < definition.panels.size(); ++panel) {
        while (objectIndex < definition.textureObjects.size() && layout.objectPanels[objectIndex] == static_cast<std::int32_t>(panel)) {
            const std::size_t index = objectIndex++;
            d3t::UITextureObjectDefinition &object = definition.textureObjects[index];
            const std::int32_t slot = actionSelectorObjectChoices[index];
            if (slot >= 0 && !numeric && static_cast<std::int16_t>(dialogueSelectionPrompt.flagIds[slot]) <= -1)
                continue;
            if (object.id == "ActionSelector::Phoenix" && (numeric || dialogueSelectionPrompt.flagIds[0] != 0xffff))
                continue;
            if (object.id == "ActionSelector::CentreIcon")
                object.frameIndex = markerFrames[ctrlSpeakMarkerGlyphIndex];
            if (object.kind == d3t::UITextureObjectKind::Sprite && object.imageName == "ActionSelector::Button")
                object.frameIndex = slot == actionSelectorSelectedButton ? buttonFrame : 0;
            const float opacity = object.id == "ActionSelector::CentreIcon" ? GetUITransitionValue(actionSelectorPulseTransition) : 1.0f;
            if (drawingUI)
                DrawNativeMenuObject(definition, object, layout.objects[index], opacity);
        }
        while (labelIndex < definition.labels.size() && layout.labelPanels[labelIndex] == static_cast<std::int32_t>(panel)) {
            const std::size_t index = labelIndex++;
            const d3t::UILabelDefinition &label = definition.labels[index];
            const std::int32_t slot = actionSelectorLabelChoices[index];
            if (slot < 0 || (!numeric && static_cast<std::int16_t>(dialogueSelectionPrompt.flagIds[slot]) <= -1))
                continue;
            const std::string_view text = numeric ? std::string_view(numericActionSelector.labels[slot].data()) : dialogueSelectionPrompt.labels[slot];
            DrawMovesLabel(label, layout.labels[index], text, 1.0f, &definition);
        }
    }
    if (numeric) {
        SetMoneyHudCashAdjustment(-numericActionSelector.amount);
        EnableElement(UIElement::GameMessage);
    }
    return 0;
}

void PrepareSharedItemImages() {
    const auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::SharedItems)];
    for (std::size_t index = 0; index < sharedItemImages.size(); ++index)
        sharedItemImages[index] = definition.findImage(sharedItemImageNames[index]);
}

bool ResolveSharedItemImage(std::uint16_t icon, d3t::UISharedSpriteImage &image, bool silhouette) {
    const auto bank = static_cast<std::size_t>((icon >> 8U) + (silhouette ? 16U : 0U));
    if (bank >= sharedItemImages.size() || !sharedItemImages[bank])
        return false;
    image.textureHandle = sharedItemImages[bank]->textureHandle;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    if (!d3t::GetTexture2DDimensions(image.textureHandle, &width, &height))
        return false;
    const auto uv = GetCommonIconRect(icon);
    // DrawCommonIcon uses the common bank's authored UV table, but the HD
    // SharedItems image, not the Dreamcast COMMON.SPR texture.
    image.source = {uv[0] * 0.125f * width, height - uv[3] * 0.125f * height, (uv[2] - uv[0]) * 0.125f * width, (uv[3] - uv[1]) * 0.125f * height};
    return true;
}

float GameMessagePosition() {
    return GetUITransitionValue(gameMessageTransition);
}

void ApplyGameMessageContent() {
    auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::GameMessage)];
    for (auto &object : definition.textureObjects) {
        if (object.id == "UIGameMessage::PlaceholderSprite") {
            object.hidden = gameMessageIcon == -1;
            if (!object.hidden) {
                object.frameIndex = gameMessageIcon;
                object.useShared = true;
            }
            object.colour = (object.colour & 0x00ffffffU) | (gameMessageMode == 0 ? 0xff000000U : 0U);
        } else if (object.id == "UIGameMessage::Money") {
            object.hidden = gameMessageMode != 1;
        } else if (object.id == "UIGameMessage::TokenImage") {
            object.hidden = gameMessageMode == 0 || gameMessageMode == 1;
        }
    }
}

void ApplyGameMessagePosition(float position);

std::int64_t PrepareGameMessageLayout() {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::GameMessage)];
    BindAuthoredMenuLayout(definition, gameMessageLayout);
    gameMessageMainPanel = MenuPanelIndex(definition, "UIGameMessage::MainPanelAnim");
    gameMessageStartPanel = MenuPanelIndex(definition, "UIGameMessage::MainPanelAnimStart");
    gameMessageEndPanel = MenuPanelIndex(definition, "UIGameMessage::MainPanelAnimEnd");
    gameMessageTransition = UIAnimAllocSlot();
    gameMessagePrepared = true;
    ApplyGameMessageContent();
    ApplyGameMessagePosition(0.0f);
    return 1;
}

std::int64_t GameMessageSelected() {
    if (gameMessageTransition != -1) {
        UIAnim_Start(gameMessageTransition, 0.0f, 1.0f, 0.5f, gameMessageNextDelay);
        gameMessageNextDelay = 0.0f;
        AicaExecuteWrapper(3);
    }
    return 1;
}

std::int64_t GameMessageDestroy() {
    if (gameMessageTransition != -1) {
        UIAnim_Start(gameMessageTransition, 1.0f, 0.0f, 0.5f, 0.0f);
        AicaExecuteWrapper(3);
    }
    return 1;
}

std::int64_t GameMessageDebugGate() {
    return GetDebugForceUIEnabled(UIElement::GameMessage);
}

std::int64_t ResetGameMessageBindings() {
    gameMessageMainPanel = gameMessageStartPanel = gameMessageEndPanel = -1;
    gameMessagePrepared = false;
    return 0;
}

void FormatMoneyAmount(char *output, std::int32_t value, bool cash) {
    std::array<char, 16> digits{};
    const auto magnitude = static_cast<std::uint32_t>(value < 0 ? -static_cast<std::int64_t>(value) : value);
    const auto converted = std::to_chars(digits.data(), digits.data() + digits.size(), magnitude);
    const auto count = static_cast<std::size_t>(converted.ptr - digits.data());
    const bool suffixCurrency = uiFontLanguage == 2 || uiFontLanguage == 3;
    if (value < 0)
        *output++ = '-';
    if (cash && !suffixCurrency) {
        std::memcpy(output, "HK$ ", 4);
        output += 4;
    }
    for (std::size_t index = 0; index < count; ++index) {
        if (index && (count - index) % 3 == 0)
            *output++ = suffixCurrency ? ' ' : ',';
        *output++ = digits[index];
    }
    if (cash && suffixCurrency) {
        std::memcpy(output, " HK$", 4);
        output += 4;
    }
    *output = '\0';
}

void FormatGameMessageAmount(std::int32_t value, bool cash) {
    FormatMoneyAmount(gameMessageText.data(), value, cash);
}

void ApplyGameMessagePosition(float position) {
    const std::size_t index = static_cast<std::size_t>(UIElement::GameMessage);
    if (!gameMessagePrepared)
        return;
    d3t::UIDefinition &definition = uiDefinitions[index];
    AuthoredMenuLayout &layout = gameMessageLayout;
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, {}, {}, {}, {});
    d3t::UIPanelBoundsOverride override{};
    std::span<const d3t::UIPanelBoundsOverride> overrides{};
    if (gameMessageMainPanel >= 0 && gameMessageStartPanel >= 0 && gameMessageEndPanel >= 0) {
        const d3t::UIRect &from = layout.panels[gameMessageStartPanel];
        const d3t::UIRect &to = layout.panels[gameMessageEndPanel];
        const float inverse = 1.0f - position;
        override = {.panel = static_cast<std::size_t>(gameMessageMainPanel),
            .bounds = {to.x * position + (-from.width) * inverse, from.y * inverse + to.y * position, to.width * position + from.width * inverse,
                from.height * inverse + to.height * position}};
        overrides = std::span{&override, 1};
    }
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels, overrides,
        GetUILayoutPaddingScale());
}

std::int64_t DrawGameMessage() {
    if (!gameMessagePrepared)
        return 0;
    ApplyGameMessagePosition(GameMessagePosition());
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::GameMessage)];
    AuthoredMenuLayout &layout = gameMessageLayout;
    if (!drawingUI)
        return 0;
    for (std::size_t object = 0; object < definition.textureObjects.size(); ++object) {
        const d3t::UIResolvedTextureObject &placement = layout.objects[object];
        const auto &image = definition.textureObjects[object];
        d3t::UISharedSpriteImage shared{};
        if (image.useShared && !ResolveSharedItemImage(static_cast<std::uint16_t>(image.frameIndex), shared))
            continue;
        d3t::DrawUITextureObject(definition, image, placement, 1.0f, image.useShared ? &shared : nullptr);
    }
    const std::string_view text(gameMessageText.data());
    if (text.empty())
        return 0;
    const float sx = static_cast<float>(d3t::g_originalScreenWidth) / 1920.0f;
    const float sy = static_cast<float>(d3t::g_originalScreenHeight) / 1080.0f;
    const UIFontSelection font{uiFontLanguage, 0, 3}; // authored BL.
    const UIFontSelection strokeFont{uiFontLanguage, 1, 3};
    for (std::size_t label = 0; label < layout.labels.size(); ++label) {
        const auto &authored = definition.labels[label];
        const auto *panel = definition.findPanel(authored.panelId);
        if (!panel)
            continue;
        const d3t::UIResolvedLabel &placement = layout.labels[label];
        const d3t::UIRect bounds{placement.bounds.x * sx, placement.bounds.y * sy, placement.bounds.width * sx, placement.bounds.height * sy};
        const auto fit = FitSubtitleLabel(font, text, bounds, panel->width * authored.width / 1920.0f, panel->height * authored.height / 1080.0f, true);
        if (fit.height <= 0.0f)
            continue;
        const std::array<std::int32_t, 4> clip{static_cast<std::int32_t>(placement.clip.x * sx), static_cast<std::int32_t>(placement.clip.y * sy),
            static_cast<std::int32_t>((placement.clip.x + placement.clip.width) * sx),
            static_cast<std::int32_t>((placement.clip.y + placement.clip.height) * sy)};
        const auto drawLine = [&](std::string_view line, const d3t::UIRect &rectangle) {
            const float width = GetUIFontTextWidth(font, fit.height, line);
            const float left = rectangle.x + (rectangle.width - width) * 0.5f;
            const float top = rectangle.y + (rectangle.height - fit.height) * 0.5f;
            DrawUIFontText(strokeFont, fit.height, line, left, top, 0xff000000U, clip);
            DrawUIFontText(font, fit.height, line, left, top, 0xffffffffU, clip);
        };
        if (fit.split == std::string_view::npos) {
            drawLine(text, bounds);
        } else {
            auto half = bounds;
            half.height *= 0.5f;
            drawLine(text.substr(0, fit.split), half);
            half.y += half.height;
            drawLine(text.substr(fit.split + 1), half);
        }
    }
    return 0;
}

} // namespace

namespace ui_detail {

std::int32_t helpWidgetRunCount{}, helpWidgetLineCount{}, helpWidgetIconRunCount{}, helpWidgetBoxCount{};
std::uint8_t helpWidgetRelayout{}, helpWidgetBoxesLocked{};
std::array<HelpWidgetRun, 64> helpWidgetRuns{};
std::array<HelpWidgetBox, 4> helpWidgetBoxes{};
std::int32_t helpWidgetIconCount{};
std::array<UIButtonIconWidget, 32> helpWidgetIcons{};
std::array<std::int32_t, 32> helpWidgetIconActions{};
d3t::UIRect helpWidgetPauseRect{}, helpWidgetLocationRect{};
std::array<char, 64> helpWidgetPauseText{}, helpWidgetLocationText{};
std::int32_t helpWidgetMode{}, helpWidgetArcadeContext{}, helpWidgetBlinkTransition{};
std::uint8_t helpWidgetPeekHidden{};
std::int32_t helpWidgetKeyboardType{};
std::array<d3t::UIWindow *, 2> helpWidgetPanels{};
nk_panel helpWidgetPanel{};

PawnFlyerRuntime pawnSetFlyer, pawnItemFlyer;
std::int32_t pawnSetFlyerIndex{}, pawnItemFlyerIndex{}, pawnSetFlyerInspection{};
char pawnSetFlyerShopLabel[256]{}, pawnItemFlyerShopLabel[256]{};

void InitializeUIElements() {
    for (std::atomic<bool> &ready : uiDefinitionReady)
        ready.store(false, std::memory_order_relaxed);
    for (std::size_t index = 0; index < g_uiElements.size(); ++index) {
        UIElementState &element = g_uiElements[index];
        element = {};
        element.name = "Unknown";
        element.index = static_cast<std::int32_t>(index);
        element.config.uiHandleIndex = -1;
    }
    UIElementState &screenFade = g_uiElements[static_cast<std::size_t>(UIElement::GameScreenFade)];
    screenFade.name = "Game Screen Fade";
    screenFade.isPressed = ScreenFadeAcceptsInput;
    screenFade.render = RenderScreenFilm;
    gameScreenFadeColour = 0;

    UIElementState &splash = g_uiElements[static_cast<std::size_t>(UIElement::Splash)];
    splash.name = "Splash";
    splash.parseDefinitionFromFile = LoadSplash;

    splash.isSelected = SplashSelected;
    splash.reset = ResetSplash;
    splash.destroy = DestroySplash;
    splash.render = SplashRender;
    splash.isPressed = HandleSplashInput;
    UIElementState &pressStart = g_uiElements[static_cast<std::size_t>(UIElement::PressStart)];
    pressStart.name = "Press Start";
    pressStart.parseDefinitionFromFile = LoadPressStart;
    pressStart.isSelected = PressStartSelected;
    pressStart.isPressed = PressStartInput;
    pressStart.render = PressStartRender;
    pressStart.tick = TickPressStart;
    UIElementState &bigKanjiText = g_uiElements[static_cast<std::size_t>(UIElement::BigKanjiText)];
    bigKanjiText.name = "Game HUD Big Kanji Text";
    bigKanjiText.parseDefinitionFromFile = LoadBigKanjiText;
    bigKanjiText.isPressed = BigKanjiTextEnabled;
    bigKanjiText.render = RenderBigKanjiText;
    bigKanjiText.isSelected = BindBigKanjiText;
    bigKanjiText.reset = ResetBigKanjiText;
    bigKanjiText.tick = BigKanjiTextTickNullsub;
    bigKanjiText.destroy = BigKanjiTextEnabled;
    auto &gameMenuOverlay = g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)];
    gameMenuOverlay.name = "Game Menu";
    gameMenuOverlay.parseDefinitionFromFile = LoadGameMenu;
    gameMenuOverlay.isSelected = BindGameMenu;
    gameMenuOverlay.isPressed = GameMenuInput;
    gameMenuOverlay.render = RenderGameMenu;
    gameMenuOverlay.reset = ResetGameMenu;
    gameMenuOverlay.tick = TickGameMenu;
    gameMenuOverlay.destroy = DestroyGameMenu;
    gameMenuOverlay.resetRender = ResetGameMenuRender;
    gameMenuOverlay.fieldAc = 1;
    auto &items = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuItems)];
    items.name = "SubMenu: Items";
    items.parseDefinitionFromFile = LoadItemsMenu;
    items.isSelected = BindItemsMenu;
    items.isPressed = ItemsMenuInput;
    items.render = RenderItemsMenu;
    items.reset = ResetItemsMenu;
    items.tick = TickItemsMenu;
    items.destroy = DestroyItemsMenu;
    RegisterCollectionAndPhotos();
    RegisterOptionsAndPrompt();
    RegisterNativeGameHud();
    RegisterGameNotebook();

    UIElementState &intro = g_uiElements[static_cast<std::size_t>(UIElement::IntroCutsceneCredits)];
    intro.name = "Intro Cutscene - Credits & Logo";
    intro.parseDefinitionFromFile = LoadIntroCutsceneCredits;
    intro.isSelected = BindIntroCutsceneCredits;
    intro.render = DrawIntroCutsceneCredits;
    intro.reset = ResetIntroCutsceneCredits;
    UIElementState &fightGauge = g_uiElements[static_cast<std::size_t>(UIElement::FightGauge)];
    fightGauge.name = "Fight Gauge";
    fightGauge.parseDefinitionFromFile = LoadFightGaugeDefinition;
    fightGauge.isSelected = BindFightGaugeUI;

    UIElementState &gameQte = g_uiElements[static_cast<std::size_t>(UIElement::GameQte)];
    gameQte.name = "Game QTE";
    gameQte.parseDefinitionFromFile = LoadGameQte;
    gameQte.isSelected = BindGameQte;
    gameQte.isPressed = GameQteInputNullsub;
    gameQte.render = RenderGameQte;

    InitializeEndingCreditsElement(g_uiElements[static_cast<std::size_t>(UIElement::Credits)]);
    UIElementState &gameHudWatch = g_uiElements[static_cast<std::size_t>(UIElement::GameHudWatch)];
    gameHudWatch.name = "Game HUD Watch";
    gameHudWatch.parseDefinitionFromFile = LoadGameHudWatch;
    gameHudWatch.tick = GameHudWatchTickNullsub;
    gameHudWatch.destroy = GameHudWatchDestroy;
    gameHudWatch.render = RenderGameHudWatch;
    gameHudWatch.isSelected = BindGameHudWatch;
    gameHudWatch.fieldAc = 1;

    UIElementState &loading = g_uiElements[static_cast<std::size_t>(UIElement::LoadingScreen)];
    loading.name = "SLoad Screen";
    loading.parseDefinitionFromFile = LoadLoadingScreenDefinition;
    loading.isSelected = BindLoadingScreenDefinition;
    loading.tick = TickLoadingScreenDefinition;
    loading.render = RenderLoadingScreenDefinition;
    loading.destroy = DestroyLoadingScreenDefinition;
    loading.reset = ResetLoadingScreenDefinition;

    UIElementState &mainMenu = g_uiElements[static_cast<std::size_t>(UIElement::MainMenu)];
    mainMenu.name = "Main Menu";
    mainMenu.parseDefinitionFromFile = LoadMainMenu;
    mainMenu.isSelected = MainMenuSelected;
    mainMenu.isPressed = MainMenuWheelInput;
    mainMenu.render = MainMenuRender;
    mainMenu.tick = TickMainMenu;
    mainMenu.destroy = DestroyMainMenuPage;

    UIElementState &mainMenuWheel = g_uiElements[static_cast<std::size_t>(UIElement::MainMenuWheel)];
    mainMenuWheel.name = "Main Menu Wheel";
    mainMenuWheel.parseDefinitionFromFile = LoadMainMenuWheel;
    mainMenuWheel.isSelected = BindMainMenuWheel;
    mainMenuWheel.isPressed = MainMenuWheelInput;
    mainMenuWheel.render = MainMenuWheelRender;
    mainMenuWheel.tick = TickMainMenuWheel;
    mainMenuWheel.destroy = DestroyMainMenuWheel;
    g_mainMenuWheelPending = 1;
    UIElementState &mainMenuNew = g_uiElements[static_cast<std::size_t>(UIElement::MainMenuNew)];
    mainMenuNew.name = "Main Menu New";
    mainMenuNew.parseDefinitionFromFile = LoadMainMenuNew;
    UIElementState &mainMenuContinue = g_uiElements[static_cast<std::size_t>(UIElement::MainMenuContinue)];
    mainMenuContinue.name = "Main Menu Continue";
    mainMenuContinue.parseDefinitionFromFile = LoadMainMenuContinue;
    UIElementState &mainMenuCollection = g_uiElements[static_cast<std::size_t>(UIElement::ShenmueCollection)];
    mainMenuCollection.name = "Main Menu Collection";
    mainMenuCollection.parseDefinitionFromFile = LoadMainMenuCollection;
    mainMenuCollection.isSelected = BindOmakeCollectionUI;
    mainMenuCollection.tick = TickOmakeCollectionUI;
    mainMenuCollection.render = RenderOmakeCollectionUI;
    mainMenuCollection.isPressed = InputOmakeCollectionUI;
    mainMenuCollection.destroy = DestroyOmakeCollectionUI;
    mainMenuCollection.reset = ResetOmakeCollectionUI;
    mainMenuCollection.resetRender = ResetOmakeCollectionRender;
    mainMenuCollection.unknown98 = OmakeCollectionHints;
    UIElementState &gameBackground = g_uiElements[static_cast<std::size_t>(UIElement::GameBackground)];
    gameBackground.name = "Game Background";
    gameBackground.parseDefinitionFromFile = LoadGameBackground;
    gameBackground.isPressed = ScreenFadeAcceptsInput;
    gameBackground.isSelected = ScreenFadeAcceptsInput;
    gameBackground.render = DrawScreenFilm;
    gameBackground.reset = ResetGameBackgroundNullsub;
    gameBackground.fieldAc = 1;
    gameBackground.tick = TickGameBackground;
    gameBackground.destroy = DestroyGameBackground;
    UIElementState &gameMoves = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuMoves)];
    gameMoves.name = "SubMenu: Moves";
    gameMoves.parseDefinitionFromFile = LoadGameMoves;
    gameMoves.isSelected = BindGameMovesDefinition;
    gameMoves.isPressed = PressGameMoves;
    gameMoves.render = RenderGameMoves;
    gameMoves.destroy = DestroyGameMoves;
    gameMoves.tick = TickGameMoves;
    gameMoves.reset = ResetGameMoves;
    gameMoves.unknown88 = SetGameMovesTheme;
    gameMoves.unknown98 = GameMovesHints;
    UIElementState &streetHud = g_uiElements[static_cast<std::size_t>(UIElement::StreetFighterHud)];
    streetHud.name = "Street Fighter HUD";
    streetHud.parseDefinitionFromFile = LoadStreetFighterHud;
    streetHud.isPressed = StreetFighterHudInput;
    streetHud.render = RenderStreetFighterHud;
    streetHud.isSelected = BindStreetFighterHud;
    streetHud.reset = ResetStreetFighterHud;
    streetHud.tick = ClearStreetFighterHud;
    streetHud.destroy = DestroyStreetFighterHud;
    UIElementState &duckHud = g_uiElements[static_cast<std::size_t>(UIElement::DuckRace)];
    duckHud.name = "Duck Race";
    duckHud.parseDefinitionFromFile = LoadDuckRaceHud;
    duckHud.isPressed = DuckRaceHudInput;
    duckHud.render = RenderDuckRaceHud;
    duckHud.isSelected = BindDuckRaceHud;
    duckHud.reset = ResetDuckRaceHud;
    duckHud.tick = ClearDuckRaceHud;
    duckHud.destroy = DuckRaceHudInput;
    UIElementState &information = g_uiElements[static_cast<std::size_t>(UIElement::GameInfoBar)];
    information.name = "Game Menu: Information Bar";
    information.parseDefinitionFromFile = LoadInformationBar;
    information.isSelected = BindInformationBarLayout;
    information.isPressed = InformationBarEnabled;
    information.render = RenderInformationBar;
    information.reset = ResetInformationBar;
    information.tick = TickInformationBar;
    information.destroy = InformationBarEnabled;
    information.unknown88 = SetInformationBarTheme;
    UIElementState &moveLearned = g_uiElements[static_cast<std::size_t>(UIElement::MoveLearned)];
    moveLearned.name = "Game Move Learned";
    moveLearned.parseDefinitionFromFile = LoadMoveLearned;
    moveLearned.isSelected = BindMoveLearnedDefinition;
    moveLearned.isPressed = MoveLearnedInput;
    moveLearned.tick = EnableMoveLearnedMenuLayer;
    moveLearned.destroy = DisableMoveLearnedMenuLayer;
    moveLearned.reset = ResetMoveLearnedBindings;
    moveLearned.render = RenderMoveLearnedDefinition;

    UIElementState &options = g_uiElements[static_cast<std::size_t>(UIElement::Options)];
    options.name = "Options";
    UIElementState &loadSave = g_uiElements[static_cast<std::size_t>(UIElement::LoadSave)];
    loadSave.name = "Load/Save";
    loadSave.parseDefinitionFromFile = LoadLoadSave;
    loadSave.isSelected = LoadSaveSelected;
    loadSave.isPressed = LoadSaveInput;
    loadSave.render = LoadSaveRender;
    loadSave.tick = LoadSaveTick;
    loadSave.destroy = LoadSaveDestroy;
    loadSave.reset = LoadSaveResetNullsub;
    loadSave.unknown88 = SetLoadSaveTheme;
    loadSave.unknown98 = LoadSaveHints;

    UIElementState &notebookPages = g_uiElements[static_cast<std::size_t>(UIElement::NotebookPages)];
    notebookPages.name = "Notebook Pages";
    notebookPages.config.init = 3;
    notebookPages.isSelected = CreateMemoNotebookPages;
    notebookPages.destroy = DestroyMemoNotebookPages;

    UIElementState &cinemaBars = g_uiElements[static_cast<std::size_t>(UIElement::CinemaBars)];
    cinemaBars.name = "Cinema bars";
    cinemaBars.render = RenderCinemaBars;
    cinemaBars.tick = ShowCinemaBars;
    cinemaBars.destroy = CloseCinemaBars;
    cinemaBars.unknown80 = CinemaBarsAreActive;
    cinemaBars.fieldAc = 1;

    UIElementState &subtitles = g_uiElements[static_cast<std::size_t>(UIElement::Subtitles)];
    subtitles.name = "Subtitles";
    subtitles.parseDefinitionFromFile = LoadSubtitles;
    subtitles.isSelected = BindSubtitles;
    subtitles.reset = ResetSubtitleBindings;
    UIElementState &cutsceneSkip = g_uiElements[static_cast<std::size_t>(UIElement::CutsceneSkip)];
    cutsceneSkip.name = "Cutscene Skip Button";
    cutsceneSkip.parseDefinitionFromFile = LoadCutsceneSkip;
    cutsceneSkip.render = RenderCutsceneSkip;
    UIElementState &qteMini = g_uiElements[static_cast<std::size_t>(UIElement::QteMinigame)];
    qteMini.name = "QTE MiniGame";
    qteMini.parseDefinitionFromFile = LoadQteMinigame;
    qteMini.isSelected = BindQteMinigame;
    qteMini.isPressed = QteMinigameInput;
    qteMini.reset = ResetQteMinigame;
    qteMini.tick = ClearQteMinigame;
    qteMini.destroy = DestroyQteMinigame;
    qteMini.render = RenderQteMinigame;
    qteMini.fieldAc = 1;
    g_uiElements[static_cast<std::size_t>(UIElement::PlatformIcons)].name = "Platform Icons";
    UIElementState &actionSelector = g_uiElements[static_cast<std::size_t>(UIElement::ActionSelector)];
    actionSelector.name = "Action Selector";
    actionSelector.parseDefinitionFromFile = LoadActionSelector;
    actionSelector.isSelected = BindActionSelectorLayout;
    actionSelector.isPressed = ActionSelectorPressed;
    actionSelector.tick = StartActionSelector;
    actionSelector.destroy = ActionSelectorDestroy;
    actionSelector.render = DrawActionSelector;
    UIElementState &pawnSets = g_uiElements[static_cast<std::size_t>(UIElement::PawnshopFlyer2)];
    pawnSets.name = "Pawnshop Flyer";
    pawnSets.parseDefinitionFromFile = LoadPawnSetFlyer;
    pawnSets.isSelected = BindPawnSetFlyer;
    pawnSets.isPressed = PawnSetFlyerInput;
    pawnSets.tick = TickPawnSetFlyer;
    pawnSets.destroy = DestroyPawnSetFlyer;
    pawnSets.render = RenderPawnSetFlyer;
    pawnSets.unknown98 = PawnSetFlyerHints;
    pawnSets.reset = ResetPawnSetFlyer;
    UIElementState &pawnItems = g_uiElements[static_cast<std::size_t>(UIElement::PawnshopFlyer1)];
    pawnItems.name = "Pawnshop Flyer";
    pawnItems.parseDefinitionFromFile = LoadPawnItemFlyer;
    pawnItems.isSelected = BindPawnItemFlyer;
    pawnItems.isPressed = PawnItemFlyerInput;
    pawnItems.tick = TickPawnItemFlyer;
    pawnItems.destroy = DestroyPawnItemFlyer;
    pawnItems.render = RenderPawnItemFlyer;
    pawnItems.unknown98 = PawnItemFlyerHints;
    pawnItems.reset = ResetPawnItemFlyer;
    auto &moneyHud = g_uiElements[static_cast<std::size_t>(UIElement::GameHudMoney)];
    moneyHud.name = "Game HUD Money";
    moneyHud.parseDefinitionFromFile = AdvanceMoneyHudInit;
    moneyHud.unknown70 = ReloadMoneyHud;
    moneyHud.unknown78 = GetMoneyHudInitState;
    moneyHud.postInit = RenderMoneyHud;
    UIElementState &timer = g_uiElements[static_cast<std::size_t>(UIElement::GameHudTimer)];
    timer.name = "Game HUD Timer";
    timer.parseDefinitionFromFile = LoadGameHudTimer;
    timer.isPressed = GameHudTimerDebugGate;
    timer.render = RenderGameHudTimer;
    timer.isSelected = BindGameHudTimer;
    timer.reset = ResetGameHudTimer;
    timer.tick = ClearGameHudTimer;
    timer.destroy = GameHudTimerDebugGate;
    auto &hudSnapshot = g_uiElements[static_cast<std::size_t>(UIElement::HudSnapshot)];
    hudSnapshot.name = "HUD: Snapshot";
    hudSnapshot.parseDefinitionFromFile = LoadHudSnapshot;
    hudSnapshot.isSelected = BindHudSnapshot;
    hudSnapshot.render = RenderHudSnapshot;
    UIElementState &message = g_uiElements[static_cast<std::size_t>(UIElement::GameMessage)];
    message.name = "Game Message";
    message.parseDefinitionFromFile = LoadGameMessage;
    message.isSelected = PrepareGameMessageLayout;
    message.isPressed = GameMessageDebugGate;
    message.render = DrawGameMessage;
    message.reset = ResetGameMessageBindings;
    message.tick = GameMessageSelected;
    message.destroy = GameMessageDestroy;
    auto &shared = g_uiElements[static_cast<std::size_t>(UIElement::SharedItems)];
    shared.name = "Shared Items";
    shared.parseDefinitionFromFile = LoadSharedItems;
    shared.isSelected = GetIconSet;
    shared.isPressed = SharedItems_IsPressed;
    shared.fieldAc = 1;
    auto &help = g_uiElements[static_cast<std::size_t>(UIElement::GameHelp)];
    help.parseDefinitionFromFile = LoadHelpDefinition;
    help.isSelected = HelpWidgetBind;
    help.render = HelpWidgetRender;
    help.isPressed = HelpWidgetInput;
    help.initialize = HelpWidgetShow;
    help.destroy = HelpWidgetDestroy;
    help.unknown98 = HelpWidgetHints;
    UIElementState &platformIcons = g_uiElements[static_cast<std::size_t>(UIElement::PlatformIcons)];
    platformIcons.parseDefinitionFromFile = LoadPlatformIcons;
    platformIcons.isSelected = PlatformIcons_IsSelected;
}

// @verified - p
std::int64_t RenderCinemaBars() {
    cinemaBarAmount = std::fmax(0.0f, std::fmin(1.0f, GetFrameTime() * cinemaBarDirection + cinemaBarAmount));
    const float ease = std::sin((cinemaBarAmount * 3.141592741f) * 0.5f);
    const std::int32_t opacity = static_cast<std::int32_t>(std::fmax(0.0f, std::fmin(1.0f, cinemaBarAmount * 10.0f)) * 255.0f);
    nk_context &context = *GetNativeUiContext();
    const float width = static_cast<float>(UI_GetAspectWidth());
    const float height = 1080.0f;
    const float barHeight = (height * 0.119047611951828f) * ease;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, opacity));
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(
            &context, &cinemaBarPanels[0], "Aspect Bar Top", "Aspect Bar Top", nk_rect(-10.0f, -1.0f, width + 10.0f, barHeight), NK_WINDOW_NO_SCROLLBAR);
        nk_end(&context);
        nk_begin_titled(&context, &cinemaBarPanels[1], "Aspect Bar Bottom", "Aspect Bar Bottom",
            nk_rect(-10.0f, (height - barHeight) + 1.0f, width + 10.0f, barHeight), NK_WINDOW_NO_SCROLLBAR);
        nk_end(&context);
    }
    return 0;
}

// @verified - p
std::int64_t RenderScreenFilm() {
    nk_context &context = *GetNativeUiContext();
    nk_color colour{};
    std::memcpy(&colour, &gameScreenFadeColour, sizeof(colour));
    context.style.window.fixed_background = nk_style_item_color(colour);
    const float width = static_cast<float>(UI_GetAspectWidth());
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &screenFadePanel, "Screen Fade", "Screen Fade", nk_rect(-10.0f, -10.0f, width + 20.0f, 1100.0f), NK_WINDOW_NO_SCROLLBAR);
        nk_end(&context);
    }
    return 0;
}

std::intptr_t DrawScreenFilmFrame() {
    RefreshNotebook();
    UIElementState &element = g_uiElements[static_cast<std::size_t>(g_currActiveUIIndex)];
    if (g_currActiveUIIndex == static_cast<std::int32_t>(UIElement::GameHudMoney))
        element.postInit();
    else
        element.render();
    Nuklear_BindRenderTarget();
    NuklearUpdateForAspectRatio(GetAspectRatioModeShouldScale());
    CopyNuklearTexToBackBuffer();
    return 0;
}

void RenderScreenFilmFrame(std::int32_t index) {
    if (!drawingUI)
        return;
#if defined(_WIN32)
    ExecuteCurrentCommandList();
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(1);
    const NuklearFrameCallback previous = g_nuklearFrameCallback;
    const std::int32_t previousIndex = g_currActiveUIIndex;
    g_currActiveUIIndex = index;
    g_nuklearFrameCallback = DrawScreenFilmFrame;
    InvokeNuklearFrame(d3t::DX11::g_ppD3DDeviceContext);
    FinishNuklearFrame();
    g_nuklearFrameCallback = previous;
    g_currActiveUIIndex = previousIndex;
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    d3t::BindUIFrameRenderTarget();
#endif
}

// STVW snapshot/debug and biography viewer ownership.

HLib::Task *snapshotDebugTask{};
SnapshotDebugWork *snapshotDebugWork{};
SnapshotViewerStorage snapshotDebugStorage{};
std::array<std::uint32_t, 48> snapshotDebugThumbnails{};
std::array<SnapshotMetadataRecord *, 126> &snapshotDebugOrder = snapshotDebugStorage.order;
SnapshotMetadataOwner *&snapshotDebugMetadata = snapshotDebugStorage.metadata;
std::int32_t snapshotDebugInitialized{};
std::int16_t snapshotDebugPulse{}, snapshotDebugPulseStep{};
std::int32_t warningMessageIndex{}, warningMessageNumber{};
SnapshotDebugTextResources snapshotDebugTextResources{};
std::int32_t snapshotDebugSpriteCount{};
std::array<std::int32_t, 7> snapshotDebugSprites{};
} // namespace

namespace ui_detail {

std::int32_t legacySelectorSubmenuPhase{}, legacySelectorSubmenuSelection{};

#include "ui_item_selector_draw.inl"

void NativeCreditsNoOp(void *) {
}

void EndingStaffRollTerminal(void *) {
    g_actNumber = 1;
    g_entryPoint = 0;
    g_loadedActNumber = 1;
    SetNextSceneNumber(1);
    hotLoadModule = SystemModule::Cold;
    ChangeDisc();
    ResetSelectedInputDevice();
    ChangeSystemModule(SystemModule::Cold, 0);
}

// @verified - p
void EndingStaffRollWaitForSave(void *taskData) {
    EndingStaffRollTaskData *const data = static_cast<EndingStaffRollTaskData *>(taskData);
    if (IsSaveAllowed() != 0)
        return;
    WaitForNextTask_0(data->screenFilmTask, 1, 30);
    HLib::SetCurrentTaskTo(EndingStaffRollTerminal);
    HLib::SetupTaskDelay(32);
}

bool EndingCreditsComplete() noexcept {
    const float total = ((creditsState.tables[0].height + 0.0f) + creditsState.tables[1].height) + creditsState.tables[2].height;
    return creditsState.scrollProgress > CREDITS_SIGNATURE_DURATION * creditsState.scrollSpeed + total;
}

// @verified - p
void EndingStaffRollNewCallback(void *) {
    if (!EndingCreditsComplete())
        return;
    DestroyElementByIndex(UIElement::Credits);
    StartShowScreenOfId(6);
    HLib::SetCurrentTaskTo(EndingStaffRollWaitForSave);
}

void *LoadEndingStaffRollResource() {
    return LoadLegacySceneFile("scene/04/stfrol", "STAFF_01.SPR", STAFF_ROLL_SPRITE);
}

} // namespace

std::int32_t GetCurrentUIState() {
    return currentUIState;
}
#include "ui_omake.inl"
std::int64_t CopyHudHintIds(std::span<const std::int16_t> ids) {
    for (std::size_t i = 0; i < ids.size(); ++i)
        nativeHudRequestedHints[i] = (nativeHudRequestedHints[i] & 0xffff0000U) | static_cast<std::uint16_t>(ids[i]);
    return reinterpret_cast<std::int64_t>(nativeHudRequestedHints.data());
}
void PublishHudHints(const std::uint32_t *input, bool overrideHints) {
    if (overrideHints)
        nativeHudOverride = input != nullptr;
    if (!input) {
        if (!overrideHints)
            for (std::size_t i = 0; i < 4; ++i)
                nativeHudShownHints[i] = (nativeHudShownHints[i] & 0xffff0000U) | 0xffffU;
        return;
    }
    for (std::size_t i = 0; i < 3; ++i)
        menuInputHintHistory[i] = menuInputHintHistory[i + 1];
    for (std::size_t i = 0; i < 5; ++i)
        menuInputHintHistory[3][i] = input[i];
    std::array<std::uint32_t, 5> &output = overrideHints ? nativeHudOverrideHints : nativeHudShownHints;
    for (std::size_t column = 0; column < 5; ++column) {
        std::uint32_t hint = menuInputHintHistory[3][column];
        if (static_cast<std::uint16_t>(hint) == 0xffffU)
            for (std::size_t previous = 3; previous-- > 0;)
                if (static_cast<std::uint16_t>(menuInputHintHistory[previous][column]) != 0xffffU) {
                    hint = menuInputHintHistory[previous][column];
                    break;
                }
        const std::int32_t transition = nativeHudTransitions[column];
        if (((output[column] & 0x10000U) && transition >= 0 && IsUITransitionActive(transition)) ||
            (static_cast<std::uint16_t>(hint) != static_cast<std::uint16_t>(output[column]) && static_cast<std::uint16_t>(hint) != 0xffffU))
            hint |= 0x10000U;
        if ((hint & 0x10000U) && transition >= 0 && !IsUITransitionActive(transition)) {
            UIAnim_Start(transition, 1, 0, 1.2f, 0);
            if (nativeHudBlink >= 0 && !IsUITransitionActive(nativeHudBlink))
                UIAnim_Start(nativeHudBlink, 1, 0.1f, 0.2f, 0.01f);
        }
        output[column] = hint;
    }
}

// @verified - p
void HandleUIStates() {
    const std::uint32_t *hints = nullptr;
    if (IsAnyItemSelectActive() || IsSharedItemsWidgetOwnedBy(0) || g_uiElements[1].isVisible) {
        if (sharedItemsWidget.active)
            g_uiFixedMenuHintIds[2] = 58;
        hints = reinterpret_cast<const std::uint32_t *>(CopyHudHintIds(g_uiFixedMenuHintIds));
    } else if (g_debugImGuiRenderMode == 1) {
        for (std::size_t index = g_uiElements.size(); index-- > 1;) {
            UIElementState &element = g_uiElements[index];
            if (!element.config.status || !element.unknown98)
                continue;
            const std::int64_t result = element.unknown98();
            if (result) {
                hints = reinterpret_cast<const std::uint32_t *>(result);
                break;
            }
        }
    }
    PublishHudHints(hints, true);
}

// @verified - p
std::int32_t GetDebugForceUIEnabled(UIElement) noexcept {
    return 1;
}

// @verified - p
void ResetStreetFighterHUD() {
    if (streetFighterHudWindows[0])
        ShowUIWindow(streetFighterHudWindows[0]);
}

void vfSetupFightUiAndCameraState(VFMA_t *game) {
    std::uint32_t saved = 0;
    if (plymGetSpecialMode() == 1) {
        plymSetSpecialMode(0);
        saved = 0x400;
    }
    if ((g_pFightStage->battleFlags & 2) == 0) {
        if ((g_loadingFlag & 0x20) == 0)
            saved |= 0x800;
        FinishLoading();
    }
    if (game->type == 4) {
        if (static_cast<std::uint16_t>(GetUiFlag(2)) == 1) {
            saved |= static_cast<std::uint16_t>(GetUiFlag(0)) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(GetUiFlag(1))) << 16) | 0x200;
            SetHelpIconMenuFlags(1025);
        } else
            AddHelpIconMenuFlags(1025);
    }
    const std::uint8_t icon = helpGetCurrentIconIdx(1);
    game->savedHelpTopic = icon;
    if ((g_pFightStage->battleFlags & 2) == 0 && game->type != 2) {
        if (game->type == 9)
            EV_EventSetUpHelpIcon_0(HELP_TOPIC_MOVE_INSTRUCTION, 0);
        else {
            const std::uint8_t offset = static_cast<std::uint8_t>(icon - 89);
            if ((offset > 63 || (0xf0000000003fffffULL & (1ULL << offset)) == 0) && icon != HELP_TOPIC_MOVE_INSTRUCTION)
                EV_EventSetUpHelpIcon_0(HELP_TOPIC_FREE_BATTLE, 0);
        }
    }
    if (IsClockAndCompassSuppressed()) {
        if ((g_pFightStage->battleFlags & 2) == 0)
            SetRenderClockAndCompassTo(1);
        saved |= 0x4000;
    }
    vfSetupFightCameraState(game);
    game->savedUiState = saved;
}

// @verified - p
void vfPostFightLoadingNullsub(VFMA_t *) {
}

void vfRestorePostFightUIState(VFMA_t *game) {
    using enum CameraState;
    const std::uint32_t saved = game->savedUiState;
    if (saved & 0x400)
        plymSetSpecialMode(1);
    if ((g_pFightStage->battleFlags & 2) == 0) {
        if (saved & 0x800)
            FinishLoading();
        else
            vfPostFightLoadingNullsub(game);
    }
    const std::uint8_t icon = helpGetCurrentIconIdx(1);
    if (static_cast<std::uint8_t>(icon - 89) > 21 && static_cast<std::uint8_t>(icon + 107) > 3)
        if (game->savedHelpTopic != HELP_TOPIC_MOVE_INSTRUCTION)
            EV_EventSetUpHelpIcon_0(game->savedHelpTopic, 0);
    if (game->type == 4) {
        if (saved & 0x200)
            SetHelpIconMenuFlags(static_cast<std::uint8_t>(saved));
        else {
            helpIconState.persistentVisible &= 0x200;
            helpIconState.persistentSelected = helpIconState.persistentVisible;
            helpIconState.frameOverlaySelected &= helpIconState.frameOverlayVisible;
            ConfigureHelpDirectionBlink(helpIconState.persistentVisible);
        }
    }
    if (saved & 0x4000)
        SetRenderClockAndCompassTo(0);
    if (game->type != 9) {
        HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
        if (task)
            HLib::GetTaskParameter(task);
        if (EV_camCtrlToggleCombatCamera(2, -1) == 0 && camCtrlGetState() == ChaseGoal) {
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Event);
            camCtrlSetState(Init);
            camCtrlSetPerspective(static_cast<std::uint16_t>(game->savedPerspective));
        }
    }
}

// @verified - p
void SetStreetFighterTimerDigits(const std::array<std::int32_t, 6> &digits) {
    char *const text = streetFighterHudText[0].data();
    if (digits[0] > 0)
        std::snprintf(text, 256, "%d%d:%d%d:%d%d", digits[0], digits[1], digits[2], digits[3], digits[4], digits[5]);
    else if (digits[1] > 0)
        std::snprintf(text, 256, "%d:%d%d:%d%d", digits[1], digits[2], digits[3], digits[4], digits[5]);
    else
        std::snprintf(text, 256, "%d%d:%d%d", digits[2], digits[3], digits[4], digits[5]);

    if (streetFighterHudLabels[0])
        streetFighterHudLabels[0]->colour = digits[0] >= 1 || digits[1] >= 1 || digits[2] >= 1 ? 0xffffffffU : 0xff0c18b7U;
}

// @verified - p
void SetGameHudTimerCaption(const char *text, std::uint32_t colour) {
    if (text) {
#ifdef _WIN32
        strcpy_s(gameHudTimerText.data(), gameHudTimerText.size(), text);
#else
        const std::size_t length = std::strlen(text);
        if (length < gameHudTimerText.size())
            std::memcpy(gameHudTimerText.data(), text, length + 1);
        else
            gameHudTimerText[0] = '\0';
#endif
        if (gameHudTimerPanel)
            gameHudTimerPanel->alpha = 1.0f;
        if (gameHudTimerLabel)
            gameHudTimerLabel->colour = ConstructColour((colour >> 16U) & 0xffU, (colour >> 8U) & 0xffU, colour & 0xffU, colour >> 24U);
    } else if (gameHudTimerPanel) {
        gameHudTimerPanel->alpha = 0.0f;
    }
}

void SetDuckRaceTimerCaption(const char *text) {
    if (text) {
        const std::size_t length = std::strlen(text);
        if (length < duckRaceHudText[6].size())
            std::memcpy(duckRaceHudText[6].data(), text, length + 1);
        else
            duckRaceHudText[6][0] = '\0';
    }
    if (duckRaceHudWindows[12])
        duckRaceHudWindows[12]->alpha = text ? 1.0f : 0.0f;
}
void SetDuckRacePlacementRow(std::int32_t row, const char *text, std::int32_t bowtie) {
    if (text) {
        const std::size_t length = std::strlen(text);
        if (length < duckRaceHudText[row].size())
            std::memcpy(duckRaceHudText[row].data(), text, length + 1);
        else
            duckRaceHudText[row][0] = '\0';

        if (duckRaceHudWindows[row + 6])
            duckRaceHudWindows[row + 6]->alpha = 1.0f;
        if (duckRaceHudWindows[row])
            duckRaceHudWindows[row]->alpha = 1.0f;
        if (duckRaceHudBowties[row])
            duckRaceHudBowties[row]->sprite = bowtie;
    } else {
        if (duckRaceHudWindows[row + 6])
            duckRaceHudWindows[row + 6]->alpha = 0.0f;
        if (duckRaceHudWindows[row])
            duckRaceHudWindows[row]->alpha = 0.0f;
    }
}
// @verified - p
void ConstructNumStrikesLeftText(std::int32_t strikes) {
    streetFighterHudLabels[2]->colour |= 0xff000000U;
    streetFighterHudText[2][0] = '\0';
    if (strikes > 0) {
        const char *const key = strikes <= 1 ?
            "UISFSTRIKES1" :
            "UISFSTRIKESM"; // @text UISFSTRIKES1 EN: "1 Strike Left" | JA: "残り1発" ; UISFSTRIKESM EN: "[[VALUE]] Strikes Left" | JA: "残り[[VALUE]]発"

        std::snprintf(streetFighterHudText[2].data(), streetFighterHudText[2].size(), "%s", GetStringForUILabel(key));
        std::array<char, 16> value;
        std::snprintf(value.data(), value.size(), "%u", static_cast<std::uint32_t>(strikes));
        TextFormat(streetFighterHudText[2].data(), static_cast<std::int32_t>(streetFighterHudText[2].size()), "[[VALUE]]", value.data());
    }
}
// @verified - p
void GetSFLevel(std::int32_t level) {
    streetFighterHudLabels[1]->colour |= 0xff000000U;

    std::snprintf(streetFighterHudText[1].data(), streetFighterHudText[1].size(), "%s", GetStringForUILabel("UISFLEVEL"));
    std::array<char, 16> value;
    std::snprintf(value.data(), value.size(), "%u", static_cast<std::uint32_t>(level));
    TextFormat(streetFighterHudText[1].data(), static_cast<std::int32_t>(streetFighterHudText[1].size()), "[[LEVEL]]", value.data());
}

// @verified - p
void SetFightRoundPrompt(std::int32_t round) {
    if (round >= 0) {
        if (streetFighterHudWindows[1])
            streetFighterHudWindows[1]->alpha = 1.0f;
        d3t::UILabelWidget *const label = streetFighterHudLabels[3];
        if (label) {
            label->colour = (label->colour & 0x00ffffffU) | 0xc8000000U;
            label->text = GetStringForUILabel(UI_FIGHT_ROUND_TOKENS[round <= 2 ? round : 2]);
        }
    } else if (streetFighterHudWindows[1]) {
        streetFighterHudWindows[1]->alpha = 0.0f;
    }
}
float GetUIViewportWidth() noexcept {
    if (ShouldScaleS2AspectRatio())
        return 1439.0f;
    return static_cast<float>(
        static_cast<std::int32_t>(1080.0f * static_cast<float>(d3t::g_originalScreenWidth) / static_cast<float>(d3t::g_originalScreenHeight)));
}

const UIMessageBox &GetCurrentUIMessageBox() noexcept {
    return informationBar.current;
}

void CopyUIMessageBox(const UIMessageBox *message) {
    InformationBarRuntime &runtime = informationBar;
    runtime.remaining = 0.0f;
    const std::uint32_t colour = message && message->overrideColour ? message->colour : 0xffffffffU;
    for (d3t::UILabelWidget *label : runtime.labels)
        if (label)
            label->colour = colour;
    if (message) {
        runtime.current = *message;
        if (runtime.windows[5])
            runtime.windows[5]->alpha = 1.0f;
    } else {
        runtime.current.imageIndex = runtime.current.widgetIndex = runtime.current.imageFrame = -1;
        runtime.current.primaryText = nullptr;
        runtime.current.primaryLocalized = 0;
        runtime.current.reserved49.fill(std::byte{});
        if (runtime.windows[5])
            runtime.windows[5]->alpha = 0.0f;
    }
    for (std::array<char, 256> &text : runtime.text)
        text[0] = '\0';
    const auto localize = [](const char *&text, std::uint8_t &localized) {
        if (!text || localized)
            return;
        const std::string_view value = GetLocalizedSceneText(text);
        if (!value.empty()) {
            text = value.data();
            localized = 1;
        }
    };
    localize(runtime.current.primaryText, runtime.current.primaryLocalized);
    if (runtime.current.primaryText) {
        const char *const split = std::strstr(runtime.current.primaryText, "\xa1\xf5");
        if (split) {
            std::snprintf(runtime.text[1].data(), 256, "%.*s", static_cast<int>(split - runtime.current.primaryText), runtime.current.primaryText);
            std::snprintf(runtime.text[2].data(), 256, "%s", split + 2);
        } else
            std::snprintf(runtime.text[0].data(), 256, "%s", runtime.current.primaryText);
    }
    localize(runtime.current.secondaryText, runtime.current.secondaryLocalized);
    localize(runtime.current.tertiaryText, runtime.current.tertiaryLocalized);
    if (runtime.current.secondaryText)
        std::snprintf(runtime.text[4].data(), 256, "%s", runtime.current.secondaryText);
    if (runtime.current.tertiaryText)
        std::snprintf(runtime.text[3].data(), 256, "%s", runtime.current.tertiaryText);
    for (std::size_t index = 0; index < 2; ++index) {
        d3t::UIImageWidget *const image = runtime.images[index];
        if (!image)
            continue;
        image->frame = runtime.current.imageIndex;
        image->image = runtime.current.imageFrame;
        if (image->frame == -1 || image->image == -1)
            image->field44 = 0;
    }
    d3t::UIWindow *const focus = runtime.windows[6];
    if (!focus)
        return;
    focus->alpha = 0.0f;
    if (message && !runtime.current.hideTarget) {
        if (runtime.current.widgetIndex == -1) {
            const Vector4 &bounds = runtime.current.bounds;
            if (bounds.x > 0.0f && bounds.y > 0.0f && bounds.z > 0.0f && bounds.w > 0.0f) {
                focus->layout.parentId[0] = '\0';
                focus->alpha = 1.0f;
                focus->layout.scaleStyle = 4;
                focus->layout.alignX = 9;
                focus->layout.alignY = 6;
                focus->layout.offsetX = bounds.x / static_cast<float>(UI_GetAspectWidth());
                focus->layout.offsetY = bounds.y / 1080.0f;
                focus->layout.width.value.fraction = bounds.z / 1920.0f;
                focus->layout.height.value.fraction = bounds.w / 1080.0f;
            }
        } else {
            const d3t::UIWindow *const target = UI_GetWindow(runtime.current.widgetIndex);
            if (target) {
                std::snprintf(focus->layout.parentId.data(), focus->layout.parentId.size(), "%s", target->id.data());
                focus->alpha = 1.0f;
                focus->layout.scaleStyle = 3;
                focus->layout.alignX = 1;
                focus->layout.alignY = 1;
                focus->layout.offsetX = 0.0f;
            }
        }
    }
}

void ShowTemporaryUIMessageBox(const UIMessageBox *message, float seconds) {
    if (!message)
        return;
    InformationBarRuntime &runtime = informationBar;
    if (runtime.remaining <= 0.0f) {
        runtime.previous = runtime.current;
        if (runtime.text[1][0] && runtime.text[2][0])
            std::snprintf(runtime.previousText.data(), runtime.previousText.size(), "%s\xa1\xf5%s", runtime.text[1].data(), runtime.text[2].data());
        else
            std::snprintf(runtime.previousText.data(), runtime.previousText.size(), "%s", runtime.text[0].data());
    }
    CopyUIMessageBox(message);
    runtime.remaining = seconds;
}

// @verified - p
void SetMoveLearnedUiLine(std::int32_t index, std::string_view token) {
    if (index < 3) {
        const std::string_view text = GetLocalizedSceneText(token);
        char *const destination = g_moveLearnedUiText[index].data();
#ifdef _WIN32
        strcpy_s(destination, 256, text.data());
#else
        if (text.size() >= 256)
            std::abort();
        std::memcpy(destination, text.data(), text.size() + 1);
#endif
    }
}

namespace ui_detail {} // namespace

void ExecuteFightGaugeUI() noexcept {
    constexpr std::size_t index = static_cast<std::size_t>(UIElement::FightGauge);
    if (currentUIState != -1 && currentUIState != static_cast<std::int32_t>(UIElement::FightGauge))
        return;
    UIElementState &element = g_uiElements[index];
    if (element.config.init != 3)
        return;
    SetGameUiFlag(gameUiSecondaryFlags, index, true);
    UpdateFightGaugeDefinition();
    EnableElement(UIElement::FightGauge);
}

// @verified - p
void Queue_OP_StaffRoll() {
    openingStaffRollTask = HLib::EnqueueTaskWithParameter(OP_StaffRoll_Callback, 8, 0, sizeof(OpeningStaffRollTaskData), TASK_OPST);
    if (!openingStaffRollTask)
        return;
    OpeningStaffRollTaskData *const data = static_cast<OpeningStaffRollTaskData *>(HLib::GetTaskParameter(openingStaffRollTask));
    if (!data)
        return;
    *data = {.textures = {-1, -1, -1}, .alpha = 0.0f, .item = 0, .phase = 2, .clock = -68.0f};
    void *const allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "OPST.SPR", OPENING_STAFF_ROLL_SPRITE);
    if (!allocation)
        return;
    TEXNReader(allocation, static_cast<std::int64_t>(HLib::GetTaskDataSize(allocation)), data->textures);
    HLib::FreeTaskData(allocation);
}

bool GetHasCreditsStillGotTimeRemaining() noexcept {
    return EndingCreditsComplete();
}
// @verified - p
std::int32_t GetEndingStaffRollFlag() noexcept {
    return endingStaffRollFlag;
}

// @verified - p
void SetEndingStaffRollFlag(std::int32_t value) noexcept {
    endingStaffRollFlag = value;
}

void StaffRoll(void *) {
    DispatchPostFxFilterRender(1);
    InitSceneEffects();
    AllocateDIRE("scene/04/stfrol", 0, 100);
    creditsLegacyParameters.unknown0c = 1;
    creditsLegacyParameters.unknown00 = 0.0f;
    creditsLegacyParameters.unknown08 = 0;
    creditsLegacyParameters.unknown04 = 2;
    HLib::Task *const film = Queue_SCF_Callback_ScreenFilmTaskEntry(8);
    WaitForNextTask_0(film, 0, 30);
    d3t::SetSceneClearColour(0xff000000U);
    g_systemManageDebugDisplay = 0;
    Queue_KeyRepeat(0);
    HLib::SetCurrentTaskTo(NativeCreditsNoOp);
    ShowElementByIndex(UIElement::Credits);

    endingStaffRollFlag = 1;
    endingStaffRollTask = HLib::EnqueueTaskWithParameter(EndingStaffRollNewCallback, 8, 0, sizeof(EndingStaffRollTaskData), TASK_STAF);
    EndingStaffRollTaskData *const data = static_cast<EndingStaffRollTaskData *>(HLib::GetTaskParameter(endingStaffRollTask));
    *data = {};
    data->resource = LoadEndingStaffRollResource();
    data->resourceCopy = data->resource;
    data->screenFilmTask = film;
    data->field6c = 1;
    data->field74 = 1;
    data->field40 = 1;
    SM_Init_Enqueue(1);
    HLib::SetupTimedTask(endingStaffRollTask, 90);
}
void ResetLoadingScreen() {
    ResetLoadingScreenDefinition();
}

void SetupLoadingScreen(shen_name area, shen_name subarea, const DateTime &dateTime, const char *label, std::uint32_t flags, std::int32_t floor,
    std::int32_t fadeMode, std::int32_t entry) {
    loadingScreen.collectionPoster = false;
    loadingScreen.area = area;
    loadingScreen.subarea = subarea;
    loadingScreen.dateTime = dateTime;
    loadingScreen.flags = flags;
    loadingScreen.floor = floor;
    loadingScreen.fadeMode = fadeMode;
    loadingScreen.entry = entry;
    if (loadingScreen.images[0] >= 0)
        g_uiImageWidgets[loadingScreen.images[0]].field44 = 0;
    if (loadingScreen.images[2] >= 0)
        g_uiImageWidgets[loadingScreen.images[2]].field44 = 0;
    if (static_cast<std::int32_t>(area.name) >= 0) {
        if (loadingScreen.hasLocationState)
            ResetLoadingLocationStateNoOp();
        PrepareLoadingLocationStateNoOp();
        loadingScreen.hasLocationState = true;
    }

    std::string &time = loadingScreen.text[static_cast<std::size_t>(LoadingLabelSlot::Time)];
    std::string &date = loadingScreen.text[static_cast<std::size_t>(LoadingLabelSlot::Date)];
    std::string &areaText = loadingScreen.text[static_cast<std::size_t>(LoadingLabelSlot::Area)];
    std::string &subAreaText = loadingScreen.text[static_cast<std::size_t>(LoadingLabelSlot::SubArea)];
    std::string &previousAreaText = loadingScreen.text[static_cast<std::size_t>(LoadingLabelSlot::PreviousArea)];
    if ((flags & 4U) != 0 || dateTime.year == 0 || fadeMode == 8) {
        time.clear();
        date.clear();
    } else {
        const std::int32_t year = static_cast<std::int32_t>(dateTime.year) + 1900;
        date = FormatLoadingScreenDate(year, dateTime.month, dateTime.day, dateTime.week, 3);
        time = FormatLoadingScreenTime(dateTime.hour, dateTime.minute, dateTime.hour >= 12 ? 1 : 0);
    }
    if (area.name == 0xffffffffU) {
        areaText.clear();
        subAreaText.clear();
        previousAreaText.clear();
        UpdateLoadingScreenLabels();
        return;
    }

    areaText = label && label[0] ? ResolveLoadingAreaLabel(label) : std::string{};
    subAreaText.clear();
    if ((flags & 8U) != 0) {
        if ((flags & 0x10U) != 0) {
            subAreaText = "R";
        } else if (floor > 0) {
            subAreaText = std::to_string(floor);
            subAreaText.push_back('F');
        } else {
            subAreaText = "B";
            subAreaText.append(std::to_string(std::int64_t{1} - floor));
        }
    }
    ReleaseLoadingHeader(0);
    ReleaseLoadingHeader(1);
    std::array<char, 260> path{};
    if (label) {
        std::snprintf(path.data(), path.size(), "/ui/loadscreen/textures/headers/LD_%s_0.png", label);
        loadingScreen.renderHandles[0] = d3t::d3t_AllocateRenderHandle();
        loadingScreen.headerRequests[0] = SetupUsageForTexture(path.data(), loadingScreen.renderHandles[0]);
        loadingScreen.collectionPoster =
            !std::strcmp(label, "MG01") || !std::strcmp(label, "MG02") || !std::strcmp(label, "MG03") || !std::strcmp(label, "MG04");
    }
    if (const LoadingPreviousAreaEntry *const previous = FindPreviousLoadingArea(area, entry)) {
        previousAreaText = GetLocalizedSceneText(previous->label);
        std::snprintf(path.data(), path.size(), "/ui/loadscreen/textures/headers/LD_%s_0.png", previous->header.data());
        loadingScreen.renderHandles[1] = d3t::d3t_AllocateRenderHandle();
        loadingScreen.headerRequests[1] = SetupUsageForTexture(path.data(), loadingScreen.renderHandles[1]);
    }

    if (g_currentMap == AREA_QEDJ && area == LOADING_NDAY) {
        areaText.clear();
        subAreaText.clear();
        previousAreaText.clear();
    } else if (fadeMode == 8) {
        UIWidget_Animate(loadingScreen.animations[LoadingText], 1, 0.3f, 0.0f);
    } else {
        UIWidget_Animate(loadingScreen.animations[LoadingBackground], 1, 0.3f, 0.0f);
        if (area != LOADING_TBCN && area != LOADING_GOV2 && !loadingScreen.collectionPoster)
            UIWidget_Animate(loadingScreen.animations[LoadingText], 1, 0.3f, 0.0f);
        const LoadingWindow header = loadingScreen.collectionPoster ? LoadingCollectionPoster : LoadingHeader;
        UIWidget_Animate(loadingScreen.animations[header], 1, 0.3f, 0.0f);
        const float locationDuration = area == LOADING_GOV2 || area == LOADING_TBCN ? 4.0f : 0.3f;
        UIWidget_Animate(loadingScreen.animations[LoadingLocation], 1, locationDuration, 0.0f);
        for (const LoadingWindow index : LOADING_FADE_IN_ORDER)
            UIWidget_Animate(loadingScreen.animations[index], 1, 0.3f, 0.0f);
    }
    UpdateLoadingScreenLabels();
}

std::int32_t g_shouldShowLoading{};

// @verified - p
void InstallLoadingScreenCallbacks() noexcept {
    g_updateLoadingScreen = UpdateLoadingScreenEnable;
    g_toggleLoadingScreen = ToggleLoadingScreen;
}

// @verified - p
void UpdateLoadingScreenEnable() {
    if (GetNumFramesProcessed() != 0 || GetIsLoadScreenActive())
        EnableElement(UIElement::LoadingScreen);
}
// @verified - p
void ToggleLoadingScreen(std::int32_t vSyncCount, std::int32_t mode) {
    if (vSyncCount != 0) {
        if (mode == 0) {
            DestroyElementByIndex(UIElement::LoadingScreen);
            g_shouldShowLoading = 0;
        }
        return;
    }
    ShowElementByIndex(UIElement::LoadingScreen);
}

// @verified - p
bool GetIsLoadScreenActive() noexcept {

    for (const LoadingWindow index : LOADING_ACTIVE_ORDER) {
        const std::uint32_t animation = static_cast<std::uint32_t>(loadingScreen.animations[index]);
        if (animation < g_uiWidgetBindings.size() && g_uiWidgetBindings[animation].animating)
            return true;
    }
    return false;
}
// @verified - p
std::int64_t PlatformIcons_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[static_cast<std::size_t>(UIElement::PlatformIcons)].config.uiHandleIndex);
    for (std::size_t index = 0; index < g_platformButtonIconImages.size(); ++index) {
        const std::int32_t image = UI_FindViewImage(slot, kPlatformButtonIconImageNames[index]);
        g_platformButtonIconImages[index] = image == -1 || slot > 75U ? nullptr : &g_uiViewSlots[slot].images[static_cast<std::size_t>(image)];
    }
    g_platformIconsSheet = -1;
    for (std::size_t sheet = 0; sheet < g_uiSpriteSheets.size(); ++sheet) {
        const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
        if (candidate.imageName && candidate.used == 1 && !std::strncmp("ButtonIcons", candidate.imageName, 0x40)) {
            g_platformIconsSheet = static_cast<std::int32_t>(sheet);
            break;
        }
    }
    return 1;
}

// @verified - p
void RenderingRelatedInventory(struct nk_image &image, std::uint16_t item, std::int32_t mask) {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[static_cast<std::size_t>(UIElement::SharedItems)].config.uiHandleIndex);
    if (slot > 75U)
        return;
    const unsigned sheet = (item >> 8U) + (mask == 1 ? 16U : 0U);
    const d3t::UIViewImage &source = g_uiViewSlots[slot].images[g_inventoryIconSheetImageIndices[sheet]];
    image.handle.id = source.textureHandle;
    image.w = source.width;
    image.h = source.height;
    const std::array<std::uint8_t, 4> cell = GetCommonIconRect(item);
    const float width = static_cast<float>(image.w), height = static_cast<float>(image.h);
    const float left = (static_cast<float>(cell[0]) * 0.125f) * width;
    const float top = height - (static_cast<float>(cell[3]) * 0.125f) * height;
    image.region[0] = static_cast<unsigned short>(static_cast<std::int32_t>(left));
    image.region[1] = static_cast<unsigned short>(static_cast<std::int32_t>(top));
    image.region[2] = static_cast<unsigned short>(static_cast<std::int32_t>((static_cast<float>(cell[2]) * 0.125f) * width - left));
    image.region[3] = static_cast<unsigned short>(static_cast<std::int32_t>(height - (static_cast<float>(cell[1]) * 0.125f) * height - top));
}

// @verified - p
std::int64_t GetIconSet() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[static_cast<std::size_t>(UIElement::SharedItems)].config.uiHandleIndex);
    if (slot <= 75U) {
        for (std::size_t sheet = 0; sheet < g_inventoryIconSheetImageIndices.size(); ++sheet)
            g_inventoryIconSheetImageIndices[sheet] = UI_FindViewImage(slot, sharedItemImageNames[sheet]);
    }
    return 1;
}

// @verified - p
std::int64_t SharedItems_IsPressed() {
    if (IsHelpOverlayVisible(UIElement::GameHelp) || !sharedItemsWidget.active || (g_inputBuffer[0].pressedPriority & 4U) == 0)
        return 1;
    RequestExecPause();
    return 0;
}

void InitializeNuklearUI(std::uint32_t renderHandle) {
    ReleaseFightGaugeDefinition();
    ReleaseCollectionMenuResources();
    ReleaseBigKanjiText();
    ResetBigKanjiText();
    InitializeUITransitions();
    actionSelectorMotionTransition = actionSelectorPulseTransition = actionSelectorButtonTransition = -1;
    cancelPressed = false;
    enterPressed = false;
    loadSaveDefinitionFailed.store(false, std::memory_order_relaxed);
    sleepSelectionActive = false;
    sleepSelectionIconId = 0;
    sharedItemsWidget = {};
    sharedItemsWidget.owner = -1;
    dialogueSelectionPrompt = {};
    subtitlePresentation = {};
    subtitleVoicePalettes = {};
    subtitleVoiceColours = {};
    subtitleCinemaOffset = 0.0f;
    loadingScreen = {};
    loadingScreen.labels.fill(-1);
    loadingScreen.images.fill(-1);
    ResetLoadingScreenDefinition();
    snapshotOrderCount = 0;
    for (SnapshotImageRuntime &image : snapshotImages) {
        image.generation.fetch_add(1, std::memory_order_acq_rel);
        const std::uint32_t texture = image.texture.exchange(0, std::memory_order_acq_rel);
        if (texture)
            d3t::InitTexture2DResource(texture);
        image.state.store(0, std::memory_order_release);
    }
    mainMenuSelection = MainMenuItem::NewGame;
    frontendRoute = FrontendRoute::Wheel;
    continueAvailable = ContinueModuleIsAvailable();
    mainMenuFrameWasRendered = false;
    mainMenuWheelFrameWasRendered = false;
    gameUiVisibilityFlags.fill(0);
    gameUiSecondaryFlags.fill(0);
    notebookRefreshFrame = 0;
    gameHudCompassImage = nullptr;
    gameHudCompassWindow = nullptr;
    gameScreenFadeColour = 0;
    InitializeUIElements();
    DrainUIFontRequests();
    FreeUIFontDefinition(uiFontLanguage);
    uiFontLanguage = g_sceneTextLanguage;
    uiFontRenderHandle = renderHandle;
    SelectAppropriateFontDefinition(uiFontLanguage, renderHandle);
    subtitleLabelRectangles.clear();
    previousRenderHandle = renderHandle;
    InitializeSplashScreens(renderHandle);
    LoadSplash();
    previousRenderHandle = 0;
    LoadPressStart();
    LoadMainMenu();
    LoadIntroCutsceneCredits();
    LoadGameQte();
    LoadBigKanjiText();
    LoadStreetFighterHud();
    LoadDuckRaceHud();
    LoadGameHudTimer();
    LoadEndingCredits();
    for (const std::size_t index : std::array<std::size_t, 13>{10, 11, 12, 13, 14, 20, 21, 22, 23, 24, 25, 70, 53})
        g_uiElements[index].parseDefinitionFromFile();
    LoadLoadingScreenDefinition();
    LoadMainMenuWheel();
    LoadMainMenuNew();
    LoadFightGaugeDefinition();
    LoadMainMenuContinue();
    LoadMainMenuCollection();
    LoadGameBackground();
    LoadInformationBar();
    LoadGameMenu();
    LoadItemsMenu();
    LoadSubMenuOptions();
    LoadOptionsControls();
    LoadOptionsAudio();
    LoadOptionsGraphics();
    LoadNativePrompt();
    LoadGameMoves();
    LoadMoveLearned();
    LoadLoadSave();
    LoadSubtitles();
    LoadCutsceneSkip();
    LoadActionSelector();
    LoadPawnSetFlyer();
    LoadPawnItemFlyer();
    LoadQteMinigame();
    LoadNativeGameHud();
    sharedItemImages.fill(nullptr);
    gameMessageLayout = {};
    gameMessageMainPanel = gameMessageStartPanel = gameMessageEndPanel = -1;
    gameMessagePrepared = false;
    gameMessageTransition = 0;
    gameMessageNextDelay = 0.0f;
    gameMessageText.fill('\0');
    LoadSharedItems();
    LoadHelpDefinition();
    LoadPlatformIcons();
    LoadGameMessage();
    LoadGameHudWatch();
    LoadNativeGameHud();
    LoadHudSnapshot();
}

// @verified - p
std::int64_t ExecuteElementCallback(UIElement elementIndex) {
    const std::uint32_t index = static_cast<std::uint32_t>(elementIndex);
    if (index >= static_cast<std::uint32_t>(UIElement::Count) ||
        (currentUIState != -1 && index != static_cast<std::uint32_t>(currentUIState) && (index - 58U) > 1U && elementIndex != UIElement::GameHud))
        return 0;
    const UIElementState &element = g_uiElements[index];
    if (element.render && g_debugImGuiRenderMode == 1) {
        SetGameUiFlag(gameUiSecondaryFlags, index, true);
        return 0;
    }
    if (!element.unknown28)
        return 0;
    SetGameUiFlag(gameUiSecondaryFlags, index, true);
    return element.unknown28();
}

// @verified - p
void SetIsDrawingUI(std::int32_t drawing) {
    g_waitingForNextTick = drawing;
}

// @verified - p
bool IsNonBettingHelpTopic() {
    return ((static_cast<std::uint32_t>(helpGetCurrentHelp()) - 65U) & ~8U) != 0;
}

void ShowElementByIndex(UIElement elementIndex) {
    const std::int32_t value = static_cast<std::int32_t>(elementIndex);
    if (value < 0 || value >= static_cast<std::int32_t>(UIElement::Count)) {
        return;
    }

    if (currentUIState != -1 && value != currentUIState && static_cast<std::uint32_t>(value - 58) > 1U && elementIndex != UIElement::GameHud) {
        return;
    }

    UIElementState &element = g_uiElements[static_cast<std::size_t>(value)];
    if (!element.isVisible) {
        if (elementIndex == UIElement::GameMenu || (elementIndex == UIElement::GameHelp && !IsGameUIActive()))
            g_uiHelpAdmission = 1;
        if (element.tick) {
            element.tick();
            if (element.render) {
                const bool wasDrawing = drawingUI;
                drawingUI = false;
                SetIsDrawingUI(0);
                element.render();
                SetIsDrawingUI(1);
                drawingUI = wasDrawing;
            }
        }
        if (element.initialize) {
            element.initialize();
            if (element.postInit) {
                SetIsDrawingUI(0);
                element.postInit();
                SetIsDrawingUI(1);
            }
        }
        element.isVisible = 1;
        SetGameUiFlag(gameUiVisibilityFlags, static_cast<std::size_t>(value), true);
    }
    element.isRenderable = 0;
}

// @verified - p
void DisableElement(UIElement elementIndex) {
    const std::uint32_t index = static_cast<std::uint32_t>(elementIndex);
    if (index >= static_cast<std::uint32_t>(UIElement::Count))
        return;
    UIElementState &element = g_uiElements[index];
    if (!element.isRenderable)
        return;
    element.isRenderable = 0;
    if (element.resetRender)
        element.resetRender();
}

// @verified - p
void EnableRenderableByIndex(UIElement elementIndex) {
    const std::uint32_t index = static_cast<std::uint32_t>(elementIndex);
    if (index >= static_cast<std::uint32_t>(UIElement::Count))
        return;
    UIElementState &element = g_uiElements[index];
    if (element.isRenderable)
        return;
    element.isRenderable = 1;
    if (element.resetRender)
        element.resetRender();
}

// @verified - p
void SetUIRenderSuppression(std::int32_t suppressed) {
    for (UIElementState &element : g_uiElements) {
        const std::int32_t value = suppressed == 1 ? 1 : 0;
        if (value ? element.isRenderable != 0 : element.isRenderable == 0)
            continue;
        element.isRenderable = value;
        if (element.resetRender)
            element.resetRender();
    }
}

void SetMovesMenuItemActionState(std::int32_t state) noexcept {
    gameMovesItemActionState = state;
}

std::uint32_t ReInitElement(UIElement elementIndex) {
    const auto index = static_cast<std::uint32_t>(elementIndex);
    if (index >= g_uiElements.size())
        return 3;
    const auto parser = g_uiElements[index].parseDefinitionFromFile;
    return parser ? parser() : 3;
}

// @verified - p
void PublishBigKanjiText(const char *spriteName, LegacyIconSprite *sprites, std::int32_t count) {
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::BigKanjiText)];
    d3t ::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(element.config.uiHandleIndex);
    if (!view)
        return;
    view->imagesLoading = count;
    for (std::uint32_t index = 0; index < static_cast<std::uint32_t>(count); ++index) {
        BigKanjiGlyph &glyph = bigKanjiGlyphs[index];
        glyph.sprite = sprites + index;
        std::array<char, 260> path;
        std::snprintf(path.data(), path.size(), "/ui/BigKanjiText/Textures/%s/moji_%d.tga", spriteName, index);
        glyph.texture = SetupUsageForTexture(path.data(), 0);
        glyph.admittedTexture = 0;
        if (bigKanjiImages[index])
            bigKanjiImages[index]->image = static_cast<std::int32_t>(index);
    }
}

// @verified - p
void PublishBigKanjiGlyph(std::int32_t index, std::uint32_t colour) {
    if (static_cast<std::uint32_t>(index) >= bigKanjiGlyphs.size())
        return;
    BigKanjiGlyph &glyph = bigKanjiGlyphs[index];
    glyph.requested = 1;
    glyph.colour = (colour & 0xff00ff00U) | ((colour >> 16U) & 0xffU) | ((colour & 0xffU) << 16U);
}

// @verified - p
void ReleaseBigKanjiText() {
    for (BigKanjiGlyph &glyph : bigKanjiGlyphs) {
        d3t::InitTexture2DResource(glyph.admittedTexture);
        glyph.admittedTexture = 0;
    }
    const UIElementState &element = g_uiElements[static_cast<std ::size_t>(UIElement::BigKanjiText)];
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(element.config.uiHandleIndex);
    if (view)
        view->imagesLoading = 0;
}

std::int64_t DestroyElementByIndex(UIElement elementIndex) {
    const std::int32_t value = static_cast<std::int32_t>(elementIndex);
    if (value < 0 || value >= static_cast<std::int32_t>(UIElement::Count)) {
        return 0;
    }

    if (currentUIState != -1 && value != currentUIState && static_cast<std::uint32_t>(value - 58) > 1U && elementIndex != UIElement::GameHud) {
        return 0;
    }
    UIElementState &element = g_uiElements[static_cast<std::size_t>(value)];
    if (!element.isVisible)
        return 1;
    if (elementIndex == UIElement::GameMenu || (elementIndex == UIElement::GameHelp && !IsGameUIActive())) {
        g_uiHelpAdmission = 0;
        g_uiHelpAdmissionValue = 0.0f;
    }
    const UIAction destroy = element.destroy ? element.destroy : element.unknown60;
    if (destroy && static_cast<std::uint32_t>(destroy()) != 1U)
        return 0;
    element.isVisible = 0;
    SetGameUiFlag(gameUiVisibilityFlags, static_cast<std::size_t>(value), false);

    return 1;
}

bool IsSharedItemsWidgetOwnedBy(std::int32_t owner) noexcept {
    return sharedItemsWidget.active != 0 && (owner == -1 || sharedItemsWidget.owner == owner);
}
bool IsHelpOverlayVisible(UIElement element) noexcept {
    const auto index = static_cast<std::int32_t>(element);
    return index >= 0 && index < static_cast<std::int32_t>(g_uiElements.size()) && g_uiElements[static_cast<std::size_t>(index)].isVisible != 0;
}
bool IsElementEnabled(UIElement element) noexcept {
    const auto index = static_cast<std::int32_t>(element);
    return index >= 0 && index < static_cast<std::int32_t>(g_uiElements.size()) && g_uiElements[static_cast<std::size_t>(index)].enabled != 0;
}
bool IsElementAvailable(UIElement element) noexcept {
    const auto index = static_cast<std::int32_t>(element);
    return index >= 0 && index < static_cast<std::int32_t>(g_uiElements.size()) &&
        g_uiElements[static_cast<std::size_t>(index)].parseDefinitionFromFile != nullptr;
}

// Whether a UI element's bit is set in the frame's secondary element flags.
bool IsGameUiSecondaryFlagSet(std::int32_t index) {
    return ((gameUiSecondaryFlags[static_cast<std::size_t>(index) / 32] >> (index & 31)) & 1U) != 0;
}

bool GetIsFreeInputType(std::int32_t type) {

    if ((IsHelpOverlayVisible(UIElement::GameHelp) && !IsNonBettingHelpTopic()) || GetIsKeyboardActive() == 1 ||
        (static_cast<std::uint32_t>(ReturnZero()) != 1U && g_uiFreeInputOverride == 0))
        return false;
    constexpr std::size_t skip = static_cast<std::size_t>(UIElement::CutsceneSkip);
    constexpr std::uint32_t skipFlag = 1U << (skip & 31U);
    if (g_uiElements[skip].isVisible || (g_uiRefreshedElements[skip / 32] & skipFlag) || (g_uiPreviousRefreshedElements[skip / 32] & skipFlag))
        return true;
    if (type >= 0) {
        if (type <= 2 || type == 6)
            return true;
        if (type <= 5)
            return false;
    }
    return GetCurrentUIInputType() != static_cast<std::int32_t>(InputType::Qte2) && GetCurrentUIInputType() != static_cast<std::int32_t>(InputType::Fight) &&
        GetCurrentUIInputType() != static_cast<std::int32_t>(InputType::Qte);
}

// @verified - p
void UpdateCompass(float rotation) noexcept {
    gameHudCompassRotation = rotation;
}

// @verified - p
void UpdateMinuteHand(float rotation) noexcept {
    gameHudMinuteHandRotation = rotation;
}

// @verified - p
void UpdateHourHand(float rotation) noexcept {
    gameHudHourHandRotation = rotation;
}

// Shows or hides the compass ring and sets its window's alpha (percent), not a rotation.
// @verified - p
void UpdateCompassImage(std::int32_t enabled, std::int32_t alpha) noexcept {
    if (gameHudCompassImage) {
        gameHudCompassImage->field44 = enabled == 1;
        if (gameHudCompassWindow)
            gameHudCompassWindow->alpha = static_cast<float>(alpha) / 100.0f;
    }
}
bool HandleGameUIVisibility() {
    if (GetNumFramesProcessed() == 0) {
        if (GetHelpIconEnabled())
            return false;
        const std::uint32_t flags = gameUiVisibilityFlags[2];
        const bool helpOverlay = IsHelpOverlayVisible(UIElement::GameHelp);
        const bool gameUiActive = IsGameUIActive();
        const std::int32_t inputType = GetCurrentUIInputType();
        return (!g_inputBuffer[0].connectionState && !helpOverlay && IsCameraIconHidden() == 1) || (flags & 0x10000U) != 0 ||
            (((!gameUiActive && inputType != 1) || IsSharedItemsWidgetOwnedBy(-1)) && inputType != 0 && !helpOverlay);
    }

    if ((currentUIState != -1 && currentUIState != static_cast<std::int32_t>(UIElement::GameHelp)) || !IsHelpOverlayVisible(UIElement::GameHelp)) {
        return false;
    }
    DestroyElementByIndex(UIElement::GameHelp);
    return false;
}

std::int64_t ShowSharedItemsWidget(std::uint32_t itemType, std::uint32_t owner) {
    sharedItemsWidget.itemType = itemType;
    sharedItemsWidget.active = 1;
    sharedItemsWidget.owner = static_cast<std::int32_t>(owner);
    sharedItemsWidget.published = 1;
    ShowElementByIndex(UIElement::SharedItems);
    return 1;
}

std::int64_t DestroySharedItemsWidget() {
    sharedItemsWidget = {};
    sharedItemsWidget.owner = -1;
    DestroyElementByIndex(UIElement::SharedItems);
    return 1;
}

void EnableElement(UIElement element) {
    const std::size_t index = static_cast<std::size_t>(element);
    if (index < g_uiElements.size()) {
        g_uiElements[index].enabled = 1;
    }
}

// @verified - p: SetArcadeHelpContext.
void SetArcadeHelpContext(std::int32_t context) {
    helpWidgetArcadeContext = context;
}

// @verified - p: ShowArcadeHelp.
void ShowArcadeHelp(std::uint8_t topic) {
    helpWidgetMode = topic == 155;
    EV_EventSetUpHelpIcon_0(topic, 255);
    SetupCurrentHelpIcon();
    StartHelpBlinkTransition();
    EnableElement(UIElement::GameHelp);
}

// @verified - p: CloseVisibleUIElements.
void CloseVisibleUIElements() {
    for (std::uint32_t index = 0; index < g_uiElements.size(); ++index) {
        if (currentUIState != -1 && index != static_cast<std::uint32_t>(currentUIState) && (index < 58 || index > 59) && index != 64)
            continue;
        auto &element = g_uiElements[index];
        if (!element.isVisible)
            continue;
        if (index == 6 || (index == 80 && !IsGameUIActive())) {
            g_uiHelpAdmissionValue = 0.0f;
            g_uiHelpAdmission = 0;
        }
        const UIAction destroy = element.destroy ? element.destroy : element.unknown60;
        if (!destroy || static_cast<std::uint32_t>(destroy()) == 1U)
            element.isVisible = 0;
    }
}

// @verified - p: ResetGameMenuActive.
void ResetGameMenuActive() {
    gameMenuActive = 0;
}
bool IsGameUIActive() {
    return IsHelpOverlayVisible(UIElement::GameMenu) || IsHelpOverlayVisible(UIElement::GameBackground);
}

// @verified - p
void UI_BeginFrame() {
    ApplyS2AspectRatio();
    for (std::size_t word = 0; word < g_uiRefreshedElements.size(); ++word) {
        g_uiPreviousRefreshedElements[word] = g_uiRefreshedElements[word];
        g_uiRefreshedElements[word] = 0;
    }
    const std::int32_t safeHeight = static_cast<std::int32_t>(UI_SafeAreaScale() * 1080.0f);
    const float safeWidth = static_cast<float>(UI_GetAspectWidth()) - (1080.0f - static_cast<float>(uiSafeAreaHeight));
    if (safeHeight != uiSafeAreaHeight || safeWidth != uiSafeAreaWidth) {
        uiSafeAreaWidth = safeWidth;
        uiSafeAreaHeight = safeHeight;
        informationBar.current.hideTarget = 1;
        for (d3t::UIWindow &window : g_uiWindows)
            window.rectCached = 0;
    }
}

void RenderUIFrame() {
    if (!d3t::BeginUIFrame())
        return;
    RenderUIContents();
}

void UpdateElements() {
    for (std::size_t index = 0; index < g_uiElements.size(); ++index) {
        UIElementState &element = g_uiElements[index];
        if (!element.setup)
            continue;
        if (element.config.init == 1 && ((element.someFlag3 & 1) || !previousRenderHandle) &&
            uiDefinitionReady[index].exchange(false, std::memory_order_acq_rel))
            ParseLoadedUIDefinition(element);
        if (element.config.init == 2 && AreUIDefinitionImagesReady(element)) {
            element.config.init = 3;
            if ((element.someFlag3 & 1) && previousRenderHandle)
                d3t::d3t_DecrementRenderHandleReference(previousRenderHandle);
            if (index == static_cast<std::size_t>(UIElement::Subtitles)) {
                LoadSubtitleStyles();
            }
            if (index == static_cast<std::size_t>(UIElement::FightGauge))
                BindFightGaugeUI();
            if (index == static_cast<std::size_t>(UIElement::SharedItems))
                PrepareSharedItemImages();
            if (index == static_cast<std::size_t>(UIElement::IntroCutsceneCredits))
                BindIntroCutsceneCredits();
            if (element.isSelected)
                element.isSelected();
            if (element.unknown88)
                element.unknown88();
        }
    }
}

// @verified - p
std::int64_t RefreshNotebook() {
    const std::uint64_t frame = d3t::GetNumRenderedFrames();
    if (notebookRefreshFrame >= frame)
        return 0;
    EnableElement(UIElement::NotebookPages);
    nk_clear(GetNativeUiContext());
    notebookRefreshFrame = d3t::GetNumRenderedFrames();
    g_uiFrameElementHistory.fill(0);
    g_uiFrameElementHistoryIndex = 0;
    return 1;
}

// @verified - p
void SetUIElementIndex(std::int32_t index) {
    g_uiFrameElementHistory[static_cast<std::size_t>(g_uiFrameElementHistoryIndex)] = index;
    ++g_uiFrameElementHistoryIndex;
}

// @verified - p
void ReinitializeUIFileElement(std::uint32_t index) {
    if (index >= g_uiElements.size())
        return;
    UIElementState &element = g_uiElements[index];
    if (element.setup) {
        if (element.reset)
            element.reset();
        ResetUIViewSlot(element.config.uiHandleIndex);
        const std::uint32_t request = GetDefaultUIFileRequest();
        element.config.screenWidth = static_cast<std::int16_t>(request);
        element.config.screenHeight = static_cast<std::int16_t>(request >> 16U);
        LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), element.config.uiHandleIndex);
        element.config.init = 2;
    } else if (element.unknown70) {
        element.unknown70();
    }
}

// @verified - p
UIElementState *FindUIElementByIdx(std::int32_t slotIndex) {
    for (UIElementState &element : g_uiElements)
        if (element.config.uiHandleIndex == slotIndex)
            return &element;
    return nullptr;
}

// @verified - p
void UpdateUI() {
    for (UIMouseCallbackQueue &queue : g_uiMouseCallbackQueues)
        queue.count = 0;
    for (UIElementState &element : g_uiElements) {
        if ((element.setup && element.config.init != 3) || element.isRenderable || g_uiDebugSuspendElements)
            element.config.enabled = 0;
        else if (element.enabled)
            element.config.enabled = 1;
        else if ((g_debugImGuiRenderMode == 1 || element.fieldAc) &&
            (element.isVisible || (element.unknown80 && static_cast<std::uint32_t>(element.unknown80()) != 0)))
            element.config.enabled = 1;
        else
            element.config.enabled = 0;

        if (element.setup && element.config.init != 3)
            element.config.status = 0;
        else if (element.unknown80 && static_cast<std::uint32_t>(element.unknown80()) != 0)
            element.config.status = 0;
        else if (g_uiDebugSuspendElements || element.isRenderable)
            element.config.status = 0;
        else if (element.enabled)
            element.config.status = 1;
        else
            element.config.status = (g_debugImGuiRenderMode == 1 || element.fieldAc) && element.isVisible;
    }
}

// @verified - p
void RefreshAll(std::uint32_t first, std::uint32_t end) {
    for (std::uint32_t index = first; index < end; ++index) {
        UIElementState &element = g_uiElements[index];
        if (!element.config.enabled)
            continue;
        UIAction callback = g_debugImGuiRenderMode == 1 ? element.render : element.postInit;
        if (!callback)
            callback = g_debugImGuiRenderMode == 1 ? element.postInit : element.render;
        if (!callback)
            continue;
        SetGameUiFlag(g_uiRefreshedElements, index, true);
        callback();
    }
}

// @verified - p
void HandleUIPressedStates() {
    if (g_debugImGuiRenderMode != 1)
        return;
    for (std::uint32_t index = static_cast<std::uint32_t>(UIElement::Count) - 1; index != 0; --index) {
        UIElementState &element = g_uiElements[index];
        if (element.config.status) {
            if (currentUIState == -1 || index == static_cast<std::uint32_t>(currentUIState) || (index - 58U) <= 1U || index == 64) {
                for (UIMouseCallbackQueue &queue : g_uiMouseCallbackQueues) {
                    for (std::int32_t entry = queue.count - 1; entry >= 0; --entry) {
                        UIMouseCallbackEntry &action = queue.entries[static_cast<std::size_t>(entry)];
                        if (action.element != element.index)
                            continue;
                        if (action.callback)
                            action.callback(action.rect.data(), action.data);
                        queue.count = 0;
                        break;
                    }
                }
            }
            if (element.isPressed && (currentUIState == -1 || index == static_cast<std::uint32_t>(currentUIState) || (index - 58U) <= 1U || index == 64)) {
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                if (element.isPressed() == 0)
                    return;
            }
        } else if (element.enabled) {
            SetGameUiFlag(gameUiSecondaryFlags, index, true);
        }
    }
}

// @verified - p
void DrawViews() {
    ApplyS2AspectRatio();
    RefreshAll(0, 80);
    Nuklear_BindRenderTarget();
    NuklearUpdateForAspectRatio(GetAspectRatioModeShouldScale());
    NuklearDrawAspectBars(1.0f, GetAspectRatioModeShouldScale(), g_uiDebugRevealAspectBars);
    ApplyUnscaledS2AspectRatio();
    RefreshAll(80, static_cast<std::uint32_t>(UIElement::Count));
    NuklearUpdateForAspectRatio(GetAspectRatioModeShouldScale());
    CopyNuklearTexToBackBuffer();
    ApplyS2AspectRatio();
}

// @verified - p
std::intptr_t UpdateScreenScaling() {
    FontLoader();
    SetupCameraIcon();
    for (std::size_t word = 0; word < gameUiSecondaryFlags.size(); ++word) {
        g_uiPreviousInputElements[word] = gameUiSecondaryFlags[word];
        gameUiSecondaryFlags[word] = 0;
    }
    UpdateElements();
    RefreshNotebook();
    UIAnim_UpdateDeltaTime(GetFrameTime());
    UpdateUI();
    HandleUIStates();
    DrawViews();
    HandleUIPressedStates();
    for (UIElementState &element : g_uiElements)
        element.enabled = 0;
    return reinterpret_cast<std::intptr_t>(g_uiElements.data() + g_uiElements.size()) + offsetof(UIElementState, enabled);
}

// @verified - p
std::int32_t IsHudFightSpriteActive() noexcept {
    for (std::size_t index = 0; index < 5; ++index) {
        const UIButtonIconWidget *const sprite = nativeHudSpriteWidgets[index];
        if (sprite && static_cast<std::uint32_t>(sprite->sprite) - 47U <= 3U)
            return 1;
    }
    return 0;
}
void RenderUIContents() {
    for (UIMouseCallbackQueue &queue : g_uiMouseCallbackQueues)
        queue.count = 0;
    uiPointer = ConsumeWindowUIInput();
    DrainUIFontRequests();
    FontLoader();
    uiFontLanguage = g_sceneTextLanguage;
    SetupCameraIcon();
    for (std::size_t word = 0; word < gameUiSecondaryFlags.size(); ++word) {
        g_uiPreviousInputElements[word] = gameUiSecondaryFlags[word];
        gameUiSecondaryFlags[word] = 0;
    }
    ApplyS2AspectRatio();
    const float pointerXScale = GetUIViewportWidth() / static_cast<float>(d3t::g_originalScreenWidth);
    const float pointerYScale = 1080.0f / static_cast<float>(d3t::g_originalScreenHeight);
    uiPointer.position.x *= pointerXScale;
    uiPointer.position.y *= pointerYScale;
    uiPointer.previousPosition.x *= pointerXScale;
    uiPointer.previousPosition.y *= pointerYScale;
    UpdateElements();
    RefreshNotebook();

    UIAnim_UpdateDeltaTime(GetFrameTime());
    UpdateUI();
    HandleUIStates();
    if (g_uiElements[static_cast<std::size_t>(UIElement::Splash)].isVisible) {
        RenderScreenFilmFrame(static_cast<std::int32_t>(UIElement::Splash));
    }
    // ST fade quads belong to the scene pass, below every UI widget.
    for (std::size_t index = 0; index < screenColourOverlayCount; ++index) {
        const std::uint32_t colour = screenColourOverlays[index];
        std::uint32_t texture = d3t::g_defaultWhiteTexture;
        if ((colour & 0xffffffU) == 0) {
            if (!screenBlackTexture) {
                constexpr std::array<std::uint32_t, 4> blackPixels{0xff000000U, 0xff000000U, 0xff000000U, 0xff000000U};
                const d3t::Texture2DDescription description{2, 2, 1, 1, 2, 0, 0, 1, 0, 0, blackPixels.data()};
                screenBlackTexture = d3t::CreateTexture2D(&description);
            }
            texture = screenBlackTexture;
        }
        d3t::DrawUIImage(texture, 0.0f, 0.0f, static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight),
            static_cast<float>(colour >> 24U) / 255.0f);
    }
    screenColourOverlayCount = 0;
    for (std::size_t index = 0; index < g_uiElements.size(); ++index) {
        UIElementState &element = g_uiElements[index];
        if (index == static_cast<std::size_t>(UIElement::GameScreenFade) || index == static_cast<std::size_t>(UIElement::CinemaBars) ||
            index == static_cast<std::size_t>(UIElement::GameBackground)) {
            if (element.config.enabled) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::LoadSave) && loadSaveDefinitionFailed.load(std::memory_order_acquire)) {
            if (element.isVisible)
                DestroyElementByIndex(UIElement::LoadSave);
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::GameHud) || index == static_cast<std::size_t>(UIElement::GameHudWatch) ||
            index == static_cast<std::size_t>(UIElement::HudSnapshot) || index == static_cast<std::size_t>(UIElement::GameHudTimer) ||
            index == static_cast<std::size_t>(UIElement::NotebookPages)) {
            if ((element.enabled || element.isVisible) && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                if (index == static_cast<std::size_t>(UIElement::NotebookPages))
                    RenderMemoNotebookPages();
                else if (index == static_cast<std::size_t>(UIElement::GameHudTimer) || index == static_cast<std::size_t>(UIElement::GameHud) ||
                    index == static_cast<std::size_t>(UIElement::HudSnapshot) || index == static_cast<std::size_t>(UIElement::GameHudWatch))
                    RenderScreenFilmFrame(static_cast<std::int32_t>(index));
                else if (element.render)
                    element.render();
                else
                    d3t::DrawUIDefinition(uiDefinitions[index], 1.0f);
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::GameHudMoney)) {
            if (element.enabled && !element.isRenderable) {
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
                if (static_cast<std::uint32_t>(moneyHudState) - 2U <= 1U) {
                    SetGameUiFlag(g_uiRefreshedElements, index, true);
                    SetGameUiFlag(gameUiSecondaryFlags, index, true);
                }
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::StreetFighterHud)) {
            if (element.enabled && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::GameQte)) {
            if (element.config.enabled) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderGameQteFrame();
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::IntroCutsceneCredits)) {
            // Widget::Enable is a one-frame pulse, independent of ShowElement.
            if (element.enabled && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                DrawIntroCutsceneCredits();
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::CutsceneSkip)) {
            if (element.enabled && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderCutsceneSkip();
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::ActionSelector)) {
            if (element.enabled && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                DrawActionSelector();
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::GameMessage)) {
            if ((element.enabled || g_debugImGuiRenderMode == 1) && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                DrawGameMessage();
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::GameHelp)) {
            if ((element.enabled || element.isVisible) && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderHelpFrame();
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::Credits)) {
            if (element.isVisible && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            }
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::GameMenu)) {
            if (element.isVisible && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                RenderGameMenuFrame();
            }
            element.enabled = 0;
            continue;
        }
        if ((index >= static_cast<std::size_t>(UIElement::SubMenuItems) && index <= 15) || (index >= 20 && index <= 23) ||
            index == static_cast<std::size_t>(UIElement::LoadSave) || index == static_cast<std::size_t>(UIElement::YesNoPrompt) ||
            index == static_cast<std::size_t>(UIElement::GameInfoBar)) {
            if (element.config.enabled) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
            }
            if ((index == static_cast<std::size_t>(UIElement::GameInfoBar) || index == static_cast<std::size_t>(UIElement::LoadSave) ||
                    index == static_cast<std::size_t>(UIElement::YesNoPrompt)) &&
                element.config.enabled && !element.isRenderable &&
                (!g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isVisible ||
                    ((index == static_cast<std::size_t>(UIElement::YesNoPrompt) || index == static_cast<std::size_t>(UIElement::GameInfoBar)) &&
                        g_uiElements[static_cast<std::size_t>(UIElement::SubMenuOptions)].isVisible)))
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            element.enabled = 0;
            continue;
        }
        if (index == static_cast<std::size_t>(UIElement::PressStart) || index == static_cast<std::size_t>(UIElement::MainMenu)) {
            if (element.config.enabled && !element.isRenderable && element.config.init == 3) {
                SetGameUiFlag(g_uiRefreshedElements, index, true);
                SetGameUiFlag(gameUiSecondaryFlags, index, true);
                if (index == static_cast<std::size_t>(UIElement::PressStart))
                    RenderScreenFilmFrame(static_cast<std::int32_t>(index));
                else {
                    RenderScreenFilmFrame(static_cast<std::int32_t>(index));
                    mainMenuFrameWasRendered = true;
                }
            }
            element.enabled = 0;
            continue;
        }
        if (!element.isVisible || element.config.init != 3)
            continue;
        SetGameUiFlag(g_uiRefreshedElements, index, true);
        SetGameUiFlag(gameUiSecondaryFlags, index, true);
        element.enabled = 1;
        if (index != static_cast<std::size_t>(UIElement::SubMenuMoves) && index != static_cast<std::size_t>(UIElement::GameInfoBar) &&
            index != static_cast<std::size_t>(UIElement::MoveLearned) && index != static_cast<std::size_t>(UIElement::SubMenuItems) &&
            !(index >= 16 && index <= 19) && index != 58 && !(index >= 10 && index <= 25) && index != 29 && index != 53 &&
            index != static_cast<std::size_t>(UIElement::MainMenu) && index != static_cast<std::size_t>(UIElement::MainMenuWheel) &&
            index != static_cast<std::size_t>(UIElement::PawnshopFlyer1) && index != static_cast<std::size_t>(UIElement::PawnshopFlyer2) &&
            index != static_cast<std::size_t>(UIElement::QteMinigame) && index != static_cast<std::size_t>(UIElement::LoadingScreen) && element.tick)
            element.tick();
        if ((index >= 16 && index <= 19) || index == 58) {
            if (!element.isRenderable && element.render)
                element.render();
            continue;
        }
        if (element.isRenderable)
            continue;
        if (index == static_cast<std::size_t>(UIElement::MainMenuWheel)) {
            RenderMainMenuWheelFrame();
            mainMenuWheelFrameWasRendered = g_mainMenuWheelWidget != nullptr;
            continue;
        }
        if (index != static_cast<std::size_t>(UIElement::Splash) && index != static_cast<std::size_t>(UIElement::FightGauge) &&
            index != static_cast<std::size_t>(UIElement::ActionSelector) && index != static_cast<std::size_t>(UIElement::GameMessage) &&
            index != static_cast<std::size_t>(UIElement::SharedItems)) {
            if (index == static_cast<std::size_t>(UIElement::NotebookPages)) {
                RenderMemoNotebookPages();
            } else if (index == static_cast<std::size_t>(UIElement::PressStart)) {
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            } else if (index == static_cast<std::size_t>(UIElement::MoveLearned)) {
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            } else if (index == static_cast<std::size_t>(UIElement::BigKanjiText) || index == static_cast<std::size_t>(UIElement::DuckRace)) {
                RenderScreenFilmFrame(static_cast<std::int32_t>(index));
            } else {
                if (((index >= 10 && index <= 25) || index == 29 || index == 53 || index == 64 ||
                        index == static_cast<std::size_t>(UIElement::PawnshopFlyer1) || index == static_cast<std::size_t>(UIElement::PawnshopFlyer2) ||
                        index == static_cast<std::size_t>(UIElement::QteMinigame) || index == static_cast<std::size_t>(UIElement::LoadingScreen)) &&
                    element.render)
                    element.render();
                else
                    d3t::DrawUIDefinition(uiDefinitions[index], 1.0f);
                if (index == static_cast<std::size_t>(UIElement::Subtitles))
                    DrawSubtitleLabels();
                if (index == static_cast<std::size_t>(UIElement::MainMenu))
                    mainMenuFrameWasRendered = true;
            }
        }
    }


    for (std::size_t index = g_uiElements.size(); index-- > 1;) {
        auto &element = g_uiElements[index];
        if (!(gameUiSecondaryFlags[index / 32] & (1U << (index & 31))) || element.config.init != 3 || element.isRenderable)
            continue;
        if (currentUIState != -1 && static_cast<std::int32_t>(index) != currentUIState && index != 58 && index != 59 && index != 64)
            continue;
        for (UIMouseCallbackQueue &queue : g_uiMouseCallbackQueues) {
            for (std::int32_t entry = queue.count - 1; entry >= 0; --entry) {
                UIMouseCallbackEntry &action = queue.entries[static_cast<std::size_t>(entry)];
                if (action.element != element.index)
                    continue;
                if (action.callback)
                    action.callback(action.rect.data(), action.data);
                queue.count = 0;
                break;
            }
        }
        if (currentUIState != -1 && static_cast<std::int32_t>(index) != currentUIState && index != 58 && index != 59 && index != 64)
            continue;
        if (element.isPressed && element.isPressed() == 0)
            break;
    }
}

void SubmitScreenColourOverlay(std::uint32_t colour) {
    if (screenColourOverlayCount < screenColourOverlays.size())
        screenColourOverlays[screenColourOverlayCount++] = colour;
}

void SetScreenFilmColour(std::uint32_t colour) {
    if (colour)
        gameScreenFadeColour = (colour & 0xff00ff00U) | ((colour & 0x00ff0000U) >> 16U) | ((colour & 0x000000ffU) << 16U);
}

// @verified - p
d3t::UIWindow *ToggleCinemaBars(std::int32_t mode) {
    cinemaBarMode = mode;
    if (subtitlePanels[0])
        subtitlePanels[0]->layout.offsetY = mode == 1 ? -0.1388889f : -0.018518519f;
    return subtitlePanels[0];
}

void BeginSleepSelectionPrompt(std::int32_t iconState) {



    sleepSelectionIconId = iconState == 1 ? 271 : (iconState == 2 ? 15 : 266);
    enterPressed = false;
    cancelPressed = false;
    sleepSelectionActive = true;
    ShowElementByIndex(UIElement::PlatformIcons);
    EnableElement(UIElement::PlatformIcons);
}

SleepSelectionResult PollSleepSelectionResult() {
    if (!sleepSelectionActive)
        return SleepSelectionResult::Pending;
    if (cancelPressed) {
        cancelPressed = false;
        return SleepSelectionResult::Cancelled;
    }
    if (enterPressed) {
        enterPressed = false;
        return SleepSelectionResult::Selected;
    }
    return SleepSelectionResult::Pending;
}

void EndSleepSelectionPrompt() {
    sleepSelectionActive = false;
    enterPressed = false;
    cancelPressed = false;
    sleepSelectionIconId = 0;
    DestroyElementByIndex(UIElement::PlatformIcons);
}

std::int32_t GetSleepSelectionIconId() {
    return sleepSelectionIconId;
}

void BeginDialogueSelectionPrompt(const std::array<std::uint16_t, 4> &flagIds, std::uint16_t timeoutFrames, std::uint16_t defaultFlagId) {
    EndDialogueSelectionPrompt();
    std::array<std::int16_t, 4> choices{};
    for (std::size_t index = 0; index < choices.size(); ++index)
        choices[index] = static_cast<std::int16_t>(flagIds[index]);
    SetupControlSpeakAndUI(choices.data(), timeoutFrames, static_cast<std::int16_t>(defaultFlagId), 2);
    speakSetFlagTo(timeoutFrames);
    dialogueSelectionPrompt.defaultFlagId = defaultFlagId;
    dialogueSelectionPrompt.timeoutFrames = timeoutFrames;
    dialogueSelectionOwned = true;
}

DialogueSelectionResult PollDialogueSelectionPrompt(std::uint16_t &flagId) {
    flagId = 0xffff;
    if (!dialogueSelectionOwned)
        return DialogueSelectionResult::Pending;
    // SPEAK owns both the input and timeout. Polling must not tick either.
    if (ctrlSpeakResultChoiceId != 0xffff) {
        flagId = static_cast<std::uint16_t>(ctrlSpeakResultChoiceId);
        return dialogueSelectionTimedOut ? DialogueSelectionResult::TimedOut : DialogueSelectionResult::Selected;
    }
    if (ctrlSpeakIconState == 3 || ctrlSpeakIconState == 4 || (!ctrlSpeakOpenRequested && ctrlSpeakIconState == 0))
        return dialogueSelectionTimedOut ? DialogueSelectionResult::TimedOut : DialogueSelectionResult::Cancelled;
    return DialogueSelectionResult::Pending;
}

void EndDialogueSelectionPrompt() {
    if (!dialogueSelectionOwned)
        return;
    dialogueSelectionOwned = false;


    if (ctrlSpeakIconState < 3 && ctrlSpeakOpenRequested) {
        CloseSpeakIcon(1);
        padcRefreshCurrentInputs(false);
        if ((ctrlSpeakSavedLoadingFlags & 0x20U) != 0)
            g_loadingFlag = ctrlSpeakSavedLoadingFlags;
        ctrlSpeakSavedLoadingFlags = 0;
        dialogueSelectionPrompt = {};
        DestroyElementByIndex(UIElement::ActionSelector);
    }
}

bool SetDialogueSelectionFocus(std::size_t slot) {
    if (!dialogueSelectionPrompt.active || !IsDialogueSelectionSlotAvailable(slot)) {
        return false;
    }
    dialogueSelectionPrompt.focusedSlot = static_cast<std::int32_t>(slot);
    return true;
}

void MoveDialogueSelectionFocus(std::int32_t delta) {
    if (!dialogueSelectionPrompt.active || delta == 0)
        return;
    const std::int32_t direction = delta < 0 ? -1 : 1;
    std::int32_t slot = dialogueSelectionPrompt.focusedSlot;
    if (slot < 0) {
        dialogueSelectionPrompt.focusedSlot = FirstDialogueSelectionSlot();
        return;
    }
    for (std::size_t count = 0; count < dialogueSelectionPrompt.flagIds.size(); ++count) {
        slot = (slot + direction + static_cast<std::int32_t>(dialogueSelectionPrompt.flagIds.size())) %
            static_cast<std::int32_t>(dialogueSelectionPrompt.flagIds.size());
        if (IsDialogueSelectionSlotAvailable(static_cast<std::size_t>(slot))) {
            dialogueSelectionPrompt.focusedSlot = slot;
            return;
        }
    }
}

bool GetDialogueSelectionPromptState(DialogueSelectionPromptState &state) {
    state = dialogueSelectionPrompt;
    return state.active;
}

void NotifyUIEnterPressed() {
    enterPressed = true;
}

void NotifyUICancelPressed() {
    cancelPressed = true;
}
bool ConsumeUIEnterPressed() {
    const bool pressed = enterPressed;
    enterPressed = false;
    return pressed;
}

bool ConsumeUICancelPressed() {
    const bool pressed = cancelPressed;
    cancelPressed = false;
    return pressed;
}

} // namespace shenmue::s2
