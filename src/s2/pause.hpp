#pragma once

#include "s2/hlib.hpp"

#if !SHENMUE_S2_GCC295
#include <cstdint>
#endif

namespace shenmue {
namespace s2 {

// Native g_LoadingFlag (eLoadFlags): 0x1 set by ExecPause, 0x2 paused, 0x20
// busy (cleared by FinishLoading), 0x100 save/snapshot pause, 0x200 paused
// from input slot 2, 0x400/0x800 fight telop pause requests.
extern std::uint32_t g_loadingFlag;
#if SHENMUE_S2_GCC295
inline shen_name PauseTaskName() {
    return shen_name(make_shen_name('P', 'A', 'U', 'S'));
}
#define TASK_PAUS PauseTaskName()
#else
inline constexpr shen_name TASK_PAUS{SHEN_NAME(PAUS)};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if SHENMUE_S2_GCC295
inline shen_name PauseDeveloperArea() {
    return shen_name(make_shen_name('H', 'G', '0', '0'));
}
#define kPauseDeveloperArea PauseDeveloperArea()
#else
inline constexpr shen_name kPauseDeveloperArea{SHEN_NAME(HG00)};
#endif
extern HLib::Task *g_execPauseTask;
void ExecPause();
bool IsPauseInputAllowed();
#else
HLib::Task *ExecPause();
#endif
void FinishLoading();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t shenIsGamePaused();
#else
std::uint64_t shenIsGamePaused();
#endif
bool GetNotPausedOrLoading();
void RequestExecPause();
std::uint64_t GetLoadingBusyFlag();
void RequestFightTelopPause();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RequestFightTelopResume();
bool IsLoadingFlag10Set();
#else
std::uint64_t RequestFightTelopResume();
#endif
}
} // namespace shenmue::s2
