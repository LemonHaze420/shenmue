#include "s2/weather_particles_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "platform/dc/weather_texture.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/weather_data_dc.hpp"
#include "platform/dc/ninja.hpp"
#include "s2/character_dc.hpp"
#include "s2/map_control_dc.hpp"
#include <cstring>
#include "platform/dc/matrix.hpp"
#include "s2/camera_dc.hpp"
#include <cmath>
#include "s2/hlib.hpp"
#include "s2/field_collision_dc.hpp"
#include <cfloat>
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
#include "s2/frame_dc.hpp"

namespace shenmue {
namespace s2 {

void InitializeWeatherRainRenderState(WeatherParticleGroup *group, WeatherRainParticleUpdate update);
void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);
void WeatherRainParticleUpdate_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void WeatherRainParticleUpdate_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void WeatherRainParticleUpdate_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);
void ResetWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle);
void InitializeWeatherRippleRenderState(WeatherParticleGroup *group, WeatherRippleParticleUpdate update);
void WeatherRippleParticleSpawn_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleSpawn_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleSpawn_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleUpdate_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleUpdate_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleUpdate_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group);

void BeginRainOwnerRippleTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
WeatherParticleGroup *weatherGroupList;
HLib::Task *weatherParticleResourceTask;
std::int32_t weatherWarState, g_currentWeatherForceFlag = 0;
float weatherRainSpawnLimit, weatherSecondaryRainParameter, weatherRippleSpawnLimit = 0.0f, weatherParticleUnusedParameter = 0.0f;
void WeatherParticle_Callback(void *groups);
void UpdateWeatherFixc(WeatherFixcOwner *owner);
void SetRainOwnerCountImmediate(WeatherRainOwner *owner, float pressure);
void BeginRainOwnerCountTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
Vector3 weatherParticleGroundCameraPosition;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
inline bool WeatherPointInsideClipPlanes(const WeatherClipQuadPlanes &clip, const Vector3 &position) {
    const float x = position.x, z = position.z;
    return (x - clip.x[0]) * clip.edgeZ[0] - (z - clip.z[0]) * clip.edgeZ[1] < 0.0f &&
        (x - clip.x[1]) * clip.edgeZ[2] - (z - clip.z[1]) * clip.edgeZ[3] < 0.0f && (x - clip.x[2]) * clip.edgeX[0] - (z - clip.z[2]) * clip.edgeX[1] < 0.0f &&
        (x - clip.x[3]) * clip.edgeX[2] - (z - clip.z[3]) * clip.edgeX[3] < 0.0f;
}

inline bool WeatherPointInsideClip(const WeatherClipQuadPlanes &clip, const Vector3 &position) {
    if (position.y > clip.clipY1 || clip.clipY0 > position.y)
        return false;
    return WeatherPointInsideClipPlanes(clip, position);
}
#else
bool WeatherPointInsideClipPlanes(const WeatherClipQuadPlanes &clip, const Vector3 &position) {
    for (std::size_t index = 0; index < 4; ++index) {
        const float *edges = index < 2 ? clip.edgeZ : clip.edgeX;
        const std::size_t edge = (index & 1U) * 2U;
        const float side = (position.x - clip.x[index]) * edges[edge] - (position.z - clip.z[index]) * edges[edge + 1];
        if (!(side < 0.0f))
            return false;
    }
    return true;
}

bool WeatherPointInsideClip(const WeatherClipQuadPlanes &clip, const Vector3 &position) {
    if (position.y > clip.clipY1 || clip.clipY0 > position.y)
        return false;
    return WeatherPointInsideClipPlanes(clip, position);
}
#endif

static void FreeWeatherTextureSurface(KMSURFACEDESC *surface) {
    if (surface)
        kmFreeTexture(surface);
}

// @verified - d
NJS_TEXSURFACE *GetEffectTextureReference(std::int32_t index) {
    NJS_TEXSURFACE *surface;
    if (!index)
        surface = &g_weatherTextureSurfaces.surfaces[0];
    else
        surface = &g_weatherTextureSurfaces.surfaces[1];
    return surface;
}

// @verified - d
void ReleaseWeatherTextureSurfaces() {
    FreeWeatherTextureSurface(reinterpret_cast<KMSURFACEDESC *>(g_weatherTextureSurfaces.active[0]));
    FreeWeatherTextureSurface(reinterpret_cast<KMSURFACEDESC *>(g_weatherTextureSurfaces.active[1]));
    InitializeWeatherTextureSurfaces();
}

// @verified - d
const std::uint32_t *GetEffectTextureReferenceDimensions(std::int32_t index) {
    if (!index)
        return g_weatherTextureSurfaces.dimensions[0];
    return g_weatherTextureSurfaces.dimensions[1];
}

// @verified - d
void WAR_CallbackParticleResource_Callback(void *) {
    if (weatherWarState)
        HLib::ExecuteCallbackNow(WeatherParticle_Callback, weatherGroupList);
}

// @verified - d
void SetRainGroupDrawingEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (group)
        group->rainFlags = enabled ? group->rainFlags | 0x80000000U : group->rainFlags & 0x7fffffffU;
}

