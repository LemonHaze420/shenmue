#include "s1/task/wind.hpp"
#include "common/math.hpp"
#include "s1/boot.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/screen_space.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/effe.hpp"
#include "s1/timing.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

void WIND_DrawGlyphs(void *entry, std::int64_t flagOffset, std::int32_t examined, std::int32_t index);
void WIND_AspectRatioPass(void *entry);
HLib::Task *g_windTask = nullptr;
WeatherWindData *g_weatherWindData = nullptr;
extern void *g_pFont;
float g_windPulseTimer = 0.0f;
float g_fWINDIntensity = 0.0f;
std::int32_t g_WINDAngle = 0;

void WIND_Callback_1(HLib::TaskData *) {
    const double current = g_windPulseTimer;
    if (g_windPulseTimer <= 0.0f) {
        const float next = static_cast<float>(current + 0.03333333333333333);
        g_windPulseTimer = next;
        if (next > 0.0f) {
            g_fWINDIntensity = 0.5f;
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            g_windPulseTimer = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125) * 4.0f + 1.0f;
        }
    } else {
        const float next = static_cast<float>(current - 0.03333333333333333);
        g_windPulseTimer = next;
        if (next < 0.0f) {
            const std::uint32_t first = LcgStep(g_EnvPrngSeed);
            const float random = static_cast<float>(static_cast<double>((first >> 16U) & 0x7fffU) * 0.000030517578125);
            g_EnvPrngSeed = LcgStep(first);
            g_fWINDIntensity = random * 5.0f + 0.5f;
            g_windPulseTimer = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125) - 1.5f;
        }
    }
}

void WIND_Callback(HLib::TaskData *) {
    HLib::Task *const task = CurrentTask;
    g_windPulseTimer = 0.0f;
    g_fWINDIntensity = 0.0f;
    g_WINDAngle = 0;
    if (task) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = WIND_Callback_1;
        task->autoInitTaskCallbackFnPtr = nullptr;
    }
}

void WIND_UpdateEntry(void *entryPointer) {
    WindEntry &entry = *static_cast<WindEntry *>(entryPointer);
    if (entry.callback) {
        entry.callback(&entry, 0, entry.callbackContext);
    }
    if (!entry.surfaceHandleSlot || !*entry.surfaceHandleSlot || !**entry.surfaceHandleSlot) {
        return;
    }

    std::int32_t examined = 0;
    if ((entry.flags & 4) != 0) {
        entry.rowEndColumns[static_cast<std::int16_t>(entry.packedCursor >> 16)] = static_cast<std::uint8_t>(entry.packedCursor);
        std::int32_t index = entry.rowScrollOrigin;
        std::int64_t flagOffset = entry.rowScrollOrigin;
        if (entry.cellHeightPixels > 0) {
            while ((entry.rowDirtyFlags[flagOffset] & 2) == 0) {
                const std::int32_t nextIndex = index + 1;
                const std::int64_t nextOffset = flagOffset + 1;
                flagOffset = nextIndex < entry.cellHeightPixels ? nextOffset : 0;
                ++examined;
                index = nextIndex < entry.cellHeightPixels ? nextIndex : 0;
                if (examined >= entry.cellHeightPixels) {
                    break;
                }
            }
            if (examined < entry.cellHeightPixels) {
                WIND_DrawGlyphs(&entry, flagOffset, examined, index);
                entry.rowDirtyFlags[index] ^= 6;
            }
        }
        if (examined == entry.cellHeightPixels) {
            entry.flags ^= 0x0c;
            if (entry.callback) {
                entry.callback(&entry, 6, entry.callbackContext);
            }
        }
    }

    if ((entry.flags & 0x18) == 0x18) {
        entry.flags = (entry.flags ^ 0x18) | 2;
        bool changedRenderObject = false;
        for (std::int32_t index = 0; index < entry.cellHeightPixels; ++index) {
            std::uint8_t &flags = entry.rowDirtyFlags[index];
            if ((flags & 4) != 0) {
                if (!changedRenderObject) {
                    changedRenderObject = true;
                    (*entry.surfaceHandleSlot)[24] ^= 1;
                }
                flags ^= 4;
                flags |= 1;
            } else if ((flags & 8) != 0) {
                flags &= 0xf6;
            }
        }
        if (entry.callback) {
            entry.callback(&entry, 7, entry.callbackContext);
        }
    }
    if ((entry.flags & 0x22) == 0x22) {
        WIND_AspectRatioPass(&entry);
    }
}

