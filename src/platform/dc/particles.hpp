#pragma once

#include "platform/dc/sdk.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || (SHENMUE_IS_DC && SHENMUE_IS_S2))

namespace shenmue { namespace dc { namespace sdk {

struct ParticleChannel {
    std::uint32_t flags;
    std::int32_t count;
    void *particles;
    ModelPolygonContext context;
    Callback callback;
    std::int32_t spawnProbabilityQ15;
    Vector3 position;
#if SHENMUE_IS_S2
    Vector3 velocity;
    Vector3 range;
#else
    Vector3 range;
    Vector3 velocity;
#endif
    Vector3 velocityRange;
    float scaleX;
    float scaleY;
    std::uint32_t colour;
    void *textureSurface;
    union {
        std::uint32_t *texturePixels;
        void *user;
    };
};
typedef char ParticleChannelLayoutCheck1[(sizeof(ParticleChannel) == 0x68) ? 1 : -1];
typedef char ParticleChannelLayoutCheck2[(offsetof(ParticleChannel, callback) == 0x1c) ? 1 : -1];
#if SHENMUE_IS_S2
typedef char ParticleChannelLayoutCheck3[(offsetof(ParticleChannel, position) == 0x24) ? 1 : -1];
typedef char ParticleChannelLayoutCheck4[(offsetof(ParticleChannel, velocity) == 0x30) ? 1 : -1];
typedef char ParticleChannelLayoutCheck5[(offsetof(ParticleChannel, range) == 0x3c) ? 1 : -1];
typedef char ParticleChannelLayoutCheck6[(offsetof(ParticleChannel, velocityRange) == 0x48) ? 1 : -1];
#endif
typedef char ParticleChannelLayoutCheck7[(offsetof(ParticleChannel, scaleX) == 0x54) ? 1 : -1];
typedef char ParticleChannelLayoutCheck8[(offsetof(ParticleChannel, textureSurface) == 0x60) ? 1 : -1];

void ParticleChannelCallbackNullsub();
static const ParticleChannel kDefaultParticleChannel = {0, 0, 0,
    {0, 0x8a000000U, 0x208000c0U, 0}, ParticleChannelCallbackNullsub, 0,
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 1.0f, 1.0f, 0xffffffffU, 0, {0}};
extern ParticleChannel g_particleChannels[16] asm("_shenmue_dc_particle_channels");
extern float g_particleClipDepthThreshold asm("_shenmue_dc_particle_depth_threshold");
void DrawParticleChannels() asm("_shenmue_dc_draw_particle_channels");
void ReleaseParticleChannelTextures();
void ReleaseParticleChannelTexture(std::int32_t index);
void SetParticleChannelActive(std::int32_t index, std::int32_t active);
#if !SHENMUE_IS_S2 || (SHENMUE_IS_DC && SHENMUE_IS_S2)
void SetParticleChannelSpawnProbability(std::int32_t index, float probability);
#endif

} } } // namespace shenmue::dc::sdk

#endif
