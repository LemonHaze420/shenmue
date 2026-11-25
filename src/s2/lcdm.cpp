#include "s2/lcdm_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/scene_data.hpp"
#include <cstring>
#include "platform/dc/sdk.hpp"
#include "s2/frame_dc.hpp"
#include "s2/pause.hpp"
#include "s2/scene_assets_dc.hpp"
#include <sg_lcd.h>
extern "C" const std::uint8_t *kdGetDevice(std::uint32_t port);
#include "s2/cold_archive_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"

namespace shenmue {
namespace s2 {

const std::uint8_t *LCDM_VIBRATION_PATTERNS[3] = {LCDM_VIBRATION_STOP, LCDM_VIBRATION_PULSE, LCDM_VIBRATION_RAMP};

void ShutdownLcdmTask();
void LcdmInit(std::uintptr_t image);
void lcdmRenderButtonGlyphs(const std::uint8_t *glyphs);

char g_lcdmSmallDigitName[9] = "BT_S?   ";
char g_lcdmLargeDigitName[9] = "BT_L?   ";
HLib::Task *g_lcdmTask;
LcdmTaskData *g_lcdmTaskData;
std::byte *g_lcdMemory;

#define LCDM_RETURN_IF(condition)                                                                                                                              \
    do {                                                                                                                                                       \
        if (condition)                                                                                                                                         \
            return;                                                                                                                                            \
    } while (0)

// @verified - d
const std::uint8_t *FindLcdmImageData(const void *resource, const char *name) {
    const std::byte *const base = reinterpret_cast<const std::byte *>(resource);
    const LcdmImageDirectory *const directory = reinterpret_cast<const LcdmImageDirectory *>(resource);
    const LcdmImageEntry *entry = reinterpret_cast<const LcdmImageEntry *>(base + directory->tableOffset);
    if (!directory->imageCount)
        return 0;
    std::uint32_t first;
    std::memcpy(&first, name, sizeof(first));
    for (std::uint32_t remaining = directory->imageCount; remaining; --remaining, ++entry) {
        if (entry->name[0] == first) {
            std::uint32_t second;
            std::memcpy(&second, name + 4, sizeof(second));
            if (entry->name[1] == second)
                return reinterpret_cast<const std::uint8_t *>(base + entry->dataOffset);
        }
    }
    return 0;
}

// @verified - d
std::byte *AllocateLCDM() {
    std::byte *const memory = static_cast<std::byte *>(CopyColdArchiveSection(1, false, LCD_MEMORY_STORAGE));
    g_lcdMemory = memory;
    return memory;
}

// @verified - d
void InitializeLcdmTaskData() {
    const std::uint8_t *smallDigits[10], *largeDigits[10], *source;
    std::uint8_t *high, *low;
    std::int32_t digit, row;
    std::uint16_t value, highBits, lowBits;
    LCDM_RETURN_IF(!g_lcdmTaskData);
    std::memset(g_lcdmTaskData, 0, sizeof(LcdmTaskData));
    g_lcdmTaskData->resource = g_lcdMemory;
    const std::byte *resource = g_lcdMemory + 31;
    resource = reinterpret_cast<const std::byte *>(reinterpret_cast<std::uintptr_t>(resource) & ~31U);
    g_lcdmTaskData->width = 48;
    g_lcdmTaskData->height = 32;
    g_lcdmTaskData->images[0] = FindLcdmImageData(resource, "SHENMUE ");
    g_lcdmTaskData->images[1] = FindLcdmImageData(resource, "PK      ");
    g_lcdmTaskData->images[2] = FindLcdmImageData(resource, "LOAD_JP ");
    g_lcdmTaskData->images[3] = FindLcdmImageData(resource, "LOAD_US ");
    g_lcdmTaskData->images[4] = FindLcdmImageData(resource, "SAVE_JP ");
    g_lcdmTaskData->images[5] = FindLcdmImageData(resource, "SAVE_US ");
    g_lcdmTaskData->images[6] = FindLcdmImageData(resource, "OK_JP   ");
    g_lcdmTaskData->images[7] = FindLcdmImageData(resource, "OK_US   ");
    g_lcdmTaskData->images[8] = FindLcdmImageData(resource, "NG_JP   ");
    g_lcdmTaskData->images[9] = FindLcdmImageData(resource, "NG_US   ");
    g_lcdmTaskData->images[10] = FindLcdmImageData(resource, "RSM_JP  ");
    g_lcdmTaskData->images[11] = FindLcdmImageData(resource, "RSM_US  ");
    g_lcdmTaskData->images[12] = FindLcdmImageData(resource, "RSM_OK  ");
    g_lcdmTaskData->images[13] = FindLcdmImageData(resource, "RSM_NG  ");
    g_lcdmTaskData->images[16] = FindLcdmImageData(resource, "BT_START");
    g_lcdmTaskData->images[17] = FindLcdmImageData(resource, "BT_COUNT");
    g_lcdmTaskData->images[18] = FindLcdmImageData(resource, "BT_END  ");
    g_lcdmTaskData->moveGlyphs[0] = FindLcdmImageData(resource, "WAZA_KB ");
    g_lcdmTaskData->moveGlyphs[1] = FindLcdmImageData(resource, "WAZA_KF ");
    g_lcdmTaskData->moveGlyphs[2] = FindLcdmImageData(resource, "WAZA_BA ");
    g_lcdmTaskData->moveGlyphs[3] = FindLcdmImageData(resource, "WAZA_BB ");
    g_lcdmTaskData->moveGlyphs[4] = FindLcdmImageData(resource, "WAZA_BX ");
    g_lcdmTaskData->moveGlyphs[5] = FindLcdmImageData(resource, "WAZA_BY ");
    g_lcdmTaskData->moveGlyphs[6] = FindLcdmImageData(resource, "WAZA_BL ");
    g_lcdmTaskData->moveGlyphs[7] = FindLcdmImageData(resource, "WAZA_BR ");
    g_lcdmTaskData->moveGlyphs[8] = FindLcdmImageData(resource, "WAZA_BP ");
    g_lcdmTaskData->orderedMoveGlyphs[0] = g_lcdmTaskData->moveGlyphs[4];
    g_lcdmTaskData->orderedMoveGlyphs[1] = g_lcdmTaskData->moveGlyphs[2];
    g_lcdmTaskData->orderedMoveGlyphs[2] = g_lcdmTaskData->moveGlyphs[1];
    g_lcdmTaskData->orderedMoveGlyphs[3] = g_lcdmTaskData->moveGlyphs[0];
    g_lcdmTaskData->orderedMoveGlyphs[4] = g_lcdmTaskData->moveGlyphs[6];
    g_lcdmTaskData->orderedMoveGlyphs[5] = g_lcdmTaskData->moveGlyphs[3];
    g_lcdmTaskData->orderedMoveGlyphs[6] = g_lcdmTaskData->moveGlyphs[5];
    g_lcdmTaskData->orderedMoveGlyphs[7] = g_lcdmTaskData->moveGlyphs[8];
    g_lcdmTaskData->moveGlyphIndices[0] = 0;
    g_lcdmTaskData->moveGlyphIndices[1] = 1;
    g_lcdmTaskData->moveGlyphIndices[2] = 2;
    g_lcdmTaskData->moveGlyphIndices[3] = 3;
    g_lcdmTaskData->moveGlyphIndices[4] = 4;
    g_lcdmTaskData->moveGlyphIndices[5] = 5;
    g_lcdmTaskData->moveGlyphIndices[6] = 6;
    g_lcdmTaskData->moveGlyphIndices[7] = 7;
    for (digit = 0; digit < 10; ++digit) {
        g_lcdmSmallDigitName[4] = g_lcdmLargeDigitName[4] = static_cast<char>('0' + digit);
        smallDigits[digit] = FindLcdmImageData(resource, g_lcdmSmallDigitName);
        largeDigits[digit] = FindLcdmImageData(resource, g_lcdmLargeDigitName);
    }
    for (digit = 0; digit < 10; ++digit) {
        source = largeDigits[digit] + 2;
        high = g_lcdmTaskData->largeDigitsHigh[digit];
        low = g_lcdmTaskData->largeDigitsLow[digit];
        for (row = 0; row < 22; ++row) {
            value = static_cast<std::uint16_t>((source[0] << 8) | source[1]);
            source += 2;
            highBits = static_cast<std::uint16_t>(value >> 1);
            lowBits = static_cast<std::uint16_t>(value >> 4);
            *high++ = static_cast<std::uint8_t>(highBits >> 8);
            *high++ = static_cast<std::uint8_t>(highBits);
            *low++ = static_cast<std::uint8_t>(lowBits >> 8);
            *low++ = static_cast<std::uint8_t>(lowBits);
        }
    }
    for (digit = 0; digit < 10; ++digit) {
        source = smallDigits[digit] + 2;
        high = g_lcdmTaskData->smallDigitsHigh[digit];
        low = g_lcdmTaskData->smallDigitsLow[digit];
        for (row = 0; row < 10; ++row) {
            value = static_cast<std::uint16_t>(*source++ << 8);
            highBits = static_cast<std::uint16_t>(value >> 5);
            lowBits = static_cast<std::uint16_t>(value >> 3);
            *high++ = static_cast<std::uint8_t>(highBits >> 8);
            *high++ = static_cast<std::uint8_t>(highBits);
            *low++ = static_cast<std::uint8_t>(lowBits >> 8);
            *low++ = static_cast<std::uint8_t>(lowBits);
        }
    }
    LcdmTaskData *const data = g_lcdmTaskData;
    std::uint32_t *blank = reinterpret_cast<std::uint32_t *>(data->largeDigitsHigh[0]);
    blank[0] = 0;
    blank[1] = 0;
    blank[2] = 0;
    blank[3] = 0;
    blank[4] = 0;
    blank[5] = 0;
    blank[6] = 0;
    blank[7] = 0;
    blank[8] = 0;
    blank[9] = 0;
    blank[10] = 0;
    blank = reinterpret_cast<std::uint32_t *>(data->smallDigitsHigh[0]);
    blank[0] = 0;
    blank[1] = 0;
    blank[2] = 0;
    blank[3] = 0;
    blank[4] = 0;
}

// @verified - d
void FreeLcdmTaskData() {
    LCDM_RETURN_IF(!g_lcdmTaskData);
    LCDM_RETURN_IF(!g_lcdmTaskData->resource);
    HLib::FreeTaskData(g_lcdmTaskData->resource);
    g_lcdmTaskData->resource = 0;
}

// @verified - d
void LcdmCallback(void *) {
    const std::uint32_t port = GetLcdPeripheralPort();
    std::uint8_t packet[4];
    if (shenIsGamePaused() || IsDiscTrayOpen()) {
        if (g_lcdmTaskData->vibrationState > 0)
            g_lcdmTaskData->vibrationState = static_cast<std::int8_t>(-g_lcdmTaskData->vibrationState);
    } else if (g_lcdmTaskData->vibrationState < 0) {
        if (g_lcdmTaskData->vibrationState < -10)
            g_lcdmTaskData->vibrationState = static_cast<std::int8_t>(g_lcdmTaskData->vibrationState + 10);
        if ((g_lcdmTaskData->vibrationState = static_cast<std::int8_t>(-g_lcdmTaskData->vibrationState)) == 3)
            g_lcdmTaskData->vibrationState = 2;
    }
    switch (g_lcdmTaskData->vibrationState) {
    case 1:
        if (*g_lcdmTaskData->vibrationCursor == 0xff) {
            packet[0] = packet[1] = packet[2] = packet[3] = 0;
            if (!dc::sdk::StartVibration(port, packet))
                g_lcdmTaskData->vibrationState = 0;
            break;
        }
        g_lcdmTaskData->vibrationPacket[0] = g_lcdmTaskData->vibrationPacket[2] = *g_lcdmTaskData->vibrationCursor++;
        g_lcdmTaskData->vibrationPacket[1] = g_lcdmTaskData->vibrationPacket[3] = *g_lcdmTaskData->vibrationCursor++;
        g_lcdmTaskData->vibrationFrames = *g_lcdmTaskData->vibrationCursor++;
        ++g_lcdmTaskData->vibrationState;
    case 2:
        packet[0] = g_lcdmTaskData->vibrationPacket[0];
        packet[1] = g_lcdmTaskData->vibrationPacket[1];
        packet[2] = g_lcdmTaskData->vibrationPacket[2];
        packet[3] = g_lcdmTaskData->vibrationPacket[3];
        if (dc::sdk::StartVibration(port, packet) && --g_lcdmTaskData->vibrationFrames > 1)
            break;
        ++g_lcdmTaskData->vibrationState;
        break;
    case 3:
        if (--g_lcdmTaskData->vibrationFrames <= 0)
            g_lcdmTaskData->vibrationState = 1;
        break;
    default:
        if (g_lcdmTaskData->vibrationState > 0) {
            g_lcdmTaskData->vibrationState = 0;
        } else if (g_lcdmTaskData->vibrationState > -10) {
            packet[0] = packet[1] = packet[2] = packet[3] = 0;
            if (!dc::sdk::StartVibration(port, packet))
                g_lcdmTaskData->vibrationState = static_cast<std::int8_t>(g_lcdmTaskData->vibrationState - 10);
        }
        break;
    }
    if (g_lcdmTaskData->displayDirty) {
        std::uint8_t *const pixels = g_lcdmTaskData->pixels;
        const std::uint32_t flags = pdLcdGetDirection(port) == PDD_LCD_DIRECTION_FLIP ? PDD_LCD_FLAG_HVFLIP : PDD_LCD_FLAG_NOFLIP;
        if (!pdVmsLcdWrite(port, pixels, flags))
            g_lcdmTaskData->displayDirty = 0;
    } else if (*kdGetDevice(port) == 0) {
        LcdmInit(0);
    }
}

// @verified - d
std::int32_t initlcdTask() {
    do {
        g_lcdmTaskData = 0;
        g_lcdmTask = HLib::EnqueueTaskWithParameter(LcdmCallback, 2, 2, sizeof(LcdmTaskData), LCD_MEMORY_STORAGE);
    } while (0);
    if (g_lcdmTask) {
        g_lcdmTaskData = static_cast<LcdmTaskData *>(HLib::GetTaskParameter(g_lcdmTask));
        InitializeLcdmTaskData();
        if (g_lcdmTaskData->resource) {
            LcdmInit(0);
            return 1;
        }
        ShutdownLcdmTask();
    }
    return 0;
}

// @verified - d
void ShutdownLcdmTask() {
    FreeLcdmTaskData();
    HLib::DestroyTaskInternal(g_lcdmTask);
    g_lcdmTaskData = 0;
    g_lcdmTask = 0;
}

struct LcdmButtonGlyphList {
    std::int8_t count;
    std::uint8_t glyphs[7];
};

// @verified - d
void lcdmRenderButtonCommands(const std::int32_t *commands) {
    LcdmButtonGlyphList list;
    list.count = 0;
    while (*commands != 8)
        list.glyphs[list.count++] = g_lcdmTaskData->moveGlyphIndices[*commands++];
    lcdmRenderButtonGlyphs(reinterpret_cast<const std::uint8_t *>(&list));
}

// @verified - d
void LcdmSelectMessage(std::uintptr_t message) {
    LCDM_RETURN_IF(!g_lcdmTaskData);
    if (message < 0x10000U)
        g_lcdmTaskData->vibrationCursor = LCDM_VIBRATION_PATTERNS[message];
    else
        g_lcdmTaskData->vibrationCursor = reinterpret_cast<const std::uint8_t *>(message);
    g_lcdmTaskData->vibrationState = 1;
}

// @verified - d
void LcdmInit(std::uintptr_t image) {
    LCDM_RETURN_IF(!g_lcdmTaskData);
    const std::uint8_t *source = (image < 0x10000U ? g_lcdmTaskData->images[image] : reinterpret_cast<const std::uint8_t *>(image)) + 2;
    std::uint8_t *pixel = g_lcdmTaskData->pixels;
    for (std::int32_t byte = 192; byte; --byte, ++source) {
        for (std::int32_t bit = 0; bit < 8; ++bit)
            *pixel++ = *source & (0x80 >> bit) ? 8 : 0;
    }
    g_lcdmTaskData->displayDirty = 1;
}

// @verified - d
void LcdmDisplayBattleStart() {
    LCDM_RETURN_IF(!g_lcdmTaskData);
    LcdmInit(reinterpret_cast<std::uintptr_t>(g_lcdmTaskData->images[16]));
}

// @verified - d
void LcdmDisplayBattleEnd() {
    LCDM_RETURN_IF(!g_lcdmTaskData);
    LcdmInit(reinterpret_cast<std::uintptr_t>(g_lcdmTaskData->images[18]));
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void LcdmComposeBattleValues(std::int32_t smallValue, std::int32_t largeValue) {
    LCDM_RETURN_IF(!g_lcdmTaskData);
    std::uint8_t *const pixels = &g_lcdmTaskData->width + 2;
    std::memcpy(pixels, g_lcdmTaskData->images[17] + 2, 192);
    std::uint8_t *destination;
    std::int32_t row;
    if (smallValue) {
        destination = pixels + 14;
        const std::uint8_t *high = g_lcdmTaskData->smallDigitsHigh[smallValue / 10];
        const std::uint8_t *low = g_lcdmTaskData->smallDigitsLow[smallValue % 10];
        for (row = 0; row < 10; ++row) {
            *destination++ = *high++;
            *destination = *high++;
            *destination++ |= *low++;
            *destination |= *low++;
            destination += 4;
        }
    } else {
        destination = pixels + 34;
        for (row = 0; row < 7; ++row) {
            *destination++ = 0;
            *destination = 0;
            destination += 5;
        }
    }
    destination = pixels + 48;
    const std::uint8_t *high = g_lcdmTaskData->largeDigitsHigh[largeValue / 10];
    const std::uint8_t *low = g_lcdmTaskData->largeDigitsLow[largeValue % 10];
    for (row = 0; row < 22; ++row) {
        *destination++ = *high++;
        *destination = *high++;
        *destination++ |= *low++;
        *destination |= *low++;
        destination += 4;
    }
    LcdmInit(reinterpret_cast<std::uintptr_t>(&g_lcdmTaskData->width));
}
#else
// @verified - d
void LcdmComposeBattleValues(std::int32_t smallValue, std::int32_t largeValue) {
    if (!g_lcdmTaskData)
        return;
    std::uint8_t *const packed = reinterpret_cast<std::uint8_t *>(g_lcdmTaskData) + offsetof(LcdmTaskData, width);
    std::uint8_t *const pixels = packed + 2;
    std::memcpy(pixels, g_lcdmTaskData->images[17] + 2, 192);
    if (smallValue) {
        const std::int32_t tens = smallValue / 10;
        const std::uint8_t *high = g_lcdmTaskData->smallDigitsHigh[tens];
        const std::uint8_t *low = g_lcdmTaskData->smallDigitsLow[smallValue - tens * 10];
        std::uint8_t *destination = pixels + 14;
        for (std::int32_t row = 0; row < 10; ++row, destination += 6) {
            destination[0] = *high++;
            destination[1] = *high++;
            destination[1] |= *low++;
            destination[2] |= *low++;
        }
    } else {
        std::uint8_t *destination = pixels + 34;
        for (std::int32_t row = 0; row < 7; ++row, destination += 6) {
            destination[0] = 0;
            destination[1] = 0;
        }
    }
    const std::int32_t tens = largeValue / 10;
    const std::uint8_t *high = g_lcdmTaskData->largeDigitsHigh[tens];
    const std::uint8_t *low = g_lcdmTaskData->largeDigitsLow[largeValue - tens * 10];
    std::uint8_t *destination = pixels + 48;
    for (std::int32_t row = 0; row < 22; ++row, destination += 6) {
        destination[0] = *high++;
        destination[1] = *high++;
        destination[1] |= *low++;
        destination[2] |= *low++;
    }
    LcdmInit(reinterpret_cast<std::uintptr_t>(g_lcdmTaskData) + offsetof(LcdmTaskData, width));
}
#endif

// @verified - d
void lcdmDrawButtonGlyph(std::int32_t x, std::int32_t y, const std::uint8_t *glyph) {
    std::int32_t width = *glyph++;
    std::int32_t height = *glyph++;
    std::int32_t bytesPerRow = ((width + 7) & ~7) / 8;
    std::uint8_t *pixel = g_lcdmTaskData->pixels + y * 48 + x;
    for (std::int32_t row = height; row; --row) {
        std::int32_t remaining = width;
        for (std::int32_t byte = bytesPerRow; byte; --byte, ++glyph)
            for (std::int32_t bit = 0; remaining && bit < 8; --remaining, ++bit)
                *pixel++ = *glyph & (0x80 >> bit) ? 8 : 0;
        std::uint8_t *next = pixel + 48;
        pixel = next - width;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void lcdmRenderButtonGlyphs(const std::uint8_t *glyphs) {
    if (!g_lcdmTaskData)
        return;
    const std::uint8_t *images[6];
    std::int32_t remaining;
    const std::uint8_t *glyph = glyphs + 1;
    const std::uint8_t **image = images;
    for (remaining = static_cast<std::int8_t>(*glyphs); remaining; --remaining)
        *image++ = g_lcdmTaskData->orderedMoveGlyphs[*glyph++];
    std::uint32_t *pixel = reinterpret_cast<std::uint32_t *>(g_lcdmTaskData->pixels);
    for (remaining = sizeof(g_lcdmTaskData->pixels) / 4; remaining; --remaining)
        *pixel++ = 0;
    const std::int32_t count = static_cast<std::int8_t>(*glyphs);
    if (count <= 4) {
        std::int32_t x = LCDM_GLYPH_START_X[count];
        const std::int32_t y = LCDM_GLYPH_ROW_Y[1];
        image = images;
        for (remaining = count; remaining; --remaining, x += 11)
            lcdmDrawButtonGlyph(x, y, *image++);
    } else {
        std::int32_t firstRow = 2;
        std::int32_t x = LCDM_GLYPH_START_X[3];
        std::int32_t y = LCDM_GLYPH_ROW_Y[2];
        image = images;
        remaining = firstRow;
        do {
            lcdmDrawButtonGlyph(x, y, *image++);
            x += 11;
        } while (--remaining);
        firstRow = static_cast<std::int8_t>(*glyphs) - firstRow;
        x = LCDM_GLYPH_START_X[firstRow];
        y += 15;
        for (remaining = firstRow; remaining; --remaining, x += 11)
            lcdmDrawButtonGlyph(x, y, *image++);
    }
    g_lcdmTaskData->displayDirty = 1;
}
#else
// @verified - d
void lcdmRenderButtonGlyphs(const std::uint8_t *glyphs) {
    if (!g_lcdmTaskData)
        return;
    const std::uint8_t *images[6];
    const std::uint8_t *glyph = glyphs + 1;
    const std::uint8_t **image = images;
    for (std::int32_t remaining = static_cast<std::int8_t>(*glyphs); remaining; --remaining)
        *image++ = g_lcdmTaskData->orderedMoveGlyphs[*glyph++];
    std::memset(g_lcdmTaskData->pixels, 0, sizeof(g_lcdmTaskData->pixels));
    const std::int32_t count = static_cast<std::int8_t>(*glyphs);
    image = images;
    if (count <= 4) {
        std::int32_t x = LCDM_GLYPH_START_X[count];
        for (std::int32_t remaining = count; remaining; --remaining, x += 11)
            lcdmDrawButtonGlyph(x, LCDM_GLYPH_ROW_Y[1], *image++);
    } else {
        std::int32_t x = LCDM_GLYPH_START_X[3];
        for (std::int32_t remaining = 2; remaining; --remaining, x += 11)
            lcdmDrawButtonGlyph(x, LCDM_GLYPH_ROW_Y[2], *image++);
        const std::int32_t remainingCount = static_cast<std::int8_t>(*glyphs) - 2;
        x = LCDM_GLYPH_START_X[remainingCount];
        for (std::int32_t remaining = remainingCount; remaining; --remaining, x += 11)
            lcdmDrawButtonGlyph(x, LCDM_GLYPH_ROW_Y[2] + 15, *image++);
    }
    g_lcdmTaskData->displayDirty = 1;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
