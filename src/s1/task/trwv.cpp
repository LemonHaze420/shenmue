#include "s1/task/trwv.hpp"

#include "s1/boot.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/date_time.hpp"
#include "s1/file_loader.hpp"
#include "s1/hlib.hpp"
#include "s1/input.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"
#include "s1/mt5_render.hpp"
#include "s1/pvr_decoder.hpp"
#include "s1/render_array.hpp"
#include "s1/scene.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/scen.hpp"
#include "s1/timing.hpp"
#include "d3t/image_texture_loader.hpp"
#include "common/utility.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

namespace shenmue::s1 {
namespace {

constexpr std::int32_t kTrwvToken = static_cast<std::int32_t>(SHEN_NAME(TRWV));
constexpr float kPi = 3.1415927f;
constexpr double kTwoPi = 6.283185482025146;

std::array<TransformedWaveform *, 2> g_transformedWaveforms{};
// g_TRWV_pvrEmitters: the six emitters' static configs. The loader and the per-frame callback rewrite them in place (origin Y becomes the wave
// height, the sway, the system slot and the particles).
std::array<TrwvBowSprayEmitter, 6> g_trwvBowSprayEmitters{{
    {{96.2f, 0.01f, 109.91f}, {0.4f, 0.3f, 0.4f}, {0.2f, 0.06f, -0.15f}, {0.002f, 0.01f, 0.002f}, 0.6f, 1.73f, 1.65f, 0.6f, 0, 100, nullptr},
    {{95.5f, 0.01f, 109.91f}, {0.4f, 0.3f, 0.4f}, {-0.2f, 0.06f, -0.15f}, {0.002f, 0.01f, 0.002f}, 0.6f, 1.73f, 1.65f, 0.6f, 0, 100, nullptr},
    {{98.9f, 0.0f, 107.7f}, {0.4f, 0.3f, 0.4f}, {0.15f, 0.03f, -0.18f}, {0.002f, 0.01f, 0.002f}, 0.6f, 1.49f, 1.02f, 0.6f, 0, 75, nullptr},
    {{92.6f, 0.0f, 107.7f}, {0.4f, 0.3f, 0.4f}, {-0.15f, 0.03f, -0.18f}, {0.002f, 0.01f, 0.002f}, 0.6f, 1.49f, 1.02f, 0.6f, 0, 75, nullptr},
    {{100.8f, 0.0f, 105.0f}, {0.4f, 0.3f, 0.4f}, {0.09f, 0.03f, -0.22f}, {0.002f, 0.01f, 0.002f}, 0.6f, 1.1f, 0.86f, 0.6f, 0, 50, nullptr},
    {{90.5f, 0.0f, 105.0f}, {0.4f, 0.3f, 0.4f}, {-0.09f, 0.03f, -0.22f}, {0.002f, 0.01f, 0.002f}, 0.6f, 1.1f, 0.86f, 0.6f, 0, 50, nullptr},
}};
// g_tmp_TRWV_0/g_tmp_TRWV: the ship origin the emitters are placed against; g_tmp_TRWV_1: the spray colour (packed ARGB).
Vector3 g_trwvBowSprayOrigin{};
std::uint32_t g_trwvBowSprayColour = 0;
void *g_trwvBowSprayFile = nullptr;
std::uint32_t g_transformFlags = 0;
std::int32_t g_transformFrame = 0;
std::int32_t g_transformFrameLimit = 0;
HLib::Task *g_trwvTask = nullptr;

std::array<TerrainPatch, 2> g_terrainPatchData{};
std::array<TerrainColourEntry, 289> g_terrainColourBuffer{};
std::array<RuntimeEffectTexture, 4> g_runtimeEffectTextures{};
std::array<WaveComplex, 16> g_fftTwiddleFactorTable{};
std::array<WaveComplex, 16> g_fftTwiddleFactorTable2{};
float *g_trwvVelocityTable = nullptr;
float *g_trwvDistanceTable = nullptr;
Vector3 g_duskColourOffset{};
std::int32_t g_duskPreviousMinute = 0;
const Vector3 *g_activeWaveVertices = nullptr;
std::int32_t g_waveHitVertex = -1;
Vector3 g_waveQueryPoint{};
TrwvRippleList g_trwvRipples{};
TrwvRippleList *g_trwvActiveRipples = nullptr;
std::int32_t g_trwvRippleFrame = 0;

template <typename T> T *TaskDataPointer(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(g_pCurrentTaskData + offset) : nullptr;
}

template <typename T> T *ScenarioPointer(std::uint32_t offset) {
    return offset ? reinterpret_cast<T *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) : nullptr;
}

std::int32_t RelativeToTaskData(const std::byte *pointer) {
    return static_cast<std::int32_t>(pointer - g_pCurrentTaskData);
}


std::int32_t TruncateToInt32(float value) {
    if (!(value >= -2147483648.0f && value < 2147483648.0f))
        return INT32_MIN;
    return static_cast<std::int32_t>(value);
}


std::uint32_t InputSlotColour(std::size_t slot, std::size_t offset) {
    const auto *const bytes = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[slot]) + offset;
    return (static_cast<std::uint32_t>(bytes[0]) << 24U) | (static_cast<std::uint32_t>(bytes[1]) << 16U) | (static_cast<std::uint32_t>(bytes[2]) << 8U) |
        bytes[3];
}

void Generate16PointTwiddles() {
    constexpr float kAngleStep = 0.39269909f;
    constexpr float kHalfPi = 1.5707964f;
    std::int32_t index = -4;
    for (std::int32_t twiddle = 0; twiddle < 8; ++twiddle) {
        std::int32_t step = 4;
        while (step <= index) {
            index -= step;
            step >>= 1;
        }
        index += step;
        const float angle = static_cast<float>(twiddle) * kAngleStep;
        const float cosine = std::sin(kHalfPi - static_cast<float>(std::fabs(static_cast<double>(angle))));
        const float sine = std::sin(angle);
        g_fftTwiddleFactorTable2[static_cast<std::size_t>(index)] = {cosine, sine};
        g_fftTwiddleFactorTable[static_cast<std::size_t>(index)] = {cosine, -sine};
    }
}

} // namespace

void Generate32PointTwiddles() {
    constexpr float kAngleStep = 0.19634955f;
    constexpr float kHalfPi = 1.5707964f;
    std::int32_t index = -8;
    for (std::int32_t twiddle = 0; twiddle < 16; ++twiddle) {
        std::int32_t step = 8;
        while (step <= index) {
            index -= step;
            step >>= 1;
        }
        index += step;
        const float angle = static_cast<float>(twiddle) * kAngleStep;
        const float cosine = std::sin(kHalfPi - static_cast<float>(std::fabs(static_cast<double>(angle))));
        const float sine = std::sin(angle);
        g_fftTwiddleFactorTable2[static_cast<std::size_t>(index)] = {cosine, sine};
        g_fftTwiddleFactorTable[static_cast<std::size_t>(index)] = {cosine, -sine};
    }
}

namespace {

float NextSpectrumPhase(std::uint32_t &seed) {
    seed = LcgStep(seed);
    return static_cast<float>(static_cast<double>((seed >> 16U) & 0x7fffU) * 0.000030517578125) * 6.2831855f;
}

// PRNG_related_0: 16 sequential LCG draws per spectrum row, phase0 then phase1 of each record.
void TRWave_SeedSpectrumPhases(std::array<WaveSpectrumRecord, 64> &spectrum) {
    std::uint32_t seed = g_EnvPrngSeed;
    for (WaveSpectrumRecord &record : spectrum) {
        record.phase0 = NextSpectrumPhase(seed);
        record.phase1 = NextSpectrumPhase(seed);
    }
    g_EnvPrngSeed = seed;
}

float *GenerateDistanceLogLookupTable(float frequency) {
    float *const table = g_trwvDistanceTable;
    const float exponent = frequency * 0.5f;
    table[0] = 0.0f;
    float *output = table + 1;
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = row == 0 ? 1 : 0; column < 8; ++column)
            *output++ = 100.0f / fast_powf_approx(std::sqrt(static_cast<float>(column * column + row * row)), exponent);
    }
    return table;
}

float *ScaleLogTableEntries(float scale, float *table) {
    constexpr float kEpsilon = 1.1920929e-07f;
    table[0] = 0.0f;
    for (std::size_t index = 1; index < 64; ++index) {
        const float value = scale * table[index];
        table[index] = value;
        if (kEpsilon > value && value > -kEpsilon)
            table[index] = kEpsilon;
    }
    return table;
}

void TRWave_SetSpectrumAmplitudes(std::int32_t slot, const float *amplitudes) {
    TerrainPatch &patch = g_terrainPatchData[static_cast<std::size_t>(slot)];
    for (std::size_t index = 0; index < 64; ++index)
        patch.spectrum[index].amplitude = amplitudes[index];
}

void TRWave_SetSpectrumVelocities(std::int32_t slot, const float *velocities) {
    TerrainPatch &patch = g_terrainPatchData[static_cast<std::size_t>(slot)];
    for (std::size_t index = 0; index < 64; ++index) {
        patch.spectrum[index].velocity0 = velocities[index * 2U];
        patch.spectrum[index].velocity1 = velocities[index * 2U + 1U];
    }
}

void FillVelocityTable(const TransformedWaveform &waveform) {
    for (std::size_t index = 0; index < 128; index += 2) {
        g_trwvVelocityTable[index] = waveform.velocityX;
        g_trwvVelocityTable[index + 1U] = waveform.velocityZ;
    }
}

// PreprocessDataForFFT: record (row, column) seeds (row, column) and its conjugate mirror from phase0, (row, 16 - column) and
// (16 - row, column) from phase1. Column 16 and row 16 are the alignment copies, so the o == 0 / k == 0 mirrors land outside the
// 16x16 transform.
void PreprocessDataForFFT(float *real, float *imaginary, const std::array<WaveSpectrumRecord, 64> &spectrum) {
    constexpr float kFixed16PerRadian = 10430.378f;
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
            const WaveSpectrumRecord &record = spectrum[static_cast<std::size_t>(row * 8 + column)];
            const auto angle0 = static_cast<std::uint16_t>(static_cast<std::int32_t>(record.phase0 * kFixed16PerRadian));
            const std::uint32_t folded0 = angle0 <= 0x8000U ? angle0 : 0x10000U - angle0;
            const float cosine0 = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded0)) * record.amplitude;
            const float sine0 = sin_fixed16(static_cast<std::int16_t>(angle0)) * record.amplitude;
            real[row * 17 + column] = cosine0;
            imaginary[row * 17 + column] = sine0;
            real[288 - row * 17 - column] = cosine0;
            imaginary[288 - row * 17 - column] = -sine0;
            const auto angle1 = static_cast<std::uint16_t>(static_cast<std::int32_t>(record.phase1 * kFixed16PerRadian));
            const std::uint32_t folded1 = angle1 <= 0x8000U ? angle1 : 0x10000U - angle1;
            const float cosine1 = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded1)) * record.amplitude;
            const float sine1 = sin_fixed16(static_cast<std::int16_t>(angle1)) * record.amplitude;
            real[16 + row * 17 - column] = cosine1;
            imaginary[16 + row * 17 - column] = sine1;
            real[272 - row * 17 + column] = cosine1;
            imaginary[272 - row * 17 + column] = -sine1;
        }
    }
    real[0] = 0.0f;
    imaginary[0] = 0.0f;
    real[16] = 0.0f;
    imaginary[16] = 0.0f;
    real[272] = 0.0f;
    imaginary[272] = 0.0f;
    real[288] = 0.0f;
    imaginary[288] = 0.0f;
}

void ApplyButterflyPass_16Point(std::array<WaveComplex, 16> &values) {
    for (std::size_t group = 0; group < 4; ++group) {
        const WaveComplex w1 = g_fftTwiddleFactorTable[group * 2U];
        const WaveComplex w2 = g_fftTwiddleFactorTable[group * 2U + 1U];
        const WaveComplex s = g_fftTwiddleFactorTable[group];
        const WaveComplex a = values[group * 4U];
        const WaveComplex b = values[group * 4U + 1U];
        const WaveComplex c = values[group * 4U + 2U];
        const WaveComplex d = values[group * 4U + 3U];
        const float abReal = a.real - b.real;
        const float abImaginary = a.imaginary - b.imaginary;
        const WaveComplex p{abReal * w1.real + w1.imaginary * abImaginary, -w1.imaginary * abReal + w1.real * abImaginary};
        const float cdReal = c.real - d.real;
        const float cdImaginary = c.imaginary - d.imaginary;
        const WaveComplex q{cdReal * w2.real + w2.imaginary * cdImaginary, -w2.imaginary * cdReal + w2.real * cdImaginary};
        const WaveComplex abSum{a.real + b.real, a.imaginary + b.imaginary};
        const WaveComplex cdSum{c.real + d.real, c.imaginary + d.imaginary};
        values[group * 4U] = {abSum.real + cdSum.real, cdSum.imaginary + abSum.imaginary};
        const WaveComplex sumDifference{abSum.real - cdSum.real, abSum.imaginary - cdSum.imaginary};
        values[group * 4U + 2U] = {
            s.real * sumDifference.real + sumDifference.imaginary * s.imaginary, -s.imaginary * sumDifference.real + sumDifference.imaginary * s.real};
        values[group * 4U + 1U] = {p.real + q.real, q.imaginary + p.imaginary};
        const WaveComplex pqDifference{p.real - q.real, p.imaginary - q.imaginary};
        values[group * 4U + 3U] = {
            s.real * pqDifference.real + pqDifference.imaginary * s.imaginary, -s.imaginary * pqDifference.real + s.real * pqDifference.imaginary};
    }
}

void ApplyIFFTScalingAndReordering(std::array<WaveComplex, 16> &values) {
    constexpr float kScale = 0.0625f;
    for (std::size_t index = 0; index < 8; ++index) {
        const WaveComplex x = values[index];
        const WaveComplex y = values[index + 8U];
        values[index] = {(y.real + x.real) * kScale, (y.imaginary + x.imaginary) * kScale};
        values[index + 8U] = {(x.real - y.real) * kScale, (x.imaginary - y.imaginary) * kScale};
    }
}

} // namespace

