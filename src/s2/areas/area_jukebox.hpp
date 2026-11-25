#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#ifndef SHENMUE_JUKEBOX_REVISION
#define SHENMUE_JUKEBOX_REVISION 30
#endif
typedef int EventPackageArgument;
#define AREA_JUKEBOX_TASK_TOKEN TASK_EVNT
void EVT_JB_InitWk_Callback(void *taskData);
void EVT_Juke_revolve_Callback(void *taskData);
#else

#include "common/types.hpp"

#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name AREA_JUKEBOX_TASK{SHEN_NAME(EVNT)};
inline constexpr std::int32_t AREA_JUKEBOX_TASK_TOKEN = static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name);
using EventPackageArgument = std::uintptr_t;
void EVT_JB_InitWk_Callback(void *taskData);
void EVT_Juke_revolve_Callback(void *taskData);

} // namespace shenmue::s2

#endif
