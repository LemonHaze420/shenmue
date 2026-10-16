#include "d3t/ui_renderer.hpp"

#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/shader.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_definition.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <limits>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#endif

namespace shenmue::d3t {
namespace {

std::int32_t g_uiVertexBuffer = 0;
std::int32_t g_uiIndexBuffer = 0;
std::uint32_t g_uiWhiteTexture = 0;
std::uint32_t g_backBufferTexture = 0;
std::uint32_t g_sceneTexture = 0;
std::uint32_t g_sceneDepthTexture = 0;
std::uint32_t g_sceneWidth = 0;
std::uint32_t g_sceneHeight = 0;
std::uint32_t g_sceneClearColour = 0;
std::uint32_t g_resolvedSceneTexture;
std::uint32_t g_legacySceneTexture;
SceneFrameTargets g_requestedSceneTargets{};
SceneFrameTargets g_allocatedSceneTargets{};
std::uint32_t g_resolveInputLayout;
std::uint32_t g_resolveVertexShader;
std::uint32_t g_resolvePixelShader;
bool g_useResolvedScene = true;

std::uint32_t g_uiInputLayout = 0;
std::uint32_t g_uiVertexShader = 0;
std::uint32_t g_uiPixelShader = 0;
std::uint32_t g_gammaInputLayout = 0;
std::uint32_t g_gammaVertexShader = 0;
std::uint32_t g_gammaPixelShader = 0;

#if defined(_WIN32)
struct UITextureTargetState {
    D3TDeviceContext *context{};
    std::uint32_t texture{};
    std::uint32_t width{};
    std::uint32_t height{};
    bool flipVertical{};
    // The suffix contains live mapped-resource records, not pipeline state.
    std::array<std::byte, 0x978> pipeline;
    std::array<ID3D11RenderTargetView *, 8> renderTargets{};
    ID3D11DepthStencilView *depthTarget{};
    ID3D11VertexShader *vertexShader{};
    ID3D11PixelShader *pixelShader{};
    ID3D11InputLayout *inputLayout{};
    ID3D11Buffer *vertexBuffer{};
    UINT vertexStride{};
    UINT vertexOffset{};
    ID3D11Buffer *indexBuffer{};
    DXGI_FORMAT indexFormat{};
    UINT indexOffset{};
    D3D11_PRIMITIVE_TOPOLOGY topology{};
    ID3D11Buffer *vertexConstants{};
    ID3D11Buffer *pixelConstants{};
    std::array<std::array<ID3D11ShaderResourceView *, 32>, 3> shaderResources{};
    ID3D11SamplerState *sampler{};
    ID3D11BlendState *blend{};
    std::array<FLOAT, 4> blendFactor{};
    UINT sampleMask{};
    ID3D11DepthStencilState *depth{};
    UINT stencilReference{};
    ID3D11RasterizerState *rasterizer{};
    std::array<D3D11_VIEWPORT, 16> viewports{};
    UINT viewportCount{};
    std::array<D3D11_RECT, 16> scissors{};
    UINT scissorCount{};
};
UITextureTargetState uiTextureTarget;
#endif
template <typename T> T ReadState(const std::byte *state, std::size_t offset) {
    T value{};
    std::memcpy(&value, state + offset, sizeof(value));
    return value;
}

template <typename T> void WriteState(std::byte *state, std::size_t offset, const T &value) {
    std::memcpy(state + offset, &value, sizeof(value));
}

void SetTopology(D3TDeviceContext *context, std::uint32_t topology) {
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0x93c) != topology) {
        state[0x976] |= std::byte{1};
        WriteState(state, 0x93c, topology);
    }
}

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

void SetPipelineState(D3TDeviceContext *context, std::uint64_t blend) {
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

void SetGeometryState(D3TDeviceContext *context) {
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0x930) != static_cast<std::uint32_t>(g_uiIndexBuffer) || ReadState<std::uint32_t>(state, 0x934) != 11U) {
        state[0x975] |= std::byte{0x40};
        WriteState(state, 0x930, static_cast<std::uint32_t>(g_uiIndexBuffer));
        WriteState(state, 0x934, std::uint32_t{11});
    }
    if (ReadState<std::uint32_t>(state, 0xc) != static_cast<std::uint32_t>(g_uiVertexBuffer)) {
        auto dirty = ReadState<std::uint32_t>(state, 0x970);
        dirty |= 1U;
        WriteState(state, 0x970, dirty);
        WriteState(state, 0xc, static_cast<std::uint32_t>(g_uiVertexBuffer));
    }
    SetTopology(context, 0);
}

void SetRenderTarget(D3TDeviceContext *context, std::uint32_t texture) {
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data(), &texture, sizeof(texture));
    context->renderState[0x976] |= std::byte{2};
    std::memcpy(context->renderState.data() + 0x7dc, targets.data(), targets.size());
}
void SetSceneRenderTarget(D3TDeviceContext *context, std::uint32_t colour, std::uint32_t depth) {
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data(), &colour, sizeof(colour));
    std::memcpy(targets.data() + 0x80, &depth, sizeof(depth));
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

