#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;

// Sakura_InitWindKernelAndLUTs' heap blocks: the gust-strength kernel, the cumulative angle-bin table and the smoothed noise table.
inline constexpr shen_name WEATHER_WBLT = SHEN_NAME(WBLT);
inline constexpr shen_name WEATHER_S2AT = SHEN_NAME(S2AT);
inline constexpr shen_name WEATHER_NOISE = make_name('1', '/', 'F', 'W');

#pragma pack(push, 1)
struct WindEntry {
    std::uint32_t flags;
    std::int32_t rowScrollOrigin;
    std::uint8_t **surfaceHandleSlot;
    std::uint8_t *rowDirtyFlags;
    std::uint8_t *workBuffer;
    std::int16_t texturePitchBytes;
    std::int16_t textureHeight;
    std::int32_t field24;
    std::int32_t field28;
    std::int32_t field2c;
    std::int16_t contentColumns;
    std::int16_t contentRows;
    std::int16_t sourceWidth;
    std::int16_t sourceHeight;
    float uScale;
    float vScale;
    std::int16_t field40;
    std::int16_t field42;
    std::int16_t drawWidth;
    std::int16_t drawHeight;
    std::int16_t cellWidthPixels;
    std::int16_t cellHeightPixels;
#if !SHENMUE_IS_DC
    std::int32_t field4c;
#endif
    std::uint8_t *cellBufferA;
    std::uint8_t *cellBufferB;
    std::uint32_t packedCursor;
    std::uint8_t rowEndColumns[4];
    void *heapBase;
    void *callbackContext;
#if SHENMUE_IS_DC
    void (*callback)(WindEntry *, std::int32_t, void *);
#else
    void (*callback)(WindEntry *, std::int64_t, void *);
#endif
    std::int32_t waitFlag;
#if !SHENMUE_IS_DC
    std::uint32_t reserved84;
#endif
};
#pragma pack(pop)

struct WindTaskData {
    std::uint32_t identifier;
#if !SHENMUE_IS_DC
    std::uint32_t reserved004;
#endif
    WindEntry entries[5];
    std::uint32_t tailWord;
#if !SHENMUE_IS_DC
    std::uint32_t tailReserved;
#endif
};

