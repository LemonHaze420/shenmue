#include "s2/weather_air_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/fog_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/weather_dc.hpp"
#include <cmath>
#include <cstring>

namespace shenmue {
namespace s2 {

WeatherTableSample g_interpolatedWeatherSample = {};
void EV_SetPlanetLayerColours_Impl(std::uint32_t first, std::uint32_t second, std::int32_t layer);
void SetCloudAndSkyColour(std::uint32_t first, std::uint32_t second);
void SetReservedWeatherOverride0(float value);
void SetReservedWeatherOverride1(float value);

// @verified - d
void BuildWeatherSunDirection(
    std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute, float latitude, std::int32_t second, Vector3 *direction) {
    const std::int32_t elapsedDays = day + weatherAccumulatedMonthDays[month - 1] - 21;
    const std::int32_t days = weatherAccumulatedMonthDays[5] - elapsedDays;
    const float seasonalAngle = (((static_cast<float>(days) / 365.0f) * 360.0f) / 180.0f) * 3.14f;
    float time = static_cast<float>(hour) + static_cast<float>(minute) / 60.0f;
    time += static_cast<float>(second) / 3600.0f;
    float sunAngle = time - 12.0f;
    sunAngle = -sunAngle;
    sunAngle *= 15.0f;
    sunAngle *= 3.14f;
    sunAngle /= 180.0f;
    const float seasonal = dc::sdk::CosRadians(seasonalAngle);
    const float daily = dc::sdk::CosRadians(sunAngle);
    float elevation = std::fabs(((90.0f - (latitude - 23.5f * seasonal)) * daily) * 1.2f);
    if (elevation < 25.0f)
        elevation = 25.0f;
    elevation = (elevation * 3.14f) / 180.0f;
    direction->y = 0.0f;
    direction->x = 0.0f;
    direction->z = -1.0f;
    matrix_stack::PushUnitMatrix();
    matrix_stack::RotateYRadians(sunAngle);
    matrix_stack::RotateXRadians(-elevation);
    Matrix4x4::Transform(*direction, *direction);
    PopCurrentMatrix(1);
}

// @verified - d
void UpdateFog(WeatherAir &air) {
    WeatherTableSample sample;
    EvaluateWeatherTable(air.tables, air.tableCount, air.seconds, air.cloud, air.rain, air.snow, &sample);
    g_interpolatedWeatherSample = sample;
    WeatherSamplePrimary &primary = sample.primary;
    const std::uint32_t instruction = GetCurrentWeatherInstruction();
    Vector3 direction;
    Vector4 colour;
    if (instruction & 1U) {
        BuildWeatherSunDirection(air.month, air.day, air.hour, air.minute, 22.18f, air.second, &direction);
        if (std::fabs(direction.x) < 1.1920928955078125e-07f)
            direction.x = 0.0f;
        if (std::fabs(direction.y) < 1.1920928955078125e-07f)
            direction.y = 0.0f;
        if (std::fabs(direction.z) < 1.1920928955078125e-07f)
            direction.z = 0.0f;
        const std::int32_t angle = GetDirtAngle();
        if (angle) {
            const float x = direction.x, z = direction.z;
            Vector2 rotation;
            CalculateFixedSinCos(0U - static_cast<std::uint32_t>(angle), &rotation);
            float first = x * rotation.y, second = z * rotation.x;
            if (std::fabs(first) < 1.1920928955078125e-07f)
                first = 0.0f;
            if (std::fabs(second) < 1.1920928955078125e-07f)
                second = 0.0f;
            direction.x = first - second;
            if (std::fabs(direction.x) < 1.1920928955078125e-07f)
                direction.x = 0.0f;
            first = x * rotation.x;
            second = z * rotation.y;
            if (std::fabs(first) < 1.1920928955078125e-07f)
                first = 0.0f;
            if (std::fabs(second) < 1.1920928955078125e-07f)
                second = 0.0f;
            direction.z = first + second;
            if (std::fabs(direction.z) < 1.1920928955078125e-07f)
                direction.z = 0.0f;
        }
        SetLightSunDirection(air.lightIndex, direction);
    }
    if (instruction & 2U)
        SetLightRuntimeColour(air.lightIndex, *reinterpret_cast<const Vector4 *>(&primary.values[3]));
    SetReservedWeatherOverride0(primary.values[7]);
    if (instruction & 4U) {
        colour.x = colour.y = colour.z = colour.w = primary.values[7];
        UpdateGlobalLighting(colour);
    }
    SetReservedWeatherOverride1(primary.values[8]);
    if (instruction & 8U)
        SetLightSceneIntensity(air.lightIndex, primary.values[8]);
    if (instruction & 0x10U)
        lrgbSetImmediate(air.primaryColours[0], UnpackWeatherColour(primary.colours[0], &colour));
    if (instruction & 0x20U)
        lrgbSetImmediate(air.primaryColours[1], UnpackWeatherColour(primary.colours[1], &colour));
    if (instruction & 0x40U)
        lrgbSetImmediate(air.primaryColours[2], UnpackWeatherColour(primary.colours[2], &colour));
    if (instruction & 0x80U)
        SetFogRed(sample.environment.values[0]);
    if (instruction & 0x100U)
        SetFogGreen(sample.environment.values[1]);
    if (instruction & 0x200U)
        SetFogBlue(sample.environment.values[2]);
    if (instruction & 0x400U)
        SetFogDistance(sample.environment.colours[0] & 0xffffffU);
    if (instruction & 0x780U)
        ReinitFogTasks(1);
    if (instruction & 0x1800U) {
        const std::uint32_t first = sample.environment.colours[1] | 0xff000000U;
        std::uint32_t second = sample.environment.colours[2] | 0xff000000U;
        EV_SetPlanetLayerColours_Impl(first, second, 0);
        EV_SetPlanetLayerColours_Impl(first, second, 1);
        SetCloudAndSkyColour(sample.environment.colours[3] | 0xff000000U, second = sample.environment.colours[4] | 0xff000000U);
    }
}

} // namespace s2
} // namespace shenmue

#endif
