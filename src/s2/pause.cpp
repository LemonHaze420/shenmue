#include "s2/pause.hpp"

#include "s2/conversation.hpp"
#include "s2/help.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/input.hpp"
#include "s2/particle_effects.hpp"
#include "s2/save.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {

std::uint32_t g_loadingFlag = 0;

namespace {

HLib::Task *execPauseTask = nullptr;     // g_pExecPause
std::int32_t execPauseValue = 5;         // g_dExecPauseVal
std::int32_t execPauseSnapshotState = 0; // g_dExecPauseVal_0
std::int32_t execPauseRequested = 0;     // g_execPauseRequested

void SuspendPauseOwners(std::int32_t paused) {
    AssignPreviousFrameTimeDelta(paused);
    CacheCurrentParticleEffects(paused);
    PrimeAicaEmulationForPause(paused);
    PauseOrDestroyAudioAndSubtitles(paused);
    SetRainAndRippleFlagsTo(paused);
}

void ResumeGame() {
    g_loadingFlag &= ~0x100U;
    HLib::UnpauseTaskEnqueue();
    SuspendPauseOwners(0);
}

void ExecPause_Callback(void *) {
    if (!HandleGameUIVisibility() && !execPauseRequested && (GetHelpIconEnabled() == 1 || !IsHelpOverlayVisible(UIElement::GameHelp))) {
        HLib::ResetPreservedCallbacks();
        return;
    }
    const std::uint32_t helpRequested = g_helpIconState;
    if (execPauseRequested) {
        g_inputBuffer[0].pressedPriority |= 0x800U;
        execPauseRequested = 0;
    }
    const std::uint16_t entryFlags = static_cast<std::uint16_t>(g_loadingFlag);
    g_helpIconState = 0;
    if (ReturnWindowHandle() == 0)
        g_loadingFlag |= 2U;
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const bool slotTwoStart = (g_inputBuffer[2].pressedPriority & 0x800U) != 0;
    if (g_loadingFlag & 0x20eU) {
        if ((g_loadingFlag & 0x10U) && (pressed & 0xf6c6U))
            g_loadingFlag &= ~0x10U;
        const std::uint32_t flags = g_loadingFlag;
        if ((flags & 0x400U) == 0 &&
            (((flags & 0x200U) == 0 && (flags & 0x820U)) || (pressed & 0x800U) || helpRequested || slotTwoStart || (pressed & 0x200U))) {
            g_loadingFlag = flags & ~0xe1eU;
            if ((g_loadingFlag & 0x100U) == 0) {
                SelectInputDeviceForButtons(0x800);
                HLib::UnpauseTaskEnqueue();
                SuspendPauseOwners(0);
            }
        }
    } else {
        if ((execPauseValue < 0 && (g_loadingFlag & 0x20U) == 0 && (pressed & 0x800U)) || helpRequested || slotTwoStart || (g_loadingFlag & 0x400U)) {
            g_loadingFlag |= 2U;
            if (slotTwoStart)
                g_loadingFlag |= 0x200U;
            if ((g_loadingFlag & 0x100U) == 0) {
                HLib::PauseTaskRangeExecution(3, 13);
                SuspendPauseOwners(1);
            }
        }
        if ((g_loadingFlag & 2U) == 0) {
            if ((g_loadingFlag & 0x100U) == 0) {
                if (snapCheckState() && !execPauseSnapshotState)
                    execPauseSnapshotState = 1;
                if (IsSaveAllowed() || execPauseSnapshotState == 1) {
                    g_loadingFlag |= 0x100U;
                    HLib::PauseTaskRangeExecution(3, 13);
                    SuspendPauseOwners(1);
                }
            } else if (execPauseSnapshotState == 1) {
                if (!snapCheckState()) {
                    execPauseSnapshotState = 0;
                    ResumeGame();
                }
            } else if (!IsSaveAllowed()) {
                ResumeGame();
            }
        }
    }
    if (entryFlags & 0x102U)
        HLib::ReplayPreservedCallbacks();
    if ((g_loadingFlag & 0x102U) == 0)
        HLib::ResetPreservedCallbacks();
}

} // namespace

HLib::Task *ExecPause() {
    g_helpIconState = 0;
    execPauseTask = HLib::EnqueueTaskWithParameter(ExecPause_Callback, 15, 2, sizeof(std::uint32_t), TASK_PAUS);
    g_loadingFlag = 1;
    execPauseValue = -1;
    execPauseSnapshotState = 0;
    return execPauseTask;
}

// @verified - p
void FinishLoading() {
    g_loadingFlag &= ~0x20U;
}

// @verified - p
std::uint64_t shenIsGamePaused() {
    return g_loadingFlag & 2U;
}

// @verified - p
void RequestExecPause() {
    execPauseRequested = 1;
}

// @verified - p
bool GetNotPausedOrLoading() {
    return execPauseValue < 0 && (g_loadingFlag & 0x20U) == 0;
}

// @verified - p
std::uint64_t GetLoadingBusyFlag() {
    return g_loadingFlag & 0x20U;
}

// @verified - p
void RequestFightTelopPause() {
    g_loadingFlag |= 0x400U;
}

// @verified - p
std::uint64_t RequestFightTelopResume() {
    g_loadingFlag = (g_loadingFlag & ~0x400U) | 0x800U;
    return g_loadingFlag;
}

} // namespace shenmue::s2
