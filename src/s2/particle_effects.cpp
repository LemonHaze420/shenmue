#include "s2/particle_effects.hpp"

#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/drop_particles.hpp"
#include "s2/effect_fft.hpp"
#include "s2/effect_texture.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/hiki.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/pak.hpp"
#include "s2/ship_spray.hpp"
#include "s2/daku_spray.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/weather.hpp"
#include "s2/window.hpp"
#include "d3t/buffer.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <emmintrin.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#endif

    namespace shenmue::s2 {
    std::array<std::int32_t, 3> g_dakuSpraySlots{};
    std::array<Vector3, 3> g_dakuSprayPositions{};
    std::array<std::uint32_t, 3> g_dakuSprayColours{};
    std::array<Vector2, 3> g_dakuSpraySizes{};
    ShipSprayEmitter *g_shipSprayParameters{};
    Vector3 g_shipSprayPosition{};
    Vector3 g_shipSprayLocalPosition{};
    std::int32_t g_shipSprayYaw{};
    float g_shipSprayFadeRate{};
    std::uint32_t g_shipSprayColour{};
    float weatherRippleLifetimeScale = 0.5f;
    float weatherRippleLifetimeStep = 0.025f;
    std::array<WeatherClipSet, 2> weatherClipSets{};

    namespace {

    struct ParticleRecord;
    using ParticleCallback = void (*)(ParticleRecord &);

    // BirthParticle copies this complete 96-byte template to every particle. Its
    // callbacks at +0x20 run under the currently selected emitter, not a task.
    struct ParticleRecord {
        std::uint32_t flags;
        Vector3 position;
        Vector3 velocity;
        std::uint32_t reserved1c;
        ParticleCallback callback;
        std::uint64_t phaseX;
        std::uint64_t phaseY;
        std::uint64_t phaseZ;
        std::uint64_t stepX;
        std::uint64_t stepY;
        std::uint64_t stepZ;
        std::uint32_t colour;
        float scale;
};


struct ParticleEmitter {
    std::uint32_t flags;
    std::int32_t count;
    ParticleRecord *particles;
    std::uint32_t textureFlags;
    std::uint32_t samplerControl;
    std::uint32_t gpuFlags;
    std::uint32_t textureHandle;
    ParticleCallback initializer;
    std::int32_t birthThreshold;
    Vector3 position;
    Vector3 velocity;
    Vector3 spawnVariance;
    Vector3 velocityVariance;
    float halfWidth;
    float halfHeight;
    std::uint32_t colour;
    void *textureRecord;
    const std::byte *textureSource;
};

// The shared allocation is 0x18 bytes, not an array of twelve dwords.
// SmokeTextureOwner callbacks increment the WORD at +0x14 and modify the WORD at +0x16.
struct SmokeTextureOwner {
    std::int32_t slot;
    std::uint32_t reserved04;
    std::byte *pixels;
    std::uint32_t colour;
    std::uint16_t references;
    std::uint16_t activeEmitters;
};

struct SmokeEffect {
    std::int32_t slot;
    std::int32_t count;
    ParticleRecord *particles;
    Vector3 position;
    Vector3 spawnVariance;
    Vector3 velocity;
    Vector3 velocityVariance;
    shen_name flags;
    float width;
    float height;
    float birthRate;
    float initialScale;
    float fadeFactor;
    float expansion;
    std::uint32_t reserved5c;
    std::uint64_t reserved60;
    SmokeTextureOwner *texture;
    std::int32_t profile;
    float floorHeight;
};

struct FireTrailEntry {
    std::int16_t age;
    std::int16_t heightOffset;
    Vector3 position;
};

struct FireInstance {
    std::uint16_t enabled;
    std::int16_t bone;
    Vector3 localPosition;
    Vector3 worldPosition;
    float alpha;
    float width;
    float height;
    Character *character;
    float cameraOffset;
    std::uint32_t reserved34;
};

struct FireEffect {
    std::int32_t slot;
    std::uint32_t drawFlags;
    std::uint16_t flags;
    std::int16_t bone;
    std::uint32_t reserved0c;
    std::byte *pixels;
    Vector3 localPosition;
    Vector3 worldPosition;
    float width;
    float height;
    float alpha;
    std::uint32_t reserved3c;
    std::uint64_t reserved40;
    Character *character;
    float cameraOffset;
    std::uint32_t reserved54;
    FireTrailEntry *trail;
    FireInstance *instances;
};

// AllocatePUFY owns three distinct fixed pools. State 1 is allocated but
// suspended; state 2 participates in the controller's per-frame passes.
struct EffectSlot {
    std::uint32_t reserved00;
    std::int32_t link;
    std::int32_t textureSlot;
    std::int32_t state;
    HLib::TaskCallback callback;
    void *data;
    shen_name token;
    std::uint32_t reserved24;
    std::uint64_t reserved28;
};

static_assert(sizeof(ParticleRecord) == 0x60);
static_assert(offsetof(ParticleRecord, callback) == 0x20);
static_assert(offsetof(ParticleRecord, phaseY) == 0x30);
static_assert(offsetof(ParticleRecord, stepY) == 0x48);
static_assert(offsetof(ParticleRecord, colour) == 0x58);
static_assert(sizeof(ParticleEmitter) == 0x78);
static_assert(offsetof(ParticleEmitter, initializer) == 0x20);
static_assert(offsetof(ParticleEmitter, position) == 0x2c);
static_assert(offsetof(ParticleEmitter, halfWidth) == 0x5c);
static_assert(sizeof(SmokeTextureOwner) == 0x18);
static_assert(offsetof(SmokeTextureOwner, references) == 0x14);
static_assert(offsetof(SmokeTextureOwner, activeEmitters) == 0x16);
static_assert(sizeof(SmokeEffect) == 0x78);
static_assert(offsetof(SmokeEffect, flags) == 0x40);
static_assert(offsetof(SmokeEffect, texture) == 0x68);
static_assert(offsetof(SmokeEffect, profile) == 0x70);
static_assert(offsetof(SmokeEffect, floorHeight) == 0x74);
static_assert(sizeof(FireTrailEntry) == 0x10);
static_assert(sizeof(FireInstance) == 0x38);
static_assert(offsetof(FireInstance, character) == 0x28);
static_assert(sizeof(FireEffect) == 0x68);
static_assert(offsetof(FireEffect, character) == 0x48);
static_assert(offsetof(FireEffect, trail) == 0x58);
static_assert(offsetof(FireEffect, instances) == 0x60);
static_assert(sizeof(EffectSlot) == 0x30);
static_assert(offsetof(EffectSlot, state) == 0x0c);
static_assert(offsetof(EffectSlot, callback) == 0x10);
static_assert(offsetof(EffectSlot, data) == 0x18);

std::array<ParticleEmitter, 16> emitters{};
std::array<EffectSlot, 16> emitterSlots{};
std::array<EffectSlot, 2> smokeSlots{};
std::array<EffectSlot, 8> fireSlots{};
std::array<ParticleEmitterReference, 16> emitterReferences{};
std::array<std::int32_t, 5> dropEmitterSlots{};
Vector3 dropParticlePosition{};
Vector3 dropParticleLocalPosition{};
std::array<std::int32_t, 5> dropLightIndices{};
std::array<std::int32_t, 5> dropLightEnabled{};
std::array<std::uint32_t, 5> dropLightKinds{};
std::array<Vector3, 5> dropLightPositions{}, dropLightDirections{};
std::array<float, 5> dropLightRanges{}, dropLightConeCosines{};
std::array<ParticleRecord, 30> dropParticles0{}, dropParticles1{};
std::array<CompactParticleRecord, 30> dropParticles2{};
std::array<CompactParticleRecord, 40> dropParticles3{};
std::array<CompactParticleRecord, 25> dropParticles4{};
std::array<ShipSprayEmitter, 6> shipSprayK = kShipSprayK;
std::array<ShipSprayEmitter, 6> shipSprayFive = kShipSprayFive;
std::array<ShipSprayEmitter, 6> shipSprayDefault = kShipSprayDefault;
std::array<SmokeFftData *, 2> smokeFft{};
std::array<FireFftData *, 8> fireFft{};
std::array<SmokeTextureOwner *, 2> smokeTextures{};
std::array<SmokeEffect *, 16> smokeEffects{};
std::array<SmokeEffect *, 16> splashEffects{};

std::array<SmokeEffect *, 8> reservedParticleEffects{};
std::array<SmokeEffect *, 16> fallingEffects{};
std::array<FireEffect *, 8> fireEffects{};
std::array<std::uint32_t, 16> smokeTextureHandles{};
std::array<std::array<std::uint32_t, 2>, 8> fireTextureHandles{};
HLib::Task *effectController{};
ParticleEmitter *currentEmitter{};
std::int32_t effectsSuspended{};
std::uint64_t particleRandomState{1};
std::array<float, 5> smokeInitialScale{};
std::array<float, 5> smokeExpansion{};
std::array<float, 5> smokeFadeFactor{};
std::int32_t radialSmokeAngle{};
std::array<float, 5> smokeRadialVelocity{};
std::array<float, 5> smokeRadialDecay{};
std::array<float, 5> smokeOpacityScale{};
std::array<std::int32_t, 5> smokeAlphaStep{};
float stationarySmokeAlpha{};
float fallingParticleFloor{};
std::array<Vector3, 3> fallingParticleOrigins{};

float weatherRainPressureParam0 = std::bit_cast<float>(0x15b7b9bbU);
float weatherRainPressureParam1 = std::bit_cast<float>(0x3b656042U);
float weatherRainPressureParam2 = std::bit_cast<float>(0x3e19999aU);
float weatherRainPressureParam1Dry = std::bit_cast<float>(0x3b03126fU);
float weatherRainPressureParam2Dry = std::bit_cast<float>(0x3e4ccccdU);
float weatherRainSpawnDriftXDry = 0.0f;
float weatherRainSpawnDriftZDry = 0.0f;
float weatherRainSpawnDriftX = 0.0f;
float weatherRainFallSpeedDry = std::bit_cast<float>(0xbe3851ecU);
float weatherRainFallSpeedWet = std::bit_cast<float>(0xbe8f5c29U);
float weatherRainSpawnDriftZ = 0.0f;
float weatherRainDriftVarianceX = 0.0f;
float weatherRainDriftVarianceY = 0.0f;
float weatherRainDriftVarianceZ = 0.0f;
float weatherRainMotionDamping = std::bit_cast<float>(0x3d75c28fU);

struct WeatherInteractionCamera {
    Vector3 position;
    Vector3 currentCameraPosition;
    Vector3 previousBackwardDirection;
    Vector3 nextCameraPosition;
    Vector3 currentCameraPosition2;
    Vector3 backwardDirection;
};

static_assert(sizeof(WeatherInteractionCamera) == 0x48);
static_assert(offsetof(WeatherInteractionCamera, nextCameraPosition) == 0x24);
static_assert(offsetof(WeatherInteractionCamera, currentCameraPosition2) == 0x30);
static_assert(offsetof(WeatherInteractionCamera, backwardDirection) == 0x3c);

WeatherFixcOwner *weatherFixcData{};
WeatherFixcNode *weatherFixcList{};
WeatherParticleGroup *weatherGroupList{};
WeatherInteractionCamera *weatherInteractionCamera{};
HLib::Task *weatherParticleResourceTask{};
std::int32_t weatherRegisteredFixc{};
std::int32_t weatherWarState{};
float weatherRainSpawnLimit = 2.5f;
float weatherSecondaryRainParameter = 4.0f;
float weatherRippleSpawnLimit = 4.0f;
float weatherParticleUnusedParameter{};
float weatherRippleDeltaTime = 0.15f;
std::uint32_t weatherDefaultRippleColour = weatherRippleDefaultColour;
std::uint32_t weatherRainColour{};
std::uint32_t weatherRippleColour{};
float weatherRegisteredPressureParam1{};
float weatherRegisteredPressureParam2{};
Vector3 weatherRegisteredSpawnDrift{};
Vector3 weatherRegisteredDriftVariance{};
Vector3 weatherParticleGroundCameraPosition{};
Vector3 weatherClipCameraPosition{};
Matrix4x4 weatherScreenProjection{{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
Matrix4x4 weatherProjectionScratch{};
Matrix4x4 weatherParticleTransformScratch{};
WeatherParticleGroup *weatherCurrentGroup{};
void *weatherCurrentParticles{};
std::int32_t weatherParticleDrawCount{};
std::uint32_t weatherCurrentParticleFlags{};
std::uint32_t weatherParticleStride{};
std::uint32_t weatherParticleLayerStride{};
std::int32_t weatherParticleNeedsUpdate{};
const std::byte *weatherParticleStorage{};
std::uintptr_t weatherParticleEvaluate{};
float weatherProjectionX{};
Vector3 weatherProjectionFactors{};
float weatherParticleHalfWidth{};
float weatherParticleHalfHeight{};
float weatherProjectedParticleWidth{};
float weatherProjectedParticleHeight{};
float weatherParticleDeltaTime{};

using WeatherParticleRenderFunction = void (*)();
WeatherParticleRenderFunction weatherParticleRender{};

WeatherFloat *AllocateWeatherParticleLflt() {
    WeatherFloat *const value = static_cast<WeatherFloat *>(HLib::AllocateTaskData(sizeof(WeatherFloat), SHEN_NAME(LFLT)));
    std::memset(value, 0, sizeof(*value));
    return value;
}

WeatherFixcNode *FindWeatherFixcNode(WeatherFixcNode *node, const void *payload) {
    while (node) {
        if (node->payload.pointer == payload)
            return node;
        node = node->next;
    }
    return nullptr;
}

WeatherFixcNode *AllocateWeatherFixcNode(std::int32_t index, void *payload) {
    WeatherFixcNode *const node = static_cast<WeatherFixcNode *>(HLib::AllocateTaskData(sizeof(WeatherFixcNode), weatherFixcNodeName));
    node->index = index;
    node->previous = nullptr;
    node->next = nullptr;
    node->payload.pointer = payload;
    return node;
}

void AppendWeatherFixcNode(WeatherFixcNode **head, WeatherFixcNode *node) {
    WeatherFixcNode *previous = *head;
    if (!previous) {
        *head = node;
        return;
    }
    while (previous->next)
        previous = previous->next;
    previous->next = node;
    node->previous = previous;
}

void UnlinkWeatherFixcNode(WeatherFixcNode **head, WeatherFixcNode *node) {
    if (!node)
        return;
    if (*head == node)
        *head = node->next;
    if (node->previous)
        node->previous->next = node->next;
    if (node->next)
        node->next->previous = node->previous;
    node->previous = nullptr;
    node->next = nullptr;
    HLib::FreeTaskData(node);
}

void AppendWeatherParticleGroup(WeatherParticleGroup *group) {
    WeatherParticleGroup *previous = weatherGroupList;
    if (previous) {
        while (previous->next)
            previous = previous->next;
        previous->next = group;
        group->previous = previous;
    } else {
        weatherGroupList = group;
    }
    group->next = nullptr;
    ++weatherWarState;
}

WeatherParticleGroup *AllocateWeatherParticleGroup(const Vector3 &center, float spanX, float spanY, float spanZ, bool wccp) {
    const std::size_t size = wccp ? sizeof(WeatherParticleGroup) : static_cast<std::size_t>(weatherGroupMode0Size);
    WeatherParticleGroup *const group = static_cast<WeatherParticleGroup *>(HLib::AllocateTaskData(size, weatherGroupName));
    const std::byte *const clearEnd =
        reinterpret_cast<const std::byte *>(group) + (wccp ? offsetof(WeatherParticleGroup, reserved24c) : offsetof(WeatherParticleGroup, frustumSpan));
    (void)shenClearData(group, clearEnd);
    if (wccp) {
        group->clipA.clipY1 = -100000.0f;
        group->clipB.clipY1 = -100000.0f;
    }
    group->spawnVolume = (spanX * spanZ) * spanY;
    const float half = spanX * 0.5f;
    group->boundsMaxX = half + center.x;
    group->boundsMinX = center.x - half;
    group->boundsMaxY = half + center.y;
    group->boundsMinY = center.y - half;
    group->boundsMaxZ = half + center.z;
    group->boundsMinZ = center.z - half;
    AppendWeatherParticleGroup(group);
    return group;
}

void FreeWeatherParticleGroup(WeatherParticleGroup *group) {
    if (!group)
        return;
    if (!group->previous)
        weatherGroupList = group->next;
    if (group->previous)
        group->previous->next = group->next;
    if (group->next)
        group->next->previous = group->previous;
    group->previous = nullptr;
    group->next = nullptr;
    --weatherWarState;
    if (group->rainParticles) {
        HLib::FreeTaskData(group->rainCountLflt);
        HLib::FreeTaskData(group->rainParticles);
        group->rainParticles = nullptr;
        group->rainFlags = 0;
    }
    if (group->rippleColourRamp)
        HLib::FreeTaskData(group->rippleColourRamp);
    if (group->rippleParticles) {
        HLib::FreeTaskData(group->rippleCountLflt);
        HLib::FreeTaskData(group->rippleParticles);
        group->rippleTexture = nullptr;
        group->rippleParticles = nullptr;
        group->rippleFlags = 0;
    }
    HLib::FreeTaskData(group);
}

WeatherRainOwner *AllocateWeatherRainOwner(std::uint32_t frustumFov) {
    WeatherRainOwner *const owner = static_cast<WeatherRainOwner *>(HLib::AllocateTaskData(sizeof(WeatherRainOwner), weatherRainOwnerName));
    const std::byte *const clearEnd = reinterpret_cast<const std::byte *>(owner) + 40;
    (void)shenClearData(owner, clearEnd);
    owner->frustumFov = static_cast<std::int32_t>(frustumFov);
    return owner;
}

WeatherFixcOwner *AllocateWeatherFixc(std::uint32_t frustumFov) {
    WeatherFixcOwner *const fixc = static_cast<WeatherFixcOwner *>(HLib::AllocateTaskData(sizeof(WeatherFixcOwner), weatherFixcDataName));
    fixc->mode = 1;
    fixc->cara = AllocateWeatherRainOwner(frustumFov);
    fixc->windList = nullptr;
    return fixc;
}

void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure) {
    if (!group || !group->rainParticles)
        return;
    group->rainVerticalSpan = group->boundsMaxY - group->boundsMinY;
    const float count = static_cast<float>(TruncateToInt32(static_cast<float>(group->rainCapacity) * pressure));
    (void)lfltSetImmediate(static_cast<WeatherFloat *>(group->rainCountLflt), count);
    group->rainFlags |= 1U;
}

void BeginRainGroupTransition(WeatherParticleGroup *group, float rainValue, std::int32_t frames) {
    if (!group || !group->rainParticles)
        return;
    std::int32_t target = 0;
    const float height = group->boundsMaxY - group->boundsMinY;
    if (!(std::fabs(rainValue) <= 0.000001f || std::fabs(height) <= 0.000001f)) {
        target = TruncateToInt32(((static_cast<float>(group->rainCapacity) * rainValue) / height) * -group->fallSpeed);
    }
    group->rainVerticalSpan = -group->fallSpeed;
    BeginWeatherFloatTransition(static_cast<WeatherFloat *>(group->rainCountLflt), static_cast<float>(target), frames);
    group->rainFlags &= ~1U;
}
float WeatherFrustumSpan(float distance, std::int32_t fov) {
    constexpr float radiansPerAngleUnit = 6.28318530717958647692f / 65536.0f;
    const std::int32_t halfAngle = fov / 2;
    return (2.0f * std::tan(static_cast<float>(halfAngle) * radiansPerAngleUnit)) * distance;
}
void ApplyWeatherPressureParameters(WeatherParticleGroup *group, float pressure, std::int32_t sideIndex) {
    float parameter1 = weatherRainPressureParam1Dry;
    float parameter2 = weatherRainPressureParam2Dry;
    float driftX = weatherRainSpawnDriftXDry;
    float fallSpeed = weatherRainFallSpeedDry;
    float driftZ = weatherRainSpawnDriftZDry;
    if (pressure >= 0.5f) {
        const float amount = (pressure - 0.5f) + (pressure - 0.5f);
        parameter1 = (weatherRainPressureParam1 - weatherRainPressureParam1Dry) * amount + weatherRainPressureParam1Dry;
        parameter2 = (weatherRainPressureParam2 - weatherRainPressureParam2Dry) * amount + weatherRainPressureParam2Dry;
        driftX = (weatherRainSpawnDriftX - weatherRainSpawnDriftXDry) * amount + weatherRainSpawnDriftXDry;
        fallSpeed = (weatherRainFallSpeedWet - weatherRainFallSpeedDry) * amount + weatherRainFallSpeedDry;
        driftZ = (weatherRainSpawnDriftZ - weatherRainSpawnDriftZDry) * amount + weatherRainSpawnDriftZDry;
    }
    if (sideIndex == 1) {
        parameter1 *= 1.5f;
        parameter2 *= 2.0f;
    }
    group->rainPressureParam1 = parameter1;
    group->rainPressureParam2 = parameter2;
    group->spawnDriftX = driftX;
    group->fallSpeed = fallSpeed;
    group->spawnDriftZ = driftZ;
}

void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);
void WeatherRainParticleUpdate_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group);
void WeatherRainParticleUpdate_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group);
void WeatherRainParticleUpdate_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group);

bool WeatherPointInsideClipPlanes(const WeatherClipQuadPlanes &clip, const Vector3 &position, bool requireOrdered = false) {
    for (std::size_t index = 0; index < 2; ++index) {
        const std::size_t edge = index * 2;
        const float side = (position.x - clip.x[index]) * clip.edgeZ[edge] - (position.z - clip.z[index]) * clip.edgeZ[edge + 1];
        if (requireOrdered ? !(side < 0.0f) : side >= 0.0f)
            return false;
    }
    for (std::size_t index = 2; index < 4; ++index) {
        const std::size_t edge = (index - 2) * 2;
        const float side = (position.x - clip.x[index]) * clip.edgeX[edge] - (position.z - clip.z[index]) * clip.edgeX[edge + 1];
        if (requireOrdered ? !(side < 0.0f) : side >= 0.0f)
            return false;
    }
    return true;
}

bool WeatherPointInsideClip(const WeatherClipQuadPlanes &clip, const Vector3 &position, bool requireOrdered = false) {
    if (requireOrdered ? !(position.y <= clip.clipY1 && clip.clipY0 <= position.y) : position.y > clip.clipY1 || clip.clipY0 > position.y)
        return false;
    return WeatherPointInsideClipPlanes(clip, position, requireOrdered);
}
void ResetWeatherRainParticle_Mode0(WeatherRainParticle *particle) {
    particle->flags = weatherParticleSpawnFlag;
    particle->update = SpawnWeatherRainParticle_Mode0;
}

void ResetWeatherRainParticle_Direct(WeatherRainParticle *particle) {
    particle->flags = weatherParticleSpawnFlag;
    particle->update = SpawnWeatherRainParticle_Direct;
}

void ResetWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle) {
    particle->flags = weatherParticleSpawnFlag;
    particle->update = SpawnWeatherRainParticle_Mode1WCCP;
}

void SpawnWeatherRainParticle_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    ++group->rainLiveCount;
    --group->rainCurrentCount;
    particle->update = WeatherRainParticleUpdate_Mode0;
    particle->flags = activeFlag;
    particle->position.x = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinX;
    particle->position.y = group->boundsMaxY - IncrementSharedPrngFloat() * (group->rainVerticalSpan * 8.0f);
    particle->position.z = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinZ;
    particle->drift.x = (IncrementSharedPrngFloat() * group->driftVarianceX - group->driftVarianceX * 0.5f) + group->spawnDriftX;
    particle->drift.z = (IncrementSharedPrngFloat() * group->driftVarianceZ - group->driftVarianceZ * 0.5f) + group->spawnDriftZ;
}

void SpawnWeatherRainParticle_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    ++group->rainLiveCount;
    --group->rainCurrentCount;
    particle->update = WeatherRainParticleUpdate_Direct;
    particle->flags = activeFlag;
    particle->position.x = IncrementSharedPrngFloat() * group->frustumDistance + group->boundsMinX;
    particle->position.y = group->boundsMaxY - IncrementSharedPrngFloat() * (group->rainVerticalSpan * 8.0f);
    particle->position.z = IncrementSharedPrngFloat() * group->clipA.clipY1 + group->boundsMinZ;
    particle->drift.x = (IncrementSharedPrngFloat() * group->driftVarianceX - group->driftVarianceX * 0.5f) + group->spawnDriftX;
    particle->drift.z = (IncrementSharedPrngFloat() * group->driftVarianceZ - group->driftVarianceZ * 0.5f) + group->spawnDriftZ;
}

