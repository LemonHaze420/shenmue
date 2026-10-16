#pragma once

#include "s1/task/effe.hpp"
#include "logging.hpp"
#include "s1/boot.hpp"
#include "s1/collision.hpp"
#include "s1/date_time.hpp"
#include "s1/task/char.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/paus.hpp"
#include "s1/task/wexe.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/wind.hpp"

#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/mt5_render.hpp"
#include "s1/lighting.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/pvr_decoder.hpp"
#include "s1/scene.hpp"
#include "s1/render_passes.hpp"
#include "s1/render_array.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/scrl.hpp"
#include "s1/task/trwv.hpp"
#include "s1/input.hpp"
#include "s1/timing.hpp"
#include "d3t/image_texture_loader.hpp"
#include "s1/d3t/settings.hpp"

#include <algorithm>
#include <bit>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include <numbers>
#include <utility>
#include <new>

#include "s1/task/effe_weather.hpp"
#include "s1/task/effe_resources.hpp"
#include "s1/task/effe_particles.hpp"
#include "s1/task/effe_queue.hpp"
#include "s1/task/effe_fight.hpp"

namespace shenmue::s1 {
namespace effe_detail {}
using namespace effe_detail;
extern std::uint8_t g_bDebugLog;
extern std::array<float, 2> g_debugTextPosition;

} // namespace shenmue::s1

namespace shenmue::s1::detail {

void TRWV_InitializeEffectTablesNative();
void TRWV_ResetSceneGlobalsNative();

} // namespace shenmue::s1::detail

