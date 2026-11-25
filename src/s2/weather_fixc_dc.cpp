#include "s2/weather_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/weather_particles_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/date_time.hpp"
#include "s2/weather_data_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include <cstring>
#include "s2/camera_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "shenmue.h"
#include "s2/weather_wind_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather_air_dc.hpp"
#include "s2/map_control_dc.hpp"
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
#include "s2/map_info.hpp"
#include "s2/frame_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/scnw_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "platform/dc/ninja.hpp"
#include "s2/character_dc.hpp"
#include "platform/dc/matrix.hpp"

namespace shenmue {
namespace s2 {

WeatherRainOwner *AllocateWeatherRainOwner(std::int32_t fov);
void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);
void FreeWeatherRainOwnerMode0(WeatherRainOwner *owner);
void UpdateWeatherRainOwnerMode0(
    WeatherRainOwner *owner, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *previousBackwards, const Vector3 *wind);
void EnsureWeatherRainOwnerMode0(WeatherRainOwner *owner, float scale);
void FreeWeatherRainOwnerDirectRain(WeatherRainOwner *owner);
void FreeWeatherRainOwnerRainMode0(WeatherRainOwner *owner);
void FreeWeatherRainOwnerRipplesMode0(WeatherRainOwner *owner);
std::uint32_t GetRainOwnerFlags(const WeatherRainOwner *owner);
void SetRainOwnerCountImmediate(WeatherRainOwner *owner, float pressure);
void BeginRainOwnerCountTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames);
void SetRainOwnerColour(WeatherRainOwner *owner, std::uint32_t colour);
void SetRainOwnerDrift(WeatherRainOwner *owner, const Vector3 *drift, const Vector3 *variance);
void FreeWeatherRainOwnerMode1(WeatherRainOwner *owner);
void UpdateWeatherRainOwnerMode1(
    WeatherRainOwner *owner, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *previousBackwards, const Vector3 *wind);
void EnsureWeatherRainOwnerMode1(WeatherRainOwner *owner, float scale);
void FreeWeatherRainOwnerRainMode1(WeatherRainOwner *owner);
void EnsureWeatherRippleOwnerMode1(WeatherRainOwner *owner, float scale);
void FreeWeatherRainOwnerRipplesMode1(WeatherRainOwner *owner);
void SetWeatherClipPlanesSet0(
    WeatherParticleGroup *group, const Vector2 *first, const Vector2 *second, const Vector2 *third, float clipY0, float clipY1, const Vector2 *fourth);
void ApplyWeatherRainOwnerClipMode0(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);
void ApplyWeatherRippleOwnerClipMode0(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
float GetRainOwnerCount(WeatherRainOwner *owner);
float ReleaseRainOwnerRippleGroup(WeatherRainOwner *owner);
void SetWeatherClipPlanes(
    WeatherParticleGroup *group, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1);
#endif
bool WeatherFixcHasWind(const WeatherFixcOwner *owner, WeatherWind *wind);
WeatherInteractionCamera *AllocateWeatherInteractionCamera();
void AdvanceWeatherInteractionCamera(WeatherInteractionCamera *camera);
void FreeWeatherInteractionCamera(WeatherInteractionCamera *camera);
WeatherParticleGroup *AllocateWeatherFixcDirectGroup(const Vector3 *center, float spanX, float spanY, float spanZ);
void SetRainOwnerParameters(WeatherRainOwner *owner, float first, float second);

void BeginRippleGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour);
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour);
HLib::Task *WAR_CallbackParticleResource_Enqueue(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused);
std::uint32_t GetRainGroupFlags(const WeatherParticleGroup *group);
void SetRainGroupColour(WeatherParticleGroup *group, std::uint32_t colour);
void SetRainGroupDrift(WeatherParticleGroup *group, const Vector3 *drift, const Vector3 *variance);
void SetRainGroupParameters(WeatherParticleGroup *group, float first, float second);
void SetRainGroupMotionDamping(WeatherParticleGroup *group, float damping);
float GetRainGroupMotionDamping(WeatherParticleGroup *group);
std::uint32_t GetRippleGroupFlags(const WeatherParticleGroup *group);
void SetRippleGroupTimeStep(WeatherParticleGroup *group, float step);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void SetRippleGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void FreeWeatherParticleGroup(WeatherParticleGroup *group);
void EnsureWeatherRainGroup(WeatherParticleGroup *group, float scale);
void EnsureWeatherRippleMode0(WeatherParticleGroup *group, float scale);
WeatherParticleGroup *AllocateWeatherParticleGroup(const Vector3 *center, float spanX, float spanY, float spanZ);
void FreeWeatherRainMode0Group(WeatherParticleGroup *group);
void PrepareWeatherRainMode0(WeatherParticleGroup *group, const Vector3 *, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind);
void PrepareWeatherRippleMode0(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *, const Vector3 *, const Vector3 *);
float WeatherFrustumSpan(float distance, std::int32_t fieldOfView);
std::int32_t camCtrlGetPerspectiveWrapper();
void SetWeatherGroupFrustumDistance(WeatherParticleGroup *group, float distance);
void EnsureWeatherRainMode0(WeatherParticleGroup *group, float scale);
void EnsureWeatherRippleMode1(WeatherParticleGroup *group, float scale);
void ComputeWeatherGroupCentre(const WeatherParticleGroup *group, const Vector3 *position, const Vector3 *backwards, Vector3 *centre);
void SetWeatherGroupBounds(WeatherParticleGroup *group, const Vector3 *center, float span);
WeatherParticleGroup *AllocateWeatherClipParticleGroup(const Vector3 *center, float spanX, float spanY, float spanZ);
void FreeWeatherRainMode1Group(WeatherParticleGroup *group);

