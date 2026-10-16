#include "s1/render_passes.hpp"
#include "s1/boot.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/render_offsets.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/mt5_render.hpp"
#include "s1/scene_memory.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/task/effe.hpp"
#include "s1/ui_runtime.hpp"
#include "d3t/ui_nuklear.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cfloat>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#endif

namespace shenmue::s1 {

D3TDeviceContext *g_RenderDeviceContext = nullptr;
float g_fContrastValue = 0.5f;
float g_fGammaValue = 1.0f;
bool g_fFxaaEnabled = true;
bool g_fBloomEnabled = true;

namespace {

template <typename T> T ReadState(const std::byte *state, std::size_t offset) {
    T value{};
    std::memcpy(&value, state + offset, sizeof(value));
    return value;
}

template <typename T> void WriteState(std::byte *state, std::size_t offset, const T &value) {
    std::memcpy(state + offset, &value, sizeof(value));
}

} // namespace

void SetShaderState(D3TDeviceContext *context, std::uint32_t vertexShader, std::uint32_t pixelShader, std::uint32_t inputLayout) {
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0) != vertexShader) {
        state[0x975] |= std::byte{0x81};
        WriteState(state, 0x970, ~std::uint32_t{0});
    }
    WriteState(state, 0, vertexShader);
    if (ReadState<std::uint32_t>(state, 4) != pixelShader) {
        state[0x975] |= std::byte{2};
    }
    WriteState(state, 4, pixelShader);
    if (ReadState<std::uint32_t>(state, 0x938) != inputLayout) {
        state[0x975] |= std::byte{0x80};
        WriteState(state, 0x938, inputLayout);
    }
}

void SetCommonQuadState(D3TDeviceContext *context, std::uint64_t blend) {
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint64_t>(state, 0x794) != blend) {
        state[0x975] |= std::byte{8};
        WriteState(state, 0x794, blend);
    }
    const std::array<std::byte, 0x14> disabledDepth{};
    if (std::memcmp(state + 0x940, disabledDepth.data(), disabledDepth.size()) != 0) {
        state[0x975] |= std::byte{0x10};
        std::memcpy(state + 0x940, disabledDepth.data(), disabledDepth.size());
    }
    if (ReadState<std::uint32_t>(state, 0x954) != 1U) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, std::uint32_t{1});
    }

    constexpr std::array<std::uint64_t, 2> sampler{5, 0};
    if (std::memcmp(state + 0x534, sampler.data(), sizeof(sampler)) != 0) {
        auto dirty = ReadState<std::uint16_t>(state, 0x966);
        dirty |= 1U;
        WriteState(state, 0x966, dirty);
        std::memcpy(state + 0x534, sampler.data(), sizeof(sampler));
    }
}

void SetTopology(D3TDeviceContext *context, std::uint32_t topology) {
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0x93c) != topology) {
        state[0x976] |= std::byte{1};
        WriteState(state, 0x93c, topology);
    }
}

void BindPixelTexture8(D3TDeviceContext *context, std::uint32_t texture2D) {
    std::byte *const state = context->renderState.data();
    constexpr std::size_t kTextureOffset = 0x1d4;
    constexpr std::size_t kViewFlagsOffset = 0x354;
    if (ReadState<std::uint32_t>(state, kTextureOffset) != texture2D || ReadState<std::uint32_t>(state, kViewFlagsOffset) != 0U) {
        auto dirty = ReadState<std::uint32_t>(state, 0x95c);
        dirty |= 0x100U;
        WriteState(state, 0x95c, dirty);
        WriteState(state, kTextureOffset, texture2D);
        WriteState(state, kViewFlagsOffset, std::uint32_t{0});
    }
}

namespace {

[[maybe_unused]] void BindNuklearTextures(D3TDeviceContext *context, std::uint32_t texture2D) {
    std::byte *const state = context->renderState.data();
    const std::array<std::uint32_t, 2> textures{texture2D, d3t::g_defaultWhiteTexture};
    constexpr std::size_t kTextureOffset = 0x1d4;
    constexpr std::size_t kViewFlagsOffset = 0x354;
    if (std::memcmp(state + kTextureOffset, textures.data(), sizeof(textures)) != 0 || ReadState<std::uint32_t>(state, kViewFlagsOffset) != 0U ||
        ReadState<std::uint32_t>(state, kViewFlagsOffset + 4) != 0U) {
        auto dirty = ReadState<std::uint32_t>(state, 0x95c);
        dirty |= 0x300U;
        WriteState(state, 0x95c, dirty);
        std::memcpy(state + kTextureOffset, textures.data(), sizeof(textures));
        WriteState(state, kViewFlagsOffset, std::uint32_t{0});
        WriteState(state, kViewFlagsOffset + 4, std::uint32_t{0});
    }
}

[[maybe_unused]] void SetNuklearGeometryState(D3TDeviceContext *context) {
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0x930) != static_cast<std::uint32_t>(g_nuklearIndexBuffer) || ReadState<std::uint32_t>(state, 0x934) != 11U) {
        state[0x975] |= std::byte{0x40};
        WriteState(state, 0x930, static_cast<std::uint32_t>(g_nuklearIndexBuffer));
        // D3T format 11 maps to DXGI_FORMAT_R16_UINT.
        WriteState(state, 0x934, std::uint32_t{11});
    }
    if (ReadState<std::uint32_t>(state, 0xc) != static_cast<std::uint32_t>(g_nuklearVertexBuffer)) {
        auto dirty = ReadState<std::uint32_t>(state, 0x970);
        dirty |= 1U;
        WriteState(state, 0x970, dirty);
        WriteState(state, 0xc, static_cast<std::uint32_t>(g_nuklearVertexBuffer));
    }
    SetTopology(context, 0);
}

[[maybe_unused]] bool UploadNuklearQuads(D3TDeviceContext *context, std::span<const NuklearQuad> quads) {
    struct Vertex {
        float x;
        float y;
        float u;
        float v;
        std::uint32_t colour;
    };
    static_assert(sizeof(Vertex) == 20);
    if (quads.empty() || quads.size() > 0x3fffU)
        return false;
    void *const vertexData = GetWriteMappedBufferPointer(context, g_nuklearVertexBuffer, 1, 0);
    void *const indexData = GetWriteMappedBufferPointer(context, g_nuklearIndexBuffer, 1, 0);
    if (!vertexData || !indexData) {
        if (vertexData)
            DX11_CopySceneNodeToGPU(context, g_nuklearVertexBuffer);
        if (indexData)
            DX11_CopySceneNodeToGPU(context, g_nuklearIndexBuffer);
        return false;
    }
    auto *const vertices = static_cast<Vertex *>(vertexData);
    auto *const indices = static_cast<std::uint16_t *>(indexData);
    for (std::size_t index = 0; index < quads.size(); ++index) {
        const NuklearQuad &quad = quads[index];
        const std::size_t vertex = index * 4;
        std::array<float, 8> corners{quad.x, quad.y, quad.x + quad.width, quad.y, quad.x + quad.width, quad.y + quad.height, quad.x, quad.y + quad.height};
        if (quad.rotation != 0.0f) {

            const float cosine = std::cos(quad.rotation);
            const float sine = std::sin(quad.rotation);
            const float centreX = (corners[2] - corners[0]) * 0.5f + corners[0];
            const float centreY = (corners[5] - corners[1]) * 0.5f + corners[1];
            for (std::size_t corner = 0; corner < corners.size(); corner += 2) {
                const float dx = corners[corner] - centreX;
                const float dy = corners[corner + 1] - centreY;
                corners[corner] = dx * cosine + centreX - sine * dy;
                corners[corner + 1] = sine * dx + centreY + dy * cosine;
            }
        }
        vertices[vertex + 0] = {corners[0], corners[1], quad.u0, quad.v0, quad.colour};
        vertices[vertex + 1] = {corners[2], corners[3], quad.u1, quad.v0, quad.colour};
        vertices[vertex + 2] = {corners[4], corners[5], quad.u1, quad.v1, quad.colour};
        vertices[vertex + 3] = {corners[6], corners[7], quad.u0, quad.v1, quad.colour};
        const std::size_t element = index * 6;
        const auto base = static_cast<std::uint16_t>(vertex);
        indices[element + 0] = base;
        indices[element + 1] = static_cast<std::uint16_t>(base + 1);
        indices[element + 2] = static_cast<std::uint16_t>(base + 2);
        indices[element + 3] = base;
        indices[element + 4] = static_cast<std::uint16_t>(base + 2);
        indices[element + 5] = static_cast<std::uint16_t>(base + 3);
    }
    DX11_CopySceneNodeToGPU(context, g_nuklearIndexBuffer);
    DX11_CopySceneNodeToGPU(context, g_nuklearVertexBuffer);
    g_Nuklear_num_verts = static_cast<std::uint32_t>(quads.size() * 4);
    g_Nuklear_num_indices = static_cast<std::uint32_t>(quads.size() * 6);
    return true;
}

