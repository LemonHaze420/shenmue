#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;

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
    std::int32_t field4c;
    std::uint8_t *cellBufferA;
    std::uint8_t *cellBufferB;
    std::uint32_t packedCursor;
    std::uint8_t rowEndColumns[4];
    void *heapBase;
    void *callbackContext;
    void (*callback)(WindEntry *, std::int64_t, void *);
    std::int32_t waitFlag;
    std::uint32_t reserved84;
};
#pragma pack(pop)

struct WindTaskData {
    std::uint32_t identifier;
    std::uint32_t reserved004;
    WindEntry entries[5];
    std::uint32_t tailWord;
    std::uint32_t tailReserved;
};

static_assert(sizeof(WindEntry) == 0x88);
static_assert(offsetof(WindEntry, cellHeightPixels) == 0x4a);
static_assert(offsetof(WindEntry, packedCursor) == 0x60);
static_assert(offsetof(WindEntry, rowEndColumns) == 0x64);
static_assert(offsetof(WindEntry, callbackContext) == 0x70);
static_assert(offsetof(WindEntry, callback) == 0x78);
static_assert(sizeof(WindTaskData) == 0x2b8);
static_assert(offsetof(WindTaskData, entries) == 8);
static_assert(offsetof(WindTaskData, tailWord) == 0x2b0);

struct WeatherWindData {
    std::int32_t gustAngle;
    std::int32_t currentAngle;
    std::int32_t targetAngle;
    std::int32_t angleStep;
    float angleFramesRemaining;
    float gustStrength;
    float currentStrength;
    float targetStrength;
    float strengthDelta;
    float strengthFramesRemaining;
    float gustVectorZ;
    std::uint32_t reserved_02c;
    float gustVectorX;
    float strengthDivisor;
    float gustFramesRemaining;
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
