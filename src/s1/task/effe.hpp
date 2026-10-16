#pragma once

#include "s1/matrix_stack.hpp"
#include "s1/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct ScenarioContext;
struct VFActor;

enum class EffectQueueEntryKind : std::int32_t {
    Empty = 0,
    ObjectPass = 2,
    CommandStream = 5,
    Callback = 6,
    Run = 7,
};

struct EffectQueueEntry {
    Matrix4x4 *instance;
    EffectQueueEntryKind kind;
    std::uint32_t reserved_0c;
    void *dispatchFunction;
    void *dispatchArgument;
    std::uint64_t field_20;
    std::uint64_t field_28;
    void *sceneObjectOrCommandStream;
    std::uint32_t renderFlags;
    float alphaScale;
    void *savedSceneReference;
};

static_assert(sizeof(EffectQueueEntry) == 0x48);
static_assert(offsetof(EffectQueueEntry, sceneObjectOrCommandStream) == 0x30);

#pragma pack(push, 4)
struct GLBSWeatherDrawState {
    std::uint64_t next;
    std::uint64_t backlink;
    std::uint32_t drawFlags10;
    std::byte reserved_14[0x88];
    std::uint32_t drawFlags9c;
    std::byte reserved_a0[0x70];
    std::uint32_t drawFlags110;
    std::byte reserved_114[0x84];
    float volume;
    float maximumY;
    float minimumY;
    float maximumX;
    float minimumX;
    float maximumZ;
    float minimumZ;
    float dimensionX;
    float dimensionY;
    float dimensionZ;
    std::byte reserved1c0[0xa0];
};
#pragma pack(pop)

static_assert(sizeof(GLBSWeatherDrawState) == 0x260);
static_assert(offsetof(GLBSWeatherDrawState, drawFlags10) == 0x10);
static_assert(offsetof(GLBSWeatherDrawState, drawFlags9c) == 0x9c);
static_assert(offsetof(GLBSWeatherDrawState, drawFlags110) == 0x110);
static_assert(offsetof(GLBSWeatherDrawState, volume) == 0x198);


struct EffeGeometryChunk {
    std::int32_t typeAndSize;
    std::uint32_t textureWords[4];
    std::int32_t stripCount;
};

static_assert(sizeof(EffeGeometryChunk) == 0x18);


struct EffeGeometryMesh {
    std::int32_t reserved00;
    std::int32_t vertexOffset;
    std::int32_t reserved08;
    std::int32_t chunkOffset;
    Vector3 centre;
    float radius;
};

static_assert(sizeof(EffeGeometryMesh) == 0x20);
static_assert(offsetof(EffeGeometryMesh, chunkOffset) == 0x0c);
static_assert(offsetof(EffeGeometryMesh, radius) == 0x1c);

struct EffeParticle;
using EffeParticleUpdate = void (*)(EffeParticle *particle);

// The 0x60 record every EFFE particle system walks. update is the particle's own callback: sh4_parts_draw while idle, then the system's
// spawn callback and the family's motion callbacks. attribute holds an age, a radial speed or (EFPT) the task floor pointer; scale sizes the
// quad.
struct EffeParticle {
    std::uint32_t flags;
    Vector3 position;
    Vector3 velocity;
    std::uint32_t reserved1c;
    EffeParticleUpdate update;
    std::int64_t phase0;
    std::int64_t phase1;
    std::int64_t phase2;
    std::int64_t attribute;
    std::int64_t phaseStep0;
    std::int64_t phaseStep1;
    std::uint32_t colour;
    float scale;
};

static_assert(sizeof(EffeParticle) == 0x60);
static_assert(offsetof(EffeParticle, update) == 0x20);
static_assert(offsetof(EffeParticle, phase0) == 0x28);
static_assert(offsetof(EffeParticle, attribute) == 0x40);
static_assert(offsetof(EffeParticle, phaseStep0) == 0x48);
static_assert(offsetof(EffeParticle, colour) == 0x58);
static_assert(offsetof(EffeParticle, scale) == 0x5c);

// The idle particle's roll: on success it takes the current system's spawn callback, a position/velocity in the system ranges and colour.
void sh4_parts_draw(EffeParticle *particle);

using EffeRegistryCallback = void (*)(void *argument);

