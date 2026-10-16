#include "s1/task/warn.hpp"

#include "s1/boot.hpp"
#include "s1/font.hpp"
#include "s1/screen_space.hpp"
#include "s1/task/wind.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

std::int32_t g_warningState = 0;
std::int32_t g_warningMessageIndex = 0;
std::int32_t g_warningChoiceOrDisc = 0;
const char *g_warningMessage = nullptr;
std::uint32_t g_warningShowColour = 0;
std::uint32_t g_warningTargetColour = 0;
std::uint32_t g_warningPackedColour = 0;
// The longest line's last column while a message is laid out, then the lines' left edge.
std::int32_t g_warningLineBaseX = 0;
std::int16_t g_warningSurfaces[10]{};
std::int16_t g_warningLineWidths[10]{};
// While a message is laid out each panel holds its first column, first line, last column and last line; then its offset, size in pixels.
std::int16_t g_warningPanelX[3]{};
std::int16_t g_warningPanelY[3]{};
std::int16_t g_warningPanelWidth[3]{};
std::int16_t g_warningPanelHeight[3]{};

namespace {

char g_warningLineBuffer[52]{};
char *g_warningLineCursor = nullptr;
std::int16_t *g_warningSurfaceCursor = nullptr;
std::int16_t *g_warningLineWidthCursor = nullptr;
std::int16_t g_warningLineIndex = 0;
std::int16_t g_warningActivePanel = 0;
std::int32_t g_warningReleaseIndex = 0;
std::int32_t g_warningPulseFrame = 0;
std::int32_t g_warningTailFrames = 0;


void FreePVR_0(std::int32_t first, std::int32_t last) {
    for (std::int32_t index = first; index < last; ++index) {
        if (g_warningSurfaces[index] >= 0)
            PVR_FreeAllocatedSurfaceByIndex(g_warningSurfaces[index]);
        g_warningSurfaces[index] = -1;
    }
}



void WARN_LayoutMessage() {
    const char *cursor = g_warningMessage;
    char character = *cursor;
    if (!character)
        return;
    do {
        ++cursor;
        if (character == '%') {
            *g_warningLineCursor = 0;
            const std::int16_t lastColumn = static_cast<std::int16_t>(std::strlen(g_warningLineBuffer) - 1);
            if (lastColumn >= 0) {
                if (g_warningLineBaseX < lastColumn)
                    g_warningLineBaseX = lastColumn;
                const std::int16_t width = static_cast<std::int16_t>(16 * kWarningLineWidthBuckets[lastColumn]);
                *g_warningLineWidthCursor = width;
                const std::uint32_t largestFree = static_cast<std::uint32_t>(HLib_GetLargestMallocFreeBlock());
                if (!largestFree || largestFree > static_cast<std::uint32_t>(32 * width))
                    *g_warningSurfaceCursor = AllocatePVRSurface(g_warningLineBuffer, width, 0xffff, 0xf000, 0, false);
            }
            if (g_warningActivePanel >= 0) {
                g_warningPanelX[g_warningActivePanel] = 0;
                if (g_warningPanelWidth[g_warningActivePanel] < lastColumn)
                    g_warningPanelWidth[g_warningActivePanel] = lastColumn;
                g_warningPanelHeight[g_warningActivePanel] = g_warningLineIndex;
            }
            ++g_warningSurfaceCursor;
            g_warningLineCursor = g_warningLineBuffer;
            ++g_warningLineWidthCursor;
            ++g_warningLineIndex;
        } else if (character == '@') {
            const std::int8_t control = static_cast<std::int8_t>(*cursor++ - '1');
            *g_warningLineCursor = 0;
            const std::size_t length = std::strlen(g_warningLineBuffer);
            const std::int32_t panel = control / 2;
            if ((control & 1) != 0) {
                const std::int16_t lastColumn = static_cast<std::int16_t>(length - 1);
                if (g_warningPanelWidth[panel] < lastColumn)
                    g_warningPanelWidth[panel] = lastColumn;
                g_warningActivePanel = -1;
                g_warningPanelHeight[panel] = g_warningLineIndex;
            } else {
                g_warningActivePanel = static_cast<std::int16_t>(panel);
                g_warningPanelWidth[panel] = static_cast<std::int16_t>(length);
                g_warningPanelX[panel] = static_cast<std::int16_t>(length);
                g_warningPanelHeight[panel] = g_warningLineIndex;
                g_warningPanelY[panel] = g_warningLineIndex;
            }
        } else {
            if (character == '^')
                character = static_cast<char>(g_warningChoiceOrDisc + '0');
            *g_warningLineCursor++ = character;
        }
        character = *cursor;
    } while (character);
}


std::uint32_t WARN_BlendPackedColour(std::int32_t weight, std::uint32_t first, std::uint32_t second) {
    const std::uint32_t amount = static_cast<std::uint32_t>(weight < 0 ? 0 : weight > 16 ? 16 : weight);
    std::uint32_t result = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        const std::uint32_t from = (first >> shift) & 0xffU;
        const std::uint32_t to = (second >> shift) & 0xffU;
        const std::uint32_t channel = from <= to ? from + ((amount * (to - from)) >> 4) : to + (((16U - amount) * (from - to)) >> 4);
        result |= channel << shift;
    }
    return result;
}


