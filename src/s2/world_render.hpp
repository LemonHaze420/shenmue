#pragma once

#include "common/math.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_dc.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue {
namespace s2 {
extern std::byte fixedCharacterShadowMesh0[2136];
extern std::byte fixedCharacterShadowMesh1[928];
extern float g_mazeUnitDrawDistance;
extern std::int32_t g_mazeTerrainDrawEnabled;
void SetMazeUnitDrawDistance(float distance);
void SetMazeTerrainDrawEnabled(std::int32_t enabled);

struct Character;
struct MT7ModelChunk;
static const float characterShadowThreshold = 0.003000000026077032f;
void *GetFixedShadowMesh(std::uint32_t index);
// GameJam builds these from C (shadow_cast_dc.c)
extern "C" {
float GetCharacterShadowDistanceFade(const Vector3 *position, const Character *character);
float ComputeShadowLightSource(const Vector3 *position, Vector3 *source, float strength, std::uint32_t lightIndex);
void DrawProjectedCharacterShadow(CharacterUpdateCallback callback, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale, float groundY,
    float strength, const MT7Node *sourceRoot, const MT7Node *shadowRoot, Character *character);
void DrawProjectedFixedShadow(
    const Vector3 *position, const Vector3i *rotation, const Vector3 *scale, float groundY, void *mesh, float strength, Character *character);
void DrawMotionCharacterShadow(const Vector3 *position, float groundY, MT7Node *model, MT7Node *shadow, float strength, Character *character);
void DrawCharacterShadowSprite(const Vector3 *position, float groundY, Character *character);
void DrawProjectedSharedCharacterShadow(const Vector3 *position, std::uint32_t lightIndex, float groundY, Character *character, float opacity);
}
void DrawObject_AssignShadow(const Vector3 *position, float groundY, MT7Node *model, std::int32_t mode, Character *character);
void DrawShenmueEntity(void *parameter);

struct WaveBin {
    float amplitude, phaseA, speedA, phaseB, speedB;
};
typedef char Layout_world_render_0[(sizeof(WaveBin) == 0x14) ? 1 : -1];
typedef char Layout_world_render_1[(offsetof(WaveBin, amplitude) == 0) ? 1 : -1];
typedef char Layout_world_render_2[(offsetof(WaveBin, speedA) == 8) ? 1 : -1];
typedef char Layout_world_render_3[(offsetof(WaveBin, speedB) == 0x10) ? 1 : -1];
extern float g_waveLightAmbient;
extern Vector3 g_waveLightDirection;
void ApplyWaveformToMesh(void *mesh);
void ApplyWaveReflectionToMesh(void *mesh);
void ApplyWaveReflectionToNode(MT7Node *node);
void ApplyWaveformToNode(MT7Node *node);
struct FloatingSurfaceBinding {
    std::int32_t slot;
    Vector3 origin;
};
float FindWaterSurfaceHeight(Vector3 position, MT7Node *root);
void UpdateFloatingSurfaceModels(std::int32_t surfaceSlot, const FloatingSurfaceBinding *bindings);
void ApplyWaveformToModel(MT7ModelChunk *model);
void GenerateFFT(float cellSize);
void BuildWaveLightingTable(std::uint32_t flags, float ambient, float retained, float diffuseR, float diffuseG, float diffuseB, float specularR,
    float specularG, float specularB, float exponent, float intensity);
void *FillFftKernel(float scale, void *kernel);
void FillFftDistance(const void *kernel);
void ApplyFft(const void *first, const void *second);
void FinalizeFft(float scrollU, float scrollV);
void SetWaveLighting(float ambient, const Vector3 *direction);
} // namespace s2
} // namespace shenmue
#endif
#else
#include "s2/air_data.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_draw.hpp"

#include <cstddef>
#include <array>
#include <cstdint>
#include <span>

namespace shenmue::d3t {
struct LargeFileHandle;
}

namespace shenmue::s2 {
struct WeatherAir;
struct MT7Model;
struct MT7Node;
} // namespace shenmue::s2

