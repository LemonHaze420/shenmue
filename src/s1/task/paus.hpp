#pragma once

#include "s1/hlib.hpp"

#include <cstdint>
namespace shenmue::s1 {
enum PauseStateBits : std::int32_t {
    PAUSE_STATE_INIT = 0x001,
    PAUSE_STATE_PAUSED = 0x002,
    PAUSE_STATE_HIDE_HELP = 0x010,
    PAUSE_STATE_FREE = 0x020,
    PAUSE_STATE_AUDIO_PAUSED = 0x100,
};

using PauseQuery = bool (*)();
using PauseSetState = void (*)(bool paused);
using PauseScreenTransition = void (*)(bool opening);

struct PauseRuntimeBindings {
    PauseQuery inputEnabled = nullptr;
    PauseQuery debugPauseRequested = nullptr;
    PauseQuery modalActive = nullptr;
    PauseQuery menuBackgroundVisible = nullptr;
    PauseSetState setEffectsPaused = nullptr;
    PauseSetState setAudioPaused = nullptr;
    PauseSetState setGlobalScenePaused = nullptr;
    PauseScreenTransition transitionPauseScreen = nullptr;
};

extern PauseRuntimeBindings g_pauseRuntime;
extern std::int32_t g_pauseFrameAdvanceDebugState;
extern std::int32_t g_pauseFrameAdvanceState;
extern std::int32_t g_controllerReconnectHandled;
extern std::int32_t g_pauseInputRequest;
extern std::int32_t g_pauseInputTransitionPending;
extern std::int32_t g_controllerHelpOverlayState;
extern std::int32_t g_pauseActiveState;

void PAUS_BindRuntime(const PauseRuntimeBindings &bindings);
void PAUS_RequestToggle();
void PAUS_ResetRuntime();
void ModifyTasksForPauseScreen();
bool GetInputStateForHelp();
void CalculateCycleManagerTime(std::uint32_t mode);

void *PAUS_Init();
void PAUS_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
