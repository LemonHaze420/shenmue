#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#define SHENMUE_SDK_CONSTANT static const
#else
#include <cstddef>
#include <cstdint>
#define SHENMUE_SDK_CONSTANT inline constexpr
#endif
#include "common/math.hpp"

namespace shenmue {
namespace s1 {
struct ShadowVolume;
struct MoveShadowSettings;
}
} // namespace shenmue::s1

namespace shenmue {
namespace dc {
namespace sdk {

#if SHENMUE_S2_GCC295
typedef void (*Callback)();
#else
using Callback = void (*)();
#endif

void ScaleFogIntensity(float *table, float intensity);
void ComputeFogAttenuation(float *table, float nearDistance, float farDistance);

SHENMUE_SDK_CONSTANT std::size_t kSystemConfigurationWords = 24;
SHENMUE_SDK_CONSTANT std::uintptr_t kPvrRevisionRegister = 0xa05f689c;
SHENMUE_SDK_CONSTANT std::uintptr_t kVertexWorkspaceEnd = 0x0d000000;
SHENMUE_SDK_CONSTANT std::size_t kConfigurationFlags = 1;
SHENMUE_SDK_CONSTANT std::size_t kConfigurationTextureBytes = 7;
SHENMUE_SDK_CONSTANT std::size_t kConfigurationVertexBase = 9;
SHENMUE_SDK_CONSTANT std::size_t kConfigurationVertexWords = 10;
SHENMUE_SDK_CONSTANT std::size_t kConfigurationVertexPercentages = 11;
SHENMUE_SDK_CONSTANT std::size_t kConfigurationListType = 23;
SHENMUE_SDK_CONSTANT std::size_t kVertexListCount = 5;
SHENMUE_SDK_CONSTANT std::size_t kSystemMetricsWords = 50;
SHENMUE_SDK_CONSTANT std::size_t kGraphicsWorkspaceStackDepth = 48;
SHENMUE_SDK_CONSTANT std::size_t kGraphicsWorkspaceStackCells = kGraphicsWorkspaceStackDepth * 2 + 1;
SHENMUE_SDK_CONSTANT std::size_t kLightCount = SHENMUE_IS_S2 ? 16 : 32;
SHENMUE_SDK_CONSTANT std::size_t kTextureSlots = SHENMUE_IS_S2 || SHENMUE_IS_S1 ? 0x280 : 0x600;
SHENMUE_SDK_CONSTANT std::uint32_t kVibrationDeviceFunctionIndex = 8;
#if SHENMUE_S2_GCC295
SHENMUE_SDK_CONSTANT std::int32_t kSystemDisplayModes[21] = {
#else
SHENMUE_SDK_CONSTANT std::int32_t kSystemDisplayModes[21]{
#endif
    0x31, 0x00, 0x04, 0x10, 0x14, 0x20, 0x28, 0x24, 0x30, 0x38, 0x34, 0x02, 0x06, 0x12, 0x16, 0x22, 0x2a, 0x26, 0x32, 0x3a, 0x36};
#if SHENMUE_S2_GCC295
SHENMUE_SDK_CONSTANT float kFixedAngleTurnRadians = 6.28318500518798828125f;
#else
SHENMUE_SDK_CONSTANT float kFixedAngleTurnRadians = 0x1.921fb4p+2f;
#endif
SHENMUE_SDK_CONSTANT std::size_t kLightRecordWords = 0xb0 / sizeof(std::uint32_t);
SHENMUE_SDK_CONSTANT std::uint32_t kDefaultLightRecord[kLightRecordWords] = {
    0,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0x3f800000,
    0,
    0,
    0x3f800000,
    0,
    0,
    0x3f800000,
    0x3f800000,
    0,
    0,
    0,
    0,
    0,
    0x3f800000,
    0,
    0,
    0x3f800000,
    0,
    0,
    0x3f800000,
    0,
    0,
    0x3f800000,
    0,
    0x37800000,
    0xff0f0f0f,
    0x4f800000,
    0x3f800000,
    0x47800000,
    0,
    0,
    0,
};

extern "C" std::int32_t syCblCheckCable();
extern "C" void njInitSystem(std::int32_t mode, std::int32_t frame, std::int32_t count);
extern "C" void njSetBackColor(std::uint32_t first, std::uint32_t second, std::uint32_t third);
extern "C" void njGetMatrix(Matrix4x4 *memory);
extern "C" void njGetTranslation(const Matrix4x4 *matrix, Vector3 *position);
extern "C" void njSetMatrix(Matrix4x4 *destination, const Matrix4x4 *source);
extern "C" void njMultiMatrix(Matrix4x4 *destination, const Matrix4x4 *source);
extern "C" void njMirror(Matrix4x4 *matrix, const void *plane);
extern "C" void njUnitMatrix(Matrix4x4 *matrix);
extern "C" std::int32_t njInvertMatrix(Matrix4x4 *matrix);
extern "C" void njTransposeMatrix(Matrix4x4 *matrix);
extern "C" void njTranslate(Matrix4x4 *matrix, float x, float y, float z) __attribute__((renesas));
extern "C" void njScale(Matrix4x4 *matrix, float x, float y, float z) __attribute__((renesas));
extern "C" void njRotateX(Matrix4x4 *matrix, std::int32_t angle);
extern "C" void njRotateY(Matrix4x4 *matrix, std::int32_t angle);
extern "C" void njRotateZ(Matrix4x4 *matrix, std::int32_t angle);
extern "C" void njCalcPoint(Matrix4x4 *matrix, Vector3 *source, Vector3 *destination);
extern "C" void njCalcVector(Matrix4x4 *matrix, Vector3 *source, Vector3 *destination);
extern "C" float njScalor(const Vector3 *vector);
extern "C" float njInvertSqrt(float value) __attribute__((renesas));
extern "C" float njUnitVector(Vector3 *vector);
#if SHENMUE_S2_GCC295
extern "C" float njSqrt(float value);
#endif
extern "C" float njInnerProduct(const Vector3 *first, const Vector3 *second);
extern "C" float njOuterProduct(const Vector3 *first, const Vector3 *second, Vector3 *output);
extern "C" float njSin(std::int32_t angle);
extern "C" float njCos(std::int32_t angle);
extern "C" float njTan(std::int32_t angle);
extern "C" void njInitVertexBuffer(
    std::int32_t opaque, std::int32_t opaqueModifier, std::int32_t transparent, std::int32_t transparentModifier, std::int32_t punchThrough);
extern "C" void njInitPrint(std::uint32_t *texture, std::uint32_t size, std::uint32_t attributes);
extern "C" void njSetBorderColor(std::uint32_t colour);
extern "C" void njTextureFilterMode(std::int32_t mode);
extern "C" void njSetWaitVSyncFunction(Callback callback, void *parameter);
extern "C" void pdExecPeripheralServer();
extern "C" std::int32_t gdFsGetDrvStat();
extern "C" void syBtExit();
extern "C" std::int32_t kmChangeLatencyModel(std::uint32_t model, std::uint32_t flags);
extern "C" std::int32_t kmfSetVertexBufferBaseAddress(void *memory, std::uint32_t offset);
extern "C" std::int32_t kmFlipFrameBuffer();
extern "C" std::int32_t kmRender();
extern "C" std::int32_t kmfGetInternalSystemMetrics(std::uint32_t *metrics);
extern "C" std::uint32_t g_systemConfiguration[kSystemConfigurationWords] asm("__nj_SystemConfigStruct_");
extern "C" std::uint32_t *g_sdkVertexBufferPointers asm("__nj_vertex_buffer_");
extern "C" std::uint32_t g_sdkTextureMemoryBytes asm("__nj_texture_memory_size");
extern "C" std::uint32_t *g_sdkUserVertexBuffer asm("__nj_usr_vertex_buffer_ptr_");
extern "C" std::uint32_t g_sdkUserVertexBufferBytes asm("__nj_usr_vertex_buffer_size_");

extern std::int16_t g_graphicsWorkspaceDepth;
extern std::byte *g_graphicsWorkspaceStack[kGraphicsWorkspaceStackCells];
extern std::byte *g_graphicsWorkspaceFront;
extern std::byte *g_graphicsWorkspaceFrontSaved;
extern std::byte *g_graphicsWorkspaceFrontBase;
extern std::byte *g_graphicsWorkspaceBack;
extern std::byte *g_graphicsWorkspaceBackSaved;
extern std::byte *g_graphicsWorkspaceBackBase;
extern std::uint32_t g_lightRecords[kLightCount][kLightRecordWords];
extern std::uint32_t g_lightMaskByMode[2];
extern std::uint32_t g_enabledLightMask;
extern std::uint32_t g_lightProcessingEnabled;
enum ModelLightingMode { Single, Multiple };
extern ModelLightingMode g_modelLightingMode;
extern Vector4 g_ambientLight;
extern float g_modelScale;
extern float g_inverseModelScale;
extern std::uint32_t g_reflectedLightingEnabled;
extern Vector3 g_reflectorLocalDirection;
extern Vector3 g_reflectorWorldDirection;
extern Vector3 g_reflectorViewDirection;
extern float g_reflectedLightIntensity;
extern std::uint32_t g_vertexBufferBaselines[kVertexListCount];
extern std::uint32_t g_systemMetrics[kSystemMetricsWords];
extern volatile std::uint32_t g_renderSubmissionActive;
extern std::uint32_t g_punchThroughBufferBytes;
extern std::uint32_t g_printFlags;
#if SHENMUE_IS_S2
extern std::uint32_t g_modifierVolumeMode;
extern std::int32_t g_modifierVolumeListControl;
extern std::int32_t g_printCharacterWidth;
extern std::int32_t g_printCharacterHeight;
void SetModifierVolumeMode(std::uint32_t mode);
void SetModifierVolumeListControl(std::int32_t control);
void SetPrintCharacterSize(std::int32_t width, std::int32_t height);
#endif
extern std::int32_t g_modelTextureIds[kTextureSlots];
extern std::uint32_t g_polygonContextMode;
extern std::uint32_t g_modelTextureIdBase;

struct ModelPolygonContext {
    std::uint32_t flags;
    std::uint32_t isp;
    std::uint32_t tsp;
    std::uint32_t texture;
};
#if SHENMUE_S2_GCC295
typedef char ModelPolygonContextSizeCheck[sizeof(ModelPolygonContext) == 0x10 ? 1 : -1];
typedef char ModelLightingModeSizeCheck[sizeof(ModelLightingMode) == 4 ? 1 : -1];
#else
static_assert(sizeof(ModelPolygonContext) == 0x10);
#endif

// Semantic imports for the WS SDK variant; later Ninja headers have different entry-point signatures.
void InitializeSystem(std::int32_t mode, std::uint32_t framebufferFlags, std::int32_t count);
void SetVertexWorkBase(void *memory);
void SetTextureMemorySize(std::uint32_t bytes);
void InitializeRenderingMode();
void UpdateModelScreenTransform();
void SetModelScale(float scale);
void DrawBuiltInModel(void *model);
void DrawBuiltInModelWithContext(void *model, void *context);
void DrawBuiltInModelAlpha(void *model, float opacity);
void DrawLitModel(void *model);
void DrawLitModelAlpha(void *model, float opacity);
void TransformLitModel(void *model);
void InitializeParticleChannels();
void InitializeFftEffects();
void InitializeTerrainEffects();
void UpdateFftEffects();
void UpdateParticleChannel(std::int32_t index);
void DrawParticleChannel(std::int32_t index);
void SetParticleChannelEnabled(std::int32_t index, std::int32_t enabled);
void DrawShadowVolumes(s1::ShadowVolume *const *volumes, const s1::MoveShadowSettings *settings);
void SetAmbientLight(const Vector4 *colour);
void SetLightColour(std::uint32_t index, const float *colour);
#if SHENMUE_IS_S2
void SetLightModelColour(std::uint32_t index, const Vector4 *colour);
void SetLightAttenuation(std::uint32_t index, float constant, float linear, float quadratic);
#endif
void SetLightIntensity(std::uint32_t index, float intensity);
void SetLightProcessingEnabled(std::uint32_t enabled);
void SetLightType(std::uint32_t index, std::uint32_t type);
void SetLightEnabled(std::uint32_t index, std::int32_t enabled);
void SetLightPosition(std::uint32_t index, float x, float y, float z);
void SetLightDirection(std::uint32_t index, float x, float y, float z);
void SetLightRange(std::uint32_t index, float range);
void SetLightFalloff(std::uint32_t index, float falloff);
void SetLightAngle(std::uint32_t index, std::int32_t angle);
void CommitLightState();
void TransformLightState();
void EnableReflectedLighting(const Vector3 *direction, float intensity);
void DisableReflectedLighting();
void SetPunchThroughBufferSize(std::uint32_t bytes);
void SetVertexBufferSizes(
    std::int32_t opaque, std::int32_t opaqueModifier, std::int32_t transparent, std::int32_t transparentModifier, std::int32_t punchThrough);
void SetBorderColour(std::uint32_t colour);
void InitializePrint(void *texture);
void EnablePrint();
void SetPrintSize(std::uint16_t size) asm("_njPrintSize");
void RegisterWeatherTaskCallback(Callback callback);
void InitializeGraphicsWorkspace(std::byte *begin, std::byte *end);
void InitializationNullsub();
void InitializeMatrixConstantsNullsub();
void InitializeTransformConstants() asm("_shenmue_dc_matrix_identity");
void InitializeLightState();
void SetModelTextureIds(void *textures, std::int32_t count);
void BindModelTexture(ModelPolygonContext *context, std::int32_t texture);
void SetPolygonContextMode(std::uint32_t mode);
void PrepareTexturedModel(void *model);
std::int32_t PrepareUntexturedModel(void *model, void *textureIds, void *materialIds);
void RegisterInterruptCallback(Callback callback) asm("_pdSetIntFunction");
void ResetGraphicsWorkspace();
std::int32_t PushGraphicsWorkspace();
std::int32_t PopGraphicsWorkspace();
void SnapshotVertexBufferPointers();
void GetVertexBufferUsage(std::uint32_t *bytes);
void CollectSystemMetrics();
void SubmitFrame();
void ResetAudioMiddleware() asm("_ADXT_ResetLibrary");
void ExecuteAudioMiddleware() asm("_ADXT_ExecServer");
std::uint32_t RandomInt();
float RandomFloat();
void SeedRandom(std::uint32_t seed);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float TruncateFloat(float value);
#endif
float SinFixed(std::int32_t angle);
#if SHENMUE_IS_S2
float SinRadians(float angle);
float CosRadians(float angle);
#endif
float CosFixed(std::int32_t angle);
float TanFixed(std::int32_t angle);
float InverseSqrt(float value);
std::uint32_t ArcTanFixed(float value);
std::uint32_t ArcTan2Fixed(float y, float x);
std::uint32_t ArcSinFixed(float value);
void PrintText(std::int32_t x, std::int32_t y, const char *text);
std::int32_t StartVibration(std::uint32_t port, const std::uint8_t *packet);
void PurgeDataCache(void *memory, std::uint32_t blocks);
}
}
} // namespace shenmue::dc::sdk

#undef SHENMUE_SDK_CONSTANT

#endif
