#include "s1/task/wexe.hpp"
#include "s1/task/wind.hpp"
#include "s1/task/came.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/fork.hpp"

#include "s1/file_loader.hpp"
#include "s1/date_time.hpp"
#include "s1/task/time.hpp"
#include "s1/hlib.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/scrl.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"
#include "s1/task/obje.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cmath>
#include <limits>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

std::int32_t g_weatherForecastValue = 0;
std::int32_t g_disableWeather = 0;
std::int32_t g_shouldReadWeatherForDateTime = 0;
std::uint32_t g_weatherPreviousOperand = 0;
float g_fogR = 0.0f;
float g_fogG = 0.0f;
float g_fogB = 0.0f;
std::uint32_t g_fogColour = 0;
std::int32_t g_bFogEnabled = 0;
std::uint64_t g_cloudXY[2]{};
float g_fogRenderNearDistance = 1.0f;
float g_fogRenderFarDistance = 10000.0f;
float g_fogMaximumDistance = 1.0f;
std::uint32_t g_lastSetFogColour = 0;
Vector3 g_normalizedFogColour{};
std::int32_t g_fogNearRenderFlagBits = 0;
std::int32_t g_fogFarRenderFlagBits = 0;
std::int32_t g_fogIntensityRenderFlagBits = 0;
std::int32_t g_forceFogOverride = 0;
float g_forcedFogNearDistance = 0.0f;
float g_forcedFogFarDistance = 0.0f;
HLib::Task *g_weatherExecuteTask = nullptr;
WeatherDataEntry *g_cloudGameData = nullptr;
WeatherDataEntry *g_fcasGameData = nullptr;
WeatherDataEntry *g_activeRainEntry = nullptr;
WeatherDataEntry *g_activeSnowEntry = nullptr;
WeatherDataEntry *g_activeWindEntry = nullptr;

float g_rainSpawnScale = 0.05f;
float g_rainVisualBlend = 1.0f;
std::int32_t g_activeWeatherOverrides = 0;
std::int32_t g_pendingWeatherOverrides = 0;
std::int32_t g_pendingWeatherClearOverrides = 0;
std::int32_t g_allowWeatherOverrideValues = 1;
WeatherOverrideInterpolation g_cloudOverrideInterpolation{};
WeatherOverrideInterpolation g_rainOverrideInterpolation{};
WeatherOverrideInterpolation g_snowOverrideInterpolation{};
float g_overrideCloudIntensity = 0.0f;
float g_overrideRainIntensity = 0.0f;

WeatherCameraState *g_weatherCameraState = nullptr;
std::uint32_t g_weatherWccpClipSet0 = 0;
std::uint32_t g_weatherWccpClipSet1 = 0;
namespace {
struct WeatherWccpClipStorage {
    std::uint32_t count = 0;
    std::byte *records = nullptr;
};

WeatherWccpClipStorage g_weatherWccpClipStorage[2]{};
Vector3 g_weatherClipCameraPosition{};
} // namespace

float g_overrideSnowIntensity = 0.0f;
std::uint32_t g_weatherInstructionFlags = 0;
std::int32_t g_weatherMode = 0;
std::int32_t g_wdatFileSize = 0;
void *g_wdatRoot = nullptr;
void *g_cloudNode = nullptr;
void *g_rainNode = nullptr;
void *g_snowNode = nullptr;
std::int32_t g_cloudCount = 0;
std::int32_t g_rainCount = 0;
std::int32_t g_snowCount = 0;
void *g_forecastNode = nullptr;
std::int32_t g_forecastCount = 0;
void TaskUtil_InitStaticSize(std::int64_t taskData);

extern void *g_mobjData;

namespace {

struct WeatherChunk {
    std::uint32_t token;
    std::int32_t size;
};
struct FogTableFogChunk {
    std::uint32_t token;
    std::uint32_t next;
    float red;
    float green;
    float blue;
    std::uint32_t colour;
    std::int32_t mode;
};

struct FogTableBackgroundChunk {
    std::uint32_t token;
    std::uint32_t next;
    std::uint32_t cloudColours[4];
    std::uint32_t overrideClearColour;
    std::uint32_t reserved_1c;
    std::uint32_t shouldOverrideClearColour;
    std::uint32_t scrollTextureReference;
};

static_assert(sizeof(FogTableFogChunk) == 0x1c);
static_assert(sizeof(FogTableBackgroundChunk) == 0x28);

constexpr std::uint32_t kFogChunk = static_cast<std::uint32_t>('F') | (static_cast<std::uint32_t>('O') << 8U) | (static_cast<std::uint32_t>('G') << 16U) |
    (static_cast<std::uint32_t>(' ') << 24U);
constexpr std::uint32_t kBackgroundChunk = static_cast<std::uint32_t>('B') | (static_cast<std::uint32_t>('A') << 8U) |
    (static_cast<std::uint32_t>('C') << 16U) | (static_cast<std::uint32_t>('K') << 24U);
constexpr std::size_t kFogTableEntrySize = 0x50;

const std::byte *findFogEntryChunk(const std::byte *entry, std::uint32_t wanted) {
    while (*reinterpret_cast<const std::uint32_t *>(entry) != wanted) {
        const std::uint32_t next = *reinterpret_cast<const std::uint32_t *>(entry + 4);
        if (!next)
            return nullptr;
        entry += next;
    }
    return entry;
}

void SetFogMode(std::int32_t mode) {
    g_bFogEnabled = mode == 1 ? 1 : 0;
    if (g_TaskInfo.CurrentTaskIdx >= 32)
        return;

    const std::int32_t taskIndex = g_TaskInfo.CurrentTaskIdx++;
    g_TaskInfo.pWeatherTaskModule[taskIndex] = CurrentTask;
    g_TaskInfo.pWeatherTasks[taskIndex] = nullptr;
    CurrentTask->taskFn = g_bFogEnabled == 1 ? Fog_DoFogEffectsTask : TaskUtil_InitStaticSize;
}

std::int32_t readInt32(const std::byte *address) {
    std::int32_t value;
    std::memcpy(&value, address, sizeof(value));
    return value;
}

WeatherChunk *findChunk(WeatherChunk *first, std::size_t available, std::uint32_t token) {
    if (!first)
        return nullptr;
    std::size_t offset = 0;
    auto *const bytes = reinterpret_cast<std::byte *>(first);
    while (offset + sizeof(WeatherChunk) <= available) {
        WeatherChunk header{};
        std::memcpy(&header, bytes + offset, sizeof(header));
        if (header.size < static_cast<std::int32_t>(sizeof(WeatherChunk)) || static_cast<std::size_t>(header.size) > available - offset) {
            return nullptr;
        }
        if (header.token == token) {
            return reinterpret_cast<WeatherChunk *>(bytes + offset);
        }
        offset += static_cast<std::size_t>(header.size);
    }
    return nullptr;
}

constexpr std::int32_t makeToken(char a, char b, char c, char d) noexcept {
    return static_cast<std::int32_t>(static_cast<std::uint8_t>(a)) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(b)) << 8) |
        (static_cast<std::int32_t>(static_cast<std::uint8_t>(c)) << 16) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(d)) << 24);
}

std::uint8_t g_cachedWeatherYear = 0;
std::uint8_t g_cachedWeatherMonth = 0;
std::uint8_t g_cachedWeatherDay = 0;
std::uint8_t g_cachedWeatherHour = 0;
std::uint8_t g_cachedWeatherMinute = 0;

WeatherDataEntry *allocateWeatherEntry() {
    auto *const entry = static_cast<WeatherDataEntry *>(HLib_AllocHeapMemBlock(sizeof(WeatherDataEntry), makeToken('L', 'F', 'L', 'T')));
    if (entry)
        std::memset(entry, 0, sizeof(*entry));
    return entry;
}

void freeWeatherEntry(WeatherDataEntry *&entry) {
    if (entry) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entry), 0);
        entry = nullptr;
    }
}

std::uint32_t packedWeatherDateTime(const GameDateTime &dateTime) {
    const std::int32_t year = std::clamp(static_cast<std::int32_t>(dateTime.year) - 86, 0, 15);
    const std::uint32_t month = std::clamp<std::uint32_t>(dateTime.month, 1, 12);
    const std::uint32_t day = std::clamp<std::uint32_t>(dateTime.day, 1, 31);
    const std::uint32_t hour = std::min<std::uint32_t>(dateTime.hours, 23);
    const std::uint32_t minute = std::min<std::uint32_t>(dateTime.minutes, 59);
    return (static_cast<std::uint32_t>(year) << 20U) | (month << 16U) | (day << 11U) | (hour << 6U) | minute;
}

struct WeatherRuntimeCursor {
    const std::uint32_t *current;
    float dateTimeDifference;
    float value;
};

WeatherRuntimeCursor g_cloudRuntime{};
WeatherRuntimeCursor g_rainRuntime{};
WeatherRuntimeCursor g_snowRuntime{};
WeatherRuntimeCursor g_fcasRuntime{};

void resetWeatherCursor(WeatherRuntimeCursor &cursor, void *records) {
    cursor.current = static_cast<const std::uint32_t *>(records);
    cursor.dateTimeDifference = 0.0f;
    cursor.value = 0.0f;
}

float readWeatherIntensity(WeatherRuntimeCursor &cursor, std::uint32_t wanted) {
    const std::uint32_t *current = cursor.current;
    const std::uint32_t *next = current + 1;
    std::uint32_t currentDate;
    std::uint32_t nextDate;
    for (;;) {
        for (;;) {
            currentDate = *current & 0x00ffffffU;
            nextDate = *next & 0x00ffffffU;
            if (nextDate > wanted)
                break;
            ++current;
            ++next;
        }
        if (wanted >= currentDate)
            break;
        --current;
        --next;
    }
    cursor.current = current;
    cursor.dateTimeDifference = static_cast<float>(wanted - currentDate) / static_cast<float>(nextDate - currentDate);
    const float currentValue = static_cast<float>(*current >> 24U) / 255.0f;
    float step = static_cast<float>(*next >> 24U) / 255.0f - currentValue;
    if (std::fabs(step) < std::numeric_limits<float>::epsilon() || std::fabs(step *= cursor.dateTimeDifference) < std::numeric_limits<float>::epsilon()) {
        step = 0.0f;
    }
    cursor.value = step + currentValue;
    return cursor.value;
}

std::uint8_t readForecast(const GameDateTime &dateTime) {
    if (dateTime.year < 86)
        return 0;
    const std::uint32_t packedDate = static_cast<std::uint32_t>(static_cast<std::uint8_t>(dateTime.day) |
                                         (32U *
                                             (static_cast<std::uint32_t>(static_cast<std::uint8_t>(dateTime.month)) |
                                                 (32U * static_cast<std::uint32_t>(static_cast<std::uint8_t>(dateTime.year - 86))))))
        << 16U;
    const auto *const records = static_cast<const std::uint32_t *>(g_forecastNode);
    for (std::int32_t index = 0; index < g_forecastCount; ++index) {
        if ((records[index] & 0xbfff0000U) == packedDate) {
            return static_cast<std::uint8_t>(records[index] >> 8U);
        }
    }
    return 0;
}

std::int32_t g_weatherDataYear = 0;
std::int32_t g_weatherDataMonth = 1;
std::int32_t g_weatherDataDay = 1;
float g_weatherCloudValue = 0.0f;
float g_weatherRainValue = 0.0f;
float g_weatherSnowValue = 0.0f;
float g_weatherFcasValue = 0.0f;

constexpr std::array<std::uint8_t, 31> kWindDaysA{
    0x09, 0x07, 0x07, 0x07, 0x08, 0x08, 0x07, 0x05,
    0xb8, 0x0a, 0x09, 0xf9, 0xf9, 0x15, 0xf4, 0xa7,
    0xbc, 0x06, 0x08, 0x18, 0x15, 0x07, 0xab, 0xac,
    0x0a, 0x07, 0x54, 0x53, 0xb7, 0x16, 0x19};