namespace shenmue::s1 {

extern Vector4 g_effeObjectLightColour;
extern EffectQueueEntry *g_effectQueueEntries;
extern std::int32_t g_effectQueueEntryCount;
extern std::int32_t g_effectQueueEntryCapacity;
extern std::uint32_t *g_effectQueueOrder;
extern std::uint32_t *g_effectQueueOrderScratch;
extern float *g_effectQueueSortKeys;
extern bool g_replayingEffectQueue;
extern bool g_effectQueueReplayedCommandStream;
extern std::uint32_t g_effectScriptWordParameters[2];
extern Vector4 g_effectScriptMaximumIntensity;
extern Vector3 g_effectScriptVectorParameters[2];
extern float g_effectScriptScalarParameters[3];
extern HLib::Task *g_effeTask;
extern std::int32_t g_effectsTaskState;
extern std::int32_t g_effePauseState;
extern GLBSWeatherDrawState *g_pGLBSTaskData;
extern WeatherNodeData *g_weatherNodeHead;
extern std::array<EffeRegistryEntry, 16> g_effeEmitterRegistry;
extern std::array<EffeRegistryEntry, 2> g_effeFftRegistry;
extern std::array<EffeRegistryEntry, 4> g_effeTextureRegistry;
extern std::array<EffeRegistryEntry, 2> g_effeTerrainRegistry;
extern std::array<EffeEmitterTransform, 16> g_effeEmitterTransforms;
extern std::array<EffeFftData *, 2> g_effeFftData;
extern std::array<EffeFftOwner *, 16> g_effeFftOwners;
extern std::array<EffeRuntimeTexture, 16> g_effeRuntime;
extern Vector3 g_effectSubmissionLightDirection;
extern std::int32_t g_shouldLoadWeatherSprites;

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void clearEffectQueue();
extern std::array<EffeTextureResource, kEffeSlots> g_effeResources;
extern std::uint32_t g_effeSubmittedParticleSystems;
extern std::uint32_t g_effeSubmittedParticleCount;
extern std::int32_t g_effeRenderValidationSlot;
extern bool g_effeRenderValidationSubmitted;
extern std::array<EffeTaskRecord *, kEffeSlots> g_specialTasks;
extern std::array<EffeTaskRecord *, kEffeSlots> g_efptTasks;
extern std::array<EffeTaskRecord *, kSakuraSlots> g_sakuraTasks;
extern std::array<EffeTaskRecord *, kEffeSlots> g_sakuraRuntimeOwners;
extern std::array<EffeTaskRecord *, kEffeSlots> g_genericTasks;
extern std::array<std::int32_t, 5> g_vfFightGroundTaskSlots;
extern std::array<VFFightGroundEffectRecord, 5> g_vfFightGroundEffects;
extern std::uint8_t g_vfFightGroundEffectCount;
extern std::array<Vector3, 4> g_genericAnchors;
extern std::array<float, 5> g_specialLifetime;
extern std::array<float, 5> g_specialDecayScale;
extern std::array<float, 5> g_specialDecayStep;
extern std::array<float, 2> g_specialRadial;
extern std::array<float, 2> g_specialDamping;
extern std::array<float, 2> g_specialMultiplier;
extern std::array<std::int32_t, 2> g_specialFadeStep;
extern float g_specialIntensity;
extern std::int32_t g_particleSpawnSeed;
extern std::uint32_t g_specialPhase;
extern float g_genericFloor;
extern float g_sakuraRotation;
extern float g_sakuraRotationScale;
extern float g_sakuraRotationBase;
extern std::uint32_t g_sakuraStrengthBase;
extern std::int32_t g_sakuraStrength;
extern const std::byte *g_sakuraPvr;
extern std::size_t g_sakuraPvrBytes;
extern std::uint32_t g_sakuraColour;
extern Matrix4x4 g_particleScreenProjection;
extern HLib::Task *g_effeUpdateTask;
extern HLib::Task *g_sakuraUpdateTask;
extern std::uint32_t g_effectSceneGeneration;
void UpdateGeneratedSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void UpdateCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void UpdateClippedCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data);
extern WeatherSpriteTexture g_weatherSnowTexture;
extern WeatherSpriteTexture g_weatherHamonTexture;
extern ActiveWeatherNode *g_activeWeatherNodes;
extern WeatherFixc *g_tmpWeatherData;
extern bool g_effeWeatherFlag;
extern std::int32_t g_bForceDisableSnow;
extern std::uint32_t g_rainEffectOffset;
extern std::uint32_t g_rainUpdateOffset;
extern std::uint32_t g_weatherColourRampValue;
extern float g_rainParticleWidth;
extern float g_rainParticleHeight;
extern Vector3 g_rainEffectVector0;
extern Vector3 g_rainEffectVector1;
extern float g_snowParticleSize;
extern Vector3 g_snowEffectVector0;
extern Vector3 g_snowEffectVector1;
extern float g_rippleParticleSize;
void BuildRainLayersForFov(WeatherCaraData &data, float intensity);
void InitializeNativeSnowParticles(WeatherNodeData &data, float intensity, RainRandomFunction initialCallback, bool cameraRelative);
void InitializeCameraSnowSystems(WeatherFixc &fixc, float intensity);
void GenerateSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void GenerateRainParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void SpawnCaraRainParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void SpawnVolumeRainParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void Weather_UpdateSnowIntensityForData(WeatherNodeData *data, float intensity, std::int32_t stepCount);
void Weather_AttachAndDriveSystemNode(WeatherFixc *weather);
WeatherNodeData *FirstWeatherNode();
void ResetWccpClipBounds(WeatherClipBounds *bounds);
void Weather_ProcessRainIntensityForData(WeatherNodeData *data, float intensity, std::int32_t stepCount);
void Weather_SetRainLayerCount(WeatherNodeData *data, float intensity);
void Weather_UpdateSnowGroupIntensity(WeatherCaraData &data, float intensity, std::int32_t stepCount);
void Weather_SetSnowLayerCount(WeatherNodeData *data, float intensity);
void Weather_SetSnowGroupCount(const WeatherCaraData &data, float intensity);
WeatherNodeData *WeatherRippleGroup(const WeatherFixc &weather);
void Weather_SetRippleCount(const WeatherFixc &weather, float intensity);
void Weather_ApplyRippleGameData(const WeatherFixc &weather, float intensity, std::int32_t stepCount);
void Weather_UpdateColourRampForData(WeatherNodeData *data, std::uint32_t colour);
void Weather_FreeColourRamp(WeatherNodeData *data);
WeatherNodeData *AllocateGlobalWeatherData(std::size_t bytes, const Vector3 &center, float extent, float volumeFactor, bool initializeSentinels);
WeatherNodeData *AllocateAuthoredWeatherVolume(const Vector3 &center, float sizeX, float sizeY, float sizeZ);

WeatherFixc *AllocateWeatherFixc(
    std::int32_t type, std::int32_t sourceOffset, const Vector3 *center = nullptr, float sizeX = 0.0f, float sizeY = 0.0f, float sizeZ = 0.0f);

