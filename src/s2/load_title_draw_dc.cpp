#include "s2/load_title_dc.hpp"
#include "s2/legacy_sprite_types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/legacy_sprite_dc.hpp"
#include "s2/date_time.hpp"
namespace shenmue {
namespace s2 {

std::int32_t g_loadTitleStream;
std::int16_t g_loadFloorWidth;
std::int16_t g_loadTitleWidth;
std::int8_t g_loadTitleYearTens, g_loadTitleYearUnits, g_loadTitleMonthTens, g_loadTitleMonthUnits, g_loadTitleDayTens, g_loadTitleDayUnits;
std::int8_t g_loadTitleMeridiemSprite, g_loadTitleHourTens, g_loadTitleHourUnits, g_loadTitleMinuteTens, g_loadTitleMinuteUnits, g_loadTitleWeek;
const std::uint8_t kLoadTitleWeekSprites[8] = {31, 28, 33, 34, 32, 20, 30, 0};
const std::uint8_t kLoadTitleMonthSprites[12] = {29, 17, 23, 19, 26, 13, 27, 25, 24, 14, 0, 0};

void DrawLoadingLabel(std::uint32_t colour) {
    LegacySpritePacket packet;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.uv[0] = 0.0f;
    packet.uv[1] = 0.875f;
    packet.uv[2] = 1.0f;
    packet.uv[3] = 1.0f;
    packet.colour = colour | 0xffffffU;
    packet.quad.flags |= 0x400;
#else
    packet.quad.flags = 0x400;
    packet.colour = colour | 0xffffffU;
    packet.secondaryColour = 0;
    packet.uv[0] = 0.0f;
    packet.uv[1] = 0.875f;
    packet.uv[2] = 1.0f;
    packet.uv[3] = 1.0f;
    packet.reservedTail = 0;
#endif
    AdjustScreenAspectRatio(packet.quad, VerticalStretch, 230, 424, 160, 21);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadScreenSpriteTexture), 6.0f);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void NormalizeCalendarDateTime() {
    DateTime increment;
    increment.year = 0;
    increment.month = 0;
    increment.day = 0;
    increment.hour = 0;
    increment.minute = 0;
    increment.second = 0;
    IncrementDateTimeInPlace(g_calendarRecords[0], increment);
}
#endif

void DrawFloorTitle(std::int16_t centreY) {
    if (g_loadFloorTexture < 0)
        return;
    LegacySpritePacket packet;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.colour = (static_cast<std::uint32_t>(g_loadTitleAlpha) << 24) | 0xffffffU;
    packet.quad.flags |= 0x400;
    TransformScreenSpace(packet.quad, HorizontalStretch, 320, static_cast<std::int32_t>(static_cast<float>(centreY) + -96.0f), g_loadFloorWidth,
        g_loadFloorWidth, 0.649999976f, 0.649999976f, 0);
#else
    packet.quad.flags = 0x400;
    packet.colour = (static_cast<std::uint32_t>(g_loadTitleAlpha) << 24) | 0xffffffU;
    packet.secondaryColour = 0;
    packet.uv[0] = 0.0f;
    packet.uv[1] = 0.0f;
    packet.uv[2] = 1.0f;
    packet.uv[3] = 1.0f;
    packet.reservedTail = 0;
    TransformScreenSpace(packet.quad, HorizontalStretch, 320, static_cast<std::int32_t>(static_cast<float>(centreY) - 96.0f), g_loadFloorWidth,
        g_loadFloorWidth, 0.649999976f, 0.649999976f, 0);
#endif
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadFloorTexture), 6.0f);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float g_loadFloorNumberX = 320.0f, g_loadFloorNumberY = 238.0f;

void DrawFloorNumber(std::int32_t value, std::int32_t special) {
    std::int32_t magnitude = value;
    if (magnitude <= 0)
        --magnitude;
    if (magnitude < 0)
        magnitude = -magnitude;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.colour = (static_cast<std::uint32_t>(g_loadTitleAlpha) << 24) | 0xffffffU;
    packet.quad.flags |= 0x400;
    std::int32_t suffix;
    float x, suffixX;
    if (special) {
        suffix = g_loadTitleDigitTextures[12];
        suffixX = g_loadFloorNumberX;
    } else {
        if (value <= 0) {
            if (magnitude / 10)
                x = g_loadFloorNumberX;
            else
                x = g_loadFloorNumberX + 20.0f;
            suffixX = x - 40.0f;
            suffix = g_loadTitleDigitTextures[10];
        } else {
            suffix = g_loadTitleDigitTextures[11];
            if (magnitude / 10) {
                x = g_loadFloorNumberX - 40.0f;
                suffixX = x + 80.0f;
            } else {
                x = g_loadFloorNumberX - 20.0f;
                suffixX = x + 40.0f;
            }
        }
        const std::int32_t tens = magnitude / 10;
        if (tens) {
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>(g_loadFloorNumberY), 40, 40);
            DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[tens]), 9.0f);
            x += 40.0f;
        }
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>(g_loadFloorNumberY), 40, 40);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[magnitude - magnitude / 10 * 10]), 9.0f);
    }
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(suffixX), static_cast<std::int32_t>(g_loadFloorNumberY), 40, 40);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(suffix), 9.0f);
}
#else
// @verified - d
void DrawFloorNumber(std::int32_t value, std::int32_t special) {
    std::int32_t magnitude = value;
    if (magnitude <= 0)
        --magnitude;
    if (magnitude < 0)
        magnitude = -magnitude;
    LegacySpritePacket packet;
    packet.quad.flags = 0x400;
    packet.colour = (static_cast<std::uint32_t>(g_loadTitleAlpha) << 24) | 0xffffffU;
    packet.secondaryColour = 0;
    packet.uv[0] = 0.0f;
    packet.uv[1] = 0.0f;
    packet.uv[2] = 1.0f;
    packet.uv[3] = 1.0f;
    packet.reservedTail = 0;
    std::int32_t suffix;
    float suffixX;
    if (special) {
        suffix = g_loadTitleDigitTextures[12];
        suffixX = 320.0f;
    } else {
        float x;
        if (value <= 0) {
            x = magnitude / 10 ? 320.0f : 340.0f;
            suffixX = x - 40.0f;
            suffix = g_loadTitleDigitTextures[10];
        } else {
            x = magnitude / 10 ? 280.0f : 300.0f;
            suffixX = x + (magnitude / 10 ? 80.0f : 40.0f);
            suffix = g_loadTitleDigitTextures[11];
        }
        const std::int32_t tens = magnitude / 10;
        if (tens) {
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), 238, 40, 40);
            DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[tens]), 9.0f);
            x += 40.0f;
        }
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), 238, 40, 40);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[magnitude - magnitude / 10 * 10]), 9.0f);
    }
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(suffixX), 238, 40, 40);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(suffix), 9.0f);
}
#endif

