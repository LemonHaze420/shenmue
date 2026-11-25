#include "s2/weather_particles_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include <cstring>
#include "s2/weather_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/camera_dc.hpp"
#include <cmath>
#include "s2/weather_data_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/date_time.hpp"
#include "platform/dc/weather_texture.hpp"
#include "platform/dc/ninja.hpp"
#include "s2/character_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "shenmue.h"
#include "s2/weather_wind_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather_air_dc.hpp"
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
#include "s2/map_info.hpp"
#include "s2/air_dc.hpp"
#include "s2/scnw_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include <cfloat>
#include "s2/frame_dc.hpp"

namespace shenmue {
namespace s2 {

// SPHR sphere group; allocation size 0x22c.
struct WeatherSphereGroup {
    std::byte retained00[0xb0];
    float snowMotionDamping;
    std::byte retainedb4[0x14];
    float snowWidth;
    Vector3 snowDrift;
    Vector3 snowVariance;
    std::byte retainede4[0x68];
    float spawnVolume;
    Vector3 centre;
    std::byte retained15c[0x18];
    float radius, radiusSquared, distance;
    std::byte retained180[0x10];
    WeatherClipQuadPlanes clipA, clipB;
    Vector3 snowSpawnOrigin;
};

typedef char Layout_weather_sphere_group_0[(sizeof(WeatherSphereGroup) == 0x22c) ? 1 : -1];
typedef char Layout_weather_sphere_group_1[(offsetof(WeatherSphereGroup, spawnVolume) == 0x14c) ? 1 : -1];
typedef char Layout_weather_sphere_group_2[(offsetof(WeatherSphereGroup, radius) == 0x174) ? 1 : -1];
typedef char Layout_weather_sphere_group_3[(offsetof(WeatherSphereGroup, clipA) == 0x190) ? 1 : -1];

struct WeatherSnowParameters {
    float width;
    Vector3 drift, variance;
    std::uint32_t colour;
    float damping;
};

void SetWeatherDirectGroupSpawnVolume(WeatherParticleGroup *group, float spanX, float spanY, float spanZ);
void SetWeatherDirectGroupBounds(WeatherParticleGroup *group, const Vector3 *center);
void SetWeatherGroupSpawnVolume(WeatherParticleGroup *group, float spanX, float spanY, float spanZ);
void SetWeatherGroupBounds(WeatherParticleGroup *group, const Vector3 *center, float span);
HLib::Task *EnqueueWeatherParticleResource();
void AppendWeatherParticleGroup(WeatherParticleGroup *group);
void RemoveWeatherParticleGroup(WeatherParticleGroup *group);
void InsertWeatherParticleGroup(WeatherParticleGroup *previous, WeatherParticleGroup *group, WeatherParticleGroup *next);
void DetachWeatherParticleGroup(WeatherParticleGroup *group);
void LinkWeatherParticleGroups(WeatherParticleGroup *previous, WeatherParticleGroup *next);
void SplitWeatherParticleGroups(WeatherParticleGroup *previous, WeatherParticleGroup *next);
WeatherParticleGroup *GetLastWeatherParticleGroup(WeatherParticleGroup *group);
WeatherParticleGroup *GetNextWeatherParticleGroup(const WeatherParticleGroup *group);
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour);
float GetWeatherPressure();
void WAR_CallbackParticleResource_Callback(void *);
void SetRainGroupDrawingEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetRippleGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetRippleGroupDrawingEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetRainGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetWeatherRainSpawningEnabled(WeatherParticleGroup *group, std::int32_t enabled);
std::int32_t IsWeatherRainSpawningEnabled(const WeatherParticleGroup *group);
void InitializeWeatherRainDirect(WeatherParticleGroup *group);
void InitializeWeatherRippleMode0(WeatherParticleGroup *group);
void InitializeWeatherRippleMode1(WeatherParticleGroup *group);
void InitializeWeatherRippleDirect(WeatherParticleGroup *group);
void InitializeWeatherRainMode0(WeatherParticleGroup *group);
void InitializeWeatherRainMode1(WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Direct(WeatherRainParticle *particle);
void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Mode0(WeatherRainParticle *particle);
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle);

void SetRippleGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour);
float GetWeatherPressure();
void WAR_CallbackParticleResource_Callback(void *);
void SetRainGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetWeatherRainSpawningEnabled(WeatherParticleGroup *group, std::int32_t enabled);
std::int32_t IsWeatherRainSpawningEnabled(const WeatherParticleGroup *group);
void InitializeWeatherRainMode0(WeatherParticleGroup *group);
void InitializeWeatherRainMode1(WeatherParticleGroup *group);
void InitializeWeatherRainDirect(WeatherParticleGroup *group);
void InitializeWeatherRippleMode0(WeatherParticleGroup *group);
void InitializeWeatherRippleMode1(WeatherParticleGroup *group);
void InitializeWeatherRippleDirect(WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Direct(WeatherRainParticle *particle);
void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Mode0(WeatherRainParticle *particle);
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle);
void SetRainGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void InitializeWeatherRainMode0(WeatherParticleGroup *group);
void InitializeWeatherRainMode1(WeatherParticleGroup *group);
void InitializeWeatherRainDirect(WeatherParticleGroup *group);
void InitializeWeatherRippleMode0(WeatherParticleGroup *group);
void InitializeWeatherRippleMode1(WeatherParticleGroup *group);
void InitializeWeatherRippleDirect(WeatherParticleGroup *group);
void WAR_CallbackParticleResource_Callback(void *);
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour);
std::int32_t IsWeatherRainSpawningEnabled(const WeatherParticleGroup *group);
void SetWeatherRainSpawningEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void InitializeWeatherRainMode0(WeatherParticleGroup *group);
void InitializeWeatherRainMode1(WeatherParticleGroup *group);
void InitializeWeatherRainDirect(WeatherParticleGroup *group);
void InitializeWeatherRippleMode0(WeatherParticleGroup *group);
void InitializeWeatherRippleMode1(WeatherParticleGroup *group);
void InitializeWeatherRippleDirect(WeatherParticleGroup *group);
void AdjustWeatherDrawBudget(WeatherRainOwner *owner, std::int32_t mode);
void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Direct(WeatherRainParticle *particle);
void ResetWeatherRainParticle_Mode0(WeatherRainParticle *particle);
void ResetWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle);
static WeatherRainParameters weatherRainPressureHigh = {0.0035f, 0.15f, {0.0f, -0.28f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0x15b7b9bbU, 0.06f};
static WeatherRainParameters weatherRainPressureLow = {0.002f, 0.2f, {0.0f, -0.18f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0x15b7b9bbU, 0.06f};
static const WeatherSnowParameters weatherSnowPressureHigh = {0.08f, {0.0f, -0.095f, 0.0f}, {0.013f, 0.02f, 0.013f}, 0xe8cdc8d1U, 0.037f};
static const WeatherSnowParameters weatherSnowPressureLow = {0.05f, {0.0f, -0.05f, 0.0f}, {0.032f, 0.02f, 0.032f}, 0xe8cdc8d1U, 0.037f};
static const struct WeatherRippleDefaults {
    float timeStep;
    std::uint32_t colour;
} weatherRippleDefaults = {0.15f, 0xa0ffffffU};
#define WEATHER_SPHERE_STORAGE SHEN_NAME4('S', 'P', 'H', 'R')
void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);

// @verified - d
void GetDefaultWeatherRainParameters(float *first, float *second) {
    *first = weatherRainPressureHigh.first;
    *second = weatherRainPressureHigh.second;
}

void GetDefaultWeatherDrift(Vector3 *drift) {
    WeatherRainParameters *parameters = &weatherRainPressureHigh;
    *drift = parameters->drift;
}

// @verified - d
void GetDefaultWeatherDriftVariance(Vector3 *direction) {
    WeatherRainParameters *parameters = &weatherRainPressureHigh;
    *direction = parameters->variance;
}

// @verified - d
std::uint32_t GetDefaultWeatherRainColour() {
    return weatherRainPressureHigh.colour;
}

// @verified - d
float GetDefaultWeatherRippleTimeStep() {
    return weatherRippleDefaults.timeStep;
}

// @verified - d
std::uint32_t GetDefaultWeatherRippleColour() {
    return weatherRippleDefaults.colour;
}

// @verified - d
HLib::Task *WAR_CallbackParticleResource_Enqueue(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused) {
    weatherParticleUnusedParameter = unused;
    weatherRainSpawnLimit = rainSpawnLimit;
    weatherSecondaryRainParameter = secondaryRainParameter;
    weatherRippleSpawnLimit = rippleSpawnLimit;
    return EnqueueWeatherParticleResource();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float GetWeatherGroupSpawnVolume(const WeatherParticleGroup *group) {
    return group ? group->spawnVolume : 0.0f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherGroupRain(WeatherParticleGroup *group) {
    if (group && group->rainParticles) {
        FreeLFLT(group->rainCountLflt);
        HLib::FreeTaskData(group->rainParticles);
        group->rainParticles = 0;
        group->rainFlags = 0;
    }
}
#endif

// @verified - d
std::uint32_t GetRainGroupFlags(const WeatherParticleGroup *group) {
    if (!group)
        return 0;
    return group->rainFlags;
}

// @verified - d
void SetRainGroupColour(WeatherParticleGroup *group, std::uint32_t colour) {
    if (group)
        group->rainColour = colour;
}

// @verified - d
void SetRainGroupDrift(WeatherParticleGroup *group, const Vector3 *drift, const Vector3 *variance) {
    if (group) {
        *reinterpret_cast<Vector3 *>(&group->spawnDriftX) = *drift;
        group->driftVariance = *variance;
    }
}

// @verified - d
void SetRainGroupParameters(WeatherParticleGroup *group, float first, float second) {
    if (group) {
        group->rainPressureParam0 = first;
        group->rainPressureParam1 = second;
    }
}

void ApplyWeatherRainWind(WeatherParticleGroup *group, const Vector3 *wind) {
    if (!group)
        return;
    float x = group->rainMotion.x, z = group->rainMotion.z, damping = group->rainMotionDamping;
    if (std::fabs(x) < 1.1920928955078125e-07f)
        x = 0.0f;
    if (std::fabs(z) < 1.1920928955078125e-07f)
        z = 0.0f;
    if (std::fabs(damping) < 1.1920928955078125e-07f)
        damping = 0.0f;
    float decayX = x * damping;
    if (std::fabs(decayX) < 1.1920928955078125e-07f)
        decayX = 0.0f;
    x -= decayX;
    if (std::fabs(x) < 1.1920928955078125e-07f)
        x = 0.0f;
    damping *= z;
    if (std::fabs(damping) < 1.1920928955078125e-07f)
        damping = 0.0f;
    z -= damping;
    if (std::fabs(z) < 1.1920928955078125e-07f)
        z = 0.0f;
    float windX = wind->x + wind->x;
    if (std::fabs(windX) < 1.1920928955078125e-07f)
        windX = 0.0f;
    float windZ = wind->z + wind->z;
    if (std::fabs(windZ) < 1.1920928955078125e-07f)
        windZ = 0.0f;
    x += windX;
    if (std::fabs(x) < 1.1920928955078125e-07f)
        x = 0.0f;
    z += windZ;
    if (std::fabs(z) < 1.1920928955078125e-07f)
        z = 0.0f;
    group->rainMotion.x = x;
    group->rainMotion.z = z;
}

void SetRainGroupMotionDamping(WeatherParticleGroup *group, float damping) {
    group->rainMotionDamping = damping;
}

float GetRainGroupMotionDamping(WeatherParticleGroup *group) {
    return group->rainMotionDamping;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherGroupRipples(WeatherParticleGroup *group) {
    if (!group)
        return;
    if (group->rippleColourRamp)
        HLib::FreeTaskData(group->rippleColourRamp);
    if (group->rippleParticles) {
        FreeLFLT(group->rippleCountLflt);
        HLib::FreeTaskData(group->rippleParticles);
        if (group->rippleDrawState.texture)
            group->rippleDrawState.texture = 0;
        group->rippleParticles = 0;
        group->rippleFlags = 0;
    }
}
#endif

// @verified - d
std::uint32_t GetRippleGroupFlags(const WeatherParticleGroup *group) {
    return group ? group->rippleFlags : 0;
}

// @verified - d
void SetRippleGroupTimeStep(WeatherParticleGroup *group, float step) {
    if (group)
        group->rippleDeltaTime = step;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float GetRippleGroupTimeStep(const WeatherParticleGroup *group) {
    return group ? group->rippleDeltaTime : 0.0f;
}
#endif

// @verified - d
void UpdateWeatherRainOrientation(WeatherParticleGroup *group, const Vector3 *previous, const Vector3 *current) {
    Vector3 direction;
    Matrix4x4 camera;
    Vector3 transformed, axis, projected;
    group->rainMotion.y = (group->fallSpeed + current->y) - previous->y;
    direction.x = -group->spawnDriftX - group->rainMotion.x;
    direction.y = (current->y - previous->y) - group->rainMotion.y;
    direction.z = -group->spawnDriftZ - group->rainMotion.z;
    if (std::fabs(direction.x) < 1.1920928955078125e-07f)
        direction.x = 0.0f;
    if (std::fabs(direction.z) < 1.1920928955078125e-07f)
        direction.z = 0.0f;
    if (!(std::sqrt(direction.x * direction.x + direction.z * direction.z) < 1.0f))
        return;
    if (std::fabs(group->filteredRainMotion.x) < 1.1920928955078125e-07f)
        group->filteredRainMotion.x = 0.0f;
    if (std::fabs(group->filteredRainMotion.z) < 1.1920928955078125e-07f)
        group->filteredRainMotion.z = 0.0f;
    group->filteredRainMotion.x += (direction.x - group->filteredRainMotion.x) * 0.25f;
    group->filteredRainMotion.y = direction.y;
    group->filteredRainMotion.z += (direction.z - group->filteredRainMotion.z) * 0.25f;
    transformed = group->filteredRainMotion;
    camCtrlGetProjectionMatrix(&camera);
    matrix_stack::Matrix4x4_Push(&camera);
    Matrix4x4::Transform(transformed, projected);
    PopCurrentMatrix(1);
    axis.x = 1.0f;
    axis.y = 0.0f;
    axis.z = 0.0f;
    am2Vector3Cross(&axis, &projected, &axis);
    matrix_stack::InvertMatrix(&camera);
    matrix_stack::Matrix4x4_Push(&camera);
    Matrix4x4::Transform(axis, axis);
    PopCurrentMatrix(1);
    group->rainYaw = -static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(axis.y, axis.z));
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundX_Wrapper(-group->rainYaw);
    Matrix4x4::Transform(axis, axis);
    Matrix4x4::Transform(transformed, transformed);
    PopCurrentMatrix(1);
    group->rainPitch = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(axis.x, axis.z));
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-group->rainPitch);
    Matrix4x4::Transform(transformed, transformed);
    PopCurrentMatrix(1);
    group->rainRoll = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(transformed.y, transformed.x)) - 0x4000;
}

// @verified - d
void SetWeatherRippleCameraY(WeatherParticleGroup *group, float y) {
    group->cameraY = y;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float GetWeatherGroupBoundsMaxY(const WeatherParticleGroup *group) {
    return group ? group->boundsMaxY : 0.0f;
}

// @verified - d
float GetWeatherGroupBoundsMinY(const WeatherParticleGroup *group) {
    return group ? group->boundsMinY : 0.0f;
}
#endif

// @verified - d
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure) {
    if (!group || !group->rainParticles)
        return;
    const float scaledCount = static_cast<float>(group->rainCapacity) * pressure;
    const std::int32_t count = static_cast<std::int32_t>(scaledCount);
    group->rainVerticalSpan = group->boundsMaxY - group->boundsMinY;
    lfltSetImmediate(group->rainCountLflt, static_cast<float>(count));
    SetWeatherRainSpawningEnabled(group, 1);
}

// @verified - d
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames) {
    if (!group || !group->rainParticles)
        return;
    const float density = (static_cast<float>(group->rainCapacity) * pressure) / (group->boundsMaxY - group->boundsMinY);
    const float fall = -group->fallSpeed;
    const float amount = density * fall;
    const std::int32_t count = static_cast<std::int32_t>(amount);
    group->rainVerticalSpan = fall;
    BeginWeatherFloatTransition(group->rainCountLflt, static_cast<float>(count), frames);
    SetWeatherRainSpawningEnabled(group, 0);
}

