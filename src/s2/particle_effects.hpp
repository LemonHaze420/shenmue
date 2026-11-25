#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/hlib.hpp"
#include "s2/effect_fft.hpp"
#include "platform/dc/particle_simulation.hpp"
#include "platform/dc/particle_effects.hpp"

#define SHENMUE_PARTICLE_LAYOUT_NAME_(line) ShenmueParticleLayoutCheck##line
#define SHENMUE_PARTICLE_LAYOUT_NAME(line) SHENMUE_PARTICLE_LAYOUT_NAME_(line)
#define SHENMUE_PARTICLE_LAYOUT(condition) typedef char SHENMUE_PARTICLE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace dc {
namespace sdk {
void InitializeFftEffect(FftEffect *effect);
}
} // namespace dc
} // namespace shenmue
namespace shenmue {
namespace s2 {
#define EFFECT_CONTROLLER_TASK shen_name(SHEN_NAME(EFFE))
#define PARTICLE_EFFECT_TASK shen_name(SHEN_NAME(EFPT))
#define PARTICLE_FFT_TASK shen_name(SHEN_NAME(PUFY))
#define FIRE_EFFECT_TASK shen_name(SHEN_NAME(FIRE))
#define FIRE_TRAIL_TASK shen_name(SHEN_NAME(FIRB))
#define FIRE_INSTANCE_TASK shen_name(SHEN_NAME(FIRC))
struct Character;
struct FireTrailEntry {
    std::int16_t age, heightOffset;
    Vector3 position;
};
SHENMUE_PARTICLE_LAYOUT(sizeof(FireTrailEntry) == 0x10);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireTrailEntry, age) == 0);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireTrailEntry, heightOffset) == 2);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireTrailEntry, position) == 4);
static const std::int32_t FIRE_TRAIL_INITIAL_AGES[10] = {7, 20, 18, 10, 12, 5, 3, 1, 4, 7};
struct FireInstance {
    std::uint16_t enabled;
    std::int16_t bone;
    Vector3 localPosition, worldPosition;
    float alpha, width, height;
    Character *character;
    float cameraOffset;
};
SHENMUE_PARTICLE_LAYOUT(sizeof(FireInstance) == 0x30);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, bone) == 2);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, localPosition) == 4);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, worldPosition) == 0x10);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, alpha) == 0x1c);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, width) == 0x20);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, height) == 0x24);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, character) == 0x28);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireInstance, cameraOffset) == 0x2c);
struct SmokeTextureOwner {
    std::int32_t slot;
    std::byte *pixels;
    std::uint32_t colour;
    std::int16_t references;
    std::uint16_t activeEmitters;
};
struct SmokeEffect {
    std::int32_t slot, count;
    dc::sdk::Particle *particles;
    Vector3 position, spawnVariance, velocity, velocityVariance;
    shen_name flags;
    float width, height, birthRate, initialScale, fadeFactor, expansion;
    std::uint32_t retained58;
    SmokeTextureOwner *texture;
    std::int32_t profile;
    float floorHeight;
};
struct FireEffect {
    std::int32_t slot;
    std::uint32_t drawFlags;
    std::uint16_t flags;
    std::int16_t bone;
    std::byte *pixels;
    Vector3 localPosition, worldPosition;
    float width, height, alpha;
    std::uint32_t retained34;
    Character *character;
    float cameraOffset;
    FireTrailEntry *trail;
    FireInstance *instances;
};
struct ParticleEmitterReference {
    std::uint32_t active;
    Vector3 position, localPosition;
    Vector3i rotation;
};
SHENMUE_PARTICLE_LAYOUT(sizeof(ParticleEmitterReference) == 0x28);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEmitterReference, position) == 4);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEmitterReference, localPosition) == 0x10);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEmitterReference, rotation) == 0x1c);
struct ParticleEffectSlot {
    std::uint32_t retained00;
    std::int32_t link, textureSlot, state;
    HLib::TaskCallback callback;
    void *data;
    shen_name token;
    std::uint32_t retained1c;
};
SHENMUE_PARTICLE_LAYOUT(sizeof(SmokeTextureOwner) == 0x10);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeTextureOwner, pixels) == 4);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeTextureOwner, references) == 0x0c);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeTextureOwner, activeEmitters) == 0x0e);
SHENMUE_PARTICLE_LAYOUT(sizeof(SmokeEffect) == 0x68);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, position) == 0x0c);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, spawnVariance) == 0x18);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, velocity) == 0x24);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, velocityVariance) == 0x30);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, flags) == 0x3c);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, width) == 0x40);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, texture) == 0x5c);
SHENMUE_PARTICLE_LAYOUT(offsetof(SmokeEffect, profile) == 0x60);
SHENMUE_PARTICLE_LAYOUT(sizeof(FireEffect) == 0x48);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireEffect, pixels) == 0x0c);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireEffect, localPosition) == 0x10);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireEffect, width) == 0x28);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireEffect, character) == 0x38);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireEffect, trail) == 0x40);
SHENMUE_PARTICLE_LAYOUT(offsetof(FireEffect, instances) == 0x44);
SHENMUE_PARTICLE_LAYOUT(sizeof(ParticleEffectSlot) == 0x20);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEffectSlot, state) == 0x0c);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEffectSlot, callback) == 0x10);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEffectSlot, data) == 0x14);
SHENMUE_PARTICLE_LAYOUT(offsetof(ParticleEffectSlot, token) == 0x18);
// Ship-spray anchor and parameters.
struct ShipSprayAnchor {
    std::uint32_t flags;
    std::int32_t state;
    Vector3 position;
    Vector3i rotation;
};
struct ShipSprayParameters {
    std::uint32_t flags;
    std::int32_t state;
    std::int32_t slot;
    std::int32_t frames;
    std::uint32_t retained10;
    Vector3 position;
    float positionSpread;
    Vector3 velocity;
    float velocitySpread;
    float width, height;
    std::uint32_t colour;
    float birthRate;
    float growth[3];
    float fade, drift, lift;
};
SHENMUE_PARTICLE_LAYOUT(sizeof(ShipSprayAnchor) == 0x20);
SHENMUE_PARTICLE_LAYOUT(offsetof(ShipSprayParameters, position) == 0x14);
SHENMUE_PARTICLE_LAYOUT(offsetof(ShipSprayParameters, velocity) == 0x24);
SHENMUE_PARTICLE_LAYOUT(offsetof(ShipSprayParameters, width) == 0x34);
SHENMUE_PARTICLE_LAYOUT(offsetof(ShipSprayParameters, birthRate) == 0x40);
SHENMUE_PARTICLE_LAYOUT(offsetof(ShipSprayParameters, lift) == 0x58);
extern SmokeEffect *g_smokeEffects[16], *g_splashEffects[16], *g_reservedParticleEffects[8], *g_fallingEffects[16];
extern FireEffect *g_fireEffects[8];
extern SmokeTextureOwner *g_smokeTextures[2];
extern dc::sdk::FftEffect *g_smokeFftData[2];
extern ParticleEffectSlot g_particleEffectSlots[16], g_smokeEffectSlots[2], g_fireEffectSlots[8];
extern FireFftPrefix *g_fireFftOwners[8];
extern std::uint16_t *g_splashTemporaryPixels;
extern float g_stationarySmokeAlpha;
extern HLib::Task *g_particleControllerTask;
extern std::uint32_t g_particleControllerRequests;
extern std::int32_t g_particleEffectsSuspended;
extern ParticleEmitterReference g_particleEmitterReferences[16];
extern std::int32_t g_dropEmitterSlots[5];
extern float g_smokeInitialScale0, g_smokeInitialScale2, g_smokeInitialScale3, g_smokeInitialScale4;
extern float g_smokeRadialVelocity3, g_smokeRadialDecay3, g_smokeRadialVelocity4, g_smokeRadialDecay4;
void EffectControlCallback(void *data);
void EF_Callback_EffectCtrl_Enqueue();
void QueueParticleEmitterDraws(void *data);
std::int32_t AllocateParticleEffectSlot(std::int32_t type, std::int32_t textureSlot, HLib::TaskCallback callback, void *data, shen_name token);
std::int32_t CreateFireEffect(const Vector3 *position, float width, float height, std::int32_t drawMode, std::int32_t flags);
std::int32_t CreateSmokeEffect(
    const Vector3 *position, const Vector3 *spawnVolume, std::int32_t particleCount, float birthRate, std::uint32_t flags, std::int32_t profile);
