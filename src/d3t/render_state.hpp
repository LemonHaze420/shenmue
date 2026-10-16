#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {






struct D3TRenderState {
    std::uint32_t vertexShader;
    std::uint32_t pixelShader;
    std::uint32_t computeShader;
    std::uint32_t vertexBufferHandles[32];
    std::array<std::byte, 0x708> unknown08c;
    std::array<std::byte, 0x44> blendState;
    std::uint32_t blendSampleMask;
    std::array<std::byte, 0x84> outputTargets;
    std::array<std::int32_t, 4> scissorRect;
    std::array<float, 6> viewport;
    std::array<std::byte, 0xa8> unknown888;
    std::uint32_t indexBufferHandle;
    std::uint32_t indexFormat;
    std::uint32_t inputLayout;
    std::uint32_t topology;
    std::array<std::byte, 0x10> depthStencilState;
    std::uint32_t stencilReference;
    std::uint32_t rasterizerState;
    std::uint32_t shaderResourceDirtyMasks[3];
    std::uint16_t samplerDirtyMasks[3];
    std::uint16_t constantBufferDirtyMasks[3];
    std::uint32_t vertexBufferDirtyMask;
    std::uint8_t viewportDirtyMask;
    std::uint8_t pipelineStateFlags;
    std::uint8_t bufferStateFlags;
    std::uint8_t reserved977;
    std::array<std::byte, 0x150> unknown978;
};

static_assert(offsetof(D3TRenderState, blendState) == 0x794);
static_assert(offsetof(D3TRenderState, outputTargets) == 0x7dc);
static_assert(offsetof(D3TRenderState, scissorRect) == 0x860);
static_assert(offsetof(D3TRenderState, viewport) == 0x870);
static_assert(offsetof(D3TRenderState, indexBufferHandle) == 0x930);
static_assert(offsetof(D3TRenderState, inputLayout) == 0x938);
static_assert(offsetof(D3TRenderState, pipelineStateFlags) == 0x975);
static_assert(sizeof(D3TRenderState) == 0xac8);

enum class TopologyType : std::uint32_t {
    TriangleList = 0,
    TriangleStrip = 1,
    LineList = 2,
};

enum PipelineStateFlag : std::uint8_t {
    PipelineVertexShader = 0x01,
    PipelinePixelShader = 0x02,
    PipelineComputeShader = 0x04,
    PipelineBlend = 0x08,
    PipelineDepthStencil = 0x10,
    PipelineRasterizer = 0x20,
    PipelineIndexBuffer = 0x40,
    PipelineInputLayout = 0x80,
};

enum BufferStateFlag : std::uint8_t {
    BufferTopology = 0x01,
    BufferOutputTargets = 0x02,
};

} // namespace shenmue::d3t
