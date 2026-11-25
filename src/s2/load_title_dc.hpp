#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "common/types.hpp"
#define SHENMUE_LOAD_TITLE_DATA static const
#define SHENMUE_LOAD_TITLE_INIT =
namespace shenmue {
namespace s2 {
namespace HLib {
struct Task;
}
extern HLib::Task *g_memoriesTask;
extern HLib::Task *g_timeLimitTask;
extern HLib::Task *g_loadTitleTask;
extern HLib::Task *g_ktdeTask;
extern std::int32_t g_loadTitleDigitTextures[37];
extern std::int32_t g_loadTitleTexture;
extern std::int32_t g_loadFloorTexture;
extern std::uint32_t g_loadTitleFlags;
extern std::int8_t g_loadTitleFadeMode;
#if SHENMUE_S2_GCC295
enum LoadNumberMode { LoadNumberLeadingZeros = 1, LoadNumberReserveLeadingSpace = 2, LoadNumberWide = 8 };
#else
enum LoadNumberMode : std::uint32_t { LoadNumberLeadingZeros = 1, LoadNumberReserveLeadingSpace = 2, LoadNumberWide = 8 };
#endif
SHENMUE_LOAD_TITLE_DATA std::int32_t kLoadNumberPowers[4] SHENMUE_LOAD_TITLE_INIT{1, 10, 100, 1000};
extern const std::uint8_t kLoadTitleWeekSprites[8], kLoadTitleMonthSprites[12];
extern std::int32_t g_gamePauseState;
extern std::int32_t g_loadTitleStream;
extern std::int16_t g_loadFloorWidth, g_loadTitleWidth;
extern std::int8_t g_loadTitleYearTens, g_loadTitleYearUnits, g_loadTitleMonthTens, g_loadTitleMonthUnits, g_loadTitleDayTens, g_loadTitleDayUnits;
extern std::int8_t g_loadTitleMeridiemSprite, g_loadTitleHourTens, g_loadTitleHourUnits, g_loadTitleMinuteTens, g_loadTitleMinuteUnits, g_loadTitleWeek;
void InitST_TitleFunc();
void ST_LoadTitleFadeIn_Main(void *parameter);
void DrawFloorTitle(std::int16_t centreY);
void DrawFloorNumber(std::int32_t value, std::int32_t special);
void DrawLoadNumber(std::int32_t value, std::uint32_t mode, std::int32_t x, std::int32_t y, std::int32_t digits);
void DrawLoadingLabel(std::uint32_t colour);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void scenSetTaskFlags(std::int32_t mode);
#else
void scenSetTaskFlags(std::uint8_t mode);
#endif
SHENMUE_LOAD_TITLE_DATA std::uint16_t kLoadingSpriteRectangles[8] SHENMUE_LOAD_TITLE_INIT{98, 99, 28, 28, 1, 1, 2, 2};
SHENMUE_LOAD_TITLE_DATA std::int16_t kLoadingSpritePositions[3][2] SHENMUE_LOAD_TITLE_INIT{{395, 431}, {395, 431}, {24, 450}};
extern std::int32_t g_loadScreenSpriteTexture;
extern std::uint8_t g_loadingDrawActive;
extern std::int8_t g_loadingDrawMode;
extern std::uint8_t g_loadingDotFrame;
extern std::int16_t g_loadTitleAlpha;
extern std::uint32_t g_loadingSpin;
extern float g_loadingSpinRate;
void SetLoadingDrawFlags(std::int32_t mode);
void RetireLoadingDraw();
std::int32_t IsLoadingDrawActive();
void DrawLoadingModes();
void DrawLoadingDots(std::int32_t x, std::int32_t y);
void DrawLoadingSpinner(std::int32_t x, std::int32_t y);
} // namespace s2
} // namespace shenmue
#undef SHENMUE_LOAD_TITLE_DATA
#undef SHENMUE_LOAD_TITLE_INIT
#endif
