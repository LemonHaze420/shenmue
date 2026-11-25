#pragma once

#include "s2/time_program_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/legacy_sprite_types.hpp"

namespace shenmue {
namespace s2 {

static const std::uint8_t timeWatchAlignments[3] = {5, 9, 9};
static const std::int16_t timeWatchDimensions[3] = {64, 16, 16};
static const float timeWatchScaleX[3] = {1.0f, 0.25f, 0.2f}, timeWatchScaleY[3] = {1.0f, 1.0f, 1.6f}, timeWatchDepths[3] = {4.0f, 5.0f, 6.0f};
extern LegacyIconSprite g_timeWatchSprites[3], g_timeCompassSprite, g_timePeriodSprite;
extern std::uint8_t g_clockAndCompassSuppressed;
extern std::uint32_t g_timeHudVariant;
extern Vector3 g_timeHudPosition;
extern TimeCompassContext g_timeCompass;
extern std::int32_t g_timeCompassAlpha;
extern std::uint16_t g_moveUiClockAndNavigationHold;
std::int32_t authGetLoaded();
bool IsFightHealthUIActive();
bool CheckTimeHudVisibility();
std::int32_t UpdateCompassAdmission(TimeCompassContext &context);
void TimeProgramHudCallback(void *parameter);
void shenSetClockAndCompassRenderTo(std::uint8_t suppressed);
std::uint32_t SetTimeHudVariant(std::uint32_t variant);

} // namespace s2
} // namespace shenmue

#endif