std::uint32_t ConfigureSmokeEffect(std::int32_t profile, std::uint32_t index, const Vector3 *position, const Vector3 *spawnVolume, std::int32_t particleCount,
    float birthRate, std::uint32_t flags);
std::int32_t CreateSplashEffect(const Vector3 *position, const Vector3 *spawnVolume, std::int32_t count, float birthRate, std::uint32_t flags,
    float floorHeight, const char *textureArchive, const char *textureName);
std::int32_t CreateFallingParticleEffect(const Vector3 *position, const Vector3 *spawnVolume, std::int32_t count, float birthRate, std::uint32_t flags,
    const char *textureArchive, const char *textureName, float floorHeight);
void SetFallingParticleFloor(float floorHeight);
void FireControlCallback(void *data);
void InitializeFireFft(std::int32_t index, FireFftPrefix *effect);
void SetFireFftOutput(std::int32_t index, std::uint16_t *pixels);
void UpdateFireOwner(FireEffect *effect);
void UpdateFireFft(std::int32_t index, std::int32_t flags);
void InitEffectPoints();
void InitEffectControlTaskData();
void CacheCurrentParticleEffects(std::int32_t suspended);
void InitializeDropParticles();
std::int32_t AssignParticleEmitterReferences(std::int32_t index, const Vector3 &position, const Vector3 &localPosition, std::int32_t yaw);
void ResetDropParticleAdmission();
void ReleaseDropParticles();
void ResumeParticleEffect(std::int32_t type, std::uint32_t index);
void SuspendParticleEffect(std::int32_t type, std::uint32_t index);
void FreeParticleEffect(std::int32_t type, std::uint32_t index);
std::int32_t GetParticleEffectVisibility(std::int32_t type, std::uint32_t index);
void SetParticleEffectPosition(std::int32_t type, std::uint32_t index, const Vector3 *position, const Vector3 *variance);
void SetParticleEffectVelocity(std::int32_t type, std::uint32_t index, const Vector3 *velocity, const Vector3 *variance);
void SetParticleEffectUniformSize(std::int32_t type, std::uint32_t index, float size);
void SetParticleEffectSize(std::int32_t type, std::uint32_t index, float width, float height);
void SetParticleEffectColour(std::int32_t type, std::uint32_t index, std::int32_t alpha, std::int32_t red, std::int32_t green, std::int32_t blue);
void TranslateParticleEffectParticles(std::int32_t type, std::uint32_t index, const Vector3 *delta);
void SetStationarySmokeAlpha(std::uint32_t index, std::int32_t alpha);
void SetParticleEffectInitialScale(std::int32_t type, std::uint32_t index, float scale);
void InitializeShipSprayAnchor(ShipSprayAnchor *anchor);
std::int32_t SetShipSprayParameters(ShipSprayParameters *parameters, std::int32_t mode, std::int32_t side);
void UpdateSakuraParticleTuning();
std::int32_t CreateSakuraEffect(const Vector3 *position, const Vector3 *spawnVolume, std::int32_t count, float birthRate, std::uint32_t flags);
void InitializeSakuraEffects();
void SetParticleEffectBirthRate(std::int32_t type, std::uint32_t index, float fraction);
void SetSmokeEffectRadialMotion(std::uint32_t index, float velocity, float decay);
void SetFireAlpha(std::uint32_t index, float alpha);
FireInstance *SetFirePosition(std::uint32_t index, std::int32_t instance, const Vector3 *position);
void SetFireInstanceAlpha(std::uint32_t index, std::int32_t instance, float alpha);
void SetFireInstanceSize(std::uint32_t index, std::int32_t instance, float width, float height);
void SetFirePalette(std::uint32_t index, const std::uint16_t *palette);
extern Vector3 g_smokePosition;
extern std::uint32_t g_smokeParameters[4];
extern float g_smokeExpansion0, g_smokeExpansion2, g_smokeExpansion3, g_smokeExpansion4;
extern float g_smokeFadeFactor0, g_smokeFadeFactor2, g_smokeFadeFactor3, g_smokeFadeFactor4;
extern std::int32_t g_smokeAlphaStep3, g_smokeAlphaStep4;
extern float g_smokeOpacityScale3, g_smokeOpacityScale4;
void SetSmokePosition(const Vector3 *position);
void SetSmokeParameter(std::uint32_t parameter, std::uint32_t value);
void SetSmokeEffectFalloff(std::uint32_t index, std::int32_t value);
void SetSmokeEffectFadeFactor(std::uint32_t index, std::int32_t value);
void SetSmokeEffectPhaseRate(std::uint32_t index, float rate);
void SetSmokeEffectFalloffCurve(std::uint32_t index, float exponent, float scale);
void SetSmokeEffectSpread(std::uint32_t index, float falloff, float opacity);
void SetFireFalloff(std::uint32_t index, float radiusX, float radiusY, float gain);
void SetFireFalloffScalar(std::uint32_t index, float scalar);
void AttachFireEffect(std::uint32_t index, shen_name name, std::int32_t bone, float offset);
void AttachFireInstance(std::uint32_t index, std::int32_t instance, shen_name name, std::int32_t bone, float offset);
} // namespace s2
} // namespace shenmue
#undef SHENMUE_PARTICLE_LAYOUT
#undef SHENMUE_PARTICLE_LAYOUT_NAME
#undef SHENMUE_PARTICLE_LAYOUT_NAME_
#endif

