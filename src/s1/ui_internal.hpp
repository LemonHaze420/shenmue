#pragma once

#include "s1/d3t/namespace.hpp"
#include "s1/nuklear_integration.hpp"
#include "d3t/ui_nuklear.hpp"
#include "platform/performance_counter.hpp"
#include "logging.hpp"

#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/date_time.hpp"
#include "s1/boot.hpp"
#include "s1/initialization.hpp"
#include "s1/font.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/window.hpp"
#include "s1/game_utils.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/input.hpp"
#include "s1/launch_options.hpp"
#include "s1/mip_streaming.hpp"
#include "s1/modules/arcade.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/render_passes.hpp"
#include "s1/scene.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/task/film.hpp"
#include "s1/subtitles.hpp"
#include "s1/timing.hpp"
#include "s1/save_format.hpp"
#include "s1/audio_stream.hpp"
#include "s1/archive_handles.hpp"
#include "s1/task/char.hpp"
#include "s1/face.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/came.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/modu.hpp"
#include "s1/task/atck.hpp"
#include "s1/task/metk.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/save.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/paus.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/hplg.hpp"
#include "s1/task/staf.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/telm.hpp"
#include "s1/task/tmlt.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/time.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/wind.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/scrl.hpp"
#include "s1/item_catalog.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/render_array.hpp"
#include "s1/task/obje.hpp"
#include "s1/screen_space.hpp"
#include "s1/ui_definition.hpp"
#include "s1/ui_runtime.hpp"
#include "platform/performance_counter.hpp"
#include "common/utility.hpp"

#include <cJSON.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <atomic>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include "s1/ui_action_selector.hpp"
#include "s1/ui_assets.hpp"
#include "s1/ui_hud.hpp"
#include "s1/ui_qte.hpp"
#include "s1/ui_training.hpp"

namespace shenmue::s1 {
namespace ui_runtime_detail {}
using namespace ui_runtime_detail;
namespace d3t {
namespace ui_runtime_detail {}
using namespace ui_runtime_detail;
} // namespace d3t
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



struct HelpLegacyPacketSetup {
    float scaleX{};
    float scaleY{};
    float depth{};
    std::uint16_t zeroWord{};
    std::uint32_t zeroDword{};
    std::uint32_t packedFlags{};
    std::uint16_t extent{};
    std::uint8_t mode{};
};

struct HelpPresentationOwner {
    HLib::Task *task{};
    HelpLegacyPacketSetup iconPacket;
    HelpLegacyPacketSetup pagePacket;
    std::uint8_t retryIconFlag{};
    std::uint8_t animationFrame{};
    std::uint8_t transitionCommand{};
    std::uint8_t iconFlags{};
    std::int16_t alpha{};
    std::int16_t alphaStep{0x100};
    std::int32_t onScreenCount{};
    std::int8_t restoreFrames{};
    bool dismissedWhilePaused{};
    bool closing{};
};




struct MessageBoxState {
    std::uint8_t requested;
    std::uint8_t rendered;
    std::uint8_t confirmed;
    std::uint8_t blinkOff;
    std::int32_t hasTimeLimit;
};

struct SeventyManBattlePresentationOwner {
    std::array<MFBTSurface, 14> surfaces{};
    std::array<std::int32_t, 24> digitSurfaces{};

    std::array<std::int32_t, 3> iconSurfaces{-1, -1, -1};
};

struct SubtitleLoadRequest {
    std::uint32_t state;
    std::int32_t language;
};

struct GlyphLoadRequest {
    std::uint32_t state;
    std::int32_t language;
    const void *glyphData;
};

struct SubtitleFontGlyph {
    std::uint32_t codepoint;
    float advance;
    float x0;
    float y0;
    float x1;
    float y1;
    float width;
    float height;
    float u0;
    float v0;
    float u1;
    float v1;
};

struct SubtitleFontLayer {
    std::vector<SubtitleFontGlyph> glyphs;
    std::atomic<std::uint32_t> texture{};
    float lineHeight{};
};

struct SubtitleFontAtlasRequest {
    SubtitleFontLayer *layer{};
    std::uint32_t width{};
    std::uint32_t height{};
    std::array<char, 96> path{};
};

struct SubtitleTableHeader {
    std::int32_t version;
    std::int32_t count;
    const std::byte *records;
};

struct SpecialSubtitleEntry {
    std::byte identifier[8];
    std::uint32_t value;
};

struct SubtitlePresentationColours {
    std::uint32_t foreground;
    std::uint32_t outline;
    std::uint32_t background;
};

struct SplashImageDefinition {
    const char *texturePath;
    const char *audioPath;
    float duration;
    float maximumDuration;
    float fadeOutDuration;
    float minimumDuration;
};

struct SplashScreenRuntime {
    std::uint32_t textureHandle;
    std::uint32_t resolvedTextureHandle;
    std::byte imageRegion[0x20];
    void *waveData;
    std::int32_t waveIndex;
    const SplashImageDefinition *definition;
};

enum class FrontendAutomationStage : std::uint8_t {
    PressStart,
    MainMenu,
    Wheel,
    Options,
    OptionsTab,
    OptionsRow,
    OptionsEdit,
    OptionsStep,
    OptionsEndEdit,
    OptionsKeep,
    OptionsClose,
    Complete,
    Failed,
};

struct UIPanelOffset {
    const UIPanelDefinition *panel;
    float x;
    float y;
};
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {



struct CreditsImage {
    std::uint32_t request;
    std::uint32_t texture;
    struct nk_image image;
};
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



struct ItemRemarkEntry {
    std::uint8_t itemId;
    std::int8_t move;
    std::uint8_t clearRow;
    std::uint8_t setRow;
};



struct FrontendOptionsEdit {
    std::int32_t tab;
    UIGrid *grid;
    UIOption *table;
    std::int32_t count;
    std::int32_t entry;
    const char *name;
};

struct AreaLocationName {
    char area[5];
    const char *stringId;
};

inline constexpr std::int32_t kUiSubMenuItems = 9;

inline constexpr std::int32_t kUiSubMenuCollection = 10;

inline constexpr std::int32_t kUiSubMenuGacha = 11;

inline constexpr std::int32_t kUiSubMenuScrolls = 15;

inline constexpr std::int32_t kUiSubMenuMoves = 16;

inline constexpr std::int32_t kUiSubMenuOptions = 17;

inline constexpr std::int32_t kUiSubMenuOptionsAudio = 18;

inline constexpr std::int32_t kUiSubMenuOptionsControls = 19;

inline constexpr std::int32_t kUiSubMenuOptionsGraphics = 20;

inline constexpr std::int32_t kUiGameMessage = 38;

inline constexpr std::int32_t kUiYesNoPrompt = 56;

inline constexpr std::int32_t kUiGameMenuInformationBar = 57;

inline constexpr std::size_t kSubtitleUiStorageIndex = 58;

inline constexpr std::int32_t kSubtitleUiState = 58;

inline constexpr std::int32_t kUiGameHudBattery = 62;

inline constexpr std::int32_t kUiPlatformIcons = 63;

inline constexpr std::int32_t kUiCredits = 77;

inline constexpr std::int32_t kUiWristwatchOverlay = 70;

inline constexpr std::int32_t kUiVfGameOver = 72;

inline constexpr std::int32_t kUiGameHud = 65;

inline constexpr std::int32_t kUiGameHudWatch = 34;

inline constexpr std::size_t kScenarioDialogueFlagsOffset = 0xd8;

inline constexpr SubtitlePresentationColours kDefaultSubtitleColours{0xffffffffU, 0xff000000U, 0x80000000U};

inline constexpr SubtitlePresentationColours kFocusedItemSubtitleColours{0xff82ffffU, 0xff000000U, 0x80000000U};

inline constexpr std::int32_t kMainMenuWheelElement = 27;

inline constexpr std::uint32_t kUiAcceptSound = 0x900abU;

inline constexpr std::uint32_t kPlaySuccessSound = 0x1100abU;


inline constexpr std::uint32_t kItemsGridMissingIconColour = 0xa5463c3cU;

inline constexpr std::uint32_t kCreditsTextColour = 0xffffffffU;

inline constexpr std::size_t kLoadScreenElement = 0x4e;

inline constexpr std::size_t kLoadScreenBlackBackground = 1;

inline constexpr std::size_t kLoadScreenLocationSubName = 7;

inline constexpr std::size_t kLoadScreenLoadingText = 8;





inline constexpr std::size_t kOptionsSettingsOffset = offsetof(D3TApplicationSettings, keyMapping);

inline constexpr std::size_t kOptionsSettingsSize = offsetof(D3TApplicationSettings, reserved_17c) - kOptionsSettingsOffset;

inline constexpr std::uint32_t kUiNavigateRejectSound = 0x300abU;


inline constexpr std::int32_t kSaveGridSaveBlocked = 0;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::int32_t g_bIsNewGame;

extern std::int32_t g_busy_reset;

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);

extern std::int32_t g_actNumber;

extern std::int32_t g_debugForceShowAllItems;

extern std::array<std::int16_t, 5> g_searchPromptIconSlots;

extern std::int32_t g_isPlayerGameStateActive;

extern std::uint8_t g_previousHelpTokenId;

extern std::uint8_t g_defaultHelpTokenId;

extern std::uint8_t g_currentHelpTokenId;

extern std::uint8_t g_helpButtonId;

extern std::uint16_t g_currentHelpIconState;

extern std::array<UIBaseRuntime, 80> g_uiElements;

extern std::int32_t g_currentSplashScreenIndex;

extern IconPresentationOwner g_iconPresentation;

extern std::uint32_t g_sfxInputFlags;

extern std::int8_t g_cassetteCollectionUiState;

extern std::int32_t g_optionsPendingAction;

void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);

