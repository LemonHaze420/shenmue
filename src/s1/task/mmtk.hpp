#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;
struct PadCaptureEntry;
struct CVWKRuntimeSlot;
struct PackModel;
struct Mt5RuntimeNode;


enum class NotebookTaskState : std::uint8_t {
    OpenIdle = 1,
    BeginTurnLeft = 2,
    BeginTurnRight = 3,
    TurnLeftPage = 4,
    TurnRightPage = 5,
    Teardown = 6,
    Opening = 7,
    InactiveClosed = 8,
    Closing = 9,
};


struct NotebookTextureEntry {
    std::int32_t dataOffset;
    std::uint32_t idLow;
    std::uint32_t idHigh;
};
static_assert(sizeof(NotebookTextureEntry) == 12);

struct NotebookPresentationSnapshot {
    NotebookTaskState state{NotebookTaskState::InactiveClosed};
    std::uint16_t currentPage{};
    std::uint16_t lastEntryPage{};
    std::uint32_t openCount{};
    std::uint32_t closeCount{};
    std::uint8_t active{};
};


extern std::uint8_t g_notebookHelpOverrideActive;

extern std::uint16_t g_notebookLastEntryPage;
extern std::uint16_t g_notebookPersistenceWord;

extern CVWKRuntimeSlot *g_notebookJingleSubtitle;

extern std::int32_t g_hudPromptJinguruIconOverrideActive;

extern std::array<std::byte *, 4> g_notebookMemoTextureChunks;

extern std::array<std::int32_t, 4> g_notebookMemoSurfaces;

extern std::int32_t g_notebookRequestedEntryPage;

HLib::Task *MMTK_Init();
void MMTK_ResetForEnqueue();
void MMTK_Shutdown();
void MMTK_Callback(HLib::TaskData *taskData);
void MMTK_Callback_0(HLib::TaskData *taskData);
HLib::Task *MMTK_GetTask();
PadCaptureEntry *MMTK_GetInputEntry();
bool MMTK_CanOpenFromFreeRoam();

void shenBeginNotebook();
void shenEndNotebook();
void CloseNotebook();
bool ProcessNotebookStates();
void PrepareBothNotebookPages(std::int16_t page, std::uint32_t leftTexture, std::uint32_t rightTexture);
void MMTK_BeginManualSession();

bool MMTK_IsNotebookActive();
bool MMTK_HasScriptSession();

bool MMTK_HasManualSession();
NotebookTaskState MMTK_GetNotebookState();
std::uint16_t MMTK_GetLastEntryPage();
void MMTK_SetLastEntryPage(std::uint16_t page);
void MMTK_UpdateEntryForFlag(std::uint16_t flag);
std::uint16_t MMTK_GetPersistenceWord();
void MMTK_SetPersistenceWord(std::uint16_t value);
NotebookPresentationSnapshot MMTK_GetPresentationSnapshot();

// Current Shenmue scenario-function ABI adapter for Script_memoToggle.
void MMTK_ScriptMemoToggle(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