// @verified - d
void ReadWeatherRipplePvrTexture(const void *data) {
    CreateWeatherTextureSurface(64, 64, 0x302, reinterpret_cast<const std::byte *>(data) + 0x1c, 1);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetRippleGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (group) {
        if (enabled)
            group->rippleFlags &= ~0x10000000U;
        else
            group->rippleFlags |= 0x10000000U;
    }
}
#endif

// @verified - d
void SetRippleGroupDrawingEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (group) {
        if (enabled)
            group->rippleFlags |= 0x80000000U;
        else
            group->rippleFlags &= 0x7fffffffU;
    }
}

// @verified - d
void SetWeatherParticleDrawingEnabled(std::int32_t enabled) {
    for (WeatherParticleGroup *group = weatherGroupList; group; group = group->next) {
        SetRainGroupDrawingEnabled(group, enabled);
        SetRippleGroupDrawingEnabled(group, enabled);
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetRainGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (group)
        group->rainFlags = enabled ? group->rainFlags & ~0x10000000U : group->rainFlags | 0x10000000U;
}
#endif

// @verified - d
void SetWeatherRainSpawningEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (group)
        group->rainFlags = enabled ? group->rainFlags | 1U : group->rainFlags & ~1U;
}

// @verified - d
void BeginRippleGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames) {
    if (!group || !group->rippleParticles)
        return;
    const std::int32_t count = static_cast<std::int32_t>(static_cast<float>(group->rippleCapacity) * pressure);
    BeginWeatherFloatTransition(group->rippleCountLflt, static_cast<float>(count), frames);
}

// @verified - d
std::int32_t IsWeatherRainSpawningEnabled(const WeatherParticleGroup *group) {
    return group ? group->rainFlags & 1U : 0;
}

// @verified - d
void SetWeatherRainDepthEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (!group)
        return;
    if (enabled) {
        group->rainPacket.command |= 0x02000000U;
        group->rainPacket.tsp = ((group->rainPacket.tsp | 0x00100000U) & 0x00ffffffU) | 0x94000000U;
        group->rainFlags |= 0x40000000U;
    } else {
        group->rainPacket.command &= ~0x02000000U;
        group->rainPacket.tsp &= ~0x00100000U;
        group->rainPacket.tsp = (group->rainPacket.tsp & 0x00ffffffU) | 0x20000000U;
        group->rainFlags &= ~0x40000000U;
    }
}

// @verified - d
void InitializeWeatherRainDirect(WeatherParticleGroup *group) {
    InitializeWeatherRainRenderState(group, SpawnWeatherRainParticle_Direct);
}

// @verified - d
void SetWeatherRippleDepthEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (!group)
        return;
    if (enabled) {
        group->ripplePacket.command |= 0x02000000U;
        group->ripplePacket.tsp = ((group->ripplePacket.tsp | 0x00100000U) & 0x00ffffffU) | 0x94000000U;
        group->rippleFlags |= 0x40000000U;
    } else {
        group->ripplePacket.command &= ~0x02000000U;
        group->ripplePacket.tsp &= ~0x00100000U;
        group->ripplePacket.tsp = (group->ripplePacket.tsp & 0x00ffffffU) | 0x20000000U;
        group->rippleFlags &= ~0x40000000U;
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetWeatherRippleFilteringEnabled(WeatherParticleGroup *group, std::int32_t enabled) {
    if (!group)
        return;
    if (enabled)
        group->ripplePacket.tsp = (group->ripplePacket.tsp & ~0x6000U) | 0x2000U;
    else
        group->ripplePacket.tsp &= ~0x6000U;
}
#endif

// @verified - d
std::int32_t BindWeatherRippleTexture(WeatherParticleGroup *group) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t texture = 0;
    NJS_TEXSURFACE *const reference = GetEffectTextureReference(1);
    group->rippleDrawState.texture = reference;
    if (!reference)
        return 0;
    const NJS_TEXSURFACE *const surface = static_cast<NJS_TEXSURFACE *>(group->rippleDrawState.texture);
    group->ripplePacket.command = 0x80000002U;
    const std::uint32_t *dimensions = GetEffectTextureReferenceDimensions(1);
    std::int32_t width = static_cast<std::int32_t>(*dimensions++);
    std::int32_t height = static_cast<std::int32_t>(*dimensions);
    std::int32_t bits = 0;
    for (; width; width >>= 1)
        ++bits;
    bits -= 4;
    std::uint32_t size = static_cast<std::uint32_t>(bits) << 3;
    bits = 0;
    for (; height; height >>= 1)
        ++bits;
    bits -= 4;
    size = (size & ~7U) | static_cast<std::uint32_t>(bits);
    texture = (texture & 0x7fffffffU) | (surface->fSurfaceFlags << 31);
    const std::uint32_t flags = surface->fSurfaceFlags;
    texture = (texture & 0xbfffffffU) | ((flags & 8U) << 27);
    texture = (texture & 0xc7ffffffU) | (surface->PixelFormat & 0x38000000U);
    texture = (texture & 0xfbffffffU) | ((~(flags & 4U) & 4U) << 24);
    texture = (texture & 0xfdffffffU) | ((flags & 0x10U) << 21);
    texture = (texture & 0xfeffffffU) | ((flags & 2U) << 23);
    texture = (texture & 0xff800000U) | static_cast<std::uint32_t>(reinterpret_cast<std::int32_t>(surface->pSurface) >> 3);
    group->ripplePacket.command |= 8U;
    group->ripplePacket.isp = 0x8a800000U;
    group->ripplePacket.tsp = size | 0x940242c0U;
    group->ripplePacket.tcw = texture;
    group->rippleFlags |= 0x04000000U;
    return 1;
#else
    NJS_TEXSURFACE *const surface = GetEffectTextureReference(1);
    group->rippleDrawState.texture = surface;
    if (!surface)
        return 0;
    group->ripplePacket.command = 0x80000002U;
    const std::uint32_t *const dimensions = GetEffectTextureReferenceDimensions(1);
    std::int32_t widthBits = 0, heightBits = 0;
    for (std::int32_t width = static_cast<std::int32_t>(dimensions[0]); width != 0; width >>= 1)
        ++widthBits;
    for (std::int32_t height = static_cast<std::int32_t>(dimensions[1]); height != 0; height >>= 1)
        ++heightBits;
    const std::uint32_t flags = surface->fSurfaceFlags;
    const std::uint32_t format = surface->PixelFormat;
    const std::int32_t address = reinterpret_cast<std::int32_t>(surface->pSurface);
    group->ripplePacket.command |= 8U;
    group->ripplePacket.isp = 0x8a800000U;
    group->ripplePacket.tsp = (static_cast<std::uint32_t>(widthBits - 4) * 8U) | static_cast<std::uint32_t>(heightBits - 4) | 0x940242c0U;
    group->ripplePacket.tcw = (flags << 31) | ((flags & 8U) << 27) | (format & 0x38000000U) | ((~flags & 4U) << 24) | ((flags & 0x10U) << 21) |
        ((flags & 2U) << 23) | static_cast<std::uint32_t>(address >> 3);
    group->rippleFlags |= 0x04000000U;
    return 1;
#endif
}

