#include "s2/weather_wind_dc.hpp"
#include "s2/legacy_sprite_types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#include "s2/window.hpp"
#include "s2/scenario_subtitles_dc.hpp"
#include "s2/font.hpp"
#include "s2/generated_surface_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

void SetWindScaleDenominator(WeatherWind *wind, float denominator);
void SetWindPhaseTransition(WeatherWind *wind, std::int32_t phase, std::int32_t frames);
void SetWindSpeedTransition(WeatherWind *wind, float speed, std::int32_t frames);
float SampleWindImpulse();
void WindowTaskEntry_Callback(void *parameter);
void UpdateWindowSlot(WindowSlot *window, WindowTaskData *owner);
void DrawWindowMessageSprites(WindowSlot *window);
void UpdateWindowRow(WindowSlot *window, WindowTaskData *, std::int32_t row, std::int32_t line);
void ClearWindowText(WindowSlot *window);
void RasterizeWindowGlyphShadow(WindowSlot *window, FontRuntimeData *font, std::uint16_t glyph, std::int16_t column, std::int16_t row);
void UploadWindowShadowSurface(WindowSlot *window, GeneratedTextureSurface *surface, std::uint16_t foreground, std::uint16_t shadow);
void ClearWindowShadowPixels(WindowSlot *window);
void KanjiFontLoad(const char *name);
void RyouFontLoad(const char *name);
void RasterizeWindowGlyph4bpp(FontRuntimeData *font, std::uint16_t glyph, std::byte *destination, std::int32_t pixelX);
void GenerateWeibullProfile(float *samples, std::uint32_t count, float shape, float scale);
void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps);
float SampleWindFromProfile(float baseSpeed, const float *profile);

void GenerateWeibullProfile(float *samples, std::uint32_t count, float shape, float scale);
void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps);
float SampleWindFromProfile(float baseSpeed, const float *profile);
void GenerateWeibullProfile(float *samples, std::uint32_t count, float shape, float scale);
void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps);
float SampleWindFromProfile(float baseSpeed, const float *profile);
HLib::Task *windowTask;

namespace {

FontRuntimeData *fontRuntimeData;

} // namespace

// @verified - d
void BuildWindMappingTable(float *table) {
    for (std::int32_t column = 0; column < 9; ++column) {
        std::int32_t sector = 0;
        float cumulative = 0.0f;
        for (; sector < 16; ++sector) {
            cumulative += windDirectionWeights[sector * 9 + column];
            table[sector * 9 + column] = cumulative;
        }
    }
}

// @verified - d
std::int32_t MapWindToImpulseIndex(float sampledSpeed, const float *table) {
    const float scaled = sampledSpeed * 30.0f;
    std::int32_t column;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    column = static_cast<std::int32_t>(scaled);
#else
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(column) : "f"(scaled) : "fpul");
#endif
    if (column > 8)
        column = 8;
    const float target = dc::sdk::RandomFloat() * table[135 + column];
    std::int32_t sector = 0;
    while (sector < 16 && !(target < table[sector * 9 + column]))
        ++sector;
    return (sector - 1) * 0x1000;
}

// @verified - d
void InitWeibullResources() {
    windProfileSamples = static_cast<float *>(HLib::AllocateTaskData(0xc0, WEATHER_WIND_PROFILE_STORAGE));
    GenerateWeibullProfile(windProfileSamples, 48, 1.78f, 4.8f);
    std::int32_t index = 0;
    float total = 0.0f;
    for (; index < 48; ++index)
        total += windProfileSamples[index];
    totalWindProfileWeight = total;
    windMappingTable = static_cast<float *>(HLib::AllocateTaskData(0x240, WEATHER_WIND_MAPPING_STORAGE));
    BuildWindMappingTable(windMappingTable);
    windImpulseLut = static_cast<float *>(HLib::AllocateTaskData(0x400, WEATHER_WIND_IMPULSE_STORAGE));
    AllocateTMP0_TMP1(windImpulseLut, 256, 13);
    windImpulseLut[0] = 0.0f;
    windImpulseLutIndex = 0;
}

void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);

// @verified - d
WeatherWind *AllocateWIND() {
    WeatherWind *const wind = static_cast<WeatherWind *>(HLib::AllocateTaskData(sizeof(WeatherWind), WEATHER_WIND_STORAGE));
    if (!wind)
        return 0;
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(wind), reinterpret_cast<std::uint32_t *>(wind) + 14);
    SetWindScaleDenominator(wind, 55.65140152f);
    return wind;
}

// @verified - d
void FreeWIND(WeatherWind *wind) {
    if (wind)
        HLib::FreeTaskData(wind);
}

// @verified - d
void ResetWind(WeatherWind *wind) {
    if (!wind)
        return;
    wind->outputPhase = wind->targetPhase = wind->requestedPhase = 0;
    wind->phaseFramesRemaining = 0;
    wind->sampledWindSpeed = wind->currentWindSpeed = wind->requestedWindSpeed = 0.0f;
    wind->cooldownFramesRemaining = 0;
    wind->windDirection.x = 0.0f;
    wind->windDirection.y = 0.0f;
    wind->windDirection.z = 0.0f;
}

// @verified - d
void UpdateWindWithSpeed(WeatherWind *wind) {
    if (!wind)
        return;
    if (wind->requestedPhase != wind->targetPhase) {
        if (wind->phaseFramesRemaining <= 0) {
            wind->targetPhase = wind->requestedPhase;
        } else {
            --wind->phaseFramesRemaining;
            wind->targetPhase = static_cast<std::int32_t>(static_cast<std::uint32_t>(wind->requestedPhase) -
                static_cast<std::uint32_t>(wind->phaseStep) * static_cast<std::uint32_t>(wind->phaseFramesRemaining));
        }
    }
    if (wind->requestedWindSpeed != wind->currentWindSpeed) {
        if (wind->windSpeedFramesRemaining <= 0) {
            wind->currentWindSpeed = wind->requestedWindSpeed;
        } else {
            --wind->windSpeedFramesRemaining;
            wind->currentWindSpeed = wind->requestedWindSpeed - wind->windSpeedStep * static_cast<float>(wind->windSpeedFramesRemaining);
        }
    }
    if (wind->cooldownFramesRemaining <= 0) {
        const float sampled = SampleWindFromProfile(wind->currentWindSpeed, windProfileSamples);
        std::int32_t impulse = MapWindToImpulseIndex(sampled, windMappingTable);
        const float hold = SampleWindImpulse() * 1600.0f;
        std::int32_t frames;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        frames = static_cast<std::int32_t>(hold);
#else
        asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(frames) : "f"(hold) : "fpul");
#endif
        frames -= (impulse > 0x7fff ? impulse ^ 0xffff : impulse) / 1024;
        impulse = (impulse + wind->targetPhase) & 0xffff;
        wind->outputPhase = impulse;
        wind->sampledWindSpeed = sampled;
        wind->cooldownFramesRemaining = frames;
    } else {
        --wind->cooldownFramesRemaining;
    }
    const float scale = wind->sampledWindSpeed / wind->windScaleDenominator;
    wind->windDirection.x = (-dc::sdk::SinFixed(wind->outputPhase) * scale) * 0.5f;
    wind->windDirection.z = (dc::sdk::CosFixed(wind->outputPhase) * scale) * 0.5f;
}