void AdjustScreenSpaceCentered(std::int32_t panel, std::uint32_t fillColour, std::uint32_t borderColour) {
    const std::int16_t width = static_cast<std::int16_t>(g_warningPanelWidth[panel] + 8);
    const std::int16_t height = static_cast<std::int16_t>(g_warningPanelHeight[panel] + 1);
    const std::int16_t top = static_cast<std::int16_t>(g_warningPanelY[panel] + 43);
    ScreenSpaceFillParams params{.halfScreenX = static_cast<float>(static_cast<std::int16_t>(g_warningPanelX[panel] - 5 + g_warningLineBaseX)),
        .halfScreenY = static_cast<float>(top),
        .positionScaleX = 1.0f,
        .positionScaleY = 1.0f,
        .maxExtent = 7.0f,
        .colour = fillColour,
        .scaledWidth = width,
        .scaledHeight = height,
        .angle = 0,
        .alignmentType = 0};
    AdjustScreenSpaceWithRenderFlags(&params, 1);
    params.scaledWidth = static_cast<std::int16_t>(params.scaledWidth + 2);
    params.colour = borderColour;
    params.scaledHeight = 2;
    params.halfScreenX = params.halfScreenX + -1.0f;
    params.halfScreenY = params.halfScreenY + -1.0f;
    AdjustScreenSpaceWithRenderFlags(&params, 1);
    params.halfScreenY = params.halfScreenY + static_cast<float>(height);
    AdjustScreenSpaceWithRenderFlags(&params, 1);
    params.scaledWidth = 2;
    params.scaledHeight = static_cast<std::int16_t>(height + 2);
    params.halfScreenY = static_cast<float>(top - 1);
    AdjustScreenSpaceWithRenderFlags(&params, 1);
    params.halfScreenX = params.halfScreenX + static_cast<float>(width);
    AdjustScreenSpaceWithRenderFlags(&params, 1);
}



void WARN_DrawMessage() {
    if (!g_updateRenderFrameMode)
        return;
    g_warningPulseFrame = (g_warningPulseFrame + 1) & 0x1f;
    std::int32_t pulse = g_warningPulseFrame;
    if ((pulse & 0x10) != 0)
        pulse = 32 - pulse;
    const std::int32_t weight = pulse + pulse / 2;
    ScreenSpaceParams params{.halfScreenX = static_cast<float>(g_warningLineBaseX),
        .halfScreenY = 48.0f,
        .positionScaleX = 1.0f,
        .positionScaleY = 1.0f,
        .depthOrScale = 8.0f,
        .flags14 = 0,
        .angle = 0,
        .scaledHeight = 32,
        .alignmentType = 0};
    for (std::int32_t line = 0; line < 10; ++line) {
        params.scaledWidth = g_warningLineWidths[line];
        params.pvrSurfaceIndex = static_cast<std::int16_t>(g_warningSurfaces[line]);
        if (g_warningSurfaces[line] >= 0)
            AdjustScreenSpaceWithParams_0(&params, 1);
        params.halfScreenY = static_cast<float>(static_cast<double>(params.halfScreenY) + 40.0);
    }
    if (g_warningPanelWidth[0] > 0)
        AdjustScreenSpaceCentered(0, 0x3b00008cU, 0xff4040c0U);
    const std::int32_t selected = 2 - (g_warningChoiceOrDisc != 0);
    if (g_warningPanelWidth[selected] > 0)
        AdjustScreenSpaceCentered(selected, 0x3b00008cU, 0xff4040c0U);
    const std::int32_t other = 3 - selected;
    if (g_warningPanelWidth[other] > 0) {
        const std::uint32_t border = WARN_BlendPackedColour(weight - 8, 0xffc04040U, 0xff602020U);
        AdjustScreenSpaceCentered(other, WARN_BlendPackedColour(weight - 8, 0x3b8c0000U, 0x3b460000U), border);
    }
}

} // namespace



