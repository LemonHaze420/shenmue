#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/pause.hpp"

namespace shenmue {
namespace s2 {

void HDGM_Callback(void *);

void ReleaseNamedTexture_Wrapper(std::int32_t index);

std::int32_t GetSaveScreenLanguage();
void DrawMainIconOverlay(std::uint32_t colour, float depth);

static const char *const hdgmSpriteFiles[18] = {
    "TNPUSH_A.SPR",
    "TNPUSH_A.SPR",
    "TNGMOV00.SPR",
    "TNGMOV01.SPR",
    "TNTMOV00.SPR",
    "TNTMOV01.SPR",
    "TNRANKIN.SPR",
    "TNRANKIN.SPR",
    "TNRETRYE.SPR",
    "TNRETRY1.SPR",
    "TNRETRYE.SPR",
    "TNRETRY1.SPR",
    "TNCONGRA.SPR",
    "TNCONGRA.SPR",
    "TNBYEBYE.SPR",
    "TNBYEBYE.SPR",
    "TNCNGBYE.SPR",
    "TNCNGBYE.SPR",
};

std::int32_t g_hdgmTextures[4];
std::int32_t g_hdgmActive;
std::int32_t g_hdgmFrame;
std::int32_t g_hdgmAlpha;
std::int32_t g_hdgmFadeStep;
std::uint32_t g_hdgmLanguage;
std::int32_t g_hdgmTextureCount;
std::int16_t g_hdgmFadeIn;
std::int16_t g_hdgmX;
std::int16_t g_hdgmY;
std::int16_t g_hdgmWidths[4];
std::int16_t g_hdgmHeights[4];
std::int16_t g_hdgmSprite;

void EnqueueHDGM() {
    g_hdgmSprite = -1;
    g_hdgmActive = 0;
    g_hdgmFadeIn = 1;
    g_hdgmLanguage = GetSaveScreenLanguage();
    g_hdgmAlpha = 0;
    g_hdgmFadeStep = 0;
    g_hdgmX = 320;
    g_hdgmY = 240;
    g_hdgmTextureCount = 0;
    for (std::int32_t index = 0; index < 4; ++index) {
        g_hdgmTextures[index] = -1;
        g_hdgmWidths[index] = 0x100;
        g_hdgmHeights[index] = 0x100;
    }
    HLib::EnqueueTaskWithoutParameter(HDGM_Callback, 14, 0, SHEN_NAME(HDGM));
}

void ReleaseHDGMTextures() {
    for (std::int32_t index = 0; index < 4; ++index) {
        if (g_hdgmTextures[index] != -1)
            ReleaseNamedTexture_Wrapper(g_hdgmTextures[index]);
        g_hdgmTextures[index] = -1;
    }
}

void LoadHDGMSprite(std::int16_t fadeIn, std::int16_t frames, std::int16_t index) {
    if (frames <= 0)
        frames = 1;
    g_hdgmFadeStep = 0x100 / frames;
    g_hdgmFadeIn = fadeIn;
    if (g_hdgmFadeIn)
        g_hdgmAlpha = 0;
    else
        g_hdgmAlpha = 0xff;
    g_hdgmFrame = 0;
    if ((g_hdgmSprite != index || g_hdgmLanguage != GetSaveScreenLanguage()) && index <= 8) {
        ReleaseHDGMTextures();
        g_hdgmLanguage = GetSaveScreenLanguage();
        const char *const filename = hdgmSpriteFiles[index * 2 + g_hdgmLanguage];
        const char *const directory = "sprite";
        const std::uint32_t size = shenOpenFile(directory, filename);
        std::byte *const data = static_cast<std::byte *>(HLib::AllocateTaskData(size, SHEN_NAME(HDG0)));
        if (data) {
            shenLoadFile(directory, filename, data, size);
            g_hdgmTextureCount = ReadTitleSpriteTextures(data, size, g_hdgmTextures);
            const std::byte *chunk = data;
            for (std::int32_t texture = 0; texture < g_hdgmTextureCount; ++texture) {
                g_hdgmWidths[texture] = *reinterpret_cast<const std::int16_t *>(chunk + 0x28);
                g_hdgmHeights[texture] = static_cast<std::int16_t>(*reinterpret_cast<const std::int32_t *>(chunk + 0x28) >> 16);
                chunk += *reinterpret_cast<const std::int32_t *>(chunk + 4);
            }
            HLib::FreeTaskData(data);
            g_hdgmSprite = index;
        } else
            g_hdgmSprite = -1;
    }
    if (g_hdgmSprite != -1)
        g_hdgmActive = 1;
    else
        g_hdgmActive = 0;
    if (!g_hdgmSprite)
        SetLoadingBusy();
}

void LoadHDGMSpriteAt(std::int16_t fadeIn, std::int16_t frames, std::int16_t index, std::int16_t x, std::int16_t y) {
    g_hdgmX = x;
    g_hdgmY = y;
    LoadHDGMSprite(fadeIn, frames, index);
}

void LoadHDGMPositionedSprite(std::int16_t fadeIn, std::int16_t frames, std::int16_t x, std::int16_t y) {
    LoadHDGMSpriteAt(fadeIn, frames, 3, x, y);
}

std::int32_t IsHDGMActive() {
    return g_hdgmActive;
}

static void DrawHDGMSprite() {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400;
    packet.colour = static_cast<std::uint32_t>(g_hdgmAlpha) << 24 | 0xffffffU;
    switch (g_hdgmSprite) {
    case 0:
        if (!(g_hdgmFrame & 0x10)) {
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 256, 512, 96);
            packet.uv[0] = 0.0f;
            packet.uv[1] = 0.4f;
            packet.uv[2] = 1.0f;
            packet.uv[3] = 0.6f;
            DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[0]), 20.0f);
        }
        DrawMainIconOverlay(0xbf000000U, 7.0f);
        break;
    case 1:
    case 2:
    case 6:
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 208, g_hdgmWidths[0], g_hdgmHeights[0]);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[0]), 15.0f);
        break;
    case 3:
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, g_hdgmX, g_hdgmY, g_hdgmWidths[0], g_hdgmHeights[0]);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[0]), 1.0f);
        break;
    case 4:
    case 5: {
        const std::int32_t texture = g_hdgmSprite - 4;
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 208, g_hdgmWidths[texture], g_hdgmHeights[texture]);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[texture]), 15.0f);
        break;
    }
    case 7:
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 240, g_hdgmWidths[0], g_hdgmHeights[0]);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[0]), 15.0f);
        break;
    case 8:
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 128, g_hdgmWidths[0], g_hdgmHeights[0]);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[0]), 15.0f);
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 288, g_hdgmWidths[1], g_hdgmHeights[1]);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_hdgmTextures[1]), 15.0f);
        break;
    }
}

void HDGM_Callback(void *) {
    if (g_hdgmActive) {
        ++g_hdgmFrame;
        if (g_hdgmFadeIn) {
            g_hdgmAlpha += g_hdgmFadeStep;
            if (g_hdgmAlpha > 0xff)
                g_hdgmAlpha = 0xff;
        } else {
            g_hdgmAlpha -= g_hdgmFadeStep;
            if (g_hdgmAlpha < 0) {
                g_hdgmAlpha = 0;
                g_hdgmActive = 0;
                if (!g_hdgmSprite)
                    FinishLoading();
            }
        }
    }
    if (g_hdgmActive)
        DrawHDGMSprite();
}

} // namespace s2
} // namespace shenmue

#endif
