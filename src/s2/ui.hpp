#pragma once
#include "shenmue.h"

#include "s2/hlib.hpp"
#include "s2/item_effect.hpp"
#include "s2/money_icon.hpp"
#include "s2/sleep_action.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace shenmue::d3t {
struct UIWindow;
}

namespace shenmue::s2 {

struct DateTime;
struct VFMA_t;
struct MT7Model;
struct UIButtonWheel;
enum class ScreenMode : std::int32_t;

inline constexpr std::array<std::uint8_t, 16> selectorDirectionIcons{13, 0, 4, 13, 6, 7, 5, 13, 2, 1, 3, 13, 13, 13, 13, 13};
inline constexpr std::array<std::string_view, 6> UI_FONT_SIZE_NAMES{"H1", "H2", "H3", "BL", "B", "BS"};
inline constexpr std::array<std::string_view, 23> PRESS_START_PANEL_IDS{"", "", "", "PressStart::Logo", "", "PressStart::Logo_White", "Press_Start::Background",
    "Press_Start::Mural_White", "Press_Start::Mural_Blur", "", "", "", "", "PressStart::SegaCopyright", "Press_Start::Background_Lit_Left_1",
    "Press_Start::Background_Lit_Left_2", "Press_Start::Background_Lit_Left_3", "Press_Start::Background_Lit_Right_1", "Press_Start::Background_Lit_Right_2",
    "Press_Start::Background_Lit_Right_3", "PressStart::Logo_Shadow_Left", "PressStart::Logo_Shadow_Right", "PressStart::PressStartBox"};
inline constexpr std::array<std::string_view, 8> BIG_KANJI_PANEL_IDS{"BigKanjiText::PanelPlaceholder0", "BigKanjiText::PanelPlaceholder1",
    "BigKanjiText::PanelPlaceholder2", "BigKanjiText::PanelPlaceholder3", "BigKanjiText::PanelPlaceholder4", "BigKanjiText::PanelPlaceholder5",
    "BigKanjiText::PanelPlaceholder6", "BigKanjiText::PanelPlaceholder7"};
inline constexpr std::array<std::string_view, 8> BIG_KANJI_IMAGE_IDS{"BigKanjiText::ImgPlaceholder0", "BigKanjiText::ImgPlaceholder1",
    "BigKanjiText::ImgPlaceholder2", "BigKanjiText::ImgPlaceholder3", "BigKanjiText::ImgPlaceholder4", "BigKanjiText::ImgPlaceholder5",
    "BigKanjiText::ImgPlaceholder6", "BigKanjiText::ImgPlaceholder7"};
inline constexpr std::array<std::string_view, 11> QTE_MINIGAME_PANEL_IDS{"QTEMinigame::Panel_Excite_A", "QTEMinigame::Panel_Excite_B",
    "QTEMinigame::Panel_Excite_X", "QTEMinigame::Panel_Excite_Y", "QTEMinigame::Panel_Excite_PL", "QTEMinigame::Panel_Excite_PU",
    "QTEMinigame::Panel_Excite_PD", "QTEMinigame::Panel_Excite_PR", "QTEMinigame::Panel_Title_A", "QTEMinigame::Panel_Title_B", "QTEMinigame::Panel_Title_X"};
inline constexpr std::array<std::string_view, 8> QTE_MINIGAME_DIRECTION_IDS{
    "Multi_Left_PC", "Multi_Up_PC", "Multi_Down_PC", "Multi_Right_PC", "Multi_Left", "Multi_Up", "Multi_Down", "Multi_Right"};
inline constexpr std::array<const char *, 5> SUBTITLE_PANEL_IDS{"SubFullPanel", "SubHudPanel", "Sub_TopLine_Panel", "Sub_MidLine_Panel", "Sub_BotLine_Panel"};
inline constexpr std::array<const char *, 2> SUBTITLE_BACKGROUND_IDS{"Sub_Background_Full_Image", "Sub_Background_Hud_Image"};
inline constexpr std::array<std::array<const char *, 5>, 7> COLLECTION_GRID_WINDOWS{
    {{"Submenu_Collection_Gacha_Panel_GridHolder", "Submenu_Collection_Gacha::GridElementParent_#", "Submenu_Collection_Gacha::GridElement_#",
         "Submenu_Collection_Gacha::GridElement_Texture_#", "Submenu_Collection_Gacha::GridElement_Shadow_#"},
        {"Panel_Submenu_Collection_Maps_GridHolder", "Submenu_Collection_Maps::GridElementParent_#", "Submenu_Collection_Maps::GridElement_#",
            "Submenu_Collection_Maps::GridElement_Texture_#", "Submenu_Collection_Maps::GridElement_Shadow_#"},
        {"Submenu_Collection::Pawnshop_GridHolder", "Submenu_Collection_Pawnshop::GridElementParent_#", "Submenu_Collection_Pawnshop::GridElement_#",
            "Submenu_Collection_Pawnshop::GridElement_Texture_#", "Submenu_Collection_Pawnshop::GridElement_Shadow_#"},
        {"Submenu_Collection_Scrolls_Holder", "Submenu_Collection_Scrolls::GridElementParent_#", "Submenu_Collection_Scrolls::GridElement_#",
            "Submenu_Collection_Scrolls::GridElement_Texture_#", "Submenu_Collection_Scrolls::GridElement_Shadow_#"},
        {"SubMenu_Photos_Event::Panel_GridHolder", "SubMenu_Photos_Event::GridElementParent_#", "SubMenu_Photos_Event::GridElement_#",
            "SubMenu_Photos_Event::GridElement_Texture_#", "SubMenu_Photos_Event::GridElement_Shadow_#"},
        {"SubMenu_Photos_SnapShot::Panel_GridHolder", "SubMenu_Photos_SnapShot::GridElementParent_#", "SubMenu_Photos_SnapShot::GridElement_#",
            "SubMenu_Photos_SnapShot::GridElement_Texture_#", "SubMenu_Photos_SnapShot::GridElement_Shadow_#"},
        {"SubMenu_Photos_Secret::Panel_GridHolder", "SubMenu_Photos_Secret::GridElementParent_#", "SubMenu_Photos_Secret::GridElement_#",
            "SubMenu_Photos_Secret::GridElement_Texture_#", "SubMenu_Photos_Secret::GridElement_Shadow_#"}}};
inline constexpr std::array<std::array<const char *, 4>, 7> COLLECTION_GRID_LAYOUT_ITEMS{
    {{"Submenu_Collection_Gacha::GridElementParent_Background_Spacepush", "Submenu_Collection_Gacha::GridElementBackground_Game_Shading"},
        {"Submenu_Collection_Maps::GridElementParent_Background_Spacepush", "Submenu_Collection_Maps::GridElementBackground_Game_Shading"},
        {"Submenu_Collection_Pawnshop::GridElementParent_Background_Spacepush", "Submenu_Collection_Pawnshop::GridElementBackground_Game_Shading"},
        {"Submenu_Collection_Scrolls::GridElementParent_Background_Spacepush", "Submenu_Collection_Scrolls::GridElementBackground_Game_Shading"},
        {"SubMenu_Photos_Event::GridElementParent_Background_Spacepush", "SubMenu_Photos_Event::GridElementBackground_Game_Shading"},
        {"SubMenu_Photos_SnapShot::GridElementParent_Background_Spacepush", "SubMenu_Photos_SnapShot::GridElementBackground_Game_Shading",
            "SubMenu_Photos_SnapShot::GridElementBackground_Front_Shading", "SubMenu_Photos_SnapShot::GridElementBackground_Selected_Spacepush"},
        {"SubMenu_Photos_Secret::GridElementParent_Background_Spacepush", "SubMenu_Photos_Secret::GridElementBackground_Game_Shading",
            "SubMenu_Photos_Secret::GridElementBackground_Front_Shading", "SubMenu_Photos_Secret::GridElementBackground_Selected_Spacepush"}}};

inline constexpr shen_name TASK_ICQU = SHEN_NAME(ICQu);
inline constexpr std::array<float, 4> QUICK_ICON_POSITION{-176.0f, 240.0f, 1.0f, 0.0f};
inline constexpr std::array<std::uint16_t, 11> QUICK_ICON_BUTTON_TYPES{3, 7, 23, 11, 55, 67, 131, 135, 151, 769, 1047};
inline constexpr std::array<std::uint16_t, 5> QUICK_ICON_FACE_ICONS{0x700, 0x701, 0x702, 0x703, 0};

inline constexpr std::array<std::uint16_t, 16> quickIconDirectionalStatuses{0, 1, 16, 0, 64, 128, 32, 0, 4, 2, 8, 0, 0x4828, 0x544c, 0x7361, 0x466b};
inline constexpr std::array<std::uint32_t, 2> QUICK_ICON_FIRST_SOUND{0x900abU, 0x15abU}, QUICK_ICON_SECOND_SOUND{0xa00abU, 0x115abU};

#pragma pack(push, 1)
struct LegacyIconSprite {
    float x;
    float y;
    float scaleX;
    float scaleY;
    float depth;
    std::uint16_t flags;
    std::uint16_t reserved16;
    std::uint32_t rotation;
    std::uint16_t icon;
    std::int16_t width;
    std::int16_t height;
    std::uint8_t alignment;
    std::array<std::byte, 1> reserved23;
};
#pragma pack(pop)
static_assert(sizeof(LegacyIconSprite) == 0x24);
static_assert(offsetof(LegacyIconSprite, flags) == 0x14);
static_assert(offsetof(LegacyIconSprite, rotation) == 0x18);
static_assert(offsetof(LegacyIconSprite, width) == 0x1e);
static_assert(offsetof(LegacyIconSprite, alignment) == 0x22);

struct BigKanjiGlyph {
    std::uint32_t texture;
    std::uint32_t admittedTexture;
    LegacyIconSprite *sprite;
    std::uint32_t colour;
    std::uint8_t requested;
    std::array<std::byte, 3> reserved;
};
static_assert(sizeof(BigKanjiGlyph) == 0x18);
static_assert(offsetof(BigKanjiGlyph, admittedTexture) == 0x04);
static_assert(offsetof(BigKanjiGlyph, sprite) == 0x08);
static_assert(offsetof(BigKanjiGlyph, colour) == 0x10);
static_assert(offsetof(BigKanjiGlyph, requested) == 0x14);

inline constexpr std::array<std::uint32_t, 8> DDS_PIXEL_RGB565{32, 64, 0, 16, 63488, 2016, 31, 0};
inline constexpr std::array<std::uint32_t, 8> DDS_PIXEL_ARGB4444{32, 65, 0, 16, 3840, 240, 15, 61440};
inline constexpr std::array<std::uint32_t, 8> DDS_PIXEL_DXT1{32, 4, 827611204, 0, 0, 0, 0, 0};
inline constexpr std::array<std::uint32_t, 8> DDS_PIXEL_DXT3{32, 4, 861165636, 0, 0, 0, 0, 0};
inline constexpr std::array<std::uint32_t, 8> DDS_PIXEL_DXT5{32, 4, 877942852, 0, 0, 0, 0, 0};
inline constexpr std::array<std::uint32_t, 8> DDS_PIXEL_BGRA8{32, 65, 0, 32, 16711680, 65280, 255, 4278190080U};

void InitializeLegacyIconSprite(LegacyIconSprite &sprite) noexcept;
void PublishBigKanjiText(const char *spriteName, LegacyIconSprite *sprites, std::int32_t count);
void PublishBigKanjiGlyph(std::int32_t index, std::uint32_t colour);
void ReleaseBigKanjiText();
std::uint8_t DecodeSelectorInput(std::uint16_t pressed, std::uint8_t allowedDirections) noexcept;
bool AdvanceIconPosition(float &position, float &velocity, float target) noexcept;
bool RetreatIconPosition(float &position, float &velocity, float target) noexcept;
std::uint32_t GetTextureStorageSize(std::uint16_t format, std::int32_t width, std::int32_t height) noexcept;
void ParseGuiPtx(std::byte *buffer, std::uint16_t width, std::uint16_t height, std::uint32_t idLow, std::uint32_t idHigh, std::uint16_t format = 0x8089);
float GetScreenAlignmentOffset() noexcept;
std::uint32_t RasterizeItemLabelNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) noexcept;
std::span<const std::byte> SetupPtxTexture(std::uint16_t width, std::uint32_t idLow, std::uint32_t idHigh);
std::int64_t CheckTextureData();
void ReinitTextureData();
std::uint32_t RefreshItemSelectorInventory(std::uint32_t bank) noexcept;
std::span<const std::uint8_t, 64> GetItemSelectorOwnedMask() noexcept;
void PublishItemSelectorOwnedMask(const std::uint8_t *mask) noexcept;
const std::uint8_t *GetPublishedItemSelectorOwnedMask() noexcept;
std::uint16_t PublishLegacyItemLabel(const char *text, std::uint16_t width) noexcept;
std::int64_t PrepareLegacyInventoryItemLabel(std::uint16_t &label, std::uint32_t item);
void SetLegacyItemSelectorExitFlag() noexcept;
std::int64_t RequestItemSelector();
void SetItemSelectorDelayedResult(std::uint16_t result) noexcept;
inline constexpr std::array<const char *, 35> MAIN_MENU_WHEEL_IMAGES{"LT_NewGame_En", "LT_Continue_En", "LT_Collection_En", "LT_Options_En", "LT_Shots_En",
    "LT_NewGame_Jp", "LT_Continue_Jp", "LT_Collection_Jp", "LT_Options_Jp", "LT_Shots_Jp", "LT_NewGame_Fr", "LT_Continue_Fr", "LT_Collection_Fr",
    "LT_Options_Fr", "LT_Shots_Fr", "LT_NewGame_De", "LT_Continue_De", "LT_Collection_De", "LT_Options_De", "LT_Shots_De", "LT_NewGame_Kr", "LT_Continue_Kr",
    "LT_Collection_Kr", "LT_Options_Kr", "LT_Shots_Kr", "LT_NewGame_Ct", "LT_Continue_Ct", "LT_Collection_Ct", "LT_Options_Ct", "LT_Shots_Ct", "LT_NewGame_Cs",
    "LT_Continue_Cs", "LT_Collection_Cs", "LT_Options_Cs", "LT_Shots_Cs"};

