#pragma once
#include "shenmue.h"

#include "s1/hlib.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s1 {

extern std::int32_t g_bIsNewGame;
extern std::uint8_t g_displayActNumber;
extern std::int32_t g_isOnMainMenu;



extern std::int32_t g_mainMenuOpened;
extern std::int32_t g_returnToPressStart;
extern std::uint8_t g_hasPickedSaveData;
extern std::int32_t g_numRemainLoadAttempts;
extern std::int32_t g_is70ManBattleLoad;
extern std::int32_t g_pickedSaveResumeKind;
extern std::int32_t g_settingFlagsCaptured;
extern std::int32_t g_displayCapturedDialogueMode;
extern std::int32_t g_displayCapturedAreaJumpDisabled;
extern std::int32_t g_displayCapturedDefaultTriggerLayout;
extern std::int32_t g_displayCapturedWeatherMode;
extern std::int32_t g_displayDialogueSettingsDirty;
extern std::int32_t g_displayGameplaySettingsDirty;
extern std::int32_t g_70ManBattleLoadBeforeSettings;

std::int32_t DISP_GetTitleState() noexcept;
bool DISP_IsOnMainMenu() noexcept;
struct ScenarioContext;

#if SHENMUE_IS_DEMO
enum class SystemModule : std::uint32_t;
struct TrialTitleWork {
    HLib::Task *film;
    std::int32_t attractDelay;
    std::int32_t fadeDelay;
};
static_assert(offsetof(TrialTitleWork, attractDelay) == sizeof(void *));
static_assert(sizeof(TrialTitleWork) == (SHENMUE_IS_DC ? 0x0c : 0x10));

struct TrialTitleQuad {
    float x;
    float y;
    std::int16_t width;
    std::int16_t height;
    std::int16_t texture;
    std::int16_t alternate;
    float depth;
};
static_assert(sizeof(TrialTitleQuad) == 20);
static_assert(offsetof(TrialTitleQuad, texture) == 12);
static_assert(offsetof(TrialTitleQuad, depth) == 16);
inline constexpr std::array<TrialTitleQuad, 15> kTrialMainMenuQuads{{
    {.x = 128.0f, .y = 68.0f, .width = 256, .height = 128, .texture = 64, .alternate = 71, .depth = -7.0f},
    {.x = 458.0f, .y = 72.0f, .width = 512, .height = 64, .texture = 60, .alternate = 63, .depth = -7.0f},
    {.x = 104.0f, .y = 160.0f, .width = 256, .height = 128, .texture = 66, .alternate = 71, .depth = -7.0f},
    {.x = 408.0f, .y = 160.0f, .width = 512, .height = 128, .texture = 67, .alternate = 71, .depth = -7.0f},
    {.x = 104.0f, .y = 276.0f, .width = 256, .height = 128, .texture = 66, .alternate = 71, .depth = -7.0f},
    {.x = 408.0f, .y = 276.0f, .width = 512, .height = 128, .texture = 67, .alternate = 71, .depth = -7.0f},
    {.x = 104.0f, .y = 392.0f, .width = 256, .height = 128, .texture = 66, .alternate = 71, .depth = -7.0f},
    {.x = 408.0f, .y = 392.0f, .width = 512, .height = 128, .texture = 67, .alternate = 71, .depth = -7.0f},
    {.x = 72.0f, .y = 152.0f, .width = 128, .height = 128, .texture = 57, .alternate = 71, .depth = -6.0f},
    {.x = 122.0f, .y = 160.0f, .width = 128, .height = 128, .texture = 58, .alternate = 71, .depth = -6.0f},
    {.x = 408.0f, .y = 160.0f, .width = 512, .height = 128, .texture = 24, .alternate = 25, .depth = -6.0f},
    {.x = 104.0f, .y = 276.0f, .width = 128, .height = 128, .texture = 70, .alternate = 71, .depth = -6.0f},
    {.x = 408.0f, .y = 276.0f, .width = 512, .height = 128, .texture = 52, .alternate = 53, .depth = -6.0f},
    {.x = 104.0f, .y = 392.0f, .width = 128, .height = 128, .texture = 12, .alternate = 71, .depth = -6.0f},
    {.x = 408.0f, .y = 392.0f, .width = 512, .height = 128, .texture = 35, .alternate = 36, .depth = -6.0f},
}};