constexpr std::array<std::uint8_t, 28> kWindDaysFebruary{
    0x08, 0xf7, 0xe7, 0x09, 0x04, 0x54, 0x04, 0x06,
    0x04, 0x75, 0xbc, 0xad, 0x35, 0xa9, 0x06, 0x08,
    0x15, 0x08, 0x09, 0x07, 0x16, 0x07, 0xf5, 0xaa,
    0xbc, 0xb9, 0xb8, 0x06};
constexpr std::array<std::uint8_t, 31> kWindDaysB{
    0xf9, 0x07, 0xa4, 0xba, 0xb7, 0xbb, 0x09, 0x08,
    0x0b, 0x0e, 0x08, 0x45, 0xf5, 0xa9, 0x08, 0x06,
    0x06, 0x0a, 0x34, 0x08, 0x46, 0x04, 0x05, 0xac,
    0xad, 0x08, 0x87, 0x56, 0x07, 0x9a, 0x0b};

std::uint8_t weatherWindScheduleByte() {
    const std::int32_t month = std::clamp(g_weatherDataMonth - 1, 0, 11);
    const std::uint8_t *days = kWindDaysA.data();
    std::size_t dayCount = kWindDaysA.size();
    if (month == 1) {
        days = kWindDaysFebruary.data();
        dayCount = kWindDaysFebruary.size();
    } else if (month >= 2 && (month & 1) == 0) {
        days = kWindDaysB.data();
        dayCount = kWindDaysB.size();
    }
    const std::size_t day = static_cast<std::size_t>(std::clamp(g_weatherDataDay - 1, 0, static_cast<std::int32_t>(dayCount - 1)));
    return days[day];
}

std::int32_t weatherWindAngleForDate() {
    return static_cast<std::int32_t>(weatherWindScheduleByte() >> 4U) << 12U;
}

float weatherWindStrengthForDate() {
    return static_cast<float>(weatherWindScheduleByte() & 0x0fU) / 8.0f;
}

void applyScheduledWind(WeatherWindData &wind) {
    wind.targetAngle = weatherWindAngleForDate();
    wind.angleStep = (wind.targetAngle - wind.currentAngle) / 10;
    wind.angleFramesRemaining = 10.0f;
    wind.gustFramesRemaining = 0.0f;
    wind.targetStrength = weatherWindStrengthForDate() / 30.0f;
    wind.strengthDelta = wind.targetStrength - wind.currentStrength;
    wind.strengthFramesRemaining = 1.0f;
    wind.gustFramesRemaining = 0.0f;
}

float clampWeatherValue(float value) {
    return std::fabs(value) < std::numeric_limits<float>::epsilon() ? 0.0f : value;
}

GameDateTime nextMinute(GameDateTime result) {
    static constexpr std::array<std::uint8_t, 13> kDaysPerMonth{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    result.seconds = 0;
    ++result.minutes;
    if (result.minutes > 59) {
        result.minutes = 0;
        ++result.hours;
        if (result.hours > 23) {
            result.hours = 0;
            const bool leapYear = (result.year & 3U) == 0U && result.month == 2;
            const std::uint8_t days = leapYear ? 29 : kDaysPerMonth[std::min<std::size_t>(result.month, 12)];
            ++result.day;
            if (result.day > days) {
                result.day = 1;
                ++result.month;
                if (result.month > 12) {
                    result.month = 1;
                    ++result.year;
                    if (result.year > 99)
                        result.year = 0;
                }
            }
        }
    }
    RecalculateDayInTheWeek(&result);
    return result;
}
} // namespace
std::int64_t Weather_AdvanceWeatherEntry(WeatherDataEntry *entry) {
    if (entry->currentStep == entry->maxStepCount)
        return 0;
    const std::int32_t newStep = ++entry->currentStep;
    if (newStep == entry->maxStepCount) {
        entry->intensity = entry->targetIntensity;
        return 2;
    }
    entry->intensity += entry->incrementStep;
    if (std::fabs(entry->intensity) < std::numeric_limits<float>::epsilon())
        entry->intensity = 0.0f;
    return 1;
}

bool WEXE_LoadWccpClipSet(std::uint32_t setIndex, const void *sourceRecords, std::uint32_t count) {
    if (setIndex >= 2)
        return false;

    auto &storage = g_weatherWccpClipStorage[setIndex];
    auto &enabled = setIndex == 0 ? g_weatherWccpClipSet0 : g_weatherWccpClipSet1;
    enabled = 1;
    storage.count = count;
    if (storage.records)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(storage.records), 0);

    storage.records = static_cast<std::byte *>(HLib_AllocHeapMemBlock(static_cast<std::size_t>(count) * 0x3c, makeToken('W', 'C', 'C', 'P')));
    const auto *source = static_cast<const std::byte *>(sourceRecords);
    for (std::uint32_t index = 0; index < count; ++index) {
        auto *const destination = storage.records + static_cast<std::size_t>(index) * 0x3c;
        const std::uint32_t active = 1;
        std::memcpy(destination, &active, sizeof(active));
        std::memcpy(destination + sizeof(active), source + static_cast<std::size_t>(index) * 0x38, 0x38);
    }
    return storage.records != nullptr || count == 0;
}

void WEXE_ReleaseWccpClipSet(std::uint32_t setIndex) {
    if (setIndex >= 2)
        return;

    auto &storage = g_weatherWccpClipStorage[setIndex];
    if (!storage.records)
        return;

    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(storage.records), 0);
    storage = {};
    (setIndex == 0 ? g_weatherWccpClipSet0 : g_weatherWccpClipSet1) = 0;
}

void WEXE_SetWccpClipSetEnabled(std::uint32_t setIndex, std::uint32_t value) {
    if (setIndex >= 2 || !g_weatherWccpClipStorage[setIndex].records)
        return;
    (setIndex == 0 ? g_weatherWccpClipSet0 : g_weatherWccpClipSet1) = value;
}

void WEXE_SetWccpClipRecordActive(std::uint32_t setIndex, std::int32_t recordIndex, std::uint32_t value) {
    if (setIndex >= 2)
        return;

    auto &storage = g_weatherWccpClipStorage[setIndex];
    if (!storage.records || static_cast<std::int32_t>(storage.count) <= recordIndex)
        return;
    std::memcpy(storage.records + static_cast<std::size_t>(recordIndex > 0 ? recordIndex : 0) * 0x3c, &value, sizeof(value));
}

void WEXE_SetWccpClipSet0(std::uint32_t value) {
    WEXE_SetWccpClipSetEnabled(0, value);
}

void WEXE_SetWccpClipSet1(std::uint32_t value) {
    WEXE_SetWccpClipSetEnabled(1, value);
}

void Weather_UpdateWCCPClipSet(std::uint32_t setIndex, const Vector3 &cameraPosition) {
    const WeatherWccpClipStorage &storage = g_weatherWccpClipStorage[setIndex];
    if (!storage.records || !(setIndex == 0 ? g_weatherWccpClipSet0 : g_weatherWccpClipSet1))
        return;
    const std::byte *selected = nullptr;
    float nearest = 1.0e10f;
    for (std::uint32_t index = 0; index < storage.count; ++index) {
        const std::byte *const record = storage.records + static_cast<std::size_t>(index) * 0x3c;
        std::uint32_t active = 0;
        Vector3 center{};
        float radius = 0.0f;
        std::memcpy(&active, record, sizeof(active));
        std::memcpy(&center, record + 0x2c, sizeof(center));
        std::memcpy(&radius, record + 0x38, sizeof(radius));
        if (!active || !ClipScreenVisibility(center, radius, 32.0f))
            continue;
        const float distance = (center.z - cameraPosition.z) * (center.z - cameraPosition.z) + (center.x - cameraPosition.x) * (center.x - cameraPosition.x);
        if (nearest > distance) {
            nearest = distance;
            selected = record + 4;
        }
    }
    if (!selected) {
        EFFE_ResetWccpClipSet(setIndex);
        return;
    }
    Vector2 corners[4]{};
    float bounds[2]{};
    std::memcpy(corners, selected, sizeof(corners));
    std::memcpy(bounds, selected + sizeof(corners), sizeof(bounds));
    EFFE_ApplyWccpClipQuad(
        setIndex, 7U, corners, static_cast<float>(static_cast<double>(bounds[0]) - 0.1), static_cast<float>(static_cast<double>(bounds[1]) + 0.1));
}

void Weather_ResetWCCPClipSet0() {
    EFFE_ResetWccpClipSet(0);
}

void Weather_ResetWCCPClipSet1() {
    EFFE_ResetWccpClipSet(1);
}



WeatherCameraState *ICAR() {
    auto *const record = static_cast<WeatherCameraState *>(HLib_AllocHeapMemBlock(sizeof(WeatherCameraState), SHEN_NAME(ICAR)));
    record->previousEye = g_CameraEyePos;
    record->eye = g_CameraEyePos;
    record->previousTarget = g_CameraTargetPos;
    record->target = g_CameraTargetPos;
    record->direction = {};
    record->previousDirection = {};
    return record;
}



void WEXE_UpdateWeatherCameraPosition() {
    WeatherCameraState *const record = g_weatherCameraState;
    record->previousEye = record->eye;
    record->previousTarget = record->target;
    record->previousDirection = record->direction;
    record->eye = g_CameraEyePos;
    record->target = g_CameraTargetPos;
    Vector3 &direction = record->direction;
    direction = {record->target.x - record->eye.x, record->target.y - record->eye.y, record->target.z - record->eye.z};
    const float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (magnitude > 0.0f) {
        direction.x *= 1.0f / magnitude;
        direction.y *= 1.0f / magnitude;
        direction.z *= 1.0f / magnitude;
    }
}
void WEXE_ReadDataForDateTime(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute) {
    std::int32_t weatherYear = static_cast<std::int32_t>(year) - 86;
    std::int32_t weatherMonth = static_cast<std::int32_t>(month);
    std::int32_t weatherDay = static_cast<std::int32_t>(day);
    std::int32_t weatherHour = static_cast<std::int32_t>(hour);
    std::int32_t weatherMinute = static_cast<std::int32_t>(minute);
    if (weatherYear < 0) {
        weatherYear = 0;
        weatherMonth = 1;
        weatherDay = 1;
        weatherHour = 0;
        weatherMinute = 0;
    } else if (weatherYear > 15) {
        weatherYear = 15;
        weatherMonth = 12;
        weatherDay = 31;
        weatherHour = 23;
        weatherMinute = 59;
    } else if (weatherMonth < 1) {
        weatherMonth = 1;
        weatherDay = 1;
        weatherHour = 0;
        weatherMinute = 0;
    } else if (weatherMonth > 12) {
        weatherMonth = 12;
        weatherDay = 31;
        weatherHour = 23;
        weatherMinute = 59;
    } else if (weatherDay < 1) {
        weatherDay = 1;
        weatherHour = 0;
        weatherMinute = 0;
    } else if (weatherDay > 31) {
        weatherDay = 31;
        weatherHour = 23;
        weatherMinute = 59;
    } else if (weatherHour < 0) {
        weatherHour = 0;
        weatherMinute = 0;
    } else if (weatherHour > 23) {
        weatherHour = 23;
        weatherMinute = 59;
    } else if (weatherMinute < 0) {
        weatherMinute = 0;
    } else if (weatherMinute > 59) {
        weatherMinute = 59;
    }
    const std::uint32_t packedDateTime = (static_cast<std::uint32_t>(weatherYear) << 20U) | (static_cast<std::uint32_t>(weatherMonth) << 16U) |
        (static_cast<std::uint32_t>(weatherDay) << 11U) | (static_cast<std::uint32_t>(weatherHour) << 6U) | static_cast<std::uint32_t>(weatherMinute);
    g_weatherDataYear = weatherYear;
    g_weatherDataMonth = weatherMonth;
    g_weatherDataDay = weatherDay;
    g_weatherCloudValue = clampWeatherValue(readWeatherIntensity(g_cloudRuntime, packedDateTime));
    g_weatherRainValue = clampWeatherValue(readWeatherIntensity(g_rainRuntime, packedDateTime));
    g_weatherSnowValue = readWeatherIntensity(g_snowRuntime, packedDateTime);
    g_weatherFcasValue = readWeatherIntensity(g_fcasRuntime, packedDateTime);
}

