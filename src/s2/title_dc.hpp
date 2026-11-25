#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "s2/omake_selection.h"
#include "s2/hlib.hpp"
#include "s2/title_area_data.hpp"
#include "s2/backup_hotplug_dc.hpp"
#include "s2/system_modules.hpp"
#include <stddef.h>

namespace shenmue {
namespace s2 {

extern OmakeSelection omakeSelection;

struct TitleDisplayData {
    std::byte retained[0x58];
    float currentStateFrame;
    float stateFrame;
    std::uint32_t frameCount;
};

struct TitleControlData {
    HLib::Task *titleSequenceTask;
    TitleDisplayData *display;
    std::int32_t retainedState;
    std::int32_t state;
    std::byte inheritedSaveBank[0xa000];
};

struct TitleModuleWork {
    std::int32_t frames;
    HLib::Task *screenFilmTask;
};

typedef char Ownertitle_dchppLayoutCheck0[(sizeof(TitleDisplayData) == 0x64) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck1[(offsetof(TitleDisplayData, currentStateFrame) == 0x58) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck2[(offsetof(TitleDisplayData, stateFrame) == 0x5c) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck3[(offsetof(TitleDisplayData, frameCount) == 0x60) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck4[(sizeof(TitleControlData) == 0xa010) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck5[(offsetof(TitleControlData, display) == 4) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck6[(offsetof(TitleControlData, state) == 0x0c) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck7[(offsetof(TitleControlData, inheritedSaveBank) == 0x10) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck8[(sizeof(TitleModuleWork) == 8) ? 1 : -1];

#define TITLE_AREA SHEN_NAME(TITL)
#define TITLE_CONTROL_TASK SHEN_NAME(TCRL)
#define TITLE_DISPLAY_TASK SHEN_NAME(DISP)
#define TITLE_SOUND_DATA_TASK SHEN_NAME(TTSD)
static const std::int16_t titleSequenceDurations[13] = {150, 60, 240, 60, 60, 60, 60, 60, 60, 90, 90, 90, 90};

struct TitleCallbackPair {
    HLib::TaskCallback control;
    HLib::TaskCallback display;
};
typedef char Ownertitle_dchppLayoutCheck9[(sizeof(TitleCallbackPair) == 8) ? 1 : -1];

void CheckTitleDebugInput(void *data);
void AcceptTitlePressStart(void *data);
void TitleControlNullsub(void *data);
void LoadTitleIntroArea(void *data);
void LoadTitleAttractArea(void *data);
void SkipTitleWarning(void *data);
void DebugTitleControl(void *data);
void TitleMenuControl(void *data);
void DrawTitleMenu(void *data);
static const HLib::TaskCallback titleSequenceControlCallbacks[13] = {CheckTitleDebugInput, LoadTitleIntroArea, AcceptTitlePressStart, TitleControlNullsub,
    TitleControlNullsub, TitleControlNullsub, LoadTitleAttractArea, TitleControlNullsub, TitleControlNullsub, SkipTitleWarning, SkipTitleWarning,
    SkipTitleWarning, SkipTitleWarning};

void SetPressStart(void *data);
void TitleDisplayNullsub(void *data);
void DrawTitleAm2Logo(void *data);
void DrawTitleSegaLogo(void *data);
void DrawTitleDisc(void *data);
void DrawTitleYsMark(void *data);
void DrawTitleAdditionalLogo(void *data);
void DrawTitleWarning41(void *data);
void DrawTitleWarning40(void *data);
void DrawTitleWarning1(void *data);
void DrawTitleWarning43(void *data);
static const HLib::TaskCallback titleDisplayCallbacks[13] = {SetPressStart, TitleDisplayNullsub, SetPressStart, DrawTitleAm2Logo, DrawTitleSegaLogo,
    DrawTitleDisc, TitleDisplayNullsub, DrawTitleYsMark, DrawTitleAdditionalLogo, DrawTitleWarning41, DrawTitleWarning40, DrawTitleWarning1,
    DrawTitleWarning43};
static const TitleCallbackPair titleSecondaryCallbacks[2] = {
    {SHEN_FIELD(control) DebugTitleControl, SHEN_FIELD(display) SetPressStart}, {SHEN_FIELD(control) TitleMenuControl, SHEN_FIELD(display) DrawTitleMenu}};

extern HLib::Task *titleControlTask;
extern std::uint32_t logoFlags;
extern std::int32_t titleLoopSequence, needsTitleSequenceDraw, setPressStartPending, titleSequenceTime;
extern std::int32_t titleDebugSelection, titleDebugAreaCharacter, titleCurrentCoreModule;
extern std::int32_t titleWarningLogo, titleWarningUsesLogoParameters;
extern std::uint8_t titleWarningLogoParameters[7];
extern std::int32_t g_titleMenuSelection, g_previousTitleMenuSelection, g_titleMenuUIState, titleWheelRotation;
enum TitleOption { TitleOptionDialogue, TitleOptionAudio, TitleOptionTrigger, TitleOptionControl };
extern BackupHotplugData *titleBackupData;
extern std::int32_t titleDebugSaveSlot;
static const std::uint16_t kTitleStartButton = 0x0800, kTitleSkipButtons = 0x0600;

void TitleControl(void *data);
void NativeTitleNoOp(void *data);
void TitleModule(void *data);
void ResetMapRenderState();
void ResetTitleMenuState();
void SetTitleLogoPresent();
extern std::int16_t titleSelectedAct;
extern std::int32_t titleDebugAct, titleCurrentAreaIndex;
void SetTitleSceneInfo(const TitleSceneEntry &scene);
void InitializeTitleAreaSelection();
std::int32_t SetTitleMenuUIState(std::int32_t mode);
std::int32_t GetTitleMenuUIState();
void InitializeTitleWarnings();
void SetTitleWarningLogo(std::int32_t logo, const void *parameters);
void FreeTitleWarningResources();
void InitShotsImages();
void LoadAreaSpriteImages();
void ResetTitleMenuSelection();
void TitleBackupCallback(void *data);
void SM_Init_Enqueue(std::int32_t initializationMode);
void QueueSoundProgramControl();
bool LoadTitleSoundData();
void LoadTitleSoundProgram(std::int32_t sceneIndex);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct TitleDebugModule {
    const char *name;
    SystemModule module;
    void (*control)();
    std::int32_t (*initialize)();
};

struct TitleMenuDispatchEntry {
    HLib::TaskCallback control;
    HLib::TaskCallback display;
    std::int16_t uiState;
    std::int16_t sound;
};

struct TitleMenuRequest {
    std::int32_t action;
    std::int32_t callback;
};

typedef char Ownertitle_dchppLayoutCheck10[(sizeof(TitleDebugModule) == 0x10) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck11[(offsetof(TitleDebugModule, control) == 8) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck12[(sizeof(TitleMenuDispatchEntry) == 0x0c) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck13[(offsetof(TitleMenuDispatchEntry, uiState) == 8) ? 1 : -1];
typedef char Ownertitle_dchppLayoutCheck14[(sizeof(TitleMenuRequest) == 8) ? 1 : -1];

extern const TitleDebugModule titleDebugModules[9];
extern const TitleMenuDispatchEntry titleMenuDispatch[6];
extern const TitleMenuRequest titleMenuRequests[5];
extern const TitleMenuRequest *titlePendingMenuRequest;
extern std::int32_t titleMenuAction;
extern std::int32_t titleOptionsSelection;
extern std::int32_t titleOptionValues[4];
extern std::int32_t titleTimeAttackAlpha, titleTimeAttackAlphaStep;
extern std::int8_t titleDebugSaveLoaded;

std::int32_t FindLastTitleSceneGroup(const TitleSceneGroup *groups);
std::int32_t FindLastTitleArea(const TitleSceneEntry *scenes);
void SelectTitleSceneGroup(const TitleSceneGroup *groups);
void SelectTitleArea(const TitleSceneEntry *scenes);
void ControlTitleDebugSceneSelector();
void ControlTitleDebugTimeAttack();
std::int32_t InitializeTitleDebugGame();
void SetTitleDebugStartTime(shen_name area);
void ApplyTitleMenuOptions(std::int32_t unused = 0);
std::int32_t InitializeTitleDebugModule();
void ControlTitleOptions(void *data);
void DrawTitleOptions(void *data);
void InitializeTitleLoadSelection(void *data);
void InitializeTitleNewGameSelection(void *data);
void DrawTitleSaveSelection(void *data);
void ControlTitleNewGameConfirmation(void *data);
void DrawTitleNewGameConfirmation(void *data);
void ControlTitleMissingContinue(void *data);
void DrawTitleMissingContinue(void *data);
std::int32_t HasTitleContinueSave();
void BeginTitleModule(SystemModule module, std::int32_t unpause);
void RestoreTitleCallbacks(std::int32_t menu);
std::int32_t SetTitleMenuAction(std::int32_t action);
std::int32_t GetTitleMenuAction();
std::int32_t ExecuteTitleMenuCallback(TitleControlData *control, std::int32_t callback);
#endif

} // namespace s2
} // namespace shenmue

#endif
