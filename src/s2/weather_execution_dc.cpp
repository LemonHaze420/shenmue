#include "s2/weather_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/weather_data_dc.hpp"
#include "s2/weather_particles_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "shenmue.h"
#include "s2/weather_wind_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/scene_data.hpp"
#include <cstring>
#include "s2/date_time.hpp"
#include "s2/weather_air_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/weather.hpp"
#include "s2/lens_flare_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/window.hpp"
#include "s2/scenario_subtitles_dc.hpp"
#include "s2/font.hpp"
#include "s2/generated_surface_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/scene_assets_dc.hpp"
#include <cmath>
#include "s2/camera_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/air_dc.hpp"
#include "s2/scnw_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "platform/dc/ninja.hpp"
#include "s2/character_dc.hpp"
#include "platform/dc/matrix.hpp"

namespace shenmue {
namespace s2 {

void UpdateWeatherForCurrentTimeImmediate();

void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);

void ExecuteWeather_Callback(void *);

WeatherData *GetWeatherExecutionData();

void UpdateWeather();

void SetWeatherPlanetUpdateEnabled(std::int32_t enabled);

std::int32_t GetWeatherPlanetUpdateEnabled();

void SetWeatherSkyTransitionFrames(std::int32_t frames);

void ProcessNewWeatherData();

void SetWeatherCloudImmediate(float intensity);

void BeginWeatherCloudTransition(float intensity, std::int32_t frames);

void SetWeatherAuxiliaryImmediate(float intensity);

void BeginWeatherAuxiliaryTransition(float intensity, std::int32_t frames);

void UnlinkActiveWeatherWindFromFixc();

void UnlinkWeatherWindFromFixc(WeatherWind *wind);

bool IsSecondaryWeatherWindRegistered(WeatherWind *wind);

void EnsureWeatherWind();

void UpdateWeatherWind();

WeatherWind *GetWind();

void UpdateSecondaryWeatherWind();

void RegisterWeatherFixc(WeatherFixcOwner *owner);

void UpdateWeatherParticleGroups();

bool IsWeatherFixcRegistered(WeatherFixcOwner *owner);

void InitWeatherRainForAllFixc(float scale);

void SetWeatherRainPressureImmediate(float pressure);

void BeginWeatherRainTransition(float pressure, std::int32_t frames);

float GetWeatherPressure();

void SetWeatherParticleRainColour(std::uint32_t colour);

void InitWeatherRippleForAllFixc(float scale);

void BeginWeatherRippleTransition(float pressure, std::int32_t frames);

void SetWeatherParticleRippleColour(std::uint32_t colour);

void EnsureWeatherFixcRegistered();

void UnregisterActiveWeatherFixc();

void LoadWeatherClipChunk(const MapInfoChunk *chunk);

void ReadWeatherClipData(const void *data);

std::uint32_t GetCurrentWeatherInstruction();

void UpdateSunOrWindRotationVector();

void ApplyPlanetRotForCurrentDateTime();

void SetWeatherTableMode(std::int32_t mode);

void ReadWeatherConditionData();

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void SetWeatherFixcRainColour(WeatherFixcOwner *owner, std::uint32_t colour);

void SetWeatherFixcRippleColour(WeatherFixcOwner *owner, std::uint32_t colour);

void SetWeatherChannelOverride(float intensity, std::int32_t channel, std::int32_t enabled);

void SetRainOwnerRippleColour(WeatherRainOwner *owner, std::uint32_t colour);

void SetWeatherChannelOverride(float intensity, std::int32_t channel, std::int32_t enabled);

HLib::Task *AllocateICAR(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused);

void UpdateWeatherInteractionCamera();

void UpdateWeatherParticleGroundCameraPosition();

WeatherFixcOwner *AllocateWeatherFixc(std::int32_t fov);

std::uint32_t GetWeatherFixcType(const WeatherFixcOwner *owner);

void FreeWeatherFixc(WeatherFixcOwner *owner);

void AddWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind);

void RemoveWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind);

std::uint32_t GetWeatherFixcRainFlags(const WeatherFixcOwner *owner);

std::uint32_t GetWeatherFixcRippleFlags(const WeatherFixcOwner *owner);

void SetRainOwnerColour(WeatherRainOwner *owner, std::uint32_t colour);

void SetWeatherFixcRainDrift(WeatherFixcOwner *owner, const Vector3 *drift, const Vector3 *variance);

void SetWeatherFixcRainParameters(WeatherFixcOwner *owner, float first, float second);

void SetWeatherFixcRippleCountImmediate(WeatherFixcOwner *owner, float pressure);

void SetWeatherFixcRippleTimeStep(WeatherFixcOwner *owner, float step);

void FreeWeatherFixcRain(WeatherFixcOwner *owner);

void FreeWeatherFixcRipples(WeatherFixcOwner *owner);

void EnsureWeatherRainForFixc(WeatherFixcOwner *owner, float scale);

void EnsureWeatherRippleForFixc(WeatherFixcOwner *owner, float scale);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void SetWeatherParticleDrawingEnabled(std::int32_t enabled);

void SetWeatherFixcRainCountImmediate(WeatherFixcOwner *owner, float pressure);

void BeginWeatherFixcRainCountTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames);

void BeginWeatherFixcRippleTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void ResetWeatherChannelOverrides();

WeatherData *SetupWeatherData(std::int32_t baseYear);

void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);

float GetWeatherCloudIntensity(const WeatherData *data);

float GetWeatherRainIntensity(const WeatherData *data);

float GetWeatherAuxiliaryIntensity(const WeatherData *data);

std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);

float GetWeatherIntensityForCurrentDay(const WeatherData *data);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void ResetWeatherChannelOverrides();

WeatherData *SetupWeatherData(std::int32_t baseYear);

void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);

float GetWeatherCloudIntensity(const WeatherData *data);

float GetWeatherRainIntensity(const WeatherData *data);

float GetWeatherAuxiliaryIntensity(const WeatherData *data);

std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);

float GetWeatherIntensityForCurrentDay(const WeatherData *data);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void ResetWeatherChannelOverrides();

WeatherData *SetupWeatherData(std::int32_t baseYear);

void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);

float GetWeatherCloudIntensity(const WeatherData *data);

float GetWeatherRainIntensity(const WeatherData *data);

float GetWeatherAuxiliaryIntensity(const WeatherData *data);

std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);

float GetWeatherIntensityForCurrentDay(const WeatherData *data);

void FreeWeatherFixc(WeatherFixcOwner *owner);

void FreeWeatherRainOwnerMode1(WeatherRainOwner *owner);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void ResetWeatherChannelOverrides();

WeatherData *SetupWeatherData(std::int32_t baseYear);

void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);

float GetWeatherCloudIntensity(const WeatherData *data);

float GetWeatherRainIntensity(const WeatherData *data);

float GetWeatherAuxiliaryIntensity(const WeatherData *data);

std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);

float GetWeatherIntensityForCurrentDay(const WeatherData *data);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void ResetWeatherChannelOverrides();

WeatherData *SetupWeatherData(std::int32_t baseYear);

void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);

float GetWeatherCloudIntensity(const WeatherData *data);

float GetWeatherRainIntensity(const WeatherData *data);

float GetWeatherAuxiliaryIntensity(const WeatherData *data);

std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);

float GetWeatherIntensityForCurrentDay(const WeatherData *data);

void GetDefaultWeatherSpawnDrift(Vector3 *direction);