namespace {

float weatherRealCloud() {
    if ((g_activeWeatherOverrides & kWeatherOverrideCloud) != 0 && g_allowWeatherOverrideValues) {
        return g_overrideCloudIntensity;
    }
    return clampWeatherValue(g_weatherCloudValue);
}
float weatherRealRain() {
    if ((g_activeWeatherOverrides & kWeatherOverrideRain) != 0 && g_allowWeatherOverrideValues) {
        return g_overrideRainIntensity;
    }
    return clampWeatherValue(g_weatherRainValue);
}
float weatherRealSnow() {
    return (g_activeWeatherOverrides & kWeatherOverrideSnow) != 0 && g_allowWeatherOverrideValues ? g_overrideSnowIntensity : g_weatherSnowValue;
}
float weatherRealFcas() {
    return clampWeatherValue(g_weatherFcasValue);
}
void setWeatherValues() {
    WEXE_ReadDataForDateTime(g_currentDateTime.year, g_currentDateTime.month, g_currentDateTime.day, g_currentDateTime.hours, g_currentDateTime.minutes);
    g_shouldReadWeatherForDateTime = 0;
    WEXE_ApplyCloudGameData(weatherRealCloud(), 0);
    WEXE_ApplyFcasGameData(weatherRealFcas(), 0);
    Weather_SetForcedRainValue(weatherRealRain());
    Weather_SetForcedSnowValue(weatherRealSnow());
    if (WeatherWindData *const wind = WIND_GetWeatherData())
        applyScheduledWind(*wind);
    g_cachedWeatherMonth = g_currentDateTime.month;
    g_cachedWeatherDay = g_currentDateTime.day;
    g_cachedWeatherMinute = g_currentDateTime.minutes;
    g_cachedWeatherYear = g_currentDateTime.year;
    g_cachedWeatherHour = g_currentDateTime.hours;
}
void processWeatherDelta(std::int32_t deltaMinutes) {
    g_shouldReadWeatherForDateTime = 0;
    const std::int32_t totalHours = deltaMinutes / 240 / 60;
    const std::uint32_t minutes = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_currentDateTime.minutes) + deltaMinutes / 240 % 60);
    const std::uint32_t day = static_cast<std::uint32_t>(totalHours / 24 + static_cast<std::int32_t>(g_currentDateTime.day));
    const std::uint32_t hours = static_cast<std::uint32_t>(totalHours % 24 + static_cast<std::int32_t>(g_currentDateTime.hours));
    WEXE_ReadDataForDateTime(g_currentDateTime.year, g_currentDateTime.month, day, hours, minutes);
    WEXE_ApplyCloudGameData(weatherRealCloud(), deltaMinutes);
    WEXE_ApplyFcasGameData(weatherRealFcas(), deltaMinutes);
    WEXE_ApplyRainGameData(weatherRealRain(), deltaMinutes);
    WEXE_ApplySnowGameData(weatherRealSnow(), deltaMinutes);
    if (WeatherWindData *const wind = WIND_GetWeatherData())
        applyScheduledWind(*wind);
    g_cachedWeatherMonth = g_currentDateTime.month;
    g_cachedWeatherMinute = static_cast<std::uint8_t>(minutes);
    g_cachedWeatherYear = g_currentDateTime.year;
    g_cachedWeatherDay = static_cast<std::uint8_t>(day);
    g_cachedWeatherHour = static_cast<std::uint8_t>(hours);
}
void setWeatherOverride(WeatherOverrideInterpolation &interpolation, float current, float target, std::int32_t steps) {
    interpolation.stepsRemain = steps;
    interpolation.curr = current;
    interpolation.target = target;
    interpolation.step = steps ? (target - current) / static_cast<float>(steps) : 0.0f;
}
} // namespace

namespace {
enum class WeatherSpriteMode : std::int32_t {
    ClearMorning,
    ClearDay,
    ClearSunset,
    ClearNight,
    RainOrSnow,
    Overcast,
};

struct WeatherLightingSample {
    Vector3 direction;
    Vector4 colour;
    float ambient;
    float intensity;
    std::array<std::uint32_t, 3> particleColours;
};

struct WeatherFogSample {
    Vector3 parameters;
    std::uint32_t colour;
    std::array<std::uint32_t, 2> cloudColours;
};

struct WeatherSample {
    std::int32_t seconds;
    WeatherLightingSample lighting;
    WeatherFogSample fog;
    WeatherFogSample forecastFog;
};

struct WeatherTableDescriptor {
    const WeatherSample *samples;
    std::int32_t count;
    std::uint32_t reserved00c;
};

struct alignas(8) WeatherColourData {
    std::array<float, 4> current;
    std::array<float, 4> target;
    std::byte interpolationState[0x18];
};

struct WeatherFloatData {
    float current;
    float target;
    float step;
    std::int32_t currentStep;
    std::int32_t maxStepCount;
};

struct WeatherTaskRuntime {
    std::uint32_t lightIndex;
    std::uint32_t reserved004;
    WeatherTableDescriptor *tables;
    std::uint32_t tableCount;
    std::uint32_t instructionFlags;
    std::int32_t month;
    std::int32_t day;
    std::int32_t hour;
    std::int32_t minute;
    std::int32_t second;
    std::int32_t seconds;
    float clud;
    float rain;
    float snow;
    float fcas;
    std::int32_t overcast;
    std::int32_t rainOrSnow;
    WeatherSpriteMode spriteMode;
    std::uint32_t reserved04c;
    std::array<WeatherColourData *, 3> particleColours;
    std::array<WeatherFloatData *, 2> scalarState;
    std::array<WeatherColourData *, 3> auxiliaryColours;
};

static_assert(sizeof(WeatherLightingSample) == 0x30);
static_assert(sizeof(WeatherFogSample) == 0x18);
static_assert(sizeof(WeatherSample) == 0x64);
static_assert(offsetof(WeatherSample, lighting) == 4);
static_assert(offsetof(WeatherSample, fog) == 0x34);
static_assert(offsetof(WeatherSample, forecastFog) == 0x4c);
static_assert(sizeof(WeatherTableDescriptor) == 0x10);
static_assert(sizeof(WeatherColourData) == 0x38);
static_assert(sizeof(WeatherFloatData) == 0x14);
static_assert(sizeof(WeatherTaskRuntime) == 0x90);
static_assert(alignof(WeatherTaskRuntime) == 8);
static_assert(offsetof(WeatherTaskRuntime, month) == 0x18);
static_assert(offsetof(WeatherTaskRuntime, clud) == 0x30);
static_assert(offsetof(WeatherTaskRuntime, overcast) == 0x40);
static_assert(offsetof(WeatherTaskRuntime, spriteMode) == 0x48);
static_assert(offsetof(WeatherTaskRuntime, particleColours) == 0x50);
static_assert(offsetof(WeatherTaskRuntime, scalarState) == 0x68);
static_assert(offsetof(WeatherTaskRuntime, auxiliaryColours) == 0x78);
static_assert(offsetof(MapControlData, direction) == 0x7bc);

#include "weather_tables.inc"

std::array<WeatherTableDescriptor, 4> g_defaultWeatherTables{{
    {kClearWeatherSamples.data(), 12, 0},
    {kOvercastWeatherSamples.data(), 10, 0},
    {kRainWeatherSamples.data(), 9, 0},
    {kSnowWeatherSamples.data(), 9, 0},
}};

constexpr std::array<const char *, 6> kWeatherSpriteNames{"SCROLL60.SPR", "SCROLL61.SPR", "SCROLL66.SPR", "SCROLL63.SPR", "SCROLL64.SPR", "SCROLL65.SPR"};

std::int32_t g_shouldParseWeatherInstructions = 1;
WeatherTaskRuntime *g_weatherTaskRuntime = nullptr;

WeatherColourData *Weather_AllocateColourData() {
    auto *data = static_cast<WeatherColourData *>(HLib_AllocHeapMemBlock(sizeof(WeatherColourData), makeToken('L', 'R', 'G', 'B')));
    if (data)
        std::memset(data, 0, sizeof(*data));
    return data;
}

WeatherFloatData *Weather_AllocateFloatData() {
    void *const data = HLib_AllocHeapMemBlock(sizeof(WeatherFloatData), makeToken('L', 'F', 'L', 'T'));

    if (data)
        std::memset(data, 0, 0x18);
    return static_cast<WeatherFloatData *>(data);
}

WeatherTaskRuntime *GetWeatherTaskData() {
    auto *data = static_cast<WeatherTaskRuntime *>(HLib_AllocHeapMemBlock(sizeof(WeatherTaskRuntime), makeToken('W', 'A', 'I', 'R')));
    if (!data)
        return nullptr;
    data->lightIndex = 0;
    data->tables = g_defaultWeatherTables.data();
    data->tableCount = 4;
    data->instructionFlags = 0x1fff;
    data->month = g_currentDateTime.month;
    data->day = g_currentDateTime.day;
    data->hour = g_currentDateTime.hours;
    data->minute = g_currentDateTime.minutes;
    data->second = g_currentDateTime.seconds;
    data->seconds = 3600 * data->hour + 60 * data->minute + data->second;
    data->clud = 0.0f;
    data->rain = 0.0f;
    data->snow = 0.0f;
    data->fcas = 0.0f;
    data->overcast = 0;
    data->rainOrSnow = 0;
    data->spriteMode = static_cast<WeatherSpriteMode>(-1);
    for (auto *&colour : data->particleColours)
        colour = Weather_AllocateColourData();
    for (auto *&scalar : data->scalarState)
        scalar = Weather_AllocateFloatData();
    for (auto *&colour : data->auxiliaryColours)
        colour = Weather_AllocateColourData();
    return data;
}

void Weather_Free(WeatherTaskRuntime *data) {
    for (auto *colour : data->particleColours)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(colour), makeToken('L', 'R', 'G', 'B'));
    for (auto *scalar : data->scalarState)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(scalar), makeToken('L', 'F', 'L', 'T'));
    for (auto *colour : data->auxiliaryColours)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(colour), makeToken('L', 'R', 'G', 'B'));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
}

WeatherSpriteMode GetClearSkySpriteMode(std::int32_t hour, std::int32_t minute) {
    const std::int32_t minutes = minute + 60 * hour;
    if (minutes < 330 || minutes >= 1080)
        return WeatherSpriteMode::ClearNight;
    if (minutes < 450)
        return WeatherSpriteMode::ClearMorning;
    if (minutes < 990)
        return WeatherSpriteMode::ClearDay;
    return WeatherSpriteMode::ClearSunset;
}

float Weather_InterpolateValue(float current, float next, float blend) {
    float step = next - current;
    if (std::fabs(step) < std::numeric_limits<float>::epsilon())
        step = 0.0f;
    else
        step = clampWeatherValue(step * blend);
    return step + current;
}

std::uint32_t Weather_InterpolateColour(std::uint32_t current, std::uint32_t next, float blend) {
    std::uint32_t result = 0;
    for (std::uint32_t channel = 0; channel < 4; ++channel) {
        const auto shift = 24U - 8U * channel;
        const auto currentByte = static_cast<std::int32_t>((current >> shift) & 0xffU);
        const auto nextByte = static_cast<std::int32_t>((next >> shift) & 0xffU);
        const auto step = static_cast<std::int32_t>(clampWeatherValue(static_cast<float>(nextByte - currentByte) * blend));
        const auto value = currentByte + step;
        result = (result << 8U) | static_cast<std::uint32_t>(value);
    }
    return result;
}

