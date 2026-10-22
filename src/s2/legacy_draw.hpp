#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "common/math.hpp"

namespace shenmue::d3t {
struct D3TRenderState;
}

namespace shenmue::s2 {

enum class LegacyDrawCommandOpcode : std::uint8_t {
    End = 0,
    BindVertexStreams = 1,
    SelectVertexShader = 2,
    SelectPixelShader = 4,
    EngineState8 = 8,
    EngineState16 = 9,
    EngineState32 = 10,
    BindTextures = 11,
    AlphaState12 = 13,
    AlphaState8 = 14,
    DrawIndexed = 16,
    DrawIndexedOffset = 18,
    BindVertexStream = 19,
    SetRenderMode = 21,
    SetFogEnabled = 22,
};

struct LegacyDrawCommandHeader {
    std::uint32_t opcodeAndLength;

    LegacyDrawCommandOpcode Opcode() const {
        return static_cast<LegacyDrawCommandOpcode>(opcodeAndLength & 0xffU);
    }
    std::uint32_t LengthInDwords() const {
        return opcodeAndLength >> 8U;
    }
};

static_assert(sizeof(LegacyDrawCommandHeader) == 4);

struct LegacyDrawCommandStreamStats {
    std::uint32_t packetCount{};
    std::uint32_t commandCount{};
    std::uint32_t drawCount{};
    std::uint32_t textureBindCount{};
    std::uint32_t vertexStreamBindCount{};
    std::uint32_t unsupportedCommandCount{};
};

struct LegacyVertexBufferBinding {
    std::uint32_t metadata{};
    std::int32_t handle{};
};

struct LegacyIndexDraw {
    std::uint32_t commandOffset{};
    std::uint32_t primitive{};
    std::uint32_t indexCount{};
    std::uint32_t startIndex{};
};

struct LegacyTextureBinding {
    std::uint32_t commandOffset{};
    std::uint32_t recordIndex{};
    std::int32_t textureTableIndex{-1};
    std::uint32_t textureHandle{};
    const std::byte *runtimeHandle{};
};

struct LegacyBaseShaderParameters {
    Vector3 fogColour;
    std::uint32_t fogEnabled;
    Vector4 textureFactor;
    std::uint32_t alphaTestEnabled;
    float alphaReference;
    std::uint32_t alphaFunction;
    std::uint32_t alphaOperation;
    std::uint32_t alphaArgument0;
    std::uint32_t alphaArgument1;
    std::uint32_t alphaArgument2;
    std::uint32_t specularEnabled;
    std::uint32_t renderMode;
    float depthBias;
    std::uint32_t useEnvironmentMap;
    std::uint32_t disableReflections;
    Vector4 constants[3];
};

struct LegacyShaderParameters {
    LegacyBaseShaderParameters base;
    Vector4 constant0[5];
    Vector4 constant1[8];
    Vector4 constants[117];
};

static_assert(sizeof(LegacyBaseShaderParameters) == 0x80);
static_assert(sizeof(LegacyShaderParameters) == 0x8a0);
static_assert(offsetof(LegacyShaderParameters, constants) == 0x150);



struct LegacyVertexColourStateRecord {
    std::uint16_t enabled{};
    std::uint16_t kind{};
    float reserved04[4]{};
    Vector4 colour{};
    float reserved24[4]{};
    Vector3 position{};
    Vector3 direction{};
    Vector3 attenuation{};
    float coneCosine{};
    Vector3 transformedPosition{};
    Vector3 transformedDirection{};
    Vector3 modelPosition{};
    Vector3 modelDirection{};
    float reserved8c[3]{};
    float inverseRange{};
    std::uint32_t reserved9c{};
    float rangeSquared{};
    float inverseConeWidth{};
    float range{};
    float coneSine{};
    std::uint64_t reservedB0{};
};

static_assert(sizeof(LegacyVertexColourStateRecord) == 0xb8);
static_assert(offsetof(LegacyVertexColourStateRecord, colour) == 0x14);
static_assert(offsetof(LegacyVertexColourStateRecord, position) == 0x34);
static_assert(offsetof(LegacyVertexColourStateRecord, transformedPosition) == 0x5c);
static_assert(offsetof(LegacyVertexColourStateRecord, modelPosition) == 0x74);
static_assert(offsetof(LegacyVertexColourStateRecord, range) == 0xa8);






struct LegacyDrawResources {
    std::vector<LegacyVertexBufferBinding> vertexBuffers;
    std::vector<LegacyIndexDraw> indexDraws;
    std::vector<LegacyTextureBinding> textureBindings;
    std::int32_t indexBuffer{};
    bool prepared{};
    bool waveVertexColours{};
};

struct LegacyWaveVertex {
    float height{};
    std::uint32_t normal{};
    std::uint32_t diffuse{};
    std::uint32_t specular{};
};
bool UpdateLegacyWaveGeometry(std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources,
    std::span<const LegacyWaveVertex, 289> vertices, float scrollU, float scrollV);
bool ReadLegacyWaveGridPositions(std::span<const std::byte> bytes, std::uint32_t meshOffset, LegacyDrawResources &resources, std::span<Vector3, 289> positions);
bool UpdateLegacyWaveColours(
    std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources, std::uint32_t diffuse, std::uint32_t specular);
bool UpdateLegacyTextureUVs(std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources, float scrollU, float scrollV);



struct LegacyNodeCommandOverride {
    std::uint32_t nodeSourceOffset{};
    std::span<const std::byte> bytes;
    std::uint32_t xb01Offset{};
    const LegacyDrawResources *resources{};
};

enum class LegacyModelShaderPath : std::uint8_t {
    VertexColour,
    Lambert,
};





bool InspectLegacyDrawCommandStream(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, LegacyDrawCommandStreamStats &stats);

bool PrepareLegacyDrawResources(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, LegacyDrawResources &resources);
void ReleaseLegacyDrawResources(LegacyDrawResources &resources);
// Rebind an XMAZ command copy while retaining its already allocated vertex
// handles. Texture bindings come from the selected source road model.
bool RefreshLegacyDrawResources(
    std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &source, LegacyDrawResources &resources);
bool UploadLegacyVertexBuffer(LegacyDrawResources &resources, std::uint32_t index, std::span<const std::byte> bytes);



void InitializeLegacyDrawState();

void InitializeLegacyPostProcessingParameters();

void InitializeLegacyPostFxTransitionTexture();
void RenderLegacyPostFxTransition(float alpha);
void ReleaseLegacyPostProcessingTargets();

bool InitSystemMatrices();
bool GetLegacyRenderMask();
std::uint32_t GetLegacyPerspectiveProjection();
void SetProjectionOff();
bool GetLegacyBusyPreparingDraw();
bool SetLegacyBusyPreparingDraw(bool busy);
void shenSetCharaDrawFlags(std::uint32_t mode);
void shenSetRenderFlags();
void SetLegacyStencilReference(std::uint32_t reference);
void SetLegacyGlobalLighting(const Vector4 &colour);
Vector3 GetLegacyParticleLightColour();
Vector3 GetLegacyParticleLightDirection();
float GetLegacyParticleLightIntensity();
void SetLegacyFogEnabled(std::uint32_t enabled);
void SetLegacyFogMode(std::int32_t mode);
void SetLegacyFogColour(std::uint32_t colour);
void SetLegacyDepthFadeRange(float nearDistance, float farDistance);
void SetLegacyDepthFadeBlue(float intensity);

void SetLegacySavedAlphaOperation(std::uint32_t operation);
float GetTexUVW(std::uint32_t index);
std::int64_t SetTexUVW(std::uint32_t index, float width);
inline constexpr Vector4 LEGACY_MODEL_COLOUR_DEFAULT{1.0f, 0.2f, 0.2f, 0.2f};
void ExchangeLegacyModelColour(const Vector4 *colour, Vector4 *previous);
void PublishLegacyWeatherSpawnDrift(const Vector3 &drift);
struct LightRuntimeEntry;
void SetLegacyLight(std::uint32_t slot, LightRuntimeEntry &light);
void ResetLegacySelectedLight(std::uint32_t slot, std::int32_t activeMode, const LightRuntimeEntry &defaults);
void SetLegacySelectedLightKind(std::uint32_t slot, std::int32_t kind);
std::uint64_t GetLegacySelectedLightKind(std::uint32_t slot);
void SetLegacySelectedLightEnabled(std::uint32_t slot, bool enabled);
void SetLegacySelectedLightDirection(std::uint32_t slot, const Vector3 &direction);
void SetLegacySelectedLightColour(std::uint32_t slot, const Vector4 &colour);
std::uint32_t GetAvailableSelectedLightSlot();
void SetSelectedLightIntensity(std::uint32_t slot, float intensity);
void shenSetLightPosition(std::uint32_t slot, const Vector3 *position);
void shenSetLightScale2(std::uint32_t slot, float range);
void shenSetLightScale(std::uint32_t slot, float falloff);
Vector3 GetLegacyLightDirection(std::uint32_t slot);
void UpdateLegacyLightTransforms();
void SetLegacyModelScale(float scale);
float GetLegacyModelScale();
void NoteXb01Flag4Mesh();
bool SphereVisCheck(const Vector3 &center, float scale, float clipRange);
void SetupLegacyScreenMatrix(float left, float top, float width, float height);
void CacheWorldScreenProjection();
std::uint64_t ProjectCachedWorldPosition(const Vector3 *position, Vector2 *output);
std::uint64_t ProjectWorldToScreen(const Vector3 *position, Vector2 *output);
void SetShouldDomeRenderTo(std::int32_t enabled);
void shenPushScreenOutputMatrixAndMultiply();
bool GetModelCheck(const Vector3 *centre, float radius);
LegacyModelShaderPath GetLegacyCharacterShaderPath();


void SetLegacyLambertFvfReference(const Vector3 *direction);
void SetLegacyVertexColourType(std::int32_t type);
std::int32_t GetLegacyVertexColourType();
bool GetLegacyVertexColourStateEnabled(std::uint32_t slot);
void SetLegacyVertexColourStateEnabled(std::uint32_t slot, bool enabled);



bool AccumulateProjectedModelMinimumHeight(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, float &minimumHeight);




void SetLegacyProjectedShadowDirectionAndScale(const Vector3 &direction, float scale);
std::uint32_t SetLegacyProjectedShadowStencilPartition(std::uint32_t partition);
bool BeginLegacyProjectedShadowPass();
std::uint32_t ExecuteProjectedShadowCommandStream(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources);
void EndLegacyProjectedShadowPass();

void BeginLegacyCharacterShadowFrame();
bool SuspendLegacyCharacterShadowFrame();
void ResumeLegacyCharacterShadowFrame(bool suspended);
void EndLegacyCharacterShadowFrame();
void SetLegacyDisableReflections(bool disabled);
void SetLegacyMirroredDrawMode(std::uint32_t enabled);
std::uint32_t SetLegacyNavigationRenderMode(std::int32_t mode);
void SetLegacyColourWriteMask(std::uint32_t mask);
void PerformCommonShaderPass(std::uint32_t colour);





std::uint32_t ExecuteLegacyDrawCommandStream(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources,
    LegacyModelShaderPath shaderPath = LegacyModelShaderPath::Lambert, float alpha = 1.0f);


struct LegacyMaterialCommand {
    std::uint32_t header, renderFlags, blendFlags, reserved0c[3];
    Vector3 sortPosition;
    std::uint32_t reserved24[2];
    std::int8_t alphaMode;
    std::uint8_t depthBias;
    std::uint16_t reserved2e;
    float colourScale, colourAlpha;
    Vector3 colour;
    float alphaSlot6;
    std::uint32_t reserved48[3];
    std::int32_t subCommandBytes;
};
static_assert(sizeof(LegacyMaterialCommand) == 0x58);
static_assert(offsetof(LegacyMaterialCommand, sortPosition) == 0x18);
static_assert(offsetof(LegacyMaterialCommand, alphaMode) == 0x2c);
static_assert(offsetof(LegacyMaterialCommand, colourScale) == 0x30);
static_assert(offsetof(LegacyMaterialCommand, alphaSlot6) == 0x44);
static_assert(offsetof(LegacyMaterialCommand, subCommandBytes) == 0x54);
inline constexpr std::array<std::uint32_t, 6> LEGACY_MATERIAL_ALPHA_ARGUMENTS{1, 3, 0, 0, 2, 3};
inline constexpr std::array<std::uint32_t, 4> LEGACY_MATERIAL_CULL_ARGUMENTS{0, 0, 2305, 2304};
inline constexpr std::array<std::uint32_t, 8> LEGACY_MATERIAL_PRIMITIVE_ARGUMENTS{0, 0, 5, 0, 6, 0, 0, 0};
inline constexpr std::array<std::uint32_t, 4> LEGACY_MATERIAL_BLEND_ARGUMENTS{1, 0, 1, 1};
inline constexpr std::array<std::uint32_t, 8> LEGACY_MATERIAL_SOURCE_BLEND_ARGUMENTS{0, 1, 774, 775, 770, 771, 772, 773};
inline constexpr std::array<std::uint32_t, 8> LEGACY_MATERIAL_DEPTH_ARGUMENTS{512, 516, 514, 518, 513, 517, 515, 519};
inline constexpr std::array<std::uint32_t, 8> LEGACY_MATERIAL_DESTINATION_BLEND_ARGUMENTS{0, 1, 768, 769, 770, 771, 772, 773};



std::uint32_t SubmitLegacyFlagDebugQuadCompatibility(std::span<const std::byte> nativeModel);

std::uint32_t ModelFlagDispatch_SetModelFlagDataOpaque(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources);
std::uint32_t ModelFlagDispatch_ReadModelDataOpaque(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources);
std::uint32_t ReadModelDataWithAlpha(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, float alpha);
std::uint32_t SetModelFlagDataWithAlpha(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, float alpha);

using LegacyUpdateCallback = void (*)(void *data);
void SetUpdateCallback(LegacyUpdateCallback callback, void *data, float priority);



std::uint32_t RenderQueuedLegacyShaderPasses();
void BeginAndEndLegacyModelQuery(const std::byte *model);
void AssignGeoLayout(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, std::int32_t flags);
std::uint32_t GetTextureMappedData();
std::uint32_t ComputeResolutionScaledIntensity();
std::uint32_t FlushQueuedLegacyDrawPasses();

std::uint32_t RenderQueuedLegacyDrawPasses();
void SDTransparentHack();
void ResetVertCount();
void RenderQueuedLegacyDrawPassesOnly();
std::int64_t ResetLegacyRenderQueue();
bool GetLegacyRenderQueueEnabled();
void SetLegacyRenderQueueEnabled(bool enabled);
void SetLegacyQueuedDrawDepthMode(std::int32_t fromGeometry, float fallbackDepth);

struct LegacyEffectGpuFlags {

