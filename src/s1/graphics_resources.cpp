#include "s1/graphics_resources.hpp"
#include "s1/d3t/namespace.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/screen_space.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {
namespace {

std::int32_t g_rgba8ResourceCount = 0;
TextureResourceDescriptor *g_rgba8Resources = nullptr;
std::uint32_t g_rgba8FullScreenTexture = 0;

} // namespace

std::uint8_t g_hasSetupBuffersForTextures = 0;

std::int64_t ReleaseRGBA8TextureResources() {
    if (!g_rgba8Resources) {
        return 0;
    }
    for (std::int32_t index = 0; index < g_rgba8ResourceCount; ++index) {
        auto &resource = g_rgba8Resources[index];
        if (resource.texture) {
            InitTexture2DResource(static_cast<std::uint32_t>(resource.texture));
            resource.texture = 0;
        }
    }
    g_rgba8ResourceCount = 0;
    g_rgba8Resources = nullptr;
    return 1;
}

namespace {

Texture2DDescription MakeRGBA8RenderTarget(std::uint32_t width, std::uint32_t height) {
    return Texture2DDescription{
        width,
        height,
        1,
        1,
        2,
        4,
        0,
        1,
        0,
        0,
        nullptr,
    };
}

} // namespace

TextureResourceDescriptor g_generalTextureResources[3]{};
std::uint32_t g_fullScreenTexture2D = 0;
std::uint32_t g_abufferCompositionTexture2D = 0;
std::uint32_t g_originalStripsTexture2D = 0;
std::uint32_t g_auxiliaryTexture2D = 0;
std::uint32_t g_superSamplingTexture2D = 0;
std::uint32_t g_legacyFullScreenTexture2D = 0;
std::uint32_t g_fullScreenUintArrayTexture2D = 0;
std::uint32_t g_fullScreenFloatArrayTexture2D = 0;
std::uint32_t g_fullScreenUintTexture2D = 0;
std::uint32_t g_instanceBuffer = 0;
std::uint32_t g_billboardBuffer = 0;
std::uint32_t g_perObjectBuffer = 0;
std::uint32_t g_dedicatedShadowBuffer = 0;
ScreenWindow g_screenWindow{};
float g_superSampling = 0.0f;
float g_viewportInterpolationProgress = 1.0f;
float g_depthPassProgress = 1.0f;
float g_renderPassProgress = 1.0f;
std::uint8_t g_bufferPassNotBusy = 0;
float g_viewportScaleX = 1.0f;
float g_viewportScaleY = 1.0f;
std::uint8_t g_superSamplingTextureActive = 1;

void SetupGlobalRenderBuffers() {
    BufferDescription description{
        3,
        0x28000,
        40,
        2,
        nullptr,
        "Instance Buffer",
    };
    g_instanceBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));

    description.flags = 2;
    description.size = 0x80000;
    description.count = 0;
    description.name = "Billboard Buffer";
    g_billboardBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));

    struct PerObjectData {
        Matrix4x4 matrices[2];
        std::int32_t tail[8];
    };
    static_assert(sizeof(PerObjectData) == 0xa0);
    const Matrix4x4 identity{{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    const PerObjectData perObject{
        {identity, identity},
        {-1, -1, 0, 1, 0, 0, 0, 0},
    };
    description.flags = 0;
    description.size = sizeof(perObject);
    description.flags3 = 0;
    description.buffer = &perObject;
    description.name = "Default per object Buffer";
    g_perObjectBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));

    const std::array<std::byte, 40> dedicatedShadow{};
    description.size = static_cast<std::int32_t>(dedicatedShadow.size());
    description.buffer = dedicatedShadow.data();
    description.name = "Default dedicated shadow Buffer";
    g_dedicatedShadowBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));
}

