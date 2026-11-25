#pragma once
#include "shenmue.h"
#include "common/math.hpp"
#include "common/types.hpp"

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#define SHENMUE_DATETIME_LAYOUT_NAME_(line) DateTimeLayoutCheck##line
#define SHENMUE_DATETIME_LAYOUT_NAME(line) SHENMUE_DATETIME_LAYOUT_NAME_(line)
#define SHENMUE_DATETIME_LAYOUT(condition) typedef char SHENMUE_DATETIME_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_DATETIME_ALIGNOF __alignof__
#define SHENMUE_DATETIME_NULL 0
#define SHENMUE_DATETIME_NOEXCEPT
#else
#include <cstdint>
#include <cstddef>
#include <string>
#include <span>
#define SHENMUE_DATETIME_LAYOUT(condition) static_assert(condition)
#define SHENMUE_DATETIME_ALIGNOF alignof
#define SHENMUE_DATETIME_NULL nullptr
#define SHENMUE_DATETIME_NOEXCEPT noexcept
#endif

namespace shenmue {
namespace s2 {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
enum class CameraState : std::int32_t;
#endif
struct MT7Node;
struct MT7Model;
namespace HLib {
struct Task;
}

// PC v1.07 datetime_t. It is deliberately seven bytes with no tail padding. Dreamcast keeps the ten-byte calendar record.
struct DateTime {
    std::int8_t year;
    std::int8_t month;
    std::int8_t day;
    std::uint8_t week;
    std::int8_t hour;
    std::int8_t minute;
    std::int8_t second;
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    std::uint8_t retained;
    std::uint16_t flags;
#endif
};

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
SHENMUE_DATETIME_LAYOUT(sizeof(DateTime) == 10);
SHENMUE_DATETIME_LAYOUT(SHENMUE_DATETIME_ALIGNOF(DateTime) == 2);
SHENMUE_DATETIME_LAYOUT(offsetof(DateTime, retained) == 7 && offsetof(DateTime, flags) == 8);
#else
SHENMUE_DATETIME_LAYOUT(sizeof(DateTime) == 7);
SHENMUE_DATETIME_LAYOUT(SHENMUE_DATETIME_ALIGNOF(DateTime) == 1);
#endif
SHENMUE_DATETIME_LAYOUT(offsetof(DateTime, year) == 0 && offsetof(DateTime, month) == 1 && offsetof(DateTime, day) == 2);
SHENMUE_DATETIME_LAYOUT(offsetof(DateTime, week) == 3 && offsetof(DateTime, hour) == 4 && offsetof(DateTime, minute) == 5 && offsetof(DateTime, second) == 6);
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
static const std::size_t CALENDAR_RECORD_SIZE = 10;
using ::shenmue::shen_name;
#define TASK_TMEX shen_name(SHEN_NAME4('T', 'M', 'E', 'X'))
#define WATCH_MODEL_DATA shen_name(SHEN_NAME4('W', 'A', 'T', 'C'))
#else
inline constexpr std::size_t CALENDAR_RECORD_SIZE = 10;
inline constexpr shen_name TASK_TMEX{SHEN_NAME(TMEX)};
inline constexpr shen_name WATCH_MODEL_DATA{SHEN_NAME(WATC)};
#endif

#if SHENMUE_IS_DC
typedef std::int32_t DateComparisonResult;
enum { DateComparisonResult_LessThan = 0, DateComparisonResult_Equal = 1, DateComparisonResult_GreaterThan = 2 };
#else
enum class DateComparisonResult : std::int32_t {
    LessThan = 0,
    Equal = 1,
    GreaterThan = 2,
};
#endif
// HandWatchControl uses static globals, including a flags word at +0x70.

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
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
    std::int32_t savedCameraState;
#else
    CameraState savedCameraState;
#endif
    std::int32_t savedPlayerDisplay;
    std::int32_t savedPlayerVisibility;
#if SHENMUE_IS_PC
    std::uint32_t reserved64;
#endif
    HLib::Task *task;
    std::uint32_t flags;
};
#pragma pack(pop)

SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, armPosition) == 0x0c);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, armRotation) == 0x18);
#if SHENMUE_IS_PC
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, root) == 0x28);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, reader) == 0x30);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedCameraPosition) == 0x38);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedCameraTarget) == 0x44);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedPerspective) == 0x50);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedFov) == 0x54);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedCameraState) == 0x58);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedPlayerVisibility) == 0x60);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, task) == 0x68);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, flags) == 0x70);
SHENMUE_DATETIME_LAYOUT(sizeof(HandWatchControl) == 0x74);
#else
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, root) == 0x24);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, reader) == 0x28);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedCameraPosition) == 0x2c);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedCameraTarget) == 0x38);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedPerspective) == 0x44);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedCameraState) == 0x4c);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, savedPlayerVisibility) == 0x54);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, task) == 0x58);
SHENMUE_DATETIME_LAYOUT(offsetof(HandWatchControl, flags) == 0x5c);
SHENMUE_DATETIME_LAYOUT(sizeof(HandWatchControl) == 0x60);
#endif