// @verified - d
void DrawLoadNumber(std::int32_t value, std::uint32_t mode, std::int32_t x, std::int32_t y, std::int32_t digits) {
    std::int32_t powers[4] = {1, 10, 100, 1000};
    std::int32_t advance;
    if (mode & LoadNumberWide)
        advance = 32;
    else
        advance = 28;
    std::int32_t remaining = value;
    std::uint32_t started = 0;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400;
    packet.colour = (static_cast<std::uint32_t>(g_loadTitleAlpha) << 24) | 0xffffffU;
    started = mode & LoadNumberLeadingZeros;
    for (std::int32_t index = digits - 1; index >= 0; --index) {
        const std::int32_t power = powers[index];
        const std::int32_t digit = remaining / power;
        remaining -= digit * power;
        if (digit)
            started = 1;
        if (started || index <= 0) {
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 32, 32);
            DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[digit]), 9.0f);
            x += advance;
        } else if (mode & LoadNumberReserveLeadingSpace) {
            x += advance;
        }
    }
}

void ST_LoadTitleFadeIn_Main(void *) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.colour = (static_cast<std::uint32_t>(g_loadTitleAlpha) << 24) | 0xffffffU;
    packet.quad.flags |= 0x400;
    if (g_loadTitleFadeMode == 1 || g_loadTitleFadeMode == 6) {
        if (g_loadTitleFadeMode == 6)
            packet.colour = static_cast<std::uint32_t>(g_loadTitleAlpha) << 24;
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 240, 640, 480);
        DrawLegacyColourPacket(packet, 11.0f);
        return;
    }
    if (g_gamePauseState || g_loadTitleFadeMode == 8) {
        DrawLoadingLabel(static_cast<std::uint32_t>(g_loadTitleAlpha) << 24);
        return;
    }
    std::int16_t x;
    std::int16_t y;
    if (g_loadTitleTexture != -1) {
        if (g_loadTitleFlags & 4U)
            y = 240;
        else
            y = 160;
        if (g_loadFloorTexture != -1)
            y += 20;
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, y, g_loadTitleWidth, g_loadTitleWidth);
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleTexture), 6.0f);
        DrawFloorTitle(y);
        if (g_loadTitleFlags & 8U) {
            if (g_loadTitleFlags & 0x10U)
                DrawFloorNumber(static_cast<std::int32_t>(g_loadTitleFlags), 1);
            else
                DrawFloorNumber(g_loadTitleStream, 0);
        }
    }
    if (g_loadTitleFlags & 4U)
        return;
    x = 144;
    y = 344;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 128, 128);
    if (g_loadTitleMonthTens)
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[kLoadTitleMonthSprites[g_loadTitleMonthUnits - 1]]), 9.0f);
    else
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[kLoadTitleMonthSprites[g_loadTitleMonthUnits + 1]]), 9.0f);
    x += 58;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y + 8, 16, 16);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[18]), 9.0f);
    x += 28;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 32, 32);
    if (g_loadTitleDayTens)
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[g_loadTitleDayTens]), 9.0f);
    x += 28;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 32, 32);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[g_loadTitleDayUnits]), 9.0f);
    x += 30;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y + 8, 16, 16);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[16]), 9.0f);
    x += 28;
    DrawLoadNumber(g_loadTitleYearTens * 10 + 1900 + g_loadTitleYearUnits, 0, x, y, 4);
    x += 84;
    x += 76;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 128, 128);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[35]), 9.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 128, 128);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[kLoadTitleWeekSprites[g_loadTitleWeek]]), 9.0f);
    x = 232;
    y = 296;
    DrawLoadNumber(g_loadTitleHourTens * 10 + g_loadTitleHourUnits, LoadNumberWide | LoadNumberReserveLeadingSpace, x, y, 2);
    x = 290;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y, 32, 32);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[15]), 7.0f);
    x = 316;
    DrawLoadNumber(g_loadTitleMinuteTens * 10 + g_loadTitleMinuteUnits, LoadNumberLeadingZeros | LoadNumberReserveLeadingSpace, x, y, 2);
    x = 392;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, x, y + 4, 64, 64);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(g_loadTitleDigitTextures[g_loadTitleMeridiemSprite]), 7.0f);
    if (!(g_loadTitleFlags & 3U) && g_loadTitleFadeMode != 7)
        DrawLoadingLabel(static_cast<std::uint32_t>(g_loadTitleAlpha) << 24);
}

} // namespace s2
} // namespace shenmue

#endif