void SetBackBufferAndScreenRect(D3TDeviceContext *context) {
    SetRenderTarget(context, g_backBufferTexture);
    SetViewport(context, static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
    const std::array<std::int32_t, 4> scissor{
        0, 0, static_cast<std::int32_t>(d3t::g_originalScreenWidth), static_cast<std::int32_t>(d3t::g_originalScreenHeight)};
    if (std::memcmp(context->renderState.data() + 0x860, scissor.data(), sizeof(scissor)) != 0) {
        std::memcpy(context->renderState.data() + 0x860, scissor.data(), sizeof(scissor));
        context->renderState[0x976] |= std::byte{4};
    }
}
bool SetViewportDepthRange(D3TDeviceContext *context, float minDepth, float maxDepth) {
    if (!context)
        return false;
    const std::array<float, 2> depthRange{minDepth, maxDepth};
    std::byte *const state = context->renderState.data();
    if (std::memcmp(state + 0x880, depthRange.data(), sizeof(depthRange)) != 0) {
        state[0x974] |= std::byte{1};
        std::memcpy(state + 0x880, depthRange.data(), sizeof(depthRange));
    }
    return true;
}

void BindTextures(D3TDeviceContext *context, std::uint32_t texture) {
    std::byte *const state = context->renderState.data();
    const std::array<std::uint32_t, 2> textures{texture, g_uiWhiteTexture};
    constexpr std::size_t textureOffset = 0x1d4;
    constexpr std::size_t viewFlagsOffset = 0x354;
    if (std::memcmp(state + textureOffset, textures.data(), sizeof(textures)) != 0 || ReadState<std::uint32_t>(state, viewFlagsOffset) != 0U ||
        ReadState<std::uint32_t>(state, viewFlagsOffset + 4) != 0U) {
        auto dirty = ReadState<std::uint32_t>(state, 0x95c);
        dirty |= 0x300U;
        WriteState(state, 0x95c, dirty);
        std::memcpy(state + textureOffset, textures.data(), sizeof(textures));
        WriteState(state, viewFlagsOffset, std::uint32_t{0});
        WriteState(state, viewFlagsOffset + 4, std::uint32_t{0});
    }
}
void BindPixelTexture8(D3TDeviceContext *context, std::uint32_t texture) {
    std::byte *const state = context->renderState.data();
    constexpr std::size_t textureOffset = 0x1d4;
    constexpr std::size_t viewFlagsOffset = 0x354;
    if (ReadState<std::uint32_t>(state, textureOffset) != texture || ReadState<std::uint32_t>(state, viewFlagsOffset) != 0U) {
        auto dirty = ReadState<std::uint32_t>(state, 0x95c);
        dirty |= 0x100U;
        WriteState(state, 0x95c, dirty);
        WriteState(state, textureOffset, texture);
        WriteState(state, viewFlagsOffset, std::uint32_t{0});
    }
}

bool UploadUIGeometry(D3TDeviceContext *context, std::span<const UITexturedVertex> vertices, std::span<const std::uint16_t> indices) {
    const std::int32_t vertexCapacity = GetSizeForModelBuffer(g_uiVertexBuffer);
    const std::int32_t indexCapacity = GetSizeForModelBuffer(g_uiIndexBuffer);
    if (vertexCapacity <= 0 || indexCapacity <= 0 || vertices.size_bytes() > static_cast<std::size_t>(vertexCapacity) ||
        indices.size_bytes() > static_cast<std::size_t>(indexCapacity))
        return false;
    void *const vertexData = GetWriteMappedBufferPointer(context, g_uiVertexBuffer, 1, 0);
    void *const indexData = GetWriteMappedBufferPointer(context, g_uiIndexBuffer, 1, 0);
    if (!vertexData || !indexData) {
        if (vertexData)
            DX11_CopySceneNodeToGPU(context, g_uiVertexBuffer);
        if (indexData)
            DX11_CopySceneNodeToGPU(context, g_uiIndexBuffer);
        return false;
    }
    std::memcpy(vertexData, vertices.data(), vertices.size_bytes());
#if defined(_WIN32)
    if (uiTextureTarget.context && uiTextureTarget.flipVertical) {
        UITexturedVertex *const output = static_cast<UITexturedVertex *>(vertexData);
        for (std::size_t index = 0; index < vertices.size(); ++index)
            output[index].y = static_cast<float>(uiTextureTarget.height) - output[index].y;
    }
#endif
    std::memcpy(indexData, indices.data(), indices.size_bytes());
#if defined(_WIN32)
    if (uiTextureTarget.context && uiTextureTarget.flipVertical) {
        std::uint16_t *const output = static_cast<std::uint16_t *>(indexData);
        for (std::size_t index = 0; index + 2 < indices.size(); index += 3)
            std::swap(output[index + 1], output[index + 2]);
    }
#endif
    DX11_CopySceneNodeToGPU(context, g_uiIndexBuffer);
    DX11_CopySceneNodeToGPU(context, g_uiVertexBuffer);
    return true;
}

bool UploadQuad(D3TDeviceContext *context, float x, float y, float width, float height, std::uint32_t colour) {
    const std::array<UITexturedVertex, 4> vertices{{
        {x, y, 0.0f, 0.0f, colour},
        {x + width, y, 1.0f, 0.0f, colour},
        {x + width, y + height, 1.0f, 1.0f, colour},
        {x, y + height, 0.0f, 1.0f, colour},
    }};
    constexpr std::array<std::uint16_t, 6> indices{0, 1, 2, 0, 2, 3};
    return UploadUIGeometry(context, vertices, indices);
}

void SetConstants(D3TDeviceContext *context, float width, float height) {
#if defined(_WIN32)
    if (auto *const matrix = static_cast<float *>(d3t::MapTempConstantBufferForWriteDiscard(context, 1, 0, 64))) {
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
    if (auto *const reciprocal = static_cast<float *>(d3t::MapTempConstantBufferForWriteDiscard(context, 2, 1, 8))) {
        reciprocal[0] = 1.0f / width;
        reciprocal[1] = 1.0f / height;
    }
#else
    static_cast<void>(context);
    static_cast<void>(width);
    static_cast<void>(height);
#endif
}

bool SubmitUIImage(std::uint32_t texture, float x, float y, float width, float height, float opacity) {
#if defined(_WIN32)
    auto *const context = DX11::g_ppD3DDeviceContext;
    const auto target = uiTextureTarget.context ? uiTextureTarget.texture : g_backBufferTexture;
    const float targetWidth = static_cast<float>(uiTextureTarget.context ? uiTextureTarget.width : g_originalScreenWidth);
    const float targetHeight = static_cast<float>(uiTextureTarget.context ? uiTextureTarget.height : g_originalScreenHeight);
    if (!context || !target || !texture || !GetNativeShader(g_uiVertexShader, ShaderType::Vertex) || !GetNativeShader(g_uiPixelShader, ShaderType::Pixel)) {
        return false;
    }
    const auto alpha = static_cast<std::uint32_t>(std::clamp(opacity, 0.0f, 1.0f) * 255.0f + 0.5f);
    if (!UploadQuad(context, x, y, width, height, (alpha << 24U) | 0x00ffffffU)) {
        return false;
    }
    SetRenderTarget(context, target);
    SetViewport(context, targetWidth, targetHeight);
    SetShaderState(context, g_uiVertexShader, g_uiPixelShader, g_uiInputLayout);
    SetPipelineState(context, 0xf0138293ULL);
    SetGeometryState(context);
    SetConstants(context, targetWidth, targetHeight);
    BindTextures(context, texture);
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    native->DrawIndexed(6, 0, 0);
    return true;
#else
    static_cast<void>(texture);
    static_cast<void>(x);
    static_cast<void>(y);
    static_cast<void>(width);
    static_cast<void>(height);
    static_cast<void>(opacity);
    return false;
#endif
}

float AlignedPosition(UIAlign align, float parentPosition, float parentSize, float size, float authoredPosition, float offset, bool horizontal) {
    switch (align) {
    case UIAlign::Top:
        return parentPosition + authoredPosition + offset;
    case UIAlign::TopOut:
        return parentPosition - size + authoredPosition + offset;
    case UIAlign::Bottom:
        return parentPosition + parentSize - size + authoredPosition + offset;
    case UIAlign::BottomOut:
        return parentPosition + parentSize + authoredPosition + offset;
    case UIAlign::CentreRight:
        return parentPosition + (horizontal ? parentSize * 0.5f : 0.0f) + authoredPosition + offset;
    case UIAlign::CentreLeft:
        return parentPosition + (horizontal ? parentSize * 0.5f - size : 0.0f) + authoredPosition + offset;
    case UIAlign::CentreUp:
        return parentPosition + (horizontal ? 0.0f : parentSize * 0.5f - size) + authoredPosition + offset;
    case UIAlign::CentreDown:
        return parentPosition + (horizontal ? 0.0f : parentSize * 0.5f) + authoredPosition + offset;
    case UIAlign::Centre:
    default:
        return parentPosition + (parentSize - size) * 0.5f + authoredPosition + offset;
    }
}

void ScaleUILayout(std::string_view style, float authoredWidth, float authoredHeight, const UIRect &parent, float referenceWidth, float referenceHeight,
    float &width, float &height) {

    width = authoredWidth;
    height = authoredHeight;
    if (style == "Stretch") {
        width = parent.width;
        height = parent.height;
    } else if ((style == "Fit" || style == "Fill") && authoredWidth > 0.0f && authoredHeight > 0.0f) {
        const float x = parent.width / authoredWidth;
        const float y = parent.height / authoredHeight;
        const float scale = style == "Fit" ? std::min(x, y) : std::max(x, y);
        width *= scale;
        height *= scale;
    } else if (style == "Percent") {
        width = referenceWidth != 0.0f ? authoredWidth * parent.width / referenceWidth : 0.0f;
        height = referenceHeight != 0.0f ? authoredHeight * parent.height / referenceHeight : 0.0f;
    }
}

std::unordered_map<std::string, UIRect> ResolvePanelRectangles(const UIDefinition &definition, bool nativeLabelLayout = false) {
    std::unordered_map<std::string, UIRect> rectangles;
    for (const UIPanelDefinition &panel : definition.panels) {
        UIRect parent{0.0f, 0.0f, 1920.0f, 1080.0f};
        if (!panel.parentId.empty()) {
            const auto found = rectangles.find(panel.parentId);
            if (found != rectangles.end())
                parent = found->second;
        }
        UIRect rect{};
        rect.width = panel.width;
        rect.height = panel.height;
        if (nativeLabelLayout) {
            const auto *parentDefinition = definition.findPanel(panel.parentId);
            ScaleUILayout(panel.scaleStyle, panel.width, panel.height, parent, parentDefinition ? parentDefinition->width : 1920.0f,
                parentDefinition ? parentDefinition->height : 1080.0f, rect.width, rect.height);
            rect.width *= panel.scaleX;
            rect.height *= panel.scaleY;
        }
        rect.x = AlignedPosition(panel.x.align, parent.x, parent.width, rect.width, panel.x.position, panel.x.offset, true);
        rect.y = AlignedPosition(panel.y.align, parent.y, parent.height, rect.height, panel.y.position, panel.y.offset, false);
        if (!panel.id.empty())
            rectangles[panel.id] = rect;
    }
    return rectangles;
}

bool DrawPanel(const UIDefinition &definition, const UIPanelDefinition &panel, const UIRect &rect, float opacity) {
    const UIImageDefinition *const image = definition.findImage(panel.imageName);
    if (!image || !image->textureHandle)
        return false;

    UIRect draw = rect;
    if (panel.drawWidth > 0.0f && panel.drawHeight > 0.0f) {
        draw.width = panel.drawWidth;
        draw.height = panel.drawHeight;
        draw.x = AlignedPosition(panel.drawX.align, rect.x, rect.width, draw.width, panel.drawX.position, panel.drawX.offset, true);
        draw.y = AlignedPosition(panel.drawY.align, rect.y, rect.height, draw.height, panel.drawY.position, panel.drawY.offset, false);
    }
    const float scaleX = static_cast<float>(d3t::g_originalScreenWidth) / 1920.0f;
    const float scaleY = static_cast<float>(d3t::g_originalScreenHeight) / 1080.0f;
    return SubmitUIImage(image->textureHandle, draw.x * scaleX, draw.y * scaleY, draw.width * scaleX, draw.height * scaleY, opacity * panel.imageOpacity);
}

void RetireSceneTextures() {
    if (g_resolvedSceneTexture && g_resolvedSceneTexture != g_sceneTexture)
        d3t::InitTexture2DResource(g_resolvedSceneTexture);
    if (g_legacySceneTexture && g_legacySceneTexture != g_sceneTexture)
        d3t::InitTexture2DResource(g_legacySceneTexture);
    if (g_sceneTexture)
        d3t::InitTexture2DResource(g_sceneTexture);
    if (g_sceneDepthTexture)
        d3t::InitTexture2DResource(g_sceneDepthTexture);
    g_sceneTexture = g_sceneDepthTexture = g_resolvedSceneTexture = g_legacySceneTexture = 0;
    g_sceneWidth = g_sceneHeight = 0;
    g_allocatedSceneTargets = {};
}

bool EnsureSceneTargets() {
#if defined(_WIN32)
    const SceneFrameTargets targets =
        g_requestedSceneTargets.width ? g_requestedSceneTargets : SceneFrameTargets{.width = d3t::g_originalScreenWidth, .height = d3t::g_originalScreenHeight};
    if (!targets.width || !targets.height)
        return false;
    const SceneFrameTargets &allocated = g_allocatedSceneTargets;
    if (g_sceneTexture && g_sceneDepthTexture && allocated.width == targets.width && allocated.height == targets.height &&
        allocated.resolvedWidth == targets.resolvedWidth && allocated.resolvedHeight == targets.resolvedHeight &&
        allocated.legacyWidth == targets.legacyWidth && allocated.legacyHeight == targets.legacyHeight && allocated.supersampling == targets.supersampling)
        return true;
    RetireSceneTextures();
    d3t::Texture2DDescription colour{targets.width, targets.height, 1, 1, 4, 4, 0, 1, 0, 0, nullptr};
    const d3t::Texture2DDescription depth{targets.width, targets.height, 1, 1, 41, 8, 0, 1, 0, 0, nullptr};
    g_sceneTexture = d3t::CreateTexture2D(&colour);
    g_sceneDepthTexture = d3t::CreateTexture2D(&depth);
    if (targets.supersampling) {
        colour.width = targets.resolvedWidth;
        colour.height = targets.resolvedHeight;
        g_resolvedSceneTexture = d3t::CreateTexture2D(&colour);
        colour.width = targets.legacyWidth;
        colour.height = targets.legacyHeight;
        g_legacySceneTexture = d3t::CreateTexture2D(&colour);
    } else {
        g_resolvedSceneTexture = g_legacySceneTexture = g_sceneTexture;
    }
    if (!g_sceneTexture || !g_sceneDepthTexture || !g_resolvedSceneTexture || !g_legacySceneTexture) {
        RetireSceneTextures();
        return false;
    }
    g_sceneWidth = targets.width;
    g_sceneHeight = targets.height;
    g_allocatedSceneTargets = targets;
    return true;
#else
    return false;
#endif
}

std::array<float, 4> SceneClearColour() {
    constexpr float scale = 1.0f / 255.0f;
    return {
        static_cast<float>((g_sceneClearColour >> 16U) & 0xffU) * scale,
        static_cast<float>((g_sceneClearColour >> 8U) & 0xffU) * scale,
        static_cast<float>(g_sceneClearColour & 0xffU) * scale,
        static_cast<float>((g_sceneClearColour >> 24U) & 0xffU) * scale,
    };
}

} // namespace
bool ConfigureSceneFrameTargets(const SceneFrameTargets &targets, std::uint32_t inputLayout, std::uint32_t vertexShader, std::uint32_t pixelShader) {
    g_requestedSceneTargets = targets;
    g_resolveInputLayout = inputLayout;
    g_resolveVertexShader = vertexShader;
    g_resolvePixelShader = pixelShader;
    return EnsureSceneTargets();
}

void SelectResolvedSceneFrame(bool resolved) {
    g_useResolvedScene = resolved;
}

bool InitializeUIRenderer(std::uint32_t inputLayout, std::uint32_t vertexShader, std::uint32_t pixelShader, std::uint32_t gammaInputLayout,
    std::uint32_t gammaVertexShader, std::uint32_t gammaPixelShader) {
    g_uiInputLayout = inputLayout;
    g_uiVertexShader = vertexShader;
    g_uiPixelShader = pixelShader;
    g_gammaInputLayout = gammaInputLayout;
    g_gammaVertexShader = gammaVertexShader;
    g_gammaPixelShader = gammaPixelShader;
    if (!g_uiVertexBuffer) {
        const d3t::BufferDescription description{1, 1638400, 0, 2, nullptr, "nuklear sVertexBuffer"};
        g_uiVertexBuffer = d3t::CreateBuffer(&description);
    }
    if (!g_uiIndexBuffer) {
        const d3t::BufferDescription description{2, 450560, 0, 2, nullptr, "nuklear sIndexBuffer"};
        g_uiIndexBuffer = d3t::CreateBuffer(&description);
    }
    if (!g_uiWhiteTexture) {
        constexpr std::array<std::uint32_t, 4> pixels{0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU};
        const d3t::Texture2DDescription description{2, 2, 1, 1, 2, 0, 0, 1, 0, 0, pixels.data()};
        g_uiWhiteTexture = d3t::CreateTexture2D(&description);
    }
    return g_uiVertexBuffer != 0 && g_uiIndexBuffer != 0 && g_uiWhiteTexture != 0 && g_uiInputLayout != 0 && g_uiVertexShader != 0 && g_uiPixelShader != 0 &&
        g_gammaInputLayout != 0 && g_gammaVertexShader != 0 && g_gammaPixelShader != 0 && EnsureSceneTargets();
}

bool BeginUIFrame() {
#if defined(_WIN32)
    if (!g_uiVertexBuffer || !g_uiIndexBuffer || !g_uiWhiteTexture || !g_uiInputLayout || !g_uiVertexShader || !g_uiPixelShader || !g_gammaInputLayout ||
        !g_gammaVertexShader || !g_gammaPixelShader) {
        return false;
    }
    auto *const context = DX11::g_ppD3DDeviceContext;
    g_backBufferTexture = d3t::CreateNewTexture2D();
    if (!context || !g_backBufferTexture || !EnsureSceneTargets()) {
        return false;
    }
    const std::array<float, 4> colour = SceneClearColour();
    constexpr std::array<float, 4> depth{0.0f, 0.0f, 0.0f, 0.0f};
    d3t::UnmapAndClearViews(context, g_backBufferTexture, 0, colour.data());
    d3t::UnmapAndClearViews(context, g_sceneTexture, 0, colour.data());
    d3t::UnmapAndClearViews(context, g_sceneDepthTexture, 0, depth.data());
    return BindSceneFrameRenderTarget();
#else
    return false;
#endif
}

bool BindSceneFrameRenderTarget() {
#if defined(_WIN32)
    auto *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !g_sceneTexture || !g_sceneDepthTexture) {
        return false;
    }
    SetSceneRenderTarget(context, g_sceneTexture, g_sceneDepthTexture);
    SetViewport(context, static_cast<float>(g_sceneWidth), static_cast<float>(g_sceneHeight));
    return true;
#else
    return false;
#endif
}