extern std::int32_t g_searchCandidateId;

extern std::int32_t g_currentSearchState;

extern std::array<std::uint8_t, 128> g_shenmueFlags;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
extern std::array<std::vector<std::byte>, 7> g_subtitleData;

extern std::array<std::vector<std::byte>, 7> g_subtitleGlyphData;

extern std::array<UIDefinition, 80> g_parsedUIDefinitions;

extern std::array<HelpNewRun, 64> g_helpNewRuns;

extern std::int32_t g_helpNewRunCount;

extern std::int32_t g_helpNewLineCount;

extern std::array<UIButtonIconWidget, 32> g_helpNewIcons;

extern std::array<std::int32_t, 32> g_helpNewIconActions;

extern std::int32_t g_helpNewIconCount;

extern std::array<HelpNewInfoBlock, 4> g_helpNewInfoBlocks;

extern std::int32_t g_helpNewInfoBlockCount;

extern std::array<d3t::UIWindow *, 2> g_helpNewWindows;

extern UIStyleItem *g_helpNewBackground;

extern std::int32_t g_helpNewTransitionAnimSlot;

extern std::int32_t g_helpNewEmulatorContext;

extern std::int32_t g_helpNewEmulatorPrompt;

extern UILayoutRect g_helpNewTitleRect;

extern UILayoutRect g_helpNewBodyRect;

extern std::array<char, 64> g_helpNewTitleText;

extern std::array<char, 128> g_helpNewBodyText;

extern std::int32_t g_helpNewNextIcon;

extern std::uint8_t g_helpNewRelayout;

extern std::uint8_t g_helpFlag;

extern std::int32_t g_helpNewKeyboardActive;

extern std::array<std::int16_t, 4> g_helpNewPromptIconSet;

extern nk_panel g_helpNewPanel;

extern std::array<char, 1024> g_helpFormattedText;

extern std::uint32_t g_helpFormattedLength;

extern std::uint32_t g_helpColumnSkip;

extern std::uint32_t g_helpLineCounter;

extern std::array<HelpIconRecord, 10> g_helpIconRecords;

extern std::int16_t g_helpIconRecordCount;

extern std::array<std::int16_t, 20> g_helpLineWidths;

extern std::uint8_t g_helpColumnOffset;

extern std::array<std::array<char, 128>, 32> g_helpLegacyLines;

extern std::int32_t g_helpLegacyLineCount;

extern std::array<std::int32_t, 12> g_helpLegacyIconIds;

extern std::array<std::uint64_t, 12> g_helpLegacyIconLines;

extern std::array<float, 12> g_helpLegacyIconColumns;

extern std::int32_t g_helpLegacyIconCount;

extern std::int32_t g_helpTextRunLength;

extern std::int32_t g_helpTextWidth;

extern std::int32_t g_helpTextMaxWidth;

extern std::array<char, 2048> g_currentHelpText;

extern std::array<std::int16_t, 20> g_helpIconSurfaceIds;

extern std::uint8_t g_moveListScreenState;

extern std::int32_t g_debugHelpPaging;

extern std::array<float, 2> g_batteryLifeFractions;

extern std::uint16_t g_uiState;

extern UISicnPausePolicy g_currentSicn;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::uint8_t g_helpOverlayPageTokenId;

extern UIHelpWidgetState g_helpOverlayState;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
extern HelpPresentationOwner g_helpPresentation;

extern MessageBoxState g_messageBoxState;

extern std::int32_t g_yesNoPromptTimeoutSlot;

extern std::int32_t g_messageBoxSelection;

extern std::int32_t g_yesNoPromptTransitionSlot;

extern UIRetry70ManBattlePromptCallback g_messageBoxCallback;

extern const char *g_messageBoxToken;

extern std::uint8_t g_messageBoxUseString;

extern float g_messageBoxTimeout;

extern UIRetry70ManBattlePromptCallback g_retry70ManBattleFlowCallback;

extern SeventyManBattlePresentationOwner g_seventyManBattlePresentation;

extern std::array<d3t::UIWindow *, 5> g_itemsGridWindows;

extern d3t::UILabelWidget *g_itemsGridPlaceholderLabel;

extern std::array<d3t::UILayoutItem *, 2> g_itemsGridLayoutItems;

extern d3t::UIImageWidget *g_itemsGridPlaceholderImage;

extern UIGridTemplate g_itemsGridTemplate;

extern UIGrid g_itemsGrid;

extern std::array<UIGridEntry, 256> g_itemsGridElements;

extern std::int32_t g_itemsGridElementCount;

extern std::array<d3t::UIWindow *, 6> g_cassettesWindows;

extern std::array<d3t::UILayoutItem *, 2> g_cassettesLayoutItems;

extern UIButtonIconWidget *g_cassettesSpeakers;

extern UIGridTemplate g_cassettesGridTemplate;

extern UIGrid g_cassettesGrid;

extern std::array<UIGridEntry, 32> g_cassettesGridElements;

extern std::int32_t g_cassettesGridElementCount;

extern std::array<d3t::UIWindow *, 5> g_gachaWindows;

extern d3t::UILabelWidget *g_gachaPlaceholderLabel;

extern std::array<d3t::UILayoutItem *, 2> g_gachaLayoutItems;

extern d3t::UIImageWidget *g_gachaPlaceholderImage;

extern UIGridTemplate g_gachaGridTemplate;

extern UIGrid g_gachaGrid;

extern std::array<UIGridEntry, 512> g_gachaGridElements;

extern std::int32_t g_gachaGridElementCount;

extern std::array<d3t::UIWindow *, 5> g_scrollsWindows;

extern std::array<d3t::UILayoutItem *, 2> g_scrollsLayoutItems;

extern d3t::UIImageWidget *g_scrollsPlaceholderImage;

extern UIGridTemplate g_scrollsGridTemplate;

extern UIGrid g_scrollsGrid;

extern std::array<UIGridEntry, 9> g_scrollsGridElements;

extern std::int32_t g_scrollsGridElementCount;

extern UIGridEntry g_scrollsEmptyElement;

extern UIGridTemplate g_movesGridTemplate;

