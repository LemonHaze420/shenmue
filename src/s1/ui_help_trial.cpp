#include "shenmue.h"

#if SHENMUE_IS_DEMO
#pragma region WhatsShenmue
#include "s1/ui_internal.hpp"
#include "s1/ui_help_trial.hpp"

namespace shenmue::s1::ui_runtime_detail {
namespace {
ScreenSpaceParams g_trialHelpPageSprite{};
ScreenSpaceParams g_trialHelpButtonSprite{};
std::uint8_t g_trialHelpPageToken{};
} // namespace

void UI_HELPPrepareTrialPage(std::uint8_t token) {
    if (GetAreaJumpEnabled() && token < std::size(kTrialHelpTokenMap))
        token = kTrialHelpTokenMap[token];
    if (token >= std::size(kHelpPages))
        token = 0;
    g_trialHelpPageToken = token;
    g_trialHelpPageSprite = {.positionScaleX = 1.0f, .positionScaleY = 1.0f, .depthOrScale = 13.0f, .scaledHeight = 32, .alignmentType = 5};
    g_trialHelpButtonSprite = {
        .positionScaleX = 1.2f, .positionScaleY = 1.2f, .depthOrScale = 13.0f, .scaledWidth = 32, .scaledHeight = 32, .alignmentType = 5};
    for (std::size_t row = 0; row < g_helpIconSurfaceIds.size(); ++row) {
        const std::int16_t length = kHelpLineLengths[token][row];
        g_helpIconSurfaceIds[row] = length < 0 ? -1 : AllocatePVRSurface(kHelpPages[token][row], kHelpTextWidths[length], 0xffff, 0xf000, 1, false);
    }
}

void UI_HELPDrawColouredIcon(const ScreenSpaceParams *params, std::uint32_t colour) {
    AdjustScreenSpaceWithParams(params, (colour & 0x00ffffffU) | 0xff000000U, 0);
}

void UI_HELPDrawMarkers() {
    ScreenSpaceFillParams marker{};
    marker.alignmentType = 5;
    marker.scaledWidth = 10;
    marker.scaledHeight = 10;
    marker.positionScaleX = 1.0f;
    marker.positionScaleY = 1.0f;
    marker.angle = 0;
    marker.colour = 0xffff0000U;
    marker.maxExtent = 14.0f;
    if (g_trialHelpPageToken == 51 || g_trialHelpPageToken == 98) {
        marker.halfScreenX = g_trialHelpPageToken == 51 ? 169.0f : 214.0f;
        marker.halfScreenY = std::bit_cast<float>(kHelpLayoutWords[6]) + 232.0f;
        AdjustScreenSpaceWithRenderFlags(&marker, 0);
        marker.halfScreenX += 24.0f;
        marker.halfScreenY = std::bit_cast<float>(kHelpLayoutWords[7]) + 232.0f;
        AdjustScreenSpaceWithRenderFlags(&marker, 0);
    } else {
        marker.halfScreenX = g_trialHelpPageToken == 30 ? 319.5f : 343.5f;
        marker.halfScreenY = std::bit_cast<float>(kHelpLayoutWords[6]) + 244.0f;
        AdjustScreenSpaceWithRenderFlags(&marker, 0);
    }
}

namespace {

float HelpLayoutFloat(std::size_t index) {
    return std::bit_cast<float>(kHelpLayoutWords[index]);
}

std::int16_t HelpLayoutHalfword(std::size_t byteOffset) {
    return static_cast<std::int16_t>(kHelpLayoutWords[byteOffset / 4] >> ((byteOffset & 2U) * 8U));
}

void DrawHelpFrame(std::int8_t side, std::int8_t colour, std::int8_t column, std::int8_t line, std::int8_t width, std::int8_t height) {
    const float x = HelpLayoutFloat(96 + column);
    const float y = HelpLayoutFloat((side ? 87 : 77) + line);
    const std::int16_t frameWidth = HelpLayoutHalfword(414 + width * 2);
    const std::int16_t frameHeight = HelpLayoutHalfword(396 + height * 2);
    const bool shifted = g_trialHelpPageToken == 98;
    ScreenSpaceFillParams panel{};
    panel.alignmentType = 5;
    panel.halfScreenX = shifted ? x + 4.0f : x;
    panel.halfScreenY = y;
    panel.scaledWidth = frameWidth;
    panel.scaledHeight = frameHeight;
    panel.positionScaleX = panel.positionScaleY = 1.0f;
    panel.angle = 0;
    panel.colour = kHelpLayoutWords[105 + colour];
    panel.maxExtent = 11.0f;
    AdjustScreenSpaceWithRenderFlags(&panel, 0);
    ScreenSpaceFillParams edge{};
    edge.alignmentType = 5;
    edge.halfScreenX = shifted ? x + 4.0f : x;
    edge.halfScreenY = (y - static_cast<float>(frameHeight) * 0.5f) - 1.0f;
    edge.scaledWidth = static_cast<std::int16_t>(frameWidth + 2);
    edge.scaledHeight = 2;
    edge.positionScaleX = edge.positionScaleY = 1.0f;
    edge.angle = 0;
    edge.colour = kHelpLayoutWords[109 + colour];
    edge.maxExtent = 11.0f;
    AdjustScreenSpaceWithRenderFlags(&edge, 0);
    edge.halfScreenY = std::fma(0.5f, static_cast<float>(frameHeight), y) + 1.0f;
    AdjustScreenSpaceWithRenderFlags(&edge, 0);
    edge.halfScreenX = (x - static_cast<float>(frameWidth) * 0.5f) + (shifted ? 3.0f : -1.0f);
    edge.halfScreenY = y;
    edge.scaledWidth = 2;
    edge.scaledHeight = static_cast<std::int16_t>(frameHeight + 2);
    edge.colour = kHelpLayoutWords[109 + colour];
    AdjustScreenSpaceWithRenderFlags(&edge, 0);
    edge.halfScreenX = std::fma(0.5f, static_cast<float>(frameWidth), x) + (shifted ? 5.0f : 1.0f);
    AdjustScreenSpaceWithRenderFlags(&edge, 0);
}

void DrawHelpRowIcon(std::int8_t row, std::int8_t icon, std::int8_t side) {
    ScreenSpaceParams &button = g_trialHelpButtonSprite;
    button.halfScreenX = 96.0f;
    button.halfScreenY = 232.0f;
    button.scaledWidth = button.scaledHeight = 32;
    button.positionScaleX = button.positionScaleY = std::bit_cast<float>(0x3f999999U);
    button.pvrSurfaceIndex = HelpLayoutHalfword(96 + icon * 2);
    button.halfScreenY = HelpLayoutFloat(row) + 232.0f;
    if (icon == 10)
        button.scaledWidth = 64;
    if (icon == 8) {
        button.scaledWidth = button.scaledHeight = 64;
        button.positionScaleX = 1.0f;
        button.positionScaleY = std::bit_cast<float>(0x3f4cccccU);
        button.halfScreenY += 5.0f;
    }
    button.angle = icon == 12 ? 0xffffc001U : icon == 13 ? 0x3fffU : 0;
    if (side == 1)
        button.halfScreenX += 279.5f;
    else
        button.halfScreenX -= 32.0f;
    UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10 + icon]);
}

} // namespace