void shenGetCurrentDateTime(DateTime &dateTime);
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void CopyCurrentCalendarRecord(std::span<std::byte, CALENDAR_RECORD_SIZE> output);
#endif
void IncrementDateTimeInPlace(DateTime &current, const DateTime &increment);
void InitCalendar();
void InitializeNewGameCalendar();
// PC v1.07 initTimeProgram: queue-7 TIME task constructor and per-map clock setup.
void initTimeProgram();
void ComputeDayOfWeek(DateTime &dateTime);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AddDaysAndCalculateWeekday(DateTime &dateTime, std::int16_t days);
void AddDaysAndCalculateWeekdayFromCurrentDate(std::int16_t days);
#else
std::int64_t AddDaysAndCalculateWeekday(DateTime &dateTime, std::int16_t days);
std::int64_t AddDaysAndCalculateWeekdayFromCurrentDate(std::uint16_t days);
#endif
void shenGetDayInMonth(const DateTime &input, DateTime &output);
DateComparisonResult CompareDates(const DateTime &first, const DateTime &second);
void SetCurrentDateTimeTo(DateTime &dateTime);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void shenAdjustCurrentTimeByMinutes(std::int16_t minutes);
#else
std::int64_t shenAdjustCurrentTimeByMinutes(std::int16_t minutes);
#endif
// Native shenUpdateCurrentDateTime: advance to the authored morning state, including hold flags.
std::int64_t shenUpdateCurrentDateTime();
std::int32_t ComputeSecondsSince1980(const DateTime &dateTime, std::int32_t *output = SHENMUE_DATETIME_NULL);
void IncrementDateTimeBySeconds(std::uint32_t seconds, DateTime &dateTime);
void DateTime_AddMinutesInPlace(DateTime &dateTime, std::int32_t minutes);
std::int32_t CompareDateTimeSeconds(const DateTime &first, const DateTime &second);
std::int32_t DiffMinutes(const DateTime &first, const DateTime &second);
std::int32_t IsCurrentTimeRange(std::uint16_t start, std::uint16_t end);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateTempHourMinTimeScale(std::int32_t hour, std::int32_t minute, float scale);
void shenSetTimeSimulationTo(std::int8_t state);
#else
std::uint32_t UpdateTempHourMinTimeScale(std::int32_t hour, std::int32_t minute, float scale);
std::uint64_t shenSetTimeSimulationTo(std::uint32_t state);
#endif
bool IsTimeSimulationEnabled();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t SetTimeHudVariant(std::uint32_t variant);
void shenSetClockAndCompassRenderTo(std::uint8_t suppressed);
bool CheckTimeHudVisibility();
#else
std::uint32_t SetTimeHudVariant(std::int32_t variant);
void shenSetClockAndCompassRenderTo(std::int32_t suppressed) SHENMUE_DATETIME_NOEXCEPT;
bool shenIsClockAndCompassSuppressed() SHENMUE_DATETIME_NOEXCEPT;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// FormatLoadingScreenDate and FormatLoadingScreenTime produce localized labels.
void FormatLoadingScreenDate(std::span<char> output, std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t week, std::uint8_t flags);
std::string FormatLoadingScreenDate(std::int32_t year, std::int32_t month, std::uint32_t day, std::int32_t week, std::uint8_t flags);
std::string FormatLoadingScreenTime(std::int32_t hour, std::int32_t minute, std::int32_t meridiem);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void tmx_CtrlHandWatch(std::int8_t queue);
#else
void tmx_CtrlHandWatch(std::uint8_t nextFunctionIndex);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int8_t IsWristwatchOpenable();
bool OpenWristwatch(std::int32_t mode);
#else
bool IsWristwatchOpenable();
bool OpenWristwatch();
#endif
void RequestWristwatchFromHeldInput();
void CloseWristwatch(std::int32_t immediate);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CloseWristwatchImmediate();
std::int32_t IsHandWatchActive();
void CloseWristwatchImmediate2();
#endif
void SetCtrlHandWatchFlags(std::int32_t enabled);
std::int32_t GetHandWatchState();
void SetHandWatchState(std::int32_t state);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern DateTime g_calendarRecords[4];
#define g_currentDateTime (::shenmue::s2::g_calendarRecords[0])
#else
extern DateTime g_currentDateTime;
#endif
extern float g_gameTimeMilliseconds;

} // namespace s2
} // namespace shenmue

#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#undef SHENMUE_DATETIME_LAYOUT_NAME
#undef SHENMUE_DATETIME_LAYOUT_NAME_
#endif
#undef SHENMUE_DATETIME_LAYOUT
#undef SHENMUE_DATETIME_ALIGNOF
#undef SHENMUE_DATETIME_NULL
#undef SHENMUE_DATETIME_NOEXCEPT
