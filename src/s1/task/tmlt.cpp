#include "s1/task/tmlt.hpp"

#include "s1/boot.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/atck.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t volume);
extern void *g_pFont;

HLib::Task *g_TMLT_Task = nullptr;
std::int16_t g_tmp_tmlt_textures_initialised = 0;
std::array<std::int32_t, 10> g_tmltDigitPVRs{};
std::int32_t g_tmltDotPVR0 = 0;
std::int32_t g_tmltDotPVR1 = 0;
std::int16_t g_BeepIntervalFrames = 0;
std::int16_t g_BeepFrameCounter = 0;
std::uint8_t g_tmlt_Active = 0;
std::int16_t g_FrameCounter30 = 0;
std::int32_t g_curr_time_left = 0;

namespace {

constexpr std::int32_t kTmltAuxTextureKey = static_cast<std::int32_t>(SHEN_NAME(TMLT));
constexpr std::int32_t kSpriteAllocationToken = static_cast<std::int32_t>(SHEN_NAME(LDTD));
constexpr std::uint32_t kTimeLimitWarningCommand = 0x000d00ab;


constexpr std::array<std::int16_t, 11> kBeepIntervalBySeconds{2, 2, 5, 7, 9, 13, 17, 21, 24, 27, 30};

struct FontStorage {
    const std::uint8_t *kana;
    const std::uint8_t *kanji;
    const std::uint8_t *ryou;
};

constexpr std::array<std::uint8_t, 8> kGlyphBitMasks{1, 2, 4, 8, 16, 32, 64, 128};
std::uint32_t g_tmltAuxiliaryGeneration = 0;

const std::uint8_t *TmltGlyphSource(const FontStorage &font, std::uint16_t glyphCode) {
    if ((glyphCode & 0x8080U) == 0x8080U || (glyphCode & 0xff00U) == 0x0100U) {
        return font.kana ? font.kana + 30U * static_cast<std::uint8_t>(glyphCode) : nullptr;
    }
    const std::int32_t glyphIndex = 94 * static_cast<std::uint8_t>(glyphCode >> 8) + static_cast<std::uint8_t>(glyphCode) - 3135;
    return font.kanji ? font.kanji + 45 * glyphIndex : nullptr;
}

std::uint32_t ExpandTmltPaletteColour(std::uint16_t colour) {
    std::uint32_t expanded = colour >> 12;
    expanded = (expanded << 8U) + ((colour >> 8U) & 0x0fU);
    expanded = (expanded << 8U) + ((colour >> 4U) & 0x0fU);
    expanded = (expanded << 8U) + (colour & 0x0fU);
    return expanded * 17U;
}

void RasterizeTmltAuxiliaryGlyph(const FontStorage &font, std::uint32_t *pixels) {
    constexpr std::uint16_t kGlyphCode = 0x012e;
    constexpr std::array<std::uint16_t, 4> kPalette{0, 0xffff, 0xf000, 0xf000};
    const std::uint8_t *const source = TmltGlyphSource(font, kGlyphCode);
    if (!source)
        return;

    std::array<std::uint8_t, 25 * 16> coverage{};
    for (std::int32_t sourcePixel = 0; sourcePixel < 240; ++sourcePixel) {
        if ((source[sourcePixel >> 3] & kGlyphBitMasks[sourcePixel & 7]) == 0) {
            continue;
        }
        const std::int32_t x = sourcePixel & 0x0f;
        const std::int32_t y = sourcePixel >> 4;
        coverage[25 * y + x] |= 2;
        coverage[25 * y + x + 1] |= 1;
        coverage[25 * (y + 1) + x] |= 1;
        coverage[25 * (y + 1) + x + 1] |= 1;
    }

    constexpr std::int32_t kWidth = 64;
    constexpr std::int32_t kStartX = (kWidth - 12) / 2;
    for (std::int32_t sourceY = 0; sourceY < 16; ++sourceY) {
        std::uint32_t *const destination = pixels + (15 - sourceY) * kWidth + kStartX;
        const std::uint8_t *const sourceRow = coverage.data() + 25 * sourceY;
        for (std::int32_t x = 0; x < 13; ++x) {
            destination[x] = ExpandTmltPaletteColour(kPalette[sourceRow[x]]);
        }
    }
}

std::int32_t AllocateTmltAuxiliarySurface(std::size_t index, std::uint32_t generation) {
    constexpr std::int32_t kWidth = 64;
    constexpr std::int32_t kHeight = 16;
    constexpr std::size_t kPixelBytes = kWidth * kHeight * sizeof(std::uint32_t);
    auto *const font = static_cast<FontStorage *>(g_pFont);
    if (!font)
        return -1;
    auto *const pixels = static_cast<std::uint32_t *>(HLib_AllocHeapMemBlock(kPixelBytes, SHEN_NAME(TMLP)));
    if (!pixels)
        return -1;
    std::memset(pixels, 0, kPixelBytes);
    RasterizeTmltAuxiliaryGlyph(*font, pixels);

    std::int32_t surface = -1;
    if (g_pFirstTaskData) {
        const std::intptr_t offset = reinterpret_cast<std::intptr_t>(pixels) - reinterpret_cast<std::intptr_t>(g_pFirstTaskData) - 0x40;
        if (offset >= (std::numeric_limits<std::int32_t>::min)() && offset <= (std::numeric_limits<std::int32_t>::max)() && offset != 0) {
            const std::int32_t high = static_cast<std::int32_t>((generation << 1U) | static_cast<std::uint32_t>(index));
            surface = PVR_FindOrAllocateSurface(kTmltAuxTextureKey, high);
            if (surface >= 0) {
                const TextureUploadDescription upload{kWidth, kHeight, 0x8089, static_cast<std::int32_t>(offset)};
                if (d3t_UploadTexture2DToGPU(surface, &upload, pixels) < 0) {
                    PVR_FreeAllocatedSurfaceByIndex(surface);
                    surface = -1;
                }
            }
        }
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(pixels), 0);
    return surface;
}

void LoadDigitSurfaces() {
    constexpr const char *kDirectory = "sprite";
    constexpr const char *kFilename = "SWLDNUM.SPR";
    const std::uint64_t fileSize64 = shenOpenFile(kDirectory, kFilename);
    if (!fileSize64 || fileSize64 > static_cast<std::uint64_t>((std::numeric_limits<std::int32_t>::max)())) {
        return;
    }

    void *const file = ReadFile(kDirectory, kFilename, kSpriteAllocationToken);
    if (!file)
        return;

    auto *const bytes = static_cast<std::byte *>(file);
    const std::size_t fileSize = static_cast<std::size_t>(fileSize64);
    std::size_t offset = 0;
    std::size_t surfaceIndex = 0;
    while (offset + 8 <= fileSize && surfaceIndex < g_tmltDigitPVRs.size()) {
        std::uint32_t token = 0;
        std::uint32_t chunkSize = 0;
        std::memcpy(&token, bytes + offset, sizeof(token));
        std::memcpy(&chunkSize, bytes + offset + 4, sizeof(chunkSize));
        if (!token || chunkSize < 8 || chunkSize > fileSize - offset)
            break;

        g_tmltDigitPVRs[surfaceIndex++] = LoadTEXN(bytes + offset);
        offset += chunkSize;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
}



void TimeLimitHUD_Update() {
    if (!g_updateRenderFrameMode || !g_tmp_tmlt_textures_initialised || !g_pATCK)
        return;

    ATCK_AssignNewDialPreset(1);
    ATCK_RaceHUDWriteNumberToStrip(36, 100 * (g_curr_time_left + 40 * (g_curr_time_left / 60)));
}

} // namespace

void InitTMLT(std::int16_t texturesInitialised) {
    g_tmp_tmlt_textures_initialised = texturesInitialised;
    if (!texturesInitialised) {
        LoadDigitSurfaces();
        const std::uint32_t generation = g_tmltAuxiliaryGeneration++;
        g_tmltDotPVR0 = AllocateTmltAuxiliarySurface(0, generation);
        g_tmltDotPVR1 = AllocateTmltAuxiliarySurface(1, generation);
    }
    g_TMLT_Task = HLib_EnqueueTaskWithoutParameter(TMLT_Callback, 6, 0, static_cast<std::int32_t>(TASK_TMLT.name));
    g_BeepIntervalFrames = 30;
    g_BeepFrameCounter = 30;
    g_tmlt_Active = 0;
    g_FrameCounter30 = 0;
}

void FreeTMLT() {
    if (!g_TMLT_Task)
        return;

    if (!g_tmp_tmlt_textures_initialised) {
        for (const std::int32_t pvr : g_tmltDigitPVRs)
            PVR_FreeAllocatedSurfaceByIndex(pvr);
        PVR_FreeAllocatedSurfaceByIndex(g_tmltDotPVR0);
        PVR_FreeAllocatedSurfaceByIndex(g_tmltDotPVR1);
    }
    HLib_DestroyTask(g_TMLT_Task);
    g_TMLT_Task = nullptr;
    g_tmp_tmlt_textures_initialised = 0;
}

void TMLT_Callback(HLib::TaskData *) {
    if (!g_tmlt_Active)
        return;

    if (++g_FrameCounter30 >= 30) {
        std::int32_t timeLeft = g_curr_time_left - 1;
        g_FrameCounter30 = 0;
        if (timeLeft <= 0)
            timeLeft = 0;
        g_curr_time_left = timeLeft;


        if (timeLeft < static_cast<std::int32_t>(kBeepIntervalBySeconds.size()))
            g_BeepIntervalFrames = kBeepIntervalBySeconds[static_cast<std::size_t>(timeLeft)];
    }

    TimeLimitHUD_Update();

    if (g_curr_time_left <= 10 && ++g_BeepFrameCounter >= g_BeepIntervalFrames) {
        g_BeepFrameCounter = 0;
        if (g_curr_time_left > 0)
            shenExecuteAICACommand(kTimeLimitWarningCommand, 0, 0);
    }
}

} // namespace shenmue::s1
