#include "s1/task/scrl.hpp"

#include "s1/boot.hpp"
#include "s1/initialization.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/came.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/wexe.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/archive_handles.hpp"
#include "s1/file_loader.hpp"
#include "s1/motion.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/film.hpp"
#include "s1/render_passes.hpp"
#include "s1/screen_space.hpp"
#include "s1/texture_loader.hpp"
#include "s1/texture_cache.hpp"
#include "common/utility.hpp"

#include <bit>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

std::array<SkyTextureRecord, 2> g_skyTextures{};
std::array<std::int32_t, 3> g_skySpriteLoaded{};
std::array<HLib::TaskData *, 3> g_skySpriteData{};
char g_skySpritePaths[3][24]{};
char g_skySpriteFiles[3][24]{};
std::array<OriginalStrip, 4> g_skyPackets{};
std::array<OriginalStrip, 4> g_skyUpperPackets{};
std::array<OriginalStrip, 4> g_skyIncomingPackets{};
std::array<OriginalStrip, 4> g_skyIncomingUpperPackets{};
std::array<OriginalStrip, 4> g_skyCloudPackets{};
float g_skyScrollX = 0.0f;
float g_skyScrollY = 0.0f;
std::int32_t g_bSkyEnabled = 0;
std::int32_t g_skyTextureLoaded = 0;
std::uint32_t g_shouldOverrideClearColour = 0;
std::uint32_t g_overrideClearColour = 0;
std::uint32_t g_SkyClearColour = 0;
std::int32_t g_bShowSkyTexture = 0;
std::int32_t g_skyMode = 0;
std::int32_t g_skyFadeFrames = 0;
std::int32_t g_skyFadeSlot = 0;
std::int32_t g_skyFadeFreesSlot = 0;
std::int32_t g_shouldAdjustSky = 0;
std::uint32_t g_skyCorner = 0;
std::int32_t g_spriteVerticalOffset = 0;
std::int32_t g_spriteHorizontalOffset = 0;
char g_scrollRequestPath[24]{};
char g_scrollRequestFile[24]{};
std::int32_t g_scrollRequestFadeFrames = 0;
std::int32_t g_scrollFlag = 0;
void *g_scrollCurrentInfo = nullptr;
std::int32_t g_bShouldFreeActiveScroll = 0;
std::int32_t g_scrollSpriteFlags0 = 7;
std::int32_t g_scrollSpriteFlags = 7;
std::uint32_t g_skyLuminanceEnabled = 0;
std::uint16_t g_skyLuminanceHigh = 0;
std::uint16_t g_skyLuminanceMiddle = 0;
std::uint16_t g_skyLuminanceLow = 0;
extern void *g_BGDT;

namespace {

#pragma pack(push, 1)
struct ScrlTaskData {
    std::byte reserved000[8];
    std::uint32_t state;
    std::byte reserved00c[4];
};
#pragma pack(pop)

static_assert(sizeof(ScrlTaskData) == 0x10);
static_assert(offsetof(ScrlTaskData, state) == 8);

struct SkyChunk {
    std::uint32_t token;
    std::uint32_t next;
};


std::int32_t g_sentSkyNumber = 0;

std::array<std::uint32_t, 2> g_mapLumaColours{};

constexpr char kSpriteFolder[] = "sprite";


const SkyChunk *FindSkyChunk(const void *first, std::uint32_t token) {
    const auto *chunk = static_cast<const SkyChunk *>(first);
    while (chunk->token != token) {
        if (!chunk->next)
            return nullptr;
        chunk = reinterpret_cast<const SkyChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->next);
    }
    return chunk;
}


std::uint16_t ApplySkyLuminanceBias(std::uint16_t pixel, std::uint8_t pixelFormat) {
    const auto clampChannel = [](std::int32_t value, std::int32_t maximum) {
        return static_cast<std::uint16_t>(value < 0 ? 0 : value > maximum ? maximum : value);
    };
    const std::int32_t high = static_cast<std::int16_t>(g_skyLuminanceHigh);
    const std::int32_t middle = static_cast<std::int16_t>(g_skyLuminanceMiddle);
    const std::int32_t low = static_cast<std::int16_t>(g_skyLuminanceLow);
    if (pixelFormat == 0) {
        const std::uint16_t alpha = pixel & 0x8000U;
        const std::uint16_t luminance = static_cast<std::uint16_t>(
            1057U * (static_cast<std::uint16_t>(((pixel >> 2U) & 0xf8U) + 3U * ((pixel >> 10U) & 0x1fU) + 5U * (pixel & 0x1fU)) >> 4U));
        return static_cast<std::uint16_t>(alpha | clampChannel((luminance & 0x1fU) + 2 * low, 31) |
            (clampChannel(((luminance >> 5U) & 0x1fU) + 2 * middle, 31) << 5U) | (clampChannel(((luminance >> 10U) & 0x1fU) + 2 * high, 31) << 10U));
    }
    if (pixelFormat == 1) {
        const std::uint16_t luminance = static_cast<std::uint16_t>(
            2113U * (static_cast<std::uint16_t>(((pixel >> 3U) & 0xf8U) + 3U * ((pixel >> 11U) & 0x1fU) + 5U * (pixel & 0x1fU)) >> 4U));
        return static_cast<std::uint16_t>(clampChannel((luminance & 0x1fU) + 2 * low, 31) | (clampChannel(((luminance >> 5U) & 0x3fU) + 4 * middle, 63) << 5U) |
            (clampChannel(((luminance >> 11U) & 0x1fU) + 2 * high, 31) << 11U));
    }
    const std::uint16_t alpha = pixel & 0xf000U;
    const std::uint16_t luminance =
        static_cast<std::uint16_t>(273U * (static_cast<std::uint16_t>(((pixel >> 1U) & 0x78U) + 3U * ((pixel >> 8U) & 0x0fU) + 5U * (pixel & 0x0fU)) >> 4U));
    return static_cast<std::uint16_t>(alpha | clampChannel((luminance & 0x0fU) + low, 15) | (clampChannel(((luminance >> 4U) & 0x0fU) + middle, 15) << 4U) |
        (clampChannel(((luminance >> 8U) & 0x0fU) + high, 15) << 8U));
}