[[maybe_unused]] void SetNuklearConstants(D3TDeviceContext *context, float width, float height) {
#if defined(_WIN32)
    if (auto *const matrix = static_cast<float *>(MapTempConstantBufferForWriteDiscard(context, 1, 0, 64))) {
        const std::array<float, 16> orthographic{
            2.0f / width,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            -2.0f / height,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            0.0f,
            -1.0f,
            1.0f,
            0.5f,
            1.0f,
        };
        std::memcpy(matrix, orthographic.data(), sizeof(orthographic));
    }
    if (auto *const reciprocal = static_cast<float *>(MapTempConstantBufferForWriteDiscard(context, 2, 1, 8))) {
        reciprocal[0] = width > 0.0f ? 1.0f / width : 0.0f;
        reciprocal[1] = height > 0.0f ? 1.0f / height : 0.0f;
    }
#else
    static_cast<void>(context);
    static_cast<void>(width);
    static_cast<void>(height);
#endif
}

bool SubmitNuklearQuads(D3TDeviceContext *context, std::uint32_t renderTarget, std::uint32_t texture2D, std::span<const NuklearQuad> quads) {
#if defined(_WIN32)
    std::uint32_t targetWidth = 0;
    std::uint32_t targetHeight = 0;
    if (!context || !renderTarget || !texture2D || quads.empty() || !GetTexture2DDimensions(renderTarget, &targetWidth, &targetHeight) || !targetWidth ||
        !targetHeight || !d3t_GetNativeShader(g_nuklearVertexShader, D3TShaderType::Vertex) ||
        !d3t_GetNativeShader(g_nuklearPixelShader, D3TShaderType::Pixel)) {
        return false;
    }
    if (!UploadNuklearQuads(context, quads))
        return false;
    SetRenderTarget(context, renderTarget);
    SetViewport(context, static_cast<float>(targetWidth), static_cast<float>(targetHeight));
    SetShaderState(context, g_nuklearVertexShader, g_nuklearPixelShader, g_nuklearInputLayout);
    SetCommonQuadState(context, 0xf0138293ULL);
    SetNuklearGeometryState(context);
    SetNuklearConstants(context, static_cast<float>(targetWidth), static_cast<float>(targetHeight));
    BindNuklearTextures(context, texture2D);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    native->DrawIndexed(static_cast<UINT>(quads.size() * 6), 0, 0);
    return true;
#else
    static_cast<void>(context);
    static_cast<void>(renderTarget);
    static_cast<void>(texture2D);
    static_cast<void>(quads);
    return false;
#endif
}

} // namespace

void SubmitQuad(D3TDeviceContext *context) {
#if defined(_WIN32)
    const std::byte *const state = context->renderState.data();
    void *const vertexShader = d3t_GetNativeShader(ReadState<std::uint32_t>(state, 0), D3TShaderType::Vertex);
    void *const pixelShader = d3t_GetNativeShader(ReadState<std::uint32_t>(state, 4), D3TShaderType::Pixel);
    if (!vertexShader || !pixelShader) {
        return;
    }
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (native)
        native->Draw(4, 0);
#else
    static_cast<void>(context);
#endif
}

void SetRenderTarget(D3TDeviceContext *context, std::uint32_t texture2D) {
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data(), &texture2D, sizeof(texture2D));
    context->renderState[0x976] |= std::byte{2};
    std::memcpy(context->renderState.data() + 0x7dc, targets.data(), targets.size());
}

void SetViewport(D3TDeviceContext *context, float width, float height) {
    const std::array<float, 6> viewport{0.0f, 0.0f, width, height, 0.0f, 1.0f};
    std::byte *const state = context->renderState.data();
    if (std::memcmp(state + 0x870, viewport.data(), sizeof(viewport)) != 0) {
        state[0x974] |= std::byte{1};
        std::memcpy(state + 0x870, viewport.data(), sizeof(viewport));
    }
}

namespace {

void SetOriginalStripRenderFlag(std::uint32_t setting, std::int32_t value) {
    SetRenderFlags(setting, value);
    g_currentRenderFlags[setting] = value;
}

void RestoreOriginalStripRenderFlag(std::uint32_t setting) {
    SetOriginalStripRenderFlag(setting, g_defaultRenderFlags[setting]);
}

void SetOriginalStripAlphaFlag(std::uint32_t setting, std::int32_t value) {
    SetAlphaFlags(0, setting, static_cast<std::uint32_t>(value));
    g_currentAlphaRenderFlags[0].values[setting] = value;
}


void BindOriginalStripVertexBuffer(D3TDeviceContext *context, std::int32_t buffer) {
#if defined(_WIN32)
    if (context->resourceCache[0]) {
        static_cast<ID3D11DeviceContext *>(context->deviceContext)->Unmap(reinterpret_cast<ID3D11Resource *>(context->resourceCache[0]), 0);
        context->resourceCache[0] = 0;
    }
#endif
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.vertexBufferHandles[0] != static_cast<std::uint32_t>(buffer))
        state.vertexBufferDirtyMask |= 1U;
    state.vertexBufferHandles[0] = static_cast<std::uint32_t>(buffer);
}

void DrawOriginalStripVertices(D3TDeviceContext *context, std::uint32_t vertexCount, std::uint32_t startVertex) {
#if defined(_WIN32)
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    if (auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext))
        native->Draw(vertexCount, startVertex);
#else
    static_cast<void>(context);
    static_cast<void>(vertexCount);
    static_cast<void>(startVertex);
#endif
}


float ProjectOriginalStripDepth(float depth) {
    return g_tempNearClip <= depth ? (g_cameraProjectionMatrix.rows[3].z - depth * g_cameraProjectionMatrix.rows[2].z) / depth : depth;
}

