#include "auto_test.hpp"
#include "platform/dc/sdk.hpp"
#include "common/types.hpp"

#if SHENMUE_IS_DC

#if defined(_DEBUG) || (defined(SHENMUE_DC_BUILD_HASH) && SHENMUE_DC_BUILD_HASH)
#endif


#if SHENMUE_IS_S2
#include "platform/dc/mt7.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/texture_cache_dc.hpp"
#endif

#include <cmath>

#if SHENMUE_IS_S2
#include "common/matrix_stack.hpp"
namespace shenmue {
namespace s2 {
bool PopCurrentMatrix(std::int32_t count);
}
} // namespace shenmue
#endif

namespace shenmue {
namespace dc {
namespace sdk {


// @verified - d
void InitializeMatrixConstantsNullsub() {
}

namespace {
inline float MultiplyAdd(float first, float second, float addend) {
#if SHENMUE_IS_S2
    return first * second + addend;
#else
    return __builtin_fmaf(first, second, addend);
#endif
}
inline float SquareRoot(float value) {
#if SHENMUE_IS_S2
    return ::sqrtf(value);
#else
    return __builtin_sqrtf(value);
#endif
}
std::uint32_t g_randomSeed = 1;
extern "C" void njPrintC(std::int32_t location, const char *text);
extern "C" void syHwInit2();
extern "C" void syCacheOCBP(void *memory, std::uint32_t bytes);
extern "C" void kmiDiscardRenderPipeline();
extern "C" std::int32_t pdVibIsReady(std::uint32_t port);
extern "C" std::int32_t kdSetSetCondition(std::uint32_t port, std::uint32_t function, const void *data, std::uint32_t words);


} // namespace

void InitializeSystem(std::int32_t mode, std::uint32_t framebufferFlags, std::int32_t count) {
    const std::int32_t display = kSystemDisplayModes[mode] | ((framebufferFlags & 0x8000U) ? 0x10000 : 0);
    njInitSystem(display, static_cast<std::int32_t>(framebufferFlags & 0xfU), count);
    syHwInit2();
#if SHENMUE_IS_S2
    g_mt7DisplayWidth = kMT7SystemDisplayModes[mode].width;
    g_mt7DisplayHeight = kMT7SystemDisplayModes[mode].height;
    g_mt7Supersampling = (framebufferFlags & 0x8000U) != 0;
    g_mt7ExtendedPAL = static_cast<std::uint32_t>(mode) - 24U < 4U;
    g_mt7FramebufferFormat = kMT7FramebufferFormats[framebufferFlags & 0xfU];
    g_modelScale = 1.0f;
    SetMT7Viewport(0.0f, 0.0f, static_cast<float>(g_mt7DisplayWidth), static_cast<float>(g_mt7DisplayHeight));
    *reinterpret_cast<volatile std::uint32_t *>(kStoreQueueAddressControl0) = 0x0c;
    *reinterpret_cast<volatile std::uint32_t *>(kStoreQueueAddressControl1) = 0x0c;
    s2::g_defaultTextureSurfaceInitialized = 0;
    g_mt7AlphaBlendMask = 0x68;
#endif
}

// @verified - d
float SinFixed(std::int32_t angle) {
    float reduced = static_cast<float>(static_cast<std::uint16_t>(angle)) * kFixedAngleTurnRadians;
    reduced /= 65536.0f;
    reduced /= 2.0f;
    const float halfPi = 1.5707962512969971f;
    const std::int32_t quadrant = static_cast<std::int32_t>(reduced / halfPi + 0.5f);
    reduced -= static_cast<float>(quadrant) * halfPi;
    const float square = reduced * reduced;
    float fraction = 0.0f;
    for (std::int32_t divisor = 11; divisor >= 3; divisor -= 2)
        fraction = square / (static_cast<float>(divisor) - fraction);
    const float tangent = reduced / (1.0f - fraction);
    float denominator = 1.0f;
    denominator = MultiplyAdd(tangent, tangent, denominator);
    const float result = (tangent * 2.0f) / denominator;
    return quadrant & 1 ? -result : result;
}

// @verified - d
float CosFixed(std::int32_t angle) {
    std::int32_t folded = static_cast<std::uint16_t>(angle);
    if (folded > 0x8000)
        folded = 0x10000 - folded;
    return SinFixed(0x4000 - folded);
}

// @verified - d
float TanFixed(std::int32_t angle) {
    const std::uint16_t fixed = static_cast<std::uint16_t>(angle);
    if ((fixed & 0x7fffU) == 0x4000U)
        return 3.4028234663852886e+38f;
    float reduced = static_cast<float>(fixed) * kFixedAngleTurnRadians;
    reduced /= 65536.0f;
    const float halfPi = 1.5707962512969971f;
    const std::int32_t quadrant = static_cast<std::int32_t>(reduced / halfPi + 0.5f);
    reduced -= static_cast<float>(quadrant) * halfPi;
    const float square = reduced * reduced;
    float fraction = 0.0f;
    for (std::int32_t divisor = 13; divisor >= 3; divisor -= 2)
        fraction = square / (static_cast<float>(divisor) - fraction);
    const float tangent = reduced / (1.0f - fraction);
    if (!(quadrant & 1))
        return tangent;
    return tangent == 0.0f ? 3.4028234663852886e+38f : -1.0f / tangent;
}

// @verified - d
float InverseSqrt(float value) {
    if (value == 0.0f)
        return 3.4028234663852886e+38f;
    value = SquareRoot(value);
    return 1.0f / value;
}


// @verified - d
std::uint32_t ArcTanFixed(float value) {
    const float reduced = value > 1.0f || value < -1.0f ? 1.0f / value : value;
    float fraction = 0.0f;
    for (std::int32_t step = 13; step > 0; --step) {
        const float index = static_cast<float>(step);
#if SHENMUE_IS_S2
        const float numerator = ((index * reduced) * index) * reduced;
        const float denominator = (fraction + 1.0f) + static_cast<float>(step * 2);
#else
        const float numerator = ((index * index) * reduced) * reduced;
        const float denominator = (static_cast<float>(step * 2) + fraction) + 1.0f;
#endif
        fraction = numerator / denominator;
    }
    const float radians = reduced / (fraction + 1.0f);
    const float scaled = ((radians + kFixedAngleTurnRadians) * 65536.0f) / kFixedAngleTurnRadians + 0.5f;
#if SHENMUE_IS_S2
    const std::int32_t truncated = TruncateToInt32(scaled);
    const std::uint32_t angle = static_cast<std::uint32_t>(truncated) & 0xffffU;
#else
    const std::uint32_t angle = static_cast<std::uint32_t>(static_cast<std::int32_t>(scaled)) & 0xffffU;
#endif
    if (value > 1.0f)
        return (0x4000U - angle) & 0xffffU;
    if (value < -1.0f)
        return (0xc000U - angle) & 0xffffU;
    return angle;
}

// @verified - d
std::uint32_t ArcTan2Fixed(float y, float x) {
    if (y > x) {
        if (-y > x)
            return x == 0.0f ? 0x8000U : (ArcTanFixed(y / x) + 0x8000U) & 0xffffU;
        return y == 0.0f ? 0x4000U : (0x4000U - ArcTanFixed(x / y)) & 0xffffU;
    }
    if (-y > x)
        return y == 0.0f ? 0xc000U : (0xc000U - ArcTanFixed(x / y)) & 0xffffU;
    return x == 0.0f ? 0U : ArcTanFixed(y / x);
}

// @verified - d
std::uint32_t ArcSinFixed(float value) {
    float magnitude = value;
    magnitude = __builtin_fabsf(magnitude);
    if (magnitude > 1.0f)
        return 0;
    float root = 1.0f - value * value;
    root = SquareRoot(root);
    if (root == 0.0f)
        return value > 0.0f ? 0x4000U : 0xc000U;
    return ArcTanFixed(value / root);
}

// @verified - d
std::uint32_t RandomInt() {
    g_randomSeed = LcgStep(g_randomSeed);
    return (g_randomSeed >> 16) & 0x7fffU;
}

// @verified - d
float RandomFloat() {
    return static_cast<float>(RandomInt()) * 0.000030517578125f;
}

// @verified - d
void SeedRandom(std::uint32_t seed) {
    g_randomSeed = seed;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float TruncateFloat(float value) {
    const float magnitude = __builtin_fabsf(value);
    if (magnitude < 8388608.0f) {
        const std::int32_t whole = static_cast<std::int32_t>(magnitude);
        const float integral = static_cast<float>(whole);
        if (magnitude - integral != 0.0f) {
            if (value > 0.0f)
                return integral;
            return static_cast<float>(-whole);
        }
    }
    return value;
}
#endif

void PrintText(std::int32_t x, std::int32_t y, const char *text) {
#if SHENMUE_IS_S2
    s2::SetDebugScreenTextPosition(x, y);
    s2::DrawDebugScreenString(text);
#else
    njPrintC((x << 16) | y, text);
#endif
}

// @verified - d
std::int32_t StartVibration(std::uint32_t port, const std::uint8_t *packet) {
    if (!pdVibIsReady(port))
        return -2;
    const std::uint8_t parameters[4] = {packet[0], packet[1], packet[2], packet[3]};
    return kdSetSetCondition(port, kVibrationDeviceFunctionIndex, parameters, 1) < 0 ? -1 : 0;
}

// @verified - d
void PurgeDataCache(void *memory, std::uint32_t blocks) {
    std::uintptr_t address = reinterpret_cast<std::uintptr_t>(memory);
    for (std::uint32_t block = 0; block < blocks; ++block, address += 32)
        syCacheOCBP(reinterpret_cast<void *>(address), 1);
}

std::int16_t g_graphicsWorkspaceDepth = 0;
std::byte *g_graphicsWorkspaceStack[kGraphicsWorkspaceStackCells] = {};
std::byte *g_graphicsWorkspaceFront = 0;
std::byte *g_graphicsWorkspaceFrontSaved = 0;
std::byte *g_graphicsWorkspaceFrontBase = 0;
std::byte *g_graphicsWorkspaceBack = 0;
std::byte *g_graphicsWorkspaceBackSaved = 0;
std::byte *g_graphicsWorkspaceBackBase = 0;
std::uint32_t g_lightRecords[kLightCount][kLightRecordWords] = {};
ModelLightingMode g_modelLightingMode = Single;
std::uint32_t g_lightMaskByMode[2] = {};
std::uint32_t g_enabledLightMask = 0;
std::uint32_t g_lightProcessingEnabled = 0;
Vector4 g_ambientLight = {};
float g_modelScale;
float g_inverseModelScale = 0.0f;
std::uint32_t g_reflectedLightingEnabled = 0;
Vector3 g_reflectorLocalDirection = {};
Vector3 g_reflectorWorldDirection = {};
Vector3 g_reflectorViewDirection = {};
float g_reflectedLightIntensity = 0.0f;
std::uint32_t g_vertexBufferBaselines[kVertexListCount] = {};
std::uint32_t g_systemMetrics[kSystemMetricsWords] = {};
volatile std::uint32_t g_renderSubmissionActive = 0;
std::uint32_t g_punchThroughBufferBytes = 0;
std::uint32_t g_printFlags = 0;

// @verified - d
void EnableReflectedLighting(const Vector3 *direction, float intensity) {
    g_reflectedLightingEnabled = 1;
    g_reflectorLocalDirection.x = direction->x;
    g_reflectorLocalDirection.y = direction->y;
    g_reflectorLocalDirection.z = direction->z;
    g_reflectedLightIntensity = intensity;
    njCalcVector(0, &g_reflectorLocalDirection, &g_reflectorWorldDirection);
}

// @verified - d
void DisableReflectedLighting() {
    g_reflectedLightingEnabled = 0;
}

void SetVertexWorkBase(void *memory) {
    const std::uintptr_t uncached = reinterpret_cast<std::uintptr_t>(memory) | 0xa0000000U;
    if (*reinterpret_cast<volatile std::uint32_t *>(kPvrRevisionRegister) >= 8)
        g_systemConfiguration[kConfigurationVertexBase] = uncached;
    else
        kmfSetVertexBufferBaseAddress(reinterpret_cast<void *>(uncached), 0);
    g_sdkUserVertexBuffer = reinterpret_cast<std::uint32_t *>(uncached);
    g_sdkUserVertexBufferBytes = kVertexWorkspaceEnd - (uncached & 0x1fffffffU);
}

void SetTextureMemorySize(std::uint32_t bytes) {
#if SHENMUE_IS_S2
    bytes /= sizeof(std::uint32_t);
#endif
    g_systemConfiguration[kConfigurationTextureBytes] = bytes;
    g_sdkTextureMemoryBytes = bytes;
}

void InitializeRenderingMode() {
    if (*reinterpret_cast<volatile std::uint32_t *>(kPvrRevisionRegister) >= 8) {
        g_systemConfiguration[kConfigurationListType] = 0x20;
        g_systemConfiguration[kConfigurationFlags] |= 0x10;
    } else {
        kmChangeLatencyModel(1, 0x20);
    }
}

// @verified - d
void SetPunchThroughBufferSize(std::uint32_t bytes) {
    g_punchThroughBufferBytes = bytes;
}

void SetVertexBufferSizes(
    std::int32_t opaque, std::int32_t opaqueModifier, std::int32_t transparent, std::int32_t transparentModifier, std::int32_t punchThrough) {
    const std::int32_t opaqueWords = g_systemConfiguration[kConfigurationListType] == 0x20 && !opaque ? -1 : opaque / 4;
    kmiDiscardRenderPipeline();
    njInitVertexBuffer(opaqueWords, opaqueModifier / 4, transparent / 4, transparentModifier / 4, punchThrough / 4);
#if SHENMUE_IS_S2
    if (!s2::g_defaultTextureSurfaceInitialized) {
        s2::InitializeDefaultTextureSurface();
        s2::g_defaultTextureSurfaceInitialized = 1;
    }
#endif
}

// @verified - d
void SetBorderColour(std::uint32_t colour) {
    colour |= 0xff000000U;
    njSetBorderColor(colour);
}

void InitializePrint(void *texture) {
#if SHENMUE_IS_S2
    (void)texture;
    s2::InitializeDebugScreenPrint();
#else
    g_printFlags = 0;
    std::uint32_t *const pixels = texture ? reinterpret_cast<std::uint32_t *>(static_cast<std::byte *>(texture) + 0x1c) : 0;
    njInitPrint(pixels, 0x80, texture ? 0x302 : 0x300);
#endif
}

// @verified - d
void EnablePrint() {
    g_printFlags |= 1;
}

// @verified - d
void RegisterWeatherTaskCallback(Callback callback) {
    njSetWaitVSyncFunction(callback, 0);
}

// @verified - d
void InitializeGraphicsWorkspace(std::byte *begin, std::byte *end) {
    begin = reinterpret_cast<std::byte *>((reinterpret_cast<std::uintptr_t>(begin) + 31U) & ~std::uintptr_t(31));
    g_graphicsWorkspaceFrontBase = begin;
    g_graphicsWorkspaceDepth = 0;
    end = reinterpret_cast<std::byte *>((reinterpret_cast<std::uintptr_t>(end) + 31U) & ~std::uintptr_t(31));
    g_graphicsWorkspaceStack[0] = begin;
    g_graphicsWorkspaceFront = begin;
    g_graphicsWorkspaceFrontSaved = begin;
    g_graphicsWorkspaceBackBase = end;
    g_graphicsWorkspaceStack[kGraphicsWorkspaceStackDepth] = end;
    g_graphicsWorkspaceBack = end;
    g_graphicsWorkspaceBackSaved = end;
}

// @verified - d
void InitializationNullsub() {
}

// @verified - d
void InitializeLightState() {
    for (std::size_t light = 0; light < kLightCount; ++light) {
        for (std::size_t word = 0; word < kLightRecordWords; ++word)
            g_lightRecords[light][word] = kDefaultLightRecord[word];
    }
#if !SHENMUE_IS_S2
    g_lightMaskByMode[0] = 0;
    g_lightMaskByMode[1] = 0;
#endif
    g_enabledLightMask = 0;
#if !SHENMUE_IS_S2
    g_lightProcessingEnabled = 0;
#endif
}

// @verified - d
void ResetGraphicsWorkspace() {
    g_graphicsWorkspaceDepth = 0;
    g_graphicsWorkspaceFront = g_graphicsWorkspaceStack[0];
    g_graphicsWorkspaceFrontSaved = g_graphicsWorkspaceStack[0];
    g_graphicsWorkspaceBack = g_graphicsWorkspaceStack[kGraphicsWorkspaceStackDepth];
    g_graphicsWorkspaceBackSaved = g_graphicsWorkspaceStack[kGraphicsWorkspaceStackDepth];
}

// @verified - d
std::int32_t PushGraphicsWorkspace() {
    if (g_graphicsWorkspaceDepth >= static_cast<std::int32_t>(kGraphicsWorkspaceStackDepth))
        return -1;
    ++g_graphicsWorkspaceDepth;
    g_graphicsWorkspaceStack[g_graphicsWorkspaceDepth] = g_graphicsWorkspaceFront;
    g_graphicsWorkspaceStack[kGraphicsWorkspaceStackDepth + g_graphicsWorkspaceDepth] = g_graphicsWorkspaceBack;
    return 0;
}

// @verified - d
std::int32_t PopGraphicsWorkspace() {
    if (g_graphicsWorkspaceDepth <= 0)
        return -1;
    g_graphicsWorkspaceBack = g_graphicsWorkspaceStack[kGraphicsWorkspaceStackDepth + g_graphicsWorkspaceDepth];
    g_graphicsWorkspaceFront = g_graphicsWorkspaceStack[g_graphicsWorkspaceDepth];
    --g_graphicsWorkspaceDepth;
    return 0;
}

// @verified - d
void SetModelScale(float scale) {
    g_modelScale = scale;
#if !SHENMUE_IS_S2
    g_inverseModelScale = 1.0f / scale;
#endif
}

// @verified - d
void SetAmbientLight(const Vector4 *colour) {
    g_ambientLight = *colour;
    float intensity = g_ambientLight.z * 0.5899999737739563f;
    const float red = 0.29999998211860657f;
    const float blue = 0.10999999940395355f;
    intensity = MultiplyAdd(red, g_ambientLight.y, intensity);
    intensity = MultiplyAdd(blue, g_ambientLight.w, intensity);
    g_ambientLight.x = intensity;
}

// @verified - d
void SetLightColour(std::uint32_t index, const float *colour) {
    g_lightRecords[index][0x28 / 4] = floatAsU32(colour[1]);
    g_lightRecords[index][0x2c / 4] = floatAsU32(colour[2]);
    g_lightRecords[index][0x30 / 4] = floatAsU32(colour[3]);
}

// @verified - d
void SetLightIntensity(std::uint32_t index, float intensity) {
    g_lightRecords[index][0xac / 4] = floatAsU32(intensity);
}

// @verified - d
void SetLightProcessingEnabled(std::uint32_t enabled) {
    g_lightProcessingEnabled = enabled;
}

// @verified - d
void SetLightType(std::uint32_t index, std::uint32_t type) {
    std::uint32_t &word = g_lightRecords[index][0];
    word = (word & 0xffffU) | ((type & 0xffffU) << 16);
#if !SHENMUE_IS_S2
    if (word & 0xffffU) {
        const std::uint32_t bit = 1U << (index & 31U);
        const std::uint32_t mode = (type >> 3) & 1U;
        g_lightMaskByMode[1] = (g_lightMaskByMode[1] & ~bit) | (mode ? bit : 0U);
        g_lightMaskByMode[0] = (g_lightMaskByMode[0] & ~bit) | (mode ? 0U : bit);
        g_enabledLightMask = g_lightMaskByMode[0] | g_lightMaskByMode[1];
    }
#endif
}

// @verified - d
void SetLightEnabled(std::uint32_t index, std::int32_t enabled) {
    std::uint32_t &word = g_lightRecords[index][0];
    const std::uint32_t bit = 1U << (index & 31U);
    word = (word & 0xffff0000U) | static_cast<std::uint16_t>(enabled);
#if SHENMUE_IS_S2
    g_enabledLightMask |= bit;
    if (!enabled)
        g_enabledLightMask ^= bit;
#else
    g_lightMaskByMode[0] &= ~bit;
    const std::uint32_t mode = (word >> 19) & 1U;
    g_lightMaskByMode[1] &= ~bit;
    if (enabled == 1)
        g_lightMaskByMode[mode] |= bit;
    g_enabledLightMask = g_lightMaskByMode[0] | g_lightMaskByMode[1];
#endif
}

// @verified - d
void SetLightPosition(std::uint32_t index, float x, float y, float z) {
    g_lightRecords[index][0x34 / 4] = floatAsU32(x);
    g_lightRecords[index][0x38 / 4] = floatAsU32(y);
    g_lightRecords[index][0x3c / 4] = floatAsU32(z);
}

// @verified - d
void SetLightDirection(std::uint32_t index, float x, float y, float z) {
    Vector3 direction = {x, y, z};
    Vector3::NormalizeInPlace(direction);
    g_lightRecords[index][0x40 / 4] = floatAsU32(direction.x);
    g_lightRecords[index][0x44 / 4] = floatAsU32(direction.y);
    g_lightRecords[index][0x48 / 4] = floatAsU32(direction.z);
}

// @verified - d
void SetLightRange(std::uint32_t index, float range) {
    g_lightRecords[index][0xa0 / 4] = floatAsU32(range);
    g_lightRecords[index][0x98 / 4] = floatAsU32(range * range);
    g_lightRecords[index][0x90 / 4] = floatAsU32(1.0f / range);
}

void SetLightFalloff(std::uint32_t index, float falloff) {
    g_lightRecords[index][0x4c / 4] = floatAsU32(std::pow(falloff, 8.0f));
}

// @verified - d
void SetLightAngle(std::uint32_t index, std::int32_t angle) {
    const float cosine = CosFixed(angle);
    const float sine = SinFixed(angle);
    g_lightRecords[index][0xa4 / 4] = floatAsU32(sine);
    g_lightRecords[index][0x58 / 4] = floatAsU32(cosine);
    g_lightRecords[index][0x9c / 4] = floatAsU32(1.0f / (1.0f - cosine));
}

void CommitLightState() {
    const std::uint32_t count = g_lightProcessingEnabled == 0 ? 1U : kLightCount;
    for (std::uint32_t index = 0; index < count; ++index) {
        std::uint32_t *const record = g_lightRecords[index];
        if (g_lightProcessingEnabled != 0 && static_cast<std::uint16_t>(record[0]) == 0)
            continue;
        const float intensity = u32AsFloat(record[0xac / 4]);
        const float red = u32AsFloat(record[0x28 / 4]) * intensity;
        const float green = u32AsFloat(record[0x2c / 4]) * intensity;
        const float blue = u32AsFloat(record[0x30 / 4]) * intensity;
        record[0x18 / 4] = floatAsU32(red);
        record[0x1c / 4] = floatAsU32(green);
        record[0x20 / 4] = floatAsU32(blue);
        float luminance = green * 0.5899999737739563f;
        const float redWeight = 0.29999998211860657f;
        const float blueWeight = 0.10999999940395355f;
        luminance = MultiplyAdd(redWeight, red, luminance);
        luminance = MultiplyAdd(blueWeight, blue, luminance);
        record[0x14 / 4] = floatAsU32(luminance);
    }
    TransformLightState();
}

void TransformLightState() {
    if (static_cast<std::int32_t>(g_lightProcessingEnabled) <= 0) {
        Vector3 direction = {u32AsFloat(g_lightRecords[0][0x40 / 4]), u32AsFloat(g_lightRecords[0][0x44 / 4]), u32AsFloat(g_lightRecords[0][0x48 / 4])};
        njCalcVector(0, &direction, &direction);
        g_lightRecords[0][0x68 / 4] = floatAsU32(direction.x);
        g_lightRecords[0][0x6c / 4] = floatAsU32(direction.y);
        g_lightRecords[0][0x70 / 4] = floatAsU32(direction.z);
        g_modelLightingMode = Single;
    } else {
        for (std::uint32_t index = 0; index < kLightCount; ++index) {
            std::uint32_t *const record = g_lightRecords[index];
            if (static_cast<std::int32_t>(record[0]) <= 0)
                continue;
            Vector3 direction = {u32AsFloat(record[0x40 / 4]), u32AsFloat(record[0x44 / 4]), u32AsFloat(record[0x48 / 4])};
            njCalcVector(0, &direction, &direction);
            record[0x68 / 4] = floatAsU32(direction.x);
            record[0x6c / 4] = floatAsU32(direction.y);
            record[0x70 / 4] = floatAsU32(direction.z);
            Vector3 position = {u32AsFloat(record[0x34 / 4]), u32AsFloat(record[0x38 / 4]), u32AsFloat(record[0x3c / 4])};
            njCalcPoint(0, &position, &position);
            record[0x5c / 4] = floatAsU32(position.x);
            record[0x60 / 4] = floatAsU32(position.y);
            record[0x64 / 4] = floatAsU32(position.z);
        }
        g_modelLightingMode = Multiple;
    }
}

void SnapshotVertexBufferPointers() {
    for (std::size_t list = 0; list < kVertexListCount; ++list)
        g_vertexBufferBaselines[list] = g_sdkVertexBufferPointers[list];
}

void GetVertexBufferUsage(std::uint32_t *bytes) {
    for (std::size_t list = 0; list < kVertexListCount; ++list)
        bytes[list] = g_sdkVertexBufferPointers[list] - g_vertexBufferBaselines[list];
}

void CollectSystemMetrics() {
    kmfGetInternalSystemMetrics(g_systemMetrics);
}

void SubmitFrame() {
#if !SHEN_AUTO_TEST_ENABLED && (defined(_DEBUG) || (defined(SHENMUE_DC_BUILD_HASH) && SHENMUE_DC_BUILD_HASH))
#if SHENMUE_IS_S2
    s2::DrawDebugCommitHash();
#else
    s1::DrawDebugCommitHash();
#endif
#endif
    g_renderSubmissionActive = 1;
    kmRender();
    g_renderSubmissionActive = 0;
    kmFlipFrameBuffer();
#if SHEN_AUTO_TEST_ENABLED
    auto_test::FinishFrame();
#endif
}

#if SHENMUE_IS_S2
void TransformLightRecords() {
    ::shenmue::matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::NormalizeAndSetTransformMatrix();
    for (std::int32_t index = 0; index <= 15; ++index) {
        std::uint32_t *const record = g_lightRecords[index];
        if (static_cast<std::int16_t>(record[0])) {
            Matrix4x4::Transform(*reinterpret_cast<const Vector3 *>(record + 0x68 / 4), *reinterpret_cast<Vector3 *>(record + 0x80 / 4));
            Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(record + 0x5c / 4), *reinterpret_cast<Vector3 *>(record + 0x74 / 4));
        }
    }
    s2::PopCurrentMatrix(1);
}
#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