std::uint32_t ExpandPvr16TexelToRgb888(std::uint16_t pixel, std::uint8_t pixelFormat) {
    if (pixelFormat == 0)
        return ((pixel & 0x7c00U) << 9U) | ((pixel & 0x03e0U) << 6U) | ((pixel & 0x001fU) << 3U);
    if (pixelFormat == 1)
        return ((pixel & 0xf800U) << 8U) | ((pixel & 0x07e0U) << 5U) | ((pixel & 0x001fU) << 3U);
    return ((pixel & 0x0f00U) << 12U) | ((pixel & 0x00f0U) << 8U) | ((pixel & 0x000fU) << 4U);
}



const std::uint32_t *LoadLuminanceFromMap(const void *texn) {
    const auto *cursor = static_cast<const std::byte *>(texn) + 16;
    std::uint32_t token = 0;
    std::memcpy(&token, cursor, sizeof(token));
    while (token != SHEN_NAME(PVRT) && token != SHEN_NAME(GBIX)) {
        cursor += 4;
        std::memcpy(&token, cursor, sizeof(token));
    }
    const std::byte *const header = token == SHEN_NAME(GBIX) ? cursor + 12 : cursor;
    const std::byte *const pixels = header + 16;
    const std::uint8_t pixelFormat = std::to_integer<std::uint8_t>(header[8]);
    std::int16_t width = 0;
    std::int16_t height = 0;
    std::memcpy(&width, header + 12, sizeof(width));
    std::memcpy(&height, header + 14, sizeof(height));
    const auto texel = [&](std::int64_t offset) {
        std::uint16_t pixel = 0;
        std::memcpy(&pixel, pixels + offset, sizeof(pixel));
        return ExpandPvr16TexelToRgb888(g_skyLuminanceEnabled ? ApplySkyLuminanceBias(pixel, pixelFormat) : pixel, pixelFormat);
    };
    const std::uint32_t first = texel(0);
    g_mapLumaColours[0] = first;
    if (width == 8 && height == 8) {
        g_mapLumaColours[1] = first;
    } else {
        g_mapLumaColours[1] = first;
        g_mapLumaColours[0] = texel((((static_cast<std::int64_t>(height) * width) >> 1) - 1) * 4);
    }
    return g_mapLumaColours.data();
}


void legacySetSpriteFlagsDependingOnSky(const char *filename) {
    if (std::strcmp(filename, "SCROLL07.SPR") == 0 || std::strcmp(filename, "SCROLL64.SPR") == 0) {
        g_scrollSpriteFlags0 = 5;
        g_scrollSpriteFlags = 5;
        return;
    }
    g_scrollSpriteFlags0 = 3;
    g_scrollSpriteFlags = std::strcmp(filename, "SCROLL63.SPR") != 0 ? 3 : 4;
}

void ClearSpriteSlotNames(std::int32_t slot) {
    std::memset(g_skySpritePaths[slot], 0, sizeof(g_skySpritePaths[slot]));
    std::memset(g_skySpriteFiles[slot], 0, sizeof(g_skySpriteFiles[slot]));
}

void ClearScrollRequest() {
    std::memset(g_scrollRequestPath, 0, sizeof(g_scrollRequestPath));
    std::memset(g_scrollRequestFile, 0, sizeof(g_scrollRequestFile));
    g_scrollRequestFadeFrames = 0;
    g_scrollFlag = 0;
    g_scrollCurrentInfo = nullptr;
}


std::int32_t CheckForFileAt(const char *path, const char *file) {
    if (std::strcmp(g_skyTextures[0].path, path) == 0 && std::strcmp(g_skyTextures[0].file, file) == 0)
        return 1;
    for (std::int32_t slot = 0; slot < 3; ++slot) {
        if (std::strcmp(g_skySpritePaths[slot], path) == 0 && std::strcmp(g_skySpriteFiles[slot], file) == 0)
            return slot + 2;
    }
    return 0;
}