enum class UIElement : std::int32_t {
    NotebookPages = 1,
    FightGauge = 2,
    GameScreenFade = 3,
    CinemaBars = 4,
    MainMenu = 5,
    GameMenu = 6,
    GameBackground = 7,
    SharedItems = 8,
    SubMenuItems = 9,
    SubMenuCollection = 10,
    CollectionGacha = 11,
    CollectionMapList = 12,
    CollectionPawnList = 13,
    CollectionScrolls = 14,
    SubMenuMoves = 15,
    SubMenuOptions = 16,
    OptionsAudio = 17,
    OptionsControls = 18,
    OptionsGraphics = 19,
    SubMenuPhotos = 20,
    PhotosEvents = 21,
    PhotosSnapshot = 22,
    PhotosSecrets = 23,
    SnapshotViewer = 24,
    SecretViewer = 25,
    MainMenuWheel = 26,
    MainMenuNew = 27,
    MainMenuContinue = 28,
    ShenmueCollection = 29,
    Options = 30,
    MoveLearned = 31,
    OptionsGamma = 32,
    GameHudMoney = 33,
    GameHudWatch = 34,
    StreetFighterHud = 35,
    DuckRace = 36,
    GameMessage = 37,
    Subtitles = 38,
    ActionSelector = 39,
    PawnshopFlyer = 40,
    PawnshopFlyer1 = 41,
    PawnshopFlyer2 = 42,
    GameHome = 44,
    GameMoves = 45,
    GameCollection = 46,
    GameWatch = 47,
    GameQuit = 48,
    GameNotebook = 49,
    GameHudTimer = 50,
    BigKanjiText = 51,
    CollectionMaps = 70,
    CollectionPawnshop = 53,
    GameQte = 54,
    CollectionBackground = 55,
    OptionsBackground = 56,
    LoadSave = 57,
    YesNoPrompt = 58,
    GameInfoBar = 59,
    Splash = 60,
    PressStart = 61,
    IntroCutsceneCredits = 62,
    PlatformIcons = 63,
    YesNo = 58,
    Info = 59,
    GameHud = 64,
    HudSnapshot = 65,
    CutsceneSkip = 66,
    QteMinigame = 67,
    WristwatchOverlay = 69,
    CollectionMapsEnglish = 71,
    CollectionMapsFrench = 72,
    CollectionMapsGerman = 73,
    CollectionMapsJapanese = 75,
    CollectionMapsKorean = 76,
    CollectionMapsTraditionalChinese = 77,
    CollectionMapsSimplifiedChinese = 78,
    GameHelp = 80,
    Credits = 81,
    LoadingScreen = 82,
    Count = 83,
};
inline constexpr std::array<UIElement, 7> COLLECTION_MAP_LANGUAGE_ELEMENTS{UIElement::CollectionMapsEnglish, UIElement::CollectionMapsJapanese,
    UIElement::CollectionMapsFrench, UIElement::CollectionMapsGerman, UIElement::CollectionMapsKorean, UIElement::CollectionMapsTraditionalChinese,
    UIElement::CollectionMapsSimplifiedChinese};
