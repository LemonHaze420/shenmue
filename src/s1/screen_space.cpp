#include "s1/screen_space.hpp"

#include "s1/boot.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/input.hpp"
#include "s1/file_loader.hpp"
#include "s1/texture_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/texture_cache.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/render_passes.hpp"
#include "common/math.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace shenmue::s1 {

extern std::int32_t (*g_getAspectRatioModeFunction)();

namespace {

std::int32_t g_surfaceAllocationSerial = 1;

void WriteDdsHeader(void *const destination, const std::uint32_t width, const std::uint32_t height, const std::uint32_t pixelBytes) {
    auto *const bytes = static_cast<std::byte *>(destination);
    std::memset(bytes, 0, 0x80);
    const auto write32 = [bytes](const std::size_t offset, const std::uint32_t value) { std::memcpy(bytes + offset, &value, sizeof(value)); };
    write32(0x00, 0x20534444);
    write32(0x04, 0x7c);
    write32(0x08, 0x1007);
    write32(0x0c, height);
    write32(0x10, width);
    write32(0x14, 4U * width);
    constexpr std::array<std::uint32_t, 8> pixelFormat{0x20, 0x41, 0, 0x20, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000};
    std::memcpy(bytes + 0x4c, pixelFormat.data(), sizeof(pixelFormat));
    write32(0x6c, 0x1000);
    static_cast<void>(pixelBytes);
}

} // namespace

std::int32_t CreateDXPVRTextureBuffer(const std::uint32_t width, const std::uint32_t height, const std::uint16_t format) {
    const std::int32_t surface = PVR_FindOrAllocateSurface(SHEN_NAME(SURF), g_surfaceAllocationSerial++);
    if (surface < 0)
        return -1;
    if (!CreatePVRTextureBufferForSurface(surface, width, height, format, nullptr)) {
        PVR_FreeAllocatedSurfaceByIndex(surface);
        return -1;
    }
    return surface;
}

ShenmueSurfaceBuffer *SurfaceAllocation(const std::uint32_t width, const std::uint32_t height) {
    const std::int32_t primary = CreateDXPVRTextureBuffer(width, height, 0x8089);
    if (primary < 0)
        return nullptr;

    const std::size_t pixelBytes = static_cast<std::size_t>(width) * height * 4U;
    const std::size_t totalBytes = 0x80U + pixelBytes;
    void *const dds = HLib_AllocHeapMemBlock(totalBytes, SHEN_NAME(SHAD));
    std::memset(dds, 0, totalBytes);
    WriteDdsHeader(dds, width, height, static_cast<std::uint32_t>(pixelBytes));

    auto *const surface = static_cast<ShenmueSurfaceBuffer *>(HLib_AllocHeapMemBlock(sizeof(ShenmueSurfaceBuffer), SHEN_NAME(SURF)));
    if (surface) {
        surface->width = static_cast<std::uint16_t>(width);
        surface->height = static_cast<std::uint16_t>(height);
        surface->formatEncoding = 0x502;
        surface->primarySurface = static_cast<std::int16_t>(primary);
        surface->secondarySurface = -1;
        surface->reserved0c = 0;
        surface->ddsBuffer = dds;
        surface->mapped = 0;
        std::memset(surface->reserved19, 0, sizeof(surface->reserved19));
    }
    return surface;
}

ShenmueSurfaceBuffer **SurfaceAllocation_Wrapper(ShenmueSurfaceOwner *const owner) {
    ShenmueSurfaceBuffer **const slot = owner->surfaceHandleSlot;
    if (!*slot)
        *slot = SurfaceAllocation(1024, 64);
    ShenmueSurfaceBuffer *const surface = *slot;
    if (surface && surface->secondarySurface == -1) {
        surface->secondarySurface = static_cast<std::int16_t>(CreateDXPVRTextureBuffer(surface->width, surface->height, 0x8089));
    }
    return slot;
}