void FreeSCRL() {
    if (g_skySpriteLoaded[2] == 1)
        HLib_FreeTaskData(g_skySpriteData[2], 0);
    g_skySpriteData[2] = static_cast<HLib::TaskData *>(g_BGDT);
    g_skySpriteLoaded[2] = 1;
    std::strcpy(g_skySpritePaths[2], kSpriteFolder);
    std::strcpy(g_skySpriteFiles[2], g_scrollRequestFile);
    g_bShouldFreeActiveScroll = 0;
    ControlSkyTextureFlags(2, g_scrollRequestFadeFrames, 0);
}


void UpdateClearColourForSky() {
    const SkyTextureRecord &shown = g_skyTextures[0];
    const SkyTextureRecord &incoming = g_skyTextures[1];
    if (incoming.alpha <= 0.0f) {
        g_SkyClearColour = shown.clearColour;
        return;
    }
    const auto channel = [&](unsigned shift) {
        const float incomingChannel = static_cast<float>((incoming.clearColour >> shift) & 0xffU) * (incoming.alpha / 255.0f);
        const float shownChannel = static_cast<float>((shown.clearColour >> shift) & 0xffU) * (shown.alpha / 255.0f);
        return static_cast<std::uint32_t>(static_cast<std::uint8_t>(static_cast<std::int32_t>(incomingChannel + shownChannel)));
    };
    g_SkyClearColour = channel(0) | (channel(8) << 8U) | (channel(16) << 16U);
}

void SetStripCorner(OriginalStrip &strip, std::size_t corner, float x, float y, std::uint32_t colour) {
    (&strip.x0)[corner * 2] = x;
    (&strip.y0)[corner * 2] = y;
    (&strip.corner0)[corner] = colour;
}



void Sky_BuildQuadsAndUVs() {

    constexpr float kVerticalProjection = 8523.0f;
    const float perspectiveScale = 10922.0f / static_cast<float>(currentPerspectiveFov);
    float scrollX = -(static_cast<float>(g_fCamAccumYaw + g_spriteHorizontalOffset) * 0.058597326f);
    g_skyScrollX = scrollX;
    if (scrollX > 960.0f) {
        do
            scrollX += -960.0f;
        while (scrollX > 960.0f);
        g_skyScrollX = scrollX;
    }
    if (scrollX < -960.0f) {
        do
            scrollX += 960.0f;
        while (scrollX < -960.0f);
        g_skyScrollX = scrollX;
    }
    const float deltaX = g_CameraTargetPos.x - g_CameraEyePos.x;
    const float deltaY = g_CameraTargetPos.y - g_CameraEyePos.y;
    const float deltaZ = g_CameraTargetPos.z - g_CameraEyePos.z;
    const float verticalScale = kVerticalProjection / static_cast<float>(computedCameraPitch);
    const float horizontalDistance = std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
    std::int16_t cameraPitch = 0;
    if (std::fabs(deltaY) >= 0.001f || std::fabs(horizontalDistance) >= 0.001f)
        cameraPitch = static_cast<std::int16_t>(Shenmue_Atan2Fixed(deltaY, horizontalDistance));
    const std::int32_t roll = ZANGLE;
    const float scrollY =
        ((1048591.0f / kVerticalProjection) * 0.00045775712f) * static_cast<float>(static_cast<std::int16_t>(cameraPitch + g_spriteVerticalOffset));
    g_skyScrollY = scrollY;
    const float sine = sin_fixed16(static_cast<std::int16_t>(roll));
    const float cosine = cos_fixed16(static_cast<std::uint16_t>(roll));
    const auto place = [&](float x, float y, float &outX, float &outY) {
        if (perspectiveScale != 1.0f)
            x = (x - 320.0f) * perspectiveScale + 320.0f;
        if (verticalScale != 1.0f)
            y = (y - 240.0f) * verticalScale + 240.0f;
        outX = x;
        outY = y;
    };
    const auto rotate = [&](float &x, float &y) {
        const float centredX = x - 320.0f;
        const float centredY = y - 240.0f;
        y = (centredY * cosine + centredX * sine) + 240.0f;
        x = (centredX * cosine - centredY * sine) + 320.0f;
    };
    std::array<std::uint32_t, 4> cloudColours{};
    std::memcpy(cloudColours.data(), g_cloudXY, sizeof(cloudColours));
    SkyTextureRecord &shown = g_skyTextures[0];
    SkyTextureRecord &incoming = g_skyTextures[1];
    const std::uint32_t alphaBits = static_cast<std::uint32_t>(static_cast<std::int32_t>(shown.alpha)) << 24U;
    std::uint32_t colour = alphaBits | 0xffffffU;
    std::uint32_t incomingColour = g_skyMode ? (static_cast<std::uint32_t>(static_cast<std::int32_t>(incoming.alpha)) << 24U) | 0xffffffU : 0U;
    for (std::size_t segment = 0; segment < 4; ++segment) {
        const float centreX = scrollX + kSkySegmentCentres[segment].x;
        const float centreY = scrollY + kSkySegmentCentres[segment].y;
        float cloudX = 0.0f;
        float cloudY = 0.0f;
        for (std::size_t corner = 0; corner < 4; ++corner) {
            float x = 0.0f;
            float y = 0.0f;
            place(centreX + kSkyQuadCorners[corner].x, centreY + kSkyQuadCorners[corner].y, x, y);
            if (corner >= 2) {
                cloudX = x;
                cloudY = std::fmax(620.0f, y);
            }
            if (roll) {
                rotate(x, y);
                if (corner >= 2)
                    rotate(cloudX, cloudY);
            }
            SetStripCorner(g_skyPackets[segment], corner, x, y, colour);
            if (g_skyMode)
                SetStripCorner(g_skyIncomingPackets[segment], corner, x, y, incomingColour);
            if (corner >= 2)
                SetStripCorner(g_skyCloudPackets[segment], corner, cloudX, cloudY, cloudColours[corner]);
        }
    }
    if (!shown.hasUpper)
        colour = shown.upperColour | alphaBits;
    if (g_skyMode) {
        const std::uint32_t incomingAlphaBits = static_cast<std::uint32_t>(static_cast<std::int32_t>(incoming.alpha)) << 24U;
        incomingColour = incoming.hasUpper ? incomingAlphaBits | 0xffffffU : incoming.upperColour | incomingAlphaBits;
    }
    constexpr std::array<Vector2, 4> kUpperCorners{{{-480.0f, -627.75f}, {480.0f, -627.75f}, {-480.0f, 627.75f}, {480.0f, 627.75f}}};
    for (std::size_t segment = 0; segment < 4; ++segment) {
        const float centreX = scrollX + kSkyUpperCentres[segment].x;
        const float centreY = scrollY + kSkyUpperCentres[segment].y;
        for (std::size_t corner = 0; corner < 4; ++corner) {
            float x = 0.0f;
            float y = 0.0f;
            place(centreX + kUpperCorners[corner].x, centreY + kUpperCorners[corner].y, x, y);
            if (roll)
                rotate(x, y);
            SetStripCorner(g_skyUpperPackets[segment], corner, x, y, colour);
            if (corner < 2)
                SetStripCorner(g_skyCloudPackets[segment], corner, x, y, cloudColours[corner]);
            if (g_skyMode)
                SetStripCorner(g_skyIncomingUpperPackets[segment], corner, x, y, incomingColour);
        }
    }
    for (OriginalStrip &strip : g_skyPackets) {
        if (shown.alpha == 255.0f)
            strip.flags &= ~0x400U;
        else
            strip.flags |= 0x400U;
    }
}