float GetRainGroupCount(WeatherParticleGroup *group) {
    if (group && group->rainParticles)
        return GetLFLTCurrent(group->rainCountLflt);
    return 0.0f;
}

// @verified - d
void SetRippleGroupCountImmediate(WeatherParticleGroup *group, float pressure) {
    if (!group || !group->rippleParticles)
        return;
    const float amount = static_cast<float>(group->rippleCapacity) * pressure;
    const std::int32_t count = static_cast<std::int32_t>(amount);
    lfltSetImmediate(group->rippleCountLflt, static_cast<float>(count));
}

float GetRippleGroupCount(WeatherParticleGroup *group) {
    if (group && group->rippleParticles)
        return GetLFLTCurrent(group->rippleCountLflt);
    return 0.0f;
}

void FreeWeatherSphereParticleGroup(WeatherSphereGroup *group) {
    if (group) {
        RemoveWeatherParticleGroup(reinterpret_cast<WeatherParticleGroup *>(group));
        HLib::FreeTaskData(group);
    }
}

void WeatherSphereGroupNullsub() {
}

// @verified - d
float ComputeWeatherSphereRadius(float distance, std::int32_t fieldOfView) {
    distance *= dc::sdk::SinFixed(fieldOfView / 2);
    return ((distance * 5.0f) * 0.25f) * 1.2f;
}