// EFFE_Related record. One layout serves the TRWV texture (4) and terrain (2) records, the FFT records (2) and EFFE::GlobalState (16).
// state: 0 free, 1 allocated, 2 active. fftIndex >= 0 makes EFFE_Callback_0 re-upload the emitter's texture from its FFT output.
struct EffeRegistryEntry {
    std::int32_t reserved00;
    std::int32_t reserved04;
    std::int32_t fftIndex;
    std::int32_t state;
    EffeRegistryCallback callback;
    void *argument;
    std::uint32_t token;
    std::uint32_t reserved24;
    std::uint64_t reserved28;
};

static_assert(sizeof(EffeRegistryEntry) == 0x30);
static_assert(offsetof(EffeRegistryEntry, fftIndex) == 0x08);
static_assert(offsetof(EffeRegistryEntry, state) == 0x0c);
static_assert(offsetof(EffeRegistryEntry, callback) == 0x10);
static_assert(offsetof(EffeRegistryEntry, argument) == 0x18);
static_assert(offsetof(EffeRegistryEntry, token) == 0x20);
static_assert(offsetof(EffeRegistryEntry, reserved28) == 0x28);

// EFFE_Update's per-emitter transform: translate by position, rotate Z, Y then X, translate by offset.
struct EffeEmitterTransform {
    std::int32_t enabled;
    Vector3 position;
    Vector3 offset;
    std::int32_t angleX;
    std::int32_t angleY;
    std::int32_t angleZ;
};

static_assert(sizeof(EffeEmitterTransform) == 0x28);
static_assert(offsetof(EffeEmitterTransform, angleX) == 0x1c);

inline constexpr std::uint32_t kEffeEmptyRegistryToken = 0x20202020U;


extern std::int32_t g_shouldLoadWeatherSprites;
extern std::array<EffeRegistryEntry, 16> g_effeEmitterRegistry;
extern std::array<EffeRegistryEntry, 2> g_effeFftRegistry;
extern std::array<EffeRegistryEntry, 4> g_effeTextureRegistry;
extern std::array<EffeRegistryEntry, 2> g_effeTerrainRegistry;
extern std::array<EffeEmitterTransform, 16> g_effeEmitterTransforms;

std::int32_t EFFE_Related(std::int32_t table, std::int32_t fftIndex, EffeRegistryCallback callback, void *argument, std::uint32_t token);
void EFFE_ReleaseRegistryEntry(EffeRegistryEntry &entry);

// InitializeFFTData's record: the 16x16 smoke spectrum and its ARGB output (a 0x12c0 'PUFY' block; S2's smoke FFT record shares the layout).
struct EffeFftData {
    std::uint32_t flags;
    std::uint32_t reserved04;
    std::uint32_t *output;
    std::uint32_t packedColour;
    float phaseParameter;
    float outputScale;
    float phaseRateScale;
    std::array<float, 64> falloff;
    std::array<std::byte, 0x40> reserved120;
    std::array<float, 256> phases;
    std::array<float, 256> phaseRates;
    std::array<float, 289> real;
    std::array<float, 289> imaginary;
};

static_assert(sizeof(EffeFftData) == 0x1268);
static_assert(offsetof(EffeFftData, packedColour) == 0x10);
static_assert(offsetof(EffeFftData, falloff) == 0x20);
static_assert(offsetof(EffeFftData, phases) == 0x160);
static_assert(offsetof(EffeFftData, phaseRates) == 0x560);
static_assert(offsetof(EffeFftData, real) == 0x960);
static_assert(offsetof(EffeFftData, imaginary) == 0xde4);

// EnqueueSpecialEffect's per-group FFT owner: the FFT record, the output block and the special tasks using it.
struct EffeFftOwner {
    std::int32_t fftIndex;
    std::uint32_t reserved04;
    std::byte *pixels;
    std::uint32_t colour;
    std::uint16_t references;
    std::uint16_t activeMask;
};

static_assert(sizeof(EffeFftOwner) == 0x18);
static_assert(offsetof(EffeFftOwner, references) == 0x14);

// The EFFE task record behind every script effect family. EnqueueSpecialEffect, EnqueueEFPT, EnqueueEFFE and EnqueueEFFE_0 (Sakura) each
// take a 0xC0-byte HLib heap block inline: the 0x40 header and this record. The shared effect setters address it for every family.
struct EffeTaskRecord {
    std::int32_t runtimeSlot;
    std::int32_t particleCount;
    EffeParticle *particles;
    Vector3 position;
    Vector3 range;
    Vector3 velocity;
    Vector3 velocityRange;
    std::uint32_t flags;
    float width;
    float height;
    float probability;
    float particleScale;
    float decayScale;
    float decayStep;
    std::byte reserved5c[12];
    EffeFftOwner *fftOwner;
    std::int32_t subtype;
    float floor;
    std::uint64_t reserved78;
};