// @verified - d
void InitializeWeatherRippleMode0(WeatherParticleGroup *group) {
    InitializeWeatherRippleRenderState(group, WeatherRippleParticleSpawn_Mode0);
}

// @verified - d
void InitializeWeatherRippleMode1(WeatherParticleGroup *group) {
    InitializeWeatherRippleRenderState(group, WeatherRippleParticleSpawn_Mode1);
}

// @verified - d
void InitializeWeatherRippleDirect(WeatherParticleGroup *group) {
    InitializeWeatherRippleRenderState(group, WeatherRippleParticleSpawn_Direct);
}

// @verified - d
void InitializeWeatherRainMode0(WeatherParticleGroup *group) {
    InitializeWeatherRainRenderState(group, SpawnWeatherRainParticle_Mode0);
}

// @verified - d
void InitializeWeatherRainMode1(WeatherParticleGroup *group) {
    InitializeWeatherRainRenderState(group, SpawnWeatherRainParticle_Mode1WCCP);
}

// @verified - d
void InitializeWeatherRainRenderState(WeatherParticleGroup *group, WeatherRainParticleUpdate update) {
    group->rainPacket.command = 0x80000002U;
    group->rainPacket.isp = 0x88800000U;
    group->rainPacket.tsp = 0x208000c0U;
    group->rainPacket.tcw = 0;
    group->rainFlags = 0x82000000U;
    SetWeatherRainDepthEnabled(group, 1);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t *word = reinterpret_cast<std::uint32_t *>(group->rainParticles);
    for (std::int32_t index = 0; index < group->rainCapacity; ++index) {
        *word++ = 0x40000000U;
        *word++ = reinterpret_cast<std::uint32_t>(update);
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
    }
#else
    for (std::int32_t index = 0; index < group->rainCapacity; ++index) {
        WeatherRainParticle &particle = group->rainParticles[index];
        particle.flags = 0x40000000U;
        particle.update = update;
        particle.position = __typeof__(particle.position)();
        particle.drift = __typeof__(particle.drift)();
    }
#endif
}

// @verified - d
void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    --group->rainCurrentCount;
    ++group->rainLiveCount;
    particle->flags = 0x80000000U;
    particle->update = WeatherRainParticleUpdate_Direct;
    particle->position.x = dc::sdk::RandomFloat() * group->frustumDistance + group->boundsMinX;
    float random = dc::sdk::RandomFloat();
    particle->position.y = group->boundsMaxY - ((group->rainVerticalSpan * 8.0f) * random);
    particle->position.z = dc::sdk::RandomFloat() * group->clipA.clipY1 + group->boundsMinZ;
    random = dc::sdk::RandomFloat();
    particle->drift.x = (random * group->driftVariance.x - group->driftVariance.x * 0.5f) + group->spawnDriftX;
    random = dc::sdk::RandomFloat();
    particle->drift.z = (random * group->driftVariance.z - group->driftVariance.z * 0.5f) + group->spawnDriftZ;
}