ActiveWeatherNode *FindActiveWeatherNode(const WeatherFixc *fixc);
void AttachWeatherNode(WeatherFixc *fixc);
void AllocDLST(WeatherFixc &weather, WeatherWindData *wind);
void Weather_FreeWeather(WeatherFixc &weather, WeatherWindData *wind);
void RemoveActiveWeatherNode(WeatherFixc *fixc);
void FreeRainRenderData(WeatherNodeData *rain);
void FreeSnowRenderData(WeatherNodeData *snow);
void FreeRippleRenderData(WeatherNodeData *ripple);
void FreeGlobalWeatherData(WeatherNodeData *data);
void FreeWeatherFixc(WeatherFixc *fixc);
std::uint32_t WeatherFixcOffset(const WeatherFixc *fixc);
std::uint32_t NextEffectRandom();
float WeatherRandomUnit(std::uint32_t seed);
float WeatherAngleCosine(std::int32_t angle);
void SpawnCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void SpawnClippedCameraSnowParticleNative(RainParticleNative *particle, WeatherNodeData *data);
bool AdvanceCameraSnowParticle(RainParticleNative *particle, WeatherNodeData *data, RainRandomFunction spawn);
void SpawnCameraSnowParticleCore(RainParticleNative *particle, WeatherNodeData *data, bool clipped);
std::uint32_t WeatherSpriteSizeBits(const WeatherSpriteTexture &texture);
void AssignSnowTexture(WeatherNodeData &data);
void ReadWeatherParticleModel(void *particles, const char *modelPath);
float CameraSnowRadius(std::int32_t fieldOfView, float depth);
void SetCameraSnowVolume(WeatherNodeData &data, float radius, float radiusSquared, std::int32_t layer);
WeatherNodeData *AllocateCameraSnowNode(bool clipped, float radius, std::int32_t layer);

void ConfigureCameraSnowLayer(
    WeatherNodeData *&storage, bool clipped, std::int32_t fieldOfView, float depth, std::int32_t layer, float intensity, RainRandomFunction initialCallback);

bool CameraSnowOutsideFrustumSet(const WeatherClipBounds &bounds, const Vector3 &position);

void InitializeRainLayer(WeatherNodeData &data, float intensity, RainRandomFunction generator);
void BuildCaraRainLayers(WeatherCaraData &data, float intensity, std::size_t bucketBytes, bool clipSentinels, RainRandomFunction generator);
void InitRainParticles(WeatherCaraData &data, float intensity);
std::array<EffeTaskRecord *, kEffeSlots> &FamilyTasks(EffeFamily family);
EffeTaskRecord *TaskAt(EffeFamily family, std::int32_t slot);
void ResetEffeDescriptor(EffeRuntimeTexture &runtime);
void ReleaseEffeTexture(std::int32_t runtimeSlot);
std::uint32_t CreateEffeTexture(std::uint32_t width, std::uint32_t height, const void *pixels);
void AttachEffeTexture(std::int32_t runtimeSlot, std::uint32_t handle, const void *source, bool spectrum, std::uint32_t width, std::uint32_t height);
std::size_t EffectAllocationSize(const void *allocation);
const EffePvrHeader *FindEffePvr(const void *allocation);
std::uint32_t CreatePvrEffeTexture(const EffePvrHeader *pvr);
float EffeFftRandomUnit(std::uint32_t seed);
void GenerateRandomFFTPhases(float *phases);
void NormalizeAndScaleFFTInput(float *rates, float scale);
void EffeFftStoreBin(float *real, float *imaginary, std::size_t index, std::size_t mirror, float phase, float amplitude);
void BitReverseAndPreprocess(const float *phases, float *real, float *imaginary, const float *falloff);
float WrapEffeFftPhase(float phase);
void RunFFTStagesAndReorder(EffeFftData &fft);
float *PrecomputeLogTable(float base, float *table);
float *ApplyLogClampVectorScaling(float scale, float *table);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

EffeFftOwner *EFFE_AcquireFftOwner(std::uint32_t taskFlags, float magnitude);
void EFFE_ReleaseFftOwner(EffeFftOwner *owner, std::uint32_t taskFlags);
void EffeTuneSpecialEffect(EffeTaskRecord &task, std::int32_t subtype, double sizeScale, double velocityScale);
void Special0Tune(void *argument);
void Special1Tune(void *argument);
void Special2Tune(void *argument);
void Special3Tune(void *argument);
void Special4Tune(void *argument);

