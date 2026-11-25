#pragma once

#include "shenmue.h"
#include "s2/hlib.hpp"
#if SHENMUE_S2_GCC295
#define SHENMUE_WINDOW_ARRAY(type, name, count) type name[count]
#define SHENMUE_WINDOW_DATA static const
#define SHENMUE_WINDOW_INIT =
#define SHENMUE_WINDOW_CHECK(name, expression) typedef char name[(expression) ? 1 : -1]
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_WINDOW_ARRAY(type, name, count) std::array<type, count> name
#define SHENMUE_WINDOW_DATA inline constexpr
#define SHENMUE_WINDOW_INIT
#define SHENMUE_WINDOW_CHECK(name, expression) static_assert(expression)
#endif

namespace shenmue {
namespace s2 {
#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#define WINDOW_TASK_TOKEN shen_name(SHEN_NAME4('W', 'I', 'N', 'D'))
#define WINDOW_TASK_MARKER shen_name(SHEN_NAME4('W', 'N', 'D', 'T'))
#else
inline constexpr shen_name WINDOW_TASK_TOKEN{SHEN_NAME(WIND)};
inline constexpr shen_name WINDOW_TASK_MARKER{SHEN_NAME(WNDT)};
#endif
#if SHENMUE_IS_PC || (SHENMUE_IS_DC && SHENMUE_IS_S2)
struct WindowSpecification {
#if SHENMUE_IS_DC
    std::uint16_t flags;
#else
    std::uint8_t flags;
    std::uint8_t reserved01;
#endif
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
SHENMUE_WINDOW_CHECK(sizeofWindowSpecificationCheck, sizeof(WindowSpecification) == 0x1a);
SHENMUE_WINDOW_CHECK(offsetofWindowSpecificationcolumnsCheck, offsetof(WindowSpecification, columns) == 0x02);
SHENMUE_WINDOW_CHECK(offsetofWindowSpecificationxCheck, offsetof(WindowSpecification, x) == 0x0a);
SHENMUE_WINDOW_CHECK(offsetofWindowSpecificationlineHeightCheck, offsetof(WindowSpecification, lineHeight) == 0x18);

#endif

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
    SHENMUE_WINDOW_ARRAY(std::uint8_t, lineWidths, 4);
    void *allocation;
    void *callbackState;
    WindowCallback callback;
    std::int32_t colourMode;
};
SHENMUE_WINDOW_CHECK(sizeofWindowSlotCheck, sizeof(WindowSlot) == 0x88);
SHENMUE_WINDOW_CHECK(alignofWindowSlotCheck, alignof(WindowSlot) == 8);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotsurfaceCheck, offsetof(WindowSlot, surface) == 0x08);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotlineFlagsCheck, offsetof(WindowSlot, lineFlags) == 0x10);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotshadowPixelsCheck, offsetof(WindowSlot, shadowPixels) == 0x18);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcolumnsCheck, offsetof(WindowSlot, columns) == 0x48);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotglyphFlagsCheck, offsetof(WindowSlot, glyphFlags) == 0x50);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcolumnCheck, offsetof(WindowSlot, column) == 0x60);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotlineWidthsCheck, offsetof(WindowSlot, lineWidths) == 0x64);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotallocationCheck, offsetof(WindowSlot, allocation) == 0x68);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcallbackStateCheck, offsetof(WindowSlot, callbackState) == 0x70);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcallbackCheck, offsetof(WindowSlot, callback) == 0x78);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcolourModeCheck, offsetof(WindowSlot, colourMode) == 0x80);
#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#define WINDOW_SLOT_ALLOCATION shen_name(SHEN_NAME4('W', 'N', 'D', 'B'))
#else
inline constexpr shen_name WINDOW_SLOT_ALLOCATION{SHEN_NAME(WNDB)};
#endif
SHENMUE_WINDOW_DATA std::uint16_t WINDOW_SURFACE_FORMAT = 0x8089;
#endif