void GetDefaultWeatherDrift(Vector3 *drift);

void SetRainAndRippleDrawingTo(std::int32_t enabled);

void SetWeatherFixcRainCountImmediate(WeatherFixcOwner *owner, float pressure);

void InitWeibullResources();

void UpdateWindWithSpeed(WeatherWind *wind);

WeatherWind *AllocateWIND();

void BeginWindPhaseTransition(WeatherWind *wind, std::int32_t phase);

void BeginWindSpeedTransition(WeatherWind *wind, float speed);

HLib::Task *AllocateICAR(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused);

void UpdateWeatherInteractionCamera();

void UpdateWeatherParticleGroundCameraPosition();

WeatherFixcOwner *AllocateWeatherFixc(std::int32_t fov);

void RemoveWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind);

void SetRainOwnerColour(WeatherRainOwner *owner, std::uint32_t colour);

void EnsureWeatherRainForFixc(WeatherFixcOwner *owner, float scale);

void EnsureWeatherRippleForFixc(WeatherFixcOwner *owner, float scale);

void SetWeatherParticleDrawingEnabled(std::int32_t enabled);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void ResetWeatherChannelOverrides();

WeatherData *SetupWeatherData(std::int32_t baseYear);

void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);

float GetWeatherCloudIntensity(const WeatherData *data);

float GetWeatherRainIntensity(const WeatherData *data);

float GetWeatherAuxiliaryIntensity(const WeatherData *data);

std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);

float GetWeatherIntensityForCurrentDay(const WeatherData *data);

WeatherExecutionState g_weatherExecution = {};

float weatherRippleLifetimeStep = 0.0f, weatherRippleLifetimeScale = 0.0f;

Vector3 g_weatherClipCameraPosition;

WeatherWind *&weatherWind = g_weatherExecution.wind;

std::byte g_weatherConditions[0xf60] = {};

WeatherTable g_weatherTables[4] = {};

static WeatherTable weatherDefaultTables[4] = {{SHEN_FIELD(rows) g_weatherConditions, SHEN_FIELD(count) 11},
    {SHEN_FIELD(rows) g_weatherConditions + 0x420, SHEN_FIELD(count) 10}, {SHEN_FIELD(rows) g_weatherConditions + 0x7e0, SHEN_FIELD(count) 10},
    {SHEN_FIELD(rows) g_weatherConditions + 0xba0, SHEN_FIELD(count) 10}};

WeatherTableSample *g_sceneWeatherData = 0;

WeatherTable g_sceneWeatherTables[4] = {{SHEN_FIELD(rows) 0, SHEN_FIELD(count) 11}, {SHEN_FIELD(rows) 0, SHEN_FIELD(count) 10},
    {SHEN_FIELD(rows) 0, SHEN_FIELD(count) 10}, {SHEN_FIELD(rows) 0, SHEN_FIELD(count) 10}};

WeatherOverrideState g_weatherOverrideState;

float g_weatherReservedOverrides[2];

void UpdateWeatherParticleGroundCameraPosition();

void UpdateWindWithSpeed(WeatherWind *wind);

void UpdateSunLensFlare();

void Weather_UpdateWCCPClipSet0();

void Weather_UpdateWCCPClipSet1();

void UpdateWeatherInteractionCamera();

void SetWeatherParticleDrawingEnabled(std::int32_t enabled);

void SetRainOwnerColour(WeatherRainOwner *owner, std::uint32_t colour);

void SetRainGroupColour(WeatherParticleGroup *group, std::uint32_t colour);

void GetDefaultWeatherRainParameters(float *first, float *second);

void GetDefaultWeatherDriftVariance(Vector3 *direction);

std::uint32_t GetDefaultWeatherRainColour();

float GetDefaultWeatherRippleTimeStep();

std::uint32_t GetDefaultWeatherRippleColour();

void InitWeibullResources();

void shenLoadSetLensflare();

void EnsureWeatherRainForFixc(WeatherFixcOwner *owner, float scale);

void EnsureWeatherRippleForFixc(WeatherFixcOwner *owner, float scale);

void UpdateWeatherFixc(WeatherFixcOwner *owner);

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FreeWeatherParticleGroup(WeatherParticleGroup *group);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FreeWeatherRainMode0Group(WeatherParticleGroup *group);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FreeWeatherRainMode1Group(WeatherParticleGroup *group);
#endif

std::uint32_t GetWeatherFixcRainFlags(const WeatherFixcOwner *owner);

std::uint32_t GetWeatherFixcRippleFlags(const WeatherFixcOwner *owner);

void SetWeatherFixcRainDrift(WeatherFixcOwner *owner, const Vector3 *drift, const Vector3 *variance);

void SetWeatherFixcRainParameters(WeatherFixcOwner *owner, float first, float second);

void SetWeatherFixcRippleCountImmediate(WeatherFixcOwner *owner, float pressure);

void SetWeatherFixcRippleTimeStep(WeatherFixcOwner *owner, float step);

void EnsureWeatherRainForFixc(WeatherFixcOwner *owner, float scale);

void EnsureWeatherRippleForFixc(WeatherFixcOwner *owner, float scale);

void SetWeatherFixcRainCountImmediate(WeatherFixcOwner *owner, float pressure);

void BeginWeatherFixcRainCountTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames);

void BeginWeatherFixcRippleTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames);

// @verified - d
void ResetReservedWeatherOverrides() {
    g_weatherReservedOverrides[0] = 0.0f;
    g_weatherReservedOverrides[1] = 0.0f;
}

float GetReservedWeatherOverride0() {
    return g_weatherReservedOverrides[0];
}

void SetReservedWeatherOverride0(float value) {
    g_weatherReservedOverrides[0] = value;
}

float GetReservedWeatherOverride1() {
    return g_weatherReservedOverrides[1];
}

void SetReservedWeatherOverride1(float value) {
    g_weatherReservedOverrides[1] = value;
}

// @verified - d
void ResetWeatherOverrides() {
    g_weatherOverrideState.pendingFlags = 0;
    g_weatherOverrideState.releaseFlags = 0;
    g_weatherOverrideState.overrides[0].frames = 0;
    g_weatherOverrideState.overrides[0].current = 0.0f;
    g_weatherOverrideState.overrides[0].step = 0.0f;
    g_weatherOverrideState.overrides[0].target = 0.0f;
    g_weatherOverrideState.overrides[1].frames = 0;
    g_weatherOverrideState.overrides[1].current = 0.0f;
    g_weatherOverrideState.overrides[1].step = 0.0f;
    g_weatherOverrideState.overrides[1].target = 0.0f;
    g_weatherOverrideState.overrides[2].frames = 0;
    g_weatherOverrideState.overrides[2].current = 0.0f;
    g_weatherOverrideState.overrides[2].step = 0.0f;
    g_weatherOverrideState.overrides[2].target = 0.0f;
}

