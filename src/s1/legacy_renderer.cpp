#include "s1/legacy_renderer.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/mt5_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#endif

namespace shenmue::s1 {

std::array<AlphaRenderDefaults, 4> g_alphaRenderDefaults{};
std::array<AlphaRenderDefaults, 4> g_currentAlphaRenderFlags{};
LegacyRendererDefaults g_legacyRendererDefaults{};
std::array<EngineAlphaState, 4> g_engineAlphaState{};
AlphaShaderParameters g_alphaShaderParameters{};
LegacyShaderParameters g_legacyShaderParameters{};
ShaderParams g_renderParams = [] {
    ShaderParams parameters{};


    parameters.viewportScaleX = 1.0f;
    parameters.viewportScaleY = 1.0f;
    return parameters;
}();
std::array<std::int32_t, 172> g_defaultRenderFlags{};
std::array<std::int32_t, 172> g_currentRenderFlags{};
std::int32_t g_numVertices = 0;
std::uint32_t g_rendererWhiteTexture = 0;
std::int32_t g_legacyCurrentRenderingMode = 0;
std::uint64_t g_legacyEngineFlags = 0;
std::uint64_t g_legacyEngineField8 = 0;
std::int32_t g_legacyStencilReference = 0;
std::array<std::uint32_t, 2> g_legacyAlphaFlags{};
Matrix4x4 g_cameraProjectionMatrix{};
float g_tempNearClip = 0.0f;
float g_tempFarClip = 0.0f;
float g_tempExtentNearClip = 0.0f;

float g_verticalFieldOfView = 0.0f;
float g_screenWidth = 0.0f;
float g_screenHeight = 0.0f;
float g_screenHalfWidth = 0.0f;
float g_screenHalfHeight = 0.0f;
float g_screenWidthOffset = 0.0f;
float g_screenHeightOffset = 0.0f;
Vector4 g_screenHalfWithOffset{};
Vector4 g_audioLevelMatrixNormal{0.0f, 0.0f, 1.0f, 0.0f};
float g_unusedScreenScale = 1.0f;
std::array<std::byte, kDebugTextQueueCapacity> g_debugTextQueue{};
std::size_t g_debugTextQueueUsed = 0;
std::uint32_t g_debugTextQueueCount = 0;
std::uint32_t g_debugTextColour = 0xffffffffU;

namespace {

std::uint32_t GetShenmueEngineFlag_0(std::int32_t value) {
    return value >= 512 && value <= 518 ? static_cast<std::uint32_t>(value - 511) : 0U;
}

std::uint32_t GetShenmueEngineFlag(std::int32_t value) {
    switch (value) {
    case 0:
        return 1;
    case 5386:
        return 5;
    case 7681:
        return 2;
    case 7682:
        return 3;
    case 7683:
        return 4;
    case 34055:
        return 6;
    case 34056:
        return 7;
    default:
        return 0;
    }
}

std::uint32_t GetRenderProfileId(std::int32_t value) {
    if (value == 1)
        return 1;
    if (value >= 12 && value <= 17)
        return static_cast<std::uint32_t>(value - 1);
    if (value >= 768 && value <= 776)
        return static_cast<std::uint32_t>(value - 766);
    return 0;
}

std::uint32_t EncodeFourWayMode(std::int32_t value) {
    return value >= 2 && value <= 5 ? static_cast<std::uint32_t>(value - 1) : 0U;
}

} // namespace

void SetDirectXWindowConfig(std::int32_t offsetX, std::int32_t offsetY, std::int32_t width, std::int32_t height, float globalX, float globalY) {
    const std::array<float, 6> values{
        static_cast<float>(offsetX),
        static_cast<float>(offsetY),
        static_cast<float>(width),
        static_cast<float>(height),
        globalX,
        globalY,
    };
    auto *const screenWords = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(&g_screenWindow) + 8);
    for (std::size_t index = 0; index < values.size(); ++index) {
        screenWords[index] = std::bit_cast<std::uint32_t>(values[index]);
    }

    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    auto *const bytes = context->renderState.data();
    constexpr std::size_t kWindowValuesOffset = 0x870;
    if (std::memcmp(bytes + kWindowValuesOffset, values.data(), sizeof(values)) != 0) {
        bytes[0x974] |= std::byte{1};
        std::memcpy(bytes + kWindowValuesOffset, values.data(), sizeof(values));
    }
}

