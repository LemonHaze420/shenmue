#include "d3t/light_tiles.hpp"

#include "d3t/buffer.hpp"
#include <cstdlib>

namespace shenmue::d3t {
std::array<std::uint32_t, 4> g_simpleVertexBufferHandles{};
std::array<void *, 4> g_simpleCpuBuffers{};
std::array<std::uint32_t, 4> g_simpleIndexBufferHandles{};

// @verified - p
void AllocateSimpleShaderBuffers() {
    for (std::size_t index = 0; index < g_simpleCpuBuffers.size(); ++index) {
        const BufferDescription vertexDescription{
            3,
            0x17700,
            12,
            2,
            nullptr,
            nullptr,
        };
        g_simpleVertexBufferHandles[index] = static_cast<std::uint32_t>(CreateBuffer(&vertexDescription));

        const BufferDescription indexDescription{
            3,
            0x203a00,
            4,
            2,
            nullptr,
            nullptr,
        };
        g_simpleIndexBufferHandles[index] = static_cast<std::uint32_t>(CreateBuffer(&indexDescription));
        g_simpleCpuBuffers[index] = std::malloc(0x3a1b00);
    }
}

// @verified - p
void ReleaseSimpleShaderBuffers() {
    for (std::size_t index = 0; index < 4; ++index) {
        ValidateAndCommitBuffer(static_cast<std::int32_t>(g_simpleVertexBufferHandles[index]));
        ValidateAndCommitBuffer(static_cast<std::int32_t>(g_simpleIndexBufferHandles[index]));
        std::free(g_simpleCpuBuffers[index]);
    }
}

// @verified - p
void BindLightTileBuffers(D3TDeviceContext *context) {
    D3TRenderState &state = GetRenderState(*context);
    if (state.bindings.bufferHandles[1][5] != g_simpleVertexBufferHandles[2]) {
        state.shaderResourceDirtyMasks[1] |= 0x20U;
        state.bindings.bufferHandles[1][5] = g_simpleVertexBufferHandles[2];
    }
    if (state.bindings.bufferHandles[1][6] != g_simpleIndexBufferHandles[2]) {
        state.shaderResourceDirtyMasks[1] |= 0x40U;
        state.bindings.bufferHandles[1][6] = g_simpleIndexBufferHandles[2];
    }
}
} // namespace shenmue::d3t