SHENMUE_WINDOW_DATA SHENMUE_WINDOW_ARRAY(std::uint8_t, windowGlyphShadowKernel, 91) SHENMUE_WINDOW_INIT {
    0,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    0,
    1,
    2,
    2,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    2,
    2,
    1,
    1,
    2,
    4,
    8,
    8,
    16,
    16,
    16,
    8,
    8,
    4,
    2,
    1,
    1,
    2,
    4,
    8,
    16,
    32,
    128,
    64,
    16,
    8,
    4,
    2,
    1,
    1,
    2,
    4,
    8,
    8,
    16,
    16,
    64,
    64,
    8,
    4,
    2,
    1,
    1,
    2,
    2,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    2,
    2,
    1,
    0,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    4,
    2,
    1,
    0
};
SHENMUE_WINDOW_DATA SHENMUE_WINDOW_ARRAY(std::uint16_t, windowShadowAlphaBands, 8) SHENMUE_WINDOW_INIT {
    0,
    0x8000,
    0xc000,
    0xc000,
    0xf000,
    0xf000,
    0xf000,
    0xf000
};
SHENMUE_WINDOW_DATA SHENMUE_WINDOW_ARRAY(std::uint8_t, windowHalfwidthKana, 86) SHENMUE_WINDOW_INIT {
    0xa7,
    0xb1,
    0xa8,
    0xb2,
    0xa9,
    0xb3,
    0xaa,
    0xb4,
    0xab,
    0xb5,
    0xb6,
    0x81,
    0xb7,
    0x82,
    0xb8,
    0x83,
    0xb9,
    0x84,
    0xba,
    0x85,
    0xbb,
    0x86,
    0xbc,
    0x87,
    0xbd,
    0x88,
    0xbe,
    0x89,
    0xbf,
    0x8a,
    0xc0,
    0x8b,
    0xc1,
    0x8c,
    0xaf,
    0xc2,
    0x8d,
    0xc3,
    0x8e,
    0xc4,
    0x8f,
    0xc5,
    0xc6,
    0xc7,
    0xc8,
    0xc9,
    0xca,
    0x90,
    0x91,
    0xcb,
    0x92,
    0x93,
    0xcc,
    0x94,
    0x95,
    0xcd,
    0x96,
    0x97,
    0xce,
    0x98,
    0x99,
    0xcf,
    0xd0,
    0xd1,
    0xd2,
    0xd3,
    0xac,
    0xd4,
    0xad,
    0xd5,
    0xae,
    0xd6,
    0xd7,
    0xd8,
    0xd9,
    0xda,
    0xdb,
    0x6e,
    0xdc,
    0x70,
    0x71,
    0xa6,
    0xdd,
    0x9a,
    0x9b,
    0x9c
};
SHENMUE_WINDOW_DATA SHENMUE_WINDOW_ARRAY(std::uint8_t, windowHalfwidthSymbols, 86) SHENMUE_WINDOW_INIT {
    0x21,
    0x20,
    0x2a,
    0x21,
    0x48,
    0x22,
    0x49,
    0x22,
    0x74,
    0x23,
    0x70,
    0x24,
    0x73,
    0x25,
    0x75,
    0x26,
    0x47,
    0x27,
    0x4a,
    0x28,
    0x4b,
    0x29,
    0x76,
    0x2a,
    0x5c,
    0x2b,
    0x24,
    0x2c,
    0x5d,
    0x2d,
    0x3d,
    0x2d,
    0x25,
    0x2e,
    0x3f,
    0x2f,
    0x27,
    0x3a,
    0x28,
    0x3b,
    0x63,
    0x3c,
    0x61,
    0x3d,
    0x64,
    0x3e,
    0x29,
    0x3f,
    0x77,
    0x40,
    0x4e,
    0x5b,
    0x6f,
    0x5c,
    0x4f,
    0x5d,
    0x30,
    0x5e,
    0x32,
    0x5f,
    0x46,
    0x60,
    0x50,
    0x7b,
    0x43,
    0x7c,
    0x51,
    0x7d,
    0x31,
    0x7e,
    0x23,
    0xa1,
    0x56,
    0xa2,
    0x57,
    0xa3,
    0x22,
    0xa4,
    0x26,
    0xa5,
    0x3c,
    0xb0,
    0x2b,
    0xde,
    0x2c,
    0xdf
};