void SpawnWeatherRainParticle_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (group->rainCurrentCount <= 0)
        return;
    ++group->rainLiveCount;
    --group->rainCurrentCount;
    particle->update = WeatherRainParticleUpdate_Mode1WCCP;
    particle->position.x = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinX;
    particle->position.y = group->boundsMaxY - IncrementSharedPrngFloat() * (group->rainVerticalSpan * 8.0f);
    particle->position.z = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinZ;
    particle->drift.x = (IncrementSharedPrngFloat() * group->driftVarianceX - group->driftVarianceX * 0.5f) + group->spawnDriftX;
    particle->drift.z = (IncrementSharedPrngFloat() * group->driftVarianceZ - group->driftVarianceZ * 0.5f) + group->spawnDriftZ;
    if (WeatherPointInsideClip(group->clipA, particle->position) || WeatherPointInsideClip(group->clipB, particle->position)) {
        particle->flags = weatherParticleSpawnFlag;
        particle->update = SpawnWeatherRainParticle_Mode1WCCP;
    } else {
        particle->flags = activeFlag;
    }
}

void UpdateWeatherRainParticleMotion(WeatherRainParticle *particle, WeatherParticleGroup *group, float fallDelta, float xSpan, float zSpan, bool wccp = false) {
    particle->position.x += group->motionX + particle->drift.x;
    particle->position.y += fallDelta;
    particle->position.z += group->motionZ + particle->drift.z;
    if (wccp ? particle->position.x > group->boundsMaxX : !(particle->position.x <= group->boundsMaxX)) {
        particle->position.x -= xSpan;
        return;
    }
    if (wccp ? particle->position.x < group->boundsMinX : !(group->boundsMinX <= particle->position.x)) {
        particle->position.x += xSpan;
        return;
    }
    if (wccp ? particle->position.z > group->boundsMaxZ : !(particle->position.z <= group->boundsMaxZ))
        particle->position.z -= zSpan;
    else if (particle->position.z < group->boundsMinZ)
        particle->position.z += zSpan;
}

void WeatherRainParticleUpdate_Mode0(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (particle->position.y < group->boundsMinY) {
        ResetWeatherRainParticle_Mode0(particle);
        --group->rainLiveCount;
        return;
    }
    UpdateWeatherRainParticleMotion(particle, group, group->motionY, group->frustumSpan, group->frustumSpan);
}

void WeatherRainParticleUpdate_Direct(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (particle->position.y < group->boundsMinY) {
        ResetWeatherRainParticle_Direct(particle);
        --group->rainLiveCount;
        return;
    }
    UpdateWeatherRainParticleMotion(particle, group, group->fallSpeed, group->frustumDistance, group->clipA.clipY1);
}

void WeatherRainParticleUpdate_Mode1WCCP(WeatherRainParticle *particle, WeatherParticleGroup *group) {
    if (particle->position.y < group->boundsMinY) {
        ResetWeatherRainParticle_Mode1WCCP(particle);
        --group->rainLiveCount;
        return;
    }
    UpdateWeatherRainParticleMotion(particle, group, group->motionY, group->frustumSpan, group->frustumSpan, true);
    if (WeatherPointInsideClip(group->clipA, particle->position, true) || WeatherPointInsideClip(group->clipB, particle->position, true))
        particle->flags = weatherParticleSpawnFlag;
    else
        particle->flags = activeFlag;
}

void InitializeWeatherRainRenderState(WeatherParticleGroup *group, WeatherRainParticleUpdate spawn) {
    group->rainFlags = 0xc2000000U;
    group->rainRenderMode = std::bit_cast<std::int32_t>(0x82000002U);
    const std::int32_t rainModePrefix = -2004877312;
    const std::uint64_t rainModeSuffix = 546308288ULL;
    std::memcpy(group->rainGap54, &rainModePrefix, sizeof(rainModePrefix));
    std::memcpy(group->rainGap54 + 4, &rainModeSuffix, sizeof(rainModeSuffix));
    group->rainGap54[7] = static_cast<std::byte>(0x94);
    for (std::int32_t index = 0; index < group->rainCapacity; ++index) {
        WeatherRainParticle &particle = group->rainParticles[index];
        particle.flags = weatherParticleSpawnFlag;
        particle.update = spawn;
        particle.position = {};
        particle.drift = {};
    }
}

void EnsureWeatherRainParticles(WeatherParticleGroup *group, float spawnScale, WeatherRainParticleUpdate spawn) {
    if (!group || group->rainParticles)
        return;
    std::int32_t count = TruncateToInt32(std::fmin(weatherRainSpawnLimit, spawnScale) * group->spawnVolume);
    group->rainCapacity = count;
    if (count == 0)
        return;
    if (count > 1100)
        count = 1100;
    group->rainCapacity = count;
    const std::int64_t allocationSize = 40LL * count;
    group->rainParticles = static_cast<WeatherRainParticle *>(HLib::AllocateTaskData(static_cast<std::uint64_t>(allocationSize), weatherRainParticleName));
    group->rainLiveCount = 0;
    group->rainDrawCount = 0;
    group->rainCountLflt = AllocateWeatherParticleLflt();
    group->rainCurrentCount = 0;
    group->rainPressureParam0 = weatherRainPressureParam0;
    group->rainPressureParam1 = weatherRainPressureParam1;
    group->rainPressureParam2 = weatherRainPressureParam2;
    group->spawnDriftX = weatherRainSpawnDriftX;
    group->fallSpeed = weatherRainFallSpeedWet;
    group->spawnDriftZ = weatherRainSpawnDriftZ;
    group->driftVarianceX = weatherRainDriftVarianceX;
    group->driftVarianceY = weatherRainDriftVarianceY;
    group->driftVarianceZ = weatherRainDriftVarianceZ;
    group->rainVerticalSpan = group->boundsMaxY - group->boundsMinY;
    (void)lfltSetImmediate(static_cast<WeatherFloat *>(group->rainCountLflt), 0.0f);
    group->rainFlags |= 1U;
    group->motionDamping = weatherRainMotionDamping;
    InitializeWeatherRainRenderState(group, spawn);
}

void WeatherRippleParticleSpawn_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleSpawn_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleSpawn_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleUpdate_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleUpdate_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleUpdate_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleReprobe_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group);
void WeatherRippleParticleReprobe_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group);

void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour) {
    if (!group)
        return;
    if (!group->rippleColourRamp) {
        group->rippleColourRamp = static_cast<std::uint32_t *>(HLib::AllocateTaskData(0x80, weatherRippleColourName));
        std::memset(group->rippleColourRamp, 0, 0x80);
    }
    const std::uint32_t previousColour = group->colour;
    if (previousColour == colour)
        return;
    group->colour = colour;
    const std::uint32_t rgb = colour & 0x00ffffffU;
    if (((previousColour ^ colour) & 0xff000000U) != 0) {
        const float alpha = static_cast<float>(colour >> 24U);
        float phase = 0.5f;
        for (std::int32_t index = 0; index < 32; ++index) {
            const std::int32_t angle = TruncateToInt32((phase * 16384.0f) * 0.5f);
            const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
            const std::uint32_t rampAlpha = static_cast<std::uint32_t>(TruncateToInt32(((alpha * sine) * sine) * sine));
            group->rippleColourRamp[index] = rgb | (rampAlpha << 24U);
            phase += 0.050000001f;
        }
    } else {
        for (std::int32_t index = 0; index < 32; ++index)
            group->rippleColourRamp[index] = (group->rippleColourRamp[index] & 0xff000000U) | rgb;
    }
}

bool InitializeWeatherRippleAtGroundHit(
    WeatherRippleParticle *particle, WeatherParticleGroup *group, WeatherRippleParticleUpdate update, bool incrementLiveCount) {
    if (!weatherInteractionCamera)
        return false;
    Vector3 query{particle->position.x, weatherParticleGroundCameraPosition.y + 50.0f, particle->position.z};
    flddPushIntersectionFlag(8);
    flddToggleFlag(32);
    const float height = shenFindIntersectionWithScene(query, query.y);
    if (incrementLiveCount ? !(height > -3.4028235e38f) : height <= -3.4028235e38f)
        return false;
    Vector3 normal{};
    TransformIntersectionNormal(normal);
    if (incrementLiveCount ? !(std::fabs(normal.y) >= 0.92387998f) : std::fabs(normal.y) < 0.92387998f)
        return false;
    particle->position.y = height + 0.02f;
    particle->rotationPitch = -static_cast<std::int16_t>(atan2_fixed16(-normal.y, -normal.z));
    particle->rotationYaw = static_cast<std::int16_t>(atan2_fixed16(-normal.y, -normal.x)) + 0x4000;
    particle->update = update;
    if (incrementLiveCount) {
        ++group->rippleLiveCount;
        particle->age = 0.5f;
        const float lifetime = IncrementSharedPrngFloat() + 1.0f;
        particle->lifetime = lifetime;
        particle->age *= weatherRippleLifetimeScale;
        particle->lifetime *= weatherRippleLifetimeScale;
        particle->colourRampEntry = group->rippleColourRamp + TruncateToInt32((lifetime * 32.0f) * 0.5f);
    }
    return true;
}
void WeatherRippleParticleSpawn_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
    particle->position.x = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinX;
    particle->position.z = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinZ;
    (void)InitializeWeatherRippleAtGroundHit(particle, group, WeatherRippleParticleUpdate_Mode0, true);
}

void WeatherRippleParticleSpawn_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
    particle->position.x = IncrementSharedPrngFloat() * group->frustumDistance + group->boundsMinX;
    particle->position.z = IncrementSharedPrngFloat() * group->clipA.clipY1 + group->boundsMinZ;
    (void)InitializeWeatherRippleAtGroundHit(particle, group, WeatherRippleParticleUpdate_Direct, true);
}

void WeatherRippleParticleSpawn_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount)
        return;
    particle->position.x = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinX;
    particle->position.z = IncrementSharedPrngFloat() * group->frustumSpan + group->boundsMinZ;
    (void)InitializeWeatherRippleAtGroundHit(particle, group, WeatherRippleParticleUpdate_Mode1, true);
}

void WeatherRippleParticleRespawnOrAge(WeatherRippleParticle *particle, WeatherParticleGroup *group, WeatherRippleParticleUpdate spawn) {
    if (group->rippleLiveCount < group->rippleCurrentCount) {
        const float lifetime = IncrementSharedPrngFloat() + 1.0f;
        particle->age = weatherRippleLifetimeScale * 0.5f;
        particle->lifetime = lifetime * weatherRippleLifetimeScale;
        particle->colourRampEntry = group->rippleColourRamp + TruncateToInt32((lifetime * 32.0f) * 0.5f);
    } else {
        --group->rippleLiveCount;
        particle->update = spawn;
        particle->flags = weatherParticleSpawnFlag;
    }
}

void WeatherRippleParticleReprobe_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount || !InitializeWeatherRippleAtGroundHit(particle, group, WeatherRippleParticleUpdate_Mode0, false)) {
        --group->rippleLiveCount;
        particle->update = WeatherRippleParticleSpawn_Mode0;
    }
}

void WeatherRippleParticleReprobe_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (group->rippleLiveCount >= group->rippleCurrentCount || !InitializeWeatherRippleAtGroundHit(particle, group, WeatherRippleParticleUpdate_Mode1, false)) {
        --group->rippleLiveCount;
        particle->update = WeatherRippleParticleSpawn_Mode1;
    }
}

void WeatherRippleParticleUpdate_Mode0(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (particle->lifetime <= particle->age) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Mode0);
        return;
    }
    particle->age += weatherRippleLifetimeStep;
    --particle->colourRampEntry;
    particle->flags = weatherParticleSpawnFlag;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (particle->position.x < group->boundsMinX) {
        particle->position.x += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (particle->position.z > group->boundsMaxZ) {
        particle->position.z -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (particle->position.z < group->boundsMinZ) {
        particle->position.z += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode0;
        return;
    }
    if (group->cameraY > particle->position.y)
        particle->flags = activeFlag;
}

void WeatherRippleParticleUpdate_Direct(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (particle->lifetime < particle->age) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Direct);
    } else if (particle->position.y < group->cameraY) {
        particle->age += weatherRippleLifetimeStep;
        --particle->colourRampEntry;
        particle->flags = activeFlag;
    } else {
        particle->flags = weatherParticleSpawnFlag;
    }
}

void WeatherRippleParticleUpdate_Mode1(WeatherRippleParticle *particle, WeatherParticleGroup *group) {
    if (particle->lifetime <= particle->age) {
        WeatherRippleParticleRespawnOrAge(particle, group, WeatherRippleParticleSpawn_Mode1);
        return;
    }
    particle->age += weatherRippleLifetimeStep;
    --particle->colourRampEntry;
    particle->flags = weatherParticleSpawnFlag;
    if (particle->position.x > group->boundsMaxX) {
        particle->position.x -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (particle->position.x < group->boundsMinX) {
        particle->position.x += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (particle->position.z > group->boundsMaxZ) {
        particle->position.z -= group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (particle->position.z < group->boundsMinZ) {
        particle->position.z += group->frustumSpan;
        particle->update = WeatherRippleParticleReprobe_Mode1;
        return;
    }
    if (group->cameraY > particle->position.y) {
        particle->flags = activeFlag;
        if (WeatherPointInsideClipPlanes(group->clipA, particle->position, true) || WeatherPointInsideClipPlanes(group->clipB, particle->position, true))
            particle->flags = weatherParticleSpawnFlag;
    }
}

void EnsureWeatherRippleParticles(WeatherParticleGroup *group, float spawnScale, WeatherRippleParticleUpdate spawn) {
    if (!group || group->rippleParticles)
        return;
    std::int32_t count = TruncateToInt32(std::fmin(weatherRippleSpawnLimit, spawnScale) * group->spawnVolume);
    group->rippleCapacity = count;
    if (count == 0)
        return;
    if (count > 600)
        count = 600;
    group->rippleCapacity = count;
    const std::int64_t allocationSize = 56LL * count;
    group->rippleParticles =
        static_cast<WeatherRippleParticle *>(HLib::AllocateTaskData(static_cast<std::uint64_t>(allocationSize), weatherRippleParticleName));
    group->rippleLiveCount = 0;
    group->rippleDrawCount = 0;
    group->rippleCountLflt = AllocateWeatherParticleLflt();
    group->rippleCurrentCount = 0;
    AllocateWeatherRippleColourRamp(group, weatherDefaultRippleColour);
    group->rippleDeltaTime = weatherRippleDeltaTime;
    if (group->rippleParticles)
        (void)lfltSetImmediate(
            static_cast<WeatherFloat *>(group->rippleCountLflt), static_cast<float>(TruncateToInt32(static_cast<float>(group->rippleCapacity) * 0.0f)));
    group->rippleFlags = 0xc6000000U;
    group->rippleRenderMode = std::bit_cast<std::int32_t>(0x8200000aU);
    const float pfxReserved = -7.7037198e-34f;
    const std::uint64_t pfxFlags = 0x00209000c0ULL;
    std::memcpy(group->rippleGap140, &pfxReserved, sizeof(pfxReserved));
    std::memcpy(group->rippleGap140 + 4, &pfxFlags, sizeof(pfxFlags));
    group->rippleGap140[7] = static_cast<std::byte>(0x94);
    for (std::int32_t index = 0; index < count; ++index) {
        WeatherRippleParticle &particle = group->rippleParticles[index];
        particle.flags = weatherParticleSpawnFlag;
        particle.update = spawn;
        particle.position = {};
        particle.rotationPitch = 0;
        particle.rotationYaw = 0;
        particle.reserved24 = 0;
        particle.colourRampEntry = nullptr;
        particle.age = 0.0f;
        particle.lifetime = 0.0f;
    }
    group->rippleTexture = GetEffectTextureReference(1);
    std::uint32_t widthBits = 0;
    std::uint32_t heightBits = 0;
    const std::uint32_t *const textureDimensions = GetEffectTextureReferenceDimensions(1);
    for (std::uint32_t width = textureDimensions[0]; width != 0; width >>= 1U)
        ++widthBits;
    for (std::uint32_t height = textureDimensions[1]; height != 0; height >>= 1U)
        ++heightBits;
    const std::uint32_t samplerControl = 0x8a800000U;
    std::uint32_t gpuFlags = (8U * widthBits - 32U) | (heightBits - 4U) | 0x940242c0U;
    gpuFlags |= 0x100000U;
    gpuFlags = (gpuFlags & 0x00ffffffU) | 0x94000000U;
    const std::uint32_t texture = static_cast<std::uint32_t>(group->rippleTexture->handle);
    std::memcpy(group->rippleGap140, &samplerControl, sizeof(samplerControl));
    std::memcpy(group->rippleGap140 + 4, &gpuFlags, sizeof(gpuFlags));
    std::memcpy(group->rippleGap140 + 8, &texture, sizeof(texture));
    std::memset(group->gap160, 0, sizeof(group->gap160));
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const Character *const ryo = control ? static_cast<const Character *>(control->ryo) : nullptr;
    const std::int32_t characterState = ryo ? ryo->internalState : 0;
    const float maxDistance = 100.0f;
    std::memcpy(group->gap160 + 0x20, &characterState, sizeof(characterState));
    std::memcpy(group->gap160 + 0x0c, &maxDistance, sizeof(maxDistance));
}

void EnsureMode0RainGroups(WeatherRainOwner *cara, float spawnScale) {
    const Vector3 center{};
    float span = WeatherFrustumSpan(2.8f, cara->frustumFov);
    if (!cara->rain0)
        cara->rain0 = AllocateWeatherParticleGroup(center, span, span, span, false);
    EnsureWeatherRainParticles(cara->rain0, spawnScale, SpawnWeatherRainParticle_Mode0);
    cara->rain0->frustumDistance = 2.8f;
    span = WeatherFrustumSpan(6.0f, cara->frustumFov);
    if (!cara->rain1)
        cara->rain1 = AllocateWeatherParticleGroup(center, span, span, span, false);
    EnsureWeatherRainParticles(cara->rain1, spawnScale * 0.47265625f, SpawnWeatherRainParticle_Mode0);
    cara->rain1->frustumDistance = 6.0f;
}

void EnsureMode1RainGroups(WeatherRainOwner *cara, float spawnScale, float minY, float maxY) {
    const Vector3 center{};
    const float firstDistance = minY < 0.0f ? 2.8f : minY;
    const float secondDistance = maxY < 0.0f ? 6.0f : maxY;
    float span = WeatherFrustumSpan(firstDistance, cara->frustumFov);
    if (!cara->rain0)
        cara->rain0 = AllocateWeatherParticleGroup(center, span, span, span, true);
    EnsureWeatherRainParticles(cara->rain0, spawnScale, SpawnWeatherRainParticle_Mode1WCCP);
    cara->rain0->frustumDistance = firstDistance;
    span = WeatherFrustumSpan(secondDistance, cara->frustumFov);
    if (!cara->rain1)
        cara->rain1 = AllocateWeatherParticleGroup(center, span, span, span, true);
    EnsureWeatherRainParticles(cara->rain1, spawnScale * 0.47265625f, SpawnWeatherRainParticle_Mode1WCCP);
    cara->rain1->frustumDistance = secondDistance;
}

void EnsureMode0RippleGroup(WeatherRainOwner *cara, float spawnScale) {
    const Vector3 center{};
    const float span = WeatherFrustumSpan(4.0f, cara->frustumFov);
    if (!cara->rippleGroup)
        cara->rippleGroup = AllocateWeatherParticleGroup(center, span, 1.0f, span, false);
    EnsureWeatherRippleParticles(cara->rippleGroup, spawnScale, WeatherRippleParticleSpawn_Mode0);
    cara->rippleGroup->frustumDistance = 4.0f;
}

void EnsureMode1RippleGroup(WeatherRainOwner *cara, float spawnScale) {
    const Vector3 center{};
    const float span = WeatherFrustumSpan(4.0f, cara->frustumFov);
    if (!cara->rippleGroup)
        cara->rippleGroup = AllocateWeatherParticleGroup(center, span, 1.0f, span, true);
    EnsureWeatherRippleParticles(cara->rippleGroup, spawnScale, WeatherRippleParticleSpawn_Mode1);
    cara->rippleGroup->frustumDistance = 4.0f;
}

void EnsureWeatherRainForFixc(WeatherFixcOwner *fixc, float spawnScale, float minY, float maxY) {
    switch (fixc->mode) {
    case 0:
        EnsureMode0RainGroups(static_cast<WeatherRainOwner *>(fixc->cara), spawnScale);
        break;
    case 1:
        EnsureMode1RainGroups(static_cast<WeatherRainOwner *>(fixc->cara), spawnScale, minY, maxY);
        break;
    case 6:
        EnsureWeatherRainParticles(static_cast<WeatherParticleGroup *>(fixc->cara), spawnScale, SpawnWeatherRainParticle_Direct);
        break;
    }
}

void EnsureWeatherRippleForFixc(WeatherFixcOwner *fixc, float spawnScale) {
    switch (fixc->mode) {
    case 0:
        EnsureMode0RippleGroup(static_cast<WeatherRainOwner *>(fixc->cara), spawnScale);
        break;
    case 1:
        EnsureMode1RippleGroup(static_cast<WeatherRainOwner *>(fixc->cara), spawnScale);
        break;
    case 6:
        EnsureWeatherRippleParticles(static_cast<WeatherParticleGroup *>(fixc->cara), spawnScale, WeatherRippleParticleSpawn_Direct);
        break;
    }
}

void AddWeatherWindLink(WeatherFixcOwner *fixc) {
    WeatherWind *const wind = GetWind();
    if (!wind || FindWeatherFixcNode(fixc->windList, wind))
        return;
    WeatherFixcNode *const node = AllocateWeatherFixcNode(0, wind);
    AppendWeatherFixcNode(&fixc->windList, node);
}

void RemoveWeatherWindLink(WeatherFixcOwner *fixc) {
    WeatherWind *const wind = GetWind();
    WeatherFixcNode *const node = FindWeatherFixcNode(fixc->windList, wind);
    if (node)
        UnlinkWeatherFixcNode(&fixc->windList, node);
}

void ModulateWeatherFixc(WeatherFixcOwner *fixc) {
    if (FindWeatherFixcNode(weatherFixcList, fixc))
        return;
    WeatherFixcNode *const node = AllocateWeatherFixcNode(fixc->mode, fixc);
    AppendWeatherFixcNode(&weatherFixcList, node);
    WeatherRainOwner *const cara = fixc->mode < 2U ? static_cast<WeatherRainOwner *>(fixc->cara) : nullptr;
    WeatherParticleGroup *const rain = cara ? cara->rain0 : static_cast<WeatherParticleGroup *>(fixc->cara);
    WeatherParticleGroup *const ripple = cara ? cara->rippleGroup : static_cast<WeatherParticleGroup *>(fixc->cara);
    if (rain && rain->rainFlags != 0 && (cara || fixc->mode == 6)) {
        WeatherFloat *const pressure = GetWeatherPressureLflt();
        const float current = GetWeatherPressure();
        const auto initializeRain = [&](WeatherParticleGroup *group) {
            if (!group)
                return;
            if (group->rainParticles) {
                SetRainGroupCountImmediate(group, current);
                if (lfltIsInterpolating(pressure))
                    BeginRainGroupTransition(group, pressure->target, lfltGetFramesRemaining(pressure));
            }
            group->rainPressureParam0 = std::bit_cast<float>(weatherRainColour);
            group->rainPressureParam1 = weatherRegisteredPressureParam1;
            group->rainPressureParam2 = weatherRegisteredPressureParam2;
            group->spawnDriftX = weatherRegisteredSpawnDrift.x;
            group->fallSpeed = weatherRegisteredSpawnDrift.y;
            group->spawnDriftZ = weatherRegisteredSpawnDrift.z;
            group->driftVarianceX = weatherRegisteredDriftVariance.x;
            group->driftVarianceY = weatherRegisteredDriftVariance.y;
            group->driftVarianceZ = weatherRegisteredDriftVariance.z;
        };
        initializeRain(rain);
        if (cara)
            initializeRain(cara->rain1);
    }
    if (ripple && ripple->rippleFlags != 0 && (cara || fixc->mode == 6)) {
        WeatherFloat *const count = GetWeatherRippleLflt();
        if (std::fabs(count->current) < 0.00000011920929f)
            count->current = 0.0f;
        if (ripple->rippleParticles) {
            (void)lfltSetImmediate(static_cast<WeatherFloat *>(ripple->rippleCountLflt),
                static_cast<float>(TruncateToInt32(static_cast<float>(ripple->rippleCapacity) * count->current)));
            if (lfltIsInterpolating(count))
                BeginWeatherFloatTransition(static_cast<WeatherFloat *>(ripple->rippleCountLflt),
                    static_cast<float>(TruncateToInt32(static_cast<float>(ripple->rippleCapacity) * count->target)), lfltGetFramesRemaining(count));
        }
        AllocateWeatherRippleColourRamp(ripple, weatherRippleColour);
        ripple->rippleDeltaTime = weatherRippleDeltaTime;
    }
    AddWeatherWindLink(fixc);
}

void UnregisterWeatherFixc(WeatherFixcOwner *fixc) {
    WeatherFixcNode *const node = FindWeatherFixcNode(weatherFixcList, fixc);
    if (node)
        UnlinkWeatherFixcNode(&weatherFixcList, node);
    RemoveWeatherWindLink(fixc);
}

void FreeWeatherFixc(WeatherFixcOwner *fixc) {
    if (!fixc)
        return;
    const std::uint32_t mode = static_cast<std::uint32_t>(fixc->mode);
    if (mode == 0 || mode == 1) {
        WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
        if (cara) {
            FreeWeatherParticleGroup(cara->rain0);
            FreeWeatherParticleGroup(cara->rain1);
            FreeWeatherParticleGroup(cara->rippleGroup);
            HLib::FreeTaskData(cara);
        }
    } else if (mode == 6) {
        FreeWeatherParticleGroup(static_cast<WeatherParticleGroup *>(fixc->cara));
    }
    HLib::FreeTaskData(fixc);
}

void SetWeatherClipPlanes(WeatherParticleGroup *group, std::size_t setIndex, const std::array<Vector2, 4> &points, float clipY0, float clipY1) {
    if (!group)
        return;
    WeatherClipQuadPlanes &clip = setIndex == 0 ? group->clipA : group->clipB;
    for (std::size_t index = 0; index < 4; ++index) {
        clip.x[index] = points[index].x;
        clip.z[index] = points[index].y;
    }
    clip.clipY0 = clipY0;
    clip.clipY1 = clipY1;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::size_t next = (index + 1U) & 3U;
        auto &edges = index < 2 ? clip.edgeZ : clip.edgeX;
        const std::size_t edge = (index & 1U) * 2U;
        edges[edge + 1] = points[next].x - points[index].x;
        edges[edge] = points[next].y - points[index].y;
    }
}

void ApplyWeatherClipToRain(WeatherFixcOwner *fixc, std::size_t setIndex, const std::array<Vector2, 4> &points, float clipY0, float clipY1) {
    if (fixc->mode != 1)
        return;
    WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
    SetWeatherClipPlanes(cara->rain0, setIndex, points, clipY0, clipY1);
    SetWeatherClipPlanes(cara->rain1, setIndex, points, clipY0, clipY1);
}

void ApplyWeatherClipToRipple(WeatherFixcOwner *fixc, std::size_t setIndex, const std::array<Vector2, 4> &points, float clipY0, float clipY1) {
    if (fixc->mode != 1)
        return;
    WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
    SetWeatherClipPlanes(cara->rippleGroup, setIndex, points, clipY0, clipY1);
    SetWeatherClipPlanes(cara->rippleGroup1, setIndex, points, clipY0, clipY1);
}

void Weather_ApplyWCCPClipSet0ToRainGroups(const std::array<Vector2, 4> &points, float clipY0, float clipY1) {
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        ApplyWeatherClipToRain(static_cast<WeatherFixcOwner *>(node->payload.pointer), 0, points, clipY0 + -0.1f, clipY1 + 0.1f);
}

void Weather_ApplyWCCPClipSet0ToRippleGroups(const std::array<Vector2, 4> &points, float clipY0, float clipY1) {
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        ApplyWeatherClipToRipple(static_cast<WeatherFixcOwner *>(node->payload.pointer), 0, points, clipY0 + -0.1f, clipY1 + 0.1f);
}

void Weather_ApplyWCCPClipSet1ToParticleGroups(const std::array<Vector2, 4> &points, float clipY0, float clipY1) {
    const float minimum = clipY0 + -0.1f;
    const float maximum = clipY1 + 0.1f;
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next) {
        ApplyWeatherClipToRain(static_cast<WeatherFixcOwner *>(node->payload.pointer), 1, points, minimum, maximum);
        ApplyWeatherClipToRipple(static_cast<WeatherFixcOwner *>(node->payload.pointer), 1, points, minimum, maximum);
    }
}

const WeatherClipEntry *SelectWeatherClipEntry(const WeatherClipSet &set) {
    const WeatherClipEntry *selected = nullptr;
    float distance = 1.0e10f;
    const WeatherClipEntry *entry = set.entries;
    std::int32_t remaining = set.count;
    if (remaining != 0) {
        do {
            const bool visible = g_currentMap.name == weatherSphereVisibilityMap.name ? SphereVisCheck(entry->center, entry->scale, 32.0f) :
                                                                                        ScreenVisCheck(entry->center, entry->scale, 32.0f);
            if (entry->enabled != 0 && visible) {
                const float dz = entry->center.z - weatherClipCameraPosition.z;
                const float dx = entry->center.x - weatherClipCameraPosition.x;
                const float score = (dz * dz + dx * dx) - entry->scale;
                if (distance > score) {
                    distance = score;
                    selected = entry;
                }
            }
            ++entry;
            remaining = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(remaining) - 1U);
        } while (remaining != 0);
    }
    return selected;
}

