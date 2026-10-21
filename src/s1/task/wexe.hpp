#pragma once

#include "s1/hlib.hpp"
#include <cstddef>
#include <cstdint>
#include "common/math.hpp"

namespace shenmue::s1 {
struct ScenarioContext;

#if SHENMUE_IS_PC
inline constexpr std::uint32_t kWeatherFcasInterpolation[2] = {0x00010800U, 0x00fcfdfbU};
#endif
struct WeatherDataEntry {
    float intensity;
    float targetIntensity;
    float incrementStep;
    std::int32_t currentStep;
    std::int32_t maxStepCount;
};

static_assert(sizeof(WeatherDataEntry) == 0x14);
static_assert(offsetof(WeatherDataEntry, incrementStep) == 0x08);
static_assert(offsetof(WeatherDataEntry, maxStepCount) == 0x10);



extern std::int32_t g_weatherMode;
extern std::int32_t g_wdatFileSize;
extern void *g_wdatRoot;
extern void *g_cloudNode;
extern void *g_rainNode;
extern void *g_snowNode;
extern std::int32_t g_cloudCount;
extern std::int32_t g_rainCount;
extern std::int32_t g_snowCount;
extern void *g_forecastNode;
extern std::int32_t g_forecastCount;
extern std::int32_t g_weatherForecastValue;
extern std::int32_t g_disableWeather;
extern std::int32_t g_shouldReadWeatherForDateTime;
extern std::uint32_t g_weatherPreviousOperand;
extern HLib::Task *g_weatherExecuteTask;
extern WeatherDataEntry *g_cloudGameData;
extern WeatherDataEntry *g_fcasGameData;
extern WeatherDataEntry *g_activeRainEntry;
extern WeatherDataEntry *g_activeSnowEntry;
extern WeatherDataEntry *g_activeWindEntry;
extern float g_rainSpawnScale;
extern float g_rainVisualBlend;
extern std::int32_t g_activeWeatherOverrides;
extern std::int32_t g_pendingWeatherOverrides;
extern std::int32_t g_pendingWeatherClearOverrides;
extern std::int32_t g_allowWeatherOverrideValues;
extern float g_overrideCloudIntensity;
extern float g_overrideRainIntensity;
extern float g_overrideSnowIntensity;


struct WeatherCameraState {
    Vector3 previousEye;
    Vector3 previousTarget;
    Vector3 previousDirection;
    Vector3 eye;
    Vector3 target;
    Vector3 direction;
    std::uint32_t pad48;
    std::uint32_t pad4C;
};
static_assert(sizeof(WeatherCameraState) == 0x50);
static_assert(offsetof(WeatherCameraState, eye) == 0x24);
static_assert(offsetof(WeatherCameraState, direction) == 0x3c);


extern WeatherCameraState *g_weatherCameraState;
WeatherCameraState *ICAR();
extern std::uint32_t g_weatherWccpClipSet0;
extern std::uint32_t g_weatherWccpClipSet1;
void WEXE_UpdateWeatherCameraPosition();
bool WEXE_LoadWccpClipSet(std::uint32_t setIndex, const void *sourceRecords, std::uint32_t count);
void WEXE_ReleaseWccpClipSet(std::uint32_t setIndex);
void WEXE_SetWccpClipSetEnabled(std::uint32_t setIndex, std::uint32_t value);
void WEXE_SetWccpClipRecordActive(std::uint32_t setIndex, std::int32_t recordIndex, std::uint32_t value);
void WEXE_SetWccpClipSet0(std::uint32_t value);
void WEXE_SetWccpClipSet1(std::uint32_t value);
void Weather_ResetWCCPClipSet0();
void Weather_ResetWCCPClipSet1();

struct WeatherOverrideInterpolation {
    std::int32_t stepsRemain;
    float curr;
    float step;
    float target;
};

extern WeatherOverrideInterpolation g_cloudOverrideInterpolation;
extern WeatherOverrideInterpolation g_rainOverrideInterpolation;
extern WeatherOverrideInterpolation g_snowOverrideInterpolation;

constexpr std::int32_t kWeatherOverrideCloud = 1;
constexpr std::int32_t kWeatherOverrideRain = 2;
constexpr std::int32_t kWeatherOverrideSnow = 4;
constexpr std::int32_t kWeatherOverrideDual = 8;

void WEXE_ReadDataForDateTime(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute);
void WEXE_SetInterpolatedWeatherOverride(std::int32_t weatherType, std::int32_t allowOverrides, std::int32_t stepsRemain, float targetIntensity);
void WEXE_WeatherTaskUtil(std::uint32_t mode, std::uint32_t operand, std::uint32_t value);
bool WEXE_WeatherDataUtil(std::uint32_t mode, std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute,
    std::int32_t intValue, std::uint32_t &result);
void TaskUtil_InitStaticSize(std::int64_t taskData);

extern float g_fogR;
extern float g_fogG;
extern float g_fogB;
extern std::uint32_t g_fogColour;
extern std::int32_t g_bFogEnabled;
extern std::uint64_t g_cloudXY[2];
extern float g_fogRenderNearDistance;
extern float g_fogRenderFarDistance;
extern float g_fogMaximumDistance;
extern std::uint32_t g_lastSetFogColour;
extern Vector3 g_normalizedFogColour;

void Weather_SetWeatherDataByMode(std::int32_t mode);
HLib::Task *WEXE_Init();
void WEXE_Destroy();
void Weather_Tick(HLib::TaskData *taskData);
std::int64_t Weather_AdvanceWeatherEntry(WeatherDataEntry *entry);
void Weather_ApplyWeatherEntry(WeatherDataEntry *entry, float intensity, std::int32_t stepCount);
void WEXE_ApplyCloudGameData(float intensity, std::int32_t stepCount);
void WEXE_ApplyRainGameData(float intensity, std::int32_t stepCount);
void WEXE_ApplySnowGameData(float intensity, std::int32_t stepCount);
void WEXE_ApplyWindGameData(float intensity, std::int32_t stepCount);
void WEXE_ApplyFcasGameData(float intensity, std::int32_t stepCount);
float WEXE_GetCloudIntensity();
float WEXE_GetRainIntensity();
float WEXE_GetSnowIntensity();
float WEXE_GetWindIntensity();
std::int32_t Weather_GetCurrentWeatherEffectMode();
void Fog_DoFogEffectsTask(std::int64_t taskData);
void Script_ProcessFogFromTable(ScenarioContext *context, void *rawParams);
void AllocateWEDT();
void Weather_ReadChunks(void *fileData);
std::uint8_t GetFCAS();

void Fog_ApplySpecialSettings();
} // namespace shenmue::s1
