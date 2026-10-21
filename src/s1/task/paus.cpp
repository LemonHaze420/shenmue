#include "s1/task/paus.hpp"
#include "logging.hpp"

#include "s1/boot.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/save.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/scen.hpp"
#include "s1/timing.hpp"

#include <bit>
#include <cstdint>
#include <cstdio>

namespace shenmue::s1 {

std::int32_t g_pauseFrameAdvanceDebugState = 0;
std::int32_t g_pauseFrameAdvanceState = 0;
std::int32_t g_controllerReconnectHandled = 0;
std::int32_t g_pauseInputRequest = 0;
std::int32_t g_pauseInputTransitionPending = 0;
std::int32_t g_controllerHelpOverlayState = 0;
std::int32_t g_pauseActiveState = 0;
PauseRuntimeBindings g_pauseRuntime{};

void CalculateCycleManagerTime(std::uint32_t mode) {
    if (SCEN_IsConversationCameraOverrideActive()) {
        const std::int32_t current = static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime));
        if (mode == 0) {
            g_CurrentFrameDelta = current - CycleManager_DeltaTime;
            HLib_DeltaTime = static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime)) - CycleManager_DeltaTime_0;
        } else {
            CycleManager_DeltaTime = current - g_CurrentFrameDelta;
            CycleManager_DeltaTime_0 = static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime)) - HLib_DeltaTime;
        }
    }
    const std::int32_t elapsed = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
    if (mode != 0) {
        g_TimeDelta = elapsed - g_scenarioTaskDeltaTime;
    } else {
        g_scenarioTaskDeltaTime = elapsed - g_TimeDelta;
    }
}

namespace {


constexpr std::uint64_t kHelpInputOverride = 1ULL << 44U;
constexpr std::uint64_t kHelpInputBlock = 1ULL << 46U;
constexpr std::uint64_t kHelpInputAuxiliary = 1ULL << 32U;
constexpr std::uint16_t kPauseRequestButton = 0x0800U;
constexpr std::uint16_t kFrameAdvanceButton = 0x0200U;
constexpr std::uint16_t kHelpVisibleHeldButtons = 0x0006U;
constexpr std::uint16_t kHelpVisiblePressedMask = 0xf6c6U;
constexpr std::int32_t kGameMenuElement = 6;
constexpr std::int32_t kGameBackgroundElement = 7;
constexpr std::int32_t kHelpNewElement = 76;

bool Query(PauseQuery query, bool fallback) {
    return query ? query() : fallback;
}

} // namespace

bool GetInputStateForHelp() {
    if (g_NumFramesProcessed != 0) {
        UIBaseRuntime &help = g_uiElements[kHelpNewElement];
        if (g_currUIState == -1 || g_currUIState == kHelpNewElement) {
            if (!help.isVisible || !help.destroy || help.destroy() == 1 || g_busy_reset == 1) {
                help.isVisible = 0;
            }
        }
        return false;
    }
    if (g_controllerHelpOverlayState == 1)
        return false;

    const bool menuVisible = g_uiElements[kGameMenuElement].isVisible != 0 || g_uiElements[kGameBackgroundElement].isVisible != 0;
    const InputMode inputType = GetCurrentInputType();
    if ((menuVisible || inputType == InputMode::GameUi) && (g_inputContextFlags & kHelpInputOverride) == 0) {
        return false;
    }
    if (inputType == InputMode::Menu)
        return false;
    if ((g_inputContextFlags & kHelpInputBlock) != 0)
        return false;
    if ((g_inputContextFlags & kHelpInputAuxiliary) != 0 && g_70mbShownIconStatus == 4) {
        return false;
    }
    return true;
}