float OriginalStripDepth(float maxExtent) {
    return maxExtent >= 0.0f ? maxExtent + 100.0f : 1.0f / std::fmax(1.0f, maxExtent * -1.0f);
}

} // namespace



void DrawVertices_original_strip00_HN(std::int32_t vertexCount, const float *packet) {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    BindOriginalStripVertexBuffer(context, g_original_strip00_Buffer);
    SetShaderState(context, g_originalStrip00VertexShader, g_originalStrip00PixelShader, g_originalStrip00InputLayout);
    auto *const vertices = static_cast<std::byte *>(GetWriteMappedBufferPointer(context, g_original_strip00_Buffer, 2, 20U * g_originalStrip00VertexCursor));
    for (std::int32_t index = 0; index < vertexCount; ++index) {
        const float *const source = packet + index * 4;
        std::byte *const vertex = vertices + static_cast<std::size_t>(index) * 20U;
        const std::array<float, 3> position{source[0] - 0.5f, source[1] - 0.5f, ProjectOriginalStripDepth(source[2])};
        std::memcpy(vertex, position.data(), sizeof(position));
        std::memcpy(vertex + 16, source + 3, sizeof(float));
    }
    DX11_CopySceneNodeToGPU(context, g_original_strip00_Buffer);
    SetOriginalStripRenderFlag(92, 0);
    SetTopology(context, 1);
    CommitLegacyScreenRenderState(context, false);
    DrawOriginalStripVertices(context, static_cast<std::uint32_t>(vertexCount), g_originalStrip00VertexCursor);
    RestoreOriginalStripRenderFlag(92);
    BindLegacyPixelTexture(context, g_rendererWhiteTexture);
    g_originalStrip00VertexCursor += static_cast<std::uint32_t>(vertexCount);
}





void CalculateViewCameraTransforms(const float *packet, std::int32_t texture) {
    auto *const records = static_cast<OriginalStripPassRecord *>(g_original_strip00_shader);
    const std::int32_t index = g_originalStripPassCount;
    OriginalStripPassRecord &record = records[index];
    record.startVertex = static_cast<std::int32_t>(g_originalStrip03VertexCursor);
    record.texture = texture;
    record.vertexCount = 4;
    if (g_tempNearClip <= packet[2]) {
        record.depth = -1.0f;
        EFFE_AssignObjectPass(Do_original_strip_03_Pass_HN, reinterpret_cast<void *>(static_cast<std::intptr_t>(index)), packet[2]);
    } else {
        record.depth = packet[2];
    }
    static_cast<OriginalStripPassRecord **>(g_original_strip00_buffer)[index] = &record;
    ++g_originalStripPassCount;
    auto *const vertices = static_cast<std::byte *>(GetWriteMappedBufferPointer(
        DX11::g_ppD3DDeviceContext, DX11::g_tmp_buffer_handle, (g_originalStrip03VertexCursor != 0U) + 1, 32U * g_originalStrip03VertexCursor));
    if (!vertices)
        return;
    for (std::size_t corner = 0; corner < 4; ++corner) {
        std::byte *const vertex = vertices + corner * 32U;
        if (texture != -1) {
            const float *const source = packet + corner * 7U;
            const std::array<float, 3> position{source[0], source[1], ProjectOriginalStripDepth(source[2])};
            std::memcpy(vertex, position.data(), sizeof(position));
            std::memcpy(vertex + 16, source + 5, 2U * sizeof(float));
            std::memcpy(vertex + 24, source + 3, 2U * sizeof(float));
        } else {
            const float *const source = packet + corner * 4U;
            const std::array<float, 3> position{source[0], source[1], ProjectOriginalStripDepth(source[2])};
            constexpr std::array<std::uint32_t, 3> unused{};
            std::memcpy(vertex, position.data(), sizeof(position));
            std::memcpy(vertex + 16, source + 3, sizeof(float));
            std::memcpy(vertex + 20, unused.data(), sizeof(unused));
        }
    }
    DX11_CopySceneNodeToGPU(DX11::g_ppD3DDeviceContext, DX11::g_tmp_buffer_handle);
    g_originalStrip03VertexCursor += 4U;
}





void Do_original_strip_03_Pass_HN(void *argument) {
    const std::int32_t index = static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(argument));
    const OriginalStripPassRecord &record = static_cast<const OriginalStripPassRecord *>(g_original_strip00_shader)[index];
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    SetShaderState(context, g_originalStrip03VertexShader, g_originalStrip03PixelShader, g_originalStrip03InputLayout);
    BindOriginalStripVertexBuffer(context, DX11::g_tmp_buffer_handle);
    SetOriginalStripRenderFlag(147, 0);
    SetOriginalStripRenderFlag(7, 0);
    SetOriginalStripRenderFlag(103, 1);
    SetOriginalStripRenderFlag(92, 0);
    if (record.texture < 0) {
        BindLegacyPixelTexture(context, record.texture == -1 ? g_rendererWhiteTexture : ResolveStripTextureHandle(static_cast<std::uint32_t>(record.texture)));
    } else if (const SurfaceRecord *const surface = GetPVRSurfaceRecord(record.texture)) {
        BindLegacyPixelTexture(context, ResolveStripTextureHandle(surface->secondary.extraParam));
    }
    SetTopology(context, 1);
    CommitLegacyScreenRenderState(context, false);
    DrawOriginalStripVertices(context, static_cast<std::uint32_t>(record.vertexCount), static_cast<std::uint32_t>(record.startVertex));
    RestoreOriginalStripRenderFlag(92);
    BindLegacyPixelTexture(context, g_rendererWhiteTexture);
    for (const std::uint32_t setting : {1U, 0U, 4U, 3U})
        SetOriginalStripAlphaFlag(setting, g_alphaRenderDefaults[0].values[setting]);
    RestoreOriginalStripRenderFlag(103);
    RestoreOriginalStripRenderFlag(7);
    RestoreOriginalStripRenderFlag(147);
}



void SetRenderFlags_4(const float *packet) {
    if ((g_sceneDataMemoryFlags & 2U) != 0U) {
        SetOriginalStripRenderFlag(64, 0);
        SetOriginalStripRenderFlag(60, 1);
        SetOriginalStripRenderFlag(59, 1);
        SetOriginalStripRenderFlag(147, 0);
        DrawVertices_original_strip00_HN(4, packet);
        RestoreOriginalStripRenderFlag(147);
        RestoreOriginalStripRenderFlag(59);
        RestoreOriginalStripRenderFlag(60);
        RestoreOriginalStripRenderFlag(64);
    } else {
        CalculateViewCameraTransforms(packet, -1);
    }
}




void ApplyFadeToPacket(void *rawStrip, float maxExtent) {
    const OriginalStrip &strip = *static_cast<const OriginalStrip *>(rawStrip);
    const float depth = OriginalStripDepth(maxExtent);
    const bool perCorner = (strip.flags & 8U) != 0U;
    const std::array<float, 16> packet{strip.x0, strip.y0, depth, std::bit_cast<float>(strip.corner0), strip.x1, strip.y1, depth,
        std::bit_cast<float>(perCorner ? strip.corner1 : strip.corner0), strip.x2, strip.y2, depth,
        std::bit_cast<float>(perCorner ? strip.corner2 : strip.corner0), strip.x3, strip.y3, depth,
        std::bit_cast<float>(perCorner ? strip.corner3 : strip.corner0)};
    if ((strip.flags & 0x400U) != 0U) {
        SetRenderFlags_4(packet.data());
        return;
    }
    SetOriginalStripRenderFlag(147, 0);
    SetOriginalStripRenderFlag(60, 0);
    g_currentRenderFlags[59] = 1;
    SetRenderFlags(59, 1);
    DrawVertices_original_strip00_HN(4, packet.data());
    RestoreOriginalStripRenderFlag(147);
    RestoreOriginalStripRenderFlag(59);
    RestoreOriginalStripRenderFlag(60);
}