// @verified - d
void UpdateWeatherForMinute(std::int32_t mode, std::int32_t enabled, std::int32_t frames, float value) {
    DateTime time;
    std::int32_t minutes;
    float step;
    shenGetCurrentDateTime(time);
    UpdateWeatherTimelines(g_weatherExecution.data, time.year, time.month, time.day, time.hour, time.minute);
    if (value < 0.0f)
        value = 0.0f;
    else if (value > 1.0f)
        value = 1.0f;
    switch (mode) {
    case 0:
        g_weatherOverrideState.pendingFlags |= 1U;
        g_weatherOverrideState.overrides[0].frames = frames;
        g_weatherOverrideState.overrides[0].current = GetWeatherCloudIntensity(g_weatherExecution.data);
        if (enabled) {
            g_weatherOverrideState.releaseFlags |= 1U;
            minutes = frames / 80 + 1;
            SetWeatherOverrideEnabled(0);
            UpdateWeatherTimelines(g_weatherExecution.data, time.year, time.month, time.day, time.hour, time.minute + minutes);
            value = GetWeatherCloudIntensity(g_weatherExecution.data);
            SetWeatherOverrideEnabled(1);
        } else {
            g_weatherOverrideState.releaseFlags &= ~1U;
        }
        g_weatherOverrideState.overrides[0].target = value;
        if (frames)
            step = (value - g_weatherOverrideState.overrides[0].current) / static_cast<float>(frames);
        else
            step = 0.0f;
        g_weatherOverrideState.overrides[0].step = step;
        SetWeatherSkyTransitionFrames(frames);
        AdvanceWeatherOverride(0);
        BeginWeatherCloudTransition(g_weatherOverrideState.overrides[0].current, 0);
        break;
    case 1:
        if (g_weatherOverrideState.pendingFlags & 8U)
            break;
        g_weatherOverrideState.pendingFlags |= 2U;
        g_weatherOverrideState.overrides[1].frames = frames;
        g_weatherOverrideState.overrides[1].current = GetWeatherRainIntensity(g_weatherExecution.data);
        if (enabled) {
            g_weatherOverrideState.releaseFlags |= 2U;
            minutes = frames / 80 + 1;
            SetWeatherOverrideEnabled(0);
            UpdateWeatherTimelines(g_weatherExecution.data, time.year, time.month, time.day, time.hour, time.minute + minutes);
            value = GetWeatherRainIntensity(g_weatherExecution.data);
            SetWeatherOverrideEnabled(1);
        } else {
            g_weatherOverrideState.releaseFlags &= ~2U;
        }
        g_weatherOverrideState.overrides[1].target = value;
        if (frames)
            step = (value - g_weatherOverrideState.overrides[1].current) / static_cast<float>(frames);
        else
            step = 0.0f;
        g_weatherOverrideState.overrides[1].step = step;
        SetWeatherSkyTransitionFrames(frames);
        AdvanceWeatherOverride(1);
        BeginWeatherRainTransition(g_weatherOverrideState.overrides[1].current, 0);
        BeginWeatherRippleTransition(g_weatherOverrideState.overrides[1].current, 0);
        break;
    case 2:
    case 3:
        break;
    }
}

// @verified - d
void AdvanceWeatherOverride(std::int32_t channel) {
    switch (channel) {
    case 0:
        if (--g_weatherOverrideState.overrides[0].frames < 1) {
            g_weatherOverrideState.overrides[0].frames = 0;
            g_weatherOverrideState.overrides[0].current = g_weatherOverrideState.overrides[0].target;
            if (g_weatherOverrideState.releaseFlags & 1U) {
                g_weatherOverrideState.releaseFlags &= ~1U;
                g_weatherOverrideState.pendingFlags &= ~1U;
                SetWeatherChannelOverride(0.0f, 0, 0);
                SetWeatherSkyTransitionFrames(500);
            } else {
                SetWeatherChannelOverride(g_weatherOverrideState.overrides[0].current, 0, 1);
            }
        } else {
            g_weatherOverrideState.overrides[0].current += g_weatherOverrideState.overrides[0].step;
            SetWeatherChannelOverride(g_weatherOverrideState.overrides[0].current, 0, 1);
        }
        break;
    case 1:
        if (--g_weatherOverrideState.overrides[1].frames < 1) {
            g_weatherOverrideState.overrides[1].frames = 0;
            g_weatherOverrideState.overrides[1].current = g_weatherOverrideState.overrides[1].target;
            if (g_weatherOverrideState.releaseFlags & 2U) {
                g_weatherOverrideState.releaseFlags &= ~2U;
                g_weatherOverrideState.pendingFlags &= ~2U;
                SetWeatherChannelOverride(0.0f, 1, 0);
                SetWeatherSkyTransitionFrames(500);
            } else {
                SetWeatherChannelOverride(g_weatherOverrideState.overrides[1].current, 1, 1);
            }
        } else {
            g_weatherOverrideState.overrides[1].current += g_weatherOverrideState.overrides[1].step;
            SetWeatherChannelOverride(g_weatherOverrideState.overrides[1].current, 1, 1);
        }
        break;
    case 2:
    case 3:
        break;
    }
}