void GetWindDirection(const WeatherWind *wind, Vector3 *direction) {
    *direction = wind->windDirection;
}

std::int32_t GetWindTargetPhase(const WeatherWind *wind) {
    if (!wind)
        return 0;
    return wind->targetPhase;
}

float GetCurrentWindSpeed(const WeatherWind *wind) {
    return wind ? wind->currentWindSpeed : 0.0f;
}

// @verified - d
std::int32_t GetCurrentWindPhaseAccumulator(const WeatherWind *wind) {
    if (!wind)
        return 0;
    return wind->outputPhase;
}

// @verified - d
float GetEffectiveWindSpeed(const WeatherWind *wind) {
    return wind ? wind->sampledWindSpeed : 0.0f;
}

// @verified - d
void SetWindScaleDenominator(WeatherWind *wind, float denominator) {
    if (wind)
        wind->windScaleDenominator = denominator;
}

// @verified - d
void BeginWindPhaseTransition(WeatherWind *wind, std::int32_t phase) {
    SetWindPhaseTransition(wind, phase, 10);
    wind->cooldownFramesRemaining = 0;
}

// @verified - d
void BeginWindSpeedTransition(WeatherWind *wind, float speed) {
    SetWindSpeedTransition(wind, speed, 1);
    wind->cooldownFramesRemaining = 0;
}

// @verified - d
void SetWindPhaseTransition(WeatherWind *wind, std::int32_t phase, std::int32_t frames) {
    if (!wind)
        return;
    wind->requestedPhase = phase;
    wind->phaseFramesRemaining = frames;
    const std::int32_t difference = static_cast<std::int32_t>(static_cast<std::uint32_t>(phase) - static_cast<std::uint32_t>(wind->targetPhase));
    wind->phaseStep = difference / frames;
}

// @verified - d
void SetWindSpeedTransition(WeatherWind *wind, float speed, std::int32_t frames) {
    if (!wind)
        return;
    wind->requestedWindSpeed = speed;
    wind->windSpeedFramesRemaining = frames;
    wind->windSpeedStep = (speed - wind->currentWindSpeed) / static_cast<float>(frames);
}

// @verified - d
float SampleWindImpulse() {
    const float value = windImpulseLut[windImpulseLutIndex++];
    if (windImpulseLutIndex > 255)
        windImpulseLutIndex = 0;
    return value;
}

void ConvertShiftJisTextToEuc(std::uint8_t *text) {
    while (*text) {
        const std::uint8_t lead = *text++;
        if (IsScenarioShiftJisLeadByte(lead)) {
            const std::uint8_t trail = *text++;
            const std::uint16_t character = ConvertWindowShiftJisCharacter((lead << 8) | trail) | 0x8080U;
            text[-2] = character >> 8;
            text[-1] = character;
        }
    }
}

// @verified - d
std::uint16_t ConvertWindowShiftJisCharacter(std::uint16_t character) {
    std::uint8_t high = character >> 8;
    std::uint8_t low = character;
    if (high > 0xdfU)
        high -= 0x40;
    high = (high - 0x81) * 2 + 0x21;
    if (static_cast<std::int8_t>(low) < 0)
        --low;
    low -= 0x1f;
    if (low > 0x7eU) {
        low -= 0x5e;
        ++high;
    }
    return (high << 8) | low;
}

// @verified - d
std::uint16_t StripWindowCharacterHighBits(std::uint16_t character) {
    return character & 0x7f7fU;
}

// @verified - d
std::int32_t IsScenarioShiftJisLeadByte(std::uint32_t character) {
    const std::uint32_t value = character & 0xffU;
    if (value - 0x81U < 0x1fU || value - 0xe0U < 0x1dU)
        return 1;
    return 0;
}

// @verified - d
std::int32_t IsScenarioShiftJisTrailByte(std::uint32_t character) {
    const std::uint32_t value = character & 0xffU;
    if (value - 0x40U <= 0xbcU && value != 0x7fU)
        return 1;
    return 0;
}

// @verified - d
std::int32_t IsWindowMultibyteCharacter(std::uint32_t character) {
    if (((character & 0xffU) - 0xa1U) < 0x5eU)
        return 1;
    return 0;
}

// @verified - d
std::int32_t IsScenarioEucTrailByte(std::uint32_t character) {
    return IsWindowMultibyteCharacter(character);
}

// @verified - d
std::uint16_t ConvertWindowCharacterToHalfwidth(std::uint16_t character) {
    if (character < 0x100U)
        return character;
    character &= 0x7f7fU;
    if (static_cast<std::uint16_t>(character - 0x2330U) < 10U)
        return character + 0xdd00U;
    if (static_cast<std::uint16_t>(character - 0x2341U) < 26U)
        return character + 0xdd00U;
    if (static_cast<std::uint16_t>(character - 0x2361U) < 26U)
        return character + 0xdd00U;
    if (static_cast<std::uint16_t>(character - 0x2521U) < 86U) {
        const std::uint8_t kana = windowHalfwidthKana[character - 0x2521];
        if (kana == 0x6e || kana == 0x70 || kana == 0x71)
            return windowHalfwidthKana[character - 0x2521] | 0xa580U;
        return windowHalfwidthKana[character - 0x2521] | 0x8080U;
    }
    for (std::uint32_t index = 0; index < 86U; index += 2)
        if (character == (windowHalfwidthSymbols[index] | 0x2100U))
            return windowHalfwidthSymbols[index + 1];
    return character | 0x8080U;
}

// @verified - d
void InitializeScenarioTextDecoder(std::int32_t mode, const char *text, ScenarioTextDecoder &decoder) {
    decoder.cursor = reinterpret_cast<const std::uint8_t *>(text);
    decoder.legacyEncodingMode = static_cast<std::uint32_t>(mode);
    if (mode == 0) {
        decoder.isLeadByte = IsScenarioShiftJisLeadByte;
        decoder.isTrailByte = IsScenarioShiftJisTrailByte;
    } else {
        decoder.isLeadByte = IsWindowMultibyteCharacter;
        decoder.isTrailByte = IsScenarioEucTrailByte;
    }
    decoder.pendingPrefix = 0;
}

