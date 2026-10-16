#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>
#include <array>

namespace shenmue::s2 {
struct MapInfoChunk;
struct WeatherFixcNode;
extern WeatherFixcNode *g_secondaryWeatherWindList;

struct WeatherFloat {
    float current;
    float target;
    float step;
    std::int32_t frame;
    std::int32_t frameCount;
};

struct WeatherColour {
    std::array<float, 4> current;
    std::array<float, 4> target;
    std::array<float, 4> step;
    std::int32_t frame;
    std::int32_t frameCount;
};

static_assert(sizeof(WeatherFloat) == 0x14);
static_assert(sizeof(WeatherColour) == 0x38);

std::int32_t lfltGetFramesRemaining(const WeatherFloat *value);
bool lfltIsInterpolating(const WeatherFloat *value);
std::int32_t lfltSetImmediate(WeatherFloat *value, float target);
std::int32_t lrgbSetImmediate(WeatherColour *value, const Vector4 *target);
std::int32_t shenClearData(void *start, const void *end);

void BeginWeatherFloatTransition(WeatherFloat *value, float target, std::int32_t frames);
std::int32_t AdvanceWeatherFloat(WeatherFloat *value);
WeatherFloat *GetWeatherPressureLflt();
WeatherFloat *GetWeatherRippleLflt();


struct WeatherWind {
    std::int32_t outputPhase;
    std::int32_t targetPhase;
    std::int32_t requestedPhase;
    std::int32_t phaseStep;
    std::int32_t phaseFramesRemaining;
    float sampledWindSpeed;
    float currentWindSpeed;
    float requestedWindSpeed;
    float windSpeedStep;
    std::int32_t windSpeedFramesRemaining;
    Vector3 windDirection;
    float windScaleDenominator;
    std::int32_t holdPreviousOutput;
    std::int32_t cooldownFramesRemaining;
};
static_assert(sizeof(WeatherWind) == 0x40);
static_assert(offsetof(WeatherWind, sampledWindSpeed) == 0x14);
static_assert(offsetof(WeatherWind, windDirection) == 0x28);
static_assert(offsetof(WeatherWind, cooldownFramesRemaining) == 0x3c);



inline constexpr shen_name TASK_WEXE = SHEN_NAME(WEXE);
inline constexpr shen_name TASK_WDAT = SHEN_NAME(WDAT);
inline constexpr shen_name TASK_WAIR = SHEN_NAME(WAIR);
inline constexpr std::array<shen_name, 4> WEATHER_TIMELINE_TASKS{SHEN_NAME(WIT1), SHEN_NAME(WIT2), SHEN_NAME(WIT3), SHEN_NAME(WIT4)};
inline constexpr shen_name WEATHER_SCENE_DATA{SHEN_NAME(WSCN)};
std::uint64_t WSCN_LoadWeatherData(const char *directory, const char *filename);
std::int64_t WSCN_DeleteWeatherData();
void QueueWeatherExecution(const MapInfoChunk *weatherChunk);
void FreeWAIR();
void UpdateWeather();
std::int64_t RefreshWeatherForCurrentTime(std::int32_t frames);
void UpdateWeatherForMinute(std::int32_t mode, std::int32_t enable, std::int32_t frames, float value);
std::int64_t ResetWeatherOverrides();
void SetWeatherInstruction(std::uint32_t instruction, bool enabled);
std::uint32_t GetCurrentWeatherInstruction();

void SetWeatherUpdateEnabledTo(std::int32_t enabled);
std::uint64_t GetWeatherUpdateEnabled();
std::int32_t GetCurrentWeather();

void SetWeatherShouldUpdateDay(std::int32_t flag);
void SetWeatherMinimumSunElevation(float elevation);
// SetWeatherLightManager is an independent lens-flare-manager gate, not ambient lighting.
std::int32_t SetWeatherLightManager(std::int32_t enabled);

float GetWeatherPressure();
extern std::int32_t g_planetPreviousRotationDay, g_planetCurrentRotationDay, g_planetRotationCycleDays;
void ApplyPlanetRotForCurrentDateTime();
float UpdateWeatherAndTime(std::int32_t minutes);
float GetWeatherWindVector();

std::array<std::uint32_t, 2> GetWeatherWaveColours();



inline constexpr std::size_t kInterpolatedWeatherVectorCount = 6;
struct WeatherTimeline {
    const std::uint32_t *nodes;
    const std::uint32_t *current;
    std::int32_t nodeCount;
    float blend;
    float intensity;
    std::uint32_t padding1c;
};



struct WeatherMonth {
    const std::array<std::uint8_t, 32> *days;
    std::int64_t dayCount;
};

struct WeatherYear {
    const WeatherMonth *months;
    std::int64_t monthCount;
};

struct WeatherData {
    std::int32_t baseYear;
    std::int32_t year;
    std::int32_t month;
    std::int32_t day;
    std::array<WeatherTimeline *, 4> timelines;
    const WeatherYear *years;
    std::int64_t timelineKindCount;
    std::int32_t flags;
    std::uint32_t padding44;
};
WeatherData *SetupWeatherData(std::int32_t baseYear);

struct WeatherTable {
    const std::byte *rows;
    std::int32_t count;
    std::uint32_t padding0c;
};

struct WeatherAir {
    std::uint32_t lightIndex;
    std::uint32_t padding04;
    const WeatherTable *tables;
    std::int32_t tableCount;
    std::uint32_t instruction;
    std::int32_t month;
    std::int32_t day;
    std::int32_t hour;
    std::int32_t minute;
    std::int32_t second;
    std::int32_t seconds;
    float cloud;
    float rain;
    float snow;
    float auxiliary;
    std::int32_t field40;
    std::int32_t field44;
    std::int32_t mode;
    std::uint32_t padding4c;
    WeatherColour *colour50;
    WeatherColour *colour58;
    WeatherColour *colour60;
    WeatherFloat *float68;
    WeatherFloat *float70;
    WeatherColour *colour78;
    WeatherColour *colour80;
    WeatherColour *colour88;
};

#pragma pack(push, 4)
struct WeatherTableSample {
    std::int32_t seconds;
    std::array<float, 9> primaryValues;
    std::array<std::uint32_t, 3> primaryColours;
    std::array<float, 3> secondaryValues;
    std::array<std::uint32_t, 5> environmentColours;
    std::array<std::byte, 12> reserved;
};
#pragma pack(pop)

struct WeatherOverride {
    std::int32_t frames;
    float current;
    float step;
    float target;
};

static_assert(sizeof(WeatherTimeline) == 0x20);
static_assert(offsetof(WeatherTimeline, intensity) == 0x18);
static_assert(sizeof(WeatherData) == 0x48);
static_assert(offsetof(WeatherData, timelines) == 0x10);
static_assert(offsetof(WeatherData, years) == 0x30);
static_assert(sizeof(WeatherMonth) == 0x10);
static_assert(sizeof(WeatherYear) == 0x10);
static_assert(offsetof(WeatherData, flags) == 0x40);
static_assert(sizeof(WeatherFloat) == 0x14);
static_assert(sizeof(WeatherColour) == 0x38);
static_assert(sizeof(WeatherTable) == 0x10);
static_assert(sizeof(WeatherAir) == 0x90);
static_assert(offsetof(WeatherAir, seconds) == 0x2c);
static_assert(offsetof(WeatherAir, snow) == 0x38);
static_assert(offsetof(WeatherAir, auxiliary) == 0x3c);
static_assert(offsetof(WeatherAir, colour50) == 0x50);
static_assert(offsetof(WeatherAir, colour88) == 0x88);
static_assert(sizeof(WeatherTableSample) == 0x60);
static_assert(offsetof(WeatherTableSample, primaryValues) == 0x04);
static_assert(offsetof(WeatherTableSample, primaryColours) == 0x28);
static_assert(offsetof(WeatherTableSample, secondaryValues) == 0x34);
static_assert(offsetof(WeatherTableSample, environmentColours) == 0x40);
static_assert(offsetof(WeatherTableSample, reserved) == 0x54);
static_assert(sizeof(WeatherTableSample) == sizeof(Vector4) * kInterpolatedWeatherVectorCount);
Vector4 *GetInterpolatedWeatherValues();


WeatherWind *GetWind();
extern float debugSampledWindSpeedOverride;
float GetEffectiveWindSpeed(const WeatherWind *wind);
std::int32_t GetCurrentWindPhaseAccumulator(const WeatherWind *wind);
float GetCurrentWindSpeed();
void windGetDirectionVector(const WeatherWind *wind, Vector3 *out);
void SetWindFreezeFlag(WeatherWind *wind, std::int32_t enabled);
void SetNewWindPhaseAndSpeed(std::int32_t phase, float speed);
void SyncWindWithWDATWeatherState();

} // namespace shenmue::s2
