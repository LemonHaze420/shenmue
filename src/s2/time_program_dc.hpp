#pragma once

#include "s2/calendar_dc.hpp"
#include "s2/time_program.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"

namespace shenmue {
namespace s2 {

extern std::int8_t g_gameClockUpdateFlag, g_timeFrozen, g_timePreviousItemState;
extern std::uint8_t g_timeUpdateAllowed, g_timeHudUpdateSuppressed, g_timeFramesPerSecond, g_timeUpdateBlocked;
extern std::int32_t g_timeTemporaryDay, g_timeTemporaryHour, g_timeTemporaryMinute;
void InitTimeProgram_Callback(void *data);
void CtrlTimeProgram_Callback(void *data);
void UpdateGameTime(DateTime &dateTime);
void ResetTimeHudRuntime();
void UpdateTimeProgramWaitTime();

} // namespace s2
} // namespace shenmue

#endif