void SetupImmutableDXTex2D() {
#if defined(_WIN32)
    const std::uint32_t backBuffer = CreateNewTexture2D();
    Texture2DDescription backBufferDescription{};
    if (!GetTexture2DDescription(backBuffer, &backBufferDescription)) {
        return;
    }
    const Texture2DDescription description{
        backBufferDescription.width,
        backBufferDescription.height,
        1,
        1,
        backBufferDescription.format,
        0,
        0,
        1,
        0,
        0,
        nullptr,
    };
    g_screenWindow.texture2D[0] = CreateTexture2D(&description);
#endif
}

std::int64_t RGBA8_CreateTexture2D() {
    if (g_rgba8ResourceCount) {
        ReleaseRGBA8TextureResources();
    }
    g_rgba8ResourceCount = 3;
    g_rgba8Resources = g_generalTextureResources;

    for (TextureResourceDescriptor &resource : g_generalTextureResources) {
        if (resource.format != 18) {
            return 0;
        }
        InitTexture2DResource(static_cast<std::uint32_t>(resource.texture));
        const Texture2DDescription description = MakeRGBA8RenderTarget(resource.width, resource.height);
        resource.texture = CreateTexture2D(&description);
    }

    std::uint32_t width;
    std::uint32_t height;
    if (!GetTexture2DDimensions(g_fullScreenTexture2D, &width, &height)) {
        return 0;
    }
    InitTexture2DResource(g_rgba8FullScreenTexture);
    const Texture2DDescription description = MakeRGBA8RenderTarget(width, height);
    g_rgba8FullScreenTexture = CreateTexture2D(&description);
    return 1;
}

void InitGeneralTextureResources() {
    const std::int32_t originalHeight = static_cast<std::int32_t>(g_originalScreenHeight);
    const std::int32_t originalWidth = static_cast<std::int32_t>(g_originalScreenWidth);

    const float superSamplingAmount = g_currentSettings.display.superSampling + 1.0f;
    g_superSampling = g_currentSettings.display.superSampling;
    const float totalResolutionScale = g_currentSettings.display.resolutionScale * 0.6f + 0.4f;
    const float scaledWidth = originalWidth * totalResolutionScale;
    const float scaledHeight = originalHeight * totalResolutionScale;
    const std::uint32_t screenWidth = (static_cast<std::uint32_t>(scaledWidth * superSamplingAmount) + 7U) & ~7U;
    const std::uint32_t screenHeight = (static_cast<std::uint32_t>(scaledHeight * superSamplingAmount) + 7U) & ~7U;

    InitTexture2DResource(g_fullScreenTexture2D);
    InitTexture2DResource(g_abufferCompositionTexture2D);
    InitTexture2DResource(g_originalStripsTexture2D);
    InitTexture2DResource(g_auxiliaryTexture2D);
    InitTexture2DResource(g_superSamplingTexture2D);
    InitTexture2DResource(g_legacyFullScreenTexture2D);
    InitTexture2DResource(g_fullScreenUintArrayTexture2D);
    InitTexture2DResource(g_fullScreenFloatArrayTexture2D);
    InitTexture2DResource(g_fullScreenUintTexture2D);

    Texture2DDescription description{
        screenWidth,
        screenHeight,
        1,
        1,
        4,
        4,
        0,
        1,
        0,
        0,
        nullptr,
    };
    g_fullScreenTexture2D = CreateTexture2D(&description);
    g_abufferCompositionTexture2D = CreateTexture2D(&description);

    if (superSamplingAmount <= 1.0f) {
        g_superSamplingTexture2D = g_fullScreenTexture2D;
        g_legacyFullScreenTexture2D = g_fullScreenTexture2D;
    } else {
        description.width = (static_cast<std::uint32_t>(scaledWidth) + 7U) & ~7U;
        description.height = (static_cast<std::uint32_t>(scaledHeight) + 7U) & ~7U;
        g_superSamplingTexture2D = CreateTexture2D(&description);

        description.width = (static_cast<std::uint32_t>(480 * originalWidth / originalHeight) + 7U) & ~7U;
        description.height = 480;
        g_legacyFullScreenTexture2D = CreateTexture2D(&description);
        description.width = screenWidth;
        description.height = screenHeight;
    }

    description.format = 41;
    description.bindFlags = 8;
    g_originalStripsTexture2D = CreateTexture2D(&description);

    description.format = 10;
    description.bindFlags = 4;
    description.flags = 0;
    g_auxiliaryTexture2D = CreateTexture2D(&description);
    SetScreenDimensions(screenWidth, screenHeight);

    description = Texture2DDescription{
        screenWidth,
        screenHeight,
        16,
        1,
        16,
        0x20,
        0,
        1,
        0,
        0,
        nullptr,
    };
    g_fullScreenUintArrayTexture2D = CreateTexture2D(&description);
    description.format = 17;
    g_fullScreenFloatArrayTexture2D = CreateTexture2D(&description);
    description.arraySize = 1;
    description.format = 16;
    description.flags = 4;
    g_fullScreenUintTexture2D = CreateTexture2D(&description);
    g_hasSetupBuffersForTextures = 1;
}