void Run16PointFFTButterflyPasses(std::array<WaveComplex, 16> &values) {
    constexpr std::array<std::array<std::size_t, 2>, 6> kBitReversalSwaps{{{1, 8}, {2, 4}, {3, 12}, {5, 10}, {7, 14}, {11, 13}}};
    for (const std::array<std::size_t, 2> &pair : kBitReversalSwaps) {
        const WaveComplex swap = values[pair[0]];
        values[pair[0]] = values[pair[1]];
        values[pair[1]] = swap;
    }
    ApplyButterflyPass_16Point(values);
    for (std::size_t index = 0; index < 4; ++index) {
        const WaveComplex a = values[index];
        const WaveComplex b = values[index + 4U];
        values[index] = {b.real + a.real, b.imaginary + a.imaginary};
        values[index + 4U] = {a.real - b.real, a.imaginary - b.imaginary};
    }
    for (std::size_t index = 8; index < 12; ++index) {
        const WaveComplex a = values[index];
        const WaveComplex b = values[index + 4U];
        values[index] = {b.real + a.real, b.imaginary + a.imaginary};
        values[index + 4U] = {b.imaginary - a.imaginary, a.real - b.real};
    }
    ApplyIFFTScalingAndReordering(values);
}

namespace {

void UpdateSpectrumPhase(float &phase, float velocity, bool smoothing) {
    const float step = smoothing ? velocity * 30.0f * fFrameTimeMs : velocity;
    phase = step + phase;
    if (phase > kPi)
        phase = static_cast<float>(static_cast<double>(phase) - kTwoPi);
    else if (-kPi > phase)
        phase = static_cast<float>(static_cast<double>(phase) + kTwoPi);
}

void TRWave_NormalizeAndAlignFFTInput(float *real, float *imaginary, std::array<WaveSpectrumRecord, 64> &spectrum) {
    const bool smoothing = g_bEnableFrameTimeSmoothing != 0;
    for (WaveSpectrumRecord &record : spectrum) {
        UpdateSpectrumPhase(record.phase0, record.velocity0, smoothing);
        UpdateSpectrumPhase(record.phase1, record.velocity1, smoothing);
    }
    PreprocessDataForFFT(real, imaginary, spectrum);
    std::array<WaveComplex, 16> values{};
    for (std::size_t row = 0; row < 16; ++row) {
        for (std::size_t column = 0; column < 16; ++column)
            values[column] = {real[row * 17U + column], imaginary[row * 17U + column]};
        Run16PointFFTButterflyPasses(values);
        for (std::size_t column = 0; column < 16; ++column) {
            real[row * 17U + column] = values[column].real;
            imaginary[row * 17U + column] = values[column].imaginary;
        }
    }
    for (std::size_t column = 0; column < 16; ++column) {
        for (std::size_t row = 0; row < 16; ++row)
            values[row] = {real[row * 17U + column], imaginary[row * 17U + column]};
        Run16PointFFTButterflyPasses(values);
        for (std::size_t row = 0; row < 16; ++row) {
            real[row * 17U + column] = values[row].real;
            imaginary[row * 17U + column] = values[row].imaginary;
        }
    }
    for (std::size_t index = 0; index < 16; ++index) {
        real[index * 17U + 16U] = real[index * 17U];
        real[272U + index] = real[index];
    }
    real[288] = real[272];
    real[288] = real[16];
}

void SetTransformForEFFE() {
    EffectLightRenderSlot &light = g_effectLightRenderSlots[0];
    light.shaderConstants04[5] = light.colourR * light.intensity;
    light.shaderConstants04[6] = light.colourG * light.intensity;
    light.shaderConstants04[7] = light.colourB * light.intensity;
    Matrix4x4::Transform(light.negativeNormalizedDirection, light.transformedDirection);
    Matrix4x4::TransformPoint(light.position, light.transformedPosition);
}

std::uint32_t PackLitColour(std::uint32_t mask, float red, float green, float blue, float factor) {
    std::uint32_t colour = static_cast<std::uint32_t>(TruncateToInt32(red * factor)) << 8U;
    colour |= static_cast<std::uint32_t>(TruncateToInt32(green * factor));
    colour <<= 8U;
    colour |= static_cast<std::uint32_t>(TruncateToInt32(blue * factor));
    return colour | mask;
}

// Column 16 repeats column 0 and row 16 repeats row 0 in the strip-ordered colour entries.
void TRWave_CopyTerrainColourSeams() {
    g_terrainColourBuffer[32].diffuse = g_terrainColourBuffer[0].diffuse;
    g_terrainColourBuffer[32].specular = g_terrainColourBuffer[0].specular;
    g_terrainColourBuffer[33].diffuse = g_terrainColourBuffer[1].diffuse;
    g_terrainColourBuffer[33].specular = g_terrainColourBuffer[1].specular;
    for (std::size_t row = 0; row < 15; ++row) {
        g_terrainColourBuffer[50U + row * 17U].diffuse = g_terrainColourBuffer[34U + row * 17U].diffuse;
        g_terrainColourBuffer[50U + row * 17U].specular = g_terrainColourBuffer[34U + row * 17U].specular;
    }
    for (std::size_t column = 0; column < 17; ++column) {
        g_terrainColourBuffer[272U + column].diffuse = g_terrainColourBuffer[column * 2U].diffuse;
        g_terrainColourBuffer[272U + column].specular = g_terrainColourBuffer[column * 2U].specular;
    }
}

// ComputeVertexLighting: terrain colour entries in strip order (row 0/1 pairs, then rows 2..16), a flat normal per step from the
// neighbouring heights, and the wrap copies for column 16 and row 16.
void ComputeVertexLighting(std::int32_t slot, float gridSpacing, std::int32_t lightingMode) {
    constexpr float kAmbient = 0.3f;
    TerrainPatch &patch = g_terrainPatchData[static_cast<std::size_t>(slot)];
    const std::uint32_t mask = patch.colour & 0xff000000U;
    std::int32_t red = static_cast<std::int32_t>((patch.colour >> 16U) & 0xffU);
    std::int32_t green = static_cast<std::int32_t>((patch.colour >> 8U) & 0xffU);
    std::int32_t blue = static_cast<std::int32_t>(patch.colour & 0xffU);
    EffectLightRenderSlot &light = g_effectLightRenderSlots[0];
    Matrix4x4::RotateMatrixCounterClockwise90();
    Vector3 lightDirection{};
    Matrix4x4::Transform(light.transformedDirection, lightDirection);
    light.shaderConstants5C[3] = lightDirection.x;
    light.shaderConstants5C[4] = lightDirection.y;
    light.shaderConstants5C[5] = lightDirection.z;
    Matrix4x4::RotateMatrixCounterClockwise90();
    float ambient = kAmbient;
    if (lightingMode == 1) {
        ambient = g_sceneLightingActiveColour.x;
    } else if (lightingMode == 2) {
        const float scene = g_sceneLightingActiveColour.x;
        red = TruncateToInt32(static_cast<float>(red) * scene);
        blue = TruncateToInt32(static_cast<float>(blue) * scene);
        green = TruncateToInt32(static_cast<float>(green) * scene);
    }
    const float redF = static_cast<float>(red);
    const float greenF = static_cast<float>(green);
    const float blueF = static_cast<float>(blue);
    const float lightX = light.shaderConstants5C[3];
    const float lightY = light.shaderConstants5C[4];
    const float lightZ = light.shaderConstants5C[5];
    TerrainColourEntry *output = g_terrainColourBuffer.data();
    const float *heights = patch.real.data() + 17;

    const auto emit = [&](float height, float normalA, float normalB, float normalC) {
        const float length = std::sqrt(normalB * normalB + normalA * normalA + normalC * normalC);
        if (length > 0.0f) {
            const float inverse = 1.0f / length;
            normalA *= inverse;
            normalB *= inverse;
            normalC *= inverse;
        }
        output->height = height;
        if (lightingMode == 0) {
            const float brightness = ((normalA * lightX + normalB * lightY) + normalC * lightZ) + g_sceneLightingActiveColour.x;
            const float clamped = brightness > 1.0f ? 1.0f : 0.0f > brightness ? 0.0f : brightness;
            output->diffuse = PackLitColour(mask, redF, greenF, blueF, clamped);
            output->specular = 0xff000000U;
        } else {
            const float brightness = ((normalA * lightX + normalB * lightY) + normalC * lightZ) + ambient;
            const float clamped = brightness > 1.0f ? 1.0f : 0.0f > brightness ? 0.0f : brightness;
            output->diffuse = PackLitColour(mask, redF, greenF, blueF, clamped);
            if (0.37 > static_cast<double>(brightness)) {
                output->specular = mask;
            } else {
                const float power = fast_powf_approx(brightness, 16.0f);
                const float highlight = 1.0f < power ? 1.0f : power;
                output->specular = PackLitColour(mask, redF, greenF, blueF, highlight);
            }
        }
        ++output;
    };

    for (std::int32_t row = 0; row < 16; ++row) {
        const float yStart = static_cast<float>(row) * gridSpacing;
        const float yEnd = yStart + gridSpacing;
        for (std::int32_t column = 0; column < 17; ++column) {
            const float current = heights[0];
            const float top = heights[-17];
            const float next = heights[-16];
            const float xStart = static_cast<float>(column) * gridSpacing;
            const float xEnd = xStart + gridSpacing;
            if (row == 0) {
                const float dx = xEnd - xStart;
                const float dy = yEnd - yStart;
                const float dh = current - top;
                const float nh = next - top;
                const float normalA = nh * (xStart - xStart) - dx * dh;
                const float normalB = dx * dy - (yStart - yStart) * (xStart - xStart);
                const float normalC = (yStart - yStart) * dh - nh * dy;
                emit(top, normalA, normalB, normalC);
            }
            const float dy = yStart - yEnd;
            const float dx = xEnd - xStart;
            const float topDelta = top - current;
            const float nextDelta = next - current;
            const float normalB = dy * (xStart - xStart) - dx * dy;
            const float normalC = nextDelta * dy - dy * topDelta;
            const float normalA = dx * topDelta - nextDelta * (xStart - xStart);
            emit(current, normalA, normalB, normalC);
            ++heights;
        }
    }

    TRWave_CopyTerrainColourSeams();
}

std::int32_t WrapDirection(std::int32_t value, std::int32_t period) {
    if (value > period)
        return 1;
    return value < 0 ? -1 : 0;
}

std::uint32_t OffsetStripUV(std::uint32_t uv, std::int32_t offsetU, std::int32_t offsetV, std::int32_t wrapU, std::int32_t wrapV, std::int32_t period) {
    std::int32_t u = static_cast<std::int32_t>(uv >> 16U) + offsetU;
    std::int32_t v = static_cast<std::int32_t>(uv & 0xffffU) + offsetV;
    if (wrapU)
        u -= wrapU * period;
    if (wrapV)
        v -= wrapV * period;
    return (static_cast<std::uint32_t>(u) << 16U) | static_cast<std::uint16_t>(v);
}

// TRWave_ApplyTerrainColoursToStrips: publish the terrain colour entries into the strip vertices of every strip chunk and scroll their UVs. The first 17 steps
// of a chunk write vertex pairs; the colour cursor runs on across chunks and type-7 chunks replace the UV period.
void TRWave_ApplyTerrainColoursToStrips(std::int32_t period, std::int32_t textureSlot, std::int32_t offsetU, std::int32_t offsetV, EffeGeometryMesh *mesh) {
    std::byte *chunk = TaskDataPointer<std::byte>(mesh->chunkOffset);
    const TerrainColourEntry *colour = g_terrainColourBuffer.data();
    const RuntimeEffectTexture &texture = g_runtimeEffectTextures[static_cast<std::size_t>(textureSlot)];
    while (true) {
        std::int32_t header = 0;
        std::memcpy(&header, chunk, sizeof(header));
        const std::int32_t size = header >> 8;
        const std::uint32_t type = static_cast<std::uint32_t>(header) & 0xffU;
        if (type > 5U) {
            if (type == 7U) {
                std::memcpy(&period, chunk + 4, sizeof(period));
                chunk += 8;
                continue;
            }
            if (type == 0xffU)
                break;
            chunk += size;
            continue;
        }
        std::uint32_t firstUV = 0;
        std::memcpy(&firstUV, chunk + 0x34, sizeof(firstUV));
        const std::int32_t wrapU = WrapDirection(static_cast<std::int32_t>(firstUV >> 16U) + offsetU, period);
        const std::int32_t wrapV = WrapDirection(static_cast<std::int32_t>(firstUV & 0xffffU) + offsetV, period);
        auto *const geometry = reinterpret_cast<EffeGeometryChunk *>(chunk);
        geometry->typeAndSize = (size << 8) | ((texture.flags & 1U) != 0U ? 3 : 4);
        for (std::size_t word = 0; word < 4; ++word)
            geometry->textureWords[word] = texture.words[word];
        std::int32_t stepsLeft = 17;
        std::byte *cursor = chunk + sizeof(EffeGeometryChunk);
        for (std::int32_t strip = geometry->stripCount; strip; --strip) {
            std::int32_t count = 0;
            std::memcpy(&count, cursor, sizeof(count));
            cursor += 4;
            std::uint32_t remaining = count < 0 ? 0U - static_cast<std::uint32_t>(count) : static_cast<std::uint32_t>(count);
            while (remaining) {
                --remaining;
                std::uint32_t first = 0;
                std::memcpy(&first, cursor, sizeof(first));
                cursor += 4;
                if (stepsLeft <= 0) {
                    if ((first & 1U) != 0U) {
                        std::memcpy(cursor, &colour->height, 4);
                        std::uint32_t uv = 0;
                        std::memcpy(&uv, cursor + 0x14, sizeof(uv));
                        uv = OffsetStripUV(uv, offsetU, offsetV, wrapU, wrapV, period);
                        std::memcpy(cursor + 0x14, &uv, sizeof(uv));
                        std::memcpy(cursor + 0x18, &colour->diffuse, 4);
                        std::memcpy(cursor + 0x1c, &colour->specular, 4);
                        cursor += 0x20;
                        ++colour;
                    }
                    continue;
                }
                if ((first & 1U) != 0U) {
                    std::memcpy(cursor, &colour[0].height, 4);
                    std::uint32_t uv = 0;
                    std::memcpy(&uv, cursor + 0x14, sizeof(uv));
                    uv = OffsetStripUV(uv, offsetU, offsetV, wrapU, wrapV, period);
                    std::memcpy(cursor + 0x14, &uv, sizeof(uv));
                    std::memcpy(cursor + 0x18, &colour[0].diffuse, 4);
                    std::memcpy(cursor + 0x1c, &colour[0].specular, 4);
                    std::memcpy(cursor + 0x24, &colour[1].height, 4);
                    std::memcpy(&uv, cursor + 0x38, sizeof(uv));
                    uv = OffsetStripUV(uv, offsetU, offsetV, wrapU, wrapV, period);
                    std::memcpy(cursor + 0x38, &uv, sizeof(uv));
                    std::memcpy(cursor + 0x3c, &colour[1].diffuse, 4);
                    std::memcpy(cursor + 0x40, &colour[1].specular, 4);
                    colour += 2;
                    cursor += 0x44;
                    --remaining;
                }
                --stepsLeft;
            }
        }
        chunk += size;
    }
}

// TRWave_BuildStripUVs (script command 21): strip UVs from a 17x17 grid of (row * 256 / (16 / rows)) << 16 | column * 256 / (16 / columns),
// consumed in the same pair-then-single order, and a 256 period in every type-7 chunk.
void TRWave_BuildStripUVs(std::int32_t rowDivisions, std::int32_t columnDivisions, EffeGeometryMesh *mesh) {
    std::byte *chunk = TaskDataPointer<std::byte>(mesh->chunkOffset);
    std::array<std::uint32_t, 289> table{};
    const float rowScale = static_cast<float>(16 / rowDivisions);
    const float columnScale = static_cast<float>(16 / columnDivisions);
    for (std::size_t row = 0; row < 17; ++row) {
        const auto rowValue =
            static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(static_cast<float>(row)) * 256.0 / static_cast<double>(rowScale)));
        for (std::size_t column = 0; column < 17; ++column) {
            const auto columnValue = static_cast<std::uint16_t>(
                static_cast<std::int32_t>(static_cast<double>(static_cast<float>(column)) * 256.0 / static_cast<double>(columnScale)));
            table[row * 17U + column] = (static_cast<std::uint32_t>(static_cast<std::int32_t>(rowValue)) << 16U) | columnValue;
        }
    }
    const std::uint32_t *first = table.data();
    const std::uint32_t *second = table.data() + 17;
    while (true) {
        std::int32_t header = 0;
        std::memcpy(&header, chunk, sizeof(header));
        const std::uint32_t type = static_cast<std::uint32_t>(header) & 0xffU;
        if (type > 5U) {
            if (type == 7U) {
                const std::int32_t period = 256;
                std::memcpy(chunk + 4, &period, sizeof(period));
                chunk += 8;
                continue;
            }
            if (type == 0xffU)
                break;
            chunk += header >> 8;
            continue;
        }
        std::int32_t stripCount = 0;
        std::memcpy(&stripCount, chunk + 0x14, sizeof(stripCount));
        std::int32_t stepsLeft = 17;

        std::byte *&cursor = chunk;
        cursor += 0x18;
        while (stripCount) {
            std::int32_t count = 0;
            std::memcpy(&count, cursor, sizeof(count));
            --stripCount;
            cursor += 4;
            std::uint32_t remaining = count < 0 ? 0U - static_cast<std::uint32_t>(count) : static_cast<std::uint32_t>(count);
            while (remaining) {
                --remaining;
                std::uint32_t vertex = 0;
                std::memcpy(&vertex, cursor, sizeof(vertex));
                cursor += 4;
                if (stepsLeft <= 0) {
                    if ((vertex & 1U) != 0U) {
                        std::memcpy(cursor + 0x14, second++, 4);
                        cursor += 0x20;
                    }
                    continue;
                }
                if ((vertex & 1U) != 0U) {
                    std::memcpy(cursor + 0x14, first++, 4);
                    std::memcpy(cursor + 0x38, second++, 4);
                    cursor += 0x44;
                    --remaining;
                }
                --stepsLeft;
            }
        }
    }
}