#if SHENMUE_IS_DC
inline constexpr shen_name TASK_WNDT{SHEN_NAME(WNDT)};
inline constexpr shen_name TASK_WNDB{SHEN_NAME(WNDB)};
inline constexpr std::uint8_t kWindGlyphStamp[7][13]{
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
    {1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1},
    {1, 2, 4, 8, 8, 16, 16, 16, 8, 8, 4, 2, 1},
    {1, 2, 4, 8, 16, 32, 128, 64, 16, 8, 4, 2, 1},
    {1, 2, 4, 8, 8, 16, 16, 64, 64, 8, 4, 2, 1},
    {1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1},
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 4, 2, 1, 0},
};
struct WindWindowParameters {
    std::uint16_t flags;
    std::int16_t columns;
    std::int16_t rows;
    std::int16_t left;
    std::int16_t top;
    std::int16_t offsetX;
    std::int16_t offsetY;
    std::int16_t columnSpacing;
    std::int16_t rowSpacing;
    std::int16_t drawWidth;
    std::int16_t drawHeight;
    std::int16_t sourceWidth;
    std::int16_t sourceHeight;
};
static_assert(sizeof(WindWindowParameters) == 0x1a);
static_assert(sizeof(WindEntry) == 0x60);
static_assert(offsetof(WindEntry, cellWidthPixels) == 0x3c);
static_assert(offsetof(WindEntry, cellHeightPixels) == 0x3e);
static_assert(offsetof(WindEntry, cellBufferA) == 0x40);
static_assert(offsetof(WindEntry, packedCursor) == 0x48);
static_assert(offsetof(WindEntry, rowEndColumns) == 0x4c);
static_assert(offsetof(WindEntry, callbackContext) == 0x54);
static_assert(offsetof(WindEntry, callback) == 0x58);
static_assert(offsetof(WindEntry, waitFlag) == 0x5c);
static_assert(sizeof(WindTaskData) == 0x1e8);
static_assert(offsetof(WindTaskData, entries) == 4);
static_assert(offsetof(WindTaskData, tailWord) == 0x1e4);
void WIND_SelectTextFont(HLib::Task *task, std::int32_t mode);
struct TrialFontStorage;
using WindFontStorage = TrialFontStorage;
inline constexpr std::uint8_t kTextGlyphKernel[4]{2, 1, 1, 1};
inline constexpr std::int16_t kWindJisKatakana[86]{
    167,
    177,
    168,
    178,
    169,
    179,
    170,
    180,
    171,
    181,
    182,
    129,
    183,
    130,
    184,
    131,
    185,
    132,
    186,
    133,
    187,
    134,
    188,
    135,
    189,
    136,
    190,
    137,
    191,
    138,
    192,
    139,
    193,
    140,
    175,
    194,
    141,
    195,
    142,
    196,
    143,
    197,
    198,
    199,
    200,
    201,
    202,
    144,
    145,
    203,
    146,
    147,
    204,
    148,
    149,
    205,
    150,
    151,
    206,
    152,
    153,
    207,
    208,
    209,
    210,
    211,
    172,
    212,
    173,
    213,
    174,
    214,
    215,
    216,
    217,
    218,
    219,
    9582,
    220,
    9584,
    9585,
    166,
    221,
    154,
    155,
    156,
};
struct WindKanaMapping {
    std::uint16_t glyph;
    std::uint16_t kana;
};
inline constexpr WindKanaMapping kWindJisPunctuation[43]{
    {.glyph = 0x2121, .kana = 0x0020},
    {.glyph = 0x212a, .kana = 0x0021},
    {.glyph = 0x2148, .kana = 0x0022},
    {.glyph = 0x2149, .kana = 0x0022},
    {.glyph = 0x2174, .kana = 0x0023},
    {.glyph = 0x2170, .kana = 0x0024},
    {.glyph = 0x2173, .kana = 0x0025},
    {.glyph = 0x2175, .kana = 0x0026},
    {.glyph = 0x2147, .kana = 0x0027},
    {.glyph = 0x214a, .kana = 0x0028},
    {.glyph = 0x214b, .kana = 0x0029},
    {.glyph = 0x2176, .kana = 0x002a},
    {.glyph = 0x215c, .kana = 0x002b},
    {.glyph = 0x2124, .kana = 0x002c},
    {.glyph = 0x215d, .kana = 0x002d},
    {.glyph = 0x213d, .kana = 0x002d},
    {.glyph = 0x2125, .kana = 0x002e},
    {.glyph = 0x213f, .kana = 0x002f},
    {.glyph = 0x2127, .kana = 0x003a},
    {.glyph = 0x2128, .kana = 0x003b},
    {.glyph = 0x2163, .kana = 0x003c},
    {.glyph = 0x2161, .kana = 0x003d},
    {.glyph = 0x2164, .kana = 0x003e},
    {.glyph = 0x2129, .kana = 0x003f},
    {.glyph = 0x2177, .kana = 0x0040},
    {.glyph = 0x214e, .kana = 0x005b},
    {.glyph = 0x216f, .kana = 0x005c},
    {.glyph = 0x214f, .kana = 0x005d},
    {.glyph = 0x2130, .kana = 0x005e},
    {.glyph = 0x2132, .kana = 0x005f},
    {.glyph = 0x2146, .kana = 0x0060},
    {.glyph = 0x2150, .kana = 0x007b},
    {.glyph = 0x2143, .kana = 0x007c},
    {.glyph = 0x2151, .kana = 0x007d},
    {.glyph = 0x2131, .kana = 0x007e},
    {.glyph = 0x2123, .kana = 0x00a1},
    {.glyph = 0x2156, .kana = 0x00a2},
    {.glyph = 0x2157, .kana = 0x00a3},
    {.glyph = 0x2122, .kana = 0x00a4},
    {.glyph = 0x2126, .kana = 0x00a5},
    {.glyph = 0x213c, .kana = 0x00b0},
    {.glyph = 0x212b, .kana = 0x00de},
    {.glyph = 0x212c, .kana = 0x00df},
};
std::int32_t WIND_IsMultibyteLead(std::uint8_t value);
std::uint32_t WIND_ConvertJisGlyph(std::uint32_t glyph);
const std::uint8_t *WIND_GetGlyphSource(const WindFontStorage *font, std::uint16_t glyph);
void WIND_RasterTextGlyph(const WindFontStorage *font, std::uint16_t glyph, std::byte *destination, std::int32_t pitch, std::uint16_t foreground,
    std::uint16_t background, std::uint16_t transparent);
void WIND_ClearTextSurface(std::uint16_t *destination, std::int32_t width, std::int32_t height, std::uint16_t colour);
std::int32_t WIND_CreateTextSurface(
    HLib::Task *task, const char *text, std::int32_t width, std::uint16_t foreground, std::int16_t background, std::int32_t encoding, std::int32_t centered);