void InitRenderer() {
    g_legacyAlphaFlags[0] = 0xf0000000U;
    g_legacyAlphaFlags[1] = 0;
    g_legacyCurrentRenderingMode = 1;
    g_legacyEngineFlags = 0;
    g_legacyEngineField8 = 0;
    g_legacyRendererDefaults = {
        1, 1, 0, 0, 770, 771, 2304, 516, 516, 0, 1, 1, 0, 1.0f,
        0, 7680, 7680, 7680, 519, 0, -1, 15, 1, 15,
    };
    g_defaultRenderFlags.fill(0);
    g_defaultRenderFlags[2] = -1;
    g_defaultRenderFlags[3] = 15;
    g_defaultRenderFlags[7] = 1;
    g_defaultRenderFlags[16] = 1;
    g_defaultRenderFlags[38] = std::bit_cast<std::int32_t>(1.0f);
    g_defaultRenderFlags[53] = 7680;
    g_defaultRenderFlags[54] = 7680;
    g_defaultRenderFlags[55] = 7680;
    g_defaultRenderFlags[56] = 519;
    g_defaultRenderFlags[57] = 516;
    g_defaultRenderFlags[58] = 516;
    g_defaultRenderFlags[62] = 770;
    g_defaultRenderFlags[63] = 771;
    g_defaultRenderFlags[64] = 1;
    g_defaultRenderFlags[67] = 15;
    g_defaultRenderFlags[92] = 1;
    g_defaultRenderFlags[102] = 1;
    g_defaultRenderFlags[103] = 1;
    g_defaultRenderFlags[136] = 1;
    g_defaultRenderFlags[141] = 1;
    g_defaultRenderFlags[142] = 1;
    g_defaultRenderFlags[143] = 1;
    g_defaultRenderFlags[147] = 2304;
    g_defaultRenderFlags[154] = std::bit_cast<std::int32_t>(1.0f);
    g_defaultRenderFlags[160] = std::bit_cast<std::int32_t>(1.0f);
    g_defaultRenderFlags[161] = 1;
    g_defaultRenderFlags[162] = -1;
    g_defaultRenderFlags[166] = std::bit_cast<std::int32_t>(64.0f);
    g_defaultRenderFlags[170] = std::bit_cast<std::int32_t>(1.0f);
    g_defaultRenderFlags[171] = 1;
    SetDefaultRenderFlags();
}