extern std::array<d3t::UIWindow *, 25> g_movesWindows;

extern std::array<d3t::UILabelWidget *, 2> g_movesLabels;

extern std::array<UIColourImageWidget *, 5> g_movesColourImages;

extern std::array<d3t::UILayoutItem *, 28> g_movesLayoutItems;

extern std::array<UIButtonIconWidget *, 7> g_movesButtonIcons;

extern std::array<UIButtonWidget *, 8> g_movesButtons;

extern UIGrid g_movesGrid;

extern std::array<std::array<char, 256>, 2> g_movesLabelTexts;

extern std::int32_t g_uiGridInputEnabled;

extern UIFocusList *g_uiFocusedList;

extern UIGrid *g_uiCurrentGrid;

extern nk_panel g_uiGridPanel;

extern nk_panel g_uiGridScrollBarPanel;

extern std::uint8_t g_icmcCassetteIsPlayerActive;

extern std::uint8_t g_icmcCassetteSelectedIndex;

extern std::uint8_t g_cassetteBaseIndex;

extern std::int8_t g_cassetteIndex;

extern std::uint8_t g_cassettePlaying;

extern std::uint16_t g_cassetteOwnedCount;

extern std::uint16_t g_cassetteSavedHelpToken;

extern std::int16_t g_cassetteSavedHelpButton;

extern CVWKRuntimeSlot *g_currentCassetteDialogue;

extern std::uint8_t g_cassetteVisibleCount;

extern PadCaptureEntry *g_icmcInput;

extern PadCaptureEntry *g_cassetteUiCachedInput;

extern std::array<std::int16_t, 9> g_cassetteSurfaces;

extern std::int32_t g_cassettePlayRequested;

extern CassetteSoundState g_cassetteSound;

extern std::uint8_t g_cassettePlayFlag;

extern std::int32_t g_cassetteSpeakerAnimSlot;

extern std::uint8_t g_cassetteMusicNoteVisible;

extern std::array<float, 4> g_cassetteMusicNoteRect;

extern std::int32_t g_cassetteMusicNoteAnimSlot;

extern std::int32_t g_cassetteBatteryHudOwned;

extern nk_panel g_cassetteMusicNotePanel;

extern std::array<std::int32_t, 5> g_cassettePromptIconSet;

extern std::int32_t g_userResponse;

extern std::uint64_t g_uiRejectSoundFrame;

extern SharedItemsPreviewRequest g_sharedItemRequest;

extern std::int32_t g_sharedItemsRequested;

extern std::int32_t g_batteryHudIndex;

extern std::int32_t g_batteryHudAnimSlot;

extern UIButtonIconWidget *g_batteryHudSprite;

extern std::array<std::int32_t, 5> g_batteryPromptIconSet;

extern std::int32_t g_gameMenuSubmenu;

extern std::int32_t g_gameMenuPendingSubmenu;

extern std::int32_t g_gameMenuCloseState;

extern std::array<char, 256> g_gameMenuMoneyText;

extern std::array<char, 256> g_gameMenuTokenText;

extern std::array<char, 256> g_gameMenuDateText;

extern const std::array<const char *, 12> kGameMenuWindowIds;

extern const std::array<const char *, 8> kGameMenuLabelIds;

extern const std::array<const char *, 5> kGameMenuIconIds;

extern std::array<d3t::UIWindow *, 12> g_gameMenuWindows;

extern std::array<d3t::UILabelWidget *, 8> g_gameMenuLabels;

extern std::array<UIColourImageWidget *, 5> g_gameMenuIcons;

extern const std::array<std::int16_t, 40> kHelpTextRunWidths;

extern const std::array<const char *, 68> kHelpStringIds;

extern const std::array<std::int32_t, 3> kCollectionChildren;

extern std::array<d3t::UIWindow *, 3> g_collectionWindows;

extern std::array<d3t::UILabelWidget *, 4> g_collectionLabels;

extern std::array<UIColourImageWidget *, 7> g_collectionColourImages;

extern std::array<UIButtonWidget *, 3> g_collectionButtons;

extern UIFocusList g_collectionTabList;

extern std::array<std::int32_t, 5> g_collectionPromptIconSet;

extern const std::array<std::int32_t, 4> kOptionsChildren;

extern std::array<d3t::UIWindow *, 41> g_optionsWindows;

extern std::array<d3t::UILayoutItem *, 47> g_optionsLayoutItems;

extern std::array<d3t::UILabelWidget *, 13> g_optionsLabels;

extern std::array<UIColourImageWidget *, 48> g_optionsColourImages;

extern std::array<UIButtonWidget *, 19> g_optionsButtons;

extern UIButtonIconWidget *g_optionsKeyBindingKey;

extern UIGridTemplate g_optionsRowTemplate;

extern UIFocusList g_optionsTabList;

extern std::uint32_t g_optionsTabButtonColour;

extern std::uint32_t g_optionsTabButtonSelectedColour;

extern std::uint32_t g_optionsTabBackgroundColour;

extern std::uint32_t g_optionsTabBackgroundSelectedColour;

extern std::int32_t g_optionsTabBackgrounds;

extern std::uint8_t g_optionsClosePending;

extern std::int32_t g_optionsShownLanguage;

extern std::array<char, 256> g_optionsSliderText;

extern std::array<std::int32_t, 5> g_optionsPromptIconSet;

extern UIGridDrawCell g_optionsControlsRowDraw;

extern UIGridDrawCell g_optionsAudioRowDraw;

extern UIGridDrawCell g_optionsGraphicsRowDraw;

extern d3t::UIWindow *g_optionsControlsPanel;

extern UIGrid g_optionsControlsGrid;

extern std::array<std::uint64_t, 42> g_optionsControlsRecordedRows;

extern d3t::UIWindow *g_optionsAudioPanel;

extern UIGrid g_optionsAudioGrid;

extern std::array<std::uint64_t, 7> g_optionsAudioRecordedRows;

extern d3t::UIWindow *g_optionsGraphicsPanel;

extern UIGrid g_optionsGraphicsGrid;

extern std::array<std::uint64_t, 11> g_optionsGraphicsRecordedRows;

extern std::array<SubtitleLoadRequest, 8192> g_subtitleLoadRequests;

extern std::array<GlyphLoadRequest, 8192> g_glyphLoadRequests;

extern std::atomic<std::uint16_t> g_subtitleLoadRequestIndex;

extern std::atomic<std::uint16_t> g_glyphLoadRequestIndex;

extern std::array<std::int32_t, 7> g_glyphLoadStates;

extern std::array<std::array<SubtitleFontLayer, 2>, 7> g_subtitleFonts;

extern std::array<std::array<std::array<float, 6>, 4>, 7> g_uiFontPixelSizes;

extern std::array<std::array<SubtitleFontAtlasRequest, 2>, 7> g_subtitleFontRequests;

extern std::array<SubtitleFontLayer, 7> g_memoFonts;

extern std::array<SubtitleFontAtlasRequest, 7> g_memoFontRequests;

extern SubtitleTableHeader *g_stringTable;

extern const std::byte *g_stringTableData;

extern const std::byte *g_stringTableEnd;

extern std::vector<SpecialSubtitleEntry> g_specialSubtitleEntries;

extern std::array<std::array<char, 512>, 3> g_subtitleLines;

extern CVWKRuntimeSlot *g_currentSubtitleSlot;

extern std::int32_t g_cvwkReadSubtitleOnPlay;

extern SubtitlePresentationColours g_subtitlePresentationColours;

extern std::array<SubtitlePresentationColours, 4> g_subtitleClassColours;

extern const std::array<SplashImageDefinition, 6> kSplashImages;

extern std::array<SplashScreenRuntime, 6> g_splashScreens;

extern std::int32_t g_splashState;

extern std::int32_t g_forceSkipSplashScreens;

extern std::uint64_t g_splashStateStart;

extern bool g_splashPanel;

extern bool g_skipSplash;

