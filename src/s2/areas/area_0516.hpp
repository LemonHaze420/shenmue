#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0516_EVENT_TASK{SHEN_NAME(EVNT)};
void Update0516Waves(void *taskData);
} // namespace shenmue::s2::areas