std::int32_t AdvanceScenarioTextDecoder(ScenarioTextDecoder &decoder) {
next:
    decoder.currentCharacter = *decoder.cursor;
    if (decoder.currentCharacter == 0) {
        decoder.currentCharacterWidth = 0;
        return 0;
    }
    if (decoder.pendingPrefix) {
        if (decoder.currentCharacter == 0x7f) {
            ++decoder.cursor;
            decoder.pendingPrefix = 0;
            goto next;
        }
        if (!(static_cast<std::uint32_t>(static_cast<std::int16_t>(decoder.currentCharacter)) > 0xfcU)) {
            decoder.currentCharacter |= decoder.pendingPrefix;
            decoder.currentCharacterWidth = 2;
            ++decoder.cursor;
            return 1;
        }
    }
    if (!decoder.legacyEncodingMode && static_cast<std::uint32_t>(static_cast<std::int16_t>(decoder.currentCharacter)) > 0xfcU) {
        decoder.pendingPrefix = static_cast<std::uint16_t>(static_cast<std::uint16_t>(decoder.currentCharacter + 0xff84U) << 8);
        ++decoder.cursor;
        goto next;
    }
    if (decoder.isLeadByte(decoder.currentCharacter) && decoder.isTrailByte(decoder.cursor[1])) {
        decoder.currentCharacter <<= 8;
        decoder.currentCharacter |= decoder.cursor[1];
        decoder.currentCharacterWidth = 2;
        decoder.cursor += 2;
        return 1;
    }
    if (decoder.currentCharacter == '\n')
        decoder.currentCharacterWidth = 0;
    else
        decoder.currentCharacterWidth = 1;
    ++decoder.cursor;
    return 1;
}

// @verified - d
void AllocateFONT() {
    FontRuntimeData *const data = static_cast<FontRuntimeData *>(HLib::AllocateTaskData(sizeof(FontRuntimeData), FONT_STORAGE));
    HLib::SetTaskDataMotionFlag(data);
    fontRuntimeData = data;
    memset(data->kanjiName, 0, sizeof(data->kanjiName));
    memset(data->kanaName, 0, sizeof(data->kanaName));
    memset(data->ryouName, 0, sizeof(data->ryouName));
    data->kanaData = CopyColdArchiveSection(4, false, FONT_STORAGE);
    strcpy(fontRuntimeData->kanaName, "DC_KANA.FON");
    data->kanjiData = CopyColdArchiveSection(3, true, FONT_STORAGE);
    strcpy(fontRuntimeData->kanjiName, "DC_KANJI.FON");
    data->ryouData = CopyColdArchiveSection(5, true, FONT_STORAGE);
    strcpy(fontRuntimeData->ryouName, "RYOU.FON");
}

FontRuntimeData *GetFontRuntimeData() {
    return fontRuntimeData;
}

// @verified - d
void LoadFonts() {
    fontRuntimeData->kanjiName[0] = 0;
    fontRuntimeData->ryouName[0] = 0;
    KanjiFontLoad("DC_KANJI.FON");
    RyouFontLoad("RYOU.FON");
}

// @verified - d
const std::byte *GetWindowGlyphBitmap(FontRuntimeData *font, std::uint16_t glyph) {
    const WindowTaskData *const owner = static_cast<WindowTaskData *>(HLib::GetTaskParameter(windowTask));
    const std::uint32_t low = static_cast<std::uint8_t>(glyph), high = glyph >> 8;
    if ((glyph & 0x8080U) == 0x8080U)
        return reinterpret_cast<const std::byte *>(font->kanaData) + 30 * low;
    if ((glyph & 0xff00U) == 0x100U)
        return reinterpret_cast<const std::byte *>(font->kanaData) + 30 * low;
    std::int32_t index = (high - 0x21) * 94 - 0x21;
    index += low;
    if (!owner->customFlag && std::strlen(fontRuntimeData->ryouName))
        return reinterpret_cast<const std::byte *>(font->ryouData) + 45 * index;
    return reinterpret_cast<const std::byte *>(font->kanjiData) + 45 * index;
}

// @verified - d
void RasterizeWindowGlyph(FontRuntimeData *font, std::uint16_t glyph, void *destination, std::int32_t bytePitch, std::uint16_t colour, std::uint16_t shadow,
    std::uint16_t background) {
    static const std::uint8_t windowGlyphMasks[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
    static const std::uint8_t windowGlyphSpreadKernel[2][2] = {{2, 1}, {1, 1}};
    std::uint16_t palette[4];
    std::uint8_t coverage[400];
    memset(coverage, 0, sizeof(coverage));
    palette[0] = background;
    palette[1] = shadow;
    palette[2] = colour;
    palette[3] = colour;
    const std::byte *const bitmap = GetWindowGlyphBitmap(font, glyph);
    std::int32_t bit, corner, row, column;
    if (glyph > 0x20ff && (glyph & 0x8080) != 0x8080) {
        for (bit = 0; bit < 360; ++bit) {
            if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & windowGlyphMasks[bit & 7]))
                continue;
            row = bit / 24;
            column = bit % 24;
            for (corner = 0; corner < 4; ++corner)
                coverage[(column + (corner & 1)) + 25 * (row + (corner >> 1))] |= windowGlyphSpreadKernel[corner >> 1][corner & 1];
        }
        destination = static_cast<std::byte *>(destination) + 15 * bytePitch;
        for (bit = 0; bit < 16; ++bit) {
            for (corner = 0; corner < 25; ++corner)
                static_cast<std::uint16_t *>(destination)[corner] = palette[coverage[corner + 25 * bit]];
            destination = static_cast<std::byte *>(destination) - bytePitch;
        }
    } else {
        for (bit = 0; bit < 240; ++bit) {
            if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & windowGlyphMasks[bit & 7]))
                continue;
            row = bit >> 4;
            for (corner = 0; corner < 4; ++corner)
                coverage[((bit & 15) + (corner & 1)) + 25 * (row + (corner >> 1))] |= windowGlyphSpreadKernel[corner >> 1][corner & 1];
        }
        destination = static_cast<std::byte *>(destination) + 15 * bytePitch;
        for (bit = 0; bit < 16; ++bit) {
            for (corner = 0; corner < 13; ++corner)
                static_cast<std::uint16_t *>(destination)[corner] = palette[coverage[corner + 25 * bit]];
            destination = static_cast<std::byte *>(destination) - bytePitch;
        }
    }
}

// @verified - d
void ClearWindowTextPixels(std::uint16_t *destination, std::int32_t width, std::int32_t height, std::uint16_t value) {
    while (--height != -1)
        for (std::int32_t x = 0; x < width; ++x)
            *destination++ = value;
}

