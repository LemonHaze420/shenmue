#pragma once

#include <array>
#include <cstdint>

namespace shenmue::d3t {
struct D3TDeviceContext;
extern std::array<std::uint32_t, 4> g_simpleVertexBufferHandles;
extern std::array<void *, 4> g_simpleCpuBuffers;
extern std::array<std::uint32_t, 4> g_simpleIndexBufferHandles;
void AllocateSimpleShaderBuffers();
void ReleaseSimpleShaderBuffers();
void BindLightTileBuffers(D3TDeviceContext *context);
} // namespace shenmue::d3t
