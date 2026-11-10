#pragma once
#include <cstdint>
#include <array>
#include <cstddef>
#include "s2/hlib.hpp"
#include "s2/title_area_data.hpp"
#include "common/types.hpp"

namespace shenmue::s2 {
struct TitleFileSelectState;
enum class SystemModule : std::int32_t;
enum class UIElement : std::int32_t;

struct TitleCallbackPair {
    HLib::TaskCallback control;
    HLib::TaskCallback display;
};
static_assert(sizeof(TitleCallbackPair) == 0x10);

struct TitleDebugModuleEntry {
    const char *name;
    SystemModule module;
    std::int32_t reserved0c;
    void (*control)();
    std::int32_t (*initialize)(UIElement);
};
static_assert(sizeof(TitleDebugModuleEntry) == 0x20);
static_assert(offsetof(TitleDebugModuleEntry, control) == 0x10);
static_assert(offsetof(TitleDebugModuleEntry, initialize) == 0x18);
inline constexpr shen_name titleAreaName{SHEN_NAME(TITL)};
inline constexpr shen_name TITLE_CONTROL_TASK{SHEN_NAME(TCRL)};
inline constexpr shen_name TITLE_DISPLAY_TASK{SHEN_NAME(DISP)};
inline constexpr shen_name TITLE_SOUND_DATA_TASK{SHEN_NAME(TTSD)};

enum class TitleSequence : std::int32_t {
    PressStart,
    MoveToAm2Logos,
    MoveToAm2Logos2,
    MoveToAm2Logos3,
    MoveToAm2Logos4,
    MoveToAm2Logos5,
    MoveToAm2Logos6,
    MoveToAm2Logos7,
    MoveToAm2Logos8,
    MoveToAm2Logos9,
    WarningLogos,
    WarningLogos2,
    WarningLogos3,
};
inline constexpr std::array<std::int32_t, 4> TITLE_OPTION_ITEMS{0, 1, 2, 3};
struct TitleDisplayData {
    float value;
    std::byte reserved04[0x54];
    float currentStateFrame;
    float stateFrame;
    std::uint32_t frameCount;
};
struct TitleControlData {
    HLib::Task *titleSequenceTask;
    HLib::Task *screenFilmTask;
    TitleDisplayData *display;
    std::int32_t field18;
    std::int32_t state;
    std::byte inheritedSaveBank[0xa000];
    std::byte warningParameters[0x10];
};
struct TitleModuleWork {
    std::int32_t frames;
    std::int32_t reserved;
    HLib::Task *screenFilmTask;
};
static_assert(sizeof(TitleDisplayData) == 0x64);
static_assert(offsetof(TitleDisplayData, currentStateFrame) == 0x58);
static_assert(offsetof(TitleDisplayData, stateFrame) == 0x5c);
static_assert(offsetof(TitleDisplayData, frameCount) == 0x60);
static_assert(sizeof(TitleControlData) == 0xa030);
static_assert(offsetof(TitleControlData, titleSequenceTask) == 0);
static_assert(offsetof(TitleControlData, screenFilmTask) == 8);
static_assert(offsetof(TitleControlData, display) == 0x10);
static_assert(offsetof(TitleControlData, state) == 0x1c);
static_assert(offsetof(TitleControlData, inheritedSaveBank) == 0x20);
static_assert(offsetof(TitleControlData, warningParameters) == 0xa020);
static_assert(sizeof(TitleModuleWork) == 0x10);
static_assert(offsetof(TitleModuleWork, screenFilmTask) == 8);
inline constexpr std::array<std::array<std::int16_t, 2>, 5> TITLE_LOGO_DIMENSIONS{{
    {512, 128},
    {512, 512},
    {256, 128},
    {128, 128},
    {256, 256},
}};

using TitleSpriteUv = std::array<std::uint16_t, 4>;
inline constexpr std::array<TitleSpriteUv, 2> TITLE_MENU_HEADER_UV{{{0, 898, 625, 1000}, {0, 796, 625, 898}}};
inline constexpr TitleSpriteUv TITLE_WHEEL_SELECTED_UV{0, 500, 296, 796}, TITLE_WHEEL_ITEM_UV{296, 500, 593, 796};
inline constexpr std::array<TitleSpriteUv, 6> TITLE_WHEEL_LABEL_UV{
    {{625, 921, 957, 1000}, {625, 843, 957, 921}, {625, 640, 957, 765}, {625, 515, 957, 640}, {625, 765, 957, 843}, {625, 390, 957, 515}}};
inline constexpr std::array<std::int32_t, 5> TITLE_MENU_CAPTIONS{0, 1, 13, 15, 2};
inline constexpr std::array<TitleSpriteUv, 11> TITLE_OPTION_FRAME_UV{{{0, 796, 625, 1000}, {0, 593, 625, 796}, {625, 796, 988, 1000}, {625, 593, 988, 790},
    {0, 417, 341, 593}, {343, 417, 683, 593}, {0, 242, 400, 417}, {683, 457, 722, 593}, {722, 457, 761, 593}, {761, 457, 800, 593}, {800, 457, 839, 593}}};
inline constexpr std::array<TitleSpriteUv, 12> TITLE_OPTION_TEXT_UV{
    {{0, 875, 437, 1000}, {437, 875, 1000, 1000}, {0, 750, 750, 875}, {566, 750, 1000, 875}, {0, 625, 625, 750}, {0, 500, 625, 625}, {0, 375, 625, 500},
        {0, 250, 625, 375}, {0, 125, 625, 250}, {0, 0, 625, 125}, {625, 125, 1000, 250}, {625, 0, 1000, 125}}};
inline constexpr std::array<TitleSpriteUv, 11> TITLE_OPTION_VALUE_UV{{{0, 1000, 500, 875}, {0, 875, 500, 750}, {0, 625, 500, 500}, {0, 750, 500, 625},
    {0, 500, 500, 375}, {0, 500, 500, 375}, {0, 375, 500, 250}, {0, 125, 500, 0}, {0, 250, 500, 125}, {500, 875, 1000, 1000}, {500, 750, 1000, 875}}};
inline constexpr std::array<TitleSpriteUv, 28> TITLE_OPTION_LABEL_UV{{{0, 1000, 500, 875}, {0, 875, 500, 750}, {0, 750, 500, 625}, {0, 625, 500, 500},
    {0, 500, 500, 375}, {0, 375, 500, 250}, {0, 250, 500, 125}, {0, 125, 500, 0}, {500, 875, 750, 1000}, {500, 750, 750, 875}, {500, 625, 750, 750},
    {500, 500, 750, 625}, {500, 375, 750, 500}, {500, 250, 750, 375}, {500, 125, 750, 250}, {500, 0, 750, 125}, {750, 875, 1000, 1000}, {750, 750, 1000, 875},
    {750, 625, 1000, 750}, {875, 375, 1000, 500}, {750, 375, 875, 500}, {875, 250, 1000, 375}, {750, 250, 875, 375}, {750, 125, 1000, 250}, {750, 0, 1000, 125},
    {750, 500, 1000, 625}, {750, 125, 1000, 250}, {500, 0, 1000, 125}}};
inline constexpr std::array<TitleSpriteUv, 2> TITLE_OPTION_ICON_UV{{{0, 0, 500, 500}, {500, 0, 1000, 500}}};
inline constexpr std::array<std::int16_t, 3> TITLE_OPTION_TAB_LABELS{0, 1, 2}, TITLE_OPTION_PAGE0_LABELS{0, 2, 1};
inline constexpr std::array<std::int16_t, 2> TITLE_OPTION_PAGE1_LABELS{5, 6}, TITLE_OPTION_COLUMN_X{402, 422};
inline constexpr std::array<std::array<std::int16_t, 2>, 2> TITLE_OPTION_PAGE0_VALUE_LABELS{{{10, 11}, {5, 4}}};
inline constexpr std::array<std::array<std::int16_t, 4>, 2> TITLE_OPTION_COLUMN_MARKERS{{{4, 7, 5, 10}, {5, 9, 4, 8}}};
inline constexpr std::array<std::int8_t, 4> TITLE_OPTION_DIALOGUE_LABELS{6, 8, 7, 9}, TITLE_OPTION_PAGE0_COUNTS{4, 2, 2, 2};
inline constexpr std::array<std::int8_t, 2> TITLE_OPTION_PAGE1_COUNTS{1, 3}, TITLE_OPTION_PAGE2_MODE_LABELS{4, 5};
inline constexpr std::array<std::int16_t, 11> TITLE_OPTION_ITEM_LABELS{3, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14};
inline constexpr std::array<std::array<std::int8_t, 2>, 4> TITLE_OPTION_DIALOGUE_CAPTIONS{{{7, -1}, {9, 8}, {8, -1}, {10, -1}}};
inline constexpr std::array<std::array<std::int8_t, 2>, 3> TITLE_OPTION_ROW_CAPTIONS{{{7, -1}, {11, -1}, {6, -1}}};
inline constexpr std::array<std::array<shen_name, 16>, 4> TITLE_DEBUG_EXCLUDED_AREAS{{
    {SHEN_NAME(MAP0), SHEN_NAME(1000), SHEN_NAME(VIEW)},
        {SHEN_NAME(MAP0), SHEN_NAME(1000), SHEN_NAME(VIEW)},
            {SHEN_NAME(MAP0),
                SHEN_NAME(1000), SHEN_NAME(VIEW), SHEN_NAME(0337)},
                    {SHEN_NAME(MAP0), SHEN_NAME(1000), SHEN_NAME(VIEW)},
                    }};
inline constexpr std::array<const char *, 17> TITLE_DEBUG_SYSTEM_MODULE_NAMES{"Test", "Cold", "Logo", "Hot", "Title", "Option", "Game", "Sample", "Outrun",
    "After Burner", "Hangon", "Harrier", "Credits", "Movie", "OMAKE", "Disk Change", "MOV2OMAKE"};

void SetPressStart(void *);
void DrawTitleEsrb(void *);
void DrawTitleDisc(void *);
void DrawTitleAm2Logo(void *);
void DrawTitleSegaLogo(void *);
void TitleDisplayNullsub(void *);
void DrawTitleYsMark(void *);
void DrawTitleAdditionalLogo(void *);
void DrawTitleWarning41(void *);
void DrawTitleWarning40(void *);
void DrawTitleWarning1(void *);
void CheckTitleDebugInput(void *data);
void MoveToAm2LogoOrYsMark(void *data);
void LoadTitleIntroArea(void *data);
void AcceptTitlePressStart(void *data);
void LoadTitleAttractArea(void *data);
void SkipTitleWarning(void *data);

struct TitleSequenceEntry {
    std::int32_t duration;
    std::int32_t reserved04;
    HLib::TaskCallback control;
    HLib::TaskCallback display;
};
static_assert(sizeof(TitleSequenceEntry) == 0x18);
static_assert(offsetof(TitleSequenceEntry, control) == 8);
static_assert(offsetof(TitleSequenceEntry, display) == 0x10);
inline constexpr std::array<TitleSequenceEntry, 13> TITLE_SEQUENCE_TABLE{{
    {.duration = 150, .control = CheckTitleDebugInput, .display = SetPressStart},
    {.duration = 120, .control = MoveToAm2LogoOrYsMark, .display = DrawTitleEsrb},
    {.duration = 60, .control = MoveToAm2LogoOrYsMark, .display = DrawTitleDisc},
    {.duration = 150, .control = MoveToAm2LogoOrYsMark, .display = DrawTitleAm2Logo},
    {.duration = 90, .control = MoveToAm2LogoOrYsMark, .display = DrawTitleSegaLogo},
    {.duration = 60, .control = LoadTitleIntroArea, .display = TitleDisplayNullsub},
    {.duration = 240, .control = AcceptTitlePressStart, .display = SetPressStart},
    {.duration = 60, .control = LoadTitleAttractArea, .display = TitleDisplayNullsub},
    {.duration = 90, .control = MoveToAm2LogoOrYsMark, .display = DrawTitleYsMark},
    {.duration = 60, .control = MoveToAm2LogoOrYsMark, .display = DrawTitleAdditionalLogo},
    {.duration = 90, .control = SkipTitleWarning, .display = DrawTitleWarning41},
    {.duration = 90, .control = SkipTitleWarning, .display = DrawTitleWarning40},
    {.duration = 90, .control = SkipTitleWarning, .display = DrawTitleWarning1},
}};

struct TitleMenuDispatchEntry {
    HLib::TaskCallback control;
    HLib::TaskCallback display;
    TitleFileSelectState *(*initialize)();
};
static_assert(sizeof(TitleMenuDispatchEntry) == 0x18);

void SetupTitleMenuTask(std::int32_t mode);
void ReturnToTitleControlTasks();
void RequestTitleAttractSkip();
std::int64_t ExecuteTitleMenuCallback(TitleControlData *control, SystemModule module);
void TitleModule(void *taskData);
std::uint16_t InterpretTitleAnalogInput();
bool shenIsInIntro() noexcept;
extern std::uint16_t g_previousTitleInputUserHandle;
extern std::int32_t g_titleFlagShouldRun;
void ResetTitleMenuState();
void SetTitleMenuSelection(std::int32_t selection);
std::int32_t SetTitleSceneInfo(const TitleSceneEntry &scene);
extern std::int32_t g_titleMenuSelection;
extern std::int32_t g_previousTitleMenuSelection;
extern std::int32_t g_titleMenuUIState;
extern std::int64_t (*g_titleMenuTransitionCallback)();
extern std::int32_t g_titleMenuTransitionRequested;
void TitleAudioControl(std::int32_t sceneIndex);
void ApplyTitleMenuOptions();
void SetTitleMenuUIState(std::int32_t state) noexcept;
void SetTitleMenuTransitionCallback(std::int64_t (*callback)()) noexcept;
void RequestTitleMenuTransition() noexcept;
bool SelectTitlePressStart();
void ClearTitlePressStartSelection();
bool BeginTitleOptionsTask();
bool BeginTitlePlatformTask(void (*callback)(void *));
bool ReturnToTitleMenuTask();
bool ReturnToTitlePressStartTask();

void SetTitleLogoPresent();
void HandleSceneTransitionForModule();
const char *GetShenNameString(shen_name name);
std::uint32_t GetTitleVersionOverride() noexcept;
void SetTitleVersionOverride(std::uint32_t value) noexcept;

} // namespace shenmue::s2
