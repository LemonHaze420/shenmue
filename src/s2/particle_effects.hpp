#pragma once

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

struct WeatherClipEntry {
    std::uint32_t enabled;
    std::array<Vector2, 4> points;
    float clipY0;
    float clipY1;
    Vector3 center;
    float scale;
};
struct WeatherClipSet {
    std::int32_t enabled;
    std::int32_t count;
    WeatherClipEntry *entries;
};
static_assert(sizeof(WeatherClipEntry) == 60);
static_assert(offsetof(WeatherClipEntry, points) == 4);
static_assert(offsetof(WeatherClipEntry, center) == 44);
static_assert(offsetof(WeatherClipEntry, scale) == 56);
static_assert(sizeof(WeatherClipSet) == 16);
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
struct WeatherClipQuadPlanes {
    float clipY1;
    float clipY0;
    std::array<float, 4> x;
    std::array<float, 4> z;
    std::array<float, 4> edgeZ;
    std::array<float, 4> edgeX;
};

static_assert(sizeof(WeatherClipQuadPlanes) == 0x48);
static_assert(offsetof(WeatherClipQuadPlanes, x) == 0x08);
static_assert(offsetof(WeatherClipQuadPlanes, edgeZ) == 0x28);

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
    WeatherClipQuadPlanes clipA;
    WeatherClipQuadPlanes clipB;
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
    std::int32_t frustumFov;
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

extern std::int32_t g_currentWeatherForceFlag;
void SetRainAndRippleFlagsTo(std::int32_t paused);



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
void SetFireAlpha(std::uint32_t effect, float alpha);
std::uint32_t SetFirePosition(std::uint32_t effect, std::int32_t instance, const Vector3 *position);
Character *AttachFireEffect(std::uint32_t effect, shen_name name, std::int16_t bone, float offset);
Character *AttachFireInstance(std::uint32_t effect, std::int32_t instance, shen_name name, std::int16_t bone, std::int32_t offsetBits);
void SetFireCameraOffset(std::uint32_t effect, float offset);
void SetFireInstanceCameraOffset(std::uint32_t effect, std::int32_t instance, float offset);
void SetFireInstanceAlpha(std::uint32_t effect, std::int32_t instance, float alpha);
void SetFireInstanceSize(std::uint32_t effect, std::int32_t instance, float width, float height);

} // namespace shenmue::s2
