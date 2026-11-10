#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct CameraConfigPreset {
    char name[128];
    std::uint8_t fpsMode;
    std::byte reserved81[3];
    float fov;
    float nearPlane;
    float farPlane;
    std::byte reserved90[4];
    float position[4];
    float direction[4];
    std::byte reservedB4[32];
    float lookAt[4];
    float eulerAngles[2];
};
static_assert(sizeof(CameraConfigPreset) == 236);
static_assert(offsetof(CameraConfigPreset, position) == 0x94 && offsetof(CameraConfigPreset, lookAt) == 0xd4);
inline constexpr float kCameraConfigDefaultFov = 47.0f;
inline constexpr float kCameraConfigDefaultNearPlane = 0.15000001f;
inline constexpr float kCameraConfigDefaultFarPlane = 3000.0f;
extern std::array<CameraConfigPreset, 16> g_cameraConfigPresets;
extern std::int32_t g_cameraConfigPresetCount;

struct SDAccumulationTarget {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t kind;
    std::uint32_t reserved0C;
    std::uint32_t texture;
    std::uint32_t reserved14;
};
static_assert(sizeof(SDAccumulationTarget) == 24 && offsetof(SDAccumulationTarget, texture) == 0x10);
inline constexpr std::uint32_t kSDAccumulationTargetKind = 18;
extern std::array<SDAccumulationTarget, 3> g_sdAccumulationTargetTable;
extern std::int32_t g_sdAccumulationTargetCount;
extern SDAccumulationTarget *g_sdAccumulationTargets;
std::int32_t CreateSDAccumulationTargets();
std::int32_t ReleaseSDAccumulationTargets();
std::uint32_t GetPostProcessingRenderTarget(std::int32_t index);
std::uint32_t *GetFullscreenTexture();

extern std::uint32_t g_currentRenderHandle;
extern std::uint32_t g_mainModelCache;
extern std::uint32_t g_sdOverrideModelCache;

extern std::uint32_t g_skyModelCache;
extern std::uint32_t g_skyProbeModelCache;
extern std::int32_t g_doLowMipsOnly;
extern std::array<std::uint32_t, 40> g_fvfLayouts;
extern std::uint32_t g_originalStrip00VertexShader;
extern std::uint32_t g_originalStrip03VertexShader;
extern std::uint32_t g_originalStrip00PixelShader;
extern std::uint32_t g_originalStrip03PixelShader;
extern std::array<std::uint32_t, 10> g_legacyLambertVertexShaderMap;
extern std::uint32_t g_originalConstantVertexShader;
extern std::uint32_t g_originalVertexColour0Shader;
extern std::uint32_t g_originalVertexColour1Shader;
extern std::uint32_t g_lightingVertexShader;
extern std::uint32_t g_simplePixelShader;
extern std::uint32_t g_fullscreenQuadVertexShader;
extern std::uint32_t g_defaultPixelShader;
extern std::uint32_t g_blurPixelShader;
extern std::uint32_t g_legacyDefaultEffectTexture;
extern std::uint32_t g_fvfPntdsVertexShader;
extern std::uint32_t g_whiteVertexShader;
extern std::uint32_t g_projectedShadowVertexShader;
extern std::uint32_t g_fireVertexShader;
extern std::uint32_t g_fvfPtdPixelShader;
extern std::uint32_t g_fvfPtdVertexShader;
extern std::uint32_t g_fvfPdVertexShader;
extern std::uint32_t g_fvfPdPixelShader;
extern std::uint32_t g_fvfPtdTVertexShader;
extern std::uint32_t g_fvfPdTVertexShader;
extern std::uint32_t g_fvfPdTPixelShader;
extern std::uint32_t g_fvfPtdTPixelShader;
extern std::uint32_t g_stripType0InputLayout;
extern std::uint32_t g_stripType3InputLayout;
extern std::uint32_t g_nuklearInputLayout;
extern std::uint32_t g_nuklearVertexShader;
extern std::uint32_t g_nuklearPixelShader;
extern std::uint32_t texturedQuadVertexShader;
extern std::uint32_t texturedQuadPixelShader;
extern std::uint32_t texturedQuadGammaVertexShader;
extern std::uint32_t texturedQuadGammaPixelShader;
extern std::uint32_t g_emulatorPdVertexShader;
extern std::uint32_t g_emulatorPdPixelShader;
extern std::uint32_t g_emulatorPdtVertexShader;
extern std::uint32_t g_emulatorPdtPixelShader;
extern std::uint32_t g_emulatorPtVertexShader;
extern std::uint32_t g_emulatorPtPixelShader;

std::uint32_t MarkLegacyEffectTextureStateDirty();
void SetupMapRendererCaches();
bool InitRenderer();
std::int32_t AreShadersSetup();
void LoadShadersAndRenderer();
bool HasUnlockedRenderHandle();
void SetupAllShaders();
void InitGeneralTextureResources();
void SetContrastShaderAndDraw();
void ShutdownRendererContent();
std::int32_t LoadMipMap_Wrapper(void *unused);
std::uint32_t QueuePendingMipMapLoads();
void UpdateMipMaps();

void DisableFilmBarAndUpdateFramePacing();
void WaitForNextRenderFrame(std::uint32_t *handles, std::int32_t count);
void SetD3TRenderSettings(std::uint32_t updateUi);
void CameraConfig();

} // namespace shenmue::s2
