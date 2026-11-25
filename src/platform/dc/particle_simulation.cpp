#include "platform/dc/particle_simulation.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#include "platform/dc/ninja.hpp"
#include "common/types.hpp"
#if !SHENMUE_IS_S2
#include <bit>
#endif
#include <cmath>

namespace shenmue {
namespace dc {
namespace sdk {

std::uint32_t g_particleSpawnSeed = 1;
#if SHENMUE_IS_S2
std::uint32_t g_particleInterpolatedSpawnSeed = 1;
#endif
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
ParticleTextureSurface g_particleTextureSurfaces[16] = {};
#else
KMSURFACEDESC g_particleTextureSurfaces[16] = {};
#endif

// @verified - d
void SpawnInactiveParticle(Particle *particle, ParticleChannel *channel) {
    g_particleSpawnSeed *= 0x41c64e6dU;
    if (channel->spawnProbabilityQ15 <= static_cast<std::int32_t>((g_particleSpawnSeed >> 16) & 0x7fffU))
        return;
    particle->update = reinterpret_cast<ParticleUpdate>(channel->callback);
    float offsets[6];
    for (std::int32_t axis = 0; axis < 6; ++axis) {
        g_particleSpawnSeed *= 0x41c64e6dU;
        std::uint32_t sum = (g_particleSpawnSeed >> 16) & 0x7fffU;
        for (std::int32_t shift = 1; shift <= 5; ++shift)
            sum += (g_particleSpawnSeed >> shift) & 0x7fffU;
        offsets[axis] = static_cast<float>(sum) *
#if SHENMUE_IS_S2
                u32AsFloat(0x37fffffcU)
#else
                std::bit_cast<float>(0x37fffffcU)
#endif
            - 3.0f;
    }
#if SHENMUE_IS_S2
    particle->position.x = channel->range.x * offsets[0] + channel->position.x;
    particle->position.y = channel->range.y * offsets[1] + channel->position.y;
    particle->position.z = channel->range.z * offsets[2] + channel->position.z;
    particle->velocity.x = channel->velocityRange.x * offsets[3] + channel->velocity.x;
    particle->velocity.y = channel->velocityRange.y * offsets[4] + channel->velocity.y;
    particle->velocity.z = channel->velocityRange.z * offsets[5] + channel->velocity.z;
#else
    particle->position.x = std::fma(channel->range.x, offsets[0], channel->position.x);
    particle->position.y = std::fma(channel->range.y, offsets[1], channel->position.y);
    particle->position.z = std::fma(channel->range.z, offsets[2], channel->position.z);
    particle->velocity.x = std::fma(channel->velocityRange.x, offsets[3], channel->velocity.x);
    particle->velocity.y = std::fma(channel->velocityRange.y, offsets[4], channel->velocity.y);
    particle->velocity.z = std::fma(channel->velocityRange.z, offsets[5], channel->velocity.z);
#endif
    if (!(channel->flags & 0x20000000U))
        particle->colour = channel->colour;
}

#if SHENMUE_IS_S2
void SpawnInterpolatedParticle(Particle *particle, ParticleChannel *channel) {
    g_particleInterpolatedSpawnSeed *= 0x41c64e6dU;
    if (channel->spawnProbabilityQ15 <= static_cast<std::int32_t>((g_particleInterpolatedSpawnSeed >> 16) & 0x7fffU))
        return;
    particle->update = reinterpret_cast<ParticleUpdate>(channel->callback);
    float offsets[6];
    for (std::int32_t axis = 0; axis < 6; ++axis) {
        g_particleInterpolatedSpawnSeed *= 0x41c64e6dU;
        std::uint32_t sum = (g_particleInterpolatedSpawnSeed >> 16) & 0x7fffU;
        for (std::int32_t shift = 1; shift <= 5; ++shift)
            sum += (g_particleInterpolatedSpawnSeed >> shift) & 0x7fffU;
        offsets[axis] = static_cast<float>(sum) * u32AsFloat(0x37fffffcU) - 3.0f;
    }
    particle->position.x = (channel->range.x - channel->position.x) * offsets[0] + channel->position.x;
    particle->position.y = (channel->range.y - channel->position.y) * offsets[1] + channel->position.y;
    particle->position.z = (channel->range.z - channel->position.z) * offsets[2] + channel->position.z;
    particle->velocity.x = channel->velocityRange.x * offsets[3] + channel->velocity.x;
    particle->velocity.y = channel->velocityRange.y * offsets[4] + channel->velocity.y;
    particle->velocity.z = channel->velocityRange.z * offsets[5] + channel->velocity.z;
    if (!(channel->flags & 0x20000000U))
        particle->colour = channel->colour;
}

// @verified - d
void InitializeParticleChannelParticles(std::int32_t index, std::int32_t count, std::int32_t compact, void *particles) {
    g_particleChannels[index].count = count;
    g_particleChannels[index].particles = particles;
    if (compact == 1) {
        g_particleChannels[index].flags |= 0x20000000U;
        g_particleChannels[index].context.flags = 0xa0000002U;
        for (std::int32_t particle = 0; particle < count; ++particle)
            static_cast<CompactParticle *>(particles)[particle] = kDefaultCompactParticle;
    } else {
        g_particleChannels[index].flags &= ~0x20000000U;
        g_particleChannels[index].context.flags = 0x80000002U;
        for (std::int32_t particle = 0; particle < count; ++particle)
            static_cast<Particle *>(particles)[particle] = kDefaultParticle;
    }
    g_particleChannels[index].context.isp = 0x8a000000U;
    g_particleChannels[index].context.isp = (g_particleChannels[index].context.isp & 0xfc3fffffU) | ((g_particleChannels[index].context.flags & 15U) << 22);
}

void InitializeParticleChannelSpawnParticles(std::int32_t index, std::int32_t count, std::int32_t compact, void *particles, std::int32_t spawnMode) {
    std::int32_t particle;
    g_particleChannels[index].count = count;
    g_particleChannels[index].particles = particles;
    if (compact == 1) {
        g_particleChannels[index].flags |= 0x20000000U;
        g_particleChannels[index].context.flags = 0xa0000002U;
        particle = 0;
        if (spawnMode == 0) {
            for (; particle < count; ++particle)
                static_cast<CompactParticle *>(particles)[particle] = kDefaultCompactParticle;
        } else if (spawnMode == 1) {
            for (; particle < count; ++particle)
                static_cast<CompactParticle *>(particles)[particle] = kInterpolatedCompactParticle;
        }
    } else {
        g_particleChannels[index].flags &= ~0x20000000U;
        g_particleChannels[index].context.flags = 0x80000002U;
        particle = 0;
        if (spawnMode == 0) {
            for (; particle < count; ++particle)
                static_cast<Particle *>(particles)[particle] = kDefaultParticle;
        } else if (spawnMode == 1) {
            for (; particle < count; ++particle)
                static_cast<Particle *>(particles)[particle] = kInterpolatedParticle;
        }
    }
    g_particleChannels[index].context.isp = 0x8a000000U;
    g_particleChannels[index].context.isp = (g_particleChannels[index].context.isp & 0xfc3fffffU) | ((g_particleChannels[index].context.flags & 15U) << 22);
}
#else
// @verified - d
void InitializeParticleChannelParticles(std::int32_t index, std::int32_t count, std::int32_t compact, void *particles) {
    ParticleChannel &channel =
        g_particleChannels[index];
    channel.count = count;
    channel.particles = particles;
    if (compact == 1) {
        channel.flags |= 0x20000000U;
        channel.context.flags = 0xa0000002U;
        for (std::int32_t particle = 0; particle < count; ++particle)
            static_cast<CompactParticle *>(particles)[particle] = kDefaultCompactParticle;
    } else {
        channel.flags &= ~0x20000000U;
        channel.context.flags = 0x80000002U;
        for (std::int32_t particle = 0; particle < count; ++particle)
            static_cast<Particle *>(particles)[particle] = kDefaultParticle;
    }
    channel.context.isp = 0x8a000000U;
    channel.context.isp = (channel.context.isp & 0xfc3fffffU) | ((channel.context.flags & 15U) << 22);
}
#endif

// @verified - d
void SetParticleChannelCallback(std::int32_t index, ParticleUpdate callback) {
    g_particleChannels[index].callback = reinterpret_cast<Callback>(callback);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetParticleClipDepthThreshold(float threshold) {
    g_particleClipDepthThreshold = threshold;
}

// @verified - d
void SetParticleChannelColour(std::int32_t index, std::uint32_t colour) {
    g_particleChannels[index].colour = colour;
}

void SetParticleChannelDrawingEnabled(std::int32_t index, std::int32_t enabled) {
    if (enabled)
        g_particleChannels[index].flags &= ~0x04000000U;
    else
        g_particleChannels[index].flags |= 0x04000000U;
}
#endif

// @verified - d
void SetParticleChannelBlendMode(std::int32_t index, std::int32_t mode) {
    ParticleChannel &channel =
        g_particleChannels[index];
    if (mode) {
        channel.context.flags |= 0x02000000U;
        channel.context.tsp = ((channel.context.tsp | 0x00100000U) & 0x00ffffffU) | 0x94000000U;
        channel.flags |= 0x40000000U;
    } else {
        channel.context.flags &= ~0x02000000U;
#if SHENMUE_IS_S2
        channel.context.tsp = ((channel.context.tsp & 0xffefffffU) & 0x00ffffffU) | 0x20000000U;
#else
        channel.context.tsp = (channel.context.tsp & 0x00efffffU) | 0x20000000U;
#endif
        channel.flags &= ~0x40000000U;
    }
}

// @verified - d
void SetParticleChannelDepthMode(std::int32_t index, std::int32_t enabled) {
    if (enabled)
        g_particleChannels[index].context.tsp = (g_particleChannels[index].context.tsp & 0xffff9fffU) | 0x2000U;
    else
        g_particleChannels[index].context.tsp &= 0xffff9fffU;
}

#if SHENMUE_IS_S2
// @verified - d
std::int32_t SetParticleChannelTexture(std::int32_t index, std::int32_t width, std::int32_t height, std::uint32_t format, std::uint32_t *pixels) {
    std::uint32_t texture = 0;
    if (g_particleChannels[index].textureSurface)
        kmFreeTexture(static_cast<KMSURFACEDESC *>(g_particleChannels[index].textureSurface));
    else
        g_particleChannels[index].textureSurface = &g_particleTextureSurfaces[index].surface;
    if (kmCreateTextureSurface(static_cast<KMSURFACEDESC *>(g_particleChannels[index].textureSurface), width, height, static_cast<KMTEXTURETYPE>(format)))
        return 0;
    if (kmLoadTexture(static_cast<KMSURFACEDESC *>(g_particleChannels[index].textureSurface), pixels))
        return 0;
    if (g_particleChannels[index].flags & 0x20000000U)
        g_particleChannels[index].context.flags = 0xa0000002U;
    else
        g_particleChannels[index].context.flags = 0x80000002U;
    std::int32_t bits = 0;
    for (; width; width >>= 1)
        ++bits;
    bits -= 4;
    std::uint32_t size = bits << 3;
    bits = 0;
    for (; height; height >>= 1)
        ++bits;
    bits -= 4;
    size = (size & ~7U) | bits;
    texture = (texture & 0x7fffffffU) | (g_particleTextureSurfaces[index].surface.fSurfaceFlags << 31);
    texture = (texture & 0xbfffffffU) | ((g_particleTextureSurfaces[index].surface.fSurfaceFlags & 8U) << 27);
    texture = (texture & 0xc7ffffffU) | (g_particleTextureSurfaces[index].surface.PixelFormat & 0x38000000U);
    texture = (texture & 0xfbffffffU) | ((~(g_particleTextureSurfaces[index].surface.fSurfaceFlags & 4U) & 4U) << 24);
    texture = (texture & 0xfdffffffU) | ((g_particleTextureSurfaces[index].surface.fSurfaceFlags & 0x10U) << 21);
    texture = (texture & 0xfeffffffU) | ((g_particleTextureSurfaces[index].surface.fSurfaceFlags & 2U) << 23);
    texture = (texture & 0xff800000U) | (reinterpret_cast<std::uintptr_t>(g_particleTextureSurfaces[index].surface.pSurface) >> 3);
    g_particleChannels[index].context.flags |= 8U;
    g_particleChannels[index].context.isp = 0x8a800000U;
    g_particleChannels[index].context.tsp = size | 0x940242c0U;
    g_particleChannels[index].context.texture = texture;
    SetParticleChannelBlendMode(index, g_particleChannels[index].flags & 0x40000000U);
    g_particleChannels[index].texturePixels = pixels;
    g_particleChannels[index].flags |= 0x10000000U;
    return 1;
}
#else
// @verified - d
std::int32_t SetParticleChannelTexture(std::int32_t index, std::int32_t width, std::int32_t height, std::uint32_t format, std::uint32_t *pixels) {
    ParticleChannel &channel =
        g_particleChannels[index];
    KMSURFACEDESC *surface = static_cast<KMSURFACEDESC *>(channel.textureSurface);
    if (surface)
        kmFreeTexture(surface);
    else {
#if SHENMUE_IS_S2
        surface = &g_particleTextureSurfaces[index].surface;
#else
        surface = &g_particleTextureSurfaces[index];
#endif
        channel.textureSurface = surface;
    }
    if (kmCreateTextureSurface(surface, width, height, static_cast<KMTEXTURETYPE>(format)))
        return 0;
    if (kmLoadTexture(surface, pixels))
        return 0;
    channel.context.flags = (channel.flags & 0x20000000U) ? 0xa0000002U : 0x80000002U;
    std::uint32_t widthBits = 0;
    std::uint32_t heightBits = 0;
    for (; width; width >>= 1)
        ++widthBits;
    for (; height; height >>= 1)
        ++heightBits;
    const std::uint32_t size = ((widthBits - 4U) << 3) | (heightBits - 4U);
#if SHENMUE_IS_S2
    surface = &g_particleTextureSurfaces[index].surface;
#endif
    const std::uint32_t flags = surface->fSurfaceFlags;
    std::uint32_t texture = ((flags & 1U) << 31) | ((flags & 8U) << 27);
    texture = (texture & 0xc7ffffffU) | (surface->PixelFormat & 0x38000000U);
    texture = (texture & 0xfbffffffU) | ((~flags & 4U) << 24);
    texture = (texture & 0xfdffffffU) | ((flags & 0x10U) << 21);
    texture = (texture & 0xfeffffffU) | ((flags & 2U) << 23);
    texture = (texture & 0xff800000U) | (reinterpret_cast<std::uintptr_t>(surface->pSurface) >> 3);
    channel.context.flags |= 8U;
    channel.context.isp = 0x8a800000U;
    channel.context.tsp = size | 0x940242c0U;
    channel.context.texture = texture;
    SetParticleChannelBlendMode(index, channel.flags & 0x40000000U);
    channel.texturePixels = pixels;
    channel.flags |= 0x10000000U;
    return 1;
}
#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace shenmue {
namespace dc {
namespace sdk {

// @verified - d
void UpdateParticleChannel(std::int32_t index) {
    typedef char ParticleSurfaceLayout66[(sizeof(KMSURFACEDESC) == 0x20) ? 1 : -1];
    typedef char ParticleSurfaceLayout116[(offsetof(KMSURFACEDESC, fSurfaceFlags) == 0x18) ? 1 : -1];
    typedef char ParticleSurfaceLayout183[(offsetof(KMSURFACEDESC, pSurface) == 0x1c) ? 1 : -1];
#if SHENMUE_IS_S2
    kmLoadTexture(static_cast<KMSURFACEDESC *>(g_particleChannels[index].textureSurface), g_particleChannels[index].texturePixels);
#else
    const std::int16_t offset = static_cast<std::int16_t>(static_cast<std::int16_t>(index) * 0x68);
    const ParticleChannel &channel = *reinterpret_cast<const ParticleChannel *>(reinterpret_cast<const std::byte *>(g_particleChannels) + offset);
    kmLoadTexture(static_cast<KMSURFACEDESC *>(channel.textureSurface), channel.texturePixels);
#endif
}

} // namespace sdk
} // namespace dc
} // namespace shenmue
#endif
