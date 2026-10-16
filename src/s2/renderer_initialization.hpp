#pragma once

#include <array>
#include <cstdint>

namespace shenmue::s2 {

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

} // namespace shenmue::s2