std::uint32_t Weather_AccumulateColour(std::uint32_t current, std::uint32_t next, float scale) {
    const auto scaled = Weather_InterpolateColour(0, next, scale);
    std::uint32_t result = 0;
    for (std::uint32_t channel = 0; channel < 4; ++channel) {
        const auto shift = 24U - 8U * channel;
        const auto sum = ((current >> shift) & 0xffU) + ((scaled >> shift) & 0xffU);
        result = (result << 8U) | std::min(sum, 255U);
    }
    return result;
}

void Weather_InterpolateLightingSample(const WeatherLightingSample &current, const WeatherLightingSample &next, float blend, WeatherLightingSample &output) {
    output.direction = {Weather_InterpolateValue(current.direction.x, next.direction.x, blend),
        Weather_InterpolateValue(current.direction.y, next.direction.y, blend), Weather_InterpolateValue(current.direction.z, next.direction.z, blend)};
    output.colour = {Weather_InterpolateValue(current.colour.x, next.colour.x, blend), Weather_InterpolateValue(current.colour.y, next.colour.y, blend),
        Weather_InterpolateValue(current.colour.z, next.colour.z, blend), Weather_InterpolateValue(current.colour.w, next.colour.w, blend)};
    output.ambient = Weather_InterpolateValue(current.ambient, next.ambient, blend);
    output.intensity = Weather_InterpolateValue(current.intensity, next.intensity, blend);
    for (std::size_t index = 0; index < output.particleColours.size(); ++index)
        output.particleColours[index] = Weather_InterpolateColour(current.particleColours[index], next.particleColours[index], blend);
}

void Weather_InterpolateFogSample(const WeatherFogSample &current, const WeatherFogSample &next, float blend, WeatherFogSample &output) {
    output.parameters = {Weather_InterpolateValue(current.parameters.x, next.parameters.x, blend),
        Weather_InterpolateValue(current.parameters.y, next.parameters.y, blend), Weather_InterpolateValue(current.parameters.z, next.parameters.z, blend)};
    output.colour = Weather_InterpolateColour(current.colour, next.colour, blend);
    for (std::size_t index = 0; index < output.cloudColours.size(); ++index)
        output.cloudColours[index] = Weather_InterpolateColour(current.cloudColours[index], next.cloudColours[index], blend);
}

void Weather_AccumulateLightingSample(WeatherLightingSample &output, const WeatherLightingSample &input, float scale) {
    const auto accumulate = [scale](float current, float next) { return clampWeatherValue(clampWeatherValue(scale * next) + current); };
    output.direction.x = accumulate(output.direction.x, input.direction.x);
    output.direction.y = accumulate(output.direction.y, input.direction.y);
    output.direction.z = accumulate(output.direction.z, input.direction.z);
    output.colour.x = accumulate(output.colour.x, input.colour.x);
    output.colour.y = accumulate(output.colour.y, input.colour.y);
    output.colour.z = accumulate(output.colour.z, input.colour.z);
    output.colour.w = accumulate(output.colour.w, input.colour.w);
    output.ambient = accumulate(output.ambient, input.ambient);
    output.intensity = accumulate(output.intensity, input.intensity);
    for (std::size_t index = 0; index < output.particleColours.size(); ++index)
        output.particleColours[index] = Weather_AccumulateColour(output.particleColours[index], input.particleColours[index], scale);
}

void Weather_AccumulateFogSample(WeatherFogSample &output, const WeatherFogSample &input, float scale) {
    output.parameters.x = clampWeatherValue(clampWeatherValue(scale * input.parameters.x) + output.parameters.x);
    output.parameters.y = clampWeatherValue(clampWeatherValue(scale * input.parameters.y) + output.parameters.y);
    output.parameters.z = clampWeatherValue(clampWeatherValue(scale * input.parameters.z) + output.parameters.z);
    output.colour = Weather_AccumulateColour(output.colour, input.colour, scale);
    for (std::size_t index = 0; index < output.cloudColours.size(); ++index)
        output.cloudColours[index] = Weather_AccumulateColour(output.cloudColours[index], input.cloudColours[index], scale);
}

void Weather_SampleTable(const WeatherTableDescriptor &table, std::int32_t seconds, WeatherSample &output) {
    assert(table.samples && table.count > 0);
    if (table.count == 1) {
        output = {};
        Weather_AccumulateLightingSample(output.lighting, table.samples->lighting, 1.0f);
        Weather_AccumulateFogSample(output.fog, table.samples->fog, 1.0f);
        Weather_AccumulateFogSample(output.forecastFog, table.samples->forecastFog, 1.0f);
        return;
    }
    const WeatherSample *current = table.samples;
    const WeatherSample *next = current + 1;
    std::int32_t startSeconds = 0;
    std::int32_t endSeconds = 0;
    std::int32_t index = 1;
    for (; index < table.count; ++index, ++current, ++next) {
        startSeconds = current->seconds;
        endSeconds = next->seconds;
        if (startSeconds <= seconds && seconds < endSeconds)
            break;
    }
    if (index == table.count) {
        next = table.samples;
        endSeconds = 86400;

    }
    const float blend = static_cast<float>(seconds - startSeconds) / static_cast<float>(endSeconds - startSeconds);
    Weather_InterpolateLightingSample(current->lighting, next->lighting, blend, output.lighting);
    Weather_InterpolateFogSample(current->fog, next->fog, blend, output.fog);
    Weather_InterpolateFogSample(current->forecastFog, next->forecastFog, blend, output.forecastFog);
    output.seconds = seconds;
}

void Weather_MixTables(
    const WeatherTableDescriptor *tables, std::uint32_t count, std::int32_t seconds, float clud, float rain, float snow, WeatherSample &output) {
    const float cloudWeight = std::fmin(1.0f, clud * 3.0f);
    const float rainScale = rain * 3.0f;
    const float snowScale = snow * 3.0f;
    const float rainWeight = std::fmin(1.0f, rainScale);
    const float snowWeight = std::fmin(1.0f, snowScale);
    const float time =
        static_cast<float>(100 * g_currentDateTime.hours - static_cast<std::int32_t>(static_cast<double>(g_currentDateTime.minutes) / 60.0 * -100.0));
    float daylight = 1.0f;
    if (time > 1650.0f && time < 1800.0f)
        daylight = (1800.0f - time) / 150.0f;
    else if (time >= 1800.0f || time < 600.0f)
        daylight = 0.0f;
    const float overcastWeight = std::fmax(0.0f, cloudWeight - daylight * (snowWeight > rainWeight ? snowScale : rainScale));
    const float clearWeight = 1.0f - std::fmax(snowWeight, std::fmax(rainWeight, overcastWeight));
    const float total = ((clearWeight + overcastWeight) + rainWeight) + snowWeight;
    const std::array<float, 4> weights{clampWeatherValue(clearWeight / total), clampWeatherValue(overcastWeight / total), clampWeatherValue(rainWeight / total),
        clampWeatherValue(snowWeight / total)};
    assert(count <= weights.size());
    output = {};
    for (std::uint32_t index = 0; index < count; ++index) {
        const float weight = weights[index];
        if (!(weight > 0.0f))
            continue;
        WeatherSample sample;
        Weather_SampleTable(tables[index], seconds, sample);
        output.seconds = static_cast<std::int32_t>(static_cast<float>(output.seconds) + clampWeatherValue(static_cast<float>(sample.seconds) * weight));
        Weather_AccumulateLightingSample(output.lighting, sample.lighting, weight);
        Weather_AccumulateFogSample(output.fog, sample.fog, weight);
        Weather_AccumulateFogSample(output.forecastFog, sample.forecastFog, weight);
    }
}

void Weather_ParseInstruction(WeatherTaskRuntime &data) {
    WeatherSample sample;
    Weather_MixTables(data.tables, data.tableCount, data.seconds, data.clud, data.rain, data.snow, sample);
    WeatherFogSample fog;
    Weather_InterpolateFogSample(sample.fog, sample.forecastFog, data.fcas, fog);
    const auto instruction = g_weatherInstructionFlags;
    if (instruction & 1U) {
        const LightDateTime dateTime{data.month, data.day, data.hour, data.minute, data.second};
        Vector3 direction = UpdateSunForDateTime(dateTime, 22.18f, 1);
        direction.x = clampWeatherValue(direction.x);
        direction.y = clampWeatherValue(direction.y);
        direction.z = clampWeatherValue(direction.z);
        const auto *control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
        if (control->direction) {
            const auto angle = static_cast<std::uint16_t>(0U - control->direction);
            const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
            const float cosine = sin_fixed16(fixed16_cosine_angle(angle));
            const float x = direction.x;
            const float z = direction.z;
            direction.x = clampWeatherValue(clampWeatherValue(cosine * x) - clampWeatherValue(sine * z));
            direction.z = clampWeatherValue(clampWeatherValue(cosine * z) + clampWeatherValue(sine * x));
        }
        SetEffectTransformByIndex(data.lightIndex, &direction);
    }
    if (instruction & 2U)
        SetRGBStruct(data.lightIndex, &sample.lighting.colour);
    const float ambient = sample.lighting.ambient;
    Weather::g_tempOverrideLighting.x = ambient;
    if (instruction & 4U) {
        g_sceneLightingBaseColour = {ambient, ambient, ambient, ambient};
        g_sceneLightingActiveColour = g_sceneLightingBaseColour;
    }
    Weather::g_tempOverrideLighting.y = sample.lighting.intensity;
    if (instruction & 8U)
        SetLightIntensity(data.lightIndex, sample.lighting.intensity);
    for (std::size_t index = 0; index < data.particleColours.size(); ++index) {
        if (!(instruction & (0x10U << index)))
            continue;
        auto &output = *data.particleColours[index];
        const auto colour = sample.lighting.particleColours[index];
        for (std::uint32_t channel = 0; channel < 4; ++channel) {
            const float value = static_cast<float>((colour >> (24U - 8U * channel)) & 0xffU);
            output.target[channel] = value;
            output.current[channel] = value;
        }
        std::memset(output.interpolationState, 0, sizeof(output.interpolationState));
    }
    if (instruction & 0x80U)
        g_fogR = fog.parameters.x;
    if (instruction & 0x100U)
        g_fogG = fog.parameters.y;
    if (instruction & 0x200U)
        g_fogB = fog.parameters.z;
    if (instruction & 0x400U)
        g_fogColour = fog.colour & 0xffffffU;
    if (instruction & 0x780U)
        SetFogMode(1);
    if (instruction & 0x800U) {
        const std::uint64_t colour = fog.cloudColours[0];
        g_cloudXY[0] = colour | (colour << 32U);
    }
    if (instruction & 0x1000U) {
        const std::uint64_t colour = fog.cloudColours[1];
        g_cloudXY[1] = colour | (colour << 32U);
    }
}

void Weather_LoadSPR(WeatherTaskRuntime *data) {
    if (!data)
        return;
    data->rainOrSnow = data->rain >= 0.1f || data->snow >= 0.1f;
    data->overcast = data->clud > 0.25f;
    const auto mode = data->rainOrSnow ? WeatherSpriteMode::RainOrSnow :
        data->overcast                 ? WeatherSpriteMode::Overcast :
                                         GetClearSkySpriteMode(data->hour, data->minute);
    Shenmue_ReadSprite_Wrapper(nullptr, kWeatherSpriteNames[static_cast<std::size_t>(mode)]);
    data->spriteMode = mode;
}

void Weather_BgdtSpriteLoader(WeatherTaskRuntime &data) {
    if (data.rainOrSnow || (data.rain < 0.1f && data.snow < 0.1f)) {
        if (data.rainOrSnow == 1 && data.rain < 0.05f && data.snow < 0.05f)
            data.rainOrSnow = 0;
    } else {
        data.rainOrSnow = 1;
    }
    if (data.overcast || data.clud < 0.25f) {
        if (data.overcast == 1 && data.clud < 0.2f)
            data.overcast = 0;
    } else {
        data.overcast = 1;
    }
    const auto mode = data.rainOrSnow ? WeatherSpriteMode::RainOrSnow :
        data.overcast                 ? WeatherSpriteMode::Overcast :
                                        GetClearSkySpriteMode(data.hour, data.minute);
    if (data.spriteMode == mode)
        return;
    RefreshAndFreeCurrentSkyScroll();
    std::int32_t intensity = 500;
    if (static_cast<std::uint8_t>(g_currentDateTime.hours - 5) > 12 && data.spriteMode <= WeatherSpriteMode::ClearNight &&
        (mode == WeatherSpriteMode::RainOrSnow || mode == WeatherSpriteMode::Overcast))
        intensity = 800;
    if (SCRL_LoadSprite(kWeatherSpriteNames[static_cast<std::size_t>(mode)], intensity))
        data.spriteMode = mode;
}