    std::uint32_t textureFlags{};
    std::uint32_t samplerControl{};
    std::uint32_t gpuFlags{};
};


void SetEngineFlag(std::uint32_t index, std::uint32_t setting);
void RestoreEngineFlag(std::uint32_t index);
void SetAlphaFlags(std::uint32_t stage, std::uint32_t setting, std::uint32_t value);
void InitShaderStates();
void InitShenmueRendering();
void SetLegacyParticleRenderPass(bool translucent, bool compact = false);
void RenderLegacyWeatherQuads(std::int32_t count);
void UpdateShaderParams();
#if defined(_WIN32)
void ApplyLegacyEffectRenderModes(d3t::D3TRenderState &state, const LegacyEffectGpuFlags &flags, std::uint32_t texture);
void RestoreLegacyEffectRenderFlags(d3t::D3TRenderState &state);
#endif

struct HikiModelEntity;
struct LegacyEffectVertex {
    Vector3 position;
    std::uint32_t diffuse;
    std::uint32_t specular;
    float v;
    float u;
};
static_assert(sizeof(LegacyEffectVertex) == 28);
static_assert(offsetof(LegacyEffectVertex, diffuse) == 12);
static_assert(offsetof(LegacyEffectVertex, v) == 20);
void DecodeLegacyEffectStrip(const std::byte *&stream, std::span<LegacyEffectVertex> output, float uvScale);
void DrawLegacyEffectEntity(HikiModelEntity &entity);

struct LegacyFireQuad {
    Matrix4x4 worldTransform{};
    float width{};
    float height{};
    float alpha{1.0f};
    // FireFftData graphics records +0x20 and +0x50 contain these handles.
    std::array<std::uint32_t, 2> textures{};