void UnmapAndDraw_SkyRelated_2D(const float *packet, std::int32_t surface) {
    const SurfaceRecord *const record = GetPVRSurfaceRecord(surface);
    if (!record)
        return;
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    BindLegacyPixelTexture(context, ResolveStripTextureHandle(record->secondary.extraParam));
    SetShaderState(context, g_originalStrip03VertexShader, g_originalStrip03PixelShader, g_originalStrip03InputLayout);
    BindOriginalStripVertexBuffer(context, DX11::g_tmp_buffer_handle);
    auto *const vertices = static_cast<std::byte *>(GetWriteMappedBufferPointer(context, DX11::g_tmp_buffer_handle, 2, 32U * g_originalStrip03VertexCursor));
    for (std::size_t corner = 0; corner < 4; ++corner) {
        const float *const source = packet + corner * 7U;
        std::byte *const vertex = vertices + corner * 32U;
        const std::array<float, 3> position{source[0] - 0.5f, source[1] - 0.5f, ProjectOriginalStripDepth(source[2])};
        std::memcpy(vertex, position.data(), sizeof(position));
        std::memcpy(vertex + 16, source + 5, 2U * sizeof(float));
        std::array<float, 2> texcoord{source[3], source[4]};
        if (record->primary.formatWord == 0xa8000000U)
            texcoord = {static_cast<float>(static_cast<std::int32_t>(record->primary.width)) * source[3],
                static_cast<float>(static_cast<std::int32_t>(record->primary.height)) * source[4]};
        std::memcpy(vertex + 24, texcoord.data(), sizeof(texcoord));
    }
    DX11_CopySceneNodeToGPU(context, DX11::g_tmp_buffer_handle);
    SetOriginalStripRenderFlag(92, 0);
    SetTopology(context, 1);
    CommitLegacyScreenRenderState(context, false);
    DrawOriginalStripVertices(context, 4, g_originalStrip03VertexCursor);
    RestoreOriginalStripRenderFlag(92);
    BindLegacyPixelTexture(context, g_rendererWhiteTexture);
    g_originalStrip03VertexCursor += 4U;
}



void SetRenderFlags_UnmapAndDraw(const float *packet, std::int32_t surface) {
    SetOriginalStripRenderFlag(147, 0);
    SetOriginalStripRenderFlag(103, 1);
    SetOriginalStripRenderFlag(59, 1);
    SetOriginalStripAlphaFlag(3, 2);
    SetOriginalStripAlphaFlag(4, 1);
    SetOriginalStripAlphaFlag(0, 1);
    SetOriginalStripAlphaFlag(1, 1);
    UnmapAndDraw_SkyRelated_2D(packet, surface);
    for (const std::uint32_t setting : {1U, 0U, 4U, 3U})
        SetOriginalStripAlphaFlag(setting, g_alphaRenderDefaults[0].values[setting]);
    RestoreOriginalStripRenderFlag(103);
    RestoreOriginalStripRenderFlag(147);
    RestoreOriginalStripRenderFlag(59);
}



void SetRenderFlags_6(const float *packet, std::int32_t surface) {
    if ((g_sceneDataMemoryFlags & 2U) != 0U) {
        SetOriginalStripRenderFlag(64, 0);
        SetOriginalStripRenderFlag(60, 1);
        SetOriginalStripRenderFlag(59, 1);
        SetOriginalStripRenderFlag(147, 0);
        UnmapAndDraw_SkyRelated_2D(packet, surface);
        RestoreOriginalStripRenderFlag(147);
        RestoreOriginalStripRenderFlag(59);
        RestoreOriginalStripRenderFlag(60);
        RestoreOriginalStripRenderFlag(64);
    } else {
        CalculateViewCameraTransforms(packet, surface);
    }
}





void SetRenderFlags_UnmapAndDraw_0(OriginalStrip *strip, std::int32_t texture2D, float maxExtent) {
    const float depth = OriginalStripDepth(maxExtent);
    const float u0 = (strip->flags & 0x100U) != 0U ? strip->normalizedY2 : strip->normalizedY1;
    const float u1 = (strip->flags & 0x100U) != 0U ? strip->normalizedY1 : strip->normalizedY2;
    const float v0 = (strip->flags & 0x200U) != 0U ? strip->normalizedX2 : strip->normalizedX3;
    const float v1 = (strip->flags & 0x200U) != 0U ? strip->normalizedX3 : strip->normalizedX2;
    const bool perCornerColour = (strip->flags & 8U) != 0U;
    const bool perCornerSpecular = (strip->flags & 0x20U) != 0U;
    const float specular = std::bit_cast<float>(strip->vertexCount);
    const std::array<float, 28> packet{strip->x0, strip->y0, depth, u0, v0, std::bit_cast<float>(strip->corner0), specular, strip->x1, strip->y1, depth, u1, v0,
        std::bit_cast<float>(perCornerColour ? strip->corner1 : strip->corner0), perCornerSpecular ? strip->normalizedX0 : specular, strip->x2, strip->y2,
        depth, u0, v1, std::bit_cast<float>(perCornerColour ? strip->corner2 : strip->corner0), perCornerSpecular ? strip->normalizedY0 : specular, strip->x3,
        strip->y3, depth, u1, v1, std::bit_cast<float>(perCornerColour ? strip->corner3 : strip->corner0), perCornerSpecular ? strip->normalizedX1 : specular};
    if ((strip->flags & 0x800U) != 0U) {
        const std::size_t row = static_cast<std::size_t>(std::bit_cast<std::int32_t>(strip->normalizedY3));
        g_originalStripBlendBlock[0] = 0xc0;
        g_originalStripBlendBlock[11] = kOriginalStripBlendPairs[row * 2];
        g_originalStripBlendBlock[12] = kOriginalStripBlendPairs[row * 2 + 1];
    }
    if ((strip->flags & 0x400U) != 0U)
        SetRenderFlags_6(packet.data(), texture2D);
    else
        SetRenderFlags_UnmapAndDraw(packet.data(), texture2D);
}

bool InitializeHmdlInstanceStorage(std::uint32_t renderEntryHandle, std::uint32_t instanceCapacity) {
    std::byte *const entry = renderEntryHandle ? d3t::GetRenderOffsetEntry(renderEntryHandle) : nullptr;
    if (!entry || instanceCapacity == 0 || instanceCapacity > INT32_MAX / sizeof(HmdlInstanceRecord) || ReadState<std::uint32_t>(entry, 0x68) != 0 ||
        ReadState<void *>(entry, 0x70) != nullptr) {
        return false;
    }
    std::byte *const material = ReadState<std::byte *>(entry, 0x48);
    if (!material || !ReadState<void *>(entry, 0x80) || !ReadState<void *>(entry, 0xa0)) {
        return false;
    }

    const std::int32_t byteCount = static_cast<std::int32_t>(instanceCapacity * sizeof(HmdlInstanceRecord));
    const BufferDescription description{3, byteCount, sizeof(HmdlInstanceRecord), 2, nullptr, nullptr};
    const std::int32_t buffer = CreateBuffer(&description);
    if (!buffer)
        return false;
    void *const mirror = std::malloc(static_cast<std::size_t>(byteCount));
    if (!mirror) {
        ValidateAndCommitBuffer(buffer);
        return false;
    }
    WriteState(entry, 0x68, buffer);
    WriteState(entry, 0x70, mirror);
    material[0x2f] = std::byte{1};
    return true;
}