// The shore layers' heights: the FFT surface in strip order with an opaque white diffuse. Specular entries keep their previous values.
void TRWave_CopyUnlitHeights(std::int32_t slot) {
    const float *heights = g_terrainPatchData[static_cast<std::size_t>(slot)].real.data() + 17;
    TerrainColourEntry *output = g_terrainColourBuffer.data();
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 17; ++column) {
            if (row == 0) {
                output->height = heights[-17];
                output->diffuse = 0xffffffffU;
                ++output;
            }
            output->height = heights[0];
            output->diffuse = 0xffffffffU;
            ++output;
            ++heights;
        }
    }
    TRWave_CopyTerrainColourSeams();
}

// A shore alpha table is column-major (17 * column + row); its bytes replace the diffuse alpha of the strip-ordered colour entries.
void TRWave_ApplyShoreAlpha(const std::uint8_t *table) {
    TerrainColourEntry *entry = g_terrainColourBuffer.data();
    for (std::size_t column = 0; column < 17; ++column) {
        entry[0].diffuse = (entry[0].diffuse & 0x00ffffffU) | (static_cast<std::uint32_t>(table[17 * column]) << 24U);
        entry[1].diffuse = (entry[1].diffuse & 0x00ffffffU) | (static_cast<std::uint32_t>(table[17 * column + 1]) << 24U);
        entry += 2;
    }
    for (std::size_t row = 2; row < 17; ++row) {
        for (std::size_t column = 0; column < 17; ++column) {
            entry->diffuse = (entry->diffuse & 0x00ffffffU) | (static_cast<std::uint32_t>(table[17 * column + row]) << 24U);
            ++entry;
        }
    }
}

// Every full strip vertex takes the next colour entry's height, diffuse and specular, with no pair pass and no UV change. Chunk headers take the
// texture record's descriptor as in TRWave_ApplyTerrainColoursToStrips, and every other chunk is skipped by its size.
void TRWave_ApplyTerrainColoursToStripVertices(std::int32_t textureSlot, EffeGeometryMesh *mesh) {
    std::byte *chunk = TaskDataPointer<std::byte>(mesh->chunkOffset);
    const TerrainColourEntry *colour = g_terrainColourBuffer.data();
    const RuntimeEffectTexture &texture = g_runtimeEffectTextures[static_cast<std::size_t>(textureSlot)];
    while (true) {
        std::int32_t header = 0;
        std::memcpy(&header, chunk, sizeof(header));
        const std::int32_t size = header >> 8;
        const std::uint32_t type = static_cast<std::uint32_t>(header) & 0xffU;
        if (type == 0xffU)
            break;
        if (type <= 5U) {
            auto *const geometry = reinterpret_cast<EffeGeometryChunk *>(chunk);
            geometry->typeAndSize = (size << 8) | ((texture.flags & 1U) != 0U ? 3 : 4);
            for (std::size_t word = 0; word < 4; ++word)
                geometry->textureWords[word] = texture.words[word];
            std::byte *cursor = chunk + sizeof(EffeGeometryChunk);
            for (std::int32_t strip = geometry->stripCount; strip; --strip) {
                std::int32_t count = 0;
                std::memcpy(&count, cursor, sizeof(count));
                cursor += 4;
                for (std::uint32_t remaining = count < 0 ? 0U - static_cast<std::uint32_t>(count) : static_cast<std::uint32_t>(count); remaining; --remaining) {
                    std::uint32_t first = 0;
                    std::memcpy(&first, cursor, sizeof(first));
                    cursor += 4;
                    if ((first & 1U) == 0U)
                        continue;
                    std::memcpy(cursor, &colour->height, 4);
                    std::memcpy(cursor + 0x18, &colour->diffuse, 4);
                    std::memcpy(cursor + 0x1c, &colour->specular, 4);
                    cursor += 0x20;
                    ++colour;
                }
            }
        }
        chunk += size;
    }
}

// Scrolls the strip UVs as (offsetV + uv) | ((offsetU << 16) + (uv & 0xffff0000)), pairs for the first 17 steps of a chunk. Like
// TRWave_BuildStripUVs, the walk resumes at the end of the last strip rather than at the chunk size.
void TRWave_OffsetStripUVs(std::int32_t offsetU, std::int32_t offsetV, EffeGeometryMesh *mesh) {
    const auto offset = [offsetU, offsetV](std::byte *field) {
        std::uint32_t uv = 0;
        std::memcpy(&uv, field, sizeof(uv));
        const std::int32_t v = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(uv));
        uv = (static_cast<std::uint32_t>(offsetV) + static_cast<std::uint32_t>(v)) | ((static_cast<std::uint32_t>(offsetU) << 16U) + (uv & 0xffff0000U));
        std::memcpy(field, &uv, sizeof(uv));
    };
    std::byte *cursor = TaskDataPointer<std::byte>(mesh->chunkOffset);
    while (true) {
        std::int32_t header = 0;
        std::memcpy(&header, cursor, sizeof(header));
        const std::uint32_t type = static_cast<std::uint32_t>(header) & 0xffU;
        if (type > 5U) {
            if (type == 0xffU)
                break;
            cursor += header >> 8;
            continue;
        }
        std::int32_t stripCount = 0;
        std::memcpy(&stripCount, cursor + 0x14, sizeof(stripCount));
        std::int32_t stepsLeft = 17;
        cursor += 0x18;
        for (; stripCount; --stripCount) {
            std::int32_t count = 0;
            std::memcpy(&count, cursor, sizeof(count));
            cursor += 4;
            std::uint32_t remaining = count < 0 ? 0U - static_cast<std::uint32_t>(count) : static_cast<std::uint32_t>(count);
            while (remaining) {
                --remaining;
                std::uint32_t vertex = 0;
                std::memcpy(&vertex, cursor, sizeof(vertex));
                cursor += 4;
                if (stepsLeft <= 0) {
                    if ((vertex & 1U) != 0U) {
                        offset(cursor + 0x14);
                        cursor += 0x20;
                    }
                    continue;
                }
                if ((vertex & 1U) != 0U) {
                    offset(cursor + 0x14);
                    offset(cursor + 0x38);
                    cursor += 0x44;
                    --remaining;
                }
                --stepsLeft;
            }
        }
    }
}

void TRWV_RippleRest(TrwvRippleRecord *record) {
    record->alpha = 5;
}

void TRWV_RippleFall(TrwvRippleRecord *record) {
    const std::int32_t alpha = record->alpha - record->step;
    if (alpha >= 5) {
        record->alpha = static_cast<std::uint8_t>(alpha);
        return;
    }
    record->alpha = 5;
    record->handler = TRWV_RippleRest;
}

void TRWV_RippleFallHalfRate(TrwvRippleRecord *record) {
    if ((g_trwvRippleFrame & 1) != 0)
        return;
    const std::int32_t alpha = record->alpha - record->step;
    if (alpha >= 5) {
        record->alpha = static_cast<std::uint8_t>(alpha);
        return;
    }
    record->alpha = 5;
    record->handler = TRWV_RippleRest;
}

void TRWV_RippleSelectFall(TrwvRippleRecord *record) {
    const float distance = record->x * record->x + record->z * record->z;
    if (distance < 4.0f) {
        record->step = 1;
        record->handler = TRWV_RippleFall;
    } else if (distance < 19.0f) {
        record->step = 1;
        record->handler = TRWV_RippleFallHalfRate;
    } else {
        if (distance < 54.0f)
            record->step = 1;
        record->handler = TRWV_RippleRest;
    }
}

void TRWV_RippleRise(TrwvRippleRecord *record) {
    const std::int32_t alpha = record->alpha + record->step;
    if (record->limit >= alpha) {
        record->alpha = static_cast<std::uint8_t>(alpha);
        return;
    }
    record->handler = TRWV_RippleSelectFall;
}

// The rise step and peak alpha fall with the squared distance from the centre; points beyond 42 keep the initial step 1 / peak 5.
void TRWV_RippleStart(TrwvRippleRecord *record) {
    const float distance = record->x * record->x + record->z * record->z;
    if (!(distance >= 2.0f)) {
        record->step = 7;
        record->limit = 128;
    } else if (!(distance >= 4.0f)) {
        record->step = 6;
        record->limit = 107;
    } else if (!(distance >= 8.0f)) {
        record->step = 4;
        record->limit = 50;
    } else if (!(distance >= 12.0f)) {
        record->step = 3;
        record->limit = 30;
    } else if (!(distance >= 19.0f)) {
        record->step = 2;
        record->limit = 16;
    } else if (distance < 42.0f) {
        record->step = 1;
        record->limit = 5;
    }
    record->handler = TRWV_RippleRise;
}

void TRWV_InitRippleRecords(TrwvRippleRecord *records, float spacing) {
    g_trwvRipples.reserved00 = 0;
    g_trwvActiveRipples = &g_trwvRipples;
    g_trwvRipples.count = 289;
    g_trwvRipples.records = records;
    for (std::int32_t row = 0; row < 17; ++row) {
        const float x = static_cast<float>(row - 8) * spacing;
        for (std::int32_t column = 0; column < 17; ++column) {
            TrwvRippleRecord &record = *records++;
            record.x = x;
            record.gridIndex = static_cast<std::uint8_t>(column + row * 17);
            record.z = static_cast<float>(column - 8) * spacing;
            record.alpha = 5;
            record.step = 1;
            record.limit = 5;
            record.handler = TRWV_RippleStart;
        }
    }
}