std::int64_t ScaleViewport(float width, float height) {
    std::uint32_t textureWidth = 0;
    std::uint32_t textureHeight = 0;
    if (!GetTexture2DDimensions(g_fullScreenTexture2D, &textureWidth, &textureHeight))
        return 0;
    const float requestedWidth = width < 0.0f ? static_cast<float>(textureWidth) : width;
    const float requestedHeight = height < 0.0f ? static_cast<float>(textureHeight) : height;
    constexpr float baseScaleFactor = 4.0f / 9.0f;
    const float factor = g_superSamplingTextureActive ? 1.0f : baseScaleFactor;
    const float interpolation = (g_superSampling + 1.0f) * (1.0f - g_viewportInterpolationProgress) + g_viewportInterpolationProgress;
    g_viewportScaleX = interpolation * ((std::min)(requestedWidth, static_cast<float>(textureWidth)) / static_cast<float>(textureWidth)) / factor;
    g_viewportScaleY = ((std::min)(requestedHeight, static_cast<float>(textureHeight)) / static_cast<float>(textureHeight)) / factor;
    if (width < 0.0f)
        g_viewportScaleX = 1.0f;
    if (height < 0.0f)
        g_viewportScaleY = 1.0f;
    return g_superSamplingTextureActive;
}

namespace d3t {

std::int64_t GetScaledViewportDimensions(float *width, float *height) {
    if (!width || !height)
        return 0;
    std::uint32_t textureWidth = 0;
    std::uint32_t textureHeight = 0;
    if (!GetTexture2DDimensions(g_fullScreenTexture2D, &textureWidth, &textureHeight))
        return 0;
    constexpr float baseScaleFactor = 4.0f / 9.0f;
    const float resolutionScale = g_superSamplingTextureActive ? 1.0f : baseScaleFactor;
    const float interpolation = (1.0f / (g_superSampling + 1.0f)) * (1.0f - g_viewportInterpolationProgress) + g_viewportInterpolationProgress;
    *width = static_cast<float>(textureWidth) * resolutionScale * interpolation * g_viewportScaleX;
    *height = static_cast<float>(textureHeight) * resolutionScale * g_viewportScaleY;
    return textureHeight;
}
} // namespace d3t


void GetAspectRatio_0(D3TDeviceContext *context) {
    if (!g_getAspectRatioModeFunction || !g_getAspectRatioModeFunction())
        return;
    const std::uint32_t texture2D = static_cast<std::uint32_t>(g_perspectiveProjection - 1) > 2U ? g_fullScreenTexture2D : CreateNewTexture2D();
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    GetTexture2DDimensions(texture2D, &width, &height);
    std::byte *const state = context->renderState.data();
    const std::array<float, 6> window{0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f};
    if (std::memcmp(state + 0x870, window.data(), sizeof(window)) != 0) {
        state[0x974] |= std::byte{1};
        std::memcpy(state + 0x870, window.data(), sizeof(window));
    }
}

