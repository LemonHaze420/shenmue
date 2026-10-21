#pragma once
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
namespace ui_detail {}
using namespace ui_detail;

extern std::int32_t g_loadScreenSpriteTexture;

extern std::uint32_t g_helpIconState;

extern std::array<UIElementState, static_cast<std::size_t>(UIElement::Count)> g_uiElements;

extern std::int32_t requestedCameraIconMode;

extern UIButtonWheel *g_mainMenuWheelWidget;

extern std::int32_t g_mainMenuWheelIndex;

extern std::int32_t g_mainMenuWheelInputBlock;

extern std::uint8_t g_mainMenuWheelPending;

extern std::array<std::int32_t, 35> g_mainMenuWheelImages;

namespace ui_detail {

std::int64_t BindMainMenuDefinition();
extern std::array<std::vector<std::byte>, static_cast<std::size_t>(UIElement::Count)> uiDefinitionData;

extern std::array<d3t::UIDefinition, static_cast<std::size_t>(UIElement::Count)> uiDefinitions;

extern std::size_t loadSaveSelectedSlot;

extern CameraState loadSaveSavedCameraState;

extern std::int32_t currentUIState;

extern std::int32_t splashScreenIndex;

extern bool enterPressed;

extern bool forceSkipSplashScreens;

inline constexpr std::array<const char *, 9> PRESS_START_LOGO_IMAGES{"GameLogo", "GameLogo_KR", "GameLogo_SC", "GameLogo_White", "GameLogo_White_KR",
    "GameLogo_White_SC", "GameLogo_Shadow", "GameLogo_Shadow_KR", "GameLogo_Shadow_SC"};
inline constexpr std::array<const char *, 2> PRESS_START_MURAL_IMAGES{"Press_Start::Colour_Image::Mural_White", "Press_Start::Colour_Image::Mural_Blur"};
inline constexpr std::array<const char *, 4> PRESS_START_LOGO_WIDGETS{"Press_Start::Logo_Panel::Logo_Image", "Press_Start::Logo_White::Logo_White_Image",
    "Press_Start::Logo_Shadow_Left::Logo_Shadow_Left_Image", "Press_Start::Logo_Shadow_Right::Logo_Shadow_Right_Image"};
inline constexpr std::array<const char *, 2> PRESS_START_ICON_WIDGETS{"PressStart::Icon_Console", "PressStart::Icon_PC"};

// @provisional-layout: IDA has no UDT; the title-light data and consumer establish the fields and 0x50-byte stride.
struct PressStartLightBand {
    float begin;
    float end;
    float value;
};
static_assert(sizeof(PressStartLightBand) == 12 && offsetof(PressStartLightBand, value) == 8);
struct PressStartLightState {
    float duration;
    float phase;
    std::int32_t direction;
    std::uint8_t paused;
    std::array<std::byte, 3> reserved0d;
    std::int32_t shadowWindow;
    Vector2 shadowScale;
    float shadowAmplitude;
    std::array<PressStartLightBand, 3> bands;
    std::array<std::byte, 12> reserved44;
};
static_assert(sizeof(PressStartLightState) == 0x50 && offsetof(PressStartLightState, bands) == 0x20);
static_assert(offsetof(PressStartLightState, phase) == 4 && offsetof(PressStartLightState, direction) == 8);
static_assert(offsetof(PressStartLightState, shadowWindow) == 16 && offsetof(PressStartLightState, shadowScale) == 20);
inline constexpr std::array<PressStartLightState, 2> PRESS_START_LIGHT_INITIAL{{
    {.duration = 16.0f,
        .phase = 0.0f,
        .direction = 1,
        .paused = 0,
        .reserved0d = {},
        .shadowWindow = 20,
        .shadowScale = {1.5f, 0.75f},
        .shadowAmplitude = 25.0f,
        .bands = {{{.begin = -0.5f, .end = 0.5f, .value = 0.0f}, {.begin = 0.25f, .end = 0.75f, .value = 0.0f}, {.begin = 0.5f, .end = 1.5f, .value = 0.0f}}},
        .reserved44 = {}},
    {.duration = 16.0f,
        .phase = 0.5f,
        .direction = 1,
        .paused = 0,
        .reserved0d = {},
        .shadowWindow = 21,
        .shadowScale = {-1.5f, -0.75f},
        .shadowAmplitude = 25.0f,
        .bands = {{{.begin = -0.5f, .end = 0.5f, .value = 0.0f}, {.begin = 0.25f, .end = 0.75f, .value = 0.0f}, {.begin = 0.5f, .end = 1.5f, .value = 0.0f}}},
        .reserved44 = {}},
}};
struct PressStartRuntime {
    bool frozen{};
    std::array<std::int32_t, 9> images{};
    std::array<d3t::UIWindow *, 23> windows{};
    std::array<std ::int32_t, 23> transitions{};
    std::array<UIColourImageWidget *, 2> murals{};
    std ::array<UIButtonIconWidget *, 2> icons{};
    std::array<d3t::UIImageWidget *, 4> logos{};
    d3t ::UILabelWidget *prompt{};
    std::int32_t angleTransition{-1};
    std::int32_t blinkTransition{-1};
    std::array<PressStartLightState, 2> lights = PRESS_START_LIGHT_INITIAL;
};
static_assert(offsetof(UIButtonIconWidget, fontColour) == 0x9c && offsetof(UIButtonIconWidget, colour) == 0xb0);
static_assert(offsetof(d3t::UIViewImage, rotate) == 0x34 && offsetof(d3t::UIViewImage, angle) == 0x38);
void HideUIWindow(d3t::UIWindow *window);
void ShowUIWindow(d3t::UIWindow *window);
void UpdatePressStartTransitions();
void UpdatePressStartLight(PressStartLightState &light, std::int32_t firstWindow);
extern PressStartRuntime pressStartRuntime;

extern bool mainMenuFrameWasRendered;

extern d3t::UILabelWidget *mainMenuUserIdLabel;
extern char mainMenuUserIdText;

extern bool mainMenuWheelFrameWasRendered;

extern bool cancelPressed;

extern bool sleepSelectionActive;

extern DialogueSelectionPromptState dialogueSelectionPrompt;

extern bool dialogueSelectionOwned;

extern SubtitlePresentationState subtitlePresentation;

extern std::array<std::array<std::uint32_t, 3>, 4> subtitleVoicePalettes;

extern std::array<std::uint32_t, 3> subtitleVoiceColours;

extern float subtitleCinemaOffset;

extern std::vector<d3t::UIResolvedLabel> subtitleLabelRectangles;

extern std::int32_t uiFontLanguage;

extern MainMenuItem mainMenuSelection;

enum class LoadingLabelSlot : std::size_t {
    Time,
    Date,
    Area,
    SubArea,
    PreviousArea,
    Loading,
    Count,
};

struct LoadSaveRuntime {
    UIGrid grid{};
    UIGridTemplate cellTemplate{};
    std::array<SaveMenuSlot, S2SaveSlotCount> slots{};
    std::array<d3t::UIWindow *, 9> windows{};
    std::array<d3t::UILayoutItem *, 12> layouts{};
    std::array<d3t::UILabelWidget *, 7> labels{};
    std::array<std::array<char, 256>, 7> text{};
    std::array<UIColourImageWidget *, 10> colourImages{};
    std::array<UIButtonWidget *, 2> buttons{};
    std::array<d3t::UIViewImage *, 8> images{};
    std::array<std::int32_t, 22> mapImages{};
    std::array<d3t::UIViewImage *, 6> areaImages{};
    d3t::UIImageWidget *mapImage{};
    d3t::UIImageWidget *areaImage{};
    const SaveMenuSlot *selected{};
    std::array<char, 256> resultText{};
    bool writing{};
    bool bound{};
};
extern LoadSaveRuntime loadSaveRuntime;

struct AuthoredMenuLayout {
    std::vector<std::int32_t> parents, objectPanels, labelPanels;
    std::vector<d3t::UIRect> panels;
    std::vector<d3t::UIResolvedTextureObject> objects;
    std::vector<d3t::UIResolvedLabel> labels;
};
void BindAuthoredMenuLayout(const d3t::UIDefinition &definition, AuthoredMenuLayout &layout);

struct QteMinigameRuntime {
    AuthoredMenuLayout layout;
    std::array<std::int32_t, 11> panels;
    std::array<std::int32_t, 8> directions;
    std::array<float, 11> opacity{};
    MT7Model *model{};
    const std::uint32_t *titleButtons{};
};
extern QteMinigameRuntime qteMinigame;

extern std::int32_t gameMenuActive;

void ResolveUniqueItemAction(std::int32_t mode);
bool ConfirmGameMenuClose();
extern WindowUIInput uiPointer;

extern bool drawingUI;

struct InformationBarRuntime {
    UIMessageBox current{}, previous{};
    std::array<std::array<char, 256>, 5> text{};
    std::array<char, 513> previousText{};
    std::array<d3t::UIWindow *, 7> windows{};
    std::array<d3t::UILabelWidget *, 5> labels{};
    std::array<d3t::UIImageWidget *, 3> images{};
    std::array<std::int32_t, 3> backgrounds{-1, -1, -1};
    float remaining{};
    bool bound{};
};
void ClearMenuInputHints() noexcept;

void SetInformationBarVisibilityMode(std::int32_t mode) noexcept;

extern InformationBarRuntime informationBar;

void RestorePreviousInformationBarMessage();

struct SnapshotImageRuntime {
    std::atomic<std::uint32_t> texture{};
    std::atomic<std::uint32_t> generation{};
    std::atomic<std::uint32_t> state{};
};

struct SnapshotImageLoadRequest {
    std::uint32_t slot{};
    std::uint32_t generation{};
    std::filesystem::path path;
};
extern std::array<SnapshotImageRuntime, 126> snapshotImages;

extern std::array<std::uint32_t, 126> snapshotOrder;

extern std::uint32_t snapshotOrderCount;

void QueueSnapshotImageLoad(std::uint32_t slot);

void RefreshSnapshotOrder();

extern FrontendRoute frontendRoute;

extern bool continueAvailable;

extern HLib::Task *openingStaffRollTask;

extern d3t::UIPanelDefinition *introLogoPanel;

extern d3t::UITextureObjectDefinition *introLogoImage;

extern HLib::Task *endingStaffRollTask;

extern std::uint32_t currentItemSelectState;

extern std::array<CommonIconAtlas, 16> commonIconAtlases;

extern const std::array<std::uint8_t, 0x280> commonIconRectangles;

extern const std::array<std::uint16_t, 16> commonIconRectangleOffsets;

extern std::uint32_t commonIconAtlasCount;

extern std::uint32_t renderIconFlags;

extern std::uint32_t activeIconFlags;

extern PadControlEntry *iconPadControl;

extern PadControlEntry *previousIconPadControl;



struct GuiPtxHeader {
    std::uint32_t tag;
    std::uint32_t size;
    std::uint32_t idLow;
    std::uint32_t idHigh;
    std::uint32_t gbixTag;
    std::uint32_t gbixSize;
    std::uint32_t gbixIndex;
    std::uint32_t pvrTag;
    std::uint32_t pvrSize;
    std::uint32_t format;
    std::uint16_t width;
    std::uint16_t height;
    std::uint32_t reserved;
};
extern std::int32_t moneyHudCashAdjustment;

extern std::uint8_t actionSelectorInput;


struct NumericActionSelector {
    std::int32_t mode{};
    std::int32_t amount{};
    std::int32_t step{};
    std::int32_t maximum{};
    std::array<std::int32_t, 4> choices{};
    std::array<std::array<char, 256>, 4> labels{};
};
extern NumericActionSelector numericActionSelector;

extern std::array<char, 256> gameMessageText;

extern std::int32_t gameMessageTransition;

extern float gameMessageNextDelay;

extern std::int32_t gameMessageMode;

extern std::int32_t gameMessageIcon;

inline constexpr std::size_t guiPtxPrefixSize = sizeof(GuiPtxHeader) + 0x80;
inline constexpr std::size_t guiPtxBackingSize = guiPtxPrefixSize + 1024 * 16 * 4;
inline constexpr std::size_t guiPtxTextSize = guiPtxPrefixSize + 256 * 16 * 4;
extern std::byte *guiPtxBacking;

extern alignas(4) std::array<std::byte, NOW_QTE_PTX_STORAGE_SIZE> nowQtePtx;

extern std::uint32_t nowQteTexture;

extern std::array<std::int32_t, 4> ctrlSpeakTexturePoolHandles, ctrlSpeakChoiceTextHandles;

extern std::array<std::uint32_t, 4> ctrlSpeakTexturePoolFlags;

extern HLib::Task *speakIconTask;

extern HLib::Task *nowQteTask;

extern HLib::Task *mainIconTask;

extern HLib::Task *itemSelectTask;

extern HLib::Task *itemLookTask;

extern HLib::Task *uniqueItemActionTask;

extern HLib::Task *moneyIconTask;

extern HLib::Task *helpIconTask;

extern HLib::Task *itemEffectIconTask;

extern ConversationBuffer *itemEffectAudio;

extern HLib::Task *sleepActionIconTask;

extern ItemSelectorCamera itemSelectorCamera;

extern ItemSelectorContext itemSelectorContext;

extern bool itemSelectorCameraActive;

extern std::uint32_t itemSelectorNavigationInput;

extern std::uint16_t itemSelectorDelayedResult;

extern std::array<std::uint8_t, 64> itemSelectorOwnedMask;

extern const std::uint8_t *itemSelectorPublishedMask;

extern std::int32_t mainIconPostFxBank;

extern std::uint8_t mainIconPhase;

extern std::int16_t itemSelectorLabelTexture;

extern alignas(8) ItemSelectorSelection itemSelectorSelection;

extern const char *itemSelectorLabel;

void ClampItemSelectorSelection();

void RefreshItemSelectorSelection();

void RefreshItemSelectorSelectedItem();

void RefreshItemSelectorLabel();

extern PadControlEntry *sleepActionPadControl;

extern PadControlEntry *sleepActionPreviousPadControl;

extern std::int32_t mainIconDelay;

extern std::uint32_t mainIconOverlayActive;

extern std::int8_t itemSelectPhase;

extern std::uint8_t moneyIconPhase, moneyIconDrawMode, moneyIconCurrency;

extern std::int32_t moneyIconFlags, moneyIconTarget, moneyIconDisplay, moneyIconMaximum, moneyIconStepAmount;

extern std::int32_t moneyIconFixedStep, moneyIconAmount, moneyIconResult, moneyIconLastIncrement;

extern float moneyIconFrames, moneyIconSettleFrames, moneyIconSelectionScale;

extern std::uint16_t moneyIconSavedHelpSelected, moneyIconSavedHelpVisible, moneyIconSavedHelpPresent;

extern MoneyIconPosition moneyIconPosition, moneyIconSelectorPosition;

extern LegacyIconSprite moneyIconSprite;

extern std::array<LegacyIconSprite, 4> moneyIconDirections;

extern std::array<LegacyIconSprite, 2> moneyIconTextSprites, moneyIconSigns;

extern std::uint16_t sleepActionStatus;

extern std::int32_t sleepActionSubmenuState;

extern std::uint8_t quickIconPhase, quickIconUnpressed;

extern std::uint16_t quickIconFlags, quickIconRequiredStatus, quickIconButtonMask;

extern std::int16_t quickIconCurrentStatus, quickIconTerminalStatus;

extern float quickIconMaximumFrames, quickIconElapsedFrames, quickIconDrawFrames, quickIconPreviousDrawFrames;

extern std::array<float, 4> quickIconPosition;

extern float quickIconPositionDepth;

extern std::uint16_t quickIconPositionFlags;

extern LegacyIconSprite quickIconSprite;

extern std::uint32_t quickIconLegacyMode, quickIconLegacyReserved;

extern std::int16_t quickIconLegacyTexture, quickIconLegacyWidth;

extern std::uint32_t quickIconLoadingReleaseFlag;

extern std::int32_t itemEffectIconPhase;

extern std::int8_t itemEffectIconFrames;

extern ItemEffectPosition itemEffectPosition;

extern std::int16_t itemEffectLabelTexture;

extern std::uint8_t itemEffectColumns;

extern LegacyIconSprite itemEffectSprite;

extern std::int32_t itemEffectAmount;

extern const char *itemEffectName;

extern std::int32_t sleepActionFlags;

extern std::uint16_t sleepActionPhase;

extern std::int8_t sleepActionCurrentSelection, sleepActionTargetSelection;

extern std::int32_t sleepActionRetargetFrames;

extern std::int16_t sleepActionPublishedSelection;

extern std::array<float, 4> sleepActionPosition;

extern float sleepActionScale;

extern std::array<LegacyIconSprite, 4> sleepActionArrowSprites, sleepActionTextSprites;

extern std::array<LegacyIconSprite, 4> ctrlSpeakChoiceDrawSprites;

extern std::array<std::uint16_t, 3> ctrlSpeakRadialFrameIds;

extern const std::uint16_t *ctrlSpeakSelectedFrameIds;

extern std::int32_t nativeGuiActive;

extern std::array<std::uint32_t, 3> gameUiVisibilityFlags;

extern std::array<std::uint32_t, 3> gameUiSecondaryFlags;

extern std::int32_t previousCameraIconMode;

extern std::int32_t currentCameraIconMode;

extern std::int8_t easyOptionControllerState;

extern std::int8_t easyOptionMode;

extern bool easyOptionShowsOptions;

extern bool easyOptionPreviousClockSuppression;

extern float easyOptionPosition;

extern float easyOptionVelocity;

extern PadControlEntry *easyOptionPadControl;

extern PadControlEntry *easyOptionPreviousPadControl;

extern HLib::Task *easyOptionTask;

extern std::uint16_t easyOptionSavedHelpVisible;

extern std::uint8_t easyOptionSavedHelpSelected;

extern std::array<std::uint8_t, 3> easyOptionDraft;

extern std::array<std::int16_t, 2> easyOptionDescriptionTextures;

extern std::array<std::int16_t, 15> easyOptionLabelTextures;

extern EasyOptionCaption easyOptionValueCaption;

extern EasyOptionCaption easyOptionDescriptionCaption;

extern LegacyIconSprite easyOptionTextSprite, easyOptionButtonSprite;

extern std::int16_t easyOptionPulse, easyOptionPulseStep;

struct PosePreviewRecord {
    void *source{};
    void *model{};
    std::array<char, 16> characterName{};
    std::int16_t state{};
    std::array<std::byte, 6> reserved22{};
};
extern std::array<PosePreviewRecord, 8> posePreviewRecords;

extern std::uint8_t easyOptionSelectedRow;

extern std::uint8_t easyOptionPreviousValue;

extern std::uint32_t easyOptionPreviousNpcState;

extern std::byte *scenarioItemPtx;

extern std::uint32_t commonItemTexture;

extern std::uint32_t scenarioItemTexture;

} // namespace ui_detail