// Up to four shore layers stepping back along -Z from the surface origin, each with its own alpha table. Only the first recomputes the unlit
// heights and honours the terrain slot colour; the others reuse those heights and rewrite their vertices without a UV scroll.
void TRWV_DrawShoreLayers(TransformedWaveform &waveform) {
    Matrix4x4_Push();
    Matrix4x4::preTranslate(waveform.position.x, static_cast<float>(static_cast<double>(waveform.position.y) + 0.01), waveform.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(waveform.yaw));
    Matrix4x4::preTranslate(waveform.localOffset.x, waveform.localOffset.y, waveform.localOffset.z);
    TerrainPatch &patch = g_terrainPatchData[static_cast<std::size_t>(waveform.terrainSlot)];
    const bool tuned = (waveform.flags & 1U) != 0U;
    for (std::int32_t layer = 0; layer < waveform.negativeExtentZ + 1 && layer < 4; ++layer) {
        const auto *const node = static_cast<const WaveModelNode *>(waveform.shoreModels[layer]);
        EffeGeometryMesh *const mesh = TaskDataPointer<EffeGeometryMesh>(node->meshOffset);
        if (layer == 0) {
            TRWave_CopyUnlitHeights(waveform.terrainSlot);
            patch.colour = tuned && (g_inputState.slots[2].held & 0x800U) != 0U ? InputSlotColour(2, 0x0c) : 0xffffffffU;
            TRWave_ApplyShoreAlpha(waveform.shoreAlphaTables);
            std::int32_t offsetU = waveform.activeShoreOffsetU;
            std::int32_t offsetV = waveform.activeShoreOffsetV;
            if (tuned && (g_inputState.slots[2].held & 0x2000U) != 0U) {
                const auto *const bytes = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[2]);
                offsetU = bytes[0x10] - 128;
                offsetV = bytes[0x11] - 128;
            }
            TRWave_ApplyTerrainColoursToStrips(node->meshOffset, waveform.auxiliaryTextureSlot, offsetU, offsetV, mesh);
        } else {
            TRWave_ApplyShoreAlpha(waveform.shoreAlphaTables + 289 * layer);
            TRWave_ApplyTerrainColoursToStrips(node->meshOffset, waveform.auxiliaryTextureSlot, waveform.activeShoreOffsetU, waveform.activeShoreOffsetV, mesh);
            TRWave_ApplyTerrainColoursToStripVertices(waveform.auxiliaryTextureSlot, mesh);
            patch.colour = tuned && (g_inputState.slots[2].held & 0x800U) != 0U ? InputSlotColour(2, 0x0c) : 0xffffffffU;
        }
        DrawForWeather(mesh);
        Matrix4x4::preTranslate(0.0f, 0.0f, -waveform.patchSizeZ);
    }
    Matrix4x4::Pop(1);
}

// The splash ripple: lit like the surface, with the ripple records' alpha, drawn twice just above it with the phase words scrolling U, then V.
void TRWV_DrawRippleSurface(TransformedWaveform &waveform) {
    const bool tunedColour = (waveform.flags & 1U) != 0U && (g_inputState.slots[2].held & 0x800U) != 0U;
    g_terrainPatchData[static_cast<std::size_t>(waveform.terrainSlot)].colour = tunedColour ? InputSlotColour(2, 0x0c) : 0xffffffffU;
    ComputeVertexLighting(waveform.terrainSlot, waveform.scale, static_cast<std::int32_t>((waveform.flags >> 27U) & 1U));
    const TrwvRippleList &ripples = *g_trwvActiveRipples;
    for (std::uint32_t index = 0; index < ripples.count; ++index) {
        TerrainColourEntry &entry = g_terrainColourBuffer[index];
        entry.diffuse = (entry.diffuse & 0x00ffffffU) | (static_cast<std::uint32_t>(ripples.records[index].alpha) << 24U);
    }
    const auto *const node = static_cast<const WaveModelNode *>(waveform.rippleModel);
    EffeGeometryMesh *const mesh = TaskDataPointer<EffeGeometryMesh>(node->meshOffset);
    TRWave_BuildStripUVs(waveform.gridDivisionsX, waveform.gridDivisionsZ, mesh);
    TRWave_OffsetStripUVs(static_cast<std::uint16_t>(waveform.phaseX), 0, mesh);
    TRWave_ApplyTerrainColoursToStripVertices(waveform.auxiliaryTextureSlot, mesh);
    Matrix4x4_Push();
    Matrix4x4::preTranslate(waveform.position.x, static_cast<float>(static_cast<double>(waveform.position.y) + 0.001), waveform.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(waveform.yaw));
    Matrix4x4::preTranslate(waveform.localOffset.x, waveform.localOffset.y, waveform.localOffset.z);
    DrawForWeather(mesh);
    Matrix4x4::Pop(1);
    TRWave_BuildStripUVs(waveform.gridDivisionsX, waveform.gridDivisionsZ, mesh);
    TRWave_OffsetStripUVs(0, static_cast<std::uint16_t>(waveform.phaseY), mesh);
    TRWave_ApplyTerrainColoursToStripVertices(waveform.auxiliaryTextureSlot, mesh);
    Matrix4x4_Push();
    Matrix4x4::preTranslate(waveform.position.x, static_cast<float>(static_cast<double>(waveform.position.y) + 0.002), waveform.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(waveform.yaw));
    Matrix4x4::preTranslate(waveform.localOffset.x, waveform.localOffset.y, waveform.localOffset.z);
    DrawForWeather(mesh);
    Matrix4x4::Pop(1);
}

// Floats twelve vertices of the attached node 0.04 above fixed vertex heights of the surface strips.
void TRWV_FloatAttachedNode(const TransformedWaveform &waveform) {
    constexpr std::uint32_t kFloatingVertices[12][2]{{0x388, 0x124}, {0x268, 0x1c}, {0x148, 0x34}, {0xa50, 0x4c}, {0x1500, 0x64}, {0x1fb0, 0x7c},
        {0x2b00, 0x94}, {0x2ba0, 0xac}, {0x2c40, 0xc4}, {0x2230, 0xdc}, {0x1780, 0xf4}, {0xcd0, 0x10c}};
    const auto *const model = static_cast<const WaveModelNode *>(waveform.model);
    const std::byte *const strips = TaskDataPointer<std::byte>(TaskDataPointer<EffeGeometryMesh>(model->meshOffset)->chunkOffset);
    const auto *const node = static_cast<const WaveModelNode *>(waveform.attachedNode);
    std::byte *const vertices = TaskDataPointer<std::byte>(TaskDataPointer<EffeGeometryMesh>(node->meshOffset)->vertexOffset);
    for (const std::uint32_t(&pair)[2] : kFloatingVertices) {
        float height = 0.0f;
        std::memcpy(&height, strips + pair[0], sizeof(height));
        const float floated = static_cast<float>(static_cast<double>(height) + 0.04);
        std::memcpy(vertices + pair[1], &floated, sizeof(floated));
    }
}

bool TRWave_IsPointInQuad(std::int32_t start, std::int32_t size) {
    if (g_waveHitVertex != -1)
        return false;
    const Vector3 *const vertices = g_activeWaveVertices;
    const Vector3 &a = vertices[start];
    const Vector3 &b = vertices[start + size];
    const Vector3 &c = vertices[start + 17 * size + size];
    const Vector3 &d = vertices[start + 17 * size];
    const Vector3 &point = g_waveQueryPoint;
    const auto side = [&point](const Vector3 &from, const Vector3 &to) {
        const float value = (point.x - from.x) * (to.z - from.z) - (point.z - from.z) * (to.x - from.x);
        return std::fabs(static_cast<double>(value)) <= 0.1 ? 0.0f : value;
    };
    const float ab = side(a, b);
    const float bc = side(b, c);
    const float cd = side(c, d);
    const float da = side(d, a);
    return (ab >= 0.0f && bc >= 0.0f && cd >= 0.0f && da >= 0.0f) || (ab <= 0.0f && bc <= 0.0f && cd <= 0.0f && da <= 0.0f);
}

void TRWave_FindQuadContainingPoint(std::int32_t start, std::int32_t size) {
    if (g_waveHitVertex != -1)
        return;
    while (size != 1) {
        size >>= 1;
        if (TRWave_IsPointInQuad(start, size))
            TRWave_FindQuadContainingPoint(start, size);
        if (TRWave_IsPointInQuad(start + size, size))
            TRWave_FindQuadContainingPoint(start + size, size);
        if (TRWave_IsPointInQuad(start + 18 * size, size))
            TRWave_FindQuadContainingPoint(start + 18 * size, size);
        start += 17 * size;
        if (!TRWave_IsPointInQuad(start, size) || g_waveHitVertex != -1)
            return;
    }
    g_waveHitVertex = start;
}

void TransformQueryPointToPatch(const Vector3 &point) {
    Matrix4x4_Push();
    Matrix4x4::InvertRigidTransform();
    Matrix4x4::TransformPoint(point, g_waveQueryPoint);
    Matrix4x4::Pop(1);
    g_waveHitVertex = -1;
}

bool TRWave_IsPointOverPatch(const Vector3 *vertices, const Vector3 &point) {
    g_activeWaveVertices = vertices;
    TransformQueryPointToPatch(point);
    return TRWave_IsPointInQuad(0, 16);
}

// TRWave_GetHeightAtPoint: height of the wave at a point over the patch quad found by the quadtree walk, returned in world space.
float TRWave_GetHeightAtPoint(std::int32_t slot, const Vector3 *vertices, const Vector3 &point) {
    g_activeWaveVertices = vertices;
    const auto *const patch = reinterpret_cast<const float *>(&g_terrainPatchData[static_cast<std::size_t>(slot)]);
    TransformQueryPointToPatch(point);
    for (const std::int32_t start : {0, 8, 144, 136}) {
        if (TRWave_IsPointInQuad(start, 8))
            TRWave_FindQuadContainingPoint(start, 8);
    }
    const std::int32_t hit = g_waveHitVertex;
    const float *const heights = patch + 346 + hit;
    const Vector3 *const vertex = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(vertices) + static_cast<std::ptrdiff_t>(hit) * 12);
    const Vector3 local = g_waveQueryPoint;
    float baseX = vertex[0].x;
    float baseHeight = heights[0];
    float baseZ = vertex[0].z;
    const float nearHeightDelta = heights[18] - local.y;
    const float nearXDelta = vertex[18].x - local.x;
    const float nearZDelta = vertex[18].z - local.z;
    const float distance0 =
        std::sqrt(((baseHeight - local.y) * (baseHeight - local.y) + (baseX - local.x) * (baseX - local.x)) + (baseZ - local.z) * (baseZ - local.z));
    const float distance1 = std::sqrt((nearHeightDelta * nearHeightDelta + nearXDelta * nearXDelta) + nearZDelta * nearZDelta);
    if (distance0 > distance1) {
        baseX = vertex[18].x;
        baseHeight = heights[18];
        baseZ = vertex[18].z;
    }
    const float a = heights[17] - baseHeight;
    const float b = heights[1] - baseHeight;
    const float p = vertex[17].x - baseX;
    const float q = vertex[17].z - baseZ;
    const float r = vertex[1].x - baseX;
    const float s = vertex[1].z - baseZ;
    const float denominator = p * s - q * r;
    const float n1 = a * r - p * b;
    const float n2 = q * b - a * s;
    const float height = baseHeight - ((local.z - baseZ) * n1 + (local.x - baseX) * n2) / denominator;
    Matrix4x4::TransformPoint({local.x, height, local.z}, g_waveQueryPoint);
    return g_waveQueryPoint.y;
}

// TRWV_QueryWaveHeight (script command 18): walk the tiles until the vertex file's patch contains the point.
float TRWV_QueryWaveHeight(std::int32_t slot, const Vector3 &point) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    const auto *const vertices = static_cast<const Vector3 *>(waveform.vertexFile);

    float result = 0.0f;
    Matrix4x4_Push();
    bool found = false;
    for (std::int32_t z = -waveform.negativeExtentZ; z < waveform.positiveExtentZ + 1 && !found; ++z) {
        for (std::int32_t x = -waveform.negativeExtentX; x < waveform.positiveExtentX + 1; ++x) {
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::preTranslate(static_cast<float>(x) * waveform.patchSizeX, 0.0f, static_cast<float>(z) * waveform.patchSizeZ);
            const Vector3 query = point;
            if (TRWave_IsPointOverPatch(vertices, query)) {
                const Vector3 heightQuery = point;
                result = TRWave_GetHeightAtPoint(waveform.terrainSlot, vertices, heightQuery);
                Matrix4x4::Pop(1);
                found = true;
                break;
            }
            Matrix4x4::Pop(1);
        }
    }
    Matrix4x4::Pop(1);
    return result;
}

// TRWV_ComputeDuskColourOffset: dusk offsets towards R 88, G 128, B 87 over 120 minutes.
void TRWV_ComputeDuskColourOffset(std::int32_t minutes, std::uint32_t colour) {
    const std::int32_t red = static_cast<std::int32_t>((colour >> 16U) & 0xffU) - 88;
    const std::int32_t green = static_cast<std::int32_t>((colour >> 8U) & 0xffU) - 128;
    const std::int32_t blue = static_cast<std::int32_t>(colour & 0xffU) - 87;
    if (static_cast<double>(static_cast<std::uint32_t>(minutes)) == 120.0) {
        g_duskColourOffset.z = static_cast<float>(red);
        g_duskColourOffset.x = static_cast<float>(blue);
        g_duskColourOffset.y = static_cast<float>(green);
        return;
    }
    // The product is widened as an unsigned 32-bit value before the float conversion.
    const auto scaled = [minutes](
                            std::int32_t delta) { return static_cast<float>(static_cast<std::int64_t>(static_cast<std::uint32_t>(delta * minutes))) / 120.0f; };
    g_duskColourOffset.z = scaled(red);
    g_duskColourOffset.y = scaled(green);
    g_duskColourOffset.x = scaled(blue);
}

std::uint32_t ApplyDuskColourOffset(std::uint32_t colour) {
    std::uint32_t red = (colour >> 16U) & 0xffU;
    std::uint32_t green = (colour >> 8U) & 0xffU;
    std::uint32_t blue = colour & 0xffU;
    if (red >= 0x10U)
        red -= static_cast<std::uint32_t>(TruncateToInt32(g_duskColourOffset.z));
    if (green >= 0x10U)
        green -= static_cast<std::uint32_t>(TruncateToInt32(g_duskColourOffset.y));
    if (blue >= 0x10U)
        blue -= static_cast<std::uint32_t>(TruncateToInt32(g_duskColourOffset.x));
    std::uint32_t result = ((colour >> 24U) << 8U) | (red & 0xffU);
    result = (result << 8U) | (green & 0xffU);
    return (result << 8U) | (blue & 0xffU);
}

