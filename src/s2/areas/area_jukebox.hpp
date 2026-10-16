#pragma once

#include "common/types.hpp"

#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name AREA_JUKEBOX_TASK{SHEN_NAME(EVNT)};
void EVT_JB_InitWk_Callback(void *taskData);
void EVT_Juke_revolve_Callback(void *taskData);

} // namespace shenmue::s2
