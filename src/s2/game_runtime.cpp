#include "s2/game_runtime.hpp"
#include "d3t/object_store.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/ui.hpp"
#include "platform/performance_counter.hpp"

#include <algorithm>
#include <cmath>

namespace shenmue::s2 {

std::array<EventTaskEntry, 64> g_eventTasks{};
std::int32_t g_eventTaskFirst = 0;
std::int32_t g_eventTaskLast = 0;
std::int32_t g_eventTaskCount = 0;
std::uint32_t g_totalFrameCount = 0;
std::uint32_t g_rootTaskIterationCount = 0;
std::int32_t g_authorFrameCount = -1;
bool g_requestModuleChange = false;
std::uint32_t g_previousMainLoopFrameTime{};
std::uint32_t g_postPresentMainLoopFrameTime{};
float g_accumulatedFrameTimeDelta{};

std::int32_t g_gameplayGlobalState = -1;

float g_gameTimeMultiplier{};
std::int32_t g_importAnyShenmue1Save{};
std::array<std::int32_t, 2> g_frameWaitEnabled{};
std::int32_t g_shouldHashRotationMatrix{};
std::int32_t g_archiveRemapCheckEnabled{};
std::int32_t g_forceDisplayCursor{};
std::int32_t g_uiDebugSuspendElements{};
std::int32_t g_debugUiDisabled{};
std::int32_t g_debugInputGlyphMode{};
std::int32_t g_notebookDebugMode{};
std::int32_t g_debugImGuiRenderMode{};
EyeRigParameters g_eyeRigParameters{};
float g_eyeControlSmoothingRate{};

namespace {


float defaultInputFactor{};
std::int32_t unusedDebugTimeFlag{};
std::uint64_t debugPointerState{};
std::array<std::int32_t, 7> debugEnableFlags{};
std::array<std::int32_t, 5> debugClearedState{};
std::int32_t remakeRenderUI{};
std::array<float, 3> debugScale{};
std::int32_t debugFaceViseme{};

} // namespace



// @verified - p
void InitializeDebugDefaults() {
    g_gameTimeMultiplier = 1.0f;
    g_gameHangOnSteeringResponse = 0.9f;
    defaultInputFactor = 0.75f;
    g_importAnyShenmue1Save = 1;
    unusedDebugTimeFlag = 0;
    debugPointerState = 0;
    d3t::g_hashModelRequireAssignedNodes = 1;
    g_frameWaitEnabled[0] = 1;
    g_frameWaitEnabled[1] = 1;
    g_shouldHashRotationMatrix = 1;
    debugEnableFlags[0] = 1;
    debugEnableFlags[1] = 1;
    debugEnableFlags[2] = 1;
    debugEnableFlags[3] = 1;
    waitForRendererReadiness = true;
    debugEnableFlags[4] = 1;
    minimumUnlockedWaitSeconds = 0;
    maximumUnlockedWaitSeconds = 30;
    g_skipArchiveReadWait = 1;
    g_archiveRemapCheckEnabled = 1;
    debugClearedState = {};
    debugEnableFlags[5] = 1;
    debugEnableFlags[6] = 1;
    g_forceDisplayCursor = 0;
    g_uiDebugSuspendElements = 0;
    g_debugUiDisabled = 1;
    g_debugInputGlyphMode = 0;
    g_notebookDebugMode = 0;
    g_debugImGuiRenderMode = 1;
    requestedCameraIconMode = 2;
    remakeRenderUI = 0;
    for (float &scale : debugScale)
        scale = 1.0f;
    g_eyeRigParameters = {.lidYawScale = 0.09f, .lidPitchScale = 0.15f, .yawLimit = 0.395f, .pitchLimit = 0.34f};
    debugFaceViseme = 0;
    g_eyeControlSmoothingRate = 10.0f;
}

// @verified - p: SetGlobalState.
void SetGlobalState(std::int32_t state) {
    g_gameplayGlobalState = state;
}

namespace {

std::uint8_t frameCounterEnabled = 0;
std::array<float, 4> accumulatedFrameTimes{};
std::array<float, 4> startedFrameTimes{};

} // namespace

std::uint32_t GetAccumulatedFrameTimes(FrameTimerSlot slot, FrameTimeOp operation) {


    static const float counterScale = 12500000.0f / static_cast<float>(platform::performanceCounterFrequency());
    const float counterTime = static_cast<float>(platform::performanceCounter()) * counterScale;
    const float time = static_cast<float>(std::fmod(static_cast<double>(counterTime), 4294967295.0));
    const std::size_t index = static_cast<std::size_t>(slot);
    switch (operation) {
    case FrameTimeOp::Reset:
        accumulatedFrameTimes[index] = time;
        return 0;
    case FrameTimeOp::Accumulate:
        accumulatedFrameTimes[index] += time - startedFrameTimes[index];
        break;
    case FrameTimeOp::Start:
        startedFrameTimes[index] = time;
        break;
    case FrameTimeOp::Delta:
        return static_cast<std::uint32_t>(static_cast<std::int64_t>(accumulatedFrameTimes[index] - time));
    }


    return static_cast<std::uint32_t>(static_cast<std::int64_t>(time));
}

void InitEventTaskQueue() {
    g_eventTasks = {};
    g_eventTaskFirst = 0;
    g_eventTaskLast = 0;
    g_eventTaskCount = 0;
}

std::int64_t RegisterEventTask(HLib::Task *task) {
    if (!task)
        return 0;

    if (g_eventTaskCount == 0) {
        g_eventTasks[0] = {task, -1, -1};
        g_eventTaskFirst = 0;
        g_eventTaskLast = 0;
        g_eventTaskCount = 1;
        return 0;
    }

    const auto found = std::find_if(g_eventTasks.begin(), g_eventTasks.end(), [](const EventTaskEntry &entry) { return entry.task == nullptr; });
    if (found == g_eventTasks.end()) {
        return 0;
    }
    const std::int32_t index = static_cast<std::int32_t>(std::distance(g_eventTasks.begin(), found));
    found->task = task;
    found->previous = g_eventTaskLast;
    found->next = -1;
    g_eventTasks[static_cast<std::size_t>(g_eventTaskLast)].next = index;
    g_eventTaskLast = index;
    ++g_eventTaskCount;
    return 1;
}

std::int64_t UnregisterEventTask(HLib::Task *task) {
    if (!task || g_eventTaskCount == 0) {
        return 0;
    }
    const auto found = std::find_if(g_eventTasks.begin(), g_eventTasks.end(), [task](const EventTaskEntry &entry) { return entry.task == task; });
    if (found == g_eventTasks.end()) {
        return 0;
    }
    const std::int32_t index = static_cast<std::int32_t>(std::distance(g_eventTasks.begin(), found));
    const std::int32_t previous = found->previous;
    const std::int32_t next = found->next;
    found->task = nullptr;
    if (previous != -1) {
        g_eventTasks[static_cast<std::size_t>(previous)].next = next;
    }
    if (next != -1) {
        g_eventTasks[static_cast<std::size_t>(next)].previous = previous;
    }
    if (index == g_eventTaskLast) {
        g_eventTaskLast = previous != -1 ? previous : g_eventTaskLast;
    }
    if (index == g_eventTaskFirst) {
        g_eventTaskFirst = next != -1 ? next : g_eventTaskFirst;
    }
    --g_eventTaskCount;
    return 1;
}

void CleanupEventTaskQueue() {
    std::int32_t index = g_eventTaskFirst;
    for (std::int32_t remaining = g_eventTaskCount; remaining > 0 && index >= 0; --remaining) {
        EventTaskEntry &entry = g_eventTasks[static_cast<std::size_t>(index)];
        const std::int32_t next = entry.next;
        if (entry.task)
            HLib::DestroyTask(entry.task);

        index = next;
    }
    InitEventTaskQueue();
}

// @verified - p
void ResetSavedFrameCounter() noexcept {
    frameCounterEnabled = 0;
    g_totalFrameCount = 0;
}

void InitializeFrameVars() {
    frameCounterEnabled = 1;
    g_totalFrameCount = 0;
}

void IncrementSteamStat(std::int32_t) {
}
// Steam is excluded middleware; retain the game-owned call boundary without a Steam backend.
void CheckSteamStats() {
}

void IncrementFrameCount() {
    if (!frameCounterEnabled) {
        return;
    }
    ++g_totalFrameCount;
    if (g_authorFrameCount != -1) {
        ++g_authorFrameCount;
    }
}

void SetRequestModuleChange(std::int32_t requested) {
    g_requestModuleChange = requested != 0;
}

} // namespace shenmue::s2
