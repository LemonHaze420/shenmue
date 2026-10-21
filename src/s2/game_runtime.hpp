#pragma once

#include "s2/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct EventTaskEntry {
    HLib::Task *task;
    std::int32_t previous;
    std::int32_t next;
};

static_assert(sizeof(EventTaskEntry) == 0x10);

enum class FrameTimerSlot : std::uint32_t {
    MainLoop,
    Update,
    Render,
    Animation,
};

enum class FrameTimeOp : std::uint32_t {
    Reset,
    Accumulate,
    Start,
    Delta,
};

std::uint32_t GetAccumulatedFrameTimes(FrameTimerSlot slot, FrameTimeOp operation);

void InitEventTaskQueue();
std::int64_t RegisterEventTask(HLib::Task *task);
std::int64_t UnregisterEventTask(HLib::Task *task);
void CleanupEventTaskQueue();

extern std::int32_t g_gameplayGlobalState;
void SetGlobalState(std::int32_t state);

void ResetSavedFrameCounter() noexcept;
void InitializeFrameVars();
void IncrementFrameCount();
void SetRequestModuleChange(std::int32_t requested);


void IncrementSteamStat(std::int32_t stat);
void CheckSteamStats();

void ReadSm2Saves(std::int32_t slot);
extern std::array<EventTaskEntry, 64> g_eventTasks;
extern std::int32_t g_eventTaskFirst;
extern std::int32_t g_eventTaskLast;
extern std::int32_t g_eventTaskCount;
extern std::uint32_t g_totalFrameCount;
extern std::uint32_t g_rootTaskIterationCount;
extern std::int32_t g_authorFrameCount;
extern bool g_requestModuleChange;
extern std::uint32_t g_previousMainLoopFrameTime;
extern std::uint32_t g_postPresentMainLoopFrameTime;
extern float g_accumulatedFrameTimeDelta;


struct EyeRigParameters {
    float lidYawScale;
    float lidPitchScale;
    float yawLimit;
    float pitchLimit;
};

static_assert(sizeof(EyeRigParameters) == 0x10);


extern float g_gameTimeMultiplier;
extern std::int32_t g_importAnyShenmue1Save;
extern std::array<std::int32_t, 2> g_frameWaitEnabled;
extern std::int32_t g_shouldHashRotationMatrix;
extern std::int32_t g_archiveRemapCheckEnabled;
extern std::int32_t g_forceDisplayCursor;
extern std::int32_t g_uiDebugSuspendElements;
extern std::int32_t g_debugUiDisabled;
extern std::int32_t g_debugInputGlyphMode;
extern std::int32_t g_notebookDebugMode;
extern std::int32_t g_debugImGuiRenderMode;
extern EyeRigParameters g_eyeRigParameters;
extern float g_eyeControlSmoothingRate;

void InitializeDebugDefaults();

} // namespace shenmue::s2