void CalcWeather(TransformedWaveform &waveform) {
    if (!g_updateRenderFrameMode || !g_bShouldDraw)
        return;
    auto *const model = static_cast<WaveModelNode *>(waveform.model);
    if (!model || CachedAttachmentOwnsSubmission(model, 1.0f, workTransformMatrix))
        return;
    Vector3 savedLightDirection{};
    const std::uint32_t flags = waveform.flags;
    const bool updateSurface = (flags & 0x10000004U) == 0U || (flags & 8U) != 0U;
    const bool swapLight = updateSurface && (flags & 0x4000000U) != 0U && ((flags & 1U) == 0U || (g_inputState.slots[3].held & 0x8000U) == 0U);
    if (updateSurface) {
        if (swapLight) {
            savedLightDirection = LGHT_GetDataSlot(0)->direction;
            SetEffectTransformByIndex(0, &waveform.lightDirection);
        }
        SetTransformForEFFE();
        std::uint32_t colour = (flags & 1U) != 0U && (g_inputState.slots[2].held & 0x8000U) != 0U ? InputSlotColour(1, 0x0e) : waveform.colourArgb;
        const std::uint8_t hours = g_currentDateTime.hours;
        const auto minutes = static_cast<std::int8_t>(g_currentDateTime.minutes);
        if (static_cast<std::uint8_t>(hours - 17U) <= 1U) {
            if ((flags & 0x20000000U) == 0U) {
                if (minutes == g_duskPreviousMinute + 1)
                    TRWV_ComputeDuskColourOffset(hours == 18 ? minutes + 60 : minutes, colour);
                colour = ApplyDuskColourOffset(colour);
            }
        } else if (hours >= 19) {
            if ((flags & 0x20000000U) == 0U) {
                TRWV_ComputeDuskColourOffset(120, colour);
                colour = ApplyDuskColourOffset(colour);
            }
        } else {
            g_duskColourOffset = {};
        }
        g_duskPreviousMinute = minutes;
        g_terrainPatchData[static_cast<std::size_t>(waveform.terrainSlot)].colour = colour;
        const std::int32_t lightingMode = (waveform.flags & 0x8000000U) != 0U ? 1 : (waveform.flags & 0x2000000U) != 0U ? 2 : 0;
        ComputeVertexLighting(waveform.terrainSlot, waveform.scale, lightingMode);
        std::int32_t offsetU = waveform.activeStripOffsetU;
        std::int32_t offsetV = waveform.activeStripOffsetV;
        if ((waveform.flags & 1U) != 0U && (g_inputState.slots[3].held & 0x2000U) != 0U) {
            const auto *const bytes = reinterpret_cast<const std::uint8_t *>(&g_inputState.slots[3]);
            offsetU = bytes[0x10] - 128;
            offsetV = bytes[0x11] - 128;
        }
        TRWave_ApplyTerrainColoursToStrips(model->meshOffset, waveform.textureSlot, offsetU, offsetV, TaskDataPointer<EffeGeometryMesh>(model->meshOffset));
    }

    float maximumDistance = 9999.0f;
    float nearestDistance = 9999.0f;
    bool allCulled = true;
    const Matrix4x4 cameraTransform = g_tmp_cam_xform;
    const Vector3 cameraEye = g_CameraEyePos;
    const float radius = (waveform.scale * 16.0f) * 0.7071f;
    const Vector2 &viewScale = prevScreenSpaceToWorldScale;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(waveform.position.x, waveform.position.y, waveform.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(waveform.yaw));
    Matrix4x4::preTranslate(waveform.localOffset.x, waveform.localOffset.y, waveform.localOffset.z);
    Matrix4x4::preTranslate(
        -(static_cast<float>(waveform.negativeExtentX) * waveform.patchSizeX), 0.0f, -(static_cast<float>(waveform.negativeExtentZ) * waveform.patchSizeZ));
    if (-waveform.negativeExtentZ < waveform.positiveExtentZ + 1) {
        for (std::int32_t z = -waveform.negativeExtentZ; z < waveform.positiveExtentZ + 1; ++z) {
            Matrix4x4_Push();
            for (std::int32_t x = -waveform.negativeExtentX; x < waveform.positiveExtentX + 1; ++x) {
                Matrix4x4_Push();
                workTransformMatrix = cameraTransform;
                Matrix4x4::NormalizeAndSetTransformMatrix();
                Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
                EffeGeometryMesh *const mesh = TaskDataPointer<EffeGeometryMesh>(model->meshOffset);
                Vector3 worldCentre{};
                Matrix4x4::TransformPoint(mesh->centre, worldCentre);
                workTransformMatrix = cameraTransform;
                Vector3 view{};
                Matrix4x4::TransformPoint(worldCentre, view);
                view.z = view.z * -1.0f;
                const float depth = view.z + radius;
                bool inView = false;
                bool visible = false;
                if (depth >= 0.0f) {
                    const float halfWidth = depth * viewScale.x + radius;
                    const float halfHeight = depth * viewScale.y + radius;
                    if (view.x <= halfWidth && -halfWidth < view.x && view.y <= halfHeight && -halfHeight < view.y) {
                        inView = true;
                        visible = cameraEye.y >= 2.5f || shenIsLocalSphereVisibleInScreenDepthTable(worldCentre, radius);
                    }
                }
                Matrix4x4::Pop(1);
                if (visible) {
                    if ((waveform.flags & 4U) == 0U) {
                        const float dx = worldCentre.x - cameraEye.x;
                        const float dy = worldCentre.y - cameraEye.y;
                        const float dz = worldCentre.z - cameraEye.z;
                        nearestDistance = std::fmin(std::sqrt((dx * dx + dy * dy) + dz * dz), nearestDistance);
                    }
                    DrawForWeather(TaskDataPointer<EffeGeometryMesh>(model->meshOffset));
                }
                if (allCulled && inView)
                    allCulled = false;
                Matrix4x4::preTranslate(waveform.patchSizeX, 0.0f, 0.0f);
            }
            Matrix4x4::Pop(1);
            Matrix4x4::preTranslate(0.0f, 0.0f, waveform.patchSizeZ);
        }
        maximumDistance = nearestDistance;
    }
    Matrix4x4::Pop(1);
    if (waveform.shoreAlphaTables)
        TRWV_DrawShoreLayers(waveform);
    if (waveform.rippleActive)
        TRWV_DrawRippleSurface(waveform);

    if (swapLight) {
        SetEffectTransformByIndex(0, &savedLightDirection);
        SetTransformForEFFE();
    }
    if ((waveform.flags & 4U) == 0U)
        waveform.flags = allCulled || maximumDistance > 64.0f ? waveform.flags | 0x10000000U : waveform.flags & ~0x10000000U;
    if ((waveform.flags & 8U) != 0U)
        waveform.flags &= ~8U;
}

// While controller slot 3 holds 0x800, its stick and trigger bytes replace the velocities, frequency and amplitude.
void RefreshSpectrum(TransformedWaveform &waveform) {
    const InputSlot &tuning = g_inputState.slots[3];
    if ((tuning.held & 0x800U) != 0U) {
        const auto *const bytes = reinterpret_cast<const std::uint8_t *>(&tuning);
        if (!waveform.velocityFile) {
            for (std::size_t index = 0; index < 128; index += 2) {
                g_trwvVelocityTable[index] = static_cast<float>(bytes[0x0e] / 255.0 - 0.5);
                g_trwvVelocityTable[index + 1U] = static_cast<float>(bytes[0x0f] / 255.0 - 0.5);
            }
            TRWave_SetSpectrumVelocities(waveform.terrainSlot, g_trwvVelocityTable);
        }
        float *const table = GenerateDistanceLogLookupTable(static_cast<float>(bytes[0x0d] / 255.0 * 10.0));
        TRWave_SetSpectrumAmplitudes(waveform.terrainSlot, ScaleLogTableEntries(static_cast<float>(bytes[0x0c] / 255.0), table));
        return;
    }
    if (!waveform.velocityFile) {
        FillVelocityTable(waveform);
        TRWave_SetSpectrumVelocities(waveform.terrainSlot, g_trwvVelocityTable);
    }
    float *const table = GenerateDistanceLogLookupTable(waveform.frequency);
    TRWave_SetSpectrumAmplitudes(waveform.terrainSlot, ScaleLogTableEntries(waveform.amplitude, table));
}

void RunWaveFFT(TransformedWaveform &waveform) {
    TerrainPatch &patch = g_terrainPatchData[static_cast<std::size_t>(waveform.terrainSlot)];
    TRWave_NormalizeAndAlignFFTInput(patch.real.data(), patch.imaginary.data(), patch.spectrum);
}

void SetupTRWave(TransformedWaveform *waveform) {
    if (!waveform)
        return;
    const std::uint32_t flags = waveform->flags;
    if ((flags & 0x10000004U) != 0U && (flags & 8U) == 0U) {
        CalcWeather(*waveform);
        return;
    }
    waveform->phaseX = static_cast<std::int16_t>(waveform->phaseX + waveform->phaseStepX);
    waveform->phaseY = static_cast<std::int16_t>(waveform->phaseY + waveform->phaseStepY);
    if ((flags & 1U) != 0U)
        RefreshSpectrum(*waveform);
    if (waveform->rippleActive) {
        const TrwvRippleList &ripples = *g_trwvActiveRipples;
        TrwvRippleRecord *record = ripples.records;
        for (std::uint32_t index = 0; index < ripples.count; ++index, ++record)
            record->handler(record);
        ++g_trwvRippleFrame;
    }
    RunWaveFFT(*waveform);
    waveform->activeStripOffsetU = waveform->stripOffsetU;
    waveform->activeStripOffsetV = waveform->stripOffsetV;
    waveform->activeShoreOffsetU = waveform->shoreOffsetU;
    waveform->activeShoreOffsetV = waveform->shoreOffsetV;
    CalcWeather(*waveform);
    waveform->activeStripOffsetU = 0;
    waveform->activeStripOffsetV = 0;
    waveform->activeShoreOffsetU = 0;
    waveform->activeShoreOffsetV = 0;
    if (waveform->attachedNode)
        TRWV_FloatAttachedNode(*waveform);
}

// TRWave_DrawModelTree: the node tree of a mode-1 waveform, each mesh coloured and queued in its node transform.
void TRWave_DrawModelTree(std::int32_t textureSlot, std::int32_t offsetU, std::int32_t offsetV, WaveModelNode *node) {
    do {
        Matrix4x4_Push();
        Matrix4x4::preTranslate(node->posX, node->posY, node->posZ);
        const auto rotate = [](std::int32_t angle, void (*rotation)(float, float)) {
            if (!angle)
                return;
            const auto raw = static_cast<std::uint32_t>(angle);
            const std::uint16_t folded = raw <= 0x8000U ? static_cast<std::uint16_t>(raw) : static_cast<std::uint16_t>(0U - raw);
            const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
            rotation(sin_fixed16(static_cast<std::int16_t>(raw)), cosine);
        };
        rotate(node->rotZ, Matrix4x4::RotateAroundZ);
        rotate(node->rotY, Matrix4x4::RotateAroundY);
        rotate(node->rotX, Matrix4x4::RotateAroundX);
        if (node->scaleX != 1.0f || node->scaleY != 1.0f || node->scaleZ != 1.0f) {
            Matrix4x4::Scale(node->scaleX, node->scaleY, node->scaleZ);
            g_sceneObjectMaxScale = node->scaleX;
        }
        if (EffeGeometryMesh *const mesh = TaskDataPointer<EffeGeometryMesh>(node->meshOffset)) {

            TRWave_ApplyTerrainColoursToStrips(static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(mesh)), textureSlot, offsetU, offsetV, mesh);
            DrawForWeather(mesh);
        }
        if (WaveModelNode *const child = TaskDataPointer<WaveModelNode>(node->childOffset))
            TRWave_DrawModelTree(textureSlot, offsetU, offsetV, child);
        Matrix4x4::Pop(1);
        node = TaskDataPointer<WaveModelNode>(node->siblingOffset);
    } while (node);
}

void TRWV_DrawModelWave(TransformedWaveform &waveform) {
    if (!g_updateRenderFrameMode || !g_bShouldDraw)
        return;
    const std::uint32_t flags = waveform.flags;
    const bool swapLight = (flags & 0x4000000U) != 0U && ((flags & 1U) == 0U || (g_inputState.slots[3].held & 0x8000U) == 0U);
    Vector3 savedLightDirection{};
    if (swapLight) {
        savedLightDirection = LGHT_GetDataSlot(0)->direction;
        SetEffectTransformByIndex(0, &waveform.lightDirection);
        SetTransformForEFFE();
    }
    Matrix4x4_Push();
    Matrix4x4::preTranslate(waveform.position.x, waveform.position.y, waveform.position.z);
    const bool tunedColour = (flags & 1U) != 0U && (g_inputState.slots[2].held & 0x8000U) != 0U;
    g_terrainPatchData[static_cast<std::size_t>(waveform.terrainSlot)].colour = tunedColour ? InputSlotColour(1, 0x0e) : waveform.colourArgb;
    const std::int32_t lightingMode = (flags & 0x8000000U) != 0U ? 1 : (flags & 0x2000000U) != 0U ? 2 : 0;
    std::int32_t offsetU = waveform.activeStripOffsetU;
    std::int32_t offsetV = waveform.activeStripOffsetV;
    const InputSlot &tuning = g_inputState.slots[3];
    if ((flags & 1U) != 0U && (tuning.held & 0x2000U) != 0U) {
        const auto *const bytes = reinterpret_cast<const std::uint8_t *>(&tuning);
        const double level = bytes[0x0f] / 255.0;
        ComputeVertexLighting(waveform.terrainSlot, static_cast<float>(level * 4.0 * level), lightingMode);
        offsetU = bytes[0x10] - 128;
        offsetV = bytes[0x11] - 128;
    } else {
        ComputeVertexLighting(waveform.terrainSlot, waveform.scale, lightingMode);
    }
    TRWave_DrawModelTree(waveform.textureSlot, offsetU, offsetV, static_cast<WaveModelNode *>(waveform.model));
    Matrix4x4::Pop(1);
    if (swapLight) {
        SetEffectTransformByIndex(0, &savedLightDirection);
        Shenmue_InitEffectByIndexExact();
    }
}

