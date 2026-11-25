#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/weather_dc.hpp"
#include "s2/weather_table.hpp"

namespace shenmue { namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float GetCurrentWindSpeed();
#endif
void FreeWAIR();
std::int32_t GetCurrentWeather();
WeatherTableSample *GetInterpolatedWeatherValues();
void SetNewWindPhaseAndSpeed(std::int32_t phase, float speed);
void SyncWindWithWDATWeatherState();
void WSCN_LoadWeatherData(const char *directory, const char *filename);
void WSCN_DeleteWeatherData();
void SetWeatherLightManager(std::int32_t enabled);
void RefreshWeatherForCurrentTime(std::int32_t frames);
void UpdateWeatherForMinute(std::int32_t mode, std::int32_t enabled, std::int32_t frames, float value);

} } // namespace shenmue::s2

#else

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/lflt.hpp"
#include "s2/lrgb.hpp"
#include "s2/weather_table.hpp"

#include <cstddef>
#include <cstdint>
#include <array>

namespace shenmue::s2 {
struct MapInfoChunk;
struct WeatherFixcNode;
extern WeatherFixcNode *g_secondaryWeatherWindList;

std::int32_t shenClearData(void *start, const void *end);

WeatherFloat *GetWeatherPressureLflt();
WeatherFloat *GetWeatherRippleLflt();

// PC v1.07 WIND (0x40): the weather task's wind state, eased toward the WDAT day's wind phase and speed.
struct WeatherWind {
    std::int32_t outputPhase;              // +0x00
    std::int32_t targetPhase;              // +0x04
    std::int32_t requestedPhase;           // +0x08
    std::int32_t phaseStep;                // +0x0c
    std::int32_t phaseFramesRemaining;     // +0x10
    float sampledWindSpeed;                // +0x14
    float currentWindSpeed;                // +0x18
    float requestedWindSpeed;              // +0x1c
    float windSpeedStep;                   // +0x20
    std::int32_t windSpeedFramesRemaining; // +0x24
    Vector3 windDirection;                 // +0x28
    float windScaleDenominator;            // +0x34
    std::int32_t holdPreviousOutput;       // +0x38
    std::int32_t cooldownFramesRemaining;  // +0x3c
};
static_assert(sizeof(WeatherWind) == 0x40);
static_assert(offsetof(WeatherWind, sampledWindSpeed) == 0x14);
static_assert(offsetof(WeatherWind, windDirection) == 0x28);
static_assert(offsetof(WeatherWind, cooldownFramesRemaining) == 0x3c);

// Native WDAT/WEXE/WAIR path publishing sun/light state, global lighting,
// fog, AIR gradients and cloud selection. WIND/FIXC particles remain separate.
inline constexpr shen_name TASK_WEXE = SHEN_NAME(WEXE);
inline constexpr shen_name TASK_WDTA = SHEN_NAME(WDTA);
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
// Native SetWeatherUpdateEnabledTo: ExecuteWeather_Callback does nothing while it is cleared.
void SetWeatherUpdateEnabledTo(std::int32_t enabled);
std::uint64_t GetWeatherUpdateEnabled();
std::int32_t GetCurrentWeather();
// Native wdatSetShouldUpdateDayTo; WDAT setup clears it and FLGS/STEP publish internal flag 1001.
void SetWeatherShouldUpdateDay(std::int32_t flag);
void SetWeatherMinimumSunElevation(float elevation);
// SetWeatherLightManager is an independent lens-flare-manager gate, not ambient lighting.
std::int32_t SetWeatherLightManager(std::int32_t enabled);
// Native getters flush sub-epsilon values in the published LFLT owners.
float GetWeatherPressure();
extern std::int32_t g_planetPreviousRotationDay, g_planetCurrentRotationDay, g_planetRotationCycleDays;
void ApplyPlanetRotForCurrentDateTime();
float UpdateWeatherAndTime(std::int32_t minutes);
float GetWeatherWindVector();
// The two unmasked colours at native interpolated weather +0x48/+0x50.
std::array<std::uint32_t, 2> GetWeatherWaveColours();
// FTBL475: process-lifetime, mutable borrowed storage; each weather publication overwrites all six vectors.
// Mixed payload bytes: signed seconds +0x00, nine floats +0x04, three packed colours +0x28, three floats +0x34,
// five packed environment colours +0x40. The final twelve bytes are incidental native stack padding, not defined weather values.
inline constexpr std::size_t kInterpolatedWeatherVectorCount = 6;
struct WeatherTimeline {
    const std::uint32_t *nodes;
    const std::uint32_t *current;
    std::int32_t nodeCount;
    float blend;
    float intensity;
    std::uint32_t padding1c;
};

// Native WDAT day table: each day byte holds the wind direction sector in its high nibble and the wind
// strength in eighths in its low nibble.
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
static_assert(sizeof(WeatherTable) == 0x10);
static_assert(sizeof(WeatherAir) == 0x90);
static_assert(offsetof(WeatherAir, seconds) == 0x2c);
static_assert(offsetof(WeatherAir, snow) == 0x38);
static_assert(offsetof(WeatherAir, auxiliary) == 0x3c);
static_assert(offsetof(WeatherAir, colour50) == 0x50);
static_assert(offsetof(WeatherAir, colour88) == 0x88);
static_assert(sizeof(WeatherTableSample) == sizeof(Vector4) * kInterpolatedWeatherVectorCount);
Vector4 *GetInterpolatedWeatherValues();
// Native Weather::GetWind, GetEffectiveWindSpeed and GetCurrentWindPhaseAccumulator. Their debug overrides
// are never set by retail code.
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

#endif