// @verified - d
void ResetWeatherRainParticle_Direct(WeatherRainParticle *particle) {
    particle->flags = 0x40000000U;
    particle->update = SpawnWeatherRainParticle_Direct;
}

// @verified - d
void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    --group->rainCurrentCount;
    ++group->rainLiveCount;
    particle->flags = 0x80000000U;
    particle->update = WeatherRainParticleUpdate_Mode0;
    particle->position.x = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinX;
    float random = dc::sdk::RandomFloat();
    particle->position.y = group->boundsMaxY - ((group->rainVerticalSpan * 8.0f) * random);
    particle->position.z = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinZ;
    random = dc::sdk::RandomFloat();
    particle->drift.x = (random * group->driftVariance.x - group->driftVariance.x * 0.5f) + group->spawnDriftX;
    random = dc::sdk::RandomFloat();
    particle->drift.z = (random * group->driftVariance.z - group->driftVariance.z * 0.5f) + group->spawnDriftZ;
}

// @verified - d
void ResetWeatherRainParticle_Mode0(WeatherRainParticle *particle) {
    particle->flags = 0x40000000U;
    particle->update = SpawnWeatherRainParticle_Mode0;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    --group->rainCurrentCount;
    ++group->rainLiveCount;
    particle->update = WeatherRainParticleUpdate_Mode1WCCP;
    particle->position.x = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinX;
    float random;
    random = dc::sdk::RandomFloat();
    particle->position.y = group->boundsMaxY - ((group->rainVerticalSpan * 8.0f) * random);
    particle->position.z = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinZ;
    random = dc::sdk::RandomFloat();
    particle->drift.x = (random * group->driftVariance.x - group->driftVariance.x * 0.5f) + group->spawnDriftX;
    random = dc::sdk::RandomFloat();
    particle->drift.z = (random * group->driftVariance.z - group->driftVariance.z * 0.5f) + group->spawnDriftZ;
    if (WeatherPointInsideClip(group->clipA, particle->position) || WeatherPointInsideClip(group->clipB, particle->position)) {
        particle->flags = 0x40000000U;
        particle->update = SpawnWeatherRainParticle_Mode1WCCP;
    } else {
        particle->flags = 0x80000000U;
    }
}
#else
// @verified - d
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    --group->rainCurrentCount;
    ++group->rainLiveCount;
    particle->update = WeatherRainParticleUpdate_Mode1WCCP;
    float random = dc::sdk::RandomFloat();
    float position = group->boundsMinX;
    asm("fmac fr0,%1,%0" : "+f"(position) : "f"(group->frustumSpan), "w"(random));
    particle->position.x = position;
    random = dc::sdk::RandomFloat();
    particle->position.y = group->boundsMaxY - ((group->rainVerticalSpan * 8.0f) * random);
    random = dc::sdk::RandomFloat();
    position = group->boundsMinZ;
    asm("fmac fr0,%1,%0" : "+f"(position) : "f"(group->frustumSpan), "w"(random));
    particle->position.z = position;
    random = dc::sdk::RandomFloat();
    particle->drift.x = (random * group->driftVariance.x - group->driftVariance.x * 0.5f) + group->spawnDriftX;
    random = dc::sdk::RandomFloat();
    particle->drift.z = (random * group->driftVariance.z - group->driftVariance.z * 0.5f) + group->spawnDriftZ;
    if (WeatherPointInsideClip(group->clipA, particle->position) || WeatherPointInsideClip(group->clipB, particle->position)) {
        particle->flags = 0x40000000U;
        particle->update = SpawnWeatherRainParticle_Mode1WCCP;
    } else {
        particle->flags = 0x80000000U;
    }
}
#endif

// @verified - d
void WeatherRainParticleUpdate_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->boundsMinY > particle->position.y)
        goto reset;
    particle->position.x = (particle->drift.x + group->rainMotion.x) + particle->position.x;
    particle->position.y += group->fallSpeed;
    particle->position.z = (particle->drift.z + group->rainMotion.z) + particle->position.z;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumDistance;
        return;
    }
    if (particle->position.x < group->boundsMinX) {
        particle->position.x += group->frustumDistance;
        return;
    }
    if (particle->position.z > group->boundsMaxZ)
        particle->position.z -= group->clipA.clipY1;
    else if (particle->position.z < group->boundsMinZ)
        particle->position.z += group->clipA.clipY1;
    return;
reset:
    particle->flags = 0x40000000U;
    particle->update = SpawnWeatherRainParticle_Direct;
    --group->rainLiveCount;
}

// @verified - d
void WeatherRainParticleUpdate_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->boundsMinY > particle->position.y)
        goto reset;
    particle->position.x = (particle->drift.x + group->rainMotion.x) + particle->position.x;
    particle->position.y += group->rainMotion.y;
    particle->position.z = (particle->drift.z + group->rainMotion.z) + particle->position.z;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumSpan;
        return;
    }
    if (particle->position.x < group->boundsMinX) {
        particle->position.x += group->frustumSpan;
        return;
    }
    if (particle->position.z > group->boundsMaxZ)
        particle->position.z -= group->frustumSpan;
    else if (particle->position.z < group->boundsMinZ)
        particle->position.z += group->frustumSpan;
    return;
