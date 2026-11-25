#pragma once

#include "platform/dc/particles.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#include "platform/dc/ninja.hpp"

namespace shenmue {
namespace dc {
namespace sdk {

struct FftEffect {
    std::uint32_t flags;
    std::uint16_t *pixels;
    std::uint16_t colour;
    std::uint16_t retained0a;
    float speed;
    float intensity;
    float phaseStepScale;
    float amplitudes[80];
    float phases[256];
    float phaseSteps[256];
    float real[17][17];
    float imaginary[17][17];
};
typedef char ParticleEffectsLayout577[(sizeof(FftEffect) == 0x1260) ? 1 : -1];
typedef char ParticleEffectsLayout621[(offsetof(FftEffect, phases) == 0x158) ? 1 : -1];
typedef char ParticleEffectsLayout674[(offsetof(FftEffect, phaseSteps) == 0x558) ? 1 : -1];
typedef char ParticleEffectsLayout731[(offsetof(FftEffect, real) == 0x958) ? 1 : -1];
typedef char ParticleEffectsLayout782[(offsetof(FftEffect, imaginary) == 0xddc) ? 1 : -1];

struct TerrainPhase {
    float amplitude;
    float first;
    float firstStep;
    float second;
    float secondStep;
};
struct TerrainEffect {
    std::uint32_t flags;
    std::uint32_t colour;
    std::byte retained08[0x58];
    TerrainPhase phases[8][8];
    float real[17][17];
    float imaginary[17][17];
};
struct TerrainChannel {
    KMSURFACEDESC surface;
    std::byte retained20[8];
    std::uint32_t context[4];
    std::uint32_t flags;
};
typedef char ParticleEffectsLayout1294[(sizeof(TerrainPhase) == 0x14) ? 1 : -1];
typedef char ParticleEffectsLayout1339[(sizeof(KMSURFACEDESC) == 0x20) ? 1 : -1];
typedef char ParticleEffectsLayout1385[(sizeof(TerrainEffect) == 0xe68) ? 1 : -1];
typedef char ParticleEffectsLayout1432[(offsetof(TerrainEffect, phases) == 0x60) ? 1 : -1];
typedef char ParticleEffectsLayout1488[(offsetof(TerrainEffect, real) == 0x560) ? 1 : -1];
typedef char ParticleEffectsLayout1543[(offsetof(TerrainEffect, imaginary) == 0x9e4) ? 1 : -1];
typedef char ParticleEffectsLayout1603[(sizeof(TerrainChannel) == 0x3c) ? 1 : -1];
typedef char ParticleEffectsLayout1650[(offsetof(TerrainChannel, context) == 0x28) ? 1 : -1];
typedef char ParticleEffectsLayout1708[(offsetof(TerrainChannel, flags) == 0x38) ? 1 : -1];

#if SHENMUE_IS_S2
static const float kFftPhaseTurn = 6.2831854820251465f;
static const float kFftPhasePi = 3.1415927410125732f;
static const float kFftTwiddleStep = 0.39269909262657166f;
#else
static const float kFftPhaseTurn = 6.283185005187988f;
static const float kFftPhasePi = 3.141592502593994f;
static const float kFftTwiddleStep = 0.39269906282424927f;
#endif
static const float kFftInverse16Scale = 0.0625f;
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
static const std::int32_t kFftPhaseModes[8][8] = {{0, 1, 1, 1, 2, 2, 2, 2}, {1, 2, 2, 2, 2, 2, 2, 2}, {1, 2, 2, 2, 2, 2, 2, 2}, {1, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
#endif

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
typedef FftEffect *FftEffectHandle;
#else
typedef std::int32_t FftEffectHandle;
extern FftEffect g_fftEffects[2];
#endif
static const float kFftDenormalEpsilon = 0.00000011920928955078125f;
extern TerrainEffect g_terrainEffects[2];
extern TerrainChannel g_terrainChannels[4];
#if SHENMUE_IS_S2
extern Vector2 g_fftForwardTwiddles[32];
extern Vector2 g_fftBackwardTwiddles[32];
float FftMultiplyAdd(float first, float second, float addend);
#else
extern Vector2 g_fftForwardTwiddles[32] asm("_shenmue_dc_fft_forward_twiddles");
extern Vector2 g_fftBackwardTwiddles[32] asm("_shenmue_dc_fft_backward_twiddles");
#endif
void InitializeFftTwiddles();
void InitializeFftPhaseSteps(float *steps, float scale);
void FftReverse16(Vector2 *values);
void FftRadix4(Vector2 *values, std::int32_t logSize, const Vector2 *twiddles);
void FftInverseHalves(Vector2 *values, std::int32_t logSize);
void FftInverseScale(Vector2 *values, std::int32_t logSize, const float *scale);
void FftInverse16(Vector2 *values);
#if SHENMUE_IS_S2
static const float kFftInverse32Scale = 0.03125f;
void InitializeFftTwiddles32();
void FftInverse32(Vector2 *values);
#endif
void SetFftEffectActive(FftEffectHandle handle, std::int32_t active);
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
void InitializeFftEffect(FftEffect *effect);
#endif
void UpdateFftEffect(FftEffectHandle handle);
void SetFftEffectPixels(FftEffectHandle handle, std::uint16_t *pixels);
void SetFftEffectColour(FftEffectHandle handle, std::int16_t colour);
float *BuildFftFalloff(float exponent, float *table);
float *ScaleFftFalloff(float scale, float *table);
void SetFftEffectFalloff(FftEffectHandle handle, const float *table);
void SetFftEffectPhaseScale(FftEffectHandle handle, float scale);
std::uint32_t GetTerrainTextureSize(std::uint32_t size);
std::uint32_t *SetTerrainChannelTexture(
    std::int8_t index, std::uint32_t format, std::int32_t width, std::int32_t height, std::uint32_t *pixels, std::uint32_t flags);

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
