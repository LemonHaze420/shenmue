#pragma once

#include "s2/hlib.hpp"

#include <cstdint>

namespace shenmue::s2 {




extern std::uint32_t g_loadingFlag;
inline constexpr shen_name TASK_PAUS{SHEN_NAME(PAUS)};

HLib::Task *ExecPause();
void FinishLoading();
std::uint64_t shenIsGamePaused();
bool GetNotPausedOrLoading();
void RequestExecPause();
std::uint64_t GetLoadingBusyFlag();
void RequestFightTelopPause();
std::uint64_t RequestFightTelopResume();

} // namespace shenmue::s2