#else

#include "s2/weather_particles_data.hpp"

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>
#include <array>
#include <span>

namespace shenmue::s2 {

constexpr shen_name effectToken{SHEN_NAME(EFPT)};
constexpr shen_name controllerToken{SHEN_NAME(EFFE)};
constexpr std::uint32_t invalidEffect = 0xffffffffU;
constexpr std::uint32_t activeFlag = 0x80000000U;
constexpr std::uint32_t suspendedFlag = 0x02000000U;

inline constexpr shen_name weatherFixcDataName{SHEN_NAME(FIXC)};
inline constexpr shen_name weatherFixcNodeName{SHEN_NAME(DLST)};
inline constexpr shen_name weatherRainOwnerName{SHEN_NAME(CARA)};
inline constexpr shen_name weatherGroupName{SHEN_NAME(GBLS)};
inline constexpr shen_name weatherRainParticleName{SHEN_NAME(RINP)};
inline constexpr shen_name weatherRippleParticleName{SHEN_NAME(RPLP)};
inline constexpr shen_name weatherRippleColourName{SHEN_NAME(RCOL)};
inline constexpr shen_name weatherInteractionCameraName{SHEN_NAME(ICAR)};
inline constexpr shen_name weatherResourceTaskName{SHEN_NAME(EFFE)};
inline constexpr std::uint32_t weatherParticlePausedFlag = 0x10000000U;
inline constexpr std::uint32_t weatherParticleSpawnFlag = 0x40000000U;
inline constexpr std::int32_t weatherGroupMode0Size = 0x1c0;
inline constexpr std::uint32_t weatherRippleDefaultColour = 0xa0ffffffU;
inline constexpr shen_name weatherClipEntryName{SHEN_NAME(WCCP)};
inline constexpr shen_name weatherSphereVisibilityMap{SHEN_NAME(QD00)};

extern std::array<WeatherClipSet, 2> weatherClipSets;
struct Character;
struct LegacyEffectTexture;

struct ParticleEmitterReference {
    std::uint32_t active;
    Vector3 position;
    Vector3 localPosition;
    Vector3i rotation;
};
static_assert(sizeof(ParticleEmitterReference) == 40);
static_assert(offsetof(ParticleEmitterReference, localPosition) == 16);
static_assert(offsetof(ParticleEmitterReference, rotation) == 28);

struct WeatherParticleGroup;
struct WeatherRainParticle;
struct WeatherRippleParticle;
using WeatherRainParticleUpdate = void (*)(WeatherRainParticle *, WeatherParticleGroup *);
using WeatherRippleParticleUpdate = void (*)(WeatherRippleParticle *, WeatherParticleGroup *);

struct WeatherRainParticle {
    std::uint32_t flags;
    std::uint32_t reserved04;
    WeatherRainParticleUpdate update;
    Vector3 position;
    Vector3 drift;
};

struct WeatherRippleParticle {
    std::uint32_t flags;
    std::uint32_t reserved04;
    WeatherRippleParticleUpdate update;
    Vector3 position;
    std::int32_t rotationPitch;
    std::int32_t rotationYaw;
    std::int32_t reserved24;
    std::uint32_t *colourRampEntry;
    float age;
    float lifetime;
};
struct WeatherParticleGroup {
    WeatherParticleGroup *next;
    WeatherParticleGroup *previous;
    std::uint32_t rainFlags;
    std::uint32_t reserved14;
    WeatherRainParticle *rainParticles;
    std::int32_t rainCapacity;
    std::int32_t rainLiveCount;
    std::int32_t rainDrawCount;
    std::int32_t reserved2c;
    void *rainCountLflt;
    std::int32_t rainCurrentCount;
    float rainVerticalSpan;
    float motionX;
    float motionY;
    float motionZ;
    float motionDamping;
    std::int32_t rainRenderMode;
    std::byte rainGap54[0x0c];
    float rainPressureParam0;
    float rainPressureParam1;
    float rainPressureParam2;
    std::int32_t rotationX;
    std::int32_t rotationY;
    std::int32_t rotationZ;
    float spawnDriftX;
    float fallSpeed;
    float spawnDriftZ;
    float driftVarianceX;
    float driftVarianceY;
    float driftVarianceZ;
    float smoothMotionX;
    float smoothMotionY;
    float smoothMotionZ;
    std::uint32_t snowFlags;
    void *snowParticles;
    std::byte snowGapA8[0x08];
    std::int32_t snowDrawCount;
    std::byte snowGapB4[0x24];
    std::int32_t snowRenderMode;
    std::byte snowGapDC[0x10];
    float snowDeltaTime;
    std::byte snowGapF0[0x20];
    std::uint32_t rippleFlags;
    std::uint32_t reserved114;
    WeatherRippleParticle *rippleParticles;
    std::int32_t rippleCapacity;
    std::int32_t rippleLiveCount;
    std::int32_t rippleDrawCount;
    std::int32_t reserved12c;
    void *rippleCountLflt;
    std::int32_t rippleCurrentCount;
    std::int32_t rippleRenderMode;
    std::byte rippleGap140[0x0c];
    std::uint32_t colour;
    float rippleDeltaTime;
    float cameraY;
    std::uint32_t *rippleColourRamp;
    std::byte gap160[0x30];
    LegacyEffectTexture *rippleTexture;
    float spawnVolume;
    float boundsMaxY;
    float boundsMinY;
    float boundsMaxX;
    float boundsMinX;
    float boundsMaxZ;
    float boundsMinZ;
    float frustumDistance;
    float frustumSpan;
    WeatherClipQuadPlanes clipA; // Native WTHR/WCCP set 0 copied into each live group.
    WeatherClipQuadPlanes clipB; // Native WTHR/WCCP set 1 copied into each live group.
    std::uint32_t reserved24c;
};

struct WeatherFixcNode {
    WeatherFixcNode *previous;
    WeatherFixcNode *next;
    std::int32_t index;
    std::int32_t reserved14;
    union Payload {
        void *pointer;
        float scalar;
        std::int32_t integer;
    } payload;
};

struct WeatherFixcOwner {
    std::int32_t mode;
    std::int32_t reserved04;
    void *cara;
    WeatherFixcNode *windList;
};

struct WeatherRainOwner {
    WeatherParticleGroup *rain0;
    WeatherParticleGroup *rain1;
    std::uint32_t mode;
    std::uint32_t reserved14;
    void *storageEntry18;
    WeatherParticleGroup *rippleGroup;
    WeatherParticleGroup *rippleGroup1;
    std::int32_t frustumFov; // native CARA +0x30 input used by span calculations
    std::int32_t reserved34;
};

static_assert(sizeof(WeatherRainParticle) == 0x28);
static_assert(offsetof(WeatherRainParticle, update) == 0x08);
static_assert(offsetof(WeatherRainParticle, position) == 0x10);
static_assert(offsetof(WeatherRippleParticle, position) == 0x10);
static_assert(offsetof(WeatherRippleParticle, lifetime) == 0x34);
static_assert(sizeof(WeatherRippleParticle) == 0x38);
static_assert(offsetof(WeatherRippleParticle, colourRampEntry) == 0x28);
static_assert(sizeof(WeatherParticleGroup) == 0x250);
static_assert(offsetof(WeatherParticleGroup, rainParticles) == 0x18);
static_assert(offsetof(WeatherParticleGroup, rainCountLflt) == 0x30);
static_assert(offsetof(WeatherParticleGroup, rainFlags) == 0x10);
static_assert(offsetof(WeatherParticleGroup, rainCapacity) == 0x20);
static_assert(offsetof(WeatherParticleGroup, rainPressureParam0) == 0x60);
static_assert(offsetof(WeatherParticleGroup, fallSpeed) == 0x7c);
static_assert(offsetof(WeatherParticleGroup, rippleCountLflt) == 0x130);
static_assert(offsetof(WeatherParticleGroup, rippleColourRamp) == 0x158);
static_assert(offsetof(WeatherParticleGroup, rippleFlags) == 0x110);
static_assert(offsetof(WeatherParticleGroup, rippleParticles) == 0x118);
static_assert(offsetof(WeatherParticleGroup, spawnVolume) == 0x198);
static_assert(offsetof(WeatherParticleGroup, clipA) == 0x1bc);
static_assert(offsetof(WeatherParticleGroup, clipB) == 0x204);
static_assert(sizeof(WeatherFixcOwner) == 0x18);
static_assert(sizeof(WeatherFixcNode) == 0x20);
static_assert(offsetof(WeatherFixcOwner, cara) == 0x08);
static_assert(offsetof(WeatherFixcOwner, windList) == 0x10);
static_assert(offsetof(WeatherFixcNode, previous) == 0x00);
static_assert(offsetof(WeatherFixcNode, next) == 0x08);
static_assert(offsetof(WeatherFixcNode, index) == 0x10);
static_assert(offsetof(WeatherFixcNode, payload) == 0x18);
static_assert(sizeof(WeatherRainOwner) == 0x38);
static_assert(offsetof(WeatherRainOwner, frustumFov) == 0x30);
static_assert(offsetof(WeatherRainOwner, rain0) == 0x00);
static_assert(offsetof(WeatherRainOwner, rain1) == 0x08);
static_assert(offsetof(WeatherRainOwner, mode) == 0x10);
static_assert(offsetof(WeatherRainOwner, storageEntry18) == 0x18);
static_assert(offsetof(WeatherRainOwner, rippleGroup) == 0x20);
static_assert(offsetof(WeatherRainOwner, rippleGroup1) == 0x28);

struct WeatherParticleVertex {
    Vector4 position;
    std::uint32_t colour;
    float u;
    float v;
};
static_assert(sizeof(WeatherParticleVertex) == 0x1c);
static_assert(offsetof(WeatherParticleGroup, rippleTexture) == 0x190);

namespace HLib {
struct Task;
}

extern float weatherRippleLifetimeScale;
extern float weatherRippleLifetimeStep;
void UpdateWeatherParticleGroundCameraPosition();
void UpdateWeatherInteractionCamera();
void UpdateWeatherClipCameraPosition();
void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);
void Weather_UpdateWCCPClipSet0();
void Weather_UpdateWCCPClipSet1();
void Weather_ResetWCCPClipSet0();
void Weather_ResetWCCPClipSet1();
void ApplyWeatherRainAndRippleClip(const Vector2 *clip);
void ApplyWeatherParticleClip(const Vector2 *clip);
LegacyEffectTexture *ReadWeatherRipplePvrTexture(std::span<const std::byte> image);
HLib::Task *WAR_CallbackParticleResource_Enqueue(float rainSpawnLimit, float secondaryRainParam, float rippleSpawnLimit, float unused);
void WAR_CallbackParticleResource_Callback(void *data);
HLib::Task *AllocateICAR(float rainSpawnLimit, float secondaryRainParam, float rippleSpawnLimit, float unused);
void InitializeWeatherParticleResources();
void Weather_RegisterActiveFIXCAndInitEffects();
void EnsureWeatherFixcRegistered();
void EV_WeatherEffectUtil_Impl(std::uint32_t frustumFov, float spawnScale, std::int64_t unused, float rippleScale, float minY, float maxY);
std::int64_t Weather_FreeActiveFIXC();
std::int32_t SetWeatherRainPressureImmediate(float pressure);
void BeginRainPressureTransitionForWeatherEffects(float rainValue, std::int32_t frames);
void ComputeCloudVisibility(float rainValue, std::int32_t frames);
void UpdateWeatherParticleGroups();
void SetWeatherParticleRainColour(std::uint32_t colour);
void SetWeatherParticleRippleColour(std::uint32_t colour);
void SetWeatherParticleDrawingEnabled(std::int32_t enabled);