reset:
    particle->flags = 0x40000000U;
    particle->update = SpawnWeatherRainParticle_Mode0;
    --group->rainLiveCount;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void WeatherRainParticleUpdate_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->boundsMinY > particle->position.y)
        goto reset;
    particle->position.x = (particle->drift.x + group->rainMotion.x) + particle->position.x;
    particle->position.y += group->rainMotion.y;
    particle->position.z = (particle->drift.z + group->rainMotion.z) + particle->position.z;
    if (particle->position.x > group->boundsMaxX)
        particle->position.x -= group->frustumSpan;
    else if (particle->position.x < group->boundsMinX)
        particle->position.x += group->frustumSpan;
    else if (particle->position.z > group->boundsMaxZ)
        particle->position.z -= group->frustumSpan;
    else if (particle->position.z < group->boundsMinZ)
        particle->position.z += group->frustumSpan;
    if (WeatherPointInsideClip(group->clipA, particle->position)) {
        particle->flags = 0x40000000U;
        return;
    }
    if (WeatherPointInsideClip(group->clipB, particle->position)) {
        particle->flags = 0x40000000U;
        return;
    }
    particle->flags = 0x80000000U;
    return;
reset:
    particle->flags = 0x40000000U;
    particle->update = SpawnWeatherRainParticle_Mode1WCCP;
    --group->rainLiveCount;
}

void RotateWeatherVectorXZ(float *outX, float *outZ, float x, float z, float sine, float cosine) {
    *outX = sine * z + cosine * x;
    *outZ = -sine * x + cosine * z;
}
#else
// @verified - d
void WeatherRainParticleUpdate_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->boundsMinY > particle->position.y) {
        ResetWeatherRainParticle_Mode1WCCP(particle);
        --group->rainLiveCount;
        return;
    }
    particle->position.x = (particle->drift.x + group->rainMotion.x) + particle->position.x;
    particle->position.y += group->rainMotion.y;
    particle->position.z = (particle->drift.z + group->rainMotion.z) + particle->position.z;
    if (particle->position.x > group->boundsMaxX)
        particle->position.x -= group->frustumSpan;
    else if (group->boundsMinX > particle->position.x)
        particle->position.x += group->frustumSpan;
    else if (particle->position.z > group->boundsMaxZ)
        particle->position.z -= group->frustumSpan;
    else if (group->boundsMinZ > particle->position.z)
        particle->position.z += group->frustumSpan;
    particle->flags =
        WeatherPointInsideClip(group->clipA, particle->position) || WeatherPointInsideClip(group->clipB, particle->position) ? 0x40000000U : 0x80000000U;
}
#endif

// @verified - d
void ResetWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle) {
    particle->flags = 0x40000000U;
    particle->update = SpawnWeatherRainParticle_Mode1WCCP;
}

// @verified - d
void InitializeWeatherRippleRenderState(WeatherParticleGroup *group, WeatherRippleParticleUpdate update) {
    group->ripplePacket.command = 0x80000002U;
    group->ripplePacket.isp = 0x88800000U;
    group->ripplePacket.tsp = 0x208000c0U;
    group->ripplePacket.tcw = 0;
    group->rippleFlags = 0x82000000U;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t *word = reinterpret_cast<std::uint32_t *>(group->rippleParticles);
    for (std::int32_t index = 0; index < group->rippleCapacity; ++index) {
        *word++ = 0x40000000U;
        *word++ = reinterpret_cast<std::uint32_t>(update);
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
        *word++ = 0;
    }
#else
    for (std::int32_t index = 0; index < group->rippleCapacity; ++index) {
        WeatherRippleParticle &particle = group->rippleParticles[index];
        particle.flags = 0x40000000U;
        particle.update = update;
        particle.position = __typeof__(particle.position)();
        particle.rotationPitch = 0;
        particle.rotationYaw = 0;
        particle.retainedRotation = 0;
        particle.colourRampEntry = 0;
        particle.age = 0.0f;
        particle.lifetime = 0.0f;
    }
#endif
    BindWeatherRippleTexture(group);
    SetWeatherRippleDepthEnabled(group, 1);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    do {
        WeatherRippleDrawState *const drawState = &group->rippleDrawState;
        drawState->flags = 0;
        drawState->firstParameter = drawState->distanceLimit = drawState->thirdParameter = 0.0f;
        drawState->controls[2] = 0;
        drawState->controls[1] = 0;
        drawState->controls[0] = 0;
        drawState->characterLayer = 0;
        drawState->retainedState = 0;
    } while (0);
#else
    group->rippleDrawState.flags = 0;
    group->rippleDrawState.thirdParameter = 0.0f;
    group->rippleDrawState.distanceLimit = 0.0f;
    group->rippleDrawState.firstParameter = 0.0f;
    group->rippleDrawState.controls[2] = 0;
    group->rippleDrawState.controls[1] = 0;
    group->rippleDrawState.controls[0] = 0;
    group->rippleDrawState.characterLayer = 0;
    group->rippleDrawState.retainedState = 0;
#endif
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    group->rippleDrawState.characterLayer = GetCharacterLayer(control->ryo);
    group->rippleDrawState.distanceLimit = 100.0f;
}