void SetWeatherSphereRadius(WeatherSphereGroup *group, float radius, std::int32_t hemisphere) {
    if (!group)
        return;
    group->radius = radius;
    group->radiusSquared = radius * radius;
    if (!hemisphere)
        group->spawnVolume = ((3.14159274f * (radius * radius)) * radius) * 2.7f;
    else
        group->spawnVolume = ((group->radiusSquared * 3.14159274f) * radius) * 1.1f;
}

void SetWeatherSphereDistance(WeatherSphereGroup *group, float distance) {
    if (!group)
        return;
    if (distance == -1.0f) {
        const float span = (group->radius * 4.0f) / 5.0f;
        group->distance = span / dc::sdk::SinFixed(static_cast<std::int16_t>(camCtrlGetPerspective() / 2));
    } else {
        group->distance = distance;
    }
}

float GetWeatherSphereDistance(const WeatherSphereGroup *group) {
    return group ? group->distance : 0.0f;
}

void SetWeatherSphereCentre(WeatherSphereGroup *group, const Vector3 *centre) {
    group->centre.x = centre->x;
    group->centre.y = centre->y;
    group->centre.z = centre->z;
}

void GetWeatherSphereCentre(const WeatherSphereGroup *group, Vector3 *centre) {
    if (group) {
        centre->x = group->centre.x;
        centre->y = group->centre.y;
        centre->z = group->centre.z;
    }
}

float GetWeatherSphereRadius(const WeatherSphereGroup *group) {
    return group ? group->radius : 0.0f;
}

void ComputeWeatherSphereCentre(const Vector3 *position, const Vector3 *backwards, Vector3 *centre, float distance) {
    centre->x = backwards->x * distance + position->x;
    centre->y = backwards->y * distance + position->y;
    centre->z = backwards->z * distance + position->z;
}