void PrepareWeatherRainMode1(
    WeatherParticleGroup *group, const Vector3 *, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind, std::int32_t side);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PrepareWeatherRippleMode1(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind);
#else
void PrepareWeatherRippleMode1(WeatherParticleGroup *group, const Vector3 *position);
#endif
void EnsureWeatherRainMode1(WeatherParticleGroup *group, float scale);
void EnsureWeatherRippleGroup(WeatherParticleGroup *group, float scale);
void BeginRippleGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
std::uint32_t GetRainGroupFlags(const WeatherParticleGroup *group);
std::uint32_t GetRippleGroupFlags(const WeatherParticleGroup *group);
void SetRainGroupParameters(WeatherParticleGroup *group, float first, float second);
void SetRippleGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void SetRippleGroupTimeStep(WeatherParticleGroup *group, float step);
void UnregisterWeatherFixc(WeatherFixcOwner *owner);
void UnregisterWeatherFixc(WeatherFixcOwner *owner);
void SetRainGroupDrift(WeatherParticleGroup *group, const Vector3 *drift, const Vector3 *variance);
void EnsureWeatherRainGroup(WeatherParticleGroup *group, float scale);
void EnsureWeatherRippleGroup(WeatherParticleGroup *group, float scale);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void SetRainGroupColour(WeatherParticleGroup *group, std::uint32_t colour);
WeatherInteractionCamera *weatherInteractionCamera = 0;
void FreeWeatherRainMode1Group(WeatherParticleGroup *group);
void FreeWeatherParticleGroup(WeatherParticleGroup *group);
std::int32_t weatherDrawBudgetEnabled = 0;
float weatherDrawBudgetRemaining = 0.0f;
void FreeWeatherRainMode0Group(WeatherParticleGroup *group);
void PrepareWeatherRainMode0(WeatherParticleGroup *group, const Vector3 *, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind);
void PrepareWeatherRippleMode0(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *, const Vector3 *, const Vector3 *);
float WeatherFrustumSpan(float distance, std::int32_t fieldOfView);
void ComputeWeatherGroupCentre(const WeatherParticleGroup *group, const Vector3 *position, const Vector3 *backwards, Vector3 *centre);
void SetWeatherGroupBounds(WeatherParticleGroup *group, const Vector3 *center, float span);

void PrepareWeatherRainMode1(
    WeatherParticleGroup *group, const Vector3 *, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind, std::int32_t side);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PrepareWeatherRippleMode1(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind);
#else
void PrepareWeatherRippleMode1(WeatherParticleGroup *group, const Vector3 *position);
#endif
void UpdateWeatherDirectGroup(WeatherParticleGroup *group, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *wind);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void SetRainGroupColour(WeatherParticleGroup *group, std::uint32_t colour);
void FreeWeatherGroupRain(WeatherParticleGroup *group);
void FreeWeatherGroupRipples(WeatherParticleGroup *group);
float GetRainGroupCount(WeatherParticleGroup *group);
float GetRippleGroupCount(WeatherParticleGroup *group);

// @verified - d
void CopyWeatherWindDirection(const WeatherWind *wind, Vector3 *direction) {
    direction->x = wind->windDirection.x;
    direction->y = wind->windDirection.y;
    direction->z = wind->windDirection.z;
}

// @verified - d
HLib::Task *AllocateICAR(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused) {
    weatherInteractionCamera = AllocateWeatherInteractionCamera();
    return WAR_CallbackParticleResource_Enqueue(rainSpawnLimit, secondaryRainParameter, rippleSpawnLimit, unused);
}