inline constexpr std::array<const char *, 7> COLLECTION_MAP_LANGUAGE_FILES{"/ui/gamemenu_new/maps/Collection_Maps_EN.ui",
    "/ui/gamemenu_new/maps/Collection_Maps_JP.ui", "/ui/gamemenu_new/maps/Collection_Maps_FR.ui", "/ui/gamemenu_new/maps/Collection_Maps_DE.ui",
    "/ui/gamemenu_new/maps/Collection_Maps_KR.ui", "/ui/gamemenu_new/maps/Collection_Maps_TC.ui", "/ui/gamemenu_new/maps/Collection_Maps_SC.ui"};
inline constexpr shen_name TASK_STAF = SHEN_NAME(STAF);
inline constexpr shen_name STAFF_ROLL_SPRITE = SHEN_NAME(STZZ);
inline constexpr shen_name TASK_OPST = SHEN_NAME(OPST);
inline constexpr shen_name OPENING_STAFF_ROLL_SPRITE = SHEN_NAME(OPSS);

struct OpeningStaffRollTaskData {
    std::int32_t textures[3];
    float alpha;
    std::int32_t item;
    std::int32_t phase;
    float clock;
};
static_assert(sizeof(OpeningStaffRollTaskData) == 0x1c);
static_assert(alignof(OpeningStaffRollTaskData) == 4);
static_assert(offsetof(OpeningStaffRollTaskData, textures) == 0x00);
static_assert(offsetof(OpeningStaffRollTaskData, alpha) == 0x0c);
static_assert(offsetof(OpeningStaffRollTaskData, item) == 0x10);
static_assert(offsetof(OpeningStaffRollTaskData, phase) == 0x14);
static_assert(offsetof(OpeningStaffRollTaskData, clock) == 0x18);