inline constexpr std::array<TrialTitleQuad, 26> kTrialAreaMenuQuads{{
    {.x = 144.0f, .y = 262.0f, .width = 256, .height = 212, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 496.0f, .y = 262.0f, .width = 256, .height = 212, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 320.0f, .y = 144.0f, .width = 256, .height = 212, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 320.0f, .y = 380.0f, .width = 256, .height = 212, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 156.0f, .y = 56.0f, .width = 512, .height = 64, .texture = 13, .alternate = 13, .depth = -7.0f},
    {.x = 458.0f, .y = 56.0f, .width = 512, .height = 64, .texture = 60, .alternate = 63, .depth = -7.0f},
    {.x = 192.0f, .y = 262.0f, .width = 128, .height = 128, .texture = 0, .alternate = 71, .depth = -6.0f},
    {.x = 544.0f, .y = 262.0f, .width = 128, .height = 128, .texture = 0, .alternate = 71, .depth = -6.0f},
    {.x = 368.0f, .y = 144.0f, .width = 128, .height = 128, .texture = 0, .alternate = 71, .depth = -6.0f},
    {.x = 368.0f, .y = 380.0f, .width = 128, .height = 128, .texture = 0, .alternate = 71, .depth = -6.0f},
    {.x = 192.0f, .y = 254.0f, .width = 128, .height = 128, .texture = 57, .alternate = 71, .depth = -5.0f},
    {.x = 544.0f, .y = 254.0f, .width = 128, .height = 128, .texture = 54, .alternate = 71, .depth = -5.0f},
    {.x = 368.0f, .y = 144.0f, .width = 128, .height = 128, .texture = 58, .alternate = 71, .depth = -5.0f},
    {.x = 368.0f, .y = 380.0f, .width = 128, .height = 128, .texture = 59, .alternate = 71, .depth = -5.0f},
    {.x = 86.0f, .y = 238.0f, .width = 128, .height = 32, .texture = 17, .alternate = 71, .depth = -5.0f},
    {.x = 448.0f, .y = 238.0f, .width = 128, .height = 32, .texture = 22, .alternate = 71, .depth = -5.0f},
    {.x = 280.0f, .y = 120.0f, .width = 128, .height = 32, .texture = 47, .alternate = 71, .depth = -5.0f},
    {.x = 272.0f, .y = 356.0f, .width = 128, .height = 32, .texture = 50, .alternate = 71, .depth = -5.0f},
    {.x = 104.0f, .y = 296.0f, .width = 128, .height = 32, .texture = 38, .alternate = 39, .depth = -5.0f},
    {.x = 484.0f, .y = 296.0f, .width = 256, .height = 32, .texture = 31, .alternate = 31, .depth = -5.0f},
    {.x = 280.0f, .y = 182.0f, .width = 128, .height = 32, .texture = 40, .alternate = 41, .depth = -5.0f},
    {.x = 280.0f, .y = 416.0f, .width = 128, .height = 32, .texture = 43, .alternate = 44, .depth = -5.0f},
    {.x = 288.0f, .y = 262.0f, .width = 64, .height = 64, .texture = 3, .alternate = 6, .depth = -5.0f},
    {.x = 352.0f, .y = 262.0f, .width = 64, .height = 64, .texture = 5, .alternate = 8, .depth = -5.0f},
    {.x = 320.0f, .y = 232.0f, .width = 64, .height = 64, .texture = 4, .alternate = 7, .depth = -5.0f},
    {.x = 320.0f, .y = 288.0f, .width = 64, .height = 64, .texture = 10, .alternate = 9, .depth = -5.0f},
}};