// @verified - d
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour) {
    float halfTurn = 2.0f;
    std::int32_t index;
    if (!group)
        return;
    if (!group->rippleColourRamp) {
        std::uint32_t *const storage = static_cast<std::uint32_t *>(HLib::AllocateTaskData(0x80, WEATHER_RIPPLE_COLOUR_STORAGE));
        group->rippleColourRamp = storage;
        ZeroWordRange(storage, storage + 31);
    }
    const std::uint32_t previous = group->rippleColour;
    if (previous == colour)
        return;
    std::uint32_t *ramp = group->rippleColourRamp;
    if ((previous & 0xff000000U) == (colour & 0xff000000U)) {
        group->rippleColour = colour;
        colour &= 0xffffffU;
        for (index = 0; index < 32; ++index, ++ramp)
            *ramp = (*ramp & 0xff000000U) | colour;
        return;
    }
    group->rippleColour = colour;
    const std::int32_t alpha = colour >> 24;
    colour &= 0xffffffU;
    float phase = 0.5f;
    for (index = 0; index < 32; ++index) {
        const float sine = dc::sdk::SinFixed(static_cast<std::int32_t>((phase * 16384.0f) / halfTurn));
        *ramp++ = (static_cast<std::uint32_t>(alpha * sine * sine * sine) << 24) | colour;
        phase += 0.05f;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetWeatherParticleGroupRippleColour(WeatherParticleGroup *group) {
    return group ? group->rippleColour : 0;
}
#endif

// @verified - d
void ResetWeatherOverrideState() {
    ResetWeatherChannelOverrides();
    ResetWeatherOverrides();
}

// @verified - d
void SetNewWindPhaseAndSpeed(std::int32_t phase, float speed) {
    WeatherWind *const wind = GetWind();
    if (!wind)
        return;
    BeginWindPhaseTransition(wind, phase);
    BeginWindSpeedTransition(wind, speed);
}

// @verified - d
void SyncWindWithWDATWeatherState() {
    WeatherWind *const wind = GetWind();
    if (!wind)
        return;
    WeatherData *const data = GetWeatherExecutionData();
    DateTime time;
    shenGetCurrentDateTime(time);
    UpdateWeatherTimelines(data, time.year, time.month, time.day, time.hour, time.minute);
    BeginWindPhaseTransition(wind, GetWeatherPhaseForCurrentDay(data));
    BeginWindSpeedTransition(wind, GetWeatherIntensityForCurrentDay(data) / 30.0f);
}

// @verified - d
void Weather_RegisterActiveFIXCAndInitEffects() {
    RegisterWeatherFixc(g_weatherExecution.activeFixc);
    g_weatherExecution.fixcRegistered = 1;
    InitWeatherRainForAllFixc(1.5f);
    InitWeatherRippleForAllFixc(0.95f);
}

// @verified - d
void QueueWeatherExecution(const MapInfoChunk *weatherChunk) {
    weatherRippleLifetimeStep = 0.05f;
    weatherRippleLifetimeScale = 1.0f;
    AllocateICAR(2.5f, 4.0f, 4.0f, 0.01f);
    ResetWeatherChannelOverrides();
    ResetWeatherOverrides();
    ResetReservedWeatherOverrides();
    ReadWeatherConditionData();
    SetWeatherTableMode(1);
    g_sceneWeatherData = 0;
    g_weatherExecution.updateEnabled = 1;
    g_weatherExecution.updateTime = 0;
    g_weatherExecution.task = HLib::EnqueueTaskWithoutParameter(ExecuteWeather_Callback, 13, 8, WEATHER_EXECUTION_TASK);
    g_weatherExecution.data = SetupWeatherData(86);
    g_weatherExecution.minute = 0;
    g_weatherExecution.hour = 0;
    g_weatherExecution.day = 0;
    g_weatherExecution.month = 0;
    g_weatherExecution.year = 0;
    g_weatherExecution.air = 0;
    g_weatherExecution.lightManagerEnabled = 1;
    g_weatherExecution.lightingEnabled = 1;
    g_weatherExecution.skyTransitionFrames = 500;
    g_weatherExecution.planetUpdateEnabled = 1;
    g_weatherExecution.cloud = AllocateLFLT();
    g_weatherExecution.auxiliary = AllocateLFLT();
    g_weatherExecution.wind = 0;
    g_weatherExecution.secondaryWindList = 0;
    g_weatherExecution.weatherEffects = 0;
    g_weatherExecution.pressure = AllocateLFLT();
    g_weatherExecution.rainColour = GetDefaultWeatherRainColour();
    GetDefaultWeatherRainParameters(&g_weatherExecution.rainPressureParameter0, &g_weatherExecution.rainPressureParameter1);
    GetDefaultWeatherDrift(&g_weatherExecution.spawnDrift);
    GetDefaultWeatherDriftVariance(&g_weatherExecution.driftVariance);
    g_weatherExecution.rippleCount = AllocateLFLT();
    g_weatherExecution.rippleColour = GetDefaultWeatherRippleColour();
    g_weatherExecution.rippleDeltaTime = GetDefaultWeatherRippleTimeStep();
    g_weatherExecution.planetLayerOffset.x = 0.0f;
    g_weatherExecution.planetLayerOffset.y = 0.0f;
    g_weatherExecution.instructionFlags = 0x3fff;
    for (std::int32_t index = 0; index < 2; ++index) {
        weatherClipSets[index].enabled = 0;
        weatherClipSets[index].count = 0;
        weatherClipSets[index].entries = 0;
    }
    InitWeibullResources();
    EnsureWeatherWind();
    g_weatherExecution.activeFixc = 0;
    g_weatherExecution.fixcRegistered = 0;
    EnsureWeatherFixcRegistered();
    UnregisterActiveWeatherFixc();
    UpdateWeatherForCurrentTimeImmediate();
    UpdateWeather();
    LoadWeatherClipChunk(weatherChunk);
    shenLoadSetLensflare();
}

// @verified - d
void SetWeatherUpdateEnabledTo(std::int32_t enabled) {
    g_weatherExecution.updateEnabled = enabled;
}

// @verified - d
std::int32_t GetWeatherUpdateEnabled() {
    return g_weatherExecution.updateEnabled;
}

// @verified - d
void UpdateWeatherForCurrentTimeImmediate() {
    g_weatherExecution.updateTime = 1;
    DateTime date;
    shenGetCurrentDateTime(date);
    UpdateWeatherTimelines(g_weatherExecution.data, date.year, date.month, date.day, date.hour, date.minute);
    SetWeatherCloudImmediate(GetWeatherCloudIntensity(g_weatherExecution.data));
    SetWeatherAuxiliaryImmediate(GetWeatherAuxiliaryIntensity(g_weatherExecution.data));
    SetWeatherRainPressureImmediate(GetWeatherRainIntensity(g_weatherExecution.data));
    if (g_weatherExecution.wind) {
        BeginWindPhaseTransition(g_weatherExecution.wind, GetWeatherPhaseForCurrentDay(g_weatherExecution.data));
        BeginWindSpeedTransition(g_weatherExecution.wind, GetWeatherIntensityForCurrentDay(g_weatherExecution.data) / 30.0f);
    }
    g_weatherExecution.year = date.year;
    g_weatherExecution.month = date.month;
    g_weatherExecution.day = date.day;
    g_weatherExecution.hour = date.hour;
    g_weatherExecution.minute = date.minute;
}

// @verified - d
void RefreshWeatherForCurrentTime(std::int32_t frames) {
    g_weatherExecution.updateTime = 1;
    DateTime time;
    shenGetCurrentDateTime(time);
    const std::int32_t totalMinutes = frames / 240;
    time.minute = static_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(time.minute) + totalMinutes % 60));
    const std::int32_t hours = totalMinutes / 60;
    time.hour = static_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(time.hour) + hours % 24));
    time.day = static_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(time.day) + hours / 24));
    UpdateWeatherTimelines(g_weatherExecution.data, time.year, time.month, time.day, time.hour, time.minute);
    BeginWeatherCloudTransition(GetWeatherCloudIntensity(g_weatherExecution.data), frames);
    BeginWeatherAuxiliaryTransition(GetWeatherAuxiliaryIntensity(g_weatherExecution.data), frames);
    BeginWeatherRainTransition(GetWeatherRainIntensity(g_weatherExecution.data), frames);
    WeatherWind *wind = g_weatherExecution.wind;
    if (wind) {
        const std::int32_t phase = GetWeatherPhaseForCurrentDay(g_weatherExecution.data);
        BeginWindPhaseTransition(wind, phase);
        wind = g_weatherExecution.wind;
        const float speed = GetWeatherIntensityForCurrentDay(g_weatherExecution.data) / 30.0f;
        BeginWindSpeedTransition(wind, speed);
    }
    g_weatherExecution.year = time.year;
    g_weatherExecution.month = time.month;
    g_weatherExecution.day = time.day;
    g_weatherExecution.hour = time.hour;
    g_weatherExecution.minute = time.minute;
}

void ResetWeatherUpdateTime() {
    g_weatherExecution.updateTime = 0;
}

std::int32_t GetWeatherUpdateTime() {
    return g_weatherExecution.updateTime;
}