// @verified - d
void UpdateWeatherInteractionCamera() {
    AdvanceWeatherInteractionCamera(weatherInteractionCamera);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ReleaseWeatherInteractionCamera() {
    FreeWeatherInteractionCamera(weatherInteractionCamera);
}

WeatherFixcOwner *AllocateWeatherFixcMode0(std::int32_t fov) {
    WeatherFixcOwner *const owner = static_cast<WeatherFixcOwner *>(HLib::AllocateTaskData(sizeof(WeatherFixcOwner), WEATHER_FIXC_STORAGE));
    owner->mode = 0;
    owner->cara = AllocateWeatherRainOwner(fov);
    owner->windList = 0;
    return owner;
}
#endif

// @verified - d
WeatherFixcOwner *AllocateWeatherFixc(std::int32_t fov) {
    WeatherFixcOwner *const owner = static_cast<WeatherFixcOwner *>(HLib::AllocateTaskData(sizeof(WeatherFixcOwner), WEATHER_FIXC_STORAGE));
    owner->mode = 1;
    owner->cara = AllocateWeatherRainOwner(fov);
    owner->windList = 0;
    return owner;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
WeatherFixcOwner *AllocateWeatherFixcGroup(const Vector3 *center, float spanX, float spanY, float spanZ) {
    WeatherFixcOwner *const owner = static_cast<WeatherFixcOwner *>(HLib::AllocateTaskData(sizeof(WeatherFixcOwner), SHEN_NAME4('F', 'I', 'X', 'G')));
    owner->mode = 6;
    owner->cara = AllocateWeatherFixcDirectGroup(center, spanX, spanY, spanZ);
    owner->windList = 0;
    return owner;
}

// @verified - d
void FreeWeatherFixc(WeatherFixcOwner *owner) {
    if (!owner)
        return;
    switch (owner->mode) {
    case 0:
        FreeWeatherRainOwnerMode0(static_cast<WeatherRainOwner *>(owner->cara));
        break;
    case 1:
        FreeWeatherRainOwnerMode1(static_cast<WeatherRainOwner *>(owner->cara));
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        FreeWeatherParticleGroup(static_cast<WeatherParticleGroup *>(owner->cara));
        break;
    }
    HLib::FreeTaskData(owner);
}
#endif

// @verified - d
void UpdateWeatherFixc(WeatherFixcOwner *owner) {
    Vector3 wind;
    wind.x = wind.y = wind.z = 0.0f;
    for (WeatherFixcNode *node = owner->windList; node; node = GetNextWeatherFixcNode(node)) {
        Vector3 direction;
        GetWindDirection(reinterpret_cast<const WeatherWind *>(GetWeatherFixcNodeData(node)), &direction);
        wind.x += direction.x;
        wind.z += direction.z;
    }
    switch (owner->mode) {
    case 0:
        UpdateWeatherRainOwnerMode0(static_cast<WeatherRainOwner *>(owner->cara), &weatherInteractionCamera->position,
            &weatherInteractionCamera->currentCameraPosition, &weatherInteractionCamera->currentCameraPosition2, &weatherInteractionCamera->backwardDirection,
            &wind);
        break;
    case 1:
        UpdateWeatherRainOwnerMode1(static_cast<WeatherRainOwner *>(owner->cara), &weatherInteractionCamera->position,
            &weatherInteractionCamera->currentCameraPosition, &weatherInteractionCamera->currentCameraPosition2, &weatherInteractionCamera->backwardDirection,
            &wind);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        UpdateWeatherDirectGroup(static_cast<WeatherParticleGroup *>(owner->cara), &weatherInteractionCamera->position,
            &weatherInteractionCamera->currentCameraPosition, &weatherInteractionCamera->currentCameraPosition2, &wind);
        break;
    }
}

// @verified - d
std::uint32_t GetWeatherFixcType(const WeatherFixcOwner *owner) {
    return owner->mode;
}

// @verified - d
void EnsureWeatherRainForFixc(WeatherFixcOwner *owner, float scale) {
    switch (owner->mode) {
    case 0:
        EnsureWeatherRainOwnerMode0(static_cast<WeatherRainOwner *>(owner->cara), scale);
        break;
    case 1:
        EnsureWeatherRainOwnerMode1(static_cast<WeatherRainOwner *>(owner->cara), scale);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        EnsureWeatherRainGroup(static_cast<WeatherParticleGroup *>(owner->cara), scale);
        break;
    }
}

void FreeWeatherFixcRain(WeatherFixcOwner *owner) {
    switch (owner->mode) {
    case 0:
    case 1:
        FreeWeatherRainOwnerDirectRain(static_cast<WeatherRainOwner *>(owner->cara));
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        FreeWeatherGroupRain(static_cast<WeatherParticleGroup *>(owner->cara));
        break;
    }
}

void ApplyWeatherClipSet0ToFixcRain(
    WeatherFixcOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    if (owner->mode == 1)
        ApplyWeatherRainOwnerClipMode0(static_cast<WeatherRainOwner *>(owner->cara), first, second, third, fourth, clipY0, clipY1);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetWeatherFixcRainFlags(const WeatherFixcOwner *owner) {
    switch (owner->mode) {
    case 0:
        return GetRainOwnerFlags(reinterpret_cast<const WeatherRainOwner *>(owner->cara));
    case 1:
        return GetRainOwnerFlags(reinterpret_cast<const WeatherRainOwner *>(owner->cara));
    }
    return GetRainGroupFlags(reinterpret_cast<const WeatherParticleGroup *>(owner->cara));
}
#else
// @verified - d
std::uint32_t GetWeatherFixcRainFlags(const WeatherFixcOwner *owner) {
    if (owner->mode == 0 || owner->mode == 1)
        return GetRainOwnerFlags(reinterpret_cast<const WeatherRainOwner *>(owner->cara));
    return GetRainGroupFlags(reinterpret_cast<const WeatherParticleGroup *>(owner->cara));
}
#endif

// @verified - d
void SetWeatherFixcRainCountImmediate(WeatherFixcOwner *owner, float pressure) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerCountImmediate(static_cast<WeatherRainOwner *>(owner->cara), pressure);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        SetRainGroupCountImmediate(static_cast<WeatherParticleGroup *>(owner->cara), pressure);
        break;
    }
}

// @verified - d
void BeginWeatherFixcRainCountTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames) {
    switch (owner->mode) {
    case 0:
    case 1:
        BeginRainOwnerCountTransition(static_cast<WeatherRainOwner *>(owner->cara), pressure, frames);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        BeginRainGroupCountTransition(static_cast<WeatherParticleGroup *>(owner->cara), pressure, frames);
        break;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float GetWeatherFixcRainCount(WeatherFixcOwner *owner) {
    switch (owner->mode) {
    case 0:
        return GetRainOwnerCount(static_cast<WeatherRainOwner *>(owner->cara));
    case 1:
        return GetRainOwnerCount(static_cast<WeatherRainOwner *>(owner->cara));
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        return GetRainGroupCount(static_cast<WeatherParticleGroup *>(owner->cara));
    }
    return 0.0f;
}
#endif

// @verified - d
void SetWeatherFixcRainColour(WeatherFixcOwner *owner, std::uint32_t colour) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerColour(static_cast<WeatherRainOwner *>(owner->cara), colour);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        SetRainGroupColour(static_cast<WeatherParticleGroup *>(owner->cara), colour);
        break;
    }
}