void CalcTRWave(TransformedWaveform *waveform) {
    if (!waveform)
        return;
    if ((waveform->flags & 1U) != 0U) {
        const InputSlot &tuning = g_inputState.slots[3];
        const auto *const bytes = reinterpret_cast<const std::uint8_t *>(&tuning);
        const bool tuned = (tuning.held & 0x800U) != 0U;
        float *const table = GenerateDistanceLogLookupTable(tuned ? static_cast<float>(bytes[0x0d] / 255.0 * 10.0) : waveform->frequency);
        TRWave_SetSpectrumAmplitudes(waveform->terrainSlot, ScaleLogTableEntries(tuned ? static_cast<float>(bytes[0x0c] / 255.0) : waveform->amplitude, table));
    }
    RunWaveFFT(*waveform);
    waveform->activeStripOffsetU = waveform->stripOffsetU;
    waveform->activeStripOffsetV = waveform->stripOffsetV;
    TRWV_DrawModelWave(*waveform);
    waveform->activeStripOffsetU = 0;
    waveform->activeStripOffsetV = 0;
}

std::uint32_t GetTypeForResolution(std::uint32_t size) {
    switch (size) {
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return 0;
    }
}

// Recreates the data-less texture from the decoded, vertically flipped PVR, the raw source for families without PVR flags, or an encoded image in place.
std::uint32_t CreatePVRForDirectX(DecodedTextureFormat &out, const std::byte *source) {
    if ((out.extraParam & 0x80U) != 0U) {
        const std::size_t offset = source[0] == std::byte{'D'} ? 0U : 4U;
        return d3t::ReplaceDecodedImage(static_cast<std::uint32_t>(out.texture), source + offset, 0x100000U, true) ? 0U : 4U;
    }
    std::vector<std::byte> flipped;
    const std::byte *data = source;
    if ((out.extraParam & 0x10cU) != 0U) {
        std::uint32_t magic = 0;
        std::memcpy(&magic, source, sizeof(magic));
        if (magic == SHEN_NAME(GBIX)) {
            std::uint32_t offset = 0;
            std::memcpy(&offset, source + 4, sizeof(offset));
            source += 8 + offset;
            std::memcpy(&magic, source, sizeof(magic));
        }
        if (magic != SHEN_NAME(PVRT)) {
            source -= 16;
            std::memcpy(&magic, source, sizeof(magic));
        }
        const std::uint8_t pixelFormat = std::to_integer<std::uint8_t>(source[8]);

        if (magic != SHEN_NAME(PVRT) || pixelFormat > 6U)
            return 4;
        const std::size_t bits = pixelFormat <= 4U ? 32U : pixelFormat == 5U ? 4U : 8U;
        std::uint16_t width = 0;
        std::uint16_t height = 0;
        std::memcpy(&width, source + 0x0c, sizeof(width));
        std::memcpy(&height, source + 0x0e, sizeof(height));
        const std::size_t allocation = height * ((bits * width / 8U + 3U) & ~std::size_t{3});
        std::vector<std::byte> decoded(allocation);
        PVR_DecodeMain(decoded.data(), source);
        flipped.resize(allocation);
        const std::size_t row = width * (bits / 8U);
        for (std::size_t index = 0; index < height; ++index)
            std::memcpy(flipped.data() + index * row, decoded.data() + (height - index - 1U) * row, row);
        data = flipped.data();
    }
    Texture2DDescription description{};
    const std::uint32_t previous = static_cast<std::uint32_t>(out.texture);
    if (!GetTexture2DDescription(previous, &description))
        return 4;
    description.initialData = data;
    const std::uint32_t texture = CreateTexture2D(&description);
    InitTexture2DResource(previous);
    out.texture = texture;
    return 0;
}

bool CreateDynamicTexture(
    std::int32_t slot, std::uint32_t formatType, std::int32_t width, std::int32_t height, const std::byte *pvr, std::uint32_t textureFlags) {
    RuntimeEffectTexture &record = g_runtimeEffectTextures[static_cast<std::size_t>(slot)];
    if ((((formatType & 0xff00U) - 256U) & 0xfffffdffU) == 0U) {
        if ((formatType & 0xffU) == 1U) {
            formatType |= 0x89U;
        } else if ((formatType & 0xffU) == 2U) {
            formatType &= ~2U;
            formatType |= 0x89U;
        }
    }
    if (CreateDirectXTexture2D(
            record.format, static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height), static_cast<std::uint16_t>(formatType), nullptr) ||
        CreatePVRForDirectX(record.format, pvr))
        return false;
    const std::uint32_t resolution = (GetTypeForResolution(record.format.width) << 3U) | GetTypeForResolution(record.format.height);
    record.flags = textureFlags;
    record.words[3] = static_cast<std::uint32_t>(record.format.texture);
    if ((textureFlags & 1U) != 0U) {
        record.words[0] = 0x808c000eU;
        record.words[2] = resolution | 0x20002440U;
        record.words[1] = 0xca800000U;
    } else {
        record.words[0] = 0x8200000eU;
        record.words[2] = resolution | 0x94002440U;
        record.words[1] = 0xcb800000U;
    }
    if (((textureFlags >> 1U) & 1U) != 0U)
        record.words[2] &= 0xff3fffffU;
    else
        record.words[2] |= 0x800000U;
    if (((textureFlags >> 1U) & 2U) != 0U) {
        record.words[2] |= 0x1000c0U;
    } else {
        record.words[2] &= ~0x100000U;
        record.words[2] |= 0x40U;
    }
    return true;
}

void ReleaseRuntimeEffectTexture(std::int32_t slot) {
    if (slot > 3)
        return;
    RuntimeEffectTexture &record = g_runtimeEffectTextures[static_cast<std::size_t>(slot)];
    if (!record.format.texture)
        return;
    InitTexture2DResource(static_cast<std::uint32_t>(record.format.texture));
    record.format.texture = 0;
    record.words[0] = 0;
    record.words[1] = 0;
    --g_TextureRefCount;
}

void RelocateOffset(std::int32_t &offset, std::byte *file) {
    offset = offset ? RelativeToTaskData(file + offset) : 0;
}

// setup_trwave_ptrs: rebase every file-relative node and mesh offset onto g_pCurrentTaskData.
void setup_trwave_ptrs(WaveModelNode *node, std::byte *file) {
    if (node->meshOffset) {
        node->meshOffset = RelativeToTaskData(file + node->meshOffset);
        auto *const mesh = TaskDataPointer<EffeGeometryMesh>(node->meshOffset);
        if (mesh->vertexOffset)
            mesh->vertexOffset = RelativeToTaskData(file + mesh->vertexOffset);
        if (mesh->chunkOffset)
            mesh->chunkOffset = RelativeToTaskData(file + mesh->chunkOffset);
    }
    if (node->childOffset) {
        RelocateOffset(node->childOffset, file);
        setup_trwave_ptrs(TaskDataPointer<WaveModelNode>(node->childOffset), file);
    }
    if (node->siblingOffset) {
        RelocateOffset(node->siblingOffset, file);
        setup_trwave_ptrs(TaskDataPointer<WaveModelNode>(node->siblingOffset), file);
    }
    RelocateOffset(node->offset34, file);
    RelocateOffset(node->offset38, file);
    RelocateOffset(node->offset3C, file);
}

void LoadWater(const char *filename, void *model) {
    const auto handle = reinterpret_cast<std::uintptr_t>(model);
    if (!std::strcmp(filename, "trsea025.mt6")) {
        if (g_areaId == SHEN_NAME(OP00))
            d3t::FileRead_ALL(handle, "/scene/01/JHD0/iker1002.mt5_/iker1002.fbx", 0, 1);
        else if (g_areaId == SHEN_NAME(JHD0))
            d3t::FileRead_ALL(handle, "/scene/01/JHD0/iker1001.mt5_/iker1001.fbx", 0, 1);
        return;
    }
    const char *const dot = std::strchr(filename, '.');
    if (!dot || std::strlen(filename) >= 0x40)
        return;
    const auto stemLength = static_cast<std::size_t>(dot - filename);
    char stem[64]{};
    std::memcpy(stem, filename, stemLength);
    stem[stemLength] = '\0';
    char area[5]{};
    std::memcpy(area, &g_areaId, 4);
    char path[272]{};
    std::snprintf(path, sizeof(path), "/scene/maps/%s/%s_/%s.fbx", area, filename, stem);
    d3t::FileRead_ALL(handle, path, 0, 0x81);
}

void FreeEffects(std::int32_t slot) {
    TransformedWaveform *const waveform = g_transformedWaveforms[static_cast<std::size_t>(slot)];
    EFFE_ReleaseRegistryEntry(g_effeTerrainRegistry[static_cast<std::size_t>(waveform->terrainSlot)]);
    for (const std::int32_t textureSlot : {waveform->textureSlot, waveform->auxiliaryTextureSlot}) {
        if (textureSlot < 0)
            continue;
        ReleaseRuntimeEffectTexture(textureSlot);
        EFFE_ReleaseRegistryEntry(g_effeTextureRegistry[static_cast<std::size_t>(textureSlot)]);
    }
    if (waveform->model)
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->modelFile), 0);
    if (waveform->rippleModel)
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->rippleModelFile), 0);
    for (std::size_t index = 0; index < 4; ++index) {
        if (!waveform->shoreModels[index])
            continue;
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->shoreModelFiles[index]), 0);
        waveform->shoreModels[index] = nullptr;
    }

    if (waveform->rippleRecords)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(waveform->rippleRecords), 0);
    if (waveform->shoreAlphaTables)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(waveform->shoreAlphaTables), 0);
    if (waveform->velocityFile)
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->velocityFile), 0);
    if (waveform->vertexFile)
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->vertexFile), 0);
    if (waveform->rippleRecords)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(waveform->rippleRecords), 0);
    if (g_trwvDistanceTable) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_trwvDistanceTable), 0);
        g_trwvDistanceTable = nullptr;
    }
    if (g_trwvVelocityTable) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_trwvVelocityTable), 0);
        g_trwvVelocityTable = nullptr;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(waveform), 0);
    g_transformedWaveforms[static_cast<std::size_t>(slot)] = nullptr;
}

void TRWV_SetWaveShape(std::int32_t slot, float amplitude, float frequency) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    waveform.amplitude = amplitude;
    waveform.frequency = frequency * 10.0f;
    float *const table = GenerateDistanceLogLookupTable(waveform.frequency);
    TRWave_SetSpectrumAmplitudes(waveform.terrainSlot, ScaleLogTableEntries(waveform.amplitude, table));
}

void TRWV_SetWaveVelocity(std::int32_t slot, float velocityX, float velocityZ) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    waveform.velocityX = velocityX;
    waveform.velocityZ = velocityZ;
    if (waveform.velocityFile)
        return;
    FillVelocityTable(waveform);
    TRWave_SetSpectrumVelocities(waveform.terrainSlot, g_trwvVelocityTable);
}

void TRWV_SetWaveShapeAndVelocity(std::int32_t slot, float amplitude, float frequency, float velocity) {
    TRWV_SetWaveShape(slot, amplitude, frequency);
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    waveform.velocityX = velocity - 0.5f;
    waveform.velocityZ = waveform.velocityX;
    FillVelocityTable(waveform);
    TRWave_SetSpectrumVelocities(waveform.terrainSlot, g_trwvVelocityTable);
}

void TRWV_SetStripDivisions(std::int32_t slot, std::int32_t rowDivisions, std::int32_t columnDivisions) {
    const auto *const node = static_cast<const WaveModelNode *>(g_transformedWaveforms[static_cast<std::size_t>(slot)]->model);
    if (EffeGeometryMesh *const mesh = TaskDataPointer<EffeGeometryMesh>(node->meshOffset)) {
        TRWave_BuildStripUVs(rowDivisions, columnDivisions, mesh);
        return;
    }
    const WaveModelNode *const child = TaskDataPointer<WaveModelNode>(node->childOffset);
    if (!child)
        return;
    if (EffeGeometryMesh *const mesh = TaskDataPointer<EffeGeometryMesh>(child->meshOffset))
        TRWave_BuildStripUVs(rowDivisions, columnDivisions, mesh);
}

// Script command 300: a second texture record and four copies of the shore model.
void TRWV_LoadShoreModels(std::int32_t slot, const char *textureDirectory, const char *textureFilename, const char *modelDirectory, const char *modelFilename) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    waveform.auxiliaryTextureSlot = EFFE_Related(4, -1, nullptr, nullptr, static_cast<std::uint32_t>(kTrwvToken));
    auto *const textureFile = static_cast<std::byte *>(ReadFile(textureDirectory, textureFilename, kTrwvToken));
    std::uint32_t format = 0;
    std::int16_t width = 0;
    std::int16_t height = 0;
    std::memcpy(&format, textureFile + 0x14, sizeof(format));
    std::memcpy(&width, textureFile + 0x18, sizeof(width));
    std::memcpy(&height, textureFile + 0x1a, sizeof(height));
    CreateDynamicTexture(waveform.auxiliaryTextureSlot, format, width, height, textureFile + 0x1c, 6);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(textureFile), 0);
    for (std::size_t index = 0; index < 4; ++index) {
        auto *const modelFile = static_cast<std::byte *>(ReadFile(modelDirectory, modelFilename, kTrwvToken));
        std::uint32_t rootOffset = 0;
        std::memcpy(&rootOffset, modelFile + 8, sizeof(rootOffset));
        auto *const root = reinterpret_cast<WaveModelNode *>(modelFile + rootOffset);
        waveform.shoreModels[index] = root;
        setup_trwave_ptrs(root, modelFile);
        waveform.shoreModelFiles[index] = modelFile;
    }
}

// Script command 312: strip UVs for the loaded shore models; the divisions are kept for the ripple surface.
void TRWV_BuildShoreStripUVs(std::int32_t slot, std::int32_t rowDivisions, std::int32_t columnDivisions) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    for (void *const model : waveform.shoreModels) {
        if (model)
            TRWave_BuildStripUVs(rowDivisions, columnDivisions, TaskDataPointer<EffeGeometryMesh>(static_cast<const WaveModelNode *>(model)->meshOffset));
    }
    waveform.gridDivisionsX = rowDivisions;
    waveform.gridDivisionsZ = columnDivisions;
}