void SetRenderFlags(std::uint32_t setting, std::int32_t value) {
    switch (setting) {
    case 1: {
        g_legacyStencilReference = value;
        D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
        auto *const state = context->renderState.data();
        std::uint64_t oldFlags{};
        std::int32_t oldStencil{};
        std::memcpy(&oldFlags, state + 0x940, sizeof(oldFlags));
        std::memcpy(&oldStencil, state + 0x950, sizeof(oldStencil));
        if (oldFlags != g_legacyEngineFlags || oldStencil != value) {
            state[0x975] |= std::byte{0x10};
            std::memcpy(state + 0x940, &g_legacyEngineFlags, sizeof(g_legacyEngineFlags));
            std::memcpy(state + 0x948, &g_legacyEngineField8, sizeof(g_legacyEngineField8));
            std::memcpy(state + 0x950, &value, sizeof(value));
        }
        break;
    }
    case 2:
        g_legacyEngineFlags = (g_legacyEngineFlags & 0xfffffffffff807ffULL) | (static_cast<std::uint64_t>(static_cast<std::uint8_t>(value)) << 11U);
        break;
    case 3:
        g_legacyEngineFlags = (g_legacyEngineFlags & 0xfffffffff807ffffULL) | (static_cast<std::uint64_t>(static_cast<std::uint8_t>(value)) << 19U);
        break;
    case 7:
        g_legacyEngineFlags = (g_legacyEngineFlags & ~1ULL) | (value != 0);
        break;
    case 34:
        g_legacyShaderParameters.fogColour.x = static_cast<float>((static_cast<std::uint32_t>(value) >> 16U) & 0xffU) / 255.0f;
        g_legacyShaderParameters.fogColour.y = static_cast<float>((static_cast<std::uint32_t>(value) >> 8U) & 0xffU) / 255.0f;
        g_legacyShaderParameters.fogColour.z = static_cast<float>(static_cast<std::uint32_t>(value) & 0xffU) / 255.0f;
        break;
    case 52:
        g_legacyEngineFlags = (g_legacyEngineFlags & ~0x40ULL) | (static_cast<std::uint64_t>(value != 0) << 6U);
        break;
    case 53:
        g_legacyEngineFlags ^= (g_legacyEngineFlags ^ (static_cast<std::uint64_t>(GetShenmueEngineFlag(value)) << 31U)) & 0x780000000ULL;
        break;
    case 54:
        g_legacyEngineFlags ^= (g_legacyEngineFlags ^ (static_cast<std::uint64_t>(GetShenmueEngineFlag(value)) << 27U)) & 0x78000000ULL;
        break;
    case 55:
        g_legacyEngineFlags ^= (g_legacyEngineFlags ^ (static_cast<std::uint64_t>(GetShenmueEngineFlag(value)) << 35U)) & 0x7800000000ULL;
        break;
    case 56:
        g_legacyEngineFlags ^= (g_legacyEngineFlags ^ (static_cast<std::uint64_t>(GetShenmueEngineFlag_0(value)) << 7U)) & 0x780ULL;
        break;
    case 57:
        g_legacyEngineFlags ^= (g_legacyEngineFlags ^ (static_cast<std::uint64_t>(GetShenmueEngineFlag_0(value)) << 2U)) & 0x3cULL;
        break;
    case 58:
        g_legacyShaderParameters.alphaFunction = static_cast<std::uint32_t>(value - 512);
        break;
    case 59:
        g_legacyAlphaFlags[0] = (g_legacyAlphaFlags[0] & 0xfffffffeU) | (value != 0);
        break;
    case 60:
        g_legacyShaderParameters.alphaTestEnabled = static_cast<std::uint32_t>(value);
        break;
    case 61:
        g_legacyShaderParameters.alphaReference = static_cast<float>(value) / 255.0f;
        break;
    case 62:
        g_legacyAlphaFlags[0] ^= (g_legacyAlphaFlags[0] ^ (GetRenderProfileId(value) << 2U)) & 0x7cU;
        break;
    case 63:
        g_legacyAlphaFlags[0] ^= (g_legacyAlphaFlags[0] ^ (GetRenderProfileId(value) << 7U)) & 0xf80U;
        break;
    case 64:
        g_legacyEngineFlags = (g_legacyEngineFlags & ~2ULL) | (static_cast<std::uint64_t>(value != 0) << 1U);
        break;
    case 67:
        g_legacyAlphaFlags[0] = (g_legacyAlphaFlags[0] & 0x0fffffffU) | (static_cast<std::uint32_t>(value) << 28U);
        break;
    case 92:
        g_legacyShaderParameters.fogEnabled = static_cast<std::uint32_t>(value);
        break;
    case 103:
        g_legacyShaderParameters.specularEnabled = static_cast<std::uint32_t>(value);
        break;
    case 147: {
        const std::uint32_t mode = value == 2304 ? 1U : value == 2305 ? 2U : 0U;
        g_legacyCurrentRenderingMode = (g_legacyCurrentRenderingMode & ~0x18) | static_cast<std::int32_t>(mode << 3U);
        break;
    }
    case 149: {
        std::uint32_t encoded = 0U - static_cast<std::uint32_t>(value);
        if (value > 0)
            encoded = static_cast<std::uint32_t>(value) | 0x80U;
        const std::uint32_t current = static_cast<std::uint32_t>(g_legacyCurrentRenderingMode);
        g_legacyCurrentRenderingMode = static_cast<std::int32_t>(current ^ ((current ^ (encoded << 5U)) & 0x1fe0U));
        break;
    }
    case 171:
        g_legacyAlphaFlags[0] = (g_legacyAlphaFlags[0] & 0xffff8fffU) | (EncodeFourWayMode(value) << 12U);
        break;
    case 174:
        g_legacyCurrentRenderingMode = (g_legacyCurrentRenderingMode & ~4) | ((value != 0) ? 4 : 0);
        break;
    case 207:
        g_legacyAlphaFlags[0] ^= (g_legacyAlphaFlags[0] ^ (GetRenderProfileId(value) << 15U)) & 0xf8000U;
        break;
    case 208:
        g_legacyAlphaFlags[0] ^= (g_legacyAlphaFlags[0] ^ (GetRenderProfileId(value) << 20U)) & 0x1f00000U;
        break;
    case 209:
        g_legacyAlphaFlags[0] = (g_legacyAlphaFlags[0] & 0xf1ffffffU) | (EncodeFourWayMode(value) << 25U);
        break;
    case 210:
        g_legacyShaderParameters.fogColour.w =
            (g_legacyShaderParameters.fogColour.x != 0.0f || g_legacyShaderParameters.fogColour.y != 0.0f || g_legacyShaderParameters.fogColour.z != 0.0f) ?
            std::bit_cast<float>(value) :
            0.0f;
        break;
    default:
        break;
    }
}