float WeatherFlushSmallFloat(float value) {
    return std::fabs(value) < 0.00000011920929f ? 0.0f : value;
}

void ApplyWeatherRainWind(WeatherParticleGroup *group, const Vector3 &windOffset) {
    const float x = WeatherFlushSmallFloat(group->motionX);
    const float z = WeatherFlushSmallFloat(group->motionZ);
    const float damping = WeatherFlushSmallFloat(group->motionDamping);
    const float nextX = WeatherFlushSmallFloat(x - WeatherFlushSmallFloat(damping * x));
    const float nextZ = WeatherFlushSmallFloat(z - WeatherFlushSmallFloat(damping * z));
    group->motionX = WeatherFlushSmallFloat(nextX + WeatherFlushSmallFloat(windOffset.x + windOffset.x));
    group->motionZ = WeatherFlushSmallFloat(nextZ + WeatherFlushSmallFloat(windOffset.z + windOffset.z));
}

void UpdateWeatherRainOrientation(WeatherParticleGroup *group, const Vector3 &camPos, const Vector3 &camPos2) {
    float x = WeatherFlushSmallFloat(-group->spawnDriftX - group->motionX);
    float z = WeatherFlushSmallFloat(-group->spawnDriftZ - group->motionZ);
    group->motionY = (group->fallSpeed + camPos2.y) - camPos.y;
    const float y = (camPos2.y - camPos.y) - group->motionY;
    if (!(std::sqrt(z * z + x * x) < 1.0f))
        return;
    group->smoothMotionX = WeatherFlushSmallFloat(group->smoothMotionX);
    group->smoothMotionZ = WeatherFlushSmallFloat(group->smoothMotionZ);
    group->smoothMotionY = y;
    group->smoothMotionX = (x - group->smoothMotionX) * 0.25f + group->smoothMotionX;
    group->smoothMotionZ = (z - group->smoothMotionZ) * 0.25f + group->smoothMotionZ;
    Vector3 motion{group->smoothMotionX, group->smoothMotionY, group->smoothMotionZ};
    Matrix4x4 projection = g_cameraControl->projectionMatrix;
    Matrix4x4_Push(&projection);
    Vector3 projected{};
    Matrix4x4::Transform(motion, projected);
    Matrix4x4::Pop(1);
    Vector3 axis{0.0f, -projected.z, projected.y};
    Matrix4x4::InvertInPlace(&projection);
    Matrix4x4_Push(&projection);
    Matrix4x4::Transform(axis, axis);
    Matrix4x4::Pop(1);
    group->rotationX = -static_cast<std::int16_t>(atan2_fixed16(axis.y, axis.z));
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-group->rotationX));
    Matrix4x4::Transform(axis, axis);
    Matrix4x4::Transform(motion, motion);
    Matrix4x4::Pop(1);
    group->rotationY = static_cast<std::int16_t>(atan2_fixed16(axis.x, axis.z));
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-group->rotationY));
    Matrix4x4::Transform(motion, motion);
    Matrix4x4::Pop(1);
    group->rotationZ = static_cast<std::int16_t>(atan2_fixed16(motion.y, motion.x)) - 0x4000;
}

void UpdateWeatherRainGroup(WeatherParticleGroup *group, const Vector3 &cameraPosition, const Vector3 &currentCameraPosition,
    const Vector3 &currentCameraPosition2, const Vector3 &backward, const Vector3 &windOffset, bool pressureMode, std::int32_t sideIndex, bool direct = false) {
    if (!group)
        return;
    if (!direct) {
        const std::int16_t perspective = static_cast<std::int16_t>(camCtrlGetPerspective());
        group->frustumSpan = WeatherFrustumSpan(group->frustumDistance, perspective);
        const Vector3 center{backward.x * group->frustumDistance + cameraPosition.x, backward.y * group->frustumDistance + cameraPosition.y,
            backward.z * group->frustumDistance + cameraPosition.z};
        const float half = group->frustumSpan * 0.5f;
        group->boundsMaxX = half + center.x;
        group->boundsMinX = center.x - half;
        group->boundsMaxY = half + center.y;
        group->boundsMinY = center.y - half;
        group->boundsMaxZ = half + center.z;
        group->boundsMinZ = center.z - half;
    }
    if (group->rainParticles) {
        std::int32_t advancing{};
        if (!direct)
            advancing = AdvanceWeatherFloat(static_cast<WeatherFloat *>(group->rainCountLflt));
        if (pressureMode)
            ApplyWeatherPressureParameters(group, std::fmin(1.0f, GetWeatherPressure()), sideIndex);
        ApplyWeatherRainWind(group, windOffset);
        UpdateWeatherRainOrientation(group, currentCameraPosition, currentCameraPosition2);
        if (direct)
            advancing = AdvanceWeatherFloat(static_cast<WeatherFloat *>(group->rainCountLflt));
        WeatherFloat *const count = static_cast<WeatherFloat *>(group->rainCountLflt);
        count->current = WeatherFlushSmallFloat(count->current);
        group->rainCurrentCount = TruncateToInt32(count->current);
        if (group->rainCurrentCount >= group->rainCapacity)
            group->rainCurrentCount = group->rainCapacity - 1;
        if (advancing == 0 && (group->rainFlags & 1U) != 0) {
            group->rainFlags &= ~1U;
            const std::int32_t requestedCount = group->rainCurrentCount;
            group->rainLiveCount = 0;
            std::int32_t index = 0;
            for (; group->rainCurrentCount > 0; ++index) {
                if (direct)
                    SpawnWeatherRainParticle_Direct(&group->rainParticles[index], group);
                else if (pressureMode)
                    SpawnWeatherRainParticle_Mode1WCCP(&group->rainParticles[index], group);
                else
                    SpawnWeatherRainParticle_Mode0(&group->rainParticles[index], group);
            }
            for (; index < group->rainCapacity; ++index) {
                if (direct)
                    ResetWeatherRainParticle_Direct(&group->rainParticles[index]);
                else if (pressureMode)
                    ResetWeatherRainParticle_Mode1WCCP(&group->rainParticles[index]);
                else
                    ResetWeatherRainParticle_Mode0(&group->rainParticles[index]);
            }
            const float verticalSpan = -group->fallSpeed;
            const float height = direct ? group->boundsMaxY - group->boundsMinY : group->frustumSpan;
            const std::int32_t adjustedCount = TruncateToInt32((static_cast<float>(requestedCount) * verticalSpan) / height);
            group->rainCurrentCount = adjustedCount >= group->rainCapacity ? group->rainCapacity - 1 : adjustedCount;
            group->rainVerticalSpan = verticalSpan;
            (void)lfltSetImmediate(static_cast<WeatherFloat *>(group->rainCountLflt), static_cast<float>(group->rainCurrentCount));
        }
        group->rainDrawCount = group->rainLiveCount != 0 || group->rainCurrentCount != 0 ? group->rainCapacity : 0;
    }
    if (group->rippleParticles) {
        if (pressureMode) {
            const float pressure = GetWeatherPressure();
            if (pressure >= 0.5f) {
                const float amount = (pressure - 0.5f) + (pressure - 0.5f);
                weatherRippleLifetimeScale = amount * 0.16659999f + 0.5f;
                weatherRippleLifetimeStep = (amount * 0.5f + 0.5f) * 0.050000001f;
            } else {
                weatherRippleLifetimeScale = 0.5f;
                weatherRippleLifetimeStep = 0.025f;
            }
        }
        group->cameraY = cameraPosition.y;
        AdvanceWeatherFloat(static_cast<WeatherFloat *>(group->rippleCountLflt));
        WeatherFloat *const count = static_cast<WeatherFloat *>(group->rippleCountLflt);
        count->current = WeatherFlushSmallFloat(count->current);
        group->rippleCurrentCount = TruncateToInt32(count->current);
        if (group->rippleCurrentCount >= group->rippleCapacity)
            group->rippleCurrentCount = group->rippleCapacity - 1;
        group->rippleDrawCount = group->rippleCurrentCount == 0 ? 0 : group->rippleCapacity;
    }
}

Vector4 TransformWeatherVector(const Vector4 &vector) {
    const auto &rows = workTransformMatrix.rows;
    return {((rows[1].x * vector.y + rows[0].x * vector.x) + rows[2].x * vector.z) + rows[3].x * vector.w,
        ((rows[1].y * vector.y + rows[0].y * vector.x) + rows[2].y * vector.z) + rows[3].y * vector.w,
        ((rows[0].z * vector.x + rows[1].z * vector.y) + rows[2].z * vector.z) + rows[3].z * vector.w,
        ((rows[1].w * vector.y + rows[0].w * vector.x) + rows[2].w * vector.z) + rows[3].w * vector.w};
}

Vector4 TransformWeatherParticlePosition(const Vector3 &position) {
    Vector3 projected{};
    Matrix4x4::TransformPoint(position, projected);
    const auto &rows = workTransformMatrix.rows;
    const float w = ((rows[1].w * position.y + rows[0].w * position.x) + rows[2].w * position.z) + rows[3].w;
    return {projected.x, projected.y, projected.z, w};
}

void WeatherParticleAnnotationNullsub(const char *) {
}
void WeatherParticleEndAnnotationNullsub() {
}
void WeatherParticleRenderNullsub() {
}

void PublishWeatherParticleCallback(std::int32_t index) {
    const std::byte *const callback =
        weatherParticleStorage + static_cast<std::uint32_t>(index) * weatherParticleLayerStride + offsetof(WeatherRainParticle, update);


    std::memcpy(&weatherParticleEvaluate, callback, sizeof(weatherParticleEvaluate));
}

void EvaluateWeatherParticle(void *particle) {
    if (weatherParticleLayerStride == sizeof(WeatherRippleParticle))
        reinterpret_cast<WeatherRippleParticleUpdate>(weatherParticleEvaluate)(static_cast<WeatherRippleParticle *>(particle), weatherCurrentGroup);
    else
        reinterpret_cast<WeatherRainParticleUpdate>(weatherParticleEvaluate)(static_cast<WeatherRainParticle *>(particle), weatherCurrentGroup);
}

void ApplyWeatherParticleRotationX(std::int32_t angle) {
    const float cosine = cos_fixed16(static_cast<std::uint16_t>(angle));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const Vector4 row1 = TransformWeatherVector({0.0f, cosine, sine, 0.0f});
    const Vector4 row2 = TransformWeatherVector({0.0f, -sine, cosine, 0.0f});
    workTransformMatrix.rows[1] = row1;
    workTransformMatrix.rows[2] = row2;
}

void ApplyWeatherParticleRotationY(std::int32_t angle) {
    const float cosine = cos_fixed16(static_cast<std::uint16_t>(angle));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const Vector4 row0 = TransformWeatherVector({cosine, 0.0f, -sine, 0.0f});
    const Vector4 row2 = TransformWeatherVector({sine, 0.0f, cosine, 0.0f});
    workTransformMatrix.rows[0] = row0;
    workTransformMatrix.rows[2] = row2;
}

void ApplyWeatherParticleRotationZ(std::int32_t angle) {
    const float cosine = cos_fixed16(static_cast<std::uint16_t>(angle));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const Vector4 row0 = TransformWeatherVector({cosine, sine, 0.0f, 0.0f});
    const Vector4 row1 = TransformWeatherVector({-sine, cosine, 0.0f, 0.0f});
    workTransformMatrix.rows[0] = row0;
    workTransformMatrix.rows[1] = row1;
}

void DoRainParticleEffect() {
    if ((weatherCurrentParticleFlags & weatherParticlePausedFlag) != 0)
        return;
    Matrix4x4::GetTransformMatrix(&weatherParticleTransformScratch);
    for (std::int32_t index = 0; index < weatherParticleDrawCount; ++index) {
        EvaluateWeatherParticle(weatherCurrentParticles);
        Matrix4x4::SetActiveTransform(&weatherParticleTransformScratch);
        weatherCurrentParticles = static_cast<std::byte *>(weatherCurrentParticles) + weatherParticleStride;
        PublishWeatherParticleCallback(index + 1);
    }
}

WeatherParticleVertex *GetWeatherParticleVertexBuffer(std::int32_t count) {
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return nullptr;
    return static_cast<WeatherParticleVertex *>(d3t::GetMappedConstantBuffer(context, 0, 168U * static_cast<std::uint32_t>(count)));
#else
    (void)count;
    return nullptr;
#endif
}

bool WriteWeatherBillboardQuad(WeatherParticleVertex *output, float width, float height, std::uint32_t colour) {
    const Vector4 topLeft = TransformWeatherVector({-width, height, 0.0f, 1.0f});
    const Vector4 bottomLeft = TransformWeatherVector({-width, -height, 0.0f, 1.0f});
    const Vector4 topRight = TransformWeatherVector({width, height, 0.0f, 1.0f});
    const Vector4 bottomRight = TransformWeatherVector({width, -height, 0.0f, 1.0f});
    if (!(topLeft.w > 0.0f))
        return false;
    output[0] = {{topLeft.x / topLeft.w, topLeft.y / topLeft.w, topLeft.z / topLeft.w, 1.0f / topLeft.w}, colour, 0, 0};
    if (!(topRight.w > 0.0f))
        return false;
    output[1] = {{topRight.x / topRight.w, topRight.y / topRight.w, topRight.z / topRight.w, 1.0f / topRight.w}, colour, 1, 0};
    if (!(bottomRight.w > 0.0f))
        return false;
    output[2] = {{bottomRight.x / bottomRight.w, bottomRight.y / bottomRight.w, bottomRight.z / bottomRight.w, 1.0f / bottomRight.w}, colour, 1, 1};
    if (!(topLeft.w > 0.0f))
        return false;
    output[3] = output[0];
    if (!(bottomRight.w > 0.0f))
        return false;
    output[4] = output[2];
    if (!(bottomLeft.w > 0.0f))
        return false;
    output[5] = {{bottomLeft.x / bottomLeft.w, bottomLeft.y / bottomLeft.w, bottomLeft.z / bottomLeft.w, 1.0f / bottomLeft.w}, colour, 0, 1};
    return true;
}

void WeatherRenderRotatedRainBillboardQuads() {
    WeatherRainParticle *particle = static_cast<WeatherRainParticle *>(weatherCurrentParticles);
    const std::uint32_t colour = std::bit_cast<std::uint32_t>(weatherCurrentGroup->rainPressureParam0);
    WeatherParticleVertex *output = GetWeatherParticleVertexBuffer(weatherParticleDrawCount);
    Matrix4x4::GetTransformMatrix(&weatherParticleTransformScratch);
    std::int32_t written = 0;
    if (output) {
        for (std::int32_t index = 0; index < weatherParticleDrawCount; ++index, ++particle) {
            if (weatherParticleNeedsUpdate == 0)
                EvaluateWeatherParticle(particle);
            if ((particle->flags & activeFlag) != 0) {
                const Vector4 center = TransformWeatherParticlePosition(particle->position);
                workTransformMatrix.rows[3] = center;
                ApplyWeatherParticleRotationX(weatherCurrentGroup->rotationX);
                ApplyWeatherParticleRotationY(weatherCurrentGroup->rotationY);
                ApplyWeatherParticleRotationZ(weatherCurrentGroup->rotationZ);
                if (WriteWeatherBillboardQuad(output + 6 * written, weatherParticleHalfWidth, weatherParticleHalfHeight, colour))
                    ++written;
            }
            Matrix4x4::SetActiveTransform(&weatherParticleTransformScratch);
            PublishWeatherParticleCallback(index + 1);
        }
    }
    RenderLegacyWeatherQuads(written);
}

void WeatherRenderRainSnowStreakQuads() {
    WeatherRainParticle *particle = static_cast<WeatherRainParticle *>(weatherCurrentParticles);
    std::uint32_t colour{};
    std::memcpy(&colour, weatherCurrentGroup->snowGapDC + 12, sizeof(colour));
    WeatherParticleVertex *output = GetWeatherParticleVertexBuffer(weatherParticleDrawCount);
    std::int32_t written = 0;
    if (output) {
        for (std::int32_t index = 0; index < weatherParticleDrawCount; ++index, ++particle) {
            if (weatherParticleNeedsUpdate == 0)
                EvaluateWeatherParticle(particle);
            if ((particle->flags & activeFlag) != 0) {
                const Vector4 center = TransformWeatherParticlePosition(particle->position);
                if (weatherParticleUnusedParameter < center.w) {
                    const float reciprocal = 1.0f / center.w;
                    const float width = weatherProjectedParticleWidth * reciprocal;
                    const Vector4 point{center.x * reciprocal, center.y * reciprocal, center.z * reciprocal, reciprocal};
                    WeatherParticleVertex *const quad = output + 6 * written;
                    quad[0] = {point, colour, 0, 0};
                    quad[1] = {{point.x + width, point.y - width, point.z, point.w}, colour, 1, 1};
                    quad[2] = {{point.x + width, point.y, point.z, point.w}, colour, 1, 0};
                    quad[3] = {point, colour, 0, 0};
                    quad[4] = {{point.x, point.y - width, point.z, point.w}, colour, 0, 1};
                    quad[5] = {{point.x + width, point.y - width, point.z, point.w}, colour, 1, 1};
                    ++written;
                }
            }
            PublishWeatherParticleCallback(index + 1);
        }
    }
    RenderLegacyWeatherQuads(written);
}

void WeatherRenderRippleBillboardQuads() {
    WeatherRippleParticle *particle = static_cast<WeatherRippleParticle *>(weatherCurrentParticles);
    WeatherParticleVertex *output = GetWeatherParticleVertexBuffer(weatherParticleDrawCount);
    Matrix4x4::GetTransformMatrix(&weatherParticleTransformScratch);
    std::int32_t written = 0;
    if (output) {
        for (std::int32_t index = 0; index < weatherParticleDrawCount; ++index, ++particle) {
            if (weatherParticleNeedsUpdate == 0)
                EvaluateWeatherParticle(particle);
            if ((particle->flags & activeFlag) != 0) {
                const float size = weatherParticleDeltaTime * particle->age;
                const std::uint32_t colour = *particle->colourRampEntry;
                const Vector4 center = TransformWeatherParticlePosition(particle->position);
                workTransformMatrix.rows[3] = center;
                ApplyWeatherParticleRotationX(particle->rotationPitch);
                ApplyWeatherParticleRotationY(particle->rotationYaw);
                ApplyWeatherParticleRotationZ(particle->reserved24);
                if (WriteWeatherBillboardQuad(output + 6 * written, size, size, colour))
                    ++written;
            }
            Matrix4x4::SetActiveTransform(&weatherParticleTransformScratch);
            PublishWeatherParticleCallback(index + 1);
        }
    }
    RenderLegacyWeatherQuads(written);
}

const std::array<WeatherParticleRenderFunction, 8> weatherParticleRenderFunctions{WeatherParticleRenderNullsub, WeatherRenderRotatedRainBillboardQuads,
    WeatherParticleRenderNullsub, WeatherRenderRippleBillboardQuads, WeatherParticleRenderNullsub, WeatherParticleRenderNullsub,
    WeatherRenderRainSnowStreakQuads, WeatherRenderRainSnowStreakQuads};

void DispatchWeatherParticleLayer(
    std::uint32_t flags, void *particles, std::int32_t count, std::uint32_t stride, const std::int32_t *renderMode, const char *annotation) {
    if ((flags & activeFlag) == 0 || count == 0 || !particles)
        return;
    WeatherParticleAnnotationNullsub(annotation);
    weatherCurrentParticles = particles;
    weatherParticleDrawCount = count;
    weatherParticleLayerStride = stride;
    weatherParticleStorage = static_cast<const std::byte *>(HLib::GetPointerFromDynamicHeapOffsetBase(HLib::GetDynamicHeapOffset(particles)));
    SetLegacyWeatherRenderPass((flags & weatherParticleSpawnFlag) != 0);
    if ((flags & 0x20000000U) != 0) {
        weatherParticleStride = stride;
        weatherCurrentParticleFlags = flags;
        weatherParticleRender = DoRainParticleEffect;
    } else {
        weatherParticleNeedsUpdate = 0;
        weatherParticleRender = weatherParticleRenderFunctions[(flags >> 25U) & 7U];
#if defined(_WIN32)
        d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
        if (context) {
            LegacyEffectGpuFlags gpuFlags{};
            std::uint32_t texture{};
            std::memcpy(&gpuFlags, renderMode, sizeof(gpuFlags));
            std::memcpy(&texture, reinterpret_cast<const std::byte *>(renderMode) + 12, sizeof(texture));
            ApplyLegacyEffectRenderModes(d3t::GetRenderState(*context), gpuFlags, texture);
        }
#else
        (void)renderMode;
#endif
    }
    PublishWeatherParticleCallback(0);
    weatherParticleRender();
#if defined(_WIN32)
    if (d3t::DX11::g_ppD3DDeviceContext)
        RestoreLegacyEffectRenderFlags(d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext));