extern float g_splashOpacity;

extern std::int32_t g_pressStartIdleFrames;

extern bool g_pressStartIntroPending;

extern bool g_pressStartIntroActive;

extern const std::array<const char *, 5> kPressStartWindows;

extern std::array<std::int32_t, 5> g_pressStartFadeSlots;

extern float g_pressStartIntroTime;

extern std::int32_t g_pressStartBlinkSlot;

extern const std::array<const char *, 3> kPressStartLogoImages;

extern std::array<std::int32_t, 3> g_pressStartLogoImages;

extern std::array<d3t::UIWindow *, 5> g_pressStartWindows;

extern UIColourImageWidget *g_pressStartEmblemImage;

extern const std::array<const char *, 2> kPressStartIconIds;

extern std::array<UIButtonIconWidget *, 2> g_pressStartIcons;

extern d3t::UILabelWidget *g_pressStartLabel;

extern d3t::UIImageWidget *g_pressStartLogoImage;

extern std::int32_t g_mainMenuWheelIndex;

extern UIButtonWheel *g_activeMainMenuWheelLayout;

extern UIButtonWheel g_mainMenuWheelSnapshot;

extern std::int32_t g_mainMenuWheelRefresh;

extern std::int32_t g_mainMenuWheelPreviousSelection;

extern const std::array<const char *, 28> kMainMenuWheelImages;

extern std::array<std::int32_t, 28> g_mainMenuWheelImages;

extern std::int32_t g_mainMenuStatIndex;

extern const std::array<std::int32_t, 4> kMainMenuIds;

extern std::uint8_t g_mainMenuNewGamePicked;

extern std::uint8_t g_mainMenuWheelArmed;

extern std::int32_t g_mainMenuWheelAnimSlot;

extern std::uint64_t g_uiAcceptSoundFrame;

extern std::vector<char> g_frontendAutomationOriginalSettingsFile;

extern bool g_frontendAutomationSettingsFileCaptured;

extern bool g_frontendAutomationSettingsFileExisted;

extern FrontendAutomationStage g_frontendAutomationStage;

extern std::uint16_t g_frontendAutomationFrames;

extern std::int32_t g_frontendOptionsStep;

extern std::int32_t g_frontendOptionsRow;

extern float g_frontendOptionsOriginal;

extern float g_frontendOptionsStart;

extern const std::array<const char *, 4> kMainMenuPageWindowIds;

extern const std::array<const char *, 28> kMainMenuTitleImages;

extern const std::array<const char *, 3> kMainMenuFrontImageIds;

extern d3t::UIWindow *g_mainMenuContentPanel;

extern std::array<d3t::UIWindow *, 4> g_mainMenuPageWindows;

extern d3t::UIImageWidget *g_mainMenuTitleImage;

extern d3t::UIWindow *g_switchAccountButtonIcon;

extern d3t::UIWindow *g_switchAccountButtonLabel;

extern std::array<std::int32_t, 28> g_mainMenuTitleImageIndices;

extern std::array<UIColourImageWidget *, 3> g_mainMenuFrontImages;

extern std::array<std::uint8_t, 3> g_mainMenuFrontImageAlphas;

extern d3t::UILabelWidget *g_mainMenuUserIdLabel;

extern std::array<char, 1> g_mainMenuUserIdText;

extern std::array<std::int32_t, 5> g_hudPromptRequestedSlots;

extern const std::array<const char *, 5> kSubtitleWindowIds;

extern const std::array<const char *, 2> kSubtitleImageIds;

extern std::array<d3t::UIWindow *, 5> g_subtitleWindows;

extern std::int32_t g_subtitleTextAnimSlot;

extern std::int32_t g_subtitleFullPanelBinding;

extern std::int32_t g_subtitleHudPanelBinding;

extern std::array<d3t::UIImageWidget *, 2> g_subtitleBackgroundImages;

extern d3t::UILabelWidget *g_subtitlePhoneLabel;

extern std::array<std::int32_t, 3> g_subtitleForegroundLabels;

extern std::array<std::int32_t, 3> g_subtitleOutlineLabels;

extern std::array<std::int32_t, 3> g_subtitleBackgroundLabels;

extern std::array<char, 512> g_subtitleText;

extern float g_subtitleVariableScale;

extern bool g_subtitlesShown;

extern const std::array<std::int32_t, 7> kMovesCommandActions;

extern const std::array<const char *, 3> kYesNoPromptWindowIds;

extern const std::array<const char *, 2> kYesNoPromptTextImageIds;

extern const std::array<const char *, 4> kYesNoPromptSpriteIds;

extern const std::array<const char *, 16> kYesNoPromptImageIds;

extern std::array<d3t::UIWindow *, 3> g_yesNoPromptWindows;

extern std::array<d3t::UIImageWidget *, 2> g_yesNoPromptTextImages;

extern std::array<UIButtonIconWidget *, 4> g_yesNoPromptSprites;

extern std::array<std::int32_t, 16> g_yesNoPromptImages;

extern std::array<char, 512> g_messageBoxTimedText;

extern std::array<std::int32_t, 5> g_hudPromptRequestedSlots;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::array<CreditsTable, 3> g_creditsTables;

extern float g_creditsScrollSpeed;

extern float g_creditsScrollProgress;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
extern CreditsImage *g_creditsSections;

extern std::uint32_t g_creditsSectionCount;

extern std::uint32_t g_creditsSectionCapacity;

extern CreditsImage g_creditsSignature;

extern d3t::UIWindow *g_creditsScrollSpace;

extern float g_creditsFadeAlpha;

extern std::uint8_t g_creditsFadeOutRequested;

extern std::uint8_t g_creditsScrollInitialized;

extern nk_panel g_creditsPanel;

extern const std::array<const char *, 3> kCreditsTypes;

extern const std::array<const char *, 9> kLoadScreenWindowIds;

extern std::array<std::array<char, 11>, 3232> g_notebookTextIds;

extern std::int32_t g_memoPadDebugMode;

extern std::int32_t g_debugNotebookOpenRequested;

extern std::int32_t g_debugNotebookOpenPage;

extern std::uint8_t g_debugNotebookRedraw;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::array<std::uint32_t, 4> g_notebookTextureLeft;

extern std::array<std::uint32_t, 4> g_notebookTextureRight;

extern std::array<std::uint8_t, 4> g_notebookLeftPageFlags;

extern std::array<std::uint8_t, 4> g_notebookRightPageFlags;

extern std::array<std::array<char, 1280>, 4> g_memoText;

extern std::array<const char *, 4> g_memoTextPages;

extern std::array<std::byte *, 4> g_notebookMemoTextureData;

extern std::int32_t g_70mbStartPromptIconVisible;

extern std::uint8_t g_lcdmHelpIconSpritesLoaded;

extern std::int16_t g_helpIconWidth;

extern std::int16_t g_helpIconHeight;

extern std::array<std::int32_t, 16> g_helpRetrySurfaces;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
extern const std::array<ItemRemarkEntry, 11> kItemRemarks;

extern const std::array<std::array<const char *, 3>, 8> kItemRemarkLines;

extern const std::array<std::int16_t, 3> kMoveSetMoveCounts;

extern const std::array<std::int32_t, 5> kGameMenuSubmenus;

extern std::array<const char *, 9> g_languageChoiceLabels;

extern std::array<std::int32_t, 9> g_languageChoiceValues;

extern const std::array<const char *, 7> kOptionsLanguageNames;

extern std::array<std::array<char, 18>, 10> g_optionsAudioDeviceTexts;

extern std::array<const char *, 11> g_optionsAudioDeviceChoices;

extern std::array<UIOption, 7> g_optionsAudioTable;

extern std::array<UIOption, 42> g_optionsControlsTable;

extern std::array<UIOption, 11> g_optionsGraphicsTable;

extern std::array<std::byte, kOptionsSettingsSize> g_tmpUserKeyMapping;