// @verified - d
void SetWeatherFixcRainDrift(WeatherFixcOwner *owner, const Vector3 *drift, const Vector3 *variance) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerDrift(static_cast<WeatherRainOwner *>(owner->cara), drift, variance);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        SetRainGroupDrift(static_cast<WeatherParticleGroup *>(owner->cara), drift, variance);
        break;
    }
}

// @verified - d
WeatherRainOwner *AllocateWeatherRainOwner(std::int32_t fov) {
    WeatherRainOwner *const owner = static_cast<WeatherRainOwner *>(HLib::AllocateTaskData(sizeof(WeatherRainOwner), WEATHER_RAIN_OWNER_STORAGE));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(owner), reinterpret_cast<std::uint32_t *>(&owner->rippleGroup1));
    owner->frustumFov = fov;
    return owner;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainOwnerMode0(WeatherRainOwner *owner) {
    FreeWeatherRainOwnerRainMode0(owner);
    FreeWeatherRainOwnerRipplesMode0(owner);
    HLib::FreeTaskData(owner);
}
#endif

// @verified - d
void FOCK_Reader(const MapInfoChunk *parameters) {
    if (!parameters)
        weatherDrawBudgetEnabled = 1;
    else {
        const std::int32_t flags = reinterpret_cast<const WeatherDrawBudgetChunk *>(parameters)->flags;
        weatherDrawBudgetEnabled = flags & 1;
    }
}

// @verified - d
void SetWeatherDrawBudgetEnabled(std::int32_t enabled) {
    weatherDrawBudgetEnabled = enabled;
}

// @verified - d
std::int32_t GetWeatherDrawBudgetEnabled() {
    return weatherDrawBudgetEnabled;
}

struct WeatherDrawBudgetCoefficients {
    float rain, ripple;
};

static const WeatherDrawBudgetCoefficients weatherParticleDrawCosts[2] = {{SHEN_FIELD(rain) 1.7391303777694702f, SHEN_FIELD(ripple) 3.3333332538604736f},
    {SHEN_FIELD(rain) 2.090909004211426f, SHEN_FIELD(ripple) 3.8888888359069824f}};

static const WeatherDrawBudgetCoefficients weatherParticleDrawRates[2] = {{SHEN_FIELD(rain) 0.574999988079071f, SHEN_FIELD(ripple) 0.30000001192092896f},
    {SHEN_FIELD(rain) 0.47826087474823f, SHEN_FIELD(ripple) 0.2571428716182709f}};

void AdjustWeatherDrawBudget(WeatherRainOwner *owner, std::int32_t mode) {
    if (!weatherDrawBudgetEnabled)
        return;
    const std::uint32_t now = *reinterpret_cast<std::uint32_t *>(kFrameTimerCounterRegister);
    const std::uint32_t elapsed = g_frameTimerStart - now;
    float remaining = 32583.328125f;
    remaining -= static_cast<float>(elapsed) / 12.5f;
    if (remaining < 0.0f) {
        if (WeatherParticleGroup *group = owner->rain0) {
            if (group->rainDrawCount > 90)
                group->rainDrawCount = 90;
            if (group->rainDrawCount > group->rainCapacity)
                group->rainDrawCount = group->rainCapacity;
        }
        if (WeatherParticleGroup *group = owner->rain1) {
            if (group->rainDrawCount > 200)
                group->rainDrawCount = 200;
            if (group->rainDrawCount > group->rainCapacity)
                group->rainDrawCount = group->rainCapacity;
        }
        if (WeatherParticleGroup *group = owner->rippleGroup)
            group->rippleDrawCount = 0;
    } else {
        const WeatherDrawBudgetCoefficients *drawCost = &weatherParticleDrawCosts[mode];
        const WeatherDrawBudgetCoefficients *drawRate = &weatherParticleDrawRates[mode];
        if (WeatherParticleGroup *group = owner->rain0) {
            if (static_cast<float>(group->rainDrawCount) > 0.0f) {
                const float cost = static_cast<float>(group->rainDrawCount) * drawCost->rain;
                if (remaining > cost) {
                    remaining -= cost;
                } else {
                    const float capacity = remaining * drawRate->rain;
                    std::int32_t count = static_cast<std::int32_t>(capacity);
                    if (count > 90)
                        count = 90;
                    if (count >= group->rainCapacity)
                        count = group->rainCapacity - 1;
                    if (count < group->rainDrawCount)
                        group->rainDrawCount = count;
                    remaining = 0.0f;
                }
            }
            if (group->rainDrawCount > group->rainCapacity)
                group->rainDrawCount = group->rainCapacity;
        }
        if (WeatherParticleGroup *group = owner->rain1) {
            const float cost = static_cast<float>(group->rainDrawCount) * drawCost->rain;
            if (cost > 0.0f) {
                if (remaining > 0.0f) {
                    if (remaining > cost) {
                        remaining -= cost;
                    } else {
                        const float capacity = remaining * drawRate->rain;
                        std::int32_t count = static_cast<std::int32_t>(capacity);
                        if (count > 200)
                            count = 200;
                        if (count >= group->rainCapacity)
                            count = group->rainCapacity - 1;
                        if (count < group->rainDrawCount)
                            group->rainDrawCount = count;
                        remaining = 0.0f;
                    }
                } else if (group->rainDrawCount > 200) {
                    group->rainDrawCount = 200;
                }
            }
            if (group->rainDrawCount > group->rainCapacity)
                group->rainDrawCount = group->rainCapacity;
        }
        if (WeatherParticleGroup *group = owner->rippleGroup) {
            const float cost = static_cast<float>(group->rippleDrawCount) * drawCost->ripple;
            if (cost > 0.0f) {
                if (remaining > 0.0f) {
                    if (remaining > cost) {
                        remaining -= cost;
                    } else {
                        const float capacity = remaining * drawRate->ripple;
                        std::int32_t count = static_cast<std::int32_t>(capacity);
                        if (count >= group->rippleCapacity)
                            count = group->rippleCapacity - 1;
                        if (count < group->rippleDrawCount)
                            group->rippleDrawCount = count;
                        remaining = 0.0f;
                    }
                } else {
                    group->rippleDrawCount = 0;
                }
            }
            if (group->rippleDrawCount > group->rippleCapacity)
                group->rippleDrawCount = group->rippleCapacity;
        }
    }
    weatherDrawBudgetRemaining = remaining;
}