#endif
    WeatherParticleEndAnnotationNullsub();
}

void UpdateWeatherParticleCallback(void *data) {
    weatherScreenProjection.rows[0].x = 320.0f;
    weatherScreenProjection.rows[1].y = -240.0f;
    weatherScreenProjection.rows[3].x = 320.0f;
    weatherScreenProjection.rows[3].y = 240.0f;
    Matrix4x4_Push(&weatherScreenProjection);
    Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.entries[2].stack);
    weatherProjectionScratch = workTransformMatrix;
    Matrix4x4 projection{};
    Matrix4x4::GetTransformMatrix(&projection);
    weatherProjectionX = projection.rows[0].x;
    weatherProjectionFactors = {projection.rows[0].y, projection.rows[1].x, projection.rows[1].y};
    Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.top.stack - 1);
    WeatherParticleGroup *group = static_cast<WeatherParticleGroup *>(data);
    do {
        WeatherParticleGroup *const next = group->next;
        weatherCurrentGroup = group;
        if ((group->rainFlags & activeFlag) != 0 && group->rainDrawCount != 0 && group->rainParticles) {
            weatherParticleHalfWidth = group->rainPressureParam1;
            weatherParticleHalfHeight = group->rainPressureParam2;
            weatherProjectedParticleWidth = weatherParticleHalfWidth * weatherProjectionX;
            weatherProjectedParticleHeight = weatherParticleHalfHeight * weatherProjectionFactors.z;
            DispatchWeatherParticleLayer(group->rainFlags, group->rainParticles, group->rainDrawCount, 40, &group->rainRenderMode, "Rain");
        }
        if ((group->snowFlags & activeFlag) != 0 && group->snowDrawCount != 0 && group->snowParticles) {
            weatherParticleHalfWidth = group->snowDeltaTime;
            weatherProjectedParticleWidth = weatherParticleHalfWidth * weatherProjectionX;
            DispatchWeatherParticleLayer(group->snowFlags, group->snowParticles, group->snowDrawCount, 40, &group->snowRenderMode, "Snow");
        }
        if ((group->rippleFlags & activeFlag) != 0 && group->rippleDrawCount != 0 && group->rippleParticles) {
            weatherParticleDeltaTime = group->rippleDeltaTime;
            DispatchWeatherParticleLayer(group->rippleFlags, group->rippleParticles, group->rippleDrawCount, 56, &group->rippleRenderMode, "Ripples");
        }
        group = next;
    } while (group);
    Matrix4x4::Pop(1);
}

void WeatherParticle_Callback(void *data) {
    SetUpdateCallback(UpdateWeatherParticleCallback, data, 0.0f);
}
void BeginRainTransitionForFixc(WeatherFixcOwner *fixc, float rainValue, std::int32_t frames) {
    const std::uint32_t mode = static_cast<std::uint32_t>(fixc->mode);
    if (mode < 2U) {
        WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
        BeginRainGroupTransition(cara->rain0, rainValue, frames);
        BeginRainGroupTransition(cara->rain1, rainValue, frames);
    } else if (mode == 6U) {
        BeginRainGroupTransition(static_cast<WeatherParticleGroup *>(fixc->cara), rainValue, frames);
    }
}

void AdvanceWeatherInteractionCamera() {
    if (!weatherInteractionCamera || !g_cameraControl)
        return;
    weatherInteractionCamera->position = weatherInteractionCamera->nextCameraPosition;
    weatherInteractionCamera->currentCameraPosition = weatherInteractionCamera->currentCameraPosition2;
    weatherInteractionCamera->previousBackwardDirection = weatherInteractionCamera->backwardDirection;
    camCtrlGetNextCamPos(&weatherInteractionCamera->nextCameraPosition);
    (void)camCtrlGetTargetPosition(&weatherInteractionCamera->currentCameraPosition2);
    weatherInteractionCamera->backwardDirection = g_cameraControl->backwardsDirection;
    Vector3 &direction = weatherInteractionCamera->backwardDirection;
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (length != 0.0f) {
        direction.x /= length;
        direction.y /= length;
        direction.z /= length;
    }
}

void UpdateWeatherFixc(WeatherFixcOwner *fixc) {
    Vector3 windOffset{};
    for (WeatherFixcNode *node = fixc->windList; node; node = node->next) {
        Vector3 direction{};
        windGetDirectionVector(static_cast<WeatherWind *>(node->payload.pointer), &direction);
        windOffset.x += direction.x;
        windOffset.z += direction.z;
    }
    if (!weatherInteractionCamera)
        return;
    const std::uint32_t mode = static_cast<std::uint32_t>(fixc->mode);
    if (mode == 1U) {
        WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
        windOffset.x *= 8.0f;
        windOffset.y *= 8.0f;
        windOffset.z *= 8.0f;
        UpdateWeatherRainGroup(cara->rain0, weatherInteractionCamera->position, weatherInteractionCamera->currentCameraPosition,
            weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection, windOffset, true, 0);
        UpdateWeatherRainGroup(cara->rain1, weatherInteractionCamera->position, weatherInteractionCamera->currentCameraPosition,
            weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection, windOffset, true, 1);
        UpdateWeatherRainGroup(cara->rippleGroup, weatherInteractionCamera->position, weatherInteractionCamera->currentCameraPosition,
            weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection, windOffset, true, 0);
    } else if (mode == 0U) {
        WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
        UpdateWeatherRainGroup(cara->rain0, weatherInteractionCamera->position, weatherInteractionCamera->currentCameraPosition,
            weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection, windOffset, false, 0);
        UpdateWeatherRainGroup(cara->rain1, weatherInteractionCamera->position, weatherInteractionCamera->currentCameraPosition,
            weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection, windOffset, false, 1);
        UpdateWeatherRainGroup(cara->rippleGroup, weatherInteractionCamera->position, weatherInteractionCamera->currentCameraPosition,
            weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection, windOffset, false, 0);
    } else if (mode == 6U) {
        UpdateWeatherRainGroup(static_cast<WeatherParticleGroup *>(fixc->cara), weatherInteractionCamera->position,
            weatherInteractionCamera->currentCameraPosition, weatherInteractionCamera->currentCameraPosition2, weatherInteractionCamera->backwardDirection,
            windOffset, false, 0, true);
    }
}

float EffectDelta(float value) {
    return value * HLib::GetScaledDeltaTick();
}

float ParticleDrift(std::uint64_t value) {
    return std::bit_cast<float>(static_cast<std::uint32_t>(value));
}

void SetParticleDrift(std::uint64_t &word, float value) {
    word = (word & 0xffffffff00000000ULL) | std::bit_cast<std::uint32_t>(value);
}

void SetParticleAlpha(ParticleRecord &particle, float alpha) {
    particle.colour = (particle.colour & 0x00ffffffU) | (static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha)) << 24U);
}

template <typename Record> void BirthParticle(Record &particle);
void FadeSmoke(ParticleRecord &particle);
void FadeRadialSmoke(ParticleRecord &particle);
void FadeSpreadingSmoke(ParticleRecord &particle);
void FadeWideSmoke(ParticleRecord &particle);

void IntegrateParticle(ParticleRecord &particle) {
    particle.position.x = EffectDelta(particle.velocity.x) + particle.position.x;
    particle.position.y = EffectDelta(particle.velocity.y) + particle.position.y;
    particle.position.z = EffectDelta(particle.velocity.z) + particle.position.z;
}

void RotateParticle(ParticleRecord &particle) {


    const auto step = static_cast<std::int32_t>(particle.stepY);
    particle.phaseY += static_cast<std::uint32_t>(static_cast<std::int32_t>(EffectDelta(static_cast<float>(step))));
}

void RetireParticle(ParticleRecord &particle) {
    particle.flags = 0;
    particle.callback = BirthParticle;
}

float NextParticleOffset() {
    particleRandomState *= 1103515245ULL;
    std::uint32_t taps = (particleRandomState >> 16U) & 0x7fffU;
    for (std::uint32_t shift = 1; shift != 6; ++shift) {
        taps += (particleRandomState >> shift) & 0x7fffU;
    }
    return static_cast<float>(taps) * std::bit_cast<float>(std::uint32_t{939524092}) - 3.0f;
}

// AM2_Effect_Code: a successful BirthParticle installs the initializer;
// it does not invoke it until the next renderer visit to this particle.
template <typename Record> void BirthParticle(Record &particle) {
    ParticleEmitter &emitter = *currentEmitter;
    particleRandomState *= 1103515245ULL;
    float threshold = static_cast<float>(emitter.birthThreshold);
    if (emitter.birthThreshold < 0)
        threshold += 1.8446744e19f;
    threshold = EffectDelta(threshold);
    std::uint64_t high = 0;
    if (threshold >= 9.223372e18f) {
        threshold -= 9.223372e18f;
        if (threshold < 9.223372e18f)
            high = 0x8000000000000000ULL;
    }
    const std::uint64_t cutoff = high + static_cast<std::uint32_t>(static_cast<std::int32_t>(threshold));
    if (((particleRandomState >> 16U) & 0x7fffU) >= cutoff)
        return;
    particle.callback = reinterpret_cast<decltype(particle.callback)>(emitter.initializer);
    particle.position.x = NextParticleOffset() * emitter.spawnVariance.x + emitter.position.x;
    particle.position.y = NextParticleOffset() * emitter.spawnVariance.y + emitter.position.y;
    particle.position.z = NextParticleOffset() * emitter.spawnVariance.z + emitter.position.z;
    particle.velocity.x = NextParticleOffset() * emitter.velocityVariance.x + emitter.velocity.x;
    particle.velocity.y = NextParticleOffset() * emitter.velocityVariance.y + emitter.velocity.y;
    particle.velocity.z = NextParticleOffset() * emitter.velocityVariance.z + emitter.velocity.z;
    if constexpr (sizeof(Record) == sizeof(ParticleRecord)) {
        if ((emitter.flags & 0x20000000U) == 0)
            particle.colour = emitter.colour;
    }
}



void GrowSmoke(ParticleRecord &particle) {
    IntegrateParticle(particle);
    RotateParticle(particle);
    particle.scale = EffectDelta(smokeExpansion[0]) + particle.scale;
    const auto alpha = static_cast<std::int32_t>(EffectDelta(6.0f)) + static_cast<std::int32_t>(particle.colour >> 24U);
    const float maximum = (1.0f - smokeFadeFactor[0] * particle.scale) * 128.0f;
    if (static_cast<float>(alpha) < maximum) {
        SetParticleAlpha(particle, static_cast<float>(alpha));
    } else {
        SetParticleAlpha(particle, maximum);
        particle.callback = FadeSmoke;
    }
}

void FadeSmoke(ParticleRecord &particle) {
    IntegrateParticle(particle);
    RotateParticle(particle);
    particle.scale = EffectDelta(smokeExpansion[0]) + particle.scale;
    const float alpha = 1.0f - particle.scale * smokeFadeFactor[0];
    if (alpha < 0.0f)
        RetireParticle(particle);
    SetParticleAlpha(particle, alpha * 128.0f);
}

void InitializeSmoke(ParticleRecord &particle) {
    particle.scale = smokeInitialScale[0];
    particle.callback = GrowSmoke;
    particle.flags = activeFlag;
    particle.phaseY = IncrementSharedPrngUInt16();
    particle.stepY = 80;
}

// MoveStationarySmoke's profile one never rotates, expands or self-retires.
void MoveStationarySmoke(ParticleRecord &particle) {
    IntegrateParticle(particle);
    SetParticleAlpha(particle, stationarySmokeAlpha);
}

void InitializeStationarySmoke(ParticleRecord &particle) {
    particle.flags = activeFlag;
    particle.callback = MoveStationarySmoke;
    particle.scale = 1.0f;
}

// MoveRadialSmoke and its fade stages advance alpha by exactly one
// per visit; its movement, unlike that byte increment, is delta-scaled.
void MoveRadialSmoke(ParticleRecord &particle) {
    const std::uint32_t alpha = particle.colour >> 24U;
    const auto angle = static_cast<std::int32_t>((static_cast<float>(alpha * 2U) * 8192.0f) / 45.0f);
    const float wave = sin_fixed16(static_cast<std::int16_t>(static_cast<std::uint16_t>(angle)));
    particle.position.x = EffectDelta(wave * particle.velocity.x) + particle.position.x;
    particle.position.y = EffectDelta(particle.velocity.y) + particle.position.y;
    particle.position.z = EffectDelta(wave * particle.velocity.z) + particle.position.z;
    std::uint32_t nextAlpha = alpha + 1U;
    if (nextAlpha >= 0x7fU) {
        particle.velocity.x = IncrementSharedPrngFloat() * 0.0020000001f - 0.001f;
        particle.velocity.z = IncrementSharedPrngFloat() * 0.0020000001f - 0.001f;
        const float rise = IncrementSharedPrngFloat() * particle.velocity.y;
        nextAlpha = 127;
        particle.callback = FadeRadialSmoke;
        particle.velocity.y = rise + 0.001f;
    }
    SetParticleAlpha(particle, static_cast<float>(nextAlpha));
}

void FadeRadialSmoke(ParticleRecord &particle) {
    IntegrateParticle(particle);
    RotateParticle(particle);
    particle.scale = EffectDelta(smokeExpansion[2]) + particle.scale;
    const float alpha = 1.0f - particle.scale * smokeFadeFactor[2];
    if (alpha < 0.0f)
        RetireParticle(particle);
    SetParticleAlpha(particle, (alpha * 0.80000001f) * 128.0f);
}

void InitializeRadialSmoke(ParticleRecord &particle) {
    particle.scale = smokeInitialScale[2];
    particle.callback = MoveRadialSmoke;
    particle.flags = activeFlag;
    particle.phaseY = 0;
    particle.stepY = 80;
    particle.velocity.x = sin_fixed16(static_cast<std::int16_t>(static_cast<std::uint16_t>(radialSmokeAngle))) * 0.00039999999f;
    particle.velocity.z = sin_fixed16(static_cast<std::int16_t>(
                              fixed16_cosine_angle(static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::uint16_t>(radialSmokeAngle)))))) *
        0.00039999999f;
    radialSmokeAngle =
        static_cast<std::int32_t>(static_cast<std::uint32_t>(radialSmokeAngle) + static_cast<std::uint32_t>(static_cast<std::int32_t>(EffectDelta(546.0f))));
    if (particle.velocity.x < 0.0f)
        particle.stepY = 0ULL - particle.stepY;
    if (particle.velocity.z < 0.0f)
        particle.stepY = 0ULL - particle.stepY;
}

void IntegrateSpreadingSmoke(ParticleRecord &particle, std::size_t profile) {
    const float driftX = ParticleDrift(particle.stepX);
    const float driftZ = ParticleDrift(particle.stepZ);
    particle.position.x = EffectDelta(particle.velocity.x + driftX) + particle.position.x;
    particle.position.y = EffectDelta(particle.velocity.y) + particle.position.y;
    particle.position.z = EffectDelta(particle.velocity.z + driftZ) + particle.position.z;
    SetParticleDrift(particle.stepX, smokeRadialDecay[profile] * driftX);
    SetParticleDrift(particle.stepZ, smokeRadialDecay[profile] * driftZ);
    RotateParticle(particle);
    particle.scale = EffectDelta(smokeExpansion[profile]) + particle.scale;
}

void GrowSpreadingSmokeProfile(ParticleRecord &particle, std::size_t profile, ParticleCallback fade) {
    IntegrateSpreadingSmoke(particle, profile);
    const auto alpha =
        static_cast<std::int32_t>(EffectDelta(static_cast<float>(smokeAlphaStep[profile]) + 1.0f)) + static_cast<std::int32_t>(particle.colour >> 24U);
    const float maximum = 1.0f - (smokeFadeFactor[profile] * particle.scale) * smokeOpacityScale[profile];
    if (maximum < 0.1f) {
        RetireParticle(particle);
    } else if (static_cast<double>(alpha) < static_cast<double>(maximum) * 128.0) {
        SetParticleAlpha(particle, static_cast<float>(alpha));
    } else {
        SetParticleAlpha(particle, maximum * 128.0f);
        particle.callback = fade;
    }
}

void FadeSpreadingSmokeProfile(ParticleRecord &particle, std::size_t profile) {
    IntegrateSpreadingSmoke(particle, profile);
    const float alpha = 1.0f - (particle.scale * smokeFadeFactor[profile]) * smokeOpacityScale[profile];
    if (alpha < 0.1f)
        RetireParticle(particle);
    SetParticleAlpha(particle, alpha * 128.0f);
}

void GrowSpreadingSmoke(ParticleRecord &particle) {
    GrowSpreadingSmokeProfile(particle, 3, FadeSpreadingSmoke);
}
void FadeSpreadingSmoke(ParticleRecord &particle) {
    FadeSpreadingSmokeProfile(particle, 3);
}
void GrowWideSmoke(ParticleRecord &particle) {
    GrowSpreadingSmokeProfile(particle, 4, FadeWideSmoke);
}
void FadeWideSmoke(ParticleRecord &particle) {
    FadeSpreadingSmokeProfile(particle, 4);
}

void InitializeSpreadingSmokeProfile(ParticleRecord &particle, std::size_t profile, ParticleCallback callback) {
    const auto angle = static_cast<std::uint16_t>(2 * IncrementSharedPrngUInt16());
    SetParticleDrift(particle.stepX,
        sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(static_cast<std::int16_t>(angle))))) *
            smokeRadialVelocity[profile]);
    const float driftZ = sin_fixed16(static_cast<std::int16_t>(angle)) * smokeRadialVelocity[profile];
    particle.flags = activeFlag;
    particle.callback = callback;
    SetParticleDrift(particle.stepZ, driftZ);
    particle.scale = smokeInitialScale[profile];
    particle.phaseY = IncrementSharedPrngUInt16();
    particle.stepY = 80;
}

void InitializeSpreadingSmoke(ParticleRecord &particle) {
    InitializeSpreadingSmokeProfile(particle, 3, GrowSpreadingSmoke);
}
void InitializeWideSmoke(ParticleRecord &particle) {
    InitializeSpreadingSmokeProfile(particle, 4, GrowWideSmoke);
}

void EffectControlCallback(void *);
void SmokeControlCallback(void *);
void StationarySmokeControlCallback(void *);
void RadialSmokeControlCallback(void *);
void SpreadingSmokeControlCallback(void *);
void WideSmokeControlCallback(void *);
void FireControlCallback(void *);

std::uint32_t effectTaskReferences{};
std::array<LegacyEffectTexture, 16> smokeTextureRecords{};

template <std::size_t Count>
std::int32_t AllocateEffectSlot(std::array<EffectSlot, Count> &slots, std::int32_t textureSlot, HLib::TaskCallback callback, void *data, shen_name token) {
    for (std::size_t index = 0; index != Count; ++index) {
        EffectSlot &slot = slots[index];
        if (slot.state != 0)
            continue;
        slot.token = token;
        slot.state = 1;
        slot.textureSlot = textureSlot;
        slot.callback = callback;
        slot.data = data;
        return static_cast<std::int32_t>(index);
    }
    return -1;
}

void ClearEffectSlot(EffectSlot &slot) {
    slot.state = 0;
    slot.link = -1;
    slot.textureSlot = -1;
    slot.callback = nullptr;
    slot.data = nullptr;
    slot.token = shen_name{0x20202020U};
    slot.reserved28 = 0;
}

void SetTextureRecordHandle(FireFftGraphicsRecord &record, std::uint32_t handle) {
    const std::uint64_t nativeHandle = handle;
    std::memcpy(record.storage.data() + 0x20, &nativeHandle, sizeof(nativeHandle));
}

void WriteSmokeDdsHeader(std::byte *data) {
    std::array<std::uint32_t, 32> header{};
    header[0] = make_shen_name('D', 'D', 'S', ' ');
    header[1] = 124;
    header[2] = 4111;
    header[3] = 16;
    header[4] = 16;
    header[5] = 64;
    constexpr std::array<std::uint32_t, 8> pixelFormat{
        32, 65, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000};
    std::copy(pixelFormat.begin(), pixelFormat.end(), header.begin() + 19);
    header[27] = 4096;
    std::memcpy(data, header.data(), sizeof(header));
}

void CreateAuthoredParticleTexture(
    std::int32_t emitterIndex, std::uint32_t width, std::uint32_t height, std::uint16_t format, const std::byte *pixels, std::size_t size, bool rawDds = false);

void UploadSmokeTexture(SmokeEffect &effect) {
    CreateAuthoredParticleTexture(effect.slot, 16, 16, 0x8083U, effect.texture->pixels, 0x480, true);
}

void CreateSmokeTexture(SmokeEffect &effect) {
    WriteSmokeDdsHeader(effect.texture->pixels);
    UploadSmokeTexture(effect);
}

SmokeEffect *&ParticleEffectEntry(std::int32_t type, std::uint32_t index) {
    const auto signedIndex = static_cast<std::int32_t>(index);
    switch (type) {
    case 0:
        return smokeEffects[signedIndex];
    case 1:
        return splashEffects[signedIndex];
    case 2:
        return reservedParticleEffects[signedIndex];
    default:
        return fallingEffects[signedIndex];
    }
}

SmokeEffect *FindSmokeEffect(std::int32_t type, std::uint32_t index) {
    if (index == invalidEffect || static_cast<std::uint32_t>(type) > 3U)
        return nullptr;
    return ParticleEffectEntry(type, index);
}

FireEffect *FindFireEffect(std::uint32_t index) {
    return index != invalidEffect ? fireEffects[static_cast<std::int32_t>(index)] : nullptr;
}

void SetEmitterSize(ParticleEmitter &emitter, float width, float height) {
    emitter.halfWidth = (emitter.flags & 0x20000000U) ? width : width * 0.5f;
    emitter.halfHeight = (emitter.flags & 0x20000000U) ? height : height * 0.5f;
}

void SetEmitterBirthRate(ParticleEmitter &emitter, float fraction) {
    emitter.birthThreshold = _mm_cvttsd_si32(_mm_set_sd(static_cast<double>(fraction) * 32767.0));
}

void RegisterEmitter(SmokeEffect &effect, ParticleCallback initializer) {
    ParticleEmitter &emitter = emitters[static_cast<std::size_t>(effect.slot)];
    emitter.count = effect.count;
    emitter.particles = effect.particles;
    emitter.flags &= ~0x20000000U;
    emitter.textureFlags = 0x80000002U;
    emitter.samplerControl = 0x88800000U;
    emitter.initializer = initializer;
    emitter.position = effect.position;
    emitter.spawnVariance = effect.spawnVariance;
    emitter.velocity = effect.velocity;
    emitter.velocityVariance = effect.velocityVariance;
    SetEmitterBirthRate(emitter, effect.birthRate);
    SetEmitterSize(emitter, effect.width, effect.height);
    ParticleRecord initial{};
    initial.callback = BirthParticle;
    initial.colour = 0xffffffffU;
    initial.scale = 1.0f;
    for (std::int32_t index = 0; index < effect.count; ++index) {
        effect.particles[index] = initial;
    }
}

void EnableEmitter(ParticleEmitter &emitter) {
    emitter.gpuFlags = ((emitter.gpuFlags | 0x100000U) & 0x00ffffffU) | 0x94000000U;
    emitter.textureFlags |= 0x02000000U;
    emitter.flags |= 0x40000000U;
}

