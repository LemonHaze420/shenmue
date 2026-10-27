#pragma once
#include "shenmue.h"
#include "common/math.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <cstddef>
#include <string>
#include <span>

namespace shenmue::s2 {
enum class CameraState : std::int32_t;
struct MT7Node;
struct MT7Model;
namespace HLib {
struct Task;
}


struct DateTime {
    std::int8_t year;
    std::int8_t month;
    std::int8_t day;
    std::uint8_t week;
    std::int8_t hour;
    std::int8_t minute;
    std::int8_t second;
};

static_assert(sizeof(DateTime) == 7);
static_assert(alignof(DateTime) == 1);
static_assert(offsetof(DateTime, year) == 0 && offsetof(DateTime, month) == 1 && offsetof(DateTime, day) == 2);
static_assert(offsetof(DateTime, week) == 3 && offsetof(DateTime, hour) == 4 && offsetof(DateTime, minute) == 5 && offsetof(DateTime, second) == 6);
inline constexpr std::size_t CALENDAR_RECORD_SIZE = 10;
inline constexpr shen_name TASK_TMEX{SHEN_NAME(TMEX)};
inline constexpr shen_name WATCH_MODEL_DATA{SHEN_NAME(WATC)};

enum class DateComparisonResult : std::int32_t {
    LessThan = 0,
    Equal = 1,
    GreaterThan = 2,
};


#pragma pack(push, 4)
struct HandWatchControl {
    std::uint16_t savedHelpVisible;
    std::uint16_t savedHelpMode;
    std::uint16_t savedHelpSelected;
    std::uint16_t reserved06;
    std::int32_t state;
    Vector3 armPosition;
    Vector3i armRotation;
#if SHENMUE_IS_PC
    std::uint32_t reserved24;
#endif
    MT7Node *root;
    MT7Model *reader;
    Vector3 savedCameraPosition;
    Vector3 savedCameraTarget;
    std::int32_t savedPerspective;
    std::int32_t savedFov;
    CameraState savedCameraState;
    std::int32_t savedPlayerDisplay;
    std::int32_t savedPlayerVisibility;
#if SHENMUE_IS_PC
    std::uint32_t reserved64;
#endif
    HLib::Task *task;
    std::uint32_t flags;
};
#pragma pack(pop)

static_assert(offsetof(HandWatchControl, armPosition) == 0x0c);
static_assert(offsetof(HandWatchControl, armRotation) == 0x18);
#if SHENMUE_IS_PC
static_assert(offsetof(HandWatchControl, root) == 0x28);
static_assert(offsetof(HandWatchControl, reader) == 0x30);
static_assert(offsetof(HandWatchControl, savedCameraPosition) == 0x38);
static_assert(offsetof(HandWatchControl, savedCameraTarget) == 0x44);
static_assert(offsetof(HandWatchControl, savedPerspective) == 0x50);
static_assert(offsetof(HandWatchControl, savedFov) == 0x54);
static_assert(offsetof(HandWatchControl, savedCameraState) == 0x58);
static_assert(offsetof(HandWatchControl, savedPlayerVisibility) == 0x60);
static_assert(offsetof(HandWatchControl, task) == 0x68);
static_assert(offsetof(HandWatchControl, flags) == 0x70);
static_assert(sizeof(HandWatchControl) == 0x74);
#else
static_assert(offsetof(HandWatchControl, root) == 0x24);
static_assert(offsetof(HandWatchControl, reader) == 0x28);
static_assert(offsetof(HandWatchControl, savedCameraPosition) == 0x2c);
static_assert(offsetof(HandWatchControl, savedCameraTarget) == 0x38);
static_assert(offsetof(HandWatchControl, savedPerspective) == 0x44);
static_assert(offsetof(HandWatchControl, savedCameraState) == 0x4c);
static_assert(offsetof(HandWatchControl, savedPlayerVisibility) == 0x54);
static_assert(offsetof(HandWatchControl, task) == 0x58);
static_assert(offsetof(HandWatchControl, flags) == 0x5c);
static_assert(sizeof(HandWatchControl) == 0x60);
#endif

void GetCurrentDateTime(DateTime &dateTime);
void CopyCurrentCalendarRecord(std::span<std::byte, CALENDAR_RECORD_SIZE> output);
void IncrementDateTimeInPlace(DateTime &current, const DateTime &increment);
void InitCalendar();
void InitializeNewGameCalendar();

void initTimeProgram();
void ComputeDayOfWeek(DateTime &dateTime);
std::int64_t AddDaysAndCalculateWeekday(DateTime &dateTime, std::int16_t days);
std::int64_t AddDaysAndCalculateWeekdayFromCurrentDate(std::uint16_t days);
void shenGetDayInMonth(const DateTime &input, DateTime &output);
DateComparisonResult CompareDates(const DateTime &first, const DateTime &second);
void SetCurrentDateTimeTo(DateTime &dateTime);
std::int64_t shenAdjustCurrentTimeByMinutes(std::int16_t minutes);

std::int64_t shenUpdateCurrentDateTime();
std::int32_t ComputeSecondsSince1980(const DateTime &dateTime, std::int32_t *output = nullptr);
void IncrementDateTimeBySeconds(std::uint32_t seconds, DateTime &dateTime);
void DateTime_AddMinutesInPlace(DateTime &dateTime, std::int32_t minutes);
std::int32_t CompareDateTimeSeconds(const DateTime &first, const DateTime &second);
std::int32_t DiffMinutes(const DateTime &first, const DateTime &second);
std::int32_t IsCurrentTimeRange(std::uint16_t start, std::uint16_t end);
std::uint32_t UpdateTempHourMinTimeScale(std::int32_t hour, std::int32_t minute, float scale);
std::uint64_t shenSetTimeSimulationTo(std::uint32_t state);
bool IsTimeSimulationEnabled();
std::uint32_t SetTimeHudVariant(std::int32_t variant);
void shenSetClockAndCompassRenderTo(std::int32_t suppressed) noexcept;
bool shenIsClockAndCompassSuppressed() noexcept;

// FormatLoadingScreenDate and FormatLoadingScreenTime produce localized labels.
void FormatLoadingScreenDate(std::span<char> output, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t week, std::uint8_t flags);
std::string FormatLoadingScreenDate(std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t week, std::uint8_t flags);
std::string FormatLoadingScreenTime(std::int32_t hour, std::int32_t minute, std::int32_t meridiem);

void tmx_CtrlHandWatch(std::uint8_t nextFunctionIndex);
bool IsWristwatchOpenable();
bool OpenWristwatch();
void RequestWristwatchFromHeldInput();
void CloseWristwatch(std::int32_t immediate);
void SetCtrlHandWatchFlags(std::int32_t enabled);

extern DateTime g_currentDateTime;

extern float g_gameTimeMilliseconds;
} // namespace shenmue::s2