void AssignParticleEmitterReferences(std::int32_t emitter, const Vector3 &position, const Vector3 &localPosition, std::int32_t yaw);
// InitializeEffectControl/InitializeEffectPoints reset the map's fixed effect pools.
void InitializeEffectControl();
void EnqueueEffectControlTask();
void InitializeEffectPoints();
// CacheCurrentParticleEffects freezes/unfreezes all sixteen emitter callbacks.
void CacheCurrentParticleEffects(std::int32_t suspended);
// Native g_CurrentWeatherForceFlag; rain and ripple group flags track pause.
extern std::int32_t g_currentWeatherForceFlag;
void SetRainAndRippleFlagsTo(std::int32_t paused);

// Native families 0/1/2/3/4 are smoke/splash/reserved/falling/fire.
// Except the -1 sentinel, effect and instance indices are native unchecked indices.
std::uint64_t CreateSplashEffect(const Vector3 &position, const Vector3 &spawnVolume, std::int32_t particleCount, float birthRate, shen_name flags,
    std::int32_t floorHeightBits, const char *textureArchive, const char *textureName);
std::uint64_t CreateFallingParticleEffect(const Vector3 &position, const Vector3 &spawnVolume, std::int32_t particleCount, float birthRate, shen_name flags,
    const char *textureArchive, const char *textureName, float floorHeight);