void Sky_DrawQuads() {
    if (!g_updateRenderFrameMode || static_cast<std::uint32_t>(g_perspectiveProjection - 1) <= 2U)
        return;
    std::array<std::uint32_t, 4> cloudColours{};
    std::memcpy(cloudColours.data(), g_cloudXY, sizeof(cloudColours));
    const bool drawClouds = (cloudColours[0] >> 24U) != 0 || (cloudColours[2] >> 24U) != 0;
    const SkyTextureRecord &shown = g_skyTextures[0];
    const SkyTextureRecord &incoming = g_skyTextures[1];
    const std::int32_t mode = g_skyMode;
    for (std::size_t segment = 0; segment < 4; ++segment) {
        const OriginalStrip &cloud = g_skyCloudPackets[segment];
        const float minimum = std::fmin(cloud.x3, std::fmin(cloud.x2, std::fmin(cloud.x1, cloud.x0)));
        const float maximum = std::fmax(cloud.x3, std::fmax(cloud.x2, std::fmax(cloud.x1, cloud.x0)));
        if ((minimum < 0.0f && maximum < 0.0f) || (minimum > 640.0f && maximum > 640.0f))
            continue;
        g_skyPackets[segment].flags &= ~0x400U;
        SetRenderFlags_UnmapAndDraw_0(&g_skyPackets[segment], shown.surface, 2000.0f);
        g_skyUpperPackets[segment].flags &= ~0x400U;
        if (shown.hasUpper)
            SetRenderFlags_UnmapAndDraw_0(&g_skyUpperPackets[segment], shown.upperSurface, 2000.0f);
        else
            ApplyFadeToPacket(&g_skyUpperPackets[segment], 2000.0f);
        if (mode) {
            g_skyIncomingPackets[segment].flags &= ~0x400U;
            g_skyIncomingUpperPackets[segment].flags &= ~0x400U;
            SetRenderFlags_UnmapAndDraw_0(&g_skyIncomingPackets[segment], incoming.surface, 1990.0f);
            if (incoming.hasUpper)
                SetRenderFlags_UnmapAndDraw_0(&g_skyIncomingUpperPackets[segment], incoming.upperSurface, 1990.0f);
            else
                ApplyFadeToPacket(&g_skyIncomingUpperPackets[segment], 1990.0f);
        }
        if (drawClouds) {
            g_skyCloudPackets[segment].flags &= ~0x400U;
            ApplyFadeToPacket(&g_skyCloudPackets[segment], 1980.0f);
        }
    }
}


void Sky_PresentScreenQuad() {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{};
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    strip.corner0 = g_skyCorner;
    AdjustAspectRatioMatrix(&strip, 5, 320, 240, 640, 480, 4);
    strip.flags |= 0x400U;
    ApplyFadeToPacket(&strip, 5.0f);
}