bool UploadHmdlInstanceRecords(D3TDeviceContext *context, std::uint64_t skinningKey, std::span<const HmdlInstanceRecord> instances) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(skinningKey));
    if (!context || !slot || !slot->renderHandles || (skinningKey >> 32U) == 0 || slot->valueTable.capacity == 0 || !slot->valueTable.values ||
        !slot->valueTable.hashes || !slot->valueTable.probes || instances.size() > UINT32_MAX / 6U) {
        return false;
    }
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(skinningKey >> 32U));
    const std::uint32_t renderHandle = slot->renderHandles[index];
    std::byte *const entry = renderHandle ? d3t::GetRenderOffsetEntry(renderHandle) : nullptr;
    if (!entry)
        return false;
    const std::int32_t buffer = ReadState<std::int32_t>(entry, 0x68);
    void *const mirror = ReadState<void *>(entry, 0x70);
    std::byte *const mesh = ReadState<std::byte *>(entry, 0x80);
    std::byte *const primitives = ReadState<std::byte *>(entry, 0xa0);
    D3TBufferBackendView bufferView{};
    if (!mirror || !mesh || !primitives || !GetBufferBackendView(buffer, &bufferView) || bufferView.description.count != sizeof(HmdlInstanceRecord) ||
        bufferView.sizeInBytes < 0 || instances.size_bytes() > static_cast<std::size_t>(bufferView.sizeInBytes)) {
        return false;
    }



    void *const mapped = GetWriteMappedBufferPointer(context, buffer, 1, 0);
    if (mapped && !instances.empty()) {
        std::memcpy(mapped, instances.data(), instances.size_bytes());
    }
    DX11_CopySceneNodeToGPU(context, buffer);
    if (!instances.empty()) {
        std::memcpy(mirror, instances.data(), instances.size_bytes());
    }
    const std::uint32_t count = static_cast<std::uint32_t>(instances.size());
    WriteState(entry, 0x6c, count);
    WriteState(primitives, 4, count * 2U);
    WriteState(mesh, 4, count * 6U);

    Vector3 minimum{FLT_MAX, FLT_MAX, FLT_MAX};
    Vector3 maximum{-FLT_MAX, -FLT_MAX, -FLT_MAX};
    for (const HmdlInstanceRecord &instance : instances) {
        const float radius = std::fmax(std::fabs(instance.cornerOffsets[1]),
            std::fmax(std::fabs(instance.cornerOffsets[3]), std::fmax(std::fabs(instance.cornerOffsets[0]), std::fabs(instance.cornerOffsets[2]))));
        const auto expand = [radius](float position, float &low, float &high) {
            if (high < position + radius)
                high = position + radius;
            if (position - radius <= low)
                low = position - radius;
        };
        expand(instance.position.x, minimum.x, maximum.x);
        expand(instance.position.y, minimum.y, maximum.y);
        expand(instance.position.z, minimum.z, maximum.z);
    }
    WriteState(mesh, 0x2c, minimum);
    WriteState(mesh, 0x38, maximum);
    WriteState(primitives, 0x0c, minimum);
    WriteState(primitives, 0x18, maximum);
    return true;
}

void ReleaseHmdlInstanceCpuMirrors() {
    for (std::uint32_t index = 0; index < 256; ++index) {
        const std::uint32_t generation = d3t::g_d3tModels.generations[index];
        if ((generation & 0x80000000U) == 0)
            continue;
        std::byte *const entry = d3t::GetRenderOffsetEntry(generation | index);
        void *const mirror = ReadState<void *>(entry, 0x70);
        std::free(mirror);
        WriteState(entry, 0x70, static_cast<void *>(nullptr));
    }


}

EffeParticleVertex *MapEffeParticleVertices(D3TDeviceContext *context, std::uint32_t particleCount) {
#if defined(_WIN32)
    if (!context || !particleCount || !g_fvfPtdTVertexShader || !g_fvfPtdTPixelShader || !g_fvfLayouts[28] ||
        particleCount > UINT32_MAX / (6U * sizeof(EffeParticleVertex))) {
        return nullptr;
    }
    return static_cast<EffeParticleVertex *>(GetMappedConstantBuffer(context, 0, static_cast<std::uint32_t>(particleCount * 6U * sizeof(EffeParticleVertex))));
#else
    static_cast<void>(context);
    static_cast<void>(particleCount);
    return nullptr;
#endif
}