std::span<const std::byte> SetupPtxTexture(std::uint16_t width, std::uint32_t idLow, std::uint32_t idHigh);

namespace ui_detail {

std::int64_t ReleaseCtrlSpeakTextHandle(std::int32_t handle);
void ReleaseSpeakTextSlots();

void PrepareSpeakTextSlots();
extern std::int8_t ctrlSpeakIconState;

extern bool ctrlSpeakOpenRequested, ctrlSpeakHasSpecialLabel, ctrlSpeakForceTextLabels;

extern std::int32_t ctrlSpeakMarkerGlyphIndex, ctrlSpeakDedicatedCancelEnabled;

extern std::array<std::int16_t, 4> ctrlSpeakChoiceIds, ctrlSpeakChoiceWidths, ctrlSpeakTargetChoiceWidths, ctrlSpeakChoiceWidthDeltas;

extern std::uint16_t ctrlSpeakMode, ctrlSpeakDefaultChoiceId, ctrlSpeakResultChoiceId, ctrlSpeakSelectedChoiceSlot;

extern std::int16_t ctrlSpeakElapsedFrames, ctrlSpeakTimeoutFrames;

extern std::int32_t ctrlSpeakSelectionAnimationFrame;

extern std::uint32_t ctrlSpeakPulseAngle, ctrlSpeakSavedLoadingFlags;

extern std::array<float, 4> ctrlSpeakSelectorOrigin;

extern float ctrlSpeakSpecialLabelWidthScale, ctrlSpeakHalfWidthExpansion, ctrlSpeakLeftOriginX, ctrlSpeakRightOriginX;

extern float ctrlSpeakResizeFrame, ctrlSpeakSelectorScale, ctrlSpeakRadialScale, ctrlSpeakRetainedMotion;

extern bool dialogueSelectionTimedOut;

void DrawCtrlSpeakSelector(void *);
std::uint32_t ConsumeActionSelectorInput();
extern HelpIconState helpIconState;

extern std::uint32_t helpIconDeferredRemoval, iconSelectFlags, helpIconPreviousLoadingFlags, helpIconPublishedVisible;

extern std::int32_t helpIconItemEffectState, helpIconItemEffectFrames, helpIconItemPromptFrames, helpIconBlinkFrames, helpIconBlinkVisible;

extern std::array<HelpDirectionBlink, 4> helpDirectionBlinks;

extern float currentIconDeltaTime;

extern std::int16_t iconDeltaTime;

extern std::uint32_t helpIconDrawVisible, helpIconDrawSelected, qteForcedHelp;

extern std::int32_t helpIconHudDelay, helpIconRefreshDelay, qteCameraAtTop;

extern std::array<std::int16_t, 4> helpIconHintIds;

extern std::array<std::uint16_t, 4> helpIconHintSelected;

extern std::uint32_t helpIconHintAngle;

extern std::array<std::uint32_t, 4> helpIconHintAngles;

extern std::int32_t qteControlState;

extern std::uint32_t qteControlMask;

extern std::int32_t nowQteDrawSuppression;

extern std::int32_t qteDrawFrames;

std::int32_t FirstDialogueSelectionSlot();

extern std::array<d3t::UIWindow *, 5> subtitlePanels;

extern std::array<d3t::UIImageWidget *, 2> subtitleBackgrounds;

extern std::array<std::array<std::int32_t, 3>, 3> subtitleNativeLabels;

extern std::array<std::uint32_t, 20> subtitleNativeVoicePalettes;

extern std::int32_t subtitleActivityTransition;

extern std::array<std::int32_t, 2> subtitleAlphaBindings;

inline constexpr char SPLASH_WINDOW_ID[] = "Splash::Panel";
extern d3t::UIWindow *splashWindow;
extern std::array<BigKanjiGlyph, 8> bigKanjiGlyphs;
extern std::array<d3t::UIWindow *, 8> bigKanjiPanels;
extern std::array<UIColourImageWidget *, 8> bigKanjiImages;
extern std::array<std::array<char, 256>, 4> streetFighterHudText;
extern std::array<d3t::UIWindow *, 2> streetFighterHudWindows;
extern std::array<d3t::UILabelWidget *, 4> streetFighterHudLabels;
extern std::array<std::array<char, 256>, 7> duckRaceHudText;
extern std::array<d3t::UIWindow *, 13> duckRaceHudWindows;
extern std::array<d3t::UILabelWidget *, 7> duckRaceHudLabels;
extern std::array<UIButtonIconWidget *, 6> duckRaceHudBowties;
std::int64_t SplashSelected();
std::int64_t ResetSplash();
std::int64_t DestroySplash();
struct SplashImageDefinition {
    const char *texturePath;
    const char *audioPath;
    float duration;
    float maximumDuration;
    float fadeOutDuration;
    float minimumDuration;
};

struct SplashRuntime {
    std::uint32_t request{};
    std::uint32_t textureHandle{};
    struct nk_image image {};
    std::byte *audioData{};
    std::uint64_t audioSize{};
    const SplashImageDefinition *definition{};
};
static_assert(sizeof(SplashImageDefinition) == 0x20 && offsetof(SplashImageDefinition, duration) == 0x10);
static_assert(sizeof(SplashRuntime) == 0x40 && offsetof(SplashRuntime, image) == 8);
static_assert(offsetof(SplashRuntime, audioData) == 0x28 && offsetof(SplashRuntime, audioSize) == 0x30 && offsetof(SplashRuntime, definition) == 0x38);
inline constexpr std::array<const char *, 7> SPLASH_LANGUAGE_SUFFIXES{"", "jp", "fr", "de", "kr", "tc", "sc"};
std::uint32_t LoadSplashImage(SplashRuntime &runtime);
bool ValidateSplashImage(SplashRuntime &runtime);
bool UI_SetImageFromTexture(struct nk_image &image, std::uint32_t texture);
extern const std::array<SplashImageDefinition, 6> splashImages;

extern std::array<SplashRuntime, 6> splashRuntime;

extern std::int32_t splashScreenMode;

extern std::uint64_t splashStateStart;

extern bool splashSkipPressed;

void InitializeSplashScreens(std::uint32_t renderHandle);

void ProcessSplashScreen();

void DrawSplashImage(const SplashRuntime &runtime, float opacity);
std::int64_t SplashRender();
std::int64_t HandleSplashInput();
extern nk_panel splashPanel;
struct SubtitleLabelFit {
    float height{};
    std::size_t split{std::string_view::npos};
};
SubtitleLabelFit FitSubtitleLabel(
    const UIFontSelection &font, std::string_view text, const d3t::UIRect &bounds, float referenceWidth, float referenceHeight, bool allowWrap);

void DrawSubtitleLabels();

void LoadSubtitleStyles();

std::uint32_t ReadUIDefinitionFile(UIElement elementIndex, const char *path, std::int32_t flags);

void DeleteSnapshotSlot(std::uint32_t slot);

std::uint32_t LoadSplash();

std::uint32_t LoadPressStart();

std::uint32_t LoadMainMenu();

std::uint32_t LoadMainMenuWheel();

std::uint32_t LoadMainMenuNew();

std::uint32_t LoadMainMenuContinue();

std::uint32_t LoadMainMenuCollection();

bool FindUIParentBounds(std::string_view id, d3t::UIRect &bounds, const d3t::UIPanelDefinition **authored = nullptr);
std::array<float, 2> GetUILayoutPaddingScale();

void DrawMovesLabel(
    const d3t::UILabelDefinition &label, const d3t::UIResolvedLabel &placement, std::string_view text, float opacity, d3t::UIDefinition *definition = nullptr);

bool FindUIParentBounds(std::string_view id, d3t::UIRect &bounds, const d3t::UIPanelDefinition **authored);

std::int32_t MenuPanelIndex(const d3t::UIDefinition &definition, std::string_view id);

void BindAuthoredMenuLayout(const d3t::UIDefinition &definition, AuthoredMenuLayout &layout);

bool AskKeepOrRevertChanges();
void FormatGameMenuNumber(std::span<char> output, std::int32_t value);

void DrawNativeMenuObject(
    d3t::UIDefinition &definition, const d3t::UITextureObjectDefinition &object, const d3t::UIResolvedTextureObject &placement, float opacity);

bool ResolveSharedItemImage(std::uint16_t icon, d3t::UISharedSpriteImage &image, bool silhouette = false);
void ActivateItemsRow(float *, void *data);

void DrawInventoryGridCell(UIGrid *grid, const Vector4 *bounds, const std::int32_t *dictionary, std::int32_t ordinal);


struct NativePromptRuntime {
    std::array<d3t::UIWindow *, 3> windows{};
    std::array<d3t::UIImageWidget *, 2> labelImages{};
    std::array<UIButtonIconWidget *, 4> sprites{};
    std::array<std::int32_t, 16> images{};
    std::array<char, 1024> text{};
    void (*callback)(std::int32_t){};
    std::int32_t selected{1}, timer{-1}, blink{-1};
    bool bound{};
    bool active{}, useString{}, blinking{}, limited{};
};
extern NativePromptRuntime nativePrompt;

std::uint32_t LoadNativePrompt();

void EnterNativePrompt(float *, void *data);
void ClickNativePrompt(float *, void *data);
void EnterNativePrompt(float *, void *data);

void ClickNativePrompt(float *bounds, void *data);

void NativeMessageBox(std::string_view text, bool useString, std::int32_t clearAfter, void (*callback)(std::int32_t), float seconds = 0.0f);

enum class OptionKind : std::uint8_t { Toggle, List, Slider, Key, Heading };
enum class OptionChoices : std::uint8_t {
    None,
    OffOn,
    AudioOutputs,
    AudioLanguages,
    SubtitleLanguages,
    Dialogue,
    WindowModes,
    Displays,
    Resolutions,
    Bloom,
    Aspect
};
struct OptionSpec {
    OptionKind kind;
    const char *token;
    const char *description;
    std::uint16_t offset;
    OptionChoices choices{};
    std::int16_t minimum{}, maximum{};
    bool byte{};
};
constexpr std::uint16_t AudioOptionOffset(std::size_t offset) {
    return static_cast<std::uint16_t>(offsetof(S2UserConfiguration280, audio) + offset);
}
extern const std::array<OptionSpec, 7> audioOptions;

constexpr OptionSpec KeyOption(const char *token, std::int32_t action) {
    return {OptionKind::Key, token, token, static_cast<std::uint16_t>(offsetof(S2UserConfiguration280, keyMappings) + action * sizeof(std::int32_t))};
}
extern const std::array<OptionSpec, 31> controlsOptions;

extern const std::array<OptionSpec, 11> graphicsOptions;

struct OptionChildRuntime {
    UIGrid grid{};
    AuthoredMenuLayout layout;
    std::array<std::int32_t, 31> dictionary{};
    std::int32_t count{}, root{-1};
    bool bound{};
};
struct OptionsRuntime {
    AuthoredMenuLayout layout;
    std::array<std::int32_t, 4> tabs{-1, -1, -1, -1};
    std::array<std::int32_t, 5> kinds{-1, -1, -1, -1, -1};
    std::vector<std::int8_t> panelKinds;
    std::int32_t rowRoot{-1}, root{-1}, current{-1}, requested{}, editing{-1};
    std::int32_t savedInteger{}, duplicate{-1}, pendingKey{-1};
    float savedFraction{};
    struct SliderDrag {
        float previousPointer{};
        bool active{};
    };
    std::array<std::array<SliderDrag, 31>, 3> sliderDrag{};
    std::int32_t sliderTrack{-1};
    d3t::UILabelDefinition keyIconLabel;
    std::uint32_t previousKey{256};
    CameraState savedCamera{};
    bool closing{}, cameraRequested{};
    bool tabFocus{};
    std::int32_t hoveredTab{};
    std::array<char, 1024> keyQuestion{};
};
extern OptionsRuntime options;

extern const OptionSpec *selectedOptionDescription;

extern std::array<OptionChildRuntime, 3> optionChildren;

extern const std::array<UIElement, 3> optionChildElements;

void ApplyOptions();
void ConfirmOptionsReset(std::int32_t response);
std::uint32_t LoadSubMenuOptions();

std::uint32_t LoadOptionsControls();

std::uint32_t LoadOptionsAudio();

std::uint32_t LoadOptionsGraphics();

void ApplyOptions();

bool AskKeepOrRevertChanges();

void ConfirmOptionsReset(std::int32_t response);

void RegisterOptionsAndPrompt();

void DrawNativeMenuObject(
    d3t::UIDefinition &definition, const d3t::UITextureObjectDefinition &object, const d3t::UIResolvedTextureObject &placement, float opacity);

inline constexpr std::array<const char *, 17> UI_GAME_HUD_WINDOWS{"GameHUD::MainHudParent", "GameHUD::ButtonBParent", "GameHUD::ButtonBPanel",
    "GameHUD::ButtonB", "GameHUD::ButtonBIcon", "GameHUD::ButtonAPanel", "GameHUD::ButtonA", "GameHUD::ButtonAIcon", "GameHUD::ButtonYPanel",
    "GameHUD::ButtonY", "GameHUD::ButtonYIcon", "GameHUD::ButtonXPanel", "GameHUD::ButtonX", "GameHUD::ButtonXIcon", "GameHUD::CameraPanel",
    "GameHUD::CameraButtonPanel", "GameHUD::CameraIconPanel"};
inline constexpr std::array<const char *, 6> UI_GAME_HUD_SPRITES{
    "GameHUD_ConfirmSprite", "GameHUD_CancelSprite", "GameHUD_EventSprite", "GameHUD_SystemSprite", "GameHUD_CameraSprite", "GameHUD_CameraStateSprite"};
inline constexpr std::array<const char *, 5> UI_GAME_HUD_BUTTONS{
    "GameHUD_ConfirmIcon", "GameHUD_CancelIcon", "GameHUD_EventIcon", "GameHUD_SystemIcon", "GameHUD_CameraIcon"};
inline constexpr const char *UI_GAME_HUD_CAMERA_LABEL = "Camera_Label";
inline constexpr const char *UI_GAME_HUD_CAMERA_BASE = "HUDCameraBase";
inline constexpr const char *UI_HUD_SNAPSHOT_SPRITE = "GameHUDSnapshot::CameraIconPanel::CameraStateSprite";
inline constexpr float UI_HUD_KEYBOARD_Y_SHIFT = 0.0092592593f;
extern std::array<d3t::UIWindow *, 17> nativeHudWindows;
extern std::array<UIButtonIconWidget *, 6> nativeHudSpriteWidgets;
extern std::array<UIButtonIconWidget *, 5> nativeHudButtonWidgets;
extern d3t::UILabelWidget *nativeHudCameraLabel;
extern UIColourImageWidget *nativeHudCameraBase;
extern std::array<std::int32_t, 5> nativeHudTransitions;
extern std::int32_t nativeHudBlink, nativeHudKeyboardMode;
void NativeGameHudClicked(float *rectangle, void *window);
void RefreshNativeHudHints(std::uint32_t *hints);
std::int64_t UpdateGameHudCameraVisibility();
std::int64_t NativeGameHudNullsub();
std::int64_t NativeGameHudInputZero();

extern std::array<std::uint32_t, 5> nativeHudRequestedHints, nativeHudShownHints, nativeHudOverrideHints;

extern std::int32_t nativeHudItemPromptIcon;


struct CollectionItemRow {
    std::int32_t type{}, index{}, quantity{}, iconIndex{};
    const char *label{};
    std::int32_t flyer{}, reserved1c{};
};
struct CollectionMapRow {
    std::int32_t area{}, image{};
    const char *label{};
    std::int32_t heading{}, reserved14{};
};
struct CollectionAuthoredLayout : AuthoredMenuLayout {
    std::vector<d3t::UIPanelBoundsOverride> overrides;
};
void BindCollectionLayout(const d3t::UIDefinition &definition, CollectionAuthoredLayout &layout);

void ResolveCollectionNativeLayout(
    const d3t::UIDefinition &definition, CollectionAuthoredLayout &layout, const d3t::UIPanelBoundsOverride *explicitOverride = nullptr);

struct CollectionChildRuntime {
    UIGrid grid{};
    CollectionAuthoredLayout layout;
    std::vector<std::uint8_t> rowPanels;
    std::int32_t holder{-1}, rowRoot{-1};
    std::array<d3t::UIWindow *, 5> windows{};
    std::array<d3t::UILayoutItem *, 4> layoutItems{};
    UIGridTemplate cellTemplate{};
    std::array<UIColourImageWidget *, 7> photoImages{};
    std::array<std::int32_t, 7> photoViewImages{};
    UIButtonWidget *photoButton{};
    d3t::UILabelDefinition rowLabel;
    std::array<char, 32> quantity{};
    bool bound{};
};
extern const std::array<UIElement, 7> collectionChildren;

extern const std::array<const char *, 7> collectionFiles;

extern const std::array<const char *, 29> collectionMapImages;

extern const std::array<shen_name, 12> collectionPawnShops;

extern const std::array<shen_name, 48> collectionBioNames;

extern std::array<CollectionChildRuntime, 7> collectionChild;

extern d3t::UIImageWidget *collectionMapPreview;

extern std::array<std::int32_t, 29> collectionMapImageIndices;

extern std::array<CollectionItemRow, 512> collectionGacha;

extern std::array<CollectionItemRow, 64> collectionEventPhotos;

extern std::array<CollectionItemRow, 12> collectionPawn;

extern std::array<CollectionMapRow, 28> collectionMaps;

extern std::array<WfMenuEntry, 25> collectionScrolls;

extern std::array<std::array<std::int32_t, 2>, 54> collectionSecretRows;

extern std::array<CollectionAlbum, 48> collectionAlbums;

extern std::array<std::uint32_t, 126> collectionViewerOrder;

extern std::uint32_t collectionViewerCount, collectionViewerIndex;

extern UIElement collectionViewerReturn;

extern bool collectionViewerWrap, collectionViewerBack;

extern CollectionAuthoredLayout collectionViewerLayout, collectionPresentLayout;

extern std::array<std::array<char, 256>, 7> collectionViewerText;

extern std::vector<std::uint32_t> collectionPresentTextures;

extern std::uint32_t collectionPresentIndex;

extern std::array<std::uint8_t, 126> collectionSnapshotPriority;

void PumpCollectionSnapshotImages();
const d3t::UIImageDefinition *CollectionLocalizedMapImage(std::int32_t index);
void DrawCollectionPostcard(const d3t::UISharedSpriteImage &, const d3t::UIRect &, const d3t::UIRect &, float, float, bool);
extern float collectionSnapshotLoadingTime;

extern std::uint32_t collectionSnapshotDeleteClick;

void DrawCollectionSnapshotStatus(std::uint32_t, const d3t::UIRect &, const d3t::UIRect &, bool);
extern UIElement collectionPresentReturn;

void DrawCollectionDefinition(UIElement element, AuthoredMenuLayout &layout, std::span<const std::uint8_t> excluded = {});

void OpenCollectionSnapshotViewer(std::span<const std::uint32_t> order, std::uint32_t selected, UIElement returning, bool wrap);

void OpenCollectionPresent(std::uint32_t group);
void OpenCollectionPawnFlyer(std::int32_t index);
void RequestCollectionSnapshotDelete(std::uint32_t slot);
extern float collectionSnapshotDeleteAlpha;

template <std::size_t Child> std::int64_t TickCollectionChild();
template <std::size_t Child> std::int64_t TickCollectionChild();

struct CollectionTabsRuntime {
    UIGridTabSelector nativeTabs{};
    std::array<void *, 10> nativeWindows{};
    std::array<void *, 4> nativeLabels{};
    std::array<UIColourImageWidget *, 11> nativeImages{};
    std::array<UIButtonWidget *, 4> nativeButtons{};
    std::int32_t shown{-1};
    bool bound{};
};
extern CollectionTabsRuntime collectionTabs, photosTabs;

extern std::array<UIElementState *, 7> collectionMapLanguageElements;

extern std::array<const d3t::UIImageDefinition *, 25> collectionLocalizedMaps;

extern std::int32_t collectionMapLanguage;

extern std::int32_t pendingCollectionMapLanguage;

extern bool collectionMapLanguagePending;

const d3t::UIImageDefinition *CollectionLocalizedMapImage(std::int32_t index);

void DrawCollectionPostcard(
    const d3t::UISharedSpriteImage &image, const d3t::UIRect &container, const d3t::UIRect &clip, float border, float opacity, bool framed);

struct CollectionUnderlayAlpha {
    std::int32_t object{-1};
    std::uint8_t alpha{};
};
extern std::array<CollectionUnderlayAlpha, 3> collectionUnderlayAlpha;

extern bool collectionUnderlayHidden;

extern std::uint32_t collectionPendingDelete;

void RequestCollectionSnapshotDelete(std::uint32_t slot);

extern bool collectionPresentFramed;

void OpenCollectionPresent(std::uint32_t group);

struct CollectionPawnFlyer {
    CollectionAuthoredLayout layout;
    UIGrid grid;
    const PawnShopRecord *shop{};
    std::vector<std::int32_t> order;
    std::vector<std::uint32_t> scores;
    std::vector<PawnStockCount> scratch;
    std::int32_t holder{-1}, index{};
    bool bound{};
};
extern CollectionPawnFlyer collectionPawnFlyer;

std::int64_t TickCollectionPawnFlyer();
void OpenCollectionPawnFlyer(std::int32_t index);

std::int64_t TickCollectionPawnFlyer();

void RegisterCollectionAndPhotos();

void PumpCollectionSnapshotImages();

void DrawCollectionSnapshotStatus(std::uint32_t slot, const d3t::UIRect &rectangle, const d3t::UIRect &clip, bool selected);

void ReleaseCollectionMenuResources();

inline constexpr const char *UI_HUD_WATCH_COMPASS_IMAGE = "GameHUDWatch_CompassPanel::CompassImage";
inline constexpr const char *UI_HUD_WATCH_TIME_MARKER = "TIME";
extern d3t::UIImageWidget *gameHudCompassImage;
extern d3t::UIWindow *gameHudCompassWindow;
extern std::array<char, 0x200> gameHudWatchTimeText;
extern float gameHudCompassRotation, gameHudMinuteHandRotation, gameHudHourHandRotation;
std::int32_t GetHourMinSecond(std::int32_t *hour, std::int32_t *minute, std::int32_t *second);

inline constexpr std::array<const char *, 3> UI_MOVE_LEARNED_LABELS{"Text_Line_1", "Text_Line_2", "Text_Line_3"};
inline constexpr const char *UI_HUD_TIMER_PANEL = "GameHudTimer::TimerPanel";
inline constexpr const char *UI_HUD_TIMER_LABEL = "GameHudTimer:TimerLabel";
extern std::array<d3t::UILabelWidget *, 3> moveLearnedLabels;
extern std::array<char, 256> gameHudTimerText;
extern d3t::UIWindow *gameHudTimerPanel;
extern d3t::UILabelWidget *gameHudTimerLabel;
extern UIButtonIconWidget *hudSnapshotCameraState;

void ConstructHudCameraState(std::uint32_t mode, UIButtonIconWidget *widget);

std::uint32_t LoadSubtitles();

std::int64_t BindSubtitles();

std::int64_t ResetSubtitleBindings();

void PublishActionSelectorText(std::size_t slot, std::string_view text);

float ActionSelectorPosition();

void SelectActionSelectorButton(std::int32_t slot);

bool ResolveSharedItemImage(std::uint16_t icon, d3t::UISharedSpriteImage &image, bool silhouette);

float GameMessagePosition();

void ApplyGameMessageContent();

void FormatMoneyAmount(char *output, std::int32_t value, bool cash);

void FormatGameMessageAmount(std::int32_t value, bool cash);

std::int64_t TickPressStart();

std::int64_t PressStartSelected();

std::int64_t PressStartInput();

std::int64_t PressStartRender();

std::int64_t LoadSaveSelected();

std::int64_t LoadSaveResetNullsub();

std::int64_t SetLoadSaveTheme();

std::int64_t LoadSaveHints();

std::int64_t LoadSaveInput();

std::int64_t LoadSaveTick();

std::int64_t LoadSaveRender();

std::int64_t LoadSaveDestroy();

std::int64_t MainMenuRender();

std::int64_t MainMenuSelected();

std::int64_t TickMainMenu();

std::int64_t ActivateMainMenuChoice(MainMenuItem selection);
std::int64_t BindMainMenuWheel();

std::int64_t MainMenuWheelRender();

void RenderMainMenuWheelFrame();
std::int64_t TickMainMenuWheel();

std::int64_t DestroyMainMenuWheel();

std::int64_t MainMenuWheelInput();

std::int64_t ActivateMainMenuChoice(MainMenuItem selection);

extern std::int32_t helpWidgetRunCount, helpWidgetLineCount, helpWidgetIconRunCount, helpWidgetBoxCount;

extern std::uint8_t helpWidgetRelayout, helpWidgetBoxesLocked;

extern std::array<HelpWidgetRun, 64> helpWidgetRuns;

extern std::array<HelpWidgetBox, 4> helpWidgetBoxes;

extern std::int32_t helpWidgetIconCount;

extern std::array<UIButtonIconWidget, 32> helpWidgetIcons;

extern std::array<std::int32_t, 32> helpWidgetIconActions;

extern d3t::UIRect helpWidgetPauseRect, helpWidgetLocationRect;

extern std::array<char, 64> helpWidgetPauseText, helpWidgetLocationText;

extern std::int32_t helpWidgetMode, helpWidgetArcadeContext, helpWidgetBlinkTransition;

extern std::uint8_t helpWidgetPeekHidden;

extern std::int32_t helpWidgetKeyboardType;

extern std::array<d3t::UIWindow *, 2> helpWidgetPanels;

extern nk_panel helpWidgetPanel;

void StartHelpBlinkTransition();

std::int64_t HelpWidgetRender();

void RenderHelpFrame();

std::uint32_t LoadHelpDefinition();

std::uint32_t LoadPlatformIcons();

std::int64_t HelpWidgetInput();

std::int64_t HelpWidgetBind();

void HelpWidgetShow();

std::int64_t HelpWidgetDestroy();

std::int64_t HelpWidgetHints();

struct PawnFlyerRuntime {
    UIGrid grid{};
    AuthoredMenuLayout layout;
    UIGridStyle style{UI_GRID_FRONTEND_STYLE};
    std::int32_t nameLabel{-1}, divider{-1}, placeholder{-1};
    std::array<std::int32_t, 5> collected{-1, -1, -1, -1, -1};
    std::int32_t imageControl{};
    float imageAngle{};
    char price[64]{}, quantity[32]{};
};
extern PawnFlyerRuntime pawnSetFlyer, pawnItemFlyer;

extern std::int32_t pawnSetFlyerIndex, pawnItemFlyerIndex, pawnSetFlyerInspection;

extern char pawnSetFlyerShopLabel[256], pawnItemFlyerShopLabel[256];

std::uint32_t LoadPawnSetFlyer();

std::uint32_t LoadPawnItemFlyer();

std::int64_t BindPawnSetFlyer();

std::int64_t BindPawnItemFlyer();

std::int64_t ResetPawnSetFlyer();

std::int64_t ResetPawnItemFlyer();

std::int64_t TickPawnSetFlyer();

std::int64_t TickPawnItemFlyer();

std::int64_t DestroyPawnSetFlyer();

std::int64_t DestroyPawnItemFlyer();

std::int64_t PawnSetFlyerInput();

std::int64_t PawnItemFlyerInput();

std::int64_t RenderPawnSetFlyer();

std::int64_t RenderPawnItemFlyer();

std::int64_t PawnSetFlyerHints();

std::int64_t PawnItemFlyerHints();

std::uint8_t ConfigureHelpDirectionBlink(std::uint32_t visible);

void PrepareSpeakTextSlots();

extern HLib::Task *snapshotDebugTask;

extern SnapshotDebugWork *snapshotDebugWork;

extern SnapshotViewerStorage snapshotDebugStorage;

extern std::array<std::uint32_t, 48> snapshotDebugThumbnails;

extern std::array<SnapshotMetadataRecord *, 126> &snapshotDebugOrder;

extern SnapshotMetadataOwner *&snapshotDebugMetadata;

extern std::int32_t snapshotDebugInitialized;

extern std::int16_t snapshotDebugPulse, snapshotDebugPulseStep;

extern std::int32_t warningMessageIndex, warningMessageNumber;

extern SnapshotDebugTextResources snapshotDebugTextResources;

extern std::int32_t snapshotDebugSpriteCount;

extern std::array<std::int32_t, 7> snapshotDebugSprites;

} // namespace ui_detail

