#pragma once
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/ninja.hpp"
#define SHENMUE_DEBUG_SCREEN_CONSTANT static const

namespace shenmue { namespace s2 {
SHENMUE_DEBUG_SCREEN_CONSTANT std::size_t kDebugScreenFontPixels = 8 * 1024;
SHENMUE_DEBUG_SCREEN_CONSTANT std::size_t kDebugScreenUvCount = 257;
SHENMUE_DEBUG_SCREEN_CONSTANT std::size_t kDebugScreenTextBytes = 512;
SHENMUE_DEBUG_SCREEN_CONSTANT std::uintptr_t kDebugScreenStoreQueueControl0 = 0xff000038;
SHENMUE_DEBUG_SCREEN_CONSTANT std::uintptr_t kDebugScreenStoreQueueControl1 = 0xff00003c;
SHENMUE_DEBUG_SCREEN_CONSTANT std::uintptr_t kDebugScreenStoreQueueAlias = 0xe0000000;
SHENMUE_DEBUG_SCREEN_CONSTANT std::uint32_t kDebugScreenVertexAddressMask = 0x03ffffff;
struct DebugScreenPrintHeader {
    std::uint32_t command;
    std::uint32_t mode;
    std::uint32_t textureMode;
    std::uint32_t texture;
    std::uint32_t colour;
    std::int32_t opaqueList;
};
typedef char DebugScreenHeaderSizeCheck[sizeof(DebugScreenPrintHeader) == 0x18 ? 1 : -1];
typedef char DebugScreenSurfaceSizeCheck[sizeof(KMSURFACEDESC) == 0x20 ? 1 : -1];
typedef char DebugScreenSurfaceFlagsOffsetCheck[offsetof(KMSURFACEDESC, fSurfaceFlags) == 0x18 ? 1 : -1];
typedef char DebugScreenSurfacePointerOffsetCheck[offsetof(KMSURFACEDESC, pSurface) == 0x1c ? 1 : -1];
extern "C" const std::uint16_t shenmue_s2_debug_screen_font[kDebugScreenFontPixels];
extern const std::uint16_t *g_debugScreenFontPixels;
extern DebugScreenPrintHeader g_debugScreenPrintHeader;
extern KMSURFACEDESC g_debugScreenFontSurface;
extern std::uint16_t g_debugScreenFontGlyphSize;
extern std::int32_t g_debugScreenFontTextureIndex;
extern float g_debugScreenFontUvOrigin, g_debugScreenFontUvWidth, g_debugScreenFontUvStep;
extern std::int16_t g_debugScreenFontUvs[kDebugScreenUvCount];
extern std::int32_t g_debugScreenPrintX, g_debugScreenPrintY, g_debugScreenLineOrigin;
extern std::int32_t g_debugScreenPrintOrientation;
extern float g_debugScreenPrintDepth;
extern char g_debugScreenText[kDebugScreenTextBytes];
void InitializeDebugScreenPrint();
void SetDebugScreenFontPixels(const std::uint16_t *pixels);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetDebugScreenTextColour(std::uint32_t colour);
#endif
void SetDebugScreenPrintDepth(float depth);
void SetDebugScreenPrintOrientation(std::int32_t orientation);
void SetDebugScreenOpaqueList(std::int32_t enabled);
void SetDebugScreenTextureMode();
void ResetDebugScreenPrintHeader();
volatile std::uint32_t *WriteDebugScreenGlyph(std::int32_t x, std::int32_t y, std::int8_t character, volatile std::uint32_t *output);
void DrawDebugScreenString(const char *text);
void SetDebugScreenTextPosition(std::int32_t column, std::int32_t line);
void PrintDebugScreenText(const char *format, std::int32_t width, float value);
void PrintDebugScreenText(const char *format, const char *text, float value);
extern "C" std::int32_t shenmue_s2_event_DebugLogToScreen(const char *format, ...);
#if defined(_DEBUG) || (defined(SHENMUE_DC_BUILD_HASH) && SHENMUE_DC_BUILD_HASH)
void DrawDebugCommitHash();
#endif
} } // namespace shenmue::s2
#undef SHENMUE_DEBUG_SCREEN_CONSTANT

#endif