inline constexpr std::array<TrialTitleQuad, 31> kTrialOptionsQuads{{
    {.x = 164.0f, .y = 64.0f, .width = 256, .height = 64, .texture = 56, .alternate = 71, .depth = -7.0f},
    {.x = 463.0f, .y = 64.0f, .width = 512, .height = 64, .texture = 61, .alternate = 62, .depth = -7.0f},
    {.x = 170.0f, .y = 156.0f, .width = 464, .height = 112, .texture = 68, .alternate = 71, .depth = -7.0f},
    {.x = 170.0f, .y = 274.0f, .width = 464, .height = 112, .texture = 68, .alternate = 71, .depth = -7.0f},
    {.x = 170.0f, .y = 392.0f, .width = 464, .height = 112, .texture = 68, .alternate = 71, .depth = -7.0f},
    {.x = 170.0f, .y = 156.0f, .width = 128, .height = 128, .texture = 55, .alternate = 71, .depth = -6.0f},
    {.x = 170.0f, .y = 274.0f, .width = 256, .height = 128, .texture = 11, .alternate = 71, .depth = -6.0f},
    {.x = 170.0f, .y = 392.0f, .width = 128, .height = 128, .texture = 14, .alternate = 71, .depth = -6.0f},
    {.x = 168.0f, .y = 156.0f, .width = 128, .height = 32, .texture = 45, .alternate = 71, .depth = -5.0f},
    {.x = 170.0f, .y = 274.0f, .width = 256, .height = 128, .texture = 19, .alternate = 20, .depth = -5.0f},
    {.x = 170.0f, .y = 368.0f, .width = 128, .height = 32, .texture = 26, .alternate = 27, .depth = -5.0f},
    {.x = 396.0f, .y = 156.0f, .width = 128, .height = 32, .texture = 32, .alternate = 71, .depth = -6.0f},
    {.x = 568.0f, .y = 156.0f, .width = 128, .height = 32, .texture = 46, .alternate = 71, .depth = -6.0f},
    {.x = 396.0f, .y = 274.0f, .width = 64, .height = 32, .texture = 15, .alternate = 71, .depth = -6.0f},
    {.x = 568.0f, .y = 274.0f, .width = 64, .height = 32, .texture = 18, .alternate = 71, .depth = -6.0f},
    {.x = 480.0f, .y = 376.0f, .width = 128, .height = 32, .texture = 28, .alternate = 29, .depth = -6.0f},
    {.x = 396.0f, .y = 156.0f, .width = 128, .height = 128, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 466.0f, .y = 156.0f, .width = 96, .height = 96, .texture = 3, .alternate = 6, .depth = -7.0f},
    {.x = 496.0f, .y = 156.0f, .width = 96, .height = 96, .texture = 5, .alternate = 8, .depth = -7.0f},
    {.x = 568.0f, .y = 156.0f, .width = 128, .height = 128, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 348.0f, .y = 392.0f, .width = 128, .height = 128, .texture = 3, .alternate = 6, .depth = -7.0f},
    {.x = 480.0f, .y = 392.0f, .width = 240, .height = 160, .texture = 69, .alternate = 69, .depth = -7.0f},
    {.x = 612.0f, .y = 392.0f, .width = 128, .height = 128, .texture = 5, .alternate = 8, .depth = -7.0f},
    {.x = 170.0f, .y = 396.0f, .width = 128, .height = 32, .texture = 21, .alternate = 71, .depth = -6.0f},
    {.x = 430.0f, .y = 408.0f, .width = 64, .height = 32, .texture = 34, .alternate = 71, .depth = -6.0f},
    {.x = 520.0f, .y = 408.0f, .width = 64, .height = 32, .texture = 34, .alternate = 71, .depth = -6.0f},
    {.x = 172.0f, .y = 424.0f, .width = 256, .height = 32, .texture = 48, .alternate = 49, .depth = -6.0f},
    {.x = 396.0f, .y = 274.0f, .width = 128, .height = 128, .texture = 65, .alternate = 69, .depth = -7.0f},
    {.x = 466.0f, .y = 274.0f, .width = 96, .height = 96, .texture = 3, .alternate = 6, .depth = -7.0f},
    {.x = 496.0f, .y = 274.0f, .width = 96, .height = 96, .texture = 5, .alternate = 8, .depth = -7.0f},
    {.x = 568.0f, .y = 274.0f, .width = 128, .height = 128, .texture = 65, .alternate = 69, .depth = -7.0f},
}};