void ReinitTextureData();

namespace ui_detail {

void ResolveUniqueItemAction(std::int32_t mode);

bool ConfirmGameMenuClose();

extern std::int32_t legacySelectorSubmenuPhase, legacySelectorSubmenuSelection;

extern Vector2 itemSelectorWindowPosition;

extern std::array<LegacyIconSprite, 8> selectorNumberSprites;

extern LegacyIconSprite selectorCurrencySprite;

std::int32_t GetSelectorNumberDigits(std::int32_t value);

void DrawSelectorCash(const float *position, std::int32_t value, std::uint8_t currency, std::uint32_t colour, std::uint8_t fullPrefix, ScreenMode mode);

void InitializeItemSelectorWindow();

void RenderLegacyItemSelector(void *parameter);

std::int64_t ReleaseCtrlSpeakTextHandle(std::int32_t handle);

std::uint32_t ConsumeActionSelectorInput();

inline constexpr std::int32_t sleepActionNoSelection = 4;
void DrawCtrlSpeakSelector(void *);

} // namespace ui_detail

void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour, ScreenMode mode);

void DrawMoneyAmount(float x, float y, float depth, float scale, std::int32_t amount, std::uint8_t cash, std::uint32_t colour, ScreenMode mode);

namespace ui_detail {

std::int64_t BindMainMenuDefinition();

} // namespace ui_detail