#if SHENMUE_IS_PC
extern std::int16_t g_windowParticleDebugFlags;
WindowSlot *AllocateWindowSlot(HLib::Task *task, const WindowSpecification &specification);
void ClearWindowSlot(WindowSlot *window);
void ClearWindowText(WindowSlot *window);
void AttachWindowCallback(WindowSlot *window, WindowCallback callback, void *state);
void AdvanceWindowLine(WindowSlot *window);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row);
#else
bool GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row);
#endif
std::int32_t WriteWindowGlyphByte(WindowSlot *window, std::uint8_t flags, std::uint8_t value);
std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode);
std::uint16_t ConvertWindowCharacterToHalfwidth(std::uint16_t character);
std::uint16_t ConvertWindowShiftJisCharacter(std::uint16_t character);
void RecoverWindowSurface(WindowSlot *window);
void HideWindowSlot(WindowSlot *window);
void ShowWindowSlot(WindowSlot *window);
void RevealWindowGlyphs(WindowSlot *window);
void SetWindowColourMode(WindowSlot *window, std::int32_t mode);
#elif SHENMUE_IS_DC && SHENMUE_IS_S2
SHENMUE_WINDOW_DATA std::size_t kWindowShadowBufferBytes = 0x5a40;
#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#define WINDOW_SLOT_ALLOCATION shen_name(SHEN_NAME4('W', 'N', 'D', 'B'))
#else
inline constexpr shen_name WINDOW_SLOT_ALLOCATION{SHEN_NAME(WNDB)};
#endif
struct WindowSlot;
WindowSlot *AllocateWindowSlot(HLib::Task *task, const WindowSpecification &specification);
void AttachWindowCallback(WindowSlot *window, void (*callback)(WindowSlot *, std::int32_t, void *), void *state);
void RecoverWindowSurface(WindowSlot *window);
struct WindowSlot {
    std::uint32_t flags;
    std::int32_t firstLine;
    void **surface;
    std::uint8_t *lineFlags;
    std::uint8_t *shadowPixels;
    std::int16_t bytePitch, textureHeight;
    float x, y;
    std::uint32_t retained20;
    std::int16_t horizontalGap, verticalGap, lineWidth, lineHeight;
    float u, v;
    std::int16_t field34, field36, width, height;
    std::int16_t columns, rows;
    std::uint8_t *glyphFlags, *glyphBytes;
    std::int16_t column, row;
    SHENMUE_WINDOW_ARRAY(std::uint8_t, lineWidths, 4);
    void *allocation, *callbackState;
    void (*callback)(WindowSlot *, std::int32_t, void *);
    std::int32_t colourMode;
};
SHENMUE_WINDOW_CHECK(sizeofWindowSlotCheck, sizeof(WindowSlot) == 0x60);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotsurfaceCheck, offsetof(WindowSlot, surface) == 8);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotlineFlagsCheck, offsetof(WindowSlot, lineFlags) == 0x0c);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotshadowPixelsCheck, offsetof(WindowSlot, shadowPixels) == 0x10);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotbytePitchCheck, offsetof(WindowSlot, bytePitch) == 0x14);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotxCheck, offsetof(WindowSlot, x) == 0x18);
SHENMUE_WINDOW_CHECK(offsetofWindowSlothorizontalGapCheck, offsetof(WindowSlot, horizontalGap) == 0x24);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotuCheck, offsetof(WindowSlot, u) == 0x2c);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotwidthCheck, offsetof(WindowSlot, width) == 0x38);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcolumnsCheck, offsetof(WindowSlot, columns) == 0x3c);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotglyphFlagsCheck, offsetof(WindowSlot, glyphFlags) == 0x40);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcolumnCheck, offsetof(WindowSlot, column) == 0x48);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotlineWidthsCheck, offsetof(WindowSlot, lineWidths) == 0x4c);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotallocationCheck, offsetof(WindowSlot, allocation) == 0x50);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcallbackStateCheck, offsetof(WindowSlot, callbackState) == 0x54);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcallbackCheck, offsetof(WindowSlot, callback) == 0x58);
SHENMUE_WINDOW_CHECK(offsetofWindowSlotcolourModeCheck, offsetof(WindowSlot, colourMode) == 0x5c);
struct WindowTaskData {
    shen_name name;
    SHENMUE_WINDOW_ARRAY(WindowSlot, slots, 5);
    std::uint32_t customFlag;
};
SHENMUE_WINDOW_CHECK(sizeofWindowTaskDataCheck, sizeof(WindowTaskData) == 0x1e8);
SHENMUE_WINDOW_CHECK(offsetofWindowTaskDataslotsCheck, offsetof(WindowTaskData, slots) == 4);
SHENMUE_WINDOW_CHECK(offsetofWindowTaskDataslotssizeofWindowSlotCheck, offsetof(WindowTaskData, slots) + sizeof(WindowSlot) == 0x64);
SHENMUE_WINDOW_CHECK(offsetofWindowTaskDataslots4sizeofWindowSlotCheck, offsetof(WindowTaskData, slots) + 4 * sizeof(WindowSlot) == 0x184);
SHENMUE_WINDOW_CHECK(offsetofWindowTaskDatacustomFlagCheck, offsetof(WindowTaskData, customFlag) == 0x1e4);
struct FontRuntimeData;
struct GeneratedTextureSurface;
extern HLib::Task *windowTask;
HLib::Task *QueueWindowTask(std::uint8_t nextFunctionIndex = 2);
void WindowTaskEntry_Callback(void *parameter);
void UpdateWindowSlot(WindowSlot *window, WindowTaskData *owner);
void UpdateWindowRow(WindowSlot *window, WindowTaskData *owner, std::int32_t row, std::int32_t line);
void DrawWindowMessageSprites(WindowSlot *window);
void RasterizeWindowGlyphShadow(WindowSlot *window, FontRuntimeData *font, std::uint16_t glyph, std::int16_t column, std::int16_t row);
void UploadWindowShadowSurface(WindowSlot *window, GeneratedTextureSurface *surface, std::uint16_t foreground, std::uint16_t shadow);
const std::byte *GetWindowGlyphBitmap(FontRuntimeData *font, std::uint16_t glyph);
void RasterizeWindowGlyph(FontRuntimeData *font, std::uint16_t glyph, void *destination, std::int32_t bytePitch, std::uint16_t colour, std::uint16_t shadow,
    std::uint16_t background);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void wndSetFlag(HLib::Task *task, std::int32_t flag);