bool BindUIFrameRenderTarget() {
#if defined(_WIN32)
    auto *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !g_backBufferTexture)
        return false;
    SetRenderTarget(context, g_backBufferTexture);
    SetViewport(context, static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
    return true;
#else
    return false;
#endif
}
bool BeginUITextureRenderTarget(std::uint32_t texture, bool flipVertical) {
#if defined(_WIN32)
    auto *const context = DX11::g_ppD3DDeviceContext;
    D3TTextureBackendView target{};
    if (uiTextureTarget.context || !context || !context->deviceContext || !GetTexture2DBackendView(texture, &target) || !target.view ||
        !(target.bindFlags & 4U) || target.arraySize != 1 || target.mipLevels != 1 || !target.width || !target.height)
        return false;

    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    auto &saved = uiTextureTarget;
    saved.context = context;
    saved.texture = texture;
    saved.width = target.width;
    saved.height = target.height;
    saved.flipVertical = flipVertical;
    std::memcpy(saved.pipeline.data(), context->renderState.data(), saved.pipeline.size());
    native->OMGetRenderTargets(8, saved.renderTargets.data(), &saved.depthTarget);
    native->VSGetShader(&saved.vertexShader, nullptr, nullptr);
    native->PSGetShader(&saved.pixelShader, nullptr, nullptr);
    native->IAGetInputLayout(&saved.inputLayout);
    native->IAGetVertexBuffers(0, 1, &saved.vertexBuffer, &saved.vertexStride, &saved.vertexOffset);
    native->IAGetIndexBuffer(&saved.indexBuffer, &saved.indexFormat, &saved.indexOffset);
    native->IAGetPrimitiveTopology(&saved.topology);
    native->VSGetConstantBuffers(0, 1, &saved.vertexConstants);
    native->PSGetConstantBuffers(1, 1, &saved.pixelConstants);
    // Binding an output also unbinds aliased inputs, including stages which
    // glyph drawing itself does not modify.
    native->VSGetShaderResources(0, 32, saved.shaderResources[0].data());
    native->PSGetShaderResources(0, 32, saved.shaderResources[1].data());
    native->CSGetShaderResources(0, 32, saved.shaderResources[2].data());
    native->PSGetSamplers(0, 1, &saved.sampler);
    native->OMGetBlendState(&saved.blend, saved.blendFactor.data(), &saved.sampleMask);
    native->OMGetDepthStencilState(&saved.depth, &saved.stencilReference);
    native->RSGetState(&saved.rasterizer);
    saved.viewportCount = static_cast<UINT>(saved.viewports.size());
    native->RSGetViewports(&saved.viewportCount, saved.viewports.data());
    saved.scissorCount = static_cast<UINT>(saved.scissors.size());
    native->RSGetScissorRects(&saved.scissorCount, saved.scissors.data());
    SetRenderTarget(context, texture);
    SetViewport(context, static_cast<float>(target.width), static_cast<float>(target.height));
    constexpr std::array<float, 4> transparent{};
    UnmapAndClearViews(context, texture, 0, transparent.data());
    return true;
#else
    static_cast<void>(texture);
    return false;
#endif
}