void UI_HELPDrawTrialPage() {
    DrawMiddleOfScreenStretchedAdjusted(0x76000000U, 10.0f);
    const std::int32_t pageToken = static_cast<std::int8_t>(g_trialHelpPageToken);
    ScreenSpaceParams &page = g_trialHelpPageSprite;
    for (std::int16_t row = 0; row < 20; ++row) {
        const std::int16_t length = kHelpLineLengths[pageToken][row];
        if (length <= 0)
            continue;
        page.scaledWidth = kHelpTextWidths[length];
        if (!row) {
            page.halfScreenY = HelpLayoutFloat(row / 2) + 232.0f;
            for (std::int32_t index = 0; index < 5; ++index) {
                if (kHelpTextWidths[23 + index] == page.scaledWidth)
                    page.halfScreenX = HelpLayoutFloat(113 + index);
            }
        } else {
            const std::uint8_t flags = kHelpLineFlags[pageToken][row];
            for (std::int32_t index = 0; index < 5; ++index) {
                if (kHelpTextWidths[23 + index] != page.scaledWidth)
                    continue;
                if (row % 2) {
                    if (flags == 1)
                        page.halfScreenX = HelpLayoutFloat(118 + index) + 450.0f;
                    else if (flags == 2)
                        page.halfScreenX = HelpLayoutFloat(118 + index) + 564.0f;
                    else if (flags == 3)
                        page.halfScreenX = HelpLayoutFloat(118 + index) + 560.0f;
                    else
                        page.halfScreenX = HelpLayoutFloat(113 + index);
                } else if (flags == 1) {
                    page.halfScreenX = HelpLayoutFloat(118 + index) + 135.0f;
                } else {
                    page.halfScreenX = HelpLayoutFloat(113 + index);
                }
            }
            page.halfScreenY = HelpLayoutFloat(row / 2) + 232.0f;
        }
        page.pvrSurfaceIndex = g_helpIconSurfaceIds[row];
        if (page.pvrSurfaceIndex != -1)
            AdjustScreenSpaceWithParams_0(&page, 0);
    }
    ScreenSpaceParams &button = g_trialHelpButtonSprite;
    button.halfScreenX = 96.0f;
    button.halfScreenY = 232.0f;
    button.depthOrScale = 13.0f;
    button.scaledWidth = button.scaledHeight = 32;
    button.positionScaleX = button.positionScaleY = std::bit_cast<float>(0x3f999999U);
    button.angle = 0;
    const std::int32_t token = static_cast<std::int8_t>(g_trialHelpPageToken);
    switch (token) {
    case 0:
    case 56:
        DrawHelpFrame(1, 3, 1, 3, 1, 1);
        DrawHelpFrame(1, 1, 2, 3, 1, 1);
        DrawHelpFrame(1, 0, 1, 7, 1, 3);
        DrawHelpFrame(1, 2, 2, 6, 1, 1);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(3, 10, 0);
        DrawHelpRowIcon(3, 7, 1);
        DrawHelpRowIcon(4, 5, 0);
        DrawHelpRowIcon(4, 1, 1);
        DrawHelpRowIcon(6, 6, 0);
        DrawHelpRowIcon(6, 2, 1);
        DrawHelpRowIcon(7, 0, 0);
        DrawHelpRowIcon(7, 3, 1);
        DrawHelpRowIcon(8, 3, 0);
        DrawHelpRowIcon(9, 4, 0);
        DrawHelpRowIcon(9, 8, 1);
        button.halfScreenX = 96.0f;
        button.depthOrScale = 13.0f;
        button.scaledWidth = button.scaledHeight = 32;
        button.positionScaleX = button.positionScaleY = std::bit_cast<float>(0x3f999999U);
        button.halfScreenX += g_trialHelpPageToken == 0 ? 247.0f : 300.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(100);
        button.halfScreenY = HelpLayoutFloat(1) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[12]);
        break;
    case 1:
    case 57:
        DrawHelpFrame(0, 2, 1, 4, 1, 2);
        DrawHelpFrame(1, 1, 2, 3, 1, 1);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(3, 2, 0);
        DrawHelpRowIcon(3, 0, 1);
        DrawHelpRowIcon(4, 1, 1);
        DrawHelpRowIcon(5, 3, 0);
        DrawHelpRowIcon(9, 8, 1);
        break;
    case 2:
    case 58:
        DrawHelpFrame(0, 2, 1, 4, 1, 2);
        DrawHelpFrame(0, 1, 2, 4, 1, 2);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(3, 2, 0);
        DrawHelpRowIcon(3, 6, 1);
        DrawHelpRowIcon(4, 7, 1);
        DrawHelpRowIcon(5, 3, 0);
        DrawHelpRowIcon(5, 1, 1);
        DrawHelpRowIcon(9, 8, 1);
        break;
    case 3:
    case 17:
    case 39:
    case 44:
    case 60:
    case 76:
    case 90:
    case 91: {
        const std::int8_t row = token == 3 || token == 60 ? 6 : token == 17 || token == 76 || token == 44 || token == 90 ? 4 : 5;
        DrawHelpFrame(0, 2, 1, row, 1, 0);
        DrawHelpRowIcon(row, 2, 0);
        if (g_trialHelpPageToken == 39 || g_trialHelpPageToken == 44 || g_trialHelpPageToken == 91 || g_trialHelpPageToken == 90) {
            button.pvrSurfaceIndex = HelpLayoutHalfword(100);
            button.halfScreenY = HelpLayoutFloat(1) + 232.0f;
            button.halfScreenX +=
                g_trialHelpPageToken == 39 || g_trialHelpPageToken == 44 ? std::bit_cast<float>(0x43304cccU) : std::bit_cast<float>(0x434a1999U);
            UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[12]);
        }
        if (g_trialHelpPageToken == 3 || g_trialHelpPageToken == 60) {
            DrawHelpFrame(0, 3, 2, 6, 1, 0);
            DrawHelpRowIcon(6, 8, 1);
        } else if (g_trialHelpPageToken == 44 || g_trialHelpPageToken == 90) {
            DrawHelpFrame(0, 3, 2, 4, 1, 0);
            DrawHelpRowIcon(4, 8, 1);
        }
        break;
    }
    case 4:
    case 22:
    case 37:
    case 61:
    case 81: {
        const std::int8_t row = token == 4 || token == 61 ? 6 : 4;
        DrawHelpFrame(0, 2, 0, row, 0, 0);
        button.pvrSurfaceIndex = HelpLayoutHalfword(96);
        button.halfScreenY = HelpLayoutFloat(row) + 232.0f;
        button.halfScreenX += g_trialHelpPageToken == 61 || g_trialHelpPageToken == 81 ? 168.0f : 200.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10]);
        for (std::int32_t icon = 2; icon < 6; ++icon) {
            button.pvrSurfaceIndex = HelpLayoutHalfword(96 + icon * 2);
            button.halfScreenY = HelpLayoutFloat(row) + 232.0f;
            button.halfScreenX += 49.0f;
            UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10 + icon]);
        }
        break;
    }
    case 5:
    case 62:
        DrawHelpFrame(0, 1, 2, 5, 1, 0);
        DrawHelpRowIcon(5, 0, 0);
        DrawHelpRowIcon(5, 4, 1);
        if (g_trialHelpPageToken == 5) {
            DrawHelpFrame(0, 0, 1, 5, 1, 0);
            DrawHelpFrame(0, 2, 2, 7, 1, 0);
            DrawHelpRowIcon(7, 2, 1);
        } else {
            DrawHelpFrame(1, 0, 1, 5, 1, 1);
            DrawHelpFrame(0, 2, 2, 8, 1, 0);
            DrawHelpRowIcon(8, 2, 1);
        }
        break;
    case 6:
    case 63:
        DrawHelpFrame(1, 1, 0, 6, 0, 1);
        DrawHelpRowIcon(6, 1, 0);
        DrawHelpRowIcon(6, 6, 1);
        DrawHelpRowIcon(7, 7, 1);
        break;
    case 7:
    case 64:
        DrawHelpFrame(0, 0, 1, 5, 1, 0);
        DrawHelpFrame(0, 1, 2, 5, 1, 0);
        DrawHelpFrame(0, 2, 2, 7, 1, 0);
        DrawHelpRowIcon(5, 0, 0);
        DrawHelpRowIcon(5, 1, 1);
        DrawHelpRowIcon(7, 2, 1);
        break;
    case 8:
    case 9:
    case 67:
    case 68: {
        const std::int8_t row = token == 8 || token == 67 ? 5 : 6;
        DrawHelpFrame(1, 0, 1, row + 1, 1, 3);
        DrawHelpFrame(1, 2, 2, row, 1, 1);
        DrawHelpRowIcon(row, 0, 0);
        DrawHelpRowIcon(row, 2, 1);
        DrawHelpRowIcon(row + 1, 3, 1);
        break;
    }
    case 10:
    case 69:
        if (token == 10)
            DrawHelpFrame(1, 2, 0, 6, 0, 3);
        else
            DrawHelpFrame(0, 2, 0, 7, 0, 4);
        break;
    case 11:
    case 12:
    case 13:
        break;
    case 14:
    case 33:
    case 34:
    case 35:
    case 36:
    case 70:
    case 71:
    case 72:
    case 73: {
        const std::int8_t row = token == 14 ? 5 : 3;
        DrawHelpFrame(0, 0, 1, row, 1, 0);
        DrawHelpFrame(1, 2, 2, row, 1, 1);
        DrawHelpRowIcon(row, 0, 0);
        DrawHelpRowIcon(row, 2, 1);
        DrawHelpRowIcon(row + 1, 3, 1);
        break;
    }
    case 15:
    case 74:
        DrawHelpFrame(1, 0, 1, 5, 1, 1);
        DrawHelpFrame(0, 1, 2, 5, 1, 0);
        DrawHelpFrame(0, 2, 2, 8, 1, 2);
        DrawHelpFrame(0, 3, 1, 9, 1, 0);
        DrawHelpRowIcon(5, 0, 0);
        DrawHelpRowIcon(5, 1, 1);
        DrawHelpRowIcon(6, 3, 0);
        DrawHelpRowIcon(7, 2, 1);
        DrawHelpRowIcon(8, 4, 1);
        DrawHelpRowIcon(9, 5, 1);
        DrawHelpRowIcon(9, 9, 0);
        break;
    case 16:
    case 21:
    case 75:
    case 80: {
        const std::int8_t row = token == 16 || token == 75 ? 3 : 2;
        DrawHelpFrame(1, 0, 1, row + 1, 1, 3);
        DrawHelpFrame(0, 1, 2, row, 1, 0);
        DrawHelpFrame(0, 2, 2, row + 4, 1, 2);
        DrawHelpFrame(0, 3, 1, row + 5, 1, 0);
        DrawHelpRowIcon(row, 6, 0);
        DrawHelpRowIcon(row, 1, 1);
        DrawHelpRowIcon(row + 1, 0, 0);
        DrawHelpRowIcon(row + 3, 3, 0);
        DrawHelpRowIcon(row + 3, 2, 1);
        DrawHelpRowIcon(row + 4, 4, 1);
        DrawHelpRowIcon(row + 5, 5, 1);
        DrawHelpRowIcon(row + 5, 7, 0);
        break;
    }
    case 18:
    case 77: {
        const std::int8_t row = token == 18 ? 3 : 4;
        DrawHelpFrame(0, 0, 1, row, 1, 0);
        DrawHelpFrame(0, 2, 2, row + 1, 1, 2);
        DrawHelpRowIcon(row, 0, 0);
        DrawHelpRowIcon(row, 2, 1);
        DrawHelpRowIcon(row + 1, 3, 1);
        DrawHelpRowIcon(row + 2, 5, 1);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(9, 8, 1);
        break;
    }
    case 19:
    case 78:
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpFrame(1, 1, 2, 4, 1, 1);
        DrawHelpFrame(0, 2, 2, 7, 1, 0);
        DrawHelpRowIcon(4, 0, 0);
        DrawHelpRowIcon(4, 6, 1);
        DrawHelpRowIcon(5, 7, 1);
        DrawHelpRowIcon(7, 3, 1);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(9, 8, 1);
        break;
    case 20:
    case 79: {
        const std::int8_t row = token == 20 ? 4 : 5;
        DrawHelpFrame(0, 0, 1, row, 1, 0);
        DrawHelpFrame(0, 2, 2, row, 1, 0);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(row, 0, 0);
        DrawHelpRowIcon(row, 3, 1);
        DrawHelpRowIcon(9, 8, 1);
        break;
    }
    case 23:
    case 82:
        DrawHelpFrame(0, 0, 1, 3, 1, 2);
        DrawHelpFrame(0, 1, 2, 2, 1, 0);
        DrawHelpFrame(1, 2, 2, 4, 1, 1);
        DrawHelpFrame(0, 3, 1, 7, 1, 0);
        DrawHelpFrame(0, 3, 2, 7, 1, 0);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(2, 6, 0);
        DrawHelpRowIcon(2, 1, 1);
        DrawHelpRowIcon(3, 7, 0);
        DrawHelpRowIcon(4, 0, 0);
        DrawHelpRowIcon(4, 2, 1);
        DrawHelpRowIcon(5, 3, 1);
        DrawHelpRowIcon(7, 9, 0);
        DrawHelpRowIcon(7, 5, 1);
        DrawHelpRowIcon(9, 8, 1);
        break;
    case 24:
    case 83:
        DrawHelpFrame(1, 0, 1, 3, 1, 3);
        DrawHelpFrame(0, 1, 2, 2, 1, 0);
        DrawHelpFrame(1, 2, 2, 4, 1, 1);
        DrawHelpFrame(0, 3, 1, 7, 1, 0);
        DrawHelpFrame(0, 3, 2, 7, 1, 0);
        DrawHelpRowIcon(2, 6, 0);
        DrawHelpRowIcon(2, 1, 1);
        DrawHelpRowIcon(3, 7, 0);
        DrawHelpRowIcon(4, 0, 0);
        DrawHelpRowIcon(4, 2, 1);
        DrawHelpRowIcon(5, 2, 0);
        DrawHelpRowIcon(5, 3, 1);
        DrawHelpRowIcon(7, 9, 0);
        DrawHelpRowIcon(7, 5, 1);
        break;
    case 25:
    case 84:
        DrawHelpFrame(0, 0, 1, 3, 1, 2);
        DrawHelpFrame(1, 1, 2, 2, 1, 1);
        DrawHelpFrame(1, 2, 2, 5, 1, 1);
        DrawHelpFrame(0, 3, 1, 6, 1, 0);
        DrawHelpRowIcon(2, 6, 0);
        DrawHelpRowIcon(2, 1, 1);
        DrawHelpRowIcon(3, 7, 0);
        DrawHelpRowIcon(3, 4, 1);
        DrawHelpRowIcon(4, 0, 0);
        DrawHelpRowIcon(5, 2, 1);
        DrawHelpRowIcon(6, 3, 1);
        DrawHelpRowIcon(6, 5, 0);
        break;
    case 26:
    case 85: {
        const std::int8_t row = token == 26 ? 3 : 4;
        DrawHelpFrame(0, 3, 1, row, 1, 0);
        DrawHelpFrame(0, 2, 2, row, 1, 0);
        DrawHelpRowIcon(row, 2, 0);
        DrawHelpRowIcon(row, 3, 1);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(9, 8, 1);
        button.pvrSurfaceIndex = HelpLayoutHalfword(118);
        button.scaledWidth = 128;
        button.scaledHeight = 64;
        button.positionScaleX = button.positionScaleY = std::bit_cast<float>(0x3f199999U);
        button.halfScreenX = 96.0f;
        if (g_trialHelpPageToken == 26) {
            button.halfScreenY = HelpLayoutFloat(1) + 232.0f;
            button.halfScreenX += std::bit_cast<float>(0x431b1998U);
        } else {
            button.halfScreenY = HelpLayoutFloat(2) + 232.0f;
            button.halfScreenX -= 28.5f;
        }
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[21]);
        break;
    }
    case 27:
    case 86:
        DrawHelpFrame(0, 0, 2, 4, 1, 0);
        DrawHelpFrame(1, 2, 1, 4, 1, 1);
        DrawHelpRowIcon(4, 0, 1);
        DrawHelpRowIcon(4, 2, 0);
        DrawHelpRowIcon(5, 3, 0);
        DrawHelpFrame(0, 3, 2, 6, 1, 0);
        DrawHelpRowIcon(6, 8, 1);
        break;
    case 28:
    case 87:
        button.halfScreenX = 96.0f;
        button.halfScreenX += g_trialHelpPageToken == 28 ? std::bit_cast<float>(0x430de666U) : 195.5f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(100);
        button.halfScreenY = HelpLayoutFloat(g_trialHelpPageToken == 28 ? 3 : 4) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[12]);
        break;
    case 29:
    case 65:
        DrawHelpFrame(0, 2, 0, 6, 0, 0);
        button.halfScreenX += g_trialHelpPageToken == 29 ? 150.0f : 120.0f;
        for (std::int32_t icon = 2; icon < 5; ++icon) {
            button.pvrSurfaceIndex = HelpLayoutHalfword(96 + icon * 2);
            button.halfScreenY = HelpLayoutFloat(6) + 232.0f;
            button.halfScreenX += 49.0f;
            UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10 + icon]);
        }
        break;
    case 30:
    case 66:
        DrawHelpFrame(1, 2, 1, 8, 1, 1);
        DrawHelpFrame(1, 3, 2, 8, 1, 1);
        DrawHelpRowIcon(8, 2, 0);
        DrawHelpRowIcon(8, 0, 1);
        DrawHelpRowIcon(9, 3, 0);
        button.halfScreenX = g_trialHelpPageToken == 30 ? 319.5f : 343.5f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(96);
        button.halfScreenY = HelpLayoutFloat(6) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10]);
        UI_HELPDrawMarkers();
        DrawHelpRowIcon(9, 8, 1);
        break;
    case 31:
    case 59:
        DrawHelpFrame(0, 2, 1, 4, 1, 2);
        DrawHelpFrame(0, 1, 2, 3, 1, 0);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(3, 2, 0);
        DrawHelpRowIcon(3, 1, 1);
        DrawHelpRowIcon(5, 3, 0);
        DrawHelpRowIcon(9, 8, 1);
        break;
    case 32:
    case 88:
        if (token == 32)
            button.halfScreenX += 133.0f;
        else
            button.halfScreenX -= 40.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(106);
        button.halfScreenY = HelpLayoutFloat(token == 32 ? 4 : 5) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[15]);
        if (g_trialHelpPageToken == 32)
            button.halfScreenX += 140.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(104);
        button.halfScreenY = HelpLayoutFloat(g_trialHelpPageToken == 32 ? 6 : 8) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[14]);
        break;
    case 38:
    case 89: {
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpFrame(0, 3, 2, 4, 1, 0);
        DrawHelpRowIcon(4, 0, 0);
        button.halfScreenX += std::bit_cast<float>(0x43854cccU);
        const bool first = g_trialHelpPageToken == 38;
        for (std::int32_t icon = 6; icon < 8; ++icon) {
            button.pvrSurfaceIndex = HelpLayoutHalfword(96 + icon * 2);
            button.halfScreenY = HelpLayoutFloat(4) + 232.0f;
            button.halfScreenX += first ? 40.0f : 33.0f;
            UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10 + icon]);
        }
        if (first) {
            button.halfScreenX = 2.0f;
            for (std::int32_t icon = 6; icon < 8; ++icon) {
                button.pvrSurfaceIndex = HelpLayoutHalfword(96 + icon * 2);
                button.halfScreenY = HelpLayoutFloat(6) + 232.0f;
                button.halfScreenX += 45.0f;
                UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10 + icon]);
            }
        }
        button.halfScreenX = 4.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(108);
        button.halfScreenY = HelpLayoutFloat(first ? 8 : 7) + 232.0f;
        button.halfScreenX += 40.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[16]);
        button.pvrSurfaceIndex = HelpLayoutHalfword(110);
        button.halfScreenY = HelpLayoutFloat(first ? 8 : 7) + 232.0f;
        button.halfScreenX += 55.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[17]);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(9, 8, 1);
        break;
    }
    case 40:
    case 92:
        DrawHelpFrame(0, 0, 1, 4, 1, 0);
        DrawHelpRowIcon(4, 0, 0);
        break;
    case 41:
    case 93: {
        const std::int8_t row = token == 41 ? 4 : 5;
        button.halfScreenX = token == 41 ? std::bit_cast<float>(0x433a4cccU) : std::bit_cast<float>(0x439fccccU);
        button.pvrSurfaceIndex = HelpLayoutHalfword(106);
        button.halfScreenY = HelpLayoutFloat(row) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[15]);
        button.halfScreenX = 56.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(110);
        button.halfScreenY = HelpLayoutFloat(row + 1) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[17]);
        button.pvrSurfaceIndex = HelpLayoutHalfword(98);
        button.halfScreenY = HelpLayoutFloat(row + 1) + 232.0f;
        button.halfScreenX += 80.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[11]);
        button.halfScreenX = 56.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(100);
        button.halfScreenY = HelpLayoutFloat(row + 2) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[12]);
        break;
    }
    case 42:
    case 94:
        button.halfScreenX = 56.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(104);
        button.halfScreenY = HelpLayoutFloat(1) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[14]);
        break;
    case 43:
    case 50:
    case 95:
    case 96: {
        const std::int8_t row = token == 43 || token == 50 ? 4 : 5;
        if (token == 43 || token == 95)
            DrawHelpFrame(1, 2, 0, row, 0, 1);
        else
            DrawHelpFrame(0, 2, 0, row, 0, 0);
        DrawHelpRowIcon(row, 2, 0);
        DrawHelpRowIcon(row, 3, 1);
        button.halfScreenX = g_trialHelpPageToken == 43 || g_trialHelpPageToken == 50 ? 136.0f : 187.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(100);
        button.halfScreenY = HelpLayoutFloat(2) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[12]);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(9, 8, 1);
        break;
    }
    case 45:
    case 46:
    case 47:
    case 48:
    case 52:
    case 53:
    case 54:
    case 55:
        DrawHelpFrame(1, 0, 1, 2, 1, 1);
        DrawHelpFrame(1, 1, 2, 2, 1, 1);
        DrawHelpFrame(0, 3, 2, 5, 1, 0);
        DrawHelpFrame(0, 2, 1, 5, 1, 0);
        DrawHelpRowIcon(2, 0, 0);
        DrawHelpRowIcon(2, 6, 1);
        DrawHelpRowIcon(3, 1, 0);
        DrawHelpRowIcon(3, 7, 1);
        DrawHelpRowIcon(5, 3, 0);
        DrawHelpRowIcon(5, 8, 1);
        break;
    case 49:
    case 97:
        DrawHelpFrame(0, 0, 1, 3, 1, 0);
        DrawHelpFrame(0, 3, 2, 3, 1, 0);
        DrawHelpRowIcon(3, 0, 0);
        DrawHelpRowIcon(3, 8, 1);
        button.scaledWidth = button.scaledHeight = 32;
        button.positionScaleX = button.positionScaleY = std::bit_cast<float>(0x3f999999U);
        button.halfScreenX = 56.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(106);
        button.halfScreenY = HelpLayoutFloat(1) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[15]);
        break;
    case 51:
    case 98:
        DrawHelpFrame(1, 2, 1, 7, 1, 3);
        DrawHelpFrame(0, 3, 2, 9, 1, 0);
        DrawHelpRowIcon(6, 12, 0);
        DrawHelpRowIcon(7, 13, 0);
        DrawHelpRowIcon(8, 2, 0);
        DrawHelpRowIcon(9, 3, 0);
        DrawHelpRowIcon(9, 8, 1);
        button.halfScreenY = 232.0f;
        button.scaledWidth = button.scaledHeight = 32;
        button.positionScaleX = button.positionScaleY = std::bit_cast<float>(0x3f999999U);
        button.angle = 0;
        button.halfScreenX = 152.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(100);
        button.halfScreenY = HelpLayoutFloat(8) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[12]);
        button.pvrSurfaceIndex = HelpLayoutHalfword(102);
        button.halfScreenY = HelpLayoutFloat(9) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[13]);
        button.halfScreenX = g_trialHelpPageToken == 51 ? 181.0f : 226.0f;
        button.pvrSurfaceIndex = HelpLayoutHalfword(96);
        button.halfScreenY = HelpLayoutFloat(6) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10]);
        button.halfScreenY = HelpLayoutFloat(7) + 232.0f;
        UI_HELPDrawColouredIcon(&button, kHelpLayoutWords[10]);
        UI_HELPDrawMarkers();
        break;
    }
}

} // namespace shenmue::s1::ui_runtime_detail
#pragma endregion
#endif