std::uint32_t CommitEffeParticleVertices(
    D3TDeviceContext *context, std::uint32_t particleCount, std::uint32_t texture2D, bool transparent, const EffeMaterialWords *material) {
#if defined(_WIN32)
    if (!context || !particleCount)
        return 0;
    SetShaderState(context, g_fvfPtdTVertexShader, g_fvfPtdTPixelShader, g_fvfLayouts[28]);
    SetTopology(context, 0);

    const std::int32_t savedRenderingMode = g_legacyCurrentRenderingMode;
    const auto savedAlphaFlags = g_legacyAlphaFlags;
    const auto savedShaderParameters = g_legacyShaderParameters;
    const auto savedFogEnabled = g_renderParams.fogEnabled;
    const auto savedSpecularEnabled = g_renderParams.specularEnabled;
    const auto savedAlphaTestEnabled = g_renderParams.alphaTestEnabled;
    const std::uint64_t savedEngineFlags = g_legacyEngineFlags;
    const std::uint64_t savedEngineField8 = g_legacyEngineField8;
    if (material) {
        constexpr std::array<std::int32_t, 8> kFogModes{
            1, 0, 774, 775, 770, 771, 772, 773};
        constexpr std::array<std::int32_t, 8> kFogCurves{
            1, 0, 768, 769, 770, 771, 772, 773};
        constexpr std::array<std::uint32_t, 6> kBlendFactors{1, 3, 0, 0, 2, 3};
        SetRenderFlags(103, (material->texture >> 2U) & 1U);
        SetRenderFlags(147, (material->render & 0x18000000U) != 0 ? 2305 : 0);
        SetRenderFlags(64, (material->render & 0x04000000U) == 0 ? 1 : 0);
        SetRenderFlags(62, kFogModes[material->alpha >> 29U]);
        SetRenderFlags(63, kFogCurves[(material->alpha >> 26U) & 7U]);
        SetAlphaFlags(0, 0, kBlendFactors[(material->alpha >> 16U) & 5U]);
        SetAlphaFlags(0, 1, kBlendFactors[(material->alpha >> 15U) & 5U]);
        const std::uint32_t alphaMode = (material->alpha >> 13U) & 3U;
        const std::uint32_t alphaRgb = alphaMode == 0 ? 1U : alphaMode == 2 ? 0U : 2U;
        const std::uint32_t alphaA = alphaMode == 2 ? 1U : alphaRgb;
        SetAlphaFlags(0, 3, alphaRgb);
        SetAlphaFlags(0, 4, alphaRgb);
        SetAlphaFlags(0, 5, alphaA);
        const std::uint32_t filterMode = (material->alpha >> 6U) & 3U;
        SetAlphaFlags(0, 16, filterMode == 1 ? 2U : filterMode == 3 ? 4U : 3U);
        if (filterMode == 1) {
            SetAlphaFlags(0, 18, 2);
        } else if (filterMode == 0) {
            SetAlphaFlags(0, 19, 2);
        } else if (filterMode == 2) {
            SetAlphaFlags(0, 19, 0);
        } else {
            SetAlphaFlags(0, 17, 2);
            SetAlphaFlags(0, 18, 0);
        }
        g_renderParams.specularEnabled = (material->texture >> 2U) & 1U;
        g_renderParams.alphaTestEnabled = 0;
    } else {
        SetRenderFlags(147, 0);
        SetRenderFlags(103, 1);
        SetRenderFlags(59, 1);
        SetRenderFlags(92, 0);
        SetAlphaFlags(0, 3, 2);
        SetAlphaFlags(0, 4, 1);
        SetAlphaFlags(0, 0, 1);
        SetAlphaFlags(0, 1, 1);
        g_renderParams.fogEnabled = 0;
        g_renderParams.specularEnabled = 1;
        g_renderParams.alphaTestEnabled = 0;
    }
    BindLegacyPixelTexture(context, ResolveStripTextureHandle(texture2D));
    CommitLegacyScreenRenderState(context, transparent);
    SetCommonQuadState(context, transparent ? 0xf0138293ULL : 0xf0000000ULL);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (native)
        native->Draw(particleCount * 6U, 0);
    BindLegacyPixelTexture(context, g_rendererWhiteTexture);
    g_legacyCurrentRenderingMode = savedRenderingMode;
    g_legacyAlphaFlags = savedAlphaFlags;
    for (const std::uint32_t setting : {16U, 18U, 19U, 0U, 1U, 3U, 4U, 5U}) {
        SetAlphaFlags(0, setting, static_cast<std::uint32_t>(g_alphaRenderDefaults[0].values[setting]));
        g_currentAlphaRenderFlags[0].values[setting] = g_alphaRenderDefaults[0].values[setting];
    }
    g_legacyShaderParameters = savedShaderParameters;
    g_legacyEngineFlags = savedEngineFlags;
    g_legacyEngineField8 = savedEngineField8;
    g_renderParams.fogEnabled = savedFogEnabled;
    g_renderParams.alphaTestEnabled = savedAlphaTestEnabled;
    g_renderParams.specularEnabled = savedSpecularEnabled;
    return native ? particleCount : 0U;
#else
    static_cast<void>(context);
    static_cast<void>(particleCount);
    static_cast<void>(texture2D);
    static_cast<void>(transparent);
    static_cast<void>(material);
    return 0;
#endif
}

std::uint32_t DrawProjectedOriginalStrip03Batch(D3TDeviceContext *context, std::span<const OriginalStrip> strips,
    std::span<const ProjectedOriginalStripDepths> depths, std::uint32_t texture2D, bool transparent, const EffeMaterialWords *material) {
#if defined(_WIN32)
    if (!context || strips.empty() || !texture2D || !g_fvfPtdTVertexShader || !g_fvfPtdTPixelShader || !g_fvfLayouts[28]) {
        return 0;
    }

    struct ParticleVertex {
        Vector4 position;
        std::uint32_t diffuse;
        float u;
        float v;
    };
    static_assert(sizeof(ParticleVertex) == 0x1c);

    const std::size_t drawCount = (std::min)(strips.size(), kOriginalStrip03BatchQuadCapacity);
    if (drawCount == 0U || (!depths.empty() && depths.size() < drawCount)) {
        return 0;
    }

    void *const mapped = GetMappedConstantBuffer(context, 0, static_cast<std::uint32_t>(drawCount * 6U * sizeof(ParticleVertex)));
    if (!mapped)
        return 0;

    ParticleVertex *output = static_cast<ParticleVertex *>(mapped);
    const auto projectDepth = [](float depth) {
        return depth >= g_tempNearClip && depth != 0.0f ? (g_cameraProjectionMatrix.rows[3].z - depth * g_cameraProjectionMatrix.rows[2].z) / depth : depth;
    };
    for (std::size_t index = 0; index < drawCount; ++index) {
        const OriginalStrip &strip = strips[index];
        const ProjectedOriginalStripDepths projected =
            depths.empty() ? ProjectedOriginalStripDepths{g_tempNearClip, g_tempNearClip, g_tempNearClip, g_tempNearClip} : depths[index];
        const std::array<ParticleVertex, 4> corners{{
            {{strip.x0, strip.y0, projectDepth(projected.corner0), 1.0f}, strip.corner0, 0.0f, 0.0f},
            {{strip.x1, strip.y1, projectDepth(projected.corner1), 1.0f}, strip.corner1, 1.0f, 0.0f},
            {{strip.x2, strip.y2, projectDepth(projected.corner2), 1.0f}, strip.corner2, 0.0f, 1.0f},
            {{strip.x3, strip.y3, projectDepth(projected.corner3), 1.0f}, strip.corner3, 1.0f, 1.0f},
        }};
        const std::array<ParticleVertex, 6> triangles{{
            corners[0],
            corners[1],
            corners[3],
            corners[0],
            corners[3],
            corners[2],
        }};
        std::memcpy(output, triangles.data(), sizeof(triangles));
        output += triangles.size();
    }

    return CommitEffeParticleVertices(context, static_cast<std::uint32_t>(drawCount), texture2D, transparent, material);
#else
    static_cast<void>(context);
    static_cast<void>(strips);
    static_cast<void>(depths);
    static_cast<void>(texture2D);
    static_cast<void>(transparent);
    static_cast<void>(material);
    return 0;
#endif
}

void CreateTexture2DInstance(D3TDeviceContext *context) {
#if defined(_WIN32)
    if (!context)
        return;
    SetRenderTarget(context, CreateNewTexture2D());
    SetViewport(context, static_cast<float>(g_originalScreenWidth), static_cast<float>(g_originalScreenHeight));
#else
    static_cast<void>(context);
#endif
}

std::int64_t DrawSuperSampled(D3TDeviceContext *context, std::uint32_t texture2D, std::uint32_t outputTexture2D) {
    if (!context)
        return 0;
    SetRenderTarget(context, outputTexture2D);

    std::uint32_t width = 0;
    std::uint32_t height = 0;
    if (!GetTexture2DDimensions(outputTexture2D, &width, &height))
        return 0;
    SetViewport(context, static_cast<float>(width), static_cast<float>(height));
    SetShaderState(context, g_texturedQuadVertexShader, g_texturedQuadPixelShader, g_fvfLayouts[34]);
    SetCommonQuadState(context, 0xf0000000ULL);

    constexpr float kBaseScaleFactor = 4.0f / 9.0f;
    const float interpolation = (1.0f / (g_superSampling + 1.0f)) * (1.0f - g_viewportInterpolationProgress) + g_viewportInterpolationProgress;
    const float factor = g_superSamplingTextureActive ? 1.0f : kBaseScaleFactor;
    const float scaledInterpolation = interpolation * factor;
    const std::array<Vector4, 4> vertices{{
        {-1.0f, -1.0f, 0.0f, factor},
        {1.0f, -1.0f, scaledInterpolation, factor},
        {-1.0f, 1.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, scaledInterpolation, 0.0f},
    }};
    static_cast<void>(vertices);
#if defined(_WIN32)
    if (void *const mapped = GetMappedConstantBuffer(context, 3, 64)) {
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    }
#endif
    SetTopology(context, 1);
    BindPixelTexture8(context, texture2D);
    SubmitQuad(context);
    SetTopology(context, 0);
    return 0;
}