void RestoreUIRenderTarget() {
#if defined(_WIN32)
    auto &saved = uiTextureTarget;
    if (!saved.context)
        return;
    auto *const context = saved.context;
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    native->OMSetRenderTargets(8, saved.renderTargets.data(), saved.depthTarget);
    native->VSSetShader(saved.vertexShader, nullptr, 0);
    native->PSSetShader(saved.pixelShader, nullptr, 0);
    native->IASetInputLayout(saved.inputLayout);
    native->IASetVertexBuffers(0, 1, &saved.vertexBuffer, &saved.vertexStride, &saved.vertexOffset);
    native->IASetIndexBuffer(saved.indexBuffer, saved.indexFormat, saved.indexOffset);
    native->IASetPrimitiveTopology(saved.topology);
    native->VSSetConstantBuffers(0, 1, &saved.vertexConstants);
    native->PSSetConstantBuffers(1, 1, &saved.pixelConstants);
    native->VSSetShaderResources(0, 32, saved.shaderResources[0].data());
    native->PSSetShaderResources(0, 32, saved.shaderResources[1].data());
    native->CSSetShaderResources(0, 32, saved.shaderResources[2].data());
    native->PSSetSamplers(0, 1, &saved.sampler);
    native->OMSetBlendState(saved.blend, saved.blendFactor.data(), saved.sampleMask);
    native->OMSetDepthStencilState(saved.depth, saved.stencilReference);
    native->RSSetState(saved.rasterizer);
    native->RSSetViewports(saved.viewportCount, saved.viewports.data());
    native->RSSetScissorRects(saved.scissorCount, saved.scissors.data());
    std::memcpy(context->renderState.data(), saved.pipeline.data(), saved.pipeline.size());
    const auto release = [](auto *&resource) {
        if (resource)
            resource->Release();
        resource = nullptr;
    };
    for (auto *&target : saved.renderTargets)
        release(target);
    for (auto &stage : saved.shaderResources)
        for (auto *&resource : stage)
            release(resource);
    release(saved.depthTarget);
    release(saved.vertexShader);
    release(saved.pixelShader);
    release(saved.inputLayout);
    release(saved.vertexBuffer);
    release(saved.indexBuffer);
    release(saved.vertexConstants);
    release(saved.pixelConstants);
    release(saved.sampler);
    release(saved.blend);
    release(saved.depth);
    release(saved.rasterizer);
    saved.context = nullptr;
    saved.texture = 0;
#endif
}