// @verified - d
void UpdateWeatherRainOwnerMode0(
    WeatherRainOwner *owner, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *previousBackwards, const Vector3 *wind) {
    Vector3 centre;
    WeatherParticleGroup *group;
    if (owner->rain0) {
        group = owner->rain0;
        const float span = WeatherFrustumSpan(group->frustumDistance, camCtrlGetPerspectiveWrapper());
        group->frustumSpan = span;
        ComputeWeatherGroupCentre(group, position, previousBackwards, &centre);
        SetWeatherGroupBounds(group, &centre, span);
        PrepareWeatherRainMode0(group, position, target, backwards, wind);
    }
    if (owner->rain1) {
        group = owner->rain1;
        const float span = WeatherFrustumSpan(group->frustumDistance, camCtrlGetPerspectiveWrapper());
        group->frustumSpan = span;
        ComputeWeatherGroupCentre(group, position, previousBackwards, &centre);
        SetWeatherGroupBounds(group, &centre, span);
        PrepareWeatherRainMode0(group, position, target, backwards, wind);
    }
    if (owner->rippleGroup) {
        group = owner->rippleGroup;
        const float span = WeatherFrustumSpan(group->frustumDistance, camCtrlGetPerspectiveWrapper());
        group->frustumSpan = span;
        ComputeWeatherGroupCentre(group, position, previousBackwards, &centre);
        SetWeatherGroupBounds(group, &centre, span);
        PrepareWeatherRippleMode0(group, position, target, backwards, wind);
    }
    AdjustWeatherDrawBudget(owner, 0);
}

void EnsureWeatherRainOwnerMode0(WeatherRainOwner *owner, float scale) {
    Vector3 center;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;
    float span = WeatherFrustumSpan(2.8f, owner->frustumFov);
    if (!owner->rain0)
        owner->rain0 = AllocateWeatherParticleGroup(&center, span, span, span);
    EnsureWeatherRainMode0(owner->rain0, scale);
    SetWeatherGroupFrustumDistance(owner->rain0, 2.8f);
    span = WeatherFrustumSpan(6.0f, owner->frustumFov);
    if (!owner->rain1)
        owner->rain1 = AllocateWeatherParticleGroup(&center, span, span, span);
    EnsureWeatherRainMode0(owner->rain1, scale * 0.47265625f);
    SetWeatherGroupFrustumDistance(owner->rain1, 6.0f);
}

void FreeWeatherRainOwnerDirectRain(WeatherRainOwner *owner) {
    FreeWeatherGroupRain(owner->rain0);
    owner->rain0 = 0;
    FreeWeatherGroupRain(owner->rain1);
    owner->rain1 = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainOwnerRainMode0(WeatherRainOwner *owner) {
    FreeWeatherRainMode0Group(owner->rain0);
    FreeWeatherRainMode0Group(owner->rain1);
}
#endif

void EnsureWeatherRippleOwnerMode0(WeatherRainOwner *owner, float scale) {
    Vector3 center;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;
    const float span = WeatherFrustumSpan(4.0f, owner->frustumFov);
    if (!owner->rippleGroup)
        owner->rippleGroup = AllocateWeatherParticleGroup(&center, span, 1.0f, span);
    EnsureWeatherRippleMode1(owner->rippleGroup, scale);
    SetWeatherGroupFrustumDistance(owner->rippleGroup, 4.0f);
}

void FreeWeatherRainOwnerDirectRipples(WeatherRainOwner *owner) {
    FreeWeatherGroupRipples(owner->rippleGroup);
    owner->rippleGroup = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainOwnerRipplesMode0(WeatherRainOwner *owner) {
    FreeWeatherRainMode0Group(owner->rippleGroup);
}
#endif

// @verified - d
std::uint32_t GetRainOwnerFlags(const WeatherRainOwner *owner) {
    return GetRainGroupFlags(owner->rain0);
}

// @verified - d
void SetRainOwnerCountImmediate(WeatherRainOwner *owner, float pressure) {
    SetRainGroupCountImmediate(owner->rain0, pressure);
    SetRainGroupCountImmediate(owner->rain1, pressure);
}

// @verified - d
void BeginRainOwnerCountTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames) {
    BeginRainGroupCountTransition(owner->rain0, pressure, frames);
    BeginRainGroupCountTransition(owner->rain1, pressure, frames);
}

float GetRainOwnerCount(WeatherRainOwner *owner) {
    return GetRainGroupCount(owner->rain0);
}

// @verified - d
void SetRainOwnerColour(WeatherRainOwner *owner, std::uint32_t colour) {
    SetRainGroupColour(owner->rain0, colour);
    SetRainGroupColour(owner->rain1, colour);
}

// @verified - d
void SetRainOwnerDrift(WeatherRainOwner *owner, const Vector3 *drift, const Vector3 *variance) {
    SetRainGroupDrift(owner->rain0, drift, variance);
    SetRainGroupDrift(owner->rain1, drift, variance);
}

// @verified - d
void SetWeatherFixcRainParameters(WeatherFixcOwner *owner, float first, float second) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerParameters(static_cast<WeatherRainOwner *>(owner->cara), first, second);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        SetRainGroupParameters(static_cast<WeatherParticleGroup *>(owner->cara), first, second);
        break;
    }
}

