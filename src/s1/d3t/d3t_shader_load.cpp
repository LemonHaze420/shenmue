#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/namespace.hpp"

#include "s1/fvf_layout.hpp"
#include "s1/shader_initialization.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

std::uint32_t g_originalStrip00VertexShaderLoad = 0;
std::uint32_t g_originalStrip03VertexShaderLoad = 0;
std::uint32_t g_originalStrip00PixelShaderLoad = 0;
std::uint32_t g_originalStrip03PixelShaderLoad = 0;
std::uint32_t g_fvfPtdTVertexShaderLoad = 0;
std::uint32_t g_fvfPtdTPixelShaderLoad = 0;
std::uint32_t g_originalStrip00VertexShader = 0;
std::uint32_t g_originalStrip03VertexShader = 0;
std::uint32_t g_originalStrip00PixelShader = 0;
std::uint32_t g_originalStrip03PixelShader = 0;
std::uint32_t g_fvfPtdTVertexShader = 0;
std::uint32_t g_fvfPtdTPixelShader = 0;
std::uint32_t g_lightingVcolVertexShaderLoad = 0;
std::uint32_t g_simplePixelShaderLoad = 0;
std::uint32_t g_lightingVcolVertexShader = 0;
std::uint32_t g_simplePixelShader = 0;
std::uint32_t g_originalLambVertexShaderLoad = 0;
std::uint32_t g_originalConstVertexShaderLoad = 0;
std::uint32_t g_originalVcol0VertexShaderLoad = 0;
std::uint32_t g_originalVcol1VertexShaderLoad = 0;
std::uint32_t g_lightingVertexShaderLoad = 0;
std::uint32_t g_originalLambVertexShader = 0;
std::uint32_t g_originalConstVertexShader = 0;
std::uint32_t g_originalVcol0VertexShader = 0;
std::uint32_t g_originalVcol1VertexShader = 0;
std::uint32_t g_lightingVertexShader = 0;
std::array<std::uint32_t, 64> g_originalLambVertexShaders{};
std::array<std::uint32_t, 64> g_simplePixelShaders{};

std::uint32_t g_fvfPdTVertexShaderLoad = 0;
std::uint32_t g_fvfPdTPixelShaderLoad = 0;
std::uint32_t g_texturedQuadShaderLoad = 0;
std::uint32_t g_texturedQuadGammaShaderLoad = 0;
std::uint32_t g_abufferWriteShaderLoad = 0;
std::uint32_t g_abufferCompositionShaderLoad = 0;
std::uint32_t g_abufferClearShaderLoad = 0;
std::uint32_t g_contrastShaderLoad = 0;

std::uint32_t g_fvfPdTVertexShader = 0;
std::uint32_t g_fvfPdTPixelShader = 0;
std::uint32_t g_texturedQuadVertexShader = 0;
std::uint32_t g_texturedQuadPixelShader = 0;
std::uint32_t g_texturedQuadGammaVertexShader = 0;
std::uint32_t g_texturedQuadGammaPixelShader = 0;
std::uint32_t g_abufferWriteVertexShader = 0;
std::uint32_t g_abufferWritePixelShader = 0;
std::uint32_t g_abufferCompositionVertexShader = 0;
std::uint32_t g_abufferCompositionPixelShader = 0;
std::uint32_t g_abufferClearVertexShader = 0;
std::uint32_t g_abufferClearPixelShader = 0;
std::uint32_t g_contrastVertexShader = 0;
std::uint32_t g_contrastPixelShader = 0;

namespace {


std::uint32_t g_fvfPdVertexShaderLoad = 0;
std::uint32_t g_fvfPntVertexShaderLoad = 0;
std::uint32_t g_fvfPntdsVertexShaderLoad = 0;
std::uint32_t g_fvfPtdVertexShaderLoad = 0;
std::uint32_t g_fvfPtdsVertexShaderLoad = 0;
std::uint32_t g_fvfPdPixelShaderLoad = 0;
std::uint32_t g_fvfPntPixelShaderLoad = 0;
std::uint32_t g_fvfPntdsPixelShaderLoad = 0;
std::uint32_t g_fvfPtdPixelShaderLoad = 0;
std::uint32_t g_fvfPtdsPixelShaderLoad = 0;
std::uint32_t g_shadowProjectionVertexShaderLoad = 0;
std::uint32_t g_shadowVolumeVertexShaderLoad = 0;
std::uint32_t g_shadowProjectionPixelShaderLoad = 0;
std::uint32_t g_debugDrawVertexShaderLoad = 0;
std::uint32_t g_fullscreenQuadVertexShaderLoad = 0;
std::uint32_t g_fireVertexShaderLoad = 0;
std::uint32_t g_whiteVertexShaderLoad = 0;
std::uint32_t g_debugDrawPixelShaderLoad = 0;
std::uint32_t g_cafPixelShaderLoad = 0;
std::uint32_t g_cafScreenPixelShaderLoad = 0;
std::uint32_t g_msfPixelShaderLoad = 0;
std::uint32_t g_grayPixelShaderLoad = 0;
std::uint32_t g_defaultPixelShaderLoad = 0;
std::uint32_t g_blurPixelShaderLoad = 0;
std::uint32_t g_emuPdShaderLoad = 0;
std::uint32_t g_emuPdtShaderLoad = 0;
std::uint32_t g_emuPtShaderLoad = 0;

} // namespace