void SetDefaultRenderFlags() {
    for (std::uint32_t setting = 0; setting < g_defaultRenderFlags.size(); ++setting) {
        SetRenderFlags(setting, g_defaultRenderFlags[setting]);
        g_currentRenderFlags[setting] = g_defaultRenderFlags[setting];
    }
}

void SetAlphaRenderFlags() {
    for (AlphaRenderDefaults &lane : g_alphaRenderDefaults) {
        lane = {};
        lane.values[0] = 1;
        lane.values[1] = 1;
        lane.values[3] = 2;
        lane.values[4] = 2;
        lane.values[5] = 2;
        lane.values[16] = 4;
        lane.values[17] = 1;
        lane.values[18] = 2;
        lane.values[19] = 1;
    }
    g_alphaRenderDefaults[0].values[19] = 0;
    SetDefaultAlphaFlags();
}

void SetAlphaFlags(std::uint32_t lane, std::uint32_t setting, std::uint32_t value) {
    if (lane >= g_engineAlphaState.size()) {
        return;
    }

    std::uint64_t &flags = g_engineAlphaState[lane].flags;
    switch (setting) {
    case 0:
        flags = (flags & 0xffffffffffffff3fULL) | (value == 1 ? 0ULL : value == 2 ? 0x40ULL : 0x80ULL);
        break;
    case 1:
        flags = (flags & 0xfffffffffffffcffULL) | (value == 1 ? 0ULL : value == 2 ? 0x100ULL : 0x200ULL);
        break;
    case 3:
        flags = (flags & 0xfffffffffffffffcULL) | (value != 1);
        break;
    case 4:
        flags = (flags & 0xfffffffffffffff3ULL) | (value == 1 ? 0ULL : 4ULL);
        break;
    case 5:
        flags = (flags & 0xffffffffffffffcfULL) | (value == 1 ? 0ULL : 0x10ULL);
        break;
    case 6:
        flags = (flags & 0xfffc00000003ffffULL) | (static_cast<std::uint64_t>(value) << 18U);
        break;
    case 16:
        g_alphaShaderParameters.operation = value;
        break;
    case 17:
        g_alphaShaderParameters.argument0 = value;
        break;
    case 18:
        g_alphaShaderParameters.argument1 = value;
        break;
    case 19:
        g_alphaShaderParameters.argument2 = value;
        break;
    default:
        break;
    }
}