void SetFallingParticleFloor(float floorHeight);
std::uint64_t CreateSmokeEffect(
    const Vector3 &position, const Vector3 &spawnVolume, std::int32_t particleCount, float birthRate, shen_name flags, std::int32_t profile);
std::uint64_t CreateFireEffect(const Vector3 &position, float width, float height, std::int32_t drawMode, std::uint16_t flags);
void SetParticleEffectVelocity(std::int32_t type, std::uint32_t effect, const Vector3 &velocity, const Vector3 &variance);
void SetParticleEffectColour(std::int32_t type, std::uint32_t effect, std::int32_t alpha, std::uint8_t red, std::uint8_t green, std::uint8_t blue);
void SetParticleEffectPosition(std::int32_t type, std::uint32_t effect, const Vector3 *position, const Vector3 *spawnVolume);
void SetParticleEffectSize(std::int32_t type, std::uint32_t effect, float width, float height);
void SetParticleEffectInitialScale(std::int32_t type, std::uint32_t effect, float scale);
void SetParticleEffectBirthRate(std::int32_t type, std::uint32_t effect, float fraction);
void SetParticleEffectUniformSize(std::int32_t type, std::uint32_t effect, float size);
void TranslateParticleEffectParticles(std::int32_t type, std::uint32_t effect, const Vector3 &delta);
std::uint64_t GetParticleEffectVisibility(std::int32_t type, std::uint32_t effect);
void SetSmokeEffectFalloff(std::uint32_t effect, std::int32_t value);
void SetSmokeEffectFadeFactor(std::uint32_t effect, std::int32_t value);
void SetSmokeEffectPhaseRate(std::uint32_t effect, float rate);
void SetSmokeEffectFalloffCurve(std::uint32_t effect, float radius, float gain);
void SetStationarySmokeAlpha(std::uint32_t effect, std::int32_t alpha);
// Reconfigure an already allocated smoke emitter; this does not recreate its particles or texture owner.
std::uint32_t ConfigureSmokeEffect(std::int32_t profile, std::uint32_t effect, const Vector3 &position, const Vector3 &spawnVolume, std::int32_t particleCount,
    float birthRate, shen_name flags);