void WIND_Callback_0(HLib::TaskData *taskData) {
    auto *entry = reinterpret_cast<std::byte *>(taskData) + 8;
    for (std::int32_t index = 0; index < 5; ++index, entry += 0x88) {
        if ((*reinterpret_cast<std::uint32_t *>(entry) & 1) != 0) {
            WIND_UpdateEntry(entry);
        }
    }
}

HLib::Task *WIND_Init() {





    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(WIND_Callback_0, 2, 2, SHEN_NAME(WIND));
    if (task) {
        if (!task->task_data_ptr) {
            auto *const data = static_cast<WindTaskData *>(HLib_AllocHeapMemBlock(sizeof(WindTaskData), SHEN_NAME(TASK)));
            if (!data) {
                task->initCallbackFnPtr = nullptr;
                task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
                task->autoInitTaskCallbackFnPtr = nullptr;
                task = nullptr;
            } else {
                HLib_EnableTokenCheck(data);
                task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
            }
        } else {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
            task = nullptr;
        }
    }

    g_windTask = task;
    auto *const data = task ? reinterpret_cast<WindTaskData *>(task->task_data_ptr) : nullptr;
    if (data) {
        data->identifier = SHEN_NAME(WNDT);
        for (WindEntry &entry : data->entries)
            entry.flags = 0;
        data->tailWord = 1;
    }
    return task;
}

namespace {

constexpr std::int32_t makeToken(char a, char b, char c, char d) noexcept {
    return static_cast<std::int32_t>(static_cast<std::uint8_t>(a)) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(b)) << 8) |
        (static_cast<std::int32_t>(static_cast<std::uint8_t>(c)) << 16) | (static_cast<std::int32_t>(static_cast<std::uint8_t>(d)) << 24);
}

constexpr std::array<std::uint8_t, 8> kGlyphBitMasks{
    1, 2, 4, 8, 16, 32, 64, 128};
constexpr std::array<std::uint8_t, 91> kGlyphStamp{
    0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
    1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1,
    1, 2, 4, 8, 8, 16, 16, 16, 8, 8, 4, 2, 1,
    1, 2, 4, 8, 16, 32, 128, 64, 16, 8, 4, 2, 1,
    1, 2, 4, 8, 8, 16, 16, 64, 64, 8, 4, 2, 1,
    1, 2, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1,
    0, 1, 2, 2, 2, 2, 2, 2, 2, 4, 2, 1, 0};

struct FontStorage {
    const std::uint8_t *kana;
    const std::uint8_t *kanji;
    const std::uint8_t *ryou;
};

#pragma pack(push, 1)
struct WindRenderObject {
    std::uint32_t flags;
    std::uint8_t formatIndex;
    std::uint8_t reserved_005;
    std::uint16_t updateDepth;
    std::int16_t surfaceIndices[4];
    std::uint32_t *pixels;
    std::uint8_t selectedSurface;
};
#pragma pack(pop)

static_assert(offsetof(WindRenderObject, updateDepth) == 0x06);
static_assert(offsetof(WindRenderObject, surfaceIndices) == 0x08);
static_assert(offsetof(WindRenderObject, pixels) == 0x10);
static_assert(offsetof(WindRenderObject, selectedSurface) == 0x18);

struct WeatherWindRegistryNode {
    WeatherWindRegistryNode *previous;
    WeatherWindRegistryNode *next;
    WeatherWindData *wind;
    std::uint64_t reserved;
};

static_assert(sizeof(WeatherWindRegistryNode) == 0x20);