void SetDefaultAlphaFlags() {
    for (std::uint32_t lane = 0; lane < g_alphaRenderDefaults.size(); ++lane) {
        const AlphaRenderDefaults &defaults = g_alphaRenderDefaults[lane];
        AlphaRenderDefaults &current = g_currentAlphaRenderFlags[lane];

        constexpr std::array<std::uint32_t, 10> order{
            16, 18, 19, 0, 1, 3, 4, 5, 6, 17,
        };
        for (const std::uint32_t setting : order) {
            const std::uint32_t value = static_cast<std::uint32_t>(defaults.values[setting]);
            SetAlphaFlags(lane, setting, value);
            current.values[setting] = defaults.values[setting];
        }
    }
}

void Create_2x2_Texture() {
    constexpr std::array<std::uint8_t, 16> pixels{
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };
    const Texture2DDescription description{
        2,
        2,
        1,
        1,
        2,
        0,
        0,
        1,
        0,
        0,
        pixels.data(),
    };
    g_rendererWhiteTexture = CreateTexture2D(&description);
}

Matrix4x4 g_returnTmpTransformMatrix{};

void Matrix4x4_RotateAndPush() {
    Matrix4x4_Push(g_matrixStack.entries[2].stack);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    g_returnTmpTransformMatrix = workTransformMatrix;
    Matrix4x4::Pop(1);
}

namespace {

Vector4 ApplyReturnTransform(const Vector3 &point) {
    Matrix4x4_Push(&g_returnTmpTransformMatrix);
    const Matrix4x4 &m = workTransformMatrix;
    const Vector4 result{m.rows[1].x * point.y + m.rows[0].x * point.x + m.rows[2].x * point.z + m.rows[3].x,
        m.rows[1].y * point.y + m.rows[0].y * point.x + m.rows[2].y * point.z + m.rows[3].y,
        m.rows[1].z * point.y + m.rows[0].z * point.x + m.rows[2].z * point.z + m.rows[3].z,
        m.rows[1].w * point.y + m.rows[0].w * point.x + m.rows[2].w * point.z + m.rows[3].w};
    return result;
}

float ProjectedInverseW(float w) {
    if (1.0e-07f > w)
        w += 1.0e-07f;
    return 1.0f / w;
}

} // namespace

void WorldToScreen(const Vector3 &world, Vector2 &screen) {
    const Vector4 projected = ApplyReturnTransform(world);
    const float inverse = ProjectedInverseW(projected.w);
    const float y = (1.0f - projected.y * inverse) * g_screenHeight * 0.5f + g_screenHeightOffset;
    screen.x = (projected.x * inverse + 1.0f) * g_screenWidth * 0.5f + g_screenWidthOffset;
    screen.y = y;
    Matrix4x4::Pop(1);
}

void ConstructWorldSpaceFromScreen(const Vector3 &world, Vector2 &screen) {
    const Vector4 projected = ApplyReturnTransform(world);
    const float inverse = ProjectedInverseW(projected.w);
    float aspect = g_screenWidth / g_screenHeight - 1.3333334f + 1.0f;
    aspect = aspect < 1.33f ? aspect : 1.33f;
    const float x = (projected.x * inverse + 1.0f) * g_screenWidth * 0.5f / g_screenWidth * 640.0f;
    const float y = (1.0f - projected.y * inverse) * g_screenHeight * 0.5f / g_screenHeight * 480.0f;
    screen.x = aspect * x + g_screenWidthOffset;
    screen.y = y + g_screenHeightOffset;
    Matrix4x4::Pop(1);
}