bool PresentSceneFrame() {
#if defined(_WIN32)
    auto *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !g_backBufferTexture || !g_sceneTexture || !GetNativeShader(g_gammaVertexShader, ShaderType::Vertex) ||
        !GetNativeShader(g_gammaPixelShader, ShaderType::Pixel)) {
        return false;
    }
    std::uint32_t source = g_useResolvedScene ? g_resolvedSceneTexture : g_legacySceneTexture;
    if (g_allocatedSceneTargets.supersampling) {
        if (!GetNativeShader(g_resolveVertexShader, ShaderType::Vertex) || !GetNativeShader(g_resolvePixelShader, ShaderType::Pixel))
            return false;
        const float width = static_cast<float>(g_useResolvedScene ? g_allocatedSceneTargets.resolvedWidth : g_allocatedSceneTargets.legacyWidth);
        const float height = static_cast<float>(g_useResolvedScene ? g_allocatedSceneTargets.resolvedHeight : g_allocatedSceneTargets.legacyHeight);
        SetRenderTarget(context, source);
        SetViewport(context, width, height);
        SetShaderState(context, g_resolveVertexShader, g_resolvePixelShader, g_resolveInputLayout);
        SetPipelineState(context, 0xf0000000ULL);
        const float uv = g_useResolvedScene ? 1.0f : 0.44444445f;
        const std::array<std::array<float, 4>, 4> resolveVertices{
            {{{-1.0f, -1.0f, 0.0f, uv}}, {{1.0f, -1.0f, uv, uv}}, {{-1.0f, 1.0f, 0.0f, 0.0f}}, {{1.0f, 1.0f, uv, 0.0f}}}};
        if (void *const mapped = d3t::GetMappedConstantBuffer(context, 0, 64))
            std::memcpy(mapped, resolveVertices.data(), sizeof(resolveVertices));
        BindPixelTexture8(context, g_sceneTexture);
        SetTopology(context, 1);
        d3t::DX11_ResetMappedResources(context);
        d3t::DX11_UnmapAndSyncShaderBindings(context);
        d3t::DX11_SetupShadersForDXAndSetViewports(context);
        static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
        SetTopology(context, 0);
    }
    if (g_requestedSceneTargets.width)
        SetBackBufferAndScreenRect(context);
    else {
        SetRenderTarget(context, g_backBufferTexture);
        SetViewport(context, static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
    }
    SetShaderState(context, g_gammaVertexShader, g_gammaPixelShader, g_gammaInputLayout);
    SetPipelineState(context, 0xf0000000ULL);
    const std::array<std::array<float, 4>, 4> vertices{{
        {{-1.0f, -1.0f, 0.0f, 1.0f}},
        {{1.0f, -1.0f, 1.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f, 0.0f}},
    }};
    if (void *const mapped = d3t::GetMappedConstantBuffer(context, 8, 64)) {
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    }
    BindPixelTexture8(context, source);
    SetTopology(context, 1);
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    native->Draw(4, 0);
    SetTopology(context, 0);
    return true;
#else
    return false;
#endif
}

bool DrawFullscreenContrast(std::uint32_t vertexShader, std::uint32_t pixelShader, float contrast) {
#if defined(_WIN32)
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !g_backBufferTexture || !GetNativeShader(vertexShader, ShaderType::Vertex) || !GetNativeShader(pixelShader, ShaderType::Pixel))
        return false;
    SetShaderState(context, vertexShader, pixelShader, 0);
    SetBackBufferAndScreenRect(context);
    if (void *const mapped = MapTempConstantBufferForWriteDiscard(context, 2, 4, 4))
        std::memcpy(mapped, &contrast, sizeof(contrast));
    std::byte *const state = context->renderState.data();
    if (ReadState<std::uint32_t>(state, 0x954) != 1U) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, std::uint32_t{1});
    }
    if (ReadState<std::uint64_t>(state, 0x794) != 0xf0000121ULL) {
        state[0x975] |= std::byte{8};
        WriteState(state, 0x794, std::uint64_t{0xf0000121ULL});
    }
    SetTopology(context, 1);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
    return true;
#else
    static_cast<void>(vertexShader);
    static_cast<void>(pixelShader);
    static_cast<void>(contrast);
    return false;
#endif
}

void ReleaseSceneFrameTargets() {
    RetireSceneTextures();
    g_requestedSceneTargets = {};
    g_sceneClearColour = 0;
}

void SetSceneClearColour(std::uint32_t colour) {
    g_sceneClearColour = colour;
}

bool GetSceneFrameTargetSize(float *width, float *height) {
    if (!width || !height || !g_sceneTexture || !g_sceneWidth || !g_sceneHeight)
        return false;
    std::uint32_t textureWidth = 0;
    std::uint32_t textureHeight = 0;
    if (!GetTexture2DDimensions(g_sceneTexture, &textureWidth, &textureHeight) || textureWidth != g_sceneWidth || textureHeight != g_sceneHeight) {
        return false;
    }
    *width = static_cast<float>(g_sceneWidth);
    *height = static_cast<float>(g_sceneHeight);
    return true;
}

bool GetSceneViewportDepthRange(float *minDepth, float *maxDepth) {
    auto *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !minDepth || !maxDepth)
        return false;
    const std::byte *const state = context->renderState.data();
    *minDepth = ReadState<float>(state, 0x880);
    *maxDepth = ReadState<float>(state, 0x884);
    return true;
}