void CreateAuthoredParticleTexture(
    std::int32_t emitterIndex, std::uint32_t width, std::uint32_t height, std::uint16_t format, const std::byte *pixels, std::size_t size, bool rawDds) {
    ParticleEmitter &emitter = emitters[emitterIndex];
    if (emitter.textureHandle) {
        d3t::InitTexture2DResource(emitter.textureHandle);
        smokeTextureRecords[emitterIndex].handle = 0;
        --g_legacyEffectTextureCount;
    }
    LegacyEffectTexture &texture = smokeTextureRecords[emitterIndex];
    emitter.textureRecord = &texture;
    if (InitializeLegacyEffectTexture(texture, width, height, format) != 0)
        return;
    if (rawDds ? !d3t::ReplaceDecodedImage(static_cast<std::uint32_t>(texture.handle), pixels, size, true) :
                 UploadLegacyEffectPvr(texture, {pixels, size}) != 0)
        return;
    emitter.textureHandle = static_cast<std::uint32_t>(texture.handle);
    smokeTextureHandles[emitterIndex] = emitter.textureHandle;
    std::uint32_t widthBits = 0, heightBits = 0;
    for (std::uint32_t value = width; value; value >>= 1U)
        ++widthBits;
    for (std::uint32_t value = height; value; value >>= 1U)
        ++heightBits;
    emitter.textureFlags = 0x8000000aU;
    emitter.samplerControl = 0x8a800000U;
    emitter.gpuFlags = (8U * widthBits - 32U) | (heightBits - 4U) | 0x940242c0U;
    if (emitter.flags & 0x40000000U) {
        emitter.gpuFlags = ((emitter.gpuFlags | 0x100000U) & 0x00ffffffU) | 0x94000000U;
        emitter.textureFlags |= 0x02000000U;
    } else {
        emitter.gpuFlags = ((emitter.gpuFlags & 0x6bedbd3fU) | 0x940242c0U) & 0x00ffffffU;
        emitter.gpuFlags |= 0x20000000U;
    }
    emitter.flags |= 0x10000000U;
    emitter.textureSource = rawDds ? pixels : pixels + 28;
}

void AdvanceParticlePhase(std::uint64_t &phase, std::uint64_t step) {
    phase += static_cast<std::uint32_t>(static_cast<std::int32_t>(EffectDelta(static_cast<float>(static_cast<std::int32_t>(step)))));
}

void MoveSplashParticle(ParticleRecord &particle) {
    const float floor = *reinterpret_cast<const float *>(particle.stepX);
    if (floor <= particle.position.y) {
        particle.position.y = EffectDelta(particle.velocity.y) + particle.position.y;
        particle.position.x = EffectDelta(particle.velocity.x) + particle.position.x;
        particle.position.z = EffectDelta(particle.velocity.z) + particle.position.z;
        particle.velocity.y -= 0.0049999999f;
    } else {
        RetireParticle(particle);
    }
}

void MoveShrinkingSplashParticle(ParticleRecord &particle) {
    MoveSplashParticle(particle);
    if (particle.scale > 0.001f)
        particle.scale -= EffectDelta(particle.scale <= 0.1f ? 0.001f : 0.0099999998f);
}

void MoveWideSplashParticle(ParticleRecord &particle) {
    MoveSplashParticle(particle);
    if (particle.scale > 0.2f)
        particle.scale -= EffectDelta(0.0049999999f);
}

void InitializeSplashParticle(ParticleRecord &particle) {
    particle.flags = activeFlag;
    particle.callback = MoveSplashParticle;
}

void InitializeShrinkingSplashParticle(ParticleRecord &particle) {
    particle.flags = activeFlag;
    particle.callback = MoveShrinkingSplashParticle;
}

void InitializeRadialSplashParticle(ParticleRecord &particle) {
    particle.callback = MoveShrinkingSplashParticle;
    particle.flags = activeFlag;
    const auto angle = static_cast<std::uint16_t>(2U * IncrementSharedPrngUInt16());
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    particle.position.x = cosine * EffectDelta(particle.velocity.x) + particle.position.x;
    particle.position.z = sine * EffectDelta(particle.velocity.x) + particle.position.z;
    particle.velocity.x = cosine * particle.velocity.z;
    particle.velocity.z = sine * particle.velocity.z;
    particle.stepZ = 0;
}

void InitializeWideSplashParticle(ParticleRecord &particle) {
    particle.flags = activeFlag;
    particle.callback = MoveWideSplashParticle;
}

void SplashControlCallback(void *data) {
    SmokeEffect &effect = *static_cast<SmokeEffect *>(data);
    if ((effect.flags.name & 1U) == 0)
        return;
    const LegacyInputSlot &first = g_inputBuffer[1];
    const LegacyInputSlot &second = g_inputBuffer[2];
    const auto velocity = [](std::uint8_t value) { return float(value) / 255.0f * 0.2f - 0.1f; };
    effect.velocity = {velocity(first.analog0c), velocity(first.analog0d), velocity(first.analog0e)};
    effect.velocityVariance = {velocity(second.analog0f), velocity(second.analog10), velocity(second.analog11)};
    ParticleEmitter &emitter = emitters[effect.slot];
    emitter.velocity = effect.velocity;
    emitter.velocityVariance = effect.velocityVariance;
    if (g_windowParticleDebugFlags & 0x0800U) {
        effect.birthRate = float(first.analog0f) / 255.0f * 0.1f;
        SetEmitterBirthRate(emitter, effect.birthRate);
    }
    effect.width = float(first.analog10) / 255.0f * 0.1f;
    effect.height = effect.width;
    SetEmitterSize(emitter, effect.width, effect.height);
    emitter.colour = std::uint32_t(second.analog0e) | (std::uint32_t(second.analog0d) << 8U) | (std::uint32_t(second.analog0c) << 16U) |
        (std::uint32_t(first.analog11) << 24U);
    for (std::int32_t index = 0; index < effect.count; ++index)
        effect.particles[index].colour = emitter.colour;
}

void MoveFallingParticle(ParticleRecord &particle) {
    AdvanceParticlePhase(particle.phaseY, particle.stepY);
    AdvanceParticlePhase(particle.phaseZ, particle.stepZ);
    if (!(fallingParticleFloor <= particle.position.y)) {
        RetireParticle(particle);
        return;
    }
    float vertical = sin_fixed16(static_cast<std::int16_t>(particle.phaseY)) * particle.velocity.y;
    if (vertical > 0.0f)
        vertical = -vertical;
    if (std::fabs(particle.velocity.x) + std::fabs(particle.velocity.z) < 0.016000001f) {
        vertical = std::fmin(-0.016000001f, vertical);
        particle.phaseZ +=
            static_cast<std::uint32_t>(static_cast<std::int32_t>(EffectDelta(static_cast<float>(static_cast<std::int32_t>(particle.stepZ)) * 8.0f)));
    }
    particle.position.y = EffectDelta(vertical) + particle.position.y;
    const float sine = sin_fixed16(static_cast<std::int16_t>(particle.phaseY));
    particle.position.x = EffectDelta(sine * particle.velocity.x) + particle.position.x;
    particle.position.z = EffectDelta(sine * particle.velocity.z) + particle.position.z;
}

void MoveDriftingParticle(ParticleRecord &particle) {
    AdvanceParticlePhase(particle.phaseY, particle.stepY);
    AdvanceParticlePhase(particle.phaseZ, particle.stepZ);
    if (fallingParticleFloor > particle.position.y) {
        RetireParticle(particle);
        return;
    }
    const auto angle = static_cast<std::uint16_t>(particle.phaseY);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    particle.position.y = EffectDelta(particle.velocity.y - std::fabs(cosine * 0.015f) - 0.016000001f) + particle.position.y;
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    particle.position.x = EffectDelta(sine * 0.0099999998f + particle.velocity.x) + particle.position.x;
    particle.position.z = EffectDelta(sine * 0.0099999998f + particle.velocity.z) + particle.position.z;
    const float x = particle.position.x - fallingParticleOrigins[0].x;
    const float z = particle.position.z - fallingParticleOrigins[0].z;
    if (x * x + z * z > 64.0f)
        RetireParticle(particle);
}

Vector4 SampleFallingParticleCurve(const Vector4 *points, float time) {
    const float cubic = time * time * time;
    const float inverse = 1.0f - time;
    const float a = inverse * inverse * inverse;
    const float b = inverse * time * inverse * 3.0f;
    const float c = inverse * (time * time) * 3.0f;
    return {((a * points[0].x + b * points[1].x) + c * points[2].x) + cubic * points[3].x,
        ((b * points[1].y + a * points[0].y) + c * points[2].y) + cubic * points[3].y,
        ((b * points[1].z + a * points[0].z) + c * points[2].z) + cubic * points[3].z,
        ((b * points[1].w + a * points[0].w) + c * points[2].w) + cubic * points[3].w};
}

void MoveCurvedParticle(ParticleRecord &particle) {
    constexpr std::array<Vector4, 7> points{{
        {0.0f, 0.0f, 0.0f, 0.2f}, {0.0f, 1.5f, 0.0f, 0.6f},
        {-1.5f, 1.5f, 0.0f, 0.8f}, {-1.5f, 2.0f, -1.5f, 0.8f},
        {-1.5f, 2.5f, -3.0f, 0.8f}, {1.0f, 2.5f, -3.0f, 0.8f},
        {1.0f, 5.0f, -2.5f, 2.0f}}};
    Vector4 curve{};
    if (particle.velocity.y >= 2.0f)
        RetireParticle(particle);
    else
        curve = SampleFallingParticleCurve(
            points.data() + (particle.velocity.y >= 1.0f ? 3 : 0), particle.velocity.y >= 1.0f ? particle.velocity.y - 1.0f : particle.velocity.y);
    const Vector3 &origin = fallingParticleOrigins[1];
    particle.position.x = (origin.x + curve.x) + sin_fixed16(static_cast<std::int16_t>(particle.phaseY)) * curve.w;
    particle.position.y = (origin.y + curve.y) + sin_fixed16(static_cast<std::int16_t>(3ULL * (particle.phaseY / 2ULL))) * (curve.w * 0.30000001f);
    particle.position.z = (origin.z + curve.z) + sin_fixed16(static_cast<std::int16_t>(5ULL * (particle.phaseY / 2ULL))) * curve.w;
    AdvanceParticlePhase(particle.phaseY, particle.stepY);
    AdvanceParticlePhase(particle.phaseZ, particle.stepZ);
    particle.velocity.y += 0.022222223f;
}

void MoveVortexParticle(ParticleRecord &particle) {
    if (particle.position.y < -0.5f || particle.position.y > 8.0f) {
        RetireParticle(particle);
        return;
    }
    const float z = fallingParticleOrigins[2].z - particle.position.z;
    const float x = fallingParticleOrigins[2].x - particle.position.x;
    const float distance = std::fmax(0.0099999998f, z * z + x * x);
    const auto angle = static_cast<std::uint16_t>(particle.phaseY);
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    particle.position.x = EffectDelta(sine * 0.0099999998f + particle.velocity.x) + particle.position.x;
    particle.position.y = EffectDelta(particle.velocity.y - std::fabs(cosine * 0.015f) - 0.016000001f) + particle.position.y;
    particle.position.z = EffectDelta(sine * 0.0099999998f + particle.velocity.z) + particle.position.z;
    const float towardX = x / distance;
    const float towardZ = z / distance;
    particle.velocity.x = EffectDelta(towardX * 0.014117648f - towardZ * 0.0019607844f) + particle.velocity.x;
    particle.velocity.y = (std::fabs(sine) / distance) * 0.077254906f;
    particle.velocity.z = (EffectDelta(towardZ * 0.014117648f + towardX * 0.0019607844f) + particle.velocity.z) * 0.97843134f;
    particle.velocity.x *= 0.97843134f;
    AdvanceParticlePhase(particle.phaseY, particle.stepY);
    AdvanceParticlePhase(particle.phaseZ, particle.stepZ);
}

void MoveLitFallingParticle(ParticleRecord &particle) {
    const Vector3 colour = GetLegacyParticleLightColour();
    particle.position.x = EffectDelta(particle.velocity.x) + particle.position.x;
    AdvanceParticlePhase(particle.phaseX, particle.stepX);
    particle.position.y = EffectDelta(particle.velocity.y) + particle.position.y;
    AdvanceParticlePhase(particle.phaseY, particle.stepY);
    particle.position.z = EffectDelta(particle.velocity.z) + particle.position.z;
    AdvanceParticlePhase(particle.phaseZ, particle.stepZ);
    const Vector3 light = GetLegacyParticleLightDirection();
    float intensity = GetLegacyParticleLightIntensity();
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(EffectDelta(float(static_cast<std::int32_t>(particle.phaseZ))))));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(EffectDelta(float(static_cast<std::int32_t>(particle.phaseY))))));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(static_cast<std::int32_t>(EffectDelta(float(static_cast<std::int32_t>(particle.phaseX))))));
    Matrix4x4 transform;
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    const float dot = (light.x * -transform.rows[0].z + light.y * -transform.rows[1].z) + light.z * -transform.rows[2].z;
    if (dot > 0.0f)
        intensity += dot;
    const float gain = std::fmin(1.0f, intensity) * 255.0f;
    particle.colour = std::uint8_t(static_cast<std::int32_t>(colour.z * gain)) |
        (std::uint32_t(std::uint8_t(static_cast<std::int32_t>(colour.y * gain))) << 8U) |
        (std::uint32_t(std::uint8_t(static_cast<std::int32_t>(colour.x * gain))) << 16U) | 0xff000000U;
    if (!(fallingParticleFloor <= particle.position.y))
        particle.callback = BirthParticle;
}

void InitializeFallingParticle(ParticleRecord &particle) {
    particle.flags = 0x90000000U;
    particle.callback = MoveFallingParticle;
    particle.phaseX = IncrementSharedPrngUInt16();
    particle.phaseY = IncrementSharedPrngUInt16();
    particle.phaseZ = IncrementSharedPrngUInt16();
    particle.stepY = 512;
    particle.stepZ = 512;
}

void InitializeDriftingParticle(ParticleRecord &particle) {
    InitializeFallingParticle(particle);
    particle.callback = MoveDriftingParticle;
    particle.stepY = (IncrementSharedPrngUInt16() >> 4U) + 256;
}

void InitializeCurvedParticle(ParticleRecord &particle) {
    InitializeFallingParticle(particle);
    particle.callback = MoveCurvedParticle;
    particle.stepY = (IncrementSharedPrngUInt16() >> 5U) + 256;
    particle.velocity.y = 0.0f;
}

void InitializeVortexParticle(ParticleRecord &particle) {
    InitializeFallingParticle(particle);
    particle.callback = MoveVortexParticle;
    particle.stepY = (IncrementSharedPrngUInt16() >> 5U) + 512;
}

void InitializeLitFallingParticle(ParticleRecord &particle) {
    particle.flags = activeFlag;
    particle.callback = MoveLitFallingParticle;
    particle.phaseX = particle.phaseY = particle.phaseZ = 0;
    particle.stepX = IncrementSharedPrngUInt16() & 0xfffU;
    particle.stepY = IncrementSharedPrngUInt16() & 0xfffU;
    particle.stepZ = IncrementSharedPrngUInt16() & 0xfffU;
    particle.colour = 0xff000000U;
}

// UpdateSmokeControls profiles read physical input slots 1..3.
// Their globals alias the ordinary 24-byte g_inputBuffer records.
void UpdateSmokeControls(SmokeEffect &effect, std::size_t profile) {
    if ((effect.flags.name & 1U) == 0)
        return;
    const LegacyInputSlot &first = g_inputBuffer[1];
    const LegacyInputSlot &second = g_inputBuffer[2];
    const LegacyInputSlot &third = g_inputBuffer[3];
    SmokeFftData &fft = *smokeFft[effect.texture->slot];
    std::array<float, 64> falloff;
    GenerateSmokeFalloff(float(third.analog0c) * 10.0f / 255.0f - 2.0f, falloff);
    ScaleSmokeFalloff(float(third.analog0d) * 100.0f / 255.0f, falloff);
    SetSmokeFftFalloff(fft, falloff);
    effect.fadeFactor = float(third.analog0d) / 255.0f + 0.69999999f;
    effect.expansion = float(third.analog0c) / 255.0f * 0.02f - 0.0039999997f;
    if (profile != 1) {
        smokeFadeFactor[profile] = effect.fadeFactor;
        smokeExpansion[profile] = effect.expansion;
    } else {
        stationarySmokeAlpha = float(first.analog11);
    }
    if (profile != 1 && profile != 2 && (second.held & 0x8001U) == 0x8001U) {
        const float amplitude = (float(second.analog11) - 128.0f) / 255.0f;
        SetSmokeFftAmplitude(fft, amplitude + amplitude);
    }
    ParticleEmitter &emitter = emitters[effect.slot];
    emitter.colour = std::uint32_t(second.analog0e) | (std::uint32_t(second.analog0d) << 8) | (std::uint32_t(second.analog0c) << 16);
    for (std::int32_t index = 0; index < effect.count; ++index) {
        ParticleRecord &particle = effect.particles[index];
        particle.colour = (particle.colour & 0xff000000U) | emitter.colour;
    }
    const float sizeScale = profile == 2 ? 0.2f : 10.0f;
    effect.height = float(second.analog0f) / 255.0f * sizeScale;
    effect.width = float(second.analog10) / 255.0f * sizeScale;
    SetEmitterSize(emitter, effect.width, effect.height);
    const float velocityScale = profile == 2 ? 0.003f : 0.1f;
    const float centre = profile >= 3 ? 127.0f : 0.0f;
    effect.velocity = {(float(third.analog0e) - centre) / 255.0f * velocityScale, (float(third.analog0f) - centre) / 255.0f * velocityScale,
        (float(third.analog10) - centre) / 255.0f * velocityScale};
    effect.velocityVariance = {
        float(first.analog0c) / 255.0f * velocityScale, float(first.analog0d) / 255.0f * velocityScale, float(first.analog0e) / 255.0f * velocityScale};
    emitter.velocity = effect.velocity;
    emitter.velocityVariance = effect.velocityVariance;
    effect.initialScale = float(third.analog11) / 255.0f * 0.5f;
    if (profile != 1)
        smokeInitialScale[profile] = effect.initialScale;
    if (profile < 3 || (g_windowParticleDebugFlags & 0x0200U) == 0) {
        effect.birthRate = float(first.analog0f) / 255.0f * 0.1f;
        SetEmitterBirthRate(emitter, effect.birthRate);
    }
    if (profile >= 3) {
        if (g_windowParticleDebugFlags < 0) {
            smokeAlphaStep[profile] = first.analog10 / 3U;
            smokeOpacityScale[profile] = float(first.analog11) / 255.0f * 10.0f;
        }
        if (g_windowParticleDebugFlags & 0x2000U) {
            smokeRadialVelocity[profile] = float(first.analog10) / 255.0f;
            smokeRadialDecay[profile] = float(first.analog11) / 255.0f;
        }
    }
}

void SmokeControlCallback(void *data) {
    UpdateSmokeControls(*static_cast<SmokeEffect *>(data), 0);
}
void StationarySmokeControlCallback(void *data) {
    UpdateSmokeControls(*static_cast<SmokeEffect *>(data), 1);
}
void RadialSmokeControlCallback(void *data) {
    UpdateSmokeControls(*static_cast<SmokeEffect *>(data), 2);
}
void SpreadingSmokeControlCallback(void *data) {
    UpdateSmokeControls(*static_cast<SmokeEffect *>(data), 3);
}
void WideSmokeControlCallback(void *data) {
    UpdateSmokeControls(*static_cast<SmokeEffect *>(data), 4);
}

void QueueCurrentFireQuad(std::size_t index, float alpha) {
    const FireFftData &fft = *fireFft[index];
    QueueFireQuad({workTransformMatrix, fft.width, fft.height, alpha, fireTextureHandles[index], fft.tick});
}

// FireControlCallback is separate from the earlier bone/trail/FFT update pass.
void FireControlCallback(void *data) {
    auto *const effect = static_cast<FireEffect *>(data);
    const std::int16_t yaw = g_cameraControl ? static_cast<std::int16_t>(-g_cameraControl->nextRotation.yaw) : 0;
    const float offsetX = -sin_fixed16(yaw);
    const float offsetZ = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw))));
    const bool draw = IsWazamakiInactive() == 0;
    if (!effect)
        return;
    FireFftData &fft = *fireFft[effect->slot];
    if (effect->drawFlags & 1U) {
        const LegacyInputSlot &first = g_inputBuffer[1];
        const LegacyInputSlot &second = g_inputBuffer[2];
        effect->height = float(second.analog0f) / 255.0f * 10.0f;
        effect->width = float(second.analog10) / 255.0f * 10.0f;
        SetFireFftSize(fft, effect->width, effect->height);
        std::array<float, 256> falloff{};
        GenerateFireFalloff(float(first.analog0c) / 255.0f * 10.0f, float(first.analog0d) / 255.0f * 10.0f, falloff);
        ScaleFireFalloff(float(first.analog0e) / 255.0f * 10.0f, falloff);
        SetFireFftFalloff(fft, falloff);
        SetFireFftScalar(fft, float(first.analog0f) / 255.0f * -0.5f);
    }
    if (effect->trail) {
        const float width = fft.width;
        const float height = fft.height;
        for (std::size_t index = 0; index < 512; ++index) {
            const FireTrailEntry &entry = effect->trail[index];
            if (entry.age < 0)
                continue;
            SetFireFftSize(fft, float(entry.age + 22) / 42.0f * width, float(entry.age + 1) / 21.0f * height);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(entry.position.x, float(entry.heightOffset) * 0.000099999997f + entry.position.y, entry.position.z);
            if (draw)
                QueueCurrentFireQuad(effect->slot, effect->alpha);
            Matrix4x4::Pop(1);
        }
        SetFireFftSize(fft, width, height);
    } else {
        Matrix4x4_Push();
        Matrix4x4::preTranslate(effect->worldPosition.x, effect->worldPosition.y, effect->worldPosition.z);
        Matrix4x4::preTranslate(effect->cameraOffset * offsetX, 0.0f, effect->cameraOffset * offsetZ);
        if (draw)
            QueueCurrentFireQuad(effect->slot, effect->alpha);
        Matrix4x4::Pop(1);
    }
    if (effect->instances) {
        const float width = fft.width;
        const float height = fft.height;
        for (std::size_t index = 0; index < 32; ++index) {
            const FireInstance &instance = effect->instances[index];
            if (!instance.enabled)
                continue;
            SetFireFftSize(fft, instance.width, instance.height);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(instance.worldPosition.x, instance.worldPosition.y, instance.worldPosition.z);
            Matrix4x4::preTranslate(instance.cameraOffset * offsetX, 0.0f, instance.cameraOffset * offsetZ);
            if (draw)
                QueueCurrentFireQuad(effect->slot, instance.alpha);
            Matrix4x4::Pop(1);
        }
        SetFireFftSize(fft, width, height);
    }
}

void UpdateFireAttachment(Character *character, std::int16_t bone, const Vector3 &local, Vector3 &world, float &alpha) {
    world = local;
    if (!character)
        return;
    MotionWork *const motion = FindMotionWork(character);
    Matrix4x4 transform;
    const Matrix4x4 *const source = motion                                                        ? charaGetBoneTransform(motion, bone) :
        ResolveCharacterDrawNodeTransform(character, static_cast<std::uint16_t>(bone), transform) ? &transform :
                                                                                                    nullptr;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(source);
    Matrix4x4::preTranslate(local.x, local.y, local.z);
    world = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    alpha = character->alpha;
}

void UpdateFireOwner(FireEffect &effect) {
    UpdateFireAttachment(effect.character, effect.bone, effect.localPosition, effect.worldPosition, effect.alpha);
    if (effect.trail) {
        constexpr std::array<std::int16_t, 10> seedAges{
            7, 20, 18, 10, 12, 5, 3, 1, 4, 7};
        for (std::int32_t index = 511; index >= 0; --index) {
            FireTrailEntry &entry = effect.trail[index];
            if (index > 10) {
                entry.age = effect.trail[index - 1].age;
            } else if (index == 10) {
                entry.age = static_cast<std::int16_t>(static_cast<double>(IncrementSharedPrngFloat()) * 30.0 - 15.0);
                if (entry.age < 0)
                    entry.age = static_cast<std::int16_t>(IncrementSharedPrngFloat() * 7.0f);
            } else {
                entry.age = seedAges[index];
            }
            entry.heightOffset = static_cast<std::int16_t>(IncrementSharedPrngFloat() * 400.0f - 350.0f);
            entry.position = index == 0 ? effect.worldPosition : effect.trail[index - 1].position;
        }
    }
    if (effect.instances) {
        for (std::size_t index = 0; index < 32; ++index) {
            FireInstance &instance = effect.instances[index];
            if (!instance.enabled)
                continue;
            UpdateFireAttachment(instance.character, instance.bone, instance.localPosition, instance.worldPosition, instance.alpha);
        }
    }
    FireFftData &fft = *fireFft[effect.slot];
    if ((fft.flags & activeFlag) == 0)
        return;
    UpdateFireFft(fft, effect.flags);
    std::array<std::uint32_t, 1024> pixels;
    for (std::size_t index = 0; index < pixels.size(); ++index) {
        const std::uint32_t packed = fft.output[index];
        pixels[index] = (((packed >> 8) & 15U) | (((packed >> 4) & 15U) << 8) | ((packed & 15U) << 16) | ((packed >> 12) << 24)) * 17U;
    }
    const std::size_t output = (fft.tick & 1U) == 0;
    auto &handle = fireTextureHandles[effect.slot][output];
    d3t::InitTexture2DResource(handle);
    const d3t::Texture2DDescription description{32, 32, 1, 1, 2, 0, 0, 1, 0, 0, pixels.data()};
    handle = d3t::CreateTexture2D(&description);
    SetTextureRecordHandle(output ? fft.graphicsRecord1 : fft.graphicsRecord0, handle);
}