// @verified - d
std::uint32_t GetRainOwnerRippleFlags(const WeatherRainOwner *owner) {
    return GetRippleGroupFlags(owner->rippleGroup);
}

// @verified - d
void EnsureWeatherRippleForFixc(WeatherFixcOwner *owner, float scale) {
    switch (owner->mode) {
    case 0:
        EnsureWeatherRippleOwnerMode0(static_cast<WeatherRainOwner *>(owner->cara), scale);
        break;
    case 1:
        EnsureWeatherRippleOwnerMode1(static_cast<WeatherRainOwner *>(owner->cara), scale);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        EnsureWeatherRippleMode0(static_cast<WeatherParticleGroup *>(owner->cara), scale);
        break;
    }
}

void FreeWeatherFixcRipples(WeatherFixcOwner *owner) {
    switch (owner->mode) {
    case 0:
    case 1:
        FreeWeatherRainOwnerDirectRipples(static_cast<WeatherRainOwner *>(owner->cara));
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        FreeWeatherGroupRipples(static_cast<WeatherParticleGroup *>(owner->cara));
        break;
    }
}

void ApplyWeatherClipSet0ToFixcRipple(
    WeatherFixcOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    if (owner->mode == 1)
        ApplyWeatherRippleOwnerClipMode0(static_cast<WeatherRainOwner *>(owner->cara), first, second, third, fourth, clipY0, clipY1);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetWeatherFixcRippleFlags(const WeatherFixcOwner *owner) {
    switch (owner->mode) {
    case 0:
        return GetRainOwnerRippleFlags(reinterpret_cast<const WeatherRainOwner *>(owner->cara));
    case 1:
        return GetRainOwnerRippleFlags(reinterpret_cast<const WeatherRainOwner *>(owner->cara));
    }
    return GetRippleGroupFlags(reinterpret_cast<const WeatherParticleGroup *>(owner->cara));
}
#else
// @verified - d
std::uint32_t GetWeatherFixcRippleFlags(const WeatherFixcOwner *owner) {
    if (owner->mode == 0 || owner->mode == 1)
        return GetRainOwnerRippleFlags(reinterpret_cast<const WeatherRainOwner *>(owner->cara));
    return GetRippleGroupFlags(reinterpret_cast<const WeatherParticleGroup *>(owner->cara));
}
#endif

// @verified - d
void SetRainOwnerRippleCountImmediate(WeatherRainOwner *owner, float pressure) {
    SetRippleGroupCountImmediate(owner->rippleGroup, pressure);
}

// @verified - d
void SetWeatherFixcRippleCountImmediate(WeatherFixcOwner *owner, float pressure) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerRippleCountImmediate(static_cast<WeatherRainOwner *>(owner->cara), pressure);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        SetRippleGroupCountImmediate(static_cast<WeatherParticleGroup *>(owner->cara), pressure);
        break;
    }
}

// @verified - d
void BeginRainOwnerRippleTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames) {
    BeginRippleGroupCountTransition(owner->rippleGroup, pressure, frames);
}

float ReleaseRainOwnerRippleGroup(WeatherRainOwner *owner) {
    return GetRippleGroupCount(owner->rippleGroup);
}

// @verified - d
void SetRainOwnerRippleColour(WeatherRainOwner *owner, std::uint32_t colour) {
    AllocateWeatherRippleColourRamp(owner->rippleGroup, colour);
}

// @verified - d
void SetRainOwnerRippleTimeStep(WeatherRainOwner *owner, float step) {
    SetRippleGroupTimeStep(owner->rippleGroup, step);
}

// @verified - d
void BeginWeatherFixcRippleTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames) {
    switch (owner->mode) {
    case 0:
    case 1:
        BeginRainOwnerRippleTransition(static_cast<WeatherRainOwner *>(owner->cara), pressure, frames);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        BeginRippleGroupCountTransition(static_cast<WeatherParticleGroup *>(owner->cara), pressure, frames);
        break;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float GetWeatherFixcRippleCount(WeatherFixcOwner *owner) {
    switch (owner->mode) {
    case 0:
        return ReleaseRainOwnerRippleGroup(static_cast<WeatherRainOwner *>(owner->cara));
    case 1:
        return ReleaseRainOwnerRippleGroup(static_cast<WeatherRainOwner *>(owner->cara));
    case 2:
    case 3:
    case 4:
    case 6:
        return GetRippleGroupCount(static_cast<WeatherParticleGroup *>(owner->cara));
    case 5:
        break;
    }
    return 0.0f;
}
#endif

// @verified - d
void SetWeatherFixcRippleColour(WeatherFixcOwner *owner, std::uint32_t colour) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerRippleColour(static_cast<WeatherRainOwner *>(owner->cara), colour);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        AllocateWeatherRippleColourRamp(static_cast<WeatherParticleGroup *>(owner->cara), colour);
        break;
    }
}