inline constexpr shen_name kTrialTitleToken = make_name('T', 'N', 'T', 'L');
inline constexpr shen_name kTrialSystemTitleToken = make_name('S', 'L', 'T', 'L');
inline constexpr shen_name kTrialAreaMain = make_name('T', 'I', 'K', 'N');
inline constexpr std::array<shen_name, 4> kTrialTitleAreas{
    make_name('H', 'G', 'Y', 'F'), make_name('H', 'G', 'M', 'F'), make_name('H', 'G', 'R', 'F'), make_name('H', 'G', 'S', 'F')};
inline constexpr std::int32_t kTrialDialogueGlyphs[4][3]{{21, 34, 33}, {42, 34, 16}, {23, 34, 34}, {51, 33, 34}};

extern std::uint8_t g_trialTitleState;
extern std::int8_t g_trialTitleSelection;
extern std::int8_t g_trialTitleAreaSelection;
extern std::int8_t g_trialTitleOptionSelection;
extern std::int8_t g_trialTitleAudioOption;
extern std::int8_t g_trialTitleAreaJumpOption;
extern std::int8_t g_trialTitleDialogueOption;
extern std::int8_t g_trialTitlePreviousDialogueOption;
extern std::uint16_t g_trialTitlePressed;
extern std::uint16_t g_trialTitleHeld;
extern std::uint16_t g_trialTitleRepeated;
extern std::int16_t g_trialTitleRepeatDelay;
extern std::uint32_t g_trialTitleCursorColour;
extern std::int32_t g_trialTitleCursorAlpha;
extern std::int32_t g_trialTitleCursorDelta;
extern std::array<std::int32_t, 71> g_trialSystemTitleTextures;
extern SystemModule g_trialTitleNextModule;
extern std::int32_t g_trialTitlePromptAlpha;
extern std::int32_t g_trialTitleAlternateAlpha;
extern std::uint32_t g_trialTitlePromptColour;
extern std::uint32_t g_trialTitleAlternateColour;
extern std::int16_t g_trialTitleInputDelay;
extern std::int16_t g_trialTitleBlinkTicks;
extern std::uint8_t g_trialTitleBlinkPhase;
extern std::array<std::int32_t, 3> g_trialTitleTextures;
void TrialTitle_ResetBlink(std::int8_t reverse);
void TrialTitle_DrawPressStart();
void TrialTitle_ResetState();
void TrialTitle_ResetGame();
void TrialTitle_Module(HLib::TaskData *taskData);
bool TrialTitle_DrawFrontend(std::uint32_t renderTarget);
void TrialTitle_FadeIn(HLib::TaskData *taskData);
void TrialTitle_FadeOut(HLib::TaskData *taskData);
void TrialTitle_PressStart(HLib::TaskData *taskData);
void TrialTitle_EnterMenu(HLib::TaskData *taskData);
void TrialTitle_Menu(HLib::TaskData *taskData);
void TrialTitle_AreaSelect();
void TrialTitle_Options();
void TrialTitle_DrawOptions();
void TrialTitle_Exit(HLib::TaskData *taskData);
#endif

void InternalInit();
void DISP_Module(HLib::TaskData *taskData);
std::int64_t LoadIntro();
void GetSaveSlotString();
void LoadSceneForSaveFile(std::int32_t actNumber);
void HLTaskFunc_InternalWeatherInit(std::int32_t mode);
void Script_cdttUtil(ScenarioContext *context, void *rawParams);
void Script_CtrlTimeLimit(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