void DrawEmitter(void *data) {
    const std::size_t index = reinterpret_cast<std::uintptr_t>(data);
    ParticleEmitter &emitter = emitters[index];
    if ((emitter.flags & activeFlag) == 0)
        return;
    currentEmitter = &emitter;
    const Matrix4x4 transform = workTransformMatrix;
    const bool update = (emitter.flags & suspendedFlag) == 0;
    const bool draw = (emitter.flags & 0x04000000U) == 0;
    if (draw &&
        !BeginSmokeDraw(static_cast<std::uint32_t>(emitter.count), emitter.flags, emitter.textureHandle,
            {emitter.textureFlags, emitter.samplerControl, emitter.gpuFlags})) {
        return;
    }
    if (emitter.flags & 0x20000000U) {
        CompactParticleRecord *const particles = reinterpret_cast<CompactParticleRecord *>(emitter.particles);
        for (std::int32_t particleIndex = 0; particleIndex < emitter.count; ++particleIndex) {
            if (update)
                particles[particleIndex].callback(particles[particleIndex]);
        }
        if (draw)
            EndSmokeDraw();
        return;
    }
    for (std::int32_t particleIndex = 0; particleIndex < emitter.count; ++particleIndex) {
        ParticleRecord &particle = emitter.particles[particleIndex];
        if (update)
            particle.callback(particle);
        if (draw && (particle.flags & activeFlag) != 0) {
            AppendSmokeBillboard(workTransformMatrix, particle.position, static_cast<std::uint16_t>(particle.phaseY), particle.scale * emitter.halfWidth,
                particle.scale * emitter.halfHeight, particle.colour);
        }
        workTransformMatrix = transform;
    }
    if (draw)
        EndSmokeDraw();
}

void QueueEmitterDraws(void *) {
    for (std::size_t index = 0; index < emitterSlots.size(); ++index) {
        if (emitterSlots[index].state == 0 || !IsWorldDrawingEnabled())
            continue;
        const ParticleEmitterReference &reference = emitterReferences[index];
        const bool renderMask = GetLegacyRenderMask() != 0;
        if (reference.active) {
            Matrix4x4_Push(renderMask ? &transformMatrix_prePass : nullptr);
            Matrix4x4::preTranslate(reference.position.x, reference.position.y, reference.position.z);
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(reference.rotation.z));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(reference.rotation.y));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(reference.rotation.x));
            Matrix4x4::preTranslate(reference.localPosition.x, reference.localPosition.y, reference.localPosition.z);
        } else if (renderMask) {
            Matrix4x4_Push(&transformMatrix_prePass);
        }
        SetUpdateCallback(DrawEmitter, reinterpret_cast<void *>(static_cast<std::uintptr_t>(index)), 900.0f);
        if (reference.active || renderMask)
            Matrix4x4::Pop(1);
    }
}

void EffectControlCallback(void *) {
    for (std::size_t index = 0; index < smokeSlots.size(); ++index) {
        EffectSlot &slot = smokeSlots[index];
        if (slot.state == 2) {
            if (effectsSuspended == 0 && slot.callback)
                slot.callback(slot.data);
            UpdateSmokeFft(*smokeFft[index]);
        }
    }
    for (EffectSlot &slot : fireSlots) {
        if (slot.state == 2) {
            if (effectsSuspended == 0 && slot.data)
                UpdateFireOwner(*static_cast<FireEffect *>(slot.data));
            if (slot.callback)
                HLib::ExecuteCallbackNow(slot.callback, slot.data);
        }
    }
    for (std::size_t index = 0; index < emitterSlots.size(); ++index) {
        EffectSlot &slot = emitterSlots[index];
        if (slot.state == 2) {
            if (slot.callback)
                slot.callback(slot.data);
            if (slot.textureSlot >= 0 && effectsSuspended == 0)
                UploadSmokeTexture(*static_cast<SmokeEffect *>(slot.data));
        }
    }
    HLib::ExecuteCallbackNow(QueueEmitterDraws, nullptr);
}
// @verified - p
void MoveDakuSprayParticle(ParticleRecord &particle) {

    particle.scale -= EffectDelta(0.05f);
    if (static_cast<double>(particle.scale) < 0.1) {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
    }
    const float alpha = static_cast<float>(particle.colour >> 24U) / 255.0f - EffectDelta(0.01f);
    if (alpha < 0.05f) {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
    }
    SetParticleAlpha(particle, alpha * 255.0f);
}

// @verified - p
void InitializeDakuSprayParticle0(ParticleRecord &particle) {
    particle.flags |= activeFlag;
    particle.callback = MoveDakuSprayParticle;
    particle.colour = g_dakuSprayColours[0];
    particle.scale = 1.0f;
}

// @verified - p
void InitializeDakuSprayParticle1(ParticleRecord &particle) {
    particle.flags |= activeFlag;
    particle.callback = MoveDakuSprayParticle;
    particle.colour = g_dakuSprayColours[1];
    particle.scale = 1.0f;
}

// @verified - p
void InitializeDakuSprayParticle2(ParticleRecord &particle) {
    particle.flags |= activeFlag;
    particle.callback = MoveDakuSprayParticle;
    particle.colour = g_dakuSprayColours[2];
    particle.scale = 1.0f;
}
void AssignStandardParticleEmitterBuffer(std::int32_t slot, std::int32_t count, ParticleRecord *particles) {
    ParticleEmitter &emitter = emitters[slot];
    emitter.count = count;
    emitter.particles = particles;
    emitter.flags &= ~0x20000000U;
    emitter.textureFlags = 0x80000002U;
    ParticleRecord initial{};
    initial.callback = BirthParticle;
    initial.colour = 0xffffffffU;
    initial.scale = 1.0f;
    for (std::int32_t index = 0; index < count; ++index)
        particles[index] = initial;
    emitter.samplerControl = 0x88800000U;
}
void MoveShipSprayParticle(ParticleRecord &particle) {
    IntegrateParticle(particle);
    particle.velocity.y -= EffectDelta(0.005f);
    particle.scale -= EffectDelta(0.04f);
    if (particle.scale < 0.1f) {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
    }
    const float alpha = static_cast<float>(particle.colour >> 24U) / 255.0f - EffectDelta(0.01f);
    if (alpha < 0.05f) {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
    }
    SetParticleAlpha(particle, alpha * 255.0f);
}

// @verified - p
void InitializeShipSprayParticle(ParticleRecord &particle) {
    const float opacity = g_hikiOpacity;
    if (opacity >= 0.0001f) {
        particle.flags |= activeFlag;
        particle.callback = MoveShipSprayParticle;
        particle.scale = 1.0f;
        particle.colour = (g_shipSprayColour & 0x00ffffffU) |
            (static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(g_shipSprayColour >> 24U) * opacity)) << 24U);
    } else {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
        particle.colour = 0;
    }
}

void DropParticleControlCallback(void *) {
    for (std::int32_t slot : dropEmitterSlots)
        AssignParticleEmitterReferences(slot, dropParticlePosition, dropParticleLocalPosition, 0);
    Vector3 root{};
    GetLightRootPosition(&root);

    for (std::size_t index = 0; index < dropLightIndices.size(); ++index) {
        const std::int32_t lightIndex = dropLightIndices[index];
        if (lightIndex < 0) {
            dropLightEnabled[index] = 0;
            continue;
        }
        const std::uint32_t slot = static_cast<std ::uint32_t>(lightIndex);
        dropLightEnabled[index] = static_cast<std::int32_t>(GetLightEntryIndex(slot));
        dropLightKinds[index] = static_cast<std::uint32_t>(GetLegacySelectedLightKind(slot));
        const LightRuntimeEntry *const light = GetLightRuntimeEntry(slot);
        if (light) {
            dropLightPositions[index] = light->vector20;
            if ((light->indexes[1] & 0xf7U) == 3) {
                dropLightPositions[index].x += root.x;
                dropLightPositions[index].y += root.y;
                dropLightPositions[index].z += root.z;
            }
            dropLightDirections[index] = {light->values2c[0], light->values2c[1], light->value34};
            dropLightRanges[index] = light->vectors[1].z;
            dropLightConeCosines[index] = cos_fixed16(static_cast<std::uint16_t>(light->unknown02));
        } else {
            dropLightRanges[index] = 0;
            dropLightConeCosines[index] = cos_fixed16(0);
        }
        dropLightPositions[index].x -= EffectDelta(dropParticlePosition.x);
        dropLightPositions[index].y -= EffectDelta(dropParticlePosition.y);
        dropLightPositions[index].z -= EffectDelta(dropParticlePosition.z);
    }
}

void MoveDropParticle(ParticleRecord &particle) {
    IntegrateParticle(particle);
    particle.velocity.y -= EffectDelta(0.010888889f);
    particle.colour = 0x70d0c0ffU;
    for (std::size_t index = 0; index < dropLightIndices.size(); ++index) {
        const std::uint32_t kind = dropLightKinds[index];
        if (dropLightEnabled[index] != 1 || kind - 3U > 1U)
            continue;
        const Vector3 &position = dropLightPositions[index];
        Vector3 direction{particle.position.x - position.x, particle.position.y - position.y, particle.position.z - position.z};
        const float distance = Vector3::NormalizeInPlace(direction);
        const Vector3 &lightDirection = dropLightDirections[index];
        const float dot = -((direction.y * lightDirection.y + direction.x * lightDirection.x) + direction.z * lightDirection.z);
        if (dropLightRanges[index] > distance && (kind == 3U || dot > dropLightConeCosines[index])) {
            particle.colour = 0x8090ffa0U;
            break;
        }
    }
    if (particle.position.y < 7.0f) {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
    }
}

void InitializeDropParticle(ParticleRecord &particle) {
    particle.flags = activeFlag;
    particle.callback = MoveDropParticle;
}
void MoveCompactDropParticle(CompactParticleRecord &particle) {
    particle.position.x = EffectDelta(particle.velocity.x) + particle.position.x;
    particle.position.y = EffectDelta(particle.velocity.y) + particle.position.y;
    particle.position.z = EffectDelta(particle.velocity.z) + particle.position.z;
    particle.velocity.y -= EffectDelta(0.010888889f);
    if (particle.position.y < 7.0f) {
        particle.flags &= ~activeFlag;
        particle.callback = BirthParticle;
    }
}

void InitializeCompactDropParticle(CompactParticleRecord &particle) {
    particle.flags = (particle.flags & 0x00ffffffU) | activeFlag;
    particle.callback = MoveCompactDropParticle;
}

} // namespace

void InitializeDropParticles() {
    if (dropEmitterSlots[0] >= 0)
        return;
    dropParticlePosition = {234.0f, 78.0f, 122.0f};
    dropParticleLocalPosition = {};
    dropLightIndices = {1, 2, -1, -1, -1};
    for (std::size_t index = 0; index < dropEmitterSlots.size(); ++index) {
        dropEmitterSlots[index] = AllocateEffectSlot(emitterSlots, -1, index == 0 ? DropParticleControlCallback : nullptr, nullptr, TASK_DROP);
        if (dropEmitterSlots[index] < 0)
            return;
    }
    std::uint32_t imageSize{};
    void *const image = LoadLegacySceneFile("MISC", "DROP.PVR", TASK_DROP, &imageSize);
    if (!image)
        return;
    for (std::size_t index = 0; index < dropEmitterSlots.size(); ++index) {
        const DropEmitterSettings &settings = dropEmitterSettings[index];
        ParticleEmitter &emitter = emitters[dropEmitterSlots[index]];
        emitter.flags |= activeFlag;
        emitter.count = settings.count;
        if (settings.compact) {
            emitter.flags |= 0x20000000U;
            CompactParticleRecord *const particles = index == 2 ? dropParticles2.data() : index == 3 ? dropParticles3.data() : dropParticles4.data();
            emitter.particles = reinterpret_cast<ParticleRecord *>(particles);
            emitter.textureFlags = 0xa0000002U;
            const CompactParticleRecord initial{.flags = 0x00ffffffU, .position = {}, .velocity = {}, .reserved1c = 0, .callback = BirthParticle}

            ;
            for (std::int32_t particle = 0; particle < settings.count; ++particle)
                particles[particle] = initial;
        } else {
            emitter.flags &= ~0x20000000U;
            emitter.particles = index == 0 ? dropParticles0.data() : dropParticles1.data();
            emitter.textureFlags = 0x80000002U;
            const ParticleRecord initial{.flags = 0,
                .position =
                    {

                    }

                // @verified - p
                ,
                .velocity = {},
                .reserved1c = 0,
                .callback = BirthParticle,
                .phaseX = 0,
                .phaseY = 0,
                .phaseZ = 0,
                .stepX = 0,
                .stepY = 0,
                .stepZ = 0,
                .colour = 0xffffffffU,
                .scale = 1.0f}

            // @verified - p
            ;
            for (std::int32_t particle = 0; particle < settings.count; ++particle)
                emitter.particles[particle] = initial;
        }
        emitter.samplerControl = 0x88800000U;
        SetEmitterSize(emitter, settings.width, settings.height);
        emitter.initializer = settings.compact ? reinterpret_cast<ParticleCallback>(InitializeCompactDropParticle) : InitializeDropParticle;
        SetEmitterBirthRate(emitter, settings.birthRate);
        emitter.position = settings.position;
        emitter.spawnVariance = settings.spawnVariance;
        emitter.velocity = settings.velocity;
        emitter.velocityVariance = settings.velocityVariance;
        emitter.colour = 0xff000000U;
        emitter.gpuFlags = (emitter.gpuFlags & 0x00ffffffU) | 0x94100000U;
        emitter.textureFlags |= 0x02000000U;
        emitter.flags |= 0x40000000U;
        emitterSlots[dropEmitterSlots[index]].state = 2;
        AssignParticleEmitterReferences(dropEmitterSlots[index], dropParticlePosition, dropParticleLocalPosition, 0);
    }
    HLib::FreeTaskData(image);
    }

void ReleaseDropParticles() {
    const std::int32_t first = dropEmitterSlots[0];
    if (emitterSlots[first].state == 2) {
        emitterSlots[first].state = 1;
        emitters[first].flags &= ~activeFlag;
    }
    for (std::int32_t &slot : dropEmitterSlots) {
        if (slot < 0)
            continue;
        emitters[slot].flags &= ~activeFlag;
        ClearEffectSlot(emitterSlots[slot]);
        slot = -1;
    }
}

void ResetDropParticleAdmission() {
    dropEmitterSlots[0] = -1;
}

void UpdateWeatherClipCameraPosition() {
    camCtrlGetNextCamPos(&weatherClipCameraPosition);
}

void ApplyWeatherRainAndRippleClip(const Vector2 *clip) {
    const std::array<Vector2, 4> points{{clip[0], clip[1], clip[2], clip[3]}};
    Weather_ApplyWCCPClipSet0ToRainGroups(points, clip[4].x, clip[4].y);
    Weather_ApplyWCCPClipSet0ToRippleGroups(points, clip[4].x, clip[4].y);
}

void ApplyWeatherParticleClip(const Vector2 *clip) {
    const std::array<Vector2, 4> points{{clip[0], clip[1], clip[2], clip[3]}};
    Weather_ApplyWCCPClipSet1ToParticleGroups(points, clip[4].x, clip[4].y);
}

void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex) {
    WeatherClipSet &set = weatherClipSets[static_cast<std::uint32_t>(setIndex)];
    set.enabled = 1;
    set.count = count;
    if (set.entries)
        HLib::FreeTaskData(set.entries);
    set.entries = static_cast<WeatherClipEntry *>(HLib ::AllocateStaticOrDynamicBlock(static_cast<std::uint64_t>(60LL * count), weatherClipEntryName));
    const std::byte *source = static_cast<const std::byte *>(entries);
    for (std::int32_t index = 0; index < count; ++index) {
        set.entries[index].enabled = 1;
        std::memcpy(reinterpret_cast<std::byte *>(&set.entries[index]) + offsetof(WeatherClipEntry, points), source, 56);
        source += 56;
    }
}

void Weather_ResetWCCPClipSet0() {
    const std::array<Vector2, 4> points{};
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        ApplyWeatherClipToRain(static_cast<WeatherFixcOwner *>(node->payload.pointer), 0, points, -0.1f, -99999.898f);
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        ApplyWeatherClipToRipple(static_cast<WeatherFixcOwner *>(node->payload.pointer), 0, points, -0.1f, -99999.898f);
}

void Weather_ResetWCCPClipSet1() {
    const std ::array<Vector2, 4> points{

    }

    ;
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next) {
        ApplyWeatherClipToRain(static_cast<WeatherFixcOwner *>(node->payload.pointer), 1, points, -0.1f, -99999.898f);
        ApplyWeatherClipToRipple(static_cast<WeatherFixcOwner *>(node->payload.pointer), 1, points, -0.1f, -99999.898f);
    }

    // @verified - p
}

void Weather_UpdateWCCPClipSet0() {
    const WeatherClipSet &set = weatherClipSets[0];
    if (!set.entries || set.enabled == 0)
        return;
    const WeatherClipEntry *const selected = SelectWeatherClipEntry(set);
    if (selected) {
        Weather_ApplyWCCPClipSet0ToRainGroups(selected->points, selected->clipY0, selected->clipY1);
        Weather_ApplyWCCPClipSet0ToRippleGroups(selected->points, selected->clipY0, selected->clipY1);
    } else {
        Weather_ResetWCCPClipSet0();
    }
}
void Weather_UpdateWCCPClipSet1() {
    const WeatherClipSet &set = weatherClipSets[1];
    if (!set.entries || set.enabled == 0)
        return;
    const WeatherClipEntry *const selected = SelectWeatherClipEntry(set);
    if (selected)
        Weather_ApplyWCCPClipSet1ToParticleGroups(selected->points, selected->clipY0, selected->clipY1);
    else
        Weather_ResetWCCPClipSet1();
}

void UpdateWeatherParticleGroundCameraPosition() {
    camCtrlGetNextCamPos(&weatherParticleGroundCameraPosition);
}

LegacyEffectTexture *ReadWeatherRipplePvrTexture(std::span<const std::byte> image) {
    std::uint32_t magic{};
    std::memcpy(&magic, image.data(), sizeof(magic));
    const std::size_t header = magic == SHEN_NAME(GBIX) ? 12U : 0U;
    std::uint16_t format{};
    std::uint16_t width{};
    std::uint16_t height{};
    std::memcpy(&format, image.data() + header + 8, sizeof(format));
    std::memcpy(&width, image.data() + header + 12, sizeof(width));
    std::memcpy(&height, image.data() + header + 14, sizeof(height));
    return CreateEffectTextureReference(width, height, format, image, 1);
}

HLib::Task *WAR_CallbackParticleResource_Enqueue(float rainSpawnLimit, float secondaryRainParam, float rippleSpawnLimit, float unused) {
    weatherSecondaryRainParameter = secondaryRainParam;
    weatherWarState = 0;
    g_currentWeatherForceFlag = 0;
    weatherGroupList = nullptr;
    weatherRainSpawnLimit = rainSpawnLimit;
    weatherRippleSpawnLimit = rippleSpawnLimit;
    weatherParticleUnusedParameter = unused;
    weatherParticleResourceTask =
        HLib::EnqueueTaskWithoutParameter(WAR_CallbackParticleResource_Callback, 14, 8, static_cast<std::int32_t>(weatherResourceTaskName.name));
    return weatherParticleResourceTask;
}

void WAR_CallbackParticleResource_Callback(void *) {
    if (weatherWarState != 0)
        (void)HLib::ExecuteCallbackNow(WeatherParticle_Callback, weatherGroupList);
}

void EnsureWeatherFixcRegistered() {
    if (weatherRegisteredFixc != 0)
        return;
    if (!weatherFixcData)
        weatherFixcData = AllocateWeatherFixc(14563U);
    Weather_RegisterActiveFIXCAndInitEffects();
}
void Weather_RegisterActiveFIXCAndInitEffects() {
    ModulateWeatherFixc(weatherFixcData);
    weatherRegisteredFixc = 1;

    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        EnsureWeatherRainForFixc(static_cast<WeatherFixcOwner *>(node->payload.pointer), 1.5f, -1.0f, -1.0f);
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        EnsureWeatherRippleForFixc(static_cast<WeatherFixcOwner *>(node->payload.pointer), 0.95f);
}

HLib ::Task *AllocateICAR(float rainSpawnLimit, float secondaryRainParam, float rippleSpawnLimit, float unused) {
    WeatherInteractionCamera *const camera =
        static_cast<WeatherInteractionCamera *>(HLib::AllocateTaskData(sizeof(WeatherInteractionCamera), weatherInteractionCameraName));
    camCtrlGetNextCamPos(&camera->position);
    camCtrlGetNextCamPos(&camera->nextCameraPosition);
    (void)camCtrlGetTargetPosition(&camera->currentCameraPosition);
    (void)camCtrlGetTargetPosition(&camera->currentCameraPosition2);
    camera->previousBackwardDirection = {};
    camera->backwardDirection = {};
    weatherInteractionCamera = camera;
    return WAR_CallbackParticleResource_Enqueue(rainSpawnLimit, secondaryRainParam, rippleSpawnLimit, unused);
}

void InitializeWeatherParticleResources() {
    weatherRainColour = std::bit_cast<std::uint32_t>(weatherRainPressureParam0);
    weatherRippleColour = weatherDefaultRippleColour;
    weatherRegisteredPressureParam1 = weatherRainPressureParam1;
    weatherRegisteredPressureParam2 = weatherRainPressureParam2;
    weatherRegisteredSpawnDrift = {weatherRainSpawnDriftX, weatherRainFallSpeedWet, weatherRainSpawnDriftZ};
    weatherRegisteredDriftVariance = {weatherRainDriftVarianceX, weatherRainDriftVarianceY, weatherRainDriftVarianceZ};
    weatherFixcList = nullptr;
    weatherGroupList = nullptr;
    weatherFixcData = AllocateWeatherFixc(14563U);
    Weather_RegisterActiveFIXCAndInitEffects();
    if (weatherFixcData && weatherRegisteredFixc != 0) {
        UnregisterWeatherFixc(weatherFixcData);
        weatherRegisteredFixc = 0;
    }
}

void EV_WeatherEffectUtil_Impl(std::uint32_t frustumFov, float spawnScale, std::int64_t, float rippleScale, float minY, float maxY) {
    if (weatherFixcData) {
        UnregisterWeatherFixc(weatherFixcData);
        FreeWeatherFixc(weatherFixcData);
        weatherFixcData = nullptr;
        weatherRegisteredFixc = 0;
    }
    weatherFixcData = AllocateWeatherFixc(frustumFov);
    ModulateWeatherFixc(weatherFixcData);
    weatherRegisteredFixc = 1;
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        EnsureWeatherRainForFixc(static_cast<WeatherFixcOwner *>(node->payload.pointer), spawnScale, minY, maxY);
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        EnsureWeatherRippleForFixc(static_cast<WeatherFixcOwner *>(node->payload.pointer), rippleScale);
}

std::int64_t Weather_FreeActiveFIXC() {
    if (weatherFixcData) {
        UnregisterWeatherFixc(weatherFixcData);
        FreeWeatherFixc(weatherFixcData);
        weatherFixcData = nullptr;
        weatherRegisteredFixc = 0;
    }
    return 0;
}
std::int32_t SetWeatherRainPressureImmediate(float pressure) {
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next) {
        WeatherFixcOwner *const fixc = static_cast<WeatherFixcOwner *>(node->payload.pointer);
        if (fixc->mode < 2U) {
            WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
            SetRainGroupCountImmediate(cara->rain0, pressure);
            SetRainGroupCountImmediate(cara->rain1, pressure);
        } else if (fixc->mode == 6U) {
            SetRainGroupCountImmediate(static_cast<WeatherParticleGroup *>(fixc->cara), pressure);
        }
    }
    return lfltSetImmediate(GetWeatherPressureLflt(), pressure);
}

void BeginRainPressureTransitionForWeatherEffects(float rainValue, std::int32_t frames) {
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        BeginRainTransitionForFixc(static_cast<WeatherFixcOwner *>(node->payload.pointer), rainValue, frames);
}

void UpdateWeatherInteractionCamera() {
    AdvanceWeatherInteractionCamera();
}

void UpdateWeatherParticleGroups() {
    AdvanceWeatherFloat(GetWeatherPressureLflt());
    AdvanceWeatherFloat(GetWeatherRippleLflt());
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next)
        UpdateWeatherFixc(static_cast<WeatherFixcOwner *>(node->payload.pointer));
    (void)GetWeatherPressure();
    PublishLegacyWeatherSpawnDrift(weatherRegisteredSpawnDrift);
}

