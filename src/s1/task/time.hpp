#pragma once

#include "s1/date_time.hpp"
#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum class DateComparisonResult : std::uint8_t {
    lessThan = 0,
    equal = 1,
    greaterThan = 2,
};

#pragma pack(push, 1)
struct TimeWatchTransform {
    std::uint32_t field_00;
    std::uint16_t field_04;
    std::int16_t field_06;
    std::int32_t field_08;
    std::uint32_t field_0c;
    Vector3 position;
    std::uint32_t field_1c;
    std::uint32_t field_20;
};

struct TimeClockUiGlobals {
    TimeWatchTransform transform;
    std::int32_t transition;
    float field_28;
    float field_2c;
    float field_30;
    float field_34;
    float field_38;
    std::uint16_t field_3c;
    std::uint16_t reserved_3e;
    std::int32_t cameraYaw;
    std::int32_t clockFaceFlags;
    std::uint16_t clockFaceMode;
    std::uint8_t clockFaceLayer;
    std::uint8_t reserved_4b;
    std::int32_t skipCycles;
    float clockFaceX;
    float clockFaceY;
    float clockFaceScaleX;
    float clockFaceScaleY;
    float clockFaceDepth;
    std::uint16_t field_64;
    std::uint16_t reserved_66;
    std::int32_t field_68;
    std::int32_t hourHandFlags;
    std::uint16_t hourHandMode;
    std::uint8_t hourHandLayer;
    std::uint8_t reserved_73;
    float hourHandX;
    float hourHandY;
    float hourHandStep;
    float hourHandScale;
    float hourHandDepth;
    std::uint16_t field_88;
    std::uint16_t reserved_8a;
    std::uint32_t hourHand;
    std::int32_t minuteHandFlags;
    std::uint16_t minuteHandMode;
    std::uint8_t minuteHandLayer;
    std::uint8_t reserved_97;
    float minuteHandX;
    float minuteHandY;
    float minuteHandStep;
    float minuteHandScale;
    float minuteHandDepth;
    std::uint16_t field_ac;
    std::uint16_t reserved_ae;
    std::uint32_t minuteHand;
    std::int32_t secondHandFlags;
    std::uint16_t secondHandMode;
    std::uint8_t secondHandLayer;
};
#pragma pack(pop)

static_assert(sizeof(TimeWatchTransform) == 0x24);
static_assert(offsetof(TimeClockUiGlobals, transition) == 0x24);
static_assert(offsetof(TimeClockUiGlobals, skipCycles) == 0x4c);
static_assert(offsetof(TimeClockUiGlobals, hourHand) == 0x8c);
static_assert(offsetof(TimeClockUiGlobals, minuteHand) == 0xb0);
static_assert(sizeof(TimeClockUiGlobals) == 0xbb);


inline constexpr std::uint8_t kDaysInMonth[13]{31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

extern GameDateTime g_currentDateTime;
extern GameDateTime g_maximumDateTime;
extern GameDateTime g_temporaryDateTime;
extern std::uint16_t g_temporaryDateTimeWord;
extern std::uint8_t g_timeValue;
extern std::uint8_t g_freezeTime;
extern std::uint8_t g_timeDisabled;
extern std::uint8_t g_timeBlockedUpdate;
extern std::uint8_t g_shouldTimeAdvance;
extern std::uint8_t g_hasIncrementedYear;
extern std::uint8_t g_hasIncrementedMonth;
extern std::uint8_t g_hasIncrementedDay;
extern std::uint8_t g_hasIncrementedMinutes;
extern std::uint8_t g_hasIncrementedSeconds;
extern float g_newCycleTime;
extern float g_secondHandAngle;
extern float g_minuteHandRadians;
extern float g_hourHandRadians;
extern TimeClockUiGlobals g_timeClockUi;
extern std::uint8_t g_unusedTimeBypass;
extern float g_currentDeltaTimeSim;
extern float g_timeMultiplier;
extern float g_cycleTimeMultiplier;

DateComparisonResult CompareDates(const GameDateTime *first, const GameDateTime *second);
bool shenGetCanUpdate();
void IncrementDateTimeByDays(GameDateTime &dateTime, std::int16_t days);
void IncrementCurrentDateTime(GameDateTime &now, const GameDateTime &request);
void SetNewDateTime(GameDateTime &dateTime);
void SetDateAndTime(std::uint8_t currentHours, std::uint8_t currentMinutes, std::uint8_t maximumHours, std::uint8_t maximumMinutes);
bool TIME_IsActive();
void TIME_EnableAdvance();
void TIME_ResetCycleTime();
void TIME_SetBlocked(bool blocked);
void TIME_SetDisabled(bool disabled);
void TIME_SetUpdateEnabled(std::uint8_t state);
void TIME_ApplyDateRange(const GameDateTime &current, const GameDateTime &maximum);
void TIME_WriteUpdateHistory(std::uint8_t *output);
void TIME_Callback(HLib::TaskData *taskData);
void TIME_Callback_0(HLib::TaskData *taskData);
void TIME_UpdateClockUI();
void TIME_SetupGVars();
void RecalculateDayInTheWeek(GameDateTime *dateTime);
void UpdateTime();
void EnqueueTIME();

} // namespace shenmue::s1
