#include "s2/time_hud_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/camera_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/map_control_dc.hpp"

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void PrepareDrawSimpleBlack(const LegacyIconSprite &sprite, std::uint32_t colour);
#endif

void TimeProgramHudCallback(void *parameter) {
    const Vector3 *position = reinterpret_cast<const Vector3 *>(parameter);
    if (CheckTimeHudVisibility()) {
        for (std::int32_t index = 0; index < 3; ++index) {
            g_timeWatchSprites[index].x = position->x;
            g_timeWatchSprites[index].y = position->y;
            g_timeWatchSprites[index].scaleX = position->z * timeWatchScaleX[index];
            g_timeWatchSprites[index].scaleY = position->z * timeWatchScaleY[index];
        }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        PrepareDrawSimpleBlack(g_timeWatchSprites[0], 0xccffffffU);
        PrepareDrawSimpleBlack(g_timeWatchSprites[1], 0x7fffffffU);
        PrepareDrawSimpleBlack(g_timeWatchSprites[2], 0x7fffffffU);
#else
        PrepareDrawSimple(g_timeWatchSprites[0], 0xccffffffU);
        PrepareDrawSimple(g_timeWatchSprites[1], 0x7fffffffU);
        PrepareDrawSimple(g_timeWatchSprites[2], 0x7fffffffU);
#endif
        g_timePeriodSprite.x = position->x + 22.0f;
        g_timePeriodSprite.y = position->y + 44.0f;
        PrepareDrawWithMode(g_timePeriodSprite);
        std::int32_t visible = UpdateCompassAdmission(g_timeCompass);
        if (visible) {
            if (g_timeCompassAlpha <= 99)
                g_timeCompassAlpha += 8;
        } else if (g_timeCompassAlpha > 0) {
            g_timeCompassAlpha -= 8;
            visible = 1;
        }
        std::int32_t heading = g_timeCompass.heading;
        const std::int32_t target = static_cast<std::int16_t>(-camCtrlGetNextRotY()) + static_cast<std::int16_t>(GetDirtAngle());
        std::int32_t direction = g_timeCompass.direction;
        const std::int32_t previousDirection = direction;
        const std::int16_t delta = static_cast<std::int16_t>(target - heading);
        if (!visible || (!direction && (delta < 0 ? -delta : delta) <= 2183)) {
            heading = target;
        } else {
            if (delta < 0) {
                heading = static_cast<std::int16_t>(heading - 364);
                direction = -1;
            } else if (delta > 0) {
                heading = static_cast<std::int16_t>(heading + 364);
                direction = 1;
            } else {
                direction = 0;
            }
            if (previousDirection && previousDirection != direction && (delta < 0 ? -delta : delta) <= 2183) {
                heading = target;
                direction = 0;
            }
            g_timeCompass.direction = direction;
        }
        if (visible) {
            g_timeCompassSprite.rotation = -heading;
            g_timeCompassSprite.x = position->x;
            g_timeCompassSprite.y = position->y;
            g_timeCompassSprite.scaleX = g_timeCompassSprite.scaleY = position->z * 1.2f;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            PrepareDrawSimpleBlack(g_timeCompassSprite, static_cast<std::uint32_t>(175 * g_timeCompassAlpha / 100) << 24 | 0x00ffffffU);
#else
            PrepareDrawSimple(g_timeCompassSprite, static_cast<std::uint32_t>(175 * g_timeCompassAlpha / 100) << 24 | 0x00ffffffU);
#endif
        }
        g_timeCompass.heading = heading;
    } else {
        g_timeCompass.heading = static_cast<std::int16_t>(-camCtrlGetNextRotY()) + static_cast<std::int16_t>(GetDirtAngle());
    }
}

} // namespace s2
} // namespace shenmue

#endif