void SetWeatherParticleRainColour(std::uint32_t colour) {
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next) {
        WeatherFixcOwner *const fixc = static_cast<WeatherFixcOwner *>(node->payload.pointer);
        const auto setColour = [colour](WeatherParticleGroup *group) {
            if (group)
                group->rainPressureParam0 = std::bit_cast<float>(colour);
        };
        if (fixc->mode < 2U) {
            WeatherRainOwner *const cara = static_cast<WeatherRainOwner *>(fixc->cara);
            setColour(cara->rain0);
            setColour(cara->rain1);
        } else if (fixc->mode == 6U) {
            setColour(static_cast<WeatherParticleGroup *>(fixc->cara));
        }
    }
    weatherRainColour = colour;
}

void SetWeatherParticleRippleColour(std::uint32_t colour) {
    for (WeatherFixcNode *node = weatherFixcList; node; node = node->next) {
        WeatherFixcOwner *const fixc = static_cast<WeatherFixcOwner *>(node->payload.pointer);
        if (fixc->mode < 2U) {
            AllocateWeatherRippleColourRamp(static_cast<WeatherRainOwner *>(fixc->cara)->rippleGroup, colour);
        } else if (fixc->mode == 6U) {
            AllocateWeatherRippleColourRamp(static_cast<WeatherParticleGroup *>(fixc->cara), colour);
        }
    }
    weatherRippleColour = colour;
}

void SetWeatherParticleDrawingEnabled(std::int32_t enabled) {
    for (WeatherParticleGroup *group = weatherGroupList; group; group = group->next) {
        if (enabled != 0) {
            group->rainFlags |= activeFlag;
            group->rippleFlags |= activeFlag;
        } else {
            group->rainFlags &= ~activeFlag;
            group->rippleFlags &= ~activeFlag;
        }
    }
}

// @verified - p
void AssignParticleEmitterReferences(std::int32_t emitter, const Vector3 &position, const Vector3 &localPosition, std::int32_t yaw) {
    ParticleEmitterReference &reference = emitterReferences[static_cast<std::size_t>(emitter)];
    reference.position = position;
    reference.localPosition = localPosition;
    reference.rotation = {.x = 0, .y = yaw, .z = 0};
    reference.active = 1;
}

// @verified - p
void AssignShipSprayPosition(const Vector3 &position) {
    g_shipSprayPosition = position;
}

// @verified - p
std::uint64_t AssignShipSprayLocalPosition(const Vector3 &position) {
    g_shipSprayLocalPosition = position;
    return std::bit_cast<std::uint32_t>(position.z);
}

// @verified - p
void AssignShipSprayYaw(std::int32_t yaw) {
    g_shipSprayYaw = yaw;
}

// @verified - p
void ResetShipSprayParameters() {
    if ((g_currentMap.name & 0xffU) == 'K') {
        g_shipSprayParameters = shipSprayK.data();
    } else {
        g_shipSprayParameters = ((g_currentMap.name >> 8U) & 0xffU) == '5' ? shipSprayFive.data() : shipSprayDefault.data();
    }
    g_shipSprayParameters[0].slot = -1;
}

// @verified - p
void ShipSprayControlCallback(void *) {
    if (!(g_hikiOpacity >= 0.0001f))
        return;
    if (!(g_shipSprayFadeRate <= 0.0001f)) {
        g_hikiOpacity -= EffectDelta(g_shipSprayFadeRate);
        if (!(g_hikiOpacity >= 0.0f)) {
            g_hikiOpacity = 0.0f;
            for (std::size_t index = 0; index < 6; ++index) {
                const std::int32_t slot = g_shipSprayParameters[index].slot;
                if (emitterSlots[slot].state == 2) {
                    emitterSlots[slot].state = 1;
                    emitters[slot].flags &= ~activeFlag;
                }
            }
            return;
        }
    }
    Vector3 positions[6];
    for (std::size_t index = 0; index < 6; ++index)
        positions[index] = {.x = g_shipSprayParameters[index].position.x, .y = 0, .z = g_shipSprayParameters[index].position.z};
    for (std::size_t index = 0; index < 2; ++index) {
        ShipSprayEmitter &parameters = g_shipSprayParameters[index];
        if (parameters.position.y < 0.0f)
            parameters.widthOffset -= EffectDelta(0.05f);
        else
            parameters.widthOffset += EffectDelta(0.05f);
        if (!(parameters.widthOffset <= 1.3f))
            parameters.widthOffset = 1.3f;
        else if (parameters.widthOffset < -0.6f)
            parameters.widthOffset = -0.6f;
    }
    for (std::size_t index = 0; index < 6; ++index) {
        const ShipSprayEmitter &parameters = g_shipSprayParameters[index];
        ParticleEmitter &emitter = emitters[parameters.slot];
        SetEmitterSize(emitter, parameters.widthOffset + parameters.width, parameters.height);
        emitter.position = positions[index];
        emitter.spawnVariance = parameters.spawnVariance;
        AssignParticleEmitterReferences(parameters.slot, g_shipSprayPosition, g_shipSprayLocalPosition, g_shipSprayYaw);
    }
}

void InitializeShipSpray() {
    if (g_shipSprayParameters[0].slot >= 0)
        return;
    g_shipSprayLocalPosition = {.x = 0, .y = 0, .z = 24.0f};
    g_hikiOpacity = 1.0f;
    g_shipSprayPosition = {};
    g_shipSprayYaw = 0;
    g_shipSprayFadeRate = 0.0f;
    if ((g_currentMap.name & 0xffU) == 'K')
        g_shipSprayColour = kShipSprayColourK;
    else
        g_shipSprayColour = ((g_currentMap.name >> 8U) & 0xffU) == '5' ? kShipSprayColourFive : kShipSprayColourDefault;
    void *image = nullptr;
    std::uint32_t size = 0;
    if (!(image = LoadLegacySceneFile("MISC", "SHIBU.PVR", TASK_SHIP, &size)))
        return;
    const std::uint32_t *const pvr = static_cast<const std::uint32_t *>(image);
    const std::uint32_t width = pvr[6] & 0xffffU;
    const std::uint32_t height = pvr[6] >> 16U;
    for (std::size_t index = 0; index < 6; ++index) {
        ShipSprayEmitter &parameters = g_shipSprayParameters[index];
        parameters.widthOffset = 0.0f;
        parameters.slot = AllocateEffectSlot(emitterSlots, -1, index ? nullptr : ShipSprayControlCallback, nullptr, TASK_SHIP);
        if (parameters.slot < 0)
            continue;
        parameters.particles = HLib::AllocateTaskData(96ULL * static_cast<std::uint32_t>(parameters.count), TASK_SHIP);
        if (!parameters.particles)
            continue;
        ParticleEmitter &emitter = emitters[parameters.slot];
        emitter.count = parameters.count;
        emitter.particles = static_cast<ParticleRecord *>(parameters.particles);
        emitter.flags &= ~0x20000000U;
        emitter.textureFlags = 0x80000002U;
        emitter.samplerControl = 0x88800000U;
        const ParticleRecord initial{.flags = 0,
            .position = {},
            .velocity = {},
            .reserved1c = 0,
            .callback = BirthParticle,
            .phaseX = 0,
            .phaseY = 0,
            .phaseZ = 0,
            .stepX = 0,
            .stepY = 0,
            .stepZ = 0,
            .colour = 0xffffffffU,
            .scale = 1.0f}

        // @verified - p
        ;
        for (std::int32_t particle = 0; particle < parameters.count; ++particle)
            emitter.particles[particle] = initial;
        emitter.position = parameters.position;
        emitter.spawnVariance = parameters.spawnVariance;
        emitter.initializer = InitializeShipSprayParticle;
        emitter.velocity = parameters.velocity;
        emitter.velocityVariance = parameters.velocityVariance;
        SetEmitterBirthRate(emitter, parameters.birthRate);
        SetEmitterSize(emitter, parameters.width, parameters.height);
        if (emitter.textureHandle)
            d3t::InitTexture2DResource(emitter.textureHandle);
        LegacyEffectTexture &texture = smokeTextureRecords[parameters.slot];
        emitter.textureRecord = &texture;

        if (InitializeLegacyEffectTexture(texture, width, height, static_cast<std::uint16_t>(pvr[5])) == 0 &&
            UploadLegacyEffectPvr(texture, {static_cast<const std::byte *>(image), size}) == 0) {
            emitter.textureHandle = static_cast<std::uint32_t>(texture.handle);
            std::uint32_t widthBits = 0;
            std::uint32_t heightBits = 0;
            for (std::uint32_t value = width; value; value >>= 1U)
                ++widthBits;
            for (std::uint32_t value = height; value; value >>= 1U)
                ++heightBits;
            emitter.textureFlags = 0x8000000aU;
            emitter.samplerControl = 0x8a800000U;
            emitter.gpuFlags = ((8U * widthBits - 32U) | (heightBits - 4U)) & 0x6bedbd3fU;
            emitter.gpuFlags = ((emitter.gpuFlags | 0x940242c0U) & 0x00ffffffU) | 0x20000000U;
            emitter.flags |= 0x10000000U;
            emitter.textureSource = static_cast<const std::byte *>(image) + 28;
        }
        emitter.gpuFlags = (emitter.gpuFlags & 0x00ffffffU) | 0x94100000U;
        emitter.textureFlags |= 0x02000000U;
        emitter.flags |= 0xc0000000U;
        emitter.gpuFlags = (emitter.gpuFlags & ~0x4000U) | 0x2000U;
        emitter.colour = g_shipSprayColour;
        if (emitterSlots[parameters.slot].state == 1)
            emitterSlots[parameters.slot].state = 2;
    }
    HLib::FreeTaskData(image);
}

// @verified - p
void ResetDakuSpray() {
    g_dakuSpraySlots[0] = -1;
}
// @verified - p
void DakuSprayControlCallback(void *) {
    const Vector3 local{};
    for (std::uint32_t index = 0; index < 3; ++index)
        AssignParticleEmitterReferences(g_dakuSpraySlots[index], g_dakuSprayPositions[index], local, 0);
}

// @verified - p
void SetDakuSprayColour(std::uint32_t emitter, std::uint32_t colour) {
    if (emitter <= 2)
        g_dakuSprayColours[emitter] = colour;
}

// @verified - p
void SetDakuSprayPosition(std::uint32_t emitter, const Vector3 &position) {
    if (emitter <= 2)
        g_dakuSprayPositions[emitter] = position;
}

void SetDakuSpraySize(std::uint32_t emitter, float width, float height) {
    if (emitter <= 2) {
        g_dakuSpraySizes[emitter] = {.x = width, .y = height};
        const std::int32_t slot = g_dakuSpraySlots[emitter];
        if (slot >= 0)
            SetEmitterSize(emitters[slot], width, height);
    }
}

// @verified - p
void InitializeDakuSpray() {
    if (g_dakuSpraySlots[0] >= 0)
        return;
    std::uint32_t size = 0;
    void *const image = LoadLegacySceneFile("MISC", "SHIBU.PVR", TASK_DAKU, &size);
    if (!image)
        return;
    const std::uint32_t *const pvr = static_cast<const std::uint32_t *>(image);
    const std::uint32_t width = pvr[6] & 0xffffU;
    const std::uint32_t height = pvr[6] >> 16U;
    for (std::uint32_t index = 0; index < 3; ++index) {
        g_dakuSprayPositions[index] = {};
        const std::int32_t slot = AllocateEffectSlot(emitterSlots, -1, index == 0 ? DakuSprayControlCallback : nullptr, nullptr, TASK_DAKU);
        g_dakuSpraySlots[index] = slot;
        if (slot < 0)
            continue;
        ParticleRecord *const particles = static_cast<ParticleRecord *>(HLib::AllocateTaskData(0x3c0, TASK_DAKU));
        if (!particles)
            continue;
        AssignStandardParticleEmitterBuffer(slot, 10, particles);
        ParticleEmitter &emitter = emitters[slot];
        emitter.position = {};
        emitter.spawnVariance = {};
        emitter.initializer = index == 0 ? InitializeDakuSprayParticle0 : index == 1 ? InitializeDakuSprayParticle1 : InitializeDakuSprayParticle2;
        emitter.velocity = {};
        emitter.velocityVariance = {};
        SetEmitterBirthRate(emitter, 0.6f);
        g_dakuSpraySizes[index] = {.x = 1.1f, .y = 0.86f};
        SetEmitterSize(emitter, 1.1f, 0.86f);
        CreateAuthoredParticleTexture(slot, width, height, static_cast<std::uint16_t>(pvr[5]), static_cast<const std::byte *>(image), size);
        EnableEmitter(emitter);

        emitter.gpuFlags = (emitter.gpuFlags & ~0x4000U) | 0x2000U;
        emitter.flags |= activeFlag;
        g_dakuSprayColours[index] = DAKU_SPRAY_DEFAULT_COLOUR;
        emitter.colour = DAKU_SPRAY_DEFAULT_COLOUR;
        if (emitterSlots[slot].state == 1) {
            emitterSlots[slot].state = 2;
            emitter.flags |= activeFlag;
        }
    }
    HLib::FreeTaskData(image);
}

void InitializeEffectControl() {
    effectController = nullptr;
    effectTaskReferences = 0;
    effectsSuspended = 0;
    smokeFft.fill(nullptr);
    fireFft.fill(nullptr);
    for (ParticleEmitter &emitter : emitters) {
        emitter = {};
        emitter.samplerControl = 0x8a000000U;
        emitter.gpuFlags = 0x208000c0U;
        emitter.halfWidth = 1.0f;
        emitter.halfHeight = 1.0f;
        emitter.colour = 0xffffffffU;
    }
    ResetHikiTextures();
    ResetShipSprayParameters();
    ResetHikiTaskData();
    ResetDakuSpray();
    ResetDropParticleAdmission();
    for (ParticleEmitterReference &reference : emitterReferences)
        reference.active = 0;
    for (EffectSlot &slot : emitterSlots)
        ClearEffectSlot(slot);
    for (EffectSlot &slot : smokeSlots)
        ClearEffectSlot(slot);
    for (EffectSlot &slot : fireSlots)
        ClearEffectSlot(slot);
}

void EnqueueEffectControlTask() {
    if (!effectController) {
        effectController = HLib::EnqueueTaskWithoutParameter(EffectControlCallback, 13, 8, static_cast<std::int32_t>(controllerToken.name));
    }
    ++effectTaskReferences;
}

void InitializeEffectPoints() {
    smokeEffects.fill(nullptr);
    splashEffects.fill(nullptr);
    reservedParticleEffects.fill(nullptr);
    fallingEffects.fill(nullptr);
    smokeTextures.fill(nullptr);
    fireEffects.fill(nullptr);
    smokeTextureHandles.fill(0);
    for (auto &handles : fireTextureHandles)
        handles.fill(0);
}

void CacheCurrentParticleEffects(std::int32_t suspended) {
    if (suspended == 1) {
        for (ParticleEmitter &emitter : emitters)
            emitter.flags |= suspendedFlag;
        effectsSuspended = 1;
    } else if (suspended == 0) {
        for (ParticleEmitter &emitter : emitters)
            emitter.flags &= ~suspendedFlag;
        effectsSuspended = 0;
    }
}

std::int32_t g_currentWeatherForceFlag = 0;

void SetRainAndRippleFlagsTo(std::int32_t paused) {
    g_currentWeatherForceFlag = paused != 0;
    for (WeatherParticleGroup *group = weatherGroupList; group; group = group->next) {
        if (paused != 0) {
            group->rainFlags |= weatherParticlePausedFlag;
            group->rippleFlags |= weatherParticlePausedFlag;
        } else {
            group->rainFlags &= ~weatherParticlePausedFlag;
            group->rippleFlags &= ~weatherParticlePausedFlag;
        }
    }
}

std::uint64_t CreateSplashEffect(const Vector3 &position, const Vector3 &spawnVolume, std::int32_t count, float birthRate, shen_name flags,
    std::int32_t floorHeightBits, const char *textureArchive, const char *textureName) {
    std::uint32_t index = 0;
    while (index < splashEffects.size() && splashEffects[index])
        ++index;
    if (index == splashEffects.size())
        return invalidEffect;
    EnqueueEffectControlTask();
    auto *const effect = static_cast<SmokeEffect *>(HLib::AllocateTaskData(sizeof(SmokeEffect), effectToken));
    splashEffects[index] = effect;
    effect->slot = AllocateEffectSlot(emitterSlots, -1, SplashControlCallback, effect, effectToken);
    effect->count = count;
    effect->particles = static_cast<ParticleRecord *>(HLib::AllocateTaskData(96ULL * static_cast<std::uint64_t>(count), effectToken));
    effect->position = position;
    effect->spawnVariance = spawnVolume;
    effect->velocity = {0.001f, 0.1f, 0.001f};
    effect->velocityVariance = {0.001f, 0.001f, 0.001f};
    effect->floorHeight = std::bit_cast<float>(floorHeightBits);
    effect->flags = flags;
    effect->birthRate = birthRate;
    effect->width = effect->height = 0.02f;
    effect->texture = nullptr;
    effect->fadeFactor = 1.0f;
    effect->expansion = 0.0f;
    effect->initialScale = 1.0f;
    effect->profile = 0;
    const std::uint32_t profile = (flags.name >> 8U) & 0xfU;
    ParticleCallback initializer = emitters[effect->slot].initializer;
    switch (profile) {
    case 0:
        initializer = InitializeSplashParticle;
        break;
    case 1:
        initializer = InitializeShrinkingSplashParticle;
        break;
    case 2:
        initializer = InitializeRadialSplashParticle;
        break;
    case 3:
        initializer = InitializeWideSplashParticle;
        break;
    }
    RegisterEmitter(*effect, initializer);
    EnableEmitter(emitters[effect->slot]);
    if (textureArchive && textureName) {
        std::uint32_t size = 0;
        void *image = LoadLegacySceneFile(textureArchive, textureName, effectToken, &size);
        CreateAuthoredParticleTexture(effect->slot, 32, 32, 0x8083U, static_cast<const std::byte *>(image), size);
        HLib::FreeTaskData(image);
    } else {
        auto *const image = static_cast<std::byte *>(HLib::AllocateTaskData(0x480, effectToken));
        WriteSmokeDdsHeader(image);
        auto *const pixels = reinterpret_cast<std::uint32_t *>(image + 0x80);
        std::fill_n(pixels, 256, 0x6fffffffU);
        CreateAuthoredParticleTexture(effect->slot, 16, 16, 0x8083U, image, 0x480, true);
        HLib::FreeTaskData(image);
    }
    SuspendParticleEffect(1, index);
    for (std::int32_t particle = 0; particle < count; ++particle)
        effect->particles[particle].stepX = reinterpret_cast<std::uintptr_t>(&effect->floorHeight);
    return index;
}

std::uint64_t CreateFallingParticleEffect(const Vector3 &position, const Vector3 &spawnVolume, std::int32_t count, float birthRate, shen_name flags,
    const char *textureArchive, const char *textureName, float floorHeight) {
    std::uint32_t index = 0;
    while (index < fallingEffects.size() && fallingEffects[index])
        ++index;
    if (index == fallingEffects.size())
        return invalidEffect;
    EnqueueEffectControlTask();
    auto *const effect = static_cast<SmokeEffect *>(HLib::AllocateTaskData(sizeof(SmokeEffect), effectToken));
    fallingEffects[index] = effect;
    effect->slot = AllocateEffectSlot(emitterSlots, -1, nullptr, nullptr, effectToken);
    effect->count = count;
    effect->particles = static_cast<ParticleRecord *>(HLib::AllocateTaskData(sizeof(ParticleRecord) * static_cast<std::uint64_t>(count), effectToken));
    for (std::int32_t particle = 0; particle < count; ++particle)
        effect->particles[particle].stepX = index;
    effect->position = position;
    effect->spawnVariance = spawnVolume;
    effect->velocity = {0.0060000001f, -0.015f, 0.0060000001f};
    effect->velocityVariance = {0.012f, 0.001f, 0.012f};
    effect->flags = flags;
    effect->birthRate = birthRate;
    effect->width = 0.039999999f;
    effect->height = 0.079999998f;
    effect->texture = nullptr;
    effect->fadeFactor = 0.0f;
    effect->expansion = 0.0f;
    effect->initialScale = 1.0f;
    effect->profile = 0;
    const std::uint32_t profile = (flags.name >> 8U) & 0xffU;
    ParticleCallback initializer = emitters[effect->slot].initializer;
    switch (profile) {
    case 0:
        initializer = InitializeFallingParticle;
        break;
    case 1:
        initializer = InitializeDriftingParticle;
        fallingParticleOrigins[0] = position;
        break;
    case 2:
        initializer = InitializeCurvedParticle;
        fallingParticleOrigins[1] = position;
        break;
    case 3:
        initializer = InitializeVortexParticle;
        fallingParticleOrigins[2] = position;
        break;
    case 4:
        initializer = InitializeLitFallingParticle;
        break;
    }
    RegisterEmitter(*effect, initializer);
    EnableEmitter(emitters[effect->slot]);
    std::uint32_t size = 0;
    void *image = LoadLegacySceneFile(textureArchive, textureName, effectToken, &size);
    if (image)
        CreateAuthoredParticleTexture(effect->slot, 64, 128, 0x8001U, static_cast<const std::byte *>(image), size);
    HLib::FreeTaskData(image);
    SuspendParticleEffect(3, index);
    fallingParticleFloor = floorHeight;
    return index;
}

// @verified - p
void SetFallingParticleFloor(float height) {
    fallingParticleFloor = height;
}