void TRWV_LoadRippleModel(std::int32_t slot, const char *textureDirectory, const char *textureFilename, const char *modelDirectory, const char *modelFilename) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    waveform.rippleRecords = static_cast<TrwvRippleRecord *>(HLib_AllocHeapMemBlock(289 * sizeof(TrwvRippleRecord), kTrwvToken));
    waveform.auxiliaryTextureSlot = EFFE_Related(4, -1, nullptr, nullptr, static_cast<std::uint32_t>(kTrwvToken));
    auto *const textureFile = static_cast<std::byte *>(ReadFile(textureDirectory, textureFilename, kTrwvToken));
    std::uint32_t format = 0;
    std::int16_t width = 0;
    std::int16_t height = 0;
    std::memcpy(&format, textureFile + 0x14, sizeof(format));
    std::memcpy(&width, textureFile + 0x18, sizeof(width));
    std::memcpy(&height, textureFile + 0x1a, sizeof(height));
    CreateDynamicTexture(waveform.auxiliaryTextureSlot, format, width, height, textureFile + 0x1c, 6);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(textureFile), 0);
    auto *const modelFile = static_cast<std::byte *>(ReadFile(modelDirectory, modelFilename, kTrwvToken));
    std::uint32_t rootOffset = 0;
    std::memcpy(&rootOffset, modelFile + 8, sizeof(rootOffset));
    auto *const root = reinterpret_cast<WaveModelNode *>(modelFile + rootOffset);
    waveform.rippleModel = root;
    setup_trwave_ptrs(root, modelFile);
    waveform.rippleModelFile = modelFile;
    waveform.rippleActive = 0;
}

// Script command 403.
void TRWV_ReleaseRippleModel(std::int32_t slot) {
    TransformedWaveform &waveform = *g_transformedWaveforms[static_cast<std::size_t>(slot)];
    waveform.rippleActive = 0;
    if (waveform.rippleRecords)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(waveform.rippleRecords), 0);
    waveform.rippleRecords = nullptr;
    if (waveform.auxiliaryTextureSlot >= 0) {
        ReleaseRuntimeEffectTexture(waveform.auxiliaryTextureSlot);
        EFFE_ReleaseRegistryEntry(g_effeTextureRegistry[static_cast<std::size_t>(waveform.auxiliaryTextureSlot)]);
    }
    waveform.auxiliaryTextureSlot = -1;
    if (waveform.rippleModel)
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform.rippleModelFile), 0);
    waveform.rippleModel = nullptr;
}

} // namespace

namespace detail {

void TRWV_InitializeEffectTablesNative() {
    Generate16PointTwiddles();
    for (TerrainPatch &patch : g_terrainPatchData) {
        patch.reserved000 = 0;
        patch.colour = 0xffffffffU;
        TRWave_SeedSpectrumPhases(patch.spectrum);
    }
    for (RuntimeEffectTexture &record : g_runtimeEffectTextures)
        record.format.texture = 0;
}

void TRWV_ResetSceneGlobalsNative() {
    g_duskColourOffset = {};
    g_transformFrameLimit = 0;
    g_transformFrame = 0;
    g_duskPreviousMinute = 0;
    g_transformFlags = 1;
    g_transformedWaveforms.fill(nullptr);
}

} // namespace detail

HLib::Task *TRWV_Init() {
    g_trwvTask = HLib_EnqueueTaskWithoutParameter(reinterpret_cast<HLib::TaskCallback>(TRWV_Callback), 11, 8, kTrwvToken);
    return g_trwvTask;
}

HLib::Task *TRWV_GetTask() {
    return g_trwvTask && g_trwvTask->mainTaskCallbackFnPtr == reinterpret_cast<HLib::TaskCallback>(TRWV_Callback) ? g_trwvTask : nullptr;
}

TransformedWaveform *TRWV_GetWaveform(std::uint32_t slot) {
    return slot < g_transformedWaveforms.size() ? g_transformedWaveforms[slot] : nullptr;
}

std::uint32_t InitTransformedWaveform(const Vector3 *origin, const char *textureDirectory, const char *textureFilename, const char *waveformDirectory,
    const char *waveformFilename, float scale, std::uint32_t flags) {
    std::uint32_t slot = 0;
    while (slot < g_transformedWaveforms.size() && g_transformedWaveforms[slot])
        ++slot;

    if (slot == g_transformedWaveforms.size())
        return 0xffffffffU;
    auto *const waveform = static_cast<TransformedWaveform *>(HLib_AllocHeapMemBlock(sizeof(TransformedWaveform), kTrwvToken));
    g_transformFrameLimit = 0;
    g_transformFrame = 0;
    g_duskPreviousMinute = 0;
    g_transformedWaveforms[slot] = waveform;
    g_transformFlags = 1;
    waveform->flags = flags | 0x0aU;
    waveform->position = origin ? *origin : Vector3{};
    waveform->gridDivisionsX = 4;
    waveform->gridDivisionsZ = 4;
    waveform->scale = scale;
    waveform->amplitude = 0.30000001f;
    waveform->frequency = 2.76f;
    waveform->velocityX = -0.059999999f;
    waveform->velocityZ = -0.059999999f;
    waveform->reserved030 = 0;
    waveform->reserved034 = 0.0099999998f;
    waveform->stripOffsetU = 0;
    waveform->lightDirection = {1.0f, -0.31f, -0.73000002f};
    waveform->patchSizeX = 16.0f;
    waveform->patchSizeZ = 16.0f;
    waveform->yaw = 0;
    waveform->localOffset = {};
    waveform->positiveExtentX = 0;
    waveform->negativeExtentX = 0;
    waveform->positiveExtentZ = 0;
    waveform->negativeExtentZ = 0;
    waveform->stripOffsetV = 10;
    waveform->shoreOffsetU = 0;
    waveform->shoreOffsetV = -2;
    waveform->phaseX = 0;
    waveform->phaseY = 0;
    waveform->phaseStepX = 2;
    waveform->phaseStepY = 4;
    waveform->rippleActive = 0;
    waveform->rippleRecords = nullptr;
    waveform->vertexFile = nullptr;
    waveform->shoreAlphaTables = nullptr;
    waveform->velocityFile = nullptr;
    waveform->rippleModel = nullptr;
    for (void *&model : waveform->shoreModels)
        model = nullptr;
    waveform->attachedNode = nullptr;
    waveform->terrainSlot = EFFE_Related(3, -1, nullptr, nullptr, static_cast<std::uint32_t>(kTrwvToken));
    waveform->textureSlot = EFFE_Related(4, -1, nullptr, nullptr, static_cast<std::uint32_t>(kTrwvToken));
    waveform->auxiliaryTextureSlot = -1;
    waveform->colourArgb = 0xffffffffU;
    g_duskColourOffset = {};

    auto *const textureFile = static_cast<std::byte *>(textureDirectory && textureFilename ? ReadFile(textureDirectory, textureFilename, kTrwvToken) : nullptr);
    if (textureFile) {
        const std::uint32_t mode = (flags >> 8U) & 0x0fU;
        const std::uint32_t textureMode = (flags >> 12U) & 0x0fU;
        std::uint32_t format = 0;
        std::int16_t width = 0;
        std::int16_t height = 0;
        std::memcpy(&format, textureFile + 0x14, sizeof(format));
        std::memcpy(&width, textureFile + 0x18, sizeof(width));
        std::memcpy(&height, textureFile + 0x1a, sizeof(height));
        if (mode == 1U || (mode == 0U && textureMode == 0U))
            CreateDynamicTexture(waveform->textureSlot, format, width, height, textureFile + 0x0c, 3);
        else if (mode == 0U && textureMode == 1U)
            CreateDynamicTexture(waveform->textureSlot, format, width, height, textureFile + 0x0c, 6);

        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(textureFile + 0x1c), 0);
    }
    waveform->model = nullptr;
    waveform->modelFile = nullptr;
    auto *const modelFile =
        static_cast<std::byte *>(waveformDirectory && waveformFilename ? ReadFile(waveformDirectory, waveformFilename, kTrwvToken) : nullptr);
    if (modelFile) {
        std::uint32_t rootOffset = 0;
        std::memcpy(&rootOffset, modelFile + 8, sizeof(rootOffset));
        auto *const root = reinterpret_cast<WaveModelNode *>(modelFile + rootOffset);
        waveform->model = root;
        setup_trwave_ptrs(root, modelFile);
        waveform->modelFile = modelFile;
        LoadWater(waveformFilename, root);
    }

    g_trwvVelocityTable = static_cast<float *>(HLib_AllocHeapMemBlock(128 * sizeof(float), kTrwvToken));
    FillVelocityTable(*waveform);
    TRWave_SetSpectrumVelocities(waveform->terrainSlot, g_trwvVelocityTable);
    g_trwvDistanceTable = static_cast<float *>(HLib_AllocHeapMemBlock(64 * sizeof(float), kTrwvToken));
    float *const table = GenerateDistanceLogLookupTable(waveform->frequency);
    TRWave_SetSpectrumAmplitudes(waveform->terrainSlot, ScaleLogTableEntries(waveform->amplitude, table));
    return slot;
}

void TRWV_Callback(void *) {
    std::uint32_t flags = g_transformFlags;
    if ((flags & 1U) == 0U) {
        if ((flags & 2U) != 0U) {
            if (g_transformFrame > g_transformFrameLimit) {
                flags = (flags & 0xfffffffcU) | 1U;
                g_transformFlags = flags;
            }
        } else {
            g_transformFrame = 0;
            flags |= 2U;
            g_transformFlags = flags;
        }
        if ((flags & 1U) == 0U)
            return;
    }
    for (TransformedWaveform *const waveform : g_transformedWaveforms) {
        if (!waveform || (waveform->flags & 2U) == 0U)
            continue;
        const std::uint32_t mode = (waveform->flags >> 8U) & 0x0fU;
        if (mode == 0U)
            SetupTRWave(waveform);
        else if (mode == 1U)
            CalcTRWave(waveform);
    }
}

namespace {


void TRWV_UpdateBowSprayParticle(EffeParticle *particle) {
    const float vy = particle->velocity.y;
    particle->position.x = particle->velocity.x + particle->position.x;
    particle->position.z = particle->velocity.z + particle->position.z;
    particle->position.y = vy + particle->position.y;
    particle->velocity.y = static_cast<float>(static_cast<double>(vy) - 0.005);
    particle->scale = static_cast<float>(static_cast<double>(particle->scale) - 0.04);
    if (0.1 > static_cast<double>(particle->scale)) {
        particle->flags &= 0x7fffffffU;
        particle->update = sh4_parts_draw;
    }
    const auto fraction = static_cast<float>(static_cast<double>(particle->colour >> 24U) / 255.0);
    const auto alpha = static_cast<float>(static_cast<double>(fraction) - 0.01);
    if (0.05 > static_cast<double>(alpha)) {
        particle->flags &= 0x7fffffffU;
        particle->update = sh4_parts_draw;
    }
    particle->colour = (particle->colour & 0x00ffffffU) | (static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<double>(alpha) * 255.0)) << 24U);
}


void TRWV_SpawnBowSprayParticle(EffeParticle *particle) {
    particle->flags |= 0x80000000U;
    particle->update = TRWV_UpdateBowSprayParticle;
    particle->colour = g_trwvBowSprayColour;
    particle->scale = 1.0f;
}




void TRWV_UpdateBowSprayEmitters(void *) {
    TransformedWaveform *const waveform = g_transformedWaveforms[0];
    if (!waveform)
        return;
    if ((waveform->flags & 1U) != 0) {
        if ((g_inputState.slots[1].held & 0x8000U) != 0) {
            const std::uint32_t colour = InputSlotColour(1, 0x0e);
            g_trwvBowSprayColour = colour;
            for (const TrwvBowSprayEmitter &emitter : g_trwvBowSprayEmitters)
                g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)].colour = colour;
        } else {
            for (const TrwvBowSprayEmitter &emitter : g_trwvBowSprayEmitters)
                g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)].colour = waveform->emitterColour;
            g_trwvBowSprayColour = waveform->emitterColour;
        }
    }
    const double offsetX = 96.0 - static_cast<double>(g_trwvBowSprayOrigin.x);
    const double offsetY = 0.0 - static_cast<double>(g_trwvBowSprayOrigin.y);
    const double offsetZ = 89.0 - static_cast<double>(g_trwvBowSprayOrigin.z);
    for (TrwvBowSprayEmitter &emitter : g_trwvBowSprayEmitters) {
        const Vector3 point{static_cast<float>(static_cast<double>(emitter.origin.x) - offsetX),
            static_cast<float>(static_cast<double>(emitter.origin.y) - offsetY), static_cast<float>(static_cast<double>(emitter.origin.z) - offsetZ)};
        emitter.origin.y = TRWV_QueryWaveHeight(waveform->terrainSlot, point);
    }
    for (std::size_t index = 0; index < 2; ++index) {
        TrwvBowSprayEmitter &emitter = g_trwvBowSprayEmitters[index];
        const float height = emitter.origin.y;
        float sway = emitter.sway;
        if (height >= 0.0f)
            sway = static_cast<float>(static_cast<double>(sway) + 0.05);
        else if (0.0f > height)
            sway = static_cast<float>(static_cast<double>(sway) - 0.05);
        if (static_cast<double>(sway) > 1.3)
            sway = 1.3f;
        else if (-0.6 > static_cast<double>(sway))
            sway = -0.6f;
        emitter.sway = sway;
        EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)];
        const float width = emitter.width + sway;
        const bool full = (runtime.flags & 0x20000000U) != 0;
        runtime.halfWidth = full ? width : width * 0.5f;
        runtime.halfHeight = full ? emitter.height : emitter.height * 0.5f;
    }
    for (const TrwvBowSprayEmitter &emitter : g_trwvBowSprayEmitters) {
        EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)];
        runtime.positionMin = {static_cast<float>(static_cast<double>(emitter.origin.x) - offsetX),
            static_cast<float>(static_cast<double>(emitter.origin.y) - offsetY), static_cast<float>(static_cast<double>(emitter.origin.z) - offsetZ)};
        runtime.positionRange = emitter.range;
        g_effeEmitterTransforms[static_cast<std::size_t>(emitter.runtimeSlot)] = {
            .enabled = 1, .position = waveform->position, .offset = waveform->localOffset, .angleX = 0, .angleY = waveform->yaw, .angleZ = 0};
    }
}

