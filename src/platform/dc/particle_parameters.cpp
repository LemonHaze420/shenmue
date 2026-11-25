#include "platform/dc/particle_simulation.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || (SHENMUE_IS_DC && SHENMUE_IS_S2))

#include "platform/dc/ninja.hpp"

namespace shenmue {
namespace dc {
namespace sdk {

float g_particleClipDepthThreshold = 0.0f;

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
ParticleChannel g_particleChannels[16] = {};
#endif

// @verified - d
void SetParticleChannelActive(std::int32_t index, std::int32_t active) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (active)
        g_particleChannels[index].flags |= 0x80000000U;
    else
        g_particleChannels[index].flags &= ~0x80000000U;
#else
    ParticleChannel &channel =
        g_particleChannels[index]
        ;
    std::uint32_t &flags = channel.flags;
    if (active)
        flags |= 0x80000000U;
    else
        flags &= ~0x80000000U;
#endif
}

#if !SHENMUE_IS_S2 || (SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
void SetParticleChannelSpawnProbability(std::int32_t index, float probability) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    g_particleChannels[index].spawnProbabilityQ15 = static_cast<std::int32_t>(probability * 32767.0f);
#else
    ParticleChannel &channel =
        g_particleChannels[index];
#if SHENMUE_IS_S2
    channel.spawnProbabilityQ15 = TruncateToInt32(probability * 32767.0f);
#else
    channel.spawnProbabilityQ15 = static_cast<std::int32_t>(probability * 32767.0f);
#endif
#endif
}

#endif

// @verified - d
void SetParticleChannelPositionRange(std::int32_t index, float x, float y, float z, float rangeX, float rangeY, float rangeZ) {

    ParticleChannel &channel =
        g_particleChannels[index]
        ;
    channel.position = (Vector3){x, y, z};
    channel.range = (Vector3){rangeX, rangeY, rangeZ};
}

// @verified - d
void SetParticleChannelVelocityRange(std::int32_t index, float x, float y, float z, float rangeX, float rangeY, float rangeZ) {

    ParticleChannel &channel =
        g_particleChannels[index]
        ;
    channel.velocity = (Vector3){x, y, z};
    channel.velocityRange = (Vector3){rangeX, rangeY, rangeZ};
}

// @verified - d
void SetParticleChannelSize(std::int32_t index, float width, float height) {
    ParticleChannel &channel =
        g_particleChannels[index]
        ;
    if (!(channel.flags & 0x20000000U)) {
#if SHENMUE_IS_S2
        width *= 0.5f;
        height *= 0.5f;
#else
        width /= 2.0f;
        height /= 2.0f;
#endif
    }
    channel.scaleX = width;
    channel.scaleY = height;
}

#if SHENMUE_IS_S2
void SetParticleChannelEnabled(std::int32_t index, std::int32_t enabled) {
    if (enabled)
        g_particleChannels[index].flags &= ~0x02000000U;
    else
        g_particleChannels[index].flags |= 0x02000000U;
}
#endif

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
// @verified - d
void ReleaseParticleChannelTexture(std::int32_t index) {
#if SHENMUE_IS_S2
    ParticleChannel &channel = g_particleChannels[index];
#else
    ParticleChannel &channel = *reinterpret_cast<ParticleChannel *>(
        reinterpret_cast<std::byte *>(g_particleChannels) + static_cast<std::int16_t>(static_cast<std::uint32_t>(index) * sizeof(ParticleChannel)));
#endif
    if (channel.textureSurface) {
        kmFreeTexture(static_cast<KMSURFACEDESC *>(channel.textureSurface));
        channel.textureSurface = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (channel.flags & 1U)
            channel.context.flags = 0xa0000002U;
        else
            channel.context.flags = 0x80000002U;
        channel.context.isp = 0x8a000000U & 0xfc3fffffU;
        channel.context.isp |= (channel.context.flags & 15U) << 22;
#else
        channel.context.flags = channel.flags & 1U ? 0xa0000002U : 0x80000002U;
        channel.context.isp = 0x8a000000U;
        channel.context.isp = (channel.context.flags & 15U) << 22 | (channel.context.isp & 0xfc3fffffU);
#endif
        channel.context.tsp = 0x208000c0U;
        channel.context.texture = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ReleaseParticleChannelTextures() {
    ParticleChannel *channel = g_particleChannels;
    for (std::int32_t index = 0; index < 16; ++index, ++channel) {
        if (channel->textureSurface) {
            kmFreeTexture(static_cast<KMSURFACEDESC *>(channel->textureSurface));
            channel->textureSurface = 0;
            if (channel->flags & 1U)
                channel->context.flags = 0xa0000002U;
            else
                channel->context.flags = 0x80000002U;
            channel->context.isp = 0x8a000000U & 0xfc3fffffU;
            channel->context.isp |= (channel->context.flags & 15U) << 22;
            channel->context.tsp = 0x208000c0U;
            channel->context.texture = 0;
        }
    }
}
#else
// @verified - d
void ReleaseParticleChannelTextures() {
    for (std::size_t index = 0; index < 16; ++index) {
        ParticleChannel &channel = g_particleChannels[index];
        if (!channel.textureSurface)
            continue;
        kmFreeTexture(static_cast<KMSURFACEDESC *>(channel.textureSurface));
        channel.textureSurface = 0;
        channel.context.flags = channel.flags & 1U ? 0xa0000002U : 0x80000002U;
        channel.context.isp = 0x8a000000U;
        channel.context.isp = (channel.context.flags & 15U) << 22 | (channel.context.isp & 0xfc3fffffU);
        channel.context.tsp = 0x208000c0U;
        channel.context.texture = 0;
    }
}
#endif

// @verified - d
void ParticleChannelCallbackNullsub() {
}

// @verified - d
void InitializeParticleChannels() {
    for (std::int32_t index = 0; index < 16; ++index) {
        ParticleChannel *channels = g_particleChannels;
        channels[index] = kDefaultParticleChannel;
    }
    g_particleClipDepthThreshold = 0.0f;
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