struct EndingStaffRollTaskData {
#if SHENMUE_IS_PC
    void *resource;
    std::array<std::byte, 0x18> reserved08;
    void *resourceCopy;
    std::array<std::byte, 0x10> reserved28;
    std::uint32_t field38;
    std::uint32_t reserved3c;
    std::uint32_t field40;
    std::array<std::byte, 0x0c> reserved44;
    HLib::Task *screenFilmTask;
    std::array<std::byte, 0x14> reserved58;
    std::uint32_t field6c;
    std::uint32_t reserved70;
    std::uint32_t field74;
    std::array<std::byte, 0x18> reserved78;
#else
    void *resource;
    std::array<std::byte, 0x0c> reserved04;
    void *resourceCopy;
    std::array<std::byte, 0x08> reserved14;
    std::uint32_t field38;
    std::uint32_t reserved20;
    std::uint32_t field40;
    std::array<std::byte, 0x08> reserved28;
    HLib::Task *screenFilmTask;
    std::array<std::byte, 0x0c> reserved34;
    std::uint32_t field6c;
    std::uint32_t reserved44;
    std::uint32_t field74;
    std::array<std::byte, 0x10> reserved4c;
#endif
};

static_assert(offsetof(EndingStaffRollTaskData, resource) == 0x00);
#if SHENMUE_IS_PC
static_assert(sizeof(EndingStaffRollTaskData) == 0x90);
static_assert(offsetof(EndingStaffRollTaskData, resourceCopy) == 0x20);
static_assert(offsetof(EndingStaffRollTaskData, field38) == 0x38);
static_assert(offsetof(EndingStaffRollTaskData, field40) == 0x40);
static_assert(offsetof(EndingStaffRollTaskData, screenFilmTask) == 0x50);
static_assert(offsetof(EndingStaffRollTaskData, field6c) == 0x6c);
static_assert(offsetof(EndingStaffRollTaskData, field74) == 0x74);
static_assert(alignof(EndingStaffRollTaskData) == 8);
#else
static_assert(sizeof(EndingStaffRollTaskData) == 0x5c);
static_assert(offsetof(EndingStaffRollTaskData, resourceCopy) == 0x10);
static_assert(offsetof(EndingStaffRollTaskData, field38) == 0x1c);
static_assert(offsetof(EndingStaffRollTaskData, field40) == 0x24);
static_assert(offsetof(EndingStaffRollTaskData, screenFilmTask) == 0x30);
static_assert(offsetof(EndingStaffRollTaskData, field6c) == 0x40);
static_assert(offsetof(EndingStaffRollTaskData, field74) == 0x48);
#endif

enum class InputType : std::int32_t {
    Menu = 0,
    GameUi = 1,
    Game = 2,
    Fight = 3,
    Qte2 = 4,
    Qte = 5,
    Vehicle = 6,
};

enum class MainMenuItem : std::int32_t {
    NewGame = 0,
    Continue = 1,
    Collection = 2,
    Options = 3,
    Shots = 4,
};

enum class FrontendRoute : std::int32_t {
    Wheel = 0,
    NewGame = 1,
    Continue = 2,
    TitleLoad = 3,
    SeventyMan = 4,
    Options = 5,
};

enum class SleepSelectionResult : std::int32_t {
    Pending = 0,
    Cancelled = 1,
    Selected = 2,
};
enum class DialogueSelectionResult : std::int32_t {
    Pending = 0,
    Selected = 1,
    Cancelled = 2,
    TimedOut = 3,
};

struct DialogueSelectionPromptState {
    std::array<std::uint16_t, 4> flagIds{0xffff, 0xffff, 0xffff, 0xffff};
    std::array<std::string_view, 4> labels{};
    std::uint16_t defaultFlagId{0xffff};
    std::uint16_t timeoutFrames{};
    std::uint32_t elapsedFrames{};
    std::int32_t focusedSlot{-1};
    bool active{};
    bool textAvailable{};
};

struct SubtitlePresentationState {
    std::array<char, 13> token{};
    std::string_view text{};
    std::array<std::string_view, 3> lines{};
    float lineScaleY{0.33f};
    std::array<std::uint32_t, 3> colours{};
    float cinemaOffset{};
    std::int32_t language{};
    std::uint32_t durationFrames{};
    std::uint32_t remainingFrames{};
    bool active{};
};

struct UIConfig {
    std::int32_t init;
    std::int16_t screenWidth;
    std::int16_t screenHeight;
    std::int32_t uiHandleIndex;
    std::int32_t enabled;
    std::int32_t status;
};

using UIAction = std::int64_t (*)();

struct UIElementState {
    const char *name;
    std::int32_t index;
    std::int32_t type;
    std::uint32_t (*parseDefinitionFromFile)();
    UIAction isSelected;
    UIAction isPressed;
    UIAction unknown28;
    UIAction render;
    UIAction postInit;
    UIAction unknown40;
    UIAction tick;
    void (*initialize)();
    UIAction destroy;
    UIAction unknown60;
    UIAction reset;
    UIAction unknown70;
    UIAction unknown78;
    UIAction unknown80;
    UIAction unknown88;
    UIAction resetRender;
    UIAction unknown98;
    std::int32_t isVisible;
    std::int32_t enabled;
    std::int32_t isRenderable;
    std::int32_t fieldAc;
    std::int32_t setup;
    std::int32_t someFlag3;
    char uiConfigFilepath[260];
    UIConfig config;
};

static_assert(sizeof(UIConfig) == 0x14);
static_assert(sizeof(UIElementState) == 0x1d0);
static_assert(offsetof(UIElementState, isVisible) == 0xa0);
static_assert(offsetof(UIElementState, setup) == 0xb0);
static_assert(offsetof(UIElementState, config) == 0x1bc);