// @verified - d
HLib::Task *QueueWindowTask(std::uint8_t nextFunctionIndex) {
    const std::int8_t functionIndex = nextFunctionIndex;
    windowTask = HLib::EnqueueTaskWithParameter(WindowTaskEntry_Callback, functionIndex, 2, sizeof(WindowTaskData), WINDOW_TASK_TOKEN);
    WindowTaskData *const data = static_cast<WindowTaskData *>(HLib::GetTaskParameter(windowTask));
    data->name = WINDOW_TASK_MARKER;
    for (std::int32_t index = 0; index < 5; ++index)
        data->slots[index].flags = 0;
    data->customFlag = 1;
    return windowTask;
}

// @verified - d
void WindowTaskEntry_Callback(void *parameter) {
    WindowTaskData *const owner = static_cast<WindowTaskData *>(parameter);
    for (std::int32_t index = 0; index < 5; ++index) {
        if (owner->slots[index].flags & 1U)
            UpdateWindowSlot(&owner->slots[index], owner);
    }
}

// @verified - d
void UpdateWindowSlot(WindowSlot *window, WindowTaskData *owner) {
    bool swapped = false;
    if (window->callback)
        window->callback(window, 0, window->callbackState);
    if (*window->surface) {
        std::int32_t count;
        if (window->flags & 4U) {
            window->lineWidths[window->row] = static_cast<std::uint8_t>(window->column);
            std::int32_t line = window->firstLine;
            for (count = 0; count < window->rows; ++count) {
                if (window->lineFlags[line] & 2U) {
                    UpdateWindowRow(window, owner, count, line);
                    window->lineFlags[line] ^= 6U;
                    break;
                }
                if (++line >= window->rows)
                    line = 0;
            }
            if (count == window->rows) {
                window->flags ^= 0xcU;
                if (window->callback)
                    window->callback(window, 6, window->callbackState);
            }
        }
        std::uint32_t flags = window->flags;
        if (flags & 0x10U) {
            if (flags & 8U) {
                window->flags = (flags ^ 0x18U) | 2U;
                for (count = 0; count < window->rows; ++count) {
                    if (window->lineFlags[count] & 4U) {
                        if (!swapped) {
                            SwapGeneratedSurfaceBuffer(static_cast<GeneratedTextureSurface *>(*window->surface));
                            swapped = true;
                        }
                        window->lineFlags[count] ^= 4U;
                        window->lineFlags[count] |= 1U;
                    } else if (window->lineFlags[count] & 8U)
                        window->lineFlags[count] &= 0xf6U;
                }
                if (window->callback)
                    window->callback(window, 7, window->callbackState);
                flags = window->flags;
            }
        }
        if ((flags & 0x22U) == 0x22U)
            DrawWindowMessageSprites(window);
    }
}

// @verified - d
void DrawWindowMessageSprites(WindowSlot *window) {
    window->lineWidths[window->row] = static_cast<std::uint8_t>(window->column);
    std::uint32_t longest;
    if (window->lineWidths[0] > window->lineWidths[1])
        longest = window->lineWidths[0];
    else
        longest = window->lineWidths[1];
    const std::int32_t x = 320 - (static_cast<std::int32_t>(longest * 13) >> 1);
    std::int32_t y;
    if (window->lineWidths[1])
        y = 376;
    else
        y = 410;
    std::int32_t line = window->firstLine;
    for (std::int32_t index = 0; index < window->rows; ++index) {
        if (window->lineFlags[line] & 1U) {
            LegacySpritePacket packet;
            do {
                INIT_LEGACY_SPRITE_PACKET(packet);
            } while (0);
            SetScreenQuadBounds(packet.quad, x, y + 128, x + 1024, y);
            packet.uv[2] = 1.0f;
            packet.uv[3] = 1.0f;
            packet.quad.flags |= 0x400U;
            const std::int32_t texture = GetGeneratedSurfaceBackTexture(static_cast<GeneratedTextureSurface *>(*window->surface));
            DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(texture), 9.0f);
        }
        if (++line >= window->rows)
            line = 0;
    }
}

// @verified - d
void UpdateWindowRow(WindowSlot *window, WindowTaskData *, std::int32_t row, std::int32_t line) {
    std::int32_t center = 0;
    if (!(g_inputBuffer[1].held & 0x200U)) {
        if (row == 0) {
            if (window->lineWidths[0] < window->lineWidths[1])
                center = window->lineWidths[1] - window->lineWidths[0];
        }
        if (row != 0 && window->lineWidths[0] > window->lineWidths[1])
            center = window->lineWidths[0] - window->lineWidths[1];
        center /= 2;
    }
    std::uint8_t *flags = window->glyphFlags + row * window->columns;
    const std::uint8_t *bytes = window->glyphBytes + row * window->columns;
    for (std::int32_t column = 0; column < window->columns;) {
        std::int32_t count = 1;
        if (*flags & 0x80U) {
            *flags &= 0x7fU;
            std::uint16_t glyph;
            if (*flags & 1U) {
                if (flags[1] & 2U) {
                    glyph = static_cast<std::uint16_t>(bytes[0] << 8);
                    flags[1] &= 0x7fU;
                    glyph |= bytes[1];
                    count = 2;
                } else
                    glyph = 0x120;
            } else
                glyph = bytes[0] | 0x100U;
            RasterizeWindowGlyphShadow(window, fontRuntimeData, glyph, static_cast<std::int16_t>((column + center) % 46), static_cast<std::int16_t>(line % 2));
        }
        column += count;
        flags += count;
        bytes += count;
    }
    switch (static_cast<std::uint32_t>(window->colourMode)) {
    case 0:
        UploadWindowShadowSurface(window, static_cast<GeneratedTextureSurface *>(*window->surface), 0xffff, 0x631);
        break;
    case 1:
        UploadWindowShadowSurface(window, static_cast<GeneratedTextureSurface *>(*window->surface), 0xffff, 7);
        break;
    case 2:
        UploadWindowShadowSurface(window, static_cast<GeneratedTextureSurface *>(*window->surface), 0xffff, 0x631);
        break;
    }
}