inline constexpr std::array<EffeRegistryCallback, 5> kSpecialTunes{Special0Tune, Special1Tune, Special2Tune, Special3Tune, Special4Tune};

void EFPT_UpdateCallback(void *argument);
void Special0Spawn(EffeParticle *particle);
void Special1Spawn(EffeParticle *particle);
void Special2Spawn(EffeParticle *particle);
void Special3Spawn(EffeParticle *particle);
void Special4Spawn(EffeParticle *particle);
void EfptSpawnFall(EffeParticle *particle);
void EfptSpawnFallFade(EffeParticle *particle);
void EfptSpawnRing(EffeParticle *particle);
void EfptSpawnFallShrink(EffeParticle *particle);
void GenericSpawnDrift(EffeParticle *particle);
void GenericSpawnFall(EffeParticle *particle);
void GenericSpawnCurve(EffeParticle *particle);
void GenericSpawnAttract(EffeParticle *particle);
void SakuraSpawnParticle(EffeParticle *particle);

// The spawn callback each family's creator installs in the system; subtypes outside a table keep the template no-op.
inline constexpr std::array<EffeParticleUpdate, 5> kSpecialSpawns{Special0Spawn, Special1Spawn, Special2Spawn, Special3Spawn, Special4Spawn};

inline constexpr std::array<EffeParticleUpdate, 4> kEfptSpawns{EfptSpawnFall, EfptSpawnFallFade, EfptSpawnRing, EfptSpawnFallShrink};

inline constexpr std::array<EffeParticleUpdate, 4> kGenericSpawns{GenericSpawnDrift, GenericSpawnFall, GenericSpawnCurve, GenericSpawnAttract};

extern EffeRuntimeTexture *g_effeCurrentSystem;
void EFFE_NoOpParticle(EffeParticle *);
void EFFE_FillParticleTemplate(EffeParticle *particles, std::int32_t count);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

std::int32_t AllocateEffeTask(EffeFamily family, std::int32_t count, EffeRegistryCallback callback);
void SyncEffeTask(EffeFamily family, const EffeTaskRecord &task);
void SetEffeActive(EffeTaskRecord &task, bool active);
void FreeEffeTask(EffeFamily family, std::int32_t slot);
std::uint64_t ParticleSpawnThreshold(float probability);
void EFFE_RetireParticle(EffeParticle *particle);
float EffeFoldedCosine(std::int64_t angle);
void EffeSetAlpha(EffeParticle *particle, std::int32_t alpha);
float &EffeRadialX(EffeParticle *particle);
float &EffeRadialZ(EffeParticle *particle);
void SakuraGroundedParticle(EffeParticle *particle);
void SakuraFallingParticle(EffeParticle *particle);
void EfptFall(EffeParticle *particle);
void EfptFallParticle(EffeParticle *particle);
void EfptFallFadeParticle(EffeParticle *particle);
void EfptFallShrinkParticle(EffeParticle *particle);
void EffeGenericPhases(EffeParticle *particle);
void GenericDriftParticle(EffeParticle *particle);
double GenericFallCosine(std::int64_t phase);
void GenericFallParticle(EffeParticle *particle);
Vector4 GenericBezier(const Vector4 &p0, const Vector4 &p1, const Vector4 &p2, const Vector4 &p3, float t);
void GenericCurveParticle(EffeParticle *particle);
void GenericAttractParticle(EffeParticle *particle);
void EffeSpecialIntegrate(EffeParticle *particle);
void Special0FadeParticle(EffeParticle *particle);
void Special0RiseParticle(EffeParticle *particle);
void Special1DriftParticle(EffeParticle *particle);
void Special2FadeParticle(EffeParticle *particle);
void Special2SwirlParticle(EffeParticle *particle);
void EffeSpecialRadialMove(EffeParticle *particle, std::size_t slot);
void EffeSpecialRadialFade(EffeParticle *particle, std::size_t slot);
void EffeSpecialRadialRise(EffeParticle *particle, std::size_t slot, EffeParticleUpdate fade);
void EffeSpecialRadialSpawn(EffeParticle *particle, std::size_t slot, EffeParticleUpdate rise);
void Special3FadeParticle(EffeParticle *particle);
void Special3RiseParticle(EffeParticle *particle);
void Special4FadeParticle(EffeParticle *particle);
void Special4RiseParticle(EffeParticle *particle);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

