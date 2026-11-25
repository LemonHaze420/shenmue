#include "s2/debug_screen_dc.hpp"
#include "common/types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/mt7.hpp"
#include "platform/dc/sdk.hpp"
#include <cstdarg>
#include <cstdio>
#if defined(_DEBUG) || (defined(SHENMUE_DC_BUILD_HASH) && SHENMUE_DC_BUILD_HASH)
#include "build_info.hpp"
#endif

namespace shenmue {
namespace s2 {
const std::uint16_t *g_debugScreenFontPixels = shenmue_s2_debug_screen_font;
DebugScreenPrintHeader g_debugScreenPrintHeader;
KMSURFACEDESC g_debugScreenFontSurface;
std::uint16_t g_debugScreenFontGlyphSize;
std::int32_t g_debugScreenFontTextureIndex;
float g_debugScreenFontUvOrigin, g_debugScreenFontUvWidth, g_debugScreenFontUvStep = 0.0f;
std::int16_t g_debugScreenFontUvs[kDebugScreenUvCount];
std::int32_t g_debugScreenPrintX = 0, g_debugScreenPrintY = 0, g_debugScreenLineOrigin = 0;
std::int32_t g_debugScreenPrintOrientation = 0;
float g_debugScreenPrintDepth = 999.9999389648438f;
char g_debugScreenText[kDebugScreenTextBytes] = {};

// @verified - d
void SetDebugScreenFontPixels(const std::uint16_t *pixels) {
    g_debugScreenFontPixels = pixels;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetDebugScreenTextColour(std::uint32_t colour) {
    g_debugScreenPrintHeader.colour = colour;
}
#endif

// @verified - d
void SetDebugScreenPrintDepth(float depth) {
    g_debugScreenPrintDepth = 1.0f / depth;
}

// @verified - d
void SetDebugScreenPrintOrientation(std::int32_t orientation) {
    g_debugScreenPrintOrientation = orientation;
}

// @verified - d
void SetDebugScreenTextureMode() {
    g_debugScreenPrintHeader.texture = ((g_debugScreenFontSurface.fSurfaceFlags & 1U) << 31) | ((g_debugScreenFontSurface.fSurfaceFlags & 8U) << 27) |
        (g_debugScreenFontSurface.PixelFormat & 0x38000000U) | ((~g_debugScreenFontSurface.fSurfaceFlags & 4U) << 24) |
        ((g_debugScreenFontSurface.fSurfaceFlags & 16U) << 21) | (reinterpret_cast<std::uint32_t>(g_debugScreenFontSurface.pSurface) >> 3);
}

// @verified - d
void ResetDebugScreenPrintHeader() {
    g_debugScreenPrintHeader.command = 0xa2000009U;
    g_debugScreenPrintHeader.mode = 0x80000000U;
    g_debugScreenPrintHeader.textureMode = 0x949004c7U;
    SetDebugScreenTextureMode();
    g_debugScreenPrintHeader.colour = 0xffffffffU;
    g_debugScreenPrintHeader.opaqueList = 0;
}

// @verified - d
void SetDebugScreenOpaqueList(std::int32_t enabled) {
    g_debugScreenPrintHeader.opaqueList = enabled;
    g_debugScreenPrintHeader.command &= 0xf8ffffffU;
    g_debugScreenPrintHeader.textureMode &= 0x03ffffffU;
    if (enabled)
        g_debugScreenPrintHeader.textureMode |= 0x20000000U;
    else {
        g_debugScreenPrintHeader.command |= 0x02000000U;
        g_debugScreenPrintHeader.textureMode |= 0x94000000U;
    }
}

// @verified - d
void InitializeDebugScreenPrint() {
    g_debugScreenPrintX = 0;
    g_debugScreenPrintY = 0;
    g_debugScreenLineOrigin = 0;
    g_debugScreenFontTextureIndex = -1;
    g_debugScreenFontGlyphSize = 8;
    dc::sdk::g_printCharacterWidth = 8;
    dc::sdk::g_printCharacterHeight = 8;
    dc::sdk::g_printFlags = 0;
    g_debugScreenFontUvOrigin = 0.0f;
    g_debugScreenFontUvWidth = 1.0f;
    g_debugScreenFontUvStep = 0.0078125f;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kDebugScreenUvCount); ++index) {
        const float v = static_cast<float>(index) * g_debugScreenFontUvStep;
        g_debugScreenFontUvs[index] = static_cast<std::int16_t>(floatAsU32(v) >> 16);
    }
    kmCreateTextureSurface(&g_debugScreenFontSurface, 8, 1024, static_cast<KMTEXTURETYPE>(0x902));
    kmLoadTexture(&g_debugScreenFontSurface, reinterpret_cast<Uint32 *>(const_cast<std::uint16_t *>(g_debugScreenFontPixels)));
    ResetDebugScreenPrintHeader();
}

// @verified - d
volatile std::uint32_t *WriteDebugScreenGlyph(std::int32_t x, std::int32_t y, std::int8_t character, volatile std::uint32_t *output) {
    if (g_debugScreenPrintOrientation) {
        const std::int32_t rotatedX = -y - dc::sdk::g_printCharacterHeight + dc::sdk::g_mt7DisplayHeight;
        const float right = static_cast<float>(rotatedX) + static_cast<float>(dc::sdk::g_printCharacterHeight);
        const float bottom = static_cast<float>(x) + static_cast<float>(dc::sdk::g_printCharacterWidth);
        const std::uint32_t firstUv = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_debugScreenFontUvs[character]));
        const std::uint32_t lastUv = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_debugScreenFontUvs[character + 1]));
        output[0] = 0xf0000000U;
        output[1] = floatAsU32(static_cast<float>(rotatedX));
        output[2] = floatAsU32(static_cast<float>(x));
        output[3] = floatAsU32(g_debugScreenPrintDepth);
        output[4] = floatAsU32(right);
        output[5] = floatAsU32(static_cast<float>(x));
        output[6] = floatAsU32(g_debugScreenPrintDepth);
        output[7] = floatAsU32(right);
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output += 8;
        output[0] = floatAsU32(bottom);
        // The rotated native path writes these lanes in the already submitted queue half.
        output[-7] = floatAsU32(g_debugScreenPrintDepth);
        output[-6] = floatAsU32(static_cast<float>(rotatedX));
        output[-5] = floatAsU32(bottom);
        const std::uint32_t value = static_cast<std::uint32_t>(static_cast<std::int32_t>(character));
        output[4] = (value << 24) | (value << 16) | (value << 8) | value;
        output[5] = lastUv;
        output[6] = firstUv;
        output[7] = firstUv | 0x3f800000U;
    } else {
        const float right = static_cast<float>(x) + static_cast<float>(dc::sdk::g_printCharacterWidth);
        const float bottom = static_cast<float>(y) + static_cast<float>(dc::sdk::g_printCharacterHeight);
        const std::uint32_t firstUv = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_debugScreenFontUvs[character]));
        const std::uint32_t lastUv = static_cast<std::uint32_t>(static_cast<std::int32_t>(g_debugScreenFontUvs[character + 1]));
        output[0] = 0xf0000000U;
        output[1] = floatAsU32(static_cast<float>(x));
        output[2] = floatAsU32(static_cast<float>(y));
        output[3] = floatAsU32(g_debugScreenPrintDepth);
        output[4] = floatAsU32(right);
        output[5] = floatAsU32(static_cast<float>(y));
        output[6] = floatAsU32(g_debugScreenPrintDepth);
        output[7] = floatAsU32(right);
        asm volatile("pref @%0" : : "r"(output) : "memory");
        output += 8;
        output[0] = floatAsU32(bottom);
        output[1] = floatAsU32(g_debugScreenPrintDepth);
        output[2] = floatAsU32(static_cast<float>(x));
        output[3] = floatAsU32(bottom);
        const std::uint32_t value = static_cast<std::uint32_t>(static_cast<std::int32_t>(character));
        output[4] = (value << 24) | (value << 16) | (value << 8) | value;
        output[5] = firstUv;
        output[6] = firstUv | 0x3f800000U;
        output[7] = lastUv | 0x3f800000U;
    }
    asm volatile("pref @%0" : : "r"(output) : "memory");
    return output + 8;
}

