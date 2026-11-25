#include "s2/calendar_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/load_title_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/time_program_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/easy_option_state_dc.hpp"
#include "s2/ui_state_dc.hpp"

namespace shenmue {
namespace s2 {

void InitCalendar();
void ResetCalendarRecord(std::uint32_t index);
void InitTimeProgram_Callback(void *);
void CtrlTimeProgram_Callback(void *);
void UpdateGameTime(DateTime &dateTime);
DateComparisonResult CompareDates(const DateTime &first, const DateTime &second);
void ComputeDayOfWeek(DateTime &dateTime);
std::uint8_t GetParentMenu();
std::uint8_t UpdateSelectorMemo();

DateTime g_calendarRecords[4] = {};
float g_secondsPerGameMinute;
float g_defaultSecondsPerGameMinute;
float g_realSecondsToGameMinute = 0.0f;
float g_calendarSecondAccumulator;
float g_calendarRealTimeScale;
std::int8_t g_gameClockUpdateFlag, g_timeFrozen = 0, g_timePreviousItemState;
std::uint8_t g_timeUpdateAllowed, g_timeHudUpdateSuppressed = 0, g_timeFramesPerSecond, g_timeUpdateBlocked;
std::int32_t g_timeTemporaryDay, g_timeTemporaryHour, g_timeTemporaryMinute;

// @verified - d
void InitializeNewGameCalendar() {
    scenSetTaskFlags(4);
    InitCalendar();
}

// @verified - d
void InitCalendar() {
    scenSetTaskFlags(5);
    ResetCalendarRecord(0);
    ResetCalendarRecord(1);
    ResetCalendarRecord(2);
    g_defaultSecondsPerGameMinute = g_secondsPerGameMinute = 3.0f;
    g_calendarSecondAccumulator = 0.0f;
    g_realSecondsToGameMinute = 1.0f / g_secondsPerGameMinute;
    g_calendarRealTimeScale = -1.0f;
    ResetSTEP();
    InitST_TitleFunc();
}

// @verified - d
void ResetCalendarRecord(std::uint32_t index) {
    const std::uint8_t record = static_cast<std::uint8_t>(index);
    g_calendarRecords[record].year = 87;
    g_calendarRecords[record].month = 2;
    g_calendarRecords[record].day = 23;
    if (g_actNumber == 0x50) {
        g_calendarRecords[record].hour = 12;
        g_calendarRecords[record].minute = 0;
        g_calendarRecords[record].second = 0;
    } else {
        g_calendarRecords[record].hour = 12;
        g_calendarRecords[record].minute = 30;
        g_calendarRecords[record].second = 0;
    }
    g_calendarRecords[record].flags = 0;
    ComputeDayOfWeek(g_calendarRecords[record]);
}

// @verified - d
void initTimeProgram() {
    HLib::EnqueueTaskWithoutParameter(InitTimeProgram_Callback, 7, 0, SHEN_NAME4('T', 'I', 'M', 'E'));
    g_timeFrozen = 0;
    g_timeHudUpdateSuppressed = 0;
    g_timeFramesPerSecond = 30;
    g_timeUpdateBlocked = 0;
    g_timeUpdateAllowed = 1;
    g_secondsPerGameMinute = g_defaultSecondsPerGameMinute;
    if ((g_currentMap & 0xffffU) == (SHEN_NAME4('Q', 'F', '0', '0') & 0xffffU))
        g_secondsPerGameMinute = 30.0f;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'U', 'G', '0'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
        g_secondsPerGameMinute = 30.0f;
        break;
    case SHEN_NAME4('K', 'M', 'Z', '1'):
    case SHEN_NAME4('K', 'M', 'Z', '2'):
    case SHEN_NAME4('K', 'M', 'Z', '3'):
    case SHEN_NAME4('K', 'M', 'Z', '4'):
    case SHEN_NAME4('K', 'R', 'K', '2'):
    case SHEN_NAME4('K', 'R', 'K', '4'):
    case SHEN_NAME4('K', 'R', 'K', '6'):
        g_secondsPerGameMinute = 4.5f;
        break;
    }
    g_realSecondsToGameMinute = 1.0f / g_secondsPerGameMinute;
    g_calendarRealTimeScale = -1.0f;
    switch (static_cast<std::uint32_t>(g_actNumber)) {
    case 0:
        g_timeUpdateAllowed = 0;
        break;
    case 1:
        if (g_currentMap == SHEN_NAME4('1', '0', '0', '0'))
            g_timeUpdateBlocked = 1;
        else if (g_currentMap == SHEN_NAME4('2', '0', '0', '0'))
            g_timeUpdateBlocked = 1;
        break;
    case 80:
        g_timeUpdateAllowed = 0;
        g_timeUpdateBlocked = 1;
        break;
    }
    if (g_timeUpdateBlocked)
        g_timeUpdateAllowed = 0;
    g_gameClockUpdateFlag = g_timeUpdateAllowed;
    g_calendarRecords[2].year = 99;
    g_calendarRecords[2].month = 12;
    g_calendarRecords[2].day = 31;
    g_calendarRecords[2].hour = 23;
    g_calendarRecords[2].minute = 59;
    g_calendarRecords[2].second = 59;
    g_calendarRecords[3].year = 0;
    g_calendarRecords[3].month = 0;
    g_calendarRecords[3].day = 0;
    g_calendarRecords[3].hour = 0;
    g_calendarRecords[3].minute = 0;
    g_calendarRecords[3].second = 0;
    ResetTimeHudRuntime();
}

// @verified - d
void InitTimeProgram_Callback(void *) {
    HLib::SetCurrentTaskTo(CtrlTimeProgram_Callback);
}

// @verified - d
void CtrlTimeProgram_Callback(void *) {
    if (g_timeUpdateBlocked)
        return;
    if (g_timeFrozen != 1) {
        g_calendarRecords[3].year = 0;
        g_calendarRecords[3].month = 0;
        g_calendarRecords[3].day = 0;
        g_calendarRecords[3].hour = 0;
        g_calendarRecords[3].minute = 0;
        g_calendarRecords[3].second = 0;
        if (g_gameClockUpdateFlag == 1) {
            UpdateGameTime(g_calendarRecords[0]);
            if (CompareDates(g_calendarRecords[0], g_calendarRecords[2]) != DateComparisonResult_LessThan)
                g_gameClockUpdateFlag = 2;
        }
    }
    g_timePreviousItemState = static_cast<std::int8_t>(GetItemState(2));
    if (!g_timeHudUpdateSuppressed && g_timePreviousItemState)
        UpdateTimeProgramWaitTime();
}

// @verified - d
void IncrementDateTimeInPlace(DateTime &current, const DateTime &increment) {
    current.second = static_cast<std::int8_t>(current.second + increment.second);
    if (current.second > 59) {
        current.second = 0;
        ++current.minute;
    }
    current.minute = static_cast<std::int8_t>(current.minute + increment.minute);
    if (current.minute > 59) {
        current.minute = 0;
        ++current.hour;
    }
    current.hour = static_cast<std::int8_t>(current.hour + increment.hour);
    if (current.hour > 23) {
        current.hour = 0;
        ++current.day;
    }
    current.day = static_cast<std::int8_t>(current.day + increment.day);
    std::int8_t lastDay;
    if ((current.year & 3) == 0 && current.month == 2)
        lastDay = 29;
    else
        lastDay = calendarMonthDays[current.month];
    if (current.day > lastDay) {
        current.day = 1;
        ++current.month;
    }
    current.month = static_cast<std::int8_t>(current.month + increment.month);
    if (current.month > 12) {
        current.month = 1;
        ++current.year;
    }
    current.year = static_cast<std::int8_t>(current.year + increment.year);
    if (current.year > 99)
        current.year = 0;
    ComputeDayOfWeek(current);
}

// @verified - d
std::int32_t ComputeSecondsSince1980(const DateTime &dateTime, std::int32_t *output) {
    std::int32_t years = dateTime.year - 80;
    std::int32_t leapDays = years / 4;
    if (!(years & 3) && dateTime.month <= 2)
        --leapDays;
    std::int32_t days = years * 365 + leapDays + calendarMonthStartDays[dateTime.month - 1] - 1;
    days += dateTime.day;
    const std::int32_t result = ((days * 24 + dateTime.hour) * 60 + dateTime.minute) * 60 + dateTime.second;
    if (output)
        *output = result;
    return result;
}

// @verified - d
void IncrementDateTimeBySeconds(std::uint32_t seconds, DateTime &dateTime) {
    std::uint32_t minutes, hours, days, years;
    std::int32_t leapDays, day, month;

    dateTime.second = seconds % 60;
    minutes = seconds / 60;
    dateTime.minute = minutes % 60;
    hours = minutes / 60;
    dateTime.hour = hours % 24;
    days = hours / 24;
    day = days % 365;
    years = days / 365;
    leapDays = static_cast<std::int32_t>(years - 1) / 4;
    if (leapDays <= day) {
        day -= leapDays;
        dateTime.year = years + 80;
    } else {
        day += 365;
        dateTime.year = years + 79;
        day -= leapDays;
        if (((years + 79) & 3) == 0)
            ++day;
    }
    month = 1;
    if ((dateTime.year & 3) == 0) {
        if (day >= calendarMonthStartDays[1])
            for (month = 2; month < 12 && day >= calendarMonthStartDays[month] + 1; ++month)
                ;
    } else {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        for (month = 2; month < 12 && day >= calendarMonthStartDays[month]; ++month)
#else
        for (month = 1; month < 12 && day >= calendarMonthStartDays[month]; ++month)
#endif
            ;
    }
    dateTime.month = month;
    dateTime.day = day - calendarMonthStartDays[month - 1] + 1;
    if ((dateTime.year & 3) == 0 && dateTime.month > 2)
        --dateTime.day;
    ComputeDayOfWeek(dateTime);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t CompareDateTimeSeconds(const DateTime &first, const DateTime &second) {
    const std::uint32_t firstSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(first, 0));
    const std::uint32_t secondSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(second, 0));
    if (firstSeconds < secondSeconds)
        return -1;
    if (firstSeconds > secondSeconds)
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DateTime_AddInPlace(DateTime &dateTime, const DateTime &increment) {
    IncrementDateTimeBySeconds(static_cast<std::uint32_t>(ComputeSecondsSince1980(dateTime, 0) + ComputeSecondsSince1980(increment, 0)), dateTime);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t DateTime_SubtractInPlace(DateTime &dateTime, const DateTime &subtrahend) {
    const std::int32_t seconds = ComputeSecondsSince1980(dateTime, 0) - ComputeSecondsSince1980(subtrahend, 0);
    IncrementDateTimeBySeconds(static_cast<std::uint32_t>(seconds), dateTime);
    return seconds;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DateTime_AddSecondsInPlace(DateTime &dateTime, std::int32_t seconds) {
    IncrementDateTimeBySeconds(static_cast<std::uint32_t>(ComputeSecondsSince1980(dateTime, 0) + seconds), dateTime);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DateTime_AddMinutesInPlace(DateTime &dateTime, std::int32_t minutes) {
    DateTime_AddSecondsInPlace(dateTime, minutes * 60);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t DiffMinutes(const DateTime &first, const DateTime &second) {
    const std::uint32_t firstSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(first, 0));
    const std::uint32_t secondSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(second, 0));
    return static_cast<std::int32_t>(firstSeconds - secondSeconds) / 60;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetCurrentCalendarRecord(DateTime &record) {
    record = g_calendarRecords[0];
}
#endif

// @verified - d
void AddDaysAndCalculateWeekday(DateTime &dateTime, std::int16_t days) {
    for (std::int16_t elapsed = 0; elapsed < days; ++elapsed) {
        std::int16_t lastDay;
        if ((dateTime.year & 3) == 0 && dateTime.month == 2)
            lastDay = 29;
        else
            lastDay = calendarMonthDays[dateTime.month];
        if (++dateTime.day > lastDay) {
            dateTime.day = 1;
            if (++dateTime.month > 12) {
                dateTime.month = 1;
                if (++dateTime.year > 99)
                    dateTime.year = 0;
            }
        }
    }
    ComputeDayOfWeek(dateTime);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void shenGetDayInMonth(const DateTime &input, DateTime &output) {
    output.day = input.day;
    output.month = input.month;
    output.year = input.year;
    output.day = static_cast<std::int8_t>(output.day - 1);
    if (output.day > 0)
        return;
    output.month = static_cast<std::int8_t>(output.month - 1);
    if (output.month < 1) {
        output.month = 12;
        output.year = static_cast<std::int8_t>(output.year - 1);
        if (output.year < 0)
            output.year = 99;
    }
    if ((output.year & 3) == 0 && output.month == 2)
        output.day = 29;
    else
        output.day = calendarMonthDays[output.month];
}
#endif

// @verified - d
void UpdateGameTime(DateTime &dateTime) {
    if (!GetParentMenu() || UpdateSelectorMemo() || IsEasyOptionActive())
        return;
    if (g_calendarRealTimeScale > 0.0f) {
        if (g_timeTemporaryDay == dateTime.day) {
            if (g_timeTemporaryHour < dateTime.hour)
                g_calendarRealTimeScale = -1.0f;
            else if (g_timeTemporaryHour == dateTime.hour && g_timeTemporaryMinute <= dateTime.minute)
                g_calendarRealTimeScale = -1.0f;
        } else {
            g_calendarRealTimeScale = -1.0f;
        }
    }
    if (g_calendarRealTimeScale > 0.0f)
        g_calendarSecondAccumulator += g_calendarRealTimeScale;
    else
        g_calendarSecondAccumulator += g_realSecondsToGameMinute;
    while (!(60.0f > g_calendarSecondAccumulator)) {
        g_calendarSecondAccumulator -= 60.0f;
        g_calendarRecords[3].minute = 1;
        if (++dateTime.minute > 59) {
            dateTime.minute = 0;
            g_calendarRecords[3].hour = 1;
            if (++dateTime.hour > 23) {
                dateTime.hour = 0;
                g_calendarRecords[3].day = 1;
                const std::int8_t lastDay = (dateTime.year & 3) == 0 && dateTime.month == 2 ? 29 : calendarMonthDays[dateTime.month];
                dateTime.day = static_cast<std::int8_t>(dateTime.day + 1);
                if (dateTime.day > lastDay) {
                    dateTime.day = 1;
                    g_calendarRecords[3].month = 1;
                    if (++dateTime.month > 12) {
                        dateTime.month = 1;
                        g_calendarRecords[3].year = 1;
                        if (++dateTime.year > 99)
                            dateTime.year = 0;
                    }
                }
                ComputeDayOfWeek(dateTime);
            }
        }
    }
    dateTime.second = static_cast<std::int32_t>(g_calendarSecondAccumulator);
}

// @verified - d
DateComparisonResult CompareDates(const DateTime &first, const DateTime &second) {
    std::int32_t firstValue = (first.year * 64 + first.month) * 64 + first.day;
    std::int32_t secondValue = (second.year * 64 + second.month) * 64 + second.day;
    if (firstValue < secondValue)
        return DateComparisonResult_LessThan;
    if (firstValue > secondValue)
        return DateComparisonResult_GreaterThan;
    firstValue = (first.hour * 64 + first.minute) * 64 + first.second;
    secondValue = (second.hour * 64 + second.minute) * 64 + second.second;
    if (firstValue < secondValue)
        return DateComparisonResult_LessThan;
    return firstValue > secondValue ? DateComparisonResult_GreaterThan : DateComparisonResult_Equal;
}

// @verified - d
void ComputeDayOfWeek(DateTime &dateTime) {
    std::int16_t year = static_cast<std::int16_t>(dateTime.year + 1900);
    std::int8_t month = dateTime.month;
    const std::int8_t day = dateTime.day;
    if (month < 3) {
        year = static_cast<std::int16_t>(year - 1);
        month = static_cast<std::int8_t>(month + 12);
    }
    dateTime.week = static_cast<std::uint8_t>((year + year / 4 - year / 100 + year / 400 + (13 * month + 8) / 5 + day) % 7);
}

// @verified - d
void SetCurrentDateTimeTo(DateTime &dateTime) {
    DateTime increment;
    increment.year = 0;
    increment.month = 0;
    increment.day = 0;
    increment.hour = 0;
    increment.minute = 0;
    increment.second = 0;
    IncrementDateTimeInPlace(dateTime, increment);
    DateTime &current = g_calendarRecords[0];
    current.year = dateTime.year;
    current.month = dateTime.month;
    current.day = dateTime.day;
    current.hour = dateTime.hour;
    current.minute = dateTime.minute;
    current.second = dateTime.second;
    ComputeDayOfWeek(current);
}

// @verified - d
void shenGetCurrentDateTime(DateTime &dateTime) {
    dateTime.year = g_currentDateTime.year;
    dateTime.month = g_currentDateTime.month;
    dateTime.day = g_currentDateTime.day;
    dateTime.week = g_currentDateTime.week;
    dateTime.hour = g_currentDateTime.hour;
    dateTime.minute = g_currentDateTime.minute;
    dateTime.second = g_currentDateTime.second;
}

void SetCurrentDateTimeAndEndTime(DateTime &current, const DateTime &end) {
    SetCurrentDateTimeTo(current);
    g_calendarRecords[2].year = end.year;
    g_calendarRecords[2].month = end.month;
    g_calendarRecords[2].day = end.day;
    g_calendarRecords[2].hour = end.hour;
    g_calendarRecords[2].minute = end.minute;
    g_calendarRecords[2].second = end.second;
    ComputeDayOfWeek(g_calendarRecords[2]);
    if (g_timeUpdateAllowed)
        g_gameClockUpdateFlag = 1;
}

DateComparisonResult SetCalendarCompareDate(const DateTime &dateTime) {
    g_calendarRecords[1].year = dateTime.year;
    g_calendarRecords[1].month = dateTime.month;
    g_calendarRecords[1].day = dateTime.day;
    g_calendarRecords[1].hour = dateTime.hour;
    g_calendarRecords[1].minute = dateTime.minute;
    g_calendarRecords[1].second = dateTime.second;
    ComputeDayOfWeek(g_calendarRecords[1]);
    return CompareDates(g_calendarRecords[0], g_calendarRecords[1]);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetCalendarTimeRange(std::int8_t startHour, std::int8_t startMinute, std::int8_t endHour, std::int8_t endMinute) {
    g_calendarRecords[2].year = g_calendarRecords[0].year;
    g_calendarRecords[2].month = g_calendarRecords[0].month;
    g_calendarRecords[2].day = g_calendarRecords[0].day;
    g_calendarRecords[0].hour = startHour;
    g_calendarRecords[0].minute = startMinute;
    g_calendarRecords[0].second = 0;
    ComputeDayOfWeek(g_calendarRecords[0]);
    if (startHour > endHour)
        AddDaysAndCalculateWeekday(g_calendarRecords[2], 1);
    else if (startHour == endHour && startMinute > endMinute)
        AddDaysAndCalculateWeekday(g_calendarRecords[2], 1);
    g_calendarRecords[2].hour = endHour;
    g_calendarRecords[2].minute = endMinute;
    g_calendarRecords[2].second = 0;
    ComputeDayOfWeek(g_calendarRecords[2]);
    if (g_timeUpdateAllowed)
        g_gameClockUpdateFlag = 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t CalendarQueryNullsub() {
    return 0;
}
#endif

// @verified - d
void AddDaysAndCalculateWeekdayFromCurrentDate(std::int16_t days) {
    AddDaysAndCalculateWeekday(g_calendarRecords[0], days);
}

// @verified - d
void shenSetTimeSimulationTo(std::int8_t state) {
    if (g_timeUpdateAllowed)
        g_gameClockUpdateFlag = state;
}

void AdvanceCurrentDateTime(const DateTime &dateTime, std::uint8_t mode) {
    if (mode == 2) {
        g_calendarRecords[0].hour = dateTime.hour;
        g_calendarRecords[0].minute = dateTime.minute;
        g_calendarRecords[0].second = dateTime.second;
        return;
    }
    std::int32_t later = 0;
    if (dateTime.year > g_calendarRecords[0].year)
        later = 1;
    else if (dateTime.month > g_calendarRecords[0].month)
        later = 1;
    else if (dateTime.day > g_calendarRecords[0].day)
        later = 1;
    if (!later)
        return;
    g_calendarRecords[0].year = dateTime.year;
    g_calendarRecords[0].month = dateTime.month;
    g_calendarRecords[0].day = dateTime.day;
    if (!mode)
        return;
    g_calendarRecords[0].hour = dateTime.hour;
    g_calendarRecords[0].minute = dateTime.minute;
    g_calendarRecords[0].second = dateTime.second;
}

void GetCalendarElapsedTime(DateTime &dateTime) {
    dateTime.year = g_calendarRecords[3].year;
    dateTime.month = g_calendarRecords[3].month;
    dateTime.day = g_calendarRecords[3].day;
    dateTime.hour = g_calendarRecords[3].hour;
    dateTime.minute = g_calendarRecords[3].minute;
}

// @verified - d
void UpdateTempHourMinTimeScale(std::int32_t hour, std::int32_t minute, float scale) {
    g_timeTemporaryDay = g_currentDateTime.day;
    g_timeTemporaryHour = hour;
    g_timeTemporaryMinute = minute;
    g_calendarRealTimeScale = scale / 30.0f;
}

void ToggleTimeFrozen() {
    g_timeHudUpdateSuppressed = (g_timeHudUpdateSuppressed + 1) & 1;
}

static const char *const calendarWeekdayNames[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

void DrawCalendarDebugDateTime(const DateTime *dateTime, std::uint16_t column) {
    if (!g_developerDisplayEnabled)
        return;
    SetDebugScreenTextPosition(column, 24);
    shenmue_s2_event_DebugLogToScreen("%2d", dateTime->year);
    SetDebugScreenTextPosition(column + 3, 24);
    shenmue_s2_event_DebugLogToScreen("%2d", dateTime->month);
    SetDebugScreenTextPosition(column + 6, 24);
    shenmue_s2_event_DebugLogToScreen("%2d", dateTime->day);
    SetDebugScreenTextPosition(column + 2, 24);
    shenmue_s2_event_DebugLogToScreen("/");
    SetDebugScreenTextPosition(column + 5, 24);
    shenmue_s2_event_DebugLogToScreen("/");
    SetDebugScreenTextPosition(column, 25);
    shenmue_s2_event_DebugLogToScreen("%2d", dateTime->hour);
    SetDebugScreenTextPosition(column + 3, 25);
    shenmue_s2_event_DebugLogToScreen("%02d", dateTime->minute);
    SetDebugScreenTextPosition(column + 6, 25);
    shenmue_s2_event_DebugLogToScreen("%02d", dateTime->second);
    SetDebugScreenTextPosition(column + 2, 25);
    shenmue_s2_event_DebugLogToScreen(":");
    SetDebugScreenTextPosition(column + 5, 25);
    shenmue_s2_event_DebugLogToScreen(":");
    SetDebugScreenTextPosition(column, 26);
    shenmue_s2_event_DebugLogToScreen(calendarWeekdayNames[static_cast<std::int8_t>(dateTime->week)]);
    SetDebugScreenTextPosition(column - 1, 27);
    shenmue_s2_event_DebugLogToScreen("%2.2f", g_secondsPerGameMinute);
    SetDebugScreenTextPosition(column + 5, 27);
    if (g_secondsPerGameMinute != 0.0f)
        shenmue_s2_event_DebugLogToScreen("%2.1f", 30.0f / g_secondsPerGameMinute);
    else
        shenmue_s2_event_DebugLogToScreen("****");
}

// @verified - d
void shenAdjustCurrentTimeByMinutes(std::int16_t minutes) {
    std::int32_t record = 0;
    std::int32_t minute = g_calendarRecords[record].minute;
    std::int32_t hour = g_calendarRecords[record].hour;
    std::int32_t days = 0;
    minute += minutes;
    while (minute > 59) {
        minute -= 60;
        ++hour;
    }
    while (minute < 0) {
        minute += 60;
        --hour;
    }
    while (hour > 23) {
        hour -= 24;
        ++days;
    }
    while (hour < 0)
        hour += 24;
    g_calendarRecords[record].minute = static_cast<std::int8_t>(minute);
    g_calendarRecords[record].hour = static_cast<std::int8_t>(hour);
    if (days)
        AddDaysAndCalculateWeekdayFromCurrentDate(static_cast<std::int16_t>(days));
}

void WaitTimerMicroseconds(float microseconds) {
    float ticks = microseconds * 12.5f;
    float start = static_cast<float>(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister));
    while (!(start - static_cast<float>(*reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister)) > ticks)) {
    }
}

} // namespace s2
} // namespace shenmue

extern "C" void __attribute__((renesas)) shenmue_s2_event_UpdateTempHourMinTimeScale(std::int32_t hour, std::int32_t minute, float scale) {
    shenmue::s2::UpdateTempHourMinTimeScale(hour, minute, scale);
}

#endif