// @verified - p
std::uint64_t CreateSmokeEffect(
    const Vector3 &position, const Vector3 &spawnVolume, std::int32_t particleCount, float birthRate, shen_name flags, std::int32_t profile) {
    std::uint32_t index = 0;
    while (index < smokeEffects.size() && smokeEffects[index])
        ++index;
    if (index == smokeEffects.size())
        return invalidEffect;
    auto *const effect = static_cast<SmokeEffect *>(HLib::AllocateTaskData(sizeof(SmokeEffect), effectToken));
    smokeEffects[index] = effect;
    effect->position = position;
    effect->spawnVariance = spawnVolume;
    effect->velocity = {0.0f, 0.0080000004f, 0.0f};
    effect->velocityVariance = {0.013f, 0.0040000002f, 0.013f};
    effect->birthRate = birthRate;
    effect->width = 3.0f;
    effect->height = 3.0f;
    effect->texture = nullptr;
    effect->flags = flags;
    effect->count = particleCount;
    effect->profile = profile;
    HLib::TaskCallback control{};
    ParticleCallback initializer{};
    float falloffGain = 128.0f;
    switch (profile) {
    case 0:
        control = SmokeControlCallback;
        initializer = InitializeSmoke;
        break;
    case 1:
        control = StationarySmokeControlCallback;
        initializer = InitializeStationarySmoke;
        stationarySmokeAlpha = 32.0f;
        falloffGain = 54.0f;
        effect->velocity = {};
        effect->velocityVariance = {};
        effect->width = 8.0f;
        effect->height = 1.0f;
        break;
    case 2:
        control = RadialSmokeControlCallback;
        initializer = InitializeRadialSmoke;
        break;
    case 3:
        control = SpreadingSmokeControlCallback;
        initializer = InitializeSpreadingSmoke;
        smokeOpacityScale[3] = 1.0f;
        smokeRadialVelocity[3] = 0.0f;
        smokeRadialDecay[3] = 0.0f;
        smokeAlphaStep[3] = 3;
        break;
    case 4:
        control = WideSmokeControlCallback;
        initializer = InitializeWideSmoke;
        smokeOpacityScale[4] = 1.0f;
        smokeRadialVelocity[4] = 0.0f;
        smokeRadialDecay[4] = 0.0f;
        smokeAlphaStep[4] = 3;
        break;
    }
    if (profile >= 0 && profile <= 4) {
        if (profile != 1) {
            smokeExpansion[profile] = 0.0060392162f;
            smokeFadeFactor[profile] = 1.2019608f;
            smokeInitialScale[profile] = 0.1f;
        }
        effect->fadeFactor = profile == 1 ? 0.91176468f : 1.2019608f;
        effect->expansion = 0.0060392162f;
        effect->initialScale = 0.1f;
    }
    const std::uint32_t textureIndex = (flags.name >> 4U) & 0xfU;
    effect->texture = smokeTextures[textureIndex];
    if (!effect->texture) {
        auto *const texture = static_cast<SmokeTextureOwner *>(HLib::AllocateTaskData(sizeof(SmokeTextureOwner), effectToken));
        effect->texture = texture;
        smokeTextures[textureIndex] = texture;
        texture->slot = AllocateEffectSlot(smokeSlots, -1, nullptr, nullptr, effectToken);
        auto *const fft = static_cast<SmokeFftData *>(HLib::AllocateTaskData(sizeof(SmokeFftData), SHEN_NAME(PUFY)));
        smokeFft[texture->slot] = fft;
        std::memset(fft, 0, sizeof(*fft));
        InitializeSmokeFft(*fft);
        texture->colour = 4095;
        SetSmokeFftColour(*fft, static_cast<std::uint16_t>(texture->colour));
        texture->pixels = static_cast<std::byte *>(HLib::AllocateTaskData(0x49f, effectToken));
        std::memset(texture->pixels, 0xff, 0x49f);
        SetSmokeFftOutput(*fft, texture->pixels);
        std::array<float, 64> falloff;
        GenerateSmokeFalloff(3.019608f, falloff);
        ScaleSmokeFalloff((falloffGain * 100.0f) / 255.0f, falloff);
        SetSmokeFftFalloff(*fft, falloff);
        SetSmokeFftAmplitude(*fft, 0.5f);
        texture->references = 0;
        texture->activeEmitters = 0;
    }
    ++effect->texture->references;
    effect->slot = AllocateEffectSlot(emitterSlots, effect->texture->slot, control, effect, effectToken);
    effect->particles = static_cast<ParticleRecord *>(HLib::AllocateTaskData(sizeof(ParticleRecord) * static_cast<std::uint64_t>(effect->count), effectToken));
    RegisterEmitter(*effect, initializer);
    CreateSmokeTexture(*effect);
    emitters[effect->slot].gpuFlags = (emitters[effect->slot].gpuFlags & ~0x4000U) | 0x2000U;
    EnableEmitter(emitters[effect->slot]);
    SuspendParticleEffect(0, index);
    EnqueueEffectControlTask();
    return index;
}
// @verified - p
std::uint32_t ConfigureSmokeEffect(std::int32_t profile, std::uint32_t index, const Vector3 &position, const Vector3 &spawnVolume, std::int32_t particleCount,
    float birthRate, shen_name flags) {
    SmokeEffect &effect = *smokeEffects[index];
    effect.position = position;
    effect.spawnVariance = spawnVolume;
    effect.velocity = {0.0f, 0.0080000004f, 0.0f};
    effect.velocityVariance = {0.013f, 0.0040000002f, 0.013f};
    effect.flags = flags;
    effect.count = particleCount;
    effect.birthRate = birthRate;
    effect.width = 3.0f;
    effect.height = 3.0f;
    effect.profile = profile;
    ParticleCallback initializer{};
    switch (profile) {
    case 0:
        initializer = BirthParticle;
        break;
    case 1:
        initializer = InitializeStationarySmoke;
        stationarySmokeAlpha = 32.0f;
        effect.velocity = {};
        effect.velocityVariance = {};
        effect.fadeFactor = 0.91176468f;
        effect.width = 8.0f;
        effect.height = 1.0f;
        break;
    case 2:
        initializer = InitializeRadialSmoke;
        break;
    case 3:
        initializer = InitializeSpreadingSmoke;
        break;
    case 4:
        initializer = InitializeWideSmoke;
        break;
    }
    if (profile >= 0 && profile <= 4) {
        if (profile != 1) {
            smokeExpansion[profile] = 0.0060392162f;
            smokeFadeFactor[profile] = 1.2019608f;
            smokeInitialScale[profile] = 0.1f;
            effect.fadeFactor = 1.2019608f;
        }
        effect.expansion = 0.0060392162f;
        effect.initialScale = 0.1f;
        if (profile >= 3) {
            smokeOpacityScale[profile] = 1.0f;
            smokeRadialVelocity[profile] = 0.0f;
            smokeRadialDecay[profile] = 0.0f;
            smokeAlphaStep[profile] = 3;
        }
    }
    if (effect.texture) {
        std::array<float, 64> falloff;
        GenerateSmokeFalloff(2.8f, falloff);
        ScaleSmokeFalloff(5.0f, falloff);
        SetSmokeFftFalloff(*smokeFft[effect.texture->slot], falloff);
        SetSmokeFftAmplitude(*smokeFft[effect.texture->slot], 0.30000001f);
    }
    ParticleEmitter &emitter = emitters[effect.slot];
    emitter.initializer = initializer;
    emitter.position = position;
    emitter.spawnVariance = spawnVolume;
    emitter.velocity = effect.velocity;
    emitter.velocityVariance = effect.velocityVariance;
    SetEmitterBirthRate(emitter, birthRate);
    SetEmitterSize(emitter, effect.width, effect.height);
    emitter.gpuFlags = ((emitter.gpuFlags | 0x100000U) & 0x00ffffffU) | 0x94000000U;
    emitter.textureFlags |= 0x02000000U;
    emitter.flags |= 0x40000000U;
    emitter.gpuFlags = (emitter.gpuFlags & ~0x4000U) | 0x2000U;
    return index;
}

// @verified - p
void SetSmokeEffectSpread(std::uint32_t index, float falloff, float opacity) {
    SmokeEffect *const effect = smokeEffects[index];
    if (!effect || (effect->profile != 3 && effect->profile != 4))
        return;
    smokeOpacityScale[effect->profile] = opacity;
    std::int32_t step = static_cast<std::int32_t>((falloff - 1.0f) * 13.888889f + 3.0f);
    if (step < 0)
        step = 0;
    smokeAlphaStep[effect->profile] = step;
}

// @verified - p
void SetSmokeEffectRadialMotion(std::uint32_t index, float velocity, float decay) {
    SmokeEffect *const effect = smokeEffects[index];
    if (!effect || (effect->profile != 3 && effect->profile != 4))
        return;
    smokeRadialVelocity[effect->profile] = velocity;
    smokeRadialDecay[effect->profile] = decay;
}

// @verified - p
std::uint64_t CreateFireEffect(const Vector3 &position, float width, float height, std::int32_t drawMode, std::uint16_t flags) {
    std::uint32_t index = 0;
    while (index < fireEffects.size() && fireEffects[index])
        ++index;
    if (index == fireEffects.size())
        return invalidEffect;
    EnqueueEffectControlTask();
    auto *const effect = static_cast<FireEffect *>(HLib::AllocateTaskData(sizeof(FireEffect), effectToken));
    fireEffects[index] = effect;
    if (effect)
        std::memset(effect, 0, sizeof(*effect));
    if (!fireFft[index]) {
        fireFft[index] = static_cast<FireFftData *>(HLib::AllocateTaskData(sizeof(FireFftData), SHEN_NAME(FIRE)));
        if (fireFft[index]) {
            std::memset(fireFft[index], 0, sizeof(FireFftData));
            InitializeFireFft(*fireFft[index]);
        }
    }
    if (!effect || !fireFft[index])
        return invalidEffect;
    if (flags & 1U) {
        constexpr std::size_t numTrails = 512;
        constexpr std::size_t fireTrailInstancesSize = sizeof(FireTrailEntry) * numTrails;

        effect->trail = static_cast<FireTrailEntry *>(HLib::AllocateTaskData(fireTrailInstancesSize, SHEN_NAME(FIRB)));
        if (effect->trail) {
            std::memset(effect->trail, 0, fireTrailInstancesSize);
            for (std::size_t i = 0; i != numTrails; ++i) {
                effect->trail[i].age = -1;
                effect->trail[i].position = position;
            }
        }
    }
    if (flags & 4U) {
        constexpr std::size_t numInstances = 32;
        constexpr std::size_t fireInstancesSize = sizeof(FireInstance) * numInstances;

        effect->instances = static_cast<FireInstance *>(HLib::AllocateTaskData(fireInstancesSize, SHEN_NAME(FIRC)));
        if (effect->instances) {
            std::memset(effect->instances, 0, fireInstancesSize);
            for (std::size_t i = 0; i != numInstances; ++i) {
                FireInstance &instance = effect->instances[i];
                instance.localPosition = position;
                instance.width = width;
                instance.height = height;
                instance.alpha = 1.0f;
                instance.cameraOffset = 0.0f;
            }
        }
    }
    effect->worldPosition = position;
    effect->localPosition = position;
    effect->cameraOffset = 0.0f;
    effect->width = width;
    effect->height = height;
    effect->drawFlags = static_cast<std::uint32_t>(drawMode);
    effect->flags = flags;
    effect->alpha = 1.0f;
    effect->slot = AllocateEffectSlot(fireSlots, -1, FireControlCallback, effect, effectToken);
    effect->pixels = static_cast<std::byte *>(HLib::AllocateTaskData(0x81f, effectToken));
    FireFftData &fft = *fireFft[index];
    if (effect->pixels) {
        auto *const aligned = reinterpret_cast<std::uint16_t *>((reinterpret_cast<std::uintptr_t>(effect->pixels) + 31U) & ~std::uintptr_t{31U});
        SetFireFftOutput(fft, aligned);
        const d3t::Texture2DDescription description{32, 32, 1, 1, 2, 0, 0, 1, 0, 0, nullptr};
        for (std::size_t i = 0; i != 2; ++i) {
            fireTextureHandles[index][i] = d3t::CreateTexture2D(&description);
        }
        SetTextureRecordHandle(fft.graphicsRecord0, fireTextureHandles[index][0]);
        SetTextureRecordHandle(fft.graphicsRecord1, fireTextureHandles[index][1]);
        std::array<float, 256> falloff{};
        GenerateFireFalloff(3.137255f, 4.9411764f, falloff);
        ScaleFireFalloff(0.78431374f, falloff);
        SetFireFftFalloff(fft, falloff);
    }
    SetFireFftScalar(fft, -0.15294118f);
    SetFireFftSize(fft, width, height);
    SuspendParticleEffect(4, index);
    return index;
}

// @verified - p
void SuspendParticleEffect(std::int32_t type, std::uint32_t index) {
    if (type == 4) {
        if (FireEffect *const effect = FindFireEffect(index)) {
            if (fireSlots[effect->slot].state == 2) {
                fireSlots[effect->slot].state = 1;
                fireFft[effect->slot]->flags &= ~activeFlag;
            }
        }
        return;
    }
    SmokeEffect *const effect = FindSmokeEffect(type, index);
    if (!effect)
        return;
    if (emitterSlots[effect->slot].state == 2) {
        emitterSlots[effect->slot].state = 1;
        emitters[effect->slot].flags &= ~activeFlag;
    }
    if (effect->texture && static_cast<std::uint32_t>(effect->profile) <= 4U) {
        SmokeTextureOwner &texture = *effect->texture;
        texture.activeEmitters &= static_cast<std::uint16_t>(~(1U << effect->slot));
        if (texture.activeEmitters == 0 && smokeSlots[texture.slot].state == 2) {
            smokeSlots[texture.slot].state = 1;
            smokeFft[texture.slot]->flags &= ~activeFlag;
        }
    }
    effect->flags.name &= ~2U;
}

// @verified - p
void ResumeParticleEffect(std::int32_t type, std::uint32_t index) {
    if (type == 4) {
        if (FireEffect *const effect = FindFireEffect(index)) {
            if (fireSlots[effect->slot].state == 1) {
                fireSlots[effect->slot].state = 2;
                fireFft[effect->slot]->flags |= activeFlag;
            }
        }
        return;
    }
    if (index == invalidEffect || static_cast<std::uint32_t>(type) > 3U)
        return;
    SmokeEffect *const effect = ParticleEffectEntry(type, index);
    if (emitterSlots[effect->slot].state == 1) {
        emitterSlots[effect->slot].state = 2;
        emitters[effect->slot].flags |= activeFlag;
    }
    if (effect->texture && static_cast<std::uint32_t>(effect->profile) <= 4U) {
        SmokeTextureOwner &texture = *effect->texture;
        if (smokeSlots[texture.slot].state == 1) {
            smokeSlots[texture.slot].state = 2;
            smokeFft[texture.slot]->flags |= activeFlag;
        }
        texture.activeEmitters |= static_cast<std::uint16_t>(1U << effect->slot);
    }
    effect->flags.name |= 2U;
}

// @verified - p
void SetParticleEffectVelocity(std::int32_t type, std::uint32_t index, const Vector3 &velocity, const Vector3 &variance) {
    if (SmokeEffect *const effect = FindSmokeEffect(type, index)) {
        effect->velocity = velocity;
        effect->velocityVariance = variance;
        emitters[effect->slot].velocity = velocity;
        emitters[effect->slot].velocityVariance = variance;
    }
}

// @verified - p
void SetParticleEffectColour(std::int32_t type, std::uint32_t index, std::int32_t alpha, std::uint8_t red, std::uint8_t green, std::uint8_t blue) {
    if (type != 0 && type != 1)
        return;
    if (SmokeEffect *const effect = FindSmokeEffect(type, index)) {
        const std::uint32_t colour =
            (static_cast<std::uint32_t>(alpha) << 24U) | (static_cast<std::uint32_t>(red) << 16U) | (static_cast<std::uint32_t>(green) << 8U) | blue;
        emitters[effect->slot].colour = colour;
        for (std::int32_t i = 0; i < effect->count; ++i) {
            effect->particles[i].colour = colour;
        }
    }
}

// @verified - p
void SetParticleEffectPosition(std::int32_t type, std::uint32_t index, const Vector3 *position, const Vector3 *variance) {
    if (type == 4) {
        if (FireEffect *const effect = FindFireEffect(index); effect && position)
            effect->localPosition = *position;
    } else if (SmokeEffect *const effect = FindSmokeEffect(type, index)) {
        effect->position = *position;
        effect->spawnVariance = *variance;
        emitters[effect->slot].position = effect->position;
        emitters[effect->slot].spawnVariance = effect->spawnVariance;
    }
}

// @verified - p
void SetParticleEffectSize(std::int32_t type, std::uint32_t index, float width, float height) {
    if (type == 4) {
        if (FireEffect *const effect = FindFireEffect(index)) {
            effect->width = width;
            effect->height = height;
            SetFireFftSize(*fireFft[effect->slot], width, height);
        }
    } else if (SmokeEffect *const effect = FindSmokeEffect(type, index)) {
        effect->width = width;
        effect->height = height;
        SetEmitterSize(emitters[effect->slot], width, height);
    }
}

// @verified - p
void SetParticleEffectInitialScale(std::int32_t type, std::uint32_t index, float scale) {
    if (index == invalidEffect || static_cast<std::uint32_t>(type) > 3U)
        return;
    SmokeEffect *const effect = ParticleEffectEntry(type, index);
    if (type == 0 && effect->profile >= 0 && effect->profile <= 4 && effect->profile != 1)
        smokeInitialScale[effect->profile] = scale;
    if (effect)
        effect->initialScale = scale;
}

// @verified - p
void SetParticleEffectBirthRate(std::int32_t type, std::uint32_t index, float fraction) {
    if (SmokeEffect *const effect = FindSmokeEffect(type, index)) {
        effect->birthRate = fraction;
        SetEmitterBirthRate(emitters[effect->slot], fraction);
    }
}

// @verified - p
void SetSmokeEffectFalloff(std::uint32_t index, std::int32_t value) {
    SmokeEffect *const effect = FindSmokeEffect(0, index);
    if (!effect)
        return;
    const float setting = static_cast<float>(value);
    if (effect->texture) {
        std::array<float, 64> falloff;
        GenerateSmokeFalloff(((setting * 10.0f) / 255.0f) - 2.0f, falloff);
        ScaleSmokeFalloff((effect->fadeFactor - 0.69999999f) * 100.0f, falloff);
        SetSmokeFftFalloff(*smokeFft[effect->texture->slot], falloff);
    }
    if (static_cast<std::uint32_t>(effect->profile) > 4U)
        return;
    const float expansion = (setting / 255.0f) * 0.02f - 0.0039999997f;
    if (effect->profile != 1)
        smokeExpansion[effect->profile] = expansion;
    effect->expansion = expansion;
}

// @verified - p
void SetSmokeEffectFadeFactor(std::uint32_t index, std::int32_t value) {
    SmokeEffect *const effect = FindSmokeEffect(0, index);
    if (!effect)
        return;
    const float setting = static_cast<float>(value);
    if (effect->texture) {
        std::array<float, 64> falloff;
        GenerateSmokeFalloff(((effect->expansion + 0.0039999997f) * 10.0f) / 0.02f - 2.0f, falloff);
        ScaleSmokeFalloff((setting * 100.0f) / 255.0f, falloff);
        SetSmokeFftFalloff(*smokeFft[effect->texture->slot], falloff);
    }
    if (static_cast<std::uint32_t>(effect->profile) > 4U)
        return;
    const float fade = setting / 255.0f + 0.69999999f;
    if (effect->profile != 1)
        smokeFadeFactor[effect->profile] = fade;
    effect->fadeFactor = fade;
}

// @verified - p
void SetSmokeEffectPhaseRate(std::uint32_t index, float rate) {
    if (index == invalidEffect)
        return;
    const SmokeEffect &effect = *smokeEffects[static_cast<std::int32_t>(index)];
    SetSmokeFftAmplitude(*smokeFft[effect.texture->slot], rate);
}

// @verified - p
void SetFirePalette(std::uint32_t index, const std::uint16_t *palette) {
    const FireEffect &effect = *fireEffects[static_cast<std::int32_t>(index)];
    SetFireFftPalette(*fireFft[effect.slot], palette);
}

// @verified - p
void SetFireFalloff(std::uint32_t index, float radiusX, float radiusY, float gain) {
    const FireEffect &effect = *fireEffects[static_cast<std::int32_t>(index)];
    std::array<float, 256> falloff{};
    GenerateFireFalloff(radiusX, radiusY, falloff);
    ScaleFireFalloff(gain, falloff);
    SetFireFftFalloff(*fireFft[effect.slot], falloff);
}

// @verified - p
void SetFireFalloffScalar(std::uint32_t index, float scalar) {
    const FireEffect &effect = *fireEffects[static_cast<std::int32_t>(index)];
    SetFireFftScalar(*fireFft[effect.slot], scalar);
}

// @verified - p
std::uint32_t SetFirePosition(std::uint32_t index, std::int32_t instance, const Vector3 *position) {
    FireInstance &target = fireEffects[static_cast<std::int32_t>(index)]->instances[instance];
    if (!position) {
        target.enabled = 0;
        return 0;
    }
    target.localPosition = *position;
    target.enabled = 1;
    return 1;
}

// @verified - p
Character *AttachFireEffect(std::uint32_t index, shen_name name, std::int16_t bone, float offset) {
    FireEffect &effect = *fireEffects[static_cast<std::int32_t>(index)];
    Character *const character = name.name ? FindCharacterByScenarioName(name) : nullptr;
    effect.cameraOffset = offset;
    effect.character = character;
    effect.bone = bone;
    return character;
}

// @verified - p
void SetFireAlpha(std::uint32_t index, float alpha) {
    fireEffects[static_cast<std::int32_t>(index)]->alpha = alpha;
}

// @verified - p
Character *AttachFireInstance(std::uint32_t index, std::int32_t instance, shen_name name, std::int16_t bone, std::int32_t offsetBits) {
    FireInstance &target = fireEffects[static_cast<std::int32_t>(index)]->instances[instance];
    Character *const character = name.name ? FindCharacterByScenarioName(name) : nullptr;
    target.cameraOffset = std::bit_cast<float>(offsetBits);
    target.character = character;
    target.bone = bone;
    return character;
}

// @verified - p
void SetFireCameraOffset(std::uint32_t index, float offset) {
    fireEffects[static_cast<std::int32_t>(index)]->cameraOffset = offset;
}

// @verified - p
void SetFireInstanceCameraOffset(std::uint32_t index, std::int32_t instance, float offset) {
    fireEffects[static_cast<std::int32_t>(index)]->instances[instance].cameraOffset = offset;
}

// @verified - p
void SetFireInstanceAlpha(std::uint32_t index, std::int32_t instance, float alpha) {
    fireEffects[static_cast<std::int32_t>(index)]->instances[instance].alpha = alpha;
}

// @verified - p
void SetFireInstanceSize(std::uint32_t index, std::int32_t instance, float width, float height) {
    FireInstance &target = fireEffects[static_cast<std::int32_t>(index)]->instances[instance];
    target.width = width;
    target.height = height;
}

// @verified - p
void SetSmokeEffectFalloffCurve(std::uint32_t index, float radius, float gain) {
    if (index == invalidEffect)
        return;
    const SmokeEffect &effect = *smokeEffects[static_cast<std::int32_t>(index)];
    std::array<float, 64> falloff;
    GenerateSmokeFalloff(radius, falloff);
    ScaleSmokeFalloff(gain, falloff);
    SetSmokeFftFalloff(*smokeFft[effect.texture->slot], falloff);
}

// @verified - p
void SetStationarySmokeAlpha(std::uint32_t index, std::int32_t alpha) {
    const SmokeEffect *const effect = smokeEffects[static_cast<std::int32_t>(index)];
    if (effect && effect->profile == 1)
        stationarySmokeAlpha = static_cast<float>(alpha);
}

// @verified - p
void SetParticleEffectUniformSize(std::int32_t type, std::uint32_t index, float size) {
    if (type == 4) {
        if (FireEffect *const effect = FindFireEffect(index)) {
            effect->width = effect->height = size;
            SetFireFftSize(*fireFft[effect->slot], size, size);
        }
    } else if (SmokeEffect *const effect = FindSmokeEffect(type, index)) {
        effect->width = effect->height = size;
        SetEmitterSize(emitters[effect->slot], size, size);
    }
}

// @verified - p
void TranslateParticleEffectParticles(std::int32_t type, std::uint32_t index, const Vector3 &delta) {
    SmokeEffect *const effect = FindSmokeEffect(type, index);
    if (!effect)
        return;
    for (std::int32_t particleIndex = 0; particleIndex < effect->count; ++particleIndex) {
        ParticleRecord &particle = effect->particles[particleIndex];
        particle.position.x = EffectDelta(delta.x) + particle.position.x;
        particle.position.y = EffectDelta(delta.y) + particle.position.y;
        particle.position.z = EffectDelta(delta.z) + particle.position.z;
    }
}

// @verified - p
std::uint64_t GetParticleEffectVisibility(std::int32_t type, std::uint32_t index) {
    if (index == invalidEffect)
        return 0;
    if (type == 4) {
        const FireEffect *const effect = FindFireEffect(index);
        return effect ? ((effect->drawFlags & 2U) != 0) : invalidEffect;
    }
    const SmokeEffect *const effect = FindSmokeEffect(type, index);
    return effect ? ((effect->flags.name & 2U) != 0) : invalidEffect;
}

// @verified - p
void FreeParticleEffect(std::int32_t type, std::uint32_t index) {
    if (type == 4) {
        FireEffect *const effect = FindFireEffect(index);
        if (!effect)
            return;
        if (effect->pixels)
            HLib::FreeTaskData(effect->pixels);
        if (effect->trail)
            HLib::FreeTaskData(effect->trail);
        if (effect->instances)
            HLib::FreeTaskData(effect->instances);
        if (fireFft[index])
            HLib::FreeTaskData(fireFft[index]);
        fireFft[index] = nullptr;
        ClearEffectSlot(fireSlots[effect->slot]);
        HLib::FreeTaskData(effect);
        fireEffects[index] = nullptr;
        return;
    }
    if (index == invalidEffect || static_cast<std::uint32_t>(type) > 3U)
        return;
    SmokeEffect *const effect = ParticleEffectEntry(type, index);
    if (type == 0 && !effect)
        return;
    if (effect->particles)
        HLib::FreeTaskData(effect->particles);
    ParticleEffectEntry(type, index) = nullptr;
    SmokeTextureOwner *const texture = effect->texture;
    if (texture && static_cast<std::uint32_t>(effect->profile) <= 4U) {
        if (texture->pixels) {
            auto &handle = smokeTextureHandles[effect->slot];
            if (handle) {
                d3t::InitTexture2DResource(handle);
                --g_legacyEffectTextureCount;
            }
            handle = 0;
            emitters[effect->slot].textureRecord = nullptr;
            emitters[effect->slot].textureHandle = 0;
            emitters[effect->slot].textureFlags = (emitters[effect->slot].flags & 1U) ? 0xa0000002U : 0x80000002U;
            emitters[effect->slot].samplerControl = 0x88800000U;
            emitters[effect->slot].gpuFlags = 0x208000c0U;
            smokeTextureRecords[effect->slot].handle = 0;
        }
        --texture->references;
        if (texture->references == 0) {
            smokeFft[texture->slot]->flags &= ~activeFlag;
            HLib::FreeTaskData(smokeFft[texture->slot]);
            smokeFft[texture->slot] = nullptr;
            ClearEffectSlot(smokeSlots[texture->slot]);
            smokeTextures[(effect->flags.name >> 4U) & 0xfU] = nullptr;
            if (texture->pixels)
                HLib::FreeTaskData(texture->pixels);
            HLib::FreeTaskData(texture);
        }
    }
    emitters[effect->slot].flags &= ~activeFlag;
    ClearEffectSlot(emitterSlots[effect->slot]);
    HLib::FreeTaskData(effect);
}
} // namespace shenmue::s2