bool SetSceneViewportDepthRange(float minDepth, float maxDepth) {
    auto *const context = DX11::g_ppD3DDeviceContext;
    return SetViewportDepthRange(context, minDepth, maxDepth);
}

UIRect ResolveUIPanelBounds(const UIPanelDefinition &panel, const UIRect &parent, float authoredParentWidth, float authoredParentHeight) {
    UIRect bounds{};
    ScaleUILayout(panel.scaleStyle, panel.width, panel.height, parent, authoredParentWidth, authoredParentHeight, bounds.width, bounds.height);
    bounds.width *= panel.scaleX;
    bounds.height *= panel.scaleY;
    bounds.x = AlignedPosition(panel.x.align, parent.x, parent.width, bounds.width, panel.x.position, panel.x.offset, true);
    bounds.y = AlignedPosition(panel.y.align, parent.y, parent.height, bounds.height, panel.y.position, panel.y.offset, false);
    return bounds;
}

void ResolveUIDefinitionLayout(const UIDefinition &definition, std::span<const std::int32_t> parents, std::span<UIRect> panels,
    std::span<UIResolvedTextureObject> objects, std::span<UIResolvedLabel> labels, std::span<const std::int32_t> objectPanels,
    std::span<const std::int32_t> labelPanels, std::span<const UIPanelBoundsOverride> overrides, std::array<float, 2> paddingScale) {





    for (UIRect &bounds : panels)
        bounds.height = std::numeric_limits<float>::quiet_NaN();
    const auto resolvePanel = [&](auto &&self, std::size_t index) -> const UIRect & {
        UIRect &bounds = panels[index];
        if (!std::isnan(bounds.height))
            return bounds;
        for (const UIPanelBoundsOverride &override : overrides)
            if (override.panel == index) {
                bounds = override.bounds;
                return bounds;
            }
        const UIPanelDefinition &panel = definition.panels[index];
        const std::int32_t parentIndex = parents[index];
        const UIRect parent = parentIndex < 0 ? UIRect{0, 0, 1920, 1080} : self(self, static_cast<std::size_t>(parentIndex));
        const UIPanelDefinition *const source = parentIndex < 0 ? nullptr : &definition.panels[parentIndex];
        bounds = ResolveUIPanelBounds(panel, parent, source ? source->width : 1920.0f, source ? source->height : 1080.0f);
        return bounds;
    };
    for (std::size_t index = 0; index < definition.panels.size(); ++index)
        resolvePanel(resolvePanel, index);
    const auto place = [&](const auto &object, std::int32_t index, UIRect &bounds, UIRect &clip, bool fullscreen, float paddingX, float paddingY) {
        if (index < 0) {
            bounds = {};
            clip = {};
            return;
        }
        const UIPanelDefinition &panel = definition.panels[index];
        UIRect parent = panels[index];
        parent.x += panel.paddingX;
        parent.y += panel.paddingY;
        parent.width = std::max(0.0f, parent.width - panel.paddingX * 2.0f);
        parent.height = std::max(0.0f, parent.height - panel.paddingY * 2.0f);
        // Fullscreen changes the texture's sampling rectangle, not its owning
        // panel/window scissor (S2 4D5A80 -> 48D520). Keep it inside that window.
        clip = parent;
        if (fullscreen)
            parent = {0, 0, 1920, 1080};
        ScaleUILayout(object.scaleStyle, object.width, object.height, parent, panel.width, panel.height, bounds.width, bounds.height);
        const float width = bounds.width, height = bounds.height;
        bounds.x = AlignedPosition(
            object.x.align, parent.x, parent.width, width, object.x.position * parent.width / 1920.0f, object.x.offset * parent.width / 1920.0f, true);
        bounds.y = AlignedPosition(
            object.y.align, parent.y, parent.height, height, object.y.position * parent.height / 1080.0f, object.y.offset * parent.height / 1080.0f, false);
        const float px = paddingX * paddingScale[0], py = paddingY * paddingScale[1];
        bounds.width -= px * 2.0f;
        bounds.height -= py * 2.0f;
        bounds.x += px + (bounds.width - bounds.width * object.scaleX) * 0.5f;
        bounds.y += py + (bounds.height - bounds.height * object.scaleY) * 0.5f;
        bounds.width *= object.scaleX;
        bounds.height *= object.scaleY;
    };
    for (std::size_t index = 0; index < objects.size(); ++index)
        place(definition.textureObjects[index], objectPanels[index], objects[index].bounds, objects[index].clip, definition.textureObjects[index].fullscreen,
            definition.textureObjects[index].paddingX, definition.textureObjects[index].paddingY);
    for (std::size_t index = 0; index < labels.size(); ++index)
        place(definition.labels[index], labelPanels[index], labels[index].bounds, labels[index].clip, false, 0.0f, 0.0f);
}

std::vector<UIResolvedLabel> GetUILabelRectangles(const UIDefinition &definition) {
    const auto panels = ResolvePanelRectangles(definition, true);
    std::vector<UIResolvedLabel> result;
    result.reserve(definition.labels.size());
    for (const auto &label : definition.labels) {
        const auto found = panels.find(label.panelId);
        const auto *panelDefinition = definition.findPanel(label.panelId);
        if (found == panels.end() || !panelDefinition) {
            result.push_back({});
            continue;
        }
        UIRect parent = found->second;
        parent.x += panelDefinition->paddingX;
        parent.y += panelDefinition->paddingY;
        parent.width = std::max(0.0f, parent.width - panelDefinition->paddingX * 2.0f);
        parent.height = std::max(0.0f, parent.height - panelDefinition->paddingY * 2.0f);
        UIRect bounds{};
        ScaleUILayout(label.scaleStyle, label.width, label.height, parent, panelDefinition->width, panelDefinition->height, bounds.width, bounds.height);
        const float width = bounds.width;
        const float height = bounds.height;
        bounds.width *= label.scaleX;
        bounds.height *= label.scaleY;
        bounds.x = AlignedPosition(label.x.align, parent.x, parent.width, width, label.x.position, label.x.offset, true) + (width - bounds.width) * 0.5f;
        bounds.y = AlignedPosition(label.y.align, parent.y, parent.height, height, label.y.position, label.y.offset, false) + (height - bounds.height) * 0.5f;
        result.push_back({bounds, parent});
    }
    return result;
}

std::vector<UIResolvedTextureObject> GetUITextureObjectRectangles(const UIDefinition &definition) {
    const auto panels = ResolvePanelRectangles(definition, true);
    std::vector<UIResolvedTextureObject> result;
    result.reserve(definition.textureObjects.size());
    for (const auto &object : definition.textureObjects) {
        const auto found = panels.find(object.panelId);
        const auto *panel = definition.findPanel(object.panelId);
        if (found == panels.end() || !panel) {
            result.push_back({});
            continue;
        }
        UIRect parent = found->second;
        parent.x += panel->paddingX;
        parent.y += panel->paddingY;
        parent.width = std::max(0.0f, parent.width - panel->paddingX * 2.0f);
        parent.height = std::max(0.0f, parent.height - panel->paddingY * 2.0f);
        UIRect bounds{};
        ScaleUILayout(object.scaleStyle, object.width, object.height, parent, panel->width, panel->height, bounds.width, bounds.height);
        const float width = bounds.width;
        const float height = bounds.height;
        bounds.width *= object.scaleX;
        bounds.height *= object.scaleY;
        bounds.x = AlignedPosition(object.x.align, parent.x, parent.width, width, object.x.position, object.x.offset, true) + (width - bounds.width) * 0.5f;
        bounds.y =
            AlignedPosition(object.y.align, parent.y, parent.height, height, object.y.position, object.y.offset, false) + (height - bounds.height) * 0.5f;
        result.push_back({bounds, parent});
    }
    return result;
}