void InitializeNuklearUI(std::uint32_t renderHandle);
void UI_BeginFrame();
extern std::int32_t g_uiDebugRevealAspectBars;
extern std::int32_t g_uiHelpAdmission;
extern float g_uiHelpAdmissionValue;
extern std::int32_t g_uiFreeInputOverride;
void SetIsDrawingUI(std::int32_t drawing);
bool IsNonBettingHelpTopic();
extern std::array<std::uint32_t, 3> g_uiRefreshedElements;
extern std::array<std::uint32_t, 3> g_uiPreviousRefreshedElements;
extern std::array<std::uint32_t, 3> g_uiPreviousInputElements;
void UpdateCollectionMapLanguage(std::int32_t language);
void RefreshMainMenuTitle();
std::int32_t GetButtonWheelDirection(std::int32_t index);
extern UIButtonWheel *g_mainMenuWheelWidget;
extern std::int32_t g_mainMenuWheelIndex;
extern std::int32_t g_mainMenuWheelInputBlock;
extern std::uint8_t g_mainMenuWheelPending;
void UpdateMainMenuWheel();
extern std::array<std::int32_t, 35> g_mainMenuWheelImages;
void RefreshMainMenuWheelDescription();
std::intptr_t RefreshMainMenuWheelLanguage();
std::intptr_t ResetMainMenuWheel();
void RefreshOptionsLanguage();
void RetireCollectionMenuResources();
void ShowElementByIndex(UIElement element);
std::int64_t ExecuteElementCallback(UIElement element);
std::uint32_t ReInitElement(UIElement element);
std::int64_t DestroyElementByIndex(UIElement element);
void CloseVisibleUIElements();
void ResetPressStartLogoImages();
void ResetGameMenuActive();
bool IsGameUIActive();
void SetMoveLearnedUiLine(std::int32_t index, std::string_view token);
bool IsSharedItemsWidgetOwnedBy(std::int32_t owner) noexcept;
std::int64_t ShowSharedItemsWidget(std::uint32_t itemType, std::uint32_t owner);
std::int64_t DestroySharedItemsWidget();
std::int64_t EnableSnapshotViewer();
bool ShowSnapshotViewer(std::uint32_t slot);
std::int32_t IsCameraIconHidden() noexcept;

extern std::int32_t requestedCameraIconMode;
void SetDebugHideCameraIcon(std::int32_t hidden) noexcept;
void SetupCameraIcon() noexcept;
std::int32_t IsHudFightSpriteActive() noexcept;
std::int32_t GetCurrentUIInputType() noexcept;
bool GetIsFreeInputType(std::int32_t type = -1);
bool IsGameUiSecondaryFlagSet(std::int32_t index);
bool HandleGameUIVisibility();
bool IsHelpOverlayVisible(UIElement element) noexcept;
bool IsElementEnabled(UIElement element) noexcept;
bool IsElementAvailable(UIElement element) noexcept;
void UpdateCompass(float rotation) noexcept;
void UpdateMinuteHand(float rotation) noexcept;
void UpdateHourHand(float rotation) noexcept;
void UpdateCompassImage(std::int32_t enabled, std::int32_t alpha) noexcept;
bool IsEasyOptionActive() noexcept;
void SetNewEasyOptionState(std::int32_t mode) noexcept;
bool AbortEasyOptionState(std::int32_t requested) noexcept;
std::uint8_t GetEasyOptionValue(std::uint32_t row) noexcept;
std::uint32_t GetEasyOptionSelectedRow() noexcept;
HLib::Task *QueueEasyOptionTask();
void ExecuteFightGaugeUI() noexcept;
void UpdateFightRoundMarkers(std::int32_t playerRounds, std::int32_t enemyRounds);
void SetHelpIconState() noexcept;
extern std::uint32_t g_helpIconState;
void EnableElement(UIElement element);
void DisableElement(UIElement element);
void SetUIRenderSuppression(std::int32_t suppressed);
void RenderUIFrame();
void SetContinueAvailability(bool available);
bool ContinueIsAvailable();
MainMenuItem GetMainMenuSelection();
bool SetMainMenuSelection(MainMenuItem item);
void MoveMainMenuSelection(std::int32_t delta);
void ResetLoadingScreen();
void SetupLoadingScreen(shen_name area, shen_name subarea, const DateTime &dateTime, const char *label, std::uint32_t flags, std::int32_t floor,
    std::int32_t fadeMode, std::int32_t entry);
bool GetIsLoadScreenActive() noexcept;
void ToggleLoadingScreen(std::int32_t vSyncCount, std::int32_t mode);
void UpdateLoadingScreenEnable();
void InstallLoadingScreenCallbacks() noexcept;
extern std::int32_t g_shouldShowLoading;
void SetFrontendRoute(FrontendRoute route);
void ReturnToTitleWheel();
void RenderUIContents();

std::int64_t RefreshNotebook();
void SetUIElementIndex(std::int32_t index);
void UpdateUI();
void RefreshAll(std::uint32_t first, std::uint32_t end);
void HandleUIStates();
void HandleUIPressedStates();
void DrawViews();
std::intptr_t UpdateScreenScaling();
void ProcessTitleMenuInput();
void Queue_OP_StaffRoll();
void StaffRoll(void *taskData);
bool GetHasCreditsStillGotTimeRemaining() noexcept;
std::int32_t GetEndingStaffRollFlag() noexcept;
void SetEndingStaffRollFlag(std::int32_t value) noexcept;
void ProcessTitleOptionsInput();
void NotifyUIEnterPressed();
void NotifyUICancelPressed();
bool ConsumeUIEnterPressed();
bool ConsumeUICancelPressed();
void BeginSleepSelectionPrompt(std::int32_t iconState);
SleepSelectionResult PollSleepSelectionResult();
void EndSleepSelectionPrompt();
std::int32_t GetSleepSelectionIconId();
void BeginDialogueSelectionPrompt(const std::array<std::uint16_t, 4> &flagIds, std::uint16_t timeoutFrames, std::uint16_t defaultFlagId = 0xffff);
DialogueSelectionResult PollDialogueSelectionPrompt(std::uint16_t &flagId);
void EndDialogueSelectionPrompt();
bool SetDialogueSelectionFocus(std::size_t slot);
void MoveDialogueSelectionFocus(std::int32_t delta);
bool GetDialogueSelectionPromptState(DialogueSelectionPromptState &state);