float EffeDot(const Vector3 &left, const Vector3 &right);
Vector3 NormalizeEffeVector(const Vector3 &value, const Vector3 &fallback);
EffeProjectionBasis BuildEffeProjectionBasis();
void UpdateRainParticleNative(RainParticleNative *particle, WeatherNodeData *data);
float FlushWeatherEpsilon(float value);
bool WeatherPointInsideClipQuad(const WeatherClipBounds &bounds, float x, float y, float z);
void UpdateCaraRainParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void UpdateVolumeRainParticleNative(RainParticleNative *particle, WeatherNodeData *data);
void BeginRippleCycle(RippleParticleNative &ripple, const WeatherNodeData *group);
void RestartRippleCycle(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction respawn);
bool ProbeRippleImpact(RippleParticleNative &ripple, WeatherNodeData *group);
void EvaluateRainImpactLocation(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction update);
void SpawnRippleAtRandom(RippleParticleNative &ripple, WeatherNodeData *group, float spanX, float spanZ, RainRandomFunction update);
void ReprobeRippleImpact(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction spawn, RainRandomFunction update);
bool AdvanceBucketRipple(RippleParticleNative &ripple, WeatherNodeData *group, RainRandomFunction reprobe);
bool RippleInsideClipEdges(const WeatherClipBounds &bounds, float x, float z);
void RainCallback(RainParticleNative *record, WeatherNodeData *group);
void EvaluateRainImpactLocation_0(RainParticleNative *record, WeatherNodeData *group);
void EvaluateRainImpactLocation_2(RainParticleNative *record, WeatherNodeData *group);
void SpawnRandomRainImpactCandidate(RainParticleNative *record, WeatherNodeData *group);
void UpdateClippedRainImpact(RainParticleNative *record, WeatherNodeData *group);
void Rain_CalcPRNGWithCallback(RainParticleNative *record, WeatherNodeData *group);
void EvaluateRainImpactLocation_1(RainParticleNative *record, WeatherNodeData *group);
void EvaluateRainImpactLocation_3(RainParticleNative *record, WeatherNodeData *group);
void AssignHamonTex2D(WeatherNodeData &data);
void SetCallbackFunc_AssignHamonTex2D(WeatherNodeData &data, RainRandomFunction callback);
void InitializeRippleLayer(WeatherNodeData &data, float intensity, RainRandomFunction callback);
void BuildCaraRippleLayer(WeatherCaraData &data, float intensity, std::size_t bucketBytes, bool clipSentinels, RainRandomFunction callback);
void UpdateRainDrift(WeatherNodeData *group, const Vector3 &forces);
void ApplyDampedCameraPushXZ(WeatherNodeData *group, const Vector3 &forces);
std::int16_t FoldWeatherAngle(std::int32_t angle);
void Bucket_UpdateOrientationFromCameraDelta(WeatherNodeData *group, const Vector3 &previousTarget, const Vector3 &target);
void SetRainParametersFromIntensity(WeatherNodeData *group, float intensity, std::int32_t layer);
void Snow_LUTBlend_ParamsFromIntensity(WeatherNodeData *group, float intensity);
extern float g_weatherSnowBudget;
void Snow_ClampActiveCountByBudget(WeatherNodeData *group, std::int32_t kind, std::int32_t layer);
void PublishWeatherRenderCount(WeatherParticlePool &pool);
std::int32_t ClampWeatherTarget(WeatherParticlePool &pool);
void ResetRainGroupParticles(WeatherNodeData *group, RainRandomFunction generator, float divisor);
void Rain_UpdateBucket(WeatherNodeData *group, const Vector3 &previousTarget, const Vector3 &target, const Vector3 &forces, std::int32_t layer, bool caraRain);
void UpdateWeatherGroupCullBox(WeatherNodeData *group, const Vector3 &direction);
void UpdateWeatherSnowCenter(WeatherNodeData *group, const Vector3 &direction);
void Weather_UpdateContainmentVolumes(WeatherNodeData *group, std::int32_t layer);
void ShiftWeatherSnowReflectionCenter(WeatherNodeData *group);
void RetireSnowParticles(WeatherNodeData *group, RainRandomFunction liveCallback, RainRandomFunction spawnCallback);
void Snow_UpdateBucket(WeatherNodeData *group, const Vector3 &forces, std::int32_t layer, bool clipped);
void Weather_UpdateRippleTarget(WeatherNodeData *group);
void Rain_UpdateFromGameDataAndAdvanceTimers(WeatherNodeData *group);
void Weather_BuildCullBoxAndDispatch(WeatherCaraData &data, const Vector3 &forces);
void Weather_UpdateSnowParticles(WeatherCaraData &data, const Vector3 &forces);
void Weather_UpdateParticleForTypeFromTable(WeatherNodeData &data, const Vector3 &forces);
void Weather_DispatchParticlesForZone(WeatherFixc &weather);
Vector4 ApplyWeatherViewTransform(const Vector3 &point);
Vector4 ApplyWeatherTransform(const Vector4 &value);
void RotateWeatherQuad(std::int32_t angleX, std::int32_t angleY, std::int32_t angleZ);
bool WriteWeatherQuad(EffeParticleVertex *output, float halfWidth, float halfHeight, std::uint32_t colour);
std::uint32_t ConstructAndCommitVertsForDrawing_0(D3TDeviceContext *context, const WeatherDrawGroup &group);
std::uint32_t ConstructAndCommitVertsForDrawing_1(D3TDeviceContext *context, const WeatherDrawGroup &group);
std::uint32_t ConstructAndCommitVertsForDrawing_2(D3TDeviceContext *context, const WeatherDrawGroup &group);
void ApplyWeatherRecordCallbacks(const WeatherDrawGroup &group);
void SubmitWeatherParticleGroup(
    D3TDeviceContext *context, const WeatherDrawGroup &group, const EffeMaterialWords &groupMaterial, const std::uint32_t &groupTextureHandle);