// @verified - d
WindowSlot *AllocateWindowSlot(HLib::Task *task, const WindowSpecification &specification) {
    if (!task)
        task = windowTask;
    WindowTaskData *const owner = static_cast<WindowTaskData *>(HLib::GetTaskParameter(task));
    WindowSlot *window = owner->slots;
    for (std::int32_t index = 0; index < 5; ++index, ++window)
        if (!window->flags)
            break;
    window->flags = 1;
    window->firstLine = 0;
    window->horizontalGap = specification.horizontalGap;
    if (window->horizontalGap == -1)
        window->horizontalGap = 1;
    window->verticalGap = specification.verticalGap;
    if (window->verticalGap == -1)
        window->verticalGap = 1;
    window->x = static_cast<float>(specification.x);
    window->y = static_cast<float>(specification.y);
    window->columns = specification.columns;
    window->rows = specification.rows;
    window->field34 = specification.field06;
    window->field36 = specification.field08;
    std::int32_t pixelWidth = window->columns;
    pixelWidth *= window->horizontalGap + 12;
    window->width = specification.width;
    if (window->width == -1)
        window->width = static_cast<std::int16_t>(pixelWidth);
    window->height = specification.height;
    if (window->height == -1)
        window->height = static_cast<std::int16_t>(window->rows * (window->verticalGap + 15));
    window->lineWidth = specification.lineWidth;
    if (window->lineWidth == -1)
        window->lineWidth = static_cast<std::int16_t>(pixelWidth);
    window->lineHeight = specification.lineHeight;
    if (window->lineHeight == -1)
        window->lineHeight = 15;
    std::int32_t textureWidth = 8;
    while (textureWidth < pixelWidth)
        textureWidth *= 2;
    if (specification.flags & 1U)
        window->u = 1.0f;
    else
        window->u = static_cast<float>(pixelWidth) / static_cast<float>(textureWidth);
    window->bytePitch = static_cast<std::int16_t>(textureWidth * 2);
    window->textureHeight = 16;
    window->v = 0.9375f;
    std::size_t shadowBytes = kWindowShadowBufferBytes;
    std::size_t bytes = 4U;
    bytes += shadowBytes;
    bytes += window->columns * window->rows * 2;
    std::byte *const allocation = static_cast<std::byte *>(HLib::AllocateTaskData(bytes + window->rows, WINDOW_SLOT_ALLOCATION));
    window->allocation = allocation;
    window->surface = reinterpret_cast<void **>(allocation);
    window->shadowPixels = reinterpret_cast<std::uint8_t *>(allocation + 4);
    window->glyphFlags = window->shadowPixels + shadowBytes;
    window->glyphBytes = window->glyphFlags + window->columns * window->rows;
    window->lineFlags = window->glyphBytes + window->columns * window->rows;
    window->callbackState = 0;
    window->callback = 0;
    window->colourMode = 0;
    *window->surface = AllocateSHAD_SRUF(1024, 64, 0, 2);
    if (*window->surface)
        EnsureGeneratedSurfaceBackBuffer(static_cast<GeneratedTextureSurface *>(*window->surface));
    ClearWindowText(window);
    return window;
}

void FreeWindowSlot(WindowSlot *window) {
    if (window->callback)
        window->callback(window, 2, window->callbackState);
    if (*window->surface)
        DestroyGeneratedSurface(static_cast<GeneratedTextureSurface *>(*window->surface));
    HLib::FreeTaskData(window->allocation);
    window->flags = 0;
}

void ReleaseWindowSurface(WindowSlot *window) {
    if (*window->surface) {
        ClearWindowText(window);
        DestroyGeneratedSurface(static_cast<GeneratedTextureSurface *>(*window->surface));
        *window->surface = 0;
    }
}

// @verified - d
void RecoverWindowSurface(WindowSlot *window) {
    if (!*window->surface) {
        *window->surface = AllocateSHAD_SRUF(1024, 64, 0, 2);
        if (*window->surface)
            EnsureGeneratedSurfaceBackBuffer(static_cast<GeneratedTextureSurface *>(*window->surface));
    }
}

// @verified - d
void AttachWindowCallback(WindowSlot *window, void (*callback)(WindowSlot *, std::int32_t, void *), void *state) {
    window->callback = callback;
    window->callbackState = state;
    if (callback)
        callback(window, 5, state);
}

// @verified - d
void ClearWindowSlot(WindowSlot *window) {
    std::int32_t index = window->columns * window->rows;
    std::memset(window->glyphFlags, 0x80, index);
    std::memset(window->glyphBytes, 0x20, index);
    window->column = window->row = 0;
    for (index = 0; index < window->rows; ++index)
        window->lineFlags[index] |= 8U;
    ClearWindowShadowPixels(window);
    if (window->callback)
        window->callback(window, 4, window->callbackState);
    window->lineWidths[0] = 0;
    window->lineWidths[1] = 0;
    if ((window->flags & 4U) && window->callback)
        window->callback(window, 6, window->callbackState);
    window->flags &= ~4U;
    window->flags &= ~8U;
}

// @verified - d
void ClearWindowText(WindowSlot *window) {
    window->firstLine = 0;
    for (std::int32_t row = 0; row < window->rows; ++row)
        window->lineFlags[row] = 0;
    ClearWindowSlot(window);
}

// @verified - d
void AdvanceWindowLine(WindowSlot *window) {
    window->lineWidths[window->row] = static_cast<std::uint8_t>(window->column);
    window->column = 0;
    if (++window->row > window->rows)
        window->row = window->rows;
    if (window->callback)
        window->callback(window, 3, window->callbackState);
}

void SetWindowWritePosition(WindowSlot *window, std::int32_t column, std::int32_t row) {
    if (column < 0)
        column = 0;
    else if (column >= window->columns)
        column = window->columns - 1;
    if (row < 0)
        row = 0;
    else if (row >= window->rows)
        row = window->rows - 1;
    window->column = column;
    window->row = row;
}

// @verified - d
std::int32_t GetWindowWritePosition(WindowSlot *window, std::int32_t *column, std::int32_t *row) {
    *column = window->column;
    *row = window->row;
    if (window->column < 0 || window->column >= window->columns)
        return 0;
    if (window->row < 0 || window->row >= window->rows)
        return 0;
    return 1;
}

// @verified - d
std::int32_t WriteWindowGlyphByte(WindowSlot *window, std::uint8_t flags, std::uint8_t value) {
    if (window->flags & 0x0cU)
        return 0x8001;
retry:
    if (window->row >= window->rows)
        return 0x8000;
    if ((flags & 1U) && window->column > window->columns - 2) {
        AdvanceWindowLine(window);
        goto retry;
    }
    const std::int32_t cell = window->row * window->columns + window->column;
    window->glyphFlags[cell] = flags | 0x80U;
    window->glyphBytes[cell] = value;
    std::int32_t line = static_cast<std::int32_t>(static_cast<std::uint32_t>(window->row) + static_cast<std::uint32_t>(window->firstLine));
    if (line >= window->rows)
        line -= window->rows;
    window->lineFlags[line] &= ~8U;
    window->lineFlags[line] |= 2U;
    if (++window->column >= window->columns)
        AdvanceWindowLine(window);
    return 0;
}

