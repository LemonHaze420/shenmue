#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct OriginalStrip {
    std::uint32_t flags;
    float x0;
    float y0;
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    std::uint32_t corner0;
    std::uint32_t corner1;
    std::uint32_t corner2;
    std::uint32_t corner3;
    std::uint32_t vertexCount;
    float normalizedX0;
    float normalizedY0;
    float normalizedX1;
    float normalizedY1;
    float normalizedX2;
    float normalizedY2;
    float normalizedX3;
    float normalizedY3;
};

struct ScreenSpaceParams {
    float halfScreenX;
    float halfScreenY;
    float positionScaleX;
    float positionScaleY;
    float depthOrScale;
    std::uint16_t flags14;
    std::uint16_t flags16;
    std::uint32_t angle;
    std::int16_t pvrSurfaceIndex;
    std::int16_t scaledWidth;
    std::int16_t scaledHeight;
    std::uint8_t alignmentType;
    std::uint8_t reserved23;
};

// AdjustScreenSpaceWithRenderFlags' quad: a filled colour strip.
struct ScreenSpaceFillParams {
    float halfScreenX;
    float halfScreenY;
    float positionScaleX;
    float positionScaleY;
    float maxExtent;
    std::uint32_t colour;
    std::int16_t scaledWidth;
    std::int16_t scaledHeight;
    std::uint32_t angle;
    std::uint8_t alignmentType;
};

struct ShenmueSurfaceBuffer {
    std::uint16_t width;
    std::uint16_t height;
    std::uint32_t formatEncoding;
    std::int16_t primarySurface;
    std::int16_t secondarySurface;
    std::uint32_t reserved0c;
    void *ddsBuffer;
    std::uint8_t mapped;
    std::uint8_t reserved19[7];
};

struct ShenmueSurfaceOwner {
    std::uint64_t reserved;
    ShenmueSurfaceBuffer **surfaceHandleSlot;
};
#pragma pack(pop)

static_assert(sizeof(OriginalStrip) == 0x58);
static_assert(sizeof(ScreenSpaceParams) == 0x24);
static_assert(sizeof(ScreenSpaceFillParams) == 0x21 && offsetof(ScreenSpaceFillParams, colour) == 0x14 && offsetof(ScreenSpaceFillParams, angle) == 0x1c &&
    offsetof(ScreenSpaceFillParams, alignmentType) == 0x20);
static_assert(sizeof(ShenmueSurfaceBuffer) == 0x20);
static_assert(sizeof(ShenmueSurfaceOwner) == 0x10);

void SetRenderFlags_UnmapAndDraw_0(OriginalStrip *strip, std::int32_t texture2D, float maxExtent);
void AdjustAspectRatioMatrix(OriginalStrip *strip, std::int32_t alignmentType, std::int32_t horizontalPosition, std::int32_t verticalPosition,
    std::int32_t width, std::int32_t height, std::int32_t screenMode);
void aspect_ratio_2(OriginalStrip *strip, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom, std::int32_t screenMode);
void UILayout_BuildQuad(OriginalStrip *strip, std::int32_t alignmentType, std::int32_t halfWidth, std::int32_t halfHeight, std::int32_t scaledWidth,
    std::int32_t scaledHeight, float positionScaleX, float positionScaleY, std::uint32_t angle, std::int32_t screenMode);
void AdjustScreenSpaceWithParams_0(const ScreenSpaceParams *params, std::int32_t screenMode);
void AdjustScreenSpaceWithRenderFlags(const ScreenSpaceFillParams *params, std::int32_t screenMode);
void AdjustDefaultScreenSpace(const ScreenSpaceParams *params);
void AdjustScreenSpaceWithParams(const ScreenSpaceParams *params, std::uint32_t flags, std::int32_t screenMode);
void DrawMiddleOfScreenStretchedAdjusted(std::uint32_t colour, float maxExtent);
std::int32_t CreateDXPVRTextureBuffer(std::uint32_t width, std::uint32_t height, std::uint16_t format);
ShenmueSurfaceBuffer *SurfaceAllocation(std::uint32_t width, std::uint32_t height);
ShenmueSurfaceBuffer **SurfaceAllocation_Wrapper(ShenmueSurfaceOwner *owner);

extern std::int32_t (*g_getAspectRatioModeFunction)();

} // namespace shenmue::s1