// @verified - d
void ExecuteWeather_Callback(void *) {
    if (!g_weatherExecution.updateEnabled)
        return;
    weatherRippleLifetimeStep = 0.05f;
    weatherRippleLifetimeScale = 1.0f;
    if (g_weatherExecution.updateTime == 1) {
        DateTime date;
        shenGetCurrentDateTime(date);
        if (g_weatherExecution.year != date.year || g_weatherExecution.month != date.month || g_weatherExecution.day != date.day) {
            if (g_weatherExecution.wind) {
                UpdateWeatherTimelines(g_weatherExecution.data, date.year, date.month, date.day, date.hour, date.minute);
                BeginWindPhaseTransition(g_weatherExecution.wind, GetWeatherPhaseForCurrentDay(g_weatherExecution.data));
                BeginWindSpeedTransition(g_weatherExecution.wind, GetWeatherIntensityForCurrentDay(g_weatherExecution.data) / 30.0f);
            }
            g_weatherExecution.year = date.year;
            g_weatherExecution.month = date.month;
            g_weatherExecution.day = date.day;
        }
        if (g_weatherOverrideState.pendingFlags & 1U) {
            AdvanceWeatherOverride(0);
            BeginWeatherCloudTransition(g_weatherOverrideState.overrides[0].current, 0);
        }
        if (g_weatherOverrideState.pendingFlags & 8U) {
            AdvanceWeatherOverride(3);
            BeginWeatherRainTransition(g_weatherOverrideState.overrides[1].current, 0);
            BeginWeatherRippleTransition(g_weatherOverrideState.overrides[1].current, 0);
        } else {
            if (g_weatherOverrideState.pendingFlags & 2U) {
                AdvanceWeatherOverride(1);
                BeginWeatherRainTransition(g_weatherOverrideState.overrides[1].current, 0);
                BeginWeatherRippleTransition(g_weatherOverrideState.overrides[1].current, 0);
            }
            if (g_weatherOverrideState.pendingFlags & 4U)
                AdvanceWeatherOverride(2);
        }
        if (g_weatherExecution.hour != date.hour || g_weatherExecution.minute != date.minute) {
            DateTime increment;
            increment.year = increment.month = increment.day = increment.hour = increment.second = 0;
            increment.minute = 1;
            IncrementDateTimeInPlace(date, increment);
            UpdateWeatherTimelines(g_weatherExecution.data, date.year, date.month, date.day, date.hour, date.minute);
            if ((g_weatherOverrideState.pendingFlags & 1U) == 0)
                BeginWeatherCloudTransition(GetWeatherCloudIntensity(g_weatherExecution.data), 80);
            if (!(g_weatherOverrideState.pendingFlags & 8U)) {
                if (!(g_weatherOverrideState.pendingFlags & 2U)) {
                    BeginWeatherRainTransition(GetWeatherRainIntensity(g_weatherExecution.data), 80);
                    BeginWeatherRippleTransition(GetWeatherRainIntensity(g_weatherExecution.data), 80);
                }
            }
            BeginWeatherAuxiliaryTransition(GetWeatherAuxiliaryIntensity(g_weatherExecution.data), 80);
            g_weatherExecution.hour = date.hour;
            g_weatherExecution.minute = date.minute;
        }
        if (GetCurrentWeatherInstruction() & 0x2000U)
            UpdateSunOrWindRotationVector();
        ApplyPlanetRotForCurrentDateTime();
    }
    UpdateWeatherParticleGroundCameraPosition();
    UpdateWeatherWind();
    UpdateSecondaryWeatherWind();
    ProcessNewWeatherData();
    UpdateSunLensFlare();
    camCtrlGetNextCamPos(&g_weatherClipCameraPosition);
    Weather_UpdateWCCPClipSet0();
    Weather_UpdateWCCPClipSet1();
    UpdateWeatherInteractionCamera();
    UpdateWeatherParticleGroups();
    SetRainAndRippleDrawingTo(g_worldDrawingEnabled);
}

WeatherData *GetWeatherExecutionData() {
    return g_weatherExecution.data;
}

// @verified - d
void UpdateWeather() {
    if (g_weatherExecution.air)
        return;
    g_weatherExecution.air = AllocateWeatherAir(0, g_weatherTables, 4);
    EnsureWeatherFixcRegistered();
    SetWeatherPlanetUpdateEnabled(1);
    DateTime time;
    shenGetCurrentDateTime(time);
    SetWeatherAirTime(g_weatherExecution.air, time.month, time.day, time.hour, time.minute, time.second);
    UpdateWeatherTimelines(g_weatherExecution.data, time.year, time.month, time.day, time.hour, time.minute);
    SetWeatherCloudImmediate(GetWeatherCloudIntensity(g_weatherExecution.data));
    SetWeatherAuxiliaryImmediate(GetWeatherAuxiliaryIntensity(g_weatherExecution.data));
    SetWeatherRainPressureImmediate(GetWeatherRainIntensity(g_weatherExecution.data));
    SetWeatherAirCloud(g_weatherExecution.air, GetLFLTCurrent(g_weatherExecution.cloud));
    SetWeatherAirRain(g_weatherExecution.air, GetWeatherPressure());
    SetWeatherAirAuxiliary(g_weatherExecution.air, GetLFLTCurrent(g_weatherExecution.auxiliary));
    if (g_weatherExecution.lightManagerEnabled == 1)
        UpdateFog(*g_weatherExecution.air);
    if (g_weatherExecution.lightingEnabled == 1)
        UpdateClouds(g_weatherExecution.air);
    if (GetWeatherPlanetUpdateEnabled() == 1) {
        SetWeatherParticleRainColour(wairPackRainColorRGBA(g_weatherExecution.air));
        SetWeatherParticleRippleColour(wairPackRippleColorRGBA(g_weatherExecution.air));
    }
}

// @verified - d
void FreeWAIR() {
    if (!g_weatherExecution.air)
        return;
    FreeWeatherAir(g_weatherExecution.air);
    g_weatherExecution.air = 0;
    SetWeatherPlanetUpdateEnabled(0);
}

WeatherAir *GetWeatherAir() {
    return g_weatherExecution.air;
}

void SetWeatherLightManagerEnabled(std::int32_t enabled) {
    g_weatherExecution.lightManagerEnabled = enabled;
}

std::int32_t GetWeatherLightManagerEnabled() {
    return g_weatherExecution.lightManagerEnabled;
}

void SetWeatherLightingEnabled(std::int32_t enabled) {
    g_weatherExecution.lightingEnabled = enabled;
}

std::int32_t GetWeatherLightingEnabled() {
    return g_weatherExecution.lightingEnabled;
}

// @verified - d
void SetWeatherPlanetUpdateEnabled(std::int32_t enabled) {
    g_weatherExecution.planetUpdateEnabled = enabled;
}

// @verified - d
std::int32_t GetWeatherPlanetUpdateEnabled() {
    return g_weatherExecution.planetUpdateEnabled;
}

// @verified - d
void SetWeatherSkyTransitionFrames(std::int32_t frames) {
    g_weatherExecution.skyTransitionFrames = frames;
}

// @verified - d
void ProcessNewWeatherData() {
    if (!g_weatherExecution.air)
        return;
    AdvanceWeatherFloat(g_weatherExecution.cloud);
    AdvanceWeatherFloat(g_weatherExecution.auxiliary);
    DateTime time;
    shenGetCurrentDateTime(time);
    SetWeatherAirTime(g_weatherExecution.air, time.month, time.day, time.hour, time.minute, time.second);
    SetWeatherAirCloud(g_weatherExecution.air, GetLFLTCurrent(g_weatherExecution.cloud));
    SetWeatherAirRain(g_weatherExecution.air, GetWeatherPressure());
    SetWeatherAirAuxiliary(g_weatherExecution.air, GetLFLTCurrent(g_weatherExecution.auxiliary));
    if (g_weatherExecution.lightManagerEnabled == 1)
        UpdateFog(*g_weatherExecution.air);
    if (g_weatherExecution.lightingEnabled == 1)
        UpdateWeatherClouds(g_weatherExecution.air, g_weatherExecution.skyTransitionFrames);
    if (GetWeatherPlanetUpdateEnabled() == 1) {
        SetWeatherParticleRainColour(wairPackRainColorRGBA(g_weatherExecution.air));
        SetWeatherParticleRippleColour(wairPackRippleColorRGBA(g_weatherExecution.air));
    }
}

// @verified - d
void SetWeatherCloudImmediate(float intensity) {
    lfltSetImmediate(g_weatherExecution.cloud, intensity);
}