std::int32_t WriteWindowCharacter(WindowSlot *window, std::uint16_t character, std::int32_t encodingMode) {
    if (encodingMode == 2) {
        character = ConvertWindowCharacterToHalfwidth(character);
        encodingMode = 1;
    }
    std::int32_t result;
    if (character < 0x100U) {
        if (character == 10) {
            AdvanceWindowLine(window);
            result = 1;
        } else
            result = WriteWindowGlyphByte(window, 0, static_cast<std::uint8_t>(character));
    } else {
        std::uint16_t (*convert)(std::uint16_t);
        if (encodingMode == 1)
            convert = StripWindowCharacterHighBits;
        else
            convert = ConvertWindowShiftJisCharacter;
        character = convert(character);
        result = WriteWindowGlyphByte(window, 1, static_cast<std::uint8_t>(character >> 8U));
        if (result)
            return result;
        result = WriteWindowGlyphByte(window, 2, static_cast<std::uint8_t>(character));
    }
    return result;
}

std::int32_t WriteWindowDecodedText(WindowSlot *window, ScenarioTextDecoder &decoder) {
    std::int32_t result = 0;
    while (AdvanceScenarioTextDecoder(decoder)) {
        result = WriteWindowCharacter(window, decoder.currentCharacter, decoder.legacyEncodingMode);
        if (result & 0x8000)
            break;
    }
    return result;
}

// @verified - d
void SetWindowColourMode(WindowSlot *window, std::int32_t mode) {
    window->colourMode = mode;
}

// @verified - d
void RevealWindowGlyphs(WindowSlot *window) {
    window->flags |= 0x24U;
}

// @verified - d
void ShowWindowSlot(WindowSlot *window) {
    window->flags |= 0x30U;
}

// @verified - d
void HideWindowSlot(WindowSlot *window) {
    window->flags &= ~0x20U;
}

void SetWindowOrigin(WindowSlot *window, std::int16_t x, std::int16_t y) {
    window->field34 = x;
    window->field36 = y;
}

// @verified - d
std::int32_t RenderWindowTextSurface(
    HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode, std::int32_t centered) {
    if (!task)
        task = windowTask;
    (void)HLib::GetTaskParameter(task);
    FontRuntimeData *const font = fontRuntimeData;
    std::int32_t length;
    std::uint16_t glyph;
    if (!text)
        length = 0;
    else if (conversionMode)
        length = static_cast<std::int32_t>(std::strlen(text) * 13 - 1);
    else
        length = static_cast<std::int32_t>((std::strlen(text) / 2) * 14 - 2);
    if (length == 0 || length > width)
        return -1;
    GeneratedTextureSurface *const surface = AllocateSHAD_SRUF(width, 16, 0, 2);
    if (!surface)
        return -1;
    std::uint16_t *pixels = static_cast<std::uint16_t *>(GetGeneratedSurfacePixels(surface));
    ClearWindowTextPixels(pixels, width, 16, 0);
    if (centered)
        pixels += (width - length) / 2;
    while (*text) {
        if (IsWindowMultibyteCharacter(*text)) {
            glyph = static_cast<std::uint16_t>(*text++ << 8);
            glyph |= static_cast<std::uint8_t>(*text++);
            if (conversionMode) {
                RasterizeWindowGlyph(font, StripWindowCharacterHighBits(glyph), pixels, width * 2, colour, shadow, 0);
                pixels += 26;
            } else {
                glyph = ConvertWindowCharacterToHalfwidth(glyph);
                if (glyph < 0x100U)
                    glyph |= 0x100U;
                RasterizeWindowGlyph(font, glyph, pixels, width * 2, colour, shadow, 0);
                pixels += 14;
            }
        } else {
            glyph = static_cast<std::uint8_t>(*text++);
            glyph |= 0x100U;
            RasterizeWindowGlyph(font, glyph, pixels, width * 2, colour, shadow, 0);
            pixels += 14;
        }
    }
    CommitGeneratedSurface(surface);
    return RetireGeneratedSurface(surface);
}

// @verified - d
std::int32_t CreateCenteredWindowTextSurface(
    HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode) {
    return RenderWindowTextSurface(task, text, width, colour, shadow, conversionMode, 1);
}