void ResetSkyPacket(OriginalStrip &strip, float x2, float x3) {
    strip.corner0 = 0xffffffffU;
    strip.vertexCount = 0;
    strip.normalizedY1 = 0.0f;
    strip.normalizedX2 = x2;
    strip.normalizedY2 = 1.0f;
    strip.normalizedX3 = x3;
    strip.normalizedY3 = 0.0f;
    strip.flags |= 0x408U;
}

} // namespace

bool SCRL_SetLuminanceEnabled(std::uint32_t enabled) {
    const bool previous = g_skyLuminanceEnabled;
    g_skyLuminanceEnabled = enabled;
    return previous;
}

bool SCRL_SetLuminanceChannels(std::uint16_t high, std::uint16_t middle, std::uint16_t low) {
    const bool previous = g_skyLuminanceEnabled;
    g_skyLuminanceHigh = high;
    g_skyLuminanceMiddle = middle;
    g_skyLuminanceLow = low;
    return previous;
}


void InitSky() {
    g_skyScrollX = 0.0f;
    g_skyScrollY = 0.0f;
    g_cloudXY[0] = 0;
    g_cloudXY[1] = 0;
    g_bSkyEnabled = 0;
    g_skyTextureLoaded = 0;
    g_shouldOverrideClearColour = 0;
    g_overrideClearColour = 0;
    g_SkyClearColour = 0;
    g_skyMode = 0;
    g_skyFadeFrames = 0;
    g_skyFadeSlot = 0;
    g_skyFadeFreesSlot = 0;
    g_spriteVerticalOffset = 0;
    g_spriteHorizontalOffset = 0;
    g_bShowSkyTexture = 1;
    for (std::int32_t slot = 0; slot < 3; ++slot) {
        g_skySpriteLoaded[slot] = 0;
        g_skySpriteData[slot] = nullptr;
        ClearSpriteSlotNames(slot);
    }
    for (std::size_t record = 0; record < 2; ++record) {
        SkyTextureRecord &texture = g_skyTextures[record];
        texture.upperColour = 0;
        texture.clearColour = 0;
        texture.hasUpper = 0;
        texture.fogTableReference = 0;
        texture.surface = 0xffff;
        texture.upperSurface = 0xffff;
        texture.alphaStep = 0.0f;
        if (record)
            texture.alpha = 0.0f;
        else
            texture.alpha = 255.0f;
        std::memset(texture.path, 0, sizeof(texture.path));
        std::memset(texture.file, 0, sizeof(texture.file));
    }
    std::memset(g_scrollRequestPath, 0, sizeof(g_scrollRequestPath));
    std::memset(g_scrollRequestFile, 0, sizeof(g_scrollRequestFile));
    g_scrollRequestFadeFrames = 0;
    g_scrollFlag = 0;
    g_scrollCurrentInfo = nullptr;
    g_bShouldFreeActiveScroll = 1;
}



void LoadSpriteTexture(void *data, std::int32_t record) {
    SkyTextureRecord &texture = g_skyTextures[static_cast<std::size_t>(record)];
    if (record == 0 && g_skyTextureLoaded) {
        PVR_FreeAllocatedSurfaceByIndex(g_skyTextures[0].surface);
        if (g_skyTextures[0].hasUpper)
            PVR_FreeAllocatedSurfaceByIndex(g_skyTextures[0].upperSurface);
    }
    if (!data)
        return;
    std::int32_t surface = -1;
    if (static_cast<const SkyChunk *>(data)->token == SHEN_NAME(SCRL)) {
        const SkyChunk *lower = static_cast<const SkyChunk *>(data);
        do
            lower = lower->next ? reinterpret_cast<const SkyChunk *>(reinterpret_cast<const std::byte *>(lower) + lower->next) : nullptr;
        while (lower && lower->token != SHEN_NAME(SCL0));
        auto *const lowerTexn = const_cast<SkyChunk *>(lower ? lower + 1 : nullptr);
        const std::uint32_t *const colours = LoadLuminanceFromMap(lowerTexn);
        texture.upperColour = colours[0];
        texture.clearColour = colours[1];
        surface = LoadTEXN(lowerTexn);
        texture.hasUpper = 1;
        const SkyChunk *const upper = FindSkyChunk(data, SHEN_NAME(SCL1));
        texture.upperSurface = LoadTEXN(const_cast<SkyChunk *>(upper ? upper + 1 : nullptr));
        texture.hasUpper = texture.upperSurface >= 0 ? 1 : 0;
    } else {
        texture.hasUpper = 0;
        const std::uint32_t *const colours = LoadLuminanceFromMap(data);
        texture.upperColour = colours[0];
        texture.clearColour = colours[1];
        surface = LoadTEXN(data);
        texture.upperSurface = 0xffff;
    }
    texture.surface = surface;
    if (record == 0)
        g_SkyClearColour = g_skyTextures[0].clearColour;
    g_skyTextureLoaded = 1;
    g_bShowSkyTexture = 1;
}