void DrawCharacterShadowComposite(D3TDeviceContext *context) {
    if (!context || g_perspectiveProjection == 1)
        return;

    constexpr float kBaseScaleFactor = 4.0f / 9.0f;
    const float interpolation = (1.0f / (g_superSampling + 1.0f)) * (1.0f - g_viewportInterpolationProgress) + g_viewportInterpolationProgress;
    const float factor = g_superSamplingTextureActive ? 1.0f : kBaseScaleFactor;
    g_renderParams.viewportScaleX = g_viewportScaleX * factor * interpolation;
    g_renderParams.viewportScaleY = g_viewportScaleY * factor;

    constexpr float kPackedDiffuse = std::bit_cast<float>(std::uint32_t{0x7f000000});
    const std::array<Vector4, 5> constants{{
        {-1.0f, 1.0f, 0.0f, 1.0f},
        {kPackedDiffuse, -1.0f, -1.0f, 0.0f},
        {1.0f, kPackedDiffuse, 1.0f, 1.0f},
        {0.0f, 1.0f, kPackedDiffuse, 1.0f},
        {-1.0f, 0.0f, 1.0f, kPackedDiffuse},
    }};

    constexpr std::array<std::pair<std::uint32_t, std::int32_t>, 7> kCompositeRenderFlags{{
        {59, 1}, {64, 0}, {92, 0}, {52, 1}, {56, 517}, {55, 0}, {2, 15},
    }};
    for (const std::pair<std::uint32_t, std::int32_t> &flag : kCompositeRenderFlags) {
        SetRenderFlags(flag.first, flag.second);
        g_currentRenderFlags[flag.first] = flag.second;
    }

    SetTopology(context, 1);
    SetShaderState(context, g_fvfPdTVertexShader, g_fvfPdTPixelShader, g_fvfLayouts[29]);
    BindPixelTexture8(context, g_auxiliaryTexture2D);

    constexpr std::array<std::pair<std::uint32_t, std::uint32_t>, 5> kCompositeSamplerFlags{{
        {4, 1}, {3, 1}, {5, 1}, {0, 3}, {1, 3},
    }};
    for (const std::pair<std::uint32_t, std::uint32_t> &flag : kCompositeSamplerFlags) {
        SetAlphaFlags(1, flag.first, flag.second);
        g_currentAlphaRenderFlags[1].values[flag.first] = static_cast<std::int32_t>(flag.second);
    }

#if defined(_WIN32)
    if (void *const mapped = GetMappedConstantBuffer(context, 3, sizeof(constants))) {
        std::memcpy(mapped, constants.data(), sizeof(constants));
    }
#else
    static_cast<void>(constants);
#endif
    CommitLegacyScreenRenderState(context, false);
    SubmitQuad(context);

    for (const std::pair<std::uint32_t, std::uint32_t> &flag : kCompositeSamplerFlags) {
        const std::uint32_t value = static_cast<std::uint32_t>(g_alphaRenderDefaults[1].values[flag.first]);
        SetAlphaFlags(1, flag.first, value);
        g_currentAlphaRenderFlags[1].values[flag.first] = static_cast<std::int32_t>(value);
    }
    constexpr std::array<std::uint32_t, 7> kRestoredRenderFlags{59, 52, 55, 56, 64, 92, 2};
    for (const std::uint32_t setting : kRestoredRenderFlags) {
        SetRenderFlags(setting, g_defaultRenderFlags[setting]);
        g_currentRenderFlags[setting] = g_defaultRenderFlags[setting];
    }
    g_renderParams.fogEnabled = g_legacyShaderParameters.fogEnabled;
}

D3TDeviceContext *DrawFullPass_Quad(const RenderPassHandles *renderPass, DrawPassOpType passType) {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context)
        return nullptr;
    CreateTexture2DInstance(context);

    const std::uint32_t flags = static_cast<std::uint32_t>(passType);
    const bool gammaCorrect = (flags & 1U) != 0;
    const bool alphaBlend = (flags & 4U) != 0;
    SetShaderState(context, gammaCorrect ? g_texturedQuadGammaVertexShader : g_texturedQuadVertexShader,
        gammaCorrect ? g_texturedQuadGammaPixelShader : g_texturedQuadPixelShader, g_fvfLayouts[gammaCorrect ? 35 : 34]);
    SetCommonQuadState(context, alphaBlend ? 0xf0138293ULL : 0xf0000000ULL);

    float aspectScale = 1.0f;
    const std::uint32_t texture2D = renderPass ? renderPass->texture2D : 0;
    if ((flags & 2U) != 0) {
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        if (GetTexture2DDimensions(texture2D, &width, &height) && height && g_originalScreenWidth && g_originalScreenHeight) {
            aspectScale = (static_cast<float>(width) / static_cast<float>(height)) /
                (static_cast<float>(g_originalScreenHeight) / static_cast<float>(g_originalScreenWidth));
        }
    }
    const std::array<Vector4, 4> vertices{{
        {-aspectScale, -1.0f, 0.0f, 1.0f},
        {aspectScale, -1.0f, 1.0f, 1.0f},
        {-aspectScale, 1.0f, 0.0f, 0.0f},
        {aspectScale, 1.0f, 1.0f, 0.0f},
    }};
    static_cast<void>(vertices);
#if defined(_WIN32)
    void *const mapped = GetMappedConstantBuffer(context, 8, 64);
    if (mapped) {
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    }
#endif
    SetTopology(context, 1);
    BindPixelTexture8(context, texture2D);
    SubmitQuad(context);
    SetTopology(context, 0);
    return context;
}



void HangOnRender_DrawQuad(std::uint32_t texture2D) {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context)
        return;
    SetupPPFX(context);
    SetShaderState(context, g_texturedQuadVertexShader, g_texturedQuadPixelShader, g_fvfLayouts[34]);
    std::byte *const state = context->renderState.data();
    constexpr std::uint64_t kBlend = 0xf0000000ULL;
    if (ReadState<std::uint32_t>(state, 0x794) != static_cast<std::uint32_t>(kBlend))
        state[0x975] |= std::byte{8};
    WriteState(state, 0x794, kBlend);
    if (ReadState<std::uint64_t>(state, 0x940) != 0U || ReadState<std::uint32_t>(state, 0x950) != 0U) {
        state[0x975] |= std::byte{0x10};
        WriteState(state, 0x940, std::uint64_t{0});
        WriteState(state, 0x948, std::uint64_t{0});
        WriteState(state, 0x950, std::uint32_t{0});
    }
    if (ReadState<std::uint32_t>(state, 0x954) != 1U) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, std::uint32_t{1});
    }
    if (ReadState<std::uint64_t>(state, 0x534) != 0U) {
        WriteState(state, 0x966, static_cast<std::uint16_t>(ReadState<std::uint16_t>(state, 0x966) | 1U));
        WriteState(state, 0x534, std::array<std::uint64_t, 2>{});
    }

    const DisplayModeInfo *const screen = GetScreenResolution();
    const std::int32_t width = static_cast<std::int32_t>(screen->width);
    const std::int32_t height = static_cast<std::int32_t>(screen->height);
    const std::int32_t scale = height / 224 >= width / 320 ? width / 320 : height / 224;
    const float y = static_cast<float>(scale * 224) / static_cast<float>(height);
    const float x = y / (static_cast<float>(width) / static_cast<float>(height) / 1.4285715f);
    const std::array<Vector4, 4> vertices{{
        {-x, -y, 0.1875f, 0.9375f},
        {x, -y, 0.8125f, 0.9375f},
        {-x, y, 0.1875f, 0.0625f},
        {x, y, 0.8125f, 0.0625f},
    }};