float Weather_GetCLUDIntensity() {
    if (std::fabs(g_cloudGameData->intensity) < std::numeric_limits<float>::epsilon()) {
        g_cloudGameData->intensity = 0.0f;
    }
    return g_cloudGameData->intensity;
}
} // namespace

std::uint32_t PackWeatherColour(WeatherColourData &data) {
    std::uint32_t colour = 0;
    for (float &channel : data.current) {
        channel = clampWeatherValue(channel);
        colour = (colour << 8U) | static_cast<std::uint32_t>(static_cast<std::int32_t>(channel));
    }
    return colour;
}

std::uint32_t PackRainFloats(WeatherTaskRuntime &data) {
    return PackWeatherColour(*data.particleColours[0]);
}

std::uint32_t PackVector4FloatsInto32BitInt(WeatherTaskRuntime &data) {
    return PackWeatherColour(*data.particleColours[1]);
}

std::uint32_t PackWeatherColourRampFloats(WeatherTaskRuntime &data) {
    return PackWeatherColour(*data.particleColours[2]);
}

void Weather_InitTaskData() {
    if (g_weatherTaskRuntime)
        return;
    g_weatherTaskRuntime = GetWeatherTaskData();
    AllocFIXC();
    auto &data = *g_weatherTaskRuntime;
    g_weatherPreviousOperand = 1;
    data.month = g_currentDateTime.month;
    data.day = g_currentDateTime.day;
    data.hour = g_currentDateTime.hours;
    data.minute = g_currentDateTime.minutes;
    data.second = g_currentDateTime.seconds;
    data.seconds = data.second + 60 * (data.minute + 60 * data.hour);
    WEXE_ReadDataForDateTime(g_currentDateTime.year, data.month, data.day, data.hour, data.minute);
    WEXE_ApplyCloudGameData(weatherRealCloud(), 0);
    WEXE_ApplyFcasGameData(clampWeatherValue(g_weatherFcasValue), 0);
    Weather_SetForcedRainValue(weatherRealRain());
    Weather_SetForcedSnowValue(weatherRealSnow());
    data.clud = g_cloudGameData->intensity = clampWeatherValue(g_cloudGameData->intensity);
    data.rain = g_activeRainEntry->intensity = clampWeatherValue(g_activeRainEntry->intensity);
    data.snow = g_activeSnowEntry->intensity = clampWeatherValue(g_activeSnowEntry->intensity);
    data.fcas = g_fcasGameData->intensity = clampWeatherValue(g_fcasGameData->intensity);
    if (g_shouldParseWeatherInstructions == 1)
        Weather_ParseInstruction(data);
    if (EFFE_GetWeatherSpriteLoadPolicy() == 1)
        Weather_LoadSPR(&data);
    if (g_weatherPreviousOperand == 1) {
        SetRainEffectOffset(PackRainFloats(data));
        Weather_UpdateRainOffset(PackVector4FloatsInto32BitInt(data));
        Weather_UpdateColourRamp(PackWeatherColourRampFloats(data));
    }
}

void Weather_AdvanceGameDataAndWriteCB() {
    if (!g_weatherTaskRuntime)
        return;
    Weather_AdvanceWeatherEntry(g_cloudGameData);
    Weather_AdvanceWeatherEntry(g_fcasGameData);
    auto &data = *g_weatherTaskRuntime;
    data.month = g_currentDateTime.month;
    data.day = g_currentDateTime.day;
    data.hour = g_currentDateTime.hours;
    data.minute = g_currentDateTime.minutes;
    data.second = g_currentDateTime.seconds;
    data.seconds = data.second + 60 * (data.minute + 60 * data.hour);
    data.clud = g_cloudGameData->intensity = clampWeatherValue(g_cloudGameData->intensity);
    data.rain = g_activeRainEntry->intensity = clampWeatherValue(g_activeRainEntry->intensity);
    data.snow = g_activeSnowEntry->intensity = clampWeatherValue(g_activeSnowEntry->intensity);
    data.fcas = g_fcasGameData->intensity = clampWeatherValue(g_fcasGameData->intensity);
    if (g_shouldParseWeatherInstructions == 1)
        Weather_ParseInstruction(data);
    if (EFFE_GetWeatherSpriteLoadPolicy() == 1)
        Weather_BgdtSpriteLoader(data);
    if (g_weatherPreviousOperand == 1) {
        SetRainEffectOffset(PackRainFloats(data));
        Weather_UpdateRainOffset(PackVector4FloatsInto32BitInt(data));
        Weather_UpdateColourRamp(PackWeatherColourRampFloats(data));
    }
}

void WEXE_InterpolateWeatherOverride(std::int32_t channel) {
    WeatherOverrideInterpolation *interpolation = nullptr;
    if (channel == 0)
        interpolation = &g_cloudOverrideInterpolation;
    else if (channel == 1)
        interpolation = &g_rainOverrideInterpolation;
    else if (channel == 2)
        interpolation = &g_snowOverrideInterpolation;
    if (!interpolation) {
        if (channel != 3)
            return;
        auto &rain = g_rainOverrideInterpolation;
        auto &snow = g_snowOverrideInterpolation;
        --snow.stepsRemain;
        if (--rain.stepsRemain > 0) {
            rain.curr += rain.step;
            snow.curr += snow.step;
        } else {
            rain.stepsRemain = 0;
            snow.stepsRemain = 0;
            rain.curr = rain.target;
            snow.curr = snow.target;
            if ((g_pendingWeatherClearOverrides & kWeatherOverrideDual) != 0) {
                g_pendingWeatherOverrides &= ~kWeatherOverrideDual;
                g_pendingWeatherClearOverrides &= ~kWeatherOverrideDual;
                g_overrideRainIntensity = 0.0f;
                g_overrideSnowIntensity = 0.0f;
                g_activeWeatherOverrides &= ~(kWeatherOverrideRain | kWeatherOverrideSnow);
                return;
            }
        }
        g_activeWeatherOverrides |= kWeatherOverrideRain | kWeatherOverrideSnow;
        g_overrideRainIntensity = std::fmax(0.0f, std::fmin(1.0f, rain.curr));
        g_overrideSnowIntensity = std::fmax(0.0f, std::fmin(1.0f, snow.curr));
        return;
    }
    if (--interpolation->stepsRemain > 0) {
        interpolation->curr += interpolation->step;
    } else {
        interpolation->stepsRemain = 0;
        interpolation->curr = interpolation->target;
        const std::int32_t bit = channel == 0 ? kWeatherOverrideCloud : channel == 1 ? kWeatherOverrideRain : kWeatherOverrideSnow;
        if ((g_pendingWeatherClearOverrides & bit) != 0) {
            g_pendingWeatherOverrides &= ~bit;
            g_activeWeatherOverrides &= ~bit;
            if (channel == 0)
                g_overrideCloudIntensity = 0.0f;
            if (channel == 1)
                g_overrideRainIntensity = 0.0f;
            if (channel == 2)
                g_overrideSnowIntensity = 0.0f;
            g_pendingWeatherClearOverrides &= ~bit;
            return;
        }
    }
    const float value = std::fmax(0.0f, std::fmin(1.0f, interpolation->curr));
    g_activeWeatherOverrides |= channel == 0 ? kWeatherOverrideCloud : channel == 1 ? kWeatherOverrideRain : kWeatherOverrideSnow;
    if (channel == 0)
        g_overrideCloudIntensity = value;
    if (channel == 1)
        g_overrideRainIntensity = value;
    if (channel == 2)
        g_overrideSnowIntensity = value;
}