bool ProcessSubtitleText(std::string_view rawMessage);
std::uint32_t ProcessVoiceFlags(std::int32_t speakerMode);
bool BeginSubtitlePresentation(std::string_view token, std::string_view localizedText, std::int32_t language, std::uint32_t durationFrames);
bool TickSubtitlePresentation();
void EndSubtitlePresentation();
bool GetSubtitlePresentationState(SubtitlePresentationState &state);
void SetForceSkipSplashScreens(bool enabled);
bool PressStartIsReady();
void FreezePressStartPresentation();
void RestorePressStartPresentation();
void AnimatePressStartWidget(std::int32_t widget, float duration, float from, float to);
void AnimatePressStartAngle(float duration, float from, float to);
bool MainMenuIsReady();
bool MainMenuFrameWasRendered();
bool SplashSequenceComplete();
std::int32_t GetSplashScreenIndex();
void SetScreenFilmColour(std::uint32_t colour);
d3t::UIWindow *ToggleCinemaBars(std::int32_t mode);
void SubmitScreenColourOverlay(std::uint32_t colour);


#pragma pack(push, 1)
struct UIMessageBox {
    // External UI-context handle; imageFrame below is that context's local image index.
    std::int32_t imageIndex;
    std::int32_t widgetIndex;
    Vector4 bounds;
    std::int32_t imageFrame;
    std::uint8_t hideTarget;
    std::array<std::byte, 3> reserved1d;

    std::uint32_t texture;
    std::uint32_t reserved24;
    std::uint16_t textureWidth, textureHeight, cropX, cropY, cropWidth, cropHeight;
    std::array<std::byte, 12> reserved34;
    const char *primaryText;
    std::uint8_t primaryLocalized;
    std::array<std::byte, 7> reserved49;
    const char *secondaryText;
    std::uint8_t secondaryLocalized;
    std::array<std::byte, 7> reserved59;
    const char *tertiaryText;
    std::uint8_t tertiaryLocalized;
    std::uint8_t overrideColour;
    std::uint32_t colour;
    std::array<std::byte, 2> reserved6e;
};
#pragma pack(pop)
static_assert(sizeof(UIMessageBox) == 0x70);
static_assert(offsetof(UIMessageBox, bounds) == 8);
static_assert(offsetof(UIMessageBox, primaryText) == 0x40);
static_assert(offsetof(UIMessageBox, texture) == 0x20);
static_assert(offsetof(UIMessageBox, colour) == 0x6a);
const UIMessageBox &GetCurrentUIMessageBox() noexcept;
void CopyUIMessageBox(const UIMessageBox *message);
void ShowTemporaryUIMessageBox(const UIMessageBox *message, float seconds);
std::int32_t CheckAllSaveFiles(std::int32_t first, std::int32_t source);
float GetUIViewportWidth() noexcept;
std::int32_t GetDebugForceUIEnabled(UIElement element) noexcept;
void SetStreetFighterTimerDigits(const std::array<std::int32_t, 6> &digits);
void SetGameHudTimerCaption(const char *text, std::uint32_t colour);
std::int64_t CopyHudHintIds(std::span<const std::int16_t> ids);
void PublishHudHints(const std::uint32_t *hints, bool overrideHints);
inline std::array<std::int16_t, 5> g_uiFixedMenuHintIds{-1, 44, -1, -1, -1};
void ConstructNumStrikesLeftText(std::int32_t strikes);
void GetSFLevel(std::int32_t level);
void SetFightRoundPrompt(std::int32_t round);
void ResetStreetFighterHUD();
void vfSetupFightUiAndCameraState(VFMA_t *game);
void vfRestorePostFightUIState(VFMA_t *game);
inline constexpr std::array<const char *, 2> UI_STREET_FIGHTER_PANELS{"StreetFighter_Prompt_Panel", "StreetFighter::RoundPanel"};
inline constexpr std::array<const char *, 4> UI_STREET_FIGHTER_LABELS{"Time_Label", "Level_Label", "Strikes_Label", "StreetFighter::RoundLabel"};
void SetDuckRaceTimerCaption(const char *text);
void SetDuckRacePlacementRow(std::int32_t row, const char *text, std::int32_t bowtie);
inline constexpr std::array<const char *, 13> UI_DUCK_RACE_PANELS{"DuckRace::Rank1BowtiePanel", "DuckRace::Rank2BowtiePanel", "DuckRace::Rank3BowtiePanel",
    "DuckRace::Rank4BowtiePanel", "DuckRace::Rank5BowtiePanel", "DuckRace::Rank6BowtiePanel", "DuckRace::Rank1Panel", "DuckRace::Rank2Panel",
    "DuckRace::Rank3Panel", "DuckRace::Rank4Panel", "DuckRace::Rank5Panel", "DuckRace::Rank6Panel", "DuckRace::RaceTimerPanel"};
inline constexpr std::array<const char *, 7> UI_DUCK_RACE_LABELS{"DuckRace:Rank1Label", "DuckRace:Rank2Label", "DuckRace:Rank3Label", "DuckRace:Rank4Label",
    "DuckRace:Rank5Label", "DuckRace:Rank6Label", "DuckRace:TimerLabel"};
inline constexpr std::array<const char *, 6> UI_DUCK_RACE_BOWTIES{
    "DuckRace::Rank1Bowtie", "DuckRace::Rank2Bowtie", "DuckRace::Rank3Bowtie", "DuckRace::Rank4Bowtie", "DuckRace::Rank5Bowtie", "DuckRace::Rank6Bowtie"};
inline constexpr std::array<const char*, 3> UI_FIGHT_ROUND_TOKENS{"UIROUND001", "UIROUND002", "UIROUND003"}; // @text UIROUND001 EN: "ROUND 1" | JA: "第１ラウンド" ; UIROUND002 EN: "ROUND 2" | JA: "第２ラウンド" ; UIROUND003 EN: "FINAL ROUND" | JA: "最終ラウンド"
inline constexpr std::array<const char *, 7> UI_INFORMATION_PANELS{"Information_Bar::InfoBoxParent_FrontEnd", "Information_Bar::InfoBoxPanelFrontEnd",
    "Information_Bar::InfoBoxParent_InGame", "Information_Bar::InfoBoxPanelInGame_Texture", "Information_Bar::InfoBoxPanelInGame",
    "Information_Bar::InfoBoxLabels", "Information_Bar::Panel_FocusOutline"};