std::array<std::uint32_t, 10> g_unlitVertexShaderSet{};
std::array<std::uint32_t, 10> g_litVertexShaderSet{};
std::array<std::uint32_t, 10> g_unlitInputLayoutSet{};
std::array<std::uint32_t, 10> g_litInputLayoutSet{};
std::uint32_t g_vcol0VertexShaderReference = 0;
const std::uint32_t *g_currentVertexShaderSet = nullptr;
const std::uint32_t *g_currentInputLayoutSet = nullptr;

namespace d3t {

void LoadOriginalStripShaders() {
    const std::uint32_t renderHandle = g_activeRenderHandle;
    g_originalStrip00VertexShaderLoad =
        QueueShaderCompileFromHLSL("/shaders/original_strip00_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_originalStrip03VertexShaderLoad =
        QueueShaderCompileFromHLSL("/shaders/original_strip03_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_originalStrip00PixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/original_strip00_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_originalStrip03PixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/original_strip03_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
}



void LoadShaders() {
    const std::uint32_t renderHandle = g_activeRenderHandle;
    g_debugDrawVertexShaderLoad =
        QueueShaderCompileFromHLSL("/engine/assets/shaders/DebugDraw_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_originalLambVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/original_lamb_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_originalConstVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/original_const_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_originalVcol0VertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/original_vcol0_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_originalVcol1VertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/original_vcol1_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_lightingVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/lighting_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fullscreenQuadVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fullscreen_quad_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_shadowVolumeVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/sv_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fireVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fire_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_debugDrawPixelShaderLoad =
        QueueShaderCompileFromHLSL("/engine/assets/shaders/DebugDraw_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_simplePixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/simple_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_cafPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/caf_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_cafScreenPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/caf_screen_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_msfPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/msf_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_grayPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/gray_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_defaultPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/default_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_blurPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/blur_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPdVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pd_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fvfPntVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pnt_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fvfPntdsVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pntds_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fvfPtdVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_ptd_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fvfPtdTVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_ptd_t_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_fvfPtdsVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_ptds_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_lightingVcolVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/lighting_vcol_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_whiteVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/white_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_shadowProjectionVertexShaderLoad = QueueShaderCompileFromHLSL("/shaders/shadow_proj_vs.hlsl", renderHandle, d3t_CreateShader(0), D3TShaderType::Vertex);
    g_shadowProjectionPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/shadow_proj_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPdPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pd_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPntPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pnt_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPntdsPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_pntds_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPtdPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_ptd_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPtdTPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_ptd_t_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_fvfPtdsPixelShaderLoad = QueueShaderCompileFromHLSL("/shaders/fvf/fvf_ptds_ps.hlsl", renderHandle, d3t_CreateShader(1), D3TShaderType::Pixel);
    g_emuPdShaderLoad = LoadPrecompiledShader("/shaders/emu/emu_pd", renderHandle);
    g_emuPdtShaderLoad = LoadPrecompiledShader("/shaders/emu/emu_pdt", renderHandle);
    g_emuPtShaderLoad = LoadPrecompiledShader("/shaders/emu/emu_pt", renderHandle);
}
} // namespace d3t


void LoadD3TGenericShaders() {
    g_fvfPdTVertexShader = d3t_GetLoadedShader(g_fvfPdTVertexShaderLoad, 0);
    g_fvfPdTPixelShader = d3t_GetLoadedShader(g_fvfPdTPixelShaderLoad, 0);
    g_texturedQuadVertexShader = d3t_GetLoadedShader(g_texturedQuadShaderLoad, 0);
    g_texturedQuadPixelShader = d3t_GetLoadedShader(g_texturedQuadShaderLoad, 1);
    g_texturedQuadGammaVertexShader = d3t_GetLoadedShader(g_texturedQuadGammaShaderLoad, 0);
    g_texturedQuadGammaPixelShader = d3t_GetLoadedShader(g_texturedQuadGammaShaderLoad, 1);
    g_abufferWriteVertexShader = d3t_GetLoadedShader(g_abufferWriteShaderLoad, 0);
    g_abufferWritePixelShader = d3t_GetLoadedShader(g_abufferWriteShaderLoad, 1);
    g_abufferCompositionVertexShader = d3t_GetLoadedShader(g_abufferCompositionShaderLoad, 0);
    g_abufferCompositionPixelShader = d3t_GetLoadedShader(g_abufferCompositionShaderLoad, 1);
    g_abufferClearVertexShader = d3t_GetLoadedShader(g_abufferClearShaderLoad, 0);
    g_abufferClearPixelShader = d3t_GetLoadedShader(g_abufferClearShaderLoad, 1);
    g_contrastVertexShader = d3t_GetLoadedShader(g_contrastShaderLoad, 0);
    g_contrastPixelShader = d3t_GetLoadedShader(g_contrastShaderLoad, 1);
    g_originalLambVertexShaders[29] = g_fvfPdTVertexShader;
    g_simplePixelShaders[13] = g_fvfPdTPixelShader;
    g_originalLambVertexShaders[34] = g_texturedQuadVertexShader;
    g_simplePixelShaders[18] = g_texturedQuadPixelShader;
    g_originalLambVertexShaders[35] = g_texturedQuadGammaVertexShader;
    g_simplePixelShaders[19] = g_texturedQuadGammaPixelShader;
    g_originalLambVertexShaders[36] = g_abufferWriteVertexShader;
    g_simplePixelShaders[20] = g_abufferWritePixelShader;
    g_originalLambVertexShaders[37] = g_abufferCompositionVertexShader;
    g_simplePixelShaders[21] = g_abufferCompositionPixelShader;
    g_originalLambVertexShaders[38] = g_abufferClearVertexShader;
    g_simplePixelShaders[22] = g_abufferClearPixelShader;
    g_originalLambVertexShaders[40] = g_contrastVertexShader;
    g_simplePixelShaders[25] = g_contrastPixelShader;
}



void AssignShaderReferences() {
    g_originalLambVertexShader = d3t_GetLoadedShader(g_originalLambVertexShaderLoad, 0);
    g_originalConstVertexShader = d3t_GetLoadedShader(g_originalConstVertexShaderLoad, 0);
    g_originalVcol0VertexShader = d3t_GetLoadedShader(g_originalVcol0VertexShaderLoad, 0);
    g_originalVcol1VertexShader = d3t_GetLoadedShader(g_originalVcol1VertexShaderLoad, 0);
    g_lightingVertexShader = d3t_GetLoadedShader(g_lightingVertexShaderLoad, 0);
    g_lightingVcolVertexShader = d3t_GetLoadedShader(g_lightingVcolVertexShaderLoad, 0);
    g_simplePixelShader = d3t_GetLoadedShader(g_simplePixelShaderLoad, 0);
    g_fvfPtdTVertexShader = d3t_GetLoadedShader(g_fvfPtdTVertexShaderLoad, 0);
    g_fvfPtdTPixelShader = d3t_GetLoadedShader(g_fvfPtdTPixelShaderLoad, 0);

    g_simplePixelShaders[0] = g_simplePixelShader;
    g_simplePixelShaders[1] = d3t_GetLoadedShader(g_defaultPixelShaderLoad, 0);
    g_simplePixelShaders[2] = d3t_GetLoadedShader(g_cafPixelShaderLoad, 0);
    g_simplePixelShaders[3] = d3t_GetLoadedShader(g_cafScreenPixelShaderLoad, 0);
    g_simplePixelShaders[4] = d3t_GetLoadedShader(g_grayPixelShaderLoad, 0);
    g_simplePixelShaders[5] = d3t_GetLoadedShader(g_msfPixelShaderLoad, 0);
    g_simplePixelShaders[6] = d3t_GetLoadedShader(g_blurPixelShaderLoad, 0);
    g_simplePixelShaders[7] = d3t_GetLoadedShader(g_fvfPntdsPixelShaderLoad, 0);
    g_simplePixelShaders[8] = d3t_GetLoadedShader(g_fvfPtdsPixelShaderLoad, 0);
    g_simplePixelShaders[9] = d3t_GetLoadedShader(g_fvfPntPixelShaderLoad, 0);
    g_simplePixelShaders[10] = d3t_GetLoadedShader(g_fvfPdPixelShaderLoad, 0);
    g_simplePixelShaders[11] = d3t_GetLoadedShader(g_fvfPtdPixelShaderLoad, 0);
    g_simplePixelShaders[12] = g_fvfPtdTPixelShader;
    g_simplePixelShaders[17] = d3t_GetLoadedShader(g_debugDrawPixelShaderLoad, 0);

    for (const std::size_t index : {0U, 1U, 4U, 5U, 6U, 14U, 15U, 16U, 17U, 19U})
        g_originalLambVertexShaders[index] = g_originalLambVertexShader;
    g_originalLambVertexShaders[2] = g_originalConstVertexShader;
    g_originalLambVertexShaders[10] = g_originalVcol0VertexShader;
    g_originalLambVertexShaders[11] = g_originalVcol1VertexShader;
    g_originalLambVertexShaders[12] = g_originalVcol0VertexShader;

    g_originalLambVertexShaders[13] = d3t_GetLoadedShader(g_shadowVolumeVertexShaderLoad, 0);
    g_originalLambVertexShaders[20] = d3t_GetLoadedShader(g_fullscreenQuadVertexShaderLoad, 0);
    g_originalLambVertexShaders[21] = g_lightingVertexShader;
    g_originalLambVertexShaders[22] = d3t_GetLoadedShader(g_fireVertexShaderLoad, 0);
    g_originalLambVertexShaders[33] = d3t_GetLoadedShader(g_debugDrawVertexShaderLoad, 0);
    g_originalLambVertexShaders[8] = d3t_GetLoadedShader(g_whiteVertexShaderLoad, 0);
    g_originalLambVertexShaders[23] = d3t_GetLoadedShader(g_fvfPntdsVertexShaderLoad, 0);
    g_originalLambVertexShaders[24] = d3t_GetLoadedShader(g_fvfPtdsVertexShaderLoad, 0);
    g_originalLambVertexShaders[25] = d3t_GetLoadedShader(g_fvfPntVertexShaderLoad, 0);
    g_originalLambVertexShaders[26] = d3t_GetLoadedShader(g_fvfPdVertexShaderLoad, 0);
    g_originalLambVertexShaders[27] = d3t_GetLoadedShader(g_fvfPtdVertexShaderLoad, 0);
    g_originalLambVertexShaders[28] = g_fvfPtdTVertexShader;
    g_originalLambVertexShaders[30] = d3t_GetLoadedShader(g_emuPdShaderLoad, 0);
    g_simplePixelShaders[14] = d3t_GetLoadedShader(g_emuPdShaderLoad, 1);
    g_originalLambVertexShaders[31] = d3t_GetLoadedShader(g_emuPdtShaderLoad, 0);
    g_simplePixelShaders[15] = d3t_GetLoadedShader(g_emuPdtShaderLoad, 1);
    g_originalLambVertexShaders[32] = d3t_GetLoadedShader(g_emuPtShaderLoad, 0);
    g_simplePixelShaders[16] = d3t_GetLoadedShader(g_emuPtShaderLoad, 1);


    g_originalLambVertexShaders[7] = g_lightingVcolVertexShader;
    g_originalLambVertexShaders[39] = d3t_GetLoadedShader(g_shadowProjectionVertexShaderLoad, 0);
    g_simplePixelShaders[24] = 0;
    g_simplePixelShaders[23] = d3t_GetLoadedShader(g_shadowProjectionPixelShaderLoad, 0);
}




void AssignShaderReferences_0() {
    g_vcol0VertexShaderReference = g_originalLambVertexShaders[10];
    g_unlitVertexShaderSet = {g_originalLambVertexShaders[0], g_originalLambVertexShaders[1], g_originalLambVertexShaders[2], 0, g_originalLambVertexShaders[4],
        g_originalLambVertexShaders[5], g_originalLambVertexShaders[6], g_originalLambVertexShaders[7], g_originalLambVertexShaders[8], 0};
    g_litVertexShaderSet = {g_originalLambVertexShaders[21], g_originalLambVertexShaders[1], g_originalLambVertexShaders[2], 0, g_originalLambVertexShaders[16],
        g_originalLambVertexShaders[16], g_originalLambVertexShaders[6], g_originalLambVertexShaders[7], g_originalLambVertexShaders[8], 0};
    g_currentVertexShaderSet = g_unlitVertexShaderSet.data();
    g_unlitInputLayoutSet = {g_fvfLayouts[0], g_fvfLayouts[1], g_fvfLayouts[2], 0, g_fvfLayouts[4], g_fvfLayouts[5], g_fvfLayouts[6], 0, g_fvfLayouts[0], 0};
    g_litInputLayoutSet = {g_fvfLayouts[0], g_fvfLayouts[0], g_fvfLayouts[2], 0, g_fvfLayouts[16], g_fvfLayouts[16], g_fvfLayouts[6], 0, g_fvfLayouts[0], 0};
    g_currentInputLayoutSet = g_unlitInputLayoutSet.data();
}

} // namespace shenmue::s1