WeatherWindRegistryNode *g_weatherWindRegistry = nullptr;
constexpr std::array<std::array<float, 9>, 16> kSakuraAngleWeights{{
        {{0.3f, 6.0f, 6.0f, 4.0f, 4.0f, 2.0f, 2.0f, 2.0f, 2.0f}},
        {{0.3f, 5.0f, 5.0f, 2.0f, 1.4f, 0.9f, 0.8f, 0.7f, 0.2f}},
        {{0.3f, 1.0f, 1.0f, 1.0f, 0.5f, 0.2f, 0.0f, 0.0f, 0.0f}},
        {{0.3f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 1.0f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 0.6f, 0.6f, 0.8f, 0.7f, 0.6f, 0.5f, 0.5f, 0.0f}},
        {{0.2f, 0.8f, 1.2f, 2.5f, 2.1f, 1.7f, 1.2f, 0.6f, 0.0f}},
        {{0.2f, 0.8f, 1.0f, 1.6f, 1.0f, 0.5f, 0.4f, 0.0f, 0.0f}},
        {{0.2f, 1.0f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 0.7f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 0.7f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.2f, 0.8f, 0.7f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},
        {{0.3f, 1.0f, 1.0f, 1.0f, 0.6f, 0.4f, 0.4f, 0.3f, 0.0f}},
        {{0.4f, 3.5f, 2.8f, 2.5f, 2.2f, 1.2f, 1.2f, 1.2f, 1.2f}},
}};

std::array<float, 256> g_sakuraWindNoise{};
std::uint32_t g_sakuraWindNoiseIndex = 0;
bool g_sakuraWindNoiseInitialized = false;


std::array<float, 48> g_sakuraKernel{};
float g_sakuraKernelTotalWeight{};

// @verified - p
float EvaluatePowExpKernel(float shape, float scale, float t) {
    constexpr float kEpsilon = 0.00000011920929f;
    if (std::fabs(shape) < kEpsilon || std::fabs(scale) < kEpsilon || std::fabs(t) < kEpsilon)
        return 0.0f;
    const float ratio = t / scale;
    if (std::fabs(ratio) < kEpsilon)
        return 0.0f;
    const float exponent = shape - 1.0f;
    if (std::fabs(exponent) < kEpsilon)
        return 0.0f;
    const float power = fast_powf_approx(ratio, exponent);
    if (std::fabs(power) < kEpsilon)
        return 0.0f;
    const float factor = shape / scale;
    if (std::fabs(factor) < kEpsilon)
        return 0.0f;
    const float density = factor * power;
    if (std::fabs(density) < kEpsilon)
        return 0.0f;
    const float decay = -(power * ratio);
    if (std::fabs(decay) < kEpsilon)
        return 0.0f;
    const float falloff = fast_expf_approx(decay);
    if (std::fabs(falloff) < kEpsilon)
        return 0.0f;
    return density * falloff;
}

// @verified - p
void BuildPowExpKernelLUT48(float shape, float range) {
    constexpr float kEpsilon = 0.00000011920929f;
    const float rangeMagnitude = std::fabs(range);
    float sample = 0.0f;
    std::size_t index = 0;
    do {
        float t = 0.0f;
        if (rangeMagnitude >= kEpsilon) {
            const float fraction = sample / 48.0f;
            if (std::fabs(fraction) >= kEpsilon && std::fabs(fraction * range) >= kEpsilon)
                t = fraction * range;
        }
        g_sakuraKernel[index++] = EvaluatePowExpKernel(shape, 1.0f, t);
        sample = static_cast<float>(static_cast<double>(sample) + 1.0);
    } while (48.0f > sample);
}

float nextEnvironmentRandom(std::uint32_t &seed) {
    seed = LcgStep(seed);
    return static_cast<float>((seed >> 16U) & 0x7fffU) / 32768.0f;
}

float nextSakuraWindNoise() {
    const float result = g_sakuraWindNoise[g_sakuraWindNoiseIndex];
    g_sakuraWindNoiseIndex = (g_sakuraWindNoiseIndex + 1U) & 0xffU;
    return result;
}

// A gust strength for wind strength `strength`: the next smoothed noise sample, above 0.35 and doubled (at most 1), picks a point
// of the kernel's cumulative weight (plus one), and the kernel bin reached there scales strength by 0.1 per bin. Past the last bin
// the leftover weight extends the scale, capped at 2/3.
// @verified - p
float GetValueFromSakuraLUT(float strength) {
    if (0.0f >= strength)
        return 0.0f;
    const float noise = nextSakuraWindNoise();
    const double total = static_cast<double>(g_sakuraKernelTotalWeight) + 1.0;
    float level = static_cast<float>(static_cast<double>(noise) - 0.35);
    level = 0.0f > level ? 0.0f : level;
    const float doubled = static_cast<float>(static_cast<double>(level) + static_cast<double>(level));
    const float clamped = 1.0f < doubled ? 1.0f : doubled;
    const float target = static_cast<float>(static_cast<double>(clamped) * total);
    float cumulative = 0.0f;
    std::int32_t bin = 0;
    while (bin < 48) {
        cumulative += g_sakuraKernel[static_cast<std::size_t>(bin)];
        if (cumulative > target)
            break;
        ++bin;
    }
    if (bin < 48)
        return static_cast<float>(static_cast<double>(static_cast<float>(bin - 1) * strength) * 0.1);
    const float excess = target - g_sakuraKernelTotalWeight;
    const double span = total - static_cast<double>(g_sakuraKernelTotalWeight);
    const float value = static_cast<float>(static_cast<double>(excess) / span * 80.0 * static_cast<double>(strength) * 0.1);
    return static_cast<double>(value) >= 0.6666666666666666 ? 0.6666667f : value;
}



std::int32_t pickSakuraWindAngle(float strength) {
    const std::int32_t strengthIndex = std::min(
        static_cast<std::int32_t>(strength * 30.0f), 8);
    const float random = nextEnvironmentRandom(g_EnvPrngSeed);
    float total = 0.0f;
    for (const auto &weights : kSakuraAngleWeights) {
        total += weights[static_cast<std::size_t>(strengthIndex)];
    }
    const float selected = random * total;
    float cumulative = 0.0f;
    for (std::int32_t bin = 0; bin < 16; ++bin) {
        cumulative += kSakuraAngleWeights[static_cast<std::size_t>(bin)][static_cast<std::size_t>(strengthIndex)];
        if (cumulative > selected) {
            return (bin << 12) - 0x1000;
        }
    }
    return 0xf000;
}

const std::uint8_t *glyphSource(const FontStorage *font, std::uint16_t glyphCode) {
    if (!font)
        return nullptr;
    if ((glyphCode & 0x8080U) == 0x8080U || (glyphCode & 0xff00U) == 0x0100U) {
        return font->kana ? font->kana + 30U * static_cast<std::uint8_t>(glyphCode) : nullptr;
    }

    const std::int32_t glyphIndex = 94 * static_cast<std::uint8_t>(glyphCode >> 8) + static_cast<std::uint8_t>(glyphCode) - 3135;
    const bool usePrimaryKanji = !g_windTask || !g_windTask->task_data_ptr || reinterpret_cast<WindTaskData *>(g_windTask->task_data_ptr)->tailWord != 0;
    const std::uint8_t *const base = usePrimaryKanji || !font->ryou ? font->kanji : font->ryou;
    return base ? base + 45 * glyphIndex : nullptr;
}

void stampGlyph(WindEntry &entry, std::uint16_t glyphCode, std::int16_t column, std::int16_t row) {
    const std::uint8_t *const source = glyphSource(static_cast<const FontStorage *>(g_pFont), glyphCode);
    if (!source || !entry.workBuffer)
        return;

    const std::int32_t bitCount = glyphCode < 0x2100U ? 240 : 360;
    for (std::int32_t pixel = 0; pixel < bitCount; ++pixel) {
        if ((source[pixel >> 3] & kGlyphBitMasks[pixel & 7]) == 0) {
            continue;
        }

        const std::ptrdiff_t horizontal = 13 * (column / 2) + 6 * (column % 2);
        std::uint8_t *destination = glyphCode < 0x2100U ? entry.workBuffer + horizontal + (pixel & 0x0f) + 608 * (17 * row + (pixel >> 4)) :
                                                          entry.workBuffer + horizontal + (pixel % 24) + 608 * (17 * row + pixel / 24);
        const std::uint8_t *stamp = kGlyphStamp.data();
        for (std::int32_t stampRow = 0; stampRow < 7; ++stampRow) {
            for (std::int32_t stampColumn = 0; stampColumn < 13; ++stampColumn) {
                *destination++ |= *stamp++;
            }
            destination += 596;
        }
    }
}

std::uint32_t expandPaletteColour(std::uint16_t colour) {
    std::uint32_t expanded = colour >> 12;
    expanded = (expanded << 8) + ((colour >> 8) & 0x0fU);
    expanded = (expanded << 8) + ((colour >> 4) & 0x0fU);
    expanded = (expanded << 8) + (colour & 0x0fU);
    return expanded * 17U;
}

void uploadWindBitmap(WindEntry &entry, std::uint16_t backgroundColour, std::uint16_t foregroundColour) {
    if (!entry.surfaceHandleSlot || !*entry.surfaceHandleSlot)
        return;
    WindRenderObject &object = *reinterpret_cast<WindRenderObject *>(*entry.surfaceHandleSlot);
    if (!object.pixels || !entry.workBuffer)
        return;

    std::array<std::uint16_t, 256> palette{};
    for (std::uint32_t index = 1; index < 16; ++index) {
        palette[index] = static_cast<std::uint16_t>((foregroundColour & 0x0fffU) | ((index & 0x0eU) << 11));
    }
    for (std::uint32_t index = 16; index < 128; ++index) {
        const std::uint16_t alpha = index < 32 ? 0x8000U : index < 64 ? 0xc000U : 0xf000U;
        palette[index] = static_cast<std::uint16_t>(alpha | foregroundColour);
    }
    std::fill(palette.begin() + 128, palette.end(), backgroundColour);

    ++object.updateDepth;
    std::uint32_t *destination = object.pixels + (object.formatIndex == 2 ? 32 : 0);
    const std::uint8_t *source = entry.workBuffer;
    for (std::int32_t y = 0; y < 38; ++y) {
        for (std::int32_t x = 0; x < 608; ++x) {
            destination[x] = expandPaletteColour(palette[source[x]]);
        }
        source += 608;
        destination += 1024;
    }

    --object.updateDepth;
    if (object.updateDepth != 0 || object.selectedSurface >= 4)
        return;
    const std::int16_t surfaceIndex = object.surfaceIndices[object.selectedSurface];
    if (surfaceIndex < 0)
        return;

    const std::uint32_t textureHandle = GetPVRSurfaceTextureHandle(surfaceIndex);
    Texture2DDescription description{};
    if (textureHandle && GetTexture2DDescription(textureHandle, &description)) {
        description.initialData = object.pixels;
        ReplaceTexture2DResource(textureHandle, &description);
    }
}


WeatherWindData *allocateWindData() {
    auto *const wind = static_cast<WeatherWindData *>(HLib_AllocHeapMemBlock(sizeof(WeatherWindData), makeToken('W', 'I', 'N', 'D')));
    if (!wind)
        return nullptr;
    std::memset(wind, 0, sizeof(*wind));
    wind->strengthDivisor = std::bit_cast<float>(0x425e9b09U);
    return wind;
}

void freeWindData(WeatherWindData *wind) {
    if (wind) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(wind), 0);
    }
}