// @verified - d
void BeginWeatherCloudTransition(float intensity, std::int32_t frames) {
    BeginWeatherFloatTransition(g_weatherExecution.cloud, intensity, frames);
}

// @verified - d
float GetWeatherWindVector() {
    return GetLFLTCurrent(g_weatherExecution.cloud);
}

// @verified - d
void SetWeatherAuxiliaryImmediate(float intensity) {
    lfltSetImmediate(g_weatherExecution.auxiliary, intensity);
}

// @verified - d
void BeginWeatherAuxiliaryTransition(float intensity, std::int32_t frames) {
    BeginWeatherFloatTransition(g_weatherExecution.auxiliary, intensity, frames);
}

float GetWeatherAuxiliary() {
    return GetLFLTCurrent(g_weatherExecution.auxiliary);
}

void SetWeatherAirTableData(std::int32_t index, const std::byte *rows, std::int32_t count) {
    if (g_weatherExecution.air)
        SetWeatherAirTable(g_weatherExecution.air, index, rows, count);
}

const std::byte *GetWeatherAirTable(std::int32_t index, std::int32_t *count) {
    const std::byte *data = 0;
    if (g_weatherExecution.air) {
        data = GetWeatherAirTableData(g_weatherExecution.air, index);
        *count = GetWeatherAirTableCount(g_weatherExecution.air, index);
    }
    return data;
}

// @verified - d
void EnsureWeatherWind() {
    if (!g_weatherExecution.wind)
        g_weatherExecution.wind = AllocateWIND();
}

void FreeWeatherWind() {
    if (g_weatherExecution.wind) {
        UnlinkActiveWeatherWindFromFixc();
        FreeWIND(g_weatherExecution.wind);
        g_weatherExecution.wind = 0;
    }
}

// @verified - d
void UpdateWeatherWind() {
    if (g_weatherExecution.wind)
        UpdateWindWithSpeed(g_weatherExecution.wind);
}

// @verified - d
WeatherWind *GetWind() {
    return g_weatherExecution.wind;
}

void RegisterSecondaryWeatherWind(WeatherWind *wind) {
    if (IsSecondaryWeatherWindRegistered(wind))
        return;
    WeatherFixcNode *const node = AllocateWeatherFixcNode(0, wind);
    if (!g_weatherExecution.secondaryWindList)
        g_weatherExecution.secondaryWindList = node;
    else
        LinkWeatherFixcNodes(GetLastWeatherFixcNode(g_weatherExecution.secondaryWindList), node);
}

void UnregisterSecondaryWeatherWind(WeatherWind *wind) {
    UnlinkWeatherWindFromFixc(wind);
    WeatherFixcNode *const node = FindWeatherFixcNode(g_weatherExecution.secondaryWindList, wind);
    if (node) {
        if (g_weatherExecution.secondaryWindList == node)
            g_weatherExecution.secondaryWindList = GetNextWeatherFixcNode(node);
        DetachWeatherFixcNode(node);
        FreeWeatherFixcNode(node);
    }
}

// @verified - d
void UpdateSecondaryWeatherWind() {
    for (WeatherFixcNode *node = g_weatherExecution.secondaryWindList; node; node = GetNextWeatherFixcNode(node))
        UpdateWindWithSpeed(static_cast<WeatherWind *>(GetWeatherFixcNodeData(node)));
}

std::int32_t GetSecondaryWeatherWinds(WeatherWind **winds) {
    std::int32_t count = 0;
    for (WeatherFixcNode *node = g_weatherExecution.secondaryWindList; node; node = GetNextWeatherFixcNode(node), ++count)
        winds[count] = static_cast<WeatherWind *>(GetWeatherFixcNodeData(node));
    return count;
}

std::int32_t CountSecondaryWeatherWinds() {
    std::int32_t count = 0;
    WeatherFixcNode *node = g_weatherExecution.secondaryWindList;
    while (node) {
        node = GetNextWeatherFixcNode(node);
        ++count;
    }
    return count;
}

bool IsSecondaryWeatherWindRegistered(WeatherWind *wind) {
    if (FindWeatherFixcNode(g_weatherExecution.secondaryWindList, wind))
        return true;
    return false;
}

// @verified - d
void RegisterWeatherFixc(WeatherFixcOwner *owner) {
    if (IsWeatherFixcRegistered(owner))
        return;
    WeatherFixcNode *const node = AllocateWeatherFixcNode(static_cast<std::int32_t>(GetWeatherFixcType(owner)), owner);
    if (!g_weatherExecution.weatherEffects)
        g_weatherExecution.weatherEffects = node;
    else
        LinkWeatherFixcNodes(GetLastWeatherFixcNode(g_weatherExecution.weatherEffects), node);
    if (GetWeatherFixcRainFlags(owner)) {
        SetWeatherFixcRainCountImmediate(owner, GetLFLTCurrent(g_weatherExecution.pressure));
        if (lfltIsInterpolating(g_weatherExecution.pressure)) {
            const float target = GetLFLTTarget(g_weatherExecution.pressure);
            const std::int32_t frames = lfltGetFramesRemaining(g_weatherExecution.pressure);
            BeginWeatherFixcRainCountTransition(owner, target, frames);
        }
        SetWeatherFixcRainColour(owner, g_weatherExecution.rainColour);
        SetWeatherFixcRainParameters(owner, g_weatherExecution.rainPressureParameter0, g_weatherExecution.rainPressureParameter1);
        SetWeatherFixcRainDrift(owner, &g_weatherExecution.spawnDrift, &g_weatherExecution.driftVariance);
    }
    if (GetWeatherFixcRippleFlags(owner)) {
        SetWeatherFixcRippleCountImmediate(owner, GetLFLTCurrent(g_weatherExecution.rippleCount));
        if (lfltIsInterpolating(g_weatherExecution.rippleCount)) {
            const float target = GetLFLTTarget(g_weatherExecution.rippleCount);
            const std::int32_t frames = lfltGetFramesRemaining(g_weatherExecution.rippleCount);
            BeginWeatherFixcRippleTransition(owner, target, frames);
        }
        SetWeatherFixcRippleColour(owner, g_weatherExecution.rippleColour);
        SetWeatherFixcRippleTimeStep(owner, g_weatherExecution.rippleDeltaTime);
    }
    if (g_weatherExecution.wind)
        AddWeatherWindLink(owner, g_weatherExecution.wind);
}

// @verified - d
void UnregisterWeatherFixc(WeatherFixcOwner *owner) {
    WeatherFixcNode *const node = FindWeatherFixcNode(g_weatherExecution.weatherEffects, owner);
    if (node) {
        if (g_weatherExecution.weatherEffects == node)
            g_weatherExecution.weatherEffects = GetNextWeatherFixcNode(node);
        DetachWeatherFixcNode(node);
        FreeWeatherFixcNode(node);
    }
    RemoveWeatherWindLink(owner, g_weatherExecution.wind);
}

// @verified - d
void UpdateWeatherParticleGroups() {
    AdvanceWeatherFloat(g_weatherExecution.pressure);
    AdvanceWeatherFloat(g_weatherExecution.rippleCount);
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        UpdateWeatherFixc(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)));
}

std::int32_t GetWeatherFixcOwners(WeatherFixcOwner **owners) {
    std::int32_t count = 0;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        owners[count++] = static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node));
    return count;
}

std::int32_t CountWeatherFixc() {
    std::int32_t count = 0;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        ++count;
    return count;
}