WeatherSphereGroup *AllocateWeatherSphereGroup(const Vector3 *centre, std::int32_t hemisphere, float radius) {
    WeatherSphereGroup *const group = static_cast<WeatherSphereGroup *>(HLib::AllocateTaskData(0x22c, WEATHER_SPHERE_STORAGE));
    group->clipA.clipY1 = -100000.0f;
    group->clipB.clipY1 = -100000.0f;
    SetWeatherSphereRadius(group, radius, hemisphere);
    SetWeatherSphereCentre(group, centre);
    AppendWeatherParticleGroup(reinterpret_cast<WeatherParticleGroup *>(group));
    return group;
}

void FreeWeatherSphereGroup(WeatherSphereGroup *group) {
    if (group) {
        RemoveWeatherParticleGroup(reinterpret_cast<WeatherParticleGroup *>(group));
        HLib::FreeTaskData(group);
    }
}

void ApplyWeatherSnowPressureParameters(WeatherSphereGroup *group, float pressure) {
    float width;
    Vector3 drift;
    Vector3 variance;
    float damping;
    if (pressure < 0.5f) {
        width = weatherSnowPressureLow.width;
        drift.x = weatherSnowPressureLow.drift.x;
        drift.y = weatherSnowPressureLow.drift.y;
        drift.z = weatherSnowPressureLow.drift.z;
        variance.x = weatherSnowPressureLow.variance.x;
        variance.y = weatherSnowPressureLow.variance.y;
        variance.z = weatherSnowPressureLow.variance.z;
        damping = weatherSnowPressureLow.damping;
    } else {
        const float fraction = (pressure - 0.5f) + (pressure - 0.5f);
        float difference;
        difference = weatherSnowPressureHigh.width - weatherSnowPressureLow.width;
        width = weatherSnowPressureLow.width;
        width += difference * fraction;
        difference = weatherSnowPressureHigh.drift.x - weatherSnowPressureLow.drift.x;
        drift.x = weatherSnowPressureLow.drift.x;
        drift.x += difference * fraction;
        difference = weatherSnowPressureHigh.drift.y - weatherSnowPressureLow.drift.y;
        drift.y = weatherSnowPressureLow.drift.y;
        drift.y += difference * fraction;
        difference = weatherSnowPressureHigh.drift.z - weatherSnowPressureLow.drift.z;
        drift.z = weatherSnowPressureLow.drift.z;
        drift.z += difference * fraction;
        difference = weatherSnowPressureHigh.variance.x - weatherSnowPressureLow.variance.x;
        variance.x = weatherSnowPressureLow.variance.x;
        variance.x += difference * fraction;
        difference = weatherSnowPressureHigh.variance.y - weatherSnowPressureLow.variance.y;
        variance.y = weatherSnowPressureLow.variance.y;
        variance.y += difference * fraction;
        difference = weatherSnowPressureHigh.variance.z - weatherSnowPressureLow.variance.z;
        variance.z = weatherSnowPressureLow.variance.z;
        variance.z += difference * fraction;
        difference = weatherSnowPressureHigh.damping - weatherSnowPressureLow.damping;
        damping = weatherSnowPressureLow.damping;
        damping += difference * fraction;
    }
    group->snowWidth = width;
    group->snowDrift.x = drift.x;
    group->snowDrift.y = drift.y;
    group->snowDrift.z = drift.z;
    group->snowVariance.x = variance.x;
    group->snowVariance.y = variance.y;
    group->snowVariance.z = variance.z;
    group->snowMotionDamping = damping;
    group->snowSpawnOrigin.x = drift.x - variance.x * 0.5f;
    group->snowSpawnOrigin.y = drift.y - variance.y * 0.5f;
    group->snowSpawnOrigin.z = drift.z - variance.z * 0.5f;
}

// @verified - d
void ApplyWeatherPressureParameters(WeatherParticleGroup *group, float pressure, std::int32_t side) {
    float first, second;
    Vector3 drift;
    if (pressure < 0.5f) {
        first = weatherRainPressureLow.first;
        second = weatherRainPressureLow.second;
        drift.x = weatherRainPressureLow.drift.x;
        drift.y = weatherRainPressureLow.drift.y;
        drift.z = weatherRainPressureLow.drift.z;
    } else {
        const float fraction = (pressure - 0.5f) + (pressure - 0.5f);
        float difference;
        difference = weatherRainPressureHigh.first - weatherRainPressureLow.first;
        first = weatherRainPressureLow.first;
        first += difference * fraction;
        difference = weatherRainPressureHigh.second - weatherRainPressureLow.second;
        second = weatherRainPressureLow.second;
        second += difference * fraction;
        difference = weatherRainPressureHigh.drift.x - weatherRainPressureLow.drift.x;
        drift.x = weatherRainPressureLow.drift.x;
        drift.x += difference * fraction;
        difference = weatherRainPressureHigh.drift.y - weatherRainPressureLow.drift.y;
        drift.y = weatherRainPressureLow.drift.y;
        drift.y += difference * fraction;
        difference = weatherRainPressureHigh.drift.z - weatherRainPressureLow.drift.z;
        drift.z = weatherRainPressureLow.drift.z;
        drift.z += difference * fraction;
    }
    group->rainPressureParam0 = first;
    group->rainPressureParam1 = second;
    if (side == 1) {
        group->rainPressureParam0 *= 1.5f;
        group->rainPressureParam1 += group->rainPressureParam1;
    }
    group->spawnDriftX = drift.x;
    group->fallSpeed = drift.y;
    group->spawnDriftZ = drift.z;
}