namespace {

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool ProbeWeatherRippleGround(WeatherRippleParticle *particle) {
    Vector3 query = {
        SHEN_FIELD(x) particle->position.x, SHEN_FIELD(y) weatherParticleGroundCameraPosition.y + 50.0f, SHEN_FIELD(z) particle->position.z + 0.25f};
    const float forward = FindFieldIntersectionHeight(&query);
    if (!(forward > -FLT_MAX))
        return false;
    query.z = particle->position.z - 0.25f;
    const float backward = FindFieldIntersectionHeight(&query);
    particle->rotationPitch = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(-(forward - backward), 0.5f));
    const std::int32_t pitch = static_cast<std::int16_t>(particle->rotationPitch);
    if ((pitch < 0 ? -pitch : pitch) > 0x1000)
        return false;
    query.x = particle->position.x + 0.25f;
    query.z = particle->position.z;
    const float right = FindFieldIntersectionHeight(&query);
    query.x = particle->position.x - 0.25f;
    const float left = FindFieldIntersectionHeight(&query);
    particle->rotationYaw = -static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(right - left, 0.5f));
    const std::int32_t yaw = static_cast<std::int16_t>(particle->rotationYaw);
    if ((yaw < 0 ? -yaw : yaw) > 0x1000)
        return false;
    query.x = particle->position.x;
    const float height = FindFieldIntersectionHeight(&query);
    particle->position.y = 0.02f + height;
    particle->rotationPitch -= 0x4000;
    return true;
}
#endif

} // namespace

// @verified - d
void SpawnWeatherRippleAtGround(WeatherRippleParticle *ripple, WeatherParticleGroup *group, WeatherRippleParticleUpdate update) {
    WeatherRippleParticle *const particle = ripple;
    Vector3 query;
    query.x = particle->position.x;
    query.y = weatherParticleGroundCameraPosition.y + 50.0f;
    query.z = particle->position.z + 0.25f;
    float forward = FindFieldIntersectionHeight(&query);
    if (!(forward > -FLT_MAX))
        return;
    query.z = particle->position.z - 0.25f;
    forward -= FindFieldIntersectionHeight(&query);
    particle->rotationPitch = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(-forward, 0.5f));
    const std::int32_t pitch = static_cast<std::int16_t>(particle->rotationPitch);
    if ((pitch < 0 ? -pitch : pitch) > 0x1000)
        return;
    query.x = particle->position.x + 0.25f;
    query.z = particle->position.z;
    const float right = FindFieldIntersectionHeight(&query);
    query.x = particle->position.x - 0.25f;
    const float left = FindFieldIntersectionHeight(&query);
    particle->rotationYaw = -static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(right - left, 0.5f));
    const std::int32_t yaw = static_cast<std::int16_t>(particle->rotationYaw);
    if ((yaw < 0 ? -yaw : yaw) > 0x1000)
        return;
    query.x = particle->position.x;
    const float height = FindFieldIntersectionHeight(&query);
    particle->position.y = 0.02f + height;
    particle->rotationPitch -= 0x4000;
    ++group->rippleLiveCount;
    particle->update = update;
    particle->age = 0.5f;
    particle->lifetime = 1.0f + dc::sdk::RandomFloat();
    const std::int32_t index = static_cast<std::int32_t>((particle->lifetime * 32.0f) * 0.5f);
    particle->age *= weatherRippleLifetimeScale;
    particle->lifetime *= weatherRippleLifetimeScale;
    particle->colourRampEntry = group->rippleColourRamp + index;
}

// @verified - d
void WeatherRippleParticleRespawnOrAge(WeatherRippleParticle *particle, WeatherParticleGroup *group, WeatherRippleParticleUpdate spawn) {
    if (group->rippleLiveCount >= group->rippleCurrentCount) {
        --group->rippleLiveCount;
        particle->update = spawn;
        particle->flags = 0x40000000U;
    } else {
        particle->age = 0.5f;
        particle->lifetime = 1.0f + dc::sdk::RandomFloat();
        const float amount = (particle->lifetime * 32.0f) * 0.5f;
        const std::int32_t index = static_cast<std::int32_t>(amount);
        particle->age *= weatherRippleLifetimeScale;
        particle->lifetime *= weatherRippleLifetimeScale;
        particle->colourRampEntry = group->rippleColourRamp + index;
    }
}

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WeatherRippleParticleSpawn_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
    float random = dc::sdk::RandomFloat();
    float position = group->boundsMinX;
    asm("fmac fr0,%1,%0" : "+f"(position) : "f"(group->frustumDistance), "w"(random));
    particle->position.x = position;
    random = dc::sdk::RandomFloat();
    position = group->boundsMinZ;
    asm("fmac fr0,%1,%0" : "+f"(position) : "f"(group->clipA.clipY1), "w"(random));
    particle->position.z = position;
    SpawnWeatherRippleAtGround(particle, group, WeatherRippleParticleUpdate_Direct);
}
#endif