void RasterizeWindowGlyph4bpp(FontRuntimeData *font, std::uint16_t glyph, std::byte *destination, std::int32_t pixelX);
std::int32_t RasterizeWindowTextLine4bpp(std::byte *destination, std::int32_t width, const char *text, std::int32_t mode, std::int32_t centered);
#endif
std::int32_t IsWindowMultibyteCharacter(std::uint32_t character);
std::uint16_t StripWindowCharacterHighBits(std::uint16_t character);
std::uint16_t ConvertWindowCharacterToHalfwidth(std::uint16_t character);
void ClearWindowTextPixels(std::uint16_t *destination, std::int32_t width, std::int32_t height, std::uint16_t value);
std::int32_t RenderWindowTextSurface(
    HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode, std::int32_t centered);
std::int32_t SetupUIElement(HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode);
std::int32_t CreateCenteredWindowTextSurface(
    HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t CreateVerticalWindowTextSurface(HLib::Task *task, const char *text, std::int32_t height, std::uint16_t colour, std::uint16_t shadow);
#endif
void HideWindowSlot(WindowSlot *window);
void ShowWindowSlot(WindowSlot *window);
void ClearWindowShadowPixels(WindowSlot *window);
void ClearWindowSlot(WindowSlot *window);
void ClearWindowText(WindowSlot *window);
void SetWindowColourMode(WindowSlot *window, std::int32_t mode);
void RevealWindowGlyphs(WindowSlot *window);
void AdvanceWindowLine(WindowSlot *window);
std::int32_t GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row);
std::int32_t WriteWindowGlyphByte(WindowSlot *window, std::uint8_t flags, std::uint8_t value);
std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode);
#endif
} // namespace s2
} // namespace shenmue

#undef SHENMUE_WINDOW_ARRAY
#undef SHENMUE_WINDOW_DATA
#undef SHENMUE_WINDOW_INIT
#undef SHENMUE_WINDOW_CHECK