// @verified - d
std::int32_t SetupUIElement(HLib::Task *task, const char *text, std::int32_t width, std::uint16_t colour, std::uint16_t shadow, std::int32_t conversionMode) {
    return RenderWindowTextSurface(task, text, width, colour, shadow, conversionMode, 0);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t RasterizeWindowTextLine4bpp(std::byte *destination, std::int32_t width, const char *text, std::int32_t mode, std::int32_t centered) {
    FontRuntimeData *const font = fontRuntimeData;
    std::memset(destination, 0xff, width * 8);
    std::int32_t x = 0;
    if (centered) {
        if (mode)
            x = std::strlen(text) * 13 - 1;
        else
            x = std::strlen(text) / 2 * 14 - 2;
        x = (width - x) / 2;
    }
    while (*text) {
        if (*text == '\t') {
            x = x ? 366 : 52;
            ++text;
        } else if (static_cast<std::uint8_t>(*text - 1) < 8)
            x += *text++ * 26;
        else {
            std::uint16_t glyph;
            if (IsWindowMultibyteCharacter(*text)) {
                glyph = static_cast<std::uint16_t>(*text++ << 8);
                glyph |= static_cast<std::uint8_t>(*text++);
                if (mode) {
                    RasterizeWindowGlyph4bpp(font, StripWindowCharacterHighBits(glyph), destination, x);
                    x += 26;
                } else {
                    glyph = ConvertWindowCharacterToHalfwidth(glyph);
                    if (glyph < 0x100U)
                        glyph |= 0x100U;
                    RasterizeWindowGlyph4bpp(font, glyph, destination, x);
                    x += 14;
                }
            } else {
                glyph = *text++ & 0xff;
                glyph |= 0x100U;
                RasterizeWindowGlyph4bpp(font, glyph, destination, x);
                x += 14;
            }
        }
        if (x > width)
            break;
    }
    return x;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t CreateVerticalWindowTextSurface(HLib::Task *task, const char *text, std::int32_t height, std::uint16_t colour, std::uint16_t shadow) {
    if (!task)
        task = windowTask;
    (void)HLib::GetTaskParameter(task);
    FontRuntimeData *const font = fontRuntimeData;
    std::int32_t count = 0;
    if (text) {
        for (std::int32_t offset = 0; text[offset]; ++count) {
            if (IsWindowMultibyteCharacter(text[offset]))
                offset += 2;
            else
                ++offset;
        }
    }
    const std::int32_t extent = count * 16 - 1;
    if (!extent || extent > height)
        return -1;
    GeneratedTextureSurface *const surface = AllocateSHAD_SRUF(32, height, 0, 2);
    if (!surface)
        return -1;
    std::uint16_t *const pixels = static_cast<std::uint16_t *>(GetGeneratedSurfacePixels(surface));
    do {
        ClearWindowTextPixels(pixels, 32, height, 0);
    } while (0);
    std::uint16_t *cursor = pixels + ((height - extent) / 2) * 32 + count * 512;
    while (*text) {
        cursor -= 512;
        std::uint16_t glyph;
        const std::int32_t multibyte = IsWindowMultibyteCharacter(*text);
        const std::int32_t first = *text++;
        if (multibyte) {
            glyph = static_cast<std::uint16_t>(static_cast<std::uint32_t>(first) << 8);
            const std::uint8_t second = static_cast<std::uint8_t>(*text++);
            glyph |= second;
            glyph = StripWindowCharacterHighBits(glyph);
        } else {
            glyph = static_cast<std::uint8_t>(first) | 0x100U;
        }
        RasterizeWindowGlyph(font, glyph, cursor, 64, colour, shadow, 0);
    }
    CommitGeneratedSurface(surface);
    return RetireGeneratedSurface(surface);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::uint8_t windowGlyphSpread[2][2] = {{2, 1}, {1, 1}};
std::uint16_t memoRubyPalette[4] = {0, 0, 0xa00b, 0xa00b};

void RasterizeWindowWideGlyph(FontRuntimeData *font, std::uint16_t glyph, void *destination, std::int32_t bytePitch, std::uint16_t colour, std::uint16_t shadow,
    std::uint16_t background) {
    static const std::uint8_t windowWideGlyphMasks[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
    std::uint8_t coverage[16][25];
    std::uint16_t palette[4];
    std::int32_t bit, corner;
    for (bit = 0; bit < 16; ++bit)
        for (corner = 0; corner < 25; ++corner)
            coverage[bit][corner] = 0;
    palette[0] = background;
    palette[1] = shadow;
    palette[2] = colour;
    palette[3] = colour;
    const std::byte *const bitmap = GetWindowGlyphBitmap(font, glyph);
    if (glyph <= 0x20ff || (glyph & 0x8080) == 0x8080)
        return;
    for (bit = 0; bit < 360; ++bit) {
        if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & windowWideGlyphMasks[bit & 7]))
            continue;
        for (corner = 0; corner < 4; ++corner)
            coverage[bit / 24 + (corner >> 1)][bit % 24 + (corner & 1)] |= windowGlyphSpread[corner >> 1][corner & 1];
    }
    destination = static_cast<std::byte *>(destination) + 15 * bytePitch;
    for (bit = 0; bit < 16; ++bit) {
        for (corner = 0; corner < 25; ++corner)
            static_cast<std::uint16_t *>(destination)[corner] = palette[coverage[bit][corner]];
        destination = static_cast<std::byte *>(destination) - bytePitch;
    }
}

void RasterizeMemoRuby(FontRuntimeData *font, std::uint16_t glyph, void *destination, std::int32_t bytePitch, const std::uint16_t *colours) {
    static const std::uint8_t memoRubyGlyphMasks[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
    std::uint8_t coverage[16][25];
    std::uint16_t palette[4];
    std::int32_t bit, corner, column;
    memset(coverage, 0, sizeof(coverage));
    for (bit = 0; bit < 4; ++bit)
        palette[bit] = colours[bit];
    const std::byte *const bitmap = GetWindowGlyphBitmap(font, glyph);
    if (glyph <= 0x20ff || (glyph & 0x8080) == 0x8080)
        return;
    for (bit = 0; bit < 360; ++bit) {
        if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & memoRubyGlyphMasks[bit & 7]))
            continue;
        for (corner = 0; corner < 4; ++corner)
            coverage[bit / 24 + (corner >> 1)][bit % 24 + (corner & 1)] |= windowGlyphSpread[corner >> 1][corner & 1];
    }
    destination = static_cast<std::byte *>(destination) + 7 * bytePitch;
    for (bit = 0; bit < 8; ++bit) {
        column = 0;
        for (corner = 0; corner < 25; ++corner) {
            if (corner & 1) {
                static_cast<std::uint16_t *>(destination)[column] = palette[coverage[2 * bit][corner]] | palette[coverage[2 * bit + 1][corner]] |
                    palette[coverage[2 * bit][corner + 1]] | palette[coverage[2 * bit + 1][corner + 1]];
                column++;
            }
        }
        destination = static_cast<std::byte *>(destination) - bytePitch;
    }
}

void RasterizeMemoLine(std::uint16_t *destination, const char *text, const char *ruby, std::int16_t width, const std::uint16_t *colours) {
    (void)HLib::GetTaskParameter(windowTask);
    FontRuntimeData *const font = fontRuntimeData;
    std::uint16_t *rubyCursor = destination;
    std::uint16_t glyph;
    while (*text) {
        if (IsWindowMultibyteCharacter(*text)) {
            glyph = static_cast<std::uint16_t>(*text++ << 8);
            glyph = StripWindowCharacterHighBits(glyph | static_cast<std::uint8_t>(*text++));
            RasterizeWindowWideGlyph(font, glyph, destination, width + width, *colours++, 0, 0);
            destination += 26;
        } else {
            glyph = static_cast<std::uint8_t>(*text++) | 0x100U;
            RasterizeWindowGlyph(font, glyph, destination, width + width, *colours++, 0, 0);
            destination += 14;
        }
    }
    if (ruby) {
        const std::uint32_t savedFlag = wndGetFlag(0);
        wndSetFlag(0, 1);
        while (*ruby) {
            if (!IsWindowMultibyteCharacter(*ruby))
                break;
            glyph = static_cast<std::uint16_t>(*ruby++ << 8);
            glyph = StripWindowCharacterHighBits(glyph | static_cast<std::uint8_t>(*ruby++));
            RasterizeMemoRuby(font, glyph, rubyCursor + width * 16, width + width, memoRubyPalette);
            rubyCursor += 13;
        }
        wndSetFlag(0, static_cast<std::int32_t>(savedFlag));
    }
}
#endif

// @verified - d
void RasterizeWindowGlyphShadow(WindowSlot *window, FontRuntimeData *font, std::uint16_t glyph, std::int16_t column, std::int16_t row) {
    static const std::uint8_t windowGlyphShadowMasks[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
    const std::byte *const bitmap = GetWindowGlyphBitmap(font, glyph);
    std::int32_t columnOffset, rowOffset, bit, x, y;
    std::uint8_t *output;
    const std::uint8_t *kernel;
    if (glyph > 0x20ff) {
        columnOffset = 13 * column;
        rowOffset = 17 * row;
        for (bit = 0; bit < 360; ++bit) {
            if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & windowGlyphShadowMasks[bit & 7]))
                continue;
            output = window->shadowPixels + 608 * (bit / 24 + rowOffset) + (bit % 24 + columnOffset);
            kernel = windowGlyphShadowKernel;
            for (y = 0; y < 7; ++y) {
                for (x = 0; x < 13; ++x)
                    *output++ |= *kernel++;
                output += 596;
            }
        }
    } else {
        columnOffset = 13 * column;
        rowOffset = 17 * row;
        for (bit = 0; bit < 240; ++bit) {
            if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & windowGlyphShadowMasks[bit & 7]))
                continue;
            output = window->shadowPixels + 608 * ((bit >> 4) + rowOffset) + ((bit & 15) + columnOffset);
            kernel = windowGlyphShadowKernel;
            for (y = 0; y < 7; ++y) {
                for (x = 0; x < 13; ++x)
                    *output++ |= *kernel++;
                output += 596;
            }
        }
    }
}

