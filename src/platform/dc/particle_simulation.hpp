#pragma once
#include "platform/dc/particles.hpp"
#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || (SHENMUE_IS_DC && SHENMUE_IS_S2))
#include "platform/dc/ninja.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
struct ParticleTextureSurface {
    KMSURFACEDESC surface;
    std::uint8_t retained20[8];
};
typedef char ParticleSimulationLayoutCheck1[(sizeof(ParticleTextureSurface) == 0x28) ? 1 : -1];
extern ParticleTextureSurface g_particleTextureSurfaces[16];
#else
extern KMSURFACEDESC g_particleTextureSurfaces[16];
#endif
struct Particle;
typedef void (*ParticleUpdate)(Particle *, ParticleChannel *);
struct Particle {
    std::uint32_t flags;
    Vector3 position;
    Vector3 velocity;
    ParticleUpdate update;
    std::int32_t phase0;
    std::int32_t phase1;
    std::int32_t phase2;
    std::int32_t attribute;
    std::int32_t phaseStep0;
    std::int32_t phaseStep1;
    std::uint32_t colour;
    float scale;
};
struct CompactParticle {
    std::uint32_t flags;
    Vector3 position;
    Vector3 velocity;
    ParticleUpdate update;
};
typedef char ParticleSimulationLayoutCheck2[(sizeof(Particle) == 0x40) ? 1 : -1];
typedef char ParticleSimulationLayoutCheck3[(offsetof(Particle, update) == 0x1c) ? 1 : -1];
typedef char ParticleSimulationLayoutCheck4[(offsetof(Particle, colour) == 0x38) ? 1 : -1];
typedef char ParticleSimulationLayoutCheck5[(sizeof(CompactParticle) == 0x20) ? 1 : -1];
void SpawnInactiveParticle(Particle *particle, ParticleChannel *channel);
static const Particle kDefaultParticle = {0, {0, 0, 0}, {0, 0, 0}, SpawnInactiveParticle, 0, 0, 0, 0, 0, 0, 0xffffffffU, 1.0f};
static const CompactParticle kDefaultCompactParticle = {0xffffffU, {0, 0, 0}, {0, 0, 0}, SpawnInactiveParticle};
#if SHENMUE_IS_S2
void SpawnInterpolatedParticle(Particle *particle, ParticleChannel *channel);
static const Particle kInterpolatedParticle = {0, {0, 0, 0}, {0, 0, 0}, SpawnInterpolatedParticle, 0, 0, 0, 0, 0, 0, 0xffffffffU, 1.0f};
static const CompactParticle kInterpolatedCompactParticle = {0xffffffU, {0, 0, 0}, {0, 0, 0}, SpawnInterpolatedParticle};
#endif
extern std::uint32_t g_particleSpawnSeed;
#if SHENMUE_IS_S2
extern std::uint32_t g_particleInterpolatedSpawnSeed;
#endif
void InitializeParticleChannelParticles(std::int32_t index, std::int32_t count, std::int32_t compact, void *particles);
#if SHENMUE_IS_S2
void InitializeParticleChannelSpawnParticles(std::int32_t index, std::int32_t count, std::int32_t compact, void *particles, std::int32_t spawnMode);
#endif
void SetParticleChannelCallback(std::int32_t index, ParticleUpdate callback);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetParticleChannelColour(std::int32_t index, std::uint32_t colour);
#endif
void SetParticleChannelPositionRange(std::int32_t index, float x, float y, float z, float rangeX, float rangeY, float rangeZ);
void SetParticleChannelVelocityRange(std::int32_t index, float x, float y, float z, float rangeX, float rangeY, float rangeZ);
void SetParticleChannelSize(std::int32_t index, float width, float height);
std::int32_t SetParticleChannelTexture(std::int32_t index, std::int32_t width, std::int32_t height, std::uint32_t format, std::uint32_t *pixels);
void SetParticleChannelBlendMode(std::int32_t index, std::int32_t mode);
void SetParticleChannelDepthMode(std::int32_t index, std::int32_t enabled);
} // namespace sdk
} // namespace dc
} // namespace shenmue
#endif