bool SelectMainMenuPage(std::uint32_t page);

std::int64_t CopyHudHintIds(std::span<const std::int16_t> ids);

void PublishHudHints(const std::uint32_t *input, bool overrideHints);

std::int32_t GetDebugForceUIEnabled(UIElement) noexcept;

float GetUIViewportWidth() noexcept;

const UIMessageBox &GetCurrentUIMessageBox() noexcept;

void CopyUIMessageBox(const UIMessageBox *message);

void ShowTemporaryUIMessageBox(const UIMessageBox *message, float seconds);

void ParseGuiPtx(std::byte *buffer, std::uint16_t width, std::uint16_t height, std::uint32_t idLow, std::uint32_t idHigh, std::uint16_t format);

void InitializeLegacyIconSprite(LegacyIconSprite &sprite) noexcept;

bool AdvanceIconPosition(float &position, float &velocity, float target) noexcept;

bool RetreatIconPosition(float &position, float &velocity, float target) noexcept;

bool IsEasyOptionActive() noexcept;

void SetNewEasyOptionState(std::int32_t mode) noexcept;

bool AbortEasyOptionState(std::int32_t requested) noexcept;

void SetEndingStaffRollFlag(std::int32_t value) noexcept;

bool GetIsLoadScreenActive() noexcept;