std::vector<UIRect> GetUIGaugeRectangles(const UIDefinition &definition) {
    const auto panels = ResolvePanelRectangles(definition, true);
    std::vector<UIRect> rectangles;
    rectangles.reserve(definition.gauges.size());
    for (const auto &gauge : definition.gauges) {
        const auto found = panels.find(gauge.panelId);
        rectangles.push_back(found != panels.end() ? found->second : UIRect{});
    }
    return rectangles;
}

std::uint32_t DrawUIGauge(const UIDefinition &definition, const UIGaugeState &gauge, const UIRect &bounds, float opacity) {
    if (gauge.hidden == 1 || bounds.width <= 0.0f || bounds.height <= 0.0f)
        return 0;
    const float sx = static_cast<float>(g_originalScreenWidth) / 1920.0f;
    const float sy = static_cast<float>(g_originalScreenHeight) / 1080.0f;
    const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(bounds.x * sx), static_cast<std::int32_t>(bounds.y * sy),
        static_cast<std::int32_t>((bounds.x + bounds.width) * sx), static_cast<std::int32_t>((bounds.y + bounds.height) * sy)};
    const auto draw = [&](std::int32_t imageIndex, const UIRect &rect, std::uint32_t colour) -> std::uint32_t {
        if (imageIndex < 0 || static_cast<std::size_t>(imageIndex) >= definition.images.size())
            return 0;
        const auto texture = definition.images[imageIndex].textureHandle;
        const auto alpha = static_cast<std::uint32_t>((colour >> 24U) * std::clamp(opacity, 0.0f, 1.0f));
        colour = (colour & 0x00ffffffU) | (alpha << 24U);
        const std::array<UITexturedVertex, 4> vertices{{
            {rect.x * sx, rect.y * sy, 0, 0, colour},
            {(rect.x + rect.width) * sx, rect.y * sy, 1, 0, colour},
            {(rect.x + rect.width) * sx, (rect.y + rect.height) * sy, 1, 1, colour},
            {rect.x * sx, (rect.y + rect.height) * sy, 0, 1, colour},
        }};
        constexpr std::array<std::uint16_t, 6> indices{0, 1, 2, 0, 2, 3};
        return DrawUITexturedGeometry(texture, vertices, indices, scissor) ? 1U : 0U;
    };
    std::uint32_t submitted = gauge.transparentBackground ? 0 : draw(gauge.backgroundImage, bounds, gauge.colour);
    const auto orbRect = [&](std::int32_t index) {
        return UIRect{bounds.x + bounds.width * gauge.positions[index][0], bounds.y + bounds.height * gauge.positions[index][1], bounds.width * gauge.width,
            bounds.height * gauge.height};
    };
    // 3DE250 draws a complete empty orb beneath the partially opaque final
    // filled orb. Empty trailing orbs belong only to the primary widget.
    for (std::int32_t index = 0; index < gauge.filledCount && index < 12; ++index) {
        const UIRect rect = orbRect(index);
        std::uint32_t colour = 0xffffffffU;
        if (gauge.translucentLastOrb == 1 && index == gauge.filledCount - 1) {
            submitted += draw(gauge.images[8], rect, colour);
            const auto alpha = static_cast<std::uint8_t>(static_cast<std::int64_t>(gauge.lastOrbAlpha));
            colour = 0x00ffffffU | (static_cast<std::uint32_t>(alpha) << 24U);
        }
        const auto orbColour = gauge.orbColours[index];
        if (orbColour >= 0 && orbColour < 9)
            submitted += draw(gauge.images[orbColour], rect, colour);
    }
    if (!gauge.suppressEmptyOrbs) {
        for (std::int32_t index = std::max(0, gauge.filledCount); index < gauge.count && index < 12; ++index)
            submitted += draw(gauge.images[8], orbRect(index), 0xffffffffU);
    }
    return submitted;
}

bool DrawUIRoundedRectangle(const UIRect &bounds, float radius, std::uint32_t colour, const UIRect &clip) {
    if (bounds.width <= 0.0f || bounds.height <= 0.0f || !(colour >> 24U))
        return false;
    const float sx = static_cast<float>(g_originalScreenWidth) / 1920.0f;
    const float sy = static_cast<float>(g_originalScreenHeight) / 1080.0f;
    radius = std::min(radius, std::min(bounds.width, bounds.height) * 0.5f);
    std::array<UITexturedVertex, 21> vertices{};
    std::array<std::uint16_t, 60> indices{};
    vertices[0] = {(bounds.x + bounds.width * 0.5f) * sx, (bounds.y + bounds.height * 0.5f) * sy, 0.5f, 0.5f, colour};
    for (std::uint16_t corner = 0; corner < 4; ++corner) {
        const float cx = bounds.x + (corner == 0 || corner == 3 ? radius : bounds.width - radius);
        const float cy = bounds.y + (corner < 2 ? radius : bounds.height - radius);
        for (std::uint16_t step = 0; step < 5; ++step) {
            const std::uint16_t index = 1 + corner * 5 + step;
            const std::array<float, 2> &direction = UI_ROUNDED_RECT_DIRECTIONS[index - 1];
            vertices[index] = {(cx + direction[0] * radius) * sx, (cy + direction[1] * radius) * sy, 0.5f, 0.5f, colour};
            indices[(index - 1) * 3] = 0;
            indices[(index - 1) * 3 + 1] = index;
            indices[(index - 1) * 3 + 2] = index == 20 ? 1 : index + 1;
        }
    }
    const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(clip.x * sx), static_cast<std::int32_t>(clip.y * sy),
        static_cast<std::int32_t>((clip.x + clip.width) * sx), static_cast<std::int32_t>((clip.y + clip.height) * sy)};
    return DrawUITexturedGeometry(g_defaultWhiteTexture, vertices, indices, scissor);
}