    std::uint32_t frameTick{};
};

bool BeginSmokeDraw(std::uint32_t count, std::uint32_t emitterFlags, std::uint32_t texture, const LegacyEffectGpuFlags &gpuFlags, const Matrix4x4 &projection);
bool AppendSmokeBillboard(
    const Matrix4x4 &worldTransform, const Vector3 &position, const Vector3i &rotation, bool xyz, float width, float height, std::uint32_t colour);
void EndSmokeDraw();




bool QueueFireQuad(const LegacyFireQuad &quad);

struct LegacyStripType3Vertex {
    Vector3 position;
    float u;
    float v;
    std::uint32_t diffuse;
    std::uint32_t specular;
};

static_assert(sizeof(LegacyStripType3Vertex) == 0x1c);
struct LegacyStripType0Vertex {
    Vector3 position;
    std::uint32_t diffuse;
};
struct LegacyStripType3Batch {
    std::int32_t texture;
    std::int32_t vertexCount;
    std::uint32_t firstVertex;
    float sortDepth;
};
static_assert(sizeof(LegacyStripType0Vertex) == 0x10);
static_assert(sizeof(LegacyStripType3Batch) == 0x10);

bool InitializeLegacyStripBuffers(std::uint32_t type0Capacity, std::uint32_t type3Capacity);
void ReleaseLegacyStripType3();
void ResetLegacyStripType3Frame();
bool IsStripBatchingEnabled();
void SetStripBatchingEnabled(std::int32_t enabled);
void SubmitStripType3Quad(std::span<const LegacyStripType0Vertex> vertices);
bool QueueStripType3DrawBatch(std::int32_t count, const void *vertices, std::int32_t texture);
void FlushQueuedStripType3Batches();
void ApplyLegacyStripRenderMode();
void SetLegacyStripProjection(const Matrix4x4 &projection, float nearClip);
void SetLegacyStripDepthTest(bool enabled);
void SetLegacyStripScissor(bool enabled, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom);
extern std::int32_t g_legacyScissorEnabled;
bool DrawLegacyStripType3AlphaTestImmediate(std::span<const LegacyStripType3Vertex> vertices, std::uint32_t texture);
bool DrawLegacyStripType3AlphaBlendImmediate(std::span<const LegacyStripType3Vertex> vertices, std::uint32_t texture);
void GetLegacyStripBlendModes(std::int32_t *source, std::int32_t *destination);
void SetLegacyStripBlendModes(std::int32_t source, std::int32_t destination);

namespace legacy_draw_detail {
extern float temporaryModelScale;
} // namespace legacy_draw_detail

} // namespace shenmue::s2