void WARN_Callback(HLib::TaskData *) {
    if (g_warningState != 0) {
        switch (g_warningState) {
        case 1:
            for (std::int16_t &surface : g_warningSurfaces) {
                if (surface >= 0)
                    PVR_FreeAllocatedSurfaceByIndex(surface);
                surface = -1;
            }
            g_warningSurfaceCursor = g_warningSurfaces;
            g_warningLineCursor = g_warningLineBuffer;
            g_warningLineBaseX = 0;
            g_warningLineWidthCursor = g_warningLineWidths;
            for (std::int32_t panel = 0; panel < 3; ++panel) {
                g_warningPanelWidth[panel] = -99;
                g_warningPanelHeight[panel] = -99;
                g_warningPanelX[panel] = 99;
                g_warningPanelY[panel] = 99;
            }
            g_warningLineIndex = 0;
            g_warningActivePanel = -1;
            WARN_LayoutMessage();
            g_warningLineBaseX = (620 - 14 * g_warningLineBaseX) / 2;
            for (std::int32_t panel = 0; panel < 3; ++panel) {
                const std::int16_t lines = static_cast<std::int16_t>(g_warningPanelHeight[panel] - g_warningPanelY[panel] + 1);
                const std::int16_t columns = static_cast<std::int16_t>(g_warningPanelWidth[panel] - g_warningPanelX[panel] + 1);
                g_warningPanelX[panel] = static_cast<std::int16_t>(14 * g_warningPanelX[panel]);
                g_warningPanelWidth[panel] = static_cast<std::int16_t>(14 * columns);
                g_warningPanelY[panel] = static_cast<std::int16_t>(40 * g_warningPanelY[panel]);
                g_warningPanelHeight[panel] = static_cast<std::int16_t>(40 * lines);
            }
            g_warningReleaseIndex = 0;
            g_warningPulseFrame = 0;
            g_warningState = 2;
            break;
        case 2:
            if (g_warningMessageIndex <= 0) {
                g_warningState = 3;
                g_warningTargetColour = 0;
                g_warningTailFrames = 4;
            } else {
                WARN_DrawMessage();
            }
            break;
        case 3: {
            if (--g_warningTailFrames < 0)
                g_warningState = 0;
            const std::int32_t last = g_warningReleaseIndex + 5 > 10 ? 10 : g_warningReleaseIndex + 5;
            FreePVR_0(g_warningReleaseIndex, last);
            g_warningReleaseIndex = last;
            break;
        }
        default:
            break;
        }
    } else if (g_warningMessageIndex > 0) {
        LoadFont("DC_KANJI.FON");
        auto *const wind = reinterpret_cast<WindTaskData *>(HLib_GetTaskDataForTask(g_windTask));
        g_warningState = 1;
        wind->tailWord = 1;
        g_warningMessage = kLegacySavingMessages[g_warningMessageIndex - 1];
        g_warningTargetColour = g_warningShowColour;
    }

    if (g_warningPackedColour <= g_warningTargetColour) {
        const std::uint32_t step = g_warningTargetColour - g_warningPackedColour;
        g_warningPackedColour += step > 0x8000000U ? 0x8000000U : step;
    } else {
        const std::uint32_t step = g_warningPackedColour - g_warningTargetColour;
        g_warningPackedColour -= step > 0x8000000U ? 0x8000000U : step;
    }
    if (g_warningPackedColour)
        DrawMiddleOfScreenStretchedAdjusted(g_warningPackedColour, 6.0f);
}


void WRN_CtrlWarning_Enqueue() {
    g_warningState = 0;
    g_warningMessageIndex = 0;
    for (std::int16_t &surface : g_warningSurfaces)
        surface = -1;
    HLib_EnqueueTaskWithoutParameter(WARN_Callback, 15, 0, SHEN_NAME(WARN));
    g_warningShowColour = kWarningShowColour;
    g_warningTargetColour = 0;
    g_warningPackedColour = 0;
}

} // namespace shenmue::s1