extern UIOption *g_optionsDescribedOption;

extern UIOption *g_optionsInputAction;

extern float g_optionsEditValue;

extern std::int32_t g_optionsEditCurrent;

extern std::int32_t g_optionsKeyMapAction;

extern std::int32_t g_optionsLastKeyIndex;

extern std::array<char, 512> g_optionsKeyBindingText;

extern std::uint32_t g_optionsSuccessSoundFrame;

extern const std::array<FrontendOptionsEdit, 3> kFrontendOptionsEdits;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern SleepSaveTrainingMenu g_sleepSaveTrainingMenu;

extern MovesScreenElement g_movesScreen;

extern std::array<std::int16_t, 5> g_searchPromptIconSlots;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
extern std::array<std::int32_t, 5> g_movesPromptIconSet;

extern std::int32_t g_movesRemapAssignSoundFrame;

extern const char kUnknownMoveText[];

extern const std::array<std::uint32_t, 4> kMovesLevelColours;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::array<SaveSlotInfo, 10> g_saveSlotInfo;
}

namespace shenmue::s1::ui_runtime_detail {
extern const std::array<const char *, 12> kLoadMonthIds;

extern const std::array<const char *, 8> kLoadDayIds;

extern const AreaLocationName kAreaLocationNames[82];
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::int32_t g_iIsInGame;

extern std::int32_t g_isInGame;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
extern std::uint64_t g_loadSaveRejectFrame;

extern std::uint64_t g_loadSaveSaveSoundFrame;

extern std::uint64_t g_loadSaveBlockedSoundFrame;

extern UIGridTemplate g_loadSaveGridTemplate;

extern UIGrid g_loadSaveGrid;

extern std::array<std::array<char, 256>, 7> g_loadSaveLabelTexts;

extern std::array<d3t::UIWindow *, 9> g_loadSaveWindows;

extern std::array<d3t::UILayoutItem *, 12> g_loadSaveLayoutItems;

extern std::array<d3t::UILabelWidget *, 7> g_loadSaveLabels;

extern std::array<UIButtonWidget *, 2> g_loadSaveButtons;

extern std::array<UIColourImageWidget *, 9> g_loadSaveColourImages;

extern std::array<d3t::UIViewImage *, 8> g_loadSaveViewImages;

extern std::array<d3t::UIViewImage *, 6> g_loadSaveAreaImages;

extern std::array<d3t::UIViewImage *, 25> g_loadSaveMapImages;

extern d3t::UIImageWidget *g_loadSaveMapImage;

extern d3t::UIImageWidget *g_loadSaveAreaImage;

extern d3t::UIWindow *g_loadSaveGridHolder;

extern bool g_loadSaveConfirmLocked;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern LoadScreenPresentation g_loadScreenPresentation;
}