WeatherWindRegistryNode *findStandaloneWind(WeatherWindData *wind) {
    for (WeatherWindRegistryNode *node = g_weatherWindRegistry; node; node = node->next) {
        if (node->wind == wind)
            return node;
    }
    return nullptr;
}

bool registerStandaloneWind(WeatherWindData *wind) {
    auto *const node = static_cast<WeatherWindRegistryNode *>(HLib_AllocHeapMemBlock(sizeof(WeatherWindRegistryNode), makeToken('D', 'L', 'S', 'T')));
    if (!node)
        return false;
    std::memset(node, 0, sizeof(*node));
    node->wind = wind;
    node->next = g_weatherWindRegistry;
    if (node->next)
        node->next->previous = node;
    g_weatherWindRegistry = node;
    return true;
}

void unregisterStandaloneWind(WeatherWindRegistryNode *node) {
    if (!node)
        return;
    if (node->previous) {
        node->previous->next = node->next;
    } else {
        g_weatherWindRegistry = node->next;
    }
    if (node->next)
        node->next->previous = node->previous;
    WeatherWindData *const wind = node->wind;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(node), 0);
    freeWindData(wind);
}

WeatherWindData *windFromOffset(std::uint32_t offset) {
    return offset && g_pCurrentTaskData ? reinterpret_cast<WeatherWindData *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) : nullptr;
}