void RenderingRelatedInventory(struct nk_image &image, std::uint16_t item, std::int32_t mask);

std::int64_t ExecuteElementCallback(UIElement elementIndex);

void ShowElementByIndex(UIElement elementIndex);

void DisableElement(UIElement elementIndex);

void EnableRenderableByIndex(UIElement elementIndex);

std::int64_t DestroyElementByIndex(UIElement elementIndex);

bool IsHelpOverlayVisible(UIElement element) noexcept;

std::int32_t IsCameraIconHidden() noexcept;

void SetDebugHideCameraIcon(std::int32_t hidden) noexcept;

void SetupCameraIcon() noexcept;

std::int32_t GetCurrentUIInputType() noexcept;

std::int64_t ShowSharedItemsWidget(std::uint32_t itemType, std::uint32_t owner);

void EnableElement(UIElement element);

bool IsGameUIActive();

std::array<std::uint8_t, 4> GetCommonIconRect(std::uint16_t icon);

void CheckHelpIcon(std::uint32_t visible, std::uint32_t selected, CheckHelpIconOp operation);

std::uint32_t GetUiFlag(std::uint8_t mode);

std::int32_t GetQteControlState() noexcept;

void SetHelpIconMenuFlags(std::uint16_t flags);

void AddHelpIconMenuFlags(std::uint16_t flags);

std::uint32_t GetIconSelectFlags() noexcept;

void set_icon_select_flags_to(std::uint32_t flags);

void UpdateGameMessageMoney(std::int32_t value);

std::uint32_t RasterizeItemLabelNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) noexcept;

std::uint16_t PublishLegacyItemLabel(const char *text, std::uint16_t width) noexcept;

std::int32_t SetupControlSpeakAndUI(std::int16_t *choices, std::int32_t timeout, std::int16_t defaultChoice, std::uint16_t mode);

std::int64_t CloseSpeakIcon(std::int32_t force);

void speakSetFlagTo(std::uint16_t timeout);

std::uint32_t GetItemSelectionMode(std::uint8_t slot);

std::uint32_t RefreshItemSelectorInventory(std::uint32_t bank) noexcept;

std::uint32_t GetCurrentItemSelectState();

bool IsAnyItemSelectActive();

std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled);

std::uint16_t GetPadControlData(std::uint8_t mode);

void padcRefreshCurrentInputs(bool bind);

void SetMoneyHudCashAdjustment(std::int32_t amount) noexcept;

} // namespace shenmue::s2