void SetupDirectXWindow(float widthOffset, float heightOffset, float screenWidth, float screenHeight) {
    SetDirectXWindowConfig(static_cast<std::int32_t>(widthOffset), static_cast<std::int32_t>(heightOffset), static_cast<std::int32_t>(screenWidth),
        static_cast<std::int32_t>(screenHeight), 0.0f, 1.0f);
    g_screenWidth = screenWidth;
    g_screenHeight = screenHeight;
    g_screenHalfWidth = screenWidth * 0.5f;
    g_screenHalfHeight = screenHeight * -0.5f;
    g_screenWidthOffset = widthOffset;
    g_screenHeightOffset = heightOffset;
    g_screenHalfWithOffset.x = g_screenHalfWidth + widthOffset;
    g_screenHalfWithOffset.y = screenHeight * 0.5f + heightOffset;
    g_audioLevelMatrixNormal = {0.0f, 0.0f, 1.0f, 0.0f};
    g_unusedScreenScale = 1.0f;
}

void Matrix4x4_SetupCameraProjectionMatrix(std::int32_t pitch, float aspectRatio, float nearClip, float farClip) {
    constexpr float kTwoPi = 6.2831855f;
    constexpr float kFixedAngleScale = 0.000015258789f;
    constexpr float kHalf = 0.5f;

    g_verticalFieldOfView = static_cast<float>(pitch) * kTwoPi * kFixedAngleScale;

    Matrix4x4 projection{};
    if (g_perspectiveProjection == 1) {
        const float tangent = std::tan(g_verticalFieldOfView * kHalf);
        const float yScale = 1.0f / tangent;
        projection.rows[0].x = yScale / aspectRatio;
        projection.rows[1].y = yScale;
        projection.rows[2].z = -1.0000499f;
        projection.rows[2].w = -1.0f;
        projection.rows[3] = {0.0f, 0.0f, -0.15000750124454498f, 0.0f};
    } else {
        std::int32_t halfPitch = pitch >> 1;
        if (((halfPitch - 0x4000) & 0xffff7fff) == 0) {
            halfPitch = (halfPitch & ~0xffff) | static_cast<std::uint16_t>(halfPitch - 1);
        }
        const float tangent = std::tan(static_cast<float>(static_cast<std::uint16_t>(halfPitch)) * kTwoPi * kFixedAngleScale);
        const float yScale = 1.0f / tangent;
        projection.rows[0].x = yScale / aspectRatio;
        projection.rows[1].y = yScale;
        projection.rows[2].z = nearClip / (farClip - nearClip);
        projection.rows[2].w = -1.0f;
        projection.rows[3].z = (nearClip * farClip) / ((farClip - nearClip) * 0.40000001f);
    }

    Matrix4x4::Multiply(&projection);
    g_cameraProjectionMatrix = workTransformMatrix;
    g_tempExtentNearClip = 1.0f / nearClip;
    g_tempNearClip = nearClip;
    g_tempFarClip = farClip;
}

bool QueueDebugText(float x, float &y, std::string_view text) {
    if (text.empty() || g_debugTextQueueUsed + text.size() + 35 >= g_debugTextQueue.size())
        return false;
    const float widthScale = g_screenWidth / 640.0f;
    const float heightScale = g_screenHeight / 480.0f;
    const DebugTextRecordHeader header{
        x,
        y,
        g_debugTextColour,
        static_cast<std::uint32_t>(text.size()),
        9.0f * widthScale,
        18.0f * heightScale,
        10.0f * widthScale,
        16.0f * heightScale,
    };
    std::byte *const destination = g_debugTextQueue.data() + g_debugTextQueueUsed;
    const std::size_t recordSize = text.size() + 35;
    std::memset(destination, 0, recordSize);
    std::memcpy(destination, &header, sizeof(header));
    std::memcpy(destination + sizeof(header), text.data(), text.size());
    for (const char character : text) {
        if (character == '\n')
            y += 1.0f;
    }
    g_debugTextQueueUsed += recordSize;
    ++g_debugTextQueueCount;
    return true;
}

