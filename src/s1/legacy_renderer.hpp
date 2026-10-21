#pragma once

#include "s1/matrix_stack.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace shenmue::s1 {

inline constexpr std::array<std::uint32_t, 68> kDefaultRenderFlagOrder{7, 8, 66, 64, 60, 62, 63, 147, 57, 61, 58, 26, 59, 92, 103, 34, 35, 36, 37, 38, 48, 52,
    53, 54, 55, 56, 1, 2, 3, 4, 128, 129, 130, 131, 102, 139, 141, 142, 143, 145, 146, 5, 148, 151, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 166, 67,
    171, 16, 132, 133, 134, 135, 136, 140, 152, 165, 167, 170};
struct AlphaRenderDefaults {
    std::array<std::int32_t, 32> values;
};

static_assert(sizeof(AlphaRenderDefaults) == 0x80);
inline constexpr std::size_t kDebugTextQueueCapacity = 33792;

struct DebugTextRecordHeader {
    float x;
    float y;
    std::uint32_t colour;
    std::uint32_t textLength;
    float characterWidth;
    float characterHeight;
    float advanceX;
    float lineHeight;
};

static_assert(sizeof(DebugTextRecordHeader) == 32);


struct RenderPipelinePreset {
    std::int32_t vertexShaderAndLayoutIndex;
    std::int32_t pixelShaderIndex;
    std::int32_t vertexStride;
};

static_assert(sizeof(RenderPipelinePreset) == 12);

struct EngineAlphaState {
    std::uint64_t flags;
    std::uint64_t companion;
};

static_assert(sizeof(EngineAlphaState) == 0x10);

struct AlphaShaderParameters {
    std::uint32_t operation;
    std::uint32_t argument0;
    std::uint32_t argument1;
    std::uint32_t argument2;
};

struct LegacyShaderParameters {
    Vector4 fogColour;
    std::uint32_t fogEnabled;
    std::uint32_t alphaTestEnabled;
    float alphaReference;
    std::uint32_t alphaFunction;
    std::uint32_t specularEnabled;
};

struct DirectionalLightShaderParameters {
    Vector4 direction;
    Vector4 diffuse;
};

struct PointLightShaderParameters {
    Vector4 packed;
};

struct SpotLightShaderParameters {
    Vector4 position;
    Vector4 direction;
    Vector4 diffuse;
    Vector4 attenuation;
    Vector4 cutoff;
};

struct ShaderParams {
    Vector4 fogColour;
    std::uint32_t fogEnabled;
    float viewportScaleX;
    float viewportScaleY;
    float padding;
    std::uint32_t alphaTestEnabled;
    float alphaReference;
    std::uint32_t alphaFunction;
    std::uint32_t alphaOperation;
    std::uint32_t alphaArgument0;
    std::uint32_t alphaArgument1;
    std::uint32_t alphaArgument2;
    std::uint32_t specularEnabled;
    std::uint32_t renderMode;
    std::uint32_t ignoreLighting;
    std::uint32_t ignoreAlpha;
    std::uint32_t useEnvironmentMap;
    std::array<Vector4, 17> constants;
    Vector4 ambientLight;
    Vector4 lightCount;
    std::array<DirectionalLightShaderParameters, 3> directionalLights;
    std::array<PointLightShaderParameters, 32> pointLights;
    std::array<SpotLightShaderParameters, 3> spotLights;
};

static_assert(sizeof(ShaderParams) == 1232);
static_assert(offsetof(ShaderParams, constants) == 0x50);
static_assert(offsetof(ShaderParams, ambientLight) == 0x160);