namespace shenmue::s2 {

std::uint32_t DrawMT7Model(const MT7Model &model, const Matrix4x4 *entityTransform = nullptr, LegacyModelShaderPath shaderPath = LegacyModelShaderPath::Lambert,
    float alpha = 1.0f, const LegacyNodeCommandOverride *commandOverride = nullptr, const Matrix4x4 *matrixFrames = nullptr,
    const Matrix4x4 *controllerTransformBase = nullptr);
std::uint32_t DrawMT7NodeHierarchy(
    const MT7Model &model, const MT7Node &node, LegacyModelShaderPath shaderPath = LegacyModelShaderPath::Lambert, float alpha = 1.0f);
std::uint32_t mtRecursiveSetFlag(const MT7Model &model, const MT7Node *node, float alpha);
extern Vector3 g_scriptedModelScale;
extern std::int32_t g_modelDeformationSuppressed;
void InitModelVector();
// The D3T light direction is reset by SkyDomeProbe and republished by the LGHT task.
extern Vector3 g_d3tLightDirection;
// The nine HD sky-dome sources under a directory are placed for the Sky
// and SkyProbe caches; the release frees each source's realized model handle.
void LoadSkyDomeModels(const char *directory);
void ReleaseSkyDomeModels(const std::uint32_t *handles);
// Native SetupRenderBufferHandles/SkyDomeProbe, run by MapControlTask_Enqueue on /scene/maps/<map>.
void SetupRenderBufferHandles();
void SkyDomeProbe(const char *mapDirectory);
extern Matrix4x4 g_scriptedIncomingTransform;
void mtRestoreIncomingTransform();
void charaApplyScale(const MT7Model *model, const MT7Node *node, float alpha = 1.0f);
void applyPosRotScl(const MT7Model *model, const MT7Node *node);
void charaPrepareNodesForDraw(const MT7Model *model, const MT7Node *node, float alpha);
void imgmDrawModelNode(const MT7Model *model, const MT7Node *node, float alpha);
void ConstructMatricesForObjectRecursive(MT7Model &model, MT7Node *node);
void PrepareProceduralObjects(MT7Model &model, MT7Node *node, float alpha);
void mtUpdateTransparencyCallback(const MT7Model *model, const MT7Node *node, float alpha);
void DrawMT7ProceduralBranch(MT7Model &model, MT7Node *node, std::uint16_t cutoffId, float alpha);
void SetupRefboard();
std::uint64_t DrawDummyCharacterModel(const MT7Model &model);
inline constexpr shen_name SCENARIO_PIN_CHARACTER{SHEN_NAME(PIN_)};
void DrawPIN(shen_name character, std::int32_t enabled, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale);
inline constexpr shen_name SCENARIO_DUMMY_CHARACTER{SHEN_NAME(dumy)};
void ApplyTranslationRotationScaleXYZToCharaByID(shen_name character, float scaleY, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale);
void ApplyTranslationRotationScaleXYZToCharaSibling(
    shen_name character, std::int32_t siblingOrdinal, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale);
// Native g_entity is scoped by the character core, not retained by the model.
struct Character;
Character *SetScriptedModelEntity(Character *entity);
// Callback-based projected shadow; the caller supplies its
// entity TRS and selected root, and retains all model ownership.
void ProjectCharacterShadow(
    Character *character, const Vector3 &position, const Vector3i &rotation, const Vector3 &scale, float groundY, const MT7Model *root, float strength);
void ProjectFixedCharacterShadow(
    Character *character, const Vector3 &position, const Vector3i &rotation, const Vector3 &scale, float groundY, std::size_t meshIndex, float strength);
void DrawObject_AssignShadow(Character *character, const Vector3 &position, float groundY);
void DrawShenmueEntity(void *parameter);
extern std::uint32_t g_characterShadowDisabled;
inline constexpr std::size_t characterShadowStorageSize = 0x1c020;
alignas(32) extern std::array<std::byte, characterShadowStorageSize> g_characterShadowStorage;
extern void *g_characterShadowRuntime;
void SetShadowPositioningMode(bool relative);
void SetCharacterUngroundedShadowMode(std::int32_t mode);
void ResetCharacterShadowStencilReference();

struct MapModelRuntimeSlot;
struct MapCellOffset;
// Native ACPH/ACPD consumer; 0 is world, 1 includes field-grid depth, 2 queues projected MAPS shadows.
std::uint32_t ProcessObjectRenderCommandStream(const MapModelRuntimeSlot &slot, std::span<const MapCellOffset> visibleCells, MapCellOffset cameraCell,
    LegacyModelShaderPath shaderPath, std::int32_t mode);
std::uint32_t ProcessAlphaObjectRenderCommandStream(
    const MapModelRuntimeSlot &slot, std::span<const MapCellOffset> visibleCells, MapCellOffset cameraCell, LegacyModelShaderPath shaderPath, float alpha);
void SetTempCommandStreamOffset(const Vector3 *offset);
const Vector3 &GetTempCommandStreamOffset();
std::uint32_t DrawObjectWorldPass();
std::uint32_t DrawMapLightingPlacementWorldPass();
HLib::Task *QueueObjectDrawTask(std::uint8_t nextFunctionIndex);
inline constexpr float defaultShadowRange = 14.5f;
extern Vector3 g_shadowRange;
float GetShadowRange();
void SHRG_Reader(const void *chunk);
shen_name MVTR_Reader();
void SetGlobalShadowLightingEnabled(std::int32_t enabled);
bool IsGlobalShadowLightingEnabled();
void ResetGlobalShadowTiming();
void SetGlobalShadowFadeRequested(std::int32_t requested);
float GetGlobalShadowOpacity();
void UpdateGlobalShadowTiming();
void MaybeDoGlobalShadowLighting();
void ResolveGlobalShadowLighting(float opacity = 0.0f);
std::uint32_t DrawAirWorldPass();
void InitializeAirPresentation(const void *mapInfo);
void ResetAirPresentation();
inline constexpr shen_name TASK_AIRD = SHEN_NAME(AIRD);
// Native AllocateDomeMesh reads DOOM.BIN as DOOE and copies each sky model out of it as DOME task data.
inline constexpr shen_name AIR_DOOM_FILE{SHEN_NAME(DOOE)};
inline constexpr shen_name AIR_DOME_MODEL{SHEN_NAME(DOME)};
extern std::int32_t g_airDrawDomeEnabled;
extern std::int32_t g_airDrawCloudsEnabled;
extern std::int32_t g_airDrawPlanetsEnabled;
extern std::uint32_t g_airClearColour;
void SetClearColourForClouds(std::uint32_t colour);
void SetCloudAndSkyColour(std::uint32_t skyColour, std::uint32_t cloudColour);
std::uint32_t GetWeatherCloudOrSkyColour(std::int32_t channel);
#if defined(SHENMUE_NULL_LIBRARY)
#pragma region NullLibrary
bool ReadNullAirSky(float *nightBlend);
#pragma endregion
#endif
void SetIsSkySetup(std::int32_t value);
void SetPlanetRotByIndex(const Vector3i &rotation, std::int32_t index);
void UpdateCelestialRotation(const Vector3i &angles, const Vector3 &input, Vector3 &output);
void UpdateCelestialCoordinates();
void shenUpdatePlanetByLayer(float value, std::int32_t channel, std::int32_t layer);
void EV_SetPlanetLayerColours_Impl(std::uint32_t firstColour, std::uint32_t secondColour, std::int32_t layer);
float *SetPlanetLayerParameter(float value, std::int32_t layer);
float GetPlanetLayerParameter(std::int32_t layer);
std::uint64_t GetPlanetLayerColour(std::int32_t channel, std::int32_t layer);
std::uint64_t SetPlanetLayerOffset(const Vector3 &offset, std::int32_t layer);
std::uint64_t ResetPlanetLayerOffset(std::int32_t layer);
struct AirCloudTextureState {
    std::int32_t active{};
    std::int32_t frames{};
    std::int32_t loadingTextureIndex{-1};
    d3t::LargeFileHandle *sprite{};
    std::array<float, 2> blendSteps{};
    std::array<std::int32_t, 2> textureIndices{-1, -1};
};
std::int32_t GetHasInitCloudTextures();
std::int32_t GetCurrentAirLayerIndex();
bool GetIsCurrentCloudTextureValid();
void SetSkyWeatherModes(std::int32_t previous, std::int32_t current);
extern std::int32_t g_previousSkyWeatherMode, g_currentSkyWeatherMode;
void UpdateWeatherClouds(WeatherAir *air, std::int32_t frames);
void UpdateClouds(WeatherAir *air);
HLib::Task *QueueAirDrawTask(std::uint8_t nextFunctionIndex);

// Native environmental FFT resource owner. Kernels are
// caller-owned 8x8 float tables; byte spans preserve their authored storage.
void GenerateFFT(float cellSize);
void *GenerateDistanceFalloffRowMajor(float exponent, std::span<std::byte, 0x100> kernel);
void *GenerateDistanceFalloffKernel8x8(float exponent, std::span<std::byte, 0x100> kernel);
void *FillFftKernel(float scale, void *kernel);
void FillFftDistance(const void *kernel);
void ApplyFft(const void *first, const void *second);
void FinalizeFft(float scrollU, float scrollV);
std::span<const LegacyWaveVertex, 289> GetWaveVertices();
std::span<const float, 289> GetWaveHeights();
// Native surface lighting intentionally continues its real-height reads into
// the adjacent imaginary array; expose both borrowed banks without cross-array arithmetic.
struct WaveLightingView {
    std::span<const float, 289> real;
    std::span<const float, 289> imaginary;
    const std::uint32_t &flags;
    const float &cellSize;
    const float &lightAmbient;
    const Vector3 &lightDirection;
    const float &ambient;
    const Vector3 &diffuse;
    const Vector3 &specular;
    const float &specularExponent;
    const float &specularIntensity;
    const float &scrollU;
    const float &scrollV;
};
WaveLightingView GetWaveLightingView();
void SetWaveLighting(float ambient, const Vector3 &direction);
void BuildWaveLightingTable(std::uint32_t flags, float ambient, float unused, const Vector3 &diffuse, const Vector3 &specular, float exponent, float intensity);
// Waveform and UV animation traverse the slot's visible root subtree and mutate its retained XB01 buffers.
// Reflection recolours the second slot hierarchy.
bool ApplyWaveTextureResource(std::int32_t slotIndex);
// Single first-child mesh owner; unlike the tree owner it does not touch the matrix stack.
bool ApplyWaveTextureMeshResource(std::int32_t slotIndex);
bool ApplyWaveReflectionResource(std::int32_t slotIndex);
void SetTextureAnimationUV(float scrollU, float scrollV);
bool ApplyTextureAnimationResource(std::int32_t slotIndex);
std::uint32_t *FindSpringTextureCommand(MT7Node *node, std::int32_t ordinal);
MT7Node *InitializeSpringTextureAnimation(std::int32_t preset);
std::uint32_t *AdvanceSpringTextureAnimation();
inline constexpr std::int32_t waterTexturePresets[8][4]{
    {20, 23, 21, 22}, {10, 13, 11, 12}, {20, 23, 21, 22}, {12, 15, 13, 14}, {8, 11, 9, 10}, {12, 16, 13, 14}, {20, 23, 21, 22}, {12, 16, 13, 14}};
MT7Node *InitializeWaterTextureAnimation(std::int32_t preset);
void AdvanceWaterTextureAnimation();
std::uint32_t DrawWaterTextureSet();
std::uint32_t DrawWaterReflectionSet();
std::uint32_t DrawSpringTextureNode();
MT7Node *InitializeWaterReflectionSet();
void AdvanceWaterReflectionSet();
struct FloatingSurfaceBinding {
    std::int32_t slot;
    Vector3 origin;
};
static_assert(sizeof(FloatingSurfaceBinding) == 16);
static_assert(offsetof(FloatingSurfaceBinding, origin) == 4);
inline constexpr Vector2 floatingSurfaceSamplePoints[3]{{0.0f, 3.8f}, {1.4f, -3.98f}, {-1.4f, -3.98f}};
void UpdateFloatingSurfaceModels(std::int32_t surfaceSlot, const FloatingSurfaceBinding *bindings);

// Native shadow readers have separate per-map and cached shared lifetimes.
inline constexpr shen_name MEMORY_SHADOW_SPRITE{SHEN_NAME(SHDM)};
inline constexpr shen_name MEMORY_SHARED_SHADOW{SHEN_NAME(SHDS)};
inline constexpr std::size_t scenarioShadowModelCacheCapacity = 10;
struct ScenarioShadowModelCacheEntry {
    MT7Model *model;
    char filename[16];
};
static_assert(sizeof(ScenarioShadowModelCacheEntry) == 0x18);
static_assert(offsetof(ScenarioShadowModelCacheEntry, filename) == 8);
MT7Model *LoadShadowSprite();
MT7Model *CreateMT7(const char *image);
void ResetShadowData();
bool Read_SYDNSRSSS();
bool LoadSYD();
void RetireSharedShadowModel();
void *LoadSYD_S();
void *AllocateSHDS();
const MT7Model *GetSHDS();
const MT7Model *GetShadowDataModel(std::size_t index);
std::uint32_t DrawFixedShadowMesh(std::size_t index, float alpha);

} // namespace shenmue::s2

#endif
