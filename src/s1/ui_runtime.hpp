#pragma once

#include "d3t/ui_definition.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "s1/d3t/namespace.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/hlib.hpp"
#include "s1/screen_space.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

struct nk_buffer;
struct nk_context;
struct nk_draw_null_texture;
struct nk_panel;

namespace shenmue::s1 {

constexpr std::int32_t kUiNotebookPages = 1;
constexpr std::int32_t kUiScreenFade = 2;
constexpr std::int32_t kUiGameMenu = 6;
constexpr std::int32_t kUiGameMenuBackground = 7;
constexpr std::int32_t kUiSharedItems = 8;
constexpr std::int32_t kUiSubMenuCassettes = 12;
constexpr std::int32_t kUiGameHudGauge = 35;
constexpr std::int32_t kUiActionSelector = 39;
constexpr std::int32_t kUiHelpNew = 76;





struct CreditsCell {
    std::uint32_t kind;
    std::uint32_t value;
};
struct CreditsRow {
    std::array<CreditsCell, 4> cells;
};
struct CreditsImage;
struct CreditsTable {
    CreditsRow *rows;
    std::uint32_t rowCount;
    std::uint32_t rowCapacity;
    CreditsImage *images;
    std::uint32_t imageCount;
    std::uint32_t imageCapacity;
    char *text;
    std::uint32_t textSize;
    std::uint32_t textCapacity;
    float height;
    std::uint8_t loaded;
    std::uint32_t contentHandle;
};
static_assert(sizeof(CreditsRow) == 0x20);
static_assert(offsetof(CreditsTable, text) == 0x20);
static_assert(offsetof(CreditsTable, height) == 0x30);
static_assert(offsetof(CreditsTable, loaded) == 0x34);
static_assert(offsetof(CreditsTable, contentHandle) == 0x38);
static_assert(sizeof(CreditsTable) == 0x40);
extern std::array<CreditsTable, 3> g_creditsTables;
extern float g_creditsScrollSpeed;
extern float g_creditsScrollProgress;

extern std::uint8_t g_previousHelpTokenId;
extern std::uint8_t g_defaultHelpTokenId;
extern std::uint8_t g_currentHelpTokenId;
extern std::uint8_t g_helpButtonId;
extern std::uint16_t g_currentHelpIconState;
extern std::int32_t g_optionsPendingAction;
std::int32_t ConfirmChanges();
std::int16_t AllocatePVRSurface(
    const char *text, std::int32_t width, std::uint16_t foreground, std::uint16_t background, std::int32_t mappingMode, bool centered);

struct PadCaptureEntry;
struct CVWKRuntimeSlot;
struct OwnedItemCatalogEntry;
struct PackModel;
struct Mt5RuntimeNode;



struct SleepSaveTrainingMenu {
    HLib::Task *task{};
    PadCaptureEntry *input{};
    PadCaptureEntry *previousInput{};
    std::uint8_t phase{};
    // 0 Sleep, 1 Save, 2 Training, 3 Options, 4 none.
    std::uint8_t selection{4};
    std::uint8_t active{};
    // Set by the SCNF request and by the Save, Training and Options owners when they hand control back.
    std::uint8_t requested{};
    // SCNF 13's result: 1 cancelled, 2 sleep.
    std::uint8_t scriptResult{};
    // Bit 0 allows cancelling; bit 1 hides Sleep and Training.
    std::int8_t flags{1};
    std::uint8_t highlightChoice{4};
    std::array<float, 4> choicePulse{1.0f, 1.0f, 1.0f, 1.0f};
    std::uint8_t returnHelpToken{};
};

extern SleepSaveTrainingMenu g_sleepSaveTrainingMenu;



struct GameTrainingElement {

    std::int32_t state{};

    std::int32_t focus{};

    std::int32_t animSlot{-1};

    float rootPanelStart{};

    std::int32_t languageImageOffset{};

    std::array<char, 512> resultText{};
    std::array<char, 512> focusTitle{};
};

extern GameTrainingElement g_gameTrainingElement;

std::uint32_t ParseGameTraining();
bool DrawGameTrainingPresentation(std::uint32_t renderTarget);




#pragma pack(push, 1)
struct InformationBarMessage {
    std::int32_t iconAction;
    std::int32_t outlinePanel;
    std::array<float, 4> outline;
    std::int32_t iconIndex;
    std::uint8_t noOutline;
    std::array<std::uint8_t, 3> reserved1d;
    std::array<std::byte, 0x20> iconImage;
    const char *text;
    std::uint8_t textLiteral;
    std::array<std::uint8_t, 7> reserved49;
    const char *leftText;
    std::uint8_t leftTextLiteral;
    std::array<std::uint8_t, 7> reserved59;
    const char *rightText;
    std::uint8_t rightTextLiteral;
    std::uint8_t colourOverride;
    std::array<std::uint8_t, 4> colour;
    std::array<std::uint8_t, 2> reserved6e;
};
#pragma pack(pop)
static_assert(sizeof(InformationBarMessage) == 0x70);
static_assert(offsetof(InformationBarMessage, iconIndex) == 0x18);
static_assert(offsetof(InformationBarMessage, text) == 0x40);
static_assert(offsetof(InformationBarMessage, leftText) == 0x50);
static_assert(offsetof(InformationBarMessage, rightText) == 0x60);
static_assert(offsetof(InformationBarMessage, colour) == 0x6a);

// The Information Bar label texts d3t_CopyStringToUILabel formats from the message (g_infoBarRightText, g_infoBarLeftText,
// g_infoBarSingleLine, g_infoBarFirstLine and g_infoBarSecondLine): the right and left texts, and the text on one line or split at the
// line break.
struct InformationBarLabels {
    InformationBarMessage message;
    std::array<char, 256> rightText;
    std::array<char, 256> leftText;
    std::array<char, 256> singleLine;
    std::array<char, 256> firstLine;
    std::array<char, 256> secondLine;