namespace {

void SetEffectsPaused(bool paused) {
    if (g_pauseRuntime.setEffectsPaused)
        g_pauseRuntime.setEffectsPaused(paused);
}

void SetAudioPaused(bool paused) {
    if (g_pauseRuntime.setAudioPaused) {
        g_pauseRuntime.setAudioPaused(paused);
    } else {
        SOND_SetPaused(paused);
    }
}

void SetGlobalScenePaused(bool paused) {
    if (g_pauseRuntime.setGlobalScenePaused) {
        g_pauseRuntime.setGlobalScenePaused(paused);
        return;
    }
    constexpr std::uint32_t kScenePaused = 0x10000000U;
    for (GLBSWeatherDrawState *state = g_pGLBSTaskData; state; state = reinterpret_cast<GLBSWeatherDrawState *>(state->next)) {
        if (paused) {
            state->drawFlags10 |= kScenePaused;
            state->drawFlags9c |= kScenePaused;
            state->drawFlags110 |= kScenePaused;
        } else {
            state->drawFlags10 &= ~kScenePaused;
            state->drawFlags9c &= ~kScenePaused;
            state->drawFlags110 &= ~kScenePaused;
        }
    }
}

void PauseOwnedTasks() {
    HLib_PauseTaskRangeExecution(3, 12);
    CalculateCycleManagerTime(1);
    SetEffectsPaused(true);
    g_effePauseState = 1;
    SetAudioPaused(true);
    SetGlobalScenePaused(true);
}

void ResumeOwnedTasks() {
    HLib_UnpauseTaskEnqueue();
    CalculateCycleManagerTime(0);
    SetEffectsPaused(false);
    g_effePauseState = 0;
    SetAudioPaused(false);
    SetGlobalScenePaused(false);
}

} // namespace

void PAUS_BindRuntime(const PauseRuntimeBindings &bindings) {
    g_pauseRuntime = bindings;
}

void PAUS_RequestToggle() {
    g_pauseInputRequest = 1;
}


void ModifyTasksForPauseScreen() {
    const bool audioPaused = (Shenmue_nPauseState & PAUSE_STATE_AUDIO_PAUSED) != 0;
    g_controllerReconnectHandled = 0;
    Shenmue_nPauseState &= static_cast<std::int32_t>(0xffffffe1U);
    if (audioPaused)
        return;
    ResumeOwnedTasks();
}
std::uint32_t IsNotPaused() {
    return static_cast<std::uint32_t>(Shenmue_nPauseState) & 2U;
}

void PAUS_ResetRuntime() {
    ModifyTasksForPauseScreen();
    g_pauseFrameAdvanceDebugState = 0;
    g_pauseFrameAdvanceState = 0;
    g_controllerReconnectHandled = 0;
    g_pauseInputRequest = 0;
    g_pauseInputTransitionPending = 0;
    g_controllerHelpOverlayState = 0;
    g_pauseActiveState = 0;
    g_pauseRuntime = {};
}

void *PAUS_Init() {
    g_pauseFrameAdvanceDebugState = 0;
    g_pauseFrameAdvanceState = 0;
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(PAUS_Callback, 15, 2, SHEN_NAME(PAUS));
    void *result = nullptr;
    if (task) {
        if (!task->task_data_ptr) {
            void *const data = HLib_AllocHeapMemBlock(4, SHEN_NAME(TASK));
            if (data)
                HLib_EnableTokenCheck(data);
            task->task_data_ptr = static_cast<HLib::TaskData *>(data);
            if (data) {
                Shenmue_nPauseState = PAUSE_STATE_INIT;
                g_controllerReconnectHandled = 0;
                return data;
            }
        }
        result = std::bit_cast<void *>(&HLib_DestroyCurrentTask);
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
    }
    Shenmue_nPauseState = PAUSE_STATE_INIT;
    g_controllerReconnectHandled = 0;
    return result;
}