// @verified - d
void DrawDebugScreenString(const char *text) {
    const std::int32_t list = g_debugScreenPrintHeader.opaqueList ? 0 : 2;
    const std::uint32_t address = dc::sdk::g_sdkVertexBufferPointers[list];
    std::int32_t x = g_debugScreenPrintX, y = g_debugScreenPrintY;
    const bool retainOrigin = (dc::sdk::g_printFlags & 1U) != 0;
    *reinterpret_cast<volatile std::uint32_t *>(kDebugScreenStoreQueueControl0) = address >> 24;
    *reinterpret_cast<volatile std::uint32_t *>(kDebugScreenStoreQueueControl1) = address >> 24;
    volatile std::uint32_t *output = reinterpret_cast<volatile std::uint32_t *>((address & kDebugScreenVertexAddressMask) | kDebugScreenStoreQueueAlias);
    output[0] = g_debugScreenPrintHeader.command;
    output[1] = g_debugScreenPrintHeader.mode;
    output[2] = g_debugScreenPrintHeader.textureMode;
    output[3] = g_debugScreenPrintHeader.texture;
    output[4] = g_debugScreenPrintHeader.colour;
    asm volatile("pref @%0" : : "r"(output) : "memory");
    output += 8;
    for (; *text; ++text) {
        if (*text == ' ')
            x += dc::sdk::g_printCharacterWidth;
        else if (*text == '\n') {
            x = retainOrigin ? g_debugScreenLineOrigin : 0;
            y += dc::sdk::g_printCharacterHeight;
        } else if (*text == '\r')
            x = retainOrigin ? g_debugScreenLineOrigin : 0;
        else {
            output = WriteDebugScreenGlyph(x, y, static_cast<std::int8_t>(*text), output);
            x += dc::sdk::g_printCharacterWidth;
        }
    }
    g_debugScreenPrintX = x;
    g_debugScreenPrintY = y;
    dc::sdk::g_sdkVertexBufferPointers[list] =
        (dc::sdk::g_sdkVertexBufferPointers[list] & ~kDebugScreenVertexAddressMask) | (reinterpret_cast<std::uint32_t>(output) & kDebugScreenVertexAddressMask);
}

