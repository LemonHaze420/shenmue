#pragma once

#include "shenmue.h"
#include "s2/hlib.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name WINDOW_TASK_TOKEN{SHEN_NAME(WIND)};
inline constexpr shen_name WINDOW_TASK_MARKER{SHEN_NAME(WNDT)};
#if SHENMUE_IS_PC
struct GeneratedTextureSurface;
struct WindowSlot;
enum class WindowCallbackEvent : std::int32_t {
    Tick = 0,
    LineAdvanced = 3,
    WindowCleared = 4,
    CallbackAttached = 5,
    GlyphRevealComplete = 6,
    PageRotationComplete = 7
};
using WindowCallback = void (*)(WindowSlot *window, WindowCallbackEvent event, void *state);

struct WindowSpecification {
    std::uint8_t flags;
    std::uint8_t reserved01;
    std::int16_t columns;
    std::int16_t rows;
    std::int16_t field06;
    std::int16_t field08;
    std::int16_t x;
    std::int16_t y;
    std::int16_t horizontalGap;
    std::int16_t verticalGap;
    std::int16_t width;
    std::int16_t height;
    std::int16_t lineWidth;
    std::int16_t lineHeight;
};
static_assert(sizeof(WindowSpecification) == 0x1a);
static_assert(offsetof(WindowSpecification, columns) == 0x02);
static_assert(offsetof(WindowSpecification, x) == 0x0a);
static_assert(offsetof(WindowSpecification, lineHeight) == 0x18);

struct WindowSlot {
    std::uint32_t flags;
    std::int32_t firstLine;
    GeneratedTextureSurface **surface;
    std::uint8_t *lineFlags;
    std::uint8_t *shadowPixels;
    std::int16_t bytePitch;
    std::int16_t textureHeight;
    float x;
    float y;
    std::uint32_t reserved2c;
    std::int16_t horizontalGap;
    std::int16_t verticalGap;
    std::int16_t lineWidth;
    std::int16_t lineHeight;
    float u;
    float v;
    std::int16_t field40;
    std::int16_t field42;
    std::int16_t width;
    std::int16_t height;
    std::int16_t columns;
    std::int16_t rows;
    std::uint32_t reserved4c;
    std::uint8_t *glyphFlags;
    std::uint8_t *glyphBytes;
    std::int16_t column;
    std::int16_t row;
    std::array<std::uint8_t, 4> lineWidths;
    void *allocation;
    void *callbackState;
    WindowCallback callback;
    std::int32_t colourMode;
};
static_assert(sizeof(WindowSlot) == 0x88);
static_assert(alignof(WindowSlot) == 8);
static_assert(offsetof(WindowSlot, surface) == 0x08);
static_assert(offsetof(WindowSlot, lineFlags) == 0x10);
static_assert(offsetof(WindowSlot, shadowPixels) == 0x18);
static_assert(offsetof(WindowSlot, columns) == 0x48);
static_assert(offsetof(WindowSlot, glyphFlags) == 0x50);
static_assert(offsetof(WindowSlot, column) == 0x60);
static_assert(offsetof(WindowSlot, lineWidths) == 0x64);
static_assert(offsetof(WindowSlot, allocation) == 0x68);
static_assert(offsetof(WindowSlot, callbackState) == 0x70);
static_assert(offsetof(WindowSlot, callback) == 0x78);
static_assert(offsetof(WindowSlot, colourMode) == 0x80);
inline constexpr shen_name WINDOW_SLOT_ALLOCATION{SHEN_NAME(WNDB)};
inline constexpr std::uint16_t WINDOW_SURFACE_FORMAT = 0x8089;
inline constexpr std::array<std::uint8_t, 91> windowGlyphShadowKernel{
    0,1,2,2,2,2,2,2,2,2,2,1,0, 1,2,2,4,4,4,4,4,4,4,2,2,1, 1,2,4,8,8,16,16,16,8,8,4,2,1,
    1,2,4,8,16,32,128,64,16,8,4,2,1, 1,2,4,8,8,16,16,64,64,8,4,2,1, 1,2,2,4,4,4,4,4,4,4,2,2,1,
    0,1,2,2,2,2,2,2,2,4,2,1,0};
inline constexpr std::array<std::uint16_t, 8> windowShadowAlphaBands{0, 0x8000, 0xc000, 0xc000, 0xf000, 0xf000, 0xf000, 0xf000};
inline constexpr std::array<std::uint8_t, 86> windowHalfwidthKana{
    0xa7,0xb1,0xa8,0xb2,0xa9,0xb3,0xaa,0xb4,0xab,0xb5,0xb6,0x81,0xb7,0x82,0xb8,0x83,
    0xb9,0x84,0xba,0x85,0xbb,0x86,0xbc,0x87,0xbd,0x88,0xbe,0x89,0xbf,0x8a,0xc0,0x8b,
    0xc1,0x8c,0xaf,0xc2,0x8d,0xc3,0x8e,0xc4,0x8f,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0x90,
    0x91,0xcb,0x92,0x93,0xcc,0x94,0x95,0xcd,0x96,0x97,0xce,0x98,0x99,0xcf,0xd0,0xd1,
    0xd2,0xd3,0xac,0xd4,0xad,0xd5,0xae,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0x6e,0xdc,0x70,
    0x71, 0xa6, 0xdd, 0x9a, 0x9b, 0x9c};
inline constexpr std::array<std::uint8_t, 86> windowHalfwidthSymbols{
    0x21,0x20,0x2a,0x21,0x48,0x22,0x49,0x22,0x74,0x23,0x70,0x24,0x73,0x25,0x75,0x26,
    0x47,0x27,0x4a,0x28,0x4b,0x29,0x76,0x2a,0x5c,0x2b,0x24,0x2c,0x5d,0x2d,0x3d,0x2d,
    0x25,0x2e,0x3f,0x2f,0x27,0x3a,0x28,0x3b,0x63,0x3c,0x61,0x3d,0x64,0x3e,0x29,0x3f,
    0x77,0x40,0x4e,0x5b,0x6f,0x5c,0x4f,0x5d,0x30,0x5e,0x32,0x5f,0x46,0x60,0x50,0x7b,
    0x43,0x7c,0x51,0x7d,0x31,0x7e,0x23,0xa1,0x56,0xa2,0x57,0xa3,0x22,0xa4,0x26,0xa5,
    0x3c, 0xb0, 0x2b, 0xde, 0x2c, 0xdf};

extern std::int16_t g_windowParticleDebugFlags;
WindowSlot *AllocateWindowSlot(HLib::Task *task, const WindowSpecification &specification);
void ClearWindowSlot(WindowSlot *window);
void ClearWindowText(WindowSlot *window);
void AttachWindowCallback(WindowSlot *window, WindowCallback callback, void *state);
void AdvanceWindowLine(WindowSlot *window);
bool GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row);
std::int32_t WriteWindowGlyphByte(WindowSlot *window, std::uint8_t flags, std::uint8_t value);
std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode);
std::uint16_t ConvertWindowCharacterToHalfwidth(std::uint16_t character);
std::uint16_t ConvertWindowShiftJisCharacter(std::uint16_t character);
void RecoverWindowSurface(WindowSlot *window);
void HideWindowSlot(WindowSlot *window);
void ShowWindowSlot(WindowSlot *window);
void RevealWindowGlyphs(WindowSlot *window);
void SetWindowColourMode(WindowSlot *window, std::int32_t mode);
#endif
} // namespace shenmue::s2