void PAUS_Callback(HLib::TaskData *) {
    if (!GetInputStateForHelp() && !g_pauseInputRequest && (g_controllerHelpOverlayState == 1 || (g_inputContextFlags & kHelpInputOverride) == 0)) {
        return;
    }

    const InputSlot &input = g_inputState.slots[0];
    std::int32_t pauseState = Shenmue_nPauseState;
    if (Query(g_pauseRuntime.debugPauseRequested, false) && !Query(g_pauseRuntime.inputEnabled, input.enabled != 0)) {
        pauseState |= PAUSE_STATE_PAUSED;
        Shenmue_nPauseState = pauseState;
    }

    std::int32_t frameAdvanceState = g_pauseFrameAdvanceState;
    if (frameAdvanceState < 0)
        frameAdvanceState = 0;
    g_pauseFrameAdvanceState = frameAdvanceState;

    if (g_xinputReserved == 0 && !g_controllerReconnectHandled) {
        frameAdvanceState = 1;
        g_pauseFrameAdvanceState = frameAdvanceState;
        g_controllerReconnectHandled = 1;
        g_controllerHelpOverlayState = 1;
        g_helpOverlayPageTokenId = 66;
        g_helpOverlayState = UIHelpWidgetState::PreparePage;
        pauseState &= ~PAUSE_STATE_PAUSED;
        Shenmue_nPauseState = pauseState;
    }

    const std::uint16_t pressed = input.pressedPriority;
    const std::int32_t requestedInput = g_pauseInputRequest;
    if ((pressed & kPauseRequestButton) == 0) {
        if (!requestedInput) {
            if (!frameAdvanceState)
                goto normalizeFrameAdvance;
            goto processFrameAdvance;
        }
    } else if (!requestedInput) {
        goto setFrameAdvance;
    }
    g_pauseInputTransitionPending = 1;
setFrameAdvance:
    frameAdvanceState = 1;
    g_pauseInputRequest = 0;
processFrameAdvance:
    if (g_updateRenderFrameMode <= 0)
        frameAdvanceState = -1;
    g_pauseFrameAdvanceState = frameAdvanceState;
normalizeFrameAdvance :

{
    std::int32_t debugFrameAdvance = g_pauseFrameAdvanceDebugState;
    if (debugFrameAdvance < 0)
        debugFrameAdvance = 0;
    g_pauseFrameAdvanceDebugState = debugFrameAdvance;
    if ((g_dbg_flags & 0x800) != 0) {
        debugFrameAdvance = 1;
    } else if (debugFrameAdvance == 0) {
        goto processPauseState;
    }
    if (g_updateRenderFrameMode <= 0)
        debugFrameAdvance = -1;
    g_pauseFrameAdvanceDebugState = debugFrameAdvance;
}

processPauseState:
    if ((pauseState & PAUSE_STATE_PAUSED) != 0) {
        if ((pressed & kFrameAdvanceButton) != 0)
            frameAdvanceState = 1;
        g_pauseFrameAdvanceState = frameAdvanceState;
    }

    if ((pauseState & 0x0e) != 0) {
        if ((pauseState & PAUSE_STATE_HIDE_HELP) != 0) {
            if ((pressed & kHelpVisiblePressedMask) != 0) {
                pauseState &= ~PAUSE_STATE_HIDE_HELP;
                Shenmue_nPauseState = pauseState;
            }
        } else if (input.held == kHelpVisibleHeldButtons) {
            pauseState |= PAUSE_STATE_HIDE_HELP;
            Shenmue_nPauseState = pauseState;
        }
        if ((pauseState & PAUSE_STATE_FREE) != 0 || frameAdvanceState < 0) {
            ModifyTasksForPauseScreen();
            pauseState = Shenmue_nPauseState;
        }
        goto updatePauseActiveState;
    }

    if ((pauseState & PAUSE_STATE_FREE) == 0 && frameAdvanceState < 0) {
        pauseState |= PAUSE_STATE_PAUSED;
        Shenmue_nPauseState = pauseState;
        if ((pauseState & PAUSE_STATE_AUDIO_PAUSED) == 0) {
            pauseState |= PAUSE_STATE_AUDIO_PAUSED;
            Shenmue_nPauseState = pauseState;
            PauseOwnedTasks();
            pauseState = Shenmue_nPauseState;
        }
    }
    if ((pauseState & PAUSE_STATE_PAUSED) != 0)
        goto updatePauseActiveState;

    if ((pauseState & PAUSE_STATE_AUDIO_PAUSED) != 0) {
        if (g_easyOptionState.state == 0 && g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_NONE) {
            Shenmue_nPauseState = pauseState & ~PAUSE_STATE_AUDIO_PAUSED;
            ResumeOwnedTasks();
            pauseState = Shenmue_nPauseState;
        }
        goto updatePauseActiveState;
    }

    if (g_sceneId == 80 || (g_easyOptionState.state == 0 && g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_NONE)) {
        goto updatePauseActiveState;
    }
    pauseState |= PAUSE_STATE_AUDIO_PAUSED;
    Shenmue_nPauseState = pauseState;
    if (g_uiElements[kGameMenuElement].isVisible != 0 || g_uiElements[kGameBackgroundElement].isVisible != 0) {
        goto updatePauseActiveState;
    }
    PauseOwnedTasks();

updatePauseActiveState:
    if ((Shenmue_nPauseState & (PAUSE_STATE_PAUSED | PAUSE_STATE_AUDIO_PAUSED)) == 0) {
        g_pauseActiveState = 0;
    }
}

} // namespace shenmue::s1