#if defined(_WIN32)
    if (void *const mapped = GetMappedConstantBuffer(context, 8, 64))
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
#endif
    SetTopology(context, 1);
    BindPixelTexture8(context, texture2D);
    SubmitQuad(context);
    SetTopology(context, 0);
}

void Do_contrast_Pass() {
    D3TDeviceContext *const context = g_RenderDeviceContext;
    if (!context)
        return;
    SetShaderState(context, g_contrastVertexShader, g_contrastPixelShader, 0);
    CreateTexture2DInstance(context);
#if defined(_WIN32)
    if (auto *const value = static_cast<float *>(MapTempConstantBufferForWriteDiscard(context, 2, 4, 4))) {
        *value = g_fContrastValue;
    }
#endif
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0x954) != 1U) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, std::uint32_t{1});
    }
    constexpr std::uint64_t kContrastBlend = 0xf0000121ULL;
    if (ReadState<std::uint64_t>(state, 0x794) != kContrastBlend) {
        state[0x975] |= std::byte{8};
        WriteState(state, 0x794, kContrastBlend);
    }
    if (ReadState<std::uint32_t>(state, 0x938) != 0U) {
        state[0x975] |= std::byte{0x80};
        WriteState(state, 0x938, std::uint32_t{0});
    }
    SetTopology(context, 1);
    SubmitQuad(context);
}

bool BeginNuklearFrame(D3TDeviceContext *context, std::uint32_t renderTarget) {
    if (!context || !renderTarget || !g_nuklearVertexShader || !g_nuklearPixelShader || !g_nuklearVertexBuffer || !g_nuklearIndexBuffer ||
        !g_nuklearInputLayout) {
        return false;
    }
#if defined(_WIN32)
    D3TTextureBackendView view{};
    if (!GetTexture2DBackendView(renderTarget, &view) || !view.view) {
        return false;
    }
    constexpr std::array<float, 4> transparent{};
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->ClearRenderTargetView(static_cast<ID3D11RenderTargetView *>(view.view), transparent.data());
#endif
    return true;
}

bool DrawNuklearImage(
    D3TDeviceContext *context, std::uint32_t renderTarget, std::uint32_t texture2D, float x, float y, float width, float height, float opacity) {
    const auto alpha = static_cast<std::uint32_t>((std::clamp(opacity, 0.0f, 1.0f) * 255.0f) + 0.5f);
    const NuklearQuad quad{x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, (alpha << 24U) | 0x00ffffffU};
    return SubmitNuklearQuads(context, renderTarget, texture2D, std::span{&quad, 1});
}

bool DrawNuklearQuads(D3TDeviceContext *context, std::uint32_t renderTarget, std::uint32_t texture2D, std::span<const NuklearQuad> quads) {
    return SubmitNuklearQuads(context, renderTarget, texture2D, quads);
}

bool CompositeNuklearFrame(D3TDeviceContext *context, std::uint32_t texture2D) {
#if defined(_WIN32)
    if (!context || !texture2D)
        return false;

    // Nuklear renders into its own target.  Composite it with D3T's authored
    // fullscreen pass; CopyResource bypassed the render state and replaced the
    // backbuffer with an empty target before asynchronous UI textures arrived.
    D3TDeviceContext *const previous = DX11::g_ppD3DDeviceContext;
    DX11::g_ppD3DDeviceContext = context;
    const RenderPassHandles pass{texture2D};
    const bool submitted = DrawFullPass_Quad(&pass, DrawPassOpType::AlphaBlend) != nullptr;
    DX11::g_ppD3DDeviceContext = previous;
    return submitted;
#else
    static_cast<void>(context);
    static_cast<void>(texture2D);
    return false;
#endif
}

Vector4 g_currentUIViewportRect{};
Vector2 g_currentUIViewportDepth{};
bool g_bOrthographicRender = true;
Matrix4x4 g_notebookPageMatrix{};
std::array<float, 2> g_nuklearArmedImageSize{};
bool g_nuklearTargetRedirectPending = false;
const std::uint32_t *g_nuklearTargetRedirect = nullptr;



UIViewport AdjustScreenForAspectRatio(std::int32_t mode) {
    const std::int32_t bufferWidth = static_cast<std::int32_t>(g_uiScreenWidth);
    const std::int32_t originalWidth = static_cast<std::int32_t>(g_originalScreenWidth);
    const std::int32_t screenWidth = originalWidth > bufferWidth ? bufferWidth : originalWidth;
    float adjustment = 0.0f;
    if (mode == 1) {
        const float buffer = static_cast<float>(bufferWidth);
        const float screenHeight = static_cast<float>(static_cast<std::int32_t>(g_originalScreenHeight));
        const float scale = (1.0f / (1080.0f / buffer)) * (screenHeight / static_cast<float>(screenWidth));
        adjustment = static_cast<float>(static_cast<std::uint32_t>(static_cast<std::int64_t>(buffer / scale))) - 1439.964f;
    }
    return {.rect = {.x = adjustment * 0.5f, .y = 0.0f, .z = static_cast<float>(bufferWidth) - adjustment, .w = 1080.0f}, .depthRange = {.x = 0.0f, .y = 1.0f}};
}


void CreateOrthographicViewMatrix(std::int32_t width, std::int32_t height, Matrix4x4 *matrix) {
    const float x = static_cast<float>(width);
    const float y = static_cast<float>(height);
    *matrix = {.rows = {{.x = 2.0f / x}, {.y = 2.0f / (0.0f - y)}, {.z = 0.5f}, {.x = (x + 0.0f) / (0.0f - x), .y = (y + 0.0f) / y, .z = 0.5f, .w = 1.0f}}};
}



void Nuklear_BindRenderTarget() {
    D3TDeviceContext *const context = g_RenderDeviceContext;
    if (!context || !g_NuklearInitialized)
        return;
    if (g_nuklearTargetRedirectPending) {
        g_nuklearTargetRedirectPending = false;
        SetRenderTarget(context, *g_nuklearTargetRedirect);
        return;
    }
    SetRenderTarget(context, g_nuklearTexture);
    constexpr std::array<float, 4> kTransparent{};
    UnmapAndClearViews(context, g_nuklearTexture, 0, kTransparent.data());
}



void NuklearUpdateForAspectRatio(std::int32_t mode) {
    D3TDeviceContext *const context = g_RenderDeviceContext;
    if (!context || !g_NuklearInitialized)
        return;
    const UIViewport viewport = AdjustScreenForAspectRatio(mode);
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (std::memcmp(state.viewport.data(), &viewport, sizeof(viewport)) != 0) {
        state.viewportDirtyMask |= 1U;
        std::memcpy(state.viewport.data(), &viewport, sizeof(viewport));
    }
    Nuklear_RenderPass(&viewport);
}

} // namespace shenmue::s1