std::uint32_t windOffset(const WeatherWindData *wind) {
    return HLib_GetTaskDataOffset(wind);
}

void setWindAngle(WeatherWindData *wind, std::uint32_t angle) {
    if (!wind)
        return;
    wind->targetAngle = static_cast<std::int32_t>(angle);
    wind->angleStep = (wind->targetAngle - wind->currentAngle) / 10;
    wind->angleFramesRemaining = 10.0f;
    wind->gustFramesRemaining = 0.0f;
}

void setWindStrength(WeatherWindData *wind, std::uint32_t strength) {
    if (!wind)
        return;
    wind->targetStrength = std::bit_cast<float>(strength);
    wind->strengthDelta = wind->targetStrength - wind->currentStrength;
    wind->strengthFramesRemaining = 1.0f;
    wind->gustFramesRemaining = 0.0f;
}

} // namespace

std::uint64_t RasterizeFontGlyph(const void *font, std::uint16_t glyph, void *pixels, std::int32_t pitch, std::uint16_t foreground, std::uint16_t background) {
    const std::uint8_t *const source = glyphSource(static_cast<const FontStorage *>(font), glyph);
    if (!source || !pixels)
        return 0;
    const bool kana = glyph < 0x2100U || (glyph & 0x8080U) == 0x8080U;
    const std::int32_t sourceWidth = kana ? 16 : 24;
    const std::int32_t bitCount = kana ? 240 : 360;
    const std::int32_t outputWidth = kana ? 13 : 25;
    const std::uint16_t palette[4]{0, background, foreground, foreground};
    std::uint8_t coverage[400]{};
    for (std::int32_t bit = 0; bit < bitCount; ++bit) {
        if ((source[bit >> 3] & kGlyphBitMasks[bit & 7]) == 0)
            continue;
        const std::int32_t x = bit % sourceWidth;
        const std::int32_t y = bit / sourceWidth;
        coverage[y * 25 + x] |= 2U;
        coverage[y * 25 + x + 1] |= 1U;
        coverage[(y + 1) * 25 + x] |= 1U;
        coverage[(y + 1) * 25 + x + 1] |= 1U;
    }
    std::uint32_t lastPixel = 0;
    for (std::int32_t y = 0; y < 16; ++y) {
        std::uint32_t *const destination = reinterpret_cast<std::uint32_t *>(static_cast<std::uint8_t *>(pixels) + (15 - y) * pitch);
        for (std::int32_t x = 0; x < outputWidth; ++x) {
            lastPixel = expandPaletteColour(palette[coverage[y * 25 + x]]);
            destination[x] = lastPixel;
        }
    }
    return kana ? 13U : lastPixel;
}