    float messageSeconds;
    InformationBarMessage savedMessage;
    std::array<char, 513> savedText;
};

extern InformationBarLabels g_informationBarLabels;
namespace d3t {
bool FormatVirtualKeyString(std::int32_t key, std::array<char, 64> &name);
void CopyStringToUILabel(const InformationBarMessage *message);
void ShowMessageStr(const InformationBarMessage *message, float seconds);
} // namespace d3t


struct FightMoveInfo {
    float progress;
    float newExperience;
    std::int32_t reserved08;
    std::uint8_t commandCount;
    std::array<std::uint8_t, 6> commands;
    std::uint8_t reserved13;
};
static_assert(sizeof(FightMoveInfo) == 0x14);
static_assert(offsetof(FightMoveInfo, commandCount) == 0x0c);

std::int32_t GetFightAnimInfos(std::int32_t move, char *name, FightMoveInfo *info);


using MovesScreenFightMoveCallback = void (*)(std::int32_t move);






struct MovesScreenElement {
    std::int32_t fightMoveCount;
    std::array<std::int32_t, 51> fightMoves;
    std::int32_t buttonAnimSlot;
    std::int32_t highlightAnimSlot;
    std::int32_t buttonAnimFrame;
    std::int32_t selectedMove;
    std::int32_t remapMove;
    std::uint32_t sceneTexture;
    std::uint32_t sceneTextureWidth;
    std::uint32_t sceneTextureHeight;
    bool sceneTextureBound;
    std::int32_t initialized;
    std::int32_t destroyRyoPending;
    MovesScreenFightMoveCallback adjustFightMove;
};

extern MovesScreenElement g_movesScreen;


struct SaveSlotInfo {
    std::int32_t valid;
    std::int32_t slotIndex;
    std::uint32_t areaId;
    std::int32_t reserved0c;
    std::array<char, 192> gameTime;
    std::array<char, 64> placeName;
    std::array<char, 128> saveTime;
};
static_assert(sizeof(SaveSlotInfo) == 400);

extern std::array<SaveSlotInfo, 10> g_saveSlotInfo;

void GenerateLoadingScreenText(
    char *destination, std::size_t size, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t dayOfWeek, std::uint8_t mode);
void CreateStringForSaveDataMenuLabel(char *destination, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t dayOfWeek, std::int32_t hour,
    std::int32_t minute, std::int32_t second, std::uint8_t mode);
const char *GetIDForAreaID(std::uint32_t areaId);
void CreateSaveDataInfoForMenu(std::int32_t loadType);
std::int64_t TryInvokeRegisteredCallback(std::int32_t elementIndex);


extern std::int32_t g_uiDebugElementDispatch;


extern std::int32_t g_uiDebugSuspendElements;
extern std::int32_t g_notebookDebugMode;

extern std::uint8_t g_debugUnknown0;
std::int32_t GetKeyForInputType(std::int32_t action, std::int32_t inputType);
std::int32_t ResolvePressedKey(std::int32_t action, std::int32_t inputType, std::int32_t doubleWidth);
std::int32_t ResolveButtonIconGlyph(std::int32_t action, std::int32_t controlMethod, std::int32_t inputType, std::int32_t doubleWidth);


extern std::array<std::uint32_t, 3> g_uiRegisteredCallbackRequests;
extern std::array<std::uint32_t, 3> g_uiPreviousInputElements;

extern std::array<std::uint32_t, 3> g_uiRefreshedElements;
extern std::array<std::uint32_t, 3> g_uiPreviousRefreshedElements;



extern std::int32_t g_iIsInGame;
extern std::int32_t g_isInGame;
void UpdateInGameStatus();

constexpr std::int32_t kUiLoadSave = 0x37;

struct UIPanelDefinition;
struct UIObjectDefinition;

enum class UIIconNotificationState : std::uint8_t {
    Idle = 0,
    FadeIn = 1,
    OpenRequested = 2,
    Visible = 3,
    CloseRequested = 4,
    FadeOut = 5,
};

enum class UIIconPromptKind : std::uint8_t {
    None = 0,
    Button = 1,
    Directional = 2,
};

enum class UIHelpWidgetState : std::uint8_t {
    Idle = 0,
    PreparePage = 1,
    ForceClose = 2,
    Active = 3,
    RestorePrevious = 4,
    ForceOpen = 5,
    ResetToIdle = 7,
};
extern std::uint8_t g_helpOverlayPageTokenId;
extern UIHelpWidgetState g_helpOverlayState;

enum class UIIconSystemMenuState : std::uint8_t {
    Closed = 0,
    Unused1 = 1,
    OpenRequested = 2,
    OpeningTransition = 3,
    Visible = 4,
    ClosingTeardown = 5,
    CloseRequested = 6,
    ClosingTransition = 7,
};

enum class UIIconSystemMenuSelection : std::uint8_t {
    Resume = 0,
    Settings = 1,
    Moves = 2,
    Notebook = 3,
    Collection = 4,
    Cassettes = 5,
    Tokens = 6,
    Date = 7,
    Items = 8,
};

enum class UIActionSelectorState : std::uint8_t {
    Idle = 0,
    Opening = 1,
    OpenRequested = 2,
    Active = 3,
    SelectionCommitted = 4,
    Closing = 5,
};

struct UIActionSelectorRequest {
    std::int16_t up;
    std::int16_t left;
    std::int16_t right;
    std::int16_t down;
    std::uint16_t timeoutFrames;
    std::int16_t timeoutHelpToken;
    std::int16_t mode;
};



struct TimedHelpSelectorChoice {
    std::int32_t enabled;
    std::uint32_t mapId;
    std::int16_t helpToken;
    std::uint16_t entryId;
    std::uint16_t scenarioBand;
    std::uint8_t schedule[8];
    std::uint16_t scheduleWord;
};
static_assert(sizeof(TimedHelpSelectorChoice) == 0x18);
static_assert(offsetof(TimedHelpSelectorChoice, schedule) == 0x0e);

enum class UICashOverlayState : std::uint8_t {
    Idle = 0,
    WaitOpenAnimation = 1,
    OpenRequested = 2,
    DisplayActive = 3,
    CloseRequested = 4,
    WaitCloseAnimation = 5,
};

enum class UICashOverlayDisplayMode : std::uint8_t {
    None = 0,
    ManualCurrentBalance = 1,
    BalanceDeltaAutoClose = 2,
};

// The answer d3t_MessageBox's callback receives: the selection (No or Yes), a timed box running out, or B.
enum class UIRetry70ManBattlePromptResult : std::int32_t {
    No = 1,
    Yes = 2,
    Timeout = 3,
    Cancel = 4,
};

struct MFBTSurface {
    std::int32_t surface{-1};
    std::int32_t width{};
    std::int32_t height{};
};
static_assert(sizeof(MFBTSurface) == 12);

inline constexpr std::array<const char *, 6> kSeventyManPanelIds{
    "Panel_StartPrompt_Overlay", "Panel_StartPrompt", "Panel_BodyCount", "Panel_GameTime", "Panel_YourRank", "Panel_ScoreTable"};
inline constexpr std::array<const char *, 8> kSeventyManLabelIds{"Label_BodyCount", "Label_GameTime", "Label_YourRank", "Label_ScoreTable0",
    "Label_ScoreTable1", "Label_ScoreTable2", "Label_ScoreTable3", "Label_ScoreTable4"};
inline constexpr std::array<std::array<std::uint8_t, 6>, 5> kSeventyManPanelVisibility{
    {{0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 1}, {1, 1, 0, 0, 0, 0}}};
// Game QTE in the 80-entry element table: ICQu enables it only while the QTE prompt is in its shown flash phase.
inline constexpr std::int32_t kUiGameQte = 52;


inline constexpr std::int32_t kUi70ManBattle = 64;
inline constexpr std::array<const char *, 8> k70mbStringIds{"UI70MBDEF0", "UI70MBLOSS", "UI70MBWIN1", "UI70MBWIN2", "UI70MBWIN3", "UI70MBWIN4", // @text UI70MBDEF0 EN: "Defeated" | JA: "人抜き" ; UI70MBLOSS EN: "You did not rank in the leaderboard" | JA: "ランキングにエントリーできませんでした" ; UI70MBWIN1 EN: "Your rank is 1st" | JA: "１位をゲット" ; UI70MBWIN2 EN: "Your rank is 2nd" | JA: "２位をゲット" ; UI70MBWIN3 EN: "Your rank is 3rd" | JA: "３位をゲット" ; UI70MBWIN4 EN: "Your rank is 4th" | JA: "４位をゲット"
    "UI70MBWIN5", "UI70MBRETY"}; // @text UI70MBWIN5 EN: "Your rank is 5th" | JA: "５位をゲット" ; UI70MBRETY EN: "Do you wish to retry?" | JA: "もう一度チャレンジしますか？"
extern std::array<std::array<char, 256>, 8> g_70mbLabelText;
extern std::int32_t g_70mbShownIconStatus;
extern std::int32_t g_70mbStartPromptIconVisible;
void FormatTop5TimesText();
inline constexpr std::array<std::int32_t, 13> kMFBTSurfaceWidths{256, 32, 256, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
inline constexpr std::array<std::array<std::uint16_t, 3>, 6> kMFBTElapsedDigitLayout{
    {{18, 0, 32}, {46, 0, 32}, {100, 0, 32}, {132, 0, 32}, {178, 8, 16}, {194, 8, 16}}};
inline constexpr std::array<std::uint32_t, 6> kMFBTElapsedDigitRemainders{1800000, 180000, 30000, 3000, 300, 30};
inline constexpr shen_name kMFBTDigitAllocationToken{SHEN_NAME(LDTD)};
inline constexpr shen_name kVfPromptSpriteAllocationToken{SHEN_NAME(HLP0)};

using UIRetry70ManBattlePromptCallback = void (*)(UIRetry70ManBattlePromptResult result);

enum class UISicnPausePolicy : std::int32_t {
    ClearOnPause = 0,
    KeepOnPause = 1,
    Free = 2,
};

enum class UICdttMode : std::int32_t {
    None = 0,
    SkipLoadScreenRender = 1,
    Unknown2 = 2,
    Unknown3 = 3,
    TitleOrIntroDateSetup = 4,
    GameplayDateSetup = 5,
    SkipLoadScreenRenderAlt = 6,
    SpecialDateLoadScreen = 7,
    HideLoadingText = 8,
};
struct UICdttPresentationPayload {
    std::uint32_t areaId;
    std::int32_t locationIndex;
    std::int32_t specialDateIndex;
    std::int32_t year;
    std::int32_t month;
    std::int32_t day;
    std::int32_t dayOfWeek;
    std::int32_t hours;
    std::int32_t minutes;
    char headerFilename[16];
};

struct UICdttPresentationText {
    char date[64];
    char time[32];
    char location[128];
    char subLocation[128];
    char headerPath[96];
    std::int16_t locationIndex;
    std::uint8_t specialDateIndex;
    std::uint8_t loadingTextVisible;
};

enum class UISfxOverlayState : std::uint8_t {
    Idle = 0,
    Opening = 1,
    Requested = 2,
    Active = 3,
    Resolved = 4,
    Closing = 5,
};

extern UISfxOverlayState g_sfxOverlayState;

struct UISfxOverlayRequest {
    std::uint16_t expectedMask;
    std::int16_t timeoutFrames;
    std::uint8_t mode;
    std::uint8_t bonusFrames;
};

enum class UISfxOverlayTimeoutTuningMode : std::int32_t {
    Session = 0,
    DisableBoost = 1,
    SetStepFrames = 2,
    SetPendingBonusFrames = 3,
    SetMaximumFrames = 4,
};

enum class UISfxOverlayTimeoutSessionAction : std::uint8_t {
    Begin = 0,
    Advance = 1,
    Reset = 2,
};

struct UISfxOverlayTimeoutTuningParameters {
    std::int32_t value;
    UISfxOverlayTimeoutTuningMode mode;
};

struct UISfxOverlaySnapshot {
    UISfxOverlayState state;
    std::int16_t timeoutFrames;
    std::int16_t elapsedFrames;
    std::int16_t result;
    std::int16_t pendingBonusFrames;
    std::int16_t maximumFrames;
    std::int16_t stepFrames;
    std::int16_t accumulatedFrames;
    std::int32_t boostCount;
    std::uint8_t boostEnabled;
    std::uint8_t boostSessionActive;
};

struct UIPresentationSnapshot {
    std::uint8_t helpToken;
    std::uint8_t helpButton;
    std::uint8_t helpVisible;
    std::uint8_t helpClosing;
    std::int16_t helpAlpha;
    UIIconNotificationState iconState;
    UIIconPromptKind iconKind;
    std::uint8_t iconExpectedInput;
    std::int16_t iconAlpha;
    std::uint16_t iconOwnerMask;
    std::uint16_t sicnState;
    std::int16_t sicnTimeout;
    UISicnPausePolicy sicnPausePolicy;
    UICdttMode cdttMode;
    std::int16_t cdttAlpha;
    UISfxOverlaySnapshot sfxOverlay;
    UIHelpWidgetState helpWidgetState;
    UIIconSystemMenuState iconSystemMenuState;
    UIIconSystemMenuSelection iconSystemMenuSelection;
    std::uint8_t iconSystemMenuVisible;
    std::uint8_t cdttLoadingTextVisible;
};

static_assert(sizeof(UIActionSelectorRequest) == 14);

enum class UIGameMessageContentType : std::uint32_t {
    Item = 0,
    Cassette = 1,
    Money = 2,
    Token = 3,
};


struct UIActionSelectorElement {
    std::int32_t centerIconAnimSlot{-1};
    std::int32_t selectedChoiceAnimSlot{-1};
    std::int32_t highlightAnimSlot{-1};
};



struct UIGridEntry {
    std::int32_t type;
    std::int32_t id;
    std::int32_t count;
    std::array<std::int32_t, 3> reserved0c;
    std::int32_t drawnCell;
    std::int32_t reserved1c;
};
static_assert(sizeof(UIGridEntry) == 0x20);
static_assert(offsetof(UIGridEntry, drawnCell) == 0x18);



struct HelpNewRun {
    std::int32_t kind;
    std::int32_t line;
    std::int32_t column;
    std::array<char, 128> text;
    std::int32_t length;
    std::int32_t icon;
    UILayoutRect rect;
};
static_assert(sizeof(HelpNewRun) == 0xa4);
static_assert(offsetof(HelpNewRun, rect) == 0x94);



struct HelpNewInfoBlock {
    std::uint32_t colour;
    std::int32_t column;
    std::int32_t line;
    std::int32_t lines;
    std::int32_t kind;
    UILayoutRect rect;
};
static_assert(sizeof(HelpNewInfoBlock) == 0x24);


struct HelpIconRecord {
    std::int16_t id;
    std::uint8_t line;
    std::uint8_t column;
};
static_assert(sizeof(HelpIconRecord) == 4);



struct HelpNewPageBlocks {
    std::int32_t count;
    std::array<std::array<std::int8_t, 5>, 4> blocks;
};
constexpr std::array<HelpNewPageBlocks, 68> kHelpNewPageBlocks{{
    {.count = 4, .blocks = {{{3, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {0, 1, 6, 1, 2}, {2, 2, 6, 1, 1}}}},
    {.count = 2, .blocks = {{{2, 1, 6, 1, 1}, {1, 2, 6, 1, 1}}}},
    {.count = 2, .blocks = {{{2, 1, 5, 1, 2}, {1, 2, 5, 1, 3}}}},
    {.count = 1, .blocks = {{{2, 1, 7, 1, 0}}}},
    {.count = 1, .blocks = {{{2, 2, 6, 0, 0}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 2, .blocks = {{{2, 1, 7, 1, 0}, {1, 2, 7, 1, 3}}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 0}}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 2}}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 2}}}},
    {.count = 1, .blocks = {{{2, 2, 3, 0, 3}}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 1}}}},
    {.count = 2, .blocks = {{{0, 1, 6, 1, 0}, {2, 2, 6, 1, 1}}}},
    {.count = 3, .blocks = {{{0, 1, 7, 1, 3}, {1, 2, 7, 1, 1}, {3, 2, 10, 1, 0}}}},
    {.count = 2, .blocks = {{{0, 1, 6, 1, 0}, {2, 2, 6, 1, 3}}}},
    {.count = 3, .blocks = {{{0, 1, 6, 1, 2}, {2, 2, 6, 1, 3}, {3, 1, 10, 1, 0}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 1}}}},
    {.count = 2, .blocks = {{{1, 1, 8, 1, 2}, {2, 2, 8, 1, 0}}}},
    {.count = 2, .blocks = {{{0, 1, 6, 1, 2}, {2, 2, 6, 1, 0}}}},
    {.count = 3, .blocks = {{{0, 1, 4, 1, 2}, {2, 2, 4, 1, 3}, {3, 1, 8, 1, 0}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 1, .blocks = {{{0, 1, 7, 1, 2}}}},
    {.count = 4, .blocks = {{{0, 1, 2, 1, 2}, {1, 2, 2, 1, 1}, {3, 1, 6, 1, 1}, {2, 2, 6, 1, 1}}}},
    {.count = 2, .blocks = {{{3, 1, 7, 1, 0}, {2, 2, 7, 1, 0}}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 1}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 1, .blocks = {{{2, 2, 6, 0, 0}}}},
    {.count = 2, .blocks = {{{2, 2, 9, 1, 1}, {0, 1, 9, 1, 1}}}},
    {.count = 2, .blocks = {{{2, 1, 6, 1, 2}, {1, 2, 6, 1, 2}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 2, .blocks = {{{0, 1, 4, 1, 0}, {2, 2, 4, 1, 1}}}},
    {.count = 2, .blocks = {{{0, 1, 4, 1, 0}, {2, 2, 4, 1, 1}}}},
    {.count = 2, .blocks = {{{0, 1, 4, 1, 0}, {2, 2, 4, 1, 1}}}},
    {.count = 2, .blocks = {{{0, 1, 4, 1, 0}, {2, 2, 4, 1, 1}}}},
    {.count = 1, .blocks = {{{2, 2, 6, 0, 0}}}},
    {.count = 1, .blocks = {{{0, 1, 7, 1, 0}}}},
    {.count = 1, .blocks = {{{2, 1, 8, 1, 0}}}},
    {.count = 1, .blocks = {{{2, 2, 8, 0, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 1, .blocks = {{{2, 1, 7, 2, 0}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 0, .blocks = {{}}},
    {.count = 0, .blocks = {{}}},
    {.count = 1, .blocks = {{{2, 2, 7, 0, 3}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 3, 1, 1}, {1, 2, 3, 1, 1}, {3, 2, 6, 1, 0}, {2, 1, 6, 1, 0}}}},
    {.count = 3, .blocks = {{{0, 1, 7, 1, 3}, {1, 2, 7, 1, 1}, {3, 2, 10, 1, 0}}}},
    {.count = 1, .blocks = {{{0, 2, 6, 2, 1}}}},
    {.count = 1, .blocks = {{{0, 2, 6, 0, 1}}}},
    {.count = 1, .blocks = {{{2, 2, 6, 0, 2}}}},
    {.count = 3, .blocks = {{{0, 1, 5, 1, 2}, {2, 2, 5, 1, 3}, {3, 1, 9, 1, 0}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 0, .blocks = {{}}},
    {.count = 2, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 0}}}},
    {.count = 2, .blocks = {{{0, 1, 8, 1, 0}, {2, 2, 8, 1, 0}}}},
    {.count = 4, .blocks = {{{0, 1, 7, 1, 1}, {2, 1, 10, 1, 0}, {1, 2, 7, 1, 1}, {3, 2, 10, 1, 0}}}},
    {.count = 3, .blocks = {{{0, 1, 7, 1, 0}, {2, 2, 7, 1, 0}, {3, 2, 9, 1, 0}}}},
    {.count = 3, .blocks = {{{0, 1, 6, 1, 0}, {2, 2, 6, 1, 0}, {3, 2, 8, 1, 0}}}},
    {.count = 1, .blocks = {{{2, 1, 8, 1, 0}}}},
    {.count = 3, .blocks = {{{0, 1, 6, 1, 0}, {2, 2, 6, 1, 0}, {3, 2, 8, 1, 0}}}},
    {.count = 0, .blocks = {{}}},
    {.count = 0, .blocks = {{}}},
    {.count = 4, .blocks = {{{0, 1, 4, 1, 2}, {1, 2, 4, 1, 0}, {3, 1, 8, 1, 0}, {2, 2, 8, 1, 0}}}},
}};


constexpr std::array<std::uint32_t, 4> kHelpNewFrameColours{0xff00008cU, 0xff8c0000U, 0xff008c00U, 0xff008c8cU};


constexpr std::array<const char *, 2> kHelpNewWindowIds{"Help_BackgroundPanel", "Help_MainPanel"};





struct UIGridStyle {
    float spacing;
    float holderPadding;
    alignas(8) std::array<std::byte, 0x28> fixedBackground;
    std::uint32_t cellColour;
    float field34;
    float scrollBarGap;
    float scrollBarWidth;
    float trackRounding;
    float rounding;
    float border;
    std::uint32_t trackColour;
    std::uint32_t trackLineColour;
    std::uint32_t trackBorderColour;
    std::uint32_t thumbColour;
    std::uint32_t thumbLineColour;
    std::uint32_t thumbCentreColour;
    std::uint32_t textColour;
    std::int32_t fontSize;
    std::int32_t reserved6C;
};
static_assert(sizeof(UIGridStyle) == 0x70);
static_assert(offsetof(UIGridStyle, cellColour) == 0x30 && offsetof(UIGridStyle, trackColour) == 0x4c && offsetof(UIGridStyle, fontSize) == 0x68);


inline constexpr UIGridStyle kUIGridFrontEndStyle{.spacing = 8.0f,
    .holderPadding = 25.0f,
    .fixedBackground = {},
    .cellColour = 0,
    .field34 = 10.0f,
    .scrollBarGap = 25.0f / 1080.0f,
    .scrollBarWidth = 28.0f / 1080.0f,
    .trackRounding = 8.0f,
    .rounding = 6.0f,
    .border = 3.0f,
    .trackColour = 0xff2f2f2fU,
    .trackLineColour = 0x0aefe3f7U,
    .trackBorderColour = 0xaaa4a294U,
    .thumbColour = 0xffa4a294U,
    .thumbLineColour = 0x23efe3f7U,
    .thumbCentreColour = 0x50502328U,
    .textColour = 0xffffffffU,
    .fontSize = 4,
    .reserved6C = 0};
inline constexpr UIGridStyle kUIGridInGameStyle{.spacing = 8.0f,
    .holderPadding = 25.0f,
    .fixedBackground = {},
    .cellColour = 0,
    .field34 = 10.0f,
    .scrollBarGap = 25.0f / 1080.0f,
    .scrollBarWidth = 28.0f / 1080.0f,
    .trackRounding = 8.0f,
    .rounding = 6.0f,
    .border = 3.0f,
    .trackColour = 0xff140f0cU,
    .trackLineColour = 0x03c8ebf0U,
    .trackBorderColour = 0x645b5548U,
    .thumbColour = 0xff5b5548U,
    .thumbLineColour = 0x0ac8ebf0U,
    .thumbCentreColour = 0x5a502328U,
    .textColour = 0xffffffffU,
    .fontSize = 4,
    .reserved6C = 0};

struct UIGrid;
// A grid's cell callback (+0x30): the grid, the shared grid panel, the holder's rect, the cell's rect in the holder, the cell spacing, the
// element index the grid draws for, the grid's elements (null past the element count) and the cell's index.
using UIGridDrawCell = void (*)(UIGrid *grid, nk_panel *panel, const UILayoutRect *holder, const UILayoutRect *cell, float spacing, std::int32_t element,
    void *elements, std::int32_t index);
// A grid's element accessor (+0x60) and per-row height in rows (+0x68).
using UIGridGetElement = void *(*)(void *elements, std::int32_t index);
using UIGridRowHeight = std::int32_t (*)(UIGrid *grid, std::int32_t index);







struct UIGrid {
    std::int32_t columns{1};
    std::int32_t visibleRows{1};
    std::int32_t rows{1};
    std::uint8_t showEmptyCells{1};
    std::uint8_t wrap{};
    std::uint8_t pagedScroll{};
    std::uint8_t reserved0F{};
    const UIGridStyle *style{};
    d3t::UIWindow *holder{};
    UILayoutRect rect{};
    UIGridDrawCell drawCell{};
    d3t::UIMouseCallbacks mouseCallbacks{};
    std::int32_t reserved5C{};
    UIGridGetElement getElement{};
    UIGridRowHeight rowHeight{};
    std::int32_t navigateSound{3};
    std::int32_t reserved74{};
    std::int32_t elementCount{};
    std::int32_t reserved7C{};
    void *elements{};
    std::int32_t animSlot{-1};
    float scrollSpeed{};
    std::int32_t dragging{};
    std::int32_t smoothScroll{};
    float dragOffset{};
    std::int32_t scrollRow{};
    float scrollOffset{};
    std::int32_t fieldA4{};
    std::int32_t selectedRow{};
    std::int32_t selectedColumn{};
    std::int32_t previousScrollRow{-1};
    std::int32_t previousSelectedRow{-1};
    std::int32_t previousSelectedColumn{-1};
    float dragStart{};
    float dragDelta{};
    std::int32_t confirmRequested{};
    std::int32_t changed{};
    std::int32_t hovered{};
};
static_assert(sizeof(UIGrid) == 0xd0);
static_assert(offsetof(UIGrid, mouseCallbacks) == 0x38 && offsetof(UIGrid, getElement) == 0x60 && offsetof(UIGrid, elementCount) == 0x78);
static_assert(offsetof(UIGrid, animSlot) == 0x88 && offsetof(UIGrid, scrollRow) == 0x9c && offsetof(UIGrid, confirmRequested) == 0xc4);





constexpr std::size_t kUIGridTemplateCapacity = 52;
struct UIGridTemplate {
    std::int32_t initialized;
    std::int32_t viewSlot;
    std::int32_t first;
    std::int32_t end;
    std::array<std::int32_t, kUIGridTemplateCapacity> clipWindows;
    d3t::UIWindow **windows;
    const char *const *ids;
    std::int32_t layoutFirst;
    std::int32_t layoutEnd;
    d3t::UILayoutItem **layoutItems;
    std::int32_t labelFirst;
    std::int32_t labelEnd;
    d3t::UILabelWidget **labels;
    std::array<std::int32_t, kUIGridTemplateCapacity> idTemplatePositions;
    std::array<std::int32_t, kUIGridTemplateCapacity> parentTemplatePositions;
    std::int32_t rootCount;
    std::array<d3t::UIWindowLayout *, kUIGridTemplateCapacity> roots;
    std::int32_t clipCount;
    std::array<d3t::UIWindowLayout *, kUIGridTemplateCapacity> clips;
};
static_assert(sizeof(UIGridTemplate) == 0x600);
static_assert(offsetof(UIGridTemplate, windows) == 0xe0 && offsetof(UIGridTemplate, idTemplatePositions) == 0x110);
static_assert(offsetof(UIGridTemplate, rootCount) == 0x2b0 && offsetof(UIGridTemplate, clipCount) == 0x458);


inline constexpr std::array<const char *, 5> kItemsGridWindowIds{"SubMenu_Items::Panel_GridHolder", "SubMenu_Items::GridElementParent_#",
    "SubMenu_Items::GridElement_#", "SubMenu_Items::GridElement_Texture_#", "SubMenu_Items::GridElement_Shadow_#"};
inline constexpr std::array<const char *, 2> kItemsGridLayoutItemIds{
    "SubMenu_Items::GridElementParent_Background_Spacepush", "SubMenu_Items::GridElementBackground_Game_Shading"};




inline constexpr std::array<const char *, 25> kMovesWindowIds{"GameMoves::Panel_Root", "GameMoves::RightPanel", "GameMoves::PowerBar_Fill",
    "GameMoves::SpeedBar_Fill", "GameMoves::RigidBar_Fill", "GameMoves::DummyMove_#", "GameMoves::DummyMove_BarProgress_#", "GameMoves::DummyMove_BarNewExp_#",
    "GameMoves::DummyMove_Bar_#", "GameMoves::DummyMove_BarTexture_#", "GameMoves::DummyMove_ArrowLeft_#", "GameMoves::DummyMove_ArrowRight_#",
    "GameMoves::DummyMove_Bar_Label_#", "GameMoves::DummyMove_Button_01_#", "GameMoves::DummyMove_Button_02_#", "GameMoves::DummyMove_Button_03_#",
    "GameMoves::DummyMove_Button_04_#", "GameMoves::DummyMove_Button_05_#", "GameMoves::DummyMove_Button_06_#", "GameMoves::DummyMove_Plus_01_#",
    "GameMoves::DummyMove_Plus_02_#", "GameMoves::DummyMove_Plus_03_#", "GameMoves::DummyMove_Plus_04_#", "GameMoves::DummyMove_Plus_05_#",
    "GameMoves::DummyMove_Assign_#"};
inline constexpr std::array<const char *, 2> kMovesLabelIds{"GameMoves::DummyMoveNameLabel_Shadow", "GameMoves::DummyMoveNameLabel"};
inline constexpr std::array<const char *, 5> kMovesColourImageIds{
    "GameMoves::MainTexture_Game", "GameMoves::MainTexture_Front", "GameMoves::BarTexture_Game", "GameMoves::Ryo", "GameMoves::XBase"};
inline constexpr std::array<const char *, 28> kMovesLayoutItemIds{"GameMoves::DummyMove_Spacepush_#", "GameMoves::DummyMove_Padded_Spacepush_#",
    "GameMoves::DummyMove_BarProgress_Spacepush_#", "GameMoves::DummyMove_BarNewExp_Spacepush_#", "GameMoves::DummyMove_BarBackground_Spacepush_#",
    "GameMoves::DummyMove_Bar_Spacepush_#", "GameMoves::DummyMove_BarShadow_Spacepush_#", "GameMoves::DummyMove_BarShadow2_Spacepush_#",
    "GameMoves::DummyMove_LabelShadow_Spacepush_#", "GameMoves::DummyMove_Label_Spacepush_#", "GameMoves::DummyMove_Button_01_Spacepush_#",
    "GameMoves::DummyMove_Button_02_Spacepush_#", "GameMoves::DummyMove_Button_03_Spacepush_#", "GameMoves::DummyMove_Button_04_Spacepush_#",
    "GameMoves::DummyMove_Button_05_Spacepush_#", "GameMoves::DummyMove_Button_06_Spacepush_#", "GameMoves::DummyMove_Plus_01_Spacepush_#",
    "GameMoves::DummyMove_Plus_02_Spacepush_#", "GameMoves::DummyMove_Plus_03_Spacepush_#", "GameMoves::DummyMove_Plus_04_Spacepush_#",
    "GameMoves::DummyMove_Plus_05_Spacepush_#", "GameMoves::DummyMove_Assign_Spacepush_#", "GameMoves::DummyMove_ArrowLeftShadow_Spacepush",
    "GameMoves::DummyMove_ArrowLeftLight_Spacepush", "GameMoves::DummyMove_ArrowLeftMain_Spacepush", "GameMoves::DummyMove_ArrowRightShadow_Spacepush",
    "GameMoves::DummyMove_ArrowRightLight_Spacepush", "GameMoves::DummyMove_ArrowRightMain_Spacepush"};
inline constexpr std::array<const char *, 7> kMovesButtonIconIds{"GameMoves::FightAssignedMove", "GameMoves::DummyMoveButton_01",
    "GameMoves::DummyMoveButton_02", "GameMoves::DummyMoveButton_03", "GameMoves::DummyMoveButton_04", "GameMoves::DummyMoveButton_05",
    "GameMoves::DummyMoveButton_06"};
inline constexpr std::array<const char *, 8> kMovesButtonIds{"GameMoves::MainPanelButton", "GameMoves::DummyMoveBar_Background",
    "GameMoves::DummyMoveBar_BarProgress", "GameMoves::DummyMoveBar_BarNewExp", "GameMoves::DummyMoveBar_Bar", "GameMoves::PowerBar_Background",
    "GameMoves::SpeedBar_Background", "GameMoves::RigidBar_Background"};




struct UIFocusList {
    std::int32_t focused;          // +0x00, -1 while the content has the pad
    std::int32_t lastFocused;
    std::int32_t selected;
    std::int32_t previousSelected;
    std::uint8_t hasFocus;
    std::uint8_t previousHasFocus;
    std::array<std::uint8_t, 2> reserved12;
    std::int32_t first;
    std::int32_t last;
    std::int32_t reserved1c;
    d3t::UIWindow *const *windows;
    d3t::UILabelWidget *const *labels;
    std::uint8_t initialized;
    std::array<std::uint8_t, 7> reserved31;
    std::int32_t shownIndex;
};
static_assert(offsetof(UIFocusList, hasFocus) == 0x10);
static_assert(offsetof(UIFocusList, first) == 0x14);
static_assert(offsetof(UIFocusList, windows) == 0x20);
static_assert(offsetof(UIFocusList, initialized) == 0x30);
static_assert(offsetof(UIFocusList, shownIndex) == 0x38);



struct UIFocusListColours {
    std::uint32_t selected;
    std::uint32_t normal;
    std::uint32_t focused;
};
inline constexpr UIFocusListColours kUIFocusListFrontEndColours{.selected = 0xffffffffU, .normal = 0x40ffffffU, .focused = 0xffe6d2ffU};
inline constexpr UIFocusListColours kUIFocusListInGameColours{.selected = 0xffffffffU, .normal = 0x40ffffffU, .focused = 0xffffffffU};



inline constexpr std::array<const char *, 3> kCollectionWindowIds{
    "SubMenu_Collection::Tab1Button", "SubMenu_Collection::Tab2Button", "SubMenu_Collection::Tab3Button"};
inline constexpr std::array<const char *, 4> kCollectionLabelIds{
    "SubMenu_Collection::Label_Tab_1", "SubMenu_Collection::Label_Tab_2", "SubMenu_Collection::Label_Tab_3", nullptr};
inline constexpr std::array<const char *, 7> kCollectionColourImageIds{"SubMenu_Collection::Image_Tab_1", "SubMenu_Collection::Image_Tab_2",
    "SubMenu_Collection::Image_Tab_3", "SubMenu_Collection::Texture_Alpha_1", "SubMenu_Collection::Texture_Alpha_2", "SubMenu_Collection::Texture_Alpha_3",
    "SubMenu_Collection::Texture_Alpha_4"};
inline constexpr std::array<const char *, 3> kCollectionButtonIds{
    "SubMenu_Collection::Button_Tab_1", "SubMenu_Collection::Button_Tab_2", "SubMenu_Collection::Button_Tab_3"};


inline constexpr std::uint32_t kCollectionTabButtonColour = 0x7f322b1cU;
inline constexpr std::uint32_t kCollectionTabButtonSelectedColour = 0xff322b1cU;
inline constexpr std::uint32_t kCollectionTabBackgroundColour = 0x00ffffffU;
inline constexpr std::uint32_t kCollectionTabBackgroundSelectedColour = 0xa0ffffffU;



inline constexpr std::array<const char *, 6> kCassettesWindowIds{"Submenu_Collection_Cassettes::Panel_GridHolder", "Submenu_Collection::Cassettes_Left",
    "Submenu_Collection_Cassettes::GridElementParent_#", "Submenu_Collection_Cassettes::GridElement_#", "Submenu_Collection_Cassettes::GridElement_Texture_#",
    "Submenu_Collection_Cassettes::GridElement_Shadow_#"};
inline constexpr std::array<const char *, 2> kCassettesLayoutItemIds{
    "Submenu_Collection_Cassettes::GridElementParent_Background_Spacepush", "Submenu_Collection_Cassettes::GridElementBackground_Game_Shading"};

inline constexpr std::array<const char *, 5> kGachaWindowIds{"Submenu_Collection_Gacha_Panel_GridHolder", "Submenu_Collection_Gacha::GridElementParent_#",
    "Submenu_Collection_Gacha::GridElement_#", "Submenu_Collection_Gacha::GridElement_Texture_#", "Submenu_Collection_Gacha::GridElement_Shadow_#"};
inline constexpr std::array<const char *, 2> kGachaLayoutItemIds{
    "Submenu_Collection_Gacha::GridElementParent_Background_Spacepush", "Submenu_Collection_Gacha::GridElementBackground_Game_Shading"};

inline constexpr std::array<const char *, 5> kScrollsWindowIds{"Submenu_Collection_Scrolls_Holder", "Submenu_Collection_Scrolls::GridElementParent_#",
    "Submenu_Collection_Scrolls::GridElement_#", "Submenu_Collection_Scrolls::GridElement_Texture_#", "Submenu_Collection_Scrolls::GridElement_Shadow_#"};
inline constexpr std::array<const char *, 2> kScrollsLayoutItemIds{
    "Submenu_Collection_Scrolls::GridElementParent_Background_Spacepush", "Submenu_Collection_Scrolls::GridElementBackground_Game_Shading"};







enum class UIOptionType : std::int32_t {
    Toggle = 0,
    List = 1,
    Slider = 2,
    KeyBinding = 3,
    Heading = 4,
};

struct UIOption {
    UIOptionType type;
    std::int32_t flag04;
    const char *name;
    const char *(*text)(std::int32_t choice);
    std::int32_t current;
    float value;
    const char *(*choice)(std::int32_t index);
    std::int32_t (*choiceValue)(std::int32_t index);
    std::int32_t localizeChoices;
    std::int32_t flag34;
    std::int32_t (*getInt)();
    float (*getFloat)();
    void (*setInt)(std::int32_t value);
    void (*setFloat)(float value);
    std::int32_t (*available)();
    std::int32_t cut;
    float dragX;
    std::int32_t dragging;
    std::int32_t reserved6C;
};
static_assert(sizeof(UIOption) == 0x70);
static_assert(offsetof(UIOption, current) == 0x18 && offsetof(UIOption, choice) == 0x20 && offsetof(UIOption, localizeChoices) == 0x30);
static_assert(offsetof(UIOption, getInt) == 0x38 && offsetof(UIOption, available) == 0x58 && offsetof(UIOption, dragging) == 0x68);




inline constexpr std::array<const char *, 41> kOptionsWindowIds{"SubMenu_Options::Tab1Button", "SubMenu_Options::Tab2Button", "SubMenu_Options::Tab3Button",
    "SubMenu_Options::Tab4Button", "SubMenu_Options::TabIcon4", "SubMenu_Options::TabLabel4", "SubMenu_Options::Tab4", "SubMenu_Options::Panel_Root",
    "SubMenu_Options::OptionParent_#", "SubMenu_Options::Option_Toggle_#", "SubMenu_Options::Option_ToggleRight_#", "SubMenu_Options::Option_ToggleLeft_#",
    "SubMenu_Options::Option_Toggle_Texture_#", "SubMenu_Options::Option_Toggle_Shadow_#", "SubMenu_Options::Option_Toggle_Label_#",
    "SubMenu_Options::Option_Toggle_Overlay_#", "SubMenu_Options::Option_List_#", "SubMenu_Options::Option_List_Overlay_#",
    "SubMenu_Options::Option_ListSelected_Parent_#", "SubMenu_Options::Option_ListSelected_#", "SubMenu_Options::Option_List_Left_#",
    "SubMenu_Options::Option_List_Right_#", "SubMenu_Options::Option_List_Texture_#", "SubMenu_Options::Option_List_Shadow_#",
    "SubMenu_Options::Option_List_Label_#", "SubMenu_Options::Option_Scroll_#", "SubMenu_Options::Option_Scroll_Texture_#",
    "SubMenu_Options::Option_Scroll_Shadow_#", "SubMenu_Options::Option_Scroll_Label_#", "SubMenu_Options::Option_ScrollBar_#",
    "SubMenu_Options::Option_ScrollBar_Button_#", "SubMenu_Options::Option_Scroll_Overlay_#", "SubMenu_Options::Option_KeyBinding_#",
    "SubMenu_Options::Option_KeyBinding_Texture_#", "SubMenu_Options::Option_KeyBinding_Shadow_#", "SubMenu_Options::Option_KeyBinding_Label_#",
    "SubMenu_Options::KeyBinding_Button_Parent_#", "SubMenu_Options::KeyBinding_Button_#", "SubMenu_Options::KeyBinding_Key_#",
    "SubMenu_Options::Option_KeyBinding_Overlay_#", "SubMenu_Options::Option_Title_#"};
inline constexpr std::array<const char *, 13> kOptionsLabelIds{"SubMenu_Options::ToggleOptionNameLabel", "SubMenu_Options::ToggleRight_Label",
    "SubMenu_Options::ToggleLeft_Label", "SubMenu_Options::ListOptionNameLabel", "SubMenu_Options::ListSelected_Label",
    "SubMenu_Options::ScrollOptionNameLabel", "SubMenu_Options::ScrollButton_Label", "SubMenu_Options::KeyBindingOptionNameLabel",
    "SubMenu_Options::TitleLabel", "SubMenu_Options::Label_Tab_1", "SubMenu_Options::Label_Tab_2", "SubMenu_Options::Label_Tab_3",
    "SubMenu_Options::Label_Tab_4"};
inline constexpr std::array<const char *, 47> kOptionsLayoutItemIds{"SubMenu_Options::ToggleOption_Background_Spacepush",
    "SubMenu_Options::ToggleBackground_Game_Shading", "SubMenu_Options::ToggleBackground_Front_Shading", "SubMenu_Options::ToggleBackground_Selected_Spacepush",
    "SubMenu_Options::ToggleOption_Label_Spacepush", "SubMenu_Options::ToggleRight_Spacepush", "SubMenu_Options::ToggleRight_Shadow_Spacepush",
    "SubMenu_Options::ToggleRight_Shadow_Spacepush2", "SubMenu_Options::ToggleRight_Label_Spacepush", "SubMenu_Options::ToggleLeft_Spacepush",
    "SubMenu_Options::ToggleLeft_Shadow_Spacepush", "SubMenu_Options::ToggleLeft_Shadow_Spacepush2", "SubMenu_Options::ToggleLeft_Label_Spacepush",
    "SubMenu_Options::ToggleOption_Overlay_Spacepush", "SubMenu_Options::ListOption_Background_Spacepush", "SubMenu_Options::ListBackground_Game_Shading",
    "SubMenu_Options::ListBackground_Front_Shading", "SubMenu_Options::ListBackground_Selected_Spacepush", "SubMenu_Options::ListOption_Label_Spacepush",
    "SubMenu_Options::List_Spacepush", "SubMenu_Options::List_Shadow_Spacepush", "SubMenu_Options::List_Shadow_Spacepush2",
    "SubMenu_Options::ListSelected_Label_Spacepush", "SubMenu_Options::Option_List_Left_Spacepush", "SubMenu_Options::Option_List_Right_Spacepush",
    "SubMenu_Options::ListOption_Overlay_Spacepush", "SubMenu_Options::ScrollOption_Background_Spacepush", "SubMenu_Options::ScrollBackground_Game_Shading",
    "SubMenu_Options::ScrollBackground_Front_Shading", "SubMenu_Options::ScrollBackground_Selected_Spacepush", "SubMenu_Options::ScrollOption_Label_Spacepush",
    "SubMenu_Options::Option_ScrollBar_Spacepush", "SubMenu_Options::Option_ScrollBar2_Spacepush", "SubMenu_Options::Option_ScrollBarButton_Spacepush",
    "SubMenu_Options::Option_ScrollBarButtonLabel_Spacepush", "SubMenu_Options::ScrollOption_Overlay_Spacepush",
    "SubMenu_Options::KeyBinding_Background_Spacepush", "SubMenu_Options::KeyBindingBackground_Game_Shading",
    "SubMenu_Options::KeyBindingBackground_Front_Shading", "SubMenu_Options::KeyBindingBackground_Selected_Spacepush",
    "SubMenu_Options::KeyBindingOption_Label_Spacepush", "SubMenu_Options::KeyBinding_Button_Spacepush", "SubMenu_Options::KeyBinding_Shadow_Spacepush",
    "SubMenu_Options::KeyBinding_Shadow_Spacepush2", "SubMenu_Options::KeyBinding_Key_Spacepush", "SubMenu_Options::KeyBinding_Overlay_Spacepush",
    "SubMenu_Options::Title_Label_Spacepush"};
inline constexpr std::array<const char *, 48> kOptionsColourImageIds{"SubMenu_Options::Image_Tab_1", "SubMenu_Options::Image_Tab_2",
    "SubMenu_Options::Image_Tab_3", "SubMenu_Options::Image_Tab_4", "SubMenu_Options::Texture_Front_1", "SubMenu_Options::Texture_Front_2",
    "SubMenu_Options::Texture_Front_3", "SubMenu_Options::Texture_Front_4", "SubMenu_Options::Texture_Game_1", "SubMenu_Options::Texture_Game_2",
    "SubMenu_Options::Texture_Game_3", "SubMenu_Options::Texture_Game_4", "SubMenu_Options::MainTexture_Game", "SubMenu_Options::MainTexture_Front",
    "SubMenu_Options::ToggleTexture_Game", "SubMenu_Options::ToggleTexture_Game2", "SubMenu_Options::ToggleTexture_Front",
    "SubMenu_Options::ToggleBackground_Game", "SubMenu_Options::ToggleBackground_Front", "SubMenu_Options::ToggleBackground_Selected",
    "SubMenu_Options::ToggleRight_SubBackground_Game", "SubMenu_Options::ToggleRight_SubBackground_Front", "SubMenu_Options::ToggleLeft_SubBackground_Game",
    "SubMenu_Options::ToggleLeft_SubBackground_Front", "SubMenu_Options::ListTexture_Game", "SubMenu_Options::ListTexture_Game2",
    "SubMenu_Options::ListTexture_Front", "SubMenu_Options::ListBackground_Game", "SubMenu_Options::ListBackground_Front",
    "SubMenu_Options::ListBackground_Selected", "SubMenu_Options::List_SubBackground_Game", "SubMenu_Options::List_SubBackground_Front",
    "SubMenu_Options::Arrow_Left", "SubMenu_Options::Arrow_Right", "SubMenu_Options::ScrollTexture_Game", "SubMenu_Options::ScrollTexture_Game2",
    "SubMenu_Options::ScrollTexture_Front", "SubMenu_Options::ScrollBackground_Front", "SubMenu_Options::ScrollBackground_Game",
    "SubMenu_Options::ScrollBackground_Selected", "SubMenu_Options::KeyBindingTexture_Game", "SubMenu_Options::KeyBindingTexture_Game2",
    "SubMenu_Options::KeyBindingTexture_Front", "SubMenu_Options::KeyBindingBackground_Game", "SubMenu_Options::KeyBindingBackground_Front",
    "SubMenu_Options::KeyBindingBackground_Selected", "SubMenu_Options::KeyBinding_Button_SubBackground_Game",
    "SubMenu_Options::KeyBinding_Button_SubBackground_Front"};
inline constexpr std::array<const char *, 19> kOptionsButtonIds{"SubMenu_Options::MainPanelButton", "SubMenu_Options::Toggle_Background",
    "SubMenu_Options::Option_ToggleRight_Button", "SubMenu_Options::Option_ToggleLeft_Button", "SubMenu_Options::ToggleOption_Overlay",
    "SubMenu_Options::List_Background", "SubMenu_Options::Option_ListSelectedButton", "SubMenu_Options::ListOption_Overlay",
    "SubMenu_Options::Scroll_Background", "SubMenu_Options::Option_ScrollBar_BackgroundOutline", "SubMenu_Options::Option_ScrollBar_Button",
    "SubMenu_Options::ScrollOption_Overlay", "SubMenu_Options::KeyBinding_Background", "SubMenu_Options::KeyBinding_Button_Button",
    "SubMenu_Options::KeyBinding_Overlay", "SubMenu_Options::Button_Tab_1", "SubMenu_Options::Button_Tab_2", "SubMenu_Options::Button_Tab_3",
    "SubMenu_Options::Button_Tab_4"};

inline constexpr std::array<std::int32_t, 13> kOptionsFrontEndImages{13, 16, 18, 23, 21, 26, 28, 31, 36, 37, 42, 44, 47};
inline constexpr std::array<std::int32_t, 17> kOptionsInGameImages{12, 14, 15, 17, 20, 22, 24, 25, 27, 30, 34, 35, 38, 40, 41, 43, 46};

// The Options colour statics (static initializers), by what STATIC_SubMenu__Options_unk_func7 gives them to in the frontend and in game:
// the tab Buttons and the shown tab's, the tab backgrounds and the shown tab's, the main panel Button, the row background Buttons, the row
// control Buttons, the scroll bar Buttons and the list arrows; and the key binding label colours, bound and unbound (also the
// unbound-keys warning's).
struct UIOptionsColours {
    std::uint32_t tabButton;
    std::uint32_t tabButtonShown;
    std::uint32_t tabBackground;
    std::uint32_t tabBackgroundShown;
    std::uint32_t mainPanelButton;
    std::uint32_t rowBackground;
    std::uint32_t rowControl;
    std::uint32_t scrollBar;
    std::uint32_t arrow;
};
inline constexpr UIOptionsColours kUIOptionsFrontEndColours{.tabButton = 0x00000000U,
    .tabButtonShown = 0xffa2997cU,
    .tabBackground = 0x00ffffffU,
    .tabBackgroundShown = 0x647f7f7fU,
    .mainPanelButton = 0xffa2997cU,
    .rowBackground = 0xc8565250U,
    .rowControl = 0xc8ffebaaU,
    .scrollBar = 0xb4c8b991U,
    .arrow = 0xffffffffU};
inline constexpr UIOptionsColours kUIOptionsInGameColours{.tabButton = 0x7f322b1cU,
    .tabButtonShown = 0xff322b1cU,
    .tabBackground = 0x00ffffffU,
    .tabBackgroundShown = 0xa0ffffffU,
    .mainPanelButton = 0xff322b1cU,
    .rowBackground = 0xdc272415U,
    .rowControl = 0xff5d5640U,
    .scrollBar = 0xff5d5640U,
    .arrow = 0xffffffffU};
inline constexpr std::uint32_t kOptionsKeyBoundColour = 0xffffffffU;
inline constexpr std::uint32_t kOptionsKeyUnboundColour = 0xff1e3fb9U;


struct SharedItemsPreviewRequest {
    std::uint16_t requestActive;
    std::array<std::uint8_t, 6> reserved02;
    std::int32_t requestType;
    std::int32_t itemId;
};
static_assert(sizeof(SharedItemsPreviewRequest) == 16);
static_assert(offsetof(SharedItemsPreviewRequest, requestType) == 8);




inline constexpr std::array<const char *, 9> kLoadSaveWindowIds{"LoadScreen::Panel_GridHolder", "LoadScreen::Load_Screen_Panel",
    "LoadScreen::LoadEntry_Parent_#", "LoadScreen::LoadEntry_Background_#", "LoadScreen::LoadEntry_Texture_#", "LoadScreen::LoadEntry_Shading_#",
    "LoadScreen::LoadEntry_Icon_#", "LoadScreen::LoadEntry_MidText_#", "LoadScreen::LoadEntry_RightText_#"};
inline constexpr std::array<const char *, 8> kLoadSaveViewImageIds{"LoadBackground", "LoadScreen::ImageFrame", "LoadScreen::ImageBase",
    "LoadScreen::MapAreaBackground", "LoadScreen::AreaDefault", "LoadScreen::MapDefault", "LoadScreen::AreaNoSave", "LoadScreen::MapNoSave"};
inline constexpr std::array<const char *, 9> kLoadSaveColourImageIds{"LoadScreen::MainTexture_Game", "LoadScreen::MainTexture_Front",
    "LoadScreen::LoadEntryTexture_Game", "LoadScreen::LoadEntryTexture_Game2", "LoadScreen::LoadEntryTexture_Front", "LoadScreen::LoadEntryBackground_Game",
    "LoadScreen::LoadEntryBackground_Front", "LoadScreen::LoadEntryBackground_Selected", "LoadScreen::LoadRowIcon"};
inline constexpr std::array<const char *, 12> kLoadSaveLayoutItemIds{"LoadScreen::LoadEntry_Background_Spacepush",
    "LoadScreen::LoadEntryBackground_Game_Shading", "LoadScreen::LoadEntryBackground_Front_Shading", "LoadScreen::LoadEntry_ShadingSelected_Spacepush",
    "LoadScreen::LoadEntry_Icon_Spacepush", "LoadScreen::LoadEntry_Number_Spacepush", "LoadScreen::LoadEntry_GameLabel_Spacepush",
    "LoadScreen::LoadEntry_DataLabel_Spacepush", "LoadScreen::LoadEntry_PlaceName_Spacepush", "LoadScreen::LoadEntry_GameTime_Spacepush",
    "LoadScreen::LoadEntry_SaveTimeLabel_Spacepush", "LoadScreen::LoadEntry_SaveTime_Spacepush"};
inline constexpr std::array<const char *, 7> kLoadSaveLabelIds{"LoadScreen::LoadEntry_Number", "LoadScreen::LoadEntry_GameLabel",
    "LoadScreen::LoadEntry_DataLabel", "LoadScreen::LoadEntry_PlaceName", "LoadScreen::LoadEntry_GameTime", "LoadScreen::LoadEntry_SaveTimeLabel",
    "LoadScreen::LoadEntry_SaveTime"};
inline constexpr std::array<const char *, 2> kLoadSaveButtonIds{"LoadScreen::MainPanelButton", "LoadScreen::LoadEntry_Background"};
inline constexpr std::array<const char *, 25> kLoadSaveMapImageIds{"LoadScreen::EN_MAPS_A000_00", "LoadScreen::EN_MAPS_A000_01", "LoadScreen::EN_MAPS_A000_02",
    "LoadScreen::EN_MAPS_A000_03", "LoadScreen::EN_MAPS_A000_04", "LoadScreen::EN_MAPS_A000_05", "LoadScreen::EN_MAPS_W000_00", "LoadScreen::EN_MAPS_W000_01",
    "LoadScreen::EN_MAPS_W000_02", "LoadScreen::EN_MAPS_W000_03", "LoadScreen::EN_MAPS_W000_04", "LoadScreen::EN_MAPS_W000_05", "LoadScreen::EN_MAPS_W000_06",
    "LoadScreen::EN_MAPS_W000_07", "LoadScreen::EN_MAPS_Q000_00", "LoadScreen::EN_MAPS_Q000_01", "LoadScreen::EN_MAPS_Q000_02", "LoadScreen::EN_MAPS_Q000_03",
    "LoadScreen::EN_MAPS_Q000_04", "LoadScreen::EN_MAPS_Q000_05", "LoadScreen::EN_MAPS_Q000_06", "LoadScreen::EN_MAPS_Q000_07", "LoadScreen::EN_MAPS_Q000_08",
    "LoadScreen::EN_MAPS_Q000_09", "LoadScreen::EN_MAPS_Q000_10"};
inline constexpr std::array<const char *, 6> kLoadSaveAreaImageIds{
    "LoadScreen::Area_Aba", "LoadScreen::Area_Wan", "LoadScreen::Area_Kow", "LoadScreen::Area_Hei", "LoadScreen::Area_SM1", "LoadScreen::Area_SM2"};




void SRCH_ApplyPromptIconTypeSet(const std::int32_t *prompts);
void SRCH_ApplySearchPromptSlots();




struct UIGameMessagePresentation {
    std::int32_t animSlot{-1};
    std::array<d3t::UIWindow *, 3> windows{};
    d3t::UILabelWidget *label{};
    UIButtonIconWidget *placeholderSprite{};
    std::array<UIColourImageWidget *, 2> images{};
    std::int32_t spriteIndex{};
    std::array<char, 256> labelText{};
    UIGameMessageContentType contentType{UIGameMessageContentType::Item};
};



struct UIDrawObject {
    void *model;
    Vector3 position;
    Vector3i rotation;
    float scale;
    Vector4 lightColour;
    std::int32_t lightYaw;
    std::array<std::int32_t, 18> textureIds;
};
static_assert(offsetof(UIDrawObject, position) == 0x08);
static_assert(offsetof(UIDrawObject, rotation) == 0x14);
static_assert(offsetof(UIDrawObject, scale) == 0x20);
static_assert(offsetof(UIDrawObject, lightColour) == 0x24);
static_assert(offsetof(UIDrawObject, lightYaw) == 0x34);
static_assert(offsetof(UIDrawObject, textureIds) == 0x38);

void DrawObjectForUI(UIDrawObject *object);



extern std::array<std::uint32_t, 4> g_notebookTextureLeft;
extern std::array<std::uint32_t, 4> g_notebookTextureRight;
extern std::array<std::uint8_t, 4> g_notebookLeftPageFlags;
extern std::array<std::uint8_t, 4> g_notebookRightPageFlags;
extern std::array<std::array<char, 1280>, 4> g_memoText;
extern std::array<const char *, 4> g_memoTextPages;
extern std::array<std::byte *, 4> g_notebookMemoTextureData;
void CreateNotebookTexture2DForPages();
void CreateNotebookTexture2D(std::int32_t surface, std::byte *memoTexture);
void FormatNotebookText(std::uint32_t pageIndex, std::uint32_t textBuffer, std::uint32_t pageSlot, std::uint32_t line, char messageType);
void Notebook_BindModelPageTextures();
void InitNotebookTexture2D();


std::uint32_t UI_SetPVRSurfaceTexture(std::int32_t surface, std::uint32_t texture);

void UI_ActivateElement(UIBaseRuntime &element);

void HandleUICallbacks();



enum class UIIconSystemGameState : std::int8_t {
    Timex = 0,
    Collection = 1,
    SaveDisplay = 2,
    Unused3 = 3,
    Notebook = 4,
    MovesScreen = 5,
    EasyOptions = 6,
    SystemMenuIdle = 7,
    SystemMenuRefresh = 8,
};




struct IconSystemMenuCameraTransition {
    Vector3 eye;
    Vector3 savedEye;
    Vector3 destinationEye;
    Vector3 eyeStep;
    Vector3 target;
    Vector3 savedTarget;
    Vector3 destinationTarget;
    Vector3 targetStep;
    float ryoOpacity;
    // g_pRyoPrevRenderOpacity.
    float savedRyoOpacity;
    float ryoOpacityStep;
    // g_icon_system_menu_saved_author_state and g_icon_system_menu_saved_temp_game_state.
    std::int32_t savedAuthorState;
    std::int32_t savedTemporaryGameState;
    std::int32_t frameCount;
    std::int32_t frame;
    std::uint8_t mode;
};
static_assert(offsetof(IconSystemMenuCameraTransition, savedEye) == 0x0c);
static_assert(offsetof(IconSystemMenuCameraTransition, target) == 0x30);
static_assert(offsetof(IconSystemMenuCameraTransition, targetStep) == 0x54);
static_assert(offsetof(IconSystemMenuCameraTransition, ryoOpacity) == 0x60);
static_assert(offsetof(IconSystemMenuCameraTransition, savedAuthorState) == 0x6c);
static_assert(offsetof(IconSystemMenuCameraTransition, frameCount) == 0x74);
static_assert(offsetof(IconSystemMenuCameraTransition, mode) == 0x7c);




struct IconPresentationOwner {
    HLib::Task *rootTask{};
    HLib::Task *menuTask{};
    HLib::Task *itemLookTask{};
    HLib::Task *systemStateTask{};
    HLib::Task *moneyTask{};
    HLib::Task *actionSelectorTask{};
    HLib::Task *notificationTask{};
    HLib::Task *cassetteTask{};
    PadCaptureEntry *input{};
    PadCaptureEntry *previousInput{};
    PadCaptureEntry *notificationInput{};
    PadCaptureEntry *notificationPreviousInput{};
    PadCaptureEntry *itemLookInput{};
    PadCaptureEntry *itemLookPreviousInput{};
    UIIconNotificationState state{UIIconNotificationState::Idle};
    UIIconPromptKind kind{UIIconPromptKind::None};
    UIIconSystemMenuState menuState{UIIconSystemMenuState::Closed};
    UIIconSystemMenuSelection menuSelection{UIIconSystemMenuSelection::Items};
    std::uint8_t expectedInput{};

    std::int32_t promptRotation{};
    std::uint16_t promptDescriptor{};
    std::uint8_t overlayTransitionStage{};
    std::uint8_t overlayMode{};
    std::uint8_t itemInfoPopupPhase{};
    std::uint8_t itemInfoPopupFrames{};
    CVWKRuntimeSlot *itemInfoVoice{};
    std::uint8_t itemInfoPopupJinguruType{};
    std::uint8_t itemInfoPopupVisible{};

    std::uint8_t itemInfoPopupNameWidthClass{};
    std::array<float, 4> itemInfoPopupPosition{-672.0f, 55.0f, 1.0f, 0.0f};
    ScreenSpaceParams itemIconParams{};
    std::int16_t itemInfoSurface{-1};
    std::uint16_t inputMask{};
    std::uint16_t iconFlags{};
    std::uint32_t cachedPlayerControlMode{};
    std::uint16_t bufferedInitialPressMask{};
    std::uint16_t bufferedFollowupPressMask{};


    UIIconSystemGameState systemGameState{UIIconSystemGameState::SystemMenuIdle};
    std::int32_t pendingState{};
    std::uint16_t cassetteItemState{};
    IconSystemMenuCameraTransition cameraTransition{};


    bool savedSicnTaskActive{};
    std::int32_t savedRyoMotionVisible{};
    std::int32_t savedRyoFlag10{};
    std::int16_t timeoutFrames{};
    std::int16_t alpha{};
    std::int16_t alphaStep{255};
    std::int16_t bobOffset{};
    std::int16_t bobStep{3};
    std::int16_t inputCaptureMode{};
    std::uint16_t ownerMask{};
    std::uint16_t savedSicnState{};
    UISicnPausePolicy savedSicnPausePolicy{UISicnPausePolicy::ClearOnPause};
    std::uint8_t savedHelpToken{};
    std::uint8_t savedHelpButton{};
    std::int32_t savedPlayerGameState{7};

    std::uint8_t itemLookSavedHelpToken{};
    std::uint8_t itemLookSavedHelpButton{};
    std::uint16_t itemLookSavedSicnState{};
    UISicnPausePolicy itemLookSavedSicnPausePolicy{UISicnPausePolicy::ClearOnPause};
    std::uint16_t sharedItemMenuState{};
    std::int16_t sharedItemPreviewValue{-1};
    std::uint32_t inventoryGridRevision{};
    std::int16_t inventoryOwnedEntryCount{};
    std::uint16_t inventoryOwnedItemCount{};
    std::int8_t inventorySelectionPage{};
    std::int8_t inventorySelectionSlot{};
    std::int8_t inventoryPageCount{1};
    std::int8_t inventorySlotsPerPage{8};
    bool active{};
    bool menuCloseRequested{};

    bool sharedItemPreviewIsCassette{};




    const OwnedItemCatalogEntry *itemLookEntry{};
    float itemLookZoom{};
    float itemLookZoomMax{};
    float itemLookZoomMin{};
    std::int32_t itemLookYaw{};
    Vector3 itemLookPosition{};
    std::int32_t itemLookLightYaw{};
    std::int32_t itemLookSavedAuthorState{};
    std::int32_t itemLookSavedTemporaryGameState{};
    std::int32_t itemLookSavedFov{};
    std::int32_t itemLookSavedRyoRenderFlag{};
    std::uint32_t itemLookSavedRyoVisible{};
    Vector3 itemLookSavedEye{};
    Vector3 itemLookSavedTarget{};
    bool itemLookSavedSicnTaskActive{};
    PackModel *itemLookModel{};
    Mt5RuntimeNode *itemLookModelRoot{};
    CVWKRuntimeSlot *itemLookRemark{};
    std::uint32_t itemLookRemarkMove{};
    std::int32_t itemLookTeachFrames{};
    bool itemLookScriptOpened{};
};

extern IconPresentationOwner g_iconPresentation;


extern std::uint16_t g_tmpPadcHeld;
extern std::uint32_t g_sfxInputFlags;
extern std::uint16_t g_tmpRightStickHi;
extern std::uint16_t g_tmpRightStickLo;

extern std::int32_t g_conversationPromptActivationLock;

extern std::int8_t g_cassetteCollectionUiState;

extern std::int32_t g_isPlayerGameStateActive;


extern std::uint8_t g_previousHelpTokenId;
extern std::uint8_t g_defaultHelpTokenId;
extern std::uint8_t g_currentHelpTokenId;
extern std::uint8_t g_helpButtonId;
extern std::uint16_t g_currentHelpIconState;
struct ScenarioContext;

void UIAnim_UpdateDeltaTime(float deltaTime);
void UI_PresentationOwnersInit();
struct PackSlot;
std::int32_t COMICONSPRLoad(PackSlot *pack);
bool convCanOpen();
void UI_SetConversationPromptSceneEnabled(bool enabled);
void UI_PresentationOwnersShutdown();
void UI_PresentSearchFocusedItem(HLib::Task *scenarioTask, const char *stringId, bool visible);
bool UI_ICLISharedItemMenuIsActive();
bool UI_ICLISetSharedItemPreview(std::int16_t itemId);
bool UI_ICLILoadSharedItemPreview(std::int16_t itemId);
void UI_ICLIResetSharedItemMenu();
void DestroySharedItemsMenuIfTrueResponse();

UIPresentationSnapshot UI_GetPresentationSnapshot();
void UI_GUITasksEnqueue();
void UI_GUITasksShutdown();
void ReturnToTitleScreen();

void UI_HELPDispatch(std::int32_t mode, std::uint8_t helpIconId, std::uint8_t buttonId);
void UI_HELPClear();
void UI_HELPSetEmulatorContext(bool enabled);
void UI_HELPClearEmulatorPrompt();
void UI_HELPPreparePage(std::uint8_t token);
void UI_HELPStartTransitionAnim();
HLib::Task *UI_HELPGetTask();

void UI_HELPCallback(HLib::TaskData *taskData);

bool UI_ICONRequestNotification(std::uint16_t inputMask, std::int16_t timeoutFrames, std::int16_t alphaFrames, std::int32_t ownerAndCaptureMode);
bool UI_ICONNotificationIsFadingOut();
void UI_ICONClear();
void UI_ICONCallback(HLib::TaskData *taskData);
void UI_ICNQCallback(HLib::TaskData *taskData);

bool UI_ICONRequestSystemMenu();
void UI_ICONRequestSystemMenuClose();
bool UI_ICONSystemMenuIsClosed();
void UI_ICONRecomputeInventoryGrid();
bool UI_ICONStartItemInfoPopup(std::uint16_t itemId);
bool UI_ICONAdvanceItemInfoPopup();
void UI_ResetForSequenceStart();
void UI_HandleDobuitaEventStartTransition();
void UI_ResetConversationDetailsState();
void UI_ICMoWaitCloseAnimation();
void UI_ResetForCPCT();
void UI_CloseConversationUIAndReturnToFreeRoam();
void UI_ICISCallback(HLib::TaskData *taskData);
void UI_ICLICallback(HLib::TaskData *taskData);
void UI_ICSSCallback(HLib::TaskData *taskData);
void UI_ICMCCallback(HLib::TaskData *taskData);
bool UI_ICMCBegin();
void UI_StopPlayingCassettes();
bool UI_ICMCWaitForClosed(std::int32_t &result);

void UI_ICMoRequest(std::int32_t value, bool useCash);
void UI_ICMoShowCurrentBalance(bool show);
void UI_ICMoBeginSelector(std::int32_t maximumValue, std::uint16_t timeoutFrames);
void UI_ICMoRequestClose();
bool UI_ICMoIsIdle();
bool UI_ICMoPeekResult(std::int32_t &result);
void UI_ICMoCallback(HLib::TaskData *taskData);

bool UI_ICSpRequest(const UIActionSelectorRequest &request);
bool UI_ICSpIsIdle();
bool UI_ICSpPeekSelectedHelpToken(std::int16_t &helpToken);
bool UI_ICSpPeekSelectedChoiceIndex(std::int32_t &choiceIndex);
void UI_ICSpCancel();
void UI_ICSpCallback(HLib::TaskData *taskData);

void UI_ICSVRequest(std::int8_t flags);
std::uint8_t UI_ICSVNativeReadinessComposite() noexcept;
bool UI_ICSVPeekScriptResult(std::uint32_t &result);
void UI_ICSVCallback(HLib::TaskData *taskData);

bool UI_ShowRetry70ManBattlePrompt(UIRetry70ManBattlePromptCallback callback);
void UI_CloseRetry70ManBattlePrompt();
void MFBT_Init(std::uint32_t *bestTimes, std::uint32_t &defeatedEnemies);
void MFBT_ResetSurfaceIds();
void MFBT_ReleaseResources();
void AdjustAspectRatioMatrixHorizontalStretchAdjusted(std::int32_t num, std::int32_t horizontalPosition, std::int32_t verticalPosition);
void shenComputeNumEnemies(std::uint32_t currentTicks, std::uint32_t defeatedEnemies);
void MFBT_DrawElapsedTime(std::uint32_t currentTicks, std::int32_t horizontalPosition, std::int32_t verticalPosition);
void VFMA_ICON();
std::uint64_t shenLoadSprite(std::int8_t transition, std::uint8_t fadeTicks, std::uint8_t buttonId);
extern std::uint8_t g_lcdmHelpIconSpritesLoaded;
extern std::int16_t g_helpIconWidth;
extern std::int16_t g_helpIconHeight;
void UI_VFUpdateHealthPercentage(float healthPercentage, std::int32_t gaugeStatus, float pulsePosition);

void UI_ICQuCallback(HLib::TaskData *taskData);
void UI_SICNEnqueue(std::uint16_t state, UISicnPausePolicy pausePolicy);
void UI_SICNSetState(std::uint16_t state, UISicnPausePolicy pausePolicy);
void UI_SICNClearState(std::uint16_t state);
bool UI_SICNIsTaskActive();
void UI_SICNDispatch(std::int32_t mode, std::uint16_t state, UISicnPausePolicy pausePolicy);
void UI_SICNCallback(HLib::TaskData *taskData);


struct MapLocationToken {
    const char *areaId;
    const char *uiToken;
    const char *areaName;
};



struct LoadScreenPresentation {
    std::array<d3t::UIWindow *, 9> windows{};
    std::array<std::int32_t, 9> widgetBindings{-1, -1, -1, -1, -1, -1, -1, -1, -1};
    std::array<char, 512> time{};
    std::array<char, 512> date{};
    std::array<char, 512> area{};
    std::array<char, 512> subArea{};
    std::array<char, 512> nowLoading{};
    std::uint32_t headerTexture{};
    std::int32_t headerImage{63};
    bool loadingTextHeld{};
    std::int32_t imageSet{};
};

std::int32_t UIWidget_Bind(std::int32_t objectHandle, float *property, std::array<float, 2> hidden, std::array<float, 2> shown);
void UIWidget_Animate(std::int32_t binding, std::int32_t direction, float duration, float delay);
void LoadLoadingScreenImage(std::int32_t name, std::int32_t loadIndex, std::int32_t specialDateIndex, std::int32_t year, std::int32_t month, std::int32_t day,
    std::int32_t dayOfWeek, std::int32_t hours, std::int32_t minutes, bool pm, const char *filename, UICdttMode mode);
extern LoadScreenPresentation g_loadScreenPresentation;

extern std::int32_t g_debugLoadingWidgetEnabled;
extern std::array<UIColourImageWidget *, 31> g_frontendEmblemImages;
extern std::int32_t g_frontendEmblemImageCount, g_frontendEmblemAnimSlot;

extern std::int32_t g_qteSessionActive;
void UI_SFXOverlayBegin(const UISfxOverlayRequest &request);
void UI_SFXOverlayBegin(std::int16_t timeoutFrames);
void UI_SFXOverlayResolve(std::int16_t result);
bool UI_SFXOverlayConsumeResultWhileOpen(std::int16_t &result);
bool UI_SFXOverlayConsumeResultIfIdle(std::int16_t &result);
void UI_SFXOverlayCallback(HLib::TaskData *taskData);
bool UI_IsFrontendCaptureReady() noexcept;

// Shenmue scenario function 392.
void Script_SetSfxOverlayTimeoutTuning(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1

#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {

struct CashOverlayOwner {
    UICashOverlayState state{UICashOverlayState::Idle};
    std::int32_t value{};
    std::int32_t maximumValue{};
    std::int32_t selectedValue{};
    std::int32_t targetBalance{};
    std::int16_t timeoutFrames{600};
    std::int16_t elapsedFrames{};
    float autoCloseFrames{};
    std::uint16_t savedSicnState{};
    UISicnPausePolicy savedSicnPausePolicy{UISicnPausePolicy::KeepOnPause};
    bool savedSicnActive{};
    std::int16_t selectorSurface{-1};
    bool useCash{true};
    bool selectorActive{};
    UICashOverlayDisplayMode displayMode{UICashOverlayDisplayMode::None};
    bool resetRequested{};
    bool balanceAnimating{};
    std::int32_t lastBalanceAnimationStep{};
};

} // namespace shenmue::s1::ui_runtime_detail

#include <array>
#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {

inline constexpr std::uint16_t kUiStateTransientNotification = 0x200;

inline constexpr std::uint16_t kIconOwnerScnt = 1;

inline constexpr std::uint16_t kIconOwnerScript = 2;

struct SicnPromptSlot {
    std::uint16_t assetId{};
    std::uint16_t width{};
    std::uint16_t height{};
    float x{};
    float y{};
    float scaleX{1.0f};
    float scaleY{1.0f};
    float depth{8.0f};
    bool active{};
};

inline constexpr std::array<std::uint16_t, 12> kSicnPromptAssetIds{294, 265, 264, 269, 268, 298, 297, 293, 296, 2, 302, 245};

inline constexpr std::array<std::uint16_t, 12> kSicnPromptWidths{64, 32, 32, 32, 32, 32, 32, 32, 32, 32, 128, 32};

inline constexpr std::array<std::uint16_t, 12> kSicnPromptHeights{64, 32, 32, 32, 32, 32, 32, 32, 32, 32, 64, 32};

inline constexpr std::array<std::uint8_t, 4> kIconButtonMatchCodes{8, 9, 10, 11};

inline constexpr std::array<std::uint16_t, 4> kIconButtonAssetIds{264, 265, 268, 269};

inline constexpr std::array<std::uint8_t, 16> kDirectionInputCodes{24, 21, 26, 27, 25, 16, 23, 0, 0, 4, 3, 4, 2, 4, 1, 4};

} // namespace shenmue::s1::ui_runtime_detail

#include <array>
#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {


inline constexpr std::array<std::uint32_t, 12> kUiSoundCommands{
    0x400abU, 0x100abU, 0x900abU, 0x200abU, 0x300abU, 0x800abU, 0x8abU, 0x208abU, 0x408abU, 0x308abU, 0x208abU, 0x1100abU};

} // namespace shenmue::s1::ui_runtime_detail

#include <array>

namespace shenmue::s1::ui_runtime_detail {

// The Game Message's windows (MainPanelAnim and its Start and End placements) and its Token and Money images.
inline constexpr std::array<const char *, 3> kGameMessageWindowIds{
    "UIGameMessage::MainPanelAnim", "UIGameMessage::MainPanelAnimStart", "UIGameMessage::MainPanelAnimEnd"};

inline constexpr std::array<const char *, 2> kGameMessageImageIds{"UIGameMessage::TokenImage", "UIGameMessage::MoneyImage"};

// The Information Bar's windows (the front-end bar's parent and panel, the in-game bar's parent, texture and panel, the labels panel and the
// focus outline), its right and left text labels, its one- and two-line IconLabels, its left and right icons and background image, and the
// view images of its in-game and front-end backgrounds and the front-end shadow.
inline constexpr std::array<const char *, 7> kInformationBarWindowIds{"Information_Bar::InfoBoxParent_FrontEnd", "Information_Bar::InfoBoxPanelFrontEnd",
    "Information_Bar::InfoBoxParent_InGame", "Information_Bar::InfoBoxPanelInGame_Texture", "Information_Bar::InfoBoxPanelInGame",
    "Information_Bar::InfoBoxLabels", "Information_Bar::Panel_FocusOutline"};

inline constexpr std::array<const char *, 2> kInformationBarLabelIds{"Label_InfoBox_RightText", "Label_InfoBox_LeftText"};

inline constexpr std::array<const char *, 3> kInformationBarIconLabelIds{"Label_InfoBox_1Line", "Label_InfoBox_2Line1", "Label_InfoBox_2Line2"};

inline constexpr std::array<const char *, 3> kInformationBarImageIds{"Image_InfoBar_Left", "Image_InfoBar_Right", "Image_Background"};

inline constexpr std::array<const char *, 3> kInformationBarBackgroundImageIds{
    "InfoBarBackground_Ingame", "InfoBarBackground_Frontend", "InfoBarBackground_Frontend_Shadow"};

} // namespace shenmue::s1::ui_runtime_detail