std::int32_t WIND_CreateCenteredTextSurface(
    HLib::Task *task, const char *text, std::int32_t width, std::uint16_t foreground, std::int16_t background, std::int32_t encoding);
std::int32_t WIND_CreateVerticalTextSurface(HLib::Task *task, const char *text, std::int32_t height, std::uint16_t foreground, std::int16_t background);
HLib::Task *EnqueueWNDT(std::int16_t queue);
HLib::Task *WIND_InitializeWeatherTask();
WindEntry *WIND_CreateWindow(HLib::Task *task, const WindWindowParameters *parameters);
void WIND_ReleaseSurface(WindEntry *window);
void WIND_AllocateSurface(WindEntry *window);
void WIND_ClearEntry(WindEntry *window);
void WIND_SetWait(WindEntry *window, std::int32_t wait);
void WIND_RequestAdvance(WindEntry *window);
void WIND_ClearAdvanceRequest(WindEntry *window);
void WIND_Show(WindEntry *window);
#else
static_assert(sizeof(WindEntry) == 0x88);
static_assert(offsetof(WindEntry, cellHeightPixels) == 0x4a);
static_assert(offsetof(WindEntry, packedCursor) == 0x60);
static_assert(offsetof(WindEntry, rowEndColumns) == 0x64);
static_assert(offsetof(WindEntry, callbackContext) == 0x70);
static_assert(offsetof(WindEntry, callback) == 0x78);
static_assert(sizeof(WindTaskData) == 0x2b8);
static_assert(offsetof(WindTaskData, entries) == 8);
static_assert(offsetof(WindTaskData, tailWord) == 0x2b0);
#endif

struct WeatherWindData {
    std::int32_t gustAngle;
    std::int32_t currentAngle;
    std::int32_t targetAngle;
    std::int32_t angleStep;
#if SHENMUE_IS_DC
    std::int32_t angleFramesRemaining;
#else
    float angleFramesRemaining;
#endif
    float gustStrength;
    float currentStrength;
    float targetStrength;
    float strengthDelta;
#if SHENMUE_IS_DC
    std::int32_t strengthFramesRemaining;
#else
    float strengthFramesRemaining;
#endif
    float gustVectorZ;
    std::uint32_t reserved_02c;
    float gustVectorX;
    float strengthDivisor;
#if SHENMUE_IS_DC
    std::int32_t gustFramesRemaining;
#else
    float gustFramesRemaining;
#endif
};

static_assert(sizeof(WeatherWindData) == 0x3c);
static_assert(offsetof(WeatherWindData, currentAngle) == 0x04);
static_assert(offsetof(WeatherWindData, targetAngle) == 0x08);
static_assert(offsetof(WeatherWindData, currentStrength) == 0x18);
static_assert(offsetof(WeatherWindData, targetStrength) == 0x1c);
static_assert(offsetof(WeatherWindData, gustVectorZ) == 0x28);
static_assert(offsetof(WeatherWindData, gustVectorX) == 0x30);
static_assert(offsetof(WeatherWindData, gustFramesRemaining) == 0x38);

extern HLib::Task *g_windTask;
extern WeatherWindData *g_weatherWindData;
extern float g_windPulseTimer;
extern float g_fWINDIntensity;
extern std::int32_t g_WINDAngle;

HLib::Task *WIND_Init();
void WIND_Destroy();
bool WIND_ValidateLifecycle();
WeatherWindData *WIND_EnsureWeatherData();
WeatherWindData *WIND_GetWeatherData();
void Weather_Wind_UpdateFromWDAT(WeatherWindData *wind);
void WIND_UpdateWeatherData();
void WIND_ResetWeatherDataForScene();
void WIND_DestroyWeatherData();
void WIND_InitSakuraWindLUT();
void WIND_InitializeSakuraWindState();
void WIND_Callback(HLib::TaskData *taskData);
void WIND_Callback_1(HLib::TaskData *taskData);
void WIND_Callback_0(HLib::TaskData *taskData);
void WIND_UpdateEntry(void *entry);
void WIND_DrawGlyphs(void *entry, std::int64_t flagOffset, std::int32_t examined, std::int32_t index);
void WIND_AspectRatioPass(void *entry);
std::uint64_t RasterizeFontGlyph(const void *font, std::uint16_t glyph, void *pixels, std::int32_t pitch, std::uint16_t foreground, std::uint16_t background);

void Script_Weather_WindUtils(ScenarioContext *context, void *rawParameters);

} // namespace shenmue::s1