// PVR_LoadWrapper (commands 200/201; 201 loads model/object shibu.pvr): six emitters on fresh EFFE registry entries, emitter 0 carrying the
// callback, all sharing the file's texture. The file is freed at the end although each system keeps a pointer into it.
std::int32_t TRWV_LoadBowSprayEmitters(std::int32_t slot, const Vector3 &origin, const char *directory, const char *filename) {
    g_trwvBowSprayOrigin = origin;
    g_trwvBowSprayColour = 0x3ec6f8f6U;
    g_transformedWaveforms[static_cast<std::size_t>(slot)]->emitterColour = 0x3ec6f8f6U;
    g_trwvBowSprayFile = ReadFile(directory, filename, kTrwvToken);
    const auto *const file = static_cast<const std::byte *>(g_trwvBowSprayFile);
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::memcpy(&width, file + 0x18, sizeof(width));
    std::memcpy(&height, file + 0x1a, sizeof(height));
    const double offsetX = 96.0 - static_cast<double>(g_trwvBowSprayOrigin.x);
    const double offsetY = 0.0 - static_cast<double>(g_trwvBowSprayOrigin.y);
    const double offsetZ = 89.0 - static_cast<double>(g_trwvBowSprayOrigin.z);
    for (std::size_t index = 0; index < g_trwvBowSprayEmitters.size(); ++index) {
        TrwvBowSprayEmitter &emitter = g_trwvBowSprayEmitters[index];
        emitter.sway = 0.6f;
        emitter.runtimeSlot = EFFE_Related(0, -1, index == 0 ? TRWV_UpdateBowSprayEmitters : nullptr, nullptr, static_cast<std::uint32_t>(kTrwvToken));
        emitter.particles = static_cast<EffeParticle *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(emitter.particleCount) * 96, kTrwvToken));
        if (emitter.runtimeSlot < 0)
            continue;
        EFFE_InitializeParticleSystem(emitter.runtimeSlot, emitter.particleCount, emitter.particles);
        EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)];
        runtime.positionRange = emitter.range;
        runtime.positionMin = {static_cast<float>(static_cast<double>(emitter.origin.x) - offsetX),
            static_cast<float>(static_cast<double>(emitter.origin.y) - offsetY), static_cast<float>(static_cast<double>(emitter.origin.z) - offsetZ)};
        runtime.particleUpdate = TRWV_SpawnBowSprayParticle;
        runtime.velocityMin = emitter.velocity;
        runtime.velocityRange = emitter.velocityRange;
        runtime.spawnProbabilityQ15 = static_cast<std::int32_t>(static_cast<double>(emitter.probability) * 32767.0);
        const bool full = (runtime.flags & 0x20000000U) != 0;
        runtime.halfWidth = full ? emitter.width : emitter.width * 0.5f;
        runtime.halfHeight = full ? emitter.height : emitter.height * 0.5f;
        EFFE_AttachPvrTexture(emitter.runtimeSlot, file + 0x0c, file + 0x1c, true, width, height);
        runtime.flags |= 0x80000000U;
        runtime.colour = g_trwvBowSprayColour;
        EffeRegistryEntry &entry = g_effeEmitterRegistry[static_cast<std::size_t>(emitter.runtimeSlot)];
        if (entry.state == 1) {
            runtime.flags |= 0x80000000U;
            entry.state = 2;
        }
    }
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_trwvBowSprayFile), 0);
    return 0;
}

// au_re_HLib_FreeTaskData_1 (commands 202/203): emitter 0 deactivates, then every system stops drawing, its registry entry is released and
// its particles are freed. The texture stays with the system.
void TRWV_FreeBowSprayEmitters() {
    const std::int32_t first = g_trwvBowSprayEmitters[0].runtimeSlot;
    if (first >= 0 && g_effeEmitterRegistry[static_cast<std::size_t>(first)].state == 2) {
        g_effeEmitterRegistry[static_cast<std::size_t>(first)].state = 1;
        g_effeRuntime[static_cast<std::size_t>(first)].flags &= ~0x80000000U;
    }
    for (TrwvBowSprayEmitter &emitter : g_trwvBowSprayEmitters) {
        if (emitter.runtimeSlot >= 0) {
            g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)].flags &= 0x7fffffffU;
            EFFE_ReleaseRegistryEntry(g_effeEmitterRegistry[static_cast<std::size_t>(emitter.runtimeSlot)]);
        }
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(emitter.particles), 0);
        emitter.particles = nullptr;
    }
}

} // namespace

void Script_MT5Loader_TRWV_TRWAVE(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!context || !context->stack || !params)
        return;
    const std::uint32_t command = params[0];
    if (command == 0) {
        context->stack->accumulator =
            InitTransformedWaveform(ScenarioPointer<const Vector3>(params[1]), ScenarioPointer<const char>(params[2]), ScenarioPointer<const char>(params[3]),
                ScenarioPointer<const char>(params[4]), ScenarioPointer<const char>(params[5]), std::bit_cast<float>(params[6]), params[7]);
        return;
    }
    if (command == 19) {
        const Vector3 *const origin = ScenarioPointer<const Vector3>(params[1]);
        const Vector3 *const endpoint = ScenarioPointer<const Vector3>(params[2]);
        const Vector3 *const span = ScenarioPointer<const Vector3>(params[3]);
        if (origin && endpoint && span)
            context->stack->accumulator = atan2_fixed16((endpoint->y + span->y) * 0.5f - origin->y, endpoint->z - origin->z);
        return;
    }
    if (command == 20) {
        const Vector3 *const left = ScenarioPointer<const Vector3>(params[2]);
        const Vector3 *const right = ScenarioPointer<const Vector3>(params[3]);
        if (left && right)
            context->stack->accumulator = atan2_fixed16(left->y - right->y, left->x - right->x);
        return;
    }

    switch (command) {
    case 202:
    case 203:
        TRWV_FreeBowSprayEmitters();
        return;
    case 210: {
        const std::uint32_t colour = (params[2] << 24U) | ((params[3] & 0xffU) << 16U) | ((params[4] & 0xffU) << 8U) | (params[5] & 0xffU);
        if (g_transformedWaveforms[0])
            g_transformedWaveforms[0]->emitterColour = colour;
        g_trwvBowSprayColour = colour;
        for (const TrwvBowSprayEmitter &emitter : g_trwvBowSprayEmitters)
            g_effeRuntime[static_cast<std::size_t>(emitter.runtimeSlot)].colour = colour;
        return;
    }
    default:
        break;
    }

    const std::uint32_t slot = params[1];
    if (slot >= g_transformedWaveforms.size())
        return;
    TransformedWaveform *const waveform = g_transformedWaveforms[slot];
    if (!waveform)
        return;
    const auto slotIndex = static_cast<std::int32_t>(slot);

    switch (command) {
    case 1:
        waveform->flags |= 2U;
        break;
    case 2:
        waveform->flags &= ~2U;
        break;
    case 3:
        FreeEffects(slotIndex);
        break;
    case 5:
        if (const Vector3 *const position = ScenarioPointer<const Vector3>(params[2]))
            waveform->position = *position;
        break;
    case 9:
        TRWV_SetWaveShapeAndVelocity(slotIndex, std::bit_cast<float>(params[2]), std::bit_cast<float>(params[3]), std::bit_cast<float>(params[4]));
        break;
    case 10:
        waveform->patchSizeX = std::bit_cast<float>(params[2]);
        waveform->patchSizeZ = std::bit_cast<float>(params[3]);
        waveform->positiveExtentX = static_cast<std::int32_t>(params[4]);
        waveform->negativeExtentX = static_cast<std::int32_t>(params[5]);
        waveform->positiveExtentZ = static_cast<std::int32_t>(params[6]);
        waveform->negativeExtentZ = static_cast<std::int32_t>(params[7]);
        break;
    case 11:
        waveform->flags |= 0x04000000U;
        waveform->lightDirection = {std::bit_cast<float>(params[2]), std::bit_cast<float>(params[3]), std::bit_cast<float>(params[4])};
        break;
    case 12:
        waveform->yaw = static_cast<std::int32_t>(params[2]);
        break;
    case 13:
        TRWV_SetWaveShape(slotIndex, std::bit_cast<float>(params[2]), std::bit_cast<float>(params[3]));
        break;
    case 14:
        TRWV_SetWaveVelocity(slotIndex, std::bit_cast<float>(params[2]), std::bit_cast<float>(params[3]));
        break;
    case 15: {
        const char *const directory = ScenarioPointer<const char>(params[2]);
        const char *const filename = ScenarioPointer<const char>(params[3]);
        if (waveform->velocityFile)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->velocityFile), 0);
        if (directory && filename) {
            waveform->velocityFile = ReadFile(directory, filename, kTrwvToken);
            TRWave_SetSpectrumVelocities(waveform->terrainSlot, static_cast<const float *>(waveform->velocityFile));
        }
        break;
    }
    case 16:
        waveform->stripOffsetU = static_cast<std::int32_t>(params[2]);
        waveform->stripOffsetV = static_cast<std::int32_t>(params[3]);
        break;
    case 17: {
        const char *const directory = ScenarioPointer<const char>(params[2]);
        const char *const filename = ScenarioPointer<const char>(params[3]);
        if (waveform->vertexFile)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(waveform->vertexFile), 0);
        waveform->vertexFile = ReadFile(directory, filename, kTrwvToken);
        break;
    }
    case 18:
        if (static_cast<std::uint32_t>(g_perspectiveProjection - 1) > 2U) {
            if (const Vector3 *const point = ScenarioPointer<const Vector3>(params[2]))
                context->stack->accumulator = std::bit_cast<std::uint32_t>(TRWV_QueryWaveHeight(slotIndex, *point));
        }
        break;
    case 21:
        TRWV_SetStripDivisions(slotIndex, static_cast<std::int32_t>(params[2]), static_cast<std::int32_t>(params[3]));
        break;
    case 22: {
        std::uint32_t mask = 0;
        if (params[2] == 1)
            mask = 0x08000000U;
        else if (params[2] == 2)
            mask = 0x02000000U;
        else if (params[2] == 3)
            mask = 4U;
        if (!mask)
            break;
        const bool wasSet = (waveform->flags & mask) != 0;
        if (params[3] == 1)
            waveform->flags |= mask;
        else if (params[3] == 2)
            waveform->flags &= ~mask;
        else if (params[3] == 3)
            waveform->flags ^= mask;
        context->stack->accumulator = wasSet ? 1U : 0U;
        break;
    }
    case 23:
        if (const Vector3 *const offset = ScenarioPointer<const Vector3>(params[2]))
            waveform->localOffset = *offset;
        break;
    case 24: {
        const std::uint32_t colour = (params[2] << 24U) | ((params[3] & 0xffU) << 16U) | ((params[4] & 0xffU) << 8U) | (params[5] & 0xffU);
        waveform->colourArgb = colour;
        g_terrainPatchData[static_cast<std::size_t>(waveform->terrainSlot)].colour = colour;
        break;
    }
    case 200:
        context->stack->accumulator = static_cast<std::uint32_t>(TRWV_LoadBowSprayEmitters(
            slotIndex, *ScenarioPointer<const Vector3>(params[2]), ScenarioPointer<const char>(params[3]), ScenarioPointer<const char>(params[4])));
        break;
    case 201:
        TRWV_LoadBowSprayEmitters(slotIndex, *ScenarioPointer<const Vector3>(params[2]), "model/object", "shibu.pvr");
        break;
    case 300:
        TRWV_LoadShoreModels(slotIndex, ScenarioPointer<const char>(params[2]), ScenarioPointer<const char>(params[3]), ScenarioPointer<const char>(params[4]),
            ScenarioPointer<const char>(params[5]));
        context->stack->accumulator = 0;
        break;
    case 310: {
        const char *const directory = ScenarioPointer<const char>(params[2]);
        const char *const filename = ScenarioPointer<const char>(params[3]);
        if (waveform->shoreAlphaTables)
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(waveform->shoreAlphaTables), 0);
        if (directory && filename)
            waveform->shoreAlphaTables = static_cast<std::uint8_t *>(ReadFile(directory, filename, kTrwvToken));
        break;
    }
    case 311:
        waveform->shoreOffsetU = static_cast<std::int32_t>(params[2]);
        waveform->shoreOffsetV = static_cast<std::int32_t>(params[3]);
        break;
    case 312:
        TRWV_BuildShoreStripUVs(slotIndex, static_cast<std::int32_t>(params[2]), static_cast<std::int32_t>(params[3]));
        break;
    case 400:
        TRWV_LoadRippleModel(slotIndex, ScenarioPointer<const char>(params[2]), ScenarioPointer<const char>(params[3]), ScenarioPointer<const char>(params[4]),
            ScenarioPointer<const char>(params[5]));
        context->stack->accumulator = 0;
        break;
    case 401:
        waveform->rippleActive = 1;
        TRWV_InitRippleRecords(waveform->rippleRecords, waveform->scale);
        waveform->phaseX = 0;
        waveform->phaseY = 0;
        waveform->phaseStepX = 2;
        waveform->phaseStepY = 4;
        break;
    case 402:
        waveform->rippleActive = 0;
        break;
    case 403:
        TRWV_ReleaseRippleModel(slotIndex);
        break;
    case 500:
    case 1000: {
        const Character *const character = CHID_Find(context->task->task, params[2]);
        waveform->attachedNode = TaskDataPointer<WaveModelNode>(static_cast<const Mt5RuntimeNode *>(character->model)->childOffset);
        break;
    }
    default:
        break;
    }
}

void TRWV_Destroy() {
    const HLib::Task *const task = g_trwvTask;
    g_trwvTask = nullptr;
    if (task && task->mainTaskCallbackFnPtr == reinterpret_cast<HLib::TaskCallback>(TRWV_Callback))
        HLib_DestroyTask(const_cast<HLib::Task *>(task));
    for (std::size_t slot = 0; slot < g_transformedWaveforms.size(); ++slot) {
        if (g_transformedWaveforms[slot])
            FreeEffects(static_cast<std::int32_t>(slot));
    }
    g_transformFlags = 0;
    g_transformFrame = 0;
    g_transformFrameLimit = 0;
}

} // namespace shenmue::s1