void Shenmue_ReadSprite_Wrapper(ScenarioContext *, const char *filename) {
    if (g_skyMode)
        RefreshAndFreeCurrentSkyScroll();
    char path[24]{};
    char file[24]{};
    std::strcpy(path, kSpriteFolder);
    std::strcpy(file, filename);
    const std::int32_t found = CheckForFileAt(path, file);
    HLib::TaskData *data = nullptr;
    if (found == 1)
        return;
    if (found) {
        data = g_skySpriteData[static_cast<std::size_t>(found - 2)];
    } else {
        const std::uint64_t size = shenOpenFile(path, file);
        if (size) {
            data = static_cast<HLib::TaskData *>(HLib_AllocMemBlock(size, SHEN_NAME(SCRL)));
            if (data && !shenLoadFile(path, file, data, static_cast<std::int32_t>(size))) {
                HLib_FreeTaskData(data, 0);
                data = nullptr;
            }
        }
    }
    std::strcpy(g_skyTextures[0].path, path);
    std::strcpy(g_skyTextures[0].file, file);
    legacySetSpriteFlagsDependingOnSky(file);
    LoadSpriteTexture(data, 0);
    if (!found && data)
        HLib_FreeTaskData(data, 0);
}


void ReadSprite_Wrapper(ScenarioContext *, const char *filename, std::int32_t slot) {
    if (slot >= 3)
        return;
    if (g_skySpriteLoaded[static_cast<std::size_t>(slot)] == 1)
        HLib_FreeTaskData(g_skySpriteData[static_cast<std::size_t>(slot)], 0);
    HLib::TaskData *data = nullptr;
    const std::uint64_t size = shenOpenFile(kSpriteFolder, filename);
    if (size) {
        data = static_cast<HLib::TaskData *>(HLib_AllocMemBlock(size, SHEN_NAME(SCRL)));
        if (data && !shenLoadFile(kSpriteFolder, filename, data, static_cast<std::int32_t>(size))) {
            HLib_FreeTaskData(data, 0);
            data = nullptr;
        }
    }
    g_skySpriteData[static_cast<std::size_t>(slot)] = data;
    g_skySpriteLoaded[static_cast<std::size_t>(slot)] = 1;
    std::strcpy(g_skySpritePaths[slot], kSpriteFolder);
    std::strcpy(g_skySpriteFiles[slot], filename);
}


void LoadSCRL1(std::int32_t shouldFreeSlot, std::int32_t slot) {
    if (slot >= 3 || g_skySpriteLoaded[static_cast<std::size_t>(slot)] != 1)
        return;
    if (g_skyMode)
        RefreshAndFreeCurrentSkyScroll();
    std::strcpy(g_skyTextures[0].path, g_skySpritePaths[slot]);
    std::strcpy(g_skyTextures[0].file, g_skySpriteFiles[slot]);
    legacySetSpriteFlagsDependingOnSky(g_skySpriteFiles[slot]);
    LoadSpriteTexture(g_skySpriteData[static_cast<std::size_t>(slot)], 0);
    if (shouldFreeSlot) {
        HLib_FreeTaskData(g_skySpriteData[static_cast<std::size_t>(slot)], 0);
        g_skySpriteData[static_cast<std::size_t>(slot)] = nullptr;
        g_skySpriteLoaded[static_cast<std::size_t>(slot)] = 0;
        ClearSpriteSlotNames(slot);
    }
}



void ReadSCRL(void *mapInfo) {
    const SkyChunk *const scrl = FindSkyChunk(mapInfo, SHEN_NAME(SCRL));
    std::int32_t skyNumber = -1;
    if (scrl) {
        std::memcpy(&skyNumber, scrl + 1, sizeof(skyNumber));
        if (skyNumber < -1)
            skyNumber = 0;
    }
    const auto offsetAngle = [&](std::uint32_t token) {
        const SkyChunk *const chunk = FindSkyChunk(mapInfo, token);
        if (!chunk)
            return 0;
        float degrees = 0.0f;
        std::memcpy(&degrees, chunk + 1, sizeof(degrees));
        return static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f)));
    };
    const std::int32_t verticalOffset = offsetAngle(SHEN_NAME(SCOF));
    const std::int32_t horizontalOffset = offsetAngle(SHEN_NAME(SHOF));
    bool entryHasSky = false;
    if (const SkyChunk *const sent = FindSkyChunk(mapInfo, SHEN_NAME(SENT))) {
        g_sentSkyNumber = -1;
        const auto *entry = reinterpret_cast<const std::int32_t *>(sent + 1);


        while (*entry != static_cast<std::int32_t>(g_entryPoint)) {
            if (*entry == -1)
                break;
            ++entry;
            if (*entry == -1) {
                ++entry;
                while (*entry != -1)
                    ++entry;
                ++entry;
            }
        }
        if (*entry == static_cast<std::int32_t>(g_entryPoint)) {
            g_sentSkyNumber = entry[2];
            entryHasSky = true;
        }
    }
    InitSky();
    g_spriteVerticalOffset = verticalOffset;
    g_spriteHorizontalOffset = horizontalOffset;
    if (entryHasSky)
        skyNumber = g_sentSkyNumber;
    if (skyNumber == -1) {
        g_bShowSkyTexture = 0;
    } else {
        char filename[24]{};
        std::snprintf(filename, sizeof(filename), "SCROLL%02d.SPR", skyNumber);
        Shenmue_ReadSprite_Wrapper(nullptr, filename);
    }
    g_bSkyEnabled = 1;
    for (std::size_t segment = 0; segment < 4; ++segment) {
        g_skyUpperPackets[segment].flags = 0;
        g_skyIncomingUpperPackets[segment].flags = 0;
        g_skyPackets[segment].flags = 0;
        g_skyIncomingPackets[segment].flags = 0;
        g_skyCloudPackets[segment].flags = 0;
        ResetSkyPacket(g_skyUpperPackets[segment], 9.0f, 0.0f);
        ResetSkyPacket(g_skyIncomingUpperPackets[segment], 9.0f, 0.0f);
        ResetSkyPacket(g_skyCloudPackets[segment], 0.0f, 1.0f);
        ResetSkyPacket(g_skyPackets[segment], 0.0f, 0.99000001f);
        ResetSkyPacket(g_skyIncomingPackets[segment], 0.0f, 0.99000001f);
    }
}