std::int32_t CountWeatherFixcOfType(std::uint32_t type) {
    std::int32_t count = 0;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        if (GetWeatherFixcType(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node))) == type)
            ++count;
    return count;
}

// @verified - d
bool IsWeatherFixcRegistered(WeatherFixcOwner *owner) {
    if (FindWeatherFixcNode(g_weatherExecution.weatherEffects, owner))
        return true;
    return false;
}

void InitWeatherRainForAllFixc(float scale) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        EnsureWeatherRainForFixc(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), scale);
}

void FreeWeatherRainForAllFixc() {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        FreeWeatherFixcRain(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)));
}

// @verified - d
void SetWeatherRainPressureImmediate(float pressure) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRainCountImmediate(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), pressure);
    lfltSetImmediate(g_weatherExecution.pressure, pressure);
}

// @verified - d
void BeginWeatherRainTransition(float pressure, std::int32_t frames) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        BeginWeatherFixcRainCountTransition(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), pressure, frames);
    BeginWeatherFloatTransition(g_weatherExecution.pressure, pressure, frames);
}

// @verified - d
float GetWeatherPressure() {
    return GetLFLTCurrent(g_weatherExecution.pressure);
}

// @verified - d
void SetWeatherParticleRainColour(std::uint32_t colour) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRainColour(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), colour);
    g_weatherExecution.rainColour = colour;
}

std::uint32_t GetWeatherRainColour() {
    return g_weatherExecution.rainColour;
}

void SetWeatherRainParameters(float first, float second) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRainParameters(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), first, second);
    g_weatherExecution.rainPressureParameter0 = first;
    g_weatherExecution.rainPressureParameter1 = second;
}

void GetWeatherRainParameters(float *first, float *second) {
    *first = g_weatherExecution.rainPressureParameter0;
    *second = g_weatherExecution.rainPressureParameter1;
}

void SetWeatherRainDrift(const Vector3 *drift, const Vector3 *variance) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRainDrift(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), drift, variance);
    g_weatherExecution.spawnDrift.x = drift->x;
    g_weatherExecution.spawnDrift.y = drift->y;
    g_weatherExecution.spawnDrift.z = drift->z;
    g_weatherExecution.driftVariance.x = variance->x;
    g_weatherExecution.driftVariance.y = variance->y;
    g_weatherExecution.driftVariance.z = variance->z;
}

void GetWeatherRainDrift(Vector3 *drift, Vector3 *variance) {
    drift->x = g_weatherExecution.spawnDrift.x;
    drift->y = g_weatherExecution.spawnDrift.y;
    drift->z = g_weatherExecution.spawnDrift.z;
    variance->x = g_weatherExecution.driftVariance.x;
    variance->y = g_weatherExecution.driftVariance.y;
    variance->z = g_weatherExecution.driftVariance.z;
}

void WeatherRainNullsub() {
}

void InitWeatherRippleForAllFixc(float scale) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        EnsureWeatherRippleForFixc(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), scale);
}

void FreeWeatherRipplesForAllFixc() {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        FreeWeatherFixcRipples(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)));
}

// @verified - d
void BeginWeatherRippleTransition(float pressure, std::int32_t frames) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        BeginWeatherFixcRippleTransition(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), pressure, frames);
    BeginWeatherFloatTransition(g_weatherExecution.rippleCount, pressure, frames);
}

float GetWeatherRippleCount() {
    return GetLFLTCurrent(g_weatherExecution.rippleCount);
}

// @verified - d
void SetWeatherParticleRippleColour(std::uint32_t colour) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRippleColour(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), colour);
    g_weatherExecution.rippleColour = colour;
}

std::uint32_t GetWeatherRippleColour() {
    return g_weatherExecution.rippleColour;
}

void SetWeatherRippleTimeStep(float step) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        SetWeatherFixcRippleTimeStep(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), step);
    g_weatherExecution.rippleDeltaTime = step;
}

float GetWeatherRippleTimeStep() {
    return g_weatherExecution.rippleDeltaTime;
}

void LinkActiveWeatherWindToFixc() {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        AddWeatherWindLink(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), g_weatherExecution.wind);
}

void UnlinkActiveWeatherWindFromFixc() {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        RemoveWeatherWindLink(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), g_weatherExecution.wind);
}

void LinkWeatherWindToFixc(WeatherWind *wind) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        AddWeatherWindLink(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), wind);
}

void UnlinkWeatherWindFromFixc(WeatherWind *wind) {
    if (!wind)
        return;
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        RemoveWeatherWindLink(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node)), wind);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void Weather_FreeActiveFIXC() {
    if (g_weatherExecution.activeFixc) {
        UnregisterWeatherFixc(g_weatherExecution.activeFixc);
        FreeWeatherFixc(g_weatherExecution.activeFixc);
        g_weatherExecution.activeFixc = 0;
        g_weatherExecution.fixcRegistered = 0;
    }
}
#endif

// @verified - d
void EnsureWeatherFixcRegistered() {
    if (g_weatherExecution.activeFixc) {
        if (!g_weatherExecution.fixcRegistered)
            Weather_RegisterActiveFIXCAndInitEffects();
    } else if (!g_weatherExecution.fixcRegistered) {
        g_weatherExecution.activeFixc = AllocateWeatherFixc(14563);
        Weather_RegisterActiveFIXCAndInitEffects();
    }
}

// @verified - d
void UnregisterActiveWeatherFixc() {
    if (g_weatherExecution.activeFixc && g_weatherExecution.fixcRegistered) {
        UnregisterWeatherFixc(g_weatherExecution.activeFixc);
        g_weatherExecution.fixcRegistered = 0;
    }
}

// @verified - d
std::int32_t GetCurrentWeather() {
    DateTime time;
    shenGetCurrentDateTime(time);
    WeatherData *const data = GetWeatherExecutionData();
    UpdateWeatherTimelines(data, time.year, time.month, time.day, time.hour, time.minute);
    std::int32_t weather;
    if (GetWeatherRainIntensity(data) > 0.0f)
        weather = 2;
    else if (GetWeatherCloudIntensity(data) > 0.0f)
        weather = 1;
    else
        weather = 0;
    return weather;
}

// @verified - d
float GetMaximumWeatherRainForecast(std::int32_t minutes) {
    DateTime time, increment;
    increment.year = 0;
    increment.month = 0;
    increment.day = 0;
    increment.hour = 0;
    increment.second = 0;
    increment.minute = 10;
    IncrementDateTimeInPlace(time, increment);
    shenGetCurrentDateTime(time);
    WeatherData *const data = GetWeatherExecutionData();
    UpdateWeatherTimelines(data, time.year, time.month, time.day, time.hour, time.minute);
    float maximum = GetWeatherRainIntensity(data);
    const std::int32_t untilNext = 10 - time.minute % 10;
    increment.minute = static_cast<std::int8_t>(untilNext);
    IncrementDateTimeInPlace(time, increment);
    increment.minute = 10;
    const std::int32_t steps = (minutes - untilNext) / 10;
    for (std::int32_t index = 0; index <= steps; ++index) {
        UpdateWeatherTimelines(data, time.year, time.month, time.day, time.hour, time.minute);
        const float rain = GetWeatherRainIntensity(data);
        if (rain > maximum)
            maximum = rain;
        IncrementDateTimeInPlace(time, increment);
    }
    return maximum;
}

