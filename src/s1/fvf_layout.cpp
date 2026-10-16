#include "s1/fvf_layout.hpp"

#include "s1/archive_data.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/shader_initialization.hpp"

#include <array>
#include <cstring>
#include <initializer_list>

namespace shenmue::s1 {

std::array<std::uint32_t, 40> g_fvfLayouts{};
std::uint32_t g_originalStrip00InputLayout = 0;
std::uint32_t g_originalStrip03InputLayout = 0;

namespace {

struct ElementDefinition {
    const char *name;
    std::uint32_t semanticIndex;
    std::uint32_t format;
    std::uint8_t inputSlot;
    std::uint8_t extraStride;
};

D3TFVFDescription makeDescription(std::initializer_list<ElementDefinition> definitions) {
    D3TFVFDescription description{};
    description.elementCount = static_cast<std::uint32_t>(definitions.size());
    std::size_t index = 0;
    for (const ElementDefinition &definition : definitions) {
        D3TFVFElement &element = description.elements[index++];
        element.semanticName = definition.name;
        element.semanticIndex = definition.semanticIndex;
        element.format = definition.format;
        element.inputClassification = 0;
        element.inputSlot = definition.inputSlot;
        element.extraStride = definition.extraStride;
        element.semanticHash = d3t_HashData(definition.name, static_cast<std::uint32_t>(std::strlen(definition.name)));
    }
    return description;
}

std::uint32_t create(std::initializer_list<ElementDefinition> definitions) {
    const D3TFVFDescription description = makeDescription(definitions);
    return CreateFVF(&description);
}

} // namespace

std::int64_t CreateFVFLayouts() {
    constexpr ElementDefinition position21{"POSITION", 0, 21, 0, 0};
    constexpr ElementDefinition position21Step4{"POSITION", 0, 21, 0, 4};
    constexpr ElementDefinition position21Step12{"POSITION", 0, 21, 0, 12};
    constexpr ElementDefinition position22{"POSITION", 0, 22, 0, 0};
    constexpr ElementDefinition position20{"POSITION", 0, 20, 0, 0};
    constexpr ElementDefinition normal21{"NORMAL", 0, 21, 0, 0};
    constexpr ElementDefinition diffuse37{"DIFFUSE", 0, 37, 0, 0};
    constexpr ElementDefinition diffuse37Step4{"DIFFUSE", 0, 37, 0, 4};
    constexpr ElementDefinition diffuse22_0{"DIFFUSE", 0, 22, 0, 0};
    constexpr ElementDefinition diffuse22_1{"DIFFUSE", 1, 22, 0, 0};
    constexpr ElementDefinition specular37{"SPECULAR", 0, 37, 0, 0};
    constexpr ElementDefinition texcoord0{"TEXCOORD", 0, 20, 0, 0};
    constexpr ElementDefinition texcoord1{"TEXCOORD", 1, 20, 0, 0};
    constexpr ElementDefinition texcoord2{"TEXCOORD", 2, 20, 0, 0};
    constexpr ElementDefinition texcoord3{"TEXCOORD", 3, 20, 0, 0};

    const std::uint32_t positionNormalTex = create({position21, normal21, texcoord0});
    const std::uint32_t steppedPositionTex = create({position21Step12, texcoord0});
    const std::uint32_t fullColourTex = create({position21Step4, diffuse37, specular37, texcoord0});
    const std::uint32_t colourTex = create({position21Step4, diffuse37Step4, texcoord0});
    const std::uint32_t fourTexcoords = create({position22, texcoord0, texcoord1, texcoord2, texcoord3});
    const std::uint32_t positionTex = create({position21, texcoord0});
    const std::uint32_t legacyLit = create({position21, normal21, texcoord0, diffuse22_0, diffuse22_1});
    const std::uint32_t positionOnly = create({position21});
    const std::uint32_t fullLegacy = create({position21, normal21, diffuse37, specular37, texcoord0});
    const std::uint32_t litLegacy = create({position21, diffuse37, specular37, texcoord0});
    const std::uint32_t positionDiffuse = create({position21, diffuse37});
    const std::uint32_t positionDiffuseTex = create({position21, diffuse37, texcoord0});
    const std::uint32_t position22DiffuseTex = create({position22, diffuse37, texcoord0});
    const std::uint32_t position22Diffuse = create({position22, diffuse37});
    const std::uint32_t position22Tex = create({position22, texcoord0});
    const std::uint32_t position20Tex = create({position20, texcoord0});

    for (const std::size_t index : {0U, 1U, 4U, 5U, 6U, 14U, 15U, 16U, 17U, 19U, 25U}) {
        g_fvfLayouts[index] = positionNormalTex;
    }
    g_fvfLayouts[2] = steppedPositionTex;
    g_fvfLayouts[7] = legacyLit;
    g_fvfLayouts[10] = fullColourTex;
    g_fvfLayouts[11] = fourTexcoords;
    g_fvfLayouts[12] = fullColourTex;
    g_fvfLayouts[13] = positionOnly;
    g_fvfLayouts[20] = positionTex;
    g_fvfLayouts[22] = legacyLit;
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
    g_fvfLayouts[39] = legacyLit;



    static_cast<void>(colourTex);

    const std::uint32_t renderHandle = g_activeRenderHandle;
    g_fvfPdTVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pd_t_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fvfPdTPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pd_t_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_texturedQuadShaderLoad = LoadPrecompiledShader("/engine/assets/shaders/texturedquad", renderHandle);
    g_texturedQuadGammaShaderLoad = LoadPrecompiledShader("/engine/assets/shaders/texturedquad_gammacorrect", renderHandle);
    g_abufferWriteShaderLoad = LoadPrecompiledShader("/shaders/abuffer_write", renderHandle);
    g_abufferCompositionShaderLoad = LoadPrecompiledShader("/shaders/abuffer_comp", renderHandle);
    g_abufferClearShaderLoad = LoadPrecompiledShader("/shaders/abuffer_clear", renderHandle);
    g_contrastShaderLoad = LoadPrecompiledShader("/shaders/contrast", renderHandle);
    return g_contrastShaderLoad;
}

std::int64_t Shenmue_InitShaders() {
    const std::uint32_t strip00Layout = create({
        {"POSITION", 0, 22, 0, 0},
        {"DIFFUSE", 0, 2, 0, 0},
    });
    const std::uint32_t strip03Layout = create({
        {"POSITION", 0, 22, 0, 0},
        {"DIFFUSE", 0, 2, 0, 0},
        {"SPECULAR", 0, 2, 0, 0},
        {"TEXCOORD", 0, 20, 0, 0},
    });

    g_originalStrip00VertexShader = d3t_GetLoadedShader(g_originalStrip00VertexShaderLoad);
    g_originalStrip03VertexShader = d3t_GetLoadedShader(g_originalStrip03VertexShaderLoad);
    g_originalStrip00PixelShader = d3t_GetLoadedShader(g_originalStrip00PixelShaderLoad);
    g_originalStrip03PixelShader = d3t_GetLoadedShader(g_originalStrip03PixelShaderLoad);
    g_lightingVcolVertexShader = d3t_GetLoadedShader(g_lightingVcolVertexShaderLoad);
    g_simplePixelShader = d3t_GetLoadedShader(g_simplePixelShaderLoad);
    g_originalStrip00InputLayout = strip00Layout;
    g_originalStrip03InputLayout = strip03Layout;
    return strip03Layout;
}

} // namespace shenmue::s1