// @verified - d
void SetWeatherFixcRippleTimeStep(WeatherFixcOwner *owner, float step) {
    switch (owner->mode) {
    case 0:
    case 1:
        SetRainOwnerRippleTimeStep(static_cast<WeatherRainOwner *>(owner->cara), step);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        SetRippleGroupTimeStep(static_cast<WeatherParticleGroup *>(owner->cara), step);
        break;
    }
}

// @verified - d
void AddWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind) {
    if (wind && !WeatherFixcHasWind(owner, wind)) {
        WeatherFixcNode *const node = AllocateWeatherFixcNode(0, wind);
        if (!owner->windList)
            owner->windList = node;
        else
            LinkWeatherFixcNodes(GetLastWeatherFixcNode(owner->windList), node);
    }
}

// @verified - d
void RemoveWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind) {
    WeatherFixcNode *const node = FindWeatherFixcNode(owner->windList, wind);
    if (node) {
        if (owner->windList == node)
            owner->windList = GetNextWeatherFixcNode(node);
        DetachWeatherFixcNode(node);
        FreeWeatherFixcNode(node);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainOwnerMode1(WeatherRainOwner *owner) {
    FreeWeatherRainOwnerRainMode1(owner);
    FreeWeatherRainOwnerRipplesMode1(owner);
    HLib::FreeTaskData(owner);
}
#endif

void UpdateWeatherRainOwnerMode1(
    WeatherRainOwner *owner, const Vector3 *position, const Vector3 *target, const Vector3 *backwards, const Vector3 *previousBackwards, const Vector3 *wind) {
    Vector3 centre;
    Vector3 scaledWind;
    scaledWind.x = wind->x * 8.0f;
    scaledWind.y = wind->y * 8.0f;
    scaledWind.z = wind->z * 8.0f;
    WeatherParticleGroup *group;
    if (owner->rain0) {
        group = owner->rain0;
        const float span = WeatherFrustumSpan(group->frustumDistance, camCtrlGetPerspectiveWrapper());
        group->frustumSpan = span;
        ComputeWeatherGroupCentre(group, position, previousBackwards, &centre);
        SetWeatherGroupBounds(group, &centre, span);
        PrepareWeatherRainMode1(group, position, target, backwards, &scaledWind, 0);
    }
    if (owner->rain1) {
        group = owner->rain1;
        const float span = WeatherFrustumSpan(group->frustumDistance, camCtrlGetPerspectiveWrapper());
        group->frustumSpan = span;
        ComputeWeatherGroupCentre(group, position, previousBackwards, &centre);
        SetWeatherGroupBounds(group, &centre, span);
        PrepareWeatherRainMode1(group, position, target, backwards, &scaledWind, 1);
    }
    if (owner->rippleGroup) {
        group = owner->rippleGroup;
        const float span = WeatherFrustumSpan(group->frustumDistance, camCtrlGetPerspectiveWrapper());
        group->frustumSpan = span;
        ComputeWeatherGroupCentre(group, position, previousBackwards, &centre);
        SetWeatherGroupBounds(group, &centre, span);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        PrepareWeatherRippleMode1(group, position, target, backwards, wind);
#else
        PrepareWeatherRippleMode1(group, position);
#endif
    }
    AdjustWeatherDrawBudget(owner, 1);
}

void EnsureWeatherRainOwnerMode1(WeatherRainOwner *owner, float scale) {
    Vector3 center;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;
    float span = WeatherFrustumSpan(2.8f, owner->frustumFov);
    if (!owner->rain0)
        owner->rain0 = AllocateWeatherClipParticleGroup(&center, span, span, span);
    EnsureWeatherRainMode1(owner->rain0, scale);
    SetWeatherGroupFrustumDistance(owner->rain0, 2.8f);
    span = WeatherFrustumSpan(6.0f, owner->frustumFov);
    if (!owner->rain1)
        owner->rain1 = AllocateWeatherClipParticleGroup(&center, span, span, span);
    EnsureWeatherRainMode1(owner->rain1, scale * 0.47265625f);
    SetWeatherGroupFrustumDistance(owner->rain1, 6.0f);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainOwnerRainMode1(WeatherRainOwner *owner) {
    FreeWeatherRainMode1Group(owner->rain0);
    FreeWeatherRainMode1Group(owner->rain1);
}
#endif

void ApplyWeatherRainOwnerClipMode0(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    SetWeatherClipPlanesSet0(owner->rain0, first, second, third, clipY0, clipY1, fourth);
    SetWeatherClipPlanesSet0(owner->rain1, first, second, third, clipY0, clipY1, fourth);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyWeatherRainOwnerClipMode1(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    SetWeatherClipPlanes(owner->rain0, first, second, third, fourth, clipY0, clipY1);
    SetWeatherClipPlanes(owner->rain1, first, second, third, fourth, clipY0, clipY1);
}
#endif

void EnsureWeatherRippleOwnerMode1(WeatherRainOwner *owner, float scale) {
    Vector3 center;
    center.x = center.y = center.z = 0.0f;
    const float span = WeatherFrustumSpan(4.0f, owner->frustumFov);
    if (!owner->rippleGroup)
        owner->rippleGroup = AllocateWeatherClipParticleGroup(&center, span, 1.0f, span);
    EnsureWeatherRippleGroup(owner->rippleGroup, scale);
    SetWeatherGroupFrustumDistance(owner->rippleGroup, 4.0f);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void FreeWeatherRainOwnerRipplesMode1(WeatherRainOwner *owner) {
    FreeWeatherRainMode1Group(owner->rippleGroup);
}
#endif

void ApplyWeatherRippleOwnerClipMode0(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    SetWeatherClipPlanesSet0(owner->rippleGroup, first, second, third, clipY0, clipY1, fourth);
    SetWeatherClipPlanesSet0(owner->rippleGroup1, first, second, third, clipY0, clipY1, fourth);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ApplyWeatherRippleOwnerClipMode1(
    WeatherRainOwner *owner, const Vector2 *first, const Vector2 *second, const Vector2 *third, const Vector2 *fourth, float clipY0, float clipY1) {
    SetWeatherClipPlanes(owner->rippleGroup, first, second, third, fourth, clipY0, clipY1);
    SetWeatherClipPlanes(owner->rippleGroup1, first, second, third, fourth, clipY0, clipY1);
}
#endif

void GetWeatherRainOwnerClipMode0(
    const WeatherRainOwner *owner, Vector2 *first, Vector2 *second, Vector2 *third, Vector2 *fourth, float *clipY0, float *clipY1) {
    const WeatherParticleGroup *group;
    if (owner->rain0)
        group = owner->rain0;
    else if (owner->mode)
        group = reinterpret_cast<const WeatherParticleGroup *>(owner->mode);
    else
        group = owner->rippleGroup;
    *clipY1 = group->clipA.clipY1;
    *clipY0 = group->clipA.clipY0;
    first->x = group->clipA.x[0];
    first->y = group->clipA.z[0];
    second->x = group->clipA.x[1];
    second->y = group->clipA.z[1];
    third->x = group->clipA.x[2];
    third->y = group->clipA.z[2];
    fourth->x = group->clipA.x[3];
    fourth->y = group->clipA.z[3];
}

// @verified - d
bool WeatherFixcHasWind(const WeatherFixcOwner *owner, WeatherWind *wind) {
    if (FindWeatherFixcNode(owner->windList, wind))
        return true;
    return false;
}

// @verified - d
WeatherInteractionCamera *AllocateWeatherInteractionCamera() {
    WeatherInteractionCamera *const camera =
        static_cast<WeatherInteractionCamera *>(HLib::AllocateTaskData(sizeof(WeatherInteractionCamera), WEATHER_INTERACTION_CAMERA_STORAGE));
    camCtrlGetNextCamPos(&camera->position);
    camCtrlGetNextCamPos(&camera->nextCameraPosition);
    camCtrlGetTargetPosition(&camera->currentCameraPosition);
    camCtrlGetTargetPosition(&camera->currentCameraPosition2);
    camera->previousBackwardDirection.x = camera->backwardDirection.x = 0.0f;
    camera->previousBackwardDirection.y = camera->backwardDirection.y = 0.0f;
    camera->previousBackwardDirection.z = camera->backwardDirection.z = 0.0f;
    return camera;
}

void FreeWeatherInteractionCamera(WeatherInteractionCamera *camera) {
    HLib::FreeTaskData(camera);
}

void AdvanceWeatherInteractionCamera(WeatherInteractionCamera *camera) {
    Vector3 *const next = &camera->nextCameraPosition;
    Vector3 *const target = &camera->currentCameraPosition2;
    Vector3 *const backwards = &camera->backwardDirection;
    camera->position.x = next->x;
    camera->position.y = next->y;
    camera->position.z = next->z;
    camera->currentCameraPosition.x = target->x;
    camera->currentCameraPosition.y = target->y;
    camera->currentCameraPosition.z = target->z;
    camera->previousBackwardDirection.x = backwards->x;
    camera->previousBackwardDirection.y = backwards->y;
    camera->previousBackwardDirection.z = backwards->z;
    camCtrlGetNextCamPos(next);
    camCtrlGetTargetPosition(target);
    backwards->x = target->x - next->x;
    backwards->y = target->y - next->y;
    backwards->z = target->z - next->z;
    am2Vector3NormalizeInPlace(backwards);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
WeatherRainOwner *GetWeatherFixcMode1RainOwner(const WeatherFixcOwner *owner) {
    if (owner->mode == 1)
        return static_cast<WeatherRainOwner *>(owner->cara);
    return 0;
}
#endif

// @verified - d
void SetRainOwnerParameters(WeatherRainOwner *owner, float first, float second) {
    SetRainGroupParameters(owner->rain0, first, second);
    SetRainGroupParameters(owner->rain1, first, (second * 0.17f) / 0.152f);
}

void SetRainOwnerMotionDamping(WeatherRainOwner *owner, float damping) {
    SetRainGroupMotionDamping(owner->rain0, damping);
    SetRainGroupMotionDamping(owner->rain1, damping);
}

float GetRainOwnerMotionDamping(WeatherRainOwner *owner) {
    return GetRainGroupMotionDamping(owner->rain0);
}

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void __attribute__((renesas)) shenmue_s2_event_EV_WeatherEffectUtil_Impl(std::uint32_t fov, float rainScale, float unused, float rippleScale) {
    shenmue::s2::EV_WeatherEffectUtil_Impl(fov, rainScale, unused, rippleScale);
}
extern "C" void __attribute__((renesas)) shenmue_s2_event_Weather_FreeActiveFIXC() {
    shenmue::s2::Weather_FreeActiveFIXC();
}
#endif

#endif