void WEXE_SetInterpolatedWeatherOverride(std::int32_t weatherType, std::int32_t allowOverrides, std::int32_t stepsRemain, float targetIntensity) {
    const std::int32_t year = static_cast<std::int8_t>(g_currentDateTime.year);
    const std::int32_t month = static_cast<std::int8_t>(g_currentDateTime.month);
    const std::int32_t day = static_cast<std::int8_t>(g_currentDateTime.day);
    const std::int32_t hours = static_cast<std::int8_t>(g_currentDateTime.hours);
    const std::int32_t minutes = static_cast<std::int8_t>(g_currentDateTime.minutes);
    const auto readFuture = [&]() {
        WEXE_ReadDataForDateTime(static_cast<std::uint32_t>(year), static_cast<std::uint32_t>(month), static_cast<std::uint32_t>(day),
            static_cast<std::uint32_t>(hours), static_cast<std::uint32_t>(stepsRemain / 120 + minutes + 1));
    };
    const auto stepToward = [stepsRemain](float target, float current) { return stepsRemain ? (target - current) / static_cast<float>(stepsRemain) : 0.0f; };
    WEXE_ReadDataForDateTime(static_cast<std::uint32_t>(year), static_cast<std::uint32_t>(month), static_cast<std::uint32_t>(day),
        static_cast<std::uint32_t>(hours), static_cast<std::uint32_t>(minutes));
    float target = 0.0f > targetIntensity ? 0.0f : (1.0f < targetIntensity ? 1.0f : targetIntensity);

    if (weatherType == 0) {
        g_pendingWeatherOverrides |= kWeatherOverrideCloud;
        g_cloudOverrideInterpolation.stepsRemain = stepsRemain;
        const float current = weatherRealCloud();
        g_cloudOverrideInterpolation.curr = current;
        if (allowOverrides) {
            g_pendingWeatherClearOverrides |= kWeatherOverrideCloud;
            g_allowWeatherOverrideValues = 0;
            readFuture();
            target = weatherRealCloud();
            g_allowWeatherOverrideValues = 1;
        } else {
            g_pendingWeatherClearOverrides &= ~kWeatherOverrideCloud;
        }
        g_cloudOverrideInterpolation.target = target;
        g_cloudOverrideInterpolation.step = stepToward(target, current);
        WEXE_InterpolateWeatherOverride(0);
        WEXE_ApplyCloudGameData(g_cloudOverrideInterpolation.curr, 0);
        return;
    }

    if (weatherType == 1) {
        if ((g_pendingWeatherOverrides & kWeatherOverrideDual) != 0)
            return;
        g_pendingWeatherOverrides |= kWeatherOverrideRain;
        g_rainOverrideInterpolation.stepsRemain = stepsRemain;
        const float current = weatherRealRain();
        g_rainOverrideInterpolation.curr = current;
        if (allowOverrides) {
            g_pendingWeatherClearOverrides |= kWeatherOverrideRain;
            g_allowWeatherOverrideValues = 0;
            readFuture();
            target = weatherRealRain();
            g_allowWeatherOverrideValues = 1;
        } else {
            g_pendingWeatherClearOverrides &= ~kWeatherOverrideRain;
        }
        g_rainOverrideInterpolation.target = target;
        g_rainOverrideInterpolation.step = stepToward(target, current);
        WEXE_InterpolateWeatherOverride(1);
        const float rain = g_rainOverrideInterpolation.curr;
        WEXE_ApplyRainGameData(rain, 0);
        WEXE_ApplyWindGameData(rain, 0);
        return;
    }

    if (weatherType == 2) {
        if ((g_pendingWeatherOverrides & kWeatherOverrideDual) != 0)
            return;
        const std::int32_t active = g_activeWeatherOverrides;
        const std::int32_t pending = g_pendingWeatherOverrides | kWeatherOverrideSnow;
        g_pendingWeatherOverrides = pending;
        const float current = weatherRealSnow();
        std::int32_t pendingClear;
        if (allowOverrides) {
            pendingClear = g_pendingWeatherClearOverrides | kWeatherOverrideSnow;
            g_pendingWeatherClearOverrides = pendingClear;
            readFuture();
            g_allowWeatherOverrideValues = 1;
            target = g_weatherSnowValue;
        } else {
            pendingClear = g_pendingWeatherClearOverrides & ~kWeatherOverrideSnow;
            g_pendingWeatherClearOverrides = pendingClear;
        }
        g_snowOverrideInterpolation.target = target;
        const float step = stepToward(target, current);
        g_snowOverrideInterpolation.step = step;
        g_snowOverrideInterpolation.stepsRemain = stepsRemain - 1;
        float snow = target;
        if (stepsRemain - 1 > 0) {
            snow = step + current;
            g_snowOverrideInterpolation.curr = snow;
        } else {
            g_snowOverrideInterpolation.curr = target;
            g_snowOverrideInterpolation.stepsRemain = 0;
            if ((pendingClear & kWeatherOverrideSnow) != 0) {
                g_overrideSnowIntensity = 0.0f;
                g_pendingWeatherClearOverrides = pendingClear & ~kWeatherOverrideSnow;
                g_pendingWeatherOverrides = pending & ~kWeatherOverrideSnow;
                g_activeWeatherOverrides = active & ~kWeatherOverrideSnow;
                WEXE_ApplySnowGameData(snow, 0);
                return;
            }
        }
        g_overrideSnowIntensity = std::fmax(0.0f, std::fmin(1.0f, snow));
        g_activeWeatherOverrides = active | kWeatherOverrideSnow;
        WEXE_ApplySnowGameData(snow, 0);
        return;
    }

    if (weatherType != 3 || (g_pendingWeatherOverrides & (kWeatherOverrideRain | kWeatherOverrideSnow)) != 0)
        return;
    g_pendingWeatherOverrides |= kWeatherOverrideDual;
    g_rainOverrideInterpolation.stepsRemain = stepsRemain;
    const float currentRain = weatherRealRain();
    g_rainOverrideInterpolation.curr = currentRain;
    g_snowOverrideInterpolation.stepsRemain = stepsRemain;
    const float currentSnow = weatherRealSnow();
    g_snowOverrideInterpolation.curr = currentSnow;
    readFuture();
    g_allowWeatherOverrideValues = 0;
    float rainTarget = weatherRealRain();
    float snowTarget = g_weatherSnowValue;
    g_allowWeatherOverrideValues = 1;
    if (allowOverrides) {
        g_pendingWeatherClearOverrides |= kWeatherOverrideDual;
        g_rainOverrideInterpolation.target = rainTarget;
    } else {
        g_pendingWeatherClearOverrides &= ~kWeatherOverrideDual;
        snowTarget = std::fmax(snowTarget, rainTarget);
        if (snowTarget > 0.0f && snowTarget < 0.3f)
            snowTarget = 0.7f;
        g_rainOverrideInterpolation.target = 0.0f;
        rainTarget = 0.0f;
    }
    g_snowOverrideInterpolation.target = snowTarget;
    g_rainOverrideInterpolation.step = stepToward(rainTarget, currentRain);
    g_snowOverrideInterpolation.step = stepToward(snowTarget, currentSnow);
    WEXE_InterpolateWeatherOverride(3);
    const float rain = g_rainOverrideInterpolation.curr;
    WEXE_ApplyRainGameData(rain, 0);
    WEXE_ApplyWindGameData(rain, 0);
    WEXE_ApplySnowGameData(g_snowOverrideInterpolation.curr, 0);
}
void WEXE_WeatherTaskUtil(std::uint32_t mode, std::uint32_t operand, std::uint32_t value) {
    auto storeWord = [](std::uint32_t offset, std::uint32_t word) {
        if (g_pCurrentTaskData && offset)
            *reinterpret_cast<std::uint32_t *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) = word;
    };
    if (mode == 1) {
        Weather_InitTaskData();
    } else if (mode == 2) {
        if (g_weatherTaskRuntime) {
            Weather_Free(g_weatherTaskRuntime);
            g_weatherTaskRuntime = nullptr;
            g_weatherPreviousOperand = 0;
        }
    } else if (mode == 3) {
        WEXE_ApplyCloudGameData(std::bit_cast<float>(operand), 0);
    } else if (mode == 4) {
        WEXE_ApplyCloudGameData(std::bit_cast<float>(operand), static_cast<std::int32_t>(value));
    } else if (mode == 5) {
        storeWord(operand, std::bit_cast<std::uint32_t>(Weather_GetCLUDIntensity()));
    } else if (mode == 6) {
        WEXE_ApplyFcasGameData(std::bit_cast<float>(operand), 0);
    } else if (mode == 7) {
        WEXE_ApplyFcasGameData(std::bit_cast<float>(operand), static_cast<std::int32_t>(value));
    } else if (mode == 8) {
        g_fcasGameData->intensity = clampWeatherValue(g_fcasGameData->intensity);
        storeWord(operand, std::bit_cast<std::uint32_t>(g_fcasGameData->intensity));
    } else if (mode >= 0xa && mode <= 0xd) {
        if (g_weatherTaskRuntime) {
            auto &table = g_weatherTaskRuntime->tables[mode - 0xa];
            table.samples = operand ? reinterpret_cast<const WeatherSample *>(g_pCurrentTaskData + static_cast<std::int32_t>(operand)) : nullptr;
            table.count = static_cast<std::int32_t>(value);
        }
    } else if (mode == 0xe) {
        const auto offset =
            g_weatherTaskRuntime ? reinterpret_cast<std::uintptr_t>(g_weatherTaskRuntime) - reinterpret_cast<std::uintptr_t>(g_pCurrentTaskData) : 0U;
        assert(offset <= std::numeric_limits<std::uint32_t>::max());
        storeWord(operand, static_cast<std::uint32_t>(offset));
    } else if (mode == 0xf) {
        g_weatherPreviousOperand = operand;
    } else if (mode == 0x11) {
        storeWord(operand, g_weatherPreviousOperand);
    } else if (mode == 0x14) {
        g_weatherInstructionFlags |= operand;
    } else if (mode == 0x15) {
        g_weatherInstructionFlags &= ~operand;
    } else if (mode == 0x16) {
        storeWord(operand, g_weatherInstructionFlags);
    } else if (mode == 0x17) {
        constexpr std::uint32_t kFog = 0x780;
        if (operand)
            g_weatherInstructionFlags |= kFog;
        else
            g_weatherInstructionFlags &= ~kFog;
    } else if (mode == 0x18) {
        constexpr std::uint32_t kCloud = 0x1800;
        if (operand)
            g_weatherInstructionFlags |= kCloud;
        else
            g_weatherInstructionFlags &= ~kCloud;
    } else if (mode == 0x19) {
        constexpr std::uint32_t kLighting = 0xc;
        if (operand)
            g_weatherInstructionFlags |= kLighting;
        else
            g_weatherInstructionFlags &= ~kLighting;
    } else if (mode == 0x1a) {
        constexpr std::uint32_t kColour = 0x70;
        if (operand)
            g_weatherInstructionFlags |= kColour;
        else
            g_weatherInstructionFlags &= ~kColour;
    } else if (mode == 0x1b) {
        Weather_LoadSPR(g_weatherTaskRuntime);
    } else if (mode == 0x1c) {
        constexpr std::uint32_t kLow = 0xf;
        if (operand)
            g_weatherInstructionFlags |= kLow;
        else
            g_weatherInstructionFlags &= ~kLow;
    }
}

bool WEXE_WeatherDataUtil(std::uint32_t mode, std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute,
    std::int32_t intValue, std::uint32_t &result) {
    auto storeWord = [](std::uint32_t offset, std::uint32_t word) {
        if (g_pCurrentTaskData && offset)
            *reinterpret_cast<std::uint32_t *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) = word;
    };
    if (mode == 1) {
        if (year)
            g_shouldReadWeatherForDateTime = 1;
        else
            setWeatherValues();
    } else if (mode == 2) {
        if (!year)
            processWeatherDelta(static_cast<std::int32_t>(month));
    } else if (mode == 3) {
        storeWord(year, static_cast<std::uint32_t>(g_shouldReadWeatherForDateTime));
    } else if (mode >= 4 && mode <= 9) {
        WEXE_ReadDataForDateTime(year, month, day, hour, minute);
        const std::uint32_t value = mode == 4 ? std::bit_cast<std::uint32_t>(weatherRealRain()) :
            mode == 5                         ? std::bit_cast<std::uint32_t>(weatherRealSnow()) :
            mode == 6                         ? std::bit_cast<std::uint32_t>(weatherRealCloud()) :
            mode == 7                         ? std::bit_cast<std::uint32_t>(weatherRealFcas()) :
            mode == 8                         ? static_cast<std::uint32_t>(weatherWindAngleForDate()) :
                                                std::bit_cast<std::uint32_t>(weatherWindStrengthForDate());
        storeWord(static_cast<std::uint32_t>(intValue), value);
    } else if (mode == 0xa) {
        float snow;
        float rain;
        float cloud;
        if (static_cast<std::int8_t>(g_currentDateTime.year) == year && static_cast<std::int8_t>(g_currentDateTime.month) == month &&
            static_cast<std::int8_t>(g_currentDateTime.day) == day && static_cast<std::int8_t>(g_currentDateTime.hours) == hour &&
            static_cast<std::int8_t>(g_currentDateTime.minutes) == minute) {
            snow = WEXE_GetSnowIntensity();
            rain = WEXE_GetRainIntensity();
            cloud = Weather_GetCLUDIntensity();
        } else {
            WEXE_ReadDataForDateTime(year, month, day, hour, minute);
            snow = weatherRealSnow();
            rain = weatherRealRain();
            cloud = weatherRealCloud();
        }
        result = snow > 0.0f ? 3U : rain > 0.0f ? 2U : cloud > 0.0f ? 1U : 0U;
        return true;
    } else if (mode == 0xb) {
        // Unlike GetFCAS (second byte of the current date's record), the script lookup packs the full argument words and returns the low byte.
        result = 0;
        const std::int32_t yearOffset = static_cast<std::int32_t>(year) - 86;
        if (yearOffset >= 0) {
            const std::uint32_t packedDate = (day | (32U * (month | (32U * static_cast<std::uint32_t>(yearOffset))))) << 16U;
            const auto *const records = static_cast<const std::uint32_t *>(g_forecastNode);
            for (std::int32_t index = 0; index < g_forecastCount; ++index) {
                if ((records[index] & 0xbfff0000U) == packedDate) {
                    result = records[index] & 0xffU;
                    break;
                }
            }
        }
        return true;
    } else if (mode == 0xc) {
        const std::uint32_t forecast = GetFCAS();
        result = year == 1 ? (forecast == 0 ? 1U : 0U) :
            year == 2      ? forecast & 1U :
            year == 3      ? (forecast >> 1U) & 1U :
            year == 4      ? (forecast >> 2U) & 1U :
            year == 5      ? ((forecast & 8U) != 0 ? 1U : 0U) :
                             forecast;
        return true;
    } else if (mode == 0xd) {
        WEXE_SetInterpolatedWeatherOverride(
            static_cast<std::int32_t>(year), static_cast<std::int32_t>(month), static_cast<std::int32_t>(day), std::bit_cast<float>(hour));
    }
    return false;
}
void Weather_ApplyWeatherEntry(WeatherDataEntry *entry, float intensity, std::int32_t stepCount) {
    if (stepCount < 1) {
        entry->targetIntensity = intensity;
        entry->incrementStep = 0.0f;
        entry->currentStep = 0;
        entry->maxStepCount = 0;
        entry->intensity = intensity;
        return;
    }

    if (std::fabs(intensity) < std::numeric_limits<float>::epsilon()) {
        intensity = 0.0f;
    }
    entry->targetIntensity = intensity;
    const float difference = intensity - entry->intensity;
    const float increment = difference / static_cast<float>(stepCount);
    entry->incrementStep =
        std::fabs(difference) >= std::numeric_limits<float>::epsilon() && std::fabs(increment) >= std::numeric_limits<float>::epsilon() ? increment : 0.0f;
    entry->currentStep = 0;
    entry->maxStepCount = stepCount;
}

