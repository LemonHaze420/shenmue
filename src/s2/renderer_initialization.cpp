#include "s2/renderer_initialization.hpp"
#include "s2/sd_light.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/ui.hpp"
#include "s2/camera.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/renderer_scratch.hpp"
#include "s2/texture_source.hpp"
#include "s2/input_bindings.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/content.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/fvf.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/model_cache.hpp"
#include "d3t/object_store.hpp"
#include "d3t/shader.hpp"
#include "d3t/shader_load.hpp"
#include "d3t/threads.hpp"
#include "d3t/texture.hpp"
#include "d3t/mip_streaming.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_runtime.hpp"

#include <array>
#include <cstring>
#include <initializer_list>

    namespace shenmue::s2 {

namespace {
std::uint32_t legacyEffectTextureStateDirty{};
bool legacyShadersInitialized{};
}

// @verified - p
std::uint32_t MarkLegacyEffectTextureStateDirty() {
    if (!legacyEffectTextureStateDirty)
        legacyEffectTextureStateDirty = 1;
    return legacyEffectTextureStateDirty;
}

std::uint32_t g_currentRenderHandle = 0;
std::uint32_t g_mainModelCache = 0;
std::uint32_t g_sdOverrideModelCache = 0;
std::uint32_t g_skyModelCache = 0;
std::uint32_t g_skyProbeModelCache = 0;
std::int32_t g_doLowMipsOnly = 2;
std::array<std::uint32_t, 40> g_fvfLayouts{};
std::uint32_t g_originalStrip00VertexShader = 0;
std::uint32_t g_originalStrip03VertexShader = 0;
std::uint32_t g_originalStrip00PixelShader = 0;
std::uint32_t g_originalStrip03PixelShader = 0;
std::array<std::uint32_t, 10> g_legacyLambertVertexShaderMap{};
std::uint32_t g_originalConstantVertexShader = 0;
std::uint32_t g_originalVertexColour0Shader = 0;
std::uint32_t g_originalVertexColour1Shader = 0;
std::uint32_t g_lightingVertexShader = 0;
std::uint32_t g_simplePixelShader = 0;
std::uint32_t g_fullscreenQuadVertexShader = 0;
std::uint32_t g_defaultPixelShader = 0;
std::uint32_t g_blurPixelShader = 0;
std::uint32_t g_legacyDefaultEffectTexture = 0;
std::uint32_t g_fvfPntdsVertexShader = 0;
std::uint32_t g_whiteVertexShader = 0;
std::uint32_t g_projectedShadowVertexShader = 0;
std::uint32_t g_fireVertexShader = 0;
std::uint32_t g_fvfPtdPixelShader = 0;
std::uint32_t g_fvfPtdVertexShader = 0;
std::uint32_t g_fvfPdVertexShader = 0;
std::uint32_t g_fvfPdPixelShader = 0;
std::uint32_t g_fvfPtdTVertexShader = 0;
std::uint32_t g_fvfPtdTPixelShader = 0;
std::uint32_t g_fvfPdTVertexShader = 0;
std::uint32_t g_fvfPdTPixelShader = 0;
std::uint32_t g_stripType0InputLayout = 0;
std::uint32_t g_stripType3InputLayout = 0;
std::uint32_t g_nuklearInputLayout = 0;
std::uint32_t g_nuklearVertexShader = 0;
std::uint32_t g_nuklearPixelShader = 0;
std::uint32_t texturedQuadVertexShader = 0;
std::uint32_t texturedQuadPixelShader = 0;
std::uint32_t texturedQuadGammaVertexShader = 0;
std::uint32_t texturedQuadGammaPixelShader = 0;
std::uint32_t g_emulatorPdVertexShader = 0;
std::uint32_t g_emulatorPdPixelShader = 0;
std::uint32_t g_emulatorPdtVertexShader = 0;
std::uint32_t g_emulatorPdtPixelShader = 0;
std::uint32_t g_emulatorPtVertexShader = 0;
std::uint32_t g_emulatorPtPixelShader = 0;

namespace {
std::uint32_t g_mipMapTaskHandle = 0;

struct ElementDefinition {
    const char *name;
    std::uint32_t semanticIndex;
    std::uint32_t format;
    std::uint8_t inputSlot;
    std::uint8_t extraStride;
};

std::uint32_t MakeFVF(std::initializer_list<ElementDefinition> definitions) {
    d3t::FVFDescription description{};
    description.elementCount = static_cast<std::uint32_t>(definitions.size());
    std::size_t index = 0;
    for (const ElementDefinition &definition : definitions) {
        d3t::FVFElement &element = description.elements[index++];
        element.semanticName = definition.name;
        element.semanticIndex = definition.semanticIndex;
        element.format = definition.format;
        element.inputClassification = 0;
        element.inputSlot = definition.inputSlot;
        element.extraStride = definition.extraStride;
        element.semanticHash = d3t::d3t_HashData(definition.name, static_cast<std::uint32_t>(std::strlen(definition.name)));
    }
    return d3t::CreateFVF(&description);
}

std::uint32_t fvfPdTVertexShaderLoad = 0;
std::uint32_t fvfPdTPixelShaderLoad = 0;
std::uint32_t texturedQuadShaderLoad = 0;
std::uint32_t texturedQuadGammaShaderLoad = 0;
std::uint32_t contrastShaderLoad = 0;
std::uint32_t originalStrip00VertexShaderLoad = 0;
std::uint32_t originalStrip03VertexShaderLoad = 0;
std::uint32_t originalStrip00PixelShaderLoad = 0;
std::uint32_t originalStrip03PixelShaderLoad = 0;
std::uint32_t originalLambertVertexShaderLoad = 0;
std::uint32_t originalConstantVertexShaderLoad = 0;
std::uint32_t originalVertexColour0ShaderLoad = 0;
std::uint32_t originalVertexColour1ShaderLoad = 0;
std::uint32_t lightingVertexShaderLoad = 0;
std::uint32_t simplePixelShaderLoad = 0;
std::uint32_t fullscreenQuadVertexShaderLoad = 0;
std::uint32_t defaultPixelShaderLoad = 0;
std::uint32_t blurPixelShaderLoad = 0;
std::uint32_t fvfPntdsVertexShaderLoad = 0;
std::uint32_t fvfPtdVertexShaderLoad = 0;
std::uint32_t whiteVertexShaderLoad = 0;
std::uint32_t projectedShadowVertexShaderLoad = 0;
std::uint32_t fireVertexShaderLoad = 0;
std::uint32_t fvfPtdPixelShaderLoad = 0;
std::uint32_t fvfPtdTVertexShaderLoad = 0;
std::uint32_t fvfPtdTPixelShaderLoad = 0;
std::uint32_t fvfPdVertexShaderLoad = 0;
std::uint32_t fvfPdPixelShaderLoad = 0;
std::uint32_t nuklearShaderLoad = 0;
std::uint32_t simpleShaderLoad = 0;
std::uint32_t emulatorPdShaderLoad = 0;
std::uint32_t emulatorPdtShaderLoad = 0;
std::uint32_t emulatorPtShaderLoad = 0;

std::uint32_t contrastVertexShader = 0;
std::uint32_t contrastPixelShader = 0;

void CreateFVFLayouts(std::uint32_t renderHandle) {
    constexpr ElementDefinition position21{"POSITION", 0, 21, 0, 0};
    constexpr ElementDefinition position21Step4{"POSITION", 0, 21, 0, 4};
    constexpr ElementDefinition position21Step12{"POSITION", 0, 21, 0, 12};
    constexpr ElementDefinition position22{"POSITION", 0, 22, 0, 0};
    constexpr ElementDefinition position20{"POSITION", 0, 20, 0, 0};
    constexpr ElementDefinition normal21{"NORMAL", 0, 21, 0, 0};
    constexpr ElementDefinition diffuse37{"DIFFUSE", 0, 37, 0, 0};
    constexpr ElementDefinition diffuse37Step4{"DIFFUSE", 0, 37, 0, 4};
    constexpr ElementDefinition specular37{"SPECULAR", 0, 37, 0, 0};
    constexpr ElementDefinition texcoord0{"TEXCOORD", 0, 20, 0, 0};
    constexpr ElementDefinition texcoord1{"TEXCOORD", 1, 20, 0, 0};
    constexpr ElementDefinition texcoord2{"TEXCOORD", 2, 20, 0, 0};
    constexpr ElementDefinition texcoord3{"TEXCOORD", 3, 20, 0, 0};

    const std::uint32_t positionNormalTex = MakeFVF({position21, normal21, texcoord0});
    const std::uint32_t steppedPositionTex = MakeFVF({position21Step12, texcoord0});
    const std::uint32_t fullColourTex = MakeFVF({position21Step4, diffuse37, specular37, texcoord0});
    const std::uint32_t colourTex = MakeFVF({position21Step4, diffuse37Step4, texcoord0});
    const std::uint32_t fourTexcoords = MakeFVF({position22, texcoord0, texcoord1, texcoord2, texcoord3});
    const std::uint32_t positionTex = MakeFVF({position21, texcoord0});
    const std::uint32_t fullLegacy = MakeFVF({position21, normal21, diffuse37, specular37, texcoord0});
    const std::uint32_t litLegacy = MakeFVF({position21, diffuse37, specular37, texcoord0});
    const std::uint32_t positionDiffuse = MakeFVF({position21, diffuse37});
    const std::uint32_t positionDiffuseTex = MakeFVF({position21, diffuse37, texcoord0});
    const std::uint32_t position22DiffuseTex = MakeFVF({position22, diffuse37, texcoord0});
    const std::uint32_t position22Diffuse = MakeFVF({position22, diffuse37});
    const std::uint32_t position22Tex = MakeFVF({position22, texcoord0});
    const std::uint32_t position20Tex = MakeFVF({position20, texcoord0});




    g_fvfLayouts.fill(0);
    for (const std::size_t index : {0U, 1U, 4U, 5U, 6U, 13U, 14U, 15U, 16U, 17U, 19U, 25U}) {
        g_fvfLayouts[index] = positionNormalTex;
    }
    g_fvfLayouts[2] = steppedPositionTex;
    g_fvfLayouts[10] = fullColourTex;
    g_fvfLayouts[11] = colourTex;
    g_fvfLayouts[12] = fullColourTex;
    g_fvfLayouts[20] = fourTexcoords;
    g_fvfLayouts[22] = positionTex;
    g_fvfLayouts[23] = fullLegacy;
    g_fvfLayouts[24] = litLegacy;
    g_fvfLayouts[26] = positionDiffuse;
    g_fvfLayouts[27] = positionDiffuseTex;
    g_fvfLayouts[28] = position22DiffuseTex;
    g_fvfLayouts[29] = position22Diffuse;
    g_fvfLayouts[30] = position22Diffuse;
    g_fvfLayouts[31] = position22DiffuseTex;
    g_fvfLayouts[32] = position22Tex;
    g_fvfLayouts[34] = position20Tex;
    g_fvfLayouts[35] = position20Tex;

    fvfPdTVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_pd_t_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    fvfPdTPixelShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_pd_t_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    texturedQuadShaderLoad = d3t::LoadPrecompiledShader("/engine/assets/shaders/texturedquad", renderHandle);
    texturedQuadGammaShaderLoad = d3t::LoadPrecompiledShader("/engine/assets/shaders/texturedquad_gammacorrect", renderHandle);
    contrastShaderLoad = d3t::LoadPrecompiledShader("/shaders/contrast", renderHandle);
}

void LoadOriginalStripShaders(std::uint32_t renderHandle) {
    originalStrip00VertexShaderLoad = d3t::LoadShaderHLSL("/shaders/original_strip00_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    originalStrip03VertexShaderLoad = d3t::LoadShaderHLSL("/shaders/original_strip03_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    originalStrip00PixelShaderLoad = d3t::LoadShaderHLSL("/shaders/original_strip00_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    originalStrip03PixelShaderLoad = d3t::LoadShaderHLSL("/shaders/original_strip03_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
}

void LoadNativeLegacyShaders(std::uint32_t renderHandle) {



    originalLambertVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/original_lamb_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    originalConstantVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/original_const_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    originalVertexColour0ShaderLoad = d3t::LoadShaderHLSL("/shaders/original_vcol0_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    originalVertexColour1ShaderLoad = d3t::LoadShaderHLSL("/shaders/original_vcol1_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    lightingVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/lighting_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    simplePixelShaderLoad = d3t::LoadShaderHLSL("/shaders/simple_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    fullscreenQuadVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fullscreen_quad_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    defaultPixelShaderLoad = d3t::LoadShaderHLSL("/shaders/default_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    blurPixelShaderLoad = d3t::LoadShaderHLSL("/shaders/blur_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    fvfPntdsVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_pntds_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    whiteVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/white_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    projectedShadowVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/sv_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    fireVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fire_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    fvfPtdPixelShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_ptd_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    fvfPtdVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_ptd_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    fvfPtdTVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_ptd_t_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    fvfPtdTPixelShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_ptd_t_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    fvfPdVertexShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_pd_vs.hlsl", renderHandle, d3t::CreateShader(0), d3t::ShaderType::Vertex);
    fvfPdPixelShaderLoad = d3t::LoadShaderHLSL("/shaders/fvf/fvf_pd_ps.hlsl", renderHandle, d3t::CreateShader(1), d3t::ShaderType::Pixel);
    emulatorPdShaderLoad = d3t::LoadPrecompiledShader("/shaders/emu/emu_pd", renderHandle);
    emulatorPdtShaderLoad = d3t::LoadPrecompiledShader("/shaders/emu/emu_pdt", renderHandle);
    emulatorPtShaderLoad = d3t::LoadPrecompiledShader("/shaders/emu/emu_pt", renderHandle);
}

void LoadNuklearBuffer(std::uint32_t renderHandle) {
    g_nuklearInputLayout = MakeFVF({
        {"POSITION", 0, 20, 0, 0},
        {"TEXCOORD", 0, 20, 0, 0},
        {"COLOR", 0, 2, 0, 0},
    });
    nuklearShaderLoad = d3t::LoadPrecompiledShader("/engine/assets/shaders/nuklear", renderHandle);
}

void LoadD3TGenericShaders() {
    g_fvfPdTVertexShader = d3t::GetLoadedShader(fvfPdTVertexShaderLoad);
    g_fvfPdTPixelShader = d3t::GetLoadedShader(fvfPdTPixelShaderLoad);
    texturedQuadVertexShader = d3t::GetLoadedShader(texturedQuadShaderLoad, 0);
    texturedQuadPixelShader = d3t::GetLoadedShader(texturedQuadShaderLoad, 1);
    texturedQuadGammaVertexShader = d3t::GetLoadedShader(texturedQuadGammaShaderLoad, 0);
    texturedQuadGammaPixelShader = d3t::GetLoadedShader(texturedQuadGammaShaderLoad, 1);
    contrastVertexShader = d3t::GetLoadedShader(contrastShaderLoad, 0);
    contrastPixelShader = d3t::GetLoadedShader(contrastShaderLoad, 1);
}

void InitStripShaders() {
    g_stripType0InputLayout = MakeFVF({
        {"POSITION", 0, 22, 0, 0},
        {"DIFFUSE", 0, 2, 0, 0},
    });
    g_stripType3InputLayout = MakeFVF({
        {"POSITION", 0, 22, 0, 0},
        {"DIFFUSE", 0, 2, 0, 0},
        {"SPECULAR", 0, 2, 0, 0},
        {"TEXCOORD", 0, 20, 0, 0},
    });
    g_originalStrip00VertexShader = d3t::GetLoadedShader(originalStrip00VertexShaderLoad);
    g_originalStrip03VertexShader = d3t::GetLoadedShader(originalStrip03VertexShaderLoad);
    g_originalStrip00PixelShader = d3t::GetLoadedShader(originalStrip00PixelShaderLoad);
    g_originalStrip03PixelShader = d3t::GetLoadedShader(originalStrip03PixelShaderLoad);
}

void InitNativeLegacyShaderReferences() {
    const std::uint32_t lambert = d3t::GetLoadedShader(originalLambertVertexShaderLoad);
    g_originalConstantVertexShader = d3t::GetLoadedShader(originalConstantVertexShaderLoad);
    g_originalVertexColour0Shader = d3t::GetLoadedShader(originalVertexColour0ShaderLoad);
    g_originalVertexColour1Shader = d3t::GetLoadedShader(originalVertexColour1ShaderLoad);
    g_lightingVertexShader = d3t::GetLoadedShader(lightingVertexShaderLoad);
    g_simplePixelShader = d3t::GetLoadedShader(simplePixelShaderLoad);
    g_fullscreenQuadVertexShader = d3t::GetLoadedShader(fullscreenQuadVertexShaderLoad);
    g_defaultPixelShader = d3t::GetLoadedShader(defaultPixelShaderLoad);
    g_blurPixelShader = d3t::GetLoadedShader(blurPixelShaderLoad);
    g_fvfPntdsVertexShader = d3t::GetLoadedShader(fvfPntdsVertexShaderLoad);
    g_whiteVertexShader = d3t::GetLoadedShader(whiteVertexShaderLoad);
    g_projectedShadowVertexShader = d3t::GetLoadedShader(projectedShadowVertexShaderLoad);
    g_fireVertexShader = d3t::GetLoadedShader(fireVertexShaderLoad);
    g_fvfPtdPixelShader = d3t::GetLoadedShader(fvfPtdPixelShaderLoad);
    g_fvfPtdVertexShader = d3t::GetLoadedShader(fvfPtdVertexShaderLoad);
    g_fvfPtdTVertexShader = d3t::GetLoadedShader(fvfPtdTVertexShaderLoad);
    g_fvfPtdTPixelShader = d3t::GetLoadedShader(fvfPtdTPixelShaderLoad);
    g_fvfPdVertexShader = d3t::GetLoadedShader(fvfPdVertexShaderLoad);
    g_fvfPdPixelShader = d3t::GetLoadedShader(fvfPdPixelShaderLoad);
    g_emulatorPdVertexShader = d3t::GetLoadedShader(emulatorPdShaderLoad, 0);
    g_emulatorPdPixelShader = d3t::GetLoadedShader(emulatorPdShaderLoad, 1);
    g_emulatorPdtVertexShader = d3t::GetLoadedShader(emulatorPdtShaderLoad, 0);
    g_emulatorPdtPixelShader = d3t::GetLoadedShader(emulatorPdtShaderLoad, 1);
    g_emulatorPtVertexShader = d3t::GetLoadedShader(emulatorPtShaderLoad, 0);
    g_emulatorPtPixelShader = d3t::GetLoadedShader(emulatorPtShaderLoad, 1);




    g_legacyLambertVertexShaderMap = {lambert, lambert, g_originalConstantVertexShader, 0, lambert, lambert, lambert, 0, 0, g_whiteVertexShader};
}

} // namespace
std::int32_t LoadMipMap_Wrapper(void *) {
    if (d3t::g_pendingMipMapRequestCount != 0 && d3t::g_pendingContentRemappingCount == 0 && !framePacingTimerUpdated)
        d3t::LoadMipMaps();
    return 0;
}

std::uint32_t QueuePendingMipMapLoads() {
    if (d3t::d3t_IsTaskActive(g_mipMapTaskHandle))
        return g_mipMapTaskHandle;
    const d3t::D3TTaskDescriptor task{.data = nullptr, .callback = LoadMipMap_Wrapper, .completion = nullptr, .priority = 4};
    g_mipMapTaskHandle = d3t::d3t_QueueTask(&task, 4);
    return g_mipMapTaskHandle;
}

void UpdateMipMaps() {
    if (d3t::g_pendingMipMapRequestCount != 0 && d3t::g_pendingContentRemappingCount == 0 && !framePacingTimerUpdated)
        QueuePendingMipMapLoads();
    d3t::g_mipMapFrameCounters.fill(0);
}

// Map admission owns the global sweep, not HIKI's local reset routines.
void SetupMapRendererCaches() {
    d3t::ResetObjectStoreForMap();
    SweepEffectTextureSources();
    d3t::ReleaseModelCache(g_mainModelCache);
    g_mainModelCache = d3t::AllocateModelCache("Main");
    d3t::ReleaseModelCache(g_sdOverrideModelCache);
    g_sdOverrideModelCache = d3t::AllocateModelCache("SD Override");
    NormalizeMapFramePacing();
}

void InitGeneralTextureResources() {
    const S2UserConfiguration280 &configuration = GetRuntimeUserConfiguration();
    const float sampling = configuration.superSampling + 1.0f;
    const float scale = configuration.resolutionScale * 0.60000002f + 0.40000001f;
    const float width = static_cast<float>(d3t::g_originalScreenWidth) * scale;
    const float height = static_cast<float>(d3t::g_originalScreenHeight) * scale;
    const d3t::SceneFrameTargets targets{.width = static_cast<std::uint32_t>((static_cast<std::int32_t>(width * sampling) + 7) & ~7),
        .height = static_cast<std::uint32_t>((static_cast<std::int32_t>(height * sampling) + 7) & ~7),
        .resolvedWidth = static_cast<std::uint32_t>((static_cast<std::int32_t>(width) + 7) & ~7),
        .resolvedHeight = static_cast<std::uint32_t>((static_cast<std::int32_t>(height) + 7) & ~7),
        .legacyWidth = ((480U * d3t::g_originalScreenWidth / d3t::g_originalScreenHeight) + 7U) & ~7U,
        .legacyHeight = 480,
        .supersampling = sampling > 1.0f};
    d3t::ConfigureSceneFrameTargets(targets, g_fvfLayouts[34], texturedQuadVertexShader, texturedQuadPixelShader);
    d3t::SelectResolvedSceneFrame(configuration.enableSuperSampling != 0);
}

void SetContrastShaderAndDraw() {
    d3t::DrawFullscreenContrast(contrastVertexShader, contrastPixelShader, g_s2CurrentContrast);
}

// @verified - p
bool HasUnlockedRenderHandle() {
    if (!d3t::d3t_IsRenderHandlePending(g_currentRenderHandle))
        d3t::d3t_CloseRenderHandle(g_currentRenderHandle);
    return g_currentRenderHandle == 0;
}

void SetupAllShaders() {
    if (legacyShadersInitialized)
        return;
    if (!d3t::d3t_IsRenderHandlePending(g_currentRenderHandle))
        d3t::d3t_CloseRenderHandle(g_currentRenderHandle);
    const bool hasRenderHandle = g_currentRenderHandle != 0;
    if (hasRenderHandle)
        DisableFilmBarAndUpdateFramePacing();
    WaitForNextRenderFrame(&g_currentRenderHandle, 1);
    d3t::d3t_CloseRenderHandle(g_currentRenderHandle);

    InitNativeLegacyShaderReferences();
    constexpr std::array<std::uint32_t, 4> whitePixels{0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU};
    const d3t::Texture2DDescription whiteDescription{2, 2, 1, 1, 2, 0, 0, 1, 0, 0, whitePixels.data()};
    g_legacyDefaultEffectTexture = d3t::CreateTexture2D(&whiteDescription);
    if (!g_legacyDefaultEffectTexture)
        return;
    InitializeLegacyDrawState();
    if (hasRenderHandle)
        SetD3TRenderSettings(0);
    legacyShadersInitialized = true;
}

bool InitRenderer() {
    SetFramePacingRendererInitialized(false);
    g_currentRenderHandle = d3t::d3t_AllocateRenderHandle();
    if (!g_currentRenderHandle)
        return false;

    CreateFVFLayouts(g_currentRenderHandle);
    LoadOriginalStripShaders(g_currentRenderHandle);
    LoadNativeLegacyShaders(g_currentRenderHandle);
    QueueSDTextureOverride("/textureOverride/SDTextureOverride.json", g_currentRenderHandle);
    InitializeNuklearUI(g_currentRenderHandle);
    LoadNuklearBuffer(g_currentRenderHandle);
    while (d3t::d3t_IsRenderHandlePending(g_currentRenderHandle)) {
        d3t::ProcessRemappings();
    }
    d3t::d3t_CloseRenderHandle(g_currentRenderHandle);

    LoadD3TGenericShaders();
    InitStripShaders();
    SetupAllShaders();
    if (!g_legacyDefaultEffectTexture)
        return false;
    g_nuklearVertexShader = d3t::GetLoadedShader(nuklearShaderLoad, 0);
    g_nuklearPixelShader = d3t::GetLoadedShader(nuklearShaderLoad, 1);
    InitGeneralTextureResources();
    SetupNuklear();
    if (!d3t::InitializeUIRenderer(
            g_nuklearInputLayout, g_nuklearVertexShader, g_nuklearPixelShader, g_fvfLayouts[35], texturedQuadGammaVertexShader, texturedQuadGammaPixelShader)) {
        return false;
    }

    InitializeSDLightMetadata();
    std::uint32_t simpleRenderHandle = d3t::d3t_AllocateRenderHandle();
    simpleShaderLoad = d3t::LoadPrecompiledShader("/engine/assets/shaders/simple", simpleRenderHandle);
    if (!simpleShaderLoad) {
        d3t::d3t_CloseRenderHandle(simpleRenderHandle);
        return false;
    }
    DisableFilmBarAndUpdateFramePacing();
    WaitForNextRenderFrame(&simpleRenderHandle, 1);
    d3t::d3t_CloseRenderHandle(simpleRenderHandle);
    SetD3TRenderSettings(0);
    SetFramePacingRendererInitialized(true);
    return true;
}

void DisableFilmBarAndUpdateFramePacing() {
    camCtrlSetFilmBarEnabled(0);
    shenSetForcedFilmbars(1);
    UpdateFramePacing();
}

void WaitForNextRenderFrame(std::uint32_t *handles, std::int32_t count) {
    WaitForNextFrameWithCallback(handles, count, GetTotalFramesToRender);
}

void SetD3TRenderSettings(std::uint32_t updateUi) {
    UpdateCurrentFramesRendered(updateUi);
    shenSetForcedFilmbars(0);
}

void ShutdownRendererContent() {
    SetFramePacingRendererInitialized(false);
    RetireCollectionMenuResources();
    ReleaseSDTextureOverrides();
    ReleaseLegacyPostProcessingTargets();
    if (g_legacyDefaultEffectTexture) {
        d3t::InitTexture2DResource(g_legacyDefaultEffectTexture);
        g_legacyDefaultEffectTexture = 0;
    }
    ReleaseSDLightMetadata();
    d3t::ReleaseSceneFrameTargets();
    d3t::ShutdownThreads();
    FreeRendererScratch();
}

} // namespace shenmue::s2