void AdjustAspectRatioMatrix(OriginalStrip *strip, std::int32_t alignment, std::int32_t horizontalPosition, std::int32_t verticalPosition, std::int32_t width,
    std::int32_t height, std::int32_t screenMode) {
    const std::int32_t verticalAlignment = alignment / 4;
    if (verticalAlignment == 0) {
        strip->y0 = static_cast<float>(verticalPosition);
        strip->y1 = static_cast<float>(verticalPosition);
        strip->y2 = static_cast<float>(verticalPosition + height);
        strip->y3 = static_cast<float>(verticalPosition + height);
    } else if (verticalAlignment == 1) {
        const std::int32_t top = verticalPosition - height / 2;
        strip->y0 = static_cast<float>(top);
        strip->y1 = static_cast<float>(top);
        strip->y2 = static_cast<float>(top + height);
        strip->y3 = static_cast<float>(top + height);
    } else {
        const std::int32_t top = verticalPosition - height + 1;
        strip->y0 = static_cast<float>(top);
        strip->y1 = static_cast<float>(top);
        strip->y2 = static_cast<float>(verticalPosition + 1);
        strip->y3 = static_cast<float>(verticalPosition + 1);
    }

    const std::int32_t horizontalAlignment = alignment & 3;
    if (horizontalAlignment == 0) {
        strip->x0 = static_cast<float>(horizontalPosition);
        strip->x2 = static_cast<float>(horizontalPosition);
        strip->x1 = static_cast<float>(horizontalPosition + width);
        strip->x3 = static_cast<float>(horizontalPosition + width);
    } else if (horizontalAlignment == 1) {
        const std::int32_t left = horizontalPosition - width / 2;
        strip->x0 = static_cast<float>(left);
        strip->x2 = static_cast<float>(left);
        strip->x1 = static_cast<float>(left + width);
        strip->x3 = static_cast<float>(left + width);
    } else {
        const std::int32_t left = horizontalPosition - width + 1;
        strip->x0 = static_cast<float>(left);
        strip->x2 = static_cast<float>(left);
        strip->x1 = static_cast<float>(horizontalPosition + 1);
        strip->x3 = static_cast<float>(horizontalPosition + 1);
    }

    if (g_getAspectRatioModeFunction && g_getAspectRatioModeFunction()) {
        return;
    }

    const auto displayWidth = GetScreenResolution()->width;
    const auto displayHeight = GetScreenResolution()->height;
    const float displayAspect = static_cast<float>(displayWidth) / static_cast<float>(displayHeight);
    const float scale = 1.3333334f / displayAspect;
    float offset = 0.0f;
    if (screenMode == 1 || screenMode == 3 || screenMode == 4) {
        offset = (1.0f - scale) * 640.0f * 0.5f;
    } else if (screenMode == 2) {
        offset = (1.0f - scale) * 640.0f;
    }

    strip->x0 = scale * strip->x0 + offset;
    strip->x1 = scale * strip->x1 + offset;
    strip->x2 = scale * strip->x2 + offset;
    strip->x3 = scale * strip->x3 + offset;
    if (screenMode == 3) {
        strip->x0 -= offset;
        strip->x2 -= offset;
        strip->x1 += offset + 0.5f;
        strip->x3 += offset + 0.5f;
    }
}