// @verified - d
void SetDebugScreenTextPosition(std::int32_t column, std::int32_t line) {
    if (g_debugScreenPrintOrientation) {
        g_debugScreenPrintX = static_cast<std::int32_t>(static_cast<float>(column * dc::sdk::g_printCharacterWidth) + dc::sdk::g_mt7ViewportY);
        g_debugScreenLineOrigin = g_debugScreenPrintX;
        const float origin = dc::sdk::g_mt7ViewportX + dc::sdk::g_mt7ViewportWidth;
        g_debugScreenPrintY = static_cast<std::int32_t>(static_cast<float>(line * dc::sdk::g_printCharacterHeight + dc::sdk::g_mt7DisplayHeight) - origin);
    } else {
        g_debugScreenPrintX = static_cast<std::int32_t>(static_cast<float>(column * dc::sdk::g_printCharacterWidth) + dc::sdk::g_mt7ViewportX);
        g_debugScreenLineOrigin = g_debugScreenPrintX;
        g_debugScreenPrintY = static_cast<std::int32_t>(static_cast<float>(line * dc::sdk::g_printCharacterHeight) + dc::sdk::g_mt7ViewportY);
    }
}

// @verified - d
void PrintDebugScreenText(const char *format, std::int32_t width, float value) {
    std::sprintf(g_debugScreenText, format, width, static_cast<double>(value));
    DrawDebugScreenString(g_debugScreenText);
}

// @verified - d
void PrintDebugScreenText(const char *format, const char *text, float value) {
    std::sprintf(g_debugScreenText, format, text, static_cast<double>(value));
    DrawDebugScreenString(g_debugScreenText);
}

// @verified - d
extern "C" std::int32_t shenmue_s2_event_DebugLogToScreen(const char *format, ...) {
    std::va_list arguments;
    va_start(arguments, format);
    std::vsprintf(g_debugScreenText, format, arguments);
    va_end(arguments);
    DrawDebugScreenString(g_debugScreenText);
    return 0;
}

#if defined(_DEBUG) || (defined(SHENMUE_DC_BUILD_HASH) && SHENMUE_DC_BUILD_HASH)
void DrawDebugCommitHash() {
    if (!g_debugScreenFontSurface.pSurface)
        return;
    const DebugScreenPrintHeader header = g_debugScreenPrintHeader;
    const std::int32_t x = g_debugScreenPrintX, y = g_debugScreenPrintY, origin = g_debugScreenLineOrigin;
    const std::int32_t orientation = g_debugScreenPrintOrientation;
    const std::int32_t width = dc::sdk::g_printCharacterWidth, height = dc::sdk::g_printCharacterHeight;
    const float depth = g_debugScreenPrintDepth;
    volatile std::uint32_t *const control0 = reinterpret_cast<volatile std::uint32_t *>(kDebugScreenStoreQueueControl0);
    volatile std::uint32_t *const control1 = reinterpret_cast<volatile std::uint32_t *>(kDebugScreenStoreQueueControl1);
    const std::uint32_t previousControl0 = *control0;
    const std::uint32_t previousControl1 = *control1;
    ResetDebugScreenPrintHeader();
    g_debugScreenPrintOrientation = 0;
    dc::sdk::g_printCharacterWidth = 8;
    dc::sdk::g_printCharacterHeight = 8;
    g_debugScreenPrintDepth = 999.9999389648438f;
    SetDebugScreenTextPosition(1, 1);
    DrawDebugScreenString(SHENMUE_BUILD_LABEL);
    g_debugScreenPrintHeader = header;
    g_debugScreenPrintX = x;
    g_debugScreenPrintY = y;
    g_debugScreenLineOrigin = origin;
    g_debugScreenPrintOrientation = orientation;
    dc::sdk::g_printCharacterWidth = width;
    dc::sdk::g_printCharacterHeight = height;
    g_debugScreenPrintDepth = depth;
    *control0 = previousControl0;
    *control1 = previousControl1;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