// @verified - d
void SetWeatherRipplePressureParameters(float pressure) {
    if (pressure < 0.5f) {
        weatherRippleLifetimeStep = 0.025f;
        weatherRippleLifetimeScale = 0.5f;
    } else {
        float fraction = pressure - 0.5f;
        fraction += fraction;
        weatherRippleLifetimeStep = (0.5f + fraction * 0.5f) * 0.05f;
        weatherRippleLifetimeScale = 0.5f + fraction * 0.16659999f;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetWeatherSphereClipPlanesSet0(
    WeatherSphereGroup *group, const Vector2 *first, const Vector2 *second, const Vector2 *third, float clipY0, float clipY1, const Vector2 *fourth) {
    if (!group)
        return;
    group->clipA.x[0] = first->x;
    group->clipA.z[0] = first->y;
    group->clipA.x[1] = second->x;
    group->clipA.z[1] = second->y;
    group->clipA.x[2] = third->x;
    group->clipA.z[2] = third->y;
    group->clipA.x[3] = fourth->x;
    group->clipA.z[3] = fourth->y;
    group->clipA.clipY0 = clipY0;
    group->clipA.clipY1 = clipY1;
    group->clipA.edgeZ[1] = second->x - first->x;
    group->clipA.edgeZ[3] = third->x - second->x;
    group->clipA.edgeX[1] = fourth->x - third->x;
    group->clipA.edgeX[3] = first->x - fourth->x;
    group->clipA.edgeZ[0] = second->y - first->y;
    group->clipA.edgeZ[2] = third->y - second->y;
    group->clipA.edgeX[0] = fourth->y - third->y;
    group->clipA.edgeX[2] = first->y - fourth->y;
}

// @verified - d
void SetWeatherSphereClipPlanesSet1(
    WeatherSphereGroup *group, const Vector2 *first, const Vector2 *second, const Vector2 *third, float clipY0, float clipY1, const Vector2 *fourth) {
    if (!group)
        return;
    group->clipB.x[0] = first->x;
    group->clipB.z[0] = first->y;
    group->clipB.x[1] = second->x;
    group->clipB.z[1] = second->y;
    group->clipB.x[2] = third->x;
    group->clipB.z[2] = third->y;
    group->clipB.x[3] = fourth->x;
    group->clipB.z[3] = fourth->y;
    group->clipB.clipY0 = clipY0;
    group->clipB.clipY1 = clipY1;
    group->clipB.edgeZ[1] = second->x - first->x;
    group->clipB.edgeZ[3] = third->x - second->x;
    group->clipB.edgeX[1] = fourth->x - third->x;
    group->clipB.edgeX[3] = first->x - fourth->x;
    group->clipB.edgeZ[0] = second->y - first->y;
    group->clipB.edgeZ[2] = third->y - second->y;
    group->clipB.edgeX[0] = fourth->y - third->y;
    group->clipB.edgeX[2] = first->y - fourth->y;
}
#endif

WeatherParticleGroup *AllocateWeatherFixcDirectGroup(const Vector3 *center, float spanX, float spanY, float spanZ) {
    WeatherParticleGroup *const group = static_cast<WeatherParticleGroup *>(HLib::AllocateTaskData(0x174, WEATHER_GROUP_STORAGE));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(group), reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uint8_t *>(group) + 0x148));
    SetWeatherDirectGroupSpawnVolume(group, spanX, spanY, spanZ);
    SetWeatherDirectGroupBounds(group, center);
    AppendWeatherParticleGroup(group);
    return group;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherParticleGroup(WeatherParticleGroup *group) {
    if (group) {
        RemoveWeatherParticleGroup(group);
        FreeWeatherGroupRain(group);
        FreeWeatherGroupRipples(group);
        HLib::FreeTaskData(group);
    }
}
#endif

// @verified - d
void UpdateWeatherDirectGroup(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind) {
    if (group->rainParticles) {
        ApplyWeatherRainWind(group, wind);
        UpdateWeatherRainOrientation(group, target, backwards);
        const std::int32_t advancing = AdvanceWeatherFloat(group->rainCountLflt);
        const float count = GetLFLTCurrent(group->rainCountLflt);
        group->rainCurrentCount = static_cast<std::int32_t>(count);
        if (group->rainCurrentCount >= group->rainCapacity)
            group->rainCurrentCount = group->rainCapacity - 1;
        if (advancing == 0 && IsWeatherRainSpawningEnabled(group)) {
            WeatherRainParticle *particle = group->rainParticles;
            const std::int32_t requested = group->rainCurrentCount;
            SetWeatherRainSpawningEnabled(group, 0);
            group->rainLiveCount = 0;
            std::int32_t index = 0;
            while (group->rainCurrentCount > 0) {
                SpawnWeatherRainParticle_Direct(particle, group);
                ++particle;
                ++index;
            }
            for (; index < group->rainCapacity; ++index, ++particle)
                ResetWeatherRainParticle_Direct(particle);
            const float adjusted = (static_cast<float>(requested) * -group->fallSpeed) / (group->boundsMaxY - group->boundsMinY);
            group->rainCurrentCount = static_cast<std::int32_t>(adjusted);
            if (group->rainCurrentCount >= group->rainCapacity)
                group->rainCurrentCount = group->rainCapacity - 1;
            group->rainVerticalSpan = -group->fallSpeed;
            lfltSetImmediate(group->rainCountLflt, static_cast<float>(group->rainCurrentCount));
        }
        group->rainDrawCount = group->rainLiveCount != 0 || group->rainCurrentCount != 0 ? group->rainCapacity : group->rainCurrentCount;
    }
    if (group->rippleParticles) {
        SetWeatherRippleCameraY(group, position->y);
        AdvanceWeatherFloat(group->rippleCountLflt);
        const float count = GetLFLTCurrent(group->rippleCountLflt);
        group->rippleCurrentCount = static_cast<std::int32_t>(count);
        if (group->rippleCurrentCount >= group->rippleCapacity)
            group->rippleCurrentCount = group->rippleCapacity - 1;
        if (group->rippleCurrentCount == 0)
            group->rippleDrawCount = 0;
        else
            group->rippleDrawCount = group->rippleCapacity;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void WeatherDirectGroupNullsub() {
}
#endif

void SetWeatherDirectGroupSpawnVolume(WeatherParticleGroup *group, float spanX, float spanY, float spanZ) {
    if (group) {
        group->frustumDistance = spanX;
        group->frustumSpan = spanY;
        group->clipA.clipY1 = spanZ;
        group->spawnVolume = (spanX * spanZ) * spanY;
    }
}

void EnsureWeatherRainGroup(WeatherParticleGroup *group, float scale) {
    if (!group || group->rainParticles)
        return;
    if (scale > weatherRainSpawnLimit)
        scale = weatherRainSpawnLimit;
    group->rainCapacity = static_cast<std::int32_t>(group->spawnVolume * scale);
    if (group->rainCapacity == 0)
        return;
    if (group->rainCapacity > 1100)
        group->rainCapacity = 1100;
    group->rainParticles = static_cast<WeatherRainParticle *>(
        HLib::AllocateTaskData(static_cast<std::uint32_t>(group->rainCapacity) * sizeof(WeatherRainParticle), WEATHER_RAIN_PARTICLE_STORAGE));
    group->rainLiveCount = 0;
    group->rainDrawCount = 0;
    group->rainCountLflt = AllocateLFLT();
    group->rainCurrentCount = 0;
    SetRainGroupColour(group, weatherRainPressureHigh.colour);
    SetRainGroupParameters(group, weatherRainPressureHigh.first, weatherRainPressureHigh.second);
    SetRainGroupDrift(group, &weatherRainPressureHigh.drift, &weatherRainPressureHigh.variance);
    SetRainGroupCountImmediate(group, 0.0f);
    SetRainGroupMotionDamping(group, weatherRainPressureHigh.damping);
    InitializeWeatherRainDirect(group);
}

void EnsureWeatherRippleMode0(WeatherParticleGroup *group, float scale) {
    if (!group || group->rippleParticles)
        return;
    if (scale > weatherRippleSpawnLimit)
        scale = weatherRippleSpawnLimit;
    group->rippleCapacity = static_cast<std::int32_t>(group->spawnVolume * scale);
    if (group->rippleCapacity == 0)
        return;
    if (group->rippleCapacity > 600)
        group->rippleCapacity = 600;
    group->rippleParticles = static_cast<WeatherRippleParticle *>(
        HLib::AllocateTaskData(static_cast<std::uint32_t>(group->rippleCapacity) * sizeof(WeatherRippleParticle), WEATHER_RIPPLE_PARTICLE_STORAGE));
    group->rippleLiveCount = 0;
    group->rippleDrawCount = 0;
    group->rippleCountLflt = AllocateLFLT();
    group->rippleCurrentCount = 0;
    AllocateWeatherRippleColourRamp(group, weatherRippleDefaults.colour);
    SetRippleGroupTimeStep(group, weatherRippleDefaults.timeStep);
    SetRippleGroupCountImmediate(group, 0.0f);
    InitializeWeatherRippleMode0(group);
}

void SetWeatherDirectGroupBounds(WeatherParticleGroup *group, const Vector3 *center) {
    group->boundsMaxX = center->x + (&group->frustumDistance)[0] * 0.5f;
    group->boundsMinX = center->x - (&group->frustumDistance)[0] * 0.5f;
    group->boundsMaxY = center->y + (&group->frustumDistance)[1] * 0.5f;
    group->boundsMinY = center->y - (&group->frustumDistance)[1] * 0.5f;
    group->boundsMaxZ = center->z + (&group->frustumDistance)[2] * 0.5f;
    group->boundsMinZ = center->z - (&group->frustumDistance)[2] * 0.5f;
}

void GetWeatherDirectGroupCentre(const WeatherParticleGroup *group, Vector3 *center) {
    center->x = -(group->frustumDistance * 0.5f - group->boundsMaxX);
    center->y = -(group->frustumSpan * 0.5f - group->boundsMaxY);
    center->z = -(group->clipA.clipY1 * 0.5f - group->boundsMaxZ);
}

// @verified - d
WeatherParticleGroup *AllocateWeatherParticleGroup(const Vector3 *center, float spanX, float spanY, float spanZ) {
    WeatherParticleGroup *const group = static_cast<WeatherParticleGroup *>(HLib::AllocateTaskData(0x170, WEATHER_GROUP_STORAGE));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(group), reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uint8_t *>(group) + 0x16c));
    SetWeatherGroupSpawnVolume(group, spanX, spanY, spanZ);
    SetWeatherGroupBounds(group, center, spanX);
    AppendWeatherParticleGroup(group);
    return group;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainMode0Group(WeatherParticleGroup *group) {
    if (group) {
        RemoveWeatherParticleGroup(group);
        FreeWeatherGroupRain(group);
        FreeWeatherGroupRipples(group);
        HLib::FreeTaskData(group);
    }
}
#endif

// @verified - d
void PrepareWeatherRainMode0(WeatherParticleGroup *group, const Vector3 *, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind) {
    const std::int32_t advancing = AdvanceWeatherFloat(group->rainCountLflt);
    ApplyWeatherRainWind(group, wind);
    UpdateWeatherRainOrientation(group, target, backwards);
    const float count = GetLFLTCurrent(group->rainCountLflt);
    group->rainCurrentCount = static_cast<std::int32_t>(count);
    if (group->rainCurrentCount >= group->rainCapacity)
        group->rainCurrentCount = group->rainCapacity - 1;
    if (advancing == 0 && IsWeatherRainSpawningEnabled(group)) {
        WeatherRainParticle *particle = group->rainParticles;
        const std::int32_t requested = group->rainCurrentCount;
        SetWeatherRainSpawningEnabled(group, 0);
        group->rainLiveCount = 0;
        std::int32_t index = 0;
        while (group->rainCurrentCount > 0) {
            SpawnWeatherRainParticle_Mode0(particle, group);
            ++particle;
            ++index;
        }
        for (; index < group->rainCapacity; ++index, ++particle)
            ResetWeatherRainParticle_Mode0(particle);
        group->rainVerticalSpan = -group->fallSpeed;
        const float adjusted = (static_cast<float>(requested) * group->rainVerticalSpan) / group->frustumSpan;
        group->rainCurrentCount = static_cast<std::int32_t>(adjusted);
        if (group->rainCurrentCount >= group->rainCapacity)
            group->rainCurrentCount = group->rainCapacity - 1;
        lfltSetImmediate(group->rainCountLflt, static_cast<float>(group->rainCurrentCount));
    }
    group->rainDrawCount = group->rainLiveCount != 0 || group->rainCurrentCount != 0 ? group->rainCapacity : group->rainCurrentCount;
}

// @verified - d
void PrepareWeatherRippleMode0(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *, const Vector3 *, const Vector3 *) {
    SetWeatherRippleCameraY(group, position->y);
    AdvanceWeatherFloat(group->rippleCountLflt);
    const float count = GetLFLTCurrent(group->rippleCountLflt);
    group->rippleCurrentCount = static_cast<std::int32_t>(count);
    if (group->rippleCurrentCount >= group->rippleCapacity)
        group->rippleCurrentCount = group->rippleCapacity - 1;
    if (group->rippleCurrentCount == 0)
        group->rippleDrawCount = 0;
    else
        group->rippleDrawCount = group->rippleCapacity;
}

// @verified - d
float WeatherFrustumSpan(float distance, std::int32_t fieldOfView) {
    return (distance * dc::sdk::TanFixed(fieldOfView / 2)) * 2.0f;
}

// @verified - d
void SetWeatherGroupSpawnVolume(WeatherParticleGroup *group, float spanX, float spanY, float spanZ) {
    if (group)
        group->spawnVolume = (spanX * spanZ) * spanY;
}

// @verified - d
void SetWeatherGroupFrustumDistance(WeatherParticleGroup *group, float distance) {
    group->frustumDistance = distance;
}

void EnsureWeatherRainMode0(WeatherParticleGroup *group, float scale) {
    if (!group || group->rainParticles)
        return;
    if (scale > weatherRainSpawnLimit)
        scale = weatherRainSpawnLimit;
    group->rainCapacity = static_cast<std::int32_t>(group->spawnVolume * scale);
    if (group->rainCapacity == 0)
        return;
    if (group->rainCapacity > 1100)
        group->rainCapacity = 1100;
    group->rainParticles = static_cast<WeatherRainParticle *>(
        HLib::AllocateTaskData(static_cast<std::uint32_t>(group->rainCapacity) * sizeof(WeatherRainParticle), WEATHER_RAIN_PARTICLE_STORAGE));
    group->rainLiveCount = 0;
    group->rainDrawCount = 0;
    group->rainCountLflt = AllocateLFLT();
    group->rainCurrentCount = 0;
    SetRainGroupColour(group, weatherRainPressureHigh.colour);
    SetRainGroupParameters(group, weatherRainPressureHigh.first, weatherRainPressureHigh.second);
    SetRainGroupDrift(group, &weatherRainPressureHigh.drift, &weatherRainPressureHigh.variance);
    SetRainGroupCountImmediate(group, 0.0f);
    SetRainGroupMotionDamping(group, weatherRainPressureHigh.damping);
    InitializeWeatherRainMode0(group);
}

void EnsureWeatherRippleMode1(WeatherParticleGroup *group, float scale) {
    if (!group || group->rippleParticles)
        return;
    if (scale > weatherRippleSpawnLimit)
        scale = weatherRippleSpawnLimit;
    group->rippleCapacity = static_cast<std::int32_t>(group->spawnVolume * scale);
    if (group->rippleCapacity == 0)
        return;
    if (group->rippleCapacity > 600)
        group->rippleCapacity = 600;
    group->rippleParticles = static_cast<WeatherRippleParticle *>(
        HLib::AllocateTaskData(static_cast<std::uint32_t>(group->rippleCapacity) * sizeof(WeatherRippleParticle), WEATHER_RIPPLE_PARTICLE_STORAGE));
    group->rippleLiveCount = 0;
    group->rippleDrawCount = 0;
    group->rippleCountLflt = AllocateLFLT();
    group->rippleCurrentCount = 0;
    AllocateWeatherRippleColourRamp(group, weatherRippleDefaults.colour);
    SetRippleGroupTimeStep(group, weatherRippleDefaults.timeStep);
    SetRippleGroupCountImmediate(group, 0.0f);
    InitializeWeatherRippleMode1(group);
}

// @verified - d
void ComputeWeatherGroupCentre(const WeatherParticleGroup *group, const Vector3 *position, const Vector3 *backwards, Vector3 *centre) {
    centre->x = backwards->x * group->frustumDistance + position->x;
    centre->y = backwards->y * group->frustumDistance + position->y;
    centre->z = position->z + backwards->z * group->frustumDistance;
}

// @verified - d
void SetWeatherGroupBounds(WeatherParticleGroup *group, const Vector3 *center, float span) {
    const float half = span * 0.5f;
    group->boundsMaxX = center->x + half;
    group->boundsMinX = center->x - half;
    group->boundsMaxY = center->y + half;
    group->boundsMinY = center->y - half;
    group->boundsMaxZ = center->z + half;
    group->boundsMinZ = center->z - half;
}

// @verified - d
WeatherParticleGroup *AllocateWeatherClipParticleGroup(const Vector3 *center, float spanX, float spanY, float spanZ) {
    WeatherParticleGroup *const group = static_cast<WeatherParticleGroup *>(HLib::AllocateTaskData(0x200, WEATHER_GROUP_STORAGE));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(group), reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::uint8_t *>(group) + 0x1fc));
    group->clipA.clipY1 = -100000.0f;
    group->clipB.clipY1 = -100000.0f;
    SetWeatherGroupSpawnVolume(group, spanX, spanY, spanZ);
    SetWeatherGroupBounds(group, center, spanX);
    AppendWeatherParticleGroup(group);
    return group;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainMode1Group(WeatherParticleGroup *group) {
    if (group) {
        RemoveWeatherParticleGroup(group);
        FreeWeatherGroupRain(group);
        FreeWeatherGroupRipples(group);
        HLib::FreeTaskData(group);
    }
}
#endif

// @verified - d
void PrepareWeatherRainMode1(
    WeatherParticleGroup *group, const Vector3 *, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind, std::int32_t side) {
    const std::int32_t advancing = AdvanceWeatherFloat(group->rainCountLflt);
    float pressure = GetWeatherPressure();
    if (pressure > 1.0f)
        pressure = 1.0f;
    ApplyWeatherPressureParameters(group, pressure, side);
    ApplyWeatherRainWind(group, wind);
    UpdateWeatherRainOrientation(group, target, backwards);
    const float count = GetLFLTCurrent(group->rainCountLflt);
    group->rainCurrentCount = static_cast<std::int32_t>(count);
    if (group->rainCurrentCount >= group->rainCapacity)
        group->rainCurrentCount = group->rainCapacity - 1;
    if (advancing == 0 && IsWeatherRainSpawningEnabled(group)) {
        WeatherRainParticle *particle = group->rainParticles;
        const std::int32_t requested = group->rainCurrentCount;
        SetWeatherRainSpawningEnabled(group, 0);
        group->rainLiveCount = 0;
        std::int32_t index = 0;
        while (group->rainCurrentCount > 0) {
            SpawnWeatherRainParticle_Mode1WCCP(particle, group);
            ++particle;
            ++index;
        }
        for (; index < group->rainCapacity; ++index, ++particle)
            ResetWeatherRainParticle_Mode1WCCP(particle);
        group->rainVerticalSpan = -group->fallSpeed;
        const float adjusted = (static_cast<float>(requested) * group->rainVerticalSpan) / group->frustumSpan;
        group->rainCurrentCount = static_cast<std::int32_t>(adjusted);
        if (group->rainCurrentCount >= group->rainCapacity)
            group->rainCurrentCount = group->rainCapacity - 1;
        lfltSetImmediate(group->rainCountLflt, static_cast<float>(group->rainCurrentCount));
    }
    group->rainDrawCount = group->rainLiveCount != 0 || group->rainCurrentCount != 0 ? group->rainCapacity : group->rainCurrentCount;
}

// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PrepareWeatherRippleMode1(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *, const Vector3 *, const Vector3 *) {
#else
void PrepareWeatherRippleMode1(WeatherParticleGroup *group, const Vector3 *position) {
#endif
    SetWeatherRipplePressureParameters(GetWeatherPressure());
    SetWeatherRippleCameraY(group, position->y);
    AdvanceWeatherFloat(group->rippleCountLflt);
    const float count = GetLFLTCurrent(group->rippleCountLflt);
    group->rippleCurrentCount = static_cast<std::int32_t>(count);
    if (group->rippleCurrentCount >= group->rippleCapacity)
        group->rippleCurrentCount = group->rippleCapacity - 1;
    if (group->rippleCurrentCount == 0)
        group->rippleDrawCount = 0;
    else
        group->rippleDrawCount = group->rippleCapacity;
}

void EnsureWeatherRainMode1(WeatherParticleGroup *group, float scale) {
    if (!group || group->rainParticles)
        return;
    if (scale > weatherRainSpawnLimit)
        scale = weatherRainSpawnLimit;
    group->rainCapacity = static_cast<std::int32_t>(group->spawnVolume * scale);
    if (group->rainCapacity == 0)
        return;
    if (group->rainCapacity > 1100)
        group->rainCapacity = 1100;
    group->rainParticles = static_cast<WeatherRainParticle *>(
        HLib::AllocateTaskData(static_cast<std::uint32_t>(group->rainCapacity) * sizeof(WeatherRainParticle), WEATHER_RAIN_PARTICLE_STORAGE));
    group->rainLiveCount = 0;
    group->rainDrawCount = 0;
    group->rainCountLflt = AllocateLFLT();
    group->rainCurrentCount = 0;
    SetRainGroupColour(group, weatherRainPressureHigh.colour);
    SetRainGroupParameters(group, weatherRainPressureHigh.first, weatherRainPressureHigh.second);
    SetRainGroupDrift(group, &weatherRainPressureHigh.drift, &weatherRainPressureHigh.variance);
    SetRainGroupCountImmediate(group, 0.0f);
    SetRainGroupMotionDamping(group, weatherRainPressureHigh.damping);
    InitializeWeatherRainMode1(group);
}

void EnsureWeatherRippleGroup(WeatherParticleGroup *group, float scale) {
    if (!group || group->rippleParticles)
        return;
    if (scale > weatherRippleSpawnLimit)
        scale = weatherRippleSpawnLimit;
    group->rippleCapacity = static_cast<std::int32_t>(group->spawnVolume * scale);
    if (group->rippleCapacity == 0)
        return;
    if (group->rippleCapacity > 600)
        group->rippleCapacity = 600;
    group->rippleParticles = static_cast<WeatherRippleParticle *>(
        HLib::AllocateTaskData(static_cast<std::uint32_t>(group->rippleCapacity) * sizeof(WeatherRippleParticle), WEATHER_RIPPLE_PARTICLE_STORAGE));
    group->rippleLiveCount = 0;
    group->rippleDrawCount = 0;
    group->rippleCountLflt = AllocateLFLT();
    group->rippleCurrentCount = 0;
    AllocateWeatherRippleColourRamp(group, weatherRippleDefaults.colour);
    SetRippleGroupTimeStep(group, weatherRippleDefaults.timeStep);
    SetRippleGroupCountImmediate(group, 0.0f);
    InitializeWeatherRippleDirect(group);
}

void SetWeatherClipPlanesSet0(
    WeatherParticleGroup *group, const Vector2 *first, const Vector2 *second, const Vector2 *third, float clipY0, float clipY1, const Vector2 *fourth) {
    if (!group)
        return;
    group->clipA.x[0] = first->x;
    group->clipA.z[0] = first->y;
    group->clipA.x[1] = second->x;
    group->clipA.z[1] = second->y;
    group->clipA.x[2] = third->x;
    group->clipA.z[2] = third->y;
    group->clipA.x[3] = fourth->x;
    group->clipA.z[3] = fourth->y;
    group->clipA.clipY0 = clipY0;
    group->clipA.clipY1 = clipY1;
    group->clipA.edgeZ[1] = second->x - first->x;
    group->clipA.edgeZ[3] = third->x - second->x;
    group->clipA.edgeX[1] = fourth->x - third->x;
    group->clipA.edgeX[3] = first->x - fourth->x;
    group->clipA.edgeZ[0] = second->y - first->y;
    group->clipA.edgeZ[2] = third->y - second->y;
    group->clipA.edgeX[0] = fourth->y - third->y;
    group->clipA.edgeX[2] = first->y - fourth->y;
}

// @verified - d
HLib::Task *EnqueueWeatherParticleResource() {
    weatherWarState = 0;
    g_currentWeatherForceFlag = 0;
    weatherGroupList = 0;
    weatherParticleResourceTask = HLib::EnqueueTaskWithoutParameter(WAR_CallbackParticleResource_Callback, 14, 8, WEATHER_RESOURCE_TASK);
    return weatherParticleResourceTask;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DestroyWeatherParticleResource() {
    HLib::DestroyTaskInternal(weatherParticleResourceTask);
}
#endif

void AppendWeatherParticleGroup(WeatherParticleGroup *group) {
    InsertWeatherParticleGroup(GetLastWeatherParticleGroup(weatherGroupList), group, 0);
    if (!weatherGroupList)
        weatherGroupList = group;
    ++weatherWarState;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RemoveWeatherParticleGroup(WeatherParticleGroup *group) {
    if (!group->previous)
        weatherGroupList = group->next;
    DetachWeatherParticleGroup(group);
    --weatherWarState;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetRainAndRippleFlagsTo(std::int32_t state) {
    WeatherParticleGroup *group = weatherGroupList;
    if (state) {
        g_currentWeatherForceFlag = 1;
        state = 0;
    } else {
        g_currentWeatherForceFlag = 0;
        state = 1;
    }
    for (; group; group = GetNextWeatherParticleGroup(group)) {
        SetRainGroupUpdatesEnabled(group, state);
        SetRippleGroupUpdatesEnabled(group, state);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetWeatherForceFlag() {
    return g_currentWeatherForceFlag;
}
#endif

void SetRainAndRippleDrawingTo(std::int32_t enabled) {
    for (WeatherParticleGroup *group = weatherGroupList; group; group = GetNextWeatherParticleGroup(group)) {
        SetRainGroupDrawingEnabled(group, enabled);
        SetRippleGroupDrawingEnabled(group, enabled);
    }
}

void InsertWeatherParticleGroup(WeatherParticleGroup *previous, WeatherParticleGroup *group, WeatherParticleGroup *next) {
    LinkWeatherParticleGroups(previous, group);
    LinkWeatherParticleGroups(group, next);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DetachWeatherParticleGroup(WeatherParticleGroup *group) {
    WeatherParticleGroup *const previous = group->previous;
    WeatherParticleGroup *const next = group->next;
    SplitWeatherParticleGroups(previous, group);
    SplitWeatherParticleGroups(group, next);
    LinkWeatherParticleGroups(previous, next);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void LinkWeatherParticleGroups(WeatherParticleGroup *previous, WeatherParticleGroup *next) {
    if (previous)
        previous->next = next;
    if (next)
        next->previous = previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SplitWeatherParticleGroups(WeatherParticleGroup *previous, WeatherParticleGroup *next) {
    if (previous)
        previous->next = 0;
    if (next)
        next->previous = 0;
}
#endif

WeatherParticleGroup *GetLastWeatherParticleGroup(WeatherParticleGroup *group) {
    for (; group; group = group->next)
        if (!group->next)
            return group;
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
WeatherParticleGroup *GetNextWeatherParticleGroup(const WeatherParticleGroup *group) {
    if (!group)
        return 0;
    return group->next;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