void ClearDebugTextQueue() noexcept {
    g_debugTextQueueUsed = 0;
    g_debugTextQueueCount = 0;
}

const std::byte *GetDebugTextQueueData() noexcept {
    return g_debugTextQueue.data();
}

std::size_t GetDebugTextQueueSize() noexcept {
    return g_debugTextQueueUsed;
}

std::uint32_t GetDebugTextQueueCount() noexcept {
    return g_debugTextQueueCount;
}

namespace {


void SetCurrentRenderFlag(std::uint32_t setting, std::int32_t value) {
    SetRenderFlags(setting, value);
    g_currentRenderFlags[setting] = value;
}

void SetCurrentAlphaFlag(std::uint32_t setting, std::int32_t value) {
    SetAlphaFlags(0, setting, static_cast<std::uint32_t>(value));
    g_currentAlphaRenderFlags[0].values[setting] = value;
}



void CommitAndDrawGeometryVertices() {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    std::byte *const state = context->renderState.data();
    std::uint32_t topology = 0;
    std::memcpy(&topology, state + 0x93c, sizeof(topology));
    if (topology != 1U) {
        state[0x976] |= std::byte{1};
        topology = 1U;
        std::memcpy(state + 0x93c, &topology, sizeof(topology));
    }
    ShaderParams &parameters = g_renderParams;
    parameters.fogColour = g_legacyShaderParameters.fogColour;
    parameters.fogEnabled = g_legacyShaderParameters.fogEnabled;
    parameters.alphaTestEnabled = g_legacyShaderParameters.alphaTestEnabled;
    parameters.alphaReference = g_legacyShaderParameters.alphaReference;
    parameters.alphaFunction = g_legacyShaderParameters.alphaFunction;
    parameters.alphaOperation = g_alphaShaderParameters.operation;
    parameters.alphaArgument0 = g_alphaShaderParameters.argument0;
    parameters.alphaArgument1 = g_alphaShaderParameters.argument1;
    parameters.alphaArgument2 = g_alphaShaderParameters.argument2;
    parameters.specularEnabled = g_legacyShaderParameters.specularEnabled;
    CommitLegacyScreenRenderState(context, false);
#if defined(_WIN32)
    const std::uint32_t vertexCount = static_cast<std::uint32_t>(g_numVertices);
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    if (auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext))
        native->Draw(vertexCount, 0);
#endif
}

} // namespace

void SetRenderAndAlphaFlags(std::uint32_t textureWord) {
    constexpr std::array<std::int32_t, 8> kPvrSourceBlendToRenderSetting{1, 0, 774, 775, 770, 771, 772, 773};
    constexpr std::array<std::int32_t, 8> kPvrDestinationBlendToRenderSetting{1, 0, 768, 769, 770, 771, 772, 773};
    constexpr std::array<std::int32_t, 6> kPvrUvModeToSamplerAddress{1, 3, 0, 0, 2, 3};
    SetCurrentRenderFlag(0x3e, kPvrSourceBlendToRenderSetting[textureWord >> 29U]);
    SetCurrentRenderFlag(0x3f, kPvrDestinationBlendToRenderSetting[(textureWord >> 26U) & 7U]);
    SetCurrentAlphaFlag(0, kPvrUvModeToSamplerAddress[(textureWord >> 16U) & 5U]);
    SetCurrentAlphaFlag(1, kPvrUvModeToSamplerAddress[(textureWord >> 15U) & 5U]);
    switch ((textureWord >> 13U) & 3U) {
    case 0:
        SetCurrentAlphaFlag(3, 1);
        SetCurrentAlphaFlag(4, 1);
        SetCurrentAlphaFlag(5, 1);
        break;
    case 2:
        SetCurrentAlphaFlag(3, 2);
        SetCurrentAlphaFlag(4, 2);
        SetCurrentAlphaFlag(5, 1);
        break;
    default:
        SetCurrentAlphaFlag(3, 2);
        SetCurrentAlphaFlag(4, 2);
        SetCurrentAlphaFlag(5, 2);
        break;
    }
    switch ((textureWord >> 6U) & 3U) {
    case 0:
        SetCurrentAlphaFlag(16, 3);
        SetCurrentAlphaFlag(19, 2);
        break;
    case 1:
        SetCurrentAlphaFlag(16, 2);
        SetCurrentAlphaFlag(18, 2);
        break;
    case 2:
        SetCurrentAlphaFlag(16, 3);
        SetCurrentAlphaFlag(19, 0);
        break;
    default:
        SetCurrentAlphaFlag(16, 4);
        SetAlphaFlags(0, 18, 2);
        g_currentAlphaRenderFlags[0].values[18] = 2;
        g_currentAlphaRenderFlags[0].values[19] = 0;
        SetAlphaFlags(0, 19, 0);
        break;
    }
}