void WEXE_ApplyCloudGameData(float intensity, std::int32_t stepCount) {
    Weather_ApplyWeatherEntry(g_cloudGameData, intensity, stepCount);
}

void WEXE_ApplyRainGameData(float intensity, std::int32_t stepCount) {
    EFFE_ApplyRainGameData(intensity, stepCount);
    Weather_ApplyWeatherEntry(g_activeRainEntry, intensity, stepCount);
}

void WEXE_ApplySnowGameData(float intensity, std::int32_t stepCount) {
    EFFE_ApplySnowGameData(intensity, stepCount);
    Weather_ApplyWeatherEntry(g_activeSnowEntry, intensity, stepCount);
}

void WEXE_ApplyWindGameData(float intensity, std::int32_t stepCount) {
    EFFE_ApplyWindGameData(intensity, stepCount);
    Weather_ApplyWeatherEntry(g_activeWindEntry, intensity, stepCount);
}

void WEXE_ApplyFcasGameData(float intensity, std::int32_t stepCount) {
    Weather_ApplyWeatherEntry(g_fcasGameData, intensity, stepCount);
}

float WEXE_GetCloudIntensity() {
    return g_cloudGameData ?
        ((g_activeWeatherOverrides & kWeatherOverrideCloud) != 0 && g_allowWeatherOverrideValues ? g_overrideCloudIntensity : g_cloudGameData->intensity) :
        0.0f;
}

float WEXE_GetRainIntensity() {
    if (std::fabs(g_activeRainEntry->intensity) < std::numeric_limits<float>::epsilon()) {
        g_activeRainEntry->intensity = 0.0f;
    }
    return g_activeRainEntry->intensity;
}

float WEXE_GetSnowIntensity() {
    if (std::fabs(g_activeSnowEntry->intensity) < std::numeric_limits<float>::epsilon()) {
        g_activeSnowEntry->intensity = 0.0f;
    }
    return g_activeSnowEntry->intensity;
}

float WEXE_GetWindIntensity() {
    if (std::fabs(g_activeWindEntry->intensity) < std::numeric_limits<float>::epsilon())
        g_activeWindEntry->intensity = 0.0f;
    return g_activeWindEntry->intensity;
}

std::int32_t Weather_GetCurrentWeatherEffectMode() {
    WEXE_ReadDataForDateTime(g_currentDateTime.year, g_currentDateTime.month, g_currentDateTime.day, g_currentDateTime.hours, g_currentDateTime.minutes);
    const float snow = (g_activeWeatherOverrides & kWeatherOverrideSnow) != 0 && g_allowWeatherOverrideValues ? g_overrideSnowIntensity : g_weatherSnowValue;
    if (snow > 0.0f)
        return 3;

    float rain = (g_activeWeatherOverrides & kWeatherOverrideRain) != 0 && g_allowWeatherOverrideValues ? g_overrideRainIntensity : g_weatherRainValue;
    if ((g_activeWeatherOverrides & kWeatherOverrideRain) == 0 || !g_allowWeatherOverrideValues) {
        rain = clampWeatherValue(rain);
    }
    if (rain > 0.0f)
        return 2;

    if ((g_activeWeatherOverrides & kWeatherOverrideCloud) != 0 && g_allowWeatherOverrideValues) {
        return g_overrideCloudIntensity > 0.0f;
    }
    return clampWeatherValue(g_weatherCloudValue) > 0.0f;
}

HLib::Task *WEXE_Init() {
    WIND_ResetWeatherDataForScene();
    g_cachedWeatherYear = 0;
    g_cachedWeatherMonth = 0;
    g_cachedWeatherDay = 0;
    g_cachedWeatherHour = 0;
    g_cachedWeatherMinute = 0;
    g_disableWeather = 0;
    g_shouldReadWeatherForDateTime = 1;
    g_weatherTaskRuntime = nullptr;
    g_weatherPreviousOperand = 1;
    g_weatherForecastValue = 0;
    g_activeWeatherOverrides = 0;
    g_pendingWeatherOverrides = 0;
    g_pendingWeatherClearOverrides = 0;
    g_allowWeatherOverrideValues = 1;
    g_overrideCloudIntensity = 0.0f;
    g_overrideRainIntensity = 0.0f;
    g_overrideSnowIntensity = 0.0f;
    g_cloudOverrideInterpolation = {};
    g_rainOverrideInterpolation = {};
    g_snowOverrideInterpolation = {};
    Weather::g_tempOverrideLighting = {};
    g_weatherInstructionFlags = 0x1fff;
    g_shouldParseWeatherInstructions = 1;
    g_defaultWeatherTables = {{
        {kClearWeatherSamples.data(), 12, 0},
        {kOvercastWeatherSamples.data(), 10, 0},
        {kRainWeatherSamples.data(), 9, 0},
        {kSnowWeatherSamples.data(), 9, 0},
    }};
    g_weatherWccpClipStorage[0] = {};
    g_weatherWccpClipStorage[1] = {};
    g_weatherWccpClipSet0 = 0;
    g_weatherWccpClipSet1 = 0;
    g_cloudGameData = allocateWeatherEntry();
    g_fcasGameData = allocateWeatherEntry();
    g_activeRainEntry = allocateWeatherEntry();
    g_activeSnowEntry = allocateWeatherEntry();
    g_activeWindEntry = allocateWeatherEntry();
    WIND_EnsureWeatherData();
    g_weatherExecuteTask = HLib_EnqueueTaskWithoutParameter(Weather_Tick, 13, 8, makeToken('W', 'E', 'X', 'E'));
#if SHENMUE_IS_PC
    g_fcasRuntime = {.current = kWeatherFcasInterpolation};
#endif
    return g_weatherExecuteTask;
}



void WEXE_Destroy() {
    if (g_weatherExecuteTask) {
        HLib_DestroyTask(g_weatherExecuteTask);
        g_weatherExecuteTask = nullptr;
    }
    if (g_weatherTaskRuntime) {
        Weather_Free(g_weatherTaskRuntime);
        g_weatherTaskRuntime = nullptr;
    }
    freeWeatherEntry(g_cloudGameData);
    freeWeatherEntry(g_fcasGameData);
    freeWeatherEntry(g_activeRainEntry);
    freeWeatherEntry(g_activeSnowEntry);
    freeWeatherEntry(g_activeWindEntry);
    WIND_DestroyWeatherData();
    g_cachedWeatherYear = 0;
    g_cachedWeatherMonth = 0;
    g_cachedWeatherDay = 0;
    WEXE_ReleaseWccpClipSet(0);
    WEXE_ReleaseWccpClipSet(1);
    g_weatherWccpClipSet0 = 0;
    g_weatherWccpClipSet1 = 0;
    g_cachedWeatherHour = 0;
    g_cachedWeatherMinute = 0;
}

void Weather_Tick(HLib::TaskData *) {
    if (!g_disableWeather)
        return;
    g_rainSpawnScale = 0.05f;
    g_rainVisualBlend = 1.0f;

    const std::uint8_t currDay = g_currentDateTime.day;
    const std::uint8_t currMonth = g_currentDateTime.month;
    const std::uint8_t currHour = g_currentDateTime.hours;
    const std::uint8_t currMins = g_currentDateTime.minutes;
    const std::uint8_t currYear = g_currentDateTime.year;
    if (!g_shouldReadWeatherForDateTime) {
        if (g_cachedWeatherYear != currYear || g_cachedWeatherMonth != currMonth || g_cachedWeatherDay != currDay) {
            if (WeatherWindData *const wind = WIND_GetWeatherData()) {
                WEXE_ReadDataForDateTime(currYear, currMonth, currDay, currHour, currMins);
                applyScheduledWind(*wind);
            }
            g_cachedWeatherYear = currYear;
            g_cachedWeatherMonth = currMonth;
            g_cachedWeatherDay = currDay;
        }

        std::int32_t overrides = g_pendingWeatherOverrides;
        if ((overrides & kWeatherOverrideCloud) != 0) {
            WEXE_InterpolateWeatherOverride(0);
            WEXE_ApplyCloudGameData(g_cloudOverrideInterpolation.curr, 0);
            overrides = g_pendingWeatherOverrides;
        }
        if ((overrides & kWeatherOverrideDual) != 0) {
            WEXE_InterpolateWeatherOverride(3);
            WEXE_ApplyRainGameData(g_rainOverrideInterpolation.curr, 0);
            WEXE_ApplyWindGameData(g_rainOverrideInterpolation.curr, 0);
            WEXE_ApplySnowGameData(g_snowOverrideInterpolation.curr, 0);
            overrides = g_pendingWeatherOverrides;
        }
        if ((overrides & kWeatherOverrideRain) != 0) {
            WEXE_InterpolateWeatherOverride(1);
            WEXE_ApplyRainGameData(g_rainOverrideInterpolation.curr, 0);
            WEXE_ApplyWindGameData(g_rainOverrideInterpolation.curr, 0);
            overrides = g_pendingWeatherOverrides;
        }
        if ((overrides & kWeatherOverrideSnow) != 0) {
            WEXE_InterpolateWeatherOverride(2);
            WEXE_ApplySnowGameData(g_snowOverrideInterpolation.curr, 0);
            overrides = g_pendingWeatherOverrides;
        }

        if (g_cachedWeatherHour != currHour || g_cachedWeatherMinute != currMins) {
            const GameDateTime target = nextMinute(g_currentDateTime);
            WEXE_ReadDataForDateTime(target.year, target.month, target.day, target.hours, target.minutes);
            if ((overrides & kWeatherOverrideCloud) == 0)
                WEXE_ApplyCloudGameData(weatherRealCloud(), 120);
            if ((overrides & kWeatherOverrideDual) == 0 && (overrides & kWeatherOverrideRain) == 0) {
                WEXE_ApplyRainGameData(weatherRealRain(), 120);
                WEXE_ApplyWindGameData(weatherRealRain(), 120);
            }
            if ((overrides & kWeatherOverrideDual) == 0 && (overrides & kWeatherOverrideSnow) == 0)
                WEXE_ApplySnowGameData(weatherRealSnow(), 120);

            WEXE_ApplyFcasGameData(g_weatherFcasValue, 120);
            g_cachedWeatherHour = target.hours;
            g_cachedWeatherMinute = target.minutes;
        }
    }

    WIND_UpdateWeatherData();
    Weather_AdvanceGameDataAndWriteCB();
    g_weatherClipCameraPosition = g_CameraEyePos;
    Weather_UpdateWCCPClipSet(0, g_weatherClipCameraPosition);
    Weather_UpdateWCCPClipSet(1, g_weatherClipCameraPosition);
    WEXE_UpdateWeatherCameraPosition();
    EFFE_UpdateWeatherState();
    EFFE_UpdateWeatherDrawBits(g_bShouldDraw);
}

void Weather_ReadChunks(void *fileData) {
    auto findChunkUnchecked = [](WeatherChunk *chunk, std::uint32_t token) {
        while (chunk->token != token) {
            if (chunk->size == 0)
                return static_cast<WeatherChunk *>(nullptr);
            chunk = reinterpret_cast<WeatherChunk *>(reinterpret_cast<std::byte *>(chunk) + chunk->size);
        }
        return chunk;
    };
    g_wdatRoot = fileData;
    auto *const first = static_cast<WeatherChunk *>(fileData);
    static_cast<void>(findChunkUnchecked(first, makeToken('W', 'D', 'A', 'T')));
    auto bindChunk = [&](std::uint32_t token, void *&node, std::int32_t &count, WeatherRuntimeCursor &runtime) {
        WeatherChunk *const chunk = findChunkUnchecked(first, token);
        count = chunk->size / static_cast<std::int32_t>(sizeof(std::uint32_t)) - 2;
        node = chunk + 1;
        resetWeatherCursor(runtime, node);
    };
    bindChunk(makeToken('C', 'L', 'U', 'D'), g_cloudNode, g_cloudCount, g_cloudRuntime);
    bindChunk(makeToken('R', 'A', 'I', 'N'), g_rainNode, g_rainCount, g_rainRuntime);
    bindChunk(makeToken('S', 'N', 'O', 'W'), g_snowNode, g_snowCount, g_snowRuntime);
#if SHENMUE_IS_PC
    WeatherChunk *const forecast = findChunkUnchecked(first, makeToken('F', 'C', 'A', 'S'));
    g_forecastCount = forecast->size / static_cast<std::int32_t>(sizeof(std::uint32_t)) - 2;
    g_forecastNode = forecast + 1;
#else
    bindChunk(makeToken('F', 'C', 'A', 'S'), g_forecastNode, g_forecastCount, g_fcasRuntime);
#endif
}