inline constexpr std::array<const char *, 5> UI_INFORMATION_LABELS{
    "Label_InfoBox_1Line", "Label_InfoBox_2Line1", "Label_InfoBox_2Line2", "Label_InfoBox_RightText", "Label_InfoBox_LeftText"};
inline constexpr std::array<const char *, 3> UI_INFORMATION_IMAGES{"Image_InfoBar_Left", "Image_InfoBar_Right", "Image_Background"};
inline constexpr std::array<const char *, 3> UI_INFORMATION_BACKGROUNDS{
    "InfoBarBackground_Ingame", "InfoBarBackground_Frontend", "InfoBarBackground_Frontend_Shadow"};
inline constexpr std::array<const char *, 6> UI_MOVES_COLOUR_BUTTONS{"GameMoves::MainPanelButton", "GameMoves::DummyMoveBar_Background",
    "GameMoves::DummyMoveBar_Bar", "GameMoves::PowerBar_Background", "GameMoves::SpeedBar_Background", "GameMoves::RigidBar_Background"};
inline constexpr std::array<std::uint32_t, 6> UI_MOVES_FRONT_COLOURS{0xff3f6a70U, 0x4628414bU, 0xe6416982U, 0x4628414bU, 0x4628414bU, 0x4628414bU};
inline constexpr std::array<std::uint32_t, 6> UI_MOVES_GAME_COLOURS{0xff322b1cU, 0xdc272415U, 0xe6507e8aU, 0xdc272415U, 0xdc272415U, 0xdc272415U};
inline constexpr std::array<const char *, 25> UI_MOVES_ROW_LAYOUTS{"GameMoves::DummyMove_Spacepush_#", "GameMoves::DummyMove_Padded_Spacepush_#",
    "GameMoves::DummyMove_Bar_Spacepush_#", "GameMoves::DummyMove_BarShadow_Spacepush_#", "GameMoves::DummyMove_BarShadow2_Spacepush_#",
    "GameMoves::DummyMove_LabelShadow_Spacepush_#", "GameMoves::DummyMove_Label_Spacepush_#", "GameMoves::DummyMove_Button_01_Spacepush_#",
    "GameMoves::DummyMove_Button_02_Spacepush_#", "GameMoves::DummyMove_Button_03_Spacepush_#", "GameMoves::DummyMove_Button_04_Spacepush_#",
    "GameMoves::DummyMove_Button_05_Spacepush_#", "GameMoves::DummyMove_Button_06_Spacepush_#", "GameMoves::DummyMove_Plus_01_Spacepush_#",
    "GameMoves::DummyMove_Plus_02_Spacepush_#", "GameMoves::DummyMove_Plus_03_Spacepush_#", "GameMoves::DummyMove_Plus_04_Spacepush_#",
    "GameMoves::DummyMove_Plus_05_Spacepush_#", "GameMoves::DummyMove_Crit_Spacepush_#", "GameMoves::DummyMove_ArrowLeftShadow_Spacepush",
    "GameMoves::DummyMove_ArrowLeftLight_Spacepush", "GameMoves::DummyMove_ArrowLeftMain_Spacepush", "GameMoves::DummyMove_ArrowRightShadow_Spacepush",
    "GameMoves::DummyMove_ArrowRightLight_Spacepush", "GameMoves::DummyMove_ArrowRightMain_Spacepush"};


struct LegacyUISpritePrimitive {
    std::uint32_t texture{};
    float centerX{};
    float centerY{};
    float width{};
    float height{};
    float u0{};
    float v0{};
    float u1{1.0f};
    float v1{1.0f};
    std::uint32_t colour{0xffffffffU};
};
bool DrawLegacyUISpritePrimitive(const LegacyUISpritePrimitive &primitive);

bool DrawLegacyUIColourPrimitive(float centerX, float centerY, float width, float height, std::uint32_t colour);
void DrawMoneyAmount(float x, float y, float depth, float scale, std::int32_t amount, std::uint8_t cash, std::uint32_t colour, ScreenMode mode);
void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour, ScreenMode mode);

struct PakGlobalData;
struct CommonIconAtlas {
    std::uint32_t textureHandle{};
    std::int32_t textureIndex = -1;
};

std::uint32_t LoadCommonIconSprites(PakGlobalData *package);
std::int64_t LoadLoadScreenSpriteTexture(PakGlobalData *package);
extern std::int32_t g_loadScreenSpriteTexture;
void ReleaseLoadScreenSpriteTexture();
void ReleaseCommonIconSprites();
CommonIconAtlas *GetCommonIconAtlas(std::uint16_t icon);
std::array<std::uint8_t, 4> GetCommonIconRect(std::uint16_t icon);
bool ReplaceCommonIconAtlasImage(std::uint16_t icon, std::string_view path);

enum class RenderIconSlot : std::int32_t {
    Any = 0,
    ItemSelect = 1,
    ActionSelector = 2,
    Qte = 3,
    MoneyMessage = 4,
    ItemEffectMessage = 5,
    SleepAction = 6,
    Unknown7 = 7,
    Count = 8,
};
enum class ECashOrCoins : std::int32_t {
    Coins = 0,
    Cash = 1,
    Cash2 = 2,
};

std::int64_t AddThisToCashOrSegaCoins(std::int32_t amount, ECashOrCoins mode) noexcept;
void CloseMoneyIcon() noexcept;
std::int64_t SetMoneyRelatedUIElement(std::int16_t maximum, std::int64_t unused, std::int16_t step) noexcept;
std::int64_t SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int64_t unused, std::int32_t step, std::int32_t flags) noexcept;
std::uint64_t GetNumBet() noexcept;
void SetMoneyHudCashAdjustment(std::int32_t amount) noexcept;
void KeepMoneyIconOpen() noexcept;
std::int64_t AbortMoneyIcon(std::int32_t force) noexcept;
std::int64_t AddCashOrCoinsWithStep(std::int32_t amount, std::int32_t mode, std::int32_t step) noexcept;
std::int64_t AnimateMoneyAmount(std::int32_t initial, std::int32_t target, std::int32_t modeIndex) noexcept;