Vector3 TransformEffePoint(const Vector3 &point);
Vector3 EffeWorldToView(const Vector3 &world, const EffeProjectionBasis &basis);
void SetEffeParticlesPaused(bool paused);
void RunEffeParticleCallbacks96(const EffeRuntimeTexture &runtime);
void ConstructParticleSystemElements(const EffeRuntimeTexture &runtime);
void ClearVertsForDrawing(D3TDeviceContext *context, const EffeRuntimeTexture &runtime);
void ClearVertsForDrawing_0(D3TDeviceContext *context, const EffeRuntimeTexture &runtime);
Vector4 TransformEffePoint4(const Vector3 &point);

std::uint32_t DrawEffeParticleQuads(
    D3TDeviceContext *context, EffeRuntimeTexture &runtime, std::uint32_t selector, const EffeMaterialWords &material, std::uint32_t texture);

void SubmitAlphaQuads(std::size_t slot);
void EFFE_SubmitForDraw(void *rawIndex);
void EFFE_Update();
EffeTaskRecord *SakuraTaskAt(std::int32_t slot);
void FreeSakuraTask(EffeTaskRecord *task);
void SetSakuraActive(EffeTaskRecord &task, bool active);
void SetSakuraPositionRange(EffeTaskRecord &task, const Vector3 &position, const Vector3 &range);
void SetSakuraVelocityRange(EffeTaskRecord &task, const Vector3 &velocity, const Vector3 &range);
void SetSakuraProbability(EffeTaskRecord &task, float probability);
void SetSakuraDimensions(EffeTaskRecord &task, float width, float height);
void TranslateSakuraParticles(EffeTaskRecord &task, const Vector3 &amount);
std::array<std::uint8_t, 12> ReadSakuraControls();
void UpdateSakuraOwner(void *argument);
void EffectTaskCallback(HLib::TaskData *);
void SakuraTaskCallback(HLib::TaskData *);
void EnsureEffeUpdateTask();
std::int32_t CreateSpecialEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags, std::int32_t subtype);
std::int32_t CreateSakuraEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags);

std::int32_t CreateEfptEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags, float floor,
    const char *root, const char *filename);

std::int32_t CreateGenericEffe(const Vector3 &position, const Vector3 &range, std::int32_t count, float probability, std::uint32_t flags, float floor,
    const char *root, const char *filename);