void SetRenderAndAlphaFlags_0() {
    BindLegacyPixelTexture(DX11::g_ppD3DDeviceContext, g_rendererWhiteTexture);
    for (const std::uint32_t setting : {0x93U, 0x67U, 0x66U, 0x39U, 0x40U, 0x3eU, 0x3fU, 0x42U})
        SetCurrentRenderFlag(setting, g_defaultRenderFlags[setting]);
    for (const std::uint32_t setting : {16U, 18U, 19U, 0U, 1U, 3U, 4U, 5U})
        SetCurrentAlphaFlag(setting, g_alphaRenderDefaults[0].values[setting]);
}

bool CreateAlphaTextureForParticleEffects(const std::uint32_t *words) {
    if (!words)
        return false;
    SetCurrentRenderFlag(0x67, static_cast<std::int32_t>((words[0] >> 2U) & 1U));
    SetCurrentRenderFlag(0x93, (words[1] & 0x18000000U) != 0U ? 2305 : 0);
    SetCurrentRenderFlag(0x40, (words[1] & 0x4000000U) == 0U ? 1 : 0);
    SetRenderAndAlphaFlags(words[2]);
    if ((words[0] & 8U) != 0U && words[3])
        BindLegacyPixelTexture(DX11::g_ppD3DDeviceContext, ResolveStripTextureHandle(words[3]));
    else
        BindLegacyPixelTexture(DX11::g_ppD3DDeviceContext, g_rendererWhiteTexture);
    return true;
}

void DrawVertices() {
    if (g_numVertices <= 0)
        return;
    SetRenderFlags(0x3c, 0);
    g_currentRenderFlags[0x3b] = 0;
    g_currentRenderFlags[0x3c] = 0;
    SetRenderFlags(0x3b, 0);
    CommitAndDrawGeometryVertices();
}

void DrawVertices_HN() {
    if (g_numVertices <= 0)
        return;
    SetCurrentRenderFlag(0x3b, 1);
    CommitAndDrawGeometryVertices();
    SetCurrentRenderFlag(0x3b, g_defaultRenderFlags[0x3b]);
}

void DrawQueuedVertices_HN() {
    if (g_numVertices <= 0)
        return;
    SetRenderFlags(0x3d, 0);
    g_currentRenderFlags[0x3c] = 1;
    g_currentRenderFlags[0x3d] = 0;
    SetCurrentRenderFlag(0x3a, 516);
    SetRenderFlags(0x3c, 1);
    CommitAndDrawGeometryVertices();
    SetCurrentRenderFlag(0x3d, g_defaultRenderFlags[0x3d]);
    SetCurrentRenderFlag(0x3a, g_defaultRenderFlags[0x3a]);
    SetCurrentRenderFlag(0x3c, g_defaultRenderFlags[0x3c]);
}

const std::array<void (*)(), 3> g_mainDrawingFunctions{DrawVertices, DrawVertices_HN, DrawQueuedVertices_HN};

} // namespace shenmue::s1