void LoadSPR_0(const void *sclm) {
    if (!sclm)
        return;
    const auto *entry = static_cast<const std::byte *>(sclm) + 8;
    for (std::int32_t index = 0; index < 3; ++index) {
        std::int32_t pair[2]{};
        std::memcpy(pair, entry, sizeof(pair));
        if (pair[0] == -1)
            break;
        char filename[24]{};
        std::snprintf(filename, sizeof(filename), "SCROLL%02d.SPR", pair[1]);
        if (pair[0] < 3)
            ReadSprite_Wrapper(nullptr, filename, pair[0]);
        entry += sizeof(pair);
    }
}


void SCRL_Callback_0(HLib::TaskData *) {
    if (g_scrollFlag == 1 && CheckFileInfo(g_scrollCurrentInfo) == 3 && g_bShouldFreeActiveScroll) {
        ReadCachedFile(g_scrollCurrentInfo);
        if (g_bShouldFreeActiveScroll)
            FreeSCRL();
    }
    if (g_skyMode == 1) {
        const bool finished = --g_skyFadeFrames < 0;
        g_skyTextures[0].alpha -= g_skyTextures[0].alphaStep;
        g_skyTextures[1].alpha -= g_skyTextures[1].alphaStep;
        if (finished)
            RefreshAndFreeCurrentSkyScroll();
    }
    UpdateClearColourForSky();
    if (g_bShouldDraw && g_bSkyEnabled && g_bShowSkyTexture) {
        Sky_BuildQuadsAndUVs();
        Sky_DrawQuads();
    }
    g_ClearColour = g_shouldOverrideClearColour ? g_overrideClearColour : g_SkyClearColour;
    if (g_shouldAdjustSky)
        Sky_PresentScreenQuad();
}



void ControlSkyTextureFlags(std::int32_t slot, std::int32_t fadeFrames, std::int32_t freesSlot) {
    if (slot >= 3 || g_skySpriteLoaded[static_cast<std::size_t>(slot)] != 1 || g_skyMode == 1)
        return;
    std::strcpy(g_skyTextures[0].path, g_skySpritePaths[slot]);
    std::strcpy(g_skyTextures[0].file, g_skySpriteFiles[slot]);
    legacySetSpriteFlagsDependingOnSky(g_skySpriteFiles[slot]);
    LoadSpriteTexture(g_skySpriteData[static_cast<std::size_t>(slot)], 1);
    g_skyTextures[0].alpha = 255.0f;
    g_skyFadeSlot = slot;
    g_skyFadeFreesSlot = freesSlot;
    g_skyFadeFrames = fadeFrames;
    g_skyMode = 1;
    g_skyTextures[0].alphaStep = 255.0f / static_cast<float>(fadeFrames);
    g_skyTextures[1].alphaStep = -(255.0f / static_cast<float>(fadeFrames));
    g_skyTextures[1].alpha = 0.0f;
}