bool DrawUITextureObject(const UIDefinition &definition, const UITextureObjectDefinition &object, const UIResolvedTextureObject &placement, float opacity,
    const UISharedSpriteImage *sharedImage) {
    if (object.hidden || opacity <= 0.0f || (object.kind == UITextureObjectKind::Sprite && object.frameIndex == -1))
        return false;
    if (object.kind == UITextureObjectKind::ColourButton) {
        const std::uint32_t colour =
            (object.colour & 0x00ffffffU) | (static_cast<std::uint32_t>((object.colour >> 24U) * std::clamp(opacity, 0.0f, 1.0f)) << 24U);
        return DrawUIRoundedRectangle(placement.bounds, object.rounding, colour, placement.clip);
    }
    std::uint32_t texture = 0;
    UIRect source{};
    if (sharedImage || object.kind == UITextureObjectKind::ButtonIcon || (object.kind == UITextureObjectKind::Sprite && object.useShared)) {
        if (!sharedImage)
            return false;
        texture = sharedImage->textureHandle;
        source = sharedImage->source;
    } else {
        const auto *image = definition.findImage(object.imageName);
        if (!image)
            return false;
        texture = image->textureHandle;
        if (object.kind == UITextureObjectKind::Sprite) {
            const auto sheet = std::find_if(
                definition.spriteSheets.begin(), definition.spriteSheets.end(), [&](const auto &entry) { return entry.imageName == object.imageName; });
            if (sheet == definition.spriteSheets.end() || object.frameIndex < 0 || static_cast<std::size_t>(object.frameIndex) >= sheet->frames.size())
                return false;
            const auto &frame = sheet->frames[object.frameIndex];
            source = {frame.x, frame.y, frame.width, frame.height};
        }
    }
    std::uint32_t textureWidth = 0;
    std::uint32_t textureHeight = 0;
    if (!GetTexture2DDimensions(texture, &textureWidth, &textureHeight) || !textureWidth || !textureHeight)
        return false;
    if (object.kind == UITextureObjectKind::Image) {
        source = {0, 0, static_cast<float>(textureWidth), static_cast<float>(textureHeight)};
    } else {

        source.x = static_cast<std::uint16_t>(source.x);
        source.y = static_cast<std::uint16_t>(source.y);
        source.width = static_cast<std::uint16_t>(source.width);
        source.height = static_cast<std::uint16_t>(source.height);
    }
    const float u0 = source.x / textureWidth;
    const float v0 = source.y / textureHeight;
    const float u1 = (source.x + source.width) / textureWidth;
    const float v1 = (source.y + source.height) / textureHeight;
    const auto &bounds = placement.bounds;
    const float cx = bounds.x + bounds.width * 0.5f;
    const float cy = bounds.y + bounds.height * 0.5f;
    const float sx = static_cast<float>(g_originalScreenWidth) / 1920.0f;
    const float sy = static_cast<float>(g_originalScreenHeight) / 1080.0f;
    const float sine = std::sin(object.rotation);
    const float cosine = std::cos(object.rotation);
    const auto alpha = static_cast<std::uint32_t>((object.colour >> 24U) * std::clamp(opacity, 0.0f, 1.0f));
    const auto colour = (object.colour & 0x00ffffffU) | (alpha << 24U);
    std::array<UITexturedVertex, 4> vertices{{
        {-bounds.width * 0.5f, -bounds.height * 0.5f, u0, v0, colour},
        {bounds.width * 0.5f, -bounds.height * 0.5f, u1, v0, colour},
        {bounds.width * 0.5f, bounds.height * 0.5f, u1, v1, colour},
        {-bounds.width * 0.5f, bounds.height * 0.5f, u0, v1, colour},
    }};
    for (auto &vertex : vertices) {
        const float x = vertex.x;
        const float y = vertex.y;
        vertex.x = (cx + x * cosine - y * sine) * sx;
        vertex.y = (cy + x * sine + y * cosine) * sy;
    }
    constexpr std::array<std::uint16_t, 6> indices{0, 1, 2, 0, 2, 3};
    const auto &clip = placement.clip;
    const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(clip.x * sx), static_cast<std::int32_t>(clip.y * sy),
        static_cast<std::int32_t>((clip.x + clip.width) * sx), static_cast<std::int32_t>((clip.y + clip.height) * sy)};
    return DrawUITexturedGeometry(texture, vertices, indices, scissor);
}

std::uint32_t DrawUIDefinition(const UIDefinition &definition, float opacity) {
    const auto rectangles = ResolvePanelRectangles(definition);
    std::uint32_t submitted = 0;
    for (const UIPanelDefinition &panel : definition.panels) {
        const auto found = rectangles.find(panel.id);
        if (found == rectangles.end())
            continue;
        submitted += DrawPanel(definition, panel, found->second, opacity) ? 1U : 0U;
    }
    return submitted;
}

std::uint32_t DrawUIPanels(const UIDefinition &definition, std::span<const UIPanelDrawState> panels) {
    const auto rectangles = ResolvePanelRectangles(definition);
    std::uint32_t submitted = 0;
    for (const UIPanelDrawState &state : panels) {
        const UIPanelDefinition *const panel = definition.findPanel(std::string(state.panelId));
        if (!panel)
            continue;
        const auto found = rectangles.find(panel->id);
        if (found == rectangles.end())
            continue;
        submitted += DrawPanel(definition, *panel, found->second, state.opacity) ? 1U : 0U;
    }
    return submitted;
}

bool DrawUIImage(std::uint32_t texture, float x, float y, float width, float height, float opacity) {
    return SubmitUIImage(texture, x, y, width, height, opacity);
}

bool DrawUITexturedGeometry(
    std::uint32_t texture, std::span<const UITexturedVertex> vertices, std::span<const std::uint16_t> indices, const std::array<std::int32_t, 4> &scissor) {
#if defined(_WIN32)
    auto *const context = DX11::g_ppD3DDeviceContext;
    const auto target = uiTextureTarget.context ? uiTextureTarget.texture : g_backBufferTexture;
    const float targetWidth = static_cast<float>(uiTextureTarget.context ? uiTextureTarget.width : g_originalScreenWidth);
    const float targetHeight = static_cast<float>(uiTextureTarget.context ? uiTextureTarget.height : g_originalScreenHeight);
    if (!context || !target || !texture || vertices.empty() || indices.empty() || !GetNativeShader(g_uiVertexShader, ShaderType::Vertex) ||
        !GetNativeShader(g_uiPixelShader, ShaderType::Pixel) || !UploadUIGeometry(context, vertices, indices))
        return false;
    SetRenderTarget(context, target);
    SetViewport(context, targetWidth, targetHeight);
    SetShaderState(context, g_uiVertexShader, g_uiPixelShader, g_uiInputLayout);
    SetPipelineState(context, 0xf0138293ULL);
    std::byte *const state = context->renderState.data();
    constexpr std::uint32_t scissoredRasterizer = 5;
    if (ReadState<std::uint32_t>(state, 0x954) != scissoredRasterizer) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, scissoredRasterizer);
    }
    if (std::memcmp(state + 0x860, scissor.data(), sizeof(scissor)) != 0) {
        std::memcpy(state + 0x860, scissor.data(), sizeof(scissor));
        state[0x976] |= std::byte{4};
    }
    SetGeometryState(context);
    SetConstants(context, targetWidth, targetHeight);
    BindTextures(context, texture);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->DrawIndexed(static_cast<std::uint32_t>(indices.size()), 0, 0);
    if (ReadState<std::uint32_t>(state, 0x954) != 17U) {
        state[0x975] |= std::byte{0x20};
        WriteState(state, 0x954, std::uint32_t{17});
    }
    return true;
#else
    static_cast<void>(texture);
    static_cast<void>(vertices);
    static_cast<void>(indices);
    static_cast<void>(scissor);
    return false;
#endif
}

} // namespace shenmue::d3t