namespace shenmue::s1::ui_runtime_detail {
extern const std::array<const char *, 4> kLoadingUITokens;

extern const std::array<const char *, 4> kLoadingDots;

extern const char kLoadScreenSubAreaMark[];

extern const std::array<MapLocationToken, 86> kMapLocationTokens;
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
extern std::int32_t g_uiDebugElementDispatch;

extern std::int32_t g_notebookDebugMode;

extern std::array<std::uint32_t, 3> g_uiRegisteredCallbackRequests;
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void SetHelpDetailsByIndex(std::int32_t index);

void InstallOwnedTaskMain(HLib::TaskCallback callback);

void EnsureHelpTask();

void ActivateOwnedElement(std::int32_t index, const char *path);

bool ResetOwnedElement(std::int32_t index);

void HideOwnedElement(std::int32_t index);

Character *MapRyo();

std::uint32_t OpenUIFile(const char *path, UIBaseRuntime &element);

std::uint32_t LoadPresentationDefinition(std::int32_t index, const char *path);

std::uint32_t LoadPressStart();

std::uint32_t LoadMainMenu();

bool RestoreFrontendOptionsSettingsFile();

void SetFrontendAutomationStage(FrontendAutomationStage stage, const char *optionsDiagnostic = nullptr);

bool QueueFrontendAutomationInput(std::uint16_t input);

std::uint32_t LoadMainMenuWheel();

std::int64_t MainMenuWheelSelected();

std::int64_t MainMenuWheelInput();

std::int64_t MainMenuWheelTick();

void MainMenuWheelRender();

std::int64_t MainMenuWheel_GetPromptIconSet();

void FrontendAutomation_MainMenuWheelTick();

std::int64_t MainMenuWheelDestroy();

std::int64_t MainMenuSelected();

std::int64_t MainMenuTick();

std::int64_t MainMenuReset();

std::int64_t MainMenuInput();

std::int64_t FrontendAutomation_MainMenuTick();

void MainMenuRender();

std::int64_t MainMenuDestroy();

std::int64_t PressStartSelected();

std::int64_t PressStartInput();

std::int64_t PressStartTick();

void PressStartRender();

std::int64_t PressStartDestroy();

std::int64_t Splash();

std::int64_t ResetSplash();

std::int64_t InitShaders();

std::int64_t GetInput();

void InitLoadingSplash();

std::int64_t ProcessGlobalGameState();

const SubtitleFontGlyph *FindSubtitleGlyph(const SubtitleFontLayer &font, std::uint32_t codepoint);

std::uint32_t DecodeSubtitleCodepoint(const char *&text);

bool DrawSubtitleLayer(
    const SubtitleFontLayer &font, const char *text, float lineY, float offsetX, float offsetY, std::uint32_t colour, std::uint32_t renderTarget);

std::uint32_t Subtitles_LoadUI();

std::int64_t Subtitles_IsSelected();

std::int64_t Subtitles_Tick();

void Subtitles_Render();

std::int64_t ResetSubtitles();

std::int64_t Subtitles_Reset();

std::string FormatCashOverlayText(std::int32_t value);

bool GameMessageAnimIdle();

std::string FormatGroupedNumber(std::int32_t value);

void GameMessage_SetContent(UIGameMessageContentType type, std::int32_t value, const char *text);

UILayoutRect UIObjectLayoutRect(const UIObjectDefinition &object, const UIPanelDefinition &panel, const UILayoutRect &panelRect, float unit);

void UI_PlaySound(std::int32_t index);

UILayoutRect ResolveUIPanelRect(const UIDefinition &definition, const UIPanelDefinition &panel, std::int32_t depth, const UIPanelOffset *offset);

void UI_RestartCurrentGridSelection();

const UIGridStyle &UIGrid_Style(const UIGrid *grid);

UILayoutRect UIGrid_GetSelectedCellRect(UIGrid *grid);

std::int32_t UIGrid_SelectedIndex(const UIGrid &grid);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t::ui_runtime_detail {
void *cbGetMenuElementPointer(void *elements, std::int32_t index);

void DrawGridItem(UIGrid *grid, std::int32_t element);
} // namespace shenmue::s1::d3t::ui_runtime_detail

namespace shenmue::s1::ui_runtime_detail {
std::int64_t UIGrid_IsPressed(UIGrid &grid);

void UIGrid_ResetRuntime(UIGrid &grid);

std::int32_t UIGrid_PlaceCellWidgets(UIGridTemplate &cellTemplate, std::int32_t index, const UILayoutRect &holder, const UILayoutRect &cell);

void UIGrid_AfterDraw(
    UIGridTemplate &cellTemplate, UIGrid *grid, std::int32_t index, std::int32_t firstId, std::int32_t endId, const UILayoutRect &holder, nk_panel *panel);

void UIGrid_DrawItemCell(
    UIGrid *grid, nk_panel *, const UILayoutRect *, const UILayoutRect *cell, float, std::int32_t element, void *elements, std::int32_t index);

std::int32_t UI_FindViewSpacePush(std::uint32_t slotIndex, const char *id);

std::int64_t ItemsMenu_IsSelected();

void ItemsMenu_Render();

std::int64_t ItemsMenu_Tick();

std::int64_t ItemsMenu_IsPressed();

std::int64_t ItemsMenu_Destroy();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
std::int32_t ResolveButtonIconGlyph(std::int32_t action, std::int32_t controlMethod, std::int32_t inputType, std::int32_t doubleWidth);
}

namespace shenmue::s1::d3t {
bool FormatVirtualKeyString(std::int32_t key, std::array<char, 64> &name);
}

namespace shenmue::s1::ui_runtime_detail {
bool UI_DrawButtonIcon(
    const UIDefinition &definition, const UIObjectDefinition &widget, const UILayoutRect &bounds, std::int32_t action, float alpha, std::uint32_t renderTarget);

std::int64_t YesNoPrompt_IsSelected();

std::int64_t YesNoPrompt_Reset();

void YesNoPrompt_Render();

std::int64_t YesNoPrompt_Destroy();

std::int64_t YesNoPrompt_ProcessMessageBox();

std::int64_t YesNoPrompt_Tick();

std::int64_t YesNoPrompt_GetPromptIconSet();

std::int32_t UI_FindViewIconWidget(std::int32_t kind, std::uint32_t slotIndex, const char *id);

std::int64_t HelpNew_IsSelected();

void HelpNew_Render();

std::int64_t HelpNew_IsPressed();

std::int64_t HelpNew_Reset();

std::int64_t HelpNew_GetPromptIconSet();

std::int64_t HelpNew_Tick();

std::int64_t Credits_InitScrollSpace();

std::int64_t Credits_Reset();

std::int64_t Credits_IsPressed();

void ReadCreditsCSV();

std::int64_t Credits_Destroy();

std::int64_t Credits_Tick();

bool DrawLoadScreenPresentation(std::uint32_t renderTarget);

std::uint32_t NotebookPages_ParseDefinition();

std::int64_t PrepareNotebookBuffers();

void NotebookPages_Initialize();

std::int64_t InitNotebookTextures();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void UI_ActivateElement(UIBaseRuntime &element);

std::uint32_t UI_SetPVRSurfaceTexture(std::int32_t surface, std::uint32_t texture);

void UI_GUITasksShutdown();
} // namespace shenmue::s1

namespace shenmue::s1::d3t::ui_runtime_detail {
bool MessageBox(const char *token, bool useString, std::int32_t clearAfterDone, UIRetry70ManBattlePromptCallback callback, float timeBeforeInactive);
}

namespace shenmue::s1 {
void UI_CloseRetry70ManBattlePrompt();

void UI_HELPClear();

void UI_ICONRecomputeInventoryGrid();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
bool CVWK_IsVoiceSlotActive(const CVWKRuntimeSlot *slot);
}

namespace shenmue::s1 {
std::int16_t AllocatePVRSurface(const char *, std::int32_t width, std::uint16_t foreground, std::uint16_t background, std::int32_t, bool centered);

void UI_CloseConversationUIAndReturnToFreeRoam();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void RestoreIconSystemMenuCameraTransition(IconSystemMenuCameraTransition &transition);
}

namespace shenmue::s1 {
void UI_ICISCallback(HLib::TaskData *);

void UI_ICISMainCallback(HLib::TaskData *);

void UI_ICLIResetSharedItemMenu();
} // namespace shenmue::s1

namespace shenmue::s1::d3t {
void Shared_Items_Tick(std::int32_t type, std::int32_t id);
}

namespace shenmue::s1 {
void WriteInput_Callback(const UIGridEntry &entry);

std::int64_t GameMenu_IsPressed();

std::int64_t GameMenu_IsSelected();

std::int64_t GameMenu_Reset();

std::int64_t GameMenu_Tick();

void GameMenu_Render();

std::int64_t GameMenu_Destroy();

std::int64_t GameMenu_StartDraw();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void Collection_ShowSelectedCell(UIGrid &grid, std::int32_t element, const char *text);
}

namespace shenmue::s1 {
std::int64_t CassettesMenu_IsSelected();

std::int64_t CassettesMenu_Reset();

std::int64_t CassettesMenu_IsPressed();

std::int64_t CassettesMenu_Tick();

void CassettesMenu_Render();

std::int64_t CassettesMenu_Destroy();

std::int64_t GachaMenu_IsSelected();

std::int64_t GachaMenu_Reset();

std::int64_t GachaMenu_IsPressed();

std::int64_t GachaMenu_Tick();

void GachaMenu_Render();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void UIFocusList_ColourLabels(const UIFocusList &list);

std::int32_t UIFocusList_HandleInput(UIFocusList &list);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
std::int64_t CollectionMenu_IsSelected();

std::int64_t CollectionMenu_Reset();

std::int64_t CollectionMenu_IsPressed();

std::int64_t CollectionMenu_Tick();

void CollectionMenu_Render();

std::int64_t CollectionMenu_Destroy();

std::int64_t CollectionMenu_ElementDraw();

std::int64_t CollectionMenu_GetPromptIconSet();

std::int64_t ScrollsMenu_IsSelected();

std::int64_t ScrollsMenu_Reset();

std::int64_t ScrollsMenu_IsPressed();

std::int64_t ScrollsMenu_Tick();

void ScrollsMenu_Render();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
const char *Options_ReadString(const char *id);

const char *Options_AudioDeviceText(std::int32_t choice);

void UIGrid_SelectElement(UIGrid &grid, std::int32_t index);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
std::int64_t BatteryHud_IsSelected();

std::int64_t BatteryHud_IsPressed();

std::int64_t BatteryHud_Tick();

std::int64_t BatteryHud_GetPromptIconSet();

std::int64_t OptionsControls_IsSelected();

std::int64_t OptionsControls_Reset();

std::int64_t OptionsControls_IsPressed();

std::int64_t OptionsControls_Tick();

void OptionsControls_Render();

std::int64_t OptionsAudio_IsSelected();

std::int64_t OptionsAudio_Reset();

std::int64_t OptionsAudio_IsPressed();

std::int64_t OptionsAudio_Tick();

void OptionsAudio_Render();

std::int64_t OptionsGraphics_IsSelected();

std::int64_t OptionsGraphics_Reset();

std::int64_t OptionsGraphics_IsPressed();

std::int64_t OptionsGraphics_Tick();

void OptionsGraphics_Render();

std::int64_t OptionsMenu_IsSelected();

std::int64_t OptionsMenu_GetContentPanel();

std::int64_t OptionsMenu_Reset();

std::int64_t OptionsMenu_Tick();

void OptionsMenu_Render();

std::int64_t OptionsMenu_IsPressed();

std::int64_t OptionsMenu_Destroy();

std::int64_t OptionsMenu_GetPromptIconSet();

void UI_StopPlayingCassettes();

void UI_ICMCCallback(HLib::TaskData *);

void UI_ICMCMainCallback(HLib::TaskData *);

std::uint16_t RefreshBatteryLifeForCassettes();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
std::int32_t DecodeUITextCharacter(const char *text, std::uint32_t *codepoint, std::int32_t size);

std::int32_t FillStringData(const char *text, std::int32_t size);

std::int32_t TextFormat(char *buffer, std::int32_t size, const char *tag, const char *replacement);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t {
void CopyStringToUILabel(const InformationBarMessage *message);

void ShowMessageStr(const InformationBarMessage *message, float seconds);
} // namespace shenmue::s1::d3t

namespace shenmue::s1 {
std::int32_t GetFightAnimInfos(std::int32_t move, char *name, FightMoveInfo *info);
}

namespace shenmue::s1::ui_runtime_detail {
void UpdateWidget();

void UpdateState(std::int32_t animSet);

std::uint32_t MovesScreen_Load();

std::int32_t UI_FindViewButton(std::uint32_t slotIndex, const char *id);

std::int64_t MovesScreen_IsSelected();

std::int64_t MovesScreen_Reset();

void MovesScreen_Render();

std::int64_t MovesScreen_Update();

std::int64_t MovesScreen_IsPressed();

std::int64_t MovesScreen_Destroy();

std::int64_t MovesScreen_GetPromptIconSet();

std::int64_t MovesScreen_GetContentPanel();

d3t::UILabelWidget *UI_LabelRecord(std::int32_t index);

UIColourImageWidget *UI_ColourImageRecord(std::int32_t index);

UIButtonIconWidget *UI_ButtonIconRecord(std::int32_t index);
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void GenerateLoadingScreenText(
    char *destination, std::size_t size, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t dayOfWeek, std::uint8_t mode);

void CreateSaveDataInfoForMenu(std::int32_t loadType);

void UpdateInGameStatus();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
std::uint32_t LoadSave_ParseDefinition();

std::int64_t LoadSave_IsSelected();

std::int64_t LoadSave_GetContentPanel();

void LoadSave_Render();

std::int64_t LoadSave_IsPressed();

std::int64_t LoadSave_Tick();

std::int64_t LoadSave_Destroy();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
void SRCH_ApplySearchPromptSlots();

void UI_SICNEnqueue(std::uint16_t state, UISicnPausePolicy pausePolicy);

void UI_SICNSetState(std::uint16_t state, UISicnPausePolicy pausePolicy);

bool UI_SICNIsTaskActive();

void UI_SICNClearState(std::uint16_t state);

bool CDTT_IsLoadingUIActive();

CVWK *CVWK_Initialize();

CVWKRuntimeSlot *ProcessSubtitleFile(const char *voiceId, std::uint32_t participantId, std::uint32_t flags, std::uint32_t *aliasState);

const char *ReadStringFromStringTableById(const SubtitleStringId &id);

const char *InitAndRetrieveStringFromID(const char *stringId);
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {
void LoadScreen_ResetUIStates();

std::int64_t LoadScreen_Destroy();

std::int64_t LoadScreen_IsSelected();

std::int64_t LoadScreen_Reset();

std::int64_t LoadScreen_UpdateNowLoading();
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {
std::int64_t TryInvokeRegisteredCallback(std::int32_t elementIndex);
}

namespace shenmue::s1::d3t {
void ActivateElementByIdx(std::int32_t elementIndex);

bool ResetElementByIdx(std::int32_t elementIndex);
} // namespace shenmue::s1::d3t

namespace shenmue::s1 {
void ReadSubtitleGlyphsAndSubs(std::int32_t language);

std::int64_t LoadSplash();
} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {



template <typename Frame> void PlayGatedUiSound(Frame &frame, std::uint32_t command) {
    if (frame + 5 > static_cast<Frame>(g_FrameCounter))
        return;
    frame = static_cast<Frame>(g_FrameCounter);
    shenExecuteAICACommand(command, 0, 0);
}
} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

extern UIBaseRuntime *const g_cinemaBars;

extern std::uint16_t g_tmpPadcHeld;

extern std::uint16_t g_tmpRightStickHi;

extern std::uint16_t g_tmpRightStickLo;

extern std::int32_t g_conversationPromptActivationLock;

void UI_ICONMainCallback(HLib::TaskData *);

void UI_ICSpMainCallback(HLib::TaskData *);

void UI_ICQuMainCallback(HLib::TaskData *);

void UI_ICNQMainCallback(HLib::TaskData *);

void UI_ICSVMainCallback(HLib::TaskData *);

void iconUpdateInputBuffer();

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

extern std::array<std::vector<std::byte>, 80> g_uiDefinitions;

extern std::array<std::atomic<bool>, 80> g_uiDefinitionReady;

extern std::int32_t g_fightGaugeDisplayMode;

extern std::array<std::int32_t, 2> g_fightGaugeWidgetIndices;

extern std::array<std::int32_t, 2> g_fightGaugeScissorIndices;

extern std::uint32_t g_fightGaugeFrameFlags;

extern float g_vfGaugeHealthPercent;

extern std::array<char, 512> g_gameHudWatchTime;

extern std::array<ActionSelectorChoiceUISlot, 4> g_actionSelectorChoiceSlots;

extern ActionSelectorChoiceUISlot *g_actionSelectorSelectedChoiceSlot;

extern std::array<d3t::UIWindow *, 4> g_actionSelectorChoicePanels;

extern std::array<d3t::UIWindow *, 3> g_actionSelectorMainPanels;

extern UIButtonIconWidget *g_actionSelectorCenterIcon;

extern d3t::UIImageWidget *g_actionSelectorPhoenixImage;

extern HLib::Task *g_sicnTask;

extern std::uint8_t g_sicnTaskActive;

extern std::int16_t g_sicnTransientStateTimeoutFrames;

extern std::array<SicnPromptSlot, 12> g_sicnPromptSlots;

extern std::uint16_t g_hudPromptFlagsThisFrame;

extern bool g_sicnPopulatedHudPromptsThisFrame;

extern bool g_sicnClearPendingUntilUnpause;

extern std::int32_t Shenmue_nPrevPauseState;

extern std::int32_t g_sfxFlashOnFrames;

extern std::int32_t g_sfxTimedOutMarker;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

extern UISfxOverlayState g_sfxOverlayState;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

extern std::uint8_t g_sfxOverlayFlags;

extern std::uint16_t g_currentSfxMask;

extern std::int16_t g_sfxTimeoutFrames;

extern std::int16_t g_sfxFrameCounter;

extern std::array<std::int16_t, 2> g_sfxOverlayResult;

extern SfxOverlayBoostFrames g_qteOverlay;

extern std::int16_t g_sfxOverlayTimeoutPendingBonusFrames;

extern std::int16_t g_qteOverlayTimeoutMaxFrames;

extern std::int32_t g_sfxOverlayTimeoutBoostEnabled;

extern std::int32_t g_sfxFlashOffFrames;

extern std::int32_t g_sfxOverlayTimeoutBoostCount;

extern std::uint8_t g_sfxFlashCounter;

extern std::uint8_t g_sfxStateDirty;

extern HLib::Task *g_sfxOverlayTask;

extern ActionSelectorOwner g_actionSelectorPresentation;

extern CashOverlayOwner g_cashOverlayPresentation;

extern UIActionSelectorElement g_actionSelectorElement;

extern MoveLearnedPresentationOwner g_moveLearnedPresentation;

extern UIGameMessagePresentation g_gameMessagePresentation;

extern OwnedSpriteSet<32> g_commonIconSprites;

void ActionSelectorSetSelectedChoice(std::int32_t choice);

std::int64_t WristwatchOverlay_IsPressed();

std::int64_t WristwatchOverlay_GetPromptIconSet();

void DestroyOwnedTask(HLib::Task *&task, std::int32_t expectedToken);

bool CapturePad(PadCaptureEntry *input, PadCaptureEntry *&previous);

void RestorePad(PadCaptureEntry *&previous);

void ReleasePadCapture(PadCaptureEntry *&input, PadCaptureEntry *&previous);

void ReleaseIconInput();

bool IsOwnedUiTask(
    const HLib::Task *task, std::int16_t queueIndex, std::int32_t token, HLib::TaskCallback initialCallback, HLib::TaskCallback mainCallback = nullptr);

void EnsureIconRootTask();

void EnsureSicnTask();

void UI_BindViewImages(UIDefinition &definition);

void UI_ResolveLabelScaleVars(UIDefinition &definition);

extern std::array<UILegacyFile, 80> g_uiLegacyFiles;

std::uint32_t OpenLegacyUIFile(const char *path, std::uint32_t index);

std::int32_t TryLoadUIFromFileHandle(std::uint32_t index, std::int32_t &slot);

void ActionSelectorSetChoiceLabelsFromTokenIds(const std::array<std::int32_t, 4> &tokens);

void ActionSelectorSetValueChoiceLabels(std::int32_t maximumValue);

std::int64_t GameMessage_IsSelected();

std::int64_t GameMessage_Tick();

std::int64_t GameMessage_Reset();

void GameMessage_Render();

std::int64_t GameMessage_Destroy();

extern std::array<std::int32_t, 12> g_uiSoundFrames;

extern std::array<d3t::UIWindow *, 7> g_informationBarWindows;

extern std::array<d3t::UILabelWidget *, 2> g_informationBarSideLabels;

extern std::array<UIIconLabelWidget *, 3> g_informationBarTextLabels;

extern std::array<d3t::UIImageWidget *, 3> g_informationBarIcons;

extern std::array<std::int32_t, 3> g_informationBarBackgroundImages;

extern std::uint32_t g_informationBarTextColour;

void InformationBar_Render();

std::int64_t InformationBar_IsSelected();

std::int64_t InformationBar_Reset();

std::int64_t InformationBar_Tick();

std::int64_t InformationBar_GetContentPanel();

std::int64_t ActionSelector_IsSelected();

extern std::int32_t g_actionSelectorInputCode;

extern std::uint16_t g_actionSelectorPreviousPressed;

void ActionSelector_Render();

std::int64_t ActionSelector_IsPressed();

std::int64_t ActionSelector_Tick();

std::int64_t ActionSelector_Destroy();

std::int64_t ActionSelector_GetPromptIconSet();

extern std::array<d3t::UIWindow *, 17> g_gameHudWindows;

extern d3t::UILabelWidget *g_gameHudCameraLabel;

extern UIColourImageWidget *g_gameHudCameraBase;

extern std::array<UIButtonIconWidget *, 6> g_gameHudPromptSprites;

extern std::array<UIButtonIconWidget *, 5> g_gameHudButtonIcons;

extern std::int32_t g_hudPromptKeyboardLayoutApplied;

extern std::array<std::int32_t, 4> g_hudPromptEffectiveSlots;

extern std::array<std::int32_t, 5> g_hudPromptOverrideSlots;

extern bool g_hudPromptOverrideActive;

extern std::int32_t g_hudPromptHighlightAnimSlot;

extern std::array<std::int32_t, 5> g_hudPromptIconAnimSlots;

void UI_UpdateSearchIcons();

std::int64_t GameHud_IsSelected();

std::int64_t GameHud_Tick();

std::int64_t GameHudWatch_IsSelected();

std::int64_t GameHudWatch_Tick();

std::uint32_t GameHudGauge_LoadUI();

std::int64_t GameHudGauge_IsSelected();

std::int64_t GameHudGauge_Tick();

std::int64_t ResetGlobalInput();

std::uint32_t SeventyManBattle_LoadUI();

extern std::array<std::int32_t, 6> g_70mbPanelWindows;

extern std::array<std::int32_t, 6> g_70mbPanelBindings;

extern std::array<std::int32_t, 8> g_70mbLabels;

std::int64_t SeventyManBattle_SetupUI();

std::int64_t SeventyManBattle_Tick();

std::int64_t SeventyManBattle_GetPromptIconSet();

std::uint32_t GameOver_LoadUI();

std::int64_t GameOver_Tick();

std::uint32_t PracticeHud_LoadUI();

std::int64_t PracticeHud_Tick();

std::int64_t PracticeHud_GetPromptIconSet();

std::int64_t GameBackground_Tick();

extern std::array<d3t::UILabelWidget *, 3> g_moveLearnedLabels;

std::int64_t GameMoveLearned_IsSelected();

std::int64_t GameMoveLearned_Tick();

std::int64_t GameMoveLearned_Reset();

std::int64_t HandleEwzoUiEvent(EwzoUiEvent event, const EwzoUiBinding &binding);

extern std::array<UIButtonIconWidget *, 4> g_qteMinigameKeyIcons;

extern std::array<UIButtonIconWidget *, 4> g_qteMinigamePadIcons;

std::uint32_t QteMinigame_ParseDefinition();

std::int64_t QteMinigame_IsSelected();

std::int64_t QteMinigame_Reset();

void QteMinigame_Render();

std::int64_t QteMinigame_Destroy();

std::int64_t QteMinigame_Tick();

extern std::array<UIButtonIconWidget *, 3> g_gameQteIcons;

extern std::array<d3t::UIWindow *, 19> g_gameQteWindows;

extern std::array<float, 3> g_gameQtePanelOffsets;

extern std::int32_t g_gameQteAnimSlot;

extern std::int32_t g_gameQteHidden;

extern std::uint16_t g_gameQteMask;

std::uint32_t GameQte_ParseDefinition();

std::int64_t GameQte_IsSelected();

std::int64_t GameQte_Tick();

std::int32_t UI_FindSpriteSheetByImageName(const char *name);

std::uint32_t RaceHud_ParseDefinition();

std::int64_t RaceHud_IsSelected();

std::int64_t RaceHud_Tick();

void RaceHud_Render();

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

extern std::array<std::uint32_t, 3> g_uiRefreshedElements;

extern std::array<std::uint32_t, 3> g_uiPreviousRefreshedElements;

void UI_GUITasksEnqueue();

extern std::array<std::array<char, 256>, 8> g_70mbLabelText;

extern std::int32_t g_70mbShownIconStatus;

bool convCanOpen();

extern std::int32_t g_qteSessionActive;

void UI_ICONClear();

void UI_ICMoWaitCloseAnimation();

void UI_ICMoCallback(HLib::TaskData *);

void UI_ICSpCancel();

void UI_ICSpCallback(HLib::TaskData *);

void UI_ICSVCallback(HLib::TaskData *);

extern InformationBarLabels g_informationBarLabels;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

extern std::int32_t g_trainingMovePickSoundFrame;

extern std::array<std::array<char, 512>, 3> g_informationBarLocalizedTexts;

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

extern GameTrainingElement g_gameTrainingElement;

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {

extern d3t::UILabelWidget *g_gameTrainingResultLabel;

extern std::array<d3t::UIImageWidget *, 5> g_gameTrainingLabelWidgets;

extern std::array<d3t::UIWindow *, 11> g_gameTrainingWindows;

extern std::array<std::int32_t, 84> g_gameTrainingLabelImageIndices;

std::int64_t GameTraining_IsSelected();

void GameTraining_Render();

std::int64_t GameTraining_IsPressed();

std::int64_t GameTraining_Tick();

std::int64_t GameTraining_Destroy();

extern std::int32_t g_gameTrainingLegacyUIViewSlot;

extern std::int32_t g_gameTrainingLegacyUILoadState;

extern std::uint32_t g_gameTrainingLegacyUIFileHandle;

extern const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> *g_gameTrainingLegacyView;

extern std::array<d3t::UILabelWidget *, 8> g_gameTrainingLegacyLabels;

extern std::array<d3t::UILabelWidget *, 2> g_gameTrainingLegacyDescriptions;

extern std::array<UIColourImageWidget *, 8> g_gameTrainingLegacyArrows;

extern std::array<UIColourImageWidget *, 4> g_gameTrainingLegacyGlows;

extern std::int32_t g_gameTrainingLegacySelection;

extern const char *g_uiReloadFileMemory;

std::int64_t GameTrainingWidgetUpdate();

std::int64_t GameTraining_ReloadLegacyUI();

extern std::int32_t g_gameHudSkipUILoadState;

extern std::uint32_t g_gameHudSkipUIFileHandle;

extern std::int32_t g_gameHudSkipUIViewSlot;

std::uint32_t LoadGameHudUI();

std::int64_t GameHudSkip_LoadState();

std::int64_t GameHudSkip_PostInit();

std::int64_t GameHudSkip_ReloadUI();

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

std::uint32_t ParseGameTraining();

void UI_ICQuCallback(HLib::TaskData *);

void UI_ICQuMainCallback(HLib::TaskData *taskData);

void UI_ICONCallback(HLib::TaskData *);

void UI_ICNQCallback(HLib::TaskData *);

void UI_SICNDispatch(std::int32_t mode, std::uint16_t state, UISicnPausePolicy pausePolicy);

std::int64_t GetIconSet();

std::int64_t SharedItems_IsPressed();

} // namespace shenmue::s1
