#pragma once

#include "s1/d3t/namespace.hpp"
#include "d3t/shader.hpp"
#include "d3t/shader_load.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

using D3TShaderType = d3t::ShaderType;

extern std::uint32_t g_originalStrip00VertexShaderLoad;
extern std::uint32_t g_originalStrip03VertexShaderLoad;
extern std::uint32_t g_originalStrip00PixelShaderLoad;
extern std::uint32_t g_originalStrip03PixelShaderLoad;
extern std::uint32_t g_fvfPtdTVertexShaderLoad;
extern std::uint32_t g_fvfPtdTPixelShaderLoad;
extern std::uint32_t g_originalStrip00VertexShader;
extern std::uint32_t g_originalStrip03VertexShader;
extern std::uint32_t g_originalStrip00PixelShader;
extern std::uint32_t g_originalStrip03PixelShader;
extern std::uint32_t g_fvfPtdTVertexShader;
extern std::uint32_t g_fvfPtdTPixelShader;
extern std::uint32_t g_lightingVcolVertexShaderLoad;
extern std::uint32_t g_simplePixelShaderLoad;
extern std::uint32_t g_lightingVcolVertexShader;
extern std::uint32_t g_simplePixelShader;
extern std::uint32_t g_originalLambVertexShaderLoad;
extern std::uint32_t g_originalConstVertexShaderLoad;
extern std::uint32_t g_originalVcol0VertexShaderLoad;
extern std::uint32_t g_originalVcol1VertexShaderLoad;
extern std::uint32_t g_lightingVertexShaderLoad;
extern std::uint32_t g_originalLambVertexShader;
extern std::uint32_t g_originalConstVertexShader;
extern std::uint32_t g_originalVcol0VertexShader;
extern std::uint32_t g_originalVcol1VertexShader;
extern std::uint32_t g_lightingVertexShader;
extern std::array<std::uint32_t, 64> g_originalLambVertexShaders;
extern std::array<std::uint32_t, 64> g_simplePixelShaders;

extern std::uint32_t g_fvfPdTVertexShaderLoad;
extern std::uint32_t g_fvfPdTPixelShaderLoad;
extern std::uint32_t g_texturedQuadShaderLoad;
extern std::uint32_t g_texturedQuadGammaShaderLoad;
extern std::uint32_t g_abufferWriteShaderLoad;
extern std::uint32_t g_abufferCompositionShaderLoad;
extern std::uint32_t g_abufferClearShaderLoad;
extern std::uint32_t g_contrastShaderLoad;

extern std::uint32_t g_fvfPdTVertexShader;
extern std::uint32_t g_fvfPdTPixelShader;
extern std::uint32_t g_texturedQuadVertexShader;
extern std::uint32_t g_texturedQuadPixelShader;
extern std::uint32_t g_texturedQuadGammaVertexShader;
extern std::uint32_t g_texturedQuadGammaPixelShader;
extern std::uint32_t g_abufferWriteVertexShader;
extern std::uint32_t g_abufferWritePixelShader;
extern std::uint32_t g_abufferCompositionVertexShader;
extern std::uint32_t g_abufferCompositionPixelShader;
extern std::uint32_t g_abufferClearVertexShader;
extern std::uint32_t g_abufferClearPixelShader;
extern std::uint32_t g_contrastVertexShader;
extern std::uint32_t g_contrastPixelShader;

using d3t::CreateShader;
using d3t::DestroyAllShaders;
using d3t::DestroyShader;
using d3t::GetFVFOffset;
using d3t::GetFVFStride;
using d3t::GetInputLayout;
using d3t::GetNativeShader;

inline std::uint32_t d3t_CreateShader(std::int32_t type) {
    return d3t::CreateShader(type);
}
inline void d3t_DestroyShader(std::uint32_t shader) {
    d3t::DestroyShader(shader);
}
inline void d3t_DestroyAllShaders() {
    d3t::DestroyAllShaders();
}
inline bool d3t_CreateShaderResource(std::uint32_t shader, const void *bytecode, std::uint64_t bytecodeLength, D3TShaderType type) {
    return d3t::CreateShaderResource(shader, bytecode, bytecodeLength, type);
}
inline void *d3t_GetInputLayout(std::uint32_t vertexShader, std::uint32_t fvfHandle) {
    return d3t::GetInputLayout(vertexShader, fvfHandle);
}
inline void *d3t_GetNativeShader(std::uint32_t shader, D3TShaderType expectedType) {
    return d3t::GetNativeShader(shader, expectedType);
}
inline std::uint32_t d3t_GetFVFStride(std::uint32_t fvfIndex, std::uint32_t slot) {
    return d3t::GetFVFStride(fvfIndex, slot);
}
inline std::uint32_t d3t_GetFVFOffset(std::uint32_t fvfIndex, std::uint32_t slot) {
    return d3t::GetFVFOffset(fvfIndex, slot);
}
inline std::uint32_t QueueShaderCompileFromHLSL(const char *path, std::uint32_t renderHandle, std::uint32_t shader, D3TShaderType type) {
    return d3t::LoadShaderHLSL(path, renderHandle, shader, type);
}
using d3t::LoadPrecompiledShader;
inline std::uint32_t d3t_GetLoadedShader(std::uint32_t loadHandle, std::size_t shaderIndex = 0) {
    return d3t::GetLoadedShader(loadHandle, shaderIndex);
}


extern std::array<std::uint32_t, 10> g_unlitVertexShaderSet;
extern std::array<std::uint32_t, 10> g_litVertexShaderSet;
extern std::array<std::uint32_t, 10> g_unlitInputLayoutSet;
extern std::array<std::uint32_t, 10> g_litInputLayoutSet;
extern std::uint32_t g_vcol0VertexShaderReference;
extern const std::uint32_t *g_currentVertexShaderSet;
extern const std::uint32_t *g_currentInputLayoutSet;

namespace d3t {
void LoadOriginalStripShaders();
void LoadShaders();
} // namespace d3t
void LoadD3TGenericShaders();
void AssignShaderReferences();
void AssignShaderReferences_0();
std::int64_t Shenmue_InitShaders();

} // namespace shenmue::s1
