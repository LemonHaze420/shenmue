#pragma once
#include <cstdint>
#include <array>
#include <cstddef>
#include "s2/hlib.hpp"
#include "s2/title_area_data.hpp"
#include "common/types.hpp"

namespace shenmue::s2 {
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
    std::byte reserved20[0xa010];
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
static_assert(sizeof(TitleModuleWork) == 0x10);
static_assert(offsetof(TitleModuleWork, screenFilmTask) == 8);
inline constexpr std::array<std::array<std::int16_t, 2>, 5> TITLE_LOGO_DIMENSIONS{{
    {512, 128},
    {512, 512},
    {256, 128},
    {128, 128},
    {256, 256},
}};

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

void TitleModule(void *taskData);
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
void SetTitleMenuUIState(std::int32_t state) noexcept;
void SetTitleMenuTransitionCallback(std::int64_t (*callback)()) noexcept;
void RequestTitleMenuTransition() noexcept;
bool SelectTitlePressStart();
bool BeginTitleOptionsTask();
bool BeginTitlePlatformTask(void (*callback)(void *));
bool ReturnToTitleMenuTask();
bool ReturnToTitlePressStartTask();

void SetTitleLogoPresent();
std::uint32_t GetTitleVersionOverride() noexcept;
void SetTitleVersionOverride(std::uint32_t value) noexcept;

} // namespace shenmue::s2
