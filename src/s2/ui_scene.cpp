#include "s2/ui_scene.hpp"

#include "s2/d3t_renderer.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/render_state.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_renderer.hpp"
#include <array>
#include <bit>
#include <cstring>

namespace shenmue::s2 {
UISceneTarget g_uiSceneTarget{};
UISceneTarget g_uiSceneAuxiliaryTarget{};
namespace {
UISceneTarget *activeTarget{};
void (*sceneCallback)(){};
}

void CreateTexture2DForNuklear(std::int32_t width, std::int32_t height) {
    g_uiSceneTarget.width = width;
    g_uiSceneTarget.height = height;
    d3t::Texture2DDescription description{.width = static_cast<std::uint32_t>(width),
        .height = static_cast<std::uint32_t>(height),
        .arraySize = 1,
        .mipLevels = 1,
        .format = 2,
        .bindFlags = 4,
        .flags = 0,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = nullptr};
    g_uiSceneTarget.colour = d3t::CreateTexture2D(&description);
    description.format = 41;
    description.bindFlags = 8;
    g_uiSceneTarget.depth = d3t::CreateTexture2D(&description);
}

void BindUISceneImage(std::int32_t index, d3t::UIViewImage *image) {
    if (!image || image->textureHandle == -1)
        return;
    if (index == 0)
        activeTarget = &g_uiSceneTarget;
    else if (index == 1)
        activeTarget = &g_uiSceneAuxiliaryTarget;
    else
        return;
    std::uint32_t width{}, height{};
    std::uint32_t texture = activeTarget->colour;
    if (!d3t::GetTexture2DDimensions(texture, &width, &height)) {
        texture = d3t::g_defaultWhiteTexture;
        d3t::GetTexture2DDimensions(texture, &width, &height);
    }
    image->textureHandle = static_cast<std::int32_t>(texture);
    image->width = static_cast<std::uint16_t>(width);
    image->height = static_cast<std::uint16_t>(height);
    image->region = {0, 0, image->width, image->height};
}

void SetUISceneCallback(void (*callback)()) {
    sceneCallback = callback;
}

bool BeginUISceneRenderQueue() {
#if defined(_WIN32)
    if (!BeginD3TRenderPass(false))
        return false;
    d3t::D3TRenderState &state = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleList)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    if (activeTarget) {
        const float width =
            static_cast<float>(activeTarget->height) * static_cast<float>(d3t::g_originalScreenWidth) / static_cast<float>(d3t::g_originalScreenHeight);
        const std::array<float, 6> viewport{
            (width - static_cast<float>(activeTarget->width)) * -0.5f, 0.0f, width, static_cast<float>(activeTarget->height), 0.0f, 1.0f};
        if (std::memcmp(state.viewport.data(), viewport.data(), sizeof(viewport)) != 0) {
            state.viewport = viewport;
            state.viewportDirtyMask |= 1;
        }
        constexpr float clearComponent = std::bit_cast<float>(0xffffffffU);
        constexpr std::array<float, 4> colour{clearComponent, clearComponent, clearComponent, clearComponent};
        constexpr std::array<float, 4> depth{};
        d3t::UnmapAndClearViews(d3t::DX11::g_ppD3DDeviceContext, activeTarget->colour, 0, colour.data());
        d3t::UnmapAndClearViews(d3t::DX11::g_ppD3DDeviceContext, activeTarget->depth, 0, depth.data());
        state.outputTargets.fill(std::byte{});
        std::memcpy(state.outputTargets.data(), &activeTarget->colour, sizeof(activeTarget->colour));
        std::memcpy(state.outputTargets.data() + 0x80, &activeTarget->depth, sizeof(activeTarget->depth));
        state.bufferStateFlags |= d3t::BufferOutputTargets;
    }
    return true;
#else
    return false;
#endif
}

void RenderUIScene() {
    if (!sceneCallback)
        return;
    if (!BeginUISceneRenderQueue())
        return;
    sceneCallback();
    ExecuteCurrentCommandList();
}
} // namespace shenmue::s2