void WIND_InitSakuraWindLUT() {
    auto *const coefficients = static_cast<float *>(HLib_AllocHeapMemBlock(13 * sizeof(float), makeToken('T', 'M', 'P', '0')));
    auto *const samples = static_cast<float *>(HLib_AllocHeapMemBlock(13 * sizeof(float), makeToken('T', 'M', 'P', '1')));
    coefficients[0] = 1.0f;
    float totalWeight = coefficients[0];
    for (std::size_t index = 1; index < 13; ++index) {
        const float n = static_cast<float>(index);
        coefficients[index] = ((2.0f * n - 1.0f) * coefficients[index - 1U]) / (2.0f * n);
        totalWeight += coefficients[index];
    }

    std::uint32_t seed = g_EnvPrngSeed;
    for (std::size_t index = 0; index < 13; ++index)
        samples[index] = nextEnvironmentRandom(seed);
    for (float &output : g_sakuraWindNoise) {
        float weighted = 0.0f;
        for (std::size_t index = 0; index < 13; ++index)
            weighted += coefficients[index] * samples[index];
        output = weighted / totalWeight;
        for (std::size_t index = 0; index < 12; ++index)
            samples[index] = samples[index + 1];
        samples[12] = nextEnvironmentRandom(seed);
    }
    g_EnvPrngSeed = seed;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(samples), 0);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(coefficients), 0);
}



void WIND_InitializeSakuraWindState() {
    BuildPowExpKernelLUT48(1.78f, 4.8f);
    float total = 0.0f;
    for (const float weight : g_sakuraKernel)
        total += weight;
    g_sakuraKernelTotalWeight = total;
    WIND_InitSakuraWindLUT();
    g_sakuraWindNoise[0] = 0.0f;
    g_sakuraWindNoiseIndex = 0;
    g_sakuraWindNoiseInitialized = true;
}
WeatherWindData *WIND_EnsureWeatherData() {
    if (!g_weatherWindData) {
        g_weatherWindData = allocateWindData();
    }
    return g_weatherWindData;
}

WeatherWindData *WIND_GetWeatherData() {
    return g_weatherWindData;
}