WeatherFixcOwner *FindWeatherFixcOfType(std::uint32_t type) {
    for (WeatherFixcNode *node = g_weatherExecution.weatherEffects; node; node = GetNextWeatherFixcNode(node))
        if (GetWeatherFixcType(static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node))) == type)
            return static_cast<WeatherFixcOwner *>(GetWeatherFixcNodeData(node));
    return 0;
}

// @verified - d
void LoadWeatherClipChunk(const MapInfoChunk *chunk) {
    if (chunk)
        ReadWeatherClipData(chunk + 1);
}

// @verified - d
void ReadWeatherClipData(const void *data) {
    const WeatherClipChunkHeader *const header = reinterpret_cast<const WeatherClipChunkHeader *>(data);
    const std::byte *entries = reinterpret_cast<const std::byte *>(header + 1);
    if (header->firstCount != 0) {
        Weather_LoadWCCPEntries(entries, header->firstCount, 0);
        entries += static_cast<std::uint32_t>(header->firstCount) * 56U;
    }
    if (header->secondCount != 0)
        Weather_LoadWCCPEntries(entries, header->secondCount, 1);
}

void AddWeatherInstructionFlags(std::uint32_t flags) {
    g_weatherExecution.instructionFlags |= flags;
}

void RemoveWeatherInstructionFlags(std::uint32_t flags) {
    g_weatherExecution.instructionFlags &= ~flags;
}

// @verified - d
void SetWeatherInstruction(std::uint32_t flags) {
    g_weatherExecution.instructionFlags = flags;
}

// @verified - d
std::uint32_t GetCurrentWeatherInstruction() {
    return g_weatherExecution.instructionFlags;
}

// @verified - d
void UpdateSunOrWindRotationVector() {
    float wind = GetWeatherWindVector();
    wind *= 0.0005f;
    float pressure = GetWeatherPressure();
    pressure *= 0.00099999993f;
    float speed = wind;
    if (pressure > speed)
        speed = pressure;
    g_weatherExecution.planetLayerOffset.x = speed + 0.0005f;
    g_weatherExecution.planetLayerOffset.y = 0.0f;
    DateTime date;
    shenGetCurrentDateTime(date);
    const float hour = date.hour * 10000.0f;
    const float minute = (date.minute / 60.0f) * 10000.0f;
    const float second = (date.second / 60.0f) * 100.0f * 1.666666f;
    const float time = hour + minute + second;
    const std::uint16_t angle = static_cast<std::uint16_t>(time * 0.0000083333334f * 65536.0f);
    const float x = g_weatherExecution.planetLayerOffset.x;
    const float y = g_weatherExecution.planetLayerOffset.y;
    const float sine = dc::sdk::SinFixed(angle);
    const float cosine = dc::sdk::CosFixed(angle);
    g_weatherExecution.planetLayerOffset.x = x * cosine - y * sine;
    g_weatherExecution.planetLayerOffset.y = x * sine + y * cosine;
    shenUpdatePlanetByLayer(g_weatherExecution.planetLayerOffset.x, 0, 0);
    shenUpdatePlanetByLayer(g_weatherExecution.planetLayerOffset.y, 1, 0);
    shenUpdatePlanetByLayer(g_weatherExecution.planetLayerOffset.x, 0, 1);
    shenUpdatePlanetByLayer(g_weatherExecution.planetLayerOffset.y, 1, 1);
}

// @verified - d
void ApplyPlanetRotForCurrentDateTime() {
    DateTime date;
    shenGetCurrentDateTime(date);
    const std::int32_t month = date.month;
    std::int32_t day = date.day;
    const std::int32_t hour = date.hour;
    if ((month & 1) == 0)
        day += 31;
    if (hour <= 11)
        --day;
    if (day < 0)
        day = 0;
    float elapsed;
    std::int32_t denominator;
    if (planetRotationDays[1] < day) {
        day -= planetRotationDays[1];
        elapsed = day;
        denominator = planetRotationDays[2] - planetRotationDays[1];
    } else if (planetRotationDays[0] < day) {
        day -= planetRotationDays[0];
        elapsed = day;
        denominator = planetRotationDays[1] - planetRotationDays[0];
    } else {
        elapsed = day;
        denominator = planetRotationDays[0];
    }
    elapsed /= denominator;
    Vector3i rotation;
    rotation.x = rotation.z = 0;
    rotation.y = static_cast<std::uint16_t>(elapsed * 65536.0f);
    SetPlanetRotByIndex(rotation, 0);
}

std::int32_t GetCurrentWindTargetPhase() {
    return GetWindTargetPhase(GetWind());
}

// @verified - d
float GetCurrentWindSpeed() {
    return GetCurrentWindSpeed(GetWind());
}

// @verified - d
void SetWeatherTableMode(std::int32_t mode) {
    const WeatherTable *source;
    WeatherTable *target;
    std::int32_t count;
    g_weatherExecution.tableMode = mode;
    if (mode == 0)
        source = weatherDefaultTables;
    else if (mode == 1)
        source = weatherDefaultTables;
    else
        source = g_sceneWeatherTables;
    target = g_weatherTables;
    count = 4;
    do {
        target->rows = source->rows;
        target->count = source->count;
        ++source;
        ++target;
    } while (--count);
}

std::int32_t GetWeatherTableMode() {
    return g_weatherExecution.tableMode;
}

const std::byte *GetWeatherTableRows(std::int32_t index) {
    return g_weatherTables[index].rows;
}

std::int32_t GetWeatherTableCount(std::int32_t index) {
    return g_weatherTables[index].count;
}

// @verified - d
struct WeatherConditionData {
    std::uint32_t words[sizeof(g_weatherConditions) / 4];
};

void ReadWeatherConditionData() {
    void *const data = shenLoadFile("misc", "W_COND.BIN", WEATHER_CONDITION_STORAGE);
    *reinterpret_cast<WeatherConditionData *>(g_weatherConditions) = *static_cast<WeatherConditionData *>(data);
    HLib::FreeTaskData(data);
}

// @verified - d
void WSCN_LoadWeatherData(const char *directory, const char *filename) {
    std::int32_t previousCount = 0;
    g_sceneWeatherData = static_cast<WeatherTableSample *>(shenLoadFile(directory, filename, WEATHER_SCENE_STORAGE));
    for (std::int32_t index = 0; index < 4; ++index) {
        g_sceneWeatherTables[index].rows = reinterpret_cast<const std::byte *>(g_sceneWeatherData + previousCount);
        previousCount = g_sceneWeatherTables[index].count;
    }
    SetWeatherTableMode(2);
}

// @verified - d
void WSCN_DeleteWeatherData() {
    SetWeatherTableMode(1);
    if (g_sceneWeatherData)
        HLib::FreeTaskData(g_sceneWeatherData);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void WSCN_SetWeatherShouldUpdateDay(std::int32_t enabled) {
    SetWeatherShouldUpdateDay(enabled);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EV_WeatherEffectUtil_Impl(std::uint32_t fov, float rainScale, float, float rippleScale) {
    Weather_FreeActiveFIXC();
    g_weatherExecution.activeFixc = AllocateWeatherFixc(static_cast<std::int32_t>(fov));
    RegisterWeatherFixc(g_weatherExecution.activeFixc);
    g_weatherExecution.fixcRegistered = 1;
    InitWeatherRainForAllFixc(rainScale);
    InitWeatherRippleForAllFixc(rippleScale);
}
#endif

} // namespace s2
} // namespace shenmue

#endif
