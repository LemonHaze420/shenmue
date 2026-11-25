#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/date_time.hpp"

namespace shenmue {
namespace s2 {

static const std::int8_t calendarMonthDays[16] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0, 31, 0};

static const std::int32_t calendarMonthStartDays[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetCurrentCalendarRecord(DateTime &record);
#endif
extern float g_secondsPerGameMinute;
extern float g_defaultSecondsPerGameMinute;
extern float g_realSecondsToGameMinute;
extern float g_calendarSecondAccumulator;
extern float g_calendarRealTimeScale;
void ResetCalendarRecord(std::uint32_t index);
std::int32_t DateTime_SubtractInPlace(DateTime &dateTime, const DateTime &subtrahend);
void SetCalendarTimeRange(std::int8_t startHour, std::int8_t startMinute, std::int8_t endHour, std::int8_t endMinute);
std::int32_t IsCurrentTimeRange(std::uint32_t range);

} // namespace s2
} // namespace shenmue

#endif