void Weather_Wind_UpdateFromWDAT(WeatherWindData *wind) {
    if (!wind)
        return;
    const float frameAmount = g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 30.0f : 1.0f;

    if (wind->targetAngle != wind->currentAngle) {
        if (wind->angleFramesRemaining > 0.0f) {
            wind->angleFramesRemaining -= frameAmount;
            wind->currentAngle =
                static_cast<std::int32_t>(static_cast<float>(wind->targetAngle) - static_cast<float>(wind->angleStep) * wind->angleFramesRemaining);
        } else {
            wind->currentAngle = wind->targetAngle;
        }
    }

    if (wind->targetStrength != wind->currentStrength) {
        float current = wind->targetStrength;
        if (wind->strengthFramesRemaining > 0.0f) {
            wind->strengthFramesRemaining -= frameAmount;
            current -= wind->strengthFramesRemaining * wind->strengthDelta;
        }
        wind->currentStrength = current;
    }

    if (wind->gustFramesRemaining > 0.0f) {
        wind->gustFramesRemaining -= frameAmount;
    } else {
        if (!g_sakuraWindNoiseInitialized)
            WIND_InitializeSakuraWindState();
        const float gust = GetValueFromSakuraLUT(wind->currentStrength);
        const std::int32_t gustOffset = pickSakuraWindAngle(gust);
        const float noise = nextSakuraWindNoise() * 1600.0f;
        const std::int32_t foldedOffset = gustOffset >= 0x8000 ? (gustOffset ^ 0xffff) : gustOffset;
        wind->gustStrength = gust;
        wind->gustAngle = static_cast<std::uint16_t>(gustOffset + wind->currentAngle);
        wind->gustFramesRemaining = static_cast<float>(static_cast<std::int32_t>(noise) - foldedOffset / 1024);
    }

    const float scaledStrength = wind->gustStrength / wind->strengthDivisor;
    const std::uint16_t angle = static_cast<std::uint16_t>(wind->gustAngle);
    const std::int32_t foldedAngle = angle <= 0x8000U ? static_cast<std::int32_t>(angle) : 0x10000 - static_cast<std::int32_t>(angle);
    wind->gustVectorZ = -sin_fixed16(static_cast<std::int16_t>(angle)) * scaledStrength * 0.5f;
    wind->gustVectorX = sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedAngle)) * scaledStrength * 0.5f;
}

void WIND_UpdateWeatherData() {
    Weather_Wind_UpdateFromWDAT(g_weatherWindData);
    for (WeatherWindRegistryNode *node = g_weatherWindRegistry; node; node = node->next) {
        Weather_Wind_UpdateFromWDAT(node->wind);
    }
}

void WIND_ResetWeatherDataForScene() {
    g_weatherWindRegistry = nullptr;
    g_weatherWindData = nullptr;
}

void WIND_DestroyWeatherData() {
    while (g_weatherWindRegistry) {
        unregisterStandaloneWind(g_weatherWindRegistry);
    }
    EFFE_DetachWindData(g_weatherWindData);
    freeWindData(g_weatherWindData);
    g_weatherWindData = nullptr;
}

void WIND_Destroy() {
    if (g_windTask) {
        HLib_DestroyTask(g_windTask);
        g_windTask = nullptr;
    }
    WIND_DestroyWeatherData();
}