// @verified - d
void WeatherRippleParticleSpawn_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    particle->position.x = dc::sdk::RandomFloat() * group->frustumDistance + group->boundsMinX;
    particle->position.z = dc::sdk::RandomFloat() * group->clipA.clipY1 + group->boundsMinZ;
#else
    float random = dc::sdk::RandomFloat();
    float position = group->boundsMinX;
    asm("fmac fr0,%1,%0" : "+f"(position) : "f"(group->frustumSpan), "w"(random));
    particle->position.x = position;
    random = dc::sdk::RandomFloat();
    position = group->boundsMinZ;
    asm("fmac fr0,%1,%0" : "+f"(position) : "f"(group->frustumSpan), "w"(random));
    particle->position.z = position;
#endif
    SpawnWeatherRippleAtGround(particle, group, WeatherRippleParticleUpdate_Mode0);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WeatherRippleParticleUpdate_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    WeatherRippleParticle *const visible = particle;
    if (!(particle->age > particle->lifetime)) {
        if (!(group->cameraY > particle->position.y)) {
            particle->flags = 0x40000000U;
        } else {
            visible->age += weatherRippleLifetimeStep;
            --visible->colourRampEntry;
            visible->flags = 0x80000000U;
        }
    } else {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Mode0);
    }
}
#endif

// @verified - d
void WeatherRippleParticleSpawn_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
    particle->position.x = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinX;
    particle->position.z = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinZ;
    SpawnWeatherRippleAtGround(particle, group, WeatherRippleParticleUpdate_Mode1);
}

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WeatherRippleParticleReprobe_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount || !ProbeWeatherRippleGround(particle)) {
        --group->rippleLiveCount;
        particle->update = WeatherRippleParticleSpawn_Mode0;
    } else {
        particle->update = WeatherRippleParticleUpdate_Mode0;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void WeatherRippleParticleReprobe_Mode1(WeatherRippleParticle *ripple, WeatherParticleGroup *group) {
    WeatherRippleParticle *const particle = ripple;
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        goto reset;
    {
        Vector3 query;
        query.x = particle->position.x;
        query.y = weatherParticleGroundCameraPosition.y + 50.0f;
        query.z = particle->position.z + 0.25f;
        float forward = FindFieldIntersectionHeight(&query);
        if (!(forward > -FLT_MAX))
            goto reset;
        query.z = particle->position.z - 0.25f;
        forward -= FindFieldIntersectionHeight(&query);
        particle->rotationPitch = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(-forward, 0.5f));
        const std::int32_t pitch = static_cast<std::int16_t>(particle->rotationPitch);
        if ((pitch < 0 ? -pitch : pitch) > 0x1000)
            goto reset;
        query.x = particle->position.x + 0.25f;
        query.z = particle->position.z;
        const float right = FindFieldIntersectionHeight(&query);
        query.x = particle->position.x - 0.25f;
        const float left = FindFieldIntersectionHeight(&query);
        particle->rotationYaw = -static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(right - left, 0.5f));
        const std::int32_t yaw = static_cast<std::int16_t>(particle->rotationYaw);
        if ((yaw < 0 ? -yaw : yaw) > 0x1000)
            goto reset;
        query.x = particle->position.x;
        const float height = FindFieldIntersectionHeight(&query);
        particle->position.y = 0.02f + height;
        particle->rotationPitch -= 0x4000;
        particle->update = WeatherRippleParticleUpdate_Mode1;
        return;
    }
reset:
    --group->rippleLiveCount;
    particle->update = WeatherRippleParticleSpawn_Mode1;
}
#else
// @verified - d
void WeatherRippleParticleReprobe_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount || !ProbeWeatherRippleGround(particle)) {
        --group->rippleLiveCount;
        particle->update = WeatherRippleParticleSpawn_Mode1;
    } else {
        particle->update = WeatherRippleParticleUpdate_Mode1;
    }
}
#endif

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WeatherRippleParticleUpdate_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (particle->age > particle->lifetime) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Direct);
    } else if (group->cameraY > particle->position.y) {
        particle->age += weatherRippleLifetimeStep;
        --particle->colourRampEntry;
        particle->flags = 0x80000000U;
    } else {
        particle->flags = 0x40000000U;
    }
}