void UILayout_BuildQuad(OriginalStrip *strip, std::int32_t alignmentType, std::int32_t halfWidth, std::int32_t halfHeight, std::int32_t scaledWidth,
    std::int32_t scaledHeight, float positionScaleX, float positionScaleY, std::uint32_t angle, std::int32_t screenMode) {
    std::int32_t top = 0;
    std::int32_t bottom = scaledHeight;
    if (alignmentType / 4 == 1) {
        bottom = scaledHeight / 2;
        top = scaledHeight / 2 - scaledHeight;
    } else if (alignmentType / 4 != 0) {
        bottom = 1;
        top = 1 - scaledHeight;
    }
    std::int32_t left = 0;
    std::int32_t right = scaledWidth;
    if ((alignmentType & 3) == 1) {
        right = scaledWidth / 2;
        left = scaledWidth / 2 - scaledWidth;
    } else if ((alignmentType & 3) != 0) {
        right = 1;
        left = 1 - scaledWidth;
    }

    const float x = static_cast<float>(halfWidth);
    const float y = static_cast<float>(halfHeight);
    if (angle) {
        const float cosine = cos_fixed16(static_cast<std::uint16_t>(angle));
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        const float cosineX = cosine * positionScaleX;
        const float cosineY = cosine * positionScaleY;
        const float sineX = sine * positionScaleX;
        const float negativeSineY = -sine * positionScaleY;
        strip->x0 = static_cast<float>(top) * negativeSineY + static_cast<float>(left) * cosineX + x;
        strip->y0 = static_cast<float>(top) * cosineY + static_cast<float>(left) * sineX + y;
        strip->x1 = static_cast<float>(top) * negativeSineY + static_cast<float>(right) * cosineX + x;
        strip->y1 = static_cast<float>(top) * cosineY + static_cast<float>(right) * sineX + y;
        strip->x2 = static_cast<float>(bottom) * negativeSineY + static_cast<float>(left) * cosineX + x;
        strip->y2 = static_cast<float>(bottom) * cosineY + static_cast<float>(left) * sineX + y;
        strip->x3 = static_cast<float>(bottom) * negativeSineY + static_cast<float>(right) * cosineX + x;
        strip->y3 = static_cast<float>(bottom) * cosineY + static_cast<float>(right) * sineX + y;
    } else {
        strip->x0 = static_cast<float>(left) * positionScaleX + x;
        strip->y0 = static_cast<float>(top) * positionScaleY + y;
        strip->x1 = static_cast<float>(right) * positionScaleX + x;
        strip->y1 = static_cast<float>(top) * positionScaleY + y;
        strip->x2 = static_cast<float>(left) * positionScaleX + x;
        strip->y2 = static_cast<float>(bottom) * positionScaleY + y;
        strip->x3 = static_cast<float>(right) * positionScaleX + x;
        strip->y3 = static_cast<float>(bottom) * positionScaleY + y;
    }

    if (g_getAspectRatioModeFunction && g_getAspectRatioModeFunction())
        return;
    const float scale = 1.3333334f / (static_cast<float>(GetScreenResolution()->width) / static_cast<float>(GetScreenResolution()->height));
    float offset = 0.0f;
    if (screenMode == 1 || screenMode == 3 || screenMode == 4)
        offset = (1.0f - scale) * 640.0f * 0.5f;
    else if (screenMode == 2)
        offset = (1.0f - scale) * 640.0f * 0.5f + (1.0f - scale) * 640.0f * 0.5f;
    strip->x0 = scale * strip->x0;
    strip->x1 = scale * strip->x1;
    strip->x2 = scale * strip->x2;
    strip->x3 = scale * strip->x3;
    strip->x0 = offset + strip->x0;
    strip->x1 = offset + strip->x1;
    strip->x2 = offset + strip->x2;
    strip->x3 = offset + strip->x3;
    if (screenMode == 3) {
        strip->x0 -= offset;
        strip->x2 -= offset;
        strip->x1 = offset + 0.5f + strip->x1;
        strip->x3 = offset + 0.5f + strip->x3;
    }
}


void AdjustScreenSpaceWithParams_0(const ScreenSpaceParams *params, std::int32_t screenMode) {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{.corner0 = 0xffffffffU, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
    UILayout_BuildQuad(&strip, params->alignmentType, static_cast<std::int32_t>(params->halfScreenX), static_cast<std::int32_t>(params->halfScreenY),
        params->scaledWidth, params->scaledHeight, params->positionScaleX, params->positionScaleY, params->angle, screenMode);
}

// @verified - p
void AdjustScreenSpaceWithParams(const ScreenSpaceParams *params, std::uint32_t flags, std::int32_t screenMode) {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip;
    strip.flags = 0;
    strip.corner0 = flags | 0x00ffffffU;
    strip.vertexCount = flags & 0x00ffffffU;
    strip.normalizedY1 = strip.normalizedX2 = strip.normalizedY3 = 0.0f;
    strip.normalizedY2 = strip.normalizedX3 = 1.0f;
    UILayout_BuildQuad(&strip, params->alignmentType, static_cast<std::int32_t>(params->halfScreenX), static_cast<std::int32_t>(params->halfScreenY),
        params->scaledWidth, params->scaledHeight, params->positionScaleX, params->positionScaleY, params->angle, screenMode);
}

void AdjustScreenSpaceWithRenderFlags(const ScreenSpaceFillParams *params, std::int32_t screenMode) {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{.corner0 = params->colour, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
    UILayout_BuildQuad(&strip, params->alignmentType, static_cast<std::int32_t>(params->halfScreenX), static_cast<std::int32_t>(params->halfScreenY),
        params->scaledWidth, params->scaledHeight, params->positionScaleX, params->positionScaleY, params->angle, screenMode);
    strip.flags |= 0x400U;
    ApplyFadeToPacket(&strip, params->maxExtent);
}

void DrawMiddleOfScreenStretchedAdjusted(std::uint32_t colour, float maxExtent) {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{.corner0 = colour, .normalizedY2 = 1.0f, .normalizedX3 = 1.0f};
    AdjustAspectRatioMatrix(&strip, 5, 320, 240, 640, 480, 3);
    ApplyFadeToPacket(&strip, maxExtent);
}

} // namespace shenmue::s1