bool GetShouldRenderIcons(RenderIconSlot slot);
bool ToggleRenderIcons(RenderIconSlot slot, bool enabled);
std::uint8_t GetParentMenu();
void iconToggleStateFor(std::uint8_t slot, bool enabled);
std::uint32_t GetItemSelectionMode(std::uint8_t slot);

std::uint32_t GetCurrentItemSelectState();
bool IsModeBitActive(std::int32_t mode);
bool IsAnyItemSelectActive();
std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled);
// IsItemSelectable: bit 5, owned by MEMO admission/retirement.
bool IsItemSelectable();
std::uint16_t GetPadControlData(std::uint8_t mode);
void padcRefreshCurrentInputs(bool bind);
std::int64_t RefreshPadControlData(std::uint32_t slot, std::int32_t force);

enum class CheckHelpIconOp : std::int32_t {
    Set = 0,
    Add = 1,
    Remove = 2,
    FrameOverlay = 3,
};

struct HelpIconState {
    std::uint32_t persistentVisible;
    std::uint32_t persistentSelected;
    std::uint32_t frameOverlayVisible;
    std::uint32_t frameOverlaySelected;
    std::uint32_t removalBlinkMask;
};
static_assert(sizeof(HelpIconState) == 0x14);
static_assert(offsetof(HelpIconState, persistentSelected) == 4);
static_assert(offsetof(HelpIconState, frameOverlayVisible) == 8);
static_assert(offsetof(HelpIconState, frameOverlaySelected) == 0xc);
static_assert(offsetof(HelpIconState, removalBlinkMask) == 0x10);

std::uint8_t SetPersistentHelpIconState(std::uint32_t visible, std::uint32_t selected, std::int32_t merge);
void CheckHelpIcon(std::uint32_t visible, std::uint32_t selected, CheckHelpIconOp operation);
std::uint32_t GetPersistentHelpIconFlags(std::uint32_t index);
const HelpIconState &GetHelpIconState();
std::int32_t GetHelpIconItemEffectState();
void doItemEffectStuff();
void RequestHelpIconItemPrompt();
// 1 shows the HUD notebook prompt's new-entry icon (3847) in place of the plain notebook (51).
void SetHudItemPromptIcon(std::int32_t enabled);
bool ToggleItemEffect(std::uint32_t itemId, std::int32_t amount);
bool CloseItemEffect(std::int32_t force);
bool SetSleepActionIcon(std::int16_t flags);
void ResetSleepActionIcon();
void ResumeSleepActionIconAfterSave();
std::uint64_t GetSleepActionIconFlags();
std::uint64_t GetSleepActionIconResult();
void ResetSleepActionDefaultState();
HLib::Task *CtrlSleepActionIcon(std::uint8_t nextFunctionIndex);
HLib::Task *GUI_Tasks_Enqueue();
bool OpenGameWatchMenuItem();
void CloseGameWatchMenuItem(std::int32_t immediate);
bool OpenSaveGameMenuItem();
void CloseSaveGameMenuItem();
std::uint32_t GetUiFlag(std::uint8_t mode);
void SetHelpIconMenuFlags(std::uint16_t flags);
void AddHelpIconMenuFlags(std::uint16_t flags);
void ResetHelpIconsToItemPrompt();
void set_icon_select_flags_to(std::uint32_t flags);
std::uint32_t GetIconSelectFlags() noexcept;
bool InitializeNativeGuiResources();
void FreeNativeGuiResources();
bool initSCNT_item_data();



void SetGameMessageContent(std::int32_t mode, std::int32_t value, std::string_view text);
void UpdateGameMessageMoney(std::int32_t value);
bool IsGameMessageVisible();
void SetGameMessageDelay(float seconds);
void ClearGameMessageMode();
HLib::Task *InitNowQte(std::uint8_t nextFunctionIndex);
std::int32_t GetQteControlState() noexcept;
void SetQteControlState(std::int32_t state) noexcept;
void SetQteCameraAtTop(std::int32_t mode) noexcept;
void qteSetupMaskFor90(std::uint32_t mask) noexcept;
std::uint32_t GetQteMaskFor90() noexcept;
void SetNowQteDrawEnabled(bool enabled) noexcept;
bool IsNowQteDrawEnabled() noexcept;

void SetShowQteFlag(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint16_t buttonFlags) noexcept;
void SetShowQteFlagForButtonType(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint32_t buttonType) noexcept;
std::int32_t GetCurrentQteStatus() noexcept;
bool GetIsQuickIconShowing() noexcept;
bool ResetQuickIconState(bool enabled) noexcept;
std::uint32_t GetQuickIconLoadingReleaseState() noexcept;
void SetQteButtonState(std::uint16_t mask) noexcept;
void SetQteMinigameModel(MT7Model *model) noexcept;
void SetQteMinigameTitleButtons(const std::uint32_t *buttons) noexcept;

std::int32_t SetupControlSpeakAndUI(std::int16_t *choices, std::int32_t timeout, std::int16_t defaultChoice, std::uint16_t mode);
std::int64_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t timeout, std::int16_t defaultChoice, std::uint16_t mode, std::int32_t style);
std::uint32_t shenGetCtrlSpeakIconFlag();
void speakSetFlagTo(std::uint16_t timeout);
bool IsCtrlSpeakIconInactive();
void HelpWidgetResetRuns();
void HelpWidgetParseLine(std::string_view line);
void HelpWidgetAddButtonIcon(std::int32_t code);
void FormatColoursForHelpTopic(std::uint8_t topic);
bool IsSpeakIconCancellable();
std::int64_t CloseSpeakIcon(std::int32_t force);
HLib::Task *InitSpeakIcon(HLib::Task *parent, std::uint8_t nextFunctionIndex);
void ResetMainIconState();
std::uint64_t IsMainIconInputReady();

std::uint64_t IsMainIconOverlayActive();

std::int32_t GetCurrentUIState();
void PreparePawnSetFlyer(std::int32_t index, std::int32_t inspection);
void PreparePawnItemFlyer(std::int32_t index);
void EnableRenderableByIndex(UIElement element);
extern std::array<UIElementState, static_cast<std::size_t>(UIElement::Count)> g_uiElements;
extern std::uint32_t g_nuklearTexture;

} // namespace shenmue::s2