void MapForSuperSampling(D3TDeviceContext *context, float scale) {
#if defined(_WIN32)
    auto *values = static_cast<float *>(MapTempConstantBufferForWriteDiscard(context, 3, 4, 12));
    if (!values)
        return;
    constexpr float baseScaleFactor = 4.0f / 9.0f;
    const float factor = g_superSamplingTextureActive ? 1.0f : baseScaleFactor;
    const float interpolation = (1.0f / (g_superSampling + 1.0f)) * (1.0f - g_viewportInterpolationProgress) + g_viewportInterpolationProgress;
    values[0] = scale;
    values[1] = g_viewportScaleX * factor * interpolation;
    values[2] = g_viewportScaleY * factor;
#else
    static_cast<void>(context);
    static_cast<void>(scale);
#endif
}

void SetupCharacterAlphaDepthTarget(D3TDeviceContext *context) {
    if (!context)
        return;
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data() + 0x80, &g_originalStripsTexture2D, sizeof(g_originalStripsTexture2D));
    context->renderState[0x976] |= std::byte{2};
    std::memcpy(context->renderState.data() + 0x7dc, targets.data(), targets.size());
}

void SetupCharacterShadowTarget(D3TDeviceContext *context) {
    if (!context)
        return;
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data(), &g_auxiliaryTexture2D, sizeof(g_auxiliaryTexture2D));
    std::memcpy(targets.data() + 0x80, &g_originalStripsTexture2D, sizeof(g_originalStripsTexture2D));
    context->renderState[0x976] |= std::byte{2};
    std::memcpy(context->renderState.data() + 0x7dc, targets.data(), targets.size());
}

void SetupPPFX(D3TDeviceContext *context) {
    if (!context)
        return;
    std::byte *const state = context->renderState.data();
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data(), &g_fullScreenTexture2D, sizeof(g_fullScreenTexture2D));
    std::memcpy(targets.data() + 0x40, &g_fullScreenUintArrayTexture2D, sizeof(g_fullScreenUintArrayTexture2D));
    std::memcpy(targets.data() + 0x48, &g_fullScreenFloatArrayTexture2D, sizeof(g_fullScreenFloatArrayTexture2D));
    std::memcpy(targets.data() + 0x50, &g_fullScreenUintTexture2D, sizeof(g_fullScreenUintTexture2D));
    std::memcpy(targets.data() + 0x80, &g_originalStripsTexture2D, sizeof(g_originalStripsTexture2D));
    state[0x976] |= std::byte{2};
    std::memcpy(state + 0x7dc, targets.data(), targets.size());

    float scaledWidth = 0.0f;
    float scaledHeight = 0.0f;
    if (!d3t::GetScaledViewportDimensions(&scaledWidth, &scaledHeight)) {
        return;
    }
    const std::array<float, 6> viewport{0.0f, 0.0f, scaledWidth, scaledHeight, 0.0f, 1.0f};
    if (std::memcmp(state + 0x870, viewport.data(), sizeof(viewport)) != 0) {
        state[0x974] |= std::byte{1};
        std::memcpy(state + 0x870, viewport.data(), sizeof(viewport));
    }
}

std::int64_t SetScreenDimensions(std::uint32_t width, std::uint32_t height) {
    TextureResourceDescriptor &quarterA = g_generalTextureResources[0];
    TextureResourceDescriptor &full = g_generalTextureResources[1];
    TextureResourceDescriptor &quarterB = g_generalTextureResources[2];

    quarterA.width = width >> 2;
    quarterA.height = height >> 2;
    quarterA.format = 18;
    quarterA.texture = 0;

    full.width = width;
    full.height = height;
    full.format = 18;
    full.texture = 0;

    quarterB.width = width >> 2;
    quarterB.height = height >> 2;
    quarterB.format = 18;
    quarterB.texture = 0;
    return RGBA8_CreateTexture2D();
}

} // namespace shenmue::s1