void AllocateWEDT() {
    const auto *const mobj = static_cast<const std::byte *>(g_mobjData);
    const std::byte *const tableBase = mobj + 0x10;
    const std::int32_t selector = readInt32(mobj + 0x1c);
    const std::int32_t offset = readInt32(tableBase + selector + 0x48);
    const std::int32_t size = readInt32(tableBase + selector + 0x4c);
    auto *const data = static_cast<HLib::TaskData *>(HLib_AllocMemBlock(static_cast<std::uint64_t>(size), SHEN_NAME(WEDT)));
    std::memcpy(data, tableBase + offset, static_cast<std::size_t>(size));
    g_wdatFileSize = !data || (reinterpret_cast<std::uintptr_t>(data) & 0x1fU) != 0 ? 0 : static_cast<std::int32_t>(data[-1].content_size) - 64;
    Weather_ReadChunks(data);
}

void Weather_SetWeatherDataByMode(std::int32_t mode) {
    if (mode == g_weatherMode)
        return;
    const char *const filename = mode != 0 ? "WEAT_ORG.BIN" : "WEATHER.BIN";
    if (mode != 0)
        static_cast<void>(shenOpenFile("sprite", filename));
    static_cast<void>(shenLoadFile("sprite", filename, g_wdatRoot, g_wdatFileSize));
    Weather_ReadChunks(g_wdatRoot);
    g_weatherMode = mode;
}

std::uint8_t GetFCAS() {
    return readForecast(g_currentDateTime);
}

void Weather_ProcessFogMode(const std::byte *entry) {
    const auto *const fog = reinterpret_cast<const FogTableFogChunk *>(findFogEntryChunk(entry, kFogChunk));
    const auto *const background = reinterpret_cast<const FogTableBackgroundChunk *>(findFogEntryChunk(entry, kBackgroundChunk));

    g_fogR = fog->red;
    g_fogB = fog->blue;
    g_fogG = fog->green;
    g_fogColour = fog->colour;
    SetFogMode(fog->mode);

    std::memcpy(g_cloudXY, background->cloudColours, sizeof(g_cloudXY));
    g_shouldOverrideClearColour = 1;
    g_overrideClearColour = background->overrideClearColour;
    g_shouldOverrideClearColour = background->shouldOverrideClearColour;
    g_skyTextures[0].fogTableReference = background->scrollTextureReference;
}

void Script_ProcessFogFromTable(ScenarioContext *, void *rawParams) {
    const std::uint32_t index = *static_cast<const std::uint32_t *>(rawParams);
    const std::byte *mapInfo = nullptr;
    if (g_mapControlTask && g_mapControlTask->task_data_ptr) {
        mapInfo = static_cast<const std::byte *>(reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr)->mapInfo);
    }
    const std::byte *const table = findFogEntryChunk(mapInfo, kFogChunk);
    const std::uintptr_t entryAddress =
        reinterpret_cast<std::uintptr_t>(table) + sizeof(WeatherChunk) + static_cast<std::uintptr_t>(index) * kFogTableEntrySize;
    Weather_ProcessFogMode(reinterpret_cast<const std::byte *>(entryAddress));
}

namespace {

float (&g_fogIntensityTable)[128] = g_followerLoadFogBlock.fogIntensityTable;
std::int32_t g_fogDensityInitialized = 0;

float *Fog_ScaleFogIntensityTable(float intensity) {
    for (float &value : g_fogIntensityTable)
        value = std::max(std::min(value * intensity, 1.0f), 0.0f);
    return g_fogIntensityTable + std::size(g_fogIntensityTable);
}

std::int64_t Fog_ComputeFogAttenuation(float nearDistance, float farDistance) {
    std::int16_t packedDensity = 0;
    if (nearDistance > 65535.0f) {
        packedDensity = static_cast<std::int16_t>(0x8010U);
    } else {
        int exponent = 0;
        float doubledMantissa = 2.0f * static_cast<float>(std::frexp(static_cast<double>(-farDistance), &exponent));
        --exponent;
        std::uint16_t mantissaBits = 0;
        if (doubledMantissa - 1.0f >= 0.0f) {
            doubledMantissa -= 1.0f;
            mantissaBits |= 0x8000U;
        }
        if (doubledMantissa - 0.5f >= 0.0f) {
            doubledMantissa -= 0.5f;
            mantissaBits |= 0x4000U;
        }
        if (doubledMantissa - 0.25f >= 0.0f) {
            doubledMantissa -= 0.25f;
            mantissaBits |= 0x2000U;
        }
        if (doubledMantissa - 0.125f >= 0.0f) {
            doubledMantissa -= 0.125f;
            mantissaBits |= 0x1000U;
        }
        if (doubledMantissa - 0.0625f >= 0.0f) {
            doubledMantissa -= 0.0625f;
            mantissaBits |= 0x0800U;
        }
        if (doubledMantissa - 0.03125f >= 0.0f) {
            doubledMantissa -= 0.03125f;
            mantissaBits |= 0x0400U;
        }
        if (doubledMantissa - 0.015625f >= 0.0f) {
            doubledMantissa -= 0.015625f;
            mantissaBits |= 0x0200U;
        }
        if (doubledMantissa >= 0.0078125f)
            mantissaBits |= 0x0100U;
        packedDensity = static_cast<std::int16_t>(mantissaBits | static_cast<std::uint16_t>(exponent));
    }
    if (g_fogDensityInitialized == 0)
        g_fogDensityInitialized = 1;
    const float maximumExponent = std::fmax(static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint8_t>(packedDensity)) - 7), 0.0f);
    g_fogMaximumDistance = std::pow(2.0f, maximumExponent) * static_cast<float>(static_cast<std::uint8_t>(static_cast<std::uint16_t>(packedDensity) >> 8U));

    std::int64_t result = 124;
    if (farDistance * -0.00390625f < -nearDistance) {
        int rangeExponent = 0;
        const float mantissa = static_cast<float>(std::frexp(static_cast<double>(farDistance / nearDistance), &rangeExponent));
        const int count = (16 * rangeExponent - 16) | (-16 - static_cast<int>((mantissa + mantissa) * -16.0f));
        float *const table = g_fogIntensityTable;
        for (int index = 0; index < count; ++index) {
            table[index] = static_cast<float>(count - index) / static_cast<float>(count);
            result = index;
        }
        if (count < 128) {


            const std::ptrdiff_t start = static_cast<std::ptrdiff_t>(offsetof(FollowerLoadFogBlock, fogIntensityTable)) +
                static_cast<std::ptrdiff_t>(count) * static_cast<std::ptrdiff_t>(sizeof(float));
            const std::ptrdiff_t first = start > 0 ? start : 0;
            std::memset(reinterpret_cast<std::byte *>(&g_followerLoadFogBlock) + first, 0,
                static_cast<std::size_t>(static_cast<std::ptrdiff_t>(sizeof(FollowerLoadFogBlock)) - first));
            result = 0;
        }
    } else {
        for (std::size_t index = 0; index < std::size(g_fogIntensityTable); ++index) {
            g_fogIntensityTable[index] = (128.0f - static_cast<float>(index)) * 0.0078125f;
        }
    }
    return result;
}

void Fog_ApplySpecialSettingsImpl() {
    if (g_areaId == SHEN_NAME(MFSY) && g_fogR == 40.0f && g_fogG == 60.0f) {
        g_fogRenderNearDistance = 20.25f;
        g_fogRenderFarDistance = 200.0f;
        g_fogMaximumDistance = 800.0f;
    }
    g_renderParams.constants[15] = {g_fogRenderNearDistance, g_fogRenderFarDistance, g_fogMaximumDistance, 0.0f};
    g_forcedFogNearDistance = g_fogRenderNearDistance;
    g_forcedFogFarDistance = g_fogRenderFarDistance;
    if (g_forceFogOverride) {
        g_forcedFogNearDistance = g_renderParams.constants[7].w;
        g_forcedFogFarDistance = g_renderParams.constants[7].x;
    }
}

void Fog_SetRenderFlags(std::uint32_t colour) {
    SetRenderFlags(34, static_cast<std::int32_t>(colour));
    g_lastSetFogColour = colour;
    g_normalizedFogColour = {
        static_cast<float>((colour >> 16U) & 0xffU) / 255.0f, static_cast<float>((colour >> 8U) & 0xffU) / 255.0f, static_cast<float>(colour & 0xffU) / 255.0f};
}

void Fog_UpdateFogEffects() {
    if (g_bFogEnabled != 1)
        return;
    const float intensity = g_fogB;
    Fog_ScaleFogIntensityTable(intensity);
    const float nearDistance = g_fogR;
    const float farDistance = g_fogG;
    SetRenderFlags(36, std::bit_cast<std::int32_t>(nearDistance));
    SetRenderFlags(37, std::bit_cast<std::int32_t>(farDistance));
    g_fogNearRenderFlagBits = std::bit_cast<std::int32_t>(nearDistance);
    g_fogRenderNearDistance = nearDistance;
    g_fogFarRenderFlagBits = std::bit_cast<std::int32_t>(farDistance);
    g_fogRenderFarDistance = farDistance;

    Fog_ApplySpecialSettingsImpl();
    Fog_SetRenderFlags(g_fogColour);
    SetRenderFlags(210, std::bit_cast<std::int32_t>(intensity));
    g_fogIntensityRenderFlagBits = std::bit_cast<std::int32_t>(intensity);
    Fog_ComputeFogAttenuation(-nearDistance, -farDistance);
    Fog_ScaleFogIntensityTable(g_fogB);
}

} // namespace

void Fog_ApplySpecialSettings() {
    Fog_ApplySpecialSettingsImpl();
}

void Fog_DoFogEffectsTask(std::int64_t) {
    Fog_UpdateFogEffects();
}

void TaskUtil_InitStaticSize(std::int64_t) {
    std::memset(g_fogIntensityTable, 0, sizeof(g_fogIntensityTable));
}

void FOG_InitFog() {
    g_fogR = 20.0f;
    g_fogB = 1.0f;
    g_fogG = 800.0f;
    g_fogColour = 0xffffffff;
    g_bFogEnabled = 1;
    g_cloudXY[0] = 0x00ffffff00ffffffULL;
    g_cloudXY[1] = 0x00ffffff00ffffffULL;

    std::int32_t taskIndex = g_TaskInfo.CurrentTaskIdx;
    if (taskIndex >= 32) {
        return;
    }
    HLib::Task *const task = CurrentTask;
    g_TaskInfo.pWeatherTaskModule[taskIndex] = task;
    g_TaskInfo.pWeatherTasks[taskIndex] = nullptr;
    task->taskFn = TaskUtil_InitStaticSize;
    ++taskIndex;
    g_TaskInfo.CurrentTaskIdx = taskIndex;
    if (taskIndex < 32) {
        task->taskFn = Fog_DoFogEffectsTask;
        g_TaskInfo.pWeatherTaskModule[taskIndex] = task;
        g_TaskInfo.pWeatherTasks[taskIndex] = nullptr;
        g_TaskInfo.CurrentTaskIdx = taskIndex + 1;
    }
}

} // namespace shenmue::s1