// @verified - d
void WeatherRippleParticleUpdate_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (!(particle->lifetime > particle->age)) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Mode0);
        return;
    }
    particle->age += weatherRippleLifetimeStep;
    --particle->colourRampEntry;
    particle->flags = 0x40000000U;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (group->boundsMinX > particle->position.x) {
        particle->position.x += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (particle->position.z > group->boundsMaxZ) {
        particle->position.z -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (group->boundsMinZ > particle->position.z) {
        particle->position.z += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (group->cameraY > particle->position.y)
        particle->flags = 0x80000000U;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WeatherRippleParticleUpdate_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    WeatherRippleParticle *const ripple = particle;
    if (particle->age < particle->lifetime) {
        particle->age += weatherRippleLifetimeStep;
        --particle->colourRampEntry;
        particle->flags = 0x40000000U;
        if (ripple->position.x > group->boundsMaxX) {
            ripple->position.x -= group->frustumSpan;
            ripple->update = WeatherRippleParticleReprobe_Mode1;
        } else if (ripple->position.x < group->boundsMinX) {
            ripple->position.x += group->frustumSpan;
            ripple->update = WeatherRippleParticleReprobe_Mode1;
        } else if (ripple->position.z > group->boundsMaxZ) {
            ripple->position.z -= group->frustumSpan;
            ripple->update = WeatherRippleParticleReprobe_Mode1;
        } else if (ripple->position.z < group->boundsMinZ) {
            ripple->position.z += group->frustumSpan;
            ripple->update = WeatherRippleParticleReprobe_Mode1;
        } else if (group->cameraY > ripple->position.y) {
            ripple->flags = 0x80000000U;
        }
    } else {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Mode1);
    }
}
#else
// @verified - d
void WeatherRippleParticleUpdate_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (!(particle->lifetime > particle->age)) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Mode1);
        return;
    }
    particle->age += weatherRippleLifetimeStep;
    --particle->colourRampEntry;
    particle->flags = 0x40000000U;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (group->boundsMinX > particle->position.x) {
        particle->position.x += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (particle->position.z > group->boundsMaxZ) {
        particle->position.z -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (group->boundsMinZ > particle->position.z) {
        particle->position.z += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (group->cameraY > particle->position.y) {
        particle->flags = 0x80000000U;
        if (WeatherPointInsideClipPlanes(group->clipA, particle->position) || WeatherPointInsideClipPlanes(group->clipB, particle->position))
            particle->flags = 0x40000000U;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WeatherRippleParticleSpawn_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
    particle->position.x = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinX;
    particle->position.z = dc::sdk::RandomFloat() * group->frustumSpan + group->boundsMinZ;
    SpawnWeatherRippleAtGround(particle, group, WeatherRippleParticleUpdate_Direct);
}

void WeatherRippleParticleReprobe_Direct(WeatherRippleParticle *ripple, WeatherParticleGroup *group) {
    WeatherRippleParticle *const particle = ripple;
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        goto reset;
    {
        Vector3 query;
        query.x = particle->position.x;
        query.y = weatherParticleGroundCameraPosition.y + 50.0f;
        query.z = particle->position.z + 0.25f;
        float forward = FindFieldIntersectionHeight(&query);
        if (!(forward > -FLT_MAX))
            goto reset;
        query.z = particle->position.z - 0.25f;
        forward -= FindFieldIntersectionHeight(&query);
        particle->rotationPitch = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(-forward, 0.5f));
        const std::int32_t pitch = static_cast<std::int16_t>(particle->rotationPitch);
        if ((pitch < 0 ? -pitch : pitch) > 0x1000)
            goto reset;
        query.x = particle->position.x + 0.25f;
        query.z = particle->position.z;
        const float right = FindFieldIntersectionHeight(&query);
        query.x = particle->position.x - 0.25f;
        const float left = FindFieldIntersectionHeight(&query);
        particle->rotationYaw = -static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(right - left, 0.5f));
        const std::int32_t yaw = static_cast<std::int16_t>(particle->rotationYaw);
        if ((yaw < 0 ? -yaw : yaw) > 0x1000)
            goto reset;
        query.x = particle->position.x;
        const float height = FindFieldIntersectionHeight(&query);
        particle->position.y = 0.02f + height;
        particle->rotationPitch -= 0x4000;
        particle->update = WeatherRippleParticleUpdate_Direct;
        return;
    }
reset:
    --group->rippleLiveCount;
    particle->update = WeatherRippleParticleSpawn_Direct;
}

void WeatherRippleParticleUpdate_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (!(particle->lifetime > particle->age)) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Direct);
        return;
    }
    particle->age += weatherRippleLifetimeStep;
    --particle->colourRampEntry;
    particle->flags = 0x40000000U;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Direct;
        return;
    }
    if (group->boundsMinX > particle->position.x) {
        particle->position.x += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Direct;
        return;
    }
    if (particle->position.z > group->boundsMaxZ) {
        particle->position.z -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Direct;
        return;
    }
    if (group->boundsMinZ > particle->position.z) {
        particle->position.z += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Direct;
        return;
    }
    if (group->cameraY > particle->position.y) {
        particle->flags = 0x80000000U;
        if (WeatherPointInsideClipPlanes(group->clipA, particle->position) || WeatherPointInsideClipPlanes(group->clipB, particle->position))
            particle->flags = 0x40000000U;
    }
}
#endif

// @verified - d
void UpdateWeatherParticleGroundCameraPosition() {
    camCtrlGetNextCamPos(&weatherParticleGroundCameraPosition);
}

} // namespace s2
} // namespace shenmue

#endif