void RefreshAndFreeCurrentSkyScroll() {
    if (!g_skyMode)
        return;
    SkyTextureRecord &shown = g_skyTextures[0];
    SkyTextureRecord &incoming = g_skyTextures[1];
    PVR_FreeAllocatedSurfaceByIndex(shown.surface);
    if (shown.hasUpper)
        PVR_FreeAllocatedSurfaceByIndex(shown.upperSurface);
    shown.upperColour = incoming.upperColour;
    shown.clearColour = incoming.clearColour;
    shown.hasUpper = incoming.hasUpper;
    shown.fogTableReference = incoming.fogTableReference;
    shown.surface = incoming.surface;
    shown.upperSurface = incoming.upperSurface;
    shown.alpha = 255.0f;
    incoming.upperColour = 0;
    incoming.clearColour = 0;
    incoming.hasUpper = 0;
    incoming.fogTableReference = 0;
    incoming.surface = 0xffff;
    incoming.upperSurface = 0xffff;
    incoming.alphaStep = 0.0f;
    incoming.alpha = 0.0f;
    std::memset(incoming.path, 0, sizeof(incoming.path));
    std::memset(incoming.file, 0, sizeof(incoming.file));
    const std::int32_t slot = g_skyFadeSlot;
    g_skyMode = 0;
    g_skyFadeFrames = 0;
    std::strcpy(shown.path, g_skySpritePaths[slot]);
    std::strcpy(shown.file, g_skySpriteFiles[slot]);
    legacySetSpriteFlagsDependingOnSky(g_skySpriteFiles[slot]);
    LoadSpriteTexture(g_skySpriteData[static_cast<std::size_t>(slot)], 0);
    if (g_skyFadeFreesSlot == 1) {
        HLib_FreeTaskData(g_skySpriteData[static_cast<std::size_t>(g_skyFadeSlot)], 0);
        g_skySpriteData[static_cast<std::size_t>(g_skyFadeSlot)] = nullptr;
        g_skySpriteLoaded[static_cast<std::size_t>(g_skyFadeSlot)] = 0;
        ClearSpriteSlotNames(g_skyFadeSlot);
    }
    if (g_scrollCurrentInfo) {
        g_skySpriteData[static_cast<std::size_t>(g_skyFadeSlot)] = nullptr;
        g_skySpriteLoaded[static_cast<std::size_t>(g_skyFadeSlot)] = 0;
        ClearSpriteSlotNames(g_skyFadeSlot);
        ClearScrollRequest();
        g_bShouldFreeActiveScroll = 1;
    }
}


bool SCRL_LoadSprite(const char *filename, std::int32_t fadeFrames) {
    if (g_scrollFlag == 1 || PACK_HasValidPack())
        return false;
    if (g_scrollCurrentInfo)
        ReadCachedFile(g_scrollCurrentInfo);
    if (HLib_GetLargestMallocFreeBlock() < 163840U)
        return false;
    char path[256];
    std::snprintf(path, sizeof(path), ".\\sprite\\%s", filename);
    LargeFileHandle *const info = LargeFile_OpenPathHandle(path);
    g_scrollCurrentInfo = info;
    if (!info)
        return false;
    std::strcpy(g_scrollRequestPath, kSpriteFolder);
    std::strcpy(g_scrollRequestFile, filename);
    g_scrollRequestFadeFrames = fadeFrames;
    g_scrollFlag = 1;
    GetFileFromCachedBuffer(info, LargeFile_GetSizeInBlocks(info), g_BGDT, 0);
    return true;
}


void SetShouldLoadWeatherSprites(std::int32_t shouldLoad) {
    g_shouldLoadWeatherSprites = shouldLoad;
    if (shouldLoad)
        return;
    if (g_skyMode)
        RefreshAndFreeCurrentSkyScroll();
    if (g_scrollFlag == 1 && g_scrollCurrentInfo && CheckFileInfo(g_scrollCurrentInfo) != 3) {
        ReadCachedFile(g_scrollCurrentInfo);
        g_bShouldFreeActiveScroll = 1;
        ClearScrollRequest();
    }
}


void LoadSCRL(void *data, char *chunkName) {
    if (g_skyMode)
        RefreshAndFreeCurrentSkyScroll();
    std::strcpy(g_skyTextures[0].path, kSpriteFolder);
    std::strcpy(g_skyTextures[0].file, chunkName);
    legacySetSpriteFlagsDependingOnSky(chunkName);
    LoadSpriteTexture(data, 0);
}



char *LoadSCR(void *data, char *chunkName, std::int32_t slot) {
    if (g_skySpriteLoaded[static_cast<std::size_t>(slot)] == 1)
        HLib_FreeTaskData(g_skySpriteData[static_cast<std::size_t>(slot)], 0);
    g_skySpriteData[static_cast<std::size_t>(slot)] = static_cast<HLib::TaskData *>(data);
    g_skySpriteLoaded[static_cast<std::size_t>(slot)] = 1;
    std::strcpy(g_skySpritePaths[slot], kSpriteFolder);
    std::strcpy(g_skySpriteFiles[slot], chunkName);
    return g_skySpriteFiles[slot] + std::strlen(chunkName) + 1;
}


void Script_SetScrollU(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (params[0]) {
        g_shouldAdjustSky = 1;
        g_skyCorner = params[1];
    } else {
        g_shouldAdjustSky = 0;
        g_skyCorner = 0;
    }
}


HLib::Task *SCRL_Init(void *mapInfo) {
    ReadSCRL(mapInfo);
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(SCRL_Callback_0, 12, 6, static_cast<std::int32_t>(SHEN_NAME(SCRL)));
    if (!task)
        return nullptr;
    if (task->task_data_ptr) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }

    auto *const data = static_cast<ScrlTaskData *>(HLib_AllocHeapMemBlock(sizeof(ScrlTaskData), SHEN_NAME(TASK)));
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    if (!data) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        return nullptr;
    }
    HLib_EnableTokenCheck(data);
    std::memset(data, 0, sizeof(*data));
    data->state = 0;
    return task;
}

} // namespace shenmue::s1