void Snow_TickSystemsInit(float intensity);
void SetEffePositionRange(EffeTaskRecord &task, const Vector3 &position, const Vector3 &range);
void SetEffeVelocityRange(EffeTaskRecord &task, const Vector3 &velocity, const Vector3 &range);
void SetEffeDimensions(EffeTaskRecord &task, float width, float height);
void SetEffeProbability(EffeTaskRecord &task, float probability);
void TranslateEffe(EffeTaskRecord &task, const Vector3 &amount);
void LoadSakuraResource();
float EffectParameterFloat(const std::uint32_t *parameters, std::size_t index);
void SetEffectResult(ScenarioContext *context, std::int32_t value);
extern std::byte *g_effectTransformContexts;
extern void *g_effectAuxiliaryEntries;
extern std::uint32_t *g_effectAuxiliaryOrder;
extern std::int32_t g_effectTransformContextCapacity;
extern std::int32_t g_effectAuxiliaryCapacity;
extern std::int32_t g_effectTransformContextCount;
extern std::int32_t g_effectAuxiliaryCount;
extern std::int32_t g_effectObjectPassCount;
extern std::int32_t g_effectCallbackCount;
extern std::int32_t g_effectRunCount;
extern Matrix4x4 g_effectTransform;
extern std::uint64_t g_effectReserved;
extern std::uint32_t g_effectPackedColour;
extern std::int32_t g_effectBuffersEnabled;
extern std::int32_t g_effectSortEnabled;
extern float g_effectDefaultSortKey;
std::size_t SaturatedMultiply(std::size_t first, std::size_t second);
void *AllocateEffectBytes(std::size_t size);
void ReleaseOwnedEffectBuffers();
std::int64_t InitializeEffectBufferStorage(const EffectBufferConfiguration &configuration);
EffectQueueEntry *allocateEffectQueueEntry(EffectQueueEntryKind kind, float sortKey);
void ResetRuntimeEffectTablesNative();
void ResetEffectRegistryTablesNative();
void InitEffectTablesNative();
void EnsureEffeOwnerNative();
void ResetPrivateSceneEffectGlobalsNative();
std::uint8_t VFFightGroundEffectHourBand();
std::uint32_t VFFightGroundEffectColour(const VFActor *actor);
Vector3 VFFightGroundEffectPosition(std::uint8_t type, VFActor *actor, Character *character);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void ApplyWccpClipQuad(WeatherClipBounds *bounds, const Vector2 (&corners)[4], float minimumY, float maximumY);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void Weather_ApplySnowGameDataForWeather(const WeatherFixc &weather, float intensity, std::int32_t stepCount);

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void Weather_SetRainParticleSize(const WeatherFixc &weather, float width, float height);
void Weather_SetRainEffectVectors(const WeatherCaraData &data, const Vector3 &vector0, const Vector3 &vector1);
void Weather_SetSnowEffectVectors(const WeatherCaraData &data, const Vector3 &vector0, const Vector3 &vector1);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void ResetEffeRuntimeForScene();

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

extern std::array<std::int32_t, 4> g_effectValidationOrder;
extern std::array<float, 4> g_effectValidationMatrices;
extern std::int32_t g_effectValidationCount;

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

extern float g_inverseGeometryScale;
void CommitOrCombineGeometryBuffers(EffeGeometryChunk *chunk, std::int32_t renderPass);
void CombineGeometryBuffersIgnoredAlpha_RenderPass0(EffeGeometryChunk *chunk);
void CommitOrCombineGeometryBuffers_RenderPass1(EffeGeometryChunk *chunk);
void CommitOrCombineGeometryBuffers_RenderPass2(EffeGeometryChunk *chunk);
void GeometryChunkNoOp(EffeGeometryChunk *);
void SetInverseGeometryScale(EffeGeometryChunk *chunk);

inline constexpr std::array<void (*)(EffeGeometryChunk *), 8> kGeometryChunkFunctions{
    CombineGeometryBuffersIgnoredAlpha_RenderPass0,
    CommitOrCombineGeometryBuffers_RenderPass1,
    CommitOrCombineGeometryBuffers_RenderPass2,
    CombineGeometryBuffersIgnoredAlpha_RenderPass0,
    CommitOrCombineGeometryBuffers_RenderPass1,
    CommitOrCombineGeometryBuffers_RenderPass2,
    GeometryChunkNoOp,
    SetInverseGeometryScale,
};
void ConstructFullXFormsForEntity(void *argument);

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void resetEffectQueueOrder();
void sortEffectQueueOrder();

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {} // namespace shenmue::s1