// @verified - d
void UploadWindowShadowSurface(WindowSlot *window, GeneratedTextureSurface *surface, std::uint16_t foreground, std::uint16_t shadow) {
    std::uint16_t alphaBands[8];
    std::uint16_t palette[256];
    alphaBands[1] = 0x8000;
    alphaBands[2] = alphaBands[3] = 0xc000;
    alphaBands[4] = alphaBands[5] = alphaBands[6] = alphaBands[7] = 0xf000;
    shadow &= 0xfffU;
    std::int32_t index = 0;
    palette[index++] = 0;
    for (; index < 16; ++index)
        palette[index] = ((index << 11) & 0x7000) | shadow;
    for (; index < 128; ++index)
        palette[index] = alphaBands[index >> 4] | shadow;
    for (; index < 256; ++index)
        palette[index] = foreground;
    const std::uint8_t *source = window->shadowPixels;
    std::uint16_t *output = static_cast<std::uint16_t *>(GetGeneratedSurfacePixels(surface));
    for (std::int32_t y = 0; y < 38; ++y) {
        for (std::int32_t x = 0; x < 608; ++x)
            *output++ = palette[*source++];
        output += 416;
    }
    CommitGeneratedSurface(surface);
}

// @verified - d
void ClearWindowShadowPixels(WindowSlot *window) {
    std::memset(window->shadowPixels, 0, kWindowShadowBufferBytes);
}

// @verified - d
void KanjiFontLoad(const char *name) {
    if (!std::strlen(fontRuntimeData->kanjiName) || CompareCharacterTableTokens(name, fontRuntimeData->kanjiName)) {
        shenLoadFile("/misc", name, fontRuntimeData->kanjiData, 0x55c46);
        std::strcpy(fontRuntimeData->kanjiName, name);
    }
}

// @verified - d
void RyouFontLoad(const char *name) {
    if (!std::strlen(fontRuntimeData->ryouName) || CompareCharacterTableTokens(name, fontRuntimeData->ryouName)) {
        shenLoadFile("/misc", name, fontRuntimeData->ryouData, 0x55c46);
        std::strcpy(fontRuntimeData->ryouName, name);
    }
}

// @verified - d
void KanaFontLoad(const char *name) {
    if (!std::strlen(fontRuntimeData->kanaName) || CompareCharacterTableTokens(name, fontRuntimeData->kanaName)) {
        shenLoadFile("/misc", name, fontRuntimeData->kanaData, 0x1e20);
        std::strcpy(fontRuntimeData->kanaName, name);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
char *GetKanjiFontName() {
    return fontRuntimeData->kanjiName;
}

char *GetRyouFontName() {
    return fontRuntimeData->ryouName;
}

char *GetKanaFontName() {
    return fontRuntimeData->kanaName;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void wndSetFlag(HLib::Task *task, std::int32_t flag) {
    if (!task)
        task = windowTask;
    WindowTaskData *const data = static_cast<WindowTaskData *>(HLib::GetTaskParameter(task));
    data->customFlag = static_cast<std::uint32_t>(flag);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t wndGetFlag(HLib::Task *task) {
    if (!task)
        task = windowTask;
    return static_cast<WindowTaskData *>(HLib::GetTaskParameter(task))->customFlag;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void wndNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RasterizeWindowGlyph4bpp(FontRuntimeData *font, std::uint16_t glyph, std::byte *destination, std::int32_t pixelX) {
    static const std::uint8_t windowGlyph4bppMasks[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
    static const std::uint8_t windowGlyph4bppSpread[2][2] = {{2, 1}, {1, 1}};
    std::uint16_t palette[4];
    std::uint8_t coverage[400];
    std::int32_t i, j, x, offset, y;
    memset(coverage, 0, sizeof(coverage));
    palette[0] = 0xff;
    palette[1] = 0x11;
    palette[2] = 0;
    palette[3] = 0;
    const std::byte *const bitmap = GetWindowGlyphBitmap(font, glyph);
    if (glyph > 0x20ff && (glyph & 0x8080) != 0x8080) {
        for (i = 0; i < 360; ++i) {
            if (!(static_cast<std::uint8_t>(bitmap[i >> 3]) & windowGlyph4bppMasks[i & 7]))
                continue;
            x = i / 24;
            offset = i % 24;
            for (j = 0; j < 4; ++j)
                coverage[(offset + (j & 1)) + 25 * (x + (j >> 1))] |= windowGlyph4bppSpread[j >> 1][j & 1];
        }
        for (j = 0; j < 25; ++j) {
            x = pixelX + j;
            offset = x / 16;
            offset *= 128;
            if (x & 8)
                offset += 64;
            if (x & 4)
                offset += 16;
            if (x & 2)
                offset += 4;
            if (x & 1)
                offset += 1;
            for (i = 0; i < 16; i += 2) {
                y = 14 - i;
                x = y & 8 ? 32 : 0;
                if (y & 4)
                    x += 8;
                if (y & 2)
                    x += 2;
                *(destination + offset + x) = static_cast<std::byte>((palette[coverage[j + 25 * i]] & 0xf0) + (palette[coverage[j + 25 * (i + 1)]] & 0x0f));
            }
        }
    } else {
        for (i = 0; i < 240; ++i) {
            if (!(static_cast<std::uint8_t>(bitmap[i >> 3]) & windowGlyph4bppMasks[i & 7]))
                continue;
            x = i >> 4;
            for (j = 0; j < 4; ++j)
                coverage[((i & 15) + (j & 1)) + 25 * (x + (j >> 1))] |= windowGlyph4bppSpread[j >> 1][j & 1];
        }
        for (j = 0; j < 13; ++j) {
            x = pixelX + j;
            offset = x / 16;
            offset *= 128;
            if (x & 8)
                offset += 64;
            if (x & 4)
                offset += 16;
            if (x & 2)
                offset += 4;
            if (x & 1)
                offset += 1;
            for (i = 0; i < 16; i += 2) {
                y = 14 - i;
                x = y & 8 ? 32 : 0;
                if (y & 4)
                    x += 8;
                if (y & 2)
                    x += 2;
                *(destination + offset + x) = static_cast<std::byte>((palette[coverage[j + 25 * i]] & 0xf0) + (palette[coverage[j + 25 * (i + 1)]] & 0x0f));
            }
        }
    }
}
#endif

} // namespace s2
} // namespace shenmue

#endif