static_assert(sizeof(EffeTaskRecord) == 0x80);
static_assert(offsetof(EffeTaskRecord, flags) == 0x40);
static_assert(offsetof(EffeTaskRecord, probability) == 0x4c);
static_assert(offsetof(EffeTaskRecord, particleScale) == 0x50);
static_assert(offsetof(EffeTaskRecord, fftOwner) == 0x68);
static_assert(offsetof(EffeTaskRecord, subtype) == 0x70);
static_assert(offsetof(EffeTaskRecord, floor) == 0x74);

struct EffeTextureResource;


struct EffeRuntimeTexture {
    std::uint32_t flags;
    std::int32_t particleCount;
    void *particles;
    std::uint32_t descriptor[4];
    EffeParticleUpdate particleUpdate;
    std::int32_t spawnProbabilityQ15;
    Vector3 positionMin;
    Vector3 velocityMin;
    Vector3 positionRange;
    Vector3 velocityRange;
    float halfWidth;
    float halfHeight;
    std::uint32_t colour;
    EffeTextureResource *resource;
    const void *pvrSource;
};

static_assert(sizeof(EffeRuntimeTexture) == 0x78);
static_assert(offsetof(EffeRuntimeTexture, particleUpdate) == 0x20);
static_assert(offsetof(EffeRuntimeTexture, resource) == 0x68);

extern std::array<EffeRuntimeTexture, 16> g_effeRuntime;



void EFFE_InitializeParticleSystem(std::int32_t slot, std::int32_t count, EffeParticle *particles);

// InitAndCreateTexture2D over a PVRT header, as the creators leave the descriptor words: spectrum selects word 2 with 0x2000 set and 0x4000
// clear (special effects and the TRWV bow spray).
void EFFE_AttachPvrTexture(std::int32_t slot, const void *pvrt, const void *source, bool spectrum, std::uint32_t width, std::uint32_t height);

extern std::array<EffeFftData *, 2> g_effeFftData;
extern std::array<EffeFftOwner *, 16> g_effeFftOwners;
void InitializeFFTData(EffeFftData *fft);
void ProcessAndQuantizeFFTOutput(EffeFftData *fft);

struct WeatherNodeData;

extern WeatherNodeData *g_weatherNodeHead;

extern EffectQueueEntry *g_effectQueueEntries;
extern std::int32_t g_effectQueueEntryCount;
extern std::int32_t g_effectQueueEntryCapacity;
extern std::uint32_t *g_effectQueueOrder;
extern std::uint32_t *g_effectQueueOrderScratch;
extern float *g_effectQueueSortKeys;
extern bool g_replayingEffectQueue;
extern HLib::Task *g_effeTask;
extern std::int32_t g_effectsTaskState;
extern std::int32_t g_effePauseState;
extern GLBSWeatherDrawState *g_pGLBSTaskData;
struct EffeParticleDiagnostics {
    std::uint32_t allocatedTasks;
    std::uint32_t activeTasks;
    std::uint32_t activeGenericTasks;
    std::uint32_t spawnedParticles;
    std::uint32_t texturedTasks;
    std::uint32_t submittedParticleSystems;
    std::uint32_t submittedParticles;
    bool renderValidationSubmitted;
};

using EffectQueueCallback = void (*)(void *argument);

