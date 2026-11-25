#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/lflt.hpp"
#include "s2/hlib.hpp"
#include "s2/weather_particles_data.hpp"
#include <cstddef>

namespace shenmue {
namespace s2 {

struct WeatherRainParticle;
struct WeatherRippleParticle;
struct WeatherWind;
struct WeatherParticleGroup;
struct WeatherRainOwner;
typedef void (*WeatherRainParticleUpdate)(WeatherRainParticle *, WeatherParticleGroup *);
typedef void (*WeatherRippleParticleUpdate)(WeatherRippleParticle *, WeatherParticleGroup *);
struct WeatherRainParticle {
    std::uint32_t flags;
    WeatherRainParticleUpdate update;
    Vector3 position, drift;
};
struct WeatherRippleParticle {
    std::uint32_t flags;
    WeatherRippleParticleUpdate update;
    Vector3 position;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    union {
        std::int32_t rotationPitch;
        std::int32_t angleX;
    };
    union {
        std::int32_t rotationYaw;
        std::int32_t angleY;
    };
    union {
        std::int32_t retainedRotation;
        std::int32_t angleZ;
    };
    union {
        std::uint32_t *colourRampEntry;
        const std::uint32_t *colour;
    };
    union {
        float age;
        float scale;
    };
    float lifetime;
#else
    std::int32_t rotationPitch, rotationYaw, retainedRotation;
    std::uint32_t *colourRampEntry;
    float age, lifetime;
#endif
};
struct WeatherParticlePacket {
    std::uint32_t command, isp, tsp, tcw;
};
struct WeatherRippleDrawState {
    std::uint32_t flags;
    float firstParameter, distanceLimit, thirdParameter;
    std::int32_t controls[3];
    std::int32_t characterLayer;
    std::uint32_t retainedState;
    void *texture;
};
typedef char Layout_weather_particles_dc_0[(sizeof(WeatherRainParticle) == 0x20) ? 1 : -1];
typedef char Layout_weather_particles_dc_1[(offsetof(WeatherRainParticle, update) == 4) ? 1 : -1];
typedef char Layout_weather_particles_dc_2[(offsetof(WeatherRainParticle, position) == 8) ? 1 : -1];
typedef char Layout_weather_particles_dc_3[(sizeof(WeatherRippleParticle) == 0x2c) ? 1 : -1];
typedef char Layout_weather_particles_dc_4[(offsetof(WeatherRippleParticle, colourRampEntry) == 0x20) ? 1 : -1];
typedef char Layout_weather_particles_dc_5[(sizeof(WeatherParticlePacket) == 0x10) ? 1 : -1];
typedef char Layout_weather_particles_dc_6[(sizeof(WeatherRippleDrawState) == 0x28) ? 1 : -1];
typedef char Layout_weather_particles_dc_7[(offsetof(WeatherRippleDrawState, texture) == 0x24) ? 1 : -1];
#define WEATHER_GROUP_STORAGE SHEN_NAME4('G', 'B', 'L', 'S')
#define WEATHER_RAIN_PARTICLE_STORAGE SHEN_NAME4('R', 'I', 'N', 'P')
#define WEATHER_RIPPLE_PARTICLE_STORAGE SHEN_NAME4('R', 'P', 'L', 'P')
struct WeatherRainParameters {
    float first, second;
    Vector3 drift, variance;
    std::uint32_t colour;
    float damping;
};
typedef char Layout_weather_particles_dc_8[(sizeof(WeatherRainParameters) == 0x28) ? 1 : -1];
static const float weatherRainDrawCost[2] = {1.7391303777694702f, 2.090909004211426f};
static const float weatherRippleDrawCost[2] = {3.3333332538604736f, 3.8888888359069824f};
static const float weatherRainDrawRate[2] = {0.574999988079071f, 0.47826087474823f};
static const float weatherRippleDrawRate[2] = {0.30000001192092896f, 0.2571428716182709f};
void EnsureWeatherRainOwnerMode0(WeatherRainOwner *owner, float scale);
void EnsureWeatherRainOwnerMode1(WeatherRainOwner *owner, float scale);
void EnsureWeatherRainGroup(WeatherParticleGroup *group, float scale);
void EnsureWeatherRippleOwnerMode0(WeatherRainOwner *owner, float scale);
void EnsureWeatherRippleOwnerMode1(WeatherRainOwner *owner, float scale);
void EnsureWeatherRippleGroup(WeatherParticleGroup *group, float scale);
struct MapInfoChunk;
struct WeatherClipChunkHeader {
    std::uint32_t retainedHeader;
    std::int32_t firstCount, secondCount;
    std::uint32_t retainedTail;
};
typedef char Layout_weather_particles_dc_9[(sizeof(WeatherClipChunkHeader) == 0x10) ? 1 : -1];
#define WEATHER_CLIP_STORAGE SHEN_NAME4('W', 'C', 'C', 'P')
extern WeatherClipSet weatherClipSets[2];
struct WeatherClipVolume {
    Vector2 points[4];
    float clipY0, clipY1;
};
typedef char WeatherClipVolumeSizeCheck[sizeof(WeatherClipVolume) == 40 ? 1 : -1];
void ConfigureWeatherVolume(const WeatherClipVolume *volume);
void ReleaseWeatherVolumeGroup(std::int32_t index);
void SetWeatherVolumeGroupEnabled(std::int32_t enabled, std::int32_t index);
void SetWeatherVolumeEnabled(std::int32_t index, std::uint32_t enabled, std::int32_t group);

void LoadWeatherClipChunk(const MapInfoChunk *chunk);
void ReadWeatherClipData(const void *data);
void Weather_LoadWCCPEntries(const void *entries, std::int32_t count, std::int32_t setIndex);
void Weather_UpdateWCCPClipSet0();
void Weather_UpdateWCCPClipSet1();
void Weather_ResetWCCPClipSet0();
void Weather_ResetWCCPClipSet1();
extern WeatherParticleGroup *weatherGroupList;
extern WeatherInteractionCamera *weatherInteractionCamera;
extern HLib::Task *weatherParticleResourceTask;
extern std::int32_t weatherWarState, g_currentWeatherForceFlag;
extern float weatherRainSpawnLimit, weatherSecondaryRainParameter, weatherRippleSpawnLimit, weatherParticleUnusedParameter;
extern Vector3 weatherParticleGroundCameraPosition;
#define WEATHER_INTERACTION_CAMERA_STORAGE SHEN_NAME4('I', 'C', 'A', 'R')
#define WEATHER_RESOURCE_TASK SHEN_NAME4('E', 'F', 'F', 'E')
WeatherInteractionCamera *AllocateWeatherInteractionCamera();
void AdvanceWeatherInteractionCamera(WeatherInteractionCamera *camera);
HLib::Task *AllocateICAR(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused);
HLib::Task *WAR_CallbackParticleResource_Enqueue(float rainSpawnLimit, float secondaryRainParameter, float rippleSpawnLimit, float unused);
HLib::Task *EnqueueWeatherParticleResource();
void WAR_CallbackParticleResource_Callback(void *);
void UpdateWeatherInteractionCamera();
void UpdateWeatherParticleGroundCameraPosition();
void SetRainGroupDrawingEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetRippleGroupDrawingEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetWeatherParticleDrawingEnabled(std::int32_t enabled);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetRainGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetRippleGroupUpdatesEnabled(WeatherParticleGroup *group, std::int32_t enabled);
WeatherParticleGroup *GetNextWeatherParticleGroup(const WeatherParticleGroup *group);
void SetRainAndRippleFlagsTo(std::int32_t paused);
#endif
void UpdateWeatherParticleGroups();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void WeatherParticle_Callback(void *groups);
#endif
struct WeatherParticleGroup {
    WeatherParticleGroup *next;
    WeatherParticleGroup *previous;
    std::uint32_t rainFlags;
    WeatherRainParticle *rainParticles;
    std::int32_t rainCapacity, rainLiveCount, rainDrawCount;
    WeatherFloat *rainCountLflt;
    std::int32_t rainCurrentCount;
    float rainVerticalSpan;
    Vector3 rainMotion;
    float rainMotionDamping;
    WeatherParticlePacket rainPacket;
    std::uint32_t rainColour;
    float rainPressureParam0, rainPressureParam1;
    std::int32_t rainYaw, rainPitch, rainRoll;
    float spawnDriftX;
    float fallSpeed;
    float spawnDriftZ;
    Vector3 driftVariance;
    Vector3 filteredRainMotion;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::uint32_t snowFlags;
    WeatherRainParticle *snowParticles;
    std::int32_t snowCapacity, snowLiveCount, snowDrawCount;
    WeatherFloat *snowCountLflt;
    std::int32_t snowCurrentCount;
    float snowVerticalSpan;
    Vector3 snowMotion;
    float snowMotionDamping;
    WeatherParticlePacket snowPacket;
    std::uint32_t snowColour;
    float snowWidth;
    std::byte retainedcc[0x1c];
#else
    std::byte retainedRainDrift[0x64];
#endif
    std::uint32_t rippleFlags;
    WeatherRippleParticle *rippleParticles;
    std::int32_t rippleCapacity, rippleLiveCount, rippleDrawCount;
    WeatherFloat *rippleCountLflt;
    std::int32_t rippleCurrentCount;
    WeatherParticlePacket ripplePacket;
    std::uint32_t rippleColour;
    float rippleDeltaTime;
    float cameraY;
    std::uint32_t *rippleColourRamp;
    WeatherRippleDrawState rippleDrawState;
    float spawnVolume;
    float boundsMaxY, boundsMinY, boundsMaxX, boundsMinX, boundsMaxZ, boundsMinZ;
    float frustumDistance, frustumSpan;
    WeatherClipQuadPlanes clipA, clipB;
};
typedef char Layout_weather_particles_dc_10[(sizeof(WeatherParticleGroup) == 0x200) ? 1 : -1];
typedef char Layout_weather_particles_dc_11[(offsetof(WeatherParticleGroup, rainParticles) == 0x0c) ? 1 : -1];
typedef char Layout_weather_particles_dc_12[(offsetof(WeatherParticleGroup, rainCountLflt) == 0x1c) ? 1 : -1];
typedef char Layout_weather_particles_dc_13[(offsetof(WeatherParticleGroup, rainColour) == 0x48) ? 1 : -1];
typedef char Layout_weather_particles_dc_14[(offsetof(WeatherParticleGroup, rainPressureParam0) == 0x4c) ? 1 : -1];
typedef char Layout_weather_particles_dc_15[(offsetof(WeatherParticleGroup, rainYaw) == 0x54) ? 1 : -1];
typedef char Layout_weather_particles_dc_16[(offsetof(WeatherParticleGroup, filteredRainMotion) == 0x78) ? 1 : -1];
typedef char Layout_weather_particles_dc_17[(offsetof(WeatherParticleGroup, spawnDriftX) == 0x60) ? 1 : -1];
typedef char Layout_weather_particles_dc_18[(offsetof(WeatherParticleGroup, fallSpeed) == 0x64) ? 1 : -1];
typedef char Layout_weather_particles_dc_19[(offsetof(WeatherParticleGroup, driftVariance) == 0x6c) ? 1 : -1];
typedef char Layout_weather_particles_dc_20[(offsetof(WeatherParticleGroup, rippleParticles) == 0xec) ? 1 : -1];
typedef char Layout_weather_particles_dc_21[(offsetof(WeatherParticleGroup, rippleCountLflt) == 0xfc) ? 1 : -1];
typedef char Layout_weather_particles_dc_22[(offsetof(WeatherParticleGroup, rippleColour) == 0x114) ? 1 : -1];
typedef char Layout_weather_particles_dc_23[(offsetof(WeatherParticleGroup, rippleDeltaTime) == 0x118) ? 1 : -1];
typedef char Layout_weather_particles_dc_24[(offsetof(WeatherParticleGroup, rippleColourRamp) == 0x120) ? 1 : -1];
typedef char Layout_weather_particles_dc_25[(offsetof(WeatherParticleGroup, spawnVolume) == 0x14c) ? 1 : -1];
typedef char Layout_weather_particles_dc_26[(offsetof(WeatherParticleGroup, boundsMaxY) == 0x150) ? 1 : -1];
typedef char Layout_weather_particles_dc_27[(offsetof(WeatherParticleGroup, boundsMinY) == 0x154) ? 1 : -1];
typedef char Layout_weather_particles_dc_28[(offsetof(WeatherParticleGroup, clipA) == 0x170) ? 1 : -1];
typedef char Layout_weather_particles_dc_29[(offsetof(WeatherParticleGroup, clipB) == 0x1b8) ? 1 : -1];
struct WeatherFixcNode {
    WeatherFixcNode *previous;
    WeatherFixcNode *next;
    std::int32_t index;
    union Payload {
        void *pointer;
        float scalar;
        std::int32_t integer;
    } payload;
};
struct WeatherFixcOwner {
    std::uint32_t mode;
    void *cara;
    WeatherFixcNode *windList;
};
struct WeatherRainOwner {
    WeatherParticleGroup *rain0;
    WeatherParticleGroup *rain1;
    std::uint32_t mode;
    void *storage;
    WeatherParticleGroup *rippleGroup;
    WeatherParticleGroup *rippleGroup1;
    std::int32_t frustumFov;
};
typedef char Layout_weather_particles_dc_30[(sizeof(WeatherFixcNode) == 0x10) ? 1 : -1];
typedef char Layout_weather_particles_dc_31[(offsetof(WeatherFixcNode, payload) == 0x0c) ? 1 : -1];
typedef char Layout_weather_particles_dc_32[(sizeof(WeatherFixcOwner) == 0x0c) ? 1 : -1];
typedef char Layout_weather_particles_dc_33[(offsetof(WeatherFixcOwner, cara) == 4) ? 1 : -1];
typedef char Layout_weather_particles_dc_34[(sizeof(WeatherRainOwner) == 0x1c) ? 1 : -1];
typedef char Layout_weather_particles_dc_35[(offsetof(WeatherRainOwner, frustumFov) == 0x18) ? 1 : -1];

#define WEATHER_FIXC_STORAGE SHEN_NAME4('F', 'I', 'X', 'C')
#define WEATHER_FIXC_NODE_STORAGE SHEN_NAME4('D', 'L', 'S', 'T')
#define WEATHER_RAIN_OWNER_STORAGE SHEN_NAME4('C', 'A', 'R', 'A')
WeatherFixcNode *AllocateWeatherFixcNode(std::int32_t index, void *payload);
void FreeWeatherFixcNode(WeatherFixcNode *node);
void *GetWeatherFixcNodeData(const WeatherFixcNode *node);
WeatherFixcNode *FindWeatherFixcNode(WeatherFixcNode *node, const void *payload);
WeatherFixcNode *GetLastWeatherFixcNode(WeatherFixcNode *node);
WeatherFixcNode *GetPreviousWeatherFixcNode(const WeatherFixcNode *node);
WeatherFixcNode *GetNextWeatherFixcNode(const WeatherFixcNode *node);
void LinkWeatherFixcNodes(WeatherFixcNode *first, WeatherFixcNode *second);
void SplitWeatherFixcNodes(WeatherFixcNode *first, WeatherFixcNode *second);
void DetachWeatherFixcNode(WeatherFixcNode *node);
WeatherFixcOwner *AllocateWeatherFixc(std::int32_t fov);
WeatherRainOwner *AllocateWeatherRainOwner(std::int32_t fov);
std::uint32_t GetRainGroupFlags(const WeatherParticleGroup *group);
std::uint32_t GetRippleGroupFlags(const WeatherParticleGroup *group);
std::uint32_t GetRainOwnerFlags(const WeatherRainOwner *owner);
std::uint32_t GetRainOwnerRippleFlags(const WeatherRainOwner *owner);
std::uint32_t GetWeatherFixcRainFlags(const WeatherFixcOwner *owner);
std::uint32_t GetWeatherFixcRippleFlags(const WeatherFixcOwner *owner);
void SetRainGroupDrift(WeatherParticleGroup *group, const Vector3 *drift, const Vector3 *variance);
void SetRainOwnerDrift(WeatherRainOwner *owner, const Vector3 *drift, const Vector3 *variance);
void SetWeatherFixcRainDrift(WeatherFixcOwner *owner, const Vector3 *drift, const Vector3 *variance);
void SetRainGroupParameters(WeatherParticleGroup *group, float first, float second);
void SetRainOwnerParameters(WeatherRainOwner *owner, float first, float second);
void SetWeatherFixcRainParameters(WeatherFixcOwner *owner, float first, float second);
void SetRippleGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void SetRainOwnerRippleCountImmediate(WeatherRainOwner *owner, float pressure);
void SetWeatherFixcRippleCountImmediate(WeatherFixcOwner *owner, float pressure);
void SetRippleGroupTimeStep(WeatherParticleGroup *group, float step);
void SetRainOwnerRippleTimeStep(WeatherRainOwner *owner, float step);
void SetWeatherFixcRippleTimeStep(WeatherFixcOwner *owner, float step);
void EnsureWeatherRainForFixc(WeatherFixcOwner *owner, float scale);
void EnsureWeatherRippleForFixc(WeatherFixcOwner *owner, float scale);
std::uint32_t GetWeatherFixcType(const WeatherFixcOwner *owner);
bool IsWeatherFixcRegistered(WeatherFixcOwner *owner);
void RegisterWeatherFixc(WeatherFixcOwner *owner);
void UnregisterWeatherFixc(WeatherFixcOwner *owner);
bool WeatherFixcHasWind(const WeatherFixcOwner *owner, WeatherWind *wind);
void AddWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind);
void RemoveWeatherWindLink(WeatherFixcOwner *owner, WeatherWind *wind);
void Weather_RegisterActiveFIXCAndInitEffects();
void EnsureWeatherFixcRegistered();
void UnregisterActiveWeatherFixc();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FreeWeatherFixc(WeatherFixcOwner *owner);
void Weather_FreeActiveFIXC();
void EV_WeatherEffectUtil_Impl(std::uint32_t fov, float rainScale, float unused, float rippleScale);
void EV_wthrApplyRainGroups(const Vector2 *clip);
#endif

float WeatherFrustumSpan(float distance, std::int32_t fieldOfView);
void SetWeatherGroupBounds(WeatherParticleGroup *group, const Vector3 *center, float span);
void ApplyWeatherRainWind(WeatherParticleGroup *group, const Vector3 *wind);
void UpdateWeatherRainOrientation(WeatherParticleGroup *group, const Vector3 *previous, const Vector3 *current);
void ApplyWeatherPressureParameters(WeatherParticleGroup *group, float pressure, std::int32_t side);
void SetWeatherRipplePressureParameters(float pressure);
extern std::int32_t weatherDrawBudgetEnabled;
extern float weatherDrawBudgetRemaining;
struct WeatherDrawBudgetChunk {
    shen_name name;
    std::uint32_t byteSize, flags;
};
typedef char Layout_weather_particles_dc_36[(sizeof(WeatherDrawBudgetChunk) == 12) ? 1 : -1];
void FOCK_Reader(const MapInfoChunk *parameters);
void SetWeatherDrawBudgetEnabled(std::int32_t enabled);
std::int32_t GetWeatherDrawBudgetEnabled();
std::int32_t IsWeatherRainSpawningEnabled(const WeatherParticleGroup *group);
void SetWeatherRainSpawningEnabled(WeatherParticleGroup *group, std::int32_t enabled);
void SetRainGroupCountImmediate(WeatherParticleGroup *group, float pressure);
void SetRainOwnerCountImmediate(WeatherRainOwner *owner, float pressure);
void SetWeatherFixcRainCountImmediate(WeatherFixcOwner *owner, float pressure);
void BeginRainGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void BeginRainOwnerCountTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames);
void BeginWeatherFixcRainCountTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames);
void BeginWeatherRainTransition(float pressure, std::int32_t frames);
void BeginRippleGroupCountTransition(WeatherParticleGroup *group, float pressure, std::int32_t frames);
void BeginRainOwnerRippleTransition(WeatherRainOwner *owner, float pressure, std::int32_t frames);
void BeginWeatherFixcRippleTransition(WeatherFixcOwner *owner, float pressure, std::int32_t frames);
void BeginWeatherRippleTransition(float pressure, std::int32_t frames);
#define WEATHER_RIPPLE_COLOUR_STORAGE SHEN_NAME4('R', 'C', 'O', 'L')
void SetRainGroupColour(WeatherParticleGroup *group, std::uint32_t colour);
void SetRainOwnerColour(WeatherRainOwner *owner, std::uint32_t colour);
void SetWeatherFixcRainColour(WeatherFixcOwner *owner, std::uint32_t colour);
void AllocateWeatherRippleColourRamp(WeatherParticleGroup *group, std::uint32_t colour);
void SetRainOwnerRippleColour(WeatherRainOwner *owner, std::uint32_t colour);
void SetWeatherFixcRippleColour(WeatherFixcOwner *owner, std::uint32_t colour);
void SetWeatherParticleRainColour(std::uint32_t colour);
void SetWeatherParticleRippleColour(std::uint32_t colour);

} // namespace s2
} // namespace shenmue

#endif