struct LegacyRendererDefaults {
    std::int32_t depthStencilEnabled;
    std::int32_t renderFlag4;
    std::int32_t alphaTestEnabled;
    std::int32_t alphaReference;
    std::int32_t fogMode;
    std::int32_t fogCurve;
    std::int32_t renderingMode;
    std::int32_t bloomEffect;
    std::int32_t alphaFunction;
    std::int32_t fogDensityEnabled;
    std::int32_t fogEnabled;
    std::int32_t specularEnabled;
    std::uint32_t fogColour;
    float fogScalar;
    std::int32_t renderFlag;
    std::int32_t advancedRenderFlags;
    std::int32_t renderFlag2;
    std::int32_t renderFlag3;
    std::int32_t renderLayer;
    std::int32_t stencilReference;
    std::int32_t stencilMask;
    std::uint64_t stencilOperations;
    std::int32_t shadeMode;
    std::int32_t fogQuality;
};

extern std::array<AlphaRenderDefaults, 4> g_alphaRenderDefaults;
extern std::array<AlphaRenderDefaults, 4> g_currentAlphaRenderFlags;
extern LegacyRendererDefaults g_legacyRendererDefaults;
extern std::array<EngineAlphaState, 4> g_engineAlphaState;
extern AlphaShaderParameters g_alphaShaderParameters;
extern LegacyShaderParameters g_legacyShaderParameters;
extern ShaderParams g_renderParams;
extern std::array<std::int32_t, 172> g_defaultRenderFlags;
extern const std::array<RenderPipelinePreset, 8> g_inputLayoutTypes;
extern std::int32_t g_renderVertexShaderAndLayoutIndex;
extern std::int32_t g_renderPixelShaderIndex;
extern std::int32_t g_renderVertexStride;
extern std::int32_t g_renderPassIndex;
extern std::int32_t g_numVertices;
extern std::array<std::int32_t, 172> g_currentRenderFlags;
extern std::uint32_t g_rendererWhiteTexture;
extern std::int32_t g_legacyCurrentRenderingMode;
extern std::uint64_t g_legacyEngineFlags;
extern std::uint64_t g_legacyEngineField8;
extern std::int32_t g_legacyStencilReference;
extern std::array<std::uint32_t, 2> g_legacyAlphaFlags;
extern std::int32_t g_perspectiveProjection;
extern Matrix4x4 g_cameraProjectionMatrix;
extern float g_tempNearClip;
extern float g_tempFarClip;
extern float g_tempExtentNearClip;
extern float g_screenWidth;
extern float g_screenHeight;
extern float g_screenWidthOffset;
extern float g_screenHeightOffset;


void Matrix4x4_RotateAndPush();
void WorldToScreen(const Vector3 &world, Vector2 &screen);
void ConstructWorldSpaceFromScreen(const Vector3 &world, Vector2 &screen);
void InitRenderer();
void SetAlphaRenderFlags();
void SetDefaultAlphaFlags();
void SetDefaultRenderFlags();
void SetRenderFlags(std::uint32_t setting, std::int32_t value);
void SetAlphaFlags(std::uint32_t lane, std::uint32_t setting, std::uint32_t value);
void Create_2x2_Texture();
void SetRenderAndAlphaFlags(std::uint32_t textureWord);
void SetRenderAndAlphaFlags_0();
bool CreateAlphaTextureForParticleEffects(const std::uint32_t *words);
void DrawVertices();
void DrawVertices_HN();
void DrawQueuedVertices_HN();

extern const std::array<void (*)(), 3> g_mainDrawingFunctions;
void SetDirectXWindowConfig(std::int32_t offsetX, std::int32_t offsetY, std::int32_t width, std::int32_t height, float globalX, float globalY);
void SetupDirectXWindow(float widthOffset, float heightOffset, float screenWidth, float screenHeight);
void Matrix4x4_SetupCameraProjectionMatrix(std::int32_t pitch, float aspectRatio, float nearClip, float farClip);

bool QueueDebugText(float x, float &y, std::string_view text);
void ClearDebugTextQueue() noexcept;
const std::byte *GetDebugTextQueueData() noexcept;
std::size_t GetDebugTextQueueSize() noexcept;
std::uint32_t GetDebugTextQueueCount() noexcept;
} // namespace shenmue::s1