void SetSmokeEffectSpread(std::uint32_t effect, float falloff, float opacity);
void SetSmokeEffectRadialMotion(std::uint32_t effect, float velocity, float decay);
void SuspendParticleEffect(std::int32_t type, std::uint32_t effect);
void ResumeParticleEffect(std::int32_t type, std::uint32_t effect);
void FreeParticleEffect(std::int32_t type, std::uint32_t effect);
void SetFirePalette(std::uint32_t effect, const std::uint16_t *palette);
void SetFireFalloff(std::uint32_t effect, float radiusX, float radiusY, float gain);
void SetFireFalloffScalar(std::uint32_t effect, float scalar);
void ResetFireShaderInstanceCount() noexcept;
void SetFireAlpha(std::uint32_t effect, float alpha);
std::uint32_t SetFirePosition(std::uint32_t effect, std::int32_t instance, const Vector3 *position);
Character *AttachFireEffect(std::uint32_t effect, shen_name name, std::int16_t bone, float offset);
Character *AttachFireInstance(std::uint32_t effect, std::int32_t instance, shen_name name, std::int16_t bone, std::int32_t offsetBits);
void SetFireCameraOffset(std::uint32_t effect, float offset);
void SetFireInstanceCameraOffset(std::uint32_t effect, std::int32_t instance, float offset);
void SetFireInstanceAlpha(std::uint32_t effect, std::int32_t instance, float alpha);
void SetFireInstanceSize(std::uint32_t effect, std::int32_t instance, float width, float height);

} // namespace shenmue::s2

#endif