void WIND_DrawGlyphs(void *entryPointer, std::int64_t, std::int32_t skippedStates, std::int32_t previousIndex) {
    WindEntry &entry = *static_cast<WindEntry *>(entryPointer);
    if (!entry.cellBufferA || !entry.cellBufferB || entry.cellWidthPixels <= 0) {
        return;
    }

    std::int32_t columnOffset = 0;
    if (skippedStates != 0) {
        if (entry.rowEndColumns[0] > entry.rowEndColumns[1]) {
            columnOffset = entry.rowEndColumns[0] - entry.rowEndColumns[1];
        }
    } else if (entry.rowEndColumns[0] < entry.rowEndColumns[1]) {
        columnOffset = entry.rowEndColumns[1] - entry.rowEndColumns[0];
    }

    std::uint8_t *control = entry.cellBufferA + skippedStates * entry.cellWidthPixels;
    std::uint8_t *glyph = entry.cellBufferB + skippedStates * entry.cellWidthPixels;
    for (std::int32_t processed = 0; processed < entry.cellWidthPixels;) {
        std::int32_t step = 1;
        if (static_cast<std::int8_t>(*control) < 0) {
            *control &= 0x7fU;
            std::uint16_t glyphCode = 0;
            if ((*control & 1U) != 0) {
                if ((control[1] & 2U) != 0) {
                    control[1] &= 0x7fU;
                    step = 2;
                    glyphCode = static_cast<std::uint16_t>((static_cast<std::uint16_t>(glyph[0]) << 8) | glyph[1]);
                } else {
                    glyphCode = 0x0120;
                }
            } else {
                glyphCode = static_cast<std::uint16_t>(glyph[0] | 0x0100U);
            }
            const std::int32_t position = columnOffset + 2 * processed;
            stampGlyph(entry, glyphCode, static_cast<std::int16_t>(position / 92), static_cast<std::int16_t>(previousIndex % 2));
        }
        processed += step;
        control += step;
        glyph += step;
    }

    switch (entry.waitFlag) {
    case 0:
    case 2:
        uploadWindBitmap(entry, static_cast<std::uint16_t>(-547), 1585);
        break;
    case 1:
        uploadWindBitmap(entry, static_cast<std::uint16_t>(-547), 7);
        break;
    case 3:
        uploadWindBitmap(entry, static_cast<std::uint16_t>(-10), 0);
        break;
    default:
        break;
    }
}

void WIND_AspectRatioPass(void *entryPointer) {
    if (!g_updateRenderFrameMode)
        return;
    WindEntry &entry = *static_cast<WindEntry *>(entryPointer);
    if (!entry.rowDirtyFlags || entry.cellHeightPixels <= 0)
        return;

    entry.rowEndColumns[static_cast<std::int16_t>(entry.packedCursor >> 16)] = static_cast<std::uint8_t>(entry.packedCursor);
    const std::int32_t widest = std::max(entry.rowEndColumns[0], entry.rowEndColumns[1]);
    const std::int32_t left = 320 - 27 * widest / 4;
    const std::int32_t top = entry.rowEndColumns[1] ? 376 : 410;

    std::int32_t index = entry.rowScrollOrigin;
    for (std::int32_t visited = 0; visited < entry.cellHeightPixels; ++visited) {
        if ((entry.rowDirtyFlags[index] & 1U) != 0) {
            OriginalStrip quad{};
            quad.corner0 = 0xffffffffU;
            quad.normalizedY2 = 1.0f;
            quad.normalizedX3 = 1.0f;
            AdjustAspectRatioMatrix(&quad, 0, left, top, 1024, 128, 1);
            std::swap(quad.y0, quad.y2);
            std::swap(quad.y1, quad.y3);
            quad.normalizedY2 = 1.0f;
            quad.normalizedX3 = 1.0f;
        }
        if (++index >= entry.cellHeightPixels)
            index = 0;
    }
}

void Script_Weather_WindUtils(ScenarioContext *context, void *rawParameters) {
    if (!rawParameters)
        return;
    const auto *const parameters = static_cast<const std::uint32_t *>(rawParameters);
    switch (parameters[0]) {
    case 1:
        WIND_EnsureWeatherData();
        break;
    case 2:
        EFFE_DetachWindData(g_weatherWindData);
        freeWindData(g_weatherWindData);
        g_weatherWindData = nullptr;
        break;
    case 3:
        setWindAngle(g_weatherWindData, parameters[1]);
        break;
    case 4:
        setWindStrength(g_weatherWindData, parameters[1]);
        break;
    case 9: {
        if (!context || !context->stack)
            return;
        WeatherWindData *const wind = allocateWindData();
        if (!wind || !registerStandaloneWind(wind)) {
            freeWindData(wind);
            context->stack->accumulator = 0;
            return;
        }
        const std::uint32_t offset = windOffset(wind);
        context->stack->accumulator = offset;
        break;
    }
    case 10: {
        WeatherWindData *const wind = windFromOffset(parameters[1]);
        EFFE_DetachWindData(wind);
        WeatherWindRegistryNode *const node = findStandaloneWind(wind);
        if (node)
            unregisterStandaloneWind(node);
        else
            freeWindData(wind);
        break;
    }
    case 12:
        setWindAngle(windFromOffset(parameters[1]), parameters[2]);
        break;
    case 13:
        setWindStrength(windFromOffset(parameters[1]), parameters[2]);
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