HLib::Task *EFFE_Init();
void EFFE_Destroy();
void EFFE_Callback(HLib::TaskData *taskData);
void EFFE_ResetSceneStateNative();
void EFFE_UpdateWeatherDrawBits(std::int32_t shouldDraw);
void EFFE_UpdateWeatherState();
void AllocFIXC();
void Weather_SetForcedRainValue(float intensity);
void EFFE_ApplyRainGameData(float intensity, std::int32_t stepCount);
void Weather_SetForcedSnowValue(float intensity);
void EFFE_ApplySnowGameData(float intensity, std::int32_t stepCount);
void EFFE_ApplyWindGameData(float intensity, std::int32_t stepCount);
void EFFE_InitializeSnowWeatherSystems(float intensity);
void EFFE_ProcessWeather(float intensity);
// WeatherUtils mode 20 and AllocFIXC: allocate the ripple (hamon) pools of every attached FIXC.
void Weather_TickPerSystemValues(float intensity);
// Shenmue_InitSpriteBank consumers for /MISC/TEXTURES w_snow.pvr and w_hamon.pvr.
void CreateDDSorPVR(const void *file);
void EFFE_CreateHamonTexture(const void *file);
void EFFE_FreeRainRenderData();
void EFFE_SetRainParticleSize(float width, float height);
void EFFE_FreeSnowRenderData();
void EFFE_FreeRippleRenderData();
void EFFE_SetSnowParticleSize(float size);
void EFFE_SetRippleParticleSize(float size);
void EFFE_SetRainEffectVectors(const Vector3 &vector0, const Vector3 &vector1);
void EFFE_SetSnowEffectVectors(const Vector3 &vector0, const Vector3 &vector1);
void EFFE_ApplyWccpClipQuad(std::uint32_t setIndex, std::uint32_t groupMask, const Vector2 (&corners)[4], float minimumY, float maximumY);
void SetRainEffectOffset(std::uint32_t colour);
void EFFE_AttachWindData(void *windData);
void EFFE_DetachWindData(void *windData);
void EFFE_ResetWccpClipSet(std::uint32_t setIndex);
void Weather_UpdateRainOffset(std::uint32_t colour);
void Weather_UpdateColourRamp(std::uint32_t colour);
void EFFE_Callback2(void *sceneData);
void EFFE_ScriptWeatherUtilsForFIXC(ScenarioContext *context, void *rawParameters);
void EFFE_SetSnowParticleBudgetEnabled(bool enabled);
bool EFFE_IsSnowParticleBudgetEnabled();
void EFFE_InitVFFightGroundEffects();
void EFFE_FreeVFFightGroundEffects();
void EFFE_FadeVFFightGroundEffects();
void EFFE_SpawnVFFightGroundEffect(std::uint8_t type, VFActor *actor, Character *stageCharacter = nullptr);
std::int32_t EFFE_GetWeatherSpriteLoadPolicy();
void EFFE_AssignObjectPass(EffectQueueCallback callback, void *argument, float sortKey);
void DrawForWeather(EffeGeometryMesh *mesh);
void EFFE_AssignCallback(EffectQueueCallback callback, void *argument, float sortKey);
std::int64_t EFFE_Run(void *taskArgument, void *sceneData, float sortKey);

void EFFE_RemoveQueuedSceneObjectReferences(void *sceneObject);
std::int64_t InitEffectBuffers();
void SetupEffeTransform();
void EFFE_SetLightSlotEnabled(std::uint32_t lightIndex, std::uint32_t enabled);
void EFFE_SetLightSlotModeFlags(std::uint32_t lightIndex, std::uint16_t modeFlags);
std::uint32_t EFFE_GetEnabledStandardLightMask();
std::uint32_t EFFE_GetEnabledSpotRenderLightMask();
extern Vector3 g_effectSubmissionLightDirection;
void SubmitToEffectBuffer(void *commandStream, float alphaScale, std::uint32_t renderFlags, void *modelBase);
void EFFE_ReplayQueue(std::int32_t mode, std::uint32_t sceneFlagMask, EffectQueueEntryKind kind);
void DrawEffectABufferWritePasses();
void DrawEffectCommandStreamDepthPass();
void EFFE_DrawObjectPasses();
void EFFE_ReplayQueuedEffects(std::int32_t objectPasses);
void EFFE_FinishQueuedEffects();
void EFFE_DrawProjectedShadows();
extern Vector4 g_effeObjectLightColour;
void EFFE_SetScriptPosition(const Vector4 &position);
void EFFE_SetScriptWordParameter(std::uint32_t mode, std::uint32_t value);
void EFFE_SetScriptVector3Parameter(std::uint32_t mode, const Vector3 &value);
void EFFE_SetScriptVector4Parameter(std::uint32_t mode, const Vector4 &value);
void EFFE_SetScriptScalarParameter(std::uint32_t mode, float value);
void EFFE_ResetScriptLightParametersForMap();
extern std::uint32_t g_effectScriptWordParameters[2];
extern Vector4 g_effectScriptMaximumIntensity;
extern Vector3 g_effectScriptVectorParameters[2];
extern float g_effectScriptScalarParameters[3];

void Script_sakuraUtils(ScenarioContext *context, void *rawParameters);
void Script_InitSakuraEffectTasks_0(ScenarioContext *context, void *rawParameters);
void Script_EffectUtils(ScenarioContext *context, void *rawParameters);
void Script_EffectUtils_0(ScenarioContext *context, void *rawParameters);
void Script_SetEFPT(ScenarioContext *context, void *rawParameters);
void Script_effeInitTexture2D(ScenarioContext *context, void *rawParameters);

} // namespace shenmue::s1
